//---------------------------------
// 2003/10/17 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"


#include "..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"

//-----------------------------------------
// CommonLogic
//-----------------------------------------
#include "MsgType-Structure.h"
#include "MsgRval-Define.h"
#include "MsgType-Person.h"

// 건물
#include "Cityhall\Cityhall.h"
#include "Bank\Bank-Manager.h"
#include "Stock\Stock.h"
#include "Market\Market.h"
#include "House\House.h"
#include "Hunt\Hunt.h"
#include "PostOffice\PostOffice.h"

#include "../Client/NInterface/NVillageDetailInfo/NVillageDetailInfo.h"

// 건물 정보 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_STRINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_StrInfo* pclInfo = (cltGameMsgRequest_StrInfo*)pclMsg->cData;

	UpdateStrInfoToClient( id, pclInfo->siStrType );
	
	// 수렵장인 경우, (어드벤티지를 전송한다.) 
	if(pclInfo->siStrType == RANKTYPE_HUNT)
	{
		cltMsg clMsg(GAMEMSG_RESPONSE_HUNTADDRATE, pclHuntManager->siHuntAddRate);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}

}
// 마을 상세 정보중에서 특정 건물의 정보를 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_VILLAGEINFOSTRINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_VillageInfoStrInfo* pclInfo = (cltGameMsgRequest_VillageInfoStrInfo*)pclMsg->cData;

	SI32 villageunique = pclInfo->siVillageUnique;
	if(villageunique <= 0)return ;
	if(villageunique >= MAX_VILLAGE_NUMBER)return ;

	// 마을의 villageunique 근거로 건물의 정보를 구한다. 
	cltStrInfo* pclSend = GetStrInfo( villageunique, pclInfo->siStrType);

	switch(pclInfo->siStrType)
	{
	case RANKTYPE_HUNT:
		if(pclSend)
		{
			cltGameMsgResponse_VillageInfoStrInfo msg( villageunique, pclInfo->siStrType, pclSend, 0, 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEINFOSTRINFO, sizeof(cltGameMsgResponse_VillageInfoStrInfo), (BYTE*)&msg);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			//cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEINFOSTRINFO, 	pclSend->GetTotalSize(), 	(BYTE*)pclSend);
			//pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		break;
	case RANKTYPE_CITYHALL:
		if(pclSend)
		{
			if( pclRankManager == NULL ) return;
			// 대행수가 존재 여부
			cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_CITYHALL, villageunique);
			SI32 chiefid = pclrank->clPerson.GetPersonID();
			if(chiefid < 0 ) chiefid = 0;

			cltCityHall *pclCityHall = NULL;
			pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];
			if(pclCityHall == NULL ) return;
			// 주주총회 여부 - 0 : 주총 아님, 1 : 주총 진행 중
			SI32 generalmeeting = pclCityHall->clGeneralMeetingInfo.siMatterType;

			cltGameMsgResponse_VillageInfoStrInfo msg( villageunique, pclInfo->siStrType, pclSend, chiefid, generalmeeting );
			cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEINFOSTRINFO, sizeof(cltGameMsgResponse_VillageInfoStrInfo), (BYTE*)&msg);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		break;
	}
}


