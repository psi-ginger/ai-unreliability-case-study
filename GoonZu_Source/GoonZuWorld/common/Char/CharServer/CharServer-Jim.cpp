//---------------------------------
// 2004/8/31 김태곤
//---------------------------------
#include "..\..\Server\Server.h"


#include "..\CharManager\CharManager.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "..\..\CommonLogic\MsgType-Person.h"

extern cltCommonLogic* pclClient;


// 찜이 풀린다. 
void cltCharServer::LoseJim()
{
	if(clJim.siJimHostCharUnique <= 0)return ;


	// 호스트의 정보도 삭제한다. 
	SI32 hostid = pclCM->GetIDFromCharUnique(clJim.siJimHostCharUnique);
	if(hostid)
	{
		if(pclCM->CR[hostid]->clJim.siJimSlaveCharUnique == GetCharUnique())
		{
			pclCM->CR[hostid]->clJim.siJimSlaveCharUnique = 0;
		}
	}
	
	clJim.Init();
	
	cltGameMsgResponse_Jim clinfo(GetCharUnique(), 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_JIM, sizeof(clinfo), (BYTE*)&clinfo);
	SendNeighbourMsg(&clMsg, true);
	
	
}