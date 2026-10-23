//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "../Common/CommonHeader.h"

#include "../Client/Client.h"

#include <Graphic.h>
#include <Mouse.h>
#include <MainFunction.h>
#include <Graphic_Capsule.h>
#include "../../Lib/NGraphics/Graphics.h"

//#include "../ColorTable/ColorTable.h"
#include "../../Lib/Sound/SoundMgr.h"
#include "../../MapEdit/MapEdit.h"

#include "../../Common/Map/Map.h"
#include "../../Common/Item/ItemCommon/cltSetItem.h"

#include "../../CommonLogic/BlackArmy/BlackArmy.h"
#include "../../CommonLogic/Guild/GuildWar.h"

#include <MoveMark.h>

#include "../AttackEffect/AttackEffectMgr.h"

#include "../Prompt/PromptBaseDefinition.h"
#include "../Prompt/PromptMgr.h"

#include "Bullet/Bullet.h"
#include "Smoke/Smoke.h"
#include "Char\CharManager\CharManager.h"

#include "../Effect/FireFly/FireFlyMgr.h"						// 반딧불

#include "../Effect/IceFlash/IceFlashMgr.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

#include "Help\Help.h"

#include "../ImageInterface/FriendLoginDraw/FriendLoginDraw.h"
#include "../ImageInterface/ResidentLoginDraw/ResidentLoginDraw.h"
#include "../ImageInterface/NaesiPortraitDraw/NaesiPortraitDraw.h"
#include "../ImageInterface/VillageNotice/VillageNotice.h"
#include "../ImageInterface/VillageWarNotice/VillageWarNotice.h"
#include "../ImageInterface/NewYearEvent/NewYearEvent.h"

#include "../Interface/Durability/Durability.h"
#include "../Interface/WebEventDlg/WebEventDlg.h"
#include "../Interface\LoginNoticeDlg\LoginNoticeDlg.h"


#include "../Interface/ServerOnStepNotifyDlg/ServerOnStepNotifyDlg.h"

#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../NInterface/NItemMallManager/NItemMallManager.h"
#include "../ninterface/nmagicinfodlg/MagicInfoDlg.h"
#include "../NInterface/NMiniMapDlg/NMiniMapDlg.h"
#include "../ninterface/NPremiumStatus/NPremiumStatus.h"
#include "../InterfaceMgr/InterfaceMgr.h"
#include "../FeastEffect/FeastEffectManager.h"

#include "../ninterface/NBuffStatus/NBuffstatus.h"
#include "../ninterface/NMagicCoolTimeDlg/NMagicCoolTimeDlg.h"
#include "../CommonLogic/Guild/GuildHuntMapWarStatMgr.h"

#include "PCRoom/PCRoomIntro.h"

#include "../ninterface/NPCRoom/NPCRoom_Display.h"

extern cltCommonLogic* pclClient;
extern SI32		gsiResolution;
extern SHORT	GameMouseX;
extern SHORT	GameMouseY;

bool cltClient::GameDrawHeroViewOperate()
{
	static UI32 LastDrawClock;
	static UI32 LastDrawFrame = 0;

	
	cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
	if(pclmap == NULL)return false;

/*	UI32 diff			= TABS(CurrentClock - LastDrawClock);
	if(diff < 50)return ;
	LastDrawClock  = CurrentClock;
*/

	//--------------------------------------
	// 게임 초반의 장면은 그려주지 않는다. 
	//--------------------------------------
	if(GetFrame() < 3 )
		return false;

	//------------------------------------------------
	// 같은 프레임에서 두번 출력하지 않는다. 
	//------------------------------------------------
	if(GetFrame() != 0 && LastDrawFrame == GetFrame())
		return false;
	LastDrawFrame = GetFrame();

	//-------------------------------------------------
	// 주인공 시점으로 게임을 진행하게 한다. 
	// MapEdit, Server에서는 사용되지 않아야 한다. 
	//-------------------------------------------------
	if(GameMode == GAMEMODE_CLIENT)
	{
		HeroViewOperate();
	}

	return true;
}

