// GlobalTradeServer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "GlobalTradeServer.h"
#include "GlobalTradeServerDlg.h"

#include "../BeTheRich/Server/NTradeServerProcess/TradeServerMsg.h"

#include "GameProtocol.h"
#include "DBProtocol.h"
#include "DBProcess.h"
#include "DBLogProtocol.h"

GameProtocol	g_GameProtocol;
DBProtocol		g_DBProtocol;
DBProcess		g_DBProcess;

DBLogProtocol	g_LogProtocol;

char		g_dbip[ 128 ];
char		g_dbname[ 128 ];
char		g_dbid[ 128 ];
char		g_dbpw[ 128 ];
unsigned short		g_listenport;
unsigned short		g_loglistenport;

int			g_maxclient;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGlobalTradeServerApp

BEGIN_MESSAGE_MAP(CGlobalTradeServerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGlobalTradeServerApp 생성

CGlobalTradeServerApp::CGlobalTradeServerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CGlobalTradeServerApp 개체입니다.

CGlobalTradeServerApp theApp;


// CGlobalTradeServerApp 초기화

BOOL CGlobalTradeServerApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성한 응용 프로그램"));

	// SJY
	FILE* fp;
	char buffer[1024] = "" ;
	char ip[20] = "" ;

	//fp = fopen("ServerIP.txt", "rt");
	fp = fopen("ServerInfo.txt", "rt");
	if(fp == NULL)
	{
		MessageBox(NULL," No <ServerIP.txt> file", "Error",MB_OK);
		return FALSE;
	}

	char szName[128];
	char szValue[128];

	while( fgets(buffer, 1024, fp) != NULL )
	{
		sscanf( buffer, "%s %s", szName, szValue );

		if		( strcmp( szName, "TradeServerIP") == 0 )		{	strncpy(ip, szValue, sizeof(ip));				}
		else if	( strcmp( szName, "ClientListenPort") == 0 )	{	g_listenport = atoi(szValue);					}
		else if	( strcmp( szName, "DatabaseName") == 0 )		{	strncpy(g_dbname, szValue, sizeof(g_dbname));	}
		else if	( strcmp( szName, "DatabaseIP") == 0 )			{	strncpy(g_dbip, szValue, sizeof(g_dbip));		}
		else if	( strcmp( szName, "DatabaseLoginID") == 0 )		{	strncpy(g_dbid, szValue, sizeof(g_dbid));		}
		else if	( strcmp( szName, "DatabaseLoginPW") == 0 )		{	strncpy(g_dbpw, szValue, sizeof(g_dbpw));		}
		else if	( strcmp( szName, "LogClientListenPort") == 0 )	{	g_loglistenport = atoi(szValue);				}
	}

//	fgets(buffer, 1024, fp);
//	sscanf(buffer, "%s",ip);
	fclose(fp);

	g_GameProtocol.Init();
	g_GameProtocol.CreateThread();
	g_GameProtocol.GetListen()->Start();

	g_LogProtocol.Init( g_loglistenport, g_dbip, g_dbname, g_dbid, g_dbpw );
	g_LogProtocol.CreateThread();
	g_LogProtocol.GetListen()->Start();

//#ifdef _DEBUG
	//g_DBProcess.Init( "116.127.222.221", 20002 );
//#else
	g_DBProcess.Init(ip, 20012);
//#endif
	g_DBProcess.Run();

//	strcpy(g_dbname, "TradeServer");
//	strcpy(g_dbid, "TradeServerClient");
//	strcpy(g_dbpw, "emfla");

	g_DBProtocol.Init( 20012 , g_dbip, g_dbname, g_dbid, g_dbpw );
	g_DBProtocol.CreateThread();
	g_DBProtocol.GetListen()->Start();

	CGlobalTradeServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 대화 상자가 확인을 눌러 없어지는 경우 처리할
		// 코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 대화 상자가 취소를 눌러 없어지는 경우 처리할
		// 코드를 배치합니다.
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}

void cHeartBeatSystem::Init()
{
	m_siState =	HEARTBEATSYSTEM_STATE_NONE;
}

bool cHeartBeatSystem::Send_HeartBeat( ITZSession* pSession )
{
	if( pSession == NULL )									return false;
	if( pSession->GetState() != SESSION_STATE_ESTABLISHED )	return false;

	switch( m_siState )
	{
	case HEARTBEATSYSTEM_STATE_NONE:
	case HEARTBEATSYSTEM_STATE_HEARTBEAT_RECEIVED:		{					}		break;

	// 이전에 요청한 하트비트에 대한 결과를 받지 못하면, 요청하지 않는다.
	case HEARTBEATSYSTEM_STATE_HEARTBEAT_SEND:			{	return false;	}		break;
	}

	NTradeServerResponse_HeartBeat clHeartBeat;
	pSession->GetSendBuffer()->Write((char*)&clHeartBeat);

	// 하트비트를 요청한 것으로 체크
	m_siState = HEARTBEATSYSTEM_STATE_HEARTBEAT_SEND;

	return true;
}

bool cHeartBeatSystem::Receive_HeartBeat( ITZSession* pSession )
{
	if( pSession == NULL )									return false;
	if( pSession->GetState() != SESSION_STATE_ESTABLISHED )	return false;

	// 이전에 하트비트를 요청한 기록이 없다면, 요청 취소
	if( m_siState != HEARTBEATSYSTEM_STATE_HEARTBEAT_SEND )		return false;

	// 결과를 받은 것으로 체크
	m_siState = HEARTBEATSYSTEM_STATE_HEARTBEAT_RECEIVED;

	return true;
}

bool cGame::Request_HeartBeat()
{
	if( m_pHeartBeatSystem == NULL )			return false;
	
	if( m_dwHeartBeatClock == 0 )
	{
		// 초기 시간 설정
		m_dwHeartBeatClock = GetTickCount();
		return true;
	}

	if( GetSession() && GetSession()->GetState() == SESSION_STATE_ESTABLISHED )
	{
		if( GetTickCount() - m_dwHeartBeatClock >= 30 * 1000 )		// 30초마다 한번씩 요청한다.
		{
			m_dwHeartBeatClock = GetTickCount();
			if( m_pHeartBeatSystem->Send_HeartBeat( GetSession() ) == false )
			{
				// 하트비트를 제대로 보내지 못하면, 연결을 끊는다.
				GetSession()->CloseSocket();
				return false;
			}
		}
	}
	return true;
}

bool cGame::Received_HeartBeat()
{
	if( m_pHeartBeatSystem == NULL )			return false;

	if( GetSession() && GetSession()->GetState() == SESSION_STATE_ESTABLISHED )
	{
		if( m_pHeartBeatSystem->Receive_HeartBeat( GetSession() ) == false )
		{
			// 하트비트를 제대로 받지 못하면, 연결을 끊는다.
			GetSession()->CloseSocket();
			return false;
		}
	}
	return true;
}
