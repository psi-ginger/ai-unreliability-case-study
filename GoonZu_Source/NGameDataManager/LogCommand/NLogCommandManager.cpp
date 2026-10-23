//=============================================================================
//
//=============================================================================

#include "NLogCommandManager.h"
#include "../../NLib/NDataLoader.h"

NLogCommandManager::NLogCommandManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGCOMMAND_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogCommandManager::~NLogCommandManager()
{
	for(SI32 i=0; i<MAX_LOGCOMMAND_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogCommandManager::Init()
{
	return LoadLogCommand();
}

BOOL NLogCommandManager::LoadLogCommand()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\Commands.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szLogCommand[50];

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szLogCommand,		sizeof(szLogCommand),
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{

			NLogCommand* pkNewData = new NLogCommand;
			if(pkNewData)
			{
				StringCchCopy(pkNewData->m_szCommand, sizeof(pkNewData->m_szCommand), szLogCommand);
			}

			if(m_siCount >= MAX_LOGCOMMAND_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}