bool cltClient::GameDrawMap()
{	
	cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
	if(pclmap == NULL)return false;

	//-------------------------------------------------
	// 지도를 그린다. 
	//-------------------------------------------------
	if ( pclmap )
	{
		pclmap->DrawMap();
	}

	lpBackScreen->BltFast( 0, 0, lpScreenMapBuffer, NULL, NULL );

	//-------------------------------------------
	// 캐릭터 및 오브젝트 출력. 
	//-------------------------------------------
	if(GP.LockSurface(lpBackScreen)==TRUE)
	{
		// 캐릭터를 포함한 객체들을 그린다. 
		pclmap->DrawObj();

		if ( pclClient->GameMode == GAMEMODE_CLIENT )
		{
			if(pclMapManager)
			{
				if(pclMapManager->IsFogMap(pclmap->siMapIndex))
					pclmap->DrawFog(lpBackScreen,this);
			}			
		}

		GP.UnlockSurface(lpBackScreen);
	}

	//-------------------------------------------
	// 지도 지형정보를 표시한다. - [유상]영어버전일경우 출력하지 않음(멥 에디터 모드일때는 관계없음)
	//-------------------------------------------
	if ( (pclClient->GameMode == GAMEMODE_MAPEDIT) || (pclClient->siServiceArea != ConstServiceArea_English) )
	{
		HDC hdc;

		if(lpBackScreen->GetDC(&hdc) == DD_OK)
		{
			if ( pclmap )
			{
				pclmap->SetGeoInfo(hdc);
			}

			lpBackScreen->ReleaseDC(hdc);
		}

		if ( pclmap )
		{
			pclmap->DrawGeoInfo(lpBackScreen, pclHelpManager);
		}
	}

	//-----------------------------------------------
	// 화면 변위 설정. 
	//-----------------------------------------------
	SI32 virtualx = 0; 
	SI32 virtualy = 0; 

	if(pclmap)
	{
		virtualx = pclmap->VirtualMapX;
		virtualy = pclmap->VirtualMapY;
	}

	SI32 rx	= 0, ry	= 0;
	// 배를 타고 있다면 배의 RX, RY를 근거로 한다.
	rx = pclCM->CR[1]->GetRX();
	ry = pclCM->CR[1]->GetRY();

	// 일본버전만 옛날 스크롤 방식
	if (pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		// 화면의 RX, RY를 설정한다. 
		SetScreenRXY(rx, ry);

		pclUnderMouseObject->SetRX(rx);
		pclUnderMouseObject->SetRY(ry);
	}

	//-----------------------------------------
	// Hit Effect를 그려준다. 
	//-----------------------------------------
	if(pclmap)
	{
		SI32 centerx	= 0, centery	= 0;
		SHORT screenx	= 0, screeny	= 0;
		
		pclmap->GetXYDotFromXYCord(virtualx, virtualy, centerx, centery);
		pclmap->TransFromRealDotToScreenDot(centerx, centery, screenx, screeny);
		
		g_AttackEffectMgr.Draw(lpBackScreen, centerx, centery, screenx, screeny ); 
	}

	//------------------------------------------
	// 물방울을 그린다.
	//------------------------------------------
	if( GP.LockSurface(lpBackScreen) == TRUE )
	{   
	   if( pclmap && pclmap->pclWaterDrop )
	   {
		   SI32 centerx = 0, centery = 0;
		   SI32 x, y;

		   pclmap->GetXYDotFromXYCord(virtualx, virtualy, centerx, centery);

		   x	= centerx + (pclmap->MH.MapXsize * MAP_TILE_XSIZE)/2 - (siClientScreenXsize/2);
		   y	= centery - (siScreenMode800X600YSize + siScreenModeYOffSet) /2;

		   pclmap->pclWaterDrop->Draw( x, y, siClientScreenXsize, siScreenMode800X600YSize + siScreenModeYOffSet);

	   }


		//------------------------------------------
		// 반딧불을 그린다.
		//------------------------------------------
	   if( pclmap && pclmap->pclFireFly )
	   {
		   SI32 centerx = 0, centery = 0;
		   SI32 x, y;

		   pclmap->GetXYDotFromXYCord(virtualx, virtualy, centerx, centery);

		   x	= centerx + (pclmap->MH.MapXsize * MAP_TILE_XSIZE)/2 - (siClientScreenXsize/2);
		   y	= centery - (siScreenMode800X600YSize + siScreenModeYOffSet)/2;

		   pclmap->pclFireFly->Draw( x, y, siClientScreenXsize, siScreenMode800X600YSize + siScreenModeYOffSet);

	   }

		//------------------------------------------
		// 얼음성 반짝이를 그린다.
		//------------------------------------------
	   if( pclmap && pclmap->pclIceFlash )
	   {
		   SI32 centerx = 0, centery = 0;
		   SI32 x, y;

		   pclmap->GetXYDotFromXYCord(virtualx, virtualy, centerx, centery);

		   x	= centerx + (pclmap->MH.MapXsize * MAP_TILE_XSIZE)/2 - (siClientScreenXsize/2);
		   y	= centery - (siScreenMode800X600YSize + siScreenModeYOffSet)/2;

		   pclmap->pclIceFlash->Draw( x, y, siClientScreenXsize, siScreenMode800X600YSize + siScreenModeYOffSet);

	   }

	   //------------------------------------------
	   // 작은풀숲 홀씨를 그린다.
	   //------------------------------------------
	   if( pclmap && pclmap->pclSeedDrop )
	   {
		   SI32 centerx = 0, centery = 0;
		   SI32 x, y;

		   pclmap->GetXYDotFromXYCord(virtualx, virtualy, centerx, centery);

		   x	= centerx + (pclmap->MH.MapXsize * MAP_TILE_XSIZE)/2 - (siClientScreenXsize/2);
		   y	= centery - (siScreenMode800X600YSize + siScreenModeYOffSet)/2;

		   pclmap->pclSeedDrop->Draw( x, y, siClientScreenXsize, siScreenMode800X600YSize + siScreenModeYOffSet);

	   }


		//-------------------------------------------------
		//  이동마크의행동 
		//-------------------------------------------------
		if ( pclmap )
		{
			if ( pclmap->pclMoveMark )
			{
				pclmap->pclMoveMark->Action();
			}
		}
		

		//--------------------------------------
		// 이동마크 그리기. 
		//--------------------------------------
	   // 이동 마크 찍기
		if ( pclmap )
		{
			pclmap->pclMoveMark->DrawMoveMark();
		}

	   GP.UnlockSurface(lpBackScreen);
	}

	//--------------------------------------
	// 메시지 표시. 
	//--------------------------------------
	SI32 yinstance = 0 ;
	for ( int i = 0 ; i < MAX_SUMMON_OWN ; i ++ )
	{
		cltSummon * pclSummon = pclCM->CR[1]->pclCI->clSummonInfo.GetSummon(i);
		if ( pclSummon && pclSummon->siKind > 0 )
			yinstance = ( i + 1 ) * 50 ;
	}

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
		yinstance += 30 ;

#ifndef _NEW_INTERFACE_
		pclMessage->Show(this, 90, 120 + yinstance , lpBackScreen, GetScreenRX(), GetScreenRY());
#else
		pclMessage->Show(this, 90, 150 + yinstance , lpBackScreen, GetScreenRX(), GetScreenRY());
#endif

	//------------------------------
	// 캐릭터 이름을 표시한다. 
	//------------------------------
	// 캐릭터 이름을 만든다. 
	if( g_SoundOnOffDlg.m_bShowCharName == false )
	{   
		pclCM->MakeCharName();
	}

	if(GP.LockSurface(lpBackScreen) == TRUE)
	{
		if( g_SoundOnOffDlg.m_bShowCharName == false )
		{        
			// 캐릭터 이름을 출력한다..
			pclCM->DrawCharName();
		}

		// 캐릭터 말풍선을 표시한다. 	
		pclCM->DrawChatBalloon();

		GP.UnlockSurface(lpBackScreen);
	}

	return true;
}

