#include "VoteMgr.h"
#include "Msg/MsgType-Vote.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../Char/CharServer/Char-Server.h"

#include "..\..\Client\Client.h"

#include "../../Server/Server.h"

#include "../../CommonLogic/MsgRval-Define.h"

extern cltCommonLogic* pclClient;


CVoteMgr::CVoteMgr()
{
	m_pstCandidature	= new stCandidature[ MAX_CANDIDATURE_NUM ];
	m_strVoteExplain	= new TCHAR[ MAX_VOTEEXPLAIN_LENGTH ];
	m_siGateID			= 0;

	Initialize();	
}

CVoteMgr::~CVoteMgr()
{	
	if ( m_strVoteExplain )
	{
		delete [] m_strVoteExplain;
		m_strVoteExplain = NULL;
	}

	if ( m_pstCandidature )
	{
		delete [] m_pstCandidature;
		m_pstCandidature = NULL;
	}
	
}

void CVoteMgr::Initialize()
{
	m_bPlayStatus	= false;

	memset( m_pstCandidature, 0, sizeof(stCandidature) * MAX_CANDIDATURE_NUM );
	memset( m_strVoteExplain, 0, MAX_VOTEEXPLAIN_LENGTH);
}

void CVoteMgr::Destroy()
{
}

bool CVoteMgr::ReadVoteData()
{
	// 버전을 파일에 박아 두면 자동으로 가능하지만... 지금은 그렇게 하지 않음.
	FILE *fp = _tfopen( TEXT("./Vote/Vote.txt"), TEXT("rt") );

	if ( fp == NULL )
		return false;

	TCHAR Buffer[256] = TEXT("");
	SI32 index = 0;

	// 투표 설명
	//_ftscanf( fp, TEXT("%s"), m_strVoteExplain );
	_fgetts( m_strVoteExplain, MAX_VOTEEXPLAIN_LENGTH - 3, fp );		// -3 은 적당히 읽으라고... ( 혹시나 해서... )

	SI32 VoteExplainLen = _tcslen( m_strVoteExplain );
	if ( VoteExplainLen < 1 )
	{
		return false;
	}

	if ( m_strVoteExplain[ VoteExplainLen - 1 ] == '\n' )
	{
		m_strVoteExplain[ VoteExplainLen - 1 ] = '\0';
	}

	// 그냥 설명 읽음.
	_fgetts( Buffer, 127, fp );

	StringCchCat( m_strVoteExplain, MAX_VOTEEXPLAIN_LENGTH, TEXT("") );
	//strcat( m_strVoteExplain, TEXT("\r\n") );

	while( ! feof( fp ) )
	{
		_ftscanf( fp, TEXT("%d %s %s"), &m_pstCandidature[ index ].siListIndex, m_pstCandidature[ index ].strChatName, m_pstCandidature[ index ].strWebPageURL );
		
		++index;

		if ( index >= MAX_CANDIDATURE_NUM )		// 혹시나 txt 파일에 후보수를 엄청 적었을까 싶어서...
			break;
	}

	fclose( fp );

	return true;
}

void CVoteMgr::SetVoteStatus( bool bSetPlay )
{
	if ( bSetPlay )
	{
		// 기존에 투표가 진행중이라면 어째야 할까? 그냥 둬야 겠지?
		// 혹시나 운영자가 실수로 2번 호출할 수도 있으므로...
		if ( m_bPlayStatus )
		{
			return;
		}
		VotePlay();		
	}
	else
	{
		VoteStop();
	}

	OpenTheGate( bSetPlay );
	
	return;
}

bool CVoteMgr::IsPlayStatus()
{
	return m_bPlayStatus;
}

void CVoteMgr::VoteStop()
{
	LeavePoolMap();

	Initialize();

	return;
}

