//---------------------------------
// 2004/8/27 김태곤
//---------------------------------
#include "../../Server/Server.h"
#include "Char-Server.h"
#include "../CharManager/CharManager.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../Common/NPCManager/NPC.h"

#include "../../GameEvent/GameEvent.h"

#include "Msg/MsgType-Quest.h"
#include "../../CommonHeader.h"
#include "MsgRval-Define.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Item.h"

#include "../../Common/Quest/NamingQuest.h"
#include "../../common/Char/CharServer/Char-Item.h"


extern cltCommonLogic* pclClient;





//[지연] : 문자열로 된 npc카인드를 초기화 한다.
void cltCharServer::InitArray_RemoveUnLuckQuestNPC()
{
	// 노가다로 초기화;;
	m_arRemoveUnluckQuest_NPC[0] = TEXT( "KIND_SOPHIA" );
	m_arRemoveUnluckQuest_NPC[1] = TEXT( "KIND_SPARKY" );
	m_arRemoveUnluckQuest_NPC[2] = TEXT( "KIND_MERIEL" );
	m_arRemoveUnluckQuest_NPC[3] = TEXT( "KIND_BRAI" );
	m_arRemoveUnluckQuest_NPC[4] = TEXT( "KIND_BELITA" );
	m_arRemoveUnluckQuest_NPC[5] = TEXT( "KIND_HINEAS" );
	m_arRemoveUnluckQuest_NPC[6] = TEXT( "KIND_KELAN" );
	m_arRemoveUnluckQuest_NPC[7] = TEXT( "KIND_JEFFREY" );
	m_arRemoveUnluckQuest_NPC[8] = TEXT( "KIND_MOLDA" );

	m_arRemoveUnluckQuest_Unique[0] = 2000;
	m_arRemoveUnluckQuest_Unique[1] = 2400;
	m_arRemoveUnluckQuest_Unique[2] = 3000;
	m_arRemoveUnluckQuest_Unique[3] = 3500;
	m_arRemoveUnluckQuest_Unique[4] = 4000;
	m_arRemoveUnluckQuest_Unique[5] = 4500;
	m_arRemoveUnluckQuest_Unique[6] = 28000;
	m_arRemoveUnluckQuest_Unique[7] = 5500;
	m_arRemoveUnluckQuest_Unique[8] = 26000;
}

// 퀘스트의 파라메타를 증가시킨다. 
void cltCharServer::IncQuestPara(SI32 clearcond, SI32 para1, SI32 incnum)
{
	SI16 questtype;
	SI16 unique;
	SI32 startindex = 0;
	// 해당 종류의 클리어 조건을 갖는 퀘스트를 찾을때까지. 
	while(pclCI->clQuestInfo.IsQuestGoing( clearcond, &questtype, &unique, NULL, NULL, &startindex) == true)
	{
		if(pclCI->clQuestInfo.IncPara(questtype, para1, incnum) == true)
		{
			// 클라이언트에 퀘스트의 상태를 통보한다. 
			SetUpdateSwitch(UPDATE_QUEST, true, 0);

			if ( TABS( lastQuestSaveDBTime - pclClient->CurrentClock ) > 60000 )
			{
				// LEEKH MODIFY 2007.10.25
				cltPersonQuestUnitInfo* pQuestUnit = &pclCI->clQuestInfo.clUnit[questtype];
				if(pQuestUnit != NULL)
				{
					// DB에 퀘스트의 상태를 저장한다. 
					sDBRequest_SetQuestInfo clMsg(pclCI->GetPersonID(), questtype, pQuestUnit);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}
		}
	}
}


// 캐릭터가 진행하고 있는 퀘스트를 점검하여 종결시킨다. 
void cltCharServer::DoQuestCheckAndAction(SI32 questclearcond)
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI16 questtype, questunique;
	SI16 para1, para2;
	if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		cltPersonQuestUnitInfo* pclquestinfo = &pclCI->clQuestInfo.clUnit[questtype];
		if(pclquestinfo->siCurQuestStep != QUEST_STEP_MIDDLE)return ;

		/*		//KHY - 0822 - 해피빈 퀘스트 추가.
		if(questtype == QUEST_TYPE_SPECIAL || questtype == QUEST_TYPE_NAMING || questtype == QUEST_TYPE_HAPPYBEAN )
		{

		}
		else
		{
			if(pclquestinfo->siCurQuestUnique != questunique)return ;
		}*/

			switch( questtype )
		{
			case QUEST_TYPE_SPECIAL:
			case QUEST_TYPE_NAMING:
			case QUEST_TYPE_HAPPYBEAN:
			case QUEST_TYPE_MISSION1:
				{
				}
				break;
			default:
				{
					if( pclquestinfo->siCurQuestUnique != questunique )			return;
				}
				break;
		}

		// 퀘스트의 상태를 변경한다. 
		pclquestinfo->siQuestPara1	= para1;
		pclquestinfo->siQuestPara2	= para2;

		// DB에 퀘스트의 상태를 저장한다. 
		sDBRequest_SetQuestInfo clMsg(pclCI->GetPersonID(), questtype, pclquestinfo);
		pclClient->SendDBMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);

		// 클라이언트에 퀘스트의 상태를 통보한다. 
		SetUpdateSwitch(UPDATE_QUEST, true, 0);
	}
}


