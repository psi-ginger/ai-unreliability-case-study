//=============================================================================
//
//=============================================================================
#ifndef _NGAMEDATAMANAGER_H_
#define _NGAMEDATAMANAGER_H_

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		{ if(p) { delete   (p);     (p)=NULL; } }
#endif

#include "LogServerInfo/NLogServerInfoManager.h"
#include "LogCommand/NLogCommandManager.h"
#include "LogCommandType/NLogCommandTypeManager.h"
#include "LogCommandIndex/NLogCommandIndexManager.h"
#include "LogCommandFilter/NLogCommandFilterManager.h"
#include "LogTextTrans/NLogTextTransManager.h"
#include "QuestTypeGameData/NQuestTypeGameDataManager.h"
#include "ItemGameData/NItemGameDataManager.h"
#include "LogDescription/NLogDescriptionManager.h"
#include "LogReasonDescription/NLogReasonDescriptionManager.h"
#include "CharGameData/NCharGameDataManager.h"
#include "VillageGameData/NVillageGameDataManager.h"

class NGameDataManager
{
protected:
	NLogServerInfoManager*		m_pkNLogServerInfoManager;
	NLogCommandManager*			m_pkNLogCommandManager;
	NLogCommandTypeManager*		m_pkNLogCommandTypeManager;
	NLogCommandIndexManager*	m_pkNLogCommandIndexManager;
	NLogCommandFilterManager*	m_pkNLogCommandFilterManager;
	NLogTextTransManager*		m_pkNLogTextTransManager;
	NQuestTypeGameDataManager*	m_pkNQuestTypeGameDataManager;
	NItemGameDataManager*		m_pkNItemGameDataManager;
	NLogDescriptionManager*		m_pkNLogDescriptionManager;
	NLogReasonDescriptionManager* m_pkNLogResaonDescriptionManager;
	NCharGameDataManager*		m_pkNCharGameDataManager;
	NVillageGameDataManager*	m_pkNVillageGameDataManager;
	NTextManager*				m_pTextManager;

public:
	NGameDataManager();
	~NGameDataManager();

	SI32		m_siServiceArea;

	BOOL		Init( TCHAR* pszLanguage);
	BOOL		InitGameData( SI32 siServiceArea);
	BOOL		Shutdown();

	SI32 GetServerInfoCount()												{	return m_pkNLogServerInfoManager->GetCount();								}
	NLogServerInfo* GetLogServerInfoAtIndex(SI32 siIndex)					{	return m_pkNLogServerInfoManager->GetAtIndex(siIndex);						}
	NLogServerInfo* FindLogServerInfo(TCHAR* pszServerName)					{	return m_pkNLogServerInfoManager->FindLogServerInfo(pszServerName);			}

	SI32 GetLogCommandCount()												{	return m_pkNLogCommandManager->GetCount();									}
	NLogCommand* GetLogCommandAtIndex(SI32 siIndex)							{	return m_pkNLogCommandManager->GetAtIndex(siIndex);							}

	SI32 GetLogCommandTypeMaxCount()										{	return m_pkNLogCommandTypeManager->GetMaxCount();							}
	NLogCommandType* GetLogCommandTypeAtIndex(SI32 siIndex)					{	return m_pkNLogCommandTypeManager->GetAtIndex(siIndex);						}
	NLogCommandType* GetLogCommandType(SI32 siType)							{	return m_pkNLogCommandTypeManager->GetType(siType);							}

	SI32 GetLogCommandIndexCount()											{	return m_pkNLogCommandIndexManager->GetCount();								}
	NLogCommandIndex* GetLogCommandIndexAtIndex(SI32 siIndex)				{	return m_pkNLogCommandIndexManager->GetAtIndex(siIndex);					}

	SI32 GetLogCommandFilterCount()											{	return m_pkNLogCommandFilterManager->GetCount();							}
	NLogCommandFilter* GetLogCommandFilterAtIndex(SI32 siIndex)				{	return m_pkNLogCommandFilterManager->GetAtIndex(siIndex);					}
	NLogCommandFilter* FindCommandFilterInfo(TCHAR* pszFilterName)			{	return m_pkNLogCommandFilterManager->FindLogCommandFilter(pszFilterName);	}

	SI32 GetLogTextCount()													{	return m_pkNLogTextTransManager->GetCount();								}
	NLogTextTrans* GetLogTextTransAtIndex(SI32 siIndex)						{	return m_pkNLogTextTransManager->GetAtIndex(siIndex);						}
	TCHAR* FindLogTextTransCommand(const TCHAR* pszCommand, const TCHAR* pszType, SI32 siIndex)	{	return m_pkNLogTextTransManager->FindLogTextTransCommand(pszCommand, pszType, siIndex);	}

	NItemGameData* GetItemData(SI32 siUnique)								{	return m_pkNItemGameDataManager->GetData(siUnique);							}
	TCHAR* GetItemName(SI32 siUnique)										{	return m_pkNItemGameDataManager->GetItemName(siUnique);						}
	SI32 GetItemUnique(TCHAR* pszItemName)									{	return m_pkNItemGameDataManager->GetItemUnique(pszItemName);				}

	SI32 GetLogDescriptionCount()											{	return m_pkNLogDescriptionManager->GetCount();								}
	NLogDescription* GetLogDescriptionAtIndex(SI32 siIndex)					{	return m_pkNLogDescriptionManager->GetAtIndex(siIndex);						}
	TCHAR* GetDescription(SI32 siType, SI32 siIndex)						{	return m_pkNLogDescriptionManager->GetDescription(siType, siIndex);			}

	SI32 GetLogReasonDescriptionCount()										{	return m_pkNLogResaonDescriptionManager->GetCount();						}
	NLogReasonDescription* GetLogReasonDescriptionAtIndex(SI32 siIndex)		{	return m_pkNLogResaonDescriptionManager->GetAtIndex(siIndex);				}
	TCHAR* GetReasonDescription(SI32 siType, SI32 siIndex, SI32 siReason)	{	return m_pkNLogResaonDescriptionManager->GetReasonDescription(siType, siIndex, siReason);	}

	TCHAR* GetCharName(SI32 siUnique)										{	return m_pkNCharGameDataManager->GetCharName(siUnique);						}

	TCHAR* GetVillageName(SI32 siVillageUnique)								{	return m_pkNVillageGameDataManager->GetVillageName(siVillageUnique);		}

	// 텍스트 매니저에서 텍스트를 얻어온다.
	TCHAR * GetTxtFromMgr(SI32 index , bool bStructure = false );
};

#endif
