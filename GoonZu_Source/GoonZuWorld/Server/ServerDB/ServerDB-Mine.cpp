// server&db-land
#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"
#include "..\Common\Letter\Letter.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Mine.h"

#include "Msg\MsgType-Mine.h"
#include "MsgRval-Define.h"

#include "PersonInfo.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"

// 단위 광산 정보 얻어 오기 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYMINEUNIT(sPacketHeader* pPacket)
{
	sDBResponse_ManyMineUnit* pclMsg = (sDBResponse_ManyMineUnit*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 0 ) return;

	SI32 villageunique = pclMsg->siVillageUnique;
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER )	return;
	if( pclMineManager->pclStruct[villageunique] == NULL )			return;

	cltMine *pclMine;
	for( int i = 0; i < MAX_MINEUNIT_NUMBER; i++ )
	{
		pclMine = (cltMine *)pclMineManager->pclStruct[ villageunique ];
		pclMine->SetMineUnit( i, &pclMsg->clMineUnit[i] );
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyMineUnit"));
}

// 계약 설정 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_CHANGECONTRACT(sPacketHeader* pPacket)
{

	sDBResponse_MineChangeContract* pclMsg = (sDBResponse_MineChangeContract*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = 0;
	SI32 id = pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) {

		// 해당하는 광산의 계약 조건을 변경한다. 
		cltMine *pclMine = (cltMine *)pclMineManager->pclStruct[pclMsg->siVillageUnique];
		pclMine->SetRentContract( &pclMsg->clContract );
	
		if(pclCM->IsValidID(id)== false)return ;

		// 클라이언트에 변경된 계약조건을 보낸다. 
		if(pclCM->CR[id]->IsValidConnection())	
		{
			cltStrInfo* pclinfo = pclMineManager->GetStrInfo(pclMsg->siVillageUnique);
			if(pclinfo == NULL)return ;

			cltMsg clMsg(GAMEMSG_RESPONSE_STRINFO,  pclinfo->GetTotalSize(), (BYTE*)pclinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			
		}
		returnval = SRVAL_MINE_SUCCESS_SETCONTRACT;

	}

	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_MINE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 임차 신청 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_RENTAPPLY(sPacketHeader* pPacket)
{

	sDBResponse_MineRentApply* pclMsg = (sDBResponse_MineRentApply*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = SRVAL_MINE_FAIL_APPLY;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	switch( pclMsg->siResult ) {
	case 1:
		{
			
			// 신청자를 등록한다.
			if(pclMsg->siVillageUnique)
			{
				cltMine *pclMine = (cltMine *)pclMineManager->pclStruct[pclMsg->siVillageUnique];
				pclMine->AddApplyer( &pclMsg->clApplyer, &pclMsg->clContract );
			}

			returnval = SRVAL_MINE_SUCCESS_APPLY;

			// 임차 신청에 성공 할때만 로그를 남긴다.
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 18] // 광산 임차 신청 로그쓰기. 
				// param1		 == 세금, 
				// param2		 == 기간, 
				// pszCharParam1 == 등록시간 (게임내 시간)
				TCHAR pszCharParam1[50] = {'\0', };
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
					pclMsg->clContract.clDate.uiYear,
					pclMsg->clContract.clDate.uiMonth,
					pclMsg->clContract.clDate.uiDay,
					pclMsg->clContract.clDate.uiHour,
					pclMsg->clContract.clDate.uiMinute );

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINE, LOGCOMMAND_INDEX_MINE_RENT_APPLY, 
					0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					pclMsg->clContract.siRentFee, pclMsg->clContract.siRentPeriod, 0, 0, 0,
					pszCharParam1, NULL);
			}
		}
		break;
	case 2:
		{
			returnval = SRVAL_MINE_FAIL_APPLY;
		}
		break;
	case 3:
		{
			returnval = SRVAL_MINE_FAIL_APPLY_ALREADYAPPLY;
		}
		break;

	case 4:
		{
			returnval = SRVAL_MINE_FAIL_APPLY;
		}
		break;
	}
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_MINE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// 임차 신청 취소 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_CANCELRENTAPPLY(sPacketHeader* pPacket)
{

	sDBResponse_MineCancelRentApply* pclMsg = (sDBResponse_MineCancelRentApply *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = SRVAL_MINE_FAIL_CANCELAPPLY;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if( pclMsg->siResult == 1 ) {

		returnval = SRVAL_MINE_SUCCESS_CANCELAPPLY;
		
		// 신청자를 등록한다.
		if(pclMsg->siVillageUnique) {

			cltMine *pclMine;
			pclMine = (cltMine *)pclMineManager->pclStruct[pclMsg->siVillageUnique];
			pclMine->DeleteApplyer( pclMsg->siPersonID );
		}

		// 신청취소에 성공할때만 로그를 남긴다.
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 9] // 임차 신청 취소 로그쓰기.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINE, LOGCOMMAND_INDEX_MINE_CANCEL_RENT_APPLY, 
				0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				0, 0, 0, 0, 0, NULL, NULL);
		}
	}
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_MINE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}	
}


