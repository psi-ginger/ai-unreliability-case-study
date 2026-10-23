//---------------------------------
// 2003/8/18 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\Common\Map\Map.h"
#include "../../Common/Skill/Skill-Manager.h"
#include "../Statistics/Statistics.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Server/AuthServer/AuthMsg.h"

//-----------------------------------
// DBManager
//----------------------------------
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-House.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"


//-----------------------------------
// CommonLogic 
//----------------------------------
#include "MsgRval-Define.h"
#include "MsgType-House.h"
#include "MsgType-Person.h"

// 시전을 회수한다 
void cltServer::DoMsg_DBMSG_RESPONSE_RECALLHOUSE(sPacketHeader* pPacket)
{
	sDBResponse_RecallHouse* pclMsg = (sDBResponse_RecallHouse*)pPacket;

	if(pclMsg->siResult <= 0)
	{
		if(pclMsg->siResult == -9) // 돈이 모질랐다
		{

		}
		else if(pclMsg->siResult == -10) // 아이템이 남아있다.
		{

		}
		else if(pclMsg->siResult == -11) // 말이 남아있다.
		{

		}
		return ;
	}

	SI32 personid		= pclMsg->siPersonID;
	SI32 villageunique	= pclMsg->siVillageUnique;
	SI32 houseindex		= pclMsg->siHouseIndex;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 10] // 창고 회수. 
		// param1		 == 대상이 되는 단위 시전의 인덱스.
		// pszCharParam1 == 이름.
		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_RECALL,
										pclMsg->siRecallReason, NULL, NULL, personid, 0, 0, NULL, 0, 0, 0, villageunique, 0, 0, 
										houseindex, 0, 0, 0, 0, pclMsg->szOwnerName, NULL);
	}

	//-------------------------------------------
	// 시전안의 물품을 역참으로 보낸다. 
	//-------------------------------------------
	/*	cltHouseUnit* punit =pclHouseManager->pclStruct[villageunique]->GetHouseUnit(houseindex);
	if(punit)
	{
	bool bfirstswitch = false;
	for(SI32 i = 0;i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER;i++)
	{
	cltItem* pclitem = &(punit->clStorageInfo.clItem[i]);

	if(pclitem->siUnique)
	{
	if(bfirstswitch == false)
	{
	bfirstswitch = true;
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(3792);
	TCHAR index[128]= TEXT(" ") ;

	sprintf(index,TEXT("%d"),houseindex +1);
	Nsprintf(buffer,pText,TEXT("village"), pclVillageManager->GetName(villageunique)   ,TEXT("index"),  index  ,NULL);

	//sprintf(buffer,pText,pclVillageManager->GetName(villageunique), houseindex +1);

	SendPostOfficeItemToPerson((TCHAR*)punit->clPerson.GetName(), pclitem, buffer);
	}
	else
	{
	SendPostOfficeItemToPerson((TCHAR*)punit->clPerson.GetName(), pclitem, NULL);
	}

	}
	}
	}
	*/

	// 시전을 회수한다. 
	pclHouseManager->RecallHouse(villageunique, houseindex);


	if(pclMsg->siRecallReason ==  HOUSERECALL_REASON_CANCELLATION)
	{
		// 퍼슨에게 성공 메시지를 보내준다.
		SI32 id = pclCM->GetIDFromPersonID(personid);
		
		if(pclCM->IsValidID(id)== false)return ;

		GMONEY housemoney	= pclMsg->siHouseMoney;
		GMONEY remainmoney = pclMsg->siRemainMoney;
		GMONEY penaltymoney = pclMsg->siPenaltyMoney;

		// 유저의 잔돈 세팅~!
		pclCM->CR[id]->pclCI->clIP.SetMoney(remainmoney);
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

		// 시전의 잔돈 세팅~!
		cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_HOUSE );
		if( pStrInfo ) {

			pStrInfo->clCommonInfo.clMoney.Set( housemoney );
		}

		SendServerResponseMsg(RANKTYPE_HOUSE,  SRVAL_HOUSE_SUCCESSCANCELLATION,0,0,pclCM->CR[id]->GetCharUnique());

		// 시전행수에게 쪽지를 보낸다.

		cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_HOUSE,villageunique);
		SI32 housePersonid = pclrank->GetPersonID();	// 시전 행수의 Persionid

		cltLetterMsg_RecallHouse clletter(pclMsg->siRecallReason, pclMsg->siVillageUnique, pclMsg->siHouseIndex,pclMsg->szOwnerName);
		SI32 letterid = SendLetterMsg(housePersonid, (cltLetterHeader*)&clletter);
		if(pclCM->IsValidID(letterid))	// letterid == 0 이라면 접속이 아되어있어서 DB로 간거다.
		{
			pclCM->CR[letterid]->pclCI->clRealEstate.Delete(REALESTATE_TYPE_HOUSE, pclMsg->siVillageUnique, pclMsg->siHouseIndex);
			pclCM->CR[letterid]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);
		}

	}
	else if( pclMsg->siRecallReason == HOUSERECALL_REASON_FEEDELAY || pclMsg->siRecallReason == HOUSERECALL_REASON_CONTRACTEND )
	{	
		// 해당 유저에게 쪽지를 보낸다. 
		cltLetterMsg_RecallHouse clletter(pclMsg->siRecallReason, pclMsg->siVillageUnique, pclMsg->siHouseIndex);
		SI32 letterid = SendLetterMsg(personid, (cltLetterHeader*)&clletter);
		if(pclCM->IsValidID(letterid))	// letterid == 0 이라면 접속이 아되어있어서 DB로 간거다.
		{
			pclCM->CR[letterid]->pclCI->clRealEstate.Delete(REALESTATE_TYPE_HOUSE, pclMsg->siVillageUnique, pclMsg->siHouseIndex);
			pclCM->CR[letterid]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);
		}

	}
	else
	{
		return; 
	}
	// 회수된 시전 개수를 통계낸다. 
	pclStatisticsManager->clDailyStatistics.siRecallHouseNumber++;

}

