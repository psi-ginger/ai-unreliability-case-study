//---------------------------------
// 2003/6/26 김태곤
//---------------------------------
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-House.h"

#include "..\Client\Interface\HouseUnit\HouseUnitBaseInfo.h"

#include "MsgRval-Define.h"
#include "..\CommonLogic\MsgType-House.h"
#include "..\CommonLogic\MsgType-Person.h"

#include "../CommonLogic/RealEstate/RealEstate.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

// 창고 이름 변경
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITNAMECHANGE(cltMsg* pclMsg, SI32 id)
{
	//메시지를 확보한다. 
	cltGameMsgRequest_ChangeHouseUnitName* pclData= (cltGameMsgRequest_ChangeHouseUnitName*)pclMsg->cData;

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;
	
	if(m_pCAbuseFilter->CheckAbuseWord(pclData->m_szHouseName))
	{
		return;
	}

	SI32 siVillageunique = pclChar->GetCurrentVillageUnique();
	if(siVillageunique <= 0 || siVillageunique >= MAX_VILLAGE_NUMBER)	return ;

	if( siVillageunique != pclData->m_siVillageUnique)
		return;

	sDBRequest_ChangeHouseUnitName clSendMsg(id, pclChar->pclCI->GetPersonID(), siVillageunique, pclData->m_siHouseIndex, pclData->m_szHouseName);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clSendMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CHANGEHOUSECONTRACT(cltMsg* pclMsg, SI32 id)
{

	//메시지를 확보한다. 
	cltRentContractPacket* pclpacket = (cltRentContractPacket*)pclMsg->cData;
	if(pclpacket->clRentContract.IsValid() == FALSE)return ;

	//----------------------------------
	// 요청자가 시전행수인지 확인한다. 
	//----------------------------------
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;


	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;

	cltSimpleRank clrank(RANKTYPE_HOUSE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 시전행수가 아니라 계약 조건 변경에 실패. 
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILSETCONTRACT_NOTCHIEF,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// 계약 기간을 확인한다. 
	if(pclpacket->clRentContract.siRentPeriod <= 0 || pclpacket->clRentContract.siRentPeriod > MAX_RENT_PERIOD_YEAR)return ;

	// 임대금액을 확인한다. 
	if(pclpacket->clRentContract.siRentFee < MIN_HOUSE_RENT_FEE || pclpacket->clRentContract.siRentFee > MAX_HOUSE_RENT_FEE)return ;


	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_HOUSE );

	if( pStrInfo == NULL ) return;

	if( pclpacket->clRentContract.siRentFee < pStrInfo->clHouseStrInfo.clRentContract.siRentFee - 10000 ||
		pclpacket->clRentContract.siRentFee > pStrInfo->clHouseStrInfo.clRentContract.siRentFee + 10000 ) 
	{
			SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILSETCONTRACT_WRONGFEE,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
	}

	if( pclTime->clDate.GetDateVary() < 
		pStrInfo->clHouseStrInfo.clLastChangeDate.GetDateVary() + 30 ) 
	{
			SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILSETCONTRACT_DATE,  0, 0, pclCM->CR[id]->GetCharUnique());
				return;
	}

	//-----------------------------------
	// 계약 종료일을 기록한다. 
	//-----------------------------------
	pclpacket->clRentContract.clDate.Set(&pclTime->clDate);
	pclpacket->clRentContract.clDate.AddMonth(pclpacket->clRentContract.siRentPeriod * 12);


	// DB매니저에게 계약 조건을 변경하도록 요청한다. 
	sDBRequest_ChangeHouseContract clMsg(id, pclpacket->siVillageUnique, &pclpacket->clRentContract, &pclTime->clDate );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

// 시전 리스트를 요청한다. (누구나 요청할 수 있다. )
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITLIST(cltMsg* pclMsg, SI32 id)
{
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	//------------------------------------------------------------
	// 해당 마을의 시전에서 정보를 구해서 클라이언트로 보낸다. 
	//------------------------------------------------------------
	cltGameMsgRequest_HouseUnitList clInfo(villageunique);
	cltGameMsgResponse_MyHouseUnitList clMyHouseList;

	SI32 siMyHouseIndex = 0;
	const SI32 YEARTOMONTH = 12;

	for(SI32 i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		// 각각의 하우스유닛에 대해 RealEstateMarketManager에 상품으로 등록되어있는 하우스유닛인지 확인
		bool b = pclRealEstateMarketManager->IsExistOrder( PersonID, villageunique, i);

		clInfo.clPerson[i].Set(&pclHouseManager->pclStruct[villageunique]->GetHouseUnit(i)->clPerson);

		clInfo.bSell[i] = b;

		// 자신의 시전 정보 라면
		if( PersonID == clInfo.clPerson[i].GetPersonID())
		{
			cltHouseUnit* pclHouseUnit = pclHouseManager->pclStruct[villageunique]->GetHouseUnit(i);
			if( NULL != pclHouseUnit)
			{
				cltMyHouseInfo clMyHouseInfo( i, pclHouseUnit->m_szName , &pclHouseUnit->clContract);

				clMyHouseList.clMyHouseInfo[siMyHouseIndex].Set(&clMyHouseInfo);
				++siMyHouseIndex;
			}
		}
	}

	cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITLIST, sizeof(clInfo), (BYTE*)&clInfo);	
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	if( IsCountrySwitch(Switch_ChangeHouseList))
	{
		// [기형] 자신의 시전 정보 추가 전송
		cltMsg clMsgExtend(GAMEMSG_RESPONSE_MYHOUSEUNITLIST, sizeof(clMyHouseList), (BYTE*)&clMyHouseList);	
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsgExtend);
	}	
}

// 시전 상세 정보 리스트를 요청한다. (시전 행수만 요청할 수 있다.  )
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITLIST_DETAIL(cltMsg* pclMsg, SI32 id)
{
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	//-----------------------------------
	// 해당 마을의 시전행수인지 확인한다. 
	//-----------------------------------
	cltSimpleRank clrank(RANKTYPE_HOUSE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		return ;
	}

	//------------------------------------------------------------
	// 해당 마을의 시전에서 정보를 구해서 클라이언트로 보낸다. 
	//------------------------------------------------------------
	cltGameMsgRequest_HouseUnitListDetail clInfo(villageunique);

	for(SI32 i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		clInfo.clContract[i].Set(&pclHouseManager->pclStruct[villageunique]->GetHouseUnit(i)->clContract);
	}

	cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITLIST_DETAIL, sizeof(clInfo), (BYTE*)&clInfo);

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}



