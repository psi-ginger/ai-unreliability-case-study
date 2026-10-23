#include "Button.h"
#include "Static.h"
#include "ImageButton.h"

#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

CImageButton::CImageButton( CControllerMgr *pParent ) : CButton( pParent )
{
	memset( m_strInImageFileName, 0, MAX_PATH );

	m_siInImageFontIndex = 0;
	m_siInImageXPos = 0;
	m_siInImageYPos = 0;
	m_siInTextXPos = 0;
	m_siInTextYPos = 0;
	m_siValue = 0;
}

CImageButton::~CImageButton()
{
}

void CImageButton::Create( SI16 ControllerID, TCHAR* Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_BUTTON, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	SetText( Text, GetTextPrintOption() );

	return;
}

void CImageButton::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
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
		iState = CONTROLLER_STATE_PRESSED;
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

					DrawTileImg( pBkImg, SPR_PUTSPRT, GetFontIndex() + AddFontIndex, 0, BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetWidth(), GetHeight(), BUTTON_TILE_IMAGE_NUM );
				}
				else
				{
					DrawTileImg( pBkImg, SPR_PUTSPRT, GetFontIndex() + ( iState * BUTTON_TILE_IMAGE_NUM ), 0, BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetWidth(), GetHeight(), BUTTON_TILE_IMAGE_NUM );
				}
			}
			else if ( GetControllerKind() == CONTROLLER_KIND_COMBOBOX )
			{	
				DrawTileImg( pBkImg, SPR_PUTSPRT, GetFontIndex(), 0, BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetWidth(), GetHeight(), BUTTON_TILE_IMAGE_NUM );
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

					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + AddFontIndex );
					break;
				case CONTROLLER_STATE_PRESSED:
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 1 );
					break;
				case CONTROLLER_STATE_MOUSEOVER:
					m_uiPreState = CONTROLLER_STATE_MOUSEOVER;
					AddFontIndex = ( iState + m_uiAniCounter++ ) * BUTTON_TILE_IMAGE_NUM;
					if ( m_uiAniCounter >= BUTTON_ANIMATION_NUM )
					{
						m_uiAniCounter = BUTTON_ANIMATION_NUM - 1;
					}
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 2 + m_uiAniCounter );
					break;
				case CONTROLLER_STATE_DISABLED:
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 5 );
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
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 0 );
					break;
				case CONTROLLER_STATE_PRESSED:
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 1 );
					break;
				case CONTROLLER_STATE_MOUSEOVER:
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 2 );
					break;
				case CONTROLLER_STATE_DISABLED:
					GP.PutSpr( pBkImg,BaseXPos + GetX() + nOffsetX, BaseYPos + GetY() + nOffsetY, GetFontIndex() + 3 );
					break;
				}
			}
		}
	}

	TSpr *pInImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strInImageFileName );

	if (pInImg)
	{
		GP.PutSpr( pInImg,BaseXPos + GetX() + m_siInImageXPos, BaseYPos + GetY() + m_siInImageYPos, m_siInImageFontIndex );
	}

	DrawStaticText( BaseXPos + nOffsetX + m_siInTextXPos, BaseYPos + nOffsetY + m_siInTextYPos );

	return;
}

void CImageButton::SetInImageFileName( TCHAR *InImageFileName )
{
	if (InImageFileName != NULL)
	{
		StringCchCopy( m_strInImageFileName, MAX_PATH, InImageFileName );
	}
}

void CImageButton::SetInImageFontIndex( SI16 InImageFontIndex)
{
	m_siInImageFontIndex = InImageFontIndex;
}

void CImageButton::SetInImagePos( SI32 XPos, SI32 YPos )
{
	m_siInImageXPos = XPos;
	m_siInImageYPos = YPos;
}

void CImageButton::SetInTextPos( SI32 XPos, SI32 YPos )
{
	m_siInTextXPos = XPos;
	m_siInTextYPos = YPos;
}

void CImageButton::SetValue( SI32 Value )
{
	m_siValue = Value;
}

SI32 CImageButton::GetValue()
{
	return m_siValue;
}