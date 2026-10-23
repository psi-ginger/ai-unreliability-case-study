#ifndef _USERMANAGER_H
#define _USERMANAGER_H

#include "NDataTypes.h"
#include "NList.h"
#include "NHashTableString.h"

#define USERSTATE_NONE				0
#define USERSTATE_FRONTAUTH			1
#define USERSTATE_GAMELOGIN			2

class CUserInfo
{
public:
	CUserInfo() {};
	~CUserInfo() {};

	TCHAR			m_szAccountID[ 20 ];
	TCHAR			m_szPW[ 20 ];
	TCHAR			m_szIP[ 20 ];
	SI32			m_idnum;
	SI32			m_siCount;
	SI32			m_siState;

	SI32			m_siAccountStatus;

};


class CUserManager
{
public:
	CUserManager();
	~CUserManager();

	void						FrontAuth( TCHAR *accountid, TCHAR *pw, TCHAR *ip, int idnum, int accountstatus );
	int							GameAuth( int idnum );
	void						GameLogOut( int idnum );
	void						WriteLogFile( char *szLog );

	NHashTable< int, int >		m_hashtable;

	NList< CUserInfo >			m_userlist;
	NListPool< CUserInfo >		m_userlistpool;

	CRITICAL_SECTION			m_cs;

	int							m_HanGameJPCount;
	int							m_GamaniaJPCount;
	int							m_GamepotJPCount;
	int							m_NDEJPCount;
	int							m_NEXONJPCount;
};



#endif
