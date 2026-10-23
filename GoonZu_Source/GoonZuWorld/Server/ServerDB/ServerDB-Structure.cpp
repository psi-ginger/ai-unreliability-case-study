//---------------------------------
// 2003/10/17 김태곤
//---------------------------------

#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"


//-----------------------------
// Common
//-----------------------------
#include "..\..\Common\Map\Map.h"
#include "Map\FieldObject\FieldObject.h"

// 건물
#include "..\CommonLogic\Cityhall\Cityhall.h"
#include "..\CommonLogic\Bank\Bank-Manager.h"
#include "..\CommonLogic\Stock\Stock.h"
#include "..\CommonLogic\Market\Market.h"
#include "..\CommonLogic\House\House.h"
#include "..\CommonLogic\Hunt\Hunt.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"
#include "..\CommonLogic\HorseMarket\HorseMarket.h"
#include "..\CommonLogic\RealEstate\RealEstate.h"
#include "..\CommonLogic\SummonMarket\SummonMarket.h"
#include "..\CommonLogic\Land\Land.h"
#include "..\CommonLogic\Feast\Feast.h"
#include "..\CommonLogic\SummonHeroMarket\SummonHeroMarket.h"
#include "..\CommonLogic\Mine\Mine.h"
#include "..\CommonLogic\Guild\Guild.h"


#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Cityhall.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "MsgType-Structure.h"
#include "MsgRval-Define.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"


#define TUTORIALSTOCKMONEY 5000	//앙카란 듀토리얼 마을 주식 배당금


