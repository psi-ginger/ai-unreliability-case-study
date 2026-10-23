//----------------------------
// 영진 2007/04/13
//----------------------------
#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"


#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Gacha.h"

#include "MsgRval-Define.h"

#include "../Server/GachaManager/GachaManager.h"
#include "../CommonLogic/Msg/MsgType-Gacha.h"

#include "../Server/Gacha2Manager/Gacha2Manager.h"
				
void cltServer::DoMsg_GAMEMSG_REQUEST_GACHAPRODUCT_LIST( cltMsg* pclMsg, SI32 id )
{
	//cltGameMsgRequest_GachaProduct_List* pclinfo = (cltGameMsgRequest_GachaProduct_List*)pclMsg->cData;
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}

	cltItem clGachaProductList[MAX_GACHA_PRODUCT];

	for( SI16 i = 0 ; i < MAX_GACHA_PRODUCT ; i++ )
	{
		clGachaProductList[i].Init();

		clGachaProductList[i].siUnique =	pclGachaManager->m_GachaProduct[0][i].ItemUnique;
		clGachaProductList[i].siItemNum =	pclGachaManager->m_GachaProduct[0][i].ItemNum;
		clGachaProductList[i].uiDateUseDay =	pclGachaManager->m_GachaProduct[0][i].ItemUseDate;
	}
	

	cltGameMsgResponse_GachaProduct_List clinfo(  clGachaProductList , pclchar->pclCI->uiGachaCnt );
	cltMsg clMsg(GAMEMSG_RESPONSE_GACHAPRODUCT_LIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}

//KHY - 1104 -  갓차2 -라쿤 보물 상자. 
void cltServer::DoMsg_GAMEMSG_REQUEST_GACHA2PRODUCT_LIST( cltMsg* pclMsg, SI32 id )
{
	//cltGameMsgRequest_GachaProduct_List* pclinfo = (cltGameMsgRequest_GachaProduct_List*)pclMsg->cData;
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}

	cltItem clGachaProductList[MAX_GACHA2_PRODUCT];

	for( SI16 i = 0 ; i < MAX_GACHA2_PRODUCT ; i++ )
	{
		clGachaProductList[i].Init();

		clGachaProductList[i].siUnique =	pclGacha2Manager->m_GachaProduct[0][i].ItemUnique;
		clGachaProductList[i].siItemNum =	pclGacha2Manager->m_GachaProduct[0][i].ItemNum;
		clGachaProductList[i].uiDateUseDay =	pclGacha2Manager->m_GachaProduct[0][i].ItemUseDate;
	}
	

	cltGameMsgResponse_Gacha2Product_List clinfo(  clGachaProductList , pclchar->pclCI->uiGachaCnt );
	cltMsg clMsg(GAMEMSG_RESPONSE_GACHA2PRODUCT_LIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}


// 현제 쓰지 않는듯 한데 그래도 혹시 모르니까 여기도 한번 보자 
void cltServer::DoMsg_GAMEMSG_REQUEST_GACHABUY( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_GachaBuy* pclinfo = (cltGameMsgRequest_GachaBuy*)pclMsg->cData;
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;
	
	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}	

	// 이미 가차 중이라면 그냥 끝낸다.
	if( pclchar->bNowGachaBuy )
	{
		return;
	}
    
	// 아이템을 넣을 인벤 빈칸이 확보되어 있는지 체크
	if( pclchar->IsBlankInv(1,INVMODE_ALL) == false )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_FULLINVEN, 0, 0, pclchar->GetID());
		return; 
	}



	cltItem clGetItem;
	cltItem clKeyItem;

	// 가차 열쇠, 열쇠 조각의 갯수가 충분한지 체크

	if( pclinfo->siUseKeyKind == GACHA_USE_GOLDKEY )
	{
		clKeyItem.siUnique = GACHA_GOLDKEY_ITEMUNIQUE;
		clKeyItem.siItemNum = 1;
	}
	else if( pclinfo->siUseKeyKind == GACHA_USE_SILVERKEY )
	{
		clKeyItem.siUnique = GACHA_SILVERKEY_ITEMUNIQUE;
		clKeyItem.siItemNum = 1;
	}
	else if( pclinfo->siUseKeyKind == GACHA_USE_BRONZEKEY )
	{
		clKeyItem.siItemNum = 1;

		// 동열쇠 사용의 경우 이벤트 동열쇠를 우선으로 사용한다.
		if( pclchar->pclCI->clCharItem.GetItemNum( GACHA_BRONZEKEY_EVENT_ITEMUNIQUE ) >= clKeyItem.siItemNum )
			clKeyItem.siUnique = GACHA_BRONZEKEY_EVENT_ITEMUNIQUE;
		else
			clKeyItem.siUnique = GACHA_BRONZEKEY_ITEMUNIQUE;
	}
	else if( pclinfo->siUseKeyKind == GACHA_USE_KEYPIECE ) 
	{
		clKeyItem.siUnique = GACHA_KEYPIECE_ITEMUNIQUE;
		clKeyItem.siItemNum = GACHA_KEYPIECE_NEED_NUM;
	}
	else
	{
		return;
	}

	if( pclchar->pclCI->clCharItem.GetItemNum( clKeyItem.siUnique ) < clKeyItem.siItemNum )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID());
		return; 
	}