bool cltClient::GamaDrawInterface()
{

	//SI32 i;

	cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
	if(pclmap == NULL)return false;

	//--------------------------------
	// Help를 출력한다. 
	//--------------------------------
	pclHelpManager->Draw(lpBackScreen);

	if(GameMode == GAMEMODE_CLIENT)
	{
		//-------------------------
		// Pannel 출력 
		//-------------------------
		
		if(pclPannel)
		{
			cltCharCommon* pclChar = pclCM->CR[1];
			
			// 신분 이름 구하기. 
			//TCHAR rankname[128]=TEXT("");
			//cltSimpleRank* pclrank = &pclChar->pclCI->clCharRank;
			//UI08 grank = pclChar->pclCI->clBI.uiGRank ;
			//GetRankName(pclrank, rankname, 128, grank);

			static bool bRandStart = true ;
			static SI32 siRandX = 0 ;
			static SI32 siRandY = 0 ;
			if ( bRandStart )
			{
				siRandX = rand() % 3 ;
				siRandY = rand() % 3 ;
				bRandStart = false ;
			}
			
			pclPannel->Draw( lpBackScreen, GetScreenRX(), GetScreenRY(), siRandX, siRandY );
			
			//pclPannel->Draw(lpBackScreen, GetScreenRX(), GetScreenRY(), 
			//	rankname, 
			//	pclChar->pclCI->clIP.GetMoney(), 
			//	pclChar->pclCI->clIP.GetLevel(), 
			//	pclChar->pclCI->clIP.GetCurExp(),
			//	pclChar->clPB.GetNextExp(), 
			//	pclChar->pclCI->clIP.GetLife(), 
			//	pclChar->clPB.GetMaxLife(),
			//	pclChar->pclCI->clIP.GetMana(),
			//	pclChar->clPB.GetMaxMana(),
			//	pclChar->clPB.clHealth.AnalyzeHealth(pclChar->pclCI->clIP.GetLevel()),
			//	0, 0,
			//	pclChar->clPB.GetPrevExp(),
			//	pclChar->pclCI->clIP.Life + pclChar->clPB.siReservedLife,
			//	siRandX,siRandY);
			
		
		//	for ( i = 0 ; i < MAX_SUMMON_OWN ; i ++ )
		//	{
		//		//cltSummon *pclSummon = pclChar->pclCI->clSummonInfo.GetSummon(pclChar->siSummonIndex);
		//		cltSummon *pclSummon = pclChar->pclCI->clSummonInfo.GetSummon(i);
		//		if( pclSummon->siKind > 0 && pclSummon->siKind < MAX_KIND_NUMBER )//&& pclSummon->siStatus == SUMMON_STATUS_TAKEOUT )
		//		{
		//			SI32 kind = pclSummon->siKind;
		//			cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
		//			if ( pclki != NULL )
		//			{
		//				cltSummonScroll *pclScroll = &pclSummon->clScroll;

		//				cltPIPInfo			clipinfo2;
		//				clipinfo2.Set(&pclSummon->clIP);

		//				cltParametaBox clPB;
		//				clPB.Init(pclItemManager, &clipinfo2, NULL, pclki);

		//				bool bmosterhero = false;
		//				// 아이템몰에서 판매하는 소환영웅이고 몬스터에서 진화한 것이라면, 
		//				if(pclki->bSummonMall && pclSummon->siIdentity == 0)	
		//				{
		//					bmosterhero = true;
		//				}
		//				clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, pclScroll, GAMEMODE_CLIENT, true, bmosterhero);

		//				SI32 summonskill = pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

		//				SI32 minutevary = pclClient->GetMinuteFromsTime(((cltClient*)pclClient)->sTime);
		//				SI32 leftsummonstart = min(TABS(pclSummon->siTakeInMinute - minutevary), SUMMON_TAKEOUT_DELAY);
		//				
		//				/*
		//				pclPannel->DrawSummon(lpBackScreen, GetScreenRX(), GetScreenRY(), pclki->siSummonPortrait, 
		//					pclSummon->clIP.GetCurExp(),
		//					clPB.GetNextExp(),
		//					pclSummon->clIP.GetLife(),
		//					clPB.GetMaxLife(),
		//					pclSummon->clIP.GetMana(),
		//					clPB.GetMaxMana(),
		//					clPB.GetPrevExp(),
		//					pclSummon->clIP.GetLife(),
		//					pclSummon->siStatus,
		//					pclSummon->clIP.GetLuk(),
		//					pclClient->GetMaxStaminaValue(summonskill ),
		//					leftsummonstart,
		//					i,
		//					siRandX,siRandY);
		//				*/
		//			}
		//		}
		//	}
		}


		if(( m_pDialog[NITEMMALLMANAGER_DLG]))
		{
			if(pclCM->CR[1]->pclCI->clItemMall.GetExistence())
			{
				((CNItemMallButtonManager*)m_pDialog[NITEMMALLMANAGER_DLG])->SetFlash(true);				
			}
			else
			{				
				((CNItemMallButtonManager*)m_pDialog[NITEMMALLMANAGER_DLG])->SetFlash(false);
			}
		}
				
		if ( NULL == m_pDialog[NMAGICCOOLTIME_DLG] )
		{
			CreateInterface( NMAGICCOOLTIME_DLG );
		}

		if ( pclDurability )
		{
			pclDurability->Draw( lpBackScreen, GetScreenRX(), GetScreenRY() );
		}

		// 프롬프트 
	//	if(bPromptSwitch)
		{ 
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{	
			}
			else
			{
				if ( GP.LockSurface( lpBackScreen ) == TRUE )
				{
					// 판넬을 출력한다. 
					TSpr* pspr = GetGlobalSpr(GIMG_MAGIC_PROMPTBAR);
					if(pspr)
					{
						SI32 startx = 80 + GetScreenRX();
						SI32 starty = 0;
						if ( g_SoundOnOffDlg.m_bFullScreenMode )
						{
							starty = GetScreenRY() + siScreenMode800X600YSize + siScreenModeYOffSet - pspr->GetXSize();
						}
						else
						{
							starty = GetScreenRY() + siScreenMode800X600YSize + siScreenModeYOffSet - pspr->GetYSize();
						}

						SI32 drawnum = (siClientScreenXsize - (80 * 2)) / pspr->GetXSize();

						for(SI32 i = 0;i < drawnum;i++)
						{
							GP.PutMpr(pspr, startx + i * pspr->GetXSize(), starty, PUT_SPR_SPR, FALSE, 0 );
						}
					}

					g_PromptMgr.Draw( 100 + GetScreenRX(), siScreenMode800X600YSize + siScreenModeYOffSet -   + GetScreenRY());

					GP.UnlockSurface( lpBackScreen );
				}
			}
		}
		
		if ( pclVillageNotice )
		{
			pclVillageNotice->Draw( lpBackScreen, GetScreenRX(), GetScreenRY() );
		}

		if ( IsWhereServiceArea(ConstServiceArea_China | ConstServiceArea_English | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE) )
		{
			// 메인맵에서 흑의군단 이벤트
			if ( pclBlackWarManager )
			{
				pclBlackWarManager->Draw( lpBackScreen, GetScreenRX(), GetScreenRY() );
			}
		}

		if ( pclBlackArmyManager )
		{
			pclBlackArmyManager->Draw( lpBackScreen, GetScreenRX(), GetScreenRY() );
		}
		if(GP.LockSurface(lpBackScreen)==TRUE)
		{
			
			//--------------------------
			// 로고를 출력한다. 
			//--------------------------

			if(pclLogo)
			{
				pclLogo->Draw(siClientScreenXsize, GetScreenYSize(), GetScreenRX(), GetScreenRY());
			}
			//--------------------------------------
			// 관전 모드인 경우 서버명을 출력해준다.
			//--------------------------------------
			// 관전 모드이면, 
			if(bWatchModeSwitch == true)
			{
				TSpr* pspr = pclClient->GetGlobalSpr( GIMG_SERVERNAME );

				if(pspr)
				{
					GP.PutSpr(pspr, siClientScreenXsize - 165 - pspr->GetXSize() + GetScreenRX(), 
						siScreenMode800X600YSize + siScreenModeYOffSet - 15 - pspr->GetXSize() + GetScreenRY(), 
						siServerUnique - 1);
				}
			}
			
			GP.UnlockSurface(lpBackScreen);
			
		}
		
		SI32 yindent = 0;


		if(GP.LockSurface(lpBackScreen)==TRUE)
		{
			cltCharCommon* pclChar = pclCM->CR[1];

			static long siLastDrawClock = 0;//pclClient->CurrentClock;
			//static long siDrawMarkClock = 0;

			if ( IsWhereServiceArea(ConstServiceArea_Korea | ConstServiceArea_NHNChina) )
			{
				if ( pclChar->pclCI->clBI.bPCRoom )
				{
					// 5분에 한번씩 1분 노출된다
					if( labs( labs(pclClient->CurrentClock) - labs(siLastDrawClock) ) > 300000L )
					{
						siLastDrawClock = pclClient->CurrentClock;

						if ( NULL == m_pDialog[NPCROOM_DISPLAY_DLG] )
						{
							CreateInterface( NPCROOM_DISPLAY_DLG );
						}

						((CNPCRoom_Display*)m_pDialog[NPCROOM_DISPLAY_DLG])->ShowDisplayPCRoom();
					}
				}
			}
			
			/*
			if( pclChar->pclCI->clBI.bPCRoom )
			{

				// 두 시각의 차가 5분을 넘는다면

				if( labs( labs(pclClient->CurrentClock) - labs(siLastDrawClock) ) > 300000L )
				{
					siLastDrawClock = pclClient->CurrentClock;
					siDrawMarkClock = pclClient->CurrentClock;
				}

				// 그려주는 시각에서 1분 동안은 그림을 그려준다.
				if( labs( labs(pclClient->CurrentClock) - labs(siDrawMarkClock) ) < 60000L  )
				{
					// [GOONZU-649 김영훈 수정:2007.11.28]
					// [영훈] PC방 프리미엄 강화에 따른 마크 추가
					SI32	siMarkNumber[Const_SpecialPCRoomMark]	= { GIMG_SPECIAL_PCROOM_MARK_1,		// 순간이동
																		GIMG_SPECIAL_PCROOM_MARK_4,		// 네잎클로버
																		GIMG_SPECIAL_PCROOM_MARK_2,		// 전투경험치
																		GIMG_SPECIAL_PCROOM_MARK_3		// 제조경험치
																	};	
					SI32	siSubPosition[Const_SpecialPCRoomMark]	= { 286, 234, 182, 130 }; // 기본위치가 되는 좌표
					SI32	siScreenModeAddPosition					= 0; // 스크린 모드에 따른 위치 추가값

					SI32	siScreenX								= GetScreenRX();
					SI32	siScreenY								= GetScreenRY();

					TSpr*	psprPCRoomMark							= NULL;
					

					// 풀스크린일 경우 기본 좌표에 -20을 빼줌
					if ( g_bFullScreen )
					{
						siScreenModeAddPosition = -20;
					}

					for ( SI32 siIndex = 0; siIndex<Const_SpecialPCRoomMark; siIndex++ )
					{
						psprPCRoomMark = NULL;
						psprPCRoomMark = GetGlobalSpr( siMarkNumber[ siIndex ] );

						if ( psprPCRoomMark != NULL )
						{
							SI32 siPositionX = siClientScreenXsize - siSubPosition[ siIndex ] + siScreenX + siScreenModeAddPosition;
							SI32 siPositionY = siScreenY + 210;

							GP.PutSpr( psprPCRoomMark, siPositionX, siPositionY, 0 );
						}
					}
				}
			}

			*/
			SI32	siXPos, siYPos;
			TSpr* pSpr = GetGlobalSpr(GIMG_BUFIMAGE);
			TSpr* pSpr2 = GetGlobalSpr(GIMG_BUFPERCENT);
			siXPos = siClientScreenXsize - 300 - pSpr->GetXSize() +GetScreenRX() - (g_SoundOnOffDlg.m_bMaxWindowMode? 30:0);
			siYPos = MAP_TILE_YSIZE + GetScreenRY() + 5;

/*
			// 프리미엄 이미지들이 떠 있으면 좀더 왼쪽으로
			if (m_pDialog[NPREMIUMSTATUS_DLG] && ((CNPremiumStatus*)m_pDialog[NPREMIUMSTATUS_DLG])->IsPremiumShow() )
			{
				siXPos -= 105;
			}


			if(pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_INCREASEMOVESPEED))
			{
				if (pSpr)
				{
					GP.PutSpr(pSpr, siXPos, siYPos, 2);
				}
				siXPos -= 40;
			}

			// 버프가 걸려있으면
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclCM->CR[1]->pclCI->clBI.clBufInfo );
			SI16 percent = 0 ;
			SI16 add = 0;
			SI32 currentTime = ((cltClient*)pclClient)->clClientDate.GetHourVary();

			for ( SI16 i=0 ; i < MAX_BUF ; i++ )
			{
				if ( clBuf.m_stBuf[i].siBufType == 0 ) 
					continue;
				if ( currentTime > clBuf.m_stBuf[i].siOverTime )
					continue;

				if ( currentTime < clBuf.m_stBuf[i].siOverTime )
				{
					GP.PutSpr( pSpr, siXPos, siYPos, clBuf.m_stBuf[i].siBufType-1 );
					if ( clBuf.m_stBuf[i].siPercent )
					{
						GP.PutSpr( pSpr2, siXPos+15, siYPos+28, clBuf.m_stBuf[i].siPercent / 10 - 1 );
					}
				}
				else		// 남은 시간이 거의 없을 경우! 깜빡인다.
				{
					if ( pclClient->CurrentClock % 1200 > 300 )
					{
						GP.PutSpr( pSpr, siXPos, siYPos, clBuf.m_stBuf[i].siBufType-1 );
						if ( clBuf.m_stBuf[i].siPercent )
						{
							GP.PutSpr( pSpr2, siXPos+15, siYPos+28, clBuf.m_stBuf[i].siPercent / 10 - 1 );
						}
					}

				}
				siXPos -= 40;
			}
*/
			// 마을 트리가 건설되어있다면
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if( ((cltCharClient*)pclCM->CR[1])->siEventNPCKind[i] == GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) ||
					 ((cltCharClient*)pclCM->CR[1])->siEventNPCKind[i] == GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")) )
				{
					TSpr* pSpr = GetGlobalSpr(GIMG_CHRISTMAS_BUF);

					GP.PutSpr( pSpr,siXPos - 105, siYPos + 35, 1 );
					siXPos -= 40;
				}	

			}
			//발렌타인데이 전쟁
			if (pclClient->pclCM->CR[1]->pclValentineWar->GetCouple() > 0 && 
				pclClient->pclCM->CR[1]->dwValentineAnswerTime > 0 ) 
			{
				// 10초동안 그림 그려준다.
				if ( TABS(pclClient->CurrentClock - pclClient->pclCM->CR[1]->dwValentineAnswerTime ) < 10000 )
				{
					TSpr* pspr = pclClient->GetGlobalSpr(GIMG_VALENTINEWAR);

					SI32 siPositionX = pspr->GetXSize() + GetScreenRX()	;
					SI32 siPositionY = GetScreenRY() + 100;
                    GP.PutSpr(pspr,siPositionX,siPositionY,pspr->GetYSize());

				}

			}

			
/*
#ifdef EVENT_CHRISTMAS
	
			// 마을 트리가 건설되어있다면
			
 			if( ((cltCharClient*)pclCM->CR[1])->siEventNPCKind > 0 )
			{
				TSpr* pSpr = GetGlobalSpr(GIMG_CHRISTMAS_BUF);
				
				GP.PutSpr( pSpr,siXPos, siYPos, 1 );
				siXPos -= 40;
			}
#endif
*/

			/*
#ifdef EVENT_HAPPY_NEWYEAR

			// 마을 트리가 건설되어있다면

			if( ((cltCharClient*)pclCM->CR[1])->siEventNPCKind > 0 )
			{
				TSpr* pSpr = GetGlobalSpr(GIMG_CHRISTMAS_BUF);

				GP.PutSpr( pSpr,siXPos, siYPos, 1 );
				siXPos -= 40;
			}
#endif
*/ 
/*
#ifdef EVENT_SUMMON_EVENT

			// 소환수가 소환되어 있다면
			
			SI32 summonId = pclCM->CR[1]->GetSummonID();

			if( pclCM->IsAlive(summonId) == true )
			{
				TSpr* pSpr = GetGlobalSpr(GIMG_SUMMON_EVENT_BUF);

				GP.PutSpr( pSpr,siXPos, siYPos, 1 );
				siXPos -= 40;
			}

#endif
*/
			// 산타 세트 효과가 발휘되고있다면

			if(    pclCM->CR[1]->m_siSetItemType == XMAS_SETITEM
				|| pclCM->CR[1]->m_siSetItemType == BOY_SULBIM_SETITEM
				|| pclCM->CR[1]->m_siSetItemType == GIRL_SULBIM_SETITEM )
			{
				TSpr* pSpr = GetGlobalSpr(GIMG_CHRISTMAS_BUF);

				GP.PutSpr( pSpr,siXPos - 105, siYPos + 35, 0 );
				siXPos -= 40;
			}


			GP.UnlockSurface(lpBackScreen);
		}
		

		//---------------------------
		// 생산 지연 시간을 표시한다. 
		//---------------------------
		cltCharClient* pclchar = (cltCharClient*)pclCM->CR[1];
		 if(pclchar->bStartMakeItemSwitch)
		 {
			 pclchar->DrawMakeDelay(lpBackScreen, GetScreenRX(), GetScreenRY());
		 }


		 
		 if(GP.LockSurface(lpBackScreen)==TRUE)
		 {
			//------------------------------
			// 주민 로그인 정보를 표시한다.
			//------------------------------
			 m_pResidentLoginDraw->Draw( GetScreenRX(), GetScreenRY() );

			 
			//------------------------------
			// 친구 로그인 정보를 표시한다.
			//------------------------------
			 m_pFriendLoginDraw->Draw( GetScreenRX(), GetScreenRY() );

			 //--------------------------------
			 // 마을 전쟁 상황 알림 
			 //--------------------------------
			m_pVillageWarNotice->Draw( GetScreenRX(), GetScreenRY() );


			//--------------------------------
			// 신년 이벤트 상황 알림 메니져
			//--------------------------------
			m_pNewYearEventNotice->Draw( GetScreenRX(), GetScreenRY() );

			GP.UnlockSurface(lpBackScreen);
		 }
	



		 //cyj 내시 퀘스트시 마우스 클릭 이미지 나타나도록 
		 // 내시
	      /*  
		 if(pclQuestManager->siQuestType == QUEST_TYPE_BEGINNER2)
		 {
			 if( pclQuestManager->  siCurrentQuestUnique == 30003 || pclQuestManager->siCurrentQuestUnique == 30012 ||pclQuestManager->siCurrentQuestUnique == 30024)
				 pclQuestManager->DrawClickMouse(lpBackScreen, pclQuestManager->siCurrentQuestUnique);
		 }*/
		


		 /*cyj 역관은 제외
		 // 역관
		 if(pclQuestManager->siQuestType == QUEST_TYPE_BEGINNER5)
		 {
			 if( pclQuestManager->siCurrentQuestUnique == 25003 || pclQuestManager->siCurrentQuestUnique == 25012 ||pclQuestManager->siCurrentQuestUnique == 25024)
				 pclQuestManager->DrawClickMouse(lpBackScreen, pclQuestManager->siCurrentQuestUnique);
		 }
		 */

		 // 길드전 중이면
		if (pclGuildWarClient)
		{
			if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar))	// 적용후
			{
				if ( IsGuildWarMap_Use_New_GuildWar(pclCM->CR[1]->GetMapIndex()) || IsGuildWarMap_NEW(pclCM->CR[1]->GetMapIndex()))
					pclGuildWarClient->DrawStatus( lpBackScreen, GetScreenRX(), GetScreenRY() );
			}
			else	// 적용전
			{
				if ( IsGuildWarMap_Not_Use_New_GuildWar(pclCM->CR[1]->GetMapIndex()) )
					pclGuildWarClient->DrawStatus( lpBackScreen, GetScreenRX(), GetScreenRY() );
			}
		}

		//	[종호]	사냥터 획득 관련 길드전 중이라면
		if(m_pclGuildHuntMapWarStatMgr)
		{
			if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar))	// 적용후
			{
				if(m_pclGuildHuntMapWarStatMgr->IsGuildHuntMapWar() == true 
					&& IsGuildWarMap_NEW(pclCM->CR[1]->GetMapIndex()) )
					m_pclGuildHuntMapWarStatMgr->DrawStatus(lpBackScreen, GetScreenRX(), GetScreenRY());
			}
		}
		if ( pclClient->bTimeCount == true )
		{
			if(pclClient->siTimeSet > 0 )
			{
				DrawTimeCount( pclClient->siTimeSet, lpBackScreen, GetScreenRX(), GetScreenRY() );
			}
		}
		if ( pclClient->siRoundCount >= 0)
		{
			DrawRound( pclClient->siRoundCount, lpBackScreen, GetScreenRX(), GetScreenRY() );
		}
	}
	return true;
}


