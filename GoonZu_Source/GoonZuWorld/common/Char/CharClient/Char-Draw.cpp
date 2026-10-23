#include "../../Client/Client.h"
#include <Graphic_Capsule.h>

//#include "../../Client/ColorTable/ColorTable.h"

#include <Mouse.h>	
#include "Util/Util.h"
#include "DevTool/DevCharInfo/DevCharInfo.h"
#include "DevTool/DevNPCInfo/DevNPCInfo.h"

#include "../../../common/Item/ItemCommon/cltSetItem.h"
#include "../../../Client/Client_BokBulBokMgr/Client_BBBMgr.h"

//#include "School/School.h"

// 채팅. 
#include "../../Client/Interface/DrawChat/DrawChat.h"
// 캐릭터 이름 출력
#include "../../Client/Interface/DrawCharName/DrawCharName.h"
// 풀스크린 모드냐 아니냐
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../Client/NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

// 파티 관련 출력 때문에...
//#include "../../Client/Interface/PartyDlg/PartyDlg.h"
// 경험치 표시 
#include "../../Client/Effect/ExpCounter/ExpCounter.h"
// 치료   표시 
#include "../../Client/Effect/HealCounter/HealCounter.h"
// 필살기 표시
#include "CharCriticalEffect/CharCriticalEffect.h"
// 파티 동료 표시 하기 위해서.
#include "CharPartyEmoticon/CharPartyEmoticon.h"
// 스틸 방지를 위한 찜 표시
#include "CharJim/CharJim.h"
// 조합형 캐릭터 서피스 관리자. 
#include "Char/CCD/CCDSurfaceMgr.h"
#include "Smoke/Smoke.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"
#include "../Client/NInterface/NPartyDlg/NPartyDlg.h"
#include "../Client/Ninterface/NMinimapdlg/NMinimapDlg.h"
#include "../Client/MarkManager/MarkManager.h"

#include "Guild/Guild.h"

#include "../common/SystemNPC/SystemNPC.h"
#include "../CommonLogic/CommonLogic.h"
//#include "../Common/CommonHeader.h"
#include "../common/Event/event.h"
#include "../common/Skill/Skill-Manager.h"

#include "../CommonLogic/Marriage/MarriageManager.h"

extern cltCommonLogic* pclClient;
extern	SHORT					GameMouseX, GameMouseY;
extern	RECT				ScreenRect;							// 지도가 표시되는 영역 
extern DWORD dwQuitTimer;

#define COLOR_BORDER       246

#define ENERGY_UNIT_SIZE	3

enum QUEST_DISPLAY_MARK
{
	QUEST_DISPLAY_MARK_NONE = 0,
	QUEST_DISPLAY_MARK_GREETING,
	QUEST_DISPLAY_MARK_GOING,
	QUEST_DISPLAY_MARK_CLEAR,

	//---------------------------------------
	// 이 위로 추가해 주세요
	//---------------------------------------
	QUEST_DISPLAY_MARK_MAX
};

// 캐릭터의 이름을 만든다.
bool cltCharClient::MakeCharName(cltCharClient* pclchar)
{
	cltClient* pclclient = (cltClient*)pclClient; 
	// 마우스가 위치한 캐릭터를 찾음.
	SI32 undermouseid = pclclient->pclUnderMouseObject->GetUnderMouseID();

	if(pclchar == NULL)pclchar = this;

	// 말류 이면 리턴. 
	if(pclchar->pclKI->IsAtb(ATB_HORSE))return false;

	// [영훈] 해적선 : 파티인던 입구 NPC라면 리턴
	if ( pclchar->GetKind() == pclclient->GetUniqueFromHash( TEXT("KIND_PIRATESHIP_GATE") ) || 
		pclchar->GetKind() == pclclient->GetUniqueFromHash( TEXT("KIND_SANTAGATE") )) return false;

	// [영훈] 수호정령_090525 : 수호정령일때는 아무정보도 안보인다
	if ( IDENTITY_SOULGUARD == pclchar->clIdentity.siIdentity )
	{
		return false;
	}

	SI32 interval = 0;
	if(IsPC(pclchar->GetCharUnique())  == true)
	{
		// 만약 마우스를 올려놓았다면 레벨 정보도 추가로 표시해주기때문에 좀더 빨리 갱신해준다.
		if(pclchar->GetID() == undermouseid)
			interval = 20;
		else
			interval = 40;
	}
	else 		// PC가 아닌경우는 갱신하지 않는다.
	{
		if ( pclDrawCharName->IsSet() == true &&
			pclchar->clIdentity.siIdentity	!= IDENTITY_PERSON_SUMMON )
			return false;

		// 만약 마우스를 올려놓았다면 레벨 정보도 추가로 표시해주기때문에 좀더 빨리 갱신해준다.
		if(pclchar->GetID() == undermouseid)
			interval = 20;
		else
			return false;
	}

	if(TABS(pclClient->GetFrame() - dwLastMakeCharNameFrame ) < interval)
		return false;
	dwLastMakeCharNameFrame  = pclClient->GetFrame();

	//if(TABS(pclClient->GetFrame() - dwLastMakeCharNameFrame ) < interval)return false;
	//dwLastMakeCharNameFrame  = pclClient->GetFrame();

	TCHAR temp[256]		= TEXT("");
	TCHAR buffer[256]	= TEXT("");
	TCHAR buffer2[256]	= TEXT("");
	TCHAR rankname[256]	= TEXT("");

	//StringCchCopy(buffer, 256, TEXT(""));

	// Identity 가 존재하는 특수 캐릭터인 경우 , 레벨 정보
	switch(pclchar->clIdentity.siIdentity)
	{
	case IDENTITY_CASTLE:		
		StringCchPrintf(buffer, 256, TEXT("Lv%d "), max(1, pclchar->clIdentity.clGodInfo.siCapa - 99));		break;
	case IDENTITY_VILLAGEGOD:	
		StringCchPrintf(buffer, 256, TEXT("Lv%d "), max(1, pclchar->clIdentity.clGodInfo.siCapa - 99));		break;
	case IDENTITY_VILLAGEGOD_SUMMON:
		StringCchPrintf(buffer, 256, TEXT("Lv%d "), max(1, pclchar->clIdentity.clGodInfo.siCapa - 99));		break;
	case IDENTITY_PERSON_SUMMON:
		StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->pclCI->clIP.GetLevel());				break;
	case IDENTITY_VILLAGEHUNT:
		StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->pclCI->clIP.GetLevel());				break;
	case IDENTITY_INSTANCEMAP:
		StringCchPrintf(buffer, 256, TEXT(""));														break;
	case IDENTITY_SOULGUARD:
		StringCchPrintf(buffer, 256, TEXT(""));														break;
	default:	
		{
			if(pclchar->GetID() == 1) // 자기 자신이면 레벨 표시 안해줌.
				break;		

			if(pclchar->pclKI->IsAtb(ATB_SYSTEMNPC))
				break;

			// 마우스가 올려져있는 캐릭터이거나 플레이캐릭터가 아니면 레벨을 표시한다.
			if(pclchar->GetID() == undermouseid || IsPC(pclchar->GetCharUnique()) == false )
			{
				StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->pclCI->clIP.GetLevel());	

				//if ( pclClient->siServiceArea == ConstServiceArea_China )
				//{
				//	if ( IsPC(pclchar->GetCharUnique()) == false )
				//	{
				//		if ( pclchar->pclKI->IsAtb(ATB_BOSS) )
				//		{
				//			StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->clPB.GetBossBattleLevel() );
				//		}
				//		else
				//		{
				//			StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->clPB.GetBattleLevel() );
				//		}
				//	}
				//	else
				//	{
				//		StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->pclCI->clIP.GetLevel() );
				//	}
				//}
				//else
				//{
				//		StringCchPrintf(buffer, 256, TEXT("Lv%d "), pclchar->pclCI->clIP.GetLevel());		
				//}
				//break;
			}
		}

	}

	SI32 guildicon = -1 ;
	TSpr* guildspr	= NULL;

	// naming - 
	if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel )
	{
		if(_tcscmp(pclchar->GetName(), TEXT("")) != 0)
		{
			TCHAR temp[256];
			//if ( pclchar->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GWANGGAETOMONUMENT")) 
			//	&& pclchar->pclMap->siMapIndex == MAPINDEX_DOCKDO )
			//{

			//	TCHAR* pText = GetTxtFromMgr(3513);
			//	StringCchPrintf(temp, 256, TEXT("[%s]"), pText);
			//	StringCchCat(buffer2, 256, temp);
			//}
			//else
			//{
			StringCchPrintf(temp, 256, TEXT("[%s]"), pclchar->GetName());
			StringCchCat(buffer, 256, temp);
			//}
		}

		// 운영자 권한에 따라 표시 정보가 다르다. 
		if(pclCM->IsValidID(1) && pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
		{
			if(((cltClient*)pclClient)->bWatchModeSwitch == false)
			{
				StringCchPrintf(temp, 256, TEXT("(%d)%d"), pclchar->CharUnique, pclchar->clPB.GetBattlePower());
				StringCchCat(buffer, 256, temp);
			}
		}


		cltNamingQuestUnit* pclUnit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );

		TCHAR buf2[256] = {0,};

		if( pclUnit != NULL )
		{
			StringCchPrintf( buf2, 256, TEXT("\r%s"), pclUnit->GetNaming() );
			StringCchCat( buffer, 256, buf2 );
		}

		// 길드 정보 출력
		if ( pclCI->clBI.clGuildRank.IsGuildMember() && 
			pclchar->pclCI->clBI.clGuildRank.uiConfirm != GUILDSTATUS_STANDING )
		{
			pclclient->m_pMarkManager->GetMarkSprAndFontIndex( pclCI->clBI.clGuildRank.siMarkIndex, &guildspr, &guildicon );
		}
	}
	else
	{
		// 신분을 적어준다. 
		if(pclchar->pclCI->clCharRank.siType || pclchar->pclCI->clBI.uiGRank )
		{
			if(GetRankName(&pclchar->pclCI->clCharRank, rankname, 256, pclchar->pclCI->clBI.uiGRank) == TRUE)
			{
				StringCchCat(buffer, 256, rankname);
			}
		}

		if(IsPC(pclchar->GetCharUnique()))
		{
			////cyj GM Mode가 Show(1)이면 [GM]마크 출력
			//if(pclchar->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
			//	StringCchPrintf(buffer2, 256, TEXT("[GM]"));

			//cyj GM Mode가 Hide(2)이면 [HIDE]마크 출력
			if(pclchar->pclCI->clBI.uiGMMode == GMMODE_ATB_HIDE)
				StringCchPrintf(buffer2, 256, TEXT("[HIDE]"));

			if(pclchar->pclCI->clBI.uiGMMode != GMMODE_ATB_GMSHOW)
			{
				TCHAR* pvillagename = pclClient->pclVillageManager->GetName(pclchar->pclCI->clBI.siHomeVillage);
				if(pvillagename)
				{
					StringCchCat(buffer2, 256, pvillagename);
				}
			}


		}

		if(_tcscmp(pclchar->GetName(), TEXT("")) != 0)
		{
			TCHAR temp[256];
			//if ( pclchar->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GWANGGAETOMONUMENT")) 
			//	&& pclchar->pclMap->siMapIndex == MAPINDEX_DOCKDO )
			//{

			//	TCHAR* pText = GetTxtFromMgr(3513);
			//	StringCchPrintf(temp, 256, TEXT("[%s]"), pText);
			//	StringCchCat(buffer2, 256, temp);
			//}
			//else
			//{
			StringCchPrintf(temp, 256, TEXT("[%s]"), pclchar->GetName());
			StringCchCat(buffer2, 256, temp);
			//}
		}

		// 운영자 권한에 따라 표시 정보가 다르다. 
		if(pclCM->IsValidID(1) && pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
		{
			if(((cltClient*)pclClient)->bWatchModeSwitch == false)
			{
				StringCchPrintf(temp, 256, TEXT("(%d)%d"), pclchar->CharUnique, pclchar->clPB.GetBattlePower());
				StringCchCat(buffer2, 256, temp);
			}
		}

		// 이름 정보 출력. 
		if(_tcscmp(buffer2, TEXT("")) != 0)
		{
			StringCchCat(buffer, 256, TEXT("\r\n"));
			StringCchCat(buffer, 256, buffer2);
		}

		// 길드 정보 출력
		if ( pclCI->clBI.clGuildRank.IsGuildMember() && 
			pclchar->pclCI->clBI.clGuildRank.uiConfirm != GUILDSTATUS_STANDING )
		{
			if ( pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_MASTER )
			{
				TCHAR* pRank = GetTxtFromMgr(6540);
				StringCchCat(buffer, 256, TEXT("\r\n"));
				StringCchPrintf(temp, 256, TEXT("%s %s"), pclCI->clBI.clGuildRank.szGuildName,pRank);
				StringCchCat(buffer, 256, temp);
			}
			else if ( pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_SECONDMASTER )
			{
				TCHAR* pRank = GetTxtFromMgr(6541);
				StringCchCat(buffer, 256, TEXT("\r\n"));
				StringCchPrintf(temp, 256, TEXT("%s %s"), pclCI->clBI.clGuildRank.szGuildName,pRank);
				StringCchCat(buffer, 256, temp);
			}
			else
			{
				StringCchCat(buffer, 256, TEXT("\r\n"));
				StringCchPrintf(temp, 256, TEXT("%s"), pclCI->clBI.clGuildRank.szGuildName);
				StringCchCat(buffer, 256, temp);
			}

			pclclient->m_pMarkManager->GetMarkSprAndFontIndex( pclCI->clBI.clGuildRank.siMarkIndex, &guildspr, &guildicon );
			//guildspr = pclClient->GetGlobalSpr( GIMG_VILLAGEMARK01 );
			//guildicon = pclCI->clBI.clGuildRank.siMarkIndex - 1 ;
		}
	}





	StringCchCopy(szCharNameInfo, 128, buffer);

	SI32 icon		= 0;
	TSpr* pspr	= NULL;

	// MVP일때
	if( pclchar->pclCI->clBI.uiGMMode == GMMODE_ATB_WARMVP )
	{
		pspr = pclClient->GetGlobalSpr( GIMG_MVPMARK1 );
		icon = 0;

	}
	// 마을 마크 
	else if( pclchar->pclCI->clBI.siHomeVillage > 0 ) 
	{
		if( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 변경후 
		{
			// 길드전장에 있을때는 보여주지 않는다.
			if ( IsGuildWarMap_Use_New_GuildWar(pclchar->GetMapIndex()) == false  ||
				IsGuildWarMap_NEW(pclchar->GetMapIndex()) == false )//예전 길드전이거나 새로운 길드전일때
			{
				if( pclchar->pclCI->clBI.uiGMMode != GMMODE_ATB_GMSHOW)
				{
					SI32 villagemark = 0;
					if( pclchar->pclCI->clBI.siHomeVillage >= 0 && pclchar->pclCI->clBI.siHomeVillage < MAX_VILLAGE_NUMBER )
						villagemark = ((cltClient*)pclClient)->pclVillageManager->pclVillageInfo[ pclchar->pclCI->clBI.siHomeVillage ]->clVillageVary.siVillageMark;

					if( villagemark > 0 ) {
						pclclient->m_pMarkManager->GetMarkSprAndFontIndex( villagemark, &pspr, &icon );
					}
				}
			}
		}
		else	// 변경전
		{
			// 길드전장에 있을때는 보여주지 않는다.
			if ( IsGuildWarMap_Not_Use_New_GuildWar(pclchar->GetMapIndex()) == false /* ||
				IsGuildWarMap_NEW(pclchar->GetMapIndex()) == false*/ )//예전 길드전이거나 새로운 길드전일때
			{
				if( pclchar->pclCI->clBI.uiGMMode != GMMODE_ATB_GMSHOW)
				{
					SI32 villagemark = 0;
					if( pclchar->pclCI->clBI.siHomeVillage >= 0 && pclchar->pclCI->clBI.siHomeVillage < MAX_VILLAGE_NUMBER )
						villagemark = ((cltClient*)pclClient)->pclVillageManager->pclVillageInfo[ pclchar->pclCI->clBI.siHomeVillage ]->clVillageVary.siVillageMark;

					if( villagemark > 0 ) {
						pclclient->m_pMarkManager->GetMarkSprAndFontIndex( villagemark, &pspr, &icon );
					}
				}
			}
		}
	}

	SI32 sex = 0;
	// 남자거나 몬스터이면, 
	if(pclchar->pclCI->clBI.uiSex || IsPC(pclchar->GetCharUnique()) == false)
	{
		sex = 1;
	}

	// 무기술의 달인 엠블렘 마크
	TSpr*	psprEmblem		= NULL;
	SI32	siEmblemIndex	= -1; // 시작 인덱스가 0부터이기 때문에 -1로 초기값을 잡아야 한다

	if ( IsPC(pclchar->GetCharUnique()) && (SKILLMSTER_WEAPONTYPE_NONE < pclchar->m_uiWeaponSkillMasterEmblem) && (SKILLMSTER_WEAPONTYPE_MAX > pclchar->m_uiWeaponSkillMasterEmblem) )
	{
		psprEmblem		= pclClient->GetGlobalSpr( GIMG_EMBLEM );
		siEmblemIndex	= (pclchar->m_uiWeaponSkillMasterEmblem-1); // 시작 인덱스는 0이기 때문에 -1을 빼줘야 한다(m_uiWeaponSkillMasterEmblem의 유효값은 1부터이다)
	}

	// 길드 마크 출력 방지 

	//if(pclClient->siServiceArea == ConstServiceArea_Japan)
	//{
	//	guildicon = -1;
	//}

	pclDrawCharName->Set( pclclient->pclDrawCharNameSurface, szCharNameInfo, pspr, icon, sex, pclchar->pclCI->clBI.uiGMMode,guildicon ,guildspr, siEmblemIndex, psprEmblem );

	return true;
}

// 캐릭터의 이름을 표시한다. 
void cltCharClient::DrawName()
{

	if(pclDrawCharName)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 startx = DrawStartX+(ImageXsize/2);
		SI32 starty =  0 ;

		if(selectRealAnyType( ))
			starty = DrawStartY+ImageYsize - pclKI->GetBottomIndent();
		else
			starty = DrawStartY+ImageYsize - GetKindInfo(true)->GetBottomIndent();

		pclDrawCharName->Draw( startx, starty);
	}
}