/*
	SI32 KeyItemPos = pclchar->pclCI->clCharItem.GetItemPos( clKeyItem.siUnique );

	if( KeyItemPos < 0 )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID());
		return; 
	}
*/
	SI32 KeyItemPos = -1 ;
	
	// 필요한 갯수 이상의 아이템이 한슬롯에 존재해야 한다
	for(SI16 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(pclchar->pclCI->clCharItem.clItem[i].siUnique == clKeyItem.siUnique)
		{
			if( pclchar->pclCI->clCharItem.clItem[i].siItemNum >=  clKeyItem.siItemNum )
			{
				KeyItemPos = i ;
				break;
			}
		}
	}

	if( KeyItemPos < 0 )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID());
		return; 
	}
	// 가차로 줄 상품을 만들어 낸다.

	// 최고급 상품이 나올 확률에 대한 보너스 포인트	
	SI32 BonusGachaRate = min( pclchar->pclCI->uiGachaCnt , MAX_GACHA_BONUS_VALUE ); 
	bool GetBestProduct = false;

	// 보너스 포인트에 따라 확률적으로 최그급 상품을 준다.
 	if( BonusGachaRate )
	{
		// 100회 이상 도전시 최고 10%까지 올라간다
		SI32 RandVal1 = TABS(CurrentClock) / 10 % 10;

 		SI32 RandVal = rand()%1000  + RandVal1 * 1000;


		if( BonusGachaRate > RandVal )
		{
			GetBestProduct = true;
			// 골드 등급의 첫번째 상품이 최상급 상품이다.
			clGetItem.siUnique = pclGachaManager->m_GachaProduct[0][0].ItemUnique;
			clGetItem.siItemNum = pclGachaManager->m_GachaProduct[0][0].ItemNum;
			clGetItem.uiDateUseDay = pclGachaManager->m_GachaProduct[0][0].ItemUseDate;

			pclLog->FilePrint( TEXT("Config\\GachaBestItem.txt"), TEXT("Get From Bonus\tBonusCount:%d\tKeyUnique:%d"), pclchar->pclCI->uiGachaCnt, clKeyItem.siUnique );
		}

	}
	

	// 일반 가차 시작. 
	if( GetBestProduct == false )
	{
		// 위의 최고가챠 확률과 설정값이 다름
		SI32 RandVal1 = TABS(CurrentClock) / 10 % 10;

		SI32 RandVal = rand()%100  + RandVal1 * 100;


		SI32 GachaRateSum = 0;
		SI32 GachaKeyIndex = 0;
		//가차 아이템에 따른 확률설정
		if( pclinfo->siUseKeyKind ==  GACHA_USE_GOLDKEY )
		{
			GachaKeyIndex = 0;
		}
		else if( pclinfo->siUseKeyKind ==  GACHA_USE_SILVERKEY )
		{
			GachaKeyIndex = 1;
		}
		else if( pclinfo->siUseKeyKind ==  GACHA_USE_BRONZEKEY )
		{
			GachaKeyIndex = 2;
		}
		else if( pclinfo->siUseKeyKind ==  GACHA_USE_KEYPIECE )
		{
			//[김영훈 수정:2007.11.28 - 종훈씨가 확률을 브론즈와 같도록 수정 요망하였습니다(1에서 2로 수정)]
			GachaKeyIndex = 2;
		}
		else
		{
			return;
		}
		// 가차 최고 상품은 첫번쨰 상품!
		for( SI16 i = 0 ; i < MAX_GACHA_PRODUCT ; i++ )
		{
			// 각 품목의 당첨 확률에 걸린것이라면 
			if(  GachaRateSum <= RandVal  
				&& RandVal <= GachaRateSum + pclGachaManager->m_GachaProduct[GachaKeyIndex][i].GachaRate  )
			{
				// 첫번째 상품에 당첨이라면 최고 상품에 당첨된 것이다
				if(  i == 0 ) 
				{
					GetBestProduct = true;
					pclLog->FilePrint( TEXT("Config\\GachaBestItem.txt"), TEXT("Get From Normal\tBonusCount:%d\tKeyUnique:%d"), pclchar->pclCI->uiGachaCnt, clKeyItem.siUnique );
				}
				
				// 당첨된 상품을 세팅해준다.
				clGetItem.siUnique = pclGachaManager->m_GachaProduct[GachaKeyIndex][i].ItemUnique;
				clGetItem.siItemNum = pclGachaManager->m_GachaProduct[GachaKeyIndex][i].ItemNum;
				clGetItem.uiDateUseDay = pclGachaManager->m_GachaProduct[GachaKeyIndex][i].ItemUseDate;

				break;
			}

			GachaRateSum += pclGachaManager->m_GachaProduct[GachaKeyIndex][i].GachaRate;
		}

	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();
	

	

	SI16 itempos = 0;
	cltItem cltoitem;
	SI32 rtnvalue = 0;

	if( pclchar->CanAddInv(INVMODE_ALL, &clGetItem, &itempos, &cltoitem, &rtnvalue) == true )
	{
		// DB로 가챠 열쇠 혹은 열쇠 조각 소모와 함께 아이템 지급 의뢰

		pclchar->bNowGachaBuy = true;
		
		//---------------------------------------------
		// 열쇠를 소모시킬 자리와 아이템이 들어갈 자리를 예약한다.
		//---------------------------------------------

		pclchar->SetInputItemReserve(itempos , CurrentClock );
		pclchar->SetInputItemReserve(KeyItemPos , CurrentClock );

		UI08 uiGachaCntAddNum = pclGachaManager->GetCachaCntAddNum( pclinfo->siUseKeyKind );
		
		sDBRequest_GachaBuy clMsg( id , pclchar->pclCI->GetPersonID() , &clKeyItem  , &clGetItem , clGetItem.CanPile(pclItemManager) , KeyItemPos , itempos , pclchar->pclCI->uiGachaCnt , GetBestProduct, uiGachaCntAddNum);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_FULLINVEN, 0, 0, pclchar->GetID());
		return;		
	}


}

