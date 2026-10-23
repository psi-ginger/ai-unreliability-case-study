#include "NHelperMiniDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "NHelperQuestionOrAnswerDlg.h"

NHelperMiniDlg::NHelperMiniDlg()
{

}


NHelperMiniDlg::~NHelperMiniDlg()
{
	Destroy();
}


void NHelperMiniDlg::Init()
{

}


void NHelperMiniDlg::Destroy()
{


}


void NHelperMiniDlg::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NHelperDlg/NHelperMiniDlg.ddf");

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_helper_mini") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this , NHELPER_DLG, "dialog_helper_mini" , StatiNHelperMiniDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NHELPER_DLG_HELP_BUTTON, this), NHELPER_DLG_HELP_BUTTON,  "button_helper_mini");

	m_InterfaceFactory.GetButton(NHELPER_DLG_HELP_BUTTON)->SetUseToolTip( GetTxtFromMgr(9568) );

	if( g_bFullScreen)	MovePos( 385, 65 );
	else		 		MovePos( 383, 40 );

	//cltClient *pclclient = (cltClient*)pclClient;
	//if( pclclient->m_pDialog[NMINIMAP_DLG] )
	//{
	//	SetX( pclclient->m_pDialog[NMINIMAP_DLG]->GetX() - 50 );  
	//	SetY( pclclient->m_pDialog[NMINIMAP_DLG]->GetY() -  5 );  
	//}

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

	Show( false );	
} 

void NHelperMiniDlg::Action()
{
	if( pclClient->IsCountrySwitch( Switch_Helper ) )
	{
		if ( pclClient->GetGameStatus() == GS_GAME )	
		{
			// 설정이 안되있으면.
			cltClient * pclclient = (cltClient*) pclClient;
			if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bHelper == false && false == IsShow() )
				Show( true );	 
			else if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bHelper == true &&  true == IsShow() )
				Show( false );
		}
	}
}

void CALLBACK NHelperMiniDlg::StatiNHelperMiniDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NHelperMiniDlg *pThis = (NHelperMiniDlg *)pControllerMgr;
	pThis->NHelperMiniDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NHelperMiniDlg::NHelperMiniDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;
	
	switch(nControlID)
	{
		case NHELPER_DLG_HELP_BUTTON:
			{
				switch( nEvent )
				{
					case EVENT_BUTTON_CLICKED:
						{
							if( pclclient->m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG] )	return;
							if( pclclient->m_pDialog[NHELPER_QUESTION_LIST_DLG] )		return;

							// 운영자 이면 조건 부문하고 무조건 질문 리스트를 요청하는거여~.
							if( 200 <= pclClient->pclCM->CR[1]->GetGameMasterLevel() )
							{
								cltMsg clMsg(GAMEMSG_REQUEST_QUESTION_LIST_GET, 0, NULL);
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
							else
							{
								if( 50 > pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() )		
								{
									cltMsg clMsg(GAMEMSG_REQUEST_CHECK_QUESTION, 0, NULL);
									pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
								}
								else
								{
									cltMsg clMsg(GAMEMSG_REQUEST_QUESTION_LIST_GET, 0, NULL);
									pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
								}
							}						
 
							m_InterfaceFactory.GetButton(NHELPER_DLG_HELP_BUTTON)->SetFocus( false );
						}
						break;
				}
			}
			break;
		
		default:	break;
	}
	
}




