//---------------------------------
// 2003/6/18 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "../../Client/Client.h"
#include "../../Client/InterfaceMgr/DialogController.h"
#include "../../Client/NStructureDlg/Market/NMarketDlg.h"

#include "..\..\Resource.h"

#include "Market.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"

//-------------------------------------
// CommonLogic
//-------------------------------------
#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "..\..\CommonLogic\Msg\MsgType-Quest.h"

#include "../CityHall/TaxpaymentListDlg.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

//----------------------------------------
// Server
//----------------------------------------
#include "..\..\Server\Statistics\Statistics.h"

//-------------------------------------
// 이조
//-------------------------------------
#include "..\Server\Minister\MinisterBase.h"
#include "..\Server\Minister\Minister-Izo.h"
#include "..\Server\Minister\MinisterMgr.h"



extern cltCommonLogic* pclClient;

cltMarketManager::cltMarketManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName ) : cltStructureMngBasic(MAX_MARKET_TAB_NUMBER, pclcm, ranktype, strFileName )
{
	SI32 i, j;
	
	m_hItemImage		= NULL;
	m_hItemMallBitmap	= NULL;

	SellButtonEnableSwitch	= FALSE;

	ImageListConnectedSwitch	= FALSE;

	pclCurrentInvItem			= NULL;
	pclCurrentStgItem			= NULL;

	ShowResponseOrderSiwtch		= TRUE;
	siResponseOrderIndex		= 0;
	CurrentSelectedBuyItemUnique	= 0;


	hUserDlgBuyHwnd				= NULL;

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{
		pclItemTypeSearch[i]	= NULL;
		pclItemTypeOrder[i]		= NULL;
		siItemTypeOrderIndex[i]	= 0;
	}

	if(gamemode == GAMEMODE_SERVER)
	{
		pclOrderManager	= new cltTradeOrderManager(MAX_MARKET_ORDER_NUMBER);
		
		
		for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
		{
			pclItemTypeSearch[i]	= new SI32[MAX_MARKET_ORDER_NUMBER];
			for(j = 0;j < MAX_MARKET_ORDER_NUMBER;j++)
			{
				pclItemTypeSearch[i][j] = -1;
			}

			pclItemTypeOrder[i]		= new SI32[MAX_MARKET_ORDER_NUMBER];
			for(j = 0;j < MAX_MARKET_ORDER_NUMBER;j++)
			{
				pclItemTypeOrder[i][j]	= -1;
			}
		}
		
	}
	else
	{
		pclOrderManager		= NULL;
	}

	siRefBufferIndex	= 0;

	m_hUserDlgBuyList = NULL ;
	m_siSelectedBuyList = -1 ;
	for ( i = 0 ; i < MAX_RESPONSE_ORDER_NUMBER ; i ++ )
		m_siSelectedBuyIndex[i] = -1 ;

	// 마켓에 등록된 아이템들의 최소가격과 그 아이템의 등록 개수를 저장한다.
	for (i = 0 ; i < MAX_ITEMINFO_NUMBER; i++ )
	{
		m_MinPrice[i]		= MAX_PRICE_IN_MARKET;
		m_MinPriceAmount[i] = -1;
	}
}

cltMarketManager::~cltMarketManager()
{
	SI32 i;

	if(pclOrderManager)
	{
		delete pclOrderManager;
		pclOrderManager = NULL;
	}

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{
		if(pclItemTypeOrder[i])
		{
			delete[] pclItemTypeOrder[i];
			pclItemTypeOrder[i] = NULL;
		}

		if(pclItemTypeSearch[i])
		{
			delete[] pclItemTypeSearch[i];
			pclItemTypeSearch[i] = NULL;
		}
	}

}

// 최초 환영 인사말표시 . 
void cltMarketManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1667);
	SetMainExplain(hDlg, pText);
}

