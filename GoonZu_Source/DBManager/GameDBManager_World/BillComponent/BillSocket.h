#ifndef __BILLSOCKET_H_
#define __BILLSOCKET_H_

#define MAX_PACKETSIZE				1024

#include "CashHeader.h"
#include "winsock2.h"

class CBillSocket  
{
public:
	// Exception class if createsocket fails in constructor 
	CBillSocket(SOCKET h = INVALID_SOCKET);
	~CBillSocket();

	int		Open();
	void	Close();

	int		Connect(LPSTR lpszHostAddress, UINT nHostPort, int nSize );
	
	SOCKET	H() const 
	{ 
		return m_hSocket; 
	}

	SOCKET		m_hSocket;
	char		m_sHost[128];
	char		m_sMainHost[128];
	char		m_sBackupHost[128];
	int			m_nPort;
	int			m_nMainPort;
	int			m_nBackupPort;
	char		m_pBuffer[MAX_PACKETSIZE];
	TIMEVAL		m_Contimeout;	// for connection timeout
	TIMEVAL		m_Cmdtimeout;	// for command timeout
	int			m_nInvalidBufferSize;

	void	Init(SOCKET h);

};

#endif	//	__BILLSOCKET_H_
