#include "CheckBox.h"

#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"


#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Client.h>
extern cltCommonLogic *pclClient;



CCheckBox::CCheckBox( CControllerMgr *pParent ) : CButton( pParent )
{
	m_bCheck = false;
}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::Create( SI16 ControllerID, TCHAR* Text, TCHAR*FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_CHECKBOX, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width - CHECKBOX_SIZE, Height, SurfaceWidth, SurfaceHeight );

	SetText( Text, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	return;
}

bool CCheckBox::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				SetLPressed( true );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				return true;
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				if( IsLPressed() )
				{
					SetLPressed( false );
					SetCheck( ! m_bCheck, GetControllerKind() );

					return true;
				}
			}

			break;
		}
	};

	return false;
}

void CCheckBox::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient*)pclClient;

	// 박스 그려주기
	TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( CHECKBOX_BUTTON_FILENAME );

	if ( pSpr )
	{
		GP.PutSpr(pSpr, BaseXPos + GetX(), BaseYPos + GetY(), CHECKBOX_BUTTON_FONTINDEX );
	}

	// 체크 이미지 그려 주기
	if ( IsCheck() && pSpr )
	{
		GP.PutSpr(pSpr, BaseXPos + GetX(), BaseYPos + GetY(), CHECKBOX_BUTTON_FONTINDEX + 1 );
	}
    
	// Text 출력
	if ( pSpr )
	{	
		DrawStaticText( BaseXPos + CHECKBOX_SIZE, BaseYPos );
	}
	else
	{
		DrawStaticText( BaseXPos, BaseYPos );
	}

	return;
}

void CCheckBox::SetCheck( bool Check, SI16 ControllerKind /* = CONTROLLER_KIND_CHECKBOX */, bool bSendEvent /* = true */ )
{
	m_bCheck = Check;

	if ( ControllerKind == CONTROLLER_KIND_CHECKBOX )
	{
		if(bSendEvent)		GetParent()->SendEvent( EVENT_CHECKBOX_CHANGED, true, this );
	}

	if ( ControllerKind == CONTROLLER_KIND_RADIOBUTTON )
	{
		if(bSendEvent)		GetParent()->SendEvent( EVENT_RADIOBUTTON_CHANGED, true, this );
	}
	
	return;	
}