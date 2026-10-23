//---------------------------------
// 2003/8/7 김태곤
//---------------------------------
#include <Stdio.h>
#include "cltItem.h"

#include "../../../CommonLogic/CommonLogic.h"

//-----------------------------
// Common
//-----------------------------
#include "../../../common/PersonInfo.h"
#include "../../../common/Char\CharManager\CharManager.h"
#include "../../../common/Skill\Skill-Manager.h"
#include "../../../common/Char\Disease\Disease.h"
#include "../../../common/Util\Util.h"
#include "../../../common/Char\CharCommon\Char-Common.h"
#include "../../../common/DailyQuest3/DailyQuest3Mgr.h"


#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"

#include "..\Server\Server.h"
#include "..\Server\Minister\MinisterBase.h"
#include "..\Server\Minister\Minister-Izo.h"
#include "../../../Server/TraceBoot.h"
#include "..\Server\Minister\MinisterMgr.h"

#include "../Server/GachaManager/GachaManager.h"
#include "../Server/Gacha2Manager/Gacha2Manager.h"


extern cltCommonLogic* pclClient;

SI32 siBigWennyItemList[]=
{
	ITEMUNIQUE(1000),	// 초심자의목걸이[제조]
	ITEMUNIQUE(1010),	// 초심자의목걸이[무기]
	ITEMUNIQUE(1020)	// 초심자의목걸이[생산]
};

SI32 siBigWarItemList[]=
{
	ITEMUNIQUE(5350),	// 불의속성석
	ITEMUNIQUE(5355),	// 물의속성석
	ITEMUNIQUE(5360),	// 바람의속성석
	ITEMUNIQUE(5365)	// 땅의속성석
};

SI32 siBigChristmas[]=
{
	ITEMUNIQUE(7150),	// 
	ITEMUNIQUE(7160),	// 
	ITEMUNIQUE(7170)	// 
};


cltItemManagerCommon::cltItemManagerCommon()
{
	SI32 i;

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{
		pclItemTypeInfo[i] = NULL;
	}

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		pclItemInfo[i] = NULL;
	}


	for(i = 0;i < MAX_ITEM_UNIQUE;i++)
	{
		ItemRef[i] = 0;
	}

	pclItemPrice				= NULL;
	pclItemNewPrice				= NULL;
	pcltSetItemAbilityManager	= NULL;
	pclSkillBookManager			= NULL;
	// [성웅] 레벨 보상 상자에서 아이템을 준다 
	pclRewardItemLimitLevelForChina	=	NULL	;
	siItemNum					= 0;
	bCanMakeRareItemSwitch		= true;
	dwLastMakeRareItemClock		= 0;
	m_bcheckcnppricerateswitch = false;

	for(i = 0;i < 9; i++)
	{
		clMarketConditions[i] = NULL;
	}


/*	for ( i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
	{
		stInvestmentLines[i].siInvestmentItemUnique = 0 ;
		stInvestmentLines[i].siInvestmentItemPrice = 0 ;

		stInvestmentLines[i].siMarginalProfit = 0 ;

		for ( j = 0 ; j < MAX_MATERIAL_NUMBER ; j ++ )
		{
			stInvestmentLines[i].siMaterialItemUnique[j] = 0 ;
			stInvestmentLines[i].siMaterialItemNeedNum[j] = 0 ;
			stInvestmentLines[i].siMaterialItemPrice[j] = 0 ;
		}
	}*/

	// ItemInfoAtb 등록
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_ITEMMALL"),					ITEMINFOATB_ITEMMALL );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_CANUSEITEM"),				ITEMINFOATB_CANUSEITEM );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_NOTDAILYQUEST"),			ITEMINFOATB_NOTDAILYQUEST );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_NOTWENNYOLDMAN"),			ITEMINFOATB_NOTWENNYOLDMAN );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_NOTTRADE"),					ITEMINFOATB_NOTTRADE );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_FIRSTACTION"),				ITEMINFOATB_FIRSTACTION );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_SPECIAL_JAPAN"),			ITEMINFOATB_SPECIAL_JAPAN );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_SPECIAL_GLOBAL"),			ITEMINFOATB_SPECIAL_GLOBAL );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_SPECIAL_KOREA"),			ITEMINFOATB_SPECIAL_KOREA );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_WENNYOLDMAN_MATERIAL"),		ITEMINFOATB_WENNYOLDMAN_MATERIAL );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_NOTSELL"),					ITEMINFOATB_NOTSELL );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_NOTSHOW_MARKETCONFITIONS"),	ITEMINFOATB_NOTSHOW_MARKETCONFITIONS );

	
	// CANUSEITEM_TYPE 등록
	ItemInfoAtbParser.AddAtbInfo( TEXT("CANUSEITEM_TYPE_EAST"),					CANUSEITEM_TYPE_EAST );
	ItemInfoAtbParser.AddAtbInfo( TEXT("CANUSEITEM_TYPE_WEST"),					CANUSEITEM_TYPE_WEST );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_CRYSTAL_SLOT_CREATE"),		ITEMINFOATB_CRYSTAL_SLOT_CREATE );
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_CRYSTAL_REMOVE"),			ITEMINFOATB_CRYSTAL_REMOVE );
	
	//[진성] 레어 소환인형 설정(사용: 1.수련장 등록 금지.) => 2008-7-15
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_RARE_SUMMONDOLL"),			ITEMINFOATB_RARE_SUMMONDOLL );
	//KHY - 20090526 - 아바타 결정체.
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_CRYSTAL_AVATAR"),			ITEMINFOATB_CRYSTAL_AVATAR );
	
	//[진성] 스킬북
	ItemInfoAtbParser.AddAtbInfo( TEXT("ITEMINFOATB_SKILLBOOK"),				ITEMINFOATB_SKILLBOOK );

	

}

cltItemManagerCommon::~cltItemManagerCommon()
{
	SI32 i ,x;

	if(pclItemPrice)
	{
		delete pclItemPrice;
		pclItemPrice = NULL;
	}

	if( pclItemNewPrice )
	{
		delete pclItemNewPrice;
		pclItemNewPrice = NULL;
	}
		
	if( pcltSetItemAbilityManager )
	{
		delete pcltSetItemAbilityManager;
		pcltSetItemAbilityManager = NULL;
	}

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i])
		{
			delete pclItemInfo[i];
			pclItemInfo[i] = NULL;
		}
	}

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{
		if(pclItemTypeInfo[i])
		{
			delete pclItemTypeInfo[i];
			pclItemTypeInfo[i] = NULL;
		}
	}

	for(i = 0;i < 9;i++)
	{
		if(clMarketConditions[i])
		{
			for(x = 0;x < MAX_ITEM_UNIQUE;x++)
			{
				if(clMarketConditions[i]->pclMarketConditionsB[x])
				{
					delete clMarketConditions[i]->pclMarketConditionsB[x];
					clMarketConditions[i]->pclMarketConditionsB[x] = NULL;
				}
			}

			delete clMarketConditions[i];
			clMarketConditions[i] = NULL;
		}
	}

	if( pclSkillBookManager )
	{
		delete pclSkillBookManager;
		pclSkillBookManager = NULL;
	}
// [성웅] 레벨 보상 상자에서 아이템을 준다 
	if(pclRewardItemLimitLevelForChina)// 중국레벨 보상 아이템
	{
		delete pclRewardItemLimitLevelForChina	;
		pclRewardItemLimitLevelForChina	=	NULL	;
	}
	

	if( pclSetItemManager)
	{
		delete pclSetItemManager;
		pclSetItemManager = NULL;
	}

}
/*
bool cltItemManagerCommon::GetLimitDateItemValue(SI32 unique , SI32* usedate)
{
	if(unique < 0)return false;
	if(unique >= MAX_ITEM_UNIQUE)return false;

	cltServer* pclserver = (cltServer*)pclClient;

	if ( unique > 0 )
	{
		SI32 ref = ItemRef[unique] ;
		if ( ref > 0 )
		{
			SI32 UseDate = pclItemInfo[ref]->siUseDate ;
			if ( UseDate )
				*usedate = pclserver->pclTime->clDate.GetDateVary() + UseDate ;
			else
				*usedate = 0 ;

			if ( *usedate > 0 )
				return true ;
			else return false ;
		}
	}
	
	return false ;
}
*/
void cltItemManagerCommon::Create()
{
	// 아이템의 비트맵 이미지 정보를 설정한다. (클라이언트 전용) 
	//if ( pclClient->siServiceArea == ConstServiceArea_Korea )
	//{
		SetBitmapImage();
	TraceBoot("C2 a after SetBitmapImage");
		SetItemImageBigList();	//[진성] 아아템 큰 이미지 세팅. => 2008-9-23
	TraceBoot("C2 b after SetItemImageBigList");
	//}

	// 아이템 타입을 설정한다. 
	SetItemType();
	TraceBoot("C2 c after SetItemType");
	
	// 아이템 정보를 설정한다. 
	SetItemData();
	TraceBoot("C2 d after SetItemData");

	// 아이템의 재료와 도구를 설정한다. 
	SetItemMaterial();
	TraceBoot("C2 e after SetItemMaterial");

	pclItemPrice	= new cltItemPrice(this);
	pclItemNewPrice = new cltItemPrice(this);
	TraceBoot("C2 f after item price managers");
	
	if( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		//[진성] 세트 아이템 단계결 능력치 정보 초기화
		pcltSetItemAbilityManager = new cltSetItemAbilityManager;
		pcltSetItemAbilityManager->Init();
	}

	// [진성] 스킬북
	pclSkillBookManager = new cltSkillBookManager;
	pclSkillBookManager->Init();
	TraceBoot("C2 g after pclSkillBookManager->Init");
	
	pclSetItemManager = new CSetItemManager;
	pclSetItemManager->LoadSetItemInfo();
	TraceBoot("C2 h after pclSetItemManager->LoadSetItemInfo");
	// [성웅] 레벨 보상 상자에서 아이템을 준다 
	if ( pclClient->IsCountrySwitch(Switch_LevelLimitBox) ) 
	{
		pclRewardItemLimitLevelForChina = new cltRewardItemForLimitLevel	;
		pclRewardItemLimitLevelForChina->init()	;
	}
#ifdef _DEBUG
	//// 같은 이름이 존재하는 아이템이 있는지 검색한다.
	//for( SI32 i=0; i<MAX_ITEMINFO_NUMBER; i++ )
	//{
	//	if( pclItemInfo[i] == NULL )			continue;
	//	
	//	for( SI32 j=0; j<MAX_ITEMINFO_NUMBER; j++ )
	//	{
	//		if( pclItemInfo[j] == NULL )												continue;
	//		if( pclItemInfo[i]->clItem.siUnique == pclItemInfo[j]->clItem.siUnique )	continue;
	//		if( _tcscmp( pclItemInfo[i]->GetName(), pclItemInfo[j]->GetName() ) == 0 )
	//		{
	//			NTCHARString256 kMsg;
	//			kMsg += "유니크[";
	//			kMsg += SI16ToString(pclItemInfo[i]->clItem.siUnique);
	//			kMsg += "]와 ";
	//			kMsg += "유니크[";
	//			kMsg += SI16ToString(pclItemInfo[j]->clItem.siUnique);
	//			kMsg += "] 의 ";
	//			kMsg += "이름이 같습니다.";
	//			MessageBox(NULL, kMsg, "ITEMNAME ERROR", MB_OK);
	//		}
	//	}
	//}
#endif
}

// 아이템타입, 속성, 소속, 타입코드, 이름
BOOL cltItemManagerCommon::SetItemType(SI32 itemtype, SI64 siatb, SI64 siattach, const TCHAR* typecode, const TCHAR* pname)
{

	if(itemtype < 0 || itemtype >= MAX_ITEMTYPE_NUMBER)
	{
		TCHAR buffer[256];
		StringCchPrintf(buffer, 256, TEXT("SetItemType(SI32 itemtype, SI64 siatb) %d %d"), itemtype, siatb);
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Error"), buffer, MB_OK|MB_TOPMOST);
		return FALSE;
	}

	if(pclItemTypeInfo[itemtype] == NULL)
	{
		pclItemTypeInfo[itemtype] = new cltItemTypeInfo(itemtype, siatb, siattach, typecode, pname);
		return TRUE;
	}

	return FALSE;
}

// 아이템의 이름으로 아이템의 유니크 값을 구한다. 
SI32 cltItemManagerCommon::FindUniqueFromName(const TCHAR* pname)
{
	SI32 i;
	
	if( NULL == pname)
		return 0;

	if ( !_tcsicmp(TEXT("NULL"),pname)) return 0;

	for(i = 1;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i])
		{
			if(_tcscmp(pclItemInfo[i]->GetName() , pname) == 0)
			{
				return pclItemInfo[i]->clItem.siUnique;
			}
		}
	}

	return 0;
}

SI08 cltItemManagerCommon::FindMakeAniTypeFromUnique(SI32 siUnique)
{

	if(siUnique == 0)return 0;

	SI08 makeanytype = MAKE_ANI_ARMOR;
	SI32 itemtype = GetItemType(siUnique);

	switch(itemtype)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_NECK:
	case ITEMTYPE_RING:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:
	case ITEMTYPE_MANTLE:
	case ITEMTYPE_AVATAR_MANTLE:
	case ITEMTYPE_AVATAR_NECK:
	case ITEMTYPE_AVATAR_RING:
	case ITEMTYPE_AVATAR_DRESS:
	case ITEMTYPE_AVATAR_HAT:
	case ITEMTYPE_MINERAL:
		{
			makeanytype = MAKE_ANI_ARMOR;
			// 방어구					
		}
		break;
	case ITEMTYPE_HEAL:						
		{
			// 약
			makeanytype = MAKE_ANI_HEAL;

		}
		break;
	case ITEMTYPE_FOOD:
	case ITEMTYPE_SEAFOOD:
		{
			makeanytype = MAKE_ANI_FOOD;
		}
		break;
	case ITEMTYPE_CRYSTAL://아티팩트
		{
			makeanytype = ENCHANT_ANI_WEPONE;
		}
		break;
	case ITEMTYPE_DOLL:
		{
			makeanytype = MAKE_ANI_HEAL;
		}
		break;

	default:
		{
			makeanytype = MAKE_ANI_ARMOR;
		}
		break;
	}

	return makeanytype;
}

SI08 cltItemManagerCommon::FindEnchantAniTypeFromUnique(SI32 siUnique)
{
	if(siUnique == 0)return 0;

	SI08 enchantanitype = ENCHANT_ANI_WEPONE;
	SI32 itemtype = GetItemType(siUnique);

	switch(itemtype)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
		{
			//무기 인첸트
			enchantanitype = ENCHANT_ANI_WEPONE;				
		}
		break;	

	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_NECK:
	case ITEMTYPE_RING:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:
	case ITEMTYPE_MANTLE:
		{
			//방어구 인첸트
			enchantanitype = ENCHANT_ANI_ARMOR;
		}
		break;
	default:
		{
			enchantanitype = ENCHANT_ANI_WEPONE;
		}
		break;
	}

	return enchantanitype;

}

// 아이템의 타입이름을 근거로 아이템타입의 유니크값을 구한다.
SI32 cltItemManagerCommon::FindItemTypeUniqueFromTypeName(const TCHAR* itemtypename)
{
	SI32 i;

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{
		if(pclItemTypeInfo[i])
		{
			if(_tcscmp(pclItemTypeInfo[i]->GetName(), itemtypename) == 0)
			{
				return i;
			}
		}
	}

	return -1;

}

// 아이템의 타입코드를  근거로 아이템타입의 유니크값을 구한다.
SI32 cltItemManagerCommon::FindItemTypeUniqueFromTypeCode(const TCHAR* itemtypeCode)
{
	SI32 i;

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{
		if(pclItemTypeInfo[i])
		{
			if(_tcscmp(pclItemTypeInfo[i]->szTypeCode, itemtypeCode) == 0)
			{
				return i;
			}
		}
	}

	return -1;

}
// 떨어뜨려야 하는 아이템인지 아닌지 결정 한다
bool cltItemManagerCommon::IsCanDropItem( SI32 siItemUniuqe ){
	// 아이템 몰 아이템은 무조건 이름을 써서 파괴 한다.
	// 거래 불가 아이템을 이름을 써서 파괴 한다.
	// 위 사양이 아닌 경우에는 그냥 파괴한다.
	// 이름을 써서 파괴 할경우는 false
	// 그냥 파괴 할경우 true
	// 받은 아이템 유니크로 해당 ref 를 구한다.
	SI32 ref = FindItemRefFromUnique( siItemUniuqe );
	if(ref <= 0)return false; 
	// 파괴 되야 하는 아이템					->	봉인 풀린 장비 아이템,이건 따로 결정 한다.
	
	// 파괴 안되고 드롭되는 아이템				->	트레이트 가능한 아이템,
	// 파괴 안되고 드롭도 안되야 하는 아이템	->	봉인이 안풀린 트레이드 가능한 장비 아이템, 일반 재료 아이템중에 트레이드 가능 아이템

	// ref를 토대로 해당 아이템의 item이 트래이드 불가능 아이템일 때 는 드롭하지 않는다.
	if( pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_NOTTRADE ) )		return false ;
	//// ref를 토대로 해당 아이템의 item이 아이템몰 아이템일 때
	//if( pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_ITEMMALL ) )		return false ;
	
	// 트레이드 가능한 아이템일 경우에는 떨어뜨린다.
	else																return true ;
}
SI32 cltItemManagerCommon::AddItem(SI32 imageindex, SI32 smallimageindex, TCHAR* name, SI32 sitype, SI32 weight,
								   cltItemSkillInfo* pclskillinfo, cltItemConition* pclcondition, TCHAR* armcode,
								   TCHAR* material, TCHAR* tool,SI32 productunit, SI32 soundtype, cltItem* pclitem,
								   SI32 Grade, SI32 MinNum,cltCrystalItemData *CrystalItemData, bool bNewMarket, bool ImmutableUseDate)
{
	SI32 i;
	if(pclitem->siUnique <= 0)
	{
		MsgBox(TEXT("fsdf"), TEXT("Vsdf:%d"), pclitem->siUnique);
	}


	for(i = 1;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i])continue;
		
#ifdef _DEBUG
		TCHAR szName[64];
		if( _tcscmp( name, "NULL" ) == 0 )
		{
			StringCchPrintf(szName, sizeof(szName), "%s(%d)", "NoItemText", pclitem->siUnique );
			name = szName;
		}
#endif
		pclItemInfo[i] = new cltItemInfo(imageindex, smallimageindex, name, sitype, weight, pclskillinfo,
										 pclcondition, armcode, material, tool, productunit, soundtype, pclitem, Grade, MinNum, CrystalItemData, bNewMarket, ImmutableUseDate);
		
		// Unique값을 근거로 쉽게 Ref를 찾는다.
		ItemRef[pclitem->siUnique] = i;
		
		// 아이템의 개수를 증가시킨다. 
		siItemNum++;
		
		return i;
	}

	MsgBox(TEXT("Item Num Exceed"), TEXT("Item Num Exceed"));

	return -1;

}

// 아이템의 적절한 위치인지 확인한다. 
bool cltItemManagerCommon::CheckProperItemPos(cltItem* pclItem, SI32 siDragTo, SI32 siKind)
{
	// [영훈] 소환수 장비착용 : 아래 코드에 주석처리 되어있는것은 모두 필요없는 것들이지만 아직 만약을 우해 놔둠

	if(pclItem == NULL)return false;

	// 해당 아이템의 타입을 구한다.
	SI32 itemtype = pclItem->GetType(this);
	if(itemtype ==0)return false;

		
	// 장착 불가 품목은 리펀 false 
	SI32 ref = FindItemRefFromUnique(pclItem->siUnique);
	if (ref <= 0) return	false;

	bool bCanEquip = true;

	switch(siDragTo)
	{
	case PERSONITEM_WEAPON1:
	case PERSONITEM_WEAPON2:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;

		bCanEquip = pclItem->IsCorrectPrivateEquip(this, siKind);

		//cyj 동양, 서양 전용일때 조건 확인(소환수는 적용안함)
		if (bCanEquip == false)
			return false;

		// 무기 속성을 가진 아이템이면 오케이.
		if(pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE)return true;
		if(itemtype == ITEMTYPE_FISHINGROD)	return true;
		if(itemtype == ITEMTYPE_IRON)		return true;
		
		
		//cyj 화살 및 탄환 장착 가능하도록 수정
		if(pclItem->IsBulletItem())	return true;

		break;

		/*
	case PERSONITEM_WEAPON1 + PERSONITEM_SUMMON1EQUIP0:
	case PERSONITEM_WEAPON1 + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		// 무기 속성을 가진 아이템이면 오케이.
		if(pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE)return true;
		if(itemtype == ITEMTYPE_FISHINGROD)	return true;
		if(itemtype == ITEMTYPE_IRON)		return true;
		//cyj 화살 및 탄환 장착 가능하도록 수정
		if(pclItem->IsBulletItem())	return true;
		break;

	case PERSONITEM_WEAPON2 + PERSONITEM_SUMMON1EQUIP0:
	case PERSONITEM_WEAPON2 + PERSONITEM_SUMMON2EQUIP0:
		return false;
		break;
	*/
	case PERSONITEM_HELMET:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;

		bCanEquip = pclItem->IsCorrectPrivateEquip(this, siKind);

		//cyj 동양, 서양 전용일때 조건 확인(소환수는 적용안함)
		if (bCanEquip == false)
			return false;

		if(itemtype == ITEMTYPE_HELMET)return true;
		break;

	/*
	case PERSONITEM_HELMET + PERSONITEM_SUMMON1EQUIP0:
	case PERSONITEM_HELMET + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_HELMET)return true;
		break;
	*/

	
	case PERSONITEM_HAT:
	//case PERSONITEM_HAT + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_HAT + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_HAT || itemtype == ITEMTYPE_AVATAR_HAT )return true;
		break;
	
	case PERSONITEM_ARMOUR:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;

		bCanEquip = pclItem->IsCorrectPrivateEquip(this, siKind);

		//cyj 동양, 서양 전용일때 조건 확인(소환수는 적용안함)
		if (bCanEquip == false)
			return false;

		if(itemtype == ITEMTYPE_ARMOUR)return true;
		break;

	/*
	case PERSONITEM_ARMOUR + PERSONITEM_SUMMON1EQUIP0:
	case PERSONITEM_ARMOUR + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_ARMOUR)return true;
		break;
	*/
	case PERSONITEM_DRESS:
	//case PERSONITEM_DRESS + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_DRESS + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_DRESS || itemtype == ITEMTYPE_AVATAR_DRESS)return true;
		break;

	case PERSONITEM_NECK:
	//case PERSONITEM_NECK + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_NECK + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_NECK || itemtype == ITEMTYPE_AVATAR_NECK)return true;
		break;

	case PERSONITEM_BELT:
	//case PERSONITEM_BELT + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_BELT + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_BELT)return true;
		break;

	case PERSONITEM_RING1:
	case PERSONITEM_RING2:
	//case PERSONITEM_RING1 + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_RING1 + PERSONITEM_SUMMON2EQUIP0:
	//case PERSONITEM_RING2 + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_RING2 + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_RING || itemtype == ITEMTYPE_AVATAR_RING)return true;
		break;

	case PERSONITEM_SHOES:
	//case PERSONITEM_SHOES + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_SHOES + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_SHOES)return true;
		break;

	case PERSONITEM_MANTLE:
	//case PERSONITEM_MANTLE + PERSONITEM_SUMMON1EQUIP0:
	//case PERSONITEM_MANTLE + PERSONITEM_SUMMON2EQUIP0:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_MANTLE || itemtype == ITEMTYPE_AVATAR_MANTLE )return true; // 아바타 망토
		break;

	case PERSONITEM_BAG:
	case PERSONITEM_BAG_SUB:
		if(pclItemInfo[ref]->bCanEquipSwitch == false)return false;
		if(itemtype == ITEMTYPE_BAG)return true;
		break;

	/*
	case PERSONITEM_BAG + PERSONITEM_SUMMON1EQUIP0:
	case PERSONITEM_BAG + PERSONITEM_SUMMON2EQUIP0:
	case PERSONITEM_BAG_SUB + PERSONITEM_SUMMON1EQUIP0:
	case PERSONITEM_BAG_SUB + PERSONITEM_SUMMON2EQUIP0:
		return false;
		break;
	*/

	default:			// 인벤토리. 
		return true;
		
	}

	return false;
}


// siUnique아이템의 공격 사거리를 구한다. 
SI32 cltItemManagerCommon::GetAttackRange(SI32 unique)
{
	SI32 ref = FindItemRefFromUnique(unique);
	if(ref > 0)
	{
		return pclItemInfo[ref]->GetAttackRange();
	}

	return 0;
}

// 아이템의 시세를 설정한다. 
void cltItemManagerCommon::SetCurrentPrice(SI32 itemunique, GMONEY price, SI32 itemnum, bool bforceswitch)
{
	pclItemPrice->SetPrice(this, itemunique, price, itemnum, bforceswitch);
}

void cltItemManagerCommon::SetCurrentNewPrice(SI32 itemunique, SI64 price, SI32 itemnum, bool bforceswitch)
{
	pclItemNewPrice->SetPrice(this, itemunique, price, itemnum, bforceswitch );
}

// 아이템의 시세를 얻어온다.
cltItemPriceUnit* cltItemManagerCommon::GetCurrentPrice(SI32 itemnunique)
{
	return pclItemPrice->GetPrice(this, itemnunique);
}

cltItemPriceUnit* cltItemManagerCommon::GetCurrentNewPrice(SI32 itemnunique)
{
	return pclItemNewPrice->GetPrice(this, itemnunique);
}


// Unique를 근거로 Atb를 구한다. 
SI64 cltItemManagerCommon::GetItemAtb(SI32 siunique)
{
	if(siunique == 0)return 0;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)return 0;

	// 아이템의 Type을 구한다. 
	SI32 type = pclItemInfo[ref]->siType;

	// 아이템 Type의 Atb를 구한다. 
	SI64 atb = pclItemTypeInfo[type]->siAtb;

	return atb;

}

SI32 cltItemManagerCommon::GetItemGrade(SI32 ItemUnique)
{
	if(ItemUnique == 0)return 0;
	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(ItemUnique);

	if(ref <= 0)return 0;

	SI32 maxItemGrade = pclItemInfo[ref]->siItemGrade;

	return maxItemGrade;


}

// Unique를 근거로 Type를 구한다. 
SI32 cltItemManagerCommon::GetItemType(SI32 siunique)
{
	if(siunique == 0)return 0;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)return 0;

	// 아이템의 Type을 구한다. 
	return pclItemInfo[ref]->siType;

}

// Unique를 근거로 Atb를 구한다. 
SI64 cltItemManagerCommon::GetItemAttach(SI32 siunique)
{
	if(siunique == 0)return 0;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)return 0;

	// 아이템의 Type을 구한다. 
	SI32 type = pclItemInfo[ref]->siType;

	// 아이템 Type의 Attach를 구한다. 
	SI64 adaptattach = pclItemTypeInfo[type]->siAttach;

	return adaptattach;

}

// 해당 Scroll 이 아이템에 붙일수 있는지 확인한다.
BOOL cltItemManagerCommon::CanItemAttach(SI32 scrollunique, SI32 tounique)
{
	if(scrollunique == 0)	return FALSE;
	if(tounique == 0)		return FALSE;

	// 아이템의 ref을 구한다. 
	SI32 scrollref = FindItemRefFromUnique(scrollunique);
	if(scrollref <= 0)return FALSE;
	
	SI32 toref = FindItemRefFromUnique(tounique);
	if(toref <= 0)return FALSE;

	// 아이템의 Type을 구한다. 
	SI32 adaptattach = pclItemInfo[scrollref]->siScrollAdaptAttach;

	SI32 type = pclItemInfo[toref]->siType;

	switch(adaptattach)
	{
		case ITEMATTACH_SKILLUPDRESS:
			{
				if(pclItemInfo[toref]->siUpgradeSkillType & UPGRADESKILLTYPE_MAKE)
				{
					/// 올릴 수 있는 한도가 있어야 성공. 
					if (type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT)
					{
						if(pclItemInfo[toref]->clItem.clItemStatusArmour.uiSkillUp)
						{
							return TRUE;
						}
					}
					else
					{
						if(pclItemInfo[toref]->clItem.clItemArmour.uiSkillUp)
						{
							return TRUE;
						}
					}
				}
			}
			break;
		case ITEMATTACH_USEWEAPONUPDRESS:
			{
				if(pclItemInfo[toref]->siUpgradeSkillType & UPGRADESKILLTYPE_USEWEAPON)
				{
					/// 올릴 수 있는 한도가 있어야 성공. 
					if (type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT)
					{
						if(pclItemInfo[toref]->clItem.clItemArmour.uiSkillUp)
						{
							return TRUE;
						}
					}
					else
					{
						if(pclItemInfo[toref]->clItem.clItemStatusArmour.uiSkillUp)
						{
							return TRUE;
						}
					}
				}
			}
			break;

		default:
			{
				SI64 typeattach = pclItemTypeInfo[type]->siAttach;

				if(typeattach & adaptattach) return TRUE;
			}
			break;
	}
	

	return FALSE;


}

// unique를 근거로 이름을 구한다. 
const TCHAR* cltItemManagerCommon::GetName(SI32 siunique)
{
	if(siunique == 0)return NULL;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)return NULL;

	return pclItemInfo[ref]->GetName();
}