// 시전 임차 희망자 명단을 삭제한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_DELHOUSEAPPLYER(cltMsg* pclMsg, SI32 id)
{
	// PCK : 임차 희망자를 모두 삭제하는것을 수정.
	SI32 Slot = (SI32)*pclMsg->cData;

	// 마을의 유니크를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)	return ;

	// PersonID를 확보한다. 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)		return ;

	//-----------------------------------
	// 해당 마을의 시전행수인지 확인한다. 
	//-----------------------------------
	cltSimpleRank clrank(RANKTYPE_HOUSE, villageunique);
	if(pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILDELHOUSEAPPLYER_NOTCHIEF,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//-----------------------------------
	// DB에 명단을 삭제할 것을 요청한다.
	//-----------------------------------
	sDBRequest_DelHouseApplyer clMsg(id, personid, villageunique, Slot);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}



void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEAPPLYERLIST(cltMsg* pclMsg, SI32 id)
{

	//----------------------------------
	// 요청자가 시전행수인지 확인한다. 
	//----------------------------------
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;


	cltSimpleRank clrank(RANKTYPE_HOUSE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 시전행수가 아니라 계약 조건 변경에 실패. 
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILSETCONTRACT_NOTCHIEF,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}


	// 해당 마을의 시전에서 정보를 구해서 클라이언트로 보낸다. 
	for(SI32 i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		cltHouseApplyerPacket sendMsg(i, pclHouseManager->pclStruct[villageunique]->GetHouseApplyer(i) );

		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEAPPLYERLIST, sizeof(cltHouseApplyerPacket), (BYTE*)&sendMsg);		
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}

// 시전 해약을 위한 위약금을 요청해왔다
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSECANCELLATIONPENALTYMONEY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_HouseCancellationPenaltyMoney* pclinfo = (cltGameMsgRequest_HouseCancellationPenaltyMoney*)pclMsg->cData;

	GMONEY siPenaltyMoney = 0; // 위약금
	GMONEY siRentMoney = 0; // 계약된 임대료

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0 ) return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER) return ;

	SI32 villageunique = pclinfo->siVillageUnique;
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;
	SI32 houseunitslot = pclinfo->siHouseIndex;
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return ;
	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if(pclHouseManager->IsHouseHost(personid, villageunique, houseunitslot) == false)return ;

	// 간단 예외처리 끝


	// 돈계산 시작
	cltHouseUnit* pclhouse = pclHouseManager->GetHouseUnit(villageunique,houseunitslot);
	if( pclhouse == NULL) return;
	cltServer * pclserver = (cltServer*)pclClient;

	siRentMoney = pclhouse->clContract.siRentFee;

	siPenaltyMoney = siRentMoney ;	// 위약금은 1회분 계약 임대료 로 한다.

	cltGameMsgResponse_HouseCancellationPenaltyMoney sendMsg(siPenaltyMoney);
	cltMsg clMsg(GAMEMSG_RESPONSE_HOUSECANCELLATIONPENALTYMONEY,sizeof(sendMsg), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSECANCELLATION(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_HouseCancellation* pclinfo = (cltGameMsgRequest_HouseCancellation*)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0 ) return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER) return ;

	SI32 villageunique = pclinfo->siVillageUnique;
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;
	SI32 houseunitslot = pclinfo->siHouseIndex;
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return ;
	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if(pclHouseManager->IsHouseHost(personid, villageunique, houseunitslot) == false)return ;

	// 간단 예외처리 끝
	// 아이템, 말, 소환수가 시전안에 있나 확인한다.
	cltHouseUnitStorageInfo* pclstg = pclHouseManager->GetStgPointer(villageunique, houseunitslot);
	if(pclstg == NULL)return ;

	if(pclstg->IsItemRemaind() || pclstg->IsHorseRemaind() || pclstg->IsSummonRemaind())
	{
		SendServerResponseMsg( RANKTYPE_HOUSE, SRVAL_HOUSE_FAILREMAINDITEM,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 위약금 계산 시작
	// 돈계산 시작
	cltHouseUnit* pclhouse = pclHouseManager->GetHouseUnit(villageunique,houseunitslot);
	if( pclhouse == NULL) return;


	cltServer * pclserver = (cltServer*)pclClient;

	GMONEY siRentMoney = pclhouse->clContract.siRentFee;
	// SI32 siRemainDay =  5; //pclhouse->clContract.clDate.GetDay() + pclhouse->clContract.siRentPeriod - pclserver->pclTime->clDate.uiDay;
	GMONEY siPenaltyMoney = siRentMoney; // * siRemainDay * 0.1f;

	// 유저가 가진돈과 위약금을 비교하여 돈이 있는지 체크

	if( siPenaltyMoney > pclCM->CR[id]->pclCI->clIP.GetMoney() ) {
		SendServerResponseMsg( RANKTYPE_HOUSE, SRVAL_HOUSE_FAILNOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}	

	// 유저의 이름을 알아온다.
	const TCHAR* pOwnerName = pclCM->CR[id]->GetName();

	// DB에 시전 지워달라고 요청 시작, 덤으로 위약금도 보내서 유저의 소지금에서 차감한다.
	sDBRequest_RecallHouse clMsg(personid, HOUSERECALL_REASON_CANCELLATION, villageunique, houseunitslot, siPenaltyMoney,pOwnerName);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, siPenaltyMoney);
}

// 특정 마을에서의 임차 신청을 취소한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSERENTAPPLYCANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_HouseRentApplyCancel* pclinfo = (cltGameMsgRequest_HouseRentApplyCancel*)pclMsg->cData;

	// personid확보. 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	// 마을 유니크를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return;
	if(villageunique != pclinfo->siVillageUnique)					return;

	// DB로 취소를 요청한다. 
	sDBRequest_HouseRentApplyCancel clMsg(id, villageunique, personid);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 16] // 시전 임대 취소(행수가 신청자를 목록에서 삭제했을 때)
	//	// 
	//	cltServer* pclserver = (cltServer*)pclClient;
	//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_RENTAPPLYCANCEL,
	//									0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, villageunique, 0, 0, 
	//									0, 0, 0, 0, 0, NULL, NULL);
	//}
}

