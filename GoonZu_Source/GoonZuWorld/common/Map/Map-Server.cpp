
#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"

#include "map.h"
#include "..\Smoke\Smoke.h"
#include "..\Char\KindInfo\KindInfo.h"
#include "..\..\Lib\AStar\AStarListMgr.h"

#include "..\..\Server\ServerMap\ServerMap.h"

#include "ITZWorld.h"

extern cltCommonLogic* pclClient;

cltMapServer::cltMapServer(ITZWorld< ITZMap > *pWorld, UI32 mapindex, SI32 mapunique, SHORT xsize, SHORT ysize, SI32 tileset1, SI32 tileset2, cltCharManager* pclcm, cltTileManager* pcltileset, cltFieldObjectManager* pclfieldobjectmanager) 
: cltMapCommon(mapindex, mapunique, xsize, ysize, tileset1, tileset2, pclcm, pcltileset, pclfieldobjectmanager)

{
	pclServerMap = (cltServerMap *)pWorld->GetMap( mapindex );

	pWorld->GetMap(mapindex)->CreateMap( xsize, ysize );
}

cltMapServer::~cltMapServer()
{
	// pclServerMap은 메모리 헤제를 이곳에서 할 필요가 없음	
}