//-----------------------------------------------------------------------------
// 2007/12/21 ÀÌ°­Çü
//-----------------------------------------------------------------------------

#ifndef _NLUA_H
#define _NLUA_H

#include "./include/lua.hpp"
#include "NDataTypes.h"

#ifdef _DEBUG
	#pragma comment(lib, "./Lib/NLua/lib/luaD5.1.lib") 
#else

#endif

class NLua
{
protected:
	lua_State*		m_pluaState;

public:
	NLua();
	~NLua();

	bool LoadFile( const char* pszFileName );
	bool LoadBuffer( const char* pBuffer, size_t BufSize, const char* pszName );

	bool DoFile( const char* pszFileName );
	bool DoBuffer( const char* pBuffer, size_t BufSize );
	bool DoString( const char* pString );

	bool PCall( int nArgs = 0, int nResults = LUA_MULTRET );

	bool RegistFunction( lua_CFunction Func, const TCHAR* pszFuncName );
};

#endif