// 이미지의 인덱스를 가져온다. 
SI32 cltItemManagerCommon::						GetImage(SI32 siunique)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref > 0)
	{
		return pclItemInfo[ref]->siImageIndex;
	}

	return 0;
}

// 아이템 장착 조건을 충족시키는지 확인한다. 
// 2003.10.23
bool cltItemManagerCommon::CheckCondition(SI32 siitemunique, cltCharCommon* pclchar, SI32* returnval)
{
	SI32 ref = FindItemRefFromUnique(siitemunique);
	if(ref <= 0)return false;

	SI32 totalskilllevel = 0;
	if(pclItemInfo[ref]->clCondition.siSkillKind)
	{
		totalskilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(pclItemInfo[ref]->clCondition.siSkillKind, 0, 0, &pclchar->pclCI->clCharItem.clItem[0], pclchar->GetID());
	}

	// 스킬 필요없는 낚시 퀘스트 수행시 강제로 스킬 레밸을 10으로 만듬
	if ( totalskilllevel < 10 )
	{
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_FISHNOSKILL;
		if(pclchar->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			totalskilllevel = 10;
		}
	}

	// 소환수인경우 스킬레벨은 무조건 100으로
	if ( IDENTITY_PERSON_SUMMON == pclchar->clIdentity.siIdentity )
	{
		totalskilllevel = 100;
	}
	
	// [영훈] 아이템 체크시 행운에 영향
	SI32 siHndOrLuk = 0;
	if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
	{
		siHndOrLuk = pclchar->clPB.clTotalBA.GetLuk();
	}
	else
	{
		siHndOrLuk = pclchar->clPB.clTotalBA.GetHnd();
	}

	// 전체능력치가 설정되지 않은 상태에선 능력치에 영향을 받지 않도록 한다. - by LEEKH 2009-03-20
	if( pclchar->clPB.bSetTotalBA == false )
	{
		siHndOrLuk = MAX_SI32;

		return pclItemInfo[ref]->clCondition.Check(pclItemInfo[ref]->siType,
			pclchar->GetKind(),
			pclchar->pclCI->clIP.GetLevel(), 
			MAX_SI32,
			MAX_SI32,
			MAX_SI32,
			MAX_SI32,
			siHndOrLuk,
			totalskilllevel, 
			pclchar->pclCI->clIP.siFameLevel,
			returnval);
	}

	return pclItemInfo[ref]->clCondition.Check(pclItemInfo[ref]->siType,
		pclchar->GetKind(),
		pclchar->pclCI->clIP.GetLevel(), 
		pclchar->clPB.clTotalBA.GetStr(),
		pclchar->clPB.clTotalBA.GetDex(),
		pclchar->clPB.clTotalBA.GetMag(),
		pclchar->clPB.clTotalBA.GetVit(),
		siHndOrLuk,
		totalskilllevel, 
		pclchar->pclCI->clIP.siFameLevel,
		returnval);
}

/*
bool cltItemManagerCommon::CheckCondition(SI32 siitemunique, cltSummon* pclsummon, SI32* returnval,SI32 addstr,SI32 adddex,SI32 addmag)
{
	SI32 ref = FindItemRefFromUnique(siitemunique);
	if(ref <= 0)return false;

	if(pclsummon == NULL)return false;

	//----------------------------------------------
	// 자격이 되는 소환 영웅에게만 붙일 수 있다.
	//----------------------------------------------
	if(pclsummon->siKind == 0 || pclClient->pclKindInfoSet->pclKI[pclsummon->siKind] == NULL 
		|| pclClient->pclKindInfoSet->pclKI[pclsummon->siKind]->bSummonMall == false)
	{
		*returnval = ITEMEQUIP_FAIL_NOSUMMONHERO;
		return false;
	}

	SI32 limitlevel = 100 ;
#ifdef _DEBUG
	limitlevel = 1 ;
#endif
	if(pclsummon->clIP.GetLevel() < limitlevel )
	{
		*returnval = ITEMEQUIP_FAIL_OUTOFSUMMONLEVEL;
		return false;
	}


	SI32 totalskilllevel = 100;

	return pclItemInfo[ref]->clCondition.Check(pclItemInfo[ref]->siType,
		pclsummon->siKind,
		pclsummon->clIP.GetLevel(), 
		addstr,
		adddex,
		addmag,
		pclsummon->clIP.GetVit(),
		0,
		totalskilllevel, 
		100,		// 명성 레벨은 100으로 설정해준다. (모든 옷을 다 입을 수 있다. ) 
		returnval);

}
*/
// 소환수가 해당 아이템을 장착 가능한지				
bool cltItemManagerCommon::CheckConditionForSummon( IN cltCharCommon* pclChar, IN SI32 siItemUnique, IN cltSummon *pclsummon, IN cltItem* pclItemList, OUT SI32 *psiReturnValue )
{
	if ( (NULL == pclsummon) || (NULL == psiReturnValue) )
	{
		return false;
	}

	if ( pclsummon->siStatus != SUMMON_STATUS_TAKEOUT )
	{
		return false;
	}
	
	SI32 siRef = FindItemRefFromUnique( siItemUnique );
	if ( 0 >= siRef )
	{
		return false;
	}

	SI32 siSummonID = pclChar->GetSummonID();
	if ( false == pclClient->pclCM->IsValidID(siSummonID) )
	{
		return false;
	}

	cltCharCommon* pclSummonChar = (cltCharCommon*)pclClient->pclCM->CR[siSummonID];
	if ( NULL == pclSummonChar )
	{
		return false;
	}

	bool bCheckLimitForLevel	= false;
	//SI32 siSummonLevel			= pclsummon->clIP.GetLevel();
	SI16 siAttackWearStoneNum	= pclsummon->siAttackWearStoneNum;
	SI16 siDefenseWearStoneNum	= pclsummon->siDefenseWearStoneNum;

	// 장비석에 관한 실제 검사는 아래에서 하고 여기에서는 단순히 있는지 없는지만 검사
	if ( (0 < siAttackWearStoneNum) || (0 < siDefenseWearStoneNum) )
	{
		bCheckLimitForLevel = true;
	}

	// 장착가능레벨을 없앰
	/*
	// 장비석관련조건이 충족되지 않고, 장비를 착용할수없는 최소레벨보다 적으면 
	if ( (false == bCheckLimitForLevel) && (SUMMON_EQUIP_LIMITMINLEVEL > siSummonLevel) )
	{
		*psiReturnValue = ITEMEQUIP_FAIL_OUTOFSUMMONLEVEL;
		return false;
	}
	*/

	// 소환수 Kind Info 가져오기
	cltKindInfo* pclki = NULL;
	cltGMCommandKindInfo* pclGMki = NULL;

	if ( pclClient->IsCountrySwitch(Switch_Summon_Separate) ) 
	{
		pclki = pclClient->pclKindInfoSet->pclSummonKI[pclsummon->siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclsummon->siKind];
	}
	else
	{
		pclki = pclClient->pclKindInfoSet->pclKI[pclsummon->siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclsummon->siKind];
	}

	if ( NULL == pclki )
	{
		return false;
	}

	if ( NULL == pclGMki)
	{
		return false;
	}

	// 소환수 장비 장착타입 검색
	SI16 siSummonEquipType	= pclki->siSummonEquip;
	SI32 siItemType			= pclItemInfo[siRef]->siType;
	SI32 siNeedLevel		= pclItemInfo[siRef]->clCondition.siLevel;


	// 장비를 착용할 수 없는 소환수라면
	if ( SUMMON_EQUIPTYPE_NONE == siSummonEquipType )
	{
		return false;
	}

	// 아이템 타입에 따른 착용여부
	switch	( siItemType ) 
	{
		// 근거리 무기일때는 장비 착용타입을 검사한다
		case ITEMTYPE_SWORD:
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_AXE:
			{
				if ( SUMMON_EQUIPTYPE_FARDISTANCE != siSummonEquipType )
				{
					*psiReturnValue = ITEMEQUIP_FAIL_WRONGEQUIPTYPE;
					return false;
				}

				else if ( /*(SUMMON_EQUIP_LIMITMINLEVEL > siSummonLevel) && */(siNeedLevel > siAttackWearStoneNum) )
				{
					*psiReturnValue = ITEMEQUIP_FAIL_NOTENOUGHSTONE;
					return false;
				}
			}
			break;

		// 원거리 무기일때는 장비 착용타입을 검사한다
		case ITEMTYPE_BOW:
		case ITEMTYPE_GUN:
		case ITEMTYPE_STAFF:
			{
				if ( SUMMON_EQUIPTYPE_LONGDISTANCE != siSummonEquipType )
				{
					*psiReturnValue = ITEMEQUIP_FAIL_WRONGEQUIPTYPE;
					return false;
				}

				if ( /*(SUMMON_EQUIP_LIMITMINLEVEL > siSummonLevel) && */(siNeedLevel > siAttackWearStoneNum) )
				{
					*psiReturnValue = ITEMEQUIP_FAIL_NOTENOUGHSTONE;
					return false;
				}
			}
			break;

		// 방어구
		case ITEMTYPE_HELMET:
		case ITEMTYPE_ARMOUR:
		case ITEMTYPE_BELT:
		case ITEMTYPE_SHOES:
		case ITEMTYPE_MANTLE:
			{
				if ( /*(SUMMON_EQUIP_LIMITMINLEVEL > siSummonLevel) && */(siNeedLevel > siDefenseWearStoneNum) )
				{
					*psiReturnValue = ITEMEQUIP_FAIL_NOTENOUGHSTONE;
					return false;
				}
			}
			break;

	}
	
	// 아이템을 실질적으로 착용할수있는지 검사
	cltParametaBox	clPB;
	cltPIPInfo		clPIPInfo;

	clPIPInfo.Set( &pclsummon->clIP );
	clPB.Init( pclClient->pclItemManager, &clPIPInfo, NULL, pclki, pclGMki );

	clPB.Action( pclSummonChar, 0, PERSONITEM_WEAPON1, pclItemList, &pclSummonChar->pclCI->clHorseInfo, &pclSummonChar->pclCI->clHealthInfo, &pclSummonChar->pclCI->clCharSkill, &pclSummonChar->clIdentity, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );
	
	return pclItemInfo[siRef]->clCondition.Check(pclItemInfo[siRef]->siType,
		pclsummon->siKind,
		pclsummon->clIP.GetLevel(), 
		clPB.clTotalBA.GetStr(),
		clPB.clTotalBA.GetDex(),
		clPB.clTotalBA.GetMag(),
		pclsummon->clIP.GetVit(),
		0,
		100,		// 스킬레벨은 관계없이 착용할 수 있다
		100,		// 명성 레벨은 100으로 설정해준다. (모든 옷을 다 입을 수 있다. ) 
		psiReturnValue);


}

bool cltItemManagerCommon::IsCanEquipItemPosForSummon( SI32 siEquipPosition, SI32 siItemUnique )
{
	switch ( siEquipPosition ) 
	{
		// 착용 가능한 아이템 위치
		case PERSONITEM_HELMET:		// 투구
		case PERSONITEM_ARMOUR:		// 갑옷
		case PERSONITEM_WEAPON1:	// 무기1
		case PERSONITEM_BELT:		// 벨트
		case PERSONITEM_SHOES:		// 신발
		case PERSONITEM_MANTLE:		// 망토
			{
				return true;
			}
			break;

		case PERSONITEM_WEAPON2:	// 무기2 장착창은 총알과 화살만
			{
				switch ( siItemUnique )
				{
				case ITEMUNIQUE(7850):	// 화살
				case ITEMUNIQUE(7855):	// 총알
				case ITEMUNIQUE(13600):	// 과금화살
				case ITEMUNIQUE(13601):	// 과금화살
					{
						return true;
					}
					break;
				}
			}
			break;
	}

	return false;
}

// iconcode를 근거로 
SI32 cltItemManagerCommon::FindBitmapIndex(TCHAR* typecode, TCHAR* iconcode)
{
	TCHAR buffer[256];

	StringCchPrintf(buffer, 256, TEXT("Item\\%s\\%s.bmp"), typecode, iconcode);
	return GetBitmapIndexFromBitmapName(buffer);

}


// 아이템(약)과 치료할 수 있는 질병을 연결한다. 
void cltItemManagerCommon::ConnectDisease( cltDiseaseManager *pcldiseasemanager)
{
	SI32 i = 0;
	
	for ( i = 0; i < MAX_ITEMINFO_NUMBER; ++i )
	{
		if ( pclItemInfo[ i ] )
		{
			if ( _tcscmp( pclItemInfo[ i ]->szDiseaseName, TEXT("") ) != 0
			&&	 _tcscmp( pclItemInfo[ i ]->szDiseaseName, TEXT("NULL") ) != 0)
			{		
				pclItemInfo[ i ]->siDiseaseUnique = pcldiseasemanager->FindUniqueFromName( pclItemInfo[ i ]->szDiseaseName );
			}
		}
	}
	
	return;
}

// 아이템(약)과 치료할 수 있는 질병타입을 연결한다. 
void cltItemManagerCommon::ConnectDiseaseType( cltDiseaseManager *pcldiseasemanager)
{
	SI32 i = 0;

	for ( i = 0; i < MAX_ITEMINFO_NUMBER; ++i )
	{
		if ( pclItemInfo[ i ] )
		{
			if ( _tcscmp( pclItemInfo[ i ]->szDiseaseType, TEXT("") ) != 0
				&&	 _tcscmp( pclItemInfo[ i ]->szDiseaseType, TEXT("NULL") ) != 0)
			{	
				// 질병관련 아이템 테이블에서 해당 질병에 대한 유니크 = 질병테이블에서 질병이름.
				pclItemInfo[ i ]->siDiseaseUnique = pcldiseasemanager->FindUniqueFromDiseaseType( pclItemInfo[ i ]->szDiseaseType );
			}
		}
	}

	return;
}

// 부적을 부착한다. 
bool cltItemManagerCommon::Enchant(cltItem* pequipclitem, SI32 scrollunique, SI32* prtnval, SI32* ppara1, SI32* ppara2)
{
	if(scrollunique <= 0)return false;

	// 부적의 내용을 확인한다. 
	cltItemInfo* pclscrollinfo = FindItemInfo(scrollunique);
	if(pclscrollinfo == NULL)return false;

	// 부적의 기능을 확인한다. 
	SI32 function	= pclscrollinfo->siScrollFunction;
	if(function <= 0)return false;

	SI32 ability	= pclscrollinfo->siScrollAbility;
	if(ability <= 0)return false;

	// 이 아이템이 부적의 기능에 부합하는가 ?
	if(CanItemAttach(scrollunique, pequipclitem->siUnique)== FALSE)
	{
		*prtnval	= ENCHANT_FAIL_REASON_NOTATBMATCH;
		*ppara1		= pclscrollinfo->siScrollMax;
		*ppara2		= 0;
		return false;
	}

	//==================================================================
	// 최대 내구도를 높여주는 Scroll 이라면 기존 Scroll 시스템에 누적되지 않는다.
	//==================================================================
	if(pclscrollinfo->siScrollAdaptAttach == ITEMATTACH_DURABLE)
	{
		UI32 uiMaxDurability = (UI32)pclClient->GetGlobalValue( TEXT("GV_MaxDurability") );
		if(pequipclitem->clItemCommon.clCommonInfo.uiMaxDurability >= (uiMaxDurability/2))
		{
			*prtnval	= ENCHANT_FAIL_REASON_NOMORE;
			*ppara1		= uiMaxDurability/2;
			*ppara2		= 2;		
			return false;
		}

		if ( pequipclitem->GetType(pclClient->pclItemManager) == ITEMTYPE_DRESS ||
			pequipclitem->GetType(pclClient->pclItemManager) == ITEMTYPE_HAT ||
			pequipclitem->GetType(pclClient->pclItemManager) == ITEMTYPE_NECK )
		{
			*prtnval	= ENCHANT_FAIL_REASON_NOTATBMATCH;
			*ppara1		= pclscrollinfo->siScrollMax;
			*ppara2		= 0;
			return false;
		}

		// 11 Bit 데이터가 갖을수 있는 최대 내구도를 넘지 않도록 한다.
		SI32 siMaxDurabe = min(uiMaxDurability, (UI32)(pequipclitem->clItemCommon.clCommonInfo.uiMaxDurability + ability));
		pequipclitem->clItemCommon.clCommonInfo.uiMaxDurability = siMaxDurabe;
		return true;
	}
	//==================================================================

	// 이 아이템의 내구도를 낮춘다.
	if((SI32)pequipclitem->GetDurability()  <= pclscrollinfo->siScrollDecDuration)return false;
	pequipclitem->DecreaseDurability(this, pclscrollinfo->siScrollDecDuration);

	SI32 siEnchantMax = 0;
	if(pclscrollinfo->siScrollAdaptAttach == ITEMATTACH_SKILLUPDRESS)
	{
		cltItemInfo* pcllinfo = FindItemInfo(pequipclitem->siUnique);
		if(pcllinfo == NULL)return false;
		siEnchantMax = pequipclitem->clItemArmour.uiSkillUp * 10;
		*ppara2		 = 1;
	}
	else if(pclscrollinfo->siScrollAdaptAttach == ITEMATTACH_USEWEAPONUPDRESS)
	{
		cltItemInfo* pcllinfo = FindItemInfo(pequipclitem->siUnique);
		if(pcllinfo == NULL)return false;
		siEnchantMax = pequipclitem->clItemArmour.uiSkillUp * 10;
		*ppara2		 = 1;
	}

	else
	{
		siEnchantMax	= pclscrollinfo->siScrollMax;
		*ppara2		 = 0;
	}

	SI32 temp;
	
	// 같은 기능이 이미 있다면 
	if((SI32)pequipclitem->clItemCommon.clOptionInfo.uiType1 == function)
	{
		temp = pequipclitem->clItemCommon.clOptionInfo.uiPara1;
		temp += ability;

		if(temp > siEnchantMax)
		{
			*prtnval	= ENCHANT_FAIL_REASON_NOMORE;
			*ppara1		= siEnchantMax;
			return false;
		}

		temp = min(1000, temp);

		pequipclitem->clItemCommon.clOptionInfo.uiPara1 = temp;
		
	}
	else if((SI32)pequipclitem->clItemCommon.clOptionInfo.uiType2 == function)
	{
		temp = pequipclitem->clItemCommon.clOptionInfo.uiPara2;
		temp += ability;

		if(temp > siEnchantMax)
		{
			*prtnval	= ENCHANT_FAIL_REASON_NOMORE;
			*ppara1		= siEnchantMax;
			return false;
		}

		temp = min(1000, temp);

		pequipclitem->clItemCommon.clOptionInfo.uiPara2 = temp;
	}
	else if(pequipclitem->clItemCommon.clOptionInfo.uiType1 == 0)
	{
		pequipclitem->clItemCommon.clOptionInfo.uiType1	= function;

		temp = min(1000, ability);

		pequipclitem->clItemCommon.clOptionInfo.uiPara1	= temp;

	}
	else if(pequipclitem->clItemCommon.clOptionInfo.uiType2 == 0)
	{

		pequipclitem->clItemCommon.clOptionInfo.uiType2	= function;

		temp = min(1000, ability);

		pequipclitem->clItemCommon.clOptionInfo.uiPara2	= temp;
	}
	else
	{
		*prtnval	= ENCHANT_FAIL_REASON_NOMOREKIND;
		*ppara1		= siEnchantMax;
		return false;
	}

	return true;
}

// 특정 물품을 특정 캐릭터가 만들때 품질 지수를 구한다. 
SI32 cltItemManagerCommon::GetQualityRate(SI32 itemunique, SI32 id, SI32* addlevel)
{
	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return 0;

	SI32 qualityrrate = 100;

	//--------------------------------------------
	// 기술에 의한 품질 지수 변동 
	//--------------------------------------------
	SI32 needskillunique	= pclItemInfo[ref]->clSkillInfo.siSkill;
	if(needskillunique > 0)
	{
		//-------------------------------------------
		// 아이템 제작에 필요한 스킬레벨과 캐릭터의 스키레벨의 차이를 구한다.
		//-------------------------------------------
		SI32 skilllevel			= pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel(needskillunique, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0]);
		*addlevel			= max(0, ( skilllevel - pclItemInfo[ref]->clSkillInfo.siSkillLevel));

		//-------------------------------------------
		// 손재주에 의한 품질 지수 변동 
		//-------------------------------------------
		SI32 addhnd;

		//-------------------------------------------
		// 제조 활성화 변경에 의해 손재주 값을 사용
		//-------------------------------------------
		if(pclClient->IsCountrySwitch(Switch_ManufactureRevitalize))
		{
			if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )
				addhnd =  pclClient->pclCM->CR[id]->clPB.clTotalBA.GetLuk();
			else
				addhnd =  pclClient->pclCM->CR[id]->clPB.clTotalBA.GetHnd();
		}
		else
		{
			if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )
				addhnd =  pclClient->pclCM->CR[id]->clPB.clTotalBA.GetLuk();
			else
			{
				SI32 needhnd	=  pclItemInfo[ref]->clSkillInfo.siHnd;
				addhnd =  max(0, pclClient->pclCM->CR[id]->clPB.clTotalBA.GetHnd() - needhnd);
			}
		}

		if(*addlevel > 8)
		{
			qualityrrate += 25;
		}
		else	
		{
			qualityrrate += ( *addlevel ) * 3;
		}

		// 손재주를 1000자리부터다!!(차이나는 손재주를 1000자리로 밀어준다. 나중에 1000을 나눠서 값을 알아낸다)
		(*addlevel) += 1000*addhnd;
	}

	return min(125, qualityrrate);
	/*	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return 0;

	SI32 qualityrrate = 100;

	//--------------------------------------------
	// 기술에 의한 품질 지수 변동 
	//--------------------------------------------
	SI32 needskillunique	= pclItemInfo[ref]->clSkillInfo.siSkill;
	if(needskillunique > 0)
	{
	SI32 skilllevel			= pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel(needskillunique, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0]);
	*addlevel			= max(0, ( skilllevel - pclItemInfo[ref]->clSkillInfo.siSkillLevel));

	if(addlevel > 35)
	{
	qualityrrate += ( (addlevel - 35) * 5 / 10);
	addlevel = 35;
	}

	if(addlevel > 30)
	{
	qualityrrate += ( (addlevel - 30) * 10 / 10);
	addlevel = 30;
	}

	qualityrrate  += (addlevel * 15 / 10);	// 스킬당 1.5퍼센트 확률로 성능 향상. 			
	}


	//-------------------------------------------
	// 손재주에 의한 품질 지수 변동 
	//-------------------------------------------
	SI32 needhnd	=  pclItemInfo[ref]->clSkillInfo.siHnd;
	SI32 addhnd		=  max(0, pclClient->pclCM->CR[id]->clPB.clTotalBA.GetHnd() - needhnd);

	if(addhnd > 150)
	{
	qualityrrate	+= (addhnd - 150) * 2 / 50;
	addhnd		= 150;
	}

	if(addhnd > 100)
	{
	qualityrrate	+= (addhnd - 100) * 5 / 50;
	addhnd		= 100;
	}

	qualityrrate    += (addhnd * 10 / 50);	// 잉여 손재주의 1/5을 장비의 항상에 보탠다. 

	return min(200, qualityrrate);
	*/
}

// 교역 상인용 아이템 정보를 얻는다. 
bool cltItemManagerCommon::GetForeignNPCItemInfo(SI32 itemtype, SI32* punique, GMONEY* pprice, SI32* pitemnum)
{
	SI32 i;
	cltItemPriceUnit* pclprice;
	SI32 itemlistnum = 0;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i] == NULL)continue;

		// 같은 타입의 아이템을 찾는다. 
		if(pclItemInfo[i]->siType != itemtype)continue;
		
		// 순수 원료여서는 안된다. 
		if(pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique == 0)continue;
		
		pclprice = GetCurrentPrice(pclItemInfo[i]->clItem.siUnique);
		if(pclprice == NULL)continue;

		GMONEY price = pclprice->siPrice;
		if(price <= 10)price = 10;

		cltServer* pclserver = (cltServer*)pclClient;

		if (pclserver->pclMinisterMgr == NULL)
			return FALSE;
		CMinisterIzo* pclminister = (CMinisterIzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_IZO);
		SI32 rate = min(75, max(50, pclminister->m_siPriceValue));

		// 정보를 얻어야 햘 최대 갯수를 확인한다. 
		if(itemlistnum < *pitemnum )
		{
			punique[itemlistnum]		= pclItemInfo[i]->clItem.siUnique;

			if(pclItemTypeInfo[itemtype]->siAtb & ITEMATB_OBJECT)
			{
				// 가격은 시세의 75%
				pprice[itemlistnum]			= max(1,(GMONEY)(((REAL64)price/100)*rate));
			}
			else
			{
				// 가격은 시세의 75%
				pprice[itemlistnum]			= max(1,(GMONEY)(((REAL64)price/100)*rate));

			}
			


			itemlistnum++;
		}
	}

	if(itemlistnum <= 0)return false;

	*pitemnum = itemlistnum;

	return true;
}

