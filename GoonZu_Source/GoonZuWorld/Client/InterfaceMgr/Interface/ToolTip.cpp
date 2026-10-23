#include "ToolTip.h"

#include <client.h>

#include <Graphic.h>
#include <Graphic_Capsule.h>


#include "../InterfaceMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Interface/Edit.h"	// define 문을 가져오기 위해 포함

#include <Client.h>
extern cltCommonLogic *pclClient;

#define TOOLTIP_OFFSET_X		4
#define TOOLTIP_OFFSET_Y		4
#define TOOLTIP_LINE_SPACE		4

CToolTip::CToolTip()
{	
	m_lpSurface = NULL;
	m_bSetText = false;

	m_ImgEncBuffer = NULL;

	m_siDrawHeight = 0;
	m_siDrawWidth = 0;
	m_siFontWeight = 0;
	m_siFontHeight = BASE_FONT_HEIGHT;
	m_siFontWidth = 0;
	m_FontColor = RGB( 0, 0, 0 );

	m_siOffSet = 25;
}

CToolTip::~CToolTip()
{
	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}

	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}
}

void CToolTip::Create( SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	if( g_graphics.CreateSurface16( &m_lpSurface, SurfaceWidth, SurfaceHeight, TRUE, RGB16_TRANS ) == FALSE )
	{
		MsgBox( TEXT("CreateOffScreen Error"), TEXT("CToolTip::Create") );
		return;
	}

	m_siWidth = SurfaceWidth;
	m_siHeight = SurfaceHeight;

	return;
}

void CToolTip::SetToolTip( TCHAR *Text ,bool bColor)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(pclclient->m_pInterfaceMgr->m_bAlreadyOpenToolTip == TRUE)
	{
		return;
	}
	
	pclclient->m_pInterfaceMgr->m_bAlreadyOpenToolTip = true;


	if ( Text == NULL || m_strText == NULL )
	{
		m_bSetText = false;
		return;
	}

	if ( _tcsncmp( m_strText, Text, MAX_TOOLTIP_MSG_SIZE - 1 ) == 0 )
	{
		return;
	}

	_tcsncpy( m_strText, Text, MAX_TOOLTIP_MSG_SIZE );
  	m_strText[ MAX_TOOLTIP_MSG_SIZE - 1 ] = NULL;

	m_bSetText = true;

	
	

	if( bColor == true)
		MakeColorTextImgEncBuffer();		
	else
		MakeTextImgEncBuffer();
		
	
	return;
}

void CToolTip::MakeTextImgEncBuffer()	// 단색 텍스트 이미지 생성
{
	SAFE_DELETE_A( m_ImgEncBuffer );

	SI16 SurfaceWidth = m_siWidth;
	SI16 SurfaceHeight = m_siHeight;

 	
	if ( m_lpSurface )
 	{
		m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

		g_graphics.ClearSurface( m_lpSurface, RGB16_TRANS );

		HDC hDC;

		if( m_lpSurface->GetDC( &hDC ) == DD_OK)
		{
			HFONT hFont;
			cltClient *pclclient = (cltClient *)pclClient;
			if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
				hFont = CreateFont( m_siFontHeight, m_siFontWidth, 0, 0, m_siFontWeight, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, pclclient->m_pInterfaceMgr->GetFontKind() );
			}
			else {
				hFont = CreateFont( m_siFontHeight, m_siFontWidth, 0, 0, m_siFontWeight, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, pclclient->m_pInterfaceMgr->GetFontKind() );
			}
			

			HFONT hOldFont = NULL;

			hOldFont = (HFONT)SelectObject( hDC, hFont );

			SetTextColor( hDC, m_FontColor );
			SetTextColor( hDC, RGB(0,0,0) );
			SetBkMode( hDC, TRANSPARENT );

			SetRect( &m_rcToolTip, TOOLTIP_OFFSET_X, TOOLTIP_OFFSET_Y, SurfaceWidth - TOOLTIP_OFFSET_X - 1, SurfaceHeight - TOOLTIP_OFFSET_Y - 1 );

			m_siDrawHeight = DrawText( hDC, m_strText, -1, &m_rcToolTip, DT_LEFT | DT_TOP | DT_WORDBREAK ) + 3;

			DrawText( hDC, m_strText, -1, &m_rcToolTip, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );

			SelectObject( hDC, hOldFont );

			if ( hFont )
			{
				DeleteObject( hFont );
				hFont = NULL;
			}

			m_lpSurface->ReleaseDC( hDC );
		}

		if ( GP.LockSurface( m_lpSurface ) )
		{
			if(m_ImgEncBuffer)
			{
				int size;
				g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
			}

			GP.UnlockSurface( m_lpSurface );
		}

		m_siDrawWidth = m_rcToolTip.right - m_rcToolTip.left;

	}
	
	return;

}

