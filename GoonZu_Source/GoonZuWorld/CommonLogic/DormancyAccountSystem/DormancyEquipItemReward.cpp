#include ".\dormancyequipitemreward.h"

#include "../common/char/CharCommon/Char-Common.h"

CDormancyEquipItemReward::~CDormancyEquipItemReward(void)
{
	if(NULL != m_pclDormancyEquipItemMgr)
	{
		delete m_pclDormancyEquipItemMgr;
	}
	if(NULL != m_pclDormancyBoxMgr)
	{
		delete m_pclDormancyBoxMgr;
	}
}

CDormancyEquipItemReward::CDormancyEquipItemReward()
{
	// 보급 상자 관리 클래스
	m_pclDormancyEquipItemMgr = NULL;
	// 보상 장비 아이템 관리 클래스
	m_pclDormancyBoxMgr = NULL;
	// 공통 로직 객체
	m_pclCommon = pclClient;
}

//생성
BOOL	CDormancyEquipItemReward::Create()
{
	if(NULL != m_pclDormancyEquipItemMgr)
		return FALSE;
	if(NULL != m_pclDormancyBoxMgr)
		return FALSE;

	m_pclDormancyEquipItemMgr = new CDormancyEquipItemMgr();
	if(NULL == m_pclDormancyEquipItemMgr)
		return FALSE;
	if(!m_pclDormancyEquipItemMgr->Create())
		return FALSE;

	m_pclDormancyBoxMgr = new CDormancyBoxMgr(m_pclDormancyEquipItemMgr);
	if(NULL == m_pclDormancyBoxMgr)
		return FALSE;
	if(!m_pclDormancyBoxMgr->Create())
		return FALSE;

	return TRUE;
}

//보급 상자에 들어 있는 아이템을 인덱스 순서 대로 본다.
cltItemInfo* CDormancyEquipItemReward::GetItemInfoInBox(SI32 siBoxUnique, SI32 siIndex)
{
	if( NULL == m_pclDormancyBoxMgr)
		return NULL;
	if( NULL == m_pclDormancyEquipItemMgr)
		return NULL;

	CDormancyBox* pclDormancyBox = m_pclDormancyBoxMgr->GetDormancyBox(siBoxUnique);
	if(NULL == pclDormancyBox)
		return NULL;

	CDormancyEquipItem* pclDormancyEquipItem = pclDormancyBox->GetEquipItem(siIndex);
	if(NULL == pclDormancyEquipItem)
		return NULL;

	return m_pclCommon->pclItemManager->FindItemInfo(pclDormancyEquipItem->GetUnique());
}

//보급 상자에 들어 있는 아이템을 인덱스 순서 대로 본다.
SI32 CDormancyEquipItemReward::GetItemUniqueInBox(SI32 siBoxUnique, SI32 siIndex)
{
	if( NULL == m_pclDormancyBoxMgr)
		return -1;
	if( NULL == m_pclDormancyEquipItemMgr)
		return -1;

	CDormancyBox* pclDormancyBox = m_pclDormancyBoxMgr->GetDormancyBox(siBoxUnique);
	if(NULL == pclDormancyBox)
		return -1;

	CDormancyEquipItem* pclDormancyEquipItem = pclDormancyBox->GetEquipItem(siIndex);
	if(NULL == pclDormancyEquipItem)
		return -1;

	return pclDormancyEquipItem->GetUnique();
}

//장비 아이템 변화하기
BOOL	CDormancyEquipItemReward::UpgradeItemInven(cltCharCommon *pclChar)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclCommon)
		return FALSE;

	// 캐릭터의 인벤토리의 아이템
	cltPItemInfo &pclPItemInfo = pclChar->pclCI->clCharItem;

	// 아이템 매니저
	cltItemManagerCommon *pclItemManager = m_pclCommon->pclItemManager;
	if( NULL == pclItemManager)
		return FALSE;

	//	보상 아이템 정보
	cltItemInfo *pclDormancyItemInfo = NULL;

	cltItem *pclItem = NULL;
	for(SI32 siItemPos = 0 ; siItemPos < MAX_ITEM_PER_PERSON; siItemPos++)
	{
		pclItem = &pclPItemInfo.clItem[siItemPos];
		if( NULL == pclItem)
			continue;

		pclDormancyItemInfo = pclItemManager->FindItemInfo(pclItem->siUnique);
		if(NULL == pclDormancyItemInfo)
			continue;
		if(pclDormancyItemInfo->IsDormancy())
		{
			UpgradeItem(pclChar, pclItem->siUnique, siItemPos, pclDormancyItemInfo->siType);
		}		
	}
	return TRUE;
}

