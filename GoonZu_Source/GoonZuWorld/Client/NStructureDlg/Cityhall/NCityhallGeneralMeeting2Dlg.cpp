#include "../../../Common/CommonHeader.h"
#include "../../Client.h"

#include "MsgType-Structure.h"

#include "Char\CharManager\CharManager.h"
#include "./NCityhallGeneralMeeting2Dlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include <tchar.h>

extern cltCommonLogic* pclClient;

NGeneralMeeting2Dlg::NGeneralMeeting2Dlg()
{
	m_pStatic = NULL;
	m_pStatic1 = NULL;
	m_pStatic2 = NULL;
	m_pStatic3 = NULL;

	m_pButtonAgree = NULL;
	m_pButtonOpposit = NULL;
	m_pButtonRefresh = NULL;

	m_pListAgree = NULL;
	m_pListOpposit = NULL;

	m_pEditAgree = NULL;
	m_pEditOpposit = NULL;

	m_pEditChat = NULL;
	m_pEditChatInput = NULL;

	m_pOutLine = NULL;

}

NGeneralMeeting2Dlg::~NGeneralMeeting2Dlg()
{
	SAFE_DELETE(m_pListAgree);
	SAFE_DELETE(m_pListOpposit);

	SAFE_DELETE(m_pButtonAgree);
	SAFE_DELETE(m_pButtonOpposit);
	SAFE_DELETE(m_pButtonRefresh);

	SAFE_DELETE(m_pEditAgree);
	SAFE_DELETE(m_pEditOpposit);

	SAFE_DELETE(m_pEditChat);
	SAFE_DELETE(m_pEditChatInput);

	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pStatic1);
	SAFE_DELETE(m_pStatic2);
	SAFE_DELETE(m_pStatic3);

	SAFE_DELETE(m_pOutLine);

}

void NGeneralMeeting2Dlg::InitInfo()
{

}

void NGeneralMeeting2Dlg::AddInfo( int villageunique )
{
	//m_siVillageUnique[ m_curIndex++ ] = villageunique;
}

void NGeneralMeeting2Dlg::SetText()
{
	/*

	int villageunique = m_siVillageUnique[ m_curIndex ];
	if( villageunique )
	{

		char buf[ 1024 ];
		char *pText = GetTxtFromMgr(5839);

		StringCchPrintf( buf, 256, pText, pclClient->pclVillageManager->GetName( villageunique ) );
		m_pStatic->SetText( buf );
	}
	*/
}

