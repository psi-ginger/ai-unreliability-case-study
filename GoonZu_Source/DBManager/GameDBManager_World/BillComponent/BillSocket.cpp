// Socket.cpp: implementation of the CBillSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "BillSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CBillSocket::Init(SOCKET h)
{
	m_hSocket = h;

	m_sHost[0]			= 0;
	m_nPort				= 0;
	m_Contimeout.tv_sec	= 5;		// Connection Default Timeout
	m_Contimeout.tv_usec	= 0;

	m_Cmdtimeout.tv_sec	= 30;		// Command Default Timeout
	m_Cmdtimeout.tv_usec	= 0;
}

void CBillSocket::Close()
{
	::closesocket(m_hSocket);

	m_hSocket = INVALID_SOCKET;
}

CBillSocket::CBillSocket(SOCKET h)
{
	Init(h);
}

//*****************************************************************
CBillSocket::~CBillSocket()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}

int CBillSocket::Open()
{
	int nResult = 0;

	// 이미 Open된 경우
	if (m_hSocket != INVALID_SOCKET)
		return 0;

	return Connect(m_sHost, m_nPort, MAX_PACKETSIZE);
}

int CBillSocket::Connect(LPSTR lpszHostAddress, UINT nHostPort, int nSize)
{
	if (lpszHostAddress == NULL) 
		return PLPAY_SOCKERR_200;

	if (m_hSocket == INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		WSADATA	wsd; 
		int res;
		
		if( res = WSAStartup( MAKEWORD(1,1), &wsd ) != 0 )
		{
			int error = WSAGetLastError();
			return error;
		}
		m_hSocket = ::socket(PF_INET, SOCK_STREAM, 0);
		if (m_hSocket == INVALID_SOCKET)
		{
			return WSAGetLastError();
		}
	}

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	LPSTR lpszAscii = (LPSTR)lpszHostAddress;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
		{
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		}
		else
		{
			WSASetLastError(WSAEINVAL);
			return PLPAY_SOCKERR_200;
		}
	}
	sockAddr.sin_port = htons((u_short)nHostPort);

	// 해당 소켓을 Non-Blocking Mode로 설정한다.
	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	int iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, (u_long FAR*) &iMode);

	int nRet = connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	//int nRet = connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	// With a nonblocking socket, 
	// the connection attempt cannot be completed immediately. 
	// In this case, connect will return SOCKET_ERROR, 
	// and WSAGetLastError will return WSAEWOULDBLOCK.
	if (nRet == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return (WSAGetLastError());
	}

	fd_set  m_fdwrite; 
	FD_ZERO(&m_fdwrite); 
	FD_SET(m_hSocket, &m_fdwrite);
	TIMEVAL		Conntimeout;	// for connection timeout
	Conntimeout.tv_sec = 2;
	Conntimeout.tv_usec = 0;	// connection은 2초간만 기다림

	nRet = select(0, NULL, &m_fdwrite, NULL, &Conntimeout);
	if (nRet == SOCKET_ERROR)
	{
		nRet = PLPAY_SOCKERR_240;
	}
	else if (nRet == 0) // timeout
	{
		nRet = PLPAY_SOCKERR_270;
	}
	else
	{
		m_nInvalidBufferSize = nSize;
		nRet = 0;
	}
	FD_CLR(m_hSocket, &m_fdwrite);

	return (nRet);
}
