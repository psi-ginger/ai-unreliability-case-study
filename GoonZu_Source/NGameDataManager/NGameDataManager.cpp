//=============================================================================
//
//=============================================================================
#include "NGameDataManager.h"
#include "./../GoonZuWorld/common/CommonHeader.h"

NGameDataManager::NGameDataManager()
{
	m_siServiceArea					= 0;

	m_pkNLogServerInfoManager		= NULL;
	m_pkNLogCommandManager			= NULL;
	m_pkNLogCommandTypeManager		= NULL;
	m_pkNLogCommandIndexManager		= NULL;
	m_pkNLogCommandFilterManager	= NULL;
	m_pkNLogTextTransManager		= NULL;
	m_pkNQuestTypeGameDataManager	= NULL;
	m_pkNItemGameDataManager		= NULL;
}

NGameDataManager::~NGameDataManager()
{
	Shutdown();
}

BOOL NGameDataManager::Init( TCHAR* pszLanguage)
{
	m_pkNLogServerInfoManager = new NLogServerInfoManager;
	m_pkNLogServerInfoManager->Init();

	m_pkNLogCommandManager = new NLogCommandManager;
	m_pkNLogCommandManager->Init();

	m_pkNLogCommandTypeManager = new NLogCommandTypeManager;
	m_pkNLogCommandTypeManager->Init(pszLanguage);

	m_pkNLogCommandIndexManager = new NLogCommandIndexManager;
	m_pkNLogCommandIndexManager->Init(pszLanguage);

	m_pkNLogDescriptionManager = new NLogDescriptionManager;
	m_pkNLogDescriptionManager->Init();

	m_pkNLogResaonDescriptionManager = new NLogReasonDescriptionManager;
	m_pkNLogResaonDescriptionManager->Init(pszLanguage);

	// 타입에 인덱스 위치 저장
	for(int i=0; i<m_pkNLogCommandIndexManager->GetCount(); i++)
	{
		NLogCommandIndex* pCommandIndex = m_pkNLogCommandIndexManager->GetAtIndex(i);
		if(pCommandIndex && pCommandIndex->m_siType > 0)
		{
			NLogCommandType* pCommandType = m_pkNLogCommandTypeManager->GetAtIndex(pCommandIndex->m_siType);
			if(pCommandType)
			{
				if(pCommandType->m_siIndexStart < 0)
					pCommandType->m_siIndexStart = i;
			}
		}
	}
	m_pkNLogCommandFilterManager = new NLogCommandFilterManager;
	m_pkNLogCommandFilterManager->Init();

	m_pkNLogTextTransManager = new NLogTextTransManager;
	m_pkNLogTextTransManager->Init();

	return TRUE;
}

BOOL NGameDataManager::InitGameData(SI32 siServiceArea)
{
	m_pTextManager = new NTextManager;
	m_pTextManager->CreateTextManager();

	TCHAR szTextMgrFileName[MAX_PATH] = TEXT("");

	switch ( siServiceArea )
	{
	case ConstServiceArea_Korea :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_KOR.ntx") );
		}
		break;
	case ConstServiceArea_Japan :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_JPN.ntx") );
		}
		break;
	case ConstServiceArea_China :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_CHI.ntx") );
		}
		break;
	case ConstServiceArea_English :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_ENG.ntx") );
		}
		break;
	case ConstServiceArea_Taiwan :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_TAI.ntx") );
		}
		break;
	case ConstServiceArea_USA :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_ENG.ntx") );
		}
		break;
	case ConstServiceArea_NHNChina :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_CHI.ntx") );
		}
		break;
	case ConstServiceArea_EUROPE :
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_EUR.ntx") );
		}
		break;
	default:
		{
			StringCchCopy( szTextMgrFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Text_ENG.ntx") );
		}
	}

	m_pTextManager->LoadTextFromCryptFile( szTextMgrFileName, 1 );

	m_pkNQuestTypeGameDataManager = new NQuestTypeGameDataManager;
	m_pkNQuestTypeGameDataManager->Init();

	m_pkNItemGameDataManager = new NItemGameDataManager;
	m_pkNItemGameDataManager->Init(siServiceArea);

	m_pkNCharGameDataManager = new NCharGameDataManager;
	m_pkNCharGameDataManager->Init(siServiceArea, m_pTextManager);

	m_pkNVillageGameDataManager = new NVillageGameDataManager;
	m_pkNVillageGameDataManager->Init(siServiceArea);

	return TRUE;
}

BOOL NGameDataManager::Shutdown()
{
	SAFE_DELETE(m_pkNLogServerInfoManager);
	SAFE_DELETE(m_pkNLogCommandManager);
	SAFE_DELETE(m_pkNLogCommandTypeManager);
	SAFE_DELETE(m_pkNLogCommandIndexManager);
	SAFE_DELETE(m_pkNLogCommandFilterManager);
	SAFE_DELETE(m_pkNLogDescriptionManager);
	SAFE_DELETE(m_pkNLogTextTransManager);
	SAFE_DELETE(m_pkNLogResaonDescriptionManager);

	SAFE_DELETE(m_pkNQuestTypeGameDataManager);
	SAFE_DELETE(m_pkNItemGameDataManager);
	SAFE_DELETE(m_pkNCharGameDataManager);
	SAFE_DELETE(m_pkNVillageGameDataManager);

	return TRUE;
}

// 텍스트 매니저에서 텍스트를 얻어온다
TCHAR * NGameDataManager::GetTxtFromMgr(SI32 index , bool bStructure )
{
	if( NULL == m_pTextManager)
		return NULL;

	TCHAR * buffer = NULL ;
	TCHAR strIndex[MAX_TEXT_KEY] = TEXT("");
	//	TCHAR szNULL[8] = TEXT("");

	if ( bStructure )
	{
		StringCchPrintf(strIndex, MAX_TEXT_KEY, TEXT("%d"), index+15000);//15000번대 이상의 텍스트는 인덱스+15000

		buffer = m_pTextManager->GetText(strIndex);
		if (!buffer)
		{
			return TEXT("NULL");
		}

	}
	else
	{
		StringCchPrintf(strIndex, MAX_TEXT_KEY, TEXT("%d"), index);

		buffer = m_pTextManager->GetText(strIndex);
		if (!buffer)
		{
			return TEXT("NULL");
		}

	}

	return buffer ;
}
