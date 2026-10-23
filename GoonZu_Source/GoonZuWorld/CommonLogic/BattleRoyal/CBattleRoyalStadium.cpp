#include "./CBattleRoyalStadium.h"
#include "../MsgType.h"
#include "../../Server/Server.h"
#include "../../Client/Client.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../msg/MsgType-BattleRoyal.h"

CBattleRoyalStadium::CBattleRoyalStadium()
{
	m_pclCommon = pclClient;

	// 경기장의 문이 열려 있는지?
	m_bOpenGame = false;

	m_siMobCount = 0;
	m_pclWinner = NULL;
	m_siKindBlackarmySword = 0;
	m_siKindBlackarmyRifle = 0;
	m_siKindBlackarmyKatar = 0;
}

CBattleRoyalStadium::~CBattleRoyalStadium()
{

}

BOOL	CBattleRoyalStadium::Init()
{
	if( NULL == m_pclCommon)
		return FALSE;

	// 경기장의 입장 최대 최소 레벨 결정
	m_uiCanEnterMinLevel = m_pclCommon->pclMapManager->GetMinLevel(m_siMapIndex);
	m_uiCanEnterMaxLevel = m_pclCommon->pclMapManager->GetMaxLevel(m_siMapIndex);
	
	return TRUE;
}

// 경기장 입구를 연다.
void	CBattleRoyalStadium::OpenGate()
{
	// 경기장의 문이 열려 있는지?
	m_bOpenGame = true;
}
// 경기장 입구를 닫는다.
void	CBattleRoyalStadium::CloseGate()
{
	// 경기장의 문이 열려 있는지?
	m_bOpenGame = false;
}

// 참가자 퇴장
BOOL	CBattleRoyalStadium::DeleteGamer(SI32 siPersonID)
{	return TRUE;
}




// 입장 가능 최소 레벨
UI32	CBattleRoyalStadium::GetCanEnterMinLevel()
{
	return m_uiCanEnterMinLevel;
}
// 입장 가능 최대 레벨
UI32	CBattleRoyalStadium::GetCanEnterMaxLevel()
{
	return m_uiCanEnterMaxLevel;
}
// 경기장 입구가 열려 있는지?
BOOL	CBattleRoyalStadium::IsOpenedGate()
{
	return m_bOpenGame;
}

SI32	CBattleRoyalStadium::GetMapIndex()
{
	return m_siMapIndex;
}

cltCharCommon* CBattleRoyalStadium::GetWinner()
{
	return m_pclWinner;
}

//====================================================================================================================================
CBattleRoyalStadiumServer::CBattleRoyalStadiumServer()
{
	m_pclServer = (cltServer*)pclClient;
	m_siGamerCount = 0;
	m_siBeforeGamerCount = 0;
	m_siGamingUserCount = 0;

}

CBattleRoyalStadiumServer::~CBattleRoyalStadiumServer()
{

}


BOOL	CBattleRoyalStadiumServer::Init()
{
	if( NULL == m_pclCommon)
		return FALSE;

	// 경기장의 입장 최대 최소 레벨 결정
	m_uiCanEnterMinLevel = m_pclCommon->pclMapManager->GetMinLevel(m_siMapIndex);
	m_uiCanEnterMaxLevel = m_pclCommon->pclMapManager->GetMaxLevel(m_siMapIndex);

	m_MobCharIDArray.Resize(MAX_UNIT_COUNT,MAX_UNIT_COUNT);
	m_BeforeGamerArray.Resize(MAX_GAMER_COUNT,MAX_GAMER_COUNT);
	m_GamerArray.Resize(MAX_GAMER_COUNT,MAX_GAMER_COUNT);

	return TRUE;
}

// 경기장이 열림을 알린다.
BOOL	CBattleRoyalStadiumServer::Notice(TCHAR* pszContentText)
{
	if( NULL ==  pszContentText )
		return FALSE;

	TCHAR* pszTitleText = GetTxtFromMgr(5482); 
	if( NULL ==  pszTitleText )
		return FALSE;

	cltGameMsgResponse_Notice clNotice(pszTitleText, pszContentText,true);
	cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

	m_pclServer->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);	

	return TRUE;
}

