#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Letter\Letter.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-PostOffice.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

#include "MSg\MsgType-List.h"
#include "Msg\MsgType-PostOffice.h"
#include "MsgRval-Define.h"

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_MASTERSENDITEM(sPacketHeader* pPacket)
{
	sDBResponse_PostOfficeMasterSendItem* pclMsg = (sDBResponse_PostOfficeMasterSendItem *)pPacket;

	cltCharServer*	pclmasterchar = NULL;
	if( pclCM->IsValidID( pclMsg->usCharID ) )
	{
		pclmasterchar = pclCM->GetCharServer( pclMsg->usCharID );
	}
	
	if( pclMsg->siReason == 100 )		// 운영자가 명령어로 아이템을 전송했다.
	{
		switch( pclMsg->siResult )
		{
		case 1:
			{
				// 성공했으므로 받은 사람에게 쪽지 전달 - 보내는 사람은 [GM] 이다.
				TCHAR charname[MAX_PLAYER_NAME];
				StringCchPrintf(charname, MAX_PLAYER_NAME, TEXT("[GM]"));

				// 쪽지 보내기
				cltLetterMsg_Mail	clletter( (TCHAR *)charname );
				SendLetterMsg( pclMsg->siReceiverPersonID, (cltLetterHeader *)&clletter );
			}
			break;
		case 3:		// 캐릭터명 틀림
			{
				SendServerResponseMsg( 0, SRVAL_GAMEMASTER_POSTOFFICE_NOPERSON, 0, 0, pclmasterchar->GetCharUnique());
			}
			break;
		case 4:		// 상대방 메일함이 꽉참
			{
				SendServerResponseMsg( 0, SRVAL_GAMEMASTER_POSTOFFICE_NOTENOUGHBOX, 0, 0, pclmasterchar->GetCharUnique());
			}
			break;
		case 5:		// 알수 없는 에러
			{
				SendServerResponseMsg( 0, SRVAL_GAMEMASTER_POSTOFFICE_SYSTEMERROR, 0, 0, pclmasterchar->GetCharUnique());
			}
			break;
		}
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		SendLog_WriteLogToDB(	LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_SEND_ITEM, 
								pclMsg->siReason, pclmasterchar, NULL, pclMsg->siReceiverPersonID, 
								0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, 
								NULL, NULL);
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_SENDITEM(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	

	sDBResponse_PostOfficeSendItem* pclMsg = (sDBResponse_PostOfficeSendItem *)pPacket;

	if( pclMsg->siItemInventoryIndex < 0 || pclMsg->siItemInventoryIndex > MAX_ITEM_PER_PERSON )	return;

	SI32 id = pclMsg->usCharID;
	cltItem clSendItem;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	switch( pclMsg->siResult ) {
	case 1:
		{
			//----------------------------------------//
			// [추가 : 황진성 2007.9.5]
			// 보내는 아이템의 개수를 담는다. 
			SI32	SendItemCnt = 0;
			//----------------------------------------//


			returnval = SRVAL_POSTOFFICE_SUCCESS_SENDITEM;
					
			// 인벤토리 갱신
			if( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siSenderPersonID )
			{

				if ( pclMsg->siItemNum < 0 || pclMsg->siItemNum > MAX_ITEM_PILE_NUMBER )
				{
					return;
				}

				if ( pclMsg->siItemNum == 0 )
				{
					pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].Init();
				}
				else
				{
					if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siUnique != pclMsg->siItemUnique )
					{
						return;
					}

					//------------------------------------------------------------------------------------------------------------------------//
					// [추가 : 황진성 2007.9.7]
					// 보내는 물품의 개수.
					SendItemCnt = pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siItemNum - pclMsg->siItemNum;
					//------------------------------------------------------------------------------------------------------------------------//

					pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siUnique = pclMsg->siItemUnique;
					pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].siItemNum = pclMsg->siItemNum;

					// 보내는 물건 아이템 정보 생성 - by LEEKH 2009-01-29
					clSendItem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ] );
					clSendItem.SetItemNum( SendItemCnt );
				}

			}

			pclCM->CR[id]->SetUpdateSwitch( UPDATE_INVENTORY, true , 0);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			{
				// [로그추가 : 황진성 2007. 10. 10] // 물품 보내기 로그쓰기.
				// param2 == 보낸 아이템의 인벤 번호, param3 == 보낸 아이템 유니크, param4 == 보낸 아이템의 개수, param5 == 역참 보유자금
				//TCHAR str[50] = {'\0', };
				//StringCchPrintf(str, sizeof(str), "InventoryIndex : %d, Unique : %d, Num : %d",
				//									pclMsg->siItemInventoryIndex,
				//									pclMsg->siItemUnique,
				//									pclMsg->siItemNum	);

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_SEND_ITEM, 
									 0, (cltCharServer*)(pclCM->CR[pclMsg->usCharID]), NULL, pclMsg->siReceiverPersonID, 
									 pclMsg->siFee, pclMsg->clPersonMoney.itMoney, &clSendItem, pclMsg->siItemNum, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									 0, pclMsg->siSendItemInventoryIndex, pclMsg->siSendItemUnique, pclMsg->siSendItemNum, pclMsg->clPostOfficeMoney.itMoney, 
									 NULL, NULL);
			}
			

			// 현금 정보 업데이트 
			pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->clPersonMoney.itMoney );
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

			// 역참 보유자금 업데이트 
			cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_POSTOFFICE );
			if( pStrInfo ) {

				pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clPostOfficeMoney );
			}

			//cyj 쪽찌도 [GM] 붙여서
			TCHAR charname[MAX_PLAYER_NAME];
			if (pclMsg->siPostType == 1)
			{
				TCHAR buf[MAX_PLAYER_NAME + 6];
				StringCchCopy(buf, MAX_PLAYER_NAME + 6, pclCM->CR[id]->GetName());
				StringCchPrintf(charname, MAX_PLAYER_NAME, TEXT("[GM]"));
			}
			else
			{
				StringCchPrintf(charname, MAX_PLAYER_NAME, pclCM->CR[id]->GetName());
			}

			// 쪽지 보내기
			cltLetterMsg_Mail	clletter( (TCHAR *)charname );
			SendLetterMsg( pclMsg->siReceiverPersonID, (cltLetterHeader *)&clletter );

			// 역참으로 물건 보내기 퀘스트중이라면, 
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SENDITEMBYPOST);
		}
		
		break;

	case 2:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE;
		break;

	case 3:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_WRONGNAME;
		break;

	case 4:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_FULLRECVBOX;
		break;
	//KHY - 0614 - 가차 아이템은 같은 계정간 캐릭터에 한하여 우편으로 주고 받을 수 있다.
	case 5:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_ANOTHERPERSON;
		break;		
	}

	if( returnval ) {
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_SENDMONEY(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	
	
	sDBResponse_PostOfficeSendMoney* pclMsg = (sDBResponse_PostOfficeSendMoney *)pPacket;
	
	SI32 id = pclMsg->usCharID;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	switch( pclMsg->siResult ) {
	case 1:
		{

			returnval = SRVAL_POSTOFFICE_SUCCESS_SENDMONEY;

			// 현금 정보 업데이트 
			pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->clPersonMoney.itMoney );
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

			// 역참 보유자금 업데이트 
			cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_POSTOFFICE );
			if( pStrInfo ) {

				pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clPostOfficeMoney );
			}
			
			//cyj 쪽찌도 [GM] 붙여서
			TCHAR charname[MAX_PLAYER_NAME];
			if (pclMsg->siPostType == 1)
			{
				TCHAR buf[MAX_PLAYER_NAME + 6];
				StringCchCopy(buf, MAX_PLAYER_NAME + 6, pclCM->CR[id]->GetName());
				StringCchPrintf(charname, MAX_PLAYER_NAME, TEXT("[GM]"));
			}
			else
			{
				StringCchPrintf(charname, MAX_PLAYER_NAME, pclCM->CR[id]->GetName());
			}

			// 쪽지 보내기
			cltLetterMsg_Mail	clletter( (TCHAR *)charname );
			SendLetterMsg( pclMsg->siReceiverPersonID, (cltLetterHeader *)&clletter );
		}
		break;

	case 2:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHMONEY;
		break;

	case 3:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_WRONGNAME;
		break;

	case 4:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_FULLRECVBOX;
		break;
	}


	if( returnval ) 
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // 송금 하기 로그쓰기.
			//param1 == 결과, param2 == 송신자가 지불한 수수료, Param3 == 역참 보유자금.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_SEND_MONEY, 
								0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siReceiverPersonID, pclMsg->siMoney, pclMsg->clPersonMoney.itMoney, 0, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								returnval, pclMsg->siFee, pclMsg->clPostOfficeMoney.itMoney, 0, 0, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_SENDMSG(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	
	
	sDBResponse_PostOfficeSendMsg* pclMsg = (sDBResponse_PostOfficeSendMsg *)pPacket;
	
	SI32 id = pclMsg->usCharID;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	switch( pclMsg->siResult ) {
	case 1:
		{
			returnval = SRVAL_POSTOFFICE_SUCCESS_SENDMSG;

			// 현금 정보 업데이트 
			pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->clPersonMoney.itMoney );
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

			// 역참 보유자금 업데이트 
			cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_POSTOFFICE );
			if( pStrInfo ) {

				pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clPostOfficeMoney );
			}

			//cyj 쪽찌도 [GM] 붙여서
			TCHAR charname[MAX_PLAYER_NAME];
			if (pclMsg->siPostType == 1)
			{
				TCHAR buf[MAX_PLAYER_NAME + 6];
				StringCchCopy(buf, MAX_PLAYER_NAME + 6, pclCM->CR[id]->GetName());
				StringCchPrintf(charname, MAX_PLAYER_NAME, TEXT("[GM]"));
			}
			else
			{
				StringCchPrintf(charname, MAX_PLAYER_NAME, pclCM->CR[id]->GetName());
			}

			// 쪽지 보내기
			cltLetterMsg_Mail	clletter( (TCHAR *)charname );
			SendLetterMsg( pclMsg->siReceiverPersonID, (cltLetterHeader *)&clletter );
		}
		break;

	case 2:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE;
		break;

	case 3:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_WRONGNAME;
		break;

	case 4:
		returnval = SRVAL_POSTOFFICE_FAIL_SEND_FULLRECVBOX;
		break;
	}


	if( returnval ) 
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 메시지 보내기 로그쓰기.
			// param1 = 결과, param2 = 우체국 보유자금, pszcharparam1 = 보내는 메시지
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_SEND_MSG, 
								0, (cltCharServer*)(pclCM->CR[pclMsg->usCharID]), NULL, pclMsg->siReceiverPersonID, 
								pclMsg->siFee, pclMsg->clPersonMoney.itMoney, 0, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								returnval, pclMsg->clPostOfficeMoney.itMoney, 0, 0, 0, pclMsg->szMsg, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_RECVITEMLIST(sPacketHeader* pPacket)
{	

	sDBResponse_PostOfficeRecvItemList* pclMsg = (sDBResponse_PostOfficeRecvItemList *)pPacket;

	cltGameMsgResponse_PostOfficeRecvItemList sendMsg;

	UI16 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE) return;

	sendMsg.siVillageUnique = pclMsg->siVillageUnique;

	sendMsg.usCount = pclMsg->usCount;
	//역참 아이템 리스트를 보내기 전에 기간 체크 후 삭제
	
	memcpy( &sendMsg.clRecvItemInfo, pclMsg->clRecvItemInfo, sizeof( cltPostOfficeRecvItemInfo ) * pclMsg->usCount );

	if( pclMsg->usCount > MAX_POSTOFFICE_RECV_NUMBER )		pclMsg->usCount = MAX_POSTOFFICE_RECV_NUMBER;
	//cyj clRecvItemInfo의 siPostType이 1이면 GM 이므로 송신자명 앞에 [GM]붙임
	for (int i = 0; i < pclMsg->usCount; i++)
	{
		if (sendMsg.clRecvItemInfo[i].siPostType == 1)
		{
			TCHAR buf[MAX_PLAYER_NAME + 6];
			StringCchCopy(buf, MAX_PLAYER_NAME + 6, sendMsg.clRecvItemInfo[i].szSenderPersonName);
			StringCchPrintf(sendMsg.clRecvItemInfo[i].szSenderPersonName, MAX_PLAYER_NAME, TEXT("[GM]"));
		}
	}

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_POSTOFFICE_RECVITEMLIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST(sPacketHeader* pPacket)
{
	sDBResponse_PostOfficeRecvMoneyList* pclMsg = (sDBResponse_PostOfficeRecvMoneyList *)pPacket;

	cltGameMsgResponse_PostOfficeRecvMoneyList sendMsg;

	UI16 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE) return;

	sendMsg.siVillageUnique = pclMsg->siVillageUnique;

	sendMsg.usCount = pclMsg->usCount;
	memcpy( &sendMsg.clRecvMoneyInfo, pclMsg->clRecvMoneyInfo, sizeof( cltPostOfficeRecvMoneyInfo ) * pclMsg->usCount );

	if( pclMsg->usCount > MAX_POSTOFFICE_RECV_NUMBER )		pclMsg->usCount = MAX_POSTOFFICE_RECV_NUMBER;
	//cyj clRecvMoneyInfo의 siPostType이 1이면 GM 이므로 송신자명 앞에 [GM]붙임
	for (int i = 0; i < pclMsg->usCount; i++)
	{
		if (sendMsg.clRecvMoneyInfo[i].siPostType == 1)
		{
			TCHAR buf[MAX_PLAYER_NAME + 6];
			StringCchCopy(buf, MAX_PLAYER_NAME + 6, sendMsg.clRecvMoneyInfo[i].szSenderPersonName);
			StringCchPrintf(sendMsg.clRecvMoneyInfo[i].szSenderPersonName, MAX_PLAYER_NAME, TEXT("[GM]"));
		}
	}

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_RECVMSGLIST(sPacketHeader* pPacket)
{
	sDBResponse_PostOfficeRecvMsgList* pclMsg = (sDBResponse_PostOfficeRecvMsgList *)pPacket;

	cltGameMsgResponse_PostOfficeRecvMsgList sendMsg;

	UI16 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE) return;

	sendMsg.siVillageUnique = pclMsg->siVillageUnique;

	sendMsg.usCount = pclMsg->usCount;
	memcpy( &sendMsg.clRecvMsgInfo, pclMsg->clRecvMsgInfo, sizeof( cltPostOfficeRecvMsgInfo ) * pclMsg->usCount );

	if( pclMsg->usCount > MAX_POSTOFFICE_RECV_NUMBER )		pclMsg->usCount = MAX_POSTOFFICE_RECV_NUMBER;
	//cyj clRecvMoneyInfo의 siPostType이 1이면 GM 이므로 송신자명 앞에 [GM]붙임
	for (int i = 0; i < pclMsg->usCount; i++)
	{
		if (sendMsg.clRecvMsgInfo[i].siPostType == 1)
		{
			TCHAR buf[MAX_PLAYER_NAME + 6];
			StringCchCopy(buf, MAX_PLAYER_NAME + 6, sendMsg.clRecvMsgInfo[i].szSenderPersonName);
            StringCchPrintf(sendMsg.clRecvMsgInfo[i].szSenderPersonName, MAX_PLAYER_NAME, TEXT("[GM]"));
		}

/*		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // 수정 : PCK - 메시지 수신로그로 변경.
			// param1 = 결과, pszcharparam1 = 받은 메시지
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_GET_MSG, 
				0, (cltCharServer*)(pclCM->CR[pclMsg->usCharID]), NULL, pclMsg->siPersonID, 0, 0, 0, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				pclMsg->siResult, 0, 0, 0, 0, pclMsg->clRecvMsgInfo[i].szMsg, NULL);
		}*/
	}
	cltMsg clNormalMsg( GAMEMSG_RESPONSE_POSTOFFICE_RECVMSGLIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_GETITEM(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	

	sDBResponse_PostOfficeGetItem* pclMsg = (sDBResponse_PostOfficeGetItem *)pPacket;

	SI32 id = pclMsg->usCharID;
	cltItem clRecvItem;

	if(pclCM->IsValidID(id) == FALSE) return;

	if( pclMsg->siResult == 1 ) {

		returnval = SRVAL_POSTOFFICE_SUCCESS_GETITEM;

		// 인벤토리 갱신
		if( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
		{
			// 기간제 아이템인지 체크 후 기간제 아이템이라면 GetItemUseAbleDay 로 사용일 가능수 체크
			if(pclMsg->clItem.IsHaveUseDay())
			{
				SI32 useableDay = 0;
				BOOL rVal = GetItemUseAbleDay(&pclMsg->clItem,&useableDay);
                

				if(useableDay < 0 && rVal == TRUE ) //[영진] 남은 사용 기간 - 일때 처리
				{
					if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
					{
						//KHY - 0723 - 기간아이템 파괴 방지.					
					}
					else
					{
						// 고급품은 지워지지 않는다. - by LEEKH
						if( pclMsg->clItem.clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
						{
							// DB 삭제 요청
							sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),pclMsg->siItemInventoryIndex
								,&pclMsg->clItem,USEITEM_REASON_DATEOVER);

							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
							
							
							// User역참의 받은 아이템 목록 갱신
							LPARAM lParam = MAKELPARAM( pclMsg->siMsgID, LPARAMHIWORD_ITEM );

							cltGameMsgResponse_ListDelItem		sendMsg( LISTID_POSTOFFICE_RECVITEM, lParam );
							cltMsg clMsg(GAMEMSG_RESPONSE_LIST_DELITEM, sizeof(cltGameMsgResponse_ListDelItem), (BYTE*)&sendMsg);

							pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

							SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_GETITEM_USEDATEOVER,  0, 0, pclCM->CR[id]->GetCharUnique());
							// 아이템 사용기간 오버 메시지 보내고 처리 끝냄
							return;
						}
					}
				}
			}

			if( pclMsg->siItemInventoryIndex >= 0 && pclMsg->siItemInventoryIndex < MAX_ITEM_PER_PERSON )
			{
				pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemInventoryIndex ].Set( &pclMsg->clItem );
				pclCM->CR[id]->SetUpdateSwitch( UPDATE_INVENTORY, true, 0 );

				clRecvItem.Set( &pclMsg->clItem );
				clRecvItem.SetItemNum( pclMsg->siRecvItemNum );
			}

			{
				LPARAM lParam = MAKELPARAM( pclMsg->siMsgID, LPARAMHIWORD_ITEM );

				cltGameMsgResponse_ListDelItem		sendMsg( LISTID_POSTOFFICE_RECVITEM, lParam );
				cltMsg clMsg(GAMEMSG_RESPONSE_LIST_DELITEM, sizeof(cltGameMsgResponse_ListDelItem), (BYTE*)&sendMsg);
				
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 우체국에서 아이템 획득. - PCK : 다시 추가함.
			// param1 == 인벤토리 위치
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_GET_ITEM, 
											0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siSenderPersonID, 0, 0, &clRecvItem, pclMsg->clItem.GetItemNum(), 
											0, 0, 0, 0, 0, 
											pclMsg->siItemInventoryIndex, 0, 0, 0, 0, NULL, NULL);
		}
 
	} else if( pclMsg->siResult == 2 ) {
		returnval = SRVAL_POSTOFFICE_FAIL_GETITEM_FULLINVEN;
	}

	if( returnval ) {
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_GETMONEY(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	

	sDBResponse_PostOfficeGetMoney* pclMsg = (sDBResponse_PostOfficeGetMoney *)pPacket;

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE) return;

	if( pclMsg->siResult == 1 ) {

		returnval = SRVAL_POSTOFFICE_SUCCESS_GETMONEY;

		//------------------------------------------------------------------------------------------------------------------------//
		// [추가 : 황진성 2007.9.5]
		// 보내는 돈의 액수.
		SI32	GetMoney = pclMsg->siPersonMoney - pclCM->CR[id]->pclCI->clIP.GetMoney();
		//------------------------------------------------------------------------------------------------------------------------//

		// 소지금 갱신
		if( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
		{
			pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siPersonMoney );
			pclCM->CR[id]->SetUpdateSwitch( UPDATE_IP, true , 0);

			{
				LPARAM lParam = MAKELPARAM( pclMsg->siMsgID, LPARAMHIWORD_MONEY );

				cltGameMsgResponse_ListDelItem		sendMsg( LISTID_POSTOFFICE_RECVITEM, lParam );
				cltMsg clMsg(GAMEMSG_RESPONSE_LIST_DELITEM, sizeof(cltGameMsgResponse_ListDelItem), (BYTE*)&sendMsg);
				
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 10. 10] // 아이템(돈) 수신 로그쓰기. 
		//	// param1 == 1:아이템, 2:돈, 3:메시지, pszParam1 == 받는이 이름.
		//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_GET, 
		//						0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siSenderPersonID, 0, pclMsg->siPersonMoney, 0, 0, 0, 0, 0, 0, 0, 
		//						2, 0, 0, 0, 0, NULL, NULL);
		//}
		
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 8] // 우체국에서 돈 회수.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_GET_MONEY, 
				0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siSenderPersonID, GetMoney, pclMsg->siPersonMoney, NULL, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, NULL, NULL);
		}

	}

	if( returnval ) {
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_DELMSG(sPacketHeader* pPacket)
{
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_SETFEE(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	

	sDBResponse_PostOfficeSetFee* pclMsg = (sDBResponse_PostOfficeSetFee *)pPacket;

	SI32 id = pclMsg->usCharID;
	
	if( pclCM->IsValidID(id) == FALSE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID )	return;

	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_POSTOFFICE );

	if( pStrInfo ) {
		pStrInfo->clPostOfficeStrInfo.clMoneyFee.Set( &pclMsg->clMoneyFee );
		pStrInfo->clPostOfficeStrInfo.clItemFee.Set( &pclMsg->clItemFee );
		pStrInfo->clPostOfficeStrInfo.siMsgFee = pclMsg->siMsgFee;
		pStrInfo->clPostOfficeStrInfo.siRecvFee = pclMsg->siRecvFee;

		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_SUCCESS_SETFEE, 0, 0, pclCM->CR[id]->GetCharUnique());

	}

	

}