// 지휘효과 출력 함수.	   
BOOL cltCharClient::DrawGeneralEft()
{
	if ( (clPB.siGeneralAC <= 0) && (clPB.siGeneralDamage <= 0) )
	{
		return FALSE;
	}

	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_DEFUP);

	if(pspr == NULL)return FALSE;

	SI32 font = 0;
	if(clPB.siGeneralDamage && clPB.siGeneralAC)
	{
		font = 26;
	}
	else if(clPB.siGeneralDamage)
	{
		font = 0;
	}
	else if(clPB.siGeneralAC)
	{
		font = 13;
	}

	font = font + (pclClient->CurrentClock / 200)  % 13;

	if(selectRealAnyType( ))
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		GP.PutSpr(pspr, DrawStartX + ImageXsize/2 - pspr->GetXSize() /2, 
			DrawStartY + ImageYsize   - pspr->GetYSize() /2 -pclKI->clKIDrawInfo.siBottomIndent, 
			font);
	}
	else
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		GP.PutSpr(pspr, DrawStartX + ImageXsize/2 - pspr->GetXSize() /2, 
			DrawStartY + ImageYsize   - pspr->GetYSize() /2 - GetKindInfo(true)->clKIDrawInfo.siBottomIndent, 
			font);
	}

	return TRUE;
}

// 캐릭터의 그림자를 출력하는 함수 
BOOL cltCharClient::DrawShadow()
{
	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	// by LEEKH 2007.12.24 - TRANSFORM
	if ( ppclKI->clKIDrawInfo.bHideShadow == true ) return FALSE ;

	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_CHARSHADOW);

	if(pspr == NULL)return FALSE;

	SI32 font ;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 data = (ppclKI->clKIDrawInfo.siXsizePercent * ImageXsize) / 100;

	if(data < 40)
	{
		font = 2;
	}
	else if(data < 50)
	{
		font = 1;
	}
	else 
	{
		font  = 0;
	}

	// by LEEKH 2007.12.24 - TRANSFORM
	GP.PutSprShadow(pspr, DrawStartX + ImageXsize/2 - pspr->GetXSize() /2, 
		DrawStartY + ImageYsize   - pspr->GetYSize() /2 - ppclKI->clKIDrawInfo.siBottomIndent, 
		font);


	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Description	:	캐릭터를 찍을 좌표(DrawStartX, DrawStartY)를 계산한다. 
void cltCharClient::CalculateDrawStartXY()
{
	SHORT centerx = 0, centery = 0;

	SHORT x = GetX();
	SHORT y = GetY();


	cltMapClient* pclmap = (cltMapClient*)pclMap;

	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
	pclmap->GetCenterXYByMapCenterXY(x, y, centerx, centery);

	bool selectKind = false;
	if(selectRealAnyType( ))
	{
		selectKind = pclKI->IsAtb(ATB_HERO);
	}
	else
	{
		selectKind = GetKindInfo(true)->IsAtb(ATB_HERO);
	}

	// 주인공 캐릭터의 크기는 정해져 있다. 
	if(selectKind)
	{
		ImageXsize = 320;
		ImageYsize = 240;

		DrawStartX = centerx - ImageXsize/2 + siRX;
		DrawStartY = centery - ImageYsize + pclKI->GetBottomIndent() + siRY + VibrateY;
	}
	else
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 ref = GetKindInfo(true)->clCharAniManager.FindFirstAni();
		if(ref <= 0)MsgBox(TEXT("fd9jfd"), TEXT("fd9jf:%s"), GetName());

		// by LEEKH 2007.12.24 - TRANSFORM
		ImageXsize = GetKindInfo(true)->clCharAniManager.GetImageXsize(ref, pclClient->CurrentClock);
		ImageYsize = GetKindInfo(true)->clCharAniManager.GetImageYsize(ref, pclClient->CurrentClock);

		// by LEEKH 2007.12.24 - TRANSFORM
		DrawStartX = centerx - ImageXsize/2 + siRX;
		DrawStartY = centery - ImageYsize + GetKindInfo(true)->GetBottomIndent() + siRY + VibrateY;
	}
}


///////////////////////////////////////////////////////////////////////////////
// 찍을 위치를 계산한다. 
void cltCharClient::CalculateDrawStartXYInMapCaptureMode()
{
	SHORT centerx=0, centery=0;

	SHORT x=GetX();
	SHORT y=GetY();


	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
	SI32 mapcenterx = 0, mapcentery = 0;
	pclMap->GetXYDotFromXYCord(x, y, mapcenterx, mapcentery);

	centerx=mapcenterx;
	centery=mapcentery;

	// by LEEKH 2007.12.24 - TRANSFORM
	DrawStartX=centerx-ImageXsize/2 + GetRX();

	if(selectRealAnyType( ))
		DrawStartY=centery-ImageYsize + pclKI->GetBottomIndent() + GetRY() + VibrateY;
	else
		DrawStartY=centery-ImageYsize + GetKindInfo(true)->GetBottomIndent() + GetRY() + VibrateY;

}


// 말 풍선을 그린다. 
void cltCharClient::DrawChatBalloon()
{
	if(pclDrawChat)
	{
		cltKindInfo* ppclKI = pclKI;

		if(selectRealAnyType( ) == false)
			ppclKI = GetKindInfo(true);

		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize	= ppclKI->clKIDrawInfo.siXsizePercent*ImageYsize/100;
		SI32 centery	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 20;


		// [영훈] 변신시에 말풍선 출력위치 다시 계산 - 2008.01.31
		if ( clTransFormInfo.bTransFormMode == TRUE )
		{
			realysize	= (SI32)((ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize) / 100);
			centery		= DrawStartY + (ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize);
		}
		else
		{
			// 말을 타고 있으면 좀더 위에다 출력한다. 
			SI32 siHorseUnique = 0;
			if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
			{
				centery -= 20;
			}
		}

		// 앉아있을때는 좀더 아래에
		if(GetCurrentOrder() == ORDER_SITDOWN)
		{
			centery += 20;
		}

		pclDrawChat->Draw(DrawStartX+ImageXsize/2, centery, &ScreenRect);
	}
}

// 신분 표시를 한다. 
void cltCharClient::DrawRankMark(SI32 font)
{
	//if( pclClient->siServiceArea ==  ConstServiceArea_Korea )
	//{

	//	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_RANKMARK);

	//	if(pspr == NULL)return ;

	//	SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

	//	SI32 realysize	= pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

	//	SI32 starty	= DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize() /2 - 15;

	//	// 말을 타고 있으면 좀더 위에다 출력한다. 
	//	if(pclCI->clHorseInfo.siCurrentHorseIndex >= 0)
	//	{
	//		starty -= 20;
	//	}

	//	// 앉아있을때는 좀더 아래에
	//	if(GetCurrentOrder() == ORDER_SITDOWN)
	//	{
	//		starty += 20;
	//	}

	//	GP.PutSpr(pspr, startx,		starty,	 font);

	//}
	//else if( pclClient->siServiceArea == ConstServiceArea_China )
	//{
	//	SI32 startx = DrawStartX + ImageXsize/2 - m_RankMarkEncHres/2 ;
	//	SI32 realysize	= pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	//	SI32 starty	= DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - realysize - m_RankMarkEncVres - 15;


	//	// 말을 타고 있으면 좀더 위에다 출력한다. 
	//	if(pclCI->clHorseInfo.siCurrentHorseIndex >= 0)
	//	{
	//		starty -= 20;
	//	}

	//	// 앉아있을때는 좀더 아래에
	//	if(GetCurrentOrder() == ORDER_SITDOWN)
	//	{
	//		starty += 20;
	//	}

	//	if ( m_RankMarkImgEncBuffer )
	//	{
	//		GP.PutXpr16( startx  , starty , 
	//			//MAX_RANKMARK_WIDTH, MAX_RANKMARK_HEIGHT,
	//			m_RankMarkEncHres, m_RankMarkEncVres,
	//			m_RankMarkImgEncBuffer,
	//			RGB16_TRANS );
	//	}

	//}
	//else
	//{
	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	SI32 startx = DrawStartX + ImageXsize/2 - m_RankMarkEncHres/2 ;
	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - m_RankMarkEncVres - 15;

	// 말을 타고 있으면 좀더 위에다 출력한다. 
	SI32 siHorseUnique = 0;
	if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) == HORSEUNIQUE_SASURI)
	{
		starty -= HORSE_EFFECTHIGHT_NORMAL_SASURI;
	}
	else
	{
		if( siHorseUnique != 0 )
		{
			starty -= 20;
		}
	}

	// 앉아있을때는 좀더 아래에
	if(GetCurrentOrder() == ORDER_SITDOWN)
	{
		starty += 20;
	}

	if ( m_RankMarkImgEncBuffer )
	{
		GP.PutXpr16( startx  , starty , 
			//MAX_RANKMARK_WIDTH, MAX_RANKMARK_HEIGHT,
			m_RankMarkEncHres, m_RankMarkEncVres,
			m_RankMarkImgEncBuffer,
			RGB16_TRANS );
	}
	//}
}

// 최고 장인 표시를 한다. 
void cltCharClient::DrawMakeMasterMark(SI32 skillunique)
{
	if(skillunique < 1)return ;

	TSpr* pspr;
	SI32 font = skillunique - 1;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);


	// 생산도구제작술(unique:54) 의 경우에만 font 14출력
	if ( skillunique == SKILL_MAKETOOL1 )
	{
		font = 14;
	}

	// [진성] 결정체 제조 명인 표시 => 2008-5-15
	if ( skillunique == SKILL_ARTIFACT )
	{
		font = 27;
	}

	// PCK : 아바타 장인 아이콘 추가 (0907.23)
	if(skillunique == SKILL_MAKEAVATAR_DRESS)
	{
		font = 13;
	}
	else if(skillunique == SKILL_MAKEAVATAR_HAT)
	{
		font = 31;
	}
	else if(skillunique == SKILL_MAKEAVATAR_MANTLE)
	{
		font = 35;
	}
	else if(skillunique == SKILL_MAKEAVATAR_RING)
	{
		font = 12;
	}
	else if(skillunique == SKILL_MAKEAVATAR_NECK)
	{
		font = 24;
	}

	pspr = pclClient->GetGlobalSpr(GIMG_MAKEMASTER);

	if(pspr == NULL)return ;

	SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize() /2 - 10;

	// 말을 타고 있으면 좀더 위에다 출력한다. 
	SI32 siHorseUnique = 0;
	if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) == HORSEUNIQUE_SASURI)
	{
		starty -= HORSE_EFFECTHIGHT_NORMAL_SASURI;
	}
	else
	{
		if( siHorseUnique != 0 )
		{
			starty -= 20;
		}
	}

	// 앉아있을때는 좀더 아래에
	if(GetCurrentOrder() == ORDER_SITDOWN)
	{
		starty += 20;
	}

	GP.PutSpr(pspr, startx,		starty,	 font);

}

// GM마크를 표시한다.
void cltCharClient::DrawGMMark()
{
	TSpr* pspr;
	SI32 font = 0;

	bool bResult = true;
	switch(pclCI->clBI.uiGMMode)
	{
		//[추가 : 황진성 2007. 11. 27 운영자이며 버프를 적용시킨 GM 모드에 마크 그리기.]
	case GMMODE_ATB_BUFF:	bResult = false; 
		break;

	case GMMODE_ATB_GMSHOW: bResult = false; 
		break;
	}

	if(bResult)
	{
		return;
	}

	pspr = pclClient->GetGlobalSpr(GIMG_GMMARK);

	if(pspr == NULL)return ;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize() /2 - 10;

	// 말을 타고 있으면 좀더 위에다 출력한다. 
	SI32 siHorseUnique = 0;
	if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) == HORSEUNIQUE_SASURI)
	{
		starty -= HORSE_EFFECTHIGHT_NORMAL_SASURI;
	}
	else
	{
		if( siHorseUnique != 0 )
		{
			starty -= 20;
		}
	}

	// 앉아있을때는 좀더 아래에
	if(GetCurrentOrder() == ORDER_SITDOWN)
	{
		starty += 20;
	}

	GP.PutSpr(pspr, startx,		starty,	 font);

}

// [지연] 복불복 몹일때 머리위에 이미지 띄워줌 (원킬, 키, 선물몹만) - 1 : 원킬몹(4), 2 : 키몹(3), 3 : 선물몹(2)
void cltCharClient::DrawBBBSpecialIndicator( SI32 siType )
{
	cltKindInfo* ppclKI = pclKI;
	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	TSpr* pspr = pclClient->GetGlobalSpr(GIMG_PVP_COUNTDOWN);		// 특수몹 표시용 이미지 생기면 그 이미지의 enum을 여기다 넣음 gimg_bbbspecialindicator 등의 이름

	if ( NULL != pspr )
	{
		SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
		SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize() /2 - 10;

		GP.PutSpr(pspr, startx,	starty,	siType );	// 마지막 0은 나중에 이미지 따로 생기면 해당 이미지의 인덱스를 넣음. 3장 필요
	}
}

//[추가 : 황진성 2008. 1. 15 => 최강길드의 마크를 표시하는 함수]
void cltCharClient::DrawBestGuildMark(void)
{
	static TSpr Spr("gimg/BestGuildMark.SPR");
	if(NULL == Spr.pImage)
	{
		MsgBox("파일열기 실패", "BestGuildMark.SPR");
	}

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	SI32 startx = DrawStartX + ImageXsize/2 - Spr.GetXSize() /2;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - Spr.GetYSize() /2 - 10;

	// 말을 타고 있으면 좀더 위에다 출력한다. 
	SI32 siHorseUnique = 0;
	if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) == HORSEUNIQUE_SASURI)
	{
		starty -= HORSE_EFFECTHIGHT_NORMAL_SASURI;
	}
	else
	{
		if( siHorseUnique != 0 )
		{
			starty -= 20;
		}
	}

	// 앉아있을때는 좀더 아래에
	if(GetCurrentOrder() == ORDER_SITDOWN)
	{
		starty += 20;
	}

	GP.PutSprAlpha(&Spr, startx, starty, 13, 0);
}

void cltCharClient::DrawPCRoomMark( void )
{
	TSpr* psprPCRoomMark = pclClient->GetGlobalSpr( GIMG_PCROOM_CHAR_HEAD );
	if ( NULL == psprPCRoomMark )
	{
		return;
	}

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	SI32 startx = DrawStartX + ImageXsize/2 - psprPCRoomMark->GetXSize() /2;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - psprPCRoomMark->GetYSize() /2 - 10;

	// 말을 타고 있으면 좀더 위에다 출력한다. 
	SI32 siHorseUnique = 0;
	if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) == HORSEUNIQUE_SASURI)
	{
		starty -= HORSE_EFFECTHIGHT_NORMAL_SASURI;
	}
	else
	{
		if( siHorseUnique != 0 )
		{
			starty -= 20;
		}
	}

	// 앉아있을때는 좀더 아래에
	if(GetCurrentOrder() == ORDER_SITDOWN)
	{
		starty += 20;
	}

	GP.PutSprAlpha(psprPCRoomMark, startx, starty, 13, 0);
}
/*
void cltCharClient::DrawArrow()
{
	TSpr* psprEffect	= pclClient->GetGlobalSpr(GIMG_ARROWEFFECT);
	if(psprEffect == NULL)return ;

	SI32 startx = DrawStartX + ImageXsize/2 - psprEffect->GetXSize() /2;
	SI32 realysize	= pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 starty	= DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - realysize - psprEffect->GetYSize() /2 - 15;
	starty -= 25;

	SI32 font = 0;
	font = font + (pclClient->CurrentClock / 40)  % psprEffect->GetImageNum();

	GP.PutSprScreen(psprEffect, startx, starty, font);
}
*/

void cltCharClient::DrawQuestion()
{
	TSpr* psprEffect	= pclClient->GetGlobalSpr(GIMG_QUESTNPCEFFECT);
	if(psprEffect == NULL)return ;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 startx = DrawStartX + ImageXsize/2 - psprEffect->GetXSize() /2;
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - psprEffect->GetYSize() /2 - 15;
	starty -= 37;

	SI32 font = 0;
	font = font + TABS((pclClient->CurrentClock / 80))  % 6;

	GP.PutSpr(psprEffect,startx,starty,font);
	GP.PutSprScreen(psprEffect, startx, starty, font+6);
}


void cltCharClient::DrawRedExclamation()
{
	TSpr* psprEffect	= pclClient->GetGlobalSpr(GIMG_QUESTNPCEFFECT);
	if(psprEffect == NULL)return ;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 startx = DrawStartX + ImageXsize/2 - psprEffect->GetXSize() /2;
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - psprEffect->GetYSize() /2 - 15;
	starty -= 37;

	SI32 font = 12;
	font = font + TABS((pclClient->CurrentClock / 80))  % 6;

	GP.PutSpr(psprEffect,startx,starty,font);
	GP.PutSprScreen(psprEffect, startx, starty, font+6);
}


void cltCharClient::DrawBlueExclamation()
{
	TSpr* psprEffect	= pclClient->GetGlobalSpr(GIMG_QUESTNPCEFFECT);
	if(psprEffect == NULL)return ;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 startx = DrawStartX + ImageXsize/2 - psprEffect->GetXSize() /2;
	SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - psprEffect->GetYSize() /2 - 15;
	starty -= 37;

	SI32 font = 24;
	font = font + TABS((pclClient->CurrentClock / 80))  % 6;

	GP.PutSpr(psprEffect,startx,starty,font);
	GP.PutSprScreen(psprEffect, startx, starty, font+6);
}

