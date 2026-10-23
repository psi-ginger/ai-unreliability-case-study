#ifndef _SBSLOGIN_H
#define _SBSLOGIN_H

#include "itzbase.h"

#define REQUESTCODE_IDNUMGET		1
#define REQUESTCODE_JOINGAME		2

class sbslogininfo
{
public:
	sbslogininfo()
	{
		m_requestCode = 0;

		m_usPacketUnique = 0;
		m_usFrontCharID = 0;

		m_szID[ 0 ] = NULL;
		m_szPW[ 0 ] = NULL;
		m_szIP[ 0 ] = NULL;
		m_szMacAddress[ 0 ] = NULL;
		m_szCookie[ 0 ] = NULL;

		m_bValid = FALSE;
	}

	~sbslogininfo() {};

public:
	BOOL	m_bValid;
	
	UI16	m_usPacketUnique;
	UI16	m_usFrontCharID;
	char	m_szID[ 20 ];
	char	m_szPW[ 20 ];
	char	m_szIP[ 20 ];	
	char	m_szMacAddress[ 20 ];
	char	m_szCookie[ 20 ];
	UI16	m_requestCode;

};

class SBSLogin
{
friend unsigned __stdcall sbs_worker( LPVOID param );

public:
	SBSLogin();
	~SBSLogin();

	void				Init( SI32 maxlist, char *szIP, char *szIP2, UI16 port );
	void				CreateThread();

	BOOL				Connect();
	void				CloseSocket();

	BOOL				AddAuthUser( UI16 requestcode, UI16 packetUnique, UI16 frontCharID, char *szID, char *szPW, char *szIP, char *szCookie, char *szMacAddress );

	BOOL				RequestAuth();
	void				ResponseAuth();

	void				RequestIDNUMGET();
	void				RequestJOINGAME();

	void				WriteLogFile( char *szLog );
	
public:
	char				m_szIP[ 2 ][ 20 ];
	UI16				m_usPort;

	static SI32			m_siActiveIP;

	SOCKET				m_socket;
	SOCKADDR_IN			m_sockaddr;
	fd_set				m_fdset;

	char				m_recvbuf[ 1024 ];
	int					m_recvbytes;

	BOOL				m_bRun;

	sbslogininfo*		m_pLoginInfo;		
	
	SI32				m_siStartIndex;
	SI32				m_siEndIndex;
	SI32				m_siMaxList;
	SI32				m_siCount;

};

#endif