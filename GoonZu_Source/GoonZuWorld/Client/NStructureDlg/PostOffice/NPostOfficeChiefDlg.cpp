#include "NPostOfficeDlg.h"

//#include "../../InterfaceMgr/Interface/ListView.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NPostOfficeChiefDlg::NPostOfficeChiefDlg()
{
}

NPostOfficeChiefDlg::~NPostOfficeChiefDlg()
{
	
}

void NPostOfficeChiefDlg::InitDlg()
{
	SetControlMap( EDIT_FEERATE_MONEY, TEXT("editbox_feerate_money") );
	SetControlMap( EDIT_FEERATE_ITEM, TEXT("editbox_feerate_item") );
	SetControlMap( EDIT_FEE_MSG, TEXT("editbox_fee_msg") );
	SetControlMap( BUTTON_SETUP, TEXT("button_setup") );


}

void NPostOfficeChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CASE_CONTROL( BUTTON_SETUP )
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
		cltGameMsgRequest_PostOfficeSetFee setfee;

		

		// 물품 수수료
		if( setfee.clItemFee.SetFeeRate( (int)( _tstof( GetEditText( EDIT_FEERATE_ITEM ) ) * 10 ) ) == FALSE) {
			TCHAR* pText = GetTxtFromMgr(1709);
			SetMainExplain( pText );
			return;
		}

		// 송금 수수료
		if( setfee.clMoneyFee.SetFeeRate( (int)( _tstof( GetEditText( EDIT_FEERATE_MONEY ) ) * 10 ) ) == FALSE) {
			TCHAR* pText = GetTxtFromMgr(1709);
			SetMainExplain( pText );
			return;
		}

		// 메시지 수수료
		setfee.siMsgFee = _tstoi( GetEditText( EDIT_FEE_MSG ) );

		cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SETFEE, sizeof( cltGameMsgRequest_PostOfficeSetFee ), (BYTE *)&setfee );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	}


}