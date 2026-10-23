//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "TileSet.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "CommonLogic.h"
#include "NDataloader.h"
#include "NHashTableString.h"


extern cltCommonLogic* pclClient;

cltTileInfo::cltTileInfo(SHORT unique, DWORD dwatb, UI08 color, DWORD dwimagefilenumber, cltTileInfo* pparent, TCHAR* szname)
{
	SI32 i;

	if(unique > 255)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Not Proper TileSet Unique. Over 256"), TEXT("Error"), MB_OK|MB_TOPMOST);
		Unique	= 0;
	}
	else
	{
		Unique	= unique;
	}
	
	dwAtb	= dwatb;
	uiColor	= color;
	
	pParent	= pparent;

	if(pParent)
	{
		pParent->AddChild(this);
	}
	
	StringCchCopy(szName, MAX_PATH, szname);

	dwChildNumber	= 0;
	
	if(dwimagefilenumber <= MAX_TILE_PER_TILESET)
	{
		TileImageFileNumber		= dwimagefilenumber;
	}
	else
	{
		TileImageFileNumber		= 0;
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Not Proper TileImageFile Number."), TEXT("Error"), MB_OK|MB_TOPMOST);
	}

	for(i = 0;i < MAX_TILE_PER_TILESET;i++)
	{
		pSpr[i] = NULL;
	}

//	pXSpr = NULL;

	pTileSetArrayInfo	= NULL;

	// 파일에서 배열 정보를 읽어온다. 
	TCHAR szfilename[128];
	StringCchPrintf(szfilename, 128, TEXT("Tile\\%s\\%s_Array.txt"), szName, szName);
	
	ReadTileSetArrayInfo(szfilename);


}

cltTileInfo::~cltTileInfo()
{
	SI32 i;
/*
	if( pXSpr ) {
		delete pXSpr;
		pXSpr = NULL;
	}
*/
	for( i = 0;i < MAX_TILE_PER_TILESET;i++)
	{
		if(pSpr[i])
		{
			GP.FreeSpr(*pSpr[i]);
			pSpr[i]->pImage = NULL;
			delete pSpr[i];
			pSpr[i] = NULL;

		}
	}

	if(pTileSetArrayInfo )
	{
		delete[] pTileSetArrayInfo ;
		pTileSetArrayInfo  = NULL;
	}

}

// 보유한 모든 타일 세트를 메모리 해제한다. 
SI32 cltTileInfo::FreeAllSpr()
{
	SI32 i;
	SI32 size = 0;
/*
	if( pXSpr ) {
		delete pXSpr;
		pXSpr = NULL;
	}
*/
	for(i = 0;i < MAX_TILE_PER_TILESET;i++)
	{
		if(pSpr[i])
		{
			size += pSpr[i]->GetImageDataSize();
			GP.FreeSpr(*pSpr[i]);

			delete pSpr[i];
			pSpr[i] = NULL;

		}
	}

	return size;
}

// 타일 배열 정보를 읽어온다. 
BOOL cltTileInfo::ReadTileSetArrayInfo(TCHAR* szfilename)
{

	SI32 i;
	SI32 varynumber = 0;
	SI32 index = 0;
	TCHAR data[30];
	FILE* fp;
	TCHAR buffer[1024];

	fp = _tfopen(szfilename, TEXT("rt"));
	if(fp == NULL)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)

#ifdef _DEBUG
			MessageBox(NULL, szfilename, TEXT("File Read Error"), MB_OK|MB_TOPMOST);
#endif
		return FALSE;
	}

	// 첫번째 1줄은 무시한다. 
	_fgetts(buffer, 1024, fp);

	// 변종의 숫자를 파악한다. 
	_fgetts(buffer, 1024, fp);
	_stscanf(buffer, TEXT("%d"), &varynumber);

	// pTileSetArrayInfo를 적절한 크기로 확보한다. 
	pTileSetArrayInfo = new BYTE[1 + varynumber * 2 * 15]; // File, Font 정보.
	pTileSetArrayInfo[index] = (BYTE)varynumber;
	index++;

	while( _fgetts(buffer, 1024, fp) != NULL)
	{

		switch(varynumber)
		{
		case 1:
			_stscanf(buffer, TEXT("%d %d"), &data[0], &data[1]);
			break;
		case 2:
			_stscanf(buffer, TEXT("%d %d %d %d"), &data[0], &data[1], &data[2], &data[3]);
			break;
		case 3:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);
			break;
		case 4:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]);
			break;
		case 5:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], &data[8], &data[9] );
			break;
		case 6:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], &data[8], &data[9], &data[10], &data[11]);
			break;
		case 7:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], &data[8], &data[9], &data[10], &data[11], &data[12], &data[13]);
			break;
		case 8:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], &data[8], &data[9], &data[10], &data[11], &data[12], &data[13], &data[14], &data[15]);
			break;
		case 9:
			_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"), &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], &data[8], &data[9], &data[10], &data[11], &data[12], &data[13], &data[14], &data[15], &data[16], &data[17]);
			break;
		default:
			_stscanf(buffer, TEXT("%d %d"), &data[0], &data[1]);
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox(NULL, TEXT("fdsf93kd"), TEXT("Fd08fff"), MB_OK|MB_TOPMOST);
			break;
		}
		

		for(i = 0;i < (varynumber * 2);i++)
		{
			pTileSetArrayInfo[index] = data[i];
			index++;
		}
		
	}

	fclose(fp);

	return TRUE;

}