// 임차를 신청한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSERENTAPPLY(cltMsg* pclMsg, SI32 id)
{
	// 클라이언트가 보내온 계약 조건을 확인한다.
	cltRentContractPacket* pclpacket = (cltRentContractPacket*) pclMsg->cData;
	SI32 sivillageunique = pclpacket->siVillageUnique;
	if(sivillageunique <= 0 || sivillageunique >= MAX_VILLAGE_NUMBER)	return ;
	cltRentContract* pclcontract = &pclpacket->clRentContract;

	// 임대기간이 부적절하면 실패. 
	if(pclcontract->siRentPeriod <= 0 || pclcontract->siRentPeriod > MAX_RENT_PERIOD_YEAR)return ;

	// 기본 레벨 조건을 충족시키지 못하면 실패. 
	if(pclCM->CR[id]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE)return ;


	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 날짜를 얻어온다. 
	pclcontract->clDate.Set(&pclTime->clDate);

	// 해당 시전의 계약조건과 비교하여 동일하지 않으면 실패. 
	if(pclHouseManager->IsSameContract(sivillageunique, pclcontract) == FALSE)
	{
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILAPPLY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// 전장 계좌가 있어야 한다. 
	cltAccountNum*	pclAccountNum = &pclCM->CR[id]->pclCI->clBank.clAccountNum;
	if(pclAccountNum->GetAccountNum() == 0)
	{
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILAPPLY_NOACCOUNT,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// 이미 등록되어 있으면 실패. 
	cltRentApplyer* pclapplyer = pclHouseManager->GetHouseApplyerFromPersonID(sivillageunique, personid);
	if(pclapplyer)
	{
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILAPPLY_ALREADYAPPLY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// 신청자로 등록한다.
	sDBRequest_HouseRentApply clMsg(id, sivillageunique, personid, pclcontract);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSERENT(cltMsg* pclMsg, SI32 id)
{

	// 임대 요청을 확보한다. 
	cltRequestRent* pclrent = (cltRequestRent*)pclMsg->cData;

	//----------------------------------
	// 요청자가 시전행수인지 확인한다. 
	//----------------------------------
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;


	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;


	cltSimpleRank clrank(RANKTYPE_HOUSE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 시전행수가 아니라 계약 조건 변경에 실패. 
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILSETCONTRACT_NOTCHIEF,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//----------------------------
	// 빈시전이 있는지 확인한다. 
	//----------------------------
	if(pclHouseManager->IsEmpty(pclrent->siVillageUnique, pclrent->siSlot) == FALSE)
	{
		// 빈 시전이 없어서 임대할 수 없다.
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_NOEMPTYHOUSE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//---------------------------------
	// 신청자가 유효한지 확인한다.
	//---------------------------------
	if(pclrent->siPersonID == 0)
	{
		// 임차 희망자가 유효하지 않다. 
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_INVALIDHOUSEAPPLYER,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	cltRentApplyer* pclapplyer = pclHouseManager->GetHouseApplyerFromPersonID(pclrent->siVillageUnique, pclrent->siPersonID);
	if(pclapplyer == NULL)
	{
		// 임차 희망자가 유효하지 않다. 
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_INVALIDHOUSEAPPLYER,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//---------------------------------
	// 임차인이 동의한 계약 내용과 동일한지 확인한다. 
	//---------------------------------
	// 계약 내용을 확정한다. 
	cltStrInfo* pclvary = pclHouseManager->GetStrInfo(pclrent->siVillageUnique);
	if(pclvary == NULL)return ;

	// 임대료가 0원인지 체크한다.
	if(pclvary->clHouseStrInfo.clRentContract.siRentFee <= 0)
		return;

	cltRentContract clcontract;
	clcontract.Set(&pclvary->clHouseStrInfo.clRentContract);

	if(pclapplyer->clContract.IsSame(&clcontract) == false)
	{
		SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILNOTSAMECONTRACT,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//--------------------------------
	// 빈 시전의 임차인을 등록한다. 
	//--------------------------------
	// 현재 날짜로 계약일을 설정한다. 
	clcontract.clDate.Set(&pclTime->clDate);

	// DBManager에게 임대를 요청한다.
	sDBRequest_HouseRent clMsg(id, pclrent->siVillageUnique, pclrent->siPersonID, pclrent->siSlot, &clcontract, (TCHAR*)pclCM->CR[id]->GetName());
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

// 인벤에서 창고로 아이템을 이동. 
void cltServer::DoMsg_GAMEMSG_REQUEST_MOVEITEMINVTOSTG(cltMsg* pclMsg, SI32 id)
{

	stHouseUnitData* pclinfo = (stHouseUnitData*)pclMsg->cData;


	MOVEITEMINVTOSTG(id, pclinfo->si32VillageUnique, pclinfo->si32HouseUnitSlot, pclinfo->si32lParam1, &pclinfo->clItem);

	//------------------------------------------
	// 사용기간을 확인한다.
	//------------------------------------------

}

BOOL cltServer::MOVEITEMINVTOSTG(SI32 siCharId, SI32 siVillageUnique, SI32 siHouseIndex, SI32 siItemPos, cltItem* pclInvenItem, bool bNoMoveNewPos)
{
	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)return FALSE;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[siCharId]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER)	return FALSE;

	//----------------------------------------------
	// 창고에 아이템을 넣을 공간이 있는지 확인한다. 
	//---------------------------------------------
	SI32 personid		= pclCM->CR[siCharId]->pclCI->GetPersonID();
	if(personid <= 0)return FALSE;


	if( NULL == pclInvenItem)
		return FALSE;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siCharId];

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();


	//----------------------------------------------
	// 인벤에 실제로 해당 물품이 있는지 확인한다. 
	//----------------------------------------------
	SI32 frompos = siItemPos;
	if(frompos < 0 || frompos >= MAX_ITEM_PER_PERSON)return FALSE;

	cltItem* pclitem = &pclCM->CR[siCharId]->pclCI->clCharItem.clItem[frompos];

	// 같은 아이템인지 비교한다. 
	cltItem clitem1;
	clitem1.Set(pclInvenItem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(pclitem);
	clitem2.SetItemNum(1);

	if(clitem1.IsSame(&clitem2, false) == FALSE)return FALSE;

	// 개수가 적절한지 확인한다. 
	if(pclInvenItem->GetItemNum() >  pclitem->GetItemNum())return FALSE;

	//------------------------------------------------------
	// 파티홀에서 받은 아이템은 저장할 수 없다.
	//------------------------------------------------------
	if( pclitem->clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )	
		return FALSE;

	// 귀속전용 개인상점
	if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == frompos	)
	{
		// 상점 아이템이 실제로 존재하는지 체크 
		if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
		{  
			SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, siCharId );
			return FALSE;
		}
	}

	//------------------------------------------------------
	//휴면 장비 아이템이라면 창고로 이동 불가
	//------------------------------------------------------
	if( NULL != this->pclDormancySystem)
	{
		CDormancyEquipItemRewardServer *pclRewardServer = (CDormancyEquipItemRewardServer *) this->pclDormancySystem->GetDormancyEquipItemReward();
		if( NULL != pclRewardServer)
		{
			if( TRUE == pclRewardServer->IsDormancyEquipItem(pclInvenItem->siUnique))
			{
				SendServerResponseMsg(0, SRVAL_FAIL_MOVE_ITEM_INHOUSE, 0, 0, siCharId );
				return FALSE;
			}
		}
	}

	//기간제 아이템의 경우 남은 사용기간이 있는지 확인한다.
	SI32 itemunique = pclInvenItem->siUnique ;
	if ( itemunique > 0 )
	{
		if ( pclInvenItem->IsHaveUseDay() )  //[영진] 남은 사용기간 없음 알림
		{
			SI32 useableDay = 0;
			BOOL rVal = GetItemUseAbleDay(pclInvenItem,&useableDay);
			if(rVal == FALSE) return FALSE;

			if( useableDay < 0) //사용기간이 지나서 지워준다 그리고 클라이언트에 알림.
			{
				//KHY - 0723 - 기간아이템 파괴 방지.
				if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
				{
					return FALSE;	
				}
				else
				{
					// 기간이 지난 고급품은 창고에 넣을 수 없다. - by LEEKH
					if( pclInvenItem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
					{
						// 메시지 출력
						return FALSE;
					}
					else
					{
						// 나머지 아이템은 지워준다.
						sDBRequest_UseItem useitem(siCharId,pclCM->CR[siCharId]->pclCI->GetPersonID(),frompos,pclInvenItem,USEITEM_REASON_DATEOVER);
						pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

						cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclInvenItem->siUnique, useableDay);
						cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
						pclCM->CR[siCharId]->SendNetMsg((sPacketHeader*)&clMsg );				
					}
					return FALSE;
				}
			}
		}
	}

	/*	SI32 NotUsingData;
	if ( pclItemManager->GetLimitDateItemValue( clitem1.siUnique, &NotUsingData ) )
	{
	return;
	}*/
	// SJY DEBUG

	/*cyj 이벤트 아이템유니크 삭제
	if ( clitem1.siUnique == ITEMUNIQUE(8150) ||
	clitem1.siUnique == ITEMUNIQUE(14300) ||
	clitem1.siUnique == ITEMUNIQUE(14400) ||
	clitem1.siUnique == ITEMUNIQUE(14500) ) // 반격의깃발 등
	{
	return ;
	}
	*/
	SI32 PersonID		= pclchar->pclCI->GetPersonID()	;

	// 해당창고가 사용자의 소유인지 확인한다.
	if(pclHouseManager->IsHouseHost(personid, siVillageUnique, siHouseIndex) == false)return FALSE;

	// 시전을 팔려고 내놨는지 확인
	for ( SI32 i=0 ; i<MAX_REALESTATEMARKET_ORDER_NUMBER;i++) 
	{
		if ( siVillageUnique > 0 &&  PersonID> 0 ) 
		{
			if( pclRealEstateMarketManager->pclRealEstateOrder[i].siVillageUnique == siVillageUnique )
			{
				if(pclRealEstateMarketManager->pclRealEstateOrder[i].siPersonID ==   PersonID && 
					pclRealEstateMarketManager->pclRealEstateOrder[i].siHouseIndex == siHouseIndex)
				{
					return	FALSE;
				}
			}			
		}
	}

	if(pclRealEstateMarketManager->IsExistOrder(personid, siVillageUnique, siHouseIndex))
	{
		cltItemInfo* pcliteminfo = pclItemManager->FindItemInfo( pclInvenItem->siUnique );
		if (pcliteminfo == NULL) return FALSE;

		if (pcliteminfo->IsItemInfoAtb(ITEMINFOATB_NOTTRADE) ||pclInvenItem->IsBelonging(pclItemManager))
		{
			SendServerResponseMsg(RANKTYPE_HOUSE, SRVAL_HOUSE_FAILINPUT_NOTRADEITEM,  0, 0, pclCM->CR[siCharId]->GetCharUnique());
			return FALSE;
		}
	}

	// 해당 창고에 빈자리가 있는지 확인한다. 
	SI32 sitopos;
	if(pclHouseManager->CanAddStg(siVillageUnique, siHouseIndex, pclItemManager, pclInvenItem, &sitopos) == false)
	{
		SendServerResponseMsg( RANKTYPE_HOUSE, SRVAL_HOUSE_FAILINPUT_FULLSTG,  0, 0, pclCM->CR[siCharId]->GetCharUnique() );
		return FALSE;
	}

	if( true == bNoMoveNewPos)
	{
		cltItem *pclItem = pclHouseManager->GetStgItemPointer(siVillageUnique, siHouseIndex, sitopos);
		if(NULL == pclItem)
			return FALSE;
		if( 0 == pclItem->siUnique)
			return FALSE;
	}

	if( true == pclchar->bPersonalShopSwitch ||
		true == pclchar->bPremiumPersonalShop )
	{
		for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
		{
			if( pclchar->clShopData.m_clItem[i].siUnique == pclInvenItem->siUnique  )
			{
				SendServerResponseMsg(0, SRVAL_FAIL_PERSONALSHOP_MOVEITEM, 0, 0, siCharId);
				return FALSE;
			}
		}
	}

	//----------------------------------------------
	// 아이템을 옮기도록 DBManager에게 요청한다. 
	//----------------------------------------------
	sDBRequest_MoveItemInvToStg clMsg(siCharId, personid, frompos, siVillageUnique, siHouseIndex, sitopos, pclInvenItem, pclInvenItem->CanPile(pclItemManager));
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return TRUE;
}

// 창고에서 인벤으로 이템을 이동. 
void cltServer::DoMsg_GAMEMSG_REQUEST_MOVEITEMSTGTOINV(cltMsg* pclMsg, SI32 id)
{
	stHouseUnitData* pclinfo = (stHouseUnitData*)pclMsg->cData;

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;


	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER)	return ;


	//------------------------------------------
	// 창고에 실제 물품이 있는지 확인한다.
	//-----------------------------------------
	SI32 personid		= pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;



	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	SI32 villageunique	= pclinfo->si32VillageUnique;
	SI32 houseunitslot	= pclinfo->si32HouseUnitSlot;

	// 해당창고가 사용자의 소유인지 확인한다.
	if(pclHouseManager->IsHouseHost(personid, villageunique, houseunitslot) == false)return ;	
	// 아이템이 실제로 있는지 확인한다. 
	SI32 frompos = pclinfo->si32lParam1;
	if(frompos < 0 || frompos >= MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER)return ;

	cltItem* pclitem = pclHouseManager->GetStgItemPointer(villageunique, houseunitslot, frompos);
	if(pclitem == NULL)return ;

	// 창고는 제약하지 않는다.
/*	if(pclClient->IsCountrySwitch(Switch_FourLeaf))
	{
		//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
		if( ITEMUNIQUE(2755) == pclitem->siUnique || ITEMUNIQUE(2754) == pclitem->siUnique )	
		{	
			if( pclitem->siUnique != siFourLeafUnique )	
			{
				SendServerResponseMsg( 0, SRVAL_RETURN_NOT_USE_FOURLEAF, 0, 0, pclchar->GetID() );
				return;	
			}
		}
	}*/

	// 같은 아이템인지 비교한다. 
	cltItem clitem1;
	clitem1.Set(&pclinfo->clItem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(pclitem);
	clitem2.SetItemNum(1);

	if(clitem1.IsSame(&clitem2, false) == FALSE)return ;

	// 개수가 적절한지 확인한다. 
	if(pclinfo->clItem.GetItemNum() >  pclitem->GetItemNum())return ;

	//------------------------------------------
	// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
	//------------------------------------------
	SI16 invpos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &pclinfo->clItem, &invpos, &cltoitem, &rtnvalue) == false)return ;

	//------------------------------------------
	// 아이템을 옮기도록 DBManager에게 요청한다. 
	//------------------------------------------
	sDBRequest_MoveItemStgToInv clMsg(id, villageunique, houseunitslot, frompos, personid, invpos, &pclinfo->clItem, pclinfo->clItem.CanPile(pclItemManager));
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
} 

// 창고정보를 요청. 
void cltServer::DoMsg_GAMEMSG_REQUEST_STGINFO(cltMsg* pclMsg, SI32 id)
{
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER)	return ;


	GameMsgRequest_HouseUnitStgInfo* pclinfo = (GameMsgRequest_HouseUnitStgInfo*)pclMsg->cData;
	SI32 villageunique = pclinfo->siVillageUnique;
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;
	SI32 houseunitslot = pclinfo->siHouseUnitSlot;
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return ;

	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if(pclHouseManager->IsHouseHost(personid, villageunique, houseunitslot) == false)return ;


	//------------------------------------------
	// 창고의 정보를 Client로 보낸다. 
	//------------------------------------------
	cltHouseUnitStorageInfo* pclstg = pclHouseManager->GetStgPointer(villageunique, houseunitslot);
	if(pclstg == NULL)return ;

	GameMsgResponse_HouseUnitStgInfo clInfo(villageunique, houseunitslot, pclstg);
	cltMsg clMsg(GAMEMSG_RESPONSE_STGINFO, sizeof(clInfo), (BYTE*)&clInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 시전에 마구간을 생성해 달라고 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITSTABLECREATE(cltMsg* pclMsg, SI32 id)
{
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER)	return ;


	cltGameMsgRequest_HouseUnitStableCreate *pclinfo = (cltGameMsgRequest_HouseUnitStableCreate *)pclMsg->cData;

	SI32 VillageUnique = pclinfo->siVillageUnique;
	if( VillageUnique <= 0 || VillageUnique >= MAX_VILLAGE_NUMBER )
	{
		return ;
	}
	SI32 HouseUnitSlot = pclinfo->siHouseUnitSlot;
	if( HouseUnitSlot < 0 || HouseUnitSlot >= MAX_HOUSEUNIT_NUMBER )
	{
		return ;
	}

	SI32 CreateStableIndex = pclinfo->siCreateStableIndex;
	if ( CreateStableIndex < 0 || CreateStableIndex >= MAX_HOUSEUNIT_HORSENUMBER )
	{
		return;
	}

	//------------------------------------------
	// 지을 마구간이 존재하는지 체크 ( 다 찼는 경우이거나, 빈 곳의 번호가 틀린 경우에는 그냥 return (차례로 넣으므로... ) )
	//------------------------------------------
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( VillageUnique, HouseUnitSlot );
	if( pclStg == NULL ) return ;

	for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
	{
		if ( pclStg->bCreatedHouseUnitStable[ i ] == false )
		{
			if ( i == pclinfo->siCreateStableIndex )
			{
				break;
			}
		}
	}

	// 마구간 지을 공간이 없거나, 빈 곳의 번호가 클라이언트가 보낸 것이랑 틀릴때
	if ( i == MAX_HOUSEUNIT_HORSENUMBER )
	{
		return;
	}

	/*
	//------------------------------------------
	// 마구간을 만들기 위한 세금(MAX_HOUSEUNIT_CREATESTABLEFEE) 이상의 금액을 소지하고 있는지 확인 
	//------------------------------------------
	if ( pclCM->CR[id]->pclCI->clIP.GetMoney() < MAX_HOUSEUNIT_CREATESTABLEFEE )
	{
	return;
	}
	*/

	//------------------------------------------
	// 이용권이 있는지 체크
	//------------------------------------------
	//SI16 ItemUnique = ITEMUNIQUE(24025);		// 시전개조이용권
	//SI32 ItemPos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( ItemUnique );
	//if ( ItemPos < 0 )
	//{
	//	return;
	//}

	//if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ].siItemNum < HOUSEUNIT_CREATESTABLE_USEITEMNUM )
	//{
	//	return;
	//}
	//------------------------------------------
	// 소지금이 있는지 체크
	//------------------------------------------
	GMONEY siNeedMoney = HOUSEUNIT_CREATESTABLE_NEEDMONEY * pclItemManager->clItemStatistic.siNPCPriceRate;

	if ( siNeedMoney == 0 || pclCM->CR[id]->pclCI->clIP.GetMoney() < siNeedMoney )
	{
		return;
	}

	//cltItem clTempItem;
	//clTempItem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ] );
	//clTempItem.siItemNum = HOUSEUNIT_CREATESTABLE_USEITEMNUM;


	/*
	// 마구간 짓는데서 마을 레벨 제한 해제
	//------------------------------------------
	// 마을 레벨이 맞는지 확인 ( 레벨 1에서는 마구간 1개만 2에서는 마구간 2개만 가능... )
	//------------------------------------------
	cltStrInfo *pStrInfo = GetStrInfo( VillageUnique, RANKTYPE_CITYHALL );

	if ( pStrInfo == NULL )
	{
	return;
	}

	// 마을 레벨이 0인경우
	if ( pStrInfo->clCityhallStrInfo.siVillageLevel < 1 )
	{
	SendServerResponseMsg(0, SRVAL_HOUSE_VILLAGELEVELLIMIT, 1, 0, id);
	return;
	}

	// 마을 레벨이 낮아서 마구간으로 증설할 수 없음.
	if ( i >= pStrInfo->clCityhallStrInfo.siVillageLevel )
	{
	SendServerResponseMsg(0, SRVAL_HOUSE_VILLAGELEVELLIMIT, pStrInfo->clCityhallStrInfo.siVillageLevel + 1, 0, id);
	return;
	}
	*/

	//------------------------------------------
	// 마구간을 생성하라고 DB 에 요청
	//------------------------------------------	
	sDBRequest_HouseUnitStableCreate clMsg( id, PersonID, VillageUnique, HouseUnitSlot, siNeedMoney, pclinfo->siCreateStableIndex );	
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_TAX, siNeedMoney);

	return;
}

// 시전에 말을 넣어 달라고 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITHORSEIN(cltMsg* pclMsg, SI32 id)
{
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill == 0)return ;

	// 말 넣기
	cltGameMsgRequest_HouseUnitHorseIn *pclinfo = (cltGameMsgRequest_HouseUnitHorseIn *)pclMsg->cData;

	SI32 VillageUnique = pclinfo->siVillageUnique;
	if( VillageUnique <= 0 || VillageUnique >= MAX_VILLAGE_NUMBER )
	{
		return ;
	}
	SI32 HouseUnitSlot = pclinfo->siHouseUnitSlot;
	if( HouseUnitSlot < 0 || HouseUnitSlot >= MAX_HOUSEUNIT_NUMBER )
	{
		return ;
	}

	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if( pclHouseManager->IsHouseHost( PersonID, VillageUnique, HouseUnitSlot ) == false )
	{
		return;
	}

	//------------------------------------------
	// Person에게 말이 있는지 확인 ( 말이 있어야 시전에 넣을 수 있으므로... )
	// Person에게 말이 있다면, 말의 종류도 확인 ( 말의 상세 정보까지는 확인 필요없음 ) 사실, 말 종류 확인도 필요없음.
	// Person이 말에 타고 있다면 실패 해버림
	// 말 인벤토리에 아이템이 있다면 실패 해버림
	//------------------------------------------
	// 말에서 타고 있으므로...
	if ( pclCM->CR[id]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
	{
		return;
	}

	cltHorse* pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
	if ( pclHorse == NULL)
	{
		return;
	}
	SI32 horseindex	= pclCM->CR[id]->pclCI->clHorseInfo.siCurrentShowHorse;

	// 말의 종류가 틀림
	if ( pclHorse->siHorseUnique != pclinfo->siHorseUnique )
	{
		return;
	}


	// 말 인벤토리에 아이템이 있는지 확인
	if ( pclCM->CR[id]->pclCI->clCharItem.IsHorseHaveItem() == true )
	{
		return;
	}
	// 시전을 팔려고 내놨는지 확인
	for ( SI32 i=0 ; i<MAX_REALESTATEMARKET_ORDER_NUMBER;i++) 
	{
		if ( VillageUnique > 0 && PersonID > 0 ) 
		{
			if( pclRealEstateMarketManager->pclRealEstateOrder[i].siVillageUnique == VillageUnique )
			{
				if(pclRealEstateMarketManager->pclRealEstateOrder[i].siPersonID == PersonID && 
				pclRealEstateMarketManager->pclRealEstateOrder[i].siHouseIndex == HouseUnitSlot)
				{
					return	;
				}
			}			
		}
	}

	//------------------------------------------
	// 시전에 말을 넣을 수 있는 곳을 찾는다.
	//------------------------------------------
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( VillageUnique, HouseUnitSlot );
	if( pclStg == NULL ) return ;

	/*
	SI16 HorseAvailableAddPos = -1;
	if ( pclStg->CanAddHorse( &HorseAvailableAddPos ) == false )
	{		
	return;
	}
	*/

	SI16 HorseAvailableAddPos = pclinfo->siStgHorseIndex;
	// 말을 넣을 수 있는지 체크
	if ( pclStg->CanAddHorse( &HorseAvailableAddPos ) == false )
	{
		// 말이 있다면, 바꿀수 있으므로(change) 말이 있는지 체크	
		if ( ! pclStg->IsHorseExist( HorseAvailableAddPos ) )
		{
			return;
		}		
	}

	//------------------------------------------
	// 말을 시전으로 옮기기를 DB에 요청
	//------------------------------------------
	sDBRequest_HouseUnitHorseIn clMsg( id, PersonID, VillageUnique, HouseUnitSlot, horseindex, HorseAvailableAddPos );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return;
}

// 시전에서 말을 꺼내 달라고 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITHORSEOUT(cltMsg* pclMsg, SI32 id)
{
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER)	return ;


	cltGameMsgRequest_HouseUnitHorseOut *pclinfo = (cltGameMsgRequest_HouseUnitHorseOut *)pclMsg->cData;

	if ( pclinfo->siStgHorseIndex < 0 || pclinfo->siStgHorseIndex >= MAX_HOUSEUNIT_HORSENUMBER )
	{
		return;
	}

	if ( pclinfo->siStgHorseUnique < 1 )
	{
		return;
	}

	SI32 VillageUnique = pclinfo->siVillageUnique;
	if( VillageUnique <= 0 || VillageUnique >= MAX_VILLAGE_NUMBER )
	{
		return ;
	}
	SI32 HouseUnitSlot = pclinfo->siHouseUnitSlot;
	if( HouseUnitSlot < 0 || HouseUnitSlot >= MAX_HOUSEUNIT_NUMBER )
	{
		return ;
	}	

	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if( pclHouseManager->IsHouseHost( PersonID, VillageUnique, HouseUnitSlot ) == false )
	{
		return;
	}

	//------------------------------------------
	// 시전 말 리스트 그 위치에 그 말이 있는지 확인
	//------------------------------------------
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( VillageUnique, HouseUnitSlot );
	if( pclStg == NULL ) return ;

	if ( pclStg->clHorse[ pclinfo->siStgHorseIndex ].siHorseUnique < 1 ||
		pclStg->clHorse[ pclinfo->siStgHorseIndex ].siHorseUnique != pclinfo->siStgHorseUnique )
	{
		return;
	}

	/*
	// 사람에게 말이 있으면 시전에 그 위치에 있는 말과 바꾸기로 했음. (따라서 아래의 말이 있는지 없는지는 확인할 필요가 없음 )
	//------------------------------------------
	// Person에게 말이 없는지 확인 ( 말이 없어야 Person 에게 말을 줄 수 있으므로... )
	//------------------------------------------
	cltHorse* pclHorse = &pclCM->CR[id]->pclCI->clHorseInfo.clHorse;
	if ( pclHorse == NULL )
	{
	return;
	}
	*/	

	SI32 index = pclCM->CR[id]->pclCI->clHorseInfo.FindEmptyHorseIndex();
	if( index < 0 )
	{
		cltHorse* pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
		if ( pclHorse == NULL)
		{
			return;
		}

		SI32 horseindex	= pclCM->CR[id]->pclCI->clHorseInfo.siCurrentShowHorse;

		sDBRequest_HouseUnitHorseOut clMsg( id, PersonID, VillageUnique, HouseUnitSlot, pclinfo->siStgHorseIndex, horseindex );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		return;
	}
	else
	{
		//------------------------------------------
		// 시전에서 말을 꺼내서 사람에게 넣어 주기를 DB에 요청한다.
		//------------------------------------------
		sDBRequest_HouseUnitHorseOut clMsg( id, PersonID, VillageUnique, HouseUnitSlot, pclinfo->siStgHorseIndex, index );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	return;
}

// 시전에서 고물상을 생성하라고 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill <= 0 || curvill >= MAX_VILLAGE_NUMBER)	return ;

	cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate *pclinfo = (cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate *)pclMsg->cData;

	SI32 VillageUnique = pclinfo->siVillageUnique;
	if( VillageUnique <= 0 || VillageUnique >= MAX_VILLAGE_NUMBER )
	{
		return ;
	}

	SI32 HouseUnitSlot = pclinfo->siHouseUnitSlot;
	if( HouseUnitSlot < 0 || HouseUnitSlot >= MAX_HOUSEUNIT_NUMBER )
	{
		return ;
	}

	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if( pclHouseManager->IsHouseHost( PersonID, VillageUnique, HouseUnitSlot ) == false )
	{
		return;
	}

	//------------------------------------------
	// 고물상을 건설할 수 있는지 확인
	//------------------------------------------
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( VillageUnique, HouseUnitSlot );
	if( pclStg == NULL ) return ;


	if ( pclStg->bCreatedHouseUnitUsedGoodsBuilding )
	{
		return;
	}

	//------------------------------------------
	// 이용권이 있는지 체크
	//------------------------------------------
	SI16 ItemUnique = ITEMUNIQUE(24025);		// 시전개조이용권  14080 ->   24025
	SI32 ItemPos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( ItemUnique );
	if ( ItemPos < 0 )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ].siItemNum < HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM )
	{
		return;
	}

	cltItem clTempItem;
	clTempItem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ] );
	clTempItem.siItemNum = HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM;

	sDBRequest_HouseUnitUsedGoodsBuildingCreate clMsg( id, PersonID, VillageUnique, HouseUnitSlot, ItemPos, &clTempItem );
	pclGameDBProcess->SendMsg( (sPacketHeader *) &clMsg );

	return;
}