// 장비에 부적 바르기.
bool cltCharServer::MakeSpecialQuest_Scroll()
{

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 10;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_SCROLL;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	0;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	1;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 기술 올리기  특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_Skill()
{

	//적절한 기술을 찾는다. 
	SI32 skillunique;		// 올려야 할 기술종류. 
	SI32 upgradeskilllevel;	// 올려야 할 기술 정도.



#ifdef _SAFE_MEMORY
	if(pclCI->clCharSkill.GetRandSkillForUpgrade(pclClient->pclSkillManager, &pclCI->clCharItem.clItem[0], &skillunique, &upgradeskilllevel )
		== false) return false; // 퀘 완료 경험치 얻어오게 변경.
#else
	if(pclCI->clCharSkill.GetRandSkillForUpgrade(pclClient->pclSkillManager, pclCI->clCharItem.clItem, &skillunique, &upgradeskilllevel )
		== false) return false; // // 퀘 완료 경험치 얻어오게 변경.
#endif

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp		= pclCI->clIP.GetLevel() * 100;


	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_SKILL;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	skillunique;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	upgradeskilllevel;


	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;

}
// 장비 장착하기 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_Equip()
{

	SI32 equiplist[100];
	SI32 listindex	= 0;
	SI32 equippos[] ={
		PERSONITEM_WEAPON1,
			PERSONITEM_WEAPON2,
			PERSONITEM_HELMET, 
			PERSONITEM_BELT,
			PERSONITEM_ARMOUR, 
			PERSONITEM_SHOES,
	}; 

	SI32 equipposnum = sizeof(equippos) / sizeof(SI32);

	// 장비하지 않은 물품을 고른다. 
	for(SI32 i = 0;i <equipposnum;i++)
	{
		if(pclCI->clCharItem.clItem[ equippos[i] ].siUnique == 0)
		{
			equiplist[listindex] = equippos[i];
			listindex++;
		}

	}

	SI32 pos = 0;
	if(listindex)
	{
		pos = equiplist[rand() % listindex];
	}
	else
	{
		return false;
	}

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 100;


	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_EQUIPPOS;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	pos;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	1;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}


// 낚시하기용 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_Fish()
{

	//SI32 fishlist[] ={
	//	16100,	// 붕어 
	//	16110,	// 메기 
	//	16120	// 잉어 
	//};

	//SI32 fishlistnum = sizeof(fishlist) / sizeof(SI32);

	//SI32 fishkind	= fishlist[rand() % fishlistnum];
	//SI32 data = (rand() % 4);
	//SI32 fishnum	= max(2, data);	

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 100;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_FISH;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	0;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	1;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 농사짓기용 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_Farm()
{

	SI32 farmitemnum = 0;

	//SI32 *pfarmlist = NULL;
	//if(pclClient->pclItemManager->GetFarmItemList(&farmitemnum, &pfarmlist) == false)
	//{
	//	if(pfarmlist)
	//	{
	//		delete[] pfarmlist;
	//		pfarmlist = NULL;
	//	}
	//	return false;
	//}

	//if(farmitemnum <= 0)return false;

	SI32 farmkind	= 0; // 종류에 상관 없다. 필요없으나 para값은 입력..
	SI32 farmnum	= 1;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_FARM;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	farmkind;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	farmnum;

	// 보상을 구한다. 
	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 100;

	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 채광하기용 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_Mine()
{

	SI32 farmkind	= 0; // 종류에 상관 없다. 필요없으나 para값은 입력..
	SI32 farmnum	= 1;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MINE;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	farmkind;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	farmnum;

	// 보상을 구한다. 
	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 100;

	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 친구만들기용 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_MakeFriend()
{

	// 현재 친구의 수를 구한다. 
	SI32 curfriendnum	= 0;
	if(pclFriendInfo)		curfriendnum = pclFriendInfo->GetFriendNum();
	if( curfriendnum >= MAX_FRIEND_NUMBER )return false;

	cltQuestRewardInfo clrewardinfo;

	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 100;


	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKEFRIEND;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	1;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	curfriendnum + 1;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 전투용 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_Battle()
{
	SI32 monkind	= pclClient->GetUniqueFromHash( TEXT("KIND_RACCON") );
	SI32 monnum		= 1;
	SI32 exp		= 100;

	cltQuestRewardInfo clrewardinfo;

	// 레벨에 맞는 적절한 몬스터와 수량을 구한다. 
	if(pclClient->pclKindInfoSet->FindGoodMonsterForSpecialQuest(pclCI->clIP.GetLevel(),&monkind, &monnum, &clrewardinfo ) == false)
	{
		return false;
	}

	/*
#ifdef _DEBUG
	monnum = 3;
#endif
	*/

		// 퀘스트의 종료 조건을 구한다. 
		pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_KILLMON;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	monkind;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	monnum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 물건 사용해보기 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_UseItem()
{
	SI32 siitemlist[] ={
		ITEMUNIQUE(2565),	ITEMUNIQUE(2570),	ITEMUNIQUE(2575),
			ITEMUNIQUE(2580),	ITEMUNIQUE(2585),	ITEMUNIQUE(2590),	ITEMUNIQUE(2595),
			ITEMUNIQUE(2600),	ITEMUNIQUE(2605),	ITEMUNIQUE(2610),	ITEMUNIQUE(2615),
			ITEMUNIQUE(2620),	ITEMUNIQUE(2630),	ITEMUNIQUE(2635),

			ITEMUNIQUE(3735),	ITEMUNIQUE(3740),	ITEMUNIQUE(3745),	ITEMUNIQUE(3750),
			ITEMUNIQUE(3755),	ITEMUNIQUE(3760),	ITEMUNIQUE(3765),	ITEMUNIQUE(3770),
			ITEMUNIQUE(3775),	ITEMUNIQUE(3780),	ITEMUNIQUE(3785),	ITEMUNIQUE(3790),
			ITEMUNIQUE(3795),	ITEMUNIQUE(3800),	ITEMUNIQUE(3805),	ITEMUNIQUE(3810),
			ITEMUNIQUE(3815),	ITEMUNIQUE(3820),	ITEMUNIQUE(3825),	ITEMUNIQUE(3830),
			ITEMUNIQUE(3835),	ITEMUNIQUE(3840),
	};

	SI32 itemlistnum	= sizeof(siitemlist) / sizeof(SI32);
	if(itemlistnum <= 0)return false;

	SI16 itemunique = siitemlist[ rand() % itemlistnum];


#ifdef _DEBUG
	//SI16 itemnum	= 1;
	SI16 itemnum = 30 + ( rand() % 21); // 사용 개수 변경 30~50개. (테스트후 변경)
#else


	//SI16 itemnum	= 50;
	SI16 itemnum = 30 + ( rand() % 21); // 사용 개수 변경 30~50개.
#endif


	cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, itemunique);
	if(pclprice == NULL)return false;



	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp		= pclCI->clIP.GetLevel() * 100;
	//clrewardinfo.siMoney	= pclprice->siPrice * itemnum * 50 / 100;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_USEITEM;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	itemunique;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	itemnum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}



// 물품 가져오기 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_GetItem()
{
	SI32 itemunique	= 0;
	SI32 itemnum	= 1;

	SI16 itemtypelist[] = {
		ITEMTYPE_MINERAL,	
			ITEMTYPE_CLOTH,
			ITEMTYPE_TREE,
	};

	SI32 itemtypelistnum = sizeof(itemtypelist) / sizeof(SI16);

	SI16 itemtype = itemtypelist[ rand() % itemtypelistnum];


	cltQuestRewardInfo clrewardinfo;

	// 내 인벤에 있는경우는 다시 찾는다.
	for ( SI32 i=0; i < 3; ++i  )
	{
		itemunique = pclClient->pclMarketManager->GetRandomOrder( itemtype );

		if(itemunique <= 0)
		{
			return false;
		}

		if( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_NOTDAILYQUEST ) )
		{
			return false;
		}

		// 내 인벤에 없는 경우에만 더이상 찾기 중지;
		if(pclCI->clCharItem.FindItemInv(itemunique) == false ) 
			break;
	}

#ifdef _DEBUG
	//itemnum = 3;
	itemnum = 30 + ( rand() % 21); // 수집개수 변경 30~50개.
#else
	//itemnum = 50;	// 수집개수 50개.
	itemnum = 30 + ( rand() % 21); // 수집개수 변경 30~50개.
#endif



	clrewardinfo.siExp		= pclCI->clIP.GetLevel() * 100;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_ITEM;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	itemunique;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	itemnum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}


// 물품 생산하기 특수 퀘스트를 만든다. 
bool cltCharServer::MakeSpecialQuest_MakeItem()
{

	SI16 skilllist[] = {
		SKILL_MAKEMED,	
			SKILL_MAKEFOOD,	
			SKILL_MAKEARMOUR1,	
			SKILL_MAKEHELMET1,	
			SKILL_MAKESWORD1,	
			SKILL_MAKESPEAR1,	
			SKILL_MAKEAXE1,	
			SKILL_MAKESTAFF1,
			SKILL_MAKEBOW1,	
			SKILL_MAKEGUN1,	
			SKILL_MAKESHOES1,	
			SKILL_MAKEBELT1,	
			SKILL_MAKEDRESS1,	
			SKILL_MAKEIRON1,
			SKILL_MAKETOOL1,
			SKILL_MAKEHAT1,
			SKILL_ARTIFACT
	};


	SI32 skillnum = sizeof(skilllist) / sizeof(SI16);

	SI16 skillbuffer[MAX_SKILL_NUMBER];
	SI16 skilllevelbuffer[MAX_SKILL_NUMBER];
	SI32 skillbufferindex = 0;
	for(SI32 i = 0;i < skillnum;i++)
	{
		SI32 tempskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(skilllist[i], 0, NULL);
		if(tempskilllevel == 0)continue;

		skillbuffer[skillbufferindex]		= skilllist[i];
		skilllevelbuffer[skillbufferindex]	= tempskilllevel;
		skillbufferindex++;
	}

	if(skillbufferindex == 0)
	{
		return false;
	}

	SI32 index			= rand() % skillbufferindex;
	SI32 skillunique	= skillbuffer[index];
	SI32 skilllevel		= skilllevelbuffer[index];


	// 특정 기술로 만들수 있는 물품의 리스트를 구한다. 
	SI16 siitemlist[MAX_ITEMINFO_NUMBER];
	SI32 itemlistindex = 0;
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{

		if(pclClient->pclItemManager->pclItemInfo[i] == NULL)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkill != skillunique)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel > skilllevel)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel < max(1, skilllevel/2) )continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique == 0)continue;


		if(pclClient->pclItemManager->pclItemInfo[i]->CanMake_Skill(&pclCI->clCharSkill, &pclCI->clCharItem, &clPB.clTotalBA) == false)continue;

		siitemlist[itemlistindex] = pclClient->pclItemManager->pclItemInfo[i]->clItem.siUnique;
		itemlistindex++;
	}

	if(itemlistindex == 0)return false;

	SI32 makeitemunique = siitemlist[rand() % itemlistindex];
	//SI32 makenum	= 3 + ( rand() % 3 ); // 제조 개수 3~5개.

	SI32 makenum = 1 + ( rand() % 5 );	  // 제조 개수 1~ 5개.


	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)	return false;

	cltQuestRewardInfo clrewardinfo;

	clrewardinfo.siExp		= pclCI->clIP.GetLevel() * 100;
	clrewardinfo.siMoney	= 0;


	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKEITEM;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	makeitemunique;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	makenum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;

}