// 경기에 참가자 리스트를 저장한다.
BOOL	CBattleRoyalStadiumServer::SaveGamerList()
{
	// 1. 전 유저 목록 초기화
	m_BeforeGamerArray.ClearData();
	// 2. 시작될 배틀로얄에 참가한 유저 목록 저장
	cltCharServer* pclCharServer = NULL;

	for (SI32 siID = 0; siID < MAX_PERSON_NUMBER; siID++)
	{
		if( m_siGamerCount >= MAX_GAMER_COUNT)
			break;

		pclCharServer = m_pclServer->pclCM->GetCharServer(siID);
		if( NULL == pclCharServer)
			continue;

		if(m_siMapIndex == pclCharServer->pclCI->clBI.GetMapIndex())
		{
			CBattleRoyalPersonChangedData& pclGamer = m_GamerArray.GetData(m_siGamerCount);

			pclGamer.SetPersonID( pclCharServer->pclCI->GetPersonID()) ;
			++m_siGamerCount;
		}
	}
	
	// 3. 다음 배틀로얄에서 시작될 배틀로얄에 참가한 유저를 거부 하기 위한 용도의 목록 저장

	// 배틀 로얄에 입장한 유저 목록 저장
	m_siBeforeGamerCount = m_siGamerCount;
	m_siGamingUserCount =  m_siGamerCount;

	for( SI32 i = 0 ; i < m_siBeforeGamerCount; i++)
	{
		CBattleRoyalPersonChangedData& pclGamer =  m_GamerArray.GetData(i);
		m_BeforeGamerArray[i] = pclGamer.GetPersonID();

		pclCharServer = m_pclServer->pclCM->GetCharServer(i);
	}

	// 로그 -  계정, 캐릭터 명, 레벨 
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		m_pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BATTLEROYAL, LOGCOMMAND_INDEX_BATTLEROYAL_GAMERCOUNT,
			0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			0, m_siGamerCount, 0, 0, 0, 0, NULL, NULL);
	}

	return TRUE;
}

// 참가자 퇴장
BOOL	CBattleRoyalStadiumServer::DeleteGamer(SI32 siPersonID)
{
	if(m_siGamerCount <= 0)
		return FALSE;
	
	for(SI32 i = 0; i < MAX_GAMER_COUNT; i++)
	{
		CBattleRoyalPersonChangedData& pclGamer =  m_GamerArray.GetData(i);

		if( pclGamer.GetPersonID() == siPersonID)
		{
			pclGamer.SetPersonID(0);
			--m_siGamingUserCount;
			return TRUE;
		}
	}
	
	return FALSE;
}
// 몹을 제거한다.
BOOL	CBattleRoyalStadiumServer::RemoveMob()
{
	if( NULL == m_pclServer)
		return FALSE;


	cltCharServer * pclChar = NULL;
	for(SI32 i = 0; i < MAX_UNIT_COUNT; i++)
	{
		pclChar =m_pclServer->pclCM->GetCharServer(m_MobCharIDArray.GetData(i));

		if(NULL == pclChar)
			continue;
	
		pclChar->SetLife(0);
	}

	return TRUE;
}

