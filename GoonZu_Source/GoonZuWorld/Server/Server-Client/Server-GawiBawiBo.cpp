#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../common/Order/order.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "../../CommonLogic/MsgType-GawiBawiBo.h"
#include "../GawiBawiBoMgr/GawiBawiBoMgr_Server.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"


//  [8/12/2009 종호_GAWIBAWIBO]	금액 차감
void cltServer::DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_CHARGED_MONEY(cltMsg* pclMsg, SI32 id)
{
	cltGameRequest_GawiBawiBo_ChargedMoney* pclInfo = (cltGameRequest_GawiBawiBo_ChargedMoney*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
		return;

	GMONEY siMoney = GetGawiBawiBoMgr()->GetChargedMoney();	
	if(siMoney < 0)
		return;
    
	//	금액을 차감시킬거임
	sDBRequest_ChangeMoney clMsg(pclChar->GetID(), pclChar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_GAWIBAWIBO_CHARGED_MONEY, -siMoney);
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


	//	 플레이어가 소진한 금액을 로그 남김
	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GAWIBAWIBO, 
									LOGCOMMAND_INDEX_GAWIBAWIBO_DEL_MONEY, 		
									0, 
									pclChar, 
									NULL, 
									0,
									siMoney,
									0,
									NULL,
									0,
									0,
									0,
									0,
									0,
									0,
									0, 
									0,
									0, 
									0, 
									0, 
									NULL, 
									NULL);
}


//  [8/12/2009 종호_GAWIBAWIBO]	승무패 연산
void cltServer::DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_CALCULATE_RESULT(cltMsg* pclMsg, SI32 id)
{
	cltGameRequest_GawiBawiBo_CalculateResult* pclInfo = (cltGameRequest_GawiBawiBo_CalculateResult*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
		return;

	//	 승무패 결과 연산
	SI08 siResult = GetGawiBawiBoMgr()->GetCalculateResult();
	SI08 siValidCount = CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_NONE;


	// 항상 이기는 모드이면
	if(GetGawiBawiBoMgr()->IsAlwaysWin() == true)	 
		siResult = CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_WIN;
	

	// 연승제한이 걸린상태인데 플레이어가 이겼다면
	if(siResult == CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_WIN)
	{
		if(GetGawiBawiBoMgr()->GetLimitWin() <= pclChar->GetGawiBawiBoCount())		
			siResult = CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_LOSE;
	}

	if(siResult == CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_WIN || siResult == CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_ABUSE_WIN)	// 이겼다면 연승 카운트 증가
	{
		siValidCount = GetGawiBawiBoMgr()->IsValidCount(pclChar->GetGawiBawiBoCount()+1);

		switch(siValidCount)
		{
		case CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_VALID:	// 연승카운트 값이 유효한가.
				pclChar->IncreaseGawiBawiBoCount();
			break;

		case CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_MAXWIN:
				pclChar->IncreaseGawiBawiBoCount();
			break;

		case CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_INVALID:
			{
				pclClient->pclLog->FilePrint("GAWIBAWIBO_ERR.txt", "[Error-StraightWin] GAWIBAWIBO_RESULT_VALIDCOUNT_INVALID :: Current Count = %d",pclChar->GetGawiBawiBoCount()+1);				
				pclChar->InitGawiBawiBoCount();
			}
			break;

		default:
			break;
		}
	}
	else if(siResult == CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_LOSE)	//	졌다면 연승 카운트 초기화
		pclChar->InitGawiBawiBoCount();

	cltGameResponse_GawiBawiBo_CalculateResult clInfo(siResult, pclChar->GetGawiBawiBoCount(), siValidCount);
	cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_GAWIBAWIBO_CALCULATE_RESULT, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
}

//	보상 패킷
void cltServer::DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_REWARD(cltMsg* pclMsg, SI32 id)
{
	cltGameRequest_GawiBawiBo_Reward* pclInfo = (cltGameRequest_GawiBawiBo_Reward*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
		return;


    //	 클라이언트에서 주려는 보상과 서버에서 보려즌 보상 금액이 일치하는가?
	GMONEY siServerMoney = GetGawiBawiBoMgr()->GetCalculateCumulativeMoney(pclChar->GetGawiBawiBoCount());
	//GMONEY siClientMoney = pclInfo->m_siMoney;
	
	////	클라이언트의 보상금액과 서버의 보상금액이 다른 경우
	//if(siServerMoney != siClientMoney)
	//{
	//	pclClient->pclLog->FilePrint("GAWIBAWIBO_ERR.txt", "[Error-RewardMonyeData] ServerMoney = %d, ClientMoney = %d",siServerMoney, siClientMoney);
	//	assert(0);

	//	return;
	//}

	// 8연승 이상 했다면 주변인들에게 알림
	if(pclChar->GetGawiBawiBoCount() >= 8)
	{
		TCHAR szName[256] = "0,";
		StringCchCopy(szName, 256, pclChar->GetName());

		cltGameResponse_GawiBawiBo_Notice_Reward cNotice(szName, pclChar->GetGawiBawiBoCount(), siServerMoney);
		cltMsg clMsg(GAMEMSG_RESPONSE_GAWIBAWIBO_NOTICE_RESULT, sizeof(cltGameResponse_GawiBawiBo_Notice_Reward), (BYTE*)&cNotice);

		//pclChar->SendNeighbourMsg(&clMsg, true);
		pclCM->SendAllPersonInMapMsg((sPacketHeader*)&clMsg, MAPINDEX_FESTIVAL);
	}

	//	 보상 받는 경우는 게임을 중단 한 경우이므로 연승카운트 초기화
	pclChar->InitGawiBawiBoCount();

	//	 보상해주려는 금액에 문제가 없으므로 보상해 준다.
	if(GetGawiBawiBoMgr()->IsValidCount(pclChar->GetGawiBawiBoCount()) == CGawiBawiBoMgr_Server::GAWIBAWIBO_RESULT_VALIDCOUNT_MAXWIN)
	{
		// 최고연승이라면~
		sDBRequest_ChangeMoney clMsg(pclChar->GetID(), pclChar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_GAWIBAWIBO_REWARD_MAX_MONEY, +siServerMoney);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
	}
	else
	{
		sDBRequest_ChangeMoney clMsg(pclChar->GetID(), pclChar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_GAWIBAWIBO_REWARD_MONEY, +siServerMoney);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
	}



	//	플레이어가 벌어 들인 금액을 로그에 남김.
	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GAWIBAWIBO, 
									LOGCOMMAND_INDEX_GAWIBAWIBO_ADD_MONEY, 		
									0, 
									pclChar, 
									NULL, 
									0,
									siServerMoney,
									0,
									NULL,
									0,
									0,
									0,
									0,
									0,
									0,
									0, 
									0,
									0, 
									0, 
									0, 
									NULL, 
									NULL);
}
