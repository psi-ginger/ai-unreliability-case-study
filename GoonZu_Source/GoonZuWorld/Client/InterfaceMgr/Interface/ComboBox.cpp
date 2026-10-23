#include "ComboBox.h"
#include "../ControllerMgr.h"
#include "../ControllerObj.h"
#include "../ControllerSurfaceObj.h"
#include "../InterfaceMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <zmouse.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

CComboBox::CComboBox( CControllerMgr *pParent ) : CButton( pParent )
{
	m_siComboBoxHeight = 0;

	m_siComboBoxLineHeight = COMBOBOX_LINE_HEIGHT;

	m_siOffsetY = 0;

	m_bOpened = false;

	m_siSelectedIndex = -1;
	m_siFocusedIndex = -1;

	m_ImgEncBuffer =  NULL;

	for ( SI16 i = 0; i < MAX_COMBOBOX_ITEM_NUM; ++i )
	{
		m_pComboBoxItemDatas[ i ] = NULL;
	}

	m_siCurrentEmptyComboBoxIndex = 0;

	SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	m_bReverseDraw = false;

	m_siDropDownYPos = 0;
	m_siDropDownYSize = 0;

	// SetBorder( true );
	// SetBKColorUse( true );

	m_strComboBoxButtonFileName = new TCHAR[ MAX_PATH ];
	SetComboBoxButtonFileName( COMBOBOX_DROPDOWN_BUTTON_FILENAME );
	m_siComboBoxButtonFontIndex = COMBOBOX_DROPDOWN_BUTTON_FONTINDEX;

	m_strDropDownImageFileName = new TCHAR[ MAX_PATH ];
	StringCchCopy( m_strDropDownImageFileName, MAX_PATH, COMBOBOX_OPEN_BACKGROUND_FILENAME );
	m_siDropDownImageFontIndex = COMBOBOX_OPEN_BACKGROUND_FONTINDEX;
	m_bDropDownImageTileImg = true;
	m_siDropDownImageTileNum = 9;


	m_strFocusImageFileName = new TCHAR[ MAX_PATH ];
	StringCchCopy( m_strFocusImageFileName, MAX_PATH, COMBOBOX_FOCUSIMAGE_FILENAME );
	m_siFocusImageFontIndex = 3;
	m_bFocusImageTileImg = true;
	m_siFocusImageTileNum = 3;
	m_siFocusImageAlphaValue = COMBOBOX_FOCUSIMAGE_ALPHA_VALUE;

	m_crDropDownBKColor = CONTROLLER_BACKGROUND_COLOR;
	m_crDropDownLineColor = CONTROLLER_BORDER_COLOR;

	m_pScroll = new CScroll( pParent );

	m_bEnableWheel = true;
}

CComboBox::~CComboBox()
{
	SAFE_DELETE_A( m_strComboBoxButtonFileName );
	SAFE_DELETE_A( m_strDropDownImageFileName );
	SAFE_DELETE_A( m_ImgEncBuffer );

	for ( SI16 i = 0; i < MAX_COMBOBOX_ITEM_NUM; ++i )
	{
		SAFE_DELETE( m_pComboBoxItemDatas[ i ] );
	}

	SAFE_DELETE( m_pScroll );

	//cyj m_strFocusImageFileName 메모리해제
	SAFE_DELETE_A( m_strFocusImageFileName );
}

void CComboBox::Create( SI16 ControllerID, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight, SI16 ComboBoxHeight, SI16 ComboBoxLineHeight, SI16 OffsetY )
{
	if ( SurfaceHeight < ComboBoxHeight + ComboBoxLineHeight )
	{
		SurfaceHeight = ComboBoxHeight + ComboBoxLineHeight;
#ifdef _DEBUG
		OutputDebugString(  TEXT("ComboBox 높이 자동 수정\r\n") );
#endif
	}

	m_siComboBoxHeight = ComboBoxHeight;
	m_siOffsetY = OffsetY;

	CControllerObj::Create( ControllerID, CONTROLLER_KIND_COMBOBOX, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	if ( m_bReverseDraw )
	{	
		m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, XPos + Width - SCROLL_WIDTH - 1, YPos + Height - SurfaceHeight, SCROLL_WIDTH, ComboBoxHeight, SCROLL_WIDTH, ComboBoxHeight, m_siComboBoxLineHeight );
	}
	else
	{
		m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, XPos + Width - SCROLL_WIDTH - 1, YPos + Height, SCROLL_WIDTH, ComboBoxHeight, SCROLL_WIDTH, ComboBoxHeight, m_siComboBoxLineHeight );
	}

	return;
}

