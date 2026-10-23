//---------------------------------
// 2003/7/29 ���°�
//---------------------------------

#include <CommonLogic.h>
#include "MapManager.h"

#include "../../../NLib/NTextManager.h"

#include "SmallMap\SmallMap.h"
#include "MapTypeManager.h"
//#include "../GateManager/GateManager.h"
//#include "../GateManager/InstanceMapGateManager.h"
#include "../../Char/ParametaBox/parametabox.h"
#include "../../Char/CharServer/Char-Server.h"
#include "../../Char/CharManager/CharManager.h"
#include "../../../Server/Server.h"
#include "../../../Server/TraceBoot.h"

#include "../../../CommonLogic/MsgType-System.h"
#include "../../../CommonLogic/Msg/MsgType-Hunt.h"
#include "../../../CommonLogic/MsgRval-Define.h"

#include "../../../Common/PartyMgr/PartyObj.h"
#include "../../../Common/PartyMgr/PartyMgr.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"

#include "../../NPCManager/NPC.h"

#include "NDataLoader.h"

#include "NTCHARString.h"

cltMapCommon* OrgAddress[MAX_MAP_NUMBER];

extern cltCommonLogic* pclClient;

//----------------------------------------------
// cltMapManager
//----------------------------------------------
cltMapManager::cltMapManager(SI32 gamemode, SI32 servicearea, cltVillageManager*		pclvillagemanager)
{
	int i;

	pclVillageManager	= pclvillagemanager;

	if(pclvillagemanager == NULL)
	{
		MsgBox(TEXT("fsd39f"), TEXT("sffds"));
	}

	GameMode = gamemode;

	CurrentMapIndex	= 0;
	pclCurrentMap	= NULL;

	for( i = 0; i < MAX_MAP_NUMBER; ++i )
	{
		pclMapArray[i] = NULL;
	}

	pclMapTypeManager	= new cltMapTypeManager;


	// ���� �̸��� �ҷ��� �����Ѵ�. 
	LoadMapName(servicearea);
	TraceBoot("cltMapManager ctor: LoadMapName done");

	pclGateManager		= new cltGateManager();
	TraceBoot("cltMapManager ctor: gate manager done");
	pclInstanceMapGateManager = NULL;

	// ���� ������ ��ġ. 
	DlgRectInitSwitch	= FALSE;

	if( gamemode == GAMEMODE_SERVER )
	{

		m_world.CreateWorld( MAX_MAP_NUMBER, 
			MAX_CHAR_NUMBER, 
			800, 
			600, 
			MAP_TILE_XSIZE, 
			MAP_TILE_YSIZE );
		TraceBoot("cltMapManager ctor: CreateWorld done");
	}

	m_UpdateTimer_Action.Init( 10 * 1000 );		// 10�� ������Ʈ
	m_UpdateTimer_Action.SetActive( true, ::GetTickCount() );

	m_siGatePositionID = 0;
	m_siChristmasGatePositionID = 0;
	m_siFestivalGatePositionID = 0;
}

cltMapManager::~cltMapManager()
{

	for( int i = 0; i < MAX_MAP_NUMBER; ++i )
	{
		NDelete( pclMapArray[i] );
	}

	NDelete( pclMapTypeManager );
	NDelete( pclGateManager );
	NDelete( pclInstanceMapGateManager );
}

void cltMapManager::Init()
{
	pclInstanceMapGateManager = new cltInstanceMapGateManager();
}

// ���� �̸��� �ҷ��� �����Ѵ�. 
void cltMapManager::LoadMapName(SI32 servicearea)
{
	TCHAR szFileName[MAX_PATH] = TEXT("");

#ifdef _DEV
	StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_DEV.txt") );
#else
	switch ( pclClient->siServiceArea )
	{
	case ConstServiceArea_Korea:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_KOR.txt") );
		}
		break;
	case ConstServiceArea_Japan:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_JPN.txt") );
		}
		break;
	case ConstServiceArea_China:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_CHI.txt") );
		}
		break;
	case ConstServiceArea_English:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_ENG.txt") );
		}
		break;
	case ConstServiceArea_Taiwan:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_TAI.txt") );
		}
		break;
	case ConstServiceArea_USA:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_ENG.txt") );
		}
		break;
	case ConstServiceArea_NHNChina:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_CHI.txt") );
		}
		break;
	case ConstServiceArea_EUROPE:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Map_EUR.txt") );
		}
		break;
	}
#endif

	NTextManager MapNameText;
	MapNameText.CreateTextManager();

	// Plaintext text is the permanent choice for this deployment (see
	// cltCommonLogic::Create) - the NCR decrypt path is not used.
	MapNameText.LoadTextFromFile( szFileName, 1 );
	TraceBoot("LoadMapName: map-name text loaded from '%s'", szFileName);

/*
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_Korea :
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfoK.txt") );
		break;
	case ConstServiceArea_China :
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfoC.txt") );
		break;
	case ConstServiceArea_Japan : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfoJ.txt") );
		break;
	case ConstServiceArea_English : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfoE.txt") );
		break;
	case ConstServiceArea_Taiwan : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfoT.txt") );
		break;
	case ConstServiceArea_USA : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfoU.txt") );
		break;

	default:
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data\\MapInfo.txt") );
		break;
	}
*/
	TCHAR	datfilename[35];
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// �ѱ�
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // ����
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  // NHNChina
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data\\MapInfoG.txt"));
	}

	/*
	NTextManager MapNameText;
	MapNameText.CreateTextManager();

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		MapNameText.LoadTextFromCryptFile(TEXT("TextMgr\\MapTextMgr.txt"), pclClient->GetTextColumn());	
	}
	else
	{
		MapNameText.LoadTextFromFile(TEXT("TextMgr\\MapTextMgr.txt"), pclClient->GetTextColumn());
	}
	*/
	

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;
	FILE* fp = _tfopen( datfilename, TEXT("rb") );
	BYTE magic[3] = {0};
	BOOL bNCRFile = FALSE;

	if( fp != NULL )
	{
		fread( magic, 1, sizeof(magic), fp );
		fclose( fp );
		bNCRFile = magic[0] == 'N' && magic[1] == 'C' && magic[2] == 'R';
	}

	if( bNCRFile )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datfilename );
	}

	TraceBoot("LoadMapName: siServiceArea=%d file='%s' opened=%d isNCR=%d bLoadFile=%d",
		pclClient->siServiceArea, datfilename, (fp != NULL) ? 1 : 0, bNCRFile ? 1 : 0, bLoadFile ? 1 : 0);
	
	if (!bLoadFile)
	{
		BOOL bFallback = FALSE;
		FILE* fpf = _tfopen( TEXT("Data\\mapinfoE.txt"), TEXT("rb") );
		if( fpf )
		{
			BYTE m[3] = {0};
			fread( m, 1, 3, fpf );
			fclose( fpf );
			if( m[0] == 'N' && m[1] == 'C' && m[2] == 'R' )
				bFallback = dataloader.LoadDataFromCryptFile( TEXT("Data\\mapinfoE.txt") );
			else
				bFallback = dataloader.LoadDataFromFile( TEXT("Data\\mapinfoE.txt") );
		}
		TraceBoot("LoadMapName: FALLBACK Data\\mapinfoE.txt bLoadFile=%d", bFallback ? 1 : 0);
		if( !bFallback )
		{
			MsgBox( TEXT("Not LoadFromFile MapInfo"),TEXT("Error"));
			return;
		}
	}

	SI32 mapindex;
	TCHAR mapname[1024];
	SI32 mapunique;
	SI32 villageunique;
	SI32 parentmap;
	SI32 minlevel;
	SI32 maxlevel;
	SI32 blockwarp;
	SI32 encountmap;
	SI32 seamap;
	SI32 landmap;
	SI32 fogmap;
	SI32 instancemap;
	SI32 guildmap;
	SI32 pkMap;
	SI32 cloneMap = 0;
	SI32 userLimit = 0;
	SI32 siCanMarket = 0;
	TCHAR musicfile[256];

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&mapindex,   4,		
			NDATA_MBSTRING,	mapname,	256,
			NDATA_INT32,	&mapunique,	4,
			NDATA_INT32,	&villageunique,	4,
			NDATA_INT32,	&parentmap,	4,
			NDATA_INT32,	&minlevel,		4,
			NDATA_INT32,	&maxlevel,		4,
			NDATA_INT32,	&blockwarp,		4,			
			NDATA_INT32,	&encountmap,	4,
			NDATA_INT32,	&seamap,	4,
			NDATA_INT32,	&landmap,	4,
			NDATA_INT32,	&fogmap,	4,
			NDATA_INT32,	&instancemap,	4,
			NDATA_INT32,	&guildmap,	4,
			NDATA_INT32,	&pkMap,	4,
			NDATA_INT32,	&cloneMap,	4,
			NDATA_INT32,	&userLimit,	4,
			NDATA_INT32,	&siCanMarket,	4,
			NDATA_MBSTRING,	musicfile,	256,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 siRowCount = 0;
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() ) 
		{
			++siRowCount;
			if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)
			{
				// mapinfoE.txt declares indexes past MAX_MAP_NUMBER; writing those would
				// corrupt clUnitMapInfo[]/pclMapArray[] and crash the server on load.
				TraceBoot("mapinfoE: SKIP out-of-range mapindex=%d (MAX_MAP_NUMBER=%d) unique=%d", mapindex, MAX_MAP_NUMBER, mapunique);
				continue;
			}
			if(mapindex == 0)
			{
				TraceBoot("mapinfoE: parsed row index=0 name='%s' unique=%d village=%d parent=%d",
					mapname, mapunique, villageunique, parentmap);
			}
			// MapTextMgr.txt ���� �� �̸��� �����´�.

			TCHAR* szMapNameText = MapNameText.GetText(SI32ToString(mapindex));
			if(szMapNameText != NULL)
			{
				StringCchCopy(mapname, sizeof(mapname), szMapNameText);
			}
			else if(mapname[0] == NULL)
			{
				StringCchPrintf(mapname, sizeof(mapname), TEXT("Map_%d"), mapindex);
			}

			if (mapname[0] != NULL)
			{
				if(villageunique)
				{
					TCHAR* pvillagename = pclVillageManager->GetName(villageunique);
					if(pvillagename)
					{
						SetMapName(mapindex, pvillagename);
					}
				}
				else if(IsHuntMap(mapindex))
				{
					SI32 huntvillageunique = mapindex - 200;
					TCHAR* pvillagename = pclVillageManager->GetName(huntvillageunique);
					if(pvillagename)
					{
						TCHAR temp[256];
						StringCchCopy(temp, 256, pvillagename);
						TCHAR* pText = GetTxtFromMgr(1704);
						StringCchCat(temp, 256, pText);

						SetMapName(mapindex, temp);

					}

				}
				else
				{
					SetMapName(mapindex, mapname);
				}

				SetBGM( mapindex, musicfile );

				// MaxLevel�� 0 �̸� ���� �ְ������� �ٲپ� �ش�.
				if(maxlevel <= 0)		maxlevel = MAX_CHARACTER_LEVEL;

				SetMapBasicInfo(mapindex, mapunique, villageunique, 
					minlevel, maxlevel, DATA_TO_BOOL(blockwarp), parentmap,
					DATA_TO_BOOL(encountmap), DATA_TO_BOOL(seamap),
					DATA_TO_BOOL(landmap), DATA_TO_BOOL(fogmap),
					instancemap, DATA_TO_BOOL(guildmap), DATA_TO_BOOL(pkMap),
					userLimit, DATA_TO_BOOL(siCanMarket) );

				if( cloneMap )
				{
					AddCloneMap( cloneMap, mapindex );
				}

			}
		}
	}

	TraceBoot("mapinfoE: total_rows_parsed=%d", siRowCount);

	// The dataloader pass above already populated clUnitMapInfo[] for every valid
	// index. The legacy Map\MapInfo*.txt parser below re-reads the same data with
	// no bounds checking on mapindex (MapInfo.txt reaches 465 while
	// MAX_MAP_NUMBER is 400), which writes out of bounds and faults. Skip it.
	if( siRowCount > 0 )
	{
		TraceBoot("LoadMapName: skipping legacy Map\\MapInfo*.txt pass (dataloader already loaded %d rows)", siRowCount);
		return;
	}


	/*cyj
	//-------------------------------------------
	// ���� ������ ���� �ٸ� �ؽ�Ʈ�� �ҷ��´�. 
	//-------------------------------------------
	if(servicearea == ConstServiceArea_Korea )	// �ѱ�
	{
		StringCchCopy(filename, 128, TEXT("Map\\MapInfo.txt"));

	}

	else if(servicearea == ConstServiceArea_China)
	{
		StringCchCopy(filename, 128, TEXT("Map\\MapInfoC.txt"));
	}
	else if(servicearea == ConstServiceArea_English)
	{
		StringCchCopy(filename, 128, TEXT("Map\\MapInfoE.txt"));
		//_tcscpy(filename, TEXT("Map\\MapInfo_China.txt"));
	}
	else if(servicearea == ConstServiceArea_Japan)
	{
		StringCchCopy(filename, 128, TEXT("Map\\MapInfoJ.txt"));
		//_tcscpy(filename, TEXT("Map\\MapInfo_China.txt"));
	}
	else
	{
		MsgBox(TEXT("fdf38fj"), TEXT("fdd3:%d"), servicearea);
		return ;
	}
	
	fp = _tfopen(filename, TEXT("rt"));

	if(fp == NULL)
	{
		MessageBox(NULL, TEXT("MapInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return ;
	}

	// ù��° 6���� �����Ѵ�. 
	for(i = 0;i < 6;i++)
		_fgetts(buffer, 1024, fp);
	
	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		ParseText( buffer );

		mapindex = _tstoi( tokens[0] );
		MStrCpy( mapname, tokens[1], 256 );
		mapunique = _tstoi( tokens[2] );
		villageunique = _tstoi( tokens[3] );
		parentmap = _tstoi( tokens[4] );
		levellimit = _tstoi( tokens[5] );
		encountmap = _tstoi( tokens[6] );
		seamap = _tstoi( tokens[7] );
		landmap = _tstoi( tokens[8] );
		fogmap = _tstoi( tokens[9] );
		instancemap = _tstoi( tokens[10] );

	//	_stscanf(buffer, TEXT("%d %s %d %d %d %d %d %d %d %d %d"),
	//		&mapindex, mapname, &mapunique, &villageunique, &parentmap, &levellimit,&encountmap,
	//		&seamap, &landmap,&fogmap,&instancemap);

		if(villageunique)
		{
			TCHAR* pvillagename = pclVillageManager->GetName(villageunique);
			if(pvillagename)
			{
				SetMapName(mapindex, pvillagename);
			}
		}
		else if(IsHuntMap(mapindex))
		{
			SI32 huntvillageunique = mapindex - 200;
			TCHAR* pvillagename = pclVillageManager->GetName(huntvillageunique);
			if(pvillagename)
			{
				TCHAR temp[256];
				StringCchCopy(temp, 256, pvillagename);
				TCHAR* pText = GetTxtFromMgr(1704);
				StringCchCat(temp, 256, pText);

				SetMapName(mapindex, temp);
			}
		}
		else
		{
			SetMapName(mapindex, mapname);
		}

		SetMapBasicInfo(mapindex, mapunique, villageunique, levellimit, parentmap,
			DATA_TO_TYPE_BOOL(encountmap), DATA_TO_TYPE_BOOL(seamap),
			DATA_TO_TYPE_BOOL(landmap),DATA_TO_TYPE_BOOL(fogmap),DATA_TO_TYPE_BOOL(instancemap));
	}

	fclose(fp);
	*/

}