//KHY - 1104 -  갓차2 -라쿤 보물 상자. 
void cltServer::DoMsg_GAMEMSG_REQUEST_GACHA2BUY( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_GachaBuy* pclinfo = (cltGameMsgRequest_GachaBuy*)pclMsg->cData;
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;
	
	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}
	// 이미 가차 중이라면 그냥 끝낸다.
	if( pclchar->bNowGachaBuy )
	{
		return;
	}

	// 아이템을 넣을 인벤 빈칸이 확보되어 있는지 체크
	if( pclchar->IsBlankInv(1,INVMODE_ALL) == false )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_FULLINVEN, 0, 0, pclchar->GetID());
		return; 
	}



	cltItem clGetItem;
	cltItem clKeyItem;

	// 가차2 열쇠 충분한지 체크

	//KHY - 1104 -  갓차2 -라쿤 보물 상자. 
	if( pclinfo->siUseKeyKind == GACHA2_USE_KEY1)
	{
		clKeyItem.siUnique = GACHA2_RACCOONKEY_ITEMUNIQUE;
		clKeyItem.siItemNum = 1;
	}
	else
	{
		return;
	}

	if( pclchar->pclCI->clCharItem.GetItemNum( clKeyItem.siUnique ) < clKeyItem.siItemNum )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID());
		return; 
	}

	SI32 KeyItemPos = -1 ;
	
	// 필요한 갯수 이상의 아이템이 한슬롯에 존재해야 한다
	for(SI16 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(pclchar->pclCI->clCharItem.clItem[i].siUnique == clKeyItem.siUnique)
		{
			if( pclchar->pclCI->clCharItem.clItem[i].siItemNum >=  clKeyItem.siItemNum )
			{
				KeyItemPos = i ;
				break;
			}
		}
	}

	if( KeyItemPos < 0 )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID());
		return; 
	}
	// 가차로 줄 상품을 만들어 낸다.

	// 최고급 상품이 나올 확률에 대한 보너스 포인트	
	SI32 BonusGachaRate = min( pclchar->pclCI->uiGachaCnt , MAX_GACHA_BONUS_VALUE ); 
	bool GetBestProduct = false;

	// 보너스 포인트에 따라 확률적으로 최그급 상품을 준다.
 	if( BonusGachaRate )
	{
		// 100회 이상 도전시 최고 10%까지 올라간다
		SI32 RandVal1 = TABS(CurrentClock) / 10 % 10;

 		SI32 RandVal = rand()%1000  + RandVal1 * 1000;


		if( BonusGachaRate > RandVal )
		{
			GetBestProduct = true;
			// 골드 등급의 첫번째 상품이 최상급 상품이다.
			clGetItem.siUnique = pclGacha2Manager->m_GachaProduct[0][0].ItemUnique;
			clGetItem.siItemNum = pclGacha2Manager->m_GachaProduct[0][0].ItemNum;
			clGetItem.uiDateUseDay = pclGacha2Manager->m_GachaProduct[0][0].ItemUseDate;

			pclLog->FilePrint( TEXT("Config\\GachaBestItem.txt"), TEXT("Get From Bonus\tBonusCount:%d\tKeyUnique:%d"), pclchar->pclCI->uiGachaCnt, clKeyItem.siUnique );
		}

	}
	

	// 일반 가차 시작. 
	if( GetBestProduct == false )
	{
		// 위의 최고가챠 확률과 설정값이 다름
		SI32 RandVal1 = TABS(CurrentClock) / 10 % 10;

		SI32 RandVal = rand()%100  + RandVal1 * 100;


		SI32 GachaRateSum = 0;
		SI32 GachaKeyIndex = 0;

		//가차 아이템에 따른 확률설정
		if( pclinfo->siUseKeyKind ==  GACHA2_USE_KEY1 )
		{
			//KHY - 1104 -  갓차2 -라쿤 보물 상자.  - 첫번째 인덱스
			GachaKeyIndex = 0;
		}
		else
		{
			return;
		}

		for( SI16 i = 0 ; i < MAX_GACHA2_PRODUCT ; i++ )
		{
			// 각 품목의 당첨 확률에 걸린것이라면 
			if(  GachaRateSum <= RandVal  
				&& RandVal <= GachaRateSum + pclGacha2Manager->m_GachaProduct[GachaKeyIndex][i].GachaRate  )
			{
				// 첫번째 상품에 당첨이라면 최고 상품에 당첨된 것이다
				if(  i == 0 ) 
				{
					GetBestProduct = true;

					pclLog->FilePrint( TEXT("Config\\GachaBestItem.txt"), TEXT("Get From Normal\tBonusCount:%d\tKeyUnique:%d"), pclchar->pclCI->uiGachaCnt, clKeyItem.siUnique );
				}
				
				// 당첨된 상품을 세팅해준다.
				clGetItem.siUnique = pclGacha2Manager->m_GachaProduct[GachaKeyIndex][i].ItemUnique;
				clGetItem.siItemNum = pclGacha2Manager->m_GachaProduct[GachaKeyIndex][i].ItemNum;
				clGetItem.uiDateUseDay = pclGacha2Manager->m_GachaProduct[GachaKeyIndex][i].ItemUseDate;

				break;
			}

			GachaRateSum += pclGacha2Manager->m_GachaProduct[GachaKeyIndex][i].GachaRate;
		}

	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();
	
	SI16 itempos = 0;
	cltItem cltoitem;
	SI32 rtnvalue = 0;

	if( pclchar->CanAddInv(INVMODE_ALL, &clGetItem, &itempos, &cltoitem, &rtnvalue) == true )
	{
		// DB로 가챠 열쇠 혹은 열쇠 조각 소모와 함께 아이템 지급 의뢰

		pclchar->bNowGachaBuy = true;
		
		//---------------------------------------------
		// 열쇠를 소모시킬 자리와 아이템이 들어갈 자리를 예약한다.
		//---------------------------------------------

		pclchar->SetInputItemReserve(itempos , CurrentClock );
		pclchar->SetInputItemReserve(KeyItemPos , CurrentClock );

		UI08 uiGachaCntAddNum = pclGachaManager->GetCachaCntAddNum( pclinfo->siUseKeyKind );
		
		sDBRequest_GachaBuy clMsg( id , pclchar->pclCI->GetPersonID() , &clKeyItem  , &clGetItem , clGetItem.CanPile(pclItemManager) , KeyItemPos , itempos , pclchar->pclCI->uiGachaCnt , GetBestProduct, uiGachaCntAddNum);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_FULLINVEN, 0, 0, pclchar->GetID());
		return;		
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CARRYOVER_GACHA_LIST(cltMsg* pclMsg, SI32 id)
{
	if ( false == IsCountrySwitch(Switch_CarryOverGacha) )
	{
		return;
	}
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}
	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}

	SI32 siTotalProductUnique[ TOTAL_GACHA_PRODUCT ];

	ZeroMemory( siTotalProductUnique, sizeof(siTotalProductUnique) );
	
	// 이달의 가챠
	siTotalProductUnique[0] = pclGachaManager->m_GachaProduct[0][0].ItemUnique;

	// 이월 가챠
	for ( SI32 siIndex=0; siIndex<MAX_CARRYOVER_PRODUCT; siIndex++ )
	{
		siTotalProductUnique[siIndex + 1] = pclGachaManager->GetCarryOverGachaItemUnique( siIndex );
	}
	

	cltGameMsgResponse_CarryOver_Gacha_list clinfo( siTotalProductUnique, pclchar->pclCI->uiGachaCnt );
	cltMsg clMsg( GAMEMSG_RESPONSE_CARRYOVER_GACHA_LIST, sizeof(clinfo), (BYTE*)&clinfo );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_CARRYOVER_SELECTPRODUCT_LIST(cltMsg* pclMsg, SI32 id)
{
	if ( false == IsCountrySwitch(Switch_CarryOverGacha) )
	{
		return;
	}

	cltGameMsgRequest_CarryOver_SelectProduct_List* pclInfo = (cltGameMsgRequest_CarryOver_SelectProduct_List*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}

	if ( false == pclGachaManager->IsPare(pclInfo->m_siSelectIndex, pclInfo->m_siSelectUnique) )
	{
		return;
	}

	// 선택된 가챠의 상품 리스트를 갖고온다
	SI32 siProductList[MAX_GACHA_PRODUCT];

	pclGachaManager->GetGachaProductFromIndex( pclInfo->m_siSelectIndex, pclInfo->m_siSelectKeyKind, siProductList );

	// 클라이언트로 보내준다
	cltGameMsgResponse_CarryOver_SelectProduct_List clinfo( siProductList );
	cltMsg clMsg( GAMEMSG_RESPONSE_CARRYOVER_SELECTPRODUCT_LIST, sizeof(clinfo), (BYTE*)&clinfo );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}

