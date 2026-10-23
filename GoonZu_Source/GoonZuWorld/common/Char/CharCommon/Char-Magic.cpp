//---------------------------------
// 2003/10/19 김태곤
//---------------------------------

#include "..\..\Client\Client.h"

#include "..\..\Client\Music\Music.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"
#include "..\..\Util\Util.h"
#include "..\..\Order\Order.h"
#include "..\..\CommonLogic\MagicMgr\MagicAttribute\MagicAttribute.h"
#include "..\..\CommonLogic\MagicMgr\MagicMgr.h"

#include "..\..\Common\Skill\Skill-Manager.h"

#include "../../Char//CharServer/Char-Server.h"

extern cltCommonLogic* pclClient;



BOOL cltCharCommon::Magic()
{
	SI32 healability = 0 ;

	// 마법 시전 동작이 준비되어 있으면, 
	SI32 ref = pclKI->clCharAniManager.FindRefFromAnitype(ANITYPE_MAGIC);
	if(ref >= 0)
	{
		SetAnimation(ANITYPE_MAGIC);
	}
	else
	{
		SetAnimation(ANITYPE_ATTACK);
	}
	
	if(DoAnimationDelay() == FALSE)return FALSE;
	
	 if(clAniInfo.IsInstant() && bHitDoneSwitch == false)
	 {
		 bHitDoneSwitch = true;

		 MagicOperation();

	     OrderStop(siID, BY_COMPUTER);
	 }

	 if(clAniInfo.GetAniStep() == 0 )//&& clAniInfo.siRetryNum > 0 )
	 {
		 bHitDoneSwitch = false;
		 bBusySwitch	= false;
	   return TRUE;
	 }
	 else
	 {
		 // 행동 중에는 바쁘다. 
		 bBusySwitch	= true;
		 return FALSE;
	 }

}



void cltCharCommon::MagicOperation()
{
	SI32 attackid	= clCurOrder.siPara2;
	SI16 magickind	= clCurOrder.siPara3;

	if ( pclCM->IsValidID(attackid) == false )
	{
		return;
	}

	stMagicAttributeData* psMagicData = pclClient->pclMagicManager->GetMagicAttribute( magickind );
	if ( NULL == psMagicData )
	{
		return;
	}

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	if ( NULL == pMagicAttributePtr )
	{
		return;
	}

	SI32 returnval = 0;

	if ( MAGIC_CASTTYPE_BUF == psMagicData->si08MagicCastType )
	{
		if ( false == CanMagicBuf(magickind, &returnval) )			return;
	}
	else
	{
		if ( false == CanMagic(attackid, magickind, &returnval) )	return;
	}

	

	//////////////////////////////////////////////////////////////////////////
	// MANA Check
	//////////////////////////////////////////////////////////////////////////
	SI32 kind = 0;
	if(IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique()))	kind = GetKind();
	SI32 SkillType = pMagicAttributePtr->GetMagicSkillType(magickind);

	// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
	if( SKILL_SIKLLBOOK == SkillType && IsPC(GetID())  )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclCharSkillBookInfo->GetSkillBookMagic( GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return;

		SkillType = pclSkillBookMagic->GetSkillUnique();
	}

#ifdef _SAFE_MEMORY
	SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel(SkillType, kind, pclCI->clIP.GetLevel(), &pclCI->clCharItem.clItem[0], GetID());
#else
	SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel(SkillType, kind, pclCI->clIP.GetLevel(), pclCI->clCharItem.clItem, GetID());
