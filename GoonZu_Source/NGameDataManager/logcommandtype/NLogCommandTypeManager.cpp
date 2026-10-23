//=============================================================================
//
//=============================================================================

#include "NLogCommandTypeManager.h"
#include "../../NLib/NDataLoader.h"

NLogCommandTypeManager::NLogCommandTypeManager()
{
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

BOOL NLogCommandTypeManager::Init(TCHAR* pszLanaugae)
{
	return LoadLogCommandType(pszLanaugae);
}

BOOL NLogCommandTypeManager::LoadLogCommandType(TCHAR* pszLanaugae)
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\CommandType.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szLogCommand[50];
	TCHAR	szLogCommandEng[50];	
	SI32	siType;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szLogCommand,		sizeof(szLogCommand),
		NDATA_MBSTRING,	szLogCommandEng,	sizeof(szLogCommandEng),
		NDATA_INT32,	&siType,			sizeof(siType),
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	// 0번째는 빈 데이터
	NLogCommandType* pkNewData = NULL;
	
	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			if(siType < 0 || siType >= MAX_LOGCOMMANDTYPE_COUNT)		continue;

			pkNewData = new NLogCommandType;
			if(pkNewData)
			{
				if(pszLanaugae && (_tcscmp(pszLanaugae, "ENGLISH") == 0) )
				{
					pkNewData->Set(szLogCommandEng, siType);
				}
				else
				{
					pkNewData->Set(szLogCommand, siType);
				}
			}
			m_pkData[siType] = pkNewData;
		}
	}

	return TRUE;
}
