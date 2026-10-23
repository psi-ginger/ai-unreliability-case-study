//=============================================================================
//
//	NSelledLogClientProcess.h
//
//	Copyright 2007 HQGame
//
//=============================================================================
#ifndef _NSELLEDLOGCLIENTPROCESS_H_
#define _NSELLEDLOGCLIENTPROCESS_H_

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../TradeLog/LogDBMsg.h"

#include "NSelledLogClientProcessDefines.h"

class NSelledLogProcess : public ITZPerson
{
public:
	NSelledLogProcess()	{}
	~NSelledLogProcess()	{}
};

class NSelledLogClientProcess
{
private:
	ITZSessionFactory< NSelledLogProcess >	m_sf;
	ITZSessionManager						m_sm;
	ITZNetIOCP								m_iocp;

	CMDIChildWnd*						m_pkSelledLogFrame;

	ITZSession*							m_pNSelledLogProcessSession;

	char								m_szNSelledLogServerIP[ 20 ];
	UI16								m_uiNSelledLogServerBasePort;	
	SI32								m_siReason;

	SI32								m_siProcessState;

	DWORD								m_dwStartConnectTime;

	sDBRequest_SelledItemLog			m_stRequestSelledItemMsg;

private:
	class stDBNewLogMsgPtr
	{
	public:
		void (NSelledLogClientProcess::*Func)(sPacketHeader* pPacket, ITZSession* pSession);
	};
	stDBNewLogMsgPtr *m_pDBNewLogMsgPtr;

public:
	NSelledLogClientProcess(CMDIChildWnd* m_pkParent);
	~NSelledLogClientProcess();

	void		Init();
	void		Run();

	BOOL		IsEstablished();

	SI32		GetReason()						{	return m_siReason;						}

	SI32		GetProcessState()				{	return m_siProcessState;				}

	BOOL		RequsetSelledLog(sDBRequest_SelledItemLog* pLog);
	BOOL		Stop();

	BOOL		SendMsg(sPacketHeader *pPacket);

	void		SetIP(const TCHAR* pIP)			{	strncpy(m_szNSelledLogServerIP, pIP, 20);		m_szNSelledLogServerIP[19] = '\0';	}
	void		SetPort(const UI16 uiPort)		{	m_uiNSelledLogServerBasePort = uiPort;										}

protected:
	void	MessageProc(sPacketHeader* pPacket, ITZSession* pSession);

protected:
	void	NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_DBMSG_RESPONSE_SELLED_LOG(sPacketHeader* pPacket, ITZSession* pSession);
};

#endif
