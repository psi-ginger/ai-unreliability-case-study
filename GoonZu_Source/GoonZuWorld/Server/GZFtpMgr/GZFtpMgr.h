#pragma once

#include <windows.h>

#include <directives.h>

#include "../../CommonLogic/gzftp.h"

#define	GZFTP_MAX_FTP_NUM		20	

class	cltGameMsgGZFTPProtocol;
class	cltCharServer;

class cltGZFtpMgr
{
private:

	cltGZFtp	m_clGZFtp[ GZFTP_MAX_FTP_NUM ];

	DWORD		m_dwLastError;

public:

	cltGZFtpMgr();
	~cltGZFtpMgr();

	VOID	Initialize();
	VOID	Free();

	DWORD	CreateFtp( GZFTPOpenMode mode, CHAR *pszFileName, CHAR *pszsendername );
	VOID	DeleteFtp( DWORD dwID );

	SI32	FindEmptySlot();

	cltGZFtp*	GetFtpByIndex( SI32 siIndex );
	cltGZFtp*	GetFtpByID( DWORD dwID );

	VOID	DisaptchGZFtpMsg( cltCharServer *pclCharServer, cltGameMsgGZFTPProtocol *pclinfo );

	VOID	WriteLog( CHAR *pszFormat, ... );

	DWORD	GetLastError();

	VOID	Poll();

	static	VOID	GZFtpErrorHandle( DWORD dwID, DWORD errorcode );
	static	VOID	GZFtpRecvCompleted( DWORD dwID, CHAR *pszSenderName, CHAR *pszFileName );

};


