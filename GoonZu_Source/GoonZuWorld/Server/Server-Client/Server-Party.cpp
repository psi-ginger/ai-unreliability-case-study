#include "..\Server.h"
#include "..\..\CommonLogic\Msg\MsgType-Party.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\Common\PartyMgr\PartyMgr.h"
#include "..\..\Common\PartyMgr\PartyObj.h"
#include "../../common/Order/order.h"
#include "Char\CharManager\CharManager.h"
#include "Map/map.h"

#include "../../CommonLogic/MsgRval-Define.h"
#include "../../client/ninterface/npartydlg/NPartyDlg.h"

/*
// 파티 생성
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_CREATE( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_Party_Create *pclPartyCreate = (cltGameMsgRequest_Party_Create *) pclMsg->cData;

	// 파티명이 없게 해서 만들라고 왔음.
	if( _tcslen( pclPartyCreate->strPartyName ) < 1 )
	{
		return;
	}

	if ( pclCM->CR[id]->GetCharUnique() < 1 )
	{
		return;
	}

	// 파티에 참가중이면, 파티 생성 안됨.
	if ( pclCM->CR[id]->siPartyIndex > 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();

	if( FromPersonID < 1 )
		return ;

	SI32 siPartyIndex = pclPartyMgr->CreateParty( FromPersonID, pclCM->CR[id]->GetCharUnique() );

	// 파티 생성 개수 초과한 경우
	if (  siPartyIndex < 0 )
	{
		return;
	}

	pclCM->CR[id]->siPartyIndex  = siPartyIndex; 

	cltGameMsgResponse_Party_Create clGameMsgResponse_Party_Create( siPartyIndex );
	cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_CREATE, sizeof( clGameMsgResponse_Party_Create ), (BYTE*)&clGameMsgResponse_Party_Create );
	pclCM->CR[id]->SendNetMsg( (BYTE*)&clMsg );

	return;
}
*/

// 파티장이 될려는 사람이 파티를 하기 위해서 어떤 유저에게 파티 초대를 하는 부분
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_CREATEINVITE( cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltGameMsgRequest_Party_CreateInvite *pclPartyCreateInvite = (cltGameMsgRequest_Party_CreateInvite *) pclMsg->cData;

	if ( pclPartyCreateInvite->siInvitedCharUnique < 1 )
	{
		return;
	}

	// 파티 중이면 파티 생성 하면서 초대하는 이 메시지는 보낼 수 없음.
	if ( pclCM->CR[ id ]->siPartyIndex > 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	SI32 toid = pclCM->GetIDFromCharUnique( pclPartyCreateInvite->siInvitedCharUnique );
	if(toid < 1) return;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID < 1) return;

	// 파티에 참가중이면, 파티 초대 안됨.
	if ( pclCM->CR[ toid ]->siPartyIndex > 0 )
	{
		SendServerResponseMsg(0, SRVAL_PARTYINVITE_ALREADYPARTY,  0, 0, id);
		return;
	}

	// 파티 초대 거부 상태
	cltCharServer *pclChar = ( cltCharServer * )pclCM->CR[toid];

	if ( pclChar->bPartyInviteReject )
	{
		SendServerResponseMsg( 0, SRVAL_PARTYINVITE_REJECT,  0, 0, id );
		return;
	}

	TCHAR *strCharName = (TCHAR*)pclCM->CR[id]->pclCI->clBI.GetName();

	if ( strCharName == NULL )
	{
		return;
	}

	// 파티 대상에게 파티에 참여할것인지를 물음
	cltGameMsgResponse_Party_CreateInvite clGameMsgResponse_Party_CreateInvite( strCharName, FromPersonID );
	cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_CREATEINVITE, sizeof( clGameMsgResponse_Party_CreateInvite ), (BYTE*)&clGameMsgResponse_Party_CreateInvite );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg );

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_CREATEJOIN( cltMsg* pclMsg, SI32 id )
{
	WRITE_MEMLOG( 150, 1, 0, 0 );

	cltGameMsgRequest_Party_CreateJoin *pclPartyCreateJoin = (cltGameMsgRequest_Party_CreateJoin *) pclMsg->cData;


	//---------------------------------------------
	// [기형] 배틀로얄 경기장이라면 
	//---------------------------------------------
	if(NULL != m_pclBattleRoyalMgr)
	{
		cltCharServer* pclchar = pclCM->GetCharServer(id);
		if ( NULL == pclchar)
			return;

		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclchar);
		if( NULL != pclBattleRoyal)
		{
			if( TRUE == pclBattleRoyal->IsExistStadium(pclchar) )
			{
				return;
			}
		}
	}

	if ( pclPartyCreateJoin->siLeaderPersonID < 1 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 파티 중이면서 다른 파티 생성 초대에 응할 수는 없는 일이지...
	if ( pclCM->CR[ id ]->siPartyIndex > 0 )
	{
		return;
	}

	SI32 LeaderID = pclCM->GetIDFromPersonID( pclPartyCreateJoin->siLeaderPersonID );

	if(pclCM->IsValidID(LeaderID) == false) return;

	// 요청한 그 사이에 파티장이 될 사람이 파티에 참가중이면, 
	if ( pclCM->CR[ LeaderID ]->siPartyIndex > 0 )
	{

		SendServerResponseMsg(0, SRVAL_PARTYINVITE_LEADERALREADYPARTY,  0, 0, id);

		WRITE_MEMLOG( 150, 2, 0, 0 );
		return;
	}

	SI32 siPartyIndex = pclPartyMgr->CreateParty( pclPartyCreateJoin->siLeaderPersonID, pclCM->CR[LeaderID]->GetCharUnique(), pclCM->CR[LeaderID]->pclCI->clIP.GetLevel() );

	// 파티 생성 개수 초과한 경우
	if (  siPartyIndex < 0 )
	{
		return;
	}

	pclCM->CR[LeaderID]->siPartyIndex = siPartyIndex; 
	pclCM->CR[id]->siPartyIndex = siPartyIndex; 

	CPartyObj *pPartyObj = pclPartyMgr->GetPartyObj( siPartyIndex );

	// 인덱스 번호의 파티가 존재한다.
	if ( pPartyObj == NULL )
	{
		return;
	}

	// 파티번호의 파티장이 맞는지 확인해봄
	if ( ! pPartyObj->IsPartyLeader( pclPartyCreateJoin->siLeaderPersonID ) )
	{
		return;
	}

	SI16 JoinedIndex = pPartyObj->JoinParty( FromPersonID, pclCM->CR[id]->GetCharUnique(),pclCM->CR[id]->pclCI->clIP.GetLevel() );

	// 이 경우는 아래에 JoinedIndex < 0  이 될 리가 절대 없겠지...
	if ( JoinedIndex < 0 )
	{
		pclLog->FilePrint( TEXT("PartyError.txt"), TEXT("Joined Index = %d"), JoinedIndex );
		return;
	}

	TCHAR *strLeaderCharName = (TCHAR*)pclCM->CR[LeaderID]->pclCI->clBI.GetName();
	if ( strLeaderCharName == NULL )
	{
		return;
	}

	TCHAR *strMemberCharName = (TCHAR*)pclCM->CR[id]->pclCI->clBI.GetName();
	if ( strMemberCharName == NULL )
	{
		return;
	}

	// 파티 이름 얻어오고 파티 오브젝트에 세팅
	TCHAR* pszPartyName = NULL;
	pszPartyName = m_pServer_PartyMatchingMgr->GetPartyName( pclCM->CR[LeaderID]->GetCharUnique() );
	pPartyObj->SetPartyName( pszPartyName );

	// 파티원으로 다른 파티에 가입하는 사람이 만든 파티매칭이 있다면 지워준다.
	m_pServer_PartyMatchingMgr->Remove_PartyMatching( pclCM->CR[id]->GetCharUnique() );

	// 파티퀘 정보 얻어옴
	bool bIsPartyQuestParty = false;
	SI32 siCategory = -1;
	SI32 siMission = -1;

	bIsPartyQuestParty = m_pServer_PartyMatchingMgr->IsExist( pclCM->CR[LeaderID]->GetCharUnique() );
	if ( true == bIsPartyQuestParty )
	{
		siCategory = m_pServer_PartyMatchingMgr->Get_Category( pclCM->CR[LeaderID]->GetCharUnique() );
		siMission = m_pServer_PartyMatchingMgr->Get_Mission( pclCM->CR[LeaderID]->GetCharUnique() );
	}

	//리더의 위치
	cltCharPos clLeaderCharPos(pclCM->CR[LeaderID]->GetMapIndex(), pclCM->CR[LeaderID]->GetX(),pclCM->CR[LeaderID]->GetY());
	//파티원의 위치
	cltCharPos clMemberCharPos(pclCM->CR[id]->GetMapIndex(), pclCM->CR[id]->GetX(),pclCM->CR[id]->GetY());

	// 파티원꺼
	cltGameMsgResponse_Party_CreateJoin clGameMsgResponse_Party_CreateJoin( siPartyIndex, pclPartyCreateJoin->siLeaderPersonID, pclCM->GetIDFromCharUnique(LeaderID), pclCM->CR[LeaderID]->pclCI->clIP.GetLevel(), strLeaderCharName, &clLeaderCharPos , FromPersonID, pclCM->GetIDFromCharUnique(id), pclCM->CR[id]->pclCI->clIP.GetLevel(), strMemberCharName,&clMemberCharPos, pszPartyName );
	cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_CREATEJOIN, sizeof( clGameMsgResponse_Party_CreateJoin ), (BYTE*)&clGameMsgResponse_Party_CreateJoin );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

	// 파장꺼
	cltGameMsgResponse_Party_CreateJoin clSendMsgInfo_toLeader( siPartyIndex, pclPartyCreateJoin->siLeaderPersonID, pclCM->GetIDFromCharUnique(LeaderID), pclCM->CR[LeaderID]->pclCI->clIP.GetLevel(), strLeaderCharName, &clLeaderCharPos , FromPersonID, pclCM->GetIDFromCharUnique(id), pclCM->CR[id]->pclCI->clIP.GetLevel(), strMemberCharName,&clMemberCharPos, pszPartyName, bIsPartyQuestParty, siCategory, siMission );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PARTY_CREATEJOIN, sizeof( clSendMsgInfo_toLeader ), (BYTE*)&clSendMsgInfo_toLeader );
	pclCM->CR[LeaderID]->SendNetMsg((sPacketHeader*)&clMsg2 );

	WRITE_MEMLOG( 150, 3, 0, 0 );

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 파티 시작 쓰기. 
		// Param1 == 파티 번호. 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_CREATEJOIN, 
			0, (cltCharServer*)(pclCM->CR[id]), NULL, pclPartyCreateJoin->siLeaderPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			siPartyIndex, 0, 0, 0, 0, NULL, NULL);
	}
	return;
}

