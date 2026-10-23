#include "VoteStatusMgr.h"
#include "Msg/MsgType-Vote.h"
#include "../../NLib/NUpdateTimer.h"
#include "Char\CharManager\CharManager.h"


extern cltCommonLogic* pclClient;

CVoteStatusMgr::CVoteStatusMgr()
{
	Init();
}

CVoteStatusMgr::~CVoteStatusMgr()
{

}


void CVoteStatusMgr::Init()
{
#ifdef _SAFE_MEMORY
	m_siVote.ZeroMem();
	m_siSendVote.ZeroMem();
#else
	ZeroMemory( m_siVote,	(sizeof(SI32)*MAX_CANDIDATURE_NUM) );
	ZeroMemory( m_siSendVote,	(sizeof(SI32)*MAX_CANDIDATURE_NUM) );
#endif
	ZeroMemory( m_tszCandidateName , (sizeof(TCHAR) * MAX_CANDIDATURE_NUM * MAX_PLAYER_NAME) );

	m_siCandidateNum	= 0;
	m_siTimeCounter		= 0;

	m_siTopVoteNum		= 0;
	m_siTotalVote		= 0;

	m_bStart			= false;
}


void CVoteStatusMgr::StartVoteStatus( SI32 *psiVote )
{
	//-- 오류체크
	if ( psiVote == NULL )
	{
		return;
	}

	//-- 이미 개표 시작 중이라면 개표를 하지 않음
	if ( m_bStart == true )
	{
		return;
	}

	//-- 모든 변수를 초기화
	Init();

	SI08	siTopVote		= 0;
	SI32	siNewVote		= 0;
	SI32	siOldVote		= 0;
	SI32	siTotalVote		= 0;
	
	SI32	siTrickNum		= 200;	// 투표 유저수와 투표율 뻥튀기용
	SI32	siRealTotalVote	= 0;	// 이 수치가 0이면 개표 시작할 필요없음
		
	if ( false == ReadCandidateName() )
	{
		return;
	}

	//-- 후보자 수, 최대 득표 표수를 얻는다
	for ( SI32 siCount=0; siCount<MAX_CANDIDATURE_NUM; siCount++ )
	{
		// 후보자 이름이 없을때까지
		if ( _tcscmp(m_tszCandidateName[siCount], TEXT("")) == 0 )
		{
			break;
		}
		// 최대 득표수를 구하기 위한 사전 작업
		siNewVote = psiVote[siCount];
		
		// 후보자별 최종 득표수를 저장하고
		m_siVote[siCount]	=	psiVote[siCount] + siTrickNum;
		siRealTotalVote		+=	psiVote[siCount];

		// 최대 득표자가 몇번째인지 구하고
		if ( siNewVote > siOldVote )
		{
			siTopVote = siCount;
			siOldVote = siNewVote;
		}
		
		// 모든 투표수를 구한다
		siTotalVote += m_siVote[siCount];

		// 후보자 수 구하기
		m_siCandidateNum++;
	}
	
	//-- 후보자가 없거나 투표수가 없다면 종료
	if ( (0 >= m_siCandidateNum) || (0 >= siRealTotalVote) )
	{
		return;
	}

	m_siTopVoteNum	= m_siVote[siTopVote];
	m_siTotalVote	= siTotalVote;

	//-- 개표가 시작되었다
	m_bStart		= true;

	//-- 메시지를 전송한다	
	cltGameMsgResponse_StartVoteStatus clMsgStartVoteStatus( (TCHAR**)m_tszCandidateName, m_siCandidateNum, m_siTotalVote, m_siTopVoteNum );
	cltMsg clMsg( GAMEMSG_RESPONSE_STARTVOTESTATUS, sizeof(clMsgStartVoteStatus), (BYTE*)&clMsgStartVoteStatus);
	pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

	//-- Action 함수를 기동시킨다
	m_kActionTimer.Init(1 * 1000 * Const_VoteStatusMgr_End_Count);					// 1분에 한번씩 실행됨
	m_kActionTimer.SetActive(true, ::GetTickCount());	
	
}


void CVoteStatusMgr::Action()
{
	if ( m_kActionTimer.IsTimed( GetTickCount() ) == false )
	{
		return;
	}

	SendVoteStatus();

	m_siTimeCounter++;

	if ( m_siTimeCounter >= (Const_VoteStatusMgr_End_Count+1) )
	{
		m_kActionTimer.SetActive( false, GetTickCount() );
		m_bStart = false;
	}
}