// 물품 시세를 조정한다. 
void cltItemManagerCommon::CalcItemPrice( _SYSTEMTIME sTime)
{
	SI32 i, j;
	static DWORD dwLastLowPriceCheckClock = 0;
	//static bool bcheckcnppricerateswitch = false;
	//cyj 서버기동시 무수리 시세 무조건 계산하도록
	static bool bFirstCheck = false;
	cltServer *pclserver = (cltServer*)pclClient;
	// DB 초기화 완료될때까지 기다림
	if( pclserver->bCompleteDBInitAll == FALSE )		return;


	if(dwLastLowPriceCheckClock == 0)
	{
		dwLastLowPriceCheckClock = pclClient->CurrentClock;
	}

	// 이따금 최저주문 가격을 참고하여 시세를 변동한다. 
	bool bchecklowpriceswitch = false;
	DWORD checktime = 3600000 ;	// 1시간 간격. 

#ifdef _DEBUG
	checktime = 60000 ; // 1분 간격
#endif

	if((DWORD)TABS(pclClient->CurrentClock - dwLastLowPriceCheckClock) > checktime)
	{
		dwLastLowPriceCheckClock = pclClient->CurrentClock; 
		bchecklowpriceswitch = true;
	}

	for(i = 1;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i])
		{
			SI32 itemunique = pclItemInfo[i]->clItem.siUnique;
			SI32 type		= pclItemInfo[i]->siType;

			// OBJECT TYPE의 아이템은 재료시세를 기반으로 시세를 계산한다. - 목걸이는 제외한다.
			// 아티펙트 결정체는 OBJECT TYPE 처럼 시세를 계산한다.
			if( (pclItemTypeInfo[type]->IsAtb(ITEMATB_OBJECT) == TRUE && type != ITEMTYPE_NECK) ||
				(type == ITEMTYPE_CRYSTAL) )
			{
				// 재료의 시세 가격을 모두 더한다.
				GMONEY sum = 0;
				for(j = 0;j < MAX_MATERIAL_NUMBER;j++)
				{
					SI32 materialunique = pclItemInfo[i]->clProductionInfo.clMaterial[j].siUnique;
					if(materialunique)
					{
						cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, materialunique);
						if(pclprice)
						{
							sum += (pclprice->siPrice * pclItemInfo[i]->clProductionInfo.clMaterial[j].siNeedNum);
						}
					}
				}

				// 아이템에 계산된 가격을 설정한다.
				pclItemPrice->SetPrice(this, itemunique, sum, 100, true);				
			}
			// 오브젝트 물품이 아닌 경우. - 목걸이 포함
			else 
			{
				// 이따금 최저주문 가격을 참고하여 시세를 변동한다. - 1시간에 한번 수행된다.
				if(bchecklowpriceswitch == true || bFirstCheck == false)
				{
					GMONEY	price; 
					SI32	amount;

					if(pclClient->pclMarketManager->FindLowPriceOrderInfo(itemunique, &price, &amount) == false)	continue;

					cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, itemunique);
					if(pclprice == NULL)continue;

					// 최저 가격 이하로는 떨어지지 않는다. 
					price = max(MIN_ITEM_PRICE, price);

					// 최저 가격이 현재 시세보다 높다면, 
					if(price  > pclprice->siPrice)
					{
						if(pclprice->siPrice > 100)	
						{
							pclprice->fPrice	= pclprice->fPrice * 1.005f;	// 0.5% 높여준다. 
						}
						else 
						{
							pclprice->fPrice	= pclprice->fPrice * 1.1f;		// 10% 높여준다. 
						}

						GMONEY siOldPrice = pclprice->siPrice;		// pricebug와 관련된 코드는 버그 수정 이후 모두 지워야함

						pclprice->siPrice	= (SI64)pclprice->fPrice;

						// 은괴값으로 조사해본다 KKM
						if( 5055 == pclprice->siUnique )
						{
							((cltServer*)pclClient)->pclLog->FilePrint( "price1.txt", "%I64d	%f	%I64d", siOldPrice, pclprice->fPrice, pclprice->siPrice );
						}					

						// DB에 저장한다. 
						const TCHAR* pszItemName = pclClient->pclItemManager->GetName(pclprice->siUnique);
						sDBRequest_SetItemPrice clMsg(pclprice->siUnique, pclprice->siPrice, pclprice->fPrice, pclprice->siItemNum, pszItemName);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
					// 최저 가격이 현재 시세보다 낮다면, 
					else if(price  < pclprice->siPrice)
					{
						if(price < 1000000 )
						{
							pclprice->fPrice	= pclprice->fPrice * 0.99f;		// 1% 낮춰준다. 
						}
						else 
						{
							pclprice->fPrice	= pclprice->fPrice * 0.95f;		// 5% 낮춰준다. 
						}

						GMONEY siOldPrice = pclprice->siPrice;	// pricebug와 관련된 코드는 버그 수정 이후 모두 지워야함

						pclprice->siPrice	= (SI64)pclprice->fPrice;

						// 은괴값으로 조사해본다 KKM
						if( 5055 == pclprice->siUnique )
						{
							((cltServer*)pclClient)->pclLog->FilePrint( "price2.txt", "%I64d	%f	%I64d", siOldPrice, pclprice->fPrice, pclprice->siPrice );
						}

						// DB에 저장한다. 
						const TCHAR* pszItemName = pclClient->pclItemManager->GetName(pclprice->siUnique);
						sDBRequest_SetItemPrice clMsg(pclprice->siUnique, pclprice->siPrice, pclprice->fPrice, pclprice->siItemNum, pszItemName);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
				}
			}
		}
	}

	//----------------------------------------------------
	// 평균 가격을 구한다. (물가.) 
	//----------------------------------------------------
	INT64 sum		= 0;
	SI32 itemindex	= 0;
	for( i = 1;i < MAX_ITEMINFO_NUMBER;i++ )
	{
		if(pclItemInfo[i])
		{
			SI32 itemunique = pclItemInfo[i]->clItem.siUnique;

			cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, itemunique);
			if(pclprice == NULL)continue;
			if(pclprice->siPrice < 100)continue;			// 너무 싼 물품은 제외
			if(pclprice->siPrice > 200000000)continue;	// 너무 비싼 물품은 제외

			sum += pclprice->siPrice;
			itemindex++;
		}
	}
	
	// 통합마켓의 시세가 npcrate에 영향을 주게 한다.
	if( pclClient->IsCountrySwitch( Switch_NewMarket ) )
	{
		for( i = 1; i < MAX_ITEMINFO_NUMBER; i++ )
		{
			if(pclItemInfo[i])
			{
				SI32 itemunique = pclItemInfo[i]->clItem.siUnique;

				cltItemPriceUnit* pclprice = pclItemNewPrice->GetPrice(this, itemunique);
				if(pclprice == NULL)				continue;
				if(pclprice->siPrice < 100)			continue;	// 너무 싼 물품은 제외
				if(pclprice->siPrice > 200000000)	continue;	// 너무 비싼 물품은 제외

				sum += pclprice->siPrice;
				itemindex++;
			}
		}
	}
	
	if(itemindex)
	{
		//clItemStatistic.siItemPriceIndex = sum / itemindex;
		//[진성] 통합마켓 때미 공식변경.
		clItemStatistic.siItemPriceIndex = (sum / itemindex) * 0.8;
		

		//KHY - NPCRATE 확대 - 임시 코드 - 매분 25초에 들어옴으로, 하루에 한번만 실행된다.
		if((sTime.wHour == 0)&&(sTime.wMinute == 0)&&(clItemStatistic.bLastNPCPriceRateDaySwitch == true))
		{
			clItemStatistic.bLastNPCPriceRateDaySwitch = false;
		}

	
		// 무수리 시세는 한번만 결정된다.
		if((pclClient->GetFrame() > 500 && m_bcheckcnppricerateswitch == false)||(clItemStatistic.bLastNPCPriceRateDaySwitch == false))
		{
			m_bcheckcnppricerateswitch = true;
			bFirstCheck = true;
			clItemStatistic.bLastNPCPriceRateDaySwitch = true;
			
			if(clItemStatistic.siItemPriceIndex		 < 100000)	clItemStatistic.siNPCPriceRate = 100;
			else if(clItemStatistic.siItemPriceIndex < 150000)	clItemStatistic.siNPCPriceRate = 110;
			else if(clItemStatistic.siItemPriceIndex < 200000)	clItemStatistic.siNPCPriceRate = 120;
			else if(clItemStatistic.siItemPriceIndex < 250000)	clItemStatistic.siNPCPriceRate = 130;
			else if(clItemStatistic.siItemPriceIndex < 300000)	clItemStatistic.siNPCPriceRate = 140;
			else if(clItemStatistic.siItemPriceIndex < 350000)	clItemStatistic.siNPCPriceRate = 150;
			else if(clItemStatistic.siItemPriceIndex < 400000)	clItemStatistic.siNPCPriceRate = 160;
			else if(clItemStatistic.siItemPriceIndex < 450000)	clItemStatistic.siNPCPriceRate = 170;
			else if(clItemStatistic.siItemPriceIndex < 500000)	clItemStatistic.siNPCPriceRate = 180;
			else if(clItemStatistic.siItemPriceIndex < 550000)	clItemStatistic.siNPCPriceRate = 190;
			else if(clItemStatistic.siItemPriceIndex < 600000)	clItemStatistic.siNPCPriceRate = 200;
			else if(clItemStatistic.siItemPriceIndex < 650000)	clItemStatistic.siNPCPriceRate = 210;
			else if(clItemStatistic.siItemPriceIndex < 700000)	clItemStatistic.siNPCPriceRate = 220;
			else if(clItemStatistic.siItemPriceIndex < 800000)	clItemStatistic.siNPCPriceRate = 240;
			else if(clItemStatistic.siItemPriceIndex < 900000)	clItemStatistic.siNPCPriceRate = 260;
			else if(clItemStatistic.siItemPriceIndex < 1000000)	clItemStatistic.siNPCPriceRate = 280;
			else if(clItemStatistic.siItemPriceIndex < 1100000)	clItemStatistic.siNPCPriceRate = 300;
			else if(clItemStatistic.siItemPriceIndex < 1200000)	clItemStatistic.siNPCPriceRate = 320;
			else if(clItemStatistic.siItemPriceIndex < 1300000)	clItemStatistic.siNPCPriceRate = 340;
			else if(clItemStatistic.siItemPriceIndex < 1400000)	clItemStatistic.siNPCPriceRate = 360;
			else if(clItemStatistic.siItemPriceIndex < 1500000)	clItemStatistic.siNPCPriceRate = 380;
			else if(clItemStatistic.siItemPriceIndex < 1600000)	clItemStatistic.siNPCPriceRate = 400;
			else if(clItemStatistic.siItemPriceIndex < 1700000)	clItemStatistic.siNPCPriceRate = 420;
			else if(clItemStatistic.siItemPriceIndex < 1800000)	clItemStatistic.siNPCPriceRate = 440;
			else if(clItemStatistic.siItemPriceIndex < 1900000)	clItemStatistic.siNPCPriceRate = 460;
			else if(clItemStatistic.siItemPriceIndex < 2000000)	clItemStatistic.siNPCPriceRate = 480;
			else if(clItemStatistic.siItemPriceIndex < 2100000)	clItemStatistic.siNPCPriceRate = 500;
			else if(clItemStatistic.siItemPriceIndex < 2200000)	clItemStatistic.siNPCPriceRate = 520;
			else if(clItemStatistic.siItemPriceIndex < 2300000)	clItemStatistic.siNPCPriceRate = 540;
			else if(clItemStatistic.siItemPriceIndex < 2400000)	clItemStatistic.siNPCPriceRate = 560;
			else if(clItemStatistic.siItemPriceIndex < 2500000)	clItemStatistic.siNPCPriceRate = 580;
			else if(clItemStatistic.siItemPriceIndex < 2600000)	clItemStatistic.siNPCPriceRate = 600;
			else if(clItemStatistic.siItemPriceIndex < 2700000)	clItemStatistic.siNPCPriceRate = 620;
			else if(clItemStatistic.siItemPriceIndex < 2800000)	clItemStatistic.siNPCPriceRate = 640;
			else if(clItemStatistic.siItemPriceIndex < 2900000)	clItemStatistic.siNPCPriceRate = 660;
			else if(clItemStatistic.siItemPriceIndex < 3000000)	clItemStatistic.siNPCPriceRate = 680;
			else if(clItemStatistic.siItemPriceIndex < 3100000)	clItemStatistic.siNPCPriceRate = 700;
			else if(clItemStatistic.siItemPriceIndex < 3200000)	clItemStatistic.siNPCPriceRate = 720;
			else if(clItemStatistic.siItemPriceIndex < 3300000)	clItemStatistic.siNPCPriceRate = 740;
			else if(clItemStatistic.siItemPriceIndex < 3400000)	clItemStatistic.siNPCPriceRate = 760;
			else if(clItemStatistic.siItemPriceIndex < 3500000)	clItemStatistic.siNPCPriceRate = 780;
			else if(clItemStatistic.siItemPriceIndex < 3600000)	clItemStatistic.siNPCPriceRate = 800;
			else if(clItemStatistic.siItemPriceIndex < 3700000)	clItemStatistic.siNPCPriceRate = 820;
			else if(clItemStatistic.siItemPriceIndex < 3800000)	clItemStatistic.siNPCPriceRate = 840;
			else if(clItemStatistic.siItemPriceIndex < 3900000)	clItemStatistic.siNPCPriceRate = 860;
			else if(clItemStatistic.siItemPriceIndex < 4000000)	clItemStatistic.siNPCPriceRate = 880;
			else if(clItemStatistic.siItemPriceIndex < 4100000)	clItemStatistic.siNPCPriceRate = 900;
			else if(clItemStatistic.siItemPriceIndex < 4200000)	clItemStatistic.siNPCPriceRate = 920;
			else if(clItemStatistic.siItemPriceIndex < 4300000)	clItemStatistic.siNPCPriceRate = 940;
			else if(clItemStatistic.siItemPriceIndex < 4400000)	clItemStatistic.siNPCPriceRate = 960;
			else if(clItemStatistic.siItemPriceIndex < 4500000)	clItemStatistic.siNPCPriceRate = 980;
			else if(clItemStatistic.siItemPriceIndex < 5000000)	clItemStatistic.siNPCPriceRate = 1000;
			else if(clItemStatistic.siItemPriceIndex < 5500000)	clItemStatistic.siNPCPriceRate = 1150;
			else if(clItemStatistic.siItemPriceIndex < 6000000)	clItemStatistic.siNPCPriceRate = 1300;
			else if(clItemStatistic.siItemPriceIndex < 6500000)	clItemStatistic.siNPCPriceRate = 1450;
			else if(clItemStatistic.siItemPriceIndex < 7000000)	clItemStatistic.siNPCPriceRate = 1600;
			else if(clItemStatistic.siItemPriceIndex < 7500000)	clItemStatistic.siNPCPriceRate = 1750;
			else if(clItemStatistic.siItemPriceIndex < 8000000)	clItemStatistic.siNPCPriceRate = 1900;
			else if(clItemStatistic.siItemPriceIndex < 8500000)	clItemStatistic.siNPCPriceRate = 2050;
			else if(clItemStatistic.siItemPriceIndex < 9000000)	clItemStatistic.siNPCPriceRate = 2200;
			else if(clItemStatistic.siItemPriceIndex < 9500000)	clItemStatistic.siNPCPriceRate = 2350;
			else if(clItemStatistic.siItemPriceIndex < 10000000)clItemStatistic.siNPCPriceRate = 2500;

			//else 	clItemStatistic.siNPCPriceRate = 3000;
			else if(clItemStatistic.siItemPriceIndex < 11000000)clItemStatistic.siNPCPriceRate = 3000;
			else if(clItemStatistic.siItemPriceIndex < 12000000)clItemStatistic.siNPCPriceRate = 3500;
			else if(clItemStatistic.siItemPriceIndex < 13000000)clItemStatistic.siNPCPriceRate = 4000;
			else if(clItemStatistic.siItemPriceIndex < 14000000)clItemStatistic.siNPCPriceRate = 4500;
			else if(clItemStatistic.siItemPriceIndex < 15000000)clItemStatistic.siNPCPriceRate = 5000;
			else if(clItemStatistic.siItemPriceIndex < 16000000)clItemStatistic.siNPCPriceRate = 5500;
			else if(clItemStatistic.siItemPriceIndex < 17000000)clItemStatistic.siNPCPriceRate = 6000;
			else if(clItemStatistic.siItemPriceIndex < 18000000)clItemStatistic.siNPCPriceRate = 6500;
			else if(clItemStatistic.siItemPriceIndex < 19000000)clItemStatistic.siNPCPriceRate = 7000;
			else if(clItemStatistic.siItemPriceIndex < 20000000)clItemStatistic.siNPCPriceRate = 7500;
			else if(clItemStatistic.siItemPriceIndex < 23000000)clItemStatistic.siNPCPriceRate = 8500;
			else if(clItemStatistic.siItemPriceIndex < 26000000)clItemStatistic.siNPCPriceRate = 9500;
			else if(clItemStatistic.siItemPriceIndex < 29000000)clItemStatistic.siNPCPriceRate = 10500;
			else if(clItemStatistic.siItemPriceIndex < 32000000)clItemStatistic.siNPCPriceRate = 11500;
			else if(clItemStatistic.siItemPriceIndex < 35000000)clItemStatistic.siNPCPriceRate = 12500;
			else if(clItemStatistic.siItemPriceIndex < 38000000)clItemStatistic.siNPCPriceRate = 13500;
			else if(clItemStatistic.siItemPriceIndex < 41000000)clItemStatistic.siNPCPriceRate = 14500;
			else if(clItemStatistic.siItemPriceIndex < 44000000)clItemStatistic.siNPCPriceRate = 15500;
			else if(clItemStatistic.siItemPriceIndex < 47000000)clItemStatistic.siNPCPriceRate = 16500;
			else if(clItemStatistic.siItemPriceIndex < 50000000)clItemStatistic.siNPCPriceRate = 17500;
			else if(clItemStatistic.siItemPriceIndex < 56000000)clItemStatistic.siNPCPriceRate = 19500;
			else if(clItemStatistic.siItemPriceIndex < 62000000)clItemStatistic.siNPCPriceRate = 21500;
			else if(clItemStatistic.siItemPriceIndex < 68000000)clItemStatistic.siNPCPriceRate = 23500;
			else if(clItemStatistic.siItemPriceIndex < 74000000)clItemStatistic.siNPCPriceRate = 25500;
			else if(clItemStatistic.siItemPriceIndex < 80000000)clItemStatistic.siNPCPriceRate = 27500;
			else if(clItemStatistic.siItemPriceIndex < 86000000)clItemStatistic.siNPCPriceRate = 29500;
			else if(clItemStatistic.siItemPriceIndex < 92000000)clItemStatistic.siNPCPriceRate = 31500;
			else if(clItemStatistic.siItemPriceIndex < 98000000)clItemStatistic.siNPCPriceRate = 33500;
			else if(clItemStatistic.siItemPriceIndex < 100000000)clItemStatistic.siNPCPriceRate = 35500;
			else 	clItemStatistic.siNPCPriceRate = 35500;

			//KHY - NPCRATE 확대 - 임시 코드
			//----------------------------------------------------------------------------------
			if(clItemStatistic.siNPCPriceRate > 3000)
			{
				SI32 temNPCRATE = ReadLastNPCRATE( );

				if(clItemStatistic.siNPCPriceRate > temNPCRATE)
				{
					clItemStatistic.siNPCPriceRate = temNPCRATE +100;

					WriteLastNPCRATE(clItemStatistic.siNPCPriceRate );
				}
			}
			//----------------------------------------------------------------------------------
			
			// 서버 정보를 통보한다. 
			((cltServer*)pclClient)->SendServerInfo(0);

			pclClient->pclNPCItemInfo->Create(clItemStatistic.siNPCPriceRate);
		}
	}
	else if(clItemStatistic.siNPCPriceRate <= 0)
	{
		// fresh-server fix: no market trades yet => itemindex==0 => rate never set.
		// Without a rate, the NPC shop handler returns nothing and NPCItemInfo::Create(0)
		// is a no-op, so every NPC shop looks empty. Use the default 100% rate.
		clItemStatistic.siItemPriceIndex	= 0;
		clItemStatistic.siNPCPriceRate	= 100;
		((cltServer*)pclClient)->SendServerInfo(0);
		pclClient->pclNPCItemInfo->Create(clItemStatistic.siNPCPriceRate);
	}

	
}

//KHY - NPCRATE 확대 - 임시 코드
//-------------------------------------------------------------------------------------------------------------------
SI32 cltItemManagerCommon::ReadLastNPCRATE()
{
	TCHAR	fileNameBuffer[256]; // 만들 파일명.
	FILE 	*fp = NULL;

	SI32 	siLastNPCRATE = 0; 

	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\LastNPCRATE.log"));			
	fp = _tfopen( fileNameBuffer , TEXT("rb") );

	if ( fp != NULL )
	{
		TCHAR buffer[1024] = { TEXT('\0') };		

		if( _fgetts(buffer, 1024, fp) != NULL )
		{
			HQDivideByTab(buffer,"%d", &siLastNPCRATE);			
		}
		
		fclose( fp );
	}

	return siLastNPCRATE;
}

void cltItemManagerCommon::WriteLastNPCRATE(SI32 nowLastNpcrate)
{
	TCHAR	fileNameBuffer[256]; // 만들 파일명.
	FILE 	*fp = NULL;

	SI32 	siLastNPCRATE = nowLastNpcrate; 

	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\LastNPCRATE.log"));			
	fp = _tfopen( fileNameBuffer , TEXT("w") );

	if ( fp != NULL )
	{
		//fwrite(&siLastNPCRATE , sizeof(SI32) , 1 , fp);
		_ftprintf( fp, TEXT("%d"), siLastNPCRATE);

		fclose( fp );
	}
}
//-------------------------------------------------------------------------------------------------------------------

// 아이템의 수리비를 구한다. 
GMONEY cltItemManagerCommon::GetNPCRepairPrice(cltItem* pclitem)
{
	SI32 ref = FindItemRefFromUnique(pclitem->siUnique);
	if(ref <= 0)return 0;

	SI32 type = pclItemInfo[ref]->siType;

	// 수리 불가품인 경우, 
	if((pclItemTypeInfo[type]->siAtb & ITEMATB_REPAIR) == 0)return 0;

	// 시세를 얻어온다. 
	cltItemPriceUnit* pclprice = GetCurrentPrice(pclitem->siUnique);
	if(pclprice == NULL)return 0;


	GMONEY repairprice = 0;
	GMONEY price = pclprice->siPrice;


	if(price > 1200000)
	{
		repairprice += ((price - 1200000) / 12);
		price = 1200000;
	}
	if(price > 800000)
	{
		repairprice += ((price - 800000) / 15);
		price = 800000;
	}
	if(price > 400000)
	{
		repairprice += ((price - 400000) / 18);
		price = 400000;
	}

	repairprice += (price / 20);

	// 호품이나 명품은 수리비가 비싸다. 
	switch(pclitem->clItemCommon.clCommonInfo.uiRareInfo)
	{
	case RARELEVEL_1:	repairprice *= 2;	break;
	case RARELEVEL_2:	repairprice *= 3;	break;
	}

	return max(1, repairprice);


}

// 캐릭터 kind를 근거로 소환 인형의 유니크를 구한다. 
SI32 cltItemManagerCommon::GetItemDollUnique(SI32 kind)
{
	SI32 i;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i] == NULL)continue;

		if(pclItemInfo[i]->siMakeKind == kind)
		{
			return pclItemInfo[i]->clItem.siUnique;
		}
	}

	return 0;
}

// 모병관 퀘스트의 보상으로 지급할 물품의 종류와 수량 확인. 
bool cltItemManagerCommon::GetDailyQuestRewardInfo(GMONEY money, SI32* pitemunique, SI32* pitemnum, GMONEY* prewardmoney )
{
	SI32 i;
	SI32 itemunique[MAX_ITEMINFO_NUMBER];
	for(i= 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		itemunique[i] = 0;
	}
	SI32 itemindex = 0;
	cltItemPriceUnit* pclprice;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i] == NULL)continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_ANIMAL)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_DOLL)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_BOOK)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_TICKET)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_MANTLE)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_BAG)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_SCROLL)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_SEAFOOD)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_ETC)		continue;

		if(pclItemInfo[i]->siType == ITEMTYPE_CRYSTAL)		continue;//아티팩트 결정체 
		if(pclItemInfo[i]->siType == ITEMTYPE_COMPOSITIONMATERIAL)		continue;//합성재료
		if(pclItemInfo[i]->siType == ITEMTYPE_EVENTITEM)		continue;//이벤트아이템
		if(pclItemInfo[i]->siType == ITEMTYPE_BOX)		continue;//박스아이템
		if(pclItemInfo[i]->siType == ITEMTYPE_PREMIUM)		continue;

		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_MANTLE)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_DRESS)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_HAT)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_NECK)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_RING)		continue;
        
		// 원료가 없는 물품이어야 한다. 
		if(pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique)continue;

		// 오브젝트 성격의 물품은 아니어야 한다. 
		if(GetItemAtb(pclItemInfo[i]->clItem.siUnique) & ITEMATB_OBJECT)continue;

	
		// 아이템의 시세 확인 
		pclprice = pclItemPrice->GetPrice(this, pclItemInfo[i]->clItem.siUnique);
		if(pclprice == NULL)continue;
		if (pclprice->siPrice < 100 )	continue;
		
		// 송편은 아니어야 한다. 
		if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3730))continue;
		// 말사료는 아니어야 한다. 
		if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3855))continue;

		if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_NOTDAILYQUEST)) continue;

		if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_ITEMMALL)) continue;

		/*cyj 이벤트 아이템유니크 삭제
		// 흥부박은 아니어야 한다. 
		if(pclItemInfo[i]->clItem.siUnique == 8200)continue;
		// 생생탕은 아니어야 한다.
		if(pclItemInfo[i]->clItem.siUnique == 2350)continue;
		*/
	
		itemunique[itemindex] = pclItemInfo[i]->clItem.siUnique;			
		itemindex++;
		
	}

	if(itemindex <= 0)return false;

	// 아이템의 종류 확보. 
	*pitemunique = itemunique[rand() % itemindex];

	// 아이템의 시세 확인 
	pclprice = pclItemPrice->GetPrice(this, *pitemunique);
	if(pclprice == NULL)return false;

	GMONEY itemprice = pclprice->siPrice * 6;	// 시세보다 높은 가격으로 계산한다. 

	// 아이템의 개수 확보. 
	*pitemnum	= (SI32)(min(30, max(1, money / itemprice)));

	*prewardmoney = pclprice->siPrice * (*pitemnum);

	return true;
}
bool cltItemManagerCommon::GetDailyQuest2RewardInfo(GMONEY money, SI32* pitemunique, SI32* pitemnum, GMONEY* prewardmoney )
{
	SI32 i;
	SI32 itemunique[MAX_ITEMINFO_NUMBER];
	cltItemPriceUnit* pclprice;
	for(i= 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		itemunique[i] = 0;
	}
	SI32 itemindex = 0;
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)		
	{
		if(pclItemInfo[i] == NULL)continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_ANIMAL)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_DOLL)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_BOOK)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_TICKET)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_MANTLE)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_BAG)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_SCROLL)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_SEAFOOD)	continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_ETC)		continue;

		if(pclItemInfo[i]->siType == ITEMTYPE_CRYSTAL)					continue;	//	아티팩트 결정체 
		if(pclItemInfo[i]->siType == ITEMTYPE_COMPOSITIONMATERIAL)		continue;	//	합성재료
		if(pclItemInfo[i]->siType == ITEMTYPE_EVENTITEM)					continue;	//	이벤트아이템
		if(pclItemInfo[i]->siType == ITEMTYPE_BOX)						continue;	//	박스아이템
		if(pclItemInfo[i]->siType == ITEMTYPE_PREMIUM)					continue;	//	프리미엄 아이템

		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_MANTLE)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_DRESS)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_HAT)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_NECK)		continue;
		if(pclItemInfo[i]->siType == ITEMTYPE_AVATAR_RING)		continue;

		// 원료가 없는 물품이어야 한다. 
		if(pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique)continue;

		// 오브젝트 성격의 물품은 아니어야 한다. 
		if(GetItemAtb(pclItemInfo[i]->clItem.siUnique) & ITEMATB_OBJECT)continue;

		//// 아이템의 시세 확인 
		pclprice = pclItemPrice->GetPrice(this, pclItemInfo[i]->clItem.siUnique);
		if(pclprice == NULL)continue;
		if (pclprice->siPrice < 300 ) continue;//300원보다 ㅈㅈ

		// 송편은 아니어야 한다. 
		if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3730))continue;
		// 말사료는 아니어야 한다. 
		if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3855))continue;

		if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_NOTDAILYQUEST)) continue;

		if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_ITEMMALL)) continue;

		itemunique[itemindex] = pclItemInfo[i]->clItem.siUnique;

		itemindex++;

	}

	if(itemindex <= 0)return false;

	// 아이템의 종류 확보. 
	*pitemunique = itemunique[rand() % itemindex];
	if(rand() % 100 == 45){
		switch(rand()%3) 
		{
		case 0:{
			*pitemunique = ITEMUNIQUE(5380);
			*pitemnum	= 0 ;
			*prewardmoney = 0;
			   }
			   break;
		case 1:
			{
				*pitemunique = ITEMUNIQUE(7175);
				*pitemnum	= 0 ;
				*prewardmoney = 0;
			}
			break;
		case 2:
			{
				*pitemunique = ITEMUNIQUE(6140);
				*pitemnum	= 0 ;
				*prewardmoney = 0;
			}
			break;
		}
	}
	else{
		// 아이템의 개수 확보. 
		*pitemnum	= 0 ;

		*prewardmoney = 0;
	}
	return true;
}
//데일리 퀘스트의 아이템을 가지고 온다.
bool cltItemManagerCommon::GetDailyQuest3KindInfo(SI16* pUnique, SI16* pMakeNum)
{
	SI32 i;
	SI32 itemunique[MAX_ITEMINFO_NUMBER];
	//초기화.
	for(i= 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		itemunique[i] = 0;
	}
	SI32 itemindex = 0;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		// 아이템정보가 NULL이면 안된다.
		if(pclItemInfo[i] == NULL)continue;
		//먼저 아이템 몰 아이템이면 continue
		if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_ITEMMALL)) continue;
		// 데일리 퀘스트가 아니면 안된다.
		if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_NOTDAILYQUEST)) continue;
		// 말사료는 아니어야 한다. 
		if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3855))continue;
		//흡혈 셋트 일경우는 패스.
		if ( pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE( 685 ) ||	// 흡혈귀 흉갑
			pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE( 875) ||		// 흡혈귀 벨트
			pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE( 785 ) ||		// 흡혈귀 투구
			pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE( 1700 ) ) {	// 흡혈귀 신발
				continue;
		}

		//무기나 방어구 일 경우에만 해야 하므로.해당 타입 일때만 리스트를 넣는다.
		if (pclItemInfo[i]->siType == ITEMTYPE_SWORD	||
			pclItemInfo[i]->siType == ITEMTYPE_SPEAR	||
			pclItemInfo[i]->siType == ITEMTYPE_STAFF	||
			pclItemInfo[i]->siType == ITEMTYPE_AXE		||
			pclItemInfo[i]->siType == ITEMTYPE_BOW		||
			pclItemInfo[i]->siType == ITEMTYPE_GUN		||
			pclItemInfo[i]->siType == ITEMTYPE_CANNON	||
			pclItemInfo[i]->siType == ITEMTYPE_ARMOUR	||
			pclItemInfo[i]->siType == ITEMTYPE_BELT		||
			pclItemInfo[i]->siType == ITEMTYPE_SHOES	||
			pclItemInfo[i]->siType == ITEMTYPE_HELMET
			) 
		{
			// 만들어낼 아이템은 제조 아이템의 기술 레벨에 따라서 변한다.
			SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( pclItemInfo[i]->clItem.siUnique);
			if (Ref <= 0 ) continue;
			SI32 SkillLevelLimit = pclClient->pclItemManager->pclItemInfo[Ref]->clSkillInfo.siSkillLevel; // 제작기술Lev
			if (SkillLevelLimit <= 20 ) continue;// 제조 기술 레벨은 최소 20이상
			// 조건이 모두 충족 되었을때. 아이템유니크를 인덱스에 넣는다.
			itemunique[itemindex] = pclItemInfo[i]->clItem.siUnique;
			//인덱스 증가.
			itemindex++;
		}				
	}
	if(itemindex <= 0)return false;
	// 아이템이 다양하게 들어가야 하므로.
	*pUnique = itemunique[rand() % itemindex];
	
	// 만들어낼 아이템은 제조 아이템의 기술 레벨에 따라서 변한다.
	SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( *pUnique );
	if (Ref <= 0 ) return false;
	SI32 SkillLevelLimit = pclClient->pclItemManager->pclItemInfo[Ref]->clSkillInfo.siSkillLevel; // 제작기술Lev
	if (SkillLevelLimit <= 20 ) return false;// 제조 기술 레벨은 최소 20이상

	//10~40개 사이로 나온다.
	SI32 randtemp = rand() % MAX_DAILYQUEST3_MAKEITEM_NUM +10;
	SI32 skilltemp = SkillLevelLimit/10;

	*pMakeNum	=randtemp/skilltemp +5 ;
	return true;
}

