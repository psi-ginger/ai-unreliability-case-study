#include ".\dormancypointreward.h"

#include "../common/char/CharCommon/Char-Common.h"
CDormancyPointReward::CDormancyPointReward(void)
{
	// 포인트 아이템 관리 클래스
	m_pclDormancyPointItemMgr = NULL;
	// 공통 로직 객체
	m_pclCommon = pclClient;
}

CDormancyPointReward::~CDormancyPointReward(void)
{
	if(NULL != m_pclDormancyPointItemMgr)
	{
		delete m_pclDormancyPointItemMgr;
		m_pclDormancyPointItemMgr = NULL;
	}
}

//생성
BOOL		CDormancyPointReward::Create()
{
	if(NULL != m_pclDormancyPointItemMgr)
		return FALSE;

	m_pclDormancyPointItemMgr = new CDormancyPointItemMgr();
	if(NULL == m_pclDormancyPointItemMgr)
		return FALSE;
	if(!m_pclDormancyPointItemMgr->Create())
		return FALSE;

	return TRUE;
}

//인덱스로 아이템 정보를 알아 낸다
cltItemInfo	*CDormancyPointReward::GetItemInfo(SI32 siIndex)
{
	if( NULL == m_pclDormancyPointItemMgr)
		return NULL;
	if( NULL == m_pclCommon->pclItemManager)
		return NULL;

	CDormancyPointItem *pclPointItem = m_pclDormancyPointItemMgr->GetDormancyPointItemFromIndex(siIndex);
	if( NULL == pclPointItem)
		return NULL;

	return m_pclCommon->pclItemManager->FindItemInfo(pclPointItem->GetUnique());
}

//인덱스로 아이템 유니크를 알아 낸다.
SI32		CDormancyPointReward::GetItemUnique(SI32 siIndex)
{
	SI32 siReturnValue = 0;
	cltItemInfo	*pclItemInfo = GetItemInfo(siIndex);
	if(NULL != pclItemInfo)
		siReturnValue = pclItemInfo->clItem.siUnique;

	return siReturnValue;
}

//포인트 아이템 종류 갯수
SI32		CDormancyPointReward::GetItemCount()
{
	if( NULL == m_pclDormancyPointItemMgr)
		return 0;
	return m_pclDormancyPointItemMgr->GetCount();
}

//아이템의 휴면 포인트를 매니저의 인덱스로 알아낸다.
SI32		CDormancyPointReward::GetItemPoint(SI32 siIndex)
{
	if( NULL == m_pclDormancyPointItemMgr)
		return -1;

	CDormancyPointItem *pclPointItem = m_pclDormancyPointItemMgr->GetDormancyPointItemFromIndex(siIndex);
	if( NULL == pclPointItem)
		return -1;
	
	return pclPointItem->GetPoint();
}