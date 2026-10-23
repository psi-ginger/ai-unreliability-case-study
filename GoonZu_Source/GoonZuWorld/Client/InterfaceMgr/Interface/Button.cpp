#include "Button.h"
#include "Static.h"
#include "ToolTip.h"
#include "ComboBox.h"

#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"


#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

CButton::CButton( CControllerMgr *pParent ) : CStatic( pParent )
{
	m_bLPressed = false;
	m_bRPressed = false;
	m_bUseToolTip = false;
	m_strToolTipMsg = NULL;

	m_bAniType = false;
	m_uiAniCounter = 0;
	m_uiPreState = 0;

	SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
}

CButton::~CButton()
{
	if ( m_strToolTipMsg )
	{
		delete [] m_strToolTipMsg;
		m_strToolTipMsg = NULL;
	}
}

void CButton::Create( SI16 ControllerID, TCHAR* Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_BUTTON, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	SetText( Text, GetTextPrintOption() );

	return;
}

bool CButton::HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam )
{
	if ( ( ! IsAvailable() ) || ( ! IsFocus() ) )
	{
		return false;
	}

	switch( uMsg )
	{
		/*
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_SPACE:
			case VK_RETURN:
				m_bLPressed = true;
				return true;
			}
		}
		break;
		*/
	case WM_CHAR:
		{
			switch( wParam )
			{
			case VK_SPACE:
			case VK_RETURN:
				{
					m_bLPressed = false;
					GetParent()->SendEvent( EVENT_BUTTON_CLICKED, true, this );
 
					return true;
				}
				break;
			}
		}
		break;
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
		/*
	default:
		{	
			int temp = 0;
			temp++;
			TCHAR TempBuffer[128];
			sprintf( TempBuffer, TEXT("%d"), uMsg );
			OutputDebugStr( TempBuffer );
		}
		break;
		*/
	}

	return false;
}

bool CButton::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				m_bLPressed = true;
				SetCapture( pclClient->GetHwnd() );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}
				return true;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				//GetParent()->ClearFocus();
				GetParent()->SendEvent( EVENT_BUTTON_DBCLICKED, true, this );
			}
			return true;
		}
		break;
	case WM_LBUTTONUP:
		{
			if( m_bLPressed )
			{
				m_bLPressed = false;
				ReleaseCapture();
				
				if( IsInArea( pt.x, pt.y ) )
				{
					GetParent()->SendEvent( EVENT_BUTTON_CLICKED, true, this );
				}
				return true;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				m_bRPressed = true;
				SetCapture( pclClient->GetHwnd() );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}
				return true;
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			if( m_bRPressed )
			{
				m_bRPressed = false;
				ReleaseCapture();

				if( IsInArea( pt.x, pt.y ) )
				{
					GetParent()->SendEvent( EVENT_BUTTON_RCLICKED, true, this );
				}
				return true;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				// 마우스 오버 메시지 보냄
				//GetParent()->SendEvent( EVENT_BUTTON_MOUSEOVER, true, this );

				if ( m_bUseToolTip )
				{
					if ( m_strToolTipMsg )
					{
						cltClient *pclclient = (cltClient*)pclClient;
						pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTip( m_strToolTipMsg );
					}
				}
			}
		}
		break;
	};


	return false;
}

void CButton::OnHotkey()
{
	GetParent()->SendEvent( EVENT_BUTTON_CLICKED, true, this ); 
}