//데일리 퀘스트 3 보상아이템 설정.
bool cltItemManagerCommon::GetDailyQuest3RewardInfo( SI32* siRewardExp, SI16 siMakeItemNum, SI16* pRewarditemunique, SI16* pRewarditemnum, SI32 SkillLevelLimit )
{
	cltItemPriceUnit*		pclprice ;// 보상아이템의 시세 - 1000 이하의 보상은 제외. 보상아이템 설정시 사용
	SI32 i					=	0 ;
	SI32 SkillLimitTemp		=	0 ;			// 보상 경험치 계산을 위해 아이템 제작 기술 레벨 제한 Temp 변수
	SI32 RewardTemp			=	0 ;			//	보상 갯수 계산을 위한 Temp 변수
	SI32 RewardTemp2		=	0 ;			//	보상 갯수 계산을 위한 Temp 변수 2
	SI32 itemunique[MAX_ITEMINFO_NUMBER] ;	// 아이템 유니크를 저장할 배열
	SI32 itemindex			=	0 ;			// 아이템 인덱스 
	//SI32 SkillLevelLimit	만들 아이템의 제작 필요스킬

    // 아아템 유니크를 넣을 배열을 초기화 시킨다.	
	for(i= 0;i < MAX_ITEMINFO_NUMBER;i++)
	{	itemunique[i] = 0;	}
	// 보상을 줄 아이템을 설정한다.
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i] == NULL)continue;
		//	보상아이템은 이 3가지 종류만 준다. 그러므로 이 때만 조건문 안으로 들어간다. 그 외에는 pass
		if (pclItemInfo[i]->siType == ITEMTYPE_CLOTH	||
			pclItemInfo[i]->siType == ITEMTYPE_MINERAL	||
			pclItemInfo[i]->siType == ITEMTYPE_TREE) {
				// 원료가 없는 물품이어야 한다. 
				if(pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique)continue ;
				// 오브젝트 성격의 물품은 아니어야 한다. 
				if(GetItemAtb(pclItemInfo[i]->clItem.siUnique) & ITEMATB_OBJECT)continue ;
				// 송편은 아니어야 한다. 
				if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3730))continue ;
				// 말사료는 아니어야 한다. 
				if(pclItemInfo[i]->clItem.siUnique == ITEMUNIQUE(3855))continue ;
                // 아이템 탭이 데일리 퀘스트가 아니면 안된다.
				if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_NOTDAILYQUEST)) continue ;
				// 아이템몰 아이템은 지급 하지 않는다.
				if(pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_ITEMMALL)) continue ;
				// 보상을 줄 아이템의 시세를 가지고 온다.
				pclprice = pclItemPrice->GetPrice(this, pclItemInfo[i]->clItem.siUnique) ;
				if(pclprice == NULL)continue ;
				//아이템 시세가 1000원 미민의 아이템은 주지 않는다.
				if (pclprice->siPrice < 1000 ) continue ;
				
				// 조건이 모두 충족 되었을때. 아이템유니크를 인덱스에 넣는다.
				itemunique[itemindex] = pclItemInfo[i]->clItem.siUnique ;
				//인덱스 증가.
				itemindex++ ;
		}
		else{
			continue;
		}
	}
	if(itemindex <= 0)return false ;
	// 보상 아이템의 종류 확보 
	*pRewarditemunique = itemunique[rand() % itemindex] ;
	//---------------------------------------------------------------------------------------------------------------------------------
	// 보상아이템의 시세 확인 
	//---------------------------------------------------------------------------------------------------------------------------------
	pclprice = pclItemPrice->GetPrice(pclClient->pclItemManager, *pRewarditemunique) ;
	if(pclprice == NULL)	return false ;
	// 보상아이템 시세가 1000원 미만일경우 false 리턴
	if (pclprice->siPrice < 1000 )	return false ;
	//---------------------------------------------------------------------------------------------------------------------------------
	// 보상경험치 설정
	//---------------------------------------------------------------------------------------------------------------------------------
	SkillLimitTemp = (SkillLevelLimit / 3) ;
	if (SkillLimitTemp <= 0) return false ;
	* siRewardExp = SkillLevelLimit * siMakeItemNum * 300 /100 * SkillLimitTemp ;
	//---------------------------------------------------------------------------------------------------------------------------------
	//보상아이템 갯수를 구한다.
	//---------------------------------------------------------------------------------------------------------------------------------
	RewardTemp = ( pclprice->siPrice * 2 );
	if ( RewardTemp <= 0 ) return false;
	RewardTemp2 = ( *siRewardExp * siMakeItemNum * 7 ) / RewardTemp ;
	*pRewarditemnum = min( 300, max( 1,RewardTemp2 ) ) ;

	return true;
}
// 물품의 재료의 재료의 재료의 재료를 구하는 재귀함수 -영진
bool cltItemManagerCommon::GetWeenyOldManMaterialInfo(SI16 resolutionRate, SI32 disassembleItem, cltItem* pDisAssemitem)
{
	SI32 itemIndex = 0 , i = 0, j = 0;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( disassembleItem );
	if ( ref <= 0 ) 
		return false;	

	for( i = 0 ; i < MAX_MATERIAL_NUMBER ; i++)
	{
		// 원료 아이템이 무효해서는 안된다. 
		SI32 materialUnique = pclItemInfo[ ref ]->clProductionInfo.clMaterial[i].siUnique;
		if( materialUnique <= 0 ) continue;

		SI32 materialRef = FindItemRefFromUnique( materialUnique );
		if( materialRef <= 0 ) continue;

		SI32 materialType = pclItemInfo[ materialRef ]->siType;
		// 원료가 오브젝트 아이템이라면 그 재료를 구해서 더해넣는다.
		if( pclItemTypeInfo[ materialType ]->IsAtb( ITEMATB_OBJECT ) == TRUE )
		{
			// 재귀함수
			if( GetWeenyOldManMaterialInfo( resolutionRate , materialUnique , pDisAssemitem ) == false)
			{
				return false;
			}
		}
		else // 원료가 그냥 아이템이라면 그대로 추가해 준다.
		{
			SI32 giveItemNum = pclItemInfo[ref]->clProductionInfo.clMaterial[i].siNeedNum * resolutionRate / 100;
			if( giveItemNum <= 0 ) continue;

			SI32 emptyIndex = -1;
			
			// 동일한 아이템이 이미 목록에 있다면 갯수를 더해준다.
			for( j = 0; j < MAX_ITEM_PER_WENNYOLD ; j++ )
			{
				if( pDisAssemitem[ j ].siUnique == materialUnique )
				{
					pDisAssemitem[ j ].siItemNum += giveItemNum;
					if( pDisAssemitem[ j ].siItemNum > MAX_ITEM_PILE_NUMBER ) 
					{
						pDisAssemitem[ j ].siItemNum = MAX_ITEM_PILE_NUMBER;
					}
					break;
				}
				else if( pDisAssemitem[ j ].siUnique == 0 )	// 빈 인덱스를 기억해 둔다. 나중에 여기다가 아이템을 넣기 위해
				{
					if( emptyIndex < 0 )	// 빈 인덱스를 아직 못찾은 상태라면 현재 빈 인덱스를 기억시킨다.
					{
						emptyIndex = j ;
					}
				}
			}

			// for 문을 break 없이 그냥 빠져나왔다는 것은 아직 아이템을 목록에 넣지 못했다는 뜻이다.
			if( j >= MAX_ITEM_PER_WENNYOLD )
			{
				// 목록에 없었던 경우 기억된 빈 인덱스에 아이템을 넣어준다.
				if( emptyIndex >= 0 && emptyIndex < MAX_ITEM_PER_WENNYOLD )
				{
					if( pDisAssemitem[ emptyIndex ].siUnique == 0 )
					{
						pDisAssemitem[ emptyIndex ].siUnique = materialUnique ;
						pDisAssemitem[ emptyIndex ].siItemNum += giveItemNum;

						if( pDisAssemitem[ emptyIndex ].siItemNum > MAX_ITEM_PILE_NUMBER ) 
						{
							pDisAssemitem[ emptyIndex ].siItemNum = MAX_ITEM_PILE_NUMBER;
						}

					}
				}
			}
			
			// 목록이 가득차있다면 더이상 다른 종류의 아이템을 넣어줄순 없다. 그냥 넘어간다.

		}
	}

	return true;
}

bool cltItemManagerCommon::GetRareItemWithWeenyOld(cltItem *pclitem)
{
	if (pclitem == NULL)
		return false;

	cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, pclitem->siUnique);
	GMONEY itemprice = pclprice->siPrice;
		
	SI32 percent = 0;

	if (IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_ITEMMALL) == true)
	{
		cltServer *pclserver = (cltServer*)pclClient;

		SI32 useableDay = 0;
		BOOL rVal = pclserver->GetItemUseAbleDay(pclitem, &useableDay);
		if(rVal == FALSE) return false;

		percent = 34;

		if (useableDay > 0)
		{
			percent = percent + (useableDay * 2);
		}

		if (percent > 94)
		{
			percent = 94;
		}
        
	}
	else
	{

		if (itemprice >= clItemStatistic.siItemPriceIndex * 15)		percent = 3;
		else if (itemprice < clItemStatistic.siItemPriceIndex * 15)		percent = 2;
		else if (itemprice < clItemStatistic.siItemPriceIndex * 14)		percent = 1;
		else if (itemprice < clItemStatistic.siItemPriceIndex * 13)		percent = 0;

	}

	if (rand() % 100 + 1 <= percent)
		return true;
	else
		return false;

	

}


// 이호식에게서 아이템 분해했을때 결과값을 계산한다
SI16 cltItemManagerCommon::GetWennyOldItem( cltItem* pclItem, SI16 *resolutionRate )
{
	cltServer* pclserver = (cltServer*)pclClient;

	// 가챠 아이템이라면

	if( pclserver->pclGachaManager->IsGachaRareItem( pclItem->siUnique ) == true )
	{
		return WENNYMSG_GACHARARE;
	}

	//KHY - 0115 발렌타인데이 이벤트.
	if( GetValentineEvent(pclItem->siUnique) == true )
	{
		return WENNYMSG_VAlENTINEITEM;
	}

	//KHY - 0218 화이트데이 이벤트.
	if( GetWhiteEvent(pclItem->siUnique) == true )
	{
		return WENNYMSG_WHITEITEM;
	}

	// 아이템몰 아이템이라면
	if ( IsItemInfoAtb( pclItem->siUnique , ITEMINFOATB_ITEMMALL ) )
	{
		SI32 useableDay = 0;
		BOOL rVal = pclserver->GetItemUseAbleDay(pclItem, &useableDay);
		//if(rVal == FALSE) 
			//return WENNYMSG_FAILED;

		// 목걸이 나올 확률을 구한다.
		SI32 necklaceRate = 10;
		necklaceRate += useableDay;
		necklaceRate = max( 40, necklaceRate );

		SI32 siRand = 0 ;
		siRand = rand()%100 + 1;

		// 대만의 경우 이호식 상자가 안나오게 수정함. (PCK - 2007.09.11)
		if (pclClient->IsCountrySwitch(Switch_WENNYBOXDisable))
		{
			if ( siRand <= necklaceRate )
			{
				return WENNYMSG_NECKLACE;
			}
			else if( siRand <= necklaceRate + 2 )
			{
				return WENNYMSG_NONE;
			}
			else
			{
				return WENNYMSG_NONE;
			}
		}
		else
		{
			if ( siRand <= necklaceRate )
			{
				return WENNYMSG_NECKLACE;
			}
			else if( siRand <= necklaceRate + 2 )
			{
				return WENNYMSG_NONE;
			}
			else
			{
				return WENNYMSG_BOX;
			}
		}

	}
	// 이호식 전용 재료를 주는 아이템이라면 10% 의 확률로 전용 재료를 줌
	else if( IsItemInfoAtb( pclItem->siUnique , ITEMINFOATB_WENNYOLDMAN_MATERIAL ) )
	{
		SI32 siRand = 0 ;
		siRand = rand()%100 ;
		
		if( siRand < 10 )
		{
			return WENNYMSG_WENNYMATERIAL;
		}
	}
	
	// 위에 해당되지 않는 일반적인 경우

	cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice( pclItem->siUnique );

	GMONEY priceIndex = pclClient->pclItemManager->clItemStatistic.siItemPriceIndex;

	if ( priceIndex == 0  )
		return WENNYMSG_FAILED;

	// 20레벨 이하 체크
	SI32 ref = FindItemRefFromUnique( pclItem->siUnique );
	SI32 equiplevel = pclItemInfo[ref]->clCondition.siLevel;

	SI64 siPenalty = (pclprice->siPrice * 100) / priceIndex;
	SI64 rareRate = 0;

	if ( siPenalty < 130 )	rareRate = 1;
	else if ( siPenalty < 140 )	rareRate = 2;
	else if ( siPenalty < 150 )	rareRate = 3;
	else if ( siPenalty < 160 )	rareRate = 4;
	else	rareRate = 5;

	BOOL bEnableWennyMsg_Box = TRUE;
	
	// 대만의 경우 이호식상자가 안나오도록 수정함 (PCK - 2007.09.11)
	if (pclClient->IsCountrySwitch(Switch_WENNYBOXDisable))
	{
		bEnableWennyMsg_Box = FALSE;
	}

	if(pclClient->IsCountrySwitch(Switch_WENNYBOXCHECKLEVEL))
	{
		// 20레벨 이상의 장비만 박스 나옴..
		if(equiplevel < 20)		bEnableWennyMsg_Box = FALSE;
	}

	SI32 siRand = rand() % 100 + 1;
	if ( siRand <= rareRate )
	{
		*resolutionRate = 200;
		return WENNYMSG_DOUBLE;
	}
	else if( siRand <= rareRate*2 && bEnableWennyMsg_Box)	// 20레벨 이상의 장비만 박스 나옴..
	{
		return WENNYMSG_BOX;
	}
	else
	{
		// 내구도 수리석 받기
		if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
		{
			SI32 siGetDurRepareStone = rand() % 100; //0 ~ 99
			if ( 5 > siGetDurRepareStone )	// 5%확률로 받음
			{
				return WENNYMSG_REPAIR_DUR;
			}
		}

		SI32 resolutionRand = rand() % 100 + 1;

		if ( resolutionRand <= 5 )			*resolutionRate += 30;
		else if ( resolutionRand <= 20 )	*resolutionRate += 40;
		else if ( resolutionRand <= 40 )	*resolutionRate += 50;
		else if ( resolutionRand <= 70 )	*resolutionRate += 60;
		else if ( resolutionRand <= 90 )	*resolutionRate += 70;
		else								*resolutionRate += 80;

		*resolutionRate += resolutionRand % 10; 

		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			cltServer* pclserver = (cltServer*)pclClient;
			CMinisterIzo* pclminister = (CMinisterIzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_IZO);
			SI32 value = pclminister->GetTinTin();

			*resolutionRate += value;
		}

		if ( *resolutionRate < 50 )			
			return WENNYMSG_SMALL;
		else if ( *resolutionRate < 70 )	
			return WENNYMSG_MEDIUM;
		else 		
			return WENNYMSG_LARGE;
	}
	
}

// 건물 수리용 아이템을 구한다.
bool cltItemManagerCommon::GetItemForStrRepair(GMONEY strprice, SI32* pitemunique, SI32* pitemnum)
{

	SI32 i;
	SI32 itemunique[MAX_ITEMINFO_NUMBER];
	for(i= 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		itemunique[i] = 0;
	}
	SI32 itemindex = 0;

	/*cyj 건물수리에 필요한 재료 석탄, 철광석, 금광석으로 제한
	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i] == NULL)continue;
		if(pclItemInfo[i]->siType != ITEMTYPE_IRON
		&& pclItemInfo[i]->siType != ITEMTYPE_MINERAL
		&& pclItemInfo[i]->siType != ITEMTYPE_CLOTH
		&& pclItemInfo[i]->siType != ITEMTYPE_TREE)continue;

		itemunique[itemindex] = pclItemInfo[i]->clItem.siUnique;
		itemindex++;
		
	}

	if(itemindex <= 0)return false;
	*/
	// 석탄 (일본판 유니크)
	itemunique[0] = ITEMUNIQUE(5000); //ITEMUNIQUE(5300);
	itemindex++;

	// 철광석 (일본판 유니크)
	itemunique[1] = ITEMUNIQUE(5010); //ITEMUNIQUE(5302);
	itemindex++;

	// 금광석 (일본판 유니크)
	itemunique[2] = ITEMUNIQUE(5040); //ITEMUNIQUE(5314);
	itemindex++;

	// 아이템의 종류 확보. 
	*pitemunique = itemunique[rand() % itemindex];

	// 아이템의 시세 확인 
	cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, *pitemunique);
	if(pclprice == NULL)return false;

	
	GMONEY itemprice = max(100, pclprice->siPrice );

	// 건물 가격의 0.7%에 해당하는 금액어치를 구입한다. 
	strprice = max(1000, strprice * 7 / 1000);

	// 아이템의 개수 확보. 
	*pitemnum	= min(100, max(1, strprice / itemprice));

	return true;

}


// 특정 아이템이 누적 가능한가 ?
bool cltItemManagerCommon::CanPile(SI32 itemunique)
{
	if(itemunique <= 0)return false;
	if(itemunique >= MAX_ITEM_UNIQUE)return false;
	
	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return false;

	// 기간제 아이템은 겹칠 수 없다	- By LEEKH 2007.06.19
	if(pclItemInfo[ref]->clItem.IsHaveUseDay() == TRUE)		return false;

	SI32 type = pclItemInfo[ref]->siType;
	if(type < 0)return false;

	if(pclItemTypeInfo[type] == NULL)return false;

	if(pclItemTypeInfo[type]->IsAtb(ITEMATB_OBJECT) == FALSE)
	{
		return true;
	}

	return false;

}

// 만들 수 있는 물품인가 ?(원료가 존재하는 물품인가. ) 
bool cltItemManagerCommon::CanMakableItem(SI32 itemunique)
{
	if(itemunique <= 0)return false;
	if(itemunique >= MAX_ITEM_UNIQUE)return false;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return false;

	// 첫번째 원료가 없다면 제조 불가 물품. 
	if(pclItemInfo[ref]->clProductionInfo.clMaterial[0].siUnique <= 0)return false;

	return true;
}


// 투자상담가의 종목을 계산한다.
void cltItemManagerCommon::CalcInvestment(bool bStart)
{
	if ( bStart == false ) return ;

	//----------------------------------------------------------------
	// 데이터 초기화 작업
	for ( SI32 i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
	{
		stInvestmentLines[i].siInvestmentItemUnique = 0 ;
		stInvestmentLines[i].siInvestmentItemPrice = 0 ;

		stInvestmentLines[i].siMarginalProfit = 0 ;

		for ( SI32 r = 0 ; r < MAX_MATERIAL_NUMBER ; r ++ )
		{
			stInvestmentLines[i].siMaterialItemPrice[r] = 0 ;
		}
	}
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// 전체 아이템에 대해...
	for ( i = 0 ; i < MAX_ITEMINFO_NUMBER ; i ++)
	{
		if(pclItemInfo[i] == NULL)						continue;

		// 제조할 수 없는 물건이면, 스킵
		SI32 currentunique = pclItemInfo[i]->clItem.siUnique ;
		if ( CanMakableItem(currentunique) == false )	continue ;

		// 재료의 총 시세을 구한다. = 제조시 필요금액
		GMONEY siTotalPrice = 0 ;
		for ( SI32 j = 0 ; j < MAX_MATERIAL_NUMBER ; j ++ )
		{
			cltMaterialInfo pclMaterialInfo = pclItemInfo[i]->clProductionInfo.clMaterial[j] ;
			SI32 materialunique = pclMaterialInfo.siUnique ;
			if ( materialunique <= 0 ) continue ;

			cltItemPriceUnit* pclprice = GetCurrentPrice(materialunique) ;
			if ( pclprice == NULL ) continue ;

			GMONEY materialprice = pclprice->siPrice * pclMaterialInfo.siNeedNum ;
			siTotalPrice += materialprice ;
		}

		// 아이템시세 X 생산개수 = 판매금액
		GMONEY siCurrentPrice = GetCurrentPrice(currentunique)->siPrice * pclItemInfo[i]->clProductionInfo.siProductUnit ;

		// 차액을 구한다.
		// 판매금액 - 제조시 필요금액
		GMONEY marginalprofit = siCurrentPrice - siTotalPrice ;

		// 제조시 필요한 금액보다 판매 금액이 더 크다면...
		if ( marginalprofit > 0 )
		{
			for ( SI32 k = 0 ; k < MAX_INVESTMENT_ITEM_NUMBER ; k ++ )
			{
				// 저장된 이익보다, 현재 구한 이익이 더 크다면...
				if ( stInvestmentLines[k].siMarginalProfit < marginalprofit )
				{
					// 데이터 저장을 위해 나머지 데이터를 뒤로 미루고 공간을 만든다.
					// 복사하면서 마지막 데이터는 없어진다.
					for( SI32 loop = MAX_INVESTMENT_ITEM_NUMBER - 2; loop >= k ; loop-- )
						stInvestmentLines[loop+1] = stInvestmentLines[loop];

					stInvestmentLines[k].siInvestmentItemPrice = GetCurrentPrice(currentunique)->siPrice ;
					stInvestmentLines[k].siInvestmentItemUnique = currentunique ;
					stInvestmentLines[k].siMarginalProfit = marginalprofit ;

					// 재료들의 시세를 저장한다.
					for ( int l = 0 ; l < MAX_MATERIAL_NUMBER ; l ++ )
					{
						cltMaterialInfo pclMaterialInfo = pclItemInfo[i]->clProductionInfo.clMaterial[l] ;
						SI32 materialunique = pclMaterialInfo.siUnique ;
						if ( materialunique <= 0 ) continue ;

						cltItemPriceUnit* pclprice = GetCurrentPrice(materialunique) ;
						if ( pclprice == NULL ) continue ;

						stInvestmentLines[k].siMaterialItemPrice[l] = pclprice->siPrice ;
					}
					break;
				}
			}
		}
	}
}

// 대박 전리품을 랜덤하게 얻어온다. 
SI32 cltItemManagerCommon::GetRandBigWarItem()
{
	SI32 bigwaritemnum = sizeof(siBigWarItemList) / sizeof(SI32);

	return siBigWarItemList[rand() % bigwaritemnum];
}
SI32 cltItemManagerCommon::GetRandBigWennyItem()
{
	SI32 bigwennyitemnum = sizeof(siBigWennyItemList) / sizeof(SI32);

	return siBigWennyItemList[rand() % bigwennyitemnum];
}

SI32 cltItemManagerCommon::GetRandBigChristmasItem()
{
	SI32 bigChristmasitemnum = sizeof(siBigChristmas) / sizeof(SI32);

	return siBigChristmas[rand() % bigChristmasitemnum];
}


// 특정 물품이 대박 물품인가. 
bool cltItemManagerCommon::IsBigWarItem(SI32 itemunique)
{
	SI32 bigwaritemnum = sizeof(siBigWarItemList) / sizeof(SI32);
	
	for(SI32 i = 0;i < bigwaritemnum; i++)
	{
		if(itemunique == siBigWarItemList[i])return true;
	}

	return false;
}

// 랜덤하게 아이템의 이름을 구한다. 
const TCHAR* cltItemManagerCommon::GetRandomItemName()
{
	SI32 i;
	SI16 itemlist[MAX_ITEMINFO_NUMBER];
	SI32 itemindex = 0;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		itemlist[i] = 0;
	}


	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if( pclItemInfo[i] == NULL )
			continue;
		if( false == pclItemInfo[i]->IsItemInfoAtb( ITEMINFOATB_SHOWLIST ) )
			continue;
		if( pclItemInfo[i]->IsItemInfoAtb( ITEMINFOATB_NOTTRADE ) )
			continue;
		if( pclItemInfo[i]->siType == ITEMTYPE_BOOK || 
			pclItemInfo[i]->siType == ITEMTYPE_TICKET )
			continue;		

		itemlist[itemindex] = i;
		itemindex++;
	}

	if(itemindex)
	{
		SI32 index = itemlist[rand() % itemindex];

		return pclItemInfo[ index ]->GetName();
	}

	return NULL;
}


SI32 cltItemManagerCommon::GetCushionUniqueFromCharKind(SI32 charkind)
{
	if( charkind == KIND_HERO3 )
	{
		return ITEMUNIQUE(13610);
	}
	else if( charkind == KIND_HEROINE3  )
	{
		return ITEMUNIQUE(13611);
	}
	else if( charkind == KIND_HERO4  )
	{
		return ITEMUNIQUE(13608);
	}
	else if( charkind == KIND_HEROINE4  )
	{
		return ITEMUNIQUE(13609);
	}

	return -1;
}

//stEnchantNeed cltItemManagerCommon::GetEnchantNeedItem(cltItem cltOrgItem,SI32 siSuccessUpItemNum ,BOOL bUseBlessingStone , BOOL bUseSealingUpIgnore )
void cltItemManagerCommon::GetEnchantNeedItem(cltItem cltOrgItem, stEnchantNeed* pclEnchantNeed, SI32 siSuccessUpItemNum ,BOOL bUseBlessingStone , BOOL bUseSealingUpIgnore )
{
	if( pclEnchantNeed == NULL )			return;

 	stEnchantNeed EnchantNeedItem;	

 	for(SI32 i=0; i< MAX_ENCHANT_NUMBER; i++) //우선 초기화 ㅋㅋ
	{
		EnchantNeedItem.siElementUnique[i] =0;
		EnchantNeedItem.siElementNum[i] =0;
	}
	EnchantNeedItem.EnchantRate = 0.0f;
	EnchantNeedItem.EnchantDownRate = 0.0f;
	EnchantNeedItem.EnchantFailRate = 0.0f;
	EnchantNeedItem.siNeedSkill =0;

	SI32 siItemGrade = cltOrgItem.Element.siPower;
	SI32 siItemElement = cltOrgItem.Element.siType;

	SI32 siItemType = 0;
	
	if(cltOrgItem.Element.siPower >= 0 && cltOrgItem.Element.siPower < MAX_ENCHANT_GRADE)
		siItemGrade = cltOrgItem.Element.siPower + 1;


	if(siItemGrade <= 1)
	{
		EnchantNeedItem.EnchantRate = 1.00f;
		EnchantNeedItem.EnchantDownRate = 0.0f;
		EnchantNeedItem.EnchantFailRate = 0.0f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1;  
		
	}	
	else if(siItemGrade == 2)
	{
		EnchantNeedItem.EnchantRate = 0.80f;
		EnchantNeedItem.EnchantDownRate = 0.0f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] =cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1;
		
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A  || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러 
		}
		
		EnchantNeedItem.siElementNum[1] = 5;
	
		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 5;
		}

	}
	else if(siItemGrade == 3)
	{
		EnchantNeedItem.EnchantRate = 0.60f;
		EnchantNeedItem.EnchantDownRate = 0.10f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] =cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 	
		if(siItemElement == ENCHANT_TYPE_FIRE_A ||  siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러
		}
		
		EnchantNeedItem.siElementNum[1] =5;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 5;
		}

	}

	else if(siItemGrade == 4)
	{
		EnchantNeedItem.EnchantRate = 0.40f;
		EnchantNeedItem.EnchantDownRate = 0.20f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] =cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 	
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		
		EnchantNeedItem.siElementNum[1] = 7 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 7;
		}

	}

	
	else if(siItemGrade == 5)
	{
		EnchantNeedItem.EnchantRate = 0.30f;
		EnchantNeedItem.EnchantDownRate = 0.20f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		EnchantNeedItem.siElementNum[1] = 7 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 7;
		}

	}

	else if(siItemGrade == 6)
	{
		EnchantNeedItem.EnchantRate = 0.20f;
		EnchantNeedItem.EnchantDownRate = 0.20f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		EnchantNeedItem.siElementNum[1] = 10 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 10;
		}

	}

	else if(siItemGrade == 7)
	{
		EnchantNeedItem.EnchantRate = 0.10f;
		EnchantNeedItem.EnchantDownRate = 0.25f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		EnchantNeedItem.siElementNum[1] = 10 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 10;
		}

	}
	else if(siItemGrade == 8)
	{
		EnchantNeedItem.EnchantRate = 0.05f ;
		EnchantNeedItem.EnchantDownRate = 0.25f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		EnchantNeedItem.siElementNum[1] = 15 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 30;
		}

	}
	else if(siItemGrade == 9)
	{
		EnchantNeedItem.EnchantRate =  0.01f;
		EnchantNeedItem.EnchantDownRate = 0.25f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		EnchantNeedItem.siElementNum[1] = 15 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 40;
		}

	}
	else if(siItemGrade == 10)
	{
		EnchantNeedItem.EnchantRate = 0.001f ;
		EnchantNeedItem.EnchantDownRate = 0.30f;
		EnchantNeedItem.EnchantFailRate = 0.05f;
		EnchantNeedItem.siElementUnique[0] = cltOrgItem.siUnique;
		EnchantNeedItem.siElementNum[0] = 1; 
		if(siItemElement == ENCHANT_TYPE_FIRE_A || siItemElement == ENCHANT_TYPE_FIRE_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5350);
		}
		else if(siItemElement == ENCHANT_TYPE_WATER_A || siItemElement == ENCHANT_TYPE_WATER_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5355);
		}
		else if(siItemElement == ENCHANT_TYPE_WIND_A || siItemElement == ENCHANT_TYPE_WIND_D )
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5360);
		}
		else if(siItemElement == ENCHANT_TYPE_GROUND_A || siItemElement == ENCHANT_TYPE_GROUND_D)
		{
			EnchantNeedItem.siElementUnique[1] = ITEMUNIQUE(5365);
		}
		else
		{
			// 에러처리 해야함 와선 안되는곳
		}
		EnchantNeedItem.siElementNum[1] =  20 ;
		//		EnchantNeedItem.siNeedSkill = 40 ;

		if(bUseBlessingStone)
		{
			EnchantNeedItem.siElementUnique[2] = ITEMUNIQUE(5370);
			EnchantNeedItem.siElementNum[2] = 100;
		}

	}	
	

	if( siSuccessUpItemNum )
	{
		EnchantNeedItem.siElementUnique[3] = ITEMUNIQUE(13606);
		EnchantNeedItem.siElementNum[3] = siSuccessUpItemNum;
		
		REAL32 SuccessUpRate = 0.01f * siSuccessUpItemNum;

        SuccessUpRate = min(0.10f, SuccessUpRate);

		EnchantNeedItem.EnchantRate += SuccessUpRate;

	}
	
	if( bUseSealingUpIgnore )
	{
		EnchantNeedItem.siElementUnique[4] = ITEMUNIQUE(13607);
		EnchantNeedItem.siElementNum[4] = 1;
	}

	if( bUseBlessingStone )
	{
		EnchantNeedItem.EnchantFailRate = 0.0f;
		EnchantNeedItem.EnchantDownRate = 0.0f;
	}

	EnchantNeedItem.siItemGrade = siItemGrade;


	//return EnchantNeedItem;
	memcpy(pclEnchantNeed, &EnchantNeedItem, sizeof(EnchantNeedItem));
}

