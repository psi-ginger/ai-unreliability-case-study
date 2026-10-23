// server-land
#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Feast.h"
#include "NDate.h"

#include "MsgRval-Define.h"
#include "..\CommonLogic\Msg\\MsgType-Feast.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_RESERVE( cltMsg* pclMsg, SI32 id )
{

	cltGameMsgRequest_FeastReserve* pclinfo = (cltGameMsgRequest_FeastReserve*)pclMsg->cData;
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	// 이미 예약자가 있으면
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus == 2 ) {

		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_RESERVE_ALREADY, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	// 연회 진행 중이라면,
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 0 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_RESERVE_OPEN, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	// 연회 날짜가 적절한가

	SYSTEMTIME systime;
	GetLocalTime( &systime );
	
	NDate date, resvdate;
	date.SetDate( systime.wYear, systime.wMonth, systime.wDay );
	resvdate.SetDate( pclinfo->siYear, pclinfo->siMonth, pclinfo->siDay );
	
	SI32 gday1 = date.GetDateVary();
	SI32 gday2 = resvdate.GetDateVary();

	if( gday1 > gday2 || gday1 + 1 < gday2 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_RESERVE_DATE, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if( gday1 == gday2 ) {
		if( pclinfo->siHour * 60 + pclinfo->siMinute <
			systime.wHour * 60 + systime.wMinute ) {

				SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_RESERVE_DATE, 0, 0, pclCM->CR[id]->GetCharUnique());	
				return;

			}
	}

	// 물품이 준비 되었는가
	if( pclStrInfo->clFeastStrInfo.CanResv() == FALSE ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_RESERVE_ITEM, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}


	GMONEY cost = pclStrInfo->clFeastStrInfo.clFeastInfo.siCost;

	// 예약 신청을 한다
	sDBRequest_FeastReserve clMsg( id, villageunique, personid, 
		pclinfo->siYear, pclinfo->siMonth, pclinfo->siDay, pclinfo->siHour, pclinfo->siMinute,
		pclinfo->szReason, cost );

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, cost);

    pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_ADDPRESENTLIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastAddPresentList* pclinfo = (cltGameMsgRequest_FeastAddPresentList*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)
		return ;
		
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	if( pclinfo->siItemNum == 0 ) return;

	// 연회중이거나 예약자가 있을경우에는 변경하지 못한다.
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 0 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_NOCHANGE, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	// 이미 등록된 아이템이면 

	for( int i = 0; i < MAX_FEAST_PRESENT_NUMBER; ++i ) {

		if( pclStrInfo->clFeastStrInfo.clFeastInfo.siItemUnique[ i ] == pclinfo->siItemUnique ) {

			SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_ITEM_ALREADY, 0, 0, pclCM->CR[id]->GetCharUnique());	
			return;
		}
	}


	if( pclItemManager->CanPile( pclinfo->siItemUnique ) == FALSE ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_ITEM_WRONG, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if( pclinfo->siItemNum > MAX_ITEM_PILE_NUMBER ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_ITEM_TOOMANY, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	// 리스트에서 빈 index를 구한다 
	SI32 index = pclStrInfo->clFeastStrInfo.clFeastInfo.GetEmptyPresentIndex();


	sDBRequest_FeastAddPresentList clMsg( id, villageunique, index, pclinfo->siItemUnique, pclinfo->siItemNum );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_DELPRESENTLIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastDelPresentList* pclinfo = (cltGameMsgRequest_FeastDelPresentList*)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	// 연회중이거나 예약자가 있을경우에는 변경하지 못한다.
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 0 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_NOCHANGE, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if( pclinfo->siListIndex < 0 || pclinfo->siListIndex >= MAX_FEAST_PRESENT_NUMBER )				return;

	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siItemUnique[ pclinfo->siListIndex ] != pclinfo->siItemUnique ) return;
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siItemNum[ pclinfo->siListIndex ] != pclinfo->siItemNum ) return;

	sDBRequest_FeastDelPresentList clMsg( id, villageunique, pclinfo->siListIndex );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}


