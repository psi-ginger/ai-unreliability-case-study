//=============================================================================
//
//	NNewLogClientProcess.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "../stdafx.h"
#include "../resource.h"
#include "NNewLogClientProcess.h"
#include "NewLogFrame.h"

#define DBMessageMap(AAA,BBB) m_pDBNewLogMsgPtr[AAA].Func = &NNewLogClientProcess::BBB

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
NNewLogClientProcess::NNewLogClientProcess(CMDIChildWnd* m_pkParent)
{
	m_pkNewLogFrame			= m_pkParent;
	m_pNNewLogProcessSession	= NULL;

	ZeroMemory(&m_stRequestMsg, sizeof(m_stRequestMsg));	

	m_siProcessState		= NEWLOGCLIENTPROCESS_STATE_IDLE;

	m_szNNewLogServerIP[0]		= '\0';
	m_uiNNewLogServerBasePort	= 0;
	m_siReason				= 0;

	m_szLoginID[0]			= '\0';
	m_szLoginPW[0]			= '\0';

	m_pDBNewLogMsgPtr = new stDBNewLogMsgPtr[DBMSG_END];

	if(m_pDBNewLogMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < DBMSG_END; i++)
			m_pDBNewLogMsgPtr[i].Func = &NNewLogClientProcess::NullFuncPtr;

		DBMessageMap(DBMSG_RESPONSE_NEWLOG, DoMsg_DBMSG_RESPONSE_NEWLOG);
	}
}

