#include "RadioButton.h"

#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"


#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Client.h>
extern cltCommonLogic *pclClient;



CRadioButton::CRadioButton( CControllerMgr *pParent ) : CCheckBox( pParent )
{
	m_siGroupID = -1;
}

CRadioButton::~CRadioButton()
{
}

void CRadioButton::Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight, SI16 GroupID )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_RADIOBUTTON, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width - RADIOBUTTON_SIZE, Height, SurfaceWidth, SurfaceHeight );

	m_siGroupID = GroupID;

	SetText( Text, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	return;
}

bool CRadioButton::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
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
				m_bLPressed = true;

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				return true;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				if( IsLPressed() )
				{
					SetLPressed( false );
					SetCheck( true, GetControllerKind() );
					GetParent()->SetRadioButtonCheck( GetControllerID(), GetGroupID() );

					return true;
				}
			}
		}
		break;
	};

	return false;
}

void CRadioButton::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient*)pclClient;

	// 라디오 버튼 배경 그려주기
	TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( RADIOBUTTON_BUTTON_FILENAME );
	
	if ( pSpr )
	{
		GP.PutSpr( pSpr,BaseXPos + GetX(), BaseYPos + GetY(), RADIOBUTTON_BUTTON_FONTINDEX );
	}

	// 라디오 버튼 체크 이미지 그려 주기
	if ( IsCheck() && pSpr )
	{
		GP.PutSpr( pSpr,BaseXPos + GetX(), BaseYPos + GetY(), RADIOBUTTON_BUTTON_FONTINDEX + 4 );
	}
	
	// Text 출력
	if ( pSpr )
	{	
		DrawStaticText( BaseXPos + RADIOBUTTON_SIZE, BaseYPos );
	}
	else
	{
		DrawStaticText( BaseXPos, BaseYPos );
	}

	return;
}

void CRadioButton::SetNonEventOneButtonCheck(bool bCheck)
{
	m_bCheck = bCheck;
}

void CRadioButton::SetNonEventCheck()
{	
	SetNonEventOneButtonCheck(true);	
	GetParent()->SetNonEventRadioButtonCheck( GetControllerID(), GetGroupID() );
}