void cltServer::DoMsg_DBMSG_RESPONSE_CHANGESTRMONEY(sPacketHeader* pPacket)
{
	sDBResponse_ChangeStrMoney* pclMsg = (sDBResponse_ChangeStrMoney*)pPacket;

	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siStrType );

	// 성공이면 건물 보유자금 갱신 
	if( pclMsg->siResult == 1 ) 
	{
		if( pStrInfo ) 
		{
			pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clMoney );
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 각각의 건물의 건물의 보유 자금을 증가 시키거나 감소.
			// param1 == 건물 종류.  
			// param2 == 건물 종류, 
			// param3 == 증감 금액, 
			// param4 == 건물 보유자금 
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_CHANGE_STRUCTURE_MONEY, 
											pclMsg->siReason, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siStrType, pclMsg->lParam, pclMsg->siChangeMoney, pclMsg->clMoney.itMoney, 0, 
											NULL, NULL);
		}
	}

	switch( pclMsg->siReason )
	{
	case CHANGESTRMONEY_NPCPAY:
		{
			SI32 slot = LOWORD( pclMsg->lParam );
			SI32 npckind = HIWORD( pclMsg->lParam );

			if( pclMsg->siResult == 1 ) {
				// NPC 급여 지급 성공 			

				// 상단 입출금 내역에 추가

				if( pStrInfo ) {

					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( npckind,
						INOUTCODE_NPCPAY, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, npckind, 
						INOUTCODE_NPCPAY, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

					clMoney.Set( -clMoney.itMoney );
					clCurrencyInfo.clOutputMoney_NPCPayment.Increase( &clMoney );
				}
				

				
			} else {
				// 보유 자금 부족 고용해지

				sDBRequest_HireNPC clMsg( 0, pclMsg->siVillageUnique, slot, 0, 0, 0, &pclTime->clDate  , HIRENPC_HIRENPC );

				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

			}
			
		}
		break;
	case CHANGESTRMONEY_EVENTPAY:
		{
			if( pclMsg->siResult == 1 ) {
				// 이벤트 비용 지급 성공 			
				// 상단 입출금 내역에 추가

				if( pStrInfo ) {

					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 0,
						INOUTCODE_EVENTPAY, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, 0, 
						INOUTCODE_EVENTPAY, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}
			
			} else {
				// 보유 자금 부족 

			}
		}
		break;


	case CHANGESTRMONEY_VILLAGEMARK:
		{
			if( pclMsg->siResult == 1 ) {
				// 상단 입출금 내역에 추가

				if( pStrInfo ) {

					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 0,
						INOUTCODE_VILLAGEMARK, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, 0, 
						INOUTCODE_VILLAGEMARK, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}
			}
		}
		break;

	case CHANGESTRMONEY_VILLAGEMAP:
		{
			if( pclMsg->siResult == 1 ) {
				// 상단 입출금 내역에 추가

				if( pStrInfo ) {

					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					switch(pclMsg->siStrType)
					{
					case RANKTYPE_CITYHALL:
						{
							pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 0,
								INOUTCODE_VILLAGEMAP, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

							sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, 0, 
								INOUTCODE_VILLAGEMAP, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

							pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

						}
						break;
					case RANKTYPE_HUNT:
						{
							// 수렵장 정보 입출금 내역은 없음 => 위에서 건물 보유 자금 업뎃함
						}
						break;
					default:
						break;
					}
				}
			}
		}
		break;
	case CHANGESTRMONEY_INSTANCEPROFIT:
		{
			if( pclMsg->siResult == 1 ) {
				// 상단 입출금 내역에 추가

				if( pStrInfo ) {

					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 0,
						INOUTCODE_INSTANCEPROFIT, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, 0, 
						INOUTCODE_INSTANCEPROFIT, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	
				}
			}
		}
		break;
	case CHANGESTRMONEY_DESTORYSTR:
		{
			if( pclMsg->siResult == 1 ) {
				// 상단 입출금 내역에 추가

				if( pStrInfo ) {

					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 0,
						INOUTCODE_DESTROYSTR, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, 0, 
						INOUTCODE_DESTROYSTR, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}
			}
		}
		break;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETSTRINFO(sPacketHeader* pPacket)
{
	sDBResponse_SetStrInfo* pclMsg = (sDBResponse_SetStrInfo*)pPacket;

	// 결과를 확인한다. 
	if( pclMsg->siResult == 0 )
	{
		return ;
	}

	switch(pclMsg->clStrInfo.clCommonInfo.siStrType)
	{
	case RANKTYPE_BANK:
		pclBankManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 22] // 은행 세팅.
			// param1 == 타행 입출금 수수료,		 
			// param2 == 최저 수수료, 
			// param3 == 최대 수수료 
			// param4 == 계좌당 최대 입금 가능액,	 
			// param5 == 총입금액
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BANK, LOGCOMMAND_INDEX_BANK_SET_BANK, 
								 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								 pclMsg->clStrInfo.clBankStrInfo.clFee.GetFeeRate(),
								 pclMsg->clStrInfo.clBankStrInfo.clFee.GetMinFee(), 
								 pclMsg->clStrInfo.clBankStrInfo.clFee.GetMaxFee(), 
								 pclMsg->clStrInfo.clBankStrInfo.clMaxMoney.itMoney, 
								 pclMsg->clStrInfo.clBankStrInfo.clTotalInputMoney.itMoney, NULL, NULL);
		}
		break;

	case RANKTYPE_STOCK:
		pclStockManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		break;

	case RANKTYPE_MARKET:
		pclMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		break;

	case RANKTYPE_HUNT:
		pclHuntManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		break;

	case RANKTYPE_HOUSE:
		pclHouseManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		break;

	case RANKTYPE_CITYHALL:
		pclCityHallManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		break;

	case RANKTYPE_POSTOFFICE:
		pclPostOfficeManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		break;

	
	case RANKTYPE_HORSEMARKET:
		pclHorseMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	case RANKTYPE_REALESTATEMARKET:
		pclRealEstateMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	case RANKTYPE_SUMMONMARKET:
		pclSummonMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	
	case RANKTYPE_LAND:
		pclLandManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;
	case RANKTYPE_FEAST:
		pclFeastManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	case RANKTYPE_SUMMONHEROMARKET:
		pclSummonHeroMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	case RANKTYPE_MINE:
		pclMineManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;
	case RANKTYPE_GUILD:
		pclGuildManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	case RANKTYPE_NEWMARKET:
		pclNewMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
		break;

	default:
		MsgBox(TEXT("fdsv8dfjfd"), TEXT("fdskf83j:%d"), pclMsg->clStrInfo.clCommonInfo.siStrType);
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_GETSTRINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetStrInfo* pclMsg = (sDBResponse_GetStrInfo*)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	// 결과를 확인한다. 
	if(pclMsg->siResult == 0)
	{
		return ;
	}
	SI32 i;

	switch(pclMsg->clStrInfo.clCommonInfo.siStrType)
	{
	case RANKTYPE_BANK:
		pclBankManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_BANK"));
		break;

	case RANKTYPE_STOCK:
		pclStockManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_STOCK"));
		break;

	case RANKTYPE_MARKET:
		pclMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_MARKET"));
		break;

	case RANKTYPE_HUNT:
		pclHuntManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_HUNT"));
		break;

	case RANKTYPE_HOUSE:
		pclHouseManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_HOUSE"));
		break;

	case RANKTYPE_CITYHALL:
		{
			pclCityHallManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);

			//----------------------------------
			// NPC를 고용한다. 
			//----------------------------------
			for( i = 0; i < MAX_HIRED_NPC_NUM; i++ )
			{
				SI32 kind = pclMsg->clStrInfo.clCityhallStrInfo.clHiredNPC[i].siNPCKind;
				if( kind ) {

					Hire(kind, i, pclMsg->siVillageUnique, true);

					//----------------------------------
					// NPC Pay Manager에 추가
					//----------------------------------
					pclHiredNPCPayManager->AddNPC( kind );
				}
			}


			//----------------------------------
			// 이벤트 NPC를 만든다.
			//----------------------------------

//			SI32 siCurrentEventNPC = 0;

			for ( i = 0; i <MAX_HIRED_EVENTNPC_NUM; i++ ) {
				SI32 kind = pclMsg->clStrInfo.clCityhallStrInfo.clHiredEventNPC[i].siNPCKind;

				if( kind > 0 )
				{
					if( pclClient->pclKindInfoSet->IsValidKind( kind ) )
					{
						HireEventNPC(kind, pclMsg->siVillageUnique, i, true);
						//----------------------------------
						// NPC Pay Manager에 추가
						//----------------------------------
						pclHiredNPCPayManager->AddNPC( kind );
					}
				}
			}
 
			//------------------------------------
			// VillageManager에 선전포고 정보 설정 
			//------------------------------------
			//cltDate * CurDate = &pclTime->clDate;
			NDate StandardDate;	// 기준 시각. 당일 8시로 설정
			
			StandardDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);

				
			for( i = 0; i < MAX_VILLAGE_NUMBER; ++i )
			{
				if( pclMsg->clStrInfo.clCityhallStrInfo.bDeclareWarVillage[ i ] == 1 ) {

					pclVillageManager->siDeclareWarVillage[ pclMsg->siVillageUnique ] = i;
					
					//pclVillageManager->clDeclareWarDate[ pclMsg->siVillageUnique ].Set( 
					//	&pclMsg->clStrInfo.clCityhallStrInfo.clDeclareWarDate );
					pclVillageManager->clDeclareWarDate[pclMsg->siVillageUnique ].SetDate
						( pclMsg->clStrInfo.clCityhallStrInfo.uiDeclareWarDateYear
						 ,pclMsg->clStrInfo.clCityhallStrInfo.uiDeclareWarDateMonth
						 ,pclMsg->clStrInfo.clCityhallStrInfo.uiDeclareWarDateDay	);


					
					// 당일 저녁에 시작되는 공성전이라면 siNowWarVillage 정보에 추가
					NDate * DeclareWarDate = &pclVillageManager->clDeclareWarDate[ pclMsg->siVillageUnique ];

					// 공성전 선포 날짜가 당일 8시를 기준으로 3일 전이었다면 공성국가로 설정.
					if( DeclareWarDate->GetDateVary() + 2  == StandardDate.GetDateVary() )
					{
						pclVillageManager->siNowWarVillage[pclMsg->siVillageUnique] = pclVillageManager->siDeclareWarVillage[pclMsg->siVillageUnique];
					}
				}
			}
			
			clLastSettedNowVillageWarIinfo.uiYear  = sTime.wYear - 2000;
			clLastSettedNowVillageWarIinfo.uiMonth = sTime.wMonth;
			clLastSettedNowVillageWarIinfo.uiDay   = sTime.wDay;


			{
				cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];
				pclCityHall->clLevelInfo.siVisitorNum = pclMsg->clStrInfo.clCityhallStrInfo.siVisitLevel;
			}
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_CITYHALL"));
		}
		break;

	case RANKTYPE_POSTOFFICE:
		pclPostOfficeManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo);
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_POSTOFFICE"));
		break;
	

	case RANKTYPE_HORSEMARKET:
		pclHorseMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_HORSEMARKET"));
		break;

	case RANKTYPE_REALESTATEMARKET:
		pclRealEstateMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_REALESTATEMARKET"));
		break;

	case RANKTYPE_SUMMONMARKET:
		pclSummonMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_SUMMONMARKET"));
		break;

	
	case RANKTYPE_LAND:
		pclLandManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_LAND"));
		break;

	case RANKTYPE_FEAST:
		pclFeastManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_FEAST"));
		break;

	case RANKTYPE_SUMMONHEROMARKET:
		pclSummonHeroMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_SUMMONHEROMARKET"));
		break;

	case RANKTYPE_MINE:
		pclMineManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_MINE"));
		break;
	case RANKTYPE_GUILD:
		pclGuildManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_GUILD"));
		break;

	case RANKTYPE_NEWMARKET:
		pclNewMarketManager->SetStrInfo(pclMsg->siVillageUnique, &pclMsg->clStrInfo );
			pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStrInfo : RANKTYPE_NEWMARKET"));
		break;

	default:
		MsgBox(TEXT("fdjfd1"), TEXT("fdskf83j:%d"), pclMsg->clStrInfo.clCommonInfo.siStrType);
	}

}


