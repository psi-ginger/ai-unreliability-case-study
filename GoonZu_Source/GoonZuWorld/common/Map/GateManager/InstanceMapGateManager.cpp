//-----------------------------------------------------------------------------
// 2005/09/26 이제완
//-----------------------------------------------------------------------------
#include <CommonLogic.h>
#include "InstanceMapGateManager.h"
#include "NDataloader.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------
// cltInstanceMapGate
//----------------------------------------------

cltInstanceMapGate::cltInstanceMapGate()
{
	Init();
}

cltInstanceMapGate::cltInstanceMapGate(SI32 unique, TCHAR* name, SI32 founique, SI32 srcmapindex, SI32 srcx, SI32 srcy, SI32 dstmapindex, SI32 dstx, SI32 dsty)
{
	siUnique		= unique;
	MStrCpy( szName, name, 100 );

	FOUnique		= founique;
	SrcMapIndex		= srcmapindex;
	SrcX			= srcx;
	SrcY			= srcy;
	DstMapIndex		= dstmapindex;
	DstX			= dstx;
	DstY			= dsty;
}

void cltInstanceMapGate::Init()
{
	ZeroMemory(this, sizeof(cltInstanceMapGate));
}

void cltInstanceMapGate::Set( cltInstanceMapGate* pclgate )
{
	if(pclgate == NULL)return ;

	siUnique		= pclgate->siUnique;
	MStrCpy( szName, pclgate->szName, 100 );

	FOUnique		= pclgate->FOUnique;
	SrcMapIndex		= pclgate->SrcMapIndex;
	SrcX			= pclgate->SrcX;
	SrcY			= pclgate->SrcY;
	DstMapIndex		= pclgate->DstMapIndex;
	DstX			= pclgate->DstX;
	DstY			= pclgate->DstY;
}

//----------------------------------------------
// cltInstanceMapGateManager
//----------------------------------------------
cltInstanceMapGateManager::cltInstanceMapGateManager()
{
	m_siMaxSetCount = 0;
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		ReadInstanceMapGateData(pclClient->siServiceArea);
	}
}

cltInstanceMapGateManager::~cltInstanceMapGateManager()
{
}

void cltInstanceMapGateManager::ReadInstanceMapGateData(SI32 ServiceArea)
{
	cltInstanceMapGateInfo aGateInfo;
	TCHAR	datfilename[MAX_PATH];

	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  //NHN중국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData/InstantMap/InstantMapGateInfoG.txt"));
	}

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;
	SI32		siSetNumber;

	bLoadFile = dataloader.LoadDataFromFile( datfilename );

	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("InstanceMapGateInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
	}
	
	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siSetNumber,			4,
		NDATA_INT32,	&aGateInfo.siUnique,	4,
		NDATA_INT32,	&aGateInfo.siTxtNum,	4,
		NDATA_INT32,	&aGateInfo.siFOUnique,	4,
		NDATA_INT32,	&aGateInfo.siSrcMapIndex,	4,
		NDATA_INT32,	&aGateInfo.siSrcMapX,	4,
		NDATA_INT32,	&aGateInfo.siSrcMapY,	4,
		NDATA_INT32,	&aGateInfo.siDstMapIndex,	4,
		NDATA_INT32,	&aGateInfo.siDstMapX,	4,
		NDATA_INT32,	&aGateInfo.siDstMapY,	4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if (dataloader.ReadData())
		{
			if (aGateInfo.siUnique != 0)
			{
				TCHAR * name = GetTxtFromMgr(aGateInfo.siTxtNum);
				if ( name == NULL )
				{
					continue ;

				}
				if( m_siMaxSetCount < siSetNumber )		m_siMaxSetCount = siSetNumber;

				siSetNumber--;

				cltInstanceMapGate clGate(  aGateInfo.siUnique + INSTANCEMAPGATE_START,
											name, 
											aGateInfo.siFOUnique, 
											aGateInfo.siSrcMapIndex,
											aGateInfo.siSrcMapX, 
											aGateInfo.siSrcMapY,
											aGateInfo.siDstMapIndex, 
											aGateInfo.siDstMapX, 
											aGateInfo.siDstMapY );

				Add(siSetNumber, &clGate);

				// 클론 맵을 가지고 있으면 게이트정보도 복사한다.
				if( pclClient->pclMapManager->HaveCloneMap(aGateInfo.siSrcMapIndex) &&
					pclClient->pclMapManager->HaveCloneMap(aGateInfo.siDstMapIndex) )
				{
					SI32 srccloneindex  = pclClient->pclMapManager->clUnitMapInfo[aGateInfo.siSrcMapIndex].siCloneIndex;
					SI32 destcloneindex = pclClient->pclMapManager->clUnitMapInfo[aGateInfo.siDstMapIndex].siCloneIndex;

					for ( SI32 i=0; i < CLONEMAP_NUM; ++i )
					{
						SI32 sMapIndex = pclClient->pclMapManager->clCloneMapInfo[srccloneindex].siDstMapIndex[i];
						SI32 dMapIndex = pclClient->pclMapManager->clCloneMapInfo[destcloneindex].siDstMapIndex[i];

						if ( sMapIndex > 0 && dMapIndex > 0)
						{
							cltInstanceMapGate clGate(  aGateInfo.siUnique + ((i + 1) * MAX_INSTANCEMAPGATE_NUMBER) + INSTANCEMAPGATE_START,
								name, 
								aGateInfo.siFOUnique, 
								sMapIndex,
								aGateInfo.siSrcMapX, 
								aGateInfo.siSrcMapY,
								dMapIndex, 
								aGateInfo.siDstMapX, 
								aGateInfo.siDstMapY );

							Add(siSetNumber, &clGate);
						}
					}
				}
			}
		}
	}
}



