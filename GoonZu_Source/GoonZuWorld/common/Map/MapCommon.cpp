//---------------------------------
// 2003/9/5 ���°�
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"

#include "map.h"
#include "..\Smoke\Smoke.h"
#include "..\Char\KindInfo\KindInfo.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

#include "..\..\Lib\AStar\AStarListMgr.h"
#include "..\..\Client\SmallMap\SMallMap.h"

extern cltCommonLogic* pclClient;


//-----------------------------------------------------
// cltMapCommon
//-----------------------------------------------------
cltMapCommon::cltMapCommon()
{
	Init();
}

cltMapCommon::cltMapCommon(SI32 mapindex, SI32 mapunique, SHORT xsize, SHORT ysize, SI32 tileset1, SI32 tileset2, cltCharManager* pclcm, cltTileManager* pcltileset, cltFieldObjectManager* pclfieldobjectmanager)
{
	Init();

	int i;/*, j;*/

	MH.Version			=	1;
	MH.MapType			=	0;

	siMapIndex			=	mapindex;

	siMapUnique			=	mapunique;

	pclTileManager		=	pcltileset;
	pclFieldObjectManager	=	pclfieldobjectmanager;
	pclCM				=	pclcm;

	MH.MapXsize			=	xsize;
	MH.MapYsize			=	ysize;

	FreeMap();

	AllocMap(MH.MapXsize, MH.MapYsize);

	for(i=0;i<MH.MapYsize;i++)
	{
		YOff[i] = i* MH.MapXsize;
	}

	pclPath				= new CAStarListMgr(this);
	pclPath->IsBlocking	= IsMoveColl;



	// ����Ʈ Ÿ�� ��Ʈ�� ����Ѵ�.
	SI32 unique1, unique2;
	if(tileset1)
	{
		unique1 = tileset1;
	}
	else
	{
		unique1 = 1;	// "Tile_Default1"
	}

	if(tileset2)
	{
		unique2 =tileset2;
	}
	else
	{
		unique2 = 90;	//"Tile_Default2"
	}

	//-----------------------------------------------------------------------------------------
	// �����ս� ��� - �ʱ�ȭ�ÿ� �����ɷ��� �ڵ带 ������.
	//-----------------------------------------------------------------------------------------
	memset( pclLayer[0]->pUnique, unique1, sizeof(UI08) * MH.MapYsize * MH.MapXsize );
	memset( pclLayer[1]->pUnique, unique2, sizeof(UI08) * MH.MapYsize * MH.MapXsize );

	DWORD dwDrawInfo0 = 0;
	DWORD dwDrawInfo1 = 0;
	SI32 siref = 0;

	dwDrawInfo0 |= unique1 << 24;
	siref = pclTileManager->FindRefFromUnique_TileSet(unique1);
	dwDrawInfo0 |= siref << 16;

	dwDrawInfo1 |= unique2 << 24;
	siref = pclTileManager->FindRefFromUnique_TileSet(unique2);
	dwDrawInfo1 |= siref << 16;

	for(i=0;i<MH.MapYsize * MH.MapXsize;i++)
	{
		pclLayer[0]->pDrawInfo[i] = dwDrawInfo0;
		pclLayer[1]->pDrawInfo[i] = dwDrawInfo1;
	}
	//-----------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// �����ս� ��� �����ڵ�
	//-----------------------------------------------------------------------------------------
	//for(i=0;i<MH.MapYsize;i++)
	//	for(j=0;j<MH.MapXsize;j++)
	//	{
	//		// Layor 0 
	//		pclLayer[0]->SetUnique(j, i, unique1);
	//		pclLayer[0]->SetDrawInfo_Unique(j, i, unique1);
	//		SI32 siref = pclTileManager->FindRefFromUnique_TileSet(unique1);
	//		pclLayer[0]->SetDrawInfo_Ref(j, i, siref);
	//		pclLayer[0]->SetDrawInfo_File(j, i, 0 );
	//		pclLayer[0]->SetDrawInfo_Font(j, i, 0);

	//		// Layor 1 
	//		pclLayer[1]->SetUnique(j, i, unique2);
	//		pclLayer[1]->SetDrawInfo_Unique(j, i, unique2);
	//		siref = pclTileManager->FindRefFromUnique_TileSet(unique2);
	//		pclLayer[1]->SetDrawInfo_Ref(j, i, siref);
	//		pclLayer[1]->SetDrawInfo_File(j, i, 0 );
	//		pclLayer[1]->SetDrawInfo_Font(j, i, 0);
	//	}
	//-----------------------------------------------------------------------------------------
		
	// ���� �ȳ� ���� ����. 
	pclMapGeoInfo	= new cltMapGeoInfo(this,  MH.MapXsize, MH.MapYsize);

	ValidRect.top		= 0;
	ValidRect.left		= 0;
	ValidRect.right		= MH.MapXsize - 1;
	ValidRect.bottom	= MH.MapYsize - 1;
}