void cltMarketManager::Do_WM_INITDIALOG(SI32 charunique)
{

	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);	

	// 건물에 대한 정보를 서버에 요청한다. 
	SI32 id = 1;
	if(id)
	{
		//-----------------------------------------------------------
		// 캐릭터 창을 여는 퀘스트가 진행중이라면 
		// 게임 서버로 퀘스트가 완료되었음을 알려준다.
		//-----------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_STRUCTUREOPEN;
		if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

	}

	
	// 구매 대화 상자 핸들을 초기화한다. 
	hUserDlgBuyHwnd				= NULL;
	
}

// 객주를 신설한다.
BOOL cltMarketManager::CreateMarket(SI32 siunique)
{
	
	// 객주를 만든다. 
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltMarket(siunique);
		return TRUE;
	}
		
	return FALSE;
}


// 특정아이템의 가장 저렴한 주문을 찾는다. 
bool cltMarketManager::FindLowPriceOrderInfo(SI32 unique, GMONEY* pprice, SI32* pamount)
{
	// 가지고 있던 데이터를 사용하는 것으로 변경 - by LEEKH 2008-10-09 
	// return pclOrderManager->FindLowPriceOrderInfo(unique, pprice, pamount);

	cltServer* pclserver = (cltServer*)pclClient;
	// DB 초기화 완료될때까지 기다림
	if( pclserver->bCompleteDBInitAll == FALSE )		return false;

	SI32 ref = pclserver->pclItemManager->FindItemRefFromUnique(unique);
	if( ref < 0 || ref >= MAX_ITEMINFO_NUMBER )			return false;
	if(m_MinPriceAmount[ref] < 0)						return false;

	*pprice = m_MinPrice[ref];
	*pamount = m_MinPriceAmount[ref];

	return true;
}

void cltMarketManager::MakeLowPriceOrderInfo()
{
	SI32 i = 0;
	cltServer* pclserver = (cltServer*)pclClient;
	// DB 초기화 완료될때까지 기다림
	if( pclserver->bCompleteDBInitAll == FALSE )		return;

	for(i=0; i<MAX_ITEMINFO_NUMBER; i++)
	{
		// 아이템의 최저가격 계산을 위해 데이터 초기화
		m_MinPrice[i] = MAX_PRICE_IN_MARKET;
		m_MinPriceAmount[i] = -1;
	}

	//---------------------------------------------------------
	// 아이템의 최저 가격을 저장한다.
	cltItem  clItem;
	cltTradeOrder** porder	= pclOrderManager->pclOrderSell;
	for(i = 0;i < pclOrderManager->siMaxOrderNum;i++)
	{
		if(porder[i] == NULL)							continue;

		clItem.Set(&porder[i]->clItemData);
		SI32 ref = pclserver->pclItemManager->FindItemRefFromUnique(clItem.siUnique);
		if( ref < 0 || ref >= MAX_ITEMINFO_NUMBER )		continue;

		if ( porder[i]->siPrice < m_MinPrice[ref] && porder[i]->bBankError == false )
		{
			m_MinPrice[ref]			= porder[i]->siPrice;
			m_MinPriceAmount[ref]	= porder[i]->siAmount;
		}
	}
	//---------------------------------------------------------
}

