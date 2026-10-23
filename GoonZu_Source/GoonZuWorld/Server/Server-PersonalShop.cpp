//---------------------------------
// 2003/9/18 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "../../Lib/HashTable/HashTable_StringInt.h"
#include "Msg/MsgType-PersonalShop.h"
#include "MsgType-Item.h"
#include "MsgRval-Define.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-PersonalShop.h"
#include "../Server/Minister/MinisterMgr.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Client/ninterface/npersonalshopdlg/NPersonalShopDlg.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_PERSONALSHOPBANNER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PersonalShopBanner* pclinfo = (cltGameMsgRequest_PersonalShopBanner*)pclMsg->cData;

	SI32 chatid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(chatid) == false)return ;

	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[chatid];
	if(pclchar->bPersonalShopSwitch == false)	return ;

	// 머릿말 데이터를 보낸다. 
	cltGameMsgResponse_PersonalShopBanner clinfo(pclinfo->siCharUnique, pclchar->bPersonalShopSwitch, &pclchar->clShopBanner);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONALSHOPBANNER, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 개인상점  정보를 요청한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_PERSONALSHOPDATA(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PersonalShopData* pclinfo = (cltGameMsgRequest_PersonalShopData*)pclMsg->cData;

	SI32 chatid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(chatid) == false)return ;

	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[chatid];
	// 새로 초기화하고 설정한다.
	if(pclchar->bPersonalShopSwitch == false)
	{
		pclchar->clShopData.Init();

		if(pclinfo->bManufacture == FALSE)
		{
			pclchar->clShopData.m_siShopMode = PSHOP_MODE_SELL;
		}
		else
		{
			if(pclinfo->bMaker == TRUE )
			{
				pclchar->clShopData.m_siShopMode = PSHOP_MODE_MAKEITEM_WANT ;
			}
			else
			{
				pclchar->clShopData.m_siShopMode = PSHOP_MODE_MAKEITEM;
			}
		}

		pclchar->clShopData.m_siPersonID = pclchar->pclCI->GetPersonID();
		StringCchCopy(pclchar->clShopData.m_szAccountID, MAX_PLAYER_NAME, pclchar->pclCI->clBI.szAccountID);
	}
	else if(true == pclchar->bPersonalShopSwitch)
	{
		/*if(pclinfo->bManufacture == TRUE)  
		{
			if(pclchar->clShopData.m_siShopMode == PSHOP_MODE_SELL)
			{
				SendServerResponseMsg( 0, SRVAL_PERSONAL_SHOP_MODE_CHANGE_FAIL, 0, 0, chatid );
				return;
			}
		}
		else
		{
			if(pclchar->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM)
			{
				SendServerResponseMsg( 0, SRVAL_PERSONAL_SHOP_MODE_CHANGE_FAIL, 0, 0, chatid );
				return;
			}
		}*/
	}

	// 현재 시세 정보를 설정한다.
	pclchar->clShopData.SetCurrentPrice(pclItemManager);

	// 개인상점 
	cltGameMsgResponse_PersonalShopData clinfo(pclinfo->siCharUnique, &pclchar->clShopData);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}