void cltCharClient::DrawQuestionMark( SI16 siMode )
{
	if ( (QUEST_DISPLAY_MARK_NONE >= siMode) || (QUEST_DISPLAY_MARK_MAX <= siMode) )
	{
		return;
	}

	TSpr* psprEffect = pclClient->GetGlobalSpr( GIMG_QUESTNPCEFFECT );
	if ( NULL == psprEffect )
	{
		return;
	}
	const SI16 siFrame = 10;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);

	SI32 siStartX		= DrawStartX + ImageXsize/2 - psprEffect->GetXSize() /2;
	SI32 siRealySize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 siStartY		= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - siRealySize - psprEffect->GetYSize() /2 - 52;
	SI32 siFont			= ((siMode-1) * siFrame) + (TABS((pclClient->CurrentClock / 80))  % siFrame);

	//GP.PutSpr( psprEffect, siStartX, siStartY, siFont );
	GP.PutSprScreen( psprEffect, siStartX, siStartY, siFont/*+siFrame*/ );

}



//-----------------------------------------
// 캐릭터 그리기 
//-----------------------------------------
bool cltCharClient::Draw()
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltKindInfo* ppclKI = pclKI;

	if(selectRealAnyType( ) == false)
		ppclKI = GetKindInfo(true);


	// 주인공이고 관전모드이면 출력하지 않는다. 
	if(GetID() == 1 && ((cltClient*)pclClient)->bWatchModeSwitch == true)
	{
		return false;
	}

	// 운영자이고 하이드 모드이면 다른 사용자에게 보이지 않는다.
	if(GetID() != 1 && pclCI->clBI.uiGMMode == GMMODE_ATB_HIDE)
	{
		return false;
	}

	// 운영자가 데리고 다니는 말도 하이드 모드일때는 보이지 않는다.
	if (pclCM->IsValidID(siHostID))
	{
		if(siHostID != 1 && pclCM->CR[siHostID]->pclCI->clBI.uiGMMode == GMMODE_ATB_HIDE)
		{
			return false;
		}
	}

	// HealthBar Switch 
	bool bDrawHealthBarSwitch = false;

	// 연관된 지도에서만 그려준다. 
	if(pclClient->pclMapManager->pclCurrentMap != pclMap)return false;

	// 그려주기가 설정되어 있을 때에만 그려준다.
	if(DrawSwitch==FALSE)return false;

	// 같은 프레임에서는 두번그려지지 않도록 한다. 
	if(dwDrawFrame == pclClient->GetFrame())return false;
	dwDrawFrame = pclClient->GetFrame();

	// CLONE이라면 죽은모습은 찍어주지 않는다.
	if(Animation == ANITYPE_DYING && IsAtb(ATB_CLONE) && clInnerStatus.IsStatus(INNERSTATUS_CLONE) == true) return false;


	// 찍을 위치 관련 정보를  판단한다. 
	CalculateDrawStartXY();
	
	// [기형] 배틀로얄 중- 변신 상태 인지
	bool bDrawSetItem = true;
	CBattleRoyalClient * pclBattleRoyalClient = ((cltClient*)pclClient)->m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		if(TRUE == pclBattleRoyalClient->IsTransform(this))
		{
			bDrawSetItem = false;
		}
	}

	// [기형] 배틀로얄 - 파워업 모드 캐릭터 일 경우
	if( bBattleRoyalPowerupMode == true && bDrawSetItem == false)
	{
		TSpr* pspr = pclClient->GetGlobalSpr( GIMG_HUMAN_EFFECT3 );
		if( pspr )
		{
			if( pspr )
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}	
		}	
	}

	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) && bDrawSetItem == true)
	{
		SI16 siCompletedSetItemIndex = clPB.clShapeEquip.siCompletedSetItemIndex;
		SI16 siCompletedSetItemIndexStep = clPB.clShapeEquip.siCompletedSetItemIndexStep;

		SI16 siCompletedSetItemIndex2 = clPB.clShapeEquip.siCompletedSetItemIndex2;
		SI16 siCompletedSetItemIndexStep2 = clPB.clShapeEquip.siCompletedSetItemIndexStep2;

		if(( m_siSetItemType == 0) && (siCompletedSetItemIndex >0))
		{
			m_siSetItemType = siCompletedSetItemIndex;
			m_siSetItemStep = siCompletedSetItemIndexStep;	//[진성] 셋트 아이템의 단계. 2008-11-12
		}

		if(( m_siSetItemType2 == 0) && (siCompletedSetItemIndex2 >0))
		{

			m_siSetItemType2 = siCompletedSetItemIndex2;
			m_siSetItemStep2 = siCompletedSetItemIndexStep2;	//[진성] 2번째 셋트 아이템의 단계. 2008-11-12
		}

		if( m_siSetItemType )
		{
			TSpr* pspr = pclClient->GetGlobalSpr( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemHumanEffect(this, USE_SETITEM_1) );
			if( pspr )
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}	
		}

		if( m_siSetItemType2 )
		{
			TSpr* pspr = pclClient->GetGlobalSpr( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemHumanEffect(this, USE_SETITEM_2) );
			if( pspr )
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}	
		}

		if( m_siSetItemType3 )
		{
			TSpr* pspr = pclClient->GetGlobalSpr( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemHumanEffect(this, USE_SETITEM_3) );
			if( pspr )
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}	
		}

		if( m_siSetItemType4 )
		{
			TSpr* pspr = pclClient->GetGlobalSpr( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemHumanEffect(this, USE_SETITEM_4) );
			if( pspr )
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}	
		}

		if( m_siSetItemType5 )
		{
			TSpr* pspr = pclClient->GetGlobalSpr( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemHumanEffect(this, USE_SETITEM_5) );
			if( pspr )
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}	
		}
	}


	// 그림자가 있는 것은 그림자를 그려준다. 
	DrawShadow();

	// [기형] 배틀로얄 맵 일 경우
	if ( bDrawSetItem == true)
	{

		// 지휘효과 출력. 
		DrawGeneralEft();

		if ( pclBoss )
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 centery	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent;// - realysize/2;

			pclBoss->Draw(DrawStartX+ImageXsize/2, centery);
		}

		//if(pclRegeneration)
		//{
		//	SI32 realysize	= pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		//	SI32 centery	= DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent;// - realysize/2;

		//	pclRegeneration->DrawBottom(DrawStartX+ImageXsize/2, centery);
		//}


		//--------------------------
		// 신분 표시 (기자..) 
		//--------------------------
		// 기자모자.
		bool bHaveRank = false; // 아래 길드마크때문에 신분이 있으면 이 값을 true로 변경 요망
		SI32 Itemunique = pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique;
		if(Itemunique == 0)
		{
			Itemunique = clPB.clShapeEquip.clHelmet.uiUnique;
		}

		// [영훈] 기자단 모자 : 모자 효과
		if ( CONST_JOURNALIST_HAT_ITEMUNIQUE == Itemunique )
		{
			DrawRankMark( 0 );
			bHaveRank = true;
		}

		SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique(Itemunique);

		if(Ref > 0)
		{
			/*
			// 기자 전용. 
			if(pclClient->pclItemManager->pclItemInfo[Ref]->siNeedRank == 1)
			{
				DrawRankMark(0);	
			}
			*/

				// 방송인 전용. 
				if(pclClient->pclItemManager->pclItemInfo[Ref]->siNeedRank == 4)
				{
					DrawRankMark(42);	
					bHaveRank = true;
				}
				//  수호천사 전용. 
				if(pclClient->pclItemManager->pclItemInfo[Ref]->siNeedRank == 5)
				{
					DrawRankMark(54);
					bHaveRank = true;
				}
				//  방송인도우미 전용 
				if(pclClient->pclItemManager->pclItemInfo[Ref]->siNeedRank == 6)
				{
					DrawRankMark(57);
					bHaveRank = true;
				}
				//  포교 도우미 전용 
				if(pclClient->pclItemManager->pclItemInfo[Ref]->siNeedRank == 7)
				{
					DrawRankMark(62);
					bHaveRank = true;
				}
		}


		if(IsPC(GetCharUnique()) && (clInnerStatus.siTopSkillUnique ))
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_HUMAN_EFFECT);
			if(pspr)
			{
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

				//GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
				GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
			}

			DrawMakeMasterMark(clInnerStatus.siTopSkillUnique);
		}
		//[추가 : 황진성 2008. 1. 14 => 최강 길드인 길드원은 장인마크를 그려준다..] 2008-5-6 신분표시가 없을때만 나오게 수정.
		else if ( IsPC(GetCharUnique()) && pclCM->IsValidID(siID) && (pclCM->CR[siID]->uiBestGuild == 1) && (false==bHaveRank) )
		{
			DrawBestGuildMark();
		}
		//[추가 : 황진성 2007. 11. 27 운영자이며 버프를 적용시킨 GM 모드에 마크 그리기.]
		else if (IsPC(GetCharUnique()) && pclCM->IsValidID(siID) && pclCM->CR[siID]->pclCI->clBI.uiGMMode == GMMODE_ATB_BUFF)
		{
			DrawGMMark();	
			DrawKingEffect();
		}
		// 최고 장인 표시나 GM마크를 그린다. (같은 위치이므로)
		else if (IsPC(GetCharUnique()) && pclCM->IsValidID(siID) && pclCM->CR[siID]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
		{
			DrawGMMark();
		}
		else if ( pclClient->IsWhereServiceArea(ConstServiceArea_Korea | ConstServiceArea_NHNChina) && (IsPC(GetCharUnique())) && (pclCM->IsValidID(siID)) && (!bHaveRank) && (pclCM->CR[siID]->pclCI->clBI.bPCRoom) )
		{
			DrawPCRoomMark();
		}

		if ( pclClient->IsCountrySwitch(Switch_BokBulBok) )
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			// [지연] : 복불복 이벤트 몹 : pvp카운트로 몹 타입 표시 ( 1 : 원킬, 2 : 키, 3 : 선물몹, 일반몹 )
			cltClient* pClient = (cltClient*)pclClient;
			if ( true == pClient->m_pClient_BBBMgr->m_bGMShowMonsterType )
			{
				if ( IsNPC(GetCharUnique()) )
				{
					if ( pclCM->IsValidID(siID) )
					{
						SI08 siMydentity = pclCM->CR[siID]->clIdentity.siIdentity;
						if ( siMydentity == IDENTITY_BOKBULBOK_ITEMMONSTER )
						{
							DrawBBBSpecialIndicator( 2 );	// pvp카운트 이미지의 4번 인덱스 그림이 숫자 3이다
						}
						else if ( siMydentity == IDENTITY_BOKBULBOK_KEYMONSTER )
						{
							DrawBBBSpecialIndicator( 3 );	// pvp카운트 이미지의 4번 인덱스 그림이 숫자 2이다
						}
						else if ( siMydentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
						{
							DrawBBBSpecialIndicator( 4 );	// pvp카운트 이미지의 4번 인덱스 그림이 숫자 1이다
						}
					}
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		// 제왕의 망토
		if ( IsPC(GetCharUnique()) )
		{
			SI32 mantleunique = pclCI->clCharItem.clItem[PERSONITEM_MANTLE].siUnique ;
			if ( mantleunique == 0 )
			{
				mantleunique = clPB.clShapeEquip.clMantle.uiUnique ;
			}

			if ( mantleunique == ITEMUNIQUE(18510) )
			{
				TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_HUMAN_EFFECT);
				if(pspr)
				{
					// by LEEKH 2007.12.24 - TRANSFORM
					SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
					SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
					SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+28;

					//GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
					GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);				
				}
			}
		}

		//----------------------
		// 왕 전용 
		//----------------------
		SI32 itemunique = pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique;
		if(itemunique == 0)
		{
			itemunique = clPB.clShapeEquip.clArmour.uiUnique;
		}
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);

		if(ref > 0 && pclClient->pclItemManager->pclItemInfo[ref]->siNeedRank == 2)
		{
			DrawKingEffect();
		}

		if ( pclHeartEffect )
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent ;
			pclHeartEffect->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
		}

		if( pclCushionEffect ) 
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent ;
			pclCushionEffect->Draw( DrawStartX + ImageXsize/2, realyimagestarty ); 
		}

		//----------------------
		//성황신을 경우 성황신 Effect출력
		//----------------------
		switch(clIdentity.siIdentity)
		{
		case IDENTITY_VILLAGEGOD:			
			{
				//MakeRankMarkEncBuffer();
				DrawRankMark(43);	
				DrawGodEffect();
			}
			break;
		case IDENTITY_CASTLE:				break;
		case IDENTITY_VILLAGEGOD_SUMMON:	break;
		case IDENTITY_PERSON_SUMMON:
			{
				//이제는 소환영웅도 보여준다.
				//if ( !pclKI->bSummonMall )
				DrawRankMark(44);
			}
			break;
		case IDENTITY_VILLAGEHUNT:			/*DrawRankMark(45);*/break; 
		}
	}
	//KHY - 1220 - 변신시스템.
	if(ppclKI->IsAtb(ATB_HERO))
	{
		DrawHero();
	}
	else
	{
		DrawNormal();
	}

	// [기형] 배틀로얄 중- 변신 상태 인지
	pclBattleRoyalClient = ((cltClient*)pclClient)->m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		if(TRUE == pclBattleRoyalClient->IsTransform(this))
		{
			return true;
		}
	}

	if((clTransFormInfo.bTransFormMode == true) && (IsPC(GetCharUnique())) &&(selectRealAnyType( ) == false))
	{
		if(clTransFormInfo.bTransForm100Effect ==true)
		{
			DrawTransformSyc100Effect();
		}
	}

	//----------------------
	//성황신을 경우 성황신 Effect출력
	//----------------------
	switch(clIdentity.siIdentity)
	{
	case IDENTITY_VILLAGEGOD:			break;
	case IDENTITY_CASTLE:				break;
	case IDENTITY_VILLAGEGOD_SUMMON:	break;
	case IDENTITY_PERSON_SUMMON:		/*DrawSummonEffect();*/	break; 
	case IDENTITY_VILLAGEHUNT:			break;
	}

	//-------------------------------------------
	// 가차 펫 이펙트
	//-------------------------------------------
	bool bGachaSummon = false;

	if( GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SUPERBEARTANG")) )
	{
		bGachaSummon = true;
	}

	if( bGachaSummon )
	{
		DrawGachaSummonEffect();
	}

	if( pclFourLeafEffect )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize ;
		pclFourLeafEffect->Draw( DrawStartX + ImageXsize/2, realyimagestarty );  
	}

	if(pclCushionCatEffect)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent ;
		pclCushionCatEffect->Draw( DrawStartX + ImageXsize/2, realyimagestarty ); 
	}

	//---------------------------
	// 선공 방지 효과가 있다면 출력한다. 
	//---------------------------
	if(bNoFirstAttackSwitch)
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_NOFIRSTATTACK);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 20;

			GP.PutSprLight1(pspr, startx,	starty,	 TABS((pclClient->CurrentClock / 200)) % 12 );
		}
	}
	//---------------------------
	// ICE 방지 효과가 설정되어 있다면 
	//---------------------------
	if(bNoIceAttackSwitch)
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_NOICEATTACK);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize - 20;

			GP.PutSpr(pspr, startx,	starty,	 TABS((pclClient->CurrentClock / 150)) % 20);
		}

	}

	// [진성] 버프 룰렛 작동중이면 주사위모냥 띄우기. => 2008-6-10
	if(bBuffRouletteSwitch)
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_DICE);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize - 20;

			GP.PutSpr(pspr, startx,	starty,	 TABS((pclClient->CurrentClock / 80)) % 12 );
		}
	}

	//-------------------------------------	
	// [진성] 엘릭서.
	//-------------------------------------
	if( bElixirSwitch )
	{
		TSpr* pspr = pclClient->GetGlobalSpr(GIMG_ELIXIR);

		if(pspr)
		{
			SI32 startx = DrawStartX + (ImageXsize / 2) - (pspr->GetXSize() / 2);

			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent * ImageYsize / 100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 70;   

			bool bReverse = false;
			switch( siDirection )
			{
			case SOUTH:								
			case SOUTH|WEST:						
			case SOUTH|EAST:											
			case EAST :	 		
				{
					bReverse = false;	
				}
				break;
			case WEST: 
			case NORTH|WEST:	
			case NORTH:			
			case NORTH|EAST:	
				{
					bReverse = true;	
				}
				break;
			}		  

			GP.PutSprScreen( pspr, startx,	starty,	 TABS((pclClient->CurrentClock / 80)) % 10, bReverse ); 
		}
	}

	//	PVP리그에 참가 한다.
	if (siEnemUnique > 0 ) 
	{
		bool bPVPMAP	=	false ;
		SI32 siMyPersonID = pclClient->pclCM->CR[1]->pclCI->GetPersonID()	;
		cltCharCommon* pclChar = pclClient->pclCM->GetChar(1)	;	// 클라니까 아이디 1 
		SI32 siMapIndex = pclChar->GetMapIndex()	;
		cltMapCommon* pclMap = pclClient->pclMapManager->GetMapPointer( siMapIndex )	;

		if(pclMap != NULL)
		{
			SI32 siColl = pclMap->GetCollMap( pclChar->GetX() , pclChar->GetY()	)	;

			switch( siMapIndex ) {
	case  MAPINDEX_MAINFIELD :
		{
			if ( siColl & BLOCK_EVENT4 ) //PVP리그 맵이고 경기장 안으로 존재 하면 
			{
				bPVPMAP = true	;
			}
		}
		break;
	case  MAPINDEX_PVPMAP :
		{
			if ( siColl & BLOCK_EVENT4 ) //PVP리그 맵이고 경기장 안으로 존재 하면 
			{
				bPVPMAP = true	;
			}
		}
		break;
			}
		}
		if ( true == bPVPMAP ) 
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_LEAGUE);

			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty = DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 30;
				// 말을 타고 있으면 좀더 위에다 출력한다. 
				SI32 siHorseUnique = 0;
				if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
				{
					starty -= 20;
				}
				
				GP.PutSpr(pspr, startx,	starty,	0 )	;
			}
		}
	}
	// [성웅 ] pvp 시작전 카운트 다운 
	if(bPvpCountDown)
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_COUNTDOWN);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;

			// 말을 타고 있으면 좀더 위에다 출력한다. 
			SI32 siHorseUnique = 0;
			if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
			{
				starty -= 50;
			}
			// 시간 조절 중
			SI32 sitime = (pclClient->CurrentClock) - siPVPWaitTime;

			if ( sitime > 1000 )
			{
				if ( 1000 <= sitime && 2000 > sitime ) 
				{
					GP.PutSpr(pspr, startx,	starty,	0 );
				}
				if ( 2000 <= sitime && 3000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	1 );
				}
				if ( 3000 <= sitime&& 4000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	2 );
				}
				if ( 4000 <= sitime&& 5000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	3 );
				}
				if ( 5000 <= sitime&& 6000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	4 );
				}
				if ( 6000 <= sitime && 7000 > sitime ) 
				{
					GP.PutSpr(pspr, startx,	starty,	5 );
				}
			}
		}
	}
	if(siPVPWin)// 승리 표시 
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_WLD);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 50;
			// 말을 타고 있으면 좀더 위에다 출력한다. 
			SI32 siHorseUnique = 0;
			if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
			{
				starty -= 50;
			}

			// 시간 조절 중
			SI32 sitime = (pclClient->CurrentClock) - siPVPEndTime ;
			if ( 0 <= sitime && 5000 > sitime ) 
			{
				GP.PutSpr(pspr, startx,	starty,	0 );//승리
			}
			else if(5000 < sitime)
			{
				siPVPWin	=	false	;
				siPVPLose	=	false	;
				siPVPDraw	=	false	;
				siPVPEndTime	=	0	;
			}
		}
	}
	if(siPVPLose)//  패배 표시 
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_WLD);
		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 50;
			// 말을 타고 있으면 좀더 위에다 출력한다. 
			SI32 siHorseUnique = 0;
			if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
			{
				starty -= 50;
			}

			// 시간 조절 중
			SI32 sitime = (pclClient->CurrentClock) - siPVPEndTime ;
			if ( 0 <= sitime && 5000 > sitime ) 
			{
				GP.PutSpr(pspr, startx,	starty,	1 );//승리
			}
			else if(5000 < sitime)
			{
				siPVPWin	=	false	;
				siPVPLose	=	false	;
				siPVPDraw	=	false	;
				siPVPEndTime	=	0	;
			}
		}
	}
	if(siPVPDraw)// 비김 표시
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_WLD);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize - 50;
			// 말을 타고 있으면 좀더 위에다 출력한다. 
			SI32 siHorseUnique = 0;
			if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
			{
				starty -= 50;
			}

			// 시간 조절 중
			SI32 sitime = (pclClient->CurrentClock) - siPVPEndTime ;
			if ( 0 <= sitime && 5000 > sitime ) 
			{
				GP.PutSpr(pspr, startx,	starty,	2 );//승리
			}
			else if( 5000 < sitime )
			{
				siPVPWin	=	false	;
				siPVPLose	=	false	;
				siPVPDraw	=	false	;
				siPVPEndTime	=	0	;
			}
		}
	}
	if ( pclValentineWar )
	{
		SI16 siCouple = pclValentineWar->GetCouple();
		if(siCouple > 0)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_EMOTICON);
			switch( siCouple ) {				
	case VALENTINEWAR_COUPLE:
		{
			if(pspr)
			{
				SI32 imagenum = 56	;
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize-30;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF ) 
					|| pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK ) ) //선공방지나 네잎클로버사용시 더 올린다.
				{
					starty = starty - 20 ;
				}
				else if ( bPvpCountDown == TRUE ) // pvp 카운트 할떄는 더 올린다.
				{
					starty = starty - 50 ;

				}
				// 시간 조절 해서
				SI32 sitemptimecount = (pclClient->CurrentClock/1000)	;
				SI32 timecount =	sitemptimecount%10;
				switch(timecount )
				{
				case 1:
					{imagenum	=	106	;}break;
				case 2:
					{imagenum	=	107	;}break;
				case 3:
					{imagenum	=	112	;}break;
				case 4:
					{imagenum	=	113	;}break;
				case 5:
					{imagenum	=	57	;}break;
				case 6:
					{imagenum	=	58	;}break;
				case 7:
					{imagenum	=	245	;}break;
				case 8:
					{ imagenum	=	246	; }	break;
				case 9:
					{ imagenum	=	106	; }break;								
				default:
					{ imagenum	=	57	; }break;
				}
				GP.PutSpr(pspr, startx,	starty,imagenum )		;
			}
		}break;
	case VALENTINEWAR_SOLO:
		{					
			if(pspr)
			{
				SI32 imagenum =  21	;
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 30;
				// 네잎이나 선공 방지 하면 좀더 올린다.
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF ) 
					|| pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK ) ) 
				{
					starty = starty - 20 ;
				}
				else if ( bPvpCountDown == TRUE ) 
				{
					starty = starty - 50 ;

				}
				// 시간 조절 해서
				SI32 sitemptimecount = (pclClient->CurrentClock/1000)	;
				SI32 timecount =	sitemptimecount%10;
				switch(timecount ) 
				{
				case 1:
					{imagenum	=	22	;}break;
				case 2:
					{imagenum	=	28	;}break;
				case 3:
					{imagenum	=	29	;}break;
				case 4:
					{imagenum	=	35	;}break;
				case 5:
					{imagenum	=	36	;}break;
				case 6:
					{imagenum	=	49	;}break;
				case 7:
					{imagenum	=	50	;}break;
				case 8:
					{ imagenum	=	84	; }	break;
				case 9:
					{ imagenum	=	85	; }break;								
				default:
					{ imagenum	=	21	; }break;
				}
				GP.PutSpr(pspr, startx,	starty,imagenum )		;
			}
		}break;
			}
		}
	}
	if(bRacoonDodgeEventCountDown)//Switch_10MonthEvent_RacoonDodge
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_COUNTDOWN);

		if(pspr)
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;

			// 말을 타고 있으면 좀더 위에다 출력한다. 
			SI32 siHorseUnique = 0;
			if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
			{
				starty -= 50;
			}
			// 시간 조절 중
			SI32 sitime = (pclClient->CurrentClock) - siRacoonDodgeWaitTime;

			if ( sitime > 1000 )
			{
				if ( 1000 <= sitime && 2000 > sitime ) 
				{
					GP.PutSpr(pspr, startx,	starty,	0 );
				}
				if ( 2000 <= sitime && 3000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	1 );
				}
				if ( 3000 <= sitime&& 4000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	2 );
				}
				if ( 4000 <= sitime&& 5000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	3 );
				}
				if ( 5000 <= sitime&& 6000 > sitime) 
				{
					GP.PutSpr(pspr, startx,	starty,	4 );
				}
				if ( 6000 <= sitime && 7000 > sitime ) 
				{
					GP.PutSpr(pspr, startx,	starty,	5 );
				}
			}
		}
	}

	if ( pclClient->IsCountrySwitch(Switch_BokBulBok) )
	{
		//////////////////////////////////////////////////////////////////////////
		// [지연] 복불복 이벤트를 위한 이미지 출력 (5초 카운트)
		if(bBBBEvent_5SecCountDownFlag)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_COUNTDOWN);

			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;

				// 말을 타고 있으면 좀더 위에다 출력한다. 
				SI32 siHorseUnique = 0;
				if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
				{
					starty -= 50;
				}
				// 시간 조절 중
				SI32 sitime = (pclClient->CurrentClock) - siBBBEvent_5SecCountDown;
				cltClient* pClient = (cltClient*)pclClient;
				SI32 siStartDelay = (pClient->m_pClient_BBBMgr->m_siStageStartDelay) / 1000;
				SI32 siStartIndex = 5 - siStartDelay;				// 5초 일땐 5부터, 4초일땐 4부터, 3초일땐 3부터 등등.. 시작 시간에 따라 달라지는 인덱스 가중치
				SI32 siCurrentImageIndex = sitime / 1000;			// 밀리초 단위의 경과시간을 1000으로 나눈 몫. 즉 0부터 시작

				if (0 <= siCurrentImageIndex + siStartIndex && 5 >= siCurrentImageIndex + siStartIndex )		// 이미지 인덱스는 5를 넘지 않는다.
				{
					GP.PutSpr(pspr, startx,	starty,	siCurrentImageIndex + siStartIndex );
				}

			}
		}

		// [지연] 복불복 이벤트를 위한 이미지 출력 (5초간 Lose 메세지)
		if(bBBBEvent_StageFailFlag)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_WLD);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 50;
				// 말을 타고 있으면 좀더 위에다 출력한다. 
				SI32 siHorseUnique = 0;
				if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
				{
					starty -= 50;
				}

				// 시간 조절 중
				SI32 sitime = (pclClient->CurrentClock) - siBBBEvent_StageFailCountDown ;

				cltClient* pClient = (cltClient*)pclClient;
				SI32 siDelay = pClient->m_pClient_BBBMgr->m_siStageLoseDelay;

				if ( siDelay > sitime )			// 5
				{
					GP.PutSpr(pspr, startx,	starty,	1 );
				}
			}
		}

		// [지연] 복불복 이벤트를 위한 이미지 출력 (5초간 Win 메세지)
		if(bBBBEvent_StageClearFlag)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PVP_WLD);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 50;
				// 말을 타고 있으면 좀더 위에다 출력한다. 
				SI32 siHorseUnique = 0;
				if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
				{
					starty -= 50;
				}

				// 시간 조절 중
				SI32 sitime = (pclClient->CurrentClock) - siBBBEvent_StageClearCountDown;

				cltClient* pClient = (cltClient*)pclClient;
				SI32 siDelay = pClient->m_pClient_BBBMgr->m_siStageWinDelay;

				if ( siDelay > sitime )			// 5
				{
					GP.PutSpr(pspr, startx,	starty,	0 );
				}
			}
		}

		// [지연] 복불복 이벤트를 위한 이미지 출력 (최종 10단계 완료 후 뜨는 win/lose ) 
		if(bBBBEvent_TotalStageClearFlag)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_BBB_WL);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;

				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 50;
				// 말을 타고 있으면 좀더 위에다 출력한다. 
				SI32 siHorseUnique = 0;
				if( (siHorseUnique = pclCI->clHorseInfo.GetCurrentRideHorseUnique()) != 0)
				{
					starty -= 10;
				}

				// 시간 조절 중
				SI32 sitime = pclClient->CurrentClock - siBBBEvent_TotalStageClearCountDown;
				sitime = sitime % 800;

				SI32 siCurrentIndex = (sitime / 200) + siBBBEvent_TotalStageClearResultImageIndex;		// 1초에 한번씩 애니메이션 풀로 재생
				GP.PutSpr(pspr, startx,	starty,	siCurrentIndex );
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}


	if(pclKI->clKIDrawInfo.siHeadNameFont >= 0)
	{
		DrawRankMark(pclKI->clKIDrawInfo.siHeadNameFont);
	}

	if ( pclCM->CR[1]->pclKI->IsAtb(ATB_HERO) )
	{
		cltSystemNPC clNPCInfo;
		if ( pclClient->pclSystemNPCManager->FindSystemNPCInfoFromNPCKind( GetKind(), &clNPCInfo ) )
		{
			if ( QUESTDISPLAY_MODE_NONE != clNPCInfo.m_siImageDisplayMode )
			{
				if ( false == pclCM->CR[1]->pclCI->clQuestInfo.IsAllClear(pclClient->pclQuestManager, clNPCInfo.m_siQuestType) )
				{
					SI16 siDisplayMark = GetQuestDisplayMark( &clNPCInfo );

					DrawQuestionMark( siDisplayMark );

					cltCharPos clCharPos(GetMapIndex(),GetX(),GetY() );

					((CNMiniMapDlg*)pclclient->m_pDialog[NMINIMAP_DLG])->SetMinimapNPC(GetKind(),&clCharPos);
				}
			}
		}
	}

	//----------------------
	// 파티 동료를 표시한다. 
	//----------------------
	if( pclclient->m_pDialog[ NPARTY_DLG ] )
	{
		if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyUser( GetCharUnique() ) == true )
		{
			TSpr* pspr	= pclClient->GetGlobalSpr( GIMG_PARTYEMOTICON );

			if(pspr)
			{	
				SI32 charheight = 30; //30 은 캐릭터의 가슴팍 정도 높이
				if(GetCurrentOrder() == ORDER_SITDOWN) charheight -= 18;

				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
				// by LEEKH 2007.12.24 - TRANSFORM
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2  -charheight; 
				pclPartyEmoticon->Draw( pspr, startx, starty );
			}
		}
	}


	// 치료효과를 출력한다. 
	DWORD healinterval = TABS(pclClient->CurrentClock - dwHealClock);
	SI32 healframe		= 12;
	SI32 healdelay		= 100; //밀리세컨드
	if((SI32)healinterval < healframe * healdelay)
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_HEAL);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2;

			GP.PutSprAdd(pspr, startx,		starty,	 healinterval / healdelay);

		}
	}

	// 수호정령 특수모션 이펙트를 표시한다
	if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		DWORD	dwSoulGuardSF_Interval	= TABS(pclclient->CurrentClock - dwSoulGuardSF_Clock);
		TSpr*	psprSoulGuardSF			= NULL;

		SI32	siDrawDelay				= 100;

		// 새로운 카인트 추가시 이곳에 넣으면 됨
		if ( pclClient->GetUniqueFromHash("KIND_SOULGUARD_TINKY") == siSoulGuardSF_Kind )
		{
			psprSoulGuardSF = pclClient->GetGlobalSpr( GIMG_HEAL_Y );
		}
		// PCK : 수호정령 - 불씨정령 추가 (09.07.03)
		else if ( pclClient->GetUniqueFromHash("KIND_SOULGUARD_FIRESPIRIT") == siSoulGuardSF_Kind )
		{
			psprSoulGuardSF = pclClient->GetGlobalSpr( GIMG_HEAL_R );
		}
		// 펌킨
		else if ( pclClient->GetUniqueFromHash("KIND_SOULGUARD_PUMPKIN") == siSoulGuardSF_Kind )
		{
			psprSoulGuardSF = pclClient->GetGlobalSpr( GIMG_HEAL_P );
		}
		// 얼음정령
		else if ( pclClient->GetUniqueFromHash("KIND_SOULGUARD_ICESPIRIT") == siSoulGuardSF_Kind )
		{
			psprSoulGuardSF = pclClient->GetGlobalSpr( GIMG_HEAL_B );
		}
		else if ( pclClient->GetUniqueFromHash("KIND_SOULGUARD_TK_EVENT") == siSoulGuardSF_Kind )
		{
			psprSoulGuardSF = pclClient->GetGlobalSpr( GIMG_HEAL_Y );
		}
		/* 요런식으로 추가해 주세요
		else if ( pclClient->GetUniqueFromHash("KIND_SOULGUARD_TINKY") == siSoulGuardSF_Kind )
		{
			psprSoulGuardSF = pclClient->GetGlobalSpr( GIMG_HEAL );
		}
		*/

			if ( (psprSoulGuardSF)																	// 이미지가 있다면
				&& (0 < psprSoulGuardSF->GetImageNum())												// 이미지가 프레임이 0보다 크다면
				&& ((SI32)dwSoulGuardSF_Interval < (psprSoulGuardSF->GetImageNum()*siDrawDelay))	// 이팩트를 뿌려줄 수 있다면
				)
			{
				SI32 startx		= DrawStartX + ImageXsize/2 - psprSoulGuardSF->GetXSize() /2;
				SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
				SI32 starty		= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - psprSoulGuardSF->GetYSize() /2;

				GP.PutSprAdd( psprSoulGuardSF, startx, starty, (dwSoulGuardSF_Interval/siDrawDelay) );
			}
	}


	//마을 성문 이라면
	if(GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_CASTLE"))   && pclClient->bVillageWarSwitch 
		&&	 pclCM->IsValidID(GetID())
		&&   pclClient->pclVillageManager->IsVillageAtWar( pclCM->CR[GetID()]->pclCI->clBI.siHomeVillage ) )
	{
		SI32 siCastleLife =  pclCM->CR[GetID()]->clPB.GetMaxLife(); // pclCM->CR[GetID()]->pclCI->clIP.siVit*4;

		if( pclCM->CR[GetID()]->GetLife() <  siCastleLife * 0.8)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE4);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 ;
				SI32 starty	= DrawStartY - 40;//+ 30;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = TABS((pclClient->CurrentClock / 100)) % pspr->GetImageNum() ;	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
		if( pclCM->CR[GetID()]->GetLife() <  siCastleLife * 0.6)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE3);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 + 100 ;
				SI32 starty	= DrawStartY - 200;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = TABS((pclClient->CurrentClock / 100)) % pspr->GetImageNum();	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
		if( pclCM->CR[GetID()]->GetLife() <  siCastleLife * 0.4)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE2);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 + 80;
				SI32 starty	= DrawStartY -250;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = TABS((pclClient->CurrentClock / 100)) % pspr->GetImageNum();	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
		if( pclCM->CR[GetID()]->GetLife() <  siCastleLife * 0.2)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE1);
			if(pspr)
			{
				SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 + 210;
				SI32 starty	= DrawStartY - 260;//+ 30;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = TABS((pclClient->CurrentClock / 100)) % pspr->GetImageNum();	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
	}

	// 독에 중독되었을 경우
	if(clInnerStatus.IsStatus(INNERSTATUS_POISON))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_POISON);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2 - 30;
			//GP.PutSprScreen(pspr, startx,		starty, pclClient->GetFrame()%pspr->GetImageNum());
			if(pspr->GetImageNum() > 0)	GP.PutSprScreen(pspr, startx, starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());

		}
	}
	
	//[진성] 세트 아이템 독에 중독되었을 경우
	if( clInnerStatus.IsStatus( INNERSTATUS_POISON_SETITEM_EFFECT ) )
	{
		TSpr* pspr = pclClient->GetGlobalSpr(GIMG_SETITEM_POISON); 
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = (DrawStartX + ImageXsize/2 - pspr->GetXSize() / 2) - 5;   
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2 - 50;
			//GP.PutSprScreen(pspr, startx,		starty, pclClient->GetFrame()%pspr->GetImageNum());
			if(pspr->GetImageNum() > 0)	GP.PutSprScreen(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 80))%pspr->GetImageNum());  
			if(pspr->GetImageNum() > 0)	GP.PutSprScreen(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 40))%pspr->GetImageNum());     

		}
	}

	

	// 얼음에 얼렸을 경우 
	if(clInnerStatus.IsStatus(INNERSTATUS_ICE))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_ICEFROZEN);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - /*realysize/2 -*/ pspr->GetYSize()/2;
			GP.PutSprAdd(pspr, startx,		starty, 0);
		}
	}

	// 얼음성 보스 몹에게 언 경우
	if(clInnerStatus.IsStatus(INNERSTATUS_FROZEN))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGICFROZEN);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - /*realysize/2 -*/ pspr->GetYSize()/2-50;
			//GP.PutSprScreen(pspr, startx,		starty, pclClient->GetFrame()%pspr->GetImageNum());
			GP.PutSprScreen(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());

		}
	}
	// 스턴이 걸렸을 경우 // 이경우에는 일단 그림을 아이스로 해준다 지금은.
	if(clInnerStatus.IsStatus(INNERSTATUS_STUN))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_STUN);
		if(pspr) 
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - /*realysize/2 -*/ pspr->GetYSize()/2;
			GP.PutSprScreen(pspr, startx,	starty - 90, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum());   
		}
	}

	if( cltCharCurseEffect::CURSE_NONE != m_clCharCurseEffect.GetCurse( GetID() ) )
	{
		TSpr* pspr	= NULL;
		switch( m_clCharCurseEffect.GetCurse( GetID() ) )
		{
			case cltCharCurseEffect::CURSE_ATTACK:	pspr	= pclClient->GetGlobalSpr(GIMG_CURSE_ATTACK);	break;
			case cltCharCurseEffect::CURSE_DEFENSE:	pspr	= pclClient->GetGlobalSpr(GIMG_CURSE_DEFENSE);	break;
			case cltCharCurseEffect::CURSE_DODGE:	pspr	= pclClient->GetGlobalSpr(GIMG_CURSE_DODGE);	break;
			case cltCharCurseEffect::CURSE_HIT:		pspr	= pclClient->GetGlobalSpr(GIMG_CURSE_HIT);		break;
		}

		if(pspr) 
		{
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - realysize/2 - ppclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize();  
			GP.PutSprScreen(pspr, startx,	starty - 20, max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum()); 
		}	
	}
	
	// 공격력 저하 마법에 걸렸을 경우 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSEATTACK))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_CURSEATTACK);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize()/2;
			//GP.PutSprScreen(pspr, startx,		starty, pclClient->GetFrame()%pspr->GetImageNum());
			GP.PutSprScreen(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());

		}
	}

	// 크리티컬 저하 마법에 걸렸을 경우 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSECRITICAL))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_CURSECRITICAL); 
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize()/2;
			GP.PutSprAdd(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());
		}
	}

	// 방어력 저하 마법에 걸렸을 경우 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSEDEFENSE))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_CURSEDEFENSE);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize()/3;
			GP.PutSprAdd(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());
		}
	}

	// 이동속도저하 마법에 걸렸을 경우 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSEMOVESPEED))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_CURSEMOVESPEED);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
			SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize ;
			GP.PutSprLight(pspr, startx,		starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());
		}
	}

	// 불 붙는 마법에 걸렸을 경우
	if(clInnerStatus.IsStatus(INNERSTATUS_MAGICFIRE))
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGICFIRE);
		if(pspr)
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2 - 50 ;
			GP.PutSprScreen(pspr, startx,	starty, max(0, TABS(pclClient->CurrentClock / 50))%pspr->GetImageNum());
		}
	}


	// Drain Life인경우 
	if(bDrainSwitch)
	{
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_DRAIN);
		if(pspr)
		{
			SI32 font = 0;
			SI32 siFrameNum = pspr->GetImageNum()/5;
			bool reverseswitch = false;
			switch(siDirection)
			{
			case SOUTH:
				reverseswitch=false;
				font=siFrameNum*1+siDrainFrame;
				break;
			case SOUTH|WEST:
				reverseswitch=false;
				font=siFrameNum*2+siDrainFrame;
				break;
			case WEST:
				reverseswitch=false;
				font=siFrameNum*3+siDrainFrame;
				break;
			case NORTH|WEST:
				reverseswitch=false;
				font=siFrameNum*4+siDrainFrame;
				break;
			case NORTH:
				reverseswitch=true;
				font=siFrameNum*3+siDrainFrame;
				break;
			case NORTH|EAST:
				reverseswitch=true;
				font=siFrameNum*2+siDrainFrame;
				break;
			case EAST:
				reverseswitch=true;
				font=siFrameNum*1+siDrainFrame;
				break;
			case EAST|SOUTH:
				reverseswitch=false;
				font=siFrameNum*0+siDrainFrame;
				break;
			default:
				reverseswitch = false;
				font=0;
				break;
			}

			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
			SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2 - 30;
			GP.PutSprAdd(pspr, startx,		starty, font , reverseswitch);

			siDrainFrame++;
			if(siDrainFrame >= siFrameNum) {siDrainFrame = 0; bDrainSwitch =false;}
		}
	}

	if(pclRegeneration)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 centery	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - 65;

		pclRegeneration->DrawCenter(DrawStartX+ImageXsize/2, centery );
	}


	if(pclCharCut)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 centery	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2;

		pclCharCut->Draw(DrawStartX+ImageXsize/2, centery);
	}

	if ( pclLevelUp )
	{
		// 유저 캐릭
		if( IsPC( GetCharUnique() ) )
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;  
			pclLevelUp->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
		}
		// 소환수등
		else
		{
			// [진성] 소환펫 레벨업 이미지 좌표 다르게 수정.
			pclLevelUp->DrawNpc( DrawStartX + ImageXsize / 2, DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent );  
		}
	}

	if ( pclMVPEffect )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		pclMVPEffect->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
	}

	if( pclEnchantEff)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		pclEnchantEff->Draw( DrawStartX + ImageXsize/2, realyimagestarty, Animation  );
	}
	if( pclQuestEff)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		pclQuestEff->Draw( DrawStartX + ImageXsize/2, realyimagestarty);
	}
	if(pclExpCounter)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		pclExpCounter->Draw( DrawStartX + ImageXsize/2, realyimagestarty );

	}
	if(pclHealCounter)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize - 10;
		pclHealCounter->Draw( DrawStartX + ImageXsize/2, realyimagestarty );

	}
	if ( pclSkillLevelUp )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		pclSkillLevelUp->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
	}
	if ( pclEmoticonEff )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		pclEmoticonEff->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
	}

	if ( m_pclValentineEmoticonEff )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
		m_pclValentineEmoticonEff->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
	}

	if ( pclCriticalEffect )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 centery	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2;
		pclCriticalEffect->Draw( DrawStartX+ImageXsize/2, centery );
	}


	if ( pclJim )
	{
		// 소환수는 찜표시를 하지 않도록 한다.
		if(GetLife() > 0 && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON) 
		{
			// by LEEKH 2007.12.24 - TRANSFORM
			SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
			SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize;
			pclJim->Draw( DrawStartX + ImageXsize/2, realyimagestarty );
		}

		// 내소환수가 누군가로 부터 찜당하면 HealthBar를 그려준다.
		else if( pclJim->IsJimStatus() && 
			clIdentity.siIdentity == IDENTITY_PERSON_SUMMON &&
			pclCM->CR[1]->siChildCharUnique == GetCharUnique() ) 
		{
			bDrawHealthBarSwitch = true;
		}
	}

	// 찜이 설정되지 않는 캐릭터인 경우 체력 게이지를 보여준다. 
	if(pclKI->IsAtb(ATB_NOJJIM))
	{
		bDrawHealthBarSwitch = true;
	}


	// 정보가 표시되는 캐릭터인 경우에는 그 크기도 표시한다. 
	if(GetCharUnique() && pclClient->pclDevTool[DEV_TOOL_CHARINFO]->GetCharUnique() == GetCharUnique())
	{
		GP.Box(DrawStartX, DrawStartY, 
			DrawStartX+ImageXsize-1, DrawStartY+ImageYsize-1, 
			10);

		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 realxsize = ppclKI->clKIDrawInfo.siXsizePercent*ImageXsize/100;
		SI32 realysize = ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

		// by LEEKH 2007.12.24 - TRANSFORM
		GP.Box(DrawStartX + (ImageXsize-realxsize)/2 ,			
			DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize,
			DrawStartX + (ImageXsize-realxsize)/2 + realxsize,
			DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent, 
			20);
	}


	//-----------------------------
	// 마우스 아래 캐릭터이면, 
	// 혹은 찜 되어 있는 캐릭터이면, 
	//-----------------------------
	if(GetLife() > 0 && (IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique())))
	{
		cltClient* pclclient = (cltClient*)pclClient;
		if(clJim.siJimHostCharUnique && (clJim.siJimHostCharUnique == pclCM->CR[1]->GetCharUnique()))
		{
			bDrawHealthBarSwitch = true;
		}
	}

	//-----------------------------
	// 공성전 중,
	// 내가 PC 이고 주인공과의 적대적인 관계라면 나의 Health Bar를 그려준다.
	//-----------------------------
	/*if(pclClient->bVillageWarSwitch == true)
	{
		if(IsPC(GetCharUnique()) && IsEnemy(1))
		{
			bDrawHealthBarSwitch = true;
		}
	}*/


		//-----------------------------
		// 소속이 있는 특수 캐릭터들은 항상 에너지바를 그려준다.
		//-----------------------------
		switch(clIdentity.siIdentity)
	{
		case IDENTITY_CASTLE:	
		case IDENTITY_VILLAGEGOD:
		case IDENTITY_VILLAGEGOD_SUMMON:	bDrawHealthBarSwitch = true;					break;
			// 소환수 에너지바 안보여준다.
			//// 적일떄 보여준거 안보여준다.
			//case IDENTITY_PERSON_SUMMON:	if( IsEnemy(1) == false )	bDrawHealthBarSwitch = true;	break;
		case IDENTITY_VILLAGEHUNT:			bDrawHealthBarSwitch = true;					break;
		case IDENTITY_MOFUMOFUEVENT:		bDrawHealthBarSwitch = true;					break;
	}

	if ( GetID() == 1 )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 startx = DrawStartX + ImageXsize/2+3;
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent+145;
		pclClient->pclQuestManager->DrawDirectionArrow( startx , starty );
	}


	// 체력바를 보이는 설정이 되어 있고 체력이 유효하며 체력바를 안 보여주는 속성이 없다면 
	if(bDrawHealthBarSwitch == true && GetLife() > 0 && pclKI->IsAtb(ATB_NOHEALTHBAR) == FALSE )
		DrawHealthBar();

	//----------------------
	// 버프 마법 이펙트
	//----------------------
	{
		SI32 realysize			= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 realyimagestarty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize ;
		m_clMagicBufMgr.Draw( DrawStartX + ImageXsize/2, realyimagestarty );  
	}

	/*cyj
	// pc방 유저일 경우 표시 
	if( pclCI->clBI.IsPCRoom() ) {
		TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_PCROOM_MARK);

		if( pspr ) {

			SI32 startx = DrawStartX + 130;
			SI32 starty	= DrawStartY + 118;

			if( pclCI->clCharRank.GetRankType() == 0 ) {
				starty += 12;
			}
			GP.PutSpr(pspr, startx, starty, 0);
		}
	}
	*/


		// TEST TSET TEST TSST
		//DrawHealthBar();

		/*
		#ifdef _DEBUG
		// 캐릭터 조합에서 출력되는 이미지 개수 볼려고...
		if ( pclClient->GetFrame() % 200 == 0 )
		{
		int ImageNum = pclCharDraw->GetLoadedImageNum();
		TCHAR Buffer[256]=TEXT("");
		sprintf( Buffer, TEXT("Character ImageNum = %d"), ImageNum );
		pclClient->pclMessage->SetMsg( Buffer );
		}
		#endif
		*/

		//------------------------------------
		// 찜 표시 
		//------------------------------------
		/*	if(clJim.siJimHostCharUnique)
		{
		UI08 color = 0;
		if(pclClient->IsUser(clJim.siJimHostCharUnique))
		{
		color = 100;
		}
		else
		{
		color = 50;
		}

		SI32 realxsize = pclKI->clKIDrawInfo.siXsizePercent*ImageXsize/100;
		SI32 realysize = pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;

		GP.Box(DrawStartX + (ImageXsize-realxsize)/2 ,			
		DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - realysize,
		DrawStartX + (ImageXsize-realxsize)/2 + realxsize,
		DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent, 
		color);

		}
		*/


		return true;
}

