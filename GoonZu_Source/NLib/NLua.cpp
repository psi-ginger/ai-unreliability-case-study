#include <string.h>
#include "NLua.h"

NLua::NLua()
{
	m_pluaState	 = lua_open();
	if( m_pluaState == NULL )			return;

	luaL_openlibs( m_pluaState );
}

NLua::NLua( lua_State* pluaState )
{
	m_pluaState = pluaState;
	m_siOpenType = NLUA_OPEN_TYPE_SET;
}

NLua::~NLua()
{
	if( m_pluaState != NULL )
	{
		lua_close( m_pluaState );
	}
}

bool NLua::DoBuffer(const char* pszBuffer, int nSize, const char* pszBufferName )
{
	if( m_pluaState == NULL )									return false;
	if( pszBuffer == NULL )										return false;
	if( nSize == 0 )											return false;
	if( pszBufferName == NULL )									return false;

	int ret = 0;

	ret = luaL_loadbuffer( m_pluaState, pszBuffer, nSize, pszBufferName );
	if( ret == 0 )
	{
		ret = lua_pcall( m_pluaState, 0, LUA_MULTRET, 0 );
	}

	return (ret == 0);
}

bool NLua::DoString(const char* pszString)
{
	if( m_pluaState == NULL )									return false;
	if( pszString == NULL )										return false;

	int ret = 0;

	ret = luaL_loadstring( m_pluaState, pszString );
	if( ret == 0 )
	{
		ret = lua_pcall( m_pluaState, 0, LUA_MULTRET, 0 );
	}

	return (ret == 0);
}

bool NLua::DoFile(const char* pszFilename)
{
	if( m_pluaState == NULL )									return false;
	if( pszFilename == NULL )									return false;

	int ret = 0;

	ret = luaL_loadfile( m_pluaState, pszFilename );
	if( ret == 0 )
	{
		ret = lua_pcall( m_pluaState, 0, LUA_MULTRET, 0 );
	}

	return (ret == 0);
}

void NLua::Call(int nParam, int nRet)
{
	if( m_pluaState == NULL )									return;
	if( nParam < 0 )											return;
	if( nRet < 0 )												return;

	lua_call( m_pluaState, nParam, nRet );
}

bool NLua::RegistCFunction(lua_CFunction pFunc, const char *pszFuncName)
{
	if( m_pluaState == NULL )									return false;
	if( pFunc == NULL )											return false;
	if( pszFuncName == NULL || strlen(pszFuncName) <= 0 )		return false;

	lua_pushcfunction( m_pluaState, pFunc );
	lua_setglobal( m_pluaState, pszFuncName );

	return true;
}

void NLua::RegistFputs( void* pUserPT, lua_fputs pRedined_fputs )
{
	void* pUserPointer = this;
	if( pUserPT != NULL )		pUserPointer = pUserPT;

	lua_ReDefine_Fputs( m_pluaState, pUserPointer, pRedined_fputs );
}

//--------------------------------------------------------------------------------------------
// Nil 관련
//--------------------------------------------------------------------------------------------
bool NLua::IsNil( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL)	return false;

	return lua_isnil( m_pluaState, nIndex );
}

//--------------------------------------------------------------------------------------------
// 스택관련 함수
//--------------------------------------------------------------------------------------------
SI32 NLua::GetTop()
{
	if( m_pluaState == NULL)	return 0;
	return lua_gettop(m_pluaState);
}

void NLua::Pop( int nCount )
{
	if( m_pluaState == NULL)	return;
	lua_pop( m_pluaState, nCount );
}

void NLua::Remove( int nIndex )
{
	if( m_pluaState == NULL)	return;
	lua_remove( m_pluaState, nIndex );	
}

int NLua::Type( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL)	return LUA_TNONE;

	return lua_type( m_pluaState, nIndex );
}

void NLua::GetField( int nIndex, const char* pszFieldName )
{
	if( pszFieldName == NULL )			return;
	if( m_pluaState == NULL )			return;

	lua_getfield( m_pluaState, nIndex, pszFieldName );
}

