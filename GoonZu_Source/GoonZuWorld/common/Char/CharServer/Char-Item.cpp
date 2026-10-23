#include "../Common/CommonHeader.h"
#include "../../Server/Server.h"

#include "char/CharServer/Char-Server.h"

#include "../../Client/Music/Music.h"

#include "Msg/MsgType-Party.h"
#include "Msg/MsgType-Feast.h"
#include "MsgType-System.h"
#include "Msg/MsgType-Village.h"
#include "Msg/MsgType-Fishing.h"
#include "Msg/MsgType-Agriculture.h"
#include "Msg/MsgType-HappyBeanQuest.h"

#include "../Common/Bullet/Bullet.h"
#include "../Common/Order/Order.h"
#include "../common/GameEvent/GameEvent.h"
#include "../common/Item/ItemCommon/Item-BookBox.h"
#include "../CharManager/CharManager.h"
#include "../CommonLogic/BoxItemMgr/BoxItemMgr.h"

#include "Util/Util.h"
#include "Skill/Skill-Manager.h"

#include "Guild/Guild.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"

//--------------------------------------
// Server
//--------------------------------------
#include "Statistics/Statistics.h"

#include "../Common/PartyMgr/PartyObj.h"
#include "../Common/PartyMgr/PartyMgr.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Summon.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Fishing.h"
#include "../../DBManager/GameDBManager_World/DBMsg-SoulGuard.h"
#include <MsgType-Person.h>
#include <MsgType-Item.h>
#include <MsgType-System.h>
#include <Msg/MsgType-ItemMall.h>
#include <Msg/MsgType-Fishing.h>
#include "MsgRval-Define.h"

#include "../Common/Fishing/FishingMgr.h"

#include "Char-Item.h"

extern cltCommonLogic* pclClient;


// 예약되어 있는 사용 아이템 처리를 한다. 
void cltCharServer::ItemUseReserveAction()
{
	if(IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique()))return;

	if(clItemUseReserveInfo.IsReserved() == false)return;

	// DB로 예약되어 있는 아이템을 변경하라고 요청한다. 
	for(SI32 i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		SI32 unique		=  clItemUseReserveInfo.clInfo[i].siUnique;
		SI32 itemnum	=  clItemUseReserveInfo.clInfo[i].siItemNum;
		SI32 pos		=  clItemUseReserveInfo.clInfo[i].siItemPos;
		clItemUseReserveInfo.clInfo[i].Init();

		if(unique == 0)continue;

		cltItem clitem;

		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(unique);
		if(ref <= 0)continue;

		// 아이템을 소모할 것. 
		if(itemnum < 0)
		{

			clitem.Set(&pclClient->pclItemManager->pclItemInfo[ref]->clItem);
			clitem.siItemNum = -itemnum;

			if (pos < 0)
				pos = pclCI->clCharItem.FindItemInv(&clitem, false);

			if(pos >= 0)
			{
				sDBRequest_UseItem clMsg(GetID(), pclCI->GetPersonID(), pos, &clitem,USEITEM_REASON_NONE);
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
		// 아이템을 증가시킬 것. 
		else if(itemnum > 0)
		{
			clitem.Set(&pclClient->pclItemManager->pclItemInfo[ref]->clItem);
			clitem.siItemNum = itemnum;

			if (pos < 0)
				pos = pclCI->clCharItem.FindItemInv(&clitem, false);

			if(pos >= 0)
			{
				// 사용자에게 아이템을 넣는 메시지를 사용한다. 
				sDBRequest_InputItemToPerson clMsg(GetID(), pclCI->GetPersonID(), pos, INPUTITEMPERSON_OPTION_NONE, clitem.CanPile(pclClient->pclItemManager), &clitem, 0, clitem.IsCreateSID(pclClient->pclItemManager));
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
	}	
}

// 예약되어있는 화살 및 탄환을 소비한다.
void cltCharServer::BulletUseReserveAction()
{
	SI32 siCharUnique = GetCharUnique();

	// 사용자 케릭터 일때만
	if ( false == IsPC(GetCharUnique()) )
	{
		return;
	}


	// 케릭터 아이템이 예약되어 있을때
	if ( true == clBulletUseReserveInfo.IsReserved() )
	{
		BulletUseReserveAction_ForPerson();
	}

	// 케릭터가 갖고 있는 소환수의 아이템이 예약되어 있을때
	if ( true == clSummonBulletUseReserveInfo.IsReserved() )
	{
		BulletUseReserveAction_ForSummon();
	}

}

void cltCharServer::BulletUseReserveAction_ForPerson()
{
	for(SI32 i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		SI32 unique		=  clBulletUseReserveInfo.clInfo[i].siUnique;
		SI32 itemnum	=  clBulletUseReserveInfo.clInfo[i].siItemNum;
		SI32 pos		=  clBulletUseReserveInfo.clInfo[i].siItemPos;
		clBulletUseReserveInfo.clInfo[i].Init();

		if(unique == 0)continue;

		cltItem clitem;

		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(unique);
		if(ref <= 0)continue;

		// 아이템을 소모할 것. 
		if(itemnum < 0)
		{
			clitem.Set(&pclClient->pclItemManager->pclItemInfo[ref]->clItem);
			clitem.siItemNum = -itemnum;

			if(pos >= 0)
			{
				sDBRequest_UseItem clMsg(GetID(), pclCI->GetPersonID(), pos, &clitem,USEITEM_REASON_NONE);
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
	}	

}

void cltCharServer::BulletUseReserveAction_ForSummon()
{
	for ( SI32 siIndex=0; siIndex<MAX_USEITEM_RESERVE_NUM; siIndex++ )
	{
		SI16 siSummonIndex	= clSummonBulletUseReserveInfo.m_clInfo[ siIndex ].m_siSummonIndex;
		SI16 siItemUnique	= clSummonBulletUseReserveInfo.m_clInfo[ siIndex ].m_siItemUnique;
		SI16 siItemNum		= clSummonBulletUseReserveInfo.m_clInfo[ siIndex ].m_siItemNum;
		SI16 siItemPos		= clSummonBulletUseReserveInfo.m_clInfo[ siIndex ].m_siItemPos;


		if ( (0 > siSummonIndex) || (0 >= siItemUnique) || (0 <= siItemNum) || (0 > siItemPos) )
		{
			continue;
		}

		cltSummon* pclsummon = pclCI->clSummonInfo.GetSummon( siSummonIndex );
		if( pclsummon == NULL )			continue;

		SI32 siRef = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
		if ( 0 >= siRef )
		{
			continue;
		}

		cltItem clItem;

		clItem.Set( &pclClient->pclItemManager->pclItemInfo[ siRef ]->clItem );
		clItem.SetItemNum( (-siItemNum) );

		sDBRequest_SummonUseItem clMsg( GetID(), pclCI->GetPersonID(), pclsummon->siSummonID, siItemPos, &clItem, SUMMONUSEITEM_REASON_GENERAL );
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}

	clSummonBulletUseReserveInfo.Init();

}

// 소환수를 캐릭터 옆에 생성한다.
SI32 cltCharServer::CreateSummon(cltSummon* pclSummon)
{
	cltServer* pclserver = (cltServer*)pclClient;
	cltItemManagerCommon* pclItemManager;
	pclItemManager = pclClient->pclItemManager;

	if( pclSummon == NULL )	return	0	;

	SI32 siSummonKind = pclSummon->siKind;
	//=========================================================================
	// 해당 몬스터가 될수 있는지 검사한다.
	//=========================================================================
	//if(pclClient->pclKindInfoSet->pclKI[siSummonKind]->IsAtb(ATB_SUMMON) == FALSE)
	if(siSummonKind < 0 || siSummonKind >= MAX_KINDINFO_NUMBER)	return 0;
	cltKindInfo*	pclKI = NULL;
	if( pclClient->IsCountrySwitch(Switch_Summon_Separate) )
	{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[siSummonKind];	}
	else
	{	pclKI = pclClient->pclKindInfoSet->pclKI[siSummonKind];			}

	if(pclKI == NULL)					return 0;
	if(pclKI->siSummonRank <= 0)
	{
		return 0;
	}

	SI32 siSummonIndex = pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclSummon->siSummonID);
	if ( siSummonIndex < 0 )	return	0	;

	SI32 serverminutevary = pclClient->GetMinuteFromsTime(pclserver->sTime);

	BOOL bCanTakeOut = FALSE;

	// PCK : 과금 소환수의 경우 시간과 상관없이 스태미너가 0이면 소환되지 않는다.
	if( pclKI != NULL && pclKI->IsAtb(ATB_SUMMONSTAMINASTATIC) )
	{
		if( pclSummon->clIP.GetLuk() <= 0 )
		{
			((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMON_NEEDCHARGESTAMINA,  0, 0, GetCharUnique());
			return 0;
		}
	}
	else
	{
		if(	pclSummon->clIP.GetLuk() <= 0 && 
			TABS(pclSummon->siTakeInMinute - serverminutevary) < SUMMON_TAKEOUT_DELAY)
		{
			((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMON_FAIL_TAKEOUTDELAY,  0, 0, GetCharUnique());
			return 0;
		}
	}

	//=========================================================================
	// DOLL 아이템의 해당 Monter Kind를 얻어와 주위에 소환한다.
	//=========================================================================
	SI32 siSummonsX, siSummonsY;
	if(pclMap->FindEmptyAreaForCharLocate(siSummonKind, GetX(), GetY(), 20, &siSummonsX, &siSummonsY, true) == true)
	{
		//// 모후모후 이벤트 지역에서는 캐릭터 위치에 강제로 소환한다. - by LEEKH
		//if(pclMap->siMapIndex == MAPINDEX_MOFUMOFULAND)
		//{
		//	siSummonsX = GetX();
		//	siSummonsY = GetY();
		//}
		// 이벤트 맵이라도 소환수를 생성 가능 하도록 한다.[성웅]
		cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(GetMapIndex());
		if(pclmap != NULL)
		{
			SI32 siColl = pclmap->CalcCollInfo( GetX(), GetY())	;
			if(siColl & BLOCK_EVENT4|| siColl & BLOCK_EVENT5 || siColl & BLOCK_EVENT6)
			{
				siSummonsX = GetX();
				siSummonsY = GetY();
			}
		}

		if(pclMap->siMapIndex == MAPINDEX_MOFUMOFULAND) // 혹시 몰라서 살려 둔다.
		{
			siSummonsX = GetX();
			siSummonsY = GetY();
		}

		cltIdentity clIdentity;
		clIdentity.MakeSummon(IDENTITY_PERSON_SUMMON, siSummonIndex, GetCharUnique(), pclCI->GetPersonID());

		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(&clInnerStatus.clEnemyType);		// 주인의 적대 정보를 사용한다. 


		SI32 siSummonID = pclserver->SetNPCChar(siSummonKind, 
			pclCI->clBI.siHomeVillage,  
			&clIdentity, 
			1, 
			pclSummon->clIP.GetLife(), 
			siSummonsX, 
			siSummonsY, 
			GetMapIndex(), 
			NPCITEMMODE_NONE, 
			&clinnerstatus );
		if(pclCM->IsValidID(siSummonID))
		{
			// 소환수를만들었으면 만드놈의 소유물임을 설정해야한다.

			// 중요 정보를 업데이트 한다. 
			pclserver->pclCM->CR[ siSummonID ]->pclCI->clIP.Set(&pclSummon->clIP);

#ifdef _SAFE_MEMORY
			SI32 summonskill = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclCI->clCharItem.clItem[0], siID );
#else
			SI32 summonskill = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, pclCI->clCharItem.clItem, siID );
#endif
			if (pclSummon->clIP.GetLuk() <= 0)
				pclserver->pclCM->CR[ siSummonID ]->pclCI->clIP.SetLuk(pclClient->GetMaxStaminaValue(summonskill, pclSummon->siKind));
			else	
				pclserver->pclCM->CR[ siSummonID ]->pclCI->clIP.SetLuk(pclSummon->clIP.GetLuk());

			pclserver->pclCM->CR[ siSummonID ]->SetUpdateSwitch(UPDATE_IP, true, 0);

			pclserver->pclCM->CR[ siSummonID ]->ParametaBoxAction();

			// 체력과 마법력을 설정해준다. 
			pclserver->pclCM->CR[ siSummonID ]->SetLife(pclSummon->clIP.GetLife());
			pclserver->pclCM->CR[ siSummonID ]->SetMana(pclSummon->clIP.GetMana());
			return siSummonID;
		}
	}

	return 0;
}

SI32 cltCharServer::CreateSoulGuard( cltSoulGuard* pclCreateSoulGuard )
{
	if ( NULL == pclCreateSoulGuard )
	{
		return 0;
	}

	if ( NULL == pclSoulGuard )
	{
		return 0;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return 0;
	}

	if ( NULL == pclserver->m_pclSoulGuardMgr )
	{
		return 0;
	}

	SI32 siSoulGuardKind = pclCreateSoulGuard->GetKind();
	if ( (0 > siSoulGuardKind) || (MAX_KINDINFO_NUMBER <= siSoulGuardKind) )
	{
		return 0;
	}

	SI32 siMyX			= GetX();
	SI32 siMyY			= GetY();

	SI32 siSoulGuardX	= 0;
	SI32 siSoulGuardY	= 0;

	// 소환될 위치를 찾아야 한다
	if ( false == pclMap->FindEmptyAreaForCharLocate(siSoulGuardKind, siMyX, siMyY, 20, &siSoulGuardX, &siSoulGuardY, true) )
	{
		return 0;
	}

	// 주인공이 이벤트 지역에 있으면 주인공 위치로 설정해 준다
	cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(GetMapIndex());
	if ( NULL != pclmap )
	{
		SI32 siCollInfo = pclmap->CalcCollInfo( siMyX, siMyY );
		if ( (BLOCK_EVENT4 & siCollInfo) || (BLOCK_EVENT5 & siCollInfo) || (BLOCK_EVENT6 & siCollInfo) )
		{
			siSoulGuardX = siMyX;
			siSoulGuardY = siMyY;
		}
	}

	// 라쿤랜드일때는 강제로 주인의 위치로 넣어준다
	if ( MAPINDEX_MOFUMOFULAND == pclMap->siMapIndex )
	{
		siSoulGuardX = siMyX;
		siSoulGuardY = siMyY;
	}

	// 수호정령에 필요한 기본 주인정보를 입력한다
	cltIdentity clSoulGuardIdentity;
	clSoulGuardIdentity.MakeSoulGuard( IDENTITY_SOULGUARD, GetCharUnique(), pclCI->GetPersonID() );

	cltInnerStatusInfo clSoulGuardInner;
	clSoulGuardInner.Init();

	// 실제 수호정령 생성
	SI32 siSoulGuardID = pclserver->SetNPCChar( siSoulGuardKind,		pclCI->clBI.siHomeVillage,
		&clSoulGuardIdentity, 
		1,						1,
		siSoulGuardX,			siSoulGuardY,				GetMapIndex(),
		NPCITEMMODE_NONE,
		&clSoulGuardInner
		);
	if ( false == pclCM->IsValidID(siSoulGuardID) )
	{
		return 0;
	}

	// 수호정령이 갖고있는 능력을 미리 설정해 준다
	SI32 siHaveAbility = pclserver->m_pclSoulGuardMgr->GetHaveAbility( siSoulGuardKind );

	pclSoulGuard->SetHaveAbility( siHaveAbility );

	// 소환된 상태로 변경
	pclSoulGuard->SetStatus( SOULGUARD_STATUS_OUT );

	return siSoulGuardID;
}

void cltCharServer::DeleteSoulGuard( void )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	if ( NULL == pclSoulGuard )
	{
		return;
	}

	if ( false == pclserver->pclCM->IsValidID(siMySoulGuardUnique) )
	{
		return;
	}

	cltCharServer* pclSoulGuardChar = pclserver->pclCM->GetCharServer( siMySoulGuardUnique );
	if ( NULL == pclSoulGuardChar )
	{
		return;
	}

	if ( IDENTITY_SOULGUARD !=  pclSoulGuardChar->clIdentity.siIdentity )
	{
		return;
	}

	if ( pclCI->GetPersonID() != pclSoulGuardChar->clIdentity.clSoulGuard.m_siParentPersonID )
	{
		return;
	}

	// 실제 수호정령을 삭제한다
	pclSoulGuardChar->DeleteInCell();
	pclserver->pclCM->DeleteChar( siMySoulGuardUnique );

	// 내가 갖고 있는 수호정령 정보 변경
	pclSoulGuard->SetStatus( SOULGUARD_STATUS_IN );

	siMySoulGuardUnique = 0;

}

// 아이템을 사용한다. 
bool cltCharServer::UseItem(cltItem clitem,SI32 itempos)
{
	cltItemManagerCommon* pclItemManager;
	pclItemManager = pclClient->pclItemManager;

	SI32 siRealItemUniques[MAX_ITEM_UNIQUE];
	SI32 siRealItemCount = 0;

	// 아이템을 사용할 수 없는 것이면 , 
	SI32 returnval = 0;
	if(CanUseItem(clitem, &returnval, itempos) == false)return false;

	cltServer* pclserver = (cltServer*)pclClient;

	if( true == bPersonalShopSwitch ||
		true == bPremiumPersonalShop )
	{
		for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
		{
			if( clShopData.m_clItem[i].siUnique == clitem.siUnique  )
			{
				//KHY - 1023 - 총알 화살 무한사용 방지.
				if(( clShopData.m_clItem[i].siUnique != ITEMUNIQUE( 7850 ))&& ( clShopData.m_clItem[i].siUnique != ITEMUNIQUE( 7855 )) )
				{
					if( clShopData.m_clItem[i].siItemNum >= clitem.siItemNum )
					{
						//	SendServerResponseMsg(0, SRVAL_FAIL_PERSONALSHOP_USEITEM, 0, 0, GetID());
						return false;
					}
				}
			}
		}
	}

	SI32 itemunique = clitem.siUnique;
	SI32 ref		= pclItemManager->FindItemRefFromUnique(itemunique);
	if(ref <= 0)return false;

	SI32 itemtype	= pclItemManager->pclItemInfo[ref]->siType;
	GMONEY itematb	= pclItemManager->pclItemTypeInfo[itemtype]->siAtb;

	if(pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CANUSEITEM)  == false)return false;

	//	cltServer* pclserver = (cltServer*)pclClient;

	// 길드전장 안에서 회복약을 사용할 경우
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 변경후
	{
		if( IsGuildWarMap_Use_New_GuildWar(GetMapIndex()) )	// 예전 길드전은 약제한
		{
			if( itemunique == ITEMUNIQUE( 2565 ) ||
				itemunique == ITEMUNIQUE( 2570 ) ||
				itemunique == ITEMUNIQUE( 2575 ) ||
				itemunique == ITEMUNIQUE( 2580 ) ||
				itemunique == ITEMUNIQUE( 2585 ) || 
				// 마력회복제도 추가
				itemunique == ITEMUNIQUE( 2590 ) ||
				itemunique == ITEMUNIQUE( 2595 ) ||
				itemunique == ITEMUNIQUE( 2600 ) ||
				itemunique == ITEMUNIQUE( 2605 ) 
				)
			{
				SI32 fieldindex =	0	;
				if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 변경후
				{
					fieldindex =  GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( GetMapIndex() );
				}
				else	// 변경전
				{
					fieldindex = GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( GetMapIndex() );
				}
				if ( pclserver->IsCountrySwitch( Switch_NewGuildHuntMapWar ))	// 변경후
				{
					if ( fieldindex >= 0 && fieldindex < GUILDWAR_FIELDNUM_OLD &&
						pclserver->pclGuildWarManager[fieldindex]->CanDrinkPotion(pclCI->GetPersonID()) )
					{
						pclserver->pclGuildWarManager[fieldindex]->DrinkPotion( pclCI->GetPersonID() );
					}
					else
					{
						return false;
					}
				}
				else	// 변경전 
				{
					if ( fieldindex >= 0 && fieldindex < GUILDWAR_FIELDNUM &&
						pclserver->pclGuildWarManager[fieldindex]->CanDrinkPotion(pclCI->GetPersonID()) )
					{
						pclserver->pclGuildWarManager[fieldindex]->DrinkPotion( pclCI->GetPersonID() );
					}
					else
					{
						return false;
					}

				}
			}
		}


	}
	else	// 변경전
	{
		if( IsGuildWarMap_Not_Use_New_GuildWar(GetMapIndex()) )	// 예전 길드전은 약제한
		{
			if( itemunique == ITEMUNIQUE( 2565 ) ||
				itemunique == ITEMUNIQUE( 2570 ) ||
				itemunique == ITEMUNIQUE( 2575 ) ||
				itemunique == ITEMUNIQUE( 2580 ) ||
				itemunique == ITEMUNIQUE( 2585 ) || 
				// 마력회복제도 추가
				itemunique == ITEMUNIQUE( 2590 ) ||
				itemunique == ITEMUNIQUE( 2595 ) ||
				itemunique == ITEMUNIQUE( 2600 ) ||
				itemunique == ITEMUNIQUE( 2605 ) 
				)
			{
				SI32 fieldindex =	0	;
				if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 변경후
				{
					fieldindex =  GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( GetMapIndex() );
				}
				else	// 변경전
				{
					fieldindex = GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( GetMapIndex() );
				}
				if ( pclserver->IsCountrySwitch( Switch_NewGuildHuntMapWar ))	// 변경후
				{
					if ( fieldindex >= 0 && fieldindex < GUILDWAR_FIELDNUM_OLD &&
						pclserver->pclGuildWarManager[fieldindex]->CanDrinkPotion(pclCI->GetPersonID()) )
					{
						pclserver->pclGuildWarManager[fieldindex]->DrinkPotion( pclCI->GetPersonID() );
					}
					else
					{
						return false;
					}
				}
				else	// 변경전 
				{
					if ( fieldindex >= 0 && fieldindex < GUILDWAR_FIELDNUM &&
						pclserver->pclGuildWarManager[fieldindex]->CanDrinkPotion(pclCI->GetPersonID()) )
					{
						pclserver->pclGuildWarManager[fieldindex]->DrinkPotion( pclCI->GetPersonID() );
					}
					else
					{
						return false;
					}

				}
			}
		}
	}
	switch(itemtype)
	{
		//==============================================================================================================================
		// DOLL
		//==============================================================================================================================
	case ITEMTYPE_DOLL:
		{
			// 소환수 생성으로 DB와 통신중이므로 실패
			if(bIsNowAddSummonThroughDB)		{		return false;		}

			//------------------------------
			// 성공결정 
			//------------------------------
			bool bsuccess = false;
			if(itemunique == ITEMUNIQUE(12000))		// 구리댁 
			{
				if(rand() % 100 < HATCH_SUMMON_PERCENT)
				{
					bsuccess = true;
				}
			}
			else
			{
				bsuccess = true;
			}

			if(bsuccess)
			{
				cltServer* pclserver = (cltServer*)pclClient;

				SI32 siSummonKind = 0, preturn = 0, siSummonIndex = 0;
				if(CanUseSummonDoll(itemunique, &siSummonKind, &siSummonIndex, &preturn) == true)
				{
					//--------------------------------
					// 진화후의 소환수 Kind를 점검한다.
					//--------------------------------
					if(siSummonKind <= 0 || siSummonKind >= MAX_KIND_NUMBER)	return false;

					//====================================================
					// 새로 만들어질 소환수를 설정한다. - 기본정보 설정
					//====================================================
					cltPIPInfoCommon	clipinfo;
					cltPIPInfo			clipinfo2;

					//[진성] 소환수 테이블 분리 후 만들 소환수 정보 참조 수정. => 2008-6-18
					cltKindInfo* pclki = NULL;
					cltGMCommandKindInfo* pclGMki = NULL;

					if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
					{
						pclki = pclClient->pclKindInfoSet->pclSummonKI[siSummonKind];
						pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[siSummonKind];
					}
					else
					{
						pclki = pclClient->pclKindInfoSet->pclKI[siSummonKind];
						pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[siSummonKind];
					}
					if(NULL == pclki)
						break;

					clipinfo.SetBasicAbility(&pclki->clBA);	
					clipinfo2.Set(&clipinfo);

					// 소환수 능력치 설정
					cltParametaBox clPB;
					clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclki, pclGMki);
					clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, NULL, GAMEMODE_SERVER, true, false);

					//cyj 소환수의 Luk은 스태미너로 사용
#ifdef _SAFE_MEMORY
					SI32 summonskill = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclCI->clCharItem.clItem[0], siID );
#else
					SI32 summonskill = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, pclCI->clCharItem.clItem, siID );
#endif
					// 소환수 스테미너 설정
					clipinfo.SetLuk( pclClient->GetMaxStaminaValue(summonskill, siSummonKind) );
					clipinfo.SetLife(clPB.GetMaxLife());	
					clipinfo.SetMana(clPB.GetMaxMana());

					cltSummonScroll clscroll(pclki->siSummonScroll[0], 0, pclki->siSummonScroll[1], 0);

					SI32 siSummonStatus = 0;
					if ( pclClient->IsCountrySwitch(Switch_SummonHouse) )
					{
						// 보관소로 바로 들어가므로 상태는 TAKEIN이다.
						siSummonStatus = SUMMON_STATUS_TAKEIN;
					}
					else
					{
						if(GetSummonID())	siSummonStatus = SUMMON_STATUS_TAKEIN;
						else				siSummonStatus = SUMMON_STATUS_TAKEOUT;
					}

					// 소환수 이름 설정
					TCHAR* pText = GetTxtFromMgr(449);
					TCHAR summonname[MAX_PLAYER_NAME] = TEXT("");
					StringCchCopy(summonname, MAX_PLAYER_NAME, pText);

					SI08 siIdentity = SUMMON_IDENTITY_NORMAL;

					cltSummonItemInfo clItemInfo;
					clItemInfo.Init();

					SI32 siTransformKind    = 0;
					SI32 siTransformAbility = 0;
					if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
					{
						pclserver->GetSummonTransformManager()->GetTransformSummonInfo( siSummonKind, siTransformKind, siTransformAbility );					
					}
					// DB에 소환수를 설정한다.
					cltSummon clSummon(	0,
						siSummonKind,
						summonname,
						siSummonStatus,
						SUMMON_ATTACKTYPE_AVOID,
						siIdentity,
						0,
						0,
						0,
						&clipinfo,		
						&clscroll,
						0
						);

					// Switch_SummonHouse 스위치가 켜져있으면, siSummonIndex 는 0 ~ 6 까지
					// Switch_SummonHouse 스위치가 꺼저있으면, siSummonIndex 는 0 ~ 2 까지
					SendDBSummon(SETSUMMON_REASON_MAKE, siSummonIndex, &clSummon);

					// 클라이언트에 사실을 알린다.
					((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMONSUCCESS,  0, 0, GetCharUnique());

					//-------------------------------------
					// 소환수를 소환중이므로 다음 소환수는 소환시키지 못하게 한다.
					//-------------------------------------
					bIsNowAddSummonThroughDB = true;

				}
				else
				{
					return false;
				}
			}
			else
			{
				// 클라이언트에 사실을 알린다. 
				((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMONHATCHFAIL,  0, 0, GetCharUnique());
			}
		}
		break;

		//==============================================================================================================================
		// BOOK
		//==============================================================================================================================
	case ITEMTYPE_BOOK:
		{	
			//[진성] 스킬북.
			if( pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SKILLBOOK ) )
			{
				static cltSkillBookManager*	pclSkillBookManager = pclClient->pclItemManager->pclSkillBookManager;
				if( NULL == pclSkillBookManager ) return false;

				// 스킬북 마법이면.(보조마법이 아니다)
				if( 0 >= pclSkillBookManager->GetSkillBookParentMagicFromItemUnique( clitem.siUnique ) )
				{
					cltSkillBookMagic clAddSkillBookMagic( 0, 0, &pclserver->sTime );
					switch( GetAttackType() )
					{
					case ATTACKTYPE_SWORD:		clAddSkillBookMagic.SetSkillUnique(SKILL_USESWORD1);	break;
					case ATTACKTYPE_SPEAR:		clAddSkillBookMagic.SetSkillUnique(SKILL_USESPEAR1);	break;
					case ATTACKTYPE_AXE:		clAddSkillBookMagic.SetSkillUnique(SKILL_USEAXE1);		break;
					case ATTACKTYPE_STAFF:		clAddSkillBookMagic.SetSkillUnique(SKILL_USESTAFF1);	break;
					case ATTACKTYPE_STAFF1:		clAddSkillBookMagic.SetSkillUnique(SKILL_USESTAFF1);	break;
					case ATTACKTYPE_STAFF2:		clAddSkillBookMagic.SetSkillUnique(SKILL_USESTAFF1);	break;
					case ATTACKTYPE_NEW_STAFF2:	clAddSkillBookMagic.SetSkillUnique(SKILL_USESTAFF1);	break;
					case ATTACKTYPE_BOW:		clAddSkillBookMagic.SetSkillUnique(SKILL_USEBOW1);		break;
					case ATTACKTYPE_BOW1:		clAddSkillBookMagic.SetSkillUnique(SKILL_USEBOW1);		break;
					case ATTACKTYPE_NEW_BOW1:	clAddSkillBookMagic.SetSkillUnique(SKILL_USEBOW1);		break;
					case ATTACKTYPE_GUN:		clAddSkillBookMagic.SetSkillUnique(SKILL_USEGUN1);		break;
					case ATTACKTYPE_GUN1:		clAddSkillBookMagic.SetSkillUnique(SKILL_USEGUN1);		break;
					case ATTACKTYPE_NEW_GUN1:	clAddSkillBookMagic.SetSkillUnique(SKILL_USEGUN1);		break;
					default:					return false;
					}

					clAddSkillBookMagic.SetMagicKind( pclSkillBookManager->GetSkillBookMagicFromItemUnique( clitem.siUnique ) );
					if( 0 >= clAddSkillBookMagic.GetMagicKind() ) return false;

					SI08 siIndex = pclCharSkillBookInfo->GetIndexFromAttackType( GetAttackType() );
					if( 0 > siIndex ) return false;

					sDBRequest_SkillBook_Magic_Add	clMsg( GetID(), pclCI->GetPersonID(), siIndex, &clAddSkillBookMagic );
					pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
				}
				// 보조 마법.
				else
				{
					if( pclClient->pclItemManager->pclSkillBookManager->GetSkillBookParentMagicFromItemUnique( clitem.siUnique ) )
					{
						SI32 skilllevel  = pclCI->clCharSkill.GetTotalSkillLevel( clitem.GetSkillUnique( pclItemManager ), 0, 0 );
						SI32 skilllimit  = clitem.GetSkillLevelLimit( pclClient->pclItemManager );
						if( skilllevel < skilllimit ) return false;
					}

					SI32 siMagicKind		= pclSkillBookManager->GetSkillBookMagicFromItemUnique( clitem.siUnique );
					SI32 siParentMagicKind	= pclSkillBookManager->GetSkillBookParentMagicFromItemUnique( clitem.siUnique );

					if( 0 >= siMagicKind || 0 >= siParentMagicKind ) return false;

					SI32 skillunique = clitem.GetSkillUnique( pclItemManager );
					cltSkillBookSubMagic clSkillBookSubMagic( skillunique, siParentMagicKind, siMagicKind, &pclserver->sTime );

					sDBRequest_SkillBook_SubMagic_Add	clMsg( GetID(), pclCI->GetPersonID(), &clSkillBookSubMagic );
					pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
				}


				// 스킬북 사용하기 퀘스트 중이라면
				IncQuestPara(QUEST_CLEAR_COND_USE_SKILLBOOK, 1, clitem.siItemNum);
			}
			else
			{
				//----------------------------------
				// 스킬의 종류를 확인한다.
				//----------------------------------
				SI32 skillunique	= clitem.GetSkillUnique(pclClient->pclItemManager);
				SI32 maxskilllevel	= pclClient->pclSkillManager->GetMaxLevel(skillunique);
				SI32 siDecreaseSkillPoint = 1;

				// 제조활성화 : 제조와 관련된 책은 스킬 보너스 포인트를 소모하지 않는다. by LEEKH 2007.07.30
				//if(	pclClient->IsWhereServiceArea(ConstSwitch_ManufactureRevitalize) && 
				if(	pclClient->IsCountrySwitch(Switch_ManufactureRevitalize) && 
					pclClient->pclSkillManager->IsManufactureSkill(skillunique))
				{
					siDecreaseSkillPoint = 0;
				}

				sDBRequest_IncreaseSkill clMsg(GetID(), pclCI->GetPersonID(), skillunique, 1, maxskilllevel, siDecreaseSkillPoint, clitem);
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
		break;

		//==============================================================================================================================
		// FOOD
		//==============================================================================================================================
	case ITEMTYPE_FOOD:
	case ITEMTYPE_SEAFOOD:
		{
			cltServer* pclserver = (cltServer*)pclClient;

			// 자이언트 라쿤 부활 알 세트.
			if( itemunique == ITEMUNIQUE(3937) )
			{
				if( 80 > rand()%100 ) 
				{
					SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.

					SI16 siPercent = 1;
					stBuf* pstBuf = pclCI->clBI.clBufInfo.GetBuf( BUF_GIANTRACCOON_RESURRECT_EVENT );
					if( pstBuf )
					{
						siPercent = pstBuf->siPercent;
						switch( siPercent )
						{
						case 1:   
						case 2:	
						case 3:	
						case 4:	siPercent++;					break;
						case 5:	siPercent = pstBuf->siPercent;	break;
						}
					}

					SI16 index = pclCI->clBI.clBufInfo.FindBufSlot( BUF_GIANTRACCOON_RESURRECT_EVENT );
					if( index > -1 )
					{
						stBuf DelBuf( BUF_GIANTRACCOON_RESURRECT_EVENT, 0, 0, 0 );
						sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), index, &DelBuf );
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
					else
					{
						// 새로운 버프적용 할 빈 인덱스 얻기.
						index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_GIANTRACCOON_RESURRECT_EVENT, false );		
					}

					if( index > -1 )
					{
						// 버프를 준다.
						stBuf addBuf( BUF_GIANTRACCOON_RESURRECT_EVENT, siPercent, usetime );
						AddBufThroughDB( index, &addBuf , false, 0, NULL, 0 );
					}
				}
				else
				{
					((cltServer*)pclClient)->SendServerResponseMsg( 0, SRVAL_GIANTRACCOONRESURRECTEVENT_FAIL_USE_ITEM, 0, 0, GetID() );	
				}
			}
			//cyj 극상초쿄 사용
			else if( itemunique == ITEMUNIQUE(3860) || itemunique == ITEMUNIQUE(3865) || itemunique == ITEMUNIQUE(3870))
			{
				if ( bIsSetPremium ) return false ;
				if ( pclCI->clBI.bPremiumService ) return false ;
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ; 
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP ) ) return false;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS ) ) return false;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP ) ) return false;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300 ) ) return false;

				SI32 usedate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() + 2 ;
				sDBRequest_SetPremium clPremium(GetCharUnique(),pclCI->GetPersonID(),true,usedate, 0, 0, 0, 0, &clitem,itempos,SETPREMIUM_EXP);
				((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clPremium);		
				bIsSetPremium = true ;
				// 떡갈나무찰떡은 다른 음식 효과를 가질수 없다!!!
				return false ;

			}
			// [추가 : 황진성 2007. 11. 12.  추수 감사절 이벤트. 칠면조 요리 사용.]
			else if(itemunique == ITEMUNIQUE(3911))
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				SI32 usetime =  0;
				usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 15 ;// 30분간사용				
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_THANKSGIVING_DAY, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_THANKSGIVING_DAY, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}
			// 장인의 초쿄
			else if( itemunique == ITEMUNIQUE(3903) )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS ) ) return false ; 

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_NOTBONUSTIME_MAKEEXPUP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_NOTBONUSTIME_MAKEEXPUP, 50, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );

				}
				// 제조 만쥬는 다른 음식 효과를 가질수 없다!!!
				return false;
			}
			// 제조 만쥬
			else if( itemunique == ITEMUNIQUE(3880) )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS ) ) return false ; 

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_MAKEEXPUP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_MAKEEXPUP, 50, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );

				}
				// 제조 만쥬는 다른 음식 효과를 가질수 없다!!!
				return false;
			}
			//cyj 떡갈나무 찰떡
			else if( itemunique == ITEMUNIQUE(3875))
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ; 
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_MAKEEXPUP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_MAKEEXPUP, 20, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false;
			}
			// 사랑의 초콜릿
			else if( itemunique == ITEMUNIQUE(3890)  || itemunique == ITEMUNIQUE(3899) )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300) ) return false ;	// 스페셜 엠플

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_LOVEPOWER, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_LOVEPOWER, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false ;			
			}
			// 우정의 초콜릿
			else if( itemunique == ITEMUNIQUE(3895) || itemunique == ITEMUNIQUE(3901) )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300) ) return false ;	// 스페셜 엠플

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_FRIENDSHIP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_FRIENDSHIP, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false ;	
			}
			// 히나모찌
			else if( itemunique == ITEMUNIQUE(3897) )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300) ) return false ;	// 스페셜 엠플

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_HINAMOCHI, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_HINAMOCHI, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false ;	
			}
			//채광용 농경용 아이템 
			else if( itemunique == ITEMUNIQUE(3918)|| itemunique == ITEMUNIQUE(3919) || itemunique == ITEMUNIQUE(3920))
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false;
				//1일은 고구마 7일은 양파 30일은 오이를 사용 했음 차후 변경 해줘야 함,
				SI32 usetime =  0;
				//아이템의 종류에 따라 서로 다른 시간을 준다.
				if( itemunique == ITEMUNIQUE(3918))	// 현실시간  1일 =  30 * 720 = 21600  / 30 시간   ㅇㅇ????
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 720 ;
				else if( itemunique == ITEMUNIQUE(3919) )	// 현실시간 15일 = 30 * 24 = 720 /2 시간  30 * 720 = 21600/2 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 5040 ;
				else if( itemunique == ITEMUNIQUE(3920) )	// 현실시간 30일 = 30 * 24 = 720시간  30 * 720 = 21600 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;
				else
					return false ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_FARMINGNMININGFOREVER, true );
				if ( index > -1 )
				{
					stBuf addBuf(BUF_FARMINGNMININGFOREVER, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}
			//----------------------------------------------------------------------------------
			// [지연] 럼블체리 아이템.
			//       - 사용하면 여러 버프효과 중에서 무작위로 48분간 받을 수 있는 일회성 아이템.
			//----------------------------------------------------------------------------------
			else if( itemunique == ITEMUNIQUE(3926) )	
			{
				// 해당아이템을 인벤에 넣을 수 있는지 확인
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				//럼블체리 버프가 걸려있으면 그 버프를 삭제한다.
				SI16 index0 = -1;

				index0 = pclCI->clBI.clBufInfo.FindBufGetTypeSlot(BUF_GET_TYPE_RUMBLECHERRY);
				{
					if( index0 > -1) // 버프가 있을경우에
					{
						// 기존에 걸려있던 버프를 삭제한다.
						stBuf DelBuf( pclCI->clBI.clBufInfo.m_stBuf[index0].siBufType, 0, 0, 0 );
						sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), index0, &DelBuf );
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
				}

				// 기본 현재 상태를 찾아 설정한다.
				SI16 IsHaveBufType[12] = 
				{
					BUF_PREMIUMEXPUP,			// 경험치 150% 획득가능. (성장비약)
						BUF_ATTACKUP,				// 공격력 20% 증가.
						BUF_DEFENSEUP,				// 방어력 20% 증가.

						BUF_MAKEEXPUP,				// 제조 경험치 150% 증가.
						BUF_NOFIRSTATTACK,			// 선공 방지 효과.
						BUF_SUCCESS_MANUFACTURE,	// 아이템 제조 100% 성공 효과. (집중 총명탕)

						BUF_FOURLEAF,				// 네잎크로버(아이템획득 *2) 효과.
						BUF_LOVEPOWER,				// 사랑의 캔디 효과 - 전투/제조 경험치 150% 증가, 소환수스테미너 방지 효과, 수환수 전투력 증가.
						BUF_FRIENDSHIP,				// 우정의 캔디 효과 - 전투/제조 경험치 120% 증 가, 공격력/방어력 120% 증가.

						BUF_HINAMOCHI,				// 레인보우 케이크 효과 - 전투/제조 경험치 200% 증가, 공격력/방어력 120% 상승, 선공방지.
						BUF_SUMMONATACUP,			// 소환수 영양제 효과 - 소환수 공격력, 방어력 20% 증가.
						BUF_FARMINGNMININGFOREVER   // 새참 효과 - 농경, 채광시 포만감 감소 안함.
				};

				// 상수 값
				SI16 BufCreateRate_Const[12] = 
				{
					50,
						150,
						150,

						80,
						110,
						120,

						10,
						30,
						30,

						20,
						150,
						100
				};

				// 경험치가 증가하는 버프를 가지고 있을 때 
				// DB정보->캐릭터기본정보.버프관련정보.가지고있는버프()
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) )
				{
					IsHaveBufType[0] = 0;
					BufCreateRate_Const[0] = 0;
				}

				// 공격력 증가.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ||
					pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP30) )
				{
					IsHaveBufType[1] = 0;
					BufCreateRate_Const[1] = 0;
				}
				// 방어력 증가.
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ||
					pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP30) )
				{
					IsHaveBufType[2] = 0;
					BufCreateRate_Const[2] = 0;
				}

				// 제조 경험치 증가.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_MAKEEXPUP) )
				{
					IsHaveBufType[3] = 0;
					BufCreateRate_Const[3] = 0;
				}

				// 선공 방지 효과.
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK) )
				{
					IsHaveBufType[4] = 0;
					BufCreateRate_Const[4] = 0;
				}

				// 집중 총명탕.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUCCESS_MANUFACTURE) )
				{
					IsHaveBufType[5] = 0;
					BufCreateRate_Const[5] = 0;
				}

				// 네잎크로버 효과.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF) )
				{
					IsHaveBufType[6] = 0;
					BufCreateRate_Const[6] = 0;
				}

				// 사랑의 캔디 효과.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) )
				{
					IsHaveBufType[7] = 0;
					BufCreateRate_Const[7] = 0;
				}

				// 우정의 캔디 효과.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) )
				{
					IsHaveBufType[8] = 0;
					BufCreateRate_Const[8] = 0;
				}

				// 레인보우 케이크 효과.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) )
				{
					IsHaveBufType[9] = 0;
					BufCreateRate_Const[9] = 0;
				}

				// 소환수 영양제 효과.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP) )
				{
					IsHaveBufType[10] = 0;
					BufCreateRate_Const[10] = 0;
				}

				// 새참 효과.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FARMINGNMININGFOREVER) )
				{
					IsHaveBufType[11] = 0;
					BufCreateRate_Const[11] = 0;
				}

				SI16 BufRateConstTotal = 0;		// 상수들의 총 합.

				//상수들의 합을 구한다.
				for( int j = 0; j < 12; j++)
				{
					BufRateConstTotal = BufRateConstTotal + BufCreateRate_Const[j];
				}

				SI16 SelectedValue = 0;								//상수 총합 만큼의 범위중 하나의 값을 선택

				SelectedValue = rand() % BufRateConstTotal +  1;	//뽑힌값의 범위는 ( 1 ~ 상수총합)

				//뽑은 값이 어떤 버프의 확률에 포함되는지 알기 위해 상수값을 차례로 누적해가며 체크한다.
				SI16 tmpConstTotal = 0;

				for( int k = 0; k < 12; ++k)
				{
					tmpConstTotal  = tmpConstTotal + BufCreateRate_Const[k];
					if( SelectedValue <= tmpConstTotal) // 선택된 값이 더해진 버프의 상수값의 범위에 포함된다면.
					{
						//어떤 버프가 나와야 하는지 버프의 인덱스를 갖도록 값을 변경.
						SelectedValue = k; // 적용될 버프 인덱스!
						break;
					}
				}

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( IsHaveBufType[SelectedValue], false );


				SI16 siPercent = 0; //타입에 따라 설정.


				switch(IsHaveBufType[SelectedValue])
				{
				case BUF_MAKEEXPUP:
					siPercent = 50;
					break;
				case BUF_ATTACKUP:
				case BUF_DEFENSEUP:
				case BUF_SUMMONATACUP:	
					siPercent = 20;
					break;
				case BUF_PREMIUMEXPUP:
				case BUF_NOFIRSTATTACK:						
				case BUF_SUCCESS_MANUFACTURE:
				case BUF_FOURLEAF:
					siPercent = 0;						
					break;
				}

				if( index > -1 ) // 버프가 있을때
				{
					stBuf addBuf(IsHaveBufType[SelectedValue], siPercent , usetime, BUF_GET_TYPE_RUMBLECHERRY);
					AddBufThroughDB( index, &addBuf , true, itempos, &clitem);
				}
				return false;
			}

			//[추가 : 보스 와일드 보어 이벤트로 송편아이템 이용, 황진성 2007.9.18]
			// 제한적인 아이템으로 이벤트시(솔잎아이템) 외에는 만들 수 없는 아이템이므로 국가 코드 생략.
			else if( itemunique == ITEMUNIQUE(3730))
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 ItemUnique = 0;	
				ItemUnique = GetUniqueWithRange( ItemUnique_3730_ItemNum, sizeof(ItemUnique_3730_ItemNum) / sizeof(cltUniqueRange), 100);

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				switch(ItemUnique)
				{
				case 15900: usedate = 30;	break;		// 가방	7일로 되어있어서 30일로 수정
				case 18110: usedate = 7;	break;		// 빨간망토
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				SI32 ItemCnt = 0;
				switch(ItemUnique)
				{
				case 5620:	ItemCnt = 1;	break;
				case 24040: ItemCnt = 5;	break;
				case 15900: ItemCnt = 1;	break;
				case 18110: ItemCnt = 1;	break;
				case 2575:	ItemCnt = 30;	break;
				case 2580:	ItemCnt = 15;	break;
				case 24015: ItemCnt = 2;	break;
				case 24000: ItemCnt = 5;	break;
				}
				clDrawItem.SetItemNum( ItemCnt );

				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}

				return false;	
			}
			// [춘기] 수박버프
			else if(itemunique == ITEMUNIQUE(3927))
			{
				if ( itempos <= 0 || itempos  >= MAX_ITEM_PER_PERSON)	return false;

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.

				SI16 Index = pclCI->clBI.clBufInfo.FindBufSlot(BUF_WATERMELON);
				if(0 > Index)
					Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_WATERMELON, false);	

				// 기존에 수박 버프가 걸려있으면 지워준다.
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_WATERMELON) )
				{
					stBuf DelBuf( pclCI->clBI.clBufInfo.m_stBuf[Index].siBufType, 0, 0, BUF_WATERMELON );
					sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), Index, &DelBuf );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					// 새롭게 다시 시간을 세팅해 준다.
					stBuf addBuf(BUF_WATERMELON, 0, usetime);
					AddBufThroughDB( Index, &addBuf , true , itempos , &clitem );
				}
				else
				{
					stBuf addBuf(BUF_WATERMELON, 0, usetime);
					AddBufThroughDB( Index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}
			else if( ITEMUNIQUE(3928) == itemunique )
			{
				if ( NULL == pclSoulGuard )
				{
					return false;
				}

				if ( TRUE == pclSoulGuard->IsEmpty() )
				{
					return false;
				}

				SI16 siSetFellowShip = pclSoulGuard->GetFellowShip() + 10;
				if ( Const_Max_SoulGuard_FellowShip < siSetFellowShip )
				{
					siSetFellowShip = Const_Max_SoulGuard_FellowShip;
				}

				// 캐릭터 먼저 설정해 주고
				pclSoulGuard->SetFellowShip( siSetFellowShip );

				// DB에 보낸다
				sDBRequest_SoulGuard_FellowShipSet clMsg( pclCI->GetPersonID(), pclSoulGuard->GetSoulGuardID(), pclSoulGuard->GetFellowShip() );
				pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

			}
			// PCK : 야시장 음식 추가(망고빙수, 타코야끼, 볶음국수, 솜사탕, 떡꼬치, 바나나큐 - 09.08.04)
			// 1. 망고빙수
			else if( itemunique == ITEMUNIQUE(3934) )
			{
				// 아라퀘스트 트리케라몬 변신 퀘스트 수행 중에도 야시장 음식 사용할 수 있도록 변경으로 주석처리.
				//if( pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS) ) return false;

				SI32 id = GetID(); 

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24;		//현실시간 48분간

				SI32		siCurKind = 0;
				SI16		siCharKind[7];
				SI16		siPercent[7];
				SI32		siCharTransCount = 0;
				SI32		siKind = 0;

				if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM))
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM);
					if(pclBuf)
					{
						siCurKind = pclBuf->siPercent;
					}
				}

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FRUITBROS");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FROKEN");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FROZENKNIGHT");
				siPercent[siCharTransCount] = 150;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FROZENSORCERER");
				siPercent[siCharTransCount] = 100;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_ICEHOUND");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_MARIONETTE");
				siPercent[siCharTransCount] = 100;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FROZENQUEEN");
				siPercent[siCharTransCount] = 50;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				SI32 siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];
				}

				SI32 siRand = rand() % siTotalPercent;
				SI32 siSelected = -1;
				siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];		
					if( siRand < siTotalPercent )
					{
						siSelected = i;
						break;
					}
				}

				siKind = siCharKind[siSelected];

				SI16 Index = -1;
				Index = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );				

				// 기존에 변신 버프가 걸려있으면 지워준다.
				if( -1 >= Index )
				{
					Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
				}

				stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
				AddBufThroughDB( Index, &addBuf , true, itempos, &clitem);

				return false;
			}
			// 2. 타코야끼 (변신 스피릿 중 하나로 변신)
			else if( itemunique == ITEMUNIQUE(3931)  )
			{
				// 아라퀘스트 트리케라몬 변신 퀘스트 수행 중에도 야시장 음식 사용할 수 있도록 변경으로 주석처리.
				//if( pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS) ) return false;

				SI32 id = GetID();

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24;		//현실시간 48분간

				SI32		siCurKind = 0;
				SI16		siCharKind[13];
				SI16		siPercent[13];
				SI32		siCharTransCount = 0;
				SI32		siKind = 0;

				if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM))
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM);
					if(pclBuf)
					{
						siCurKind = pclBuf->siPercent;
					}
				}

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_PYEONGGANG");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_ONDAL");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_NINJAMAN");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_NINJAGIRL");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_CATMELEE");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_CATRANGE");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE");
				siPercent[siCharTransCount] = 80;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_OCTOPUS");
				siPercent[siCharTransCount] = 40;
				if(siCharKind[siCharTransCount] > 0 && siCurKind != siCharKind[siCharTransCount])	siCharTransCount++;

				SI32 siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];
				}

				SI32 siRand = rand() % siTotalPercent;
				SI32 siSelected = -1;
				siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];		
					if( siRand < siTotalPercent )
					{
						siSelected = i;
						break;
					}
				}

				siKind = siCharKind[siSelected];

				SI16 Index = -1;
				Index = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );				

				// 기존에 변신 버프가 걸려있으면 지워준다.
				if( -1 >= Index )
				{
					Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
				}

				stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
				AddBufThroughDB( Index, &addBuf , true, itempos, &clitem);
				return false;
			}
			// 3. 볶음국수
			else if( itemunique == ITEMUNIQUE(3930) )
			{
				// 아라퀘스트 트리케라몬 변신 퀘스트 수행 중에도 야시장 음식 사용할 수 있도록 변경으로 주석처리.
				//if( pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS) ) return false;

				SI32 id = GetID();

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24;		//현실시간 48분간

				SI32		siCurKind = 0;
				SI16		siCharKind[7];
				SI16		siPercent[7];
				SI32		siCharTransCount = 0;
				SI32		siKind = 0;

				if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM))
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM);
					if(pclBuf)
					{
						siCurKind = pclBuf->siPercent;
					}
				}

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_MUSHROOM");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_WILDBOAR");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_CATFISH");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_SHARK");
				siPercent[siCharTransCount] = 150;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_STONECOW");
				siPercent[siCharTransCount] = 100;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_RACCOONDOG");
				siPercent[siCharTransCount] = 100;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_OTOHIME");
				siPercent[siCharTransCount] = 50;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;


				SI32 siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];
				}

				SI32 siRand = rand() % siTotalPercent;
				SI32 siSelected = -1;
				siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];		
					if( siRand < siTotalPercent )
					{
						siSelected = i;
						break;
					}
				}

				siKind = siCharKind[siSelected];

				SI16 Index = -1;
				Index = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );				

				// 기존에 변신 버프가 걸려있으면 지워준다.
				if( -1 >= Index )
				{
					Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
				}

				stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
				AddBufThroughDB( Index, &addBuf , true, itempos, &clitem);

				return false;
			}
			// 4. 솜사탕 (탈것의 외관이 랜덤으로 바뀜)
			else if( itemunique == ITEMUNIQUE(3929))
			{
				SI32 id = GetID();

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24;		//현실시간 48분간

				SI32		siCurKind = 0;
				SI16		siCharKind[20];
				SI16		siPercent[20];
				SI32		siCharTransCount = 0;
				SI32		siKind = 0;

				if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM_HORSE))
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM_HORSE);
					if(pclBuf)
					{
						siCurKind = pclBuf->siPercent;
					}
				}

				siCharKind[siCharTransCount] = HORSEUNIQUE_BROWN;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_BLACK;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_WHITE;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_RED;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_RUDOLF;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_TIGER;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_THOUSAND;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_STEEL;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_WOLF;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_ZEBRA;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_BEAST;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_FOAL;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_CAMEL;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_TIGER2;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_ELEPHANT;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_WHITETIGER;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_MOTORCYCLE;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_FIREHATCH;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = HORSEUNIQUE_WHITE_TEST;
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				//siCharKind[siCharTransCount] = HORSEUNIQUE_BAPHOMET;
				//siPercent[siCharTransCount] = 47;
				//if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_RACCOON");
				siPercent[siCharTransCount] = 47;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				SI32 siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];
				}

				SI32 siRand = rand() % siTotalPercent;
				SI32 siSelected = -1;
				siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];		
					if( siRand < siTotalPercent )
					{
						siSelected = i;
						break;
					}
				}

				siKind = siCharKind[siSelected];

				SI16 Index = -1;
				SI16 siIndexTransForm = -1;

				Index			 = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM_HORSE );				
				siIndexTransForm = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );

				cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if( pclHorse == NULL || pclHorse->siHorseUnique <= 0 )
				{
					return false;
				}

				// 기존에 변신 버프가 걸려있거나 말에 탄상태가 아니면 솜사탕 사용을 못한다.
				if( siIndexTransForm > -1 || pclClient->pclCM->CR[id]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == false)
				{
					return false;
				}

				// 라쿤이 걸리면 말변신이 아닌 캐릭터 변신을 한다.
				if( siKind == pclClient->GetUniqueFromHash("KIND_RACCOON") )
				{
					// 말변신이 걸린 상태라면 말변신을 풀고 라쿤 변신을 한다.
					if(Index > -1)
					{
						stBuf DelBuf( pclCI->clBI.clBufInfo.m_stBuf[Index].siBufType, 0, 0, BUF_TRANSFORM_HORSE );
						sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), Index, &DelBuf );
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

						siIndexTransForm = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);

						// 새롭게 다시 시간을 세팅해 준다.
						stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
						AddBufThroughDB( siIndexTransForm, &addBuf , true, itempos, &clitem );
					}
					else
					{
						siIndexTransForm = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);

						// 새롭게 다시 시간을 세팅해 준다.
						stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
						AddBufThroughDB( siIndexTransForm, &addBuf , true, itempos, &clitem);
					}
				}
				else
				{
					if( -1 >= Index )
					{
						Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM_HORSE, false);	
					}

					stBuf addBuf(BUF_TRANSFORM_HORSE, siKind, usetime);
					AddBufThroughDB( Index, &addBuf, true, itempos, &clitem);
				}

				return false;
			}
			// 5. 떡꼬치
			else if( itemunique == ITEMUNIQUE(3932))
			{
				// 아라퀘스트 트리케라몬 변신 퀘스트 수행 중에도 야시장 음식 사용할 수 있도록 변경으로 주석처리.
				//if( pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS) ) return false;

				SI32 id = GetID();

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24;		//현실시간 48분간

				SI32		siCurKind = 0;
				SI16		siCharKind[7];
				SI16		siPercent[7];
				SI32		siCharTransCount = 0;
				SI32		siKind = 0;

				if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM))
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM);
					if(pclBuf)
					{
						siCurKind = pclBuf->siPercent;
					}
				}

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_TIGER");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_COW");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_LADYBUG");
				siPercent[siCharTransCount] = 200;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_RABBIT");
				siPercent[siCharTransCount] = 150;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_HARPY");
				siPercent[siCharTransCount] = 100;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_WILDCAT");
				siPercent[siCharTransCount] = 100;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_MANDRAGORA");
				siPercent[siCharTransCount] = 50;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				SI32 siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];
				}

				SI32 siRand = rand() % siTotalPercent;
				SI32 siSelected = -1;
				siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];		
					if( siRand < siTotalPercent )
					{
						siSelected = i;
						break;
					}
				}

				siKind = siCharKind[siSelected];

				SI16 Index = -1;
				Index = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );				

				// 기존에 변신 버프가 걸려있으면 지워준다.
				if( -1 >= Index )
				{
					Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
				}

				stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
				AddBufThroughDB( Index, &addBuf , true, itempos, &clitem);

				return false;
			}
			// 6. 바나나큐
			else if( itemunique == ITEMUNIQUE(3933) )
			{
				// 아라퀘스트 트리케라몬 변신 퀘스트 수행 중에도 야시장 음식 사용할 수 있도록 변경으로 주석처리.
				//if( pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS) ) return false;

				SI32 id = GetID();

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24;		//현실시간 48분간

				SI32		siCurKind = 0;
				SI16		siCharKind[6];
				SI16		siPercent[6];
				SI32		siCharTransCount = 0;
				SI32		siKind = 0;

				if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM))
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM);
					if(pclBuf)
					{
						siCurKind = pclBuf->siPercent;
					}
				}

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_BEARTANG");
				siPercent[siCharTransCount] = 190;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_SUPERBEARTANG");
				siPercent[siCharTransCount] = 190;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_CATPET");
				siPercent[siCharTransCount] = 190;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_RUDOLPHPET");
				siPercent[siCharTransCount] = 190;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_MINEGIRL");
				siPercent[siCharTransCount] = 190;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				siCharKind[siCharTransCount] = pclClient->GetUniqueFromHash("KIND_COSMOSRABBIT2");
				siPercent[siCharTransCount] = 50;
				if(siCharKind[siCharTransCount] > 0 )	siCharTransCount++;

				SI32 siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];
				}

				SI32 siRand = rand() % siTotalPercent;
				SI32 siSelected = -1;
				siTotalPercent = 0;

				for(SI32 i=0; i<siCharTransCount; i++)
				{
					siTotalPercent += siPercent[i];		
					if( siRand < siTotalPercent )
					{
						siSelected = i;
						break;
					}
				}

				siKind = siCharKind[siSelected];

				SI16 Index = -1;
				Index = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );				

				// 기존에 변신 버프가 걸려있으면 지워준다.
				if( -1 >= Index )
				{
					Index = pclCI->clBI.clBufInfo.FindEmptySlot(BUF_TRANSFORM, false);	
				}

				stBuf addBuf(BUF_TRANSFORM, siKind, usetime, BUF_GET_TYPE_NIGHTMARKET);
				AddBufThroughDB( Index, &addBuf , true, itempos, &clitem);

				return false;
			}

			// 말용 아이템인 경우, 
			else if(pclItemManager->pclItemInfo[ref]->siHorseFoodSwitch == 1)
			{
				// 말이 없으면 사용할 수 없다. 
				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if( pclhorse == NULL || pclhorse->siHorseUnique < 0 || 
					pclhorse->siHorseUnique == HORSEUNIQUE_WOLF )
				{
					return false;
				}

				// 말 먹이를 준다. 
				if(pclClient->pclHorseManager->GiveFood(pclhorse, &clitem) == true)
				{
					// 클라이언트로 말 정보를 갱신한다. 
					SetUpdateSwitch(UPDATE_HORSE, true, 0);

					// DB에 저장한다. 
					UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse, pclhorse, SETHORSE_REASON_UPDATE);

					DoQuestCheckAndAction(QUEST_CLEAR_COND_FEEDHORSE1);
				}
			}
			else if(pclItemManager->pclItemInfo[ref]->siHorseFoodSwitch == 2)
			{
				// 말이 없으면 사용할 수 없다. 
				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if( pclhorse == NULL || pclhorse->siHorseUnique < 0 || 
					pclhorse->siHorseUnique == HORSEUNIQUE_WOLF )
				{
					return false;
				}

				// 말 먹이를 준다. 
				if(pclClient->pclHorseManager->GiveFood(pclhorse, &clitem) == true)
				{
					// 클라이언트로 말 정보를 갱신한다. 
					SetUpdateSwitch(UPDATE_HORSE, true, 0);

					// DB에 저장한다. 
					UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse, pclhorse, SETHORSE_REASON_UPDATE);

					DoQuestCheckAndAction(QUEST_CLEAR_COND_FEEDHORSE1);
				}
			}
			else if(pclItemManager->pclItemInfo[ref]->siHorseFoodSwitch == 3)
			{
				// 말이 없으면 사용할 수 없다. 
				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if( pclhorse == NULL || pclhorse->siHorseUnique < 0 || 
					pclhorse->siHorseUnique == HORSEUNIQUE_WOLF )
				{
					return false;
				}

				// 말 먹이를 준다. 
				if(pclClient->pclHorseManager->GiveFood(pclhorse, &clitem) == true)
				{
					// 클라이언트로 말 정보를 갱신한다. 
					SetUpdateSwitch(UPDATE_HORSE, true, 0);

					// DB에 저장한다. 
					UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse, pclhorse, SETHORSE_REASON_UPDATE);

					DoQuestCheckAndAction(QUEST_CLEAR_COND_FEEDHORSE2);
				}
			}
			else if ( pclItemManager->pclItemInfo[ref]->bFishingBaitSwitch == true )
			{
				// 사용 불가인 경우 return false 하면 됨.

				// 근처에 낚시할 곳이 없다면 사용할 수 없다.				
				SI32 dir = 0;
				if(IsFishArea(GetX(), GetY(), &dir) == true)
				{
					SI16 FishKindListRef = pclClient->pclFishingMgr->IsAvailableBait( clitem.siUnique );

					if ( FishKindListRef < 0)
					{
						FishingStop();
						return false;
					}

					// 해당 낚시터에서 낚이는 물고기와 현재 미끼를 비교해서 서로 맞지 않으면 낚시를 하지 않는다. (PCK : 08.04.03)

					if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
					{
						cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

						SI32 XPos = pclchar->pclCI->clBI.GetX();
						SI32 YPos = pclchar->pclCI->clBI.GetY();
						SI16 FishKindList[MAX_FISHKIND_NUMBER];
						memset( FishKindList, 0, sizeof( SI16 ) * MAX_FISHKIND_NUMBER );
						SI16 Counter = 0;

						for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
						{
							if ( pclClient->pclFishingMgr->m_pFishKindList[i] != NULL )
							{
								if(pclClient->pclFishingMgr->m_pFishKindList[i]->IsAvailableGainFish(XPos,YPos))
								{
									++Counter;
									FishKindList[Counter] = pclClient->pclFishingMgr->m_pFishKindList[i]->GetFishItemUnique();
								}

								if(pclClient->pclFishingMgr->m_pFishKindList[i]->IsSameFishItemUnique(FishKindList[Counter]) == TRUE)
								{
									if(pclClient->pclFishingMgr->m_pFishKindList[i]->IsAvailableBait(clitem.siUnique) == FALSE)
									{
										FishingStop();

										// 미끼의 종류에 따라서 나오는 메시지를 다르게 설정한다.
										if (clitem.siUnique == ITEMUNIQUE(16010))	// 갯지렁이
										{
											((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHNOTBAITSHRIMP, ITEMUNIQUE(16140), 0, GetCharUnique());
											return FALSE;
										}
										else if (clitem.siUnique == ITEMUNIQUE(16140))	// 새우
										{
											((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHNOTBAITLUGWORM, ITEMUNIQUE(16010), 0, GetCharUnique());
											return FALSE;
										}
									}
								}
							}
						}

						if(Counter <= 0)
						{
							FishingStop();
							((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_NOPROPERFISHAREA, 0, 0, GetCharUnique());
							return FALSE;
						}
					}
					else
					{
						//[진성] 물이면 아무곳에서나 낚시 모드가 되는것 수정. 리소스상의 낚시지역 확인. => 2008-9-4
						SI16 Counter = 0;
						for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
						{
							if ( pclClient->pclFishingMgr->m_pFishKindList[i] != NULL )
							{
								if( pclClient->pclFishingMgr->m_pFishKindList[i]->IsAvailableGainFish( GetX(), GetY() ) )
								{
									++Counter;
								}
							}
						}

						if(Counter <= 0)
						{
							FishingStop();
							((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_NOPROPERFISHAREA, 0, 0, GetCharUnique());
							return FALSE;
						}
					}

					if ( ! IsAvailableFishing( clitem.siUnique, FishKindListRef ) )
					{
						FishingStop();
						return false;
					}

					// 낚시 도구가 현재 사용 가능 한지 체크
					DBToolInfo clFishingToolList[ MAX_FISHING_TOOL_NUMBER ];

					for ( SI16 kk = 0; kk < MAX_FISHING_TOOL_NUMBER; ++kk )
					{
						clFishingToolList[ kk ].Init();
					}

					if ( ! GetFishingToolInfo( FishKindListRef, clFishingToolList ) )
					{
						FishingStop();
						return false;
					}						

					// 미끼를 이 아이템으로 하고 낚시를 한다.
					siFishingBaitItemUnique = clitem.siUnique;

					// 낚시대 던지자 마자 바로 못 낚게 시간 제한 줘야지..
					dwPreCatchFishClock = pclClient->CurrentClock;

					// 낚시 도구의 내구도를 떨어 드린다.
					sDBRequest_FishingDecreaseToolDurability dbMsg( pclCI->GetPersonID(), GetID(), clFishingToolList );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&dbMsg);

					cltGameMsgResponse_FishingStart clMsgResponse_FishingStart( siFishingBaitItemUnique, clFishingToolList );
					cltMsg clMsg( GAMEMSG_RESPONSE_FISHINGSTART, sizeof( clMsgResponse_FishingStart ), (BYTE*)&clMsgResponse_FishingStart );
					SendNetMsg((sPacketHeader*)&clMsg );

					// 서버를 낚시하게 만든다. 
					OrderFish(GetID(), dir, BY_PLAYER);						
				}
				else
				{
					// 클라이언트에 사실을 알린다. 
					((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_NOPROPERFISHAREA,  0, 0, GetCharUnique());
					return false;
				}					
			}
			else
			{

				SI32 max = MAXORG(pclCI->clIP.GetLevel());
				UI32 hungryrecover = clitem.GetHungry(pclClient->pclItemManager);

				//------------------------------------
				// 배고픔의 정상 여부. 
				//------------------------------------
				bool bnormalswitch = false; 
				if(clPB.clHealth.GetHungryStatus(pclCI->clIP.GetLevel()) & (HEALTH_STATUS_HUNGRY_NORMAL|HEALTH_STATUS_HUNGRY_7|HEALTH_STATUS_HUNGRY_8|HEALTH_STATUS_HUNGRY_9))
				{
					bnormalswitch = true;
				}

				// 포만감을 회복시켜준다.
				clPB.clHealth.IncreaseHungry(hungryrecover);

				// 소화기관에 영향을 준다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty)
				{
					if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty < 0 || bnormalswitch)
					{

						SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg1Penalty / 10;
						SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg1Penalty) % 10;

						pclCI->clHealthInfo.IncreaseInnerOrg1(penalty);

						if((rand() % 10) < modul)
						{
							if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty > 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg1(1);
							}
							if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty < 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg1(-1);
							}
						}

						// 최대값 범위를 벗어나지 않는다. 
						if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG1) > max)
						{
							pclCI->clHealthInfo.siInnerOrg1 = max;
						}
						else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG1) < -max)
						{
							pclCI->clHealthInfo.siInnerOrg1 = -max;
						}

						SetUpdateSwitch(UPDATE_HEALTH, true, 0);
					}
				}

				// 순환기관에 영향을 준다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty)
				{
					if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty < 0 || bnormalswitch)
					{
						SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg2Penalty / 10;
						SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg2Penalty) % 10;

						pclCI->clHealthInfo.IncreaseInnerOrg2(penalty);

						if((rand() % 10) < modul)
						{
							if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty > 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg2(1);
							}
							if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty < 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg2(-1);
							}
						}

						// 최대값 범위를 벗어나지 않는다. 
						if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG2) > max)
						{
							pclCI->clHealthInfo.siInnerOrg2 = max;
						}
						else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG2) < -max)
						{
							pclCI->clHealthInfo.siInnerOrg2 = -max;
						}

						SetUpdateSwitch(UPDATE_HEALTH, true, 0);
					}
				}
				// 호흡기관에 영향을 준다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty)
				{
					if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty < 0 || bnormalswitch)
					{

						SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg3Penalty / 10;
						SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg3Penalty) % 10;

						pclCI->clHealthInfo.IncreaseInnerOrg3(penalty);

						if((rand() % 10) < modul)
						{
							if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty > 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg3(1);
							}
							if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty < 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg3(-1);
							}
						}

						// 최대값 범위를 벗어나지 않는다. 
						if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG3) > max)
						{
							pclCI->clHealthInfo.siInnerOrg3 = max;
						}
						else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG3) < -max)
						{
							pclCI->clHealthInfo.siInnerOrg3 = -max;
						}


						SetUpdateSwitch(UPDATE_HEALTH, true, 0);
					}
				}
				// 신경기관에 영향을 준다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty)
				{
					if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty < 0 || bnormalswitch)
					{

						SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg4Penalty / 10;
						SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg4Penalty) % 10;

						pclCI->clHealthInfo.IncreaseInnerOrg4(penalty);

						if((rand() % 10) < modul)
						{
							if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty > 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg4(1);
							}
							if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty < 0)
							{
								pclCI->clHealthInfo.IncreaseInnerOrg4(-1);
							}

						}

						// 최대값 범위를 벗어나지 않는다. 
						if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG4) > max)
						{
							pclCI->clHealthInfo.siInnerOrg4 = max;
						}
						else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG4) < -max)
						{
							pclCI->clHealthInfo.siInnerOrg4 = -max;
						}


						SetUpdateSwitch(UPDATE_HEALTH, true, 0);
					}
				}


				SI16 questtype, unique ,para1, para2;

				if( pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_HUNGRY ,&questtype, &unique, &para1, &para2 ))
				{
					if( clitem.siUnique == para1 && 
						questtype >= 0 && questtype < MAX_QUEST_TYPE )   // 퀘스트 아이템과 비교 후 배부름 퀘스트 진행
					{
						pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 = 1;
						SetUpdateSwitch(UPDATE_QUEST, true, 0);
					}
				}				

				// 클라이언트에게 음식 섭취로 배가 불렀음을 통보한다. 
				cltGameMsgResponse_IncHungry clInfo(clitem.siUnique, clPB.clHealth.pclInfo->siHungry);
				cltMsg clMsg(GAMEMSG_REPONSE_INCHUNGRY, sizeof(clInfo), (BYTE*)&clInfo);
				SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;

		//==============================================================================================================================
		// HEAL
		//==============================================================================================================================
	case ITEMTYPE_HEAL:
		{
			//[진성] 마력의 비약은 그냥 우클릭으로 사용 못하게 막음.
			if( ITEMUNIQUE(2789) == clitem.siUnique )	return false;

			//KHY - 0115 발렌타인데이 이벤트.
			if( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_BLACKCACAORANDOM ) //블랙 카카오.
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;	

				// 기본 현재 상태를 찾아 설정한다.
				SI16 IsHaveBufType[8] = {	
					BUF_PREMIUMEXPUP,			// 성장비약.//열혈EXP 포션을 쓰면 사용불가
						BUF_ATTACKUP,				// 공격력 증가.
						BUF_DEFENSEUP,				// 방어력 증가.
						BUF_MAKEEXPUP,				// 제조 경험치 증가.
						BUF_NOFIRSTATTACK,			// 선공 방지 효과.
						BUF_SUCCESS_MANUFACTURE,	// 제조 100% 효과가 있다면
						BUF_FOURLEAF,				// 네잎클로버 - 아이템 드랍률 2배.
						BUF_SUMMONATACUP};			// 소환수 강화. 


					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP) ||pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) // 성장비약.//열혈EXP 포션을 쓰면 사용불가
						IsHaveBufType[0] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ||  // 공격력 증가.
						pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP30) ) 
						IsHaveBufType[1] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ||  // 방어력 증가.
						pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP30) ) 
						IsHaveBufType[2] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_MAKEEXPUP) )  // 제조 경험치 증가.
						IsHaveBufType[3] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK) )  // 선공 방지 효과.
						IsHaveBufType[4] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUCCESS_MANUFACTURE) )  //제조 100% 효과가 있다면
						IsHaveBufType[5] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF) )  // 네잎클로버 - 아이템 드랍률 2배.
						IsHaveBufType[6] = 0;

					if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ||pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP))  //소환수 강화.
						IsHaveBufType[7] = 0;


					SI16 SelectBufType[8] = {0,}; // 선택 가능한 버프를 저장한다.
					SI16 noneSelect = 0;


					for(int i = 0; i < 8 ; i++)
					{
						if(IsHaveBufType[i] > 0)
						{
							SelectBufType[noneSelect] = IsHaveBufType[i] ;
							noneSelect++;
						}
					}

					if(noneSelect == 0) // 걸만한 버프가 없다.
						return false ;

					SI32 siRand = 0 ;
					siRand = rand()%noneSelect;  // 버프 종류를 선택한다.

					SI16 Buftype = SelectBufType[siRand];

					SI16 siPercent = 0; //타입에 따라 설정.

					switch(Buftype)
					{
					case BUF_MAKEEXPUP:
						siPercent = 50;
						break;
					case BUF_ATTACKUP:
					case BUF_DEFENSEUP:
					case BUF_SUMMONATACUP:	
						siPercent = 20;
						break;
					case BUF_PREMIUMEXPUP:
					case BUF_NOFIRSTATTACK:						
					case BUF_SUCCESS_MANUFACTURE:
					case BUF_FOURLEAF:
						siPercent = 0;						
						break;
					}

					SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;  // 1일  = 48분
					SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( Buftype, false );

					if ( index > -1 )
					{
						stBuf addBuf(Buftype, siPercent, usetime);
						AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
					}

					return false ;

			}
			//KHY - 0218 화이트데이 이벤트.
			else if( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_CANDYRANDOM ) //블랙 카카오.
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;	

				// 기본 현재 상태를 찾아 설정한다.
				SI16 IsHaveBufType[8] = {BUF_PREMIUMEXPUP, // 성장비약.//열혈EXP 포션을 쓰면 사용불가
					BUF_ATTACKUP,  // 공격력 증가.
					BUF_DEFENSEUP,  // 방어력 증가.
					BUF_MAKEEXPUP,  // 제조 경험치 증가.
					BUF_NOFIRSTATTACK,  // 선공 방지 효과.
					BUF_SUCCESS_MANUFACTURE,  //제조 100% 효과가 있다면
					BUF_FOURLEAF, // 네잎클로버 - 아이템 드랍률 2배.
					BUF_SUMMONATACUP};//소환수 강화. 


				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP) ||pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) // 성장비약.//열혈EXP 포션을 쓰면 사용불가
					IsHaveBufType[0] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ||  // 공격력 증가.
					pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP30) ) 
					IsHaveBufType[1] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ||  // 방어력 증가.
					pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) || pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP30) ) 
					IsHaveBufType[2] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_MAKEEXPUP) )  // 제조 경험치 증가.
					IsHaveBufType[3] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK) )  // 선공 방지 효과.
					IsHaveBufType[4] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUCCESS_MANUFACTURE) )  //제조 100% 효과가 있다면
					IsHaveBufType[5] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF) )  // 네잎클로버 - 아이템 드랍률 2배.
					IsHaveBufType[6] = 0;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ||pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP))  //소환수 강화.
					IsHaveBufType[7] = 0;


				SI16 SelectBufType[8] = {0,}; // 선택 가능한 버프를 저장한다.
				SI16 noneSelect = 0;

				for(int i = 0; i < 8 ; i++)
				{
					if(IsHaveBufType[i] > 0)
					{
						SelectBufType[noneSelect] = IsHaveBufType[i] ;
						noneSelect++;
					}
				}

				if(noneSelect == 0) // 걸만한 버프가 없다.
					return false ;

				SI32 siRand = 0 ;
				siRand = rand()%noneSelect;  // 버프 종류를 선택한다.

				SI16 Buftype = SelectBufType[siRand];

				SI16 siPercent = 0; //타입에 따라 설정.

				switch(Buftype)
				{
				case BUF_MAKEEXPUP:
					siPercent = 50;
					break;
				case BUF_ATTACKUP:
				case BUF_DEFENSEUP:
				case BUF_SUMMONATACUP:	
					siPercent = 20;
					break;
				case BUF_PREMIUMEXPUP:
				case BUF_NOFIRSTATTACK:						
				case BUF_SUCCESS_MANUFACTURE:
				case BUF_FOURLEAF:
					siPercent = 0;						
					break;
				}

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;  // 1일  = 48분
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( Buftype, false );

				if ( index > -1 )
				{
					stBuf addBuf(Buftype, siPercent, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;

			}			
			else if( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HEROATTACKUP )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP30) ) return false ;

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				
				// 누적 가능으로 변경되어 
				stBuf addBuf(BUF_ATTACKUP, 20, usetime);
				AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );

				//SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_ATTACKUP, false );

				//if ( index > -1 )
				//{
				//	stBuf addBuf(BUF_ATTACKUP, 20, usetime);
				//	//AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				//	AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );
				//}

				return false ;
			}
			else if( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HEROATTACKUP30 )
			{//전투력 증가약 30일간[2007.08.30 손성웅]
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_ATTACKUP) ) return false ;

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600;
				//21600 ;//아직 시간 설정이 안됐음 테스트 위해 짧게잡음
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_ATTACKUP30, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_ATTACKUP30, 20, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}
			// 방어력 증가 약
			else if( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HERODEFENSEUP )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP30) ) return false ;



				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;

				// 누적 가능으로 변경되 처리 함수 변경
				stBuf addBuf(BUF_DEFENSEUP, 20, usetime);
				AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );

				//SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_DEFENSEUP, false );

				//if ( index > -1 )
				//{
				//	stBuf addBuf(BUF_DEFENSEUP, 20, usetime);
				//	//AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				//	 AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );
				//}

				return false ;
			}
			else if( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HERODEFENSEUP30 )
			{//방어력 증가약 30일간[2007.08.30 손성웅]
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_DEFENSEUP) ) return false ;



				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;//21600 30일 시간. ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_DEFENSEUP30, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_DEFENSEUP30, 20, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}//방어력 증가약 30일간[2007.08.30 손성웅]

			// 네잎 클로버			
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_FOURLEAF )
			{
				if(pclClient->IsCountrySwitch(Switch_FourLeaf))
				{
					if( ITEMUNIQUE(2755) == clitem.siUnique || ITEMUNIQUE(2754) == clitem.siUnique	)
					{
						//[진성] 현재 사용가능한 네잎크로버인지 확인. => 2008-8-14
						if( clitem.siUnique != pclserver->siFourLeafUnique )
						{
							pclserver->SendServerResponseMsg( 0, SRVAL_RETURN_NOT_USE_FOURLEAF, 0, 0, GetCharUnique() );
							return false;
						}
					}
				}

				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS) ) return false ;//이관 보너스

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_FOURLEAF, false );

				if ( index > -1 )
				{		
					stBuf addBuf(BUF_FOURLEAF, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;

			}
			// 해피빈 해피타임 [2007.08.22 손성웅]
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HAPPYBEAN )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				SI32 usetime =  0;
				usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 15 ;// 30분간사용				
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_HAPPYBEAN, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_HAPPYBEAN, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}


			//열혈 EXP 캡슐
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SUPEREXPUP )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP) ) return false ;//헬레나 성장비약
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;	//성장캡슐
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ) return false ;	//레인보우
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ;	//우정
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS) ) return false ;	//이관보너스
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300) ) return false ;	// 스페셜 엠플

				SI32 usetime =  0;

				if( itemunique == ITEMUNIQUE(2775) )	// 현실시간  7일 =  7 * 24 = 168시간  30 * 168 =  5040 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 5040 ;
				else if( itemunique == ITEMUNIQUE(2778) )	// 현실시간 15일 = 30 * 24 = 720 /2 시간  30 * 720 = 21600/2 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + (21600/2) ;
				else if( itemunique == ITEMUNIQUE(2780) )	// 현실시간 30일 = 30 * 24 = 720시간  30 * 720 = 21600 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;
				else
					return false ;

				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_SUPEREXPUP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_SUPEREXPUP, 0, usetime);
					/*addBuf.siBufType = BUF_SUPEREXPUP;
					addBuf.siPercent = 0;
					addBuf.siAdd = 0;
					addBuf.siOverTime = usetime;
					addBuf.bOverlap = false;*/

						AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}
			// 특제 파나의 성장 비약			
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_PREMIUMEXPUP )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;  // 인벤이 꽉차있는지 확인.	
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) return false ;// 열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300) ) return false ;	// 스페셜 엠플
				if ( true == pclCI->clBI.bPremiumService ) return false;

				SI32 usetime =  0;

				if( itemunique == ITEMUNIQUE(2757) )		// 현실시간  1일 =  1 * 24 =  24시간  30 *  24 =   720 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 720 ;
				else if( itemunique == ITEMUNIQUE(2759) )	// 현실시간  7일 =  7 * 24 = 168시간  30 * 168 =  5040 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 5040 ;
				else if( itemunique == ITEMUNIQUE(2760) )	// 현실시간 30일 = 30 * 24 = 720시간  30 * 720 = 21600 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;
				else if( itemunique == ITEMUNIQUE(2761) )	// 현실시간 30일 = 30 * 24 = 720시간  30 * 720 = 21600 시간
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;
				else if( itemunique == ITEMUNIQUE(2798) )	// GoonzuEvent Item (ITEMUNIQUE(2757)와 같은 기능을 한다.)
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 720 ;
				else
					return false ;


				// 누적가능으로 변경되어서 처리 함수 변경
				stBuf addBuf(BUF_PREMIUMEXPUP, 0, usetime);
				AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );

				//SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_PREMIUMEXPUP, false );

				//if ( index > -1 )
				//{
				//	stBuf addBuf(BUF_PREMIUMEXPUP, 0, usetime);
				//	/*addBuf.siBufType =	BUF_PREMIUMEXPUP;
				//	addBuf.siPercent = 0;
				//	addBuf.siAdd = 0;
				//	addBuf.siOverTime = usetime;
				//	addBuf.bOverlap = false;*/

				//		//AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				//		AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );

				//}

				return false ;
			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_NOFIRSTATTACK_PREMIUM)
			{
				// [진성] 선공방지 버프를 사용중이면 해당 퀘스트 클리어.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK ) )
					IncQuestPara(QUEST_CLEAR_COND_USEITEM, clitem.siUnique, clitem.siItemNum);

				// 선공방지 효과는 중복해서 사용 할 수 없다.
				//if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK) )	return false ;

				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON )	return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) )	return false ;

				// 이미 60초짜리 라도 쓰고 있다면 쓸 수 없다.
				//if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK) )	return false ;

				SI32 usetime = 0;
				if( ITEMUNIQUE(2792) == clitem.siUnique )
					usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;		//21600 30일 시간.
				else
					usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;			// 현실시간 48분.

				// 누적가능으로 변경되어서 처리 함수 변경
				stBuf addBuf(BUF_NOFIRSTATTACK, 0, usetime);
				AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );


				//SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_NOFIRSTATTACK, false );
				//if ( index > -1 )
				//{
				//	stBuf addBuf(BUF_NOFIRSTATTACK, 0, usetime);
				//	//AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				//	AddBufThroughDB_AccumulateEnable( &addBuf , true , itempos , &clitem );
				//}
				return false ;				
			}

			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_NOFIRSTATTACK)		//	[종호] 이미 선공방지 아이템을 먹은상태라면 60초짜리 선공방지아이템은 중복될수 없다.
			{
				// [진성] 선공방지 버프를 사용중이면 해당 퀘스트 클리어.
				if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK ) )
					IncQuestPara(QUEST_CLEAR_COND_USEITEM, clitem.siUnique, clitem.siItemNum);

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOFIRSTATTACK) )	return false ;
			}

			//소환수 공격력 방어력 증가 (48분짜리(게임시간 하루) 효과가 있다면,
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SUMMONATACUP)
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) ) return false;	// 사랑버프 효과가 더 강하므로 못먹는다.

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_SUMMONATACUP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_SUMMONATACUP, 20, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false ;				
			}

			//제조 100% 효과가 있다면
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SUCCESS_MANUFACTURE)
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_SUCCESS_MANUFACTURE, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_SUCCESS_MANUFACTURE, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false ;				
			}

			// 모후모후 향수 효과가 있다면...
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_MOFUMOFU_PERFUME)
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_MOFUMOFU_PERFUME, false );

				SI16 siKind = pclClient->GetUniqueFromHash("KIND_RABBIT");
				if ( index > -1 )
				{
					stBuf addBuf(BUF_MOFUMOFU_PERFUME, siKind, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}
				return false ;				
			}
			// 이관보상 버프
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_REWARDBONUS )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_MAKEEXPUP) ) return false ;// 제조 경험치 증가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) return false ;//열혈EXP 포션을 쓰면 사용불가
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP) ) return false ;//헬레나 성장비약
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false ;//성장캡슐
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ) return false ;//레인보우
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false ;//우정
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF) ) return false ;// 네잎 클로버
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_NOTBONUSTIME_MAKEEXPUP ) ) return false ;// 네잎 클로버
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SPECIALAMPUL_EXPUP300 ) ) return false ;// 스페셜 엠플

				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 720;	// 현실시간 1일
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_REWARDBONUS, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_REWARDBONUS, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );

					// 해당 아이템을 사용하면, 메시지창이 출력된다.
					pclserver->SendServerResponseMsg(0, SRVAL_ITEMUSE_REWARDBONUS, 8515, 0, GetCharID());
				}

				return false ;				
			}
			// PCK : 제조 프리미엄 아이템
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_PREMIUMMAKEEXPUP )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;			

				SI32 usetime = 0;
				if( itemunique == ITEMUNIQUE(2788) )	// 현실시간  1일 =  1 * 24 = 24시간  30 * 24 =  720 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 720 ;
				else if( itemunique == ITEMUNIQUE(2787) )	// 현실시간 7일 =  7 * 24 = 168시간  30 * 168 =  5040 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 5040 ;
				else if( itemunique == ITEMUNIQUE(2786) )	// 현실시간 30일 = 30 * 24 = 720시간  30 * 720 = 21600 시간 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;
				else
					return false ;

				//SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600;	// 현실시간 30일
				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_PREMIUMMAKEEXPUP, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_PREMIUMMAKEEXPUP, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;				
			}
			// 스페셜 엠플			
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SPECIALAMPUL )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUPEREXPUP) ) return false;	// 열혈 EXP 캡슐
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_PREMIUMEXPUP) ) return false; // 특제 파나의 성장비약(멀티 비타민)
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_HINAMOCHI) ) return false;	// 레인보우케이크
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER) ) return false;	// 하트캔디,러브러브 초콜릿
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP) ) return false;	// 스타캔디,우정의 초코렛
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_REWARDBONUS) ) return false;	// 이관보상 아이템(헬레나의 극상 프리미엄 비약)
				if ( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_RESIDENTS_EXPUP_10) ) return false; // 마을 EXP 캡슐


				SI32 usetime =  0;

				if( itemunique == ITEMUNIQUE(2790) ) 
					usetime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ; // 현실시간 48분간.

				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_SPECIALAMPUL_EXPUP300, false );

				if ( index > -1 )
				{
					stBuf addBuf(BUF_SPECIALAMPUL_EXPUP300, 0, usetime);
					AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
				}

				return false ;
			}
			//------------------------------------------------------------------------------------------------
			// 마을 EXP 캡슐
			//------------------------------------------------------------------------------------------------
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_RESIDENTS_EXPUP_10 )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON )	return false;
				if( itemunique != ITEMUNIQUE( 2801 ) ) return false;

				cltServer* pServer = (cltServer*)pclClient;
				SI32 siMyHomeTownUnique = 0;

				//////////////////////////////////////////////////////////////////////////
				SI32 usetime = 0;															// 일단 걸어줄 버프의 정보를 담은 디폴트 버프를 만들고 (유저에게 실제로 줄때 사용하는게 아닌 줄 버프의 정보를 담아두는 용도로 사용)
				usetime = pServer->pclTime->clDate.GetHourVary() + 24;		// 디폴트 값은 현실시간 48분간 (게임시간1일).
				stBuf addBuf( BUF_RESIDENTS_EXPUP_10, 0, usetime );

				//////////////////////////////////////////////////////////////////////////
				if ( RANKTYPE_CITYHALL == pclCI->clCharRank.siType )						// 아이템 사용한 사람이 영주일때
				{
					siMyHomeTownUnique = pclCI->clCharRank.GetVillageUnique();				// 영주 일땐 랭크 정보에서 마을 유니크를 얻는다. (캐릭터 기본 정보에는 들어있지 않기 때문)
				}
				else																		// 아이템 사용한 사람이 영주가 아닐때
				{
					siMyHomeTownUnique = pclCI->clBI.siHomeVillage;
				}

				//////////////////////////////////////////////////////////////////////////
				if ( true == pServer->pclVillageManager->IsValidVillage( siMyHomeTownUnique ) )
				{
					// 영주 캐릭의 버프 처리 (영주는 clBI.siHomeVillage에 값이 세팅이 안되어 있기 때문에 AddBufToAllTownPerson()함수에서 체크되지 않는다)
					if ( RANKTYPE_CITYHALL == pclCI->clCharRank.siType )					// 아이템 사용한 사람이 영주일때
					{
						if ( true == IsValidConnection() )
						{
							if ( false == pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300) )
							{
								AddTownBufThroughDB( this, &addBuf );
							}
						}
					}
					else																	// 아이템 사용한 사람이 영주가 아닐때
					{
						cltRank* pclRank = pServer->pclRankManager->GetRank( RANKTYPE_CITYHALL, siMyHomeTownUnique );
						if ( NULL != pclRank )
						{
							cltRank clRank;
							SI32 siPersonID = 0;
							clRank.Set( pclRank );

							siPersonID = clRank.clPerson.GetPersonID();
							if ( 0 < siPersonID )
							{
								SI32 siCharID = 0;
								siCharID = pServer->pclCM->GetIDFromPersonID( siPersonID );
								if ( 0 < siCharID )
								{
									cltCharServer* pCityJJang = (cltCharServer*)pServer->pclCM->CR[siCharID];
									if ( NULL != pCityJJang )		// 영주 캐릭터를 찾았다면
									{
										if ( true == pCityJJang->IsValidConnection() )
										{
											if ( false == pCityJJang->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300) )
											{
												pCityJJang->AddTownBufThroughDB( pCityJJang, &addBuf );
											}
										}
									}
								}
							}
						}
					}

					// 해당 마을 주민들에게 버프를 준다
					AddBufToAllTownPerson( siMyHomeTownUnique, &addBuf, BUF_SPECIALAMPUL_EXPUP300, 0 );
				}

				//////////////////////////////////////////////////////////////////////////
				// 사용한 유저의 아이템만 1개 감소 (실제로 아이템이 감소되면서 발생하는 효과는 없는것과 마찬가지)
				sDBRequest_UseItem clMsg( GetID(), pclCI->GetPersonID(), itempos, &clitem, USEITEM_REASON_NONE );
				pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

				return false;
			}
			//------------------------------------------------------------------------------------------------
			// 마을 강화제
			//------------------------------------------------------------------------------------------------
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_RESIDENTS_STRONG )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON )	return false;
				if( itemunique != ITEMUNIQUE( 2802 ) ) return false;

				cltServer* pServer = (cltServer*)pclClient;
				SI32 siMyHomeTownUnique = 0;

				//////////////////////////////////////////////////////////////////////////
				SI32 usetime = 0;															// 일단 걸어줄 버프의 정보를 담은 디폴트 버프를 만들고 (유저에게 실제로 줄때 사용하는게 아닌 줄 버프의 정보를 담아두는 용도로 사용)
				usetime = pServer->pclTime->clDate.GetHourVary() + 24;		// 디폴트 값은 현실시간 48분간 (게임시간1일).
				stBuf addBuf( BUF_RESIDENTS_STRONG, 10, usetime );

				//////////////////////////////////////////////////////////////////////////
				if ( RANKTYPE_CITYHALL == pclCI->clCharRank.siType )						// 아이템 사용한 사람이 영주일때
				{
					siMyHomeTownUnique = pclCI->clCharRank.GetVillageUnique();				// 영주 일땐 랭크 정보에서 마을 유니크를 얻는다. (캐릭터 기본 정보에는 들어있지 않기 때문)
				}
				else																		// 아이템 사용한 사람이 영주가 아닐때
				{
					siMyHomeTownUnique = pclCI->clBI.siHomeVillage;
				}

				//////////////////////////////////////////////////////////////////////////
				if ( true == pServer->pclVillageManager->IsValidVillage( siMyHomeTownUnique ) )
				{
					// 영주 캐릭의 버프 처리 (영주는 clBI.siHomeVillage에 값이 세팅이 안되어 있기 때문에 AddBufToAllTownPerson()함수에서 체크되지 않는다)
					if ( RANKTYPE_CITYHALL == pclCI->clCharRank.siType )					// 아이템 사용한 사람이 영주일때
					{
						if ( true == IsValidConnection() )
						{
							AddTownBufThroughDB( this, &addBuf );
						}
					}
					else																	// 아이템 사용한 사람이 영주가 아닐때
					{
						cltRank* pclRank = pServer->pclRankManager->GetRank( RANKTYPE_CITYHALL, siMyHomeTownUnique );
						if ( NULL != pclRank )
						{
							cltRank clRank;
							SI32 siPersonID = 0;
							clRank.Set( pclRank );

							siPersonID = clRank.clPerson.GetPersonID();
							if ( 0 < siPersonID )
							{
								SI32 siCharID = 0;
								siCharID = pServer->pclCM->GetIDFromPersonID( siPersonID );
								if ( 0 < siCharID )
								{
									cltCharServer* pCityJJang = (cltCharServer*)pServer->pclCM->CR[siCharID];
									if ( NULL != pCityJJang )		// 영주 캐릭터를 찾았다면
									{
										if ( true == pCityJJang->IsValidConnection() )
										{
											pCityJJang->AddTownBufThroughDB( pCityJJang, &addBuf );
										}
									}
								}
							}
						}
					}

					// 해당 마을 주민들에게 버프를 준다
					AddBufToAllTownPerson( siMyHomeTownUnique, &addBuf, 0, 0 );
				}

				//////////////////////////////////////////////////////////////////////////
				// 사용한 유저의 아이템만 1개 감소 (실제로 아이템이 감소되면서 발생하는 효과는 없는것과 마찬가지)
				sDBRequest_UseItem clMsg( GetID(), pclCI->GetPersonID(), itempos, &clitem, USEITEM_REASON_NONE );
				pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

				return false;
			}

			// 퀘스트용 안티 포션
			else if ( pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_QUEST_ANTIPOTION )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON )
				{
					return false;
				}

				SI32 siUseItem = 0;
				if ( ITEMUNIQUE(2796) == itemunique )
				{
					siUseItem = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ; // 현실시간 48분간.
				}

				SI16 siEmptySlot = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_QUEST_ANTIPOTION, false );
				if ( -1 < siEmptySlot )
				{
					stBuf addBuf(BUF_QUEST_ANTIPOTION, 0, siUseItem);
					AddBufThroughDB( siEmptySlot, &addBuf , true , itempos , &clitem );
				}

				return false;
			}
			// 해독제를 사용했다면,
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_ANTIDOTES)
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				SI32 siChangeType = 0;
				// 중독 상태 해독.
				if( clInnerStatus.IsStatus( INNERSTATUS_POISON ) )
				{
					clInnerStatus.DelStatus(INNERSTATUS_POISON);
					siChangeType = CHANGETYPE_POISON;
				}
				else if( clInnerStatus.IsStatus( INNERSTATUS_POISON_SETITEM_EFFECT ) )
				{
					clInnerStatus.DelStatus(INNERSTATUS_POISON_SETITEM_EFFECT);
					siChangeType = CHANGETYPE_POISON_SETITEM_EFFECT;
				}
				
				// 이웃에게 통보 
				cltCharChangeStatusInfo	StatusInfo( siChangeType, false );
				cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				SendNeighbourMsg(&clMsg, true);
			}
			else if( HEALEFT_TRANSFORM_SYNCRO_UP == pclItemManager->pclItemInfo[ref]->siHealEft )
			{	
				static const SI32 siAddTime = 28512;

				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON )	return false ;

				if( false == clTransFormInfo.bTransFormMode )	return false;

				// 싱크로 100% 면 아이템을 소모하지 않는다.
				if( (SI32)clTransFormInfo.GetSynchroTransForm( clTransFormInfo.uiNowTransFormKind ) >= 100 )
				{
					pclserver->SendServerResponseMsg( 0, SRVAL_TRANSFORM_SYNCRO_UP, 100, 0,  GetID() );
					return false; 
				}

				// DB에 변신 시간을 추가해준다.
				sDBRequest_Transform_AddTime clMsg1(GetCharID(), pclCI->GetPersonID(), clTransFormInfo.uiNowTransFormKind, siAddTime );
				pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg1);


				UI16 tKind = clTransFormInfo.uiNowTransFormKind;
				SI32 siBeforeSync = (SI32)clTransFormInfo.GetSynchroTransForm( tKind );
				// 해당 변신의 사용시간을 더한다.
				clTransFormInfo.AddTransFormTime(tKind, siAddTime);
				SI32 siCurSync = (SI32)clTransFormInfo.GetSynchroTransForm( tKind );

				if( siCurSync == 100 && siBeforeSync < siCurSync )
				{
					// 싱크율 100를 달성한 경우 명품 알림창을 띄운다.
					cltItem clItem;		clItem.Init();

					SI32 siItemUnique = GetItemUniqueFromCharKind(clTransFormInfo.uiNowTransFormKind);
					if(siItemUnique > 0)
					{
						SI16 brareswitch = 0;
						if( pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clItem, 0, 0, &brareswitch,0,0,0,0,0,0) == true )
						{
							clItem.SetItemNum(1);
							cltGameMsgResponse_MaxSyncro_Attainment clMaxSyncro( &clItem, clTransFormInfo.uiNowTransFormKind, pclCI->clBI.GetName() );
							cltMsg clMsg( GAMEMSG_RESPONSE_MAXSYNCRO_ATTAINMENT, sizeof(clMaxSyncro), (BYTE*)&clMaxSyncro );
							pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
						}
					}
				}

				// 싱크율이 변경되었으므로, 클라이언트에 한번 전송한다.
				if( siCurSync != siBeforeSync )
				{
					// 클라이언트에 정보를 전송한다.
					cltGameMsgResponse_SetTransForm	clTrasFormInfo( &clTransFormInfo );
					cltMsg clMsg( GAMEMSG_RESPONSE_TRANSFORM, sizeof(cltTransFormInfo), (BYTE*)&clTrasFormInfo );
					SendNetMsg( (sPacketHeader*)&clMsg );			

					pclserver->SendServerResponseMsg( 0, SRVAL_TRANSFORM_SYNCRO_UP, 200, 0,  GetID() );
				}

				// 변신 100% 이펙트를 그려준다.
				if( siCurSync >= 100 )
				{
					cltCharChangeStatusInfo clchangeinfo(CHANGETYPE_TRANSFORM100EFFECT, true);
					cltGameMsgResponse_StatusInfo clinfo(GetID(), &clchangeinfo);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clinfo), (BYTE*)&clinfo);
					SendNeighbourMsg(&clMsg, true);	
				}


			}
			// 선공 방지 효과가 있다면, 
			if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_NOFIRSTATTACK)
			{
				bNoFirstAttackSwitch = true;
				dwNoFirstAttackClock = pclClient->CurrentClock;

				// 이웃에게 통보 
				cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACK, bNoFirstAttackSwitch);
				cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				SendNeighbourMsg(&clMsg, true);
			}
			// [기형] 소환수 선공방지 스프
			if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SUMMONNOFIRSTATTACK)
			{
				SI32 siSummonID = GetSummonID();

				// 소환수
				cltCharServer *pclSummon = pclCM->GetCharServer(siSummonID);
				if(NULL != pclSummon)
				{
					// 이미 선공방지 상태라면
					if( true == pclSummon->bNoFirstAttackSwitch)
					{
						return false;
					}

					// 프리미엄 선공방지 스프를 먹은 상태라면
					if( true == pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum)
					{
						return false;
					}

					pclSummon->bNoFirstAttackSwitch = true;
					pclSummon->dwNoFirstAttackClock = pclClient->CurrentClock;

					// 이웃에게 통보 
					cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_SUMMONNOFIRSTATTACK, pclSummon->bNoFirstAttackSwitch);
					cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
					SendNeighbourMsg(&clMsg, true);
				}
				else
				{
					return false;
				}
			}
			// 말 능력 향상 효과가 있다면, 
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HORSEWINGS)
			{
				// 말이 있어야 아이템을 사용한다. - by LEEKH 2007.10.18
				cltHorse *pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if(pclHorse == NULL || pclHorse->siHorseUnique <= 0)
				{
					return false;
				}

				// 오토바이는 탈것 강화제 영향을 받지 않는다.->2009-03-04 기획팀의 요청으로 오토바이도 탈것강화제를 사용할수 있도록 수정 
				//if(pclHorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
				//	return false;

				if ( pclHorse->siStatusTicketNum == 0 )
				{
					pclHorse->clAddStatusDate.Set(&((cltServer*)pclClient)->pclTime->clDate);
				}
				pclHorse->siStatusTicketNum += 1 ;

				// DB에 저장한다. 
				SetUpdateSwitch(UPDATE_HORSE, true, 0);
				UpdateHorseToDB((cltServer*)pclClient,
					pclCI->clHorseInfo.siCurrentShowHorse,
					pclHorse, SETHORSE_REASON_TICKET);
			}
			// 해동 효과가가 있다면, 
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_UNFREEZE)
			{
				// 해동 상태 해소. 
				clInnerStatus.DelStatus(INNERSTATUS_ICE);

				// 이웃에게 통보 (ICE 방시 설정)
				dwNoIceAttackClock = pclClient->CurrentClock;
				bNoIceAttackSwitch = true;
				cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOICEATTACK, bNoIceAttackSwitch);

				cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				SendNeighbourMsg(&clMsg, true);
			}
			// 프리미엄 효과가 있다면
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_PREMIUM )
			{
				if ( bIsSetPremium ) return false ;
				if ( pclCI->clBI.bPremiumService ) return false ;
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) ) return false;	// 사랑버프 효과가 더 강하므로 못먹는다.
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP) ) return false;	// 사랑버프 효과가 더 강하므로 못먹는다.
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI) ) return false;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMEXPUP) ) return false;

				SI32 usedate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() + 2 ;
				sDBRequest_SetPremium clPremium(GetCharUnique(),pclCI->GetPersonID(),true,usedate,0,0,0,0,&clitem,itempos,SETPREMIUM_EXP);
				((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clPremium);


				bIsSetPremium = true ;

				return false ;
			}

			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_COMBAT )
			{
				if ( bIsSetCombat ) return false ;
				if ( pclCI->clBI.bCombatService ) return false ;
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				SI32 usedate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() + 30 ;
				sDBRequest_SetPremium clCombat(GetCharUnique(),pclCI->GetPersonID(),true,0,usedate,0,0,0,&clitem,itempos,SETPREMIUM_COMBAT);
				((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clCombat);

				bIsSetCombat = true ;

				return false ;
			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_BULLET )
			{
				if ( bIsBulletService ) return false ;
				if ( pclCI->clBI.bBulletService ) return false ;
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				SI32 usedate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() + 361;
				sDBRequest_SetPremium clCombat(GetCharUnique(),pclCI->GetPersonID(),true,0,0,0, usedate,0,&clitem,itempos,SETPREMIUM_BULLET);
				((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clCombat);

				bIsBulletService = true ;

				return false ;
			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SUMMONSTAMINA )
			{
				if( pclCI->clSummonInfo.IsSummonTranform( siSummonIndex ) )	
				{
					pclserver->SendServerResponseMsg( 0, SRVAL_RETURN_SUMMON_TRANSFORM_USE_STAMINA_ITEM, 0, 0, GetID() );
					return false;
				}

				if ( bIsSummonStaminaService )							return false ;
				if ( pclCI->clBI.bSummonStaminaService )				return false ;
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON )	return false ;
				if ( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) )	return false;	// 사랑버프 효과가 더 강하므로 못먹는다.

				SI32 usedate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() + 2;
				//					sDBRequest_SetPremium clCombat(GetCharUnique(),pclCI->GetPersonID(),true,0,0,0,usedate,0,&clitem,itempos,SETPREMIUM_SUMMONSTIMINA);
				//KHY - 0910 - 소환스태미너제 사용 버그수정.
				sDBRequest_SetPremium clCombat(GetCharUnique(),pclCI->GetPersonID(),true,0,0,0,0,usedate,&clitem,itempos,SETPREMIUM_SUMMONSTIMINA);
				((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clCombat);

				bIsSummonStaminaService = true ;

				return false ;
			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_HEROSTAMINA )
			{
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				if(pclCI->clHealthInfo.uiHealthPoint < 110)	// 최대 건강 수치를 넘길 수는 없다. 
				{
					pclCI->clHealthInfo.uiHealthPoint++;
				}
				else
				{
					return false;
				}

				SetUpdateSwitch(UPDATE_HEALTH, true, 0);

			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_DIGESTIVE )
			{					
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;


				SI32 hungryrecover = -20;
				// 포만감을 회복시켜준다.
				clPB.clHealth.IncreaseHungry(hungryrecover);

				SetUpdateSwitch(UPDATE_HEALTH, true, 0);

			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_ALLDESEASE )
			{	
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;


				cltCurDisease clCurDisease;
				if(clPB.clHealth.GetCurDisease(&clCurDisease) == true)
				{
					// 유니크가 일치하는지 확인한다. 


					// 질병을 치료한다. 
					if( clCurDisease.siOrg1Unique > 0 )					
					{
						clPB.clHealth.CureDisease(clCurDisease.siOrg1Unique);
					}
					if( clCurDisease.siOrg2Unique > 0 )					
					{
						clPB.clHealth.CureDisease(clCurDisease.siOrg2Unique);
					}
					if( clCurDisease.siOrg3Unique > 0 )					
					{
						clPB.clHealth.CureDisease(clCurDisease.siOrg3Unique);
					}
					if( clCurDisease.siOrg4Unique > 0 )					
					{
						clPB.clHealth.CureDisease(clCurDisease.siOrg4Unique);
					}


					// 클라이언트로 통보한다. 
					SetUpdateSwitch(UPDATE_HEALTH, true, 0);

					// DB에 저장한다. 
					sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


				}	
			}
			else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_FAME )
			{	
				if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) return false ;

				IncreaseFameExp(10);
			}
			else if (pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_ADDEXP)
			{
				if (itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON){ return false;}
				else if (pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_ADDEXP){
					if (itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON){
						return false;
					}
					//백년삼 확률별로 경험치 적용 [2007.07.25 손성웅]
					SI64 AddExp= 0		;
					SI32 PercentNum = 0	;
					PercentNum = rand() % 1000;
					if ( PercentNum > 0  && PercentNum <=700 )
					{
						AddExp = rand() % 10000 + 10000;
					}
					else if ( PercentNum >700 && PercentNum <=820 )
					{
						AddExp = rand() % 10000 + 20000;
					}
					else if ( PercentNum >820  && PercentNum <=890  )
					{
						AddExp = rand() % 10000 + 40000;
					}
					else if ( PercentNum >890  && PercentNum <= 940  )
					{
						AddExp = rand() % 10000 + 60000;
					}
					else if ( PercentNum >940 && PercentNum <=970 )
					{
						AddExp = rand() % 10000 + 70000;
					}
					else if ( PercentNum >970 && PercentNum <=990 )
					{
						AddExp = rand() % 10000 + 80000;
					}
					else if ( PercentNum >990 && PercentNum <=100 )
					{
						AddExp = rand() % 10000 + 90000;
					}
					IncreaseExp(AddExp);
				}
			}


			cltCharServer* pcltargetchar = NULL; 
			if(pclItemManager->pclItemInfo[ref]->siAdapt == ADAPT_SUMMON)
			{
				SI32 id = GetSummonID();

				if(pclCM->IsAlive(id))
				{
					pcltargetchar = (cltCharServer*)pclCM->CR[id];
				}

				if(pcltargetchar == NULL)	break;
			}
			else
			{
				pcltargetchar = this;
			}

			SI32 orglife	= pcltargetchar->pclCI->clIP.GetLife();
			SI32 orgmana	= pcltargetchar->pclCI->clIP.GetMana();

			SI32 life		= clitem.GetLife(pclClient->pclItemManager);
			SI32 mana		= clitem.GetMana(pclClient->pclItemManager);
			SI32 disease	= clitem.GetDisease(pclClient->pclItemManager); 

			SI32 lifediff	= max(0, pcltargetchar->clPB.GetMaxLife() - orglife - pcltargetchar->clPB.siReservedLife);
			SI32 inclife	= min(lifediff, life);
			pcltargetchar->clPB.siReservedLife += inclife;


			SI32 max = MAXORG(pcltargetchar->pclCI->clIP.GetLevel());

			if(pcltargetchar->clPB.GetMaxMana() > orgmana + (SI32)mana)
			{
				pcltargetchar->pclCI->clIP.IncreaseMana(mana);
			}
			else
			{
				pcltargetchar->pclCI->clIP.SetMana(pcltargetchar->clPB.GetMaxMana());
			}

			SI32 diffmana = pcltargetchar->pclCI->clIP.GetMana() - orgmana;


			// 소환수인 경우 주인의 데이터를 업데이트하고 클라이언트에 통보한다. 
			if(pclItemManager->pclItemInfo[ref]->siAdapt == ADAPT_SUMMON)
			{
			}
			// 사람인 경우 건강에도 관여한다. 
			else if(pclItemManager->pclItemInfo[ref]->siAdapt == ADAPT_MAN)
			{
				// 소화기관을 손상시킨다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty)
				{
					SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg1Penalty / 10;
					SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg1Penalty) % 10;

					pclCI->clHealthInfo.IncreaseInnerOrg1(penalty);

					if((rand() % 10) < modul)
					{
						if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty > 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg1(1);
						}
						if(pclItemManager->pclItemInfo[ref]->siOrg1Penalty < 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg1(-1);
						}

					}
					// 최대값 범위를 벗어나지 않는다. 
					if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG1) > max)
					{
						pclCI->clHealthInfo.siInnerOrg1 = max;
					}
					else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG1) < -max)
					{
						pclCI->clHealthInfo.siInnerOrg1 = -max;
					}


					SetUpdateSwitch(UPDATE_HEALTH, true, 0);

					// DB에 저장한다. 
					sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}

				// 순환기관을 손상시킨다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty)
				{
					SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg2Penalty / 10;
					SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg2Penalty) % 10;

					pclCI->clHealthInfo.IncreaseInnerOrg2(penalty);

					if((rand() % 10) < modul)
					{
						if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty > 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg2(1);
						}
						if(pclItemManager->pclItemInfo[ref]->siOrg2Penalty < 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg2(-1);
						}

					}

					// 최대값 범위를 벗어나지 않는다. 
					if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG2) > max)
					{
						pclCI->clHealthInfo.siInnerOrg2 = max;
					}
					else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG2) < -max)
					{
						pclCI->clHealthInfo.siInnerOrg2 = -max;
					}


					SetUpdateSwitch(UPDATE_HEALTH, true, 0);

					// DB에 저장한다. 
					sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}


				// 호흡기관을 손상시킨다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty)
				{
					SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg3Penalty / 10;
					SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg3Penalty) % 10;

					pclCI->clHealthInfo.IncreaseInnerOrg3(penalty);

					if((rand() % 10) < modul)
					{
						if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty > 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg3(1);
						}
						if(pclItemManager->pclItemInfo[ref]->siOrg3Penalty < 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg3(-1);
						}

					}

					// 최대값 범위를 벗어나지 않는다. 
					if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG3) > max)
					{
						pclCI->clHealthInfo.siInnerOrg3 = max;
					}
					else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG3) < -max)
					{
						pclCI->clHealthInfo.siInnerOrg3 = -max;
					}


					SetUpdateSwitch(UPDATE_HEALTH, true, 0);

					// DB에 저장한다. 
					sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}

				// 신경기관을 손상시킨다. 
				if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty)
				{
					SI32 penalty	= pclItemManager->pclItemInfo[ref]->siOrg4Penalty / 10;
					SI32 modul		= TABS(pclItemManager->pclItemInfo[ref]->siOrg4Penalty) % 10;

					pclCI->clHealthInfo.IncreaseInnerOrg4(penalty);

					if((rand() % 10) < modul)
					{
						if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty > 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg4(1);
						}
						if(pclItemManager->pclItemInfo[ref]->siOrg4Penalty < 0)
						{
							pclCI->clHealthInfo.IncreaseInnerOrg4(-1);
						}

					}

					// 최대값 범위를 벗어나지 않는다. 
					if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG4) > max)
					{
						pclCI->clHealthInfo.siInnerOrg4 = max;
					}
					else if(pclCI->clHealthInfo.GetOrg(HEALTH_ORGAN_TYPE_ORG4) < -max)
					{
						pclCI->clHealthInfo.siInnerOrg4 = -max;
					}

					SetUpdateSwitch(UPDATE_HEALTH, true, 0);

					// DB에 저장한다. 
					sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}

				// 치료할 수 있는 질병이 있다면, 
				if(disease)
				{
					// 현재의 질병 유니크를 얻어온다. 
					cltCurDisease clCurDisease;
					if(pcltargetchar->clPB.clHealth.GetCurDisease(&clCurDisease) == true)
					{
						// 유니크가 일치하는지 확인한다. 
						if(clCurDisease.IsIn(disease) == true)
						{
							// 질병을 치료한다. 
							pcltargetchar->clPB.clHealth.CureDisease(disease);

							// 클라이언트로 통보한다. 
							pcltargetchar->SetUpdateSwitch(UPDATE_HEALTH, true, 0);

							// DB에 저장한다. 
							sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

						}
					}
				}
			}


			if(inclife)
			{
				cltGameMsgResponse_IncLife clInfo(pcltargetchar->GetCharUnique(), pcltargetchar->pclCI->clIP.GetLife(), 0, pcltargetchar->clPB.siReservedLife, inclife, INCLIFE_REASON_HEAL);
				cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
				SendNetMsg((sPacketHeader*)&clMsg);

			}
			if(diffmana)
			{
				cltGameMsgResponse_IncMana clInfo(pcltargetchar->GetCharUnique(), pcltargetchar->pclCI->clIP.GetMana(), diffmana, INCMANA_REASON_HEAL);
				cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
				SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;

		//==============================================================================================================================
		// ANIMAL
		//==============================================================================================================================
	case ITEMTYPE_ANIMAL:
		{
			cltServer* pclserver = (cltServer*)pclClient;


			bool bnoticeallswitch = false;
			SI32 speed			= 0;
			SI32 sistr = 0, sidex = 0, simag = 0;
			SI16 life = 1;

			SI32 horseunique	= HORSEUNIQUE_BROWN;		
			TCHAR* pText = GetTxtFromMgr(667);
			TCHAR horsename[128];
			StringCchCopy(horsename, 128, pText);

			switch(pclItemManager->pclItemInfo[ref]->clItem.siUnique)
			{
			case ITEMUNIQUE(5605):	// 수명 30일 남은 나이 많은 갈색마만 나오는 메달
				{
					speed = 1500;
					sistr	= 2000;
					sidex	= 2000;
					simag	= 2000;	
					life	= 2700;
				}
				break;

			case ITEMUNIQUE(5600):	// 망아지 - 작은 메달
				{
					// 대만 크리스마스 이벤트 (PCK - 2007.11.16)
					cltUniqueRange TaiwanChristmasEvent_SmallMedal[] = 
					{
						{	HORSEUNIQUE_BLACK,		2	}	// < 2		흑마
						,{	HORSEUNIQUE_WHITE,		2	}	// < 4		백마
						,{  HORSEUNIQUE_ZEBRA,		2	}	// < 6		얼룩마
						,{	HORSEUNIQUE_RUDOLF,		10	}	// < 16		루돌프	
					};

					// 글로벌, USA, 일본 크리스마스 이벤트 (PCK - 2007.11.29)
					cltUniqueRange ChristmasEvent_SmallMedal[]=
					{
						{	HORSEUNIQUE_BLACK,		3	}	// < 3		흑마
						,{	HORSEUNIQUE_WHITE,		3	}	// < 6		백마
						,{	HORSEUNIQUE_ZEBRA,		1	}	// < 7		얼룩마
						,{	HORSEUNIQUE_THOUSAND,	1	}	// < 8		천리마
						,{	HORSEUNIQUE_RED,		1	}	// < 9		적토마
						,{	HORSEUNIQUE_STEEL,		1	}	// < 10		철기마
						,{	HORSEUNIQUE_RUDOLF,		10	}	// < 20		루돌프
					};

					cltUniqueRange* pChristmasEvent = NULL;
					SI32			siCount = 0;

					SYSTEMTIME st_eventstart;
					SYSTEMTIME st_eventend;
					ZeroMemory(&st_eventstart, sizeof(st_eventstart));
					ZeroMemory(&st_eventend,   sizeof(st_eventend));

					if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
					{
						st_eventstart.wYear = 2007;		st_eventstart.wMonth = 11;	st_eventstart.wDay = 27;
						st_eventend.wYear = 2007;		st_eventend.wMonth = 12;	st_eventend.wDay = 27;

						pChristmasEvent = TaiwanChristmasEvent_SmallMedal;
						siCount = sizeof(TaiwanChristmasEvent_SmallMedal) / sizeof(cltUniqueRange);
					}
					else if(pclClient->IsWhereServiceArea(ConstServiceArea_USA))
					{
						st_eventstart.wYear = 2007;		st_eventstart.wMonth = 12;	st_eventstart.wDay = 12;
						st_eventend.wYear = 2007;		st_eventend.wMonth = 12;	st_eventend.wDay = 27;	

						pChristmasEvent = ChristmasEvent_SmallMedal;
						siCount = sizeof(ChristmasEvent_SmallMedal) / sizeof(cltUniqueRange);
					}
					else if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
					{
						st_eventstart.wYear = 2007;		st_eventstart.wMonth = 12;	st_eventstart.wDay = 12;
						st_eventend.wYear = 2007;		st_eventend.wMonth = 12;	st_eventend.wDay = 26;	st_eventend.wHour = 11;	

						pChristmasEvent = ChristmasEvent_SmallMedal;
						siCount = sizeof(ChristmasEvent_SmallMedal) / sizeof(cltUniqueRange);
					}
					else if(pclClient->IsWhereServiceArea(ConstServiceArea_English))
					{
						st_eventstart.wYear = 2007;		st_eventstart.wMonth = 12;	st_eventstart.wDay = 04;
						st_eventend.wYear = 2007;		st_eventend.wMonth = 12;	st_eventend.wDay = 27;

						pChristmasEvent = ChristmasEvent_SmallMedal;
						siCount = sizeof(ChristmasEvent_SmallMedal) / sizeof(cltUniqueRange);
					}
					else if(pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina))//NHN중국도 크리스마스이벤트는 할테니 기간추가
					{
						st_eventstart.wYear = 2007;		st_eventstart.wMonth = 12;	st_eventstart.wDay = 04;
						st_eventend.wYear = 2007;		st_eventend.wMonth = 12;	st_eventend.wDay = 27;

						pChristmasEvent = ChristmasEvent_SmallMedal;
						siCount = sizeof(ChristmasEvent_SmallMedal) / sizeof(cltUniqueRange);
					}
					else if ( pclClient->IsWhereServiceArea(ConstServiceArea_EUROPE) )
					{
						st_eventstart.wYear = 2007;		st_eventstart.wMonth = 12;	st_eventstart.wDay = 04;
						st_eventend.wYear = 2007;		st_eventend.wMonth = 12;	st_eventend.wDay = 27;

						pChristmasEvent = ChristmasEvent_SmallMedal;
						siCount = sizeof(ChristmasEvent_SmallMedal) / sizeof(cltUniqueRange);
					}
					else
					{
						st_eventstart.wYear = 2001;		st_eventstart.wMonth = 1;	st_eventstart.wDay = 1;
						st_eventend.wYear = 2001;		st_eventend.wMonth = 1;	st_eventend.wDay = 2;
					}

					if(TimeInBounds(&pclserver->sTime, &st_eventstart, &st_eventend))
					{
						if(pChristmasEvent != NULL)
							horseunique = GetUniqueWithRange( pChristmasEvent, siCount, 100);
					}
					else
					{
						horseunique = GetUniqueWithRange( ItemUnique_5600_SmallMedal, sizeof(ItemUnique_5600_SmallMedal) / sizeof(cltUniqueRange), 1000);
					}

					// 나머지는 갈색마임.
					if(horseunique == 0)		horseunique	= HORSEUNIQUE_BROWN;

					if ( pclClient->siServiceArea == ConstServiceArea_China )
					{
						horseunique = HORSEUNIQUE_BROWN;
					}

					// 초기 능력치를 대만만 다르게 적용함 (PCK - 2007.09.11)
					//if (pclClient->IsWhereServiceArea(ConstSwitch_HorseAbility))
					if (pclClient->IsCountrySwitch(Switch_HorseAbility))
					{
						// 능력 설정, 모두에게 알림 설정
						switch(horseunique)
						{
						case HORSEUNIQUE_BLACK:	
						case HORSEUNIQUE_WHITE:	
						case HORSEUNIQUE_ZEBRA:	
						case HORSEUNIQUE_THOUSAND:
						case HORSEUNIQUE_RED:	
						case HORSEUNIQUE_STEEL:
						case HORSEUNIQUE_RUDOLF:
							{
								speed = 1400;
								sistr = 500;
								sidex = 500;
								simag = 500;

								bnoticeallswitch = true;
							}
							break;
						case HORSEUNIQUE_BROWN:
							{
								speed = 1400;
								sistr = 500;
								sidex = 500;
								simag = 500;
							}
							break;
						}
					}
					else
					{
						// 능력 설정, 모두에게 알림 설정
						switch(horseunique)
						{
						case HORSEUNIQUE_BLACK:	
						case HORSEUNIQUE_WHITE:	
						case HORSEUNIQUE_ZEBRA:	
						case HORSEUNIQUE_THOUSAND:
						case HORSEUNIQUE_RED:	
						case HORSEUNIQUE_STEEL:	
							{
								speed = 800;
								sistr	= 500;
								sidex	= 500;
								simag	= 500;

								bnoticeallswitch = true;
							}
							break;
						case HORSEUNIQUE_BROWN:
							{
								speed = 500 + (300  - (rand() % 600));
								sistr	= 50 + rand() % 10;
								sidex	= 50 + rand() % 10;
								simag	= 50 + rand() % 10;
							}
							break;
						case HORSEUNIQUE_RUDOLF:
							{
								speed = 1400;
								sistr = 500;
								sidex = 500;
								simag = 500;
							}
							break;
						}
					}

					// 말 이름 설정
					switch(horseunique)
					{
					case HORSEUNIQUE_BLACK:	{	TCHAR* pText1 = GetTxtFromMgr(2806);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_WHITE:	{	TCHAR* pText1 = GetTxtFromMgr(2805);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_ZEBRA:	{	TCHAR* pText1 = GetTxtFromMgr(2769);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_THOUSAND:{	TCHAR* pText1 = GetTxtFromMgr(2791);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_RED:	{	TCHAR* pText1 = GetTxtFromMgr(2753);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_STEEL:	{	TCHAR* pText1 = GetTxtFromMgr(5174);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_RUDOLF:{	TCHAR* pText1 = GetTxtFromMgr(3370);	StringCchCopy(horsename, 128, pText1);	}	break;
					case HORSEUNIQUE_BROWN:	{	}	break;
					}
				}
				break;

			case ITEMUNIQUE(5700): // 구미호
				{
					horseunique = HORSEUNIQUE_BEAST;
					TCHAR* pText1 = GetTxtFromMgr(6418);
					StringCchCopy(horsename, 128, pText1);
					speed = 800;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;

			case ITEMUNIQUE(5695): //  늑대
				{
					horseunique = HORSEUNIQUE_WOLF;
					TCHAR* pText1 = GetTxtFromMgr(7059);
					StringCchCopy(horsename, 128, pText1);
					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_WOLF );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_WOLF );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_WOLF );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_WOLF );
				}
				break;
				//KHY - 0911 - 호랑이 추가.	
			case ITEMUNIQUE(5690): //  호랑이
				{
					horseunique = HORSEUNIQUE_TIGER;
					TCHAR* pText1 = GetTxtFromMgr(5394);
					StringCchCopy(horsename, 128, pText1);
					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_TIGER );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_TIGER );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_TIGER );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_TIGER );
				}
				break;
				//KHY - 0622 - 군주S 초보자용  망아지 추가.
			case ITEMUNIQUE(5603): //  기존 초보자용 마패 사용	
				{
					horseunique = HORSEUNIQUE_FOAL;
					TCHAR* pText1 = GetTxtFromMgr(7505);
					StringCchCopy(horsename, 128, pText1);
					//KHY - 0820 - 군주S 망아지 수정.
					// 대만의 경우는 게임시간 1500일로 정함.
					//if(pclClient->IsWhereServiceArea(ConstSwitch_HorseAbility)){	/*life = 3500;*/	}
					if(pclClient->IsCountrySwitch(Switch_HorseAbility)){	/*life = 3500;*/	}
					else													   {	life = 1050;	}	//게임시간 450일.

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_FOAL );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_FOAL );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_FOAL );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_FOAL );
				}
				break;
				//KHY - 0917 - 군주S 갈색마만 나오는 매달.
			case ITEMUNIQUE(5620): // 갈색마.
				{
					life	= 2000; //게임시간 1000일.

					//speed = 500 + (300  - (rand() % 600));

					speed = 1400;
					sistr	= 500 + rand() % 100;
					sidex	= 500 + rand() % 100;
					simag	= 500 + rand() % 100;
				}
				break;
				// kkm 중국 퀘스트 아이템
			case ITEMUNIQUE(5610): 
				{
					horseunique = HORSEUNIQUE_FOAL;
					TCHAR* pText1 = GetTxtFromMgr(7505);
					StringCchCopy(horsename, 128, pText1);
					life	= 1050; //게임시간 450일.

					speed	= 1500;
					sistr	= 3000;
					sidex	= 3000;
					simag	= 3000;
				}
				break;
				// kkm 중국 고급마패
			case ITEMUNIQUE(5615):	
				{
					SI32 horseUniqueArray[6] = { HORSEUNIQUE_BLACK, HORSEUNIQUE_WHITE, HORSEUNIQUE_RED, 
						HORSEUNIQUE_THOUSAND, HORSEUNIQUE_STEEL, HORSEUNIQUE_ZEBRA };

					SI32 index = rand() % 6;
					horseunique = horseUniqueArray[index];

					// 능력 설정, 모두에게 알림 설정
					speed = 800;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;

					// 말 이름 설정
					StringCchCopy( horsename, 128, pclClient->pclHorseManager->GetHorseName(horseunique) );

					pclClient->pclLog->FileConfigPrint( "horse", "%d %s", horseunique, horsename );
				}
				break;
				// PCK - 루돌프 메달 추가 (07.11.28)
			case ITEMUNIQUE(5705):
				{
					horseunique = HORSEUNIQUE_RUDOLF;
					TCHAR* pText1 = GetTxtFromMgr(3370);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 기승동물 메달등록 (흑마메달 - 07.12.11)
			case ITEMUNIQUE(5710):
				{
					horseunique = HORSEUNIQUE_BLACK;
					TCHAR* pText1 = GetTxtFromMgr(2806);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 기승동물 메달등록 (백마메달 - 07.12.11)
			case ITEMUNIQUE(5715):
				{
					horseunique = HORSEUNIQUE_WHITE;
					TCHAR* pText1 = GetTxtFromMgr(2805);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 기승동물 메달등록 (적토마메달 - 07.12.11)
			case ITEMUNIQUE(5720):
				{
					horseunique = HORSEUNIQUE_RED;
					TCHAR* pText1 = GetTxtFromMgr(2753);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 기승동물 메달등록 (얼룩마메달 - 07.12.11)
			case ITEMUNIQUE(5725):
				{
					horseunique = HORSEUNIQUE_ZEBRA;
					TCHAR* pText1 = GetTxtFromMgr(2769);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 기승동물 메달등록 (천리마메달 - 07.12.11)
			case ITEMUNIQUE(5730):
				{
					horseunique = HORSEUNIQUE_THOUSAND;
					TCHAR* pText1 = GetTxtFromMgr(2791);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 기승동물 메달등록 (철기마메달 - 07.12.11)
			case ITEMUNIQUE(5735):
				{
					horseunique = HORSEUNIQUE_STEEL;
					TCHAR* pText1 = GetTxtFromMgr(5174);
					StringCchCopy(horsename, 128, pText1);

					speed	= 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;
				// PCK - 낙타메달 등록 (08.02.18)
			case ITEMUNIQUE(5685):
				{
					horseunique = HORSEUNIQUE_CAMEL;
					TCHAR* pText1 = GetTxtFromMgr(7936);
					StringCchCopy(horsename, 128, pText1);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_CAMEL );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_CAMEL );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_CAMEL );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_CAMEL );
				}
				break;
				// PCK : 군주S용 이벤트 호랑이 등록 (PCK : 08.04.01)
			case ITEMUNIQUE(5625):
				{
					horseunique = HORSEUNIQUE_TIGER2;
					TCHAR* pText1 = GetTxtFromMgr(5394);
					StringCchCopy(horsename, 128, pText1);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_TIGER2 );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_TIGER2 );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_TIGER2 );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_TIGER2 );
				}
				break;
				// PCK : 코끼리 추가 (08.06.12)
			case ITEMUNIQUE(5740):
				{
					horseunique = HORSEUNIQUE_ELEPHANT;
					TCHAR* pText1 = GetTxtFromMgr(8460);
					StringCchCopy(horsename, 128, pText1);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_ELEPHANT );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_ELEPHANT );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_ELEPHANT );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_ELEPHANT );
				}
				break;
				// PCK :백호 추가 (08.08.14)
			case ITEMUNIQUE(5745):
				{
					horseunique = HORSEUNIQUE_WHITETIGER;
					TCHAR* pText = GetTxtFromMgr(8752);
					StringCchCopy(horsename, 128, pText);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_WHITETIGER );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_WHITETIGER );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_WHITETIGER );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_WHITETIGER );
				}
				break;
				// PCK :오토바이 추가 (08.08.14)
				//KHY - 1111 - 오토바이 추가.	
			case ITEMUNIQUE(5748):
				{
					horseunique = HORSEUNIQUE_MOTORCYCLE;
					TCHAR* pText = GetTxtFromMgr(8795);
					StringCchCopy(horsename, 128, pText);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_MOTORCYCLE );
					sistr	= 4000;
					sidex	= 4000;
					simag	= 4000;

					// 종량제 시간 충전 - 10000L = 20000분 = 120만초. 
					SI32 siAddTime = 10000 * 2 * 60 ;

					// DB에 저장한다. 
					cltItem clitem2; // 의미 없는 아이템.

					sDBRequest_TimeMeterRate_AddTime clMsg(GetCharID(), pclCI->GetPersonID(), TIMEMETERRATE_MOTORCYCLE, siAddTime, &clitem2, -1);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);				
				}
				break;
				// PCK : 불해치 추가 (08.12.10)
			case ITEMUNIQUE(5749):
				{
					horseunique = HORSEUNIQUE_FIREHATCH;
					TCHAR* pText = GetTxtFromMgr(2788);
					StringCchCopy(horsename, 128, pText);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_FIREHATCH );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_FIREHATCH );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_FIREHATCH );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_FIREHATCH );
				}
				break;
				// 손성웅 - 채험용 백마 2009-03-17
			case ITEMUNIQUE(5746):
				{
					horseunique = HORSEUNIQUE_WHITE_TEST;
					TCHAR* pText = GetTxtFromMgr(9897);
					StringCchCopy(horsename, 128, pText);

					speed	= 1500;
					sistr	= 2000;
					sidex	= 2000;
					simag	= 2000;
				}
				break;
				// PCK : 바포메트 추가 (09.07.30)
			case ITEMUNIQUE(5751):
				{
					horseunique = HORSEUNIQUE_BAPHOMET;
					TCHAR* pText = GetTxtFromMgr(30304);
					StringCchCopy(horsename, 128, pText);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_BAPHOMET );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_BAPHOMET );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_BAPHOMET );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_BAPHOMET );
				}
				break;
			
			case ITEMUNIQUE(5750): //  회색 늑대 추가 : 박기형( 2009.8.30)
				{
					horseunique = HORSEUNIQUE_GRAYWOLF;
					TCHAR* pszText = GetTxtFromMgr(40047);
					StringCchCopy(horsename, 128, pszText);
					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_GRAYWOLF );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_GRAYWOLF );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_GRAYWOLF );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_GRAYWOLF );
				}
				break;
				// 기형 : 사수리
			case ITEMUNIQUE(5752):
				{
					horseunique = HORSEUNIQUE_SASURI;
					TCHAR* pText = GetTxtFromMgr(14035);
					StringCchCopy(horsename, 128, pText);

					speed	= pclClient->pclHorseManager->GetFullSpeed( HORSEUNIQUE_SASURI );
					sistr	= pclClient->pclHorseManager->GetFullStr( HORSEUNIQUE_SASURI );
					sidex	= pclClient->pclHorseManager->GetFullDex( HORSEUNIQUE_SASURI );
					simag	= pclClient->pclHorseManager->GetFullMag( HORSEUNIQUE_SASURI );
				}
				break;
			default:
				return false;
			}

			//------------------------------
			// 성별 결정. 
			//------------------------------
			SI32 sisex = ANIMAL_MALE;
			if(rand() % 5 == 0)
			{
				sisex = ANIMAL_FEMALE;
			}

			SI32 curhour = pclserver->pclTime->clDate.GetHourVary();


			SI32 randomvalue = rand() % 1000;
			TCHAR randomstr[128]=TEXT("");
			_itot(randomvalue, randomstr, 10);

			StringCchCat(horsename, 128, randomstr);

			cltDate cldate;
			cldate.MakeFullDateFromDateVary(0);

			cltHorse clHorse(horseunique, horsename, &pclserver->pclTime->clDate, sisex, life, 0,
				sistr, sidex, simag, speed, curhour,0,&cldate,0
				,&cldate,0
				,&cldate,0
				,&cldate,0
				,0);

			if(pclserver->pclTime->clDate.uiMonth == 0)
			{
				TCHAR* pText = GetTxtFromMgr(2807);
				if (pclClient->GameMode == GAMEMODE_SERVER)
					pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), pText, 
					pclserver->pclTime->clDate.uiYear, pclserver->pclTime->clDate.uiMonth, pclserver->pclTime->clDate.uiDay);
			}

			// 내 소유의 말을 추가한다. 
			SI32 index;
			if(pclCI->clHorseInfo.Add(&clHorse, &index) == true)
			{
				//KHY - 1001 - 기승동물 5마리 보유 수정.
				pclCI->clHorseInfo.SetCurrentShowHorseIndex(index);

				// DB에 말을 추가한다.  // index,
				UpdateHorseToDB((cltServer*)pclClient, index, &clHorse, SETHORSE_REASON_MAKE);

				// 주인과 이웃에게 말 정보를 보낸다. 
				SendNeighbourMyHorseStatus(index);

				//// 명마가 나왔으면 전체 공지한다. 
				if(bnoticeallswitch && GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)
				{
					cltGameMsgResponse_GoodHorse clinfo((TCHAR*)GetName(), pclCI->GetPersonID(), horseunique, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_GOODHORSE, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
				}

				//cyj 말 생산하면 말인벤토리에 있는 아이템 정보 가져오도록 추가
				sDBRequest_PersonItemInfoGet clMsg(siID, pclCI->GetPersonID());
				pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);

			}
			else
			{
				cltGameMsgResponse_NoAddHorse clinfo( 0 ); // 0 = 말 추가 실패. - 
				cltMsg clMsg(GAMEMSG_RESPONSE_NOADDHORSE, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				return false;
			}

		}
		break;
		//==============================================================================================================================
		// ETC
		//==============================================================================================================================
	case ITEMTYPE_ETC:
		{
			if(clitem.siUnique == ITEMUNIQUE(7860) )	// 소환알
			{
				cltServer *pclserver = (cltServer*)pclClient;
				if (pclserver->pclCM->pclIDManager->GetNPCCharNumber() > MAX_NPC_NUMBER - 1000)
				{
					return false;
				}

				if (GetMapIndex() == MAPINDEX_BEGINNER1 || GetMapIndex() == MAPINDEX_PALACE || GetMapIndex() == MAPINDEX_MOFUMOFULAND)
				{
					return false;
				}

				SI32 pos = 0 ;
				SI32 x, y;
				SI32 tempx, tempy, level;
				SI32 mapindex = 0;

				bool bSelectKind = false;

				SI32 randvalue = rand() % 100;
				SI32 cnt = 0;

				SI32 minSort = 0, maxSort = 0; 

				//cltValuesRange ItemUnique_7860_Values_Christmas[] = 
				//{
				//	{	10,		0,		10,		0	}	// < 10
				//	,{	39,		10,		40,		0	}	// < 49
				//	,{	29,		40,		100,	0	}	// < 78
				//	,{	5,		100,	150,	0	}	// < 83
				//	,{	5,		150,	160,	0	}	// < 88
				//	,{	1,		160,	170,	0	}	// < 89
				//	,{	10,		170,	171,	0	}	// < 99
				//	,{	1,		180,	190,	0	}	// < 100
				//};

				SI32 siCount = sizeof(ItemUnique_7860_Values)/ sizeof(cltValuesRange);
				cltValuesRange* pValuesRange = (cltValuesRange*)ItemUnique_7860_Values;

				bool bSantaRacoon = false;
				bool bKoinobory	= false;

					// 크리스마스 이벤트때는 소환알 확률이 변경된다
					if ( true == pclserver->pclEventTimeManager->InEventTime(TEXT("ChristmasEvent"), &pclserver->sTime) )
					{
						SI16 siSantaRacoonPercent = 0;
						siSantaRacoonPercent = rand() % 10;
						// 1/10의 확률이니까 0이 나오면 이벤트 라쿤 등장
						if ( 0 == siSantaRacoonPercent )
						{
							bSantaRacoon = true;
						}

						cltValuesRange* pValuesRange = (cltValuesRange*)ItemUnique_7860_Values;
						siCount = sizeof(ItemUnique_7860_Values) / sizeof(cltUniqueRange);

						//pValuesRange = ItemUnique_7860_Values_Christmas;
						//siCount = sizeof(ItemUnique_7860_Values_Christmas) / sizeof(cltUniqueRange);
					}
					// [영훈] 골든 위크때는 소환알 확률이 변경된다
					else if ( true == pclserver->pclEventTimeManager->InEventTime(TEXT("GoldenWeekEvent"), &pclserver->sTime) )
					{
						SI16 siKoinoboryPercent = 0;
						siKoinoboryPercent = rand() % 10;
						// 1/10의 확률이니까 0이 나오면 이벤트 라쿤 등장
						if ( 0 == siKoinoboryPercent )
						{
							bKoinobory = true;
						}

						cltValuesRange* pValuesRange = (cltValuesRange*)ItemUnique_7860_Values;
						siCount = sizeof(ItemUnique_7860_Values) / sizeof(cltUniqueRange);

						//pValuesRange = (cltValuesRange*)ItemUnique_7860_Values_GoldenWeek;
						//siCount = sizeof(ItemUnique_7860_Values_GoldenWeek) / sizeof(cltUniqueRange);
					}

					// 산타라쿤, 코이노보리가 아닌 일반적인 몬스터 소트레벨 구간이 선택됐을때
					cltValuesRange* pValues = GetValuesWithRange(pValuesRange, siCount, 100);
					if(pValues)
					{
						minSort = pValues->siPara1;
						maxSort = pValues->siPara2;
					}

					// 라쿤이 선택됐을때
					if ( true == bSantaRacoon )
					{
						minSort = 1001;   // 이벤트 라쿤 소트 레벨 (조건식에 걸려 하나 낮은 레벨로 설정)
						maxSort = 1002;   // 이벤트 라쿤 소트 레벨
					}

					// 코이노보리가 선택 됬을때
					if( true == bKoinobory)
					{
						minSort = 1000;   // 코이노보리 소트 레벨
						maxSort = 1001;   // 코이노보리 소트 레벨
					}

					SI32 ValidKindCnt = 0;
					SI32 ValidKindArray[MAX_KINDINFO_NUMBER]= {0, };
					SI32 i = 0;
					for(  i = 0 ; i < MAX_KINDINFO_NUMBER ; i ++ )
					{
						if (pclClient->pclKindInfoSet->IsValidKind(i))
						{
							cltKindInfo *pclki = pclClient->pclKindInfoSet->pclKI[i];

							switch (pclClient->siServiceArea)
							{
							case ConstServiceArea_Korea :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
									continue;
								break;
							case ConstServiceArea_China :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
									continue;
								break;
							case ConstServiceArea_Japan :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
									continue;
								break;
							case ConstServiceArea_English :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
									continue;
								break;
							case ConstServiceArea_Taiwan :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
									continue;
								break;
							case ConstServiceArea_USA :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
									continue;
								break;
							case ConstServiceArea_NHNChina :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
									continue;
								break;
							case ConstServiceArea_EUROPE :
								if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
									continue;
								break;
							}

							if (pclClient->pclKindInfoSet->pclKI[i]->IsAtb(ATB_MONSTER) == false 
								||	pclClient->pclKindInfoSet->GetSortLevel( i ) <= 0 )
							{
								continue;
							}

							if( pclClient->pclKindInfoSet->GetSortLevel( i ) > minSort
								&& pclClient->pclKindInfoSet->GetSortLevel( i ) <=maxSort )
							{
								ValidKindArray[ValidKindCnt] = i;
								ValidKindCnt++;
							}
						}					
					}

					if(ValidKindCnt > 0 &&  ValidKindCnt <= MAX_KINDINFO_NUMBER  )
					{
						SI32 rndIndex = rand() % ValidKindCnt;					

						if( ValidKindArray[rndIndex] > 0 )
						{
							mapindex = GetMapIndex();

							tempx = GetX();
							tempy = GetY();

							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(	ValidKindArray[rndIndex] , tempx, tempy, 10, &x, &y, true) == false)
							{
								break;
							}

							level = rand() % 50 + 1;

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

							SI32 id = pclserver->SetNPCChar(	ValidKindArray[rndIndex] , VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

							if(pclCM->IsValidID(id))
							{
								pclserver->SendServerResponseMsg(0, SRVAL_SUMMONEGG_USED, ValidKindArray[rndIndex], 0, GetID());
							}
						}

					}
					else
					{
						return false;
					}
			}
			// 2007.4.16 수수께끼 소환알 2 - By. 박춘기
			else if(clitem.siUnique == ITEMUNIQUE(7865))	// 수수께끼소환알2
			{
				cltServer *pclserver = (cltServer*)pclClient;
				if (pclserver->pclCM->pclIDManager->GetNPCCharNumber() > MAX_NPC_NUMBER - 1000)
				{
					return false;
				}

				if (GetMapIndex() == MAPINDEX_BEGINNER1 || GetMapIndex() == MAPINDEX_PALACE || GetMapIndex() == MAPINDEX_MOFUMOFULAND)
				{
					return false;
				}

				SI32 pos = 0 ;
				SI32 x, y;
				SI32 tempx, tempy, level;
				SI32 mapindex = 0;

				bool bSelectKind = false;

				SI32 randvalue = rand() % 100;
				SI32 cnt = 0;

				SI32 SortLevel = 0;

				// 확률계산
				if( randvalue < 3 )			{		SortLevel = 1;		}
				else if( randvalue < 6)		{		SortLevel = 2;		}
				else if( randvalue < 9)		{		SortLevel = 3;		}
				else if( randvalue < 12)	{		SortLevel = 4;		}
				else if( randvalue < 15)	{		SortLevel = 5;		}
				else if( randvalue < 18)	{		SortLevel = 6;		}
				else if( randvalue < 11)	{		SortLevel = 7;		}
				else if( randvalue < 24)	{		SortLevel = 8;		}
				else if( randvalue < 27)	{		SortLevel = 9;		}
				else if( randvalue < 30)	{		SortLevel = 10;		}
				else if( randvalue < 33)	{		SortLevel = 11;		}
				else if( randvalue < 36)	{		SortLevel = 12;		}
				else if( randvalue < 39)	{		SortLevel = 13;		}
				else if( randvalue < 42)	{		SortLevel = 14;		}
				else if( randvalue < 45)	{		SortLevel = 15;		}
				else if( randvalue < 48)	{		SortLevel = 16;		}
				else if( randvalue < 51)	{		SortLevel = 17;		}
				else if( randvalue < 54)	{		SortLevel = 18;		}
				else if( randvalue < 57)	{		SortLevel = 19;		}
				else if( randvalue < 60)	{		SortLevel = 20;		}
				else if( randvalue < 63)	{		SortLevel = 21;		}
				else if( randvalue < 66)	{		SortLevel = 22;		}
				else if( randvalue < 69)	{		SortLevel = 23;		}
				else if( randvalue < 74)	{		SortLevel = 501;		} // (91~97) => (501~507) 소트레벨 수정으로 변경.
				else if( randvalue < 79)	{		SortLevel = 502;		}
				else if( randvalue < 84)	{		SortLevel = 503;		}
				else if( randvalue < 88)	{		SortLevel = 504;		}
				else if( randvalue < 82)	{		SortLevel = 505;		}
				else if( randvalue < 96)	{		SortLevel = 506;		}
				else if( randvalue < 100)	{		SortLevel = 507;		}

				SI32 ValidKindCnt = 0;
				SI32 ValidKindArray[MAX_KINDINFO_NUMBER]= {0, };
				SI32 i = 0;
				for(  i = 0 ; i < MAX_KINDINFO_NUMBER ; i ++ )
				{
					if (pclClient->pclKindInfoSet->IsValidKind(i))
					{
						cltKindInfo *pclki = pclClient->pclKindInfoSet->pclKI[i];

						switch (pclClient->siServiceArea)
						{
						case ConstServiceArea_Korea :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
								continue;
							break;
						case ConstServiceArea_China :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
								continue;
							break;
						case ConstServiceArea_Japan :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
								continue;
							break;
						case ConstServiceArea_English :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
								continue;
							break;
						case ConstServiceArea_Taiwan :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
								continue;
							break;
						case ConstServiceArea_USA :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
								continue;
							break;
						case ConstServiceArea_NHNChina :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
								continue;
							break;
						case ConstServiceArea_EUROPE :
							if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
								continue;
							break;

						}

						if (pclClient->pclKindInfoSet->pclKI[i]->IsAtb(ATB_MONSTER) == false 
							||	pclClient->pclKindInfoSet->GetSortLevel( i ) <= 0 )
						{
							continue;
						}

						if( pclClient->pclKindInfoSet->GetSortLevel( i ) == SortLevel)
						{
							ValidKindArray[ValidKindCnt] = i;
							ValidKindCnt++;
						}
					}					
				}					

				if(ValidKindCnt > 0 &&  ValidKindCnt <= MAX_KINDINFO_NUMBER  )
				{
					SI32 rndIndex = rand() % ValidKindCnt;					

					if( ValidKindArray[rndIndex] > 0 )
					{
						// 수수게끼 소환알에서 랜덤으로 여러마리를 뽑아낸다.
						int num = (rand() % 5) + 1;
						int count = 0, created = 0;
						while( count < num)
						{
							count++;

							mapindex = GetMapIndex();

							tempx = GetX();
							tempy = GetY();

							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(	ValidKindArray[rndIndex] , tempx, tempy, 10, &x, &y, true) == false)
							{
								continue;
							}

							level = rand() % 50 + 1;

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

							SI32 id = pclserver->SetNPCChar(	ValidKindArray[rndIndex] , VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
							if(pclCM->IsValidID(id))
							{
								// 수수깨끼 소환알2에 의해 소환된 몬스터이다.
								cltCharServer* pclchar = (cltCharServer*)(pclClient->pclCM->CR[id]);
								pclchar->siCharMode = CHARMODE_SUMMON_EGG2;

								created++;
							}							
						}
						if(created > 0)		pclserver->SendServerResponseMsg(0, SRVAL_SUMMONEGG_USED, ValidKindArray[rndIndex], 0, GetID());
						else
							return false;
					}
				}
				else
				{
					return false;
				}
			}		
			//KHY - 모후 상자  - 
			else if ( clitem.siUnique == ITEMUNIQUE(7975)) // 책 상자
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				cltServer* pclserver = (cltServer*)pclClient;

				SI32 siItemUnique = pclserver->pclBookBox->GetRandBookUnique();
				if ( siItemUnique )
				{
					cltItem clDrawItem;	// 추첨해서 나온 결과아이템
					clDrawItem.Init();

					SI16 brareswitch = 0;

					if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clDrawItem, 0, 0, &brareswitch ) == false)
						return false;

					clDrawItem.SetItemNum(1);

					SI16 invpos;
					cltItem cltoitem;
					SI32 rtnvalue;
					if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
					{
						((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
						return false;
					}

					pclItemManager->AddBookBoxItemNum(clDrawItem.siUnique, clDrawItem.siItemNum);

					// 새로운 통계 - 책상자에 의해 나온 물품의 통계를 낸다.
					//NStatistics_Add("Item", clDrawItem.siUnique, "BookBox", clDrawItem.siItemNum);


					if ( clitem.siUnique != 0 )
					{
						sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
				}
				return false;

			}
			// [영훈] 보이노리코
			else if ( clitem.siUnique == ITEMUNIQUE(8114) )
			{
				cltServer *pclserver = (cltServer*)pclClient;
				if (pclserver->pclCM->pclIDManager->GetNPCCharNumber() > MAX_NPC_NUMBER - 1000)
				{
					return false;
				}

				SI32 siKind		= pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYGREEN") );

				SI32 siMapIndex = GetMapIndex();
				SI32 siNowX		= GetX();
				SI32 siNowY		= GetY();

				SI32 siGetX		= 0;
				SI32 siGetY		= 0;

				SI32 siLevel	= 1;
				SI32 siEnergy	= 100;

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				cltMapCommon* pclMapCom = pclClient->pclMapManager->pclMapArray[siMapIndex];
				if ( NULL == pclMapCom )
				{
					return false;
				}

				// 혹시 생성되지 못할것에 대비해서 몇번 돌아본다
				for ( SI32 siCount=0; siCount<10; siCount++ )
				{
					if ( false == pclMapCom->FindEmptyAreaForCharLocate(siKind, siNowX, siNowY, 20, &siGetX, &siGetY, true) )
					{
						continue;
					}

					SI32 siCharID = pclserver->SetNPCChar( siKind, VILLAGE_NONE, NULL, siLevel, siEnergy, siGetX, siGetY, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if ( pclCM->IsValidID(siCharID) )
					{
						// 퀘스트 전용 몬스터로 신분 변경
						cltCharServer* pclMonster = (cltCharServer*)pclClient->pclCM->GetCharServer(siCharID);
						if ( NULL == pclMonster )
						{
							continue;
						}

						pclMonster->clIdentity.MakeSoulBound( IDENTITY_SOULBOUND, pclCI->GetPersonID() );
						break;
					}							
				}

			}
			// PCK : 잃어버린 세계에서 제조한 기공화석, 금속사용제는 우클릭으로 사용 못하게 한다. (09.05.13)
			else if(clitem.siUnique == ITEMUNIQUE(8115) || clitem.siUnique == ITEMUNIQUE(8116))
			{
				return false;
			}
			// PCK : 잃어버린 세계에서 제조한 정찰 변신약을 사용하면 트리케라놉스로 변신한다. (09.05.13)
			else if(clitem.siUnique == ITEMUNIQUE(8117))
			{
				SI32 id = GetID();

				if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS))
				{
					SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;

					SI16 siKind = pclClient->GetUniqueFromHash("KIND_TRICERAMONS");

					SI16 siBufIndex = -1;

					if ( true == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM ) )
					{
						siBufIndex = pclCI->clBI.clBufInfo.FindBufSlot( BUF_TRANSFORM );
					}
					else
					{
						siBufIndex = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_TRANSFORM );
					}

					if ( -1 < siBufIndex )
					{
						stBuf addBuf(BUF_TRANSFORM, siKind, usetime);
						((cltCharServer*)pclCM->CR[id])->AddBufThroughDB( siBufIndex, &addBuf ,false, 0, NULL, 0);
					}
				}

				return false;
			}
			// PCK : 잃어버린 세계에서 제조한 공격방어제를 사용하면 공격력 30%증가 버프를 얻는다.
			else if ( clitem.siUnique == ITEMUNIQUE(8118) )
			{
				SI32 id = GetID();

				SI32 siUseItem = 0;

				siUseItem = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ; // 현실시간 48분간.

				SI16 siEmptySlot = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_QUEST_ATTACK, false );

				if ( -1 < siEmptySlot )
				{
					stBuf addBuf(BUF_QUEST_ATTACK, 0, siUseItem);
					AddBufThroughDB( siEmptySlot, &addBuf , false , 0 , &clitem );
				}

				return false;
			}
			else if ( (ITEMUNIQUE(8119) == clitem.siUnique) || (ITEMUNIQUE(8120) == clitem.siUnique) )
			{
				if ( FALSE == IsMarried() )
				{
					return false;
				}

				SI32 siNowHourVary		= pclserver->pclTime->clDate.GetHourVary();
				SI32 siSetLimitHourVary	= siNowHourVary + 24; // 게임시간 24시간(현실시간 48분을 더한다)
				SI32 siGetPersonID		= pclCI->GetPersonID();
				SI32 siGetLoveLevel		= pclMarriage->GetLoveLevel();

				/*
				if ( MARRIAGE_LOVE_LEVEL_10 <= siGetLoveLevel )
				{
					return false;
				}
				*/

					// 아이템에 때라 애정도를 설정을 다르게 한다
					if ( ITEMUNIQUE(8119) == clitem.siUnique )
					{
						siGetLoveLevel += 1;

						// 최대 레벨일때는 변경없이 시간만 늘어나므로 무조건 10으로 해준다
						if ( MARRIAGE_LOVE_LEVEL_10 <= siGetLoveLevel )
						{
							siGetLoveLevel = MARRIAGE_LOVE_LEVEL_10;
						}
					}
					else if ( ITEMUNIQUE(8120) == clitem.siUnique )
					{
						siGetLoveLevel = MARRIAGE_LOVE_LEVEL_10;
					}

					// 배우자 캐릭터도 같이 업데이트
					SI32			siMateCharID	= pclserver->pclCM->GetIDFromPersonID( pclMarriage->GetMatePersonID() );
					cltCharServer*	pclMateChar		= pclserver->pclCM->GetCharServer( siMateCharID );
					if ( NULL == pclMateChar )
					{
						return false;
					}

					if ( FALSE == pclMateChar->IsMarried() )
					{
						return false;
					}

					// 애정도를 미리 설정해 준다. DB다녀온사이 배우자가 또 아이템을 쓰면 그때도 애정도가 올라가야 하니까
					// 내캐릭터
					pclMarriage->SetLoveLevel( siGetLoveLevel );
					pclMarriage->SetLimitHourVary( siSetLimitHourVary );
					pclMarriage->SetUseHourVary( siNowHourVary );

					// 배우자
					pclMateChar->pclMarriage->SetLoveLevel( siGetLoveLevel );
					pclMateChar->pclMarriage->SetLimitHourVary( siSetLimitHourVary );

					// DB로 보내준다
					sDBRequest_Marry_Love_Set clMsg( siGetPersonID, siNowHourVary, siSetLimitHourVary, siGetLoveLevel );
					pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
			}
			//----------------------------------------------------------------------------------
			//군주데이미션 이용권 : 기획 내용 변경으로 아이템 부여/사용 방식이 아닌 퀘스트 부여 방식으로 변경되서 코드 제거
			//else if(clitem.siUnique == ITEMUNIQUE(8154))
			//{	
			//	// 군주데이일 경우에만 사용가능.
			//	if (  true == pclserver->pclEventTimeManager->InEventTime(TEXT("GoonzuDay_Week"), &pclserver->sTime) )
			//	{
			//		if(Make_GoonzuDayMission() == false)
			//		{
			//			// 일단 퀘를 생성하지 못한 상황이 발생하는것에 대해선 보류
			//			return false;
			//		}
			//	}	
			//}
			//------------------------------------------------------------------------------------------------------
			// 간식용 쿠키
			else if ( (ITEMUNIQUE(8122) == clitem.siUnique))
			{
				if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
				{
					if( pclFamily != NULL )
					{
						SI32 siMyFamilyClass = pclFamily->GetMyClass_InFamily() ;

						if ( MYCLASS_IN_FAMILY_CHILDREN == siMyFamilyClass )
						{
							SI32 siMyPersonID = pclCI->GetPersonID()	;
							SI32 siCookieEndTime_Now = pclFamily->GetMyCookie_EndTime( siMyPersonID ) ;	// 현제 나의 쿠키 시간
							SI32 siHourVary = pclserver->pclTime->clDate.GetHourVary()	;			// 현제 서버 시간.

							// 일단 현제 나의 쿠키 시간이 지났는지 확인한다.
							if ( siCookieEndTime_Now <= siHourVary )	// 이전 쿠키 적용 시간이 끝났다. 
							{

								SI32 siEndDatevary   = siHourVary	+ 24	;
								SI32 siLimitDatevary = siHourVary	+ 10	;
								SI32 siCookieLevel   = USE_COOKIE_LEVEL_1	;	// 지났으니 다시 1
								// DB에 쏜다.
								// DB작업을 안해서 디비로 안날리고 바로..클라로 쏴야지 
								sDBRequest_Set_My_CookeEffect clMsg( siMyPersonID, siEndDatevary, siLimitDatevary, siCookieLevel );
								pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
							}
							else	// 아직 쿠키 사용 시간이 남아 있다.
							{
								SI32 siEndDatevary = siHourVary	+ 24	;		// 쿠키 적용 한계 시간
								SI32 siLimitDatevary = siHourVary	+ 10	;	// 쿠키 사용 제한 시간 		

								SI32 siCookieLevel = pclFamily->GetMyCookieLevel(siMyPersonID )	;
								siCookieLevel += 1 ;// 내 레벨에서 한단계 증가 
								if ( siCookieLevel >= USE_COOKIE_LEVEL_5) 
								{
									siCookieLevel = USE_COOKIE_LEVEL_5	;
								}

								// DB에 쏜다.
								// DB작업을 안해서 디비로 안날리고 바로..클라로 쏴야지 
								sDBRequest_Set_My_CookeEffect clMsg( siMyPersonID, siEndDatevary, siLimitDatevary, siCookieLevel );
								pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);

							}
						}
						else
						{
							return false;	
						}
					}
				}				
			}
			// PCK : 아이템 타입 ETC로 이동 (09.06.17)
			else if ( ITEMUNIQUE(8121) == clitem.siUnique )
			{
				SI32 siMonsterKind	= pclClient->GetUniqueFromHash( TEXT("KIND_QUESTEMPERORMASTER") );
				if ( 0 >= siMonsterKind )
				{
					return false;
				}

				SI32 siGetMapIndex	= GetMapIndex();
				if ( (0 > siGetMapIndex) || (MAX_MAP_NUMBER <= siGetMapIndex) )
				{
					return false;
				}

				SI32 siSourceX		= GetX();
				SI32 siSourceY		= GetY();
				SI32 siDestX		= 0;
				SI32 siDestY		= 0;

				bool bIsCreated		= false;

				cltMapCommon* pclMapCommon = pclClient->pclMapManager->pclMapArray[siGetMapIndex];
				if ( NULL == pclMapCommon )
				{
					return false;
				}

				// 혹시 나올 위치를 찾지 못했다면 적당하게 위치를 몇번 더 돌면서 위치를 찾는다
				for ( SI32 siCount=0; siCount<10; siCount++ )
				{
					if ( false == pclMapCommon->FindEmptyAreaForCharLocate(siMonsterKind, siSourceX, siSourceY, 10, &siDestX, &siDestY, true) )
					{
						continue;
					}

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

					SI32 siCreatedID = pclserver->SetNPCChar( siMonsterKind, VILLAGE_NONE, NULL, 0, 0, siDestX, siDestY, siGetMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if ( pclCM->IsValidID(siCreatedID) )
					{
						// 퀘스트 전용 몬스터로 신분 변경
						cltCharServer* pclMonster = (cltCharServer*)pclClient->pclCM->GetCharServer(siCreatedID);
						if ( NULL == pclMonster )
						{
							continue;
						}

						pclMonster->clIdentity.MakeQuest( IDENTITY_QUEST, pclCI->GetPersonID(), GetCharUnique() );
						pclMonster->dwTimerClock = pclserver->CurrentClock;

						bIsCreated = true;

						break;
					}							
				}

				if ( false == bIsCreated )
				{
					return false;
				}
			}

			// 파티퀘스트 보상 아이템 사용시 
			else if (	ITEMUNIQUE(8148) == clitem.siUnique || ITEMUNIQUE(8136) == clitem.siUnique ||
						ITEMUNIQUE(8140) == clitem.siUnique || ITEMUNIQUE(8144) == clitem.siUnique ||
						ITEMUNIQUE(8125) == clitem.siUnique || ITEMUNIQUE(8129) == clitem.siUnique)
			{
				switch (clitem.siUnique)
				{
				// 일반퀘스트 보상 
				case ITEMUNIQUE(8136):
				case ITEMUNIQUE(8140):
				case ITEMUNIQUE(8144):
				case ITEMUNIQUE(8148):
					{
						SI16 siIsHaveBuf[4] = {BUF_EXP_UP, BUF_ATTACKUP, BUF_DEFENSEUP, BUF_MAKEEXPUP};

						// 사용할수 있는 버프들만 추려냅니다.
						for(SI32 i = 0; i < 4; i++)
						{
							if ( pclCI->clBI.clBufInfo.IsHaveBuf(siIsHaveBuf[i]) == true)
								siIsHaveBuf[i] = 0;
						}

						// 사용할수 있는 버프들을 배열에 재 정리 합니다.
						SI32 siCount = 0; // 사용할수 있는 버프의 갯수
						SI16 siSelectBufType[4] = {0,};

						for(i = 0; i < 4; i++)
						{
							if(siIsHaveBuf[i] > 0)
							{
								siSelectBufType[siCount] = siIsHaveBuf[i];
								siCount++;
							}
						}

						// 사용할수 있는 버프가 없다면 Return;
						if(siCount <= 0)
							return false;

						// 여기서 버프 쓸수 있는 버프 체크 후 적용
						SI32 siAddBufIndex = rand()%siCount;

                        
						SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;  // 1일  = 48분
						SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( siSelectBufType[siAddBufIndex], false );

						SI16 siPercent = 0;

						switch(siSelectBufType[siAddBufIndex])
						{
						case BUF_EXP_UP:	siPercent = 20;	break;
						case BUF_ATTACKUP:	siPercent = 20;	break;
						case BUF_DEFENSEUP:	siPercent = 20;	break;
						case BUF_MAKEEXPUP:	siPercent = 20;	break;
						}

						if ( index > -1 )
						{
							stBuf addBuf(siSelectBufType[siAddBufIndex], siPercent, usetime);
							AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
						}							
					}
					break;

				// 피시방퀘스트 보상
				case ITEMUNIQUE(8125):
				case ITEMUNIQUE(8129):
					{
						SI16 siIsHaveBuf[4] = {BUF_EXP_UP, BUF_ATTACKUP, BUF_DEFENSEUP, BUF_MAKEEXPUP};

						// 사용할수 있는 버프들만 추려냅니다.
						for(SI32 i = 0; i < 4; i++)
						{
							if ( pclCI->clBI.clBufInfo.IsHaveBuf(siIsHaveBuf[i]) == true)
								siIsHaveBuf[i] = 0;
						}

						// 사용할수 있는 버프들을 배열에 재 정리 합니다.
						SI32 siCount = 0; // 사용할수 있는 버프의 갯수
						SI16 siSelectBufType[4] = {0,};

						for(i = 0; i < 4; i++)
						{
							if(siIsHaveBuf[i] > 0)
							{
								siSelectBufType[siCount] = siIsHaveBuf[i];
								siCount++;
							}
						}

						// 사용할수 있는 버프가 없다면 Return;
						if(siCount <= 0)
							return false;

						// 여기서 버프 쓸수 있는 버프 체크 후 적용
						SI32 siAddBufIndex = rand()%siCount;


						SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;  // 1일  = 48분
						SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( siSelectBufType[siAddBufIndex], false );

						SI16 siPercent = 0;

						switch(siSelectBufType[siAddBufIndex])
						{
						case BUF_EXP_UP:	siPercent = 50;	break;
						case BUF_ATTACKUP:	siPercent = 50;	break;
						case BUF_DEFENSEUP:	siPercent = 50;	break;
						case BUF_MAKEEXPUP:	siPercent = 50;	break;
						}

						if ( index > -1 )
						{
							stBuf addBuf(siSelectBufType[siAddBufIndex], siPercent, usetime);
							AddBufThroughDB( index, &addBuf , true , itempos , &clitem );
						}		
					}
					break;
				}
			}
			
		}
		break;

	case ITEMTYPE_TICKET:
		{
			if ( clitem.siUnique == ITEMUNIQUE(14300) || clitem.siUnique == ITEMUNIQUE(14400) || clitem.siUnique == ITEMUNIQUE(14500) ) // 모범상,우등상,노력상
			{
				pclClient->pclGameEventManager->CheckPrizeEvent(GetCharUnique(),clitem.siUnique);
			}
		}
		break;

	case ITEMTYPE_CRYSTAL:				//아티팩트 결정체.
		{
			return false;
		}
		break;
	case ITEMTYPE_COMPOSITIONMATERIAL:	//아티팩트 합성제료
		{
			return false;
		}
		break;
	case ITEMTYPE_EVENTITEM:			//이벤트 아이템
		{
			// [진성] 추석 이벤트. 추석월병 버프적용. => 2008-8-7
			if(itemunique == ITEMUNIQUE(13047))
			{
				SI32 usetime	= ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;// 48분간사용				
				SI16 siBufType	=  BUF_STATUE_UP_STR + ( rand() % 5 );
				SI16 index		= pclCI->clBI.clBufInfo.FindEmptySlot( siBufType, false );

				if ( index > -1 )
				{
					stBuf addBuf( siBufType, 5, usetime );
					AddBufThroughDB( index, &addBuf , false , itempos , &clitem );
				}

				return false ;
			}
			//---------------------------------------------------------------------------------------------------------------
			//	보물찾기 이벤트
			//	
			//
			//---------------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13058) )
			{

				//MakeTreasureQuest_Start()	;


			}			
			//-------------------------------------------------------------------------------------------------------------

			// [진성] 추석 이벤트. 에그월병 버프적용. => 2008-8-14
			else if(itemunique == ITEMUNIQUE(13048))
			{
				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;// 48분간사용				
				SI16 index	 = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_EVENT_EGGMOONCAKE, false );

				if ( index > -1 )
				{
					stBuf addBuf( BUF_EVENT_EGGMOONCAKE, 20, usetime );
					AddBufThroughDB( index, &addBuf , false , itempos , &clitem );
				}

				return false ;
			}
			//----------------------------------------------------------------------------------
			// 요술램프
			else if(clitem.siUnique == ITEMUNIQUE(13025))
			{	
				// 특수 퀘스트를 만든다. 
				if(MakeSpecialQuest() == false)
				{
					// 꽝 되었음을 알린다. 
					((cltServer*)pclClient)->SendServerResponseMsg( 0, SRVAL_LOTTOFAIL,  0, 0, GetCharUnique() );
				}
			}
			//------------------------------------------------------------------------------------------------------

			else if ( clitem.siUnique == ITEMUNIQUE(13042) ) // 폭죽
			{
				if( GetCurrentVillageUnique() <= 0 ) 
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_FEAST_FAIL_USE_ONLY_VILLAGE, 0, 0, GetID());					
					return false;
				}

				SI16 FeastKind[4] = {3,5,6,7};

				SI16 randFeast[cltGameMsgRequest_FeastUseEffect::MAX_EFFECT_NUM_AT_ONCE];


				SI16 siStartPosX = (SI16)GetX();
				SI16 siStartPosY = (SI16)GetY();

				randFeast[0] = FeastKind[ rand() % 4 ];

				// 마을에 있는 모든 클라이언트로 폭죽 보내기
				cltGameMsgResponse_FeastUseEffect sendMsg( randFeast, cltGameMsgRequest_FeastUseEffect::MAX_EFFECT_NUM_AT_ONCE, siStartPosX, siStartPosY);
				cltMsg clMsg( GAMEMSG_RESPONSE_FEAST_USEEFFECT, sizeof( sendMsg ), (BYTE*)&sendMsg );

				pclCM->SendAllPersonInVillageMsg((sPacketHeader*)&clMsg, GetCurrentVillageUnique());
			}
			//----------------------------------------------------------------------------------
			// 크리스마스 양말
			else if ( clitem.siUnique == ITEMUNIQUE(13002))
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 ItemUnique = 0;	
				ItemUnique = GetUniqueWithRange( ItemUnique_13002_ItemNum, sizeof(ItemUnique_13002_ItemNum) / sizeof(cltUniqueRange), 1000);

				SI16 brareswitch = 0;
				SI32 usedate	 = 0;					// 사용기간.
				cltItem clDrawItem;						// 추첨해서 나온 결과아이템
				clDrawItem.Init();


				SI32 ItemCnt = 1;
				switch(ItemUnique)
				{
				case 24094:		ItemCnt = 1; usedate = 1; break;
				case 7865:	
				case 3915:		ItemCnt = 3;	break;
				case 18161:		usedate = 7;	break;
				case 23581:		usedate = 7;	break;
				default:		ItemCnt = 1;	break;
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				clDrawItem.SetItemNum( ItemCnt );

				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}

				// 물품 사용하기 퀘스트 진행중이었는지 확인하여 조치한다. 
				IncQuestPara(QUEST_CLEAR_COND_USEITEM, clitem.siUnique, clitem.siItemNum);

				return false;

			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13063) && true == pclClient->IsCountrySwitch(Switch_NewYearEvent_2010) )	// 2010신년이벤트 - 포춘쿠키 - 2009.10.14
			{
				if(pclClient->pclEventTimeManager->InEventTime("Fortune_Event", &pclserver->sTime) == false )	return false	;

				// '흉' 버프를 가지고 있으면 퀘스트만 새로 갱신
				if ( true == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_EVENT_FORTUNE_BAD ) )
				{
					if(MakeQuest_RemoveUnLuckStatus() == false)
					{
						// 일단 퀘를 생성하지 못한 상황이 발생하는것에 대해선 보류

						return false;
					}

					// 디비에서 쿠키를 1개 줄여주고
					if ( clitem.siUnique != 0 )
					{
						sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem );
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}

					cltMissionQuestInfo* pQuestInfo = &pclCI->clQuestInfo.clMissionQuestInfo;

					// 운세 뽑기
					cltValuesRange* pFortune = NULL;

					pFortune = GetValuesWithRange( ItemValue_UnFortune, sizeof(ItemValue_UnFortune) / sizeof(cltValuesRange), 100);
					if ( NULL == pFortune ) return false;

					SI32 siFortuneQuality = pFortune->siPara2;
					SI32 siTextUnique = pFortune->siPara3;

					// '흉' 버프를 받았다고 클라이언트에 패킷 전달
					cltGameMsgResponse_2010NewYear_FortuneResult clinfo( pclCI->GetPersonID(), 3, siTextUnique, 0, pQuestInfo->clQuestClearConditionInfo.siFlag, pQuestInfo->clQuestClearConditionInfo.siPara1, pQuestInfo->clQuestClearConditionInfo.siPara2 );
					cltMsg clMsg( GAMEMSG_RESPONSE_FORTUNERESULT, sizeof(clinfo), (BYTE*)&clinfo );
					SendNetMsg( (sPacketHeader*)&clMsg );

					return false;
				}

				// '흉' 버프가 없을땐 정상적인 루틴으로 처리
				// 빈 인벤 공간 확인?
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(ItemValue_13063, sizeof(ItemValue_13063) / sizeof(cltValuesRange), 100);
				if(pItemValue == NULL)			return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				cltItem clOutPutItem;
				clOutPutItem.Init();

				// 나온 아이템에 대한 처리
				if ( -1 == ItemUnique )		// 운세일경우엔 운세 관련 처리
				{
					SI32 siBuffNameUnique = -1;		// 버프 이름의 유니크를 저장할 변수

					//////////////////////////////////////////////////////////////////////////
					// 운세 뽑기
					cltValuesRange* pFortune = NULL;

					pFortune = GetValuesWithRange( ItemValue_Fortune, sizeof(ItemValue_Fortune) / sizeof(cltValuesRange), 1000);
					if ( NULL == pFortune ) return false;

					SI32 siFortuneQuality = pFortune->siPara2;
					SI32 siTextUnique = pFortune->siPara3;


					//////////////////////////////////////////////////////////////////////////
					// 운세의 결과 처리
					if ( 1 == siFortuneQuality )		// 대길 일때
					{
						// 신행운상자 지급 (13358)
						ItemUnique = 13358;

						if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
							return false;

						Count = 1;
						clOutPutItem.SetItemNum( Count );

						//------------------------------------------
						// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
						//------------------------------------------
						SI16 invpos;
						cltItem cltoitem;
						SI32 rtnvalue;
						if(CanAddInv(INVMODE_ALL, &clOutPutItem, &invpos, &cltoitem, &rtnvalue) == false)
						{
							((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
							return false;
						}

						if ( clitem.siUnique != 0 )
						{
							sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clOutPutItem, pclItemManager->CanPile(clOutPutItem.siUnique), clOutPutItem.IsCreateSID(pclItemManager), false );
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}

						//////////////////////////////////////////////////////////////////////////
						// hp, mp를 랜덤하게 50~500 채운다.
						SI16 siHealAmount = 0;

						siHealAmount = rand() % 451 + 50;	// 힐량은 50~500 사이
						IncreaseLife( siHealAmount );

						// 사용자인 경우, 클라이언트로 보내준다. 
						if(IsPC(GetCharUnique()))
						{
							cltGameMsgResponse_IncLife clInfo(GetCharUnique(), pclCI->clIP.GetLife(),  siHealAmount, clPB.siReservedLife, 0, INCMANA_REASON_HEAL);
							cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
							SendNetMsg((sPacketHeader*)&clMsg);
						}

						siHealAmount = rand() % 451 + 50;	// 힐량은 50~500 사이
						IncreaseMana( siHealAmount );

						// 사용자인 경우, 클라이언트로 보내준다. 
						if(IsPC(GetCharUnique()))
						{
							cltGameMsgResponse_IncMana clInfo(GetCharUnique(), pclCI->clIP.GetMana(),  siHealAmount, INCMANA_REASON_HEAL);
							cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
							SendNetMsg((sPacketHeader*)&clMsg);
						}
						//////////////////////////////////////////////////////////////////////////

						// '대길'이 나왔다고 클라이언트에 패킷 전달 (클라에 필요한 대화상자등의 처리는 이 패킷 처리부에서)
						cltGameMsgResponse_2010NewYear_FortuneResult clinfo( pclCI->GetPersonID(), siFortuneQuality, siTextUnique, 0, 0, 0, 0 );
						cltMsg clMsg( GAMEMSG_RESPONSE_FORTUNERESULT, sizeof(clinfo), (BYTE*)&clinfo );
						SendNetMsg( (sPacketHeader*)&clMsg );

						return false;
					}
					else if ( 2 == siFortuneQuality )	// 길 일때
					{
						SI16 index = -1;
						index = pclCI->clBI.clBufInfo.FindBufGetTypeSlot(BUF_GET_TYPE_FORTUNECOOKIE);
						// 1. '길'버프 상태인가?
						if ( -1 < index )
						{
							// 기존에 걸려있던 버프를 삭제한다.
							stBuf DelBuf( pclCI->clBI.clBufInfo.m_stBuf[index].siBufType, 0, 0, 0 );
							sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), index, &DelBuf );
							((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}

						//////////////////////////////////////////////////////////////////////////
						cltValuesRange clBufRange[30];
						ZeroMemory( clBufRange, sizeof(clBufRange) );

						SI32 siOriginalCount = sizeof(ItemValue_Fortune_Buf) / sizeof(ItemValue_Fortune_Buf[0]);
						siOriginalCount = min( siOriginalCount, 30 );

						SI32 siRealCount	= 0;	// 실제 랜덤을 돌려야 되는 갯수
						SI32 siRealPercent	= 0;	// 총합 확률

						for ( SI32 siIndex=0; siIndex<siOriginalCount; siIndex++ )
						{
							// 갖고 있지 않은 버프만 셋팅해 준다
							if ( false == pclCI->clBI.clBufInfo.IsHaveBuf(ItemValue_Fortune_Buf[siIndex].siPara1) )
							{
								clBufRange[siRealCount++].Set( ItemValue_Fortune_Buf[siIndex].siPercent, ItemValue_Fortune_Buf[siIndex].siPara1, ItemValue_Fortune_Buf[siIndex].siPara2, ItemValue_Fortune_Buf[siIndex].siPara3 );

								siRealPercent += ItemValue_Fortune_Buf[siIndex].siPercent;
							}
						}
						
						// 새로 걸어줄 버프를 선택
						cltValuesRange* pFortuneBuf = NULL;
						pFortuneBuf = GetValuesWithRange(clBufRange, siRealCount, siRealPercent);
						if(pFortuneBuf == NULL)			return false;

						SI32 siBufType			= pFortuneBuf->siPara1;
						SI32 siBufValue			= pFortuneBuf->siPara2;
						siBuffNameUnique		= pFortuneBuf->siPara3;	// 버프 이름 유니크
						SI32 siUseTime			= ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;// 48분간사용

						// 새로 걸어줄 버프랑 같은 타입의 버프가 있으면 지운다.
						SI16 index0 = -1;
						index0 = pclCI->clBI.clBufInfo.FindEmptySlot( siBufType );
						if ( -1 >= index0 )
						{
							return false;
						}

						stBuf addBuf0( siBufType, siBufValue, siUseTime, BUF_GET_TYPE_FORTUNECOOKIE );
						AddBufThroughDB( index0, &addBuf0 , false , itempos , &clitem );

						// 디비에서 쿠키를 1개 줄여주고
						if ( clitem.siUnique != 0 )
						{
							sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem );
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}

						//////////////////////////////////////////////////////////////////////////
						// hp, mp를 랜덤하게 50~500 채운다.
						SI16 siHealAmount = 0;

						siHealAmount = rand() % 451 + 50;	// 힐량은 50~500 사이
						IncreaseLife( siHealAmount );

						// 사용자인 경우, 클라이언트로 보내준다. 
						if(IsPC(GetCharUnique()))
						{
							cltGameMsgResponse_IncLife clInfo(GetCharUnique(), pclCI->clIP.GetLife(),  siHealAmount, clPB.siReservedLife, 0, INCMANA_REASON_HEAL);
							cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
							SendNetMsg((sPacketHeader*)&clMsg);
						}

						siHealAmount = rand() % 451 + 50;	// 힐량은 50~500 사이
						IncreaseMana( siHealAmount );

						// 사용자인 경우, 클라이언트로 보내준다. 
						if(IsPC(GetCharUnique()))
						{
							cltGameMsgResponse_IncMana clInfo(GetCharUnique(), pclCI->clIP.GetMana(),  siHealAmount, INCMANA_REASON_HEAL);
							cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
							SendNetMsg((sPacketHeader*)&clMsg);
						}
						//////////////////////////////////////////////////////////////////////////

						// '길'이 나왔다고 클라에 패킷 전달 ('길'에 대한 클라쪽의 대화상자등의 처리는 이 패킷 처리부에서)
						cltGameMsgResponse_2010NewYear_FortuneResult clinfo( pclCI->GetPersonID(), siFortuneQuality, siTextUnique, siBuffNameUnique, 0, 0, 0 );
						cltMsg clMsg( GAMEMSG_RESPONSE_FORTUNERESULT, sizeof(clinfo), (BYTE*)&clinfo );
						SendNetMsg( (sPacketHeader*)&clMsg );

						return false;
					}
					else if ( 3 == siFortuneQuality )	// 뽑은 운세가 '흉' 일때
					{
						//////////////////////////////////////////////////////////////////////////

						// '길' 버프가 이미 있으면 지워준다
						SI16 index = -1;
						index = pclCI->clBI.clBufInfo.FindBufGetTypeSlot( BUF_GET_TYPE_FORTUNECOOKIE );
						if ( -1 < index )
						{
							// 기존에 걸려있던 버프를 삭제한다.
							stBuf DelBuf( pclCI->clBI.clBufInfo.m_stBuf[index].siBufType, 0, 0, 0 );
							sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), index, &DelBuf );
							((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}

						//////////////////////////////////////////////////////////////////////////

						// '흉' 버프를 준다.
						SI32 siUseTime	= ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 21600 ;// 30일을 준다.(이벤트 기간에만 있을 버프, 대략30일로 줌)
						index			= pclCI->clBI.clBufInfo.FindEmptySlot( BUF_EVENT_FORTUNE_BAD, false );

						stBuf addBuf( BUF_EVENT_FORTUNE_BAD, 0, siUseTime );
						AddBufThroughDB( index, &addBuf , false , itempos , &clitem );

						//'흉' 버프를 고칠수 있는 퀘스트를 생성
						if ( false == MakeQuest_RemoveUnLuckStatus() )
						{
							// 일단 퀘를 만들지 못한 경우에 대해선 생각하지 않는다.

							return false;
						}

						cltMissionQuestInfo* pQuestInfo = &pclCI->clQuestInfo.clMissionQuestInfo;

						// 디비에서 쿠키를 1개 줄여주고
						if ( clitem.siUnique != 0 )
						{
							sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem );
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}

						// '흉' 버프를 받았다고 클라이언트에 패킷 전달
						cltGameMsgResponse_2010NewYear_FortuneResult clinfo( pclCI->GetPersonID(), siFortuneQuality, siTextUnique, 0, pQuestInfo->clQuestClearConditionInfo.siFlag, pQuestInfo->clQuestClearConditionInfo.siPara1, pQuestInfo->clQuestClearConditionInfo.siPara2 );
						cltMsg clMsg( GAMEMSG_RESPONSE_FORTUNERESULT, sizeof(clinfo), (BYTE*)&clinfo );
						SendNetMsg( (sPacketHeader*)&clMsg );

						return false;
					}

					return false;
				}
				else	// 아이템일경우엔 아이템 관련 처리
				{
					Count = (13062 == ItemUnique) ? (rand()%3+1) : 1 ; // 쿠키조각일경우엔 1~3개로 갯수 조정

					if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
						return false;

					clOutPutItem.SetItemNum( Count );

					//------------------------------------------
					// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
					//------------------------------------------
					SI16 invpos;
					cltItem cltoitem;
					SI32 rtnvalue;
					if(CanAddInv(INVMODE_ALL, &clOutPutItem, &invpos, &cltoitem, &rtnvalue) == false)
					{
						((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
						return false;
					}

					if ( clitem.siUnique != 0 )
					{
						sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clOutPutItem, pclItemManager->CanPile(clOutPutItem.siUnique), clOutPutItem.IsCreateSID(pclItemManager), false );
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}

					// 클라에 쿠키에서 아이템을 뽑았다고 패킷 전달 (아이템 뽑은것에 대한 클라쪽의 대화상자등의 처리는 이 패킷 처리부에서)

					// 팅키는 100으로 약속 (의미는 없다)
					NTCHARString128 pUserName = GetName();
					cltGameMsgResponse_2010NewYear_FortuneResult clinfo( pclCI->GetPersonID(), 100, 0, 0, 0, 0, 0, pUserName );
					cltMsg clMsg( GAMEMSG_RESPONSE_FORTUNERESULT, sizeof(clinfo), (BYTE*)&clinfo );
					pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

					return false;
				}

				return false;
			}

			//----------------------------------------------------------------------------------
			// 황금알
			else if(clitem.siUnique == ITEMUNIQUE(13005) )
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				// 나올 아이템 목록
				const SI32 siBoxItemList[]=
				{
					ITEMUNIQUE(5350),
						ITEMUNIQUE(5355),
						ITEMUNIQUE(5360),
						ITEMUNIQUE(5365),
						ITEMUNIQUE(5370),
						ITEMUNIQUE(2710),
						ITEMUNIQUE(24020),
						ITEMUNIQUE(2735),
						ITEMUNIQUE(2685),
						ITEMUNIQUE(2680)	
				};


				SI32 rndItemIndex = rand() % ( sizeof(siBoxItemList) / sizeof(SI32) );

				SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				SI32 Itemnum = 1;

				SI32 randItemnumVal = rand() % 100;

				// 확률에 딸느 갯수 조정
				if( randItemnumVal < 10 )			{		Itemnum = 1;		}
				else if( randItemnumVal < 24 )		{		Itemnum = 2;		}
				else if( randItemnumVal < 44 )		{		Itemnum = 3;		}
				else if( randItemnumVal < 58 )		{		Itemnum = 4;		}
				else if( randItemnumVal < 70 )		{		Itemnum = 5;		}
				else if( randItemnumVal < 80 )		{		Itemnum = 6;		}
				else if( randItemnumVal < 88 )		{		Itemnum = 7;		}
				else if( randItemnumVal < 94 )		{		Itemnum = 8;		}
				else if( randItemnumVal < 98 )		{		Itemnum = 9;		}
				else if( randItemnumVal < 100 )		{		Itemnum = 10;		}


				clDrawItem.SetItemNum( Itemnum );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}			
			//----------------------------------------------------------------------------------
			// 황금라쿤알
			else if( clitem.siUnique == ITEMUNIQUE(13028) )	
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = 0;
				SI32 usedate = 0;
				SI16 brareswitch = 0;
				//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
				if(pclClient->IsCountrySwitch(Switch_RaccoonEvent_Change_Goonzu))	// 군주 라쿤으로 변경 되면 황금 라쿤알은 아래 리스트로 변경 
				{
					SI32 rndVal = rand() % 1000;
					if( rndVal < 2 )				{		rndItemUnique = ITEMUNIQUE(5600);							}	// 작은메달					0.2%
					else if (rndVal < 4)			{		rndItemUnique = ITEMUNIQUE(5370);							}	// 축복의 돌				0.2%
					else if (rndVal < 8)			{		rndItemUnique = ITEMUNIQUE(5365);							}	// 땅의 속성석				0.4%
					else if (rndVal < 12)			{		rndItemUnique = ITEMUNIQUE(5360);							}	// 바람의 속성석			0.4%
					else if (rndVal < 16)			{		rndItemUnique = ITEMUNIQUE(5350);							}	// 불의 속성석				0.4%
					else if (rndVal < 20)			{		rndItemUnique = ITEMUNIQUE(5355);							}	// 물의 속성석				0.4%
					else if (rndVal < 110)			{		rndItemUnique = ITEMUNIQUE(5380);							}	// 티타니움					9%
					else if (rndVal < 200)			{		rndItemUnique = ITEMUNIQUE(6140);							}	// 마호가니					9%
					else if (rndVal < 290)			{		rndItemUnique = ITEMUNIQUE(7175);							}	// 아라미드					9%
					else if (rndVal < 380)			{		rndItemUnique = ITEMUNIQUE(7185);							}	// 검은실크					9%
					else if (rndVal < 690)			{		rndItemUnique = ITEMUNIQUE(24075);							}	// 프리미엄 개인상점 이용권 31%
					else if (rndVal < 1000)			{		rndItemUnique = ITEMUNIQUE(24047);							}	// 자막고백이용권			31%
				}
				else
				{
					SI32 rndVal = rand() % 100;
					//KHY - 1217 - 황금 라쿤알 보상 변경. - 
					if(pclClient->IsCountrySwitch(Switch_RaccoonGoldeggChange))
					{
						if( rndVal < 1 )		{		rndItemUnique = ITEMUNIQUE(5600);							}
						else if( rndVal < 2 )	{		rndItemUnique = ITEMUNIQUE(23090);		usedate = 7;		}
						else if( rndVal < 8 )	{		rndItemUnique = ITEMUNIQUE(2720);							}
						else if( rndVal < 15 )	{		rndItemUnique = ITEMUNIQUE(2715);							}
						else if( rndVal < 57 )	{		rndItemUnique = ITEMUNIQUE(24075);							}
						else if( rndVal < 100 )	{		rndItemUnique = ITEMUNIQUE(24047);							}
					}
					else
					{
						if( rndVal < 2 )		{		rndItemUnique = ITEMUNIQUE(5600);							}
						else if( rndVal < 3 )	{		rndItemUnique = ITEMUNIQUE(5370);							}
						else if( rndVal < 8 )	{		rndItemUnique = ITEMUNIQUE(5365);							}
						else if( rndVal < 12 )	{		rndItemUnique = ITEMUNIQUE(5360);							}
						else if( rndVal < 16 )	{		rndItemUnique = ITEMUNIQUE(5350);							}
						else if( rndVal < 20 )	{		rndItemUnique = ITEMUNIQUE(5355);							}
						else if( rndVal < 60 )	{		rndItemUnique = ITEMUNIQUE(24075);							}
						else if( rndVal < 100 )	{		rndItemUnique = ITEMUNIQUE(24047);							}
					}
				}
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
				{
					return false;
				}

				SI32 rndItemNum = 1; // 1 고정.

				clDrawItem.SetItemNum( rndItemNum );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}
			//----------------------------------------------------------------------------------
			// 장미 상자
			else if( clitem.siUnique == ITEMUNIQUE(13021) )	
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = 0;
				SI32 rndVal = rand() % 100;

				//KHY - 0218  - 화이트데이 
				if( rndVal < 5 )			{		rndItemUnique = ITEMUNIQUE(5600);		}
				else if( rndVal < 15 )		{		rndItemUnique = ITEMUNIQUE(3899); 		} //사랑의 캔디
				else if( rndVal < 25 )		{		rndItemUnique = ITEMUNIQUE(3901);		} //우정의 캔디
				else if( rndVal < 30 )		{		rndItemUnique = ITEMUNIQUE(24094);		}
				else if( rndVal < 45 )		{		rndItemUnique = ITEMUNIQUE(2730);		}
				else if( rndVal < 60 )		{		rndItemUnique = ITEMUNIQUE(7860);		}
				else if( rndVal < 65 )		{		rndItemUnique = ITEMUNIQUE(24040);		}
				else if( rndVal < 70 )		{		rndItemUnique = ITEMUNIQUE(5400);		}
				else if( rndVal < 90 )		{		rndItemUnique = ITEMUNIQUE(24000);		}
				else if( rndVal < 100 )		{		rndItemUnique = ITEMUNIQUE(2705);		}

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				SI32 rndItemNum = 1;

				clDrawItem.SetItemNum( rndItemNum );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}
			//-----------------------------------------------------------------------------------------------
			// 장미 상자
			else if( clitem.siUnique == ITEMUNIQUE(13015) )	
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = 0;
				SI32 rndVal = rand() % 100;

				//KHY - 0115 발렌타인데이 이벤트. - 아이템 종류, 확률 수정.
				if( rndVal < 5 )			{			rndItemUnique = ITEMUNIQUE(5600);			}
				else if( rndVal < 15 )		{			rndItemUnique = ITEMUNIQUE(3890);			}
				else if( rndVal < 25 )		{			rndItemUnique = ITEMUNIQUE(3895);			}
				else if( rndVal < 30 )		{			rndItemUnique = ITEMUNIQUE(24094);			}
				else if( rndVal < 45 )		{			rndItemUnique = ITEMUNIQUE(2730);			}
				else if( rndVal < 60 )		{			rndItemUnique = ITEMUNIQUE(7860);			}
				else if( rndVal < 65 )		{			rndItemUnique = ITEMUNIQUE(24040);			}
				else if( rndVal < 70 )		{			rndItemUnique = ITEMUNIQUE(5400);			}
				else if( rndVal < 90 )		{			rndItemUnique = ITEMUNIQUE(24000);			}
				else if( rndVal < 100 )		{			rndItemUnique = ITEMUNIQUE(2705);			}

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				SI32 rndItemNum = 1;

				clDrawItem.SetItemNum( rndItemNum );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;

			}
			////-----------------------------------------------------------------------------------------------
			//// 보물찾기 이벤트 낡은 쇠뭉치.
			//else if( clitem.siUnique == ITEMUNIQUE(13347) )
			//{
			//	if( IsBlankInv(1,INVMODE_ALL) == false )
			//	{
			//		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
			//		return false;
			//	}

			//	SI32 rndItemUnique = 0;
			//	SI32 rndVal = rand() % 100;
			//	SI32 rndItemNum = 0;

			//	if( rndVal < 20 )			{			rndItemUnique = ITEMUNIQUE(5020);			rndItemNum = rand()%3+1 ;	}
			//	else if( rndVal < 40 )		{			rndItemUnique = ITEMUNIQUE(5010);			rndItemNum = rand()%3+1 ;	}
			//	else if( rndVal < 55 )		{			rndItemUnique = ITEMUNIQUE(5050);			rndItemNum = rand()%3+1 ;	}
			//	else if( rndVal < 70 )		{			rndItemUnique = ITEMUNIQUE(5040);			rndItemNum = rand()%3+1 ;	}

			//	else if( rndVal < 75 )		{			rndItemUnique = ITEMUNIQUE(13051);			rndItemNum = 1	; }
			//	else if( rndVal < 80 )		{			rndItemUnique = ITEMUNIQUE(13052);			rndItemNum = 1	;}
			//	else if( rndVal < 85 )		{			rndItemUnique = ITEMUNIQUE(13053);			rndItemNum = 1	;}
			//	else if( rndVal < 90 )		{			rndItemUnique = ITEMUNIQUE(13054);			rndItemNum = 1	;}
			//	else if( rndVal < 95 )		{			rndItemUnique = ITEMUNIQUE(13055);			rndItemNum = 1	;}
			//	else if( rndVal < 100 )		{			rndItemUnique = ITEMUNIQUE(13056);			rndItemNum = 1	;}

			//	SI16 brareswitch = 0;
			//	SI32 usedate = 0;
			//	cltItem clDrawItem;					// 추첨해서 나온 결과아이템
			//	clDrawItem.Init();

			//	if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
			//		return false;
			//	clDrawItem.SetItemNum( rndItemNum );
			//	//------------------------------------------
			//	// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
			//	//------------------------------------------
			//	SI16 invpos;
			//	cltItem cltoitem;
			//	SI32 rtnvalue;
			//	if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
			//	{
			//		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
			//		return false;
			//	}

			//	if ( clitem.siUnique != 0 )
			//	{
			//		sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager));
			//		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			//	}

			//	return false;

			//}
			////-------------------------------------------------------------------------------------------------------------
			// 보물찾기 이벤트.
			// 재앙 셋트
			// -60%확률로 일반 몬스터-
			// -35%확률로 게임내 유령 NPC-
			// -5%확률로 영웅 유령 NPC-
			// 내용은 모두 임시 
			else if(clitem.siUnique == ITEMUNIQUE(13057))
			{
				if ( pclClient->IsCountrySwitch(Switch_TreasureEvent)  )
				{
					if( pclClient->pclEventTimeManager->InEventTime("TreasureEvent", &pclserver->sTime) == true )
					{
						cltServer *pclserver = (cltServer*)pclClient;

						if (pclserver->pclCM->pclIDManager->GetNPCCharNumber() > MAX_NPC_NUMBER - 1000)
						{
							return false;
						}
						if (GetMapIndex() == MAPINDEX_BEGINNER1 || GetMapIndex() == MAPINDEX_PALACE || GetMapIndex() == MAPINDEX_MOFUMOFULAND)
						{
							return false;
						}
						SI32 pos = 0 ;
						SI32 x, y;
						SI32 tempx, tempy, level;
						SI32 mapindex = 0;

						bool bSelectKind = false;

						SI32 siMonsterKind	=	0	;
						SI32 TempKind	=	0	;
						// 일단 무슨 몬스터를 소환할지 결정 해야 겠지
						// -60%확률로 일반 몬스터-
						// -35%확률로 게임내 유령 NPC-
						// -5%확률로 영웅 유령 NPC-
						// 일반 몬스터인지 혹 유령들인지 결정 한다.
						//SI32 TempKind = GetUniqueWithRange( ItemUnique_13057_Treasure, sizeof(ItemUnique_13057_Treasure) / sizeof(cltUniqueRange), 1000);

						SI32 siSelectMonClass = rand() % 100 ;
						if ( siSelectMonClass <60 )	// 일반 몬스터 
						{
							TempKind	=	3	;
						}
						else if (siSelectMonClass < 95 )	//	일번유령
						{
							TempKind	=	1	;
						}
						else if (siSelectMonClass <100 )	// 유령
						{
							TempKind	=	2	;
						}

						if ( TempKind == 1  )	// 일반 유령을 등장 시킨다
						{
							SI32 rndVal = rand() % 100;
							if( rndVal < 17 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_DAEGUM1");		}	//17
							else if( rndVal < 34 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_CATAR1");		}	//17
							else if( rndVal < 51 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_RIFLE1");		}	//17
							else if( rndVal < 66 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_DAEGUM2");		}	//15
							else if( rndVal < 81 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_CATAR2");		}	//15
							else if( rndVal < 96 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_RIFLE2");		}	//15
							else if( rndVal < 98 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_REDSHABEL");		}	//2
							else if( rndVal < 100 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_TREASURE_BLUESHABEL");		}	//2
						}
						else if ( TempKind == 2  )	// 영웅 유령을 등장 시킨다.
						{
							siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_CURSEHIRO");
						}
						else	// 일반 몬스터들을 랜덤 하게 등장 시킨다.
						{
							SI32 rndVal = rand() % 100;
							if( rndVal < 18 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_RACCOON");		}	//18
							else if( rndVal < 33 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_VINE");		}	//15
							else if( rndVal < 46 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_GOTPA");		}	//13
							else if( rndVal < 59 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_MARIONETTE");		}	//13
							else if( rndVal < 71 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_SHARK");		}	//12
							else if( rndVal < 81 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_CANNONMOL");		}	//10
							else if( rndVal < 89 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_DEACON");		}	//8
							else if( rndVal < 96 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_REDBAT");		}	//7
							else if( rndVal < 99 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_BROWNWOLF");		}	//3
							else if( rndVal < 100 )			{			siMonsterKind	=	pclClient->GetUniqueFromHash("KIND_GREATWALLSLIME");		}	//1
						}

						if(siMonsterKind > 0 )
						{
							mapindex = GetMapIndex();

							tempx = GetX();
							tempy = GetY();

							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(	siMonsterKind , tempx, tempy, 10, &x, &y, true) == false)
							{
								break;
							}

							level = rand() % 50 + 1;

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

							cltIdentity clIdentity;
							clIdentity.MakeTreasureEvent(IDENTITY_TREASURE_EVENT);

							SI32 id = pclserver->SetNPCChar( siMonsterKind , VILLAGE_NONE, &clIdentity, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
							cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

							pclchar->dwTimerClock = pclserver->CurrentClock;
							pclchar->siCharMode = CHARMODE_TREASUREEVENT;

						}
						else
						{
							return false;
						}
					}

				}
			}
			//[진성] 서버 이벤트. NDE 뱃지 소모. => 2008-6-18
			else if( clitem.siUnique == ITEMUNIQUE(13043) )	
			{
				/*if ( itempos <= 0 || itempos >= MAX_ITEM_PER_PERSON ) 
				return false;*/

					// DB와 버프로 통신을 하는 중이기 때문에 아무처리도 하지않는다.
					if(IsNowAddBufThroughDB())		return false;

				const SI32 BuffKindNum = 6;													// 아이템 소모 후 얻을 수 있는 버프 개수.
				cltUniqueRange SetBuffKind[BuffKindNum];									// 버프의 종류와 나오는 확률.
				ZeroMemory(SetBuffKind, sizeof(SetBuffKind));
				for(SI32 Count = 0; Count < BuffKindNum - 1; ++Count)	
				{
					if(Count >= BuffKindNum)
						break;

					SetBuffKind[Count].siUnique  = BUF_STATUE_UP_STR + Count;
					SetBuffKind[Count].siPercent = 19;
				}
				SetBuffKind[BuffKindNum-1].siUnique  = BUF_EXP_UP;
				SetBuffKind[BuffKindNum-1].siPercent = 5;

				SI32 CompleteBuffKind = GetUniqueWithRange(SetBuffKind, BuffKindNum, 100);
				if(0 < CompleteBuffKind)
				{
					SI16 siPercent = 30; 
					if(BUF_EXP_UP == CompleteBuffKind)
					{
						siPercent = 100;
					}

					SI16 siIndex = pclCI->clBI.clBufInfo.FindBufSlot(CompleteBuffKind);
					if(0 > siIndex)
						siIndex = pclCI->clBI.clBufInfo.FindEmptySlot(CompleteBuffKind, false);	

					//버프는 48분.
					SI32 siUsetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;

					// 적용할 버프가 있었으면 해당버프 시간 연장.
					if(true == pclCI->clBI.clBufInfo.IsHaveBuf(CompleteBuffKind)) 
					{
						SI32 siOverTime = pclCI->clBI.clBufInfo.GetBuf(CompleteBuffKind)->siOverTime;
						SI32 siCurTime  = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary();

						// 현재 사용중인 버프의 남은시간을 계산.
						SI32 RemainTime = siOverTime - siCurTime;

						// 이전 버프를 제거하고 
						stBuf DelBuf(CompleteBuffKind, 0, 0);
						sDBRequest_DeleteBuf clMsg(pclCI->GetPersonID(), siIndex, &DelBuf);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

						// 같은 버프인데 시간만 연장해서 다시 세팅.
						stBuf addBuf(CompleteBuffKind, siPercent, RemainTime + siUsetime);
						AddBufThroughDB(siIndex, &addBuf , false, itempos, &clitem, 0);
					}
					// 적용할 버프가 없었으면 새로 버프 적용.
					else
					{
						stBuf addBuf(CompleteBuffKind, siPercent, siUsetime);
						AddBufThroughDB(siIndex, &addBuf , false, itempos, &clitem, 0);
					}
				}
				return false;
			}
			//-----------------------------------------------------------------------------------------------
			else if(
				clitem.siUnique == ITEMUNIQUE(13044) ||		// 금메달	(임시 유니크)
				clitem.siUnique == ITEMUNIQUE(13045) ||		// 은메달	(임시 유니크)
				clitem.siUnique == ITEMUNIQUE(13046)		// 동메달	(임시 유니크)
				)	
			{
				if (pclCI->clBI.clBufInfo.IsHaveBuf( BUF_OLYMPICMEDAL ))	return false;
				cltUniqueRange SetBuffKind;
				SetBuffKind.Init();
				switch( clitem.siUnique ) {
	case ITEMUNIQUE(13044) :{	// 금메달
		SetBuffKind.siUnique	=	BUF_OLYMPICMEDAL	;
		SetBuffKind.siPercent	=	20	;
							}
							break;
	case ITEMUNIQUE(13045) :{// 은메달
		SetBuffKind.siUnique	=	BUF_OLYMPICMEDAL	;
		SetBuffKind.siPercent	=	10	;
							}
							break;
	case ITEMUNIQUE(13046) :{// 동메달
		SetBuffKind.siUnique	=	BUF_OLYMPICMEDAL	;
		SetBuffKind.siPercent	=	5	;
							}
							break;
				}
				if( 0 < SetBuffKind.siUnique && 
					0 < SetBuffKind.siPercent &&
					false == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_OLYMPICMEDAL ))
				{
					SI16 siIndex = pclCI->clBI.clBufInfo.FindEmptySlot(SetBuffKind.siUnique, false);
					//버프는 48분.
					SI32 siUsetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
					stBuf addBuf(SetBuffKind.siUnique, SetBuffKind.siPercent, siUsetime);
					AddBufThroughDB(siIndex, &addBuf , false, itempos, &clitem, 0);
				}
				return false;
			}
			//----------------------------------------------------------------------------------
			// Trick or Treat 사탕
			else if( clitem.siUnique == ITEMUNIQUE(13050) )	
			{
				if (pclClient->IsCountrySwitch(Switch_HalloweenEvent) == false) 			return false;

				if( pclClient->pclEventTimeManager->InEventTime("HalloweenEvent", &pclserver->sTime) == true )
				{
					SI32	siType = rand() % 2;
					SI32	siSkillUnique = 0;

					if( siType == 0 )
					{
						SI32	siAddExp = (rand() % 40001) + 10000;		// 10,000 ~ 50,000
						IncreaseExp( (SI64)siAddExp, GETEXP_REASON_EVENT );
					}
					else
					{
						SI32	siAddExp = (rand() % 4001) + 1000;		// 1,000 ~ 5,000
						SI16 siSkillCount = 0;
						SI16 siSkills[MAX_SKILL_NUMBER] = { 0, };

						for(SI32 i=0; i<MAX_SKILL_NUMBER; i++)
						{
							cltSkillInfo* pclSkillInfo = pclClient->pclSkillManager->pclSkillInfo[i];
							if(pclSkillInfo)
							{
								switch(pclSkillInfo->siSkillType)
								{
								case SKILLTYPE_R_USEWEAPON:
								case SKILLTYPE_R_MAKEWEAPON:
								case SKILLTYPE_R_ARMOR:
								case SKILLTYPE_R_MAKEHARVEST:
								case SKILLTYPE_R_HARVEST:
								case SKILLTYPE_R_MAKECLOTH:
								case SKILLTYPE_R_MAKEACCESSARY:
								case SKILLTYPE_R_ETC:
									// 전투력 향상술은 제외.	
									//case SKILLTYPE_FIGHT:
									{
										siSkills[siSkillCount++] = pclSkillInfo->siUnique;
									}
									break;
								}
							}
						}

						if(siSkillCount > 0)		siSkillUnique = siSkills[rand() %siSkillCount];
						if(siSkillUnique > 0)		SetSkillUpdate( siSkillUnique, siAddExp );
					}
				}
				else
					return false;
			}
			//-----------------------------------------------------------------------------------------------

			// [춘기] - 눈 결정 버프 (48분간 몬스터 사냥시 일정 확률로 호랑가시나무 열매 획득 가능)
			else if(clitem.siUnique == ITEMUNIQUE(13059))
			{
				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.

				SI16 index	= pclCI->clBI.clBufInfo.FindEmptySlot( BUF_CHRISTMASEVENT_SNOWBLESS, false );

				if ( index > -1 )
				{
					stBuf addBuf( BUF_CHRISTMASEVENT_SNOWBLESS, 0, usetime );
					AddBufThroughDB( index, &addBuf , false , itempos , &clitem );
				}

				return false ;
			}
			// [춘기] - 크리스마스 장식 버프 ( 48분간 탈것의 모든능력치를 +5 올려주는 버프)
			else if(clitem.siUnique == ITEMUNIQUE(13061))
			{
				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.

				SI16 index	= pclCI->clBI.clBufInfo.FindEmptySlot( BUF_CHRISTMASEVENT_DECORATION, false );

				if ( index > -1 )
				{
					stBuf addBuf( BUF_CHRISTMASEVENT_DECORATION, 0, usetime );
					AddBufThroughDB( index, &addBuf , false , itempos , &clitem );
				}

				return false ;
			}
			else if( ITEMUNIQUE(13064) == clitem.siUnique )	// [진성] 자이언트 라쿤 부활알.
			{
				if( IsBlankInv(1, INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI16	brareswitch		= 0;
				SI32	usedate			= 0;			// 사용기간.
				SI32	siItemUnique	= 0;
				cltItem clInputItem;					// 추첨해서 나온 결과아이템
				clInputItem.Init();
				
				switch( rand()%4 )
				{
					case 0:	siItemUnique = ITEMUNIQUE(13065); break;
					case 1:	siItemUnique = ITEMUNIQUE(13066); break;
					case 2:	siItemUnique = ITEMUNIQUE(13067); break;
					case 3:	siItemUnique = ITEMUNIQUE(13068); break;
				}

				if( pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clInputItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				clInputItem.SetItemNum( 1 );

				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16	invpos;
				cltItem cltoitem;
				SI32	rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clInputItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clInputItem, pclItemManager->CanPile(clInputItem.siUnique), clInputItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					pclClient->pclLog->FilePrint( "config\\GiantRaccoonResurrectEvent.log" , "[OpenItem] %s : ", pclItemManager->GetName( clInputItem.siUnique ) );
				}
			}
		}
		break;
	case ITEMTYPE_BOX://박스아이템 - 박스를 열면 다른 아이템이 생성된다.
		{
			cltItem		clOutPutItem[MAX_BOX_OUTPUT];		// 최대 2개까지 한번에 지급이 가능하다.
			SI16		brareswitch = 0;
			bool		bCanNotice[MAX_BOX_OUTPUT]	= {0,};	// 대박 알림여부

			//====================================================================
			// 지급하고 싶은 아이템을 설정한다.
			//====================================================================

			//------------------------------------------------------------------------------------------------------
			if ( (pclClient->m_pclBoxItemMgr) && (TRUE == pclClient->m_pclBoxItemMgr->IsNormalBoxItem(clitem.siUnique)) )
			{
				if ( false == IsBlankInv(1, INVMODE_ALL) )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				CBoxItemUnit* pclGetBoxItemUnit = pclClient->m_pclBoxItemMgr->GetRandItemFromUnique( clitem.siUnique );
				if ( NULL == pclGetBoxItemUnit )
				{
					return FALSE;
				}

				SI16 siGetRareSwitch	= 0;
				SI32 siGetItemUnique	= pclGetBoxItemUnit->GetItemUnique();
				SI32 siGetItemNum		= pclGetBoxItemUnit->GetItemNum();

				SI32 siReference		= pclItemManager->FindItemRefFromUnique( siGetItemUnique );
				if ( 0 >= siReference )
				{
					return false;
				}

				SI32 siGetUseDate		= pclItemManager->pclItemInfo[siReference]->clItem.uiDateUseDay;

				// 지급시 날짜가 정해져 있다면 정해진 날짜로 변경
				if ( 0 < pclGetBoxItemUnit->GetItemUseDate() )
				{
					siGetUseDate = pclGetBoxItemUnit->GetItemUseDate();
				}

				if ( false == pclClient->pclItemManager->MakeRandItemUnique(siGetItemUnique, &clOutPutItem[0], 0, 0, &siGetRareSwitch, 0, 0, 0, 0, 0, siGetUseDate) )
				{
					return false;
				}

				clOutPutItem[0].SetItemNum( siGetItemNum );
				// 대박알림 여부
				bCanNotice[0]	= DATA_TO_BOOL( pclGetBoxItemUnit->GetCanNotice() );

			}
			// 호박상자류 처리
			else if ( (pclClient->m_pclBoxItemMgr) && (TRUE == pclClient->m_pclBoxItemMgr->IsAmberBoxItem(clitem.siUnique)) )
			{
				if ( false == IsBlankInv(1, INVMODE_ALL) )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 siInBoxItemList[AMBER_BOX_LIST_NUM] = { 0, };

				SI32 siListCount = pclClient->m_pclBoxItemMgr->GetInBoxItemList( clitem.siUnique, siInBoxItemList, AMBER_BOX_LIST_NUM );
				if ( 0 >= siListCount )
				{
					return false;
				}

				// 아이템 박스 리스트
				cltGameMsgResponse_Amber_Box_List clinfo( siInBoxItemList, siListCount, clitem.siUnique );
				cltMsg clMsg(GAMEMSG_RESPONSE_AMBER_BOX_LIST, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				// 클릭하여 렌덤하게 선택된 아이템 정보 요청.		
				sDBRequest_Amber_Box_Select_Item_List clMsg2( GetID(), pclCI->GetPersonID(), clitem.siUnique );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg2);

			}
			else if(clitem.siUnique == ITEMUNIQUE(13300))	// 제갈공명 옷상자
			{
				if(pclClient->pclItemManager->MakeRandItemUnique( ITEMUNIQUE(23695), &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( ITEMUNIQUE(23395), &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;
			}
			//-----------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13322) ) // 강아지 상자   Cushion Box
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = 0;

				switch(GetKind())
				{
				case KIND_HERO3 :
					rndItemUnique = ITEMUNIQUE(13617); //닥스훈트
					break;
				case KIND_HEROINE3 :
					rndItemUnique = ITEMUNIQUE(13619); //시츄
					break;
				case KIND_HERO4 :
					rndItemUnique = ITEMUNIQUE(13618); //토이푸들
					break;
				case KIND_HEROINE4 :
					rndItemUnique = ITEMUNIQUE(13620); //시베리안 허스키
					break;
				}

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;
				clDrawItem.SetItemNum( 1 );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}
			//-----------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13336) ) // 이불 상자.
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = 0;

				switch(GetKind())
				{
				case KIND_HERO3 :
					rndItemUnique = ITEMUNIQUE(13629);
					break;
				case KIND_HEROINE3 :
					rndItemUnique = ITEMUNIQUE(13628);
					break;
				case KIND_HERO4 :
					rndItemUnique = ITEMUNIQUE(13629); 
					break;
				case KIND_HEROINE4 :
					rndItemUnique = ITEMUNIQUE(13628); 
					break;
				}

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;
				clDrawItem.SetItemNum( 1 );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}
			else if( clitem.siUnique == ITEMUNIQUE(13350) ) // PCK :  잠자는 냐옹이 상자 (09.06.12)
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = 0;

				switch(GetKind())
				{
				case KIND_HERO3 :
					rndItemUnique = ITEMUNIQUE(13649);
					break;
				case KIND_HEROINE3 :
					rndItemUnique = ITEMUNIQUE(13650);
					break;
				case KIND_HERO4 :
					rndItemUnique = ITEMUNIQUE(13647);
					break;
				case KIND_HEROINE4 :
					rndItemUnique = ITEMUNIQUE(13648);
					break;
				}

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;
				clDrawItem.SetItemNum( 1 );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}
			// 복 주머니 - by LEEKH 2007.12.26
			else if( clitem.siUnique == ITEMUNIQUE(13329) )	
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				SI32 rndItemUnique = GetUniqueWithRandom(siItemUnique_8115_ItemUnique, sizeof(siItemUnique_8115_ItemUnique) / sizeof(SI32) );
				if(rndItemUnique <= 0)
					return false;

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				clDrawItem.SetItemNum( 1 );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				return false;
			}
			//------------------------------------------------------------------------------------------------------
			//KHY - 1104 -  갓차2 -라쿤 보물 상자.  - 라쿤 복주머니
			else if( clitem.siUnique == ITEMUNIQUE(13337) )	
			{
				if( IsBlankInv(1,INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}
				// 최종 선정 아이템.
				SI32 rndItemUnique = 0;

				// 보너스 아이템은 여기서 설정한다.
				SI32 bonusItemUnique = ITEMUNIQUE(12506); // 밤비.
				SI32 bonusItemRate = 5; // 보너스 아이템이 나올 확률.

				SI32 randBonus = rand()%1000;

				if(randBonus < bonusItemRate)
				{
					rndItemUnique = bonusItemUnique;
				}
				else
				{
					rndItemUnique = GetUniqueWithRandom(siItemUnique_13337_ItemUnique, sizeof(siItemUnique_13337_ItemUnique) / sizeof(SI32) );				
				}

				if(rndItemUnique <= 0)
					return false;

				SI16 brareswitch = 0;
				SI32 usedate = 0;
				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clDrawItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
					return false;

				clDrawItem.SetItemNum( 1 );
				//------------------------------------------
				// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
				//------------------------------------------
				SI16 invpos;
				cltItem cltoitem;
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clDrawItem, &invpos, &cltoitem, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				if ( clitem.siUnique != 0 )
				{
					sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos, &clDrawItem, pclItemManager->CanPile(clDrawItem.siUnique), clDrawItem.IsCreateSID(pclItemManager), false);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				// 보너스 아이템이 나왔다면.
				if(rndItemUnique == bonusItemUnique)
					((cltServer*)(pclClient))->SendRareItemMade(GetCharID(), &clDrawItem, RAREITEM_NOTICE_SWITCH_GACHA );

				return false;
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13301))	// 틴틴상자
			{
#ifdef _CHINA
				const SI32 siTreasureBoxItemList[]=
				{
					ITEMUNIQUE(24000),
						ITEMUNIQUE(24020),
						ITEMUNIQUE(5350),
						ITEMUNIQUE(5355),
						ITEMUNIQUE(5360),
						ITEMUNIQUE(5365),
						ITEMUNIQUE(24045),
				};
#else
				const SI32 siTreasureBoxItemList[]=
				{
					ITEMUNIQUE(24000),
						ITEMUNIQUE(7860),
						ITEMUNIQUE(24020),
						ITEMUNIQUE(2710),
						ITEMUNIQUE(2700),
						ITEMUNIQUE(5350),
						ITEMUNIQUE(5355),
						ITEMUNIQUE(5360),
						ITEMUNIQUE(5365),
						ITEMUNIQUE(24045),
						ITEMUNIQUE(3865),
				};
#endif
				const SI32 siTinTinUnique			= ITEMUNIQUE(18510);	// 이호식 망토 유니크
				const SI32 siArchdevilMakeUnique	= ITEMUNIQUE(13027);	// 대악마 날개 유니크

				SI32 siRand = rand() % 100 + 1;

				//KHY - 0829 - 대천사 대악마. 재료추가.
				if(pclClient->IsCountrySwitch(Switch_ArchangelArchdevil))
				{
					if ( siRand <= 5 ) 
					{
						SI16 brareswitch = 0;
						SI32 usedate = 30;

						if(pclClient->pclItemManager->MakeRandItemUnique( siTinTinUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
							return false;

						clOutPutItem[0].SetItemNum( 1 );
					}
					else if ( (siRand > 5) &&(siRand <= 10)) // 스패셜에서 5%로 수정되어 수치 변경. 20->10
					{
						SI16 brareswitch = 0;
						SI32 usedate = 7;

						if(pclClient->pclItemManager->MakeRandItemUnique( siArchdevilMakeUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
							return false;

						clOutPutItem[0].SetItemNum( 1 );
					}
					else
					{
						SI32 siRand2 = rand() % ( sizeof(siTreasureBoxItemList) / sizeof(SI32) );


						SI16 brareswitch = 0;
						if(pclClient->pclItemManager->MakeRandItemUnique( siTreasureBoxItemList[siRand2], &clOutPutItem[0], 0, 0, &brareswitch) == false)
							return false;

						SI32 siRand3 = rand() % 100 + 1;

						if ( siRand3 <= 30 )		{		clOutPutItem[0].SetItemNum(1);		}
						else if ( siRand3 <= 65 )	{		clOutPutItem[0].SetItemNum(2);		}
						else if ( siRand3 <= 90 )	{		clOutPutItem[0].SetItemNum(3);		}
						else if ( siRand3 <= 95 )	{		clOutPutItem[0].SetItemNum(4);		}
						else						{		clOutPutItem[0].SetItemNum(5);		}
					}
				}
				else
				{
					if ( siRand <= 5 ) 
					{
						SI16 brareswitch = 0;
						SI32 usedate = 30;

						if(pclClient->pclItemManager->MakeRandItemUnique( siTinTinUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)
							return false;

						clOutPutItem[0].SetItemNum( 1 );
					}
					else
					{
						SI32 siRand2 = rand() % ( sizeof(siTreasureBoxItemList) / sizeof(SI32) );


						SI16 brareswitch = 0;
						if(pclClient->pclItemManager->MakeRandItemUnique( siTreasureBoxItemList[siRand2], &clOutPutItem[0], 0, 0, &brareswitch) == false)
							return false;

						SI32 siRand3 = rand() % 100 + 1;

						if ( siRand3 <= 30 )			{		clOutPutItem[0].SetItemNum(1);			}
						else if ( siRand3 <= 65 )		{		clOutPutItem[0].SetItemNum(2);			}
						else if ( siRand3 <= 90 )		{		clOutPutItem[0].SetItemNum(3);			}
						else if ( siRand3 <= 95 )		{		clOutPutItem[0].SetItemNum(4);			}
						else							{		clOutPutItem[0].SetItemNum(5);			}
					}
				}
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13302))	// 키메라 상자
			{
				SI32 siItemUnique = 0;

				if		(pclClient->IsWhereServiceArea(ConstServiceArea_China))
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_China, sizeof(siItemUnique_13302_ItemUnique_China) / sizeof(SI32));
				}
				else if (pclClient->IsWhereServiceArea(ConstServiceArea_English))
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_English, sizeof(siItemUnique_13302_ItemUnique_English) / sizeof(SI32));
				}
				else if (pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_Japan, sizeof(siItemUnique_13302_ItemUnique_Japan) / sizeof(SI32));
				}
				else if (pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_Korea, sizeof(siItemUnique_13302_ItemUnique_Korea) / sizeof(SI32));
				}
				else if (pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina))
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_NHNChina, sizeof(siItemUnique_13302_ItemUnique_NHNChina) / sizeof(SI32));
				}
				else if (pclClient->IsWhereServiceArea(ConstServiceArea_EUROPE))
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_Europe, sizeof(siItemUnique_13302_ItemUnique_Europe) / sizeof(SI32));
				}
				else
				{
					siItemUnique = GetUniqueWithRandom(siItemUnique_13302_ItemUnique_Etc, sizeof(siItemUnique_13302_ItemUnique_Etc) / sizeof(SI32));
				}

				if(siItemUnique <= 0)			return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem[0], 0, 0, &brareswitch) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13303))	// 행운의상자
			{
				SI32 ItemUnique = GetUniqueWithRange( ItemUnique_13303_ItemNum, sizeof(ItemUnique_13303_ItemNum) / sizeof(cltUniqueRange), 100);

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, 0) == false)
					return false;

				clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13304))	// 책 상자
			{
				siRealItemCount = MakeRealItemUnqiues(	ItemUnique_13304_ItemUnique, sizeof(ItemUnique_13304_ItemUnique) / sizeof(SI32), 
					siRealItemUniques, MAX_ITEM_UNIQUE);

				SI32 siItemUnique = GetUniqueWithRandom( siRealItemUniques, siRealItemCount );
				if ( siItemUnique <= 0 )		return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
			}	

			else if(clitem.siUnique == ITEMUNIQUE(13344))	// 아바타 책상자
			{
				siRealItemCount = MakeRealItemUnqiues(	ItemUnique_13344_ItemUnique, sizeof(ItemUnique_13344_ItemUnique) / sizeof(SI32), 
					siRealItemUniques, MAX_ITEM_UNIQUE);

				SI32 siItemUnique = GetUniqueWithRandom( siRealItemUniques, siRealItemCount );
				if ( siItemUnique <= 0 )		return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
			}
			else if(clitem.siUnique == ITEMUNIQUE(13361))	// 제조 책상자
			{
				siRealItemCount = MakeRealItemUnqiues(	ItemUnique_13361_ItemUnique, sizeof(ItemUnique_13361_ItemUnique) / sizeof(SI32), 
					siRealItemUniques, MAX_ITEM_UNIQUE);

				SI32 siItemUnique = GetUniqueWithRandom( siRealItemUniques, siRealItemCount );
				if ( siItemUnique <= 0 )		return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
			}
			else if(clitem.siUnique == ITEMUNIQUE(13362))	// 기술 책상자
			{
				siRealItemCount = MakeRealItemUnqiues(	ItemUnique_13362_ItemUnique, sizeof(ItemUnique_13362_ItemUnique) / sizeof(SI32), 
					siRealItemUniques, MAX_ITEM_UNIQUE);

				SI32 siItemUnique = GetUniqueWithRandom( siRealItemUniques, siRealItemCount );
				if ( siItemUnique <= 0 )		return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
			}
			else if(clitem.siUnique == ITEMUNIQUE(13363))	// 생산 책상자
			{
				siRealItemCount = MakeRealItemUnqiues(	ItemUnique_13363_ItemUnique, sizeof(ItemUnique_13363_ItemUnique) / sizeof(SI32), 
					siRealItemUniques, MAX_ITEM_UNIQUE);

				SI32 siItemUnique = GetUniqueWithRandom( siRealItemUniques, siRealItemCount );
				if ( siItemUnique <= 0 )		return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
			}

			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13305))	// 냐옹이 방석
			{
				/*
				// 나올 아이템 목록
				const SI32 siBoxItemList[]=
				{
					ITEMUNIQUE(13608),
						ITEMUNIQUE(13609),
						ITEMUNIQUE(13610),
						ITEMUNIQUE(13611)						
				};

				SI32 rndItemIndex = rand() % ( sizeof(siBoxItemList) / sizeof(SI32) );
				SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				// 일본을 제외한 모든 국가는 자신의 캐릭터에 맞는 고양이가 나온다.
				// by LEEKH 2008.01.04 - 사양수정
				if( pclClient->IsWhereServiceArea(ConstServiceArea_Japan) == false )
				{
					switch(GetKind())
					{
					case KIND_HERO3 :
						rndItemUnique = ITEMUNIQUE(13610);
						break;
					case KIND_HEROINE3 :
						rndItemUnique = ITEMUNIQUE(13611);
						break;
					case KIND_HERO4 :
						rndItemUnique = ITEMUNIQUE(13608);
						break;
					case KIND_HEROINE4 :
						rndItemUnique = ITEMUNIQUE(13609);
						break;
					}
				}
				*/
					SI32 rndItemUnique = 0;

				switch(GetKind())
				{
				case KIND_HERO3 :
					rndItemUnique = ITEMUNIQUE(13610);
					break;
				case KIND_HEROINE3 :
					rndItemUnique = ITEMUNIQUE(13611);
					break;
				case KIND_HERO4 :
					rndItemUnique = ITEMUNIQUE(13608);
					break;
				case KIND_HEROINE4 :
					rndItemUnique = ITEMUNIQUE(13609);
					break;
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13306))	// 메이드복 상자
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23392);	// ホワイトブリム（男性用）- HAT
						rndItemUnique2 = ITEMUNIQUE(23692);	// エプロンドレス（男性用）- DRESS
					}
					else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23390);	// ホワイトブリム（女性用）- HAT
						rndItemUnique2 = ITEMUNIQUE(23690);	// エプロンドレス（女性用）- DRESS
					}
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23690),
							ITEMUNIQUE(23390),
							ITEMUNIQUE(23692),
							ITEMUNIQUE(23392),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13307) ||		// 강화된 근력반지상자 
				clitem.siUnique == ITEMUNIQUE(13308) ||		// 강화된 순발력반지상자
				clitem.siUnique == ITEMUNIQUE(13309) || 	// 강화된 생명력반지상자
				clitem.siUnique == ITEMUNIQUE(13310) || 	// 강화된 지력반지상자
				clitem.siUnique == ITEMUNIQUE(13311) || 	// 강화된 손재주반지상자
				clitem.siUnique == ITEMUNIQUE(13312) )		// 강화된 행운반지상자
			{

				SI32 itemUnique = 0;

				switch( clitem.siUnique )
				{
				case ITEMUNIQUE(13307): itemunique = ITEMUNIQUE(7530);		break;
				case ITEMUNIQUE(13308): itemunique = ITEMUNIQUE(7535);		break;
				case ITEMUNIQUE(13309): itemunique = ITEMUNIQUE(7540);		break;
				case ITEMUNIQUE(13310): itemunique = ITEMUNIQUE(7545);		break;
				case ITEMUNIQUE(13311): itemunique = ITEMUNIQUE(7550);		break;
				case ITEMUNIQUE(13312): itemunique = ITEMUNIQUE(7555);		break;
				}

				// 30일짜리 반지를 생성한다.
				if(pclClient->pclItemManager->MakeRandItemUnique( itemunique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,30) == false)
					return false;

				clOutPutItem[0].SetItemNum( 1 );
				SI32 rndVal = rand() % 10 + 6;

				//KHY - 0820 - 반지 능력치 버그 수정.
				switch( clitem.siUnique )
				{
				case ITEMUNIQUE(13307): clOutPutItem[0].clItemRing.uiStr = rndVal ;		break;
				case ITEMUNIQUE(13308): clOutPutItem[0].clItemRing.uiDex = rndVal ;		break;
				case ITEMUNIQUE(13309): clOutPutItem[0].clItemRing.uiVit = rndVal ;		break;
				case ITEMUNIQUE(13310): clOutPutItem[0].clItemRing.uiMag = rndVal ;		break;
				case ITEMUNIQUE(13311): clOutPutItem[0].clItemRing.uiHnd = rndVal ;		break;
				case ITEMUNIQUE(13312): clOutPutItem[0].clItemRing.uiLuk = rndVal ;		break;
				}
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13313))	// Greek Box
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23388);	// ホワイトブリム（男性用）- HAT
						rndItemUnique2 = ITEMUNIQUE(23688);	// エプロンドレス（男性用）- DRESS
					}
					else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23386);	// ホワイトブリム（女性用）- HAT
						rndItemUnique2 = ITEMUNIQUE(23686);	// エプロンドレス（女性用）- DRESS
					}
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23388),
							ITEMUNIQUE(23688),
							ITEMUNIQUE(23386),
							ITEMUNIQUE(23686),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13314))
			{
				const SI32 siTreasureBoxItemNumList[20] =
				{
					10, 20,			// 최소, 최대값
						10, 20,
						10, 20,
						10, 20,
						1, 5,
						1, 5,
						1, 5,
						1, 5,
						1, 5,
						5, 10
				};

				SI32 itemUnique = GetUniqueWithRandom(siItemUnique_13314_ItemUnique, sizeof(siItemUnique_13314_ItemUnique) / sizeof(SI32));
				if(itemUnique <= 0)		return false;

				SI32 itemCount = max( 1, sizeof(siItemUnique_13314_ItemUnique) / sizeof(SI32) );
				SI32 selectedItem = rand() % itemCount;

				SI32 minNum = siTreasureBoxItemNumList[selectedItem*2];
				SI32 maxNum = siTreasureBoxItemNumList[selectedItem*2+1];
				SI32 itemNum = minNum + max(1, rand()%(maxNum-minNum+1) );

				if(pclClient->pclItemManager->MakeRandItemUnique( itemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				// 아이템의 갯수
				clOutPutItem[0].SetItemNum( itemNum );
			}

			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13360))	// 선물 상자
			{
				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(siItemUnique_13360_ItemUnique, sizeof(siItemUnique_13360_ItemUnique) / sizeof(cltValuesRange), 1000);

				if ( pItemValue == NULL )		return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13315))	// 재료 상자
			{
				const SI32 siTreasureBoxItemNumList[20] =
				{
					10, 20,			// 최소, 최대값
						10, 20,
						10, 20,
						10, 20,
						10, 20,
						10, 20,
						10, 20,
						10, 20,
						10, 20,
						10, 20
				};

				SI32 itemUnique = GetUniqueWithRandom(siItemUnique_13315_ItemUnique, sizeof(siItemUnique_13315_ItemUnique) / sizeof(SI32));;
				if(itemUnique <= 0)				return false;

				SI32 itemCount = max( 1, sizeof(siItemUnique_13315_ItemUnique) / sizeof(SI32) );
				SI32 selectedItem = rand() % itemCount;

				SI32 minNum = siTreasureBoxItemNumList[selectedItem*2];
				SI32 maxNum = siTreasureBoxItemNumList[selectedItem*2+1];
				SI32 itemNum = minNum + max(1, rand()%(maxNum-minNum+1) );

				if(pclClient->pclItemManager->MakeRandItemUnique( itemUnique, &clOutPutItem[0], 0, 0, &brareswitch ) == false)
					return false;

				// 아이템의 갯수
				clOutPutItem[0].SetItemNum( itemNum );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13316))	// 신년 행운 상자
			{
				cltValuesRange* pItemValue = NULL;

				//if(pclClient->siServiceArea == ConstServiceArea_USA || pclClient->siServiceArea == ConstServiceArea_Korea)
				//{				
				//	pItemValue = GetValuesWithRange(ItemValue_13316_ItemNumUSA_KOR, sizeof(ItemValue_13316_ItemNumUSA_KOR) / sizeof(cltValuesRange), 1000);
				//}
				//else
				//{
				//	pItemValue = GetValuesWithRange(ItemValue_13316_ItemNum, sizeof(ItemValue_13316_ItemNum) / sizeof(cltValuesRange), 1000);
				//}

				// [춘기] 신년 행운 상자의 경우 남,녀에 따라 상자의 아이템 내용이 달라진다. (08.11.13)
				// [춘기] 수정 : 일본의 경우 신년행운상자의 내용이 다르다 (08.12.18)
				if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
				{
					// 남자의 경우				
					if(GetKind() == KIND_HERO1 ||  GetKind() == KIND_HERO2 || GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4)
					{
						pItemValue = GetValuesWithRange(ItemValue_13316WithManJapan_ItemNum, sizeof(ItemValue_13316WithManJapan_ItemNum) / sizeof(cltValuesRange), 1000);
					}
					// 여성의 경우
					else if(GetKind() == KIND_HEROINE1 ||  GetKind() == KIND_HEROINE2 || GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4)
					{
						pItemValue = GetValuesWithRange(ItemValue_13316WithWomanJapan_ItemNum, sizeof(ItemValue_13316WithWomanJapan_ItemNum) / sizeof(cltValuesRange), 1000);
					}
				}
				else
				{
					// 남자의 경우				
					if(GetKind() == KIND_HERO1 ||  GetKind() == KIND_HERO2 || GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4)
					{
						pItemValue = GetValuesWithRange(ItemValue_13316WithMan_ItemNum, sizeof(ItemValue_13316WithMan_ItemNum) / sizeof(cltValuesRange), 1000);
					}
					// 여성의 경우
					else if(GetKind() == KIND_HEROINE1 ||  GetKind() == KIND_HEROINE2 || GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4)
					{
						pItemValue = GetValuesWithRange(ItemValue_13316WithWoman_ItemNum, sizeof(ItemValue_13316WithWoman_ItemNum) / sizeof(cltValuesRange), 1000);
					}
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(pclClient->siServiceArea == ConstServiceArea_Taiwan)
				{
					if(15900 == ItemUnique)
					{
						ItemUnique = 15901;
					}
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
				clOutPutItem[0].CancellationSealingUp( pclserver->sTime );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13317))	// NinJa Spirit Box
			{
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13615), //닌자  - 남  - 원거리.
				//		ITEMUNIQUE(13616) //닌자  - 여  - 근거리.				
				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13338))	// Christmas Spirit BOX
			{
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13633), //크리스마스 근거리변신
				//		ITEMUNIQUE(13632)  //크리스마스 원거리변신

				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13318))	// Random Chocolat Box
			{
				cltValuesRange* pItemValue = NULL;

				// 수정 : 전국가 동일하게 초코상자에서 나오는 아이템 적용함 (08.12.23)

				pItemValue = GetValuesWithRange(ItemValue_13318_ItemNum, sizeof(ItemValue_13318_ItemNum) / sizeof(cltValuesRange), 1000);

				if ( pItemValue == NULL )		return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13319))	// Random Candy Box
			{
				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(ItemValue_13319_ItemNum, sizeof(ItemValue_13319_ItemNum) / sizeof(cltValuesRange), 1000);

				if ( pItemValue == NULL )					return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//[진성] 중국 프로모터 추천인 상자.
			else if(clitem.siUnique == ITEMUNIQUE(13349))
			{
				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(ItemValue_13349_ItemNum, sizeof(ItemValue_13349_ItemNum) / sizeof(cltValuesRange), 1000);

				if ( pItemValue == NULL )					return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13342))	// 군주상자
			{
				cltValuesRange* pItemValue = NULL;

				if ( pclClient->IsWhereServiceArea(ConstServiceArea_Japan) )
					pItemValue = GetValuesWithRange(ItemValue_13342_ItemNum_Japan, sizeof(ItemValue_13342_ItemNum_Japan) / sizeof(cltValuesRange), 1000);
				else
					pItemValue = GetValuesWithRange(ItemValue_13342_ItemNum, sizeof(ItemValue_13342_ItemNum) / sizeof(cltValuesRange), 1000);

				if ( pItemValue == NULL )					return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}

			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13320))	// Pirate's Chest
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					rndItemUnique1 = ITEMUNIQUE(23718);	// 해적의복 
					rndItemUnique2 = ITEMUNIQUE(23108);	// 해적모자.
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23718),
						ITEMUNIQUE(23108),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13321))	// 금빛상자
			{
				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(ItemValue_13321, sizeof(ItemValue_13321) / sizeof(cltValuesRange), 100);
				if(pItemValue == NULL)			return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(ItemUnique <= 0)				return false;

				if(ItemUnique == 2765)	// 개수 다시 설정
				{
					Count = rand()%5+1; // 1~5개로 갯수 설정
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13358) )	// 2010신년이벤트 - 신행운상자 - 2009.10.14
			{
				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(ItemValue_13358, sizeof(ItemValue_13358) / sizeof(cltValuesRange), 1000);
				if(pItemValue == NULL)			return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(ItemUnique <= 0)				return false;

				if(ItemUnique == 3770)	// 개수 다시 설정
				{
					Count = rand()%3+1; // 1~3개로 갯수 설정
				}

				if( ItemUnique == 15900 )
				{
					usedate = 30;
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			// 보물찾기-낡은 쇠뭉치
			else if(clitem.siUnique == ITEMUNIQUE(13347))
			{
				cltValuesRange* pItemValue = NULL;

				pItemValue = GetValuesWithRange(ItemValue_13347, sizeof(ItemValue_13347) / sizeof(cltValuesRange), 100);
				if(pItemValue == NULL)			return false;

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				if(ItemUnique <= 0)				return false;

				switch( ItemUnique ) 
				{
					case ITEMUNIQUE(5010):
					case ITEMUNIQUE(5020):
					case ITEMUNIQUE(5040):
					case ITEMUNIQUE(5050):
						{
							Count = rand()%2+2; 
						}
						break;
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13323) ) // 루돌프 옷상자
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23095);	
						rndItemUnique2 = ITEMUNIQUE(23705);	
					}
					else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23096);	
						rndItemUnique2 = ITEMUNIQUE(23710);	
					}
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23095),
							ITEMUNIQUE(23705),
							ITEMUNIQUE(23096),
							ITEMUNIQUE(23710),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				clOutPutItem[0].SetItemNum(1);
				clOutPutItem[1].SetItemNum(1);
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13324) )	// [영훈] 신년 복주머니(대만전용) 2008.01.22
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_8120_ItemNumTaiwan, sizeof(ItemValue_8120_ItemNumTaiwan) / sizeof(cltValuesRange), 1000);
				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13351) )	// 성웅 [중국] pc방 보상 아이템
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13351_ItemNum_ChinaPcBang, sizeof(ItemValue_13351_ItemNum_ChinaPcBang) / sizeof(cltValuesRange), 1000);
				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13326) )	// [강형] 대박상자(농경) -> [진성] 낚시이벤트. 낡은상자로 변경. 사용하지 않아서.. 2008-8-12
			{
				if( IsBlankInv(1, INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				cltValuesRange* pItemValue = NULL;
				//[진성] 일본은 예외 처리. => 2008-9-9
				if ( pclClient->IsWhereServiceArea(ConstServiceArea_Japan) )
					pItemValue = GetValuesWithRange(ItemValue_13326_ItemNum_Japan, sizeof(ItemValue_13326_ItemNum_Japan) / sizeof(cltValuesRange), 10000);
				else
					pItemValue = GetValuesWithRange(ItemValue_13326_ItemNum, sizeof(ItemValue_13326_ItemNum) / sizeof(cltValuesRange), 10000);




				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 =  0;

				if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
					Count = pItemValue->siPara2;
				else
					Count = (rand() % pItemValue->siPara2) + 1;


				SI32 ref		= pclItemManager->FindItemRefFromUnique(ItemUnique);
				if(ref <= 0)	return false;

				SI32 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13327) )	// [강형] 대박상자(채광)
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13327_ItemNum, sizeof(ItemValue_13327_ItemNum) / sizeof(cltValuesRange), 100);
				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;

				SI32 ref		= pclItemManager->FindItemRefFromUnique(ItemUnique);
				if(ref <= 0)	return false;

				SI32 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//----------------------------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13331) )	// [성웅] 2차복주머니 2008.06.30
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13331_ItemNum, sizeof(ItemValue_13331_ItemNum) / sizeof(cltValuesRange), 1000);
				cltValuesRange* pItemValueExtra = GetValuesWithRange(ItemValue_1333_ItemNum_Extra, sizeof(ItemValue_1333_ItemNum_Extra) / sizeof(cltValuesRange), 1000);
				if ( pItemValue == NULL || 
					pItemValueExtra == NULL)
				{
					return false;
				}
				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();
				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;
				clOutPutItem[0].SetItemNum( Count );

				if ( rand()%10 >= 2 )
				{
					SI16 brareswitch = 0;
					SI32 ItemUniqueExtra  = pItemValueExtra->siPara1;
					SI32 CountExtra		 = pItemValueExtra->siPara2;
					SI32 usedateExtra	 = pItemValueExtra->siPara3;

					if ( ItemUniqueExtra <= 0 ) return false;
					cltItem clDrawItem;					// 추첨해서 나온 결과아이템
					clDrawItem.Init();
					if(pclClient->pclItemManager->MakeRandItemUnique( ItemUniqueExtra, &clOutPutItem[1], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedateExtra) == false)
						return false;
					clOutPutItem[1].SetItemNum( CountExtra );

				}
			}
			//----------------------------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13359))		// [춘기] 3차 복주머니 (2009.11.26)
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13359_ItemNum, sizeof(ItemValue_13359_ItemNum) / sizeof(cltValuesRange), 1200);
				cltValuesRange* pItemValueExtra = GetValuesWithRange(ItemValue_13359_ItemNum_Extra, sizeof(ItemValue_13359_ItemNum_Extra) / sizeof(cltValuesRange), 10000);

				if ( pItemValue == NULL || pItemValueExtra == NULL)
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;
				SI32 usedate	 = pItemValue->siPara3;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)	return false;
				clOutPutItem[0].SetItemNum( Count );

				// 80%의 확률로 추가 아이템을 얻을 수 있다.
				if ( rand()%10 >= 2 )
				{
					SI16 brareswitch = 0;
					SI32 ItemUniqueExtra  = pItemValueExtra->siPara1;
					SI32 CountExtra		 = pItemValueExtra->siPara2;
					SI32 usedateExtra	 = pItemValueExtra->siPara3;

					if ( ItemUniqueExtra <= 0 ) return false;
					cltItem clDrawItem;					// 추첨해서 나온 결과아이템
					clDrawItem.Init();
					if(pclClient->pclItemManager->MakeRandItemUnique( ItemUniqueExtra, &clOutPutItem[1], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedateExtra) == false)	return false;
					clOutPutItem[1].SetItemNum( CountExtra );

				}
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13328) )	// [강형] 상어
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13328_ItemNum, sizeof(ItemValue_13328_ItemNum) / sizeof(cltValuesRange), 100);
				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;

				SI32 ref		= pclItemManager->FindItemRefFromUnique(ItemUnique);
				if(ref <= 0)	return false;

				SI32 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13330))	// 고양이 변신 상자.
			{
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13623), //고양이 -  원거리.
				//		ITEMUNIQUE(13624) // 고양이 -  근거리.				
				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------
			else if( clitem.siUnique == ITEMUNIQUE(13333) )	//KHY - 0909 - 선무당의 선물  - 라쿤 가죽 지갑
			{
				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13333_ItemNum, sizeof(ItemValue_13333_ItemNum) / sizeof(cltValuesRange), 1000);
				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;

				SI32 Count		 = 0;

				if((pItemValue->siPara2) == (pItemValue->siPara3))
				{
					Count = pItemValue->siPara2;
				}
				else
				{
					Count = pItemValue->siPara2 + (rand()%(pItemValue->siPara2+1) );
				}

				SI32 ref		= pclItemManager->FindItemRefFromUnique(ItemUnique);
				if(ref <= 0)	return false;

				SI32 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//==========================================================================================================================================
			//else if(clitem.siUnique == ITEMUNIQUE(13351))	// 중국용 PC방 지급 랜덤 상자
			//{
			//	cltValuesRange* pItemValue = NULL;

			//	pItemValue = GetValuesWithRange(ItemValue_13351_ItemNum, sizeof(ItemValue_13351_ItemNum) / sizeof(cltValuesRange), 1000);

			//	if ( pItemValue == NULL )		return false;

			//	SI16 brareswitch = 0;
			//	SI32 ItemUnique  = pItemValue->siPara1;
			//	SI32 Count		 = pItemValue->siPara2;
			//	SI32 usedate	 = pItemValue->siPara3;

			//	if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
			//		return false;

			//	clOutPutItem[0].SetItemNum( Count );
			//}
			//==========================================================================================================================================
			// [진성] 루미너리(USA) 1주년 기념 과금 상자  => 2008-9-25
			else if( ITEMUNIQUE(13335) == clitem.siUnique )	
			{
				if( IsBlankInv(1, INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}

				cltValuesRange* pItemValue = GetValuesWithRange(ItemValue_13335_ItemNum, sizeof(ItemValue_13335_ItemNum) / sizeof(cltValuesRange), 1000);
				if ( pItemValue == NULL )
				{
					return false;
				}

				SI16 brareswitch = 0;
				SI32 ItemUnique  = pItemValue->siPara1;
				SI32 Count		 = pItemValue->siPara2;

				SI32 ref		= pclItemManager->FindItemRefFromUnique(ItemUnique);
				if(ref <= 0)	return false;

				SI32 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;;

				cltItem clDrawItem;					// 추첨해서 나온 결과아이템
				clDrawItem.Init();

				if(pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clOutPutItem[0], 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == false)
					return false;

				clOutPutItem[0].SetItemNum( Count );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13332))	// 호박 의복 상자
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23112);	// 할로윈 모자
						rndItemUnique2 = ITEMUNIQUE(23729);	// 할로윈 의복
					}
					else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23113);	// 할로윈 모자	
						rndItemUnique2 = ITEMUNIQUE(23730);	// 할로윈 의복
					}
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23112),
							ITEMUNIQUE(23729),
							ITEMUNIQUE(23113),
							ITEMUNIQUE(23730),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

			}

			else if(clitem.siUnique == ITEMUNIQUE(13341))	// 폭주족 의복 상자
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23115);	// 폭주족 모자
						rndItemUnique2 = ITEMUNIQUE(23734);	// 폭주족 의복
					}
					else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23116);	// 폭주족 모자	
						rndItemUnique2 = ITEMUNIQUE(23735);	// 폭주족 의복
					}
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23115),
							ITEMUNIQUE(23734),
							ITEMUNIQUE(23116),
							ITEMUNIQUE(23735),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13343))	// 투사 스피릿 상자
			{
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13641),	// 검사스피릿(여) - 근거리
				//		ITEMUNIQUE(13642)	// 거너스피릿(남) - 원거리
				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------	
			else if(clitem.siUnique == ITEMUNIQUE(13352))	// 페어리 스피릿 상자
			{
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13652),	// 요정 스피릿(여) - 원거리
				//		ITEMUNIQUE(13653)	// 요정 스피릿(남) - 근거리
				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------	
			else if(clitem.siUnique == ITEMUNIQUE(13357 ))	// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
			{
				// 근거리와 원거리를 선택할수 있도록 한다.
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13656 ),	// 서스콰치
				//		ITEMUNIQUE(13657 )	// 눈의 여왕
				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			else if(clitem.siUnique == ITEMUNIQUE(13356))	// 할로윈 스피릿 박스
			{
				//const SI32 siBoxItemList[]=
				//{
				//	ITEMUNIQUE(13626),	// 할로윈 스피릿 - 근거리
				//	ITEMUNIQUE(13627)	// 할로윈 스피릿 - 원거리
				//};

				//SI32 rndItemIndex = rand() % 2;
				//SI32 rndItemUnique = siBoxItemList[rndItemIndex];

				//UI16 uTransFormChar = 0;
				//cltCharServer* pclchar =(cltCharServer*)(pclClient->pclCM->CR[GetID()]);

				//uTransFormChar = pclchar->GetCharKindFromItemUnique(rndItemUnique);

				//if(pclchar->clTransFormInfo.SearchTransFormChar(uTransFormChar) == true) // 이미 있다는 표시임으로 다른하나를 설정한다.
				//{
				//	if(rndItemIndex == 0 )				rndItemIndex = 1;
				//	else if(rndItemIndex == 1 )			rndItemIndex = 0;

				//	rndItemUnique = siBoxItemList[rndItemIndex];
				//}

				//if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
				//	return false;
				//clOutPutItem[0].SetItemNum( 1 );
			}
			//------------------------------------------------------------------------------------------------------
			else if(clitem.siUnique == ITEMUNIQUE(13346))	// 결혼 예복 상자
			{
				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if( pclClient->IsCountrySwitch(Switch_GachaAvatarCorrect) )
				{
					if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23117);	// 결혼 예식 머리
						rndItemUnique2 = ITEMUNIQUE(23696);	// 결혼 예식용 턱시도
					}
					else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
					{
						rndItemUnique1 = ITEMUNIQUE(23118);	// 결혼용 면사포
						rndItemUnique2 = ITEMUNIQUE(23698);	// 웨딩용 드레스
					}
				}
				else
				{
					const SI32 siBoxItemList[] =	// 나올 아이템 목록
					{
						ITEMUNIQUE(23117),
							ITEMUNIQUE(23696),
							ITEMUNIQUE(23118),
							ITEMUNIQUE(23698),
					};

					SI32 rndVal = rand();
					SI32 rndItemIndex = rndVal % ( sizeof(siBoxItemList) / sizeof(SI32) ) / 2;

					rndItemUnique1 = siBoxItemList[rndItemIndex*2];
					rndItemUnique2 = siBoxItemList[rndItemIndex*2 + 1];	
				}

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

			}
			//[진성] 악마 의복 상자.
			else if(clitem.siUnique == ITEMUNIQUE(13364))	
			{
				if( IsBlankInv(2, INVMODE_ALL) == false )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 200, 0, GetID());
					return false;
				}

				SI32 rndItemUnique1 = 0;
				SI32 rndItemUnique2 = 0;

				if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
				{
					rndItemUnique1 = ITEMUNIQUE(23741);	
					rndItemUnique2 = ITEMUNIQUE(23410);	
				}
				else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
				{
					rndItemUnique1 = ITEMUNIQUE(23742);	
					rndItemUnique2 = ITEMUNIQUE(23411);	
				}
				
				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

				if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique2, &clOutPutItem[1], 0, 0, &brareswitch,0,0,0,0,0,0) == false)
					return false;

			}
			else if ( clitem.siUnique == ITEMUNIQUE(13348) ) // 결혼식 예복 지급 상자
			{
				SI32 rndItemUnique1 = 0;

				if ( GetKind() == KIND_HERO3 || GetKind() == KIND_HERO4 )
				{
					rndItemUnique1 = ITEMUNIQUE(23736);	// 결혼 예식용 턱시도
				}
				else if ( GetKind() == KIND_HEROINE3 || GetKind() == KIND_HEROINE4 )
				{
					rndItemUnique1 = ITEMUNIQUE(23737);	// 웨딩용 드레스
				}

				if ( rndItemUnique1 )
				{
					if(pclClient->pclItemManager->MakeRandItemUnique( rndItemUnique1, &clOutPutItem[0], 0, 0, &brareswitch,0,0,0,0,0,1) == false)
					{
						return false;
					}

					cltServer* pclserver = (cltServer*)pclClient;
					if ( pclserver )
					{
						clOutPutItem[0].CancellationSealingUp( pclserver->sTime );
					}
				}
				else
				{
					return false;
				}
			}
			
			//------------------------------------------------------------------------------------------------------
			//====================================================================
			// 위에서 지급할 아이템을 설정한다.
			//====================================================================

			SI32 i = 0;
			SI32 siOutItemCount = 0;

			// 지급되는 아이템의 개수
			for (i=0; i<MAX_BOX_OUTPUT; i++)
			{
				if(clOutPutItem[i].siUnique <= 0)		continue;
				if(clOutPutItem[i].siItemNum <= 0)		continue;

				siOutItemCount++;
			}

			// 인벤토리 빈 공간 체크
			if( IsBlankInv(siOutItemCount,INVMODE_ALL) == false )
			{
				((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
				return false;
			}

			cltItem cltoitem[MAX_BOX_OUTPUT];
			SI16	invpos[MAX_BOX_OUTPUT];

			ZeroMemory(invpos, sizeof(invpos));

			for (i=0; i<MAX_BOX_OUTPUT; i++)
			{
				if(clOutPutItem[i].siUnique <= 0)		continue;
				if(clOutPutItem[i].siItemNum <= 0)		continue;

				// 아이템 지급
				SI32 rtnvalue;
				if(CanAddInv(INVMODE_ALL, &clOutPutItem[i], &invpos[i], &cltoitem[i], &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, GetID());
					return false;
				}
				else
				{
					if(invpos[i] >= PERSONITEM_INV0 && invpos[i] < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON)
					{
						SetInputItemReserve( invpos[i] , pclClient->CurrentClock );
					}
					else
					{
						return false;
					}
				}
			}

			if ( siOutItemCount == 1 )
			{
				sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos[0], &clOutPutItem[0], pclItemManager->CanPile(clOutPutItem[0].siUnique), clOutPutItem[0].IsCreateSID(pclItemManager), bCanNotice[0] );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
			else if ( siOutItemCount == 2)
			{
				sDBRequest_OpenTreasureBox clMsg( pclCI->GetPersonID(), itempos, &clitem, 0, NULL, invpos[0], &clOutPutItem[0], pclItemManager->CanPile(clOutPutItem[0].siUnique), clOutPutItem[0].IsCreateSID(pclItemManager), bCanNotice[0], invpos[1], &clOutPutItem[1], pclItemManager->CanPile(clOutPutItem[1].siUnique), clOutPutItem[1].IsCreateSID(pclItemManager), bCanNotice[1]  );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}

			//-------------------------------------------------------------------------------
			// sDBRequest_OpenTreasureBox 패킷으로 아이템을 사용하므로 return false
			return false;
			//-------------------------------------------------------------------------------
		}
		break;

	case ITEMTYPE_PREMIUM://프리미엄아이템.
		{
			if( clitem.siUnique == ITEMUNIQUE(13661))	// 달콤한 초콜릿
			{
				SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.				

				SI16 index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_VALENTINEDAY, false );

				if ( index > -1 )
				{
					stBuf addBuf( BUF_VALENTINEDAY, 0, usetime );
					AddBufThroughDB( index, &addBuf , false, 0, NULL, 0 );

					return false;
				}
			}

			// 모터 사이클 용
			if( clitem.siUnique == ITEMUNIQUE(13635) || //헤드라이트
				clitem.siUnique == ITEMUNIQUE(13636) || // 불꽃휠
				clitem.siUnique == ITEMUNIQUE(13637)) //LED안테나
			{
				cltHorse *pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if(pclHorse == NULL || pclHorse->siHorseUnique <= 0)	return false;
				if(pclHorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
				{
					if ( pclHorse->HaveSamePremiumParts( clitem.siUnique) ==  false )// 같은 파츠가 있는지 리턴 한다.
					{
						pclHorse->SetPremiumParts( &((cltServer*)pclClient)->pclTime->clDate,clitem.siUnique );
						// DB에 저장한다. 
						SetUpdateSwitch(UPDATE_HORSE, true, 0);
						UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_PREMIUM_PARTS);
						// 주인과 이웃에게 말 정보를 보낸다. 
						SendNeighbourMyHorseStatus(pclCI->clHorseInfo.siCurrentShowHorse);
					}					
				}					
			}
			//모터사이클 아닌거용 3개			
			else if (clitem.siUnique == ITEMUNIQUE(13638) ||//등자
				clitem.siUnique == ITEMUNIQUE(13639) || //고삐
				clitem.siUnique == ITEMUNIQUE(13640) )//고글
			{
				// 말이 있어야 아이템을 사용한다. - by LEEKH 2007.10.18
				cltHorse *pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				if(pclHorse == NULL || pclHorse->siHorseUnique <= 0)	return false;
				// 오토바이는 탈것 강화제 영향을 받지 않는다.
				if(pclHorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)	return false	;
				if ( pclHorse->HaveSamePremiumParts( clitem.siUnique) ==  false )// 같은 파츠가 있는지 리턴 한다.
				{
					pclHorse->SetPremiumParts( &((cltServer*)pclClient)->pclTime->clDate,clitem.siUnique );
					// DB에 저장한다. 
					SetUpdateSwitch(UPDATE_HORSE, true, 0);
					UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_PREMIUM_PARTS);
					// 주인과 이웃에게 말 정보를 보낸다. 
					SendNeighbourMyHorseStatus(pclCI->clHorseInfo.siCurrentShowHorse);
				}
			}
			// 수호정령 소환		- [지연] : 2010신년이벤트 - 팅키인형[이벤트] (13658) 추가
			else if ( ITEMUNIQUE(13646) == clitem.siUnique || ITEMUNIQUE(13651) == clitem.siUnique|| ITEMUNIQUE(13655) == clitem.siUnique|| ITEMUNIQUE(13654) == clitem.siUnique || ITEMUNIQUE(13659) == clitem.siUnique || ITEMUNIQUE(13658) == clitem.siUnique)
			{
				SI16 siSoulGuardKind	= 0;
				SI16 siLife				= 0;
				SI16 siFellowShip		= 0;

				SI32 siTempReturn		= 0;

				if ( false == CanUseSoulGuardItem(clitem.siUnique, &siSoulGuardKind, &siLife, &siFellowShip, &siTempReturn) )
				{
					return false;
				}

				TCHAR*	pszName			= GetTxtFromMgr( 30146 );	// 수호정령
				SI16	siStatus		= SOULGUARD_STATUS_IN;
				SI32	siLimitLife		= 0;
				if ( Const_Unlimit_SoulGuard_Life <= siLife )
				{
					// 제한 없는 날짜 설정되어 있다면 변수 최대값으로 밀어넣어준다
					siLimitLife = MAX_SI32;
				}
				else
				{
					// 받아온 날짜는 실제시간 1일 기준이므로 게임시간으로 변경하기 위해 (* 30)을 곱해준다
					siLimitLife = pclserver->pclTime->clDate.GetDateVary() + (siLife*30);
				}

				sDBRequest_SoulGuardCreate clMsg( pclCI->GetPersonID(), pszName, siStatus, siSoulGuardKind, siLimitLife, siFellowShip );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			}
			return false;
		}
		break;
	case ITEMTYPE_QUEST: // 퀘스트 전용 아이템
		{
			//if ( ITEMUNIQUE(8121) == clitem.siUnique )
			//{
			//	SI32 siMonsterKind	= pclClient->GetUniqueFromHash( TEXT("KIND_QUESTEMPERORMASTER") );
			//	if ( 0 >= siMonsterKind )
			//	{
			//		return false;
			//	}

			//	SI32 siGetMapIndex	= GetMapIndex();
			//	if ( (0 > siGetMapIndex) || (MAX_MAP_NUMBER <= siGetMapIndex) )
			//	{
			//		return false;
			//	}

			//	SI32 siSourceX		= GetX();
			//	SI32 siSourceY		= GetY();
			//	SI32 siDestX		= 0;
			//	SI32 siDestY		= 0;

			//	bool bIsCreated		= false;

			//	cltMapCommon* pclMapCommon = pclClient->pclMapManager->pclMapArray[siGetMapIndex];
			//	if ( NULL == pclMapCommon )
			//	{
			//		return false;
			//	}
			//	
			//	// 혹시 나올 위치를 찾지 못했다면 적당하게 위치를 몇번 더 돌면서 위치를 찾는다
			//	for ( SI32 siCount=0; siCount<10; siCount++ )
			//	{
			//		if ( false == pclMapCommon->FindEmptyAreaForCharLocate(siMonsterKind, siSourceX, siSourceY, 10, &siDestX, &siDestY, true) )
			//		{
			//			continue;
			//		}

			//		cltInnerStatusInfo clinnerstatus;
			//		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			//		SI32 siCreatedID = pclserver->SetNPCChar( siMonsterKind, VILLAGE_NONE, NULL, 0, 0, siDestX, siDestY, siGetMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
			//		if ( pclCM->IsValidID(siCreatedID) )
			//		{
			//			// 퀘스트 전용 몬스터로 신분 변경
			//			cltCharServer* pclMonster = (cltCharServer*)pclClient->pclCM->GetCharServer(siCreatedID);
			//			if ( NULL == pclMonster )
			//			{
			//				continue;
			//			}

			//			pclMonster->clIdentity.MakeQuest( IDENTITY_QUEST, pclCI->GetPersonID(), GetCharUnique() );
			//			pclMonster->dwTimerClock = pclserver->CurrentClock;
			//			
			//			bIsCreated = true;

			//			break;
			//		}							
			//	}

			//	if ( false == bIsCreated )
			//	{
			//		return false;
			//	}
			//}
		}
		break;

	default:
		MsgBox(TEXT("UseItem()"), TEXT("re974:%d"), itemtype);
		return false;
	}

	//---------------------------------------
	// 물품 사용하기 퀘스트 진행중이었는지 확인하여 조치한다. 
	//---------------------------------------
	// 물품을 사용하는 퀘스트가 진행중이었나 ?
	IncQuestPara(QUEST_CLEAR_COND_USEITEM, clitem.siUnique, clitem.siItemNum);

	// [영훈] 서버 퀘스트 이벤트 : 아이템 사용하기
	if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
	{
		CNServerEvent_Quest* pclSE_Quest = (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();
		if ( pclSE_Quest )
		{
			SI32 siUseItemNum = max(clitem.siItemNum, siQuestCount);

			pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_USEITEM, clitem.siUnique, siUseItemNum );
		}
	}

	return true;
}   

// 아이템을 인벤토리에 넣는다. 
BOOL cltCharServer::AddItemToInventory(cltItem* pclitem)
{
	if(pclitem == NULL)return FALSE;

	// 무게를 확인한다. 
	// 최대 무게를 초과하면 더이상 들 수 없다. 
	SI32 itemweight = pclitem->GetWeight(pclClient->pclItemManager, TRUE);
	if(pclCI->clCharItem.GetTotalWeight(pclClient->pclItemManager) + itemweight > clPB.GetMaxWeight())
	{
		return FALSE;
	}

	return pclCI->clCharItem.AddInv(pclClient->pclItemManager, pclitem);

}

//캐릭터에게 아이템을 준다. 
// frompersonid : 누구에게서 아이템을 빼서 주는가. 
bool cltCharServer::InputItem(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32 inputitemreason, SI32* prtnvalue)
{
	SI16 itemtopos;
	cltItem cltoitem;

	SI32 topersonid		= pclCI->GetPersonID();
	if(topersonid <= 0)return false;

	// 자리가 있는지 검사한다.
	bool bInvetoryFull = CanAddInv(INVMODE_ALL, pclitem, &itemtopos, &cltoitem, prtnvalue) ;
	bool bigitem = false ;

	// 겹칠수 없는 아이템이라면 개수를 1로 바꾼다. - by LEEKH 2007.11.23
	if( pclitem->CanPile(pclClient->pclItemManager) == false )
	{
		pclitem->SetItemNum(1);
	}

	// 기록은 무조건 해주고 보자!
	// 추가되는 아이템이 대박 아이템이면, 
	if( inputitemreason == INPUTITEMPERSON_OPTION_WORDASSEMBLE		// 단어조합으로 얻게 된 아이템이면, 
		|| inputitemreason == INPUTITEMPERSON_OPTION_CRAZYEVENT			// 조선의 반격 이벤트로 얻게 된 아이템이면
		)
	{
		cltGameMsgResponse_BigWarItem clinfo(GetName(), pclCI->GetPersonID(), 
			pclitem, 0, inputitemreason, 0);

		cltMsg clMsg(GAMEMSG_RESPONSE_BIGWARITEM, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

		// 대박 물품 개수 통계. 
		((cltServer*)pclClient)->pclStatisticsManager->clDailyStatistics.siBigItemNum ++;

		// 대박 아이템을 받으면 기록한다.  
		if(inputitemreason == INPUTITEMPERSON_OPTION_WAR)
		{
			// 대박 아이템이면 공지한다. 
			TCHAR* pText = GetTxtFromMgr(2808);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"), pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_FARM)
		{
			TCHAR* pText = GetTxtFromMgr(2809);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"), pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_FARMSUMMON)
		{
			TCHAR* pText = GetTxtFromMgr(2810);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"), pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_FISH)
		{
			TCHAR* pText = GetTxtFromMgr(2811);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"),  pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_FISHSUMMON)
		{
			TCHAR* pText = GetTxtFromMgr(2812);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"),  pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_FAMELEVEL)
		{
			TCHAR* pText = GetTxtFromMgr(2814);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"), pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);

		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_WORDASSEMBLE)
		{
			TCHAR* pText = GetTxtFromMgr(2815);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"), pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);

		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_SPEECIALQUEST)
		{
			TCHAR* pText = GetTxtFromMgr(2816);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"), pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_OSAKA)
		{
			TCHAR* pText = GetTxtFromMgr(5278);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"),  pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_KANGHWADO)
		{
			TCHAR* pText = GetTxtFromMgr(5281);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"),  pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_CRAZYEVENT)
		{
			TCHAR* pText = GetTxtFromMgr(5299);
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"),  pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_ROULETTEGAME)
		{
			TCHAR* pText = TEXT("BigItem(박대박)-Name:%s Item:%s Num:%d Success:%d");
			pclClient->pclLog->FilePrint(TEXT("Config\\BigItem.log"),  pText, 
				GetName(), pclitem->GetShortName(pclClient->pclItemManager), pclitem->GetItemNum(),(SI32)bInvetoryFull);
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_INSTANCEMAP)
		{
			pclClient->pclLog->FilePrint(TEXT("Config\\InstanceItem.log"), TEXT("User : [%s] Item : [%s] "),
				GetName(),pclitem->GetShortName(pclClient->pclItemManager));
		}
		else if(inputitemreason == INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE_SPECIAL)
		{
			pclClient->pclLog->FilePrint(TEXT("Config\\PVPLEAGUE_SPECIALPRIZE.log"), TEXT("User : [%s] Item : [%s] "),
				GetName(),pclitem->GetShortName(pclClient->pclItemManager));
		}

		bigitem = true ;
	}

	// 자리가 없으면 못 준다. 흥부박은 여기서 기록해준다!
	if(bInvetoryFull == false)
	{
		if ( bigitem ) // 대박 아이템이면 로그를 보낸다.
		{
			((cltServer*)pclClient)->SendLog_InventoryFull( pclCI->GetPersonID(),
				pclitem->siUnique,pclitem->GetItemNum(),inputitemreason);
		}

		return false;
	}

	// 이미 해당 아이템이 있는 것이라면, 
	SI64 atb = pclItemManager->GetItemAtb(pclitem->siUnique);
	if(cltoitem.siItemNum && (atb & ITEMATB_FIRSTWARITEM))
	{
		while(clItemUseReserveInfo.Push(pclitem->siUnique, pclitem->siItemNum, itemtopos) == false)
		{
			ItemUseReserveAction();
		}
		pclCI->clCharItem.clItem[itemtopos].Increase(pclitem->siItemNum);

		// 클라이언트로 정보를 보낸다. 
		cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(pclitem->siUnique);
		if(pclprice)
		{
			cltGameMsgResponse_PersonItemInfo clinfo(itemtopos, PERSONITEMINFO_OPTION_WARITEM, pclitem->siItemNum, 0, &pclCI->clCharItem.clItem[itemtopos], pclprice->siPrice);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}	



		if( INPUTITEMPERSON_OPTION_WAR == inputitemreason)
		{
			// 퀘스트 업데이트
			SI32 siQuestItemNum = pclitem->siItemNum;

			// [영훈] 퀘스트 카운트 적용
			if ( 0 < siQuestCount )
			{
				siQuestItemNum = siQuestCount;
			}

			IncQuestPara(QUEST_CLEAR_COND_HUNT_ITEM, pclitem->siUnique,	siQuestItemNum );

			if( siPartyIndex >0 )
			{
				CPartyObj* pParty = pclClient->pclPartyMgr->GetPartyObj( siPartyIndex );

				if(pParty)
				{
					pParty->NotifyGetIemToPartyMember(GetCharUnique(), pclitem );
				}
			}
		}

		// 파티에 가입된 유저라면 파티원들에게도 아이템 획득 메시지를 보내줌

		//파티획득막음
		/*
		if( siPartyIndex > 0 && INPUTITEMPERSON_OPTION_WAR == inputitemreason)
		{
			CPartyObj* pParty = ((cltServer*)pclClient)->pclPartyMgr->GetPartyObj(siPartyIndex);
			if(pParty != NULL)
				for(SI16 i = 0; i < MAX_PARTY_USER_NUM; i++ )
				{
					stPartyUser* pPartyUser = pParty->GetPartyUser(i);
					if(pPartyUser->bPartyStatus == false &&  pPartyUser->siCharUnique <=0) continue;

					SI32 partyUserID = pclCM->GetIDFromCharUnique(pPartyUser->siCharUnique);

					if( partyUserID == siID || ((cltServer*)pclClient)->pclCM->IsValidID(partyUserID) == false) continue;

					cltGameMsgResponse_Party_NotifyItemGet clinfo( GetCharUnique()  , pclitem );
					cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_NOTIFYITEMGET,sizeof(clinfo), (BYTE*)&clinfo);
					((cltServer*)pclClient)->pclCM->CR[partyUserID]->SendNetMsg((sPacketHeader*)&clMsg);
				}
		}
		*/

	}
	else
	{
		//---------------------------------------------
		// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		ItemUseReserveAction();

		//---------------------------------------------
		// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		BulletUseReserveAction();

		// 아이템이 추가되는 자리를 예약한다. 
		clSimpleVary.dwItemInputReserveClock[itemtopos] = pclClient->CurrentClock;

		// 사용자에게 아이템을 넣는 메시지를 사용한다. 
		sDBRequest_InputItemToPerson clMsg(GetID(), topersonid, itemtopos, inputitemreason, pclitem->CanPile(pclItemManager), pclitem, 0, pclitem->IsCreateSID(pclItemManager));
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}

	return true;
}

// 캐릭터에게 아이템을 주고 필요한 조치를 취한다. 
SI32 cltCharServer::GiveItem(SI32 itemunique, SI32 itemnum, SI32 giveitemreason,SI32 usedate, SI32 siSealMode/* =GIVEITEM_SEAL_MODE_NONE */, SI32 EnchantType, SI32 EnchantLevel, SI32 SlotNum)
{
	if ( itemunique		< 1 || itemnum < 1 )
		return 0;

	// 해당 아이템을 랜덤하게 만든다. 
	cltItem clItem;
	SI16 brareswitch = 0;

	// [춘기] /seq 명령어를 사용해서 만들때 경우 추가
	if(giveitemreason == INPUTITEMPERSON_OPTION_SEQGMCOMMAND)
	{
		// RareRate 설정
		SI32 RareRate[4];
		for ( SI32 i = 0 ; i < 4 ; i++ )
		{
			RareRate[i] = GetRareItemRate(i);
		}

		if(pclClient->pclItemManager->MakeRandItemUnique( itemunique, &clItem, RareRate[0], 0, &brareswitch,RareRate[0],RareRate[1],RareRate[2],RareRate[3], 0, usedate, EnchantType, EnchantLevel, SlotNum, true) == false)		return false;
	}
	else
	{
		if(pclClient->pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)		return false;
	}

	// 개수를 조정한다. 
	// 중첩 가능한 아이템은 원하는 개수만큼 만들 수 있다. 
	if(clItem.CanPile(pclClient->pclItemManager) == true)
	{
		clItem.SetItemNum(itemnum);
	}
	// 중첩 불가 아이템은 1개만 만들 수 있다. 
	else
	{
		clItem.SetItemNum(1);
	}

	// 사용기간이 있는 것중 siSealMode따라 밀봉/해제 여부를 설정한다
	if ( (0 < clItem.uiDateUseDay) && (0 < siSealMode) )
	{
		if ( GIVEITEM_SEAL_MODE_SEAL == siSealMode )
		{
			clItem.uiDateYear	= 0;
			clItem.uiDateMonth	= 0;
			clItem.uiDateDay	= 0;
		}
		else if ( GIVEITEM_SEAL_MODE_CANCEL == siSealMode )
		{
			cltServer* pclserver = (cltServer*)pclClient;
			if ( pclserver )
			{
				clItem.CancellationSealingUp( pclserver->sTime );
			}
		}
	}

	// 해당아이템을 인벤에 넣을 수 있는지 확인한다. 
	SI16 itempos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(CanAddInv(INVMODE_ALL, &clItem, &itempos, &cltoitem, &rtnvalue) == false)
	{
		return GIVEITEM_RESULT_NOINVEN;
	}

	//[진성] 파티홀. 파티장에서 지급하는 아이템일 경우 세팅한다. => 2008. 3. 12
	if( INPUTITEMPERSON_OPTION_PARTYHALL_ITEM == giveitemreason )
	{
		cltItem temp;
		temp.Init();
		temp.siItemNum  = clItem.siItemNum;
		temp.siUnique	= clItem.siUnique;
		temp.clItemCommon.clCommonInfo.uiGetItemReason = cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM;

		if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
		{
			temp.clItemCommon.clCommonInfo.uiDurability		= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
			temp.clItemCommon.clCommonInfo.uiMaxDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
		}

		clItem.Init();
		clItem.Set(&temp);	
	}
	else
	{
		clItem.clItemCommon.clCommonInfo.uiGetItemReason = cltItemCommonInfo::REASON_NO;
	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	BulletUseReserveAction();

	// 아이템이 추가되는 자리를 예약한다. 
	clSimpleVary.dwItemInputReserveClock[itempos] = pclClient->CurrentClock;

	// 넣을 수 있다면 DB매니저에게 아이템을 특정 위치에 넣도록 요청 한다.
	sDBRequest_InputItemToPerson clMsg(GetID(), pclCI->GetPersonID(), itempos, giveitemreason, clItem.CanPile(pclClient->pclItemManager), &clItem, 0, clItem.IsCreateSID(pclClient->pclItemManager));
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	return GIVEITEM_RESULT_SUCCESS;
}

bool cltCharServer::TakeOutItem(SI32 itemunique, SI32 itemnum, SI32 takeoutitemreason)
{
	cltPItemInfo* pcliteminfo = NULL;
	cltCharServer* pclchar = (cltCharServer*)this;
	cltItem clitem;

	pcliteminfo = &pclCI->clCharItem;

	int	i;
	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(pcliteminfo->clItem[i].siUnique == itemunique)
		{
			clitem.Set(&pcliteminfo->clItem[i]);
			cltItem *pclFrom = &pclchar->pclCI->clCharItem.clItem[ i ];
			SI32 InvPos = pcliteminfo->GetItemPos( clitem.siUnique );

			if(itemnum <= clitem.siItemNum)
			{
				clitem.siItemNum = itemnum;

				//cyj 예약아이템에 위치 정보 추가
				while( pclchar->clItemUseReserveInfo.Push( clitem.siUnique, -clitem.siItemNum, InvPos ) == false )
				{
					pclchar->ItemUseReserveAction();
				}

				// 클라이언트로 업데이트 된 정보를 보낸다. 
				if( pclFrom->siItemNum <= clitem.siItemNum )
				{
					pclchar->ItemUseReserveAction();
				}

				// pclchar->ItemUseReserveAction(); 밑에 있어야 한다. 
				// 그렇지 않으면 인벤에서 물품의 위치를 찾을 수 없다.!
				pclFrom->Decrease(clitem.siItemNum);

				// 클라이언트로 사용후 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo( i, takeoutitemreason, 0, 0, pclFrom, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				itemnum = 0;
			}
			else
			{
				//cyj 예약아이템에 위치 정보 추가
				while( pclchar->clItemUseReserveInfo.Push( clitem.siUnique, -clitem.siItemNum, InvPos ) == false )
				{
					pclchar->ItemUseReserveAction();
				}

				// 클라이언트로 업데이트 된 정보를 보낸다. 
				if( pclFrom->siItemNum <= clitem.siItemNum )
				{
					pclchar->ItemUseReserveAction();
				}

				// pclchar->ItemUseReserveAction(); 밑에 있어야 한다. 
				// 그렇지 않으면 인벤에서 물품의 위치를 찾을 수 없다.!
				pclFrom->Decrease(clitem.siItemNum);

				// 클라이언트로 사용후 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo( i, takeoutitemreason, 0, 0, pclFrom, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				itemnum -= clitem.siItemNum;
			}
		}
	}
	return true;
}


bool cltCharServer::CanEnchantItem(cltGameMsgRequest_MakeItemInv* pclinfo, SI32 *returnval, SI32 *preturnpara1)
{
	SI32 makeitemunique = pclinfo->siItemUnique;
	SI32 makeitemnum	= pclinfo->siItemNum;


	cltPItemInfo* pclmaterialinv	= NULL;

	// 의뢰자가 없는 경우 제조자의 인벤을 원료인벤으로 삼는다.
	if(pclmaterialinv == NULL)
	{
		pclmaterialinv = &pclCI->clCharItem;

	}

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)return false;

	//----------------------------------------------------------------------
	// 감옥에선 만들수 없다..
	//-----------------------------------------------------------------------

	// 감옥에서는 아이템 제작 불가 불가. 
	if( clInnerStatus.IsStatus(INNERSTATUS_PRISON))
	{			
		*returnval = SRVAL_CAN_NOT_ENCHANTITEM_IN_PRISON;
		return false;
	}

	if( true == bPersonalShopSwitch ||
		true == bPremiumPersonalShop )
	{
		SI32 j = 0;
		if( clShopData.m_siShopMode == PSHOP_MODE_SELL )
		{
			for( j = 0 ; j < MAX_SHOP_ITEM_NUMBER; ++j )
			{
				if( clShopData.m_clItem[j].siUnique == makeitemunique  )
				{
					*returnval		= SRVAL_FAIL_PERSONALSHOP_USEITEM;
					return false;
				}
			}
		}
	}

	//------------------------------------------------------------------------
	// 인벤의 아이템을 근거로 만들고자 하는 아이템을 만들 수 있는지 확인한다.
	//------------------------------------------------------------------------
	SI32 producenum = 0;
	SI32 canmakertn	= 0;
	SI32 canmakertnpara1 = 0;
	/*
	if(pclClient->pclItemManager->pclItemInfo[ref]->CanMake(MAX_INVITEM_PER_PERSON, 
		&pclmaterialinv->clItem[PERSONITEM_INV0], 
		&pclCI->clCharItem.clItem[PERSONITEM_INV0], 
		&producenum, &canmakertn, &canmakertnpara1,
		false) == false)
	{
		if(canmakertn == CANMAKE_ERROR_NOMATERIAL)
		{
			*returnval		= SRVAL_INSUFFICIENTMATERIAL;		// 원료 부족. 
			*preturnpara1	= canmakertnpara1;
		}
		else 
		{
			*returnval	= SRVAL_INSUFFICIENTTOOL;		// 도구 부족. 
			*preturnpara1	= canmakertnpara1;
		}
		return false;
	}

	*/


		//-------------------------------------------
		// 생산에 필요한 재료와 그 개수를 정리한다. 
		//-------------------------------------------
		SI32 makeunit = 1;
	if(makeunit <= 0)return false;


	//----------------------------------------------
	// 인벤에 아이템을 넣을 공간이 있는지 확인한다. 
	//----------------------------------------------

	// 해당 인벤에 빈자리가 있는지 확인한다. 
	cltItem clOutputItem;
	clOutputItem.Set(&pclClient->pclItemManager->pclItemInfo[ref]->clItem);
	clOutputItem.SetItemNum( makeitemnum );

	//SI16 sitopos;
	//cltItem cltoitem;
	//SI32 rtnvalue;
	//if(pclmaterialinv->CanAddInv(pclClient->pclItemManager, INVMODE_ALL, &clOutputItem, NULL, &sitopos,&cltoitem, &rtnvalue) ==  false)
	//{
	//	*returnval	= SRVAL_NOEMPTYAREA;		// 빈 자리가 없어서 실패. 
	//	return false;
	//}

	return true;

}

// Item 을 만들수있는지 검사한다.
bool cltCharServer::CanMakeItem(cltGameMsgRequest_MakeItemInv* pclinfo, SI32 *returnval, SI32 *preturnpara1)
{	

	SI32 makeitemunique = pclinfo->siItemUnique;
	SI32 makeitemnum	= pclinfo->siItemNum;

	cltPItemInfo* pclmaterialinv	= NULL;
	BOOL	bTutorialQuest = FALSE;
	BOOL	bUseQuestItem = FALSE;

	cltMaterialInfo clMaterialInfo[MAX_MATERIAL_NUMBER];


	// 제조 의뢰자가 별도로 있는 경우, 무효하면 실패. 
	if(pclinfo->siMaterialCharUnique)
	{
		if(pclCM->IsAlive(pclinfo->siMaterialCharUnique) == false)return false;

		// 의뢰자 인벤의 포인터를 구한다. 
		pclmaterialinv = &pclCM->CR[ pclinfo->siMaterialCharUnique ]->pclCI->clCharItem;
	}

	// 의뢰자가 없는 경우 제조자의 인벤을 원료인벤으로 삼는다.
	if(pclmaterialinv == NULL)
	{
		pclmaterialinv = &pclCI->clCharItem;
	}

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)return false;


	//----------------------------------------------------------------------
	// 감옥에선 만들수 없다..
	//-----------------------------------------------------------------------
	// 감옥에서는 아이템 제작 불가 불가. 
	if( clInnerStatus.IsStatus(INNERSTATUS_PRISON))
	{			
		*returnval = SRVAL_CAN_NOT_MAKEITEM_IN_PRISON;
		return false;
	}
	//----------------------------------------------------------------------
	// 튜토리얼 제조 퀘스트 중인지 체크한다.
	//-----------------------------------------------------------------------

	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_TUTORIALMAKEITEM;

	if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
		&& 	pclinfo->siMaterialCharUnique == 0	) // 제조의뢰 이면 안된다.
	{
		if( para1 == makeitemunique 
			&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == para1)
			&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 < para2)
			)
		{
			bTutorialQuest = TRUE;

			// 퀘스트 전용 아이템을 사용하는지 얻어옴
			if(pclClient->pclQuestManager->GetTutorialMakeQuestItem(questunique,makeitemunique,clMaterialInfo) == true)
			{
				bUseQuestItem = TRUE;
			}	
			else
			{
				bUseQuestItem = FALSE;
			}
		}
	}

	questclearcond =  QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM;
	if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
		&& 	pclinfo->siMaterialCharUnique == 0	) // 제조의뢰 이면 안된다.
	{
		if( para1 == pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel 
			&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == para1)
			&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 < para2)
			)
		{
			bTutorialQuest = TRUE;

			// 퀘스트 전용 아이템을 사용하는지 얻어옴
			if(pclClient->pclQuestManager->GetTutorialMakeQuestItem(questunique,makeitemunique,clMaterialInfo) == true)
			{
				bUseQuestItem = TRUE;
			}	
			else
			{
				bUseQuestItem = FALSE;
			}
		}
	}

	//----------------------------------------------------------------------
	// 아이템을 만들 수 있는 기술이 준비되어 있는지 확인한다.
	// 튜토리얼 퀘스트 중이거나 NPC를 통한 제조일경우 예외!
	//-----------------------------------------------------------------------
	if(	bTutorialQuest == FALSE && pclinfo->bNPCMake == false )
	{		
		if(pclClient->pclItemManager->pclItemInfo[ref]->CanMake_Skill(&pclCI->clCharSkill, &pclCI->clCharItem, &clPB.clTotalBA) == false)
		{
			*returnval	= SRVAL_SKILLNOTREADYFORMAKE;
			return false;
		}
	}
	else
	{
		// 튜토리얼 퀘스트 중이라면 스킬레벨을 따지지 않는다.
	}

	//----------------------------------------------------------------------
	// 안티 포션은 해당 제조 퀘스트 시에만 제조 가능하다
	//----------------------------------------------------------------------
	SI16 siAntiPotion = ITEMUNIQUE(2796);

	if ( siAntiPotion == makeitemunique )
	{
		questclearcond = QUEST_CLEAR_COND_MAKEITEM;

		if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, siAntiPotion) )
		{
			*returnval = SRVAL_SKILLNOTREADYFORMAKE;
			return false;
		}
	}

	//----------------------------------------------------------------------------------------
	// PCK : 잃어버린 퀘스트에서 제작하는 아이템은 해당 퀘스트 시에만 제조 가능하다.(09.05.08)
	//----------------------------------------------------------------------------------------

	if ( makeitemunique  == ITEMUNIQUE(8115) || makeitemunique  == ITEMUNIQUE(8116) 
		|| makeitemunique  == ITEMUNIQUE(8117) || makeitemunique  == ITEMUNIQUE(8118))
	{
		questclearcond = QUEST_CLEAR_COND_MAKEITEM;

		if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, makeitemunique) )
		{
			*returnval = SRVAL_SKILLNOTREADYFORMAKE;
			return false;
		}
	}

	//------------------------------------------------------------------------
	// 인벤의 아이템을 근거로 만들고자 하는 아이템을 만들 수 있는지 확인한다.
	//------------------------------------------------------------------------


	SI32 producenum = 0;
	SI32 canmakertn	= 0;
	SI32 canmakertnpara1 = 0; 

	if( pclinfo->siMaterialCharUnique || pclinfo->bNPCMake ) // 재조 의뢰일 경우 기존 방식 채용
	{
		if(pclClient->pclItemManager->pclItemInfo[ref]->CanMake(MAX_INVITEM_PER_PERSON, 
			&pclmaterialinv->clItem[PERSONITEM_INV0], 
			&pclCI->clCharItem.clItem[PERSONITEM_INV0], 
			&producenum, &canmakertn, &canmakertnpara1,
			pclinfo->bNPCMake, bTutorialQuest,bUseQuestItem,clMaterialInfo) == false)
		{
			if(canmakertn == CANMAKE_ERROR_NOMATERIAL)
			{
				*returnval		= SRVAL_INSUFFICIENTMATERIAL;		// 원료 부족. 
				*preturnpara1	= canmakertnpara1;
			}
			else if(canmakertn == CANMAKE_ERROR_NOTOOL) 
			{
				*returnval	= SRVAL_INSUFFICIENTTOOL;		// 도구 부족. 
				*preturnpara1	= canmakertnpara1;
			}

			return false;
		}
		//KHY - 1211 - 과금상품 - 만능 도구
		else if(canmakertn == CANMAKE_USE_ALLPURPOSETOOL) //True 이고 - 만능도구 상자 사용.  
		{
			*returnval	= SRVAL_RETURN_USE_ALLPURPOSETOOL;		// 만능도구 상자 사용.
		}
	}
	else // 자신이 직접 만드는 경우 인벤토리 인덱스까지 넘겨주며 체크
	{
		bool check = false;
		if( true == bPersonalShopSwitch ||
			true == bPremiumPersonalShop )
		{
			if( PSHOP_MODE_SELL == clShopData.m_siShopMode )
			{
				check = true;
			}
		}

		if(pclClient->pclItemManager->pclItemInfo[ref]->CanMakeForServer(MAX_INVITEM_PER_PERSON, 
			&pclmaterialinv->clItem[PERSONITEM_INV0], 
			&pclCI->clCharItem.clItem[PERSONITEM_INV0], 
			&producenum, &canmakertn, &canmakertnpara1,
			false, &siInvMakeItemIndex[0] , &clShopData.m_clItem[0], check, bTutorialQuest,bUseQuestItem,clMaterialInfo) == false)
		{
			if(canmakertn == CANMAKE_ERROR_NOMATERIAL)
			{
				*returnval		= SRVAL_INSUFFICIENTMATERIAL;		// 원료 부족. 
				*preturnpara1	= canmakertnpara1;
			}
			else if( CANMAKE_ERROR_USE_PERSONSHOP == canmakertn )
			{
				*returnval	= SRVAL_FAIL_PERSONALSHOP_USEITEM;		// 판매중인 개인상점 아이템은 사용할 수 없다. 
				*preturnpara1	= canmakertnpara1;
			}
			else
			{
				*returnval	= SRVAL_INSUFFICIENTTOOL;		// 도구 부족. 
				*preturnpara1	= canmakertnpara1;
			}

			return false;
		}
		//KHY - 1211 - 과금상품 - 만능 도구
		else if(canmakertn == CANMAKE_USE_ALLPURPOSETOOL) //True 이고 - 만능도구 상자 사용.  
		{
			*returnval	= SRVAL_RETURN_USE_ALLPURPOSETOOL;		// 만능도구 상자 사용.
		}	
	}

	//-------------------------------------------
	// 생산 가능한 수가 생산 요구수보다 작으면 실패.
	//-------------------------------------------
	if(producenum < makeitemnum)
	{
		*returnval	= SRVAL_MATMATCHNUM;		// 수량 불일치. 
		return false;
	}

	//-------------------------------------------
	// 생산에 필요한 재료와 그 개수를 정리한다. 
	//-------------------------------------------
	SI32 makeunit = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit;
	if(makeunit <= 0)return false;

	//-------------------------------------------
	//cyj 생산될 갯수를 예상하여 MAX_ITEM_PILE_NUMBER를 넘어가면 실패
	//-------------------------------------------
	SI32 needskillunique	= pclClient->pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
	SI32 needskilllevel		= pclClient->pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;


	SI32 tempmakeitemnum = 0;

	if( bTutorialQuest == FALSE )
	{
		tempmakeitemnum = GetMakeItemNum(needskillunique, needskilllevel, makeitemunique, makeitemnum);
	}
	else
	{
		// 튜토리얼 퀘스트 중이라면 스킬레벨로 아이템 생성갯수를 미리 짐작치 않는다. 스킬레벨의 영향을 받지 않음으로.
		tempmakeitemnum = makeitemnum;
	}


	if(tempmakeitemnum > MAX_ITEM_PILE_NUMBER)
	{
		*returnval	= SRVAL_TOOMANYITEMMAKE;
		*preturnpara1 = tempmakeitemnum;

		return false;

	}

	//----------------------------------------------
	// 인벤에 아이템을 넣을 공간이 있는지 확인한다. 
	//----------------------------------------------

	// 해당 인벤에 빈자리가 있는지 확인한다. 
	cltItem clOutputItem;
	clOutputItem.Set(&pclClient->pclItemManager->pclItemInfo[ref]->clItem);
	clOutputItem.SetItemNum( makeitemnum );

	SI16 sitopos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(pclmaterialinv->CanAddInv(pclClient->pclItemManager, INVMODE_ALL, &clOutputItem, NULL, &sitopos,&cltoitem, &rtnvalue) ==  false)
	{
		*returnval	= SRVAL_NOEMPTYAREA;		// 빈 자리가 없어서 실패. 
		return false;
	}

	//[지연] - 2010신년이벤트 : 이벤트 기간중엔 쿠키조각을 위한 인벤토리 공간도 체크)
	if( pclClient->IsCountrySwitch( Switch_NewYearEvent_2010) )
	{
		//if ( false == IsBlankInv( 2, INVMODE_ALL ) )
		//{
		//	*returnval = SRVAL_NOEMPTYAREA;  // 빈 자리가 없어서 실패. 
		//	return false;
		//}

		SI32 refFortuneCookiePiece = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
		if(ref <= 0)
		{
			*returnval	= SRVAL_NOEMPTYAREA;		// 빈 자리가 없어서 실패. 
			return false;
		}

		cltItem clFortuneCookiePieceItem;
		clFortuneCookiePieceItem.Set( &pclClient->pclItemManager->pclItemInfo[refFortuneCookiePiece]->clItem );
		clFortuneCookiePieceItem.SetItemNum( 8 );	// 가능한 최대치 만큼의 여유공간(8칸)을 체크

		if(pclmaterialinv->CanAddInv(pclClient->pclItemManager, INVMODE_ALL, &clFortuneCookiePieceItem, NULL, &sitopos, &cltoitem, &rtnvalue) ==  false)
		{
			*returnval	= SRVAL_NOEMPTYAREA;		// 빈 자리가 없어서 실패. 
			return false;
		}
	}

	return true;
}


// 주문을 근거로 물건을 만든다. 
bool cltCharServer::MakeItem(cltGameMsgRequest_MakeItemInv* pclinfo)
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 i, j;
	SI16 para1	= 0;		// 반드시 초기화되어야 한다. 
	SI16 para2	= 0;
	GMONEY sifee	= 0;

	SI32 personid = pclCI->GetPersonID();
	if(personid <= 0)return false;

	// 제작 의뢰자가 설정되어 있으나 유효하지 않으면 실패. 
	cltPItemInfo* pclmaterialinv = NULL;
	cltCharServer* pclMaterialChar	= NULL;

	BOOL bTutorialQuest = FALSE;
	SI32 materialpersonid = personid;
	SI32 materialcharunique	= GetCharUnique();
	BOOL bForce_Manufacture_Success = FALSE;		// 제조를 성공으로 만들어준다. - 성공기원석, 집중총명탕

	// 집중 총명탕 효과가 있다면 제조100%
	cltBufInfo* pclMyBuf = &pclCI->clBI.clBufInfo;
	if(pclMyBuf->IsHaveBuf(BUF_SUCCESS_MANUFACTURE))	bForce_Manufacture_Success = TRUE;

	// 제작을 요청한 의뢰자가 있다면...
	if(pclinfo->siMaterialCharUnique)
	{		
		if(pclCM->IsAlive(pclinfo->siMaterialCharUnique) == false)
		{
			// 의뢰자가 죽었다면... 메시지를 전송한다.
			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}

		// 집중 총명탕 효과가 있다면 제조100%
		cltBufInfo* pclMaterialBuf = &pclClient->pclCM->CR[pclinfo->siMaterialCharUnique]->pclCI->clBI.clBufInfo;
		if(pclMaterialBuf->IsHaveBuf(BUF_SUCCESS_MANUFACTURE))	bForce_Manufacture_Success = TRUE;

		// 의뢰자에게 예약되어 있는 아이템 사용이 있다면 처리한다. 
		((cltCharServer*)(pclCM->CR[ pclinfo->siMaterialCharUnique ]))->ItemUseReserveAction();

		//---------------------------------------------
		// 의뢰자에게 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		((cltCharServer*)(pclCM->CR[ pclinfo->siMaterialCharUnique ]))->BulletUseReserveAction();


		materialcharunique	= pclinfo->siMaterialCharUnique;
		materialpersonid	= pclCM->CR[ pclinfo->siMaterialCharUnique ]->pclCI->GetPersonID();
		pclmaterialinv = &pclCM->CR[ pclinfo->siMaterialCharUnique ]->pclCI->clCharItem;
		pclMaterialChar	= (cltCharServer*)pclCM->CR[ pclinfo->siMaterialCharUnique ];

	}

	// 원료 인벤의 포인터를 확보한다. 
	// 의뢰자가 없는 경우는 내 아이템정보로 할당한다.
	if(pclmaterialinv == NULL)
	{
		pclmaterialinv = &pclCI->clCharItem;
		pclMaterialChar	= this;
	}


	//---------------------------------------------
	// 제작자에게 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	ItemUseReserveAction();        

	//---------------------------------------------
	// 제작자에게 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	BulletUseReserveAction();



	SI32 makeitemunique = pclinfo->siItemUnique;
	SI32 makeitemnum	= pclinfo->siItemNum;
	// 아이템이 중첩되지 않는 것이라면 생산 개수는 강제로 1로 조정된다. 
	SI64 atb = pclClient->pclItemManager->GetItemAtb(makeitemunique);
	if(atb & ITEMATB_OBJECT)
	{
		makeitemnum = 1;
	}

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)
	{
		siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		SendNeighbourMyMakeItemStatus();

		return false;
	}

	// 이 캐릭터의 행운 지수를 구한다.
	bool bluckuseswitch = false;			// 행운 요소가 사용되었는가?

	// 생산물의 가격 정보를 구한다. 
	cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(makeitemunique); 


	SI32 returnval		= 0;
	SI32 returnpara1	= 0;
	//__try
	{

		//----------------------------------------------------------------------
		// 튜토리얼 제조 퀘스트 중인지 체크한다.
		//-----------------------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_TUTORIALMAKEITEM;
		if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
			&& 	pclinfo->siMaterialCharUnique == 0	) // 제조의뢰 이면 안된다.
		{
			if(para1 == makeitemunique
				&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == para1)
				&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 < para2)
				)				
			{
				bTutorialQuest = TRUE;
			}
		}

		questclearcond =  QUEST_CLEAR_COND_TUTORIALMAKEITEM;
		if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
			&& 	pclinfo->siMaterialCharUnique == 0	) // 제조의뢰 이면 안된다.
		{
			if(para1 == pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel
				&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == para1)
				&& ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 < para2)
				)				
			{
				bTutorialQuest = TRUE;
			}
		}

		// 아이템을 만들수 없다면 돌아간다.
		if(CanMakeItem(pclinfo, &returnval, &returnpara1) == false)
		{
			// true, false로 돌려준다.
			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}

		//-------------------------------------------
		// 생산에 필요한 재료와 그 개수를 정리한다. 
		//-------------------------------------------
		SI32 makeunit = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit;
		if(makeunit <= 0)
		{
			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}

		// 생산 개수가 잘못 지정되어 왔다. 
		// 재조정한다. 
		makeitemnum  = makeitemnum / makeunit;
		if( makeitemnum <= 0)makeitemnum	= 1;
		makeitemnum = makeitemnum * makeunit;

		SI32 makerate = makeitemnum / makeunit;

		// DB에 재료 전송은 - 성공기원석 자리를 확보한다.
		DBMaterialInfo clinfo[MAX_MATERIAL_NUMBER_FOR_DB];
		SI32 materialindex= 0;


		// 평범한 제조 라면 원래 재료를 대입한다.

		cltMaterialInfo clMaterials[MAX_MATERIAL_NUMBER];

		if(bTutorialQuest == FALSE)
		{
#ifdef _SAFE_MEMORY
			memcpy( clMaterials	, &pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[0], MAX_MATERIAL_NUMBER * sizeof(cltMaterialInfo) );
#else
			memcpy( clMaterials	, pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial, MAX_MATERIAL_NUMBER * sizeof(cltMaterialInfo) );
#endif
		}
		else	// 튜토리얼 퀘스트 중이라면 퀘스트용 재료를 구해와 대입한다. 퀘스트용 재료가 따로 없으면 원래 재료를 대입한다.
		{
			cltMaterialInfo clQuestMaterialInfo[MAX_MATERIAL_NUMBER];
			if(pclClient->pclQuestManager->GetTutorialMakeQuestItem(questunique,makeitemunique,clQuestMaterialInfo) == false)
			{
#ifdef _SAFE_MEMORY
				memcpy( clMaterials, &pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[0], MAX_MATERIAL_NUMBER * sizeof(cltMaterialInfo) );
#else
				memcpy( clMaterials, pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial, MAX_MATERIAL_NUMBER * sizeof(cltMaterialInfo) );
#endif
			}	
			else
			{
				memcpy( clMaterials	, clQuestMaterialInfo ,MAX_MATERIAL_NUMBER * sizeof(cltMaterialInfo) );
			}			
		}

		for(i = 0;i < MAX_MATERIAL_NUMBER;i++)
		{
			SI32 materialunique = clMaterials[i].siUnique;
			if(materialunique <= 0)continue;
			SI32 materialnum	= clMaterials[i].siNeedNum;

			bool bmaterialfinddswitch = false;

			for(SI32 k =0 ; k < 28 ; k++) //자기 자신일때
			{

				if(bmaterialfinddswitch == true)continue;

				SI32 siitemindex = pclCM->CR[materialcharunique]->siInvMakeItemIndex[k];
				if(siitemindex > 0 )
				{
					if(pclmaterialinv->clItem[siitemindex].siUnique == materialunique)
					{
						clinfo[materialindex].siItemUnique	= materialunique;
						clinfo[materialindex].siStgPos		= siitemindex;
						clinfo[materialindex].siUseItemNum	= materialnum * makerate;
						materialindex++;
						j = MAX_INVITEM_PER_PERSON;
						k = 28;
						bmaterialfinddswitch = true;
					}
				}
			}


			if(bmaterialfinddswitch ==false)
			{
				// 인벤에서 해당 재료를 찾는다. 
				for(j = 0;j < MAX_INVITEM_PER_PERSON;j++)
				{
					// 이미 재료를 찾았다면, 같은 재료는 또 찾지 않는다. 
					if(bmaterialfinddswitch == true)break;

					if(  (pclmaterialinv->clItem[PERSONITEM_INV0 + j].siUnique == materialunique) &&
						(pclmaterialinv->clItem[PERSONITEM_INV0 + j].siItemNum >= materialnum * makerate)
						)
					{
						clinfo[materialindex].siItemUnique	= materialunique;
						clinfo[materialindex].siStgPos		= PERSONITEM_INV0 + j;
						clinfo[materialindex].siUseItemNum	= materialnum * makerate;


						materialindex++;

						// 원하는 재료를 찾았다. 
						bmaterialfinddswitch = true;
					}
				}
			}
		}

		// 제조하기 위해 올려놓은 재료중에 성공기원석이 있는지 찾아본다.
		for(SI32 k =0 ; k < 28 ; k++)
		{
			SI32 siitemindex = pclCM->CR[materialcharunique]->siInvMakeItemIndex[k];
			if(siitemindex > 0 )
			{
				// 성공기원석은 제조100%를 보장한다.
				if(	pclmaterialinv->clItem[siitemindex].siUnique == ITEMUNIQUE(5395) &&
					pclmaterialinv->clItem[siitemindex].siItemNum >= 1)
				{
					clinfo[materialindex].siItemUnique	= pclmaterialinv->clItem[siitemindex].siUnique;
					clinfo[materialindex].siStgPos		= siitemindex;
					clinfo[materialindex].siUseItemNum	= 1;
					materialindex++;

					bForce_Manufacture_Success = TRUE;
				}
			}
		}

		//KHY - 1211 - 과금상품 - 만능 도구
		if(returnval == SRVAL_RETURN_USE_ALLPURPOSETOOL) // 만능 도구를 사용.
		{
			SI32 itemunique = ITEMUNIQUE(8109) ; // 만능도구
			SI32 id = GetID();
			cltCharServer* pclChar = NULL;
			pclChar = (cltCharServer*)pclCM->CR[id];

			SI32 pos = pclChar->pclCI->clCharItem.GetItemPos(itemunique);

			clinfo[materialindex].siItemUnique	= itemunique;
			clinfo[materialindex].siStgPos		= pos;
			clinfo[materialindex].siUseItemNum	= 1;
			materialindex++;
		}
		//------------------------------------------
		// 생산에 필요한 도구의 내구도를 차감시킨다.
		//------------------------------------------
		if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
		{
			SI32 siSkillLevel = pclClient->pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;
			SI32 decdur = max(1, siSkillLevel/6);

			for(i = 0;i < MAX_TOOL_NUMBER;i++)
			{
				SI32 toolunique = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clTool[i].siUnique;
				if(toolunique <= 0)continue;

				bool alreadydealswitch = false;

				// 인벤에서 해당 재료를 찾는다. 
				for(j = 0;j < MAX_INVITEM_PER_PERSON;j++)
				{
					// 이미 필요한 도구를 찾았다면 , 
					if(alreadydealswitch)continue;
					cltItem* pclToolItem = &pclCI->clCharItem.clItem[PERSONITEM_INV0 + j];

					if(pclToolItem->siUnique == toolunique)
					{
						// 내구도를 소모시킨다.
						if ( 0 < pclToolItem->GetDurability() )
						{
							DecreaseEquipDur( PERSONITEM_INV0 + j, decdur );

							if ( 0 >= pclToolItem->GetDurability() )
							{
								UseItemReasonDurabilityZero( PERSONITEM_INV0 + j );
							}
						}
						else
						{
							returnval	= SRVAL_NOTENOUGHTOOLDURABILITY;		// 도구의 내구도가 부족하다. 
							para1		= toolunique;
							para2		= decdur;

							//----------------------------------------------
							// 생성에 실패했을 경우
							//----------------------------------------------

							SetRareItemRate(0,0,0,0);

							siPersonalMakeItemType = 0;

							cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
							cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
							SendNetMsg((sPacketHeader*)&clMsg);

							//----------------------------------------------
							// 이웃들에게도 알린다. 
							//----------------------------------------------
							SendNeighbourMyMakeItemStatus();
							return false;
						}

						alreadydealswitch = true;
					}
				}
			}
		}

		//--------------------------------------------
		// 확률. 
		//--------------------------------------------
		SI32 needskillunique	= 0;
		needskillunique			= pclClient->pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
		SI32 needskilllevel		= 0;
		needskilllevel			= pclClient->pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

		//cyj 스킬레벨에 따라 생산 아이템갯수 정하는 루틴 함수로 뺌
		if(bTutorialQuest == FALSE && pclinfo->bNPCMake == false )
		{
			SI32 siItemNum = 0;

			//결정체 제조에 따른 수량 구분.
			if(pclClient->pclItemManager->pclItemInfo[ref]->siType != ITEMTYPE_CRYSTAL)
			{
				siItemNum = GetMakeItemNum(needskillunique, needskilllevel, makeitemunique, makeitemnum);			
			}
			else
				siItemNum = makeitemnum;

			if(bForce_Manufacture_Success && siItemNum < makeitemnum)
			{
				// 제조 성공효과가 있는데 실패라면, 개수를 원래 상태로 유지해준다.
				makeitemnum = makeitemnum;
			}
			else
			{
				makeitemnum = siItemNum;
			}
		}
		else
		{

			//[영진] 튜토리얼 퀘스트 중엔 스킬레벨에 따른 아이템 갯수의 변화가 없다.
		}

		// 몬스터 주식회사 점수에 따라 나올 갯수를 더하거나 감한다. 오브젝트가 아닌 경우
		// 미니게임 점수가 10000점 넘어가면 무조건 10000점으로 처리
		if( 10000 <= siMonsterCorp )
		{
			siMonsterCorp = 10000;
		}

		if((atb & ITEMATB_OBJECT) == 0)
		{
			SI32 rate = min(1000,max(-1000,siMonsterCorp / 10)) ;

			makeitemnum = makeitemnum + ( ( makeitemnum * rate ) / 10000 ); 
		}

		//결정체 제조에 따른 수량 구분.
		if(pclClient->pclItemManager->pclItemInfo[ref]->siType == ITEMTYPE_CRYSTAL)
		{
			makeitemnum = GetMakeCrystalItemNum(needskillunique, needskilllevel, makeitemunique, makeitemnum);							
		}


		//cyj 몬스터 주식회사 아이템 증가이후론 많아도 실패안되도록
		//// 1만개 이상 생산은 불가. 
		//if(makeitemnum > MAX_ITEM_PILE_NUMBER)
		//{
		//	returnval	= SRVAL_TOOMANYITEMMAKE;	
		//	para1		= makeitemnum;
		//	return false;
		//}

		//------------------------------------------
		// 만들어낼 물건을 준비한다. 
		//------------------------------------------
		cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclClient->pclItemManager;
		SI32 addlevel = 0;
		SI32 qualityrrate = pclitemmanager->GetQualityRate(makeitemunique, GetID(), &addlevel);

		//------------------------------------------
		// 수호정령에 의한 아이템 능력 향상
		//------------------------------------------
		if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
		{
			SI32 siAddQualIty = SoulGuardAbilityActivity( SOULGUARD_ABILITY_MAKEABILITY_UP );
			if ( 0 < siAddQualIty )
			{
				qualityrrate = qualityrrate + ((qualityrrate*siAddQualIty)/100);
			}
		}

		// 생산 실패. 
		// obj아이템인 경우 성능을 낮춰서 만든다. 
		bool balmostfailswitch = false;

		// 제조실패방지 - by LEEKH 2007.07.23
		//if( pclClient->IsWhereServiceArea(ConstSwitch_DoNotFailedMakeItem) )
		if( pclClient->IsCountrySwitch(Switch_DoNotFailedMakeItem) )
		{
			if(makeitemnum == 0 && atb & ITEMATB_OBJECT)
			{
				makeitemnum	= 1;
				qualityrrate = rand() % 41 + 10;
				balmostfailswitch	= true;
			}
		}

		//cyj 아이템럭 제외
		//// 운이 적용되어 실패를 모면한다. 
		//SI32 luckquality = 0;
		//if(pclprice)
		//{
		//	SI32 curprice	= pclprice->siPrice;
		//	
		//	// 패자부활 
		//	if(makeitemnum == 0)
		//	{
		//		if(atb & ITEMATB_OBJECT)
		//		{
		//			curprice /= 3000;
		//			
		//			if(curprice)
		//			{
		//				SI32 luckrate = 20 + min(30, luck * 100 / curprice);
		//				
		//				if((rand() % 100) < luckrate)
		//				{
		//					makeitemnum	= 1;
		//					qualityrrate = 20 + rand() % 10;
		//					
		//					bluckuseswitch		= true;
		//					balmostfailswitch	= true;
		//				}
		//			}
		//		}
		//	}
		//	
		//	// 운에 의해 성능이 개선될지 여부, 
		//	curprice	= pclprice->siPrice;
		//	if(atb & ITEMATB_OBJECT)
		//	{
		//		curprice /= 3000;
		//		
		//		if(curprice)
		//		{
		//			SI32 luckrate = min(50, luck * 100 / curprice);
		//			
		//			if((rand() % 100) < luckrate)
		//			{
		//				SI32 data = pclprice->siPrice / 10000;
		//				if(data)
		//				{
		//					luckquality = min(15, luck * 100 / data);
		//					bluckuseswitch	= true;
		//				}
		//			}
		//		}
		//	}
		//}


		cltItem clOutputItem;
		SI16 brareswitch = 0;

		// 운영자는 설정된 RareRate를 사용한다.
		SI32 RareRate[4];
		for ( int k = 0 ; k < 4 ; k ++ )	RareRate[k] = GetRareItemRate(k);

		// 아이템을 생성한다.
		if(pclClient->pclItemManager->MakeRandItemUnique(makeitemunique, &clOutputItem,
			qualityrrate , addlevel, &brareswitch,
			RareRate[0],RareRate[1],RareRate[2],RareRate[3],siMonsterCorp) == false)
		{
			//----------------------------------------------
			// 생성에 실패했을 경우
			//----------------------------------------------

			SetRareItemRate(0,0,0,0);

			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();


			return false;
		}
		SetRareItemRate(0,0,0,0);

		SI16 siMakerServerUnique = 0;
		// 명품이면 명품 코드를 설정한다.
		// 명품이면 명품 코드를 설정한다.
		if(brareswitch == 1 )			// 명품
		{
			clOutputItem.siMakerPersonID = personid;
			clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_2;
			siMakerServerUnique = pclserver->siServerUnique;
		}
		else if ( brareswitch == 2 )	// 호품
		{
			clOutputItem.siMakerPersonID = personid;
			clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_1;
			siMakerServerUnique = pclserver->siServerUnique;
		}
		else if ( brareswitch == 3 && pclClient->siServiceArea != ConstServiceArea_China )	// 고급품
		{
			clOutputItem.siMakerPersonID = personid;
			clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_5;
			siMakerServerUnique = pclserver->siServerUnique;
		}
		// 고물품
		else if( -1 == brareswitch )
		{
			clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_6;
		}


		// 명품 또는 호품일 경우
		if ( brareswitch == 1 || brareswitch == 2 )
		{
			// 길드가 있다면 길드포인트를 증가시킨다.
			if ( pclCI->clBI.clGuildRank.IsGuildMember() == true )
			{
				cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCI->clBI.clGuildRank.GetGuildUnitIndex());
				if ( pGuild )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCI->GetPersonID(),100,TYPE_GUILDPOINT_ADD_MAKERARE);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}
			}
		}

		//----------------------------------------------
		// 인벤에 아이템을 넣을 공간이 있는지 확인한다. 
		//----------------------------------------------

		// 해당 인벤에 빈자리가 있는지 확인한다. 
		SI16 sitopos;
		cltItem cltoitem;
		SI32 rtnvalue;

		clOutputItem.SetItemNum(makeitemnum);
		if(makeitemnum	== 0)
		{
			bluckuseswitch = false;		// 생산에 실패하는 경우에는 영향 받지 않는다. 

			clOutputItem.Init();

			// 실패했음을 클라이언트로 알린다. 
			cltItem clitem;
			clitem.siUnique = makeitemunique;
			cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_MAKEFAIL, 0, 0, &clitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			siPersonalMakeItemType = 0;
			cltGameMsgResponse_MakeItemSwitch clinfo2( 0 );
			cltMsg clMsg2(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo2), (BYTE*)&clinfo2);
			SendNetMsg((sPacketHeader*)&clMsg2);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();


		}

		if(pclMaterialChar->CanAddInv(INVMODE_ALL, &clOutputItem, &sitopos,&cltoitem, &rtnvalue) ==  false)
		{
			if ( makeitemnum > 0 )
			{
				returnval	= SRVAL_NOEMPTYAREA;		// 빈 자리가 없어서 실패.

				siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
				cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				SendNeighbourMyMakeItemStatus();

				return false;
			}
		}

		// 배고프게 만든다. 
		MakeHunger();

		// 이제형바보
		SI32 ordernumber = min(pclinfo->siItemNum, makeitemnum);
		//---------------------------------------------------------------
		// 의뢰자와 제작자가 다른 경우, 의뢰자의 개인상점 정보를 갱신한다. 
		//---------------------------------------------------------------
		bool	bRequest_Want_Make	=	false	;
		// 만약 제작의뢰 라면
		if ( pclinfo->bRequestManufacture ==true ) 
		{
			bRequest_Want_Make	= true	;
			// 제작의뢰자가 샵상태인지 체크 해야한다
			cltCharServer* pclcharshopper	= (cltCharServer* )pclCM->CR[ pclinfo->siMakeCharUnique ];	
			if(pclcharshopper == NULL)	return false;	//잘못된 케릭터정보
			if(pclcharshopper->bPersonalShopSwitch == false)
			{
				siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
				cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				SendNeighbourMyMakeItemStatus();

				return false;// 상점을 개설하고 있지 않다.
			}
			// 개인 상점중 제조의뢰중인지 확인한다. 
			if(pclcharshopper->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM_WANT)
			{
				siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
				cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				SendNeighbourMyMakeItemStatus();

				return false;// 상점의 모드가 MAKE ITEM이 아니다.
			}
			// 제조의뢰품목이 동일한지 확인한다. 
			if(pclcharshopper->clShopData.m_clItem[0].siUnique != pclinfo->siItemUnique)
			{
				siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
				cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				SendNeighbourMyMakeItemStatus();

				return false;
			}
			// 제조 의뢰 개인상점에 남아있는 의뢰 아이템 수가 있는지를 검사한다.
			if(ordernumber <= 0)
			{
				siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
				cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				SendNeighbourMyMakeItemStatus();

				return false;
			}
			// 제조 의뢰 개인상점에 남아있는 아이템수를 Update 한다.
			pclcharshopper->clShopData.m_clItem[0].siItemNum =  0	;
			//---------------------------------------------------------------
			sifee	= pclcharshopper->clShopData.m_siPrice[0];
		}
		else
		{
			///그외의 경우 
			if(pclinfo->siMaterialCharUnique)
			{
				cltCharServer* pclcharshopper	= (cltCharServer* )pclCM->CR[ materialcharunique ];
				if(pclcharshopper->bPersonalShopSwitch == false)
				{
					siPersonalMakeItemType = 0;

					cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
					cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
					SendNetMsg((sPacketHeader*)&clMsg);

					//----------------------------------------------
					// 이웃들에게도 알린다. 
					//----------------------------------------------
					SendNeighbourMyMakeItemStatus();

					return false;// 상점을 개설하고 있지 않다.
				}
				// 개인 상점중 제조의뢰중인지 확인한다. 
				if(pclcharshopper->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM)
				{
					siPersonalMakeItemType = 0;

					cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
					cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
					SendNetMsg((sPacketHeader*)&clMsg);

					//----------------------------------------------
					// 이웃들에게도 알린다. 
					//----------------------------------------------
					SendNeighbourMyMakeItemStatus();

					return false;// 상점의 모드가 MAKE ITEM이 아니다.
				}
				// 제조의뢰품목이 동일한지 확인한다. 
				if(pclcharshopper->clShopData.m_clItem[0].siUnique != pclinfo->siItemUnique)
				{
					siPersonalMakeItemType = 0;

					cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
					cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
					SendNetMsg((sPacketHeader*)&clMsg);

					//----------------------------------------------
					// 이웃들에게도 알린다. 
					//----------------------------------------------
					SendNeighbourMyMakeItemStatus();

					return false;
				}
				// 제조 의뢰 개인상점에 남아있는 의뢰 아이템 수가 있는지를 검사한다.
				if(pclcharshopper->clShopData.m_clItem[0].siItemNum - ordernumber < 0)
				{
					siPersonalMakeItemType = 0;

					cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
					cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
					SendNetMsg((sPacketHeader*)&clMsg);

					//----------------------------------------------
					// 이웃들에게도 알린다. 
					//----------------------------------------------
					SendNeighbourMyMakeItemStatus();

					return false;
				}
				// 제조 의뢰 개인상점에 남아있는 아이템수를 Update 한다.
				pclcharshopper->clShopData.m_clItem[0].siItemNum -= ordernumber;
				//---------------------------------------------------------------
				sifee	= pclcharshopper->clShopData.m_siPrice[0];
			}
		}
		// NPC제작의 경우에
		if ( pclinfo->bNPCMake )
		{
			// 현제 시세의 10% 를 수수료로 지불한다.
			cltItemPriceUnit* pPrice = pclitemmanager->GetCurrentPrice( clOutputItem.siUnique );
			if ( pPrice )
			{
				sifee = pPrice->siPrice / 10;
			}
			else
			{
				return false;
			}
		}

		//-------------------------------------------
		// 만들 수 있으면 DBManager에게 요청한다. 
		//-------------------------------------------
		// 아이템이 추가되는 자리를 예약한다. 
		pclMaterialChar->clSimpleVary.dwItemInputReserveClock[sitopos] = pclClient->CurrentClock;

		sDBRequest_MakeItemInv clMsg(GetID(), personid, materialpersonid, clinfo,/* clToolList,*/ sitopos, &clOutputItem, clOutputItem.CanPile(pclClient->pclItemManager), balmostfailswitch, clOutputItem.IsCreateSID(pclClient->pclItemManager), 0, 
			sifee * ordernumber,  ordernumber, pclinfo->siItemUnique, pclinfo->bNPCMake, siMakerServerUnique,bRequest_Want_Make);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


		//------------------------------------------------------
		// 소모된 원료를 통계낸다. - 성공기원석 포함
		//------------------------------------------------------
		for(i = 0;i < MAX_MATERIAL_NUMBER_FOR_DB;i++)
		{
			if(clinfo[i].siItemUnique)
			{
				pclitemmanager->AddUseNum(clinfo[i].siItemUnique, clinfo[i].siUseItemNum);

				// 새로운 통계 - 아이템 사용 통계를 낸다.
				NStatistics_Add("Item", clinfo[i].siItemUnique, "UsedMakeMtrl", clinfo[i].siUseItemNum);
				NStatistics_Set("Item", clinfo[i].siItemUnique, "Name", pclClient->pclItemManager->GetName(clinfo[i].siItemUnique));
			}
		}

		//-------------------------------------------------------
		// 행운이 사용된 경우 행운 장비들의 내구도를 감소시킨다. 
		//-------------------------------------------------------
		//DecreaseSkillItemEquipDur(needskillunique, true);

		// 명품이 만들어진 경우 전체 공지로 알린다. 
		if( makeitemnum && brareswitch > 0)
		{
			// 기록을 남긴다. 
			TCHAR itemname[128]=TEXT("");
			if( clOutputItem.GetFullName(((cltServer*)(pclClient))->pclItemManager, itemname, 128) == true)
			{
				SI32 extraskilllevel = addlevel % 1000;		// 스킬 수치 (차이값)
				SI32 extrahand = (addlevel / 1000) / 5;		// 손재주 수치 (차이값)

				switch( brareswitch )
				{
				case RARELEVEL_1:	// 호품
				case RARELEVEL_2:	// 명품
					{
						((cltServer*)(pclClient))->pclLog->FilePrint(TEXT("config\\RareItemRecord.log"), TEXT("Person:[%s]	item:[%s]	stiwch[%d]	useday[%d]	ExtraSkill[%d]	ExtraHand[%d]"), GetName(), itemname, RAREITEM_NOTICE_SWITCH_MADE , clOutputItem.uiDateUseDay, extraskilllevel, extrahand);
					}
					break;
				case RARELEVEL_5:	// 고급품
					{
						((cltServer*)(pclClient))->pclLog->FilePrint(TEXT("config\\RareItemRecord_HighGrade.log"), TEXT("Person:[%s]	item:[%s]	stiwch[%d]	useday[%d]	ExtraSkill[%d]	ExtraHand[%d]"), GetName(), itemname, RAREITEM_NOTICE_SWITCH_MADE , clOutputItem.uiDateUseDay, extraskilllevel, extrahand);
					}
					break;
				case RARELEVEL_6:	// 고물품 - 기록을 남기지 않음.
				case RARELEVEL_3:	// 사용안함
				case RARELEVEL_4:	// 사용안함
					{
					}
					break;
				}
			}

			// 명품, 호품일때만 알려준다.
			if( 1 == brareswitch || 2 == brareswitch  )
			{
				SI32 makerid = pclCM->GetIDFromPersonID(pclCI->GetPersonID());

				((cltServer*)(pclClient))->SendRareItemMade(makerid, &clOutputItem, RAREITEM_NOTICE_SWITCH_MADE );
			}
		}
		// [영훈] 보수 소환수를 제조했을때 
		else if ( (makeitemnum) && (( ITEMUNIQUE(12520) == clOutputItem.siUnique ) || ( ITEMUNIQUE(12525) == clOutputItem.siUnique )) ) 
		{
			SI32 makerid = pclCM->GetIDFromPersonID(pclCI->GetPersonID());

			((cltServer*)(pclClient))->SendRareItemMade(makerid, &clOutputItem, RARAITEM_NOTICE_SWITCH_SUMMONDOLL );
		}


		//KHY - 0227 - 결정체 (크리스탈) 제조 추가
		// 제조시 무기와 방어구 모두의 내구도를 깍는다.

		DecreaseCrystalDurAC(CRYSTAL_DECREASE_DURABILITY_MKNUM);
		DecreaseCrystalDurDE(CRYSTAL_DECREASE_DURABILITY_MKNUM);


		//[지연] - 아이템 생산 작업이 완료 되면 3% 확률로 1개의 쿠키조각을 지급 (이벤트 기간이 아니거나 생성한 아이템이 쿠키 일땐 작동 안함)
		if ( ITEMUNIQUE(13063) != clOutputItem.siUnique )
		{
			if ( 0 < needskillunique && 0 < needskilllevel )
			{
				GiveFortuneCookiePiece( 30, 1);		
			}
		}
	}
	//__finally
	{
		if( returnval > 0 )
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendServerResponseMsg(0, returnval,  para1, para2, GetCharUnique());
			siMonsterCorp = 0 ;
			dwStartMiniGameTime = 0;
		}

	}

	return true;
}


// 주문을 근거로 물건을 만든다. 
bool cltCharServer::EnchantItem(cltGameMsgRequest_MakeItemInv* pclinfo)
{
	SI32 i;
	SI16 para1	= 0;		// 반드시 초기화되어야 한다. 
	SI16 para2	= 0;
	SI64 sifee	= 0;

	SI32 personid = pclCI->GetPersonID();
	if(personid <= 0)
		return false;

	// 제작 의뢰자가 설정되어 있으나 유효하지 않으면 실패. 
	cltPItemInfo* pclmaterialinv = NULL;
	cltCharServer* pclMaterialChar	= NULL;

	SI32 materialpersonid = personid;
	SI32 materialcharunique	 = 0;

	BOOL bUseBlessingStone = FALSE;
	SI32 siSuccessUpItemNum = 0; 

	if(pclinfo->siMaterialCharUnique==0)
		materialcharunique	= GetCharUnique();
	else
		materialcharunique= pclinfo->siMaterialCharUnique;


	if(pclinfo->siMaterialCharUnique)
	{
		if(pclCM->IsAlive(pclinfo->siMaterialCharUnique) == false)
		{
			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}

		// 의뢰자에게 예약되어 있는 아이템 사용이 있다면 처리한다. 
		((cltCharServer*)(pclCM->CR[ pclinfo->siMaterialCharUnique ]))->ItemUseReserveAction();

		materialcharunique	= pclinfo->siMaterialCharUnique;
		materialpersonid	= pclCM->CR[ pclinfo->siMaterialCharUnique ]->pclCI->GetPersonID();
		pclmaterialinv = &pclCM->CR[ pclinfo->siMaterialCharUnique ]->pclCI->clCharItem;
		pclMaterialChar	= (cltCharServer*)pclCM->CR[ pclinfo->siMaterialCharUnique ];

	}

	// 원료 인벤의 포인터를 확보한다. 
	if(pclmaterialinv == NULL)
	{
		pclmaterialinv = &pclCI->clCharItem;
		pclMaterialChar	= this;
	}


	//---------------------------------------------
	// 제작자에게 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	ItemUseReserveAction();                                                                                                                                                                                                                                                              



	SI32 makeitemunique = pclinfo->siItemUnique;
	SI32 makeitemnum	= 1;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)
	{
		siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		SendNeighbourMyMakeItemStatus();

		return false;
	}



	// 생산물의 가격 정보를 구한다. 
	cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(makeitemunique); 


	SI32 returnval		= 0;
	SI32 returnpara1	= 0;
	// by LEEKH 2008-10-06 __try 구문 삭제
	//__try
	{

		// 아이템을 만들수 없다면 돌아간다.
		if(CanEnchantItem(pclinfo, &returnval, &returnpara1) == false) 
		{
			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}
		//-------------------------------------------
		// 생산에 필요한 재료와 그 개수를 정리한다. 
		//-------------------------------------------
		SI32 makeunit = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit;
		if(makeunit <= 0)
		{
			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}
		// 생산 개수가 잘못 지정되어 왔다. 
		// 재조정한다. 

		if( makeitemnum <= 0)makeitemnum	= 1;
		SI32 makerate = makeitemnum / makeunit;

		DBMaterialInfo clinfo[MAX_ENCHANT_NUMBER];
		SI32 matunique[MAX_ENCHANT_NUMBER] ;	

		for(i =0; i < MAX_ENCHANT_NUMBER; i++)
		{
			clinfo[i].Init();
			matunique[i]= 0;
		}

		////////// 인첸트 아이템 재료로 축복의 돌이 들어갔는지 체크

		bUseBlessingStone = FALSE;
		siSuccessUpItemNum = 0;
		BOOL bUseSealingIgnore = FALSE;

		for(SI32 i =0; i< NOW_ENCHANT_NUMBER; i++)
		{
			SI32 siItemIndex = 0;

			siItemIndex  = pclCM->CR[GetCharUnique()]->siElementEnchantInx[i];

			if( siItemIndex > 0	&& siItemIndex < MAX_ITEM_PER_PERSON )
			{
				if (pclmaterialinv->clItem[siItemIndex].siUnique == ITEMUNIQUE(5370) )
				{
					bUseBlessingStone = TRUE;
				}

				if( pclmaterialinv->clItem[siItemIndex].siUnique == ITEMUNIQUE(13606) )
				{
					siSuccessUpItemNum = pclmaterialinv->clItem[siItemIndex].GetItemNum();
					siSuccessUpItemNum = min(10,siSuccessUpItemNum);
				}
				if( pclmaterialinv->clItem[siItemIndex].siUnique == ITEMUNIQUE(13607) )
				{
					bUseSealingIgnore = TRUE;
				}
			}
		}

		SI32 materialindex= 0;		
		SI32 OrgInx =pclCM->CR[GetCharUnique()]->siOrgInvEnchantInx; //pclCM->CR[materialcharunique]->siOrgInvEnchantInx;
		cltItem orgitem = pclmaterialinv->clItem[OrgInx];

		// 귀속이 안된 아이템이라면 일시밀봉 못을 사용하지 않는다.
		if( bUseSealingIgnore && orgitem.IsBelonging( ((cltServer*)pclClient)->pclItemManager ) == false )
		{
			bUseSealingIgnore = FALSE;
		}

		stEnchantNeed materialunique1;
		pclClient->pclItemManager->GetEnchantNeedItem(orgitem, &materialunique1, siSuccessUpItemNum ,bUseBlessingStone,bUseSealingIgnore);

		//cyj 운영자 명령으로 인첸트 100% 성공
		if(bSuccessEnchant && GetGameMasterLevel() > BASIC_GAMEMASTER_LEVEL)
		{
			materialunique1.EnchantRate	= 1.0f;
			materialunique1.siNeedSkill = 0;
		} 

		//if(	pclClient->IsWhereServiceArea( ConstSwitch_EnchantNoSealing) )
		if(	pclClient->IsCountrySwitch( Switch_EnchantNoSealing) )
		{
			//KHY - 0720 - 인첸트 개선 귀속 아이템도 인챈트 가능.
		}
		else
		{
			// 귀속무시 아이템도 사용 안했고 봉인이 풀려있다면 대상이 될수 없다.		
			if( orgitem.IsBelonging( ((cltServer*)pclClient)->pclItemManager ) && bUseSealingIgnore == FALSE )	
			{
				cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_NOSEALINGUP_ENCHANTFAIL, 0, 0, &orgitem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg);

				siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo1( 0 );
				cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo1), (BYTE*)&clinfo1);
				SendNetMsg((sPacketHeader*)&clMsg1);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				SendNeighbourMyMakeItemStatus();

				return false;
			}
		}
		SI32 siMaxGrade = pclClient->pclItemManager->GetItemGrade(orgitem.siUnique);
		SI32 siOrgGrade = orgitem.Element.siPower;

		//최상위 인챈트 아이템 인지 체크
		if(siMaxGrade <= siOrgGrade)
		{
			cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_ENCHANTITEMFAIL, 0, 0, &orgitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo1( 0 );
			cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo1), (BYTE*)&clinfo1);
			SendNetMsg((sPacketHeader*)&clMsg1);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			return false;
		}

		bool bitemenchant = true;

		SI32 materialunique =0;
		SI32 materialnum =0;
		SI32 siitemindex =0;

		for(SI32 i =0; i< MAX_ENCHANT_NUMBER; i++)
		{
			bool bmaterialfinddswitch = false;

			matunique[i] = materialunique1.siElementUnique[i];
			materialunique = matunique[i];
			if(materialunique <= 0) continue;
			for(SI32 k =0 ; k < (NOW_ENCHANT_NUMBER+1)  ; k++) //자기 자신일때				
			{
				materialnum = 0;

				if(bmaterialfinddswitch == true)continue;

				siitemindex = 0;
				if(k==0 )
					siitemindex = pclCM->CR[GetCharUnique()]->siMatInvEnchantInx;
				else
					siitemindex = pclCM->CR[GetCharUnique()]->siElementEnchantInx[k -1];

				materialnum= materialunique1.siElementNum[i];
				if(siitemindex > 0 )
				{
					if(pclmaterialinv->clItem[siitemindex].siUnique == materialunique)
					{
						clinfo[materialindex].siItemUnique	= materialunique;
						clinfo[materialindex].siStgPos		= siitemindex;
						clinfo[materialindex].siUseItemNum	=materialnum;						
						k =  (NOW_ENCHANT_NUMBER+1);
						bmaterialfinddswitch = true;
						if(clinfo[materialindex].siUseItemNum > pclmaterialinv->clItem[siitemindex].siItemNum)
							bitemenchant= false;

						materialindex++;

					}
				}
			}
			// 루프가 끝났을때까지 아이템을 찾지 못했다면
			if(bmaterialfinddswitch == false)
			{
				bitemenchant= false;
			}
		}

		siitemindex = 0;
		siitemindex = pclCM->CR[GetCharUnique()]->siOrgInvEnchantInx;

		if(bitemenchant==false)
		{
			//재료가 없다.
			cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_NOMATENCHANTITEMFAIL, 0, 0, &orgitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);


			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo1( 0 );
			cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo1), (BYTE*)&clinfo1);
			SendNetMsg((sPacketHeader*)&clMsg1);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();
			return false;

		}

		if( materialunique1.siElementUnique[0]==0)
		{
			//등급이 10등급이므로 더 이상 업그레이드를 할 필요가 없다.
			cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_LASTENCHANTITEMFAIL, 0, 0, &orgitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo1( 0 );
			cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo1), (BYTE*)&clinfo1);
			SendNetMsg((sPacketHeader*)&clMsg1);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();
			return false;
		}




		//------------------------------------------
		// 만들어낼 물건을 준비한다. 
		//------------------------------------------
		cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclClient->pclItemManager;


		cltItem clOutputItem;		
		SI16 brareswitch = 0;

		//----------------------------------------------
		// 인벤에 아이템을 넣을 공간이 있는지 확인한다. 
		//----------------------------------------------
		// 해당 인벤에 빈자리가 있는지 확인한다. 
		SI16 sitopos;

		// 배고프게 만든다. 
		MakeHunger();

		clOutputItem.Set(&pclmaterialinv->clItem[OrgInx]);
		sitopos = pclCM->CR[GetCharUnique()]->siOrgInvEnchantInx;
		//pclmaterialinv->clItem[OrgInx].Element.siPower = materialunique1.siItemGrade;		

		//재료 여부 


		SI32 siItemType = pclitemmanager->GetItemType(clOutputItem.siUnique);
		SI32 simakeflag=true;

		SI32 tempUnique = materialcharunique;
		materialcharunique = GetCharUnique();

		if(clOutputItem.Element.siType!=0)
		{
			pclCM->CR[materialcharunique]->siElementType = clOutputItem.Element.siType;
		}

		switch(siItemType)
		{
		case ITEMTYPE_SWORD:	
			{
				SI32 skillunique  = SKILL_MAKESWORD1;
#ifdef _SAFE_MEMORY
				SI32 level  = pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  = pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;

			}
			break;
		case ITEMTYPE_SPEAR:
			{
				SI32 skillunique  = SKILL_MAKESPEAR1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;

			}
			break;
		case ITEMTYPE_STAFF:			
			{
				SI32 skillunique  = SKILL_MAKESTAFF1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;

			}
			break;
		case ITEMTYPE_AXE:			
			{
				SI32 skillunique  = SKILL_MAKEAXE1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;

			}
			break;
		case ITEMTYPE_BOW:			
			{
				SI32 skillunique  = SKILL_MAKEBOW1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;

			}
			break;
		case ITEMTYPE_GUN:
			{
				SI32 skillunique  = SKILL_MAKEGUN1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;


			}
			break;		
		case ITEMTYPE_ARMOUR:
			{
				SI32 skillunique  = SKILL_MAKEARMOUR1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;


			}
			break;	
		case ITEMTYPE_DRESS:
			{
				SI32 skillunique  = SKILL_MAKEDRESS1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;

			}
			break;
		case ITEMTYPE_BELT:
			{
				SI32 skillunique  = SKILL_MAKEBELT1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;

			}
			break;
		case ITEMTYPE_SHOES:
			{
				SI32 skillunique  = SKILL_MAKESHOES1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;

			}
			break;
		case ITEMTYPE_HELMET:
			{
				SI32 skillunique  = SKILL_MAKEHELMET1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}
			break;
		case ITEMTYPE_HAT:
			{
				SI32 skillunique  = SKILL_MAKEHAT1;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}			
			break;


			// 주문을 근거로 아바타 아이템을 만든다.

			// 아바타 망토
		case ITEMTYPE_AVATAR_MANTLE:
			{
				SI32 skillunique  = SKILL_MAKEAVATAR_MANTLE;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}
			break;

			// 아바타 모자
		case ITEMTYPE_AVATAR_HAT:
			{
				SI32 skillunique  = SKILL_MAKEAVATAR_HAT;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}			
			break;

			// 아바타 의복
		case ITEMTYPE_AVATAR_DRESS:
			{
				SI32 skillunique  = SKILL_MAKEAVATAR_DRESS;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}			
			break;

			// 아바타 목걸이
		case ITEMTYPE_AVATAR_NECK:
			{
				SI32 skillunique  = SKILL_MAKEAVATAR_NECK;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}			
			break;

			// 아바타 반지
		case ITEMTYPE_AVATAR_RING:
			{
				SI32 skillunique  = SKILL_MAKEAVATAR_RING;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}			
			break;

		case ITEMTYPE_CRYSTAL:// 아티팩트 결정 제작 제료
			{
				SI32 skillunique  = SKILL_ARTIFACT;
#ifdef _SAFE_MEMORY
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  &pclCI->clCharItem.clItem[0] , 1);
#else
				SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0,  pclCI->clCharItem.clItem , 1);
#endif
				SI32 MaxLevel = materialunique1.siNeedSkill;
				if(level < MaxLevel) simakeflag = false;
				pclCM->CR[materialcharunique]->siElementType = pclCM->CR[materialcharunique]->siElementType  + 1;
			}			
			break;
		}

		//스킬 부족
		if(simakeflag ==false)
		{
			cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_NOSKILLENCHANTFAIL, 0, 0, &orgitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo1( 0 );
			cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo1), (BYTE*)&clinfo1);
			SendNetMsg((sPacketHeader*)&clMsg1);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();
			return false;

		}

		materialcharunique = tempUnique;

		if(clOutputItem. Element.siPower < 1 )
		{
			clOutputItem.Element.siType = pclCM->CR[materialcharunique]->siElementType;
		}	

		if(clOutputItem.Element.siType < 0 ) //return false;
			simakeflag = false;


		SI32 sitemp = TABS(pclClient->CurrentClock  + rand()) % 1000 ;
		if( sitemp  < (SI32)(materialunique1.EnchantRate * 1000 ))
		{
			simakeflag = ENCHANT_SUCCESS;
		}
		else if( sitemp < (SI32)((materialunique1.EnchantRate + materialunique1.EnchantDownRate) * 1000 ))
		{
			simakeflag = ENCHANT_DOWNGRADE;
		}
		else if( sitemp < (SI32)((materialunique1.EnchantRate + materialunique1.EnchantDownRate + materialunique1.EnchantFailRate) * 1000 ))
		{
			simakeflag = ENCHANT_BROKEN;
		}	
		else
		{
			simakeflag = ENCHANT_FAIL;
		}


		bool bmoneyflag = true;

		cltItemPriceUnit* fee = pclitemmanager->GetCurrentPrice(clOutputItem.siUnique);

		sifee = (SI64)(fee->siPrice * 0.05 ) ;
		if(personid == materialpersonid)
			sifee = 0;


		GMONEY mymoney = pclCM->CR[materialcharunique]->pclCI->clIP.GetMoney();
		if(mymoney < sifee) bmoneyflag = false;
		//소지금 부족
		if(bmoneyflag ==false)
		{
			cltGameMsgResponse_PersonItemInfo clinfo(-1, PERSONITEMINFO_OPTION_NOMONEYENCHANTFAIL, 0, 0, &orgitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo1( 0 );
			cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo1), (BYTE*)&clinfo1);
			SendNetMsg((sPacketHeader*)&clMsg1);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();
			return false;
		}

		//-----------------------------------------------------
		//		  인챈트 성공 실패를 문서기록으로 남긴다.
		//-----------------------------------------------------
#if  MAKE_BY_MUNIN //2007-06-05
		SI32 siItemGrade = clOutputItem.Element.siPower;
		static DWORD dwLastEnchantStatisticClock = 0;
		static bool backUpEnchantDataLoaded = false;

		if((backUpEnchantDataLoaded == false) ||(pclClient->GetFrame() > 100 && TABS(pclClient->CurrentClock - dwLastEnchantStatisticClock) > ENCHANT_WRITE_TIME))
		{
			_SYSTEMTIME sysNowtime;
			GetLocalTime( &sysNowtime );

			dwLastEnchantStatisticClock = pclClient->CurrentClock;

			pclClient->pclItemManager->AddEnchantNum(clOutputItem.siUnique,siItemGrade, clinfo ,simakeflag );
			pclClient->pclItemManager->WriteEnchantStatisticAction(backUpEnchantDataLoaded,sysNowtime);

			if(backUpEnchantDataLoaded ==false)
			{
				pclClient->pclItemManager->AddEnchantNum(clOutputItem.siUnique,siItemGrade, clinfo ,simakeflag );
				backUpEnchantDataLoaded = true;
			}
		}
		else
		{
			pclClient->pclItemManager->AddEnchantNum(clOutputItem.siUnique,siItemGrade, clinfo ,simakeflag );
		}
#endif
		//인챈트 실패 
		if( simakeflag != ENCHANT_SUCCESS )
		{

			cltGameMsgResponse_PersonItemInfo clPersonItemlinfo(-1, PERSONITEMINFO_OPTION_LASTENCHANTITEMFAIL, 0, 0, &orgitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clPersonItemlinfo), (BYTE*)&clPersonItemlinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

			siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clMakeIteminfo( 0 );
			cltMsg clMsg1(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clMakeIteminfo), (BYTE*)&clMakeIteminfo);
			SendNetMsg((sPacketHeader*)&clMsg1);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			SendNeighbourMyMakeItemStatus();

			//실패시 

			if( simakeflag == ENCHANT_BROKEN )
			{
				clOutputItem.Init();	
			}

			else if(simakeflag == ENCHANT_DOWNGRADE )
			{

				--clOutputItem.Element.siPower ;

				// 다운 그레이드된 아이템의 등급이 0 보다 낮거나 같다면 속성도 초기화해준다.
				if( clOutputItem.Element.siPower <= 0 )
				{
					clOutputItem.Element.siType = 0;
				}

			}
		}
		else
		{
			clOutputItem.Element.siPower = materialunique1.siItemGrade;
		}

		cltServer * pclserver = (cltServer*)pclClient;

		sDBRequest_EnchantItemInv clMsg(GetID(), simakeflag , personid, materialpersonid, clinfo, sitopos, &clOutputItem, sifee, clOutputItem.IsCreateSID(pclserver->pclItemManager) );		
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


		//------------------------------------------------------
		// 소모된 원료를 통계낸다. 
		//------------------------------------------------------
		for(SI32 i = 0;i < MAX_ENCHANT_NUMBER; i++)
		{
			if(clinfo[i].siItemUnique)
			{ 
				pclitemmanager->AddUseNum(clinfo[i].siItemUnique, clinfo[i].siUseItemNum);

				// 새로운 통계 - 아이템 사용 통계를 낸다.
				NStatistics_Add("Item", clinfo[i].siItemUnique, "UsedEnchantMtrl", clinfo[i].siUseItemNum);
				NStatistics_Set("Item", clinfo[i].siItemUnique, "Name", pclClient->pclItemManager->GetName(clinfo[i].siItemUnique));
			}
		}
	}
	// by LEEKH 2008-10-06 __try, __finally 삭제
	//	__finally
	{
		if( returnval > 0 )
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendServerResponseMsg(0, returnval,  para1, para2, GetCharUnique());
			siMonsterCorp = 0 ;
			dwStartMiniGameTime = 0;
		}

	}

	return true;
}

// 생산 예약한다.
bool cltCharServer::SetReserverMakeItem(DWORD productclock, cltGameMsgRequest_MakeItemInv* pclinfo)
{
	bMakeItemReserveSwitch = true;
	dwMakeItemStartClock	= pclClient->CurrentClock;
	dwMakeItemDelayClock	= productclock;

	pclReservedMakeItemData->Set(pclinfo);

	return true;
}


// 제조 관련 물품의  내구도를 감소시킨다. 
/*void cltCharServer::DecreaseSkillItemEquipDur(SI32 skillunique, bool bluckdemand)
{
	if( IsPC(GetCharUnique()) == false)return ;

	cltServer* pclserver = (cltServer*)pclClient;

	SI32 itempos = 0;
	for(SI32 i = 0;i < 5;i++)
	{
		if(i == 0)
		{
			itempos = PERSONITEM_HAT;
		}
		if(i == 1)
		{
			itempos = PERSONITEM_HELMET;
		}
		if(i == 2)
		{
			itempos = PERSONITEM_ARMOUR;
		}
		if(i == 3)
		{
			itempos = PERSONITEM_DRESS;
		}
		if(i == 4)
		{
			itempos = PERSONITEM_NECK;
		}

		cltItem* pclitem = &pclCI->clCharItem.clItem[itempos];
		if(pclitem->siUnique <= 0)continue;

		// 특정 기술이 요구되는 경우, 그 기술이 없는 장비는 무시한다. 
		bool bskillswitch = false; 
		if(skillunique && pclitem->GetUpgradeSkillLevel(pclClient->pclItemManager, skillunique))
		{
			bskillswitch = true;
		}

		// 행운이 없는 장비는 무시한다. 
		bool bluckswitch = true;
		//cyj 장비럭제외

		bool bsuccess = false;
		// 기술이 요구되는 상황에서 기술이 존재한다.
		if(skillunique)
		{
			if(bskillswitch == true)bsuccess = true;
		}
		// 운이 요구되는 상황에서 운이 존재한다. 
		if(bluckdemand)
		{
			if(bluckswitch == true)bsuccess = true;
		}

		if(bsuccess == false)continue ;

		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
		if(ref <= 0)	return;

		SI32 decdur = pclClient->pclItemManager->pclItemInfo[ref]->siDecDur;			// 감소시킬 내구도. 

		// 자격이 되지 않는 무기의 사용사 3배의 내구도 감소. 
		SI32 returnval = 0;
		if(CheckEquipCondition(pclClient->pclItemManager, itempos, pclitem, &returnval) == false)
		{
			decdur = 3;
		}

		pclitem->DecreaseDurability(pclserver->pclItemManager, decdur);
		SetWasteDurUpdateSwitch(itempos, true);		// 방어구의 내구도가 변경되었음을 설정 

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		SI32 option = 0;
		if(pclitem->siUnique)			{	option = PERSONITEMINFO_OPTION_NONE;	}
		else							{	option = PERSONITEMINFO_OPTION_WASTE;	}
		cltGameMsgResponse_PersonItemInfo clinfo(itempos, option, 0, 0, pclitem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		SendNetMsg((sPacketHeader*)&clMsg);

	}

}*/

void cltCharServer::DecreaseSkillItemEquipDur( SI32 skillunique )
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )	return;
	if( IsPC(GetCharUnique()) == false)return ;
	cltServer* pclserver = (cltServer*)pclClient;

	if( skillunique <= 0 )																	return;
	for(SI32 i = 0;i < PERSONITEM_INV0;i++)
	{
		cltItem* pclitem = &pclCI->clCharItem.clItem[ i ];
		if( pclitem || pclitem->siUnique <= 0 )												continue;
		if( pclitem->GetUpgradeSkillLevel(pclClient->pclItemManager, skillunique) <= 0 )	continue;

		SI32 decdur = 1;

		pclitem->DecreaseDurability(pclserver->pclItemManager, decdur);
		SetWasteDurUpdateSwitch(i, true);		// 방어구의 내구도가 변경되었음을 설정 

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(i, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltCharServer::DecreaseEquipDurFromDead(  )
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )	return;
	if( IsPC(GetCharUnique()) == false && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )return ;

	SI32 i = 0;
	// ItemPos, Percent
	cltUniqueRange MaxCount[] =
	{
		1,  70, 
			2,	25,
			3,	5 
	};
	SI32 MaxCount_Count = sizeof(MaxCount) / sizeof(cltUniqueRange);
	SI32 siSeletedMaxCount = GetUniqueWithRange(MaxCount, MaxCount_Count, 100);
	if( siSeletedMaxCount <= 0 )									return;
	if( siSeletedMaxCount > PERSONITEM_INV0 )						return;

	// 죽으면 20깎인다
	SI32 decdur = 20;
	SI32 ItemPos[PERSONITEM_INV0] = { -1, };

	SI32 siMaxCharEquipCount = 0;
	for( i=0; i< PERSONITEM_INV0; i++ )
	{
		bool	bHaveEquip = false;
		if( i == PERSONITEM_BAG || i == PERSONITEM_BAG_SUB )								continue;
		if( (i == PERSONITEM_WEAPON1 || i == PERSONITEM_WEAPON2) && (i != ActiveWeapon) )	continue;

		if( GetSummonParentID() )
		{
			SI32 siCharID = GetSummonParentID();
			cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siCharID );
			if( pclchar )
			{
				SI32 siSummonIndex = GetSummonIndex();
				cltItem* pclitem = pclchar->pclCI->clSummonInfo.GetSummonEquipItem( GetSummonIndex(), i );
				if( pclitem && pclitem->siUnique > 0 )				bHaveEquip = true;
			}
		}
		else
		{
			if( pclCI->clCharItem.clItem[i].siUnique > 0 )		bHaveEquip = true;
		}

		if( bHaveEquip )
		{
			ItemPos[siMaxCharEquipCount] = i;
			siMaxCharEquipCount++;
		}
	}
	if( siSeletedMaxCount > siMaxCharEquipCount )		siSeletedMaxCount = siMaxCharEquipCount;

	SI32 Count = siMaxCharEquipCount;

	for( i=0 ; i< siSeletedMaxCount; i++ )
	{
		SI32 siSelItemPos = rand() % Count;

		if( GetSummonParentID() )
		{	
			SI32 siCharID = GetSummonParentID();
			cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siCharID );
			if(pclchar)
			{
				pclchar->DecreaseSummonEquipDur( GetSummonIndex(), ItemPos[siSelItemPos], decdur );	
			}
		}
		else
		{	
			DecreaseEquipDur(ItemPos[siSelItemPos], decdur);	

			if ( (PERSONITEM_WEAPON1 == ItemPos[siSelItemPos]) || (PERSONITEM_WEAPON2 == ItemPos[siSelItemPos]) )
			{
				UseItemReasonDurabilityZero( ItemPos[siSelItemPos] );
			}
		}

		if( Count > 0 )	{	ItemPos[siSelItemPos] = ItemPos[Count-1];	}
		Count--;

		if( Count <= 0 )			break;
	}
}

void cltCharServer::DecreaseWeaponDur( SI32 siEnemyID )
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )	return;
	if( IsPC(GetCharUnique()) == false && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )return ;

	cltCharServer* pclEnemychar = pclClient->pclCM->GetCharServer( siEnemyID );
	if( pclEnemychar == NULL )			return;

#ifdef _DEBUG
	static SI32 siTotal = 0, siSucc = 0;
	siTotal++;
#endif

	// 50% percent
	SI32 siRand = rand() % 1000;
	if( siRand > 50 )		return;

#ifdef _DEBUG
	siSucc++;
	NTCHARString64 kMsg;
	kMsg += "Total[";
	kMsg += SI32ToString(siTotal);
	kMsg += "] Succ[";
	kMsg += SI32ToString(siSucc);
	kMsg += "] Percent[";
	REAL32 fPercent = (REAL32)siSucc * 100.0f / (REAL32)siTotal;
	kMsg += REAL32ToString(fPercent);
	kMsg += "]\n";
	OutputDebugString( kMsg );
#endif

	//SI32 decdur = 2;
	SI32 siConstA	= (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Weapon_ConstA" );
	SI32 siConstB	= (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Weapon_ConstB" );
	SI32 siWasteMin = (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Weapon_Min" );
	SI32 siWasteMax = (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Weapon_Max" );

	if ( 0 >= siConstB )
	{
#ifdef _DEBUG
		MessageBox(NULL, TEXT("GV_Waste_Dur_Armour_ConstB value is Zero"), TEXT("Wrong Value"), MB_OK|MB_TOPMOST);
#endif
		return;
	}

	SI16 siAttackerLevel = pclCI->clIP.GetLevel();
	SI16 siDefenderLevel = pclEnemychar->pclCI->clIP.GetLevel();
	SI32 decdur = siConstA + ( max(0, (siDefenderLevel - siAttackerLevel) / siConstB) );
	decdur = max( decdur, siWasteMin );
	decdur = min( decdur, siWasteMax );

	if( GetSummonParentID() )
	{
		SI32 siCharID = GetSummonParentID();
		cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siCharID );
		if(pclchar)
		{
			siSummonIndex = GetSummonIndex();
			pclchar->DecreaseSummonEquipDur( siSummonIndex, ActiveWeapon, decdur );
		}
	}
	else
	{
		DecreaseEquipDur(ActiveWeapon, decdur);

		UseItemReasonDurabilityZero( ActiveWeapon );
	}
}

void cltCharServer::DecreaseSummonEquipDur(SI32 siSummonIndex, SI32 itempos, SI32 durwaste)
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )	return;
	if(siSummonIndex < 0 || siSummonIndex >= MAX_SUMMON_OWN )		return;
	if(itempos < 0 || itempos >= MAX_SHAPE_ITEM)					return;
	if(durwaste < 0)												return;

	cltItem* pclitem = pclCI->clSummonInfo.GetSummonEquipItem( siSummonIndex, itempos );
	if( pclitem == NULL || pclitem->siUnique <= 0 )					return;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
	if( ref >0 )	{	durwaste += pclClient->pclItemManager->pclItemInfo[ref]->siDecDur;			}	

	SI32 decdur = durwaste;
	pclitem->DecreaseDurability(pclClient->pclItemManager, decdur);
	SetSummonWasteDurUpdateSwitch(siSummonIndex, itempos, true);

	cltGameMsgResponse_SummonChangeItem clSummonChangItem( siSummonIndex, itempos, pclitem );
	cltMsg clSummonMsg( GAMEMSG_RESPONSE_SUMMONCHANGEITEM, sizeof(clSummonChangItem), (BYTE*)&clSummonChangItem );
	SendNetMsg( (sPacketHeader*)&clSummonMsg );
}

void cltCharServer::DecreaseArmourDur( SI32 siEnemyID )
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )	return;
	// 사용자도 아니고 소환수도 아니면, 실패. 
	if( IsPC(GetCharUnique()) == false && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )return ;

	cltCharServer* pclEnemychar = pclClient->pclCM->GetCharServer( siEnemyID );
	if( pclEnemychar == NULL )			return;

	// 20%
	SI32 siRand = rand() % 1000;
	if( siRand > 200 )										return;

	// ItemPos, Percent
	cltUniqueRange ItemPos[] =
	{
		PERSONITEM_HELMET,  16, 
			PERSONITEM_HAT,		6, 
			PERSONITEM_ARMOUR,  16, 
			PERSONITEM_DRESS,	6, 
			PERSONITEM_BELT,	13,
			PERSONITEM_SHOES,	13, 
			PERSONITEM_MANTLE,	10,
			PERSONITEM_NECK,	6,
			PERSONITEM_RING1,	7,
			PERSONITEM_RING2,	7,
	};
	SI32 ItemPos_Count = sizeof(ItemPos) / sizeof(cltUniqueRange);
	SI32 siSeletedItemPos = GetUniqueWithRange(ItemPos, ItemPos_Count, 100);

	//	SI32 decdur = 2;
	SI32 siConstA	= (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Armour_ConstA" );
	SI32 siConstB	= (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Armour_ConstB" );
	SI32 siWasteMin = (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Armour_Min" );
	SI32 siWasteMax = (SI32)pclClient->GetGlobalValue( "GV_Waste_Dur_Armour_Max" );

	if ( 0 >= siConstB )
	{
#ifdef _DEBUG
		MessageBox(NULL, TEXT("GV_Waste_Dur_Armour_ConstB value is Zero"), TEXT("Wrong Value"), MB_OK|MB_TOPMOST);
#endif
		return;
	}

	SI16 siAttackerLevel = pclEnemychar->pclCI->clIP.GetLevel();
	SI16 siDefenderLevel = pclCI->clIP.GetLevel();
	SI32 decdur = siConstA + ( max(0, (siAttackerLevel - siDefenderLevel) / siConstB) );
	decdur = max( decdur, siWasteMin );
	decdur = min( decdur, siWasteMax );

	if( GetSummonParentID() )
	{
		SI32 siCharID = GetSummonParentID();
		cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siCharID );
		if(pclchar)
		{
			siSummonIndex = GetSummonIndex();
			pclchar->DecreaseSummonEquipDur( siSummonIndex, siSeletedItemPos, decdur );
		}
	}
	else
	{
		DecreaseEquipDur(siSeletedItemPos, decdur);
	}
}

void cltCharServer::DecreaseEquipDur(SI32 itempos, SI32 durwaste)
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )	return;
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)		return;
	if(durwaste < 0)										return;

	cltItem* pclitem = &pclCI->clCharItem.clItem[itempos];
	if( pclitem == NULL || pclitem->siUnique <= 0 )			return;

	SI32 returnval = 0;
	if(CheckEquipCondition(pclClient->pclItemManager, itempos, pclitem, &returnval) == false)	return;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
	if( ref >0 )	{	durwaste += pclClient->pclItemManager->pclItemInfo[ref]->siDecDur;			}	

	SI32 decdur = durwaste;
	pclitem->DecreaseDurability(pclClient->pclItemManager, decdur);
	SetWasteDurUpdateSwitch(itempos, true);

	cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
	SendNetMsg((sPacketHeader*)&clMsg);
}

void cltCharServer::UseItemReasonDurabilityZero( SI32 siItemPos )
{
	if ( false == pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		return;
	}

	if ( (0 > siItemPos) || (MAX_ITEM_PER_PERSON <= siItemPos) )
	{
		return;
	}

	cltItem* pclitem = &pclCI->clCharItem.clItem[siItemPos];
	if ( (NULL == pclitem) || (0 >= pclitem->siUnique) )
	{
		return;
	}

	if ( 0 < pclitem->GetDurability() )
	{
		return;
	}

	// 내구도 0에 의한 파괴는 아래 4가지 타입만 가능하다 그외 타입이 들어오면 리턴
	switch ( pclitem->GetType(pclClient->pclItemManager) )
	{
	case ITEMTYPE_IRON:
	case ITEMTYPE_FISHINGROD:
	case ITEMTYPE_DISH:
	case ITEMTYPE_PAPER:
		{
		}
		break;

	default:
		{
			return;
		}
	}

	sDBRequest_UseItem useitem( GetCharID(), pclCI->GetPersonID(), siItemPos, pclitem, USEITEM_REASON_DURABILITY_ZERO );
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&useitem);

}


// 무기의  내구도를 감소시킨다. 
/*
void cltCharServer::DecreaseWeaponDur()
{
	// 사용자도 아니고 소환수도 아니면, 실패. 
	if( IsPC(GetCharUnique()) == false
		&& clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )return ;

	// 공격자가 사용자인 경우 무기의 내구도를 떨어뜨린다. 
	// 5%의 확률로 내구도를 떨군다. 
	if(rand() % 14)return ;

	cltItem* pclitem = NULL;
	SI32 parentid = GetSummonParentID();
	// 소환수이면, 

	cltCharServer* pclchar = NULL;

	SI32 decdur = 2;

	if(parentid)
	{
		SI32 siSummonIndex = GetSummonIndex();
		if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
		{
#ifdef _DEBUG
			MsgBox(TEXT("소환수 인덱스 오류"), TEXT("잘못된 소환수 인덱스 %d"), GetSummonIndex());
#endif
			return;
		}

		// 대상 캐릭터를 정한다. 
		pclchar = (cltCharServer*)(pclCM->CR[parentid]);
		pclitem = pclchar->pclCI->clSummonInfo.GetSummonItem( siSummonIndex, ActiveWeapon );
	}
	else
	{
		// 대상 캐릭터를 정한다. 
		pclchar = this;
		pclitem = &pclchar->pclCI->clCharItem.clItem[ActiveWeapon];
	}



	if ( (NULL == pclitem) || (pclitem->siUnique <= 0) )
	{
		return ;
	}

	//pclchar->DecreaseEquipDur(ActiveWeapon, decdur);

}
*/
// 각 장비의 내구도를 감소시킨다. 
/*
void cltCharServer::DecreaseEquipDur(SI32 itempos, SI32 durwaste)
{
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)		return;

	cltServer* pclserver = (cltServer*)pclClient;

	cltItem* pclitem = &pclCI->clCharItem.clItem[itempos];
	if(pclitem->siUnique)
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
		if(ref >0)
		{
			durwaste += pclClient->pclItemManager->pclItemInfo[ref]->siDecDur;				// 방어구에 따라 내구도 감소가 커지는 것이 있다. 
		}

		SI32 decdur = durwaste;

		// 자격이 되지 않는 장비의 사용사 3배의 내구도 감소. 
		SI32 returnval = 0;
		if(CheckEquipCondition(pclClient->pclItemManager, itempos, pclitem, &returnval) == false)
		{
			decdur *= 3;
		}
		pclitem->DecreaseDurability(pclserver->pclItemManager, decdur);
		SetWasteDurUpdateSwitch(itempos, true);		// 방어구의 내구도가 변경되었음을 설정 

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		SI32 option = 0;
		if(pclitem->siUnique)			{	option = PERSONITEMINFO_OPTION_NONE;	}
		else							{	option = PERSONITEMINFO_OPTION_WASTE;	}
		cltGameMsgResponse_PersonItemInfo clinfo(itempos, option, 0, 0, pclitem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		SendNetMsg((sPacketHeader*)&clMsg);

	}

}
*/
// 방구의 내구도를 감소시킨다. 
/*
void cltCharServer::DecreaseArmourDur()
{
	// 사용자도 아니고 소환수도 아니면, 실패. 
	if( IsPC(GetCharUnique()) == false
		&& clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )return ;

	if(rand() % 3 )return ;

	SI32 randpos[] = {	PERSONITEM_ARMOUR,  4, 
		PERSONITEM_DRESS,	4, 
		PERSONITEM_HELMET,  4, 
		PERSONITEM_HAT,		4, 
		PERSONITEM_SHOES,	8, 
		PERSONITEM_BELT,	4,
		PERSONITEM_RING1,	8,
		PERSONITEM_RING2,	8,
		PERSONITEM_NECK,	4,
		PERSONITEM_MANTLE,	4,
		PERSONITEM_BAG,		4};	

	SI32 tempindex	= (rand() % 11);
	SI32 itempos	= randpos[ tempindex * 2];
	SI32 durwaste	= (rand() % randpos[tempindex * 2 + 1] ) ;

	cltItem* pclitem = NULL;
	SI32 parentid = GetSummonParentID();

	SI32 index = 0;
	cltCharServer* pclchar = NULL;

	// 소환수이면, 
	if(parentid)
	{
		// 대상 캐릭터를 정한다. 
		pclchar = (cltCharServer*)(pclCM->CR[parentid]);

		switch(GetSummonIndex())
		{
		case 0:	index = PERSONITEM_SUMMON1EQUIP0;	break;
		case 1:	index = PERSONITEM_SUMMON2EQUIP0;	break;
		default:
			MsgBox(TEXT("v222dd83jf"), TEXT("vdfe83:%d"), GetSummonIndex());
			return ;
		}

		//durwaste *= 1.3 ;
	}
	else
	{
		// 대상 캐릭터를 정한다. 
		pclchar = this;
	}


	// 각 위치의 내구도를 감소시킨다. 
	//pclchar->DecreaseEquipDur(index + itempos, durwaste);

}
*/
// InputItem시 예약된 자리를 정리한다. 
void cltCharServer::InputItemReserveAction()
{
	SI32 i;

	for(i = 0;i < MAX_ITEM_PER_PERSON;i++)
	{
		if(clSimpleVary.dwItemInputReserveClock[i] == 0)continue;
		if(TABS(pclClient->CurrentClock - clSimpleVary.dwItemInputReserveClock[i]) > 10000)
		{
			clSimpleVary.dwItemInputReserveClock[i] = 0;
		}
	}
}
bool cltCharServer::CheckItemForEmptyAnyItemSlotByHiddenQuesst(SI32 invmode, cltItem* pclitem,cltItem* pclitem2)
{
	bool	ret		= false	;
	bool	ret2	= false	;

	bool	LastRetuenValue	= false	;

	cltItem clToItem;
	clToItem.Init();
	SI16 InvPos = -1;
	SI32 rtnvalue;

	cltItem clToItem2	;
	clToItem2.Init()	;
	SI16 InvPos2 = -1	;
	SI32 rtnvalue2		;
	// 중첩될수 있는지 인벤토리를 확인해 본다.
	ret = pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_ALL, pclitem, &clSimpleVary.dwItemInputReserveClock[0], &InvPos, &clToItem, &rtnvalue);
	ret2 = pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_ALL, pclitem2, &clSimpleVary.dwItemInputReserveClock[0], &InvPos2, &clToItem2, &rtnvalue2);
	// 중첩될수 있는 공간이 없으면 빈공간이 2개가 있어야 한다.
	if ( ret == false && ret2 ==false )
	{
		if (IsBlankInv(2,INVMODE_ALL))	
		{
			LastRetuenValue = true;
		}
	}
	// 둘중 하나라도 중첩될수 있다면 빈공간은 1개만 있으면 된다.
	if ( ret == false && ret2 ==true )
	{
		SI32 siItempos = pclCI->clCharItem.FindItemInv(pclitem2->siUnique)	;
		if ( siItempos > 0 )// 겹칠수 있는 아이템이 있으면 
		{
			if( IsBlankInv(	1,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}
		}
		else
		{
			if( IsBlankInv(	2,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}
		}
	}
	else if ( ret == true && ret2 ==false )
	{
		SI32 siItempos = pclCI->clCharItem.FindItemInv(pclitem->siUnique)	;
		if ( siItempos > 0 )// 겹칠수 있는 아이템이 있으면 
		{
			if( IsBlankInv(	1,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}
		}
		else
		{
			if( IsBlankInv(	2,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}
		}
	}
	else if( ret == true && ret2 ==true )
	{
		// 둘다 중첩 가능 하면 아이템들이 있는 지 인벤에서 확인 한다.
		SI32 siItempos1 = pclCI->clCharItem.FindItemInv(pclitem->siUnique)	;
		SI32 siItempos2 = pclCI->clCharItem.FindItemInv(pclitem2->siUnique)	;
		if ( siItempos1  < 0 && siItempos2 > 0 ) 
		{
			if( IsBlankInv(	1,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}

		}
		else if ( siItempos1  > 0 && siItempos2 < 0 ) 
		{
			if( IsBlankInv(	1,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}

		}
		else if ( siItempos1  < 0 && siItempos2 < 0 ) 
		{
			if( IsBlankInv(	2,INVMODE_ALL )	)
			{
				LastRetuenValue = true;
			}
		}
		else
		{
			LastRetuenValue = true;
		}
	}
	return LastRetuenValue ;
}
bool cltCharServer::CanAddInv(SI32 invmode, cltItem* pclitem, SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue)
{
	// ALL모드이면 전체를 하나 하나 체크한다.
	if(invmode == INVMODE_ALL)
	{
		bool	ret;

		// 전체 중첩 체크 및 캐릭터 인벤의 새로운 위치 검색
		ret = pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_ALL, pclitem, &clSimpleVary.dwItemInputReserveClock[0], pitempos, pcltoitem, prtnvalue);
		if(ret)			return ret;
		// 말 인벤토리 새로운 위치 검색 (중첩위치는 위에서 검색되었음.)
		if(HaveHorse() == true)
		{
			ret = pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_HORSE, pclitem, &clSimpleVary.dwItemInputReserveClock[0], pitempos, pcltoitem, prtnvalue);
			if(ret)			return ret;
		}
		// 가방1 새로운 위치 검색 (중첩위치는 위에서 검색되었음.)
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique > 0)
		{
			ret = pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG, pclitem, &clSimpleVary.dwItemInputReserveClock[0], pitempos, pcltoitem, prtnvalue);
			if(ret)			return ret;
		}
		// 가방2 새로운 위치 검색 (중첩위치는 위에서 검색되었음.)
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique > 0)
		{
			ret = pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG_SUB, pclitem, &clSimpleVary.dwItemInputReserveClock[0], pitempos, pcltoitem, prtnvalue);
			if(ret)			return ret;
		}

		// INVMODE_ALL 모드는 여기서 끝낸다.
		return ret;
	}

	return pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, invmode, pclitem, &clSimpleVary.dwItemInputReserveClock[0], pitempos, pcltoitem, prtnvalue);
}

bool cltCharServer::IsBlankInv(SI32 invnum,SI32 invmode)
{
	// ALL모드이면 전체를 하나 하나 체크한다.
	if(invmode == INVMODE_ALL)
	{
		bool	ret;

		// 사람 인벤토리 검색
		ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_PERSON);
		if(ret)			return ret;
		// 말 인벤토리 검색
		if(HaveHorse() == true)
		{
			ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_HORSE);
			if(ret)			return ret;
		}
		// 가방1 인벤토리 검색
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique > 0)
		{
			ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_BAG);
			if(ret)			return ret;
		}
		// 가방2 인벤토리 검색
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique > 0)
		{
			ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_BAG_SUB);
			if(ret)			return ret;
		}

		// INVMODE_ALL 모드는 여기서 끝낸다.
		return ret;
	}

	return pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager,invnum,invmode);
}

bool cltCharServer::CanAddInvManyItems(SI32 invmode, cltItem* pclitem, SI32* prtnvalue)
{
	// ALL모드이면 전체를 하나 하나 체크한다.
	if(invmode == INVMODE_ALL)
	{
		bool	ret;

		// 전체 중첩 체크 및 캐릭터 인벤의 새로운 위치 검색
		ret = pclCI->clCharItem.CanAddInvManyItems(pclClient->pclItemManager, INVMODE_ALL, pclitem, &clSimpleVary.dwItemInputReserveClock[0], prtnvalue);
		if(ret)			return ret;
		// 말 인벤토리 새로운 위치 검색 (중첩위치는 위에서 검색되었음.)
		if(HaveHorse() == true)
		{
			ret = pclCI->clCharItem.CanAddInvManyItems(pclClient->pclItemManager, INVMODE_HORSE, pclitem, &clSimpleVary.dwItemInputReserveClock[0], prtnvalue);
			if(ret)			return ret;
		}
		// 가방1 새로운 위치 검색 (중첩위치는 위에서 검색되었음.)
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique > 0)
		{
			ret = pclCI->clCharItem.CanAddInvManyItems(pclClient->pclItemManager, INVMODE_BAG, pclitem, &clSimpleVary.dwItemInputReserveClock[0], prtnvalue);
			if(ret)			return ret;
		}
		// 가방2 새로운 위치 검색 (중첩위치는 위에서 검색되었음.)
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique > 0)
		{
			ret = pclCI->clCharItem.CanAddInvManyItems(pclClient->pclItemManager, INVMODE_BAG_SUB, pclitem, &clSimpleVary.dwItemInputReserveClock[0], prtnvalue);
			if(ret)			return ret;
		}

		// INVMODE_ALL 모드는 여기서 끝낸다.
		return ret;
	}
	return pclCI->clCharItem.CanAddInvManyItems(pclClient->pclItemManager, invmode, pclitem, &clSimpleVary.dwItemInputReserveClock[0], prtnvalue);
}


void cltCharServer::SetRoulette(bool bcheck)
{
	if( bcheck)
	{
		bRouletteStart = true;
		dwRouletteStartClock = pclClient->CurrentClock;
		dwRouletteDelayClock = 30000;
	}
	else
	{
		bRouletteStart = false;
		m_siBettingCount = 0;
		m_siResultCount = 0;
		dwRouletteStartClock = 0;
		dwRouletteDelayClock = 0;

		SI32 x = 0, y = 0;
		for( y = 0 ; y< 5; ++y )
		{
			m_siRandomValue[y] = 0;	// 선택될 랜덤값 0

			for( x = 0; x < 5; ++x )
			{
				m_stRouletteTable[y][x].m_bUserSelect = false;
				m_stRouletteTable[y][x].m_bCpuSelect  = false;
				m_stRouletteTable[y][x].m_bResult     = false;

				m_stRouletteTable[y][x].m_siX = 0;
				m_stRouletteTable[y][x].m_siY = 0;
				m_stRouletteTable[y][x].m_siValue = y*5 + (x + 1);
			}
		}
	}
}

void cltCharServer::CheckRoulette()
{
	SI32 x = 0, y = 0;
	for( y = 0 ; y< 5; ++y )
	{
		for( x = 0; x < 5; ++x )
		{
			if( m_stRouletteTable[y][x].m_bUserSelect == true && m_stRouletteTable[y][x].m_bCpuSelect)
			{
				m_stRouletteTable[y][x].m_bResult = true;
				m_siResultCount++;
			}
		}
	}
}

bool cltCharServer::CheckLottoRoulette()
{
	SI32 x = 0, y = 0;
	bool bResult = false;

	// 가로 체크해주고
	for( y = 0 ; y< 5; ++y )
	{
		if( m_stRouletteTable[y][0].m_bResult != true )
			continue;

		for( x = 1; x < 5; ++x )
		{
			if( m_stRouletteTable[y][x].m_bResult != true )
			{
				bResult = false;
				break;
			}

			bResult = true;
		}
	}
	// 세로 채크해주고
	for( x = 0 ; x< 5; ++x )
	{
		if( m_stRouletteTable[0][x].m_bResult != true )
			continue;

		for( y = 1; y < 5; ++y )
		{
			if( m_stRouletteTable[y][x].m_bResult != true )
			{
				bResult = false;
				break;
			}

			bResult = true;
		}
	}
	// 대각선 하자
	for( y = 0; y < 5; ++y )
	{
		if( m_stRouletteTable[y][y].m_bResult != true )
		{
			bResult = false;
			break;
		}
		bResult = true;
	}
	for( y = 4; y >= 0; --y )
	{
		if( m_stRouletteTable[y][y].m_bResult != true )
		{
			bResult = false;
			break;
		}
		bResult = true;
	}

	return bResult;
}

void cltCharServer::SetInputItemReserve(SI32 itempos, DWORD data)
{
	if( itempos < 0 || itempos >= MAX_ITEM_PER_PERSON )			return;

	clSimpleVary.dwItemInputReserveClock[itempos] = data;
}

SI32 cltCharServer::GetMakeItemNum(SI32 needskillunique, SI32 needskilllevel, SI32 makeitemunique, SI32 makeitemnum)
{
	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	SI64 atb = pclClient->pclItemManager->GetItemAtb(makeitemunique);

	cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(makeitemunique); 

	if (ref > 0)
	{
		// 생산 성공 확률을 구한다. 
		if(needskillunique > 0 && needskilllevel)
		{
#ifdef _SAFE_MEMORY
			SI32 skilllevel	= pclCI->clCharSkill.GetTotalSkillLevel(needskillunique, 0, 0, &pclCI->clCharItem.clItem[0]);
#else
			SI32 skilllevel	= pclCI->clCharSkill.GetTotalSkillLevel(needskillunique, 0, 0, pclCI->clCharItem.clItem);
#endif
			float makepbt	= pclClient->pclItemManager->pclItemInfo[ref]->GetTotalMakePbt(skilllevel);
			SI32  simakepbt	= (SI32)(makepbt * 10);
			SI32  makenum	= 0;

			if(simakepbt >= 1000)
			{

				// obj 아이템이 아니면 성공 가능성이 높을수록 더 많은 물건이 만들어진다.  
				if((atb & ITEMATB_OBJECT) == 0)
				{
					makeitemnum = (makeitemnum * simakepbt) / 1000;
				}

			}
			else
			{
				for(SI32 i = 0;i < makeitemnum;i++)
				{
					if(rand() % 1000 <= simakepbt)
					{
						makenum++;
					}
				}

				makeitemnum	= makenum;
			}
		}

	}
	else
		makeitemnum = 0;

	return makeitemnum;
}


SI32 cltCharServer::GetMakeCrystalItemNum(SI32 needskillunique, SI32 needskilllevel, SI32 makeitemunique, SI32 makeitemnum)
{
	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);

	if (ref > 0)
	{
		// 생산 성공 확률을 구한다. 
		if(needskillunique > 0 && needskilllevel)
		{
#ifdef _SAFE_MEMORY
			SI32 skilllevel	= pclCI->clCharSkill.GetTotalSkillLevel(needskillunique, 0, 0, &pclCI->clCharItem.clItem[0]);
#else
			SI32 skilllevel	= pclCI->clCharSkill.GetTotalSkillLevel(needskillunique, 0, 0, pclCI->clCharItem.clItem);
#endif
			SI32 temMinnum = 0;
			SI32 temnum = 0;
			SI32 AddPer = 0;

			//makeitemnum =((makeitemnum*140)/100);
			if(skilllevel >= 30 && skilllevel < 45)
			{
				temMinnum = ((makeitemnum*1200)/1000); // 최소값.

				AddPer = (skilllevel - 30) * 13; 
				temnum = ((makeitemnum*(1200+AddPer))/1000); // 기술 레벨에 따른 맥스값.

				makeitemnum = max(temMinnum ,temnum );
			}
			else if(skilllevel >= 45 && skilllevel < 60)
			{
				temMinnum = ((makeitemnum*1400)/1000); // 최소값.

				AddPer = (skilllevel - 45) * 13; 
				temnum = ((makeitemnum*(1400+AddPer))/1000); // 기술 레벨에 따른 맥스값.

				makeitemnum = max(temMinnum ,temnum );			
			}
			else if(skilllevel >= 60 && skilllevel < 75)
			{
				temMinnum = ((makeitemnum*1600)/1000); // 최소값.

				AddPer = (skilllevel - 60) * 13; 
				temnum = ((makeitemnum*(1600+AddPer))/1000); // 기술 레벨에 따른 맥스값.

				makeitemnum = max(temMinnum ,temnum );
			}
			else if(skilllevel >= 75 && skilllevel < 100)
			{
				temMinnum = ((makeitemnum*1800)/1000); // 최소값.

				AddPer = (skilllevel - 75) * 13; 
				temnum = ((makeitemnum*(1800+AddPer))/1000); // 기술 레벨에 따른 맥스값.

				makeitemnum = max(temMinnum ,temnum );
			}
			else if(skilllevel ==  100)
			{
				makeitemnum =((makeitemnum*2000)/1000);
			}
		}
	}
	else
		makeitemnum = 0;

	return makeitemnum;
}
// 개인상점에 자신의 특정 아이템이 등록되었는지..
bool cltCharServer::IsExistItemOnOpeningPersonShop(SI32 siItemPos)
{
	if( bPersonalShopSwitch == true && clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( clShopData.m_siSelectedInventoryPos[i] == siItemPos )
			{
				if( clShopData.m_clItem[i].siUnique > 0  )
					return true;
			}
		}

		// 귀속전용 개인상점
		if( clShopData.m_siSelectedSpacialInventoryPos == siItemPos	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( clShopData.m_clSpacialItem.siUnique > 0 )
			{
				return true;
			}
		}
	}

	return false;
}