// 임차 희망자 명단 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_APPLYERLIST(sPacketHeader* pPacket)
{
	sDBResponse_MineApplyerList* pclMsg = (sDBResponse_MineApplyerList*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if(pclMsg->siResult == 0) return ;

	cltMine *pclMine = NULL;
	for( int i = 0; i < MAX_APPLY_PER_MINE; i++ )
	{
		if(pclMineManager->pclStruct[pclMsg->siVillageUnique])
		{
			pclMine = (cltMine *)pclMineManager->pclStruct[pclMsg->siVillageUnique];
			pclMine->SetRentApplyer( i, &pclMsg->clApplyer[i].clPerson, &pclMsg->clApplyer[i].clContract );
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_MineApplyerList"));
}

// 임차 희망자 명단 삭제 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_DELAPPLYER(sPacketHeader* pPacket)
{
	sDBResponse_MineDelApplyer* pclMsg = (sDBResponse_MineDelApplyer *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siDelSlotNum < 0 || pclMsg->siDelSlotNum > MAX_APPLY_PER_MINE )				return;

	SI32 returnval = 0;

	if( pclMsg->siResult != 1 ) return;
	
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	// 성공인 경우에는 게임서버의 임차 희망자 명단을 삭제한다. 
	cltMine *pclMine	 = (cltMine *)pclMineManager->pclStruct[pclMsg->siVillageUnique];
	
	// [추가 : 황진성 2007. 11. 2 임차 희망자 명단 중 삭제할 유저의 personid.]
	SI32	siDelPerSonID = 0;

	if(pclMsg->siDelSlotNum >=0 && pclMsg->siDelSlotNum < MAX_APPLY_PER_MINE)
	{
		siDelPerSonID = pclMine->clApplyer[pclMsg->siDelSlotNum].clPerson.GetPersonID();
	}

	if(pclCM->IsValidID(pclMsg->usCharID) == TRUE)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 4] // 광산임대 희망자 제거.
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINE, LOGCOMMAND_INDEX_MINE_RENT_APPLYER_DELETE, 
											0, (cltCharServer*)pclCM->CR[id], NULL, siDelPerSonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											0, 0, 0, 0, 0, NULL, NULL);
		}
	}

	// [추가 : 황진성 2007. 11. 2 임차 희망자 명단을 모조리 삭제하는 것을 수정.]
	pclMine->DeleteApplyer(siDelPerSonID);

	// 클라이언트로 임차 희망자 명단을 모두 삭제하라는 메시지를 보낸다. 

	if(pclCM->IsValidID(id) == TRUE)
	{
		cltMsg clMsg(GAMEMSG_RESPONSE_MINE_DELAPPLYER, sizeof(SI32), (BYTE*)&pclMsg->siDelSlotNum);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// 임대 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_RENT(sPacketHeader* pPacket)
{
	sDBResponse_MineRent* pclMsg = (sDBResponse_MineRent*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = SRVAL_MINE_FAIL_RENT;

	cltMine *pclMine = (cltMine*)pclMineManager->pclStruct[ pclMsg->siVillageUnique ];
	if( pclMine == NULL ) return;

	// 희망자 명단에서 Person을 삭제한다. 
	pclMine->DeleteApplyer( pclMsg->clPerson.siPersonID );

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case -1:
		returnval	= SRVAL_MINE_FAIL_NOTSAMECONTRACT;		// 계약 조건이 다르다 
		break;

	case -2:
		returnval	= SRVAL_MINE_FAIL_NOMOREREALESTATE;		// 부동산 개수를 초과해서 가질 수 없다. 
		break;

	case 1:
		{
			returnval	= SRVAL_MINE_SUCCESS_RENT;				// 광산임대에 성공했다.
				
			cltMine *pclMine = (cltMine*)pclMineManager->pclStruct[ pclMsg->siVillageUnique ];
			if( pclMine == NULL ) return;

			// 시전 임차인으로 등록한다. 
			pclMine->SetRent( pclMsg->siIndex, &pclMsg->clPerson, &pclMsg->clRentContract );
			

			// 쪽지를 보낸다. 
			cltLetterMsg_RentMine clletter(pclMsg->siVillageUnique, pclMsg->siIndex);
			SI32 id = SendLetterMsg(pclMsg->clPerson.GetPersonID(), (cltLetterHeader*)&clletter);
			if(pclCM->IsValidID(id))
			{
				// Person이 파악한 부동산 정보를 업데이트 한다. 
				pclCM->CR[id]->pclCI->clRealEstate.Add(REALESTATE_TYPE_MINE, pclMsg->siVillageUnique, pclMsg->siIndex, &pclMsg->clRentContract);
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);

			}	
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 16] // 광산 임대 로그쓰기. 
				// param1		 == 광산 번호. 
				// param2		 == 임대 수수료.
				// param3		 == 임대기간.
				// pszCharParam1 == 임대성공했을때의 게임 시간 
				TCHAR pszCharParam1[50] = {'\0', };
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
																			pclMsg->clRentContract.clDate.uiYear,
																			pclMsg->clRentContract.clDate.uiMonth,
																			pclMsg->clRentContract.clDate.uiDay,
																			pclMsg->clRentContract.clDate.uiHour,
																			pclMsg->clRentContract.clDate.uiMinute );

				cltCharServer* pclchar = NULL;
				if(pclCM->IsValidID(pclMsg->usCharID))		pclchar = (cltCharServer*)(pclCM->CR[pclMsg->usCharID]);
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINE, LOGCOMMAND_INDEX_MINE_RENT, 
									 0, pclchar, NULL, pclMsg->clPerson.GetPersonID(), 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									 pclMsg->siIndex, pclMsg->clRentContract.siRentFee, pclMsg->clRentContract.siRentPeriod, 0, 0, 
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
		SendServerResponseMsg( RANKTYPE_MINE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
	}

}