// 마을 순회하기 특수 퀘스트를 만든다.
bool cltCharServer::MakeSpecialQuest_Gomap()
{
	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp	= pclCI->clIP.GetLevel() * 100;

	// 가야 할 마을의 인덱스를 구해서 siPara1에 넣어준다.
	SI32 siDestTownIndex = 0;
	siDestTownIndex = rand() % 42 + 1;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_LAMPGOMAP;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	siDestTownIndex;	// 가야할 마을의 인덱스.
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	1;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 제조 스킬 경험치 포인트 1 올리는 퀘스트를 만든다.
bool cltCharServer::MakeSpecialQuest_MakeSkillExpup()
{

	//적절한 기술을 찾는다. 
	SI32 skillunique = 0;		// 올려야 할 기술종류. 
	SI32 increaseSkillExp = 0;	// 올려야 할 기술 경험치를 담아올 변수


#ifdef _SAFE_MEMORY
	if(pclCI->clCharSkill.GetRandSkillEXPupForUpgrade(pclClient->pclSkillManager, &pclCI->clCharItem.clItem[0], &skillunique, &increaseSkillExp)
		== false) return false; // 퀘 완료 경험치 얻어오게 변경.
#else
	if(pclCI->clCharSkill.GetRandSkillEXPupForUpgrade(pclClient->pclSkillManager, pclCI->clCharItem.clItem, &skillunique, &increaseSkillExp)
		== false) return false; // // 퀘 완료 경험치 얻어오게 변경.
#endif

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siExp		= pclCI->clIP.GetLevel() * 100;


	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKESKILLEXPUP;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara1	=	skillunique;
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestClearConditionInfo.siPara2	=	increaseSkillExp;		// 경험치 1이 증가하면 종료되는 조건 추가.

	// 보상을 구한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 특수 퀘스트를 만든다. (요술램프)
bool cltCharServer::MakeSpecialQuest()
{
	SI32 questtype = QUEST_TYPE_SPECIAL;

	// 기존 특수 퀘스트는  삭제한다. 
	pclCI->clQuestInfo.clSpecialQuestInfo.Init();
	pclCI->clQuestInfo.clUnit[questtype].Init();

	bool bsuccess = false;

	srand( (unsigned)time( NULL ) );


	// 5% 확률로 깨진 박
	if ( rand() % 100 < 5 ) 
		return false;


	for ( SI32 i = 0; i < 3 ; ++i )
	{
		SI32 sirand = rand() % 13;
		switch( sirand )
		{
		case 0:
		case 9:
		case 10:
			if(MakeSpecialQuest_Battle() == true)bsuccess = true;
			break;
		case 1:
			// 농사짓기.  
			if(MakeSpecialQuest_Farm() == true)bsuccess	= true;
			break;
		case 2:
			// 낚시하기.
			if(MakeSpecialQuest_Fish() == true)bsuccess	= true;
			break;
		case 3:
			// 채광하기.
			if(MakeSpecialQuest_Mine() == true)bsuccess	= true;
			break;
		case 4:
			// 기술 올리기 
			if(MakeSpecialQuest_Skill() == true)bsuccess = true;
			break;
		case 5:
			// 물건 사용해보기.
			if(MakeSpecialQuest_UseItem() == true)bsuccess	= true;
			break;
		case 6:
			// 친구 만들기. 
			if(MakeSpecialQuest_MakeFriend() == true)bsuccess = true;
			break;
		case 7:
			// 물품 가져오기
			if(MakeSpecialQuest_GetItem() == true)bsuccess = true;
			break;
		case 8:
			// 물품 생산하기
			if(MakeSpecialQuest_MakeItem() == true)bsuccess = true;
			break;
		case 11:
			// 마을 순회하기.
			if(MakeSpecialQuest_Gomap() == true )bsuccess = true;
			break;
		case 12:
			// 경험치 1 올리기.
			if(MakeSpecialQuest_MakeSkillExpup() == true )bsuccess = true;
			break;
		default:
			bsuccess = false;
			break;
		}
		if ( bsuccess  ) break;
	}

	if ( bsuccess == false )
		return false;

	cltQuestInfoCommon* pclquestinfo = &pclCI->clQuestInfo.clSpecialQuestInfo;

	// 현재 진행중인 퀘스트 정보를 등록한다. 
	pclCI->clQuestInfo.clUnit[questtype].siCurQuestUnique	= 1;
	pclCI->clQuestInfo.clUnit[questtype].siQuestCmd		= pclquestinfo->clQuestClearConditionInfo.siFlag;
	pclCI->clQuestInfo.clUnit[questtype].siCurQuestStep	= QUEST_STEP_MIDDLE;


	// 특수 퀘스트 정보를 저장한다. 
	// DB에 퀘스트이 상태를 저장한다. 
#ifdef _SAFE_MEMORY
	sDBRequest_SetSpecialQuestInfo clMsg1(pclCI->GetPersonID(), &pclCI->clQuestInfo.clSpecialQuestInfo, &pclCI->clQuestInfo.bWordAssemblySwitch[0]);
#else
	sDBRequest_SetSpecialQuestInfo clMsg1(pclCI->GetPersonID(), &pclCI->clQuestInfo.clSpecialQuestInfo, pclCI->clQuestInfo.bWordAssemblySwitch);
#endif
	pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg1);

	sDBRequest_SetQuestInfo clMsg2(pclCI->GetPersonID(), questtype, &pclCI->clQuestInfo.clUnit[questtype]);
	pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg2);

	// 클라이언트에 퀘스트의 상태를 통보한다. 
	SetUpdateSwitch(UPDATE_QUEST, true, 0);

	// 특수임무의 내용을 클라이언트에 보내서 화면에 표시할 수 있게 한다. 
	cltGameMsgResponse_SpeicalQuestMade clinfo( &pclCI->clQuestInfo.clSpecialQuestInfo);
	cltMsg clMsg3(GAMEMSG_RESPONSE_SPECIALQUESTMADE, sizeof(clinfo), (BYTE*)&clinfo);
	SendNetMsg((sPacketHeader*)&clMsg3);


	return true;
}
bool cltCharServer::MakeNamingQuest( SI32 index, SI32 level )
{
	if ( index < 0 || index >= MAX_NAMINGQUEST_INDEX )
		return false ;

	if ( level > pclClient->pclNamingQuestManager->GetMaxLevel( index ) )
		return false ;

	if ( level-1 != pclCI->clPersonNaming.m_siLevel[index] )	// 다음 단계의 퀘스트가 아니면.
		return false ;
	cltNamingQuestUnit* pclUnit = pclClient->pclNamingQuestManager->GetQuestInfo(index, level);

	if ( pclUnit == NULL )
		return false;

	// 기존 특수 퀘스트는  삭제한다. 
	pclCI->clQuestInfo.clNamingQuestInfo.Init();
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_NAMING].Init();

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clNamingQuestInfo.clQuestClearConditionInfo.Set( pclUnit->GetQuestClerCond() );

	// 보상을 구한다. 
	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siNamingIndex	= index;
	clrewardinfo.siNamingLevel	= level;

	pclCI->clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	// 현재 진행중인 퀘스트 정보를 등록한다. 
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_NAMING].siCurQuestUnique	= 1;
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_NAMING].siCurQuestStep		= QUEST_STEP_MIDDLE;
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_NAMING].siQuestCmd			= pclUnit->GetQuestClerCond()->siFlag;

	// naming - 네이밍 퀘스트 상태를 저장
	sDBRequest_SetNamingQuestInfo clMsg1(pclCI->GetPersonID(), &pclCI->clQuestInfo.clNamingQuestInfo );
	pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg1);

	// 퀘스트 상태를 DB에 저장한다.
	sDBRequest_SetQuestInfo clMsg(pclCI->GetPersonID(), QUEST_TYPE_NAMING, &pclCI->clQuestInfo.clUnit[QUEST_TYPE_NAMING]);
	pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg);

	//// 클라이언트에 퀘스트의 상태를 통보한다. 
	SetUpdateSwitch(UPDATE_QUEST, true, 0);
	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendServerResponseMsg(0, SRVAL_NAMINGQUEST_START_SUCCESS, 0, 0, GetCharID());

	return true;
}

