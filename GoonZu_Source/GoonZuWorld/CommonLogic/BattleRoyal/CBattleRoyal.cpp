#include ".\CBattleRoyal.h"
#include "../../Server/Server.h"

CBattleRoyal::CBattleRoyal(void)
{
	m_pclState = &m_clEndState;
	m_pclGameMode = NULL;
	m_pclReward = NULL;
	m_pclStadium = NULL;
	m_siNextStartTime = 0;
}

CBattleRoyal::~CBattleRoyal(void)
{
	if( NULL != m_pclState )
	{
		m_pclState = NULL;
	}

	if( NULL != m_pclGameMode )
	{
		delete m_pclGameMode;
		m_pclGameMode = NULL;
	}

	if( NULL != m_pclReward )
	{
		delete m_pclReward;
		m_pclReward = NULL;
	}

	if( NULL != m_pclStadium )
	{
		delete m_pclStadium;
		m_pclStadium = NULL;
	}
}
// 대기 상태로
BOOL	CBattleRoyal::Waiting()
{
	m_pclState = &m_clWaitState;
	
	return Action();
}
// 게임 상태로
BOOL	CBattleRoyal::Gaming()
{
	m_pclState = &m_clGameState;
	
	return Action();
}
// 종료 상태로
BOOL	CBattleRoyal::Ending()
{
	m_pclState = &m_clEndState;
	
	return Action();
}
// 보상 상태로
BOOL	CBattleRoyal::Rewarding()
{
	m_pclState = &m_clRewardState;
	
	return Action();
}
// 시작 상태로
BOOL	CBattleRoyal::Starting()
{
	m_pclState = &m_clStartState;
	
	return Action();
}
// 대기상태인지
BOOL	CBattleRoyal::IsWaiteState()
{
	if( m_pclState == &m_clWaitState )
		return TRUE;
	return FALSE;
}
// 게임상태인지
BOOL	CBattleRoyal::IsGamingState()
{
	if( m_pclState == &m_clGameState )
		return TRUE;
	return FALSE;
}
// 종료상태인지
BOOL	CBattleRoyal::IsEndState()
{
	if( m_pclState == &m_clEndState )
		return TRUE;
	return FALSE;
}
// 보상상태인지
BOOL	CBattleRoyal::IsRewardState()
{
	if( m_pclState == &m_clRewardState )
		return TRUE;
	return FALSE;
}
// 시작상태인지
BOOL	CBattleRoyal::IsStartState()
{
	if( m_pclState == &m_clStartState )
		return TRUE;
	return FALSE;
}



// 행동
BOOL	CBattleRoyal::Action()
{
	if( NULL == m_pclState)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclStadium)
		return FALSE;

	BOOL bValue = m_pclState->Handle();
	return bValue;
}