// 임대료를 처리하는등의 액션을 취한다. 
void cltMarketManager::Action(bool bdaychangedswitch)
{
	static SI32 marketitemindex	= 0;

	SI32		i		  = 0;
	cltServer*	pclserver = (cltServer*)pclClient;

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;

	//----------------------------------------
	// 매일 주문 정보를 정리한다. (가격을 15%씩 낮춘다 ) 
	//----------------------------------------
	if(bdaychangedswitch)
	{
		// 전체 팔자 주문을 검색하여 날짜가 일정 시간 이상 경과한 주문은 가격을 낮춘다. 
		// 가격을 더이상 낮출 수 없는 경우에는 주문 자체를 삭제한다. (물품도.)
		if(pclserver->pclTime != NULL)
		{
			SI32 curdate = pclserver->pclTime->clDate.GetDateVary();
			
			cltTradeOrder clorder;
			for( i = 0;i < pclOrderManager->siMaxOrderNum;i++)
			{
				clorder.Set(pclOrderManager->pclOrderSell[i]);

				if(clorder.siPersonID <= 0)continue;

				// PCK : 깎이기 전의 가격을 기록한다. (로그용)
				GMONEY siBeforeMoney = clorder.siPrice;

				if(clorder.siDate + 30 <= curdate)	// 18일에 한번씩
				{
					// 가격이 유효한 경우, 
					if( clorder.siPrice )
					{
						CMinisterIzo *pclminister = (CMinisterIzo *)((cltServer*)pclClient)->pclMinisterMgr->GetMinister(MINISTER_IZO);
						SI32 downrate = pclminister->GetMarketDownRate();

						GMONEY cutprice = clorder.siPrice * downrate / 100;
						
						// 가격을 깎는다. 
						clorder.siPrice -= max(2, cutprice);
						if(clorder.siPrice < 0)clorder.siPrice = 0;
					}
					
					// 날짜도 갱신한다. 
					clorder.siDate	= curdate;

					// 변경된 주문 정보를 DB로 보낸다. 
					// 가격이 더 이상 떨어지지 않으면, 삭제한다.
					sDBRequest_ReduceOrderPrice clMsg(&clorder, siBeforeMoney);
					pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				}
			}
		}
	}

	//----------------------------------------------
	// 객주 주문을 업데이트 한다. 
	//----------------------------------------------
	if((pclClient->GetFrame() % 20000) == 0)
	{
		// 초기화. 
		for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
		{
			siItemTypeOrderIndex[i] = 0;
			
			for(SI32 j = 0;j < MAX_MARKET_ORDER_NUMBER;j++)
			{
				pclItemTypeSearch[i][j] = -1;
				pclItemTypeOrder[i][j]	= -1;
			}
		}

		cltItem  clItem;
		cltTradeOrder** porder	= pclOrderManager->pclOrderSell;
		for(i = 0;i < pclOrderManager->siMaxOrderNum;i++)
		{
			if(porder[i] == NULL)							continue;

			SI32 itemunique = porder[i]->clItemData.siUnique;
			if(itemunique <= 0)								continue;
			SetItemTypeOrder(porder[i], 100);
		}
	}
	
	//-----------------------------------------
	// 자동 구매를 수행한다. 
	//-----------------------------------------
	// [검색 : 황진성 2007. 10. 17 시스템에서 물품 구입.]
	if(pclClient->GetFrame() % pclClient->siMarketAutoBuyDelayFrame == 0)		
	{
		// 품목을 선정한다. 
		SI16 typelist[] =
		{
			ITEMTYPE_SWORD,
			ITEMTYPE_SPEAR,
			ITEMTYPE_AXE,
			ITEMTYPE_STAFF,
			ITEMTYPE_GUN,
			ITEMTYPE_BOW,
			ITEMTYPE_ARMOUR,
			ITEMTYPE_HELMET,
			ITEMTYPE_DRESS,
			//IT EMTYPE_HAT,
			//ITEMTYPE_RING,
			//ITEMTYPE_NECK,
			ITEMTYPE_BELT,
			ITEMTYPE_DISH,
			ITEMTYPE_IRON,
			//ITEMTYPE_CANNON,
			ITEMTYPE_SHOES,
			ITEMTYPE_FISHINGROD			
		};

		SI32 typenum = sizeof(typelist) / sizeof(SI16);
		
		SI32 itemtype = typelist[ rand() % typenum];

		// 적절한 팔자 주문을 찾는다. 
		cltTradeOrder* pclorder = GetOrderForSystemBuy(itemtype);
		
		
		if(pclorder)
		{
			// 구입 주문을 넣는다. 
			cltTradeOrder clorder;

			clorder.Set(pclorder);
			//[수정중 : 황진성 2007.10.24 시스템이 시장에 유저가 올린 물품을 구입하는 경우.]
			clorder.siPersonID	= 0;
			clorder.siAmount	= 1;
			
			pclserver->MarketBuyAction(0, 0, &clorder, MARKETTRADE_REASON_EQUIPITEM);

			if ( pclserver->pclStatisticsManager )
			{
				pclserver->pclStatisticsManager->clDailyStatistics.siSystemBuyMoney += clorder.siPrice;
				pclserver->pclStatisticsManager->clDailyStatistics.siSystemBuyNum++; 
			}

			SystemBuyItemStatistics( pclorder->clItemData.siUnique, pclorder->siPrice * clorder.siAmount );

			NStatistics_Add("Item", clorder.clItemData.siUnique,"SystemBuy", clorder.siAmount);
			NStatistics_Add("Item", clorder.clItemData.siUnique,"SystemBuyPrice", pclorder->siPrice);
			NStatistics_Set("Item", clorder.clItemData.siUnique, "Name", pclserver->pclItemManager->GetName(clorder.clItemData.siUnique));
		}
	}

	//-----------------------------------------
	// 자동 구매를 수행한다. 
	// KHY - 0901 -  과금 아이템 자동구매 설정.
	if (pclserver->siMarketMoneyItemAutoBuyDelayTime > 0)
	{
		if(pclClient->GetFrame()%pclserver->siMarketMoneyItemAutoBuyDelayTime == 0)		
		{
			// 과금 아이템 자동구매 금액의 총액이 현채 총통화량의 0.1%를 넘지 못한다.
			SI64 siLimitMoney = (SI64)((REAL32)pclserver->siTotalMoney * 0.001f);
			if( pclserver->siMarketMoneyItemMaxpriceNow < siLimitMoney )
			{
				// 품목을 선정한다. 
				SI16 count = 0;
				SI16 randSelect = 0;

				// 데이터가 있는 것을 대상으로 한다.
				for(SI32 i = 0; i <MAX_SYSTEMBUYTEM_NUM ;i++)
				{
					if(pclserver->stSystemBuyItem[i].m_siUnique > 0)
						count++;
				}

				if(count > 0) // 등록된 아이템이 없다.
				{
					randSelect = rand()%count; // 최종 선택.

					// 기준 가격은 아이템 시세의 80% 이하이다.
					GMONEY gMaxPrice = 0;
					if( pclserver->stSystemBuyItem[randSelect].m_siUnique > 0)
					{
						cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, pclserver->stSystemBuyItem[randSelect].m_siUnique);
						if(pclprice)		gMaxPrice = (GMONEY)(pclprice->fPrice * 0.9f);
					}
					// 적절한 팔자 주문을 찾는다.  - 기준가격 변경: 시세의 90%
					cltTradeOrder* pclorder = NULL;
					if( gMaxPrice > 0 )		pclorder = GetOrderForSystemBuyUnique(pclserver->stSystemBuyItem[randSelect].m_siUnique, gMaxPrice);

					if(pclorder)
					{
						// 구입 주문을 넣는다. 
						cltTradeOrder clorder;

						clorder.Set(pclorder);
						clorder.siPersonID	= 0;
						clorder.siAmount	= 1;

						// 내 자신이 사고 파는 것으로 시스템이 사는것임을 확인하게 한다.
						//<-------------------------------------------->
						pclserver->MarketBuyAction(0, 0, &clorder, MARKETTRADE_REASON_CASHITEM);  // 0 은 시스템 구입이다.

						// 본래 리스폰스에서 해야겠지만, 과금아이템만 구분하는것이 마땅하지 않아 여기서 더해준다.
						pclserver->siMarketMoneyItemMaxpriceNow += pclorder->siPrice;

						sDBRequest_SetSystemBuyItemNowMoney clMsg( pclserver->siMarketMoneyItemMaxpriceNow);
						pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

						if ( pclserver->pclStatisticsManager )
						{
							pclserver->pclStatisticsManager->clDailyStatistics.siSystemBuyMoney += clorder.siPrice;
							pclserver->pclStatisticsManager->clDailyStatistics.siSystemBuyNum++; 
						}

						pclClient->pclLog->FilePrint("Config\\SystemBuyItem_CashItem.log", "ItemUnique:%d\tPrice:%I64d",
								pclorder->clItemData.siUnique, pclorder->siPrice );

						SystemBuyItemStatistics( pclorder->clItemData.siUnique, pclorder->siPrice * clorder.siAmount );
					}
				}	
			}
		}
	}
}

