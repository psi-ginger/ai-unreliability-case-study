#include "../stdafx.h"
#include "GMProcess.h"
#include "GMClientFrame.h"
#include "GMClientView.h"

GMProcess::GMProcess(CDialog* m_pkParent)
{
	m_pGMSession = NULL;
	m_szIP[ 0 ] = NULL;

	m_iPageID = 0;
	m_pDataBuffer = new char[ MAX_SECTION_DATA_BUFFER ];
	m_iDataPos = 0;
	m_bBeginSection = false;

	m_pkGMClientView = m_pkParent;
}

GMProcess::~GMProcess()
{
	delete [] m_pDataBuffer;
	m_pDataBuffer = 0;
}

void GMProcess::Init()
{
	m_szIP[ 0 ] = 0;
	m_usPort = 0;

	m_sf.CreateFactory( &m_sm, 1, 300 * 1000, 300 * 1000 );

	ITZSession *pSession = m_sm.GetSession( 0 );

	InterlockedExchange( (LPLONG)&m_pGMSession, (LONG)pSession );

	m_iocp.CreateIOCP( &m_sm, 1 );

	m_bTryToConnect = false;
	m_bFailedToConnect = false;
	m_bLoggedIn = false;
}

void GMProcess::Connect( char* szIP, UI16 usPort )
{
	if( m_bTryToConnect ) return;

	memcpy( m_szIP, szIP, 20 );
	m_szIP[ 19 ] = 0;

	m_usPort = usPort;

	m_bTryToConnect = true;
	m_bFailedToConnect = false;
	m_bLoggedIn = false;

	m_dwTryConnectTick = GetTickCount();
}

void GMProcess::SendPacket( sPacketHeader* pPacket )
{
	if( m_pGMSession )
	{
		if( m_pGMSession->GetState() == SESSION_STATE_ESTABLISHED )
		{
			m_pGMSession->GetSendBuffer()->Write( (BYTE*)pPacket );
		}
	}
}

bool GMProcess::IsTryingToConnect()
{
	return m_bTryToConnect;
}

bool GMProcess::IsFailedToConnect()
{
	return m_bFailedToConnect;
}

bool GMProcess::IsConnected()
{
	return ( m_pGMSession->GetState() == SESSION_STATE_ESTABLISHED );
}

bool GMProcess::IsLoggedIn()
{
	return ( m_pGMSession->GetState() == SESSION_STATE_ESTABLISHED && m_bLoggedIn );
}

bool GMProcess::IsCompletedPage()
{
	return m_bCompletedPage;
}

bool GMProcess::IsRequested()
{
	return m_bRequested;
}

void GMProcess::Run()
{

	m_sm.Update();

	if( m_pGMSession == NULL ) return;

	if( m_bTryToConnect )
	{
		if( m_pGMSession->GetState() == SESSION_STATE_NONE ) 
		{
			if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pGMSession ) 
			{
				m_bTryToConnect = false;
				m_bRequested = false;
			} 
			else 
			{
				if( GetTickCount() - m_dwTryConnectTick > 3000 )
				{
					m_pGMSession->CloseSocket();
					m_bTryToConnect = false;
					m_bFailedToConnect = true;
				}
			}
		}
		else if( m_pGMSession->GetState() == SESSION_STATE_ESTABLISHED )
		{
			m_pGMSession->CloseSocket();
		}
	}
	// GM세션이 성공적으로 연결된 상태이면,
	else if( m_pGMSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pGMSession->GetRecvBuffer()->GetFirstPacket();		

		if( pPacket != NULL ) 
		{										
			// 패킷이 있으면,

			switch( pPacket->usCmd ) 
			{
			case GMMSG_RESPONSE_LOGIN:
				{
					sGMResponse_Login *pRecvMsg = (sGMResponse_Login *)pPacket;
					
					if( pRecvMsg->m_iResult == 1 )
					{
						m_bLoggedIn = true;
					}
				}
				break;

			case GMMSG_RESPONSE_BEGINPAGE:
				{
					sGMResponse_BeginPage *pRecvMsg = (sGMResponse_BeginPage *)pPacket;

					m_iPageID = pRecvMsg->m_iPageID;
					m_bBeginSection = true;
					m_bCompletedPage = false;
					m_iDataPos = 0;
				}
				break;

			case GMMSG_RESPONSE_ENDPAGE:
				{
					sGMResponse_EndPage *pRecvMsg = (sGMResponse_EndPage *)pPacket;

					if( m_bBeginSection == false ) break;

					if( m_iPageID != pRecvMsg->m_iPageID ) break;

					m_bCompletedPage = true;

					m_bRequested = false;
				}
				break;

			case GMMSG_RESPONSE_SECTION:
				{
					sGMResponse_Section *pRecvMsg = (sGMResponse_Section *)pPacket;

					if( m_bBeginSection == false ) break;

					if( m_iPageID != pRecvMsg->m_iPageID ) break;

					int len = _AnsiToUTF8( pRecvMsg->m_data, &m_pDataBuffer[m_iDataPos], MAX_SECTION_DATA_BUFFER - m_iDataPos );

					m_iDataPos += len;
				}
				break;

			case GMMSG_RESPONSE_INFOFROMSERVER:
				{
					// 서버에 요청했던 정보에대한 패킷 받기
					sGMResponse_InfofromServer *pRecvMsg = (sGMResponse_InfofromServer *)pPacket;
					
					// 그대로 화면에 출력
					CGMClientView* pkGMClientView = (CGMClientView*)m_pkGMClientView;
					if( pkGMClientView == NULL )		return;

					pkGMClientView->AddInfoFromServer(pRecvMsg->m_szInfofromServer);
				}
				break;
			     }

			m_pGMSession->GetRecvBuffer()->RemoveFirstPacket();
		}
	}
}