//Game Direction을 CDD Direction으로 변경한다.
SI32 cltCharClient::TransDir(SI32 gamedir)
{
	switch(gamedir)
	{
	case SOUTH:			return CCD_SOUTH;
	case SOUTH|WEST:	return CCD_SOUTH_WEST;
	case WEST:			return CCD_WEST;
	case NORTH|WEST:	return CCD_NORTH_WEST;
	case NORTH:			return CCD_NORTH;
	case NORTH|EAST:	return CCD_NORTH_EAST;
	case EAST:			return CCD_EAST;
	case SOUTH|EAST:	return CCD_SOUTH_EAST;
	default:	return CCD_SOUTH_EAST; 
	}
}

bool cltCharClient::DrawHero()
{
	SI32 weapon = 0;
	SI32 helmet = 0;
	SI32 armour = 0;
	SI32 mantle = 0;
	bool horse	= false;

	UI32 rareInfo = 0;
	SI32 enchantType = 0;
	SI32 enchantPower = 0;

	SI32 PremiumPartsUnique_Slot1	=	0 ;
	SI32 PremiumPartsUnique_Slot2	=	0 ;
	SI32 PremiumPartsUnique_Slot3	=	0 ;

	if(pclCI->clCharItem.clItem[ActiveWeapon].clItemCommon.clCommonInfo.uiRareInfo)
	{
		rareInfo = pclCI->clCharItem.clItem[ActiveWeapon].clItemCommon.clCommonInfo.uiRareInfo;
	}
	else 
	{
		rareInfo = clPB.clShapeEquip.clWeapon.uiRare;
	}

	if( pclClient->pclItemManager->IsExpiredDate(&pclCI->clCharItem.clItem[ActiveWeapon]) == true &&
		pclCI->clCharItem.clItem[ActiveWeapon].Element.siType )
	{
		enchantType = pclCI->clCharItem.clItem[ActiveWeapon].Element.siType;
		enchantPower = pclCI->clCharItem.clItem[ActiveWeapon].Element.siPower;
	}
	else
	{
		enchantType = clPB.clShapeEquip.clWeapon.uiEnchantType;
		enchantPower = clPB.clShapeEquip.clWeapon.uiEnchantPower;
	}

	//[추가 : 황진성 2007. 12. 12. 신년 과금 복장 빤딱이 보이게. 일단보류 : 다른방법으로...]
	SYSTEMTIME sTime;
	GetLocalTime( &sTime );

	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		SI32	Unique[] = {97, 172, 372, 272, 468, 568};
		SI32	Cnt		 = sizeof(Unique)/ sizeof(SI32);
		for(SI32 i = 0; i < Cnt; ++i)
		{
			if(Unique[i] == pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique)		
			{
				if(RARELEVEL_6 != pclCI->clCharItem.clItem[ ActiveWeapon ].clItemCommon.clCommonInfo.uiRareInfo)
				{				
					rareInfo = NEWYEAR_EFFECT;
				}
			}
		}
	}

	//if(true == NewYearEventDay())
	//{		
	//	SI32	Unique[] = {97, 172, 372, 272, 468, 568};
	//	SI32	Cnt		 = sizeof(Unique)/ sizeof(SI32);
	//	for(SI32 i = 0; i < Cnt; ++i)
	//	{
	//		if(Unique[i] == pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique)		
	//		{
	//			if(RARELEVEL_6 != pclCI->clCharItem.clItem[ ActiveWeapon ].clItemCommon.clCommonInfo.uiRareInfo)
	//			{				
	//				rareInfo = NEWYEAR_EFFECT;
	//			}
	//		}
	//	}
	//}


	// 무기 이펙트 설정.
	weapon = CheckJewelEffect( rareInfo , enchantType , enchantPower );

	rareInfo = 0;
	enchantType = 0;
	enchantPower = 0;	

	//-----------------------------------------------------------------------
	// 현재 보여지는 옷과 모자를 찾는다 - 이후 효과는 이것을 기준으로.
	SI32 AvailableHat = ActiveHat;
	SI32 AvailableClothes = ActiveClothes;

	if(pclCI->clCharItem.clItem[ ActiveHat ].siUnique== 0)	
	{
		if(AvailableHat==PERSONITEM_HAT)
			AvailableHat=PERSONITEM_HELMET;
		else
			AvailableHat=PERSONITEM_HAT;
	}

	if(pclCI->clCharItem.clItem[ ActiveClothes ].siUnique ==0)
	{
		if(AvailableClothes==PERSONITEM_DRESS)		
			AvailableClothes= PERSONITEM_ARMOUR;
		else
			AvailableClothes=PERSONITEM_DRESS;

	}

	SI32 NowActiveClothesUnique = pclCI->clCharItem.clItem[AvailableClothes].siUnique;		// 현재 보여지는 의목
	SI32 NowActiveHatUnique = pclCI->clCharItem.clItem[AvailableHat].siUnique;			// 현재 보여지는 모자
	//-----------------------------------------------------------------------


	if ( pclCI->clCharItem.clItem[ AvailableHat ].siUnique > 0 && pclCI->clCharItem.clItem[AvailableHat].clItemCommon.clCommonInfo.uiRareInfo )
	{		
		rareInfo = pclCI->clCharItem.clItem[AvailableHat].clItemCommon.clCommonInfo.uiRareInfo;
	}
	else
	{
		rareInfo = clPB.clShapeEquip.clHelmet.uiRare;
	}

	if ( pclCI->clCharItem.clItem[ AvailableHat ].siUnique > 0 && pclCI->clCharItem.clItem[AvailableHat].Element.siType )
	{		
		enchantType = pclCI->clCharItem.clItem[AvailableHat].Element.siType;
		enchantPower = pclCI->clCharItem.clItem[AvailableHat].Element.siPower;
	}
	else
	{
		enchantType = clPB.clShapeEquip.clHelmet.uiEnchantType;
		enchantPower = clPB.clShapeEquip.clHelmet.uiEnchantPower;		
	}

	//[추가 : 황진성 2007. 12. 12. 신년 과금 복장 빤딱이 보이게. 일단보류 : 다른방법으로...]

	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		//[추가 : 황진성 2007. 12. 12. 신년 과금 복장 빤딱이 보이게.]
		if(pclClient->pclItemManager->IsItemInfoAtb( NowActiveHatUnique, ITEMINFOATB_ITEMMALL ))
		{
			rareInfo = NEWYEAR_EFFECT;
		}
	}

	// PCK : 결혼 예복 이펙트 그리기 (09.05.07)
	// 결혼 의복과 결혼 모자를 두개 다 착용했을 경우에만 이펙트를 그려준다.
	if ( (NowActiveHatUnique == ITEMUNIQUE(23117) && NowActiveClothesUnique == ITEMUNIQUE(23696))  // 결혼 예식 머리 + 결혼 예식용 턱시도  이거나
		||  (NowActiveHatUnique == ITEMUNIQUE(23118) && NowActiveClothesUnique == ITEMUNIQUE(23698)))     // 결혼용 면사포 +  웨딩용 드레스 일때만.
	{
		// 결혼했을 경우에만 이펙트를 그려준다.
		if(IsMarried())
		{
			rareInfo = MARRIED_FULLDRESS_EFFECT;
		}
	}

	bool SmallEffect = false;
	if(pclClient->siServiceArea == ConstServiceArea_Japan)
		SmallEffect = true;

	// 모자 이펙트 설정
	helmet = CheckJewelEffect( rareInfo , enchantType , enchantPower, SmallEffect );
	//----------------------------------------------------------------------------------------------

	rareInfo = 0;
	enchantType = 0;
	enchantPower = 0;	

	if ( pclCI->clCharItem.clItem[ AvailableClothes ].siUnique > 0 && pclCI->clCharItem.clItem[ AvailableClothes ].clItemCommon.clCommonInfo.uiRareInfo )
	{
		rareInfo = pclCI->clCharItem.clItem[ AvailableClothes ].clItemCommon.clCommonInfo.uiRareInfo;
	}	
	else
	{
		rareInfo = clPB.clShapeEquip.clArmour.uiRare;
	}

	if ( pclCI->clCharItem.clItem[ AvailableClothes ].siUnique > 0 && pclCI->clCharItem.clItem[ AvailableClothes ].Element.siType )
	{
		enchantType = pclCI->clCharItem.clItem[ AvailableClothes ].Element.siType;
		enchantPower = pclCI->clCharItem.clItem[ AvailableClothes ].Element.siPower;
	}	
	else
	{
		enchantType = clPB.clShapeEquip.clArmour.uiEnchantType;
		enchantPower = clPB.clShapeEquip.clArmour.uiEnchantPower;
	}

	//[추가 : 황진성 2007. 12. 12. 신년 과금 복장 빤딱이 보이게. 일단보류 : 다른방법으로...]
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		if(pclClient->pclItemManager->IsItemInfoAtb( pclCI->clCharItem.clItem[ AvailableClothes ].siUnique, ITEMINFOATB_ITEMMALL ))
		{
			rareInfo = NEWYEAR_EFFECT;
		}
	}

	// PCK : 결혼 예복 이펙트 그리기 (09.05.07)
	// 결혼 의복과 결혼 모자를 두개 다 착용했을 경우에만 이펙트를 그려준다.
	if ( (NowActiveHatUnique == ITEMUNIQUE(23117) && NowActiveClothesUnique == ITEMUNIQUE(23696))  // 결혼 예식 머리 + 결혼 예식용 턱시도  이거나
		||  (NowActiveHatUnique == ITEMUNIQUE(23118) && NowActiveClothesUnique == ITEMUNIQUE(23698)))     // 결혼용 면사포 +  웨딩용 드레스 일때만.
	{
		// 결혼했을 경우에만 이펙트를 그려준다.
		if(IsMarried())
		{
			rareInfo = MARRIED_FULLDRESS_EFFECT;
		}
	}

	// 의복 이펙트 설정.
	armour = CheckJewelEffect( rareInfo , enchantType , enchantPower, SmallEffect );

	rareInfo = 0;
	enchantType = 0;
	enchantPower = 0;	


	if( pclCI->clCharItem.clItem[PERSONITEM_MANTLE].clItemCommon.clCommonInfo.uiRareInfo )
	{
		rareInfo = pclCI->clCharItem.clItem[PERSONITEM_MANTLE].clItemCommon.clCommonInfo.uiRareInfo;
	}
	else
	{
		rareInfo = clPB.clShapeEquip.clMantle.uiRare;
	}

	if( pclCI->clCharItem.clItem[PERSONITEM_MANTLE].Element.siType )
	{
		enchantType = pclCI->clCharItem.clItem[PERSONITEM_MANTLE].Element.siType;
		enchantPower = pclCI->clCharItem.clItem[PERSONITEM_MANTLE].Element.siPower;
	}
	else
	{
		enchantType = clPB.clShapeEquip.clMantle.uiEnchantType;
		enchantPower = clPB.clShapeEquip.clMantle.uiEnchantPower;
	}    	

	mantle = CheckJewelEffect( rareInfo , enchantType , enchantPower );

	rareInfo = 0;
	enchantType = 0;
	enchantPower = 0;	


	//KHY - 1001 - 기승동물 5마리 보유 수정.	
	cltHorse *pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclHorse != NULL)
	{
		if ( (pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE)  )
		{
			if( pclHorse->siStatusTicketNum > 0 && pclClient->pclHorseManager->GetAddStatusLimitDateVary(pclHorse) > 0 &&
				((cltClient*)pclClient)->clClientDate.GetDateVary() < pclClient->pclHorseManager->GetAddStatusLimitDateVary(pclHorse) ) // 능력 향상중이면서 타고 있는지
			{
				horse = true ;
			}
		}

	}

	//pclCI->clHorseInfo.siCurrentHorseIndex >= 0 ) // 능력 향상중이면서 타고 있는지

	// 실질적인 공격동작중. 
	SI32 attacklayerskill		= 0;
	bool battacklayerskillmode	= 0;
	SI08 siAttackScrollNum		= 0;	// 무기 사용술 부적수
	SI32 attackgimg				= 0;
	SI32 effectx				= 0;
	SI32 effecty				= 0;
	SI32 effectfont				= 0;
	bool effectreverse			= false;
	SI16 siHorseUnique			= 0;

	// 밑에 수치 데이터를 추가하면 오토바이마다 밑으로 내리는 수치를 조정할 수 있다
	SI32 siMotorCycleStartY = 0;
	SI32 siCurrentOrder = GetCurrentOrder();
	if( pclCI->clHorseInfo.GetCurrentHorseRIDEHORSE() && siCurrentOrder != ORDER_SITDOWN 
		&& siCurrentOrder != ORDER_FARM && siCurrentOrder != ORDER_MINE && siCurrentOrder != ORDER_FISH && siCurrentOrder != ORDER_DIE) 
	{
		cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
		if(pclHorse && pclHorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
		{ 
			siMotorCycleStartY = 13;
		}
		// 사수리는 날고 있는 것처럼 보여야 한다.
		else if(pclHorse && pclHorse->siHorseUnique == HORSEUNIQUE_SASURI)
		{ 
			siMotorCycleStartY = -50;
		}

	}
	// 오토바이는 바퀴가 짧아서 밑으로 내려야 한다.
	DrawStartY += siMotorCycleStartY;
	if( pclCI->clHorseInfo.GetCurrentHorseRIDEHORSE() ) 
	{
		cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
		if( pclHorse )
		{
			PremiumPartsUnique_Slot1 = pclHorse->siPremiumUnique_Slot1 ;
			PremiumPartsUnique_Slot2 = pclHorse->siPremiumUnique_Slot2 ;
			PremiumPartsUnique_Slot3 = pclHorse->siPremiumUnique_Slot3 ;
			siHorseUnique	=	pclHorse->siHorseUnique;
		}
	}

	pclCharDraw->DrawCharacter(	HeroAnimation, 
								TransDir(siDirection), 
								clAniInfo.GetAniStep(),
								DrawStartX, 
								DrawStartY,
								weapon, 
								helmet, 
								armour, 
								mantle,
								horse,
								battacklayerskillmode, 
								siAttackScrollNum,
								&effectreverse,
								&effectx,
								&effecty, 
								&effectfont,
								&attackgimg,
								PremiumPartsUnique_Slot1,
								PremiumPartsUnique_Slot2,
								PremiumPartsUnique_Slot3, 
								siHorseUnique );

	if( HeroAnimation == SPCU || HeroAnimation == SPCR)
	{

		SI16 LevelUpActionNum = 11;

		if(clAniInfo.GetAniStep() >= LevelUpActionNum)
		{
			bLevelUpDrawSwitch = false;
		}
	}

	// 앉아 있을 때는 셋트효과를 그리지 않는다.
	//if(GetCurrentOrder() != ORDER_SITDOWN)
	{
		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if( m_siSetItemType > 0)
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_1);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						} 
					}
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_1);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{ 
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pspr->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}    
							GP.PutSprScreen(pspr, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2); 
						}
					} 
				}
			}
			if( m_siSetItemType2 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_2);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						} 
					}
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_2);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{ 
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pspr->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}    
							GP.PutSprScreen(pspr, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2); 
						}
					} 
				}
			}

			if( m_siSetItemType3 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_3);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						} 
					}
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_3);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{ 
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pspr->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}    
							GP.PutSprScreen(pspr, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2); 
						}
					} 
				}
			}

			if( m_siSetItemType4 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_4);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						} 
					}
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_4);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{ 
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pspr->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}    
							GP.PutSprScreen(pspr, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2); 
						}
					} 
				}
			}

			if( m_siSetItemType5 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_5);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						} 
					}
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_5);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{ 
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pspr->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}    
							GP.PutSprScreen(pspr, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2); 
						}
					} 
				}
			}
		}
		else
		{
			switch ( m_siSetItemType )
			{
			case VAMPIRE_SETITEM:
				//case BLACKKNIGHT_SETITEM_STEP:
				{
					TSpr* pspr = pclClient->GetGlobalSpr( GIMG_SETITEMEFFECT0 );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						}
					}
					TSpr *pSpr2 = pclClient->GetGlobalSpr( GIMG_SETITEMEFFECT2 );
					if ( pSpr2 )
					{
						if(pSpr2->GetImageNum() > 0)
						{
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pSpr2->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}
							GP.PutSprScreen(pSpr2, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2);
						}
					}
				}
				break;
			case HALLOWEEN1_SETITEM:
			case HALLOWEEN2_SETITEM:
			case HALLOWEEN3_SETITEM:
			case HALLOWEEN4_SETITEM:
				{
					TSpr* pspr = pclClient->GetGlobalSpr( GIMG_SETITEME6 );
					if ( pspr )
					{
						if(pspr->GetImageNum() > 0)
						{
							m_siSetItemImgAnimationCounter1 = m_siSetItemImgAnimationCounter1%pspr->GetImageNum();
							GP.PutSprScreen(pspr,  DrawStartX + 105 , DrawStartY + 150, m_siSetItemImgAnimationCounter1++ );
						}
					}
					TSpr *pSpr2 = pclClient->GetGlobalSpr( GIMG_SETITEMEFFECT6 );
					if ( pSpr2 )
					{
						if(pSpr2->GetImageNum() > 0)
						{
							if( pclClient->GetFrame() % 2 == 0)
							{
								m_siSetItemImgAnimationCounter2 = m_siSetItemImgAnimationCounter2%pSpr2->GetImageNum();
								m_siSetItemImgAnimationCounter2++;
							}
							GP.PutSprScreen(pSpr2, DrawStartX + 100, DrawStartY + 80, m_siSetItemImgAnimationCounter2);
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}

	// [영훈] 결혼 이펙트
	BOOL bDrawWeddingEffect = FALSE;
	if ( 1 == GetCharID() )
	{
		if ( (IsMarried()) && (0 < pclMarriage->GetLoveLevel())  && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP==pclMarriage->GetConnectMode()) )	
		{
			bDrawWeddingEffect = TRUE;
		}
	} 
	else
	{
		if ( (pclMarriage) && (0 < pclMarriage->GetLoveLevel()) )
		{
			bDrawWeddingEffect = TRUE;
		}
	}

	if ( TRUE == bDrawWeddingEffect )
	{
		SI32	siLoveLevelEffect	= pclClient->m_pclMarriageMgr->GetEffectTypeFromLoveLevel( pclMarriage->GetLoveLevel() );

		TSpr*	psprEffect			= NULL;

		switch ( siLoveLevelEffect )
		{
		case MARRIAGE_EFFECT_TYPE_SMALL:	
			{	
				psprEffect = pclClient->GetGlobalSpr( GIMG_WEDDING_EFT_S );	

				if ( (NULL != psprEffect) && (0 < psprEffect->GetImageNum()) )
				{
					m_siWeddingImgAnimationCounter = m_siWeddingImgAnimationCounter % psprEffect->GetImageNum();
					GP.PutSprScreen(psprEffect, (DrawStartX + 100), (DrawStartY + 110), m_siWeddingImgAnimationCounter++ );
				}
			}	
			break;

		case MARRIAGE_EFFECT_TYPE_MEDIUM:	
			{	
				psprEffect = pclClient->GetGlobalSpr( GIMG_WEDDING_EFT_M );	

				if ( (NULL != psprEffect) && (0 < psprEffect->GetImageNum()) )
				{
					m_siWeddingImgAnimationCounter = m_siWeddingImgAnimationCounter % psprEffect->GetImageNum();
					GP.PutSprScreen(psprEffect, (DrawStartX + 85), (DrawStartY + 85), m_siWeddingImgAnimationCounter++ );
				}
			}	
			break;

		case MARRIAGE_EFFECT_TYPE_LARGE:	
			{	
				psprEffect = pclClient->GetGlobalSpr( GIMG_WEDDING_EFT_L );	

				if ( (NULL != psprEffect) && (0 < psprEffect->GetImageNum()) )
				{
					m_siWeddingImgAnimationCounter = m_siWeddingImgAnimationCounter % psprEffect->GetImageNum();
					GP.PutSprScreen(psprEffect, (DrawStartX + 50), (DrawStartY + 50), m_siWeddingImgAnimationCounter++ );
				}
			}	
			break;
		}

	}

	if ( battacklayerskillmode )
	{
		cltMapClient* pclmap = (cltMapClient*)pclMap;
		TSpr* pspr	= pclClient->GetGlobalSpr(attackgimg);
		if ( pspr )
		{
			cltEffectMapInfo pcleffect;
			pcleffect.gimg = attackgimg ;
			pcleffect.font = effectfont ;
			pcleffect.x = effectx ;
			pcleffect.y = effecty ;
			pcleffect.reverse = effectreverse ;
			pclmap->PushEffect(&pcleffect);
		}
	}

	// MC 모자인지 여부. 
	SI32 itemunique = pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique;
	if(itemunique == 0)
	{
		itemunique = clPB.clShapeEquip.clHelmet.uiUnique;
	}

	return true;
}

