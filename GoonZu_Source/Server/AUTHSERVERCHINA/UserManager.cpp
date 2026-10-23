#include "AuthServer.h"
#include "UserManager.h"
#include "GASH.h"

unsigned int MyHashFunc( int key )
{
	return key;
}


CUserManager::CUserManager()
{
	InitializeCriticalSection( &m_cs );

	m_hashtable.CreateHashTable( 20000, 30000, MyHashFunc );

	m_userlistpool.CreateListPool( 30000 );

	m_userlist.CreateList( &m_userlistpool );
}

CUserManager::~CUserManager()
{
	DeleteCriticalSection( &m_cs );
}

void CUserManager::FrontAuth( TCHAR *accountid, TCHAR *pw, TCHAR *ip, int idnum, int accountstatus )
{
	EnterCriticalSection( &m_cs );

	int index = 0;

	// 이미 해쉬 테이블에 등록 되어 있으면 리턴,,
	if( m_hashtable.Find( idnum, &index ) ) {
		LeaveCriticalSection( &m_cs );
		return;
	}

	// 해쉬 테이블에 등록한다..
	NListNode< CUserInfo > *pNode;

	pNode = m_userlist.PushBack();

	if( pNode == NULL ) {
		LeaveCriticalSection( &m_cs );
		return;
	}

	CUserInfo *pUserInfo = pNode->GetDataPtr();

	memcpy( pUserInfo->m_szAccountID, accountid, 20 );
	pUserInfo->m_szAccountID[ 19 ] = NULL;

	memcpy( pUserInfo->m_szIP, ip, 20 );
	pUserInfo->m_szIP[ 19 ] = NULL;

	memcpy( pUserInfo->m_szPW, pw, 20 );
	pUserInfo->m_szPW[ 19 ] = NULL;

	pUserInfo->m_idnum = idnum;
	pUserInfo->m_siAccountStatus = accountstatus;

	pUserInfo->m_siCount = 0;
	pUserInfo->m_siState = USERSTATE_FRONTAUTH;

	// hash table에 추가
	m_hashtable.Insert( idnum, pNode->GetIndex() );

	printf( "UM>FrontAuth : id:%s, idnum:%d, accountstatus:%d\n",
		pUserInfo->m_szAccountID, pUserInfo->m_idnum, pUserInfo->m_siAccountStatus );
	
	LeaveCriticalSection( &m_cs );
}

int CUserManager::GameAuth( int idnum )
{
	int ret = 0;

	EnterCriticalSection( &m_cs );

	int index = 0;

	// 해쉬 테이블에 등록 되어있지 않으면 리턴,
	if( m_hashtable.Find( idnum, &index ) == FALSE ) {
		LeaveCriticalSection( &m_cs );
		return -1;
	}

	CUserInfo *pUserInfo = m_userlistpool.GetDataPtr( index );

	if( pUserInfo == NULL )
	{
		LeaveCriticalSection( &m_cs );
		return -1;
	}

	pUserInfo->m_siCount++;
	pUserInfo->m_siState = USERSTATE_GAMELOGIN;

	ret = pUserInfo->m_siAccountStatus;

	// 감마니아
	if( ret == 2 )
	{
		g_gash.AddRequestInfo( REQUESTCODE_LOGIN, 
				0,
				0,
				pUserInfo->m_szAccountID,
				pUserInfo->m_szPW,
				pUserInfo->m_szIP,
				0, 0 );
	}

	printf( "UM>GameAuth : id:%s, idnum:%d, accountstatus:%d\n",
		pUserInfo->m_szAccountID, pUserInfo->m_idnum, pUserInfo->m_siAccountStatus );

	char buf[ 4096 ];
	sprintf( buf, "GAME LOGIN ID:%s, IDNUM:%d, COUNT:%d", 
		pUserInfo->m_szAccountID,
		pUserInfo->m_idnum,
		pUserInfo->m_siCount );

	WriteLogFile( buf );

	LeaveCriticalSection( &m_cs );

	return ret;
}


void CUserManager::GameLogOut( int idnum )
{
	EnterCriticalSection( &m_cs );

	int index = 0;

	// 해쉬 테이블에 등록 되어있지 않으면 리턴,
	if( m_hashtable.Find( idnum, &index ) == FALSE ) {
		LeaveCriticalSection( &m_cs );
		return;
	}

	CUserInfo *pUserInfo = m_userlistpool.GetDataPtr( index );

	if( pUserInfo == NULL )
	{
		LeaveCriticalSection( &m_cs );
		return;
	}

	pUserInfo->m_siCount--;


	char buf[ 4096 ];
	sprintf( buf, "GAME LOGOUT ID:%s, IDNUM:%d, COUNT:%d", 
		pUserInfo->m_szAccountID,
		pUserInfo->m_idnum,
		pUserInfo->m_siCount );

	WriteLogFile( buf );

	if( pUserInfo->m_siCount == 0 )
	{
		pUserInfo->m_siState = USERSTATE_NONE;

		// 감마니아 로그아웃..
		if( pUserInfo->m_siAccountStatus == 2 )
		{
				g_gash.AddRequestInfo( REQUESTCODE_LOGOUT, 
					0,
					0,
					pUserInfo->m_szAccountID,
					pUserInfo->m_szPW,
					pUserInfo->m_szIP,
					0, 0 );
		}

		m_hashtable.Remove( idnum );
	}

	LeaveCriticalSection( &m_cs );
}

void CUserManager::WriteLogFile( char *szLog ) 
{

	_SYSTEMTIME time;

	GetLocalTime( &time );

	char filename[ 128 ];
	char fileBuf[ 1024 ];

	sprintf( filename, "AUTH_%02d-%02d.txt", time.wMonth, time.wDay );

	FILE *fp = fopen( filename, "a+t" );
	fseek( fp, 0, SEEK_END );

	wsprintf( fileBuf,"[%02d-%02d-%02d:%02d:%02d.%03d] %s\r\n", 
		time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szLog );

	fwrite( fileBuf, sizeof( char ), strlen( fileBuf ), fp );

	fclose(fp);

}