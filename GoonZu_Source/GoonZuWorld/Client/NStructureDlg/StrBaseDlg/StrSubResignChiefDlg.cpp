#include "StrSubResignChiefDlg.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;


NStrSubResignChiefDlg::NStrSubResignChiefDlg()
{
}

NStrSubResignChiefDlg::~NStrSubResignChiefDlg()
{
}

void NStrSubResignChiefDlg::InitDlg()
{
	SetControlMap( BUTTON_RESIGN, TEXT("button_resign") );
}

void NStrSubResignChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	
	CASE_CONTROL( BUTTON_RESIGN )
	{
		// 서버에 (대)행수에서 사임함을 요청한다.
		SI32 id = 1;
		if(id)
		{
			//// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
			//cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			//if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == TRUE )
			//{

			//	cltMsg clMsg(GAMEMSG_REQUEST_RESIGN, 
			//		sizeof(cltSimpleRank), (BYTE*)&clrank);

			//	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			//}
			//else
			//{
			//	TCHAR* pText;
			//	if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
			//	else pText = GetTxtFromMgr(1440);
			//	SetMainExplain(pText);
			//}

		}		

	}
}