//=============================================================================
//
//	NLogClientProcess.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "stdafx.h"

#define DBMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &NLogClientProcess::BBB

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
NLogClientProcess::NLogClientProcess()
{
	m_pNLogProcessSession	= NULL;

	ZeroMemory(&m_stRequestMsg, sizeof(m_stRequestMsg));	

	m_siProcessState		= LOGCLIENTPROCESS_STATE_IDLE;

	m_szNLogServerIP[0]		= '\0';
	m_uiNLogServerBasePort	= 0;
	m_siReason				= 0;

	m_szLoginID[0]			= '\0';
	m_szLoginPW[0]			= '\0';

	m_pDBMsgPtr = new stDBMsgPtr[DBMSG_END];

	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < DBMSG_END; i++)
			m_pDBMsgPtr[i].Func = &NLogClientProcess::NullFuncPtr;

		DBMessageMap(DBMSG_RESPONSE_TEST, DoMsg_DBMSG_RESPONSE_TEST);
		DBMessageMap(DBMSG_RESPONSE_LOG, DoMsg_DBMSG_RESPONSE_LOG);
	}
}

NLogClientProcess::~NLogClientProcess()
{
	if(m_pDBMsgPtr != NULL)
		delete [] m_pDBMsgPtr;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NLogClientProcess::Init()
{
	m_sf.CreateFactory( &m_sm, MAX_LOGCLIENT_CHANNEL, MAX_LOGCLIENT_PACKET_SIZE * MAX_LOGCLIENT_PACKET_COUNT, MAX_LOGCLIENT_PACKET_SIZE * MAX_LOGCLIENT_PACKET_COUNT );
	m_iocp.CreateIOCP( &m_sm, 1 );

	CString	strPath;
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';

	strPath = szExecuteFile;

	CString	INIFile;
	INIFile = strPath + "\\Data\\Server.ini";

	char szIP[20];
	szIP[0] = '\0';
	DWORD dwLen = GetPrivateProfileString("LOGDBMANAGER", "IP", "", szIP, 20, INIFile);
	if(dwLen <= 0)
	{
		AfxMessageBox("Unknown Server IP in [Server.ini] File", MB_OK);
		return;
	}
	strcpy(m_szNLogServerIP, szIP);

	UINT uiPort = GetPrivateProfileInt("LOGDBMANAGER", "Port", 0, INIFile);
	if(uiPort == 0)
	{
		AfxMessageBox("Unknown Server PORT in [Server.ini] File", MB_OK);
		return;
	}
	m_uiNLogServerBasePort = uiPort;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NLogClientProcess::Run()
{
	m_sm.Update();

	switch(m_siProcessState)
	{
	case LOGCLIENTPROCESS_STATE_IDLE:
		{
			// 기다린다.
			if(	m_pNLogProcessSession != NULL && 
				m_pNLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNLogProcessSession->CloseSocket();
				m_pNLogProcessSession = NULL;
			}
		}
		break;

	case LOGCLIENTPROCESS_STATE_REQUESTCONNECTING:
		{
			// ID와 PW가 없으면 바로 끝낸다.
			if(strlen(m_szLoginID) <= 0 || strlen(m_szLoginPW) <= 0)
			{
				m_siProcessState = LOGCLIENTPROCESS_STATE_DONOTINPUTID;
				break;
			}
			m_pNLogProcessSession = NULL;

			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNLogServerIP, m_uiNLogServerBasePort, &siReason) == pSession)
				{
					m_siReason = siReason;
				}
				else
				{
					int nErr = WSAGetLastError();
					m_siReason = siReason;
				}
				m_dwStartConnectTime = GetTickCount();
				m_siProcessState = LOGCLIENTPROCESS_STATE_CONNECTING;
			}
		}
		break;
	case LOGCLIENTPROCESS_STATE_CONNECTING:
		{
			ITZSession* pSession = m_sm.GetSession(0);
			if(pSession != NULL && pSession->GetState() == SESSION_STATE_NONE)
			{
				SI32 siReason;
				if( m_sm.Connect(0, m_szNLogServerIP, m_uiNLogServerBasePort, &siReason) == pSession)
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
					pSession->Close();

					m_siProcessState = m_siProcessState = LOGCLIENTPROCESS_STATE_IDLE;;

					ZeroMemory(&m_stRequestMsg, sizeof(m_stRequestMsg));
					m_siProcessState = LOGCLIENTPROCESS_STATE_IDLE;

					AfxMessageBox("All Channel is Working... Please Try a few minutes later", MB_OK);

					CGLogClientDlg* LogClient = (CGLogClientDlg*)AfxGetMainWnd();
					if(LogClient != NULL)
					{
						LogClient->SearchEnable(TRUE);
					}
				}
			}
			else if(pSession != NULL && pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_pNLogProcessSession = pSession;
				m_siProcessState = LOGCLIENTPROCESS_STATE_ESTABLISH;
			}
		}
		break;
	case LOGCLIENTPROCESS_STATE_ESTABLISH:
		{
			if(	m_pNLogProcessSession != NULL &&
				m_pNLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				m_stRequestMsg.usCharID = m_pNLogProcessSession->GetPerson()->GetCharID();

				m_pNLogProcessSession->GetSendBuffer()->Write( (BYTE*) &m_stRequestMsg );
				m_siProcessState = LOGCLIENTPROCESS_STATE_WAITLOG;
			}
		}
		break;
	case LOGCLIENTPROCESS_STATE_WAITLOG:
		{
			if(	m_pNLogProcessSession != NULL &&
				m_pNLogProcessSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				SI32 siCount = m_pNLogProcessSession->GetRecvBuffer()->GetPacketCount();
				for(SI32 i=0; i<siCount; i++)
				{
					sPacketHeader* pPacket = (sPacketHeader*)m_pNLogProcessSession->GetRecvBuffer()->GetFirstPacket();

					if( pPacket != NULL )
					{
						MessageProc( pPacket, m_pNLogProcessSession);
					}

					// 처리 후 패킷을 버퍼에서 지운다
					m_pNLogProcessSession->GetRecvBuffer()->RemoveFirstPacket();
				}
			}
			else if(m_pNLogProcessSession != NULL &&
					m_pNLogProcessSession->GetState() == SESSION_STATE_DISCONNECTED)
			{
				m_siProcessState = LOGCLIENTPROCESS_STATE_FINISHLOG;
			}
		}
		break;

	case LOGCLIENTPROCESS_STATE_WRONGID:
		{
			if(	m_pNLogProcessSession != NULL)
			{				
				m_pNLogProcessSession->CloseSocket();
				m_pNLogProcessSession = NULL;

				// ID와 PW를 다시 받기위해 지운다.
				m_szLoginID[0] = '\0';
				m_szLoginPW[0] = '\0';
				AfxMessageBox("Wrong ID !!!", MB_OK);
			}
			m_siProcessState = LOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case LOGCLIENTPROCESS_STATE_WRONGPASS:
		{
			if(	m_pNLogProcessSession != NULL)
			{				
				m_pNLogProcessSession->CloseSocket();
				m_pNLogProcessSession = NULL;

				AfxMessageBox("Wrong Password !!!", MB_OK);
			}
			// ID와 PW를 다시 받기위해 지운다.
			m_szLoginID[0] = '\0';
			m_szLoginPW[0] = '\0';
			m_siProcessState = LOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case LOGCLIENTPROCESS_STATE_DONOTINPUTID:
		{
			m_siProcessState = LOGCLIENTPROCESS_STATE_IDLE;
		}
		break;

	case LOGCLIENTPROCESS_STATE_FINISHLOG:
		{
			if(	m_pNLogProcessSession != NULL)
			{				
				m_pNLogProcessSession->CloseSocket();
				m_pNLogProcessSession = NULL;

				CGLogClientDlg* LogClient = (CGLogClientDlg*)AfxGetMainWnd();
				if(LogClient != NULL)
				{
					CString Msg;
					Msg.Format("[ %d ] Items Find", LogClient->m_siSearchCount);
					AfxMessageBox(Msg, MB_OK);
				}
				else
				{
					AfxMessageBox("Search Complete !!", MB_OK);
				}
			}
			m_siProcessState = LOGCLIENTPROCESS_STATE_IDLE;
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
void NLogClientProcess::MessageProc(sPacketHeader* pPacket, ITZSession* pSession)
{
	if( pPacket == NULL )	return;
	if( pSession == NULL )	return;

	if(pPacket->usCmd >= 0 && pPacket->usCmd <= DBMSG_END)
	{
		(this->*(this->m_pDBMsgPtr[pPacket->usCmd].Func))(pPacket, pSession);
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
BOOL NLogClientProcess::IsEstablished()
{
	if(m_pNLogProcessSession == NULL)									return FALSE;
	if(m_pNLogProcessSession->GetState() != SESSION_STATE_ESTABLISHED)	return FALSE;

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
BOOL NLogClientProcess::RequsetLog(sDBRequest_Log* pLog)
{
	if(pLog == NULL)					return FALSE;

	if(m_siProcessState != LOGCLIENTPROCESS_STATE_IDLE)		return FALSE;

	m_stRequestMsg.Set(pLog);
	m_siProcessState = LOGCLIENTPROCESS_STATE_REQUESTCONNECTING;

	return TRUE;
}

BOOL NLogClientProcess::SendMsg(sPacketHeader *pPacket)
{
	if(m_pNLogProcessSession == NULL)					return FALSE;
	if(m_pNLogProcessSession->GetSendBuffer() == NULL)	return FALSE;

	if(!IsEstablished())								return FALSE;

	m_pNLogProcessSession->GetSendBuffer()->Write( (BYTE *)pPacket);

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
BOOL NLogClientProcess::Stop()
{
	if(m_siProcessState != LOGCLIENTPROCESS_STATE_WAITLOG)		return FALSE;

	CGLogClientDlg* LogClient = (CGLogClientDlg*)AfxGetMainWnd();
	if(LogClient == NULL)										return FALSE;

	LogClient->SearchEnable(TRUE);
	m_siProcessState = LOGCLIENTPROCESS_STATE_FINISHLOG;

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
void NLogClientProcess::NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession)
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", pPacket->usCmd);
	MessageBox(NULL, "DBManager Error", buffer, MB_OK);
}

//=============================================================================
//  @Name   DoMsg_DBMSG_RESPONSE_TEST()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NLogClientProcess::DoMsg_DBMSG_RESPONSE_TEST(sPacketHeader* pPacket, ITZSession* pSession)
{
	sDBResponse_Test	*pRecvMsg = (sDBResponse_Test *)pPacket;
}

//=============================================================================
//  @Name   DoMsg_DBMSG_RESPONSE_LOG()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NLogClientProcess::DoMsg_DBMSG_RESPONSE_LOG(sPacketHeader* pPacket, ITZSession* pSession)
{
	sDBResponse_Log	*pRecvMsg = (sDBResponse_Log *)pPacket;

	CGLogClientDlg* LogClient = (CGLogClientDlg*)AfxGetMainWnd();
	if(LogClient == NULL)						return;
    
	if(m_pNLogProcessSession->GetPerson()->GetCharID() != pRecvMsg->usCharID)		return;

	if(pRecvMsg->siResult == 1)
	{
		LogClient->AddLogData(pRecvMsg->stLogData);
	}
	else if (pRecvMsg->siResult == -1)
	{
		LogClient->SearchEnable(TRUE);
		m_siProcessState = LOGCLIENTPROCESS_STATE_WRONGID;
	}
	else if (pRecvMsg->siResult == -2)
	{
		LogClient->SearchEnable(TRUE);
		m_siProcessState = LOGCLIENTPROCESS_STATE_WRONGPASS;
	}
	else if (pRecvMsg->siResult == -100)
	{
		LogClient->AddLogData(pRecvMsg->stLogData);
		LogClient->SearchEnable(TRUE);
		m_siProcessState = LOGCLIENTPROCESS_STATE_FINISHLOG;
	}
}