SI32 cltCharClient::CheckJewelEffect( UI32 rare , SI32 enchanttype , SI32 enchantpower, bool Flag ) 
{
	SI32 jewelEffect = 0;

	if( rare == RARELEVEL_1 )
	{
		jewelEffect |= JEWELEFFECT_RARE1;
	}
	else if ( rare == RARELEVEL_2)
	{
		jewelEffect |= JEWELEFFECT_RARE2;
	}
	else if ( rare == NEWYEAR_EFFECT)
	{
		//[추가 : 황진성 2008. 1. 9. 신년 과금 복장 빤딱이 보이게. ]
		SYSTEMTIME sTime;
		GetLocalTime( &sTime );

		if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
		{
			jewelEffect |= JEWELEFFECT_RARE2;

			//[추가 : 황진성 2007. 12. 21 => 과금 아이템 번뜩이는거 일본만 호품으로 번뜩이게 수정.]
			if(pclClient->siServiceArea == ConstServiceArea_Japan)
			{
				if(true == Flag)
				{
					jewelEffect = 0;
					jewelEffect |= JEWELEFFECT_RARE1;
				}
			}
		}

		//if(true == NewYearEventDay())
		//{
		//	jewelEffect |= JEWELEFFECT_RARE2;

		//	//[추가 : 황진성 2007. 12. 21 => 과금 아이템 번뜩이는거 일본만 호품으로 번뜩이게 수정.]
		//	if(pclClient->siServiceArea == ConstServiceArea_Japan)
		//	{
		//		if(true == Flag)
		//		{
		//			jewelEffect = 0;
		//			jewelEffect |= JEWELEFFECT_RARE1;
		//		}
		//	}
		//}
	}
	else if(rare == MARRIED_FULLDRESS_EFFECT)
	{
		jewelEffect |= MARRIED_FULLDRESS;
	}

	if( enchanttype == ENCHANT_TYPE_FIRE_A)
	{
		if		(enchantpower >= JEWELEFFECT_GRADE3)	{	jewelEffect |= JEWELEFFECT_FIRE3;	}
		else if (enchantpower >= JEWELEFFECT_GRADE2)	{	jewelEffect |= JEWELEFFECT_FIRE2;	}
		else if	(enchantpower >= JEWELEFFECT_GRADE1)	{	jewelEffect |= JEWELEFFECT_FIRE1;	}
	}
	else if( enchanttype == ENCHANT_TYPE_WATER_A)
	{
		if		(enchantpower >= JEWELEFFECT_GRADE3)	{	jewelEffect |= JEWELEFFECT_WATER3;	}
		else if (enchantpower >= JEWELEFFECT_GRADE2)	{	jewelEffect |= JEWELEFFECT_WATER2;	}
		else if	(enchantpower >= JEWELEFFECT_GRADE1)	{	jewelEffect |= JEWELEFFECT_WATER1;	}
	}
	else if( enchanttype == ENCHANT_TYPE_GROUND_A)
	{
		if		(enchantpower >= JEWELEFFECT_GRADE3)	{	jewelEffect |= JEWELEFFECT_GROUND3;	}
		else if (enchantpower >= JEWELEFFECT_GRADE2)	{	jewelEffect |= JEWELEFFECT_GROUND2;	}
		else if	(enchantpower >= JEWELEFFECT_GRADE1)	{	jewelEffect |= JEWELEFFECT_GROUND1;	}
	}
	else if( enchanttype == ENCHANT_TYPE_WIND_A)
	{
		if		(enchantpower >= JEWELEFFECT_GRADE3)	{	jewelEffect |= JEWELEFFECT_WIND3;	}
		else if (enchantpower >= JEWELEFFECT_GRADE2)	{	jewelEffect |= JEWELEFFECT_WIND2;	}
		else if	(enchantpower >= JEWELEFFECT_GRADE1)	{	jewelEffect |= JEWELEFFECT_WIND1;	}
	}

	return jewelEffect ;
}

