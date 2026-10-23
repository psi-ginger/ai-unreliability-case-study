#ifndef _DBLOGPROTOCOL_H
#define _DBLOGPROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDB.h"
#include "ITZNetworkThread.h"

extern char						g_logdbname[];

#ifdef BEGIN_SP
#undef BEGIN_SP
#endif

//#ifdef USE_DBGW			// DBGW를 사용하면 DBGW를 통해 LogDatabase를 접근한다.
//	#define BEGIN_SP(a)				m_odbc.BeginSP("goonzu_"a);
//#else
	#define BEGIN_SP(a)				sprintf( m_szSPName, "%s.dbo.%s", g_logdbname, a );			m_odbc.BeginSP( m_szSPName );
//#endif // USE_DBGW

class DBLogProtocol : public ITZNetworkThread
{
public:
	DBLogProtocol();
	~DBLogProtocol();

public:
	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW, char* szProcedure );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

	void			MessageProc();
	
	void			SendPacket( BYTE *pPacket );
private:
	ITZSessionFactory< cLog >	m_SF;

private:
	ITZSession		*m_pSession;
	sPacketHeader	*m_pPacket;

//#ifdef USE_DBGW
//	DBGW			m_odbc;
//#else
	RichODBC		m_odbc;
//#endif
	char			m_szSPName[ 512 ];
	char			m_szProcudure[ 40 ];
};

#endif
