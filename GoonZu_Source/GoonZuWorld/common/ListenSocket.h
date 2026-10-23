#ifndef __CLISTENSOCKET__
#define __CLISTENSOCKET__

// 클라이언트들의 접속및 입출력 완료를 처리하는 소켓 클래스 
class CListenSocket
{	
protected:
	HANDLE				m_hCompletionPort;					

private:
	SOCKET				m_socket;						
	WSAEVENT			m_wsaEvent[2];				// 접속 처리 이벤트	
	bool				m_bShutdown;				// 루프 종료 여부 				

public:	
	CListenSocket(char *ip, int port);
	virtual	~CListenSocket();

public: 	
	void				Shutdown();						

	bool				associateHandleToIOCompetionPort(HANDLE handle, DWORD key);

	void				connectPoll();
	void				IOCompletionPoll();
	void				processPoll();

protected:	
	//---------------------------------------------------------------------------
	virtual void		occurConnect(SOCKET socket, SOCKADDR_IN *sockAddrIn) = 0;	
	virtual void		occurIOCompletion(DWORD		   completionKey, 
										  DWORD		   numberOfBytesTransferred, 
										  LPOVERLAPPED lpOverlapped) = 0;	
	virtual void		occurProcess()	= 0;

private:
	SOCKADDR_IN			getSockAddrIn(char* ip, int port);
};

#endif