// 파티 초대
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_INVITE( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_Party_Invite *pclPartyInvite = (cltGameMsgRequest_Party_Invite *) pclMsg->cData;

	if ( pclPartyInvite->siInvitedCharUnique < 1 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 파티번호의 파티장이 맞는지 확인해봄
	if ( ! pclPartyMgr->IsPartyLeader( FromPersonID, pclPartyInvite->siPartyIndex ) )
	{
		SendServerResponseMsg(0, SRVAL_PARTYINVITE_NOTLEADER,  0, 0, id);
		return;
	}

	// 파티에 유저를 더 추가할수 있는지 확인
	if ( ! pclPartyMgr->IsEmptyUserListExist( pclPartyInvite->siPartyIndex ) )
	{
		SendServerResponseMsg(0, SRVAL_PARTYINVITE_FULLPARTY,  0, 0, id);
		return;
	}

	SI32 toid = pclCM->GetIDFromCharUnique( pclPartyInvite->siInvitedCharUnique );
	if(pclCM->IsValidID(toid) == false) return;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if(ToPersonID < 1) return;

	// 파티에 참가중이면, 파티 초대 안됨.
	if ( pclCM->CR[ toid ]->siPartyIndex > 0 )
	{
		SendServerResponseMsg(0, SRVAL_PARTYINVITE_ALREADYPARTY,  0, 0, id);
		return;
	}

	// 파티 초대 거부 상태
	cltCharServer *pclChar = ( cltCharServer * )pclCM->CR[toid];

	if ( pclChar->bPartyInviteReject )
	{
		SendServerResponseMsg( 0, SRVAL_PARTYINVITE_REJECT,  0, 0, id );
		return;
	}

	TCHAR *strCharName = (TCHAR*)pclCM->CR[id]->pclCI->clBI.GetName();

	if ( strCharName == NULL )
	{
		return;
	}

	// 파티 대상에게 파티에 참여할것인지를 물음
	cltGameMsgResponse_Party_Invite clGameMsgResponse_Party_Invite( strCharName, FromPersonID, pclPartyInvite->siPartyIndex );
	cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_INVITE, sizeof( clGameMsgResponse_Party_Invite ), (BYTE*)&clGameMsgResponse_Party_Invite );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg );

	return;
}

// 파티 초대 거절
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_INVITEREJECT( cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->GetPersonID() < 1 )
	{
		return;
	}

	cltGameMsgRequest_Party_InviteReject *pclPartyInviteReject = (cltGameMsgRequest_Party_InviteReject *) pclMsg->cData;

	if ( pclPartyInviteReject->siPartyLeaderPersonID < 1 )
	{
		return;
	}

	SI32 LeaderID = pclCM->GetIDFromPersonID( pclPartyInviteReject->siPartyLeaderPersonID );
	if ( LeaderID && pclCM->IsValidID( LeaderID ) == TRUE )
	{
		TCHAR * RejectUserName = (TCHAR*)pclCM->CR[id]->GetName();

		if ( RejectUserName == NULL || _tcslen( RejectUserName ) < 1 )
		{
			return;
		}

		cltGameMsgResponse_Party_InviteReject clGameMsgResponse_Party_InviteReject( RejectUserName );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_INVITEREJECT, sizeof( clGameMsgResponse_Party_InviteReject ), (BYTE*)&clGameMsgResponse_Party_InviteReject );
		pclCM->CR[LeaderID]->SendNetMsg((sPacketHeader*)&clMsg );
	}

	return;
}