// 임대료를 지불한다. 
void cltServer::DoMsg_DBMSG_RESPONSE_PAYRENTFEE(sPacketHeader* pPacket)
{
	sDBResponse_PayRentFee* pclMsg = (sDBResponse_PayRentFee*)pPacket;

	SI32 id = 0;
	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		{
		}
		break;

	case 1:
		{

			// 지불자가 누군지 확인한다.
			if(pclMsg->usCharID)
			{
				id = pclMsg->usCharID;
			}
			else
			{
				id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
			}

			// 연체 횟수가 3회를 초과하는 경우 시전을 초기화한다. 
			if(pclMsg->siDelayNum > 3)
			{
				// DB매니저에게 시전을 회수할 것을 요청한다. 
				sDBRequest_RecallHouse clMsg(pclMsg->siPersonID, HOUSERECALL_REASON_FEEDELAY, pclMsg->siVillageUnique, pclMsg->siHouseIndex);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
			else
			{
				cltLetterMsg_PayRentFee clletter(pclMsg->siVillageUnique, pclMsg->siHouseIndex, pclMsg->siRentFee, pclMsg->siDelayNum, pclMsg->siDelayMoney);
				// 쪽지를 DB나 클라이언트에게 보낸다. 
				SendLetterMsg(pclMsg->siPersonID, (cltLetterHeader*)&clletter);
			}

			// 시전의 보유 자금을 업데이트 한다. 
			cltMoney clmoney;
			clmoney.Set(pclMsg->siHouseMoney);
			pclHouseManager->SetMoney(pclMsg->siVillageUnique, &clmoney);

			if(pclCM->IsValidID(id))
			{
				// 지불자의 은행잔고를 업데이트 한다. 
				pclCM->CR[id]->pclCI->clBank.SetMoney(pclMsg->siBankMoney);
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 10] // 시전의 임대료를 지불.
					// param1 == 대상이 되는 단위 시전의 인덱스,  
					// param3 == 연체 횟수. 
					// param4 == 연체금.
					cltServer* pclserver = (cltServer*)pclClient;
					pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_RENTFEEPAY,
													0, NULL, NULL, pclMsg->siPersonID, pclMsg->siRentFee, pclMsg->siBankMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
													pclMsg->siHouseIndex, 0, pclMsg->siDelayNum, pclMsg->siDelayMoney, 0, NULL, NULL);
				}
			}

			//returnval	= SRVAL_HOUSE_SUCCESSSETCONTRACT;
		}
		break;

	case -1:
		{
			// 연체 횟수가 3회를 초과하는 경우 시전을 초기화한다. 
			if( pclMsg->siDelayNum > 3)
			{
				// DB매니저에게 시전을 회수할 것을 요청한다. 
				sDBRequest_RecallHouse clMsg(pclMsg->siPersonID, HOUSERECALL_REASON_FEEDELAY, pclMsg->siVillageUnique, pclMsg->siHouseIndex);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
		break;
	}


}

// 시전 계약 조건을 설정한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGEHOUSECONTRACT(sPacketHeader* pPacket)
{

	sDBResponse_ChangeHouseContract* pclMsg = (sDBResponse_ChangeHouseContract*)pPacket;

	SI32 returnval = 0;
	SI32 id	= pclMsg->usCharID;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case 1:
		{
			// 해당하는 시전의 계약 조건을 변경한다. 
			pclHouseManager->SetRentContract(pclMsg->siVillageUnique, &pclMsg->clContract);

			if(pclCM->IsValidID(id)== false)return ;

			// 클라이언트에 변경된 계약조건을 보낸다. 
			if(pclCM->CR[id]->IsValidConnection())	
			{
				cltStrInfo* pclinfo = pclHouseManager->GetStrInfo(pclMsg->siVillageUnique);
				if(pclinfo == NULL)return ;

				cltMsg clMsg(GAMEMSG_RESPONSE_STRINFO,  pclinfo->GetTotalSize(), (BYTE*)pclinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			}

			returnval	= SRVAL_HOUSE_SUCCESSSETCONTRACT;
		}
		break;
	}
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_HOUSE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// 창고 이름 변경
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGEHOUSEUNITNAME(sPacketHeader* pPacket)
{
	sDBResponse_ChangeHouseUnitName* pclMsg = (sDBResponse_ChangeHouseUnitName*)pPacket;

	SI32 siId	= pclMsg->m_usCharID;

	if( pclMsg->m_siVillageUnique < 0 || pclMsg->m_siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if(pclMsg->m_siResult == 0)
	{
		return ;
	}

	SI32 siVillageunique = pclMsg->m_siVillageUnique;
	SI32 siHouseIndex = pclMsg->m_siIndex;
	if(pclHouseManager->pclStruct[siVillageunique] == NULL)return ;

	cltHouseUnit *pclHouseUnit = pclHouseManager->pclStruct[siVillageunique]->GetHouseUnit(siHouseIndex);
	if( NULL == pclHouseUnit)
		return;

	_tcscpy( pclHouseUnit->m_szName, pclMsg->m_szName);

	cltCharServer* pclChar = pclCM->GetCharServer(siId);
	if( NULL == pclChar)
		return;
	
	// 창고의 정보를 클라이언트에게 보내준다. 
	cltGameMsgResponse_ChangeHouseUnitName clinfo( siVillageunique, siHouseIndex, pclMsg->m_szName);
	cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITNAMECHANGE, sizeof(clinfo), (BYTE*)&clinfo);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);	
}


