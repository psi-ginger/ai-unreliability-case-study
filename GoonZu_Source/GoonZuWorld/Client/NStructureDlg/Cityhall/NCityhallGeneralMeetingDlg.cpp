#include "NCityhallDlg.h"
#include "NCityhallGeneralMeeting2Dlg.h"

#include "../../InterfaceMgr/Interface/List.h"
//#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Cityhall/CityHall-SubmitDlg.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

extern cltSuggestDlg g_cityhallSuggestdlg;

NCityhallGeneralMeetingDlg::NCityhallGeneralMeetingDlg()
{
	m_bleftTimeBreak = false;
}

NCityhallGeneralMeetingDlg::~NCityhallGeneralMeetingDlg()
{

}

void NCityhallGeneralMeetingDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( BUTTON_SUGGEST, TEXT("button_suggest") );
	SetControlMap( BUTTON_ACCEPT, TEXT("button_accept") );
//	SetControlMap( BUTTON_AGREEMENT, TEXT("button_agreement") );
//	SetControlMap( BUTTON_OPPOSITION, TEXT("button_opposition") );
	
	SetControlMap( EDIT_EXPLAIN, TEXT("editbox_explain") );
	SetControlMap( EDIT_START_DATE, TEXT("editbox_NONAME1") );
	SetControlMap( EDIT_END_DATE, TEXT("editbox_date") );

//	SetControlMap( EDIT_AGREEMENT, TEXT("editbox_agreement") );
//	SetControlMap( EDIT_OPPOSITION, TEXT("editbox_opposition") );

//	SetControlMap( LIST_AGREEMENT, TEXT("listbox_agreement") );
//	SetControlMap( LIST_OPPOSITION, TEXT("listbox_opposition") );


	SetControlMap( BUTTON_VOTE, TEXT("button_vote") );


	pCityhallManager->bUpdateGeneralMeetingInfo = FALSE;
	pCityhallManager->bRequestGeneralMeetingInfo = FALSE;

//	InitVoteList();

	ShowMeetingInfo();
//	ShowVoterList();

	// 서버에 주주총회 정보를  요청한다. 
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GENERALMEETING_INFO, 0, NULL);				
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}


}

void NCityhallGeneralMeetingDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

//	if( pCityhallManager->bUpdateGeneralMeetingInfo ) {
//		pCityhallManager->bUpdateGeneralMeetingInfo = FALSE;

		ShowMeetingInfo();
//		ShowVoterList();
//	}

	ShowLeftTime();

}


void NCityhallGeneralMeetingDlg::ShowMeetingInfo()
{

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	TCHAR buffer[ 1024 ];
	
	switch( pCityhallManager->clGeneralMeetingInfo.siMatterType ) {
	case 0:
		{
			TCHAR* pText = GetTxtFromMgr(1557);
			StringCchCopy( buffer, 1024, pText);

		}
		break;

	case 1:
		{
			if( pCityhallManager->clGeneralMeetingInfo.siAcceptCandidate == 1 ) {
				
				TCHAR* pText = GetTxtFromMgr(1558);
				StringCchPrintf( buffer, 1024, pText,	pCityhallManager->clGeneralMeetingInfo.szCandidateName
					);

			} else {

				TCHAR* pText = GetTxtFromMgr(1559);
				StringCchPrintf( buffer, 1024, pText,
					pCityhallManager->clGeneralMeetingInfo.szCandidateName,
					pCityhallManager->clGeneralMeetingInfo.szCandidateName
					);
			}
		}
		break;


	}

	SetEditText( EDIT_EXPLAIN, buffer );

}


