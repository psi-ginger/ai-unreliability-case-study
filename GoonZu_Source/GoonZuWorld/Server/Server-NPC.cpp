//---------------------------------
// 2003/8/19 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Common\SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "../CommonLogic/MsgRval-Define.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMSG-ForeignMerchantNPC.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/dbMsg-Person.h"

#include "..\Client\Interface\NPC1\NPCTradeBaseInfo.h"
//#include "..\Client\Interface\ForeignMerchant\ForeignMerchantBaseInfo.h"
#include "..\CommonLogic\Msg\MsgType-NPC.h"
#include "..\CommonLogic\MsgType-Item.h"

#include "Statistics/Statistics.h"

#include "../Common/SystemNpc/SystemNPC.h"

#include "../common/VoteMgr/VoteMgr.h"

#include "../Server/ServerReward/ServerReward.h"
#include "../../Common/GameMaster/GameMaster.h"


// NPC에게 물건을 판매. 
void cltServer::DoMsg_GAMEMSG_REQUEST_SELLNPC(cltMsg* pclMsg, SI32 id)
{
	stNPCTradeData* pclinfo	=  (stNPCTradeData*)pclMsg->cData;

	SI32 NPCCharUnique = pclinfo->siNPCCharUnique;
	if ( NPCCharUnique < 1 )
	{
		return;
	}

	SI32 NPCID = pclCM->GetIDFromCharUnique( NPCCharUnique );

	if ( NPCID < 1 )
	{
		return;
	}
	
	if ( pclCM->IsValidID( NPCID ) == false )
	{
		return;
	}
	
	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	if(pclinfo->si32lParam < 0 || pclinfo->si32lParam >= MAX_ITEM_PER_PERSON)return ;

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 팔수 없다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclinfo->si32lParam	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			// 귀속전용 개인상점
			if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->si32lParam	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}
	}

	cltItem* pclfromitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->si32lParam];

	//cyj 탄환류 아이템을 NPC에게 팔수 없슴
	if ( pclfromitem->IsBulletItem() )
	{
		// 거래 실패를 알려준다. 
		cltMsg clMsg( GAMEMSG_RESPONSE_SELLNPC, SYSTEMNPC_FAIL_SELL_BULLETITEM );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		return;
	}

	//영진 
	if ( pclItemManager->IsItemInfoAtb( pclfromitem->siUnique, ITEMINFOATB_TYPE_QUESTITEM) &&
		 pclchar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		// 거래 실패를 알려준다. 
		cltMsg clMsg( GAMEMSG_RESPONSE_SELLNPC, SYSTEMNPC_FAIL_SELL_QUESTITEM );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return ;
	}

	//------------------------------------------------------
	// 파티홀에서 받은 아이템은 판매할 수 없다.
	//------------------------------------------------------
	if( pclfromitem->clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )	
	{
		return;
	}

	// ITEMINFOATB_NOTSELL는 NPC에게 못 판다
	if( pclItemManager->IsItemInfoAtb( pclfromitem->siUnique, ITEMINFOATB_NOTSELL ) )
	{
		// 거래 실패를 알려준다. 
		cltMsg clMsg( GAMEMSG_RESPONSE_SELLNPC, SYSTEMNPC_FAIL_SELL_ITEMMALLITEM );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return ;
	}

	//---------------------------------------------------------------------------------
	// 클라이언트가 보내온 아이템과 서버에서 알고 있는 아이템이 같은 것인지 확인한다. 
	//---------------------------------------------------------------------------------
	cltItem clitem1;
	clitem1.Set(pclfromitem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(&pclinfo->clItem);
	clitem2.SetItemNum(1);

	if(clitem1.IsSame(&clitem2, false) == FALSE)
	{
		return ;
	}

	//------------------------------------------------
	// 아이템을 NPC에게 판매하는 가격을 구한다. 
	//------------------------------------------------
	GMONEY price = pclItemManager->pclItemPrice->GetPriceSellToNPC(pclItemManager, pclinfo->clItem.siUnique);

	// 피로방지 시스템 적용
	price = ((cltCharServer*)pclCM->CR[id])->CalcTiredPoint( price );

	SI32 VillageUnique = pclCM->CR[NPCID]->GetCurrentVillageUnique();
	sDBRequest_SellNPC clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->si32lParam, &pclinfo->clItem, price, VillageUnique, pclCM->CR[NPCID]->GetKind());
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			
	// 판매 금액을 집계한다. 
	cltMoney clmoney;
	clmoney.Set(pclinfo->clItem.siItemNum * price);
	clCurrencyInfo.clInputMoney_NPC.Increase(&clmoney);

	pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_SELLNPC, price);
}

