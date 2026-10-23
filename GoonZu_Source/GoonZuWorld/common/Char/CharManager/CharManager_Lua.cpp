//---------------------------------
// 2008/12/02 ÀÌ°­Çü
//---------------------------------

#include "CharManager.h"

//-------------------------------------------
// Server 
//-------------------------------------------
#include "..\..\Server\Server.h"

//-------------------------------------------
// Client
//-------------------------------------------
#include "..\..\Client\Client.h"

#include "../../Map/MapManager/MapManager.h"
#include "../../Order/order.h"

extern cltCommonLogic* pclClient;

// parameter
// para1 : CharName
int GetCharIDFromName(lua_State* L)
{
	NLua	lua( L );

	if( lua.GetTop() != 1 )				{	lua.PushInteger( (SI64)0 );		return 1;	}
	if( lua.IsString(1) == false )		{	lua.PushInteger( (SI64)0 );		return 1;	}

	SI32 siID = pclClient->pclCM->GetIDFromName( (TCHAR*)lua.ToString() );
	lua.PushInteger( siID );

	return 1;
}

// parameter
// para1 : CharID
int GetCharNameFromID(lua_State* L)
{
	NLua	lua( L );

	if( lua.GetTop() != 1 )				{	lua.PushString( "" );			return 1;	}
	if( lua.IsNumber(1) == false )		{	lua.PushString( "" );			return 1;	}

	SI32 siID = lua.ToInteger();
	if( pclClient->pclCM->IsValidID(siID))
	{
		cltCharServer* pclchar = pclClient->pclCM->GetCharServer(siID);
		lua.PushString( pclchar->GetName() );
	}
	else
		lua.PushString( "" );

	return 1;
}

// parameter
// para1 : CharID
// para2 : X
// para3 : Y
int MoveChar(lua_State* L)
{
	NLua	lua( L );

	if( lua.GetTop() != 3 )			return 0;
	if( lua.IsNumber(1) == false )	return 0;
	if( lua.IsNumber(2) == false )	return 0;
	if( lua.IsNumber(3) == false )	return 0;

	SI32 siID = lua.ToInteger(1);
	SI32 siX  = lua.ToInteger(2);
	SI32 siY  = lua.ToInteger(3);

	OrderMove( siID, siX, siY, BY_PLAYER );

	return 0;
}

// parameter
// para1 : CharID
// para2 : MapIndex
// para3 : X
// para4 : Y
int WarpChar(lua_State* L)
{
	NLua	lua( L );

	if( lua.GetTop() != 4 )			return 0;
	if( lua.IsNumber(1) == false )	return 0;
	if( lua.IsNumber(2) == false )	return 0;
	if( lua.IsNumber(3) == false )	return 0;
	if( lua.IsNumber(4) == false )	return 0;

	SI32 siID		= lua.ToInteger(1);
	SI32 siMapIndex	= lua.ToInteger(2);
	SI32 siX		= lua.ToInteger(3);
	SI32 siY		= lua.ToInteger(4);

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siID );
	if( pclchar == NULL )				return 0;

	if( siMapIndex < 0 || siMapIndex > MAX_MAP_NUMBER )		return 0;

	cltWarpInfo clWarpInfo(1, siMapIndex, false, false, siX, siY, -1, -1);
	pclchar->SetWarpInfo(&clWarpInfo, 10000);

	return 0;
}

// parameter
// para1 : CharID
int GetCharPos(lua_State* L)
{
	return 3;
}

// parameter
// para1 : CharID
int GetCharOrder(lua_State* L)
{
	NLua	lua( L );

	if( lua.GetTop() != 1 )			{	lua.PushInteger( (SI64)0 );		return 1;	}
	if( lua.IsString(1) == false )	{	lua.PushInteger( (SI64)0 );		return 1;	}

	SI32 siID = lua.ToInteger(1);

	if( pclClient->pclCM->IsValidID(siID))
	{
		cltCharServer* pclchar = pclClient->pclCM->GetCharServer(siID);
		lua.PushInteger( pclchar->GetCurrentOrder() );
	}
	else
		lua.PushInteger( (SI64)0 );

	return 1;
}

// parameter
// para1 : CharID
// para2 : ItemUnique
int GetCharHaveItemNum(lua_State* L)
{
	return 1;
}

// parameter
// para1 : CharID
// para2 : ItemUnique
int GetCharEquipItemNum(lua_State* L)
{
	return 1;
}
