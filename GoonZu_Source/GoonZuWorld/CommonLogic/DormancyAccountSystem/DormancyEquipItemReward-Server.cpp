#include ".\dormancyequipitemreward.h"

#include "../CommonLogic/MsgRval-Define.h"
#include "../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../Server/Server.h"

CDormancyEquipItemRewardServer::CDormancyEquipItemRewardServer(void) : CDormancyEquipItemReward()
{
	m_pclServer = (cltServer *)pclClient;
}

CDormancyEquipItemRewardServer::~CDormancyEquipItemRewardServer(void)
{
}

//장비 아이템 지급
BOOL	CDormancyEquipItemRewardServer::GiveItemToPlayer(SI32 siBoxUnique, SI32 siItemUnique, SI32 siIBoxPos, cltCharServer *pclChar, SI32 siItemIndex)
{
	if( NULL == pclChar)
		return FALSE;
	if( (0 > siIBoxPos) || siIBoxPos >= MAX_ITEM_PER_PERSON)
		return FALSE;

	CDormancyBox*	pclDormancyBox = NULL;
	CDormancyEquipItem*	pclDormancyEquipItem = NULL;
	const SI32	ITEMNUM = 1;

	//	보급 박스 얻기
	if( NULL == (pclDormancyBox = m_pclDormancyBoxMgr->GetDormancyBox(siBoxUnique)))
	{
		return FALSE;
	}
	
	//	보급 박스 안의 장비 아이템 지급
	if( NULL == (pclDormancyEquipItem = pclDormancyBox->GetEquipItem(siItemIndex)))
	{
		return FALSE;
	}

	SI16 brareswitch = 0;
	cltItem clSelectedItem;					// 선택한 아이템 
	clSelectedItem.Init();
	
	// 아이템 기간을 얻어 오기
	// 아이템 사용기간을 얻어오기위한 사전작업
	cltItemInfo* pclItemInfo = m_pclServer->pclItemManager->FindItemInfo(siItemUnique);
	if( NULL == pclItemInfo )

		return FALSE;

	cltItem *pclItem = &pclItemInfo->clItem;
	// 아이템 사용기간
	UI32 uiUsedate = pclItem->uiDateUseDay;

	if( FALSE == m_pclServer->pclItemManager->MakeRandItemUnique(siItemUnique, &clSelectedItem, 0, 0, &brareswitch,0,0,0,0,0,uiUsedate))
		return FALSE;

	clSelectedItem.SetItemNum( ITEMNUM );

	// 아이템 업그레이드
	UpgradeItemInfo(pclChar, &clSelectedItem);

	// 밀봉 취소
	clSelectedItem.CancellationSealingUp( m_pclServer->sTime );

	//------------------------------------------
	// 인벤에 물품을 넣을 공간이 있는지 확인한다. 
	//------------------------------------------
	SI16 siInvPos;
	cltItem clToItem;
	SI32 rtnvalue;
	if(pclChar->CanAddInv(INVMODE_ALL, &clSelectedItem, &siInvPos, &clToItem, &rtnvalue) == false)
	{
		m_pclServer->SendServerResponseMsg(0, SRVAL_FAIL_IS_FULLIVEN, 100, 0, pclChar->GetID());
		return false;
	}

	// 캐릭터 인벤토리에 있는 박스 아이템
	cltItem* pclBoxItem = &pclChar->pclCI->clCharItem.clItem[siIBoxPos];
	cltItem clUsedBoxItem;
	clUsedBoxItem.Set(pclBoxItem);
	clUsedBoxItem.SetItemNum(ITEMNUM);

	// DB로 '상자 열기' 요청
	sDBRequest_OpenTreasureBox clMsg( pclChar->pclCI->GetPersonID(), siIBoxPos, &clUsedBoxItem, 0, NULL, siInvPos, &clSelectedItem, m_pclServer->pclItemManager->CanPile(clSelectedItem.siUnique), clSelectedItem.IsCreateSID(m_pclServer->pclItemManager), false);
	m_pclServer->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	
	return TRUE;
}

//시스템 보상 : 장비 보급 상자 2종 주기
BOOL	CDormancyEquipItemRewardServer::GiveEquipBox(cltCharServer *pclCharServer)
{
	if ( NULL == pclCharServer)
		return FALSE;
	if ( NULL == m_pclDormancyBoxMgr)
		return FALSE;
	if ( NULL == m_pclServer->pclGameDBProcess)
		return FALSE;
	
	// 휴면 계정 상태가 아니라면 보급 하지 않는다.
	CDormancyInfo &rclDormancyInfo = pclCharServer->pclCI->clDormancyInfo;
	if( FALSE == rclDormancyInfo.GetDormancyState())
		return FALSE;

	TCHAR*	pReasonText = GetTxtFromMgr( 40091 );	// 지급하는 이유

	cltSystemRewardInfo		clInfo;			// 아이템 보상에 따른 정보
	clInfo.m_siIndex = SystemRewardInfoType_DormancyEquipItem; // 시스템 보상 종류 인덱스
	// 받은 날짜
	clInfo.m_sGiveTime		= m_pclServer->sTime;
	TimeAddDate(&clInfo.m_sGiveTime, CDormancyBox::GIVESYSTEMDATE, &clInfo.m_sExpireTime);
	// 지급 이유
	if ( pReasonText )
	{
		StringCchCopy( clInfo.m_szReason, 256, pReasonText );
	}

	// 박스 아이템
	CDormancyBox *pclBox = NULL;
	// 아이템 참조 인덱스
	SI32 ref = 0;
	// 지급 아이템 
	cltItem *pclRewardItem = NULL;

	// 지급 해야 할 박스 아이템들의 존재 검사
	BOOL bError = FALSE;
	for(SI32 i = 0 ; i < m_pclDormancyBoxMgr->GetCount(); i++)
	{	
		if(NULL == m_pclDormancyBoxMgr->GetDormancyBoxByIndex(i))
		{
			bError = TRUE;
		}
	}
	
	// 검사 후 이상 없을 시
	if ( FALSE == bError)
	{
		cltItemInfo* pclItemInfo = NULL;
		cltItem *pclItem = NULL;
		for(SI32 i = 0 ; i < m_pclDormancyBoxMgr->GetCount(); i++)
		{	
			pclBox = m_pclDormancyBoxMgr->GetDormancyBoxByIndex(i);
			// 아이템
			pclItemInfo = m_pclServer->pclItemManager->FindItemInfo(pclBox->GetUnique());
			if( NULL == pclItemInfo )
				return FALSE;

			pclRewardItem = &pclItemInfo->clItem;
			clInfo.m_clItem.Set( pclRewardItem );

			// 아이템을 지급 하라는 요청
			sDBRequest_GiveSystemReward clMsgReward( GIVEREWARD_SERVER, pclCharServer->pclCI->GetPersonID(), pclCharServer->GetID(), &clInfo );
			m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgReward);
		}
		// 유저에게 지급되었다는 메시지를 띄워줌
		m_pclServer->SendServerResponseMsg( 0, SRVAL_REWARDITEM_FOR_DORMANCYEQUIPBOX, 0, 0, pclCharServer->GetID() );

		return TRUE;
	}
	return FALSE;
}