// 몹을 생성한다.
BOOL	CBattleRoyalStadiumServer::CreateMob()
{
	if( NULL == m_pclServer)
		return FALSE;

	const SI32 MOBNUMCUTLINE = 15;
	const SI32 POS_BASIC_X = 6;
	const SI32 POS_BASIC_Y = 6;
	const SI32 POS_EXTENSION_LEFT_X = 10;
	const SI32 POS_EXTENSION_LEFT_Y = 26;
	const SI32 POS_EXTENSION_RIGHT_X = 26;
	const SI32 POS_EXTENSION_RIGHT_Y = 10;
	const SI32 POS_BASIC_WIDTH = 20;
	const SI32 POS_BASIC_HEIGHT = 20;
	const SI32 POS_EXTENSION_LEFT_WIDTH = 15;
	const SI32 POS_EXTENSION_LEFT_HEIGHT = 5;
	const SI32 POS_EXTENSION_RIGHT_WIDTH = 5;
	const SI32 POS_EXTENSION_RIGHT_HEIGHT = 16;

	const SI32 ROOMCOUNT = 1;
	const SI32 MOBKINDCOUNT = 3;

	const SI32 NOTEXISTPOSCOUNT = 30;
	
	// 생성될 몹 정보
	m_MobCharIDArray.ClearData();
	SI32 siX		=	0;		// X좌표
	SI32 siY		=	0;		// Y좌표
	SI32 siMakeCount=	0;		// 만들어낸 몬스터 숫자
	SI32 siEnergy	=	10000;
	SI32 siLevel	=	10;
	SI32 siMobCountInRoom = 0;	// 룸마다 포함된 몹의 수

	SI32 siBasicPosX = 0;
	SI32 siBasicPosY = 0;
	SI32 siMobKind = 0;
	SI32 siMobNumInExtensionRoom = 1;
	m_siMobCount = MAX_UNIT_COUNT - m_siGamerCount;

	if( m_siMobCount > MOBNUMCUTLINE)
	{
		siMobNumInExtensionRoom = 2;
	}

	SI32 siNoExistArrayIndex = 0;
	cltPos clNoExistPos[NOTEXISTPOSCOUNT];
	clNoExistPos[siNoExistArrayIndex++].Set(12,18);
	clNoExistPos[siNoExistArrayIndex++].Set(12,19);
	clNoExistPos[siNoExistArrayIndex++].Set(12,20);
	clNoExistPos[siNoExistArrayIndex++].Set(13,19);
    clNoExistPos[siNoExistArrayIndex++].Set(6,6);
	clNoExistPos[siNoExistArrayIndex++].Set(6,7);
	clNoExistPos[siNoExistArrayIndex++].Set(6,8);
	clNoExistPos[siNoExistArrayIndex++].Set(7,6);
	clNoExistPos[siNoExistArrayIndex++].Set(8,6);

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(  ENEMYTYPE_ALLPC, 0);
	cltIdentity clIdentity;
	clIdentity.MakeBattleRoyalMob( IDENTITY_BATTLEROYAL_MONSTER );

	srand( (unsigned)time( NULL ) );

	SI32 siDiv = 0;
	SI32 siMobID = 0;
	SI32 siTmpX = 0;
	SI32 siTmpY = 0;
	SI32 siHeightRange = 0;
	bool bRelocation = false;
	for (siMakeCount; siMakeCount < m_siMobCount; siMakeCount++ )
	{
		siTmpX = 0;
		siTmpY = 0;
		siHeightRange = 0;
		siMobID = 0;
		bRelocation = false;

		if( siMobNumInExtensionRoom > siMakeCount)
		{
			siTmpX = rand() % POS_EXTENSION_LEFT_WIDTH;
			siX = siTmpX + POS_EXTENSION_LEFT_X;

			siTmpY = rand() % POS_EXTENSION_LEFT_HEIGHT;
			siY = siTmpY + POS_EXTENSION_LEFT_Y;
		}
		else if ( siMobNumInExtensionRoom * 2 > siMakeCount && siMakeCount >= siMobNumInExtensionRoom)
		{
			siTmpX = rand() % POS_EXTENSION_RIGHT_WIDTH;
			siX = siTmpX + POS_EXTENSION_RIGHT_X;

			siTmpY = rand() % POS_EXTENSION_RIGHT_HEIGHT;
			siY = siTmpY + POS_EXTENSION_RIGHT_Y;
		}
		else
		{
			siTmpX = rand() % POS_BASIC_WIDTH;
			siX = siTmpX + POS_BASIC_X;

			siTmpY = rand() % POS_BASIC_HEIGHT;
			siY = siTmpY + POS_BASIC_Y;

			for (SI32 i = 0 ; i < NOTEXISTPOSCOUNT ; i++)
			{
				if( clNoExistPos[i].siX == 0)
					break;

				if ( clNoExistPos[i].siX == siTmpX && clNoExistPos[i].siY == siTmpY)
				{
					bRelocation = true;
				}
			}

			if ( true == bRelocation ) 
			{
				--siMakeCount;
				continue;
			}
			
			if ( siNoExistArrayIndex < NOTEXISTPOSCOUNT)
				clNoExistPos[siNoExistArrayIndex++].Set(siTmpX,siTmpY);
		}

		// 레벨 결정
		if( m_uiCanEnterMaxLevel >= m_uiCanEnterMinLevel)
			siLevel = (rand() % ( m_uiCanEnterMaxLevel -  m_uiCanEnterMinLevel)) + m_uiCanEnterMinLevel;
		else
			siLevel = m_uiCanEnterMinLevel;

		// 몹 종류 결정
		switch(rand() %MOBKINDCOUNT)
		{
		case 0:
			{	
				siMobKind = m_siKindBlackarmySword;
				break;
			}
		case 1:
			{
				siMobKind = m_siKindBlackarmyRifle;
				break;
			}
		case 2:
			{
				siMobKind = m_siKindBlackarmyKatar;
				break;
			}
		default:
			{
				siMobKind = m_siKindBlackarmySword;
				break;
			}
		}

		cltMapCommon* pclMap = m_pclServer->pclMapManager->GetMapPointer( m_siMapIndex )	;	// 맵포인터
		if(pclMap != NULL)
		{
			// 생성될 몹 정보 입력
			// 몹 유닛 생성
			siMobID = m_pclServer->SetNPCChar( siMobKind, 
				VILLAGE_NONE, 
				&clIdentity, 
				siLevel, 
				0, 
				siX, 
				siY, 
				m_siMapIndex, 
				NPCITEMMODE_NONE, 
				&clinnerstatus );
			m_MobCharIDArray[siMakeCount] = siMobID;
		}
	}
	return TRUE;
}

