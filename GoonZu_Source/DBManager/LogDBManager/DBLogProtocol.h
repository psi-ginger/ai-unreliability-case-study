#ifndef _DBLOGPROTOCOL_H_
#define _DBLOGPROTOCOL_H_

#include "LogDB.h"
#include "ITZNetworkThread.h"

class DBLogProtocol : public ITZNetworkThread
{
private:
	ITZSessionFactory< cLogClient >	m_SF;

	//RichODBC					m_odbc;
	//char						m_szSPName[ 512 ];

	NThreadParameter			m_ThreadUser[MAX_USERS];

private:
	class stDBMsgPtr
	{
	public:
		void (DBLogProtocol::*Func)(sPacketHeader* pPacket, ITZSession* pSession);
	};
	stDBMsgPtr *m_pDBMsgPtr;

public:
	DBLogProtocol();
	~DBLogProtocol();

	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

protected:
	void	MessageProc(sPacketHeader* pPacket, ITZSession* pSession);

protected:
	void	NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession);

	void	DoMsg_DBMSG_REQUEST_TEST(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_DBMSG_REQUEST_LOG(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_DBMSG_REQUEST_TOOLSLOG(sPacketHeader* pPacket, ITZSession* pSession);
	void	DoMsg_DBMSG_REQUEST_NEWLOG(sPacketHeader* pPacket, ITZSession* pSession);
	
};

#endif
