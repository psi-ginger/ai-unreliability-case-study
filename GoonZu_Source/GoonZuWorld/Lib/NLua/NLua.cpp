#include "NLua.h"

NLua::NLua()
{
	m_pluaState = lua_open();
}

NLua::~NLua()
{
	if(m_pluaState != NULL)
	{
		lua_close(m_pluaState);
	}
}

bool NLua::LoadFile( const char* pszFileName )
{
	if(m_pluaState == NULL)			return false;
	if(pszFileName == NULL)			return false;

	int nRet = luaL_loadfile(m_pluaState, pszFileName);

	if(nRet != 0)
	{
		return false;
	}

	return true;
}

bool NLua::LoadBuffer(const char* pBuffer, size_t BufSize, const char* pszName)
{
	if(m_pluaState == NULL)			return false;
	if(pBuffer == NULL)				return false;
	if(pszName == NULL)				return false;
	if(BufSize == 0)				return false;

	int nRet = luaL_loadbuffer(m_pluaState, pBuffer, BufSize, pszName);

	if(nRet != 0)
	{
		return false;
	}

	return true;
}

bool NLua::DoFile( const char* pszFileName )
{
	if(pszFileName == NULL)				return false;

	if(LoadFile(pszFileName) == false)	return false;
	if(PCall() == false)				return false;

	return true;
}

bool NLua::DoBuffer( const char* pBuffer, size_t BufSize )
{
	if(pBuffer == NULL)				return false;
	if(BufSize == 0)				return false;

	if(LoadBuffer(pBuffer, BufSize, "DoBuffer") == false)		return false;
	if(PCall() == false)										return false;

	return true;
}

bool NLua::DoString( const char* pString )
{
	if(pString == NULL)				return false;

	if(LoadBuffer(pString, strlen(pString), "DoString") == false)		return false;
	if(PCall() == false)												return false;

	return false;
}

bool NLua::PCall(int nArgs, int nResults )
{
	if(m_pluaState == NULL)			return false;

	int nRet = lua_pcall(m_pluaState, nArgs, nResults, 0);

	if(nRet != 0)
	{
		return false;
	}

	return true;
}

bool NLua::RegistFunction( lua_CFunction Func, const TCHAR* pszFuncName )
{
	if(m_pluaState == NULL)			return false;

	lua_pushcfunction(m_pluaState, Func);
	lua_setglobal(m_pluaState, pszFuncName);

	return true;
}