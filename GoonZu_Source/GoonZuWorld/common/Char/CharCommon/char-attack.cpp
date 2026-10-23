//-----------------------------------------------
//  전투. 
//-----------------------------------------------

#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"

#include "..\..\Client\Music\Music.h"
#include "..\..\Client\InterfaceMgr\DialogController.h"

#include "../../Client/ninterface/npartydlg/NPartyDlg.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"
#include "Bullet\Bullet.h"
#include "Util\Util.h"
#include "..\..\Order\Order.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicAttribute\MagicAttribute.h"
#include "..\..\Common\Skill\Skill-Manager.h"
#include "..\..\Common\MainFunction.h"
#include "..\..\Common\Event\MofuMofu\MofuMofu.h"
#include "../../common/PartyMgr/PartyMgr.h"



//---------------------------
// CommonLogic
//---------------------------
#include "..\..\commonLogic\MsgType-Person.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Item.h"
#include "../../CommonLogic/KoinoboryFamily/KoinoboryFamily.h"
#include "../../CommonLogic/MsgRval-Define.h"


extern cltCommonLogic* pclClient;

// id가 나의 최소 범위안에 있는지 확인한다. 
BOOL cltCharCommon::IsInMinRange(SHORT id, SHORT minrange)
{
	if( pclCM->IsValidID(id) == false )		return FALSE;

	if(max( TABS(GetX() - pclCM->CR[id]->GetX()), TABS(GetY()- pclCM->CR[id]->GetY()) )<=minrange)return TRUE;

	return FALSE;
}


// 캐릭터를 공격할 수 있는지 여부를 알려준다. 
BOOL cltCharCommon::CanAttack(SI32 id, bool pvpflag )
{
	if( pclCM->IsValidID(id) == false )		return FALSE;

	cltCharPos clpos( pclCM->CR[id]->GetMapIndex(), pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY() );
	cltAttackTarget clTarget( id, &clpos, 0, 0,pvpflag );

	return CanAttack(&clTarget);
}

