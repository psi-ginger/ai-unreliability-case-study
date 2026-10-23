//---------------------------------
// 2003/11/22 김태곤
//---------------------------------

#include "Client.h"


#include "Music\Music.h"

#include "MsgType.h"

#include "..\Common\Bullet\Bullet.h"
#include "..\Common\Smoke\Smoke.h"
#include "..\CommonLogic\Hunt\Hunt.h"
#include "..\CommonLogic\MagicMgr\MagicMgr.h"
#include "..\CommonLogic\MsgType-Person.h"
#include "../common/Order/order.h"
#include "Msg\MsgType-Quest.h"
#include "MsgType-Item.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "NInterface/NMiniMapDlg/NMiniMapDlg.h"
#include "NInterface/NChatDlg/NChatDlg.h"
#include "NInterface/NNewInventory/NNewInventory.h"
#include "ninterface/nnewquestdlg/NNewQuestDlg.h"
#include "Interface/TotalChatDlg/TotalChatDlg.h"
#include "PCRoom/PCRoomIntro.h"
#include "Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"

// APEX-
#include "../Apex/ApexGoonZu.h"
// -APEX

#include "../Client/MiniHomeUpdates/PopupCheck.h"

#include "../Client/ninterface/NMapNameDlg/NMapNameDlg.h"

#include "..\CommonLogic\Msg\MsgType-NPC.h"

extern CSoundOnOffDlg g_SoundOnOffDlg;
extern bool bSetScreenCenterSwitch;

static bool bGameStart;

// 로그인. (복수 캐릭터. ) 
void cltClient::DoMsg_GAMEMSG_RESPONSE_LOGINMANY(cltMsg* pclMsg, SI32 id)
{
	cltMakeCharInfoCommon* pclinfo = NULL;
	cltMakeCharInfo clinfo;
	SI32 index = 0;

	bool loop = true;
	while(loop)
	{
		pclinfo = (cltMakeCharInfoCommon*)&pclMsg->cData[index];
		// 사용자 캐릭터 정보. 
		if(pclinfo->uiType == MAKECHARTYPE_PC)
		{
			memcpy(&clinfo, pclinfo, sizeof(cltMakeCharInfo));
			index+= sizeof(cltMakeCharInfo);
		}
		// NPC캐릭터 정보. 
		else
		{

			clinfo.Set(pclinfo);
			index+= sizeof(cltMakeCharInfoCommon);
		}
		
		
		// 캐릭터를 만들 수 있도록 버퍼에 넣어준다. 
		PushChar(&clinfo);

		
		if(index >= pclMsg->GetDataSize() )break;

	}
}