// 자식 타일 세트를 추가한다. 
BOOL cltTileInfo::AddChild(cltTileInfo* pchild)
{
	if(dwChildNumber >= MAX_CHILD_TILE_SET_NUMBER)return FALSE;

	pChild[dwChildNumber] = pchild;
	
	dwChildNumber++;

	return TRUE;

}

//-----------------------------------------------------------------------------------------

cltTileManager::cltTileManager(const TCHAR* szsavepath)
{
	SI32 i;

	for(i = 0;i < MAX_TILE_UNIQUE_NUMBER;i++)
	{
		siTileRef[i] = -1;
	}

	siLoadedTileSetImageSize	= 0;

	for( i = 0; i < MAX_TILESET_NUMBER;i++)
	{
		pclTileInfo[i] = NULL;
	}

	// 파일을 읽어들일 경로를 확보한다.
	if(szsavepath)
		StringCchCopy(szSavePath, MAX_PATH, szsavepath);
	else
		StringCchCopy(szSavePath, MAX_PATH, TEXT(""));

	InitTileSet();
}



cltTileManager::~cltTileManager()
{
	DeleteTileSet();
}


BOOL cltTileManager::InitTileSet()
{
	BOOL bLoadFile = FALSE;

	NDataAtbParser  atbparser;

	// TILEATB 등록
	atbparser.AddAtbInfo( TEXT("TILEATB_BASE"), TILEATB_BASE);
	atbparser.AddAtbInfo( TEXT("TILEATB_SUB"), TILEATB_SUB);
	atbparser.AddAtbInfo( TEXT("TILEATB_SUB"), TILEATB_SUB);
	atbparser.AddAtbInfo( TEXT("TILEATB_8DIR"), TILEATB_8DIR);
	atbparser.AddAtbInfo( TEXT("TILEATB_NODIR"), TILEATB_NODIR);
	atbparser.AddAtbInfo( TEXT("TILEATB_LAYER1"), TILEATB_LAYER1);
	atbparser.AddAtbInfo( TEXT("TILEATB_LAYER2"), TILEATB_LAYER2);
	atbparser.AddAtbInfo( TEXT("TILEATB_LAYER2"), TILEATB_LAYER2);
	atbparser.AddAtbInfo( TEXT("TILEATB_1X"), TILEATB_1X);
	atbparser.AddAtbInfo( TEXT("TILEATB_2X"), TILEATB_2X);
	atbparser.AddAtbInfo( TEXT("TILEATB_4X"), TILEATB_4X);
	atbparser.AddAtbInfo( TEXT("TILEATB_BLOCK"), TILEATB_BLOCK);
	atbparser.AddAtbInfo( TEXT("TILEATB_WATER"), TILEATB_WATER);
	atbparser.AddAtbInfo( TEXT("TILEATB_DEEPWATER"), TILEATB_DEEPWATER);
	// atbparser.AddAtbInfo( TEXT("TILEATB_XSPR"), TILEATB_XSPR);
	
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/TileSetInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/TileSetInfo.txt") );
	}

	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("TileSetInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}

	SI32 unique;
	SI32 imagenumber;
	TCHAR atb[128];
	TCHAR parent[128];
	TCHAR name[128];
	UI08 uicolor;
	SI32 color;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&unique,	4,
			NDATA_MBSTRING,	atb,		128,
			NDATA_INT32,	&imagenumber,		4,
			NDATA_MBSTRING,	parent,		128,
			NDATA_MBSTRING,	name,	128,
			NDATA_INT32,	&color,		4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 index = 0;
	while (!dataloader.IsEndOfData())
	{

		if (dataloader.ReadData()) 
		{
			if (atb [ 0 ] != NULL)
			{			
				uicolor = (UI08)color;

				SI32 siatb = atbparser.GetValueByString(atb);
				
				pclTileInfo[index] = new cltTileInfo(unique, siatb, uicolor, 
					imagenumber, GetTileInfoPointer(parent), name);

				// 타일의 유니크를 저장한다. 
				if(unique >= MAX_TILE_UNIQUE_NUMBER)
				{
					if (pclClient->GameMode != GAMEMODE_SERVER)
						MessageBox(NULL, TEXT("Warning Out of TileSetUniQue Number"),TEXT("Error"), MB_OK|MB_TOPMOST);
				}
				else
				{
					siTileRef[unique] = index;
				}

				index++;

				// 타일 세트를 가질 수 있는 최대 수를 초과하면 에러 메시지를 보낸다. 
				if(index >= MAX_TILESET_NUMBER)
				{
					if (pclClient->GameMode != GAMEMODE_SERVER)
						MessageBox(NULL, TEXT("Warning Out of TileSet Number"), TEXT("Warning"), MB_OK|MB_TOPMOST);
				}
			}
		}
	}

	return TRUE;
}

