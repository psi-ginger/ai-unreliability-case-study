#ifndef _DBPROTOCOL_H
#define _DBPROTOCOL_H

#include "ITZPerson.h"
#include "ITZNetworkThread.h"
#include "RichODBC.h"
#include "..\BeTheRich\Server\NTradeServerProcess\TradeServerMsg.h"

extern char						g_dbname[];
extern char						g_dbid[];
extern char						g_dbpw[];


#define BEGIN_SP(a)				sprintf( m_szSPName, "%s.dbo.%s", g_dbname, a ); m_odbc.BeginSP( m_szSPName );
#define END_SP()				m_odbc.EndSP();
#define EXEC_SQL()				if(!m_odbc.ExecSQL()) return;
#define BEGIN_FETCHDATA()		while(m_odbc.FetchData() > 0) {
#define END_FETCHDATA()			}
#define BEGIN_NEXTRECORDSET()	if(m_odbc.NextRecordSet() == 1) {
#define END_NEXTRECORDSET()		}

class cDB_server : public ITZPerson
{
public:
	cDB_server() {};
	~cDB_server() {};

public:
	virtual void Init() {};
	virtual void Close() {};
};


class DBProtocol : public ITZNetworkThread
{
public:
	DBProtocol();
	~DBProtocol();

public:
	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

	void			MessageProc();

	void			SendPacket( char *pPacket );
private:
	ITZSessionFactory< cDB_server >	m_SF;

private:
	ITZSession		*m_pSession;
	sPacketHeader	*m_pPacket;

	RichODBC		m_odbc;
//	RichODBC		m_odbcSell;
	char			m_szSPName[ 512 ];

public:
	// Ping
	void	Ping();

// 메시지 처리함수
public:
	void	SetSellOrder();
	void	CancelSellOrder();
	void	SetBuyOrder();
	void	GetListInfo();
	void	SetSellInfo();
	void	DelSellInfo();
	void	ReducePrice();
	void	DeleteSellOrder();
	void	SetSellOrder_HoldAcu();
	void	WithdrawAcu();
	void	WithdrawAcuSuccess();
	void	SavedAcuList();

	void	GetTradeItem(cltTradeItem *pclItem);
//	void	GetTradeItem_Sell(cltTradeItem *pclItem);

	void	SetTradeItem(const cltTradeItem *pclItem);

	void	GetTradeServerOrder(cltTradeServerOrder *pclOrder);
//	void	GetTradeServerOrder_Sell(cltTradeServerOrder *pclOrder);

	void	SetTradeServerOrder(const cltTradeServerOrder *pclOrder);

	class stDBMsgPtr
	{
	public:
		void (DBProtocol::*Func)(void);
	};

	stDBMsgPtr *m_pDBMsgPtr;
	void	NullFuncPtr();

	/////////////////////////////////////
	//DB 메시지 처리 함수 추가


};

#endif
