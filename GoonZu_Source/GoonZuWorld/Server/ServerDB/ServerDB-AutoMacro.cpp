#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"
#include "../GMSystem/GMSectionData.h"
#include "../GMSystem/GMProtocol.h"
#include "..\GMSystem\GMMsg.h"
#include "../Server/Server.h"

#include "../CommonLogic/House/House.h"
#include "../Common/Skill/Skill-Manager.h"
#include "../Common/Char/CharManager/CharManager.h"

#include "../../DBManager/GameDBManager_World/DBMsg-AutoMacro.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../GoonZuWorld/CommonLogic/Msg/MsgType-AutoMacro.h"


// [추가 : 황진성 2007. 12. 7 오토프로그램 메시지 처리 DB -> S]
void cltServer::DoMsg_DBMSG_RESPONSE_AUTOMACRO(sPacketHeader* pPacket)
{
	sDBResponse_AutoMacro* pReveMsg = (sDBResponse_AutoMacro*) pPacket;	

	SI32 id = pReveMsg->usCharID;
	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclCM->IsValidID(id) == false )						return;

	if(pclCM->CR[id]->pclCI->GetPersonID() != pReveMsg->siPersonID)
	{
		return;
	}
	cltGameMsgReponse_AutoMacroOnOther AutoMacroOnOther(pReveMsg->uiAutoMacroCnt, pclCM->CR[id]->pclCI->clBI.clPerson.GetName());
	cltMsg clMsg( GAMEMSG_RESPONSE_AUTOMACRO_ON_OTHER, sizeof(AutoMacroOnOther), (BYTE*)&AutoMacroOnOther);

	// 경고조치(전체공지, 개인경고).
	if(1 == pReveMsg->uiAutoMacroCnt)
	{
		// 전체 공지
		SI32 index = 0;
		SI32 id	   = 0;
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			// 사용자인 컴에게만 전달한다. 
			if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
			{
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
	// 7일간 게임 접속 제한. 일단 접속제한은 제외하고 메시지를 보낸다.
	else 
	{
		// 전체 공지
		SI32 index = 0;
		SI32 id	   = 0;
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			// 사용자인 컴에게만 전달한다. 
			if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
			{
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		/*
		sDBRequest_BlockUser clMsg(0, sDBRequest_BlockUser::BLOCK_UNTIL_DAYS, "", 7, "AutoMacro");
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		*/
	}
}