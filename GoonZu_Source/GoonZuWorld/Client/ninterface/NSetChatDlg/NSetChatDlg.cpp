#include "NSetChatDlg.h"
#include "../../Client.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"
#include "../../../common/CommonHeader.h"
#include "../../../CommonLogic/MsgType-System.h"
#include "../../../CommonLogic/MsgType-Person.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NResidentsDlg/NResidentsDlg.h"

extern cltCommonLogic* pclClient;

CNSetChatDlg::CNSetChatDlg() : CDialogController()
{
}

CNSetChatDlg::~CNSetChatDlg()
{
}

void CNSetChatDlg::DelDialog()
{
	
	DeleteDialog();
}
void CNSetChatDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( IsCreate() ){
		DelDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NSetChatDlg/DLG_SetChatDlg.ddf" ) );
	file.CreatePopupDialog( this, NSETCHAT_DLG, TEXT( "dialog_SetChat" ), StaticCallBackDialogNChat );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  SETCHATDLG_STATIC_NONAME1,	this ), SETCHATDLG_STATIC_NONAME1,	TEXT( "statictext_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  SETCHATDLG_STATIC_NONAME2,	this ), SETCHATDLG_STATIC_NONAME2,	TEXT( "statictext_NONAME2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  SETCHATDLG_STATIC_NONAME3,	this ), SETCHATDLG_STATIC_NONAME3,	TEXT( "statictext_NONAME3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  SETCHATDLG_STATIC_NONAME4,	this ), SETCHATDLG_STATIC_NONAME4,	TEXT( "statictext_NONAME4" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  SETCHATDLG_STATIC_NONAME5,	this ), SETCHATDLG_STATIC_NONAME5,	TEXT( "statictext_NONAME5" ) );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX,  SETCHATDLG_CHECKBOX_NONAME1,	this ), SETCHATDLG_CHECKBOX_NONAME1,	TEXT( "checkbox_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX,  SETCHATDLG_CHECKBOX_NONAME2,	this ), SETCHATDLG_CHECKBOX_NONAME2,	TEXT( "checkbox_NONAME2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX,  SETCHATDLG_CHECKBOX_NONAME3,	this ), SETCHATDLG_CHECKBOX_NONAME3,	TEXT( "checkbox_NONAME3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX,  SETCHATDLG_CHECKBOX_NONAME4,	this ), SETCHATDLG_CHECKBOX_NONAME4,	TEXT( "checkbox_NONAME4" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX,  SETCHATDLG_CHECKBOX_NONAME5,	this ), SETCHATDLG_CHECKBOX_NONAME5,	TEXT( "checkbox_NONAME5" ) );

	CDialogController* pControl = pclclient->m_pDialog[NCHATBAR_DLG];
	this->MovePos( 	pControl->GetX() + pControl->GetWidth() , pControl->GetY() - 110);

	SetActionTimer( 100 );    
	SetTimerOnOff( true );
	if (pclClient->pclCM->CR[1]->pclCI->clCharOption.bChatSetNormal == false) {
		CCheckBox* pCheckBoxNormal = m_InterfaceFactory.GetCheckBox(SETCHATDLG_CHECKBOX_NONAME1) ;
		pCheckBoxNormal->SetCheck( true );
	}
	if (pclClient->pclCM->CR[1]->pclCI->clCharOption.bChatSetVillage == false) {
		CCheckBox* pCheckBoxVillage = m_InterfaceFactory.GetCheckBox(SETCHATDLG_CHECKBOX_NONAME2) ;
		pCheckBoxVillage->SetCheck(true);
	}
	if (pclClient->pclCM->CR[1]->pclCI->clCharOption.bChatSetGuild == false) {
		CCheckBox* pCheckBoxGuild = m_InterfaceFactory.GetCheckBox(SETCHATDLG_CHECKBOX_NONAME3) ;
		pCheckBoxGuild->SetCheck(true);
	}
	if (pclClient->pclCM->CR[1]->pclCI->clCharOption.bChatSetParty == false) {
		CCheckBox* pCheckBoxParty = m_InterfaceFactory.GetCheckBox(SETCHATDLG_CHECKBOX_NONAME4) ;
		pCheckBoxParty->SetCheck(true);
	}
	if (pclClient->pclCM->CR[1]->pclCI->clCharOption.bChatSetSystem == false) {
		CCheckBox* pCheckBoxSystem = m_InterfaceFactory.GetCheckBox(SETCHATDLG_CHECKBOX_NONAME5) ;
		pCheckBoxSystem->SetCheck(true);
	}
}

void CALLBACK CNSetChatDlg::StaticCallBackDialogNChat( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSetChatDlg *pThis = (CNSetChatDlg*) pControllerMgr;
	pThis->CallBackDialogNChat( nEvent, nControlID, pControl );
}

