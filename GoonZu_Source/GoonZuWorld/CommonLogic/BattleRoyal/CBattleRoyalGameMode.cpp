#include "./CBattleRoyalGameMode.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../common/char/CharServer/Char-Server.h"
#include "../CommonLogic/Msg/MsgType-BattleRoyal.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../Server/server.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"

CBattleRoyalGameMode::CBattleRoyalGameMode()
{
	m_pclPersonChangedData = NULL;
	m_siCountChangePowerupChar = 0;
	m_siGmGameMode = 0;
}
CBattleRoyalGameMode::~CBattleRoyalGameMode()
{

}
// GameMode 적용 시키기
BOOL	CBattleRoyalGameMode::Apply()
{
	// 시작 전송
	if( NULL == m_pclPersonChangedData)
		return FALSE;

	switch(m_siMode)
	{
	case BATTLEROYALGAMEMODE_RESTRICTTOUSEHEALITEM:
		{
			RestrictToUseHealItem();
			break;
		}
	case BATTLEROYALGAMEMODE_ONEKILL:
		{
			OneKill();
			break;
		}
	case BATTLEROYALGAMEMODE_POWERUPONEUSER:
		{
			PowerupOneUser();
			break;
		}
	case BATTLEROYALGAMEMODE_NORMAL:
	default:
		{
			break;
		}
	}

	ChangeCharBasicInfo();
	//TransformChar();

	cltCharServer* pclChar = NULL;
	SI32 siCharID = 0;
	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);
		
		siCharID =  pclClient->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if(NULL == pclChar)
			continue;

		cltGameMsgResponse_BattleRoyalStartGame clStartGame(m_siGameTime, m_siMode);

		cltMsg clMassage(GAMEMSG_RESPONSE_BATTLEROYAL_STARTGAME, sizeof( clStartGame ), (BYTE*)&clStartGame );
		pclChar->SendNetMsg((sPacketHeader*)&clMassage);
	}

	return TRUE;
}

SI32	CBattleRoyalGameMode::GetMode()
{
	return m_siMode;
}

// "힐링 아이템 사용 제한" 모드
BOOL	CBattleRoyalGameMode::RestrictToUseHealItem()
{
	
	return TRUE;
}
// "ONEKILL" 모드
BOOL	CBattleRoyalGameMode::OneKill()
{
	if( NULL == m_pclPersonChangedData)
		return FALSE;
	if( NULL == m_pclMobData)
		return FALSE;

	// pc
	cltCharServer* pclChar = NULL;
	SI32 siCharID = 0;
	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);

		siCharID =  pclClient->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if(NULL == pclChar)
			continue;

		pclChar->bOneKill = true;
	}

	// npc
	for(SI32 i = 0 ; i < CBattleRoyalStadium::MAX_UNIT_COUNT ; i++)
	{
		if(!IsNPC(m_pclMobData->GetData(i)))
			continue;

		pclChar = pclClient->pclCM->GetCharServer(m_pclMobData->GetData(i));
		if(NULL == pclChar)
			continue;

		pclChar->bOneKill = true;
	}

	return TRUE;
}
// "한 유저 파워 업" 모드
BOOL	CBattleRoyalGameMode::PowerupOneUser()
{
	if( NULL == m_pclPersonChangedData)
		return FALSE;

	const SI32 MAX_LOOP_COUNT = 10;

	SI32 siGameUserArray[CBattleRoyalStadiumServer::MAX_GAMER_COUNT];
	ZeroMemory(siGameUserArray, CBattleRoyalStadiumServer::MAX_GAMER_COUNT * sizeof(SI32) );
	SI32 siGameUserCount = 0;

	cltCharServer* pclChar = NULL;
	SI32 siCharID = 0;
	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);

		siCharID =  pclClient->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if(NULL == pclChar)
			continue;
		
		siGameUserArray[siGameUserCount++] = pclPersonData.GetPersonID();

		if( (true == pclChar->bBattleRoyalPowerupMode))
		{
			if( KILLEDMODE_BATTLEROYAL != pclChar->siKilledMode)
				return TRUE;
		}
		
	}

	if (m_siCountChangePowerupChar > LIMIT_COUNTERCHANGEPOWERUPCHAR)
	{
		return FALSE;
	}

	pclChar = NULL;
	SI32 siPersonID = 0;
	SI32 siIndex = 0;

	if( siGameUserCount == 0)
		return FALSE;

	for(SI32 i = 0 ; i < MAX_LOOP_COUNT; i++)
	{
		siIndex = rand() % siGameUserCount;

		siPersonID = siGameUserArray[siIndex];

		siCharID =  pclClient->pclCM->GetIDFromPersonID(siPersonID);
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if( NULL == pclChar)
			continue;

		if( pclChar->siKilledMode != KILLEDMODE_BATTLEROYAL)
		{
			pclChar->bBattleRoyalPowerupMode = true;

			cltGameMsgResponse_BattleRoyalPowerupMode clinfo( pclChar->GetCharUnique() , true);
			cltMsg clMsg(GAMEMSG_RESPONSE_BATTLEROYAL_POWERUPMODE, sizeof(cltGameMsgResponse_BattleRoyalPowerupMode), (BYTE*)&clinfo );
			pclChar->SendNeighbourMsg(&clMsg,true);
			++m_siCountChangePowerupChar;
			return TRUE;
		}
		else
		{
			continue;
		}
	}
	
	return TRUE;
}
// 모드 결정
BOOL	CBattleRoyalGameMode::DetermineMode()
{
	m_siCountChangePowerupChar = 0;

	if ( m_siGmGameMode == BATTLEROYALGAMEMODE_START )
	{
		srand((unsigned ) time(NULL));
		m_siMode = (rand() % BATTLEROYALGAMEMODE_COUNT) + 1;
	}
	else
	{
		m_siMode = m_siGmGameMode;
	}
	return TRUE;
}
// 게임 시 변경이 필요한 기본적인 캐릭터 정보 변경
BOOL	CBattleRoyalGameMode::ChangeCharBasicInfo()
{
	if( NULL == m_pclPersonChangedData)
		return FALSE;

	cltCharServer* pclChar = NULL;
	SI32 siCharID = 0;
	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);

		siCharID =  pclClient->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if(NULL == pclChar)
			continue;
		
		pclPersonData.SetCharMode(pclChar->siCharMode);
		
		pclChar->siCharMode = CHARMODE_BATTLEROYAL;

	}
	return TRUE;
}