// [파티매칭]
// 파티 가입을 원하는 유저가 파티 가입 신청을 했다. (유저 -> 서버)
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_WANTJOIN( cltMsg* pclMsg, SI32 id )
{
	if ( false == pclCM->IsValidID( id ) ) return;
	if ( NULL == pclMsg ) return;

	cltGameMsgRequest_Party_WantJoin* pJoinInfo = (cltGameMsgRequest_Party_WantJoin*)pclMsg->cData;
	if ( NULL == pJoinInfo ) return;

	if ( false == m_pServer_PartyMatchingMgr->IsExist( pJoinInfo->siLeaderUnique ) ) return;

	SI32 siLeaderID = pclCM->GetIDFromCharUnique( pJoinInfo->siLeaderUnique );
	cltCharServer* pLeaderChar = pclCM->GetCharServer( siLeaderID );
	if ( NULL == pLeaderChar ) return;

	if ( 0 < pclCM->CR[id]->siPartyIndex )
	{
		SI16 siExceptionFlags = 0;
		siExceptionFlags = (PARTYMATCHING_ALREADY_PARTYMEMBER);

		cltGameResponse_PartyMatchingException clMsgInfo( siExceptionFlags );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTYMATCHINGEXCEPTION, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
		pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

		return;
	}

	SI32 siMemberUnique = pclCM->CR[id]->GetCharUnique();

	cltGameMsgResponse_Party_WantJoin clMsgInfo( pJoinInfo->siLeaderUnique, siMemberUnique, (TCHAR*)pclCM->CR[id]->GetName(), 0 );
	cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_WANTJOIN, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
	pclCM->CR[siLeaderID]->SendNetMsg( (sPacketHeader*)&clMsg );
}