/*
SI32 cltItemManagerCommon::GetGradeItem(cltItem cltOrgItem )
{
	if(cltOrgItem.Element.siPower == 0 )      //0단계
	{
		return 20;        
	}
	else if(cltOrgItem.Element.siPower == 20 ) //1단계
	{
		return 30;        
	}
	else if(cltOrgItem.Element.siPower == 30 )  //2단계
	{
		return 60;        
	}
	else if(cltOrgItem.Element.siPower == 60 )  //3단계
	{
		return 100;        
	}
	else if(cltOrgItem.Element.siPower == 100 )  //4단계
	{
		return 150;        
	}
	else if(cltOrgItem.Element.siPower == 150 )  //5단계
	{
		return 250;        
	}
	else if(cltOrgItem.Element.siPower == 250 )  //6단계
	{
		return 450;        
	}

	return 0;

}
*/
/*

SI32 cltItemManagerCommon::GetEnchantGrade(SI32 siEnchantDamage)
{
	SI32 siEnchantGrade =0;


	switch(siEnchantDamage)
	{
	case 0:
		siEnchantGrade = 0;
		break;
	case 20:
		siEnchantGrade = 1;
		break;
	case 30:
		siEnchantGrade = 2;
		break;
	case 60:
		siEnchantGrade = 3;
		break;
	case 100:
		siEnchantGrade = 4;
		break;
	case 150:
		siEnchantGrade = 5;
		break;
	case 250:
		siEnchantGrade = 6;
		break;
	case 450:
		siEnchantGrade = 7;
		break;
	default:
		siEnchantGrade = 0;
		break;		
	}

	return siEnchantGrade;
}
*/


SI32 cltItemManagerCommon::GetEnchantDamageFromGrade(SI32 siGrade)
{
	SI32 siEnchantDamage =0;

	switch(siGrade)
	{
	case 0:
		siEnchantDamage = 0;
		break;
	case 1:
		siEnchantDamage = 20;
		break;
	case 2:
		siEnchantDamage = 30;
		break;
	case 3:
		siEnchantDamage = 60;
		break;
	case 4:
		siEnchantDamage = 100;
		break;
	case 5:
		siEnchantDamage = 150;
		break;
	case 6:
		siEnchantDamage = 200;
		break;
	case 7:
		siEnchantDamage = 250;
		break;
	case 8:
		siEnchantDamage = 320;
		break;
	case 9:
		siEnchantDamage = 400;
		break;
	case 10:
		siEnchantDamage = 500;
		break;
	default:
		siEnchantDamage = 0;
		break;		
	}

	return siEnchantDamage;
}

/*
TCHAR* cltItemManagerCommon::GetEnchantTypeName(TCHAR* szEnchantName, SI32 siEnchantType)
{


	switch(siEnchantType)
	{
	case ENCHANT_TYPE_FIRE_A:
		strcpy(szEnchantName, "불공격");
		break;
	case ENCHANT_TYPE_FIRE_D:
		strcpy(szEnchantName, "불방어");
		break;
	case ENCHANT_TYPE_WATER_A:
		strcpy(szEnchantName, "물공격");
		break;
	case ENCHANT_TYPE_WATER_D:
		strcpy(szEnchantName, "물방어");
		break;
	case ENCHANT_TYPE_WIND_A:
		strcpy(szEnchantName, "바람공격");
		break;
	case ENCHANT_TYPE_WIND_D:
		strcpy(szEnchantName, "바람방어");
		break;
	case ENCHANT_TYPE_GROUND_A:
		strcpy(szEnchantName, "흙공격");
		break;
	case ENCHANT_TYPE_GROUND_D:
		strcpy(szEnchantName, "흙방어");
		break;
	default:
		strcpy(szEnchantName, "없음");
		break;		
	}

	return szEnchantName;
}
*/
bool cltItemManagerCommon::IsCheckSkill(cltItem clOutputItem, SI32 materialcharunique)
{
	SI32 siItemType = pclClient->pclItemManager->GetItemType(clOutputItem.siUnique);
	bool simakeflag=true;
	cltPItemInfo*  pclmaterialinv = &pclClient->pclCM->CR[materialcharunique]->pclCI->clCharItem;
	stEnchantNeed materialunique1;
	GetEnchantNeedItem(clOutputItem, &materialunique1);

	switch(siItemType)
	{
	case ITEMTYPE_SWORD:	
		{
			SI32 skillunique  = SKILL_MAKESWORD1;

			SI32 level  = pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;
	case ITEMTYPE_SPEAR:
		{
			SI32 skillunique  = SKILL_MAKESPEAR1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_STAFF:			
		{
			SI32 skillunique  = SKILL_MAKESTAFF1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_AXE:			
		{
			SI32 skillunique  = SKILL_MAKEAXE1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_BOW:			
		{
			SI32 skillunique  = SKILL_MAKEBOW1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_GUN:
		{
			SI32 skillunique  = SKILL_MAKEGUN1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;		
	case ITEMTYPE_ARMOUR:
		{
			SI32 skillunique  = SKILL_MAKEARMOUR1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;	
	case ITEMTYPE_DRESS:
		{
			SI32 skillunique  = SKILL_MAKEDRESS1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_BELT:
		{
			SI32 skillunique  = SKILL_MAKEBELT1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_SHOES:
		{
			SI32 skillunique  = SKILL_MAKESHOES1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_HELMET:
		{
			SI32 skillunique  = SKILL_MAKEHELMET1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_HAT:
		{
			SI32 skillunique  = SKILL_MAKEHAT1;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;		


		}			
		break;

	case ITEMTYPE_AVATAR_MANTLE:
		{
			SI32 skillunique  = SKILL_MAKEAVATAR_MANTLE;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;

	case ITEMTYPE_AVATAR_NECK:
		{
			SI32 skillunique  = SKILL_MAKEAVATAR_NECK;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;

		
	case ITEMTYPE_AVATAR_HAT:
		{
			SI32 skillunique  = SKILL_MAKEAVATAR_HAT;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;

	case ITEMTYPE_AVATAR_RING:
		{
			SI32 skillunique  = SKILL_MAKEAVATAR_RING;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;
		

	case ITEMTYPE_AVATAR_DRESS:
		{
			SI32 skillunique  = SKILL_MAKEAVATAR_DRESS;
			SI32 level  =pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;
		

	case ITEMTYPE_CRYSTAL:	
		{
			SI32 skillunique  = SKILL_ARTIFACT;

			SI32 level  = pclClient->pclCM->CR[materialcharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;

	}



	return simakeflag;
}



// 농사 가능한 품목의 리스트를 얻는다. (plist를 할당하고 정보를 추가한후에 리턴한다.) 
bool cltItemManagerCommon::GetFarmItemList(SI32* pitemnum, SI32** plist)
{
	if( (*plist) != NULL )
		return false;


	*pitemnum	= 14;
	(*plist) = new SI32[*pitemnum];

	(*plist)[0] = ITEMUNIQUE(3600);// 쌀 
	(*plist)[1] = ITEMUNIQUE(3605);// 무 
	(*plist)[2] = ITEMUNIQUE(3610);// 미나리
	(*plist)[3] = ITEMUNIQUE(3615);// 표고버섯
	(*plist)[4] = ITEMUNIQUE(3620);// 녹두
	(*plist)[5] = ITEMUNIQUE(3625);// 감자
	(*plist)[6] = ITEMUNIQUE(3630);// 고구마
	(*plist)[7] = ITEMUNIQUE(3635);// 양파
	(*plist)[8] = ITEMUNIQUE(3640);// 오이
	(*plist)[9] = ITEMUNIQUE(3645);// 당근
	(*plist)[10] = ITEMUNIQUE(3650);// 마늘
	(*plist)[11] = ITEMUNIQUE(3655);// 콩
	(*plist)[12] = ITEMUNIQUE(3660);// 참깨
	(*plist)[13] = ITEMUNIQUE(3665);// 밀


	return true;
}

// 무역상인이 취급할 품목을 획득한다. 
bool cltItemManagerCommon::GetTraderItem(SI16* pitemunique, SI16* pitemnum, GMONEY* pitemprice)
{
	SI32 i;
	SI32 list[MAX_ITEMINFO_NUMBER];
	SI32 index	= 0;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		list[i]			= 0;
	}

	for(i = 0; i < MAX_ITEMINFO_NUMBER;i++)
	{
		if( pclItemInfo[i] == NULL )
			continue;
		// 거래 가능한 물품이어야 한다
		if( pclItemInfo[i]->IsItemInfoAtb(ITEMINFOATB_NOTTRADE) )
			continue;	
		// 제조 가능한 물품이어야 한다.
		if( 0 == pclItemInfo[i]->clProductionInfo.clMaterial[0].siUnique )
			continue;	

		SI32 type = pclItemInfo[i]->siType;

		if(pclItemTypeInfo[type]->IsAtb(ITEMATB_OBJECT) == false)continue;			// 오브젝트 성격이어야 한다. 

		cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, pclItemInfo[i]->clItem.siUnique);
		if(pclprice <= 0)continue;
		if(pclprice->siPrice < 10000)continue;										// 시세가 1만냥 이하인 물품은 제외.
		if(pclprice->siPrice > 10000000)continue;									// 시세가 1000만냥 이상인 물품은 제외.


		if(clItemStatistic.siItemTradeStatistic[i] > 100)continue;					// 거래 건수가 100건 이상인 품목은 제외. 

		// 총 거래 금액을 구한다. 
		INT64 totaltradeprice = pclprice->siPrice * clItemStatistic.siItemTradeStatistic[i];
		if(totaltradeprice > 100000000)continue;		// 거래 금액 규모가 1억 이상이면 제외. 

		list[index] = i;
		index ++;
	}

	if(index > 0)
	{
		SI32 ref = list[rand() % index];

		*pitemunique	= pclItemInfo[ref]->clItem.siUnique;

		cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, pclItemInfo[ref]->clItem.siUnique);
		*pitemprice		= ((pclprice->siPrice / 10000) + 1) * 10000 * 150 / 100;	// 시세의 150% 
		if(*pitemprice == 0)*pitemprice = 1000;
		

		*pitemnum		= min(1000, 100000000 / *pitemprice);

		return true;
	}



	return false;

}



// 특정 속성이 아이템인포 속성인가. 
bool cltItemManagerCommon::IsItemInfoAtb(SI16 itemunique, SI32 atb)
{
	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref > 0)
	{
		if(pclItemInfo[ref]->siItemInfoAtb & atb)
		{
			return true;
		}
	}

	return false;

}

bool cltItemManagerCommon::IsNewMarketTrade(SI16 itemunique)
{
	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref > 0)
	{
		return pclItemInfo[ref]->m_bNewMarket;
	}

	return false;
}

// 농경을 통해서 얻을 수 있는 기술 경험치.  
SI32 cltItemManagerCommon::GetFarmSkillExp(SI32 itemunique)
{
	cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, itemunique);
	if(pclprice == NULL)return 0;

	SI32 skillexp = min(10, max(1, pclprice->siPrice/ 200));
	
	return skillexp;
}

// 채광을 통해서 얻을 수 있는 기술 경험치.  
SI32 cltItemManagerCommon::GetMineSkillExp(SI32 itemunique)
{
	cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, itemunique);
	if(pclprice == NULL)return 0;

	SI32 skillexp = min(30, max(1, pclprice->siPrice/ 400));

	return skillexp;
}

// 소환수 시전에서 먹을 먹이랑 경험치등등
bool cltItemManagerCommon::GetSummonFoodInfo(SI32* pitemunique,SI32* pitemnum,SI32* pexp,SI32* pskillexp)
{
	SI32 foodlist[] =
	{
		ITEMUNIQUE(3795),	// 해물탕
		ITEMUNIQUE(3800),	// 잡채
		ITEMUNIQUE(3815),	// 붕어매운탕
		ITEMUNIQUE(3820),	// 메기매운탕
		ITEMUNIQUE(3825),	// 잉어찜
		ITEMUNIQUE(2615),	// 평위산
		ITEMUNIQUE(2620),	// 사간탕
		ITEMUNIQUE(2660),	// 당귀보혈탕
		ITEMUNIQUE(2665),	// 향갈탕
		ITEMUNIQUE(2670)	// 반하백출탕
	};

	SI32 index = rand() % 10 ;
	SI32 itemunique = foodlist[index] ;

	if(itemunique > 0)
	{
		*pitemunique	= itemunique;

		cltItemPriceUnit* pclprice = pclItemPrice->GetPrice(this, itemunique);
		SI32 itemnum = 1000 / pclprice->siPrice + 1 ; // 1000은 임시 기준
		itemnum = max(1,itemnum);

		*pitemnum = min(10,itemnum);

		*pexp = max(2,((pclprice->siPrice*itemnum)/1000 )* 2);
		*pskillexp = max(1,*pexp / 2 );

		return true;
	}

	return false ;
}

// 무장의 도력을 구한다. 
SI32 cltItemManagerCommon::GetEquipMag(cltItem* pclitemlist, SI16 activeweapon)
{
	SI32 siMag = 0;
	SI32 returnval = 0;

	SI32 temper = 0;

	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[activeweapon].GetEquipMag(this ,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true) // 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_RING1].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_RING2].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true )// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_HAT].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_DRESS].GetEquipMag(this,&temper);}

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_HELMET].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_ARMOUR].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_NECK]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_NECK].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_BELT].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_SHOES].GetEquipMag(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siMag += pclitemlist[PERSONITEM_MANTLE].GetEquipMag(this,&temper);}

	return siMag;
	//return pclitemlist[activeweapon].GetEquipMag(this) 
	//	+ pclitemlist[PERSONITEM_RING1].GetEquipMag(this)
	//	+ pclitemlist[PERSONITEM_RING2].GetEquipMag(this)
	//	+ pclitemlist[PERSONITEM_HAT].GetEquipMag(this)
	//	+ pclitemlist[PERSONITEM_DRESS].GetEquipMag(this);
}
SI32 cltItemManagerCommon::GetEquipMag(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar)
{
	SI32 siMag = 0;
	SI32 temReturnPer = 0;

	siMag = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_MAG ,&temReturnPer);

	// [진성] 아바타 결정체. mag. //KHY - 20090526
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siMag += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_MAG, &temReturnPer);
	//}
	
	siMag += temReturnPer;
	
	return siMag;
}
// 무장의 근력을 구한다.
SI32 cltItemManagerCommon::GetEquipStr(cltItem* pclitemlist , SI16 activeweapon)
{
	SI32 siStr = 0;
	SI32 returnval = 0;

	SI32 temper = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
        		siStr += pclitemlist[PERSONITEM_RING1].GetEquipStr(this,&temper);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)	// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siStr += pclitemlist[PERSONITEM_RING2].GetEquipStr(this,&temper);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siStr += pclitemlist[PERSONITEM_MANTLE].GetEquipStr(this,&temper);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siStr += pclitemlist[PERSONITEM_HAT].GetEquipStr(this,&temper);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siStr += pclitemlist[PERSONITEM_DRESS].GetEquipStr(this,&temper);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siStr += pclitemlist[PERSONITEM_HELMET].GetEquipStr(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siStr += pclitemlist[PERSONITEM_ARMOUR].GetEquipStr(this,&temper);}
	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siStr += pclitemlist[activeweapon].GetEquipStr(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_NECK]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siStr += pclitemlist[PERSONITEM_NECK].GetEquipStr(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siStr += pclitemlist[PERSONITEM_BELT].GetEquipStr(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siStr += pclitemlist[PERSONITEM_SHOES].GetEquipStr(this,&temper);}

	return siStr;
}
// 무장의 근력을 구한다.
SI32 cltItemManagerCommon::GetEquipStr(cltItem* pclitemlist, SI16 activeweapon,cltCharCommon* pclchar )
{
	SI32 siStr = 0;
	SI32 temReturnPer = 0;
	
	siStr  = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_STR, &temReturnPer);

	// [진성] 아바타 결정체. str. //KHY - 20090526
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siStr += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_STR, &temReturnPer);
	//}
	

	siStr += temReturnPer;
	
	return siStr;
}
// 무장의 순발력을 구한다. 
SI32 cltItemManagerCommon::GetEquipDex(cltItem* pclitemlist ,SI16 activeweapon)
{
	SI32 siDex = 0;
	SI32 returnval = 0;

	SI32 temper = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_RING1].GetEquipDex(this,&temper);}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_RING2].GetEquipDex(this,&temper);}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true )// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_MANTLE].GetEquipDex(this,&temper);	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_HAT].GetEquipDex(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true )// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_DRESS].GetEquipDex(this,&temper);}

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_HELMET].GetEquipDex(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_ARMOUR].GetEquipDex(this,&temper);}
	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[activeweapon].GetEquipDex(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_NECK]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_NECK].GetEquipDex(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_BELT].GetEquipDex(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siDex += pclitemlist[PERSONITEM_SHOES].GetEquipDex(this,&temper);}

	return siDex;
	//return pclitemlist[PERSONITEM_RING1].GetEquipDex(this)
	//	+ pclitemlist[PERSONITEM_RING2].GetEquipDex(this)
	//	+ pclitemlist[PERSONITEM_MANTLE].GetEquipDex(this)
	//	+ pclitemlist[PERSONITEM_HAT].GetEquipDex(this)
	//	+ pclitemlist[PERSONITEM_DRESS].GetEquipDex(this);
}

// 무장의 순발력을 구한다. 
SI32 cltItemManagerCommon::GetEquipDex(cltItem* pclitemlist, SI16 activeweapon,cltCharCommon* pclchar )
{
	SI32 siDex = 0;
	SI32 temReturnPer = 0;

	siDex = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_DEX ,&temReturnPer);

	// [진성] 아바타 결정체. dex. //KHY - 20090526
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siDex += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_DEX, &temReturnPer);
	//}

	siDex +=temReturnPer;
	
	return siDex;	
}
// 무장의 생명력을 구한다. 
SI32 cltItemManagerCommon::GetEquipVit(cltItem* pclitemlist ,SI16 activeweapon)
{
	SI32 siVit = 0;
	SI32 returnval = 0;

	SI32 temper = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_RING1].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_RING2].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_MANTLE].GetEquipVit(this,&temper);}	
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_HAT].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_DRESS].GetEquipVit(this,&temper);}

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_HELMET].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_ARMOUR].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[activeweapon].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_NECK]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_NECK].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_BELT].GetEquipVit(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siVit += pclitemlist[PERSONITEM_SHOES].GetEquipVit(this,&temper);}

	return siVit;
	//return pclitemlist[PERSONITEM_RING1].GetEquipVit(this)
	//	+ pclitemlist[PERSONITEM_RING2].GetEquipVit(this)
	//	+ pclitemlist[PERSONITEM_MANTLE].GetEquipVit(this)
	//	+ pclitemlist[PERSONITEM_HAT].GetEquipVit(this)
	//	+ pclitemlist[PERSONITEM_DRESS].GetEquipVit(this);

}

// 무장의 생명력을 구한다. 
SI32 cltItemManagerCommon::GetEquipVit(cltItem* pclitemlist, SI16 activeweapon,cltCharCommon* pclchar)
{
	SI32 siVit = 0;
	SI32 temReturnPer = 0;

	siVit = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_VIT,&temReturnPer);

	// [진성] 아바타 결정체. vit.
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siVit += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_VIT, &temReturnPer);
	//}
	

	siVit  += temReturnPer;

	return siVit;	
}
// 무장의 운을 구한다. 
SI32 cltItemManagerCommon::GetEquipLuk(cltItem* pclitemlist ,SI16 activeweapon)
{
	SI32 siLuk = 0;
	SI32 returnval = 0;

	SI32 temper = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_RING1].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_RING2].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_DRESS].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_HAT].GetEquipLuk(this,&temper);}

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_HELMET].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_ARMOUR].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[activeweapon].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_NECK]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_NECK].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_BELT].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_SHOES].GetEquipLuk(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siLuk += pclitemlist[PERSONITEM_MANTLE].GetEquipLuk(this,&temper);}	
	return siLuk;
}

// 무장의 운을 구한다. 
SI32 cltItemManagerCommon::GetEquipLuk(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar )
{
	SI32 siLuk = 0;
	SI32 temReturnPer = 0;
	
	siLuk = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_LUK,&temReturnPer);

	// [진성] 아바타 결정체. luk.
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siLuk += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_LUK, &temReturnPer);
	//}

	siLuk +=temReturnPer;

	return siLuk;
}
// 무장의 손재주을 구한다. 
SI32 cltItemManagerCommon::GetEquipHnd(cltItem* pclitemlist , SI16 activeweapon)
{
	SI32 siHnd = 0;
	SI32 temper = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_RING1].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)	// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_RING2].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true)	// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_DRESS].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true )	// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_HAT].GetEquipHnd(this,&temper);}

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_HELMET].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_ARMOUR].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[activeweapon].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_NECK]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_NECK].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_BELT].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_SHOES].GetEquipHnd(this,&temper);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siHnd += pclitemlist[PERSONITEM_MANTLE].GetEquipHnd(this,&temper);}	
	return siHnd;
}


// 무장의 손재주을 구한다. 
SI32 cltItemManagerCommon::GetEquipHnd(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar )
{
	SI32 siHnd = 0;
	SI32 temReturnPer = 0;
	
	siHnd = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_HND,&temReturnPer);

	// [진성] 아바타 결정체. hand.
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siHnd += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_HND, &temReturnPer);
	//}

	siHnd += temReturnPer;

	return siHnd;
}

// 무장의 손재주을 구한다. 
SI32 cltItemManagerCommon::GetEquipWis(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar )
{
	SI32 siWis = 0;
	SI32 temReturnPer = 0;
	
	siWis = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_STATE_WIS, &temReturnPer);

	// [진성] 아바타 결정체. vit.
	//if( pclClient->IsCountrySwitch( Switch_Avatar_artisan ) )
	//{
	//	siWis += GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_AVATAR_STATE_WIS, &temReturnPer );
	//}

	siWis += temReturnPer;

	return siWis;
}


// 무장의 공격타입을 구한다. 
SI32 cltItemManagerCommon::GetWeaponAttackType(cltItem* pclitemlist, SI16 activeweapon)
{
	return pclitemlist[activeweapon].GetAttackType(this);
}

// 무장의 사거리를 구한다. 
SI32 cltItemManagerCommon::GetWeaponAttackRange(cltItem* pclitemlist, SI16 activeweapon)
{
	return pclitemlist[activeweapon].GetAttackRange(this);
}

// 무장의 공격 주기를 구한다. 
SI32 cltItemManagerCommon::GetWeaponAttackInterval(cltItem* pclitemlist, SI16 activeweapon)
{
	return pclitemlist[activeweapon].GetAttackInterval(this);
}

// 장비의 운을  구한다. 
SI32 cltItemManagerCommon::GetLuck(cltItem* pclitemlist)
{
	SI32 siLuck = 0;

// 안쓴다.
/*
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siLuck += pclitemlist[PERSONITEM_ARMOUR].GetLuck(this);

	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siLuck += pclitemlist[PERSONITEM_HELMET].GetLuck(this);
*/
	if(pclitemlist[PERSONITEM_HAT].siUnique > 0)
	{
		if(IsExpiredDate(&pclitemlist[PERSONITEM_HAT]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
			siLuck += pclitemlist[PERSONITEM_HAT].GetLuck(this);
	}

	if(pclitemlist[PERSONITEM_DRESS].siUnique > 0)
	{
		if(IsExpiredDate(&pclitemlist[PERSONITEM_DRESS]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
			siLuck += pclitemlist[PERSONITEM_DRESS].GetLuck(this);
	}
	return siLuck;

	//SI32 ac = pclitemlist[PERSONITEM_ARMOUR].GetLuck(this) 
	//		+ pclitemlist[PERSONITEM_HELMET].GetLuck(this)
	//		+ pclitemlist[PERSONITEM_HAT].GetLuck(this)
	//		+ pclitemlist[PERSONITEM_DRESS].GetLuck(this);

	//return ac;
}


// 무장의 AC를 구한다. 
SI32 cltItemManagerCommon::GetAC(cltItem* pclitemlist)
{
	SI32		siAC = 0;
	SI32		returnval = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siAC += pclitemlist[PERSONITEM_HELMET].GetAC(this);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true)// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siAC += pclitemlist[PERSONITEM_ARMOUR].GetAC(this);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true )// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siAC += pclitemlist[PERSONITEM_SHOES].GetAC(this);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true )// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siAC += pclitemlist[PERSONITEM_BELT].GetAC(this);}
	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true )// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
	{siAC += pclitemlist[PERSONITEM_MANTLE].GetAC(this);}
	return siAC;

	// 투구 방어력, 
	//SI32 helmetac = pclitemlist[PERSONITEM_HELMET].GetAC(this);
	//if(helmetac)
	//{
	//	helmetac += pclitemlist[PERSONITEM_HAT].GetAC(this) / DRESS_ADD_DPOWER_RATE;
	//}
	//else
	//{
	//	helmetac = pclitemlist[PERSONITEM_HAT].GetAC(this);
	//}

	// 갑옷 방어력, 
	//SI32 armourac = pclitemlist[PERSONITEM_ARMOUR].GetAC(this);
	//if(armourac)
	//{
	//	armourac += pclitemlist[PERSONITEM_DRESS].GetAC(this) / DRESS_ADD_DPOWER_RATE;
	//}
	//else
	//{
	//	armourac = pclitemlist[PERSONITEM_DRESS].GetAC(this);
	//}


	//SI32 ac = helmetac 
	//		+ armourac
	//		+ pclitemlist[PERSONITEM_SHOES].GetAC(this)
	//		+ pclitemlist[PERSONITEM_BELT].GetAC(this)
	//		+ pclitemlist[PERSONITEM_MANTLE].GetAC(this);

	//return ac;
}
// 무장의 AC를 구한다. 
SI32 cltItemManagerCommon::GetAC(cltItem* pclitemlist, cltCharCommon* pclchar)
{
	SI32		siAC = 0;
	SI32		returnval = 0;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == true &&// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		CheckCondition(pclitemlist[PERSONITEM_HELMET].siUnique, pclchar, &returnval) == true)	//아이템의 스탯이 맞아야 한다.맞지 않으면 반영안됨[2007.08.06 손성웅]	
	{
		siAC += pclitemlist[PERSONITEM_HELMET].GetAC(this);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == true &&// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		CheckCondition(pclitemlist[PERSONITEM_ARMOUR].siUnique, pclchar, &returnval) == true)		//아이템의 스탯이 맞아야 한다.맞지 않으면 반영안됨[2007.08.06 손성웅]
	{
		siAC += pclitemlist[PERSONITEM_ARMOUR].GetAC(this);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true &&// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		CheckCondition(pclitemlist[PERSONITEM_SHOES].siUnique, pclchar, &returnval) == true)		//아이템의 스탯이 맞아야 한다.맞지 않으면 반영안됨[2007.08.06 손성웅]
	{
		siAC += pclitemlist[PERSONITEM_SHOES].GetAC(this);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true &&// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		CheckCondition(pclitemlist[PERSONITEM_BELT].siUnique, pclchar, &returnval) == true)		//아이템의 스탯이 맞아야 한다.맞지 않으면 반영안됨[2007.08.06 손성웅]
	{
		siAC += pclitemlist[PERSONITEM_BELT].GetAC(this);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true &&// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		CheckCondition(pclitemlist[PERSONITEM_MANTLE].siUnique, pclchar, &returnval) == true)		//아이템의 스탯이 맞아야 한다.맞지 않으면 반영안됨[2007.08.06 손성웅]
	{
		siAC += pclitemlist[PERSONITEM_MANTLE].GetAC(this);
	}

	return siAC;
}


// 인챈트 관련 AC를 구한다. 
SI32 cltItemManagerCommon::GetEnchantAC(cltItem* pclitemlist,cltCharCommon* pclchar)
{
	SI32 returnval = 0;
	bool bClient = false ;
	if ( pclClient->GameMode == GAMEMODE_CLIENT )
		bClient = true ;

	// 투구 속성 방어력, 
	SI32 helmetac = pclitemlist[PERSONITEM_HELMET].GetEnchantAC(this,bClient);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == false  && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_HELMET].siUnique, pclchar, &returnval) == false)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
		helmetac = 0;
	
	if(helmetac)
	{
		helmetac += pclitemlist[PERSONITEM_HAT].GetEnchantAC(this,bClient) / DRESS_ADD_DPOWER_RATE;
	}
	else
	{
		helmetac = pclitemlist[PERSONITEM_HAT].GetEnchantAC(this,bClient);
	}

	// 갑옷 속성 방어력, 
	SI32 armourac = pclitemlist[PERSONITEM_ARMOUR].GetEnchantAC(this,bClient);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == false && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_ARMOUR].siUnique, pclchar, &returnval) == false)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
		armourac = 0;

	if(armourac)
	{
		armourac += pclitemlist[PERSONITEM_DRESS].GetEnchantAC(this,bClient) / DRESS_ADD_DPOWER_RATE;
	}
	else
	{
		armourac = pclitemlist[PERSONITEM_DRESS].GetEnchantAC(this,bClient);
	}

	SI32 ac = helmetac + armourac;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true  && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_SHOES].siUnique, pclchar, &returnval) == true)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
	{
		ac += pclitemlist[PERSONITEM_SHOES].GetEnchantAC(this,bClient);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_BELT].siUnique, pclchar, &returnval) == true)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
	{
		ac += pclitemlist[PERSONITEM_BELT].GetEnchantAC(this,bClient);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_MANTLE].siUnique, pclchar, &returnval) == true)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
	{
		ac += pclitemlist[PERSONITEM_MANTLE].GetEnchantAC(this,bClient);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_RING1].siUnique, pclchar, &returnval) == true)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
	{
		ac += pclitemlist[PERSONITEM_RING1].GetEnchantAC(this,bClient);
	}

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true && // 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		CheckCondition(pclitemlist[PERSONITEM_RING2].siUnique, pclchar, &returnval) == true)//스탯이 맞지 않으면 능력x[2007.08.07 손성웅]
	{
		ac += pclitemlist[PERSONITEM_RING2].GetEnchantAC(this,bClient);
	}

		//+ pclitemlist[PERSONITEM_SHOES].GetEnchantAC(this,bClient)
		//+ pclitemlist[PERSONITEM_BELT].GetEnchantAC(this,bClient)
		//+ pclitemlist[PERSONITEM_MANTLE].GetEnchantAC(this,bClient)
		//+ pclitemlist[PERSONITEM_RING1].GetEnchantAC(this,bClient)
		//+ pclitemlist[PERSONITEM_RING2].GetEnchantAC(this,bClient);

	return ac;
}