cltMapCommon::~cltMapCommon()
{
	SI32 i;

	// ���� �ȳ� ���� ���� 

	NDelete(pclMapGeoInfo);
	NDelete(pclPath);

	NDelete_Array(pclItemMap)
	NDelete_Array(pFieldObjectMapPara)
	NDelete_Array(pFieldObjectMap);

	for(i = 0;i < MAX_MAP_LAYER;i++)
	{
		NDelete(pclLayer[i]);
	}

	NDelete_Array(pCollMap)
	NDelete_Array(pIDMap)
	NDelete_Array(YOff);
}

void cltMapCommon::Init()
{
	pIDMap				= NULL;
	pCollMap			= NULL;  

	pFieldObjectMap		= NULL;  
	pFieldObjectMapPara	= NULL;

	pclItemMap			= NULL;

	pclMapGeoInfo		= NULL;

	pclTileManager		= NULL;
	pclFieldObjectManager	= NULL;

	pclCM				= NULL;

	siVillageUnique		= 0;
	
	pclPath				= NULL;
	MoveID				= 0;

	YOff				= NULL;

	for(SI32 i = 0; i < MAX_MAP_LAYER;i++)
	{
		pclLayer[i]	= NULL;
	}

	m_siInstanceMapData			= 0;
	m_dwInstanceMapStartTime	= 0;
	m_siInstanceMapScore		= 0;
	m_siInstanceMapGateIndex	= -1;
	m_siGateMonID				= -1;
	m_siBossID					= -1;
	m_siMaxMonsterNum			= 0;
	m_siKillCount				= 0;
	m_siBonusMonCount			= 0;
	m_siClearStep				= 0;
	m_siNPCGroupID				= 0;

	ZeroMemory(m_szGuildName, sizeof(m_szGuildName));
#ifdef _SAFE_MEMORY
	m_siBonusMonID.ZeroMem();
#else
	ZeroMemory(m_siBonusMonID, sizeof(m_siBonusMonID));
#endif
}

void cltMapCommon::AllocMap(SHORT xsize, SHORT ysize)
{

	SI32 i;

	YOff = new SI32[ MH.MapYsize ];

	pIDMap = new short[ xsize * ysize ];
	if( pIDMap ) ZeroMemory(pIDMap, sizeof(short) * xsize * ysize );

	pCollMap = new short[ xsize * ysize ];
	if( pCollMap ) ZeroMemory(pCollMap, sizeof(short) * xsize * ysize );

	for( i = 0;i < MAX_MAP_LAYER; ++i )
	{
		pclLayer[i] = new cltMapLayer(xsize, ysize, YOff);
	}
		

	pFieldObjectMap = new DWORD[ xsize * ysize ];
	if( pFieldObjectMap ) ZeroMemory( pFieldObjectMap, sizeof(DWORD) * xsize * ysize );

	pFieldObjectMapPara = new DWORD[xsize * ysize];
	if( pFieldObjectMapPara ) ZeroMemory( pFieldObjectMapPara, sizeof(DWORD) * xsize * ysize );

	pclItemMap			= new cltItemMapInfo[xsize*ysize];
	if(pclItemMap)ZeroMemory(pclItemMap, sizeof(cltItemMapInfo)*xsize*ysize);


}