// 단위 시전 정보를 얻어올 것을 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYHOUSEUNIT(sPacketHeader* pPacket)
{
	sDBResponse_ManyHouseUnit* pclMsg = (sDBResponse_ManyHouseUnit*)pPacket;
	
	SI32 id	= pclMsg->usCharID;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 villageunique = pclMsg->siVillageUnique;
	if(pclHouseManager->pclStruct[villageunique] == NULL)return ;
	
	SI32 i;

	for(i = 0;i < MANYHOUSEUNIT_NUMBER;i++)
	{
		pclHouseManager->pclStruct[villageunique]->SetHouseUnit(i + pclMsg->siStartIndex, &pclMsg->clHouseUnit[i]);
	}
	//너무 많이 찍혀서 보류
	//pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyHouseUnit"));
}

// 임차 희망자 명단 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETHOUSEAPPLYERLIST(sPacketHeader* pPacket)
{
	sDBResponse_GetHouseApplyerList* pclMsg = (sDBResponse_GetHouseApplyerList*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_ITEM_PER_PERSON )		return;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 i;
	for(i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		if(pclHouseManager->pclStruct[pclMsg->siVillageUnique])
		{
			pclHouseManager->pclStruct[pclMsg->siVillageUnique]->SetHouseApplyer(i, &pclMsg->clApply[i].clPerson, &pclMsg->clApply[i].clContract);
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetHouseApplyerList"));

}
// 임차 희망자를 모두 삭제한다. 
void cltServer::DoMsg_DBMSG_RESPONSE_DELHOUSEAPPLYER(sPacketHeader* pPacket)
{
	sDBResponse_DelHouseApplyer* pclMsg = (sDBResponse_DelHouseApplyer*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siDelSlotNum < 0 || pclMsg->siDelSlotNum > MAX_APPLY_PER_HOUSE )			return;

	SI32 returnval = 0;
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)	return ;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case -1:
		returnval	= SRVAL_HOUSE_FAILDELHOUSEAPPLYER_NOTCHIEF;	// 시전 행수가 아니라 실패.(리턴한다는데 주의!!) 
		break;
	case 1:
		{
			cltHouse	*pclHouse = (cltHouse*) pclHouseManager->pclStruct[pclMsg->siVillageUnique];

			// PCK : 임차 희망자 명단중 삭제할 명단의 PersonID
			SI32	siDelPersonID = 0;

			if( pclMsg->siDelSlotNum >= 0 && pclMsg->siDelSlotNum < MAX_APPLY_PER_HOUSE )
			{
				siDelPersonID = pclHouse->clApply[pclMsg->siDelSlotNum].clPerson.GetPersonID();
			}

			// PCK : 임차 희망자중 삭제할 사람만 수정한다.
			if(siDelPersonID > 0)	pclHouse->DeleteApplyer(siDelPersonID);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 29] // 임차 희망자의 명단을 삭제.
				cltServer* pclserver = (cltServer*)pclClient;
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_DELETEHOUSERENTAPPLYER,
					0, (cltCharServer*)pclCM->CR[id], NULL, siDelPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					0, 0, 0, 0, 0, NULL, NULL);
			}

			// 클라이언트로 임차 희망자 명단을 모두 삭제하라는 메시지를 보낸다. 
			if(pclCM->IsValidID(id) == TRUE)
			{
				cltMsg clMsg(GAMEMSG_RESPONSE_DELHOUSEAPPLYER, sizeof(SI32), (BYTE*)&pclMsg->siDelSlotNum);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}

			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(RANKTYPE_HOUSE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

			returnval	= SRVAL_HOUSE_SUCCESSDELHOUSEAPPLYER;
		}
		break;
	}
}

// 임차 신청 취소에 대한 응답.
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSERENTAPPLYCANCEL(sPacketHeader* pPacket)
{
	sDBResponse_HouseRentApplyCancel* pclMsg = (sDBResponse_HouseRentApplyCancel*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_ITEM_PER_PERSON )		return;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case 1:
		{

			// 신청자를 등록한다.
			if(pclMsg->siVillageUnique)
			{
				pclHouseManager->pclStruct[pclMsg->siVillageUnique]->SetHouseApplyer(pclMsg->siIndex, &pclMsg->clApply.clPerson, &pclMsg->clApply.clContract);
			}

			returnval	= SRVAL_HOUSE_SUCCESSAPPLYCANCEL;
		}
		break;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_HOUSE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 18] // 임차 신청취소 로그쓰기. 
			// param1	 == 임차 희망자의 정보가 담긴 위치.
			// pszParam1 == 임대관련 시간.
			TCHAR pszCharParam1[50] = {'\0', };
			StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d : %d "),	
																		pclMsg->clApply.clContract.clDate.uiYear,
																		pclMsg->clApply.clContract.clDate.uiMonth,
																		pclMsg->clApply.clContract.clDate.uiDay,
																		pclMsg->clApply.clContract.clDate.uiHour,
																		pclMsg->clApply.clContract.clDate.uiMinute );


			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_RENTAPPLYCANCEL, 
								 0, (cltCharServer*)(pclCM->CR[pclMsg->usCharID]), NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								 pclMsg->siIndex, 0, 0, 0, 0, 
								 pszCharParam1, NULL);
		}
	}

}

