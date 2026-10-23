#include "NamingQuest.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../NLib/NDataLoader.h"
#include "../../Lib/EtcTool/EtcTool.h"

extern cltCommonLogic* pclClient;

void cltNamingQuestUnit::Set( TCHAR* questName, cltQuestClearConditionInfo* pclCond, TCHAR* naming, SI32 attack, SI32 defense, SI32 speed, SI32 skill )
{
	StringCchCopy( m_szQuestName, 64, questName );
	m_clClearCond.Set( pclCond );
	StringCchCopy( m_szNaming, 64, naming );
	m_siAttack = attack;
	m_siDefense = defense;
	m_siSpeed = speed;
	m_siSkill = skill;
}

cltQuestClearConditionInfo* cltNamingQuestUnit::GetQuestClerCond()
{
	return &m_clClearCond;
}

cltNamingQuest::cltNamingQuest()
{
}

cltNamingQuest::~cltNamingQuest()
{
}

void cltNamingQuest::LoadText( TCHAR* filename )
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromFile(filename);

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadNamingQuestFile()"), TEXT("LoadError:[%s]"), filename);
		return;
	}

	SI32	siIndex			= 0;
	TCHAR	szQuestName[64] = {0, };
	SI32	siFlag			= 0;
	TCHAR	szPara1[64]		= {0,};
	SI32	siPara2			= 0;
	TCHAR	szNaming[64]	= {0,};
	SI32	siAttack		= 0;
	SI32	siDefense		= 0;
	SI32	siSpeed			= 0;
	SI32	siSkill			= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,		4,
		NDATA_MBSTRING,	szQuestName,	64,
		NDATA_INT32,	&siFlag,		4,
		NDATA_MBSTRING,	&szPara1,		64,
		NDATA_INT32,	&siPara2,		4,
		NDATA_MBSTRING,	szNaming,		64,
		NDATA_INT32,	&siAttack,		4,
		NDATA_INT32,	&siDefense,		4,
		NDATA_INT32,	&siSpeed,		4,
		NDATA_INT32,	&siSkill,		4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	cltQuestClearConditionInfo clClearCond;
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			clClearCond.Init();

			switch ( m_siType )
			{
			case NAMINGQUEST_TYPE_HUNT:		
				clClearCond.siFlag = QUEST_CLEAR_COND_KILLMON;	
				clClearCond.siPara1 = pclClient->GetUniqueFromHash( szPara1 );
				clClearCond.siPara2 = siPara2;
#ifdef _DEBUG
				clClearCond.siPara2 = max( 1, siPara2 / 10 );
#endif
				break;
			case NAMINGQUEST_TYPE_COLLECT:
				{
					switch( siFlag )
					{
					case 1:	clClearCond.siFlag = QUEST_CLEAR_COND_FARM; break;
					case 2:	clClearCond.siFlag = QUEST_CLEAR_COND_MINE; break;
					case 3:	clClearCond.siFlag = QUEST_CLEAR_COND_FISH; break;
					default: clClearCond.siFlag = QUEST_CLEAR_COND_HUNT_ITEM; break;
					}
					SI32 siItemUnique = _ttoi( szPara1 );
					clClearCond.siPara1 = siItemUnique;
					clClearCond.siPara2 = siPara2;
#ifdef _DEBUG
					clClearCond.siPara2 = max( 1, siPara2 / 10 );
#endif
				}
				break;
			case NAMINGQUEST_TYPE_MOVE:
			case NAMINGQUEST_TYPE_GROWTH:
			default:
				break;
			}

			if ( siIndex < MAX_NAMINGQUEST_LEVEL )
			{
				m_clUnit[siIndex].Set( szQuestName, &clClearCond, szNaming, siAttack, siDefense, siSpeed, siSkill );
				m_siMaxLevel = siIndex;
			}
			else
			{
				MessageBox(NULL, TEXT("NamingQuest : Max Level Over"), TEXT("Error"), MB_OK|MB_TOPMOST);
			}
		}
	}
}

inline void cltNamingQuest::SetType( SI32 type )
{
	m_siType = type;
}

inline cltNamingQuestUnit* cltNamingQuest::GetQuestInfo( SI32 level )
{
	if ( level > 0 && level <= m_siMaxLevel )
	{
		return &m_clUnit[level];
	}
	else
	{
		return NULL;
	}
}

SI32 cltNamingQuest::GetType()
{
	return m_siType;
}

TCHAR* cltNamingQuest::GetTypeName()
{
	TCHAR *pText = {0,};
	switch( m_siType )
	{
	case 1:	pText = GetTxtFromMgr(10089);	break;
	case 2:	pText = GetTxtFromMgr(10090);	break;
	case 3:	pText = GetTxtFromMgr(10091);	break;
	case 4:	pText = GetTxtFromMgr(10092);	break;
	}
	return pText;
}

cltNamingQuestManager::cltNamingQuestManager()
{
}

cltNamingQuestManager::~cltNamingQuestManager()
{
}

void cltNamingQuestManager::LoadText( TCHAR* fileName )
{
	if ( pclClient->IsCountrySwitch( Switch_NamingQuest ) == false )
		return;

	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromFile(fileName);

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadNamingQuest()"), TEXT("LoadError:[%s]"), fileName);
		return;
	}

	SI32	siIndex = 0;
	TCHAR	szFileName[MAX_PATH] = {0,};
	SI32	siType = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,	4,		
		NDATA_MBSTRING,	szFileName,	MAX_PATH,
		NDATA_INT32,	&siType, 4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 index = 0;

	TCHAR szFileFullName[MAX_PATH] = {0,};

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			clNamingQuest[siIndex].SetType( siType );
			StringCchPrintf( szFileFullName, MAX_PATH, TEXT("Quest\\Naming\\%s.txt"), szFileName );
			clNamingQuest[siIndex].LoadText( szFileFullName );
		}
	}
}

cltNamingQuestUnit* cltNamingQuestManager::GetQuestInfo( SI32 index, SI32 level )
{
	if ( index >= 0 && index < MAX_NAMINGQUEST_INDEX )
	{
		return clNamingQuest[index].GetQuestInfo( level );
	}
	else
	{
		return NULL;
	}
}

TCHAR* cltNamingQuestManager::GetTypeName( SI32 index )
{
	if ( index >= 0 && index < MAX_NAMINGQUEST_INDEX )
	{
		return clNamingQuest[index].GetTypeName();
	}
	else
	{
		return NULL;
	}
}

SI32 cltNamingQuestManager::GetMaxLevel( SI32 index )
{
	if ( index >= 0 && index < MAX_NAMINGQUEST_INDEX )
	{
		return clNamingQuest[index].GetMaxLevel();
	}
	else
	{
		return 0;
	}
}

SI32 cltNamingQuestManager::GetType( SI32 index )
{
	if ( index >= 0 && index < MAX_NAMINGQUEST_INDEX )
	{
		return clNamingQuest[index].GetType();
	}
	else
	{
		return 0;
	}
}
