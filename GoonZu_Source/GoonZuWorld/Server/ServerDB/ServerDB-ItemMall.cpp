//---------------------------------
// 2004/07/26 성준엽
//---------------------------------

#include "Char\CharManager\CharManager.h"

#include "../Server/Server.h"

#include "../../Common/Map/Map.h"
#include "../../Common/Order/Order.h"
#include "../../Common/Event/Event.h"

#include "../../../Server/AuthServer/AuthMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-ItemMall.h"
#include "Skill/Skill-Manager.h"

#include "MsgType-Person.h"
#include "MsgType-Item.h"
#include "MsgRval-Define.h"
#include "Msg/MsgType-ItemMall.h"

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALLEXISTENCE(sPacketHeader* pPacket)
{
	sDBResponse_ItemMallExistence* pclMsg = (sDBResponse_ItemMallExistence*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 )							return ;
	if ( pclCM->IsValidID(id) == false)		return ;
	if ( id != pclMsg->usCharID )			return ;

	if ( pclMsg->siResult == 0 || pclMsg->siPersonID <= 0 )
	{
		SendServerResponseMsg(0, SRVAL_ITEMMALL_FAIL_DBFAIL,  0, 0, id);
		return ;
	}
/*
#ifdef _DEBUG
	pclMsg->bExistence = false ;
#endif
*/
	pclCM->CR[id]->pclCI->clItemMall.Init();
	pclCM->CR[id]->pclCI->clItemMall.SetExistence(pclMsg->bExistence);

	// 클라이언트로 통보한다. 
	cltGameMsgResponse_ItemMallExistence clinfo(pclMsg->bExistence);
	cltMsg clMsg(GAMEMSG_RESPONSE_ITEMMALLEXISTENCE, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return ;
}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALLITEMLIST(sPacketHeader* pPacket)
{
	sDBResponse_ItemMallItemList* pclMsg = (sDBResponse_ItemMallItemList*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 )							return ;
	if ( pclCM->IsValidID(id) == false )	return ;
	if ( id != pclMsg->usCharID )			return ;

	if ( pclMsg->siPersonID <= 0 )
	{
		SendServerResponseMsg(0, SRVAL_ITEMMALL_FAIL_DBFAIL,  0, 0, id);
		return ;
	}

	
	cltItem pclitem[MAX_ITEMNUM_PER_ITEMMALL];
	if ( pclMsg->siResult == 0 )
	{
		pclCM->CR[id]->pclCI->clItemMall.Init();
	}
	else
	{
		for ( int i = 0 ; i < MAX_ITEMNUM_PER_ITEMMALL ; i ++ )
		{
			SI16 brareswitch = 0;
			pclitem[i].Init();
			pclitem[i].Set(&pclMsg->clItem[i]);
			if( pclItemManager->MakeRandItemUnique( pclMsg->clItem[i].siUnique, &pclitem[i], 0, 0, &brareswitch) )
			{
				pclitem[i].SetItemNum(pclMsg->clItem[i].siItemNum) ;
				if ( pclMsg->clItem[i].uiDateUseDay > 0 )
					pclitem[i].uiDateUseDay = pclMsg->clItem[i].uiDateUseDay ;// + 1 ;
				pclMsg->clItem[i].Set(&pclitem[i]);
			}
		}

		pclCM->CR[id]->pclCI->clItemMall.SetItem(pclitem);

		pclCM->CR[id]->pclCI->clItemMall.SetItemID(pclMsg->siItemID);

		pclCM->CR[id]->pclCI->clItemMall.SetTotalItemNum(pclMsg->siTotalItemNum);
		pclCM->CR[id]->pclCI->clItemMall.SetListPage(pclMsg->siListPage);
	}

	// 클라이언트로 통보한다. 
	cltGameMsgResponse_ItemMallItemList clinfo(pclMsg->clItem,pclMsg->siItemID,pclMsg->siTotalItemNum,pclMsg->siListPage);
	cltMsg clMsg(GAMEMSG_RESPONSE_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return ;
}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALLGETITEM(sPacketHeader* pPacket)
{
	sDBResponse_ItemMallGetItem* pclMsg = (sDBResponse_ItemMallGetItem*)pPacket;

	if ( pclMsg->siResult != 1 ) return ;
	if ( pclMsg->siPersonID <= 0 ) return ;	
	if ( pclMsg->siInvPos < 0 || pclMsg->siInvPos > MAX_ITEM_PER_PERSON )	return;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( pclCM->IsValidID(id) == false )	return ;
	if ( id != pclMsg->usCharID ) return ;


	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	if( NULL == pclChar)
		return;

	if ( pclMsg->clItem.siUnique <= 0 || pclMsg->clItem.siItemNum <= 0 )
	{
		SendServerResponseMsg(0,SRVAL_ITEMMALL_FAIL_GETITEM_INFOERROR,0,0,id);
		return ;
	}
	if ( pclChar->pclCI->clCharItem.clItem[pclMsg->siInvPos].siUnique > 0 &&
		 pclItemManager->CanPile(pclMsg->clItem.siUnique) == false )
	{
		SendServerResponseMsg(0,SRVAL_ITEMMALL_FAIL_GETITEM_INFOERROR,1,0,id);
		return ;
	}

	if ( pclItemManager->CanPile(pclMsg->clItem.siUnique) == false && pclMsg->clItem.GetItemNum() != 1 )
	{
		SendServerResponseMsg(0, SRVAL_ITEMMALL_FAIL_GETITEM_NOITEM,  0, 0, id);
		return ;
	}

	// 아이템 받아옴 완료
	if ( pclChar->bIsItemMallGetItem )
		pclChar->bIsItemMallGetItem = false ;

	SI32 siGetItemNum = pclMsg->clItem.GetItemNum() - pclChar->pclCI->clCharItem.clItem[pclMsg->siInvPos].GetItemNum();

	cltItem clGetItem;
	clGetItem.Set(&pclMsg->clItem);
	clGetItem.SetItemNum(siGetItemNum);

	// 통계를 낸다.
	pclItemManager->AddMallItemNum(pclMsg->clItem.siUnique,siGetItemNum);

	// 새로운 통계 - 아이템몰에서 공급 물품의 통계를 낸다.
	NStatistics_Add("Item", pclMsg->clItem.siUnique, "MallItemNum", pclMsg->clItem.GetItemNum());
	NStatistics_Set("Item", pclMsg->clItem.siUnique, "Name", pclItemManager->GetName(pclMsg->clItem.siUnique));

	// 아이템몰  퀘스트 중이라면, 
	pclChar->DoQuestCheckAndAction(QUEST_CLEAR_COND_ITEMMALL);

	if ( pclMsg->bPossession )
	{
		pclChar->pclCI->clCharItem.clItem[pclMsg->siInvPos].Set(&pclMsg->clItem);
	}
	else if ( pclMsg->bPossession == false )
	{
		//if ( pclMsg->clItem.siUnique == 8250 ) // 명성훈장 예외 처리
		//{
			//if ( pclChar->UseItem(pclMsg->clItem) == false )
		//		return ;
		//}
	}

	// 클라이언트로 통보한다. 
	cltGameMsgResponse_ItemMallGetItem clinfo(pclMsg->siInvPos,&pclMsg->clItem);
	cltMsg clMsg(GAMEMSG_RESPONSE_ITEMMALLGETITEM, sizeof(clinfo), (BYTE*)&clinfo);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		//// PCK - 아이템 받을때 남기는 로그 추가함.
		//SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_GET_ITEM,
		//					 INPUTITEMPERSON_OPTION_ITEMMALL, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
		//					 pclMsg->siInvPos, 0, 0, 0, 0, NULL, NULL);

		// [로그추가 : 황진성 2007. 10. 16] // 아이템 몰 수령.
		// param1 == 아이템 아이디, param2 == 아이템 유니크
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ITEMMALL, LOGCOMMAND_INDEX_ITEMMALL_GET, 
							0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &clGetItem, pclMsg->clItem.GetItemNum(), 0, 0, 0, 0, 0, 
							pclMsg->siItemID, pclMsg->clItem.siUnique, 0, 0, 0, NULL, NULL);	
	}

	sDBRequest_PersonItemInfoGet clMsgToDB( id, pclMsg->siPersonID );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsgToDB);

	/*
	// 순간이동정액권 사용처리
	if ( pclMsg->clItem.siUnique == ITEMUNIQUE(14170) || pclMsg->clItem.siUnique == ITEMUNIQUE(14180) ||
		 pclMsg->clItem.siUnique == ITEMUNIQUE(14190) || pclMsg->clItem.siUnique == ITEMUNIQUE(14165) ||
		 pclMsg->clItem.siUnique == ITEMUNIQUE(14168) )
	{
		SI32 adddate = 0 ;

		switch(pclMsg->clItem.siUnique)
		{
		case ITEMUNIQUE(14190):
			{
				adddate = 900 ;
			}
			break;
		case ITEMUNIQUE(14180):
			{
				adddate = 420 ;
			}
			break;
		case ITEMUNIQUE(14170):
			{
				adddate = 210 ;
			}
			break;
		case ITEMUNIQUE(14165):
			{
				adddate = 30 ;
			}
			break;
		case ITEMUNIQUE(14168):
			{
				adddate = 90 ;
			}
			break;
		default:
			return ;
		}

		if ( pclChar->pclCI->clCharItem.GetItemNum(pclMsg->clItem.siUnique) <= 0 )
			return ;

		SI32 pos = pclChar->pclCI->clCharItem.GetItemPos(pclMsg->clItem.siUnique);
		if(pos < 0)return ;

		SI32 datevary = 0 ;
		if ( pclChar->pclCI->clBI.bUseNoLimitTicketWarp )
		{
			datevary = pclChar->pclCI->clBI.siNoLimitTicketWarpDateVary + adddate * pclMsg->clItem.siItemNum ;
		}
		else
		{
			datevary = pclTime->clDate.GetDateVary() + adddate * pclMsg->clItem.siItemNum ;
		}

		sDBRequest_NoLimitTicket_Warp clMsg(true ,pclChar->GetID(), pclChar->pclCI->GetPersonID(), pos, &pclMsg->clItem,datevary);
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}
*/
	return ;
}

