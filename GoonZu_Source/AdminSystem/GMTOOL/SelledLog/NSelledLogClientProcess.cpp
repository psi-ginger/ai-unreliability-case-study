//=============================================================================
//
//	NNewLogClientProcess.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "../stdafx.h"
#include "../resource.h"
#include "NSelledLogClientProcess.h"
#include "SelledLogFrame.h"

#define DBMessageMap(AAA,BBB) m_pDBNewLogMsgPtr[AAA].Func = &NSelledLogClientProcess::BBB

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
NSelledLogClientProcess::NSelledLogClientProcess(CMDIChildWnd* m_pkParent)
{
	m_pkSelledLogFrame			= m_pkParent;
	m_pNSelledLogProcessSession	= NULL;

	ZeroMemory(&m_stRequestSelledItemMsg, sizeof(m_stRequestSelledItemMsg));	
	
	m_siProcessState		= SELLEDLOGCLIENTPROCESS_STATE_IDLE;

	m_szNSelledLogServerIP[0]		= '\0';
	m_uiNSelledLogServerBasePort		= 0;
	m_siReason						= 0;

	m_pDBNewLogMsgPtr = new stDBNewLogMsgPtr[TRADELOG_DBMSG_END];

	if(m_pDBNewLogMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < TRADELOG_DBMSG_END; i++)
			m_pDBNewLogMsgPtr[i].Func = &NSelledLogClientProcess::NullFuncPtr;

		DBMessageMap(TRADELOG_DBMSG_RESPONSE_SELLED_LOG, DoMsg_DBMSG_RESPONSE_SELLED_LOG);
	}
}

