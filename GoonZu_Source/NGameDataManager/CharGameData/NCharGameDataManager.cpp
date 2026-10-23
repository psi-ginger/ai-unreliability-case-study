//=============================================================================
//
//=============================================================================

#include "NCharGameDataManager.h"
#include "../../NLib/NDataLoader.h"
#include "../../NLib/NTextManager.h"
#include "../../GoonZuWorld/common/CommonHeader.h"


NCharGameDataManager::NCharGameDataManager()
{
	for(SI32 i=0; i<MAX_CHAR_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NCharGameDataManager::~NCharGameDataManager()
{
	for(SI32 i=0; i<MAX_CHAR_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NCharGameDataManager::Init(SI32 siServiceArea, NTextManager* pTextManager)
{
	return LoadCharGameData(siServiceArea, pTextManager);
}

BOOL NCharGameDataManager::LoadCharGameData( SI32 siServiceArea, NTextManager* pTextManager)
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//g_pTextManager.LoadTextFromCryptFile( "TextMgr\\TextMgr.txt" , 1 );

	bLoadFile = dataloader.LoadDataFromCryptFile( "Data\\KindInfo_Unique.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	NCharKindGameData	CharKindGameData[1000];
	ZeroMemory(CharKindGameData, sizeof(CharKindGameData));

	SI32	siUnqiue = 0;
	TCHAR	szKindType[256];

	NDataTypeInfo pDataTypeInfo1[] =
	{
		NDATA_INT32,	&siUnqiue,			sizeof(siUnqiue),
		NDATA_MBSTRING,	szKindType,			sizeof(szKindType),
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo1 );

	SI32	siCount = 0;
	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			CharKindGameData[siCount].m_siUnique = siUnqiue;
			StringCchCopy(CharKindGameData[siCount].m_szKind, sizeof(CharKindGameData[siCount].m_szKind), szKindType);
			siCount++;
		}
	}

	bLoadFile = dataloader.LoadDataFromCryptFile( "Data\\KindInfo_Create.txt" );

	if (!bLoadFile)
	{
		return FALSE;
	}

	//TCHAR	szKindType[256];
	TCHAR	szCode[256];
	SI32	siSortNum = 0;
	SI32	siMoveSpeed = 0;
	SI32	siSize = 0;
	TCHAR	szBlock[256];
	TCHAR	szATB[256];
	TCHAR	szServiceArea[512];
	TCHAR	szDefence[512];
	SI32	siVibrate = 0;
    SI32	siName = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szKindType,			sizeof(szKindType),
		NDATA_MBSTRING,	szCode,				sizeof(szCode),
		NDATA_INT32,	&siSortNum,			sizeof(siSortNum),
		NDATA_INT32,	&siMoveSpeed,		sizeof(siMoveSpeed),
		NDATA_INT32,	&siSize,			sizeof(siSize),
		NDATA_MBSTRING,	szBlock,			sizeof(szBlock),
		NDATA_MBSTRING,	szATB,				sizeof(szATB),
		NDATA_MBSTRING,	szServiceArea,		sizeof(szServiceArea),
		NDATA_MBSTRING,	szDefence,			sizeof(szDefence),
		NDATA_INT32,	&siVibrate,			sizeof(siVibrate),
		NDATA_INT32,	&siName,			sizeof(siName),
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	TCHAR	szTmp[32];
	SI32 siUnique = 0;
	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			NCharGameData* pkNewData = new NCharGameData;
			if(pkNewData)
			{
				siUnique = 0;
				for(SI32 i=0; i<1000; i++)
				{
					if( CharKindGameData[i].m_siUnique <= 0 )		continue;
					if( _tcscmp(CharKindGameData[i].m_szKind, szKindType) == 0 )
					{
						siUnique = CharKindGameData[i].m_siUnique;
						break;
					}
				}
				wsprintf(szTmp, "%d", siName);
				pkNewData->m_siUnique	= siUnique;
				TCHAR* pszName = pTextManager->GetText(szTmp);
				if(pszName == NULL)
					continue;
				StringCchCopy(pkNewData->m_szCharName, sizeof(pkNewData->m_szCharName), pszName);
				StringCchCopy(pkNewData->m_szKind, sizeof(pkNewData->m_szKind), szKindType);
			}

			if(siUnique >= 0 && siUnique < MAX_CHAR_COUNT)
			{
				m_pkData[siUnique] = pkNewData;
			}
		}
	}

	return TRUE;
}
