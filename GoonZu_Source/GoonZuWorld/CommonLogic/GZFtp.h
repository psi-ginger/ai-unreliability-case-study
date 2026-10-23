#pragma once

#include <windows.h>

#include <directives.h>

#define	GZFTP_PACKET_SIZE			1024		// 1K	
#define	GZFTP_MAX_FILE_SIZE			1048576		// 1MB

#define	GZFTP_SENDPACKET_TIMEOUT	5000		// 5000ms
#define	GZFTP_RECVPACKET_TIMEOUT	5000		// 5000ms

enum	GZFTPOpenMode{ GZFTP_OPENMODE_NONE, GZFTP_OPENMODE_SEND, GZFTP_OPENMODE_RECV };

enum	GZFTPRecvState{ GZFTP_RECVSTATE_NONE, GZFTP_RECVSTATE_INITIALIZING, GZFTP_RECVSTATE_RECV_READY, GZFTP_RECVSTATE_RECV_COMPLETED };

#define	GZFTP_OK						0
#define	GZFTP_INVALID_ID				1
#define	GZFTP_WORKING					2
#define	GZFTP_NOT_EXIST_FILE			3
#define	GZFTP_FILESIZE_TOOBIG			4
#define	GZFTP_OPENFILE_ERROR			5
#define	GZFTP_CREATE_FILE_ERROR			6
#define	GZFTP_WRITEFILE_ERROR			7
#define	GZFTP_WRONG_SIZE				8
#define	GZFTP_READFILE_ERROR			9
#define	GZFTP_SEND_ERROR				10
#define GZFTP_SEND_TIMEOUTED			11
#define GZFTP_RECV_TIMEOUTED			12
#define	GZFTP_INVALID_MODE				13
#define	GZFTP_NOT_RECVSTATE				14
#define	GZFTP_INVALID_SIZE				15
#define	GZFTP_INVALID_RECVSTATE			16
#define	GZFTP_SIZEIS_ZERO				17

typedef	BOOL	( *GZFtpSendFuncPtr )( DWORD dwID, SI32 sipersonid_from, SI32 sipersonid_to, CHAR *buffer, LONG size );
typedef	VOID	( *GZFtpErrorHandler )( DWORD dwID, DWORD errorcode );
typedef	VOID	( *GZFtpRecvCompleted )( DWORD dwID, CHAR *pszSenderName, CHAR *pszFileName );

class	cltGZFtp
{
private:

	static	GZFtpSendFuncPtr	m_sendfuncptr;
	static	GZFtpErrorHandler	m_errorhandler;
	static	GZFtpRecvCompleted	m_recvcompletedfuncptr;

	GZFTPOpenMode	m_openmode;	

	CHAR			m_szFileName[ MAX_PATH ];	

	DWORD			m_dwID;

	union
	{
		struct
		{
			GZFTPRecvState	state;		

			CHAR	szSenderName[ 32 ];

			LONG	ltotalbytes;
			LONG	lrecvbytes;			

			DWORD	dwlastrecvtime;			
			

		}m_recvinfo;

		struct
		{		

			LONG	ltotalbytes;
			LONG	lsentbytes;

			SI32	personid_from;
			SI32	personid_to;

			bool	bissending;

			BOOL	bDeleteFile;

			DWORD	dwlastsenttime;
		}m_sendinfo;
	};

protected:

	DWORD	GenerateID();

	DWORD	OpenInSendMode( CHAR *pszFileName );
	DWORD	OpenInRecvMode( CHAR *pszFileName );

	VOID	PollInSendMode();
	VOID	PollInRecvMode();

public:
	cltGZFtp();
	~cltGZFtp();

	static	VOID	InitializeStaticVariable( GZFtpSendFuncPtr sendfuncptr, GZFtpErrorHandler errorhandler, GZFtpRecvCompleted recvcompletedfuncptr );

	DWORD	Initialize( GZFTPOpenMode mode, CHAR *pszFileName, CHAR *pszsendername );
	VOID	Free();

	DWORD	SetRecvDataInfo( LONG ltotalbytes );
	DWORD	SetSendDataInfo( DWORD dwID, SI32 _personid_from, SI32 _personid_to, BOOL bdeletefile );

	VOID	Poll();

	GZFTPOpenMode	GetOpenMode();


	DWORD	OnRecv( DWORD dwID, CHAR *buffer, LONG size );		

	VOID	OnSentPacketArrived( DWORD dwID );

	LONG	GetTotalBytes();

	DWORD	GetID();

};