// 건물 세금 납부 내역 요청 
void cltServer::DoMsg_GAMEMSG_REQUEST_GETTAXLIST(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgRequest_GetTaxList *pclInfo = (cltGameMsgRequest_GetTaxList *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// 마을의 villageunique 근거로 건물의 정보를 구한다. 
	cltStrInfo* pclinfo = GetStrInfo( villageunique, pclInfo->siStrType );

	if(pclinfo)
	{
		cltGameMsgResponse_GetTaxList pclMsg;

		pclMsg.siResult = 1;
		pclMsg.siVillageUnique = villageunique;
		pclMsg.siStrType = pclinfo->clCommonInfo.siStrType;
		pclMsg.clTaxInfo = pclinfo->clCommonInfo.clTaxInfo;

		cltMsg clNormalMsg( GAMEMSG_RESPONSE_GETTAXLIST, pclMsg.GetTotalSize(), (BYTE*)&pclMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
	}	

}

// 건물 세금 설정 
void cltServer::DoMsg_GAMEMSG_REQUEST_SETTAX( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SetTax *pclInfo = (cltGameMsgRequest_SetTax *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	UI16 usYear = pclTime->clDate.uiYear;

	sDBRequest_SetTax clMsg( id, villageunique, pclInfo->siStrType, usYear, &pclInfo->clDueTax );

	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

// 건물 세금 납부
void cltServer::DoMsg_GAMEMSG_REQUEST_PAYTAX(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgRequest_PayTax *pclInfo = (cltGameMsgRequest_PayTax *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// [영훈] 초보자 마을 : 초보자 마을은 건물세금과 행수 월급을 안준다
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( villageunique == Const_Beginner_Village )
		{
			return;
		}
	}

	// 금액이 10만냥 미만이면 세금 납부 안됨 

	if( pclInfo->clPayTax.itMoney < 100000 ) {
		SendServerResponseMsg( pclInfo->siStrType, SRVAL_TAX_FAIL_WRONGMONEY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}


	// 건물 내구도가 0이면 세금 납부안됨 
	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, pclInfo->siStrType );
	if( pclStrInfo == NULL ) return;

	if( pclStrInfo->clCommonInfo.siStrDura == 0 ) {
		SendServerResponseMsg( RANKTYPE_BANK, SRVAL_TAX_FAIL_PAYTAX_NODURA,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if( pclInfo->siStrType == RANKTYPE_BANK ) {

		if( (pclStrInfo->clCommonInfo.clMoney.itMoney - pclStrInfo->clBankStrInfo.clTotalInputMoney.itMoney) 
			< pclInfo->clPayTax.itMoney ) 
		{
			SendServerResponseMsg(RANKTYPE_BANK, SRVAL_TAX_FAIL_PAYTAX_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}
	}

	// 행수 급여 
	GMONEY siChiefPay;
	
	siChiefPay = (GMONEY)(pclInfo->clPayTax.itMoney * 0.2 );

	if( pclInfo->siStrType == RANKTYPE_HUNT ) {
		siChiefPay = (GMONEY)(pclInfo->clPayTax.itMoney * 0.3 );
	}

	sDBRequest_PayTax clMsg( id, villageunique, pclInfo->siStrType, &pclInfo->clPayTax, siChiefPay, pclTime->clDate.uiYear, &pclTime->clDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

	pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_MONTHMONEY, siChiefPay);

}

// 배당 내역 리스트 요청 
void cltServer::DoMsg_GAMEMSG_REQUEST_GETDIVIDENDLIST(cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_GetDividendList *pclInfo = (cltGameMsgRequest_GetDividendList *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);

	if(pclinfo)
	{
		cltGameMsgResponse_GetDividendList pclMsg;

		pclMsg.siResult = 1;
		pclMsg.siVillageUnique = villageunique;
		pclMsg.clDividendInfo = pclinfo->clCityhallStrInfo.clDividendInfo;

		cltMsg clNormalMsg( GAMEMSG_RESPONSE_GETDIVIDENDLIST, pclMsg.GetTotalSize(), (BYTE*)&pclMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
	}	


}

// 배당하기 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_PAYDIVIDEND(cltMsg *pclMsg, SI32 id )
{
	//cltGameMsgRequest_PayDividend *pclInfo = (cltGameMsgRequest_PayDividend *)pclMsg->cData;

	//// Person이 위치한 마을의 Unique를 구한다. 
	//SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	//if(villageunique <= 0)return ;
	//
	//
	//cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( villageunique );
	//if(pStrInfo == NULL)return ;

	//if( pStrInfo->clCityhallStrInfo.clDividendInfo.siCount > 0 ) {
	//	cltDate *pclDate = &pStrInfo->clCityhallStrInfo.clDividendInfo.clDividendUnitInfo[ 0 ].clDate;

	//	if( pclTime->clDate.IsLaterMonthThan( pclDate ) == FALSE ) {
	//		
	//		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_DIVIDEND_FAIL_DATEERROR,  0, 0, pclCM->CR[id]->GetCharUnique());
	//		return;
	//	}
	//}

	//// 대행수 급여 
	//SI32 siChiefPay = (SI32)(pclInfo->clTotalDividend.itMoney * 0.05);

	//// 군주(왕) 급여 
	//SI32 siKingPay = (SI32)(pclInfo->clTotalDividend.itMoney * 0.01);

	//// 부모 마을 조공 
	//SI32 siParentPay = (SI32)(pclInfo->clTotalDividend.itMoney * 0.1);
	//
	//SI32 siParentVillage = pclVillageManager->pclVillageInfo[ villageunique ]->clVillageVary.siParentVillage;
 //
	//if( siParentVillage == 0 ) siParentPay = 0;

	//sDBRequest_PayDividend clMsg( id, villageunique, &pclInfo->clTotalDividend, siChiefPay, 
	//	pclTime->clDate.uiYear, &pclTime->clDate, siParentVillage, siParentPay, siKingPay, 1 );

	//pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETSTRUCTUREMESSAGE( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_SetStructureMessage *pclInfo = (cltGameMsgRequest_SetStructureMessage *)pclMsg->cData;


	// 마을의 villageunique 근거로 건물의 정보를 구한다. 
	cltStrInfo* pclinfo = GetStrInfo( pclInfo->siVillageUnique, pclInfo->siStrType );
	
	if( pclinfo)
	{
#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			TCHAR ResultBuf[1024] = "";
			IAF::Filtering( pclInfo->szMessage, ResultBuf, sizeof(ResultBuf) );
			memcpy( pclInfo->szMessage, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
		}
#else
		//KHY - 0118 - 서버 욕설 필터링 추가.
		m_pCAbuseFilter->CheckAbuse(pclInfo->szMessage);
#endif
	
		memcpy( pclinfo->clCommonInfo.strNotice, pclInfo->szMessage, MAX_STR_NOTICE_SIZE );
		pclinfo->clCommonInfo.strNotice[ MAX_STR_NOTICE_SIZE - 1 ] = NULL;

		sDBRequest_SetStructureMessage clMsg( pclInfo->siVillageUnique, pclInfo->siStrType, pclInfo->szMessage );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
	}

	UpdateStrInfoToClient( id, pclInfo->siStrType );

	SendServerResponseMsg( pclInfo->siStrType, SRVAL_STRMSG_SUCCESS,  0, 0, pclCM->CR[id]->GetCharUnique());
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GETSTRNOTICE( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_GetStrNotice *pclInfo = (cltGameMsgRequest_GetStrNotice *)pclMsg->cData;

	cltStrInfo* pclinfo = GetStrInfo( pclInfo->siVillageUnique, pclInfo->siStrType );

	if( pclinfo ) {

		cltGameMsgResponse_GetStrNotice sendMsg;

		sendMsg.siVillageUnique = pclInfo->siVillageUnique;
		sendMsg.siStrType = pclInfo->siStrType;
		
		StringCchCopy( sendMsg.szStrNotice, MAX_STR_NOTICE_SIZE, pclinfo->clCommonInfo.strNotice );

		cltMsg clNormalMsg( GAMEMSG_RESPONSE_GETSTRNOTICE, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);

	}

}
void cltServer::DoMsg_GAMEMSG_REQUEST_CHIEFINFO(cltMsg* pclMsg, SI32 id)
{
	// 마을의 신원을 확인한다. 
	cltSimpleRank* pclrank = (cltSimpleRank*)pclMsg->cData;

	SI32 villageunique = pclrank->GetVillageUnique();
	if(villageunique == 0)return ;

	/*
	// 마을의 Chief 정보를 확보한다. 
	cltRank* pclRank = pclRankManager->GetRank(pclrank->siType, pclrank->siVillageUnique);
	if(pclRank)
	{
		cltRank clrank;
		clrank.Set(pclrank, &pclRank->clPerson);

		// 클라이언트로 정보를 보낸다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_CHIEFINFO, sizeof(cltRank), (BYTE*)&clrank);
		
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}
	*/
	
	sDBRequest_ChiefInfo clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclrank);
	pclGameDBProcess->SendMsg(DBSELECT_SUB, (sPacketHeader *)&clMsg);
	
}

// kkm
void cltServer::DoMsg_GAMEMSG_REQUEST_REPAIRSTRUCTURE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_RepairStructure *pclInfo = (cltGameMsgRequest_RepairStructure *)pclMsg->cData;

	if( pclInfo == NULL )
		return;

	if( pclInfo->siStrRepairAmount <= 0 || pclInfo->siStrRepairAmount > 1000 )
		return;

	// Person이 위치한 마을의 VillageUnique를 확보한다
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	//[진성] 통합마켓은 마을 순위가 20위 미만부터는 수리가 불가능하다.
	if( RANKTYPE_NEWMARKET == pclInfo->siRankType )
	{
		if( NEWMARKET_REPAIR_RANKING < pclVillageManager->GetVillageRank( villageunique ) )
		{
			SendServerResponseMsg( 0, SRVAL_NEWMARKET_REPAIR_FAIL,  100, 0, id );
			return;
		}
	}

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, pclInfo->siRankType );
	if( pclStrInfo == NULL ) return;

	SI32 tempLimitRepairAmount = pclStrInfo->clCommonInfo.siRepairStrDura + pclInfo->siStrRepairAmount;

	if( tempLimitRepairAmount > 1000 )
		return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	SI64 RepairMoney = 0;
	SI32 itemunique = ITEMUNIQUE(8074);  // 건물 수리 세트  - 클라에서 한번 더 체크한다. -바꿔줄때 함께 바꿔줘야한다.

	//KHY - 1102 - 마을 수리비 재조정. - 서버에서도 다시 계산한다.
	if (pclClient->IsCountrySwitch(Switch_RepairMoney) )
	{
		if(pclInfo->bItemUse)
		{
			if( pclCM->CR[id]->pclCI->clCharItem.GetItemNum(itemunique) < pclInfo->siStrRepairAmount ) //수리하고자 하는 포인트보다 수리 세트가 적다면. - 실패.
				return;			
		}
		else
		{
			if( pclTime->clDate.uiYear >= NEW_REPAIR_YEAR )
				RepairMoney = 50 * NEW_REPAIR_YEAR * pclInfo->siStrRepairAmount;
			//						tempNeedRepairMoney *= (140 * (NEW_REPAIR_YEAR/4));
			else if((pclTime->clDate.uiYear >= 0) &&(pclTime->clDate.uiYear < NEW_REPAIR_YEAR)) 
			{
				RepairMoney = 50 * (pclTime->clDate.uiYear) * pclInfo->siStrRepairAmount;
				//						tempNeedRepairMoney *= (140 * (pclCurrentDate->uiYear/4));
			}

			itemunique = 0; //미적용 국가는 0 이다.
		}
	}
	else
	{
		if( pclTime && pclTime->clDate.uiYear >= REPAIR_YEAR )
			RepairMoney = pclInfo->siStrRepairAmount * 1000;
		else if( pclTime && pclTime->clDate.uiYear >= REPAIR_YEAR - 1 )
			RepairMoney = pclInfo->siStrRepairAmount * 1000 * 8 / 10;
		else if( pclTime && pclTime->clDate.uiYear >= REPAIR_YEAR - 2 )
			RepairMoney = pclInfo->siStrRepairAmount * 1000 * 4 / 10;
		else if( pclTime && pclTime->clDate.uiYear >= REPAIR_YEAR - 3 )
			RepairMoney = pclInfo->siStrRepairAmount * 1000 * 2 / 10;
		else
			RepairMoney = pclInfo->siStrRepairAmount * 1000 / 10;

		itemunique = 0; //미적용 국가는 0 이다.
	}
	
	sDBRequest_RepairStructure clMsg( id, 
		villageunique,
		pclInfo->siRankType,
		personid,
		pclInfo->siStrRepairAmount,
		RepairMoney , itemunique);

	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}


void cltServer::DoMsg_GAMEMSG_REQUEST_VILLAGEBOARD(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
		return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
		return ;

	SI32 village = 0 ;
	SI32 level = 0 ;

	if ( pclCityHallManager->GetHighestLevelVillage(&village,&level) == false )
		return ;

	cltGameMsgResponse_VillageBoard clinfo(village);
	cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEBOARD, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return ;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CONTRIBUTION_ITEM_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Contribution_Item_List* pclinfo = (cltGameMsgRequest_Contribution_Item_List*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	cltContributionInfo* pclContributionInfo = m_clContributionEvent_Common.FindContributionInfo( pclinfo->m_siNPCKind );
	if( NULL == pclContributionInfo ) return;

	cltGameMsgResponse_Contribution_Item_List clinfo( pclContributionInfo->m_siItemUnique, pclContributionInfo->m_siItemNum );
	cltMsg clMsg( GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_LIST, PACKET(clinfo) );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	// DB에 보내버려 ㅋ 아무것도 넣지 안고 보내면 현재 기부된 정보를 얻을수있다.
	SI16 m_siItemNum[CONTRIBUTION_ITEM_NUM];
	SI32 m_siItemInvenPos[CONTRIBUTION_ITEM_NUM];
	ZeroMemory( m_siItemNum,		sizeof(m_siItemNum) );
	ZeroMemory( m_siItemInvenPos,	sizeof(m_siItemInvenPos) );
	sDBRequest_Contribution_Item_List_Input clInfo( id, 
													pclchar->pclCI->GetPersonID(),
													pclchar->GetCurrentVillageUnique(),
													pclinfo->m_siNPCKind,
													pclContributionInfo->m_siItemUnique,
													m_siItemNum,
													m_siItemInvenPos );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clInfo);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CONTRIBUTION_ITEM_INPUT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Contribution_Item_Input* pclinfo = (cltGameMsgRequest_Contribution_Item_Input*)pclMsg->cData;
	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	cltContributionInfo* pclContributionInfo = m_clContributionEvent_Common.FindContributionInfo( pclinfo->m_siNPCKind );
	if( NULL == pclContributionInfo ) return;

	// 자신의 마을과 자기가 현재 기부하려는 npc가 있는 마을이 같아야 한다. 즉, 내 마을만 기부 할 수 있다.
	if( pclContributionInfo->m_bMyVillage )
	{
		if( pclchar->GetCurrentVillageUnique() != pclchar->pclCI->clBI.siHomeVillage )	return;
	}

	// 기부가능한 아이템 유니크가 다르거나 개수가 초과되면 실패. 
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( pclContributionInfo->m_siItemUnique[siCount] != pclinfo->m_siItemUnique[siCount] )	return;
		if( pclContributionInfo->m_siItemNum[siCount]	  < pclinfo->m_siItemNum[siCount]	 )	return;
	}

	// 사용하는 아이템의 인벤토리에 존재 유무를 검사.
	SI32			siInvenPos  = 0;
	cltPItemInfo*	pclCharItem = &pclchar->pclCI->clCharItem;
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 >= pclinfo->m_siUseItemIvenPos[siCount] ) break;

		if( pclinfo->m_siUseItemIvenPos[siCount] )
		{
			siInvenPos = pclCharItem->FindItemInv( pclinfo->m_siItemUnique[siCount] );
			if( pclinfo->m_siUseItemIvenPos[siCount] != siInvenPos ) return;
		}
	}

	if( false == m_clContributionEvent_Common.IsContribution( pclinfo->m_siNPCKind, pclchar ) )
		return;

	// DB에 보내버려 ㅋ
	sDBRequest_Contribution_Item_List_Input clInfo( id, 
													pclchar->pclCI->GetPersonID(),
													pclchar->GetCurrentVillageUnique(),
													pclinfo->m_siNPCKind,
													pclinfo->m_siItemUnique,
													pclinfo->m_siItemNum,
													pclinfo->m_siUseItemIvenPos		);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clInfo);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_CONTRIBUTION_MAKE_NPC(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Contribution_Make_NPC* pclinfo = (cltGameMsgRequest_Contribution_Make_NPC*)pclMsg->cData;
	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	cltContributionInfo* pclContributionInfo = m_clContributionEvent_Common.FindContributionInfo( pclinfo->m_siNPCKind );
	if( NULL == pclContributionInfo ) return;

	// 자신의 마을과 자기가 현재 기부하려는 npc가 있는 마을이 같아야 한다. 즉, 내 마을만 기부 할 수 있다.
	if( pclContributionInfo->m_bMyVillage )
	{
		if( pclchar->GetCurrentVillageUnique() != pclchar->pclCI->clBI.siHomeVillage )	return;
	}

	if( false == m_clContributionEvent_Common.IsMakeNpc( pclinfo->m_siNPCKind, pclchar ) )
		return;
	

	sDBRequest_Contribution_Make_NPC clInfo(	id, 
												pclchar->pclCI->GetPersonID(),
												pclchar->GetCurrentVillageUnique(),
												pclinfo->m_siNPCKind,
												pclContributionInfo->m_siItemUnique,
												pclContributionInfo->m_siItemNum );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clInfo);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GIANTRACCOON_RESURRECT_EVENT_ITEM_GET(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	// 최초로 부활탑을 완성한 마을인지 확인.
	if( false == m_pclGiantRaccoonResurrectEvent->IsFirstMakeVillage( pclchar->pclCI->clBI.siHomeVillage ) ) return;

	if( 0    <= pclchar->pclCI->clCharItem.FindItemInv( ITEMUNIQUE(23499) ) ) return;
	if( true == pclchar->pclCI->clCharItem.HaveInEquip( ITEMUNIQUE(23499) ) ) return;
	

	// 현실시간  1일 
	pclchar->GiveItem( ITEMUNIQUE(23499), 1, INPUTITEMPERSON_OPTION_GIANTRACCOON_RESURRECT_EVENT, 1, GIVEITEM_SEAL_MODE_CANCEL );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_NPOTRACCOON_ITEM_GET(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	if( cltGiantRaccoonResurrectEvent::POTRACCOON_CONTRIBUTION_ITEM_NUM > pclchar->pclCI->clCharItem.GetItemNum( ITEMUNIQUE(13069) ) )	
		return;
	
	// 자이언트 라쿤 부활 알 세트 주기.
	if( GIVEITEM_RESULT_SUCCESS == pclchar->GiveItem( ITEMUNIQUE(3937), 1, INPUTITEMPERSON_OPTION_GIANTRACCOON_RESURRECT_EVENT, 0 ) )
	{
		// 주는데 성공하면 빵을 사용한다.
		SI32 siInvenPos = pclchar->pclCI->clCharItem.GetItemPos( ITEMUNIQUE(13069) );
		if( 0 > siInvenPos ) return;

		cltItem clUseItem;
		clUseItem.Set( &pclchar->pclCI->clCharItem.clItem[siInvenPos] );
		clUseItem.siItemNum = cltGiantRaccoonResurrectEvent::POTRACCOON_CONTRIBUTION_ITEM_NUM;

		sDBRequest_UseItem useitem(	pclchar->GetID(),
									pclchar->pclCI->GetPersonID(),
									siInvenPos,
									&clUseItem, 
									USEITEM_REASON_CONTRIBUTION_ITEM_INPUT	);

		pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

		SendServerResponseMsg( 0, SRVAL_GIANTRACCOONRESURRECTEVENT_GIVE_ITEM, 0, 0, pclchar->GetCharUnique() );
	}
}


