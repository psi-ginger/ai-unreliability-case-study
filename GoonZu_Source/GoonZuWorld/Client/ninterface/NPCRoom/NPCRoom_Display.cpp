
#include "NPCRoom_Display.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "NPCRoom_Reward.h"

extern cltCommonLogic* pclClient;

CNPCRoom_Display::CNPCRoom_Display( void )
{
	// 1분
	m_UpdateTimer_Hide.Init( 60000 );
}

CNPCRoom_Display::~CNPCRoom_Display( void )
{

}

void CNPCRoom_Display::Init( )
{
		
}

void CNPCRoom_Display::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPCRoom/DLG_PCRoom_Display.ddf" ) );
	file.CreatePopupDialog( this, NPCROOM_DISPLAY_DLG, TEXT( "dialog_pcroom_display" ), StaticCallBackDialogNPCRoom_Display );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NPCROOM_DISPLAY_DLG_IMAGESTATIC_OPEN, this), NPCROOM_DISPLAY_DLG_IMAGESTATIC_OPEN, TEXT("imagestatic_open") );

	if ( g_bFullScreen )
	{
		SetDialogPos( 1053, 210, GetWidth(), GetHeight() );
	}
	else
	{
		SetDialogPos( 988, 210, GetWidth(), GetHeight() );
	}
	
	SetTimerOnOff( true );
	SetActionTimer( 1000 );

	Hide();

}

void CALLBACK CNPCRoom_Display::StaticCallBackDialogNPCRoom_Display( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPCRoom_Display* pThis = (CNPCRoom_Display*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNPCRoom_Display( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNPCRoom_Display::CallBackDialogNPCRoom_Display( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	switch ( nControlID )
	{
	case NPCROOM_DISPLAY_DLG_IMAGESTATIC_OPEN:
		{
			if ( EVENT_IMAGESTATIC_LBUTTONUP == nEvent )
			{
				// PC 방 보상항목 다이얼로그를 생성해 준다
				if ( NULL == pclclient->m_pDialog[NPCROOM_DLG] )
				{
					pclclient->CreateInterface( NPCROOM_DLG );
				}
			}
		}
		break;
	}
}

void CNPCRoom_Display::Action()
{
	if ( IsShow() == false )
	{
		return;
	}
	
	if ( m_UpdateTimer_Hide.IsTimed(::GetTickCount(), false) )
	{
		Hide();
	}
	
}

void CNPCRoom_Display::Show( void )
{
	CDialogController::Show( true );
}

void CNPCRoom_Display::Hide( void )
{
	CDialogController::Show( false );
}

void CNPCRoom_Display::ShowDisplayPCRoom( void )
{
	Show();

	m_UpdateTimer_Hide.SetActive( true, ::GetTickCount() );
}