void CComboBox::Clear()
{

	for ( SI16 i = 0; i < MAX_COMBOBOX_ITEM_NUM; ++i )
	{
		SAFE_DELETE( m_pComboBoxItemDatas[ i ] );
	}

	m_siCurrentEmptyComboBoxIndex = 0;


	m_pScroll->SetTrackRange( 0, 0 );
	m_siFocusedIndex = m_siSelectedIndex = -1;
	SetText( TEXT(""), DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, false, this );

	return;
}

SI16 CComboBox::AddItem( stComboBoxItemData *pComboBoxItemData )
{
	if ( pComboBoxItemData == NULL )
	{
		return -1;
	}

	if ( m_siCurrentEmptyComboBoxIndex >= MAX_COMBOBOX_ITEM_NUM )
	{
		return -1;
	}


	SI16 RetVal = m_siCurrentEmptyComboBoxIndex;

	m_pComboBoxItemDatas[ m_siCurrentEmptyComboBoxIndex++ ] = new stComboBoxItemData( pComboBoxItemData );

	m_pScroll->SetTrackRange( 0, m_siCurrentEmptyComboBoxIndex );

	if( RetVal == 0 )
	{	
		m_siSelectedIndex = -1;
		m_siFocusedIndex = -1;
	}

	return RetVal;
}

void CComboBox::SetFocus( bool HasFocus )
{
	CButton::SetFocus( HasFocus );

	if ( ! HasFocus )
	{
		m_bOpened = false;
	}

	return;
}

bool CComboBox::IsInOpenedArea( SI32 XPos, SI32 YPos )
{
	if ( !IsShow() )
	{
		return false;
	}

	if ( m_bOpened )
	{
		if ( 0 < XPos && m_siDropDownYPos < YPos &&
			GetWidth() > XPos && m_siDropDownYPos + m_siDropDownYSize > YPos
			)
		{
			return true;
		}
	}

	return false;
}