bool cltCharClient::DrawNormal()
{
	SI32 sidrawysize;


	// 찍을 이미지를 판단한다. 
	DecideFont();

	if(pclCI->clBI.GetKind() == KIND_JOKER && (pImgSpr == NULL || pImgSpr->header.iXSize <= 0 || pImgSpr->header.iYSize <= 0))
	{
		// 운영자는 상관없다.
		if(pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)
		{
			// 하지만 또 모르니 적어도 3초 기다렸다가 끝나면 종료해 줍시다.
			if( GAMEMODE_CLIENT ==  pclClient->GameMode )
			{
				MsgBox("Error","Can't Read Important file!!");
				dwQuitTimer = pclClient->CurrentClock + 1000;
			}
			return false;
		}
	}

	sidrawysize=ImageYsize;
	SI16 drawtransmode = TransMode;
	// by LEEKH 2007.12.24 - TRANSFORM
	if ( GetKindInfo(true)->clCharAniManager.pclCharAni[Animation]->siTransMode != 0 )
		drawtransmode = GetKindInfo(true)->clCharAniManager.pclCharAni[Animation]->siTransMode ;

	SI08 alphavalue = AlphaValue ;
	if ( drawtransmode == TRANSPARENT_MODE_ALPHA )
	{
		if ( alphavalue < 0 || alphavalue > MAX_ALPHA_LEVEL - 1 )
			alphavalue = MAX_ALPHA_LEVEL - 1;
	}

	// 대상물 선택이 된 것이면 표시해준다.
	unsigned char *darktable;

	if(bNewSelectedSwitch == true)
	{
		if(NewSelectedDelay % 2==0)
		{

			darktable = (UI08*)1;

			// 일반 모드가 아닌 특수모드로 찍어야한다면 밝아질때 일반적인 방법으로 찍어준다.
			if(drawtransmode != TRANSPARENT_MODE_NORMAL) drawtransmode =  TRANSPARENT_MODE_NORMAL;

		}
		else
		{
			darktable = 0;
		}
	}
	else if( bMouseOverSwitch == true )
	{        
		darktable = (UI08*)1;

		// 일반 모드가 아닌 특수모드로 찍어야한다면 밝아질때 일반적인 방법으로 찍어준다.
		if(drawtransmode != TRANSPARENT_MODE_NORMAL) drawtransmode =  TRANSPARENT_MODE_NORMAL;
	}
	else
	{
		darktable = NULL;
	}

	// 도마돈은 무조건 알파로 그린다.
	if(pclCI->clBI.GetKind() == KIND_JOKER)
	{
		drawtransmode =  TRANSPARENT_MODE_ALPHA;
		alphavalue    = 14;
	}

	if(pImgSpr)
	{
		switch( drawtransmode )
		{
		case TRANSPARENT_MODE_OLDTRANS:
			GP.PutSprLight1(pImgSpr, DrawStartX, DrawStartY, Font, bReverseSwitch);				break;
		case TRANSPARENT_MODE_HALFTRANS:
			GP.PutSprLight(pImgSpr, DrawStartX, DrawStartY, Font, bReverseSwitch);				break;
		case TRANSPARENT_MODE_SCREEN:
			GP.PutSprScreen(pImgSpr, DrawStartX, DrawStartY, Font, bReverseSwitch);				break;
		case TRANSPARENT_MODE_MULTIPLY:
			GP.PutSprMultiply(pImgSpr, DrawStartX, DrawStartY, Font, bReverseSwitch);			break;
		case TRANSPARENT_MODE_ADD:
			GP.PutSprAdd(pImgSpr, DrawStartX, DrawStartY, Font, bReverseSwitch);				break;
		case TRANSPARENT_MODE_ALPHA:
			GP.PutSprAlpha(pImgSpr, DrawStartX, DrawStartY,alphavalue, Font, bReverseSwitch);	break;
		default:
			{
				//&Image[ clHeader.clImgInfo[font].siStartPos ];
				if( darktable ) {
					GP.PutSprEF(pImgSpr,  DrawStartX, DrawStartY,  0, Font, bReverseSwitch );	break;
				} else {
					GP.PutSpr(pImgSpr, DrawStartX, DrawStartY, Font,  bReverseSwitch );			break;
				}
			}

		}
	}	

	// [기형] 배틀로얄 중- 변신 상태 인지
	CBattleRoyalClient * pclBattleRoyalClient = ((cltClient*)pclClient)->m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		if(TRUE == pclBattleRoyalClient->IsTransform(this))
		{
			return true;
		}
	}

	// 특수 효과가 있는 경우는 특수 효과를 출력한다.
	if(siEffectFont >=0)
	{
		if(pImgSpr)	GP.PutSprLight1(pImgSpr, DrawStartX, DrawStartY, siEffectFont, bReverseSwitch);
	}
	if ( siShadowAni > 0 )
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 ref = GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(siShadowAni);
		if ( ref >= 0 )
		{
			TSpr* pSpr = NULL ;
			SI32 anifont = 0 ;
			bool reverse = false ;
			SI32 effect = 0 ;
			// by LEEKH 2007.12.24 - TRANSFORM
			alphavalue = GetKindInfo(true)->clCharAniManager.pclCharAni[siShadowAni]->siAlphaValue ;
			if ( alphavalue < 0 || alphavalue > MAX_ALPHA_LEVEL - 1 )
				alphavalue = MAX_ALPHA_LEVEL - 1;

			// by LEEKH 2007.12.24 - TRANSFORM
			GetKindInfo(true)->clCharAniManager.DecideDrawImg(siShadowAni, siDirection, clAniInfo,
				TABS(pclClient->CurrentClock), &pSpr, &anifont, &reverse, &effect );

			// by LEEKH 2007.12.24 - TRANSFORM
			if ( siShadowFont >= 0 && GetKindInfo(true)->clCharAniManager.GetDirSwitch(siShadowAni) )
				anifont = siShadowFont ;

			SI32 mode = TransMode;

			// by LEEKH 2007.12.24 - TRANSFORM
			if ( GetKindInfo(true)->clCharAniManager.pclCharAni[siShadowAni]->siTransMode != 0 )
				mode = GetKindInfo(true)->clCharAniManager.pclCharAni[siShadowAni]->siTransMode ;

			SI32 xindent = 0 ;
			SI32 yindent = 0 ;
			// by LEEKH 2007.12.24 - TRANSFORM
			GetKindInfo(true)->clCharAniManager.GetShadowIndentXY(siShadowAni,&xindent,&yindent);

			if(pSpr)
			{
				SI32 x = pSpr->GetXSize();
				SI32 y = pSpr->GetYSize();

				SI32 drawx = DrawStartX + ImageXsize/2 - x/2 - xindent ;
				SI32 drawy = DrawStartY + ImageYsize/2 - y/2 - yindent ;

				switch( mode )
				{
				case TRANSPARENT_MODE_OLDTRANS:
					GP.PutSprLight1(pSpr, drawx, drawy, anifont, reverse);		break;
				case TRANSPARENT_MODE_HALFTRANS:
					GP.PutSprLight(pSpr, drawx, drawy, anifont, reverse);		break;
				case TRANSPARENT_MODE_SCREEN:
					GP.PutSprScreen(pSpr, drawx, drawy, anifont, reverse);		break;
				case TRANSPARENT_MODE_MULTIPLY:
					GP.PutSprMultiply(pSpr,drawx, drawy, anifont, reverse);	break;
				case TRANSPARENT_MODE_ADD:
					GP.PutSprAdd(pSpr, drawx, drawy, anifont, reverse);			break;
				case TRANSPARENT_MODE_ALPHA:
					GP.PutSprAlpha(pSpr, drawx, drawy, alphavalue,  anifont, reverse);			break;
				default:
					{
						GP.PutSpr(pImgSpr, drawx, drawy, anifont,  reverse );	break;

					}
					break;

				}

				if(effect >=0)
				{
					GP.PutSprLight1(pSpr, drawx, drawy, effect, reverse);
				}
			}
		}
	}
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		cltKindInfo* ppclKI = pclKI;

		if(selectRealAnyType( ) == false)
			ppclKI = GetKindInfo(true);

		if( ppclKI )
		{
			if( m_siSetItemType )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_1);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty += 25;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_1);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty -= 30;  

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}
			}
			if( m_siSetItemType2 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_2);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty += 25;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_2);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty -= 30;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}
			}

			if( m_siSetItemType3 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_3);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty += 25;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_3);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty -= 30;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}
			}

			if( m_siSetItemType4 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_4);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty += 25;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_4);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty -= 30;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}
			}

			if( m_siSetItemType5 )
			{
				SI32 Index = -1;
				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemE(this, USE_SETITEM_5);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty += 25;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}

				Index = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemEffet(this, USE_SETITEM_5);
				if( 0 < Index )
				{
					TSpr* pspr = pclClient->GetGlobalSpr( Index );
					if( pspr )
					{
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2;
						SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
						SI32 starty	= DrawStartY + ImageYsize -ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize() /2+18;

						// [종호] 스피릿변환중에는 세트아이템 이펙트의 위치가 변경되어야 한다.
						if(pclClient->pclCM->CR[GetID()]->clTransFormInfo.bTransFormMode == true || pclClient->pclCM->CR[GetID()]->siBufTransformKind > 0)
							starty -= 30;

						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+pclClient->GetFrame()) / 3 % 30);
						GP.PutSprScreen(pspr, startx , starty+15, (GetID()+max(0, TABS(pclClient->CurrentClock / 50))) / 3 % 30);
					}	
				}
			}
		}
	}
	return true;
}