bool cltMapManager::DeleteAllMap()
{
	for( int i = 0; i < MAX_MAP_NUMBER; ++i )
	{
		NDelete( pclMapArray[i] );
	}			
	return true;
}

bool cltMapManager::SetCurrentMapIndex(SI32 index)
{
	if(index < 0 || index >= MAX_MAP_NUMBER)return false;

	CurrentMapIndex	= index;
	pclCurrentMap	= pclMapArray[ CurrentMapIndex ];
	return true;
}

SI32 cltMapManager::GetVillageUnique(SI32 index)
{
	if(index < 0 || index >= MAX_MAP_NUMBER)return 0;
	if(pclMapArray[index] == NULL)return 0;

	return pclMapArray[index]->siVillageUnique;
}

// ���� ����ũ�� ������ �ش� ������ �ε����� ã�´�. 
SI32 cltMapManager::GetMapIndexFromVillageUnique(SI32 villageunique)
{
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return -1;

	SI32 i;

	for(i = 0;i < MAX_MAP_NUMBER;i++)
	{
		if(pclMapArray[i] && pclMapArray[i]->siVillageUnique == villageunique)
		{
			return i;
		}
	}
	return -1;
}

// ���� �̸��� ������ ������ �ε����� ã�´�. 
SI32 cltMapManager::GetMapIndexFromName(TCHAR* name)
{
	SI32 i;

	for(i = 0;i < MAX_MAP_NUMBER;i++)
	{
		// KHY - 1122 - �����̵� �̿�� ����.
		if( pclClient->GameMode == GAMEMODE_SERVER )
		{
			if( pclMapArray[i] && _tcscmp(clUnitMapInfo[i].szMapName, name) == 0 )
			{
				return i;
			}
		}
		else if ( pclClient->GameMode == GAMEMODE_CLIENT )
		{
			if( _tcscmp(clUnitMapInfo[i].szMapName, name) == 0 )
			{
				return i;
			}
		}
	}
	return -1;
}


cltMapCommon* cltMapManager::GetMapPointer(SI32 index)
{
	if(index < 0 || index >= MAX_MAP_NUMBER)return NULL;
	if(pclMapArray[index] == NULL)return NULL;

	return pclMapArray[index];
}


// ������ �ҷ��´�. 
cltMapCommon* cltMapManager::Load(SI32 mapindex, SI32 mapunique, TCHAR* filename, cltCharManager* pclcm, cltTileManager* pcltileset, cltFieldObjectManager* pclfieldobjectmanager)
{
	DWORD loadTime = GetTickCount();

	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return NULL;

	FILE* fp;
	cltMapHeader MH;
	cltMapCommon* pclMap = NULL;

	// ������ ����� �о�´�. 
	fp=_tfopen(filename, TEXT("rb"));
	if(fp==NULL)
	{
		TraceBoot("MapManager::Load FAIL open file='%s' index=%d unique=%d", filename?filename:"(null)", mapindex, mapunique);
#ifdef _DEBUG
		NTCHARStringBuilder	build;
		build.SetFormat("{1} ������ �����ϴ�.") << filename;
		MessageBox(NULL, build, "cltMapManager::Load", MB_OK);
#endif
		return pclMap;
	}

	memset( &MH, 0, sizeof(cltMapHeader) );

	size_t nRead = fread(&MH, sizeof(cltMapHeader), 1, fp);

	fclose(fp);

	// Guard against missing/truncated map files: a short or garbage header would
	// otherwise be handed straight to the cltMapServer/cltMapClient constructor and
	// allocate from bogus dimensions, which crashes the server during LoadAllMap.
	if( nRead != 1 || MH.MapXsize <= 0 || MH.MapYsize <= 0 || MH.MapXsize > 10000 || MH.MapYsize > 10000 )
	{
		TraceBoot("MapManager::Load SKIP bad header file='%s' index=%d unique=%d read=%u X=%u Y=%u",
			filename, mapindex, mapunique, (unsigned)nRead,
			(unsigned)MH.MapXsize, (unsigned)MH.MapYsize);
		return NULL;
	}

	// ������ �ʱ�ȭ���Ѵ�. 
	if(GameMode == GAMEMODE_SERVER)
	{
		pclMap = new cltMapServer( (ITZWorld< ITZMap > *)&m_world, mapindex, mapunique, MH.MapXsize, MH.MapYsize, MH.TileSet1, MH.TileSet2, pclcm, pcltileset, pclfieldobjectmanager);

	}
	else
	{
		pclMap = new cltMapClient(mapindex, mapunique, MH.MapXsize, MH.MapYsize, MH.TileSet1, MH.TileSet2, pclcm, pcltileset, pclfieldobjectmanager);
	}

	

	// ������ ���������� �о�´�. 
	fp=_tfopen(filename, TEXT("rb"));
	if(fp==NULL)
	{
		delete pclMap;
		pclMap = NULL;
		return pclMap;
	}

	fread(&pclMap->MH, sizeof(cltMapHeader), 1, fp);

	pclMap->ReadIDMap(fp);
	pclMap->ReadCollMap(fp);
	
	pclMap->pclLayer[0]->Read(fp);
	pclMap->pclLayer[1]->Read(fp);

	pclMap->ReadFieldObjectMap(fp);
	pclMap->ReadFieldObjectMapPara(fp);

	if(pclMap->MH.Version  >= 2)
	{
		// �ѱ��� ������ ���� �ʴ´�.	- By LeeKH
		if( pclClient->siServiceArea != ConstServiceArea_Korea )
			pclMap->pclMapGeoInfo->Read(fp);
	}

	fclose(fp);
	
	
	pclMapArray[mapindex] = pclMap;
	OrgAddress[mapindex] = pclMapArray[mapindex];

#ifdef _DEBUG
//	loadTime = TABS( loadTime - GetTickCount() );
	TCHAR buf[256] = {0,};
	if ( GameMode == GAMEMODE_CLIENT )
	{
		StringCchPrintf( buf, 256, TEXT("MapLoadTime : %dms"), GetTickCount() - loadTime );
		pclClient->pclMessage->SetMsg( buf );
	}
	loadTime = GetTickCount();
#endif

	if ( pclClient->IsCountrySwitch( Switch_FieldObjectPreLoad) )
	{

		pclfieldobjectmanager->DeleteIfNotUse( 0 );

		bool bmainmap = ( mapindex==MAPINDEX_MAINFIELD )?true:false;

		bmainmap = false; // �ϴ� ���θ� �ε� �ٽ� �ϰ�..

		for ( SI32 i=0; i < pclMap->GetXsize(); ++i )
		{
			for ( SI32 j=0; j < pclMap->GetYsize(); ++ j )
			{
				if ( pclMap->IsInMapArea( i, j ) == false )
					continue;

				SI32 fieldobject = pclMap->GetFieldObject_Unique(i, j);

				SI32 ref = pclfieldobjectmanager->FindRefFromUniqueMinus(fieldobject);
				if(ref < 0) continue;

				SI32 file = 0;
				SI32 font = 0;

				if( pclfieldobjectmanager->pclInfo[ref]->siFOSprRef < 0)
				{
					pclfieldobjectmanager->LoadSet(pclClient->GetFrame(), pclfieldobjectmanager->pclInfo[ref], bmainmap );
				}

				TSpr* pspr = pclfieldobjectmanager->pclInfo[ref]->GetSpr(pclfieldobjectmanager, 0);
			}
		}

#ifdef _DEBUG
		//loadTime = TABS( loadTime - GetTickCount() );
		if ( GameMode == GAMEMODE_CLIENT )
		{
			StringCchPrintf( buf, 256, TEXT("FOLoadTime : %dms"), GetTickCount() - loadTime );
			pclClient->pclMessage->SetMsg( buf );
		}
#endif
	}

	return pclMap;
}

bool cltMapManager::MakeLastMap(SI32 mapindex)
{
	SI32 i, j, k;

	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return false;
	if(pclMapArray[mapindex] == NULL)return false;


	pclMapArray[mapindex]->MakeLastMap();


	// ����Ʈ�� �߰��Ѵ�. 
	// �� ����Ʈ ��尡 �ƴϾ�� �Ѵ�. 

	//cyj �ʿ���Ʈ ����ֶ��� ����Ʈ ���̵��� ����
	//if(GameMode != GAMEMODE_MAPEDIT)
	//{
		for(i = 0;i < MAX_GATE_NUMBER;i++)
		{
			cltGate* pclgate = &pclGateManager->clGate[i];

			if(pclgate->FOUnique == 0)continue;
			
			if(pclgate->SrcMapIndex != mapindex)continue;
			
			pclMapArray[mapindex]->PutGateFieldObject(pclgate->SrcX, pclgate->SrcY, pclgate->FOUnique, pclgate->siUnique);
			
			// Gate Field Object�ֺ��� �浹 ������ �����Ѵ�. 
			for(k= -10;k<10;k++)
				for(j= - 10;j<10;j++)
				{
					SI32 collinfo = pclMapArray[mapindex]->CalcCollInfo(pclgate->SrcX + j, pclgate->SrcY + k);
					if(collinfo)
					{
						pclMapArray[mapindex]->SetCollMap(pclgate->SrcX + j, pclgate->SrcY + k, collinfo);
					}
				}
			
		}

	//}

	return true;
}

