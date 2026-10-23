#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"

#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../CommonLogic/MsgType-GawiBawiBo.h"

#include "../GawiBawiBoMgr/GawiBawiBoMgr_Client.h"
#include "../../Server/GawiBawiBoMgr/GawiBawiBoMgr_Server.h"

#include "../ninterface/nrareitemdlg/NRareItemDlg.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_CHARGED_MONEY(cltMsg* pclMsg, SI32 id)
{
	
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_CALCULATE_RESULT(cltMsg* pclMsg, SI32 id)
{
	//	가위바위보 결과
	cltGameResponse_GawiBawiBo_CalculateResult* pclInfo = (cltGameResponse_GawiBawiBo_CalculateResult*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	GetGawiBawiBoMgr()->SetResult(pclInfo->m_siResult);
	GetGawiBawiBoMgr()->SetCount(pclInfo->m_siCount);
	
	switch(pclInfo->m_siValidCount)
	{
		case CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_VALID:		// 연승카운트 값이 유효한가.
			break;

		case CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_MAXWIN:	// 최고 연승인가
			GetGawiBawiBoMgr()->SetMaximumWins(true);
			break;

		case CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_INVALID:
			assert(0);
			break;
	}

	GetGawiBawiBoMgr()->NextSteps();
}



void cltClient::DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_NOTICE_RESULT(cltMsg* pclMsg, SI32 id)
{
	//	가위바위보 결과
	cltGameResponse_GawiBawiBo_Notice_Reward* pclInfo = (cltGameResponse_GawiBawiBo_Notice_Reward*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	//	누군가 가위바위보 보상을 받았다.
	TCHAR*	pTitle	= GetTxtFromMgr(40050);
	NTCHARString256	pBody(GetTxtFromMgr(40057));

	NTCHARString256 strMoney;
	g_JYLibFunction.SetNumUnit(pclInfo->m_siMoney, strMoney, 256);

	pBody.Replace("#name#", pclInfo->m_szName);
	pBody.Replace("#value#", SI64ToString(pclInfo->m_siCount));
	pBody.Replace("#money#", strMoney);

	//cltClient* pClient = (cltClient*)pclClient;
	//pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);


	// 공지창
	if(m_pDialog[NRAREITEM_DLG ] == NULL)	
		CreateInterface( NRAREITEM_DLG );

	((CNRareItemDlg*)m_pDialog[NRAREITEM_DLG])->SetCharName(pclInfo->m_szName);
	((CNRareItemDlg*)m_pDialog[NRAREITEM_DLG])->SetDescription(pBody);
	((CNRareItemDlg*)m_pDialog[NRAREITEM_DLG])->SetImage(TEXT("GImg/eventitem.spr"));

	//((CNRareItemDlg*)m_pDialog[NRAREITEM_DLG ])->SetBestGacha(NULL, pclInfo->m_szName, pclInfo->m_siCount, pclInfo->m_siMoney);		
}


//void cltClient::DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_COUNT(cltMsg* pclMsg, SI32 id)
//{
//	//	연승 결과
//	cltGameResponse_GawiBawiBo_GawiBawiBoCount_Increase* pclInfo = (cltGameResponse_GawiBawiBo_GawiBawiBoCount_Increase*)pclMsg->cData;
//
//	if(pclInfo == NULL)
//		return;
//
//	//	 서버에 적용된 현재 연승 카운트가 왔다. 이 연승 카운트를 적용
//	GetGawiBawiBoMgr()->SetCount(pclInfo->m_siCount);
//	
//	//	게임을 계속 진행
//	GetGawiBawiBoMgr()->SetStatus(CGawiBawiBoMgr_Client::GAWIBAWIBO_DO_CHECK_MONEY);
//	
//}
