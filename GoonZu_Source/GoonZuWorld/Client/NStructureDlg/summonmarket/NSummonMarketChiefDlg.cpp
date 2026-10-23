#include "NSummonMarketDlg.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-SummonMarket.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NSummonMarketChiefDlg::NSummonMarketChiefDlg()
{
}

NSummonMarketChiefDlg::~NSummonMarketChiefDlg()
{
}

void NSummonMarketChiefDlg::InitDlg()
{
	SetControlMap( EDIT_FEERATE, TEXT("editbox_feerate") );
	SetControlMap( BUTTON_SETFEERATE, TEXT("button_setfeerate") );
}


void NSummonMarketChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
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

		cltGameMsgRequest_SummonMarketSetFee setfee;

		// 수수료가 적절한지 확인한다. 
		float minfee = (float)_tstof( GetEditText( EDIT_FEERATE ) );
		if( setfee.clFee.SetFeeRate( SI32(minfee * 10) ) == TRUE ) {

			cltMsg clMsg( GAMEMSG_REQUEST_SUMMONMARKET_SETFEE, sizeof( setfee ), (BYTE *)&setfee );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

		} else {

			TCHAR* pText = GetTxtFromMgr(1439);
			SetMainExplain( pText);
		}

	}

}