// ���ƿ� �����ΰ�. 
bool cltMapManager::IsHuntMap(SI32 siMapIndex)
{
	if(siMapIndex < 0)				return false;
	if(siMapIndex >= MAX_MAP_NUMBER)return false;

	// 200�� �Ѵ� ������ ����������Ǵ� ���
	if(siMapIndex > 200 && siMapIndex < 200 + MAX_VILLAGE_NUMBER)
	{
		if(pclVillageManager->pclVillageInfo[siMapIndex - 200])	return true;
	}

	return false;
}
// ��ȿ�� �����ΰ�. 
bool cltMapManager::IsValidArea(SI32 mapindex, SI32 x, SI32 y)
{
	if(mapindex < 0)return false;
	if(mapindex  >= MAX_MAP_NUMBER)return false;
	if(pclMapArray[mapindex] == NULL)return false;

	if(x < 0)return false;
	if(pclMapArray[mapindex]->MH.MapXsize <= x)return false;

	if(y < 0)return false;
	if(pclMapArray[mapindex]->MH.MapYsize <= y)return false;

	return true;
}

bool cltMapManager::LoadMap(cltCommonLogic* pclclient, SI32 mapunique, SI32 index, SI32 villageunique,
							 SI32 parentmap, SI32 levellimit, TCHAR* mapname,
							 BOOL encountmap,BOOL seamap, BOOL landmap)
{
	TCHAR buffer[FILE_NAME_SIZE];

	TCHAR *pmaptype = NULL;
	
	pmaptype = pclMapTypeManager->GetMapTypeNameFromUnique(mapunique);
	if(pmaptype == NULL)
	{
		TraceBoot("MapManager::LoadMap FAIL NULL maptype for mapunique=%d index=%d", mapunique, index);
		return false;
	}
	else if(index <= 1)
	{
		TraceBoot("MapManager::LoadMap index=%d mapunique=%d maptype='%s'", index, mapunique, pmaptype);
	}

	//sprintf( buffer, TEXT("%s\n"), pmaptype );
	

	StringCchPrintf(buffer, FILE_NAME_SIZE, TEXT("map\\%s\\%s.Map"), pmaptype, pmaptype);


	// ������ �ƴϾ�� Ÿ���� �ҷ��´�. 
	// ���������� Ÿ���� �ε����� �ʴ´�. 
	if(GameMode != GAMEMODE_SERVER)
	{
		//----------------------------
		// ������ ����� �м��Ѵ�. 
		//----------------------------
		cltMapHeader clHeader;
		if(GetMapInfo(buffer, &clHeader) == FALSE)
		{
			MsgBox(TEXT("f9nnf"), TEXT("f9j2f:%s"), buffer);
			return FALSE;
		}
		
		
		//-----------------------------
		// Ÿ�� ��Ʈ�� �ҷ��´�. 
		//-----------------------------
		
		// ������ Ÿ�� ��Ʈ�� �����Ѵ�. 
		pclclient->pclTileManager->FreeTileSet();

		// ���ο� Ÿ�� ��Ƽ�� �ҷ��´�. 
		pclclient->pclTileManager->LoadTileSet(clHeader.TileSet1, clHeader.TileSet2);
		

	}

	//-----------------------------
	// ������ �ҷ��´�. 
	//-----------------------------

	Load(index, mapunique, buffer, pclclient->pclCM, pclclient->pclTileManager, pclclient->pclFieldObjectManager);

	if(GameMode == GAMEMODE_SERVER)
	{
		pclMapArray[index]->siMapUnique = mapunique;
	}



	StringCchPrintf(buffer, FILE_NAME_SIZE, TEXT("map\\%s\\%s.Spr"), pmaptype, pmaptype);

	SI32 smallmapmode;

	// ������ ��忡 ���� ���� ������ ��嵵 �޶�����. 
	if(GameMode == GAMEMODE_MAPEDIT)
	{
		smallmapmode = SMALLMAPMODE_MAPEDIT;
	}
	else
	{
		smallmapmode = SMALLMAPMODE_GAME;
	}

	pclMapArray[index]->MakeSmallMap(smallmapmode, buffer);

	// ���� ���������� ������ ������ ���δ�.
	// Village attach/detach only drives client-side village rendering. On the server
	// it walks the whole map through pclFieldObjectManager, which has no loaded
	// field-object data, and faults inside cltMapCommon::DeattachVillage().
	if(index == 0 && GameMode != GAMEMODE_SERVER)
	{
		pclMapArray[index]->DeattachVillage();

		// ������ �ҷ��´�.
		pclMapArray[index]->AttachVillage();
	}

	// ������ ������ ���踦 �����Ѵ�.
	pclMapArray[index]->siVillageUnique = villageunique;

    // �ҷ����ų� ���Ƿ� ������ ������ ���������� Ȯ���Ѵ�. 
    MakeLastMap(index);

	return TRUE;

}

// mapindex�� ����Ͽ� ������ �ҷ��´�. 
BOOL cltMapManager::LoadMap(cltCommonLogic* pclclient, SI32 srcmapindex, SI32 mapunique)
{
	SI32 i;

	for(i = 0;i < MAX_MAP_NUMBER;i++)
	{
		if(clUnitMapInfo[i].siMapUnique == 0)continue;

		if(srcmapindex == i)
		{
			if(mapunique == 0)mapunique = clUnitMapInfo[i].siMapUnique;
			LoadMap(pclclient, mapunique, i, clUnitMapInfo[i].siVillageUnique, clUnitMapInfo[i].siParentMap, clUnitMapInfo[i].siLevelLimit, clUnitMapInfo[i].szMapName, clUnitMapInfo[i].bEncountMap, clUnitMapInfo[i].bSeaMap, clUnitMapInfo[i].bLandMap);
			return true;
		}

	}

	return false;

}


// ��� ������ �� �о�´�. 
BOOL cltMapManager::LoadAllMap(cltCommonLogic* pclclient)
{
	SI32 i;

	SI32 siCandidates = 0;
	SI32 siLoaded = 0;

	for(i = 0;i < MAX_MAP_NUMBER;i++)
	{
		if(clUnitMapInfo[i].siMapUnique == 0)continue;

		// ��� ���� ������ �ҷ����� �ʴ´�. 
		if(clUnitMapInfo[i].siVillageUnique > CUR_VILLAGE_LIMIT)continue;
	
		++siCandidates;
		LoadMap(pclclient, clUnitMapInfo[i].siMapUnique, i, clUnitMapInfo[i].siVillageUnique, clUnitMapInfo[i].siParentMap, clUnitMapInfo[i].siLevelLimit, clUnitMapInfo[i].szMapName, clUnitMapInfo[i].bEncountMap, clUnitMapInfo[i].bSeaMap, clUnitMapInfo[i].bLandMap);
		if(pclMapArray[i] != NULL) ++siLoaded;
	}

	TraceBoot("LoadAllMap: candidates=%d loaded=%d | map0_unique=%d village0=%d pclMapArray0=%p",
		siCandidates, siLoaded, clUnitMapInfo[0].siMapUnique, clUnitMapInfo[0].siVillageUnique, (void*)pclMapArray[0]);

	// Report honest failure so callers can fall back to loading a single map.
	return siLoaded > 0 ? TRUE : FALSE;
}


// �������� ����߸� ������ ���� ã�´�. 
bool cltMapManager::FindProperAreaForDrop(SI32 mapindex, SI32 x, SI32 y, SI32* ptargetx, SI32* ptargety)
{
	SI32 length = 0;
	SI32 minlength = 1000;
	SI32 minx = 0;
	SI32 miny = 0;
	SI32 i, j;
	SI32 block = BLOCK_DEEPWATER|BLOCK_SHALLOWWATER|BLOCK_CUSTOM|BLOCK_NORMAL;

	for(i = -2;i <= 2;i++)
		for(j = -2;j <= 2;j++)
		{
			SI32 tempx = x + j;
			SI32 tempy = y + i;
			
			// ���� ��ȿ ������ üũ�Ѵ�. 
			if(pclMapArray[mapindex]->IsInMapArea(tempx, tempy) == false)continue;
			// �ٴٿ��� ��ǰ�� ���� �� ����. 
			if(pclMapArray[mapindex]->IsColl(tempx, tempy, block))continue;
			
			cltItemMapInfo* pclitemmapinfo = (cltItemMapInfo*)pclMapArray[mapindex]->GetItemMap(tempx, tempy);
			if(pclitemmapinfo == NULL)continue;
			// �̹� �ٸ� ������ �ִٸ� ����. 
			if(pclitemmapinfo->uiItemUnique)continue ;
			
			length = max(TABS(j) , TABS(i));
			if(length < minlength)
			{
				minlength = length;
				minx = tempx;
				miny = tempy;
			}
		}
		
		// ����� ������ ���� ���� ���ٸ�, 
		if(minlength == 1000)
		{
			return false;
		}
		
		*ptargetx = minx;
		*ptargety = miny;
		
		return true;
}

void cltMapManager::SetMapBasicInfo(SI32 mapindex, SI32 mapunique, 
									SI32 villageunique,
									SI32 minlevel, SI32 maxlevel,
									bool blockwarp,
									SI32 parentmap, bool encountmap, 
									bool seamap, bool landmap,bool fogmap,
									SI16 instancemap, bool guildmap, bool pkMap,
									SI32 userlimit,
									bool bCanMarket)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return ;

	clUnitMapInfo[mapindex].siMapUnique		= mapunique;
	clUnitMapInfo[mapindex].siVillageUnique	= villageunique;
	clUnitMapInfo[mapindex].siLevelLimit	= 0;
	clUnitMapInfo[mapindex].siMinLevel		= minlevel;
	clUnitMapInfo[mapindex].siMaxLevel		= maxlevel;
	clUnitMapInfo[mapindex].bBlockWarp		= blockwarp;
	clUnitMapInfo[mapindex].siParentMap		= parentmap;
	clUnitMapInfo[mapindex].bEncountMap		= encountmap;
	clUnitMapInfo[mapindex].bSeaMap			= seamap;
	clUnitMapInfo[mapindex].bLandMap		= landmap;
	clUnitMapInfo[mapindex].bFogMap			= fogmap;
	clUnitMapInfo[mapindex].siInstanceMap	= instancemap;
	clUnitMapInfo[mapindex].bGuildMap		= guildmap;
	clUnitMapInfo[mapindex].bPkMap			= pkMap;
	clUnitMapInfo[mapindex].siUserLimit		= userlimit;
	clUnitMapInfo[mapindex].bCanMarket		= bCanMarket;
}

bool cltMapManager::IsEncountMap(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return false;

	return clUnitMapInfo[mapindex].bEncountMap;

}
bool cltMapManager::IsSeaMap(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return false;

	return clUnitMapInfo[mapindex].bSeaMap;

}
bool cltMapManager::IsInstanceMap(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return false;

	return ( clUnitMapInfo[mapindex].siInstanceMap > INSTANCEMAP_TYPE_NONE );
}
bool cltMapManager::IsLandMap(SI32 mapindex)
{
	if( mapindex < 0 || mapindex >= MAX_MAP_NUMBER ) return false;

	return clUnitMapInfo[mapindex].bLandMap;
}

bool cltMapManager::IsFogMap(SI32 mapindex)
{
	if( mapindex < 0 || mapindex >= MAX_MAP_NUMBER )return false;

	return clUnitMapInfo[mapindex].bFogMap;
}

//��� ������ ������ �����ΰ�.
bool cltMapManager::IsGuildMap(SI32 mapindex )
{
	if( mapindex < 0 || mapindex >= MAX_MAP_NUMBER )return false;

	return clUnitMapInfo[mapindex].bGuildMap;
}

SI32 cltMapManager::GetParentMap(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return 0;

	return clUnitMapInfo[mapindex].siParentMap;
}

bool cltMapManager::IsPkMap(SI32 MapIndex)
{
	if(MapIndex < 0 || MapIndex >= MAX_MAP_NUMBER)
		return false;

	return clUnitMapInfo[MapIndex].bPkMap;
}

SI32 cltMapManager::GetLevelLimit(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return 0;

	//return clUnitMapInfo[mapindex].siLevelLimit;
	return clUnitMapInfo[mapindex].siMinLevel;
}

SI32 cltMapManager::GetMinLevel(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return 0;

	return clUnitMapInfo[mapindex].siMinLevel;
}

SI32 cltMapManager::GetMaxLevel(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return 0;

	return clUnitMapInfo[mapindex].siMaxLevel;
}

bool cltMapManager::IsEnterLevel(SI32 mapindex, SI32 level)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)	return false;
	if(	clUnitMapInfo[mapindex].siMinLevel <= level && 
		clUnitMapInfo[mapindex].siMaxLevel >= level)
	{
		return true;
	}

	return false;
}

bool cltMapManager::IsBlockWarp(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)	return false;

	if( clUnitMapInfo[mapindex].bBlockWarp == true )
	{
		return true;
	}

	return false;    
}

void cltMapManager::SetMapName(SI32 mapindex, TCHAR* pname)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return;

	StringCchCopy(clUnitMapInfo[mapindex].szMapName, MAX_MAP_NAME_SIZE, pname);
}

TCHAR* cltMapManager::GetMapName(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return NULL;

	return clUnitMapInfo[mapindex].szMapName;
}




