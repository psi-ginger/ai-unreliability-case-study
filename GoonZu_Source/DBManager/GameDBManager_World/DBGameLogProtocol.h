#ifndef _DBGAMELOGPROTOCOL_H
#define _DBGAMELOGPROTOCOL_H

#include "GameDB.h"
#include "ITZNetworkThread.h"

#ifdef BEGIN_SP
#undef BEGIN_SP
#endif

//#ifdef USE_DBGW			// DBGW를 사용하면 DBGW를 통해 LogDatabase를 접근한다.
//	#define BEGIN_SP(a)				m_odbc.BeginSP("goonzu_"a);
//#else
	#define BEGIN_SP(a)				sprintf( m_szSPName, "%s.dbo.%s", g_logdbname, a ); m_odbc.BeginSP( m_szSPName );
//#endif // USE_DBGW


class DBGameLogProtocol : public ITZNetworkThread
{
public:
	DBGameLogProtocol();
	~DBGameLogProtocol();

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
	class stDBMsgPtr
	{
	public:
		void (DBGameLogProtocol::*Func)(void);
	};
	
	stDBMsgPtr *m_pDBMsgPtr;
	void	NullFuncPtr();

public:
	void	PersonalLog();
};

#endif