void NGeneralMeeting2Dlg::Create()
{
	
	if(!IsCreate())
	{
		cltClient* pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile( TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_GeneralMeeting2.ddf"));

		file.CreatePopupDialog( this, NGENERALMEETING2_DLG,  TEXT("dialog_cityhallgeneralmeeting2"), StaticCallBackDialogNGeneralMeeting2Dlg);

		NEWCSTATIC(m_pStatic);
		NEWCSTATIC(m_pStatic1);
		NEWCSTATIC(m_pStatic2);
		NEWCSTATIC(m_pStatic3);

		NEWCBUTTON(m_pButtonAgree);
		NEWCBUTTON(m_pButtonOpposit);
		NEWCBUTTON(m_pButtonRefresh);

		NEWCLIST(m_pListAgree);
		NEWCLIST(m_pListOpposit);

		NEWCEDIT(m_pEditAgree);
		NEWCEDIT(m_pEditOpposit);

		m_pEditChat = new CChatEdit( this );
		NEWCEDIT(m_pEditChatInput);

		NEWCOUTLINE(m_pOutLine);

		file.CreateControl( m_pOutLine, NGENERALMEETING2_DIALOG_OUTLINE, TEXT("statictext_NONAME1")); 

		file.CreateControl( m_pStatic, NGENERALMEETING2_DIALOG_STATIC, TEXT("statictext_NONAME0")); 
		file.CreateControl( m_pStatic1, NGENERALMEETING2_DIALOG_STATIC1, TEXT("statictext_NONAME12")); 
		file.CreateControl( m_pStatic2, NGENERALMEETING2_DIALOG_STATIC2, TEXT("statictext_NONAME13")); 
		file.CreateControl( m_pStatic3, NGENERALMEETING2_DIALOG_STATIC3, TEXT("statictext_NONAME22")); 
		file.CreateControl( m_pButtonAgree, NGENERALMEETING2_DIALOG_BUTTONAGREE, TEXT("button_agreement")); 
		file.CreateControl( m_pButtonOpposit, NGENERALMEETING2_DIALOG_BUTTONOPPOSIT, TEXT("button_opposition")); 
		file.CreateControl( m_pButtonRefresh, NGENERALMEETING2_DIALOG_BUTTONREFRESH, TEXT("button_refresh")); 


		file.CreateControl( m_pListAgree, NGENERALMEETING2_DIALOG_LISTAGREE, TEXT("listbox_agreement")); 
		file.CreateControl( m_pListOpposit, NGENERALMEETING2_DIALOG_LISTOPPOSIT, TEXT("listbox_opposition")); 

		file.CreateControl( m_pEditAgree, NGENERALMEETING2_DIALOG_EDITOPPOSIT, TEXT("editbox_agreement")); 
		file.CreateControl( m_pEditOpposit, NGENERALMEETING2_DIALOG_EDITOPPOSIT, TEXT("editbox_opposition")); 

		file.CreateControl( m_pEditChat, NGENERALMEETING2_DIALOG_EDITCHAT, TEXT("editbox_chat")); 
		file.CreateControl( m_pEditChatInput, NGENERALMEETING2_DIALOG_EDITCHATINPUT, TEXT("editbox_chatinput")); 


		MovePos( 400, 100 );

		InitVoteList();
		ShowVoterList();

		char buffer[ 1024 ];
		TCHAR *pText = GetTxtFromMgr( 5860 );

		cltCityHallManager *pCityhallManager = pclClient->pclCityHallManager;

		StringCchPrintf( buffer, 512, pText, pclClient->pclVillageManager->GetName( pCityhallManager->siVillageUnique ) );

		SetTitle( buffer );

		pText = GetTxtFromMgr( 5861 );

		StringCchPrintf( buffer, 512, pText, pCityhallManager->clGeneralMeetingInfo.szCandidateName );

		m_pStatic->SetText( buffer );

		SetTimerOnOff( true );

	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK NGeneralMeeting2Dlg::StaticCallBackDialogNGeneralMeeting2Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	
	NGeneralMeeting2Dlg *pThis = (NGeneralMeeting2Dlg*)pControllerMgr;
	pThis->CallBackDialogNGeneralMeeting2Dlg( nEvent, nControlID, pControl );
	

}

void CALLBACK NGeneralMeeting2Dlg::CallBackDialogNGeneralMeeting2Dlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	char buffer[ 1024 ];
	cltCityHallManager *pCityhallManager = pclClient->pclCityHallManager;
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NGENERALMEETING2_DIALOG_EDITCHATINPUT://여긴디!
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{

					cltGameMsgRequest_GeneralMeetingChat msg;

					TCHAR *EditBoxText = m_pEditChatInput->GetText();
					
					if ( EditBoxText && EditBoxText[ 0 ] != 0)
					{						
						memcpy( msg.szText, EditBoxText, 128  );
						msg.szText[ 127 ] = NULL;
						msg.textlen = _tcslen( msg.szText );
						cltMsg clMsg( GAMEMSG_REQUEST_GENERALMEETING_CHAT, 8 + msg.textlen , (BYTE*)&msg );
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
						
					}

					m_pEditChatInput->ClearText();
				}
				break;
			}
		}
		break;


	case NGENERALMEETING2_DIALOG_BUTTONAGREE:
		{
			TCHAR* pText = NULL;

				pText = GetTxtFromMgr(1551);

				SI32 stock = (SI32)(pCityhallManager->clGeneralMeetingInfo.siTotalStock / 100.);
				StringCchPrintf( buffer, 1024, pText, stock );

				pText = GetTxtFromMgr(1552);

				stMsgBox MsgBox;
				MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0);

		}
		break;

	case NGENERALMEETING2_DIALOG_BUTTONOPPOSIT:
		{

			TCHAR* pText = NULL;

				pText = GetTxtFromMgr(1554);

				SI32 stock = (SI32)(pCityhallManager->clGeneralMeetingInfo.siTotalStock / 100.);
				StringCchPrintf( buffer, 1024, pText, stock );

				pText = GetTxtFromMgr(1552);

				stMsgBox MsgBox;
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 1 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0);


		}
		break;

	case NGENERALMEETING2_DIALOG_BUTTONREFRESH:
		{
			cltGameMsgRequest_GeneralMeetingVoteRefresh sendMsg;

			cltMsg clMsg( GAMEMSG_REQUEST_GENERALMEETING_VOTE_REFRESH, sizeof(sendMsg), (BYTE*)&sendMsg );
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );

		}
		break;

	}

}