// 2010신년이벤트 - 몬스터 사냥 퀘스트를 만든다.
bool cltCharServer::MakeQuest_RemoveUnLuckStatus_Battle()
{
	SI32 monkind	= pclClient->GetUniqueFromHash( TEXT("KIND_RACCON") );
	SI32 monnum		= 1;

	cltQuestRewardInfo clrewardinfo;

	// 레벨에 맞는 적절한 몬스터와 수량을 구한다. 
	if(pclClient->pclKindInfoSet->FindGoodMonsterForRemoveUnLuckStatusQuest(pclCI->clIP.GetLevel(),&monkind, &monnum, &clrewardinfo) == false)
	{
		return false;
	}
//#ifdef _DEBUG
//	monnum = 1;
//#endif

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_REMOVE_UNLUCK;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_KILLMON;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	monkind;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	monnum;

	// 미션 완료 제한 시간을 설정한다
	pclCI->clQuestInfo.clMissionQuestInfo.SetLeftTime( 50000 );	// 시간제한 없음 임의로 설정.

	// 보상으로 줄 아이템을 뽑는다.
	cltValuesRange* pDebuffReward = NULL;
	pDebuffReward = GetValuesWithRange(ItemValue_DebuffReward, sizeof(ItemValue_DebuffReward) / sizeof(cltValuesRange), 100);
	if(pDebuffReward == NULL)			return false;

	SI32 siItemUnique		= pDebuffReward->siPara1;	// 아이템 유니크
	SI32 siItemNum			= pDebuffReward->siPara2;	// 개수
	SI32 siUseDate			= pDebuffReward->siPara3;	// 이용기간

	// 랜덤으로 뽑은 아이템의 정보를 보상내역에 추가
	clrewardinfo.siItemUnique[0]	= ITEMUNIQUE( siItemUnique ); //해피타임 아이템
	clrewardinfo.siItemNum[0]		= siItemNum;
	clrewardinfo.siItemUseDate[0]	= siUseDate;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 2010신년이벤트 - 물품 생산하기 퀘스트를 만든다.
bool cltCharServer::MakeQuest_RemoveUnLuckStatus_MakeItem()
{
	// 1. 캐릭터 레벨을 구한다.
	// 2. 캐릭터의 레벨에 따른 생산스킬의 필요렙(기획문서에 다른)을 구한다.
	// 3. 생산스킬의 필요렙이 충족되지 못하면 물품 생산 퀘스트는 발동하지 않는다.

	SI16 siNeedMakeSkillLevel = 0;
	SI16 siCharacterLevel = pclCI->clIP.GetLevel();

	// 캐릭터 레벨 : 1~30 -> 생산퀘 발동 안함
	// 캐릭터 레벨 : 31~60 -> 필요한 생산스킬 레벨 1~ (1 이상)
	// 캐릭터 레벨 : 61~90 -> 필요한 생산스킬 레벨 20~ (20 이상)
	// 캐릭터 레벨 : 91~120 -> 필요한 생산스킬 레벨 40~ (40 이상)

	if ( 31 <= siCharacterLevel && 60 >= siCharacterLevel ) { siNeedMakeSkillLevel = 1; }
	else if ( 61 <= siCharacterLevel && 90 >= siCharacterLevel ) { siNeedMakeSkillLevel = 20; }
	else if ( 91 <= siCharacterLevel && 120 >= siCharacterLevel ) { siNeedMakeSkillLevel = 40; }
	else { return false; }

	// 캐릭터가 보유한 생산스킬의 정보(보유스킬/레벨)를 검색해 쿠키용 생퀘스트가 발동 가능한 조건을 만족하는지 체크한다.
	SI16 skilllist[] = {
		SKILL_MAKEMED,	
			SKILL_MAKEFOOD,	
			SKILL_MAKEARMOUR1,	
			SKILL_MAKEHELMET1,	
			SKILL_MAKESWORD1,	
			SKILL_MAKESPEAR1,	
			SKILL_MAKEAXE1,	
			SKILL_MAKESTAFF1,
			SKILL_MAKEBOW1,	
			SKILL_MAKEGUN1,	
			SKILL_MAKESHOES1,	
			SKILL_MAKEBELT1,	
			SKILL_MAKEDRESS1,	
			SKILL_MAKEIRON1,
			SKILL_MAKETOOL1,
			SKILL_MAKEHAT1,
			SKILL_ARTIFACT,
	};

	SI32 skillnum = sizeof(skilllist) / sizeof(SI16);

	SI16 skillbuffer[MAX_SKILL_NUMBER];
	SI16 skilllevelbuffer[MAX_SKILL_NUMBER];
	SI32 skillbufferindex = 0;
	for(SI32 i = 0;i < skillnum;i++)
	{
		SI32 tempskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(skilllist[i], 0, NULL);

		if ( siNeedMakeSkillLevel <= tempskilllevel ) 
		{
			skillbuffer[skillbufferindex]		= skilllist[i];
			skilllevelbuffer[skillbufferindex]	= tempskilllevel;
			skillbufferindex++;
		}
	}

	if(skillbufferindex == 0)
	{
		return false;
	}

	// 퀘스트 발동 조건을 만족하는 생산스킬들을 뽑아냈으면 그중 하나를 선택한다.
	SI32 index			= rand() % skillbufferindex;
	SI32 skillunique	= skillbuffer[index];
	SI32 skilllevel		= skilllevelbuffer[index];


	// 특정 기술로 만들수 있는 물품의 리스트를 구한다. 
	SI16 siitemlist[MAX_ITEMINFO_NUMBER];
	SI32 itemlistindex = 0;
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{

		if(pclClient->pclItemManager->pclItemInfo[i] == NULL)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkill != skillunique)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel > skilllevel)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel < max(1, skilllevel/2) )continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique == 0)continue;


		if(pclClient->pclItemManager->pclItemInfo[i]->CanMake_Skill(&pclCI->clCharSkill, &pclCI->clCharItem, &clPB.clTotalBA) == false)continue;

		siitemlist[itemlistindex] = pclClient->pclItemManager->pclItemInfo[i]->clItem.siUnique;
		itemlistindex++;
	}

	if(itemlistindex == 0) return false;

	// 캐릭터 레벨과 제조레벨에 맞는 무기, 장비 및 제조 수량을 구한다.
	SI32 makeitemunique = siitemlist[rand() % itemlistindex];
	SI32 makenum	= 6 + ( rand() % 15 );	// 제조 개수 6 ~ 20개.

	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)	return false;

	// 보상 내역을 설정 (일단 보류)
	cltQuestRewardInfo	clrewardinfo;

	// 보상으로 줄 아이템을 뽑는다.
	cltValuesRange* pDebuffReward = NULL;
	pDebuffReward = GetValuesWithRange(ItemValue_DebuffReward, sizeof(ItemValue_DebuffReward) / sizeof(cltValuesRange), 100);
	if(pDebuffReward == NULL)			return false;

	SI32 siItemUnique		= pDebuffReward->siPara1;	// 아이템 유니크
	SI32 siItemNum			= pDebuffReward->siPara2;	// 개수
	SI32 siUseDate			= pDebuffReward->siPara3;	// 이용기간

	// 랜덤으로 뽑은 아이템의 정보를 보상내역에 추가
	clrewardinfo.siItemUnique[0]	= ITEMUNIQUE( siItemUnique ); //해피타임 아이템
	clrewardinfo.siItemNum[0]		= siItemNum;
	clrewardinfo.siItemUseDate[0]	= siUseDate;

	// 디버프도 줄거니까 추가
	clrewardinfo.bDeBuff = true;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_REMOVE_UNLUCK;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKEITEM;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	makeitemunique;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	makenum;

	// 미션 완료 제한 시간을 설정한다
	pclCI->clQuestInfo.clMissionQuestInfo.SetLeftTime( 50000 );	// 시간제한 없음 임의로 설정.

	// 보상을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

