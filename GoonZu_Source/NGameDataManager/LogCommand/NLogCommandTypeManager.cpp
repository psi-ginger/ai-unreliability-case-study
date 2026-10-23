//=============================================================================
//
//=============================================================================

#include "NLogCommandTypeManager.h"
#include "../../NLib/NDataLoader.h"

NLogCommandTypeManager::NLogCommandTypeManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGCOMMANDTYPE_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogCommandTypeManager::~NLogCommandTypeManager()
{
	for(SI32 i=0; i<MAX_LOGCOMMANDTYPE_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogCommandTypeManager::Init()
{
	return LoadLogCommandType();
}

BOOL NLogCommandTypeManager::LoadLogCommandType()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\CommandType.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szLogCommand[50];
	SI32	siType;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szLogCommand,		sizeof(szLogCommand),
		NDATA_INT32,	&siType,				sizeof(siType),
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	// 0번째는 빈 데이터
	NLogCommandType* pkNewData = new NLogCommandType;
	m_pkData[m_siCount] = pkNewData;
	m_siCount++;

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			pkNewData = new NLogCommandType;
			if(pkNewData)
			{
				pkNewData->Set(szLogCommand, siType);
			}

			if(m_siCount >= MAX_LOGCOMMANDTYPE_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}
