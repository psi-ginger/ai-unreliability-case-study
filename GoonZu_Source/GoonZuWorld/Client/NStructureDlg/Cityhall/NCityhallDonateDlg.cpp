#include "NCityhallDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallDonateDlg::NCityhallDonateDlg()
{
	m_siRequestPage = 1;
	m_siMaxPage = 1;
}

NCityhallDonateDlg::~NCityhallDonateDlg()
{

}

void NCityhallDonateDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( BUTTON_DONATE, TEXT("button_donate") );
	SetControlMap( BUTTON_PREV, TEXT("button_prev") );
	SetControlMap( BUTTON_NEXT, TEXT("button_next") );
	SetControlMap( LIST_DONATEMEMBER, TEXT("listbox_donatemember") );
	SetControlMap( EDIT_DONATEMONEY, TEXT("editbox_donatemoney") );
	SetControlMap( EDIT_PAGE, TEXT("editbox_page") );

	TCHAR* pText[4];
	pText[0] = "No";
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = GetTxtFromMgr(1564);
	pText[3] = GetTxtFromMgr(10162);

	CList* donateList = (CList*)m_pControlMap[ LIST_DONATEMEMBER ];
	donateList->SetColumn( 0, 20,  pText[ 0 ] );
	donateList->SetColumn( 1, 100, pText[ 1 ] );
	donateList->SetColumn( 2, 100, pText[ 2 ] );
	donateList->SetColumn( 3, 120, pText[ 3 ] );

	donateList->SetBorder(true);
	donateList->SetBKColorUse(true);

	donateList->Refresh();

	SI32 villageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();
	cltGameMsgRequest_DonateMemberList sendMsg( villageUnique, m_siRequestPage );
	cltMsg clMsg( GAMEMSG_REQUEST_DONATEMEMBERLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
	pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
}

void NCityhallDonateDlg::Action()
{
}

void NCityhallDonateDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	CASE_CONTROL( BUTTON_DONATE )
	{
		stMsgBox MsgBox;
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) 
			return;

		SI32 villageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();
		TCHAR* villageName = pclClient->pclMapManager->GetMapName(villageUnique);

		if( NULL == villageName )
			return;

		TCHAR* title = GetTxtFromMgr(10164);
		TCHAR* text = GetTxtFromMgr(10159);

		CEdit* donateEdit = (CEdit*)m_pControlMap[ EDIT_DONATEMONEY ];
		TCHAR* pszMoney = NULL;
		if(donateEdit)		pszMoney = donateEdit->GetText();

		if(pszMoney)
		{
			GMONEY donateMoney = _tstoi64( pszMoney );

			TCHAR money[128] = {'\0'};
			TCHAR buffer[256] = {'\0'};

			StringCchPrintf( money, 128, "%I64d", donateMoney );
			Nsprintf( buffer, text, TEXT("village"), villageName, TEXT("money"), money, NULL );
			MsgBox.Set( pParent, title, buffer, MSGBOX_TYPE_YESNO, 31 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}
	}

	else CASE_CONTROL( BUTTON_PREV )
	{
		--m_siRequestPage;

		if( 0 < m_siRequestPage )
		{
			SI32 villageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();
			cltGameMsgRequest_DonateMemberList sendMsg( villageUnique, m_siRequestPage );

			cltMsg clMsg( GAMEMSG_REQUEST_DONATEMEMBERLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
			pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		else
		{
			m_siRequestPage = 1;
		}

	}

	else CASE_CONTROL( BUTTON_NEXT )
	{
		++m_siRequestPage;

		if( m_siRequestPage <= m_siMaxPage )
		{
			SI32 villageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();
			cltGameMsgRequest_DonateMemberList sendMsg( villageUnique, m_siRequestPage );

			cltMsg clMsg( GAMEMSG_REQUEST_DONATEMEMBERLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
			pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		else
		{
			m_siRequestPage = m_siMaxPage;
		}
	}
}

void NCityhallDonateDlg::SetDonateMemberList( DonateMemberList* pDonateMemberList, SI32 MaxPage )
{
	m_siMaxPage = MaxPage;

	CList* donateList = (CList*)m_pControlMap[ LIST_DONATEMEMBER ];
	donateList->Clear();

	stListBoxItemData listBoxData;
	TCHAR* donateDate = GetTxtFromMgr(1644);

	CEdit* pageEdit = (CEdit*)m_pControlMap[ EDIT_PAGE ];
	pageEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	TCHAR buffer[128] = {'\0'};
	StringCchPrintf( buffer, 128, "%d/%d", m_siRequestPage, m_siMaxPage );
	pageEdit->SetText( buffer );

	for( SI32 i = 0; i < MAX_DONATEMEMBERLIST_PER_PAGE; ++i )
	{
		if( 0 == pDonateMemberList[i].siDonateMoney )
			continue;

		TCHAR buffer[128] = {'\0'};
		StringCchPrintf( buffer, 128, "%d", ( i + 1 ) + ( ( m_siRequestPage - 1 ) * MAX_DONATEMEMBERLIST_PER_PAGE ) );
		listBoxData.Set( 0, buffer );

		listBoxData.Set( 1, pDonateMemberList[i].szDonateName );

		cltDate clDonateDate;
		clDonateDate.MakeFullDateFromDateVary( pDonateMemberList[i].siDonateDate );
		StringCchPrintf( buffer, 128, donateDate, clDonateDate.uiYear, clDonateDate.uiMonth, clDonateDate.uiDay );
		listBoxData.Set( 2, buffer );

		g_JYLibFunction.SetNumUnit( pDonateMemberList[i].siDonateMoney, buffer,256, TEXT("") );
		listBoxData.Set( 3, buffer, ((cltClient*)pclClient)->GetColorByPrice(pDonateMemberList[i].siDonateMoney) );

		donateList->AddItem( &listBoxData );
	}

	donateList->Refresh();
}

void NCityhallDonateDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 31:	// 기부하기
		{
			if ( RetValue )
			{
				SI32 villageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();
				CEdit* donateEdit = (CEdit*)m_pControlMap[ EDIT_DONATEMONEY ];
				TCHAR* pszMoney = NULL;
				if(donateEdit)		pszMoney = donateEdit->GetText();
				if(pszMoney)
				{
					SI64 donateMoney = _tstoi64( pszMoney );

					if( donateMoney < MIN_DONATE_MONEY )
					{
						TCHAR * pTitle = GetTxtFromMgr(10164) ;
						TCHAR * pText = GetTxtFromMgr(10158) ;

						cltClient* pclclient =  (cltClient*)pclClient;
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
						return;
					}
					cltGameMsgRequest_DonateVillageMoney sendMsg( villageUnique, donateMoney );

					cltMsg clMsg( GAMEMSG_REQUEST_DONATEVILLAGEMONEY, sizeof(sendMsg), (BYTE*)&sendMsg );
					pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
				}
			}
			else
			{
				CHAR * pTitle = GetTxtFromMgr(3890) ;
				TCHAR * pText = GetTxtFromMgr(10161) ;
				cltClient* pclclient =  (cltClient*)pclClient;
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
				return;

			}
		}
		break;
	}
}
