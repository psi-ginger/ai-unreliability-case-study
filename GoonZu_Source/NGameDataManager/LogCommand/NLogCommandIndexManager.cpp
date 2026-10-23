//=============================================================================
//
//=============================================================================

#include "NLogCommandIndexManager.h"
#include "../../NLib/NDataLoader.h"

NLogCommandIndexManager::NLogCommandIndexManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGCOMMANDINDEX_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogCommandIndexManager::~NLogCommandIndexManager()
{
	for(SI32 i=0; i<MAX_LOGCOMMANDINDEX_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogCommandIndexManager::Init()
{
	return LoadLogCommandIndex();
}

BOOL NLogCommandIndexManager::LoadLogCommandIndex()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\CommandIndex.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szLogCommand[50];
	SI32	siIndex;
	SI32	siType;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szLogCommand,		sizeof(szLogCommand),
		NDATA_INT32,	&siIndex,			sizeof(siIndex),
		NDATA_INT32,	&siType,				sizeof(siType),
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{

			NLogCommandIndex* pkNewData = new NLogCommandIndex;
			if(pkNewData)
			{
				StringCchCopy(pkNewData->m_szCommand, sizeof(pkNewData->m_szCommand), szLogCommand);
				pkNewData->m_siIndex		= siIndex;
				pkNewData->m_siType			= siType;
			}

			if(m_siCount >= MAX_LOGCOMMANDINDEX_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}
