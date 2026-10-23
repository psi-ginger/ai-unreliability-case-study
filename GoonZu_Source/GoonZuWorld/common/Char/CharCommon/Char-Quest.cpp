#include "..\Common\CommonHeader.h"
#include "..\Common\Char\CharServer\Char-Server.h"
#include "..\Common\Char\KindInfo\KindInfo.h"
#include "..\..\Client\Client.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\CommonLogic\MsgType-System.h"

// 채팅 관련. 
#include "..\Client\Interface\DrawChat\DrawChatSurface.h"
#include "..\Client\Interface\DrawChat\DrawChat.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\Server\GameDBProcess\GameDBProcess.h"

#include "../CommonLogic//Feast/Feast.h"


//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;


// 아직 완료되지 않은 가장 낮은 퀘스트의 비트를 찾는다. 
SI32 cltCharCommon::GetMinBit(SI32 type)
{
	if( type < 0 || type >= MAX_QUEST_TYPE)		return -1;

	SI32 i;
	SI32 minbit = -1;
	SI32 minunique = 1000000;

	SI32 tempdata = pclCI->clQuestInfo.clUnit[type].siQuestState;
	for(i = 0;i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		if((tempdata & 0x00000001) == 0)	// 맨 하위 비트가 0인 경우에, 
		{
			// 해당 비트의 유니크를 찾는다. 
			SI32 questunique = pclClient->pclQuestManager->GetUniqueFromBit(type, i);
			if(questunique)
			{
				// 유니크가 최소 유니크이면, 
				if(questunique < minunique)
				{
					minunique	= questunique;
					minbit		= i;
				}
			}
		}

		tempdata = (tempdata >> 1);
		
	}

	return minbit;
}

// 특정 유니크의 퀘스트를 완료했는지 확인한다. 
bool cltCharCommon::ClearQuest(SI32 questunique)
{
	//---------------------------------
	// 퀘스트의 타입과 비트를 얻어온다.
	//---------------------------------
	cltQuestInfo* pclinfo = pclClient->pclQuestManager->GetQuestInfo(questunique);
	if(pclinfo == NULL)return false;


	SI32 questtype	= pclinfo->siType;
	if(questtype < 0 || questtype >= MAX_QUEST_TYPE)		return false;
	SI32 bit		= pclClient->pclQuestManager->GetBitFromUnique(questtype, questunique);

	//--------------------------------
	// 사용자의 해당 퀘스트 상태를 확인한다
	//--------------------------------
	cltPersonQuestUnitInfo* pclquestinfo = &pclCI->clQuestInfo.clUnit[questtype];
	if(pclquestinfo == NULL)			return false;
	
	if(pclquestinfo->GetBit(bit))
	{
		return true;
	}
	return false;
}

// 임무의 완료 조건을 충족시키는가?
bool cltCharCommon::IsClearCondition(SI32 questtype, cltQuestClearConditionInfo* pclcond)
{
	SI32 i;
	
	if(questtype < 0 || questtype >= MAX_QUEST_TYPE)	return false;
	if(pclcond == NULL)return false;

	// 무조건 그냥 클리어 되는 퀘스트 이다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_NONE )	
	{
		return true;
	}

	// Status 보너스 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_BONUSZERO)
	{
		if(pclCI->clIP.GetBonus()	== 0)	// 보너스 수치가 같아야 조건 충족. 
		{
			return true;
		}
	}
	// 모든 소환수의 Status 보너스 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_SUMMONBONUS)
	{
		for(i = 0;i < MAX_SUMMON_OWN;i++)
		{
			if(pclCI->clSummonInfo.clSummon[i].siKind && pclCI->clSummonInfo.clSummon[i].clIP.GetBonus() )	// 보너스 수치가 같아야 조건 충족. 
			{
				return false;
			}
		}

		return true;
	}
	// 기술 점수 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_SKILLBONUSZERO)
	{
		if(pclCI->clIP.GetSkillBonus()	== 0)	// 보너스 수치가 같아야 조건 충족. 
		{
			return true;
		}
	}
	
	// 돈을 모아와야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_MONEY)
	{
		if(pclCI->clIP.GetMoney()	>= pclcond->siPara2)	
		{
			return true;
		}
	}
	// 돈을 전장에 입금해야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_BANK)
	{
		if(pclCI->clBank.GetMoney() >= (GMONEY)pclcond->siPara2)	
		{
			return true;
		}
	}
	// 배고픔을 면해야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_HUNGRY)
	{
		 if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 > 0)
		 {
			 return true;
		 }
	}
	// 무기를 수리한다. WEAPON1, 2 전부 검사한다.
	if(pclcond->siFlag == QUEST_CLEAR_COND_REPAIRWEAPON)
	{
		if(pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].siUnique)	
		{
			if(pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].GetDurability() >= pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].GetMaxDurability(pclClient->pclItemManager))
			{
				return true;
			}
		}
		if(pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].siUnique)	
		{
			if(pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].GetDurability() >= pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].GetMaxDurability(pclClient->pclItemManager))
			{
				return true;
			}
		}
	}
	
	// 레벨이 얼마 이상어야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_LEVEL)
	{
		if(pclCI->clIP.GetLevel() >= pclcond->siPara2)	 
		{
			return true;
		}
	}
	// 명성레벨이 얼마 이상어야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_FAMELEVEL)
	{
		if(pclCI->clIP.siFameLevel >= pclcond->siPara2)	 
		{
			return true;
		}
	}
	// 체력이 완전히 회복되어야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_FULLLIFE)
	{
		if(pclCI->clIP.GetLife() >= clPB.GetMaxLife())	 
		{
			return true;
		}
	}
	// 주식을 보유해야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_STOCK)
	{
		if(pclCI->clStock.clInfo.HaveStock() == true)
		{
			return true;
		}
	}
	// 친구를 두명 등록해야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_MAKEFRIEND)
	{
		if( pclFriendInfo && pclFriendInfo->GetFriendNum() >= pclcond->siPara2 )
		{
			return true;
		}
	}
	
	// 말을 가져야 한다. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_HORSE)
	{
		if(pclCI->clHorseInfo.HaveHorse())
		{
			return true;
		}
	}
	// 소환수 데려오기 
	if(pclcond->siFlag == QUEST_CLEAR_COND_SUMMON)
	{
		if(pclCI->clSummonInfo.clSummon[0].siKind)
		{
			return true;
		}
	}
	// 소환수 활성화
	if(pclcond->siFlag == QUEST_CLEAR_COND_SUMMONACTIVE)
	{
		for ( SI32 siIndex=0; siIndex<MAX_SUMMON_OWN; ++siIndex )
		{
			SI32 siSummonKind	= pclCI->clSummonInfo.clSummon[siIndex].siKind;
			SI32 siSummonStatus	= pclCI->clSummonInfo.clSummon[siIndex].siStatus;

			if ( (0 < siSummonKind) && (MAX_KIND_NUMBER > siSummonKind) && (SUMMON_STATUS_TAKEOUT == siSummonStatus) )
			{
				return true;
			}
		}
	}

	if(pclcond->siFlag == QUEST_CLEAR_COND_SUMMONDIE )
	{
		for ( SI32 siIndex=0; siIndex<MAX_SUMMON_OWN; ++siIndex )
		{
			SI32 siSummonKind	= pclCI->clSummonInfo.clSummon[siIndex].siKind;
			SI32 siSummonStatus	= pclCI->clSummonInfo.clSummon[siIndex].siStatus;

			if ( (0 < siSummonKind) && (MAX_KIND_NUMBER > siSummonKind) && (SUMMON_STATUS_DEAD == siSummonStatus) )
			{
				return true;
			}
		}
	}

	// 소환수 2마리 키우기. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_SUMMON2)
	{
		if(pclCI->clSummonInfo.clSummon[0].siKind && pclCI->clSummonInfo.clSummon[1].siKind)
		{
			return true;
		}
	}

	// 소환수 일정 레벨 이상 진화 시키기
	if(pclcond->siFlag == QUEST_CLEAR_COND_SUMMONEVOLUTIONRANK)
	{
		for ( SI32 siIndex=0; siIndex<MAX_SUMMON_OWN; ++siIndex )
		{
			SI32 siSummonKind	= pclCI->clSummonInfo.clSummon[siIndex].siKind;
			SI32 siSummonStatus	= pclCI->clSummonInfo.clSummon[siIndex].siStatus;

			if ( (0 < siSummonKind) && (MAX_KIND_NUMBER > siSummonKind) && (SUMMON_STATUS_TAKEOUT == siSummonStatus) )
			{
				cltKindInfo* pclKindInfo = pclClient->pclKindInfoSet->pclKI[siSummonKind];
				// 내가 갖고 있는 소환수의 진화 등급을 확인해서 클리어 여부를 결정한다.
				if ( (pclKindInfo) && (pclKindInfo->siSummonRank >= pclcond->siPara1) )
				{
					return true;
				}
			}
		}
	}

	// 특정 기술의 레벨이 얼마 이상이어야 한다.
	if(pclcond->siFlag == QUEST_CLEAR_COND_SKILL)
	{
		SI32 skillunique = pclcond->siPara1;

#ifdef _SAFE_MEMORY
		if( pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclCI->clCharItem.clItem[0]) >=  pclcond->siPara2)
		