void cltMapCommon::FreeMap()
{
	SI32 i, j;
	SI32 unique;

	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			unique = GetFieldObject_Unique(j, i);
			if(unique && GetFieldObjectLengthToBottom(j, i ) ==0 && GetFieldObjectLengthToRight(j, i) ==0)
			{
				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(unique);
				if(ref >= 0)
				{
					SI32 type = pclFieldObjectManager->pclInfo[ref]->dwType;
				}

			}

		}


	NDelete(pclPath);
	NDelete_Array(pclItemMap);
	NDelete_Array(YOff);
	NDelete_Array(pIDMap);
	NDelete_Array(pFieldObjectMapPara);
	NDelete_Array(pFieldObjectMap);
	NDelete_Array(pCollMap)
	
		for(i = 0;i < MAX_MAP_LAYER;i++)
	{
		NDelete(pclLayer[i]);
	}
}


bool cltMapCommon::IsEmpty(SI32 x, SI32 y)
{
	if( x<0  || x>=MH.MapXsize )	return FALSE;
	if( y<=0 || y>=MH.MapYsize )	return FALSE;
	if( pIDMap == NULL )	return FALSE;
	if( pCollMap == NULL )	return FALSE;
	if( pIDMap[x+YOff[y]] )	return FALSE;
	if( pCollMap[x+YOff[y]] & (BLOCK_DEEPWATER|BLOCK_NORMAL) )
		return false;
	return true;
}

// ������ ��� �ִ°�?
// ���� ĳ���Ͱ� �ִ°�, ��, �Ϲ� ��ֹ�
BOOL cltMapCommon::IsEmptyWater(SHORT x, SHORT y)
{
	if( x<0  || x>=MH.MapXsize )	return FALSE;
	if( y<=0 || y>=MH.MapYsize )	return FALSE;
	if( pIDMap == NULL )	return FALSE;
	if( pCollMap == NULL )	return FALSE;
	if( pIDMap[x+YOff[y]] )	return FALSE;
	if( (pCollMap[x+YOff[y]] & BLOCK_DEEPWATER) == 0 )	return FALSE;
	return TRUE;
}



