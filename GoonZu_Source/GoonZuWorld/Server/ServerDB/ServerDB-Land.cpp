// server&db-land
#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"
#include "..\Common\Letter\Letter.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Land.h"

#include "Msg\MsgType-Land.h"
#include "MsgRval-Define.h"

#include "PersonInfo.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"

// 단위 농장 정보 얻어 오기 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYLANDUNIT(sPacketHeader* pPacket)
{
	sDBResponse_ManyLandUnit* pclMsg = (sDBResponse_ManyLandUnit*)pPacket;

    if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siResult == 0 ) return;


	SI32 villageunique = pclMsg->siVillageUnique;
	if( pclLandManager->pclStruct[villageunique] == NULL ) return ;



	cltLand *pclLand;
	for( int i = 0; i < MAX_LANDUNIT_NUMBER; i++ )
	{
		pclLand = (cltLand *)pclLandManager->pclStruct[ villageunique ];
		pclLand->SetLandUnit( i, &pclMsg->clLandUnit[i] );
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyLandUnit"));
}

// 계약 설정 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_CHANGECONTRACT(sPacketHeader* pPacket)
{
	sDBResponse_LandChangeContract* pclMsg = (sDBResponse_LandChangeContract*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = 0;
	SI32 id = pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) {

		// 해당하는 농장의 계약 조건을 변경한다. 
		cltLand *pclLand = (cltLand *)pclLandManager->pclStruct[pclMsg->siVillageUnique];
		pclLand->SetRentContract( &pclMsg->clContract );
	
		if(pclCM->IsValidID(id)== false)return ;

		// 클라이언트에 변경된 계약조건을 보낸다. 
		if(pclCM->CR[id]->IsValidConnection())	
		{
			cltStrInfo* pclinfo = pclLandManager->GetStrInfo(pclMsg->siVillageUnique);
			if(pclinfo == NULL)return ;

			cltMsg clMsg(GAMEMSG_RESPONSE_STRINFO,  pclinfo->GetTotalSize(), (BYTE*)pclinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			
		}
		returnval = SRVAL_LAND_SUCCESS_SETCONTRACT;

	}

	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_LAND, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 임차 신청 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_RENTAPPLY(sPacketHeader* pPacket)
{
	sDBResponse_LandRentApply* pclMsg = (sDBResponse_LandRentApply*)pPacket;
    
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = SRVAL_LAND_FAIL_APPLY;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	switch( pclMsg->siResult ) {
	case 1:
		{
			
			// 신청자를 등록한다.
			if(pclMsg->siVillageUnique)
			{
				cltLand *pclLand = (cltLand *)pclLandManager->pclStruct[pclMsg->siVillageUnique];
				pclLand->AddApplyer( &pclMsg->clApplyer, &pclMsg->clContract );
			}

			returnval = SRVAL_LAND_SUCCESS_APPLY;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 18] // 임차 신청 로그쓰기. (성공했을때만.)
				// param1	 == 인덱스 
				// param2	 == 임대 수수료
				// param3	 == 임대 기간.
				// pszParam1 == 임대관련정보. 
				TCHAR pszCharParam1[50] = {'\0', };
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
					pclMsg->clContract.clDate.uiYear,
					pclMsg->clContract.clDate.uiMonth,
					pclMsg->clContract.clDate.uiDay,
					pclMsg->clContract.clDate.uiHour,
					pclMsg->clContract.clDate.uiMinute );


				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FARM, LOGCOMMAND_INDEX_FARM_RENTAPPLY, 
					0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					pclMsg->siIndex, pclMsg->clContract.siRentFee, pclMsg->clContract.siRentPeriod, 0, 0,
					pszCharParam1, NULL);
			}
		}
		break;
	case 2:
		{
			returnval = SRVAL_LAND_FAIL_APPLY;
		}
		break;
	case 3:
		{
			returnval = SRVAL_LAND_FAIL_APPLY_ALREADYAPPLY;
		}
		break;

	case 4:
		{
			returnval = SRVAL_LAND_FAIL_APPLY;
		}
		break;
	}
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_LAND, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 임차 신청 취소 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_CANCELRENTAPPLY(sPacketHeader* pPacket)
{
	sDBResponse_LandCancelRentApply* pclMsg = (sDBResponse_LandCancelRentApply *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	SI32 returnval = SRVAL_LAND_FAIL_CANCELAPPLY;

	if( pclMsg->siResult == 1 ) {

		returnval = SRVAL_LAND_SUCCESS_CANCELAPPLY;
		
		// 신청자를 등록한다.
		if(pclMsg->siVillageUnique) {

			cltLand *pclLand;
			pclLand = (cltLand *)pclLandManager->pclStruct[pclMsg->siVillageUnique];
			pclLand->DeleteApplyer( pclMsg->siPersonID );
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 농장 임차 신청취소 로그쓰기. 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FARM, LOGCOMMAND_INDEX_FARM_CANCELRENTAPPLY, 
				0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				0, 0, 0, 0, 0, NULL, NULL);
		}
	}
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_LAND, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}


