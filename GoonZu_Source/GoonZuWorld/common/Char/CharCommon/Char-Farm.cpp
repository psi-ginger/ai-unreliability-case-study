//---------------------------------
// 2004/10/18 김태곤
//---------------------------------
#include "..\Common\CommonHeader.h"
#include "..\Common\Order\Order.h"
#include "..\Common\Util\Util.h"

#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"

#include "../CommonLogic/Land/Land.h"
#include "../CommonLogic/MsgRval-Define.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

// 농사를 지을 수 있는가 ?
bool cltCharCommon::CanFarm(SI32 x, SI32 y, SI32* prtnval)
{
	cltServer* pclserver = (cltServer*)pclClient;

	if(pclMap == NULL)return false;

	if(pclMap->IsInMapArea(x, y) == false)return false;

	if((pclMap->GetMouseTileAtb(x, y) & MOUSETILEATB_FARM) == 0)return false;

	SI32 siUseAbleDay	= 0;
	BOOL bUseAble		= FALSE;

	if ( pclClient->GameMode == GAMEMODE_SERVER )
	{
		// 소환수이면 농경술이나 농기구는 확인하지 않는다. 
		if(clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)
		{
			// 농경술을 확인한다. 
#ifdef _SAFE_MEMORY
			if(pclCI->clCharSkill.GetTotalSkillLevel(SKILL_FARM, 0, 0, &pclCI->clCharItem.clItem[0]) <= 0)
#else
			if(pclCI->clCharSkill.GetTotalSkillLevel(SKILL_FARM, 0, 0, pclCI->clCharItem.clItem) <= 0)
#endif
			{
				// 스킬제한없는 농경 퀘스트 중이라면 가능.
				SI16 siQuestType, siQuestUnique, siPara1, siPara2;
				SI16 siQuestClearCond = QUEST_CLEAR_COND_FARMNOSKILL;
				if( pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2) == false )
				{
					*prtnval = CANFARM_ERROR_NOSKILL;
					return false;
				}
			}
			
			// 농기구를 확인한다. 
			if( ActiveWeapon < 0 || ActiveWeapon >= MAX_ITEM_PER_PERSON )
			{
				*prtnval = CANFARM_ERROR_NOTOOL;
				return false;
			}
			cltItem* pclitem = &pclCI->clCharItem.clItem[ ActiveWeapon];
			bUseAble = pclserver->GetItemUseAbleDay(&pclCI->clCharItem.clItem[ActiveWeapon], &siUseAbleDay);

			// PCK : 낫의 사용기간을 확인해서 지났으면 더이상 채광을 할수없게 한다.
			if(siUseAbleDay < 0)	return false;

			if(pclClient->pclItemManager->IsItemInfoAtb(pclitem->siUnique, ITEMINFOATB_FARMTOOL)  == false)
			{
				*prtnval = CANFARM_ERROR_NOTOOL;
				return false;
			}

			// 도구의 내구도가 0일때 작동안함
			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				if ( 0 >= pclitem->GetDurability() )
				{
					*prtnval = CANFARM_ERROR_NOTOOL;
					return false;
				}
			}

		}
	}
	
	return true;
}

//---------------------------------
// 캐릭터를 농사짓게  만들기 위한 설정을 한다. 
//---------------------------------
bool cltCharCommon::SetFarm(SI32 dx, SI32 dy)
{
	if(dx <  0)						dx	=	0;
	if(dx >= pclMap->MH.MapXsize)	dx	=	pclMap->MH.MapXsize - 1;
    if(dy < 0)						dy	=	0;
 	if(dy >= pclMap->MH.MapYsize)	dy	=	pclMap->MH.MapYsize - 1;

	siFarmX	=	dx; 
	siFarmY	=	dy;

	siSubOrder = CHARWORKSTEP_DECIDELENGTH;
	
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		cltCharServer *pclChar = ( cltCharServer *)this;
		pclChar->siFarmingItemUnique = 0;

		// 임시로 빌린 농경지가 해제 되어야지...
		//pclChar->siFarmingRentVillageUnique = 0;
	}
	

	return TRUE;
}

bool cltCharCommon::Farm()
{

   SHORT status;

   if(siSubOrder == CHARWORKSTEP_MOVE)
   {
		status = Move();
		
		if(MoveDone())bBusySwitch = false;
		else
		{
			bBusySwitch = true;
		}

		if(bBusySwitch == false)
		{
		   // 목적지에 도착하거나 중간 지점에 도착하면, 
		   if(status > 0)
		   {
		      siSubOrder	 =	CHARWORKSTEP_DECIDELENGTH;
		   }

		}
   }

   if(siSubOrder == CHARWORKSTEP_DECIDELENGTH)
   {
	   // 적을 공격할 수 없는 상태라면 중단한다.
	   SI32 rtnval = 0;
	   if(CanFarm(siFarmX, siFarmY, &rtnval) == false)
	   {   
		   bBusySwitch	=	false;
		   return false;
	   }
	   
	   
	   if(GetX() == siFarmX && GetY() == siFarmY)
	   {
		   siSubOrder = CHARWORKSTEP_ACTION;
		   cltCharServer *pclChar = (cltCharServer *)this;
	   }
	   else
	   {
		   SetMove(siFarmX, siFarmY);
		   siSubOrder = CHARWORKSTEP_MOVE;
	   }
  
   }

   if(siSubOrder == CHARWORKSTEP_ACTION)
   {
	   bBusySwitch = false;

	    if(FarmAction() == true)
		{
		   siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;
		}

   }

	return true;
}


bool cltCharCommon::FarmAction()
{

	SetAnimation(ANITYPE_FARM);
	
	if(DoAnimationDelay() == FALSE)return false;

	// 실제의 논리적인 행위가 일어난다. 
	if(clAniInfo.IsInstant())
	{
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			cltCharServer *pclChar = (cltCharServer *)this;
			if ( IsPC( GetCharUnique() ) )
			{
				pclChar->FarmingAction( true );
			}
			else
			{
				pclChar->FarmingAction( false );
			}
		}
	}

	if(clAniInfo.GetAniStep() == 0  )//&& clAniInfo.siRetryNum > 0)
	{
		bHitDoneSwitch	= false;
		bBusySwitch		= false;
		
		return true;
	}
	else return false;
	
}
