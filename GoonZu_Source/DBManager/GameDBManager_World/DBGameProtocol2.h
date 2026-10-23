#ifndef _DBGAMEPROTOCOL2_H
#define _DBGAMEPROTOCOL2_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDB.h"
#include "ITZNetworkThread.h"


class DBGameProtocol2 : public ITZNetworkThread
{
public:
	DBGameProtocol2();
	~DBGameProtocol2();

public:
	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );


	void			MessageProc();
	

	void			SendPacket( BYTE *pPacket );

private:
	ITZSessionFactory< cGame >	m_SF;

private:
	ITZSession		*m_pSession;
	sPacketHeader	*m_pPacket;

//#ifdef USE_DBGW
//	DBGW			m_odbc;
//#else
	RichODBC		m_odbc;
//#endif

	char			m_szSPName[ 512 ];

public:
	// 군주신문
	void	News();
	
	class stDBMsgPtr
	{
	public:
		void (DBGameProtocol2::*Func)(void);
	};
	
	stDBMsgPtr *m_pDBMsgPtr;
	void	NullFuncPtr();
};

#endif