// ������ ����ũ�� ��´�.
SI32 cltMapManager::GetMapUnique(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return 0;

	return clUnitMapInfo[mapindex].siMapUnique;
}
// ���� BGM�� �����Ѵ�.
void cltMapManager::SetBGM( SI32 mapindex, TCHAR* filename )
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER) return;

	StringCchCopy(clUnitMapInfo[mapindex].szBGMFileName, 256, filename );
}
bool cltMapManager::HaveCloneMap( SI32 mapindex )
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER) return false;

	return clUnitMapInfo[mapindex].bHaveCloneMap;
}
SI32 cltMapManager::GetUserLimit( SI32 mapindex )
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER) return 0;

	return clUnitMapInfo[mapindex].siUserLimit;
}
void cltMapManager::InitUserCount()
{
	for( SI32 i=0; i<MAX_MAP_NUMBER; i++ )
	{
		clUnitMapInfo[i].siUserCount = 0;
	}
}
void cltMapManager::AddUserCount( SI32 mapindex )
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER) return;

	++clUnitMapInfo[mapindex].siUserCount;
}
SI32 cltMapManager::GetUserCount( SI32 mapindex )
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER) return 0;

	if( pclClient->GameMode != GAMEMODE_SERVER ) return 0;

	SI32 userCount = 0;
	for ( SI32 i=0; i<MAX_PERSON_NUMBER; ++i )
	{
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[i];
		if ( pclClient->pclCM->IsValidID(i) )
		{
			if ( pclClient->pclCM->CR[i]->GetMapIndex() == mapindex )
			{
				++userCount;
			}
		}
	}
	return userCount;

//	return clUnitMapInfo[mapindex].siUserCount;
}

SI32 cltMapManager::GetCloneMap(SI32 mapindex)
{
	if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER) return 0;

	if ( clUnitMapInfo[mapindex].bHaveCloneMap == false ) return 0;

	SI32 cloneindex = clUnitMapInfo[mapindex].siCloneIndex;

	SI32 selectedMapUserCount =0;
	SI32 selectedMapIndex = 0;

	for ( SI32 i=0; i < CLONEMAP_NUM ; ++i )
	{
		SI32 dstMap = clCloneMapInfo[cloneindex].siDstMapIndex[i];
		if ( dstMap <= 0 ) continue;

		if ( GetUserLimit( mapindex ) > GetUserCount( dstMap ) )
		{
			if ( selectedMapUserCount <= GetUserCount( dstMap ) )
			{
				selectedMapUserCount = GetUserCount( dstMap );
				selectedMapIndex = dstMap;
			}
		}
	}
	if ( selectedMapIndex > 0 )
	{
		return selectedMapIndex;
	}
	else
	{
		return 0;
	}
}
void cltMapManager::AddCloneMap(SI32 srcMapIndex, SI32 dstMapIndex)
{
	if(srcMapIndex < 0 || srcMapIndex >= MAX_MAP_NUMBER) return ;
	if(dstMapIndex < 0 || dstMapIndex >= MAX_MAP_NUMBER) return ;

	for ( SI32 i=0; i<MAX_CLONEMAP_TYPE; ++i )
	{
		if ( clCloneMapInfo[i].siSrcMapIndex == srcMapIndex )
		{
			if ( clCloneMapInfo[i].AddCloneMap( dstMapIndex ) )
			{
				//clUnitMapInfo[srcMapIndex].siCloneIndex = i;
				return;
			}
			else
			{
				MsgBox(TEXT("Error"), TEXT("Clone Map Add Error"));
				return;
			}
		}
	}

	for ( SI32 i=0; i<MAX_CLONEMAP_TYPE; ++i )
	{
		if ( clCloneMapInfo[i].siSrcMapIndex == 0 )
		{
			clCloneMapInfo[i].siSrcMapIndex = srcMapIndex;

			if (  clCloneMapInfo[i].AddCloneMap( dstMapIndex ) ) 	
			{ 
				clUnitMapInfo[srcMapIndex].siCloneIndex = i;
				clUnitMapInfo[srcMapIndex].bHaveCloneMap = true;

				return;
			}
		}
	}

	MsgBox(TEXT("Error"), TEXT("Clone Map Add Error"));
}

SI32 cltMapManager::GetSourceMap( SI32 clonemapindex )
{
	for (SI32 i=0; i<MAX_CLONEMAP_TYPE; i++)
	{
		for (SI32 j=0; j<CLONEMAP_NUM; j++)
		{
			if(clCloneMapInfo[i].siDstMapIndex[j] == clonemapindex)
				return clCloneMapInfo[i].siSrcMapIndex;
		}
	}

	return 0;
}

void cltMapManager::Action()
{
	// ���������� ó���Ѵ�.
	if( pclClient->GameMode != GAMEMODE_SERVER )						return;
	cltServer* pclserver = (cltServer*)pclClient;

	if( m_UpdateTimer_Action.IsTimed( ::GetTickCount() ) == false )		return;

	SI32	i;
	SI32	siMapType;

	// ������ ����Ʈ ó��
	if ( pclClient->IsCountrySwitch( Switch_PirateShip ) == true )
	{
		PirateShipGateProcess();
	}

	// ���� 2009.8.28 - ���� �̺�Ʈ - �̺�Ʈ ����Ʈ 
	/*if( pclClient->pclEventTimeManager->InEventTime("FestivalEvent", &pclserver->sTime) )
	{
		FestivalEventGateProcess(TRUE);
	}
	else
	{
		FestivalEventGateProcess(FALSE);
	}*/
	
	//KHY - 1113 - ũ�������� �̺�Ʈ - �̺�Ʈ ����Ʈ.
	if( pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &pclserver->sTime) )
	{
		ChirstmasEventGateProcess();
	}
	else
	{
		if ( pclserver->pclCM->IsValidID( m_siChristmasGatePositionID ) != false )
		{
			// �ش� ���̵��� �ɸ��Ͱ� ���ٸ� ����
			cltCharServer* pclGateChar = (cltCharServer*)pclClient->pclCM->CR[m_siChristmasGatePositionID];
			if ( pclGateChar != NULL )
			{
				SI32 siGateKind		= pclClient->GetUniqueFromHash("KIND_SANTAGATE");	// ����Ʈ ������ KIND

				// �ش� ���̵��� �ɸ��͸� �޾ƿ����� ����Ʈ�� �ƴ϶�� ����
				if ( pclGateChar->GetKind() == siGateKind )
				{
					// �ش� ����Ʈ�� �����ϰ�
					pclGateChar->DeleteInCell();

					// ������ �Ϸ�Ǿ����� ����Ʈ ���̵� �ٽ� �ʱ�ȭ��Ŵ
					if ( pclserver->pclCM->DeleteChar( m_siChristmasGatePositionID ) == TRUE )
					{
						m_siChristmasGatePositionID = 0;
					}				
				}
			}
		}		
	}

	for( i=0; i < MAX_MAP_NUMBER; i++)
	{
		if( IsInstanceMap(i) == false )		continue;

		siMapType = GetInstanceMapType( i );

		switch ( siMapType )
		{
			case INSTANCEMAP_TYPE_GUILD:
				{
					ActionFor_GuildInstanceMap( i, siMapType );
				}
				break;

			case INSTANCEMAP_TYPE_PARTY:
				{
					ActionFor_PartyInstanceMap( i, siMapType );
				}
				break;

			case INSTANCEMAP_TYPE_VILLAGE:
				{
					ActionFor_VillageInstanceMap( i, siMapType );
				}
				break;

			case INSTANCEMAP_TYPE_SERVER:
				{
					ActionFor_ServerInstanceMap( i, siMapType );
				}
		} // switch ( GetInstanceMapType( i ) ) ����

	} // for( i=0; i < MAX_MAP_NUMBER; i++) ����
}

void cltMapManager::ActionFor_GuildInstanceMap( SI32 siMapIndex, SI16 siMapType )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if( pclserver == NULL )							return;

	bool bEnd_InstanceMap = false;
	SI32 siParentMap = GetParentMap( siMapIndex );
	if( siParentMap <= 0 )							return;

	cltMapCommon* pclMap = GetMapPointer( siParentMap );
	if( pclMap == NULL )							return;
	if( pclMap->m_dwInstanceMapStartTime <= 0 )		return;


	// ����Ʈ ���͸� ������Ƿ� ����Ʈ ����
	cltCharServer* pclchar = NULL;

	if( pclMap->m_siInstanceMapGateIndex < 0 )
	{
		if( pclMap->m_siGateMonID > 0)
		{
			if( IsInstanceNPC(pclMap->m_siGateMonID) )		pclchar = (cltCharServer*)pclClient->pclCM->CR[pclMap->m_siGateMonID];
			if( pclchar == NULL || 
				pclchar->GetKind() != pclClient->GetUniqueFromHash("KIND_KEY_RACCOON") )
			{
				pclMap->m_siInstanceMapGateIndex = rand() % pclInstanceMapGateManager->m_siMaxSetCount;
				NTCHARStringBuilder		kbuild;
				kbuild.SetFormat("m_siInstanceMapGateIndex : {1}, Max : {2} \n") << pclMap->m_siInstanceMapGateIndex << pclInstanceMapGateManager->m_siMaxSetCount;
				pclClient->pclLog->LogPrint((TCHAR*)(const TCHAR*)kbuild);
				MakeInstanceMapGate( siParentMap );

				// �ʿ� �ִ� Ŭ���̾�Ʈ���� ����Ʈ ���� ����
				cltInstanceMapGate	clGates[MAX_INSTANCEMAPGATE_COUNT];

				SI32 siCount = pclClient->pclMapManager->pclInstanceMapGateManager->GetInstanceMapGates( 
					siParentMap, pclMap->m_siInstanceMapGateIndex,
					clGates, MAX_INSTANCEMAPGATE_COUNT );

				cltGameMsgResponse_InstanceMap_GateInfo clinfo(clGates, siCount);
				cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTMAP_GATEINFO, clinfo.GetSize(), (BYTE*)&clinfo);
				pclserver->pclCM->SendAllPersonInMapMsg( (sPacketHeader*)&clMsg, siParentMap );

				cltServerResponseValue clreturn(0, SRVAL_INSTANTDUNGEON_MAKEGATE, 0, 0);
				// Ŭ���̾�Ʈ�� ������ ������. 
				cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
				pclserver->pclCM->SendAllPersonInMapMsg( (sPacketHeader*)&clRtnMsg, siParentMap );
			}
		}
	}

	// ���� ���� ����� �ؼ� �δ� ����
	if( pclMap->m_siBossID > 0 )
	{
		pclchar = NULL;
		if( IsInstanceNPC(pclMap->m_siBossID) )			pclchar = (cltCharServer*)pclClient->pclCM->CR[pclMap->m_siBossID];

		if( pclchar == NULL || 
			pclchar->GetKind() != pclClient->GetUniqueFromHash("KIND_BIGRACCOON_AGIT") )
		{
			bEnd_InstanceMap = true;
		}
	}

	// �ð��� ����Ǿ� �δ� ����
	if( (DWORD)TABS(pclMap->m_dwInstanceMapStartTime - pclserver->CurrentClock) >= pclMap->m_dwInstanceMapMaxTime )
	{
		bEnd_InstanceMap = true;
	}

	SI32   siGuildMark = -1;
	if( bEnd_InstanceMap )
	{
		SI32 siInCharID = 0;
		LeaveInstanceMap( siMapIndex, &siInCharID );

		if ( pclClient->pclCM->IsValidID( siInCharID ) == true )
		{
			siGuildMark = pclClient->pclCM->CR[ siInCharID ]->pclCI->clBI.clGuildRank.siMarkIndex;
		}

		SI32 siClearTime = pclserver->CurrentClock - pclMap->m_dwInstanceMapStartTime;
		// DB�� ����� �����Ѵ�.
		sDBRequest_InstanceMapPoint_Insert clInsert( INSTANCEMAPPOINT_TYPE_GUILD, pclMap->m_siInstanceMapData, pclMap->m_siInstanceMapScore, pclserver->pclTime->clDate.uiYear, siClearTime, pclserver->sTime );
		pclClient->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clInsert);

		// 1���� ��ü�Ǵ��� Ȯ���Ѵ�.
		bool bChangeTop = false;
		if( pclClient->pclGuildManager->siTopClearPoint < pclMap->m_siInstanceMapScore )	bChangeTop = true;

		// 1�� ������ �����Ѵ�.
		if( bChangeTop )
		{
			pclClient->pclGuildManager->siTopClearPoint = pclMap->m_siInstanceMapScore;

			TCHAR* pGuildName  = pclserver->pclGuildManager->GetGuildName(pclMap->m_siInstanceMapData);
			SI32   siScore     = pclClient->pclGuildManager->siTopClearPoint;

			cltGuildInfo* GuildInfo = pclserver->pclGuildManager->GetGuildInfo(pGuildName);
			if(GuildInfo)
			{
				SI32 id	= pclClient->pclCM->GetIDFromPersonID(GuildInfo->siMasterPersonID);
				TCHAR* pName = NULL;
				if(pclClient->pclCM->IsValidID(id))
				{
					pName = (TCHAR*)pclClient->pclCM->CR[id]->GetName();
				}
				//[�߰� : Ȳ���� 2008. 1. 17 => �ְ� ��尡 ������� ��ü �������� �˸�.]
				cltGameMsgResponse_InstantDungeon_Top_Change sendMsg(pGuildName, siGuildMark, siScore, pName);
				cltMsg clMsg( GAMEMSG_RESPONSE_INSTANTSDUNGEON_TOP_CHANGE, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);	
			}
		}

		InitInstanceMap( siMapIndex );
		pclserver->pclNPCManager->DeleteNPCInstance( siParentMap );

		if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
		{
			CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();
			if ( pclSE_Quest )
			{
				pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_INSTANCEMAPCLEAR, siMapType, 1 );
			}
		}
	}
	else
	{
		cltGameMsgResponse_cInstantsDungeonRunInfo	clInfo(0, pclMap->m_siInstanceMapScore, pclClient->pclGuildManager->siTopClearPoint, DUNGEONRUN_UPDATE, siMapType);
		cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS, sizeof(clInfo), (BYTE*)&clInfo);
		SendInInstanceMapUser( siParentMap, (sPacketHeader*)&clMsg );
	}

}

