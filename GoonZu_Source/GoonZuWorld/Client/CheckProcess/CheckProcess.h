#pragma once

#include "CommonLogic.h"

const SI32 Const_Max_MacroList		= 10;
const SI32 Const_Max_Module_Name	= 64;
const SI32 Const_Max_Exit_Count		= 300;
const SI32 Const_Max_MacAddress		= 20;
const SI32 Const_Max_IP				= 64;
const SI32 Const_Max_SendList		= 20;


// 프로세스 체크 실행 상태
enum CHECKPROCESS_STATE
{
	CHECKPROCESS_STATE_READY = 0,
	CHECKPROCESS_STATE_PROCESSING
};

// 프로세스 체크 실행 타입
enum CHECKPROCESS_TYPE
{
	CHECKPROCESS_TYPE_NONE = 0,
	
	CHECKPROCESS_TYPE_MULTICLIENT,	// 멀티클라이언트(군주) 카운트
	CHECKPROCESS_TYPE_MACRO,		// 매크로 프로그램 카운트
	CHECKPROCESS_TYPE_MODULENAME,	// 직접 입력한 모듈이름
	
	//-----------------------------------
	// 새로운 체크 타입은 위에 추가해 주세요
	//-----------------------------------
	CHECKPROCESS_TYPE_MAX
};

class cltCheckProcessMacro
{
// Function Area
public:
	cltCheckProcessMacro()	{ Init();	};
	~cltCheckProcessMacro()	{			};

	void Init( void )
	{
		ZeroMemory( this, sizeof(cltCheckProcessMacro) );
	}

	void Set( TCHAR* pszMacroName )
	{
		if ( pszMacroName )
		{
			StringCchCopy( m_szMacroName, sizeof(m_szMacroName), pszMacroName );
		}
	}

	// Interface Function
	void SetMacroName( TCHAR* pszMacroName )	
	{
		if ( pszMacroName )
		{
			StringCchCopy( m_szMacroName, sizeof(m_szMacroName), pszMacroName );
		}
	}

	TCHAR* GetMacroName( void ) { return m_szMacroName; }


private:


// Variable Area
public:
private:
	TCHAR m_szMacroName[Const_Max_Module_Name];
};

class cltCheckProcessMacroList
{
// Function Area
public:
	cltCheckProcessMacroList();
	~cltCheckProcessMacroList();

	void Init( void );

	bool AddMacroName( TCHAR* pszMacroName );

	bool GetMacroNameFirst( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize );
	bool GetMacorNameNext( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize );
	bool GetMacorNameNow( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize );
	bool GetMacorNameFromIndex( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize, IN SI32 siIndex );

	void LoadMacroList( void );

private:

// Variable Area
public:
private:
	cltCheckProcessMacro*	m_pclMacro[Const_Max_MacroList];
	SI32					m_siListCount;	// 현재 리스트의 개수
	SI32					m_siReadCount;	// 현재 몇번째 리스트를 읽었는지 저장

};

// 메니저에 쓰일 개인정보 저장
class cltCheckProcessPersonInfo
{
public:
	cltCheckProcessPersonInfo();
	~cltCheckProcessPersonInfo();

	void	Init();
	void	Set( SI32 siPersonID, SI32 siCharID, SI16 siProcessCount, TCHAR* pszIP, TCHAR* pszMacAddress )
	{
		m_siPersonID		= siPersonID;
		m_siCharID			= siCharID;
		m_siProcessCount	= siProcessCount;

		if ( pszIP )
		{
			StringCchCopy( m_szIP, Const_Max_IP, pszIP );
		}

		if ( pszMacAddress )
		{
			StringCchCopy( m_szMacAddress, Const_Max_MacAddress, pszMacAddress );
		}

	}
	
	// IP와 Mac Address가 같은지 
	bool IsSame( cltCheckProcessPersonInfo* pclPersonInfo )
	{
		if ( NULL == pclPersonInfo )
		{
			return false;
		}

		if ( (_tcscmp(m_szIP, pclPersonInfo->GetIP()) == 0) && (_tcscmp(m_szMacAddress, pclPersonInfo->GetMacAddress()) == 0) )
		{
			return true;
		}

		return false;
	}

