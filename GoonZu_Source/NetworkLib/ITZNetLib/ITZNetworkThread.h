#ifndef _ITZNETWORKTHREAD_H
#define _ITZNETWORKTHREAD_H

#include "itzbase.h"

#include "ITZSessionFactory.h"
#include "ITZListen.h"
#include "ITZNetIOCP.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

class ITZNetworkThread
{
friend unsigned __stdcall nt_worker( LPVOID param );

public:
	ITZNetworkThread();
	~ITZNetworkThread();

public:
	void				InitWinSock();
	
	void				CreateThread();
	void				Destroy();

	ITZListen*			GetListen() { return m_pListen; };
	ITZSessionManager*	GetSessionManager() { return m_pSessionManager; };
	ITZNetIOCP*			GetNetIOCP() { return m_pNetIOCP; };

	void				SetFPS( DWORD dwFPS ) { m_dwFPS = dwFPS; };

public:
	virtual	void		Update() {};
	
	virtual void		SessionProc( ITZSession *pSession ) {};
	

private:
	virtual void		LogOut( ITZSession *pSession ) {};

	ITZSessionManager*	m_pSessionManager;
	ITZListen*			m_pListen;
	ITZNetIOCP*			m_pNetIOCP;

	BOOL				m_bRun;

	HANDLE				m_hExitEvent;

	DWORD				m_dwFPS;

};


#endif