#endif
	SI32 mana		= 0;

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind			= 0;
	SI32	siSubMagicOrganDamage	= 0;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(attackid);
	if( pclChar )
	{
		if( pclClient->IsCountrySwitch(Switch_SkillBook) && pclChar->pclCharSkillBookInfo )
		{
			stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( magickind, siSubMagickind );
			if( psSubMagicData )
			{
				siSubMagicOrganDamage = psSubMagicData->si16MagicOrganDamage;
			}
		}	
	}

	if ( pclClient->GameMode == GAMEMODE_SERVER )
	{
		// 마법 지연을 0으로 초기화한다.
		if ( MAGIC_CASTTYPE_BUF != psMagicData->si08MagicCastType )
		{
			SetMagicDelay(0);
		}
		
		// Mana를 소모한다. 
		mana = pclClient->pclMagicManager->GetSpendMana(magickind, SkillLevel,GetCharUnique());
		
		// MAGIC_TYPE_SOULGUARD 이면 스킬레발과 mana를 고정값으로 정한다.
		if(magickind == MAGIC_SOULGUARD_FIRE)
		{
			SkillLevel = 1;
			mana = 0;
		}
		// 소울 가드 펌킨이 사용하는 마법
		if(magickind == MAGIC_SOULGUARD_PUMKIN)
		{	
			SkillLevel = 1;
			mana = 0;
		}
		if(pclClient->IsCountrySwitch(Switch_SoulGuard))
		{
			// 펌킨 종룡 일정확률로 마법 사용시 마나를 0으로한다.
			SI32 siCharPersonID = pclCI->GetPersonID()	;
			SI32 siCharId = pclCM->GetIDFromPersonID( siCharPersonID)	;
			cltCharServer* pclCharServer = pclCM->GetCharServer(siCharId)	;
			if(pclCharServer != NULL )
			{
				SI32 siValue =  pclCharServer->SoulGuardAbilityActivity( SOULGUARD_ABILITY_MANA_ZERO );
				if ( 0 < siValue )
				{
					mana = 0;
				}
			}
		}
		
		if(mana)
		{
			DecreaseMana(mana);
			SetUpdateSwitch(UPDATE_BASICINFO, true, 0);
			if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				SetUpdateSwitch(UPDATE_IP, true, 0);
				SI32 parentid = GetSummonParentID() ;
				if ( IsPC(parentid) )
				{
					pclCM->CR[parentid]->SetUpdateSwitch(UPDATE_SUMMON,true ,0);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CHAR MAGIC
	//////////////////////////////////////////////////////////////////////////
	// SKILLTYPE이 있으면 해당 Magic를 SET한다.
	if ( SkillType!= -1 )
	{
		if ( MAGIC_CASTTYPE_BUF == psMagicData->si08MagicCastType )
		{
			m_clMagicBufMgr.PushMagicBuf( magickind, clPB.siMagicDecDelay, GetID() );

			// HP를 늘려주는 마법은 사용과 동시에 파라미터박스를 갱신한다
			SI32 siMagicGroup = pMagicAttributePtr->GetMagicGroup( magickind );
			if ( MAGIC_GROUP_BLESSHP == siMagicGroup )
			{
				ParametaBoxAction();
			}

			if ( GAMEMODE_SERVER == pclClient->GameMode )
			{
				SI32 siMagicOrganDamage = psMagicData->si16MagicOrganDamage 
										+ ((psMagicData->si16MagicOrganDamage * siSubMagicOrganDamage) / 100);
				if (pclCI->clHealthInfo.DecreaseOrg(	psMagicData->si16MagicOrganKind, 
														siMagicOrganDamage, 
														pclCI->clIP.GetLevel()) == true )
				{
					SetUpdateSwitch(UPDATE_HEALTH, true, 0);		// 클라이언트에 업데이트 시킨다. 
				}
			}
		}	// if ( MAGIC_CASTTYPE_BUF == psMagicData->si08MagicCastType ) 종료
		else
		{
			stMagicInfo clMagicInfo(magickind, GetCharUnique(), pclCM->CR[attackid]->GetCharUnique(), mana);
			pclClient->pclMagicManager->SetMagic(	(stMagicHeader *)&clMagicInfo, 
													clPB.GetAddMagicDamage(), 
													pclMap, 
													SkillLevel, 
													pclCI->clHealthInfo.GetOrg(psMagicData->si16MagicOrganKind) );

			// 서버에서만, 
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				// 마법사용에 따른 내장기관의 손상을 발생시킨다.
				if(IsPC(GetCharUnique()))
				{
					SI32 siMagicOrganDamage = psMagicData->si16MagicOrganDamage 
											+ ((psMagicData->si16MagicOrganDamage * siSubMagicOrganDamage) / 100);

					if(pclCI->clHealthInfo.DecreaseOrg(	psMagicData->si16MagicOrganKind, 
														siMagicOrganDamage, 
														pclCI->clIP.GetLevel()) == true	)
					{
						SetUpdateSwitch(UPDATE_HEALTH, true, 0);		// 클라이언트에 업데이트 시킨다. 
					}
				}
			}
		}	// else 종료
	}
}