// 인챈트 관련 AC를 구한다. 
SI32 cltItemManagerCommon::GetEnchantACFrom(cltItem* pclitemlist, SI32 attacktype)
{
	bool bClient = false ;
	if ( pclClient->GameMode == GAMEMODE_CLIENT )
		bClient = true ;

	// 투구 속성 방어력, 
	SI32 helmetac = pclitemlist[PERSONITEM_HELMET].GetEnchantACFrom(this,bClient, attacktype);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_HELMET]) == false)		// 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		helmetac = 0;

	if(helmetac)
	{
		helmetac += pclitemlist[PERSONITEM_HAT].GetEnchantACFrom(this,bClient , attacktype) / DRESS_ADD_DPOWER_RATE;
	}
	else
	{
		helmetac = pclitemlist[PERSONITEM_HAT].GetEnchantACFrom(this,bClient, attacktype);
	}


	// 갑옷 속성 방어력, 
	SI32 armourac = pclitemlist[PERSONITEM_ARMOUR].GetEnchantACFrom(this,bClient, attacktype);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_ARMOUR]) == false)		// 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		armourac = 0;

	if(armourac)
	{
		armourac += pclitemlist[PERSONITEM_DRESS].GetEnchantACFrom(this,bClient, attacktype) / DRESS_ADD_DPOWER_RATE;
	}
	else
	{
		armourac = pclitemlist[PERSONITEM_DRESS].GetEnchantACFrom(this,bClient ,attacktype );
	}


	SI32 ac = helmetac + armourac;

	if(IsExpiredDate(&pclitemlist[PERSONITEM_SHOES]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		ac += pclitemlist[PERSONITEM_SHOES].GetEnchantACFrom(this,bClient, attacktype);

	if(IsExpiredDate(&pclitemlist[PERSONITEM_BELT]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		ac += pclitemlist[PERSONITEM_BELT].GetEnchantACFrom(this,bClient, attacktype);

	if(IsExpiredDate(&pclitemlist[PERSONITEM_MANTLE]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		ac += pclitemlist[PERSONITEM_MANTLE].GetEnchantACFrom(this,bClient, attacktype);

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		ac += pclitemlist[PERSONITEM_RING1].GetEnchantACFrom(this,bClient, attacktype);

	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		ac += pclitemlist[PERSONITEM_RING2].GetEnchantACFrom(this,bClient, attacktype);

		//+ pclitemlist[PERSONITEM_SHOES].GetEnchantACFrom(this,bClient, attacktype)
		//+ pclitemlist[PERSONITEM_BELT].GetEnchantACFrom(this,bClient ,attacktype )
		//+ pclitemlist[PERSONITEM_MANTLE].GetEnchantACFrom(this,bClient, attacktype)
		//+ pclitemlist[PERSONITEM_RING1].GetEnchantACFrom(this,bClient, attacktype)
		//+ pclitemlist[PERSONITEM_RING2].GetEnchantACFrom(this,bClient, attacktype);

	return ac;
}

// 인챈트 관련 AC를 구한다. 
SI32 cltItemManagerCommon::GetEnchantAttFrom(cltItem* pclitemlist, SI32 attacktype)
{
	bool bClient = false ;
	if ( pclClient->GameMode == GAMEMODE_CLIENT )
		bClient = true ;

	// 링1 속성 방어력, 
	SI32 ring1 = pclitemlist[PERSONITEM_RING1].GetEnchantAttFrom(this,bClient, attacktype);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING1]) == false)		// 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		ring1 = 0;


	// 링2 속성 방어력, 
	SI32 ring2 = pclitemlist[PERSONITEM_RING2].GetEnchantAttFrom(this,bClient, attacktype);
	if(IsExpiredDate(&pclitemlist[PERSONITEM_RING2]) == false)		// 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
		ring2 = 0;

	
	SI32 weapon =0;

	if(bClient)
	{
		SI32 activeweapon = pclClient->pclCM->CR[1]->ActiveWeapon; 
		weapon = pclitemlist[activeweapon].GetEnchantAttFrom(this,bClient, attacktype);
		if(IsExpiredDate(&pclitemlist[activeweapon]) == false)		// 기간이 지난 아이템은 효력이 사라진다. - by LEEKH 2007.04.11
			weapon = 0;
	}



	SI32 att = ring1 + ring2 + weapon;

	return att;
}
// 무장의 최소 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon)
{
	SI32	siMinDamage = 0;
	SI32	returnval = 0;

	SI32 CrystalMinDamagePer = 0;

	if((IsExpiredDate(&pclitemlist[activeweapon]) == true)&&(pclitemlist[activeweapon].siUnique > 0))		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siMinDamage += pclitemlist[activeweapon].GetMinDamage(this ,&CrystalMinDamagePer);
	return siMinDamage;
}
SI32 cltItemManagerCommon::GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar )
{
	SI32	siMinDamage = 0;
	SI32 CrystalMinDamagePer = 0;
	SI32 returnval = 0;

	if((IsExpiredDate(&pclitemlist[activeweapon]) == true)&&(pclitemlist[activeweapon].siUnique > 0)&&
		CheckCondition(pclitemlist[PERSONITEM_HELMET].siUnique, pclchar, &returnval) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siMinDamage += pclitemlist[activeweapon].GetMinDamage(this ,&CrystalMinDamagePer);

	return siMinDamage;

}


// 무장의 최소 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar  , SI32 * CrystalPer)
{
	SI32	siMinDamage = 0;
	//SI32 CrystalMinDamagePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siMinDamage = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK,CrystalPer);

	//siMinDamage = siMinDamage + (siMinDamage * CrystalMinDamagePer / 100);
		
	return siMinDamage;
	//return pclitemlist[activeweapon].GetMinDamage(this);
}

// 소환수의  최소 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltSummon* pclsummon)
{
	SI32		siMinDamage = 0;

	SI32 CrystalMinDamagePer = 0;

	if( IsExpiredDate(&pclitemlist[activeweapon]) == true )		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siMinDamage += pclitemlist[activeweapon].GetMinDamage(this ,&CrystalMinDamagePer);

	return siMinDamage;
	//return pclitemlist[activeweapon].GetMinDamage(this);
}

// 무장의 인챈트 타입을 구한다. 
SI32 cltItemManagerCommon::GetEnchantWeaponType(cltItem* pclitemlist, SI16 activeweapon)
{
	return pclitemlist[activeweapon].GetEnchantType(this);
}

// 무장의 최대 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon)
{
	SI32	siMaxDamage = 0;
	SI32	returnval = 0;

	SI32 CrystalMaxDamagePer = 0;

	if((IsExpiredDate(&pclitemlist[activeweapon]) == true)&&(pclitemlist[activeweapon].siUnique > 0))		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siMaxDamage += pclitemlist[activeweapon].GetMaxDamage(this ,&CrystalMaxDamagePer);

	return siMaxDamage;
	//return pclitemlist[activeweapon].GetMaxDamage(this);
}

// 무장의 최대 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar )
{
	SI32	siMaxDamage = 0;
	SI32	returnval = 0;

	SI32 CrystalMaxDamagePer = 0;

	if((IsExpiredDate(&pclitemlist[activeweapon]) == true)&&(pclitemlist[activeweapon].siUnique > 0)&&
		CheckCondition(pclitemlist[PERSONITEM_HELMET].siUnique, pclchar, &returnval) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siMaxDamage += pclitemlist[activeweapon].GetMaxDamage(this ,&CrystalMaxDamagePer);

	return siMaxDamage;
	//return pclitemlist[activeweapon].GetMaxDamage(this);
}


// 무장의 최대 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar , SI32 * CrystalPer)
{
	SI32	siMaxDamage = 0;
	//SI32 CrystalMaxDamagePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siMaxDamage = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK,CrystalPer);

	//siMaxDamage = siMaxDamage + (siMaxDamage * CrystalMaxDamagePer / 100);	

	return siMaxDamage;
}

// 소환수의 최대 데미지를 구한다. 
SI32 cltItemManagerCommon::GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltSummon* pclsummon )
{
	SI32	siMaxDamage = 0;

	SI32 CrystalMaxDamagePer = 0;

	if(IsExpiredDate(&pclitemlist[activeweapon]) == true)		// 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		siMaxDamage += pclitemlist[activeweapon].GetMaxDamage(this , &CrystalMaxDamagePer);

	return siMaxDamage;
	//return pclitemlist[activeweapon].GetMaxDamage(this);
}

//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
// 마법 공격력을 구한다. - 추가 크리스탈. 
SI32 cltItemManagerCommon::GetAddMagicalMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar )
{
	SI32	siMagicalDamage = 0;
	SI32 CrystalMagicalDamagePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siMagicalDamage = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK,&CrystalMagicalDamagePer);
	
	// 마법력이 몇퍼센트 상승했는지 보낸다. 
	return CrystalMagicalDamagePer;
}


SI32 cltItemManagerCommon::GetAddMagicalMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar )
{
	SI32	siMagicalDamage = 0;
	SI32 CrystalMagicalDamagePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siMagicalDamage = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK,&CrystalMagicalDamagePer);

	// 마법력이 몇퍼센트 상승했는지 보낸다. 
	return CrystalMagicalDamagePer;
}

SI32 cltItemManagerCommon::GetAddMagicalDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar )
{
	SI32	siMagicalDamage = 0;
	SI32 CrystalMagicalDamagePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siMagicalDamage = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MAGICAL_ATTACK,&CrystalMagicalDamagePer);

	// 마법력이 몇퍼센트 상승했는지 보낸다. 
	return CrystalMagicalDamagePer;
}

// 장비에의한 기술 향상을 구한다. 
//KHY - 0320 - 공회전 방지. - 사용기간이 남아있고, 장착하고 있는 아이템에 한하여 검사한다.
SI32 cltItemManagerCommon::GetUpgradeSkillLevel(cltItem* pclitemlist, SI32 skillunique, SI32 id )
{
	SI32 skilllevel = 0;
	SI32 crySkilllevel = 0;
	SI32 returnval = 0;
	SI32 tempSkilllevel = 0;
	SI16 activeweapon = PERSONITEM_WEAPON1;
	
	if(pclClient->pclCM->IsValidID(id) == true)
		activeweapon = pclClient->pclCM->CR[id ]->ActiveWeapon;
	
	for(int pitempos=0; pitempos <PERSONITEM_INV0 ; pitempos++)
	{
		switch(pitempos)
		{
		// 검사 아이템의 추가, 및 삭제는 이부분에서  - 
		case PERSONITEM_HELMET:
		case PERSONITEM_ARMOUR:
		case PERSONITEM_NECK:
		case PERSONITEM_RING1:	
		case PERSONITEM_RING2:
		case PERSONITEM_BELT:
		case PERSONITEM_SHOES:
		case PERSONITEM_MANTLE:
		case PERSONITEM_HAT:
		case PERSONITEM_DRESS:
			{
				if(pclitemlist[pitempos].siUnique > 0) // 1 -그 아이템을 장착 하고 있어야 한다.
				{
					if(IsExpiredDate(&pclitemlist[pitempos]) == true) // 2 - 사용기간이 남아 있어야 한다. - 혹은 없는 아이템이거나.
					{
						//if(CheckCondition(pclitemlist[pitempos].siUnique, pclchar, &returnval) == true ) // 3 - 장착 조건을 만족해야만 한다.
						{
							////////////////////////////////////////////////////////////////////////////////////////////
							// 기타 이유로 장비에서 스킬이 올라가는 경우 계산.
							tempSkilllevel += pclitemlist[pitempos].GetUpgradeSkillLevel(this, skillunique ,false); // 크리스탈은 여기서 계산하지 않는다.

							// 결정체로 장비에서 스킬이 올라가는 경우 계산.
							if(pclitemlist[pitempos].siUseCrystalNum>0)
							{
								UI08 uiSkillunique = pclitemlist[pitempos].GetCrystalAbilityFromSkillUnique(skillunique);

								crySkilllevel = pclitemlist[pitempos].GetAllCrystalAbilityNum(uiSkillunique);

								// 각 아이템의 결정체로 올라가는 스킬레벨을 누적 시킨다.
								skilllevel = skilllevel + crySkilllevel; 
							}							
							//////////////////////////////////////////////////////////////////////////////////////////////////
						}
					}
				}
			}
			break;
		default :
			break;
		}
	}

	//  - activeweapon은 어쩔수 없다. - 여기서 따로 검사.
	if((pclitemlist[activeweapon].siUnique > 0) && // 1 -그 아이템을 장착 하고 있어야 한다.
		(IsExpiredDate(&pclitemlist[activeweapon]) == true)) // 2 - 사용기간이 남아 있어야 한다. - 혹은 없는 아이템이거나.
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		// 기타 이유로 장비에서 스킬이 올라가는 경우 계산.
		tempSkilllevel += pclitemlist[activeweapon].GetUpgradeSkillLevel(this, skillunique ,false); // 크리스탈은 여기서 계산하지 않는다.

		// 결정체로 장비에서 스킬이 올라가는 경우 계산.
		if(pclitemlist[activeweapon].siUseCrystalNum>0)
		{
			UI08 uiSkillunique = pclitemlist[activeweapon].GetCrystalAbilityFromSkillUnique(skillunique);

			crySkilllevel = pclitemlist[activeweapon].GetAllCrystalAbilityNum(uiSkillunique);

			// 각 아이템의 결정체로 올라가는 스킬레벨을 누적 시킨다.
			skilllevel = skilllevel + crySkilllevel; 
		}							
		//////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// 세트 아이템에 의한 무기술 증가.
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		switch(skillunique)
		{
			case SKILL_USESWORD1:	
			case SKILL_USESPEAR1:	
			case SKILL_USEBOW1:		
			case SKILL_USEAXE1:	
			case SKILL_USEGUN1:		
			case SKILL_USESTAFF1:		
				{
					if( pclClient->pclCM->CR[id] )
					{
						if( pclClient->pclCM->CR[id]->m_siSetItemType ) 
						{
							tempSkilllevel += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclClient->pclCM->CR[id], SETITEM_ABILITY_TYPE_WEAPON_USE_SKILL, USE_SETITEM_1);
							tempSkilllevel += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclClient->pclCM->CR[id], SETITEM_ABILITY_TYPE_WEAPON_USE_SKILL, USE_SETITEM_2);
							tempSkilllevel += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclClient->pclCM->CR[id], SETITEM_ABILITY_TYPE_WEAPON_USE_SKILL, USE_SETITEM_3);
							tempSkilllevel += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclClient->pclCM->CR[id], SETITEM_ABILITY_TYPE_WEAPON_USE_SKILL, USE_SETITEM_4);
							tempSkilllevel += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclClient->pclCM->CR[id], SETITEM_ABILITY_TYPE_WEAPON_USE_SKILL, USE_SETITEM_5);
						}
					}
				}
				break;
		}
	}

	return (skilllevel + tempSkilllevel);	

}

// 장비의 필살률 향상을 구한다.   (무기에만) 
SI32 cltItemManagerCommon::GetEquipCriticalHitRate(cltItem* pclitemlist, SI16 activeweapon)
{
	SI32 pos;
	SI32 siTotalRate = 0;
	
	for(SI32 i = 0;i < 2;i++)
	{
		if(i == 0)	pos = activeweapon;
		else		pos = PERSONITEM_BELT;

		// 액티브 무기에서 찾는다. 
		cltItem* pclitem = (cltItem*)&pclitemlist[pos];
		
		if(pclitem->siUnique <= 0)continue;

		if( pclitem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_UPCRITICAL)
		{
			siTotalRate += pclitem->clItemCommon.clOptionInfo.uiPara1;
		}
		if( pclitem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_UPCRITICAL)
		{
			siTotalRate += pclitem->clItemCommon.clOptionInfo.uiPara2;
		}
	}

	return siTotalRate;
}

// 장비의 필살률 향상을 구한다. 결정체 -   (모든 장비.) 
SI32 cltItemManagerCommon::GetEquipCrystalCriticalHitRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalCriticalHitRatePer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_SPECIAL_DEATHBLOW,&CrystalCriticalHitRatePer);

	return CrystalCriticalHitRatePer;
}

// 장비의 명중률 향상을 구한다.  (무기에만)
SI32 cltItemManagerCommon::GetEquipHitRate(cltItem* pclitemlist, SI16 activeweapon)
{
	SI32 siTotalRate = 0;

	cltItem* pclitem = (cltItem*)&pclitemlist[activeweapon];
	if(pclitem->siUnique == 0)		return siTotalRate;

	if( pclitem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_UPHITRATE)
	{
		siTotalRate += pclitem->clItemCommon.clOptionInfo.uiPara1;
	}
	if( pclitem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_UPHITRATE)
	{
		siTotalRate += pclitem->clItemCommon.clOptionInfo.uiPara2;
	}
	
	return siTotalRate;
}

// 장비의 명중률 향상을 구한다. 결정체. - 물리
SI32 cltItemManagerCommon::GetEquipCrystalPhysicalHitRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPhysicalHitRatePer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE,&CrystalPhysicalHitRatePer);

	return CrystalPhysicalHitRatePer;
}

// 장비의 명중률 향상을 구한다. 결정체. - 마법.
SI32 cltItemManagerCommon::GetEquipCrystalMagicalHitRate(cltItem* pclitemlist, SI16 activeweapon , cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalMagicalHitRatePer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MAGICAL_HITRATE, &CrystalMagicalHitRatePer);

	return CrystalMagicalHitRatePer;
}

SI32 cltItemManagerCommon::GetEquipCrystalPhysicalDefensiveRate(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar  , SI32 * CrystalPer )
{
	SI32	siDefensive = 0;
	//SI32 CrystalDefensivePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siDefensive = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE,CrystalPer);

	//siDefensive = siDefensive + (siDefensive * CrystalDefensivePer / 100);	

	return siDefensive;
}

SI32 cltItemManagerCommon::GetEquipCrystalMagicalDefensiveRate(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar  , SI32 * CrystalPer)
{
	SI32	siDefensive = 0;
	//SI32 CrystalDefensivePer = 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	siDefensive = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE,CrystalPer);

	//siDefensive = siDefensive + (siDefensive * CrystalDefensivePer / 100);	

	return siDefensive;
}

// 장비의 회피율 향상을 구한다. 결정체. - 물리
SI32 cltItemManagerCommon::GetEquipCrystalPhysicalDodgeRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPhysicalDodgeRatePer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE,&CrystalPhysicalDodgeRatePer);

	return CrystalPhysicalDodgeRatePer;
}

// 장비의 회피율 향상을 구한다. 결정체. - 마법
SI32 cltItemManagerCommon::GetEquipCrystalMagicalDodgeRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalMagicalDodgeRatePer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE,&CrystalMagicalDodgeRatePer);

	return CrystalMagicalDodgeRatePer;
}

// 장비의 체력 향상을 구한다. 결정체.
SI32 cltItemManagerCommon::GetEquipCrystalHitpoint(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalHitpointPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_HITPOINT,&CrystalHitpointPer);

	return CrystalHitpointPer;
}

// 장비의 마법력 향상을 구한다. 결정체. 
SI32 cltItemManagerCommon::GetEquipCrystalManapoint(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalManapointPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_MANAPOINT,&CrystalManapointPer);

	return CrystalManapointPer;
}

// 장비의 무게량 향상을 구한다. 결정체.
SI32 cltItemManagerCommon::GetEquipCrystalWeigt(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalWeigtPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_EFFECT_WEIGHT,&CrystalWeigtPer);

	return CrystalWeigtPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalFireAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_FIREATTACK,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalWaterAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_WATERATTACK,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalWindAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_WINDATTACK,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalEarthAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_EARTHATTACK,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalFireDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalWaterDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalWindDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE,&CrystalPer);

	return CrystalPer;
}

