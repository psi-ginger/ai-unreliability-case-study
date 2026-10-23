#include "NStockDlg.h"

#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Stock.h"


#include "../../CommonLogic/MsgType-Structure.h"

#include "../../CommonLogic/MsgType.h"

#include <tchar.h>

extern cltCommonLogic* pclClient;

NStockPublicSubscriptionDlg::NStockPublicSubscriptionDlg()
{
	m_kUpdateTimer_ButtonDisable.Init(5 * 1000);
}

NStockPublicSubscriptionDlg::~NStockPublicSubscriptionDlg()
{
}

void NStockPublicSubscriptionDlg::InitDlg()
{
	SetControlMap( COMBO_VILLAGELIST, TEXT("combobox_villagelist") );
	SetControlMap( BUTTON_ORDER, TEXT("button_order") );

	CComboBox	*pCombo;
	pCombo = (CComboBox*)m_pControlMap[ COMBO_VILLAGELIST ];

	pCombo->Clear();


	// 주식 공모 마을 정보 요청
	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);
	pStockManager->UpdateStockOfferingInfoSwitch = FALSE;

	SI32 id = 1;
	if(id)
	{
		cltMsg clMsg( GAMEMSG_REQUEST_STOCKOFFERINGINFO, 0 );
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
	}

}

void NStockPublicSubscriptionDlg::Action()
{
	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

	if( m_kUpdateTimer_ButtonDisable.IsTimed(GetTickCount(), false) == true)//주식공모시 5초 딜레이
	{
		CButton *pButton;
		pButton = (CButton*)m_pControlMap[ BUTTON_ORDER ];

		if(pButton)	pButton->Enable(true);
	}

	if( pStockManager->UpdateStockOfferingInfoSwitch )
	{
		pStockManager->UpdateStockOfferingInfoSwitch = FALSE;

		CComboBox	*pCombo;
		pCombo = (CComboBox*)m_pControlMap[ COMBO_VILLAGELIST ];

		stComboBoxItemData comboBoxItemData;


		SI32 i;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclClient->pclVillageManager->pclVillageInfo[i] &&
			   pStockManager->clStockOfferingInfo[i].siStatus == 1 )
			{
				comboBoxItemData.Init();
				comboBoxItemData.Set( pclClient->pclVillageManager->pclVillageInfo[i]->szName );			
				pCombo->AddItem( &comboBoxItemData );
			}
		}

		pCombo->Refresh();
	}
}


void NStockPublicSubscriptionDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if(pclclient == NULL)								return;

	CComboBox	*pCombo = (CComboBox*)m_pControlMap[ COMBO_VILLAGELIST ];

	CASE_CONTROL( BUTTON_ORDER )
	{
		TCHAR *szSelText = pCombo->GetText( pCombo->GetSelectedIndex() );
		
		// 선택한 마을의 유니크를 구한다.
		SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
		BOOL bAccount = pclCM->CR[1]->pclCI->clBank.clAccountNum.IsValid();
		if( villageunique < 0 ) return;


		if( bAccount==FALSE)//청약시 통장계좌가 없으면 실패매세지 출력하고 실패[2007.08.10 손성웅]
		{
			TCHAR* pText = GetTxtFromMgr(7557);
			SetMainExplain( pText);
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}
		//cltMsg clMsg(GAMEMSG_REQUEST_STOCKOFFERINGORDER, villageunique);
		TCHAR	szMacAddress[20];
		TCHAR	szPrivateIP[20];
		pclclient->GetMacAddress(pclclient->pclCM->CR[1]->GetSession()->GetSocket(), szMacAddress, sizeof(szMacAddress));

		// IP ADDRESS 추출
		SOCKADDR_IN addr;
		int len = sizeof( addr );
		getsockname( pclclient->pclCM->CR[1]->GetSession()->GetSocket(), (SOCKADDR *)&addr, &len );
		memcpy( szPrivateIP, inet_ntoa( addr.sin_addr ), sizeof(szPrivateIP) );
		szPrivateIP[ sizeof(szPrivateIP) - 1 ] = '\0';

		cltGameMsgRequest_StockOfferingOrder clStockOfferingOrder(villageunique, szPrivateIP, szMacAddress );
		cltMsg clMsg(GAMEMSG_REQUEST_STOCKOFFERINGORDER, sizeof(clStockOfferingOrder), (BYTE*)&clStockOfferingOrder);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		m_kUpdateTimer_ButtonDisable.SetActive(true, GetTickCount());//주식공모시 5초 딜레이

		CButton	*pButton;//주식공모시 5초 딜레이
		pButton = (CButton*)m_pControlMap[ BUTTON_ORDER ];//주식공모시 5초 딜레이

		if(pButton)		pButton->Enable(false);//주식공모시 5초 딜레이
	}
}

void NStockPublicSubscriptionDlg::Show()
{
	NStrTabDlg::Show();

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CLICKSTOCKOFFER );
}