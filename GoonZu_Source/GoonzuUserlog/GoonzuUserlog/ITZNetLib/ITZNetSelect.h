#ifndef _ITZNETSELECT_H
#define _ITZNETSELECT_H

#include "ITZNetwork.h"
#include "ITZList.h"
#include "ITZSession.h"

class ITZSessionManager;

class ITZNetSelect
{
friend unsigned __stdcall select_worker( LPVOID param );

public:
	ITZNetSelect();
	virtual ~ITZNetSelect();

	void					CreateSelect( ITZSessionManager *pssmgr );

private:
	void					_beginSession( ITZSession *pSession );

	HANDLE					m_hOverlapped;
	ITZSessionManager*		m_pSessionManager;

	BOOL					m_bRun;

	HANDLE					m_hExitEvent;

	fd_set					m_fdset;


};

#endif