// 임차 희망자 명단 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_APPLYERLIST(sPacketHeader* pPacket)
{
	sDBResponse_LandApplyerList* pclMsg = (sDBResponse_LandApplyerList*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if(pclMsg->siResult == 0) return ;

	cltLand *pclLand = NULL;
	for( int i = 0; i < MAX_APPLY_PER_LAND; i++ )
	{
		if(pclLandManager->pclStruct[pclMsg->siVillageUnique])
		{
			pclLand = (cltLand *)pclLandManager->pclStruct[pclMsg->siVillageUnique];
			pclLand->SetRentApplyer( i, &pclMsg->clApplyer[i].clPerson, &pclMsg->clApplyer[i].clContract );
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_LandApplyerList"));
}

// 임차 희망자 명단 삭제 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_DELAPPLYER(sPacketHeader* pPacket)
{
	sDBResponse_LandDelApplyer* pclMsg = (sDBResponse_LandDelApplyer *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siDelSlotNum < 0 || pclMsg->siDelSlotNum > MAX_APPLY_PER_LAND )				return;

	SI32 returnval = 0;
	SI32 id	= pclMsg->usCharID;

	if( pclMsg->siResult != 1 ) return;

	// 성공인 경우에는 게임서버의 임차 희망자 명단을 삭제한다. 
	cltLand *pclLand = (cltLand *)pclLandManager->pclStruct[pclMsg->siVillageUnique];
	
	// PCK : 임차희망자 명단중 삭제할 유저의 PersonID
	SI32	siDelPersonID = 0;

	if(pclMsg->siDelSlotNum >=0 && pclMsg->siDelSlotNum < MAX_APPLY_PER_LAND)
	{
		siDelPersonID = pclLand->clApplyer[pclMsg->siDelSlotNum].clPerson.GetPersonID();
	}

	// PCK : 임차 희망자 명단을 모두 삭제하는것을 수정
	if(siDelPersonID > 0)	pclLand->DeleteApplyer(siDelPersonID);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [로그추가 : 황진성 2007. 10. 10] // 임차 희망자 명단을 삭제. 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FARM, LOGCOMMAND_INDEX_FARM_RENTAPPLYDELETE, 
			0, (cltCharServer*)(pclCM->CR[id]), NULL, siDelPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			0, 0, 0, 0, 0, NULL, NULL);
	}

	// 클라이언트로 임차 희망자 명단을 모두 삭제하라는 메시지를 보낸다. 
	if(pclCM->IsValidID(id) == TRUE)
	{
		cltMsg clMsg(GAMEMSG_RESPONSE_LAND_DELAPPLYER, sizeof(SI32), (BYTE*)&pclMsg->siDelSlotNum);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// 임대 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_RENT(sPacketHeader* pPacket)
{

	sDBResponse_LandRent* pclMsg = (sDBResponse_LandRent*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = SRVAL_LAND_FAIL_RENT;

	cltLand *pclLand = (cltLand*)pclLandManager->pclStruct[ pclMsg->siVillageUnique ];
	if( pclLand == NULL ) return;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	// 희망자 명단에서 Person을 삭제한다. 
	pclLand->DeleteApplyer( pclMsg->clPerson.siPersonID );

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case -1:
		returnval	= SRVAL_LAND_FAIL_NOTSAMECONTRACT;		// 계약 조건이 다르다 
		break;

	case -2:
		returnval	= SRVAL_LAND_FAIL_NOMOREREALESTATE;		// 부동산 개수를 초과해서 가질 수 없다. 
		break;

	case 1:
		{
			returnval	= SRVAL_LAND_SUCCESS_RENT;				// 농장임대에 성공했다.
				
			cltLand *pclLand = (cltLand*)pclLandManager->pclStruct[ pclMsg->siVillageUnique ];
			if( pclLand == NULL ) return;

			// 시전 임차인으로 등록한다. 
			pclLand->SetRent( pclMsg->siIndex, &pclMsg->clPerson, &pclMsg->clRentContract );
			

			// 쪽지를 보낸다. 
			cltLetterMsg_RentLand clletter(pclMsg->siVillageUnique, pclMsg->siIndex);
			SI32 id = SendLetterMsg(pclMsg->clPerson.GetPersonID(), (cltLetterHeader*)&clletter);
			if(pclCM->IsValidID(id))
			{
				// Person이 파악한 부동산 정보를 업데이트 한다. 
				pclCM->CR[id]->pclCI->clRealEstate.Add(REALESTATE_TYPE_LAND, pclMsg->siVillageUnique, pclMsg->siIndex, &pclMsg->clRentContract);
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);

			}

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 10] // 임대 로그쓰기. 
				// param1		 == 단위 농장의 인덱스, 
				// param2		 == 임대 수수료.
				// param3		 == 임대 기간.
				// pszCharParam1 == 임대관련정보.
				TCHAR pszCharParam1[50] = {'\0', };
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
					pclMsg->clRentContract.clDate.uiYear,
					pclMsg->clRentContract.clDate.uiMonth,
					pclMsg->clRentContract.clDate.uiDay,
					pclMsg->clRentContract.clDate.uiHour,
					pclMsg->clRentContract.clDate.uiMinute );

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FARM, LOGCOMMAND_INDEX_FARM_RENT, 
					0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->clPerson.siPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					pclMsg->siIndex, pclMsg->clRentContract.siRentFee, pclMsg->clRentContract.siRentPeriod, 0, 0, 
					pszCharParam1, NULL);
			}
		}
		break;
	}

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg( RANKTYPE_LAND, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
	}

}