void cltMarketManager::SystemBuyItemStatistics(SI16 siItemUnique, GMONEY gPrice)
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI32	i;
	SI32	siTotalCount[MAX_ITEM_UNIQUE];
	GMONEY	gTotalPrice[MAX_ITEM_UNIQUE];

	for (i=0; i<MAX_ITEM_UNIQUE; i++)
	{
		siTotalCount[i] = -1;
		gTotalPrice[i] = -1;
	}

	TCHAR	szFileName[MAX_PATH];
	StringCchPrintf(szFileName, sizeof(szFileName), "Config\\SystemBuyItemStatistics_%04d%02d%02d.log", pclserver->sTime.wYear, pclserver->sTime.wMonth, pclserver->sTime.wDay);

	// 이전에 생성된 파일을 읽는다.    
	FILE* fp = NULL;
	TCHAR	szLine[1024];
	fp = fopen(szFileName, "rt");
	if( fp )
	{
		fgets(szLine, sizeof(szLine), fp);		// 컬럼명 통과

		while( fgets(szLine, sizeof(szLine), fp) )
		{
			SI32	siUnique = 0;
			TCHAR	szItemName[128];
			SI32	siFileCount;
			SI64	siFilePrice;
			// 아이템유니크 아이템_이름 개수 총가격
			_stscanf( szLine, "%d %s %d %I64d", &siUnique, szItemName, &siFileCount, &siFilePrice );
			if( siUnique >= 0 && siUnique < MAX_ITEM_UNIQUE )
			{
				siTotalCount[siUnique]	= siFileCount;
				gTotalPrice[siUnique]	 = siFilePrice;
			}
		}
		fclose(fp);
	}

	// 게임에서 사용하고 있는 아이템은 변수값을 풀어준다.
	for( i=0; i<MAX_ITEM_UNIQUE; i++)
	{
		if( pclserver->pclItemManager->FindItemRefFromUnique( i ) > 0)
		{
			if( siTotalCount[i] < 0 || gTotalPrice[i] < 0 )
			{
				siTotalCount[i] = 0;		gTotalPrice[i] = 0;
			}
		}
	}

	// 값 증가
	if( siItemUnique >= 0 && siItemUnique < MAX_ITEM_UNIQUE )
	{
		siTotalCount[siItemUnique]++;
		gTotalPrice[siItemUnique] += gPrice;
	}

	// 내용을 새로 기록한다.
	fp = fopen(szFileName, "wt");
	if( fp )
	{
		// 컬럼명 기록
		fprintf(fp, "ItemUnique\tItemName\tTotalCount\tTotalPrice\n");
		for( i=0; i<MAX_ITEM_UNIQUE; i++ )
		{
			if( siTotalCount[i] < 0 )		continue;

			NTCHARString128 kItemName;
			kItemName = pclserver->pclItemManager->GetName(i);		kItemName.Replace(" ", "_");
			fprintf(fp, "%d\t%s\t%d\t%I64d\n", i, (TCHAR*)kItemName, siTotalCount[i], gTotalPrice[i]);
		}
		fclose(fp);
	}
}

