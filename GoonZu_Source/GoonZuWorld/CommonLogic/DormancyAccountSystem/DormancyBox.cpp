#include ".\dormancybox.h"

CDormancyBox::CDormancyBox(void)
{
	ZeroMemory(this, sizeof(CDormancyBox));
}

CDormancyBox::~CDormancyBox(void)
{

}

// 상자에 장비 아이템을 넣는다.  
BOOL	CDormancyBox::InsertItem(CDormancyEquipItem *pclDormancyEquipItem)
{
	if(m_siItemCount >= MAX_ITEM_COUNT)
		return FALSE;

	m_ItemArray[m_siItemCount] = pclDormancyEquipItem;

	++m_siItemCount;

	return TRUE;
}

// 상자에 들어 있는 아이템 수
SI32	CDormancyBox::GetItemCount() const
{
	return m_siItemCount;
}

VOID	CDormancyBox::SetUnique(SI32 siUnique)
{
	m_siUnique = siUnique;
}
SI32	CDormancyBox::GetUnique() const
{
	return m_siUnique;
}

// 인덱스로 장비 아이템을 얻는다.
CDormancyEquipItem*	CDormancyBox::GetEquipItem(SI32 siIndex)
{
	if( 0 > siIndex)
		return NULL;
	if(siIndex >= MAX_ITEM_COUNT)
		return NULL;

	return m_ItemArray[siIndex];
}

//================================================================================================================

CDormancyBoxMgr::CDormancyBoxMgr(CDormancyEquipItemMgr *pclDormancyEquipItemMgr)
{
	m_pclDormancyEquipItemMgr = pclDormancyEquipItemMgr;

	m_siCount = 0;
}

CDormancyBoxMgr::CDormancyBoxMgr(void)
{
	
}

CDormancyBoxMgr::~CDormancyBoxMgr(void)
{
	
}

//	데이터 생성 - 파일 읽고 메모리에 담기
BOOL	CDormancyBoxMgr::Create()
{
	NDataLoader	dataloader;

	bool bResult =FALSE;

	// 암호화 여부
	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/SelectBox.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/SelectBox.txt"));

	if (FALSE == bResult)
		return FALSE;

	if( NULL == m_pclDormancyEquipItemMgr )
		return FALSE;

	SI32	siBoxKind = 0;
	SI32	siItemUnique = 0;
	SI32	siBoxUnique = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siBoxKind,	4,
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siBoxUnique,	4,
		0,0,0
	};

	// 데이터가 담길 포인터 넘김
	dataloader.RegDataType(pDataTypeInfo);

	m_siCount = -1;
	CDormancyEquipItem* pTempDormancyEquipItem = NULL;
	
	// 데이터 읽어 오기 
	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			// 보급 상자 리스트에 존재 하지 않을 경우만 추가
			if(!IsExist(siBoxUnique))
			{
				++m_siCount;
				if( MAX_ITEM_COUNT <= m_siCount)
				{
#ifdef _DEBUG
					MsgBox("Error", "[CDormancyBoxMgr]::check MAX_ITEM_COUNT");
#endif
				break;
				}

				m_ItemArray[m_siCount].SetUnique(siBoxUnique);
			}

			// 보상 장비 아이템
			pTempDormancyEquipItem = m_pclDormancyEquipItemMgr->GetDormancyEquipItem(siItemUnique);

			pTempDormancyEquipItem->SetBoxUnique(siBoxUnique);

			// 보상 장비 아이템을 보급 상자에 넣는다.
			m_ItemArray[m_siCount].InsertItem(pTempDormancyEquipItem);
		}
		else
			break;

	}
	return TRUE;
}
// 아이템 유니크로 보급 상자가 존재 하는지 여부 
BOOL	CDormancyBoxMgr::IsExist(SI32 siUnique)
{
	if( NULL != GetDormancyBox(siUnique))
		return TRUE;
	
	return FALSE;
}

// 휴면 계정 보급 상자
CDormancyBox *CDormancyBoxMgr::GetDormancyBox(SI32 siUnique)
{
	if( siUnique <= 0)
		return NULL;

	CDormancyBox *pclDormancyBox = NULL;
	for(SI32 i = 0; i< MAX_ITEM_COUNT;i++)
	{
		pclDormancyBox = &m_ItemArray[i];

		if(NULL == pclDormancyBox)
			return NULL;

		if(pclDormancyBox->GetUnique() == siUnique)
		{
			return pclDormancyBox;
		}	
	}
	return NULL;
}

// 휴면 계정 보급 상자 (매니저의 인덱스로)
CDormancyBox *CDormancyBoxMgr::GetDormancyBoxByIndex(SI32 siIndex)
{
	if( 0 > siIndex)
		return NULL;

	if( siIndex >= MAX_ITEM_COUNT)
		return NULL;

	return &m_ItemArray[siIndex];
}

// 박스 수
SI32	CDormancyBoxMgr::GetCount()
{
	return m_siCount + 1;
}