void cltClient::GameDraw()
{
	if(GameDrawHeroViewOperate() == false)
	{
		return ;
	}

	if(GameDrawMap() == false)
	{
		return ;
	}

	if(GamaDrawInterface() == false)
	{
		return ;
	}

	//-----------------------------------
	// 화면으로 업데이트. 
	//-----------------------------------

#ifdef JEWAN_FRAME_SKIP
		if( g_graphics.m_bSkipDraw == FALSE ) {
			GameDrawUpdateScreen();

		}
#else
		GameDrawUpdateScreen();
#endif

	// 스크롤 조작을 다시 할 수 있도록 설정한다. 스크롤은 GameDraw() 한번에 한번씩만 이루어진다. 
	SetScrollProcSwitch(false);
}

void cltClient::GameDrawUpdateScreen()
{
	static SI32 oldrx, oldry;
	HRESULT hResult =0;

	/*
	RECT rect;
	rect.left   = 0;
	rect.right  = GP.GetScreenXsize();
	rect.top    = 0;
	rect.bottom = GP.GetScreenYsize();
	*/

	if(GameMode == GAMEMODE_CLIENT)
	{
		if ( GP.LockSurface(lpBackScreen)==TRUE)
		{
			// PC방
		//	if( m_pPCRoomIntro )
		//	{
		//		m_pPCRoomIntro->Draw();
		//	}

			// 내자시 이펙트
			if( m_pFeastEffectManager )
			{
				m_pFeastEffectManager->Draw();
			}

			GP.UnlockSurface(lpBackScreen);
		}

		if ( m_pInterfaceMgr )
		{
			m_pInterfaceMgr->Draw( lpBackScreen, GetScreenRX(), GetScreenRY() );
		}
		
		bool bLoginDlgShow = m_pDialog[NLOGIN_DLG] && m_pDialog[NLOGIN_DLG]->IsShow();
		bool bSelectCharDlgShow = m_pDialog[NSELECTCHAR_DLG] && m_pDialog[NSELECTCHAR_DLG]->IsShow();
		bool bSelectWorldDlgShow = m_pDialog[NSELECTWORLDLIST_DLG] && m_pDialog[NSELECTWORLDLIST_DLG]->IsShow();
		// 캐릭터 선택창, 로그인창, 서버선택창이 떠있으면 출력 안한다.
		if ( (bLoginDlgShow || bSelectCharDlgShow || bSelectWorldDlgShow) == false )
		{
			if(pclQuestManager)			pclQuestManager->DrawClickMouse(lpBackScreen, pclQuestManager->siCurrentQuestUnique);
		}

		//--------------------------------
		// 게임 등급 표시
		//--------------------------------
		if(m_kTimer_ShowRank_Time.IsActivated())
		{
			if(m_kTimer_ShowRank_Time.IsTimed(GetTickCount(), false) == true)
			{
			}
			SI32 siPosX, siPosY;

			siPosY = GetScreenRY();
			siPosX = GetScreenRX() + GetScreenXSize() - 205;

			switch(m_siDrawMode)
			{
			case 0:
				{
					siPosY += 40;
				}
				break;
			case 1:
				{
					siPosY += 40 + 185;
				}
				break;
			}

			SI32 siFont = 1;


			if(siServiceAge >= 18)
				siFont = 1;		// 18세 이상 이용가		
			else
				siFont = 0;		// 전체 이용가


			TSpr* pspr = GetGlobalSpr(GIMG_GOVERNMENT_RANKMARK);
			if ( GP.LockSurface( lpBackScreen ) == TRUE )
			{
				GP.PutSpr(pspr, siPosX,	siPosY,	siFont);

				GP.UnlockSurface(lpBackScreen);
			}
		}

		//}
		/*
		if( m_pDialog[ NMINIMAP_DLG ] )
		{
			((CNMiniMapDlg *)m_pDialog[ NMINIMAP_DLG ])->DrawMiniMap( lpBackScreen, GetScreenRX(), GetScreenRY() );
		}
		*/

	}


	//-------------------------------------------
	// 태곤테스트
	//-------------------------------------------
	//UpdateScreen(GetHwnd(), lpBackScreen, false);		// 화면 갱신을 기다린다. 

	if(oldrx != GetScreenRX() || oldry != GetScreenRY())
	{
		UpdateScreen(GetHwnd(), lpBackScreen, true);		// 화면 갱신을 기다린다. 
	}
	else
	{ 
		UpdateScreen(GetHwnd(), lpBackScreen, false);	// 화면 갱신을 기다리지 않는다. 
	}

	//-------------------------------------------


	oldrx	= GetScreenRX();
	oldry	= GetScreenRY();
	

	// 서피스가 로스트된 것이면, 
	if(hResult == DDERR_SURFACELOST)
	{
		GP.Restore();
	}
}