// �ҷ����ų� �����ϰ� ������ ������ �������� ������ �����. 
void cltMapCommon::MakeLastMap()
{
	int i, j;


	//------------------------------------
	// Ÿ�� �迭�� �� ���� �Ѵ�. 
	//------------------------------------
	if(MH.TileSet1)
	{
		SI32 siref		= pclTileManager->FindRefFromUnique_TileSet(MH.TileSet1);
		if(siref >= 0)
		{
			SI32 atb		= pclTileManager->pclTileInfo[siref]->dwAtb;
/*			
			if(atb & TILEATB_XSPR ) {
			
				for(i = 0;i < MH.MapYsize;i++)
					for(j = 0;j < MH.MapXsize;j++)
					{
						PutTileXpr(0, j, i, MH.TileSet1);
					}			

			} else
*/

			if(atb & TILEATB_NODIR)
			{
				for(i = 0;i < MH.MapYsize;i++)
					for(j = 0;j < MH.MapXsize;j++)
					{
						PutTile0(0, j, i, MH.TileSet1);
					}
			}
		}
	}

	// ��¿� Ref�� ���Ӱ� ������Ʈ �Ѵ�. Ÿ�ϼ�Ʈ�� ����Ǿ��µ� �̰��� ������Ʈ ���� ������ 
	// �߸��� ref�� �����ϰ� �ȴ�.
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			SI32 drawunique = pclLayer[0]->GetDrawInfo_Unique(j, i);
			SI32 siref  = pclTileManager->FindRefFromUnique_TileSet(drawunique);
			pclLayer[0]->SetDrawInfo_Ref(j, i, siref);

			drawunique = pclLayer[1]->GetDrawInfo_Unique(j, i);
			siref  = pclTileManager->FindRefFromUnique_TileSet(drawunique);
			pclLayer[1]->SetDrawInfo_Ref(j, i, siref);

		}

	// IDMap, CollMap�� �ʱ�ȭ�Ǿ�� �Ѵ�. 
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			pIDMap[j+YOff[i]]	= 0;

			// Custom Coll�� �ʱ�ȭ���� �ʴ´�. 
			SI32 CollData = pCollMap[j+YOff[i]];
			pCollMap[j+YOff[i]] = 0;

			if(CollData & BLOCK_CUSTOM)
			{
				pCollMap[j+YOff[i]] |= BLOCK_CUSTOM;
			}
			if(CollData & BLOCK_EVENT1)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT1;
			}
			if(CollData & BLOCK_EVENT2)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT2;
			}
			if(CollData & BLOCK_EVENT3)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT3;
			}
			if(CollData & BLOCK_EVENT4)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT4;
			}
			if(CollData & BLOCK_EVENT5)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT5;
			}
			if(CollData & BLOCK_EVENT6)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT6;
			}
			if(CollData & BLOCK_EVENT7)
			{
				pCollMap[j+YOff[i]] |= BLOCK_EVENT7;
			}
		}


	// �� ����Ʈ ��尡 �ƴϾ�� �Ѵ�. 
	if(pclClient->GameMode != GAMEMODE_MAPEDIT)
	{
		// ������ ������ 
		if(siVillageUnique >= 0 && siVillageUnique < MAX_VILLAGE_NUMBER)
		{
			if(pclClient->pclVillageManager->pclVillageInfo[siVillageUnique])
			{
				// ���� �����. 
				SI32 mapindex	= 0;
				SI32 mapposx	= pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->PosX;
				SI32 mapposy	= pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->PosY;

				SI32 founique = 1840;	// "Gate_Wood1ToMain"
				/*
				if ( pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->bUpgradeSwitch )
					founique = pclFieldObjectManager->FindUniqueFromName(TEXT("Gate_CityHall2"));
				else
					founique = pclFieldObjectManager->FindUniqueFromName(TEXT("Gate_CityHall1"));
				*/
					
				TCHAR * gatename = GetTxtFromMgr(252);	// "���� ��"
				if(founique > 0 && gatename )
				{

					cltGate clGate(siVillageUnique, gatename, founique, 0, 0,
						0, 0, 0, mapindex, mapposx, mapposy, false);

					pclClient->pclMapManager->pclGateManager->Add(&clGate);

					SI32 gatex;
					SI32 gatey;
					if(siMapUnique)
					{
						gatex = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[siMapUnique]->siGateX;
						gatey = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[siMapUnique]->siGateY;
					}
					else
					{
						gatex = 0;
						gatey = 0;
					}


					// ������ ���� �����Ѵ�. 
					DeleteFieldObject(gatex, gatey);
					// ���ο� ���� �����. 
					PutFieldObject(gatex, gatey, founique, clGate.siUnique);

				}

				if ( pclClient->siServiceArea == ConstServiceArea_Japan )
				{
					founique = 7176;	// ���� �Ĺ� ����ũ	"JapanVillage_Fence7"
				}
				else
				{
					founique = 292;		// "Gate3_MainToBear1"
				}

				if(founique > 0 && gatename )
				{

					cltGate clGate(siVillageUnique, gatename, founique, 0, 0,
						0, 0, 0, mapindex, mapposx, mapposy, false);

					pclClient->pclMapManager->pclGateManager->Add(&clGate);

					SI32 gatex;
					SI32 gatey;
					if(siMapUnique)
					{
						gatex = 35;
						gatey = 63;
					}
					else
					{
						gatex = 0;
						gatey = 0;
					}

					// ������ ���� �����Ѵ�. 
					DeleteFieldObject(gatex, gatey);
					// ���ο� ���� �����. 
					PutFieldObject(gatex, gatey, founique, clGate.siUnique);
				}

			}
		}

		if(siVillageUnique)
		{
			MakeVillageStructure();
		}

		// �������̸�, 
		if(pclClient->pclMapManager->IsHuntMap(siMapIndex) == true)
		{
			SI32 villageunique = siMapIndex - 200;

			SI32 founique = 0;
			if(siMapUnique == 13 )		// "NoName"
			{
				//founique = pclFieldObjectManager->FindUniqueFromName(TEXT("Gate6_PalaceToMain"));
				founique = 8028;		// "smallforest_upperwall09"
			}
			else if(siMapUnique == 36 )	// "NoName"
			{
				founique = 8100;		// "GARA_1_Gate01"
			}
			else if(siMapUnique == 46)	// "NoName"
			{
				founique = 6670;		// "IceCastle_Wall27"
			}
			else if(siMapUnique == 51)	// "NoName"
			{
				founique = 4340;		// "Goenraku_Wall16"
			}
			else
			{
				MsgBox(TEXT("fdsfd3fd"), TEXT("F3df:%d"), siMapUnique);
			}

			TCHAR * gatename = GetTxtFromMgr(253);		// "������"
			if(founique > 0 && gatename )
			{
				SI32 gatex		= 0, gatey		= 0;

				if(siMapUnique)
				{
					gatex	= pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[siMapUnique]->siGateX;
					gatey	= pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[siMapUnique]->siGateY;

				}

				if(pclClient->pclMapManager->pclMapArray[villageunique])
				{
					SI32 villagemaptype = pclClient->pclMapManager->pclMapArray[villageunique]->siMapUnique;
				}

				cltGate clGate(siMapIndex,gatename, founique, 0, 0,	siMapIndex, gatex, gatey, villageunique, 0, 0, false);
				
				pclClient->pclMapManager->pclGateManager->Add(&clGate);
				
				PutFieldObject(gatex, gatey, founique, clGate.siUnique);
				
			}

			
		}

	}


	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			SI32 collinfo = CalcCollInfo(j, i);
			if(collinfo)
			{
				SetCollMap(j, i, collinfo);
			}
		}

}