//장비 아이템 변화하기
BOOL	CDormancyEquipItemRewardServer::UpgradeItem(cltCharCommon *pclChar, SI32 siItemUnique, SI32 siItemPos, SI32 siType)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclServer->pclGameDBProcess)
		return FALSE;

	// 휴면 계정 장비 아이템 정보
	CDormancyEquipItem*	pclDormancyEquipItem = m_pclDormancyEquipItemMgr->GetDormancyEquipItem(siItemUnique);
	if(NULL == pclDormancyEquipItem)
		return FALSE;
	// 아이템 매니져
	cltItemManagerCommon *pclItemManager = m_pclServer->pclItemManager;
	if( NULL == pclItemManager)
		return FALSE;


	//================= 1. 업그래이드할 인벤 속의 아이템 =========================================================//
	// 캐릭터의 인벤토리의 아이템
	cltPItemInfo &rclPItemInfo =  pclChar->pclCI->clCharItem;
	// 휴면 계정 장비 아이템
	cltItem &rclItem = rclPItemInfo.clItem[siItemPos];

	//================= 2. 휴면 아이템 원본 복사 ================================================================//
	cltItem clItemSource;
	clItemSource.Set(&rclItem);

	//================= 3. 타켓 아이템 구하기====================================================================// 
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
	// 타겟 아이템
	cltItem &rclTargetItem = pclTargetItemInfo->clItem;

	//================= 4. 기존과 같은 능력치 인지 검사 & 휴면 아이템 능력치 업그래이드=====================// 
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
			if((rclItem.clItemWeapon.uiMinDamage == rclTargetItem.clItemWeapon.uiMinDamage)
				&& (rclItem.clItemWeapon.uiMaxDamage == rclTargetItem.clItemWeapon.uiMaxDamage)
				&& (rclItem.clItemWeapon.uiMag == rclTargetItem.clItemWeapon.uiMag))
			{
				return FALSE;
			}

			clItemSource.clItemWeapon.uiMinDamage = rclTargetItem.clItemWeapon.uiMinDamage;
			clItemSource.clItemWeapon.uiMaxDamage = rclTargetItem.clItemWeapon.uiMaxDamage;
			clItemSource.clItemWeapon.uiMag = rclTargetItem.clItemWeapon.uiMag;
			break;
		}
		//방어아이템 값 넣기
	case ITEMTYPE_ARMOUR:				// 갑옷
	case ITEMTYPE_BELT:					// 허리띠
	case ITEMTYPE_HELMET:				// 투구
		{
			if((rclItem.clItemArmour.uiAC == rclTargetItem.clItemArmour.uiAC)
				&& (rclItem.clItemArmour.uiLuck == rclTargetItem.clItemArmour.uiLuck)
				&& (rclItem.clItemArmour.uiSkillUp == rclTargetItem.clItemArmour.uiSkillUp))
			{
				return FALSE;
			}

			clItemSource.clItemArmour.uiAC = pclTargetItemInfo->clItem.clItemArmour.uiAC;
			clItemSource.clItemArmour.uiLuck = pclTargetItemInfo->clItem.clItemArmour.uiLuck;
			clItemSource.clItemArmour.uiSkillUp = pclTargetItemInfo->clItem.clItemArmour.uiSkillUp;
			break;
		}
	case ITEMTYPE_SHOES:				// 신발
		{
			if((rclItem.clItemShoes.uiAC == rclTargetItem.clItemShoes.uiAC)
				&& (rclItem.clItemShoes.uiMoveSpeed == rclTargetItem.clItemShoes.uiMoveSpeed))
			{
				return FALSE;
			}

			clItemSource.clItemShoes.uiAC = pclTargetItemInfo->clItem.clItemShoes.uiAC;
			clItemSource.clItemShoes.uiMoveSpeed = pclTargetItemInfo->clItem.clItemShoes.uiMoveSpeed;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	//================= 6. DB로 아이템 변경 메시지 전송========================================================// 
	sDBRequest_ExchangeItem clSendMsg(pclChar->pclCI->GetPersonID(), siItemPos, &rclItem, &clItemSource, EXCHANGEITEM_REASON_DORMANCY);
	m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clSendMsg);

	return TRUE;
}