// 2010신년이벤트 - NPC가 진행하고 있는 퀘스트를 완료하는 퀘스트를 만든다.
bool cltCharServer::MakeQuest_RemoveUnLuckStatus_NPCQuestComplete()
{
	SI32 siNPCKindUnique = -1;
	SI32 siQuestUnique = -1;

	SI16 i = 0;

	i = rand() % REMOVEUNLUCK_QUESTNPC_COUNT;

	if ( false == m_arRemoveUnluckQuest_NPC[i].IsEmpty() )
	{
		siQuestUnique = m_arRemoveUnluckQuest_Unique[i];
		siNPCKindUnique = pclClient->GetUniqueFromHash( ((TCHAR*)(m_arRemoveUnluckQuest_NPC[i])) );
	}
	else
	{
		return false;
	}

	// 	for ( i = 0; i < REMOVEUNLUCK_QUESTNPC_COUNT; ++i )
	// 	{
	// 		if ( true == m_arRemoveUnluckQuest_NPC[i].IsEmpty() ) continue;
	// 
	// 		siQuestUnique = m_arRemoveUnluckQuest_Unique[i];
	// 		if ( false == pclCI->clQuestInfo.IsClear( pclClient->pclQuestManager, siQuestUnique ) )
	// 		{
	// 			siNPCKindUnique = pclClient->GetUniqueFromHash( ((TCHAR*)(m_arRemoveUnluckQuest_NPC[i])) );
	// 			break;
	// 		}
	// 	}

	//////////////////////////////////////////////////////////////////////////

	cltQuestRewardInfo clrewardinfo;

	// 보상으로 줄 아이템을 뽑는다.
	cltValuesRange* pDebuffReward = NULL;
	pDebuffReward = GetValuesWithRange(ItemValue_DebuffReward, sizeof(ItemValue_DebuffReward) / sizeof(cltValuesRange), 100);
	if(pDebuffReward == NULL)			return false;

	SI32 siItemUnique		= pDebuffReward->siPara1;	// 아이템 유니크
	SI32 siItemNum			= pDebuffReward->siPara2;	// 개수
	SI32 siUseDate			= pDebuffReward->siPara3;	// 이용기간

	// 랜덤으로 뽑은 아이템의 정보를 보상내역에 추가
	clrewardinfo.siItemUnique[0]	= ITEMUNIQUE( siItemUnique ); //해피타임 아이템
	clrewardinfo.siItemNum[0]		= siItemNum;
	clrewardinfo.siItemUseDate[0]	= siUseDate;

	// 디버프도 줄거니까 추가
	clrewardinfo.bDeBuff = true;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique				=	MISSIONQUEST_UNIQUE_REMOVE_UNLUCK;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_CLEARQUEST;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	siQuestUnique;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	siNPCKindUnique;

	// 미션 완료 제한 시간을 설정한다
	pclCI->clQuestInfo.clMissionQuestInfo.SetLeftTime( 50000 );	// 시간제한 없음 임의로 설정.

	// 보상을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
} 

// 2010신년이벤트 - 흉 점괘가 나왔을때 수행하는 퀘스트를 만든다.
bool cltCharServer::MakeQuest_RemoveUnLuckStatus()
{
	SI32 questtype = QUEST_TYPE_MISSION1;

	// 기존 퀘스트들은 삭제한다.
	pclCI->clQuestInfo.clMissionQuestInfo.Init();
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].Init();

	bool bSuccess = false;

	srand( (unsigned)time( NULL ) );

	for( SI32 i = 0; i < 3; i++ )	// 3번까지 시도.
	{
		SI32 sirand = rand() % 3;
		switch( sirand )
		{
		case 0:
			// 전투 퀘스트.
			if( MakeQuest_RemoveUnLuckStatus_Battle() == true )bSuccess = true;
			break;
		case 1:
			// 물품 생산하기.
			if( MakeQuest_RemoveUnLuckStatus_MakeItem() == true )bSuccess = true;
			break;
		case 2:
			// NPC가 진행하는 퀘스트 완료하기.
			if( MakeQuest_RemoveUnLuckStatus_NPCQuestComplete() == true )bSuccess = true;
			break;
		default:
			bSuccess = false;
			break;
		}
		if( bSuccess ) break;
	}

	if ( bSuccess == false )
		return false;

	return true;
}

bool cltCharServer::Make_GoonzuDayMission()
{
	SI32 questtype = QUEST_TYPE_MISSION1;

	// 기존 퀘스트들은 삭제한다.
	pclCI->clQuestInfo.clMissionQuestInfo.Init();
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].Init();

	bool bSuccess = false;

	//srand( (unsigned)time( NULL ) ); // 동일한 미션 계속 받아지는 문제로 주석처리. 

	for( SI32 i = 0; i < 5; i++ )	// 3번까지 시도.
	{
		SI32 sirand = rand() % 4;
		switch( sirand )
		{
		case 0:
			// 군주데이 전투 퀘스트.
			if( Make_GoonzuDayMission_Battle() == true )bSuccess = true;
			break;
		case 1:
			// 군주데이 물품 생산하기.
			if( Make_GoonzuDayMission_MakeItem() == true )bSuccess = true;
			break;
		case 2:
			// 군주데이 재료 수집하기.
			if( Make_GoonzuDayMission_Collect() == true )bSuccess = true;
			break;
		case 3:
			// 군주데이 아이템 사용하기.
			if( Make_GoonzuDayMission_UseItem() == true )bSuccess = true;
			break;
		default:
			bSuccess = false;
			break;
		}
		if( bSuccess ) break;
	}

	if ( bSuccess == false )
		return false;

	//////////////////////////////////////////////////////////////////////////
	// 미션 완료 제한 시간을 설정한다
	pclCI->clQuestInfo.clMissionQuestInfo.SetLeftTime( 10800 );	// 미션 제한 시간 3시간. (3600 * 3)

	// 퀘스트 보상
	SI32 siLevel = 0;
	siLevel = pclCI->clIP.GetLevel();
	cltQuestRewardInfo clrewardinfo;
	if ( MAX_CHARACTER_LEVEL <= siLevel )
	{
		clrewardinfo.siItemUnique[0]	= ITEMUNIQUE(3880); // 장인의 만두
		clrewardinfo.siItemNum[0]		= 1;		// 아이템 갯수
		clrewardinfo.siExp				= 0;
	}
	else
	{
		SI64 siCalcExp = (clPB.GetNextExp() - clPB.GetPrevExp()) * 0.01;
		clrewardinfo.siExp = ( MAX_SI32< siCalcExp ) ? MAX_SI32: (SI32)siCalcExp;
		
	}	

	// 보상을 설정한다.
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);
	//////////////////////////////////////////////////////////////////////////
	

	// 디비에 저장하고 클라의 퀘스트 정보를 갱신시킨다.
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].siCurQuestUnique		= pclCI->clQuestInfo.clMissionQuestInfo.siUnique;
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].siQuestCmd			= pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag;
	pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].siCurQuestStep		= QUEST_STEP_MIDDLE;

	// 미션 퀘스트 정보를 DB에 저장한다.
	sDBRequest_SetMissionQuestInfo clMissionQuestInfo(pclCI->GetPersonID(), &pclCI->clQuestInfo.clMissionQuestInfo );
	pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMissionQuestInfo);

	// 퀘스트 상태를 DB에 저장한다.
	sDBRequest_SetQuestInfo clQuestInfo(pclCI->GetPersonID(), QUEST_TYPE_MISSION1, &pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1]);
	pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clQuestInfo);

	// 클라이언트에 퀘스트의 상태를 통보한다. 
	SetUpdateSwitch(UPDATE_QUEST, true, 0);

	// 클라에 받은 미션을 통보한다.
	cltGameMsgResponse_GoonzuDayMissionMade clinfo( &pclCI->clQuestInfo.clMissionQuestInfo );
	cltMsg clMsg3(GAMEMSG_RESPONSE_GOONZUDAYMISSION, sizeof(clinfo), (BYTE*)&clinfo);
	SendNetMsg((sPacketHeader*)&clMsg3);

	return true;
}

// 군주데이미션 - 전투퀘스트
bool cltCharServer::Make_GoonzuDayMission_Battle()
{
	SI32 monkind = pclClient->GetUniqueFromHash( TEXT ("KIND_RACCON") );
	SI32 monnum = 1;

	// 레벨에 맞는 적절한 몬스터와 수량을 구한다.
	if( pclClient->pclKindInfoSet->FindGoodMonsterForGoonzuDay(pclCI->clIP.GetLevel(),&monkind, &monnum ) == false)
	{
		return false;
	}

#ifdef _DEBUG
	monnum = 1;
#endif

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_GOONZUDAY;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_KILLMON;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	monkind;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	monnum;

	return true;
}

