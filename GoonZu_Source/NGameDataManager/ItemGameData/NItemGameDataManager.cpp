//=============================================================================
//
//=============================================================================

#include "NItemGameDataManager.h"
#include "../../NLib/NDataLoader.h"
#include "../../NLib/NTextManager.h"
#include "../../GoonZuWorld/common/CommonHeader.h"

NItemGameDataManager::NItemGameDataManager()
{
	for(SI32 i=0; i<MAX_ITEM_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NItemGameDataManager::~NItemGameDataManager()
{
	for(SI32 i=0; i<MAX_ITEM_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NItemGameDataManager::Init(SI32 siServiceArea)
{
	return LoadItemGameData(siServiceArea);
}

// [기형] 09.8.17 : ntx파일 읽기로 변경
BOOL NItemGameDataManager::LoadItemGameData(SI32 siServiceArea)
{
	NTextManager clTextManager;
	clTextManager.CreateTextManager();
	
	TCHAR szTextMgrFileName[MAX_PATH] = TEXT("");

	const SI32 COL_COUNT = 1;

	switch ( siServiceArea )
	{
	case ConstServiceArea_Korea :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_KOR.ntx") );
		}
		break;
	case ConstServiceArea_Japan :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_JPN.ntx") );
		}
		break;
	case ConstServiceArea_China :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_CHI.ntx") );
		}
		break;
	case ConstServiceArea_English :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_ENG.ntx") );
		}
		break;
	case ConstServiceArea_Taiwan :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_TAI.ntx") );
		}
		break;
	case ConstServiceArea_USA :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_ENG.ntx") );
		}
		break;
	case ConstServiceArea_NHNChina :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_CHI.ntx") );
		}
		break;
	case ConstServiceArea_EUROPE :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_EUR.ntx") );
		}
		break;
	default:
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Item_ENG.ntx") );
		}
	}

	clTextManager.LoadTextFromCryptFile(szTextMgrFileName, COL_COUNT);

	TCHAR *szBuffer = NULL ;
	TCHAR  szTmp[64] = { '\0', };
	NItemGameData *pkNewData = NULL;
	for(SI32 i = 0; i < MAX_ITEM_COUNT; i++)
	{
		_itot(i, szTmp, 10);
		szBuffer = clTextManager.GetText( szTmp );

		if(szBuffer != NULL)
		{
			pkNewData = new NItemGameData;
			if(pkNewData)
			{	
				pkNewData->m_siUnique	= i;
				StringCchCopy(pkNewData->m_szItemName, sizeof(pkNewData->m_szItemName), szBuffer);
				m_pkData[i] = pkNewData;
			}
		}
	}

	return TRUE;
}

//BOOL NItemGameDataManager::AddItemData(NDataTypeInfo* pDataTypeInfo, TCHAR* pszFileName)
//{
//	NDataLoader	dataloader;
//	BOOL bLoadFile = FALSE;
//
//	bLoadFile = dataloader.LoadDataFromCryptFile( pszFileName );
//
//	if (!bLoadFile)
//	{
//		return FALSE;
//	}
//
//	dataloader.RegDataType( pDataTypeInfo );
//
//	for(SI32 i = 0; i < MAX_ITEM_COUNT; i++)
//	{
//		NItemGameData* pkNewData = new NItemGameData;
//		if(pkNewData)
//		{
//			pkNewData->m_siUnique	= m_siItemUnique;
//			StringCchCopy(pkNewData->m_szItemName, sizeof(pkNewData->m_szItemName), m_szItemName);
//
//			m_pkData[m_siItemUnique] = pkNewData;
//		}
//	}
//
//	return TRUE;
//}
//
//BOOL NItemGameDataManager::AddItemData_UnCrypt(NDataTypeInfo* pDataTypeInfo, TCHAR* pszFileName)
//{
//	NDataLoader	dataloader;
//	BOOL bLoadFile = FALSE;
//
//	bLoadFile = dataloader.LoadDataFromFile( pszFileName );
//
//	if (!bLoadFile)
//	{
//#ifdef _DEBUG
//		NTCHARString64 kMsg;
//		kMsg  = pszFileName;
//		kMsg += " 파일이 없습니다.";
//		MessageBox(NULL, kMsg, "ERROR", MB_OK);
//#endif
//		return FALSE;
//	}
//
//	dataloader.RegDataType( pDataTypeInfo );
//
//	while( !dataloader.IsEndOfData() )
//	{
//		if( dataloader.ReadData())
//		{
//			if(m_siItemUnique < 0)					continue;
//			if(m_siItemUnique >= MAX_ITEM_COUNT)	continue;
//
//			NItemGameData* pkNewData = new NItemGameData;
//			if(pkNewData)
//			{
//				pkNewData->m_siUnique	= m_siItemUnique;
//				StringCchCopy(pkNewData->m_szItemName, sizeof(pkNewData->m_szItemName), m_szItemName);
//
//				m_pkData[m_siItemUnique] = pkNewData;
//			}
//		}
//	}
//
//	return TRUE;
//}

SI32 NItemGameDataManager::GetItemUnique(TCHAR* pszItemName)
{
	if( pszItemName == NULL )		return 0;

	for(SI32 i=0; i<MAX_ITEM_COUNT; i++)
	{
		if( m_pkData[i] == NULL )		continue;

		if( _tcscmp(m_pkData[i]->m_szItemName, pszItemName) == 0 )
			return m_pkData[i]->m_siUnique;
	}

	return 0;
}