#else
		if( pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclCI->clCharItem.clItem) >=  pclcond->siPara2)
			
#endif
		{
			return true;
		}
	}

	// [지연] 특정 기술의 제조 경험치가 1 포인트 이상 되어야 한다.
	if(pclcond->siFlag == QUEST_CLEAR_COND_MAKESKILLEXPUP)
	{
		SI32 skillunique = pclcond->siPara1;

#ifdef _SAFE_MEMORY
		if( pclCI->clCharSkill.GetTotalSkillExp(skillunique) >= pclcond->siPara2 ) // 경험치가 1 증가했는지에 대한 조건도 추가.
#else
		if( pclCI->clCharSkill.GetTotalSkillExp(skillunique) >= pclcond->siPara2 )
#endif
		{
			return true;
		}
	}

	// 기본 뭇기사용술의 레벨이 얼마 이상이어야 한다.
	if(pclcond->siFlag == QUEST_CLEAR_COND_BASICWEAPONSKILL)
	{
#ifdef _SAFE_MEMORY
		if(pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESWORD1,	0, 0, &pclCI->clCharItem.clItem[0])	>=  pclcond->siPara2 
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESPEAR1,	0, 0, &pclCI->clCharItem.clItem[0])	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEAXE1,		0, 0, &pclCI->clCharItem.clItem[0])	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEBOW1,		0, 0, &pclCI->clCharItem.clItem[0])	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEGUN1,		0, 0, &pclCI->clCharItem.clItem[0])	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESTAFF1,	0, 0, &pclCI->clCharItem.clItem[0])	>=  pclcond->siPara2)
#else
		if(pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESWORD1,	0, 0, pclCI->clCharItem.clItem)	>=  pclcond->siPara2 
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESPEAR1,	0, 0, pclCI->clCharItem.clItem)	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEAXE1,		0, 0, pclCI->clCharItem.clItem)	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEBOW1,		0, 0, pclCI->clCharItem.clItem)	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEGUN1,		0, 0, pclCI->clCharItem.clItem)	>=  pclcond->siPara2
		|| pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESTAFF1,	0, 0, pclCI->clCharItem.clItem)	>=  pclcond->siPara2)
