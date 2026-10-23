#include "..\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\CommonLogic\Msg\MsgType-PrivateTrade.h"
#include "..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderMgr.h"
#include "..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"
#include "Char\CharManager\CharManager.h"


#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "../../CommonLogic/MsgRval-Define.h"
#include "../../CommonLogic/House/House.h"

#include "MsgType-Structure.h"	// 마을 파산 관련 메세지 처리

//cltGameMsgRequest_PrivateTrade
// 1:1 거래를 요청한다.
void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_Accept *pclPrivateTrade = (cltGameMsgRequest_PrivateTrade_Accept *) pclMsg->cData;

	if(pclCM->IsValidID(id) == FALSE)
	{
		return;
	}
    

	// 존재하지 않는 CharUnique 값을 보내왔음.
	if ( pclPrivateTrade->siToPersonCharUnique <= 0 )
	{
		return;
	}
    
	// 요청한 사람의 CharUnique 값이 이상함.
	if ( pclCM->CR[id]->GetCharUnique() <= 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTrade->siToPersonCharUnique);
	if(pclCM->IsValidID(toid) == FALSE)
	{
		return;
	}

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;
	

	//거래 승락 거부 요청 메시지 or 거래중 메시지. 거래 대상이 된 사람의 선택(t,f)에 따라  다시 되돌아 오는 메시지
	if( pclPrivateTrade->siTradeMode == PRIVATETRADE_TRADE_MODE_REJECT_ACCEPT || pclPrivateTrade->siTradeMode == PRIVATETRADE_TRADE_MODE_ALREADYTRADE
		|| pclPrivateTrade->siTradeMode == PRIVATE_ENCHANT_MODE_REJECT_ACCEPT || pclPrivateTrade->siTradeMode == PRIVATE_ENCHANT_MODE_ALREADYTRADE	)
	{		
		cltGameMsgResponse_PrivateTrade_Accept clGameMsgResponse_PrivateTrade_Accept( pclCM->CR[id]->GetCharUnique(),  pclPrivateTrade->siTradeMode, 0, (TCHAR*)pclCM->CR[ id ]->GetName(), NULL );
		cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_ACCEPT, sizeof( clGameMsgResponse_PrivateTrade_Accept ), (BYTE*)&clGameMsgResponse_PrivateTrade_Accept );

		pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}	
	// 거래 승락 메시지 받음
	else if( pclPrivateTrade->siTradeMode == PRIVATE_ENCHANT_MODE_AGREE_ACCEPT )
	{		
		cltGameMsgResponse_PrivateTrade_Accept clGameMsgResponse_PrivateTrade_Accept( pclCM->CR[id]->GetCharUnique(),  pclPrivateTrade->siTradeMode, 0, (TCHAR*)pclCM->CR[ id ]->GetName(), NULL );
		cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_ACCEPT, sizeof( clGameMsgResponse_PrivateTrade_Accept ), (BYTE*)&clGameMsgResponse_PrivateTrade_Accept );

		pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}

	// 레벨 3 미만에서는 거래 안됨
	if ( pclCM->CR[id]->pclCI->clIP.GetLevel() < 3 || pclCM->CR[toid]->pclCI->clIP.GetLevel() < 3 )
	{
		return;
	}

	TCHAR *CharName1 = (TCHAR*)pclCM->CR[ id ]->GetName();
	if ( CharName1 == NULL || _tcslen ( CharName1 ) < 1 )
	{
		return;
	}

	TCHAR *CharName2 = (TCHAR*)pclCM->CR[ toid ]->GetName();
	if ( CharName2 == NULL || _tcslen ( CharName2 ) < 1 )
	{
		return;
	}

	SI16 CharLevel1 = pclCM->CR[id]->pclCI->clIP.GetLevel();
	SI16 CharLevel2 = pclCM->CR[toid]->pclCI->clIP.GetLevel();

	TCHAR *AccountID1 = (TCHAR*)pclCM->CR[ id ]->pclCI->clBI.szAccountID;
	if ( AccountID1 == NULL || _tcslen( AccountID1 ) < 1 )
	{
		return;
	}

	TCHAR *AccountID2 = (TCHAR*)pclCM->CR[ toid ]->pclCI->clBI.szAccountID;
	if ( AccountID2 == NULL || _tcslen( AccountID2 ) < 1 )
	{
		return;
	}


	// 상대방이 거래 거부 상태인 경우
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[toid];
	if ( pclChar->bPrivateTradeReject )
	{
		SendServerResponseMsg(0, SRVAL_PRIVATETRADE_REJECT,  0, 0, id);
		return;
	}

	// 거래 모드도 아니고, 인첸트 모드도 아닌 요청이 들어온 경우임
	if ( pclPrivateTrade->siTradeMode != PRIVATETRADE_TRADE_MODE && pclPrivateTrade->siTradeMode != PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}


	cltGameMsgResponse_PrivateTrade_Accept clGameMsgResponse_PrivateTrade_Accept( pclCM->CR[id]->GetCharUnique(),  pclPrivateTrade->siTradeMode, CharLevel1, CharName1, AccountID1 );
	cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_ACCEPT, sizeof( clGameMsgResponse_PrivateTrade_Accept ), (BYTE*)&clGameMsgResponse_PrivateTrade_Accept );

	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);

	return;

}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade *pclPrivateTrade = (cltGameMsgRequest_PrivateTrade *) pclMsg->cData;

	// 존재하지 않는 CharUnique 값을 보내왔음.
	if ( pclPrivateTrade->siToPersonCharUnique <= 0 )
	{
		return;
	}

	// 요청한 사람의 CharUnique 값이 이상함.
	if ( pclCM->CR[id]->GetCharUnique() <= 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTrade->siToPersonCharUnique);
	if(toid <= 0) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;

	// 상대방이 거래 거부 상태인 경우
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[toid];
	if ( pclChar->bPrivateTradeReject )
	{
		SendServerResponseMsg(0, SRVAL_PRIVATETRADE_REJECT,  0, 0, id);
		return;
	}
	


	// 레벨 3 미만에서는 거래 안됨
	if ( pclCM->CR[id]->pclCI->clIP.GetLevel() < 3 || pclCM->CR[toid]->pclCI->clIP.GetLevel() < 3 )
	{
		return;
	}

	TCHAR *CharName1 = (TCHAR*)pclCM->CR[ id ]->GetName();
	if ( CharName1 == NULL || _tcslen ( CharName1 ) < 1 )
	{
		return;
	}

	TCHAR *CharName2 = (TCHAR*)pclCM->CR[ toid ]->GetName();
	if ( CharName2 == NULL || _tcslen ( CharName2 ) < 1 )
	{
		return;
	}

	SI16 CharLevel1 = pclCM->CR[id]->pclCI->clIP.GetLevel();
	SI16 CharLevel2 = pclCM->CR[toid]->pclCI->clIP.GetLevel();

	TCHAR *AccountID1 = (TCHAR*)pclCM->CR[ id ]->pclCI->clBI.szAccountID;
	if ( AccountID1 == NULL || _tcslen( AccountID1 ) < 1 )
	{
		return;
	}

	TCHAR *AccountID2 = (TCHAR*)pclCM->CR[ toid ]->pclCI->clBI.szAccountID;
	if ( AccountID2 == NULL || _tcslen( AccountID2 ) < 1 )
	{
		return;
	}

	// 거래 모드도 아니고, 인첸트 모드도 아닌 요청이 들어온 경우임
	if ( pclPrivateTrade->siTradeMode != PRIVATETRADE_TRADE_MODE && pclPrivateTrade->siTradeMode != PRIVATETRADE_ENCHANT_MODE  )
	{
		return;
	}

	SI16 MgrIndex;
	SI16 CountIndex;	

	// 매니저로 부터 할당받지 못한 경우 ( 거래수 초과거나... 기타 데이터가 안 맞는다거나 해서... )
	if ( ! pclPrivateTradeOrderMgr->Set( FromPersonID, ToPersonID, &MgrIndex, &CountIndex, pclPrivateTrade->siTradeMode ) )
	{
		return;
	}

	cltGameMsgResponse_PrivateTrade clGameMsgResponse_PrivateTrade( pclPrivateTrade->siToPersonCharUnique, MgrIndex, CountIndex, pclPrivateTrade->siTradeMode, CharLevel2, CharName2, AccountID2 );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE, sizeof( clGameMsgResponse_PrivateTrade ), (BYTE*)&clGameMsgResponse_PrivateTrade );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);


	clGameMsgResponse_PrivateTrade.siOtherCharUnique = pclCM->CR[id]->GetCharUnique();
	MStrCpy( clGameMsgResponse_PrivateTrade.strCharName, CharName1, MAX_PLAYER_NAME );
	MStrCpy( clGameMsgResponse_PrivateTrade.strAccountID, AccountID1, MAX_PLAYER_NAME );
	clGameMsgResponse_PrivateTrade.siOtherCharLevel = CharLevel1;

	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE, sizeof( clGameMsgResponse_PrivateTrade ), (BYTE*)&clGameMsgResponse_PrivateTrade );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);

	return;
}