// 결과를 알린다.
BOOL	CBattleRoyalStadiumServer::NoticeResult()
{
	bool bresult = false;
	const TCHAR* pszCharName = NULL;

	if( NULL != m_pclWinner)
	{	
		bresult = true;
		pszCharName = m_pclWinner->GetName();
	}
	
	cltGameMsgResponse_BattleRoyalNoticeResult clNotice(pszCharName, bresult, m_siMapIndex);
	
	cltMsg clMassage(GAMEMSG_RESPONSE_BATTLEROYAL_NOTICERESULT, sizeof( clNotice ), (BYTE*)&clNotice );
	m_pclServer->pclCM->SendAllPersonMsg((sPacketHeader*)&clMassage);
	
	return TRUE;
}

// 참가자를 경기장에서 강퇴시킨다.
BOOL	CBattleRoyalStadiumServer::BanGamer()
{
	// 경기장에 있던 사용자를 엔펠리스로 워프 시킨다.
	cltCharServer* pclCharServer = NULL;
	for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
	{
		pclCharServer = pclClient->pclCM->GetCharServer(id);
		if( NULL == pclCharServer)
			continue;

		if(  m_siMapIndex == pclCharServer->GetMapIndex() && pclCharServer->siKilledMode != KILLEDMODE_BATTLEROYAL)
		{
			SI32		siWarpX = 502;
			SI32		siWarpY = 258;
			cltWarpInfo clWarp(1, MAPINDEX_MAINFIELD, false, false, siWarpX, siWarpY, -1, -1);

			pclCharServer->SetWarpInfo( &clWarp, 1024 );
			pclCharServer->Warp();
		}
	}
	return TRUE;
}

// 경기에 참가중인 유저 수를 보여준다.
UI32	CBattleRoyalStadiumServer::GetWaitGamerNum()
{
	cltCharServer* pclCharServer = NULL;
	UI32	uiCount = 0;
	for (SI32 siID = 0; siID < MAX_PERSON_NUMBER; siID++)
	{
		pclCharServer = m_pclServer->pclCM->GetCharServer(siID);
		if( NULL == pclCharServer)
			continue;

		if(m_siMapIndex == pclCharServer->pclCI->clBI.GetMapIndex())
		{
			++uiCount;
		}
	}

	return uiCount;
}

// 마지막 경기에 참가 했었는지?
BOOL	CBattleRoyalStadiumServer::WasJoinedLastGame(SI32 siPersonID)
{
	for(SI32 i = 0 ; i < MAX_GAMER_COUNT; i++)
	{
		if( siPersonID == m_BeforeGamerArray.GetData(i))
		{
			return TRUE;
		}
	}
	return FALSE;
}
// 참가자 수가 한계치인가?
BOOL	CBattleRoyalStadiumServer::IsLimitedGamerNum()
{
	if( GetGamerNum() >= MAX_GAMER_COUNT)
	{
		return TRUE;
	}

	return FALSE;
}
// 경기에 참가할 유저 수
UI32	CBattleRoyalStadiumServer::GetGamerNum()
{
	return m_siGamerCount;
}
// 경기에 참가가 중인 유저 수
UI32	CBattleRoyalStadiumServer::GetGamingGamerNum()
{
	return m_siGamingUserCount;
}
NkVector<CBattleRoyalPersonChangedData>& CBattleRoyalStadiumServer::GetPersonChangedData()
{
	return m_GamerArray;
}
NkVector<SI32>& CBattleRoyalStadiumServer::GetMobData()
{
	return m_MobCharIDArray;
}