void CToolTip::MakeColorTextImgEncBuffer()	// 중간중간 색이 들어간 텍스트 이미지 생성
{
   	SAFE_DELETE_A( m_ImgEncBuffer );

 	SI16 SurfaceWidth = m_siWidth;
	SI16 SurfaceHeight = m_siHeight;

	
	if ( m_lpSurface )
	{

		m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

		g_graphics.ClearSurface( m_lpSurface, RGB16_TRANS );

		SI16 i = 0, j = 0;

		HDC hDC;

		if( m_lpSurface->GetDC( &hDC ) == DD_OK)
		{
			cltClient *pclclient = ( cltClient * )pclClient;
            SI16 CurrentDrawXPos = TOOLTIP_OFFSET_X;
			SI16 CurrentDrawYPos = TOOLTIP_OFFSET_Y;
			HFONT hFont;
			if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
				hFont = CreateFont( m_siFontHeight, m_siFontWidth, 0, 0, m_siFontWeight, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, pclclient->m_pInterfaceMgr->GetFontKind() );
			}
			else {
				hFont = CreateFont( m_siFontHeight, m_siFontWidth, 0, 0, m_siFontWeight, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, pclclient->m_pInterfaceMgr->GetFontKind() );			}
			

			HFONT hOldFont = NULL;

			hOldFont = (HFONT)SelectObject( hDC, hFont );

			//SetTextColor( hDC, m_FontColor );
 			SetTextColor( hDC, RGB(0,0,0) );
 			 SetBkMode( hDC, TRANSPARENT );

			// m_rcToolTip을 근거로 보여줄 이미지의 가로 크기 m_siDrawWidth 구함
			SetRect( &m_rcToolTip, TOOLTIP_OFFSET_X, TOOLTIP_OFFSET_Y, SurfaceWidth - TOOLTIP_OFFSET_X - 1, SurfaceHeight - TOOLTIP_OFFSET_Y - 1 );
			
			// m_siDrawHeight 는 보여줄 이미지의 세로크기
			//m_siDrawHeight = DrawText( hDC, m_strText, -1, &m_rcToolTip, DT_LEFT | DT_TOP | DT_WORDBREAK );

			m_siDrawWidth = 0; // 가장 긴 문자 열을 찾아 가로 길이로 설정 하기 위해 초기화

 			SI16 StartPos = 0;  // 문자열의 시작 위치
			SI16 Counter = 0;  
		
			SI32	siLineLength = 0;
			SIZE sz;

			for ( i = StartPos; i <  (SI16)(_tcslen( m_strText )) ; ++i )
			{
				if( m_strText[ i ] == '\r' )
				{
 					if ( Counter > 0 )
					{	
						TextOut( hDC, CurrentDrawXPos , CurrentDrawYPos, &m_strText[ StartPos ], i - StartPos );
						m_siDrawWidth = max( m_siDrawWidth, CurrentDrawXPos + GetTextExtent( hDC, &m_strText[ StartPos ], i - StartPos ) );
					}
					CurrentDrawXPos = TOOLTIP_OFFSET_X;
					CurrentDrawYPos += m_siFontHeight + TOOLTIP_LINE_SPACE;
					StartPos = i + 2; // 1?
					i += 1;
					Counter = 0;

					if(pclClient->IsCountrySwitch(Switch_ToolTipAutoLine))		{	siLineLength = 0;		}
				}
				else if ( m_strText[ i ] == '<' )
				{  

					if ( Counter > 0 )
					{	
						m_siDrawWidth = max( m_siDrawWidth, CurrentDrawXPos + GetTextExtent( hDC, &m_strText[ StartPos ], i - StartPos ) );
						TextOut( hDC, CurrentDrawXPos, CurrentDrawYPos, &m_strText[ StartPos ], i - StartPos );
									
						//CurrentDrawYPos += m_siFontHeight + TOOLTIP_LINE_SPACE;
						CurrentDrawXPos += GetTextExtent( hDC, &m_strText[ StartPos ], i - StartPos );
					}
					
					int ConditionTextLen = ChangeText( hDC, i );
					
					i += ConditionTextLen;
					StartPos = i + 1;
					Counter = 0;

					if(pclClient->IsCountrySwitch(Switch_ToolTipAutoLine))		{	siLineLength = 0;		}
				}
				else
				{
					if(pclClient->IsCountrySwitch(Switch_ToolTipAutoLine))
					{
						// 자동 줄바꿈 기능 추가
						if(IsDBCSLeadByte(m_strText[ i ]))
						{
							GetTextExtentPoint32(hDC, &m_strText[ i ], 2, &sz);
							siLineLength += sz.cx == 0 ? 16 : sz.cx;
							++i;

							Counter += 2;
						}
						else
						{
							GetTextExtentPoint32(hDC, &m_strText[ i ], 1, &sz);
							siLineLength += sz.cx == 0 ? 8 : sz.cx;

							Counter += 1;
						}

						if(siLineLength > SurfaceWidth - 1 - 4)
						{
							if ( Counter > 0 )
							{	
								TextOut( hDC, CurrentDrawXPos , CurrentDrawYPos, &m_strText[ StartPos ], i - StartPos );
								m_siDrawWidth = max( m_siDrawWidth, CurrentDrawXPos + GetTextExtent( hDC, &m_strText[ StartPos ], i - StartPos ) );
							}
							CurrentDrawXPos = TOOLTIP_OFFSET_X;
							CurrentDrawYPos += m_siFontHeight + TOOLTIP_LINE_SPACE;
							StartPos = i - 1;
							Counter = 0;
							siLineLength = 0;
						}
					}
					else
						++Counter;
				}
			}

			if ( Counter > 0 )
			{	
 
					TextOut( hDC, CurrentDrawXPos, CurrentDrawYPos, &m_strText[ StartPos ], i - StartPos + 1  );
					CurrentDrawYPos += m_siFontHeight + TOOLTIP_LINE_SPACE;
					m_siDrawWidth = max( m_siDrawWidth, CurrentDrawXPos + GetTextExtent( hDC, &m_strText[ StartPos ], i - StartPos ) );
			}
			
			m_siDrawHeight = CurrentDrawYPos;

  			SetTextColor( hDC, RGB(0,0,0) );

			SelectObject( hDC, hOldFont );
			DeleteObject( hFont );

			m_lpSurface->ReleaseDC( hDC );
		}

		if ( GP.LockSurface( m_lpSurface ) )
		{
			if(m_ImgEncBuffer)
			{
				int size;
				g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
			}

			GP.UnlockSurface( m_lpSurface );
		}
	}


	return;

}

