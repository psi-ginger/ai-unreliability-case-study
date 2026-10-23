// server-mine
#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Mine.h"

#include "MsgRval-Define.h"
#include "..\CommonLogic\Msg\\MsgType-Mine.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_CHANGECONTRACT(cltMsg* pclMsg, SI32 id )
{

	//메시지를 확보한다. 
	cltGameMsgRequest_MineChangeContract *pclpacket = (cltGameMsgRequest_MineChangeContract *)pclMsg->cData;

	if( pclpacket->clRentContract.IsValid() == FALSE ) return ;

	SI32 returnval	= SRVAL_MINE_FAIL_SETCONTRACT;

	//----------------------------------
	// 요청자가 행수인지 확인한다. 
	//----------------------------------
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique == 0 ) return ;

	cltSimpleRank clrank(RANKTYPE_MINE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		return ;
	}

	// 계약 기간을 확인한다. 
	if( pclpacket->clRentContract.siRentPeriod <= 0 || 
		pclpacket->clRentContract.siRentPeriod > MAX_RENT_PERIOD_YEAR ) return;
	
	// 임대금액을 확인한다. 
	if( pclpacket->clRentContract.siRentFee < MIN_MINE_RENT_FEE || 
		pclpacket->clRentContract.siRentFee > MAX_MINE_RENT_FEE) return ;

	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_MINE );

	if( pStrInfo == NULL ) return;

	if( pclpacket->clRentContract.siRentFee < pStrInfo->clMineStrInfo.clRentContract.siRentFee - 10000 ||
		pclpacket->clRentContract.siRentFee > pStrInfo->clMineStrInfo.clRentContract.siRentFee + 10000 ) {

		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_SETCONTRACT_WRONGFEE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if( pclTime->clDate.GetDateVary() < 
		pStrInfo->clMineStrInfo.clLastChangeDate.GetDateVary() + 30 ) {

		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_SETCONTRACT_DATE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	
	//-----------------------------------
	// 계약 종료일을 기록한다. 
	//-----------------------------------
	pclpacket->clRentContract.clDate.Set(&pclTime->clDate);
	pclpacket->clRentContract.clDate.AddMonth(pclpacket->clRentContract.siRentPeriod * 12);

	
	// DB매니저에게 계약 조건을 변경하도록 요청한다. 
	sDBRequest_MineChangeContract clMsg(id, pclpacket->siVillageUnique, &pclpacket->clRentContract, &pclTime->clDate );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_UNITLIST(cltMsg* pclMsg, SI32 id )
{

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID <= 0 ) return;
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	//------------------------------------------------------------
	// 해당 마을의에서 정보를 구해서 클라이언트로 보낸다. 
	//------------------------------------------------------------
	cltGameMsgResponse_MineUnitList clInfo( villageunique );

	cltMine *pclMine;
	pclMine = (cltMine *)pclMineManager->pclStruct[ villageunique ];

	for( SI32 i = 0; i < MAX_MINEUNIT_NUMBER; i++ ) {
		clInfo.clPerson[i].Set( &pclMine->GetMineUnit(i)->clPerson );
	}

	cltMsg clMsg( GAMEMSG_RESPONSE_MINE_UNITLIST, sizeof(clInfo), (BYTE*)&clInfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_UNITDETAIL(cltMsg* pclMsg, SI32 id )
{

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0)return ;
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	//-----------------------------------
	// 해당 마을의 행수인지 확인한다. 
	//-----------------------------------
	cltSimpleRank clrank(RANKTYPE_MINE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		return ;
	}

	//------------------------------------------------------------
	// 해당 마을의 광산에서 정보를 구해서 클라이언트로 보낸다. 
	//------------------------------------------------------------
	cltGameMsgResponse_MineUnitListDetail clInfo(villageunique);

	cltMine *pclMine = (cltMine *)pclMineManager->pclStruct[villageunique];

	for(SI32 i = 0;i < MAX_MINEUNIT_NUMBER;i++)
	{
		clInfo.clContract[i].Set( &pclMine->GetMineUnit(i)->clContract );
	}

	cltMsg clMsg(GAMEMSG_RESPONSE_MINE_UNITDETAIL, sizeof(clInfo), (BYTE*)&clInfo );	
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}


void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_RENTAPPLY(cltMsg* pclMsg, SI32 id )
{

	// 클라이언트가 보내온 계약 조건을 확인한다.
	cltGameMsgRequest_MineRentApply* pclpacket = (cltGameMsgRequest_MineRentApply*) pclMsg->cData;
	
	cltRentContract* pclcontract = &pclpacket->clRentContract;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	// 임대기간이 부적절하면 실패. 
	if( pclcontract->siRentPeriod <= 0 || 
		pclcontract->siRentPeriod > MAX_RENT_PERIOD_YEAR )return ;
	
	// 기본 레벨 조건을 충족시키지 못하면 실패. 
//	if( pclCM->CR[id]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE )return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 날짜를 얻어온다. 
	pclcontract->clDate.Set(&pclTime->clDate);
	

	// 해당 시전의 계약조건과 비교하여 동일하지 않으면 실패. 
	cltMine *pclMine;

	pclMine = (cltMine*)pclMineManager->pclStruct[ villageunique ];

	if( pclMine->IsSameContract( pclcontract ) == FALSE)
	{
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_APPLY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// 전장 계좌가 있어야 한다. 
	cltAccountNum*	pclAccountNum = &pclCM->CR[id]->pclCI->clBank.clAccountNum;
	if( pclAccountNum->GetAccountNum() == 0 )
	{
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_APPLY_NOACCOUNT,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// 이미 등록되어 있으면 실패. 
	cltRentApplyer* pclapplyer = pclMine->GetMineApplyerFromPersonID( personid );
	if( pclapplyer )
	{		
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_APPLY_ALREADYAPPLY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
			
	// 신청자로 등록한다.
	sDBRequest_MineRentApply clMsg( id, villageunique, personid, pclcontract );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_CANCELAPPLY(cltMsg* pclMsg, SI32 id )
{

	// personid확보. 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique == 0 )return ;

	// DB로 취소를 요청한다. 
	sDBRequest_MineCancelRentApply clMsg(id, villageunique, personid);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_APPLYERLIST(cltMsg* pclMsg, SI32 id )
{

	//----------------------------------
	// 요청자가 광산행수인지 확인한다. 
	//----------------------------------
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0) return;
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;
	
	
	cltSimpleRank clrank(RANKTYPE_MINE, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE) {
		return;
	}
	
	cltGameMsgResponse_MineApplyerList sendMsg( villageunique );

	cltMine *pclMine;

	// 해당 마을의 광산에서 정보를 구해서 클라이언트로 보낸다. 
	for( SI32 i = 0; i < MAX_APPLY_PER_MINE; i++ )
	{
		pclMine = (cltMine *)pclMineManager->pclStruct[ villageunique ];

		sendMsg.clMineApplyer[ i ].Set( pclMine->GetRentApplyer( i ) );			
	}

	cltMsg clMsg(GAMEMSG_RESPONSE_MINE_APPLYERLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
	
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_DELAPPLYER(cltMsg* pclMsg, SI32 id )
{
	// [추가 : 황진성 2007. 11. 2 임차 희망자 명단을 모조리 삭제하는 것을 수정. 선택한 유저의 슬롯번호다.]
	SI32 Slot = (SI32)*pclMsg->cData;


	// 마을의 유니크를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	// PersonID를 확보한다. 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;


	//-----------------------------------
	// 해당 마을의 광산 행수인지 확인한다. 
	//-----------------------------------
	cltSimpleRank clrank(RANKTYPE_MINE, villageunique);
	if(pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return ;
	}
	
	//-----------------------------------
	// DB에 명단을 삭제할 것을 요청한다.
	//-----------------------------------
	sDBRequest_MineDelApplyer clMsg(id, personid, villageunique, Slot);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MINE_RENT(cltMsg* pclMsg, SI32 id )
{

	// 임대 요청을 확보한다. 
	cltGameMsgRequest_MineRent* pclrent = (cltGameMsgRequest_MineRent *)pclMsg->cData;


	//----------------------------------
	// 요청자가 행수인지 확인한다. 
	//----------------------------------
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID <= 0) return;
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	cltSimpleRank clrank( RANKTYPE_MINE, villageunique );
	if( pclRankManager->IsRank( PersonID, &clrank) == FALSE )
	{
		return ;
	}
	
	//----------------------------
	// 빈광산이 있는지 확인한다. 
	//----------------------------
	cltMine *pclMine;
	pclMine = (cltMine *)pclMineManager->pclStruct[ villageunique ];

	if( pclMine == NULL ) return;

	if( pclMine->IsEmpty( pclrent->siIndex ) == FALSE )
	{
		// 빈 광산이 없어서 임대할 수 없다.
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_RENT_NOTEMPTY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//---------------------------------
	// 신청자가 유효한지 확인한다.
	//---------------------------------
	if(pclrent->siPersonID == 0)
	{
		// 임차 희망자가 유효하지 않다. 
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_RENT_INVALIDPERSONID,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	
	cltRentApplyer* pclapplyer = pclMine->GetMineApplyerFromPersonID( pclrent->siPersonID );
	if( pclapplyer == NULL )
	{
		// 임차 희망자가 유효하지 않다. 
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_RENT_INVALIDPERSONID,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	
	//---------------------------------
	// 임차인이 동의한 계약 내용과 동일한지 확인한다. 
	//---------------------------------
	// 계약 내용을 확정한다. 
	cltStrInfo* pclStrInfo = GetStrInfo( villageunique, RANKTYPE_MINE );
	if( pclStrInfo == NULL )return ;

	cltRentContract clcontract;
	clcontract.Set( &pclStrInfo->clMineStrInfo.clRentContract );

	if( pclapplyer->clContract.IsSame( &clcontract ) == false)
	{
		SendServerResponseMsg(RANKTYPE_MINE, SRVAL_MINE_FAIL_NOTSAMECONTRACT,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//--------------------------------
	// 빈 광산의 임차인을 등록한다. 
	//--------------------------------
	// 현재 날짜로 계약일을 설정한다. 
	clcontract.clDate.Set(&pclTime->clDate);

	// DBManager에게 임대를 요청한다.
	sDBRequest_MineRent clMsg( id, villageunique, pclrent->siPersonID, pclrent->siIndex, &clcontract );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);


}