NSelledLogClientProcess::~NSelledLogClientProcess()
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
void NSelledLogClientProcess::Init()
{
	m_sf.CreateFactory( &m_sm, MAX_SELLEDLOGCLIENT_CHANNEL, MAX_SELLEDLOGCLIENT_PACKET_SIZE * MAX_SELLEDLOGCLIENT_PACKET_COUNT, MAX_SELLEDLOGCLIENT_PACKET_SIZE * MAX_SELLEDLOGCLIENT_PACKET_COUNT );
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
void NSelledLogClientProcess::Run()
{
	m_sm.Update();

	switch(m_siProcessState)
	{
	case SELLEDLOGCLIENTPROCESS_STATE_IDLE:
		{
			// 기다린다.
			if(	m_pNSelledLogProcessSession != NULL && 
				m_pNSelledLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNSelledLogProcessSession->CloseSocket();
				m_pNSelledLogProcessSession = NULL;
			}
		}
		break;

	case SELLEDLOGCLIENTPROCESS_STATE_REQUESTCONNECTING:
		{
			m_pNSelledLogProcessSession = NULL;

			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNSelledLogServerIP, m_uiNSelledLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}
				m_dwStartConnectTime = GetTickCount();
				m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_CONNECTING;
			}
		}
		break;
	case SELLEDLOGCLIENTPROCESS_STATE_CONNECTING:
		{
			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNSelledLogServerIP, m_uiNSelledLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}


				DWORD dwDiff = GetTickCount() - m_dwStartConnectTime;
				if( dwDiff >= MAX_SELLEDLOG_CONNECTING_TIME )
				{
					pSession->CloseSocket();

					m_siProcessState = m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_IDLE;;

					ZeroMemory(&m_stRequestSelledItemMsg, sizeof(m_stRequestSelledItemMsg));
					m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_IDLE;

					AfxMessageBox("All Channel is Working... Please Try a few minutes later", MB_OK);

					CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)m_pkSelledLogFrame;
					if(pkSelledLogFrame != NULL)
					{
						pkSelledLogFrame->SearchEnable(TRUE);
					}
				}
			}
			else if(pSession != NULL && pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNSelledLogProcessSession = pSession;
				m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_ESTABLISH;
			}
		}
		break;
	case SELLEDLOGCLIENTPROCESS_STATE_ESTABLISH:
		{
			if(	m_pNSelledLogProcessSession != NULL &&
				m_pNSelledLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_stRequestSelledItemMsg.usCharID = m_pNSelledLogProcessSession->GetPerson()->GetCharID();

				m_pNSelledLogProcessSession->GetSendBuffer()->Write( (BYTE*) &m_stRequestSelledItemMsg );
				m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_WAITLOG;
			}
		}
		break;
	case SELLEDLOGCLIENTPROCESS_STATE_WAITLOG:
		{
			if(	m_pNSelledLogProcessSession != NULL &&
				m_pNSelledLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				SI32 siCount = m_pNSelledLogProcessSession->GetRecvBuffer()->GetPacketCount();
				for(SI32 i=0; i<siCount; i++)
				{
					sPacketHeader* pPacket = (sPacketHeader*)m_pNSelledLogProcessSession->GetRecvBuffer()->GetFirstPacket();

					if( pPacket != NULL )
					{
						MessageProc( pPacket, m_pNSelledLogProcessSession);
					}

					// 처리 후 패킷을 버퍼에서 지운다
					m_pNSelledLogProcessSession->GetRecvBuffer()->RemoveFirstPacket();
				}
			}
			else if(m_pNSelledLogProcessSession != NULL &&
				m_pNSelledLogProcessSession->GetState() == SESSION_STATE_DISCONNECTED)
			{
				m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_FINISHLOG;
			}
		}
		break;

	case SELLEDLOGCLIENTPROCESS_STATE_WRONGID:
		{
			if(	m_pNSelledLogProcessSession != NULL)
			{				
				m_pNSelledLogProcessSession->CloseSocket();
				m_pNSelledLogProcessSession = NULL;
			}
			m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case SELLEDLOGCLIENTPROCESS_STATE_WRONGPASS:
		{
			if(	m_pNSelledLogProcessSession != NULL)
			{				
				m_pNSelledLogProcessSession->CloseSocket();
				m_pNSelledLogProcessSession = NULL;

				AfxMessageBox("Wrong Password !!!", MB_OK);
			}
			m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case SELLEDLOGCLIENTPROCESS_STATE_DONOTINPUTID:
		{
			m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case SELLEDLOGCLIENTPROCESS_STATE_FINISHLOG:
		{
			if(	m_pNSelledLogProcessSession != NULL)
			{
				CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)m_pkSelledLogFrame;
				m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_IDLE;

				m_pNSelledLogProcessSession->CloseSocket();
				m_pNSelledLogProcessSession = NULL;

				if(pkSelledLogFrame != NULL)
				{
					if(pkSelledLogFrame != NULL)
					{
						CString Msg;
						Msg.Format("[ %d ] Items Find", pkSelledLogFrame->m_siSearchCount);
						AfxMessageBox(Msg, MB_OK);

						pkSelledLogFrame->m_siSearchCount = 0;
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
void NSelledLogClientProcess::MessageProc(sPacketHeader* pPacket, ITZSession* pSession)
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
BOOL NSelledLogClientProcess::IsEstablished()
{
	if(m_pNSelledLogProcessSession == NULL)										return FALSE;
	if(m_pNSelledLogProcessSession->GetState() != SESSION_STATE_ESTABLISHED)	return FALSE;

	return TRUE;
}

//=============================================================================
//  @Name   RequsetSelledLog()
//! @author LeeKH						
//! @date   2009-04-20
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL NSelledLogClientProcess::RequsetSelledLog(sDBRequest_SelledItemLog* pLog)
{
	if(pLog == NULL)					return FALSE;

	if(m_siProcessState != SELLEDLOGCLIENTPROCESS_STATE_IDLE)		return FALSE;

	m_stRequestSelledItemMsg.Set(pLog);
	m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_REQUESTCONNECTING;

	return TRUE;
}

BOOL NSelledLogClientProcess::SendMsg(sPacketHeader *pPacket)
{
	if(m_pNSelledLogProcessSession == NULL)					return FALSE;
	if(m_pNSelledLogProcessSession->GetSendBuffer() == NULL)	return FALSE;

	if(!IsEstablished())								return FALSE;

	m_pNSelledLogProcessSession->GetSendBuffer()->Write( (BYTE *)pPacket);

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
BOOL NSelledLogClientProcess::Stop()
{
	if(m_siProcessState != SELLEDLOGCLIENTPROCESS_STATE_WAITLOG)		return FALSE;

	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)m_pkSelledLogFrame;
	if(pkSelledLogFrame == NULL)										return FALSE;

	pkSelledLogFrame->SearchEnable(TRUE);
	m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_FINISHLOG;

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
void NSelledLogClientProcess::NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession)
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
void NSelledLogClientProcess::DoMsg_DBMSG_RESPONSE_SELLED_LOG(sPacketHeader* pPacket, ITZSession* pSession)
{
	sDBResponse_SelledItemLog	*pRecvMsg = (sDBResponse_SelledItemLog *)pPacket;

	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)m_pkSelledLogFrame;
	if(pkSelledLogFrame == NULL)						return;

	if(m_pNSelledLogProcessSession->GetPerson()->GetCharID() != pRecvMsg->usCharID)		return;

	if(pRecvMsg->siResult == 1)
	{
		pkSelledLogFrame->AddLogData(pRecvMsg->stLogData);
	}
	else if (pRecvMsg->siResult == -1)
	{
		pkSelledLogFrame->SearchEnable(TRUE);
		m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_WRONGID;
	}
	else if (pRecvMsg->siResult == -2)
	{
		pkSelledLogFrame->SearchEnable(TRUE);
		m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_WRONGPASS;
	}
	else if (pRecvMsg->siResult == -10)
	{
		pkSelledLogFrame->SearchEnable(TRUE);
		m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_FINISHLOG;
		//AfxMessageBox( "DB Execute Error !!", MB_OK);
	}
	else if (pRecvMsg->siResult == -100)
	{
		pkSelledLogFrame->AddLogData(pRecvMsg->stLogData);
		pkSelledLogFrame->SearchEnable(TRUE);
		m_siProcessState = SELLEDLOGCLIENTPROCESS_STATE_FINISHLOG;
	}
}
