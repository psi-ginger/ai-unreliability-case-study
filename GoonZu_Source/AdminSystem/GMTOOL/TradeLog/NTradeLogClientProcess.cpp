//=============================================================================
//
//	NNewLogClientProcess.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "../stdafx.h"
#include "../resource.h"
#include "NTradeLogClientProcess.h"
#include "TradeLogFrame.h"

#define DBMessageMap(AAA,BBB) m_pDBNewLogMsgPtr[AAA].Func = &NTradeLogClientProcess::BBB

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
NTradeLogClientProcess::NTradeLogClientProcess(CMDIChildWnd* m_pkParent)
{
	m_pkTradeLogFrame			= m_pkParent;
	m_pNTradeLogProcessSession	= NULL;

	ZeroMemory(&m_stRequestTradeItemMsg, sizeof(m_stRequestTradeItemMsg));	
	
	m_siProcessState		= TRADELOGCLIENTPROCESS_STATE_IDLE;

	m_szNTradeLogServerIP[0]		= '\0';
	m_uiNTradeLogServerBasePort		= 0;
	m_siReason						= 0;

	m_pDBNewLogMsgPtr = new stDBNewLogMsgPtr[TRADELOG_DBMSG_END];

	if(m_pDBNewLogMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < TRADELOG_DBMSG_END; i++)
			m_pDBNewLogMsgPtr[i].Func = &NTradeLogClientProcess::NullFuncPtr;

		DBMessageMap(TRADELOG_DBMSG_RESPONSE_TRADE_LOG, DoMsg_DBMSG_RESPONSE_TRADE_LOG);
	}
}