//여기다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!창고!!!!!!!!!!!!!!!!!
// 임차 신청을 제출한 것에 대한 응답.
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSERENTAPPLY(sPacketHeader* pPacket)
{

	sDBResponse_HouseRentApply* pclMsg = (sDBResponse_HouseRentApply*)pPacket;
	SI32 returnval = 0;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_ITEM_PER_PERSON )		return;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case -1://유저가 임차신청을 할때 현제 가진 창고가 3개 까지만 가질수 있다.[2007.07.19 ]
		returnval	= SRVAL_HOUSE_FAILNOMOREREALESTATE;		// 부동산 개수를 초과해서 가질 수 없다. 
		break;
	case 1:
		{
			// 신청자를 등록한다.
			if(pclMsg->siVillageUnique)
			{
				pclHouseManager->pclStruct[pclMsg->siVillageUnique]->SetHouseApplyer(pclMsg->siIndex, &pclMsg->clApply, &pclMsg->clContract);
			}

			returnval	= SRVAL_HOUSE_SUCCESSAPPLY;

			
			SI32 id	= pclMsg->usCharID;
			if(pclCM->IsValidID(id) == FALSE)
			{
				return ;
			}
			else
			{
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 18] // 창고 임차 신청 로그쓰기. (성공했을때만.)
					// param1		 == 임대 수수료.
					// param2		 == 임대 기간.
					// pszCharParam1 == 임대 신청 시간.
					TCHAR pszCharParam1[50] = {'\0', };
					StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d : %d"),	
																				pclMsg->clContract.clDate.uiYear,
																				pclMsg->clContract.clDate.uiMonth,
																				pclMsg->clContract.clDate.uiDay,
																				pclMsg->clContract.clDate.uiHour,
																				pclMsg->clContract.clDate.uiMinute );

					
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_RENTAPPLY, 
										0, (cltCharServer*)(pclCM->CR[pclMsg->usCharID]), NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->clContract.siRentFee, pclMsg->clContract.siRentPeriod, 0, 0, 0, 
										pszCharParam1, NULL);
				}
			}
		}
		break;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_HOUSE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}


// 임대 허가를 제출한 것에 대한 응답.
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSERENT(sPacketHeader* pPacket)
{
	sDBResponse_HouseRent* pclMsg = (sDBResponse_HouseRent*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_ITEM_PER_PERSON )		return;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case -1:
		returnval	= SRVAL_HOUSE_FAILNOMOREREALESTATE;		// 부동산 개수를 초과해서 가질 수 없다. 
		break;
	case 1:
		{
			if(pclMsg->siVillageUnique)
			{
				// 시전 임차인으로 등록한다. 
				pclHouseManager->SetRent(pclMsg->siVillageUnique, pclMsg->siIndex, &pclMsg->clPerson, &pclMsg->clRentContract, pclMsg->m_szName);

				// 희망자 명단에서 Person을 삭제한다. 
				pclHouseManager->pclStruct[pclMsg->siVillageUnique]->SetHouseApplyer(pclMsg->siApplyerIndex, &pclMsg->clApplyer.clPerson, &pclMsg->clApplyer.clContract);


				// 쪽지를 보낸다. 
				cltLetterMsg_RentHouse clletter(pclMsg->siVillageUnique, pclMsg->siIndex);
				SI32 id = SendLetterMsg(pclMsg->clPerson.GetPersonID(), (cltLetterHeader*)&clletter);
				if(id)
				{
					// Person이 파악한 부동산 정보를 업데이트 한다. 
					pclCM->CR[id]->pclCI->clRealEstate.Add(REALESTATE_TYPE_HOUSE, pclMsg->siVillageUnique, pclMsg->siIndex, &pclMsg->clRentContract);
					pclCM->CR[id]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);

				}


			}

			returnval	= SRVAL_HOUSE_SUCCESSHOUSERENT; // 시전임대에 성공했다.

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 18] // 시전임대.
				// param1 == 임대료, param2 == 임대기간, param3 == 신청자 상태의 위치. pszParam1 == 임대시간.
				TCHAR pszCharParam1[50] = {'\0', };
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d : %d"),	
																			pclMsg->clRentContract.clDate.uiYear,
																			pclMsg->clRentContract.clDate.uiMonth,
																			pclMsg->clRentContract.clDate.uiDay,
																			pclMsg->clRentContract.clDate.uiHour,
																			pclMsg->clRentContract.clDate.uiMinute  );


				cltServer* pclserver = (cltServer*)pclClient;
				cltCharServer* pclchar = NULL;
				if( pclCM->IsValidID(pclMsg->usCharID) )		pclchar = (cltCharServer*)pclCM->CR[pclMsg->usCharID];
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_RENT, 
												0, pclchar, NULL, pclMsg->clPerson.siPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0,
												pclMsg->clRentContract.siRentFee, pclMsg->clRentContract.siRentPeriod, pclMsg->siIndex, 0, 0, 
												pszCharParam1, NULL);
			}
		}
		break;
	}


	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_HOUSE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 아이템을 인벤에서 창고로 옮기는것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MOVEITEMINVTOSTG(sPacketHeader* pPacket)
{
	sDBResponse_MoveItemInvToStg* pclMsg = (sDBResponse_MoveItemInvToStg*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if( pclMsg->siInvPos < 0 || pclMsg->siInvPos >= MAX_ITEM_PER_PERSON )		return;

	// 창고를 업데이트 한다. 
	pclHouseManager->SetItemStg(pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStgPos, &pclMsg->clStgItem);
	
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		// Person의 인벤토리를 업데이트 한다. 
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siInvPos].Set(&pclMsg->clInvItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		{
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siInvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clInvItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}


		// 창고의 정보를 클라이언트에게 보내준다. 
		{
			cltHouseUnitStgItemInfo clstginfo(pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStgPos, STGITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clStgItem );
			cltMsg clMsg(GAMEMSG_RESPONSE_STGITEMINFO, sizeof(clstginfo), (BYTE*)&clstginfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 창고 보관 로그쓰기.
			// param1 == 창고 슬롯, 
			// param2 == 인벤위치.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_MOVEITEMINVTOSTG, 
								 0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clMoveItem, pclMsg->clInvItem.siItemNum, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								 								 pclMsg->siHouseUnitSlot, pclMsg->siInvPos, pclMsg->clStgItem.siItemNum, 0, 0, NULL, NULL);
		}	
	}
}

