// tys -

#include "Char/CharManager/CharManager.h"
#include "../../Client/NInterface/NSystemRewardList/NSystemRewardListDlg.h"

#include "..\Client.h"

#include "MsgRval-Define.h"
#include "Msg/MsgType-SystemReward.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_SYSTEMREWARDLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SystemRewardList* pclinfo = (cltGameMsgResponse_SystemRewardList*)pclMsg->cData;


	CNSystemRewardListDlg* pDialog = (CNSystemRewardListDlg*)m_pDialog[ NSYSTEMREWARDLIST_DLG ];

	if ( pDialog )
	{
		pDialog->SetRewardList( pclinfo->clInfo );
		pDialog->UpdatePageInfo(pclinfo->m_siCurPage, pclinfo->m_siMaxPage);

	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_RECIEVESYSTEMREWARD(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_RecieveSystemReward* pclinfo = (cltGameMsgResponse_RecieveSystemReward*)pclMsg->cData;

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

	// 아이템 업데이트
	pclchar->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].Set( &pclinfo->clItem );

	// 돈 업데이트
	pclchar->pclCI->clIP.SetMoney( pclinfo->siMoney );

	cltClient* pClient = (cltClient*)pclClient;
	((CNSystemRewardListDlg*)pClient->m_pDialog[NSYSTEMREWARDLIST_DLG])->RequestRewardList();
}