// 군주데이미션 - 아이템만들기.
bool cltCharServer::Make_GoonzuDayMission_MakeItem()
{
	SI16 skilllist[] = {
		SKILL_MAKEMED,	
			SKILL_MAKEFOOD,	
			SKILL_MAKEARMOUR1,	
			SKILL_MAKEHELMET1,	
			SKILL_MAKESWORD1,	
			SKILL_MAKESPEAR1,	
			SKILL_MAKEAXE1,	
			SKILL_MAKESTAFF1,
			SKILL_MAKEBOW1,	
			SKILL_MAKEGUN1,	
			SKILL_MAKESHOES1,	
			SKILL_MAKEBELT1,	
			SKILL_MAKEDRESS1,	
			SKILL_MAKEIRON1,
			SKILL_MAKETOOL1,
			SKILL_MAKEHAT1,
			SKILL_ARTIFACT
	};


	SI32 skillnum = sizeof(skilllist) / sizeof(SI16);

	SI16 skillbuffer[MAX_SKILL_NUMBER];
	SI16 skilllevelbuffer[MAX_SKILL_NUMBER];
	SI32 skillbufferindex = 0;
	for(SI32 i = 0;i < skillnum;i++)
	{
		SI32 tempskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(skilllist[i], 0, NULL);
		if(tempskilllevel == 0)continue;

		skillbuffer[skillbufferindex]		= skilllist[i];
		skilllevelbuffer[skillbufferindex]	= tempskilllevel;
		skillbufferindex++;
	}

	if(skillbufferindex == 0)
	{
		return false;
	}

	SI32 index			= rand() % skillbufferindex;
	SI32 skillunique	= skillbuffer[index];
	SI32 skilllevel		= min( 45, skilllevelbuffer[index] );


	// 특정 기술로 만들수 있는 물품의 리스트를 구한다. 
	SI16 siitemlist[MAX_ITEMINFO_NUMBER];
	SI32 itemlistindex = 0;
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{

		if(pclClient->pclItemManager->pclItemInfo[i] == NULL)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkill != skillunique)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel > skilllevel)continue;
		//if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel < max(1, skilllevel/2) )continue;		// 제조레벨 없는 아이템을 만드는게 퀘스트 조건으로 나올수도 있다면 이거 주석
		if(pclClient->pclItemManager->pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique == 0)continue;


		if(pclClient->pclItemManager->pclItemInfo[i]->CanMake_Skill(&pclCI->clCharSkill, &pclCI->clCharItem, &clPB.clTotalBA) == false)continue;

		siitemlist[itemlistindex] = pclClient->pclItemManager->pclItemInfo[i]->clItem.siUnique;
		itemlistindex++;
	}

	if(itemlistindex == 0)return false;

	SI32 makeitemunique = siitemlist[rand() % itemlistindex];
	//SI32 makenum	= 3 + ( rand() % 3 ); // 제조 개수 3~5개.

	SI32 makenum = 2 + ( rand() % 7 );	  // 제조 개수 2~ 8개.

//#ifdef _DEBUG
//	SI32 makenum = 1;	
//#endif

	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)	return false;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_GOONZUDAY;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKEITEM;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	makeitemunique;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	makenum;

	return true;
}

// 군주데이미션 - 아이템수집.
bool cltCharServer::Make_GoonzuDayMission_Collect()
{
	SI32 itemunique	= 0;
	SI32 itemnum	= 1;

	SI16 itemtypelist[] = {
		ITEMTYPE_MINERAL,	
			ITEMTYPE_CLOTH,
			ITEMTYPE_TREE,
			ITEMTYPE_FOOD,
			ITEMTYPE_HEAL,
			ITEMTYPE_SEAFOOD,
			ITEMTYPE_ETC,
	};

	SI32 itemtypelistnum = sizeof(itemtypelist) / sizeof(SI16);

	SI16 itemtype = itemtypelist[ rand() % itemtypelistnum];


	cltQuestRewardInfo clrewardinfo;

	// 내 인벤에 있는경우는 다시 찾는다.
	for ( SI32 i=0; i < 3; ++i  )
	{
		itemunique = pclClient->pclMarketManager->GetRandomOrder( itemtype );

		if(itemunique <= 0) continue;

		if( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_NOTDAILYQUEST ) ) continue;

		// 내 인벤에 없는 경우에만 더이상 찾기 중지;
		if(pclCI->clCharItem.FindItemInv(itemunique) == false ) 
			break;
	}

	if ( 0 >= itemunique ) return false;

	itemnum = 5 + (rand() % 16);	// 수집개수 5 ~ 20개.

//#ifdef _DEBUG
//	itemnum = 1;	
//#endif

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_GOONZUDAY;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_ITEM;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	itemunique;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	itemnum;

	return true;
}

// 군주데이미션 - 아이템사용.
bool cltCharServer::Make_GoonzuDayMission_UseItem()
{
	SI32 siitemlist[] ={
		ITEMUNIQUE(2565),	ITEMUNIQUE(2570),	ITEMUNIQUE(2575),	ITEMUNIQUE(2685),
			ITEMUNIQUE(2580),	ITEMUNIQUE(2585),	ITEMUNIQUE(2590),	ITEMUNIQUE(2595),
			ITEMUNIQUE(2600),	ITEMUNIQUE(2605),	ITEMUNIQUE(2631),
			ITEMUNIQUE(2635),	ITEMUNIQUE(2675),	ITEMUNIQUE(2680),	ITEMUNIQUE(2681),	

			ITEMUNIQUE(3735),	ITEMUNIQUE(3740),	ITEMUNIQUE(3745),	ITEMUNIQUE(3750),
			ITEMUNIQUE(3755),	ITEMUNIQUE(3760),	ITEMUNIQUE(3765),	ITEMUNIQUE(3770),
			ITEMUNIQUE(3775),	ITEMUNIQUE(3780),	ITEMUNIQUE(3785),	ITEMUNIQUE(3790),
			ITEMUNIQUE(3795),	ITEMUNIQUE(3800),	ITEMUNIQUE(3805),	ITEMUNIQUE(3810),
			ITEMUNIQUE(3815),	ITEMUNIQUE(3820),	ITEMUNIQUE(3825),	ITEMUNIQUE(3830),
			ITEMUNIQUE(3835),	ITEMUNIQUE(3840),
	};

	SI32 itemlistnum	= sizeof(siitemlist) / sizeof(SI32);
	if(itemlistnum <= 0)return false;

	SI16 itemunique = siitemlist[ rand() % itemlistnum];

	SI16 itemnum = 5 + ( rand() % 16); // 사용 개수 5 ~ 20 개.

//#ifdef _DEBUG
//	SI16 itemnum	= 1;
//#endif

	if ( 0 >= itemunique ) return false;

	cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, itemunique);
	if(pclprice == NULL)return false;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_GOONZUDAY;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_USEITEM;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	itemunique;
	pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	itemnum;

	return true;
}

//KHY - 0822 - 해피빈 퀘스트 추가.
bool cltCharServer::MakeHappyBeanQuest()
{
	SI32 questtype = QUEST_TYPE_HAPPYBEAN;

	// 기존  해피빈 퀘스트 삭제한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.Init();
	pclCI->clQuestInfo.clUnit[questtype].Init();

	bool bsuccess = false;

	srand( (unsigned)time( NULL ) );

	for ( SI32 i = 0; i < 5 ; ++i ) // 5번까지 시도
	{
		// 확률 분포를 늘이기 위해 코딩 - by LEEKH 2007.09.04
		SI32 sirand = rand() % (4 * 100);//4; //퀘스트 겟수에 따라 조절.

		if(sirand >= 300)
		{			
			// 전투 퀘스트.
			if(MakeHappyBeanQuest_Battle() == true)bsuccess = true;
		}
		else if(sirand >= 200)
		{
			// 기술 올리기 
			if(MakeHappyBeanQuest_Skill() == true)bsuccess = true;
		}
		else if(sirand >= 100)
		{
			// 물품 가져오기
			if(MakeHappyBeanQuest_GetItem() == true)bsuccess = true;
		}
		else
		{
			// 물품 생산하기
			if(MakeHappyBeanQuest_MakeItem() == true)bsuccess = true;
		}

		if ( bsuccess  ) break;
	}

	if ( bsuccess == false )
		return false;

	//cltQuestInfoCommon* pclquestinfo = &pclCI->clQuestInfo.clHappyBeanQuestInfo;

	//// 현재 진행중인 퀘스트 정보를 등록한다. 
	//pclCI->clQuestInfo.clUnit[questtype].siCurQuestUnique	= 1;
	//pclCI->clQuestInfo.clUnit[questtype].siQuestCmd		= pclquestinfo->clQuestClearConditionInfo.siFlag;
	//pclCI->clQuestInfo.clUnit[questtype].siCurQuestStep	= QUEST_STEP_MIDDLE;

	//// 해피빈 퀘스트 정보를 저장한다. 
	//sDBRequest_SetHappyBeanQuestInfo clMsg1(pclCI->GetPersonID(), &pclCI->clQuestInfo.clHappyBeanQuestInfo );
	//pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg1);

	//// 퀘스트 상태를 DB에 저장한다.
	//sDBRequest_SetQuestInfo clMsg(pclCI->GetPersonID(), questtype, &pclCI->clQuestInfo.clUnit[questtype]);
	//pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg);

	//// 클라이언트에 퀘스트의 상태를 통보한다. 
	//SetUpdateSwitch(UPDATE_QUEST, true, 0);
	return true;
}


