#include <shlwapi.h>

#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/GZFtp.h"

#include "../InterfaceMgr/DialogController.h"
#include "Client.h"

void cltClient::DoMsg_GAMEMSG_GZFTP_PROTOCOL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgGZFTPProtocol	*pclinfo	=	( cltGameMsgGZFTPProtocol* )pclMsg->cData;	

	switch( pclinfo->msg )
	{
	case cltGameMsgGZFTPProtocol::GZFTP_INITIALIZE:
		{
			DWORD	dwResult;

			TCHAR	szFileName[ 1024 ];
			TCHAR	szFolderName[ 1024 ];
			TCHAR	szFileNamePath[ 1024 ];

			// 수신 파일명을 결정한다.
			strcpy( szFileName, pclinfo->initialize_data.szFilePath );

			PathStripPath( szFileName );		

			sprintf( szFolderName, "recvfiles\\%s\\", pclinfo->initialize_data.szSenderName );

			PathCombine( szFileNamePath, szFolderName, szFileName );			

			dwResult = m_pclGZFtp->Initialize( GZFTP_OPENMODE_RECV, szFileNamePath, pclinfo->initialize_data.szSenderName );

			if( dwResult != GZFTP_OK )
			{
				// 실패 메시지 박스를 띄운다.	
				switch( dwResult )
				{
				case GZFTP_WORKING:
					{
						TCHAR* pTitle = GetTxtFromMgr(40357)	;
						char* pText =  GetTxtFromMgr(40358)	;

						m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
					}
					break;

				case GZFTP_CREATE_FILE_ERROR:
					{
						TCHAR* pTitle = GetTxtFromMgr(40357)	;
						TCHAR* pText =  GetTxtFromMgr(40360)	;

						m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
					}

					break;					

				default:

					{
						TCHAR* pTitle = GetTxtFromMgr(40357)	;

						NTCHARString256 szErrorMsg = GetTxtFromMgr(40361);
						szErrorMsg.Replace( "#ERROR#", SI32ToString(dwResult) );

						m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, szErrorMsg);
					}

					break;
				}		
			}
			else
			{
				cltGameMsgGZFTPProtocol::__req_getfile_data	data;

				strcpy( data.szFilePath, pclinfo->initialize_data.szFilePath );

				data.dwID					=	m_pclGZFtp->GetID();
				data.bDeleteFile			=	pclinfo->initialize_data.boption_delete;

				cltGameMsgGZFTPProtocol	clinfo( cltGameMsgGZFTPProtocol::GZFTP_REQUEST_GETFILE, 
					pclinfo->initialize_data.siyourPersonID, pclinfo->initialize_data.siotherPersonID, ( BYTE* )&data, sizeof( data ) );
				cltMsg clMsg(GAMEMSG_GZFTP_PROTOCOL, sizeof( clinfo ), (BYTE*)&clinfo);
				SendMsgToServer((sPacketHeader*)&clMsg);			

				{
					TCHAR* pTitle = GetTxtFromMgr(40357)	;
					TCHAR* pText =  GetTxtFromMgr(40362)	;

					m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
				}
			}
		}

		break;
	case cltGameMsgGZFTPProtocol::GZFTP_REQUEST_GETFILE:
		{
			DWORD	dwResult;

			dwResult = m_pclGZFtp->Initialize( GZFTP_OPENMODE_SEND, pclinfo->req_getfile_data.szFilePath, "" );			

			cltGameMsgGZFTPProtocol::__res_getfile_data	data;

			data.dwID		=	pclinfo->req_getfile_data.dwID;
			data.dwResult	=	dwResult;

			if( dwResult == GZFTP_OK )
			{				
				data.ltotalbytes		=	m_pclGZFtp->GetTotalBytes();

				m_pclGZFtp->SetSendDataInfo( pclinfo->req_getfile_data.dwID, pclinfo->personid_to, pclinfo->personid_from, pclinfo->req_getfile_data.bDeleteFile );
			}			

			cltGameMsgGZFTPProtocol	clinfo( cltGameMsgGZFTPProtocol::GZFTP_RESPONSE_GETFILE, pclinfo->personid_to, pclinfo->personid_from, ( BYTE* )&data, sizeof( data ) );
			cltMsg clMsg(GAMEMSG_GZFTP_PROTOCOL, sizeof( clinfo ), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);
		}		
		break;

	case cltGameMsgGZFTPProtocol::GZFTP_RESPONSE_GETFILE:
		{
			DWORD	dwResult = pclinfo->res_getfile_data.dwResult ;		

			if( pclinfo->res_getfile_data.dwResult != GZFTP_OK )
			{
				switch(dwResult)
				{
				case GZFTP_OPENFILE_ERROR:
					{
						TCHAR* pTitle = GetTxtFromMgr(40357)	;
						char* pText =  GetTxtFromMgr(40363)	;

						m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
					}

					break;

				case GZFTP_FILESIZE_TOOBIG:
					{
						TCHAR* pTitle = GetTxtFromMgr(40357)	;
						char* pText =  GetTxtFromMgr(40364)	;

						m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
					}

					break;

				default:

					{
						TCHAR* pTitle = GetTxtFromMgr(40357)	;

						NTCHARString256 szErrorMsg = GetTxtFromMgr(40365);
						szErrorMsg.Replace( "#ERROR#", SI32ToString(dwResult) );

						m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, szErrorMsg);
					}

					break;

				}

				// GZFtp를 해제한다.
				m_pclGZFtp->Free();
			}
			else
			{
				dwResult	=	m_pclGZFtp->SetRecvDataInfo( pclinfo->res_getfile_data.ltotalbytes );

				if( dwResult != GZFTP_OK )
				{
					switch( dwResult )
					{
					default:

						{
							TCHAR* pTitle = GetTxtFromMgr(40357)	;
							NTCHARString256 szErrorMsg = GetTxtFromMgr(40365);
							szErrorMsg.Replace( "#ERROR#", SI32ToString(dwResult) );


							m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, szErrorMsg);
						}

						break;						
					}

					// GZFtp를 해제한다.
					m_pclGZFtp->Free();
				}
				else
				{

				}
			}
		}

		break;

	case cltGameMsgGZFTPProtocol::GZFTP_REQUEST_DATA:
		{

			DWORD									dwResult;
			cltGameMsgGZFTPProtocol::__res_filedata	data;

			dwResult		=	m_pclGZFtp->OnRecv( pclinfo->req_filedata.dwID, ( CHAR* )pclinfo->req_filedata.filedata, pclinfo->req_filedata.lsize );

			data.dwResult	=	dwResult;
			data.dwID		=	m_pclGZFtp->GetID();

			cltGameMsgGZFTPProtocol	clinfo( cltGameMsgGZFTPProtocol::GZFTP_RESPONSE_DATA, pclinfo->personid_to, pclinfo->personid_from, ( BYTE* )&data, sizeof( data ) );
			cltMsg clMsg(GAMEMSG_GZFTP_PROTOCOL, sizeof( clinfo ), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);
		}


		break;

	case cltGameMsgGZFTPProtocol::GZFTP_RESPONSE_DATA:
		{

			if( pclinfo->res_filedata.dwResult == GZFTP_OK )	
			{
				m_pclGZFtp->OnSentPacketArrived( pclinfo->res_filedata.dwID );
			}		
			else
			{
				// GZFtp를 해제한다.
				m_pclGZFtp->Free();
			}
		}

		break;
	}

}