void cltServer::DoMsg_GAMEMSG_REQUEST_STARTPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	//[진성] 밀봉 프리미엄. => 2008-3-25
	if(pclClient->IsCountrySwitch(Switch_SpecialPersonShop))
	{
		StartPersonalSpecialShop(pclMsg, id);
		return;
	}

	cltGameMsgRequest_StartPersonalShop* pclinfo = (cltGameMsgRequest_StartPersonalShop*)pclMsg->cData;
	if ( pclinfo == NULL )
	{
		return;
	}
		
	SI32 chatid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(chatid) == false)
	{
		return ;

	}

	// 이미 진행하고 있다면 종료하고 다시 설정해야한다.
	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[chatid];
	if ( pclchar == NULL )
	{
		return;
	}

	// 선거 유세중에는 불가능하다
	if ( true == pclchar->bIsElectionStump )
	{
		return;
	}

	// [영훈] 상점을 열 수 없는 지역이라면 리턴
	SI32 siMapIndex = pclchar->GetMapIndex();

	if ( (pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM) &&  (pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM_WANT) && (pclMapManager->IsCanMarketOpenMap(siMapIndex) == false) )
	{
		return;
	}

	SI32 siSuccess		= 1;
	SI32 siTicketPos    = 0;
	SI32 siTicketUnique = (pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT ) ? ITEMUNIQUE(24065) : ITEMUNIQUE(24010);	// 개인상점 이용권
	SI32 siTicketNum	= 1;

	if(pclinfo->clShopData.m_bPremium == true)
	{
		siTicketUnique = ITEMUNIQUE(24075);
	}

	if(bForcePersonalShopSwitch==false)	  	      {	siSuccess = -2;	} // 개인상점 강제 금지 
	else if(pclchar->bPersonalShopSwitch == true) {	siSuccess = 0;	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		if(siTicketNum <= 0)		return;

		///////////////////////////////////////////////////////
		// 이용권 확인 
		siTicketPos = pclCM->CR[chatid]->pclCI->clCharItem.GetItemPos(siTicketUnique);
		if( siTicketPos < PERSONITEM_INV0 || siTicketPos >= MAX_ITEM_PER_PERSON )								siSuccess = -1;
		if(siSuccess > 0 && pclCM->CR[chatid]->pclCI->clCharItem.clItem[siTicketPos].siItemNum < siTicketNum)	siSuccess = -1;
		//////////////////////////////////////////////////////////////////////////
	}

	// 이용권이 있는데 올려진 ItemList가 거래 금지목록이면 siSuccess = -3;
	if(siSuccess > 0 && pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM && pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM_WANT)
	{
		for(SI32 i = 0; i <MAX_SHOP_ITEM_NUMBER; ++i )
		{
			if(pclinfo->clShopData.m_clItem[i].siUnique)
			{
				SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->clShopData.m_clItem[i].siUnique);
				if(ref > 0)	
				{
					// 거래가능물품인지 검사한다.
					if( pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_NOTTRADE) ||
						//cyj 귀속아이템은 금지
						pclinfo->clShopData.m_clItem[i].IsBelonging(pclItemManager) ||
						pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(8150)||
						pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(14300) ||
						pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(14400)||
						pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(14500)) // 반격의깃발 등
					{
						siSuccess = -3;
					}

					if(pclClient->IsCountrySwitch(Switch_FourLeaf))
					{
						//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
						if( ITEMUNIQUE(2755) == pclinfo->clShopData.m_clItem[i].siUnique || 
							ITEMUNIQUE(2754) == pclinfo->clShopData.m_clItem[i].siUnique	)	
						{	
							if( pclinfo->clShopData.m_clItem[i].siUnique != siFourLeafUnique )	
							{
								siSuccess = -3;
							}
						}
					}
				}
			}

			// 파티홀에서 받은 아이템이면 거래가 불가능 하다.
			if( pclinfo->clShopData.m_clItem[i].clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
				siSuccess = -3;
		}
	}

	// 마을에서 최대 갯수를 넘지 못한다.
	if(siSuccess > 0 )
	{
		if ( pclMapManager->pclVillageManager->IsValidVillage(pclchar->GetMapIndex()) == true )
		{
			CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
			SI32 siShopNum = pclMapManager->pclVillageManager->GetPersonalShopNumber(pclchar->GetMapIndex());
			if(pclminister  && siShopNum >= pclminister->GetMaxShopNum() )
			{
				siSuccess = -4;
			}
		}
	}

	if(siSuccess > 0)
	{
		// 이용권을 사용한 마을의 Unique를 준다.
		SI32 villageunique  = pclCM->CR[id]->GetCurrentVillageUnique();

		cltItem clTicketItem;
		SI16 brareswitch = 0;
		pclClient->pclItemManager->MakeRandItemUnique( siTicketUnique, &clTicketItem, 0, 0, &brareswitch, 0,0,0,0,0,0);
		clTicketItem.SetItemNum(siTicketNum);

		// DB로 요청한다. 
		sDBRequest_StartPersonalShop clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, siTicketPos, siTicketUnique, siTicketNum, &pclinfo->clShopData, &clTicketItem);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		// 물품 사용 통계를 낸다.(개인상점 이용권)
		pclItemManager->AddUseNum(siTicketUnique, siTicketNum);

		//새로운 통계 - 물품 사용 통계를 낸다.
		//NStatistics_Add("Item", siTicketUnique, "UseNum", siTicketNum);


		//==========================================================
		// 미리 배너는 만들어 놓치 뭐 
		pclchar->SetPersonalShopBanner(&pclinfo->clShopData);
		pclchar->SetPersonalShopData(false, &pclinfo->clShopData);
		//==========================================================
		// 혹시나 해서리 다시 한번 
		pclchar->clShopData.m_siPersonID = pclchar->pclCI->GetPersonID();
		StringCchCopy(pclchar->clShopData.m_szAccountID, MAX_PLAYER_NAME, pclchar->pclCI->clBI.szAccountID);
		//==========================================================

		// 제조의뢰 퀘스트
		if ( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM )
		{
			pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_MAKEREQUESTSTART);
		}
	}
	else
	{
		// 설정 정보를 설정한다.
		// 실패 했을때 제조의뢰인지 샵을 얼다 실패한건지 구분 하기 위해서 모드를 넣어준다 .
		cltGameMsgResponse_StartPersonalShop clinfo(pclchar->GetCharUnique(), siSuccess,false, pclinfo->clShopData.m_siShopMode );
		cltMsg clMsg(GAMEMSG_RESPONSE_STARTPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 9. 3] // 개인상점 시작 쓰기. 
		//	// param1 == 개인상점의 실패여부, param2 == 프리미엄 여부.
		//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_START, 
		//		0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		//		siSuccess, false, 0, 0, 0, NULL, NULL);
		//}
	}
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_STOPPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_StopPersonalShop* pclinfo = (cltGameMsgRequest_StopPersonalShop*)pclMsg->cData;

	SI32 chatid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(chatid) == false)return ;

	// 이미 진행하고 있다면 종료하고 다시 설정해야한다.
	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[chatid];
	if(pclchar->bPersonalShopSwitch == false)	return ;
	
	// 캐릭터의 개인상점을 설정한다. 
	pclchar->SetPersonalShopBanner(NULL); 
	pclchar->SetPersonalShopData(false, NULL);
	pclchar->clShopData.m_siShopMode = PSHOP_MODE_NONE;

	//[진성] 밀봉 프리미엄 상점 아이템이 존재했었으면 초기화. => 2008-6-18
	if( pclchar->clShopData.m_siSelectedSpacialInventoryPos > 0 )
	{
		if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
		{
			pclchar->clShopData.m_siSelectedSpacialInventoryPos = 0;
			pclchar->clShopData.m_clSpacialItem.siUnique        = 0;
		}
	}
	
	// 이웃들에게 종료가 되었음을 알려준다.
	cltGameMsgResponse_StopPersonalShop clinfo(pclchar->GetCharUnique(), pclchar->pclCI->GetPersonID(), 1);
	cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNeighbourMsg(&clMsg, true);

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2008. 10. 9] // 개인상점 종료 쓰기.
	//	// param1 == 결과.
	//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_END, 
	//						0, (cltCharServer*)pclchar, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	//						1, 0, 0, 0, 0, NULL, NULL);
	//}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BUYPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_BuyPersonalShop* pclinfo = (cltGameMsgRequest_BuyPersonalShop*)pclMsg->cData;
	
	SI32 Shopperid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(Shopperid) == false)return ;
	if(pclCM->IsValidID(id)		   == false)return ;
		
	// 상품을 사기를 원하는 상점주인임을 검사한다.
	cltCharServer* pclcharshopper = (cltCharServer* )pclCM->CR[Shopperid];
	cltCharServer* pclcharbuyer   = (cltCharServer* )pclCM->CR[id];
	if ( pclcharshopper == NULL || pclcharbuyer == NULL )	return;

	cltShopTradeData	*pShoperShopData = &pclcharshopper->clShopData;

	SI32 siSuccess = 1;
	//---------------------------------------------
	// 내가 물건을 사기로 한 상점의 주인과 상점의 상태를 비교한다.
	//---------------------------------------------
	if(siSuccess > 0)
	{
		if(pclcharshopper->bPersonalShopSwitch == false)					siSuccess = -1;// 상점을 개설하고 있지 않다.
		if(pclcharshopper->clShopData.m_siShopMode != pclinfo->siShopMode)	siSuccess = -1;// 상점의 모드가 서로 다르다.
		if(pclcharshopper->clShopData.m_siShopMode != PSHOP_MODE_SELL)		siSuccess = -1;// 상점의 모드가 SELL이 아니다.
		if(pclcharshopper->pclCI->GetPersonID()!= pclinfo->siPersonID)		siSuccess = -1;// PERSON ID 가 다르다.
	}

	//---------------------------------------------
	// 내가 사고자하는 정보를 점검한다.
	//---------------------------------------------
	SI32 siShowcaseIndex = 0;
	if(siSuccess > 0)
	{
		if(pclinfo->clItem.siUnique<1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum<1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum>MAX_ITEM_PILE_NUMBER)					siSuccess = -2;	
		if(pclinfo->siPrice < 1 )											siSuccess = -2;	
		if(pclinfo->siPrice > MAX_PRICE_IN_MARKET )							siSuccess = -2;	

		// showcase index가 틀렸다.
		siShowcaseIndex = pclinfo->siShowcaseIndex;
		if(siShowcaseIndex< 0 || siShowcaseIndex>= MAX_SHOP_ITEM_NUMBER)	siSuccess = -2; 
	}
	//---------------------------------------------
	// 내가 사고자하는 정보와 상점주인의 ShowCase 정보를 비교한다.
	//---------------------------------------------
	SI32 siShopperInventoryPos = 0;
	SI32 siShowcaseItemNum = 0;
	if(siSuccess > 0)
	{
		// 아이템이 일치하지 않음.
		if(pclinfo->clItem.IsSame( &pShoperShopData->m_clItem[siShowcaseIndex], false )==FALSE )siSuccess = -3;
		// 제시한 가격이 일치하지 않음 
		if(pclinfo->siPrice != pShoperShopData->m_siPrice[siShowcaseIndex])	siSuccess = -3;
		// 상점가격이 부적절 할때 
		if(pShoperShopData->m_siPrice[siShowcaseIndex] < 1)					siSuccess = -3;
		// 상점의 판매 갯수가 부적절할때 
		siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)											siSuccess = -3;

		// inventory index가 틀렸다.
		siShopperInventoryPos = pShoperShopData->m_siSelectedInventoryPos[siShowcaseIndex];
		if(siShopperInventoryPos <PERSONITEM_INV0 || siShopperInventoryPos>= MAX_ITEM_PER_PERSON)	siSuccess = -3;
	}

	//---------------------------------------------
	// 내가 사고자하는 정보와 상점주인의 Inventory 정보를 비교한다.
	//---------------------------------------------
	SI32 siRealBuyItemNum = 0;
	SI32 siInvenItemNum= 0;
	if(siSuccess > 0)
	{
		// 인벤토리 아이템이 일치하지 않음.
		if(pclinfo->clItem.IsSame( &pclcharshopper->pclCI->clCharItem.clItem[siShopperInventoryPos], false )==FALSE )siSuccess = -4;
		
		// 실제로 구입할수 있는 개수를 구한다.
		// 구입 요구 갯수  > 진열대 갯수  > 인벤토리 갯수 => 가장 작은 개수가 구입이 된다.
		siRealBuyItemNum = pclinfo->clItem.siItemNum;
		SI32 siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)						siSuccess = -4;
		siInvenItemNum= pclcharshopper->pclCI->clCharItem.clItem[siShopperInventoryPos].siItemNum;
		if(siInvenItemNum < 1)							siSuccess = -4;

		// 가장 작은 갯수를 설정한다.
		if(siRealBuyItemNum > siShowcaseItemNum)siRealBuyItemNum = siShowcaseItemNum;
		if(siRealBuyItemNum > siInvenItemNum )	siRealBuyItemNum = siInvenItemNum;
		if(siRealBuyItemNum <1)							siSuccess = -4;
	}
	//---------------------------------------------
	// 구입자가 구입 가능한 최소 돈이 있는지 확인
	//---------------------------------------------
	SI64 siTotalPrice = 0; 
	if(siSuccess > 0)
	{
		SI64 TotalBuyerMoney = (SI64)pclCM->CR[id]->pclCI->clIP.GetMoney();
		siTotalPrice         = (SI64)pclinfo->siPrice * (SI64)siRealBuyItemNum ;
		if( siTotalPrice < 1)
		{
			siSuccess = -5;
		}
		else if ( siTotalPrice > TotalBuyerMoney )	
		{
			siSuccess = -5;
		}
		
	}
	
	
	//---------------------------------------------
	// 구입자가 아이템을 확보할 공간이 있는지 확인
	//---------------------------------------------
	cltItem clBuyItem;
	SI16	siBuyerInventoryPos = -1;
	if(siSuccess > 0)
	{
		clBuyItem.Init();
		clBuyItem.Set( &pclcharshopper->pclCI->clCharItem.clItem[siShopperInventoryPos] );
		clBuyItem.siItemNum = siRealBuyItemNum;
		cltItem clToItem;
		clToItem.Init();

		SI32 rtnvalue;
		// 구입자의 인벤에 넣을 수 있는지 확인한다.
		if ( pclcharbuyer->CanAddInv(INVMODE_ALL, &clBuyItem, &siBuyerInventoryPos, &clToItem, &rtnvalue)==false )
		{
			siSuccess = -6;
		}
	}
	//---------------------------------------------------------------
	// Shopper의 물품 -> Buyer의 BuyerInvPos, Buyer의 돈 -> Shopper
	//---------------------------------------------------------------
	// DB Message를 보낸다.
	if(siSuccess > 0)
	{
		//---------------------------------------------
		// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclcharshopper->ItemUseReserveAction();
		pclcharbuyer->ItemUseReserveAction();
		//---------------------------------------------

		//---------------------------------------------
		// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclcharshopper->BulletUseReserveAction();
		pclcharbuyer->BulletUseReserveAction();


		// 다른사람이 구입을 하지 못하도록일단 뺴둔다.
		pclcharshopper->clShopData.m_clItem[siShowcaseIndex].siItemNum -= clBuyItem.siItemNum;

		sDBRequest_PersonalShopBuyItem	clMsg(	id,		   pclcharbuyer->pclCI->GetPersonID(), 
												Shopperid, pclcharshopper->pclCI->GetPersonID(),
												siBuyerInventoryPos, siShopperInventoryPos,  
												siShowcaseIndex,  siTotalPrice,
												&clBuyItem, clBuyItem.CanPile(pclItemManager));
	
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else		
	{
		SI32 siFailReason = 0;
		switch(siSuccess)
		{
		case -1: siFailReason = BUYPERSONALSHOP_FAIL_SHOPPERSTATUS; break;// 구매실패 
		case -2: siFailReason = BUYPERSONALSHOP_FAIL_BUYERSTATUS;	break;// 구매실패 
		case -3: siFailReason = BUYPERSONALSHOP_FAIL_SHOWCASESTATUS;break;// 구매실패 
		case -4: siFailReason = BUYPERSONALSHOP_FAIL_BUY_NOITEM;	break;// 구매실패 
		case -5: siFailReason = BUYPERSONALSHOP_FAIL_BUY_NOMONEY;   break;// 구매실패 
		case -6: siFailReason = BUYPERSONALSHOP_FAIL_BUY_NOINVSPACE;break;// 구매실패 
		default: siFailReason = BUYPERSONALSHOP_FAIL_UNKNOWN; break;// 구매실패 
		}
		// Client에게 siSuccess 오류정보를 전송한다.
		cltGameMsgResponse_BuyPersonalShop clInfo(id, siFailReason, pclinfo->clItem.siUnique, pclinfo->clItem.siItemNum);
		cltMsg clMsg(GAMEMSG_RESPONSE_BUYPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SELLPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SellPersonalShop* pclinfo = (cltGameMsgRequest_SellPersonalShop*)pclMsg->cData;

	SI32 Shopperid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(Shopperid) == false)return ;
	if(pclCM->IsValidID(id)		   == false)return ;
		
	// 상품을 사기를 원하는 상점주인임을 검사한다.
	cltCharServer* pclcharshopper = (cltCharServer* )pclCM->CR[Shopperid];
	cltCharServer* pclcharseller  = (cltCharServer* )pclCM->CR[id];
	if ( pclcharshopper == NULL || pclcharseller == NULL )	return;

	cltShopTradeData	*pShoperShopData = &pclcharshopper->clShopData;

	SI32 siSuccess = 1;
	//---------------------------------------------
	// 내가 물건을 팔기로한 상점의 주인과 상점의 상태를 비교한다.
	//---------------------------------------------
	if(siSuccess > 0)
	{
		if(pclcharshopper->bPersonalShopSwitch == false)					siSuccess = -1;// 상점을 개설하고 있지 않다.
		if(pclcharshopper->clShopData.m_siShopMode != pclinfo->siShopMode)	siSuccess = -1;// 상점의 모드가 서로 다르다.
		if(pclcharshopper->clShopData.m_siShopMode != PSHOP_MODE_BUY)		siSuccess = -1;// 상점의 모드가 BUY가 아니다.
		if(pclcharshopper->pclCI->GetPersonID()!= pclinfo->siPersonID)		siSuccess = -1;// PERSON ID 가 다르다.
	}

	//---------------------------------------------
	// 내가 팔고자 하는 정보를 점검한다.
	//---------------------------------------------
	SI32 siShowcaseIndex = 0;
	if(siSuccess > 0)
	{
		if(pclinfo->clItem.siUnique <1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum < 1)									siSuccess = -2;	
		if(pclinfo->clItem.siItemNum > MAX_ITEM_PILE_NUMBER)				siSuccess = -2;	
		if(pclinfo->siPrice < 1 )											siSuccess = -2;	
		if(pclinfo->siPrice > MAX_PRICE_IN_MARKET )							siSuccess = -2;	

		// showcase index가 틀렸다.
		siShowcaseIndex = pclinfo->siShowcaseIndex;
		if(siShowcaseIndex< 0 || siShowcaseIndex>= MAX_SHOP_ITEM_NUMBER)	siSuccess = -2; 

		// 나의 inventory index가 틀렸다.
		if(pclinfo->siSellerInventoryRealIndex<PERSONITEM_INV0 || pclinfo->siSellerInventoryRealIndex>= MAX_ITEM_PER_PERSON)	siSuccess = -2;
	}
	//---------------------------------------------
	// 내가 팔고자하는 정보와 상점주인의 ShowCase 정보가 달라졌는지 검사한다.
	//---------------------------------------------
	SI32 siSellerInventoryPos = pclinfo->siSellerInventoryRealIndex;
	SI32 siShowcaseItemNum = 0;
	if(siSuccess > 0)
	{
		// 아이템이 일치하지 않음.
		if(pclinfo->clItem.siUnique != pShoperShopData->m_clItem[siShowcaseIndex].siUnique)	siSuccess = -3;
		// 제시한 가격이 일치하지 않음 
		if(pclinfo->siPrice != pShoperShopData->m_siPrice[siShowcaseIndex])	siSuccess = -3;
		// 상점가격이 부적절 할때 
		if(pShoperShopData->m_siPrice[siShowcaseIndex] < 1)					siSuccess = -3;
		// 상점의 판매 갯수가 부적절할때 
		siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)											siSuccess = -3;

		SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->clItem.siUnique);
		if(ref > 0)	
		{
			// 거래가능물품인지 검사한다.
			if( pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_NOTTRADE) ||
				//cyj 귀속아이템은 금지
				pclinfo->clItem.IsBelonging(pclItemManager) )
			{
				siSuccess = -3;
			}
		}


		// 상점의 정보와 내 인벤토리 아이템과 다를떄 
		if(pShoperShopData->m_clItem[siShowcaseIndex].siUnique < 1)			siSuccess = -3;

		// 인벤토리 Unique와 상점의 showcase의 Unique가 다를 때 
		if(pclcharseller->pclCI->clCharItem.clItem[siSellerInventoryPos].siUnique !=
			pShoperShopData->m_clItem[siShowcaseIndex].siUnique)			siSuccess = -3;
			
	}
	//---------------------------------------------
	// 내가 판매하고자하는 정보와 상점주인의 Showcase 정보를 비교해 판매가능한지 검사한다.
	//---------------------------------------------
	SI32 siRealSellItemNum = 0;
	SI32 siInvenItemNum= 0;
	if(siSuccess > 0)
	{
		// 실제로 판매 할 수 있는 개수를 구한다.
		// 판매 요구 갯수  > 진열대 갯수 > 상점이 갖고 있는 돈 ==> 가장 작은 개수가 구입이 된다.
		siRealSellItemNum = pclinfo->clItem.siItemNum;
		SI32 siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)						siSuccess = -4;
		siInvenItemNum= pclcharseller->pclCI->clCharItem.clItem[siSellerInventoryPos].siItemNum;
		if(siInvenItemNum < 1)							siSuccess = -4;

		// 가장 작은 갯수를 설정한다.
		if(siRealSellItemNum > siShowcaseItemNum)	siRealSellItemNum = siShowcaseItemNum;
		if(siRealSellItemNum > siInvenItemNum )		siRealSellItemNum = siInvenItemNum;
		if(siRealSellItemNum < 1 )					siSuccess = -4;
	}

	//---------------------------------------------
	// (상점주인) 구입자가 구입 가능한 최소 돈이 있는지 확인
	//---------------------------------------------
	SI64 siTotalPrice = 0; 
	if(siSuccess > 0)
	{
		SI64 TotalShopperMoney = (SI64)pclCM->CR[Shopperid]->pclCI->clIP.GetMoney();
		siTotalPrice           = (SI64)pclinfo->siPrice * (SI64)siRealSellItemNum ;
		// 상점 주인이 돈이 부족하면 있는 만큼만 판매한다.
		if( siTotalPrice < 1)
		{
			siSuccess = -5;
		}
		else if ( siTotalPrice > TotalShopperMoney )
		{
			siRealSellItemNum = (TotalShopperMoney / pclinfo->siPrice);
		}
		// 돈이 부족해서 1개도 상점주인이 구입을 하지 못하면 
		if(siRealSellItemNum < 1)	
		{
			siSuccess = -5;
		}
		else
		{
			siTotalPrice    = (SI64)pclinfo->siPrice * (SI64)siRealSellItemNum ;
			// 상점 주인이 돈이 부족하면 문제가 있는것이다.
			if(siTotalPrice <1)
			{
				siSuccess = -5;
			}
			else if ( siTotalPrice > TotalShopperMoney )
			{
				siSuccess = -5;
			}
		}
				
	}
	
	//---------------------------------------------
	// 구입자가 아이템을 확보할 공간이 있는지 확인
	//---------------------------------------------
	cltItem clSellItem;
	SI16	siShopperInventoryPos = -1;
	if(siSuccess > 0)
	{
		clSellItem.Init();
		clSellItem.Set( &pclcharseller->pclCI->clCharItem.clItem[siSellerInventoryPos] );
		clSellItem.siItemNum = siRealSellItemNum;
		cltItem clToItem;
		clToItem.Init();

		SI32 rtnvalue;
		// 상점주인의 인벤에 넣을 수 있는지 확인한다. 
		if ( pclcharshopper->CanAddInv(INVMODE_ALL, &clSellItem, &siShopperInventoryPos, &clToItem, &rtnvalue)==false )
		{
			siSuccess = -6;
		}
	}
	//---------------------------------------------------------------
	// Seller의 SellerInvPos 물품 -> Shopper의 ShopperInvPos, Shooper의 돈 -> Seller
	//---------------------------------------------------------------
	// DB Message를 보낸다.
	if(siSuccess > 0)
	{
		//---------------------------------------------
		// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclcharshopper->ItemUseReserveAction();
		pclcharseller->ItemUseReserveAction();
		//---------------------------------------------

		//---------------------------------------------
		// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclcharshopper->BulletUseReserveAction();
		pclcharseller->BulletUseReserveAction();

		// 다른사람이 구입을 하지 못하도록일단 뺴둔다.
		pclcharshopper->clShopData.m_clItem[siShowcaseIndex].siItemNum -= clSellItem.siItemNum;

		sDBRequest_PersonalShopSellItem	clMsg(	id,		   pclcharseller->pclCI->GetPersonID(), 
												Shopperid, pclcharshopper->pclCI->GetPersonID(),
												siSellerInventoryPos, siShopperInventoryPos,  
												siShowcaseIndex,  siTotalPrice,
												&clSellItem, clSellItem.CanPile(pclItemManager));
	
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else		
	{
		SI32 siFailReason = 0;
		switch(siSuccess)
		{
		case -1: siFailReason = SELLPERSONALSHOP_FAIL_SHOPPERSTATUS;	break;	// 구매실패 
		case -2: siFailReason = SELLPERSONALSHOP_FAIL_SELLERSTATUS;		break;	// 구매실패 
		case -3: siFailReason = SELLPERSONALSHOP_FAIL_SHOWCASESTATUS;	break;	// 구매실패 
		case -4: siFailReason = SELLPERSONALSHOP_FAIL_SELL_NOITEM;		break;	// 구매실패 
		case -5: siFailReason = SELLPERSONALSHOP_FAIL_SELL_NOMONEY;		break;	// 구매실패 
		case -6: siFailReason = SELLPERSONALSHOP_FAIL_SELL_NOINVSPACE;	break;	// 구매실패 
		default: siFailReason = SELLPERSONALSHOP_FAIL_UNKNOWN;			break;	// 구매실패 
		}
		// Client에게 siSuccess 오류정보를 전송한다.
		cltGameMsgResponse_SellPersonalShop clInfo(id, siFailReason, pclinfo->clItem.siUnique, pclinfo->clItem.siItemNum);
		cltMsg clMsg(GAMEMSG_RESPONSE_SELLPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		
		// 돈이 없어 실패를 해다면 종료 시킨다. 
		if(siFailReason == SELLPERSONALSHOP_FAIL_SELL_NOMONEY ||
		   siFailReason == SELLPERSONALSHOP_FAIL_SELL_NOINVSPACE)
		{
			// 캐릭터의 개인상점을 설정한다. 
			pclcharshopper->SetPersonalShopBanner(NULL);
			pclcharshopper->SetPersonalShopData(false, NULL);
			
			// 이웃들에게 종료가 되었음을 알려준다.
			cltGameMsgResponse_StopPersonalShop clinfo(pclcharshopper->GetCharUnique(), pclcharshopper->pclCI->GetPersonID(), 3);
			cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
			pclcharshopper->SendNeighbourMsg(&clMsg, true);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			//{
			//	// [로그추가 : 황진성 2007. 10. 9] // 개인상점 종료 쓰기.
			//	// param1 == 결과.
			//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_END, 
			//						0, (cltCharServer*)pclcharshopper, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			//						3, 0, 0, 0, 0, NULL, NULL);
			//}
		}
	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SEARCHPERSONALSHOPITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SearchPersonalShopItem* pclinfo = (cltGameMsgRequest_SearchPersonalShopItem*)pclMsg->cData;

	if(pclCM->IsValidID(id)		   == false)return;
	if(pclinfo->siItemUniqueType <1)		return;

	cltSearchShopData	clSearchShopData;
	//-------------------------------------------
	// 검색 기본데이터를 설정한다.
	//-------------------------------------------
	clSearchShopData.m_siShopMode		 = pclinfo->siShopMode;
	clSearchShopData.m_siItemUniqueType  = pclinfo->siItemUniqueType;
	//-------------------------------------------
	// 검색 가격을 다시 보낸다.
	//-------------------------------------------
	clSearchShopData.m_siItemSearchPrice = pclinfo->siMaxPrice;
	
	//-------------------------------------------
	// 사용자를 검색하여 정보를 얻는다.
	//-------------------------------------------
	SI32 siCount = 0;
	for ( SI32 i = 0; i < MAX_PERSON_NUMBER && siCount< MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
	{
		if ( pclCM->IsValidID( i ) && IsPC(i))
		{
			cltCharServer* pclchar = (cltCharServer* )pclCM->CR[i];
			// 개인 상점을 열고 있으면서 같은 ShopMode라면 물품을 검색한다.
			if(pclchar && pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode == pclinfo->siShopMode)
			{
				for(SI32 j = 0; j < MAX_SHOP_ITEM_NUMBER && siCount< MAX_SEARCHSHOP_ITEM_NUMBER; ++j) 
				{
					// 같은 타입의 아이템의 물건을 갖고 있을때 
					SI32 siItemUnique = pclchar->clShopData.m_clItem[j].siUnique;
					if( siItemUnique == 0) continue;
					
					if( pclItemManager->GetItemType(siItemUnique) == pclinfo->siItemUniqueType &&
						pclchar->clShopData.m_clItem[j].siItemNum > 0)
						
					{
						if(pclinfo->siShopMode == PSHOP_MODE_BUY)
						{
							// 가격 검색 정보가 있을경우  
							if( pclinfo->siMaxPrice> 0 && 
								pclchar->clShopData.m_siPrice[j] < pclinfo->siMaxPrice) continue;
						}
						else if(pclinfo->siShopMode == PSHOP_MODE_SELL)
						{
							// 수수료 검색 정보가 있을경우 
							if( pclinfo->siMaxPrice> 0 && 
								pclchar->clShopData.m_siPrice[j] > pclinfo->siMaxPrice) continue;
						}
						// 수수료 검색 정보가 있을경우 
						else if(pclinfo->siShopMode == PSHOP_MODE_MAKEITEM)
						{
							if( pclinfo->siMaxPrice> 0 && 
								pclchar->clShopData.m_siPrice[j] < pclinfo->siMaxPrice) continue;
						}
						
						if(siCount<MAX_SEARCHSHOP_ITEM_NUMBER)
						{
							// 개인정보 
							StringCchCopy(clSearchShopData.m_szName[siCount], MAX_PLAYER_NAME, (TCHAR *)pclchar->GetName());
							clSearchShopData.m_siLevel[siCount]		= pclchar->pclCI->clIP.GetLevel();
							// 위지정보 
							clSearchShopData.m_siMapIndex[siCount]	= pclchar->GetMapIndex();
							clSearchShopData.m_siMapX[siCount]		= pclchar->GetX();
							clSearchShopData.m_siMapY[siCount]		= pclchar->GetY();
							// ITEM 정보 
							clSearchShopData.m_clItem[siCount].Set(&pclchar->clShopData.m_clItem[j]);
							clSearchShopData.m_siPrice[siCount]		= pclchar->clShopData.m_siPrice[j];
							
							clSearchShopData.m_siHandPoint[siCount]	= pclchar->clShopData.m_siHandPoint;
							clSearchShopData.m_siSkillLevel[siCount]= pclchar->clShopData.m_siSkillLevel;
							
							siCount++;
						}
					}
				}
			}
		}
	}


	cltGameMsgResponse_SearchPersonalShopItem clinfo(TRUE, &clSearchShopData);
	cltMsg clMsg(GAMEMSG_RESPONSE_SEARCHPERSONALSHOPITEM, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MAKEPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_MakePersonalShop* pclinfo = (cltGameMsgRequest_MakePersonalShop*)pclMsg->cData;
	
	SI32 Shopperid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(Shopperid) == false)return ;
	if(pclCM->IsValidID(id)		   == false)return ;
		
	// 상품을 제작의뢰한 상점주인임을 검사한다.
	cltCharServer* pclcharshopper = (cltCharServer* )pclCM->CR[Shopperid];
	cltCharServer* pclcharbuyer   = (cltCharServer* )pclCM->CR[id];
	if ( pclcharshopper == NULL || pclcharbuyer == NULL )	return;

	cltShopTradeData	*pShoperShopData = &pclcharshopper->clShopData;

	SI32 siSuccess = 1;
	//---------------------------------------------
	// 내가 물건을 제작해주기로한 한 상점의 주인과 상점의 상태를 비교한다.
	//---------------------------------------------
	if(siSuccess > 0)
	{
		if(pclcharshopper->bPersonalShopSwitch == false)					siSuccess = -1;// 상점을 개설하고 있지 않다.
		if(pclcharshopper->clShopData.m_siShopMode != pclinfo->siShopMode)	siSuccess = -1;// 상점의 모드가 서로 다르다.
		if(pclcharshopper->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM)	siSuccess = -1;// 상점의 모드가 MAKEITEM이 아니다.
		if(pclcharshopper->pclCI->GetPersonID()!= pclinfo->siPersonID)		siSuccess = -1;// PERSON ID 가 다르다.
	}

	//---------------------------------------------
	// 내가 제작하고자 하는 정보를 점검한다.
	//---------------------------------------------
	SI32 siShowcaseIndex = 0;
	if(siSuccess > 0)
	{
		if(pclinfo->clItem.siUnique<1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum<1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum>MAX_ITEM_PILE_NUMBER)					siSuccess = -2;	
		if(pclinfo->siPrice < 0 )											siSuccess = -2;	
		if(pclinfo->siPrice > MAX_PRICE_IN_MARKET )							siSuccess = -2;	
		if(pclinfo->siHandPoint <0)											siSuccess = -2;	
		if(pclinfo->siSkillLevel<0)											siSuccess = -2;	

		// showcase index가 틀렸다.
		siShowcaseIndex = pclinfo->siShowcaseIndex;
		if(siShowcaseIndex< 0 || siShowcaseIndex>= MAX_SHOP_MAKEITEM_NUMBER)siSuccess = -2; 
	}
	//---------------------------------------------
	// 내가 제작하고자 하는 정보와 상점주인의 ShowCase 정보를 비교한다.
	//---------------------------------------------
	SI32 siShopperInventoryPos = 0;
	SI32 siShowcaseItemNum = 0;
	if(siSuccess > 0)
	{
		// 아이템이 일치하지 않음.
		if(pclinfo->clItem.IsSame( &pShoperShopData->m_clItem[siShowcaseIndex], false )==FALSE )siSuccess = -3;
		// 제시한 수수료가 일치하지 않을경우 
		if(pclinfo->siPrice != pShoperShopData->m_siPrice[siShowcaseIndex])	siSuccess = -3;
		// 상점가격이 부적절 할때 
		if(pShoperShopData->m_siPrice[siShowcaseIndex] < 0)					siSuccess = -3;
		// 상점의 판매 갯수가 부적절할때 
		siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)											siSuccess = -3;

		if(pclinfo->siHandPoint  != pShoperShopData->m_siHandPoint )		siSuccess = -3;
		if(pclinfo->siSkillLevel != pShoperShopData->m_siSkillLevel )		siSuccess = -3;
	}
	//---------------------------------------------
	// 내가 사고자하는 정보와 상점주인의 Inventory 정보를 비교한다.
	//---------------------------------------------
	SI32 siRealBuyItemNum = 0;
	SI32 siInvenItemNum= 0;
	if(siSuccess > 0)
	{
		// 실제로 구입할수 있는 개수를 구한다.
		// 제작 요구 갯수  > 진열대 갯수  => 가장 작은 개수가 구입이 된다.
		siRealBuyItemNum = pclinfo->clItem.siItemNum;
		SI32 siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)						siSuccess = -4;

		// 가장 작은 갯수를 설정한다.
		if(siRealBuyItemNum > siShowcaseItemNum)siRealBuyItemNum = siShowcaseItemNum;
		if(siRealBuyItemNum <1)							siSuccess = -4;
	}

	//---------------------------------------------
	// 상점주인이 수수료가 있는지 확인 
	//---------------------------------------------
	SI64 siTotalPrice = 0; 
	if(siSuccess > 0)
	{
		SI64 TotalBuyerMoney = (SI64)pclcharshopper->pclCI->clIP.GetMoney();
		siTotalPrice         = (SI64)pclinfo->siPrice * (SI64)siRealBuyItemNum;
		if( siTotalPrice < 0)
		{
			siSuccess = -5;
		}
		else if ( siTotalPrice > TotalBuyerMoney )	
		{
			siSuccess = -5;
		}
	}
	
	
	//---------------------------------------------
	// 제조자가 원하는 능력을 만족하고 있는지 확인 
	//---------------------------------------------
	if(siSuccess > 0)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->clItem.siUnique);
		if(ref <= 0)	return;
		
		// 제조에 필요한 기술을 확보한다. 
		SI32 skillunique = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
		if(skillunique <= 0)return ;

		// 기술 레벨이 부족하거나 손재주가 부족하면, 
		bool bConditionA = pclcharbuyer->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclcharbuyer->pclCI->clCharItem.clItem[0]) < pShoperShopData->m_siSkillLevel;
		bool bConditionB = false;
		if ( IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
		{
			bConditionB = pclcharbuyer->clPB.clTotalBA.GetLuk() < pShoperShopData->m_siHandPoint;
		}
		else
		{
			if ( IsCountrySwitch(Switch_Change_manufacture_request_interface) ) 
			{
				bConditionB = pclcharbuyer->clPB.clTotalBA.GetLuk() < pShoperShopData->m_siHandPoint;
				
			}
			else
			{
				bConditionB = pclcharbuyer->clPB.clTotalBA.GetHnd() < pShoperShopData->m_siHandPoint;
			}
			
		}

		if ( bConditionA || bConditionB )
		{
			siSuccess = -6;
		}
	}
	
	
	//---------------------------------------------------------------
	// Shopper의 물품 -> Buyer의 BuyerInvPos, Buyer의 돈 -> Shopper
	//---------------------------------------------------------------
	// DB Message를 보낸다.
	if(siSuccess > 0)
	{
		//---------------------------------------------
		// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclcharshopper->ItemUseReserveAction();
		pclcharbuyer->ItemUseReserveAction();
		//---------------------------------------------

		//---------------------------------------------
		// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclcharshopper->BulletUseReserveAction();
		pclcharbuyer->BulletUseReserveAction();
	
		//---------------------------------------------------------------
		// DO SOMETHING 태곤 
		//---------------------------------------------------------------

		cltGameMsgRequest_MakeItemInv clinfo(pclinfo->clItem.siUnique, 
			pclinfo->clItem.siItemNum, 
			pclinfo->siCharUnique,
			pclcharbuyer->GetCharUnique(),
			pclinfo->siShopMode,
			pclinfo->siType); // siTotalPrice


		pclCM->CR[id]->siPersonalMakeItemType = pclinfo->siType;

		cltGameMsgResponse_MakeItemSwitch clinfo2( pclinfo->siType );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo2), (BYTE*)&clinfo2);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();

		MakeItemProcess(id, &clinfo);

	}
	else		
	{
		SI32 siFailReason = 0;
		switch(siSuccess)
		{
		case -1: siFailReason = MAKEPERSONALSHOP_FAIL_SHOPPERSTATUS;  break;// 제작실패 
		case -2: siFailReason = MAKEPERSONALSHOP_FAIL_MAKERSTATUS;	  break;// 제작실패 
		case -3: siFailReason = MAKEPERSONALSHOP_FAIL_SHOWCASESTATUS; break;// 제작실패 
		case -4: siFailReason = MAKEPERSONALSHOP_FAIL_MAKE_NOITEM;	  break;// 제작실패 
		case -5: siFailReason = MAKEPERSONALSHOP_FAIL_MAKE_NOMONEY;   break;// 제작실패 
		case -6: siFailReason = MAKEPERSONALSHOP_FAIL_MAKE_NEEDABILITY;break;// 제작실패 
		default: siFailReason = MAKEPERSONALSHOP_FAIL_UNKNOWN;		  break;// 제작실패 
		}
		// Client에게 siSuccess 오류정보를 전송한다.
		cltGameMsgResponse_MakePersonalShop clInfo(id, siFailReason, pclinfo->clItem.siUnique, pclinfo->clItem.siItemNum);
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo2( 0 );
		cltMsg clMsg2(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo2), (BYTE*)&clinfo2);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
	}
		


}


