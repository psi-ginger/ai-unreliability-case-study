#ifndef _ITZLISTEN_H
#define _ITZLISTEN_H

#define ITZLISTEN_ACT_START		1
#define ITZLISTEN_ACT_STOP		2
#define ITZLISTEN_ACT_CLOSE		3
#define ITZLISTEN_ACT_EXIT		4

#define ITZLISTEN_STATE_RUN		100
#define ITZLISTEN_STATE_STOP	101
#define ITZLISTEN_STATE_CLOSE	102

#include "itzbase.h"

#include "ITZSessionManager.h"

class ITZListen  
{

friend unsigned __stdcall listen_worker( LPVOID param );

public:
	ITZListen();
	virtual ~ITZListen();
    
public:
	void				SetListenPort( UI16 usPort );
	UI16				GetListenPort() { return m_usPort; };

	void				CreateListen( ITZSessionManager *pssmgr, UI16 usPort );
	void				Destroy();
	
	void				Start();
	void				Stop();
	void				Close();

private:
	HANDLE				m_hExitEvent;

	HANDLE				m_hActionEvent;
	UI32				m_uiActionCode;

	UI32				m_uiState;

	BOOL				m_bInitOk;

	UI16				m_usPort;
	SOCKADDR_IN			m_sockaddr;

	ITZSessionManager*	m_pSessionManager;

};

#endif