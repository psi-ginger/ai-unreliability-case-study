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

#include "NGMClientProcessDefines.h"

class NGMProcess : public ITZPerson
{
public:
	NGMProcess()
	{
	}
	~NGMProcess()
	{
	}
};

class NGMClientProcess
{
private:
	ITZSessionFactory< NGMProcess >		m_sf;
	ITZSessionManager					m_sm;
	ITZNetIOCP							m_iocp;

	ITZSession*							m_pNGMProcessSession;

	char								m_szMasterServerIP[ 20 ];
	UI16								m_uiMasterServerBasePort;	
	SI32								m_siReason;

	char								m_szLoginID[50];
	char								m_szLoginPW[50];

	BOOL								m_bSendLogin;
	BOOL								m_bLoginOK;
	BOOL								m_bSendReqInit;
	BOOL								m_bInitOK;

private:
	class stDBMsgPtr
	{
	public:
		void (NGMClientProcess::*Func)(sPacketHeader* pPacket, ITZSession* pSession);
	};
	stDBMsgPtr *m_pDBMsgPtr;

public:
	NGMClientProcess();
	~NGMClientProcess();
	
	void		Init();
	void		Run();

	BOOL		IsEstablished();

	SI32		GetReason()						{	return m_siReason;						}

	BOOL		SendMsg(sPacketHeader *pPacket);

	void		SetIP(const TCHAR* pIP)			{	strncpy(m_szMasterServerIP, pIP, 20);		m_szMasterServerIP[19] = '\0';	}
	void		SetPort(const UI16 uiPort)		{	m_uiMasterServerBasePort = uiPort;										}

	void		SetLoginID(const TCHAR* pLoginID)		{	strncpy(m_szLoginID, pLoginID, 50);		m_szLoginID[49] = '\0';	}
	void		SetLoginPW(const TCHAR* pLoginPW)		{	strncpy(m_szLoginPW, pLoginPW, 50);		m_szLoginPW[49] = '\0';	}

	char*		GetLoginID()							{	return m_szLoginID;		}
	char*		GetLoginPW()							{	return m_szLoginPW;		}

protected:
	void	MessageProc(sPacketHeader* pPacket, ITZSession* pSession);

protected:
	void	NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_MASTERMSG_RESPONSE_GMCLOGIN(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_MASTERMSG_REPORT_SERVERREFRESHINFO(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_MASTERMSG_RESPONSE_SERVERINFO1(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_MASTERMSG_RESPONSE_SERVERINFO2(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_GOONZUGM_RESPONSE_ACCOUNTCHARINFO(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_GOONZUGM_RESPONSE_USERCHARINFO(sPacketHeader* pPacket, ITZSession* pSession);
	
};

#endif
