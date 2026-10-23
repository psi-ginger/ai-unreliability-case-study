#include "..\Common\CommonHeader.h"
#include "..\CommonLogic\CommonLogic.h"

#include "BillProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../NBillServer/NBillMsg.h"

#include "../Server.h"

extern cltCommonLogic* pclClient;

BillProcess::BillProcess()
{
	m_pBillSession = NULL;
	
	m_BillIP[ 0 ] = NULL;	
}

BillProcess::~BillProcess()
{

}

void BillProcess::Init( TCHAR *billIP, UI16 billPort )
{

	m_sf.CreateFactory( &m_sm, 1, 50000, 50000 );

	m_pBillSession = m_sm.GetSession( 0 );
	

	m_iocp.CreateIOCP( &m_sm, 1 );

	StringCchCopy( m_BillIP, 20, billIP );
	m_BillPort = billPort;

	m_refreshInfo.state = 2;	//??????????????????????????????
}

void BillProcess::Run()
{

	if( m_pBillSession == NULL) return;

	cltServer *pclServer = (cltServer *)pclClient;



	m_sm.Update();

//	DWORD		currentTick;

	//  빌 세션이 연결되지 않은 상태이면,
	if( m_pBillSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_BillIP, m_BillPort ) == m_pBillSession ) {
			m_BillLastTick = GetTickCount() + BILL_HEARTBEAT_INTERVAL;
		} else {

		}

	} 
	// 프론트 세션이 성공적으로 연결된 상태이면,
	else if( m_pBillSession->GetState() == SESSION_STATE_ESTABLISHED )
	{
/*
		// heartbeat 보내기

		currentTick = GetTickCount();
		if( currentTick >= m_BillLastTick + BILL_HEARTBEAT_INTERVAL ) {
			m_BillLastTick = currentTick;

			sReportToFront_GameHeartbeat	sendMsg;

			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.packetHeader.usCmd = BILLMSG_REPORT_GAMEHEARTBEAT;

			// 현재 접속 수 얻기 
			m_refreshInfo.currentConnections = pclServer->pclGameProtocol->GetSessionManager()->GetActiveSessionCount();
			

			sendMsg.serverId = pclServer->siServerID;
			sendMsg.refreshinfo = m_refreshInfo;

			m_pBillSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
		}

*/

		// 버퍼에서 패킷을 하나 꺼내온다. 
		sPacketHeader* pPacket = (sPacketHeader*)m_pBillSession->GetRecvBuffer()->GetFirstPacket();

		if(pPacket != NULL)
		{
			switch(pPacket->usCmd)
			{
			case BILLMSG_RESPONSE_ENCRYPTIDPW:
				{
                    ((cltServer*)pclClient)->DoMsg_BILLMSG_RESPONSE_ENCRYPTIDPW(pPacket);
				}
				break;
			case BILLMSG_RESPONSE_ENCRYPTIDPW_WEB:
				{
					((cltServer*)pclClient)->DoMsg_BILLMSG_RESPONSE_ENCRYPTIDPW_WEB(pPacket);
				}
				break;
			case BILLMSG_RESPONSE_ENCRYPTIDPW_GACHA:
				{
					((cltServer*)pclClient)->DoMsg_BILLMSG_RESPONSE_ENCRYPTIDPW_GACHA(pPacket);
				}
				break;
			default:
				{
					if (pclClient->GameMode != GAMEMODE_SERVER)
						MessageBox(NULL, TEXT("fdsdi9hf"), TEXT("fd9yu2hd"), MB_OK|MB_TOPMOST);
				}
				break;
			}

			m_pBillSession->GetRecvBuffer()->RemoveFirstPacket();
		}		

	}
}

bool BillProcess::SendMsg(sPacketHeader* pPacket)
{
	m_pBillSession->GetSendBuffer()->Write( (BYTE *)pPacket);

	return true;
}