// ���� x, y������ �浹������ ���θ� ����Ѵ�. 
SI32 cltMapCommon::CalcCollInfo(SI32 x, SI32 y)
{

	SI32 returnval = 0;
	//-----------------------------------
	// Ÿ�Ͽ� ���� �浹 ������ ���Ѵ�. (Layer 0)
	//-----------------------------------
	SI32 unique = pclLayer[0]->GetUnique(x, y);

	bool bwaterswitch = false;

	SI32 siref  = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(siref >= 0)
	{
		SI32 atb = pclTileManager->pclTileInfo[siref]->dwAtb;

		if(atb & TILEATB_BLOCK)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & TILEATB_WATER)
		{
			bwaterswitch = true;
			returnval |= BLOCK_SHALLOWWATER;
		}
		if(atb & TILEATB_DEEPWATER)
		{
			bwaterswitch = true;
			returnval |= BLOCK_DEEPWATER;
		}
	}


	//-----------------------------------
	// Ÿ�Ͽ� ���� �浹 ������ ���Ѵ�. (Layer 1)
	//-----------------------------------
	SI32 x1	= x/2*2;
	SI32 y1	= y/2*2;
	
	unique = pclLayer[1]->GetUnique(x1, y1);

	siref  = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(siref >= 0)
	{
		SI32 atb = pclTileManager->pclTileInfo[siref]->dwAtb;

		if(atb & TILEATB_BLOCK)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & TILEATB_WATER)
		{
			bwaterswitch = true;
			returnval |= BLOCK_SHALLOWWATER;
		}
		if(atb & TILEATB_DEEPWATER)
		{
			bwaterswitch = true;
			returnval |= BLOCK_DEEPWATER;
		}

	}

	// ���� ������ ���� �ƴϸ� ������. 
	if(bwaterswitch == false)
	{
		returnval |= BLOCK_LAND;
	}
	
	//------------------------------------------
	// Field Object�� ���� �浹 ������ ���Ѵ�. 
	//------------------------------------------
	SI32 fieldobject = GetFieldObject_Unique(x, y);
	if(fieldobject)
	{
		SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(fieldobject);
		if(ref < 0)return 0;
		
		SI32 type = pclFieldObjectManager->pclInfo[ref]->dwType;
		SI32 atb = pclFieldObjectManager->pclTypeInfo[type]->dwAtb;
		
		if(atb & FOATB_VILLAGE)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & FOATB_TREE)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & FOATB_MOUNTINE)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & FOATB_GATE)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & FOATB_STATUE)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & FOATB_BUILD)
		{
			returnval |= BLOCK_NORMAL;
		}
		if(atb & FOATB_BRIDGE)
		{
			if(returnval & BLOCK_DEEPWATER)
			{
				returnval -= BLOCK_DEEPWATER;// �ٸ��� ������ �� �ִ� �����̴�. 
			}
			if(returnval & BLOCK_SHALLOWWATER)
			{
				returnval -= BLOCK_SHALLOWWATER;// �ٸ��� ������ �� �ִ� �����̴�. 
			}
			
		}
	}
	
	//-------------------------
	// Custom Coll
	//-------------------------
	if(GetCollMap(x, y) & BLOCK_CUSTOM)
	{
		returnval |= BLOCK_CUSTOM;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT1)
	{
		returnval |= BLOCK_EVENT1;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT2)
	{
		returnval |= BLOCK_EVENT2;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT3)
	{
		returnval |= BLOCK_EVENT3;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT4)
	{
		returnval |= BLOCK_EVENT4;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT5)
	{
		returnval |= BLOCK_EVENT5;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT6)
	{
		returnval |= BLOCK_EVENT6;
	}
	if(GetCollMap(x, y) & BLOCK_EVENT7)
	{
		returnval |= BLOCK_EVENT7;
	}
	
	return returnval;
}


