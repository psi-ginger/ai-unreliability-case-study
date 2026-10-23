//=============================================================================
//
//=============================================================================

#include "NLogTextTransManager.h"
#include "../../NLib/NDataLoader.h"

NLogTextTransManager::NLogTextTransManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGTEXT_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogTextTransManager::~NLogTextTransManager()
{
	for(SI32 i=0; i<MAX_LOGTEXT_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogTextTransManager::Init()
{
	return LoadLogTextTrans();
}

BOOL NLogTextTransManager::LoadLogTextTrans()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\LogTextTrans.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR		szCommand[128];
	TCHAR		szType[128];	
	SI32		siIndex;			
	TCHAR		szText[128];

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szCommand,		sizeof(szCommand),
		NDATA_MBSTRING,	szType,			sizeof(szType),
		NDATA_INT32,	&siIndex,		4,	
		NDATA_MBSTRING,	szText,			sizeof(szText),		
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			NLogTextTrans* pkNewData = new NLogTextTrans;
			if(pkNewData)
			{
				StringCchCopy(pkNewData->m_szCommand, sizeof(pkNewData->m_szCommand), szCommand);
				_tcslwr(pkNewData->m_szCommand);
				StringCchCopy(pkNewData->m_szType,	  sizeof(pkNewData->m_szType), szType);
				_tcslwr(pkNewData->m_szType);
				pkNewData->m_siIndex = siIndex;
				StringCchCopy(pkNewData->m_szText,	  sizeof(pkNewData->m_szText), szText);
			}

			if(m_siCount >= MAX_LOGTEXT_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}

TCHAR* NLogTextTransManager::FindLogTextTransCommand(const TCHAR* pszCommand, const TCHAR* pszType, SI32 siIndex)
{
	for(int i=0; i<m_siCount; i++)
	{
		if(m_pkData[i] != NULL)
		{
			if(strcmp(m_pkData[i]->m_szCommand, pszCommand) == 0)
			{
				if(strcmp(m_pkData[i]->m_szType, pszType) == 0 && (m_pkData[i]->m_siIndex == siIndex))
					return m_pkData[i]->m_szText;
			}
		}
	}

	return NULL;
}