// 아이템을 창고에서 인벤으로 옮기는것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MOVEITEMSTGTOINV(sPacketHeader* pPacket)
{
	sDBResponse_MoveItemStgToInv* pclMsg = (sDBResponse_MoveItemStgToInv*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if( pclMsg->siInvPos < 0 || pclMsg->siInvPos >= MAX_ITEM_PER_PERSON )		return;

	// 창고를 업데이트 한다. 
	pclHouseManager->SetItemStg(pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStgPos, &pclMsg->clStgItem);

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		// Person의 인벤토리를 업데이트 한다. 
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siInvPos].Set(&pclMsg->clInvItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		{
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siInvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clInvItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}


		// 창고의 정보를 클라이언트에게 보내준다. 
		{
			cltHouseUnitStgItemInfo clstginfo(pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStgPos, STGITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clStgItem);
			cltMsg clMsg(GAMEMSG_RESPONSE_STGITEMINFO, sizeof(clstginfo), (BYTE*)&clstginfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 시전의 아이템을 소지품으로 이동. 
			// param1 == 창고슬롯, param2 == 인벤토리 저장위치

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_MOVEITEMSTGTOINV, 
								0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clMoveItem, pclMsg->clInvItem.siItemNum, 0, 0, pclMsg->siVillageUnique, 0, 0, 
																pclMsg->siHouseUnitSlot, pclMsg->siInvPos, pclMsg->clStgItem.siItemNum, 0, 0, NULL, NULL);
		}

		//------------------------------------------
		// 사용기간을 확인한다.
		//------------------------------------------
		/*if(CheckItemUseDayOver(&pclMsg->clInvItem))
		{	
		// DB 삭제 요청
		sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),pclMsg->siInvPos
		,&pclMsg->clInvItem,USEITEM_REASON_DATEOVER);
		pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

		// 유저에게 메시지

		}*/

		SI32 itemunique = pclMsg->clInvItem.siUnique ;
		if ( itemunique > 0 )
		{
			if ( pclMsg->clInvItem.IsHaveUseDay() )  //[영진] 남은 사용기간 없음 알림
			{
				SI32 useableDay = 0;
				BOOL rVal = GetItemUseAbleDay(&pclMsg->clInvItem,&useableDay);
				if(rVal == FALSE) return;

				if( useableDay < 0) //사용기간이 지나서 지워준다 그리고 클라이언트에 알림.
				{
					// 고급품은 지워지지 않는다. - by LEEKH
					if( pclMsg->clInvItem.clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
					{
						sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),pclMsg->siInvPos,&pclMsg->clInvItem,USEITEM_REASON_DATEOVER);
						pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

						cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclMsg->clInvItem.siUnique, useableDay);
						cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );		
					}

					return;
				}
			}
		}
	}
}


// 창고의 특정 위치에 있는 아이템 정보를 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETHOUSESTGPOS(sPacketHeader* pPacket)
{
	sDBResponse_GetHouseStgPos* pclMsg = (sDBResponse_GetHouseStgPos*) pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// 창고를 업데이트 한다. 
	pclHouseManager->SetItemStg(pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStgPos, &pclMsg->clItem);


	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		// 창고의 정보를 클라이언트에게 보내준다. 
		cltHouseUnitStgItemInfo clstginfo(pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStgPos, STGITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem);
		cltMsg clMsg(GAMEMSG_RESPONSE_STGITEMINFO, sizeof(clstginfo), (BYTE*)&clstginfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITSTABLECREATE(sPacketHeader* pPacket)
{
	sDBResponse_HouseUnitStableCreate* pclMsg = (sDBResponse_HouseUnitStableCreate*) pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;

	if( pclMsg->siStableIndex < 0 || pclMsg->siStableIndex >= MAX_HOUSEUNIT_HORSENUMBER )	return;

	//------------------------------------------------------------------------------------------
	// 시전에 마구간을 생성.
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	if ( pclStg == NULL )
	{
		return;
	}
	pclStg->bCreatedHouseUnitStable[ pclMsg->siStableIndex ] = true;
	//------------------------------------------------------------------------------------------

	// 서버 화면 업데이트( 몇개 사용됐는지. )
	// pclItemManager->AddUseNum( ITEMUNIQUE(24025), HOUSEUNIT_CREATESTABLE_USEITEMNUM );

	if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		/*
		// 세금 내고 남은돈 업데이트
		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siPersonLeftMoney );

		// 시전 보유자금 업데이트 
		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_HOUSE );
		if( pStrInfo ) 
		{
		pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clLeftHouseUnitMoney );
		}
		else
		{
		// 시전이 없는 마을이 나중에 생기면... 이런 요상한 경우가 생길수 있지...
		pclMsg->clLeftHouseUnitMoney.Set( (__int64)(0) );
		pclLog->FilePrint( TEXT("HouseUnitError.txt"), TEXT("pStrInfo == NULL, VillageUnique = %d"), pclMsg->siVillageUnique );
		}
		*/

		//pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clLeftItem );
		GMONEY siCurrentMoney = pclClient->pclCM->CR[id]->pclCI->clIP.GetMoney();
		pclCM->CR[id]->pclCI->clIP.SetMoney( siCurrentMoney - pclMsg->siHouseUnitStableCreateFee );

		cltGameMsgResponse_HouseUnitStableCreate clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siStableIndex, pclMsg->siHouseUnitStableCreateFee ); 
		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITSTABLECREATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 마구간을 생성.
			// param1 == 창고 슬롯 번호, 
			// param2 == 마구간 번호.  
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSEHORSE, LOGCOMMAND_INDEX_HOUSEHORSE_CREATE_HORSE_STABLE,
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->siHouseUnitStableCreateFee, pclCM->CR[id]->pclCI->clIP.GetMoney(), NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siHouseUnitSlot, pclMsg->siStableIndex, 0, 0, 0, NULL, NULL);
		}

	}


	return;
}

