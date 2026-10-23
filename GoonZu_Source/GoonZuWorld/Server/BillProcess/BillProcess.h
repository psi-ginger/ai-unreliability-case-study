#ifndef _BillProcess_H
#define _BillProcess_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "../../Server/FrontServer/FrontMsg.h"


#define BILL_HEARTBEAT_INTERVAL	5000							// 빌 서버로 보내는 허트비트 주기 (3초)


class cBill : public ITZPerson
{
public:
	cBill() {};
	~cBill() {};
};


class BillProcess
{
public:
	BillProcess();
	~BillProcess();

	void				Init( TCHAR *billIP, UI16 billPort);
	void				Run();

	ITZSession*			GetBillSession() { return m_pBillSession; };

private:
	ITZSessionFactory< cBill >	m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pBillSession;				// 빌 서버 세션 포인터


	TCHAR							m_BillIP[ 20 ];				// 프론트 서버 IP
	UI16							m_BillPort;					// 프론트 서버 PORT

	DWORD							m_BillLastTick;

public:
	GAMESERVER_REFRESHINFO			m_refreshInfo;

public:
	bool SendMsg(sPacketHeader* pPacket);
};



#endif