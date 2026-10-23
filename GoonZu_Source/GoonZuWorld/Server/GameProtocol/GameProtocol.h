//---------------------------------
// 2003/7/8 김태곤
//---------------------------------
#ifndef _GAMEPROTOCOL_H
#define _GAMEPROTOCOL_H

#include "..\..\NetworkLib\ITZNetLib\ITZNetworkThread.h"
#include "..\..\NetworkLib\ITZNetLib\ITZPerson.h"
#include "..\..\NetworkLib\ITZNetLib\ITZListen.h"

#include "Char\CharServer\Char-Server.h"

//==========================================================
// NET MESSAGE STATISTICS - by LEEKH
// define되면 NET_MESSAGE 통계를 파일로 적는다.
//==========================================================
//#define WRITE_NETMESSAGE_STATISTICS
//==========================================================

class cltCommonLogic;

class cltNetMessageStatistics
{
public:
	SI64		m_siSend;
	SI64		m_siRecv;
	SI32		m_siSize;

	cltNetMessageStatistics()
	{
		m_siSend	= 0;
		m_siRecv	= 0;
		m_siSize	= 0;
	}
};

class cltGameProtocol : public ITZNetworkThread{
public:
	cltCommonLogic* pclClient;

	cltGameProtocol(cltCommonLogic* pclclient);
	~cltGameProtocol();

	void Init(UI16 usClientListenPort);
	void Start();
	void Update();

	virtual void SessionProc(ITZSession *pSession);
	virtual void LogOut(ITZSession* pSession);

public:
	ITZSessionFactory < cltCharServer > m_SF;
	
	UI08								m_encrBuf[ 100000 ];

#ifdef WRITE_NETMESSAGE_STATISTICS
	cltNetMessageStatistics				m_clNetMessageStatistics[GAMEMSG_END];

	void	ClearNetMessageStatistics();
#endif

};



#endif