void NGeneralMeeting2Dlg::InitVoteList()
{
	cltCityHallManager *pCityhallManager = pclClient->pclCityHallManager;

	// 리스트
	CList *pListAgreement = m_pListAgree;
	CList *pListOpposition = m_pListOpposit;

	stListBoxItemData itemdata;

	pListAgreement->SetBorder(true);
	pListAgreement->SetBKColorUse(true);

	pListOpposition->SetBorder(true);
	pListOpposition->SetBKColorUse(true);


	int width[] = { 90, 68 };
	//	TCHAR *text[] = { TEXT("이름"), TEXT("주식수") };

	TCHAR* pText[2];

	pText[0] = GetTxtFromMgr(1556);
	pText[1] = GetTxtFromMgr(1475);

	for( int i = 0; i < 2; ++i ) {
		pListAgreement->SetColumn( i, width[ i ], pText[ i ] );
		pListOpposition->SetColumn( i, width[ i ], pText[ i ] );
	}

	pListAgreement->Refresh();
	pListOpposition->Refresh();
}


void NGeneralMeeting2Dlg::ShowVoterList()
{

	cltCityHallManager *pCityhallManager = pclClient->pclCityHallManager;

	// 리스트
	CList *pListAgreement = m_pListAgree;
	CList *pListOpposition = m_pListOpposit;

	stListBoxItemData itemdata;

	TCHAR buffer[256];

	SI32 agreestock = 0;
	SI32 oppositstock = 0;

	SI32 agreeindex = 0;
	SI32 oppositindex = 0;

	pListAgreement->Clear();
	pListOpposition->Clear();

	if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 0 ) return;

	for( int i = 0; i < 100; ++i ) {

		if( pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siPersonID == 0 ) continue;

		if( pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siVoteType == 1 ) {

			itemdata.Init();
			itemdata.Set( 0, pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].szPersonName );

			_itot( pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siStockAmount, buffer, 10 );
			itemdata.Set( 1, buffer );

			pListAgreement->AddItem( &itemdata );

			agreeindex++;

			agreestock += pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siStockAmount;

		} else if( pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siVoteType == 2 ) {


			itemdata.Init();
			itemdata.Set( 0, pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].szPersonName );

			_itot( pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siStockAmount, buffer, 10 );
			itemdata.Set( 1, buffer );

			pListOpposition->AddItem( &itemdata );

			oppositindex++;

			oppositstock += pCityhallManager->clGeneralMeetingInfo.clVoter[ i ].siStockAmount;
		}

	}

	pListAgreement->Refresh();
	pListOpposition->Refresh();

	TCHAR* pText = GetTxtFromMgr(1560);

	StringCchPrintf( buffer, 256, pText, agreeindex, agreestock );
	m_pEditAgree->SetText( buffer );

	StringCchPrintf( buffer, 256, pText, oppositindex, oppositstock );
	m_pEditOpposit->SetText( buffer );


}

void NGeneralMeeting2Dlg::Action()
{

	cltCityHallManager *pCityhallManager = pclClient->pclCityHallManager;

	if( pCityhallManager->bUpdateGeneralMeetingInfo ) {
		pCityhallManager->bUpdateGeneralMeetingInfo = FALSE;

		ShowVoterList();
	}



}


void NGeneralMeeting2Dlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltCityHallManager *pCityhallManager = pclClient->pclCityHallManager;


	switch( MsgIndex )
	{
	case 0:	//
		{
			if ( RetValue )
			{
				cltGameMsgRequest_GeneralMeetingVote sendMsg( 1 );

				cltMsg clMsg( GAMEMSG_REQUEST_GENERALMEETING_VOTE, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
			}
		}
		break;
	case 1:	//
		{
			if ( RetValue )
			{
				cltGameMsgRequest_GeneralMeetingVote sendMsg( 2 );

				cltMsg clMsg( GAMEMSG_REQUEST_GENERALMEETING_VOTE, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
			}
		}
		break;
	}

}

void NGeneralMeeting2Dlg::SetObserveMode(bool bObserve)
{
	if (bObserve)
	{

		m_pButtonAgree->Show(false);
		m_pButtonOpposit->Show(false);
		m_pEditChat->Show(false);
		m_pEditChatInput->Show(false);
		m_pStatic3->Show(false);

		SetDialogPos( GetX(), GetY(), GetWidth(), GetHeight() - 140);

	}
}

