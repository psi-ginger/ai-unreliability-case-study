#include "NStockDlg.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NStockChiefDlg::NStockChiefDlg()
{
}

NStockChiefDlg::~NStockChiefDlg()
{
}

void NStockChiefDlg::InitDlg()
{
	SetControlMap( EDIT_FEERATE, TEXT("editbox_feerate") );
	SetControlMap( BUTTON_SETFEERATE, TEXT("button_setfeerate") );
}


void NStockChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

	CASE_CONTROL( BUTTON_SETFEERATE )
	{
		int id = 1;
		if(id == 0)return;

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		cltFee clFee;

		// 수수료가 적절한지 확인한다. 
		float minfee = (float)_tstof( GetEditText( EDIT_FEERATE ) );
		if( clFee.SetFeeRate((SI32)(minfee*10)) == TRUE)
		{

			// 서버로 수수료 변경 요청을 한다. 
			cltMsg clMsg(GAMEMSG_REQUEST_SETSTOCKFEE,  
				sizeof(cltFee), (BYTE*)&clFee);
			
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1439);
			SetMainExplain( pText);
		}

	}



}