void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_CANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_Cancel *pclPrivateTradeCancel = (cltGameMsgRequest_PrivateTrade_Cancel*) pclMsg->cData;

	// 존재하지 않는 CharUnique 값을 보내왔음.
	if ( pclPrivateTradeCancel->siOtherCharUnique <= 0 )
	{
		return;
	}

	// 요청한 사람의 CharUnique 값이 이상함.
	if ( pclCM->CR[id]->GetCharUnique() <= 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeCancel->siOtherCharUnique);
	if(toid <= 0) 
	{
		pclPrivateTradeOrderMgr->Cancel( FromPersonID, pclPrivateTradeCancel->siPrivateTradeOrderMgrIndex, pclPrivateTradeCancel->siPrivateTradeOrderCountIndex );

		cltGameMsgResponse_PrivateTrade_Cancel clGameMsgResponse_PrivateTrade_Cancel;
		cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL, sizeof( clGameMsgResponse_PrivateTrade_Cancel ), (BYTE*)&clGameMsgResponse_PrivateTrade_Cancel );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return ;
	}

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;

	pclPrivateTradeOrderMgr->Cancel( FromPersonID, ToPersonID, pclPrivateTradeCancel->siPrivateTradeOrderMgrIndex, pclPrivateTradeCancel->siPrivateTradeOrderCountIndex );

	cltGameMsgResponse_PrivateTrade_Cancel clGameMsgResponse_PrivateTrade_Cancel;

	cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL, sizeof( clGameMsgResponse_PrivateTrade_Cancel ), (BYTE*)&clGameMsgResponse_PrivateTrade_Cancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);


	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_AddItem *pclPrivateTradeAddItem = (cltGameMsgRequest_PrivateTrade_AddItem *) pclMsg->cData;

	// 존재하지 않는 cha r unique 값 보내왔음.
	if ( pclPrivateTradeAddItem->siOtherCharUnique < 1 )
	{
		return;
	}

	// 요청한 사람의 CharUnique 값이 이상함.
	if ( pclCM->CR[id]->GetCharUnique() <= 0 )
	{
		return;
	}

	// 개수 확인
	if ( pclPrivateTradeAddItem->siAddItemNum < 1 )
	{
		return;
	}

	// 아이템 위치 확인
	if( pclPrivateTradeAddItem->siItemPos < PERSONITEM_INV0 || pclPrivateTradeAddItem->siItemPos >= MAX_ITEM_PER_PERSON )
	{
		return;
	}

	// 트레이드중인 것은 안된다!
	if ( ((cltCharServer*)pclCM->CR[id])->GetNoMoveItemSwitch(pclPrivateTradeAddItem->siItemPos) != 0 )
	{
		return ;
	}

	// 개인 거래 매니저로 부터 할당받은 인덱스 값이 유효한지 체크
	if ( pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex < 1 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeAddItem->siOtherCharUnique);
	if(toid <= 0) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;

	/*
	// 지금 수락상태에서 이 메시지에 올 수가 없지...
	if ( pclPrivateTradeOrderMgr->IsAcceptStatus( FromPersonID, ToPersonID, pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddItem->siPrivateTradeOrderCountIndex ) )
	{
		return;
	}
	*/

	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddItem->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddItem->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddItem->siPrivateTradeOrderCountIndex );

	cltItem* pclfromitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[pclPrivateTradeAddItem->siItemPos];

	// 가지고 있는거 보다 더 많이 올리려고 하면 에러.
	if ( pclfromitem->siItemNum < pclPrivateTradeAddItem->siAddItemNum )
	{
		return;
	}

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 팔수 없다.
	if( ((cltCharServer*)pclCM->CR[id])->bPersonalShopSwitch == true && ((cltCharServer*)pclCM->CR[id])->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( ((cltCharServer*)pclCM->CR[id])->clShopData.m_siSelectedInventoryPos[i] == pclPrivateTradeAddItem->siItemPos	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( ((cltCharServer*)pclCM->CR[id])->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}

		// 귀속전용 개인상점
		if( ((cltCharServer*)pclCM->CR[id])->clShopData.m_siSelectedSpacialInventoryPos == pclPrivateTradeAddItem->siItemPos	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( ((cltCharServer*)pclCM->CR[id])->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}



	cltItem clitem1;
	clitem1.Set(pclfromitem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(&pclPrivateTradeAddItem->clItem);
	clitem2.SetItemNum(1);

	if( clitem1.IsSame(&clitem2, false) == FALSE )
	{
		return ;
	}

/*	SI32 NotUsingData;
	if ( pclItemManager->GetLimitDateItemValue( clitem1.siUnique, &NotUsingData ) )
	{
		return;
	}*/

	//cyj 아이템 몰에서 산 기간제 아이템만 안되도록 수정
	// 기간제 아이템은 안된다.
	//if ( clitem1.uiDateUseDay > 0 )
/*	if ( pclItemManager->IsItemInfoAtb(clitem1.siUnique,ITEMINFOATB_ITEMMALL) == true )
	{
		SendServerResponseMsg(0, SRVAL_NOTRADE_ITEM,  0, 0, id);
		return ;
	}*/

	//kkm ITEMINFOATB_NOTTRADE이면 개인거래 품목에 못올리도록 수정
	if ( pclItemManager->IsItemInfoAtb(clitem1.siUnique,ITEMINFOATB_NOTTRADE) )
	{
		SendServerResponseMsg(0, SRVAL_NOTRADE_ITEM,  0, 0, id);
		return ;
	}

	//cyj 귀속 아이템이면 개인거래 품목에 못올리도록 수정
	if ( clitem1.IsBelonging(pclItemManager))
	{
		SendServerResponseMsg(0, SRVAL_NOTRADE_ITEM,  0, 0, id);
		return ;
	}

	// SJY DEBUG
	if ( clitem1.siUnique == 8150 || clitem1.siUnique == 14300 ||
		 clitem1.siUnique == 14400 || clitem1.siUnique == 14500 ) // 반격의 깃발
	{
		return ;
	}

	// 중첩이 되는 아이템인 경우
	if ( clitem2.CanPile( pclItemManager ) )
	{
		clitem2.SetItemNum( pclPrivateTradeAddItem->siAddItemNum );
	}
	// 중첩이 안되는 아이템의 경우인데, 2개 이상 Add 할려는 경우
	else if ( pclPrivateTradeAddItem->siAddItemNum > 1 )
	{
		return;
	}

	CPrivateTradeOrderObj *pTemp =	pclPrivateTradeOrderMgr->FindTradeOrderObj( pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddItem->siPrivateTradeOrderCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 인첸트 모드 일 경우에 수리 가능한 아이템만 올려야 하므로...
	if ( pTemp->GetSelcteMode() == PRIVATETRADE_ENCHANT_MODE )
	{
		return;

		/*
		if ( ! clitem2.CanRepair( pclItemManager ) )
		{
			return;
		}
		else
		{
			// 내 물품 목록에 기존에 뭔가가 올라가 있다면 올릴 수 없다.
			stTradeData *pTradeData = pTemp->GetEachPersonTradeOrderInfo()->GetTradeData( FromPersonID );
			for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
			{
				// 수리 모드일 경우에는 물품을 한명이 한개만 올릴 수 있음.
				if ( pTradeData->m_clItem[ i ].siUnique > 0 )
				{
					return;
				}
			}

			// 상대방 물품 목록에 기존에 뭔가가 올라가 있다면 올릴 수 없다.
			pTradeData = pTemp->GetEachPersonTradeOrderInfo()->GetTradeData( ToPersonID );
			for ( i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
			{
				// 수리 모드일 경우에는 물품을 한명이 한개만 올릴 수 있음.
				if ( pTradeData->m_clItem[ i ].siUnique > 0 )
				{
					return;
				}
			}
		}*/
	}

	// 목록에 아이템을 넣고, 목록에 넣은 index 를 받아옮
	SI16 siAddedIndex = pclPrivateTradeOrderMgr->AddItem
		(
		FromPersonID, 
		ToPersonID, 
		pclPrivateTradeAddItem->siPrivateTradeOrderMgrIndex,
		pclPrivateTradeAddItem->siPrivateTradeOrderCountIndex,
		&clitem2,
		pclPrivateTradeAddItem->siItemPos
		);

	if ( siAddedIndex == -1 )		// 아이템 넣을 곳 없거나, 기타 등등의 이유로...
	{
		return;
	}

	// 이 메시지를 보낸 클라이언트에게 
	// 인벤토리에서 거래 목록에 올린만큼 아이템 개수를 빼줌 ( 지워질수도 있다. )
	// 그리고 클라이언트에 목록에 올렸다고 응답해줌. 
	// 위에서 받은 siAddedIndex 값을 넣어서...

	cltGameMsgResponse_PrivateTrade_AddItem clGameMsgResponse_PrivateTradeAddItem( pclCM->CR[id]->GetCharUnique(), siAddedIndex, &clitem2 );
	
	cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_ADDITEM, sizeof( clGameMsgResponse_PrivateTradeAddItem ), (BYTE*)&clGameMsgResponse_PrivateTradeAddItem );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	// 상대방 클라이언트에게 목록에 올린 아이템을 알려줌.
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_DELLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_DelList *pclPrivateTradeDelList = (cltGameMsgRequest_PrivateTrade_DelList*) pclMsg->cData;

	if ( pclPrivateTradeDelList->siDelType < PRIVATETRADE_TYPE_MONEY || pclPrivateTradeDelList->siDelType >= PRIVATETRADE_TYPE_END )
	{
		return;
	}

	if ( pclPrivateTradeDelList->siOtherCharUnique < 1 )
	{
		return;
	}

	if ( pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex < 1 || pclPrivateTradeDelList->siPrivateTradeOrderCountIndex < 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeDelList->siOtherCharUnique);
	if(toid <= 0) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;

	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelList->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelList->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelList->siPrivateTradeOrderCountIndex );

	/*
	switch ( pclPrivateTradeDelList->siDelType )
	{	
	case PRIVATETRADE_TYPE_ITEM:
		{
			cltItem clitem1;
			
			if ( ! pclPrivateTradeOrderMgr->GetOrderIndexItem
				(
				FromPersonID, 
				ToPersonID, 
				pclPrivateTradeDelList->siParam, 
				pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex, 
				pclPrivateTradeDelList->siPrivateTradeOrderCountIndex, 
				&clitem1
				) )
			{
				return;			// 그 위치에 아이템이 존재하지 않는다.
			}
		}
		break;	
	default:
		pclPrivateTradeOrderMgr->DelOrderListIndex
			(
			FromPersonID, 
			ToPersonID, 
			pclPrivateTradeDelList->siDelType,
			pclPrivateTradeDelList->siParam,
			pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex, 
			pclPrivateTradeDelList->siPrivateTradeOrderCountIndex
		);
		return;
	}
	*/

	if ( ! pclPrivateTradeOrderMgr->DelOrderListIndex
			(
			FromPersonID, 
			ToPersonID, 
			pclPrivateTradeDelList->siDelType,
			pclPrivateTradeDelList->siParam,
			pclPrivateTradeDelList->siPrivateTradeOrderMgrIndex, 
			pclPrivateTradeDelList->siPrivateTradeOrderCountIndex
		) )
	{
		return;
	}

	// 삭제 되었음을 통보한다.
	cltGameMsgResponse_PrivateTrade_DelList clGameMsgResponse_PrivateTradeDelList( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeDelList->siDelType, pclPrivateTradeDelList->siParam );
	
	cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_DELLIST, sizeof( clGameMsgResponse_PrivateTradeDelList ), (BYTE*)&clGameMsgResponse_PrivateTradeDelList );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_DELITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_DelItem *pclPrivateTradeDelItem = (cltGameMsgRequest_PrivateTrade_DelItem *) pclMsg->cData;

	if ( pclPrivateTradeDelItem->siDelIndex < 0 )
	{
		return;
	}
	if ( pclPrivateTradeDelItem->siOtherCharUnique < 1 )
	{
		return;
	}

	if ( pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex < 1 || pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex < 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeDelItem->siOtherCharUnique);
	if(toid <= 0) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;

	/*
	// 지금 수락상태에서 이 메시지에 올 수가 없지...
	if ( pclPrivateTradeOrderMgr->IsAcceptStatus( FromPersonID, ToPersonID, pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex ) )
	{
		return;
	}
	*/

	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex );
	
	
	cltItem clitem1;
	

	if ( ! pclPrivateTradeOrderMgr->GetOrderIndexItem
		(
		FromPersonID, 
		ToPersonID, 
		pclPrivateTradeDelItem->siDelIndex, 
		pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, 
		pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex, 
		&clitem1
		) )
	{
		return;			// 그 위치에 아이템이 존재하지 않는다.
	}

	cltItem clitem2;
	clitem2.Set(&pclPrivateTradeDelItem->clItem);

	// 올라간 물건이 숫자가 틀리다.
	if ( clitem1.siItemNum != clitem2.siItemNum )
	{
		return;
	}
	
	clitem1.SetItemNum(1);
	clitem2.SetItemNum(1);

	if( clitem1.IsSame(&clitem2, false) == FALSE )
	{
		return ;
	}

	// 리스트에서 index에서 삭제한다.
	if ( ! pclPrivateTradeOrderMgr->DelOrderIndexItem
		(
		FromPersonID, 
		ToPersonID, 
		pclPrivateTradeDelItem->siDelIndex, 
		pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, 
		pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex
		) )
	{
		return;
	}

	// 삭제 되었음을 통보한다.
	cltGameMsgResponse_PrivateTrade_DelItem clGameMsgResponse_PrivateTradeDelItem( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeDelItem->siDelIndex );
	
	cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_DELITEM, sizeof( clGameMsgResponse_PrivateTradeDelItem ), (BYTE*)&clGameMsgResponse_PrivateTradeDelItem );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDPRICE( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PrivateTrade_AddPrice *pclPrivateTradeAddPrice = (cltGameMsgRequest_PrivateTrade_AddPrice * )pclMsg->cData;

	if ( pclPrivateTradeAddPrice->siOtherCharUnique < 1 )
	{
		return;
	}

	if ( pclPrivateTradeAddPrice->siPrivateTradeOrderMgrIndex < 1 || pclPrivateTradeAddPrice->siPrivateTradeOrderCountIndex < 0 )
	{
		return;
	}

	if ( pclPrivateTradeAddPrice->siPrice < 1 || pclPrivateTradeAddPrice->siPrice > pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") )
	{
		return;
	}

	GMONEY FromOriPersonMoney = pclCM->CR[id]->pclCI->clIP.GetMoney();

	if ( FromOriPersonMoney < pclPrivateTradeAddPrice->siPrice )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeAddPrice->siOtherCharUnique);
	if(toid <= 0) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;

	/*
	// 지금 수락상태에서 이 메시지에 올 수가 없지...
	if ( pclPrivateTradeOrderMgr->IsAcceptStatus( FromPersonID, ToPersonID, pclPrivateTradeDelItem->siPrivateTradeOrderMgrIndex, pclPrivateTradeDelItem->siPrivateTradeOrderCountIndex ) )
	{
		return;
	}
	*/

	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeAddPrice->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddPrice->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeAddPrice->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddPrice->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeAddPrice->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddPrice->siPrivateTradeOrderCountIndex );

	// 돈 입력은 1번만 가능. 변경은 창을 닫은 후에 가능
	if ( ! pclPrivateTradeOrderMgr->PrivateTradeSetPrice( FromPersonID, ToPersonID, pclPrivateTradeAddPrice->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddPrice->siPrivateTradeOrderCountIndex, pclPrivateTradeAddPrice->siPrice ) )
	{
		return;
	}
	/*
	clGameMsgResponse_PrivateTrade.siOtherCharUnique = pclPrivateTrade->siToPersonCharUnique;
	
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE, sizeof( clGameMsgResponse_PrivateTrade ), (BYTE*)&clGameMsgResponse_PrivateTrade );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	
	clGameMsgResponse_PrivateTrade.siOtherCharUnique = pclCM->CR[id]->GetCharUnique();

	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE, sizeof( clGameMsgResponse_PrivateTrade ), (BYTE*)&clGameMsgResponse_PrivateTrade );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);
	*/

	cltGameMsgResponse_PrivateTrade_AddPrice clPrivateTradeAddPrice1(  pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddPrice->siPrice );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_ADDPRICE, sizeof( clPrivateTradeAddPrice1 ), (BYTE*)&clPrivateTradeAddPrice1 );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	cltGameMsgResponse_PrivateTrade_AddPrice clPrivateTradeAddPrice2( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddPrice->siPrice );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_ADDPRICE, sizeof( clPrivateTradeAddPrice2 ), (BYTE*)&clPrivateTradeAddPrice2 );	
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDSTOCK( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PrivateTrade_AddStock *pclPrivateTradeAddStock = (cltGameMsgRequest_PrivateTrade_AddStock * )pclMsg->cData;
	
	if ( pclPrivateTradeAddStock->siOtherCharUnique < 1 )
	{
		return;
	}

	if ( pclPrivateTradeAddStock->siPrivateTradeOrderMgrIndex < 1 || pclPrivateTradeAddStock->siPrivateTradeOrderCountIndex < 0 )
	{
		return;
	}

	if ( pclPrivateTradeAddStock->siVillageUnique < 1 || pclVillageManager->pclVillageInfo[pclPrivateTradeAddStock->siVillageUnique] == NULL )
	{
		return;
	}

	if ( pclPrivateTradeAddStock->siStockNum < 1 || pclPrivateTradeAddStock->siStockNum > pclClient->GetGlobalValue("GV_PrivateTradeStockLimit") )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0 ) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeAddStock->siOtherCharUnique);
	if( toid <= 0 ) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if( ToPersonID <= 0 ) return ;
	
	// 수리 모드 일 경우에 수리 가능한 아이템만 올려야 하므로...
	CPrivateTradeOrderObj *pTemp =	pclPrivateTradeOrderMgr->FindTradeOrderObj( pclPrivateTradeAddStock->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddStock->siPrivateTradeOrderCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 인첸트 모드에서는 주식 거래 안됨.
	if ( pTemp->GetSelcteMode() == PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}

	// 주식 가진수 이상을 거래할려고 함.
	if ( pclPrivateTradeAddStock->siStockNum > pclCM->CR[id]->pclCI->clStock.clInfo.GetStockAmount( pclPrivateTradeAddStock->siVillageUnique ) )
	{
		return;
	}

//#if defined(_CITYHALL_BANKRUPTCY)
	// 해당 마을이 파산 상태일 때는 주식 팔자 주문을 올릴 수 없다.
	SI32 targetVillage = pclPrivateTradeAddStock->siVillageUnique;
	cltStrInfo *pTargetStrInfo = GetStrInfo( targetVillage, RANKTYPE_CITYHALL );
	if(pTargetStrInfo == NULL)return ;

	if( pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT )
	{
		// 주식 구입 대상 마을이 파산상태입니다.. => 구입 주문 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( targetVillage,  pTargetStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_STOCK_TRADE );
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}
//#endif


	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeAddStock->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddStock->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeAddStock->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddStock->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeAddStock->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddStock->siPrivateTradeOrderCountIndex );

	
	if ( ! pclPrivateTradeOrderMgr->SetStock( FromPersonID, ToPersonID, pclPrivateTradeAddStock->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddStock->siPrivateTradeOrderCountIndex, pclPrivateTradeAddStock->siVillageUnique, pclPrivateTradeAddStock->siStockNum ) )
	{
		return;
	}
	
	cltGameMsgResponse_PrivateTrade_AddStock clPrivateTradeAddStock1(  pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddStock->siVillageUnique, pclPrivateTradeAddStock->siStockNum );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_ADDSTOCK, sizeof( clPrivateTradeAddStock1 ), (BYTE*)&clPrivateTradeAddStock1 );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	cltGameMsgResponse_PrivateTrade_AddStock clPrivateTradeAddStock2( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddStock->siVillageUnique, pclPrivateTradeAddStock->siStockNum );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_ADDSTOCK, sizeof( clPrivateTradeAddStock2 ), (BYTE*)&clPrivateTradeAddStock2 );	
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);	

	return;
}