void cltMapManager::ActionFor_PartyInstanceMap( SI32 siMapIndex, SI16 siMapType  )
{
	cltServer*	pclserver			= (cltServer*)pclClient;
	if( pclserver == NULL )							return;

	bool		bEnd_InstanceMap	= false;	// �δ��� ���� ��Ű�� ����
	SI32		siParentMap			= GetParentMap( siMapIndex );
	if( siParentMap <= 0 )							return;

	cltMapCommon* pclMap			= GetMapPointer( siParentMap );
	if( pclMap == NULL )							return;
	if( pclMap->m_dwInstanceMapStartTime <= 0 )		return;

	SI32		siAddBonusTime		= 0; // ���ʽ� ���͸� ����� �� �߰��Ǵ� �ð�
	SI32		siAddBonusCount		= 0; // ���ʽ� ���� ī����


	// ���ʽ� ���͸� ������ ���� 5���� �ð��� �߰��ȴ�
	if ( pclMap->m_siInstanceMapGateIndex < 0 )
	{
		for ( SI32	siIndex=0; siIndex<MAX_INSTANCE_BONUS_MONSTER_NUMBER; siIndex++ )
		{
			cltCharServer*	pclBonusChar		= NULL;
			SI32			siBonusMonsterID	= pclMap->m_siBonusMonID[siIndex];

			if ( siBonusMonsterID <= 0 ) continue;
			
			// ���ʽ� ���Ͱ� �ִ��� �������� Ȯ���ϱ� ���� ã�ƺ���
			if ( IsInstanceNPC(siBonusMonsterID) )
			{
				pclBonusChar = (cltCharServer*)pclClient->pclCM->CR[siBonusMonsterID];
			}

			// ���ʽ� ���͸� �ҷ����� ���Ѵٰų� ã�ƺ� ������ ī�ε尡 ���ʽ� ������ ī�ε�� �ٸ��� ���ʽ� Ÿ�� �߰�
			if ( (pclBonusChar == NULL) || (pclBonusChar->GetKind() != pclClient->GetUniqueFromHash("KIND_PTIMEBONUS")) )
			{
				siAddBonusTime				+= (INSTANCEMAP_PIRATESHIP_BONUSTIME);
				siAddBonusCount				+= 1;
			}
		} // for ( SI32	siIndex=0; siIndex<MAX_INSTANCE_BONUS_MONSTER_NUMBER; siIndex++ )
	} // if ( pclMap->m_siInstanceMapGateIndex < 0 ) ����

	pclMap->m_siBonusMonCount = siAddBonusCount;

	// ���� ���͸� ������ ������ ����
	if ( pclMap->m_siBossID > 0 )
	{
		cltCharServer* pclBossChar = NULL;

		if ( IsInstanceNPC(pclMap->m_siBossID) )
		{
			pclBossChar = (cltCharServer*)pclClient->pclCM->CR[pclMap->m_siBossID];
		}

		if ( (pclBossChar == NULL) || (pclBossChar->GetKind() != pclClient->GetUniqueFromHash("KIND_PIRATEMASTER")) )
		{
			bEnd_InstanceMap = true;
		}
	} // if ( pclMap->m_siBossID > 0 )

	// �⺻ 30�а� ���ʽ� ���͸� �������� 5���� ���� ���ڸ� ���� ���� ���ʽ� Ÿ������ �Ѵ�.
	if ( (DWORD)TABS(pclMap->m_dwInstanceMapStartTime - pclserver->CurrentClock) >= (pclMap->m_dwInstanceMapMaxTime+siAddBonusTime) ) 
	{
		bEnd_InstanceMap = true;
	}

	// ���� ��� ���͸� �׿��� �δ� ����
	if ( pclMap->m_siMaxMonsterNum <= pclMap->m_siKillCount )
	{
		bEnd_InstanceMap = true;
	}

	// �δ� ����
	if ( bEnd_InstanceMap == true )
	{
		pclClient->pclLog->FilePrint( TEXT("config/PirateShiplog.log"), TEXT("[END]\tPirateShip") );

		SI32	siInCharID		= 0;
		SI32	siPartyIndex	= 0;

		// �δ��� �ִ� ��Ƽ�� ��θ� ��������
		LeaveInstanceMap( siMapIndex, &siInCharID );

		// �δ��� �ִ� ��Ƽ�� �� �Ѹ��� ���̵� �̾Ƽ� ��Ƽ Index�� ã�Ƴ���
		if ( pclClient->pclCM->IsValidID( siInCharID ) == true )
		{
			siPartyIndex = pclClient->pclCM->CR[ siInCharID ]->siPartyIndex;
		}

		// �ʿ� �ִ� ���͸� ��� �׿��� �δ��� Ŭ�����Ѱ��̶�� ��Ƽ���߿� �Ѹ����� �������� �ش�
		if ( (pclMap->m_siMaxMonsterNum <= pclMap->m_siKillCount) && (siPartyIndex >= 0) && (siPartyIndex<MAX_PARTY_OBJ_NUM) )
		{
			pclClient->pclLog->FilePrint( TEXT("config/PirateShiplog.log"), TEXT("[CLEAR]\tPirateShip") );

			GiveItemForPartyMapClear( siPartyIndex );
			
			if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
			{
				CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();
				if ( pclSE_Quest )
				{
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_INSTANCEMAPCLEAR, siMapType, 1 );
				}
			}
		} // if ( (pclMap->m_siMaxMonsterNum <= pclMap->m_siKillCount) && (siPartyIndex >= 0) && (siPartyIndex<MAX_PARTY_OBJ_NUM) ) ����


		// �δ��� �ʱ�ȭ ��Ų��
		InitInstanceMap( siMapIndex );
		pclserver->pclNPCManager->DeleteNPCInstance( siParentMap );

	} // if ( bEnd_InstanceMap == true ) ����
	else
	{
		SI32	siKillCount		= (pclMap->m_siMaxMonsterNum - pclMap->m_siKillCount);
		SI32	siBonusCount	= pclMap->m_siBonusMonCount;

		cltGameMsgResponse_cInstantsDungeonRunInfo	clInfo(0, siKillCount, siBonusCount, DUNGEONRUN_UPDATE, INSTANCEMAP_TYPE_PARTY);
		cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS, sizeof(clInfo), (BYTE*)&clInfo);
		SendPartyInInstanceMapUser( pclMap->m_siInstanceMapData, (sPacketHeader*)&clMsg );
	}
}

void cltMapManager::ActionFor_VillageInstanceMap( SI32 siMapIndex, SI16 siMapType )
{
}

void cltMapManager::ActionFor_ServerInstanceMap( SI32 siMapIndex, SI16 siMapType )
{
	cltServer*	pclserver			= (cltServer*)pclClient;
	if( pclserver == NULL )							return;

	bool		bEnd_InstanceMap	= false;	// �δ��� ���� ��Ű�� ����
	SI32		siParentMap			= GetParentMap( siMapIndex );
	if( siParentMap <= 0 )							return;

	cltMapCommon* pclMap			= GetMapPointer( siParentMap );
	if( pclMap == NULL )							return;
	if( pclMap->m_dwInstanceMapStartTime <= 0 )		return;


	// �δ��� ��� ���͸� �׿����Ƿ� �ٽ� ���� ����
	if (pclMap->m_siMaxMonsterNum <= pclMap->m_siKillCount)
	{
		SI32			siIncGroupID	= pclMap->m_siNPCGroupID + 1;
		cltNPCGroup*	pclnpcgroup		= (cltNPCGroup*)pclserver->pclNPCManager->pclNPCGroup[ siIncGroupID ];


		if ( (NULL == pclnpcgroup) || (siMapIndex != pclnpcgroup->siMapIndex) )
		{
			for ( SI32 siGroupIndex=0; siGroupIndex<MAX_NPC_GROUP_NUMBER; siGroupIndex++ )
			{
				cltNPCGroup* pclSearchGroup = (cltNPCGroup*)pclserver->pclNPCManager->pclNPCGroup[ siGroupIndex ];

				if ( (NULL != pclSearchGroup) && (siMapIndex == pclSearchGroup->siMapIndex) )
				{
					siIncGroupID = siGroupIndex;
					break;
				}
			}
		}

		pclMap->m_siMaxMonsterNum	= 0;
		pclMap->m_siKillCount		= 0;
		pclMap->m_siNPCGroupID		= siIncGroupID;
		pclMap->m_siClearStep		+= 1;

		pclserver->pclNPCManager->MakeNPCInstance( siMapIndex, 0 );
	}

	// �δ� ���� �ð�
	if ( (DWORD)TABS(pclMap->m_dwInstanceMapStartTime - pclserver->CurrentClock) >= pclMap->m_dwInstanceMapMaxTime )
	{
		bEnd_InstanceMap = true;
	}

	// �δ� ����
	if ( bEnd_InstanceMap == true )
	{
		if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
		{
			SI32 siIndex = siMapIndex - MAPINDEX_SERVEREVENT_DUNGEON1;

			pclserver->pclServerEventManager->SetInstanceMapClear(siIndex, true);
			pclserver->pclServerEventManager->AddInstanceMapStepCount( siIndex, pclMap->m_siClearStep );
			
			if(true == pclserver->pclServerEventManager->IsAllInstanceMapClear())
			{
				pclserver->pclServerEventManager->ManagerExcute( SERVER_EVENT_COMMANDTYPE_INDUN_END );
			}
		}

		LeaveInstanceMap( siMapIndex );

		// �δ��� �ʱ�ȭ ��Ų��
		InitInstanceMap( siMapIndex );
		pclserver->pclNPCManager->DeleteNPCInstance( siParentMap );
		
	}
	else
	{
		SendServrEventInstanceMapInfo( siParentMap );
	}
}