// 임대료 징수 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_PAYRENTFEE(sPacketHeader* pPacket)
{

	sDBResponse_LandPayRentFee* pclMsg = (sDBResponse_LandPayRentFee*)pPacket;

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

			// 연체 횟수가 3회를 초과하는 경우 농장을 초기화한다. 
			if(pclMsg->siDelayNum > 3)
			{
				// DB매니저에게 농장을 회수할 것을 요청한다. 
				sDBRequest_LandRecall clMsg(pclMsg->siPersonID, LANDRECALL_REASON_FEEDELAY, pclMsg->siVillageUnique, pclMsg->siLandIndex);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
			else
			{
				cltLetterMsg_PayLandRentFee clletter(pclMsg->siVillageUnique, pclMsg->siLandIndex, pclMsg->siRentFee, pclMsg->siDelayNum, pclMsg->siDelayMoney);
				// 쪽지를 DB나 클라이언트에게 보낸다. 
				SendLetterMsg(pclMsg->siPersonID, (cltLetterHeader*)&clletter);
			}
			

			if(pclCM->IsValidID(id))
			{
				// 지불자의 은행잔고를 업데이트 한다. 
				pclCM->CR[id]->pclCI->clBank.SetMoney( pclMsg->clBankMoney.itMoney );
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);
			}


			// 농장의 보유 자금을 업데이트 한다. 
			cltMoney clmoney;
			clmoney.Set( &pclMsg->clLandMoney );
			pclLandManager->SetMoney(pclMsg->siVillageUnique, &clmoney);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 10] // 농장 이용료 지불. 
				// param1 == 농장번호, 
				// param3 == 연체 횟수, 
				// param4 == 연체금,
				// param5 == 농장 보유자금
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FARM, LOGCOMMAND_INDEX_FARM_RENTFEEPAY, 
					0, NULL, NULL, pclMsg->siPersonID, pclMsg->siRentFee, pclMsg->clBankMoney.itMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					pclMsg->siLandIndex, 0, pclMsg->siDelayNum, pclMsg->siDelayMoney, pclMsg->clLandMoney.itMoney, NULL, NULL);
			}
		}
		break;

	case -1:
		{
			// 연체 횟수가 3회를 초과하는 경우 시전을 초기화한다. 
			if( pclMsg->siDelayNum > 3)
			{
				// DB매니저에게 농장을 회수할 것을 요청한다. 
				sDBRequest_LandRecall clMsg(pclMsg->siPersonID, LANDRECALL_REASON_FEEDELAY, pclMsg->siVillageUnique, pclMsg->siLandIndex);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
		break;

	}
}

// 농장  회수 
void cltServer::DoMsg_DBMSG_RESPONSE_LAND_RECALL(sPacketHeader* pPacket)
{

	sDBResponse_LandRecall* pclMsg = (sDBResponse_LandRecall*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siLandIndex < 0		|| pclMsg->siLandIndex >= MAX_LANDUNIT_NUMBER )			return;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 personid		= pclMsg->siPersonID;
	SI32 villageunique	= pclMsg->siVillageUnique;
	SI32 landindex		= pclMsg->siLandIndex;

	cltLand *pclLand = (cltLand*)pclLandManager->pclStruct[ villageunique ];
	if( pclLand == NULL ) return;

	pclLand->clUnit[ landindex ].Init();


	// 쪽지를 보낸다. 
	cltLetterMsg_LandRecall clletter(pclMsg->siRecallReason, pclMsg->siVillageUnique, pclMsg->siLandIndex);
	SI32 letterid = SendLetterMsg(personid, (cltLetterHeader*)&clletter);
	if(pclCM->IsValidID(letterid))
	{
		pclCM->CR[letterid]->pclCI->clRealEstate.Delete(REALESTATE_TYPE_LAND, pclMsg->siVillageUnique, pclMsg->siLandIndex);
		pclCM->CR[letterid]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 10] // 임대 완료(끝)  로그쓰기.
		// param1 == 농장 인덱스.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FARM, LOGCOMMAND_INDEX_FARM_END, 
							pclMsg->siRecallReason, NULL, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, villageunique, 0, 0, 
							landindex, 0, 0, 0, 0, NULL, NULL);
	}
}