// ĳ������ ���������� ���� ���� 
// ���� 4��Ʈ�� �����Ѵ�. 
// ���� 12��Ʈ�� ���� ������. 
BOOL cltMapCommon::DeleteCharCollMap(SHORT x, SHORT y)
{
	if( pCollMap == NULL )	return TRUE;
	if( IsInMapArea(x, y) == false )	return FALSE;

	pCollMap[x+YOff[y]] &= 0x0fff;
	return TRUE;
}

// �������� X, Y��ǥ�� �߽��� ��Ʈ ��ǥ�� �����ش�. (������ǥ)
// ȭ����� ��ǥ�� �ƴ϶� ���� ��ǥ�̴�. 
BOOL cltMapCommon::GetXYDotFromXYCord(SI32 x, SI32 y, SI32 & centerx, SI32 & centery)
{
	if(IsInMapArea(x, y) == false)return FALSE;

	// ��ũ�� ������ �߽� ��ǥ�� ���Ѵ�. 
	centerx = x * MAP_TILE_XSIZE_HALF
		     -y * MAP_TILE_XSIZE_HALF ;

	centery = y * MAP_TILE_YSIZE_HALF
		     +x * MAP_TILE_YSIZE_HALF ;

	return TRUE;

}

//-----------------------------------------------
// ���� ���� 
//-----------------------------------------------
// ������ �������� �����.
void cltMapCommon::DeattachVillage()
{
	SI32 i, j;

	// pclFieldObjectManager is supplied by the loader and is NULL on the server
	// preload path; the loop below dereferences it unconditionally.
	if(pclFieldObjectManager == NULL)return;

	// �������� Village�� ��� �����Ѵ�. 
	for(i = 0;i < MH.MapYsize;i++)
		for(j = 0;j < MH.MapXsize;j++)
		{
			SI32 unique = GetFieldObject_Unique(j, i);
			if(unique == 0)continue;
			
			SI32 ref	= pclFieldObjectManager->FindRefFromUniqueMinus(unique);
			if(ref < 0)continue ;


			if ( pclFieldObjectManager->pclInfo[ref]->Unique >= 1 &&
				pclFieldObjectManager->pclInfo[ref]->Unique <= 6 )
			{
				DeleteFieldObject(j, i);
			}

		}
}