void cltServer::DoMsg_DBMSG_RESPONSE_POSTOFFICE_GETRECORD(sPacketHeader *pPacket )
{
	sDBResponse_PostOfficeGetRecord* pclMsg = (sDBResponse_PostOfficeGetRecord *)pPacket;

	SI32 id = pclMsg->usCharID;

	if( pclCM->IsValidID(id) == FALSE ) return;

	cltGameMsgResponse_PostOfficeGetRecord sendMsg;

	sendMsg.usCount = pclMsg->usCount; 

	memcpy( sendMsg.recordinfo, pclMsg->recordinfo, sizeof( cltPostOfficeRecordInfo ) * pclMsg->usCount );
	
	cltMsg clMsg(GAMEMSG_RESPONSE_POSTOFFICE_GETRECORD, sizeof(cltGameMsgResponse_PostOfficeGetRecord), (BYTE*)&sendMsg);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_SAVEUSERITEM(sPacketHeader* pPacket)
{
	sDBResponse_Get_SaveUserItem* pclMsg = (sDBResponse_Get_SaveUserItem*)pPacket;

	SI32 returnval = 0;	
	SI32 id = pclMsg->siCharId;

	if( pclCM->IsValidID(id) == false ) 
		return;

	if(pclClient->IsCountrySwitch(Switch_FourLeaf))
	{
		if( ITEMUNIQUE(2755) == pclMsg->clItem.siUnique || ITEMUNIQUE(2754) == pclMsg->clItem.siUnique	)
		{
			//[진성] 현재 사용가능한 네잎크로버인지 확인. => 2008-8-14
			if( pclMsg->clItem.siUnique != siFourLeafUnique )
			{
				sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), pclMsg->siItemPos, &pclMsg->clItem, USEITEM_REASON_NONE);
				pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
				pclMsg->siResult = -1;
			}
		}
	} 

	if( pclMsg->siResult == 1 )
	{
		returnval = SRVAL_GET_SAVEUSERITEM_SUCCESS;

		if(pclMsg->clItem.siUnique <= 0)
			return;
		
		if(pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )
			return;

		SI32 ref = pclItemManager->FindItemRefFromUnique(pclMsg->clItem.siUnique) ;
		if ( ref <= 0 ) 
			return ;

		// 인벤토리 갱신
		if( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siSaveItemPersonID )
		{
			pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siUserMoney);

			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clItem );

			pclCM->CR[id]->SetUpdateSwitch( UPDATE_INVENTORY, true, 0 );

			cltGameMsgResponse_Get_SaveUserItem sendMsg(pclMsg->siUserMoney);
			cltMsg clMsg(GAMEMSG_RESPONSE_GET_SAVEUSERITEM, sizeof(cltGameMsgResponse_Get_SaveUserItem), (BYTE*)&sendMsg);

			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // 클라라 npc에게 아이템 회수.
			// param1 == 아이템 인덱스, 
			// param2 == 아이템 위치.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_HOUSEUNIT_EXPIRED_ITEM_GET, 
											0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siSaveItemPersonID, 0, pclMsg->siUserMoney, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
											pclMsg->siIndex, pclMsg->siItemPos, 0, 0, 0, NULL, NULL);
		}
	}
	else 
	{
		returnval = SRVAL_GET_SAVEUSERITEM_FAIL;
	}

	if( returnval )
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	
}

