#ifndef _GASH_H
#define _GASH_H

#define REQUESTCODE_AUTH			1
#define REQUESTCODE_LOGIN			2
#define REQUESTCODE_LOGOUT			3

#include "authmsg.h"

class gashmsginfo
{
public:
	gashmsginfo()
	{
		ZeroMemory( this, sizeof( gashmsginfo ) );
	}

	~gashmsginfo() {};

public:
	BOOL	m_bValid;

	SI32	m_requestCode;

	UI16	m_usPacketUnique;
	UI16	m_usCharID;

	TCHAR	m_szID[ 20 ];
	TCHAR	m_szPW[ 20 ];
	TCHAR	m_szIP[ 20 ];	

	union{
		BYTE					cData[ 128 ];
		sResponse_FrontAuth		response_frontauth;
	};

};


class CGash
{
	friend unsigned __stdcall gash_worker( LPVOID param );

public:
	CGash();
	~CGash();

	void				Init( SI32 maxlist, char *szIP );
	void				CreateThread();

	BOOL				Connect( UI16 port );
	void				CloseSocket();

	BOOL				AddRequestInfo( SI16 requestcode, UI16 packetUnique, UI16 charID, char *szID, char *szPW, char *szIP, BYTE *pdata, int datasize );

	BOOL				RequestMsg( gashmsginfo *pInfo );
	void				ResponseMsg( gashmsginfo *pInfo );

	void				WriteLogFile( char *szLog );

public:
	char				m_szIP[ 64 ];
	//UI16				m_usPort;

	SOCKET				m_socket;
	SOCKADDR_IN			m_sockaddr;
	fd_set				m_fdset;

	char				m_recvbuf[ 4096 ];
	int					m_recvbytes;

	BOOL				m_bRun;

	gashmsginfo*		m_pGashMsgInfo;		

	SI32				m_siStartIndex;
	SI32				m_siEndIndex;
	SI32				m_siMaxList;
	SI32				m_siCount;

	HANDLE				m_hThread;

	CRITICAL_SECTION	m_cs;
};


#endif