bool CComboBox::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
	{
		return false;
	}


	POINT ComboBoxDropDownPt;
	ComboBoxDropDownPt.x = pt.x - GetX();
	ComboBoxDropDownPt.y = pt.y - GetY();

	/*
	if( m_pScroll->HandleMouse( uMsg, ComboBoxDropDownPt, wParam, lParam ) )
	return true;
	*/
	if( m_pScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( ! m_pScroll->IsFocus() )
		{
			GetParent()->RequestFocus( this );
		}
		return true;
	}

	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{	
			if( m_bOpened )
			{
				if ( IsInOpenedArea( ComboBoxDropDownPt.x, ComboBoxDropDownPt.y ) )
				{
					m_siFocusedIndex = m_pScroll->GetTrackPos() + ( ComboBoxDropDownPt.y - m_siDropDownYPos - m_siOffsetY ) / m_siComboBoxLineHeight;
					return true;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				m_bLPressed = true;
				//SetCapture( pclClient->GetHwnd() );

				if( !IsFocus() )
					GetParent()->RequestFocus( this );

				if( IsFocus() )
				{
					m_bOpened = !m_bOpened;
				
					if( !m_bOpened )
					{
						/*
						if( !GetParent()->IsKeyboardInput() )
						GetParent()->ClearFocus();
						*/

						m_siDropDownYPos = 0;
						m_siDropDownYSize = 0;
					}
					else
					{						
						Refresh();

						GetParent()->SendEvent( EVENT_COMBOBOX_OPEN , true, this );
					}
				}

				return true;
			}

			if( m_bOpened && IsInOpenedArea( ComboBoxDropDownPt.x, ComboBoxDropDownPt.y ) )
			{
				return true;
			}

			break;
		}
		break;

	case WM_LBUTTONUP:
		{
			/*
			if( m_bPressed && IsInArea( pt.x, pt.y ) )
			{
			m_bPressed = false;
			ReleaseCapture();				
			}
			*/

			if ( m_bLPressed && IsInArea( pt.x, pt.y ) )
			{
				m_bLPressed = false;
				return true;
			}

			if( m_bOpened )
			{
				if ( IsInOpenedArea( ComboBoxDropDownPt.x, ComboBoxDropDownPt.y ) )
				{
					m_siFocusedIndex = m_pScroll->GetTrackPos() + ( ComboBoxDropDownPt.y - m_siDropDownYPos - m_siOffsetY ) / m_siComboBoxLineHeight;

					if ( m_siFocusedIndex >= 0 )
					{
						stComboBoxItemData* pItem = m_pComboBoxItemDatas[ m_siFocusedIndex ];
						if ( pItem )
						{
							if ( pItem->strText )
							{
								SetText( pItem->strText, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
								m_siSelectedIndex = m_siFocusedIndex;
							}
							else
							{
								SetText( NULL, 0 );
								m_siFocusedIndex = -1;
								m_siSelectedIndex = -1;
							}

							GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, true, this );
						}
					}
					else
					{
						SetText( NULL, 0 );
						m_siFocusedIndex = -1;
						m_siSelectedIndex = -1;
						GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, true, this );
					}

					m_bOpened = false;

					return true;
				}
				else
				{
					SetText( NULL, 0 );
					m_siFocusedIndex = -1;
					m_siSelectedIndex = -1;
					GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, true, this );

					m_bOpened = false;
				}
			}
			else
			{	
				m_siFocusedIndex = -1;
				m_bOpened = false;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bEnableWheel == false)			return true;

			int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;

			if( m_bOpened )
			{	
				/*
				UINT uLines;
				SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &uLines, 0 );
				m_pScroll->Scroll( -zDelta * uLines );
				*/

				/*
				SI16 StartPos = m_pScroll->GetTrackPos();

				if ( zDelta )
				{
				if( m_siFocusedIndex > 0 )
				{
				if ( StartPos > 0 )
				{
				m_siFocusedIndex -= zDelta;
				}

				if ( m_siFocusedIndex < 0 )
				{
				m_siFocusedIndex = 0;
				}
				}
				}
				else
				{
				if( m_siFocusedIndex > 0 )
				{
				if ( StartPos < m_siCurrentEmptyComboBoxIndex - m_pScroll->GetPageSize() )
				{
				SI16 EndPos = StartPos + m_siComboBoxHeight / COMBOBOX_LINE_HEIGHT;

				if ( EndPos >= m_siCurrentEmptyComboBoxIndex )
				{
				EndPos = m_siCurrentEmptyComboBoxIndex - 1;
				}

				m_siFocusedIndex -= zDelta;

				if ( m_siFocusedIndex >= EndPos )
				{
				m_siFocusedIndex = EndPos;
				}
				}
				}
				}
				*/

				m_pScroll->Scroll( -zDelta );
			}
			else
			{
				if( zDelta > 0 )
				{
					if( m_siFocusedIndex > 0 )
					{
						--m_siFocusedIndex;
						m_siSelectedIndex = m_siFocusedIndex;

						stComboBoxItemData* pItem = m_pComboBoxItemDatas[ m_siSelectedIndex ];
						if ( pItem )
						{
							if ( pItem->strText )
							{	
								SetText( pItem->strText, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
							}
							else
							{
								SetText( NULL, 0 );
							}
						}
						else
						{
						}

						if( !m_bOpened )
							GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, true, this );
					}          
				}
				else
				{					
					if( m_siFocusedIndex + 1 < m_siCurrentEmptyComboBoxIndex )
					{
						++m_siFocusedIndex;
						m_siSelectedIndex = m_siFocusedIndex;

						stComboBoxItemData* pItem = m_pComboBoxItemDatas[ m_siSelectedIndex ];
						if ( pItem )
						{
							if ( pItem->strText )
							{
								SetText( pItem->strText, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
							}
							else
							{
								SetText( NULL, 0 );
							}
						}
						else
						{	
						}

						if( !m_bOpened )
							GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, true, this );
					}
				}
			}

			return true;
		}
		break;
	}

	return false;
}