// 서버 리턴값 처리 함수. 
void cltMarketManager::ServerRtnValOperate()
{
	// 먼저 서버로부터 받은 메시지를 복사해 둔다. 
	SI32 returnval = siServerResponseValue;

	cltStructureMngBasic::ServerRtnValOperate();

	switch(returnval)
	{
	case SRVAL_MARKET_SUCCESS_TRADE:
//	case SRVAL_MARKET_FAIL_TRADE_LIEORDER:
		// 업데이트 해야 할 사유가 발생했을 때. 
		// 선택한 아이템의  판매 주문 리스트를 요청한다. 
		cltClient* pclclient = (cltClient*) pclClient;
		if( pclclient->m_pDialog[ NMARKET_DLG ] != NULL && pclclient->m_pDialog[ NMARKET_DLG ]->IsCreate() == true 
			&& ((NMarketUserBuyDlg*)((NMarketDlg*)pclclient->m_pDialog[ NMARKET_DLG ])->m_pUserChildDlg[1]) != NULL )
			((NMarketUserBuyDlg*)((NMarketDlg*)pclclient->m_pDialog[ NMARKET_DLG ])->m_pUserChildDlg[1])->RequestMarketOrderList( CurrentSelectedBuyItemUnique);
		if( pclclient->m_pDialog[ NMARKET_SUB_DLG ] != NULL && pclclient->m_pDialog[ NMARKET_SUB_DLG ]->IsCreate() == true )
			((NMarketUserBuyDlgSub*)pclclient->m_pDialog[ NMARKET_SUB_DLG ])->RequestMarketOrderList( CurrentSelectedBuyItemUnique);

		break;
	}

}

