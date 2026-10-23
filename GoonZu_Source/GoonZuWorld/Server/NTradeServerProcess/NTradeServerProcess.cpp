#include "../Server.h"
#include "../../common/Char/CharManager/CharManager.h"
#include "../../common/Char/CharServer/Char-Server.h"

#include "../DBManager/GameDBManager_World/DBMsg-WorldMoney.h"
#include "../DBManager/GameDBManager_World/DBMsg-Person.h"

#include "../../CommonLogic/WorldTraderMgr/WorldTraderManager.h"

#include "NTradeServerProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../CommonLogic/MsgRval-Define.h"
#include "../../CommonLogic/Msg/MsgType-WorldMoney.h"

#include "../../Server/Minister/MinisterBase.h"
#include "../../Server/Minister/MinisterMgr.h"
#include "../../Server/Minister/Minister-Izo.h"
#include "../../Server/Minister/Minister-Goonzu.h"

extern cltCommonLogic* pclClient;

NTradeServerProcess::NTradeServerProcess()
{
	m_pNTradeServerSession = NULL;
	m_NTradeServerIP[ 0 ] = NULL;
	m_NTradeServerPort = 0;
	m_siReason = 0;
	m_dwCheckLastClock = 0;
	m_siVersion = 0;
	m_bSendVersion = false;
}

NTradeServerProcess::~NTradeServerProcess()
{

}	

void NTradeServerProcess::Init()
{
	m_sf.CreateFactory( &m_sm, 1, 5000 * 1000, 5000 * 1000 );

	m_pNTradeServerSession = m_sm.GetSession(0);

	m_iocp.CreateIOCP( &m_sm, 1 );

#ifdef _DEBUG
	//strcpy( m_NTradeServerIP, "116.127.222.221" );
	strcpy( m_NTradeServerIP, "192.168.100.21" );
#else
	strcpy( m_NTradeServerIP, "222.122.40.126" );
#endif

	m_NTradeServerPort = 1588;

	// 무역서버 초기버젼 100(1.00)
	m_siVersion = 100;
}