void NCityhallGeneralMeetingDlg::ShowLeftTime()
{

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 0 ||	true == m_bleftTimeBreak )
	{
		TCHAR buf[ 256 ] = {'\0'};

		TCHAR* pText = GetTxtFromMgr(1644);
		StringCchPrintf( buf, 256, pText,	pCityhallManager->clClientStrInfo.clCityhallStrInfo.clChiefDate.uiYear,
			pCityhallManager->clClientStrInfo.clCityhallStrInfo.clChiefDate.uiMonth,
			pCityhallManager->clClientStrInfo.clCityhallStrInfo.clChiefDate.uiDay	);

		SetEditText( EDIT_START_DATE, buf );

		pText = GetTxtFromMgr(1561);
		SetEditText( EDIT_END_DATE, pText );
		return;
	}

	TCHAR buf[ 256 ] = {'\0'};

	cltDate *pclCurrentDate = &((cltClient *)pclClient)->clClientDate;
	cltDate clLeftDate;

	if( pclCurrentDate->IsLaterMonthThan( &pCityhallManager->clGeneralMeetingInfo.clCloseDate ) &&
		pclCurrentDate->IsLaterDayThan( &pCityhallManager->clGeneralMeetingInfo.clCloseDate ) ) 
	{
		return;
	}

	clLeftDate.DiffDate( &pCityhallManager->clGeneralMeetingInfo.clCloseDate, pclCurrentDate );
	if( 0 == clLeftDate.uiMonth && 0 == clLeftDate.uiDay && 0 == clLeftDate.uiHour )
		m_bleftTimeBreak = true;

	if( clLeftDate.uiMonth == 0 )
	{
		TCHAR* pText = GetTxtFromMgr(1562);
		StringCchPrintf( buf, 256, pText, clLeftDate.uiDay, clLeftDate.uiHour );
	} 
	else
	{
		TCHAR* pText = GetTxtFromMgr(1563);
		StringCchPrintf( buf, 256, pText, clLeftDate.uiMonth, clLeftDate.uiDay );
	}

	SetEditText( EDIT_END_DATE, buf );

	TCHAR* pText = GetTxtFromMgr(1644);
	StringCchPrintf( buf, 256, pText,	pCityhallManager->clClientStrInfo.clCityhallStrInfo.clChiefDate.uiYear,
		pCityhallManager->clClientStrInfo.clCityhallStrInfo.clChiefDate.uiMonth,
		pCityhallManager->clClientStrInfo.clCityhallStrInfo.clChiefDate.uiDay	);

	SetEditText( EDIT_START_DATE, buf );

}

void NCityhallGeneralMeetingDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

//	TCHAR buffer[ 1024 ];
/*
	else CASE_CONTROL( BUTTON_OPPOSITION )
	{
		TCHAR* pText = NULL;
		if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 0 ) {
			pText = GetTxtFromMgr(1549);
			SetMainExplain( pText);
			return;
		}

		if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 1 &&
			pCityhallManager->clGeneralMeetingInfo.siAcceptCandidate == 0 ) {
				pText = GetTxtFromMgr(1553);
			SetMainExplain( pText );
			return;
		}

		pText = GetTxtFromMgr(1554);

		SI32 stock = pCityhallManager->clGeneralMeetingInfo.siTotalStock / 100.;
		StringCchPrintf( buffer, 1024, pText, stock );

		pText = GetTxtFromMgr(1552);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 1 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0);

		//if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
		//{
		//	return;
		//}

		//cltGameMsgRequest_GeneralMeetingVote sendMsg( 2 );

		//cltMsg clMsg( GAMEMSG_REQUEST_GENERALMEETING_VOTE, sizeof(sendMsg), (BYTE*)&sendMsg );
		//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );


	}
*/
	CASE_CONTROL( BUTTON_ACCEPT )
	{
		if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 0 ) {
			TCHAR* pText = GetTxtFromMgr(1549);
			SetMainExplain( pText);
			return;
		}

		if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 1 &&
			pCityhallManager->clGeneralMeetingInfo.siAcceptCandidate == 1 ) {
				TCHAR* pText = GetTxtFromMgr(1555);
			SetMainExplain( pText );
			return;
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GENERALMEETING_ACCEPTCANDIDATE, 0, NULL);				
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

		//KHY - 0710 - 동의 정보 갱신.
		// 서버에 주주총회 정보를  요청한다. 
		{
			cltMsg clMsg(GAMEMSG_REQUEST_GENERALMEETING_INFO, 0, NULL);				
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}

	}

	else CASE_CONTROL( BUTTON_SUGGEST )
	{
		//g_cityhallSuggestdlg.Create( NULL, ((cltClient*)pclClient)->GetHwnd(), pCityhallManager );
		pclclient->CreateInterface( NCITYHALLSUBMIT_DLG );
	}

	else CASE_CONTROL( BUTTON_VOTE )
	{

		char *pText = NULL;

		if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 0 ) {
			pText = GetTxtFromMgr(1549);
			SetMainExplain( pText);
			return;
		}

		if( pCityhallManager->clGeneralMeetingInfo.siMatterType == 1 &&
			pCityhallManager->clGeneralMeetingInfo.siAcceptCandidate == 0 ) {
				pText = GetTxtFromMgr(1553);
				SetMainExplain( pText );
				return;
			}

		if (pCityhallManager->clGeneralMeetingInfo.IsVoter( pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
			|| pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
		{
			cltMsg clMsg(GAMEMSG_REQUEST_GENERALMEETING_CHATJOIN, 0, NULL);				
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			if( pclclient->m_pDialog[ NGENERALMEETING2_DLG ]==NULL )
			{
				pclclient->CreateInterface( NGENERALMEETING2_DLG );

				((NGeneralMeeting2Dlg*)pclclient->m_pDialog[ NGENERALMEETING2_DLG ])->SetObserveMode(true);
			}
		}

	}

}

void NCityhallGeneralMeetingDlg::Show()
{
	NStrTabDlg::Show();

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CLICKGENERALMETTING );
}