// 시전에 말을 넣는다.
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEIN(sPacketHeader* pPacket)
{
	sDBResponse_HouseUnitHorseIn* pclMsg = (sDBResponse_HouseUnitHorseIn*) pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if( pclMsg->siHouseUnitStgHorseIndex < 0 || pclMsg->siHouseUnitStgHorseIndex >= MAX_HOUSEUNIT_HORSENUMBER )	return;

	SI32 id = pclMsg->siCharID;

	//------------------------------------------------------------------------------------------
	// 시전에 말을 넣음. ( 말이 있는 위치 값에 말을 넣어줌 )
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	if ( pclStg == NULL )
	{
		return;
	}
	pclStg->clHorse[ pclMsg->siHouseUnitStgHorseIndex ].Set( &pclMsg->clHouseUnitStgHorse );
	//------------------------------------------------------------------------------------------

	// 시전에서 말을 넣었는 것을 알려줌
	if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		// 사람이 존재하므로, 시전에 말과 바꿀수도 있으므로, 말에 대해 업데이트 시켜줌		
		pclCM->CR[id]->UnrideHorse();
		pclCM->CR[id]->pclCI->clHorseInfo.SetHorse(pclMsg->siPersonHorseIndex, &pclMsg->clPersonHorse);

		cltGameMsgResponse_HouseUnitHorseIn clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siPersonHorseIndex, &pclMsg->clPersonHorse, pclMsg->siHouseUnitStgHorseIndex, &pclMsg->clHouseUnitStgHorse );
		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITHORSEIN, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// 주위 사람들에게 말 변경 업데이트 시켜줌
		cltCharServer *pclChar = (cltCharServer*)pclCM->CR[id];		
		pclChar->SendNeighbourMyHorseStatus( pclChar->pclCI->clHorseInfo.siCurrentShowHorse);

		// 말 정보 전체를 업데이트 시킨다. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_HORSE, true, 0);



		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 29] // 마구간에서 말 넣은 기록.
			// param1 == 마구간 종류, 
			// param2 == 창고의 말들 중에서 빼줘야 하는 Index.  
			// param3 == 내가 데리고 다니는 말의 배열 Index.
			// pszCharParam1 == Unique, Name, Sex, PregnentPeriod, Life, TicketNum
			// pszCharParam2 == Unique, Name, Sex, PregnentPeriod, Life, TicketNum
			TCHAR str[2][50];
			ZeroMemory(str, sizeof(str));
			StringCchPrintf(str[0], sizeof(str[0]), TEXT("OUT:  %d, %s, %d, %d, %d, %d"),	
															pclMsg->clPersonHorse.siHorseUnique, 
															pclMsg->clPersonHorse.szName, 
															pclMsg->clPersonHorse.siSex, 
															pclMsg->clPersonHorse.siPregnentPeriod, 
															pclMsg->clPersonHorse.siLife, 
															pclMsg->clPersonHorse.siStatusTicketNum );

			StringCchPrintf(str[1], sizeof(str[1]), TEXT("IN:  %d, %s, %d, %d, %d, %d"),	
															pclMsg->clHouseUnitStgHorse.siHorseUnique, 
															pclMsg->clHouseUnitStgHorse.szName, 
															pclMsg->clHouseUnitStgHorse.siSex, 
															pclMsg->clHouseUnitStgHorse.siPregnentPeriod, 
															pclMsg->clHouseUnitStgHorse.siLife, 
															pclMsg->clHouseUnitStgHorse.siStatusTicketNum);

			
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSEHORSE, LOGCOMMAND_INDEX_HOUSEHORSE_HORSEINHOUSE,
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siHouseUnitSlot, pclMsg->siHouseUnitStgHorseIndex, pclMsg->siPersonHorseIndex, 0, 0, str[0], str[1]);
		}
	}

	return;
}

