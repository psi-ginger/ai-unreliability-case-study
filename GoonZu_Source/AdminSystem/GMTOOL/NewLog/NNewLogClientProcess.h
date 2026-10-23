//=============================================================================
//
//	NLogClientProcess.h
//
//	Copyright 2007 HQGame
//
//=============================================================================
#ifndef _NNEWLOGCLIENTPROCESS_H_
#define _NNEWLOGCLIENTPROCESS_H_

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../DBManager/LogDBManager/LogDBMsg.h"

#include "NNewLogClientProcessDefines.h"

class NNewLogProcess : public ITZPerson
{
public:
	NNewLogProcess()	{}
	~NNewLogProcess()	{}
};

class NNewLogClientProcess
{
private:
	ITZSessionFactory< NNewLogProcess >	m_sf;
	ITZSessionManager					m_sm;
	ITZNetIOCP							m_iocp;

	CMDIChildWnd*						m_pkNewLogFrame;

	ITZSession*							m_pNNewLogProcessSession;

	char								m_szNNewLogServerIP[ 20 ];
	UI16								m_uiNNewLogServerBasePort;	
	SI32								m_siReason;

	SI32								m_siProcessState;

	DWORD								m_dwStartConnectTime;

	sDBRequest_NewLog					m_stRequestMsg;

	char								m_szLoginID[50];
	char								m_szLoginPW[50];

private:
	class stDBNewLogMsgPtr
	{
	public:
		void (NNewLogClientProcess::*Func)(sPacketHeader* pPacket, ITZSession* pSession);
	};
	stDBNewLogMsgPtr *m_pDBNewLogMsgPtr;

public:
	NNewLogClientProcess(CMDIChildWnd* m_pkParent);
	~NNewLogClientProcess();

	void		Init();
	void		Run();

	BOOL		IsEstablished();

	SI32		GetReason()						{	return m_siReason;						}

	SI32		GetProcessState()				{	return m_siProcessState;				}

	BOOL		RequsetLog(sDBRequest_NewLog* pLog);
	BOOL		Stop();

	BOOL		SendMsg(sPacketHeader *pPacket);

	void		SetIP(const TCHAR* pIP)			{	strncpy(m_szNNewLogServerIP, pIP, 20);		m_szNNewLogServerIP[19] = '\0';	}
	void		SetPort(const UI16 uiPort)		{	m_uiNNewLogServerBasePort = uiPort;										}

	void		SetLoginID(const TCHAR* pLoginID)		{	strncpy(m_szLoginID, pLoginID, 50);		m_szLoginID[49] = '\0';	}
	void		SetLoginPW(const TCHAR* pLoginPW)		{	strncpy(m_szLoginPW, pLoginPW, 50);		m_szLoginPW[49] = '\0';	}

	char*		GetLoginID()							{	return m_szLoginID;		}
	char*		GetLoginPW()							{	return m_szLoginPW;		}

protected:
	void	MessageProc(sPacketHeader* pPacket, ITZSession* pSession);

protected:
	void	NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_DBMSG_RESPONSE_NEWLOG(sPacketHeader* pPacket, ITZSession* pSession);
};

#endif