// [파티매칭]
// 파장이 파티 가입을 허락 한다고 서버에 알려줌 (파장 -> 서버)
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_WANTJOINRESULT( cltMsg* pclMsg, SI32 id )
{
	WRITE_MEMLOG( 150, 1, 0, 0 );

	cltGameMsgRequest_Party_WantJoinResult *pJoinInfo = (cltGameMsgRequest_Party_WantJoinResult *) pclMsg->cData;
	if ( NULL == pJoinInfo ) return;

	//---------------------------------------------
	// [기형] 배틀로얄 경기장이라면 
	//---------------------------------------------
	if(NULL != m_pclBattleRoyalMgr)
	{
		cltCharServer* pclchar = pclCM->GetCharServer(id);
		if ( NULL == pclchar) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }

		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclchar);
		if( NULL != pclBattleRoyal)
		{
			if( TRUE == pclBattleRoyal->IsExistStadium(pclchar) ) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }
		}
	}

	cltCharServer* pCharServer = pclCM->GetCharServer( id );
	if ( NULL == pCharServer ) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }

	SI32 siLeaderPersonID = 0;
	SI32 siMemberPersonID = 0;
	SI32 siLeaderID = 0;
	SI32 siMemberID = 0;
	SI32 siLeaderUnique = 0;
	SI32 siMemberUnique = 0;
	SI32 siPartyIndex = 0;
	CPartyObj* pPartyObj = NULL;
	SI16 siJoinedIndex = -1;
	TCHAR* pszPartyName = NULL;

	siLeaderPersonID = pCharServer->pclCI->GetPersonID();
	siMemberPersonID = pclCM->CR[ pclCM->GetIDFromCharUnique(pJoinInfo->siMemberUnique) ]->pclCI->GetPersonID();
	siLeaderID = pclCM->GetIDFromPersonID( siLeaderPersonID );
	siMemberID = pclCM->GetIDFromPersonID( siMemberPersonID );
	siLeaderUnique = pclCM->CR[siLeaderID]->GetCharUnique();
	siMemberUnique = pclCM->CR[siMemberID]->GetCharUnique();

	if ( 1 > siLeaderPersonID ) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }
	if ( 1 > siMemberPersonID ) return;
	if ( false == pclCM->IsValidID(siLeaderID) ) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }
	if ( false == pclCM->IsValidID(siMemberID) ) return;

	if ( 0 < pclCM->CR[siMemberID]->siPartyIndex )
	{
		SI16 siExceptionFlags = 0;
		siExceptionFlags = (PARTYMATCHING_ALREADY_PARTYMEMBER);

		cltGameResponse_PartyMatchingException clMsgInfo( siExceptionFlags );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTYMATCHINGEXCEPTION, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
		pclCM->CR[siMemberID]->SendNetMsg( (sPacketHeader*)&clMsg );

		return;
	}

	//////////////////////////////////////////////////////////////////////////

	// 파티 가입을 거절당한 경우
	if ( false == pJoinInfo->bResult )
	{
		//bool result, SI32 leaderunique, SI32 memberunique, SI32 partyindex, TCHAR* pszpartyname
		cltGameMsgResponse_Party_WantJoinResult clMsgInfo( false, siLeaderUnique, siMemberUnique, siPartyIndex, NULL );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_WANTJOINRESULT, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
		pclCM->CR[siMemberID]->SendNetMsg( (sPacketHeader*)&clMsg );

		return;
	}

	//////////////////////////////////////////////////////////////////////////


	// 파티가 없는 상태에서 생성후 조인이다.
	if ( 0 >= pclCM->CR[siLeaderID]->siPartyIndex )						// 파장이 파티에 가입되어 있는 상태가 아니라면 파티를 생성후 나머지 처리
	{
		siPartyIndex = pclPartyMgr->CreateParty( siLeaderPersonID, siLeaderUnique, pclCM->CR[siLeaderID]->pclCI->clIP.GetLevel() );
		if ( 0 > siPartyIndex ) return;	// 파티 생성 갯수를 초과해서 0보다 작은 값이 리턴됐다면 리턴

		pclCM->CR[siLeaderID]->siPartyIndex = siPartyIndex;

		//////////////////////////////////////////////////////////////////////////

		pPartyObj = pclPartyMgr->GetPartyObj( siPartyIndex );
		if ( NULL == pPartyObj ) return;

		if ( false == pPartyObj->IsPartyLeader(siLeaderPersonID) ) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }

		siJoinedIndex = pPartyObj->JoinParty( siMemberPersonID, siMemberUnique, pclCM->CR[siMemberID]->pclCI->clIP.GetLevel() );
		if ( siJoinedIndex < 0 )
		{
			pclLog->FilePrint( TEXT("PartyError.txt"), TEXT("Joined Index = %d"), siJoinedIndex );
			return;
		}

		pclCM->CR[siMemberID]->siPartyIndex = siPartyIndex;

		pszPartyName = m_pServer_PartyMatchingMgr->GetPartyName( siLeaderUnique );
		pPartyObj->SetPartyName( pszPartyName );

		TCHAR *strLeaderCharName = (TCHAR*)pclCM->CR[siLeaderID]->pclCI->clBI.GetName();
		if ( strLeaderCharName == NULL ) return;

		TCHAR *strMemberCharName = (TCHAR*)pclCM->CR[siMemberID]->pclCI->clBI.GetName();
		if ( strMemberCharName == NULL ) return;

		// 파티원으로 다른 파티에 가입하는 사람이 만든 파티매칭이 있다면 지워준다.
		m_pServer_PartyMatchingMgr->Remove_PartyMatching( siMemberUnique );

		// 파티퀘 정보 얻어옴
		bool bIsPartyQuestParty = false;
		SI32 siCategory = -1;
		SI32 siMission = -1;

		bIsPartyQuestParty = m_pServer_PartyMatchingMgr->IsExist( siLeaderUnique );
		if ( true == bIsPartyQuestParty )
		{
			siCategory = m_pServer_PartyMatchingMgr->Get_Category( siLeaderUnique );
			siMission = m_pServer_PartyMatchingMgr->Get_Mission( siLeaderUnique );
		}

		//리더의 위치
		cltCharPos clLeaderCharPos(pclCM->CR[siLeaderID]->GetMapIndex(), pclCM->CR[siLeaderID]->GetX(),pclCM->CR[siLeaderID]->GetY());
		//파티원의 위치
		cltCharPos clMemberCharPos(pclCM->CR[siMemberID]->GetMapIndex(), pclCM->CR[siMemberID]->GetX(),pclCM->CR[siMemberID]->GetY());

		cltGameMsgResponse_Party_CreateJoin clGameMsgResponse_Party_CreateJoin( siPartyIndex, siLeaderPersonID, siLeaderID, pclCM->CR[siLeaderID]->pclCI->clIP.GetLevel(), strLeaderCharName, &clLeaderCharPos , siMemberPersonID, siMemberID, pclCM->CR[siMemberID]->pclCI->clIP.GetLevel(), strMemberCharName, &clMemberCharPos, pszPartyName );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_CREATEJOIN, sizeof( clGameMsgResponse_Party_CreateJoin ), (BYTE*)&clGameMsgResponse_Party_CreateJoin );
		pclCM->CR[siMemberID]->SendNetMsg((sPacketHeader*)&clMsg );

		cltGameMsgResponse_Party_CreateJoin clSendMsgInfo_toLeader
			( 
			siPartyIndex, siLeaderPersonID, siLeaderID, pclCM->CR[siLeaderID]->pclCI->clIP.GetLevel(), strLeaderCharName, &clLeaderCharPos,
			siMemberPersonID, siMemberID, pclCM->CR[siMemberID]->pclCI->clIP.GetLevel(), strMemberCharName, &clMemberCharPos,
			pszPartyName, bIsPartyQuestParty, siCategory, siMission
			);
		cltMsg clMsg2( GAMEMSG_RESPONSE_PARTY_CREATEJOIN, sizeof( clSendMsgInfo_toLeader ), (BYTE*)&clSendMsgInfo_toLeader );
		pclCM->CR[siLeaderID]->SendNetMsg((sPacketHeader*)&clMsg2 );

		WRITE_MEMLOG( 150, 3, 0, 0 );

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 파티 시작 쓰기. 
			// Param1 == 파티 번호. 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_CREATEJOIN, 
				0, (cltCharServer*)(pclCM->CR[siLeaderID]), NULL, siLeaderPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				siPartyIndex, 0, 0, 0, 0, NULL, NULL);
		}

		return;
	}
	// 파티가 있던 상태에서 유저의 조인이다.
	else
	{
		siPartyIndex = pclCM->CR[siLeaderID]->siPartyIndex;

		//////////////////////////////////////////////////////////////////////////

		pPartyObj = pclPartyMgr->GetPartyObj( siPartyIndex );
		if ( NULL == pPartyObj ) return;

		if ( false == pPartyObj->IsPartyLeader(siLeaderPersonID) ) { m_pServer_PartyMatchingMgr->Remove_PartyMatching(pJoinInfo->siLeaderUnique); return; }

		siJoinedIndex = pPartyObj->JoinParty( siMemberPersonID, siMemberUnique, pclCM->CR[siMemberID]->pclCI->clIP.GetLevel() );
		if ( siJoinedIndex < 0 )
		{
			pclLog->FilePrint( TEXT("PartyError.txt"), TEXT("Joined Index = %d"), siJoinedIndex );
			return;
		}

		pclCM->CR[siMemberID]->siPartyIndex = siPartyIndex;

		pszPartyName = m_pServer_PartyMatchingMgr->GetPartyName( siLeaderUnique );
		pPartyObj->SetPartyName( pszPartyName );

		TCHAR *strLeaderCharName = (TCHAR*)pclCM->CR[siLeaderID]->pclCI->clBI.GetName();
		if ( strLeaderCharName == NULL ) return;

		TCHAR *strMemberCharName = (TCHAR*)pclCM->CR[siMemberID]->pclCI->clBI.GetName();
		if ( strMemberCharName == NULL ) return;

		// 파티원으로 다른 파티에 가입하는 사람이 만든 파티매칭이 있다면 지워준다.
		m_pServer_PartyMatchingMgr->Remove_PartyMatching( siMemberUnique );


		// 파티에 참가 되었음을 알려줌
		cltGameMsgResponse_Party_Join clGameMsgResponse_Party_Join( siPartyIndex, siJoinedIndex, pPartyObj->GetItemGetPattern(), pszPartyName );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_JOIN, sizeof( clGameMsgResponse_Party_Join ), (BYTE*)&clGameMsgResponse_Party_Join );
		pclCM->CR[siMemberID]->SendNetMsg((sPacketHeader*)&clMsg );

		// 파티원에게 위치를 알려주기 위해 메시지를 만들어두다.
		cltCharPos clCharPos( pclCM->CR[siMemberID]->GetMapIndex(), pclCM->CR[siMemberID]->GetX(), pclCM->CR[siMemberID]->GetY() );

		cltGameMsgResponse_Party_AddUser clGameMsgResponse_Party_AddUser( siJoinedIndex, pclCM->CR[siMemberID]->GetCharUnique(), false, strMemberCharName, pclCM->CR[siMemberID]->pclCI->clIP.GetLevel(), &clCharPos );
		cltMsg clMsgAddUser( GAMEMSG_RESPONSE_PARTY_ADDUSER, sizeof( clGameMsgResponse_Party_AddUser ), (BYTE*)&clGameMsgResponse_Party_AddUser );

		for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
		{
			if ( ! pPartyObj->GetPartyUser( i )->bPartyStatus )
			{
				continue;
			}

			SI32 CharUnique = pPartyObj->GetPartyUser( i )->siCharUnique;

			if ( CharUnique < 1 )
			{
				continue;
			}

			SI32 CharID = pclCM->GetIDFromCharUnique( CharUnique );

			if(pclCM->IsValidID(CharID) == false) continue;

			// 나머지 파티원들에게 파티원이 추가되었음을 알림
			if ( pclCM->IsValidID( CharID ) == TRUE )
			{
				if ( pclCM->CR[ CharID ]->pclCI->GetPersonID() != pPartyObj->GetPartyUser( i )->siPersonID )
				{
					continue;
				}

				// 자기 자신한테는 안 보내줌
				if ( CharID == siMemberID )
				{
					continue;
				}

				pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsgAddUser );
			}
		}

		WRITE_MEMLOG( 150, 6, 0, 0 );
		return;
	}	
}

