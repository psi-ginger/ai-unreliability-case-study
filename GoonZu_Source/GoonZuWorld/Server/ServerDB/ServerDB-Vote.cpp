#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"
#include "..\CommonLogic\Msg\MsgType-Vote.h"
#include "..\CommonLogic\MsgType-Person.h"
#include "..\CommonLogic\MsgRval-Define.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Vote.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

#include "../common/VoteMgr/VoteStatusMgr.h"

void cltServer::DoMsg_DBMSG_RESPONSE_VOTEINIT(sPacketHeader* pPacket)
{
	sDBResponse_VoteInit * pclMsg = (sDBResponse_VoteInit *)pPacket;
	
	if ( pclMsg->siResult != 1 )
	{
		return;
	}
		
	for ( SI32 i = 0; i < MAX_PERSON_NUMBER; ++i )
	{
		if ( pclCM->IsValidID( i ) )
		{
			pclCM->CR[i]->pclCI->clBI.siVoteKingSelectionIndex = 0;
		}
	}

	pclVoteMgr->SetVoteStatus( false );

	SI32 id = pclMsg->siMasterCharID;

	if ( pclMsg->siMasterPersonID < 1 || pclMsg->siMasterCharID < 1 )
	{
		return;
	}

	// 유저가 있다면, 응답해 준다.
	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siMasterPersonID )
	{
		SendServerResponseMsg(0, SRVAL_VOTE_INIT, 0, 0, pclMsg->siMasterCharID);
	}
	

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_VOTEKINGSELECTION(sPacketHeader* pPacket)
{
	sDBResponse_VoteKingSelection * pclMsg = (sDBResponse_VoteKingSelection *)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if ( pclMsg->siVoteListIndex < 1 || pclMsg->siVoteListIndex >= MAX_CANDIDATURE_NUM )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	// 유저가 있다면, 응답해 준다.
	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		pclCM->CR[id]->pclCI->clBI.siVoteKingSelectionIndex = pclMsg->siVoteListIndex;
		

		stCandidature *pCnadidature = pclVoteMgr->GetCandidatureList();
		if ( pCnadidature == NULL )
		{
			return;
		}
		
		if ( pCnadidature[ pclMsg->siVoteListIndex - 1 ].siListIndex > 0 &&
			pCnadidature[ pclMsg->siVoteListIndex - 1 ].siListIndex == pclMsg->siVoteListIndex )
		{
			cltGameMsgResponse_VoteKingSelection clinfo( pclMsg->siVoteListIndex, pCnadidature[ pclMsg->siVoteListIndex - 1 ].strChatName );
			cltMsg clMsg(GAMEMSG_RESPONSE_VOTEKINGSELECTION, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			((cltCharServer*)pclCM->CR[id])->GiveItem( ITEMUNIQUE(24094), 1, INPUTITEMPERSON_OPTION_VOTE, 0 );
		}
	}
	
	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_VOTEKINGAGGREGATE(sPacketHeader* pPacket)
{
	sDBResponse_VoteKingAggregate * pclMsg = (sDBResponse_VoteKingAggregate *)pPacket;

	if ( pclMsg->m_siResult != 1 )
	{
		return;
	}

	SI32 id = pclMsg->m_siCharID;

	if ( id < 1 )
	{
		return;
	}


	switch ( pclMsg->m_siType ) 
	{
	case VOTE_STATUS_TYPE_RESULT:
		{
			TCHAR charnames[MAX_CANDIDATURE_NUM][MAX_PLAYER_NAME];
			ZeroMemory(charnames, MAX_CANDIDATURE_NUM * MAX_PLAYER_NAME * sizeof(TCHAR));


			stCandidature *CandidateList = pclVoteMgr->GetCandidatureList();

			for (SI32 i = 0 ; i < MAX_CANDIDATURE_NUM ; i++)
			{

				if (CandidateList[i].siListIndex > 0 && CandidateList[i].siListIndex <= MAX_CANDIDATURE_NUM )
					StringCchPrintf(charnames[CandidateList[i].siListIndex - 1], MAX_PLAYER_NAME, CandidateList[i].strChatName );
			}

			cltGameMsgResponse_VoteKingAggregate clinfo( (TCHAR**)charnames, pclMsg->m_siVoteCount );
			cltMsg clMsg(GAMEMSG_RESPONSE_VOTEKINGAGGREGATE, sizeof(clinfo), (BYTE*)&clinfo);

			// 유저가 있다면, 응답해 준다.
			if ( pclCM->IsValidID(id) )
			{
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}

			//--------------------------------------------------------------------------
			// 투표상황을 로그로 기록한다.
			/*NTCHARString<2048>		Log;
			for (SI32 i = 0 ; i < MAX_CANDIDATURE_NUM ; i++)
			{
				if( Log.Length() > 0 )		Log += "\t";

				Log += charnames[i];
				Log += ":";
				Log += SI32ToString(pclMsg->m_siVoteCount[i]);
			}

			// 서버에 로그로 기록한다.
			pclClient->pclLog->FilePrint("Config\\VoteAggregate.log", "%s", (TCHAR*)Log);*/
			//--------------------------------------------------------------------------
		}
		break;

	case VOTE_STATUS_TYPE_ONAIR:
		{
			pclVoteStatusMgr->StartVoteStatus( pclMsg->m_siVoteCount );
		}
		break;
	case VOTE_STATUS_TYPE_WRITE_RESULT:
		{
			pclVoteStatusMgr->WriteResult( pclMsg->m_siVoteCount );
		}
		break;
	}


	return;
}