//---------------------------------
// 2004/2/6 김태곤
//---------------------------------

#include "Char/CharManager/CharManager.h"

#include "../Server/Server.h"

#include "../../DBManager/GameDBManager_World/DBMsg.h"

#include "Msg/MsgType-Village.h"

#include "../Client/ninterface/nvillagedetailinfo/NVillageDetailInfo.h"

#include "../CommonLogic/Cityhall/Cityhall.h"
#include "../CommonLogic/Hunt/Hunt.h"
#include "../CommonLogic/HorseMarket/HorseMarket.h"
#include "../CommonLogic/SummonMarket/SummonMarket.h"
#include "../CommonLogic/SummonHeroMarket/SummonHeroMarket.h"
#include "../CommonLogic/Land/Land.h"
#include "../CommonLogic/Feast/Feast.h"
#include "../CommonLogic/RealEstate/RealEstate.h"
#include "../CommonLogic/Mine/Mine.h"
#include "../CommonLogic/Guild/Guild.h"

// 마을 상세 정보 요청 . 
void cltServer::DoMsg_GAMEMSG_REQUEST_VILLAGEDETAILINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_VillageDetailInfo* pclinfo = (cltGameMsgRequest_VillageDetailInfo*)pclMsg->cData;

	// 마지막 메시지 보낸 시간을 체크한다. 
	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}


	SI32 i;
	bool bcheckswitch = false;
	cltGameMsgResponse_VillageDetailInfo clinfo;

	// 전쟁전적 얻어오기. 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_WAR)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			clinfo.siWarKillNum[i] = pclVillageManager->pclVillageInfo[i]->siWarKillNumber;
		}
	}
	// 주민 100명당 전쟁전적 얻어오기. 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_WARPERRESIDENT)
	{
		bcheckswitch = true;

		// 총전공을 얻어온다. 
		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			clinfo.siWarKillNum[i] = pclVillageManager->pclVillageInfo[i]->siWarKillNumber;
		}
	
		// 주민수를 고려하여 보정한다. 
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			cltStrInfo* pclstrinfo = pclCityHallManager->GetStrInfo(i);
			if(pclstrinfo)
			{
				SI32 peoplenum  = pclstrinfo->clCityhallStrInfo.siResidentNum;

				peoplenum = max(1, (peoplenum / 100) + min(1, (peoplenum % 100)));

				clinfo.siWarKillNum[i] = clinfo.siWarKillNum[i] / peoplenum;
			}
		}


	}
	// 주민수 얻어오기. 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_PEOPLE)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			cltStrInfo* pclstrinfo = pclCityHallManager->GetStrInfo(i);
			if(pclstrinfo)
			{
				clinfo.uiPeople[i] = pclstrinfo->clCityhallStrInfo.siResidentNum;
			}
		}

		if ( pclCM->IsValidID(id) )
		{
			pclCM->CR[id]->DoQuestCheckAndAction( QUEST_CLEAR_COND_SORTRESIDENT );
		}
	}
	// 현재 사용자수.
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_USER)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			clinfo.siUserNum[i] = pclVillageManager->pclVillageInfo[i]->siUserNum;
		}
	}
	// 전년도 배당액. 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_PREVYEARDBID)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclCityHallManager->GetStrInfo(i);
			if(pclstr)
			{
				SI32 year = max(0, pclTime->clDate.uiYear-1);
				clinfo.siBidMoney[i] = pclstr->clCityhallStrInfo.GetTotalDividendMoney(year);
			}
		}
	}
	// 수렵장 레벨 제한. 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_HUNT)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclHuntManager->GetStrInfo(i);
			if(pclstr)
			{
				
				// 수렵장 상태가 정상이어야 
				//[진성] 수렵장을 보유한 마을정보가 안나오는것 수정. 아래 조건문은 뭘까? => 2008-5-7
				if(/*pclstr->clHuntStrInfo.siHuntStatus == HUNT_ENTER_ENABLESTATUS 
				&& */pclVillageManager->IsThereStructure(i, RANKTYPE_HUNT))
				{
					clinfo.siStrData[i] = pclstr->clHuntStrInfo.siUserNum + 1;	// 강제로 1 이상으로 설정한다. 
				}				
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	// 사복시 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_HORSEMARKET)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclHorseMarketManager->GetStrInfo(i);
			if(pclstr)
			{
				// 수렵장 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_HORSEMARKET))
				{
					clinfo.siStrData[i] = pclstr->clHorseMarketStrInfo.clFee.GetFeeRate()+1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	// 여각 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_STOCK)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclStockManager->GetStrInfo(i);
			if(pclstr)
			{
				// 여각  상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_STOCK))
				{
					clinfo.siStrData[i] = pclstr->clStockStrInfo.clFee.GetFeeRate()+1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_NEWMARKET)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		cltStrInfo* pclStrInfo = NULL;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			pclStrInfo = pclNewMarketManager->GetStrInfo(i);
			if( NULL != pclStrInfo	)
			{
				// 여각  상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_NEWMARKET))
				{
					clinfo.siStrData[i] = pclStrInfo->clNewMarketStrInfo.clFee.GetFeeRate() + 1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	// 거간
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_REALESTATEMARKET)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclRealEstateMarketManager->GetStrInfo(i);
			if(pclstr)
			{
				// 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_REALESTATEMARKET))
				{
					clinfo.siStrData[i] = pclstr->clRealEstateMarketStrInfo.clFee.GetFeeRate()+1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	// 장예원
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_SUMMONMARKET)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclSummonMarketManager->GetStrInfo(i);
			if(pclstr)
			{
				// 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_SUMMONMARKET))
				{
					clinfo.siStrData[i] = pclstr->clSummonMarketStrInfo.clFee.GetFeeRate()+1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	// 비변사 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_SUMMONHEROMARKET)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclSummonHeroMarketManager->GetStrInfo(i);
			if(pclstr)
			{
				// 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_SUMMONHEROMARKET))
				{
					clinfo.siStrData[i] = pclstr->clSummonHeroMarketStrInfo.clFee.GetFeeRate()+1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	// 농장
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_FARM)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclLandManager->GetStrInfo(i);
			if(pclstr)
			{
				// 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_LAND))
				{
					clinfo.siStrData[i] = pclstr->clLandStrInfo.clRentContract.siRentFee + 1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	
	// 내자시
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_FEAST)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclFeastManager->GetStrInfo(i);
			if(pclstr)
			{
				// 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_FEAST))
				{
					clinfo.siStrData[i] = pclstr->clFeastStrInfo.clFeastInfo.siCost + 1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}


	// 광산
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_MINE)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;


			cltStrInfo* pclstr = pclMineManager->GetStrInfo(i);
			if(pclstr)
			{
				// 상태가 정상이어야 
				if(pclVillageManager->IsThereStructure(i, RANKTYPE_MINE))
				{
					clinfo.siStrData[i] = pclstr->clMineStrInfo.clRentContract.siRentFee + 1;
				}
				else
				{
					clinfo.siStrData[i] = 0;
				}
			}
		}
	}
	
	
	// 마을 전적
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_VILLAGEWARRECORD)
	{
		bcheckswitch = true;


		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			cltStrInfo* pclstr = pclCityHallManager->GetStrInfo(i);
			if(pclstr)
			{				
				clinfo.siVillageWarRecordWinAndLose[0][i] = pclstr->clCityhallStrInfo.siVillaeWarRecordWin;
				clinfo.siVillageWarRecordWinAndLose[1][i] = pclstr->clCityhallStrInfo.siVillaeWarRecordLose;
			}
		}
	}
	// 마을자금
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_VILLAGEMONEY)
	{
		bcheckswitch = true;


		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			cltStrInfo* pclstr = pclCityHallManager->GetStrInfo(i);
			if(pclstr)
			{
				clinfo.siStrData[i] = (SI32)(pclstr->clCommonInfo.clMoney.itMoney / 1000);
			}
		}
	}
	// 마을방문자수
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_VISITORNUM)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			clinfo.siStrData[i] = pclVillageManager->pclVillageInfo[i]->GetVisitNum();
		}
	}
	// 마을이익금
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_VILLAGEPROFIT)
	{
		bcheckswitch = true;


		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			cltStrInfo* pclstr = pclCityHallManager->GetStrInfo(i);
			if(pclstr)
			{
				clinfo.siStrData[i] = (GMONEY)pclstr->clCityhallStrInfo.clProfitMoney.itMoney / 1000;
			}
		}
	}
	// 마을주식수
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_VILLAGESTOCK)
	{
		bcheckswitch = true;


		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;

			cltStrInfo* pclstr = pclCityHallManager->GetStrInfo(i);
			if(pclstr)
			{
				clinfo.siStrData[i] = (SI32)pclstr->clCityhallStrInfo.siTotalStockAmount;
			}
		}
	}
	// 주주총회진행마을 
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_GENERALMEETING)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;
			clinfo.siStrData[i] = pclCityHallManager->IsGeneralMeeting(i); 
		}
	}
	
	// 관청 상태 정보( 정상, 파산, 안정 등..)
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_CITYHALLSTATE)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;
			clinfo.uiCityHallState[i] = pclCityHallManager->GetCityHallState(i);
		}
	}

	// 마을 점수
	if(pclinfo->siVillageDetailInfoType == NVILLAGEDETAILINFOTYPE_SCORE)
	{
		bcheckswitch = true;

		clinfo.siVillageDetailInfoType = pclinfo->siVillageDetailInfoType;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageManager->pclVillageInfo[i] == NULL)continue;
			clinfo.siTotalVillageScore[i] = pclVillageManager->pclVillageInfo[i]->clVillageVary.siTotalVillageScore;
		}
	}



	if(bcheckswitch)
	{
		// 클라이언트로 메시지를 보낸다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEDETAILINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}
