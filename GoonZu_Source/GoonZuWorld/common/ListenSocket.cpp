#include <winsock2.h>
#include <stdio.h>
#include "ListenSocket.h"
//-----------------------------------------------------------------------------------------------------------------------
//	Name : CListenSocket()
//	Desc : 변수들을 생성,초기화하고 소켓을 클라이언트의 접속을 대기하는 상태로 만든다 		  
//-----------------------------------------------------------------------------------------------------------------------
CListenSocket::CListenSocket(char *ip, int port)
{			
	m_bShutdown = FALSE;
		
	// 1. 입출력 완료 포트 생성 
	if( NULL == (m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)))
	{
		printf("CreateIoCompletionPort() fail!\n");
	}				   	

	// 2. 클라이언트 접속 대기 
	if(INVALID_SOCKET == (m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		printf("socket() fail!\n");
	}	
	SOCKADDR_IN sockAddr = getSockAddrIn(ip, port);
	if(SOCKET_ERROR   == bind(m_socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN)))
	{
		printf("bind() fail!\n");
	}	
	if(SOCKET_ERROR   == listen(m_socket, 50))
	{
		printf("listen() fail!\n");
	}

	// 3. 클라이언트 접속시 발생하는 이벤트 생성 
	m_wsaEvent[0]	  = WSACreateEvent(); 
	WSAEventSelect(m_socket, m_wsaEvent[0], FD_ACCEPT);			

	// 4. 접속 루프를 종료하는 이벤트 생성 
	m_wsaEvent[1]	  = WSACreateEvent(); 

	/*
	linger data;
	data.l_linger = 1000;
	data.l_onoff  = 1;
	
	setsockopt( m_socket, SOL_SOCKET , SO_LINGER, (char*)&data, sizeof(data) );
	*/
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : ~CAcceptSocket()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CListenSocket::~CListenSocket()
{
	Shutdown();
		
	// 이벤트 해제 
	WSACloseEvent(m_wsaEvent[0]);
	WSACloseEvent(m_wsaEvent[1]);	

}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : getSockAddrIn()
//	Desc : 지정 IP와 포트로 만든 SOCKADDR_IN 구조체 정보를 얻어온다 
//-----------------------------------------------------------------------------------------------------------------------
SOCKADDR_IN CListenSocket::getSockAddrIn(char* ip, int port)
{
	SOCKADDR_IN sockAddrIn;
	memset(&sockAddrIn, 0, sizeof(sockAddrIn));	
	sockAddrIn.sin_family		= AF_INET;
	sockAddrIn.sin_addr.s_addr  = inet_addr(ip);
	sockAddrIn.sin_port			= htons(port);	

	return sockAddrIn;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : associateHandleToIOCompetionPort()
//	Desc : 관리할 새로운 핸들을 IOCP와 연관시킨다
//-----------------------------------------------------------------------------------------------------------------------
bool CListenSocket::associateHandleToIOCompetionPort(HANDLE handle, DWORD key)
{		
	HANDLE completionPort = CreateIoCompletionPort(handle, m_hCompletionPort, key, 0);

	if( completionPort != NULL )	
	{ 	
		m_hCompletionPort = completionPort;
		return TRUE;
	}
	else
	{
		OutputDebugString("CreateIoCompletionPort() fail!\n");							
	}

	return FALSE;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : Shutdown()
//	Desc : 접속을 처리하는 루프, 입출력을 처리하는 루프, 메인처리루프를 중지 시킨다 
//-----------------------------------------------------------------------------------------------------------------------
void CListenSocket::Shutdown()
{
	if(!m_bShutdown)
	{		
		m_bShutdown = TRUE;
						
		// 접속 루프 종료 		
		SetEvent(m_wsaEvent[1]); 
		
		// 입출력 완료 루프 종료
		PostQueuedCompletionStatus(m_hCompletionPort, 0, -1, NULL); 
		
		// 소켓 닫기 
		closesocket(m_socket);
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : connectPoll()
//	Desc : 클라이언트로부터의 접속 요청을 처리한다 
//-----------------------------------------------------------------------------------------------------------------------
void CListenSocket::connectPoll()
{
	SOCKET		 socket;			
	SOCKADDR_IN  sockAddrIn;		

	int	sockAddrSize = sizeof(sockAddrIn);	
	
	WSANETWORKEVENTS event;				
	while(!m_bShutdown)
	{					
		// 이벤트 대기 
		DWORD ret = WSAWaitForMultipleEvents(2, m_wsaEvent, FALSE, WSA_INFINITE, FALSE);						
		if(ret == WSA_WAIT_FAILED)
		{ 
			printf("WSAWaitForMultipleEvents() fail!\n");			
			break;
		}			
				
		// 접속 이벤트 발생 
		else if(ret == WSA_WAIT_EVENT_0) 
		{
			WSAResetEvent(m_wsaEvent[0]);
			
			WSAEnumNetworkEvents(m_socket, m_wsaEvent[0], &event);
			if ((event.lNetworkEvents & FD_ACCEPT) == FD_ACCEPT)
			{ 								
				if( INVALID_SOCKET == (socket = accept(m_socket, (SOCKADDR*)&sockAddrIn, &sockAddrSize)) )
				{ 			
					if(WSAGetLastError() != WSAEWOULDBLOCK)											 	
					{ 
						printf("accept() fail!\n");
						break;
					}								
				}
				else
				{					
					// 접속처리 					
					occurConnect(socket, &sockAddrIn);
				}
			}
		}

		// 종료 이벤트 발생 
		else
		{
			// 루프 종료 
			WSAResetEvent(m_wsaEvent[1]);			
			break;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : IOCompletionPoll()
//	Desc : 클라이언트로부터의 입출력완료 요청을 처리한다   
//-----------------------------------------------------------------------------------------------------------------------
void CListenSocket::IOCompletionPoll()
{
	
	DWORD		  completionKey			   = 0;	
	DWORD		  numberOfBytesTransferred = 0;
	LPOVERLAPPED  lpOverlapped  = NULL;					

	while(!m_bShutdown)
	{				
		if(FALSE == GetQueuedCompletionStatus(m_hCompletionPort, &numberOfBytesTransferred, &completionKey,
											 (LPOVERLAPPED*)&lpOverlapped, INFINITE))
		{ 
			if(lpOverlapped == NULL)
			{
				if(GetLastError()!=ERROR_IO_PENDING)
				{			
					printf("GetQueuedCompletionStatus() fail!\n");
					return; 
				}
			}
		}
		
		// 루프 종료 
		if( completionKey == -1 ) break;

		// 입출력 완료 처리		
		occurIOCompletion(completionKey	, numberOfBytesTransferred, lpOverlapped);
	}	
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : processPoll()
//	Desc : 매 프레임에 한번씩 수행되는 메인처리 루프 
//-----------------------------------------------------------------------------------------------------------------------
void CListenSocket::processPoll()
{
	//while(!m_bShutdown)
	{
		occurProcess();
	}
}