SI16 CToolTip::ChangeText( HDC hDC, SI32 nPos )
{
	switch ( m_strText[ nPos + 1 ] )
	{
	case 'B':
		{
			switch( m_strText[ nPos + 2 ] )
			{
			case '0':
				m_siFontWeight = FW_NORMAL;
				break;
			case '1':
				m_siFontWeight = FW_SEMIBOLD;
				break;
			case '2':
				m_siFontWeight = FW_BOLD;
				break;
			case '3':
				m_siFontWeight = FW_EXTRABOLD;
				break;
			case '4':
				m_siFontWeight =  FW_HEAVY ;
				break;
			default:
				OutputDebugString(  TEXT("Edit : No Parameter Error1\r\n") );
				break;
			}

			if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
				m_hFont = CreateFont( m_siFontHeight, m_siFontWidth, 0, 0, m_siFontWeight, 0, 0, 0, ((cltClient*)pclClient)->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, ((cltClient*)pclClient)->m_pInterfaceMgr->GetFontKind() );
			}
			else {
				m_hFont = CreateFont( m_siFontHeight, m_siFontWidth, 0, 0, m_siFontWeight, 0, 0, 0, ((cltClient*)pclClient)->GetCharSet(), 0, 0, 0, 0, ((cltClient*)pclClient)->m_pInterfaceMgr->GetFontKind() );
			}
			
			m_hOldFont = (HFONT)SelectObject( hDC, m_hFont );

			return EDIT_CHANGETEXTDRAW_CONDITION_UNINSTALL_TEXT_LEN;
		}
		break;
	case 'C':
		{
			TCHAR strNumber[ 3 ] = TEXT("");
			strNumber[ 0 ] = m_strText[ nPos + 2 ];
			strNumber[ 1 ] = m_strText[ nPos + 3 ];
			strNumber[ 2 ] = NULL;

			int ColorIndex = _tstoi( strNumber );
			m_FontColor =  GetChatEditColor( ColorIndex ) ;
			SetTextColor( hDC, m_FontColor );
		}
		break;
	case '/':
		{
			switch( m_strText[ nPos + 2 ] )
			{
			case 'B':
				m_siFontWeight = FW_NORMAL ;
				SelectObject( hDC, m_hOldFont );
				DeleteObject( m_hFont );
				break;
			case 'C':
				m_FontColor =   GetChatEditColor( CHATEDIT_COLOR_BLACK );
				SetTextColor( hDC, m_FontColor );
				break;
			default:
				OutputDebugString(  TEXT("Edit : No Command Error2\r\n") );
				break;
			}

			return EDIT_CHANGETEXTDRAW_CONDITION_UNINSTALL_TEXT_LEN;
		}
		break;
	default:
		{
			OutputDebugString(  TEXT("Edit : No Command Error1\r\n") );

			return 0;
		}
		break;
	}

	return EDIT_CHANGETEXTDRAW_CONDITION_INSTALL_TEXT_LEN;
}