// 로그인. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_LOGIN(cltMsg* pclMsg, SI32 id)
{
	cltMakeCharInfo* pclinfo = (cltMakeCharInfo*)pclMsg->cData;
	
	// 캐릭터를 만들 수 있도록 버퍼에 넣어준다. 
	PushChar(pclinfo);

	//------------------------------------------------
	// 군주의 로그인이면 효과음을 연주한다. 
	//------------------------------------------------
	static bool bkingeffectswitch = false;
	SI32 armourunique = pclinfo->clShapeEquip.clArmour.uiUnique;
	if(bkingeffectswitch == false)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(armourunique);
		if(ref > 0 && pclItemManager->pclItemInfo[ref]->siNeedRank == 2)
		{
			bkingeffectswitch = true;

			PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_KING_LOGIN")), 0, 0);

			TCHAR* pText  = GetTxtFromMgr(817);

			pclMessage->SetMsg(pText);
		}
	}

	// 주인공 캐릭터 정보가 넘어온 것이라면, 
	if((SI32)pclinfo->uiMakeCharUnique != MyCharUnique)return ;

	pclCM->CR[id]->clPB.clShapeEquip.Set(&pclinfo->clShapeEquip);
	
	//--------------------------
	// 총알, 연기등을 모두 지운다.
	//---------------------------
	pclSmokeManager->DeleteSmoke();
	pclBulletManager->DeleteBullet();
	
	//---------------------------
	// 마법을 지운다.
	//---------------------------
	pclMagicManager->DeleteMagic();
	
	//---------------------------------
	// 주인공 캐릭터의 정보를 백업한다. 
	//---------------------------------
	if(bReceiveMyCharAllInfoSwitch == true)
	{
		// 캐릭터 정보를 백업했다. 
		bBackUpSwitch	= true;
		
		cltCharClient* pclchar = (cltCharClient*)pclCM->CR[1];
		
		pclHeroDataBackup->clMyCharPersonInfo.Set(pclCM->CR[1]->pclCI);
		
		// 전쟁에서 처치한 적의 수도 백업. 
		pclHeroDataBackup->siWarEnemyKillNumber				= pclCM->CR[1]->siWarEnemyKillNumber;
		pclHeroDataBackup->siWarKilledNumber				= pclCM->CR[1]->siWarKilledNumber;
		pclHeroDataBackup->siBlackArmyWarMapIndex			= pclCM->CR[1]->siBlackArmyWarMapIndex;

		pclHeroDataBackup->siBlackWarEnemyKillNumber		= pclCM->CR[1]->siBlackWarEnemyKillNumber;
		pclHeroDataBackup->siBlackWarKilledNumber			= pclCM->CR[1]->siBlackWarKilledNumber;
		
		//[추가 : 황진성 2008. 1. 28 => 흑의 군단 개편 정보 백업]
		pclHeroDataBackup->siBlackWarEnemyKillScore			= pclCM->CR[1]->siBlackWarEnemyKillScore;
		pclHeroDataBackup->siBlackWarEnemyKillVillageScore	= pclCM->CR[1]->siBlackWarEnemyKillVillageScore;
		
		pclHeroDataBackup->bStartMakeItemSwitch				= pclchar->bStartMakeItemSwitch;
		pclHeroDataBackup->dwStartMakeItemClock				= pclchar->dwStartMakeItemClock;
		pclHeroDataBackup->dwDelayMakeItemClock				= pclchar->dwDelayMakeItemClock;
		
		//------------------------------------------------------------------------------------------
		pclHeroDataBackup->siOldActiveClothes				= pclchar->ActiveClothes;
		pclHeroDataBackup->siOldActiveHat					= pclchar->ActiveHat;
		pclHeroDataBackup->siOldActiveWeapon				= pclchar->ActiveWeapon;
		pclHeroDataBackup->siOldCurrnetMagic				= pclchar->siCurrentMagic;
		pclHeroDataBackup->siOldCurrnetMagicCastType		= pclchar->siCurrentMagicCastType;

		//------------------------------------------------------------------------------------------
		pclHeroDataBackup->siOldBasicMoveSpeed				= pclchar->clPB.BasicMoveSpeed;
		pclHeroDataBackup->siOldShoesMoveSpeed				= pclchar->clPB.ShoesMoveSpeed;
		pclHeroDataBackup->siOldTotalMoveSpeed				= pclchar->clPB.TotalMoveSpeed;
		pclHeroDataBackup->siOldGivenMoveSpeed				= pclchar->clPB.GivenMoveSpeed;

/*
		pclHeroDataBackup->siGuildOldStr			= pclchar->clPB.siGuildStr;
		pclHeroDataBackup->siGuildOldDex			= pclchar->clPB.siGuildDex;
		pclHeroDataBackup->siGuildOldMag			= pclchar->clPB.siGuildMag;
		pclHeroDataBackup->siGuildOldVit			= pclchar->clPB.siGuildVit;
		pclHeroDataBackup->siGuildOldHnd			= pclchar->clPB.siGuildHnd;
		pclHeroDataBackup->siGuildOldLuck			= pclchar->clPB.siGuildLuck;
*/
		pclHeroDataBackup->clGuildOldBA.Set(&pclchar->clPB.clGuildBA);

		//------------------------------------------------------------------------------------------
		
		
		//이정보는 저장한후 복구 하는 정보가 아니다.
		pclHeroDataBackup->clOldAddInfo.Set(&pclchar->clAddInfo);
		pclHeroDataBackup->clOldItemMall.Set(&pclchar->pclCI->clItemMall);
		//pclHeroDataBackup->clOldLimitDateItemInfo.Set(&pclchar->pclCI->clLimitDateItem);
		
		//------------------------------------------------------------------------------------------
		// 위치 정보는 새로 받은 정보를 가지고 업데이트 한다. 
		pclHeroDataBackup->clMyCharPersonInfo.clBI.clPos.siMapIndex		= pclinfo->uiMapIndex;
		pclHeroDataBackup->clMyCharPersonInfo.clBI.clPos.siX			= pclinfo->uiX;
		pclHeroDataBackup->clMyCharPersonInfo.clBI.clPos.siY			= pclinfo->uiY;
		
		//------------------------------------------------------------------------------------------
		// 체력 정보도 새로 받은 정보를 가지고 업데이트 한다. 
		if(pclHeroDataBackup->clMyCharPersonInfo.clIP.GetLife() <= 0)
		{
			pclHeroDataBackup->clMyCharPersonInfo.clIP.SetLife(1);
		}

		//[진성] 공간상자.  워프등의 캐릭터 초기화가 이루어질 때 백업된 정보를 쉐팅. => 2008-4-3
		if(pclchar->pclSpaceBoxItem)	memcpy(&pclHeroDataBackup->clSpaceBoxItem,    pclchar->pclSpaceBoxItem,	sizeof(pclHeroDataBackup->clSpaceBoxItem));
		if(pclchar->pclTreasureBoxItem)	memcpy(&pclHeroDataBackup->clTreasureBoxItem, pclchar->pclTreasureBoxItem,	sizeof(pclHeroDataBackup->clTreasureBoxItem));

		pclHeroDataBackup->clOldMagicBufMgr.Set( &pclchar->m_clMagicBufMgr );
		// 채팅 금지 제한시간 복사
		memcpy(&pclHeroDataBackup->stMuteTime, &pclchar->m_stMuteTime, sizeof(pclHeroDataBackup->stMuteTime));

		//. 발렌타인데이 전쟁 
		pclHeroDataBackup->clValentinewarinfo.Set( pclchar->pclValentineWar);
		
		pclHeroDataBackup->clPVP_LeagueInfo.Set( pclchar->pclPVP_LeagueInfo);
	}	
	//--------------------------
	// 기존의 캐릭터를 모두 지운다. 
	//--------------------------
	pclCM->DeleteChar();

	
	//--------------------------
	// 기존의 진행중 퀘스트 정보를 지운다.
	//--------------------------
	if(pclQuestManager)
	{
		pclQuestManager->siCurrentQuestUnique = 0;
	}
	//--------------------------
	// 기존 지도를 제거한다. 
	//--------------------------
	// 지도들을 제거하기전의 상태를 기억한다.
	bool bTempSmallMapSwitch = bSmallMapSwitch;
	
	pclMapManager->DeleteAllMap();

	pclMapManager->pclCurrentMap = NULL;		// 반드시 초기화 되어야 한다. 
	

	//-----------------------------------
	// MapIndex를 가지고 지도를 읽어온다.
	//------------------------------------
	pclMapManager->LoadMap(this, pclinfo->uiMapIndex, pclinfo->siMapUnique);
	
	pclMapManager->SetCurrentMapIndex(pclinfo->uiMapIndex);

	// 화면 출력의 중심 좌표를 다시 설정하도록 만든다. 
	bSetScreenCenterSwitch = true;
	
	// 만들도록 예약되어 있는 캐릭터를 만든다. (주인공 캐릭터) 
	SetCharReservedChar();

	// 주인공 캐릭터의 정보가 완전하지 않기 때문에 다시 처리하도록 설정한다. - by LEEKH 2009-03-20
	pclCM->GetCharClient(1)->clPB.bSetTotalBA	= false;

	// 미니맵을 보고 있었으면 계속보여준다.
	bSmallMapSwitch = bTempSmallMapSwitch; // 지도들을 제거하기전의 상태로 복원한다.
	if(bSmallMapSwitch)
	{	
		if ( m_pDialog[ NMINIMAP_DLG ] == NULL )
		{
			CreateInterface( NMINIMAP_DLG );
		}			
	}

	
	
	
	//--------------------------------------------------------
	// 불필요한 인터페이스를 닫는다. 
	//--------------------------------------------------------
	DestroyNeedlessInterface();

	//--------------------------------------------------------
	// 지도가 마을이면 서버에 마을의 건물 데이터를 요청한다.
	//--------------------------------------------------------
	
	// 지도가 마을인지 확인한다. 
	if(pclMapManager->pclCurrentMap->siVillageUnique)
	{
		cltMsg clMsg(GAMEMSG_REQUEST_VILLAGESTRUCT, pclMapManager->pclCurrentMap->siVillageUnique);
		SendMsgToServer((sPacketHeader*)&clMsg);
		
		// 마을의 건물 정보를 받기 전까지는 캐릭터를 만들어서는 안된다. 왜냐하면 
		// 캐릭터가 만들어진 후에 건물 정보가 오면 캐릭터가 건물 속에 파묻히는 
		// 경우가 발생하기 때문이다. 따라서 건물 정보가 오기를 기다린다는 
		// 스위치가 작동해야 한다. 
		bWaitVillageStructureInfoSwitch		= true;
	}
	
	if(bReceiveMyCharAllInfoSwitch == false)
		bGameStart = true;
	else
		bGameStart = false;

	// 파일로부터 얻어온 환경 설정 정보 중 세팅이 안된 Server나 음악 정보를 셋팅한다.
	if( bGameStart )
	{
		bGameStart = false ;

		if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NSOUNDONOFF_DLG ] )
		{
            ((CNSoundOnOffDlg*)(m_pDialog[ NSOUNDONOFF_DLG ]))->SendCurrentSettingInfo();
		}
		else
		{
            g_SoundOnOffDlg.SendCurrentSettingInfo();
		}

		PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WELCOME_TO_GOONZU")),0,0);
	}

	//----------------------------------------
	// 음악을 연주한다. 
	//----------------------------------------
	SI32 mapindex = pclMapManager->pclCurrentMap->siMapIndex;
	PlayBackgroundMusic(mapindex);
	
	// 아이템 몰 아이템이 있는지 요청한다.
	{
		pclCM->CR[1]->pclCI->clItemMall.Init();
		cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLEXISTENCE,0);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}

	OrderStop(1 , BY_PLAYER);

	// 인벤창 맥스모드 막음
	//CNNewInventory::m_bMax = false;


	if ( pclCM->CR[1]->pclCI->clIP.GetLevel() <= 3 && 
		pclMapManager->pclCurrentMap->siMapIndex == MAPINDEX_PALACE )
	{
		if(m_pDialog[ NNEWQUEST_DLG ] == NULL)
		{
			CreateInterface( NNEWQUEST_DLG );

			if(m_pDialog[ NNEWQUEST_DLG ])
			{
				pclQuestManager->bNewQuseOpened = TRUE;
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->m_siButtonType = 0;
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->SetNewQuest(GetUniqueFromHash("KIND_WAITINGMAN"));
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->SetOpenedQuestDlg( true );
			}
		}
	}
	if(pclClient->IsCountrySwitch(Switch_FreshManHelpDialog))
	{
		CreateInterface(NHELPBUTTON_DLG);
	}

	// [기형] 휴면 계정 (2009.08.18) - 클라이언트 접속시 휴면 정보 체크 후 포인트 다이얼로그 생성 유무 처리
	if ( TRUE == pclCM->CR[1]->pclCI->clDormancyInfo.GetDormancyState() )
	{
		CDormancySystem *pclDormancySystem = this->pclDormancySystem;
		if( NULL != pclDormancySystem)
		{
			CDormancyPointRewardClient *pclDormancyClient = (CDormancyPointRewardClient *)pclDormancySystem->GetDormancyPointReward();
			if(NULL != pclDormancyClient)
			{
				pclDormancyClient->CreatePointDlg();
			}
		}
	}

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	if(pclinfo->siCurrentHorseRIDEHORSE == true)
	{
		// 추종자를 삭제한다. 
		pclCM->CR[1]->DeleteSlave();
	}
	// APEX- 한번만 실행되게 한다.
