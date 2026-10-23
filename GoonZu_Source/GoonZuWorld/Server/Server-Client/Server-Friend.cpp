//---------------------------------
// 2004/2/18 김태곤
//---------------------------------
#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Friend.h"

#include "..\Common\Order\Order.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-Friend.h"

#include "../../Lib/HashTable/HashTable_StringInt.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDADD( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FriendAdd* pclinfo = (cltGameMsgRequest_FriendAdd*)pclMsg->cData;

	StringCchCopy(m_szFriendAccountID, MAX_PLAYER_NAME,TEXT(""));

	if( pclCM->IsAlive( id ) == false )return ;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	if ( pclinfo->strFriendName == NULL || _tcslen( pclinfo->strFriendName ) < 1 )
	{
		return;
	}


	TCHAR *strName = (TCHAR*)pclCM->CR[id]->GetName();
	if ( strName == NULL )
	{
		return;
	}

	// 자기 자신을 추가 할 수 없음.
	if ( _tcscmp( strName, pclinfo->strFriendName ) == 0 )
	{
		return;
	}

	if(pclCM->CR[id]->pclFriendInfo == NULL)		return;

	// 기존에 목록에 있는지 체크
	cltSimplePerson* pSimplePersonInfo = NULL;
	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];
		
		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
		{
			if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclinfo->strFriendName ) == 0 )
			{
				// 친구 목록에 있으므로 추가할 필요 없음.
				return;
			}
		}
	}

	/*
	sDBRequest_FriendAdd clMsg( 
		FromPersonID, 
		id, 
		pclinfo->strFriendName
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	*/
	
	//----------------------------------------------------
	// Hash 테이블에서 이름으로 검색해서 PersonID 와 CharID 를 받아옮
	// false 로 리턴 되는 경우는 유저가 현재 접속되지 않은 상태임
	SI32 ToPersonID, ToCharID;
	if ( pclHashTable->Find( pclinfo->strFriendName, ToPersonID, ToCharID , 50) == false  )
	{
		SendServerResponseMsg(0, SRVAL_FRIEND_NOTCONNECTED,  0, 0, id);
		return;
	}
	//----------------------------------------------------
	
	if( pclCM->IsValidID(ToCharID) == false )	return;

	cltCharServer *pclChar = ( cltCharServer * )pclCM->CR[ToCharID];

	// 친구 추가 거부 상태
	if ( pclChar->bFriendAddReject )
	{
		SendServerResponseMsg(0, SRVAL_FRIENDADD_REJECT,  0, 0, id);
		return;
	}
	
	
	StringCchCopy(m_szFriendAccountID, MAX_PLAYER_NAME, pclCM->CR[id]->pclCI->clBI.szAccountID);
	
	// 친구로 추가하기를 원하는 유저에게 추가 요청 보냄
	cltGameMsgResponse_FriendAdd clGameMsgResponse_FriendAdd( pclinfo->siGroupID, id, FromPersonID, strName );
	cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDADD, sizeof( clGameMsgResponse_FriendAdd ), (BYTE*)&clGameMsgResponse_FriendAdd );
	pclCM->CR[ToCharID]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDDEL( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FriendDel* pclinfo = (cltGameMsgRequest_FriendDel*)pclMsg->cData;
	
	if(pclCM->IsAlive(id) == false)return ;
	
	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	if ( pclinfo->siPersonID < 1 )
	{
		return;
	}

	if(pclCM->CR[id]->pclFriendInfo == NULL)		return;

	SI16 i = 0;
	cltSimplePerson* pSimplePersonInfo = NULL;
	for (; i < MAX_FRIEND_NUMBER; ++i )
	{
		pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];
		
		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() == pclinfo->siPersonID )
		{
			break;
		}
	}

	// 삭제할 수 있는 PersonID 를 발견 할 수 없음.
	if ( i == MAX_FRIEND_NUMBER )
	{
		return;
	}

	sDBRequest_FriendDelList clMsg( 
		FromPersonID, 
		id, 
		pclinfo->siPersonID
		);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return;
}