void NLua::GetGlobal( const char* pszFieldName)
{
	if( pszFieldName == NULL )			return;
	if( m_pluaState == NULL )			return;

	lua_getglobal( m_pluaState, pszFieldName );
}

//--------------------------------------------------------------------------------------------
// 숫자관련
//--------------------------------------------------------------------------------------------
bool NLua::IsNumber(int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )		return false;
	return ( lua_isnumber( m_pluaState, nIndex) == 1 );
}

SI64 NLua::ToInteger( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )									return false;
	lua_Integer integer = lua_tointeger( m_pluaState, nIndex );
	return integer;
}

REAL64 NLua::ToNumber( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )									return false;
	lua_Number number = lua_tonumber( m_pluaState, nIndex );
	return number;
}

void NLua::PushInteger( SI64 siInteger )
{
	if( m_pluaState == NULL )									return;
	lua_pushinteger( m_pluaState, (lua_Integer)siInteger );
}

void NLua::PushNumber( REAL64 fNumber )
{
	if( m_pluaState == NULL )									return;
	lua_pushnumber( m_pluaState, (lua_Number)fNumber );
}

//--------------------------------------------------------------------------------------------
// 문자관련
//--------------------------------------------------------------------------------------------
bool NLua::IsString( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )									return false;
	return ( lua_isstring( m_pluaState, nIndex ) == 1 );
}


const char* NLua::ToString( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )									return "";
	return lua_tostring( m_pluaState, nIndex );
}

void NLua::PushString( const char* pszString )
{
	if( pszString == NULL )				return;
	if( m_pluaState == NULL )			return;

	lua_pushstring( m_pluaState, pszString );
}

//--------------------------------------------------------------------------------------------
// boolean관련
//--------------------------------------------------------------------------------------------
bool NLua::IsBoolean( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )			return false;

	return lua_isboolean( m_pluaState, nIndex );
}

bool NLua::ToBoolean( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )			return false;

	return (lua_toboolean( m_pluaState, nIndex ) == 1 );
}

void NLua::PushBoolean( bool bValue )
{
	if( m_pluaState == NULL )			return;

	lua_pushboolean( m_pluaState, bValue );
}

//--------------------------------------------------------------------------------------------
// pointer관련
//--------------------------------------------------------------------------------------------
bool NLua::IsPointer( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )			return false;

	return (lua_isnumber( m_pluaState, nIndex ) == 1 );
}

const void* NLua::ToPointer( int nIndex /* = -1 */ )
{
	if( m_pluaState == NULL )			return false;

	return lua_topointer( m_pluaState, nIndex );
}


//--------------------------------------------------------------------------------------------
// 테이블관련
//--------------------------------------------------------------------------------------------
bool NLua::IsTable( int nIndex /* = 1 */ )
{
	if( m_pluaState == NULL )			return false;
	return (lua_istable( m_pluaState, nIndex ) == 1);
}

void NLua::SetTable( int nIndex )
{
	if( m_pluaState == NULL )			return;
	lua_settable( m_pluaState, nIndex );
}

void NLua::GetTable( int nIndex )
{
	if( m_pluaState == NULL )			return;
	lua_gettable( m_pluaState, nIndex );
}

void NLua::NewTable()
{
	if( m_pluaState == NULL )			return;
	lua_newtable( m_pluaState );
}

void NLua::GetTableField( const char* pszKey )
{
	if( m_pluaState == NULL )						return;
	if( lua_istable( m_pluaState, -1 ) == false)	return;

	lua_pushstring( m_pluaState, pszKey );
	lua_gettable( m_pluaState, -2 );
}