// 이름을 변경한다.
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGENAME(sPacketHeader* pPacket)
{
	sDBResponse_ChangeName* pclinfo = (sDBResponse_ChangeName*)pPacket;

	if(pclinfo->m_bUseItem == false)
	{
		// [종호_NHN->NDOORS] 로그인시 캐릭터명을 변경하는 경우
		//.....

		SI32 siCharID = pclinfo->siCharID;

		if ( pclCM->IsValidID(siCharID) == false )
			return ;

		if ( pclinfo->siResult == 1)
			pclCM->CR[siCharID]->pclCI->clBI.clPerson.SetName(pclinfo->szName);

		sDBRequest_PersonListInfoGet clMsg(siCharID, pclCM->CR[siCharID]->pclCI->clBI.siIDNum);
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);

		cltGameMsgResponse_ChangeCharName clinfo2(pclinfo->siResult,pclinfo->szName, pclinfo->m_bUseItem);
		cltMsg clMsg2(GAMEMSG_RESPONSE_CHANGE_CHARNAME, sizeof(clinfo2), (BYTE*)&clinfo2);
		pclCM->CR[siCharID]->SendNetMsg((sPacketHeader*)&clMsg2);

		// [종호_NHN->NDOORS] 이관로그
		SendLog_WriteLogToDB(	LOGCOMMAND_TYPE_CHARACTOR, 
								LOGCOMMAND_INDEX_CHARACTER_CHANGE_NAME, 
								CHANGENAME_REASON_SPECIALCHAR_CHANGENAME,
								(cltCharServer*)pclCM->CR[siCharID], 
								NULL, 
								0, 
								0, 
								0, 
								NULL, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								pclinfo->szName, 
								pclinfo->szOldName);

	}
	else
	{
		// 아이템을 사용해서 캐릭터명을 변경하는 경우
		//.....	

		SI32 id = pclCM->GetIDFromPersonID(pclinfo->siPersonID);
		if ( pclCM->IsValidID(id) == false )
			return ;

		if ( pclinfo->siResult == 1)
		{
			pclCM->CR[id]->pclCI->clBI.clPerson.SetName(pclinfo->szName);

			// 통계에 추가한다. 
			SI32 itemunique = ITEMUNIQUE(24114) ; // 이름변경이용권
			pclItemManager->AddUseNum( itemunique, 1);

			// 새로운 통계
			//NStatistics_Add("Item", itemunique, "UseNum", 1);
		}

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclinfo->clitem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		cltGameMsgResponse_ChangeCharName clinfo2(pclinfo->siResult,pclinfo->szName, pclinfo->m_bUseItem);
		cltMsg clMsg2(GAMEMSG_RESPONSE_CHANGE_CHARNAME, sizeof(clinfo2), (BYTE*)&clinfo2);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// PCK - 아이템사용을 할땐 아이템 사용 로그도 같이 남겨준다.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
				USEITEM_REASON_CHANGENAME, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclinfo->clitem_Log, 0, 
				0, 0, 0, 0, 0, 
				pclinfo->siPos, 0, 0, 0, 0, NULL, NULL);

			// [로그추가 : 황진성 2007. 10. 9] // 이름 변경 사용내역.(아이템도 빼가나?)
			// param2== 아이템 유니크, pszParam1 == 이름
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_CHANGE_NAME, 
				0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				0, pclinfo->clitem_Log.siUnique, 0, 0, 0, pclinfo->szName, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_CHANGE_CHARKIND(sPacketHeader* pPacket)
{
	sDBResponse_Change_CharKind* pclinfo = (sDBResponse_Change_CharKind*)pPacket;

	if( pclinfo->siKind < 0 || pclinfo->siKind >= MAX_KIND_NUMBER )			return;

	SI32 id = pclCM->GetIDFromPersonID(pclinfo->siPersonID);
	if ( pclCM->IsValidID(id) == false )
		return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;

	if ( pclinfo->siKind <= 0 || pclinfo->siKind >= KIND_SYSTEM ) return ;
	if ( pclKindInfoSet->pclKI[pclinfo->siKind] == NULL ) return ;

	if ( pclinfo->siResult == 1)
	{
		if ( pclchar->pclCI->clBI.GetKind() == pclinfo->siKind ) return ;

		pclchar->pclCI->clBI.SetKind(pclinfo->siKind);

		// 통계에 추가한다. 
		SI32 itemunique = ITEMUNIQUE(24070) ; // 캐릭터변경이용권
		pclItemManager->AddUseNum( itemunique, 1);

		//새로운 통계
		//NStatistics_Add("Item", itemunique, "UseNum", 1);
	}
	else return ;

	// 캐릭터를 다시 로딩하기 위해 경복궁으로 보낸다.
	OrderStop(id, BY_COMPUTER);

	cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);
	pclchar->SetWarpInfo(&clWarpInfo, 1900);

	// 클라이언트로 업데이트 된 정보를 보낸다. 
	cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclinfo->clitem, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	cltGameMsgResponse_Change_CharKind clinfo2(pclinfo->siResult,pclinfo->siKind);
	cltMsg clMsg2(GAMEMSG_RESPONSE_CHANGE_CHARKIND, sizeof(clinfo2), (BYTE*)&clinfo2);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg2);

	if ( pclinfo->siResult == 1)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			cltServer * pclserver = (cltServer*)pclClient;

			// PCK - 아이템사용을 할땐 아이템 사용 로그도 같이 남겨준다.
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
											USEITEM_REASON_CHANGECHARKIND, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, 
											&pclinfo->clitem_Log, 0, 0, 0, 0, 0, 0, pclinfo->siPos, 0, 0, 0, 0, NULL, NULL);

			// [로그추가 : 황진성 2007. 10. 5] // 캐릭터 변경 사용내역.
			// param1 == 결과
			// param2 == 아이템 유니크
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_CHANGE_KIND, 
											0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclinfo->siKind, pclinfo->clitem_Log.siUnique, 0, 0, 0, NULL, NULL);	
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWSCHATADDMISSION(sPacketHeader* pPacket)
{
	sDBResponse_NewsChatAddmission* pclMsg = (sDBResponse_NewsChatAddmission*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( id != pclMsg->usCharID ) return ;
	if ( pclMsg->siResult != 1 || pclMsg->siPersonID <= 0 )
		return ;
	if ( pclCM->IsValidID(id) == FALSE ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if ( pclchar == NULL )
		return;

	if( pclMsg->siInvPos >= 0 && pclMsg->siInvPos < MAX_ITEM_PER_PERSON )
	{
		// 아이템 정보를 업데이트한다.
		pclchar->pclCI->clCharItem.clItem[ pclMsg->siInvPos].Set(&pclMsg->clItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siInvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}

	TCHAR * pName = (TCHAR*)pclchar->GetName() ;

	SI32 captionKind = CAPTIONKIND_NEWSCHAT;
	SI32 sendKind    = CAPTIONKIND_NEWSCHAT;
    if( pclMsg->bUseColorNews )
	{
		captionKind = CAPTIONKIND_COLORNEWSCHAT;
	}

	PushSpecialNews( pclMsg->szChatText,pName, NULL, captionKind,sendKind);

	if ( IsWhereServiceArea(ConstSwitch_NewLog) ) 
	{
		// szparam1 == 자막광고 내용1(49byte)
		// szparam2 == 자막광고 내용2(49byte)
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_USESUBTITLE, 
									0, pclchar, NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
									0, 0, 0, 0, 0, pclMsg->szChatText, &pclMsg->szChatText[49]);
	}
	

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_NOLIMITTICKET_WARP(sPacketHeader* pPacket)
{
	sDBResponse_NoLimitTicket_Warp* pclMsg = (sDBResponse_NoLimitTicket_Warp*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( id != pclMsg->usCharID ) return ;
	if ( pclMsg->siResult != 1 || pclMsg->siPersonID <= 0 )
		return ;
	if ( pclCM->IsValidID(id) == FALSE ) return ;

	cltCharServer* pclchar = pclCM->GetCharServer( id );

	// 아이템 사용했을 때만 아이템 정보 업데이트
	if ( pclMsg->bUsing )
	{
		if( pclMsg->siInvPos >= 0 && pclMsg->siInvPos < MAX_ITEM_PER_PERSON )
		{
			cltItem* pclItem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siInvPos];

			if(NULL == pclItem)
				return;

			if(IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				cltItem clUsedItem;
				clUsedItem.Set(pclItem);
				clUsedItem.SetItemNum(1);

				cltDate clEndDate;
				clEndDate.MakeFullDateFromDateVary(pclMsg->siDateVary);

				TCHAR szEndDate[32] = {0,};	
				StringCchPrintf(szEndDate, 32, GetTxtFromMgr(1644), clEndDate.uiYear, clEndDate.uiMonth, clEndDate.uiDay );

				//param1 == 인벤 위치  param2 == 아이템 타입 charparam1 == 끝나는 시간
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PREMIUMITEM_USE, 
					0, pclchar, NULL, 0, 0, 0, &clUsedItem, pclMsg->clItem.GetItemNum(), 0, 0, 0, 0, 0, 
					pclMsg->siInvPos, pclMsg->siType, 0, 0, 0, szEndDate, NULL);
			}

			// 통계에 추가한다. 
			pclItemManager->AddUseNum( pclItem->siUnique, 1);

			// 아이템 정보를 업데이트한다.
			pclItem->Set(&pclMsg->clItem);

			// 새로운 통계
			//NStatistics_Add("Item", pclMsg->clItem.siUnique, "UseNum", 1);

			if ( SUMMON_SPACE_TICKET == pclMsg->siType) 
			{

				((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMON_SUCCES_USESTRAGERICKET,  0, 0, id);// 소환수 티켓을 사용 했습니다.
			}
			

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siInvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	//무한 워프 
	if( NOLIMIT_TICKET == pclMsg->siType )
	{
		pclCM->CR[id]->pclCI->clBI.bUseNoLimitTicketWarp = pclMsg->bUsing ;
		pclCM->CR[id]->pclCI->clBI.siNoLimitTicketWarpDateVary = pclMsg->siDateVary ;
	}
	else if( MARKET_TICKET == pclMsg->siType )
	{
		pclCM->CR[id]->pclCI->clBI.bMarKetPremiumService = pclMsg->bUsing ;
		pclCM->CR[id]->pclCI->clBI.siMarKetPremiumServiceDate = pclMsg->siDateVary ;
	}
	else if( NewMarket_TICKET == pclMsg->siType )
	{
		pclCM->CR[id]->pclCI->clBI.bNewMarketPremiumService		= pclMsg->bUsing ;
		pclCM->CR[id]->pclCI->clBI.siNewMarketPremiumServiceDate	= pclMsg->siDateVary ;
	}
	else if( SPACEBOX_TICKET == pclMsg->siType )
	{
		pclCM->CR[id]->pclCI->clBI.bSpaceBoxPremiumService = pclMsg->bUsing ;
		pclCM->CR[id]->pclCI->clBI.siSpaceBoxPremiumServiceDate = pclMsg->siDateVary ;
	}
	//소환수 슬롯 확장 
	else if( SUMMON_SPACE_TICKET == pclMsg->siType)
	{
		pclCM->CR[id]->pclCI->clBI.bSummonExtraSlot = pclMsg->bUsing ;
		pclCM->CR[id]->pclCI->clBI.siSummonExtraSlotDate = pclMsg->siDateVary ;
	}

	cltGameMsgResponse_NoLimitTicket_Warp clmsg( pclMsg->siType, pclMsg->bUsing,pclMsg->siDateVary );
	cltMsg clMsg(GAMEMSG_RESPONSE_NOLIMITTICKET_WARP, sizeof(clmsg), (BYTE*)&clmsg);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return ;
}


void cltServer::DoMsg_DBMSG_RESPONSE_GET_SEL_PGLIST(sPacketHeader* pPacket)
{
	sDBResponse_Get_Sel_PGList * pclMsg = (sDBResponse_Get_Sel_PGList*)pPacket;

	if ( pclMsg->siPersonID <= 0 ) return ;	

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( pclCM->IsValidID(id) == false)	return;
	if ( id != pclMsg->usCharID ) return ;
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	if ( pclChar->bIsItemMallGetItem )
		pclChar->bIsItemMallGetItem = false ;

	if ( pclMsg->siResult != 1 ) return ;

	// 실질 데이타
	cltItemMallData cldata[MAX_ITEMMALL_LIST_NUM];

	for ( SI32 i = 0 ; i < MAX_ITEMMALL_LIST_NUM ; i ++ )
	{
		cldata[i].Init();

		for ( SI32 j = 0 ; j < MAX_ITEMMALL_SET_NUM ; j ++ )
		{
			cldata[i].clItem[j].Init();
			cldata[i].clItem[j].siUnique	= pclMsg->clItemData[i].siUnique[j] ;
			cldata[i].clItem[j].siItemNum	= pclMsg->clItemData[i].siItemNum[j] ;
			cldata[i].siLimitLevel[j]		= pclMsg->clItemData[i].siLimitLevel[j];
		}

		cldata[i].siPrice		= pclMsg->clListData[i].siPrice ;
		cldata[i].siEventPrice	= pclMsg->clListData[i].siEventPrice ;

		cldata[i].siProductKey	= pclMsg->clListData[i].siProductKey ;
		cldata[i].siGroupKey	= pclMsg->clListData[i].siGroupKey ;

		cldata[i].siImageFont	= pclMsg->clListData[i].siSprFont ;

		MStrCpy(cldata[i].szName,		pclMsg->clListData[i].szName,		128);
		MStrCpy(cldata[i].szExplain,	pclMsg->clListData[i].szExplain,	512);

		if(pclClient->IsCountrySwitch(Switch_ItemExplain2))
			MStrCpy(cldata[i].szExplain2,	pclMsg->clListData[i].szExplain2,	512);	//	[종호_szExplain2]

		cldata[i].bGift = pclMsg->clListData[i].bGiftFlag ;
		
		// 은전은 중국에서만 쓴다
		if( ConstServiceArea_China == pclClient->siServiceArea )
		{
			cldata[i].bOnlyBuySilverCoin = pclMsg->clListData[i].bOnlyBuySilverCoin;
		}
		else
		{
			cldata[i].bOnlyBuySilverCoin = false;
		}

		cldata[i].bAgeLimit = pclMsg->clListData[i].bAgeLimit;	//[진성] 거래제한. KOR: 나이, ENG: 레벨 => 2008-9-22
	}

	cltGameMsgResponse_Get_Sel_PGList clmsg(pclMsg->siResult,
											pclMsg->siMainCategory,
											pclMsg->siSubCategory,
											pclMsg->szType,
											pclMsg->siPage, 
											pclMsg->siMaxPage, 
											cldata);
	cltMsg clMsg(GAMEMSG_RESPONSE_GET_SEL_PGLIST, sizeof(clmsg), (BYTE*)&clmsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_ITEMMALL_ITEMINFO(sPacketHeader* pPacket)
{
	sDBResponse_Get_ItemMall_ItemInfo* pclMsg = (sDBResponse_Get_ItemMall_ItemInfo*)pPacket;

	if ( pclMsg->siPersonID <= 0 ) return ;	

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( pclCM->IsValidID(id) == false)	return;
	if ( id != pclMsg->usCharID ) return ;
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	if ( pclChar->bIsItemMallGetItem )
		pclChar->bIsItemMallGetItem = false ;

	if ( pclMsg->siResult != 1 ) return ;

	cltItemMallData		clItemData[3];

	for ( SI32 i = 0 ; i < 3 ; i ++ )
	{
		clItemData[i].siPrice = pclMsg->clItemData[i].siPrice ;
		clItemData[i].siProductKey = pclMsg->clItemData[i].siProductKey ;
		MStrCpy(clItemData[i].szName,pclMsg->clItemData[i].szName,128);
	}

	cltGameMsgResponse_Get_ItemMall_ItemInfo clmsg(pclMsg->siResult,clItemData);
	cltMsg clMsg(GAMEMSG_RESPONSE_GET_ITEMMALL_ITEMINFO, sizeof(clmsg), (BYTE*)&clmsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALL_BUYITEM(sPacketHeader* pPacket)
{
	sDBResponse_ItemMall_Buyitem* pclMsg = (sDBResponse_ItemMall_Buyitem*)pPacket;

	if ( pclMsg->siPersonID <= 0 ) return ;	

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( pclCM->IsValidID(id) == false) return;
	if ( id != pclMsg->usCharID ) return ;
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	cltServer *pclServer = (cltServer *)pclClient;

	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	if ( pclChar->bIsItemMallGetItem )
		pclChar->bIsItemMallGetItem = false ;

	//	if ( pclMsg->siResult != 1 ) return ;

	cltItemMallData cldata;
	cldata.Init();

	cldata.siProductKey = pclMsg->siProductKey ;
	cldata.siPrice = pclMsg->siProductPrice ;
	strcpy(cldata.szName, pclMsg->szItemName);

	// 추천인이 있는지 확인한다. 
	SI32 recommenderpersonid = pclChar->pclCI->clBI.clRecommenderInfo.siPersonID;
	if( recommenderpersonid  > 0 ) 
	{
		// 서버에 설정된 값이 있는지 확인
		if( siItemmallRPRate )
		{
#ifdef USE_PROMOTOR_SERVER
			// 중국은 프로모터가 있고 핸드폰 인증이 되야만 아이템과 포인트를 준다.	
			if( /*true == pclChar->bCellularPhoneAuth &&*/ true == pclChar->bIsPromoter )
#endif
			{
				SI32 recommendPoint = pclMsg->siProductPrice * pclMsg->siProductNum / siItemmallRPRate;
				sDBRequest_ChangeRecommendPoint clMsg( pclChar->GetID() , pclChar->pclCI->GetPersonID() , (TCHAR*)pclChar->GetName() , pclChar->pclCI->clIP.GetLevel()  , recommenderpersonid,  recommendPoint, RPGIVE_REASON_ITEMMALL );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
	}

	cltGameMsgResponse_ItemMall_Buyitem clmsg(pclMsg->siResult,&cldata,pclMsg->siProductNum,pclMsg->siCash, pclMsg->siEventCash, pclMsg->szTxNoCode);
	cltMsg clMsg(GAMEMSG_RESPONSE_ITEMMALL_BUYITEM, sizeof(clmsg), (BYTE*)&clmsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 아이템 몰 구입.
		// param1 == 상품의 갯수, param2 == 상품 인덱스, param3 == 이벤트 캐쉬
		// pszParam1 == 상품의 이름.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ITEMMALL, LOGCOMMAND_INDEX_ITEMMALL_BUY, 
			0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, pclMsg->siProductPrice, pclMsg->siCash, 0, 0, 0, 0, 0, 0, 0, 
			pclMsg->siProductNum, pclMsg->siProductKey, pclMsg->siEventCash, 0, 0, 
			pclMsg->szItemName, NULL);
	}

	SI32 personid = pclMsg->siPersonID;
	if(personid <= 0 )
	{
		SendServerResponseMsg(0,SRVAL_ITEMMALL_FAIL_GETITEM_INFOERROR,2,0,id);
		return ;
	}

	if (pclClient->siServiceArea == ConstServiceArea_English)
	{
		SDBRequest_Get_MyCash_Info_Global clMsg2(pclChar->GetCharUnique(), personid, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )
	{
		SDBRequest_Get_MyCash_Info_China clMsg2(pclChar->GetCharUnique(), personid, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		// Do Nothing if Korea

		// EVENT
		/*
		if( pclEventTimeManager->InEventTime("ItemMall_GiveBronzeKey", &sTime) )
		{
			if( pclMsg->siProductPrice >= 800 )		// 단가가 800원 이상인 것만 지급한다.
			{
				sDBRequest_ItemMall_Buyitem_ForXNoti clMsg( -1, -1, pclMsg->szMyAccountID, (TCHAR*)pclChar->GetName(), 1362211099, pclMsg->siProductNum, 0, "", pclClient->siServiceArea);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
		*/
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )
	{
		// Do Noting if USA
	}
	else if (pclClient->siServiceArea == ConstServiceArea_NHNChina)
	{
		SDBRequest_Get_MyCash_Info_NHNChina clMsg2(pclChar->GetCharUnique(), personid, pclChar->pclCI->clBI.szAccountID, pclServer->siServerID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}
	else if (pclClient->siServiceArea == ConstServiceArea_EUROPE)
	{
		SDBRequest_Get_MyCash_Info_Germany clMsg2(pclChar->GetCharUnique(), personid, pclChar->pclCI->clBI.siIDNum, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);

		// Do Nothing if Europe
	}
	else
	{
		SDBRequest_Get_MyCash_Info clMsg2(pclChar->GetCharUnique(), personid, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}

	// 아이템몰에서 구매시 보너스 상품 지급
	if ( pclEventTimeManager->InEventTime("ItemMall_GiveItem", &sTime) )
	{
		GMONEY	gmCondMoney		= GetGlobalValue( _T("GV_ItemMall_GiveItem_Money") );
		GMONEY	gmSellMoney		= pclMsg->siProductPrice * pclMsg->siProductNum;

		SI32	siGiveUinque	= GetGlobalValue( _T("GV_ItemMall_GiveItem_ItemUnique") );
		SI32	siGiveNum		= GetGlobalValue( _T("GV_ItemMall_GiveItem_ItemNum") );

		cltItem	clGiveItem;
		SI16	siRareSwitch	= 0;

		if ( (0 < gmCondMoney) && (0 <siGiveUinque) && (0 < siGiveNum) && (gmCondMoney <= gmSellMoney) )
		{
			if ( true == pclItemManager->MakeRandItemUnique(siGiveUinque, &clGiveItem, 0, 0, &siRareSwitch) )
			{
				cltSystemRewardInfo clInfo;	// 아이템 보상에 따른 정보

				// 받은 날짜
				clInfo.m_sGiveTime = sTime;

				// 만료 날짜 - 지급받은 날부터 90일 동안 수령할 수 있다.
				TimeAddDate(&clInfo.m_sGiveTime, 90, &clInfo.m_sExpireTime);

				// 지급할 아이템
				clInfo.m_clItem.Set( &clGiveItem );

				// 아이템을 지급 하라는 요청
				sDBRequest_GiveSystemReward clMsgReward( GIVEREWARD_CHAR, pclMsg->siPersonID, pclMsg->usCharID, &clInfo );
				SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsgReward );
			}
		}
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALL_GIFTITEM(sPacketHeader* pPacket)
{
	
	sDBResponse_ItemMall_Giftitem* pclMsg = (sDBResponse_ItemMall_Giftitem*)pPacket;

	if ( pclMsg->siPersonID <= 0 )		return ;	

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 )						return ;
	if ( pclCM->IsValidID(id) == false) return;
	if ( id != pclMsg->usCharID )		return ;
	
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	if ( pclChar->bIsItemMallGetItem )
		pclChar->bIsItemMallGetItem = false ;

	//	if ( pclMsg->siResult != 1 ) return ;

	cltItemMallData cldata;
	cldata.Init();

	cldata.siProductKey = pclMsg->siProductKey ;
	cldata.siPrice		= pclMsg->siProductPrice ;

	cltGameMsgResponse_ItemMall_Giftitem clmsg(	pclMsg->siResult,	
												pclMsg->szGetCharName,
												pclMsg->siProductKey,	
												pclMsg->siProductNum,
												pclMsg->siProductPrice,
												pclMsg->siCash, 
												pclMsg->siEventCash,
												pclMsg->szTxNoCode,
												pclMsg->szItemName,	
												pclMsg->siHanFailReason	);
	cltMsg clMsg(GAMEMSG_RESPONSE_ITEMMALL_GIFTITEM, sizeof(clmsg), (BYTE*)&clmsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // ITEMMALL이용 선물하기
		// param1 == 상품 수량, param2 == 상품 인덱스, param3 == 이벤트 캐쉬
		// pszParam1 == 선물 받을 사람 이름, pszParam2 == 상품의 이름
		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ITEMMALL, LOGCOMMAND_INDEX_ITEMMALL_GIFT,
										0, pclChar, NULL, 0, pclMsg->siProductPrice, pclMsg->siCash, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->siProductNum, pclMsg->siProductKey, pclMsg->siEventCash, 0, 0, 
										pclMsg->szGetCharName, pclMsg->szItemName);
	}
	

	// 선물을 준 유저에게 선물이 도착했음을 알림
	cltLetterMsg_Present clletter(  pclClient->siServerUnique , 
									(TCHAR*)pclChar->GetName(),
									pclMsg->szComment,
									pclMsg->szItemName,
									pclMsg->siProductNum,
									pclMsg->siItemImageFont, 
									pclChar->pclCI->clBI.szAccountID );
	
//	SendAccountLetterMsg( pclMsg->szGetAccountID , (cltLetterHeader*)&clletter);

	// 
	if( siServiceArea == ConstServiceArea_Korea )
	{
		if(pclMsg->siGetItemPersonID)
		{
			SendLetterMsg(pclMsg->siGetItemPersonID, (cltLetterHeader*)&clletter);
		}
	}

	if ( siServiceArea != ConstServiceArea_Korea )
	{
		if( pclMsg->siGetServerUnique == siServerUnique)	// 같은 서버 캐릭터에게 보내는 메시지라면
		{
			// 이름으로 접속중인체 체크하고 바로 볼수있게끔 메시지를 보낸다.		
			SI32 toid = pclCM->GetIDFromName(pclMsg->szGetCharName);

			if(toid == 0)
			{
				if( ConstServiceArea_China == pclClient->siServiceArea )
				{
				}
				else if( ConstServiceArea_Japan == pclClient->siServiceArea )
				{
					SendAccountLetterMsg( pclMsg->szGetAccountID , (cltLetterHeader*)&clletter);	
				}
				// [진성] 선물하기. 접속 안한 유저고 같은 서버이면 선물을 받았다는 메시지를 DB에 저장. => 2008-7-24
				else if( ConstServiceArea_USA == pclClient->siServiceArea )
				{
					SendAccountLetterMsg( pclMsg->szGetAccountID , (cltLetterHeader*)&clletter);	
				}
				else
				{
					if(pclMsg->siGetItemPersonID)
					{
						SendLetterMsg(pclMsg->siGetItemPersonID, (cltLetterHeader*)&clletter);
					}
				}
			}
			else
			{
				cltAccountLetterUnit clLetterUnit((cltLetterHeader*)&clletter);
				if( pclCM->IsValidID(toid) )
					((cltCharServer*)pclCM->CR[toid])->SendAccountLetterMsg(&clLetterUnit);			
			}
		}
		else
		{
			SendAccountLetterMsg( pclMsg->szGetAccountID , (cltLetterHeader*)&clletter);		 	    
		}	
	}
	
	if ( pclClient->siServiceArea == ConstServiceArea_English )
	{
		SDBRequest_Get_MyCash_Info_Global clMsg2(pclChar->GetCharUnique(), pclMsg->siPersonID, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}
	else if (pclClient->siServiceArea == ConstServiceArea_China )
	{
		SDBRequest_Get_MyCash_Info_China clMsg2(pclChar->GetCharUnique(), pclMsg->siPersonID, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}
	else if (pclClient->siServiceArea == ConstServiceArea_USA )
	{
		sDBRequest_Get_MyCash_Info_USA clMsg(pclChar->GetCharUnique(), pclMsg->siPersonID, pclChar->pclCI->clBI.szAccountID, pclChar->uiIDNo);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		// Do Nothing if Korea

		// EVENT
		/*
		if( pclEventTimeManager->InEventTime("ItemMall_GiveBronzeKey", &sTime) )
		{
			if( pclMsg->siProductPrice >= 800 )		// 단가가 800원 이상인 것만 지급한다.
			{
				sDBRequest_ItemMall_Buyitem_ForXNoti clMsg( -1, -1, pclMsg->szMyAccountID, (TCHAR*)pclChar->GetName(), 1362211099, pclMsg->siProductNum, 0, "", pclClient->siServiceArea);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
		*/
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_NHNChina )
	{
		/*SDBRequest_Get_MyCash_Info_Global clMsg2(pclChar->GetCharUnique(), pclMsg->siPersonID, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);*/
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		SDBRequest_Get_MyCash_Info_Germany clMsg2(pclChar->GetCharUnique(), pclMsg->siPersonID, pclChar->pclCI->clBI.siIDNum, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}
	else
	{
		SDBRequest_Get_MyCash_Info clMsg2(pclChar->GetCharUnique(), pclMsg->siPersonID, pclChar->pclCI->clBI.szAccountID);
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
	}

	// 아이템몰에서 구매시 보너스 상품 지급
	if ( pclEventTimeManager->InEventTime("ItemMall_GiveItem", &sTime) )
	{
		GMONEY	gmCondMoney		= GetGlobalValue( _T("GV_ItemMall_GiveItem_Money") );
		GMONEY	gmSellMoney		= pclMsg->siProductPrice * pclMsg->siProductNum;

		SI32	siGiveUinque	= GetGlobalValue( _T("GV_ItemMall_GiveItem_ItemUnique") );
		SI32	siGiveNum		= GetGlobalValue( _T("GV_ItemMall_GiveItem_ItemNum") );

		cltItem	clGiveItem;
		SI16	siRareSwitch	= 0;

		if ( (0 < gmCondMoney) && (0 <siGiveUinque) && (0 < siGiveNum) && (gmCondMoney <= gmSellMoney) )
		{
			if ( true == pclItemManager->MakeRandItemUnique(siGiveUinque, &clGiveItem, 0, 0, &siRareSwitch) )
			{
				cltSystemRewardInfo clInfo;	// 아이템 보상에 따른 정보

				// 받은 날짜
				clInfo.m_sGiveTime = sTime;

				// 만료 날짜 - 지급받은 날부터 90일 동안 수령할 수 있다.
				TimeAddDate(&clInfo.m_sGiveTime, 90, &clInfo.m_sExpireTime);

				// 지급할 아이템
				clInfo.m_clItem.Set( &clGiveItem );

				// 아이템을 지급 하라는 요청
				sDBRequest_GiveSystemReward clMsgReward( GIVEREWARD_CHAR, pclMsg->siPersonID, pclMsg->usCharID, &clInfo );
				SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsgReward );
			}
		}
	}

	
}

void cltServer::DoMsg_DBMSG_RESPONSE_CHECK_ACCOUNTID(sPacketHeader* pPacket)
{
	

	sDBResponse_Check_AccountID* pclMsg = (sDBResponse_Check_AccountID*)pPacket;

	if ( pclMsg->siPersonID <= 0 )
	{
		return ;	
	}
	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( pclCM->IsValidID(id) == false) return;
	if ( id != pclMsg->usCharID ) return ;
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	if ( pclChar->bIsItemMallGetItem )
		pclChar->bIsItemMallGetItem = false ;

	//if ( pclMsg->szAccountID[0] == '\0' ) return ;
	//if ( pclMsg->szAccountName[0] == '\0' ) return ;
	if ( pclMsg->szCharName[0] == '\0' ) return ;
 

	cltGameMsgResponse_Check_AccountID clmsg(pclMsg->siResult, pclMsg->siServerUnique ,pclMsg->szCharName ,pclMsg->szAccountID, pclMsg->szAccountName);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHECK_ACCOUNTID, sizeof(clmsg), (BYTE*)&clmsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_MYCASH_INFO(sPacketHeader* pPacket)
{
	SDBResponse_Get_MyCash_Info* pclMsg = (SDBResponse_Get_MyCash_Info*)pPacket;

	if(pclMsg->siResult != 1)
		return;


	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)
		return ;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	cltGameMsgResponse_Get_MyCash_Info clmsg(pclMsg->siResult, pclMsg->siCash, pclMsg->siEventCash);
	cltMsg clMsg(GAMEMSG_RESPONSE_GET_MYCASH_INFO, sizeof(clmsg), (BYTE*)&clmsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_SET_BAGITEM(sPacketHeader* pPacket)
{

	sDBResponse_Set_Bagitem* pclMsg = (sDBResponse_Set_Bagitem*)pPacket;

	if ( pclMsg->siPersonID <= 0 ) return ;	

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( pclCM->IsValidID(id) == false) return;
	if ( id != pclMsg->siCharID ) return ;
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	if ( pclChar->bUseBagItem )
		pclChar->bUseBagItem = false ;

	if ( pclMsg->siResult != 1 ) return ;


	if( pclMsg->siLeftPos >= 0 && pclMsg->siLeftPos < MAX_ITEM_PER_PERSON )
	{
		pclChar->pclCI->clCharItem.clItem[pclMsg->siLeftPos].Set(&pclMsg->clLeftItem);

		// 클라이언트로 통보
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siLeftPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clLeftItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclChar->SendNetMsg((sPacketHeader*)&clMsg);
	}

	for ( SI32 i = 0 ; i < MAX_NUM_PER_BAGITEM ; i ++ )
	{
		if ( pclMsg->siPos[i] != 0 && pclMsg->clItem[i].siUnique != 0 && pclMsg->clItem[i].siItemNum != 0 )
		{
			if( pclMsg->siPos[i] >= 0 && pclMsg->siPos[i] < MAX_ITEM_PER_PERSON )
			{
				pclChar->pclCI->clCharItem.clItem[pclMsg->siPos[i]].Set(&pclMsg->clItem[i]);

				// 클라이언트로 통보
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siPos[i], PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem[i], 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclChar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}	
}

void cltServer::DoMsg_DBMSG_RESPONSE_SET_CHANGEHERO(sPacketHeader* pPacket)
{/*
	sDBResponse_Set_ChangeHero* pclMsg = (sDBResponse_Set_ChangeHero*)pPacket;

	if ( pclMsg->siPersonID <= 0 ) return ;	

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( id != pclMsg->siCharID ) return ;
	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID() )
		return ;

	pclChar->bUseChangeHeroItem = false ;

	if ( pclMsg->siResult != 1 ) return ;

	pclChar->pclCI->clCharItem.clItem[pclMsg->siPos].Set(&pclMsg->clItem) ;

	pclChar->pclCI->clBI.siChangeHeroCode = pclMsg->siCode ;

	cltGameMsgResponse_Set_ChangeHero clinfo(pclMsg->siPos,&pclMsg->clItem,pclMsg->siCode);
	cltMsg clMsg(GAMEMSG_RESPONSE_SET_CHANGEHERO, sizeof(clinfo), (char*)&clinfo);
	pclChar->SendNetMsg((char*)&clMsg);*/
} 


void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALL_BUYITEM_FORXNOTI(sPacketHeader* pPacket)
{
	sDBResponse_ItemMall_Buyitem_ForXNoti* pclMsg = (sDBResponse_ItemMall_Buyitem_ForXNoti*)pPacket;
	pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [siArrayIndex:%d]"), TEXT("sDBResponse_ItemMall_Buyitem_ForXNoti"), pclMsg->siArrayIndex );

	// FOR EVENT
	if( pclMsg->siArrayIndex < 0 && pclMsg->siIdx < 0 )		return;

#ifdef USE_HANXNOTI

	if(pclMsg->siArrayIndex < 0 || pclMsg->siArrayIndex >= HANXNOTI_MAX_WAITING)		return;

	AddInventoryItemAns*	pkSendMsg = NULL;
	if( m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_siIdx == pclMsg->siIdx )
	{
		// 정상적인 메시지 처리
		m_pkHanXNoti_AnsMessages[pclMsg->siArrayIndex] = new AddInventoryItemAns;
		pkSendMsg = (AddInventoryItemAns*)m_pkHanXNoti_AnsMessages[pclMsg->siArrayIndex];
	}
	else
	{
		// 메시지가 늦게 도착했으므로 DB에 취소를 요청
		sDBRequest_ItemMall_CancelOrder_XNoti clMsg( pclMsg->szTxCode );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		
		pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [Cancel Order:Time Out] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
		pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog_Error.log"), TEXT("Msg:[%s] [Cancel Order:Time Out] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
	}

	if(pkSendMsg != NULL)
	{
		pkSendMsg->nIdx = pclMsg->siIdx;

		if(pclMsg->siResult == 1)
		{
			pkSendMsg->result = MRS_ERRORCODE_SUCCESS;
		}
		else
		{
			pkSendMsg->result = MRS_ERRORCODE_ADDINVENTORYITEMREQ_FAILED;
		}

		pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [result:%d]"), TEXT("AddInventoryItemAns"), pkSendMsg->result );
	}
	else
	{
		// 메모리 생성 오류
		if( m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_siIdx == pclMsg->siIdx )
		{
			// 오류처리 이므로 DB에 취소를 요청
			sDBRequest_ItemMall_CancelOrder_XNoti clMsg( pclMsg->szTxCode );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [Cancel Order:Mem Error] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
			pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog_Error.log"), TEXT("Msg:[%s] [Cancel Order:Mem Error] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
		}
	}

	// Idx 가 일치할 경우에만 
	if( m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_siIdx == pclMsg->siIdx )
	{
		// 이벤트 SET !!
		::SetEvent(m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_hWaitingHandle);
	}

#endif
}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMALL_GIFTITEM_FORXNOTI(sPacketHeader* pPacket)
{
	sDBResponse_ItemMall_Giftitem_ForXNoti* pclMsg = (sDBResponse_ItemMall_Giftitem_ForXNoti*)pPacket;
	pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [siArrayIndex:%d]"), TEXT("sDBResponse_ItemMall_Giftitem_ForXNoti"), pclMsg->siArrayIndex );

#ifdef USE_HANXNOTI

	if(pclMsg->siArrayIndex < 0 || pclMsg->siArrayIndex >= HANXNOTI_MAX_WAITING)		return;

	AddInventoryItemAns*	pkSendMsg = NULL;
	if( m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_siIdx == pclMsg->siIdx )
	{
		m_pkHanXNoti_AnsMessages[pclMsg->siArrayIndex] = new AddInventoryItemAns;
		pkSendMsg = (AddInventoryItemAns*)m_pkHanXNoti_AnsMessages[pclMsg->siArrayIndex];
	}
	else
	{
		if(pclMsg->siResult == 1)
		{
			// 메시지가 늦게 도착했으므로 DB에 취소를 요청
			sDBRequest_ItemMall_CancelOrder_XNoti clMsg( pclMsg->szTxCode );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [Cancel Order:Time Out] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
			pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog_Error.log"), TEXT("Msg:[%s] [Cancel Order:Time Out] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
		}
	}

	if(pkSendMsg != NULL)
	{
		pkSendMsg->nIdx = pclMsg->siIdx;

		if(pclMsg->siResult == 1)
		{
			pkSendMsg->result = MRS_ERRORCODE_SUCCESS;
		}
		else
		{
			pkSendMsg->result = MRS_ERRORCODE_ADDINVENTORYITEMREQ_FAILED;
		}

		pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [result:%d]"), TEXT("AddInventoryItemAns"), pkSendMsg->result );
	}
	else
	{
		// 메모리 생성 오류
		if( m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_siIdx == pclMsg->siIdx )
		{
			if(pclMsg->siResult == 1)
			{
				// 오류처리 이므로 DB에 취소를 요청
				sDBRequest_ItemMall_CancelOrder_XNoti clMsg( pclMsg->szTxCode );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

				pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [Cancel Order:No Mem] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
				pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog_Error.log"), TEXT("Msg:[%s] [Cancel Order:Time Out] [txcode:%s]"), TEXT("AddInventoryItemAns"), pclMsg->szTxCode );
			}
		}
	}

	// Idx 가 일치할 경우에만 
	if( m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_siIdx == pclMsg->siIdx )
	{
		// 이벤트 SET !!
		::SetEvent(m_kHanXNoti_Waitings[pclMsg->siArrayIndex].m_hWaitingHandle);

		//[진성] 선물하기. 웹에서 선물받은 아이템을 선물 받는 넘에게 알림. => 2008-7-24
		cltLetterMsg_Present clletter(  0, 
										pclMsg->szSendCharName,
										pclMsg->szComment,
										NULL,
										pclMsg->siProductNum,
										NULL, 
										pclMsg->szMyAccountID );

		SI32 toid = pclClient->pclCM->GetIDFromName( pclMsg->szGetCharName );
		if(toid == 0)
		{
			if( ConstServiceArea_China == pclClient->siServiceArea )
			{
			} 
			else if( ConstServiceArea_Japan == pclClient->siServiceArea )
			{
				SendAccountLetterMsg( pclMsg->szGetAccountID , (cltLetterHeader*)&clletter);	
			}
			// [진성] 선물하기. 접속 안한 유저고 같은 서버이면 선물을 받았다는 메시지를 DB에 저장. => 2008-7-24
			else if( ConstServiceArea_USA == pclClient->siServiceArea )
			{
				//SendAccountLetterMsg( pclMsg->szGetAccountID , (cltLetterHeader*)&clletter);	
			}
			else
			{
				/*if(pclMsg->siGetItemPersonID)
				{
					SendLetterMsg(pclMsg->siGetItemPersonID, (cltLetterHeader*)&clletter);
				}*/
			}
		}
		else
		{
			cltAccountLetterUnit clLetterUnit((cltLetterHeader*)&clletter);
			if( pclCM->IsValidID(toid) )
				((cltCharServer*)pclCM->CR[toid])->SendAccountLetterMsg(&clLetterUnit);			
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
// [로그추가 : 황진성 2007. 10. 16] // 웹에서 ITEMMALL이용 선물하기
			// param1	 == 상품 수량, 
			// param2	 == 상품 인덱스, 
			// param3	 == 이벤트 캐쉬
			// pszParam1 == 선물 보내는 사람 이름, 
			// pszParam2 == 선물 받을 사람 이름, 
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ITEMMALL, LOGCOMMAND_INDEX_ITEMMALL_WEB_GIFT,
											0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclMsg->siProductNum,  pclMsg->siProductKey, pclMsg->siProductPrice, 0, 0, 
											pclMsg->szSendCharName, pclMsg->szGetCharName);
		}
	}

#endif
}

// 목표 지역으로 이동 한다
void cltServer::DoMsg_DBMSG_RESPONSE_WARP_TARGETWORLD(sPacketHeader* pPacket)
{
	sDBResponse_TargetWorld* pclMsg = (sDBResponse_TargetWorld*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( id <= 0 ) return ;
	if ( id != pclMsg->usCharID ) return ;
	if ( pclMsg->siResult != 1 || pclMsg->siPersonID <= 0 )
		return ;
	if ( pclCM->IsValidID(id) == FALSE ) return ;

	cltCharServer* pclchar = pclCM->GetCharServer( id );

	// 아이템 사용했을 때만 아이템 정보 업데이트
	if ( pclMsg->siResult == 1 )
	{
		if( pclMsg->siInvPos >= 0 && pclMsg->siInvPos < MAX_ITEM_PER_PERSON )
		{
			// 아이템 정보를 업데이트한다.
			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siInvPos].Set(&pclMsg->clItem);

			// 통계에 추가한다. 
			pclItemManager->AddUseNum( pclMsg->clItem.siUnique, 1);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siInvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		// 아이템을 업데이트 한다음에 워프 시키고 끗

		cltWarpInfo	clWarpInfo(1, MAPINDEX_FESTIVAL, true, false, 133 ,168 , -1,  -1)	;
		switch(pclMsg->clItem.siUnique) 
		{
		case ITEMUNIQUE(24128):	// 임시로 탤이용권으로 
			{
				clWarpInfo.uiWarpSwitch = 1 ; 
				clWarpInfo.uiMapIndex = MAPINDEX_FESTIVAL	;
				clWarpInfo.uiForceSwitch = true	;
				clWarpInfo.uiTicketUseSwitch = false	;
				clWarpInfo.uiX = 133	;
				clWarpInfo.uiY = 138	;
				clWarpInfo.siGateX = -1	;
				clWarpInfo.siGateY = -1	;				
				
				pclchar->SetWarpInfo(&clWarpInfo, 3000);
				pclchar->Warp();			
			}
			break;
		}
	}
	else // 아이템 사용 실패시 
	{
	}
	/*cltGameMsgResponse_WarpTargetWorld clmsg( pclMsg->clItem.siUnique );
	cltMsg clMsg(GAMEMSG_RESPONSE_WARP_TARGETWORLD, sizeof(clmsg), (BYTE*)&clmsg);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);*/
	return ;
}