	// Interface Function

	// PersonID
	void	SetPersonID( SI32 siPersonID )			{ m_siPersonID = siPersonID;			}
	SI32	GetPersonID( void )						{ return m_siPersonID;					}

	// CharID
	void	SetCharID( SI32 siCharID )				{ m_siCharID = siCharID;				}
	SI32	GetCharID( void )						{ return m_siCharID;					}

	// ProcessCount
	void	SetProcessCount( SI16 siProcessCount )	{ m_siProcessCount = siProcessCount;	}
	SI16	GetProcessCount( void )					{ return m_siProcessCount;				}

	// IP
	void	SetIP( TCHAR* pszIP )
	{
		if ( pszIP )
		{
			StringCchCopy( m_szIP, Const_Max_IP, pszIP );
		}
	}
	TCHAR*	GetIP( void )							{ return m_szIP;						}

	// MacAddress
	void	SetMacAddress( TCHAR* pszMacAddress )
	{
		if ( pszMacAddress )
		{
			StringCchCopy( m_szMacAddress, Const_Max_MacAddress, pszMacAddress );
		}
	}
	TCHAR*	GetMacAddress( void )					{ return m_szMacAddress;				}



// Variable Area
public:
private:
	SI32	m_siPersonID;							// PersonID
	SI32	m_siCharID;								// 캐릭터ID
	SI16	m_siProcessCount;						// 프로세스 개수
	TCHAR	m_szIP[Const_Max_IP];					// IP
	TCHAR	m_szMacAddress[Const_Max_MacAddress];	// MacAddress
	
};

//------------------------------------------------------------------------------------------------------------------------------
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
//------------------------------------------------------------------------------------------------------------------------------

// 서버용
class cltCheckProcessManager
{
// Function Area
public:
	cltCheckProcessManager();
	~cltCheckProcessManager();

	void Init( void );
	void InitForLog( void );
	void DestroyPersonInfo( void );

	void Action( void );

	bool SendOrderToClient( SI32 siGMPersonID, SI32 siType, TCHAR* pszModuleName=NULL );

	void SendResultToGM( SI16* psiList );

	void CompareSameUser( IN SI16* psiMultiClientList, OUT SI16* psiSendList );

	bool AddPersonInfo( SI32 siCharID, SI16 siProcessCount, TCHAR* pszMacAddress );

	void WriteLog( void );

	bool GetSaveLogFileName( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize );

	void LoadMacroList( void );
	
private:


// Variable Area
public:
private:
	SI16						m_siState;								// 현재 메니져 실행 상태
	SI16						m_siType;								// 실행 타입
	SI32						m_siPersonCount;						// 현재 저장된 유저 정보 수

	SI32						m_siGMPersonID;							// 집계된 정보를 보낼 GM ID

	TCHAR						m_szModuleName[Const_Max_Module_Name];

	cltCheckProcessMacroList	m_clMacroList;

	cltCheckProcessPersonInfo*	m_pclCP_PersonInfo[MAX_PERSON_NUMBER];	// 저장될 정보

	NUpdateTimer				m_kUpdateTimer_WaitTime;				// 요청을 하고 기다리는 시간

};

//------------------------------------------------------------------------------------------------------------------------------
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
//------------------------------------------------------------------------------------------------------------------------------

// 클라이언트용
class cltCheckProcess
{
// Function Area
public:
	cltCheckProcess();
	~cltCheckProcess();

	void Init( void );

	void ExcuteCheckProcess( SI32 siType, TCHAR* pszModuleName );

	SI32 GetGoonZuProcessCount( void );
	SI32 GetProcessCountFormModuleName( const TCHAR* pszModuleName );

private:
	BOOL GetGoonZuModuleName( TCHAR* pszGoonZuName, SI32 siTextSize );

// Variable Area
private:
	TCHAR m_szGoonZuModuleName[Const_Max_Module_Name];


};