void cltServer::StartPersonalSpecialShop(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_StartPersonalShop* pclinfo = (cltGameMsgRequest_StartPersonalShop*)pclMsg->cData;

	SI32 chatid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(chatid) == false)return ;

	// 이미 진행하고 있다면 종료하고 다시 설정해야한다.
	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[chatid];
	if ( pclchar == NULL )
	{
		return;
	}

	// 선거 유세중에는 불가능하다
	if ( true == pclchar->bIsElectionStump )
	{
		return;
	}

	// 피가 0, 죽었다면 거래 불가.
	if ( false == pclCM->IsAlive(chatid))	return;
	

	// [영훈] 상점을 열 수 없는 지역이라면 리턴
	SI32 siMapIndex = pclchar->GetMapIndex();

	if ( (pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM) && (pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM_WANT) &&  (pclMapManager->IsCanMarketOpenMap(siMapIndex) == false) )
	{
		return;
	}

	SI32 siSuccess		= 1;
	SI32 siTicketPos    = 0;
	SI32 siTicketUnique = (pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM
		||pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT) ? ITEMUNIQUE(24065) : ITEMUNIQUE(24010);	// 개인상점 이용권
	SI32 siTicketNum	= 1;

	bool Special_Premiun = false;

	if(pclinfo->clShopData.m_bPremium == true)
	{
		if(pclClient->IsCountrySwitch(Switch_FourLeaf))
		{
			//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
			if( ITEMUNIQUE(2755) == pclinfo->clShopData.m_clItem[1].siUnique || 
				ITEMUNIQUE(2754) == pclinfo->clShopData.m_clItem[1].siUnique	)	
			{	
				if( pclinfo->clShopData.m_clItem[1].siUnique != siFourLeafUnique )	
				{
					siSuccess = -3;
				}
			}
		}

		// 밀봉 프리미엄이면.
		if( CNPersonalShopDlg::SPECIAL_PREMIUN_TICKET_UNIQUE == pclinfo->clShopData.m_clItem[0].siUnique )
		{
			Special_Premiun = true;

			//귀속전용 티켓 아이템을 담는다.(나중에 사용하기 위해 그리고 이아이템을 이용해 다른 짓을 못하게 하려고.)
			pclinfo->clShopData.m_clSpacialItem.Set(&pclinfo->clShopData.m_clItem[0]);
			pclinfo->clShopData.m_siSelectedSpacialInventoryPos = pclinfo->clShopData.m_siSelectedInventoryPos[0];

			siTicketUnique = ITEMUNIQUE(CNPersonalShopDlg::SPECIAL_PREMIUN_TICKET_UNIQUE);
			cltItem TempItem; 
		    TempItem.Set( &pclinfo->clShopData.m_clItem[1] );
#ifdef _SAFE_MEMORY
			pclinfo->clShopData.m_clItem.ZeroMem();
#else
			ZeroMemory(pclinfo->clShopData.m_clItem, sizeof(pclinfo->clShopData.m_clItem));
#endif
			pclinfo->clShopData.m_clItem[0].Set( &TempItem );

			pclinfo->clShopData.m_siSelectedInventoryPos[0] = pclinfo->clShopData.m_siSelectedInventoryPos[1];
			pclinfo->clShopData.m_siPrice[0]				= pclinfo->clShopData.m_siPrice[1];
			pclinfo->clShopData.m_siCurrentPrice[0]			= pclinfo->clShopData.m_siCurrentPrice[1];
		}
		else
		{
			siTicketUnique = ITEMUNIQUE(24075);
		}
		
	}

	if(bForcePersonalShopSwitch==false)	  	      {	siSuccess = -2;	} // 개인상점 강제 금지 
	else if(pclchar->bPersonalShopSwitch == true) {	siSuccess = 0;	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		if(siTicketNum <= 0)		return;

		///////////////////////////////////////////////////////
		// 이용권 확인 
		siTicketPos = pclCM->CR[chatid]->pclCI->clCharItem.GetItemPos(siTicketUnique);
		if( siTicketPos < PERSONITEM_INV0 || siTicketPos >= MAX_ITEM_PER_PERSON )								
			siSuccess = -1;
		if(siSuccess > 0 && pclCM->CR[chatid]->pclCI->clCharItem.clItem[siTicketPos].siItemNum < siTicketNum)	
			siSuccess = -1;
		//////////////////////////////////////////////////////////////////////////
	}
	
	// 밀봉 프리미엄이 아니면 거래 금지 아이템을 검사한다.
	if( false == Special_Premiun )
	{
		// 이용권이 있는데 올려진 ItemList가 거래 금지목록이면 siSuccess = -3;
		if(siSuccess > 0 && pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM && pclinfo->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM_WANT )
		{
			for(SI32 i = 0; i <MAX_SHOP_ITEM_NUMBER; ++i )
			{
				if(pclinfo->clShopData.m_clItem[i].siUnique)
				{
					SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->clShopData.m_clItem[i].siUnique);
					if(ref > 0)	
					{
						// 거래가능물품인지 검사한다.
						if( pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_NOTTRADE) ||
							//cyj 귀속아이템은 금지
							pclinfo->clShopData.m_clItem[i].IsBelonging(pclItemManager)		||
							pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(8150)	||
							pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(14300)	||
							pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(14400)	||
							pclinfo->clShopData.m_clItem[i].siUnique == ITEMUNIQUE(14500)) // 반격의깃발 등
						{
							siSuccess = -3;
						}
					}
					if(pclClient->IsCountrySwitch(Switch_FourLeaf))
					{
						//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
						if( ITEMUNIQUE(2755) == pclinfo->clShopData.m_clItem[i].siUnique || 
							ITEMUNIQUE(2754) == pclinfo->clShopData.m_clItem[i].siUnique	)	
						{	
							if( pclinfo->clShopData.m_clItem[i].siUnique != siFourLeafUnique )	
							{
								siSuccess = -3;
							}
						}
					}
				}
			}
		}
	}

	// 마을에서 최대 갯수를 넘지 못한다.
	if(siSuccess > 0 )
	{
		if ( pclMapManager->pclVillageManager->IsValidVillage(pclchar->GetMapIndex()) == true )
		{
			CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
			SI32 siShopNum = pclMapManager->pclVillageManager->GetPersonalShopNumber(pclchar->GetMapIndex());
			if(pclminister  && siShopNum >= pclminister->GetMaxShopNum() )
			{
				siSuccess = -4;
			}
		}
	}

	if(siSuccess > 0)
	{
		// 이용권을 사용한 마을의 Unique를 준다.
		SI32 villageunique  = pclCM->CR[id]->GetCurrentVillageUnique();

		cltItem clTicketItem;
		SI16 brareswitch = 0;
		pclClient->pclItemManager->MakeRandItemUnique( siTicketUnique, &clTicketItem, 0, 0, &brareswitch, 0,0,0,0,0,0);
		clTicketItem.SetItemNum(siTicketNum);

		// 귀속전용 이용권 아이템
		if(true == Special_Premiun)    
		{
			// 사용하는 귀속전용 이용권 아이템의 개수는 0개
			siTicketNum = 0;
		}

		// DB로 요청한다. 
		sDBRequest_StartPersonalShop clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, siTicketPos, siTicketUnique, siTicketNum, &pclinfo->clShopData, &clTicketItem);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		
		// 물품 사용 통계를 낸다.(개인상점 이용권)
		pclItemManager->AddUseNum(siTicketUnique, siTicketNum);
		
		//새로운 통계 - 물품(개인상점 이용권) 사용 통계를 낸다.
		//NStatistics_Add("Item", siTicketUnique, "UseNum", siTicketNum);

		//==========================================================
		// 미리 배너는 만들어 놓치 뭐 
		pclchar->SetPersonalShopBanner(&pclinfo->clShopData);
		pclchar->SetPersonalShopData(false, &pclinfo->clShopData);
		//==========================================================
		// 혹시나 해서리 다시 한번 
		pclchar->clShopData.m_siPersonID = pclchar->pclCI->GetPersonID();
		StringCchCopy(pclchar->clShopData.m_szAccountID, MAX_PLAYER_NAME, pclchar->pclCI->clBI.szAccountID);

		pclchar->clShopData.m_clSpacialItem.Set(&pclinfo->clShopData.m_clSpacialItem);						
		pclchar->clShopData.m_siSelectedSpacialInventoryPos = pclinfo->clShopData.m_siSelectedSpacialInventoryPos;
		//==========================================================

		// 제조의뢰 퀘스트
		if ( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM )
		{
			pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_MAKEREQUESTSTART);
		}
	}
	else
	{
		// 설정 정보를 설정한다.
		cltGameMsgResponse_StartPersonalShop clinfo(pclchar->GetCharUnique(), siSuccess, false,PSHOP_MODE_NONE);	// 샵만들기 실패로 모드는 없음 
		cltMsg clMsg(GAMEMSG_RESPONSE_STARTPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 9. 3] // 개인상점 시작 쓰기. 
		//	// param1 == 개인상점의 실패여부, param2 == 프리미엄 여부.
		//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_START, 
		//		0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		//		siSuccess, false, 0, 0, 0, NULL, NULL);
		//}
	}
}
void cltServer::DoMsg_GAMEMSG_REQUEST_MAKEPERSONALSHOP_MANUFACTUER(cltMsg* pclMsg, SI32 id)
{
	// 여기에서 상점 주인은 제작자 - 상점을 클릭한 사람이 제료를 가진 사람 
	cltGameMsgRequest_MakePersonalShop_Manufactuer* pclinfo = (cltGameMsgRequest_MakePersonalShop_Manufactuer*)pclMsg->cData;

	SI32 Shopperid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(Shopperid) == false)return ;
	if(pclCM->IsValidID(id)		   == false)return ;

	// 상품을 제작의뢰한 상점주인임을 검사한다.
	cltCharServer* pclmanufacture = (cltCharServer* )pclCM->CR[Shopperid];	// ->  제작자 
	cltCharServer* pclBuyer   = (cltCharServer* )pclCM->CR[id];			// -> 템을 받을 사람 
	if ( pclmanufacture == NULL || pclBuyer == NULL )	return;

	cltShopTradeData	*pShoperShopData = &pclmanufacture->clShopData;

	SI32 siSuccess = 1;
	//---------------------------------------------
	// 내가 물건을 제작해주기로한 한 상점의 주인과 상점의 상태를 비교한다.
	//---------------------------------------------
	if(siSuccess > 0)
	{
		if(pclmanufacture->bPersonalShopSwitch == false)							siSuccess = -1;// 상점을 개설하고 있지 않다.
		if(pclmanufacture->clShopData.m_siShopMode != pclinfo->siShopMode)			siSuccess = -1;// 상점의 모드가 서로 다르다.
		if(pclmanufacture->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM_WANT)		siSuccess = -1;// 상점의 모드가 PSHOP_MODE_MAKEITEM_WANT이 아니다.
		if(pclmanufacture->pclCI->GetPersonID()!= pclinfo->siPersonID)				siSuccess = -1;// PERSON ID 가 다르다.
		if(pclmanufacture->siPersonalMakeItemType > 0  )							siSuccess = -7;// 현제 제조중이다.
	}

	//---------------------------------------------
	// 내가 제작하고자 하는 정보를 점검한다.
	//---------------------------------------------
	SI32 siShowcaseIndex = 0;
	if(siSuccess > 0)
	{
		if(pclinfo->clItem.siUnique<1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum<1)										siSuccess = -2;	
		if(pclinfo->clItem.siItemNum>MAX_ITEM_PILE_NUMBER)					siSuccess = -2;	
		if(pclinfo->siPrice < 0 )											siSuccess = -2;	
		if(pclinfo->siPrice > MAX_PRICE_IN_MARKET )							siSuccess = -2;	
		if(pclinfo->siHandPoint <0)											siSuccess = -2;	
		if(pclinfo->siSkillLevel<0)											siSuccess = -2;	

		// showcase index가 틀렸다.
		siShowcaseIndex = pclinfo->siShowcaseIndex;
		if(siShowcaseIndex< 0 || siShowcaseIndex>= MAX_SHOP_MAKEITEM_NUMBER)siSuccess = -2; 
	}
	//---------------------------------------------
	// 내가 제작하고자 하는 정보와 상점주인의 ShowCase 정보를 비교한다.
	//---------------------------------------------
	SI32 siShopperInventoryPos = 0;
	SI32 siShowcaseItemNum = 0;
	if(siSuccess > 0)
	{
		// 아이템이 일치하지 않음.
		if(pclinfo->clItem.IsSame( &pShoperShopData->m_clItem[siShowcaseIndex], false )==FALSE )siSuccess = -3;
		// 제시한 수수료가 일치하지 않을경우 
		if(pclinfo->siPrice != pShoperShopData->m_siPrice[siShowcaseIndex])	siSuccess = -3;
		// 상점가격이 부적절 할때 
		if(pShoperShopData->m_siPrice[siShowcaseIndex] < 0)					siSuccess = -3;
		// 상점의 판매 갯수가 부적절할때 
		siShowcaseItemNum = pShoperShopData->m_clItem[siShowcaseIndex].siItemNum;
		if(siShowcaseItemNum < 1)											siSuccess = -3;

		if(pclinfo->siHandPoint  != pShoperShopData->m_siHandPoint )		siSuccess = -3;
		if(pclinfo->siSkillLevel != pShoperShopData->m_siSkillLevel )		siSuccess = -3;
	}
	//---------------------------------------------
	// 제작의뢰를 할사람이 돈이 있나 확인
	//---------------------------------------------
	SI64 siTotalPrice = 0; 
	if(siSuccess > 0)
	{
		SI64 TotalBuyerMoney = (SI64)pclBuyer->pclCI->clIP.GetMoney();
		siTotalPrice         = (SI64)pclinfo->siPrice * (SI64)pclinfo->clItem.siItemNum;
		if( siTotalPrice < 0)
		{
			siSuccess = -5;
		}
		else if ( siTotalPrice > TotalBuyerMoney )	
		{
			siSuccess = -5;
		}
	}
	//---------------------------------------------------------------
	// Shopper의 물품 -> Buyer의 BuyerInvPos, Buyer의 돈 -> Shopper
	//---------------------------------------------------------------
	// DB Message를 보낸다.
	if(siSuccess > 0)
	{
		//---------------------------------------------
		// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclmanufacture->ItemUseReserveAction();
		pclBuyer->ItemUseReserveAction();
		//---------------------------------------------

		//---------------------------------------------
		// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclmanufacture->BulletUseReserveAction();
		pclBuyer->BulletUseReserveAction();

		//---------------------------------------------------------------
		// DO SOMETHING 태곤 
		//---------------------------------------------------------------

		cltGameMsgRequest_MakeItemInv clinfo(pclinfo->clItem.siUnique, pclinfo->clItem.siItemNum,
			pclBuyer->GetCharUnique(),
			pclmanufacture->GetCharUnique(),
			pclinfo->siShopMode,
			pclinfo->siType,false,true); // siTotalPrice
		pclmanufacture->siPersonalMakeItemType = pclinfo->siType;

		cltGameMsgResponse_MakeItemSwitch clinfo2( pclinfo->siType );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo2), (BYTE*)&clinfo2);
		pclmanufacture->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		pclmanufacture->SendNeighbourMyMakeItemStatus();
		MakeItemProcess(pclmanufacture->GetID() , &clinfo);
	}
	else		
	{
		SI32 siFailReason = 0;
		switch(siSuccess)
		{
		case -1: siFailReason = MAKEPERSONALSHOP_FAIL_SHOPPERSTATUS;  break;// 제작실패 
		case -2: siFailReason = MAKEPERSONALSHOP_FAIL_MAKERSTATUS;	  break;// 제작실패 
		case -3: siFailReason = MAKEPERSONALSHOP_FAIL_SHOWCASESTATUS; break;// 제작실패 
		case -4: siFailReason = MAKEPERSONALSHOP_FAIL_MAKE_NOITEM;	  break;// 제작실패 
		case -5: siFailReason = MAKEPERSONALSHOP_FAIL_MAKE_NOMONEY;   break;// 제작실패 
		case -6: siFailReason = MAKEPERSONALSHOP_FAIL_MAKE_NEEDABILITY;break;// 제작실패
		case -7: siFailReason = MAKEPERSONALSHOP_FAIL_NOWMAKINGSTATUS;break;// 제작실패
		default: siFailReason = MAKEPERSONALSHOP_FAIL_UNKNOWN;		  break;// 제작실패 
		}
		//오류 전송시 
		// 모드에 따라 각각 다른 사람에게 한다 
		// 기존의 제작의뢰일 경우
		// 다른 경우에는 재료 주인에게 보낸다 

		if(pclinfo->siShopMode == PSHOP_MODE_MAKEITEM_WANT)
		{
			// Client에게 siSuccess 오류정보를 전송한다.
			cltGameMsgResponse_MakePersonalShop clInfo(id, siFailReason, pclinfo->clItem.siUnique, pclinfo->clItem.siItemNum);
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


			pclCM->CR[id]->siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo2( 0 );
			cltMsg clMsg2(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo2), (BYTE*)&clinfo2);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);
		}
		else
		{
			// Client에게 siSuccess 오류정보를 전송한다.
			cltGameMsgResponse_MakePersonalShop clInfo(id, siFailReason, pclinfo->clItem.siUnique, pclinfo->clItem.siItemNum);
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


			pclCM->CR[id]->siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo2( 0 );
			cltMsg clMsg2(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo2), (BYTE*)&clinfo2);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);
		}
		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
	}
}
void cltServer::DoMsg_GAMEMSG_REQUEST_SEARCHMANUFACTURE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SearchManufacture* pclinfo = (cltGameMsgRequest_SearchManufacture*)pclMsg->cData;

	if(pclCM->IsValidID(id)		   == false)return;
	if(pclinfo->siItemUniqueType <1)		return;

	cltSearchShopData	clSearchShopData;
	//-------------------------------------------
	// 검색 기본데이터를 설정한다.
	//-------------------------------------------
	clSearchShopData.m_siShopMode		 = pclinfo->siShopMode;
	clSearchShopData.m_siItemUniqueType  = pclinfo->siItemUniqueType;
	//-------------------------------------------
	// 사용자를 검색하여 정보를 얻는다.
	//-------------------------------------------
	SI32 siCount = 0;
	for ( SI32 i = 0; i < MAX_PERSON_NUMBER && siCount< MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
	{
		if ( pclCM->IsValidID( i ) && IsPC(i))
		{
			cltCharServer* pclchar = (cltCharServer* )pclCM->CR[i];
			// 개인 상점을 열고 있으면서 같은 ShopMode라면 물품을 검색한다.
			if(pclchar && pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode == pclinfo->siShopMode)
			{
				for(SI32 j = 0; j < MAX_SHOP_ITEM_NUMBER && siCount< MAX_SEARCHSHOP_ITEM_NUMBER; ++j) 
				{
					// 같은 타입의 아이템의 물건을 갖고 있을때 
					SI32 siItemUnique = pclchar->clShopData.m_clItem[j].siUnique;
					if( siItemUnique == 0) continue;

					if( pclItemManager->GetItemType(siItemUnique) == pclinfo->siItemUniqueType &&
						pclchar->clShopData.m_clItem[j].siItemNum > 0)

					{
						if(siCount<MAX_SEARCHSHOP_ITEM_NUMBER)
						{
							// 개인정보 
							StringCchCopy(clSearchShopData.m_szName[siCount], MAX_PLAYER_NAME, (TCHAR *)pclchar->GetName());
							clSearchShopData.m_siLevel[siCount]		= pclchar->pclCI->clIP.GetLevel();
							// 위지정보 
							clSearchShopData.m_siMapIndex[siCount]	= pclchar->GetMapIndex();
							clSearchShopData.m_siMapX[siCount]		= pclchar->GetX();
							clSearchShopData.m_siMapY[siCount]		= pclchar->GetY();
							// ITEM 정보 
							clSearchShopData.m_clItem[siCount].Set(&pclchar->clShopData.m_clItem[j]);
							clSearchShopData.m_siPrice[siCount]		= pclchar->clShopData.m_siPrice[j];

							clSearchShopData.m_siHandPoint[siCount]	= pclchar->clShopData.m_siHandPoint;
							clSearchShopData.m_siSkillLevel[siCount]= pclchar->clShopData.m_siSkillLevel;
							siCount++;
						}
					}
				}
			}
		}
	}
	cltGameMsgResponse_SearchManufacture clinfo(TRUE, &clSearchShopData);
	cltMsg clMsg(GAMEMSG_RESPONSE_SEARCHMANUFACTURE, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