void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_STGITEMIN( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastStgItemIn* pclinfo = (cltGameMsgRequest_FeastStgItemIn*)pclMsg->cData;
	

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 연회중이거나 예약자가 있을경우에는 변경하지 못한다.
	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 0 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_NOCHANGE, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if( pclItemManager->CanPile( pclinfo->siItemUnique ) == FALSE ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_ITEM_WRONG, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if( pclItemManager->IsItemInfoAtb( pclinfo->siItemUnique, ITEMINFOATB_NOTTRADE ) )
	{
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_ITEM_WRONG, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if ( pclinfo->siItemInventoryIndex < PERSONITEM_INV0 || pclinfo->siItemInventoryIndex > MAX_ITEM_PER_PERSON )
	{
		return;
	}

	if( pclinfo->siItemUnique < 1 )
	{
		return;
	}


	if ( pclinfo->siItemNum < 1 || pclinfo->siItemNum > MAX_ITEM_PILE_NUMBER )
	{
		return;
	}

	SI32 pos = -1;
	for (SI32 i = 0; i < MAX_FEAST_STG_NUMBER; i++)
	{
		if (pclStrInfo->clFeastStrInfo.clFeastStg[i].siItemUnique == pclinfo->siItemUnique)
		{
			pos = i;
		}
	}
	if (pos > 0 && pos < MAX_FEAST_STG_NUMBER)
	{
		if (pclStrInfo->clFeastStrInfo.clFeastStg[pos].siItemNum + pclinfo->siItemNum > MAX_ITEM_PILE_NUMBER)
		{
			SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_ITEM_TOOMANY, 0, 0, pclCM->CR[id]->GetCharUnique());	
			return;
		}
	}
	
	// 아이템 unique 확인
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemInventoryIndex ].siUnique != pclinfo->siItemUnique )
	{
		return;
	}

	// 아이템 개수가 적당한지 확인
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemInventoryIndex ].siItemNum < pclinfo->siItemNum )
	{
		return;
	}

	
	sDBRequest_FeastStgItemIn	clMsg( id, personid, villageunique, 
		pclinfo->siItemInventoryIndex, pclinfo->siItemUnique, pclinfo->siItemNum );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_STGITEMOUT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastStgItemOut* pclinfo = (cltGameMsgRequest_FeastStgItemOut*)pclMsg->cData;
	

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 연회중이거나 예약자가 있을경우에는 변경하지 못한다.
	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 0 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_NOCHANGE, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if ( pclinfo->siStgItemIndex < 0 || pclinfo->siStgItemIndex >= MAX_FEAST_STG_NUMBER )
	{
		return;
	}

	if( pclinfo->siStgItemUnique < 1 )
	{
		return;
	}


	if ( pclinfo->siStgItemNum < 1 || pclinfo->siStgItemNum > MAX_ITEM_PILE_NUMBER )
	{
		return;
	}


	// 아이템 unique 확인

	if ( pclStrInfo->clFeastStrInfo.clFeastStg[ pclinfo->siStgItemIndex ].siItemUnique != pclinfo->siStgItemUnique )
	{
		return;
	}

	// 아이템 개수가 적당한지 확인
	if ( pclStrInfo->clFeastStrInfo.clFeastStg[ pclinfo->siStgItemIndex ].siItemNum < pclinfo->siStgItemNum )
	{
		return;
	}

	//------------------------------------------
	// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
	//------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	SI16 invpos;
	cltItem cltoitem;
	cltItem clfromitem;
	clfromitem.siItemNum = pclinfo->siStgItemNum;
	clfromitem.siUnique = pclinfo->siStgItemUnique;
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_PERSON, &clfromitem, &invpos, &cltoitem, &rtnvalue) == false)return ;

	sDBRequest_FeastStgItemOut	clMsg( id, personid, villageunique, 
		pclinfo->siStgItemIndex, pclinfo->siStgItemUnique, pclinfo->siStgItemNum , invpos );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_SETCOST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastSetCost* pclinfo = (cltGameMsgRequest_FeastSetCost*)pclMsg->cData;
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;

	// 연회중이거나 예약자가 있을경우에는 변경하지 못한다.
	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 0 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_NOCHANGE, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	sDBRequest_FeastSetCost	clMsg( id, villageunique, pclinfo->siCost );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_GIVEITEM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastGiveItem* pclinfo = (cltGameMsgRequest_FeastGiveItem*)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	// 연회 중인가
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 1 ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_GIVEITEM_NOTOPEN, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	// 연회 예약자가 맞는가
	if( pclStrInfo->clFeastStrInfo.clFeastResvInfo.siPersonID != personid ) {
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_GIVEITEM_WRONGPERSON, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	if( pclinfo->siIndex < 0 || pclinfo->siIndex >= MAX_FEAST_PRESENT_NUMBER )		return;

	// 물품이 올바른 것인가
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siItemUnique[ pclinfo->siIndex ] != pclinfo->siItemUnique || 
		pclStrInfo->clFeastStrInfo.clFeastInfo.siItemNum[ pclinfo->siIndex ] < pclinfo->siItemNum ) {
			SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_GIVEITEM, 0, 0, pclCM->CR[id]->GetCharUnique());	
			return;
	}

	// 이미 지급한 물품은 아닌가
	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siItemGive[ pclinfo->siIndex ] == 1 ) 
	{
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_GIVEITEM_ALREADY, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;			
	}

	SI32 stgindex = 0;
	SI16 stgitemunique = 0, stgitemnum = 0;

	for( int i = 0; i < MAX_FEAST_STG_NUMBER; ++i ) {
		if( pclStrInfo->clFeastStrInfo.clFeastStg[ i ].siItemUnique == pclinfo->siItemUnique ) {
			stgindex = i;
			stgitemunique = pclinfo->siItemUnique;
			stgitemnum = pclStrInfo->clFeastStrInfo.clFeastStg[ i ].siItemNum;
			break;
		} 
	}

	if( i == MAX_FEAST_STG_NUMBER ) 
	{
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_GIVEITEM, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	sDBRequest_FeastGiveItem clMsg( id, villageunique, pclinfo->siIndex, pclinfo->siItemUnique, pclinfo->siItemNum,
		stgindex, stgitemunique, stgitemnum );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}


