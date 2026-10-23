#include "stdafx.h"
#include "Patch.h"
#include "PatchDlg.h"


#include "FrontProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "ITZEncrypt.h"

#include "../../Server/FrontServer/FrontMsg.h"


FrontProcess::FrontProcess()
{
	m_pFrontSession = NULL;
	m_szIP1[ 0 ] = NULL;
	m_szIP2[ 0 ] = NULL;

	m_bReqVersionInfo = FALSE;
	m_bVersion = FALSE;

	m_pPatchDlg = NULL;

	m_bSecretKey = FALSE;

	m_usTryConnectCount = 0;

}


FrontProcess::~FrontProcess()
{

}

void FrontProcess::Init( char *szIP1, char *szIP2, UI16 usPort, CPatchDlg *pDlg )
{
	strcpy( m_szIP1, szIP1 );
	strcpy( m_szIP2, szIP2 );

	m_usPort = usPort;

	m_sf.CreateFactory( &m_sm, 1, 50 * 1000, 50 * 1000 );

	ITZSession *pSession = m_sm.GetSession( 0 );
	
	InterlockedExchange( (LPLONG)&m_pFrontSession, (LONG)pSession );

	overlapped.CreateOverlapped( &m_sm );	

	m_bReqVersionInfo = FALSE;
	m_bVersion = FALSE;

	m_pPatchDlg = pDlg;

}

void FrontProcess::	Run()
{
	m_sm.Update();

	if( m_pFrontSession == NULL ) return;


	// 프론트 세션이 접속 중이 아니고, 버전 정보를 받은 상태가 아니라면, 접속 시도한다
	if( m_pFrontSession->GetState() == SESSION_STATE_NONE &&
		m_bVersion == FALSE ) 
	{
		m_usTryConnectCount++;

		if ( m_usTryConnectCount < 100 )
		{
			if( m_sm.Connect( 0, m_szIP1, m_usPort ) == m_pFrontSession )
			{
				m_bSecretKey = FALSE;
			}
			else 
			{
			}
		}
		else
		{
			if( m_sm.Connect( 0, m_szIP2, m_usPort ) == m_pFrontSession ) 
			{
				m_bSecretKey = FALSE;
			}
			else 
			{
			}
		}
	}

	// 프론트 세션이 성공적으로 연결된 상태이면, 
	else if( m_pFrontSession->GetState() == SESSION_STATE_ESTABLISHED ) 
	{
		// 버전 정보를 요청 하지 않았으면 요청한다
		if( m_bReqVersionInfo == FALSE && m_bSecretKey == TRUE ) 
		{

			sRequest_PatchInfo sendMsg;

			sendMsg.packetHeader.usCmd = FRONTMSG_REQUEST_PATCHINFO;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.uiCurrentVersion = m_pPatchDlg->m_uiCurrentVersion;

			m_pFrontSession->GetEncrypt()->Encode( (UI08 *)&sendMsg, m_encrBuf );

			m_pFrontSession->GetSendBuffer()->Write( (BYTE *)m_encrBuf );

			m_bReqVersionInfo = TRUE;
		}


		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pFrontSession->GetRecvBuffer()->GetFirstPacket();		
		
		if( pPacket != NULL ) 
		{										
			// 패킷이 있으면,
			
			char packet[ PACKET_MAX_SIZE ];
			memcpy( packet, pPacket, pPacket->usSize );
			m_pFrontSession->GetRecvBuffer()->RemoveFirstPacket();

			m_pPatchDlg->FrontMsgProcess( (sPacketHeader *)packet );
			
		}
	}
}