void	CBattleRoyal::InitState()	// 상태 객체 초기화
{
	m_clWaitState.Init(m_pclGameMode, m_pclReward, m_pclStadium);
	m_clGameState.Init(m_pclGameMode, m_pclReward, m_pclStadium);
	m_clEndState.Init(m_pclGameMode, m_pclReward, m_pclStadium);
	m_clRewardState.Init(m_pclGameMode, m_pclReward, m_pclStadium);
	m_clStartState.Init(m_pclGameMode, m_pclReward, m_pclStadium);
}
// 입장 가능 레벨
BOOL	CBattleRoyal::GetCanEnterLevel(UI32* uiMinLevel, UI32* uiMaxLevel)
{
	if( NULL == uiMinLevel)
		return FALSE;
	if( NULL == uiMaxLevel)
		return FALSE;
	if( NULL == m_pclStadium)
		return FALSE;

	*uiMinLevel = m_pclStadium->GetCanEnterMinLevel();
	*uiMaxLevel = m_pclStadium->GetCanEnterMaxLevel();

	return TRUE;
}
// 입장 가능한지  
BOOL	CBattleRoyal::CanEnter(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclStadium)
		return FALSE;

	SI32 siPersonID = pclChar->pclCI->GetPersonID();

	//1. 경기장 문이 열려야 함
	if (FALSE == m_pclStadium->IsOpenedGate())
		return BATTLEROYAL_ENTERERROR_ISOPENED;
	//2. 전 경기에 참가 여부
	if (TRUE == m_pclStadium->WasJoinedLastGame(siPersonID))
		return BATTLEROYAL_ENTERERROR_WASJOINED;
	//3. 제조 중 일 경우 참가 불가
	if ( pclChar->siPersonalMakeItemType > 0)
		return BATTLERPYAL_ENTERERROR_ISMAKING;
	//4. 소환수 소환 여부
	if ( pclChar->siChildCharUnique > 0)
		return BATTLERPYAL_ENTERERROR_TAKEDOUTSUMMON;
	//5. 수호정령 소환 여부
	if ( pclChar->siMySoulGuardUnique > 0)
		return BATTLERPYAL_ENTERERROR_TAKEDOUTSUMMON;
	//6. 참가자 수가 한계치 인지?
	if ( m_pclStadium->GetWaitGamerNum() >= CBattleRoyalStadiumServer::MAX_GAMER_COUNT)
		return BATTLERPYAL_ENTERERROR_FULLEDGAMER;
	//7. 파티 중인지
	if ( pclChar->siPartyIndex > 0 )
	{
		return BATTLERPYAL_ENTERERROR_JOINPARTY;
	}
	// 입장 할 수 있다면

	//1. 캐릭터를 하이드 모드로 바꾼다.
	m_pclGameMode->ChangeInvisibleMode(pclChar);

	return TRUE;
}

// 경기장 앞으로 이동
void	CBattleRoyal::WarpFrontOfStadium(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return;

	// 실제 워프할 곳
	SI32	siWarpX = 503;
	SI32	siWarpY = 257;

	cltWarpInfo clWarpPollMap(1, MAPINDEX_MAINFIELD, true, false, siWarpX, siWarpY, -1, -1);

	pclChar->SetWarpInfo( &clWarpPollMap, 905 );
	pclChar->Warp();
}

// 배틀로얄 중에 유저를 죽인 사람에게 아이템을 준다.(100% 확률 아님)
BOOL	CBattleRoyal::GiveItemToKiller(cltCharServer* pclChar)
{
	if( NULL == m_pclReward)
		return FALSE;

	if( NULL == pclChar)
		return FALSE;

	if( FALSE == m_pclReward->GiveRewardToKillingUser(pclChar))
		return FALSE;

	return TRUE;
}

// 맵 인덱스
SI32	CBattleRoyal::GetStadiumMapIndex()
{
	if( NULL == m_pclStadium)
		return FALSE;

	return m_pclStadium->GetMapIndex();
}
// 경기장을 나갔는지 지속적으로 체크
BOOL	CBattleRoyal::CheckOutGamer()
{
	if( NULL == m_pclStadium)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	cltCharServer* pclChar = NULL;
	for(SI32 i = 0 ; i < CBattleRoyalStadiumServer::MAX_GAMER_COUNT; i++)
	{
		m_pclStadium->ProcessLogoutCharByIndex(i);
		pclChar = m_pclStadium->GetGamingCharByIndex(i);
		if( NULL == pclChar)
			continue;

		if(m_pclStadium->GetMapIndex() != pclChar->GetMapIndex())
		{
			RestorChar(pclChar);

			//pclChar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
			cltMsg clMassage(GAMEMSG_RESPONSE_BATTLEROYAL_ENDGAME, 0, NULL );
			pclChar->SendNetMsg((sPacketHeader*)&clMassage);
			m_pclStadium->DeleteGamer(pclChar->pclCI->GetPersonID());
		}

		if( pclChar->siKilledMode == KILLEDMODE_BATTLEROYAL && pclChar->siCharMode == CHARMODE_BATTLEROYAL)
		{
			RestorChar(pclChar);

			//pclChar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
			cltMsg clMassage(GAMEMSG_RESPONSE_BATTLEROYAL_ENDGAME, 0, NULL );
			pclChar->SendNetMsg((sPacketHeader*)&clMassage);
		}

	}

	return TRUE;
}
// 조기 종료 확인
BOOL	CBattleRoyal::IsEarlyEnd()
{
	if( NULL == m_pclStadium)
		return FALSE;

	if( m_pclStadium->GetGamingGamerNum() > 1)
	{
		return FALSE;
	}
	
	if( m_pclStadium->GetGamingGamerNum() == 1)
	{
		if( m_pclStadium->GetMobNum() <= 0)
			return TRUE;
		return FALSE;
	}

	return TRUE;
}