void cltServer::DoMsg_DBMSG_RESPONSE_SETTAX( sPacketHeader* pPacket )
{

	SI32 id = 0;
	SI32 returnval = 0;

	sDBResponse_SetTax* pclMsg = (sDBResponse_SetTax *)pPacket;

	// 결과를 확인한다. 
	if(pclMsg->siResult == 0)
	{
		return ;
	} else {

		switch( pclMsg->siStrType )
		{
		case RANKTYPE_BANK:
			pclBankManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_STOCK:
			pclStockManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_MARKET:
			pclMarketManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_HUNT:
			pclHuntManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_HOUSE:
			pclHouseManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_CITYHALL:
			pclCityHallManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;
			
		case RANKTYPE_POSTOFFICE:
			pclPostOfficeManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		
		case RANKTYPE_HORSEMARKET:
			pclHorseMarketManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_REALESTATEMARKET:
			pclRealEstateMarketManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_SUMMONMARKET:
			pclSummonMarketManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		

		case RANKTYPE_LAND:
			pclLandManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_FEAST:
			pclFeastManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_SUMMONHEROMARKET:
			pclSummonHeroMarketManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		case RANKTYPE_MINE:
			pclMineManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;
		case RANKTYPE_GUILD:
			pclGuildManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;
		case RANKTYPE_NEWMARKET:
			pclNewMarketManager->SetTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clDueTax );
			break;

		default:
			MsgBox(TEXT("fdjfd2"), TEXT("fdskf83j:%d"), pclMsg->siStrType );
		}
	}

	returnval = SRVAL_TAX_SUCCESS_SETTAX;

	id = pclMsg->usCharID;
	if(pclCM->IsValidID(id)== false)
	{
		id = 0;
		return ;
	}

	if(returnval)
	{
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_PAYTAX( sPacketHeader* pPacket )
{
	SI32 id = 0;
	SI32 receiver = 0;
	SI32 returnval = 0;	
	
	cltMoney clMoney;
	
	sDBResponse_PayTax* pclMsg = (sDBResponse_PayTax *)pPacket;

	cltStrInfo *pclinfo1, *pclinfo2;

	receiver = pclMsg->siStrType;

	id = pclMsg->usCharID;
	if(pclCM->IsValidID(id)== false) {
		id = 0;
		return ;
	}

	// 결과를 확인한다. 
	if(pclMsg->siResult <= 0 )
	{
		if( pclMsg->siResult == -1 ) {
			returnval = SRVAL_TAX_FAIL_PAYTAX_NOTENOUGHMONEY;
		}	
		
	} else {

		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siPersonMoney );

		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

		//-------------------------------------
		// 행수에게 쪽지를 보낸다. 
		//-------------------------------------
		GMONEY salarymoney = pclMsg->siChiefPay;
		
		// 행수에게 쪽지 
		cltRank* pclrank = pclRankManager->GetRank(pclMsg->siStrType, pclMsg->siVillageUnique);
		if(pclrank)
		{
			cltLetterMsg_GetSalary clletter(&pclrank->clRank, salarymoney);
			// 쪽지를 DB나 클라이언트에게 보낸다. 
			SendLetterMsg(pclrank->clPerson.GetPersonID(), (cltLetterHeader*)&clletter);
		}

		// 상단 입출금 내역에 추가한다
		cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
		if( pclStrInfo ) {

			clMoney.Set( (SI64)(pclMsg->clPaidTax.itMoney - pclMsg->siChiefPay) );
			
			pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( pclMsg->siStrType,
				INOUTCODE_TAX, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, pclMsg->siStrType, 
				INOUTCODE_TAX, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			//{
			//	// TEMP LEEKH --------------------------------------------------------------
			//	// [로그추가 : 황진성 2007. 10. 18] // 관청 보유자금 입출금 내역.				
			//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
			//	TCHAR pszCharParam1[50] = {'\0', };
			//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
			//																pclTime->clDate.uiYear,
			//																pclTime->clDate.uiMonth,
			//																pclTime->clDate.uiDay,
			//																pclTime->clDate.uiHour,
			//																pclTime->clDate.uiMinute);


			//	cltServer* pclserver = (cltServer*)pclClient;
			//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
			//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, 
			//									pszCharParam1, NULL);
			//	// TEMP LEEKH --------------------------------------------------------------
			//}

		}


		switch( pclMsg->siStrType )
		{
		case RANKTYPE_BANK:
			pclBankManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );
			
			pclinfo1 = pclBankManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_STOCK:
			pclStockManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclStockManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_MARKET:
			pclMarketManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_HUNT:
			pclHuntManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclHuntManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;


		case RANKTYPE_HOUSE:
			pclHouseManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclHouseManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_POSTOFFICE:
			pclPostOfficeManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclPostOfficeManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		
		case RANKTYPE_HORSEMARKET:
			pclHorseMarketManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclHorseMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_REALESTATEMARKET:
			pclRealEstateMarketManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclRealEstateMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_SUMMONMARKET:
			pclSummonMarketManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclSummonMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		
		case RANKTYPE_LAND:
			pclLandManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclLandManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_FEAST:
			pclFeastManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclFeastManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_SUMMONHEROMARKET:
			pclSummonHeroMarketManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclSummonHeroMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;

		case RANKTYPE_MINE:
			pclMineManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclMineManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
			
			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;
		case RANKTYPE_GUILD:
			pclGuildManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclGuildManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;
		case RANKTYPE_NEWMARKET:
			pclNewMarketManager->SetPaidTax( pclMsg->siVillageUnique, pclMsg->usYear, &pclMsg->clPaidTax );

			pclinfo1 = pclNewMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			pclinfo2 = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

			if( pclinfo1 && pclinfo2 ) {
				pclinfo1->clCommonInfo.clMoney.Set(&pclMsg->clStructureMoney );
				//pclinfo2->clCommonInfo.clMoney.Set(&pclMsg->clCityhallMoney );
				pclinfo2->clCityhallStrInfo.clProfitMoney.Set(&pclMsg->clCityhallMoney );
			}
			break;


		default:
			MsgBox(TEXT("fdjfd3"), TEXT("fdskf83j:%d"), pclMsg->siStrType );
		}

		returnval = SRVAL_TAX_SUCCESS_PAYTAX;
	}

	if( returnval ) {
		SendServerResponseMsg( receiver, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// TEMP LEEKH --------------------------------------------------------------
			// [로그추가 : 황진성 2007. 10. 11] // 세금 납부. 
			// param1 == 건물의 타입, 
			// param2 == 시간 : 년, 
			// pszParam1 == 시간.
			TCHAR str[50] = {'\0', };
			StringCchPrintf(str, sizeof(str), TEXT("%d-%d-%d, %d : %d"),	
													pclMsg->clDate.uiYear,
													pclMsg->clDate.uiMonth,
													pclMsg->clDate.uiDay,
													pclMsg->clDate.uiHour,
													pclMsg->clDate.uiMinute);


			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_TAX, LOGCOMMAND_INDEX_TAX_PAY, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->clPaidTax.itMoney, pclMsg->siPersonMoney, 
											NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siStrType, pclMsg->usYear, 0, 0, 0, str, NULL);
			// TEMP LEEKH --------------------------------------------------------------
		}

	}


}

