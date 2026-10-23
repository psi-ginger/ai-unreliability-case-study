
#include "..\Server\Server.h"
#include "MsgRval-Define.h"

#include "../CommonLogic/Msg/MsgType-HappyBeanQuest.h"

#include "Char/CharManager/CharManager.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"

//KHY - 0822 - 해피빈 퀘스트 추가.
void cltServer::DoMsg_GAMEMSG_REQUEST_HAPPYBEANQUEST_START(cltMsg* clMsg, SI32 id)
{
	cltGameMsgRequest_HappyBeanQuestStart* pclinfo = (cltGameMsgRequest_HappyBeanQuestStart*)clMsg->cData;

	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	// LEEKH 해피빈 퀘스트 기간 제한
	if(pclinfo->siQuestType == QUEST_TYPE_HAPPYBEAN)
	{		
		if( sTime.wYear == 2007 &&
			(sTime.wMonth ==  9 && sTime.wDay > 3) ||
			(sTime.wMonth == 10 && sTime.wDay <= 3) )
		{
			// 퀘스트 상태를 DB에 저장한다.
			//sDBRequest_SetHappyBeanClearInfo clMsg2(pclchar->pclCI->GetPersonID(), pclinfo->siQuestType, pclinfo->siQuestState);
			//SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg2);
			SendLog_SetHappyBeanClearInfo(pclchar, pclinfo->siQuestType, pclinfo->siQuestState);
		}
		else
		{
			cltGameMsgResponse_HappyBeanQuestStart_NO clinfo( 2 );
			cltMsg clMsg(GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_NO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