void NTradeServerProcess::Run()
{

#ifndef _DEBUG
	if( pclClient->siServiceArea != ConstServiceArea_English && 
		//pclClient->siServiceArea != ConstServiceArea_NHNChina &&
		pclClient->siServiceArea != ConstServiceArea_Japan ) 
		return;
#endif

	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	//운영자 명령으로 트레이드프로세스 정지하도록 하면
	if ( pclserver->bForceStopTradeProcess )
	{
		if (m_pNTradeServerSession->GetState() == SESSION_STATE_CONNECTED ||
			m_pNTradeServerSession->GetState() == SESSION_STATE_ESTABLISHED )
		{
			m_pNTradeServerSession->CloseSocket();
			m_dwCheckLastClock = 0;
		}

		return;
	}

	// 게임서버가 1분동안 패킷을 못 받으면 게임서버에서 세션을 지우자!!
	DWORD currentClock = timeGetTime();
	if( ( 0 != m_dwCheckLastClock ) && ( currentClock - m_dwCheckLastClock > 1000 * 60 * 1 ) )
	{
		m_pNTradeServerSession->CloseSocket();
		m_dwCheckLastClock = 0;
	}

	m_sm.Update();	

	DWORD dwCurrentTick = GetTickCount();

	// 세션이 연결되지 않은 상태이면,
	if( m_pNTradeServerSession->GetState() == SESSION_STATE_NONE )
	{
		SI32 reason = 0;
		if( m_sm.Connect( 0, m_NTradeServerIP, m_NTradeServerPort, &reason ) == m_pNTradeServerSession ) 
		{
			m_siReason = reason;
			m_bSendVersion = false;		// 연결되면 버젼을 전송하기위해 변수를 설정한다.
			m_siVersion = 100;			// 버젼을 100(1.00)으로 초기화한다.
		}
		else
		{
			m_siReason = reason;
		}
	} 
	// 세션이 성공적으로 연결된 상태이면,
	else if( m_pNTradeServerSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		if( m_bSendVersion == false )
		{
			// 무역서버 버젼을 전송한다.
			NTradeServerRequest_Set_ServerVersion clMsg_Version( 200 );		// 200(2.00)
			m_pNTradeServerSession->GetSendBuffer()->Write( (BYTE*)&clMsg_Version );

			m_bSendVersion = true;
		}

		int count = m_pNTradeServerSession->GetRecvBuffer()->GetPacketCount();

		for( int i = 0; i < count; ++i ) {

			// 버퍼에서 패킷을 하나 꺼내온다
			sPacketHeader *pPacket = (sPacketHeader *)m_pNTradeServerSession->GetRecvBuffer()->GetFirstPacket();

			if( pPacket != NULL )
			{										
				// 패킷 커맨드에 따른 처리
				switch( pPacket->usCmd )
				{
				case NTRADESERVERRESPONSE_SET_SELLORDER:
					{
						DoMsg_NTRADESERVERRESPONSE_SET_SELLORDER( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_CANCEL_SELLORDER:
					{
						DoMsg_NTRADESERVERRESPONSE_CANCEL_SELLORDER( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_SET_BUYORDER:
					{
						DoMsg_NTRADESERVERRESPONSE_SET_BUYORDER( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_SET_SELLINFO:
					{
						DoMsg_NTRADESERVERRESPONSE_SET_SELLINFO( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_GET_LISTINFO:
					{
						DoMsg_NTRADESERVERRESPONSE_GET_LISTINFO( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_WITHDRAW_ACU:
					{
						DoMsg_NTRADESERVERRESPONSE_WITHDRAW_ACU( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_GET_SAVEDACU_LIST:
					{
						DoMsg_NTRADESERVERRESPONSE_GET_SAVEDACU_LIST( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_HEARTBEAT:
					{
						DoMsg_NTRADESERVERRESPONSE_HEARTBEAT( pPacket );
					}
					break;

				case NTRADESERVERRESPONSE_SET_SERVERVERSION:
					{
						DoMsg_NTRADESERVERRESPONSE_SET_SERVERVERSION( pPacket );
					}
					break;

				default:
					break;
				}

				// 처리 후 패킷을 버퍼에서 지운다
				m_pNTradeServerSession->GetRecvBuffer()->RemoveFirstPacket();
			}
		}
	}
}

bool NTradeServerProcess::SendMsg(  sPacketHeader *pPacket )
{
	if( m_pNTradeServerSession->GetSendBuffer() == NULL ) return false;

	m_pNTradeServerSession->GetSendBuffer()->Write( (BYTE *)pPacket);
	return true;	
}



BOOL NTradeServerProcess::IsEstablished()
{
	if( m_pNTradeServerSession == NULL ) return FALSE;
	if( m_pNTradeServerSession->GetState() != SESSION_STATE_ESTABLISHED ) return FALSE;

	return TRUE;
}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_SET_SELLORDER( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();

	NTradeServerResponse_Set_SellOrder* pclinfo = (NTradeServerResponse_Set_SellOrder*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclchar = pclserver->pclCM->GetCharServer( pclinfo->siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( pclchar->pclCI->GetPersonID() != pclinfo->clOrder.siPersonID )
	{
		return;
	}


	if ( (pclinfo->siPos < PERSONITEM_INV0) || (pclinfo->siPos >= MAX_ITEM_PER_PERSON) )
	{
		return;
	}

	if ( pclinfo->siResult != 1 )
	{
		pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_TRADESERVER_NOMORE_SELL, 0, 0, pclchar->GetCharUnique() );
		
		// 트레이드중이라고 잠근 것을 풀어주자!
		pclchar->SetNoMoveItemSwitch(pclinfo->siPos,false);

		// 현재 무역 서버와의 거래가 끝났다
		pclchar->bTradeServerUsing = false ;
		return;
	}

	if ( pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].siUnique != pclinfo->clOrder.clitem.siUnique )
	{
		// 트레이드중이라고 잠근 것을 풀어주자!
		pclchar->SetNoMoveItemSwitch(pclinfo->siPos,false);

		// 현재 무역 서버와의 거래가 끝났다
		pclchar->bTradeServerUsing = false ;
		return;
	}

	SI64 fee = pclinfo->clOrder.clitem.siPrice / 100 ;
	if ( fee <= 0 )
	{
		fee = 1;
	}

	if ( pclchar->pclCI->clWorldMoney.siWorldMoney < fee )
	{
		// 트레이드중이라고 잠근 것을 풀어주자!
		pclchar->SetNoMoveItemSwitch(pclinfo->siPos,false);

		// 현재 무역 서버와의 거래가 끝났다
		pclchar->bTradeServerUsing = false ;
		return;
	}

	sDBRequest_TradeServer_SetOrder clinfo( pclinfo->siCharID, pclinfo->clOrder.siPersonID,
		&pclinfo->clOrder, pclinfo->siPos, fee);
	pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clinfo);


	// 리스트를 수정한다.
	pclserver->pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Set(&pclinfo->clOrder);

}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_CANCEL_SELLORDER(sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();

	NTradeServerResponse_Cancel_SellOrder* pclinfo = (NTradeServerResponse_Cancel_SellOrder*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclchar = pclserver->pclCM->GetCharServer( pclinfo->siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( pclchar->pclCI->GetPersonID() != pclinfo->clOrder.siPersonID )
	{
		return;
	}

	//if ( pclchar->CanAddInv(INVMODE_ALL,&clitem,&pos,&cltoitem,&returnvalue) == false )
	//	break;

	if ( pclinfo->siResult == -1 ) // 팔려서 없다!
	{
		pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_TRADESERVER_CANCEL_NOT, 0, 0, pclchar->GetCharUnique() );

		// 예약되어 있던 자리를 해제한다.
		pclchar->SetInputItemReserve(pclinfo->siPos,0);

		// 현재 무역 서버와의 거래가 끝났다
		pclchar->bTradeServerUsing = false ;
		return;
	}
	else if ( pclinfo->siResult != 1 )
	{
		pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_TRADESERVER_CANCEL_ERROR, pclinfo->siResult, 0, pclchar->GetCharUnique() );

		// 예약되어 있던 자리를 해제한다.
		pclchar->SetInputItemReserve(pclinfo->siPos,0);

		// 현재 무역 서버와의 거래가 끝났다
		pclchar->bTradeServerUsing = false ;
		return;
	}

	cltItem clitem;
	clitem.Init();
	clitem.siUnique			= pclinfo->clOrder.clitem.siUnique ;
	clitem.siItemNum		= pclinfo->clOrder.clitem.siItemNum ;
	clitem.Element.siPower	= pclinfo->clOrder.clitem.siPower ;
	clitem.Element.siType	= pclinfo->clOrder.clitem.siType ;
	clitem.clItemCommon.clCommonInfo.uiRareInfo = pclinfo->clOrder.clitem.siRareType ;
	
	SI16 pos				= pclinfo->siPos ;
	SI32 returnvalue		= 0 ;

	SI32 ref = pclserver->pclItemManager->FindItemRefFromUnique(clitem.siUnique);
	if ( ref )
	{
		SI32 itemtype = pclserver->pclItemManager->pclItemInfo[ref]->siType;
		if ( pclserver->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
		{
			//SI16 brareswitch = false ;
			//if( pclserver->pclItemManager->MakeRandItemUnique(pclinfo->clOrder.clitem.siUnique, &clitem ,0, 0,&brareswitch) )
			//{
			//	clitem.Element.siPower = pclinfo->clOrder.clitem.siPower ;
			//	clitem.Element.siType = pclinfo->clOrder.clitem.siType ;
			//	clitem.clItemCommon.clCommonInfo.uiRareInfo = pclinfo->clOrder.clitem.siRareType ;

			//	memcpy(clitem.cData8, pclinfo->clOrder.clitem.szItemData,MAX_ITEM_UNION_BYTE);
			//}
			//else break ;

			memcpy(clitem.cData8, pclinfo->clOrder.clitem.szItemData,MAX_ITEM_UNION_BYTE);
		}

		//clitem.siItemNum = pclinfo->clOrder.clitem.siItemNum ;

		sDBRequest_TradeServer_CancelOrder clinfo( pclinfo->siCharID,pclinfo->clOrder.siPersonID,
			&pclinfo->clOrder,pos,&clitem);
		pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clinfo);
	}

	// 리스트를 수정해준다.
	pclserver->pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Init();

}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_SET_BUYORDER( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();

	NTradeServerResponse_Set_BuyOrder* pclinfo = (NTradeServerResponse_Set_BuyOrder*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( pclinfo->siResult != 1 )
	{
		return;
	}

	cltCharServer* pclchar = pclserver->pclCM->GetCharServer( pclinfo->siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( pclchar->pclCI->GetPersonID() != pclinfo->siPersonID )
	{
		return;
	}


	cltItem clitem;
	clitem.Init();

	SI32 ref = pclserver->pclItemManager->FindItemRefFromUnique(pclinfo->clOrder.clitem.siUnique);
	if ( ref )
	{
		SI32 itemtype = pclserver->pclItemManager->pclItemInfo[ref]->siType ;
		// 오브젝트 물건이냐?
		if ( pclserver->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
		{
			if ( pclserver->pclItemManager->MakeTradeServerObjectForRank(&clitem,pclinfo->clOrder.clitem.siUnique,
				pclinfo->clOrder.clitem.siItemNum,pclinfo->clOrder.clitem.siRank,
				pclinfo->clOrder.clitem.siRareType,pclinfo->clOrder.clitem.siType,
				pclinfo->clOrder.clitem.siPower) == FALSE )
			{
				return;
			}
		}
		else
		{
			clitem.siUnique = pclinfo->clOrder.clitem.siUnique ;
			clitem.siItemNum = pclinfo->clOrder.clitem.siItemNum ;
		}
	}
	else
	{
		return;
	}

		

	//cltItem cltoitem;
	//cltoitem.Init();
	//SI16 pos = 0 ;
	//SI32 returnvalue = 0 ;

	//cltCharServer* pclchar = (cltCharServer*)pclserver->pclCM->CR[pclinfo->siCharID];
	//if ( pclchar == NULL ) break;

	//if ( pclchar->CanAddInv(INVMODE_ALL,&clitem,&pos,&cltoitem,&returnvalue) == false )
	//	break;

	SI16 pos = pclinfo->siPos ;

	char sellerserver[20] = "" ;
	pclserver->GetServerNameFromNation(pclinfo->clOrder.szServerNation,pclinfo->clOrder.siServerIndex,sellerserver);
	if ( sellerserver[0] == NULL )
	{
		return;
	}
	

	//sDBRequest_TradeServer_BuyItem clinfo( pclinfo->siCharID,pclinfo->siPersonID,
	//	&pclinfo->clOrder,pos,&clitem);

	// 다른 지역 특산품일 경우 세금을 부과하지 않고 바로 보낸다.

	if ( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if ( pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) )
		{
			if(pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) == false )
			{
				sDBRequest_TradeServer_BuyItem clinfo( pclinfo->siCharID,pclinfo->siPersonID,
					&pclinfo->clOrder,pos,&clitem,sellerserver);
				pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clinfo);

				// 리스트를 수정해준다.
				pclserver->pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Init();
				return;
			}
		}
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		if ( pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) )
		{
			if(pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) == false )
			{		
				sDBRequest_TradeServer_BuyItem clinfo( pclinfo->siCharID,pclinfo->siPersonID,
					&pclinfo->clOrder,pos,&clitem,sellerserver);
				pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clinfo);


				// 리스트를 수정해준다.
				pclserver->pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Init();
				return;
			}
		}
	}

	// 세금을 부과한다.
	CMinisterIzo *pclminister = (CMinisterIzo *)pclserver->pclMinisterMgr->GetMinister(MINISTER_IZO);
	if ( NULL == pclminister )
	{
		return;
	}

	// 세금이 적용되고 있는가..
	if ( pclminister->GetIsWorldTradeTax() )
	{
		SI64 siPrice				= pclinfo->clOrder.clitem.siPrice;
		SI16 siItemNum				= pclinfo->clOrder.clitem.siItemNum;
		SI64 siAverageWorldMoney	= pclserver->pclDistributionWorldMoney->siAverageWorldMoneyPrice;

		// 주문에 올려진 아이템의 한개당 가격
		GMONEY siOrderPrice = 0;
		if ( 0 < siItemNum )
		{
			siOrderPrice = ( siPrice * siAverageWorldMoney ) / siItemNum ;
		}
		

		// 아이템의 시세
		cltItemPriceUnit* pclprice = pclserver->pclItemManager->GetCurrentPrice(pclinfo->clOrder.clitem.siUnique);
		if ( pclprice == NULL )
		{
			return ;
		}

		SI64 itemprice	= pclprice->siPrice ;

		SI32 siRatio	= 0;

		if ( 0 != itemprice )
		{
			siRatio = (SI32)(siOrderPrice * 100 / itemprice);
		}
		else
		{
			return;
		}

		// 실제 적용될 세율을 구한다.
		SI32 siTotalSpecialTaxRate = 0;

		if ( siRatio < pclminister->GetSpecialTaxRange() )
		{
			siTotalSpecialTaxRate = pclminister->GetSpecialTaxRate() + ( pclminister->GetSpecialTaxRange() - siRatio );
		}
		else
		{
			siTotalSpecialTaxRate = 1;
		}

		GMONEY siTax = ( siPrice * siAverageWorldMoney ) * siTotalSpecialTaxRate / 100;

		if( siTax > 0)
		{
			sDBRequest_ChangeMoney clMsg(pclinfo->siCharID, pclinfo->siPersonID, CHANGE_MONEY_REASON_WORLDTRADEBUY, -siTax);
			pclserver->pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);

			CMinisterGoonzu *pclgoonzu = (CMinisterGoonzu *)pclserver->pclMinisterMgr->GetMinister(MINISTER_GOONZU);
			pclgoonzu->AddTempFinances( siTax );

			pclserver->pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, siTax);
		}
	}

	sDBRequest_TradeServer_BuyItem clinfo( pclinfo->siCharID,pclinfo->siPersonID,
		&pclinfo->clOrder,pos,&clitem,sellerserver);
	pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clinfo);


	// 리스트를 수정해준다.
	pclserver->pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Init();


}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_SET_SELLINFO( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();


	NTradeServerResponse_Set_SellInfo* pclinfo = (NTradeServerResponse_Set_SellInfo*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( pclinfo->siResult != 1 )
	{
		return;
	}

	char buyerserver[20] = "" ;
	pclserver->GetServerNameFromNation( pclinfo->szBuyerServerNation, pclinfo->siBuyerServerIndex, buyerserver );
	if ( buyerserver[0] == NULL )
	{
		return;
	}

	cltTradeServerOrder clTradeOrder;
	clTradeOrder.Set( &pclinfo->clLeftOrder );

	// 새롭게 변경된 무역에서는 팔린 아큐를 바로 지급하지 않고 무역서버에서 갖고 있게된다
	if ( pclClient->IsCountrySwitch(Switch_Trade_Renewal) )
	{
		clTradeOrder.clitem.siPrice = 0;
	}

	sDBRequest_TradeServer_Set_SellInfo clinfo( buyerserver,pclinfo->szBuyerName,pclinfo->szBuyerAccountID,	&clTradeOrder );
	pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clinfo);

}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_GET_LISTINFO( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();

	
	NTradeServerResponse_Get_ListInfo* pclinfo = (NTradeServerResponse_Get_ListInfo*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( pclinfo->siResult == 1 )
	{
		for ( SI32 i = pclinfo->siListStartIndex ; i < pclinfo->siListStartIndex + MAX_TRADESERVER_GET_LIST ; i ++ )
		{
			pclserver->pclWorldTraderManager->clOrder[i].Set(&pclinfo->clOrder[i - pclinfo->siListStartIndex]);
			//pclClient->pclLog->FilePrint(TEXT("test.txt"), "%d %d %d %s %d", pclinfo->clOrder[i].siPersonID, pclinfo->clOrder[i].clitem.siUnique, pclinfo->clOrder[i].clitem.siItemNum, pclinfo->clOrder[i].szServerNation, pclinfo->clOrder[i].siServerIndex );
		}
	}

}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_WITHDRAW_ACU( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();

	NTradeServerResponse_Withdraw_Acu* pclinfo = (NTradeServerResponse_Withdraw_Acu*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	// 1이 아니면 무조건 실패
	if ( 1 != pclinfo->m_siResult )
	{
		return;
	}

	// 아큐를 지급 하기 전에 이곳에서 무역서버와의 접속여부를 다시한번 검사한다
	if ( FALSE == IsEstablished() )
	{
		return;
	}

	SI32 siPersonID		= pclinfo->m_siPersonID;
	SI32 siCharID		= pclinfo->m_siCharID;
	SI64 siLeftSavedAcu	= pclinfo->m_siLeftSavedAcu;
	SI64 siWithdrawAcu	= pclinfo->m_siWithdrawAcu;
	SI32 siReservedNum	= pclinfo->m_siReservedNum;

	sDBRequest_TradeServer_Get_Withdraw_Acu clInfo( siPersonID, siCharID, siLeftSavedAcu, siWithdrawAcu, siReservedNum );
	pclserver->SendDBMsg(DBSELECT_SYSTEM,(sPacketHeader*)&clInfo);

}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_GET_SAVEDACU_LIST( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	m_dwCheckLastClock = timeGetTime();

	NTradeServerResponse_Get_SavedACU_List* pclinfo = (NTradeServerResponse_Get_SavedACU_List*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}
	
	// 1이 아니면 무조건 실패
	if ( 1 != pclinfo->m_siResult )
	{
		return;
	}

	cltCharServer* pclchar = pclserver->pclCM->GetCharServer( pclinfo->m_siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( pclchar->pclCI->GetPersonID() != pclinfo->m_siPersonID )
	{
		return;
	}

	cltGameMsgResponse_TradeServer_Get_SavedACU_List clInfo( pclinfo->m_siSavedACU, pclinfo->m_clAcuList );
	cltMsg clMsg( GAMEMSG_RESPONSE_TRADESERVER_GET_SAVEDACU_LIST, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );

}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_HEARTBEAT(sPacketHeader *pPacket )
{
	NTradeServerResponse_HeartBeat* pclinfo = (NTradeServerResponse_HeartBeat*)pPacket;
	if ( NULL == pclinfo )	{			return;				}

	m_dwCheckLastClock = timeGetTime();

	if( m_siVersion >= 200 )
	{
		// 하트비트 응답을 한다.
		NTradeServerRequest_HeartBeat clMsg_HeartBeat;
		SendMsg((sPacketHeader*)&clMsg_HeartBeat);

		// 하트비트를 받았을 때, SellInfo를 요청한다.
		NTradeServerRequest_Set_SellInfo clMsg;
		SendMsg((sPacketHeader*)&clMsg);
	}
}

void NTradeServerProcess::DoMsg_NTRADESERVERRESPONSE_SET_SERVERVERSION(sPacketHeader *pPacket )
{
	NTradeServerResponse_Set_ServerVersion* pclinfo = (NTradeServerResponse_Set_ServerVersion*)pPacket;
	if ( NULL == pclinfo )	{			return;				}

	m_dwCheckLastClock = timeGetTime();

	m_siVersion = pclinfo->m_siVersion;
}
