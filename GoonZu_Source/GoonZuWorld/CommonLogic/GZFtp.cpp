#include <windows.h>
#include <direct.h>
#include <shlwapi.h>
#include <stdio.h>
#include "GZFtp.h"
#include <Dbghelp.h>

#include "CommonLogic.h"
#include "../Client/Client.h"

GZFtpSendFuncPtr	cltGZFtp::m_sendfuncptr;
GZFtpErrorHandler	cltGZFtp::m_errorhandler;
GZFtpRecvCompleted	cltGZFtp::m_recvcompletedfuncptr;

VOID	cltGZFtp::InitializeStaticVariable( GZFtpSendFuncPtr sendfuncptr, GZFtpErrorHandler errorhandler, GZFtpRecvCompleted recvcompletedfuncptr )
{
	m_sendfuncptr			=	sendfuncptr;
	m_errorhandler			=	errorhandler;
	m_recvcompletedfuncptr	=	recvcompletedfuncptr;
}

cltGZFtp::cltGZFtp()
{
	m_openmode	=	GZFTP_OPENMODE_NONE;
	
	ZeroMemory( m_szFileName, sizeof( m_szFileName ) );

	m_dwID		=	0;

	ZeroMemory( &m_recvinfo, sizeof( m_recvinfo ) );
	ZeroMemory( &m_sendinfo, sizeof( m_sendinfo ) );
}

cltGZFtp::~cltGZFtp()
{
}

DWORD	cltGZFtp::Initialize( GZFTPOpenMode mode, CHAR *pszFileName, CHAR *pszsendername )
{
	DWORD	dwResult = GZFTP_OK;

	if( m_openmode != GZFTP_OPENMODE_NONE )				
	{
		return	GZFTP_WORKING;
	}		

	strcpy( m_szFileName, pszFileName );

	m_openmode	=	mode;			

	ZeroMemory( &m_recvinfo, sizeof( m_recvinfo ) );
	ZeroMemory( &m_sendinfo, sizeof( m_sendinfo ) );

	switch( m_openmode )
	{
	case GZFTP_OPENMODE_SEND:

		if( ( dwResult = OpenInSendMode( pszFileName ) ) != GZFTP_OK )
		{
			goto	END;
		}

		break;

	case GZFTP_OPENMODE_RECV:
		{
			// ������ �޴°� Ŭ���̾�Ʈ�ϱ� Ŭ���̾�Ʈ �ð��� ���� 
			cltClient* pclclient = (cltClient*)pclClient;

			TCHAR	szFullFileNamePath[ MAX_PATH ];
			TCHAR	szFileNamePath[ 256 ];
			TCHAR	szFileName[ 256 ];
			TCHAR	szTempFileName[ 256 ];

			sprintf( szFileNamePath, "recvfiles\\%s\\", pszsendername);
			sprintf( szTempFileName, "%s.gma", pszsendername);

			// ������ ���ϸ��� �ֳ� Ȯ�� �Ѵ� 	
			WIN32_FIND_DATA FindData;
			HANDLE hFind;
			for ( SI32 sifileindex = 0 ; sifileindex< MAX_MACROACTION_SAMENAME_COUNT ; sifileindex++)
			{
				TCHAR	szTempFullFileNamePath[ MAX_PATH ];				
				sprintf( szTempFullFileNamePath, "%s%s", szFileNamePath,szTempFileName); 
				hFind = FindFirstFile((LPCSTR)szTempFullFileNamePath, &FindData);
				if (INVALID_HANDLE_VALUE == hFind) 
				{
					break;
				}
				else
				{
					sprintf( szTempFileName, "%s(%d).gma", pszsendername,sifileindex+1);
				}
			}
			sprintf( szFileName, "%s.rcv", szTempFileName);


			sprintf( szFullFileNamePath, "%s%s", szFileNamePath,szFileName); 

			strncpy( m_recvinfo.szSenderName, pszsendername, sizeof( m_recvinfo.szSenderName ) - 1 );
			
			if( ( dwResult = OpenInRecvMode( szFullFileNamePath ) ) != GZFTP_OK )
			{
				return	dwResult;
			}
			
			m_recvinfo.state	=	GZFTP_RECVSTATE_INITIALIZING;

			// ID�� �����Ѵ�.
			m_dwID		=	GenerateID();
		}

		break;
		
	}

END:

	if( dwResult != GZFTP_OK )
	{
		Free();
	}

	return	dwResult;
}