//---------------------------------------------------------------
// 조합형 캐릭터. 
//---------------------------------------------------------------
// 파라메타로 주어진 아이템에 따라 Person의 복장을 바꿔주게 한다.
bool cltCharClient::ChangeArmCode(SI32 itempos, SI32 unique)
{
	if ( itempos < 0 || itempos >= MAX_SHAPE_ITEM )
	{
		return false;
	}

	TCHAR* armcode = NULL; 
	bool updateswitch = false;
	cltItemManagerCommon* pclItemManager = pclClient->pclItemManager;

	//모자를 바꾸는 경우는 바로 외관을 수정한다. 
	bool bhelmetswitch = false;
	if(itempos == PERSONITEM_HAT || itempos == PERSONITEM_HELMET)
	{

		// 장착인 경우, 
		if(unique && ActiveHat == PERSONITEM_HAT && itempos == PERSONITEM_HAT)
		{
			bhelmetswitch  = true;
		}
		// 탈착인 경우, 
		else if(unique && ActiveHat == PERSONITEM_HELMET && itempos == PERSONITEM_HELMET)
		{
			bhelmetswitch  = true;
		}

		else
		{
			bhelmetswitch  = true;

			SI32 HatFlag1 = PERSONITEM_HAT;
			SI32 HatFlag2 = PERSONITEM_HELMET;

			if( ActiveHat == PERSONITEM_HELMET)
			{
				HatFlag1 =  PERSONITEM_HELMET;
				HatFlag2 =  PERSONITEM_HAT;

			}
			// 투구가 있으면 투구의 이미지로 변경한다. 
			if(pclCI->clCharItem.clItem[HatFlag1].siUnique)
			{
				unique = pclCI->clCharItem.clItem[HatFlag1].siUnique;
			}
			else
			{
				unique = pclCI->clCharItem.clItem[HatFlag2].siUnique;
			}
		}

	}

	if(bhelmetswitch)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(unique);
		if(ref > 0 )
		{
			armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
		}
		else armcode = NULL;

		if(armcode)
		{
			updateswitch	= true;
			StringCchCopy(szHelmetCode,  32, armcode );
		}
		else
		{
			updateswitch	= true;
			StringCchCopy(szHelmetCode,  32, TEXT("KOR01"));
		}
	}

	//의복을 바꾸는 경우는 바로 외관을 수정한다. 

	bool barmourswitch = false;
	if(itempos == PERSONITEM_DRESS || itempos == PERSONITEM_ARMOUR )
	{
		// 장착인 경우, 
		if(unique && ActiveClothes==PERSONITEM_DRESS && itempos == PERSONITEM_DRESS)
		{
			barmourswitch  = true;
		}
		else if(unique && ActiveClothes==PERSONITEM_ARMOUR && itempos == PERSONITEM_ARMOUR)
		{
			barmourswitch  = true;
		}
		// 탈착인 경우, 
		else
		{
			barmourswitch  = true;

			SI32 DressFlag1 = PERSONITEM_DRESS;
			SI32 DressFlag2 = PERSONITEM_ARMOUR;

			if( ActiveClothes == PERSONITEM_ARMOUR)
			{
				DressFlag1 =  PERSONITEM_ARMOUR;
				DressFlag2 =  PERSONITEM_DRESS;

			}
			// 갑옷이 있으면 갑옷의 이미지로 변경한다. 
			if(pclCI->clCharItem.clItem[DressFlag1].siUnique)
			{
				unique = pclCI->clCharItem.clItem[DressFlag1].siUnique;
			}
			else
			{
				unique = pclCI->clCharItem.clItem[DressFlag2].siUnique;
			}
		}
	}
	if(barmourswitch)

	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(unique);
		if(ref > 0 )
		{
			armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
		}
		else armcode = NULL;

		if(armcode)
		{
			updateswitch	= true;
			StringCchCopy(szArmourCode,  32, armcode);
		}
		else
		{
			updateswitch	= true;
			StringCchCopy(szArmourCode,  32, TEXT("KOR01"));
		}

	}
	if(itempos == PERSONITEM_WEAPON1)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(unique);

		if(ref > 0 )
		{
			armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
		}
		else armcode = NULL;


		if(ActiveWeapon == itempos)
		{

			if(armcode)
			{
				updateswitch	= true;
				StringCchCopy(szWeaponCode,  32, armcode);
			}
			else
			{
				updateswitch	= true;
				StringCchCopy(szWeaponCode,  32, TEXT("KOR01"));
			}
		}
	}

	if(itempos == PERSONITEM_WEAPON2)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(unique);
		if(ref > 0 )
		{
			armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
		}
		else armcode = NULL;

		if(ActiveWeapon == itempos)
		{

			if(armcode)
			{
				updateswitch	= true;
				StringCchCopy(szWeaponCode,  32, armcode);
			}
			else
			{
				updateswitch	= true;
				StringCchCopy(szWeaponCode,  32, TEXT("KOR01"));
			}
		}
	}

	if ( itempos == PERSONITEM_MANTLE)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(unique);
		if(ref > 0 )
		{
			armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
		}
		else armcode = NULL;

		if ( armcode )
		{
			updateswitch	= true;
			StringCchCopy( szMantleCode,  32, armcode );
		}
		else
		{
			updateswitch	= true;
			StringCchCopy( szMantleCode,  32, TEXT("") );
		}
	}

	SI32 EffectKind = 0;
	SI32 AttackScrollNum = 0;
	SI32 attacklayerskill = 0;

	if ( GetID() == 1 )
	{
		if ( pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique > 0  )
		{	
			SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique );

			if ( Ref )
			{
				SI32 AttackType = pclClient->pclItemManager->pclItemInfo[ Ref ]->GetAttackType();

				switch( AttackType )
				{
				case ATTACKTYPE_SWORD:
					attacklayerskill = SKILL_USESWORD1;
					break;
				case ATTACKTYPE_SPEAR:
					attacklayerskill = SKILL_USESPEAR1;
					break;
				case ATTACKTYPE_AXE:
					attacklayerskill = SKILL_USEAXE1;
					break;
				}

				AttackScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum( pclClient->pclItemManager, attacklayerskill ) ;

				if ( AttackScrollNum > 0 )
				{
					if ( AttackScrollNum > 3 )
					{
						EffectKind = 2;
					}
					else
					{
						EffectKind = 1;
					}
				}
				updateswitch = true;
			}
		}
	}
	else
	{
		if ( clPB.clShapeEquip.clWeapon.uiUnique > 0 )
		{
			SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( clPB.clShapeEquip.clWeapon.uiUnique );

			if ( Ref )
			{
				SI32 AttackType = pclClient->pclItemManager->pclItemInfo[ Ref ]->GetAttackType();

				switch( AttackType )
				{
				case ATTACKTYPE_SWORD:
					AttackScrollNum = clPB.clShapeEquip.siSwordScrollNum;
					break;
				case ATTACKTYPE_SPEAR:
					AttackScrollNum = clPB.clShapeEquip.siSpearScrollNum;
					break;
				case ATTACKTYPE_AXE:
					AttackScrollNum = clPB.clShapeEquip.siAxeScrollNum;
					break;
				}
			}

			if ( AttackScrollNum > 0 )
			{
				if ( AttackScrollNum > 3 )
				{
					EffectKind = 2;
				}
				else
				{
					EffectKind = 1;
				}

				updateswitch = true;
			}
		}
	}
	if(updateswitch == true)
	{
		//cyj 부적에 의한 공격효과 변화 임시로 막음
		EffectKind = 0;

		//KHY - 1001 - 기승동물 5마리 보유 수정.
		// 말을 타고 있다면, 
		//if ( pclCI->clHorseInfo.siCurrentHorseIndex >= 0)
		if ( pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE)
		{
			pclCharDraw->LoadItemCodeData(GetKind(), szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, szHorseCode, EffectKind, NULL );
		}
		else
		{
			pclCharDraw->LoadItemCodeData(GetKind(), szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, TEXT(""), EffectKind, NULL );
		}

		// 공격 방식을 업데이트 한다. 
		// JAYCEE ATTACK
		if(itempos == PERSONITEM_WEAPON1 || itempos == PERSONITEM_WEAPON2)
		{
#ifdef _SAFE_MEMORY
			clPB.CalculateAttackType(this, ActiveWeapon, &pclCI->clCharItem.clItem[0]);
#else
			clPB.CalculateAttackType(this, ActiveWeapon, pclCI->clCharItem.clItem);
#endif
		}

		if(itempos == PERSONITEM_WEAPON1 || itempos == PERSONITEM_WEAPON2)
		{
			// 캐릭터의 애니메이션을 변경한다. 
			// 무장등이 변경되어 애니메이션이 바뀌어야 한다. 
			SetAnimation(Animation);
		}
	}

	return true;
}

