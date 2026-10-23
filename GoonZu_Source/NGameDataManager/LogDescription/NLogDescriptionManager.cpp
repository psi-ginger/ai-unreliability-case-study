//=============================================================================
//
//=============================================================================

#include "NLogDescriptionManager.h"
#include "../../NLib/NDataLoader.h"

NLogDescriptionManager::NLogDescriptionManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGDESCRIPTION_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogDescriptionManager::~NLogDescriptionManager()
{
	for(SI32 i=0; i<MAX_LOGDESCRIPTION_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogDescriptionManager::Init()
{
	return LoadLogDescription();
}

BOOL NLogDescriptionManager::LoadLogDescription()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\LogDescription.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szLogCommand[50];
	SI32	siIndex;
	SI32	siType;
	TCHAR	szDesc[2048];

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szLogCommand,		sizeof(szLogCommand),
		NDATA_INT32,	&siIndex,			sizeof(siIndex),
		NDATA_INT32,	&siType,			sizeof(siType),
		NDATA_MBSTRING,	&szDesc,			sizeof(szDesc),
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			NLogDescription* pkNewData = new NLogDescription;
			if(pkNewData)
			{
				StringCchCopy(pkNewData->m_szCommand, sizeof(pkNewData->m_szCommand), szLogCommand);
				pkNewData->m_siIndex		= siIndex;
				pkNewData->m_siType			= siType;
				StringCchCopy(pkNewData->m_szDescription, sizeof(pkNewData->m_szDescription), szDesc);
			}

			if(m_siCount >= MAX_LOGDESCRIPTION_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}
