//=============================================================================
//
//=============================================================================

#include "NLogReasonDescriptionManager.h"
#include "../../NLib/NDataLoader.h"

NLogReasonDescriptionManager::NLogReasonDescriptionManager()
{
	m_siCount		= 0;
	for(SI32 i=0; i<MAX_LOGRESAONDESCRIPTION_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NLogReasonDescriptionManager::~NLogReasonDescriptionManager()
{
	for(SI32 i=0; i<MAX_LOGRESAONDESCRIPTION_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NLogReasonDescriptionManager::Init(TCHAR* pszLanaugae)
{
	return LoadLogReasonDescription(pszLanaugae);
}

BOOL NLogReasonDescriptionManager::LoadLogReasonDescription(TCHAR* pszLanaugae)
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( "LogData\\LogReasonDescription.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	TCHAR	szLogCommand[50];
	SI32	siIndex;
	SI32	siType;
	SI32	siReason;
	TCHAR	szDesc[2048];
	TCHAR	szDescEng[2048];

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szLogCommand,		sizeof(szLogCommand),
		NDATA_INT32,	&siIndex,			sizeof(siIndex),
		NDATA_INT32,	&siType,			sizeof(siType),
		NDATA_INT32,	&siReason,			sizeof(siReason),
		NDATA_MBSTRING,	&szDesc,			sizeof(szDesc),
		NDATA_MBSTRING,	&szDescEng,			sizeof(szDescEng),		
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			NLogReasonDescription* pkNewData = new NLogReasonDescription;
			if(pkNewData)
			{
				StringCchCopy(pkNewData->m_szCommand, sizeof(pkNewData->m_szCommand), szLogCommand);
				pkNewData->m_siIndex		= siIndex;
				pkNewData->m_siType			= siType;
				pkNewData->m_siReason		= siReason;
				// 설정된 언어에 따라 텍스트를 다르게 읽는다. - 기본은 한글
				if(pszLanaugae && (_tcscmp(pszLanaugae, "ENGLISH") == 0) )
				{
					StringCchCopy(pkNewData->m_szDescription, sizeof(pkNewData->m_szDescription), szDescEng);
				}
				else
				{
					StringCchCopy(pkNewData->m_szDescription, sizeof(pkNewData->m_szDescription), szDesc);
				}
				
			}

			if(m_siCount >= MAX_LOGRESAONDESCRIPTION_COUNT)		return FALSE;
			m_pkData[m_siCount] = pkNewData;
			m_siCount++;
		}
	}

	return TRUE;
}