// 파티 참여
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_JOIN( cltMsg* pclMsg, SI32 id )
{
	WRITE_MEMLOG( 150, 5, 0, 0 );

	if(pclCM->IsValidID(id) == false) return;

	cltGameMsgRequest_Party_Join *pclPartyJoin = ( cltGameMsgRequest_Party_Join * ) pclMsg->cData;

	if ( pclCM->CR[id]->GetCharUnique() < 1 )
	{
		return;
	}

	// 파티에 참가중이면, 파티 참여 안됨.
	if ( pclCM->CR[id]->siPartyIndex > 0 )
	{
		return;
	}

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	CPartyObj *pTemp = pclPartyMgr->GetPartyObj( pclPartyJoin->siPartyIndex );

	// 인덱스 번호의 파티가 존재한다.
	if ( pTemp == NULL )
	{
		return;
	}

	// 파티번호의 파티장이 맞는지 확인해봄
	if ( ! pTemp->IsPartyLeader( pclPartyJoin->siPartyLeaderPersonID ) )
	{
		return;
	}

	SI16 JoinedIndex = pTemp->JoinParty( FromPersonID, pclCM->CR[id]->GetCharUnique(), pclCM->CR[id]->pclCI->clIP.GetLevel() );

	// 파티 유저 인원이 다 찼다.
	if ( JoinedIndex < 0 )
	{
		return;
	}

	TCHAR *strName = (TCHAR*)pclCM->CR[id]->GetName();
	if ( strName == NULL )
	{
		return;
	}

	pclCM->CR[id]->siPartyIndex  = pclPartyJoin->siPartyIndex;

	// 파티원으로 다른 파티에 가입하는 사람이 만든 파티매칭이 있다면 지워준다.
	m_pServer_PartyMatchingMgr->Remove_PartyMatching( pclCM->CR[id]->GetCharUnique() );


	cltServer* pServer = (cltServer*) pclClient;
	if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
	{
		// 파티퀘스트중 파티에 가입되었으므로 해당파티는 파티퀘스트를 중지 합니다.
		if(pServer->pclPartyMgr->GetPartyObj(pclCM->CR[id]->siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
			pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(pclCM->CR[id]->siPartyIndex, 0, PARTYQUEST_MESSAGE_FAILED_ADDED_USER);
	}

	TCHAR* pszPartyName = NULL;
	pszPartyName = m_pServer_PartyMatchingMgr->GetPartyName( pclCM->CR[pclCM->GetIDFromPersonID(pclPartyJoin->siPartyLeaderPersonID)]->GetCharUnique() );
	if ( NULL != pszPartyName ) pTemp->SetPartyName( pszPartyName );

	// 파티에 참가 되었음을 알려줌
	cltGameMsgResponse_Party_Join clGameMsgResponse_Party_Join( pclPartyJoin->siPartyIndex, JoinedIndex, pTemp->GetItemGetPattern(), pszPartyName );
	cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_JOIN, sizeof( clGameMsgResponse_Party_Join ), (BYTE*)&clGameMsgResponse_Party_Join );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

	pclCM->CR[id]->siPartyIndex = pclPartyJoin->siPartyIndex;	// 캐릭터 정보 업데이트


	// 파티원에게 위치를 알려주기 위해 메시지를 만들어두다.
	cltCharPos clCharPos( pclCM->CR[id]->GetMapIndex(),pclCM->CR[id]->GetX(),pclCM->CR[id]->GetY() );

	cltGameMsgResponse_Party_AddUser clGameMsgResponse_Party_AddUser( JoinedIndex, pclCM->CR[id]->GetCharUnique(), false, strName, pclCM->CR[id]->pclCI->clIP.GetLevel(),&clCharPos );
	cltMsg clMsgAddUser( GAMEMSG_RESPONSE_PARTY_ADDUSER, sizeof( clGameMsgResponse_Party_AddUser ), (BYTE*)&clGameMsgResponse_Party_AddUser );

	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( ! pTemp->GetPartyUser( i )->bPartyStatus )
		{
			continue;
		}

		SI32 CharUnique = pTemp->GetPartyUser( i )->siCharUnique;

		if ( CharUnique < 1 )
		{
			continue;
		}

		SI32 CharID = pclCM->GetIDFromCharUnique( CharUnique );

		if(pclCM->IsValidID(CharID) == false) continue;

		// 나머지 파티원들에게 파티원이 추가되었음을 알림
		if ( pclCM->IsValidID( CharID ) == TRUE )
		{
			if ( pclCM->CR[ CharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
			{
				continue;
			}

			// 자기 자신한테는 안 보내줌
			if ( CharID == id )
			{
				continue;
			}

			pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsgAddUser );
		}
	}

	WRITE_MEMLOG( 150, 6, 0, 0 );
	return;
}

// 파티 탈퇴
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_LEAVE( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_Party_Leave *pclPartyLeave = ( cltGameMsgRequest_Party_Leave* ) pclMsg->cData;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();

	if( FromPersonID < 1 )
		return ;

	TCHAR *strName = (TCHAR*)pclCM->CR[id]->GetName();
	if ( strName == NULL )
	{
		return;
	}

	CPartyObj *pTemp = pclPartyMgr->GetPartyObj( pclPartyLeave->siPartyIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	SI16 MyPartyUserListIndex = pTemp->GetPartyUserListIndex( pclCM->CR[id]->GetCharUnique() );

	if ( MyPartyUserListIndex < 0 )			// 내가 파티원 리스트에 없음.
	{
		return;
	}

	// 나가려는 사람이 파장이었으면 파티매칭 리스트에 해당 유저가 만든 파티매칭이 있는지 검사해서 지워준다.
	if( true == pTemp->IsPartyLeader(FromPersonID) )
	{
		if( NULL != pclClient->m_pServer_PartyMatchingMgr)
		{
			pclClient->m_pServer_PartyMatchingMgr->Remove_PartyMatching( pclCM->CR[id]->GetCharUnique() );
		}
	}

	// [영훈] 해적선 : 내가 해적선안에 있으면 파티 해제 불가
	SI32 siMapIndex		= pclCM->CR[id]->GetMapIndex();

	if ( pclMapManager->IsPartyMap( siMapIndex ) == true )
	{
		return;
	}

	SI32 siRet = 0;
	SI16 ret = pTemp->LeaveParty( FromPersonID, &siRet );

	if ( ret < 0 ) 
	{
		return;
	}

	pclCM->CR[id]->siPartyIndex = 0;			// 캐릭터 정보 업데이트

	switch( ret )
	{
	case PARTY_DELETE:
		{
			// 유저가 나갔음을 알려줌
			/*
			cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( siRet, pclCM->CR[id]->GetCharUnique() );
			cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
			pclCM->CR[id]->SendNetMsg( (BYTE*)&clMsg1 );
			*/

				if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
				{
					if(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
					{
						if(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->GetPartyUserNum() < m_pPartyQuestMgr->GetPartyQuestCategoryData(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->m_PartyQuestInfo.m_siCategoryIndex)->m_siMinUser)
						{
							cltServer* pServer = (cltServer*) pclClient;
							pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(pclPartyLeave->siPartyIndex, id, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
						}	
					}
				}	


				for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
				{
					if ( pTemp->GetPartyUser( i )->bPartyStatus )
					{
						SI32 ListUserCharUnique = pTemp->GetPartyUser( i )->siCharUnique;

						if ( ListUserCharUnique < 1 )
						{
							continue;
						}

						SI32 ListUserCharID = pclCM->GetIDFromCharUnique( ListUserCharUnique );

						if ( ListUserCharID < 1 )
						{
							continue;
						}

						if ( pclCM->IsValidID( ListUserCharID ) == TRUE )
						{
							if ( pclCM->CR[ ListUserCharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
							{
								continue;
							}
							cltGameMsgResponse_Party_Delete clGameMsgResponse_Party_Delete( pclPartyLeave->siPartyIndex );
							cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_DELETE, sizeof( clGameMsgResponse_Party_Delete ), (BYTE*)&clGameMsgResponse_Party_Delete );
							pclCM->CR[ ListUserCharID ]->SendNetMsg((sPacketHeader*)&clMsg );

							if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
							{
								// [로그추가 : 황진성 2007. 9. 3] // 파티 종료 쓰기.
								// param1 == 파티번호.
								SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_END, 
									ret, (cltCharServer*)(pclCM->CR[ListUserCharID]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
									pclCM->CR[ ListUserCharID ]->siPartyIndex, 0, 0, 0, 0, NULL, NULL);
							}

							pclCM->CR[ ListUserCharID ]->siPartyIndex = 0;

							// [영훈] 해적선 : 해적선 에서 파티가 해제될경우 엔펠리스로 이동한다
							pclPartyMgr->InPartyDungeonDeleteParty( ListUserCharID );

							/*
							if ( pclCM->CR[ListUserCharID]->GetMapIndex() >= MAPINDEX_DAEMADO )
							{
								// 안전지대로 워프 시킨다. 
								OrderStop(id, BY_COMPUTER);

								cltWarpInfo clWarpInfo(1, MAPINDEX_DAEMADOGATE, false, false, 10, 30, -1, -1);
								((cltCharServer*)pclCM->CR[ListUserCharID])->SetWarpInfo(&clWarpInfo, 1054);
								// 실제로 워프 시킨다. 
								((cltCharServer*)pclCM->CR[ListUserCharID])->Warp();
							}
							*/
						}
					}
				}



				// 파티 리스트에서 없애기
				pclPartyMgr->DeleteParty( pTemp );


				return;					// 파티 삭제 되었으므로... 할것 없음.
		}
		break;

	case PARTY_CHANGE_LEADER:		// 나간 사람이 파티장이 었으므로... 파티장 변경을 알려줘야 함
		{
			SI32 NewLeaderUserListIndex = siRet;
			SI32 NewLeaderPersonID = pTemp->GetPartyUser( 0 )->siPersonID;
			SI32 NewLeaderCharUnique = pTemp->GetPartyUser( 0 )->siCharUnique;

			// 이 안으로 올수 없지만... 혹시나 해서 예외처리
			if ( NewLeaderUserListIndex < 0 )
			{
				return;
			}

			for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
			{
				if ( pTemp->GetPartyUser( i )->bPartyStatus )
				{
					SI32 CharUnique = pTemp->GetPartyUser( i )->siCharUnique;

					if ( CharUnique < 1 ) 
					{
						continue;
					}

					SI32 CharID = pclCM->GetIDFromCharUnique( CharUnique );

					if ( CharID < 1 )
					{
						continue;
					}

					if ( pclCM->IsValidID( CharID ) == TRUE )
					{
						if ( pclCM->CR[ CharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
						{
							continue;
						}

						// 유저가 나갔음을 알려줌
						cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( MyPartyUserListIndex, pclCM->CR[id]->GetCharUnique() );
						cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
						pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsg1 );

						// 방장이 바뀌었음을 알려줌
						cltGameMsgResponse_Party_ChangeLeader clGameMsgResponse_Party_ChangeLeader( NewLeaderUserListIndex, NewLeaderCharUnique );
						cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_CHANGELEADER, sizeof( clGameMsgResponse_Party_ChangeLeader ), (BYTE*)&clGameMsgResponse_Party_ChangeLeader );
						pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsg );

					}
				}
			}

			// 파티장이 나간경우
			if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
			{
				if(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
				{
					cltServer* pServer = (cltServer*) pclClient;
					pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(pclPartyLeave->siPartyIndex, id, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
				}
			}	

			// 자기 자신이 파티에서 나갔음을 알려줌
			cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( MyPartyUserListIndex, pclCM->CR[id]->GetCharUnique() );
			cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1 );

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 9. 3] // 파티 종료 쓰기. 
				// param1 == 파티 번호.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_END, 
					ret, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					MyPartyUserListIndex, 0, 0, 0, 0, NULL, NULL);
			}


			/*
			if ( pclCM->CR[id]->GetMapIndex() >= MAPINDEX_DAEMADO )
			{
				//((cltCharServer*)pclCM->CR[id])->WarpToSafeZone() ;
				// 안전지대로 워프 시킨다. 
				OrderStop(id, BY_COMPUTER);

				cltWarpInfo clWarpInfo(1, MAPINDEX_DAEMADOGATE, false, false, 10, 30, -1, -1);
				((cltCharServer*)pclCM->CR[id])->SetWarpInfo(&clWarpInfo, 1055);
				// 실제로 워프 시킨다. 
				((cltCharServer*)pclCM->CR[id])->Warp();
			}
			*/

				// cltGameMsgResponse_Party_ChangeLeader clGameMsgResponse_Party_ChangeLeader( ret, pclCM->CR[id]->GetCharUnique() );
		}
		break;

	default:						// 파티에 유저가 나갔으므로 알려줘야 함
		{
			for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
			{
				if ( ! pTemp->GetPartyUser( i )->bPartyStatus )
				{
					continue;
				}

				SI32 CharUnique = pTemp->GetPartyUser( i )->siCharUnique;

				if ( CharUnique < 1 )
				{
					continue;
				}

				SI32 CharID = pclCM->GetIDFromCharUnique( CharUnique );

				if ( CharID < 1 )
				{
					continue;
				}

				if ( pclCM->IsValidID( CharID ) == TRUE )
				{
					if ( pclCM->CR[ CharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
					{
						continue;
					}

					cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( siRet, pclCM->CR[id]->GetCharUnique() );
					cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
					pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsg );
				}
			}

			// 파티원이 3명 이하인 경우에는 파티퀘스트가 종료됩니다.
			if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
			{
				if(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
				{
					if(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->GetPartyUserNum() < m_pPartyQuestMgr->GetPartyQuestCategoryData(pclPartyMgr->GetPartyObj(pclPartyLeave->siPartyIndex)->m_PartyQuestInfo.m_siCategoryIndex)->m_siMinUser)
					{
						cltServer* pServer = (cltServer*) pclClient;
						pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(pclPartyLeave->siPartyIndex, id, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
					}	
				}
			}	

			// 자기 자신이 파티에서 나갔음을 알려줌
			cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( MyPartyUserListIndex, pclCM->CR[id]->GetCharUnique() );
			cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1 );

			/*
			if ( pclCM->CR[id]->GetMapIndex() >= MAPINDEX_DAEMADO )
			{
				//((cltCharServer*)pclCM->CR[id])->WarpToSafeZone() ;
				// 안전지대로 워프 시킨다. 
				OrderStop(id, BY_COMPUTER);

				cltWarpInfo clWarpInfo(1, MAPINDEX_DAEMADOGATE, false, false, 10, 30, -1, -1);
				((cltCharServer*)pclCM->CR[id])->SetWarpInfo(&clWarpInfo, 1056);
				// 실제로 워프 시킨다. 
				((cltCharServer*)pclCM->CR[id])->Warp();
			}
			*/

		}
		break;
	}

	return;
}

// 파티 강제퇴장  
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_FORCELEAVE( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_Party_ForceLeave *pclForceLeave = (cltGameMsgRequest_Party_ForceLeave *) pclMsg->cData;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;


	SI32 toid = pclCM->GetIDFromCharUnique( pclForceLeave->siForceLeaveCharUnique );
	if ( pclCM->IsValidID(toid) == false ) return;

	SI32 ToPersonID = pclCM->CR[toid]->pclCI->GetPersonID();
	if( ToPersonID < 1) return ;

	// 자기 자신 강퇴 안됨
	if ( ToPersonID == FromPersonID ) 
	{
		return;
	}

	// 같은 파티가 아니라면, 강퇴하면 안되 므로...
	if ( pclCM->CR[ toid ]->siPartyIndex != pclForceLeave->siPartyIndex )
	{
		return;
	}

	CPartyObj *pTemp = pclPartyMgr->GetPartyObj( pclForceLeave->siPartyIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 파티원인지 아닌지 체크
	if( pTemp->GetSamePersonIDIndex( FromPersonID ) < 0 || pTemp->GetSamePersonIDIndex( ToPersonID ) < 0 )
	{
		return;
	}

	// 파티 리더인지 확인(강퇴는 리더만 가능하므로.. )
	if ( ! pTemp->IsPartyLeader( FromPersonID ) )
	{
		return;
	}

	// [영훈] 해적선 : 파티원이 해적선에 있으면 파티 해제 불가
	SI32 siMapIndex		= pclCM->CR[toid]->GetMapIndex();

	if ( pclMapManager->IsPartyMap( siMapIndex )  == true )
	{
		return;
	}

	// 캐릭터 정보 업데이트
	SI32 siPartyIndex = pclCM->CR[toid]->siPartyIndex;
	pclCM->CR[toid]->siPartyIndex = 0;

	SI32 ForcedLeaveUserIndex = pTemp->ForceLeaveParty( ToPersonID );

	if ( ForcedLeaveUserIndex < 0 )
	{
		return;
	}

	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( ! pTemp->GetPartyUser( i )->bPartyStatus )
		{
			continue;
		}

		SI32 CharUnique = pTemp->GetPartyUser( i )->siCharUnique;

		if ( CharUnique < 1 )
		{
			continue;
		}

		SI32 CharID = pclCM->GetIDFromCharUnique( CharUnique );

		if ( CharID < 1 )
		{
			continue;
		}

		if ( pclCM->IsValidID( CharID ) == TRUE )
		{
			if ( pclCM->CR[ CharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
			{
				continue;
			}

			cltGameMsgResponse_Party_ForceDelUser clGameMsgResponse_Party_ForceDelUser( ForcedLeaveUserIndex, pclCM->CR[toid]->GetCharUnique() );
			cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_FORCEDELUSER, sizeof( clGameMsgResponse_Party_ForceDelUser ), (BYTE*)&clGameMsgResponse_Party_ForceDelUser );
			pclCM->CR[CharID]->SendNetMsg((sPacketHeader*)&clMsg );
		}
	}

	// 강퇴 되었음을 강퇴된 유저에게 알려줌
	cltGameMsgResponse_Party_ForceDelUser clGameMsgResponse_Party_ForceDelUser( ForcedLeaveUserIndex, pclCM->CR[toid]->GetCharUnique() );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_FORCEDELUSER, sizeof( clGameMsgResponse_Party_ForceDelUser ), (BYTE*)&clGameMsgResponse_Party_ForceDelUser );
	pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg1 );

	// 파티원이 퀘스트요구 인원 이하인 경우에는 파티퀘스트가 종료됩니다.
	if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
	{
		if(pclPartyMgr->GetPartyObj(pclForceLeave->siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
		{
			if(pTemp->GetPartyUserNum() < m_pPartyQuestMgr->GetPartyQuestCategoryData(pTemp->m_PartyQuestInfo.m_siCategoryIndex)->m_siMinUser)
			{
				cltServer* pServer = (cltServer*) pclClient;
				pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(siPartyIndex, toid, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
			}
		}
	}	

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 9. 3] // 파티 강퇴 쓰기.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_FORCELEAVE, 
			0, (cltCharServer*)(pclCM->CR[toid]), NULL, FromPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			ForcedLeaveUserIndex, 0, 0, 0, 0, NULL, NULL);
	}

	if ( pTemp->GetPartyUserNum() < 2 )
	{
		cltGameMsgResponse_Party_Delete clGameMsgResponse_Party_Delete( pclForceLeave->siPartyIndex );
		cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_DELETE, sizeof( clGameMsgResponse_Party_Delete ), (BYTE*)&clGameMsgResponse_Party_Delete );
		pclCM->CR[ id ]->SendNetMsg((sPacketHeader*)&clMsg );

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 9] // 파티 종료 쓰기.
			// param1 == 파티번호.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_END, 
				0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				pclCM->CR[id]->siPartyIndex, 0, 0, 0, 0, NULL, NULL);
		}

		pclCM->CR[ id ]->siPartyIndex = 0;

		// 파티 리스트에서 없애기
		pclPartyMgr->DeleteParty( pTemp );
	}

	return;
}

