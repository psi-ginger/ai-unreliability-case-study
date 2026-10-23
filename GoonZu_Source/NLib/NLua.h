#ifndef __NLUA_H_
#define __NLUA_H_

extern "C" {
#include "./lua/src/lua.h"
#include "./lua/src/lualib.h"
#include "./lua/src/lauxlib.h"
}

#include "NDataTypes.h"

enum NLUA_OPEN_TYPE
{
	NLUA_OPEN_TYPE_NONE,
	NLUA_OPEN_TYPE_OPEN,		// lua_open 함수를 수행함
	NLUA_OPEN_TYPE_SET			// STATE 포인터를 받아서 수행함
};

class NLua
{
private:
	SI16		m_siOpenType;
	lua_State*	m_pluaState;

public:
	NLua();
	~NLua();

	// 주어진 State를 사용한다.
	NLua( lua_State* pluaState );

	// 스크립트 실행 함수
	bool DoBuffer( const char* pszBuffer, int nSize, const char* pszBufferName = "Chunk" );
	bool DoString( const char* pszString );
	bool DoFile( const char* pszFilename );
	void Call( int nParam, int nRet );

	bool RegistCFunction( lua_CFunction pFunc, const char *pszFuncName );

	void RegistFputs( void* pUserPT, lua_fputs pRedined_fputs );

	// 스택관련 함수
	SI32 GetTop();
	void Pop( int nCount );
	void Remove( int nIndex	);
	int  Type( int nIndex = -1 );

	void GetField( int nIndex, const char* pszFieldName );
	void GetGlobal( const char* pszFieldName );

	// Nil 관련
	bool	IsNil( int nIndex = -1 );

	// 숫자관련
	bool	IsNumber( int nIndex = -1 );
	SI64	ToInteger( int nIndex = -1 );
	REAL64	ToNumber( int nIndex = -1 );
	void	PushInteger( SI64 siInteger );
	void	PushNumber( REAL64 fNumber );

	// 문자관련
	bool IsString( int nIndex = -1 );
	const char* ToString( int nIndex = -1 );
	void PushString( const char* pszString );

	// boolean관련
	bool IsBoolean( int nIndex = -1 );
	bool ToBoolean( int nIndex = -1 );
	void PushBoolean( bool bValue );

	// pointer관련
	bool IsPointer( int nIndex = -1 );
	const void* ToPointer( int nIndex = -1 );

	// 테이블관련
	bool IsTable( int nIndex = -1 );	
	void GetTable( int nIndex );
	void SetTable( int nIndex );
	void NewTable();
	void GetTableField( const char* pszKey );

	bool GetTableValue( const char* pszKey, SI64* psiValue );
	bool GetTableValue( const char* pszKey, REAL64* pfValue );
	bool GetTableValue( const char* pszKey, bool* pbValue );
	bool GetTableValue( const char* pszKey, char* pszValue, size_t nSize );

	bool GetTableValue( SI64 siKey, SI64* psiValue );
	bool GetTableValue( SI64 siKey, REAL64* pfValue );
	bool GetTableValue( SI64 siKey, bool* pbValue );
	bool GetTableValue( SI64 siKey, char* pszValue, size_t nSize );

	void SetTableValue( const char* pszKey, SI64 siValue);
	void SetTableValue( const char* pszKey, SI32 siValue)	{	SetTableValue(pszKey, (SI64)siValue);	}
	void SetTableValue( const char* pszKey, REAL64 fValue);
	void SetTableValue( const char* pszKey, REAL32 fValue)	{	SetTableValue(pszKey, (REAL32)fValue);	}
	void SetTableValue( const char* pszKey, const char* pszValue);

	void SetTableValue( SI64 siKey, SI64 siValue);
	void SetTableValue( SI64 siKey, REAL64 fValue);
	void SetTableValue( SI64 siKey, const char* pszValue);
};

#endif