void CALLBACK CNSetChatDlg::CallBackDialogNChat( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME1:
		{
			switch( nEvent )
			{
				case EVENT_CHECKBOX_CHANGED:
					{
						CheckBoxChange( SETCHATDLG_CHECKBOX_NONAME1 );

					}
			}
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME2:
		{
			switch( nEvent )
			{
				case EVENT_CHECKBOX_CHANGED:
					{
						CheckBoxChange( SETCHATDLG_CHECKBOX_NONAME2 );
					}
			}
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME3:
		{	
			switch( nEvent )
			{
				case EVENT_CHECKBOX_CHANGED:
					{
						CheckBoxChange( SETCHATDLG_CHECKBOX_NONAME3 );
					}
			}
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME4:
		{
			switch( nEvent )
			{
				case EVENT_CHECKBOX_CHANGED:
					{
						CheckBoxChange( SETCHATDLG_CHECKBOX_NONAME4 );
					}
			}
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME5:
		{	
			switch( nEvent )
			{
				case EVENT_CHECKBOX_CHANGED:
					{
						CheckBoxChange( SETCHATDLG_CHECKBOX_NONAME5 );
					
					}
			}
		}
		break;
	}

	return;
}

void CNSetChatDlg::CheckBoxChange( SI32 siObjectID )
{
	SI32 siCheckBoxID	= 0;
	SI32 siSetValue		= 0;
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( siObjectID )
	{
	case SETCHATDLG_CHECKBOX_NONAME1:
		{
			siCheckBoxID	= SETCHATDLG_CHECKBOX_NONAME1;
			siSetValue		= CHTFILLTER_NORMAL;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( SETCHATDLG_CHECKBOX_NONAME1);
			if ( pCheckBox->IsCheck() == true ) {
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetNormal( false);
			}
			else{
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetNormal( true );
			}
			/*cltGameMsgRequest_ChatSet clinfo( true, CHTFILLTER_NORMAL );
			cltMsg clMsg(GAMEMSG_REQUEST_CHATSET, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);*/
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME2:
		{
			siCheckBoxID	= SETCHATDLG_CHECKBOX_NONAME2;
			siSetValue		= CHTFILLTER_VILLAGE;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( SETCHATDLG_CHECKBOX_NONAME2);
			if ( pCheckBox->IsCheck() == true ) {
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetVillage( false);
			}
			else{
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetVillage( true );
			}
			/*cltGameMsgRequest_ChatSet clinfo( true, CHTFILLTER_VILLAGE );
			cltMsg clMsg(GAMEMSG_REQUEST_CHATSET, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);*/
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME3:
		{
			siCheckBoxID	= SETCHATDLG_CHECKBOX_NONAME3;
			siSetValue		= CHTFILLTER_GUILD;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( SETCHATDLG_CHECKBOX_NONAME3 );
			if ( pCheckBox->IsCheck() == true ) {
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetGuild( false );
			}
			else{
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetGuild( true );
			}
			/*cltGameMsgRequest_ChatSet clinfo( true, CHTFILLTER_GUILD );
			cltMsg clMsg(GAMEMSG_REQUEST_CHATSET, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);*/

		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME4:
		{
			siCheckBoxID	= SETCHATDLG_CHECKBOX_NONAME4;
			siSetValue		= CHTFILLTER_PARTY;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( SETCHATDLG_CHECKBOX_NONAME4 );
			if ( pCheckBox->IsCheck() == true ) {
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetParty( false);
			}
			else{
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetParty( true );
			}
			/*cltGameMsgRequest_ChatSet clinfo( true, CHTFILLTER_PARTY );
			cltMsg clMsg(GAMEMSG_REQUEST_CHATSET, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);*/
		}
		break;
	case SETCHATDLG_CHECKBOX_NONAME5:
		{
			siCheckBoxID	= SETCHATDLG_CHECKBOX_NONAME5;
			siSetValue		= CHTFILLTER_SYSTEM;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( SETCHATDLG_CHECKBOX_NONAME5 );
			if ( pCheckBox->IsCheck() == true ) {
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetSystem( false);
			}
			else{
				pclClient->pclCM->CR[1]->pclCI->clCharOption.SetChatSetSystem( true );
			}			
			//cltGameMsgRequest_ChatSet clinfo( true, CHTFILLTER_SYSTEM );
			//cltMsg clMsg(GAMEMSG_REQUEST_CHATSET, sizeof(clinfo), (BYTE*)&clinfo);
			//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		break;
	default:
		return;
	}

	if ( NULL == pclclient )
	{
		return;
	}

	CCheckBox* pCheckChatSet = m_InterfaceFactory.GetCheckBox(siCheckBoxID);
	if ( NULL == pCheckChatSet )
	{
		return;
	}

	CNChatBarDlg* pclChatBar = (CNChatBarDlg*)pclclient->m_pDialog[NCHATBAR_DLG];
	if ( NULL == pclChatBar )
	{
		return;
	}

	cltGameMsgRequest_Char_Option clinfo( pclClient->pclCM->CR[1]->pclCI->clCharOption );
	cltMsg clMsg(GAMEMSG_REQUEST_CHAR_OPTION, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	//pclChatBar->DelChatstatus( siSetValue );
}