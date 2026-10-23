//-----------------------------------------------------------------------------
// 2005/09/26 이제완
//-----------------------------------------------------------------------------
#include <CommonLogic.h>
#include "GateManager.h"
#include "NDataloader.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------
// cltGate
//----------------------------------------------

cltGate::cltGate()
{
	Init();
}

cltGate::cltGate(SI32 unique, TCHAR* name, SI32 founique, SI32 itemunique, SI32 itemnum, SI32 srcmapindex, SI32 srcx, SI32 srcy, SI32 dstmapindex, SI32 dstx, SI32 dsty, bool PkAreaGate)
{
	siUnique		= unique;
	MStrCpy( szName, name, 100 );

	FOUnique		= founique;
	ItemUnique		= itemunique;
	ItemNum			= itemnum;
	SrcMapIndex		= srcmapindex;
	SrcX			= srcx;
	SrcY			= srcy;
	DstMapIndex		= dstmapindex;
	DstX			= dstx;
	DstY			= dsty;
	bPkAreaGate		= PkAreaGate;
}

void cltGate::Init()
{
	ZeroMemory(this, sizeof(cltGate));
}

void cltGate::Set( cltGate* pclgate )
{
	if(pclgate == NULL)return ;

	siUnique		= pclgate->siUnique;
	MStrCpy( szName, pclgate->szName, 100 );

	FOUnique		= pclgate->FOUnique;
	ItemUnique		= pclgate->ItemUnique;
	ItemNum			= pclgate->ItemNum;
	SrcMapIndex		= pclgate->SrcMapIndex;
	SrcX			= pclgate->SrcX;
	SrcY			= pclgate->SrcY;
	DstMapIndex		= pclgate->DstMapIndex;
	DstX			= pclgate->DstX;
	DstY			= pclgate->DstY;
	bPkAreaGate		= pclgate->bPkAreaGate;
}

//----------------------------------------------
// cltGateManager
//----------------------------------------------
cltGateManager::cltGateManager()
{
	ReadGateData(pclClient->siServiceArea);
}

cltGateManager::~cltGateManager()
{
}

void cltGateManager::ReadGateData(SI32 ServiceArea)
{
	cltGateInfo aGateInfo;
/*
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	switch(ServiceArea)
	{
	case ConstServiceArea_Korea :
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfoK.txt") );
		break;
	case ConstServiceArea_China :
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfoC.txt") );
		break;
	case ConstServiceArea_Japan : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfoJ.txt") );
		break;
	case ConstServiceArea_English : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfoE.txt") );
		break;
	case ConstServiceArea_USA : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfoU.txt") );
		break;
	case ConstServiceArea_Taiwan : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfoT.txt") );
		break;
	default:
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GateInfo.txt") );
		break;
	}
*/
	TCHAR	datfilename[30];
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  // NHN중국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/GateInfoG.txt"));
	}

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datfilename );
	}



	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("GateInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
	}
	
	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&aGateInfo.siUnique,	4,
			NDATA_INT32,	&aGateInfo.siTxtNum,	4,
			NDATA_INT32,	&aGateInfo.siFOUnique,	4,
			NDATA_INT32,	&aGateInfo.siItemUnique,4,
			NDATA_INT32,	&aGateInfo.siItemNum,	4,
			NDATA_INT32,	&aGateInfo.siSrcMapIndex,	4,
			NDATA_INT32,	&aGateInfo.siSrcMapX,	4,
			NDATA_INT32,	&aGateInfo.siSrcMapY,	4,
			NDATA_INT32,	&aGateInfo.siDstMapIndex,	4,
			NDATA_INT32,	&aGateInfo.siDstMapX,	4,
			NDATA_INT32,	&aGateInfo.siDstMapY,	4,
			NDATA_INT32,	&aGateInfo.siPkAreaGate,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 cloneMapStartIndex = 700;

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

				cltGate clGate(aGateInfo.siUnique,
					name, 
					aGateInfo.siFOUnique, 
					aGateInfo.siItemUnique,
					aGateInfo.siItemNum,
					aGateInfo.siSrcMapIndex,
					aGateInfo.siSrcMapX, 
					aGateInfo.siSrcMapY,
					aGateInfo.siDstMapIndex, 
					aGateInfo.siDstMapX, 
					aGateInfo.siDstMapY,
					DATA_TO_BOOL(aGateInfo.siPkAreaGate) );

				Add(&clGate);
			}
		}
	}
}



bool cltGateManager::Add(cltGate* pclgate)
{

	if(pclgate== NULL)			return false;
	if(pclgate->FOUnique == 0)	return false;

	if(pclgate->DstMapIndex == 40)
	{
		SI32 a = 0;
	}
	// 이미 같은 게이트가 존재하는지 확인한다. 
	if(clGate[pclgate->siUnique].FOUnique)
	{
		clGate[pclgate->siUnique].Set(pclgate);
		return false;
	}

	if(clGate[pclgate->siUnique].FOUnique == 0)
	{
		clGate[pclgate->siUnique].Set(pclgate);
		return true;
	}

	return false;
}

bool cltGateManager::FindGate(SI32 index, cltGate* pclgate)
{
	SI32 i;

	pclgate->Init();

	for(i = index;i < MAX_GATE_NUMBER;i++)
	{
		if(clGate[i].FOUnique)
		{
			pclgate->Set(&clGate[i]);

			return true;
		}
	}

	return false;
}

bool cltGateManager::FindGate( SI32 srcMapIndex, SI32 dstMapIndex, cltGate* pclgate )
{
    pclgate->Init();

	for( SI32 i = 0 ; i < MAX_GATE_NUMBER ; i++ )
	{
		if( clGate[i].DstMapIndex == dstMapIndex && clGate[i].SrcMapIndex == srcMapIndex )
		{
			pclgate->Set(&clGate[i]);

			return true;
		}
	}
	return false;
}

// 특정 지도의 대표 좌표를 구한다. 
bool cltGateManager::FindImportantPos(SI32 mapindex, SI32* px, SI32* py)
{
	SI32 index;

	for(index = 0;index < MAX_GATE_NUMBER;index++)
	{
		if(clGate[index].DstMapIndex == mapindex)
		{
			*px = clGate[index].DstX;
			*py = clGate[index].DstY;
			return true;
		}
	}


	return false;

}

// 특정 지도의 야외지역 좌표를 구한다. 
bool cltGateManager::FindMainFieldPos(SI32 mapindex, SI32* px, SI32* py)
{
	SI32 index;

	for(index = 0;index < MAX_GATE_NUMBER;index++)
	{
		if(clGate[index].DstMapIndex == mapindex)
		{
			*px = clGate[index].SrcX;
			*py = clGate[index].SrcY;
			return true;
		}
	}

	return false;
}

bool cltGateManager::IsPkAreaGate(SI32 GateUnique)
{
	SI32 index;

	for(index = 0;index < MAX_GATE_NUMBER;++index)
	{
		if(clGate[index].siUnique == GateUnique)
		{
			return clGate[index].bPkAreaGate;
		}
	}

	return false;
}