// 친구 추가에 대해서 accept 가 왔다면, 무조건 믿고 그냥 친구 추가 함.
// 왜냐하면, 조작 되어서 데이터가 날아온다고 해도 별 문제 안될듯...
void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDADDACCEPT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FriendAddAccept* pclinfo = (cltGameMsgRequest_FriendAddAccept*)pclMsg->cData;
	
	if( pclCM->IsAlive( id ) == false )
		return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1 )
		return ;
	
	// accept 가 온 경우는 무조건 믿고 넘어감. 아래 if 문에서 체크하는 것은, 추가되는 유저의 정보인데...
	// 굳이 그 유저가 없다고 해서 친구 추가가 안 된다면... 이상함...
	// ex) 친구 추가 신청을 10분전에 받았는데... 그 때 자리에 없어서 accept 를 못하다가,
	// 10분뒤에 와서 추가 할때 그 사람이 없다고 친구 추가가 안되면 이상하잖아??
	//if ( pclCM->IsValidID ( pclinfo->siCharID ) && pclCM->CR[ pclinfo->siCharID ]->pclCI->GetPersonID() == 
	
	if ( pclinfo->strCharName == NULL || _tcslen( pclinfo->strCharName ) < 1 )
	{
		return;
	}


	if ( pclCM->CR[id]->GetName() == NULL || _tcslen( pclCM->CR[id]->GetName() ) < 1 )
	{
		return;
	}

	// 자기 자신은 추가 할 수 없음
	if ( _tcscmp( pclinfo->strCharName, pclCM->CR[id]->GetName() ) == 0 )
	{
		return;
	}

	if( pclCM->CR[id]->pclFriendInfo == NULL )				return;

	// 기존에 목록에 있는지 체크
	cltSimplePerson* pSimplePersonInfo = NULL;
	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];
		
		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
		{
			if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclinfo->strCharName ) == 0 )
			{
				// 친구 목록에 있으므로 추가할 필요 없음.
				return;
			}
		}
	}

	sDBRequest_FriendAddList clMsg( pclinfo->siGroupID, FromPersonID, id, (TCHAR*)pclCM->CR[id]->GetName(), FRIEND_GROUP_UNKNOWN, pclinfo->siPersonID, pclinfo->siCharID, pclinfo->strCharName );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_REJECTFRIENDADD( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FriendAddAccept* pclinfo = (cltGameMsgRequest_FriendAddAccept*)pclMsg->cData;

	if( pclCM->IsValidID(id) == false )
		return;

	if( 0 > pclinfo->siPersonID )
		return;

	cltLetterMsg_AcceptFriend_Result clletter( cltLetterMsg_AcceptFriend_Result::REJECTFRIEND, (TCHAR*)pclCM->CR[id]->GetName() );
	// 쪽지를 DB나 클라이언트에게 보낸다. 
	SendLetterMsg( pclinfo->siPersonID, (cltLetterHeader*)&clletter);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDLIST( cltMsg* pclMsg, SI32 id )
{
	if( pclCM->IsAlive( id ) == false )
		return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1 )
		return ;

	if(pclCM->CR[id]->pclFriendInfo == NULL)		return;

	SI32 mapindex[MAX_FRIEND_NUMBER] = {0,};
	SI32 level[MAX_FRIEND_NUMBER] = {0,};
	SI32 homemapindex[MAX_FRIEND_NUMBER] = {0,};
	SI32 charkind[MAX_FRIEND_NUMBER] = {0,};
	cltSimpleRank clcharRank[MAX_FRIEND_NUMBER];
	UI08 gRank[MAX_FRIEND_NUMBER] = {0,};


	cltCharCommon* pclchar = NULL;
	for( SI32 i=0 ; i<MAX_FRIEND_NUMBER ; i++ )
	{
		SI32 friendid = pclCM->GetIDFromPersonID(pclCM->CR[id]->pclFriendInfo->clFriend[i].GetPersonID());

		if ( pclCM->IsValidID( friendid ) == false  )
			continue;

		pclchar = pclCM->CR[friendid];

		if ( pclchar )
		{
			mapindex[i] = pclchar->GetMapIndex();
			level[i] = pclchar->pclCI->clIP.GetLevel();
			homemapindex[i] = pclchar->pclCI->clBI.siHomeVillage;
			charkind[i] = pclchar->GetKind();
			clcharRank[i].Set( &pclchar->pclCI->clCharRank );
			gRank[i] = pclchar->pclCI->clBI.uiGRank;
		}
	}

#ifdef _SAFE_MEMORY
	cltGameMsgResponse_FriendList clinfo( &pclCM->CR[id]->pclFriendInfo->siGroupID[0], &pclCM->CR[id]->pclFriendInfo->clFriend[0], mapindex, level, homemapindex, charkind, clcharRank, gRank );
#else
	cltGameMsgResponse_FriendList clinfo( pclCM->CR[id]->pclFriendInfo->siGroupID, pclCM->CR[id]->pclFriendInfo->clFriend, mapindex, level, homemapindex, charkind, clcharRank, gRank );
#endif
	cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDLIST, sizeof( cltGameMsgResponse_FriendList ), (BYTE*)&clinfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDMEMO(cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsValidID( id ) ==  false )
		return;

	cltGameMsgRequest_FriendMemo* pclinfo = (cltGameMsgRequest_FriendMemo*)pclMsg->cData;

	sDBRequest_FriendMemo clMsg( pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->siFriendPersonID, id );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDMEMOSET(cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsValidID( id ) == false )
		return;

	cltGameMsgRequest_FriendMemoSet* pclinfo = (cltGameMsgRequest_FriendMemoSet*)pclMsg->cData;

	sDBRequest_FriendMemoSet clMsg( pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->siFriendPersonID, pclinfo->szFriendMemo, id );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDCHANGEGROUP( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FriendChangeGroup* pclinfo = (cltGameMsgRequest_FriendChangeGroup*)pclMsg->cData;

	if(pclCM->IsAlive(id) == false)return ;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	if ( pclinfo->siFriendPersonID < 1 )
	{
		return;
	}

	if( pclCM->CR[id]->pclFriendInfo == NULL )			return;

	SI16 i = 0;
	cltSimplePerson* pSimplePersonInfo = NULL;
	for (; i < MAX_FRIEND_NUMBER; ++i )
	{
		pSimplePersonInfo = &pclCM->CR[id]->pclFriendInfo->clFriend[i];

		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() == pclinfo->siFriendPersonID )
		{
			break;
		}
	}

	// 그룹을 변경할 수 있는 PersonID 를 발견 할 수 없음.
	if ( i == MAX_FRIEND_NUMBER )
	{
		return;
	}

	sDBRequest_FriendChangeGroup clMsg( 
		FromPersonID, 
		id, 
		pclinfo->siGroupID,
		pclinfo->siFriendPersonID
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FRIENDCHANGEGROUPNAME( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_FriendChangeGroupName* pclinfo = (cltGameMsgRequest_FriendChangeGroupName*)pclMsg->cData;

	if(pclCM->IsAlive(id) == false)return ;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	sDBRequest_FriendChangeGroupName clMsg( 
		FromPersonID, 
		id
		);

	for(int i=0; i<MAX_FRIEND_GROUP; i++)
		MStrCpy(clMsg.strGroupName[i], pclinfo->szGroupName[i], MAX_FRIEND_GROUPNAME);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}