void CVoteStatusMgr::SendVoteStatus()
{
	if ( (m_siCandidateNum <= 0) || (m_siCandidateNum > MAX_CANDIDATURE_NUM) )
	{
		return;
	}

	REAL32	fSendRate	= 0.0f;
	SI32	siSendVote	= 0;
	SI32	siRandNum	= 0;

	// 10분에 한번씩 보정값이 들어가냐 안들어 가냐
	bool	bCondition	= false;//( (m_siTimeCounter > 0) && ((m_siTimeCounter % 10) == 0) );

	
	for ( SI32 siCount=0; siCount<m_siCandidateNum; siCount++ )
	{
		
		//-- 일정 1번 카운트(1분에 한번씩) 할때마다 0.33~3.32(100 / 30 = 3.33 뒷자리 빼고)
		siRandNum	= (rand() % 300) + 33; 
		fSendRate	= ( siRandNum / 10000.0f);

		// 10분에 한번씩 각 시간의 비례해서
		if ( true == bCondition )
		{
			fSendRate = 0.0333f * m_siTimeCounter;
		}

		if ( fSendRate < 0.0033f )
		{
			fSendRate = 0.0033f;
		}
		
		siSendVote = (SI32)(m_siVote[siCount] * fSendRate);

		if ( true == bCondition )
		{
			m_siSendVote[siCount] = siSendVote;
		}
		else
		{
			m_siSendVote[siCount] += siSendVote;
		}
		
		// 투표가 끝나거나 보낸값이 득표수보다 많을때
		if ( (m_siSendVote[siCount] > m_siVote[siCount]) || (m_siTimeCounter >= Const_VoteStatusMgr_End_Count) )
		{
			 m_siSendVote[siCount] = m_siVote[siCount];
		}
	}

	//-- 투표현황 메시지 보냄
	cltGameMsgResponse_VoteStatus clMsgVoteStatus( &m_siSendVote[0] );
	cltMsg clMsg( GAMEMSG_RESPONSE_VOTESTATUS, sizeof(clMsgVoteStatus), (BYTE*)&clMsgVoteStatus);
	pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
}

// [영훈] 후보자 이름을 뽑아온다
bool CVoteStatusMgr::ReadCandidateName()
{
	// 
	FILE *fp = _tfopen( TEXT("./Vote/Vote.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		return false;
	}

	TCHAR	Buffer[256] = TEXT("");

	// 투표 설명
	// 구분자는 그냥 지나감
	for ( SI32 siCount=0; siCount<2; siCount++ )
	{
		_fgetts( Buffer, sizeof(Buffer), fp );
	}

	// 실제 이름 읽어오기
	SI32	siTempNum				= 0;		// 쓰지 않음
	TCHAR	szTempUrl[256]			= TEXT("");	// 쓰지 않음
	TCHAR	szName[MAX_PLAYER_NAME]	= TEXT("");	// 후보자 이름
	SI32	siIndex					= 0;

	while( ! feof( fp ) )
	{
		ZeroMemory( szName, (sizeof(TCHAR)*MAX_PLAYER_NAME) );

		_ftscanf( fp, TEXT("%d %s %s"), &siTempNum, szName, szTempUrl );

		StringCchCopy( m_tszCandidateName[siIndex], MAX_PLAYER_NAME, szName );

		++siIndex;

		if ( siIndex >= MAX_CANDIDATURE_NUM )
		{
			break;
		}
	}

	fclose( fp );

	return true;
}

void CVoteStatusMgr::WriteResult( SI32 *psiVote )
{
	if ( psiVote == NULL )
	{
		return;
	}

	if ( ReadCandidateName() == false )
	{
		return;
	}

	NTCHARString<2048>		Log;

	for (SI32 i = 0 ; i < MAX_CANDIDATURE_NUM ; i++)
	{
		if ( _tcscmp( m_tszCandidateName[i], TEXT("") ) == 0 )
		{
			break;
		}

		if( Log.Length() > 0 )		Log += "\t";

		Log += m_tszCandidateName[i];
		Log += ":";
		Log += SI32ToString( psiVote[i] );
	}

	// 서버에 로그로 기록한다.
	pclClient->pclLog->FilePrint("Config\\voteresult.txt", "%s", (TCHAR*)Log);
}