// return  > 0 : ���� �ִ� ���� �ε���
//         = 0 : �ο��� �ʰ�
//         < 0 : ���̻� �� ���� ����
SI32 cltMapManager::GetEnterableInstanceMapIndex( SI32 siMapIndex, cltCharServer* pclchar )
{
	if( IsInstanceMap( siMapIndex ) == false )			return 0;
	if( pclchar == NULL )								return 0;
	if( pclClient->GameMode != GAMEMODE_SERVER )		return 0;

	SI32	siRetMapIndex = 0;

	SI32	siParentMapIndex = GetParentMap( siMapIndex );
	if( siParentMapIndex == 0 )							return 0;
	if( IsInstanceMap( siParentMapIndex ) == false )	return 0;

	SI32 siCharInstanceData = 0;
	// ���� Ÿ�Կ� ���� ĳ������ ������ �����Ѵ�.
	switch( clUnitMapInfo[siParentMapIndex].siInstanceMap )
	{
	case INSTANCEMAP_TYPE_GUILD:
		{
			if(pclchar->pclCI->clBI.clGuildRank.IsGuildMember() == true)
				siCharInstanceData = pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex;
		}
		break;
	case INSTANCEMAP_TYPE_PARTY:	siCharInstanceData = pclchar->siPartyIndex;								break;
	case INSTANCEMAP_TYPE_VILLAGE:	siCharInstanceData = pclchar->pclCI->clBI.siHomeVillage;				break;
	case INSTANCEMAP_TYPE_SERVER:	siCharInstanceData = siMapIndex;										break;
	}
	if( siCharInstanceData <= 0	)						return 0;

	cltMapCommon* pclParentMap = GetMapPointer( siParentMapIndex );
	if( pclParentMap == NULL)							return 0;

	bool bEnterable = false;

	// ����� ��� ��� �̸��� üũ�Ѵ�.
	if( clUnitMapInfo[siParentMapIndex].siInstanceMap == INSTANCEMAP_TYPE_GUILD)
	{
		if( pclParentMap->m_siInstanceMapData == siCharInstanceData && 
			strcmp(pclParentMap->m_szGuildName, pclchar->pclCI->clBI.clGuildRank.szGuildName) == 0 )
			bEnterable = true;
	}
	else
	{
		if( pclParentMap->m_siInstanceMapData == siCharInstanceData )
			bEnterable = true;
	}
	
	// 1.1 ���� �ʿ� ������ �� �ִ� ���� ã�´�.
	if( bEnterable )
	{
		if( GetUserLimit( siParentMapIndex ) > GetUserCountByParentMap( siParentMapIndex ) )
			siRetMapIndex = siParentMapIndex;
		else
            siRetMapIndex = -1;
	}

	// 1.2 ���� �ʿ� ������ �� �ִ� ���� ã�´�. - Ŭ�и�.
	if( siRetMapIndex <= 0 && clUnitMapInfo[siParentMapIndex].bHaveCloneMap )
	{
		SI32 cloneindex = clUnitMapInfo[siParentMapIndex].siCloneIndex;

		for ( SI32 i=0; i < CLONEMAP_NUM ; ++i )
		{
			SI32 dstMap = clCloneMapInfo[cloneindex].siDstMapIndex[i];
			if ( dstMap <= 0 )		continue;

			cltMapCommon* pclMap = GetMapPointer( dstMap );
			if( pclMap == NULL)		continue;

			bEnterable = false;
			// ����� ��� ��� �̸��� üũ�Ѵ�.
			if( clUnitMapInfo[dstMap].siInstanceMap == INSTANCEMAP_TYPE_GUILD)
			{
				if( pclMap->m_siInstanceMapData == siCharInstanceData && 
					strcmp(pclMap->m_szGuildName, pclchar->pclCI->clBI.clGuildRank.szGuildName) == 0 )
					bEnterable = true;
			}
			else
			{
				if( pclMap->m_siInstanceMapData == siCharInstanceData )
					bEnterable = true;
			}

			if( bEnterable )
			{
				if( GetUserLimit( dstMap ) > GetUserCountByParentMap( dstMap ) )
					siRetMapIndex = dstMap;
				else
					siRetMapIndex = -1;
			}
		}
	}

	// 1.3 ���� �ʿ� ������ ��� �� á��.
	if( siRetMapIndex < 0 )						return -1;

	// 2.1 ���� ������ �� �ִ� ���� ã�´�.
	if( siRetMapIndex <= 0 )
	{
		if( pclParentMap->m_siInstanceMapData == 0 )
		{
			siRetMapIndex = siParentMapIndex;
		}

		// 2.2 ���� ������ �� �ִ� ���� ã�´�. - Ŭ�и�.
		if( siRetMapIndex <= 0 && clUnitMapInfo[siParentMapIndex].bHaveCloneMap )
		{
			SI32 cloneindex = clUnitMapInfo[siParentMapIndex].siCloneIndex;

			for ( SI32 i=0; i < CLONEMAP_NUM ; ++i )
			{
				SI32 dstMap = clCloneMapInfo[cloneindex].siDstMapIndex[i];
				if ( dstMap <= 0 )		continue;

				cltMapCommon* pclMap = GetMapPointer( dstMap );
				if( pclMap == NULL)		continue;

				if( pclMap->m_siInstanceMapData == 0 )
				{
					siRetMapIndex = dstMap;
					break;
				}
			}
		}
	}
    
	return siRetMapIndex;
}

SI32 cltMapManager::GetUserCountByParentMap( SI32 mapindex )
{
	if( mapindex < 0 || mapindex >= MAX_MAP_NUMBER )	return 0;
	if( pclClient->GameMode != GAMEMODE_SERVER )		return 0;

	SI32 siParentMapIndex = pclClient->pclMapManager->GetParentMap(mapindex);
	if( siParentMapIndex < 0 || siParentMapIndex >= MAX_MAP_NUMBER )	return 0;

	SI32 userCount = 0;
	for ( SI32 i=0; i<MAX_PERSON_NUMBER; ++i )
	{
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[i];
		if ( pclClient->pclCM->IsValidID(i) )
		{
			// ĳ���Ͱ� �ִ� �� �ε���
			SI32 siCharParentMap = GetParentMap( pclchar->GetMapIndex() );
			if(siCharParentMap < 0 || siCharParentMap >= MAX_MAP_NUMBER)	continue;

			// ���� �ʿ� �ִ� ĳ������ ���� ����.
			if( siParentMapIndex != siCharParentMap )						continue;

			// ���� ĳ���� üũ - ���� ĳ������
			// KilledMode�� KILLEDMODE_INSTANCEMAP�� �ƴϸ�, ���� ���� ĳ�����̴�.
			if( pclClient->pclCM->IsAlive(i) == FALSE && pclchar->siKilledMode != KILLEDMODE_INSTANCEMAP )		continue;

			++userCount;
		}
	}
	return userCount;
}

SI32 cltMapManager::GetUserCountByMapIndex( SI32 siMapIndex )
{
	if ( (0 > siMapIndex) || (MAX_MAP_NUMBER <= siMapIndex) )
	{
		return 0;
	}
	
	if ( GAMEMODE_SERVER != pclClient->GameMode )		
	{
		return 0;
	}

	SI32 siUserCount = 0;

	for ( SI32 i=0; i<MAX_PERSON_NUMBER; ++i )
	{
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[i];
		if ( pclClient->pclCM->IsValidID(i) )
		{
			// ĳ���Ͱ� �ִ� �� �ε���
			SI32 siUSerMapIndex = pclchar->GetMapIndex();
			if ( (0 > siUSerMapIndex) || (MAX_MAP_NUMBER <= siUSerMapIndex) )	
			{
				continue;
			}

			// ���� �ʿ� �ִ� ĳ������ ���� ����.
			if( siMapIndex != siUSerMapIndex )						
			{
				continue;
			}

			// ���� ĳ���� üũ - ���� ĳ������
			// KilledMode�� KILLEDMODE_INSTANCEMAP�� �ƴϸ�, ���� ���� ĳ�����̴�.
			if( pclClient->pclCM->IsAlive(i) == FALSE && pclchar->siKilledMode != KILLEDMODE_INSTANCEMAP )		continue;

			++siUserCount;
		}
	}

	return siUserCount;
}

SI16 cltMapManager::GetInstanceMapType( SI32 siMapIndex )
{
	if ( IsInstanceMap( siMapIndex ) == false )
	{
		return INSTANCEMAP_TYPE_NONE;
	}

	return clUnitMapInfo[siMapIndex].siInstanceMap;
}

bool cltMapManager::EnterInstanceMap( SI32 parentmapindex, cltCharServer* pclchar )
{
	cltServer* pclserver = (cltServer*)pclClient;

	cltMapCommon* pclMap = GetMapPointer( parentmapindex );
	if( pclMap == NULL)									return false;
	if( pclchar == NULL)								return false;

	SI32 siCharInstanceData = 0;
	// ���� Ÿ�Կ� ���� ĳ������ ������ �����Ѵ�.
	switch( clUnitMapInfo[parentmapindex].siInstanceMap )
	{
	case INSTANCEMAP_TYPE_GUILD:	
		{
			// [�߰� : Ȳ���� 2008. 1. 25 => �δ� ����� ������� �˻�.]
			if( pclchar->pclCI->clBI.clGuildRank.IsGuildMember())
			{
				siCharInstanceData = pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex;	
			}
		}
		break;
	case INSTANCEMAP_TYPE_PARTY:	siCharInstanceData = pclchar->siPartyIndex;								break;
	case INSTANCEMAP_TYPE_VILLAGE:	siCharInstanceData = pclchar->pclCI->clBI.siHomeVillage;				break;
	case INSTANCEMAP_TYPE_SERVER:	siCharInstanceData = parentmapindex;									break;
	}
	if( siCharInstanceData <= 0	)						return false;

	// ������ �� ���� �ʿ� �����Ϸ��� �Ѵ�.
	if( pclMap->m_siInstanceMapData > 0 && pclMap->m_siInstanceMapData != siCharInstanceData)		return false;

	if( pclMap->m_siInstanceMapData == 0 )
	{
		pclMap->m_siInstanceMapData = siCharInstanceData;

		// �� ���� �ð�
		if ( pclserver->IsCountrySwitch(Switch_Server_Event) && (pclserver->pclServerEventManager) )
		{
			if ( INSTANCEMAP_TYPE_SERVER == clUnitMapInfo[parentmapindex].siInstanceMap )
			{
				pclMap->m_dwInstanceMapStartTime = pclserver->pclServerEventManager->GetInstanceMapInfo()->GetStartTime() ;
			}
		}
		
		// ���� �ð��� �����Ǿ����� ������ ���� �ð�����
		if ( 0 >= pclMap->m_dwInstanceMapStartTime )
		{
			pclMap->m_dwInstanceMapStartTime = pclClient->CurrentClock;
		}

		pclMap->m_dwInstanceMapMaxTime = pclMap->GetInstanceMapMaxTime();
		
		// ���� ���۵Ǿ����Ƿ� ���� ����
		pclserver->pclNPCManager->MakeNPCInstance( parentmapindex, 0, false );

		if( clUnitMapInfo[parentmapindex].siInstanceMap == INSTANCEMAP_TYPE_GUILD )
		{
			// ������ ��� ��� �̸��� ����
			StringCchCopy(pclMap->m_szGuildName, sizeof(pclMap->m_szGuildName), pclchar->pclCI->clBI.clGuildRank.szGuildName);

			//[�߰� : Ȳ���� 2008. 1. 14 => �ش� �������� �δ��� ������������ ��� ä��â���� ������.]
			TCHAR* pText = GetTxtFromMgr(7832);
			cltGameMsgResponse_Chat clChat(pclchar->GetCharUnique(), CHATMODE_GUILD, (TCHAR*)pclchar->GetName(), pText);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHAT, clChat.siDataSize, (BYTE*)&clChat);

			//KHY - 0131 - ��� ��� �޽��� ���� - �̸����� �˻� - ����ũ��ȣ �˻�����!
			pclClient->pclCM->SendAllGuildMemberMsg((sPacketHeader*)&clMsg, pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex,pclchar->pclCI->clBI.clGuildRank.szGuildName);
		}
	}
	
	return true;
}

void cltMapManager::InitInstanceMap( SI32 mapindex )
{
	cltMapCommon* pclMap = GetMapPointer( mapindex );
	if( pclMap == NULL)									return;

	pclMap->m_siInstanceMapData			= 0;
	pclMap->m_dwInstanceMapStartTime	= 0;
	pclMap->m_dwInstanceMapMaxTime		= pclMap->GetInstanceMapMaxTime();
	pclMap->m_siInstanceMapScore		= 0;
	pclMap->m_siInstanceMapGateIndex	= -1;
	pclMap->m_siGateMonID				= -1;
	pclMap->m_siBossID					= -1;
	pclMap->m_siMaxMonsterNum			= 0;
	pclMap->m_siKillCount				= 0;
	pclMap->m_siBonusMonCount			= 0;
	pclMap->m_siClearStep				= 0;
	pclMap->m_siNPCGroupID				= 0;
	
	ZeroMemory(pclMap->m_szGuildName, sizeof(pclMap->m_szGuildName));
#ifdef _SAFE_MEMORY
	pclMap->m_siBonusMonID.ZeroMem();
#else
	ZeroMemory(pclMap->m_siBonusMonID, sizeof(pclMap->m_siBonusMonID));
#endif
}