void cltServer::DoMsg_DBMSG_RESPONSE_PAYDIVIDEND( sPacketHeader* pPacket )
{
	
	//SI32 id = 0;
	//SI32 receiver = 0;
	//SI32 returnval = 0;	
	//
	//sDBResponse_PayDividend* pclMsg = (sDBResponse_PayDividend *)pPacket;

	//id = pclMsg->usCharID;
	//if(pclCM->IsValidID(id)== false) {
	//	id = 0;
	//	return ;
	//}

	//// 결과를 확인한다. 
	//if(pclMsg->siResult <= 0 )
	//{
	//	switch( pclMsg->siResult )
	//	{
	//	case -1:
	//		returnval = SRVAL_DIVIDEND_FAIL_NOTENOUGHMONEY;
	//		break;

	//	case -2:
	//		returnval = SRVAL_DIVIDEND_FAIL_WRONGMONEY;
	//		break;
	//	}
	//	
	//} else {

	//	pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siPersonMoney );

	//	pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

	//	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

	//	// 상단 입출금 내역에 추가한다

	//	if( pclStrInfo ) {

	//		//pclStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );	// 배당후 상단 보유자금 
	//		pclStrInfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clCityhallMoney ); // 배당후 상단 보유자금 

	//		pclStrInfo->clCityhallStrInfo.clDividendInfo.AddDividendInfo( &pclMsg->clDate, pclMsg->siTotalStock, &pclMsg->clDividendPrice );


	//		cltMoney clMoney;

	//		clMoney.Set( -(SI64)(pclMsg->clDividendPrice.itMoney * pclMsg->siTotalStock) );

	//		pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
	//			INOUTCODE_DIVIDEND, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	//		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
	//			INOUTCODE_DIVIDEND, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	//		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
	//	}
	//	
	//	cltStrInfo *pclParentStrInfo = GetStrInfo( pclMsg->siParentVillage, RANKTYPE_CITYHALL );

	//	if( pclParentStrInfo ) {
	//		//pclParentStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clParentCityhallMoney );		// 배당후 부모 마을 상단 보유자금 
	//		pclParentStrInfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clParentCityhallMoney );		// 배당후 부모 마을 상단 보유자금 
	//	}

	//	// 상단 입출금 내역(조공보냄)
	//	{
	//		if( pclParentStrInfo ) {

	//			cltMoney clMoney;
	//			clMoney.Set( -(SI64)(pclMsg->siParentPay ) );

	//			pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( pclMsg->siParentVillage,
	//				INOUTCODE_TOPARENT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	//			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, pclMsg->siParentVillage, 
	//				INOUTCODE_TOPARENT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	//			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
	//			
	//		}
	//	}

	//	// 상단 입출금 내역(조공받음)
	//	{
	//		if( pclParentStrInfo ) {

	//			cltMoney clMoney;
	//			clMoney.Set( (SI64)(pclMsg->siParentPay ) );

	//			pclParentStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( pclMsg->siVillageUnique,
	//				INOUTCODE_FROMCHILD, &pclTime->clDate, &clMoney, &pclMsg->clParentCityhallMoney );

	//			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siParentVillage, pclMsg->siVillageUnique, 
	//				INOUTCODE_FROMCHILD, &pclTime->clDate, &clMoney, &pclMsg->clParentCityhallMoney );

	//			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
	//		}
	//	}

	//	// 대행수에게 쪽지 
	//	cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_CITYHALL, pclMsg->siVillageUnique);
	//	if(pclrank)
	//	{
	//		cltLetterMsg_GetSalary clletter(&pclrank->clRank, pclMsg->siChiefPay);
	//		// 쪽지를 DB나 클라이언트에게 보낸다. 
	//		SendLetterMsg(pclrank->clPerson.GetPersonID(), (cltLetterHeader*)&clletter);
	//	}
	//	
	//	// 군주에게 쪽지 
	//	if( pclMsg->siKingPersonID )
	//	{
	//		cltLetterMsg_GetKingPay clletter( pclMsg->siVillageUnique, pclMsg->siKingPay, pclMsg->siKingPayPersent );

	//		// 쪽지를 DB나 클라이언트에게 보낸다. 
	//		SendLetterMsg( pclMsg->siKingPersonID, (cltLetterHeader*)&clletter);
	//	}
	//	

	//	// 배당 받은 사용자 개개인에게 쪽지를 보낸다. 
	//	for(SI32 i = 0;i < pclMsg->usPersonCount;i++)
	//	{
	//		cltLetterMsg_GetDividend clletter(pclMsg->siVillageUnique, (SI32)(pclMsg->clDividendPerson[i].clMoney.itMoney * pclMsg->clDividendPrice.itMoney));
	//		// 쪽지를 DB나 클라이언트에게 보낸다. 
	//		SendLetterMsg(pclMsg->clDividendPerson[i].siPersonID, (cltLetterHeader*)&clletter);
	//	}


	//	returnval = SRVAL_DIVIDEND_SUCCESS_PAYDIVIDEND;
	//}

	//if( returnval ) {
	//	SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	//}

}