void CVoteMgr::VotePlay()
{	
	Initialize();
	// 입력 파일(Vote.txt) 에러
	if ( ReadVoteData() == false )
	{
		m_bPlayStatus = false;
		return;
	}

	cltServer *pclserver = (cltServer *)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}

	m_bPlayStatus	= true;
	m_siGateID		= 0;
	
	SI32 id;
	SI32 index = 0;

	// 레벨 40이상, 아직 투표를 안한 사람에게만 메시지를 보냄
	while( id = pclClient->pclCM->GetCharOrder(index) )
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
		
		if ( (pclClient->pclCM->IsValidID( id )) && (pclchar != NULL) && (pclchar->GetSession())
			&& (pclClient->pclCM->CR[id]->pclCI->clIP.GetLevel() >= Const_CanVote_MinLevel)
			&& (pclClient->pclCM->CR[id]->pclCI->clBI.siVoteKingSelectionIndex == 0)
		)
		{
			pclserver->SendServerResponseMsg( 0, SRVAL_NOTICE_WARPTOPOLL_MAP, 0, 0, pclchar->GetID() );
		}
	}

	return;
}

stCandidature *CVoteMgr::GetCandidatureList()
{
	return m_pstCandidature;
}

TCHAR *CVoteMgr::GetVoteExplain()
{
	return m_strVoteExplain;
}

// [영훈] 투표소 게이트를 열고, 닫는 함수
void CVoteMgr::OpenTheGate( bool bOpen )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}

	SI32 siGateKind	= pclClient->GetUniqueFromHash("KIND_POLLINGGATE");	// 투표소 게이트 KIND


	if ( true == bOpen )
	{
		// 게이트를 생성했는데 혹시 또 생성할까봐서
		if ( m_siGateID > 0 )
		{
			return;
		}

		SI32 siX		= 13;	// 위치는 엔펠리스내에 공작 의자들 있는 곳
		SI32 siY		= 38;
		SI32 siMapIndex	= 158;	// 엔펠리스

		m_siGateID = pclserver->SetNPCChar(siGateKind, VILLAGE_NONE, NULL, 1, 0, siX, siY, siMapIndex, NPCITEMMODE_NONE, NULL );
	}
	else
	{
		// 게이트가 생성될때 받아온 아이디로 조건을 진행한다
		if ( pclserver->pclCM->IsValidID( m_siGateID ) == false )
		{
			return;
		}

		// 해당 아이디의 케릭터가 없다면 리턴
		cltCharServer* pclGateChar = (cltCharServer*)pclClient->pclCM->CR[m_siGateID];
		if ( pclGateChar == NULL )
		{
			return;
		}

		// 해당 아이디의 케릭터를 받아왔지만 게이트가 아니라면 리턴
		if ( pclGateChar->GetKind() != siGateKind )
		{
			return;
		}

		// 해당 게이트를 삭제하고
		pclGateChar->DeleteInCell();

		// 삭제가 완료되었으면 게이트 아이디를 다시 초기화시킴
		if ( pclserver->pclCM->DeleteChar( m_siGateID ) == TRUE )
		{
			m_siGateID = 0;
		}
	}

}

// 투표가 종료되면 투표소맵에 있는 캐릭터는 모두 내보낸다
void CVoteMgr::LeavePoolMap()
{
	cltServer* pclserver = (cltServer*)pclClient;
	if( pclserver == NULL )
	{
		return;
	}

	cltCharServer* pclchar = NULL;

	// 맵에 있는 캐릭터를 맵 밖으로 이동시킨다.
	SI32			siIndex		= 0;
	SI32			siCharID	= 0;

	SI32			siX			= 26;
	SI32			siY			= 72;
	cltWarpInfo		clWarpInfo( 1, MAPINDEX_PALACE, true, false, siX, siY, -1, -1 );


	while ( siCharID = pclClient->pclCM->GetCharOrder(siIndex) )
	{
		siIndex++;

		if ( !IsPC(siCharID) )
		{
			continue;
		}

		pclchar = (cltCharServer* )pclClient->pclCM->CR[siCharID];
		if ( pclchar == NULL )
		{
			continue;
		}
		
		// 투표소 맵에 없으면 컨티뉴
		if ( pclchar->GetMapIndex() != Const_MapIndex_PollMap )
		{
			continue;
		}

		// 엔펠리스로 이동 시킨다.
		if ( pclClient->pclCM->IsAlive(siCharID) == TRUE )
		{
			pclchar->SetWarpInfo(&clWarpInfo, 3000);
			pclchar->Warp();
		}

	}	// while ( siCharID = pclClient->pclCM->GetCharOrder(siIndex) ) 종료
}