bool NLua::GetTableValue( const char* pszKey, SI64* psiValue )
{
	if( m_pluaState == NULL )			return false;
	if( pszKey == NULL )				return false;
	if( psiValue == NULL )				return false;
	if( IsTable() == false )			return false;

	PushString( pszKey );	GetTable( -2 );
	if( IsNumber() == false )			return false;

	*psiValue = ToInteger();
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( const char* pszKey, REAL64* pfValue )
{
	if( m_pluaState == NULL )			return false;
	if( pszKey == NULL )				return false;
	if( pfValue == NULL )				return false;
	if( IsTable() == false )			return false;

	PushString( pszKey );	GetTable( -2 );
	if( IsNumber() == false )			return false;

	*pfValue = ToNumber();
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( const char* pszKey, bool* pbValue )
{
	if( m_pluaState == NULL )			return false;
	if( pszKey == NULL )				return false;
	if( pbValue == NULL )				return false;
	if( IsTable() == false )			return false;

	PushString( pszKey );	GetTable( -2 );
	if( IsBoolean() == false )			return false;

	*pbValue = ToBoolean();
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( const char* pszKey, char* pszValue, size_t nSize )
{
	if( m_pluaState == NULL )			return false;
	if( pszKey == NULL )				return false;
	if( IsTable() == false )			return false;

	PushString( pszKey );	GetTable( -2 );
	if( IsString() == false )			return false;

	strncpy( pszValue, ToString(), nSize );
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( SI64 siKey, SI64* psiValue )
{
	if( m_pluaState == NULL )			return false;
	if( psiValue == NULL )				return false;
	if( IsTable() == false )			return false;

	PushInteger( siKey );	GetTable( -2 );
	if( IsNumber() == false )			return false;

	*psiValue = ToInteger();
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( SI64 siKey, REAL64* pfValue )
{
	if( m_pluaState == NULL )			return false;
	if( pfValue == NULL )				return false;
	if( IsTable() == false )			return false;

	PushInteger( siKey );	GetTable( -2 );
	if( IsNumber() == false )			return false;

	*pfValue = ToNumber();
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( SI64 siKey, bool* pbValue )
{
	if( m_pluaState == NULL )			return false;
	if( pbValue == NULL )				return false;
	if( IsTable() == false )			return false;

	PushInteger( siKey );	GetTable( -2 );
	if( IsNumber() == false )			return false;

	*pbValue = ToBoolean();
	Pop( 1 );
	return true;
}

bool NLua::GetTableValue( SI64 siKey, char* pszValue, size_t nSize )
{
	if( m_pluaState == NULL )			return false;
	if( IsTable() == false )			return false;

	PushInteger( siKey );	GetTable( -2 );
	if( IsString() == false )			return false;

	strncpy( pszValue, ToString(), nSize );

	Pop( 1 );
	return true;
}

void NLua::SetTableValue( const char* pszKey, SI64 siValue)
{
	if( m_pluaState == NULL )			return;
	if( pszKey == NULL )				return;
	if( IsTable() == false )			return;

	PushString( pszKey );
	PushInteger( siValue );
	SetTable( -3 );
}

void NLua::SetTableValue( const char* pszKey, REAL64 fValue)
{
	if( m_pluaState == NULL )			return;
	if( pszKey == NULL )				return;
	if( IsTable() == false )			return;

	PushString( pszKey );
	PushNumber( fValue );
	SetTable( -3 );
}

void NLua::SetTableValue( const char* pszKey, const char* pszValue)
{
	if( m_pluaState == NULL )			return;
	if( pszKey == NULL )				return;
	if( IsTable() == false )			return;

	PushString( pszKey );
	PushString( pszValue );
	SetTable( -3 );
}

void NLua::SetTableValue( SI64 siKey, SI64 siValue)
{
	if( m_pluaState == NULL )			return;

	PushInteger( siKey );
	PushInteger( siValue );
	SetTable( -3 );
}

void NLua::SetTableValue( SI64 siKey, REAL64 fValue)
{
	PushInteger( siKey );
	PushNumber( fValue );
	SetTable( -3 );
}

void NLua::SetTableValue( SI64 siKey, const char* pszValue)
{
	PushInteger( siKey );
	PushString( pszValue );
	SetTable( -3 );
}