BOOL	CBattleRoyal::DieMob()
{	
	if( NULL == m_pclStadium)
		return FALSE;

	m_pclStadium->DieMob();
	return TRUE;
}

void	CBattleRoyal::SetNextStartTime(SI32 siNextStartTime)
{
	m_siNextStartTime = siNextStartTime;
}
SI32	CBattleRoyal::GetNextStartRemainedTime()
{
	_SYSTEMTIME st;
	GetLocalTime(&st);

	const SI32 HOURTOMINUTE = 60;

	SI32 siEndMinute = m_siNextStartTime * HOURTOMINUTE;
	SI32 siStartMinute = (st.wHour * HOURTOMINUTE) + st.wMinute;

	return siEndMinute - siStartMinute;
}
// 힐링아이템을 사용 할 수 있는 지
BOOL	CBattleRoyal::CanUseHealItem()
{
	if( NULL == m_pclGameMode)
		return TRUE;

	if(m_pclGameMode->GetMode() == BATTLEROYALGAMEMODE_RESTRICTTOUSEHEALITEM)
		return FALSE;

	return TRUE;
}
// 게임 플레이 시간 설정
BOOL	CBattleRoyal::SetGameTime(SI32 siTime)
{
	if( NULL == m_pclGameMode)
		return FALSE;

	m_pclGameMode->SetGameTime(siTime);

	return TRUE;
}
BOOL	CBattleRoyal::IsExistStadium(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return FALSE;

	if( pclChar->GetMapIndex() == m_pclStadium->GetMapIndex())
	{
		return TRUE;
	}

	return FALSE;
}
BOOL	CBattleRoyal::ActionGameMode()
{
	if( NULL == m_pclGameMode)
		return FALSE;

	if( BATTLEROYALGAMEMODE_POWERUPONEUSER == m_pclGameMode->GetMode())
	{
		m_pclGameMode->PowerupOneUser();	
	}

	return TRUE;
}

BOOL	CBattleRoyal::SetGMGameMode(SI32 siMode)
{
	if( NULL == m_pclGameMode)
		return FALSE;
		
	return m_pclGameMode->SetGMGameMode(siMode);
}

// 경기장에 들어갈 때 말에서 내려 있다면 강제로 말 넣기
BOOL	CBattleRoyal::TakeinHorse(cltCharServer* pclChar)
{
	if ( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	m_pclGameMode->TakeinHorse(pclChar);

	return TRUE;
}
// 경기장에 들어와서 변한 유저 정보를 원상태로 돌린다.
BOOL	CBattleRoyal::RestorChar(cltCharServer* pclChar)
{
	if ( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	m_pclGameMode->RestoreCharInfo(pclChar);
	m_pclGameMode->RestoreTrancsformChar(pclChar);

	return TRUE;
}
// 유저를 몹으로 변신 시킨다.
BOOL	CBattleRoyal::TransformChar(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;
	
	if( FALSE ==  m_pclGameMode->TransformChar(pclChar))
		return FALSE;
	
	return TRUE;
}