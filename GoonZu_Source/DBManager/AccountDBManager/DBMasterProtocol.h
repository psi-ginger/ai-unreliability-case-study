#ifndef _DBMASTERPROTOCOL_H
#define _DBMASTERPROTOCOL_H

#include "AccountDB.h"
#include "ITZNetworkThread.h"

class DBMasterProtocol : public ITZNetworkThread
{
public:
	DBMasterProtocol();
	~DBMasterProtocol();

public:
	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

private:
	ITZSessionFactory< cMaster >	m_SF;

//#ifdef USE_DBGW
//	DBGW						m_odbc;
//#else
	RichODBC					m_odbc;
//#endif
	char			m_szSPName[ 512 ];
};


#endif