void cltMapManager::MakeInstanceMapGate( SI32 parentmapindex )
{
	if( IsInstanceMap( parentmapindex ) == false )					return;
	cltMapCommon* pclParentMap = GetMapPointer( parentmapindex );
	if( pclParentMap == NULL )										return;
	if( pclParentMap->m_siInstanceMapGateIndex < 0 )				return;

	SI32	i;
	for( i=0; i<MAX_TOTAL_INSTANCEMAPGATE_NUMBER; i++ )
	{
		cltInstanceMapGate* pclGate = pclInstanceMapGateManager->GetGatebyIndex(i, pclParentMap->m_siInstanceMapGateIndex);
		if( pclGate == NULL )								continue;
		if(	pclGate->siUnique <= 0)							continue;
		if(	pclGate->FOUnique <= 0)							continue;

		SI32 mapindex = pclGate->SrcMapIndex;
		if( mapindex <= 0 || mapindex >= MAX_MAP_NUMBER )	continue;
		if( GetParentMap( mapindex ) != parentmapindex )	continue;

		cltMapCommon* pclMap = GetMapPointer( mapindex );
		if( pclMap == NULL )								continue;

		// ������ ���� �����Ѵ�. 
		pclMap->DeleteFieldObject( pclGate->SrcX, pclGate->SrcY );
		// ���ο� ���� �����. 
		pclMap->PutFieldObject( pclGate->SrcX, pclGate->SrcY, pclGate->FOUnique, pclGate->siUnique );
	}	
}

// ������ ����Ʈ ���� ó�� �Լ�
void cltMapManager::PirateShipGateProcess()
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}

	UI16 uiNowMinute	= pclserver->sTime.wMinute;								// ���� �ð�(��)
	bool bOpenGate		= ((uiNowMinute == 0) || (uiNowMinute == 30));			// 30�� ������ ����Ʈ�� ������
	bool bCloseGate		= ((uiNowMinute == 10) || (uiNowMinute == 40));			// ����Ʈ�� ������ 10���� ������

	SI32 siGateKind		= pclClient->GetUniqueFromHash("KIND_PIRATESHIP_GATE");	// ����Ʈ ������ KIND


	if ( siGateKind <= 0 )
	{
		return;
	}

	if ( (bOpenGate == false) && (bCloseGate == false) )
	{
		return;
	}

	// 30�� ������ ���� �ʿ� ������ ����Ʈ�� �����ȴ�
	if ( bOpenGate && (m_siGatePositionID == 0) )
	{
		// ����Ʈ�� ��Ÿ���� ��ġ( ���߿��� �����ϰ� �Ѱ��� ��Ÿ�� )
		POINT pointGate[INSTANCEMAP_PIRATESHIP_MAX_GATE] = {
			{160,160}
			,{169,96}
			,{214,286}
			,{176,339}
			,{360,251}
			,{414,104}
			,{214,423}
			,{437,371}
			,{291,529}
			,{269,717}
			,{475,714}
			,{578,574}
		};
		
		// ���� ��ǥ�� �������� �����ϰ� ���� �̾� �´�
		SI32 siGateIndex = (SI32)(rand() % INSTANCEMAP_PIRATESHIP_MAX_GATE);
		if ( (siGateIndex < 0) || (siGateIndex >= INSTANCEMAP_PIRATESHIP_MAX_GATE) )
		{
			return;
		}
		
		// ������ �̾ƿ� ����Ʈ�� X,Y��ǥ
		SI32 siX = (SI32)pointGate[siGateIndex].x;
		SI32 siY = (SI32)pointGate[siGateIndex].y;

		// ������ ����Ʈ ����
		m_siGatePositionID = pclserver->SetNPCChar(siGateKind, VILLAGE_NONE, NULL, 1, 0, siX, siY, 0, NPCITEMMODE_NONE, NULL );

		// �������� ��Ÿ���ٴ� �޽����� ����
		if ( m_siGatePositionID	> 0 )
		{
			//cltMsg clMsg( GAMEMSG_RESPONSE_PIRATESHIP_APPEAR, 0, NULL );
			//pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

			// �������� ���
			cltServerResponseValue clreturn(0, SRVAL_PIRATESHIP_APPEAR, siX, siY);
			// Ŭ���̾�Ʈ�� ������ ������. 
			cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
			
			pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clRtnMsg );

		}
	}

	// ����Ʈ�� �����ȵ� 10���� ����Ʈ�� �������
	if ( bCloseGate && (m_siGatePositionID > 0) )
	{	
		// ����Ʈ�� �����ɶ� �޾ƿ� ���̵�� ������ �����Ѵ�
		if ( pclserver->pclCM->IsValidID( m_siGatePositionID ) == false )
		{
			return;
		}
		
		// �ش� ���̵��� �ɸ��Ͱ� ���ٸ� ����
		cltCharServer* pclGateChar = (cltCharServer*)pclClient->pclCM->CR[m_siGatePositionID];
		if ( pclGateChar == NULL )
		{
			return;
		}
		
		// �ش� ���̵��� �ɸ��͸� �޾ƿ����� ����Ʈ�� �ƴ϶�� ����
		if ( pclGateChar->GetKind() != siGateKind )
		{
			return;
		}
		
		// �ش� ����Ʈ�� �����ϰ�
		pclGateChar->DeleteInCell();

		// ������ �Ϸ�Ǿ����� ����Ʈ ���̵� �ٽ� �ʱ�ȭ��Ŵ
		if ( pclserver->pclCM->DeleteChar( m_siGatePositionID ) == TRUE )
		{
			m_siGatePositionID = 0;
		}
	}
}

//KHY - 1113 - ũ�������� �̺�Ʈ
void cltMapManager::ChirstmasEventGateProcess()
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL)
		return;

	cltCharServer* pclGateChar = (cltCharServer*)pclClient->pclCM->CR[m_siChristmasGatePositionID];
	if(pclGateChar != NULL) // �̹� �ִ�.
		return;


	SI32 siGateKind		= pclClient->GetUniqueFromHash("KIND_SANTAGATE");	// ����Ʈ ������ KIND


	if ( siGateKind <= 0 )
	{
		return;
	}

	// ����Ʈ�� ��Ÿ���� ��ġ
	POINT pointGate[1] = {
		{292,327}
	};
	
	// ���� ��ǥ�� �������� ���� �̾� �´�
	SI32 siGateIndex = 0;
	
	// ������ �̾ƿ� ����Ʈ�� X,Y��ǥ
	SI32 siX = (SI32)pointGate[siGateIndex].x;
	SI32 siY = (SI32)pointGate[siGateIndex].y;

	//  ����Ʈ ����
	m_siChristmasGatePositionID = pclserver->SetNPCChar(siGateKind, VILLAGE_NONE, NULL, 1, 0, siX, siY, 0, NPCITEMMODE_NONE, NULL );
/*
		//�޽����� ����
		if ( m_siGatePositionID	> 0 )
		{
			//cltMsg clMsg( GAMEMSG_RESPONSE_PIRATESHIP_APPEAR, 0, NULL );
			//pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

			// �������� ���
			cltServerResponseValue clreturn(0, SRVAL_PIRATESHIP_APPEAR, siX, siY);
			// Ŭ���̾�Ʈ�� ������ ������. 
			cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
			
			pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clRtnMsg );

		}
*/
}

//���� - 2009.8.28 - ���� �̺�Ʈ
void cltMapManager::FestivalEventGateProcess(BOOL bBeingNPC)
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL)
		return;

	cltCharServer* pclGateChar = (cltCharServer*)pclClient->pclCM->GetCharServer(m_siFestivalGatePositionID);

	SI32 siGateKind		= pclClient->GetUniqueFromHash("KIND_FESTIVALGATE");	// ����Ʈ ������ KIND

	if ( siGateKind <= 0 )
	{
		return;
	}

	if( TRUE == bBeingNPC)
	{
		if(pclGateChar != NULL) // �̹� �ִ�.
			return;
	}
	else
	{
		if(pclGateChar != NULL)
		{
			// ����Ʈ�� �ƴ϶�� 
			if(pclGateChar->GetKind() != siGateKind)
			{
				return;
			}
			pclGateChar->SetLife(0);
			m_siFestivalGatePositionID = 0;

			// ���� �̺�Ʈ �ʿ� �ִ� ����ڸ� ���縮���� ���� ��Ų��.
			/*cltCharServer* pclCharServer = NULL;
			for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
			{

				pclCharServer = pclClient->pclCM->GetCharServer(id);
				if( NULL == pclCharServer)
					continue;

				if(  MAPINDEX_FESTIVAL == pclCharServer->GetMapIndex())
				{
					SI32		siWarpX = 288;
					SI32		siWarpY = 354;
					cltWarpInfo clWarp(1, MAPINDEX_MAINFIELD, false, false, siWarpX, siWarpY, -1, -1);

					pclCharServer->SetWarpInfo( &clWarp, 1024 );
					pclCharServer->Warp();
				}
			}*/
		}
		return;
	}
	
	// ����Ʈ�� ��Ÿ���� ��ġ
	POINT pointGate[1] = {
		{243,322}
	};

	// ���� ��ǥ�� �������� ���� �̾� �´�
	SI32 siGateIndex = 0;

	// ������ �̾ƿ� ����Ʈ�� X,Y��ǥ
	SI32 siX = (SI32)pointGate[siGateIndex].x;
	SI32 siY = (SI32)pointGate[siGateIndex].y;

	//  ����Ʈ ����
	m_siFestivalGatePositionID = pclserver->SetNPCChar(siGateKind, VILLAGE_NONE, NULL, 1, 0, siX, siY, 0, NPCITEMMODE_NONE, NULL );
}


// �δ� ����� �δ��� �ִ� �ɸ��� ��� ������ ������ �Լ�
void cltMapManager::LeaveInstanceMap( IN SI32 siMapIndex, OUT SI32* psiInCharID/* =NULL  */)
{
	cltServer* pclserver = (cltServer*)pclClient;
	if( pclserver == NULL )							return;

	SI32 siParentMap = GetParentMap( siMapIndex );
	if( siParentMap <= 0 )							return;

	cltMapCommon* pclMap = GetMapPointer( siParentMap );
	if( pclMap == NULL )							return;
	if( pclMap->m_dwInstanceMapStartTime <= 0 )		return;

	SI32 siMapType = GetInstanceMapType( siParentMap );

	// �ʿ� �ִ� ĳ���͸� �� ������ �̵���Ų��.
	SI32 index = 0;
	SI32 id;

	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer* )pclClient->pclCM->CR[id];

		if( !IsPC(id) ) continue;																// ����
		if( GetParentMap(pclClient->pclCM->CR[id]->GetMapIndex()) != siParentMap)	continue;	// �δ��� ����.

		SI32 x = 0 , y = 0;
		cltWarpInfo clWarpInfo(1, 0, false, true, x, y, -1, -1);

		if (pclGateManager->FindMainFieldPos( MAPINDEX_PALACE, &x, &y ) == false )
			return;

		clWarpInfo.uiMapIndex = 0; //������ ���� �� ���´�.
		clWarpInfo.uiX = x+2;
		clWarpInfo.uiY = y+2;

		// ���縮�� ������ �̵� ��Ų��.
		if (pclClient->pclCM->IsAlive(id) == FALSE)
		{
			pclchar->siKilledMode = 0;
		}
		else
		{
			SI32 siVillageMapIndex	= pclchar->pclCI->clBI.siHomeVillage;

			if ( pclserver->IsCountrySwitch(Switch_Server_Event) && (0 < siVillageMapIndex))
			{
				SI32 siMapTypeUnique	= pclserver->pclMapManager->pclMapArray[ siVillageMapIndex ]->siMapUnique;
				UI32 uiX				= (UI32)pclserver->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ siMapTypeUnique ]->siCenterX;
				UI32 uiY				= (UI32)pclserver->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ siMapTypeUnique ]->siCenterY;
				
				clWarpInfo.uiMapIndex	= (UI32)siVillageMapIndex;
				clWarpInfo.uiX			= uiX;
				clWarpInfo.uiY			= uiY;
			}

			pclchar->SetWarpInfo(&clWarpInfo, 3000);
			pclchar->Warp();
		}
		// �δ� �� ��Ȳ�� ����
		switch ( siMapType )
		{
			case INSTANCEMAP_TYPE_GUILD:
				{
					cltGameMsgResponse_cInstantsDungeonRunInfo	clInfo(0, pclMap->m_siInstanceMapScore, pclClient->pclGuildManager->siTopClearPoint, DUNGEONRUN_END, siMapType);
					cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS, sizeof(clInfo), (BYTE*)&clInfo);

					pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
				}
				break;

		}
		
		// �ش� �ʿ� �ִ� ĳ���� ���̵� ������( �Ǹ������� ã�Ƴ� ĳ���� )
		if ( psiInCharID != NULL )
		{
			*psiInCharID = id;
		}
	} // while(id = pclClient->pclCM->GetCharOrder(index)) ����
}