// ����Ʈ�� �������� �����.
void cltMapCommon::DeattachGate()
{
	SI32 i, j;

	// �������� Village�� ��� �����Ѵ�. 
	for(i = 0;i < MH.MapYsize;i++)
		for(j = 0;j < MH.MapXsize;j++)
		{
			SI32 unique = GetFieldObject_Unique(j, i);
			if(unique == 0)continue;

			SI32 ref	= pclFieldObjectManager->FindRefFromUniqueMinus(unique);
			if(ref < 0)continue ;

			if ( pclFieldObjectManager->pclInfo[ref]->dwType == FOTYPE_GATE)
			{
				DeleteFieldObject(j, i);
			}
	
		}
}

// ������ ������ ���δ�.
void cltMapCommon::AttachVillage()
{
	SI32 i;

	for(i = 0; i < MAX_VILLAGE_NUMBER;i++)
	{
		// ������ ������ �ɴ´�. 
		if(pclClient->pclVillageManager->pclVillageInfo[i])
		{
			SI32 x = pclClient->pclVillageManager->pclVillageInfo[i]->PosX;
			SI32 y = pclClient->pclVillageManager->pclVillageInfo[i]->PosY;
			SI32 unique = pclClient->pclVillageManager->pclVillageInfo[i]->siVillageUnique;


			if ( pclClient->pclVillageManager->pclVillageInfo[i]->bUpgradeSwitch )
			{
				PutFieldObject(x, y, 5, unique);
				PutFieldObject(x, y+1, 7, unique);
			}
			else
			{
				PutFieldObject(x, y, 1, unique);
				PutFieldObject(x, y+1, 6, unique);
				/*cyj �������� ���� �ȳ����� ����
				if ( pclClient->pclVillageManager->pclVillageInfo[i]->bUpgradeSwitch )
				PutFieldObject(x-2,y+2,4,0);
				else
				PutFieldObject(x-2,y+2,3,0);
				*/
			}
		}
	}
}


// ĳ���͸� ��ġ��Ű�� ���� orgx, orgy �ֺ��� �� ������ ã�´�. 
bool cltMapCommon::FindEmptyAreaForCharLocate(SI32 kind, SI32 orgx, SI32 orgy, SI32 range, SI32 *pdestx, SI32 *pdesty, bool bcharswitch)
{
	if ( kind <= 0 ) return false;

	SI32 i, j, k;

	SI32 block = pclClient->pclKindInfoSet->pclKI[kind]->siBlock;

	block |= BLOCK_SYSTEMNPC;

	// �̺�Ʈ ���������� ��������� �ʴ´�	. 
	block |= BLOCK_EVENT1;

	// �̺�Ʈ�� ĳ���Ͱ� �ƴ϶��, 
	if((pclClient->pclKindInfoSet->pclKI[kind] == NULL || pclClient->pclKindInfoSet->pclKI[kind]->IsAtb(ATB_EVENTXBOX) == false))
	{
		block |= BLOCK_EVENT2;
	}
	block |= BLOCK_EVENT3;
	block |= BLOCK_EVENT4;
	block |= BLOCK_EVENT5;
	block |= BLOCK_EVENT6;
	block |= BLOCK_EVENT7;

	if(bcharswitch)
	{
		block |= BLOCK_CHAR;

	}

	if(IsInMapArea(orgx, orgy) == false)return false;

	for(i = 0;i < range; i++)
	{
		for(j = -i;j <= i;j++)
			for(k = -i;k <= i;k++)
			{
				if( TABS(j) != i && TABS(k) != i)continue;

				// ĳ���͵� �浹 ������ �����Ѵ�. 
				if(IsColl(orgx + k, orgy + j, block) == false )
				{
					*pdestx = orgx + k;
					*pdesty	= orgy + j;
					return true;
				}
			}

	}

	return false;
}