// 시전 거래
void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDHOUSEUNIT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PrivateTrade_AddHouseUnit *pclPrivateTradeAddHouseUnit = (cltGameMsgRequest_PrivateTrade_AddHouseUnit *)pclMsg->cData;

	if ( pclPrivateTradeAddHouseUnit->siOtherCharUnique < 1 )
	{
		return;
	}

	if ( pclPrivateTradeAddHouseUnit->siPrivateTradeOrderMgrIndex < 1 || pclPrivateTradeAddHouseUnit->siPrivateTradeOrderCountIndex < 0 )
	{
		return;
	}
	
	// 시전 사용 기본 레벨 조건을 충족시키지 못하면 실패. 
	if( pclCM->CR[id]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE)
		return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0 ) return;


	//-------------------------------------------------------------
	// 그 시전의 주인이 맞는지 확인 함.
	if( pclHouseManager->IsHouseHost( FromPersonID, pclPrivateTradeAddHouseUnit->siHouseVillageUnique, pclPrivateTradeAddHouseUnit->siHouseUnitSlot ) == false )
	{
		return;
	}

	cltHouse* pclhouse = (cltHouse*)pclHouseManager->pclStruct[ pclPrivateTradeAddHouseUnit->siHouseVillageUnique ];

	cltRealEstateUnit* pRealEstateUnit = NULL;
	if( pclhouse != NULL )
	{
		pRealEstateUnit = pclCM->CR[id]->pclCI->clRealEstate.Find( pclPrivateTradeAddHouseUnit->siHouseVillageUnique, REALESTATE_TYPE_HOUSE,pclPrivateTradeAddHouseUnit->siHouseUnitSlot );

		if ( pRealEstateUnit == NULL )
		{
			return;
		}
	}
				
	//-------------------------------------------------------------
	
	SI32 toid = pclCM->GetIDFromCharUnique( pclPrivateTradeAddHouseUnit->siOtherCharUnique );
	if( toid <= 0 ) return;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if( ToPersonID <= 0 ) return;
	

	// 시전 사용 기본 레벨 조건을 충족시키지 못하면 실패. 
	if( pclCM->CR[toid]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE)
	{
		SendServerResponseMsg(0, SRVAL_PRIVATETRADE_HOUSEUNIT_LEVEL_LIMIT,  0, 0, id);
		return;
	}

	// 수리 모드 일 경우에 수리 가능한 아이템만 올려야 하므로...
	CPrivateTradeOrderObj *pTemp =	pclPrivateTradeOrderMgr->FindTradeOrderObj( pclPrivateTradeAddHouseUnit->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 인첸트 모드에서는 시전 거래 안됨.
	if ( pTemp->GetSelcteMode() == PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}


	// 시전에 거래 불가능한 말이 있는지 체크
	cltHouseUnitStorageInfo *  pclHouseUnit = pclHouseManager->GetStgPointer(pclPrivateTradeAddHouseUnit->siHouseVillageUnique, pclPrivateTradeAddHouseUnit->siHouseUnitSlot);
	if( pclHouseUnit == NULL ) return;

	for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER ; i ++ )
	{

		//KHY - 0622 - 군주S 초보자용  망아지 추가.   //KHY - 0911 - 호랑이 추가.
		// PCK : 낙타추가 (08.03.06)
		// PCK : 이벤트용 호랑이 추가 (08.04.01)
		// PCK : 코끼리 추가 (08.06.12)
		// PCK : 백호 추가 (08.08.14)
		// PCK : 오토바이 추가 (08.09.02)
		// PCK : 불해치 추가 (08.12.10)
		// 체험용 백마 : 손성웅-2009.03.17
		// PCK : 바포메트 추가(09.07.30)
		switch ( pclHouseUnit->clHorse[i].siHorseUnique )
		{
			case HORSEUNIQUE_WOLF:
			case HORSEUNIQUE_FOAL:
			case HORSEUNIQUE_TIGER:
			case HORSEUNIQUE_CAMEL:
			case HORSEUNIQUE_TIGER2:
			case HORSEUNIQUE_ELEPHANT:
			case HORSEUNIQUE_WHITETIGER:
			case HORSEUNIQUE_MOTORCYCLE:
			case HORSEUNIQUE_FIREHATCH:
			case HORSEUNIQUE_WHITE_TEST:
			case HORSEUNIQUE_BAPHOMET:
			case HORSEUNIQUE_GRAYWOLF:
			case HORSEUNIQUE_SASURI:
				{
					SendServerResponseMsg(0, SRVAL_PRIVATETRADe_HORSE_CANNOTTRADE,  0, 0, id);
					return;
				}
				break;
		}	// switch ( pclHouseUnit->clHorse[i].siHorseUnique ) 종료
	}	// for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER ; i ++ ) 종료


	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderCountIndex );

	
	if ( ! pclPrivateTradeOrderMgr->SetHouseUnit( FromPersonID, ToPersonID, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHouseUnit->siPrivateTradeOrderCountIndex, pclPrivateTradeAddHouseUnit->siHouseVillageUnique, pclPrivateTradeAddHouseUnit->siHouseUnitSlot ) )
	{
		return;
	}

	cltGameMsgResponse_PrivateTrade_AddHouseUnit clPrivateTradeAddHouseUnit1( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddHouseUnit->siHouseVillageUnique, pclPrivateTradeAddHouseUnit->siHouseUnitSlot, &pRealEstateUnit->clContract );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_ADDHOUSEUNIT, sizeof( clPrivateTradeAddHouseUnit1 ), (BYTE*)&clPrivateTradeAddHouseUnit1 );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	cltGameMsgResponse_PrivateTrade_AddHouseUnit clPrivateTradeAddHouseUnit2( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddHouseUnit->siHouseVillageUnique, pclPrivateTradeAddHouseUnit->siHouseUnitSlot, &pRealEstateUnit->clContract );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_ADDHOUSEUNIT, sizeof( clPrivateTradeAddHouseUnit2 ), (BYTE*)&clPrivateTradeAddHouseUnit2 );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);

	return;
}

