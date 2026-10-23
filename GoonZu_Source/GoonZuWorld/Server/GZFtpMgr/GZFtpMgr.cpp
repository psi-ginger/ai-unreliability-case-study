#include <shlwapi.h>
#include <stdio.h>
#include "../../Server/Server.h"

#include "../../common/char/charserver/char-server.h"
#include "../../commonlogic/msgtype.h"

#include "GZFtpMgr.h"

#include "../MacroReportedMgr/MacroReportedMgr.h"

extern cltCommonLogic * pclClient;

extern	void GetServerNameFromServerUnique(SI32 ServerUnique, char* szServername);

cltGZFtpMgr::cltGZFtpMgr()
{
	cltGZFtp::InitializeStaticVariable( NULL, GZFtpErrorHandle, GZFtpRecvCompleted );	
	
	// 매크로 로그 파일을 작성할 폴더를 생성한다.
	mkdir( "./config/macroLog" );
}

cltGZFtpMgr::~cltGZFtpMgr()
{
	Free();
}

VOID	cltGZFtpMgr::Initialize()
{		
	m_dwLastError	=	GZFTP_OK;
}

VOID	cltGZFtpMgr::Free()
{
	SI32	i;

	for( i = 0; i < GZFTP_MAX_FTP_NUM; i++ )
	{
		if( ( m_clGZFtp[ i ].GetID() == 0 ) )		continue;

		m_clGZFtp[ i ].Free();
	}
}

DWORD	cltGZFtpMgr::CreateFtp( GZFTPOpenMode mode, CHAR *pszFileName, CHAR *pszsendername )
{
	SI32		siemptySlot;
	cltGZFtp	*pclGZFtp;	

	// 수신 모드만 가능하다.
	if( mode != GZFTP_OPENMODE_RECV )															return	0;

	if( ( siemptySlot = FindEmptySlot() ) == -1 )												return	0;

	if( ( pclGZFtp = GetFtpByIndex( siemptySlot ) ) == NULL )									return	0;

	if( ( m_dwLastError = pclGZFtp->Initialize( mode, pszFileName, pszsendername ) ) != GZFTP_OK )	
	{
		return	0;
	}

	return	pclGZFtp->GetID();
}

VOID	cltGZFtpMgr::DeleteFtp( DWORD dwID )
{
	cltGZFtp	*pclGZFtp;

	if( ( pclGZFtp = GetFtpByID( dwID ) ) == NULL )			return;

	pclGZFtp->Free();
}

SI32	cltGZFtpMgr::FindEmptySlot()
{
	SI32	i;

	for( i = 0; i < GZFTP_MAX_FTP_NUM; i++ )
	{
		if( m_clGZFtp[ i ].GetID() != 0 )			continue;

		return	i;
	}

	return	-1;
}

cltGZFtp*	cltGZFtpMgr::GetFtpByIndex( SI32 siIndex )
{
	if( ( siIndex < 0 ) || ( siIndex >= GZFTP_MAX_FTP_NUM ) )	return	NULL;

	return	&m_clGZFtp[ siIndex ];
}

cltGZFtp*	cltGZFtpMgr::GetFtpByID( DWORD dwID )
{
	SI32	i;

	for( i = 0; i < GZFTP_MAX_FTP_NUM; i++ )
	{
		if( m_clGZFtp[ i ].GetID() != dwID )		continue;

		return	&m_clGZFtp[ i ];
	}

	return	NULL;

}