// ������ ��� �ִ°�?
// ���� ĳ���Ͱ� �ִ°�, ��, �Ϲ� ��ֹ�
BOOL cltMapCommon::IsEmpty(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy)
{
	SHORT i, j;
	SHORT tempx, tempy;

	// ���� ���� �ȿ� �ִ��� Ȯ���Ѵ�. 
	if(x<0 || x+xsize-1>=MH.MapXsize)return FALSE;
	if(y<0 || y+ysize-1>=MH.MapYsize)return FALSE;
	
	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			tempx=x-xsize/2+j;
			tempy=y-ysize/2+i;

			if( idx<=tempx && tempx<(idx + 1) 
			&&  idy<=tempy && tempy<(idy + 1) )return FALSE;


			if(IsEmpty(tempx, tempy)==FALSE)return FALSE;
		}

	return TRUE;
}

// ��� �ִ°�?
// ���� ĳ���Ͱ� �ִ°�, ��, �Ϲ� ��ֹ�
BOOL cltMapCommon::IsEmptyWater(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy)
{
	SHORT i, j;
	SHORT tempx, tempy;

	// ���� ���� �ȿ� �ִ��� Ȯ���Ѵ�. 
	if(x<0 || x+xsize-1>=MH.MapXsize)return FALSE;
	if(y<0 || y+ysize-1>=MH.MapYsize)return FALSE;

	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			tempx=x-xsize/2+j;
			tempy=y-ysize/2+i;
			
			if( idx <= tempx && tempx<(idx + 1) 
			&&  idy <= tempy && tempy<(idy + 1) )return FALSE;
			
			if(IsEmptyWater(tempx, tempy)==FALSE)return FALSE;
		}

	return TRUE;
}


// ���� ��ǥ�� �Է��ϸ� �⺻ ��ǥ�� ���´�. 
// realx, realy :���� ��ǥ 
// x, y :�⺻ ��ǥ 
void cltMapCommon::GetRealTileByRealDot(SHORT realx, SHORT realy, SI32* pX, SI32* pY)
{
	*pX = (realx/2+realy)/MAP_TILE_XSIZE_HALF;
	*pY = (-realx/2+realy)/MAP_TILE_XSIZE_HALF;

	SI32 mapcenterx = 0, mapcentery = 0;
	GetXYDotFromXYCord(*pX, *pY, mapcenterx, mapcentery);
}

// Ư�� ��ǥ�� Ÿ�� ������ ���´�. 
SI32 cltMapCommon::GetMouseTileAtb(SI32 x, SI32 y)
{
	SI32 atb		= 0;
	SI32 tileunique = 0;
	if(pclLayer[0])
	{
		tileunique	= pclLayer[0]->GetDrawInfo_Unique(x, y);
	}
		
	if(tileunique == 82)	// ����� 
	{
		atb |= MOUSETILEATB_FARM;
	}

	return atb;
}

DWORD cltMapCommon::GetInstanceMapMaxTime()
{
	cltServer* pclserver = (cltServer*)pclClient;

	DWORD MaxTime = 0;
	
	switch(pclserver->pclMapManager->clUnitMapInfo[siMapIndex].siInstanceMap)
	{
	case INSTANCEMAP_TYPE_GUILD:		MaxTime = INSTANCEMAP_MAX_TIME;						break;
	case INSTANCEMAP_TYPE_PARTY:		MaxTime = INSTANCEMAP_PIRATESHIP_MAX_TIME;			break;
	case INSTANCEMAP_TYPE_VILLAGE:		MaxTime = INSTANCEMAP_MAX_TIME;						break;
	case INSTANCEMAP_TYPE_SERVER:		MaxTime = INSTANCEMAP_SERVEREVENT_MAX_TIME;			break;
	}

	return MaxTime;
}

bool IsMoveColl(int x, int y, int SrcX, int SrcY, void* thisptr)
{
	cltMapCommon* pclmap = (cltMapCommon* )thisptr;

	// ���� ���� ���̸� �浹. 
	if(pclmap->IsInMapArea(x, y) == false)return true;

	 if(pclmap->GetCollMap(x, y) & pclmap->pclCM->CR[pclmap->MoveID]->Block)return true;

	return false;
}


