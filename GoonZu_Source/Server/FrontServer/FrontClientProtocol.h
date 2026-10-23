#include "ITZNetworkThread.h"

#include "FrontServer.h"

//#ifdef _JAPAN
	#define MAX_USER_CONNECTION			200
//#else
	// 중국은 500명으로
//	#define MAX_USER_CONNECTION			500
//#endif

class FrontClientProtocol : public ITZNetworkThread
{
public:
	FrontClientProtocol();
	~FrontClientProtocol();

public:
	void			Init(UI16 siListenPort);

	virtual void	Update();
	virtual void	LogOut( ITZSession *pSession );
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cClient >	m_SF;


public:
	char					m_buf[ sizeof( WORLDINFO ) * 128 ];

	UI08					m_encrBuf[ PACKET_MAX_SIZE ];

	UI16					m_usUniqueCount;

public:

	//void			SetLogFilename();
	//_SYSTEMTIME				m_systime;	
	//char					m_szLogFileName[ 128 ];
	//char					m_szFileBuf[ 1024 ];

};

