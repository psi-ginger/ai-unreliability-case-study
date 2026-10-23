//---------------------------------
// 2003/10/11 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"


#include "../../Server/AuthServer/AuthMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-House.h"

#include "..\CommonLogic\MsgType-RealEstate.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

// 부동산 계약 연장 요청. 
void cltServer::DoMsg_GAMEMSG_REQUEST_EXTENDCONTRACT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_ExtendContract* pclinfo = (cltGameMsgRequest_ExtendContract*)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	SI32 realestatetype = pclinfo->siRealEstateType ;
	if(realestatetype != REALESTATE_TYPE_HOUSE)return ;

	SI32 index = pclinfo->siIndex;

	//-------------------------------------------------
	// 주어진 부동산이 id의 소유인지 확인한다. 
	//-------------------------------------------------
	if( pclHouseManager->IsHouseHost(personid, villageunique, index) == false)
	{
		return ;
	}

	//------------------------------------------------
	// 클라이언트가 요구한 계약이 현재의 계약과 동일한지 확인한다. 
	//------------------------------------------------
	
	cltStrInfo* pclvary = pclHouseManager->GetStrInfo(villageunique);
	if(pclvary == NULL)return ;

	if(pclvary->clHouseStrInfo.clRentContract.IsSame(&pclinfo->clContract) == false)
	{
		return ;
	}

	//----------------------------------------------
	// 계약 일자를 업데이트 한다. 
	//---------------------------------------------
	cltRentContract clcontract;
	clcontract.Set(&pclinfo->clContract);
	clcontract.clDate.Set(&pclTime->clDate);

	//-----------------------------------------------
	// DB에 계약을 연장할 것을 요청한다. 
	//-----------------------------------------------
	sDBRequest_ExtendContract clMsg(id, personid, villageunique, realestatetype, index, &clcontract);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, clcontract.siRentFee);
}
