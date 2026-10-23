//=============================================================================
//
//=============================================================================

#include "NVillageGameDataManager.h"
#include "../../NLib/NDataLoader.h"
#include "../../NLib/NTextManager.h"
#include "../../GoonZuWorld/common/CommonHeader.h"


NVillageGameDataManager::NVillageGameDataManager()
{
	for(SI32 i=0; i<MAX_VILLAGE_COUNT; i++)
	{
		m_pkData[i] = NULL;
	}
}

NVillageGameDataManager::~NVillageGameDataManager()
{
	for(SI32 i=0; i<MAX_VILLAGE_COUNT; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NVillageGameDataManager::Init(SI32 siServiceArea)
{
	return LoadVillageGameData(siServiceArea);
}

BOOL NVillageGameDataManager::LoadVillageGameData(SI32 siServiceArea)
{
	TCHAR	datfilename[25];

	if( siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoK.txt"));
	}
	else if( siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoJ.txt"));
	}
	else if( siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoC.txt"));
	}
	else if( siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoE.txt"));
	}
	else if( siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoT.txt"));
	}
	else if( siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoU.txt"));
	}
	else if( siServiceArea == ConstServiceArea_EUROPE )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/villageinfoG.txt"));
	}
	else if( siServiceArea == ConstServiceArea_NHNChina )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/villageinfoC.txt"));
	}

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		

	if (!bLoadFile)		{		return FALSE;		}

	TCHAR province[64] = TEXT(""), townname[64] = TEXT("");
	SI32 vunique = 0, posx = 0, posy = 0, mapinx = 0, season = 0, farm = 0, iron = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&vunique,4,		
		NDATA_MBSTRING,	province,64,
		NDATA_MBSTRING,	townname,64,
		NDATA_INT32,	&posx, 4,
		NDATA_INT32,	&posy, 4,
		NDATA_INT32,	&mapinx, 4,
		NDATA_INT32,	&season, 4,
		NDATA_INT32,	&farm, 4,
		NDATA_INT32,	&iron,	4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() ) 
		{
			if(vunique < 0)						continue;
			if(vunique >= MAX_VILLAGE_COUNT)	continue;

			NVillageGameData* pkNewData = new NVillageGameData;
			if(pkNewData)
			{
				pkNewData->m_siVillageUnique	= vunique;
				StringCchCopy(pkNewData->m_szName, sizeof(pkNewData->m_szName), townname);

				m_pkData[vunique] = pkNewData;
			}
		}
	}

	return TRUE;
}
