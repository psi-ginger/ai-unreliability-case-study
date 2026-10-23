#include "StrSubPayTaxDlg.h"


#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NStrSubPayTaxDlg::NStrSubPayTaxDlg()
{

}

NStrSubPayTaxDlg::~NStrSubPayTaxDlg()
{

}

void NStrSubPayTaxDlg::InitDlg()
{
	SetControlMap( EDIT_MONEY, TEXT("editbox_money") );
	SetControlMap( BUTTON_PAYTAX, TEXT("button_paytax") );

}

void NStrSubPayTaxDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

	CASE_CONTROL( BUTTON_PAYTAX )
	{
		SI32 id = 1;
		if( id )
		{

			//// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
			//{
			//	cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			//	if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			//	{
			//		TCHAR* pText;
			//		if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
			//		else pText = GetTxtFromMgr(1440);
			//		SetMainExplain(pText);
			//		return;
			//	}
			//}

			//TCHAR buf[ 256 ];
			//int money = _tstoi( GetEditText( EDIT_MONEY ) );

			//if( money <= 0 ) {
			//	TCHAR* pText = GetTxtFromMgr(1438);
			//	SetMainExplain(pText);
			//	return;
			//}

			//cltMoney clMoney;

			//clMoney.Set( money );

			//cltGameMsgRequest_PayTax clPayTax( m_pStrManager->siVillageUnique, m_pStrManager->siRankType, 0, &clMoney );
			//	
			//cltMsg clMsg( GAMEMSG_REQUEST_PAYTAX, sizeof(cltGameMsgRequest_PayTax), (BYTE*)&clPayTax );

			//pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			
		}
	}

}