// NPC에게서 물건을 구입. 
void cltServer::DoMsg_GAMEMSG_REQUEST_BUYNPC(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID(id) == false )
	{
		return;
	}

	stNPCTradeData* pclinfo	=  (stNPCTradeData*)pclMsg->cData;

	if(pclinfo->si32lParam < 0 || pclinfo->si32lParam >= MAX_NPC_ITEM_NUMBER)return ;
	
	if ( pclinfo->siNPCCharUnique < 1 )
	{
		return;
	}

	SI32 NPCID = pclCM->GetIDFromCharUnique( pclinfo->siNPCCharUnique );
	if ( NPCID < 1 || pclCM->IsValidID(NPCID) == FALSE )
	{
		return;
	}

	SI32 NPCKind = pclCM->CR[NPCID]->GetKind();

	cltNPCMerchantInfo *pNPCMerchantInfo = NULL;

	for ( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		if ( pclNPCItemInfo->clNPCMerchantInfo[ k ].siNPCKind == NPCKind )
		{
			pNPCMerchantInfo = &pclNPCItemInfo->clNPCMerchantInfo[ k ];
			break;
		}
	}

	if ( pNPCMerchantInfo == NULL )
	{
		return;
	}

	cltItem* pclfromitem = &pNPCMerchantInfo->clItemInfo[pclinfo->si32lParam].clItem;

	// 개수가 무효하면 리턴. 
	if(pclinfo->clItem.siItemNum <= 0)return ;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	// 선거용 유세띠는 유세기간동안만 판매한다
	if ( pclfromitem->siUnique == ITEMUNIQUE(24080) )
	{
		if ( false == bVoteCampaign )
		{
			SendServerResponseMsg( 0, SRVAL_FAIL_DONOTBUY_ELECTIONSTUMPITEM, 0, 0, id );
			return;
		}
	}

	// 클라이언트가 보내온 아이템과 서버에서 알고 있는 아이템이 같은 것인지 확인한다. 
	cltItem clitem1;
	clitem1.Set(pclfromitem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(&pclinfo->clItem);
	clitem2.SetItemNum(1);

	if(clitem1.IsSame(&clitem2, false) == FALSE)
	{
		return ;
	}

	// 중첩이 안되는 아이템의 경우에는 복수개의 주문이 들어와도 한개만 구입할 수 있다. 
	if(pclfromitem->CanPile(pclItemManager) == false)
	{
		pclinfo->clItem.SetItemNum(1);
	}
	

	//cyj 무수리는 1000개까지 살수 있도록 수정
	SI32 MAX_BUY_NUMBER = 300;
	if (pNPCMerchantInfo->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WAITINGWOMAN")) )
	{
		MAX_BUY_NUMBER = 1000;
	}
	// [기형] 발렌타인 데이 이벤트 - 판매점 일 경우
	else if(pNPCMerchantInfo->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHOCOLATE")) )
	{
		pclinfo->clItem.SetItemNum(1);
	}
	
	// 300 개 까지만 구입 가능( 연산 범위 초과 때문에... )
	if ( pclinfo->clItem.siItemNum > MAX_BUY_NUMBER )
	{
		return;
	}

	// 가격을 구해온다. 
	GMONEY price = pNPCMerchantInfo->clItemInfo[pclinfo->si32lParam].siPrice;
	
	// 소지금 부족
	if ( pclCM->CR[id]->pclCI->clIP.GetMoney() < price * pclinfo->clItem.siItemNum )
	{
		// 거래 실패를 알려준다. 
		cltMsg clMsg( GAMEMSG_RESPONSE_BUYNPC, SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		return;
	}

	// [영훈] 대만군주 1주년 기념 반지는 밀봉이 풀려서 나온다
	if ( ITEMUNIQUE(7615) == pclinfo->clItem.siUnique )
	{
		if ( false == pclinfo->clItem.CancellationSealingUp(sTime) )
		{
			return;
		}
	}

	SI16 itempos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &pclinfo->clItem, &itempos, &cltoitem, &rtnvalue) == TRUE)
	{
		// [기형] 발렌타인 데이 이벤트 - 판매점 일 경우
		if(pNPCMerchantInfo->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHOCOLATE")) )
		{
			CValentineEventServerMgr* pclValentineEventServerMgr = (CValentineEventServerMgr*)m_pclValentineEventMgr;
			if( NULL == pclValentineEventServerMgr)
			{
				return;
			}
			else
			{
				if(pclValentineEventServerMgr->IsHolding())
				{
					if(!pclValentineEventServerMgr->BuyPresentToStore(pclinfo->siNPCCharUnique, pclinfo->clItem.siUnique))
					{
						cltMsg clMsg( GAMEMSG_RESPONSE_BUYNPC, SYSTEMNPC_FAIL_BUY_SOLDOUT );
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

						return;
					}
				}
			}
		}

		sDBRequest_BuyNPC clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), &pclinfo->clItem, pclinfo->clItem.CanPile(pclItemManager), price, itempos, NPCKind);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		// 구입 금액을 집계한다. 
		cltMoney clmoney;
		clmoney.Set(pclinfo->clItem.siItemNum * price);
		clCurrencyInfo.clOutputMoney_NPC.Increase(&clmoney);

        pclDelMoneyStatistics->Add(DELMONEY_BUYNPC, pclinfo->clItem.siItemNum * price);
	}
	else
	{
		if(pclchar)
		{
			SendServerResponseMsg(0, SRVAL_NPC_MARKET_SELL_FAIL,  0, 0, pclchar->GetCharUnique());
		}
		
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_NPCSELLITEMINFO(cltMsg* pclMsg, SI32 id)
{
	if ( pclItemManager->clItemStatistic.siNPCPriceRate == 0 )
	{
		return;
	}
	
	cltGameMsgRequest_NPCSellItemInfo* pclinfo = (cltGameMsgRequest_NPCSellItemInfo*)pclMsg->cData;

	if ( pclinfo->siNPCKind < 1 )
	{
		return;
	}
	
	cltNPCMerchantInfo *pNPCMerchantInfo = NULL;

	for ( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		if ( pclNPCItemInfo->clNPCMerchantInfo[ k ].siNPCKind == pclinfo->siNPCKind )
		{
			pNPCMerchantInfo = &pclNPCItemInfo->clNPCMerchantInfo[ k ];
			break;
		}
	}

	if ( pNPCMerchantInfo == NULL )
	{
		return;
	}

	cltGameMsgResponse_NPCSellItemInfo clNPCSellItemInfo( pclinfo->siNPCKind, &pNPCMerchantInfo->clItemInfo[0] );
		
	cltMsg clMsg( GAMEMSG_RESPONSE_NPCSELLITEMINFO, sizeof( clNPCSellItemInfo ), (BYTE*)&clNPCSellItemInfo );

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SELLTOFOREIGNMERCHANT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SellToForeignMerchant* pclinfo = (cltGameMsgRequest_SellToForeignMerchant*)pclMsg->cData;
	
	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	SI32 NPCCharUnique = pclinfo->siNPCCharUnique;
	if ( NPCCharUnique < 1 )
	{
		return;
	}

	SI32 NPCID = pclCM->GetIDFromCharUnique( NPCCharUnique );

	if ( NPCID < 1 )
	{
		return;
	}
	
	if ( pclCM->IsValidID( NPCID ) == false )
	{
		return;
	}

	if ( pclinfo->siSellItemUnique < 1 )
	{
		return;
	}

	// 거래할 아이템의 개수가 유효한지 확인. 
	if ( pclinfo->uiSellItemNum < 1 )
	{
		return;
	}

	// 1000 개 개수 제한
	if ( pclinfo->uiSellItemNum > 1000 )
	{
		return;
	}
	
	// 현재, NPC 중에 있는 NPC 인지 체크.
	if( pclinfo->siForeignMerchantKind < FOREIGNMERCHANT_HELMET || pclinfo->siForeignMerchantKind >= FOREIGNMERCHANT_END )
	{
		return;
	}

	//  인벤토리 내에 있는 아이템 위치인지 확인
	if( pclinfo->uiSelectedIndex < PERSONITEM_INV0 || pclinfo->uiSelectedIndex >= MAX_ITEM_PER_PERSON )
	{
		return ;
	}

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 팔수 없다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclinfo->uiSelectedIndex	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}

		// 귀속전용 개인상점
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->uiSelectedIndex	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}

	// 현재 NPC가 구입하는 물건이 클라이언트에서 온 정보와 합당하는지 확인. 
	// 지금 교역중인 NPC 의 교역에 관련된 정보를 받아온다.
	stForeignMerchantNPCInfo* pclForeignMerchantNPCInfo = pclForeignMerchantNPCMgr->GetForeignMerchantNPCInfo( pclinfo->siForeignMerchantKind );
	
	if ( pclForeignMerchantNPCInfo == NULL )
	{
		return;
	}

	CForeignMerchantNPC *pclForeignMerchantNPC = pclForeignMerchantNPCMgr->GetForeignMerchantNPC( pclinfo->siForeignMerchantKind );

	if ( pclForeignMerchantNPC == NULL )
	{
		return;
	}	

	// 실제로 사용자의 인벤에 해당  물품이 있는지 확인(수량도 확인.)
	cltItem* pclfromitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->uiSelectedIndex ];

	if( pclfromitem == NULL )
	{
		return;
	}

	// 아이템 유니크 값 비교
	// 현재 유저의 인벤토리에 있는 아이템 개수랑 클라이언트에서 보내온 아이템 개수가 같은지 비교
	// 판매할려는 개수가 1보다 작지는 않은지... 이 경우는 올 수도 없지만... 혹시나...
	if( pclfromitem->siUnique != pclinfo->siSellItemUnique ) 
	{
		return;
	}
	
	// 유저가 팔려는 아이템 개수 확인
	if ( pclfromitem->siItemNum < pclinfo->uiSellItemNum )
	{
		return;
	}

	//------------------------------------------------------
	// 파티홀에서 받은 아이템은 판매할 수 없다.
	//------------------------------------------------------
	if( pclfromitem->clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )	return;

	SI32 remainDay = 0;
	
	BOOL rVal = GetItemUseAbleDay( pclfromitem, &remainDay );
	
	if (remainDay < 0 && rVal == TRUE)
		return;

	// 상인이 사는 물품중에 있는 것인지 그것이 유저가 파는 물건과 같은 것인지 확인
	SI32 siBuyItemIndex = -1;
	for ( SI32 i = 0; i < pclForeignMerchantNPCInfo->siBuyItemListCount; ++i )
	{
		if ( pclForeignMerchantNPCInfo->siBuyItemUniqueList[ i ] == pclinfo->siSellItemUnique )
		{
			siBuyItemIndex = i;
			break;
		}
	}

	// 구입하는 물품이 일치하지 않음.
	if ( siBuyItemIndex == -1)
	{
		return;
	}

	// 중첩이 안되는 아이템의 경우에는 복수개의 주문이 들어와도 한개만 구입함
	if(pclfromitem->CanPile(pclItemManager) == false)
	{
		pclinfo->uiSellItemNum = 1;
	}
	
	pclForeignMerchantNPC->RefreshPrice();

	//-------------------------------
	// 시세를 업데이트 한다. 
	//------------------------------
	GMONEY price = 0;

	// 명품, 호품, 신기 일때는 아이템의 현재 시세를 갖고온다
	if ( (pclinfo->uiRareInfo == RARELEVEL_2) || (pclinfo->uiRareInfo == RARELEVEL_1) || (pclinfo->uiRareInfo == RARELEVEL_5) )
	{
		price = (pclItemManager->GetCurrentPrice( pclinfo->siSellItemUnique ))->siPrice ;
	}
	// 일반 아이템의 경우 시세의 50%~75만큼의 가격을 갖고온다
	else
	{
		price = pclForeignMerchantNPCInfo->siBuyItemPriceList[ siBuyItemIndex ];
	}
	
	
	if ( pclinfo->uiRareInfo == RARELEVEL_2 )
	{
		price *=  10;			// 아이템 시세의 10배( 명품 )
	}
	else if ( pclinfo->uiRareInfo == RARELEVEL_1 )
	{
		price *=  5;			// 아이템 시세의 3배( 호품 )
	}
	//[추가 : 황진성 2007. 12. 5 신기물품은 시세의 2배.]
	else if ( pclinfo->uiRareInfo == RARELEVEL_5 )
	{
		price *= 2;			// 아이템 시세의 2배( 신기 )
	}

	// 글로벌이 아니면 적용전 아이템 판매 가격로 적용.
	if(!pclClient->IsCountrySwitch(Switch_SellToForeignMerchant))
	{	
		if ( pclinfo->uiRareInfo == RARELEVEL_1 )
		{
			price /= 5;			// 아이템 시세의 5배를 풀고 3배( 호품 )
			price *= 3;
		}
		//[추가 : 황진성 2007. 12. 5 신기물품은 시세의 2배.]
		else if ( pclinfo->uiRareInfo == RARELEVEL_5 )
		{
			price /= 2;			// 아이템 시세의 2배를 푼다.( 신기 )
		}
	}

	// DB에 물품을 팔라고 요청..
	cltItem clItem;
	clItem.Set(pclfromitem);
	clItem.SetItemNum(pclinfo->uiSellItemNum);
	if (pclfromitem->IsSealingUp(pclItemManager) == false)
	{
		if( rVal == TRUE) // 기간제 아이템이라면
		{
			switch (remainDay)
			{
			case 2 :
				price = price * 5 / 10;
				break;
			case 1 :
				price = price * 3 / 10;
				break;
			case 0 :
				{
					if(clItem.uiDateUseDay != 0)
						price = price * 1 / 10;
				}
				break;
			}
		}
	}

	// 내구도 사용시 
	SI16 siDurabilityPenalty = 0;

	if ( IsCountrySwitch(Switch_NewDurability) )
	{
		REAL32 fGetDurabilityPenalty = pclfromitem->GetDurabilityPenlty( pclItemManager );

		price				= (GMONEY)((REAL64)price * (REAL64)fGetDurabilityPenalty);
		siDurabilityPenalty	= (SI16)(fGetDurabilityPenalty * 100.0f);
	}

	//KHY - 0530 명나라 상인 구매제한.
	if(gForeignMerchantBuyLimit > 0) // 구매제한이 설정되어 있다면.
	{
		// 하루 6번 구매제한을 한다. 제한 시 단계가 있는데 제한이 60원이면 1회차에 10원, 2회차엔 20원 제한금액이 변경되는 
		// 방식이다. 그리고 판매하는 아이템의 금액이 제한금액을 초과하는 경우는 1 ~ 5회차까지 제한된다.
		// 이유는 다음 회차에 영향을 주기 때문이다. 하지만 구매누적금액을 초기화하기 전인 6회차의 경우엔 아이템의 가격에 의한 
		// 제한을 없이 판매가 되도록 했다.
		SI32 siDay	  = pclTime->clDate.GetDay();
		SI32 siSeason = 1;
			 if(  1 <= siDay &&  5 >= siDay ) siSeason = 1;	
		else if(  6 <= siDay && 10 >= siDay ) siSeason = 2;
		else if( 11 <= siDay && 15 >= siDay ) siSeason = 3;
		else if( 16 <= siDay && 20 >= siDay ) siSeason = 4;
		else if( 21 <= siDay && 25 >= siDay ) siSeason = 5;
		else if( 26 <= siDay && 30 >= siDay ) siSeason = 6;

		if((gForeignMerchantBuyLimit / 6) * siSeason <= gForeignMerchantNowMoney + ((6 == siSeason) ? 0 : price) )
		{
			SendServerResponseMsg(0, SRVAL_CANNOTBUYITEM_FOREIGNMERCHANT, 0, 0, id );
			return ;
		}
		else
		{
			// 구매한 액수를 저장한다.
			gForeignMerchantNowMoney = gForeignMerchantNowMoney + price;

			if ( gForeignMerchantNowMoney > gForeignMerchantBuyLimit ) 
			{
				gForeignMerchantNowMoney = gForeignMerchantBuyLimit ;
			}
			//sDBRequest_SetForeignMerchantNowMoney clMsg(pclinfo->siForeignMerchantKind, gForeignMerchantNowMoney);
			sDBRequest_SetForeignMerchantNowMoney clMsg( gForeignMerchantNowMoney);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);		
		}
	}

	if(price > 0)
	{
		pclItemManager->SetCurrentPrice(pclfromitem->siUnique, price, pclinfo->uiSellItemNum);
		
		cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(pclfromitem->siUnique);
		
		if(pclprice)
		{
			const TCHAR* pszItemName = pclClient->pclItemManager->GetName(pclprice->siUnique);
			sDBRequest_SetItemPrice clMsg(pclprice->siUnique, pclprice->siPrice, pclprice->fPrice, pclprice->siItemNum, pszItemName);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 4] // 가격을 설정.
			//	// param1 == 아이템 유니크. param2 == 가격, param3 == 각격, param4 == 개수, pszParam1 == 아이템 이름.
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_ITEMPRICESET, 
			//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			//									pclprice->siUnique, pclprice->siPrice, pclprice->fPrice, pclprice->siItemNum, 0, (TCHAR*)pszItemName, NULL);
			//}
		}
	}


	// 피로방지 시스템 적용
	price = pclchar->CalcTiredPoint( price );

	SI32 VillageUnique = pclCM->CR[NPCID]->GetCurrentVillageUnique();
	sDBRequest_SellNPC clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->uiSelectedIndex, &clItem, price, VillageUnique, pclCM->CR[NPCID]->GetKind());
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_NPCBUYITEM, price);
	
	// 판매 금액을 집계한다. 
	cltMoney clmoney;
	clmoney.Set(clItem.siItemNum * price);
	clCurrencyInfo.clInputMoney_NPC.Increase(&clmoney);
	
	pclStatisticsManager->clDailyStatistics.siForeignMerchantBuyGameItemMoney += clmoney.itMoney;

	pclItemManager->AddForeignMerchantItemNum( clItem.siUnique, clItem.siItemNum );

	// 새로운 통계 - 명나라 상인에 의해 사들여진 물품의 통계를 낸다.
	NStatistics_Add("Item", clItem.siUnique ,"ForeignMerchant", clItem.siItemNum);
	NStatistics_Add("Item", clItem.siUnique ,"ForeignMerchantPrice", clmoney.itMoney);
	NStatistics_Set("Item", clItem.siUnique, "Name", pclItemManager->GetName(clItem.siUnique));

	// 퀘스트 정보를 업데이트 한다.
	((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_SELLTRADER, clItem.siUnique  , clItem.siItemNum );

	//[추가중 : 황진성 2007. 11. 19 명나라 상인의 구입 누적 정보.]

	UI16  RareLevelType;
	switch(pclinfo->uiRareInfo)
	{
		case RARELEVEL_1: 	// 호품
		case RARELEVEL_2: 	// 명품	
		case RARELEVEL_3: 	
		case RARELEVEL_4: 	
		case RARELEVEL_5: RareLevelType = pclinfo->uiRareInfo;	break;	// 고급품.
		case RARELEVEL_6:								// 고물품.(일반과 같게.)
		default:		  RareLevelType = 6;	break;  // 일반품.
	}
	
	// 배열의 번호. (-1)
	SI32 Index = RareLevelType - 1;
	
	clItemInfo[Index].m_sTime    = sTime;				// 현재 시간.
	clItemInfo[Index].m_siMoney += clmoney.itMoney;		// 거래 금액 누적.
	clItemInfo[Index].m_siCnt++;						// 횟수 누적.
	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 11. 19 명나라 상인과의 거래내역. 통계를 위한...]
		/*
			RareLevelType => 

		
		*/
		// param1 == 명나라 상인이 구입한 금액.
		// param2 == 누적 거래 횟수.
		// param3 == 누적액.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SELL_TO_FOREIGN_MERCHANT, RareLevelType, 
							 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &clItem, 0, 0, 0, 0, 0, 0, 
							 clmoney.itMoney, clItemInfo[Index].m_siCnt, clItemInfo[Index].m_siMoney, 0, 0, 
							 NULL, NULL);
		
	}

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRESELLTOFOREIGNMERCHANT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SellToForeignMerchant* pclinfo = (cltGameMsgRequest_SellToForeignMerchant*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// 실제로 사용자의 인벤에 해당  물품이 있는지 확인(수량도 확인.)
	cltItem* pclfromitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->uiSelectedIndex ];

    if( pclfromitem == NULL )
	{
		return;
	}

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 팔수 없다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclinfo->uiSelectedIndex	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}

		// 귀속전용 개인상점
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->uiSelectedIndex	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}

	stForeignMerchantNPCInfo* pclForeignMerchantNPCInfo = pclForeignMerchantNPCMgr->GetForeignMerchantNPCInfo( pclinfo->siForeignMerchantKind );

	if ( pclForeignMerchantNPCInfo == NULL )
	{
		return;
	}

	CForeignMerchantNPC *pclForeignMerchantNPC = pclForeignMerchantNPCMgr->GetForeignMerchantNPC( pclinfo->siForeignMerchantKind );

	if ( pclForeignMerchantNPC == NULL )
	{
		return;
	}	

	pclForeignMerchantNPC->RefreshPrice();

	// 상인이 사는 물품중에 있는 것인지 그것이 유저가 파는 물건과 같은 것인지 확인
	SI32 siBuyItemIndex = -1;
	for ( SI32 i = 0; i < pclForeignMerchantNPCInfo->siBuyItemListCount; ++i )
	{
		if ( pclForeignMerchantNPCInfo->siBuyItemUniqueList[ i ] == pclinfo->siSellItemUnique )
		{
			siBuyItemIndex = i;
			break;
		}
	}

	// 구입하는 물품이 일치하지 않음.
	if ( siBuyItemIndex == -1)
	{
		return;
	}

	GMONEY price = 0;

	// 명품, 호품, 신기 일때는 아이템의 현재 시세를 갖고온다
	if ( (pclinfo->uiRareInfo == RARELEVEL_2) || (pclinfo->uiRareInfo == RARELEVEL_1) || (pclinfo->uiRareInfo == RARELEVEL_5) )
	{
		price = (pclItemManager->GetCurrentPrice( pclinfo->siSellItemUnique ))->siPrice ;
	}
	// 일반 아이템의 경우 시세의 50%~75만큼의 가격을 갖고온다
	else
	{
		price = pclForeignMerchantNPCInfo->siBuyItemPriceList[ siBuyItemIndex ];
	}
		
	
	if (  pclfromitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 )
	{
		price *=  10;			// 아이템 시세의 10배( 명품 )
	}
	else if (  pclfromitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )
	{
		price *=  5;			// 아이템 시세의 3배( 호품 )
	}
	//[추가 : 황진성 2007. 12. 5 신기물품은 시세의 2배.]
	else if ( pclfromitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
	{
		price *= 2;			// 아이템 시세의 2배( 신기 )
	}
	
	// 글로벌이 아니면 적용전 아이템 판매 가격로 적용.
	if(!pclClient->IsCountrySwitch(Switch_SellToForeignMerchant))
	{	
		if ( pclinfo->uiRareInfo == RARELEVEL_1 )
		{
			price /= 5;			// 아이템 시세의 5배를 풀고 3배( 호품 )
			price *= 3;
		}
		//[추가 : 황진성 2007. 12. 5 신기물품은 시세의 2배.]
		else if ( pclinfo->uiRareInfo == RARELEVEL_5 )
		{
			price /= 2;			// 아이템 시세의 2배를 푼다.( 신기 )
		}
	}
	
	SI32 remainDay = 0;
	BOOL rVal = GetItemUseAbleDay( pclfromitem , &remainDay );
	SI16 percent = 0;

	if (pclfromitem->IsSealingUp(pclItemManager) == false)
	{
		if( rVal == TRUE && remainDay == 0 ) //일반품이다.
		{
			percent = 100;
		}
		else if( rVal == TRUE && remainDay >0 )
		{

			switch (remainDay)
			{
			case 2 :
				percent = 50;
				price = price * 5 / 10;
				break;
			case 1 :
				percent = 30;
				price = price * 3 / 10;
				break;
			case 0 :
				percent = 10;
				price = price * 1 / 10;
				break;
			default:
				percent = 100;
				break;
			}			
		}
		else if(remainDay < 0)
		{
			percent = 0;
			price = 0;
		}
	}

	// 내구도 사용시 
	SI16 siDurabilityPenalty = 0;

	if ( IsCountrySwitch(Switch_NewDurability) )
	{
		REAL32 fGetDurabilityPenalty = pclfromitem->GetDurabilityPenlty( pclItemManager );

		price				= (GMONEY)((REAL64)price * (REAL64)fGetDurabilityPenalty);
		siDurabilityPenalty	= (SI16)(fGetDurabilityPenalty * 100.0f);
	}

	cltGameMsgResponse_PreSellToForeignMerchant clPreSellInfo(remainDay, percent, price, pclfromitem->clItemCommon.clCommonInfo.uiRareInfo, siDurabilityPenalty);

	cltMsg clMsg( GAMEMSG_RESPONSE_PRESELLTOFOREIGNMERCHANT, sizeof( clPreSellInfo ), (BYTE*)&clPreSellInfo );

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_FOREIGNMERCHANTNPCBUYINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_ForeignMerchantNPCBuyInfo* pclinfo = (cltGameMsgRequest_ForeignMerchantNPCBuyInfo*)pclMsg->cData;

	// 현재, 가능한 것인지 체크
	if( pclinfo->siForeignMerchantKind < FOREIGNMERCHANT_HELMET || pclinfo->siForeignMerchantKind >= FOREIGNMERCHANT_END )
	{
		return;
	}
	
	CForeignMerchantNPC *pForeignMerchantNPC = pclForeignMerchantNPCMgr->GetForeignMerchantNPC( pclinfo->siForeignMerchantKind );

	if ( pForeignMerchantNPC == NULL )
	{
		return;
	}

	if ( ! pForeignMerchantNPC->IsAvailable() )
	{
		return;
	}

	// 지금 교역중인 NPC 의 교역에 관련된 정보를 받아온다.
	stForeignMerchantNPCInfo* pclForeignMerchantNPCInfo = pForeignMerchantNPC->GetForeignMerchantNPCInfo();

	// 정보가 이상한 경우...
	if ( pclForeignMerchantNPCInfo == NULL )
	{
		return;
	}
	
	cltGameMsgResponse_ForeignMerchantNPCBuyInfo clBuyInfo;
	
	clBuyInfo.Set
		(
		pclForeignMerchantNPCInfo->siForeignMerchantKind,
		pclForeignMerchantNPCInfo->siBuyItemType,
		&pclForeignMerchantNPCInfo->siBuyItemUniqueList[0],
		&pclForeignMerchantNPCInfo->siBuyItemPriceList[0],
		pclForeignMerchantNPCInfo->siBuyItemListCount
		);
	
	cltMsg clMsg( GAMEMSG_RESPONSE_FOREIGNMERCHANTNPCBUYINFO, sizeof( clBuyInfo ), (BYTE*)&clBuyInfo );

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

// 유저가 교역 상인 NPC 가 구매 중인 캐쉬 아이템이 무엇인지 요청( 종류별로 )
void cltServer::DoMsg_GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEMINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_ForeignMerchantBuyCashItemInfo * pclinfo = (cltGameMsgRequest_ForeignMerchantBuyCashItemInfo*)pclMsg->cData;

	if ( pclinfo->siItemType < 1 || pclinfo->siItemType >= MAX_ITEMTYPE_NUMBER )
	{
		return;
	}

	SI16 ItemUniqueList[ MAX_AVAILABLE_CASHITEMNUM ];
	UI16 ItemPriceList[ MAX_AVAILABLE_CASHITEMNUM ];
	
	SI16 Counter = pclForeignMerchantNPCBuyCashItem->GetItemUniqueAndPriceListFromItemType( pclinfo->siItemType, ItemUniqueList, ItemPriceList );

	if ( Counter < 1 )
	{
		return;
	}

	cltGameMsgResponse_ForeignMerchantBuyCashItemInfo clGameMsgResponse_ForeignMerchantBuyCashItemInfo;
	clGameMsgResponse_ForeignMerchantBuyCashItemInfo.Set( pclinfo->siItemType, ItemUniqueList, ItemPriceList, Counter );
	cltMsg clMsg( GAMEMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEMINFO, clGameMsgResponse_ForeignMerchantBuyCashItemInfo.GetTotalSize(), (BYTE*)&clGameMsgResponse_ForeignMerchantBuyCashItemInfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	
}

// 유저가 교역 상인에게 캐쉬 아이템 판매
void cltServer::DoMsg_GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	// 1초에 한번만 이용이 가능하다.
	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}
	
	/*
	// SBS 회원은 판매 불가
	if ( pclCM->CR[id]->IsSBS() )
	{
		SendServerResponseMsg(0, SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_SBS,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}
	*/

	cltGameMsgRequest_ForeignMerchantBuyCashItem * pclinfo = (cltGameMsgRequest_ForeignMerchantBuyCashItem*)pclMsg->cData;

	if ( pclinfo->siItemPos < PERSONITEM_INV0 || pclinfo->siItemPos >= PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique < 1 ||	
		pclinfo->clSellItem.siItemNum < 1 ||
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique != pclinfo->clSellItem.siUnique ||
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siItemNum < pclinfo->clSellItem.siItemNum
		)
	{
		return;
	}

	//------------------------------------------------------
	// 파티홀에서 받은 아이템은 판매할 수 없다.
	//------------------------------------------------------
	if( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
		return;


	// 구입중인 아이템인지 확인
	UI16 ItemOnePrice = 0;
	if ( ! pclForeignMerchantNPCBuyCashItem->GetItemPrice( pclinfo->clSellItem.siUnique, &ItemOnePrice ) )
	{
		return;
	}

	if ( ItemOnePrice < 1 || ItemOnePrice > 60000 )
	{
		SendServerResponseMsg(0, SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_PRICE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		
		return;
	}

	// 아이템 가격 * 판매 개수
	UI32 ItemPrice = ItemOnePrice * pclinfo->clSellItem.siItemNum;

	if ( ItemPrice < 1 || ItemPrice > 60000 )
	{
		SendServerResponseMsg(0, SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_PRICE,  0, 0, pclCM->CR[id]->GetCharUnique() );

		return;
	}

	//cyj 아이템몰에서 산 아이템만 못 팔도록 수정
	//if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].uiDateUseDay > 0)
	if ( pclItemManager->IsItemInfoAtb( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique, ITEMINFOATB_NOTTRADE) )
	{
		return ;
	}
	

	sDBRequest_ForeignMerchantBuyCashItem clMsg( id, pclCM->CR[id]->pclCI->GetPersonID(), pclCM->CR[id]->pclCI->clBI.szAccountID, 0, 0, 0, pclinfo->siItemPos, &pclinfo->clSellItem, ItemPrice );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclStatisticsManager->clDailyStatistics.siForeignMerchantBuyCashItemMoney += ItemPrice;

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GETRECOMMANDPOINT(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->IsValidID( id ) == false )
	{
		return;
	}


	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0 )	return ;
	

	sDBRequest_GetRecommendPoint clMsg( id, pclCM->CR[id]->pclCI->GetPersonID() );
	pclGameDBProcess->SendMsg( (sPacketHeader*)&clMsg);

}