VOID	cltGZFtp::Free()
{
	// ������ �޴� ������ �־��ٸ� �����Ѵ�.
	if( m_openmode == GZFTP_OPENMODE_RECV )
	{
		if( m_recvinfo.state != GZFTP_RECVSTATE_RECV_COMPLETED )
		{
			if( strlen( m_szFileName) != 0 )
			{
				DeleteFile( m_szFileName );
			}
		}
	}
	

	m_openmode	=	GZFTP_OPENMODE_NONE;
	
	ZeroMemory( m_szFileName, sizeof( m_szFileName ) );

	m_dwID		=	0;

	ZeroMemory( &m_recvinfo, sizeof( m_recvinfo ) );
	ZeroMemory( &m_sendinfo, sizeof( m_sendinfo ) );
}

DWORD	cltGZFtp::SetRecvDataInfo( LONG ltotalbytes )
{		
	if( ltotalbytes == 0 )									return	GZFTP_INVALID_SIZE;
	
	if( m_openmode != GZFTP_OPENMODE_RECV )					return	GZFTP_INVALID_MODE;
	if( m_recvinfo.state != GZFTP_RECVSTATE_INITIALIZING )	return	GZFTP_INVALID_RECVSTATE;
	
	m_recvinfo.ltotalbytes	=	ltotalbytes;
	m_recvinfo.lrecvbytes	=	0;	

	m_recvinfo.state			=	GZFTP_RECVSTATE_RECV_READY;
	m_recvinfo.dwlastrecvtime	=	timeGetTime();

	return	GZFTP_OK;
}

DWORD	cltGZFtp::SetSendDataInfo( DWORD dwID, SI32 _personid_from, SI32 _personid_to, BOOL bDeleteFile )
{
	m_dwID						=	dwID;

	m_sendinfo.personid_from	=	_personid_from;
	m_sendinfo.personid_to		=	_personid_to;
	m_sendinfo.bDeleteFile		=	bDeleteFile;

	return	GZFTP_OK;
}

VOID	cltGZFtp::Poll()
{
	switch( m_openmode )
	{
	case GZFTP_OPENMODE_SEND:

		PollInSendMode();

		break;

	case GZFTP_OPENMODE_RECV:

		PollInRecvMode();

		break;
	}

}

VOID	cltGZFtp::PollInSendMode()
{
	FILE	*fp = NULL;
	DWORD	dwResult = GZFTP_OK;
	CHAR	buffer[ GZFTP_PACKET_SIZE ];
	LONG	lsendbytes;

	if( m_sendinfo.bissending == true )												
	{
		if( timeGetTime() - m_sendinfo.dwlastsenttime > GZFTP_SENDPACKET_TIMEOUT )
		{
			m_errorhandler( m_dwID, GZFTP_SEND_TIMEOUTED );
		}

		return;
	}

	if( ( fp = fopen( m_szFileName, "rb" ) ) == NULL )
	{
		dwResult	=	GZFTP_OPENFILE_ERROR;
		goto	END;
	}

	fseek( fp, m_sendinfo.lsentbytes, SEEK_SET );

	lsendbytes	=	min( m_sendinfo.ltotalbytes - m_sendinfo.lsentbytes, GZFTP_PACKET_SIZE );	

	if( fread( buffer, lsendbytes, 1, fp ) == 0 )
	{
		dwResult	=	GZFTP_READFILE_ERROR;
		goto	END;
	}
	
	if( m_sendfuncptr( m_dwID, m_sendinfo.personid_from, m_sendinfo.personid_to, buffer, lsendbytes ) == FALSE )
	{
		dwResult	=	GZFTP_SEND_ERROR;
		goto	END;
	}

	m_sendinfo.lsentbytes	+=	lsendbytes;
	
	dwResult = GZFTP_OK;

	m_sendinfo.bissending		=	true;

	m_sendinfo.dwlastsenttime	=	timeGetTime();

END:


	if( fp )
	{
		fclose( fp );
		fp	=	NULL;
	}

	if( dwResult != GZFTP_OK )
	{
		if( m_errorhandler )
		{
			m_errorhandler( m_dwID, dwResult );
		}
	}
}