#endif
		{
			return true;
		}
	}

	///////////////
	// 특정 물품의 장착 확인. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_EQUIP)
	{

		SI32 itemunique = pclcond->siPara1;

		// 장비창중에 해당 물품이 있는지 확인한다. 
		if(pclCI->clCharItem.HaveInEquip(itemunique) == true)
		{
			return true;
		}
	}
	// 특정 장착창에 물품이 있는지 확인. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_EQUIPPOS)
	{
		// 장비창중에 해당 물품이 있는지 확인한다. 
		if( pclcond->siPara1 >= 0 && pclcond->siPara1 < MAX_ITEM_PER_PERSON &&
			pclCI->clCharItem.clItem[pclcond->siPara1].siUnique)
		{
			return true;
		}
	}
	
	// 가방에 물건 넣어보기. 
	if(pclcond->siFlag == QUEST_CLEAR_COND_BAG)
	{

		SI32 itemunique = pclcond->siPara1;

		// 장비창중에 해당 물품이 있는지 확인한다. 
		if(pclCI->clCharItem.IsBagHaveItem() == true)
		{
			return true;
		}
	}

	if(pclcond->siFlag == QUEST_CLEAR_COND_SETFATHER )
	{
		if ( pclCI->clIP.GetLevel() >= 20 || pclCI->clBI.clFatherInfo.clFather.siPersonID > 0 )
		{
			return true ;
		}
	}

	// 인사말 설정하기
	if( pclcond->siFlag == QUEST_CLEAR_COND_SETGREETING )
	{
		if( _tcslen( pclCI->clBI.szGreeting ) > 0 )
		{
			return true;
		}
	}

	// 길드 가입or개설(대기도 가능)
	if( pclcond->siFlag == QUEST_CLEAR_COND_APPLYGUILD )
	{
		if( pclCI->clBI.clGuildRank.siType > 0 )
		{
			return true;
		}
	}

	// 파티가 있다면
	if( pclcond->siFlag == QUEST_CLEAR_COND_MAKEPARTY )
	{
		if( siPartyIndex > 0)
		{
			return true;
		}
	}

	// 캐릭터 정보창을 여는 조건이라면, 
	// 기술창이 열리는 조건이라면, 
	// NPC에게 물건을 판매하는 조건이면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_OPENCHARSTATUS 
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENSKILL
	|| pclcond->siFlag == QUEST_CLEAR_COND_SELLNPC
	|| pclcond->siFlag == QUEST_CLEAR_COND_ITEMINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_NEWS
	|| pclcond->siFlag == QUEST_CLEAR_COND_GOMAP
	|| pclcond->siFlag == QUEST_CLEAR_COND_LAMPGOMAP		// 요술램프 마을 순회
	|| pclcond->siFlag == QUEST_CLEAR_COND_SELLMARKET
	|| pclcond->siFlag == QUEST_CLEAR_COND_SCROLL
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEWENNYOLDMAN2
	|| pclcond->siFlag == QUEST_CLEAR_COND_FEEDHORSE1
	|| pclcond->siFlag == QUEST_CLEAR_COND_FEEDHORSE2
	|| pclcond->siFlag == QUEST_CLEAR_COND_STRUCTUREOPEN
	|| pclcond->siFlag == QUEST_CLEAR_COND_ITEMINFODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_SELLSWORDNPC
	|| pclcond->siFlag == QUEST_CLEAR_COND_OTHERMINIHOME
	|| pclcond->siFlag == QUEST_CLEAR_COND_BUYFROMPBSTORE
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENPBSTORE
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENSTOCKMARKET
	|| pclcond->siFlag == QUEST_CLEAR_COND_LOOKPANSEO
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENVILLAGEBOARD
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENCITYHALL
	|| pclcond->siFlag == QUEST_CLEAR_COND_APPLYVILLAGE
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEFAN
	|| pclcond->siFlag == QUEST_CLEAR_COND_SENDITEMBYPOST
	|| pclcond->siFlag == QUEST_CLEAR_COND_REVIVESUMMON
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEWENNYOLDMAN
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENHUNT
	|| pclcond->siFlag == QUEST_CLEAR_COND_SKILLMASTER
	|| pclcond->siFlag == QUEST_CLEAR_COND_BROADCAST
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEWARPTICKET1
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEWARPTICKET2
	|| pclcond->siFlag == QUEST_CLEAR_COND_DAILYQUEST
	|| pclcond->siFlag == QUEST_CLEAR_COND_SPECIALQUEST
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONEVOLUTION
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONSCROLL
	|| pclcond->siFlag == QUEST_CLEAR_COND_ITEMMALL 
	|| pclcond->siFlag == QUEST_CLEAR_COND_GONPC
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKITEMINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENMAKEITEM
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKMAKEBUTTON
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENENCHANTITEM
	|| pclcond->siFlag == QUEST_CLEAR_COND_SETENCHANTITEM
	|| pclcond->siFlag == QUEST_CLEAR_COND_SETENCHANTELEMENT
	|| pclcond->siFlag == QUEST_CLEAR_COND_WARPTOPALACE
	|| pclcond->siFlag == QUEST_CLEAR_COND_BUYFROMPBSTORE
	|| pclcond->siFlag == QUEST_CLEAR_COND_PRIVATETRADE
	|| pclcond->siFlag == QUEST_CLEAR_COND_SELLFROMPBSTORE
	|| pclcond->siFlag == QUEST_CLEAR_COND_HUNTMAPINFODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_PARTYHUNTING
	|| pclcond->siFlag == QUEST_CLEAR_COND_MAGICINFODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENHOUSE
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENREALESTATEMARKET
	|| pclcond->siFlag == QUEST_CLEAR_COND_BLACKARMY
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENHORSEMARKET
	|| pclcond->siFlag == QUEST_CLEAR_COND_REVIVESUMMONBYDOLL
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENHOZODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENBYUNGZODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENGONGZODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENIZODLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENGOONZUDLG
	|| pclcond->siFlag == QUEST_CLEAR_COND_VOTEKING
	|| pclcond->siFlag == QUEST_CLEAR_COND_MAKEBOOK
	|| pclcond->siFlag == QUEST_CLEAR_COND_SEARCHMONSTERINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEWARPTICKET3
	|| pclcond->siFlag == QUEST_CLEAR_COND_USEMAGIC
	|| pclcond->siFlag == QUEST_CLEAR_COND_BUYTOPBSTORE
	|| pclcond->siFlag == QUEST_CLEAR_COND_VILLAGEDETAILINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_ENCHANTANYITEM
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENFRIEND
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENMASTER
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENMAKEREQUEST
	|| pclcond->siFlag == QUEST_CLEAR_COND_MAKEREQUESTSTART
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENNAMINGQUEST
	|| pclcond->siFlag == QUEST_CLEAR_COND_DISASSEMBLEITEM
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONCHANGESTATUS
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONATTACK
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONLEVELUP
	|| pclcond->siFlag == QUEST_CLEAR_COND_MAKEINTBOOK
	|| pclcond->siFlag == QUEST_CLEAR_COND_MAGICACTIVE
	|| pclcond->siFlag == QUEST_CLEAR_COND_GOHUNTMAP
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENGUILD
	|| pclcond->siFlag == QUEST_CLEAR_COND_LOOKPRISON
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENGOONZUDLG_NOTICE
	|| pclcond->siFlag == QUEST_CLEAR_COND_SORTRESIDENT
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKITEMPROPERTY
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENSKILLETC
	|| pclcond->siFlag == QUEST_CLEAR_COND_SITDOWN
	|| pclcond->siFlag == QUEST_CLEAR_COND_NPCINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENGUILDUSER	
	|| pclcond->siFlag == QUEST_CLEAR_COND_SEARCHMONSTERINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_SEARCHSTOCKINFO
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKSTOCKOFFER
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKCITYHALLFUND
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKGENERALMETTING
	|| pclcond->siFlag == QUEST_CLEAR_COND_CLICKPERSONALSHOP
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENITEMMALL
	|| pclcond->siFlag == QUEST_CLEAR_COND_ANGELLISTCLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_POLICESETCLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_SEARCHFARM
	|| pclcond->siFlag == QUEST_CLEAR_COND_LEASECLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENQUESTDIALOG
	|| pclcond->siFlag == QUEST_CLEAR_COND_TAKEPUPILS
	|| pclcond->siFlag == QUEST_CLEAR_COND_CHATTOBEGGINER
	|| pclcond->siFlag == QUEST_CLEAR_COND_MEMOCLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_MOVETOGROUPCLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_REQUESTMENTORSUCCESS
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENFATHERDIALOG
	|| pclcond->siFlag == QUEST_CLEAR_COND_ORDERTOMARKET
	|| pclcond->siFlag == QUEST_CLEAR_COND_OPENPOSTOFFICEDIALOG
	|| pclcond->siFlag == QUEST_CLEAR_COND_TEACHERSET
	|| pclcond->siFlag == QUEST_CLEAR_COND_SWORDSKILLCLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_PVPWIN
	|| pclcond->siFlag == QUEST_CLEAR_COND_PVPLEAGUERESERVE
	|| pclcond->siFlag == QUEST_CLEAR_COND_PVPLEAGUEWIN
	|| pclcond->siFlag == QUEST_CLEAR_COND_SETMYKEY
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONHOUSE_INPUT
	|| pclcond->siFlag == QUEST_CLEAR_COND_SUMMONHOUSE_OUTPUT
	|| pclcond->siFlag == QUEST_CLEAR_COND_MARKETCONDITION
	|| pclcond->siFlag == QUEST_CLEAR_COND_BUFFCLICK
	|| pclcond->siFlag == QUEST_CLEAR_COND_GONPC_INFORMATION
	|| pclcond->siFlag == QUEST_CLEAR_COND_MAKESKILLEXPUP
	|| pclcond->siFlag == QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS
	|| pclcond->siFlag == QUEST_CLEAR_COND_SEARCH_TOTALMAP
	) 
	{
		if(pclcond->siPara1 == pclCI->clQuestInfo.clUnit[questtype].siQuestPara1
		&& pclcond->siPara2 == pclCI->clQuestInfo.clUnit[questtype].siQuestPara2)
		{
			return true;
		}
		
		// 아이템몰 구매하기 퀘스트 중인데SBS회원이면,  
		if(pclcond->siFlag == QUEST_CLEAR_COND_ITEMMALL)
		{
			if(IsSBS())return true;
		}
		
		// 마을 주민 신청 퀘스트 중인데 이미 마을 주민이면, 
		if(pclcond->siFlag == QUEST_CLEAR_COND_APPLYVILLAGE)
		{
			if(pclCI->clBI.siHomeVillage)return true;
		}
		

		// 초보자사냥터 출입 퀘스트이고 출입제한 레벨이 걸려 있다면, 
		// 조건문 수정 - by LEEKH 2007.11.21
		if(pclcond->siFlag == QUEST_CLEAR_COND_GOMAP)
		{
			if( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == MAPINDEX_BEGINNER1 )
			{
				// 초보자 사냥터에 들어가는 퀘스트인데
				// 캐릭터의 레벨 때문에 사냥터에 못 들어가는 경우 그냥 PASS
				if( pclCI->clIP.GetLevel() > MAX_LEVEL_FOR_BEGINNERMAP )
				{
					return true;
				}
			}
		}

		// 호품 퀘스트인데 이미 한적이 있다면
		if(pclcond->siFlag == QUEST_CLEAR_COND_USEWENNYOLDMAN )
		{
			if ( pclCI->clBI.bWennyOldManUpgrade )
			{
				return true ;
			}
		}
		
		// 스승 맺기인데 이미 스승을 이미 맺었다면
		if(pclcond->siFlag == QUEST_CLEAR_COND_TEACHERSET )
		{
			if ( 0 < pclCI->clBI.clFatherInfo.clFather.GetPersonID() )
			{
				return true;
			}
		}
	}

	// 수렵장이라면..
	if( pclcond->siFlag == QUEST_CLEAR_COND_GOHUNTMAP )
	{
		if ( GetHuntVillageUnique() > 0 )
			return true;
	}
	
	// 말속도를 높이는 퀘스트 중이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_HORSESPEED)
	{
		// 현재 말의 속도를 구한다. 
		cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

		if(pclhorse)
		{
			SI32 curspeed= pclhorse->GetRealMoveSpeed();
			if(curspeed >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 인벤 아이템 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_ITEM)
	{
		SI32 itemunique = pclcond->siPara1;
		SI32 itemnum	= pclcond->siPara2;

		if(itemnum <= pclCI->clCharItem.GetItemNum(itemunique))
		{
			return true;
		}
	}

	if(pclcond->siFlag == QUEST_CLEAR_COND_ITEMCHECK)
	{
		SI32 itemunique = pclcond->siPara1;
		SI32 itemnum	= pclcond->siPara2;

		if(itemnum <= pclCI->clCharItem.GetItemNum(itemunique))
		{
			return true;
		}
	}

	// 몬스터를 일정수 이상 처치하라는 조건이면, 
	if ( (pclcond->siFlag == QUEST_CLEAR_COND_KILLMON)	
		|| (pclcond->siFlag	== QUEST_CLEAR_COND_PARTYKILLMON)		// 몬스터를 파티 맺고 처치하는 퀘스트라면	
		|| (pclcond->siFlag == QUEST_CLEAR_COND_TWOWAYKILLMON)		// 몬스터를 혼자 혹은 파티 맺고 처치하는 퀘스트라면
		|| (pclcond->siFlag == QUEST_CLEAR_COND_TWOWAYKILLMONBONUS)	// 몬스터를 혼자 혹은 파티 맺고 처치하는 퀘스트라면
		|| (pclcond->siFlag == QUEST_CLEAR_COND_MAPKILLMON)			// 해당 맵의 몬스터를 몇마리 이상 죽여야 하는 퀘스트라면
	)
	{
		// 퀘스트의 임무가 몬스터 처치이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 몬스터를 일정수 이상 때리는 조건이라면, 
	if(pclcond->siFlag	== QUEST_CLEAR_COND_HITMON)
	{
		// 퀘스트의 임무가 몬스터 때리기이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 때려야 할 수 이상을 때리면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 소환수가 몬스터를 일정수 이상 처치하라는 조건이면, 
	if(pclcond->siFlag	== QUEST_CLEAR_COND_SUMMONBATTLE)
	{
		// 퀘스트의 임무가 몬스터 처치이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}

	}

	// 특정 물고기를 일정수 이상 잡아야 하는 조건. 
	if( (pclcond->siFlag == QUEST_CLEAR_COND_FISH) || (pclcond->siFlag == QUEST_CLEAR_COND_FISHNOSKILL) )
	{
		// 퀘스트의 임무가 몬스터 처치이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}
	
	// 특정 곡물을  일정수 이상 수확해야 하는 조건. 
	if( (pclcond->siFlag == QUEST_CLEAR_COND_FARM) || (pclcond->siFlag == QUEST_CLEAR_COND_FARMNOSKILL) )
	{
		// 퀘스트의 임무가 몬스터 처치이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 특정 광물을 일정수 이상 채광해야 하는 조건.
	if( pclcond->siFlag == QUEST_CLEAR_COND_MINE || pclcond->siFlag == QUEST_CLEAR_COND_MINENOSKILL )
	{
		// 퀘스트의 임무가 몬스터 처치이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 사냥으로 아이템을 획득해야 하는 조건
	if ( pclcond->siFlag == QUEST_CLEAR_COND_HUNT_ITEM )
	{
		// 퀘스트의 임무가 몬스터 처치이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	
	// 물품을 일정수 이상 만들라는  조건이면, (명품,호품,신기 추가)
	if ( (pclcond->siFlag	== QUEST_CLEAR_COND_MAKEITEM) 
		|| (pclcond->siFlag == QUEST_CLEAR_COND_MAKE_RAREITEM) 
	)
	{
		// 퀘스트의 임무가 물품 만들기 이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 만든 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 튜토리얼로 물품을 일정수 이상 만들라는  조건이면, 
	if (pclcond->siFlag	== QUEST_CLEAR_COND_TUTORIALMAKEITEM)
	{
		// 퀘스트의 임무가 물품 만들기 이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 만든 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 물품을 일정수 이상 인첸트하라는  조건이면, 
	if ( (pclcond->siFlag == QUEST_CLEAR_COND_ENCHANTITEM)
		|| (pclcond->siFlag == QUEST_CLEAR_COND_ENCHANTEQUIPLEVELWEAPON)
	)
	{
		// 퀘스트의 임무가  인첸트 이고. 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 인첸트한 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 객주에 특정 아이템을 올리라는  조건이면, 
	if(pclcond->siFlag	== QUEST_CLEAR_COND_TUTORIALMAKEITEM)
	{
		// 퀘스트의 임무가 객주에 특정 아이템을 올리라는  조건이면, 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 만든 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}


	// 물품을 일정수 이상 사용하라는  조건이면, 
	if(pclcond->siFlag	== QUEST_CLEAR_COND_USEITEM)
	{
		// 퀘스트의 임무가 물품 사용하기이고 
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 사용된 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 특정 퀘스트를 부여 받아야 하는 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_GETQUEST)
	{
		SI32 questtype = pclClient->pclQuestManager->GetQuestTypeFromNPCKind(pclcond->siPara1);
		return pclCI->clQuestInfo.IsQuestClearOrGoing( questtype );
	}

	// 특정 퀘스트를 깨야 하는 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_CLEARQUEST)
	{
		return pclCI->clQuestInfo.IsClear(pclClient->pclQuestManager, pclcond->siPara1);
	}


	// 광고창을 띄우는 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_OPENHEADCHAT)
	{
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			if(((cltCharServer*)this)->bHeadChatSwitch == true)
			{
				return true;
			}
		}
		else
		{
			SI32 chatmode = ((cltCharClient*)this)->pclDrawChat->GetChatMode();
			if(chatmode == CHATMODE_HEAD)
			{
				return true;
			}

		}
	}

	// 아이템을 객주에 올리는 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_UPITEMTOMARKET)
	{
		// 퀘스트의 임무가 객주에 올리기이고
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 올려진 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}
	// 아이템을 무역상인에게 파는 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_SELLTRADER)
	{
		// 퀘스트의 임무가 무역상인에게 파는 거고
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 판 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}
	// 요술램프 퀘스트를 받았거나 받는다면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_GETMAGICLAMPQUEST )
	{
		if ( pclCI->clQuestInfo.clUnit[QUEST_TYPE_SPECIAL].siCurQuestUnique > 0 )
		{
			return true;
		}
	}
	// 제조 기술중 아무것이나 습득한것이 있다면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_ISLEARNED_MANUFACTURE )
	{
		SI16 siSkillType[] = {
				SKILL_MAKEMED		// 조제술.
				,SKILL_MAKEFOOD		// 요리술.
				,SKILL_MAKEARMOUR1	// 갑옷제작술1
				,SKILL_MAKEHELMET1	// 투구제작술1
				,SKILL_MAKESWORD1	// 검제작술1
				,SKILL_MAKESPEAR1	// 창제작술1
				,SKILL_MAKEAXE1		// 도끼제작술1
				,SKILL_MAKESTAFF1	// 지팡이제작술1
				,SKILL_MAKEBOW1		// 활제작술1
				,SKILL_MAKEGUN1		// 총제작술1
				,SKILL_MAKESHOES1	// 신발제작술1	
				,SKILL_MAKEBELT1	// 요대제작술1	
				,SKILL_MAKEDRESS1	// 의복제작술1
				,SKILL_MAKEIRON1	// 철물제작술 
				,SKILL_MAKETOOL1	// 생산도구제작술
				,SKILL_MAKEHAT1		// 모자제작술
				,SKILL_ARTIFACT		// 아티팩트 제작술
				,SKILL_MAKEAVATAR_DRESS	// 아바타 의복 제작술
				,SKILL_MAKEAVATAR_HAT	// 아바타 모자 제작술
				,SKILL_MAKEAVATAR_MANTLE	// 아바타 망토 제작술
				,SKILL_MAKEAVATAR_RING	// 아바타 반지 제작술
				,SKILL_MAKEAVATAR_NECK	// 아바타 목걸이 제작술
		};
		SI16 siCount = (SI16)( sizeof(siSkillType)/sizeof(SI16) );
		//SI16 siCount = 17;// 바뀔일이 없기때문에 계산식을 넣은 위의 것보다 바로 숫자를 넣었음

		for (SI16 i=0; i<siCount; i++)
		{
#ifdef _SAFE_MEMORY
			if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[i],	0, 0, &pclCI->clCharItem.clItem[0])	>= pclcond->siPara2 )
#else
			if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[i],	0, 0, pclCI->clCharItem.clItem)	>= pclcond->siPara2 )
#endif
			{
				return true;
			}
		}
	}
	// 엔펠리스에 있는 제조 퀘스트 중에 아무것이나 1단계 이상 클리어 했다면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_ISGET_MANUFACTUREQUEST )
	{
		SI16 siQuestType[] = {
			QUEST_TYPE_MAKEHEAL1	// 기초 약조제술 (헬레나)
			,QUEST_TYPE_MAKESWORD1	// 기초 검제조술 (스워디나)
			,QUEST_TYPE_MAKEBOW1	// 기초 활제조술 (보우인)
			,QUEST_TYPE_MAKEHELMET1	// 기초 투구제조술 (헤르메티오)
			,QUEST_TYPE_MAKESPEAR1	// 기초 창제조술 (스피어스)
			,QUEST_TYPE_MAKEARMOUR1	// 갑옷 제조술 (아머리어)
			,QUEST_TYPE_MAKEAX1		// 도끼 제조술 (액시아)
			,QUEST_TYPE_MAKEGUN1	// 총 제조술 (거너)
			,QUEST_TYPE_MAKESTAFF1	// 지팡이 제조술
			,QUEST_TYPE_MAKEBELT1	// 요대(벨트) 제조술 (벨티오)
			,QUEST_TYPE_MAKESHOES1	// 신발 제조술 (슈벨토)
		};
		//SI16 siCount = (SI16)( sizeof(siQuestType)/sizeof(SI16) );
		SI16 siCount = 11;

		for (SI16 i=0; i<siCount; i++)
		{
			if ( pclCI->clQuestInfo.clUnit[ siQuestType[i] ].siQuestState > 0 )
			{
				return true;
			}
		}
	}

	// (Para1)의 스텟이 (Para2) 이상이어야 한다
	if ( pclcond->siFlag == QUEST_CLEAR_COND_CHECKSTATUS )
	{
		switch (pclcond->siPara1)
		{
		case 1:// 근력(Str)
			{
				if ( clPB.clTotalBA.GetStr() >= pclcond->siPara2 ) return true;
			}
			break;
		case 2:// 순발력(Agi)
			{
				if ( clPB.clTotalBA.GetDex() >= pclcond->siPara2 ) return true;
			}
			break;
		case 3:// 지력(Wis)
			{
				if ( clPB.clTotalBA.GetMag() >= pclcond->siPara2 ) return true;
			}
			break;
		case 4:// 생명력(Vit)
			{
				if ( clPB.clTotalBA.GetVit() >= pclcond->siPara2 ) return true;
			}
			break;
		case 5:// 손재주(Dex)
			{
				if ( clPB.clTotalBA.GetHnd() >= pclcond->siPara2 ) return true;
			}
			break;
		case 6:// 행운(Luck)
			{
				if ( clPB.clTotalBA.GetLuk() >= pclcond->siPara2 ) return true;
			}
			break;
		case 7:// 이동속도(Speed)
			{
				if ( clPB.GetTotalMoveSpeed() >= pclcond->siPara2 ) return true;
			}
			break;
		case 8:// 공격력(Attack)
			{
				if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )
				{
					if ( (clPB.GetTotalPHysicalMinAttackDmg() >= pclcond->siPara2) ) return true;
				}
				else
				{
					if ( (clPB.GetTotalMinDamage() >= pclcond->siPara2) ) return true;
				}
			}
			break;
		case 9:// 방어력(Defense)
			{
				if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )
				{
					if ( clPB.GetPHysicalDefensive() >= pclcond->siPara2 ) return true;
				}
				else
				{
					if ( clPB.GetTotalAC() >= pclcond->siPara2 ) return true;
				}
			}
			break;
		}
	}

	// 6가지 무기 타입 중 아무 타입이나 para1 이상이면 퀘스트 클리어
	if ( pclcond->siFlag == QUEST_CLEAR_COND_ALLWEAPONSKILL ) 
	{
		SI32 siSkillType[]	= { SKILL_USESWORD1, SKILL_USESPEAR1, SKILL_USESTAFF1, SKILL_USEAXE1, SKILL_USEBOW1, SKILL_USEGUN1 };
	

		for ( UI16 uiCount=0; uiCount<6; uiCount++ )
		{
#ifdef _SAFE_MEMORY
			if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[uiCount], 0, 0, &pclCI->clCharItem.clItem[0]) >= pclcond->siPara1 )
#else
			if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[uiCount], 0, 0, pclCI->clCharItem.clItem) >= pclcond->siPara1 )
#endif
			{
				return true;
			}
		}
	}
//	// 35가지 스킬중 아무 스킬이나 para1 이상이면 퀘스트 클리어
//	if ( pclcond->siFlag == QUEST_CLEAR_COND_UPSKILL ) 
//	{
//		SI32 siSkillType[]	= { SKILL_USESWORD1,
//			SKILL_USESPEAR1,
//			SKILL_USESTAFF1,
//			SKILL_USEAXE1,
//			SKILL_USEBOW1,
//			SKILL_USEGUN1,
//			//장비 제잘술
//			SKILL_MAKEMED,
//			SKILL_MAKEFOOD,		
//			SKILL_MAKEARMOUR1,	
//			SKILL_MAKEHELMET1,	
//			SKILL_MAKESWORD1,	
//			SKILL_MAKESPEAR1,	
//			SKILL_MAKEAXE1,
//			SKILL_MAKESTAFF1,	
//			SKILL_MAKEBOW1,
//			SKILL_MAKEGUN1,
//			SKILL_MAKESHOES1,
//			SKILL_MAKEBELT1,
//			SKILL_MAKEDRESS1,
//			SKILL_USESWORD1,	
//			SKILL_USESPEAR1,	
//			SKILL_USEBOW1,	
//			SKILL_USEAXE1,	
//			SKILL_USEGUN1,	
//			SKILL_USESTAFF1,	
//			SKILL_SUMMON,	
//			SKILL_FISH,
//			SKILL_HORSETRAINING,
//			SKILL_MAKEIRON1,
//			SKILL_MAKETOOL1,
//			SKILL_CAPTURE,
//			SKILL_MAKEHAT1,
//			SKILL_FARM,
//			SKILL_MINE,
//			SKILL_ARTIFACT };
//
//
//		for ( UI16 uiCount=0; uiCount<35; uiCount++ )
//		{
//#ifdef _SAFE_MEMORY
//			if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[uiCount], 0, 0, &pclCI->clCharItem.clItem[0]) >= pclcond->siPara1 )
//#else
//			if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[uiCount], 0, 0, pclCI->clCharItem.clItem) >= pclcond->siPara1 )
//#endif
//			{
//				return true;
//			}
//		}
//	}
    
	// 가방칸 2칸중에 한칸이라도 가방이 장창되어 있는가?
	if (pclcond->siFlag == QUEST_CLEAR_COND_EQUIPBAG)
	{
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			SI32 siBagUnique1 = pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique;
			SI32 siBagUnique2 = pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique;

			// 장비창중에 해당 물품이 있는지 확인한다. 
			if( (siBagUnique1 > 0) || (siBagUnique2 > 0))
			{
				return true;
			}

		}
	}
	
	// 흑의 군단 맴버중에 아무나 para2만큼 잡아라
	if (pclcond->siFlag == QUEST_CLEAR_COND_KILLDARKSOLDIER)
	{
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 처치해야 할 수 이상을 처치하면, 
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 내장 기관중 어느 한 기관이라도 Max이면 클리어
	if (pclcond->siFlag == QUEST_CLEAR_COND_FULL_ORGAN)
	{
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			SI32 siLevel		= pclCI->clIP.GetLevel();
			SI32 siMaxOrgan		= min(100, MAXORG(siLevel));
			SI32 siOrganType[]	= {HEALTH_ORGAN_TYPE_ORG1, HEALTH_ORGAN_TYPE_ORG2, HEALTH_ORGAN_TYPE_ORG3, HEALTH_ORGAN_TYPE_ORG4};
			
			
			for (SI32 siIndex=0; siIndex<4; siIndex++)
			{
				SI32 siGetOrgan = pclCI->clHealthInfo.GetOrg( siOrganType[siIndex] );

				if (siGetOrgan >= siMaxOrgan)
				{
					return true;
				}
			}
			
		}
	}

	// 시장에서 물건을 구매해야하는 퀘스트라면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_BUYMARKET )
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			// 사야되는 갯수 이상이면
			if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2 )
			{
				return true;
			}
		}
	}
	
	// 특정 레벨의 아템을 만드는 퀘스트 라면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_MAKELEVELITEM )
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			// 만들어야 하는 갯수 이상이면
			if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2 )
			{
				return true;
			}
		}
	}

	// 특정 레벨의 아템을 만드는 퀘스트 라면 - 레벨 제한 없음
	if ( pclcond->siFlag == QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM )
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			// 만들어야 하는 갯수 이상이면
			if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2 )
			{
				return true;
			}
		}
	}

	// 특정레벨 무기를 몇개 이상 갖고 오라는 퀘스트 라면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_TAKEEQUIPLEVELWEAPON )
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			SI16 siNeedLevel	= pclcond->siPara1;	// 필요 레벨
			SI32 siNeedAmount	= pclcond->siPara2; // 필요한 갯수
			SI32 siNowAmount	= 0;				// 내가 갖고 있는 갯수

			// 현재 내 인벤토리의 아이템을 검사
			for ( SI32 siIndex=MAX_SHAPE_ITEM; siIndex<MAX_ITEM_PER_PERSON; ++siIndex )
			{
				cltItem* pclItem = &pclCI->clCharItem.clItem[siIndex];
				if ( NULL == pclItem )
				{
					continue;
				}
				
				// 아이템 타입이 검,창,도끼,활,총,지팡이가 아니면 패스
				if ( false == pclItem->IsWeapon( pclClient->pclItemManager ) )
				{
					continue;
				}

				// 착용레벨이 조건에 맞는 것만 카운트
				if ( siNeedLevel == pclItem->GetEquipLevel(pclClient->pclItemManager) )
				{
					++siNowAmount;
				}
			}

			if ( siNeedAmount <= siNowAmount )
			{
				return true;
			}
		}
	}
	
	// 환원을 통해 어떤 아이템을 생성해 내라(원소, 속성가루)
	if ( (pclcond->siFlag == QUEST_CLEAR_COND_GETELEMENT_RESTORE) || (pclcond->siFlag == QUEST_CLEAR_COND_GETPOWDER_RESTORE) )
	{
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 만든 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	// 특정 아이템을 만들 수 있는 아이템의 재료 모아오기
	if ( pclcond->siFlag == QUEST_CLEAR_COND_TAKEITEM_MATERIAL )
	{
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			SI32 siUnique = pclcond->siPara1;
			SI32 siAmount = pclcond->siPara2;

			if ( true == pclCI->clCharItem.IsHaveMaterialItem(pclClient->pclItemManager, siUnique, siAmount) )
			{
				return true;
			}
		}
	}

	// 슬롯이 있는 무기 아이템을 가져오라
	if ( pclcond->siFlag == QUEST_CLEAR_COND_TAKEWEAPONHAVESLOT )
	{
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			SI32 siNeedAmount	= pclcond->siPara2;
			SI32 siNowAmount	= 0;

			// 현재 내 인벤에 슬롯이 있는 아이템을 검사한다
			for ( SI32 siIndex=MAX_SHAPE_ITEM; siIndex<MAX_ITEM_PER_PERSON; ++siIndex )
			{
				cltItem* pclItem = &pclCI->clCharItem.clItem[siIndex];
				if ( NULL == pclItem )
				{
					continue;
				}
				
				if ( false == pclItem->IsWeapon( pclClient->pclItemManager ) )
				{
					continue;
				}

				if ( false == pclItem->IsHaveCrystalSlot() )
				{
					continue;
				}

				siNowAmount += pclItem->siItemNum;
			}

			if ( siNeedAmount <= siNowAmount )
			{
				return true;
			}
		}
	}

	// 무기 아이템에 슬롯을 조합하라는 퀘스트라면
	// 아이템에 아이팩트를 조합해보라는 퀘스트 라면
	if ( (pclcond->siFlag == QUEST_CLEAR_COND_ARTIFACTMIXWEAPON)
		|| (pclcond->siFlag == QUEST_CLEAR_COND_ARTIFACTMIX)
	)
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			// 갯수 이상이면
			if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2 )
			{
				return true;
			}
		}
	}

	// 특정등급의 결정체를 만들라는 퀘스트라면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_MAKECRYSTAL )
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			// 만들어야 하는 갯수 이상이면
			if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2 )
			{
				return true;
			}
		}
	}

	// 아무 소환수나 보너스 스텟을 0으로 만드는 퀘스트 라면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_SUMMONBONUSZERO )
	{
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			for ( SI32 siIndex=0; siIndex<MAX_SUMMON_OWN; ++siIndex )
			{
				SI32 siSummonKind	= pclCI->clSummonInfo.clSummon[siIndex].siKind;
				SI32 siSummonStatus	= pclCI->clSummonInfo.clSummon[siIndex].siStatus;
				SI32 siSummonBonus	= pclCI->clSummonInfo.clSummon[siIndex].clIP.GetBonus();

				if ( (0 < siSummonKind) && (MAX_KIND_NUMBER > siSummonKind) && (SUMMON_STATUS_TAKEOUT == siSummonStatus) && (0 >= siSummonBonus) )
				{
					return true;
				}
			}
		}
	}

	// 특정 레벨의 아템을 만드는 퀘스트 라면
	if ( pclcond->siFlag == QUEST_CLEAR_COND_LEARNWEAPONSKILL )
	{
		// 지금 걸린 퀘스트 확인 하고
		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
		{
			SI32 siGrade = pclcond->siPara1-1;	// 1단계부터 시작이기때문에 배열은 0부터이다 그래서 -1해준다
			SI32 siLevel = pclcond->siPara2;
			if ( (0 <= siGrade) && (3 > siGrade) )
			{
				SI32 siSkillUnique[3][6] = {
					{SKILL_SWORD_INCDR,		SKILL_SPEAR_INCAC,		SKILL_AXE_DECGAP,	SKILL_BOW_DRAIN,	SKILL_GUN_INCPHYAR,		SKILL_STAFF_INCMAG}
					,{SKILL_SWORD_INCVIT,	SKILL_SPEAR_DOUBLEAT,	SKILL_AXE_INDURE,	SKILL_BOW_INCRANGE,	SKILL_GUN_DECMAGDELAY,	SKILL_STAFF_DECUSEMANA}
					,{SKILL_SWORD_INCMAGAT,	SKILL_SPEAR_INCMAGAT,	SKILL_AXE_INCCRI,	SKILL_BOW_INCMAGAT,	SKILL_GUN_INCWEAAT,		SKILL_STAFF_INCMAGAT}
				};

				// 1단계에서 3단계(siGrade) 스킬중 아무거나 siLevel을 넘으면 클리어
				for ( SI32 siIndex=0; siIndex<6; siIndex++ )
				{
					if ( siLevel <= pclCI->clCharSkill.GetSkillLevel(siSkillUnique[siGrade][siIndex]) )
					{
						return true;
					}
				}
			}
		}
	}

	// 일정레벨이상 인챈트 된 아이템 착용해 보기
	if(pclcond->siFlag == QUEST_CLEAR_COND_ENCHANTEQUIPPOS)
	{
		// 지금 걸린 퀘스트 확인
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 1. 해당 장착부분이 무기부분인지 확인
			if(pclcond->siPara1 == PERSONITEM_WEAPON1 || pclcond->siPara1 == PERSONITEM_WEAPON2)
			{
				// 무기 부분이 들어오면 두군데를 다 검색한다.
				if(pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].Element.siPower >= pclcond->siPara2)
				{
					return true;
				}
				else if(pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].Element.siPower >= pclcond->siPara2)
				{
					return true;
				}
			}
			else
			{
				// 2. 해당 아이템이 일정 인챈트 레벨 이상으로 인챈트 되어 있으면
				if( pclCI->clCharItem.clItem[pclcond->siPara1].Element.siPower >= pclcond->siPara2)
				{
					return true;
				}
			}
		}
	}

	// 매직스킬북을 사용하는 퀘스트라면
	if(pclcond->siFlag	== QUEST_CLEAR_COND_USE_SKILLBOOK)
	{
		// 퀘스트의 임무가 매직스킬북 사용하기이고
		if(pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag)
		{
			// 사용된 물품이 정해진 수 이상이면,
			if(pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 >= pclcond->siPara2)
			{
				return true;
			}
		}
	}

	////192 para1갯수 만큼 단체메세지 보내기.
	//if ( pclcond->siFlag == QUEST_CLEAR_COND_SEND_MESSAGE )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara2 )
	//		{
	//			return true;
	//		}
	//	}
	//}