// 말 거래
void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDHORSE( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PrivateTrade_AddHorse *pclPrivateTradeAddHorse = (cltGameMsgRequest_PrivateTrade_AddHorse * )pclMsg->cData;
	
	if ( pclPrivateTradeAddHorse->siOtherCharUnique < 1 )
	{
		return;
	}

	if ( pclPrivateTradeAddHorse->siPrivateTradeOrderMgrIndex < 1 || pclPrivateTradeAddHorse->siPrivateTradeOrderCountIndex < 0 )
	{
		return;
	}

	if ( pclPrivateTradeAddHorse->siHorseIndex < 0 || pclPrivateTradeAddHorse->siHorseIndex >= MAX_HORSE_PER_PERSON )
	{
		return;
	}
	
	// 나와 있는 말이 있는지 없는지 체크
	cltHorse *pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
	if ( pclHorse == NULL )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clHorseInfo.siCurrentShowHorse != pclPrivateTradeAddHorse->siHorseIndex )
	{
		return;
	}

	// 거래 불가능한 말이면 거래 못한다고 알려줌 //KHY - 0622 - 군주S 초보자용  망아지 추가.  //KHY - 0911 - 호랑이 추가.
	// PCK : 낙타추가 (08.03.06)
	// PCK : 이벤트용 호랑이 추가 (08.04.01)
	// PCK : 코끼리 추가 (08.06.12)
	// PCK : 백호 추가 (08.08.14)
	// PCK : 오토바이 추가 (08.09.02)
	// PCK : 블해치 추가 (08.12.10)
	// 체험용 백마 : 손성웅-2009.03.17
	// PCK : 바포메트 추가(09.07.30)
	switch ( pclHorse->siHorseUnique )
	{
		case HORSEUNIQUE_WOLF:
		case HORSEUNIQUE_FOAL:
		case HORSEUNIQUE_TIGER:
		case HORSEUNIQUE_CAMEL:
		case HORSEUNIQUE_TIGER2:
		case HORSEUNIQUE_ELEPHANT:
		case HORSEUNIQUE_WHITETIGER:
		case HORSEUNIQUE_MOTORCYCLE:
		case HORSEUNIQUE_FIREHATCH:
		case HORSEUNIQUE_WHITE_TEST:
		case HORSEUNIQUE_BAPHOMET:
		case HORSEUNIQUE_GRAYWOLF:
		case HORSEUNIQUE_SASURI:
			{
				SendServerResponseMsg(0, SRVAL_PRIVATETRADe_HORSE_CANNOTTRADE,  0, 0, id);
				return;
			}
			break;
	}	// switch ( pclHorse->siHorseUnique ) 종료


	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0 ) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeAddHorse->siOtherCharUnique);
	if( toid <= 0 ) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if( ToPersonID <= 0 ) return ;


	// 두 사람의 레벨이 MIN_LEVEL_FOR_HORSE 이상이어야만이 거래가 가능함
	if ( pclCM->CR[id]->pclCI->clIP.GetLevel() < MIN_LEVEL_FOR_HORSE || pclCM->CR[toid]->pclCI->clIP.GetLevel() < MIN_LEVEL_FOR_HORSE )
	{
		return;
	}
	
	// 수리 모드 일 경우에 수리 가능한 아이템만 올려야 하므로...
	CPrivateTradeOrderObj *pTemp =	pclPrivateTradeOrderMgr->FindTradeOrderObj( pclPrivateTradeAddHorse->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHorse->siPrivateTradeOrderCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 인첸트 모드에서는 말 거래 안됨.
	if ( pTemp->GetSelcteMode() == PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}

	cltGameMsgResponse_PrivateTrade_OtherCancel clPrivateTrade_OtherCancel(1);

	cltMsg clMsg3( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPrivateTrade_OtherCancel ), (BYTE*)&clPrivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg3);

	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel(1);

	cltMsg clMsg4( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg4);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeAddHorse->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHorse->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeAddHorse->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHorse->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeAddHorse->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHorse->siPrivateTradeOrderCountIndex );

	if ( ! pclPrivateTradeOrderMgr->SetHorse( FromPersonID, ToPersonID, pclPrivateTradeAddHorse->siPrivateTradeOrderMgrIndex, pclPrivateTradeAddHorse->siPrivateTradeOrderCountIndex, pclPrivateTradeAddHorse->siHorseIndex, pclHorse ) )
	{
		return;
	}

	cltGameMsgResponse_PrivateTrade_AddHorse clPrivateTradeAddHorse1(  pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddHorse->siHorseIndex, pclHorse );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_ADDHORSE, sizeof( clPrivateTradeAddHorse1 ), (BYTE*)&clPrivateTradeAddHorse1 );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	cltGameMsgResponse_PrivateTrade_AddHorse clPrivateTradeAddHorse2( pclCM->CR[id]->GetCharUnique(), pclPrivateTradeAddHorse->siHorseIndex, pclHorse );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_ADDHORSE, sizeof( clPrivateTradeAddHorse2 ), (BYTE*)&clPrivateTradeAddHorse2 );	
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);
	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_MYACCEPT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_MyAccept *pclPrivateTradeMyAccept = (cltGameMsgRequest_PrivateTrade_MyAccept *) pclMsg->cData;
	
	if ( pclPrivateTradeMyAccept->siOtherCharUnique < 1 )
	{
		return;
	}
	
	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1 )  return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeMyAccept->siOtherCharUnique);
	if( toid < 1 || pclCM->IsValidID(toid) == FALSE )
	{
		return;
	}

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID < 1) return ;

	if ( pclCM->CR[id]->GetCharUnique() < 1 )
	{
		return;
	}

	///////////////////////////////////////////////////////////////
	// 60 프레임(3초)내에 두 사용자 모두가 Accept 한다면 둘 다 거래 취소시켜 버림.
	// 절대 60 프레임내에 수락이 일어날수 없으므로....
	if ( ! pclPrivateTradeOrderMgr->IsAvailableAccept( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex ) )
	{
		pclPrivateTradeOrderMgr->Cancel( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );

		cltGameMsgResponse_PrivateTrade_Cancel clGameMsgResponse_PrivateTrade_Cancel;

		cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL, sizeof( clGameMsgResponse_PrivateTrade_Cancel ), (BYTE*)&clGameMsgResponse_PrivateTrade_Cancel );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);

		return;
	}
	///////////////////////////////////////////////////////////////

	if ( ! pclPrivateTradeOrderMgr->IsSameAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, pclPrivateTradeMyAccept->siAcceptCounter ) )
	{
		return;
	}	

