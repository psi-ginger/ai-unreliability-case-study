#include ".\dormancypointitem.h"

CDormancyPointItem::CDormancyPointItem(void)
{
	ZeroMemory(this, sizeof(CDormancyPointItem));
}
CDormancyPointItem::~CDormancyPointItem(void)
{
}

VOID	CDormancyPointItem::SetUnique(SI32 siUnique)
{
	m_siUnique = siUnique;
}
VOID	CDormancyPointItem::SetPoint(SI32 siPoint)
{
	m_siPoint = siPoint;
}

SI32	CDormancyPointItem::GetPoint() const
{
	return m_siPoint;
}
SI32	CDormancyPointItem::GetUnique() const
{
	return m_siUnique;
}

//========================================================================================================================================

CDormancyPointItemMgr::CDormancyPointItemMgr(void)
{
	ZeroMemory(this, sizeof(CDormancyPointItemMgr));
}

CDormancyPointItemMgr::~CDormancyPointItemMgr(void)
{
}

//	데이터 생성 - 파일 읽고 메모리에 담기
BOOL	CDormancyPointItemMgr::Create()
{
	NDataLoader	dataloader;

	BOOL bResult =FALSE;

	// 암호화 여부
	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/DormancyPointItem.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/DormancyPointItem.txt"));

	if (FALSE == bResult)
		return FALSE;

	SI32	siItemUnique = 0;
	SI32	siPoint = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siPoint,	4,
		0,0,0
	};

	// 데이터가 담길 포인터 넘김
	dataloader.RegDataType(pDataTypeInfo);

	// 데이터 읽어 오기 
	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			if( MAX_ITEM_COUNT <= m_siCount)
			{
#ifdef _DEBUG
				MsgBox("Error", "[CDormancyPointItemMgr]::check MAX_ITEM_COUNT");
#endif
				break;
			}
			
			m_ItemArray[m_siCount].SetPoint(siPoint);
			m_ItemArray[m_siCount].SetUnique(siItemUnique);				
			++m_siCount;
		}
		else
			break;

	}
	return TRUE;
}

// 휴면 계정 장비 아이템 
CDormancyPointItem *CDormancyPointItemMgr::GetDormancyPointItem(SI32 siUnique)
{
	if( siUnique <= 0)
		return NULL;

	CDormancyPointItem *pclItemInfo = NULL;

	for(SI32 i = 0; i< MAX_ITEM_COUNT;i++)
	{
		pclItemInfo = &m_ItemArray[i];

		if(NULL == pclItemInfo)
			continue;

		if(pclItemInfo->GetUnique() == siUnique)
		{
			return pclItemInfo;
		}
	}

	return NULL;
}

// 아이템 유니크로 존재 하는지 여부 
BOOL	CDormancyPointItemMgr::IsExist(SI32 siUnique)
{
	if(GetDormancyPointItem(siUnique) != NULL)
		return TRUE;

	return FALSE;
}

// 인덱스로 아이템 정보 얻기
CDormancyPointItem *CDormancyPointItemMgr::GetDormancyPointItemFromIndex(SI32 siIndex)
{
	if( (0 > siIndex) || (MAX_ITEM_COUNT <= siIndex))
		return NULL;

	return &m_ItemArray[siIndex];
}

// 포인트 아이템 종류 갯수
SI32	CDormancyPointItemMgr::GetCount()
{
	return m_siCount;
}