// 파티 전체 유저 리스트 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_ALLUSERLIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_Party_AllUserList *pclReqUserList = ( cltGameMsgRequest_Party_AllUserList* ) pclMsg->cData;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	CPartyObj *pTemp = pclPartyMgr->GetPartyObj( pclReqUserList->siPartyIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 요청한 사람이 파티원인지 아닌지 체크
	if( pTemp->GetSamePersonIDIndex( FromPersonID ) < 0 )
	{
		return;
	}

	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( ! pTemp->GetPartyUser( i )->bPartyStatus )
		{
			continue;
		}

		SI32 CharUnique = pTemp->GetPartyUser( i )->siCharUnique;

		if ( CharUnique < 1 )
		{
			continue;
		}

		SI32 CharID = pclCM->GetIDFromCharUnique( CharUnique );

		if ( CharID < 1 )
		{
			continue;
		}

		if ( pclCM->IsValidID( CharID ) == TRUE )
		{
			if ( pclCM->CR[ CharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
			{
				continue;
			}

			TCHAR *strName = (TCHAR*)pclCM->CR[ CharID ]->GetName();
			if ( strName == NULL )
			{
				continue;
			}			
			cltCharPos clCharPos( pclCM->CR[CharID]->GetMapIndex(),pclCM->CR[CharID]->GetX(),pclCM->CR[CharID]->GetY() );
			cltGameMsgResponse_Party_AddUser clGameMsgResponse_Party_AddUser( i, pclCM->CR[CharID]->GetCharUnique(), pTemp->GetPartyUser( i )->bIsPartyLeader, strName, pclCM->CR[CharID]->pclCI->clIP.GetLevel(), &clCharPos );
			cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_ADDUSER, sizeof( clGameMsgResponse_Party_AddUser ), (BYTE*)&clGameMsgResponse_Party_AddUser );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );			
		}
	}	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Party_ChangeItemGetPattern* pclChangeItemGetPattern = (cltGameMsgRequest_Party_ChangeItemGetPattern*)pclMsg->cData;
	// 파티에 가입된 유저가 아니라면 무효
	if(pclCM->CR[id]->siPartyIndex <=0) 	return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;


	CPartyObj *pPartyObj = pclPartyMgr->GetPartyObj( pclCM->CR[id]->siPartyIndex );
	if ( pPartyObj == NULL ) return;

	// 요청한 사람이 파티리더인지 아닌지 체크
	if( pPartyObj->IsPartyLeader( FromPersonID ) == false )	return;

	SI16 siItemGetPattern = pclChangeItemGetPattern->siItemGetPattern;

	pPartyObj->SetItemGetPattern(siItemGetPattern);

	pPartyObj->NotifyItemGetPatternToPartyMember(siItemGetPattern );


	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 파티 설정 쓰기.
		// param1 == 파티 번호, param2 == 패턴.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PARTY, LOGCOMMAND_INDEX_PARTY_CHANGEITEMGETPATTERN, 
			0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			pPartyObj->GetPartyIndex(), siItemGetPattern, 0, 0, 0, NULL, NULL);
	}
	/*
	cltGameMsgResponse_Party_ChangeItemGetPattern clinfo(siItemGetPattern);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_CHANGEITEMGETPATTERN,sizeof(clinfo),(BYTE*)&clinfo);

	for(SI16 i = 0; i < MAX_PARTY_USER_NUM; i++)
	{
		stPartyUser* partyUser =  pPartyObj->GetPartyUser(i);

		// 파티원이면 바뀐 획득 방식을 알려준다.
		if(partyUser != NULL && partyUser->bPartyStatus )		// && partyUser->bIsPartyLeader == false)
		{
			if(pclCM->IsValidID(partyUser->siCharUnique) == false) continue;
			pclCM->CR[ partyUser->siCharUnique ]->SendNetMsg((sPacketHeader*)&clMsg );
		}		
	}    */
}