//#if defined(_CITYHALL_BANKRUPTCY)
	// 해당 마을이 파산 상태일 때는 주식 매매를 할 수 없다.

	SI32 tmpPerson1StockVillageUnique = 0;
	SI32 tmpPerson1StockNum = 0;

	SI32 tmpPerson2StockVillageUnique = 0;
	SI32 tmpPerson2StockNum = 0;

	if ( ! pclPrivateTradeOrderMgr->GetStockInfo( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &tmpPerson1StockVillageUnique, &tmpPerson1StockNum, &tmpPerson2StockVillageUnique, &tmpPerson2StockNum ) )
	{
		pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_STOCK_INFO_DIFFERENT );
		return;
	}

	// [영훈] 초보자 마을 : 개인간 초보자 마을 주식 거래 못하게 막기
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( (Const_Beginner_Village == tmpPerson1StockVillageUnique) || (Const_Beginner_Village == tmpPerson2StockVillageUnique) )
		{
			pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_STOCK_INFO_DIFFERENT );
			return;
		}
	}

	// tmpPerson2StockVillageUnique : 상대편 주식 마을 번호
	SI32 targetVillage = tmpPerson2StockVillageUnique;
	cltStrInfo *pTargetStrInfo = GetStrInfo( targetVillage, RANKTYPE_CITYHALL );

	if(pTargetStrInfo != NULL)
	{
		if( pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT )
		{
			// 주식 구입 대상 마을이 파산상태입니다.. => 구입 주문 실패 => 알림
			cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( targetVillage,  pTargetStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_STOCK_TRADE );
			cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			return;
		}
	}