SI32 cltItemManagerCommon::GetEquipCrystalEarthDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar)
{
	SI32 siRatePer = 0;
	SI32 CrystalPer = 0;

	siRatePer = GetCountEquipPersonitemAbility(pclitemlist,activeweapon,pclchar , CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE,&CrystalPer);

	return CrystalPer;
}
// 결정체 타입을 빌려다 쓴다. - abilityType
// 기본적인 검사및 모든 것을 여기서 처리한다.
//KHY - 20090526 - 아바타 결정체 추가.
SI32 cltItemManagerCommon::GetCountEquipPersonitemAbility(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar  ,UI08 abilityType , SI32 *ReturnValPer)
{
	SI32	returnval = 0;
	SI32 temReturnVal = 0;
	
	SI32 siAppendCrystalPoint		= 0;	// 중첩 가능한 결정체 포인트
	SI32 siCantAppendCrystalPoion	= 0;	// 중첩 불가능한 결정체 포인트
											

	SI32 oldTemReturnVal = 0; // 능력중첩 불가의 결정체 경우, 그중 가장 높은 값만을 취한다.
	SI32 oldReturnValPer= 0;  // 능력중첩 불가의 결정체 경우, 그중 가장 높은 값만을 취한다.


	// 능력치의 누적이 필요한 경우 - temReturnVal += , ReturnValPer  값을 직접 넘기고 누적시킴

	// 능력중첩이 안되고 오직 그중 큰값하나 쓸경우 - oldTemReturnVal = , &oldReturnValPer 등으로 값을 비교하여 큰 값만을 저장, 리턴


	for(int pitempos=0; pitempos <PERSONITEM_INV0 ; pitempos++)
	{
		// [강형] 새로운 내구도 시스템
		bool bCanUseWithDurability = true;
		if( pclClient->IsCountrySwitch( Switch_NewDurability) )
		{
			SI16 siDurability = pclitemlist[pitempos].GetDurability();
			if( siDurability <= 0 )
				bCanUseWithDurability = false;
		}

		switch(pitempos)
		{
			// 검사 아이템의 추가, 및 삭제는 이부분에서  - 
			case PERSONITEM_HELMET:
			case PERSONITEM_ARMOUR:
			case PERSONITEM_BELT:
			case PERSONITEM_SHOES:
				{
					// 아바타 결정체가 들어 올수 없음으로 검사할 필요도 없다.
					if(IsAbatarCrystalItem(abilityType) == true)
						continue;
					
					if(pclitemlist[pitempos].siUnique > 0) // 1 -그 아이템을 장착 하고 있어야 한다.
					{
						// [강형] 내구도가 남아 있어야 한다.
						if(IsExpiredDate(&pclitemlist[pitempos]) == true && bCanUseWithDurability ) // 2 - 사용기간이 남아 있어야 한다. - 혹은 없는 아이템이거나.
						{
							if(CheckCondition(pclitemlist[pitempos].siUnique, pclchar, &returnval) == true ) // 3 - 장착 조건을 만족해야만 한다.
							{
								switch(abilityType) // 크리스탈의 능력 인덱스를 빌려 썼다. - 타입에 따라 함수가 다르다. 
								{
									// ///////////////////////////////////////////////////
									//   oldTemReturnVal =  : 각 아이템 중에서 가장 큰 능력 수치 하나만 선택. =  능력 중첩 : 불가.
									//   temReturnVal +=     : 각 아이템 의 능력 수치를 모두 누적하야 더한다.	 =  능력 중첩 : 가능.						
									//////////// - 스텍 - ////////////////////////////////// - 모두 중첩 가능.
									case CRYSTALABILITY_STATE_STR:
										 temReturnVal += pclitemlist[pitempos].GetEquipStr(this, &oldReturnValPer);
										break;
									case CRYSTALABILITY_STATE_DEX:
										 temReturnVal += pclitemlist[pitempos].GetEquipDex(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_STATE_MAG:
										 temReturnVal += pclitemlist[pitempos].GetEquipMag(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_STATE_VIT:
										 temReturnVal += pclitemlist[pitempos].GetEquipVit(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_STATE_LUK:
										 temReturnVal += pclitemlist[pitempos].GetEquipLuk(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_STATE_HND:
										 temReturnVal += pclitemlist[pitempos].GetEquipHnd(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_STATE_WIS:
										 temReturnVal += pclitemlist[pitempos].GetEquipWis(this,&oldReturnValPer); 
										break;
																		
									//////////// -효과  - //////////////////////////////////
									case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:			// 물리 최소 공격력.
										oldTemReturnVal = pclitemlist[pitempos].GetMinDamage(this , &oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:		// 물리 최대 공격력.
										oldTemReturnVal =  pclitemlist[pitempos].GetMaxDamage(this, &oldReturnValPer);
										break;										
									case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:			 // 마법 최소 공격력
										oldTemReturnVal =  pclitemlist[pitempos].GetAddMagicalMinDamage(this, &oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:			 // 마법 최대 공격력
										oldTemReturnVal =  pclitemlist[pitempos].GetAddMagicalMaxDamage(this, &oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE:			// 물리 명중률
										oldTemReturnVal =  pclitemlist[pitempos].GetPhysicalHitRate(this, &oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_MAGICAL_HITRATE:				// 마법 명중률
										oldTemReturnVal =  pclitemlist[pitempos].GetMagicalHitRate(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE:			// 물리 회피율
										oldTemReturnVal = pclitemlist[pitempos].GetPhysicalDodgeRate(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE:			// 마법 회피율
										oldTemReturnVal = pclitemlist[pitempos].GetMagicalDodgeRate(this,&oldReturnValPer);
										break;										
									case CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE:		// 물리 방어율 - 중첩
										temReturnVal += pclitemlist[pitempos].GetPhysicalAC(this ,&oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE:		// 마법 방어율
										oldTemReturnVal =  pclitemlist[pitempos].GetMagicalAC(this ,&oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_HITPOINT:						// 체력량
										oldTemReturnVal = pclitemlist[pitempos].GetCrystalHitpoint(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_MANAPOINT:					// 마나량
										oldTemReturnVal =  pclitemlist[pitempos].GetCrystalManapoint(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_EFFECT_WEIGHT:						// 무게량
										oldTemReturnVal = pclitemlist[pitempos].GetCrystalWeigt(this,&oldReturnValPer);
										break;
								///////////////////- 속성 - //////////////////////////////////////////////////////////////	
									case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:				// 불 속성 공격력
										oldTemReturnVal = pclitemlist[pitempos].GetCrystalFireAttack(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:				// 물 속성 공격력
										oldTemReturnVal = pclitemlist[pitempos].GetCrystalWaterAttack(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:				// 바람 속성 공격력
										oldTemReturnVal = pclitemlist[pitempos].GetCrystalWindAttack(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:				// 땅 속성 공격력
										oldTemReturnVal = pclitemlist[pitempos].GetCrystalEarthAttack(this,&oldReturnValPer);
										break;

									case CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE:				 // 불 속성 방어력
										oldTemReturnVal =  pclitemlist[pitempos].GetCrystalFireDeffence(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE:			 // 물 속성 방어력
										oldTemReturnVal =  pclitemlist[pitempos].GetCrystalWaterDeffence(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE:				// 바람 속성 방어력
										oldTemReturnVal =  pclitemlist[pitempos].GetCrystalWindDeffence(this,&oldReturnValPer);
										break;
									case CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE:			// 땅 속성 방어력
										oldTemReturnVal =  pclitemlist[pitempos].GetCrystalEarthDeffence(this,&oldReturnValPer);
										break;
								///////////////////- 특수 능력 - //////////////////////////////////////////////////////////////	
									case CRYSTALABILITY_SPECIAL_DEATHBLOW:					// 필살. - 크리티컬 률 상승.
										oldTemReturnVal =  pclitemlist[pitempos].GetCriticalHitRate(this,&oldReturnValPer);
										break;

									default :
										break;
								}							
							}
						}
					}
					break;
				}
			case PERSONITEM_NECK:
			case PERSONITEM_RING1:	
			case PERSONITEM_RING2:
			case PERSONITEM_MANTLE:
			case PERSONITEM_HAT:
			case PERSONITEM_DRESS:
				{
					if(pclitemlist[pitempos].siUnique > 0) // 1 -그 아이템을 장착 하고 있어야 한다.
					{
						// [강형] 내구도가 남아 있어야 한다.
						if(IsExpiredDate(&pclitemlist[pitempos]) == true && bCanUseWithDurability ) // 2 - 사용기간이 남아 있어야 한다. - 혹은 없는 아이템이거나.
						{
							if(CheckCondition(pclitemlist[pitempos].siUnique, pclchar, &returnval) == true ) // 3 - 장착 조건을 만족해야만 한다.
							{
								switch(abilityType) // 크리스탈의 능력 인덱스를 빌려 썼다. - 타입에 따라 함수가 다르다. 
								{
									// ///////////////////////////////////////////////////
									//   oldTemReturnVal =  : 각 아이템 중에서 가장 큰 능력 수치 하나만 선택. =  능력 중첩 : 불가.
									//   temReturnVal +=     : 각 아이템 의 능력 수치를 모두 누적하야 더한다.	 =  능력 중첩 : 가능.						
									//////////// - 스텍 - ////////////////////////////////// - 모두 중첩 가능.
						
									// 옜날 능력도 사용 할 수 있어야 한다.
									case CRYSTALABILITY_STATE_STR:
										temReturnVal += pclitemlist[pitempos].GetEquipStr(this, &siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_STATE_DEX:
										temReturnVal += pclitemlist[pitempos].GetEquipDex(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_STATE_MAG:
										temReturnVal += pclitemlist[pitempos].GetEquipMag(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_STATE_VIT:
										temReturnVal += pclitemlist[pitempos].GetEquipVit(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_STATE_LUK:
										temReturnVal += pclitemlist[pitempos].GetEquipLuk(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_STATE_HND:
										temReturnVal += pclitemlist[pitempos].GetEquipHnd(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_STATE_WIS:
										temReturnVal += pclitemlist[pitempos].GetEquipWis(this,&siAppendCrystalPoint); 
										break;

									// [진성] 아바타 결정체. 능력치 추가.
									case CRYSTALABILITY_AVATAR_STATE_STR:
										temReturnVal += pclitemlist[pitempos].GetEquipStr(this, &siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_AVATAR_STATE_DEX:
										temReturnVal += pclitemlist[pitempos].GetEquipDex(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_AVATAR_STATE_MAG:
										temReturnVal += pclitemlist[pitempos].GetEquipMag(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_AVATAR_STATE_VIT:
										temReturnVal += pclitemlist[pitempos].GetEquipVit(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_AVATAR_STATE_LUK:
										temReturnVal += pclitemlist[pitempos].GetEquipLuk(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_AVATAR_STATE_HND:
										temReturnVal += pclitemlist[pitempos].GetEquipHnd(this,&siAppendCrystalPoint);
										break;
									case CRYSTALABILITY_AVATAR_STATE_WIS:
										temReturnVal += pclitemlist[pitempos].GetEquipWis(this,&siAppendCrystalPoint); 
										break;									
									//////////// -효과  - //////////////////////////////////////////////////////
									case CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE:		// 물리 방어율 - 중첩
										temReturnVal += pclitemlist[pitempos].GetPhysicalAC(this ,&siAppendCrystalPoint);
										break;

									////////////- 속성 - ///////////////////////////////////////////////////////	
									default :
										break;
								}							
							}
						}
					}
				}
				break;
			default :
				break;
		}
		
		//// 아바타 아이템 이라면.
		//if(pclitemlist[pitempos].GetAavatarType(this) > 0)
		//{
		//	temReturnVal += oldTemReturnVal;
		//	oldTemReturnVal = 0;

		//	*ReturnValPer += oldReturnValPer;
		//	oldReturnValPer = 0;				
		//}
		//else 
		//{
		//	//아바타 아이템이 아니라면.
		//	if(oldTemReturnVal > temReturnVal)
		//	{
		//		temReturnVal = oldTemReturnVal;
		//		oldTemReturnVal = 0;
		//	}

		//	if(oldReturnValPer > *ReturnValPer)
		//	{
		//		*ReturnValPer = oldReturnValPer;
		//		oldReturnValPer = 0;
		//	}
		//}

		


		if(oldTemReturnVal > temReturnVal)
		{
			temReturnVal = oldTemReturnVal;
			oldTemReturnVal = 0;
		}

		if(oldReturnValPer > siCantAppendCrystalPoion)
		{
			siCantAppendCrystalPoion = oldReturnValPer;
			oldReturnValPer = 0;
		}
		
		*ReturnValPer += siAppendCrystalPoint;		
		siAppendCrystalPoint = 0;

	}

	// 누적시키지 않기위한 초기화
	oldTemReturnVal = 0;
	oldReturnValPer = 0;

	siAppendCrystalPoint = 0;

	// [강형] 새로운 내구도 시스템
	bool bCanUseWithDurability = true;
	if( pclClient->IsCountrySwitch( Switch_NewDurability) )
	{
		SI16 siDurability = pclitemlist[activeweapon].GetDurability();
		if( siDurability <= 0 )
			bCanUseWithDurability = false;
	}

	//  - activeweapon은 어쩔수 없다. - 여기서 따로 검사.
	if(       IsExpiredDate(&pclitemlist[activeweapon]) == true  &&						  // 사용기간이 남아있어야 한다. - by LEEKH 2007.04.12
		bCanUseWithDurability															// [강형] 새로운 내구도 시스템
		&&CheckCondition(pclitemlist[activeweapon].siUnique, pclchar, &returnval) == true //정상적인 착용아이템이라면 [2007.08.07 손성웅]
		&&(pclitemlist[activeweapon].siUnique > 0))									  // activeweapon를 장착 하고 있어야 한다. 								  
	{
		switch(abilityType) // 크리스탈의 능력 인덱스를 빌려 썼다. - 타입에 따라 함수가 다르다. 
		{
			// ///////////////////////////////////////////////////
			//   oldTemReturnVal =  : 각 아이템 중에서 가장 큰 능력 수치 하나만 선택. =  능력 중첩 : 불가.
			//   temReturnVal +=     : 각 아이템 의 능력 수치를 모두 누정하야 더한다.	 =  능력 중첩 : 가능.						
			//////////// - 스텍 - ////////////////////////////////// - 모두 중첩 가능.
			case CRYSTALABILITY_STATE_STR:
				 temReturnVal += pclitemlist[activeweapon].GetEquipStr(this,&oldReturnValPer); 
				break;
			case CRYSTALABILITY_STATE_DEX:
				 temReturnVal +=pclitemlist[activeweapon].GetEquipDex(this,&oldReturnValPer); 
				break;
			case CRYSTALABILITY_STATE_MAG:
				 temReturnVal +=pclitemlist[activeweapon].GetEquipMag(this,&oldReturnValPer); 
				break;
			case CRYSTALABILITY_STATE_VIT:
				 temReturnVal += pclitemlist[activeweapon].GetEquipVit(this,&oldReturnValPer); 
				break;
			case CRYSTALABILITY_STATE_LUK:
				 temReturnVal += pclitemlist[activeweapon].GetEquipLuk(this,&oldReturnValPer); 
				break;
			case CRYSTALABILITY_STATE_HND:
				 temReturnVal += pclitemlist[activeweapon].GetEquipHnd(this,&oldReturnValPer); 
				break;
			case CRYSTALABILITY_STATE_WIS:
				 temReturnVal += pclitemlist[activeweapon].GetEquipWis(this,&oldReturnValPer); 
				break;
			//////////////////////////////////////////////////////////////////////////
			// [진성] 아바타 결정체. 능력치 추가.
			case CRYSTALABILITY_AVATAR_STATE_STR:
				temReturnVal += pclitemlist[activeweapon].GetEquipStr(this, &siAppendCrystalPoint);
				break;
			case CRYSTALABILITY_AVATAR_STATE_DEX:
				temReturnVal += pclitemlist[activeweapon].GetEquipDex(this,&siAppendCrystalPoint);
				break;
			case CRYSTALABILITY_AVATAR_STATE_MAG:
				temReturnVal += pclitemlist[activeweapon].GetEquipMag(this,&siAppendCrystalPoint);
				break;
			case CRYSTALABILITY_AVATAR_STATE_VIT:
				temReturnVal += pclitemlist[activeweapon].GetEquipVit(this,&siAppendCrystalPoint);
				break;
			case CRYSTALABILITY_AVATAR_STATE_LUK:
				temReturnVal += pclitemlist[activeweapon].GetEquipLuk(this,&siAppendCrystalPoint);
				break;
			case CRYSTALABILITY_AVATAR_STATE_HND:
				temReturnVal += pclitemlist[activeweapon].GetEquipHnd(this,&siAppendCrystalPoint);
				break;
			case CRYSTALABILITY_AVATAR_STATE_WIS:
				temReturnVal += pclitemlist[activeweapon].GetEquipWis(this,&siAppendCrystalPoint); 
				break;
			//////////////////////////////////////////////////////////////////////////
			
			//////////// -효과  - //////////////////////////////////
			case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:			// 물리 최소 공격력.
				oldTemReturnVal = pclitemlist[activeweapon].GetMinDamage(this , &oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:		// 물리 최대 공격력.
				oldTemReturnVal =  pclitemlist[activeweapon].GetMaxDamage(this, &oldReturnValPer);
				break;										
			//case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:				 // 마법 공격력
			//	oldTemReturnVal = pclitemlist[activeweapon].GetAddMagicalDamage(this, &oldReturnValPer);
			//	break;
			case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:			 // 마법 최소 공격력
				oldTemReturnVal =  pclitemlist[activeweapon].GetAddMagicalMinDamage(this, &oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:			 // 마법 최대 공격력
				oldTemReturnVal =  pclitemlist[activeweapon].GetAddMagicalMaxDamage(this, &oldReturnValPer);
				break;

			case CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE:			// 물리 명중률
				oldTemReturnVal =  pclitemlist[activeweapon].GetPhysicalHitRate(this, &oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_MAGICAL_HITRATE:				// 마법 명중률
				oldTemReturnVal = pclitemlist[activeweapon].GetMagicalHitRate(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE:			// 물리 회피율
				oldTemReturnVal =  pclitemlist[activeweapon].GetPhysicalDodgeRate(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE:			// 마법 회피율
				oldTemReturnVal = pclitemlist[activeweapon].GetMagicalDodgeRate(this,&oldReturnValPer);
				break;										
			case CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE:		// 물리 방어율 - 중첩
				temReturnVal += pclitemlist[activeweapon].GetPhysicalAC(this ,&oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE:		// 마법 방어율
				oldTemReturnVal = pclitemlist[activeweapon].GetMagicalAC(this ,&oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_HITPOINT:						// 체력량
				oldTemReturnVal =  pclitemlist[activeweapon].GetCrystalHitpoint(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_MANAPOINT:					// 마나량
				oldTemReturnVal = pclitemlist[activeweapon].GetCrystalManapoint(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_EFFECT_WEIGHT:						// 무게량
				oldTemReturnVal = pclitemlist[activeweapon].GetCrystalWeigt(this,&oldReturnValPer);
				break;
		///////////////////- 속성 - //////////////////////////////////////////////////////////////	
			case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:				// 불 속성 공격력
				oldTemReturnVal = pclitemlist[activeweapon].GetCrystalFireAttack(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:				// 물 속성 공격력
				oldTemReturnVal = pclitemlist[activeweapon].GetCrystalWaterAttack(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:				// 바람 속성 공격력
				oldTemReturnVal = pclitemlist[activeweapon].GetCrystalWindAttack(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:				// 땅 속성 공격력
				oldTemReturnVal = pclitemlist[activeweapon].GetCrystalEarthAttack(this,&oldReturnValPer);
				break;

			case CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE:				 // 불 속성 방어력
				oldTemReturnVal =  pclitemlist[activeweapon].GetCrystalFireDeffence(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE:			 // 물 속성 방어력
				oldTemReturnVal =  pclitemlist[activeweapon].GetCrystalWaterDeffence(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE:				// 바람 속성 방어력
				oldTemReturnVal =  pclitemlist[activeweapon].GetCrystalWindDeffence(this,&oldReturnValPer);
				break;
			case CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE:			// 땅 속성 방어력
				oldTemReturnVal =  pclitemlist[activeweapon].GetCrystalEarthDeffence(this,&oldReturnValPer);
				break;
		///////////////////- 특수 능력 - //////////////////////////////////////////////////////////////	
			case CRYSTALABILITY_SPECIAL_DEATHBLOW:					// 필살. - 크리티컬 률 상승.
				oldTemReturnVal =  pclitemlist[activeweapon].GetCriticalHitRate(this,&oldReturnValPer);
				break;
			default :
				break;
		}				

	}


	//// 아바타 아이템 이라면.
	//if(pclitemlist[activeweapon].GetAavatarType(this) > 0)
	//{
	//	temReturnVal += oldTemReturnVal;
	//	oldTemReturnVal = 0;

	//	*ReturnValPer += oldReturnValPer;
	//	oldReturnValPer = 0;				
	//}
	//else 
	//{
	//	if(oldTemReturnVal > temReturnVal)
	//	{
	//		temReturnVal = oldTemReturnVal;
	//		oldTemReturnVal = 0;
	//	}

	//	if(oldReturnValPer > *ReturnValPer)
	//	{
	//		*ReturnValPer = oldReturnValPer;
	//		oldReturnValPer = 0;
	//	}
	//}


	if(oldTemReturnVal > temReturnVal)
	{
		temReturnVal = oldTemReturnVal;
		oldTemReturnVal = 0;
	}

	if(oldReturnValPer > siCantAppendCrystalPoion)
	{
		siCantAppendCrystalPoion = oldReturnValPer;
		oldReturnValPer = 0;
	}

	*ReturnValPer += siAppendCrystalPoint;		
	*ReturnValPer += siCantAppendCrystalPoion;	

	siAppendCrystalPoint = 0;

	return temReturnVal;

}


// 장비의 데미지 감소를 구한다. 
SI32 cltItemManagerCommon::GetDamageReduceRate(cltItem* pclitemlist)
{
	SI32 i;
	SI32 item = 0;
	// 소환수 부착 부적에 의한 영향력 추가 
	SI32 siTotalRate = 0;

	for(i = 0;i < 6;i++)
	{
		if(i == 0)
		{
			item = PERSONITEM_HELMET;
		}
		else if(i == 1)
		{
			item = PERSONITEM_HAT;
		}
		else if(i == 2)
		{
			item = PERSONITEM_ARMOUR;
		}
		else if(i == 3)
		{
			item = PERSONITEM_DRESS;
		}
		else if(i == 4)
		{
			item = PERSONITEM_BELT;
		}
		else
		{
			item = PERSONITEM_SHOES;
		}

		cltItem* pclitem = (cltItem*)&pclitemlist[item];

		// 빈 아이템은 무시한다. 
		if(pclitem->siUnique == 0)	continue;
		
		SI64 atb = GetItemAtb(pclitem->siUnique);
		if((atb & ITEMATB_ARMOUR) == 0)	return siTotalRate;
		
		if( pclitem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_REDUCEDAMAGE)
		{
			siTotalRate += pclitem->clItemCommon.clOptionInfo.uiPara1;
		}
		if( pclitem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_REDUCEDAMAGE)
		{
			siTotalRate += pclitem->clItemCommon.clOptionInfo.uiPara2;
		}
	}
	
	return siTotalRate;
}

// 신발의 이동속도를 구한다. 
SI32 cltItemManagerCommon::GetMoveSpeed(cltItem* pclitemlist)
{
	SI32 movespeed = pclitemlist[PERSONITEM_SHOES].GetMoveSpeed(this);
	return movespeed;
}
// 신발의 이동속도를 구한다. 
SI32 cltItemManagerCommon::GetMoveSpeed(cltItem* pclitemlist, cltCharCommon* pclchar)
{	
	SI32 returnval = 0;
	SI32 movespeed = pclitemlist[PERSONITEM_SHOES].GetMoveSpeed(this);

	if (CheckCondition(pclitemlist[PERSONITEM_SHOES].siUnique, pclchar, &returnval) == false)
	{
		movespeed = 0;
		return movespeed;
	}
	else
	{
		return movespeed;
	}
	
}

// 아이템 이미지 정보를 얻어온다.
bool cltItemManagerCommon::GetItemGImageFileIndexAndFont( SI32 ItemUnique, SI32 *pGlobalImageIndex, SI32 *pGlobalImageFont )
{
	if ( ItemUnique < 1 || pGlobalImageIndex == NULL || pGlobalImageFont == NULL )
	{
		return false;
	}

	SI32 Data[]=
	{
			GIMG_ITEMIMG01, 200,
			GIMG_ITEMIMG02, 191,
			GIMG_ITEMIMG03, 210,
			GIMG_ITEMIMG04, 210,
			GIMG_ITEMIMG05, 200,
			GIMG_ITEMIMG06, 200,
			GIMG_ITEMIMG07, 200,
			GIMG_ITEMIMG08, 200,
			-1
	};

	//SI32 Data[]=
	//{
	//	GIMG_ITEMIMG01, 40,
	//		GIMG_ITEMIMG02, 40,
	//		GIMG_ITEMIMG03, 40,
	//		GIMG_ITEMIMG04, 40,
	//		GIMG_ITEMIMG05, 40,
	//		GIMG_ITEMIMG06, 40,
	//		GIMG_ITEMIMG07, 40,
	//		GIMG_ITEMIMG08, 40,
	//		GIMG_ITEMIMG09, 40,
	//		GIMG_ITEMIMG10, 31,
	//		GIMG_ITEMIMG11, 40,
	//		GIMG_ITEMIMG12, 40,
	//		GIMG_ITEMIMG13, 40,
	//		GIMG_ITEMIMG14, 40,
	//		GIMG_ITEMIMG15, 40,
	//		GIMG_ITEMIMG16, 40,
	//		GIMG_ITEMIMG17, 40,
	//		GIMG_ITEMIMG18, 40,
	//		GIMG_ITEMIMG19, 40,
	//		GIMG_ITEMIMG20, 40,
	//		-1
	//};


	cltItemInfo* pcliteminfo = FindItemInfo( ItemUnique );

	if ( pcliteminfo == NULL )
	{
		return false;
	}

	SI16 Counter = 0;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageIndexFont = 0;
	SI32 AccumulationImageCounter = 0;

	bool loop = true;
	while( loop )
	{
		if ( Data[ Counter * 2 ] != -1 )
		{
			if ( AccumulationImageCounter + Data[ Counter * 2 + 1 ] - 1 >=  pcliteminfo->siImageIndex )
			{
				GlobalImageIndex = Data[ Counter * 2 ];
				GlobalImageIndexFont = pcliteminfo->siImageIndex - AccumulationImageCounter;
				
				*pGlobalImageIndex = GlobalImageIndex;
				*pGlobalImageFont = GlobalImageIndexFont;

				return true;
			}
			else
			{
				AccumulationImageCounter += Data[ Counter * 2 + 1 ];
				++Counter;
			}
		}
		else
		{
			break;
		}
	}

	return false;
}

#define SETITEM_ARRAY1_NUM		8
SI32 SetItemArray1[ SETITEM_ARRAY1_NUM ] = 
{
	PERSONITEM_HELMET,
	PERSONITEM_ARMOUR,
	PERSONITEM_NECK,
	PERSONITEM_BELT,
	PERSONITEM_SHOES,
	PERSONITEM_MANTLE,
	PERSONITEM_RING1,
	PERSONITEM_RING2
};

SI32 cltItemManagerCommon::GetCompletedSetItemIndex( cltItem* pclItemList )
{
	if ( pclItemList == NULL )
	{
		return 0;
	}

	if ( pclItemList[ SetItemArray1[ 0 ] ].siUnique > 0 &&
		pclItemList[ SetItemArray1[ 1 ] ].siUnique > 0 &&
		pclItemList[ SetItemArray1[ 2 ] ].siUnique > 0 &&
		pclItemList[ SetItemArray1[ 3 ] ].siUnique > 0 &&
		pclItemList[ SetItemArray1[ 4 ] ].siUnique > 0 &&
		pclItemList[ SetItemArray1[ 5 ] ].siUnique > 0 &&
		( pclItemList[ SetItemArray1[ 6 ] ].siUnique > 0 || pclItemList[ SetItemArray1[ 7 ] ].siUnique > 0 )
		)
	{
		SI32 Ref = 0;
		bool NotEnoughCondition = false;

		// SetItemArray1 Check
		for ( SI16 i = 0; i < SETITEM_ARRAY1_NUM - 2; ++i )
		{
			if ( pclItemList[ SetItemArray1[ i ] ].siUnique > 0 )
			{
				Ref = FindItemRefFromUnique( pclItemList[ SetItemArray1[ i ] ].siUnique );
				if ( Ref > 0 )
				{
					if ( pclItemInfo[ Ref ]->siSetItemIndex != 1 )
					{
						NotEnoughCondition = true;
					}
				}
				else
				{
					NotEnoughCondition = true;
				}
			}
			else
			{
				NotEnoughCondition = true;
			}
		}

		if ( ! NotEnoughCondition )
		{
			Ref = FindItemRefFromUnique( pclItemList[ SetItemArray1[ 6 ] ].siUnique );
			if ( Ref > 0 )
			{
				if ( pclItemInfo[ Ref ]->siSetItemIndex == 1 )
				{
					return 1;
				}
			}

			Ref = FindItemRefFromUnique( pclItemList[ SetItemArray1[ 7 ] ].siUnique );
			if ( Ref > 0 )
			{
				if ( pclItemInfo[ Ref ]->siSetItemIndex == 1 )
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
}
bool cltItemManagerCommon::CheckCanPutonItem(SI32 unique )
{
	SI32 sitype = GetItemType( unique );

	switch(sitype)
	{
	case  ITEMTYPE_SWORD:
	case  ITEMTYPE_SPEAR:
	case  ITEMTYPE_STAFF:
	case  ITEMTYPE_AXE:
	case  ITEMTYPE_BOW:
	case  ITEMTYPE_GUN:
	case  ITEMTYPE_CANNON:
	case  ITEMTYPE_ARMOUR:
	case  ITEMTYPE_BELT:
	case  ITEMTYPE_SHOES:
	case  ITEMTYPE_HELMET:
	//case  ITEMTYPE_DRESS:
	//case  ITEMTYPE_HAT:
	//case  ITEMTYPE_RING:
		return true;
	default:
		return false;
	}				
}

//KHY - 0115 발렌타인데이 이벤트.
bool cltItemManagerCommon::GetValentineEvent(SI32 siUnique)
{
	switch(siUnique)
	{
		case  ITEMUNIQUE(13006): //V카드
		case  ITEMUNIQUE(13007): //A카드.
		case  ITEMUNIQUE(13008): //L 카드.
		case  ITEMUNIQUE(13009): //E1카드
		case  ITEMUNIQUE(13010): //N1카드
		case  ITEMUNIQUE(13011): //T카드
		case  ITEMUNIQUE(13012): //I 카드
		case  ITEMUNIQUE(13014): //E2카드 
		case  ITEMUNIQUE(13013): //N2카드
			return true;
		default:
			return false;
	}				
}

//KHY - 0218 화이트데이 이벤트.
bool cltItemManagerCommon::GetWhiteEvent(SI32 siUnique)
{
	switch(siUnique)
	{
		case  ITEMUNIQUE(13016): //W카드
		case  ITEMUNIQUE(13017): //H카드.
		case  ITEMUNIQUE(13018): //I 카드.
		case  ITEMUNIQUE(13019): //T카드
		case  ITEMUNIQUE(13020): //E카드
			return true;
		default:
			return false;
	}				
}
bool cltItemManagerCommon::IsExpiredDate(cltItem* pclitem)
{
	SI32		useableDay	= 0;
	
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		cltServer*	pclserver = (cltServer*)pclClient;		
		if(pclserver == NULL)			return false;
		
		BOOL bRet = pclserver->GetItemUseAbleDay(pclitem, &useableDay);
		if(bRet == TRUE && useableDay >= 0)				return true;
		//if(bRet == TRUE && useableDay < 0)				return true;
	}
	else if (pclClient->GameMode == GAMEMODE_CLIENT)
	{
		cltClient*	pclclient = (cltClient*)pclClient;
		if(pclclient == NULL)			return false;

		BOOL bRet = pclclient->GetItemUseAbleDay(pclitem, &useableDay);
		if(bRet == TRUE && useableDay >= 0)				return true;
		//if(bRet == TRUE && useableDay < 0)				return true;
	}

	return false;
}

//bool cltItemManagerCommon::UsedSlotItem(SI32 siUnique)
//{
//	if( siUnique == 0 )
//		return 0;
//
//	bool bResult = false;
//
//	SI32 itemtype	 = GetItemType(siUnique);
//
//	switch(itemtype)
//	{
//		case ITEMTYPE_SWORD:				// 검
//		case ITEMTYPE_SPEAR:				// 창
//		case ITEMTYPE_STAFF:				// 지팡이
//		case ITEMTYPE_AXE:					// 도끼
//		case ITEMTYPE_BOW:					// 활
//		case ITEMTYPE_GUN:					// 총
//		case ITEMTYPE_ARMOUR:				// 갑옷
//		case ITEMTYPE_BELT:					// 허리띠
//		case ITEMTYPE_SHOES:				// 신발
//		case ITEMTYPE_HELMET:				// 투구
//
//		// [진성] 아바타 결정체. 아바타 아이템의 결정체 조합 여부 추가.
//		case ITEMTYPE_NECK:					// 목걸이
//		case ITEMTYPE_RING:					// 반지
//		case ITEMTYPE_HAT:					// 모자
//		case ITEMTYPE_DRESS:				// 의복
//		case ITEMTYPE_MANTLE:				// 망토
//		//case ITEMTYPE_CANNON:				// 대포
//			{
//				bResult = true; 
//			}
//			break;			
//	}
//
//	// 번외로 타입은 아닌데 유니크로 적용되는 아이템이 있을지도 몰라서 만듬.
//	if( false == bResult )
//	{
//		const SI32 UsedSlotItem[]    = { 1,  };
//			  SI32 UsedSlotItemCount = sizeof(UsedSlotItem) / sizeof(SI32);
//
//		for( SI32 Count = 0; Count < UsedSlotItemCount; ++Count )
//		{
//			if(UsedSlotItem[Count] == siUnique)
//			{
//				bResult = true;
//			}
//		}
//	}
//	
//	return bResult;
//}


//KHY - 20090526 - 아바타 결정체.
bool cltItemManagerCommon::IsCrystalAbatarItem( SI32 siItemType )
{
	bool bResult = false;
	
	switch(siItemType)
	{
		case ITEMTYPE_AVATAR_MANTLE:	// 아바타 망토
		case ITEMTYPE_AVATAR_DRESS:		// 아바타 의복
		case ITEMTYPE_AVATAR_HAT:		// 아바타 모자
		case ITEMTYPE_AVATAR_NECK:		// 아바타 목걸이
		case ITEMTYPE_AVATAR_RING:		// 아바타 반지
			{
				bResult = true;
			}
			break;
		default:
				bResult = false;
			break;
	}
	
	return bResult;
}

bool cltItemManagerCommon::IsAbatarCrystalItem( UI08 uiCrystalAbility )
{
	bool bResult = false;
	
	switch(uiCrystalAbility)
	{
		case CRYSTALABILITY_AVATAR_STATE_STR:
		case CRYSTALABILITY_AVATAR_STATE_DEX:
		case CRYSTALABILITY_AVATAR_STATE_MAG:	
		case CRYSTALABILITY_AVATAR_STATE_VIT:
		case CRYSTALABILITY_AVATAR_STATE_LUK:
		case CRYSTALABILITY_AVATAR_STATE_HND:
		case CRYSTALABILITY_AVATAR_STATE_WIS:	
		case CRYSTALABILITY_AVATAR_ALLMAKESKILL:
		case CRYSTALABILITY_AVATAR_ALLWEAPONSKILL:
		case CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL:	
			{
				bResult = true;
			}
			break;
		default:
				bResult = false;
			break;
	}
	
	return bResult;
}

//KHY - 20090526 - 아바타 결정체.
SI32 cltItemManagerCommon::GetMaxAbatarCrystalAbilityNum(SI32 siItemType , UI08 uiCrystalAbility)
{
	SI32 maxAvatarCrystalAbility = 0;
	// 아바타 결정체  - 제약 조건들.
	switch(siItemType) // 경정체가 박히는 아이템에 따라.
	{
		case ITEMTYPE_AVATAR_DRESS:
		case ITEMTYPE_AVATAR_HAT:
			{
				// 스킬에 따라서 최대 값이 틀리다.
				switch(uiCrystalAbility)
				{
					case CRYSTALABILITY_AVATAR_ALLWEAPONSKILL:
					case CRYSTALABILITY_AVATAR_ALLMAKESKILL:
						maxAvatarCrystalAbility= 5;
						break;
					case CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL:
						maxAvatarCrystalAbility= 0;
						break;
					default: // 각종 스테이터스 결정체 - 만약 다른 스킬 경정체가 생기면 추가나 바꾸어줘야 한다.
						maxAvatarCrystalAbility= 10;
						break;									
				}
			}
			break;
		case ITEMTYPE_AVATAR_MANTLE:
		case ITEMTYPE_AVATAR_RING:
			{
				// 스킬에 따라서 최대 값이 틀리다.
				switch(uiCrystalAbility)
				{
					case CRYSTALABILITY_AVATAR_STATE_STR:
					case CRYSTALABILITY_AVATAR_STATE_DEX:
					case CRYSTALABILITY_AVATAR_STATE_MAG:
					case CRYSTALABILITY_AVATAR_STATE_VIT:
					case CRYSTALABILITY_AVATAR_STATE_LUK:
					case CRYSTALABILITY_AVATAR_STATE_HND:
					case CRYSTALABILITY_AVATAR_STATE_WIS:
						maxAvatarCrystalAbility= 10;
						break;									

					default:
						maxAvatarCrystalAbility= 0;
						break;									
				}
			}
			break;
		case ITEMTYPE_AVATAR_NECK:
			{
				// 스킬에 따라서 최대 값이 틀리다.
				switch(uiCrystalAbility)
				{
					case CRYSTALABILITY_AVATAR_ALLWEAPONSKILL:
					case CRYSTALABILITY_AVATAR_ALLMAKESKILL:
					case CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL:
						maxAvatarCrystalAbility= 3;
						break;
					default: 
						maxAvatarCrystalAbility= 0;
						break;									
				}
			}
			break;
	}

	return maxAvatarCrystalAbility;
}


SI32 cltItemManagerCommon::UsedSlotItem(SI32 siUnique)
{
	if( siUnique == 0 )	return SLOTITEMTYPE_NONE;

	SI32 bResult = SLOTITEMTYPE_NONE;

	// [기형] 휴면 장비 아이템인지 확인 
	if( NULL == pclClient)
	{
		return SLOTITEMTYPE_NONE;
	}

	CDormancySystem *pclDormancySystem = pclClient->pclDormancySystem;
	if( NULL != pclDormancySystem)
	{
		CDormancyEquipItemRewardClient *pclDormancyClient = (CDormancyEquipItemRewardClient *)pclDormancySystem->GetDormancyEquipItemReward();
		if( NULL != pclDormancyClient)
		{
			if( TRUE == pclDormancyClient->IsDormancyEquipItem(siUnique))
			{
				return SLOTITEMTYPE_DORMANCY;
			}
		}
	}

	SI32 itemtype	 = GetItemType(siUnique);

	switch(itemtype)
	{
	case ITEMTYPE_SWORD:				// 검
	case ITEMTYPE_SPEAR:				// 창
	case ITEMTYPE_STAFF:				// 지팡이
	case ITEMTYPE_AXE:					// 도끼
	case ITEMTYPE_BOW:					// 활
	case ITEMTYPE_GUN:					// 총
	case ITEMTYPE_ARMOUR:				// 갑옷
	case ITEMTYPE_BELT:					// 허리띠
	case ITEMTYPE_SHOES:				// 신발
	case ITEMTYPE_HELMET:				// 투구
		{
			bResult = SLOTITEMTYPE_WEAPON;
		}
		break;

	//case ITEMTYPE_NECK:				// 목걸이
	//case ITEMTYPE_RING:				// 반지
	//case ITEMTYPE_HAT:				// 모자
	//case ITEMTYPE_DRESS:				// 의복
	//case ITEMTYPE_MANTLE:				// 망토
	//case ITEMTYPE_CANNON:				// 대포

	case ITEMTYPE_AVATAR_MANTLE:	// 아바타 망토
	case ITEMTYPE_AVATAR_DRESS:		// 아바타 의복
	case ITEMTYPE_AVATAR_HAT:		// 아바타 모자
	case ITEMTYPE_AVATAR_NECK:		// 아바타 목걸이
	case ITEMTYPE_AVATAR_RING:		// 아바타 반지
		{
			bResult = SLOTITEMTYPE_AVATAR;
		}
		break;			
	}

	// 번외로 타입은 아닌데 유니크로 적용되는 아이템이 있을지도 몰라서 만듬.
	if( SLOTITEMTYPE_NONE == bResult )
	{
		const SI32 UsedSlotItem[]    = { 1,  };
		SI32 UsedSlotItemCount = sizeof(UsedSlotItem) / sizeof(SI32);

		for( SI32 Count = 0; Count < UsedSlotItemCount; ++Count )
		{
			if(UsedSlotItem[Count] == siUnique)
			{
				bResult = SLOTITEMTYPE_WEAPON;
			}
		}
	}

	return bResult;
}

// [추가 : 황진성 2008. 2. 29 => 결정체 결합이 가능한 데이타 인가?.]
BOOL cltItemManagerCommon::CanCrystalMixtureData( cltItem* _EquipItem, cltItem* _CrystalItem, SI16 _EquipItemPos)
{
	// 기본 검사.
	// ---------------------------------------------------------------------------------
	if( 0 >= _EquipItem->siUnique )
	{
		return NO_ITEM_UNIQUE;
	}
	
	// 슬롯이 있는 아이템인지를 검사한다.
	if( SLOTITEMTYPE_NONE == UsedSlotItem( _EquipItem->siUnique ) )
	{
		return NO_SLOT_ITEM;
	}

	// 아이템에 결합이 가능한 슬롯이 있는지 검사.
	//KHY - 0805 - 아트펙스 수정.
	//if( false == _EquipItem->EmptyCrystalSlot() )
	//{
	//	return NO_USED_SLOT;
	//}

	// 장착장에 위치한 아이템은 조합할 수 없다.
	if( _EquipItemPos < PERSONITEM_INV0 )
		return NO_SLOT_ITEM;


	bool EmptySlot = true;
	for( SI32 Count = 0; Count < MAX_CRYSTAL_NUM; ++Count )
	{
		if( 0 < _CrystalItem[Count].siUnique && 0 < _CrystalItem[Count].siItemNum )
		{
			EmptySlot = false;

			// 조합하려는 아이템이 결정체가 아니다.
			if( ITEMTYPE_CRYSTAL != _CrystalItem[Count].GetType( this ) )
			{
				return NO_CRYSTAL;
			}
			else
			{
				// 장비에 결정체를 조합하려는 슬롯에 이미 결정체가 있다. 
				//KHY - 0805 - 아트펙스 수정.
				//if(0 < _EquipItem->cltItemCrytal[Count].GetCrystalType())
				//{
				//	return ALREADY_USE_SLOT;
				//}
			}
		}
	}

	if( true == EmptySlot )
	{
		return ALL_EMPTY_CRYSTAL;
	}
	//------------------------------------------------------------------------------------
	
	//KHY - 20090526 - 아바타 결정체.

	// 결정체 쪽이 아바타 결정체 일 경우.
	for( SI32 CrystalCount1 = 0; CrystalCount1 < _EquipItem->siUseCrystalNum; ++CrystalCount1 )
	{		
		if( IsAbatarCrystalItem( _CrystalItem[CrystalCount1].GetCrystalAbility())) // 능력으로 결정한다.
		{
			if( IsCrystalAbatarItem( _EquipItem->GetType( this )) == false) // 아바타 아이템이 아니라면.
				return NO_CRYSTAL; // 일단은  - 조합하려는 아이템과 결정체가 맞지 않다.

		}
	}
	
	// EquipItem 쪽이 아바타 일 경우. 
	if( IsCrystalAbatarItem( _EquipItem->GetType( this ))) // 아바타 타입의 아이템 일 경우.
	{
		for( SI32 EquipCount = 0; EquipCount < _EquipItem->siUseCrystalNum; ++EquipCount )
		{
			for( SI32 CrystalCount = 0; CrystalCount < _EquipItem->siUseCrystalNum; ++CrystalCount )
			{		
				if( 0 < _EquipItem->cltItemCrytal[ EquipCount ].GetCrystalType() &&
					0 < _CrystalItem[CrystalCount].siUnique )
				{
					if( _EquipItem->cltItemCrytal[ EquipCount ].GetCrystalAblilty() == _CrystalItem[CrystalCount].GetCrystalAbility()) 
					{
						if(EquipCount == CrystalCount)
							continue; // 모든 아바타 결정체는 누적이 가능하다.
						else
							return SAME_CRYSTAL; // 다른 위치에 같은 능력이 있다면 불가하다.
					}
				}
			}
		}
		
		//한꺼번에 중복조합되는 것을 막기위해서, 각 아바타 결정체들의 타입을 교차 검사해야 한다.
		for(SI16 aCrystalCount1 = 0; aCrystalCount1 < _EquipItem->siUseCrystalNum ; ++aCrystalCount1)
		{
			// 해당 아이템이 있을경우에만, 
			if(0 < _CrystalItem[aCrystalCount1].siUnique )
			{
				for(SI16 aCrystalCount2 = 0; aCrystalCount2 < _EquipItem->siUseCrystalNum ; ++aCrystalCount2)
				{
					// 해당 아이템이 있고, 같은 슬롯이 아니어야 한다. 
					if((0 < _CrystalItem[aCrystalCount2].siUnique) &&(aCrystalCount1 != aCrystalCount2))
					{
						if( _CrystalItem[aCrystalCount1].GetCrystalAbility() == _CrystalItem[aCrystalCount2].GetCrystalAbility()) 
							return SAME_CRYSTAL; // 같은능력치의 결정체 두개를 동시에 박을 수 없다.
					}
				}
			}
		}
		// 결정체가 장비에 조합이 가능한 능력을 가지고 있는지 검사. - 이미 검사했다.
		/*for( SI32 CrystalCount = 0; CrystalCount < _EquipItem->siUseCrystalNum; ++CrystalCount )
		{		
			if( 0 < _CrystalItem[CrystalCount].siUnique )
			{
				if(false == IsMixtureAbilityType(_EquipItem, _CrystalItem[CrystalCount].GetCrystalAbility()))
				{
					return LIMIT_CRYSTAL_ABILITY;
				}
			}
		}
		*/
	}
	// 중복되는 결정체가 있으면 조합하지 않게.
	/*for( SI32 EquipCount = 0; EquipCount < _EquipItem->siUseCrystalNum; ++EquipCount )
	{
		for( SI32 CrystalCount = 0; CrystalCount < _EquipItem->siUseCrystalNum; ++CrystalCount )
		{		
			if( 0 < _EquipItem->cltItemCrytal[ EquipCount ].GetCrystalType() &&
				0 < _CrystalItem[CrystalCount].siUnique )
			{
				if( _EquipItem->cltItemCrytal[ EquipCount ].GetCrystalAblilty() == _CrystalItem[CrystalCount].GetCrystalAbility()) 
				{
					// 아래 능력은 중복이 가능하다.
					switch(_CrystalItem[CrystalCount].GetCrystalAbility())
					{
						case CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK:
						case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:
						case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:	
						case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:
						case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:	
						case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:		
						case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:	 
						case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:	
						case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:	
						case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:	continue;
					}
					return SAME_CRYSTAL;
				}
			}
		}
	}
	*/

	// 결정체가 장비에 조합이 가능한 능력을 가지고 있는지 검사.
	/*for( SI32 CrystalCount = 0; CrystalCount < _EquipItem->siUseCrystalNum; ++CrystalCount )
	{		
		if( 0 < _CrystalItem[CrystalCount].siUnique )
		{
			if(false == IsMixtureAbilityType(_EquipItem, _CrystalItem[CrystalCount].GetCrystalAbility()))
			{
				return LIMIT_CRYSTAL_ABILITY;
			}
		}
	}
	*/

	return POSSIBLE;
}

// [추가 : 황진성 2008. 2. 30 => 결정체 제거가 가능한 데이타 인가?.]
BOOL cltItemManagerCommon::CanCrystalRemoveData( cltItem* _EquipItem, bool* _RemoveSlot, cltItem* _CrystalItem )
{
	if( 0 >= _EquipItem->siUnique )
	{
		return NO_ITEM_UNIQUE;
	}

	// 슬롯이 있는 아이템인지를 검사한다.
	if( SLOTITEMTYPE_NONE == UsedSlotItem( _EquipItem->siUnique ) )
	{
		return NO_SLOT_ITEM;
	}

	//
	SI32 RemoveCrystalNum = 0;
	for( SI32 Count = 0; Count < _EquipItem->siUseCrystalNum; ++Count )
	{
		if(true == _RemoveSlot[Count])
			++RemoveCrystalNum;
	}
	if(0 == RemoveCrystalNum)
		return NO_CRYSTAL_SLOT;


	for( SI32 Count = 0; Count < _EquipItem->siUseCrystalNum; ++Count )
	{
		if( true == _RemoveSlot[ Count ] )
		{
			if( 0 >= _EquipItem->cltItemCrytal[ Count ].siCrystalType )
			{
				return NO_CRYSTAL_SLOT;	// 슬롯에 제거할 결정체가 없다.
			}
		}
	}

	SI32 RemoveCrystalCount = 0;
	for( SI32 Count = 0; Count < _EquipItem->siUseCrystalNum; ++Count )
	{
		if( 0 < _CrystalItem[Count].siUnique && 0 < _CrystalItem[Count].siItemNum )
		{
			if(  _CrystalItem[Count].siUnique == ITEMUNIQUE(1803)) // 리무브 결정체.
			{
				++RemoveCrystalCount;
			}	
			else if(  _CrystalItem[Count].siUnique == ITEMUNIQUE(1804)) // 리차지 결정체.
			{
				if( true == _RemoveSlot[ Count ] )
				{
					if(_EquipItem->cltItemCrytal[Count].GetMaxDurability() <=1500)
						return NO_LOW_DURABILITY;
				}				
				++RemoveCrystalCount;			
			}
		}
	}

	if( 0 == RemoveCrystalCount )
	{
		return NO_REMOVE_CRYSTAL;
	}

	return POSSIBLE;
}

// [추가 : 황진성 2008. 2. 2 => 결정체로 슬롯 생성이 가능한 데이타 인가?.]
BOOL cltItemManagerCommon::CanCrystalSlotCreateData( cltItem* _EquipItem, cltItem* _CrystalItem )
{
	if( 0 >= _EquipItem->siUnique || 0 >= _CrystalItem->siUnique )
	{
		return NO_ITEM_UNIQUE;
	}
	
	// 슬롯이 있는 아이템인지를 검사한다.
	if( SLOTITEMTYPE_NONE == UsedSlotItem( _EquipItem->siUnique ) )
	{
		return NO_SLOT_ITEM;
	}

	// 모든 슬롯이 생성되있다면
	if( MAX_CRYSTAL_NUM <= _EquipItem->GetUseCrystalNum() )
	{
		return FULL_CRYSTAL_SLOT;
	}

	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique( _CrystalItem->siUnique );

	SI32 siType = pclClient->pclItemManager->pclItemInfo[ ref ]->siType; 

	// 결정체가 아니면 리턴.
	if( ITEMTYPE_CRYSTAL != siType )
	{
		return NO_CRYSTAL;
	}


	return POSSIBLE;
}


bool cltItemManagerCommon::IsMixtureAbilityType(cltItem* pclEquipItem, SI32 CristalItemAbilytype)
{
	enum{ ATTACK_EQUIP, DEFENCE_EQUIP, AVATAR_EQUIP};

	BOOL Crystal_TYPE = -1; 
	switch(CristalItemAbilytype)
	{
		// 방어구
		case CRYSTALABILITY_STATE_STR:
		case CRYSTALABILITY_STATE_DEX:			
		case CRYSTALABILITY_STATE_MAG:			
		case CRYSTALABILITY_STATE_VIT:			
		case CRYSTALABILITY_STATE_LUK:			
		case CRYSTALABILITY_STATE_HND:	
		case CRYSTALABILITY_STATE_WIS:
		case CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE:
		case CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE:
		case CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE:
		case CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE:
		case CRYSTALABILITY_EFFECT_MAGICAL_HITRATE:
		case CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE:
		case CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE:
		case CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE:	
		case CRYSTALABILITY_EFFECT_HITPOINT:
		case CRYSTALABILITY_EFFECT_MANAPOINT:
		case CRYSTALABILITY_EFFECT_WEIGHT:	
		case CRYSTALABILITY_SKILL_SUMMON:	
		case CRYSTALABILITY_SKILL_FARM:
		case CRYSTALABILITY_SKILL_MINE:
		case CRYSTALABILITY_SKILL_FISH:
		case CRYSTALABILITY_SKILL_MAKEFOOD:
		case CRYSTALABILITY_SKILL_CAPTURE:
		case CRYSTALABILITY_SKILL_HORSETRAINING:
		case CRYSTALABILITY_SKILL_MAKEIRON1:
		case CRYSTALABILITY_SKILL_MAKETOOL1:	
		case CRYSTALABILITY_SKILL_MAKEMED:
		case CRYSTALABILITY_SKILL_MAKESWORD1:
		case CRYSTALABILITY_SKILL_MAKESPEAR1:
		case CRYSTALABILITY_SKILL_MAKEAXE1:
		case CRYSTALABILITY_SKILL_MAKESTAFF1:	
		case CRYSTALABILITY_SKILL_MAKEBOW1:	
		case CRYSTALABILITY_SKILL_MAKEGUN1:	
		case CRYSTALABILITY_SKILL_MAKEARMOUR1:
		case CRYSTALABILITY_SKILL_MAKEHELMET1:
		case CRYSTALABILITY_SKILL_MAKESHOES1:	
		case CRYSTALABILITY_SKILL_MAKEBELT1:
		case CRYSTALABILITY_SKILL_ARTIFACT:			
		case CRYSTALABILITY_SKILL_USESWORD1:
		case CRYSTALABILITY_SKILL_USESPEAR1:	
		case CRYSTALABILITY_SKILL_USEAXE1:	
		case CRYSTALABILITY_SKILL_USESTAFF1:	
		case CRYSTALABILITY_SKILL_USEBOW1:	
		case CRYSTALABILITY_SKILL_USEGUN1:					Crystal_TYPE = DEFENCE_EQUIP; break;

			//무기
		case CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK:
		case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:
		case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:
		case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:		Crystal_TYPE = ATTACK_EQUIP; break;

		// 아바타
		case CRYSTALABILITY_AVATAR_STATE_STR:	
		case CRYSTALABILITY_AVATAR_STATE_DEX:	
		case CRYSTALABILITY_AVATAR_STATE_MAG:	
		case CRYSTALABILITY_AVATAR_STATE_VIT:	
		case CRYSTALABILITY_AVATAR_STATE_LUK:	
		case CRYSTALABILITY_AVATAR_STATE_HND:		
		case CRYSTALABILITY_AVATAR_STATE_WIS:	
		case CRYSTALABILITY_AVATAR_ALLMAKESKILL:	
		case CRYSTALABILITY_AVATAR_ALLWEAPONSKILL:	
		case CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL:		Crystal_TYPE = AVATAR_EQUIP; break;		
	}

	//BOOL Equip_TYPE = -1;
	BOOL Equip_TYPE = -1;
	SI32 eTYPE = pclEquipItem->GetType(pclClient->pclItemManager);

	switch(eTYPE)
	{
		case ITEMTYPE_AVATAR_MANTLE : 
		case ITEMTYPE_AVATAR_DRESS : 
		case ITEMTYPE_AVATAR_HAT : 
		case ITEMTYPE_AVATAR_NECK : 
		case ITEMTYPE_AVATAR_RING : 
			Equip_TYPE = AVATAR_EQUIP;
			break;
	}

	// 조합할 아이템에 결정체의 능력이 적용되면.
	if(Equip_TYPE == Crystal_TYPE)
		return true;
	// 무속성은 무조건 가능.
	else if(-1 == Crystal_TYPE)
		return true;
	else
		return false;
	
	// ATB를 이용해서 비교 할 필요가 없다.
	/*if( pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_WEAPON) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_BOW	  ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_SWORD ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_GUN	  ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_SPEAR ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_STAFF ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_AXE	  )		)
	{
		Equip_TYPE = ATTACK_EQUIP;
	}
	else
	{
		Equip_TYPE = DEFENCE_EQUIP;
	}

	// 조합할 아이템에 결정체의 능력이 적용되면.
	if(Equip_TYPE == Crystal_TYPE)
		return true;

	// 무속성은 무조건 가능.
	else if(-1 == Crystal_TYPE)
		return true;

	else
		return false;
	*/

}
// [성웅] 아이템몰에 쓸 파일네임과 이미지 인덱스를 가지고 옵니다.
void cltItemManagerCommon::GetItemImageItemmallFileName( SI32 siFromDBImgIndex, TCHAR* FileName, SI32 siLen,  SI32* ImageIndex )
{
	SI32 siFileIndex	=	0				;
	*ImageIndex = siFromDBImgIndex % MAX_ITEM_ITEMMALLIMAGE_NUM	;// 이게 사용할 이미지 인덱스
	// 파일 인덱스 넘버.
	siFileIndex = siFromDBImgIndex / MAX_ITEM_ITEMMALLIMAGE_NUM ;
	StringCchPrintf( FileName, siLen , TEXT("NInterface/Image/ImageC_80x80_00_00%d.SPR"), siFileIndex );
	if ( 0 > siFileIndex )
	{
		*ImageIndex = 0	;
		StringCchPrintf( FileName, siLen , TEXT("NInterface/Image/ImageC_80x80_00_000.SPR"));

	}
}
// [성웅] 아이템몰에 쓸 파일네임과 이미지 인덱스를 가지고 옵니다.
void  cltItemManagerCommon::GetItemNameItemmallFileName( SI32 siFromDBImgIndex, TCHAR* FileName, SI32 siLen,  SI32* ImageIndex )
{
	SI32 siFileIndex	=	0				;
	*ImageIndex = siFromDBImgIndex % MAX_ITEM_ITEMMALLIMAGE_NUM	;// 이게 사용할 이미지 인덱스
	// 파일 인덱스 넘버.
	siFileIndex = siFromDBImgIndex / MAX_ITEM_ITEMMALLIMAGE_NUM ;
	StringCchPrintf( FileName, siLen , TEXT("NInterface/Image/Image_90x20_00_00%d.SPR"), siFileIndex );
	if ( 0 > siFileIndex )
	{
		*ImageIndex = 0	;
		StringCchPrintf( FileName, siLen , TEXT("NInterface/Image/Image_90x20_00_000.SPR"));

	}
}

bool cltItemManagerCommon::CanRepairItem( cltItem* pclitem )
{
	if( pclitem == NULL )						return false;

	//-------------------------------------------------------------------------------------------------------
	// 아이템 타입에 따른 실패
	//-------------------------------------------------------------------------------------------------------
	switch ( pclitem->GetType(this) )
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_HAT:
	case ITEMTYPE_RING:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_NECK:
	case ITEMTYPE_AVATAR_MANTLE:
	case ITEMTYPE_AVATAR_DRESS:
	case ITEMTYPE_AVATAR_HAT:		
	case ITEMTYPE_AVATAR_NECK:	
	case ITEMTYPE_AVATAR_RING:
	case ITEMTYPE_MANTLE:
		{
			// 위에 있는 타입은 수리 가능하다
		}
		break;
	default:
		{	// 그외 타입은 수리 불가능
			return false;
		}
	}
	//-------------------------------------------------------------------------------------------------------


	SI32 siMaxDurability		= (SI32)pclClient->GetGlobalValue( TEXT("GV_MaxDurability") );
	SI32 siForeverDurability	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
	SI32 siForeverDurability2	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

	//-------------------------------------------------------------------------------------------------------
	// 내구도를 충전할 수 없는 아이템
	//-------------------------------------------------------------------------------------------------------
	SI32 dur	= pclitem->GetDurability();
	SI32 maxdur = pclitem->GetMaxDurability();

	if( dur    == siForeverDurability )			return false;
	if( dur    == siForeverDurability2 )		return false;
	if( maxdur == siForeverDurability )			return false;
	if( maxdur == siForeverDurability2 )		return false;

	if( dur == maxdur )							return false;
	//-------------------------------------------------------------------------------------------------------

	return true;
}

bool cltItemManagerCommon::IsRepairStone( cltItem* pclitem )
{
	if( pclitem == NULL )									return false;

	//-------------------------------------------------------------------------------------------------------
	// 수리석이다.
	if( pclitem->siUnique == ITEMUNIQUE(8123) )				return true;
	//-------------------------------------------------------------------------------------------------------

	return false;
}

bool cltItemManagerCommon::IsRepairStone( SI32 siItemUnique )
{
	if ( ITEMUNIQUE(8123) == siItemUnique )
	{
		return true;
	}

	return false;
}



bool cltItemManagerCommon::IsRepairExtraMaterial( cltItem* pclitem )
{
	if( pclitem == NULL )									return false;

	//-------------------------------------------------------------------------------------------------------
	if( pclitem->siUnique == ITEMUNIQUE(5370) )				return true;
	//-------------------------------------------------------------------------------------------------------

	return false;
}

SI16 cltItemManagerCommon::NeedRepairStoneNum( cltItem* pclitem )
{
	if( pclitem == NULL )									return 0;
	SI32 ref = FindItemRefFromUnique( pclitem->siUnique );
	if( ref <= 0 )											return 0;

	SI16 siNeedNum = max( 1, pclItemInfo[ref]->clSkillInfo.siSkillLevel / 5 );

	return siNeedNum;
}

SI16 cltItemManagerCommon::NeedBlessingStoneNum( cltItem* pclitem, SI32 BlessingStoneUnique )
{
	if( pclitem == NULL )									return 0;
	SI32 ref = FindItemRefFromUnique( pclitem->siUnique );
	if( ref <= 0 )											return 0;

	SI16 siNeedNum = 0;

	switch( BlessingStoneUnique )
	{
	case ITEMUNIQUE(5370):
		{
			REAL32 fConstantA = 2.0;
			if(		 pclitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 )	{		fConstantA = 5.0f;		}
			else if( pclitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )	{		fConstantA = 4.0f;		}
			else if( pclitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )	{		fConstantA = 2.5f;		}

			SI16 siNeedNum = max( 1, fConstantA * (pclItemInfo[ref]->clSkillInfo.siSkillLevel / 10.0f) );

			return siNeedNum;
		}
		break;
	}

	return siNeedNum;
}

GMONEY cltItemManagerCommon::RepairPrice( cltItem* pclitem )
{
	GMONEY moneyRet = 0;

	if( pclitem == NULL )										return moneyRet;
	SI32 ref = FindItemRefFromUnique( pclitem->siUnique );
	if( ref <= 0 )												return moneyRet;

	if( CanRepairItem( pclitem) == false )						return moneyRet;

	SI32 maxdur = pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
	SI32 dur = pclitem->clItemCommon.clCommonInfo.uiDurability;

	moneyRet = pclItemInfo[ref]->clSkillInfo.siSkillLevel * (maxdur - dur) * (clItemStatistic.siNPCPriceRate / 20);

	return moneyRet;
}

bool cltItemManagerCommon::CanMaxRepairItem( cltItem* pclitem )
{
	if( pclitem == NULL )																	return false;

	//-------------------------------------------------------------------------------------------------------
	// 아이템 타입에 따른 실패
	//-------------------------------------------------------------------------------------------------------
	switch ( pclitem->GetType(this) )
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_HAT:
	case ITEMTYPE_RING:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_NECK:
	case ITEMTYPE_AVATAR_MANTLE:
	case ITEMTYPE_AVATAR_DRESS:
	case ITEMTYPE_AVATAR_HAT:		
	case ITEMTYPE_AVATAR_NECK:	
	case ITEMTYPE_AVATAR_RING:
	case ITEMTYPE_MANTLE:
		{
			// 위에 있는 타입은 수리 가능하다
		}
		break;
	default:
		{	// 그외 타입은 수리 불가능
			return false;
		}
	}
	//-------------------------------------------------------------------------------------------------------


	UI32 uiMaxDurability		= (UI32)pclClient->GetGlobalValue( TEXT("GV_MaxDurability") );
	UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
	UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

	//-------------------------------------------------------------------------------------------------------
	// 최대 내구도를 충전할 수 없는 아이템
	//-------------------------------------------------------------------------------------------------------
	if( pclitem->clItemCommon.clCommonInfo.uiDurability    == uiForeverDurability )			return false;
	if( pclitem->clItemCommon.clCommonInfo.uiDurability    == uiForeverDurability2 )		return false;
	if( pclitem->clItemCommon.clCommonInfo.uiMaxDurability == uiForeverDurability )			return false;
	if( pclitem->clItemCommon.clCommonInfo.uiMaxDurability == uiForeverDurability2 )		return false;
	if( pclitem->clItemCommon.clCommonInfo.uiMaxDurability == uiMaxDurability)				return false;

	return true;
}

bool cltItemManagerCommon::IsMaxRepairStone( cltItem* pclitem )
{
	if( pclitem == NULL )									return false;

	//-------------------------------------------------------------------------------------------------------
	if( pclitem->siUnique == ITEMUNIQUE(5418) )				return true;
	//-------------------------------------------------------------------------------------------------------

	return false;

}

bool cltItemManagerCommon::IsMaxRepairStone( SI32 siItemUnique )
{
	if ( ITEMUNIQUE(5418) == siItemUnique )
	{
		return true;
	}

	return false;
}