void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_SENDCHAT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastSendChat* pclinfo = (cltGameMsgRequest_FeastSendChat*)pclMsg->cData;

	TCHAR *pTitle = GetTxtFromMgr(6134);

	cltGameMsgResponse_Notice clNotice( pTitle, pclinfo->szText ,true);
	cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);
	
	if( pclClient->IsCountrySwitch( Switch_PartyHall ) )
	{
		SI32 SendUserMapIndex = pclCM->CR[id]->GetMapIndex();
		for(SI32 UserCount = 1; UserCount < MAX_PERSON_NUMBER; ++UserCount)
		{
			if(false == pclCM->IsValidID(UserCount))
				continue;

			if(pclCM->CR[UserCount]->GetMapIndex() == SendUserMapIndex)
			{
				pclCM->CR[UserCount]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
	else
	{
		// Person이 위치한 마을의 Unique를 구한다. 
		SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
		if(villageunique == 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

		cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

		if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 1 ) return;

		SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

		if( pclStrInfo->clFeastStrInfo.clFeastResvInfo.siPersonID != personid ) return;

		pclCM->SendAllPersonInVillageMsg((sPacketHeader*)&clMsg, villageunique);
	}
}

//#if defined(_LEON_FEAST_EFFECT)
void cltServer::DoMsg_GAMEMSG_REQUEST_FEAST_USEEFFECT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastUseEffect* pclinfo = (cltGameMsgRequest_FeastUseEffect*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)
		return ;

	// 연회중인지 체크
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return;

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_FEAST );

	if( pclStrInfo->clFeastStrInfo.clFeastInfo.siStatus != 1 ) return;

	// 연회진행자 인지 체크
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	if( pclStrInfo->clFeastStrInfo.clFeastResvInfo.siPersonID != personid ) return;

	// 폭죽이용권 체크
	// 폭죽이용권 번호 넣을 것
	SI16 itemunique = ITEMUNIQUE(24060); // 내자시폭죽이용권  14200 -> 24060

	SI32 pos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( itemunique );
	if( pos < 0 )
	{
		// 내자시폭죽이용권 없음
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_USEEFFECT_NOTHAVETICKET, 0, 0, pclCM->CR[id]->GetCharUnique());	
	}

	// 통계를 낸다. 
	pclItemManager->AddUseNum(itemunique, 1);

	// 새로운 통계
	//NStatistics_Add("Item", itemunique, "UseNum", 1);

	cltItem clitem;
	clitem.Init();

    clitem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[ pos ] );
	clitem.SetItemNum( 1 );

#ifdef _SAFE_MEMORY
	sDBRequest_FeastUseEffect sendMsg( id, personid, villageunique, pos, &clitem, &pclinfo->siEffectNum[0] );
#else
	sDBRequest_FeastUseEffect sendMsg( id, personid, villageunique, pos, &clitem, pclinfo->siEffectNum );
