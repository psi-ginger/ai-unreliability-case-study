#ifndef _ITZNETOVERLAPPED_H
#define _ITZNETOVERLAPPED_H

#include "ITZNetwork.h"
#include "ITZList.h"
#include "ITZSession.h"

class ITZSessionManager;

class ITZNetOverlapped
{
friend unsigned __stdcall overlapped_worker( LPVOID param );

public:
	ITZNetOverlapped();
	virtual ~ITZNetOverlapped();

	void					CreateOverlapped( ITZSessionManager *pssmgr );

private:
	void					_beginSession( ITZSession *pSession );

	HANDLE					m_hOverlapped;
	ITZSessionManager*		m_pSessionManager;

	BOOL					m_bRun;

	HANDLE					m_hExitEvent;
};


#endif