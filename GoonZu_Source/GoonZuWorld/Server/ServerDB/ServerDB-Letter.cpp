//---------------------------------
// 2003/9/27 김태곤
//---------------------------------

#include "Char\CharManager\CharManager.h"
#include "..\Server\Server.h"

#include "..\..\Common\Map\Map.h"

#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"

#include "..\..\Common\Letter\Letter.h"

// 쪽지를 저장한다. 
void cltServer::DoMsg_DBMSG_RESPONSE_PUSHLETTER(sPacketHeader* pPacket)
{
	sDBResponse_PushLetter* pclMsg = (sDBResponse_PushLetter*)pPacket;
	

}


// 쪽지를 꺼낸다.
void cltServer::DoMsg_DBMSG_RESPONSE_POPLETTER(sPacketHeader* pPacket)
{
	sDBResponse_PopLetter* pclMsg = (sDBResponse_PopLetter*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)return ;
	if(pclCM->CR[id]->GetSession() == NULL)return ;

	cltLetterUnit* pclletter = &pclMsg->clLetterUnit;

	// 받은 쪽지를 클라이언트에게 보낸다. 
	pclCM->CR[id]->SendLetterMsg(&pclMsg->clLetterUnit);

}

void cltServer::DoMsg_DBMSG_RESPONSE_PUSHACCOUNTLETTER(sPacketHeader* pPacket)
{
	sDBResponse_PushAccountLetter* pclMsg = (sDBResponse_PushAccountLetter*)pPacket;


}


// 쪽지를 꺼낸다.
void cltServer::DoMsg_DBMSG_RESPONSE_POPACCOUNTLETTER(sPacketHeader* pPacket)
{
	
	sDBResponse_PopAccountLetter* pclMsg = (sDBResponse_PopAccountLetter*)pPacket;

	if(pclMsg->siResult == 0)
	{
	return ;
	}

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)return ;
	if(pclCM->CR[id]->GetSession() == NULL)return ;

	cltAccountLetterUnit* pclletter = &pclMsg->clLetterUnit;

	// 받은 쪽지를 클라이언트에게 보낸다. 
	((cltCharServer*)pclCM->CR[id])->SendAccountLetterMsg(&pclMsg->clLetterUnit);

}