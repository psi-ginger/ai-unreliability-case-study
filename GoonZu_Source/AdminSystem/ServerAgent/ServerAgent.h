#ifndef _SERVERAGENT_H
#define _SERVERAGENT_H


#include "ITZPerson.h"

class AgentMasterProtocol;
class AgentServerProtocol;
class SessionList;

extern AgentMasterProtocol	g_amp;
extern AgentServerProtocol	g_asp;
extern SessionList			g_sessionList;

class cMaster;
class cServer;

class cMaster : public ITZPerson
{
public:
	
	cMaster() {

		InitializeCriticalSection( &m_cs );

		m_siServerID = 0; 
		m_szFilePath[ 0 ] = NULL; 
	
	};

	~cMaster() {

		DeleteCriticalSection( &m_cs );		
	};

	virtual void Init();
	virtual void Close();

	void wait() {
		EnterCriticalSection( &m_cs );
	};

	void Release() {
		LeaveCriticalSection( &m_cs );
	}

public:
	// 파일경로를 설정한다 
	void SetFilePath( char *str ) {

		memcpy( m_szFilePath, str, 128 );
		m_szFilePath[ 127 ] = NULL;

	};


	// server id를 설정한다
	void SetServerID( SI32 siServerID )
	{
		InterlockedExchange( (LPLONG)&m_siServerID, (LONG)siServerID );
	};

	void SetServer( cServer *pServer )
	{
		InterlockedExchange( (LPLONG)&m_pServer, (LONG)pServer );
	};


	BOOL IsEqualFilePath( char *str )
	{
		BOOL ret;

		char szA[ 128 ], szB[ 128 ];

		strcpy( szA, m_szFilePath );
		strcpy( szB, str );

		int i, c;
		c = strlen( szA );
		for( i = 0; i < c; ++i ) {
			if( szA[ i ] == '.' ) { szA[ i ] = NULL; break; };
		}

		c = strlen( szB );
		for( i = 0; i < c; ++i ) {
			if( szB[ i ] == '.' ) { szB[ i ] = NULL; break; };
		}

		if( _stricmp( szA, szB ) == 0 ) ret = TRUE; else ret = FALSE;

		return ret;
	};

	SI32 GetServerID() { return m_siServerID; };
	

public:
	cServer*			m_pServer;

	SI32				m_siServerID;
	char				m_szFilePath[ 128 ];

	DWORD				m_dwLastRecvTick;
	DWORD				m_dwLastSendTick;

	CRITICAL_SECTION	m_cs;

};


class cServer : public ITZPerson
{
public:
	cServer() { 

		InitializeCriticalSection( &m_cs );

		m_siServerID = 0; 
		m_szFilePath[ 0 ] = NULL; 
	};

	~cServer() {

		DeleteCriticalSection( &m_cs );
	};

	virtual void Init();
	virtual void Close();
	
	void wait() {
		EnterCriticalSection( &m_cs );
	};

	void Release() {
		LeaveCriticalSection( &m_cs );
	}


public:


	// 파일경로를 설정한다 
	void SetFilePath( char *str ) {


		memcpy( m_szFilePath, str, 128 );
		m_szFilePath[ 127 ] = NULL;

	};


	// server id를 설정한다
	void SetServerID( SI32 siServerID )
	{
		InterlockedExchange( (LPLONG)&m_siServerID, (LONG)siServerID );
	};

	void SetMaster( cMaster *pMaster )
	{

		InterlockedExchange( (LPLONG)&m_pMaster, (LONG)pMaster );

	};

	BOOL IsEqualFilePath( char *str )
	{
		BOOL ret;

		char szA[ 128 ], szB[ 128 ];

		strcpy( szA, m_szFilePath );
		strcpy( szB, str );

		int i, c;
		c = strlen( szA );
		for( i = 0; i < c; ++i ) {
			if( szA[ i ] == '.' ) { szA[ i ] = NULL; break; };
		}

		c = strlen( szB );
		for( i = 0; i < c; ++i ) {
			if( szB[ i ] == '.' ) { szB[ i ] = NULL; break; };
		}

		if( _stricmp( szA, szB ) == 0 ) ret = TRUE; else ret = FALSE;

		return ret;

	};
	
	SI32 GetServerID() { return m_siServerID; };

public:

	cMaster*			m_pMaster;

	SI32				m_siServerID;
	char				m_szFilePath[ 128 ];

	CRITICAL_SECTION	m_cs;
};


#endif