void cltClient::UpdateScreen(HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, bool waitrefresh)
{

	SI32 xindent = 0;
	SI32 yindent = 0;


	// 클라이언트의 환경을 고려하여 찍어줘야 할 곳을 보정한다. 
	// 예를 들어 툴바가 있다면 툴바의 크기만큼 낮춰준다. 
	if(pclToolBar)
	{
		yindent	= pclToolBar->GetToolBarYsize() + 2;
	}
	

	/*
	if ( siScreenMode == MODE_GRAPHICS1024_768 )
	{
	// 1024 * 768 용
		GP.UpdateScreen(hwnd, pSurface, 1024, 768,
			xindent, yindent, MAP_TILE_XSIZE + GetScreenRX(), MAP_TILE_YSIZE + GetScreenRY(), waitrefresh);
	}
	else
	{
		GP.UpdateScreen(hwnd, pSurface, 
			siClientScreenXsize - (MAP_TILE_XSIZE * 2), 
			siClientScreenYsize - (MAP_TILE_YSIZE * 2),
			xindent, yindent, MAP_TILE_XSIZE + GetScreenRX(), MAP_TILE_YSIZE + GetScreenRY(), waitrefresh);
	}
	*/
	int screenXSize = 0;
	int screenYSize = 0;

	switch( siScreenMode )
	{
	case MODE_GRAPHICS800_600:
		screenXSize = 800;
		screenYSize = 600;
		break;

	case MODE_GRAPHICS1024_768:
		screenXSize = 1024;
		screenYSize = 768;
		break;
	// KHY - 1218 - 1024x600 해상도 추가. imsi
	case MODE_GRAPHICS1024_600:
		screenXSize = 1024;
		screenYSize = 600;
		break;

	case MODE_GRAPHICS1280_1024:
		screenXSize = 1280;
		screenYSize = 1024;
		break;
	}

	GP.UpdateScreen(hwnd, pSurface, 
		siClientScreenXsize - (MAP_TILE_XSIZE * 2), 
		siClientScreenYsize - (MAP_TILE_YSIZE * 2),
		xindent, yindent, 
		MAP_TILE_XSIZE + GetScreenRX(), MAP_TILE_YSIZE + GetScreenRY(), waitrefresh,
		screenXSize,
		screenYSize,
		g_LoginNoticeDlg.m_bShow,
		g_LoginNoticeDlg.m_hLoginNoticeDlg,
		IsCountrySwitch(Switch_ChangeFullScreen) );
}