// 시전에서 말을 뺀다.
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEOUT(sPacketHeader* pPacket)
{
	sDBResponse_HouseUnitHorseOut* pclMsg = (sDBResponse_HouseUnitHorseOut*) pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if( pclMsg->siHouseUnitStgHorseIndex < 0 || pclMsg->siHouseUnitStgHorseIndex >= MAX_HOUSEUNIT_HORSENUMBER )	return;

	//------------------------------------------------------------------------------------------
	// 시전에서 말을 빼냄 ( 말이 있는 위치 값을 초기화 시킴 )
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	if ( pclStg == NULL )
	{
		return;
	}	
	pclStg->clHorse[ pclMsg->siHouseUnitStgHorseIndex ].Set( &pclMsg->clHouseUnitHorse );
	//------------------------------------------------------------------------------------------


	SI32 id = pclMsg->siCharID;

	// 시전에서 말을 빼는 것을 알려줌
	if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		// 사람이 존재하므로, 사람에게 말 추가
		pclCM->CR[id]->UnrideHorse();		
		//pclCM->CR[id]->pclCI->clHorseInfo.clHorse.Set( &pclMsg->clPersonHorse );
		pclCM->CR[id]->pclCI->clHorseInfo.SetHorse( pclMsg->siPersonHorseIndex, &pclMsg->clPersonHorse );


		// 시전에도 업데이트 시켜줌 ( 말을 예전에는 없애버렸지만, 가진말과 바꾸는것도 가능하게 되었음으로. )
		cltGameMsgResponse_HouseUnitHorseOut clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siHouseUnitStgHorseIndex, &pclMsg->clHouseUnitHorse, pclMsg->siPersonHorseIndex, &pclMsg->clPersonHorse );
		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITHORSEOUT, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 마구간에서 말 꺼낸 기록.
			// param1 == 마구간 종류, 
			// param2 == 창고의 말들 중에서 빼줘야 하는 Index.  
			// param3 == 내가 데리고 다니는 말의 배열 Index.
			// pszParam1 == Person 말정보, 
			// pszParam2 == HouseUnit 말정보
			TCHAR str[2][50];
			ZeroMemory(str, sizeof(str));

			StringCchPrintf(str[0], sizeof(str[0]), TEXT("OUT:  %d, %s, %d, %d, %d, %d"),	
															pclMsg->clPersonHorse.siHorseUnique, 
															pclMsg->clPersonHorse.szName, 
															pclMsg->clPersonHorse.siSex, 
															pclMsg->clPersonHorse.siPregnentPeriod, 
															pclMsg->clPersonHorse.siLife, 
															pclMsg->clPersonHorse.siStatusTicketNum );

			StringCchPrintf(str[1], sizeof(str[1]), TEXT("IN:  %d, %s, %d, %d, %d, %d"),	
															pclMsg->clHouseUnitHorse.siHorseUnique, 
															pclMsg->clHouseUnitHorse.szName, 
															pclMsg->clHouseUnitHorse.siSex, 
															pclMsg->clHouseUnitHorse.siPregnentPeriod, 
															pclMsg->clHouseUnitHorse.siLife, 
															pclMsg->clHouseUnitHorse.siStatusTicketNum);
		

			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSEHORSE, LOGCOMMAND_INDEX_HOUSEHORSE_HORSEOUTHOUSE,
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siHouseUnitSlot, pclMsg->siHouseUnitStgHorseIndex, pclMsg->siPersonHorseIndex, 0, 0, str[0], str[1]);
		}
		
		// 말 정보 전체를 업데이트 시킨다. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_HORSE, true, 0);

		// 주위 사람들에게 말 변경 업데이트 시켜줌
		cltCharServer *pclChar = (cltCharServer*)pclCM->CR[id];
		pclChar->SendNeighbourMyHorseStatus(pclChar->pclCI->clHorseInfo.siCurrentShowHorse);

	}

	return;
}


void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEINFOSET(sPacketHeader* pPacket)
{
	sDBResponse_HouseUnitHorseInfoSet* pclMsg = (sDBResponse_HouseUnitHorseInfoSet*) pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if( pclMsg->siHouseUnitHorseIndex < 0 || pclMsg->siHouseUnitHorseIndex >= MAX_HOUSEUNIT_HORSENUMBER )	return;

	//------------------------------------------------------------------------------------------
	// 시전에 들어 있는말의 정보를 업데이트
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	if ( pclStg == NULL )
	{
		return;
	}
	pclStg->clHorse[ pclMsg->siHouseUnitHorseIndex ].Set( &pclMsg->clHouseUnitHorseInfo );
	//------------------------------------------------------------------------------------------

	SI32 id = pclMsg->siCharID;
	cltCharServer *pclChar = pclCM->GetCharServer(id);
	// 시전에서 말 정보가 변경되었음을 알려줌
	if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		// 유저에게 통보
		cltGameMsgResponse_HouseUnitHorseInfoSet clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siHouseUnitHorseIndex, &pclMsg->clHouseUnitHorseInfo );
		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITHORSEINFOSET, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// 말 정보 전체를 업데이트 시킨다. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_HORSE, true, 0);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 15] // 시전에 보관된 말의 상태가 변경된 내역이 기록
			// param1	 == 마구간 종류, 
			// param2	 == 내가 데리고 다니는 말의 배열 Index. 
			// pszParam1 == Unique, Name, Sex, PregnentPeriod, Life, TicketNum

			TCHAR str[50] = {'\0', };
			StringCchPrintf(str, sizeof(str), TEXT("%d, %s, %d, %d, %d, %d"),	
													pclMsg->clHouseUnitHorseInfo.siHorseUnique, 
													pclMsg->clHouseUnitHorseInfo.szName, 
													pclMsg->clHouseUnitHorseInfo.siSex, 
													pclMsg->clHouseUnitHorseInfo.siPregnentPeriod, 
													pclMsg->clHouseUnitHorseInfo.siLife, 
													pclMsg->clHouseUnitHorseInfo.siStatusTicketNum  );

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSEHORSE, LOGCOMMAND_INDEX_HOUSEHORSE_SETHOUSEHORSEINFO,
								 0, pclChar, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								 pclMsg->siHouseUnitSlot, pclMsg->siHouseUnitHorseIndex, 0, 0, 0, str, NULL);
		}

	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEEATFOOD(sPacketHeader* pPacket)
{
	sDBResponse_HouseUnitHorseEatFood* pclMsg = (sDBResponse_HouseUnitHorseEatFood*) pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	//------------------------------------------------------------------------------------------
	// 시전에 수정된 아이템 정보를 업데이트
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	if ( pclStg == NULL )
	{
		return;
	}

	if( pclMsg->siHouseUnitItemIndex < 0 || pclMsg->siHouseUnitItemIndex >= MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER )		return;

	pclItemManager->AddUseNum( pclMsg->siHouseUnitHorseEatItemUnique, pclMsg->siHouseUnitHorseEatItemNum );

	// 새로운 통계 - 아이템 사용 통계를 낸다.
	//NStatistics_Add("Item", pclMsg->siHouseUnitHorseEatItemUnique, "UseNum", pclMsg->siHouseUnitHorseEatItemNum);

	pclStg->clItem[ pclMsg->siHouseUnitItemIndex ].Set( &pclMsg->clHouseUnitItem );
	//------------------------------------------------------------------------------------------

	SI32 id = pclMsg->siCharID;

	// 시전에서 아이템 정보가 변경되었음을 알려줌
	if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		cltGameMsgResponse_HouseUnitHorseEatFood clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siHouseUnitItemIndex, &pclMsg->clHouseUnitItem );
		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITHORSEEATFOOD, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	return;
}