//		//192 para1레벨까지 스킬올리기
//	if ( pclcond->siFlag == QUEST_CLEAR_COND_UPSKILL )
//	{
//		// 지금 걸린 퀘스트 확인 하고
//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
//		{
//			//QUEST_CLEAR_COND_ALLWEAPONSKILL
//			SI32 siSkillType[]	= { 
//				// 무기 사용술
//				SKILL_USESWORD1,
//				SKILL_USESPEAR1,
//				SKILL_USESTAFF1,
//				SKILL_USEAXE1,
//				SKILL_USEBOW1,
//				SKILL_USEGUN1,
//				//장비 제잘술
//				SKILL_MAKEMED,
//				SKILL_MAKEFOOD,		
//				SKILL_MAKEARMOUR1,	
//				SKILL_MAKEHELMET1,	
//				SKILL_MAKESWORD1,	
//				SKILL_MAKESPEAR1,	
//				SKILL_MAKEAXE1,
//				SKILL_MAKESTAFF1,	
//				SKILL_MAKEBOW1,
//				SKILL_MAKEGUN1,
//				SKILL_MAKESHOES1,
//				SKILL_MAKEBELT1,
//				SKILL_MAKEDRESS1,
//				SKILL_USESWORD1,	
//				SKILL_USESPEAR1,	
//				SKILL_USEBOW1,	
//				SKILL_USEAXE1,	
//				SKILL_USEGUN1,	
//				SKILL_USESTAFF1,	
//				SKILL_SUMMON,	
//				SKILL_FISH,
//				SKILL_HORSETRAINING,
//				SKILL_MAKEIRON1,
//				SKILL_MAKETOOL1,
//				SKILL_CAPTURE,
//				SKILL_MAKEHAT1,
//				SKILL_FARM,
//				SKILL_MINE,
//				SKILL_ARTIFACT,
//			};
//			for ( UI16 uiCount=0; uiCount<6; uiCount++ )
//			{
//#ifdef _SAFE_MEMORY
//				if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[uiCount], 0, 0, &pclCI->clCharItem.clItem[0]) >= pclcond->siPara1 )
//#else
//				if ( pclCI->clCharSkill.GetTotalSkillLevel(siSkillType[uiCount], 0, 0, pclCI->clCharItem.clItem) >= pclcond->siPara1 )
//#endif
//				{
//					return true;
//				}
//			}
//		}
//	}
	////193 데일리 퀘스트 2,3 을 para1만큼 클리어
	//if ( pclcond->siFlag == QUEST_CLEAR_COND_DAILY23SUCCES )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}
	//}
	////193 변신 싱크로 para1 만큼 달성
	//if ( pclcond->siFlag == QUEST_CLEAR_COND_CHANGELING )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		SI32 sisync = clTransFormInfo.GetSynchroTransForm(clTransFormInfo.uiNowTransFormKind);
	//		
	//		if ( sisync >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}		
	//}
	////194 마법/물리 공격력을 para1만큼 달성 -> QUEST_CLEAR_COND_CHECKSTATUS이걸로 대체 .
	//if ( pclcond->siFlag == QUEST_CLEAR_COND_ATTACKPOINT )
	//{

	//}
	//195 생산활동( 농경/ 채광 / 낚시) 물품 para1개 이상 생산
	//if ( pclcond->siFlag == QUEST_CLEAR_COND_PRODUCTION_ACTIVITIES )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}
	//}
	////196 마을 이벤트 PARA1 회 이상 참여
	//if ( pclcond->siFlag == QUEST_CLEAR_COND_VILLAGEEVENT )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}
	//}
	////197 pvp 참여 para1 회 이상 참여
	//if (pclcond->siFlag == QUEST_CLEAR_COND_PVP_PLAYING )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}
	//}
	////198 도우미 시스템 이용답변 para1개 이상
	//if (pclcond->siFlag == QUEST_CLEAR_COND_HELPERSERVICE )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}
	//}
	////199 보스 몬스터 10마리 잡기.
	//if (pclcond->siFlag == QUEST_CLEAR_COND_KILLBOSS )
	//{
	//	// 지금 걸린 퀘스트 확인 하고
	//	if ( pclCI->clQuestInfo.clUnit[questtype].siQuestCmd == pclcond->siFlag )
	//	{
	//		// para1 갯수 이상이면.
	//		if ( pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 >= pclcond->siPara1 )
	//		{
	//			return true;
	//		}
	//	}
	//}
	return false;
}