//KHY - 1127 - 클라라 물품 보관 개선
void cltServer::DoMsg_DBMSG_RESPONSE_GET_SAVEUSERHORSE(sPacketHeader* pPacket)
{
	sDBResponse_Get_SaveUserHorse* pclMsg = (sDBResponse_Get_SaveUserHorse*)pPacket;

	SI32 returnval = 0;	
	SI32 id = pclMsg->siCharId;

	if( pclCM->IsValidID(id) == false ) 
		return;

	if( pclMsg->siResult == 1 )
	{
		returnval = SRVAL_GET_SAVEUSERITEM_SUCCESS;

		SI16 horseUnique = pclMsg->clHorse.siHorseUnique;
		if(horseUnique < 0)
			return ;

		if( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siSaveHorsePersonID )
		{
			pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siUserMoney);

			//pclCM->CR[id]->UnrideHorse(); - 내릴필요없다.
			pclCM->CR[id]->pclCI->clHorseInfo.SetHorse( pclMsg->siEnptyHorseindex, &pclMsg->clHorse );

			// 말 정보 전체를 업데이트 시킨다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_HORSE, true, 0);

			// 주위 사람들에게 말 변경 업데이트 시켜줌
			cltCharServer *pclChar = (cltCharServer*)pclCM->CR[id];
			pclChar->SendNeighbourMyHorseStatus(pclChar->pclCI->clHorseInfo.siCurrentShowHorse);


			cltGameMsgResponse_Get_SaveUserItem sendMsg(pclMsg->siUserMoney);
			cltMsg clMsg(GAMEMSG_RESPONSE_GET_SAVEUSERITEM, sizeof(cltGameMsgResponse_Get_SaveUserItem), (BYTE*)&sendMsg);

			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // 클라라 npc에게 말 회수.
			// param1 == 말 유니크 
			// param2 == 말 person 아이디.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_POSTOFFICE, LOGCOMMAND_INDEX_POSTOFFICE_HOUSEUNIT_EXPIRED_HORSE_GET, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, pclMsg->siUserMoney, NULL, 0, 0, 0, 0, 0, 0, 
											horseUnique, pclMsg->siSaveHorsePersonID, 0, 0, 0, NULL, NULL);
		}
	}
	else 
	{
		returnval = SRVAL_GET_SAVEUSERITEM_FAIL;
	}

	if( returnval )
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_SAVEUSERITEMLIST(sPacketHeader* pPacket)
{
	sDBResponse_Get_SaveUserItemList* pclMsg = (sDBResponse_Get_SaveUserItemList*)pPacket;

	if( pclMsg->siResult != 1)
		return;

	SI32 id = pclMsg->siCharId;

	if( pclCM->IsValidID(id) == false ) 
		return;

	cltGameMsgResponse_Get_SaveUserItemList sendMsg;

	sendMsg.siCharId = pclMsg->siCharId;
	sendMsg.siSaveItemPersonID = pclMsg->siSaveItemPersonID;

	//KHY - 1127 - 클라라 물품 보관 개선
	memcpy( sendMsg.clSaveUsersItemInfo, pclMsg->clSaveUsersItemInfo, sizeof( cltSaveUersItemInfo ) * MAX_SAVEUSERITEM_LIST );
	memcpy( sendMsg.clSaveUsersHorseInfo, pclMsg->clSaveUsersHorseInfo, sizeof( cltSaveUersHorseInfo ) * MAX_SAVEUSERHORSE_LIST );

	for(SI32 i=0;i<MAX_SAVEUSERITEM_LIST;++i)
	{
		if(pclMsg->clSaveUsersItemInfo[i].clItem.siUnique)
		{
			SI32 itemunique = pclMsg->clSaveUsersItemInfo[i].clItem.siUnique;
			cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclItemManager, itemunique);

			SI64 tempTax = 0;
			if(pclprice)		tempTax = ( (pclprice->siPrice / 100) * pclMsg->clSaveUsersItemInfo[i].clItem.GetItemNum() );

			// 클라라를 통한 물품 회수 수수료 없음
			if(bNoChargeGetItemFromNPC)				tempTax = 0;

			sendMsg.clSaveUsersItemInfo[i].siServiceCharge = tempTax;
		}
	}

	for(SI32 i=0;i<MAX_SAVEUSERHORSE_LIST;++i)
	{
		if(pclMsg->clSaveUsersHorseInfo[i].clHorse.siHorseUnique)
		{
			REAL32	npcrate = (REAL32)pclItemManager->clItemStatistic.siNPCPriceRate;
			npcrate = npcrate / (REAL32)100;

			SI64 serviceCharge = (SI64)((REAL32)20000 * npcrate);

			// 클라라를 통한 물품 회수 수수료 없음
			if(bNoChargeGetItemFromNPC)				serviceCharge = 0;

			sendMsg.clSaveUsersHorseInfo[i].siServiceCharge = serviceCharge;
		}
	}


	cltMsg clMsg(GAMEMSG_RESPONSE_GET_SAVEUSERITEMLIST, sizeof(cltGameMsgResponse_Get_SaveUserItemList), (BYTE*)&sendMsg);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}


