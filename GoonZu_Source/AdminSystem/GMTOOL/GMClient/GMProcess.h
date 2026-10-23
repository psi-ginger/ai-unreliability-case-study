#ifndef _GMPROCESS_H
#define _GMPROCESS_H

#include "../../../NetworkLib/ITZNetLib/ITZSessionFactory.h"
#include "../../../NetworkLib/ITZNetLib/ITZSessionManager.h"
#include "../../../NetworkLib/ITZNetLib/ITZNetIOCP.h"
#include "../../../NetworkLib/ITZNetLib/ITZPerson.h"
#include "../../../NetworkLib/ITZNetLib/ITZRecvBuffer.h"
#include "../../../NetworkLib/ITZNetLib/ITZSendBuffer.h"

#include "../../../GoonZuWorld/GMSystem/GMMsg.h"

#define MAX_INFOLINE_SIZE	512

class cGMServer : public ITZPerson
{
public:
	cGMServer() {};
	~cGMServer() {};

	virtual void Init() {};
	virtual void Close() {};
};


#define MAX_SECTION_DATA_BUFFER		(600*1024)

class GMProcess
{
public:
	GMProcess(CDialog* m_pkParent);
	~GMProcess();

	void				Init();
	void				Connect( char* szIP, UI16 usPort );

	void				Run();
	ITZSession*			GetGMSession() { return m_pGMSession; };

	bool				IsTryingToConnect();
	bool				IsConnected();
	bool				IsFailedToConnect();
	bool				IsLoggedIn();
	bool				IsCompletedPage();

	bool				IsRequested();

	void				SendPacket( sPacketHeader* pPacket );

	char				m_szAnswerInfoFromSvr[MAX_INFOLINE_SIZE];

public:
	ITZSessionFactory< cGMServer >	m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pGMSession;					// GM세션 포인터

	char				m_szIP[ 20 ];
	UI16				m_usPort;

	DWORD				m_dwTryConnectTick;

	bool				m_bTryToConnect;
	bool				m_bFailedToConnect;

	bool				m_bLoggedIn;

	bool				m_bRequested;

	// Data
	int					m_iPageID;
	bool				m_bBeginSection;
	char*				m_pDataBuffer;				// utf-8형식으로 저장
	int					m_iDataPos;
	bool				m_bCompletedPage;
	char				m_szInfofromServer;

	//CMDIChildWnd*		m_pkNewLogFrame;
	CDialog*			m_pkGMClientView;
	

};

#endif