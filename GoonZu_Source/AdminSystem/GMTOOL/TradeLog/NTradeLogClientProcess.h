//=============================================================================
//
//	NTradeLogClientProcess.h
//
//	Copyright 2007 HQGame
//
//=============================================================================
#ifndef _NTRADELOGCLIENTPROCESS_H_
#define _NTRADELOGCLIENTPROCESS_H_

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "LogDBMsg.h"

#include "NTradeLogClientProcessDefines.h"

class NTradeLogProcess : public ITZPerson
{
public:
	NTradeLogProcess()	{}
	~NTradeLogProcess()	{}
};

class NTradeLogClientProcess
{
private:
	ITZSessionFactory< NTradeLogProcess >	m_sf;
	ITZSessionManager						m_sm;
	ITZNetIOCP								m_iocp;

	CMDIChildWnd*						m_pkTradeLogFrame;

	ITZSession*							m_pNTradeLogProcessSession;

	char								m_szNTradeLogServerIP[ 20 ];
	UI16								m_uiNTradeLogServerBasePort;	
	SI32								m_siReason;

	SI32								m_siProcessState;

	DWORD								m_dwStartConnectTime;

	sDBRequest_TradeItemLog				m_stRequestTradeItemMsg;

private:
	class stDBNewLogMsgPtr
	{
	public:
		void (NTradeLogClientProcess::*Func)(sPacketHeader* pPacket, ITZSession* pSession);
	};
	stDBNewLogMsgPtr *m_pDBNewLogMsgPtr;

public:
	NTradeLogClientProcess(CMDIChildWnd* m_pkParent);
	~NTradeLogClientProcess();

	void		Init();
	void		Run();

	BOOL		IsEstablished();

	SI32		GetReason()						{	return m_siReason;						}

	SI32		GetProcessState()				{	return m_siProcessState;				}

	BOOL		RequsetTradeLog(sDBRequest_TradeItemLog* pLog);
	BOOL		Stop();

	BOOL		SendMsg(sPacketHeader *pPacket);

	void		SetIP(const TCHAR* pIP)			{	strncpy(m_szNTradeLogServerIP, pIP, 20);		m_szNTradeLogServerIP[19] = '\0';	}
	void		SetPort(const UI16 uiPort)		{	m_uiNTradeLogServerBasePort = uiPort;										}

protected:
	void	MessageProc(sPacketHeader* pPacket, ITZSession* pSession);

protected:
	void	NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_DBMSG_RESPONSE_TRADE_LOG(sPacketHeader* pPacket, ITZSession* pSession);
};

#endif