//가차 버튼 누르면여기서 상품을 준다.
void cltServer::DoMsg_GAMEMSG_REQUEST_CARRYOVER_GACHABUY(cltMsg* pclMsg, SI32 id)
{
	if ( false == IsCountrySwitch(Switch_CarryOverGacha) )
	{
		return;
	}
	cltGameMsgRequest_CarryOver_GachaBuy* pclInfo = (cltGameMsgRequest_CarryOver_GachaBuy*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) && bGachaLock == true	 )	// 가차사용 가능 한지 부터 체크 한다. 사용 못할때는 메세지를 넣는다 .
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_LOCK, 0, 0, pclchar->GetID());
		return	;
	}


	// 기본적으로 쓰일 변수들
	SI32 siSelectIndex	= pclInfo->m_siSelectIndex;
	SI32 siKeyKind		= pclInfo->m_siKeyKind;

	// 선택한 인덱스와 최고 아이템이 다르다면 잘못된것
	if ( false == pclGachaManager->IsPare(siSelectIndex, pclInfo->m_siSelectUnique) )
	{
		if( true == IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) )
		{	
			SendServerResponseMsg(0, SRVAL_GACHA_NOTCORRECT_ITEM, 0, 0, pclchar->GetID());	// 가챠 최고 아이템이 다를때 메세지를 날려준다.
		}

		return;
	}

	// 이미 가차 중이라면 그냥 끝낸다.
	if ( true == pclchar->bNowGachaBuy )
	{
		return;
	}

	// 아이템을 넣을 인벤 빈칸이 확보되어 있는지 체크
	if ( false == pclchar->IsBlankInv(1,INVMODE_ALL) )
	{
		SendServerResponseMsg(0, SRVAL_GACHA_FAIL_FULLINVEN, 0, 0, pclchar->GetID());
		return; 
	}

	// 키 아이템이 있는지 검사한다
	SI32 siKeyItemUnique	= 0;
	SI32 siKeyItemNum		= 1;

	switch ( siKeyKind )
	{
	case GACHA_USE_GOLDKEY:			{	siKeyItemUnique = GACHA_GOLDKEY_ITEMUNIQUE;			}	break;
	case GACHA_USE_SILVERKEY:		{	siKeyItemUnique = GACHA_SILVERKEY_ITEMUNIQUE;		}	break;
	case GACHA_USE_BRONZEKEY:	// 동열쇠 같은 경우 이벤트 동열쇠 먼저 체크해본다
		{	
			if ( pclchar->pclCI->clCharItem.GetItemNum(GACHA_BRONZEKEY_EVENT_ITEMUNIQUE) >= siKeyItemNum )
			{
				siKeyItemUnique = GACHA_BRONZEKEY_EVENT_ITEMUNIQUE;
			}
			else
			{
				siKeyItemUnique = GACHA_BRONZEKEY_ITEMUNIQUE;		
			}
		}	
		break;
	case CARRYOVER_USE_GOLDKEY:		{	siKeyItemUnique = CARRYOVER_GOLDKEY_ITEMUNIQUE;		}	break;
	case CARRYOVER_USE_SILVERKEY:	{	siKeyItemUnique = CARRYOVER_SILVERKEY_ITEMUNIQUE;	}	break;
	case CARRYOVER_USE_BRONZEKEY:	{	siKeyItemUnique = CARRYOVER_BRONZEKEY_ITEMUNIQUE;	}	break;
	default:
		{
			return;
		}
		break;
	}

	if ( pclchar->pclCI->clCharItem.GetItemNum(siKeyItemUnique) < siKeyItemNum )
	{
		SendServerResponseMsg( 0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID() );
		return; 
	}

	// 키 아이템의 인벤토리 위치를 얻어온다
	SI32 siKeyItemPos = -1;

	for ( SI32 siPosition=PERSONITEM_INV0; siPosition<(PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON); siPosition++ )
	{
		if ( siKeyItemUnique == pclchar->pclCI->clCharItem.clItem[siPosition].siUnique )
		{
			if ( siKeyItemNum <= pclchar->pclCI->clCharItem.clItem[siPosition].siItemNum )
			{
				siKeyItemPos = siPosition;
				break;
			}
		}
	}

	if ( 0 > siKeyItemPos )
	{
		SendServerResponseMsg( 0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID() );
		return; 
	}

	// 가챠로 아이템 얻기 시작
	cltItem clGetItem;

	// 가챠 시도 횟수에 의한 최고급 상품 얻기
	SI32 siBonusGachaRate	= min( pclchar->pclCI->uiGachaCnt, MAX_GACHA_BONUS_VALUE );
	bool bGetBestProduct	= false;
	
	if ( 0 < siBonusGachaRate )
	{
		// 100회 이상 도전시 최고 10%까지 올라간다
		SI32 siTempVal = TABS(CurrentClock) / 10 % 10;		// (0~9)
		SI32 siRandVal = rand() % 1000 + siTempVal * 1000;	// (0~9999)

		if ( siBonusGachaRate > siRandVal )
		{
			bGetBestProduct = true;
			pclGachaManager->GetGachaItem( siSelectIndex, siKeyKind, 0, &clGetItem );

			pclLog->FilePrint( TEXT("Config\\GachaBestItem.txt"), TEXT("Get From Bonus\tBonusCount:%d\tKeyUnique:%d"), pclchar->pclCI->uiGachaCnt, siKeyItemUnique );
		}
	}

	// 일반 가챠 시작
	if ( false == bGetBestProduct )
	{
		// 위의 최고가챠 확률과 설정값이 다름
		SI32 siTempVal = TABS(CurrentClock) / 10 % 10;		// (0~9)
		SI32 siRandVal = rand() % 100 + siTempVal * 100;	// (0~999)

		SI32 siGachaRateSum = 0;
		SI32 siNowGachaRate	= 0;
		for ( SI32 siProductIndex=0; siProductIndex<MAX_GACHA_PRODUCT; siProductIndex++ )
		{
			siNowGachaRate = pclGachaManager->GetGachaRate( siSelectIndex, siKeyKind, siProductIndex );

			if ( (siGachaRateSum <= siRandVal) && (siRandVal <= (siGachaRateSum+siNowGachaRate)) )
			{
				if ( 0 == siProductIndex )
				{
					bGetBestProduct = true;

					pclLog->FilePrint( TEXT("Config\\GachaBestItem.txt"), TEXT("Get From Normal\tBonusCount:%d\tKeyUnique:%d"), pclchar->pclCI->uiGachaCnt, siKeyItemUnique );
				}

				pclGachaManager->GetGachaItem( siSelectIndex, siKeyKind, siProductIndex, &clGetItem );
				break;
			}

			siGachaRateSum += siNowGachaRate;
		}
	}

	//---------------------------------------------
	// 아이템을 못 받아 왔을경우 그냥 리턴
	//---------------------------------------------
	if ( (0 >= clGetItem.siUnique) || (0 >= clGetItem.siItemNum) )
	{
		return;
	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	SI16	siGetItemPos	= 0;
	cltItem	clToItem;

	SI32	siReturnValue	= 0;

	if ( true == pclchar->CanAddInv(INVMODE_ALL, &clGetItem, &siGetItemPos, &clToItem, &siReturnValue) )
	{
		// 가챠 물품 받기중이라고 설정하고
		pclchar->bNowGachaBuy = true;

		// 가챠 키 소모시킬 아이템 셋팅
		cltItem clKeyItem;

		clKeyItem.Init();
		clKeyItem.siUnique	= siKeyItemUnique;
		clKeyItem.siItemNum	= siKeyItemNum;

		// 받을 아이템 자리와 열쇠 아이템 자리를 예약
		pclchar->SetInputItemReserve( siGetItemPos,	CurrentClock );
		pclchar->SetInputItemReserve( siKeyItemPos,	CurrentClock );

		UI08 uiGachaCntAddNum = pclGachaManager->GetCachaCntAddNum( siKeyKind );

		sDBRequest_GachaBuy clMsg( id, pclchar->pclCI->GetPersonID(), &clKeyItem, &clGetItem, clGetItem.CanPile(pclItemManager), siKeyItemPos, siGetItemPos, pclchar->pclCI->uiGachaCnt, bGetBestProduct, uiGachaCntAddNum );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		SendServerResponseMsg( 0, SRVAL_GACHA_FAIL_FULLINVEN, 0, 0, pclchar->GetID() );
		return;		
	}

}