// 아이템 타입별 버퍼에 객주 주문 정보를 추가한다. 
bool cltMarketManager::SetItemTypeOrder(cltTradeOrder* pclorder, SI32 orderunique)
{
	if(pclorder == NULL)return false;

	cltItem clitem;
	clitem.Set(&pclorder->clItemData);

	SI32 itemtype = clitem.GetType(pclClient->pclItemManager);

	if(itemtype <= 0)
	{
		return false;
	}

	// 주문의 인덱스. 
	SI32 index = pclorder->siIndex;
	if(index < 0)return false;

	// 기존 정보 삭제. 
	if(pclorder->siPrice == 0)
	{
		SI32 ref = pclItemTypeSearch[itemtype][index];
		if(ref >= 0)
		{
			pclItemTypeSearch[itemtype][index]	= -1;	// 삭제. 

			pclItemTypeOrder[itemtype][ref]		= 0;

			if(siItemTypeOrderIndex[itemtype] > 0)
			{
				// 마지막 주문을 중간에 넣는다. 
				SI32 tempindex = pclItemTypeOrder[itemtype][ siItemTypeOrderIndex[itemtype] - 1 ];
				pclItemTypeOrder[itemtype][ref] = tempindex;

				pclItemTypeSearch[itemtype][tempindex]	= ref;
				
				siItemTypeOrderIndex[itemtype]--;
			}
		}
	}
	// 신규 또는 업데이트. 
	else
	{
		SI32 ref = pclItemTypeSearch[itemtype][index];
		if(ref >= 0)
		{
			pclItemTypeOrder[itemtype][ref] = index;

		}
		else
		{
			pclItemTypeSearch[itemtype][index] = siItemTypeOrderIndex[itemtype];		// 추가 

			pclItemTypeOrder[itemtype][ siItemTypeOrderIndex[itemtype] ] = index;
			siItemTypeOrderIndex[itemtype]++;	// 개수를 한개 늘인다. 

		}
		
	}

	return true;
}