// 말 코드를 변경하여 조치한다.
void cltCharClient::ChangeHorseCode( SI32 code, cltDate* cldate, SI32 ticket )
{
	if( pclCharDraw == NULL )
		return;

	SI32 EffectKind = 0;
	SI32 AttackScrollNum = 0;
	SI32 attacklayerskill = 0;

	if ( GetID() == 1 )
	{	
		if ( ActiveWeapon > 0 && pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique > 0 )
		{
			SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique );

			if ( Ref )
			{
				SI32 AttackType = pclClient->pclItemManager->pclItemInfo[ Ref ]->GetAttackType();

				switch( AttackType )
				{
				case ATTACKTYPE_SWORD:
					attacklayerskill = SKILL_USESWORD1;
					break;
				case ATTACKTYPE_SPEAR:
					attacklayerskill = SKILL_USESPEAR1;
					break;
				case ATTACKTYPE_AXE:
					attacklayerskill = SKILL_USEAXE1;
					break;
				}

				AttackScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum( pclClient->pclItemManager, attacklayerskill ) ;

				if ( AttackScrollNum > 0 )
				{
					if ( AttackScrollNum > 3 )
					{
						EffectKind = 2;
					}
					else
					{
						EffectKind = 1;
					}
				}
			}
		}
	}
	else
	{
		if ( clPB.clShapeEquip.clWeapon.uiUnique > 0 )
		{
			SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( clPB.clShapeEquip.clWeapon.uiUnique );

			if ( Ref )
			{
				SI32 AttackType = pclClient->pclItemManager->pclItemInfo[ Ref ]->GetAttackType();

				switch( AttackType )
				{
				case ATTACKTYPE_SWORD:
					AttackScrollNum = clPB.clShapeEquip.siSwordScrollNum;
					break;
				case ATTACKTYPE_SPEAR:
					AttackScrollNum = clPB.clShapeEquip.siSpearScrollNum;
					break;
				case ATTACKTYPE_AXE:
					AttackScrollNum = clPB.clShapeEquip.siAxeScrollNum;
					break;
				}
			}

			if ( AttackScrollNum > 0 )
			{
				if ( AttackScrollNum > 3 )
				{
					EffectKind = 2;
				}
				else
				{
					EffectKind = 1;
				}
			}
		}
	}

	//cyj 부적에 의한 공격효과 변화 임시로 막음
	EffectKind = 0;

	if(code == 0)
	{

		pclCI->clHorseInfo.siCurrentHorseRIDEHORSE	= false;
		StringCchCopy(szHorseCode, 32, TEXT(""));
		pclCharDraw->LoadItemCodeData( GetKind(), szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, NULL, EffectKind, NULL );
	}
	else
	{
		pclCI->clHorseInfo.siCurrentHorseRIDEHORSE	= true;

		StringCchCopy(szHorseCode, 32, pclClient->pclHorseManager->pclHorseTypeInfo[code]->szCode);

		pclCharDraw->LoadItemCodeData(GetKind(), szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, szHorseCode, EffectKind, NULL );

		if ( cldate != NULL && ticket >= 0 )
		{
			cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
			if(pclHorse != NULL)
			{
				pclHorse->clAddStatusDate.Set(cldate) ;
				pclHorse->siStatusTicketNum = ticket ;
			}
		}
	}

	// 말에 의해서 애니메이션 동작이 변경되므로 이를 수정해야 한다. 
	SetAnimation(Animation);

}

//-----------------------------------
// 물품 생산. 
//-----------------------------------
bool cltCharClient::SetMakeDelayDraw(DWORD delayclock)
{
	if(delayclock)
	{
		bStartMakeItemSwitch	= true;
		dwStartMakeItemClock	= TABS(pclClient->CurrentClock);
		dwDelayMakeItemClock	= delayclock;
	}
	else
	{
		bStartMakeItemSwitch	= false;
		dwStartMakeItemClock	= 0;
		dwDelayMakeItemClock	= 0;
	}

	return true;
}

// 물품 생산 지연을 표시한다. 
void cltCharClient::DrawMakeDelay(LPDIRECTDRAWSURFACE7 lpSurface, SI32 rx, SI32 ry)
{
	if(bStartMakeItemSwitch == false)return ;

	// 표시해야 할 숫자. 
	SI32 maxclock	= max(1, (dwDelayMakeItemClock / 1000));
	SI32 leftclock	= TABS(pclClient->CurrentClock - dwStartMakeItemClock) / 1000;

	SI32 x = 450 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 15 : 0 )  ; // 풀스크린이면 좌표 30을 더해줌
	SI32 y = 50;

	TSpr* psprpart	= pclClient->GetGlobalSpr(GIMG_MAKEITEMDRAWPART1);
	TSpr* psprframe	= pclClient->GetGlobalSpr(GIMG_MAKEITEMDRAWPART2);
	TSpr* pspr		= pclClient->GetGlobalSpr(GIMG_MAKEITEMDRAWPART3);


	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		y += 30 ;
	}

	if ( GP.LockSurface( lpSurface ) == TRUE )
	{

		// 틀 출력. 
		if(psprframe)
		{
			GP.PutSpr(psprframe, x + rx, y + ry, 0);

			// 비율 출력. 
			if(psprpart)
			{
				SI32 maxxnum = (psprframe->GetXSize() - 6) / psprpart->GetXSize();

				SI32 drawnum =  min(maxxnum, ( leftclock * maxxnum * psprpart->GetXSize() / maxclock ) / psprpart->GetXSize());

				for(SI32 i = 0;i < drawnum;i++)
				{
					GP.PutSpr(psprpart, x + 3 + i*psprpart->GetXSize() + rx, y + 4 + ry, 0);
				}
			}

			x += psprframe->GetXSize();
			y += 3;
		}


		if(pspr)
		{
			// 숫자. 
			x = GP.DrawNumber(pspr, x + rx, y + ry, 0, leftclock);

			GP.PutSpr(pspr, x, y + ry, 10);

			GP.DrawNumber(pspr, x+ pspr->GetXSize(), y + ry, 0, maxclock);
		}


		GP.UnlockSurface( lpSurface );
	}


}


//-----------------------------------
// Health Bar를 그려준다.
//-----------------------------------
void cltCharClient::DrawHealthBar()
{
	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 realxsize = GetKindInfo(true)->clKIDrawInfo.siXsizePercent*ImageXsize/100;
	SI32 realysize = GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 drawx	= DrawStartX + (ImageXsize-realxsize)/2;
	SI32 drawy	= DrawStartY + ImageYsize - GetKindInfo(true)->clKIDrawInfo.siBottomIndent - realysize;

	GP.Box(drawx, drawy - 1, drawx + realxsize + 1, drawy + 3, 255);

	// 에너지를 표시한다. 
	if(clPB.GetMaxLife() > 0)
	{
		SI32 fillsize = min(realxsize, GetLife() * realxsize / clPB.GetMaxLife() );

		GP.FillBox(drawx + 1 ,			drawy ,	drawx + 1 + fillsize -1,	drawy + 2, 20,FALSE);
		if(drawx + 1 + fillsize < drawx + realxsize)
		{
			GP.FillBox(drawx + 1 + fillsize ,drawy , drawx + realxsize,			drawy + 2, 243,FALSE);
		}
#ifdef _DEBUG
		TSpr* pspr2	= pclClient->GetGlobalSpr(GIMG_WARKILLNUM);
		if ( pspr2 )
		{
			GP.DrawNumber( pspr2, drawx, drawy, 0, GetLife() );
		}
#endif

	}
}

//-----------------------------------
// King Effect를 그려준다.
//-----------------------------------
void cltCharClient::DrawKingEffect()
{

	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_KINGEFFECT);
	if(pspr)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		//SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2;
		//SI32 realysize	= pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		//SI32 starty	= DrawStartY + ImageYsize - GetKindInfo(true)->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize()/2+35;  

		//GP.PutSprScreen(pspr, startx , starty, max(0, TABS(pclClient->CurrentClock / 50)) / 3 % 15);
		//by LEEKH 2007.12.24 - TRANSFORM
		cltKindInfo* ppclKI = pclKI;

		if(selectRealAnyType( ) == false)
			ppclKI = GetKindInfo(true);

		SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2 ;
		SI32 realysize	= ppclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 starty	= DrawStartY + ImageYsize - ppclKI->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize()/2+38;   

		GP.PutSprScreen(pspr, startx , starty, max(0, pclClient->CurrentClock / 50) / 3 % 15);    

	}
}


//-----------------------------------
// 성황신 Effect를 그려준다.
//-----------------------------------
void cltCharClient::DrawGodEffect()
{

	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_GODEFFECT);
	if(pspr)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2 ;
		SI32 realysize	= GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 starty	= DrawStartY + ImageYsize - GetKindInfo(true)->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize()/2;

		GP.PutSprScreen(pspr, startx , starty, max(0, pclClient->CurrentClock / 50) / 3 % 50);

	}
}

//-----------------------------------
//KHY - 1220 - 변신시스템.
//해당 변신 캐릭터 싱크가 100%이면.
//-----------------------------------
void cltCharClient::DrawTransformSyc100Effect()
{
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_TRANSFORM_SYNCHRO100);
	if(pspr)
	{
		SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2 ;
		SI32 realysize	= GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 starty	= DrawStartY + ImageYsize - GetKindInfo(true)->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize()/2 - 30;

		GP.PutSprScreen(pspr, startx , starty, max(0, pclClient->CurrentClock / 50) / 3 % 15);    
	}
}

//-----------------------------------
// 소환수 Effect를 그려준다.추가 :날개를 그러주는 거임. 소환수 머리위의 날개
//-----------------------------------
void cltCharClient::DrawSummonEffect()
{
	/*
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_SUMMONMARK);
	if(pspr)
	{
	SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2 ;
	SI32 realysize	= pclKI->clKIDrawInfo.siYsizePercent*ImageYsize/100;
	SI32 starty	= DrawStartY + ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - realysize - pspr->GetYSize()/2 - 10;


	GP.PutSprScreen(pspr, startx , starty, max(0, pclClient->CurrentClock / 50) / 2 % 27);
	}
	*/
}

//-----------------------------------
// 가차 소환수 펫 Effect를 그려준다.
//-----------------------------------
void cltCharClient::DrawGachaSummonEffect()
{
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_GACHASUMMON_EFFECT);
	if(pspr)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 startx = DrawStartX + ImageXsize/2 - pspr->GetXSize()/2 ;
		SI32 realysize	= GetKindInfo(true)->clKIDrawInfo.siYsizePercent*ImageYsize/100;
		SI32 starty	= DrawStartY + ImageYsize - GetKindInfo(true)->clKIDrawInfo.siBottomIndent - realysize/2 - pspr->GetYSize()/2;

		GP.PutSprScreen(pspr, startx , starty, max(0, pclClient->CurrentClock / 50) / 2 % 20);

	}
}

void cltCharClient::DrawGate(SI32 id)
{
	if(pclCM->IsAlive(id))
	{
		SI32 siGateLife = pclCM->CR[id]->pclCI->clIP.siVit*4;

		SI32 startx;
		SI32 starty;

		if( pclCM->CR[id]->GetLife() <  siGateLife * 0.9)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE4);
			if(pspr)
			{
				startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 - 33;
				starty	= DrawStartY - 40;//+ 30;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = (pclClient->CurrentClock / 100) % pspr->GetImageNum();	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
		if( pclCM->CR[id]->GetLife() <  siGateLife * 0.6)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE4);
			if(pspr)
			{
				startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 + 60;
				starty = DrawStartY + 60;//+ 30;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = (pclClient->CurrentClock / 100) % pspr->GetImageNum();	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
		if( pclCM->CR[id]->GetLife() <  siGateLife * 0.3)
		{
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE4);
			if(pspr)
			{
				startx = DrawStartX + ImageXsize/2 - pspr->GetXSize() /2 - 120;
				starty = DrawStartY + 10;//+ 30;//ImageYsize - pclKI->clKIDrawInfo.siBottomIndent - pspr->GetYSize() /2;

				SI32 font = (pclClient->CurrentClock / 100) % pspr->GetImageNum();	

				GP.PutSprLight1(pspr, startx,		starty, font);
			}
		}
	}
}

bool cltCharClient::NewYearEventDay(void)
{
	//[추가 : 황진성 2007. 12. 12 신년이벤트 기간.]
	SYSTEMTIME	StartTime;	ZeroMemory(&StartTime, sizeof(StartTime));
	SYSTEMTIME	EndTime;	ZeroMemory(&EndTime,   sizeof(EndTime));
	switch(pclClient->siServiceArea)
	{
	case ConstServiceArea_English:	
		{
			StartTime.wYear	 = 2008;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   11;		EndTime.wMonth	=	12;
			StartTime.wDay	 =   10;		EndTime.wDay	=   31;
			StartTime.wHour	 =    3;		EndTime.wHour	=    2;
		}
		break;
	case ConstServiceArea_Korea:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   27;		EndTime.wDay	=   10;
			StartTime.wHour	 =    0;		EndTime.wHour	=   23;
		}
		break;
	case ConstServiceArea_Japan:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   26;		EndTime.wDay	=   10;
			StartTime.wHour	 =    0;		EndTime.wHour	=   11;
		}
		break;
	case ConstServiceArea_Taiwan:	
	case ConstServiceArea_USA:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   26;		EndTime.wDay	=    9;
			StartTime.wHour	 =    0;		EndTime.wHour	=   23;
		}
		break;
	case ConstServiceArea_NHNChina	:	// NHN중국도 신년 이벤트는 할테니..기간 추가.
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   27;		EndTime.wDay	=    8;
			StartTime.wHour	 =    3;		EndTime.wHour	=    2;
		}
		break;

	case ConstServiceArea_EUROPE :
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   27;		EndTime.wDay	=    8;
			StartTime.wHour	 =    3;		EndTime.wHour	=    2;
		}
		break;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if(true == TimeInBounds(&pclclient->sTime, &StartTime, &EndTime))
	{
		return true;
	}

	return false;	
}

SI16 cltCharClient::GetQuestDisplayMark( cltSystemNPC* pclSystemNPCInfo )
{
	if ( NULL == pclSystemNPCInfo )
	{
		return QUEST_DISPLAY_MARK_NONE;
	}

	cltCharClient* pclMyChar = (cltCharClient*)pclCM->CR[1];
	if ( NULL == pclMyChar )
	{
		return QUEST_DISPLAY_MARK_NONE;
	}

	SI16			siQuestType		= pclSystemNPCInfo->m_siQuestType;
	cltQuestInfo*	pclQuestInfo	= &pclClient->pclQuestManager->clQuestInfo[siQuestType][0];
	if ( NULL == pclQuestInfo )
	{
		return QUEST_DISPLAY_MARK_NONE;
	}

	//============================================================================================================
	// 1. 각 DisplayMode별 조건을 검사한다
	//============================================================================================================
	SI16 siDisplayMode = pclSystemNPCInfo->m_siImageDisplayMode;

	if ( QUESTDISPLAY_MODE_CONDITION == siDisplayMode )
	{
		SI32 siNowLevel		= pclMyChar->pclCI->clIP.GetLevel();
		SI32 siNeedLevel	= pclQuestInfo->clInitConditionInfo.siLevel;
		SI16 siClearCond	= pclQuestInfo->clQuestClearConditionInfo.siFlag;
		SI32 siQuestUinque	= pclQuestInfo->clQuestClearConditionInfo.siPara1;
		bool bLevelCheck	= false;
		bool bCondCheck		= true;	// 초기값을 true로 한 이유는 코딩양을 줄이기 위함

		// 레벨과 선행퀘스트 조건이 각가 다르게 적용될수 있으므로 분리해서 검사
		if ( siNeedLevel <= siNowLevel )
		{
			bLevelCheck = true;
		}

		if ( QUEST_CLEAR_COND_CLEARQUEST == siClearCond )
		{
			if ( true == pclMyChar->pclCI->clQuestInfo.IsClear(pclClient->pclQuestManager, siQuestUinque) )
			{
				bCondCheck = true;
			}
			else
			{	
				bCondCheck = false;
			}
		}

		if ( (false == bLevelCheck) || (false == bCondCheck) )
		{
			return QUEST_DISPLAY_MARK_NONE;
		}
	}
	else if ( QUESTDISPLAY_MODE_GOING == siDisplayMode)
	{
		if ( false == pclMyChar->pclCI->clQuestInfo.IsQuestClearOrGoing( siQuestType ) )
		{
			return QUEST_DISPLAY_MARK_NONE;
		}
	}

	//============================================================================================================
	// 2. 실질적으로 어떤 마크를 그려야하는지 검사한다
	//============================================================================================================
	cltPersonQuestUnitInfo*	pclunit = &pclMyChar->pclCI->clQuestInfo.clUnit[siQuestType];
	cltQuestInfo*			pclinfo = pclClient->pclQuestManager->GetQuestInfo(pclunit->siCurQuestUnique);

	if ( (NULL == pclunit) || (NULL == pclinfo) )
	{
		return QUEST_DISPLAY_MARK_NONE;
	}

	// 진행중 퀘스트가 없다면
	if( pclunit->siCurQuestUnique == 0 )
	{
		return QUEST_DISPLAY_MARK_GREETING;
	}
	// 진행중 퀘스트를 완료 한 상태라면
	// 퀘스트를 종료한 상황이라면, 
	else if ( true == pclMyChar->IsClearCondition(siQuestType, &pclinfo->clQuestClearConditionInfo) )
	{				
		return QUEST_DISPLAY_MARK_CLEAR;
	}

	return QUEST_DISPLAY_MARK_GOING;

}
