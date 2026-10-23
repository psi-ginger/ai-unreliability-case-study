#ifndef _NTRADESERVERPROCESS_H
#define _NTRADESERVERPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "../NTradeServerProcess/TradeServerMsg.h"

class cNTradeServer : public ITZPerson
{
public:
	cNTradeServer() {};
	~cNTradeServer() {};
};


class NTradeServerProcess
{
private:

public:
	NTradeServerProcess();
	~NTradeServerProcess();

public:
	void					Init();
	void					Run();

	bool					SendMsg(sPacketHeader *pPacket );

	BOOL					IsEstablished();

	ITZSession*				GetTradeServerServersSession() { return m_pNTradeServerSession; };

	SI32					GetReason()
	{
		return m_siReason;
	}

	//---------------------------------------------------------------------------------------------------------------
	//											Packet Process Function Area
	//---------------------------------------------------------------------------------------------------------------

	void	DoMsg_NTRADESERVERRESPONSE_SET_SELLORDER(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_CANCEL_SELLORDER(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_SET_BUYORDER(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_SET_SELLINFO(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_GET_LISTINFO(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_WITHDRAW_ACU(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_GET_SAVEDACU_LIST(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_HEARTBEAT(sPacketHeader *pPacket );
	void	DoMsg_NTRADESERVERRESPONSE_SET_SERVERVERSION(sPacketHeader *pPacket );
	//---------------------------------------------------------------------------------------------------------------
	//													In Here
	//---------------------------------------------------------------------------------------------------------------
private:
	ITZSessionFactory< cNTradeServer >		m_sf;					
	ITZSessionManager				m_sm;										
	ITZNetIOCP						m_iocp;										

	ITZSession*						m_pNTradeServerSession;	

	char							m_NTradeServerIP[ 20 ];
	UI16							m_NTradeServerPort;	
	SI32							m_siReason;
	DWORD							m_dwCheckLastClock;

	SI32							m_siVersion;
	bool							m_bSendVersion;
};


#endif