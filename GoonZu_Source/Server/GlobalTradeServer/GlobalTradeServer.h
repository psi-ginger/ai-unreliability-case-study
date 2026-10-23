// GlobalTradeServer.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호
#include "ITZPerson.h"


// CGlobalTradeServerApp:
// 이 클래스의 구현에 대해서는 GlobalTradeServer.cpp을 참조하십시오.
//

class CGlobalTradeServerApp : public CWinApp
{
public:
	CGlobalTradeServerApp();

// 재정의
	public:
	virtual BOOL InitInstance();

// 구현

	DECLARE_MESSAGE_MAP()
};

extern CGlobalTradeServerApp theApp;

enum HEARTBEATSYSTEM_STATE
{
	HEARTBEATSYSTEM_STATE_NONE,
	HEARTBEATSYSTEM_STATE_HEARTBEAT_SEND,
	HEARTBEATSYSTEM_STATE_HEARTBEAT_RECEIVED,
};

class cHeartBeatSystem
{
public:
	cHeartBeatSystem()
	{
		Init();
	}

	void Init();
	bool Send_HeartBeat( ITZSession* pSession );
	bool Receive_HeartBeat( ITZSession* pSession );

protected:
	SI16			m_siState;
};

// 무역서버 서비스 버젼
// 100 : 1.00 -> 처음 개발 버젼
// 200 : 2.00 -> 수정버젼 - 1. 하트비트 사용, 2. SellInfo를 게임서버가 요청
class cGame : public ITZPerson
{
public:
	cGame()		{	m_pHeartBeatSystem = new cHeartBeatSystem;	m_dwHeartBeatClock = 0;						}
	~cGame()	{	if(m_pHeartBeatSystem)	delete m_pHeartBeatSystem;		m_pHeartBeatSystem = NULL;		}

public:
	virtual void Init() {	m_siVersion = 100;	m_dwHeartBeatClock = 0;	OutputDebugString("cGame::Init()\n");		}
	virtual void Close() {	OutputDebugString("cGame::Close()\n");	};

	bool Request_HeartBeat();
	bool Received_HeartBeat();

	SI32 m_siVersion;		// 무역서버 서비스 버젼

protected:
	cHeartBeatSystem*	m_pHeartBeatSystem;
	DWORD				m_dwHeartBeatClock;
};

class cDB : public ITZPerson
{
public:
	cDB() {};
	~cDB() {};

public:
	virtual void Init() {};
	virtual void Close() {};
};

