#ifndef _ITZNETIOCP
#define _ITZNETIOCP

#include "ITZNetwork.h"
#include "ITZList.h"
#include "ITZSession.h"

class ITZSessionManager;

class ITZNetIOCP
{
friend unsigned __stdcall iocp_worker( LPVOID param );
friend unsigned __stdcall iocp_worker_completion( LPVOID param );

public:
	ITZNetIOCP();
	virtual ~ITZNetIOCP();

	void					CreateIOCP( ITZSessionManager *pssmgr, int nTherad  );
	void					Destroy();

private:
	void					_beginSession( ITZSession *pSession );

	HANDLE					m_hIOCP;
	ITZSessionManager*		m_pSessionManager;

	BOOL					m_bRun;


	HANDLE					m_hExitEvent;

public:
	DWORD					m_dwLastTick;
};


#endif