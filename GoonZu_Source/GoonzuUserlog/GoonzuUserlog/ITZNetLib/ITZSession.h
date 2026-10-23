#ifndef _ITZSESSION_H
#define _ITZSESSION_H

#include "ITZObject.h"

#define SESSION_STATE_NONE				0
#define SESSION_STATE_CONNECTED			1
#define	SESSION_STATE_ESTABLISHED		2
#define SESSION_STATE_DISCONNECTED		3

#define RECV_POSTED		1
#define SEND_POSTED		2

class ITZSession;
class ITZRecvBuffer;
class ITZSendBuffer;

class ITZEncrypt;

class ITZPerson;

struct PER_IO_DATA 
{
	OVERLAPPED	overlapped;
	DWORD		dwOperationType;
	DWORD		dwIoSize;
	DWORD		dwFlags;
};

class ITZSession : public ITZObject
{
friend class ITZSessionManager;

public:
	ITZSession();
	virtual ~ITZSession();

public:
	virtual void		Init();
	virtual void		Close();


	void				SetBuffer( SI32 siRecvBufSize, SI32 siSendBufSize, SI32 headerType = 0 ); 

	BOOL				InitConnect( char *ip, UI16 usPort );

	BOOL				SetSocket( SOCKET socket, SOCKADDR_IN sockaddr );
	BOOL				CloseSocket();

	ITZRecvBuffer*		GetRecvBuffer() { return m_pRecvBuffer; };
	ITZSendBuffer*		GetSendBuffer() { return m_pSendBuffer; };	

	ITZEncrypt*			GetEncrypt() { return m_pEncrypt; };

	SOCKET				GetSocket() { return m_socket; }	

	void				SetPerson( ITZPerson *pPerson ) { m_pPerson = pPerson; };
	ITZPerson*			GetPerson() { return m_pPerson; };
	
	void				SetState( SI32 state );
	SI32				GetState() { return m_siState; };

	char*				GetIP() { return m_szIP; };

private:
	BOOL				_connect();
	BOOL				_closesocket();

public:
	PER_IO_DATA			m_sendIoData;						// IOCP
	PER_IO_DATA			m_recvIoData;

public:
	SOCKET				m_socket;
	SOCKADDR_IN			m_sockaddr;

	ITZRecvBuffer*		m_pRecvBuffer;
	ITZSendBuffer*		m_pSendBuffer;

	ITZEncrypt*			m_pEncrypt;		

	ITZPerson*			m_pPerson;

	char				m_szIP[ 64 ];
	UI16				m_usPort;

	SI32				m_siState;

	BOOL				m_bAttempting;
	DWORD				m_dwConnectTick;
	DWORD				m_dwCloseTick;

	CRITICAL_SECTION	m_cs;


};

#endif

