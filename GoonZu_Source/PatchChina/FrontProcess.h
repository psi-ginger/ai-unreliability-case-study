#ifndef _FRONTPROCESS_H
#define _FRONTPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetOverlapped.h"
#include "ITZPerson.h"


class CPatchDlg;

class cFront : public ITZPerson
{
public:
	cFront() {};
	~cFront() {};

	virtual void Init() { printf( "connected\n" ); };
	virtual void Close() { printf( "disconnected\n" ); };
};


class FrontProcess
{
public:
	FrontProcess();
	~FrontProcess();

	void				Init( char *szIP1, char*szIP2, UI16 usPort, CPatchDlg *pDlg, BOOL PingTest, UI08 IDCType, SI32 FrontServerIndex );
	void				Run();

	ITZSession*			GetFrontSession() { return m_pFrontSession; };

public:
	ITZSessionFactory< cFront >		m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetOverlapped				overlapped;							// IOCP 

	ITZSession*						m_pFrontSession;				// 서버에이전트 세션 포인터

	char							m_szIP1[ 64 ];
	char							m_szIP2[ 64 ];

	UI16							m_usPort;

	BOOL							m_bReqVersionInfo;				// 버전 정보를 요청 했는지 여부
	BOOL							m_bVersion;						// 버전 정보를 받았는지 여부

	CPatchDlg*						m_pPatchDlg;

	UI08							m_encrBuf[ PACKET_MAX_SIZE ];
	BOOL							m_bSecretKey;

	UI16							m_usTryConnectCount;

	BOOL							m_bRetryConnect;
	BOOL							m_bPingTest;
	UI08							m_uiIDCType;
	SI32							m_siFrontServerIndex;
};

#endif