void CButton::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient*)pclClient;

	int nOffsetX = 0;
	int nOffsetY = 0;

	int iState = CONTROLLER_STATE_NORMAL;

	if( ! IsShow() )
	{		
		return;
	}
	else if( IsAvailable() == false )
	{
		iState = CONTROLLER_STATE_DISABLED;		
	}
	else if( m_bLPressed )
	{
		iState = CONTROLLER_STATE_PRESSED;
	}
	else if( IsMouseOver() )
	{
		iState = CONTROLLER_STATE_MOUSEOVER;
		
		if ( m_bUseToolTip )
		{
			if ( m_strToolTipMsg )
			{
				pclclient->m_pInterfaceMgr->SetDrawToolTip( true );
			}
		}
	}
	else if( IsFocus() )
	{
		iState = CONTROLLER_STATE_MOUSEOVER;
	}

	UI16 AddFontIndex = 0;

	if ( IsBKTileImg() )
	{
		TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

		if ( pBkImg )
		{
			if ( GetControllerKind() == CONTROLLER_KIND_BUTTON )
			{
				if( m_bAniType )
				{	
					switch( iState )
					{
					case CONTROLLER_STATE_NORMAL:
						{
							if ( m_uiPreState == CONTROLLER_STATE_MOUSEOVER )
							{
								--m_uiAniCounter;
								if ( m_uiAniCounter < 1 )
								{
									m_uiAniCounter = 0;
									m_uiPreState = 0;
								}

								AddFontIndex = ( CONTROLLER_STATE_MOUSEOVER + m_uiAniCounter ) * BUTTON_TILE_IMAGE_NUM;
							}
							else
							{
								m_uiPreState = 0;
								m_uiAniCounter = 0;
								AddFontIndex = iState * BUTTON_TILE_IMAGE_NUM;
							}
						}
						break;
					case CONTROLLER_STATE_PRESSED:
						m_uiPreState = 0;
						m_uiAniCounter = 0;
						AddFontIndex = iState * BUTTON_TILE_IMAGE_NUM;
						break;
					case CONTROLLER_STATE_MOUSEOVER:
						{
							m_uiPreState = CONTROLLER_STATE_MOUSEOVER;
							AddFontIndex = ( iState + m_uiAniCounter++ ) * BUTTON_TILE_IMAGE_NUM;
							if ( m_uiAniCounter >= BUTTON_ANIMATION_NUM )
							{
								m_uiAniCounter = BUTTON_ANIMATION_NUM - 1;
							}
						}
						break;
					case CONTROLLER_STATE_DISABLED:
						m_uiPreState = 0;
						m_uiAniCounter = 0;
						AddFontIndex = ( iState + BUTTON_ANIMATION_NUM - 1 ) * BUTTON_TILE_IMAGE_NUM;
						break;
					default:
						m_uiPreState = 0;
						m_uiAniCounter = 0;
						AddFontIndex = 0;
						break;
					}

					DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex() + AddFontIndex, GetAlphaValue(), BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetWidth(), GetHeight(), BUTTON_TILE_IMAGE_NUM );
				}
				else
				{
					DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex() + ( iState * BUTTON_TILE_IMAGE_NUM ), GetAlphaValue(), BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetWidth(), GetHeight(), BUTTON_TILE_IMAGE_NUM );
				}
			}
			else if ( GetControllerKind() == CONTROLLER_KIND_COMBOBOX )
			{	
				DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex(), GetAlphaValue(), BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetWidth(), GetHeight(), BUTTON_TILE_IMAGE_NUM );
			}
		}
	}
	else
	{
		TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

		if ( m_bAniType )
		{
			if ( pBkImg )
			{
				switch( iState )
				{
				case CONTROLLER_STATE_NORMAL:
					if ( m_uiPreState == CONTROLLER_STATE_MOUSEOVER )
					{
						--m_uiAniCounter;
						if ( m_uiAniCounter < 1 )
						{
							m_uiAniCounter = 0;
							m_uiPreState = 0;
						}

						AddFontIndex = 2 + m_uiAniCounter;
					}
					else
					{
						m_uiPreState = 0;
						m_uiAniCounter = 0;
						AddFontIndex = 0;
					}

					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + AddFontIndex );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + AddFontIndex );
					break;
				case CONTROLLER_STATE_PRESSED:
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 1 );
					else if ( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 1 );
					break;
				case CONTROLLER_STATE_MOUSEOVER:
					m_uiPreState = CONTROLLER_STATE_MOUSEOVER;
					AddFontIndex = ( iState + m_uiAniCounter++ ) * BUTTON_TILE_IMAGE_NUM;
					if ( m_uiAniCounter >= BUTTON_ANIMATION_NUM )
					{
						m_uiAniCounter = BUTTON_ANIMATION_NUM - 1;
					}
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 2 + m_uiAniCounter );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 2 + m_uiAniCounter );
					break;
				case CONTROLLER_STATE_DISABLED:
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 5 );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 5 );
					break;
				}
			}
		}
		else
		{
			if ( pBkImg )
			{
				switch( iState )
				{
				case CONTROLLER_STATE_NORMAL:
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 0 );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 0 );
					break;
				case CONTROLLER_STATE_PRESSED:
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 1 );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 1 );
					break;
				case CONTROLLER_STATE_MOUSEOVER:
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 2 );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 2 );
					break;
				case CONTROLLER_STATE_DISABLED:
					if( GetDrawOption() == SPR_PUTSPRT )
						GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 3 );
					else if( GetDrawOption() == SPR_PUTSPRALPHAT )
						GP.PutSprAlpha( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetAlphaValue(), GetFontIndex() + 3 );
					break;
				}
			}
		}
	}

	DrawStaticText( BaseXPos + nOffsetX, BaseYPos + nOffsetY );

	if( GetControllerKind() == CONTROLLER_KIND_COMBOBOX )
	{
		CComboBox *pComboBox = (CComboBox *)this;
		
		TSpr *pComboBoxDropDownButton = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( pComboBox->GetComboBoxButtonFileName() );
		if ( pComboBoxDropDownButton )
		{
			if( GetDrawOption() == SPR_PUTSPRT )
				GP.PutSpr(pComboBoxDropDownButton, BaseXPos + GetX() + nOffsetX + GetWidth() - pComboBoxDropDownButton->GetXSize() - 1, BaseYPos + GetY() + nOffsetY + 1, pComboBox->GetComboBoxButtonFontIndex() + iState );
			else if( GetDrawOption() == SPR_PUTSPRALPHAT )
				GP.PutSprAlpha(pComboBoxDropDownButton, BaseXPos + GetX() + nOffsetX + GetWidth() - pComboBoxDropDownButton->GetXSize() - 1, BaseYPos + GetY() + nOffsetY + 1, GetAlphaValue(), pComboBox->GetComboBoxButtonFontIndex() + iState );
		}
	}

	return;
}

void CButton::Enable(bool EnableOption )
{
	CControllerObj::Enable(EnableOption);
    
	if(EnableOption)
		SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
	else
		SetFontColor( CONTROLLER_DISABLE_BUTTONTEXT_COLOR );
	
	Refresh();
}


void CButton::SetUseToolTip( TCHAR *ToolTipMsg )
{
	if ( ToolTipMsg == NULL )
	{
		m_bUseToolTip = false;
		return;
	}
	else
	{
		m_bUseToolTip = true;
		if ( m_strToolTipMsg == NULL )
		{
			m_strToolTipMsg = new TCHAR[ MAX_PATH ];
		}
		memset( m_strToolTipMsg, 0, MAX_PATH );
		_tcsncpy( m_strToolTipMsg, ToolTipMsg, MAX_PATH );
		m_strToolTipMsg[ MAX_PATH - 1 ] = NULL;
	}

	return;
}