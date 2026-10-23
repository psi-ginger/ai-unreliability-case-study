#include "..\CommonLogic\CommonLogic.h"

#include "..\Char\KindInfo\KindInfo.h"
#include "..\..\Lib\AStar\AStarListMgr.h"

#include "Map\Map.h"

extern cltCommonLogic* pclClient;

BOOL cltMapClient::IncreaseVirtualMapX(int mount)
{

	if((VirtualMapX+mount)<MH.MapXsize)
	{
		VirtualMapX+=mount;
		return TRUE;
	}
	return FALSE;
}

BOOL cltMapClient::DecreaseVirtualMapX(int mount)
{
	if(VirtualMapX-mount>=0)
	{
		VirtualMapX-=mount;
		return TRUE;
	}

	return FALSE;
}

BOOL cltMapClient::IncreaseVirtualMapY(int mount)
{

	if((VirtualMapY+mount)<MH.MapYsize)
	{
		VirtualMapY+=mount;
		return TRUE;
	}
	return FALSE;
}

BOOL cltMapClient::DecreaseVirtualMapY(int mount)
{
	if(VirtualMapY-mount>=0)
	{
		VirtualMapY-=mount;
		return TRUE;
	}

	return FALSE;
}

SHORT cltMapClient::GetVirtualMapX()
{
	return VirtualMapX;
}

SHORT cltMapClient::GetVirtualMapY()
{
	return VirtualMapY;
}