#endif
	pclGameDBProcess->SendMsg( (sPacketHeader*) &sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_EXIT( cltMsg* pclMsg, SI32 id )
{
	if(pclCM->IsValidID(id) == false)
		return ;

	SI32		siWarpX = 12 + (rand() % 5);   
	SI32		siWarpY = 12 + (rand() % 5);
	cltWarpInfo clWarpPirateShip(1, 158, false, false, siWarpX, siWarpY, -1, -1);

	pclCM->CR[id]->SetWarpInfo( &clWarpPirateShip, 1024 );
	pclCM->CR[id]->Warp();

}


void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_GET_COSTUME( cltMsg* pclMsg, SI32 id )
{
	if(pclCM->IsValidID(id) == false)
		return ;

	SI32* pInfo = (SI32*)pclMsg->cData;
	
	SI32 ItemUnique = *pInfo;

	if( 0 >= ItemUnique )
		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ id ];
    
	// 파티홀 의복은 소유개수에 제한이 있다.
	SI32 siHaveCount = 0;
	siHaveCount += pclchar->pclCI->clCharItem.GetItemNum(ItemUnique);
	siHaveCount += pclchar->pclCI->clCharItem.GetItemNumInEquip(ItemUnique);

	// 현재 1개까지만 소유할 수 있다.
	if( siHaveCount >= 1 )
	{
		return;
	}
	
	pclchar->GiveItem(ItemUnique, 1, INPUTITEMPERSON_OPTION_PARTYHALL_ITEM, 0);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_TOTAL_INFO( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_Total_Info* pclinfo = (cltGameMsgRequest_PartyHall_Total_Info*)pclMsg->cData;
	
	for(SI32 Count = 0; Count < MAX_PARTYHALL_MAP; ++Count)
	{
		cltGameMsgResponse_PartyHall_Total_Info cltInfo1( Count, 0, &pclVillageManager->pclFeastManager->clPartyHallResvInfo[Count][0] );
		cltGameMsgResponse_PartyHall_Total_Info cltInfo2( Count, 1, &pclVillageManager->pclFeastManager->clPartyHallResvInfo[Count][0] );
		cltGameMsgResponse_PartyHall_Total_Info cltInfo3( Count, 2, &pclVillageManager->pclFeastManager->clPartyHallResvInfo[Count][0] );

		cltMsg clMsg1(GAMEMSG_RESPONSE_PARTYHALL_TOTAL_INFO, sizeof(cltGameMsgResponse_PartyHall_Total_Info), (BYTE*)&cltInfo1);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

		cltMsg clMsg2(GAMEMSG_RESPONSE_PARTYHALL_TOTAL_INFO, sizeof(cltGameMsgResponse_PartyHall_Total_Info), (BYTE*)&cltInfo2);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);

		cltMsg clMsg3(GAMEMSG_RESPONSE_PARTYHALL_TOTAL_INFO, sizeof(cltGameMsgResponse_PartyHall_Total_Info), (BYTE*)&cltInfo3);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg3);
	}

	SI32 VillageUnique = pclClient->pclCM->CR[id]->GetCurrentVillageUnique();
	if(0 == VillageUnique)
		return;
	
	cltStrInfo*			  pStructInfo = pclClient->pclFeastManager->GetStrInfo(VillageUnique);
	if(NULL == pStructInfo)
		return;
	
	PartyHall_Decoration*	DecorationList = pStructInfo->clFeastStrInfo.clDecoration;
	if(NULL == DecorationList)
		return;

	
	cltGameMsgResponse_PartyHall_Decoration_List cltInfo4;
	for(SI32 Count = 0; Count < MAX_FEAST_STG_NUMBER; ++Count)
	{
		cltInfo4.m_clDecoration[Count].m_siDecorationKind = DecorationList[Count].m_DacorationKind;
		cltInfo4.m_clDecoration[Count].m_siDecorationNum  = DecorationList[Count].m_DacorationNum;
		cltInfo4.m_clDecoration[Count].m_siDecorationSlot = Count;
	}

	cltMsg clMsg4(GAMEMSG_RESPONSE_PARTYHALL_DECORATION_LIST, sizeof(cltGameMsgResponse_PartyHall_Decoration_List), (BYTE*)&cltInfo4);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GAMEMSG_REQUEST_PARTYHALL_RESERVATION( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_Reservation* pclinfo = (cltGameMsgRequest_PartyHall_Reservation*)pclMsg->cData;

	//  리스트 요청하는 코드 추가.	
	if(pclCM->IsValidID(id) == false)
		return ;
	
    SI32		Personid    = pclCM->CR[id]->pclCI->GetPersonID();

	switch( pclinfo->m_siEntranceType )
	{
		case TYPE_ENTRANCE_VILLAGE:	
			 {
				if(0 >= pclCM->CR[id]->pclCI->clBI.siHomeVillage)
					return;
			 }
			 break;
		case TYPE_ENTRANCE_GUILD:	
			 {
				 if(0 >= pclCM->CR[id]->pclCI->clBI.clGuildRank.siGuildUnitIndex)
					 return;
			 }
			 break;		
	}

	cltPatyHallResvInfo* pPatyHallResvInfo = NULL;
	
	for(SI32 MapCount = 0; MapCount < MAX_PARTYHALL_MAP; ++MapCount)
	{
		pPatyHallResvInfo = &pclClient->pclFeastManager->clPartyHallResvInfo[MapCount][0];
		if(NULL == pPatyHallResvInfo)
			continue;
		
		for(SI32 ResvCount = 0; ResvCount < MAX_PARTYHALL_RESERVATION_INFO_LIST; ++ResvCount)
		{
			// 이미 예약
			if( Personid == pPatyHallResvInfo[ResvCount].m_siPersonID)
			{
				SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ALREADY_USER, 0, pclCM->CR[id]->GetCharUnique());		
				return;
			}

			// 이미 예약이 되있는 시간이다.
			if( pclinfo->m_siPartyHallIndex ==	MapCount										&&
				pclinfo->m_siHour			==	pPatyHallResvInfo[ResvCount].m_SystemTime.wHour	&&
				0							<	pPatyHallResvInfo[ResvCount].m_siPersonID			)
			{
				SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ALREADY_USER, 0, pclCM->CR[id]->GetCharUnique());		
				return;  
			}
		}
	}
	
	

	sDBRequest_PartyHall_Reservation sendMsg( 
											  id, 
											  Personid, 
											  pclinfo->m_siYear,	
											  pclinfo->m_siMonth,
											  pclinfo->m_siDay,	
											  pclinfo->m_siHour,
											  pclinfo->m_siMinute,
											  pclinfo->m_szReason,
											  pclinfo->m_siDecorationKind,
											  pclinfo->m_siEntranceType,
											  pclinfo->m_siValueType,
											  pclinfo->m_siPartyHallIndex,
											  pclinfo->m_ResvMoney,
											  pclinfo->m_siDecorationSlotNum,
											  pclCM->CR[id]->GetCurrentVillageUnique()
											); 
	pclGameDBProcess->SendMsg( (sPacketHeader*) &sendMsg );		

}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_ADD_PRESENT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_Add_Present* pclinfo = (cltGameMsgRequest_PartyHall_Add_Present*)pclMsg->cData;

	//  리스트 요청하는 코드 추가.	  
	if(pclCM->IsValidID(id) == false)
		return ;

	SI32	Personid = pclCM->CR[id]->pclCI->GetPersonID(); 
	
	SI32 ResvIndex = pclClient->pclFeastManager->GetReveIndex(pclinfo->m_siPartyHallIndex, Personid);
	if(0 > ResvIndex)
		return;
	
	cltPatyHallResvInfo* pPatyHallResvInfo = &pclClient->pclFeastManager->clPartyHallResvInfo[pclinfo->m_siPartyHallIndex][ResvIndex];
	if(NULL == pPatyHallResvInfo)
		return;
		
	sDBRequest_PartyHall_Add_Present sendMsg(
												id, 
												pclinfo->m_siPartyHallIndex, 
												pclinfo->m_siItemSlotIndex,
												pclinfo->m_siItemUnique,
												pclinfo->m_siItemNum,
												&pPatyHallResvInfo->m_SystemTime,
												pclinfo->m_siPresentItemInvenPos
											);
				
	pclGameDBProcess->SendMsg((sPacketHeader*) &sendMsg);		
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_Entrance_Application* pclinfo = (cltGameMsgRequest_PartyHall_Entrance_Application*)pclMsg->cData;

	//  리스트 요청하는 코드 추가.	
	if(pclCM->IsValidID(id) == false)
		return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	
	SI32 ReservationListIndex = sTime.wHour;
	
	// 예약된 파티장 번호의 예약자 리스트를 포인터로 얻음.
	cltPatyHallResvInfo* pPatyHallResvInfo = &pclClient->pclFeastManager->clPartyHallResvInfo[pclinfo->m_siPartyIndex][ReservationListIndex];
	if( 0 >= pPatyHallResvInfo->m_siPersonID )
		return;
	
	// 예약된 곳의 입장 방식.(공개, 마을, 길드, 비공개)
	SI16 EntranceType = pPatyHallResvInfo->m_siEntranceType;  

	// 워프될 파티장의 위치.
	POINT* pPartyHallUserPos = &pclClient->pclFeastManager->m_PartyHallUserPos[0];	
	if(NULL == pPartyHallUserPos)
		return;
	 
	// 파티장을 예약한 유저는 기냥 이동. 
	if( personid == pPatyHallResvInfo->m_siPersonID )
	{
		cltWarpInfo clWarpPirateShip(	1, 
										PARTYHALL_START_MAP_INDEX + pclinfo->m_siPartyIndex, 
										false, 
										false, 
										pPartyHallUserPos[pclinfo->m_siPartyIndex].x, 
										pPartyHallUserPos[pclinfo->m_siPartyIndex].y, 
										-1, 
										-1	);

		pclCM->CR[id]->SetWarpInfo( &clWarpPirateShip, 1024 );
		pclCM->CR[id]->Warp();
		return;
	}

	switch( EntranceType )
	{
		case TYPE_ENTRANCE_PUBLIC:	
			{
				// 그냥 참가.
				cltWarpInfo clWarpPirateShip(	1, 
												PARTYHALL_START_MAP_INDEX + pclinfo->m_siPartyIndex, 
												false, 
												false, 
												pPartyHallUserPos[pclinfo->m_siPartyIndex].x, 
												pPartyHallUserPos[pclinfo->m_siPartyIndex].y, 
												-1, 
												-1	);

				pclCM->CR[id]->SetWarpInfo( &clWarpPirateShip, 1024 );
				pclCM->CR[id]->Warp();
			}
			break;
		case TYPE_ENTRANCE_VILLAGE:	
			{
				if( pPatyHallResvInfo->m_siValueType == pclClient->pclCM->CR[id]->pclCI->clBI.siHomeVillage )
				{
					cltWarpInfo clWarpPirateShip(	1, 
													PARTYHALL_START_MAP_INDEX + pclinfo->m_siPartyIndex, 
													false, 
													false, 
													pPartyHallUserPos[pclinfo->m_siPartyIndex].x, 
													pPartyHallUserPos[pclinfo->m_siPartyIndex].y, 
													-1, 
													-1	);

					pclCM->CR[id]->SetWarpInfo( &clWarpPirateShip, 1024 );
					pclCM->CR[id]->Warp();
				}
				else
				{
					SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ETRANCE_RESERVATION_FAIL, 0, pclCM->CR[id]->GetCharUnique());		
					return;
				}
			}
			break;
		case TYPE_ENTRANCE_GUILD:	
			{
				if( pPatyHallResvInfo->m_siValueType == pclClient->pclCM->CR[id]->pclCI->clBI.clGuildRank.siGuildUnitIndex )
				{
					// 길드가 같으므로 파티장으로.
					cltWarpInfo clWarpPirateShip(	1, 
													PARTYHALL_START_MAP_INDEX + pclinfo->m_siPartyIndex, 
													false, 
													false, 
													pPartyHallUserPos[pclinfo->m_siPartyIndex].x, 
													pPartyHallUserPos[pclinfo->m_siPartyIndex].y, 
													-1, 
													-1	);


					pclCM->CR[id]->SetWarpInfo( &clWarpPirateShip, 1024 );
					pclCM->CR[id]->Warp();
				}
				else
				{
					SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ETRANCE_RESERVATION_FAIL, 0, pclCM->CR[id]->GetCharUnique());		
					return;
				}
			}
			break;
		case TYPE_ENTRANCE_PRIVATE:	
			{
				SI32 EntranceApplicationIndex = -1;
			
				// 신청이 되있는지 그리고 안되있다면 비어있는 처음 리스트에 인덱스를 얻는다.
				for(SI32 ApplyCount = 0; ApplyCount < ENTRANCE_APPLICATION_LIST; ++ApplyCount)
				{
					//이미 신청을 했다 
					if(personid == pPatyHallResvInfo->m_clEntranceApplication[ApplyCount].m_siPersonID)
					{
						SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ETRANCE_RESERVATION, 0, pclCM->CR[id]->GetCharUnique());		
						return;
					}

					// 비어있는 입장 신청 리스트에 넣을 슬롯번호를 찾는다.
					if(  0 == pPatyHallResvInfo->m_clEntranceApplication[ApplyCount].m_siPersonID &&
						-1 == EntranceApplicationIndex												)
					{
						EntranceApplicationIndex = ApplyCount;		
					}
				}
				// -1이면 슬롯이 꽉차서 더이상 입장 예약 신청이 불가능 함. 
				if( -1 == EntranceApplicationIndex )
				{
					SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ETRANCE_RESERVATION_FAIL, 0, pclCM->CR[id]->GetCharUnique());		
					return;
				}
				
				// 해당 슬롯에 신청자를 넣는다.
				pPatyHallResvInfo->m_clEntranceApplication[EntranceApplicationIndex].m_siPersonID = personid;
				pPatyHallResvInfo->m_clEntranceApplication[EntranceApplicationIndex].m_siLevel	  = pclCM->CR[id]->pclCI->clIP.GetLevel();

				// 입장 예약 완료를 클라에게 보낸다.
				SendServerResponseMsg(0, SRVAL_FAIL_PARTYHALL,  PARTYHALL_ETRANCE_RESERVATION, 0, pclCM->CR[id]->GetCharUnique());		
			}
			break;
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION_LIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_Entrance_Application_List* pclInfo = (cltGameMsgRequest_PartyHall_Entrance_Application_List*)pclMsg->cData;

	//  리스트 요청하는 코드 추가.	
	if(pclCM->IsValidID(id) == false)
		return;
	
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltPatyHallResvInfo* pPartyHallResvInfo = NULL;
	cltPatyHallResvInfo* pPartyHallInfoList = (cltPatyHallResvInfo*)&pclClient->pclFeastManager->clPartyHallResvInfo[0];
	if(NULL == pPartyHallInfoList)
		return;

	SI32 ResvIndex = sTime.wHour;	// 시간이 파티장 예약 스롯번호와 같다.
	SI32 ResvId	   = 0;				// 예약자 아이디.
	
	// 클라에 보낼 메시지를 담좌~
	cltGameMsgResponse_PartyHall_Entrance_Application_List cltInfo;

	// 위에 클라에게 보낼 구조체에 입장 신청자 리스트의 포인터.
	Entrance_Application_Info*	pSendInfo = cltInfo.m_ApplicationArray;
	if(NULL == pSendInfo)
		return;

	cltPartyHallEntranceApplication*	pServerInfo = NULL;
	
	for(SI32 PartyHalCount = 0; PartyHalCount < MAX_PARTYHALL_MAP; ++PartyHalCount)
	{
		pPartyHallResvInfo = &pclClient->pclFeastManager->clPartyHallResvInfo[PartyHalCount][0];
		if(NULL == pPartyHallResvInfo)
			continue;
			
		pServerInfo	= &pPartyHallResvInfo[ResvIndex].m_clEntranceApplication[0];
		if(NULL == pServerInfo)
			continue;
		
		// 파티 주최자인 경우 입장 신청자 리스트를 준다.
		if(personid == pPartyHallResvInfo[ResvIndex].m_siPersonID)
		{
			for(SI32 ArrayCount = 0; ArrayCount < MAX_PARTYHALL_APPLICATION_LIST; ArrayCount++, pSendInfo++, pServerInfo++)
			{
				ResvId = pclCM->GetIDFromPersonID(pServerInfo->m_siPersonID);
				// id가 0 이면 예약자에서 제거.
				if(0 == ResvId)
				{
					pServerInfo->m_siPersonID = 0;
					pServerInfo->m_siLevel    = 0;
					continue;
				}
				//  접속이 끈어진 유저면 리스트에서 삭제.
				if(false == pclCM->IsValidID(ResvId))
				{
					pServerInfo->m_siPersonID = 0;
					pServerInfo->m_siLevel    = 0;
					continue;
				}
				
				// 최종적으로 통과하면 리스트에 담는다.
				pSendInfo->m_siPersonID = pServerInfo->m_siPersonID;
				pSendInfo->m_siLevel	= pServerInfo->m_siLevel;
				MStrCpy(pSendInfo->m_szID, pclCM->CR[ResvId]->GetName(), sizeof(pSendInfo->m_szID));
			}
						
			// 클라에 정보주기.			
			cltMsg clMsg(GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION_LIST, sizeof(cltGameMsgResponse_PartyHall_Entrance_Application_List), (BYTE*)&cltInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			break;
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_ADD_DECORATION( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_Add_Decoration* pclInfo = (cltGameMsgRequest_PartyHall_Add_Decoration*)pclMsg->cData;

	// 장식품 추가.
	if(pclCM->IsValidID(id) == false)
		return ;
	
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	
	// 장식물 포인터 얻기.
	PartyHall_Decoration* pDecorationList = pclClient->pclFeastManager->pclStruct[pclInfo->m_siViilageUnique]->clStrInfo.clFeastStrInfo.clDecoration;
	if(NULL == pDecorationList)
		return;

	// 같은 장식물이 있는지 확인.
	bool DecorationSame	   = false;	
	SI32 DecorationAddSlot = -1;	
	for( SI32 DecorationCount = 0; DecorationCount < MAX_PARTYHALL_DECORATION_ITEM; ++DecorationCount )
	{
		if( pclInfo->m_siDecorationKind == pDecorationList[DecorationCount].m_DacorationKind )
		{
			DecorationAddSlot = DecorationCount;
			DecorationSame    = true;
			break;
		}
	}
		
	// 같은 장식물이 없으면 장식물을 넣을 빈 위치를 찾음.
	if(false == DecorationSame)	
	{
		for( SI32 DecorationCount = 0; DecorationCount < MAX_PARTYHALL_DECORATION_ITEM; ++DecorationCount )
		{
			if( 0 >= pDecorationList[DecorationCount].m_DacorationKind )
			{
				DecorationAddSlot = DecorationCount;
				break;
			}
		}
	}

	sDBRequest_PartyHall_Add_Decoration sendMsg( id, personid, pclInfo->m_siViilageUnique, pclInfo->m_siDecorationKind,
															   pclInfo->m_siDecorationNum, DecorationAddSlot, pclInfo->m_DeleteItem );
	pclGameDBProcess->SendMsg( (sPacketHeader*) &sendMsg );
}

// 선물 주기.
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_GIVEITEM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PartyHall_GiveItem* pclInfo = (cltGameMsgRequest_PartyHall_GiveItem*)pclMsg->cData;
	
	if(pclCM->IsValidID(id) == false)
		return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltPatyHallResvInfo* pPartyHallResvInfoList	= NULL;
	cltPatyHallResvInfo* pPartyHallInfoList		= (cltPatyHallResvInfo*)&pclClient->pclFeastManager->clPartyHallResvInfo[0];
	if(NULL == pPartyHallInfoList)
		return;

	for(SI32 PartyHalCount = 0; PartyHalCount < MAX_PARTYHALL_MAP; ++PartyHalCount)
	{
		pPartyHallResvInfoList = &pclClient->pclFeastManager->clPartyHallResvInfo[ PartyHalCount ][ 0 ];
		if(NULL == pPartyHallResvInfoList)
			continue;

		for(SI32 ResvCount = 0; ResvCount < MAX_PARTYHALL_RESERVATION_INFO_LIST; ++ResvCount )
		{
			if( personid == pPartyHallResvInfoList[ResvCount].m_siPersonID )
			{
				// 해당 파티장의 등록된 선물을 DB에 삭제 요청.
				sDBRequest_PartyHall_GiveItem sendMsg;
				sendMsg.m_siPartyHallIndex  = PartyHalCount;
				sendMsg.m_siPersonID		= personid;
				memcpy(&sendMsg.m_SystemTime, &sTime, sizeof(sendMsg.m_SystemTime));

				//[진성] 파티장에 선물을 나눠줄때 년, 월, 일, 시만 세팅하고 나머지는 0으로 채운다. => 2008-9-12
				//		 예약된 정보가 모두 정각으로 되있기 때문에.
				sendMsg.m_SystemTime.wDayOfWeek		= 0;
				sendMsg.m_SystemTime.wMinute		= 0;
				sendMsg.m_SystemTime.wSecond		= 0;
				sendMsg.m_SystemTime.wMilliseconds	= 0;

				pclGameDBProcess->SendMsg( (sPacketHeader*) &sendMsg );
				return;
			}
		}
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_INVITATION( cltMsg* pclMsg, SI32 id )
{
	SI32* pclInfo = (SI32*)pclMsg->cData;

	SI32 PersonID = *pclInfo;

	SI32 ResvID = pclCM->GetIDFromPersonID(PersonID);	// 예약자의 아이디.

	cltPatyHallResvInfo*				pPartyHallResvInfo   = NULL;
	Entrance_Application_Info*			pSendInfo			 = NULL;
	cltPartyHallEntranceApplication*	pApplication		 = NULL;


	// 클라에 새로 갱신된 신청자리스트 정보를 담을 구조체.
	cltGameMsgResponse_PartyHall_Entrance_Application_List cltInfo;

	SI32 ResvIndex = sTime.wHour;							// 현재 시간이 바로 예약자 배열의 인덱스.
	SI32 personid  = pclCM->CR[id]->pclCI->GetPersonID();	// 신청허가를 해주는 내 아이디.
	
	for(SI32 PartyHalCount = 0; PartyHalCount < MAX_PARTYHALL_MAP; ++PartyHalCount)
	{
		pPartyHallResvInfo = &pclClient->pclFeastManager->clPartyHallResvInfo[ PartyHalCount ][ 0 ];
		if(NULL == pPartyHallResvInfo)
			continue;

		pSendInfo = cltInfo.m_ApplicationArray;
		if(NULL == pSendInfo)
			continue;

		pApplication = &pPartyHallResvInfo[ ResvIndex ].m_clEntranceApplication[0];
		if(NULL == pApplication)
			continue;
		
		if(personid == pPartyHallResvInfo[ ResvIndex ].m_siPersonID)
		{
			for(SI32 ArrayCount = 0; ArrayCount < MAX_PARTYHALL_APPLICATION_LIST; ArrayCount++, pApplication++)
			{
				// 접속이 종료된 유저는 삭제한다.
				if(false == pclCM->IsValidID(pclCM->GetIDFromPersonID(pApplication->m_siPersonID)))
				{
					pApplication->m_siPersonID = 0;
					pApplication->m_siLevel    = 0;
					continue;
				}
				
				// 신청자를 이스트에서 삭제한다. 밑에서 ResvID로 신청자를 워프시킨다.
				if(PersonID == pApplication->m_siPersonID)
				{	
					pApplication->m_siPersonID = 0;
					pApplication->m_siLevel    = 0;
					continue;
				}		
				
				// 접속중인 신청자를 신청자 리스트에 담는다.
				pSendInfo->m_siPersonID = pApplication->m_siPersonID;
				pSendInfo->m_siLevel	= pApplication->m_siLevel;
				MStrCpy( pSendInfo->m_szID, pclCM->CR[pclCM->GetIDFromPersonID(pApplication->m_siPersonID)]->GetName(), sizeof(pSendInfo->m_szID) );
				pSendInfo++; 
			}
			break; // 이미 파티장을 찾고 신청자를 확인하여 리스트를 만들었으므로 반복문 종료.
		}
	}
	
	// 새로 바뀐 입장 신청자 리스트를 보내준다.
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION_LIST, sizeof(cltGameMsgResponse_PartyHall_Entrance_Application_List), (BYTE*)&cltInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
	// 신청자의 접속 여부를 확인한다.
	if(pclCM->IsValidID(ResvID) == false)
		return;

	// 파티장에 워프시킬 위치 리스트.
	POINT* pPartyHallUserPos = &pclClient->pclFeastManager->m_PartyHallUserPos[0];
	if(NULL == pPartyHallUserPos)
		return;
	
	// 파티장이 아니면 리턴.
	if( pclCM->CR[id]->GetMapIndex() < PARTYHALL_START_MAP_INDEX )
		return;

	// 파티장 번호로 워프시킬 위치를 얻기위해서...
	SI32 PosIndex = pclCM->CR[id]->GetMapIndex() - PARTYHALL_START_MAP_INDEX;

	cltWarpInfo clWarpPirateShip(1, pclCM->CR[id]->GetMapIndex(), false, false, 
									pPartyHallUserPos[PosIndex].x, pPartyHallUserPos[PosIndex].y, -1, -1);

	pclCM->CR[ResvID]->SetWarpInfo( &clWarpPirateShip, 1024 );
	pclCM->CR[ResvID]->Warp();
}

// 파티장 폭죽.		
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTYHALL_USEEFFECT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FeastUseEffect* pclinfo = (cltGameMsgRequest_FeastUseEffect*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)
		return;

	// 폭죽이용권 체크
	// 폭죽이용권 번호 넣을 것
	SI16 itemunique = ITEMUNIQUE(24060); // 내자시폭죽이용권  14200 -> 24060

	SI32 pos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( itemunique );
	if( pos < 0 )
	{
		// 내자시폭죽이용권 없음
		SendServerResponseMsg( RANKTYPE_FEAST, SRVAL_FEAST_FAIL_USEEFFECT_NOTHAVETICKET, 0, 0, pclCM->CR[id]->GetCharUnique());	
		return;
	}

	// 통계를 낸다. 
	pclItemManager->AddUseNum(itemunique, 1);

	cltItem clitem;
	clitem.Init();

	clitem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[ pos ] );
	clitem.SetItemNum( 1 );

	// 폭죽 아이템 사용.
	sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), pos, &clitem, USEITEM_REASON_NONE);
	pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);	

	
	SI16 siStartPosX = (SI16)pclCM->CR[id]->GetX();
	SI16 siStartPosY = (SI16)pclCM->CR[id]->GetY();

	// 폭중사용.
	cltGameMsgResponse_FeastUseEffect sendMsg( &pclinfo->siEffectNum[0], cltGameMsgRequest_FeastUseEffect::MAX_EFFECT_NUM_AT_ONCE, siStartPosX, siStartPosY);
	cltMsg clMsg( GAMEMSG_RESPONSE_FEAST_USEEFFECT, sizeof( sendMsg ), (BYTE*)&sendMsg );
	
	// 해당 맵에 있는 유저는 모두 폭죽을 보여준다.
	SI32 SendUserMapIndex = pclCM->CR[id]->GetMapIndex();
	for(SI32 UserCount = 1; UserCount < MAX_PERSON_NUMBER; ++UserCount)
	{
		if(false == pclCM->IsValidID(UserCount))
			continue;

		if(pclCM->CR[UserCount]->GetMapIndex() == SendUserMapIndex)
		{
			pclCM->CR[UserCount]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}



//#endif