void cltServer::DoMsg_DBMSG_RESPONSE_AUTOPAYDIVIDEND( sPacketHeader* pPacket )
{
	sDBResponse_AutoPayDividend* pclMsg = (sDBResponse_AutoPayDividend*)pPacket;

	if( pclMsg == NULL )
		return;

	if( pclMsg->siResult != 1 )
		return;

	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

	// 상단 입출금 내역에 추가한다
	if( pclStrInfo )
	{	
		// [영훈] 초보자 마을 : 초보자 마을은 자동으로 주주배당을 하지 않는다.
		if( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
		{
			if ( pclMsg->siVillageUnique == Const_Beginner_Village )
			{
				return;
			}
		}

		pclStrInfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clCityhallMoney ); // 배당후 상단 보유자금 
		pclStrInfo->clCityhallStrInfo.clDividendInfo.AddDividendInfo( &pclMsg->clDate, pclMsg->siTotalStock, &pclMsg->clStockDividendPrice );

		cltMoney clMoney;
		clMoney.Set( -(SI64)(pclMsg->clStockDividendPrice.itMoney * pclMsg->siTotalStock) );

		pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_DIVIDEND, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
			INOUTCODE_DIVIDEND, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// TEMP LEEKH --------------------------------------------------------------
		//	// [로그추가 : 황진성 2007. 10. 18] // 관청 보유자금 입출금 내역.				
		//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
		//	TCHAR pszCharParam1[50] = {'\0', };
		//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
		//																pclTime->clDate.uiYear,
		//																pclTime->clDate.uiMonth,
		//																pclTime->clDate.uiDay,
		//																pclTime->clDate.uiHour,
		//																pclTime->clDate.uiMinute);

		//	
		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, 
		//									pszCharParam1, NULL);
		//}
		// 주민 배당으로 나간거 기록
		clMoney.Set( -(SI64)(pclMsg->clResidentDividendPrice.itMoney * pclStrInfo->clCityhallStrInfo.siResidentNum) );

		pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_RESIDENTSHARE, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		sDBRequest_CityHallSetInOutMoney sendMsg2( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
			INOUTCODE_RESIDENTSHARE, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg2 );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// TEMP LEEKH --------------------------------------------------------------
		//	// [로그추가 : 황진성 2007. 9. 20] // 관청 보유자금 입출금 내역.				
		//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.

		//	TCHAR pszCharParam1[50] = {'\0', };
		//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
		//																pclTime->clDate.uiYear,
		//																pclTime->clDate.uiMonth,
		//																pclTime->clDate.uiDay,
		//																pclTime->clDate.uiHour,
		//																pclTime->clDate.uiMinute);


		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, 
		//									pszCharParam1, NULL);
		//	// TEMP LEEKH --------------------------------------------------------------
		//}

		/*/--------------------------------------------------------------------------------//
		주민 배당금 나간 것 로그 기록 2007.09.05
		//--------------------------------------------------------------------------------/*/
		SI32 villageuniaue	= pclMsg->siVillageUnique;					// 배당받는 마을 유니크
		SI32 totalstock		= pclMsg->siTotalStock;						// 총배당 주식수.
		GMONEY totalmoney	= pclMsg->clResidentDividendPrice.itMoney;	// 주민 한명 당 받은돈
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK,LOGCOMMAND_INDEX_STOCKRESIDENSHARE,0,NULL,NULL,0,
							 0, 0, NULL, 0, 0, 0, villageuniaue, 0, 0,
							 totalmoney, totalstock, pclMsg->usResidentPersonCount, 0, 0, 
							 NULL, NULL);
		//----------------------------------------------------------------------------------//
		
		// 왕실세금으로 들어간거
		clMoney.Set( -(SI64)pclMsg->clTaxDividendPrice.itMoney  );

		pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_NATIONTAX, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		sDBRequest_CityHallSetInOutMoney sendMsg3( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
			INOUTCODE_NATIONTAX, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg3 );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// TEMP LEEKH --------------------------------------------------------------
		//	// [로그추가 : 황진성 2007. 9. 20] // 관청 보유자금 입출금 내역.				
		//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
		//	TCHAR pszCharParam1[50] = {'\0', };
		//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
		//																pclTime->clDate.uiYear,
		//																pclTime->clDate.uiMonth,
		//																pclTime->clDate.uiDay,
		//																pclTime->clDate.uiHour,
		//																pclTime->clDate.uiMinute);


		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, 
		//									pszCharParam1, NULL);
		//	// TEMP LEEKH --------------------------------------------------------------
		//}
	}

	// 대행수에게 쪽지 
	cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_CITYHALL, pclMsg->siVillageUnique);
	if(pclrank)
	{
		cltLetterMsg_GetSalary clletter(&pclrank->clRank, pclMsg->siChiefPay);
		// 쪽지를 DB나 클라이언트에게 보낸다. 
		SendLetterMsg(pclrank->clPerson.GetPersonID(), (cltLetterHeader*)&clletter);

		//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다.
		//pclCM->CR[pclrank->clPerson.GetPersonID()]->pclCI->clBank.SetMoney( pclMsg->siPersonMoney );
		//pclCM->CR[pclrank->clPerson.GetPersonID()]->SetUpdateSwitch(UPDATE_BANK, true, 0);

		SI32 siRankID = pclCM->GetIDFromPersonID(pclrank->clPerson.GetPersonID());
		if(pclCM->IsValidID(siRankID))
		{
			pclCM->CR[siRankID]->pclCI->clBank.SetMoney( pclMsg->siPersonMoney );
			pclCM->CR[siRankID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			
		}
	}

	// 군주에게 쪽지 
	if( pclMsg->siKingPersonID )
	{
		cltLetterMsg_GetKingPay clletter( pclMsg->siVillageUnique, pclMsg->siKingPay, pclMsg->siKingPayPersent );

		// 쪽지를 DB나 클라이언트에게 보낸다. 
		SendLetterMsg( pclMsg->siKingPersonID, (cltLetterHeader*)&clletter);

		//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다.
		//pclCM->CR[ pclMsg->siKingPersonID]->pclCI->clBank.SetMoney(  pclMsg->clKingAllMoney );
		//pclCM->CR[ pclMsg->siKingPersonID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			

		SI32 siKingID = pclCM->GetIDFromPersonID( pclMsg->siKingPersonID );
		if(pclCM->IsValidID(siKingID))
		{
			pclCM->CR[siKingID]->pclCI->clBank.SetMoney( pclMsg->clKingAllMoney );
			pclCM->CR[siKingID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			
		}
	}

	if( pclMsg->usStockPersonCount > MAX_DIVIDEND_PERSON )		pclMsg->usStockPersonCount = MAX_DIVIDEND_PERSON;
	SI32 i;
	// (주주)배당 받은 사용자 개개인에게 쪽지를 보낸다. 
	for( i = 0;i < pclMsg->usStockPersonCount; ++i)
	{
		if (pclMsg->clStockDividendPerson[i].siPersonID > 0 &&  pclMsg->clStockDividendPerson[i].clMoney.itMoney > 0)
		{
			cltLetterMsg_GetDividend clletter(pclMsg->siVillageUnique, pclMsg->clStockDividendPerson[i].clMoney.itMoney, 0);
			// 쪽지를 DB나 클라이언트에게 보낸다. 
			SendLetterMsg(pclMsg->clStockDividendPerson[i].siPersonID, (cltLetterHeader*)&clletter);

			//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다.
			//pclCM->CR[pclMsg->clStockDividendPerson[i].siPersonID]->pclCI->clBank.SetMoney(  pclMsg->clStockDividendPerson[i].clAllMoney.itMoney );
			//pclCM->CR[pclMsg->clStockDividendPerson[i].siPersonID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			

			SI32 siStockDividendID = pclCM->GetIDFromPersonID( pclMsg->clStockDividendPerson[i].siPersonID );
			if(pclCM->IsValidID(siStockDividendID))
			{
				pclCM->CR[siStockDividendID]->pclCI->clBank.SetMoney( pclMsg->clStockDividendPerson[i].clAllMoney.itMoney );
				pclCM->CR[siStockDividendID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 11] // 주식 배당금
					cltServer * pclServer = (cltServer*)pclClient;
					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_DIVIDENDRESIDENTS, 
													0, NULL, NULL, pclMsg->clStockDividendPerson[i].siPersonID, 
													pclMsg->clStockDividendPerson[i].clMoney.itMoney, 
													pclMsg->clStockDividendPerson[i].clAllMoney.itMoney, NULL, 0, NULL, 0, pclMsg->siVillageUnique, 0, 0, 
													0, 0, 0, 0, 0, NULL, NULL);
				}
			}
		}
	}

	if( pclMsg->usResidentPersonCount > MAX_DIVIDEND_PERSON )		pclMsg->usResidentPersonCount = MAX_DIVIDEND_PERSON;
	// (주민)배당 받은 사용자 개개인에게 쪽지를 보낸다. 
	for( i = 0;i < pclMsg->usResidentPersonCount; ++i)
	{
		if (pclMsg->clResidentDividendPerson[i].siPersonID > 0 && pclMsg->clResidentDividendPerson[i].clMoney.itMoney > 0)
		{
			cltLetterMsg_GetDividend clletter(pclMsg->siVillageUnique, pclMsg->clResidentDividendPerson[i].clMoney.itMoney, 1);
			// 쪽지를 DB나 클라이언트에게 보낸다. 
			SendLetterMsg(pclMsg->clResidentDividendPerson[i].siPersonID, (cltLetterHeader*)&clletter);

			//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다.
			//pclCM->CR[pclMsg->clStockDividendPerson[i].siPersonID]->pclCI->clBank.SetMoney(  pclMsg->clStockDividendPerson[i].clAllMoney.itMoney );
			//pclCM->CR[pclMsg->clStockDividendPerson[i].siPersonID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			

			SI32 siStockDividendID = pclCM->GetIDFromPersonID( pclMsg->clStockDividendPerson[i].siPersonID );
			if(pclCM->IsValidID(siStockDividendID))
			{
				pclCM->CR[siStockDividendID]->pclCI->clBank.SetMoney( pclMsg->clStockDividendPerson[i].clAllMoney.itMoney );
				pclCM->CR[siStockDividendID]->SetUpdateSwitch(UPDATE_BANK, true, 0);			
			}
		}
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// TEMP LEEKH --------------------------------------------------------------
		// [로그추가 : 황진성 2007. 10. 18] // 자동 배당하기	(수정필요 : 정보를 다 못 보냈음.)		
		// param1	 == 한주당 배당금, 
		// param2	 == 주민 한명당 받은돈 금액, 
		// param3	 == 왕실 세금으로 납부한 금액,
		// pszParam1 == 최근 배당일.
		TCHAR pszCharParam1[50] = {'\0', };
		StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
																	pclTime->clDate.uiYear,
																	pclTime->clDate.uiMonth,
																	pclTime->clDate.uiDay,
																	pclTime->clDate.uiHour,
																	pclTime->clDate.uiMinute);


		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_DIVIDENDSUMMARY, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->clStockDividendPrice.itMoney, 
										pclMsg->clResidentDividendPrice.itMoney, 
										pclMsg->clTaxDividendPrice.itMoney, 
										0, 0, 
										pszCharParam1, NULL);
		// TEMP LEEKH --------------------------------------------------------------
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_PAYFEE( sPacketHeader* pPacket )
{
	sDBResponse_PayFee *pclMsg = (sDBResponse_PayFee *)pPacket;

	cltStrInfo *pclinfo;

	// 결과를 확인한다. 
	if(pclMsg->siResult <= 0 )
	{

	} else {
		
		pclinfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siStrType );

		if( pclinfo ) {
			
			// 건물 보유자금 갱신
			pclinfo->clCommonInfo.clMoney.Set( &pclMsg->clStructureMoney );

			SI32 id	= pclMsg->usCharID;

			if( pclCM->IsValidID(id) == TRUE ) {

				if( pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID ) {

					// 납부자 계좌 정보 갱신
					pclCM->CR[id]->pclCI->clBank.SetMoney( pclMsg->clPersonMoney.itMoney );

					// 클라이언트에게 알려준다. 
					pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);

				}

			}

		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 각각의 건물의 수수료 납부를 요청.
			// param1 == 건물의 타입.  
			// param2 == 수수료 타입, 
			// param4 == 건물의 보유자금
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_PAY_FEE, 
											0, NULL, NULL, pclMsg->siPersonID, pclMsg->clFeeMoney.itMoney, pclMsg->clPersonMoney.itMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siStrType, pclMsg->siFeeType, 0, pclMsg->clStructureMoney.itMoney, 0, 
											NULL, NULL);
		}

	}

}