// 데이터 초기화
void	CBattleRoyalStadiumServer::InitData()
{
	m_GamerArray.ClearData();
	m_MobCharIDArray.ClearData();
	m_siGamerCount = 0;
	m_siGamingUserCount = 0;
	m_pclWinner = NULL;
	m_siMobCount = 0;
}

void	CBattleRoyalStadiumServer::ProcessLogoutCharByIndex(SI32 siIndex)
{
	if( (siIndex < 0) || (MAX_GAMER_COUNT <= siIndex))
		return ;
	if( NULL == m_pclServer)
		return ;

	CBattleRoyalPersonChangedData& pclPersonData =  m_GamerArray.GetData(siIndex);

	if( pclPersonData.GetPersonID() <= 0)
		return;

	SI32 siCharID =  m_pclServer->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
	cltCharServer* pclChar = m_pclServer->pclCM->GetCharServer(siCharID) ;

	if( NULL == pclChar)
	{
		DeleteGamer(pclPersonData.GetPersonID());
	}
	else
	{
		if( pclChar->pclCI->GetPersonID() != pclPersonData.GetPersonID())
		{
			DeleteGamer(pclPersonData.GetPersonID());
		}
	}
}

cltCharServer*	CBattleRoyalStadiumServer::GetGamingCharByIndex(SI32 siIndex)
{
	if( (siIndex < 0) || (MAX_GAMER_COUNT <= siIndex))
		return NULL;
	if( NULL == m_pclServer)
		return NULL;

	CBattleRoyalPersonChangedData& pclPersonData = m_GamerArray.GetData(siIndex);
	
	SI32 siCharID =  m_pclServer->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
	return m_pclServer->pclCM->GetCharServer(siCharID);
}

// 결과를 처리한다. (결과가 있는 경우 TRUE 처리 오류인 경우 FALSE을 반환한다.)
BOOL	CBattleRoyalStadiumServer::ProcessResult()
{
	m_pclWinner = NULL;

	SI32 siRemainedCharCount = 0 ;

	if( m_siGamingUserCount >= 1)
	{
		cltCharServer* pclChar = NULL;
		SI32 siCharID = 0;
		
		for(SI32 i = 0 ; i < MAX_GAMER_COUNT; i++)
		{
			CBattleRoyalPersonChangedData& pclPersonData =  m_GamerArray.GetData(i);
			
			siCharID =  m_pclServer->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
			pclChar = m_pclServer->pclCM->GetCharServer(siCharID);
			if(NULL == pclChar)
				continue;

			if((pclChar->GetMapIndex() == m_siMapIndex) && pclChar->siKilledMode != KILLEDMODE_BATTLEROYAL)
			{
				m_pclWinner = pclChar;
				++siRemainedCharCount;
			}
		}
	}
	
	if( siRemainedCharCount > 1)
		m_pclWinner = NULL;


	if( (m_siMobCount > 0) )
		m_pclWinner = NULL;

	return TRUE;
}
// 경기에 존재하는 몹의 수
UI32	CBattleRoyalStadiumServer::GetMobNum()
{
	return m_siMobCount;
}
// 몹 죽음
void	CBattleRoyalStadiumServer::DieMob()
{
	--m_siMobCount;
}

//====================================================================================================================================
CBattleRoyalStadiumClient::CBattleRoyalStadiumClient()
{
	m_pclClient = (cltClient*)pclClient;
}

CBattleRoyalStadiumClient::~CBattleRoyalStadiumClient()
{

}

// 참가자 퇴장
BOOL	CBattleRoyalStadiumClient::DeleteGamer(SI32 siPersonID)
{
	return TRUE;
}
// 결과를 알린다.
BOOL	CBattleRoyalStadiumClient::NoticeResult()
{
	

	return TRUE;
}