// [영훈] 해적선으로 입장할 수 있는지 확인하는 함수
void cltServer::DoMsg_GAMEMSG_REQUEST_PIRATESHIP_ENTERFIELD(cltMsg* pclMsg,SI32 id)
{
	if ( pclClient->pclCM->IsAlive( id ) == false )
	{
		return;
	}

	SI32			siCharID	= id;
	cltCharServer*	pclChar		= (cltCharServer*)pclClient->pclCM->CR[ siCharID ];
	if ( pclChar == NULL )
	{
		return;
	}

	cltServer*		pclserver	= (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}

	SI32 siCondition		= MAPINDEX_PIRATESHIP+MAX_INSTANCE_MAP_NUMBER;
	SI32 siCanEnterMapIndex = 0;

	for ( SI32 siMapIndex=MAPINDEX_PIRATESHIP; siMapIndex<siCondition; siMapIndex++ )
	{
		// 인던이 비어 있는지 확인하기 위해
		siCanEnterMapIndex = pclClient->pclMapManager->GetEnterableInstanceMapIndex( siMapIndex, pclChar );

		// 이미 다른 파티가 들어가 있다면
		if ( siCanEnterMapIndex < 0 )
		{
			continue;
		}
		else if ( siCanEnterMapIndex == 0 )
		{
			// 이곳은 파티를 맺고 들어와야 하는 곳이다
			pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_INSTANTDUNGEON_NO_PARTY, 0, 0, siCharID );
			return;
		}
		else
		{
			// 비어있는 맵이지만 내가 입장 가능한지 확인
			if ( pclClient->pclMapManager->EnterInstanceMap( siCanEnterMapIndex, pclChar ) == true )
			{
				SI32		siWarpX = PIRATESHIP_START_POSITION_X + (rand() % 5);
				SI32		siWarpY = PIRATESHIP_START_POSITION_Y + (rand() % 5);
				cltWarpInfo clWarpPirateShip(1, siCanEnterMapIndex, false, false, siWarpX, siWarpY, -1, -1);

				pclChar->SetWarpInfo( &clWarpPirateShip, 1024 );
				pclChar->Warp();
				return;
			}
			else
			{
				// 더이상 입장할 파티 맵이 없다
				pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_INSTANTDUNGEON_OVER_PARTY_MAP, 0, 0, siCharID );
				return;
			}
		}
	}

	// 더이상 입장할 파티 맵이 없다
	pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_INSTANTDUNGEON_OVER_PARTY_MAP, 0, 0, siCharID );

}