//KHY - 0822 - 해피빈 퀘스트 추가. 전투.
bool cltCharServer::MakeHappyBeanQuest_Battle()
{
	SI32 monkind	= pclClient->GetUniqueFromHash( TEXT("KIND_RACCON") );
	SI32 monnum		= 1;

	cltQuestRewardInfo clrewardinfo;

	// 레벨에 맞는 적절한 몬스터와 수량을 구한다. 
	if(pclClient->pclKindInfoSet->FindGoodMonsterForHappyBeanQuest(pclCI->clIP.GetLevel(),&monkind, &monnum, &clrewardinfo) == false)
	{
		return false;
	}
#ifdef _DEBUG
	monnum = 3;
#endif

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_KILLMON;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara1	=	monkind;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara2	=	monnum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}


//KHY - 0822 - 해피빈 퀘스트 추가. 스킬업.
bool cltCharServer::MakeHappyBeanQuest_Skill()
{

	//적절한 기술을 찾는다. 
	SI32 skillunique;		// 올려야 할 기술종류. 
	SI32 upgradeskilllevel;	// 올려야 할 기술 정도. 

	srand( (unsigned)time( NULL ) );

#ifdef _SAFE_MEMORY
	if(pclCI->clCharSkill.GetRandSkillForUpgrade(pclClient->pclSkillManager, &pclCI->clCharItem.clItem[0], &skillunique, &upgradeskilllevel/*, NULL*/) == false)return false;
#else
	if(pclCI->clCharSkill.GetRandSkillForUpgrade(pclClient->pclSkillManager, pclCI->clCharItem.clItem, &skillunique, &upgradeskilllevel/*, NULL*/) == false)return false;
#endif

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siItemUnique[0]	= ITEMUNIQUE(2770); //해피타임 아이템
	clrewardinfo.siItemNum[0]		= 1;

	SI32 sirand = rand() % 2; //+ 0 or +1

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_SKILL;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara1	=	skillunique;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara2	=	upgradeskilllevel+sirand; //스킬 레벨을 1~2올린다.

	// 보상을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;

}


//KHY - 0822 - 해피빈 퀘스트 추가. 물품 가져오기.
bool cltCharServer::MakeHappyBeanQuest_GetItem()
{
	SI32 itemunique	= 0;
	SI32 itemnum	= 10;

	SI16 itemtypelist[] = {
		ITEMTYPE_MINERAL,	
			ITEMTYPE_CLOTH,
			ITEMTYPE_TREE,
	};

	srand( (unsigned)time( NULL ) );

	SI32 itemtypelistnum = sizeof(itemtypelist) / sizeof(SI16);

	SI16 itemtype = itemtypelist[ rand() % itemtypelistnum];


	// 내 인벤에 있는경우는 다시 찾는다.
	for ( SI32 i=0; i < 3; ++i  )
	{
		itemunique = pclClient->pclMarketManager->GetRandomOrder( itemtype );

		if(itemunique <= 0)
		{
			return false;
		}

		// 내 인벤에 없는 경우에만 더이상 찾기 중지;
		if(pclCI->clCharItem.FindItemInv(itemunique) == false ) 
			break;
	}

	SI32 sirand = rand() % 20; //+ 0 or +19

#ifdef _DEBUG
	itemnum = 3;
#else
	itemnum = itemnum +sirand ;
#endif

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siItemUnique[0]	= ITEMUNIQUE(2770); //해피타임 아이템
	clrewardinfo.siItemNum[0]		= 1;

	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_ITEM;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara1	=	itemunique;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara2	=	itemnum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;
}

//KHY - 0822 - 해피빈 퀘스트 추가. 물품 생산하기
bool cltCharServer::MakeHappyBeanQuest_MakeItem()
{

	SI16 skilllist[] = {
		SKILL_MAKEMED,	
			SKILL_MAKEFOOD,	
			SKILL_MAKEARMOUR1,	
			SKILL_MAKEHELMET1,	
			SKILL_MAKESWORD1,	
			SKILL_MAKESPEAR1,	
			SKILL_MAKEAXE1,	
			SKILL_MAKESTAFF1,
			SKILL_MAKEBOW1,	
			SKILL_MAKEGUN1,	
			SKILL_MAKESHOES1,	
			SKILL_MAKEBELT1,	
			SKILL_MAKEDRESS1,	
			SKILL_MAKEIRON1,
			SKILL_MAKETOOL1,
			SKILL_MAKEHAT1,
			SKILL_ARTIFACT,
	};


	SI32 skillnum = sizeof(skilllist) / sizeof(SI16);

	SI16 skillbuffer[MAX_SKILL_NUMBER];
	SI16 skilllevelbuffer[MAX_SKILL_NUMBER];
	SI32 skillbufferindex = 0;
	for(SI32 i = 0;i < skillnum;i++)
	{
		SI32 tempskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(skilllist[i], 0, NULL);
		if(tempskilllevel == 0)continue;

		skillbuffer[skillbufferindex]		= skilllist[i];
		skilllevelbuffer[skillbufferindex]	= tempskilllevel;
		skillbufferindex++;
	}

	if(skillbufferindex == 0)
	{
		return false;
	}

	SI32 index			= rand() % skillbufferindex;
	SI32 skillunique	= skillbuffer[index];
	SI32 skilllevel		= skilllevelbuffer[index];


	// 특정 기술로 만들수 있는 물품의 리스트를 구한다. 
	SI16 siitemlist[MAX_ITEMINFO_NUMBER];
	SI32 itemlistindex = 0;
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{

		if(pclClient->pclItemManager->pclItemInfo[i] == NULL)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkill != skillunique)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel > skilllevel)continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clSkillInfo.siSkillLevel < max(1, skilllevel/2) )continue;
		if(pclClient->pclItemManager->pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique == 0)continue;


		if(pclClient->pclItemManager->pclItemInfo[i]->CanMake_Skill(&pclCI->clCharSkill, &pclCI->clCharItem, &clPB.clTotalBA) == false)continue;

		siitemlist[itemlistindex] = pclClient->pclItemManager->pclItemInfo[i]->clItem.siUnique;
		itemlistindex++;
	}

	if(itemlistindex == 0)return false;

	SI32 makeitemunique = siitemlist[rand() % itemlistindex];
	SI32 makenum	= 1 + ( rand() % 3 );

	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique(makeitemunique);
	if(ref <= 0)	return false;

	cltQuestRewardInfo clrewardinfo;
	clrewardinfo.siItemUnique[0]	= ITEMUNIQUE(2770); //해피타임 아이템
	clrewardinfo.siItemNum[0]		= 1;


	// 퀘스트의 종료 조건을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKEITEM;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara1	=	makeitemunique;
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara2	=	makenum;

	// 보상을 구한다. 
	pclCI->clQuestInfo.clHappyBeanQuestInfo.clQuestRewardInfo.Set(&clrewardinfo);

	return true;

}

bool cltCharServer::MakeMissionQuest(SI32 siMissionUnique)
{
	switch(siMissionUnique)
	{
	case MISSIONQUEST_UNIQUE_HOLLOWEEN:
		{
			MakeMissionQuest_Holloween();
		}
		break;
	case MISSIONQUEST_UNIQUE_REMOVE_UNLUCK:
		{
			MakeQuest_RemoveUnLuckStatus();
		}
		break;
	}

	return true;
}