//#endif

	
	cltGameMsgResponse_PrivateTrade_MyAccept clGameMsgResponse_PrivateTrade_MyAccept( 1 );
	
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_MYACCEPT, sizeof( clGameMsgResponse_PrivateTrade_MyAccept ), (BYTE*)&clGameMsgResponse_PrivateTrade_MyAccept );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);
	

	cltGameMsgResponse_PrivateTrade_OtherAccept clPrivateTrade_OtherAccept(1);
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERACCEPT, sizeof( clPrivateTrade_OtherAccept ), (BYTE*)&clPrivateTrade_OtherAccept );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2); 

	pclPrivateTradeOrderMgr->SetMyAccept( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );

	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );
	

	// 모두 수락했는 경우임
	if ( pclPrivateTradeOrderMgr->IsAllAccept( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex ) )
	{
		CPrivateTradeOrderObj *pTemp =	pclPrivateTradeOrderMgr->FindTradeOrderObj( pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );

		if ( pTemp == NULL )
		{
			return;
		}

		if ( pTemp->GetSelcteMode() == PRIVATETRADE_TRADE_MODE )
		{	
			// 상대방에게 줄만큼 인벤토리에 빈 공간이 있는지 체크
			cltItem clPerson1ItemList[ MAX_PRIVATETRADEORDER_ITEM_NUM ]; // 1번 유저
			memset( clPerson1ItemList, 0, sizeof( cltItem ) * MAX_PRIVATETRADEORDER_ITEM_NUM );
			
			cltItem clPerson2ItemList[ MAX_PRIVATETRADEORDER_ITEM_NUM ]; // 2번 유저
			memset( clPerson2ItemList, 0, sizeof( cltItem ) * MAX_PRIVATETRADEORDER_ITEM_NUM );

			SI16	siPerson1ItemInventoryPosList[  MAX_PRIVATETRADEORDER_ITEM_NUM ]; // 1번 유저
			memset( siPerson1ItemInventoryPosList, 0, sizeof( SI16 ) * MAX_PRIVATETRADEORDER_ITEM_NUM );

			SI16	siPerson2ItemInventoryPosList[  MAX_PRIVATETRADEORDER_ITEM_NUM ]; // 1번 유저
			memset( siPerson2ItemInventoryPosList, 0, sizeof( SI16 ) * MAX_PRIVATETRADEORDER_ITEM_NUM );

			if ( ! pclPrivateTradeOrderMgr->GetOrderItemList( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, clPerson1ItemList, clPerson2ItemList, siPerson1ItemInventoryPosList, siPerson2ItemInventoryPosList ) )
			{
				return;
			}

			// 아이템 개수 : MAX_PRIVATETRADEORDER_ITEM_NUM
			SI16 Person2ItemPos[MAX_PRIVATETRADEORDER_ITEM_NUM];
			memset( Person2ItemPos, 0, sizeof( SI16 ) * MAX_PRIVATETRADEORDER_ITEM_NUM );

			if(pclCM->CR[toid]->pclCI->clCharItem.CanInputItemList(pclItemManager, clPerson1ItemList, MAX_PRIVATETRADEORDER_ITEM_NUM, Person2ItemPos) == true)
			{
			}
			else
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_PERSON_INVENTORY_FULL );
				return;			// 인벤토리에 추가 안됨.
			}

			bool bPerson2PileSwitch[ MAX_PRIVATETRADEORDER_ITEM_NUM ];
			memset( bPerson2PileSwitch, 0, sizeof( bool ) * MAX_PRIVATETRADEORDER_ITEM_NUM );
			
			for ( SI32 k = 0; k < MAX_PRIVATETRADEORDER_ITEM_NUM; ++k )//중첩되는 아이템 확인.
			{
				if ( clPerson1ItemList[ k ].CanPile( pclItemManager ) )
				{
					bPerson2PileSwitch[ k ] = true;
				}
				else
				{
					bPerson2PileSwitch[ k ] = false;
				}
			}
			

			SI16 Person1ItemPos[MAX_PRIVATETRADEORDER_ITEM_NUM];
			memset( Person1ItemPos, 0, sizeof( SI16 ) * MAX_PRIVATETRADEORDER_ITEM_NUM );

			if(pclCM->CR[id]->pclCI->clCharItem.CanInputItemList(pclItemManager, clPerson2ItemList, MAX_PRIVATETRADEORDER_ITEM_NUM, Person1ItemPos) == true)
			{
			}
			else
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_PERSON_INVENTORY_FULL );
				return;			// 인벤토리에 추가 안됨.
			}

			bool bPerson1PileSwitch[ MAX_PRIVATETRADEORDER_ITEM_NUM ];
			memset( bPerson1PileSwitch, 0, sizeof( bool ) * MAX_PRIVATETRADEORDER_ITEM_NUM );
			
			for ( k = 0; k < MAX_PRIVATETRADEORDER_ITEM_NUM; ++k )
			{
				if ( clPerson2ItemList[ k ].CanPile( pclItemManager ) )
				{
					bPerson1PileSwitch[ k ] = true;
				}
				else
				{
					bPerson1PileSwitch[ k ] = false;
				}
			}


			//////////////////////////////////////////////////////////////////////////
			// 주식 거래 확인
			// 주식이 거래 되는 동안 수정 됐는지 다시 한번 확인 ( 혹시나 거래 중에 여각에서 팔거나 팔렸을 수도 있으므로... )
			if ( ! pclPrivateTradeOrderMgr->IsAvailableStock( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, id, toid ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_STOCK_INFO_DIFFERENT );
				return;
			}

			SI32 Person1StockVillageUnique = 0;
			SI32 Person1StockNum = 0;

			SI32 Person2StockVillageUnique = 0;
			SI32 Person2StockNum = 0;

			if ( ! pclPrivateTradeOrderMgr->GetStockInfo( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &Person1StockVillageUnique, &Person1StockNum, &Person2StockVillageUnique, &Person2StockNum ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_STOCK_INFO_DIFFERENT );
				return;
			}
			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			// 시전 거래 확인
			SI32 HouseVillageUnique1 = 0;
			SI32 HouseUnitSlot1 = 0;

			SI32 HouseVillageUnique2 = 0;
			SI32 HouseUnitSlot2 = 0;

			if ( ! pclPrivateTradeOrderMgr->GetHouseUnitInfo( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &HouseVillageUnique1, &HouseUnitSlot1,  &HouseVillageUnique2, &HouseUnitSlot2 ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HOUSEUNIT_INFO_DIFFERENT );
				return;
			}

			if ( HouseVillageUnique1 > 0 )
			{
				//-------------------------------------------------------------
				// 그 시전의 주인이 맞는지 다시 확인 함.
				if( pclHouseManager->IsHouseHost( FromPersonID, HouseVillageUnique1, HouseUnitSlot1 ) == false )
				{
					pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HOUSEUNIT_INFO_DIFFERENT );
					return;
				}
				//-------------------------------------------------------------
			}

			if ( HouseVillageUnique2 > 0 )
			{
				//-------------------------------------------------------------
				// 그 시전의 주인이 맞는지 다시 확인 함.
				if( pclHouseManager->IsHouseHost( ToPersonID, HouseVillageUnique2, HouseUnitSlot2 ) == false )
				{
					pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HOUSEUNIT_INFO_DIFFERENT );
					return;
				}
				//-------------------------------------------------------------
			}
			//////////////////////////////////////////////////////////////////////////
			


			//////////////////////////////////////////////////////////////////////////			
			// 말 거래 확인
			// 말이 거래 되는 동안 수정 됐는지 다시 한번 확인( 혹시나 중간에 말을 놔 버린 요상한 경우가 있을 수 있으므로.. )
			SI16 siPerson1HorseIndex = -1;
			SI16 siPerson2HorseIndex = -1;

			SI16 siEmpty1HorseIndex = -1;
			SI16 siEmpty2HorseIndex = -1;
			
			
			if ( ! pclPrivateTradeOrderMgr->GetHorseInfo( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &siPerson1HorseIndex, &siPerson2HorseIndex ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HORSE_INFO_DIFFERENT );
				return;
			}

			if ( ! pclPrivateTradeOrderMgr->IsAvailableHorse( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, id, toid ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HORSE_INFO_DIFFERENT );
				return;
			}

			if ( siPerson1HorseIndex != -1 )
			{
				// FromPersonID ( id ) 의 말 인벤이 비어 있는지 체크
				// 말 인벤토리에 아이템이 있는지 확인				
				if ( pclCM->CR[id]->pclCI->clCharItem.IsHorseHaveItem() == true )
				{
					// 말 인벤이 비어 있지 않으므로, 거래 할수 없다는 오류 메시지 날리면서 거래 취소 시킴
					pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HORSE_INVENTORY_NOTEMPTY );
					return;
				}
				
				// 상대방이 말을 넣을 수 있는지 체크
				siEmpty2HorseIndex = pclCM->CR[toid]->pclCI->clHorseInfo.FindEmptyHorseIndex();
				if ( siEmpty2HorseIndex < 0 )
				{
					// 상대방이 말 추가 불가
					pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HORSE_NOTEMPTY );
					return;
				}
			}

			if ( siPerson2HorseIndex != -1 )
			{
				// ToPersonID ( toid )의 말 인벤이 비어 있는지 체크
				if ( pclCM->CR[toid]->pclCI->clCharItem.IsHorseHaveItem() == true )
				{
					// 말 인벤이 비어 있지 않으므로, 거래 할수 없다는 오류 메시지 날리면서 거래 취소 시킴
					pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HORSE_INVENTORY_NOTEMPTY );
					return;
				}

				// 상대방이 말을 넣을 수 있는지 체크
				siEmpty1HorseIndex = pclCM->CR[id]->pclCI->clHorseInfo.FindEmptyHorseIndex();
				if ( siEmpty1HorseIndex < 0 )
				{
					// 상대방이 말 추가 불가
					pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_HORSE_NOTEMPTY );
					return;
				}
			}

			if ( siPerson1HorseIndex != -1 && siPerson2HorseIndex != -1 )
			{
				cltHorse *pPerson1Horse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
				if ( pPerson1Horse == NULL )
				{
					return;
				}

				cltHorse *pPerson2Horse = pclCM->CR[toid]->pclCI->clHorseInfo.GetCurrentShowHorse();
				if ( pPerson2Horse == NULL )
				{
					return;
				}

				if ( pPerson1Horse->szName == NULL || pPerson2Horse->szName == NULL )
				{
					return;
				}
				
				if ( _tcscmp( pPerson1Horse->szName, pPerson2Horse->szName ) == 0 )
				{
					// 이름이 같다면 거래를 완전히 종료 시켜 버림
					pclPrivateTradeOrderMgr->Cancel( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );
					
					cltGameMsgResponse_PrivateTrade_Cancel clGameMsgResponse_PrivateTrade_Cancel;
					cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL, sizeof( clGameMsgResponse_PrivateTrade_Cancel ), (BYTE*)&clGameMsgResponse_PrivateTrade_Cancel );
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);

					SendServerResponseMsg(0, SRVAL_PRIVATETRADE_HORSE_NAME_SAME,  0, 0, id);
					SendServerResponseMsg(0, SRVAL_PRIVATETRADE_HORSE_NAME_SAME,  0, 0, toid);
					
					return;
				}
			}
			//////////////////////////////////////////////////////////////////////////

			
			// 서로 돈을 준다면 주는 돈 이상 현재 가지고 있는지...
			GMONEY Person1Money = pclCM->CR[id]->pclCI->clIP.GetMoney();
			GMONEY Person2Money = pclCM->CR[toid]->pclCI->clIP.GetMoney();

			if ( ! pclPrivateTradeOrderMgr->IsAvailableMoney( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, Person1Money, Person2Money ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_MONEY_INSUFFICIENCY );				
				return;		// 돈이 모자람.
			}

			GMONEY siTradePerson1Money = pclPrivateTradeOrderMgr->GetPersonMoney( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );
			GMONEY siTradePerson2Money = pclPrivateTradeOrderMgr->GetPersonMoney( ToPersonID, FromPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );

			// 아이템이 같은지 체크
			for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
			{
				cltItem clItem;
				
				// i 번째 위치에 아이템이 있느냐...
				if ( pclPrivateTradeOrderMgr->GetOrderIndexItem( FromPersonID, ToPersonID, i, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &clItem ) )
				{
					SI32 siInventoryPos = pclPrivateTradeOrderMgr->GetOrderIndexItemInventoryPos( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, i );

					if ( siInventoryPos < PERSONITEM_INV0 )
					{
						return;
					}

					cltItem* pclfromitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ siInventoryPos ];

					if ( ! clItem.IsSame( pclfromitem, false ) )			// 아이템이 일치하지 않음.
					{
						pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_ITEM_INFO_DIFFERENT );
						return;
					}
				}
			}

			for ( i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
			{
				cltItem clItem;
				
				// i 번째 위치에 아이템이 있느냐...
				if ( pclPrivateTradeOrderMgr->GetOrderIndexItem( ToPersonID, FromPersonID, i, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &clItem ) )
				{
					SI32 siInventoryPos = pclPrivateTradeOrderMgr->GetOrderIndexItemInventoryPos( ToPersonID, FromPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, i );

					if ( siInventoryPos < PERSONITEM_INV0 )
					{						
						return;
					}

					cltItem* pclfromitem = &pclCM->CR[toid]->pclCI->clCharItem.clItem[ siInventoryPos ];

					if ( ! clItem.IsSame( pclfromitem, false ) )			// 아이템이 일치하지 않음.
					{
						pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_ITEM_INFO_DIFFERENT );
						return;
					}
				}
			}
			
			// 거래 쌍방의 예약된 아이템 사용을 처리한다. 
			((cltCharServer*)pclCM->CR[id])->ItemUseReserveAction();
			((cltCharServer*)pclCM->CR[toid])->ItemUseReserveAction();

			//---------------------------------------------
			// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
			//---------------------------------------------
			((cltCharServer*)pclCM->CR[id])->BulletUseReserveAction();
			((cltCharServer*)pclCM->CR[toid])->BulletUseReserveAction();

			// DB 로 거래 요청
			sDBRequest_PrivateTrade clMsg
				(
				FromPersonID, 
				pclCM->CR[id]->GetCharUnique(),
				siTradePerson1Money,
				clPerson1ItemList,
				siPerson1ItemInventoryPosList,
				Person2ItemPos,
				bPerson2PileSwitch,
				Person1StockVillageUnique,
				Person1StockNum,
				HouseVillageUnique1,
				HouseUnitSlot1,
				siPerson1HorseIndex,
				siEmpty1HorseIndex,

				ToPersonID, 
				pclCM->CR[toid]->GetCharUnique(),
				siTradePerson2Money,
				clPerson2ItemList,
				siPerson2ItemInventoryPosList,
				Person1ItemPos,
				bPerson1PileSwitch,
				Person2StockVillageUnique,
				Person2StockNum,
				HouseVillageUnique2,
				HouseUnitSlot2,
				siPerson2HorseIndex,
				siEmpty2HorseIndex
				);
			
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			// 서로 돈을 준다면 주는 돈 이상 현재 가지고 있는지...
			GMONEY Person1Money = pclCM->CR[id]->pclCI->clIP.GetMoney();
			GMONEY Person2Money = pclCM->CR[toid]->pclCI->clIP.GetMoney();

			if ( ! pclPrivateTradeOrderMgr->IsAvailableMoney( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, Person1Money, Person2Money ) )
			{
				pclPrivateTradeOrderMgr->PrivateTradeCancel( FromPersonID, ToPersonID, id, toid, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, SRVAL_PRIVATETRADE_MONEY_INSUFFICIENCY );
				return;		// 돈이 모자람.
			}

			GMONEY siTradePerson1Money = pclPrivateTradeOrderMgr->GetPersonMoney( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );
			GMONEY siTradePerson2Money = pclPrivateTradeOrderMgr->GetPersonMoney( ToPersonID, FromPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex );

			SI16 ItemCount = 0;
			SI32 RepairCharUnique = 0;		// 수리할려는 사람의 CharUnique
			SI32 MyCharUnique = 0;			// 수리를 맡기려는 사람의 CharUnique;
			SI32 RepairItemInventoryPos = 0;

			GMONEY RepairPrice = 0;

			cltItem clRepairItem;
			clRepairItem.Init();
			
			// 아이템이 같은지 체크
			for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
			{
				cltItem clItem;
				
				// i 번째 위치에 아이템이 있느냐...
				if ( pclPrivateTradeOrderMgr->GetOrderIndexItem( FromPersonID, ToPersonID, i, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &clItem ) )
				{
					SI32 siInventoryPos = pclPrivateTradeOrderMgr->GetOrderIndexItemInventoryPos( FromPersonID, ToPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, i );

					if ( siInventoryPos < PERSONITEM_INV0 )
					{
						return;
					}

					cltItem* pclfromitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ siInventoryPos ];

					if ( ! clItem.IsSame( pclfromitem, false ) )			// 아이템이 일치하지 않음.
					{
						return;
					}

					if ( ! clItem.CanRepair( pclItemManager ) )
					{
						return;
					}
					
					++ItemCount;

					// 수리 모드에서는 거래 물품이 1개만 있으므로...
					if ( ItemCount > 1 )
					{
						return;
					}

					RepairItemInventoryPos = siInventoryPos;
					clRepairItem.Set( &clItem );

					// 수리 할려는 유저의 캐릭터 unique 값
					RepairCharUnique = pclCM->CR[toid]->GetCharUnique();
					MyCharUnique = pclCM->CR[id]->GetCharUnique();
					RepairPrice = siTradePerson1Money;
				}
			}

			for ( i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
			{
				cltItem clItem;
				
				// i 번째 위치에 아이템이 있느냐...
				if ( pclPrivateTradeOrderMgr->GetOrderIndexItem( ToPersonID, FromPersonID, i, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, &clItem ) )
				{
					SI32 siInventoryPos = pclPrivateTradeOrderMgr->GetOrderIndexItemInventoryPos( ToPersonID, FromPersonID, pclPrivateTradeMyAccept->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyAccept->siPrivateTradeOrderCountIndex, i );

					if ( siInventoryPos < PERSONITEM_INV0 )
					{
						return;
					}

					cltItem* pclfromitem = &pclCM->CR[toid]->pclCI->clCharItem.clItem[ siInventoryPos ];

					if ( ! clItem.IsSame( pclfromitem, false ) )			// 아이템이 일치하지 않음.
					{
						return;
					}

					if ( ! clItem.CanRepair( pclItemManager ) )
					{
						return;
					}

					++ItemCount;

					// 수리 모드에서는 거래 물품이 1개만 있으므로...
					if ( ItemCount > 1 )
					{
						return;
					}

					RepairItemInventoryPos = siInventoryPos;
					
					clRepairItem.Set( &clItem );

					RepairCharUnique = pclCM->CR[id]->GetCharUnique();
					MyCharUnique = pclCM->CR[toid]->GetCharUnique();
					RepairPrice = siTradePerson2Money;
				}
			}


			//--------- 여기서부터 실제 거래
			SI32 rtnval = 0;
			if( Repair( RepairCharUnique, MyCharUnique, RepairItemInventoryPos, &clRepairItem, RepairPrice, &rtnval ) == false )
			{
				return;
			}
			else		// 수리 성공
			{
				cltRentContract Temp1, Temp2;
				Temp1.Init();
				Temp2.Init();

				cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( 1, 0, 0, 0, 0, 0, 0, &Temp1, 0, 0, -1, pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse() );
				cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ), (BYTE*)&clPrivateTradeEnd );
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd1( 1, 0, 0, 0, 0, 0, 0, &Temp2, 0, 0, -1, pclCM->CR[toid]->pclCI->clHorseInfo.GetCurrentShowHorse() );
				cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd1 ), (BYTE*)&clPrivateTradeEnd1 );
				pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg1);

				pclPrivateTradeOrderMgr->Cancel( FromPersonID );
				pclPrivateTradeOrderMgr->Cancel( ToPersonID );
			}
		}
	}

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_MYCANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PrivateTrade_MyCancel *pclPrivateTradeMyCancel = (cltGameMsgRequest_PrivateTrade_MyCancel *) pclMsg->cData;

	if ( pclPrivateTradeMyCancel->siOtherCharUnique < 1 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID <= 0) return ;

	SI32 toid = pclCM->GetIDFromCharUnique(pclPrivateTradeMyCancel->siOtherCharUnique);
	if(toid <= 0) return ;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID <= 0) return ;


	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel( 1 );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	
	cltGameMsgResponse_PrivateTrade_OtherCancel clPRivateTrade_OtherCancel( 1 );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPRivateTrade_OtherCancel ), (BYTE*)&clPRivateTrade_OtherCancel );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg2);

	pclPrivateTradeOrderMgr->SetMyCancel( FromPersonID, ToPersonID, pclPrivateTradeMyCancel->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyCancel->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->SetMyCancel( ToPersonID, FromPersonID, pclPrivateTradeMyCancel->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyCancel->siPrivateTradeOrderCountIndex );
	pclPrivateTradeOrderMgr->IncreaseAcceptCounter( FromPersonID, ToPersonID, pclPrivateTradeMyCancel->siPrivateTradeOrderMgrIndex, pclPrivateTradeMyCancel->siPrivateTradeOrderCountIndex );

	return;
}