VOID	cltGZFtpMgr::DisaptchGZFtpMsg( cltCharServer *pclCharServer, cltGameMsgGZFTPProtocol *pclinfo )
{
	cltServer	*server	=	( cltServer* )pclClient;

	// 수신의 경우에만 처리를 해주면 된다.

	switch( pclinfo->msg )
	{
	case cltGameMsgGZFTPProtocol::GZFTP_INITIALIZE:
		{
			DWORD	dwID;

			CHAR	szServerName[ 1024 ] = "";

			CHAR	szFileName[ 1024 ] = "";			
			CHAR	szFileNamePath[ 1024 ] = "";

			CHAR	szTestFile[ 1024 ];

			time_t	now;
			tm		*ptm;

			time( &now );

			ptm		=	localtime( &now );

			// 수신 파일명을 결정한다.
			strcpy( szFileName, pclinfo->initialize_data.szFilePath );
			
			PathStripPath( szFileName );		
			
			//GetServerNameFromServerUnique( server->szServerName, szServerName );

			strcpy( szServerName, server->szServerName );

			sprintf( szFileNamePath, "config\\macro\\%04d%02d%02d_%s_%s_%03d.gma",	ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, 
																					szServerName, pclinfo->initialize_data.szSenderName, 
																					server->m_pclMacroReportedMgr->GetReportedNum( pclCharServer->pclCI->GetPersonID() ) );						

			dwID	= CreateFtp( GZFTP_OPENMODE_RECV, szFileNamePath, pclinfo->initialize_data.szSenderName );

			WriteLog( "파일 요청. [ID = %d] [송신자:%s] [수신파일이름:%s][송신파일이름:%s] [ERROCODE :%d]", dwID, pclinfo->initialize_data.szSenderName, szFileNamePath, pclinfo->initialize_data.szFilePath, GetLastError() );

			if( dwID != 0 )
			{
				cltGameMsgGZFTPProtocol::__req_getfile_data	data;

				strcpy( data.szFilePath, pclinfo->initialize_data.szFilePath );

				data.dwID					=	dwID;
				data.bDeleteFile			=	pclinfo->initialize_data.boption_delete;
					
				cltGameMsgGZFTPProtocol	clinfo( cltGameMsgGZFTPProtocol::GZFTP_REQUEST_GETFILE, 
												pclinfo->initialize_data.siyourPersonID, pclinfo->initialize_data.siotherPersonID, ( BYTE* )&data, sizeof( data ) );
				cltMsg clMsg(GAMEMSG_GZFTP_PROTOCOL, sizeof( clinfo ), (BYTE*)&clinfo);
				pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
			}
			else
			{

			}
		}		

		break;

	case cltGameMsgGZFTPProtocol::GZFTP_RESPONSE_GETFILE:
		{
			DWORD		dwResult;		
			cltGZFtp	*pclGZFtp;

			if( ( pclGZFtp = GetFtpByID( pclinfo->res_getfile_data.dwID ) ) == NULL )				return;			

			if( pclinfo->res_getfile_data.dwResult != GZFTP_OK )
			{
				// GZFtp를 해제한다.
				DeleteFtp( pclinfo->res_getfile_data.dwID );
			}
			else
			{
				

				dwResult	=	pclGZFtp->SetRecvDataInfo( pclinfo->res_getfile_data.ltotalbytes );

				if( dwResult != GZFTP_OK )
				{
					// GZFtp를 해제한다.
					pclGZFtp->Free();
				}
			}
		}		

		break;

	case cltGameMsgGZFTPProtocol::GZFTP_REQUEST_DATA:
		{

			DWORD		dwResult;
			cltGZFtp	*pclGZFtp;
			cltGameMsgGZFTPProtocol::__res_filedata	data;

			if( ( pclGZFtp = GetFtpByID( pclinfo->req_filedata.dwID ) ) == NULL )				return;

			dwResult		=	pclGZFtp->OnRecv( pclinfo->req_filedata.dwID, ( CHAR* )pclinfo->req_filedata.filedata, pclinfo->req_filedata.lsize );

			data.dwResult	=	dwResult;
			data.dwID		=	pclGZFtp->GetID();
			
			cltGameMsgGZFTPProtocol	clinfo( cltGameMsgGZFTPProtocol::GZFTP_RESPONSE_DATA, pclinfo->personid_to, pclinfo->personid_from, ( BYTE* )&data, sizeof( data ) );
			cltMsg clMsg(GAMEMSG_GZFTP_PROTOCOL, sizeof( clinfo ), (BYTE*)&clinfo);
			pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
		}

		break;
	}
}


VOID	cltGZFtpMgr::GZFtpErrorHandle( DWORD dwID, DWORD errorcode )
{
	cltServer	*server	=	( cltServer* )pclClient;
	cltGZFtp	*pclGZFtp;

	if( ( pclGZFtp = server->m_pclGZFtpMgr->GetFtpByID( dwID ) ) == NULL )		return;

	//server->m_pclGZFtpMgr->WriteLog( "에러가 발생하였습니다 [ID = %d] [ERRCODE:%s]", dwID, errorcode );

	server->m_pclGZFtpMgr->DeleteFtp( dwID );	
}

VOID	cltGZFtpMgr::GZFtpRecvCompleted( DWORD dwID, CHAR *pszSenderName, CHAR *pszFileName )
{	
	cltServer	*server	=	( cltServer* )pclClient;	

	server->m_pclGZFtpMgr->WriteLog( "수신이 완료되었습니다. [ID:%d] [송신자:%s] [파일이름:%s]", dwID, pszSenderName, pszFileName );
}


VOID	cltGZFtpMgr::WriteLog(  CHAR *pszFormat, ... )
{
	time_t		ltime_now;
	tm			*ptm_now;
	FILE		*fp = NULL;
	va_list		vl;    
	BOOL		bIsSuccess = FALSE;
	CHAR		sztmpBuffer[ 8192 ] = "";
	CHAR		sztmpBuffer2[ 8192 ] = "";
	CHAR		szFileName[ 1024 ] = "";

	time( &ltime_now );
	ptm_now	=	localtime( &ltime_now );

	va_start( vl, pszFormat );
	wvsprintf( sztmpBuffer, pszFormat, vl );
	va_end( vl );

	sprintf( sztmpBuffer2, "%04d-%02d-%02d %02d:%02d:%02d\t%s",	ptm_now->tm_year + 1900, ptm_now->tm_mon + 1, ptm_now->tm_mday, ptm_now->tm_hour, ptm_now->tm_min, ptm_now->tm_sec, 
		sztmpBuffer );			

	sprintf( szFileName, "./config/macroLog/%04d%02d%02d.txt", ptm_now->tm_year + 1900, ptm_now->tm_mon + 1, ptm_now->tm_mday );

	if( ( fp = fopen( szFileName, "at" ) ) != NULL )
	{
		fprintf( fp, "%s\n", sztmpBuffer2 );	
		
		fclose( fp );
		fp	=	NULL;
	}	
}

DWORD	cltGZFtpMgr::GetLastError()
{
	return	m_dwLastError;
}


VOID	cltGZFtpMgr::Poll()
{
	SI32	i;

	for( i = 0; i < GZFTP_MAX_FTP_NUM; i++ )
	{
		if( ( m_clGZFtp[ i ].GetID() == 0 ) )		continue;

		m_clGZFtp[ i ].Poll();
	}
}