NTradeLogClientProcess::~NTradeLogClientProcess()
{
	if(m_pDBNewLogMsgPtr != NULL)
		delete [] m_pDBNewLogMsgPtr;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NTradeLogClientProcess::Init()
{
	m_sf.CreateFactory( &m_sm, MAX_TRADELOGCLIENT_CHANNEL, MAX_TRADELOGCLIENT_PACKET_SIZE * MAX_TRADELOGCLIENT_PACKET_COUNT, MAX_TRADELOGCLIENT_PACKET_SIZE * MAX_TRADELOGCLIENT_PACKET_COUNT );
	m_iocp.CreateIOCP( &m_sm, 1 );
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NTradeLogClientProcess::Run()
{
	m_sm.Update();

	switch(m_siProcessState)
	{
	case TRADELOGCLIENTPROCESS_STATE_IDLE:
		{
			// 기다린다.
			if(	m_pNTradeLogProcessSession != NULL && 
				m_pNTradeLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNTradeLogProcessSession->CloseSocket();
				m_pNTradeLogProcessSession = NULL;
			}
		}
		break;

	case TRADELOGCLIENTPROCESS_STATE_REQUESTCONNECTING:
		{
			m_pNTradeLogProcessSession = NULL;

			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNTradeLogServerIP, m_uiNTradeLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}
				m_dwStartConnectTime = GetTickCount();
				m_siProcessState = TRADELOGCLIENTPROCESS_STATE_CONNECTING;
			}
		}
		break;
	case TRADELOGCLIENTPROCESS_STATE_CONNECTING:
		{
			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNTradeLogServerIP, m_uiNTradeLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}


				DWORD dwDiff = GetTickCount() - m_dwStartConnectTime;
				if( dwDiff >= MAX_TRADELOG_CONNECTING_TIME )
				{
					pSession->CloseSocket();

					m_siProcessState = m_siProcessState = TRADELOGCLIENTPROCESS_STATE_IDLE;;

					ZeroMemory(&m_stRequestTradeItemMsg, sizeof(m_stRequestTradeItemMsg));
					m_siProcessState = TRADELOGCLIENTPROCESS_STATE_IDLE;

					AfxMessageBox("All Channel is Working... Please Try a few minutes later", MB_OK);

					CTradeLogFrame* pkTradeLogFrame = (CTradeLogFrame*)m_pkTradeLogFrame;
					if(pkTradeLogFrame != NULL)
					{
						pkTradeLogFrame->SearchEnable(TRUE);
					}
				}
			}
			else if(pSession != NULL && pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNTradeLogProcessSession = pSession;
				m_siProcessState = TRADELOGCLIENTPROCESS_STATE_ESTABLISH;
			}
		}
		break;
	case TRADELOGCLIENTPROCESS_STATE_ESTABLISH:
		{
			if(	m_pNTradeLogProcessSession != NULL &&
				m_pNTradeLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_stRequestTradeItemMsg.usCharID = m_pNTradeLogProcessSession->GetPerson()->GetCharID();

				m_pNTradeLogProcessSession->GetSendBuffer()->Write( (BYTE*) &m_stRequestTradeItemMsg );
				m_siProcessState = TRADELOGCLIENTPROCESS_STATE_WAITLOG;
			}
		}
		break;
	case TRADELOGCLIENTPROCESS_STATE_WAITLOG:
		{
			if(	m_pNTradeLogProcessSession != NULL &&
				m_pNTradeLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				SI32 siCount = m_pNTradeLogProcessSession->GetRecvBuffer()->GetPacketCount();
				for(SI32 i=0; i<siCount; i++)
				{
					sPacketHeader* pPacket = (sPacketHeader*)m_pNTradeLogProcessSession->GetRecvBuffer()->GetFirstPacket();

					if( pPacket != NULL )
					{
						MessageProc( pPacket, m_pNTradeLogProcessSession);
					}

					// 처리 후 패킷을 버퍼에서 지운다
					m_pNTradeLogProcessSession->GetRecvBuffer()->RemoveFirstPacket();
				}
			}
			else if(m_pNTradeLogProcessSession != NULL &&
				m_pNTradeLogProcessSession->GetState() == SESSION_STATE_DISCONNECTED)
			{
				m_siProcessState = TRADELOGCLIENTPROCESS_STATE_FINISHLOG;
			}
		}
		break;

	case TRADELOGCLIENTPROCESS_STATE_WRONGID:
		{
			if(	m_pNTradeLogProcessSession != NULL)
			{				
				m_pNTradeLogProcessSession->CloseSocket();
				m_pNTradeLogProcessSession = NULL;
			}
			m_siProcessState = TRADELOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case TRADELOGCLIENTPROCESS_STATE_WRONGPASS:
		{
			if(	m_pNTradeLogProcessSession != NULL)
			{				
				m_pNTradeLogProcessSession->CloseSocket();
				m_pNTradeLogProcessSession = NULL;

				AfxMessageBox("Wrong Password !!!", MB_OK);
			}
			m_siProcessState = TRADELOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case TRADELOGCLIENTPROCESS_STATE_DONOTINPUTID:
		{
			m_siProcessState = TRADELOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case TRADELOGCLIENTPROCESS_STATE_FINISHLOG:
		{
			if(	m_pNTradeLogProcessSession != NULL)
			{
				CTradeLogFrame* pkTradeLogFrame = (CTradeLogFrame*)m_pkTradeLogFrame;
				m_siProcessState = TRADELOGCLIENTPROCESS_STATE_IDLE;

				m_pNTradeLogProcessSession->CloseSocket();
				m_pNTradeLogProcessSession = NULL;

				if(pkTradeLogFrame != NULL)
				{
					if(pkTradeLogFrame != NULL)
					{
						CString Msg;
						Msg.Format("[ %d ] Items Find", pkTradeLogFrame->m_siSearchCount);
						AfxMessageBox(Msg, MB_OK);

						pkTradeLogFrame->m_siSearchCount = 0;
					}
					else
					{
						AfxMessageBox("Search Complete !!", MB_OK);
					}
				}
			}			
		}
		break;
	}
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NTradeLogClientProcess::MessageProc(sPacketHeader* pPacket, ITZSession* pSession)
{
	if( pPacket == NULL )	return;
	if( pSession == NULL )	return;

	if(pPacket->usCmd >= 0 && pPacket->usCmd <= TRADELOG_DBMSG_END)
	{
		(this->*(this->m_pDBNewLogMsgPtr[pPacket->usCmd].Func))(pPacket, pSession);
	}
	else
	{
		NullFuncPtr(pPacket, pSession);
	}
}

//=============================================================================
//  @Name   IsEstablished()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL NTradeLogClientProcess::IsEstablished()
{
	if(m_pNTradeLogProcessSession == NULL)									return FALSE;
	if(m_pNTradeLogProcessSession->GetState() != SESSION_STATE_ESTABLISHED)	return FALSE;

	return TRUE;
}

//=============================================================================
//  @Name   RequsetTradeLog()
//! @author LeeKH						
//! @date   2009-04-20
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL NTradeLogClientProcess::RequsetTradeLog(sDBRequest_TradeItemLog* pLog)
{
	if(pLog == NULL)					return FALSE;

	if(m_siProcessState != TRADELOGCLIENTPROCESS_STATE_IDLE)		return FALSE;

	m_stRequestTradeItemMsg.Set(pLog);
	m_siProcessState = TRADELOGCLIENTPROCESS_STATE_REQUESTCONNECTING;

	return TRUE;
}

BOOL NTradeLogClientProcess::SendMsg(sPacketHeader *pPacket)
{
	if(m_pNTradeLogProcessSession == NULL)					return FALSE;
	if(m_pNTradeLogProcessSession->GetSendBuffer() == NULL)	return FALSE;

	if(!IsEstablished())								return FALSE;

	m_pNTradeLogProcessSession->GetSendBuffer()->Write( (BYTE *)pPacket);

	return TRUE;
}

//=============================================================================
//  @Name   Stop()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL NTradeLogClientProcess::Stop()
{
	if(m_siProcessState != TRADELOGCLIENTPROCESS_STATE_WAITLOG)		return FALSE;

	CTradeLogFrame* pkTradeLogFrame = (CTradeLogFrame*)m_pkTradeLogFrame;
	if(pkTradeLogFrame == NULL)										return FALSE;

	pkTradeLogFrame->SearchEnable(TRUE);
	m_siProcessState = TRADELOGCLIENTPROCESS_STATE_FINISHLOG;

	return TRUE;
}

//=============================================================================
//  @Name   NullFuncPtr()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NTradeLogClientProcess::NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession)
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", pPacket->usCmd);
	MessageBox(NULL, "DBManager Error", buffer, MB_OK);
}

//=============================================================================
//  @Name   DoMsg_DBMSG_RESPONSE_LOG()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NTradeLogClientProcess::DoMsg_DBMSG_RESPONSE_TRADE_LOG(sPacketHeader* pPacket, ITZSession* pSession)
{
	sDBResponse_TradeItemLog	*pRecvMsg = (sDBResponse_TradeItemLog *)pPacket;

	CTradeLogFrame* pkTradeLogFrame = (CTradeLogFrame*)m_pkTradeLogFrame;
	if(pkTradeLogFrame == NULL)						return;

	if(m_pNTradeLogProcessSession->GetPerson()->GetCharID() != pRecvMsg->usCharID)		return;

	if(pRecvMsg->siResult == 1)
	{
		pkTradeLogFrame->AddLogData(pRecvMsg->stLogData);
	}
	else if (pRecvMsg->siResult == -1)
	{
		pkTradeLogFrame->SearchEnable(TRUE);
		m_siProcessState = TRADELOGCLIENTPROCESS_STATE_WRONGID;
	}
	else if (pRecvMsg->siResult == -2)
	{
		pkTradeLogFrame->SearchEnable(TRUE);
		m_siProcessState = TRADELOGCLIENTPROCESS_STATE_WRONGPASS;
	}
	else if (pRecvMsg->siResult == -10)
	{
		pkTradeLogFrame->SearchEnable(TRUE);
		m_siProcessState = TRADELOGCLIENTPROCESS_STATE_FINISHLOG;
		//AfxMessageBox( "DB Execute Error !!", MB_OK);
	}
	else if (pRecvMsg->siResult == -100)
	{
		pkTradeLogFrame->AddLogData(pRecvMsg->stLogData);
		pkTradeLogFrame->SearchEnable(TRUE);
		m_siProcessState = TRADELOGCLIENTPROCESS_STATE_FINISHLOG;
	}
}
