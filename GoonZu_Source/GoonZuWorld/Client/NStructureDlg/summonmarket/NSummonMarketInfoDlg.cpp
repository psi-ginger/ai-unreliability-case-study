#include "NSummonMarketDlg.h"

#include "../../../CommonLogic/Msg/MsgType-SummonMarket.h"
#include "../../../CommonLogic/MsgType-Structure.h"
#include "../../../CommonLogic/CityHall/TaxpaymentListDlg.h"

extern CJYLibFunction g_JYLibFunction;
extern cltCommonLogic* pclClient;

NSummonMarketInfoDlg::NSummonMarketInfoDlg()
{
}

NSummonMarketInfoDlg::~NSummonMarketInfoDlg()
{
	
}

void NSummonMarketInfoDlg::InitDlg()
{
	// 장예원 매니져
	cltSummonMarketManager *pSummonManager = ((cltSummonMarketManager *)m_pStrManager);
	if( pSummonManager == NULL ) return;

	SetControlMap( EDITBOX_STRDURA,  "editbox_strdura" );
	SetControlMap( EDITBOX_STRCHIEF,  "editbox_strchief" );
	SetControlMap( EDITBOX_STRMONEY,  "editbox_strmoney" );
	SetControlMap( BUTTON_TAXLIST, "button_taxlist");

}

void NSummonMarketInfoDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	// 장예원 매니져
	cltSummonMarketManager *pSummonMarketManager = ((cltSummonMarketManager *)m_pStrManager);

	CASE_CONTROL( BUTTON_TAXLIST )
		{
			char* pText = GetTxtFromMgr(471);
			pSummonMarketManager->pclTaxPaymentDlg->Create( NULL, NULL, pSummonMarketManager, pText );
		}
}

void NSummonMarketInfoDlg::Action()
{
	// 장예원 매니져
	cltSummonMarketManager *pSummonMarketManager = ((cltSummonMarketManager *)m_pStrManager);

	char buffer[256];

	// 건물 내구도를 표시한다
	sprintf( buffer, "%d/%d", pSummonMarketManager->clClientStrInfo.clCommonInfo.siStrDura, 10000 );
	SetEditText( EDITBOX_STRDURA, buffer );

	// 장예원행수의 이름을 표시한다. 
	if(pSummonMarketManager->UpdateChiefSwitch == true)
	{
		pSummonMarketManager->UpdateChiefSwitch = false;

		// 장을 보여준다. 
		if(pSummonMarketManager->siVillageUnique > 0)
		{
			ShowChief( &pclClient->pclVillageManager->pclVillageInfo[pSummonMarketManager->siVillageUnique]->clVillageVary.clRankPerson[RANKTYPE_PORT], 
				pclClient->pclVillageManager->pclVillageInfo[pSummonMarketManager->siVillageUnique]->RankAccountID[RANKTYPE_PORT]
				);
		}
	}

	// 보유중인 자금을 표시한다.  
	memset( buffer, 0, 256 );
	char* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit( pSummonMarketManager->clClientStrInfo.clCommonInfo.clMoney.itMoney, buffer, pText );
	SetEditText( EDITBOX_STRMONEY, buffer );
}

// 장의 정보를 보여준다. 
BOOL NSummonMarketInfoDlg::ShowChief( cltSimplePerson* pclperson, char *paccountid)
{
	char buffer[256];
	
	if(pclperson->GetPersonID())
	{
		wsprintf(buffer, "%s", pclperson->GetName());
	}
	else
	{
		char* pText = GetTxtFromMgr(1287);
		wsprintf(buffer, pText);
	}

	SetEditText( EDITBOX_STRCHIEF, buffer );

	// Chief SummonMarketrait Draw Routine!!
	//DrawSummonMarketrait(paccountid);
	
	return TRUE;
}