//KHY - 1113 - 크리스마스 이벤트 - 사냥터 분리 입장 시키는 함수.
void cltServer::DoMsg_GAMEMSG_REQUEST_CHRISTMASEVET_ENTERFIELD(cltMsg* pclMsg,SI32 id)
{
	if ( pclClient->pclCM->IsAlive( id ) == false )
	{
		return;
	}

	SI32			siCharID	= id;
	cltCharServer*	pclChar		= (cltCharServer*)pclClient->pclCM->CR[ siCharID ];
	if ( pclChar == NULL )
	{
		return;
	}

	cltServer*		pclserver	= (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}

	// 캐릭터 레벨을 찾는다.
	SI32 charlevel  = pclChar->pclCI->clIP.GetLevel();
	SI32 siCanEnterMapIndex = 0;

	if(charlevel <= 40)
	{
		siCanEnterMapIndex = MAPINDEX_SANTAFACTORY_1; // 아이의 산타 작업장.
	}
	else if(charlevel > 30 && charlevel <= 80)
	{
		siCanEnterMapIndex = MAPINDEX_SANTAFACTORY_2; // 청년의 산타 작업장.
	}		
	else if(charlevel > 80)
	{
		siCanEnterMapIndex = MAPINDEX_SANTAFACTORY_3; // 어른의 산타 작업장.
	}

	// 모든 사냥터의 좌표는 동일하다.
	SI32		siWarpX = 56 + (rand() % 5);
	SI32		siWarpY = 89 + (rand() % 5);

	cltWarpInfo clWarpPirateShip(1, siCanEnterMapIndex, false, false, siWarpX, siWarpY, -1, -1);

	pclChar->SetWarpInfo( &clWarpPirateShip, 1024 );
	pclChar->Warp();

	return;
}