void CComboBox::PrepareDropDownText()
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI16 SurfaceWidth = GetSurfaceWidth();
	SI16 SurfaceHeight = GetSurfaceHeight();

	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}

	if ( GetItemNum() < 1 )
	{
		return;
	}

	m_siDropDownYPos = 0;
	m_siDropDownYSize = m_siOffsetY;

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	/*
	SI16 AvailableHeight = m_siComboBoxHeight - ( m_siComboBoxHeight % SCROLL_BUTTON_HEIGHT );
	m_pScroll->SetScrollPos( m_rcDropDown.right - SCROLL_WIDTH - 1, m_rcDropDown.top, SCROLL_WIDTH, AvailableHeight );
	*/

	if ( pControllerSurfaceObj )
	{
		SI16 i = 0;
		SI16 j = 0;
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			m_ImgEncBuffer = new RGB16[ SurfaceWidth * ( SurfaceHeight - GetHeight() ) * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strDropDownImageFileName );

			if ( pSpr && GP.LockSurface( lpSurface ) )

			{
				if ( m_bDropDownImageTileImg )
				{
					if ( m_siComboBoxHeight < ( m_siCurrentEmptyComboBoxIndex - m_pScroll->GetTrackPos() ) * m_siComboBoxLineHeight )
					{
						DrawTileImg( pSpr, SPR_PUTSPRT, m_siDropDownImageFontIndex, 0, 0, 0, GetWidth(), m_siComboBoxHeight + m_siOffsetY, m_siDropDownImageTileNum );
					}
					else
					{
						DrawTileImg( pSpr, SPR_PUTSPRT, m_siDropDownImageFontIndex, 0, 0, 0, GetWidth(), ( m_siCurrentEmptyComboBoxIndex - m_pScroll->GetTrackPos() ) * m_siComboBoxLineHeight + m_siOffsetY, m_siDropDownImageTileNum );
					}
				}
				else
				{
					GP.PutSpr(pSpr, 0, 0, m_siDropDownImageFontIndex );
				}

				GP.UnlockSurface( lpSurface );
			}

			HDC hDC;
			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				/*
				if ( m_siComboBoxHeight < ( m_siCurrentEmptyComboBoxIndex - m_pScroll->GetTrackPos() ) * COMBOBOX_LINE_HEIGHT )
				{	
				DrawRoundRect( hDC, 0, 0, GetWidth(), m_siComboBoxHeight, true, true, m_crDropDownLineColor, m_crDropDownBKColor );
				}
				else
				{
				DrawRoundRect( hDC, 0, 0, GetWidth(), ( m_siCurrentEmptyComboBoxIndex - m_pScroll->GetTrackPos() ) * COMBOBOX_LINE_HEIGHT, true, true, m_crDropDownLineColor, m_crDropDownBKColor );
				}
				*/

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

				SI16 Counter = 0;

				for( i = m_pScroll->GetTrackPos(); i < m_siCurrentEmptyComboBoxIndex; i++ )
				{	
					stComboBoxItemData* pItem = m_pComboBoxItemDatas[ i ];

					if ( m_siComboBoxLineHeight + m_siDropDownYSize - 1 > m_siComboBoxHeight )
					{	
						continue;
					}

					RECT rect = { 3, m_siDropDownYSize, SurfaceWidth - 1, m_siComboBoxLineHeight + m_siDropDownYSize - 1 };
					m_siDropDownYSize += m_siComboBoxLineHeight;

					DrawText( hDC, pItem->strText, -1, &rect, GetTextPrintOption() );

					++Counter;
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
					g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight - GetHeight(), RGB16_TRANS, &size );
				}

				GP.UnlockSurface( lpSurface );
			}
		}
	}

	if ( GetParent()->GetY() + GetY() + GetHeight() + m_siDropDownYSize > pclclient->GetScreenYSize() - pclclient->GetScreenStatusBarYSize() )
	{
		m_siDropDownYPos = -m_siDropDownYSize;
		if ( m_siCurrentEmptyComboBoxIndex * m_siComboBoxLineHeight > m_siComboBoxHeight )
		{
			m_pScroll->SetScrollPos( GetX() + GetWidth() - SCROLL_WIDTH, GetY() + m_siDropDownYPos, SCROLL_WIDTH, m_siDropDownYSize );
		}

		m_bReverseDraw = true;
	}
	else
	{
		m_siDropDownYPos = GetHeight();
		if ( m_siCurrentEmptyComboBoxIndex * m_siComboBoxLineHeight > m_siComboBoxHeight )
		{
			m_pScroll->SetScrollPos( GetX() + GetWidth() - SCROLL_WIDTH, GetY() + m_siDropDownYPos, SCROLL_WIDTH, m_siDropDownYSize );
		}
		m_bReverseDraw = false;
	}


	return;
}

