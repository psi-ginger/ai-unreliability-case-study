#include "HuntQuestItemMgr.h"
#include "../Lib/dxutil.h"

#include "../Client/Client.h"

extern cltCommonLogic* pclClient;

CHuntQuestItemMgr::CHuntQuestItemMgr()
{
	//...
	ZeroMemory(m_HuntQuestItem_Unit_info, sizeof(m_HuntQuestItem_Unit_info));
	m_siQuestUniqueNum = 0;
}


CHuntQuestItemMgr::~CHuntQuestItemMgr()
{
	
}


bool CHuntQuestItemMgr::Initialize()			//	매니저 초기화
{
	bool bResult = false;

	//	매니저 초기화 작업.. 파일로드 등등
	bResult = LoadHuntQuestItemData();

	return bResult;
}

bool CHuntQuestItemMgr::LoadHuntQuestItemData()
{
	NDataLoader	dataloader;

	bool bResult = false;

	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Quest/HuntQuestItem.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Quest/HuntQuestItem.txt"));

	if (bResult == false)
		return false;

	SI32	siIndex			= 0;
	SI32	siQuestUnique	= 0;
	TCHAR	szMonsterKind[MAX_PATH]	= TEXT("");
	SI32	siItemUnique	= 0;
	SI32	siRandomRate	= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,		4,
		NDATA_INT32,	&siQuestUnique,	4,
		NDATA_MBSTRING,	szMonsterKind,	MAX_PATH,
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siRandomRate,	4,
		0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo);

	SI32 siCount = 0;

	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			//if(m_siUnitDataNum >= MAX_HUNTQUESTITEM_UNIT_NUM)
			//{
			//	#ifdef _DEBUG
			//	MessageBox( NULL, TEXT("Please Check HuntQuestItem.txt File - Data Num"), TEXT("Data Load Error"), MB_OK|MB_TOPMOST );
			//	#endif

			//	return false;
			//}

			for(SI32 i = 0; i < MAX_HUNTQUESTITEM_UNIQUE_NUM; i++)
			{
				if(m_HuntQuestItem_Unit_info[i].m_siQuestUnique == siQuestUnique)
				{
					for(SI32 j = 0; j < MAX_HUNTQUESTITEM_UNIT_NUM; j++)
					{
						if(m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[j].m_siMonsterKind == 0)
						{
							// 읽은 데이터 세팅
							//...
							m_HuntQuestItem_Unit_info[i].m_siQuestUnique							= siQuestUnique;
							m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[j].m_siMonsterKind	= pclClient->GetUniqueFromHash(TEXT(szMonsterKind));
							m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[j].m_siItemUnique		= siItemUnique;
							m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[j].m_siRandomRate		= siRandomRate;
							break;
						}
					}
					break;
				}
				else if(m_HuntQuestItem_Unit_info[i].m_siQuestUnique == 0)
				{
					m_HuntQuestItem_Unit_info[i].m_siQuestUnique							= siQuestUnique;
					m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[0].m_siMonsterKind	= pclClient->GetUniqueFromHash(TEXT(szMonsterKind));
					m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[0].m_siItemUnique		= siItemUnique;
					m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[0].m_siRandomRate		= siRandomRate;
					break;
				}
			}
		}
	}


	for(SI32 i = 0; i < MAX_HUNTQUESTITEM_UNIQUE_NUM; i++)		
	{
		if(m_HuntQuestItem_Unit_info[i].m_siQuestUnique > 0)
			m_siQuestUniqueNum++;
	}

	return false;	
}



HuntQuestItem_Unit* CHuntQuestItemMgr::FindQuestItem(SI32 siQuestUnique, SI32 siMonsterKind)
{
	for(SI32 i = 0; i < m_siQuestUniqueNum; i++)
	{
		if(m_HuntQuestItem_Unit_info[i].m_siQuestUnique == siQuestUnique)
		{
			for(SI32 j = 0; j < MAX_HUNTQUESTITEM_UNIT_NUM; j++)
			{
				if(m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[j].m_siMonsterKind == siMonsterKind)
					return &m_HuntQuestItem_Unit_info[i].m_HuntQuestItem_Unit[j];
			}
		}
	}
	
	return NULL;
}









