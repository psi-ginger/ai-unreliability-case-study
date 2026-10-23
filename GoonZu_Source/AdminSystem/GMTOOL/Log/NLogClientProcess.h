//=============================================================================
//
//	NLogClientProcess.h
//
//	Copyright 2007 HQGame
//
//=============================================================================
#ifndef _NLOGCLIENTPROCESS_H_
#define _NLOGCLIENTPROCESS_H_

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../DBManager/LogDBManager/LogDBMsg.h"

#include "NLogClientProcessDefines.h"

class NLogProcess : public ITZPerson
{
public:
	NLogProcess()	{}
	~NLogProcess()	{}
};

class NLogClientProcess
{
private:
	ITZSessionFactory< NLogProcess >	m_sf;
	ITZSessionManager					m_sm;
	ITZNetIOCP							m_iocp;

	CMDIChildWnd*						m_pkLogFrame;

	ITZSession*							m_pNLogProcessSession;

	char								m_szNLogServerIP[ 20 ];
	UI16								m_uiNLogServerBasePort;	
	UI16								m_uiNLogServerWorkPort;	
	SI32								m_siReason;

	SI32								m_siProcessState;

	DWORD								m_dwStartConnectTime;

	sDBRequest_Log						m_stRequestMsg;

	char								m_szLoginID[50];
	char								m_szLoginPW[50];

private:
	class stDBMsgPtr
	{
	public:
		void (NLogClientProcess::*Func)(sPacketHeader* pPacket, ITZSession* pSession);
	};
	stDBMsgPtr *m_pDBMsgPtr;

public:
	NLogClientProcess(CMDIChildWnd* m_pkParent);
	~NLogClientProcess();

	void		Init();
	void		Run();

	BOOL		IsEstablished();

	//	ITZSession*	GetConnectedLogServerSession()	{	return m_pNConnectedLogProcessSession;	}
	SI32		GetReason()						{	return m_siReason;						}

	SI32		GetProcessState()				{	return m_siProcessState;				}

	BOOL		RequsetLog(sDBRequest_Log* pLog);
	BOOL		Stop();

	BOOL		SendMsg(sPacketHeader *pPacket);

	void		SetIP(const TCHAR* pIP)			{	strncpy(m_szNLogServerIP, pIP, 20);		m_szNLogServerIP[19] = '\0';	}
	void		SetPort(const UI16 uiPort)		{	m_uiNLogServerBasePort = uiPort;		m_uiNLogServerWorkPort = m_uiNLogServerBasePort;		}

	void		SetLoginID(const TCHAR* pLoginID)		{	strncpy(m_szLoginID, pLoginID, 50);		m_szLoginID[49] = '\0';	}
	void		SetLoginPW(const TCHAR* pLoginPW)		{	strncpy(m_szLoginPW, pLoginPW, 50);		m_szLoginPW[49] = '\0';	}

	char*		GetLoginID()							{	return m_szLoginID;		}
	char*		GetLoginPW()							{	return m_szLoginPW;		}

protected:
	void	MessageProc(sPacketHeader* pPacket, ITZSession* pSession);

protected:
	void	NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_DBMSG_RESPONSE_TEST(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_DBMSG_RESPONSE_LOG(sPacketHeader* pPacket, ITZSession* pSession);
};

#endif
