//---------------------------------
// 2003/10/13 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-House.h"

#include "MsgRval-Define.h"

// 부동산 계약을 연장한다. 
void cltServer::DoMsg_DBMSG_RESPONSE_EXTENDCONTRACT(sPacketHeader* pPacket)
{
	sDBResponse_ExtendContract* pclMsg = (sDBResponse_ExtendContract*)pPacket;

	SI32 returnval = 0;

	SI32 villageunique	= pclMsg->siVillageUnique;
	SI32 realestatetype	= pclMsg->siRealEstateType;
	SI32 houseindex		= pclMsg->siIndex;

	cltRentContract* pclcontract = &pclMsg->clRentContract; 
		
	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case 1: 
		{
			switch(realestatetype)
			{
			case REALESTATE_TYPE_HOUSE:
				{
					// 시전 임차인으로 등록한다. 
					pclHouseManager->SetRent(villageunique, houseindex, &pclMsg->clPerson, pclcontract);
					
					SI32 id = pclMsg->usCharID;
					if(pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->clPerson.GetPersonID())
					{
						// Person이 파악한 부동산 정보를 업데이트 한다. 
						pclCM->CR[id]->pclCI->clRealEstate.Add(REALESTATE_TYPE_HOUSE, pclMsg->siVillageUnique, pclMsg->siIndex, &pclMsg->clRentContract);
						pclCM->CR[id]->SetUpdateSwitch(UPDATE_REALESTATE, true, 0);

						// 전장 정보를 업데이트 한다. 
						pclCM->CR[id]->pclCI->clBank.SetMoney(pclMsg->siBankMoney);
						pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);

						if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
						{
							// TEMP LEEKH --------------------------------------------------------------
							// [로그추가 : 황진성 2007. 10. 10] // 부동산 계약을 연장.
							// param2		 == 부동산의 종류, 
							// param3		 == 부동산의 번호.  
							// pszCharParam1 == 시간.
							TCHAR pszCharParam1[50] = {'\0', };
							ZeroMemory(pszCharParam1, sizeof(pszCharParam1));
							StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d : %d"),	
								pclMsg->clRentContract.clDate.uiYear,
								pclMsg->clRentContract.clDate.uiMonth,
								pclMsg->clRentContract.clDate.uiDay,
								pclMsg->clRentContract.clDate.uiHour,
								pclMsg->clRentContract.clDate.uiMinute );



							SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HOUSE, LOGCOMMAND_INDEX_HOUSE_EXTENDHOUSECONTRACT,
								0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, pclMsg->siBankMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								pclMsg->siRealEstateType, pclMsg->siIndex, 0, 0, 0,
								pszCharParam1, NULL);
							// TEMP LEEKH --------------------------------------------------------------
						}

						returnval = SRVAL_HOUSE_SUCCESSEXTENDCONTRACT;
					}
				}
				break;
			default:
				MsgBox(TEXT("fdsfijfd"), TEXT("fdfef:%d"), realestatetype);
				return ;
				break;
			}
		}
		break;
	case -1:	
		break;
	case -2:	
		returnval	= SRVAL_HOUSE_FAILEXTENDCONTRAC_NOMONEY; 
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