// 임무의 완료 조건에서 아이템이 요구되면 아이템을 캐릭터에게서 삭제한다. 
bool cltCharCommon::DoClearCondition(cltQuestClearConditionInfo* pclcond)
{
	SI32 i;

	if(pclcond == NULL)return false;
	cltPItemInfo* pcliteminfo = NULL;
	cltCharServer* pclchar = (cltCharServer*)this;

	// 인벤 아이템 조건이라면, 
	if(pclcond->siFlag == QUEST_CLEAR_COND_ITEM)
	{
		SI32 itemunique = pclcond->siPara1;
		SI32 itemnum	= pclcond->siPara2;
		cltItem clitem;

		pcliteminfo = &pclCI->clCharItem;

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
					cltGameMsgResponse_PersonItemInfo clinfo( i, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);

/*					// 사용자로부터 아이템을 빼는 메시지를 사용한다. 
					sDBRequest_OutputItemFromPerson clMsg(GetID(), pclCI->GetPersonID(), i, &clitem);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);*/
					
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
					cltGameMsgResponse_PersonItemInfo clinfo( i, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);

/*					// 사용자로부터 아이템을 빼는 메시지를 사용한다. 
					sDBRequest_OutputItemFromPerson clMsg(GetID(), pclCI->GetPersonID(), i, &clitem);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);*/

					itemnum -= clitem.siItemNum;
				}
			}
		}
	}

	return true;
}