// 객주에 올라와 있는 물품 중에서 랜덤하게 고른다. 
SI16 cltMarketManager::GetRandomOrder(SI32 itemtype )
{
	SI32 searchnum			= siItemTypeOrderIndex[itemtype];
	cltTradeOrder** porder	= pclOrderManager->pclOrderSell;

	if(searchnum <= 0)return 0;

	siRefBufferIndex	=	0;

	for(SI32 i = 0;i < searchnum;i++)
	{
		SI32 ref = pclItemTypeOrder[itemtype][ i ];

		SI32 itemunique = porder[ref]->clItemData.siUnique;

		// 예외처리 리스트
		switch( itemunique )
		{
		//광물
		case ITEMUNIQUE(5350):
		case ITEMUNIQUE(5355):
		case ITEMUNIQUE(5360):
		case ITEMUNIQUE(5365):
		case ITEMUNIQUE(5370):
		case ITEMUNIQUE(5375):
		case ITEMUNIQUE(5380):
		case ITEMUNIQUE(5385):
		case ITEMUNIQUE(5390):
		//목재
		case ITEMUNIQUE(7030):
		case ITEMUNIQUE(7070):
		case ITEMUNIQUE(7080):
		case ITEMUNIQUE(7100):
		case ITEMUNIQUE(7175):
		case ITEMUNIQUE(7180):
		case ITEMUNIQUE(7185):
			continue;
		}

		// 대상품목의 리스트를 추가한다.
		siRefBuffer[siRefBufferIndex] = ref;
		siRefBufferIndex++;
	}

	if(siRefBufferIndex)
	{
		return porder[ siRefBuffer[rand() % siRefBufferIndex] ]->clItemData.siUnique;
	}

	return 0;

}

// [검색 : 황진성 2007. 10. 17 시스템에서 물품 구입.]
// 객주에 올라와 있는 물품 중에서 시스템이 구입할 물품을 선정한다. 
cltTradeOrder* cltMarketManager::GetOrderForSystemBuy(SI32 itemtype)
{
	SI32 i;

	SI32 searchnum			= siItemTypeOrderIndex[itemtype];
	cltTradeOrder** porder	= pclOrderManager->pclOrderSell;

	if(searchnum <= 0)return 0;

	siRefBufferIndex	=	0;

	for(i = 0;i < searchnum;i++)
	{
		
		SI32 ref = pclItemTypeOrder[itemtype][ i ];
		
		// 해당 물품의 시세를 구한다. 
		cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, porder[ref]->clItemData.siUnique);
		if(pclprice == NULL)continue;
		if(pclprice->siPrice > 50000000) continue ; // 5천만냥 이상은 구하지 않는다.

		SI64 price = (SI64)pclprice->siPrice * 150 / 100 ;

		// 시세보다 훨씬 비싸면 구입하지 않는다. 
		if(porder[ref]->siPrice > price)continue;

		// 대상품목의 리스트를 추가한다.
		siRefBuffer[siRefBufferIndex] = ref;
		siRefBufferIndex++;
	}

	cltTradeOrder* pclorderrtn = NULL;

	if(siRefBufferIndex)
	{
		return porder[ siRefBuffer[rand() % siRefBufferIndex] ];
	}

	return pclorderrtn;


}


// 객주에 올라와 있는 물품 중에서 시스템이 구입할 물품을 선정한다. 
cltTradeOrder* cltMarketManager::GetOrderForSystemBuyUnique(SI16 itemUnique , GMONEY gPrice)
{
	cltServer* pclserver = (cltServer*)pclClient;
	cltTradeOrder* pclorderrtn = NULL;

	SI32 ref1=  pclClient->pclItemManager->FindItemRefFromUnique(itemUnique);

	if(ref1 == 0) return pclorderrtn ;
	SI32 itemtype	= pclClient->pclItemManager->pclItemInfo[ref1]->siType;

	SI32 searchnum			= siItemTypeOrderIndex[itemtype];
	cltTradeOrder** porder	= pclOrderManager->pclOrderSell;

	if(searchnum <= 0)return pclorderrtn;

	GMONEY tempPrice = gPrice;
	SI32 selectRef = -1;

	for(SI32 i = 0;i < searchnum;i++)
	{
		SI32 ref = pclItemTypeOrder[itemtype][ i ];

		// 해당 물품의 시세를 구한다. 
		//cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, porder[ref]->clItemData.siUnique);

		// 같은 유니크의 주문을 찾는다.
		if(porder[ref]->clItemData.siUnique ==  itemUnique)
		{
			// 설정된 금액보다 작아야 한다.
			if(porder[ref]->siPrice <= gPrice) 
			{
				// 그중에서도 가장 작아야 한다.
				if(porder[ref]->siPrice <= tempPrice )
				{
					tempPrice = porder[ref]->siPrice;
					selectRef = ref;
				}
			}
		}
	}

	if(selectRef >= 0)
	{
		return porder[ selectRef];
	}

	return pclorderrtn;


}


