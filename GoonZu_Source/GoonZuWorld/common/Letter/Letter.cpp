//---------------------------------
// 2003/9/27 김태곤
//---------------------------------

#include "Letter.h"

#include "..\..\Server\Rank\Rank.h"
#include "..\..\Client\Client.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Land.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Mine.h"
#include "..\Father\Father.h"
#include "Map\FieldObject\FieldObject.h"
#include "../Client/NStructureDlg/summonmarket/NSummonMarketDlg.h"
#include "../CommonLogic/MagicMgr/MagicMgr.h"
#include "../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"

extern cltCommonLogic* pclClient;

bool cltLetterUnit::GetText( TCHAR* buffer, SI16 txtSize )
{
	buffer[0] = NULL;

	cltLetterHeader* pclheader = (cltLetterHeader*)cData;

	switch(pclheader->siType)
	{
	case LETTERTYPE_FIRE:
		{
			cltLetterMsg_Fire* pclinfo = (cltLetterMsg_Fire*)cData;

			// 신분 이름 구하기. 
			TCHAR rankname[128]=TEXT("");
			GetRankName(&pclinfo->clRank, rankname, 128, pclinfo->uiGrank);
			
			TCHAR* pText = GetTxtFromMgr(2898);
			TCHAR year[128]= TEXT(" ") ;
			TCHAR month[128]= TEXT(" ") ;
			TCHAR day[128]= TEXT(" ") ;
			
			StringCchPrintf(year,128, TEXT("%d"),pclinfo->clDate.uiYear);
			StringCchPrintf(month,128, TEXT("%d"),pclinfo->clDate.uiMonth);
			StringCchPrintf(day,128, TEXT("%d"),pclinfo->clDate.uiDay);
			Nsprintf(buffer,pText,TEXT("year"),   year,TEXT("month"),   month ,TEXT("day"),   day ,TEXT("duty"), rankname   ,NULL);
		
			//StringCchPrintf( buffer, txtSize, pText,
			//pclinfo->clDate.uiYear,
			//pclinfo->clDate.uiMonth,
			//pclinfo->clDate.uiDay,
			//rankname);

			return true;
		}
		break;
	case LETTERTYPE_BERANK:
		{
			cltLetterMsg_BeRank* pclinfo = (cltLetterMsg_BeRank*)cData;

			// 신분 이름 구하기. 
			TCHAR rankname[128]=TEXT("");
			GetRankName(&pclinfo->clRank, rankname, 128, pclinfo->uiGrank);
			
			TCHAR* pText = GetTxtFromMgr(2899);
            TCHAR year[128]= TEXT(" ") ;
			TCHAR month[128]= TEXT(" ") ;
			TCHAR day[128]= TEXT(" ") ;
			
			StringCchPrintf(year,128, TEXT("%d"),pclinfo->clDate.uiYear);
			StringCchPrintf(month,128, TEXT("%d"),pclinfo->clDate.uiMonth);
			StringCchPrintf(day,128, TEXT("%d"),pclinfo->clDate.uiDay);
			Nsprintf(buffer,pText,TEXT("year"),   year,TEXT("month"),   month ,TEXT("day"),   day ,TEXT("duty"), rankname   ,NULL);
		
			//StringCchPrintf( buffer, txtSize, pText,
			//pclinfo->clDate.uiYear,
			//pclinfo->clDate.uiMonth,
			//pclinfo->clDate.uiDay,
			//rankname);

	
			return true;
		}
		break;
		
	case LETTERTYPE_RECALLHOUSE:
		{
			cltLetterMsg_RecallHouse* pclinfo = (cltLetterMsg_RecallHouse*)cData;

			switch(pclinfo->siRecallReason)
			{
			case HOUSERECALL_REASON_FEEDELAY:
				{
					TCHAR* pText = GetTxtFromMgr(2900);
					StringCchPrintf(buffer, txtSize, pText);
				}
				break;
			case HOUSERECALL_REASON_CONTRACTEND:
				{
					TCHAR* pText = GetTxtFromMgr(2901);
					StringCchPrintf( buffer, txtSize, pText);
				}
				break;
			case HOUSERECALL_REASON_CANCELLATION:
				{
                    TCHAR* pText = GetTxtFromMgr(5838);								
					StringCchPrintf( buffer, txtSize, pText,pclinfo->siHouseIndex+1,pclinfo->szOwnerName);

					return true;
				}
				break;
		
			default:
				MsgBox(TEXT("fdv8fdfds"), TEXT("vfj8fds:%d"), pclinfo->siRecallReason);
				break;
			}

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(2902);
			TCHAR index[128]= TEXT(" ") ;
			StringCchPrintf(index, 128, TEXT("%d"),pclinfo->siHouseIndex+1);
			Nsprintf(temp,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"),   index ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siHouseIndex+1);
			
			StringCchCat(buffer, txtSize, temp);
			return true;

		}
		break;
	case LETTERTYPE_PAYRENTFEE:
		{
			cltLetterMsg_PayRentFee* pclinfo = (cltLetterMsg_PayRentFee*)cData;
			
			SI32 villageunique = pclinfo->siVillageUnique;
			TCHAR* villagename	= pclClient->pclVillageManager->GetName(villageunique);
			SI32 houseindex		= pclinfo->siHouseIndex;
			GMONEY rentfee		= pclinfo->siRentFee;
			SI32 delaynum		= pclinfo->siDelayNum;
			GMONEY delaymoney		= pclinfo->siDelayMoney;

			if(villagename)
			{
				// 연체된 경우, 
				if(delaynum)
				{
					TCHAR szMoney[128] = { '\0', };
					g_JYLibFunction.SetNumUnit( delaymoney, szMoney, sizeof(szMoney) );

					NTCHARString512 strMessage( GetTxtFromMgr(2903) );

					strMessage.Replace( TEXT("#village#"), villagename );
					strMessage.Replace( TEXT("#index#"), SI32ToString(houseindex+1) );
					strMessage.Replace( TEXT("#count#"), SI32ToString(delaynum) );
					strMessage.Replace( TEXT("#money#"), szMoney );
					
					StringCchCopy( buffer, txtSize, strMessage );
				}
				// 정상 지불된 경우, 
				else
				{
					TCHAR* pText = GetTxtFromMgr(2904);
					TCHAR index[128]= TEXT(" ") ;
					TCHAR money[128]= TEXT(" ") ;

					StringCchPrintf(index,128, TEXT("%d"),houseindex+1);
					g_JYLibFunction.SetNumUnit( rentfee, money, sizeof(money) );
					Nsprintf(buffer,pText,TEXT("village"), villagename   ,TEXT("index"),  index  ,TEXT("money"),  money  ,NULL);

					//StringCchPrintf( buffer, txtSize, pText, villagename, houseindex+1, rentfee);
				}
			}

			return true;
		}
		break;
		
	case LETTERTYPE_STOCKTRADE:
		{
			cltLetterMsg_StockTrade* pclinfo = (cltLetterMsg_StockTrade*)cData;

			SI32 villageunique = pclinfo->siVillageUnique;
			TCHAR* villagename	= pclClient->pclVillageManager->GetName(villageunique);
			
			if(pclinfo->siOrderMode == ORDERMODE_SELL)
			{

				TCHAR* pText = GetTxtFromMgr(2905);
				
				TCHAR amount[128]= TEXT(" ") ;
				TCHAR money[128]= TEXT(" ") ;
				TCHAR result[128]= TEXT(" ") ;
				TCHAR fee[128]= TEXT(" ") ;
			
				StringCchPrintf(amount,128, TEXT("%d"),pclinfo->siTradeAmount);
				g_JYLibFunction.SetNumUnit( pclinfo->siTradeMoney, money, sizeof(money) );
				StringCchPrintf(result,128, TEXT("%d"),pclinfo->siLeftAmount);
				g_JYLibFunction.SetNumUnit( pclinfo->siFee, fee, sizeof(fee) );
				Nsprintf(buffer,pText,TEXT("what"), villagename   ,TEXT("amount"), amount   ,TEXT("money"),   money ,TEXT("result"),  result  ,TEXT("fee"),  fee  ,NULL);
				//StringCchPrintf( buffer, txtSize, pText, 
				//	villagename, 
				//	pclinfo->siTradeAmount,
				//	pclinfo->siTradeMoney,
				//	pclinfo->siLeftAmount,
				//	pclinfo->siFee);
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(2906);
				TCHAR amount[128]= TEXT(" ");
				TCHAR money[128]= TEXT(" ");
				TCHAR result[128]= TEXT(" ");
				TCHAR fee[128] =TEXT(" ");
				
			
				StringCchPrintf(amount,128, TEXT("%d"),pclinfo->siTradeAmount);
				g_JYLibFunction.SetNumUnit( pclinfo->siTradeMoney, money, sizeof(money) );
				StringCchPrintf(result,128, TEXT("%d"),pclinfo->siLeftAmount);
				g_JYLibFunction.SetNumUnit( pclinfo->siFee, fee, sizeof(fee) );
				Nsprintf(buffer,pText,TEXT("what"), villagename   ,TEXT("amount"), amount   ,TEXT("money"),   money ,TEXT("result"),  result    ,NULL);
				//StringCchPrintf( buffer, txtSize, pText, 
				//	villagename, 
				//	pclinfo->siTradeAmount,
				//	pclinfo->siTradeMoney,
				//	pclinfo->siLeftAmount);
				
			}

			return true;

		}
		break;

	case LETTERTYPE_MARKETTRADE:
		{
			cltLetterMsg_MarketTrade* pclinfo = (cltLetterMsg_MarketTrade*)cData;

			SI32 itemunique			= pclinfo->siItemUnique;
			const TCHAR* itemname	= pclClient->pclItemManager->GetName(itemunique);
			GMONEY fee				= pclinfo->siFee;
			
			if(itemname)
			{
				char* pText = NULL;
				if( cltLetterMsg_MarketTrade::TYPE_MARKET == pclinfo->uiType)
					pText = GetTxtFromMgr(2907);
				else
					pText = GetTxtFromMgr(9833);

				TCHAR item[128]= TEXT(" ");
				TCHAR num[128]= TEXT(" ");
				TCHAR money[128]= TEXT(" ") ;
				TCHAR fee[128]= TEXT(" ");
				
				StringCchPrintf(num,128, TEXT("%d"),pclinfo->siItemNum) ;
				TCHAR		szPrice[64] = { '\0', } ;
				TCHAR		szFee[64] = { '\0', } ;
				GMONEY		siPrice = pclinfo->siTradeMoney ;
				GMONEY		siFee = pclinfo->siFee ;
				g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
				g_JYLibFunction.SetNumUnit(siFee, szFee, sizeof(szFee));

				StringCchPrintf(fee,128, TEXT("%I64d"),pclinfo->siFee);
				Nsprintf(buffer,pText,TEXT("item"), itemname   ,TEXT("num"),  num  ,TEXT("money"), szPrice   ,TEXT("fee"), szFee   ,NULL);
				//StringCchPrintf( buffer, txtSize, pText, 
				//	itemname, 
				//	pclinfo->siItemNum,
				//	pclinfo->siTradeMoney,
				//	pclinfo->siFee);
			}

			return true;

		}
		break;

	case LETTERTYPE_HORSEMARKETTRADE:
		{
			cltLetterMsg_HorseMarketTrade* pclinfo = (cltLetterMsg_HorseMarketTrade *)cData;
			
			TCHAR* pText = GetTxtFromMgr(2908);			
			TCHAR money[128]= TEXT(" ");
			TCHAR fee[128]= TEXT(" ");

			g_JYLibFunction.SetNumUnit(pclinfo->siTradeMoney, money, sizeof(money));
			g_JYLibFunction.SetNumUnit(pclinfo->siFee, fee, sizeof(fee));
			Nsprintf(buffer,pText,TEXT("money"),  money  ,TEXT("fee"),   fee ,NULL);

			//StringCchPrintf( buffer, txtSize, pText, 
			//		pclinfo->siTradeMoney,
			//		pclinfo->siFee);

			return true;

		}
		break;
		
	case LETTERTYPE_MARKETPRICEDOWN:
		{
			cltLetterMsg_MarketPriceDown* pclinfo = (cltLetterMsg_MarketPriceDown*)cData;

			if( cltLetterMsg_MarketPriceDown::TYPE_NEWMARKET == pclinfo->uiType )
			{
				Nsprintf(buffer, GetTxtFromMgr(9856), NULL);
			}
			else
			{
				SI32 itemunique			= pclinfo->siItemUnique;
				const TCHAR* itemname	= pclClient->pclItemManager->GetName(itemunique);
				TCHAR* pText			= NULL;

				if(itemname == NULL)return false;

				if(pclinfo->siPrice)
				{
					pText = GetTxtFromMgr(2909);
					
					TCHAR num[128]= TEXT(" ") ;
					TCHAR money[128]= TEXT(" ") ;

					StringCchPrintf(num,128, TEXT("%d"),pclinfo->siItemNum);
					g_JYLibFunction.SetNumUnit(pclinfo->siPrice, money, sizeof(money));
					Nsprintf(buffer,pText,TEXT("item"), itemname   ,TEXT("num"), num   ,TEXT("money"),  money  ,NULL);
				}
				else
				{
					pText = GetTxtFromMgr(2910);
					
					TCHAR num[128]= TEXT(" " );

					StringCchPrintf(num,128, TEXT("%d"),pclinfo->siItemNum);
					Nsprintf(buffer,pText,TEXT("item"),  itemname  ,TEXT("num"),  num  ,NULL);
				}
			}
			
			return true;
		}
		break;

	case LETTERTYPE_HOUSERECALLWARINING:
		{
			cltLetterMsg_HouseRecallWarning* pclinfo = (cltLetterMsg_HouseRecallWarning*)cData;

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(2911);
			TCHAR index[128]= TEXT(" ");
			TCHAR month[128]= TEXT(" ");
		
			StringCchPrintf(index,128, TEXT("%d"),pclinfo->siHouseIndex+1);
			StringCchPrintf(month,128, TEXT("%d"),pclinfo->siLeftMonth);
			Nsprintf(temp,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"),  index  ,TEXT("month"), month   ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siHouseIndex+1,
			//	pclinfo->siLeftMonth);
			
			StringCchCopy(buffer, txtSize, temp);
			return true;

		}
		break;
		
	case LETTERTYPE_RENTHOUSE:
		{
			cltLetterMsg_RentHouse* pclinfo = (cltLetterMsg_RentHouse*)cData;

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(2912);
			TCHAR village[128]= TEXT(" ") ;
			TCHAR index[128]= TEXT(" ") ;
		
			StringCchPrintf(index,128, TEXT("%d"),pclinfo->siHouseIndex+1);
			Nsprintf(temp,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"),  index  ,NULL);
			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siHouseIndex+1);
			
			StringCchCopy(buffer, txtSize, temp);
			return true;

		}
		break;
		
	case LETTERTYPE_GETDIVIDEND://주식배당금.
		{
			cltLetterMsg_GetDividend* pclinfo = (cltLetterMsg_GetDividend*)cData;
			
			if(pclinfo->siType == 0)
			{
				// 화폐단위 콤마 표시
				TCHAR	szPrice[64]	= { '\0', };
				g_JYLibFunction.SetNumUnit(pclinfo->siMoney, szPrice, sizeof(szPrice));

				NTCHARString128 strMessage( GetTxtFromMgr(2917) );
				strMessage.Replace( TEXT("#village#"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique) );
				strMessage.Replace( TEXT("#money#"), szPrice );

				StringCchCopy( buffer, txtSize, strMessage );
				return true;
			}
			if(pclinfo->siType == 1)
			{
				TCHAR		szPrice[64] = { '\0', };
				GMONEY		siPrice = pclinfo->siMoney ;
				g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
				NTCHARString512	kText(GetTxtFromMgr(2913));
				kText.Replace("#village#", pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique ) );
				kText.Replace("#money#",szPrice);
				StringCchCopy(buffer, txtSize, kText);

				//TCHAR* pText = GetTxtFromMgr(2913);//주민배당금 [2007.08.13 손성웅]
				//TCHAR money[128]= TEXT(" ");
				//StringCchPrintf(money, 128, TEXT("%I64d"),pclinfo->siMoney);
				//Nsprintf(temp,pText,TEXT("village"),pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique ),TEXT("money"),money    ,NULL);
				//StringCchCopy(buffer, txtSize temp);
				return true;
			}
		}
		break;
		
	case LETTERTYPE_FATHER:
		{
			cltLetterMsg_Father* pclinfo = (cltLetterMsg_Father*)cData;

			TCHAR temp[256];
			if(pclinfo->bFatherSwitch)
			{
				TCHAR* pText = GetTxtFromMgr(2914);
				StringCchPrintf(temp, 256, pText,pclinfo->szChildName, pclinfo->szChildName);
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(2915);
				StringCchPrintf(temp, 256, pText,pclinfo->szChildName);
			}

			StringCchCopy(buffer, txtSize, temp);
			return true;

		}
		break;
		
	case LETTERTYPE_CHILDLEVELUP:
		{
			cltLetterMsg_ChildLevelUp* pclinfo = (cltLetterMsg_ChildLevelUp*)cData;

			TCHAR temp[256];
			TCHAR inc[128]= TEXT(" ") ;
			TCHAR result[128]= TEXT(" ") ;
			TCHAR money[128]= TEXT(" " );
			TCHAR exp[128]= TEXT(" " );

			//KHY - 0610 - 스승제자 S 추가수정.
			if ( ConstServiceArea_Japan != pclClient->siServiceArea )
			{
				if( cltLetterMsg_ChildLevelUp::FATHERREWARD == pclinfo->uiType )
				{
					TCHAR* pText = GetTxtFromMgr(2916);

					StringCchPrintf( inc, 128, TEXT("%d"), pclinfo->siIncLevel );
					StringCchPrintf( result,128, TEXT("%d"), NEWFATHER_LEVEL );
					g_JYLibFunction.SetNumUnit( pclinfo->siMoney, money, sizeof(money) );
					StringCchPrintf( exp,128, TEXT("%d"), pclinfo->siFameExp );
					Nsprintf( temp,pText,TEXT("user"),  pclinfo->szChildName ,TEXT("inc"),  inc  ,TEXT("result"),result   ,TEXT("money"),  money  ,TEXT("exp"), exp   ,NULL );
				}
				else if( cltLetterMsg_ChildLevelUp::GRANDFATHERREWARD == pclinfo->uiType )
				{
					TCHAR* pText = GetTxtFromMgr(10017);

				//	StringCchPrintf( inc, 128, TEXT("%d"), pclinfo->siIncLevel );
				//	StringCchPrintf( result,128, TEXT("%d"), NEWFATHER_LEVEL );
					g_JYLibFunction.SetNumUnit( pclinfo->siMoney, money, sizeof(money) );
				//	StringCchPrintf( exp,128, TEXT("%d"), pclinfo->siFameExp );
					Nsprintf( temp,pText,TEXT("user"),  pclinfo->szChildName , TEXT("money"),  money  ,NULL );
				}
				else if( cltLetterMsg_ChildLevelUp::PUPILREWARD == pclinfo->uiType )
				{
					TCHAR* pText = GetTxtFromMgr(10018);

				//	StringCchPrintf( inc, 128, TEXT("%d"), pclinfo->siIncLevel );
				//	StringCchPrintf( result,128, TEXT("%d"), NEWFATHER_LEVEL );
					g_JYLibFunction.SetNumUnit( pclinfo->siMoney, money, sizeof(money) );
				//	StringCchPrintf( exp,128, TEXT("%d"), pclinfo->siFameExp );
					Nsprintf( temp,pText,TEXT("user"),  pclinfo->szChildName , TEXT("money"),  money  ,NULL );
				}
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(2916);

				StringCchPrintf(inc, 128, TEXT("%d"), pclinfo->siIncLevel);
				StringCchPrintf(result,128, TEXT("%d"),FATHER_LEVEL);
				g_JYLibFunction.SetNumUnit( pclinfo->siMoney, money, sizeof(money) );
				StringCchPrintf(exp,128, TEXT("%d"), pclinfo->siFameExp);
				Nsprintf(temp,pText,TEXT("user"),  pclinfo->szChildName ,TEXT("inc"),  inc  ,TEXT("result"),result   ,TEXT("money"),  money  ,TEXT("exp"), exp   ,NULL);
			}
		
			StringCchCopy(buffer, txtSize, temp);
			return true;

		}
		break;
		
	case LETTERTYPE_GETSALARY://행수/대행수 급여지급
		{
			cltLetterMsg_GetSalary* pclinfo = (cltLetterMsg_GetSalary*)cData;

			TCHAR temp[256];

			TCHAR szname[128];
			if(GetRankName(&pclinfo->clRank, szname, 128) == TRUE)
			{
				TCHAR* pvillagename = pclClient->pclVillageManager->GetName(pclinfo->clRank.GetVillageUnique());
				if(pvillagename)
				{
					
					if(pclinfo->clRank.GetRankType() == RANKTYPE_CITYHALL)
					{
						TCHAR* pText = GetTxtFromMgr(7134);//영주봉급이 주주배당금으로 되어있어서 변경.[2007.08.13 손성웅]
						TCHAR money[128]= TEXT(" ");
						g_JYLibFunction.SetNumUnit( pclinfo->siMoney, money, sizeof(money) );
						/*StringCchPrintf(temp, 256, pText,pclinfo->siMoney);*/
						Nsprintf(temp,pText,TEXT("village"),  pvillagename  ,TEXT("money"),   money ,NULL);
						StringCchCopy(buffer, txtSize, temp);
						return true;
						
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2918);
						TCHAR money[128]= TEXT(" ");
						g_JYLibFunction.SetNumUnit( pclinfo->siMoney, money, sizeof(money) );
						Nsprintf(temp,pText,TEXT("village"),  pvillagename  ,TEXT("who"),   szname ,TEXT("money"),   money ,NULL);
						//sprintf(temp, pText,
							//pvillagename, szname, pclinfo->siMoney);
						
						StringCchCopy(buffer, txtSize, temp);
						return true;
					}
				}

			}
		}
		break;
	case LETTERTYPE_MAIL:
		{
			cltLetterMsg_Mail* pclinfo = (cltLetterMsg_Mail*)cData;

			TCHAR* pText = GetTxtFromMgr(2919);
			StringCchPrintf( buffer, txtSize, pText, pclinfo->szFromPersonName);
			return true;

		}	
		break;	
	case LETTERTYPE_FAMILY_DEL_FAMILY:	// 가족에서 넌 짤렸따.
		{
			cltLetterMsg_DelLetter* pclinfo = (cltLetterMsg_DelLetter*)cData;

			TCHAR *pText = GetTxtFromMgr(30132);
			StringCchPrintf(buffer, txtSize , pText);
			return true;

		}	
		break;	
	case LETTERTYPE_RECOMMENDER:
		{
			cltLetterMsg_Recommender* pclinfo = (cltLetterMsg_Recommender*)cData;

			TCHAR* pText = GetTxtFromMgr(2920);
			StringCchPrintf( buffer, txtSize, pText, pclinfo->szRecommenderName );
			return true;
		}
		break;
	case LETTERTYPE_RECOMMENDERLEVELUP:
		{
			cltLetterMsg_RecommenderLevelUp* pclinfo = (cltLetterMsg_RecommenderLevelUp*)cData;			

			const TCHAR* pitemname = pclClient->pclItemManager->GetName(pclinfo->siItemUnique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(2921);
			
				TCHAR level[128]= TEXT(" ") ;
			
				TCHAR num[128]= TEXT(" ");
				TCHAR exp[128]= TEXT(" ");
				
				StringCchPrintf(level,128, TEXT("%d"), pclinfo->siIncLevel);
			
				StringCchPrintf(num,128, TEXT("%d"),pclinfo->siItemNum);
				StringCchPrintf(exp,128, TEXT("%d"),pclinfo->siFameExp);
				Nsprintf(buffer,pText,TEXT("user"),  pclinfo->szName  ,TEXT("level"),   level ,TEXT("item"), pitemname   ,TEXT("num"),   num ,TEXT("exp"), exp   ,NULL);

			//StringCchPrintf( buffer, txtSize, pText, 
			//	pclinfo->szName, pclinfo->siIncLevel, pitemname, pclinfo->siItemNum, pclinfo->siFameExp);
			}
			return true;
		}
		break;
		
	case LETTERTYPE_STOCKLOSS:
		{
			cltLetterMsg_StockLoss* pclinfo = (cltLetterMsg_StockLoss*)cData;
			TCHAR* pText = GetTxtFromMgr(2922);
			
			TCHAR loss[128]= TEXT(" ") ;
			TCHAR left[128]= TEXT(" ");
		
			StringCchPrintf(loss, 128, TEXT("%d"),pclinfo->siLossPersent);
			StringCchPrintf(left, 128, TEXT("%d"),pclinfo->siStockAmount);
			Nsprintf(buffer,pText,TEXT("where"),pclClient->pclVillageManager->GetName( pclinfo->siWinVillage )   ,TEXT("whose"),
				pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique ),
				TEXT("loss"),	loss	,TEXT("left"), left   ,NULL);

			//StringCchPrintf( buffer, txtSize, pText,
			//	pclClient->pclVillageManager->GetName( pclinfo->siWinVillage ), 
			//	pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique ), 
			//	pclinfo->siLossPersent, pclinfo->siStockAmount );
			return true;
		}
		break;
		
	case LETTERTYPE_KINGPAY:
		{
			cltLetterMsg_GetKingPay* pclinfo = (cltLetterMsg_GetKingPay*)cData;
			TCHAR* pText = GetTxtFromMgr(2923);

			
			
			TCHAR rate[128]= TEXT(" ");
			TCHAR money[128]= TEXT(" ");			
			StringCchPrintf(rate, 128, TEXT("%d"),pclinfo->siPayPersent);
			g_JYLibFunction.SetNumUnit( pclinfo->siKingPay, money, sizeof(money) );
			Nsprintf(buffer,pText,TEXT("village"), pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique )   ,
				TEXT("rate"),   rate ,TEXT("money"), money   ,NULL);
			//StringCchPrintf( buffer, txtSize, pText,
			//	pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique ),
			//	pclinfo->siPayPersent,
			//	pclinfo->siKingPay );
			return true;
		}
		break;
		
	case LETTERTYPE_RESIDENTSHARE://마을 주민 배당금.
		{
			TCHAR buf[ 256 ];

			cltLetterMsg_ResidentShare* pclinfo = (cltLetterMsg_ResidentShare *)cData;

			switch( pclinfo->siShareType ) {
			default:
				TCHAR* pText = GetTxtFromMgr(1214);
				StringCchCopy( buf, 256, pText );
				break;
			}

			TCHAR* pText = GetTxtFromMgr(2924);
			TCHAR money[128]= TEXT(" ");
			g_JYLibFunction.SetNumUnit( pclinfo->siShareMoney, money, sizeof(money) );
			Nsprintf(buffer,pText,TEXT("village"),pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique ),TEXT("whatkind"),  buf  ,TEXT("money"),money    ,NULL);
			return true;
		}
		break;
		
	case LETTERTYPE_HORSEMARKETPRICEDOWN:
		{
			cltLetterMsg_HorseMarketPriceDown* pclinfo = (cltLetterMsg_HorseMarketPriceDown *)cData;

			if( pclinfo->siPrice > 0 ) {
				TCHAR* pText = GetTxtFromMgr(2925);
			
				TCHAR money[128]= TEXT(" ") ;
				g_JYLibFunction.SetNumUnit( pclinfo->siPrice, money, sizeof(money) );
				Nsprintf(buffer,pText,TEXT("horse"), pclinfo->szHorseName   ,TEXT("money"), money   ,NULL);
				//sprintf( buffer, pText, pclinfo->szHorseName, pclinfo->siPrice );

			} else {
				TCHAR* pText = GetTxtFromMgr(2926);
				StringCchPrintf( buffer, txtSize, pText, pclinfo->szHorseName );
			}
			

			return true;			
		}
		break;
	
	case LETTERTYPE_STRUCTUREREPAIRITEM:
		{
			cltLetterMsg_StructureRepairItem *pclinfo = (cltLetterMsg_StructureRepairItem *)cData;

			const TCHAR* itemname	= pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
			TCHAR* villname = pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique );
			TCHAR strname[ 32 ];
			TCHAR num[128]= TEXT(" ");
			StringCchPrintf(num, 128, TEXT("%d"), pclinfo->siItemNum );
			pclClient->pclVillageStructureSet->GetStructureNameFromRankType( pclinfo->siStrType, strname, 32);

			TCHAR* pText = GetTxtFromMgr(2929);
			Nsprintf(buffer,pText,TEXT("village"),   villname ,TEXT("struct"),   strname ,TEXT("item"),  itemname  ,TEXT("num"), num   ,NULL);

			//sprintf( buffer, pText,
			//	villname, strname, itemname, pclinfo->siItemNum );

			return true;

		}
		break;


	case LETTERTYPE_REALESTATEMARKETTRADE:
		{
			cltLetterMsg_RealEstateMarketTrade *pclinfo = (cltLetterMsg_RealEstateMarketTrade *)cData;

			TCHAR *pVillageName = pclClient->pclVillageManager->GetName( pclinfo->siHouseVillage );
			TCHAR s2[128]= TEXT(" ");
			TCHAR money[128]= TEXT(" ") ;
			TCHAR fee[128]= TEXT(" ") ;	
			TCHAR* pText = GetTxtFromMgr(2930);
			StringCchPrintf(s2, 128, TEXT("%d"),pclinfo->siHouseIndex + 1);
			g_JYLibFunction.SetNumUnit( pclinfo->siTradeMoney, money, sizeof(money) );
			g_JYLibFunction.SetNumUnit( pclinfo->siFee, fee, sizeof(fee) );
			Nsprintf(buffer,pText,TEXT("s1"),  pVillageName  ,TEXT("s2"),   s2, TEXT("money"), money   ,TEXT("fee"), fee   ,NULL);

			
			//sprintf( buffer, pText,
			//	pVillageName, pclinfo->siHouseIndex + 1, pclinfo->siTradeMoney, pclinfo->siFee );

			return true;

		}
		break;

	case LETTERTYPE_REALESTATEMARKETPRICEDOWN:
		{
			cltLetterMsg_RealEstateMarketPriceDown *pclinfo = (cltLetterMsg_RealEstateMarketPriceDown *)cData;

			TCHAR *pVillageName = pclClient->pclVillageManager->GetName( pclinfo->siHouseVillage );

			if( pVillageName == NULL ) break;

			if( pclinfo->siPrice > 0 ) {
				TCHAR* pText = GetTxtFromMgr(2931);
			
				TCHAR s2[128]= TEXT(" ") ;
				TCHAR money[128]= TEXT(" ") ;
			
				StringCchPrintf(s2, 128, TEXT("%d"),pclinfo->siHouseIndex + 1);
				g_JYLibFunction.SetNumUnit( pclinfo->siPrice, money, sizeof(money) );
				Nsprintf(buffer,pText,TEXT("s1"), pVillageName  ,TEXT("s2"),   s2, TEXT("money"),  money  ,NULL);

				//sprintf( buffer, pText, 
				//	pVillageName, pclinfo->siHouseIndex + 1, pclinfo->siPrice );

			} else {
				TCHAR* pText = GetTxtFromMgr(2932);
			
				TCHAR s2[128]= TEXT(" ") ;
				
				StringCchPrintf(s2, 128, TEXT("%d"),pclinfo->siHouseIndex + 1);
				Nsprintf(buffer,pText,TEXT("s1"), pVillageName   ,TEXT("s2"),  s2  ,NULL);


				//sprintf( buffer, pText, 
				//	pVillageName, pclinfo->siHouseIndex + 1 );
			}

			return true;
		}
		break;

		
	case LETTERTYPE_SUMMONMARKETTRADE:
		{
			cltLetterMsg_SummonMarketTrade *pclinfo = (cltLetterMsg_SummonMarketTrade *)cData;

			TCHAR *pName = (TCHAR *)pclClient->pclKindInfoSet->pclKI[ pclinfo->siKind ]->GetName();

			if( pName == NULL ) break;

			TCHAR* pText = GetTxtFromMgr(2933);
			TCHAR Nbuffer[512]=TEXT(" ");
		
			TCHAR money[128]= TEXT(" ");
			TCHAR fee[128]= TEXT(" ") ;
		
			g_JYLibFunction.SetNumUnit( pclinfo->siTradeMoney, money, sizeof(money) );
			g_JYLibFunction.SetNumUnit( pclinfo->siFee, fee, sizeof(fee) );
			Nsprintf(buffer,pText,TEXT("s1"),  pName  ,TEXT("s2"),  pclinfo->szName ,TEXT("money"),    money, TEXT("fee"),  fee  ,NULL);

			cltClient *pclclient = (cltClient*)pclClient;
			if(pclclient->m_pDialog[ NSUMMONMARKET_DLG ])
			{
				CControllerMgr* pChild = ((NSummonMarketDlg*)(pclclient->m_pDialog[ NSUMMONMARKET_DLG ]))->GetChildDialog(0);
				//((NSummonMarketUserDlg*)pChild)->ShowMyOrderList();
				((NSummonMarketUserDlg*)pChild)->RequestMyOrder();
			}

			//sprintf( buffer, pText,
			//	pName, pclinfo->szName, pclinfo->siTradeMoney, pclinfo->siFee );

			return true;

		}
		break;

	case LETTERTYPE_SUMMONMARKETPRICEDOWN:
		{
			cltLetterMsg_SummonMarketPriceDown *pclinfo = (cltLetterMsg_SummonMarketPriceDown *)cData;

			TCHAR *pName = (TCHAR *)pclClient->pclKindInfoSet->pclKI[ pclinfo->siKind ]->GetName();

			if( pName == NULL ) break;

			if( pclinfo->siPrice > 0 ) {
				TCHAR* pText = GetTxtFromMgr(2934);
				TCHAR money[128]= TEXT(" ");
				g_JYLibFunction.SetNumUnit( pclinfo->siPrice, money, sizeof(money) );
				Nsprintf(buffer,pText,TEXT("s1"), pName   ,TEXT("s2"), pclinfo->szName   ,TEXT("money"),  money  ,NULL);

			//	sprintf( buffer, pText, 
			//		pName, pclinfo->szName, pclinfo->siPrice );

			} else {
				TCHAR* pText = GetTxtFromMgr(2935);
				StringCchPrintf( buffer, txtSize, pText, 
					pName, pclinfo->szName );
			}

			return true;
		}
		break;

	case LETTERTYPE_SUMMONHEROMARKETTRADE:
		{
			cltLetterMsg_SummonHeroMarketTrade *pclinfo = (cltLetterMsg_SummonHeroMarketTrade *)cData;

			TCHAR *pName = (TCHAR *)pclClient->pclKindInfoSet->pclKI[ pclinfo->siKind ]->GetName();

			if( pName == NULL ) break;

			TCHAR* pText = GetTxtFromMgr(2936);
			TCHAR money[128]= TEXT(" ") ;
			TCHAR fee[128]= TEXT(" ");	
			g_JYLibFunction.SetNumUnit( pclinfo->siTradeMoney, money, sizeof(money) );
			g_JYLibFunction.SetNumUnit( pclinfo->siFee, fee, sizeof(fee) );
			Nsprintf(buffer,pText,TEXT("s1"),  pName  ,TEXT("s2"),  pclinfo->szName  ,TEXT("money"), money   ,TEXT("fee"), fee   ,NULL);


			//sprintf( buffer, pText,
			//	pName, pclinfo->szName, pclinfo->siTradeMoney, pclinfo->siFee );

			return true;

		}
		break;

	case LETTERTYPE_SUMMONHEROMARKETPRICEDOWN:
		{
			cltLetterMsg_SummonHeroMarketPriceDown *pclinfo = (cltLetterMsg_SummonHeroMarketPriceDown *)cData;

			TCHAR *pName = (TCHAR *)pclClient->pclKindInfoSet->pclKI[ pclinfo->siKind ]->GetName();

			if( pName == NULL ) break;

			if( pclinfo->siPrice > 0 ) {
				TCHAR* pText = GetTxtFromMgr(2937);
			
				TCHAR money[128]= TEXT(" ") ;
			
				g_JYLibFunction.SetNumUnit( pclinfo->siPrice, money, sizeof(money) );
				Nsprintf(buffer,pText,TEXT("s1"),pName    ,TEXT("s2"), pclinfo->szName   ,TEXT("money"), money   ,NULL);

				//sprintf( buffer, pText, 
				//	pName, pclinfo->szName, pclinfo->siPrice );

			} else {
				TCHAR* pText = GetTxtFromMgr(2938);
				StringCchPrintf( buffer, txtSize, pText, 
					pName, pclinfo->szName );
			}

			return true;
		}
		break;


	case LETTERTYPE_RESIDENTOUTDATE://주민 자동탈퇴 1년동안 접속안한 유저.
		{
			cltLetterMsg_ResidentOutDate *pclinfo = (cltLetterMsg_ResidentOutDate *)cData;

			TCHAR *pVillageName = pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique );

			TCHAR* pText = GetTxtFromMgr(2939);
			StringCchPrintf( buffer, txtSize, pText, pVillageName );

			return true;
		}
		break;


	case LETTERTYPE_RESULTBIDSTOCK:
		{
			cltLetterMsg_ResultBidStock *pclinfo = (cltLetterMsg_ResultBidStock *)cData;

			TCHAR szMoney[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->clBidMoney.itMoney, szMoney, sizeof(szMoney) );

			TCHAR *pVillageName = pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique );

			if( pclinfo->bSuccessfulBid ) 
			{
				NTCHARString512 strMessage( GetTxtFromMgr(2940) );

				strMessage.Replace( TEXT("#village#"), pVillageName );
				strMessage.Replace( TEXT("#amount#"), SI32ToString(pclinfo->siStockAmount) );
				strMessage.Replace( TEXT("#money#"), szMoney );

				StringCchCopy( buffer, txtSize, strMessage );
			} 
			else 
			{

				if( pclinfo->bExceedLimit ) 
				{
					NTCHARString512 strMessage( GetTxtFromMgr(2941) );

					strMessage.Replace( TEXT("#village#"), pVillageName );
					strMessage.Replace( TEXT("#money#"), szMoney );

					StringCchCopy( buffer, txtSize, strMessage );
				} 
				else 
				{
					NTCHARString512 strMessage( GetTxtFromMgr(2942) );

					strMessage.Replace( TEXT("#village#"), pVillageName );
					strMessage.Replace( TEXT("#money#"), szMoney );

					StringCchCopy( buffer, txtSize, strMessage );
				}

			}
			

			return true;
		}
		break;

	case LETTERTYPE_RENTLAND:
		{
			cltLetterMsg_RentLand* pclinfo = (cltLetterMsg_RentLand*)cData;

			TCHAR* pText = GetTxtFromMgr(2946);
            
			TCHAR index[128]= TEXT(" ") ;
		
			StringCchPrintf(index, 128, TEXT("%d"),pclinfo->siLandIndex+1);
			Nsprintf(buffer,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"), index   ,NULL);
			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siLandIndex+1);
			
			//StringCchCopy(buffer, txtSize, temp);
			return true;

		}
		break;

	case LETTERTYPE_PAYLANDRENTFEE:
		{
			cltLetterMsg_PayLandRentFee* pclinfo = (cltLetterMsg_PayLandRentFee*)cData;
			
			SI32 villageunique = pclinfo->siVillageUnique;
			TCHAR* villagename	= pclClient->pclVillageManager->GetName(villageunique);
			SI32 landindex		= pclinfo->siLandIndex;
			GMONEY rentfee		= pclinfo->siRentFee;
			SI32 delaynum		= pclinfo->siDelayNum;
			GMONEY delaymoney		= pclinfo->siDelayMoney;

			if(villagename)
			{
				// 연체된 경우, 
				if(delaynum)
				{
					TCHAR* pText = GetTxtFromMgr(2947);
					
					TCHAR index[128]= TEXT(" ");
					TCHAR count[128]= TEXT(" ") ;
					TCHAR money[128]= TEXT(" ") ;
				
					StringCchPrintf(index, 128, TEXT("%d"),landindex+1);
					StringCchPrintf(count, 128, TEXT("%d"),delaynum);
					g_JYLibFunction.SetNumUnit( delaymoney, money, sizeof(money) );
					Nsprintf(buffer,pText,TEXT("village"), villagename   ,TEXT("index"),index    ,TEXT("count"), count  ,TEXT("money"), money   ,NULL);

					//StringCchPrintf( buffer, txtSize, pText, villagename, landindex+1, delaynum, delaymoney);
				}
				// 정상 지불된 경우, 
				else
				{
					TCHAR* pText = GetTxtFromMgr(2948);
					TCHAR index[128]= TEXT(" ") ;
				
					TCHAR money[128]= TEXT(" ") ;
				
					StringCchPrintf(index, 128, TEXT("%d"),landindex+1);
					
					g_JYLibFunction.SetNumUnit( rentfee, money, sizeof(money) );
					Nsprintf(buffer,pText,TEXT("village"), villagename   ,TEXT("index"),index    ,TEXT("money"), money   ,NULL);
					//StringCchPrintf( buffer, txtSize, pText, villagename, landindex+1, rentfee);
				}
			}

			return true;
		}
		break;

	case LETTERTYPE_LANDRECALL:
		{
			cltLetterMsg_LandRecall* pclinfo = (cltLetterMsg_LandRecall*)cData;

			switch(pclinfo->siRecallReason)
			{
			case LANDRECALL_REASON_FEEDELAY:
				{
					TCHAR* pText = GetTxtFromMgr(2900);
					StringCchPrintf( buffer, txtSize, pText);
				}
				break;

			case LANDRECALL_REASON_CONTRACTEND:
				{
					TCHAR* pText = GetTxtFromMgr(2901);
					StringCchPrintf( buffer, txtSize, pText);
				}
				break;
			default:
				MsgBox(TEXT("fdv8fdfds"), TEXT("vfj8fds:%d"), pclinfo->siRecallReason);
				break;
			}

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(2949);
			TCHAR village[128]= TEXT(" ") ;
			TCHAR index[128]= TEXT(" ") ;

			StringCchPrintf(index, 128, TEXT("%d"),pclinfo->siLandIndex+1);
			Nsprintf(temp,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"), index   ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siLandIndex+1);
			
			StringCchCat(buffer, txtSize, temp);
			return true;
		}
		break;

	case LETTERTYPE_LANDRECALLWARNING:
		{
			cltLetterMsg_LandRecallWarning* pclinfo = (cltLetterMsg_LandRecallWarning*)cData;

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(2950);
		
			TCHAR index[128]= TEXT(" ") ;
			TCHAR month[128]= TEXT(" ") ;

			StringCchPrintf(index, 128, TEXT("%d"),	pclinfo->siLandIndex+1);
			StringCchPrintf(month, 128, TEXT("%d"),pclinfo->siLeftMonth);
			Nsprintf(temp,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"), index  ,TEXT("month"),month    ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siLandIndex+1,
			//	pclinfo->siLeftMonth);
			
			StringCchCopy(buffer, txtSize, temp);
			return true;
		}
		break;
	case LETTERTYPE_GENERALMEETING:
		{
			cltLetterMsg_GeneralMeeting* pclinfo = (cltLetterMsg_GeneralMeeting*)cData;

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(2951);
			StringCchPrintf(temp, 256, pText, 
				pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique));
			
			StringCchCat(buffer, txtSize, temp);
			return true;

		}
		break;
		
	case LETTERTYPE_HOZOANGEL:
		{
			cltLetterMsg_HozoAngel* pclinfo = (cltLetterMsg_HozoAngel*)cData;

			switch(pclinfo->siType)
			{
			case 1:
				{
					TCHAR* pText = GetTxtFromMgr(2952);
					StringCchPrintf( buffer, txtSize, pText,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			case 2:
				{
					TCHAR* pText = GetTxtFromMgr(2953);
					StringCchPrintf( buffer, txtSize, pText,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			case 3:
				{
					TCHAR* pText = GetTxtFromMgr(2954);
					StringCchPrintf( buffer, txtSize, pText,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			default:
				{
					return false ;
				}
				break;
			}

			return true ;
		}
		break;

	case LETTERTYPE_POSTOFFICE_SENDOK:
		{
			cltLetterMsg_PostOfficeSendOk* pclinfo = (cltLetterMsg_PostOfficeSendOk*)cData;

			switch( pclinfo->siType ) {
			case 1: // item
				{

				}
				break;

			case 3: // money
				{

				}
				break;
			default:
				return false;
			}

			return true;
		}
		break;

	case LETTERTYPE_RENTMINE:
		{
			cltLetterMsg_RentMine* pclinfo = (cltLetterMsg_RentMine*)cData;

			TCHAR temp[256];

			TCHAR* pText = GetTxtFromMgr(3251);
			TCHAR index[128]= TEXT(" ");

			StringCchPrintf(index, 128, TEXT("%d"),pclinfo->siMineIndex+1);
			Nsprintf(temp,pText,TEXT("village"),  pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)  ,TEXT("index"), index   ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siMineIndex+1);
			
			StringCchCopy(buffer, txtSize, temp);
			return true;

		}
		break;

	case LETTERTYPE_PAYMINERENTFEE:
		{
			cltLetterMsg_PayMineRentFee* pclinfo = (cltLetterMsg_PayMineRentFee*)cData;
			
			SI32 villageunique = pclinfo->siVillageUnique;
			TCHAR* villagename	= pclClient->pclVillageManager->GetName(villageunique);
			SI32 landindex		= pclinfo->siMineIndex;
			GMONEY rentfee		= pclinfo->siRentFee;
			SI32 delaynum		= pclinfo->siDelayNum;
			GMONEY delaymoney		= pclinfo->siDelayMoney;

			if(villagename)
			{
				// 연체된 경우, 
				if(delaynum)
				{
					TCHAR* pText = GetTxtFromMgr(3252);
					TCHAR index[128]= TEXT(" ") ;
					TCHAR count[128]= TEXT(" ") ;
					TCHAR money[128]= TEXT(" ") ;

					StringCchPrintf(index, 128, TEXT("%d"),landindex+1);
					StringCchPrintf(count, 128, TEXT("%d"),delaynum);
					g_JYLibFunction.SetNumUnit( delaymoney, money, sizeof(money) );
					Nsprintf(buffer,pText,TEXT("village"),  villagename  ,TEXT("index"),  index  ,TEXT("count"),  count ,TEXT("money"),  money  ,NULL);

					//StringCchPrintf( buffer, txtSize, pText, villagename, landindex+1, delaynum, delaymoney);
				}
				// 정상 지불된 경우, 
				else
				{
					TCHAR* pText = GetTxtFromMgr(3253);
						TCHAR index[128]= TEXT(" ") ;
				
					TCHAR money[128]= TEXT(" ") ;

					StringCchPrintf(index, 128, TEXT("%d"),landindex+1);				
					g_JYLibFunction.SetNumUnit( rentfee, money, sizeof(money) );
					Nsprintf(buffer,pText,TEXT("village"),  villagename  ,TEXT("index"),  index  ,TEXT("money"),  money  ,NULL);

					//StringCchPrintf( buffer, txtSize, pText, villagename, landindex+1, rentfee);
				}
			}

			return true;
		}
		break;

	case LETTERTYPE_MINERECALL:
		{
			cltLetterMsg_MineRecall* pclinfo = (cltLetterMsg_MineRecall*)cData;

			switch(pclinfo->siRecallReason)
			{
			case MINERECALL_REASON_FEEDELAY:
				{
					TCHAR* pText = GetTxtFromMgr(2900);
					StringCchPrintf( buffer, txtSize, pText);
				}
				break;

			case MINERECALL_REASON_CONTRACTEND:
				{
					TCHAR* pText = GetTxtFromMgr(2901);
					StringCchPrintf( buffer, txtSize, pText);
				}
				break;
			default:
				MsgBox(TEXT("fdv8fdfds"), TEXT("vfj8fds:%d"), pclinfo->siRecallReason);
				break;
			}

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(3254);
			
			TCHAR index[128]= TEXT(" ") ;
			
			StringCchPrintf(index, 128, TEXT("%d"),pclinfo->siMineIndex+1);
			Nsprintf(temp,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"), index   ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siMineIndex+1);
			
			StringCchCat(buffer, txtSize, temp);
			return true;
		}
		break;

	case LETTERTYPE_MINERECALLWARNING:
		{
			cltLetterMsg_MineRecallWarning* pclinfo = (cltLetterMsg_MineRecallWarning*)cData;

			TCHAR temp[256];
			TCHAR* pText = GetTxtFromMgr(3255);
			TCHAR index[128]= TEXT(" ");
			TCHAR month[128]= TEXT(" ") ;

			StringCchPrintf(index, 128, TEXT("%d"),pclinfo->siMineIndex+1);
			StringCchPrintf(month, 128, TEXT("%d"),	pclinfo->siLeftMonth);
			Nsprintf(buffer,pText,TEXT("village"), pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique)   ,TEXT("index"),  index  ,TEXT("month"),  month  ,NULL);

			//sprintf(temp, pText,
			//	pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique),
			//	pclinfo->siMineIndex+1,
			//	pclinfo->siLeftMonth);
			
			StringCchCopy(buffer, txtSize, temp);
			return true;
		}
		break;
//#if defined(_CITYHALL_BANKRUPTCY)
	case LETTERTYPE_VILLAGENAME_AND_DATE:	
		{
			cltLetterMsg_VillageNameAndDate* pclinfo = (cltLetterMsg_VillageNameAndDate*)cData;

			TCHAR* pText = NULL;
			TCHAR* pText2 = NULL;
			TCHAR* villagename = NULL;	
			TCHAR buffer2[128];
			memset(buffer2, 0, sizeof(buffer2));

			villagename = pclClient->pclVillageManager->GetName(pclinfo->siVillageUnique);

			switch(pclinfo->usType )
			{
			case cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTCY_ALERT:// 마을 파산 예정 경고 쪽지
				{
					pText = GetTxtFromMgr(5152);
					StringCchPrintf( buffer, txtSize, pText, villagename,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			case cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTED:	// 마을 파산 알림 쪽지
				{
					pText = GetTxtFromMgr(5153);
					StringCchPrintf( buffer, txtSize, pText, villagename,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			case cltLetterMsg_VillageNameAndDate::LETTERTYPE_RETURNSTOCKBIDVILLAGE:	// 마을 파산 후 증자 보상 알림 쪽지
				{
					pText = GetTxtFromMgr(5153);
					pText2 = GetTxtFromMgr(5181);
					StringCchPrintf(buffer2, 128, pText, villagename,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
					StringCchPrintf( buffer, txtSize, TEXT("%s\r\n%s"), buffer2, pText2 );
				}
				break;
			default:
				MsgBox(TEXT("fdv8fdfds2"), TEXT("vfj8fds2:%d"), pclinfo->usType);
				break;
			}
			return true;
		}
		break;
	case LETTERTYPE_HYUNGZOPOLICE:
		{
			cltLetterMsg_HyungzoPolice* pclinfo = (cltLetterMsg_HyungzoPolice*)cData;

			switch(pclinfo->siType)
			{
			case 1:
				{
					TCHAR* pText = GetTxtFromMgr(5527);
					StringCchPrintf( buffer, txtSize, pText,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			case 2:
				{
					TCHAR* pText = GetTxtFromMgr(5528);
					StringCchPrintf( buffer, txtSize, pText,
									pclinfo->clDate.uiYear,
									pclinfo->clDate.uiMonth,
									pclinfo->clDate.uiDay);
				}
				break;
			}
			return true;
		}
		break;

	case LETTERTYPE_EXISTENT_SAVEUSERITEM:
		{
			TCHAR* pText = GetTxtFromMgr(5832);		//TEXT("현재 시전 연장을 하지 못해서 사라진 아이템을 봉이김선달이 보관중입니다. 봉이김선달을 찾아가서 아이템을 찾으시길 바랍니다");//GetTxtFromMgr(5527);
			StringCchCopy(buffer, txtSize, pText);
			return true;
		}
		break;
	case LETTERTYPE_DORMANCY_WARING_EXPIRATION:
		{
			cltLetterMsg_DormancyWaringExpiration* pclinfo = (cltLetterMsg_DormancyWaringExpiration*)cData;

			TCHAR szDate[128] = { '\0', };
			
			g_JYLibFunction.SetNumUnit( pclinfo->m_siDate, szDate, sizeof(szDate) );

			NTCHARString128 strMessage(  GetTxtFromMgr(40144) );
			strMessage.Replace( TEXT("#date#"), szDate );
			
			StringCchCopy(buffer, txtSize, strMessage);
			return true;
		}
		break;
	case LETTERTYPE_SKILLBOOK_MAGIC_DEL:
		{
			cltLetterMsg_SkillBookMagicDel* pclinfo = (cltLetterMsg_SkillBookMagicDel*)cData;
			
			CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
			if( pMagicAttributePtr )
			{
				NTCHARString128 strMessage( GetTxtFromMgr(40354) );
				strMessage.Replace( "#magicname#", pMagicAttributePtr->GetMagicAttribute( pclinfo->m_siMagicKind )->strMagicName );
				StringCchCopy( buffer, txtSize, strMessage );
				return true;
			}
		}
		break;
	case LETTERTYPE_WORLDMONEYTRADERESULT:
		{
			cltLetterMsg_WorldMoneyTradeResult* pclinfo = (cltLetterMsg_WorldMoneyTradeResult*)cData;


			TCHAR szAmount[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siWorldMoney, szAmount, sizeof(szAmount) );

			TCHAR szMoney[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siWorldMoneyPrice, szMoney, sizeof(szMoney) );

			NTCHARString512 strMessage( GetTxtFromMgr(6254) );

			strMessage.Replace( TEXT("#amount#"), szAmount );
			strMessage.Replace( TEXT("#money#"), szMoney );


			StringCchCopy( buffer, txtSize, strMessage );

			return true;
		}
		break;

	case LETTERTYPE_STOCKOFFERING_RESULT:
		{
			cltLetterMsg_StockOfferingResult* pclinfo = (cltLetterMsg_StockOfferingResult*)cData;

			TCHAR *village= pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique );

			if( pclinfo->siType == 1 )
			{
				// 공모 당첨 되었으면,
				TCHAR* pText = GetTxtFromMgr(6011);
				sprintf( buffer, pText, village );

			} else {
				TCHAR* pText = GetTxtFromMgr(6012);
				sprintf( buffer, pText, village );
			}
			
			return true;

		}
		break;
	case LETTERTYPE_CHANGERECOMMENDPOINT:
		{
			cltLetterMsg_ChangeRecommendPoint* pclinfo = (cltLetterMsg_ChangeRecommendPoint*)cData;

			//TCHAR* pText = GetTxtFromMgr();
			 

			TCHAR* pText = GetTxtFromMgr(6385);
			StringCchPrintf(buffer ,txtSize,pText, pclinfo->szCharName, pclinfo->siLevel , pclinfo->siChangedRecommendPoint, pclinfo->siRecommentPoint); 

			return true;
		}
		break;
	case LETTERTYPE_LEVELUPRECOMMANDREWARD:
		{
			cltLetterMsg_LevelUpREcommandReward* pclinfo = (cltLetterMsg_LevelUpREcommandReward*)cData;
			
			TCHAR * pText = GetTxtFromMgr(6253 );
			
			const TCHAR* itemname =	pclClient->pclItemManager->GetName( pclinfo->siRewardItemUnique );
			
			StringCchPrintf(buffer ,txtSize,pText, pclinfo->siLevel , itemname, pclinfo->siRewardItemNum ); 
			
			return true;
		}
		break;

	case LETTERTYPE_TRADESERVER_SELL:
		{	
			cltLetterMsg_TradeServerSell * pclinfo = (cltLetterMsg_TradeServerSell*)cData;

			TCHAR szAcu[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siTradeMoney, szAcu, sizeof(szAcu) );


			NTCHARString512 strMessage( GetTxtFromMgr(6258) );

			strMessage.Replace( TEXT("#amount#"),	SI32ToString(pclinfo->siItemNum) );
			strMessage.Replace( TEXT("#itemname#"), pclClient->pclItemManager->GetName(pclinfo->siItemUnique) );
			strMessage.Replace( TEXT("#acu#"),		szAcu );
			strMessage.Replace( TEXT("#user#"),		pclinfo->szBuyerName );
			strMessage.Replace( TEXT("#server#"),	pclinfo->szBuyerServer );


			StringCchCopy( buffer, txtSize, strMessage );

			return true;
			
		}
		break;
	case LETTERTYPE_WORLDMONEY_REDUCE_PRICE:
		{
			cltLetterMsg_WorldMoneyReducePrice* pclinfo = (cltLetterMsg_WorldMoneyReducePrice*)cData;

			cltDate cldate;
			cldate.MakeFullDateFromDateVary(pclinfo->siDate);

			TCHAR szAcu[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siWorldMoney, szAcu, sizeof(szAcu) );

			TCHAR szMoney[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siPrice, szMoney, sizeof(szMoney) );

			
			NTCHARString512 strMessage( GetTxtFromMgr(6276) );

			strMessage.Replace( TEXT("#year#"),		SI32ToString(cldate.uiYear) );
			strMessage.Replace( TEXT("#month#"),	SI32ToString(cldate.uiMonth) );
			strMessage.Replace( TEXT("#day#"),		SI32ToString(cldate.uiDay) );
			strMessage.Replace( TEXT("#acu#"),		szAcu );
			strMessage.Replace( TEXT("#money#"),	szMoney );


			StringCchCopy( buffer, txtSize, strMessage );

			return true ;
		}
		break;
	case LETTERTYPE_LEVELUPRECOMMANDREWARD_FOR_RECOMMENDER:
		{
			cltLetterMsg_LevelUpREcommandRewardForRecommender * pclinfo = (cltLetterMsg_LevelUpREcommandRewardForRecommender *)cData ;
			
			TCHAR *pText = GetTxtFromMgr(6427);
			TCHAR * pItemName = (TCHAR*)pclClient->pclItemManager->GetName(pclinfo->siRewardItemUnique);

			StringCchPrintf(buffer, txtSize , pText , pclinfo->szCharName, pclinfo->siLevel, pItemName, pclinfo->siRewardItemNum);
			
			return true ;

		}
		break;
	case LETTERTYPE_GUILDJOIN:
		{
			cltLetterMsg_GuildJoin * pclinfo = (cltLetterMsg_GuildJoin*)cData;

			if ( pclinfo->m_bJoin )
			{
				TCHAR *pText = GetTxtFromMgr(6564);
				StringCchPrintf(buffer, txtSize , pText);
			}
			else
			{
				TCHAR *pText = GetTxtFromMgr(6565);
				StringCchPrintf(buffer, txtSize , pText);
			}

            return true;
		}
		break;
	case LETTERTYPE_GUILD_CONFIRMCREATEGUILD:
		{
			cltLetterMsg_Guild_ConfirmCreateGuild* pclinfo = (cltLetterMsg_Guild_ConfirmCreateGuild*)cData;

			TCHAR szMoney[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siMoney, szMoney, sizeof(szMoney) );


			if (pclinfo->bConfirm )
			{
				if ( pclinfo->siMoney > 0 )
				{
					NTCHARString512 strMessage( GetTxtFromMgr(6566) );

					strMessage.Replace( TEXT("#money#"), szMoney );

					StringCchCopy( buffer, txtSize, strMessage );
				}
			}
			else
			{
				if ( pclinfo->siMoney > 0 )
				{
					// 허가지만 돈이 없어서.
					NTCHARString512 strMessage( GetTxtFromMgr(6597) );

					strMessage.Replace( TEXT("#money#"), szMoney );

					StringCchCopy( buffer, txtSize, strMessage );

				}
				else
				{
					TCHAR *pText = GetTxtFromMgr(6567);
					StringCchPrintf(buffer, txtSize , pText);
				}
			}

			return true ;
		}
		break;
	case LETTERTYPE_GUILD_SECONDMASTER:
		{
			cltLetterMsg_Guild_SecondMaster* pclinfo = (cltLetterMsg_Guild_SecondMaster*)cData;

			if (pclinfo->bConfirm )
			{
				TCHAR *pText = GetTxtFromMgr(6599);
				StringCchPrintf(buffer, txtSize , pText);
			}
			else
			{
				TCHAR *pText = GetTxtFromMgr(6600);
				StringCchPrintf(buffer, txtSize , pText);
			}

			return true ;
		}
		break;
	case LETTERTYPE_GUILD_CHANGEMASTER:
		{
		//cltLetterMsg_Guild_ChangeMaster* pclinfo = (cltLetterMsg_Guild_ChangeMaster*)cData;

			TCHAR* pText = GetTxtFromMgr(6646);
			StringCchPrintf( buffer, txtSize, pText);

			return true;
		}
		break;
	case LETTERTYPE_GUILD_CHANGEMASTER_NOTCONNECT_MONTH:
		{
			cltLetterMsg_Guild_ChangeMaster_Notconnect_Month* pclInfo = (cltLetterMsg_Guild_ChangeMaster_Notconnect_Month*)cData;
			if ( NULL == pclInfo )
			{
				return false;
			}

			NTCHARString128 strText( GetTxtFromMgr(8781) );
			strText.Replace( TEXT("#name#"), pclInfo->szCharName );

			StringCchPrintf( buffer, txtSize, strText);

			return true;
		}
		break;
	case LETTERTYPE_GUILD_CONFIRMCHANGEVILLAGE:
		{
			// 마을 바꼈다고 메세지
			cltLetterMsg_Guild_ConfirmChangeVillage* pclinfo = (cltLetterMsg_Guild_ConfirmChangeVillage*)cData;
			if ( pclinfo->bConfirm )
			{
				TCHAR *pText = GetTxtFromMgr(6629);
				StringCchPrintf(buffer, txtSize , pText);
			}
			else
			{
			}
			return true;
		}
		break;
	case LETTERTYPE_GUILD_SUCCESSRESERVE:
		{
			cltLetterMsg_GuildWar_SuccessReserve* pclinfo = (cltLetterMsg_GuildWar_SuccessReserve*)cData;

			SI32 roomindex = pclinfo->siRoomIndex;
			// 방 인덱스로 시작 시간을 구한다.
			SI32 hour = roomindex / 2;
			SI32 minute = ( roomindex % 2 ) * 30;
			TCHAR* pText = GetTxtFromMgr(6695);
			StringCchPrintf( buffer, txtSize, pText, pclinfo->szHomeGuild, pclinfo->szAwayGuild, hour, minute);
			return true;
		}
		break;
	case LETTERTYPE_GUILD_DESTROY:
		{
			cltLetterMsg_Guild_Destroy* pclinfo = (cltLetterMsg_Guild_Destroy*)cData;

			if (pclinfo->siReason == 0)
			{
				TCHAR* pText = GetTxtFromMgr(7104);
				StringCchCopy( buffer, txtSize, pText);
			}
			else if ( pclinfo->siReason == 1 )
			{
				TCHAR* pText = GetTxtFromMgr(8427);
				StringCchCopy( buffer, txtSize, pText );
			}
			else if ( pclinfo->siReason == 2 )
			{
				TCHAR* pText = GetTxtFromMgr(10059);
				StringCchCopy( buffer, txtSize, pText );
			}
			else if ( pclinfo->siReason == 3 )
			{
				TCHAR* pText = GetTxtFromMgr(6706);
				StringCchCopy( buffer, txtSize, pText );
			}
			else if ( pclClient->IsCountrySwitch(Switch_Guild_DestroyOverMOnth) && pclinfo->siReason == 4 )	// 휴면길드 삭제 메세지
			{
				TCHAR* pText = GetTxtFromMgr(8690);
				StringCchCopy( buffer, txtSize, pText );
			}
			return true;
		}
		break;
	case LETTERTYPE_GUILDDUNGEON_APPLY:
		{
			cltLetterMsg_GuildDungeon_Apply* pclinfo = (cltLetterMsg_GuildDungeon_Apply*)cData;

			if(pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar))
			{
				TCHAR *pTitle = GetTxtFromMgr(6739);
				TCHAR *pText = GetTxtFromMgr(6740);

				StringCchPrintf( buffer, 256, pText, pclClient->pclMapManager->GetMapName( pclinfo->siMapIndex ) );
				return true;
			}
			else
			{
				TCHAR *pTitle = GetTxtFromMgr(6739);
				TCHAR *pText = GetTxtFromMgr(6740);

				StringCchPrintf( buffer, 256, pText, pclClient->pclMapManager->GetMapName( pclinfo->siMapIndex ) );
				return true;

			}
		}
		break;
	case LETTERTYPE_GUILDDUNGEON_APPLY_NEW:
		{
			cltLetterMsg_GuildDungeon_Apply* pclinfo = (cltLetterMsg_GuildDungeon_Apply*)cData;

			if(pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar))
			{
				TCHAR *pTitle = GetTxtFromMgr(6739);
				TCHAR *pText = GetTxtFromMgr(9969);

				StringCchPrintf( buffer, 256, pText);
				return true;
			}
		}
		break;
	case LETTERTYPE_GUILDDUNGEON_CHANGEGPMONEY:
		{
			cltLetterMsg_GuildDungeon_ChangeGPMoney* pclinfo = (cltLetterMsg_GuildDungeon_ChangeGPMoney*)cData;

			TCHAR* pTitle = GetTxtFromMgr(6741);


			TCHAR szMoney[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->siAddMoney, szMoney, sizeof(szMoney) );

			NTCHARString512 strMessage( GetTxtFromMgr(6742) );

			strMessage.Replace( TEXT("#gp#"),		SI32ToString(pclinfo->siAddGP) );
			strMessage.Replace( TEXT("#money#"),	szMoney );
			strMessage.Replace( TEXT("#itemname#"),	pclClient->pclItemManager->GetName(pclinfo->siItemUnique) );
			strMessage.Replace( TEXT("#amount#"),	SI32ToString(pclinfo->siItemNum) );

			StringCchCopy( buffer, txtSize, strMessage );



			return true;
		}
		break;
	case LETTERTYPE_GUILDDUNGEON_GETOWNERSHIP:
		{
			cltLetterMsg_GuildDungeon_GetOwnerShip* pclinfo = (cltLetterMsg_GuildDungeon_GetOwnerShip*)cData;

			TCHAR* pTitle = GetTxtFromMgr(6743);
			TCHAR* pText = GetTxtFromMgr(6744);

			StringCchPrintf( buffer, 256, pText, pclClient->pclMapManager->GetMapName( pclinfo->siMapIndex ) );
			return true;
		}
		break;
	case LETTERTYPE_RENOUNCEPUPIL:
		{
			cltLetterMsg_RenouncePupil* pclinfo = (cltLetterMsg_RenouncePupil*)cData;

			TCHAR* pTitle = GetTxtFromMgr(6862);
			TCHAR* pText = GetTxtFromMgr(6864);

			StringCchPrintf( buffer, 256, pText, pclinfo->szTeacherName );
			return true;
		}
		break;
	case LETTERTYPE_ACCEPTFRIEND_RESULT:
		{
			cltLetterMsg_AcceptFriend_Result* pclinfo = (cltLetterMsg_AcceptFriend_Result*)cData;

			if( pclinfo->uiType == cltLetterMsg_AcceptFriend_Result::ACCEPTFRIEND )
			{
				TCHAR* pTitle = GetTxtFromMgr(10022);
				TCHAR* pText = GetTxtFromMgr(10023);

				StringCchPrintf( buffer, 256, pText, pclinfo->szPersonName );
			}
			else if( pclinfo->uiType == cltLetterMsg_AcceptFriend_Result::REJECTFRIEND )
			{
				TCHAR* pTitle = GetTxtFromMgr(10024);
				TCHAR* pText = GetTxtFromMgr(10025);

				StringCchPrintf( buffer, 256, pText, pclinfo->szPersonName );
			}
			return true;
		}
		break;
	case ACCOUNT_LETTERTYPE_PRESENT:
		{
			cltLetterMsg_Present * pclinfo = (cltLetterMsg_Present*)cData;
			
			//[진성] 선물하기. 아이템 이름이 경우 메시지. => 2008-7-24
			if( 0 < strlen(pclinfo->szItemName))
			{
				TCHAR * pTitle = GetTxtFromMgr(6097);
				TCHAR * pText  = GetTxtFromMgr(7513);

				StringCchPrintf( buffer , 256 , pText , pclinfo->szFromPlayerName , 
														pclinfo->szItemName , 
														pclinfo->siItemNum , 
														pclinfo->szComment );
			}
			//[진성] 선물하기. 아이템 이름이 없을 경우 메시지. => 2008-7-24
			else if( 0 < strlen(pclinfo->szFromPlayerName))
			{
				TCHAR * pTitle = GetTxtFromMgr(6097);
				TCHAR * pText  = GetTxtFromMgr(8651);

				StringCchPrintf( buffer , 256 , pText , pclinfo->szFromPlayerName , 
														pclinfo->szComment );
			}
			
			return true;
		}
		break;
	case LETTERTYPE_TEACHERREWARD_PUPILREWARD:
		{
			cltLetterMsg_TeacherReward_PupilReward* pclinfo = (cltLetterMsg_TeacherReward_PupilReward*)cData;

			if( pclinfo->uiType == cltLetterMsg_TeacherReward_PupilReward::TEACHER )
			{
				TCHAR* pTitle = GetTxtFromMgr(10031);
				TCHAR* pText = GetTxtFromMgr(10029);

				const TCHAR* itemname	= pclClient->pclItemManager->GetName(pclinfo->siItemUnique);
				switch( pclClient->siServiceArea )
				{	
				case ConstServiceArea_China:	
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->szPersonName, pclinfo->siLevel, pclinfo->siItemNum, itemname );
						if( pclinfo->uiTiredState == cltCharCommon::TIRED_STATUS_LITTLE_TIRED )
						{
							pText = GetTxtFromMgr( 10198 );
							StringCchCat( buffer, 256, pText );
						}
						else if( pclinfo->uiTiredState == cltCharCommon::TIRED_STATUS_MUCH_TIRED )
						{
							pText = GetTxtFromMgr( 10199 );
							StringCchCat( buffer, 256, pText );
						}
					}
					break;
				case ConstServiceArea_NHNChina:
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->szPersonName, pclinfo->siLevel, pclinfo->siItemNum, itemname );
						if( pclinfo->uiTiredState == cltCharCommon::TIRED_STATUS_LITTLE_TIRED )
						{
							pText = GetTxtFromMgr( 10198 );
							StringCchCat( buffer, 256, pText );
						}
						else if( pclinfo->uiTiredState == cltCharCommon::TIRED_STATUS_MUCH_TIRED )
						{
							pText = GetTxtFromMgr( 10199 );
							StringCchCat( buffer, 256, pText );
						}
					}
					break;
				case ConstServiceArea_Taiwan:	
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->szPersonName, pclinfo->siLevel, pclinfo->siItemNum, itemname);
					}
					break;
				default:
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->szPersonName, pclinfo->siLevel, itemname, pclinfo->siItemNum);
					}
					break;
				}
			}
			else if( pclinfo->uiType == cltLetterMsg_TeacherReward_PupilReward::PUPIL )
			{
				TCHAR* pTitle = GetTxtFromMgr(10031);
				TCHAR* pText = GetTxtFromMgr(10030);

				const TCHAR* itemname	= pclClient->pclItemManager->GetName(pclinfo->siItemUnique);
				switch( pclClient->siServiceArea )
				{
				case ConstServiceArea_China:
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->siLevel, pclinfo->siItemNum, itemname );
						if( pclinfo->uiTiredState == cltCharCommon::TIRED_STATUS_LITTLE_TIRED )
						{
							pText = GetTxtFromMgr( 10198 );
							StringCchCat( buffer, 256, pText );
						}
						else if( pclinfo->uiTiredState == cltCharCommon::TIRED_STATUS_MUCH_TIRED )
						{
							pText = GetTxtFromMgr( 10199 );
							StringCchCat( buffer, 256, pText );
						}
					}
					break;
				case ConstServiceArea_Taiwan:
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->siLevel, pclinfo->siItemNum, itemname );
					}
					break;
				default:
					{
						StringCchPrintf( buffer, 256, pText, pclinfo->siLevel, itemname, pclinfo->siItemNum );
					}
					break;
				}
			}
			return true;
		}
		break;
	case LETTERTYPE_PVPRESULT:
		{
			cltLetterMsg_PVPResult* pclinfo = (cltLetterMsg_PVPResult*)cData;

			if( false == pclinfo->bWin )
			{
				if( pclinfo->uiType == cltLetterMsg_PVPResult::LOSE )
				{
					if( true == pclinfo->bDestroyItem )
					{
						TCHAR* pText = GetTxtFromMgr(10067);
						TCHAR* tempItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
						StringCchPrintf( buffer, 256, pText, pclinfo->szWinUserName, tempItemName );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(10063);
						TCHAR* tempItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
						StringCchPrintf( buffer, 256, pText, pclinfo->szLoseUserName, pclinfo->szWinUserName, tempItemName );
					}
				}
				else if( pclinfo->uiType == cltLetterMsg_PVPResult::VILLAGE )
				{
					TCHAR* pText = GetTxtFromMgr(10063);
					TCHAR* tempItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
					StringCchPrintf( buffer, 256, pText, pclinfo->szLoseUserName, pclinfo->szWinUserName, tempItemName );
				}
				else if( pclinfo->uiType == cltLetterMsg_PVPResult::GUILD )
				{
					TCHAR* pText = GetTxtFromMgr(10063);
					TCHAR* tempItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
					StringCchPrintf( buffer, 256, pText, pclinfo->szLoseUserName, pclinfo->szWinUserName, tempItemName );
				}
			}
			else
			{
				if( pclinfo->uiType == cltLetterMsg_PVPResult::WIN )
				{
					if( true == pclinfo->bDestroyItem )
					{
						TCHAR* pText = GetTxtFromMgr(10065);
						TCHAR* tempItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
						StringCchPrintf( buffer, 256, pText, pclinfo->szLoseUserName, tempItemName );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(10061);
						TCHAR* tempItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclinfo->siItemUnique );
						StringCchPrintf( buffer, 256, pText, pclinfo->szLoseUserName, tempItemName );
					}
				}
			}
			return true;
		}
		break;
	case LETTERTYPE_GUILD_ALERTDESTROY:
		{
			cltLetterMsg_Guild_AlertDestroy* pclinfo = (cltLetterMsg_Guild_AlertDestroy*)cData;

			TCHAR* pText = GetTxtFromMgr(10059);
			StringCchCopy( buffer, txtSize, pText );
			return true;
		}
		break;

	case LETTERTYPE_SYSTEMREWARD_GIVE:
		{
			cltLetterMsg_GiveSystemReward* pclinfo = (cltLetterMsg_GiveSystemReward*)cData;

			TCHAR itemName[256] = {0,};
			pclinfo->clInfo.m_clItem.GetFullNameNNum( pclClient->pclItemManager, itemName, 256 );

			TCHAR szMoney[128] = { '\0', };
			g_JYLibFunction.SetNumUnit( pclinfo->clInfo.m_siMoney, szMoney, sizeof(szMoney) );


			NTCHARString512 strMessage( GetTxtFromMgr(10188) );

			strMessage.Replace( TEXT("#itemname#"),	itemName );
			strMessage.Replace( TEXT("##"),			szMoney );

			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;

	case LETTERTYPE_INVITE_APPLYRESIDENT:
		{
			cltLetterMsg_Invite_Applyresident* pclinfo = (cltLetterMsg_Invite_Applyresident*)cData;

			NTCHARString512 strMessage( GetTxtFromMgr(9123) );
			strMessage.Replace( TEXT("#mentorname#"),		pclinfo->szMentorName   );
			strMessage.Replace( TEXT("#disciplename#"),		pclinfo->szDiscipleName );
			
			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;
	case LETTERTYPE_FAMILY_LEAVE_FAMILY:
		{
			cltLetterMsg_LeaveFamilyLetter* pclinfo = (cltLetterMsg_LeaveFamilyLetter*)cData;

			NTCHARString512 strMessage( GetTxtFromMgr(30135) );
			strMessage.Replace( TEXT("#name#"),		pclinfo->szName  );
			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;
	case LETTERTYPE_FAMILY_ADD_FAMILY:	// 기존가족
		{
			cltLetterMsg_ADDFamilyLetter_ToFamily* pclinfo = (cltLetterMsg_ADDFamilyLetter_ToFamily*)cData;

			NTCHARString512 strMessage( GetTxtFromMgr(30127) );
			strMessage.Replace( TEXT("#name#"),		pclinfo->szAddCharName  );
			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;
	case LETTERTYPE_FAMILY_ADD_FAMILY_TO_ME:	// 새 가족 
		{
			cltLetterMsg_ADDFamilyLetter_ToFamily_To_Me* pclinfo = (cltLetterMsg_ADDFamilyLetter_ToFamily_To_Me*)cData;

			NTCHARString512 strMessage( GetTxtFromMgr(30128) );
			strMessage.Replace( TEXT("#name1#"),		pclinfo->szParentName_1  );
			strMessage.Replace( TEXT("#name2#"),		pclinfo->szParentName_2  );
			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;
	case LETTERTYPE_CREATECHARREWARD:
		{
			NTCHARString512 strMessage( GetTxtFromMgr(8930) );

			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;

	case LETTERTYPE_WEDDINGCEREMONY_CANCEL:
		{
			NTCHARString128 strMessage( GetTxtFromMgr(30067) );

			StringCchCopy( buffer, txtSize, strMessage );
			return true;
		}
		break;
	default:
		{
			TCHAR temp[256];
			StringCchPrintf(temp, 256, TEXT("cltLetterUnit::GetText [%d]"), pclheader->siType);
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox(NULL, temp, TEXT("fd3fdf"), MB_OK|MB_TOPMOST);
		}
		break;
	}

	return false;
}