void cltServer::DoMsg_DBMSG_RESPONSE_CHIEFINFO(sPacketHeader* pPacket)
{
	sDBResponse_ChiefInfo *pclMsg = (sDBResponse_ChiefInfo *)pPacket;

	if( pclMsg->siResult != 1 )						return;

	// 말이 죽었을때만 처리한다.
	SI32 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		// 클라이언트로 정보를 보낸다. 
		cltGameMsgResponse_ChiefInfo clInfo(&pclMsg->Rank, pclMsg->AccountID);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHIEFINFO, sizeof(clInfo), (BYTE*)&clInfo);
		
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	return;
}

// kkm
void cltServer::DoMsg_DBMSG_RESPONSE_REPAIRSTRUCTURE(sPacketHeader* pPacket)
{
	sDBResponse_RepairStructure *pclMsg = (sDBResponse_RepairStructure *)pPacket;

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE)
		return;

	switch( pclMsg->siResult )
	{
	case 1:
		{

			cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siRankType );

			if( pclStrInfo )
			{
				pclStrInfo->clCommonInfo.siRepairStrDura = pclMsg->siUpdateStrRepairAmount;
				pclStrInfo->clCommonInfo.clBaseMoney.itMoney = pclMsg->siLeftBaseMoney;
			}

			cltGameMsgResponse_RepairStructure sendMsg( pclMsg->siRankType, pclMsg->siUpdateStrRepairAmount, pclMsg->siLeftBaseMoney );
			cltMsg clMsg( GAMEMSG_RESPONSE_REPAIRSTRUCTURE, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		
			if(0 != pclMsg->siItemUnique)
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo( pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0 );
				cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);
			}
			
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 11] // 건물 수리. 
				// param1 == 건물 종류. 
				// param2 == 수리할려교 요청하는 양. 
				// param3 == 수리학고 난 뒤에 수리량(누적값 최고 1000)
				// param4 == 수리하고 난뒤의 건물 자본금
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_REPAIR, 
												0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
												pclMsg->siRankType, pclMsg->siStrRepairAmount, pclMsg->siUpdateStrRepairAmount, pclMsg->siLeftBaseMoney, 0, NULL, NULL);
			}

		}
		break;

	}
}
/*
void cltServer::DoMsg_DBMSG_RESPONSE_REPAIRSTRUCTURE(sPacketHeader* pPacket)
{
	sDBResponse_RepairStructure *pclMsg = (sDBResponse_RepairStructure *)pPacket;
	
	SI32 id = pclMsg->usCharID;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	switch( pclMsg->siResult ) {
	case 1:
		{

			cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siRankType );

			if( pclStrInfo ) {
				pclStrInfo->clCommonInfo.siUseItemNum = pclMsg->siUseItemNum;
			}

			// 인벤토리 갱신
//			if( pclCM->IsValidID(id) ) //&& pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
//			{

			if ( pclMsg->siItemNum < 0 || pclMsg->siItemNum > MAX_ITEM_PILE_NUMBER )
			{
				return;
			}

			if ( pclMsg->siItemNum == 0 )
			{
				pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].Init();
			}
			else
			{
				if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siUnique != pclMsg->siItemUnique )
				{
					return;
				}

				pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siUnique = pclMsg->siItemUnique;
				pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siItemNum = pclMsg->siItemNum;
			}

			pclCM->CR[id]->SetUpdateSwitch( UPDATE_INVENTORY, true , 0);

			cltGameMsgResponse_RepairStructure sendMsg( pclMsg->siRankType, pclMsg->siUseItemNum );
			cltMsg clMsg( GAMEMSG_RESPONSE_REPAIRSTRUCTURE, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
		break;

	}

}
*/