// 임대료 징수 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_PAYRENTFEE(sPacketHeader* pPacket)
{

	sDBResponse_MinePayRentFee* pclMsg = (sDBResponse_MinePayRentFee*)pPacket;

	SI32 id = 0;
	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
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

			// 연체 횟수가 3회를 초과하는 경우 광산을 초기화한다. 
			if(pclMsg->siDelayNum > 3)
			{
				// DB매니저에게 광산을 회수할 것을 요청한다. 
				sDBRequest_MineRecall clMsg(pclMsg->siPersonID, MINERECALL_REASON_FEEDELAY, pclMsg->siVillageUnique, pclMsg->siMineIndex);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
			else
			{
				cltLetterMsg_PayMineRentFee clletter(pclMsg->siVillageUnique, pclMsg->siMineIndex, pclMsg->siRentFee, pclMsg->siDelayNum, pclMsg->siDelayMoney);
				// 쪽지를 DB나 클라이언트에게 보낸다. 
				SendLetterMsg(pclMsg->siPersonID, (cltLetterHeader*)&clletter);
			}
			

			if(pclCM->IsValidID(id))
			{
				// 지불자의 은행잔고를 업데이트 한다. 
				pclCM->CR[id]->pclCI->clBank.SetMoney( pclMsg->clBankMoney.itMoney );
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);
			}

			// 광산의 보유 자금을 업데이트 한다. 
			cltMoney clmoney;
			clmoney.Set( &pclMsg->clMineMoney );
			pclMineManager->SetMoney(pclMsg->siVillageUnique, &clmoney);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 16] // 광산임대 수수료 지불.
				// param1 == 단위 광산의 인덱스, 
				// param3 == 연체 횟수, 
				// param4 == 연체금 합계.
				// param5 == 광산 보유자금
				cltServer * pclserver = (cltServer*)pclClient;
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINE, LOGCOMMAND_INDEX_MINE_RENT_RENT_FREE_PAY, 
					0, NULL, NULL, pclMsg->siPersonID, pclMsg->siRentFee, pclMsg->clBankMoney.itMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					pclMsg->siMineIndex, 0, pclMsg->siDelayNum, pclMsg->siDelayMoney, pclMsg->clMineMoney.itMoney, NULL, NULL);
			}
		}
		break;

	case -1:
		{
			// 연체 횟수가 3회를 초과하는 경우 시전을 초기화한다. 
			if( pclMsg->siDelayNum > 3)
			{
				// DB매니저에게 광산을 회수할 것을 요청한다. 
				sDBRequest_MineRecall clMsg(pclMsg->siPersonID, MINERECALL_REASON_FEEDELAY, pclMsg->siVillageUnique, pclMsg->siMineIndex);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
		break;

	}
}

// 광산  회수 
void cltServer::DoMsg_DBMSG_RESPONSE_MINE_RECALL(sPacketHeader* pPacket)
{

	sDBResponse_MineRecall* pclMsg = (sDBResponse_MineRecall*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siMineIndex < 0 || pclMsg->siMineIndex >= MAX_MINEUNIT_NUMBER )				return;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 personid		= pclMsg->siPersonID;
	SI32 villageunique	= pclMsg->siVillageUnique;
	SI32 landindex		= pclMsg->siMineIndex;

	cltMine *pclMine = (cltMine*)pclMineManager->pclStruct[ villageunique ];
	if( pclMine == NULL ) return;

	pclMine->clUnit[ landindex ].Init();


	// 쪽지를 보낸다. 
	cltLetterMsg_MineRecall clletter(pclMsg->siRecallReason, pclMsg->siVillageUnique, pclMsg->siMineIndex);
	SI32 letterid = SendLetterMsg(personid, (cltLetterHeader*)&clletter);
	if(pclCM->IsValidID(letterid))
	{
		pclCM->CR[letterid]->pclCI->clRealEstate.Delete(REALESTATE_TYPE_MINE, pclMsg->siVillageUnique, pclMsg->siMineIndex);
		pclCM->CR[letterid]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 9. 3] // 광산 임대반환 로그쓰기. 
		// param1 ==  광산번호.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINE, LOGCOMMAND_INDEX_MINE_RECALL, 
							pclMsg->siRecallReason, NULL, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							pclMsg->siMineIndex, 0, 0, 0, 0, NULL, NULL);
	}
}
