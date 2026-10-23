
#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"
#include "..\CommonLogic\Msg\MsgType-Friend.h"
#include "..\CommonLogic\MsgType-Person.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Friend.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDALLLIST(sPacketHeader* pPacket)
{
	sDBResponse_FriendAllList * pclMsg = (sDBResponse_FriendAllList *)pPacket;

	SI32 id	= pclMsg->siCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	if(pclCM->CR[id]->pclFriendInfo == NULL)		return;

	pclCM->CR[id]->pclFriendInfo->Set(&pclMsg->clPFriendInfo);

	int		i;
	// 접속 여부 및 지금 접속해 있다면, CharID 까지 얻어줌
	for (i = 0 ; i < MAX_FRIEND_NUMBER; ++i )
	{
		if ( pclMsg->clPFriendInfo.clFriend[ i ].siPersonID > 0 )
		{
			SI32 CharID = pclCM->GetIDFromPersonID( pclMsg->clPFriendInfo.clFriend[ i ].siPersonID );
			if ( CharID > 0 )
			{
				// LEEKH TEMP CODE - 친구정보 CharCommon으로 옮김
				pclCM->CR[id]->pclFriendInfo->Set( i, pclMsg->clPFriendInfo.siGroupID[i], CharID, true ,pclCM->CR[CharID]->pclCI->clBI.szAccountID);

				// 친구의 친구리스트에도 로그인했다는 정보로 바꿔줘야함( 안 그러면 로그아웃 상태이므로... )
				for ( SI16 k = 0; k < MAX_FRIEND_NUMBER; ++k )
				{
					if ( pclCM->CR[CharID]->pclFriendInfo->clFriend[k].GetPersonID() > 0 && pclCM->CR[CharID]->pclFriendInfo->clFriend[k].GetPersonID() == pclCM->CR[id]->pclCI->GetPersonID() )
					{
						pclCM->CR[CharID]->pclFriendInfo->bConnectStatus[k] = true;
						pclCM->CR[CharID]->pclFriendInfo->siCharID[k] =id;

						break;
					}
				}

				// 로그인인 경우에만 알린다.
				if(pclMsg->siLogin == 1)
				{
					// 친구에게 로그인했음을 알려줌
					cltGameMsgResponse_FriendLogin clGameMsgResponse_FriendLogin( pclMsg->clPFriendInfo.siGroupID[i], pclCM->CR[id]->pclCI->GetPersonID(), id ,pclCM->CR[id]->pclCI->clBI.szAccountID);
					cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDLOGIN, sizeof( clGameMsgResponse_FriendLogin ), (BYTE*)&clGameMsgResponse_FriendLogin );
					pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsg );
				}
			}
			else
			{
				// LEEKH TEMP CODE - 친구정보 CharCommon으로 옮김
				pclCM->CR[id]->pclFriendInfo->Set( i, pclMsg->clPFriendInfo.siGroupID[i], 0, false ,pclMsg->clPFriendInfo.szAccountID[i]);
			}
		}
		else
		{
			// LEEKH TEMP CODE - 친구정보 CharCommon으로 옮김
			pclCM->CR[id]->pclFriendInfo->Set( i, pclMsg->clPFriendInfo.siGroupID[i], 0, false ,pclMsg->clPFriendInfo.szAccountID[i]);
		}
	}

	// 클라이언트에 정보 전송
	cltGameMsgResponse_FriendAllList clinfo;
	clinfo.clFriendInfo.Set(pclCM->CR[id]->pclFriendInfo);

	cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDALLLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDADD(sPacketHeader* pPacket)
{
	sDBResponse_FriendAdd * pclMsg = (sDBResponse_FriendAdd *)pPacket;
	
	if ( pclMsg->siResult != 1 )
	{
		return;
	}
	
	SI32 id = pclMsg->siCharID;
	if(pclCM->IsValidID(id) == false)		return;

	if ( pclMsg->siFromPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	if ( pclMsg->siToPersonID < 1 )
	{
		return;
	}

	if ( pclMsg->strToPersonName == NULL || _tcslen( pclMsg->strToPersonName ) < 1 )
	{
		return;
	}

	if(pclCM->CR[id]->pclFriendInfo == NULL)			return;

	SI16 i = 0;
	for (; i < MAX_FRIEND_NUMBER; ++i )
	{
		cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];
		
		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
		{
			if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclMsg->strToPersonName ) == 0 )
			{
				// 친구 목록에 벌써 있으므로 추가할 필요 없음.
				return;
			}
		}
	}

	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siFromPersonID )
	{
		// 서버에서 친구 추가 해줌		
		for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];

			if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() < 1 )
			{
				pSimplePersonInfo->Set( pclMsg->siToPersonID, pclMsg->strToPersonName );
				break;
			}
		}
		
		if ( i < MAX_FRIEND_NUMBER )
		{
			// 친구가 몇번째 리스트에 추가 되었음을 알려줌
			cltGameMsgResponse_FriendAdd clinfo( FRIEND_GROUP_UNKNOWN, i, pclMsg->siToPersonID, pclMsg->strToPersonName );
			cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDADD, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDDEL(sPacketHeader* pPacket)
{
	sDBResponse_FriendDel *pclMsg = (sDBResponse_FriendDel *)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}
	
	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siFromPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	if ( pclMsg->siToPersonID < 1 )
	{
		return;
	}

	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siFromPersonID )
	{
		if(pclCM->CR[id]->pclFriendInfo == NULL)			return;

		// 서버에서 친구 삭제 해줌
		for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];

			if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() == pclMsg->siToPersonID )
			{
				pSimplePersonInfo->Init();
				break;
			}
		}

		if ( i < MAX_FRIEND_NUMBER )
		{
			// 몇번째 리스트에 있는 PersonID 가  삭제되었음으로 알려줌.
			cltGameMsgResponse_FriendDel clinfo( i, pclMsg->siToPersonID );
			cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDDEL, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	
	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDADDLIST(sPacketHeader* pPacket)
{
	sDBResponse_FriendAddList *pclMsg = (sDBResponse_FriendAddList*)pPacket;


	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if( pclMsg->siFromPersonID < 1 || pclMsg->siFromCharID < 1 )
	{
		if( pclMsg->siToPersonID > 0 || pclMsg->siToCharID > 0 )
		{
			cltCharServer* pclChar = pclCM->GetCharServer(pclMsg->siToCharID);
			if( pclChar ) 
			{
				SI32 i = 0;
				// 친구 리스트 목록에 있는지 없는지 체크
				for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
				{
					cltSimplePerson *pSimplePersonInfo = &pclChar->pclFriendInfo->clFriend[i];

					if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
					{
						if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclMsg->strFromPersonName ) == 0 )
						{
							// 친구 목록에 벌써 있으므로 추가할 필요 없음.
							break;
						}
					}
				}

				// 친구 목록에 없으므로, 추가해 줌.
				if ( i == MAX_FRIEND_NUMBER )
				{
					// 서버에서 친구 추가 해줌		
					for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclChar->pclFriendInfo->clFriend[i];

						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() < 1 )
						{
							pclChar->pclFriendInfo->Set( i, pclMsg->siFromGroupID, pclMsg->siFromPersonID, 0, pclMsg->strFromPersonName, false ,TEXT(""));
							break;
						}
					}
				}
			}
		}
		return;
	}

	if( pclMsg->siToPersonID < 1 || pclMsg->siToCharID < 1 )
	{
		return;
	}

	SI32 id1 = pclMsg->siFromCharID;
	SI32 id2 = pclCM->GetIDFromPersonID( pclMsg->siToPersonID );

	//---------------------------------------------------------------
	// 친구로 등록되었음을 2명에게 알려줌
	SI16 i = 0;
	if ( pclCM->IsValidID(id1) && pclCM->CR[id1]->pclCI->GetPersonID() == pclMsg->siFromPersonID )
	{
		if(pclCM->CR[id1]->pclFriendInfo)
		{
			for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
			{
				cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id1]->pclFriendInfo->clFriend[i];

				if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
				{
					if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclMsg->strToPersonName ) == 0 )
					{
						// 친구 목록에 벌써 있으므로 추가할 필요 없음.
						break;
					}
				}
			}

			// 친구 목록에 없으므로, 추가해 줌.
			if ( i == MAX_FRIEND_NUMBER )
			{
				// 서버에서 친구 추가 해줌		
				for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
				{
					cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id1]->pclFriendInfo->clFriend[i];
					
					if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() < 1 )
					{
						if(  id2 > 0 )
						{
							pclCM->CR[id1]->pclFriendInfo->Set( i, pclMsg->siToGroupID, pclMsg->siToPersonID, id2, pclMsg->strToPersonName, true ,pclCM->CR[id2]->pclCI->clBI.szAccountID);
							StringCchCopy(m_szFriendAccountID, MAX_PLAYER_NAME, pclCM->CR[id2]->pclCI->clBI.szAccountID);
						}
						else
						{
							pclCM->CR[id1]->pclFriendInfo->Set( i, pclMsg->siToGroupID, pclMsg->siToPersonID, 0, pclMsg->strToPersonName, false ,m_szFriendAccountID);
						}
						break;
					}
				}

				// 친구를 추가한 경우에 알려줘야 하므로...
				if ( i < MAX_FRIEND_NUMBER )
				{
					// 친구가 몇번째 리스트에 추가 되었음을 알려줌
					cltGameMsgResponse_FriendAddAccept clinfo( i, pclMsg->siToGroupID, pclMsg->siToPersonID, id2, pclMsg->strToPersonName ,m_szFriendAccountID);
					cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDADDACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[id1]->SendNetMsg((sPacketHeader*)&clMsg);
					
					// LEEKH MODIFY 2007.10.25
					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 황진성 2007. 9. 3] // 친구 등록 쓰기. 
						// param1 == 친구 추가 위치, param2 == 그룹 아이디
						SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FRIEND, LOGCOMMAND_INDEX_FRIEND_ADD, 
					 						0, (cltCharServer*)(pclCM->CR[id1]), NULL, pclMsg->siToPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											i, pclMsg->siToGroupID, 0, 0, 0, NULL, NULL);
					}

					cltLetterMsg_AcceptFriend_Result clletter( cltLetterMsg_AcceptFriend_Result::ACCEPTFRIEND, pclMsg->strToPersonName );
					// 쪽지를 DB나 클라이언트에게 보낸다. 
					SendLetterMsg( pclCM->CR[id1]->pclCI->GetPersonID(), (cltLetterHeader*)&clletter);
				}
			}
		}
	}

	if ( pclCM->IsValidID(id2) && pclCM->CR[id2]->pclCI->GetPersonID() == pclMsg->siToPersonID )
	{
		if(pclCM->CR[id2]->pclFriendInfo)
		{
			// 친구 리스트 목록에 있는지 없는지 체크
			for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
			{
				cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id2]->pclFriendInfo->clFriend[i];

				if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
				{
					if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclMsg->strFromPersonName ) == 0 )
					{
						// 친구 목록에 벌써 있으므로 추가할 필요 없음.
						break;
					}
				}
			}
		
			// 친구 목록에 없으므로, 추가해 줌.
			if ( i == MAX_FRIEND_NUMBER )
			{
				// 서버에서 친구 추가 해줌		
				for ( i = 0; i < MAX_FRIEND_NUMBER; ++i )
				{
					cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id2]->pclFriendInfo->clFriend[i];
					
					if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() < 1 )
					{
						if ( pclCM->IsValidID(id1) )
						{
							pclCM->CR[id2]->pclFriendInfo->Set( i, pclMsg->siFromGroupID, pclMsg->siFromPersonID, pclMsg->siFromCharID, pclMsg->strFromPersonName, true ,pclCM->CR[id1]->pclCI->clBI.szAccountID);
						}
						else
						{
							pclCM->CR[id2]->pclFriendInfo->Set( i, pclMsg->siFromGroupID, pclMsg->siFromPersonID, 0, pclMsg->strFromPersonName, false ,TEXT(""));
						}
						break;
					}
				}
				
				// 친구를 추가 했음 알려줌.
				if ( i < MAX_FRIEND_NUMBER )
				{
					// 친구가 몇번째 리스트에 추가 되었음을 알려줌
					cltGameMsgResponse_FriendAddAccept clinfo( i, pclMsg->siFromGroupID, pclMsg->siFromPersonID, pclMsg->siFromCharID, pclMsg->strFromPersonName,pclCM->CR[id1]->pclCI->clBI.szAccountID );
					cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDADDACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[id2]->SendNetMsg((sPacketHeader*)&clMsg);

					// LEEKH MODIFY 2007.10.25
					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 황진성 2007. 9. 3] // 친구 등록 쓰기. 
						// param1 == 친구 추가 위치, param2 == 그룹 아이디
						SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FRIEND, LOGCOMMAND_INDEX_FRIEND_ADD, 
											0, (cltCharServer*)(pclCM->CR[id2]), NULL, pclMsg->siFromPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											i, pclMsg->siFromGroupID, 0, 0, 0, NULL, NULL);
					}

					cltLetterMsg_AcceptFriend_Result clletter( cltLetterMsg_AcceptFriend_Result::ACCEPTFRIEND, pclMsg->strFromPersonName );
					// 쪽지를 DB나 클라이언트에게 보낸다. 
					SendLetterMsg( pclCM->CR[id2]->pclCI->GetPersonID(), (cltLetterHeader*)&clletter);
				}
			}
		}
	}
	//---------------------------------------------------------------


	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDDELLIST(sPacketHeader* pPacket)
{
	sDBResponse_FriendDelList *pclMsg = (sDBResponse_FriendDelList*)pPacket;
	
	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if ( pclMsg->siFromPersonID < 1 || pclMsg->siToPersonID < 1 )
	{
		return;
	}

	SI32 FromID = pclMsg->siFromCharID;
	
	// 삭제 당한 사람이 존재하는 경우( 알려줘야 하므로 )
	SI32 ToID = pclCM->GetIDFromPersonID( pclMsg->siToPersonID );
	if ( ToID > 0 )
	{	
		if(pclCM->CR[ToID]->pclFriendInfo)
		{
			// 서버에서 친구 삭제 해줌
			for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
			{
				cltSimplePerson *pSimplePersonInfo = &pclCM->CR[ToID]->pclFriendInfo->clFriend[ i ];

				if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() == pclMsg->siFromPersonID )
				{
					pclCM->CR[ToID]->pclFriendInfo->Init( i );
					break;
				}
			}

			if ( i < MAX_FRIEND_NUMBER )
			{
				// 몇번째 리스트에 있는 PersonID 가  삭제되었음으로 알려줌.
				cltGameMsgResponse_FriendDel clinfo( i, pclMsg->siFromPersonID );
				cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDDEL, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[ToID]->SendNetMsg((sPacketHeader*)&clMsg);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 9] // 친구 삭제 쓰기. 
					// param1 == 리스트 번호.
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FRIEND, LOGCOMMAND_INDEX_FRIEND_DEL, 
						0, (cltCharServer*)(pclCM->CR[ToID]), NULL, pclMsg->siFromPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
						i, 0, 0, 0, 0, NULL, NULL);
				}
			}
		}
	}

	// 삭제 요청한 사람이 존재하는 경우( 알려줘야 하므로 )
	if ( pclCM->IsValidID(FromID) && pclCM->CR[FromID]->pclCI->GetPersonID() == pclMsg->siFromPersonID )
	{
		if(pclCM->CR[FromID]->pclFriendInfo)
		{
			// 서버에서 친구 삭제 해줌
			for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
			{
				cltSimplePerson *pSimplePersonInfo = &pclCM->CR[FromID]->pclFriendInfo->clFriend[ i ];

				if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() == pclMsg->siToPersonID )
				{
					pclCM->CR[FromID]->pclFriendInfo->Init( i );
					break;
				}
			}

			if ( i < MAX_FRIEND_NUMBER )
			{
				// 몇번째 리스트에 있는 PersonID 가  삭제되었음으로 알려줌.
				cltGameMsgResponse_FriendDel clinfo( i, pclMsg->siToPersonID );
				cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDDEL, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[FromID]->SendNetMsg((sPacketHeader*)&clMsg);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 9] // 친구 삭제 쓰기.
					// param1 == 리스트 번호.
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FRIEND, LOGCOMMAND_INDEX_FRIEND_DEL, 
						0, (cltCharServer*)(pclCM->CR[FromID]), NULL, pclMsg->siToPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
						i, 0, 0, 0, 0, NULL, NULL);
				}
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDMEMO(sPacketHeader* pPacket)
{
	sDBResponse_FriendMemo *pclMsg = (sDBResponse_FriendMemo*)pPacket;

	if ( pclMsg->siResult < 1 )
		return;

	if ( pclCM->IsValidID( pclMsg->siCharID ) )
	{
		cltGameMsgResponse_FriendMemo clinfo( pclMsg->szFriendMemo );
		cltMsg clMsg(GAMEMSG_RESPONSE_FRIENDMEMO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[pclMsg->siCharID]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDMEMOSET(sPacketHeader* pPacket)
{
	sDBResponse_FriendMemoSet *pclMsg = (sDBResponse_FriendMemoSet*)pPacket;

	if ( pclCM->IsValidID( pclMsg->siCharID ) == false ) 
		return;

	bool bSuccess = false;

    if ( pclMsg->siResult == 1 )
		bSuccess = true;

	cltGameMsgResponse_FriendMemoSet clinfo( bSuccess );
	cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDMEMOSET, sizeof(clinfo), (BYTE*)&clinfo );
	pclCM->CR[pclMsg->siCharID]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDCHANGEGROUP(sPacketHeader* pPacket)
{
	sDBResponse_FriendChangeGroup *pclMsg = (sDBResponse_FriendChangeGroup*)pPacket;

	if ( pclCM->IsValidID( pclMsg->siCharID ) == false ) 
		return;

	if ( pclMsg->siResult != 1 )
		return;

	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siCharID < 1 )
	{
		return;
	}

	if ( pclMsg->siToPersonID < 1 )
	{
		return;
	}

	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siFromPersonID )
	{
		if(pclCM->CR[id]->pclFriendInfo)
		{
			// 서버에서 친구의 그룹을 바꿈
			for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
			{
				cltSimplePerson *pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];

				if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() == pclMsg->siToPersonID )
				{
					pclCM->CR[id]->pclFriendInfo->siGroupID[i] = pclMsg->siGroupID;
					break;
				}
			}

			if ( i < MAX_FRIEND_NUMBER )
			{
				// 몇번째 리스트에 있는 PersonID 의 그룹ID가 변경되었는지 알려줌.
				cltGameMsgResponse_FriendChangeGroup clinfo( i, pclMsg->siGroupID, pclMsg->siToPersonID );
				cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDCHANGEGROUP, sizeof(clinfo), (BYTE*)&clinfo );
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_FRIENDCHANGEGROUPNAME(sPacketHeader* pPacket)
{
	sDBResponse_FriendChangeGroupName *pclMsg = (sDBResponse_FriendChangeGroupName*)pPacket;

	if ( pclCM->IsValidID( pclMsg->siCharID ) == false ) 
		return;

	bool bSuccess = false;

	if ( pclMsg->siResult == 1 )
		bSuccess = true;

	cltGameMsgResponse_FriendChangeGroupName clinfo( bSuccess );
	for(int i=0; i<MAX_FRIEND_GROUP; i++)
		MStrCpy(clinfo.szGroupName[i], pclMsg->strGroupName[i], MAX_FRIEND_GROUPNAME);

	cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDCHANGEGROUPNAME, sizeof(clinfo), (BYTE*)&clinfo );
	pclCM->CR[pclMsg->siCharID]->SendNetMsg((sPacketHeader*)&clMsg);
}