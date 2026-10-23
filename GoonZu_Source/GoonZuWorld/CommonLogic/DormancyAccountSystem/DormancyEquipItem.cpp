#include ".\dormancyequipitem.h"


CDormancyEquipItem::CDormancyEquipItem(void)
{
	ZeroMemory(this, sizeof(CDormancyEquipItem));
}
CDormancyEquipItem::~CDormancyEquipItem(void)
{

}

// 업그레이드 될 타겟 장비 아이템의 유니크를 저장한다.
BOOL	CDormancyEquipItem::InsertTargetItem(SI32 siTargetUnique, SI32 siUpgradeLevel)
{
	if( 0 > siUpgradeLevel)
		return FALSE;

	if(siUpgradeLevel > MAX_ARRAY_SIZE)
		return FALSE;

#ifdef _DEBUG
	if( 0 != m_TargetItemUniqueArray[siUpgradeLevel])
		MsgBox("Error", "[CDormancyEquipItem]::check DormancyEquipItem.txt(Duplicate UpgradeLevel");
#endif

	m_TargetItemUniqueArray[siUpgradeLevel] = siTargetUnique;
	return TRUE;
}

// 업그레이드 될 타겟 장비 아이템의 유니크 값 리턴
SI32	CDormancyEquipItem::GetTargetItemUnique(SI32 siUpgradeLevel)
{
	if( 0 > siUpgradeLevel)
		return 0;

	if(MAX_ARRAY_SIZE <= siUpgradeLevel)
		return 0;

	for( SI32 siIndex = siUpgradeLevel; siIndex > 0 ; siIndex--  )
	{
		if( 0 != m_TargetItemUniqueArray[siIndex] )
			return m_TargetItemUniqueArray[siIndex];	
	}

	return 0;
}

VOID	CDormancyEquipItem::SetBoxUnique(SI32 siBoxUnique)
{
	m_siBoxUnique = siBoxUnique;
}
VOID	CDormancyEquipItem::SetUnique(SI32 siUnique)
{
	m_siUnique = siUnique;
}

const	SI32	CDormancyEquipItem::GetBoxUnique() const
{
	return m_siBoxUnique;
}
const	SI32	CDormancyEquipItem::GetUnique() const
{
	return m_siUnique;
}


//============================================================

CDormancyEquipItemMgr::CDormancyEquipItemMgr(void)
{
	ZeroMemory(this, sizeof(CDormancyEquipItemMgr));
}

CDormancyEquipItemMgr::~CDormancyEquipItemMgr(void)
{
	
}

//	데이터 생성 - 파일 읽고 메모리에 담기
BOOL	CDormancyEquipItemMgr::Create()
{
	NDataLoader	dataloader;

	BOOL bResult =FALSE;

	// 암호화 여부
	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/DormancyEquipItem.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/DormancyEquipItem.txt"));

	if (FALSE == bResult)
		return FALSE;

	SI32	siItemUnique = 0;
	SI32	siUpgradeLevel = 0;
	SI32	siTargetItemUnique = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&siItemUnique,	4,
			NDATA_INT32,	&siUpgradeLevel,	4,
			NDATA_INT32,	&siTargetItemUnique,	4,
			0,0,0
	};

	// 데이터가 담길 포인터 넘김
	dataloader.RegDataType(pDataTypeInfo);

	SI32 siCount = -1;

	// 데이터 읽어 오기 
	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			// 아이템 리스트에 존재 하지 않을 경우만 추가
			if(!IsExist(siItemUnique))
			{
				++siCount;
				if( MAX_ITEM_COUNT <= siCount)
				{
#ifdef _DEBUG
					if(MAX_ITEM_COUNT <= siCount)
						MsgBox("Error", "[CDormancyEquipItemMgr]::check MAX_ITEM_COUNT");
#endif
					break;
				}
				m_ItemArray[siCount].SetUnique(siItemUnique);
				
				// 아이템 정보에 휴면 계정 아이템 인지 아닌지 부울값 입력
				SetIsDormancyToItemInfo(siItemUnique);
			}
			m_ItemArray[siCount].InsertTargetItem(siTargetItemUnique,siUpgradeLevel);
		}
		else
			break;

	}
	return TRUE;
}
// 아이템 정보에 휴면 계정 아이템 인지 아닌지 부울값 입력
BOOL	CDormancyEquipItemMgr::SetIsDormancyToItemInfo(SI32 siUnique)
{
	cltItemManagerCommon *pclItemManager = pclClient->pclItemManager;
	if( NULL == pclItemManager)
		return FALSE;
	
	cltItemInfo *pclItemInfo = pclItemManager->FindItemInfo(siUnique);
	if( NULL == pclItemInfo)
		return FALSE;
	pclItemInfo->SetDormancy( TRUE );
	
	return TRUE;
}


// 휴면 계정 장비 아이템 
CDormancyEquipItem *CDormancyEquipItemMgr::GetDormancyEquipItem(SI32 siUnique)
{
	if( siUnique <= 0)
		return NULL;

	CDormancyEquipItem *pclDormancyEquipItem = NULL;
	for(SI32 i = 0; i< MAX_ITEM_COUNT;i++)
	{
		pclDormancyEquipItem = &m_ItemArray[i];

		if(NULL == pclDormancyEquipItem)
			return NULL;

		if(pclDormancyEquipItem->GetUnique() == siUnique)
		{
			return pclDormancyEquipItem;
		}	
	}

	return NULL;
}

// 아이템 유니크로 존재 하는지 여부 
BOOL	CDormancyEquipItemMgr::IsExist(SI32 siUnique)
{
	cltItemManagerCommon *pclItemManager = pclClient->pclItemManager;
	if( NULL == pclItemManager)
		return FALSE;

	cltItemInfo *pclItemInfo = pclItemManager->FindItemInfo(siUnique);
	if( NULL == pclItemInfo)
		return FALSE;
	
	return pclItemInfo->IsDormancy();
}