void CToolTip::Draw( SI16 DrawXPos, SI16 DrawYPos )
{
	if ( m_bSetText && m_ImgEncBuffer )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		SI16 SurfaceWidth = m_siWidth;
		SI16 SurfaceHeight = m_siHeight;

		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( TOOLTIP_BACKGROUND_FILENAME );
		if ( pSpr )
		{
			SI32 DrawHeight = m_siDrawHeight + ( pSpr->GetYSize() - m_siDrawHeight % pSpr->GetYSize() );// + pSpr->GetYSize() * 2;
			SI32 DrawWidth = m_siDrawWidth + TOOLTIP_OFFSET_X * 2 + ( pSpr->GetXSize() - (m_siDrawWidth + TOOLTIP_OFFSET_X * 2 ) % pSpr->GetXSize() );// + pSpr->GetXSize() * 2;

			if ( DrawHeight > SurfaceHeight )
			{
				DrawHeight = SurfaceHeight;
			}

			if ( DrawWidth > SurfaceWidth )
			{
				DrawWidth = SurfaceWidth;
			}

			if ( DrawXPos + m_siOffSet + DrawWidth > pclclient->GetScreenXSize() )
			{
				DrawXPos = DrawXPos /*- m_siOffSet*/ - DrawWidth;
			}
			else
			{
				DrawXPos += m_siOffSet;
			}

			if ( DrawYPos + m_siOffSet + DrawHeight > pclclient->GetScreenYSize() - pclclient->GetScreenStatusBarYSize() )
			{
				DrawYPos = DrawYPos /*- m_siOffSet*/ - DrawHeight;
			}
			else
			{
				DrawYPos += m_siOffSet;
			}
			
			DrawTileImg( pSpr, SPR_PUTSPRALPHAT, TOOLTIP_BACKGROUND_FONTINDEX, 13, DrawXPos, DrawYPos, DrawWidth, DrawHeight, 9 );

			GP.PutXpr16( DrawXPos, DrawYPos, 
				DrawWidth, DrawHeight,
				m_ImgEncBuffer,
				RGB16_TRANS );
		}
	}

	return;
}