// ��Ƽ���� ����Ʈ�� ���ϰ� ���ϰ����� ��Ƽ���� ������� �� �� �ִ�
SI08 cltMapManager::GetInstanceMapPartyList( IN SI32 siPartyIndex, OUT SI32* psiPartyList )
{
	if ( siPartyIndex <= 0 )		return -1;
	if ( psiPartyList == NULL )		return -1;

	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )		return -1;


	CPartyObj* pclparty = pclClient->pclPartyMgr->GetPartyObj( siPartyIndex );
	if ( pclparty == NULL )			return -1;

	SI08 siTotalPartyNum = 0;


	// ��ȿ�� ��Ƽ���� ����Ʈ�� ���Ѵ�. 
	for ( SI32 siPartyNum=0; siPartyNum<MAX_PARTY_USER_NUM; siPartyNum++ )
	{
		// ��Ƽ���� �Ѹ��� �̾Ƴ���
		stPartyUser* pstPartyMember = pclparty->GetPartyUser(siPartyNum);

		// ��Ƽ���� ���̾Ƴ°ų� ��Ƽ���°� �ƴ϶�� ����
		if ( (pstPartyMember == NULL) || (pstPartyMember->bPartyStatus == false) )
		{
			continue;
		}
		
		// ���� �̾ƿ� ��Ƽ���� ���̵� ��ȿ���� �ʴٸ� ����
		if ( pclClient->pclCM->IsValidID( pstPartyMember->siCharUnique ) == false )
		{
			continue;
		}
		
		// �̾ƿ� ��Ƽ ����Ʈ�� PersonID�� ������ �����ϴ� �ɸ����� PersonID�� �ٸ��� ����
		if ( pclClient->pclCM->CR[ pstPartyMember->siCharUnique ]->pclCI->GetPersonID() != pstPartyMember->siPersonID )
		{
			continue;
		}

		psiPartyList[ siTotalPartyNum ] = pstPartyMember->siCharUnique;
		siTotalPartyNum++;
	} // for ( SI32 siPartyNum=0; siPartyNum<MAX_PARTY_USER_NUM; siPartyNum++ ) ����

	return siTotalPartyNum;

}

// [����] �δ��ȿ� �ִ� �������� ��Ŷ�� ������
void cltMapManager::SendInInstanceMapUser( SI32 siParentMap, sPacketHeader* psPacketHeader )
{
	if ( psPacketHeader == NULL )								return;
	if ( (siParentMap < 0) || (MAX_MAP_NUMBER<siParentMap) )	return;

	SI32 siIndex	= 0;
	SI32 siID		= 0;


	while(siID = pclClient->pclCM->GetCharOrder(siIndex))
	{
		siIndex++;

		// ��ȿ�� ���̵� �̾ƿ´�
		if ( pclClient->pclCM->IsValidID( siID ) == false )
		{
			continue;
		}

		cltCharServer* pclchar = (cltCharServer* )pclClient->pclCM->CR[siID];
        if ( pclchar == NULL )
		{
			continue;
		}
		// �δ��� ����
		if( GetParentMap(pclchar->GetMapIndex()) != siParentMap )
		{
			continue;
		}

		pclchar->SendNetMsg( psPacketHeader );
	}
}

// [����] ��Ƽ�δ� �� ���� �ִ� �������� ��Ŷ�� ������
void cltMapManager::SendPartyInInstanceMapUser( SI32 siPartyIndex, sPacketHeader* psPacketHeader )
{
	if ( psPacketHeader == NULL )								return;
	if ( (siPartyIndex<0) || (MAX_MAP_NUMBER<siPartyIndex) )	return;

	SI32 siPartyList[ MAX_PARTY_USER_NUM ]	= {0,};
	SI08 siTotalPartyNum					= 0;


	ZeroMemory( siPartyList, sizeof(siPartyList) );

	siTotalPartyNum = GetInstanceMapPartyList( siPartyIndex, siPartyList );
	if ( siTotalPartyNum <= 0 )									return;

	for ( SI08 siIndex=0; siIndex<siTotalPartyNum; siIndex++ )
	{
		SI32 siPartyID = siPartyList[siIndex];
		if ( pclClient->pclCM->IsValidID( siPartyID ) == FALSE )
		{
			continue;
		}

		cltCharServer* pclPartyChar = (cltCharServer*)pclClient->pclCM->CR[siPartyID];
		if ( pclPartyChar == NULL )
		{
			continue;
		}

		if ( IsPartyMap(pclPartyChar->GetMapIndex()) == false )
		{
			continue;
		}

		pclPartyChar->SendNetMsg( psPacketHeader );
	}

}

// [����] �̸��� ��Ƽ �δ� ������ üũ
bool cltMapManager::IsPartyMap( SI32 siMapIndex )
{
	if ( (siMapIndex < 0) || (MAX_MAP_NUMBER<siMapIndex) )
	{
		return false;
	}

	if ( IsInstanceMap( siMapIndex ) == false )
	{
		return false;
	}

	SI32 siParentMapIndex = GetParentMap( siMapIndex );

	if ( GetInstanceMapType( siParentMapIndex )  != INSTANCEMAP_TYPE_PARTY )
	{
		return false;
	}

	return true;
}

void cltMapManager::GiveItemForPartyMapClear( SI32 siPartyIndex )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}

	bool			bGiveItem							= false;
	SI32			siPartyList[ MAX_PARTY_USER_NUM ]	= {0,};
	SI08			siTotalPartyNum						= 0;
	SI32			siGivePartyMemberIndex				= -1;

	SI32			siGiveCharID						= 0;
	SI32			siGivePersonID						= 0;
	cltCharServer*	pclchar								= NULL;


	ZeroMemory( siPartyList, sizeof(siPartyList) );

	// �� ĳ���� ���̵� �̾Ƴ���
	siTotalPartyNum = GetInstanceMapPartyList( siPartyIndex, siPartyList );

	if ( (siTotalPartyNum > 0) && (siTotalPartyNum <= MAX_PARTY_USER_NUM) )
	{
		siGivePartyMemberIndex = (SI32)(rand() % siTotalPartyNum);

		if ( (siGivePartyMemberIndex >= 0) && (siGivePartyMemberIndex < siTotalPartyNum) )
		{
			bGiveItem = true;
		}
	}

	if ( bGiveItem == false )
	{
		return;
	}
	//----------------------------------------------------------------------------------------------------
	siGiveCharID = siPartyList[ siGivePartyMemberIndex ];
	if ( pclClient->pclCM->IsValidID( siGiveCharID ) == false )
	{
		return;
	}

	pclchar = (cltCharServer*)pclClient->pclCM->CR[ siGiveCharID ];
	if ( pclchar == NULL )
	{
		return;
	}

	siGivePersonID = pclchar->pclCI->GetPersonID();
	//----------------------------------------------------------------------------------------------------
	bool		bMakeResult			= false;				// ������ �������
	bool		bReferenceCheck		= false;				// ������ ������ ã�� ���

	cltItem		clRewardItem;								// ������ ������
	SI32		siRewardItemUnique	= ITEMUNIQUE( 18527 );	// ������ ���� 

	SI32		siReference			= pclClient->pclItemManager->FindItemRefFromUnique( siRewardItemUnique );

	SI16		siUsedate			= 0;
	SI16		siRareswitch		= 0;


	if ( siReference > 0 )
	{
		siUsedate			= pclClient->pclItemManager->pclItemInfo[ siReference ]->clItem.uiDateUseDay;

		bReferenceCheck		= true;
		bMakeResult			= pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique, &clRewardItem, 0, 0, &siRareswitch,0,0,0,0,0,siUsedate );
	}

	if ( (bMakeResult == true) && (bReferenceCheck == true) )
	{
		pclClient->pclLog->FilePrint( TEXT("config/PirateShiplog.log"), TEXT("[GiveItem]\tPirateShip\tPersonID:%d\tItemUnique:%d\tItemNum:%d"), siGivePersonID, clRewardItem.siUnique, clRewardItem.siItemNum );

		cltSystemRewardInfo		clInfo;			// ������ ���� ���� ����
		//NDate					kExpireDate;	// �������� ������ ���� �ӽ� ����
		//_SYSTEMTIME				sExpireTime;	// �������� ������ ���� �ӽ� ����2
		TCHAR*					pReasonText	= GetTxtFromMgr( 8079 );	// �����ϴ� ����(PC�� �����̾�)


		// ������ ����(���޵� ������(+1��)�� ����)
		//kExpireDate.SetDate( pclserver->sTime.wYear, pclserver->sTime.wMonth, pclserver->sTime.wDay );
		//kExpireDate.AddMonths( 1 );

		//ZeroMemory( &sExpireTime, sizeof(_SYSTEMTIME) );
		//sExpireTime.wYear		= kExpireDate.m_uiYear;
		//sExpireTime.wMonth		= kExpireDate.m_uiMonth;
		//sExpireTime.wDay		= kExpireDate.m_uiDay;

		// ���� ��¥
		clInfo.m_sGiveTime		= pclserver->sTime;
		// ���� ��¥ - ���޹��� ������ 30�� ���� ������ �� �ִ�.
		//clInfo.m_sExpireTime	= sExpireTime;
		TimeAddDate(&clInfo.m_sGiveTime, 30, &clInfo.m_sExpireTime);
		// ������ ������
		clInfo.m_clItem.Set( &clRewardItem );
		// ���� ����
		if ( pReasonText )
		{
			StringCchCopy( clInfo.m_szReason, 256, pReasonText );
		}

		// �������� ���� �϶�� ��û
		sDBRequest_GiveSystemReward clMsgReward( GIVEREWARD_CHAR, siGivePersonID, siGiveCharID, &clInfo );
		pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgReward);


		// �������� ���޵Ǿ��ٴ� �޽����� �����
		pclserver->SendServerResponseMsg( 0, SRVAL_INSTANTDUNGEON_CLEAR_REWARD_ITEM, 0, 0, siGiveCharID );
		


		// �������� Ŭ���� �ؼ� �������� �޾Ҵٴ� �޽����� ��� Ŭ���̾�Ʈ�� �Ѹ�
		NTCHARString64	kUserName( pclchar->GetName() );

		cltGameMsgResponse_InstantDungeon_Party_Clear sendMsg( kUserName, siRewardItemUnique );
		cltMsg clMsg( GAMEMSG_RESPONSE_INSTANTDUNGEON_PARTY_CLEAR, sizeof(sendMsg), (BYTE*)&sendMsg );
		pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

	}

}

// [����] ������ �� �� �ִ� �����ΰ�?
bool cltMapManager::IsCanMarketOpenMap( SI32 siMapIndex )
{
	if( siMapIndex < 0 || siMapIndex >= MAX_MAP_NUMBER )return false;

	return clUnitMapInfo[siMapIndex].bCanMarket;

}

void cltMapManager::SendServrEventInstanceMapInfo( SI32 siParentMap )
{
	if ( (siParentMap < 0) || (MAX_MAP_NUMBER<siParentMap) )
	{
		return;
	}

	cltMapCommon* pclMap = GetMapPointer( siParentMap );
	if ( NULL == pclMap )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	SI16	siServerUnique	= pclClient->siServerUnique;
	SI32	siClearStep		= pclMap->m_siClearStep;
	DWORD	dwRemainTime	= (pclMap->m_dwInstanceMapStartTime+pclMap->m_dwInstanceMapMaxTime) - pclClient->CurrentClock;
	SI32	siRemainMonster = pclMap->m_siMaxMonsterNum - pclMap->m_siKillCount;
	SI64	siScore			= 0;
	SI32	siDungeonScore	= pclMap->m_siInstanceMapScore;


	SI32 siIndex	= 0;
	SI32 siID		= 0;


	while(siID = pclClient->pclCM->GetCharOrder(siIndex))
	{
		siIndex++;

		// ��ȿ�� ���̵� �̾ƿ´�
		if ( pclClient->pclCM->IsValidID( siID ) == false )
		{
			continue;
		}

		if ( IsPC( siID ) == false )
		{
			continue;
		}

		cltCharServer* pclchar = (cltCharServer* )pclClient->pclCM->CR[siID];
		if ( pclchar == NULL )
		{
			continue;
		}

		
		// �δ��� ����
		if( GetParentMap(pclchar->GetMapIndex()) != siParentMap )
		{
			continue;
		}

		siScore = pclchar->siServerEvent_IndunScore;
		
		// Ŭ���̾�Ʈ���� ���� ����
		cltGameMsgResponse_Server_Event_InstanceDungeon_RunInfo	clInfo( siServerUnique, siClearStep, dwRemainTime, siRemainMonster, siScore, siDungeonScore );
		cltMsg clMsg(GAMEMSG_RESPONSE_SERVER_EVENT_INSTANCEDUNGEON_RUNINFO, sizeof(clInfo), (BYTE*)&clInfo);
		pclchar->SendNetMsg( (sPacketHeader*)&clMsg );

		if ( 0 < siScore )
		{
			// DB�� ���� ���� ����
			sDBRequest_Server_Event_Server_Person_Score_Set clScore( siID, pclchar->pclCI->GetPersonID(), siScore );
			pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clScore );
		}
	
	}
}