cltNewMarketManager::cltNewMarketManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, char *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, ranktype, strFileName )
{
	bUpdateItemPrice = false;
	bUpdateItemReducePrice = false;

	pclTaxPaymentDlg = new cltTaxPaymentListDlg;

	m_bSetFee = true;	// 수수료 설정 가능.
}


cltNewMarketManager::~cltNewMarketManager()
{
	if( pclTaxPaymentDlg ) delete pclTaxPaymentDlg;
}

BOOL cltNewMarketManager::CreateNewMarket(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltNewMarket(siunique);
		return TRUE;
	}

	return FALSE;
}

// 최초 환영 인사말표시..
void cltNewMarketManager::GreetingMsg(HWND hDlg)
{
	char* pText = GetTxtFromMgr(5652);
	SetMainExplain(hDlg, pText);
}

void cltNewMarketManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);

	// 건물에 대한 정보를 서버에 요청한다. 
	SI32 id = 1;
	if(id)
	{
		//-----------------------------------------------------------
		// 캐릭터 창을 여는 퀘스트가 진행중이라면 
		// 게임 서버로 퀘스트가 완료되었음을 알려준다.
		//-----------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_STRUCTUREOPEN;
		if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

	}
}

// 임대료를 처리하는등의 액션을 취한다. 
void cltNewMarketManager::Action(cltTime* pcltime, clock_t CurrentClock)
{
	if(pcltime == NULL)return ;

	CommonAction( pcltime );

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	static SI32 wPrevHour = -1;
	if( wPrevHour != pclserver->sTime.wHour )
	{
		wPrevHour = pclserver->sTime.wHour;

		// 아이템 시세 업데이트는 현실시간 1시간마다 업데이트
		sDBRequest_ManyItemNewPrice clMsg( 0, &pclserver->sTime );
		pclClient->SendDBMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
	}

	if( TABS( CurrentClock - dwUpdateTime ) > 60 * 1000 )
	{
		sDBRequest_NewMarketResult clMsg( pclserver->siServerUnique );
		pclClient->SendDBMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
		dwUpdateTime = CurrentClock;
	}

	// 가격하락은 아침 6시에 한번
	if( 6 == pclserver->sTime.wHour && 5 > pclserver->sTime.wMinute )
	{
		if( false == bUpdateItemReducePrice )
		{
			sDBRequest_NewMarketReducePriceNotice clMsg( pclserver->siServerUnique, &pclserver->sTime );
			pclClient->SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
		
			 pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NEWMARKET, LOGCOMMAND_INDEX_NEWMARKET_REDUCEPRICE, 
											0,	0, 	NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclserver->siServerUnique, 0, 0, 0, 0, 0, NULL	);

			 m_bSetFee			    = true;	// 수수료 설정 가능.
			 bUpdateItemReducePrice = true;
		}
	}
	else
		bUpdateItemReducePrice = false;
	
	return;
}

void cltNewMarketManager::ServerRtnValOperate()
{
	// 먼저 서버로부터 받은 메시지를 복사해 둔다. 
	SI32 returnval = siServerResponseValue;

	cltStructureMngBasic::ServerRtnValOperate();
}