// 게임 종료, 게임 이탈 시 변경된 캐릭터 정보를 되돌려놓는다.
BOOL	CBattleRoyalGameMode::RestoreCharInfo(cltCharServer* pclChar)
{
	if( NULL == m_pclPersonChangedData)
		return FALSE;

	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);

		if( pclPersonData.GetPersonID() == pclChar->pclCI->GetPersonID())
		{
			pclChar->siCharMode = pclPersonData.GetCharMode();
			
			pclChar->bOneKill = false;
			pclChar->bBattleRoyalPowerupMode = false;

			cltGameMsgResponse_BattleRoyalPowerupMode clinfo( pclChar->GetCharUnique() , false);
			cltMsg clMsg(GAMEMSG_RESPONSE_BATTLEROYAL_POWERUPMODE, sizeof(cltGameMsgResponse_BattleRoyalPowerupMode), (BYTE*)&clinfo );
			pclChar->SendNeighbourMsg(&clMsg,true);

			return TRUE;
		}
	}
	return FALSE;
}

// 특정 유저를 몹으로 변신
BOOL	CBattleRoyalGameMode::TransformChar(cltCharServer* pclChar)
{
	if(NULL == pclChar)
		return FALSE;

	SI32 siKind = 0;
	SI32 siWeaponType = pclChar->pclCI->clCharItem.clItem[ pclChar->ActiveWeapon ].GetType( pclClient->pclItemManager );

	switch(siWeaponType)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	default:
		{
			SI32 siFlag = rand() % 2;
			if(siFlag)
				siKind = m_siKindBlackarmySword;
			else
				siKind = m_siKindBlackarmyKatar;
			break;
		}
	case ITEMTYPE_GUN:
	case ITEMTYPE_BOW:
	case ITEMTYPE_STAFF:
		{
			siKind = m_siKindBlackarmyRifle;
			break;
		}
	}

	SI32 siUseTime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 6;		//현실시간 12분간

	SI16 siIndex = -1;
	siIndex = pclChar->pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );	

	// 기존에 변신 버프가 걸려있으면 지워준다.
	if( -1 >= siIndex )
	{
		siIndex = pclChar->pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
	}

	if( -1 >= siIndex)
		return FALSE;

	stBuf addBuf(BUF_TRANSFORM, siKind, siUseTime);

	pclChar->pclCI->clBI.clBufInfo.AddBuf( siIndex, addBuf );

	// 클라이언트에게도 통보한다.
	cltGameMsgResponse_AddBuf clinfo( pclChar->GetCharUnique(), siIndex, addBuf );
	cltMsg clMsg(GAMEMSG_RESPONSE_ADDBUF, sizeof(cltGameMsgResponse_AddBuf), (BYTE*)&clinfo );
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);

	return TRUE;
}

