#include "Char\CharManager\CharManager.h"
#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Vote.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-Vote.h"

#include "..\Common\VoteMgr\VoteMgr.h"
#include "../common/VoteMgr/VoteBaseInfo.h"
#include "../common/VoteMgr/VoteStatusMgr.h"


void cltServer::DoMsg_GAMEMSG_REQUEST_VOTEKINGSELECTION( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_VoteKingSelection* pclinfo = (cltGameMsgRequest_VoteKingSelection*)pclMsg->cData;

	if(pclCM->IsAlive(id) == false)return ;
	
	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;
	
	// 투표는 레벨 40 미만은 참여 불가능
	if ( pclCM->CR[id]->pclCI->clIP.GetLevel() < Const_CanVote_MinLevel )
	{
		return;
	}

	// 후보자는 1~MAX_CANDIDATURE_NUM 까지의 번호를 가지고 있음.
	if ( pclinfo->siListIndex < 1 || pclinfo->siListIndex > MAX_CANDIDATURE_NUM )
	{
		return;
	}
	
	// 이전에 투표 해놓고, 또 투표 하는 경우 ( 조작 하면 가능하므로... )
	if ( pclCM->CR[id]->pclCI->clBI.siVoteKingSelectionIndex > 0 )
	{
		return;
	}
	
	sDBRequest_VoteKingSelection clMsg(
		FromPersonID, 
		id, 
		pclinfo->siListIndex
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	// 군주 선거에 투표를 하는 퀘스트 도중이었다면 체크

	pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_VOTEKING);


	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_STARTELECTIONSTUMP( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_StartElectionStump* pclinfo = (cltGameMsgRequest_StartElectionStump*)pclMsg->cData;
	if ( pclinfo == NULL )
	{
		return;
	}

	if ( pclCM->IsAlive(id) == false )
	{
		return;
	}

	// 선거 유세기간이 아니라면 사용못함
	if ( false == bVoteCampaign )
	{
		return;
	}

	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( pclChar == NULL )
	{
		return;
	}
	
	// 상점 개설중에는 선거 유세 못한다
	if ( (pclChar->bPersonalShopSwitch == true) || (pclChar->bPersonalShopSwitch == true) )
	{
		return;
	}

	// 이미 유세중이라면 또할 필요 없는듯
	if ( pclChar->bIsElectionStump == true )
	{
		return;
	}

	// 후보자명/지지사유의 내용이 없는것이 있다면
	if ( (_tcscmp( pclinfo->m_tszCandidate, TEXT("") ) == 0)
		|| (_tcscmp( pclinfo->m_tszMaintenance, TEXT("") ) == 0)
	)
	{
		return;
	}

	SI32 siCharID		= id;
	SI32 siPersonID		= pclChar->pclCI->GetPersonID();

	SI32 siStumpUnique	= ITEMUNIQUE(24080);
	SI32 siStumpNumber	= 1;
	SI32 siStumpPosition = pclChar->pclCI->clCharItem.GetItemPos( siStumpUnique );

	// 유세이용권 아이템의 인벤위치가 이상한 곳이라면
	if ( (siStumpPosition<PERSONITEM_INV0) || (siStumpPosition>=MAX_ITEM_PER_PERSON) )
	{
		return;
	}
	
	// 유세이용권 아이템의 갯수가 1개도 없다면
	if ( pclChar->pclCI->clCharItem.clItem[ siStumpPosition ].siItemNum < siStumpNumber )
	{
		return;
	}

	// 유세 정보 저장
	pclChar->bIsElectionStump = true;

	StringCchCopy( pclChar->szCandidate, Const_Max_Candidate, pclinfo->m_tszCandidate );
	pclChar->szCandidate[Const_Max_Candidate-1] = '\0';

	StringCchCopy( pclChar->szMaintenance, Const_Max_Maintenance, pclinfo->m_tszMaintenance );
	pclChar->szMaintenance[Const_Max_Maintenance-1] = '\0';

	// 광고창을 지운다
	pclChar->SetHeadChat( false, NULL );

	// 유세이용권을 사용하도록한다
	cltItem clStumpItem;

	clStumpItem.Set( &pclChar->pclCI->clCharItem.clItem[ siStumpPosition ] );
	clStumpItem.siItemNum = siStumpNumber;

	sDBRequest_UseItem clMsgUseItem( siCharID, siPersonID, siStumpPosition, &clStumpItem, USEITEM_REASON_NONE );
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsgUseItem);

	// 이웃들에게 베너를 띄우도록 한다
	cltGameMsgResponse_StartElectionStump clMsgElectionStump( pclChar->GetCharUnique(), pclinfo->m_tszCandidate, pclinfo->m_tszMaintenance );
	cltMsg clMsg(GAMEMSG_RESPONSE_STARTELECTIONSTUMP, sizeof(clMsgElectionStump), (BYTE*)&clMsgElectionStump);
	pclChar->SendNeighbourMsg( &clMsg, true );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_ENDELECTIONSTUMP( cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsAlive(id) == false )
	{
		return;
	}

	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( pclChar == NULL )
	{
		return;
	}

	// 유세중도 아닌데 중단할 필요없음
	if ( pclChar->bIsElectionStump == false )
	{
		return;
	}

	pclChar->bIsElectionStump = false;
	StringCchCopy( pclChar->szCandidate,	Const_Max_Candidate,	TEXT("") );
	StringCchCopy( pclChar->szMaintenance,	Const_Max_Maintenance,	TEXT("") );
	
	cltGameMsgResponse_EndElectionStump clMsgEnd( pclChar->GetCharUnique() );
	cltMsg clMsg(GAMEMSG_RESPONSE_ENDELECTIONSTUMP, sizeof(clMsgEnd), (BYTE*)&clMsgEnd);
	pclChar->SendNeighbourMsg( &clMsg, true );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_ELECTIONSTUMP( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_ElectionStump* pclinfo = (cltGameMsgRequest_ElectionStump*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	SI32 siCharID = pclCM->GetIDFromCharUnique(pclinfo->m_siCharUnique);
	if ( false == pclCM->IsValidID(siCharID) )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siCharID];
	if ( NULL == pclchar )
	{
		return;
	}

	if ( false == pclchar->bIsElectionStump )
	{
		return ;
	}

	cltGameMsgResponse_StartElectionStump clMsgElectionStump( pclinfo->m_siCharUnique, pclchar->szCandidate, pclchar->szMaintenance );
	cltMsg clMsg(GAMEMSG_RESPONSE_STARTELECTIONSTUMP, sizeof(clMsgElectionStump), (BYTE*)&clMsgElectionStump);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

// [영훈] 선거 시스템 : 투표소 지역으로 이동해라
void cltServer::DoMsg_GAMEMSG_REQUEST_WARPTOPOLLMAP( cltMsg* pclMsg, SI32 id )
{
	// 현재 군주 투표중이 아니라면 갈 필요 없어
	if ( pclVoteMgr->IsPlayStatus() == false )
	{
		return;
	}

	if ( pclCM->IsAlive(id) == false )
	{
		return;
	}

	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( pclChar == NULL )
	{
		return;
	}
	
	SI32 siMapIndex = pclChar->GetMapIndex();
	// 혹시 현재 있는 곳이 투표소 지역이라면 또 워프할 필요가 있을까?
	if ( Const_MapIndex_PollMap == siMapIndex )
	{
		return;
	}

	// 개인상점을 개설한 상태에서는 이동이 안된다
	if ( (true == pclChar->bPersonalShopSwitch) || (true == pclChar->bPremiumPersonalShop) )
	{
		return;
	}

	
	// 수감되어 있는 상태라면 이동이 안된다.
	if ( pclChar->clInnerStatus.IsStatus(INNERSTATUS_PRISON) )
	{
		return;
	}

	
	SI32		siWarpX = 26;
	SI32		siWarpY = 38;
	cltWarpInfo clWarpPollMap(1, Const_MapIndex_PollMap, true, false, siWarpX, siWarpY, -1, -1);

	pclChar->SetWarpInfo( &clWarpPollMap, 905 );
	pclChar->Warp();

}

void cltServer::DoMsg_GAMEMSG_REQUEST_VOTEKINGAVAILABLE( cltMsg* pclMsg, SI32 id )
{
	// 현재 군주 투표중이 아니라면 투표할 필요 없음
	if ( pclVoteMgr->IsPlayStatus() == false )
	{
		return;
	}

	if ( pclCM->IsAlive(id) == false )
	{
		return;
	}

	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( pclChar == NULL )
	{
		return;
	}
	
	// 이미 투표를 했다
	if ( pclChar->pclCI->clBI.siVoteKingSelectionIndex > 0 )
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_REASON_ALREADYVOTE, 0, 0, pclChar->GetID() );
		return;
	}
	
	// 투표를 할 수 없는 레벨이라면
	if ( pclChar->pclCI->clIP.GetLevel() < Const_CanVote_MinLevel )
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_REASON_DONOTARRIVELEVLE, 0, 0, pclChar->GetID() );
		return;
	}

	cltGameMsgResponse_VoteKingAvailable clVoteKing( pclVoteMgr->GetVoteExplain(), pclVoteMgr->GetCandidatureList() );
	cltMsg clMsg( GAMEMSG_RESPONSE_VOTEKINGAVAILABLE, sizeof(clVoteKing), (BYTE*)&clVoteKing );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_STARTVOTESTATUS( cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsAlive(id) == false )
	{
		return;
	}

	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( pclChar == NULL )
	{
		return;
	}


	cltGameMsgResponse_StartVoteStatus clMsgStartVoteStatus( 
		(TCHAR**)pclVoteStatusMgr->m_tszCandidateName
		,pclVoteStatusMgr->m_siCandidateNum
		,pclVoteStatusMgr->m_siTotalVote
		,pclVoteStatusMgr->m_siTopVoteNum
	);
	cltMsg clMsg( GAMEMSG_RESPONSE_STARTVOTESTATUS, sizeof(clMsgStartVoteStatus), (BYTE*)&clMsgStartVoteStatus);
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );

}