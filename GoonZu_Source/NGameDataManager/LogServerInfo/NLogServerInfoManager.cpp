//=============================================================================
//
//=============================================================================

#include "NLogServerInfoManager.h"
#include "../../NLib/NDataLoader.h"

NLogServerInfoManager::NLogServerInfoManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_SERVER_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogServerInfoManager::~NLogServerInfoManager()
{
	for(SI32 i=0; i<MAX_SERVER_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogServerInfoManager::Init()
{
	return LoadServerInfo();
}

BOOL NLogServerInfoManager::LoadServerInfo()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\ServerInfo.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szServerName[64];
	TCHAR	szTableName[64];	
	TCHAR	szIP[24];
	SI32	siPort;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szServerName,		sizeof(szServerName),
		NDATA_MBSTRING,	szTableName,		sizeof(szTableName),
		NDATA_MBSTRING,	szIP,				sizeof(szIP),		
		NDATA_INT32,	&siPort,			4,		
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			NLogServerInfo* pkNewData = new NLogServerInfo;
			if(pkNewData)
			{
				StringCchCopy(pkNewData->m_szSeverName, sizeof(pkNewData->m_szSeverName), szServerName);
				StringCchCopy(pkNewData->m_szTableName, sizeof(pkNewData->m_szTableName), szTableName);
				StringCchCopy(pkNewData->m_szIP,		sizeof(pkNewData->m_szIP), szIP);
				pkNewData->m_uiPort		= siPort;
			}

			if(m_siCount >= MAX_SERVER_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}

NLogServerInfo* NLogServerInfoManager::FindLogServerInfo(TCHAR* pszServerName)
{
	for(int i=0; i<m_siCount; i++)
	{
		if(m_pkData[i] != NULL)
		{
			if(strcmp(m_pkData[i]->m_szSeverName, pszServerName) == 0)
				return m_pkData[i];
		}
	}
	return NULL;
}