BOOL	GZFtpSendFunc( DWORD dwID, SI32 sipersonid_from, SI32 sipersonid_to, CHAR *buffer, LONG size )
{
	cltClient	*client	=	( cltClient* )pclClient;

	cltGameMsgGZFTPProtocol::__req_filedata	data;

	data.dwID	=	dwID;

	memcpy( data.filedata, buffer, size );
	data.lsize	=	size;	

	cltGameMsgGZFTPProtocol	clinfo( cltGameMsgGZFTPProtocol::GZFTP_REQUEST_DATA, sipersonid_from, sipersonid_to, ( BYTE* )&data, sizeof( data ) );
	cltMsg clMsg(GAMEMSG_GZFTP_PROTOCOL, sizeof( clinfo ), (BYTE*)&clinfo);
	client->SendMsgToServer((sPacketHeader*)&clMsg);

	return	TRUE;
}


VOID	GZFtpErrorHandleFunc( DWORD dwID, DWORD errorcode )
{		
	cltClient	*client	=	( cltClient* )pclClient;


	switch( errorcode )
	{
	case GZFTP_RECV_TIMEOUTED:
		{

			TCHAR* pTitle = GetTxtFromMgr(40357)	;
			TCHAR* pText =  GetTxtFromMgr(40366)	;

			client->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	}

	// 종료한다.
	client->m_pclGZFtp->Free();
}

VOID	GZFtpRecvCompletedFunc( DWORD dwID, CHAR *pszSenderName, CHAR *pszFileName )
{
	cltClient	*pclclient	=	( cltClient* )pclClient;

	TCHAR	szFileName[ MAX_PATH ];
	TCHAR* pTitle = GetTxtFromMgr(40357)	;

	strncpy( szFileName, pszFileName, MAX_PATH - 1 );	
	PathStripPath( szFileName );		

	NTCHARString256 szExplain = GetTxtFromMgr(40367);
	szExplain.Replace( "#name1#",  pszSenderName );
	szExplain.Replace( "#name2#",  szFileName );

	pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitle, szExplain );	

}