// PC vs !PC 캐릭터를 공격할 수 있는지 여부를 알려준다. 
bool cltCharCommon::IsEnemyPCvsNOTPC(SI32 myid, SI32 enemyid)
{
	//=================================================================
	// 짐꾼 왜구 
	//=================================================================
	// 상대가 짐꾼왜구면 레벨 30이상은 공격불가. 
	//if( pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_JAPANPORTERSOL")) && 
	//	pclCM->CR[myid]->pclCI->clIP.GetLevel() > 30)	return false;

	// 내가 짐꾼왜구면 상대 레벨 30이상은 공격불가. 
	//if( pclCM->CR[myid]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_JAPANPORTERSOL")) && 
	//	pclCM->CR[enemyid]->pclCI->clIP.GetLevel() > 30)return false;

	//=================================================================
	// 양이정찰병
	//=================================================================
	// 상대가 양이정찰병이면 레벨 50이상은 공격불가. 
	//if( pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_WESTPATROLSOL")) && 
	//	pclCM->CR[myid]->pclCI->clIP.GetLevel() > 50)	return false;

	// 내가 양이정찰병이면 상대 레벨 50이상은 공격불가. 
	//if( pclCM->CR[myid]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_WESTPATROLSOL")) && 
	//	pclCM->CR[enemyid]->pclCI->clIP.GetLevel() > 50)return false;


	return false;
}

	
// id가 나의 적인가 ?
bool cltCharCommon::IsEnemy(SI32 id)
{
	SI32 siMyID		= GetID();
	SI32 siEnemyID	= id;

	//=================================================================
	// 기본적은 공격가능성 검사 
	//=================================================================
	if(GetID() == id)return false;
	if(pclClient->GameMode == GAMEMODE_SERVER)	{	if(pclCM->IsValidID(siMyID) == false)		return false;	}
	else										{	if(pclCM->IsValidID(siMyID) == false)		return false;	}
	if(pclCM->CR[siMyID]->pclKI == NULL || pclCM->CR[siMyID]->pclKI->IsAtb(ATB_ATTACK) == false)									return false;
	if(pclClient->GameMode == GAMEMODE_SERVER)	{	if(pclCM->IsValidID(siEnemyID) == false)	return false;	}
	else										{	if(pclCM->IsValidID(siEnemyID) == false)	return false;	}
	if(pclCM->CR[siEnemyID]->pclKI == NULL || pclCM->CR[siEnemyID]->pclKI->IsAtb(ATB_ATTACK) == false)									return false;

	//--배틀로얄일경우는 무조건 적으로 판단해서 공격한다
	if( IsPC(siMyID) && IsPC(siEnemyID))
	{
		if ( pclCM->CR[siMyID]->GetMapIndex() == MAPINDEX_BATTLEROYAL_MABRIUM ||  pclCM->CR[siMyID]->GetMapIndex() == MAPINDEX_BATTLEROYAL_GRANDMABRIUM  )
		{
			if ( pclCM->CR[siEnemyID]->GetMapIndex() == MAPINDEX_BATTLEROYAL_MABRIUM ||  pclCM->CR[siEnemyID]->GetMapIndex() == MAPINDEX_BATTLEROYAL_GRANDMABRIUM  )
			{
				return true	;
			}
		}
	}

	// 감옥에 갖혀 있는 사람은 전혀 전투에 관여할수 없다.
	if(pclCM->CR[siEnemyID]->clInnerStatus.IsStatus(INNERSTATUS_PRISON)	== true)	return false;
	if(pclCM->CR[siMyID]->clInnerStatus.IsStatus(INNERSTATUS_PRISON)	== true)	return false;
	// 영자나라에 있으면 불가능
	if ( pclCM->CR[siEnemyID]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
		pclCM->CR[siEnemyID]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		return false ;
	}
	// 영자나라에 있으면 불가능
	if ( pclCM->CR[siMyID]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
		pclCM->CR[siMyID]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		return false ;
	}

	// [영훈] 수호정령_090525 : 수호정령은 공격 하지도, 받지도 않는다
	if ( (IDENTITY_SOULGUARD == pclCM->CR[siMyID]->clIdentity.siIdentity)
		|| (IDENTITY_SOULGUARD == pclCM->CR[siEnemyID]->clIdentity.siIdentity) )
	{
		return false;
	}
	
	//몬스터이면서 마을 이벤트 - 몬스터 습격에서 만들어진 몬스터라면.
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if(IsPC(siMyID))
		{
			//몬스터이면서 마을 이벤트 - 몬스터 습격에서 만들어진 몬스터라면.
			if(IsNPC( siEnemyID ) && pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_MONSTER_ATTACK_EVENT )
			{
				// 같은 마을이 아니면 이 이벤트 몬스터를 가격하지 못한다.
				if(pclClient->pclMonsterAttackEvent->GetEventVillageUnique() != pclCM->CR[siMyID]->pclCI->clBI.siHomeVillage)
				{
					return false;
				}
			}
		}
	}
	else
	{
		if(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_MONSTER_ATTACK_EVENT )
		{
			// 같은 마을이 아니면 이 이벤트 몬스터를 가격하지 못한다.
			if(pclClient->pclMonsterAttackEvent->GetEventVillageUnique() != pclCM->CR[siMyID]->pclCI->clBI.siHomeVillage)
			{
				return false;
			}
		}
	}
	//몬스터이면서 라쿤 피하기 이벤트 몬스터면 공격 못한다.
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if(IsPC(siMyID))
		{
			//몬스터이면서 마을 이벤트 - 몬스터 습격에서 만들어진 몬스터라면.
			if(IsNPC( siEnemyID ) && pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT )
			{
				return false;			
			}
		}

		if(IsNPC(siMyID))
		{
			// 이벤트 몬스터들은 팻을 공격 못한다.
			if(pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT)
			{
				SI32 siParentEnemyID		= pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();
				if ( siParentEnemyID > 0 )	return	 false	;
			}
		}
	}
	else
	{
		if(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT )
		{
			return false;
		}
		if(IsNPC(siMyID))
		{
			// 이벤트 몬스터들은 팻을 공격 못한다.
			if(pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT)
			{
				SI32  siParentEnemyID		= pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();
				if ( siParentEnemyID > 0 )	return	 false	;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// [지연] 복불복 : 몬스터이면서 복불복 이벤트 몬스터면 공격 못한다.
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if(IsNPC(siMyID))
		{
			// 이벤트 몬스터들은 팻을 공격 못한다.
			SI08 siMydentity = pclCM->CR[siMyID]->clIdentity.siIdentity;
			if ( siMydentity == IDENTITY_BOKBULBOK_EVENTMONSTER ||
				siMydentity == IDENTITY_BOKBULBOK_ITEMMONSTER ||
				siMydentity == IDENTITY_BOKBULBOK_KEYMONSTER ||
				siMydentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
			{
				SI32 siParentEnemyID		= pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();
				if ( siParentEnemyID > 0 )	return	 false;
			}

			// 펫도 이벤트 몬스터들은 공격 못하고.
			SI32 siParentEnemyID = pclClient->pclCM->CR[siMyID]->GetSummonParentID();

			if ( 0 < siParentEnemyID )
			{
				SI08 siEnemyIdentity = pclCM->CR[siEnemyID]->clIdentity.siIdentity;
				if ( siEnemyIdentity == IDENTITY_BOKBULBOK_EVENTMONSTER ||
					siEnemyIdentity == IDENTITY_BOKBULBOK_ITEMMONSTER ||
					siEnemyIdentity == IDENTITY_BOKBULBOK_KEYMONSTER ||
					siEnemyIdentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
				{
					return false;
				}
			}
		}
	}
	else
	{
		if(IsNPC(siMyID))
		{
			// 이벤트 몬스터들은 팻을 공격 못한다.
			SI08 siMydentity = pclCM->CR[siMyID]->clIdentity.siIdentity;
			if ( siMydentity == IDENTITY_BOKBULBOK_EVENTMONSTER ||
				siMydentity == IDENTITY_BOKBULBOK_ITEMMONSTER ||
				siMydentity == IDENTITY_BOKBULBOK_KEYMONSTER ||
				siMydentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
			{
				SI32 siParentEnemyID		= pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();
				if ( siParentEnemyID > 0 )	return	 false;
			}

			// 펫도 이벤트 몬스터들은 공격 못하고.
			SI32 siParentEnemyID = pclClient->pclCM->CR[siMyID]->GetSummonParentID();

			if ( 0 < siParentEnemyID )
			{
				SI08 siEnemyIdentity = pclCM->CR[siEnemyID]->clIdentity.siIdentity;
				if ( siEnemyIdentity == IDENTITY_BOKBULBOK_EVENTMONSTER ||
					siEnemyIdentity == IDENTITY_BOKBULBOK_ITEMMONSTER ||
					siEnemyIdentity == IDENTITY_BOKBULBOK_KEYMONSTER ||
					siEnemyIdentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
				{
					return false;
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	

	// 퀘스트 전용 몬스터 일때는 소환한 사람과 소환한 사람이 속해있는 파티원만 칠수있다
	if ( GAMEMODE_SERVER ==  pclClient->GameMode )
	{
		if ( IsPC(siMyID) && IsNPC(siEnemyID) )
		{
			if ( IDENTITY_QUEST == pclCM->CR[siEnemyID]->clIdentity.siIdentity )
			{
				SI32 siMyPersonID		= pclCM->CR[siMyID]->pclCI->GetPersonID();
				SI32 siDestPersonID		= pclCM->CR[siEnemyID]->clIdentity.clQuestInfo.m_siEnemyPersonID;
				if ( siMyPersonID != siDestPersonID )
				{
					if ( false == pclClient->pclPartyMgr->IsPartyMember(siDestPersonID, pclCM->CR[siMyID]->siPartyIndex) )
					{
						return false;
					}
				}
			}
		}
	}
	else if ( GAMEMODE_CLIENT ==  pclClient->GameMode )
	{
		if ( IDENTITY_QUEST == pclCM->CR[siEnemyID]->clIdentity.siIdentity )
		{
			SI32 siMyPersonID		= pclCM->CR[siMyID]->pclCI->GetPersonID();
			SI32 siDestPersonID		= pclCM->CR[siEnemyID]->clIdentity.clQuestInfo.m_siEnemyPersonID;

			if ( siMyPersonID != siDestPersonID )
			{
				cltClient* pclclient = (cltClient*)pclClient;
				if ( NULL == pclclient )
				{
					return false;
				}

				if ( NULL == pclclient->m_pDialog[NPARTY_DLG] )
				{
					return false;
				}

				SI32 siFindCharID = pclCM->CR[siEnemyID]->clIdentity.clQuestInfo.m_siEnemyCharID;
				if ( false == ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->IsPartyMember( siFindCharID ) )
				{
					return false;
				}
			}
		}
	}

	// 귀속전용 몬스터는 불러낸 캐릭터만 칠 수 있다
	if ( IDENTITY_SOULBOUND == pclCM->CR[siEnemyID]->clIdentity.siIdentity )
	{
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			SI32 siMyPersonID		= pclCM->CR[siMyID]->pclCI->GetPersonID();
			SI32 siDestPersonID		= pclCM->CR[siEnemyID]->clIdentity.clSoulBound.m_siPersonID;

			if ( siMyPersonID != siDestPersonID )
			{
				return false;
			}
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			// 클라이언트 일때는 나만 검사한다
			if ( 1 == siMyID )
			{
				SI32 siMyPersonID		= pclCM->CR[siMyID]->pclCI->GetPersonID();
				SI32 siDestPersonID		= pclCM->CR[siEnemyID]->clIdentity.clSoulBound.m_siPersonID;

				if ( siMyPersonID != siDestPersonID )
				{
					return false;
				}
			}
		}

	}

	// 골든위크 이벤트 -> [영훈] 아래 제한사항은 없어짐
	/*
	if( IsPC( pclCM->CR[siMyID]->GetCharUnique() ) )
	{
		if( IsNPC( pclCM->CR[siEnemyID]->GetCharUnique() ) )
		{
			SI32 siAttackerLevel = pclCM->CR[siMyID]->pclCI->clIP.GetLevel();
			if( pclCM->CR[siEnemyID]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYRED") ) )
			{
				if( siAttackerLevel < 50 || siAttackerLevel > 100 )		return false;
			}
			else if( pclCM->CR[siEnemyID]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYBLUE") ) )
			{
				if( siAttackerLevel < 30 || siAttackerLevel > 80 )		return false;
			}
			else if( pclCM->CR[siEnemyID]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYYELLOW") ) )
			{
				if( siAttackerLevel < 1 || siAttackerLevel > 50 )		return false;
			}
		}
	}
	*/

	// 모후모후 이벤트 던젼에 있는경우 - 특수한 경우만 공격이 가능하도록 설정된다.
	if ( pclCM->CR[siMyID]->GetMapIndex()		== MAPINDEX_MOFUMOFULAND &&
		 pclCM->CR[siEnemyID]->GetMapIndex()	== MAPINDEX_MOFUMOFULAND )
	{
		SI32 siMofuMofuEventColl = BLOCK_EVENT4 | BLOCK_EVENT5 | BLOCK_EVENT6;
		cltMapCommon* pclmymap		= pclClient->pclMapManager->GetMapPointer(pclCM->CR[siMyID]->GetMapIndex());
		cltMapCommon* pclenemymap	= pclClient->pclMapManager->GetMapPointer(pclCM->CR[siEnemyID]->GetMapIndex());
		if(pclmymap != NULL && pclenemymap != NULL)
		{
			SI32 siMyColl		= pclmymap->CalcCollInfo(pclCM->CR[siMyID]->GetX(), pclCM->CR[siMyID]->GetY());
			SI32 siEnemyColl	= pclmymap->CalcCollInfo(pclCM->CR[siEnemyID]->GetX(), pclCM->CR[siEnemyID]->GetY());

			siMyColl	= siMyColl		& siMofuMofuEventColl;
			siEnemyColl = siEnemyColl	& siMofuMofuEventColl;

			// 필드내에 있고 같은 필드에 존재한다면 공격 가능
			if( (siMyColl != 0 && siEnemyColl != 0) &&
				(siMyColl == siEnemyColl))
			{ 
				// 적이 모후모후 이벤트 몬스터면 CanAttack이 0이면 공격할 수 없다.
				if(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_MOFUMOFUEVENT && pclCM->CR[siEnemyID]->clIdentity.clMofoMofuEvent.siCanAttack == 0)
					return false;

				if( pclClient->GameMode == GAMEMODE_SERVER )
				{
					// PC와 소환수는 공격을 하지 않는다.
					if( (pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON && IsPC(siEnemyID)) ||
						(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON && IsPC(siMyID)) )
						return false;

					cltServer* pclserver = (cltServer*)pclClient;
					SI32 siCurrentVillage = pclserver->pclMofuMofuEvent->GetCurrentEventVillage();

					// 이벤트 참가국이 없으면 누가 공격을??
					if(siCurrentVillage <= 0)			return false;

					// 참가국의 국민이 아니면 공격을 할수 없다.
					if( (IsPC(siMyID) && pclCM->CR[siMyID]->pclCI->clBI.siHomeVillage != siCurrentVillage) || 
						(IsPC(siEnemyID) && pclCM->CR[siEnemyID]->pclCI->clBI.siHomeVillage != siCurrentVillage) )
					{
						return false;
					}
				}
				else if( pclClient->GameMode == GAMEMODE_CLIENT )
				{
					// PC와 소환수는 공격을 하지 않는다.
					if( (siMyID == 1 && pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON) ||
						(siEnemyID == 1 && pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON))
						return false;

					// 참가국의 국민이 아니면 공격을 할수 없다.
					cltClient* pclclient = (cltClient*)pclClient;
					SI32 siCurrentVillage = pclclient->siMofuMofuEvent_VilliageUnique;

					// 이벤트 참가국이 없으면 누가 공격을??
					if(siCurrentVillage <= 0)			return false;

					// 참가국의 국민이 아니면 공격을 할수 없다.
					if( (siMyID == 1 && pclCM->CR[siMyID]->pclCI->clBI.siHomeVillage != siCurrentVillage) || 
						(siEnemyID == 1 && pclCM->CR[siEnemyID]->pclCI->clBI.siHomeVillage != siCurrentVillage) )
					{
						return false;
					}
				}
				else
					return false;

				// 같은 신분끼리는 공격할 수 없다.
				if(pclCM->CR[siMyID]->clIdentity.siIdentity == pclCM->CR[siEnemyID]->clIdentity.siIdentity)
					return false;

				return true;
			}
			else
				return false;
		}
	}

	// by LEEKH 2007.12.28 - TRANSFORM
	// 공격 애니가 없는 경우에는 죽지 않는다. 
	if(pclCM->CR[siMyID]->GetKindInfo()->clCharAniManager.FindRefFromAnitype(ANITYPE_ATTACK) < 0)		
		return false;
	// 키메라와 성벽은 마을간 적대 관계를 갖지만 서로 공격을 하지 않게끔 해준다.
	if( ( pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_VILLAGEHUNT && pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_CASTLE )
		||  ( pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_CASTLE && pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_VILLAGEHUNT ) )
	{
		return false;
	}

	// 개인 PVP 모드 체크
	SI32 siParentMyID		= 0;pclClient->pclCM->CR[ siMyID ]->GetSummonParentID();
	SI32 siParentEnemyID	= 0;pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();

	if( pclClient->GameMode == GAMEMODE_SERVER)
	{
		siParentMyID		= pclClient->pclCM->CR[ siMyID ]->GetSummonParentID();
		siParentEnemyID		= pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();

		if( siParentMyID	== 0 )		siParentMyID = siMyID;
		if( siParentEnemyID == 0 )		siParentEnemyID = siEnemyID;
	}
	else
	{
		if( pclClient->pclCM->CR[ siMyID ]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
		{
			siParentMyID	= pclCM->GetIDFromCharUnique(pclClient->pclCM->CR[ siMyID ]->clIdentity.clSummonInfo.siParentCharUnique);
		}
		else
		{
			siParentMyID	= siMyID;
		}

		if( pclClient->pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
		{
			siParentEnemyID	= pclCM->GetIDFromCharUnique(pclClient->pclCM->CR[siEnemyID]->clIdentity.clSummonInfo.siParentCharUnique);
		}
		else
		{
			siParentEnemyID = siEnemyID;
		}
	}
	if( siParentMyID != siParentEnemyID )		
	{
		if( pclClient->pclCM->IsValidID( siParentMyID ) )
		{
			cltCharCommon* pclMyChar = pclClient->pclCM->GetChar(siParentMyID);
			cltCharCommon* pclEnemyChar = pclClient->pclCM->GetChar(siParentEnemyID);
			if(pclMyChar == NULL)						return false	;
			if(pclEnemyChar == NULL)						return false	;

			// 한쪽이라도 개인PVP 상태라면,
			if( pclMyChar->siEnemUnique > 0 || pclEnemyChar->siEnemUnique > 0 )
			{
				if( pclMyChar->siEnemUnique == pclEnemyChar->GetCharUnique() )
					return true;
				else
					return false;
			}
			// 상태가 적길드 원인지 확인 한다.
			if(pclMyChar->pclCI->clBI.clGuildRank.siGuildUnitIndex > 0 )
			{
				SI32 siMyGuildUnique = pclMyChar->pclCI->clBI.clGuildRank.siGuildUnitIndex	;
				SI32 siEnemyGuildUnique = pclEnemyChar->pclCI->clBI.clGuildRank.siGuildUnitIndex	;

				SI32 siMyEnemy = pclMyChar->pclCI->clBI.clGuildRank.GetEnemyGuild()	;
				SI32 siEnemyEnemy = pclEnemyChar->pclCI->clBI.clGuildRank.GetEnemyGuild();
				if ( siMyEnemy == siEnemyGuildUnique && siEnemyEnemy== siMyGuildUnique)
				{
					return true;

				}
			}
		}			
	}
	// 모든 사용자와 적대적인 모드라면, 
	if(pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLPC))
	{
		if(pclCM->CR[siMyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLNPC))
		{	
			return true;
		}
	}	
	// 모든 NPC와 적대적인 모드라면, 
	if(pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLNPC))
	{
		if(pclCM->CR[siMyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLPC))
		{
			return true;
		}
	}
	
	// [기형] 배틀로얄
	if(pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLPC) && pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLNPC))
	{
		if(pclCM->CR[siMyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLPC) && pclCM->CR[siMyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ALLNPC))
		{
			return true;
		}
	}
	// 마을간의 적대관계를 근거로 하는 적대 모드라면, 
	if(pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_VILLAGE))
	{
		if(pclClient->bVillageWarSwitch)
		{
			SI16 MyHomeVillage  = pclCM->CR[siMyID]->pclCI->clBI.siHomeVillage;
			SI16 EnemyVillage   = pclCM->CR[siEnemyID]->pclCI->clBI.siHomeVillage;
			// 내가 속한주소지가 없거나 상대방이 속한 주소지가 없으면 실패
			if(MyHomeVillage && EnemyVillage )
			{
				// 서로 적대적인 마을이 아니면 실패 
				if(pclClient->pclVillageManager->IsAggressiveVillage(MyHomeVillage, EnemyVillage) == TRUE)
				{
					if(	pclCM->CR[siMyID]->GetMapIndex() !=0							
					|| pclCM->CR[siMyID]->GetCurrentVillageUnique() != MyHomeVillage	
					|| pclCM->CR[siMyID]->GetCurrentVillageUnique() != EnemyVillage)
					{
						// 상대가 있는 마을이 나의 주소지나 상대방의 주소지가 아니면 false
						if(	pclCM->CR[siEnemyID]->GetMapIndex() != 0							
						|| pclCM->CR[siEnemyID]->GetCurrentVillageUnique() != MyHomeVillage	
						|| pclCM->CR[siEnemyID]->GetCurrentVillageUnique() != EnemyVillage)
						{
							return true;
						}
					}
				}
			}
		}
	}

	// 특정 마을과만 적대 관계라면, (상대가 그 속성을 갖고 있다면,) 
	if(pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ONEVILLAGE))
	{		
		// 내가 그 특정 마을이어야 한다. 
		if(pclCM->CR[siMyID]->pclCI->clBI.siHomeVillage == pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.siPara1)
		{
			return true;
		}
	}
	// 특정 마을과만 적대 관계라면, (자신이 그 속성을 갖고 있다면, ) 
	if(pclCM->CR[siMyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_ONEVILLAGE))
	{
		if( pclCM->CR[siEnemyID]->pclCI->clBI.siHomeVillage == pclCM->CR[siMyID]->clInnerStatus.clEnemyType.siPara1 )
		{
			return true;
		}
	}
	if( pclClient->GameMode == GAMEMODE_SERVER)
	{
		siParentMyID		= pclClient->pclCM->CR[ siMyID ]->GetSummonParentID();
		siParentEnemyID		= pclClient->pclCM->CR[siEnemyID]->GetSummonParentID();

		if( siParentMyID	== 0 )		siParentMyID = siMyID;
		if( siParentEnemyID == 0 )		siParentEnemyID = siEnemyID;
	}
	else
	{
		if( pclClient->pclCM->CR[ siMyID ]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
		{
			siParentMyID	= pclCM->GetIDFromCharUnique(pclClient->pclCM->CR[ siMyID ]->clIdentity.clSummonInfo.siParentCharUnique);
		}
		else
		{
			siParentMyID	= siMyID;
		}

		if( pclClient->pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
		{
			siParentEnemyID	= pclCM->GetIDFromCharUnique(pclClient->pclCM->CR[siEnemyID]->clIdentity.clSummonInfo.siParentCharUnique);
		}
		else
		{
			siParentEnemyID = siEnemyID;
		}
	}

	// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
	//if( pclClient->IsWhereServiceArea(ConstSwitch_PVP) )
	if ( pclClient->siEnablePVP == 1 )
	{
		// 적ID가 없어도 Pk 맵에서는 쌈 해야 하잖아~
		if( (IsPC(siMyID)) && pclClient->pclMapManager->IsPkMap( pclCM->CR[siMyID]->GetMapIndex() ) )
			// 사람 공격할때
			if( pclCM->CR[siMyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_PVP) )
			{
				if(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )      
				{
				}
				else if( pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
				{
				}
				else if( (IsPC(siEnemyID)) && pclCM->CR[siEnemyID]->clInnerStatus.clEnemyType.IsType(ENEMYTYPE_PVP) )
				{
					return	true	;
				}
			}
	}


	
	//#endif
	return false;
}

// id가 나의 적인가 ?
bool cltCharCommon::IsEnemyPCvsPC(SI32 id)
{
	SI32 siMyID		= GetID();
	SI32 siEnemyID	= id;

	//=================================================================
	// 기본적은 공격가능성 검사 
	//=================================================================
	if(GetID() == id)return false;
	if(pclClient->GameMode == GAMEMODE_SERVER)	{	if(pclCM->IsValidID(siMyID) == false)		return false;	}
	else										{	if(pclCM->IsValidID(siMyID) == false)		return false;	}
	if(pclCM->CR[siMyID]->pclKI == NULL || pclCM->CR[siMyID]->pclKI->IsAtb(ATB_ATTACK) == false)									return false;
	if(pclClient->GameMode == GAMEMODE_SERVER)	{	if(pclCM->IsValidID(siEnemyID) == false)	return false;	}
	else										{	if(pclCM->IsValidID(siEnemyID) == false)	return false;	}
	if(pclCM->CR[siEnemyID]->pclKI == NULL || pclCM->CR[siEnemyID]->pclKI->IsAtb(ATB_ATTACK) == false)									return false;

	// 이젠 pvp 되면 thghks
	//if(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )      return false;
	// 감옥에 갖혀 있는 사람은 전혀 전투에 관여할수 없다.
	if(pclCM->CR[siEnemyID]->clInnerStatus.IsStatus(INNERSTATUS_PRISON)	== true)	return false;
	if(pclCM->CR[siMyID]->clInnerStatus.IsStatus(INNERSTATUS_PRISON)	== true)	return false;
	// 영자나라에 있으면 불가능
	if ( pclCM->CR[siEnemyID]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
		pclCM->CR[siEnemyID]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		return false ;
	}
	// 영자나라에 있으면 불가능
	if ( pclCM->CR[siMyID]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
		pclCM->CR[siMyID]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		return false ;
	}

	// PVP를 할 수 있나? pvp
	if( pclClient->pclMapManager->IsPkMap( pclCM->CR[siMyID]->GetMapIndex() ) &&	pclCM->CR[siMyID]->pclCI->clIP.GetLevel() >= 50)
	{
		if( pclClient->pclMapManager->IsPkMap( pclCM->CR[siEnemyID]->GetMapIndex() ) &&	pclCM->CR[siEnemyID]->pclCI->clIP.GetLevel() >= 50)
		{
			if(pclCM->CR[siEnemyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )      
			{
			}
			else if( pclCM->CR[siMyID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
			{
			}
			else
			{
				return true;
			}		
		}
	}

	//// 사람대사람	PVP	
	//SI32 siMyPVPCharUnique			=	pclCM->CR[siMyID]->siEnemyID	;				// 내가 가지고 있는 CHARUNIQUE
	//SI32 siEnemyPVPCharUnique		=	pclCM->CR[siEnemyID]->siEnemyID	;				// 적이 가지고 있는 CHARUNIQUE
	//
	//SI32 siEnemyPVPID				=	pclCM->CR[siEnemyID]->GetID()	;					// 찍은 적의 ID
	//SI32 siMyEnemyID				=	pclCM->GetIDFromCharUnique( siMyPVPCharUnique )	;	// 내가 가진 적케릭터의 ID

	//SI32 siMyParentID				=	pclCM->CR[siMyID]->GetSummonParentID() ;		// 나의 PARENTID
	//SI32 siEnemyParentID			=	pclCM->CR[siEnemyID]->GetSummonParentID() ;		// 적의 PARENTID	
	//SI32 siParentPVPId				=	0	;
	//SI32 siEnemyParentPVPId			=	0	;
	//SI32 siMyParentCharUnique		=	0	;
	//SI32 siEnemyParentCharUnique	=	0	;
	//// 소환수 일때만 구하기 위해서
	//if ( siMyParentID > 0) 
	//{
	//	siParentPVPId				=	pclCM->CR[siMyParentID]->siEnemyID	;			// 나의 PARENT가 가기진 적 ID
	//	siMyParentCharUnique		=	pclCM->GetIDFromCharUnique( siMyParentID )	;	// 나 부모의 CHARUNIQUE
	//}
	//if ( siEnemyParentID >0 ) 
	//{
	//	siEnemyParentPVPId			=	pclCM->CR[siEnemyParentID]->siEnemyID	;		// 적의 PARENT가 가기진 적 ID
	//	siEnemyParentCharUnique	=	pclCM->GetIDFromCharUnique( siEnemyParentID )	;	// 적의 부모의 CHARUNIQUE
	//}	
	//// 적을 때릴때, 내가 가진 적 ID가 존재 하고, 찍은 케릭터의 케릭터 유니크와 내가 가진 적아이디의 케릭터 유니크가 동일할때
	//if ( siMyPVPCharUnique > 0 && siEnemyPVPID == siMyEnemyID ){ return true; }
	//// 소환수가 공격 할떄 .
	//else if ( siMyParentID )  // 내가 소환수 이고 
	//{
	//	if ( siParentPVPId > 0 ) // 내부모 케릭터가 적 아이디가 있어야 한다
	//	{
	//		if( siMyParentID == siEnemyParentPVPId )	// 나의 부모 아이디와 적의 적케릭터가 동일하면
	//		{
	//			return true	;
	//		}
	//	}
	//}
	//else if (siEnemyParentID) // 적이 패런트 아이디가 있는 소환수이고
	//{
	//	if( siEnemyParentCharUnique == siMyPVPCharUnique )
	//	{
	//		//적부모의케릭터 유니크와 나의 케릭터 유니크가 같으면 적이다.
	//		return true	;
	//	}

	//}
	return false;
}

// 캐릭터를 공격할 수 있는지 여부를 알려준다. 
BOOL cltCharCommon::CanAttack(const cltAttackTarget* pcltarget)
{
	if(pcltarget == NULL)								return FALSE;
	
	// 화면에 볼 수 없는 캐릭터라면 공격할 수 없다. 
	if(pclCM->IsAlive( pcltarget->siAttackID )==FALSE)	return FALSE;

	// 자기 자신은 공격할 수 없다.
	if(pcltarget->siAttackID == siID)					return FALSE;

	// 지상 공격 속성이 없으면 공격할 수 없다. 
	if(IsAtb(ATB_ATTACK)==FALSE)						return FALSE;

	// 지도 영역이 서로 다르면 공격할 수 없다. 
	if(GetMapIndex() != pcltarget->clPos.GetMapIndex())	return FALSE;

	// 적의 현재 지도 위치와 내가 서로 다르면 공격할 수 없다. 
	if(GetMapIndex() != pclCM->CR[ pcltarget->siAttackID ]->GetMapIndex())return FALSE;

	// PVP를 할 수 있나? -pvp 테스트라 맵인덱스는 0;;
	if( true == pcltarget->bPVPHit )
	{
		// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
		//if( pclClient->IsWhereServiceArea( ConstSwitch_PVP ) )
		if(pclClient->siEnablePVP == 1)
		{
			// 서로 적 아이디를 가지고 있으면 맵인덱스로 체크를 안해야 한다.
			SI32 siMyId = GetID()	;	// 내아이디를 받아온다.
			SI32 siMyEnemy = pclClient->pclCM->CR[ siMyId ]->siEnemUnique ;	// 내 아이디로 적 아이디를 찾는다.
	
			SI32 MuPvpEnemy = pclCM->GetIDFromCharUnique( siMyEnemy );		// 적의 케릭터 아이디를 토대로 personID를 찾는다.
			SI32 siAttackID = pcltarget->siAttackID	;						// 공격할 대상 ID

			if ( siMyEnemy > 0 && siAttackID > 0 && MuPvpEnemy == siAttackID  ) 
			{
			}
			else if (siMyEnemy <= 0 && siAttackID <= 0  )  // 적 아이디를 가지지 않았을때 이 맵을 체크해서 pvp 맵인지 체크
			{
				SI32 parentMap = pclClient->pclMapManager->GetParentMap( GetMapIndex() );
				if(false == pclClient->pclMapManager->IsPkMap( parentMap ) || pclCI->clIP.GetLevel() < 50  )
					return FALSE;
				parentMap = pclClient->pclMapManager->GetParentMap( pclCM->CR[pcltarget->siAttackID]->GetMapIndex() );
				if( false == pclClient->pclMapManager->IsPkMap( parentMap ) || pclCM->CR[pcltarget->siAttackID]->pclCI->clIP.GetLevel() < 50)
					return FALSE;

				if( clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
					return FALSE;

				if( pclCM->CR[pcltarget->siAttackID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
					return FALSE;
			}
		}
	}
//#endif
	else
	{
		if(IsEnemy(pcltarget->siAttackID) == false)
			return FALSE;			
	}

	cltCharCommon* pchEnemychar = pclCM->GetChar( pcltarget->siAttackID )	;
	if(  pchEnemychar != NULL )
	{
		if(pchEnemychar->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT)	return FALSE;


	}
	
//	if(IsEnemy(pcltarget->siAttackID) == false)return FALSE;

	// 초기 보호 상태에 있는 캐릭터는 공격할 수 없다. 
	if(pclCM->CR[ pcltarget->siAttackID ]->GetInitProtectSwitch() == true)return FALSE;

	// 타깃이 피가 없다면 공격할 수 없다.
	if(pclCM->CR[pcltarget->siAttackID]->GetLife() <= 0 ) return FALSE ;
	// 내가 피가 없어도 공격할 수 없다.
	if(GetLife() <=0 ) return FALSE ;

	//[강형] 모후모후던젼 이벤트 - 마마 모후모후는 모후모후향수를 사용한 유저중 50레벨 이상만 공격한다.
	if(GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_MAMARACCOON")))
	{
		if(IsPC(pcltarget->siAttackID))
		{
			cltBufInfo * pclMyBuf = &pclCM->CR[ pcltarget->siAttackID ]->pclCI->clBI.clBufInfo;

			BOOL bMofuMofuPerfume = FALSE;

			if(pclMyBuf->IsHaveBuf(BUF_MOFUMOFU_PERFUME))		bMofuMofuPerfume = TRUE;
			if(pclCM->CR[pcltarget->siAttackID]->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique == ITEMUNIQUE(23090))	bMofuMofuPerfume = TRUE;

			if(bMofuMofuPerfume && pclCM->CR[ pcltarget->siAttackID ]->pclCI->clIP.GetLevel() < 50)
			{
				return FALSE;
			}
		}
	}

	// 사용자 캐릭터인 경우
	SI32 siParentID = GetSummonParentID();

	if ( IsPC(GetCharUnique()) || IsPC(siParentID) )
	{
		//[영진] 공격시 프리미엄 개인 상점이 개설되어있다면 공격 못한다.

		if( bPersonalShopSwitch == true && bPremiumPersonalShop == true )
		{
			return FALSE;
		}

		if( pclCM->CR[pcltarget->siAttackID]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SANTARACCOON_ZIQ")) )
		{
			if( pclCI->clIP.GetLevel() < 10 
				|| pclCI->clIP.GetLevel() > 40   )
			{
				return FALSE;
			}

		}

		//cyj 공격시 화살, 탄환 필요여부
		bool bBulletswitch = false;

		if(pclClient->GameMode == GAMEMODE_SERVER)	
		{
			bBulletswitch = true;
		}
		else
		{
			if(GetID() == 1)		// 자기 자신의 경우이거나. 
			{
				bBulletswitch = true;
			}
			if(GetSummonParentID() == 1)	// 자신의 소환수인 경우에, 
			{
				bBulletswitch = true;
			}
		}

		if (bBulletswitch == true)
		{
			SI16 siBulletUnique = 0;
			BOOL bUseBullet = FALSE;
			if( clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				cltKindInfo* pclki = NULL;
				if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
				{
					pclki = pclClient->pclKindInfoSet->pclSummonKI[GetKind()];
				}
				else
				{
					pclki = pclClient->pclKindInfoSet->pclKI[GetKind()];
				}

				if ( (pclki) && (pclki->siSummonEquip) )
				{
					bUseBullet = GetBulletUnique_Summon(GetSummonIndex(), &siBulletUnique);
				}
	
			}
			else
			{
				bUseBullet = GetBulletUnique_Char(&siBulletUnique);
			}

			// 사용할 수 있는 블렛이 없는 경우 클라이언트는 에러메시지를 표시한다.
			if( bUseBullet && siBulletUnique <= 0 )
			{
				if (pclClient->GameMode == GAMEMODE_CLIENT)
				{
					cltClient *pclclient = (cltClient*)pclClient;
					TCHAR* pTitle = NULL;
					TCHAR* pText = NULL;

					// PCK : 무기 타입이 활이면 화살이 없다는 메시지를, 총이면 총알이 없다는 메시지를 보여준다.
					if(GetAttackType() == ATTACKTYPE_BOW || GetAttackType() == ATTACKTYPE_BOW1 || GetAttackType() == ATTACKTYPE_NEW_BOW1)
					{
						pTitle = GetTxtFromMgr(836);
						pText = GetTxtFromMgr(5835);
					}
					else if (GetAttackType() == ATTACKTYPE_GUN || GetAttackType() == ATTACKTYPE_GUN1 || GetAttackType() == ATTACKTYPE_NEW_GUN1)
					{
						pTitle = GetTxtFromMgr(836);
						pText = GetTxtFromMgr(5836);
					}

					if (pclclient->m_pDialog[ NOTIFYMSG_DLG ])
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
				}
				// 소환수 이면서 서버일때는 클라이언트로 안되는 이유를 보내준다(소환수 공격의 경우 클라에서 선처리를 안하기 때문에)
				else if ( (GAMEMODE_SERVER == pclClient->GameMode) && (IDENTITY_PERSON_SUMMON == clIdentity.siIdentity) )
				{
					cltServer*		pclServer		= (cltServer*)pclClient;
					cltCharServer*	pclParentChar	= pclCM->GetCharServer( siParentID );
					
					if ( (pclServer) && (pclParentChar) )
					{
						SI16 siProjectileType = 0;
						switch ( GetAttackType() ) 
						{
							case ATTACKTYPE_BOW:
							case ATTACKTYPE_BOW1:
							case ATTACKTYPE_NEW_BOW1:
								{
									siProjectileType = 1;
								}
								break;
							case ATTACKTYPE_GUN:
							case ATTACKTYPE_GUN1:
							case ATTACKTYPE_NEW_GUN1:
								{
									siProjectileType = 2;
								}
								break;
						}

						pclServer->SendServerResponseMsg( 0, SRVAL_SUMMON_DONOTHAVE_PROJECTILE, siProjectileType, 0, pclParentChar->GetCharUnique() );
					}
				}

				return FALSE;
			}
		}
	}

	return TRUE;
}


bool cltCharCommon::CanMagicManaDelay(SI32 magickind, SI32* preturn )
{

	//---------------------------------------------
	// 마법 종류에 따라 공격 가능 여부를 결정한다. 
	stMagicAttributeData* psMagicData = pclClient->pclMagicManager->GetMagicAttribute( magickind ); 
	if( psMagicData == NULL )		return false;

	//========================================================
	// 마나 검사 
	//========================================================
	// Mana가 충분해야 한다. 
	//SKILL LEVEL에 의한 MANA 소비량 
	SI32 kind = 0;
	if(IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique()))	kind = GetKind();
	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	SI32 skilltype = pMagicAttributePtr->GetMagicSkillType(magickind);
	
	// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
	if( SKILL_SIKLLBOOK == skilltype && IsPC(GetID())  )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclCharSkillBookInfo->GetSkillBookMagic( GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return false;
		
		skilltype = pclSkillBookMagic->GetSkillUnique();	
	}

#ifdef _SAFE_MEMORY
	SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( skilltype, kind, pclCI->clIP.GetLevel(), &pclCI->clCharItem.clItem[0] );
#else
	SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( skilltype, kind, pclCI->clIP.GetLevel(), pclCI->clCharItem.clItem );
#endif
	// Skill Level에 의한 영향력을 주고 싶으면 영향력을 넣어 마나소비량을 계산한다.
	SI32 mana = pclClient->pclMagicManager->GetSpendMana( magickind, SkillLevel, GetCharUnique() );
	

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind	= 0;
	SI32 	siSubDelay 		= 0;
	if( pclClient->IsCountrySwitch(Switch_SkillBook) && pclCharSkillBookInfo )
	{
		stMagicAttributeData*	psSubMagicData = pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( magickind, siSubMagickind );
		if( psSubMagicData )
		{
			siSubDelay = psMagicData->si32MagicDelay;
		}
	}
	
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		if(GetMana() < mana)
		{
			*preturn = CANUSEMAGIC_ERROR_NEEDMANA;
			return false;
		}
	}
	
	//========================================================
	// 마법 지연 
	//========================================================
	// 충분한 마법 지연 시간을 확보했는지 확인한다. 
	SI32 siDelay = psMagicData->si32MagicDelay - (psMagicData->si32MagicDelay * clPB.siMagicDecDelay / 100 );
	siDelay += (siDelay * siSubDelay) / 100;
	if(GetMagicDelay() < siDelay )
	{
		*preturn = CANUSEMAGIC_ERROR_NEEDTIMEDELAY;
		return false;
	}

	return true;
}

// 마법으로 캐릭터를 공격할 수 있는지 여부를 알려준다. 
bool cltCharCommon::CanMagic(SI32 targetid, SI32 magickind, SI32* preturn, bool PVP )
{
	// 일단 초기화 시킨다.
	*preturn = CANUSEMAGIC_ERROR_INVALID;

	//========================================================
	// 기본 검사 
	//========================================================
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		// 화면에 볼 수 없는 캐릭터라면 공격할 수 없다. 
		if(pclCM->IsAlive( targetid )	==	FALSE)	return false;
	}
	else
	{
		// 화면에 볼 수 없는 캐릭터라면 공격할 수 없다. 
		if(pclCM->IsValidID( targetid ) == FALSE)	return false;
	}


	//========================================================
	// 위치 검사 
	//========================================================
	// 적의 현재 지도 위치와 내가 서로 다르면 마법을 사용할 수 없다. 
	if(GetMapIndex() != pclCM->CR[ targetid]->GetMapIndex())	return false;

	//---------------------------------------------
	// 마법 종류에 따라 공격 가능 여부를 결정한다. 
	stMagicAttributeData* psMagicData = pclClient->pclMagicManager->GetMagicAttribute( magickind ); 
	//========================================================
	// 마나 검사 
	//========================================================
	
	// Mana가 충분해야 한다. 

	//SKILL LEVEL에 의한 MANA 소비량 
	SI32 kind = 0;
	if(IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique()))	kind = GetKind();
	CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	SI32			 skilltype			= pMagicAttributePtr->GetMagicSkillType( magickind );
	if( SKILL_SIKLLBOOK == skilltype && IsPC(GetID())  )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclCharSkillBookInfo->GetSkillBookMagic( GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return false;

		skilltype = pclSkillBookMagic->GetSkillUnique();	
	}



#ifdef _SAFE_MEMORY
	SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( skilltype, kind, pclCI->clIP.GetLevel(), &pclCI->clCharItem.clItem[0], GetID() );
#else
	SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( skilltype, kind, pclCI->clIP.GetLevel(), pclCI->clCharItem.clItem, GetID() );
#endif
	// Skill Level에 의한 영향력을 주고 싶으면 영향력을 넣어 마나소비량을 계산한다.
	SI32 mana		= pclClient->pclMagicManager->GetSpendMana(magickind, SkillLevel,GetCharUnique());
	
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
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		if(GetMana() < mana)
		{
			*preturn = CANUSEMAGIC_ERROR_NEEDMANA;
			return false;
		}
	}

	//========================================================
	// 마법 지연 
	//========================================================
	bool bcheckdelayswitch = false;
	if(pclClient->GameMode == GAMEMODE_SERVER)	
	{
		bcheckdelayswitch = true;
	}
	else
	{
		if(GetID() == 1)
		{
			bcheckdelayswitch = true;
		}
		if(GetSummonParentID() == 1)
		{
			bcheckdelayswitch = true;
		}
	}

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind		= 0;
	SI32 	siSubDelay 			= 0;
	SI32 	siSubMagicDistance	= 0;
	if( pclClient->IsCountrySwitch(Switch_SkillBook) && pclCharSkillBookInfo )
	{
		stMagicAttributeData*	psSubMagicData	= pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( magickind, siSubMagickind );
		if( psSubMagicData )
		{
			siSubDelay			= psSubMagicData->si32MagicDelay;
			siSubMagicDistance	= psSubMagicData->si32MagicDistance;
		}
	}

	if(bcheckdelayswitch == true)
	{
		SI32 siMagicDelay = psMagicData->si32MagicDelay - (psMagicData->si32MagicDelay * clPB.siMagicDecDelay / 100 );
		siMagicDelay += (siMagicDelay * siSubDelay) / 100;
		// 충분한 마법 지연 시간을 확보했는지 확인한다. 
		if( GetMagicDelay() < siMagicDelay )
		{
			*preturn = CANUSEMAGIC_ERROR_NEEDTIMEDELAY;
			return false;
		}
	}

	//========================================================
	// 사정거리 
	//========================================================
	SI32 siDisX = TABS(GetX()- pclCM->CR[targetid]->GetX());
	SI32 siDisY = TABS(GetY()- pclCM->CR[targetid]->GetY());
	
	SI32 siMagicDistance = psMagicData->si32MagicDistance;
	siMagicDistance += (siMagicDistance * siSubMagicDistance) / 100;
	// 마법 사정거리가 멀면 나가지 않는다.
	if( max(siDisX, siDisY) > siMagicDistance ) 
	{
		*preturn = CANUSEMAGIC_ERROR_FARDISTANCE;
		return false;
	}
	
	//========================================================
	// 
	//========================================================
	switch(magickind)
	{
	case MAGIC_MON_HEAL:
		{
			// 중독되어 있는 상태에서는 치료 불가. 
			if(clInnerStatus.IsStatus(INNERSTATUS_POISON) || clInnerStatus.IsStatus(INNERSTATUS_MAGICFIRE) || clInnerStatus.IsStatus(INNERSTATUS_POISON_SETITEM_EFFECT) )
			{
				return false;
			}

			// 손상 입은 캐릭터여야 한다. 
			if(pclCM->CR[targetid]->clPB.GetMaxLife() == pclCM->CR[targetid]->pclCI->clIP.GetLife())return false;

			// 소환수라면 pc에게만 가능하다. 선택되어서 수동으로 사용하는게 아니면 주인이랑 자기자신에게만 가능하다.
			if ( targetid == GetCharUnique() )
			{
			}
			else
			{
				SI32 siParentID = GetSummonParentID();
				if ( pclCM->IsValidID(siParentID) )
				{
					if(IsPC(pclCM->CR[targetid]->GetCharUnique()) == false )
					{
						return false ;
					}
					else
					{
						if ( pclCM->CR[siParentID]->siSelectedCharIndex != GetSummonIndex() &&
							siParentID != targetid )
						{
							return false ;
						}
					}
				}
			}

			return true;
			
		}
		break;
	default:
		// 자기 자신은 공격할 수 없다.
		if(targetid == siID)return false;
		
		if( PVP )
		{
			if( false == IsEnemyPCvsPC(targetid) )
				return false;	
		}
		else
		{
			if( false == IsEnemy(targetid) )
				return false;
		}
		break;

	}
	
	//========================================================
	// 무기 타입 
	//========================================================
	// 사용자 캐릭터인 경우에만 점검한다. 
	if(IsPC(GetCharUnique()))
	{	
		CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
		SI32 magictype = pMagicAttributePtr->GetMagicType(magickind);
		
		// 기술에 해당하는 무기타입을 알아낸다. 
		switch(magictype)
		{
		case MAGIC_TYPE_SWORD:	if(GetAttackType() != ATTACKTYPE_SWORD )	return false;	break;	
		case MAGIC_TYPE_SPEAR:	if(GetAttackType() != ATTACKTYPE_SPEAR)		return false;	break;	
		case MAGIC_TYPE_AXE:	if(GetAttackType() != ATTACKTYPE_AXE)		return false;	break;	
		case MAGIC_TYPE_STAFF:	if(GetAttackType() != ATTACKTYPE_STAFF	&&  GetAttackType() != ATTACKTYPE_STAFF1 &&  GetAttackType() != ATTACKTYPE_STAFF2 &&  GetAttackType() != ATTACKTYPE_NEW_STAFF2) return false;	break;	
		case MAGIC_TYPE_BOW:	if(GetAttackType() != ATTACKTYPE_BOW	&&  GetAttackType() != ATTACKTYPE_BOW1	 &&  GetAttackType() != ATTACKTYPE_NEW_BOW1)	return false;		break;	
		case MAGIC_TYPE_GUN:	if(GetAttackType() != ATTACKTYPE_GUN	&&  GetAttackType() != ATTACKTYPE_GUN1	 &&  GetAttackType() != ATTACKTYPE_NEW_GUN1)	return false;		break;	
		case MAGIC_TYPE_MON:	return false;	break;
		case MAGIC_TYPE_SOULGUARD:	break;	
		case MAGIC_TYPE_SKILL_BOOK:	break;	
		}		

		bool bcheckskillswitch = false;
		if(pclClient->GameMode == GAMEMODE_SERVER)	
		{
			bcheckskillswitch = true;
		}
		else
		{
			if(GetID() == 1)		// 자기 자신의 경우이거나. 
			{
				bcheckskillswitch = true;
			}
			if(GetSummonParentID() == 1)	// 자신의 소환수인 경우에, 
			{
				bcheckskillswitch = true;

			}
		}

		if(bcheckskillswitch == true)
		{
			// 마법 구현에 필요한 기술 수준을 보유하고 있는가.  
			SI32 siSkilltype = 0;

			// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
			siSkilltype = pMagicAttributePtr->GetMagicSkillType(magickind);
			if( SKILL_SIKLLBOOK == siSkilltype && IsPC(GetID())  )
			{
				cltSkillBookMagic*	pclSkillBookMagic = pclCharSkillBookInfo->GetSkillBookMagic( GetAttackType() );	
				if( NULL == pclSkillBookMagic ) return false;

				siSkilltype = pclSkillBookMagic->GetSkillUnique();	
			}
#ifdef _SAFE_MEMORY
			SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( siSkilltype, 0, pclCI->clIP.GetLevel(), &pclCI->clCharItem.clItem[0], GetID());
#else
			SI32 SkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( siSkilltype, 0, pclCI->clIP.GetLevel(), pclCI->clCharItem.clItem, GetID());
#endif
			if(magictype == MAGIC_TYPE_SOULGUARD)	return true;

			if ( SkillLevel < psMagicData->si16MagicSkillLevel )
			{
				*preturn = CANUSEMAGIC_ERROR_NOTENOUGHSKILL;
				return false;
			}
		}
	}
	

	return true;
}

bool cltCharCommon::CanMagicBuf( SI32 siMagicKind, SI32* psiReturn )
{
	if ( NULL == psiReturn )
	{
		return false;
	}

	// 우선 초기화
	*psiReturn = CANUSEMAGIC_ERROR_INVALID;
	
	// 기본적으로 사람일때만 버프 마법을 사용할 수 있다
	SI32 siMyCharID = GetCharUnique();
	if ( false == IsPC(siMyCharID) )
	{
		return false;
	}

	// 클라이언트일때는 자기자신만 검사해야 할때가 있다 그럴때를 위해서
	bool bCheckMode = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		bCheckMode = true;
	}
	// 클라이언트 일때는 자기 자신만 검사한다
	else if ( GetID() == 1 )
	{
		bCheckMode = true;
	}
	


	//========================================================
	// 기본 적으로 쓰야할 변수들
	//========================================================
	CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	if ( NULL == pMagicAttributePtr )
	{
		return false;
	}

	stMagicAttributeData* pMagicData = pclClient->pclMagicManager->GetMagicAttribute( siMagicKind );
	if ( NULL == pMagicData )
	{
		return false;
	}

	// 버프형 마법이 아니면 쓸 수 없다
	if ( MAGIC_CASTTYPE_BUF != pMagicData->si08MagicCastType )
	{
		return false;
	}

	// 해당 마법을 사용할 수 있는 슬롯을 검사한다
	if ( (true == bCheckMode) && (false == m_clMagicBufMgr.CheckMagicBufSlot(siMagicKind)) )
	{
		return false;
	}

	// 해당 마법의 재 사용시간 검사
	if ( (GAMEMODE_SERVER == pclClient->GameMode) && (false == m_clMagicBufMgr.CheckDelay(siMagicKind)) )
	{
		*psiReturn = CANUSEMAGIC_ERROR_NEEDTIMEDELAY;
		return false;
	}


	SI32 siMyKind		= 0;
	if ( IsNPC(GetCharUnique()) || IsInstanceNPC(GetCharUnique()) )
	{
		siMyKind = GetKind();
	}
	SI32 siMyLevel	 = pclCI->clIP.GetLevel();
	SI32 siSkillType = pMagicAttributePtr->GetMagicSkillType( siMagicKind );;
	
	// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
	if( SKILL_SIKLLBOOK == siSkillType && IsPC(GetID())  )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclCharSkillBookInfo->GetSkillBookMagic( GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return false;

		siSkillType = pclSkillBookMagic->GetSkillUnique();
	}

	//-----------------------------------------------------------------------------------------------------
	// 필요 마나량을 체크한다
	//-----------------------------------------------------------------------------------------------------
#ifdef _SAFE_MEMORY	
	SI32 SkillLevel	= pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, siMyKind, siMyLevel, &pclCI->clCharItem.clItem[0] );
#else	
	SI32 SkillLevel	= pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, siMyKind, siMyLevel, pclCI->clCharItem.clItem );
#endif
	SI32 siNeedMana	= pclClient->pclMagicManager->GetSpendMana( siMagicKind, SkillLevel, GetCharUnique() );

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind = 0;
	SI32	siSubNeedMana  = 0;
	if( pclClient->IsCountrySwitch(Switch_SkillBook) && pclCharSkillBookInfo )
	{
		stMagicAttributeData*	psSubMagicData	= pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( siMagicKind, siSubMagickind );
		if( psSubMagicData )
		{
			siSubNeedMana  = pclClient->pclMagicManager->GetSpendMana( siSubMagickind, SkillLevel, GetCharUnique() );
		}
	}

	if ( (GAMEMODE_SERVER == pclClient->GameMode) && (GetMana() < siNeedMana + siSubNeedMana) )
	{
		*psiReturn = CANUSEMAGIC_ERROR_NEEDMANA;
		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// 스킬 레벨을 체크한다
	//-----------------------------------------------------------------------------------------------------
	if ( true == bCheckMode )
	{
#ifdef _SAFE_MEMORY	
		SI32 siCheckLevel	= pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, 0, siMyLevel, &pclCI->clCharItem.clItem[0] );
#else	
		SI32 siCheckLevel	= pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, 0, siMyLevel, pclCI->clCharItem.clItem );
#endif	
		if ( siCheckLevel < pMagicData->si16MagicSkillLevel )
		{
			*psiReturn = CANUSEMAGIC_ERROR_NOTENOUGHSKILL;
			return false;
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// 마법 기술 타입에 해당하는 무기를 장착하지 않았다면
	//-----------------------------------------------------------------------------------------------------
	SI32 siMagicType	= pMagicAttributePtr->GetMagicType( siMagicKind );
	switch ( siMagicType )
	{
		case MAGIC_TYPE_SWORD:	if ( GetAttackType() != ATTACKTYPE_SWORD )	return false;	break;	
		case MAGIC_TYPE_SPEAR:	if ( GetAttackType() != ATTACKTYPE_SPEAR )	return false;	break;	
		case MAGIC_TYPE_AXE:	if ( GetAttackType() != ATTACKTYPE_AXE )	return false;	break;	
		case MAGIC_TYPE_STAFF:	if ( GetAttackType() != ATTACKTYPE_STAFF &&  GetAttackType() != ATTACKTYPE_STAFF1 &&  GetAttackType() != ATTACKTYPE_STAFF2 &&  GetAttackType() != ATTACKTYPE_NEW_STAFF2) return false;break;	
		case MAGIC_TYPE_BOW:	if ( GetAttackType() != ATTACKTYPE_BOW &&  GetAttackType() != ATTACKTYPE_BOW1 &&  GetAttackType() != ATTACKTYPE_NEW_BOW1)return false;		break;	
		case MAGIC_TYPE_GUN:	if ( GetAttackType() != ATTACKTYPE_GUN &&  GetAttackType() != ATTACKTYPE_GUN1 &&  GetAttackType() != ATTACKTYPE_NEW_GUN1)return false;		break;
		case MAGIC_TYPE_MON:	return false;	break;
		case MAGIC_TYPE_SOULGUARD:	break;
		case MAGIC_TYPE_SKILL_BOOK:	break;
			
	}		

	return true;

}


//------------------------------------------
// attack_id 적을 공격하게 만드는 함수 
//------------------------------------------
BOOL cltCharCommon::SetAttack(SI32 id, bool PVPHit)
{
	// 공격 목표가 이미 죽었거나 적이 아니면 공격하지 않는다. 
	if(pclCM->IsAlive(id)==FALSE)
		return FALSE;

	// 자기 자신은 공격할 수 없다. 
	if(id == siID)
		return FALSE;

	// PVP속성을 준다 pvp-
	//if( pclClient->IsWhereServiceArea(ConstServiceArea_English) )
	if(pclClient->siEnablePVP == 1)
	{
		if( PVPHit )
		{
			if( pclClient->pclMapManager->IsPkMap( GetMapIndex() ) )
			{
				clInnerStatus.clEnemyType.siEnemyType |= ENEMYTYPE_PVP;
			}
			if( pclClient->pclMapManager->IsPkMap( pclCM->CR[id]->GetMapIndex() ) )
			{
				pclCM->CR[id]->clInnerStatus.clEnemyType.siEnemyType |= ENEMYTYPE_PVP;
			}
		}
	}
	if (pclClient->siEnablePVP == 1) 
	{
		if (PVPHit) 
		{
			SI32 siEnemyId = pclCM->CR[id]->siEnemUnique ;
			if( siEnemyId > 0 )
			{
				clInnerStatus.clEnemyType.siEnemyType |= ENEMYTYPE_PVP;
			}
			if( siEnemyId > 0 )
			{
				pclCM->CR[id]->clInnerStatus.clEnemyType.siEnemyType |= ENEMYTYPE_PVP;
			}
		}
	}

	// 공격 목표를 설정한다. 
	cltCharPos clpos(pclCM->CR[id]->GetMapIndex(), pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY());
	cltAttackTarget cltarget(id, &clpos, 0,0, PVPHit);
	
	clAttackTarget.Set(&cltarget);

	// 얼마나 집요하게 추적할 지를 결정하는 변수. 
	dwAttackGiveUpDelay		= 5000; // 5초. 
	dwLastAttackClock		= pclClient->CurrentClock;

    return TRUE;
}


BOOL cltCharCommon::ReadyToFire()
{
	if(ChangeDirection(TargetDirection)==FALSE)
		return FALSE;	

	return TRUE;
}

BOOL cltCharCommon::Escape()
{
	SI32 attackid = clAttackTarget.GetAttackID();
	if( pclCM->IsValidID(attackid) == false )		return FALSE;

	// 적을 공격하기 위해 거리를 띄우러 이동한다.
	siSubOrder	=	CHARWORKSTEP_RETREAT;
	
	cltPos clpos1(pclCM->CR[attackid]->GetX(), pclCM->CR[attackid]->GetY());
	cltPos clpos2(GetX(), GetY());

	SHORT dir = DecideDirection8(&clpos1, &clpos2);

	SHORT tx  = GetX(), ty = GetY();
	
	if(dir&NORTH)
	{
		ty=max(0, GetY()-3);
	}
	if(dir&SOUTH)
	{
		ty=min(pclMap->GetYsize() - 1, GetY()+3);
	}
	if(dir&EAST)
	{
		tx=min(pclMap->GetXsize() - 1, GetX()+3);
	}
	if(dir&WEST)
	{
		tx=max(0, GetX()-3);
	}
	
	SetMove(tx, ty);
	
	return FALSE;
}

BOOL cltCharCommon::TryAttack()
{
			 
	// 목표의 방향을 구한다. 
	SI32 attackid	= clAttackTarget.GetAttackID();
	if( pclCM->IsValidID(attackid) == false )		return FALSE;

	TargetDirection = DecideDirection8(GetX(), GetY(), pclCM->CR[attackid]->GetX(), pclCM->CR[attackid]->GetY());


	// 버프 마법에 의한 공격 주기 감소
	DWORD dwSubAttackInterval = 0;
	if ( IsPC(GetCharUnique()) )
	{
		SI32 siCalcAttackInterval = m_clMagicBufMgr.GetIncAttackInterval( this );
		if ( siCalcAttackInterval )
		{
			dwSubAttackInterval = (clPB.dwAttackInterval/100) * siCalcAttackInterval;
			pclClient->pclLog->LogPrint( TEXT("공속증가 : %d\n"), dwSubAttackInterval );
		}
	}

	//---------------------------------------------------
	// 공격 주기를 고려한다. 
	//---------------------------------------------------
	if ( TABS(pclClient->CurrentClock - dwLastAttackIntervalClock) < (int)(clPB.dwAttackInterval-dwSubAttackInterval) )
	{
		bBusySwitch = false;
		return TRUE;
	}
	dwLastAttackIntervalClock = pclClient->CurrentClock;
	
	
	// 다른 준비가 되었는지 확인한다. 
	if( ReadyToFire() == FALSE )	return TRUE;
	 
	siSubOrder	=	CHARWORKSTEP_ACTION;

	clAniInfo.siAniStep = 0;		// 애니메이션을 초기화해준다. 
	clAniInfo.dwAniStartClock	= pclClient->CurrentClock;

	// 적절한 공격 동작을 찾는다. 
/*	SI32 anilist[10];
	SI32 anilistindex = 0;

	SI32 attacktype = 0;
	for(SI32 i = 0;i < 3;i++)
	{
		if		(i == 0)attacktype = ANITYPE_ATTACK;
		else if	(i == 1)attacktype = ANITYPE_ATTACK1;
		else if	(i == 2)attacktype = ANITYPE_ATTACK2;
		else
		{
			MsgBox(TEXT("vvce9jf"), TEXT("Fdsf3d"));
		}

		SI32 aniref	= pclKI->clCharAniManager.FindRefFromAnitype(attacktype);
		if(aniref >= 0)
		{
			anilist[anilistindex] = attacktype;
			anilistindex++;
		}
	}

	if(anilistindex)
	{
		siCurrentAttackType = anilist[rand() % anilistindex];
	}
*/
	// by LEEKH 2007.12.28 - TRANSFORM
	if ( GetKindInfo(true)->siMaxAttackType )
		siCurrentAttackType = GetKindInfo(true)->siAttackList[rand() % GetKindInfo()->siMaxAttackType];

	if ( !siCurrentAttackType ) siCurrentAttackType = ANITYPE_ATTACK ;

	SetAttack(attackid, clAttackTarget.bPVPHit);
	
	
	
	return FALSE;
}



//-----------------------------
// 캐릭터 전투 
//-----------------------------
SI32 cltCharCommon::Action_Attack()
{
	SHORT status;
	SHORT direction=0;
	SI32 six = LOWORD(clCurOrder.siPara1);
	SI32 siy = HIWORD(clCurOrder.siPara1);

	/*   if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
	{
	}
	*/

	SI32 attackid = clAttackTarget.GetAttackID();

	// 버프 마법에 의한 사정거리 증가
	SI32 siAddAttackRange = GetAttackRange();
	if ( IsPC(GetCharUnique()) )
	{
		siAddAttackRange += m_clMagicBufMgr.GetIncRange( this );
	}	
	if(siSubOrder == CHARWORKSTEP_MOVE)
	{
#ifdef _DEBUG
		// 근거리 무기들은 찜을 하도록 한다.
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			if ( IsPC(GetCharUnique()) && IsNPC(attackid) )
			{
				cltServer* pclserver = (cltServer*)pclClient ;

				if ( clPB.GetAttackRange() == 1 && pclserver->pclCM->IsAlive(attackid) )
				{
					if( IsNPC(pclserver->pclCM->CR[attackid]->GetCharUnique()) == true &&
						(pclserver->pclCM->CR[attackid]->pclKI == NULL || pclserver->pclCM->CR[attackid]->pclKI->IsAtb((ATB_NOJJIM)) == false) &&
						pclserver->pclCM->CR[attackid]->clIdentity.siIdentity == 0)
					{
						// 찜당했음을 설정한다. 
						if(pclserver->pclCM->CR[attackid]->clJim.SetBeJimed(GetCharUnique(),
																			pclserver->GetFrame()) == true)
						{
							// 이미 다른 찜한 캐릭터가 있으면 찜을 해제한다. 
							if(clJim.siJimSlaveCharUnique)
							{
								// 이미 찜한 다른 캐릭터가 내가 아니어야...
								if(clJim.siJimSlaveCharUnique != pclserver->pclCM->CR[attackid]->GetCharUnique())
								{
									SI32 id = pclCM->GetIDFromCharUnique(clJim.siJimSlaveCharUnique);
									if ( pclserver->pclCM->IsAlive(id) )
									{
										pclserver->pclCM->CR[id]->clJim.Init();

										cltGameMsgResponse_Jim clinfo(clJim.siJimSlaveCharUnique, 0);
										cltMsg clMsg(GAMEMSG_RESPONSE_JIM, sizeof(clinfo), (BYTE*)&clinfo);
										((cltCharServer*)pclserver->pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);

									}

									clJim.siJimSlaveCharUnique = 0;
								}

							}

							// 찜했음을 설정한다. 
							clJim.SetJim(pclserver->pclCM->CR[attackid]->GetCharUnique());

							cltGameMsgResponse_Jim clinfo(pclserver->pclCM->CR[attackid]->GetCharUnique(), GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_JIM, sizeof(clinfo), (BYTE*)&clinfo);
							((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
						}
					}
				}
			}
		}
#endif

		status = Move();
		
		if ( MoveDone() )
		{
			bBusySwitch = false;
		}
		else
		{
			bBusySwitch = true;
		}

		if(bBusySwitch == false)
		{
			// 적을 공격할 수 없는 처지라면, 
			if(CanAttack(&clAttackTarget)==FALSE)
			{  
				return -1;
			}

			if ( IsInRange(attackid, siAddAttackRange) )
			{
				siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;
			}

			// 목적지에 도착하거나 중간 지점에 도착하면, 
			if(status > 0)
			{
				siSubOrder	 =	CHARWORKSTEP_DECIDELENGTH;
			}
		}
   }

   if(siSubOrder == CHARWORKSTEP_DECIDELENGTH)
   {
		  //----------------------------------------------------------
		  // NPC인 경우 일정 시간 추적한 다음엔 공격을 포기해야 한다. 
		  //----------------------------------------------------------
		  if(pclClient->GameMode == GAMEMODE_SERVER)
		  {
			  // 라쿤 피하기 이벤트에 참여 중이면 공격을 포기하면안된다.
			  if(IsNPC(GetCharUnique()) && pclCM->IsValidID(attackid) && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON && clIdentity.siIdentity != IDENTITY_RACOONDODGE_EVENT )
			  {
				  // 일정 시간 이상 추적했으면 포기. 
				  if(TABS(pclClient->CurrentClock - dwLastAttackClock) > (int)dwAttackGiveUpDelay)
				  {
					  bBusySwitch = false;
					  return -2;
				  }

				  // 악취탕 복용 상태면 일정 확률로 공격 포기. 
				  if(pclCM->CR[attackid]->bNoFirstAttackSwitch && rand() % 10 == 0)
				  {
					  bBusySwitch = false;
					  return -2;
				  }

				  // 프리미엄 개인 상점 개설중이라면 공격 포기.
				  if(pclCM->CR[attackid]->bPersonalShopSwitch == true 
					  && pclCM->CR[attackid]->bPremiumPersonalShop == true )
				  {
					  bBusySwitch = false;
					  return -2;
				  }
				  // 프리미엄 선공 방지약 버프 중이라면 일정 확률로 공격 포기.
				  if(pclCM->CR[attackid]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK) && rand() % 10 == 0 )
				  {
					  bBusySwitch = false;
					  return -2;					
				  }

				  // 히나모찌를 먹어서 선공 방지효과 중이라면 일정 확률로 공격 포기.
				  if(pclCM->CR[attackid]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI) && rand() % 10 == 0 )
				  {
					  bBusySwitch = false;
					  return -2;					
				  }
				  // 변신 버프 상태(야시장 음식 제외)이면 일정 확률로 공격 포기 
				  if(pclCM->CR[attackid]->pclCI->clBI.clBufInfo.IsHaveBufExecptGetType(BUF_TRANSFORM, BUF_GET_TYPE_NIGHTMARKET) && rand() % 10 == 0 )
				  {
					  bBusySwitch = false;
					  return -2;					
				  }

			  }
		  }
		  
		  // 적을 공격할 수 없는 상태라면 중단한다.
		  if(CanAttack(&clAttackTarget)==FALSE)
		  {   
			  bBusySwitch	=	false;
			  return -1;
		  }

		  //------------------------------------
		  // 마법 공격을 가할 수 있다면 
		  //------------------------------------
		  if(MagicInAttackAction() == true)
		  {
			  return 1;
		  }

		  // 적이 사정거리 안에 있으면,
		  if ( IsInRange(attackid, siAddAttackRange) )
		  { 
			  // 공격 시도 했으므로 업데이트. 
			  dwLastAttackClock = pclClient->CurrentClock;

			  // 공격을 시도한다. 
			  if(TryAttack()==TRUE)
			  {
				  return 1;
			  }
			  
		  }
		  else // 적이 사거리 밖에 있으면, 
		  {  
			  // 공격가능하고 이동 가능 하다면, 
			  if(CanAttack(&clAttackTarget) && pclCM->IsValidID(attackid) && IsAtb(ATB_MOVE)==TRUE)
			  {
				  // 적이 있는 곳으로 이동한다. 
				  SetMove(pclCM->CR[attackid]->GetX(), pclCM->CR[attackid]->GetY());

				  siSubOrder = CHARWORKSTEP_MOVE;
			  }
			  else
			  {
				  bBusySwitch = false;
				  return -1;
			  }
		  }
	   
   }
   if(siSubOrder == CHARWORKSTEP_RETREAT)
   {
	    status = Move();
		if(MoveDone())bBusySwitch = false;
		else
		{
			bBusySwitch = true;
		}

		if(bBusySwitch == false)
		{
			
			// 적을 공격할 수 없는 처지라면, 
			if(CanAttack(&clAttackTarget)==FALSE)
			{  
				return -1;
			}

			if ( IsInRange(attackid, siAddAttackRange) )
			{
				if(IsInMinRange(attackid, 1)==FALSE)
				{
					siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;
				}
			}
			
			// 목적지에 도착하거나 중간 지점에 도착하면, 
			if(status > 0)
		   {
		      siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;
		   }

		}
   }

	if(siSubOrder == CHARWORKSTEP_ACTION)
	{
		bBusySwitch = true;

		// 타격이 1회 완료되면, 다시 목표와의 거리를 측정한다. 
		if(Fire() == true)
		{
			siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;
			bBusySwitch	=	false;

			//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
			//이곳에서 결정체의 내구도를 깍는다 - 공격시, 무기.
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[GetID()];
				if ( pclchar )
				{
					if ( IDENTITY_PERSON_SUMMON == pclchar->clIdentity.siIdentity )
					{
						pclchar->DecreaseCrystalDurACForSummon(CRYSTAL_DECREASE_DURABILITY_ACNUM);
					}
					else
					{
						pclchar->DecreaseCrystalDurAC(CRYSTAL_DECREASE_DURABILITY_ACNUM);
					}

				}
			}
		}
	}

	return 1;
}


bool cltCharCommon::FireSub()
{
    int id=0;
	int x=0, y=0;
	SI32 apower = 0;
	SI32 hitrate = 0;
	int attackstartx, attackstarty;
	bool bfailswitch = false;

	SI32 attackid = clAttackTarget.GetAttackID();

	//------------------------------------------
	// 명중률 계산. 
	//------------------------------------------
	if(pclCM->IsAlive(attackid) == TRUE)
	{
		SI32 myID = GetID();

		//// GOD 모드면 명중율 100%
		//if ( pclCM->CR[myID]->bGodMode == true ) 
		//{
		//	clAttackTarget.siHitRate = clPB.GetHitRate( pclCM->CR[attackid]) * 100;
		//}GOD 모드면 명중율 100%
		//else
		//{
			clAttackTarget.siHitRate = clPB.GetHitRate( pclCM->CR[attackid], this );
		//}
		
		if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
		{
			clAttackTarget.siDodgeRate = pclCM->CR[attackid]->clPB.GetDodgeRate( pclCM->CR[myID], pclCM->CR[attackid] );
		}
		else
			clAttackTarget.siDodgeRate = 0;
	}
	else
	{
		clAttackTarget.siHitRate = 0;
		clAttackTarget.siDodgeRate = 0;
	}

	// 버프 마법에 의한 명중률 상승
	if ( IsPC(GetCharUnique()) )
	{
		SI32 siAddHitRate = m_clMagicBufMgr.GetIncHitRate( this );
		if ( siAddHitRate )
		{
			SI32 siCalcHitRate = (clAttackTarget.siHitRate / 100) * siAddHitRate;
			clAttackTarget.siHitRate += siCalcHitRate;
			pclClient->pclLog->LogPrint( TEXT("명중률증가 : %d\n"), siCalcHitRate );
		}
	}

	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		//[진성] 세트 아이템 으로 인한 명중률 변경.
		if( m_siSetItemType )
		{
			SI32 siCalcHitRate = 0;
			siCalcHitRate += (clAttackTarget.siHitRate / 100) * pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( this, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_1 );
			siCalcHitRate += (clAttackTarget.siHitRate / 100) * pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( this, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_2 );
			siCalcHitRate += (clAttackTarget.siHitRate / 100) * pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( this, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_3 );
			siCalcHitRate += (clAttackTarget.siHitRate / 100) * pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( this, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_4 );
			siCalcHitRate += (clAttackTarget.siHitRate / 100) * pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( this, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_5 );
			
			clAttackTarget.siHitRate += siCalcHitRate;
		}
	}



	if( GetAttackType() == ATTACKTYPE_SWORD
	||	GetAttackType() == ATTACKTYPE_SPEAR
	||	GetAttackType() == ATTACKTYPE_AXE
	||	GetAttackType() == ATTACKTYPE_STAFF
	||	GetAttackType() == ATTACKTYPE_HAND
	||  GetAttackType() == ATTACKTYPE_HEROSWORD )
	{
		if(clAniInfo.IsInstant()  && bHitDoneSwitch == false)
		{
			bHitDoneSwitch = true;

			if((pclKI != NULL && GetKindInfo()->IsAtb(ATB_HERO)) == TRUE )
			{
				
				switch(GetAttackType())
				{
				case ATTACKTYPE_SWORD:
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WEAPON_SWORD_FIRE")), GetX(), GetY());
					break;
				case ATTACKTYPE_SPEAR:
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WEAPON_SPEAR_FIRE")), GetX(), GetY());
					break;
				case ATTACKTYPE_AXE:
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WEAPON_AXE_FIRE")), GetX(), GetY());
					break;
				case ATTACKTYPE_STAFF:
				case ATTACKTYPE_STAFF1:
				case ATTACKTYPE_STAFF2:
				case ATTACKTYPE_NEW_STAFF2:
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAGIC_STAFF_FIRE")), GetX(), GetY());
					break;
				case ATTACKTYPE_HAND:
				case ATTACKTYPE_BOW:
				case ATTACKTYPE_BOW1:
				case ATTACKTYPE_NEW_BOW1:
				case ATTACKTYPE_GUN:
				case ATTACKTYPE_GUN1:
				case ATTACKTYPE_NEW_GUN1:
					break;
				default:
					break;
				}
			}
			// 공격 효과음. 
			if( siCurrentAttackType == ANITYPE_ATTACK )
			{
				if(GetKindInfo()->clKIEffectInfo.siAttackEffect)
				{
					pclClient->PushEffect(GetKindInfo()->clKIEffectInfo.siAttackEffect, GetX(), GetY());
				}
			}
			else
			{
				if(GetKindInfo()->clKIEffectInfo.siAttackEffect1 > 0 )
				{
					pclClient->PushEffect(GetKindInfo()->clKIEffectInfo.siAttackEffect1, GetX(), GetY());
				}
				else if(GetKindInfo()->clKIEffectInfo.siAttackEffect)
				{
					pclClient->PushEffect(GetKindInfo()->clKIEffectInfo.siAttackEffect, GetX(), GetY());
				}
			}

		/*	if( ConstServiceArea_China == pclClient->siServiceArea ||
				ConstServiceArea_English == pclClient->siServiceArea )
			{
				cltGameMsgRequest_Hit clHit( pclCM->CR[siID]->GetCharUnique(), &clAttackTarget );
				clHit.clAttackTarget.SetAttackID( pclCM->CR[attackid]->GetCharUnique() );

				cltMsg clMsg( GAMEMSG_REQUEST_HIT, sizeof(clHit), (BYTE*)&clHit );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else*/
			{
				if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					cltAttackDamageInfo cldamageinfo(GetAttackType(), GetApower(), 0);

					SI32 siAddAttackRange = GetAttackRange();
					if ( IsPC( GetCharUnique() ) )
					{
						siAddAttackRange += m_clMagicBufMgr.GetIncRange( this );
					}

					((cltServer*)pclClient)->HitOperation(siID, &cldamageinfo, siAddAttackRange, &clAttackTarget, pclMap);
				}
			}
		}
	}
	else if(GetAttackType() == ATTACKTYPE_BOW  
		|| GetAttackType() == ATTACKTYPE_GUN
		|| GetAttackType() == ATTACKTYPE_CAKE
		|| GetAttackType() == ATTACKTYPE_EGG
		|| GetAttackType() == ATTACKTYPE_CANNON
		|| GetAttackType() == ATTACKTYPE_CASTLECANNON
		|| GetAttackType() == ATTACKTYPE_SHELL
		|| GetAttackType() == ATTACKTYPE_CATFISH
		|| GetAttackType() == ATTACKTYPE_LADYBUGFAN
		|| GetAttackType() == ATTACKTYPE_SNOW
		|| GetAttackType() == ATTACKTYPE_STAFF1
		|| GetAttackType() == ATTACKTYPE_MADOGHOSTFIRE
		|| GetAttackType() == ATTACKTYPE_JAPANHEADER
		|| GetAttackType() == ATTACKTYPE_TOHANG
		|| GetAttackType() == ATTACKTYPE_SKULLBATFIRE		
		|| GetAttackType() == ATTACKTYPE_FIREGOBLIN
		|| GetAttackType() == ATTACKTYPE_POISON
		|| GetAttackType() == ATTACKTYPE_ICE
		|| GetAttackType() == ATTACKTYPE_KNIFE
		|| GetAttackType() == ATTACKTYPE_GREENFIRE
		|| GetAttackType() == ATTACKTYPE_BLACKINK
		|| GetAttackType() == ATTACKTYPE_BOOMERANG
		|| GetAttackType() == ATTACKTYPE_WINDBLADE
		|| GetAttackType() == ATTACKTYPE_SHELLFISHBOMB
		|| GetAttackType() == ATTACKTYPE_TREEMON
		|| GetAttackType() == ATTACKTYPE_VINE
		|| GetAttackType() == ATTACKTYPE_GHOSTFIRE
		|| GetAttackType() == ATTACKTYPE_SIMTORYONG
		|| GetAttackType() == ATTACKTYPE_KINGCUTTLEFISH
		|| GetAttackType() == ATTACKTYPE_CRANE
		|| GetAttackType() == ATTACKTYPE_MONKEY
		|| GetAttackType() == ATTACKTYPE_CURSEDEFENSE
		|| GetAttackType() == ATTACKTYPE_CURSEMOVESPEED
		|| GetAttackType() == ATTACKTYPE_BUTCHER
		|| GetAttackType() == ATTACKTYPE_STONEOLDMAN
		|| GetAttackType() == ATTACKTYPE_KINGCRAB
		|| GetAttackType() == ATTACKTYPE_FIREARROW
		|| GetAttackType() == ATTACKTYPE_LOBSTER
		|| GetAttackType() == ATTACKTYPE_DEADMESSENGER
		|| GetAttackType() == ATTACKTYPE_MASKMONK
		|| GetAttackType() == ATTACKTYPE_PERSONCANNON
		|| GetAttackType() == ATTACKTYPE_CANNON_MAGIC
		|| GetAttackType() == ATTACKTYPE_HEROSWORD
		|| GetAttackType() == ATTACKTYPE_BOW1
		|| GetAttackType() == ATTACKTYPE_GUN1
		|| GetAttackType() == ATTACKTYPE_STAFF2
		|| GetAttackType() == ATTACKTYPE_FOAM
		|| GetAttackType() == ATTACKTYPE_FIREBALL
		|| GetAttackType() == ATTACKTYPE_MORTAL
		|| GetAttackType() == ATTACKTYPE_WATERARROW
		|| GetAttackType() == ATTACKTYPE_STAFFWIND
		|| GetAttackType() == ATTACKTYPE_FLYVINE
		|| GetAttackType() == ATTACKTYPE_MAGICFIREBURN
		|| GetAttackType() == ATTACKTYPE_SKELLFISH
		|| GetAttackType() == ATTACKTYPE_FROZEN
		|| GetAttackType() == ATTACKTYPE_ICESTONE
		|| GetAttackType() == ATTACKTYPE_NEW_BOW1
		|| GetAttackType() == ATTACKTYPE_NEW_GUN1
		|| GetAttackType() == ATTACKTYPE_NEW_STAFF2
		|| GetAttackType() == ATTACKTYPE_ENERGYARROW
		|| GetAttackType() == ATTACKTYPE_SPEAR_MANA
		|| GetAttackType() == ATTACKTYPE_BLACKBOSS
		|| GetAttackType() == ATTACKTYPE_OCTOPUS
		|| GetAttackType() == ATTACKTYPE_OTOHIME
		|| GetAttackType() == ATTACKTYPE_BLACKSPIDER
		|| GetAttackType() == ATTACKTYPE_EVILEYE
		|| GetAttackType() == ATTACKTYPE_FROZENSORCERER
		|| GetAttackType() == ATTACKTYPE_CANNONMOL
		|| GetAttackType() == ATTACKTYPE_PRIEST
		|| GetAttackType() == ATTACKTYPE_MINEGIRL
		|| GetAttackType() == ATTACKTYPE_GREENTURTLE
		|| GetAttackType() == ATTACKTYPE_MAGICALRACCOON
		|| GetAttackType() == ATTACKTYPE_MAMARACCOON
		|| GetAttackType() == ATTACKTYPE_RACCOONTOWER
		|| GetAttackType() == ATTACKTYPE_CHICKENBIG
		|| GetAttackType() == ATTACKTYPE_MOONBLADE
		|| GetAttackType() == ATTACKTYPE_EMPERORARCHER
		|| GetAttackType() == ATTACKTYPE_NINJAMAN
		|| GetAttackType() == ATTACKTYPE_PIRATEARCHER
		|| GetAttackType() == ATTACKTYPE_EMPERORMASTER
		|| GetAttackType() == ATTACKTYPE_GREATWALLSOLDIER
		|| GetAttackType() == ATTACKTYPE_GREATWALLMAGICIAN
		|| GetAttackType() == ATTACKTYPE_CATRANGE
		|| GetAttackType() == ATTACKTYPE_GREATWALLSPIDER
		|| GetAttackType() == ATTACKTYPE_BEARTANG
		|| GetAttackType() == ATTACKTYPE_SUPERBEARTANG
		|| GetAttackType() == ATTACKTYPE_HALLOWEENGHOST
		|| GetAttackType() == ATTACKTYPE_CHRISTMASRANGE
		|| GetAttackType() == ATTACKTYPE_GREATWALLASURA
		|| GetAttackType() == ATTACKTYPE_FIGHTINGMENRANGE	// 거너스피릿
		|| GetAttackType() == ATTACKTYPE_FROZENQUEEN
		|| GetAttackType() == ATTACKTYPE_FAIRYRANGE		// 요정스피릿(여)
		|| GetAttackType() == ATTACKTYPE_SNOWQUEEN		// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
		|| GetAttackType() == ATTACKTYPE_CLOWN
		)	
		
	{
		if(clAniInfo.IsInstant() && bHitDoneSwitch == false)
		{
			bHitDoneSwitch = true;

			if(pclCM->IsValidID(attackid) )
			{
				
				// by LEEKH 2007.12.28 - TRANSFORM
				// 공격 효과음. 
				if(GetKindInfo()->clKIEffectInfo.siAttackEffect)
				{
					pclClient->PushEffect(GetKindInfo()->clKIEffectInfo.siAttackEffect, GetX(), GetY());
				}

				DecideAttackPoint(0, attackstartx, attackstarty);
				
				cltPos cltilestartpos(GetX(), GetY());
				cltPos cldotstartpos(attackstartx, attackstarty);
				
				cltPos cldotdestpos;
				pclCM->CR[attackid]->GetHitPoint(&cldotdestpos);
				
				SI32 bulletunique = pclClient->pclAttackTypeInfoManager->GetBulletUnique(GetAttackType());
				// by LEEKH 2007.12.24 - TRANSFORM - 블랫은 무조건 변신 캐릭터를 따른다.
				if( clTransFormInfo.bTransFormMode == true )
				{
					bulletunique = pclClient->pclAttackTypeInfoManager->GetBulletUnique(GetKindInfo()->clDefaultWeaponInfo.GetAttackType());
				}
				if(bulletunique)
				{			
					SI32 apower = GetApower();

					// 지팡이 마법이면 마법력을 소모한다. 
					if(GetAttackType() == ATTACKTYPE_STAFF1 || GetAttackType() == ATTACKTYPE_STAFF2 || GetAttackType() == ATTACKTYPE_NEW_STAFF2)
					{
						if ( pclKI->bSummonMall == FALSE )
						{
							if(pclCI->clIP.GetMana() >= 2)
							{
								pclCI->clIP.DecreaseMana(2);

								// 클라이언트로 알려준다. 
								SetUpdateSwitch(UPDATE_IP, true, 0);
							}
							else
							{
								bfailswitch = true;
							}
						}
					}

					// 서버인 경우
					if (pclClient->GameMode == GAMEMODE_SERVER)
					{
						cltCharServer* pclchar = NULL;
						SI16 siBulletUnique = 0;
						SI32 pos = 0;
						SI16 siUseType = 0;		// 1:캐릭터, 2:소환수
						BOOL bUseBullet = FALSE;

						// 유저인 경우
						if ( IsPC(siID) )
						{
							pclchar = (cltCharServer*)this;
							if ( NULL == pclchar )			return false;

							bUseBullet = GetBulletUnique_Char(&siBulletUnique);
							if(siBulletUnique > 0)		pos = pclCI->clCharItem.GetItemPosInEquip(siBulletUnique);
							siUseType = 1;
						}
					
						// 소환수인 경우
						if ( clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
						{
							SI32 siParentID = GetSummonParentID();
							if ( false == pclCM->IsValidID(siParentID) )	return false;

							pclchar = (cltCharServer*)pclCM->CR[siParentID];
							if ( NULL == pclchar )							return false;

							// 소환수인경우 장비 장착 가능한 소환수만 총알이 소모된다
							cltKindInfo* pclki = NULL;
							if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
							{
								pclki = pclClient->pclKindInfoSet->pclSummonKI[GetKind()];
							}
							else
							{
								pclki = pclClient->pclKindInfoSet->pclKI[GetKind()];
							}

							if ( (pclki) && (pclki->siSummonEquip) )
							{
								bUseBullet = GetBulletUnique_Summon(GetSummonIndex(), &siBulletUnique);
								if(siBulletUnique > 0)		pos = pclchar->pclCI->clSummonInfo.GetItemPosInEquip(GetSummonIndex(), siBulletUnique);
								siUseType = 2;
							}
						}

						if(bUseBullet)
						{
							if(siBulletUnique == ITEMUNIQUE(13601) || siBulletUnique == ITEMUNIQUE(13600))
							{
								//기간제 아이템일 경우에는 사용하지 않도록. [성웅]
							}
							else
							{
								if (pos < 0 && pos >= MAX_SHAPE_ITEM)		return false;

								switch(siUseType)
								{
								case 1:		// 캐릭터
									{
										cltItem clitem;
										clitem.Set( &pclCI->clCharItem.clItem[pos]);
										clitem.SetItemNum(1);

										if( pclchar->UseItem(clitem) )
										{
											while( pclchar->clBulletUseReserveInfo.Push( clitem.siUnique, -clitem.siItemNum, pos) == false )
											{
												pclchar->BulletUseReserveAction();
											}
											cltItem *pclFrom = &pclCI->clCharItem.clItem[ pos ];
											if( pclFrom->siItemNum == 1 )		{	pclchar->BulletUseReserveAction();	}

											pclFrom->Decrease(1);

											// 클라이언트로 사용후 정보를 보낸다. 
											cltGameMsgResponse_PersonItemInfo clinfo( pos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
											cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
											SendNetMsg((sPacketHeader*)&clMsg);
										}
									}
									break;

								case 2:		// 소환수
									{
										cltItem* pclItem = pclchar->pclCI->clSummonInfo.GetSummonEquipItem(GetSummonIndex(), pos);
										if( pclItem )
										{
											cltItem clitem;
											clitem.Set( pclItem );
											clitem.SetItemNum(1);

											if( pclchar->UseItem(clitem) )
											{
												while( pclchar->clSummonBulletUseReserveInfo.Push( GetSummonIndex(), clitem.siUnique, -clitem.siItemNum, pos) == false )
												{
													pclchar->BulletUseReserveAction();
												}
												cltItem *pclFrom = pclItem;
												if( pclFrom->siItemNum == 1 )		{	pclchar->BulletUseReserveAction();	}

												pclFrom->Decrease(1);

												// 클라이언트로 사용후 정보를 보낸다. 
												cltGameMsgResponse_SummonChangeItem clinfo( GetSummonIndex(), pos, pclFrom );
												cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONCHANGEITEM, sizeof(clinfo), (BYTE*)&clinfo);
												pclchar->SendNetMsg((sPacketHeader*)&clMsg);
												
											}
										}
									}
									break;
								}
							}
						}
					}
					
					if(bfailswitch == false)
					{
						

						// 사용자인 경우에만. 
						if(IsPC(GetCharUnique()))
						{
							// 최소 거리에서 공격 불가능한 캐릭터인 경우에는 
							if(pclClient->pclAttackTypeInfoManager->IsAtb(GetAttackType(), ATTACKTYPE_ATB_MINRANGE) && IsInMinRange(attackid, 1)==TRUE)
							{
								apower = max(1, apower * 7 / 10);
							}
						}

						// 캐릭터와 공격대상 지점이 같은 지도여야 한다. 
						if(GetMapIndex() == clAttackTarget.clPos.GetMapIndex()
						&& pclMap->IsInMapArea(clAttackTarget.clPos.siX, clAttackTarget.clPos.siY) == true)
						{
							// [영훈] 소환수는 기본 공격 블렛으로만 나갈수 있도록
							if ( pclClient->pclCM->IsValidID(GetSummonParentID( )) ) 
							{
								pclClient->pclLog->LogPrint( TEXT("어택타입 : %d\n"), GetAttackType() );
								bulletunique = pclClient->pclAttackTypeInfoManager->GetBulletUnique(GetKindInfo()->clDefaultWeaponInfo.GetAttackType());
							}

							{
								id = pclClient->pclBulletManager->SetBullet(pclMap, bulletunique, 0,  
																			siID, apower, 
																			&cltilestartpos,
																			&cldotstartpos, 
																			&clAttackTarget,
																			&cldotdestpos);
							}
						}
					}
					
				}
			}

		}
	}
	// 자폭 
	else if( GetAttackType() == ATTACKTYPE_SELFBOMB)
	{
		if(clAniInfo.IsInstant() == false || bHitDoneSwitch == true)return false;
		bHitDoneSwitch = true;

		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			((cltCharServer*)this)->SelfBombAction();
		}
	}
	else
	{
		//cyj MsgBox(TEXT("CharAttack"), TEXT("kind:%s atb:%d"), pclKI->GetName(), GetAttackType());
	}

	return true;
}

bool cltCharCommon::Fire()
{
	SetAnimation(siCurrentAttackType);
	if(DoAnimationDelay() == FALSE)return false;

	// 타격지점과 캐릭터의 위치가 같은 지도상에 있어야 한다. 
	if(GetMapIndex() == clAttackTarget.clPos.GetMapIndex())
	{
		FireSub();	// 실질 타격 함수. 
	}
	
	if(clAniInfo.GetAniStep() == 0 )//&& clAniInfo.siRetryNum > 0 )
	{
		bHitDoneSwitch	= false;
		bBusySwitch		= false;
		
		return true;
	}
	else return false;
	
}

BOOL cltCharCommon::GetBulletUnique_Char(SI16* psiBulletUnique)
{
	*psiBulletUnique = 0;

	if(GetAttackType() == ATTACKTYPE_BOW || GetAttackType() == ATTACKTYPE_BOW1 || GetAttackType() == ATTACKTYPE_NEW_BOW1)
	{
		// 일반 화살
		if( pclCI->clCharItem.GetItemNumInEquip(ITEMUNIQUE(7850)) > 0 )		*psiBulletUnique = ITEMUNIQUE(7850);
		// 과금 화살
		if( pclCI->clCharItem.GetItemNumInEquip(ITEMUNIQUE(13600)) > 0 )	*psiBulletUnique = ITEMUNIQUE(13600);

		return TRUE;
	}

	if(GetAttackType() == ATTACKTYPE_GUN || GetAttackType() == ATTACKTYPE_GUN1 || GetAttackType() == ATTACKTYPE_NEW_GUN1)
	{
		// 일반 총알
		if( pclCI->clCharItem.GetItemNumInEquip(ITEMUNIQUE(7855)) > 0 )		*psiBulletUnique = ITEMUNIQUE(7855);
		// 과금 총알
		if( pclCI->clCharItem.GetItemNumInEquip(ITEMUNIQUE(13601)) > 0)		*psiBulletUnique = ITEMUNIQUE(13601);

		return TRUE;
	}

	return FALSE;
}

BOOL cltCharCommon::GetBulletUnique_Summon(SI32 siSummonIndex, SI16* psiBulletUnique)
{
	*psiBulletUnique = 0;

	/*
	cltSummon* pclSummon = pclCI->clSummonInfo.GetSummon(siSummonIndex);
	if( pclSummon == NULL )			return FALSE;
	*/
	
	SI32 siParentID = GetSummonParentID();
	if ( false == pclCM->IsValidID( siParentID ) )
	{
		return FALSE;
	}
	
	cltCharCommon* pclParentChar = (cltCharCommon*)pclCM->CR[siParentID];
	if ( NULL == pclParentChar )
	{
		return FALSE;
	}

	cltSummonInfo* pclSummonInfo = &pclParentChar->pclCI->clSummonInfo;
	if( pclSummonInfo == NULL )		return FALSE;

	
	//SI32 siAttackType = pclSummon->siAttackType;
	SI32 siAttackType = GetAttackType();

	if(siAttackType == ATTACKTYPE_BOW || siAttackType == ATTACKTYPE_BOW1 || siAttackType == ATTACKTYPE_NEW_BOW1)
	{
		// 일반 화살
		if( pclSummonInfo->GetHaveItemNumFromItemUnique(siSummonIndex, ITEMUNIQUE(7850)) > 0 )
		{
			*psiBulletUnique = ITEMUNIQUE(7850);
			return TRUE;
		}

		// 과금 화살
		if( pclSummonInfo->GetHaveItemNumFromItemUnique(siSummonIndex, ITEMUNIQUE(13600)) > 0 )
		{
			SI32 siBulletPosition = -1;
			siBulletPosition = pclSummonInfo->GetItemPosInEquip(siSummonIndex, ITEMUNIQUE(13600));
			if ( 0 <= siBulletPosition )
			{
				cltItem* pBullet = pclSummonInfo->GetSummonEquipItem( siSummonIndex, siBulletPosition );
				if ( NULL != pBullet )
				{
					if ( pclClient->GameMode == GAMEMODE_SERVER )
					{
						SI32 siUseDay = -1;
						((cltServer*)pclClient)->GetItemUseAbleDay( pBullet, &siUseDay );
						if ( 0 <= siUseDay )
						{
							*psiBulletUnique = ITEMUNIQUE(13600);
							return TRUE;
						}
					}
					else
					{
						*psiBulletUnique = ITEMUNIQUE(13600);
						return TRUE;
					}
				}
			}
		}

		return TRUE;
	}

	if(siAttackType == ATTACKTYPE_GUN || siAttackType == ATTACKTYPE_GUN1 || siAttackType == ATTACKTYPE_NEW_GUN1)
	{
		// 일반 총알
		if( pclSummonInfo->GetHaveItemNumFromItemUnique(siSummonIndex, ITEMUNIQUE(7855)) > 0 )
		{
			*psiBulletUnique = ITEMUNIQUE(7855);
			return TRUE;
		}

		// 과금 총알
		if( pclSummonInfo->GetHaveItemNumFromItemUnique(siSummonIndex, ITEMUNIQUE(13601)) > 0)
		{
			SI32 siBulletPosition = -1;
			siBulletPosition = pclSummonInfo->GetItemPosInEquip(siSummonIndex, ITEMUNIQUE(13601));
			if ( 0 <= siBulletPosition )
			{
				cltItem* pBullet = pclSummonInfo->GetSummonEquipItem( siSummonIndex, siBulletPosition );
				if ( NULL != pBullet )
				{
					if ( pclClient->GameMode == GAMEMODE_SERVER )
					{
						SI32 siUseDay = -1;
						((cltServer*)pclClient)->GetItemUseAbleDay( pBullet, &siUseDay );
						if ( 0 <= siUseDay )
						{
							*psiBulletUnique = ITEMUNIQUE(13601);
							return TRUE;
						}
					}
					else
					{
						*psiBulletUnique = ITEMUNIQUE(13601);
						return TRUE;
					}
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}