enum HOLLOWEEN_MISSION
{
	HOLLOWEEN_MISSION_NONE,
	HOLLOWEEN_MISSION_HUNT,
	HOLLOWEEN_MISSION_MAKEITEM
};
bool cltCharServer::MakeMissionQuest_Holloween()
{
	bool bMakeQuest = false;
	SI32 siHolloweenMission = HOLLOWEEN_MISSION_NONE;
	SI32 siLevel = pclCI->clIP.GetLevel();
	SI32 i;

	SI32 siRand = rand() % 100;
	if(siRand >= 0 && siRand < 50)
	{
		siHolloweenMission = HOLLOWEEN_MISSION_HUNT;
	}
	else if (siRand >= 50 && siRand < 100)
	{
		siHolloweenMission = HOLLOWEEN_MISSION_MAKEITEM;
	}

	// 1 ~ 60 레벨까지는 무조건 사냥 퀘스트를 부여한다.
	if( siLevel >= 1 && siLevel <= 60 )		siHolloweenMission = HOLLOWEEN_MISSION_HUNT;

	switch(siHolloweenMission)
	{
	case HOLLOWEEN_MISSION_HUNT:
		{
			SI32 siMinLevel = 0, siMaxLevel = 0;
			SI32 siKinds[512];
			SI32 siValidKinds[512];
			SI32 siValidKindCount = 0;
			SI32 siSelectedCharKind = 0;
			SI32 siSelectedCharLevel = 0;

			if		( siLevel >=  1 && siLevel <=  30 )		{	siMinLevel =  1;		siMaxLevel = 30;	}
			else if ( siLevel >= 31 && siLevel <=  60 )		{	siMinLevel = 31;		siMaxLevel = 60;	}
			else if ( siLevel >= 61 && siLevel <=  90 )		{	siMinLevel = 60;		siMaxLevel = 90;	}
			else if ( siLevel >= 91 && siLevel <= 100 )		{	siMinLevel = 60;		siMaxLevel = 100;	}
			else if ( siLevel >= 101 && siLevel <= 120 )	{	siMinLevel = 100;		siMaxLevel = 120;	}

			ZeroMemory(siKinds, sizeof(siKinds));
			ZeroMemory(siValidKinds, sizeof(siValidKinds));

			cltNPCManagerServer* pclNPCManagerServer = (cltNPCManagerServer*)pclClient->pclNPCManager;
			if(pclNPCManagerServer)
			{
				pclNPCManagerServer->GetKindLevel(siMinLevel, siMaxLevel, siKinds, 512);
				for(i=0; i<512; i++)
				{
					if( pclClient->pclKindInfoSet->IsValidKind(siKinds[i]) == false )		continue;
					if( pclClient->pclKindInfoSet->pclKI[siKinds[i]]->IsAtb(ATB_BOSS) )		continue;
					if( pclClient->pclKindInfoSet->pclKI[siKinds[i]]->IsAtb(ATB_DAILYQUEST | ATB_DAILYQUEST2) == false )	continue;

					siValidKinds[siValidKindCount] = siKinds[i];
					siValidKindCount++;
				}

				if( siValidKindCount > 0 )
				{
					SI32 siSelect = rand() % siValidKindCount;
					siSelectedCharKind = siValidKinds[siSelect];
					siSelectedCharLevel = pclClient->pclNPCManager->GetKindMaxLevel(siSelectedCharKind);
				}
			}

			if( siSelectedCharKind > 0 && siSelectedCharLevel > 0 )
			{
				// 기존 미션 퀘스트를 삭제한다.
				pclCI->clQuestInfo.clMissionQuestInfo.Init();
				pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].Init();

				SI32 siKillNum = (rand() % 41) + 10;	// 10 ~ 50 랜덤

				SI32 siCandyCountForLevel	= 1;
				SI32 siCandyCountForKillNum = 1;

				SI32 siDiffLevel = siSelectedCharLevel - siLevel;

				SI32 siQuestTimeSec = 1;

				if( siDiffLevel < -10 )							{	siCandyCountForLevel = 1;	}
				if( siDiffLevel >= -10 && siDiffLevel <= 10)	{	siCandyCountForLevel = 2;	}
				if( siDiffLevel > 10 )							{	siCandyCountForLevel = 3;	}

				if( siKillNum == 10 )							{	siCandyCountForKillNum = 1;	}
				if( siKillNum >= 11 && siKillNum <= 30 )		{	siCandyCountForKillNum = 2;	}
				if( siKillNum >= 31 && siKillNum <= 50 )		{	siCandyCountForKillNum = 3;	}

				if( siKillNum == 10 )							{	siQuestTimeSec = 15 * 60;	}
				if( siKillNum >= 11 && siKillNum <= 30 )		{	siQuestTimeSec = 30 * 60;	}
				if( siKillNum >= 31 && siKillNum <= 50 )		{	siQuestTimeSec = 60 * 60;	}

				// 새로운 퀘스트를 설정한다.
				pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_HOLLOWEEN;
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_KILLMON;
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	siSelectedCharKind;
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	siKillNum;

				pclCI->clQuestInfo.clMissionQuestInfo.SetLeftTime( siQuestTimeSec );

				pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemUnique[0]	= ITEMUNIQUE(13050);	// 사탕
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemNum[0]	= min(siCandyCountForLevel, siCandyCountForKillNum);

				bMakeQuest = true;
			}
		}
		break;
	case HOLLOWEEN_MISSION_MAKEITEM:
		{
			SI32 siMinSkillLevel = 0, siMaxSkillLevel = 0;
			SI16 siitemlist[MAX_ITEMINFO_NUMBER];
			SI32 itemlistindex = 0;

			if		( siLevel >= 61 && siLevel <=  90 )		{	siMinSkillLevel = 1;		siMaxSkillLevel = 20;		}
			else if	( siLevel >= 91 && siLevel <= 100 )		{	siMinSkillLevel = 21;		siMaxSkillLevel = 100;		}
			else if	( siLevel >= 101 && siLevel <= 120 )	{	siMinSkillLevel = 21;		siMaxSkillLevel = 100;		}

			for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
			{
				cltItemInfo* pclItemInfo = pclClient->pclItemManager->pclItemInfo[i];

				if(pclItemInfo == NULL)												continue;

				if(pclItemInfo->clSkillInfo.siSkillLevel < siMinSkillLevel )		continue;
				if(pclItemInfo->clSkillInfo.siSkillLevel > siMaxSkillLevel )		continue;
				if(pclItemInfo->clProductionInfo.clMaterial[0].siUnique == 0)		continue;
				// PCK : 아이템중에서 기타와 이벤트 아이템은 제외한다. (09.08.28)
				if(pclItemInfo->siType == ITEMTYPE_ETC)								continue;
				if(pclItemInfo->siType == ITEMTYPE_EVENTITEM)						continue;

				siitemlist[itemlistindex] = pclItemInfo->clItem.siUnique;
				itemlistindex++;
			}

			SI32 siSelectedItemUnique = 0;
			SI32 siSelectedItemNum = 0;
			if( itemlistindex > 0 )
			{
				siSelectedItemUnique = siitemlist[rand() % itemlistindex];
				siSelectedItemNum = (rand() % 15) + 6;			// 6 ~ 20개
			}

			if( siSelectedItemUnique > 0 && siSelectedItemNum > 0 )
			{
				SI32 siQuestTimeSec = 60 * 60;		// 60분

				// 새로운 퀘스트를 설정한다.
				pclCI->clQuestInfo.clMissionQuestInfo.siUnique							=	MISSIONQUEST_UNIQUE_HOLLOWEEN;
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag	=	QUEST_CLEAR_COND_MAKEITEM;
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1	=	siSelectedItemUnique;
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2	=	siSelectedItemNum;

				pclCI->clQuestInfo.clMissionQuestInfo.SetLeftTime( siQuestTimeSec );

				pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemUnique[0]	= ITEMUNIQUE(13050);	// 사탕
				pclCI->clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemNum[0]	= (rand() % 2) + 2;		// 2 ~ 3개

				bMakeQuest = true;
			}
		}
		break;
	}

	return bMakeQuest;
}