bool cltInstanceMapGateManager::Add(SI32 siSetNumber, cltInstanceMapGate* pclgate)
{
	if(pclgate== NULL)			return false;
	if(pclgate->FOUnique == 0)	return false;

	cltInstanceMapGate* pclInstanceGate = GetGatebyUnique(pclgate->siUnique, siSetNumber);

	if(pclInstanceGate == NULL)	return false;

	// 이미 같은 게이트가 존재하는지 확인한다. 
	if(pclInstanceGate->FOUnique)
	{
		pclInstanceGate->Set(pclgate);
		return false;
	}

	if(pclInstanceGate->FOUnique == 0)
	{
		pclInstanceGate->Set(pclgate);
		return true;
	}

	return false;
}

SI32 cltInstanceMapGateManager::GetInstanceMapGates( SI32 siMapIndex, SI32 siGateSetNumber, cltInstanceMapGate* pclGates, SI32 siMaxCount )
{
	if( siMapIndex <= 0 || siMapIndex >= MAX_MAP_NUMBER )							return 0;
	if( siGateSetNumber < 0 || siGateSetNumber >= MAX_INSTANCEMAPGATE_SETNUMBER )	return 0;

	SI32	cnt = 0;

	for( SI32 i=0; i<MAX_TOTAL_INSTANCEMAPGATE_NUMBER; i++ )
	{
		cltInstanceMapGate* pclGate = &clInstanceMapGate[i][siGateSetNumber];
		if( pclGate == NULL )						continue;
		if( pclGate->siUnique <= 0)					continue;
		if( pclGate->FOUnique <= 0)					continue;
		if( pclGate->SrcMapIndex != siMapIndex )	continue;

		if( cnt >= siMaxCount )					break;

		pclGates[cnt].Set(pclGate);
		cnt++;
	}

	return cnt;
}

cltInstanceMapGate* cltInstanceMapGateManager::GetGatebyUnique(SI32 siUnique, SI32 siSetNumber)
{
	SI32 siIndex = siUnique - INSTANCEMAPGATE_START;

	return GetGatebyIndex(siIndex, siSetNumber);
}

cltInstanceMapGate* cltInstanceMapGateManager::GetGatebyIndex(SI32 siIndex, SI32 siSetNumber)
{
	if( siIndex < 0 || siIndex >= MAX_TOTAL_INSTANCEMAPGATE_NUMBER)			return NULL;
	if( siSetNumber < 0 || siSetNumber >= MAX_INSTANCEMAPGATE_SETNUMBER)	return NULL;

	return &clInstanceMapGate[siIndex][siSetNumber];
}