void cltServer::DoMsg_DBMSG_RESPONSE_SETSTRREPAIRITEM(sPacketHeader* pPacket)
{
	sDBResponse_SetStrRepairItem *pclMsg = (sDBResponse_SetStrRepairItem *)pPacket;

	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siRankType );

	if( pclStrInfo ) {
		pclStrInfo->clCommonInfo.siNeedItemUnique = pclMsg->siItemUnique;
		pclStrInfo->clCommonInfo.siNeedItemNum = pclMsg->siItemNum;
		pclStrInfo->clCommonInfo.siUseItemNum = 0;
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_SETSTRDURA(sPacketHeader* pPacket)
{
	sDBResponse_SetStrDura *pclMsg = (sDBResponse_SetStrDura *)pPacket;
	
	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siRankType );

	if( pclStrInfo ) {
		pclStrInfo->clCommonInfo.siStrDura = pclMsg->siStrDura;
	}
	
	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 11] // 건물의 내구도.
		// param1 == 건물 종류. 
		// param2 == 내구도.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_SET_STRDUR, 
										0, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->siRankType, pclMsg->siStrDura, 0, 0, 0, NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_CHANGESTRDURA(sPacketHeader* pPacket)
{
	sDBResponse_ChangeStrDura *pclMsg = (sDBResponse_ChangeStrDura *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siRankType );

	if( pclStrInfo ) {
		pclStrInfo->clCommonInfo.siStrDura = pclMsg->siStrDura;
	}
	else return ;

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 11] // 건물의 내구도 변경으로 건물해체 .
	//	// param1 == 건물 종류. param2 == 내구도.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_CHANGE_STRDUR, 
	//									0, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
	//									pclMsg->siRankType, pclMsg->siStrDura, 0, 0, 0, NULL, NULL);
	//}

	// 내구도가 0 이하이면 건물해체
	if( pclMsg->siStrDura <= 0 ) {

		SI32 strunique = pclVillageStructureSet->GetStrUniqueFromRankType( pclMsg->siRankType );
		if( strunique == 0 ) return;

		cltVillageInfo *pclVillageInfo = pclVillageManager->pclVillageInfo[ pclMsg->siVillageUnique ];
		if( pclVillageInfo == NULL ) return;

		// 해체가능한 건물인가..
		int ref = pclVillageStructureSet->FindRefFromRankType( pclMsg->siRankType );
		if( ref == -1 ) return;
		// 실제로 마을에 있는가?
		if( pclVillageInfo->clVillageVary.clStructureInfo.IsThereStructure(pclMsg->siRankType) == FALSE ) return;

		// 해체 불가능한 건물임..
		if( pclVillageStructureSet->pclVillageStructureInfo[ ref ]->bDestroyable == FALSE )
		{

			// 내구 0으로 설정 
			{
				sDBRequest_SetStrDura sendMsg( pclMsg->siVillageUnique, pclMsg->siRankType, 0 );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}

			// 보유 자금 삭감
			{
				cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
				if ( pclStrInfo == NULL ) return ;

				SI32 money = (SI32)(pclStrInfo->clCommonInfo.clMoney.itMoney * 0.5);

				if( money > 0 ) {
					sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_DESTORYSTR, 0, pclMsg->siVillageUnique,
													   RANKTYPE_CITYHALL, -money );
					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

				}
			}

		}
		else // 해체 가능한 건물임
		{
			SI32 slot = pclVillageInfo->clVillageVary.clStructureInfo.GetSlotFromStrUnique( strunique );
			if( slot == -1 ) return;


			SI32 sidatevary = pclTime->clDate.GetDateVary();
			sDBRequest_DestroyStructure sendMsg( 0, pclMsg->siVillageUnique, slot, strunique, pclMsg->siRankType, sidatevary );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		}
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_CHANGESTRBASEMONEY(sPacketHeader* pPacket)
{
	sDBResponse_ChangeStrBaseMoney *pclMsg = (sDBResponse_ChangeStrBaseMoney *)pPacket;

	if( pclMsg->siResult != 1 )
		return;

	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )
		return;

	SI32 tempVillageUnique = pclMsg->siVillageUnique;

	// 관청 자본금을 빼주자
	cltMoney clMoney;
	clMoney.itMoney = -pclMsg->siChangeMoney;

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( tempVillageUnique );

	pclinfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );

	// 상단 최근 갱신일 수정 
	pclinfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
		INOUTCODE_SETBASEMONEY, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	sDBRequest_CityHallSetInOutMoney sendMsg( tempVillageUnique, RANKTYPE_CITYHALL, 
		INOUTCODE_SETBASEMONEY, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	clMoney;
	clMoney.Set(&pclMsg->clBaseMoney);

	SI32 tempStrType = pclMsg->siStrType;
	if( RANKTYPE_CITYHALL == tempStrType)
	{
		pclCityHallManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	if( RANKTYPE_BANK ==  tempStrType)
	{
		pclBankManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_STOCK == tempStrType )
	{
		pclStockManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_MARKET == tempStrType )
	{
		pclMarketManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_HOUSE == tempStrType )
	{
		pclHouseManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_POSTOFFICE == tempStrType )
	{
		pclPostOfficeManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_HORSEMARKET == tempStrType )
	{
		pclHorseMarketManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_REALESTATEMARKET == tempStrType )
	{
		pclRealEstateMarketManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_SUMMONMARKET == tempStrType )
	{
		pclSummonMarketManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_LAND == tempStrType )
	{
		pclLandManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_FEAST == tempStrType )
	{
		pclFeastManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_SUMMONHEROMARKET == tempStrType )
	{
		pclSummonHeroMarketManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_MINE == tempStrType )
	{
		pclMineManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if( RANKTYPE_HUNT == tempStrType )
	{
		pclHuntManager->SetBaseMoney( tempVillageUnique, &clMoney );
	}
	else if ( RANKTYPE_GUILD == tempStrType )
	{
		pclGuildManager->SetBaseMoney(tempVillageUnique,&clMoney);
	}
	else if ( RANKTYPE_NEWMARKET == tempStrType )
	{
		pclNewMarketManager->SetBaseMoney(tempVillageUnique,&clMoney);
	}

	SI32 id = pclMsg->siCharID;
	if( pclCM->IsValidID(id) )
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_SETBASEMONEY,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETSTRREPAIRAMOUNT(sPacketHeader* pPacket)
{
	sDBResponse_SetStrRepairAmount *pclMsg = (sDBResponse_SetStrRepairAmount *)pPacket;

	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siRankType );

	if( pclStrInfo ) 
	{
		pclStrInfo->clCommonInfo.siRepairStrDura = 0;
	}

	
	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 11] // 건물의 수리량을 요청.
	//	// param1 == 건물 종류.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_SET_REPAIR_AMOUNT, 
	//									0, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
	//									pclMsg->siRankType, 0, 0, 0, 0, NULL, NULL);
	//}
}

void cltServer::DoMsg_DBMSG_RESPONSE_CONTRIBUTION_ITEM_LIST_INPUT(sPacketHeader* pPacket)
{
	sDBResponse_Contribution_Item_List_Input* pclMsg = (sDBResponse_Contribution_Item_List_Input*)pPacket;

	cltCharServer* pclchar = pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclchar )	return;

	cltItem clUseItem;
	SI32	siInvenPos = -1;
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 < pclMsg->m_siInputItemIvenPos[siCount] ) 
		{
			siInvenPos = pclchar->pclCI->clCharItem.GetItemPos( pclMsg->m_siItemUnique[siCount] );
			if( pclMsg->m_siInputItemIvenPos[siCount] != siInvenPos ) continue;

			clUseItem.Set( &pclchar->pclCI->clCharItem.clItem[siInvenPos] );
			clUseItem.siItemNum = pclMsg->m_siInputItemNum[siCount];

			sDBRequest_UseItem useitem(	pclchar->GetID(),
										pclchar->pclCI->GetPersonID(),
										siInvenPos,
										&clUseItem, 
										USEITEM_REASON_CONTRIBUTION_ITEM_INPUT	);

			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
		}
	}

	cltGameMsgResponse_Contribution_Item_List_Input clinfo( pclMsg->m_siItemUnique, pclMsg->m_siItemNum );
	cltMsg clMsg( GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_LIST_INPUT, PACKET(clinfo) );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_CONTRIBUTION_MAKE_NPC(sPacketHeader* pPacket)
{
	sDBResponse_Contribution_Make_NPC* pclMsg = (sDBResponse_Contribution_Make_NPC*)pPacket;

	if( 1 != pclMsg->m_siResult )	return;

	cltCharServer* pclchar = pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclchar )	return;

	// npc 슬롯이 1번이다.
	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->m_siVillageUnique );
	if( pclinfo == NULL )	return;

	// 만들어질 NPC의 변경전 NPC를 찾는다.
	cltContributionInfo* pclContributionInfo = NULL;
	for( SI32 siCount = 0; siCount < MAX_HIRED_EVENTNPC_NUM; siCount++ )
	{
		if( pclinfo->clCityhallStrInfo.clHiredEventNPC[siCount].siNPCKind == pclMsg->m_siNPCKind ) 
		{
			pclContributionInfo = m_clContributionEvent_Common.FindContributionInfo( pclMsg->m_siNPCKind );
			if( NULL == pclContributionInfo ) return;

			// NPC 해고
			sDBRequest_HireNPC clFireMsg(	pclMsg->m_usCharID, 
											pclMsg->m_siVillageUnique, 
											siCount, 
											0, 
											0, 
											0, 
											&pclTime->clDate, 
											HIRENPC_HIREEVENTNPC );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&clFireMsg );

			// NPC 고용
			sDBRequest_HireNPC clHireMsg(	pclMsg->m_usCharID, 
											pclMsg->m_siVillageUnique, 
											siCount, 
											pclContributionInfo->m_siMakeNPCKind, 
											0, 
											0, 
											&pclTime->clDate, 
											HIRENPC_HIREEVENTNPC );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );

			break;
		}
	}

	if( false == m_clContributionEvent_Common.CompleteMakeNpc( pclMsg->m_siNPCKind, pclchar ) )
		return;

	// 건축물 완성 공지.
	cltGameMsgResponse_Event_CompleteBuild clComplete( pclMsg->m_siVillageUnique, 0, pclContributionInfo->m_siMakeNPCKind );
	cltMsg clCompleteMsg( GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD, sizeof( clComplete ), (BYTE*)&clComplete );
	pclCM->SendAllPersonMsg( (sPacketHeader*)&clCompleteMsg );
}