//장비 아이템 변화하기
BOOL	CDormancyEquipItemReward::UpgradeItem(cltCharCommon *pclChar, SI32 siItemUnique, SI32 siItemPos, SI32 siType)
{
	if( NULL == pclChar)
		return FALSE;
	if( (0 > siItemPos) || (siItemPos >= MAX_ITEM_PER_PERSON) )
		return FALSE;

	// 휴면 계정 장비 아이템 정보
	CDormancyEquipItem*	pclDormancyEquipItem = m_pclDormancyEquipItemMgr->GetDormancyEquipItem(siItemUnique);
	if(NULL == pclDormancyEquipItem)
		return FALSE;

	// 캐릭터의 인벤토리의 아이템
	cltPItemInfo &pclPItemInfo =  pclChar->pclCI->clCharItem;

	// 아이템 매니저
	cltItemManagerCommon *pclItemManager = m_pclCommon->pclItemManager;
	if( NULL == pclItemManager)
		return FALSE;

	// 휴면 계정 장비 아이템
	cltItem &pclItem = pclPItemInfo.clItem[siItemPos];
	
	// 보상 아이템이 업그레이드 될 타켓 아이템 유니크
	SI32 siTargetItemUnique = 0;
	//	보상 아이템이 업그레이드 될 타켓 아이템 정보
	cltItemInfo *pclTargetItemInfo = NULL;
	// 캐릭터가 변할 레벨
	SI32 siNextLevel = pclChar->pclCI->clIP.GetLevel() + 1;

	if( (siTargetItemUnique =  pclDormancyEquipItem->GetTargetItemUnique(siNextLevel)) == 0 )
	{
		return FALSE;
	}
	pclTargetItemInfo = pclItemManager->FindItemInfo(siTargetItemUnique);

	if(NULL == pclTargetItemInfo)
		return FALSE;

	switch(siType)
	{
		//무기아이템 값 넣기
	case ITEMTYPE_SWORD:				// 검
	case ITEMTYPE_SPEAR:				// 창
	case ITEMTYPE_STAFF:				// 지팡이
	case ITEMTYPE_AXE:					// 도끼
	case ITEMTYPE_BOW:					// 활
	case ITEMTYPE_GUN:					// 총
		{
			pclItem.clItemWeapon.uiMinDamage = pclTargetItemInfo->clItem.clItemWeapon.uiMinDamage;
			pclItem.clItemWeapon.uiMaxDamage = pclTargetItemInfo->clItem.clItemWeapon.uiMaxDamage;
			pclItem.clItemWeapon.uiMag = pclTargetItemInfo->clItem.clItemWeapon.uiMag;
			break;
		}
		//방어아이템 값 넣기
	case ITEMTYPE_ARMOUR:				// 갑옷
	case ITEMTYPE_BELT:					// 허리띠
	case ITEMTYPE_SHOES:				// 신발
	case ITEMTYPE_HELMET:				// 투구
		{
			pclItem.clItemArmour.uiAC = pclTargetItemInfo->clItem.clItemArmour.uiAC;
			pclItem.clItemArmour.uiLuck = pclTargetItemInfo->clItem.clItemArmour.uiLuck;
			pclItem.clItemArmour.uiSkillUp = pclTargetItemInfo->clItem.clItemArmour.uiSkillUp;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}

//장비 아이템 변화하기(특정 아이템)
BOOL	CDormancyEquipItemReward::UpgradeItemInfo(cltCharCommon *pclChar,cltItem *pclItem)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == pclItem)
		return FALSE;

	// 휴면 계정 장비 아이템 정보
	CDormancyEquipItem*	pclDormancyEquipItem = m_pclDormancyEquipItemMgr->GetDormancyEquipItem(pclItem->siUnique);
	if(NULL == pclDormancyEquipItem)
		return FALSE;

	// 아이템 매니저
	cltItemManagerCommon *pclItemManager = m_pclCommon->pclItemManager;
	if( NULL == pclItemManager)
		return FALSE;

	// 보상 아이템이 업그레이드 될 타켓 아이템 유니크
	SI32 siTargetItemUnique = 0;
	//	보상 아이템이 업그레이드 될 타켓 아이템 정보
	cltItemInfo *pclTargetItemInfo = NULL;

	if( (siTargetItemUnique =  pclDormancyEquipItem->GetTargetItemUnique(pclChar->pclCI->clIP.GetLevel())) == 0 )
	{
		return FALSE;
	}
	pclTargetItemInfo = pclItemManager->FindItemInfo(siTargetItemUnique);

	if(NULL == pclTargetItemInfo)
		return FALSE;

	switch(pclItem->GetType(pclItemManager))
	{
		//무기아이템 값 넣기
	case ITEMTYPE_SWORD:				// 검
	case ITEMTYPE_SPEAR:				// 창
	case ITEMTYPE_STAFF:				// 지팡이
	case ITEMTYPE_AXE:					// 도끼
	case ITEMTYPE_BOW:					// 활
	case ITEMTYPE_GUN:					// 총
		{
			pclItem->clItemWeapon.uiMinDamage = pclTargetItemInfo->clItem.clItemWeapon.uiMinDamage;
			pclItem->clItemWeapon.uiMaxDamage = pclTargetItemInfo->clItem.clItemWeapon.uiMaxDamage;
			pclItem->clItemWeapon.uiMag = pclTargetItemInfo->clItem.clItemWeapon.uiMag;
			break;
		}
		//방어아이템 값 넣기
	case ITEMTYPE_ARMOUR:				// 갑옷
	case ITEMTYPE_BELT:					// 허리띠
	case ITEMTYPE_SHOES:				// 신발
	case ITEMTYPE_HELMET:				// 투구
		{
			pclItem->clItemArmour.uiAC = pclTargetItemInfo->clItem.clItemArmour.uiAC;
			pclItem->clItemArmour.uiLuck = pclTargetItemInfo->clItem.clItemArmour.uiLuck;
			pclItem->clItemArmour.uiSkillUp = pclTargetItemInfo->clItem.clItemArmour.uiSkillUp;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}


// 휴면 장비 아이템인지 확인한다.
BOOL	CDormancyEquipItemReward::IsDormancyEquipItem(SI32 siItemUnique)
{
	if( NULL == m_pclDormancyEquipItemMgr)
		return FALSE;

	return m_pclDormancyEquipItemMgr->IsExist(siItemUnique);
	
	
}