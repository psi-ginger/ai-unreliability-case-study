#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-Fishing.h"
#include "Msg\MsgType-Agriculture.h"

#include "..\Common\Fishing\FishingMgr.h"
#include "Char\CharManager\CharManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1)
	{
		return;
	}

	cltGameMsgRequest_AgricultureAutoGain * pclinfo = (cltGameMsgRequest_AgricultureAutoGain*)pclMsg->cData;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];	

	switch ( pclinfo->siAgricultureType )
	{
	case AGRICULTURE_TYPE_FISH:
		pclChar->bAutoFishing = pclinfo->bAuto;
		break;
	case AGRICULTURE_TYPE_FARM:
		pclChar->bAutoFarming = pclinfo->bAuto;
		break;
	case AGRICULTURE_TYPE_MINE:
		pclChar->bAutoMining = pclinfo->bAuto;
		break;
	default:
		{
			return;
		}
	}

	cltGameMsgResponse_AgricultureAutoGain clGameMsgResponse_AgricultureAutoGain( pclinfo->siAgricultureType, pclinfo->bAuto );
	cltMsg clMsg( GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN, sizeof( clGameMsgResponse_AgricultureAutoGain ), (BYTE*)&clGameMsgResponse_AgricultureAutoGain );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------

	return;
}