// 추천 포인트로 아이템을 구입
void cltServer::DoMsg_GAMEMSG_REQUEST_BUYRECOMMANDPRODUCT(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->IsValidID( id ) == false )
	{
		return;
	}

	cltGameMsgRequest_BuyRecommandProduct * pclinfo = (cltGameMsgRequest_BuyRecommandProduct*)pclMsg->cData;
    
	/////////////////////////////////////////////////////////////
	SI32 ProductIndex = pclinfo->siRecommandProductIndex;
	
	if( ProductIndex < 0 || ProductIndex >= MAX_RECOMMAND_PRODUCT )
		return;

	// 서버와 클라이언트의 상품 정보가 다르면 리턴
	stRecommandProduct* sRecommandProduct = &pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[ProductIndex];

	if(sRecommandProduct->ItemUnique <= 0) 
		return;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( sRecommandProduct->ItemUnique );
	
	if( ref <= 0) 
		return;

	cltItem clItem	= pclClient->pclItemManager->pclItemInfo[ref]->clItem;

	clItem.SetItemNum( sRecommandProduct->ItemNum  );
	clItem.uiDateUseDay = sRecommandProduct->ItemUseDate;

	if( clItem.IsSame(&pclinfo->sRecommandProduct,true) == false )
		return;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	// 필요 포인트를 구해온다. 
	SI32 NeedPoint = sRecommandProduct->NeedPoint;

	if(NeedPoint <= 0 ) 
		return;
	
	SI16 itempos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &pclinfo->sRecommandProduct, &itempos, &cltoitem, &rtnvalue) == TRUE)
	{
		sDBRequest_BuyRecommendProduct clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), &pclinfo->sRecommandProduct, pclinfo->sRecommandProduct.CanPile(pclItemManager), NeedPoint, itempos);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		if(pclchar)
		{
			//SendServerResponseMsg(0, SRVAL_NPC_MARKET_SELL_FAIL,  0, 0, pclchar->GetCharUnique());
		}
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_RECOMMANDPOINTLIST(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgRequest_RecommandPointList * pclinfo = (cltGameMsgRequest_RecommandPointList*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )
	{
		return;
	}

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	sDBRequest_RecommendPointList clMsg(id, pclCM->CR[id]->pclCI->GetPersonID()  , pclinfo->siCurrentPage);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_MANUFACTURENPC( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_ManufactureNPC* pclinfo = (cltGameMsgRequest_ManufactureNPC*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )
		return;

	if( pclCM->IsAlive( id ) == false )
		return;

	if( pclCM->CR[id]->CheckLastMsgClock(1000) == true )
		return;

	// TODO : 수수료가 있는지 확인한다.
	// 1. 수수료 계산 아이템의 시세를 이용
	// 2. 소지금과 비교

	cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(pclinfo->siItemUnique);
	
	GMONEY fee = pclprice->siPrice / 10;

	if ( pclCM->CR[id]->pclCI->clIP.GetMoney() < fee  )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_MANUFATURENPC_NOMONEY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

		cltGameMsgRequest_MakeItemInv clinfo( pclinfo->siItemUnique, 1, 0,0,PSHOP_MODE_NONE, 0, true );	// NPC가 만든다 
	MakeItemProcess( id, &clinfo );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_RECEIVEBEGINNERITEM(cltMsg* pclMsg, SI32 id )
{
	// TODO 
	// 이미 초보자 아이템을 받았는지 체크 하고!
	// 아이템 넣어준다!

	if( pclCM->IsValidID( id ) == false )
		return;

	if( pclCM->IsAlive( id ) == false )
		return;

	if( pclCM->CR[id]->CheckLastMsgClock(1000) == true )
		return;

	cltGameMsgRequest_RecieveBeginnerItem* pclinfo = 
		(cltGameMsgRequest_RecieveBeginnerItem*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if ( pclchar->pclCI->clBI.bReceiveBeginnerWeapon )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_ALEADYRECEIVE_BEGINNERWEAPON,  0, 0, id);
		return;
	}

	pclchar->GiveItem( pclinfo->siItemUnique, 1, INPUTITEMPERSON_OPTION_BEGINNERWEAPON, 0 );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PROMOTER_INFO_TOTAL_GET(cltMsg* pclMsg, SI32 id )
{
	cltGameRequest_Promoter_Info_Total_Get* pclinfo = (cltGameRequest_Promoter_Info_Total_Get*)pclMsg->cData;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if( NULL == pclChar ) return;

	clPromoterInfo PromoterInfo( id, clPromoterInfo::GET_PROMOTER_INFO_TOTAL );
	pclPromoterManager->Push( PromoterInfo );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PROMOTER_INFO_GET(cltMsg* pclMsg, SI32 id )
{
	/*cltGameRequest_Promoter_Info_Get* pclinfo = (cltGameRequest_Promoter_Info_Get*)pclMsg->cData;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if( NULL == pclChar ) return;

	clPromoterInfo PromoterInfo( id, clPromoterInfo::GET_PROMOTER_INFO );
	pclPromoterManager->Push( PromoterInfo );*/
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PROMOTER_URL_GET(cltMsg* pclMsg, SI32 id )
{
	cltGameRequest_Promoter_URL_Get* pclinfo = (cltGameRequest_Promoter_URL_Get*)pclMsg->cData;
	
	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if( NULL == pclChar ) return;

	clPromoterInfo PromoterInfo( id, clPromoterInfo::GET_PROMOTER_URL );
	pclPromoterManager->Push( PromoterInfo );
}


void cltServer::DoMsg_GAMEMSG_REQUEST_PROMOTERNPC_GIVE_ITEM_GET(cltMsg* pclMsg, SI32 id )
{
	cltGameRequest_PromoterNPC_Give_Item_Get* pclinfo = (cltGameRequest_PromoterNPC_Give_Item_Get*)pclMsg->cData;
	
	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if( NULL == pclChar ) return;

	// 획득할수 있는 아이템 정보 주기
	cltRewardItem**	pcltRewardItemList = pclRewardMgr->GetRecommendRewardItemList();
	if( NULL == pcltRewardItemList ) return;

	cltGameResponse_PromoterNPC_Give_Item_Get clInfo;

	for( SI32 siCount = 0; siCount < Const_Max_RecommandReward_Item; ++siCount )
	{
		if( NULL == pcltRewardItemList[siCount] ) continue;

		clInfo.ItemUnique[siCount]	= pcltRewardItemList[siCount]->GetItemUnique();
		clInfo.ItemNum[siCount]		= pcltRewardItemList[siCount]->GetItemNum();
	}

	cltMsg clMsg2(GAMEMSG_RESPONSE_PROMOTERNPC_GIVE_ITEM_GET, sizeof(clInfo), (BYTE*)&clInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MONSTERCREATE(cltMsg* pclMsg, SI32 id)
{
	cltGameRequest_MonsterCreate* pclinfo = (cltGameRequest_MonsterCreate*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	SI32 ref = pclGMManager->FindUniqueFromOrder(TEXT("cmt"));
	SI32 gamemasterlevel = pclchar->GetGameMasterLevel();

	if(ref < 1 || ref >= MAX_GMINFO_NUMBER)	return;

	// 운영자 권한이 없으면 실행 못한다.
	if(gamemasterlevel < pclGMManager->pclGMInfo[ref]->siNeedGMLevel)	return;

	SI32 siMonsterkind = pclinfo->m_siMonsterKind;
	SI32 siMonCreateNum = pclinfo->m_siMonCreateNum;
	SI32 siMonLevel = pclinfo->m_siMonLevel;

	if(siMonsterkind == 0)	return;

	SI32 CreatedMonsterNum = 0;
	SI32 count = 0;

	cltGMCommandKindInfo* pclGMCommandKI;
	pclGMCommandKI = pclKindInfoSet->pclGMCommandKindInfo[siMonsterkind];

	cltKindInfo* pclki = NULL;
	pclki = pclKindInfoSet->pclKI[siMonsterkind];
	
	pclGMCommandKI->siAggressive = pclinfo->m_siAggro;
	pclGMCommandKI->clDefaultWeaponInfo.Set(pclki->clDefaultWeaponInfo.GetAttackType(), pclki->clDefaultWeaponInfo.GetAttackRange(), pclinfo->m_siMinDmg, pclinfo->m_siMaxDmg, pclinfo->m_siMonAC);
	pclGMCommandKI->clBA.SetStr(pclinfo->m_siMonStr);
	pclGMCommandKI->clBA.SetDex(pclinfo->m_siMonDex);
	pclGMCommandKI->clBA.SetVit(pclinfo->m_siMonVit);
	pclGMCommandKI->clBA.SetMag(pclinfo->m_siMonIntl);
	pclGMCommandKI->clBA.SetHnd(pclinfo->m_siMonHnd);
	pclGMCommandKI->clBA.SetLuk(pclinfo->m_siMonLuk);
	pclGMCommandKI->clBA.SetWis(pclinfo->m_siMonWis);
	pclGMCommandKI->siCriticalRate = pclinfo->m_siCriRate;
	pclGMCommandKI->dwAttackInterval = pclinfo->m_siAttInter;
	pclGMCommandKI->clCharElemental.Set(pclinfo->m_siFire, pclinfo->m_siWater, pclinfo->m_siLand, pclinfo->m_siWind, pclinfo->m_siNature, 
		pclki->pclCharElemental.GetFireA(), pclki->pclCharElemental.GetWaterA(), pclki->pclCharElemental.GetLandA(), pclki->pclCharElemental.GetWindA());

	// 운영자명령어 로그에 남긴다. (생성한 몬스터의 이름, 마리수는 표시하고 나머지 정보는 그냥 순서대로 데이터만 찍는다.
	// 데이터 순서 : 최소데미지, 최대데미지, 방어력, 힘, 민첩, 생명력, 마력, 손재주, 행운, 지혜, 불저항력, 물저항력, 땅저항력, 바람저항력, 물리저항력
	pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use cmt : MonsterName : [%s], MonsterNum : [%d], [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] "), 
		pclCM->CR[id]->GetName(), pclki->GetName(), pclinfo->m_siMonCreateNum, pclinfo->m_siMinDmg, pclinfo->m_siMaxDmg, pclinfo->m_siMonAC, pclinfo->m_siMonStr, pclinfo->m_siMonDex, pclinfo->m_siMonVit, pclinfo->m_siMonIntl,
		pclinfo->m_siMonHnd, pclinfo->m_siMonLuk, pclinfo->m_siMonWis, pclinfo->m_siFire, pclinfo->m_siWater, pclinfo->m_siLand, pclinfo->m_siWind, pclinfo->m_siNature);

	for(SI32 i = 0; i < siMonCreateNum; i++)
	{
		SI32 x, y;
		if(pclchar->pclMap->FindEmptyAreaForCharLocate(siMonsterkind, pclchar->GetX(), pclchar->GetY(), 20, &x, &y, true) == false)
		{
			continue;
		}

		pclchar->SetInitProtectSwitch();

		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

		SI32 siSummonID = SetNPCChar(siMonsterkind, VILLAGE_NONE, NULL, siMonLevel, 0, x, y, pclchar->GetMapIndex(), NPCITEMMODE_GM, &clinnerstatus, NULL, pclGMCommandKI);

		if(siSummonID > 0)
		{
			SendServerResponseMsg( 0, SRVAL_RETURN_MONCREATE_DLG_SUCCESS, 0, 0, id );
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_ITEM_PACKING(cltMsg* pclMsg,SI32 id)
{
	cltGameRequest_ValentineDay_Item_Packing* pclinfo = (cltGameRequest_ValentineDay_Item_Packing*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	if( 0 >= pclinfo->m_clPackingItem.siItemNum )																		  return;
	if( pclinfo->m_clPackingItem.siItemNum > pclchar->pclCI->clCharItem.GetItemNum( pclinfo->m_clPackingItem.siUnique ) ) return;

	bool bResult = false;
	// 인벤에 두군데 빈곳이 있으면 기냥 통과.
	if( true == pclchar->IsBlankInv( 2, INVMODE_ALL ) )
	{
		bResult = true;
	}
	// 인벤이 꽉 차있으면 같은 아이템이 인벤에 있는지 확인해서 두개 다 있으면 아이템을 준다.
	else
	{
		SI32 siPackItemUnique1 = ITEMUNIQUE(13661);
		SI32 siPackItemUnique2 = ITEMUNIQUE(13373);

		SI32 Pos1 = pclchar->pclCI->clCharItem.GetItemPos( siPackItemUnique1 );
		SI32 Pos2 = pclchar->pclCI->clCharItem.GetItemPos( siPackItemUnique2 );
		
		// 두개 다 인벤에 아이템이 있다면
		if( 0 < Pos1 && 0 < Pos2 )
		{
			bResult = true;
		}
		// 둘중 하나가 있고
		else if( 0 < Pos1 || 0 < Pos2 )
		{
			// 인벤 하나가 비어있다면.
			if( true == pclchar->IsBlankInv( 1, INVMODE_ALL ) )
			{
				bResult = true;
			}
		}
	}

	if( false == bResult ) 
	{
		SendServerResponseMsg( 0, SRVAL_CANNOTINPUTTOINV, 0, 0, pclchar->GetID() );
		return;
	}
		
	pclchar->GiveItem( ITEMUNIQUE(13661), pclinfo->m_clPackingItem.siItemNum, INPUTITEMPERSON_OPTION_VALENTINEDAY_PAKING, 0 );
	pclchar->GiveItem( ITEMUNIQUE(13373), pclinfo->m_clPackingItem.siItemNum, INPUTITEMPERSON_OPTION_VALENTINEDAY_PAKING, 0 );

	// 아이템 사용.
	SI32 siInvenPos = pclchar->pclCI->clCharItem.GetItemPos( pclinfo->m_clPackingItem.siUnique );
	if( 0 > siInvenPos ) return;

	cltItem clUseItem;
	clUseItem.Set( &pclchar->pclCI->clCharItem.clItem[siInvenPos] );
	clUseItem.siItemNum = pclinfo->m_clPackingItem.siItemNum;

	sDBRequest_UseItem useitem(	pclchar->GetID(),
								pclchar->pclCI->GetPersonID(),
								siInvenPos,
								&clUseItem, 
								USEITEM_REASON_CONTRIBUTION_ITEM_INPUT	);

	pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

	

	cltItem	clResultItem[2];
	clResultItem[0].siUnique  = ITEMUNIQUE(13661);
	clResultItem[0].siItemNum = pclinfo->m_clPackingItem.siItemNum;
	clResultItem[1].siUnique  = ITEMUNIQUE(13373);
	clResultItem[1].siItemNum = pclinfo->m_clPackingItem.siItemNum;
	
	cltGameResponse_ValentineDay_Item_Packing clInfo( clResultItem );
	cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINEDAY_ITEM_PACKING, PACKET(clInfo) );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_BUY_ITEM(cltMsg* pclMsg,SI32 id)
{
	cltGameRequest_ValentineDay_Buy_Item* pclinfo = (cltGameRequest_ValentineDay_Buy_Item*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	if( pclinfo->m_UseMoney > pclchar->pclCI->clIP.GetMoney() ) return;

	const SI32 siNPCKind = pclClient->GetUniqueFromHash( TEXT("KIND_GIFTRACCOON") );
	
	// 해당 NPC가 판매하는 아이템의 정보 얻기.
	cltNPCMerchantInfo* pNPCMerchantInfo	= NULL;
	for( SI16 siCount = 0; siCount < MAX_NPC_MERCHANT_NUMBER; ++siCount )
	{
		if( siNPCKind == pclNPCItemInfo->clNPCMerchantInfo[siCount].siNPCKind )
		{
			pNPCMerchantInfo = &pclNPCItemInfo->clNPCMerchantInfo[siCount];
			break;
		}
	}

	if( NULL == pNPCMerchantInfo ) return;

	// 내가 구입하는 아이템이 NPC가 판매하는 아이템 목록에 있는지 확인.
	bool bNPCSellItem = false;
	for ( SI32 i = 0; i < MAX_NPC_ITEM_NUMBER; ++i )
	{
		if( pclinfo->m_clBuyItem.siUnique == pNPCMerchantInfo->clItemInfo[i].clItem.siUnique )
		{
			bNPCSellItem = true;
			break;
		}
	}

	if( false == bNPCSellItem ) return;

	if( GIVEITEM_RESULT_SUCCESS == pclchar->GiveItem( pclinfo->m_clBuyItem.siUnique, pclinfo->m_clBuyItem.siItemNum, INPUTITEMPERSON_OPTION_VALENTINEDAY_BUY, 0 ) )
	{
		sDBRequest_ChangeMoney clMsg( id, pclchar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_VALENTINEDAY_BUY_ITEM, -pclinfo->m_UseMoney );
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


		cltGameResponse_ValentineDay_Buy_Item clInfo( &pclinfo->m_clBuyItem );
		cltMsg clMsg2( GAMEMSG_RESPONSE_VALENTINEDAY_BUY_ITEM, PACKET(clInfo) );
		pclchar->SendNetMsg((sPacketHeader*)&clMsg2 );
	}
	else
	{
		SendServerResponseMsg( 0, SRVAL_CANNOTINPUTTOINV, 0, 0, pclchar->GetID() ); 
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_QUESTION(cltMsg* pclMsg,SI32 id)
{
	cltGameRequest_ValentineDay_Confession_Question* pclinfo = (cltGameRequest_ValentineDay_Confession_Question*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if( NULL == pclchar )		return;

	// 고백을 받는 사람.
	SI32 siTargetPersonID = pclCM->GetPersonIDFromName( pclinfo->m_szTargetName );
	if( 0 >= siTargetPersonID )	
	{
		SendServerResponseMsg( 0, SRVAL_VALENITNEDAY_NOT_USER, 0, 0, pclchar->GetID() ); 
		return; 
	}

	// 자신한테 고백 할 수 없음.
	if( siTargetPersonID == pclchar->pclCI->GetPersonID() ) 
	{
		SendServerResponseMsg( 0, SRVAL_VALENITNEDAY_NOT_MYSELF, 0, 0, pclchar->GetID() ); 
		return; 
	}
	
	cltCharServer* pclTargetChar = pclCM->GetCharServer( pclCM->GetIDFromPersonID( siTargetPersonID ) );
	if( NULL == pclTargetChar ) 
	{
		SendServerResponseMsg( 0, SRVAL_VALENITNEDAY_NOT_USER, 0, 0, pclchar->GetID() ); 
		return; 
	}

	if(    pclchar->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_VALENTINEDAY ) 
		|| pclTargetChar->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_VALENTINEDAY ) )
	{
		SendServerResponseMsg( 0, SRVAL_VALENITNEDAY_ALREADY_BUF, 0, 0, pclchar->GetID() ); 
		return;
	}
	
	
	cltGameResponse_ValentineDay_Confession_Question clInfo( pclchar->pclCI->GetPersonID(), (TCHAR*)pclchar->GetName(), pclinfo->m_szMsg );
	cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINEDAY_CONFESSION_QUESTION, PACKET(clInfo) );
	pclTargetChar->SendNetMsg((sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_ANSWER(cltMsg* pclMsg,SI32 id)
{
	cltGameRequest_ValentineDay_Confession_Answer* pclinfo = (cltGameRequest_ValentineDay_Confession_Answer*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	// 고백을 한사람.
	cltCharServer* pclTargetChar = pclCM->GetCharServer( pclCM->GetIDFromPersonID( pclinfo->m_siTargetPersonID ) );
	if( NULL == pclTargetChar ) return;

	if( pclinfo->m_bResult )
	{
		// 아이템 유무.
		SI32 siInvenPos = pclTargetChar->pclCI->clCharItem.GetItemPos( 13661 );
		if( 0 > siInvenPos ) return;

		SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.				

		SI16 index = pclchar->pclCI->clBI.clBufInfo.FindEmptySlot( BUF_VALENTINEDAY, false );
		if( index > -1 )
		{
			stBuf addBuf( BUF_VALENTINEDAY, 0, usetime );
			pclchar->AddBufThroughDB( index, &addBuf , false, 0, NULL, 0 );
		}

		index = pclTargetChar->pclCI->clBI.clBufInfo.FindEmptySlot( BUF_VALENTINEDAY, false );
		if( index > -1 )
		{
			stBuf addBuf( BUF_VALENTINEDAY, 0, usetime );
			pclTargetChar->AddBufThroughDB( index, &addBuf , false, 0, NULL, 0 );
		}

		// 아이템 사용.
		cltItem clUseItem;
		clUseItem.Set( &pclTargetChar->pclCI->clCharItem.clItem[siInvenPos] );
		clUseItem.siItemNum = 1;

		sDBRequest_UseItem useitem(	pclTargetChar->GetID(),
									pclTargetChar->pclCI->GetPersonID(),
									siInvenPos,
									&clUseItem, 
									USEITEM_REASON_VALENTINEDAY	);

		pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
	}
	
	cltGameResponse_ValentineDay_Confession_Answer clInfo( pclinfo->m_bResult, (TCHAR*)pclchar->GetName() );
	cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINEDAY_CONFESSION_ANSWER, PACKET(clInfo) );
	pclTargetChar->SendNetMsg((sPacketHeader*)&clMsg );
}