// 유저를 몹으로 변신
BOOL	CBattleRoyalGameMode::TransformChar()
{
	if( NULL == m_pclPersonChangedData)
		return FALSE;

	cltCharServer* pclChar = NULL;
	
	SI32 siWeaponType = 0;
	SI32 siFlag = 0;
	SI32 siKind = 0;
	SI32 siCharID = 0;
	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);

		siCharID =  pclClient->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if(NULL == pclChar)
			continue;
		
		siWeaponType = pclChar->pclCI->clCharItem.clItem[ pclChar->ActiveWeapon ].GetType( pclClient->pclItemManager );

		switch(siWeaponType)
		{
		case ITEMTYPE_SWORD:
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_AXE:
		default:
			{
				siFlag = i % 2;
				if(siFlag)
					siKind = m_siKindBlackarmySword;
				else
					siKind = m_siKindBlackarmyKatar;
				break;
			}
		case ITEMTYPE_GUN:
		case ITEMTYPE_BOW:
		case ITEMTYPE_STAFF:
			{
				siKind = m_siKindBlackarmyRifle;
				break;
			}
		}
		
		SI32 siUseTime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 6;		//현실시간 12분간
		
		SI16 siIndex = -1;
		siIndex = pclChar->pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );	
		
		// 기존에 변신 버프가 걸려있으면 지워준다.
		if( -1 >= siIndex )
		{
			siIndex = pclChar->pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
		}
		
		pclPersonData.SetTransformBufIndex(siIndex);

		stBuf addBuf(BUF_TRANSFORM, siKind, siUseTime);

		pclChar->pclCI->clBI.clBufInfo.AddBuf( siIndex, addBuf );

		// 클라이언트에게도 통보한다.
		cltGameMsgResponse_AddBuf clinfo( pclChar->GetCharUnique(), siIndex, addBuf );
		cltMsg clMsg(GAMEMSG_RESPONSE_ADDBUF, sizeof(cltGameMsgResponse_AddBuf), (BYTE*)&clinfo );
		pclChar->SendNetMsg((sPacketHeader*)&clMsg);

		// 이웃에게 통보 
		cltCharChangeStatusInfo	StatusInfo( CHANGETYPE_TRANSFORMBUF, false, (SI32)addBuf.siPercent );
		cltGameMsgResponse_StatusInfo clChangeInfo(pclChar->GetCharUnique(), &StatusInfo);
		cltMsg clMsg2(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		pclChar->SendNeighbourMsg(&clMsg2, true);

	}
	return TRUE;
}
// 몹으로 변형된 유저를 원래 상태로 
BOOL	CBattleRoyalGameMode::RestoreTrancsformChar(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return FALSE;

	SI32 siIndex = 0 ;

	
	siIndex = pclChar->pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );	

	if ( -1 >= siIndex)
		return FALSE;

	pclChar->pclCI->clBI.clBufInfo.DeleteBuf(siIndex);

	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_TRANSFORMBUF, false, 0);
	cltGameMsgResponse_StatusInfo clChangeInfo(pclChar->GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	pclChar->SendNeighbourMsg(&clMsg, true);

	return TRUE;
}
void	CBattleRoyalGameMode::SetGamers(NkVector<CBattleRoyalPersonChangedData>& rclPersonChangedData, SI32	siCount
										,NkVector<SI32>& rclMobData)
{
	m_pclPersonChangedData = &rclPersonChangedData;
	m_siPersonDataCount = siCount;
	m_pclMobData = &rclMobData;
}
// 유저를 보여준다.
BOOL	CBattleRoyalGameMode::ChangeVisibleMode()
{
	if( NULL == m_pclPersonChangedData)
		return FALSE;

	cltCharServer* pclChar = NULL;
	SI32 siCharID = 0;
	for(SI32 i = 0 ; i < m_siPersonDataCount; i++)
	{
		CBattleRoyalPersonChangedData& pclPersonData = m_pclPersonChangedData->GetData(i);

		siCharID =  pclClient->pclCM->GetIDFromPersonID(pclPersonData.GetPersonID());
		pclChar = pclClient->pclCM->GetCharServer(siCharID);
		if(NULL == pclChar)
			continue;

		pclChar->pclCI->clBI.uiGMMode = GMMODE_ATB_NONE;

		pclChar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
	}
	return TRUE;
}
// 유저를 가려준다.
BOOL	CBattleRoyalGameMode::ChangeInvisibleMode(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return FALSE;

	pclChar->pclCI->clBI.uiGMMode = GMMODE_ATB_HIDE;

	return TRUE;
}

void	CBattleRoyalGameMode::SetMode(SI32 siMode)
{
	m_siMode = siMode;
}
void	CBattleRoyalGameMode::SetGameTime(SI32 siGameTime)
{
	m_siGameTime = siGameTime;
}
BOOL	CBattleRoyalGameMode::SetGMGameMode(SI32 siMode)
{
	if( siMode >= BATTLEROYALGAMEMODE_END || siMode < BATTLEROYALGAMEMODE_START)
		return FALSE;
	m_siGmGameMode = siMode;
	return TRUE;
}

BOOL	CBattleRoyalGameMode::TakeinHorse(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return FALSE;
	
	 SI32 siHorseIndex = pclChar->pclCI->clHorseInfo.GetCurrentShowHorseIndex();

	 if(siHorseIndex < 0)
		return FALSE;

	 cltHorse* pclHorse = pclChar->pclCI->clHorseInfo.GetCurrentShowHorse();

	 if(pclHorse == NULL)
		 return FALSE;

	 if(0 == pclHorse->siHorseUnique)
		 return FALSE;

	 // DB에 말을 탔음을 알린다. 
	 sDBRequest_SetRideHorse cldbmsg(pclChar->GetCharID(), pclChar->pclCI->GetPersonID(), siHorseIndex , true);
	 ((cltServer*)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&cldbmsg);

	return TRUE;

}