void CComboBox::Refresh()
{
	PrepareDropDownText();

	CStatic::Refresh();		// 텍스트도 리프레시한다.

	return;
}

void CComboBox::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	if ( m_bOpened && m_siCurrentEmptyComboBoxIndex * m_siComboBoxLineHeight > m_siComboBoxHeight )
	{	
		m_pScroll->Show( true );
	}
	else
	{
		m_pScroll->Show( false );
	}

	cltClient *pclclient = (cltClient *)pclClient;

	CButton::DrawController( BaseXPos, BaseYPos, LeftTime );

	return;
}

void CComboBox::DrawOpenedImage( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_bOpened )
	{
		if ( m_ImgEncBuffer )
		{
			GP.PutXpr16( BaseXPos + GetX(), BaseYPos + GetY() + m_siDropDownYPos, 
				GetSurfaceWidth(), GetSurfaceHeight() - GetHeight(),
				m_ImgEncBuffer,
				RGB16_TRANS );
		}

		if ( m_siFocusedIndex >= 0 )
		{	
			TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strFocusImageFileName );
			if ( pSpr )
			{
				if ( m_siFocusedIndex < m_pScroll->GetTrackPos() || m_siFocusedIndex >= m_pScroll->GetTrackPos() + m_pScroll->GetPageSize() )
				{
				}
				else
				{
					DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siFocusImageFontIndex, m_siFocusImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + m_siDropDownYPos + m_siOffsetY + ( m_siFocusedIndex - m_pScroll->GetTrackPos() ) * m_siComboBoxLineHeight, GetWidth(), m_siComboBoxLineHeight, m_siFocusImageTileNum );
				}
			}
		}
	}

	if ( m_pScroll->IsShow() )
	{
		m_pScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	return;
}

SI16 CComboBox::FindIndexFromText( TCHAR *Text )
{
	if ( Text == NULL || _tcslen( Text ) < 1 )
	{
		return -1;
	}

	for ( SI16 i = 0; i < m_siCurrentEmptyComboBoxIndex; ++i )
	{
		if ( m_pComboBoxItemDatas[ i ] && m_pComboBoxItemDatas[ i ]->strText )
		{
			if ( _tcscmp( m_pComboBoxItemDatas[ i ]->strText, Text ) == 0 )
			{
				return i;
			}
		}
	}

	return -1;
}

TCHAR *CComboBox::GetText( SI16 Index )
{
	if ( Index < 0 || Index >= MAX_COMBOBOX_ITEM_NUM )
	{
		return NULL;
	}

	if ( m_pComboBoxItemDatas[ Index ] )
	{
		return m_pComboBoxItemDatas[ Index ]->strText;
	}

	return NULL;
}

SI32 CComboBox::GetParam(SI16 Index )
{
	if ( Index < 0 || Index >= MAX_COMBOBOX_ITEM_NUM )
	{
		return 0;
	}

	if ( m_pComboBoxItemDatas[ Index ] )
	{
		return m_pComboBoxItemDatas[ Index ]->siParam;
	}

	return 0;
}
stComboBoxItemData *CComboBox::GetItemData( SI16 Index )
{
	if ( Index < 0 || Index >= MAX_COMBOBOX_ITEM_NUM )
	{
		return NULL;
	}

	if ( m_pComboBoxItemDatas[ Index ] )
	{
		return m_pComboBoxItemDatas[ Index ];
	}

	return NULL;
}

bool CComboBox::SetCursel( SI16 Index, bool bSendEvent /* = true */ )
{
	if ( Index == -1 )
	{
		m_siSelectedIndex = -1;
		m_siFocusedIndex = -1;
		if(bSendEvent)	GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, false, this );

		SetText( NULL, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		return true;
	}

	if ( Index < 0 || Index >= MAX_COMBOBOX_ITEM_NUM )
	{
		return false;
	}

	if ( m_pComboBoxItemDatas[ Index ] == NULL )
	{
		return false;
	}

	m_siFocusedIndex = Index;
	m_siSelectedIndex = Index;     

	if(bSendEvent)	GetParent()->SendEvent( EVENT_COMBOBOX_SELECTION_CHANGED, false, this );

	if( m_pComboBoxItemDatas[ Index ]->strText )
	{
		SetText( m_pComboBoxItemDatas[ Index ]->strText, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	}

	return true;
}