#ifdef _CHINA
	if( false == m_bStartApex )
	{
		m_bStartApex = true;
		StartApexClient();
	}
#endif
	// -APEX

	// [영훈] 멥 이름 보여주기 (2008.06.17)
	if ( IsCountrySwitch(Switch_Show_MapName) )
	{
		if ( NULL == m_pDialog[ NMAPNAME_DLG ] )
		{
			CreateInterface( NMAPNAME_DLG );
		}

		((CNMapNameDlg*)m_pDialog[ NMAPNAME_DLG ])->ShowMapName( mapindex );
	}
}

// 로그아웃 . 
void cltClient::DoMsg_GAMEMSG_RESPONSE_LOGOUT(cltMsg* pclMsg, SI32 id)
{
	SI32 quitcharunique = pclMsg->siData32[0];
	SI32 quitid = cltCharCommon::CharUniqueInfo[quitcharunique];
			
	// 자기 자신을 로그아웃 시키라는 요청이면 무시한다. 자기 자신은 로그아웃 시킬 수 없다.
	if(quitid == 1)
	{
		return;
	}
	
	// 만들어진 캐릭터중에 존재하면 삭제한다.
	if(pclCM->IsValidID(quitid))
	{
		pclCM->DeleteChar(quitid);
	}

	// 생성될 캐릭터 리스트에 존재하면 삭제한다. 
	if(quitcharunique)
	{
		DeleteReservedChar(quitcharunique);
	}
}