void cltServer::DoMsg_DBMSG_RESPONSE_CHAR_NAME_CHECK(sPacketHeader* pPacket)
{
	sDBResponse_Char_Name_Check* pclinfo = (sDBResponse_Char_Name_Check*)pPacket;

	SI16 id = pclinfo->siCharId;

	if( false == pclCM->IsValidID(id) )					return;
	if( pclCM->CR[id]->GetCurrentVillageUnique() <= 0 )	return;
	if( 1 != pclinfo->siResult )						return;	
	if( 0 >= pclinfo->CheckPersonInfo.GetPersonID() )   return;

	switch( pclinfo->siReason )
	{
		case POSTOFFICE_ADD_RECV_USER:
			{
				bool m_bConnect = true;
				if(0 >= pclClient->pclCM->GetIDFromPersonID(pclinfo->CheckPersonInfo.siPersonID))
					m_bConnect = false;
				

				cltGameMsgResponse_Char_Add_Name_Check	sendMsg(pclinfo->CheckPersonInfo.szName, pclinfo->CheckPersonInfo.GetPersonID(), pclinfo->CheckPersonLevel, pclinfo->CheckPersonVillage, m_bConnect);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHAR_NAME_CHECK, sizeof(cltGameMsgResponse_Char_Add_Name_Check), (BYTE*)&sendMsg);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_FRIEND_INFO(sPacketHeader* pPacket)
{
	sDBResponse_Friend_Info* pclinfo = (sDBResponse_Friend_Info*)pPacket;

	SI16 id = pclinfo->m_siCharId;

	if( false == pclCM->IsValidID(id) )		return;
	if( 0 >= pclinfo->m_siRequestReason )	return;	// 이유가 없으면 리턴.
    
	
	// 각 리스트 요청 이유에 따른 처리를 하자.
	switch(pclinfo->m_siRequestReason)
	{
		case POSTOFFICE_GOURP_MESSAGE:
			{
				cltGameMsgResponse_PostOffice_User_List_Group	sendMsg;

				SI32 MaxCount = MAX_FRIEND_NUMBER;
				if(MAX_SEND_USER_NUM < MaxCount)
					MaxCount = MAX_SEND_USER_NUM;

				for(SI32 Count = 0; Count < MaxCount; ++Count)
				{
					if( 0 >= _tcslen( pclinfo->m_clCharInfo[Count].m_PersonName) )
						break;

					memcpy( sendMsg.m_szAddPersonNameGroup[Count], 
							pclinfo->m_clCharInfo[Count].m_PersonName,
							sizeof(sendMsg.m_szAddPersonNameGroup[Count]) );

					sendMsg.m_siLevel[Count]   = pclinfo->m_clCharInfo[Count].m_siLevel;
					sendMsg.m_siVillage[Count] = pclinfo->m_clCharInfo[Count].m_siVillageUnique;
					
					if(0 < pclCM->GetIDFromName(pclinfo->m_clCharInfo[Count].m_PersonName))
						sendMsg.m_bConnect[Count] = true;
				}

				cltMsg clMsg(GAMEMSG_RESPONSE_POSTOFFICE_USER_LIST_GROUP, sizeof(cltGameMsgResponse_PostOffice_User_List_Group), (BYTE*)&sendMsg);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_INFO(sPacketHeader* pPacket)
{
	sDBResponse_Guild_Info* pclinfo = (sDBResponse_Guild_Info*)pPacket;

	SI16 id = pclinfo->m_siCharId;

	if( false == pclCM->IsValidID(id) )		return;

	// 각 리스트 요청 이유에 따른 처리를 하자.
	switch(pclinfo->m_siRequestReason)
	{
		case POSTOFFICE_GOURP_MESSAGE:
			{
				cltGameMsgResponse_PostOffice_User_List_Group	sendMsg;

				SI32 MaxCount = MAX_GUILD_MEMBER_NUM;
				if(MAX_SEND_USER_NUM < MaxCount)
					MaxCount = MAX_SEND_USER_NUM;

				for(SI32 Count = 0; Count < MaxCount; ++Count)
				{
					if(0 >= _tcslen( pclinfo->m_clCharInfo[Count].m_PersonName) )
						break;

					memcpy( sendMsg.m_szAddPersonNameGroup[Count], 
							pclinfo->m_clCharInfo[Count].m_PersonName,
							sizeof(sendMsg.m_szAddPersonNameGroup[Count]) );

					sendMsg.m_siLevel[Count]   = pclinfo->m_clCharInfo[Count].m_siLevel;
					sendMsg.m_siVillage[Count] = pclinfo->m_clCharInfo[Count].m_siVillageUnique;

					if(0 < pclCM->GetIDFromName(pclinfo->m_clCharInfo[Count].m_PersonName))
						sendMsg.m_bConnect[Count] = true;
				}

				cltMsg clMsg(GAMEMSG_RESPONSE_POSTOFFICE_USER_LIST_GROUP, sizeof(cltGameMsgResponse_PostOffice_User_List_Group), (BYTE*)&sendMsg);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
	}

}


void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGE_RESIDENT_INFO(sPacketHeader* pPacket)
{
	sDBResponse_Village_Resident_Info* pclinfo = (sDBResponse_Village_Resident_Info*)pPacket;

	SI16 id = pclinfo->m_siCharId;

	if( false == pclCM->IsValidID(id) )		return;

	// 각 리스트 요청 이유에 따른 처리를 하자.
	switch(pclinfo->m_siRequestReason)
	{
		case POSTOFFICE_GOURP_MESSAGE:
			{
				cltGameMsgResponse_PostOffice_User_List_Group	sendMsg;

				SI32 MaxCount = MAX_RESIDENT_NUMBER;
				if(MAX_SEND_USER_NUM < MaxCount)
					MaxCount = MAX_SEND_USER_NUM;

				for(SI32 Count = 0; Count < MaxCount; ++Count)
				{
					if(0 >= _tcslen( pclinfo->m_clCharInfo[Count].m_PersonName) )
						break;

					memcpy( sendMsg.m_szAddPersonNameGroup[Count], 
							pclinfo->m_clCharInfo[Count].m_PersonName,
							sizeof(sendMsg.m_szAddPersonNameGroup[Count]) );

					sendMsg.m_siLevel[Count]   = pclinfo->m_clCharInfo[Count].m_siLevel;
					sendMsg.m_siVillage[Count] = pclinfo->m_clCharInfo[Count].m_siVillageUnique;
					
					if(0 < pclCM->GetIDFromName(pclinfo->m_clCharInfo[Count].m_PersonName))
						sendMsg.m_bConnect[Count] = true;

				}

				cltMsg clMsg(GAMEMSG_RESPONSE_POSTOFFICE_USER_LIST_GROUP, sizeof(cltGameMsgResponse_PostOffice_User_List_Group), (BYTE*)&sendMsg);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
	}
}