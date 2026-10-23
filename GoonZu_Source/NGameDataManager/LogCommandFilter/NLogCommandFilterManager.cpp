//=============================================================================
//
//=============================================================================

#include "NLogCommandFilterManager.h"
#include "../../NLib/NDataLoader.h"

NLogCommandFilterManager::NLogCommandFilterManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGCOMMANDFILTER_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogCommandFilterManager::~NLogCommandFilterManager()
{
	for(SI32 i=0; i<MAX_LOGCOMMANDFILTER_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogCommandFilterManager::Init()
{
	return LoadLogCommandFilter();
}

BOOL NLogCommandFilterManager::LoadLogCommandFilter()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\CommandFilters.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szFilterName[64];
	TCHAR	szFilter[512];

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szFilterName,		sizeof(szFilterName),
		NDATA_MBSTRING,	szFilter,			sizeof(szFilter),
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			char*	pszFilter = szFilter;
			NLogCommandFilter* pkNewData = new NLogCommandFilter;
			if(pkNewData)
			{
				if(szFilter[0] == '\"')						pszFilter = &szFilter[1];
				if(szFilter[strlen(szFilter)-1] == '\"')	szFilter[strlen(szFilter)-1] = '\0';

				StringCchCopy(pkNewData->m_szFilterName, sizeof(pkNewData->m_szFilterName), szFilterName);
				StringCchCopy(pkNewData->m_szFilter, sizeof(pkNewData->m_szFilter), pszFilter);
			}

			if(m_siCount >= MAX_LOGCOMMANDFILTER_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}

NLogCommandFilter* NLogCommandFilterManager::FindLogCommandFilter(TCHAR* pszFilterName)
{
	for(int i=0; i<m_siCount; i++)
	{
		if(m_pkData[i] != NULL)
		{
			if(strcmp(m_pkData[i]->m_szFilterName, pszFilterName) == 0)
				return m_pkData[i];
		}
	}
	return NULL;
}