void cltClient::DrawGovernMentRankMark(SI32 siDrawMode)
{
	m_kTimer_ShowRank_Time.SetActive(true, GetTickCount());
	m_siDrawMode	= siDrawMode;
}
void cltClient::DrawTimeCount(SI32 siTime,LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry)	// siTimeCount를 한다.
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 yindent  = 0;
	clock_t		m_endClock = pclClient->CurrentClock + ( siTime * 1000 );

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		yindent = 30 ;
	}
	if( m_endClock )
	{
		clock_t overTime = ( m_endClock - ( pclClient->CurrentClock ) ) / 1000;

		overTime = max( 0, overTime );

		TSpr* pspr = NULL	;
		pspr = GetGlobalSpr(GIMG_GUILDWAR_TIME);
		if(pspr != NULL)
		{
			// 전투 시작, 종료 알림 메세지 출력
			SI32 xPos = (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx;
			SI32 yPos = ry+yindent + 90;

			SI32 siMinuteFirst	=	0;			//	두자릿수 minute
			SI32 siMinuteLast	=	0;			//	한자릿수 minute
			SI32 siSecondFirst	=	0;			//	두자릿수 second
			SI32 siSecondLast	=	0;			//	두자릿수 second

			//	count -> 분 : 초 로 변환
			if( GP.LockSurface(lpsurface)==TRUE )
			{
				SI32	siMinute	=	overTime / 60;
				SI32	siSecond	=	overTime % 60;

				if(siMinute >= 10)
				{
					siMinuteFirst	=	siMinute / 10;
					siMinuteLast	=	siMinute % 10;
				}
				else
				{
					siMinuteFirst	=	0;
					siMinuteLast	=	siMinute;
				}

				if(siSecond >= 10)
				{
					siSecondFirst	=	siSecond / 10;
					siSecondLast	=	siSecond % 10;
				}
				else
				{
					siSecondFirst	=	0;
					siSecondLast	=	siSecond;
				}

				GP.PutSpr(pspr, xPos - 34, yPos, siMinuteFirst);
				GP.PutSpr(pspr, xPos - 17, yPos, siMinuteLast);

				GP.PutSpr(pspr, xPos, yPos, 10);

				GP.PutSpr(pspr, xPos + 17, yPos, siSecondFirst);
				GP.PutSpr(pspr, xPos + 34, yPos, siSecondLast);

				GP.UnlockSurface(lpsurface);
			}

			// 타임 차감을 위해서
			if ( (pclClient->CurrentClock - pclClient->dwWriteSetTime) >= 1000 )
			{
				if ( pclClient->siTimeSet <= 0 )
				{
					pclClient->dwWriteSetTime	=	0	;
					pclClient->siTimeSet	=	0	;
					pclClient->bTimeCount	=	false	;
				}
				else
				{
					pclClient->dwWriteSetTime	=	pclClient->CurrentClock	;
					pclClient->siTimeSet--;
				}
			}
		}
	}

}

void cltClient::DrawRound(SI32 siRound,LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry)// 라운드를 보여준다.
{
	// 이벤트 블럭을 설정 하고 쓰세요
	cltClient* pclclient = (cltClient*)pclClient;
	SI32 yindent  = 0;

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		yindent = 30 ;
	}

	TSpr* pspr = NULL	;
	pspr = GetGlobalSpr(GIMG_ROUNDCOUNT);	// 이미지를 가지고 온다.

	SI32 xPos = (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx;
	SI32 yPos = ry+yindent + 70;

	switch (siRound)
	{
	case GAME_ROUND_1 :
	case GAME_ROUND_2 :
	case GAME_ROUND_3 :
	case GAME_ROUND_4 :
	case GAME_ROUND_5 :
	case GAME_ROUND_6 :
	case GAME_ROUND_7 :
	case GAME_ROUND_8 :
	case GAME_ROUND_9 :
	case GAME_ROUND_10 :
		{
			GP.PutSpr(pspr, xPos, yPos+pspr->GetYSize(), siRound );
		}
		break;
	}


}