// 고물에서 아이템을 뽑아낼때 나올 아이템 정보
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill == 0)return ;

	cltGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo *pclinfo = (cltGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo*)pclMsg->cData;
	if ( pclinfo->clItem.siUnique < 1 || pclinfo->clItem.siItemNum < 1 )
	{
		return;
	}

	// 아이템 속성 확인
	if ( pclItemManager->GetItemAtb( pclinfo->clItem.siUnique ) & ITEMATB_OBJECT )
	{
	}
	else
	{
		return;
	}

	cltItem clItemList[ MAX_MATERIAL_NUMBER ];
	SI16 i = 0;

	for ( i = 0; i < MAX_MATERIAL_NUMBER; ++i )
	{
		clItemList[i].Init();
	}

	cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclItemManager;
	SI16 MakePercent = 0;
	if( ! pclitemmanager->GetDismantleMaterial( (cltCharServer *)pclCM->CR[id],  pclinfo->clItem, clItemList, &MakePercent ) )
	{
		return;
	}

	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsInfo clinfo( clItemList );
	cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

// 고물에서 아이템을 뽑아내라고 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 curvill = pclCM->CR[id]->GetCurrentVillageUnique();
	if(curvill == 0)return ;

	cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods *pclinfo = (cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods*)pclMsg->cData;

	SI32 VillageUnique = pclinfo->siVillageUnique;
	if( VillageUnique <= 0 || VillageUnique >= MAX_VILLAGE_NUMBER )
	{
		return ;
	}

	SI32 HouseUnitSlot = pclinfo->siHouseUnitSlot;
	if( HouseUnitSlot < 0 || HouseUnitSlot >= MAX_HOUSEUNIT_NUMBER )
	{
		return ;
	}

	if ( pclinfo->siItemPos < 0 || pclinfo->siItemPos >= MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER )
	{
		return;
	}

	if ( pclinfo->clItem.siUnique < 1 || pclinfo->clItem.siItemNum < 1 )
	{
		return;
	}

	// 아이템 속성 확인
	if ( pclItemManager->GetItemAtb( pclinfo->clItem.siUnique ) & ITEMATB_OBJECT )
	{

	}
	else
	{
		return;
	}

	//------------------------------------------
	// Person이 해당 창고의 주인인지 확인한다.
	//------------------------------------------
	if( pclHouseManager->IsHouseHost( PersonID, VillageUnique, HouseUnitSlot ) == false )
	{
		return;
	}

	//------------------------------------------
	// 고물상이 건설되어 있는지 확인
	//------------------------------------------
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( VillageUnique, HouseUnitSlot );
	if( pclStg == NULL ) return ;


	if ( ! pclStg->bCreatedHouseUnitUsedGoodsBuilding )
	{
		return;
	}

	//------------------------------------------
	// 시전에 소모될 아이템이 있는지 확인
	//------------------------------------------
	cltItem* pclitem = pclHouseManager->GetStgItemPointer( VillageUnique, HouseUnitSlot, pclinfo->siItemPos );
	if(pclitem == NULL)
		return;

	if ( ! pclitem->IsSame( &pclinfo->clItem, true ) )
	{
		return;
	}

	if ( pclCM->CR[id]->GetGameMasterLevel() <= BASIC_GAMEMASTER_LEVEL &&
		pclitem->GetMaxDurability() > DUR_FOR_DISMANTLE )
	{
		return;
	}

	SI16 ItemPosList[ MAX_MATERIAL_NUMBER ];
	cltItem clItemList[ MAX_MATERIAL_NUMBER ];
	SI16 i = 0;

	for ( i = 0; i < MAX_MATERIAL_NUMBER; ++i )
	{
		ItemPosList[i] = -1;
		clItemList[i].Init();
	}

	cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclItemManager;
	SI16 MakePercent = 0;
	//if( ! pclitemmanager->GetDismantleMaterial( (cltCharServer *)pclCM->CR[id],  pclinfo->clItem, clItemList, &MakePercent ) )
	//{
	//	return;
	//}

	SI16 Counter = 0;
	SI16 PreCounter = 0;
	bool InputAvailable = false;

	for ( SI16 j = 0; j < MAX_MATERIAL_NUMBER; ++j )
	{
		if ( clItemList[ j ].siUnique > 0 )
		{
			InputAvailable = false;

			for ( i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
			{
				if ( pclStg->clItem[ i ].siUnique == clItemList[ j ].siUnique )
				{
					// 아이템 중첩 개수 초과
					if ( pclStg->clItem[i].siItemNum + clItemList[ j ].siItemNum >= MAX_ITEM_PILE_NUMBER )
					{
						return;
					}

					ItemPosList[j]  = i;
					InputAvailable = true;
					break;
				}
				else if ( pclStg->clItem[ i ].siUnique == 0 && ItemPosList[j] == -1 )
				{
					if ( PreCounter > 0 )
					{
						--PreCounter;
						continue;
					}

					ItemPosList[j] = i;
					++Counter;
					PreCounter = Counter;
					InputAvailable = true;
				}
			}

			// 창고에 넣을 수 없음.
			if ( ! InputAvailable )
			{
				return;
			}
		}
	}



	sDBRequest_HouseUnitMakeItemFromUsedGoods clMsg( id, PersonID, 
													VillageUnique, HouseUnitSlot, 
													pclinfo->siItemPos, pclinfo->clItem.siUnique,
													ItemPosList, clItemList, MakePercent );
	pclGameDBProcess->SendMsg( (sPacketHeader *) &clMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 15] // 고물 하는 물품 MakePercent = 59% (회수율)
	//	// param1 == 부동산의 번호, 
	//	// param2 == 아이템 위치, 
	//	// param3 == 아이템 유니크,
	//	// param4 == 확률, 
	//	cltServer* pclserver = (cltServer*)pclClient;
	//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_SALVAGEFROM,
	//									0, (cltCharServer*)pclCM->CR[id], NULL, PersonID, 0, 0, clItemList, 0, 0, 0, VillageUnique, 0, 0, 
	//									HouseUnitSlot, pclinfo->siItemPos, pclinfo->clItem.siUnique, MakePercent, 0, NULL, NULL);
	//}

	return;
}

void cltServer::RefreshHouseUnit()
{
	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

		if(pclVillageManager->pclVillageInfo[i])
		{
			// 단위 시전의 정보를 요청한다. 
			{
				sDBRequest_ManyHouseUnit clMsg(0, i);
				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
			}
		}
	}
}

// 창고 아이템 정리
void cltServer::DoMsg_GAMEMSG_REQUEST_HOUSEITEMARRANGEMENT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ArrangeHouseItem* pclinfo = (cltGameMsgResponse_ArrangeHouseItem*)pclMsg->cData;

	cltCharServer* pclChar = pclCM->GetCharServer(id);

	if(NULL == pclChar)
		return;

	BOOL bArrangement = FALSE;

	for(SI32 i = 0; i < MAX_ITEM_PER_PERSON; i++)
	{
		cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[i];

		if ( TRUE == MOVEITEMINVTOSTG(id , pclinfo->m_siVillageUnique, pclinfo->m_siHouseIndex, i, pclitem, true))
		{
			bArrangement = TRUE;
		}
	}

	if( FALSE == bArrangement)
	{
		SendServerResponseMsg(0, SRVAL_HOUSE_FAILARRANGEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
	
}