VOID	cltGZFtp::PollInRecvMode()
{
	switch( m_recvinfo.state )
	{
	case GZFTP_RECVSTATE_INITIALIZING:

		break;

	case GZFTP_RECVSTATE_RECV_READY:
		{
			CHAR	szRenameFile[ MAX_PATH ];

			if( timeGetTime() - m_recvinfo.dwlastrecvtime > GZFTP_SENDPACKET_TIMEOUT )					
			{
				m_errorhandler( m_dwID, GZFTP_RECV_TIMEOUTED );
				return;
			}

			if( m_recvinfo.lrecvbytes <  m_recvinfo.ltotalbytes )							return;		
		
			strcpy( szRenameFile, m_szFileName );

			PathRemoveExtension( szRenameFile );
			
			CopyFile( m_szFileName, szRenameFile, FALSE );
			DeleteFile( m_szFileName );

			if( m_recvcompletedfuncptr )
			{
				m_recvcompletedfuncptr( m_dwID, m_recvinfo.szSenderName, szRenameFile );
			}

			m_recvinfo.state	=	GZFTP_RECVSTATE_RECV_COMPLETED;

			Free();
		}

		break;
	}
}

DWORD	cltGZFtp::OnRecv( DWORD dwID, CHAR *buffer, LONG size )
{
	FILE	*fp = NULL;
	DWORD	dwResult = GZFTP_OK;

	if( m_dwID != dwID )															return	GZFTP_INVALID_ID;

	if( ( m_recvinfo.lrecvbytes + size ) > m_recvinfo.ltotalbytes )					
	{
		dwResult	=	GZFTP_WRONG_SIZE;
		goto	END;
	}

	if( ( fp = fopen( m_szFileName, "ab" ) ) == NULL )			
	{
		dwResult	=	GZFTP_OPENFILE_ERROR;
		goto	END;
	}

	if( fwrite( buffer, size, 1, fp ) != 1 )										
	{
        dwResult	=	GZFTP_WRITEFILE_ERROR;
		goto	END;
	}

	m_recvinfo.lrecvbytes		+=	size;
	m_recvinfo.dwlastrecvtime	=	timeGetTime();

	dwResult = GZFTP_OK;
		
END:

	if( fp )
	{
		fclose( fp );
		fp	=	NULL;
	}
	
	return	dwResult;
}

GZFTPOpenMode	cltGZFtp::GetOpenMode()
{
	return	m_openmode;
}

DWORD	cltGZFtp::GenerateID()
{
	return	DWORD( timeGetTime() );
}

DWORD	cltGZFtp::OpenInSendMode( CHAR *pszFileName )
{	
	WIN32_FILE_ATTRIBUTE_DATA	fad;
	__int64						filesize;

	if( GetFileAttributesEx( pszFileName, GetFileExInfoStandard, &fad ) == 0 )
	{
		return	GZFTP_OPENFILE_ERROR;
	}

	filesize	=	fad.nFileSizeHigh << 32;
	filesize	|=	fad.nFileSizeLow;

	if( filesize == 0 )
	{
		return	GZFTP_SIZEIS_ZERO;
	}

	if( filesize > GZFTP_MAX_FILE_SIZE )							
	{
		return	GZFTP_FILESIZE_TOOBIG;
	}

	m_sendinfo.ltotalbytes	=	LONG( filesize );

	return	GZFTP_OK;
}
	
DWORD	cltGZFtp::OpenInRecvMode( CHAR *pszFileName )
{
	FILE	*fp = NULL;	

	strcpy( m_szFileName, pszFileName );
		
	// ������ ������ ���� �� �����Ƿ� �����Ѵ�.
	DeleteFile( m_szFileName );

	if( MakeSureDirectoryPathExists( m_szFileName ) == FALSE )
	{
		return	GZFTP_CREATE_FILE_ERROR;
	}	

	return	GZFTP_OK;
}

VOID	cltGZFtp::OnSentPacketArrived( DWORD dwID )
{
	if( m_dwID != dwID )													return;

	if( m_sendinfo.bissending != true )										return;

	m_sendinfo.bissending	=	false;

	// ���� �����͸� ��� ���´ٸ� �����Ѵ�.
	if( m_sendinfo.ltotalbytes == m_sendinfo.lsentbytes )
	{
		if( m_sendinfo.bDeleteFile == TRUE )
		{
			// ������ ��Ŷ���� ���������� ���ٸ� ������ �����Ѵ�.
			DeleteFile( m_szFileName );
		}

		Free();
	}
}

LONG	cltGZFtp::GetTotalBytes()
{
	switch( GetOpenMode() )
	{
	case GZFTP_OPENMODE_SEND:

		return	m_sendinfo.ltotalbytes;

		break;

	case GZFTP_OPENMODE_RECV:

		return	m_recvinfo.ltotalbytes;

		break;

	default:

		return	0;
	}
}

DWORD	cltGZFtp::GetID()
{
	return	m_dwID;
}
