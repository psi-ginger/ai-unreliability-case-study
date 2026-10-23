#include "Static.h"

#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "../ControllerSurfaceObj.h"

#include <Client.h>
extern cltCommonLogic *pclClient;

CStatic::CStatic( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_bSetText = false;
	memset( m_strText, 0, MAX_STATIC_TEXT_LEN );
	SetTextPrintOption( DT_LEFT | DT_TOP | DT_WORDBREAK );

	m_ImgEncBuffer = NULL;

	m_bSetDrawRect = false;
	memset( &m_rcDrawRect, 0, sizeof( RECT ) );

	ZeroMemory( &m_StringSize, sizeof(m_StringSize) );
}

CStatic::~CStatic()
{
	m_bSetText = false;	

	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}
}

void CStatic::Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_STATIC, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	SetText( Text, GetTextPrintOption() );

	return;
}

TCHAR *CStatic::GetText()
{
	if ( m_bSetText )
	{
		return m_strText;
	}

	return NULL;
}

void CStatic::SetText( TCHAR *Text, SI32 TextPrintOption /* = -1 */, LPSIZE lpStringSize /* = NULL  */)
{
	if ( Text == NULL || m_strText == NULL )
	{
		m_bSetText = false;
		return;
	}

	if ( _tcscmp( Text, m_strText ) == 0 && TextPrintOption == GetTextPrintOption() )
	{
		if ( lpStringSize != NULL)
		{
			lpStringSize->cx = m_StringSize.cx;
			lpStringSize->cy = m_StringSize.cy;
		}
		return;
	}

	_tcsncpy( m_strText, Text, MAX_STATIC_TEXT_LEN - 1 );
	m_strText[ MAX_STATIC_TEXT_LEN - 1 ] = NULL;

	if ( TextPrintOption != -1 )
	{
		SetTextPrintOption( TextPrintOption );
	}
	
	MakeTextImgEncBuffer( lpStringSize );

	return;
}

void CStatic::MakeTextImgEncBuffer(LPSIZE lpStringSize /* = NULL */)
{
	m_bSetText = false;

	SI16 SurfaceWidth = GetWidth();
	SI16 SurfaceHeight = GetHeight();


	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			HDC hDC;
			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				cltClient *pclclient = (cltClient *)pclClient;
				HFONT hFont;

				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, GetFontKind() );
				}
				

				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetTextColor( hDC, GetFontColor() );

				SetBkMode( hDC, TRANSPARENT );

				DrawBorder( hDC, 0, 0, SurfaceWidth, SurfaceHeight );

				if( ! m_bSetDrawRect )
				{
					SetRect( &m_rcDrawRect, 1, 1, SurfaceWidth - 2, SurfaceHeight - 2 );
				}

				DrawText( hDC, m_strText, -1, &m_rcDrawRect, GetTextPrintOption() );
				
				//[2007.12.20 김영훈 추가 : 현재 들어온 문자의 가로,세로 크기를 리턴]
				if (lpStringSize != NULL)
				{
					GetTextExtentPoint32( hDC, m_strText, _tcslen(m_strText), lpStringSize );

					m_StringSize.cx = lpStringSize->cx;
					m_StringSize.cy = lpStringSize->cy;
				}
				
				SelectObject( hDC, hOldFont );

				if ( hFont )
				{
					DeleteObject( hFont );
					hFont = NULL;
				}

				lpSurface->ReleaseDC( hDC );
			}

			if ( GP.LockSurface( lpSurface ) )
			{
				if(m_ImgEncBuffer)
				{
					int size;
					g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
				}

				GP.UnlockSurface( lpSurface );
			}

			m_bSetText = true;
		}
	}

	return;
}

void CStatic::Refresh()
{
	MakeTextImgEncBuffer();

	return;
}

void CStatic::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{	
	cltClient *pclclient = (cltClient *)pclClient;

	TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

	if ( pBkImg )
	{
		if ( IsBKTileImg() )
		{
			DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex(), GetAlphaValue(), BaseXPos + GetX(), BaseYPos + GetY(), GetSurfaceWidth(), GetSurfaceHeight(), GetBKTileImgNum() );
		}
		else
		{
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetFontIndex() );						break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetAlphaValue(), GetFontIndex() );	break;
			case SPR_PUTSPRSCREENT:	GP.PutSprScreen(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetFontIndex() );					break;
			}
		}
	}

	DrawStaticText( BaseXPos, BaseYPos );
}

void CStatic::DrawStaticText( SI16 BaseXPos, SI16 BaseYPos )
{
	if ( m_bSetText && m_ImgEncBuffer )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		switch( GetDrawOption() )
		{
		case SPR_PUTSPRT:
			{
				GP.PutXpr16( BaseXPos + GetX(), BaseYPos + GetY(), 
							 GetWidth(), GetHeight(),
							 m_ImgEncBuffer,
							 RGB16_TRANS );
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				GP.PutXpr16A( BaseXPos + GetX(), BaseYPos + GetY(), 
							  GetWidth(), GetHeight(),
							  m_ImgEncBuffer,
							  GetAlphaValue(),
							  RGB16_TRANS );
			}
			break;
		}
	}
}

void CStatic::SetDrawRect( RECT Rect )
{
	m_bSetDrawRect = true;
	m_rcDrawRect = Rect;
}