NNewLogClientProcess::~NNewLogClientProcess()
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
void NNewLogClientProcess::Init()
{
	m_sf.CreateFactory( &m_sm, MAX_NEWLOGCLIENT_CHANNEL, MAX_NEWLOGCLIENT_PACKET_SIZE * MAX_NEWLOGCLIENT_PACKET_COUNT, MAX_NEWLOGCLIENT_PACKET_SIZE * MAX_NEWLOGCLIENT_PACKET_COUNT );
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
void NNewLogClientProcess::Run()
{
	m_sm.Update();

	switch(m_siProcessState)
	{
	case NEWLOGCLIENTPROCESS_STATE_IDLE:
		{
			// 기다린다.
			if(	m_pNNewLogProcessSession != NULL && 
				m_pNNewLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNNewLogProcessSession->CloseSocket();
				m_pNNewLogProcessSession = NULL;
			}
		}
		break;

	case NEWLOGCLIENTPROCESS_STATE_REQUESTCONNECTING:
		{
			// ID와 PW가 없으면 바로 끝낸다.
			if(strlen(m_szLoginID) <= 0 || strlen(m_szLoginPW) <= 0)
			{
				m_siProcessState = NEWLOGCLIENTPROCESS_STATE_DONOTINPUTID;
				break;
			}
			m_pNNewLogProcessSession = NULL;

			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNNewLogServerIP, m_uiNNewLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}
				m_dwStartConnectTime = GetTickCount();
				m_siProcessState = NEWLOGCLIENTPROCESS_STATE_CONNECTING;
			}
		}
		break;
	case NEWLOGCLIENTPROCESS_STATE_CONNECTING:
		{
			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNNewLogServerIP, m_uiNNewLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}


				DWORD dwDiff = GetTickCount() - m_dwStartConnectTime;
				if( dwDiff >= MAX_CONNECTING_TIME )
				{
					pSession->CloseSocket();

					m_siProcessState = m_siProcessState = NEWLOGCLIENTPROCESS_STATE_IDLE;;

					ZeroMemory(&m_stRequestMsg, sizeof(m_stRequestMsg));
					m_siProcessState = NEWLOGCLIENTPROCESS_STATE_IDLE;

					AfxMessageBox("All Channel is Working... Please Try a few minutes later", MB_OK);

					CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)m_pkNewLogFrame;
					if(pkNewLogFrame != NULL)
					{
						pkNewLogFrame->SearchEnable(TRUE);
					}
				}
			}
			else if(pSession != NULL && pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNNewLogProcessSession = pSession;
				m_siProcessState = NEWLOGCLIENTPROCESS_STATE_ESTABLISH;
			}
		}
		break;
	case NEWLOGCLIENTPROCESS_STATE_ESTABLISH:
		{
			if(	m_pNNewLogProcessSession != NULL &&
				m_pNNewLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_stRequestMsg.usCharID = m_pNNewLogProcessSession->GetPerson()->GetCharID();

				m_pNNewLogProcessSession->GetSendBuffer()->Write( (BYTE*) &m_stRequestMsg );
				m_siProcessState = NEWLOGCLIENTPROCESS_STATE_WAITLOG;
			}
		}
		break;
	case NEWLOGCLIENTPROCESS_STATE_WAITLOG:
		{
			if(	m_pNNewLogProcessSession != NULL &&
				m_pNNewLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				SI32 siCount = m_pNNewLogProcessSession->GetRecvBuffer()->GetPacketCount();
				for(SI32 i=0; i<siCount; i++)
				{
					sPacketHeader* pPacket = (sPacketHeader*)m_pNNewLogProcessSession->GetRecvBuffer()->GetFirstPacket();

					if( pPacket != NULL )
					{
						MessageProc( pPacket, m_pNNewLogProcessSession);
					}

					// 처리 후 패킷을 버퍼에서 지운다
					m_pNNewLogProcessSession->GetRecvBuffer()->RemoveFirstPacket();
				}
			}
			else if(m_pNNewLogProcessSession != NULL &&
				m_pNNewLogProcessSession->GetState() == SESSION_STATE_DISCONNECTED)
			{
				m_siProcessState = NEWLOGCLIENTPROCESS_STATE_FINISHLOG;
			}
		}
		break;

	case NEWLOGCLIENTPROCESS_STATE_WRONGID:
		{
			if(	m_pNNewLogProcessSession != NULL)
			{				
				m_pNNewLogProcessSession->CloseSocket();
				m_pNNewLogProcessSession = NULL;

				// ID와 PW를 다시 받기위해 지운다.
				m_szLoginID[0] = '\0';
				m_szLoginPW[0] = '\0';
				AfxMessageBox("Wrong ID !!!", MB_OK);
			}
			m_siProcessState = NEWLOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case NEWLOGCLIENTPROCESS_STATE_WRONGPASS:
		{
			if(	m_pNNewLogProcessSession != NULL)
			{				
				m_pNNewLogProcessSession->CloseSocket();
				m_pNNewLogProcessSession = NULL;

				AfxMessageBox("Wrong Password !!!", MB_OK);
			}
			// ID와 PW를 다시 받기위해 지운다.
			m_szLoginID[0] = '\0';
			m_szLoginPW[0] = '\0';
			m_siProcessState = NEWLOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case NEWLOGCLIENTPROCESS_STATE_DONOTINPUTID:
		{
			m_siProcessState = NEWLOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case NEWLOGCLIENTPROCESS_STATE_FINISHLOG:
		{
			if(	m_pNNewLogProcessSession != NULL)
			{
				CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)m_pkNewLogFrame;
				m_siProcessState = NEWLOGCLIENTPROCESS_STATE_IDLE;

				m_pNNewLogProcessSession->CloseSocket();
				m_pNNewLogProcessSession = NULL;

				if(pkNewLogFrame != NULL)
				{
					if(pkNewLogFrame->RequestLog() == FALSE)
					{
						if(pkNewLogFrame != NULL)
						{
							CString Msg;
							Msg.Format("[ %d ] Items Find", pkNewLogFrame->m_siSearchCount);
							AfxMessageBox(Msg, MB_OK);

							pkNewLogFrame->m_siSearchCount = 0;
						}
						else
						{
							AfxMessageBox("Search Complete !!", MB_OK);
						}
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
void NNewLogClientProcess::MessageProc(sPacketHeader* pPacket, ITZSession* pSession)
{
	if( pPacket == NULL )	return;
	if( pSession == NULL )	return;

	if(pPacket->usCmd >= 0 && pPacket->usCmd <= DBMSG_END)
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
BOOL NNewLogClientProcess::IsEstablished()
{
	if(m_pNNewLogProcessSession == NULL)									return FALSE;
	if(m_pNNewLogProcessSession->GetState() != SESSION_STATE_ESTABLISHED)	return FALSE;

	return TRUE;
}

//=============================================================================
//  @Name   RequsetLog()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL NNewLogClientProcess::RequsetLog(sDBRequest_NewLog* pLog)
{
	if(pLog == NULL)					return FALSE;

	if(m_siProcessState != NEWLOGCLIENTPROCESS_STATE_IDLE)		return FALSE;

	m_stRequestMsg.Set(pLog);
	m_siProcessState = NEWLOGCLIENTPROCESS_STATE_REQUESTCONNECTING;

	return TRUE;
}

BOOL NNewLogClientProcess::SendMsg(sPacketHeader *pPacket)
{
	if(m_pNNewLogProcessSession == NULL)					return FALSE;
	if(m_pNNewLogProcessSession->GetSendBuffer() == NULL)	return FALSE;

	if(!IsEstablished())								return FALSE;

	m_pNNewLogProcessSession->GetSendBuffer()->Write( (BYTE *)pPacket);

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
BOOL NNewLogClientProcess::Stop()
{
	if(m_siProcessState != NEWLOGCLIENTPROCESS_STATE_WAITLOG)		return FALSE;

	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)m_pkNewLogFrame;
	if(pkNewLogFrame == NULL)										return FALSE;

	pkNewLogFrame->SearchEnable(TRUE);
	m_siProcessState = NEWLOGCLIENTPROCESS_STATE_FINISHLOG;

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
void NNewLogClientProcess::NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession)
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
void NNewLogClientProcess::DoMsg_DBMSG_RESPONSE_NEWLOG(sPacketHeader* pPacket, ITZSession* pSession)
{
	sDBResponse_NewLog	*pRecvMsg = (sDBResponse_NewLog *)pPacket;

	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)m_pkNewLogFrame;
	if(pkNewLogFrame == NULL)						return;

	if(m_pNNewLogProcessSession->GetPerson()->GetCharID() != pRecvMsg->usCharID)		return;

	if(pRecvMsg->siResult == 1)
	{
		pkNewLogFrame->AddLogData(pRecvMsg->stLogData);
	}
	else if (pRecvMsg->siResult == -1)
	{
		pkNewLogFrame->SearchEnable(TRUE);
		m_siProcessState = NEWLOGCLIENTPROCESS_STATE_WRONGID;
	}
	else if (pRecvMsg->siResult == -2)
	{
		pkNewLogFrame->SearchEnable(TRUE);
		m_siProcessState = NEWLOGCLIENTPROCESS_STATE_WRONGPASS;
	}
	else if (pRecvMsg->siResult == -100)
	{
		pkNewLogFrame->AddLogData(pRecvMsg->stLogData);
		pkNewLogFrame->SearchEnable(TRUE);
		m_siProcessState = NEWLOGCLIENTPROCESS_STATE_FINISHLOG;
	}
}