// 시전에 고물상 생성
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE(sPacketHeader* pPacket)
{
	sDBResponse_HouseUnitUsedGoodsBuildingCreate* pclMsg = (sDBResponse_HouseUnitUsedGoodsBuildingCreate*) pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;

	if ( id < 1 )
	{
		return;
	}

	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )			return;

	//------------------------------------------------------------------------------------------
	// 시전에 고물상을 생성.
	cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	if ( pclStg == NULL )
	{
		return;
	}
	pclStg->bCreatedHouseUnitUsedGoodsBuilding = true;
	//------------------------------------------------------------------------------------------

	// 서버 화면 업데이트( 몇개 사용됐는지. )
	pclItemManager->AddUseNum( ITEMUNIQUE(24025), HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM );

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", ITEMUNIQUE(24025), "UseNum", HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM);


	if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clLeftItem );

		cltGameMsgResponse_HouseUnitUsedGoodsBuildingCreate clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siItemPos, &pclMsg->clLeftItem );
		cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 10. 10] // 시전에서 고물상 생성.
		//	// param1 == 부동산의 슬롯. , 
		//	// param2 == 아이템 위치.
		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_CREATE,
		//									0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clLeftItem, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		//									pclMsg->siHouseUnitSlot, pclMsg->siItemPos, 0, 0, 0, NULL, NULL);
		//}
	}

}

// 고물에서 아이템 뽑아냄
void cltServer::DoMsg_DBMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS(sPacketHeader* pPacket)
{
	//sDBResponse_HouseUnitMakeItemFromUsedGoods* pclMsg = (sDBResponse_HouseUnitMakeItemFromUsedGoods*) pPacket;

	//if ( pclMsg->siResult != 1 )
	//{
	//	pclLog->FilePrint( TEXT("Config\\HouseUnit.txt"), TEXT("DoMsg_DBMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS pclMsg->siResult = %d"), pclMsg->siResult );
	//	return;
	//}

	//SI32 id = pclMsg->siCharID;

	//if ( id < 1 )
	//{
	//	return;
	//}

	////------------------------------------------------------------------------------------------
	//// 시전에 고물상을 생성.
	//cltHouseUnitStorageInfo* pclStg = pclHouseManager->GetStgPointer( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot );
	//if ( pclStg == NULL )
	//{
	//	return;
	//}

	//// pclMsg->siFromItemUnique		// 없어진 아이템의 ItemUnique
	//pclStg->clItem[ pclMsg->siFromItemPos ].Init();

	//for( SI16 i = 0; i < MAX_MATERIAL_NUMBER; ++i )
	//{
	//	if ( pclMsg->siToHouseUnitStgItemPos[ i ] >= 0 )
	//	{
	//		pclStg->clItem[ pclMsg->siToHouseUnitStgItemPos[ i ] ].Set( &pclMsg->clMakeLeftItem[ i ] );
	//	}
	//}
	////------------------------------------------------------------------------------------------

	//if( pclCM->IsValidID( id ) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	//{
	//	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoods clinfo( pclMsg->siVillageUnique, pclMsg->siHouseUnitSlot, pclMsg->siFromItemPos, pclMsg->siToHouseUnitStgItemPos, pclMsg->clMakeLeftItem );
	//	cltMsg clMsg(GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS, sizeof(clinfo), (BYTE*)&clinfo);
	//	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsNotify clinfo1( pclCM->CR[id]->GetCharUnique(), pclMsg->siFromItemUnique, pclMsg->siMakePercent );
	//	cltMsg clMsg1(GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSNOTIFY, sizeof(clinfo1), (BYTE*)&clinfo1);
	//	((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg( &clMsg1, true);

	//	// 고물처리술 경험치를 부여한다. 
	//	cltItemPriceUnit* pclprice	= pclItemManager->GetCurrentPrice(pclMsg->siFromItemUnique);
	//	if(pclprice)
	//	{	
	//		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
	//		//--------------------------------------
	//		// 경험치를 높여준다. 
	//		//--------------------------------------
	//		SI32 exp	= pclItemManager->GetMakeExpFromItemPrice(pclprice->siPrice , pclchar->pclCI->clIP.GetLevel()) / 2; 
	//		if(exp > 0)
	//		{
	//			pclchar->IncreaseExp(exp, GETEXP_REASON_DISMANTLE);
	//		}

	//		//-------------------------------------
	//		// 제조 경험치를 높여준다. 
	//		//-------------------------------------
	//		SI32 skillunique	= SKILL_ALCHEMY; 
	//		SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

	//		SI32 skillexp	= pclItemManager->GetMakeSkillExpFromItemPrice(pclprice->siPrice, skilllevel) / 2; 

	//		// 제조 보너스 타임이면, 
	//		if(bBonus2TimeSwitch)
	//			skillexp *= 2;
	//		else if(pclchar->pclCI->clBI.bMakeItemService)
	//		{
	//			skillexp = (SI32)(skillexp * 1.2 );
	//		}

	//		pclchar->SetSkillUpdate(skillunique , skillexp);
	//	}
	//}

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
		// [로그추가 : 황진성 2007. 10. 10] // 고물 하는 물품 MakePercent = 59% (회수율)
	//// param1 == 
	//cltServer* pclserver = (cltServer*)pclClient;
	//pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_SALVAGEFROM,
	//								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
	//								0, 0, 0, 0, 0, NULL, NULL);
	//}
}