cltTileInfo* cltTileManager::GetTileInfoPointer(TCHAR* szname)
{
	SI32 i;

	// "NULL"이 입력되면 NULL을 리턴한다. 
	if(_tcscmp(szname, TEXT("NULL")) == 0)
		return NULL;

	for(i = 0;i < MAX_TILESET_NUMBER;i++)
	{
		if(pclTileInfo[i])
		{
			if(_tcscmp(pclTileInfo[i]->szName, szname) == 0)
			{
				return pclTileInfo[i];
			}
		}
	}

	return NULL;

}

BOOL cltTileManager::DeleteTileSet()
{
	SI32 i;

	for(i = 0;i < MAX_TILESET_NUMBER;i++)
	{
		if(pclTileInfo[i])
		{
			delete pclTileInfo[i];
			pclTileInfo[i] = NULL;
		}
	}

	return TRUE;
}

// 모든 타일 세트를 메모리에서 해제한다. 
bool cltTileManager::FreeTileSet()
{
	SI32 i;

	for(i = 0;i < MAX_TILESET_NUMBER;i++)
	{
		if(pclTileInfo[i])
		{
			SI32 size = pclTileInfo[i]->FreeAllSpr();
			
			// 로딩된 메모리 사이즈를 줄인다. 
			siLoadedTileSetImageSize -= size;
		}
	}

	return true;
}

// 현재 설정에 맞게 타일 세트를 읽어온다. 
BOOL cltTileManager::LoadTileSet(SI32 sibasetilesetunique)		
{
	SI32 i;
	cltTileInfo* pcltileinfo = NULL;

	// 요구되는 베이스 타일 세트를 찾는다. 
	for(i = 0; i < MAX_TILESET_NUMBER ;i++)
	{
		if(pclTileInfo[i])
		{
			if( pclTileInfo[i]->Unique == sibasetilesetunique)
			{
				pcltileinfo = pclTileInfo[i];
				break;
			}
		}
	}

	if(pcltileinfo == NULL)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Cannot Find required TileSet"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}

	// 이미 로딩된 타일세트라면 더이상 로딩하지 않는다. 
	if(pcltileinfo->pSpr[0])
	{
		return FALSE;
	}

	// 자신의 타일을 읽어온다.

	for(i = 0; i < (SI32)pcltileinfo->TileImageFileNumber;i++)
	{
		TCHAR szfilename[128];
		StringCchPrintf(szfilename, 128, TEXT("%sTile\\%s\\%s%02d.Mpr"), szSavePath, pcltileinfo->szName, pcltileinfo->szName, i+1);
		
		pcltileinfo->pSpr[i] = new TSpr;
		if(pcltileinfo->pSpr[i])
		{
			GP.LoadTileFromFile(szfilename, pcltileinfo->pSpr[i]);
			
			// 로딩된 타일세트 크기 누계. 
			siLoadedTileSetImageSize += pcltileinfo->pSpr[i]->GetImageDataSize();
		}

	}


	// 자식 타일을 읽어온다. 
	for(i = 0; i<(SI32)pcltileinfo->dwChildNumber;i++)
	{
		LoadTileSet(pcltileinfo->pChild[i]->Unique);		
	}


	return TRUE;
}

// 지도에 필요한 타일세트를 전부 로딩한다. 
bool cltTileManager::LoadTileSet(SI32 tileset1, SI32 tileset2)
{
	if(tileset1)
	{
		LoadTileSet(tileset1);
	}
	else
	{
		tileset1 = 1;
		LoadTileSet(tileset1);
	}
	
	if(tileset2)
	{
		LoadTileSet(tileset2);
	}

	return true;

}


SI32 cltTileManager::FindRefFromUnique_TileSet(SI32 siunique)
{
	if(siunique <= 0 || siunique >= MAX_TILE_UNIQUE_NUMBER)return -1;
	return siTileRef[siunique];
}
