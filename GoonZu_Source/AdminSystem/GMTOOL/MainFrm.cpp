// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "GMTOOL.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include "./NGMClientProcess.h"
#include "../MasterServer/ServerInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	//	ON_COMMAND(ID_MENU_ID, OnMenuId)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.

	m_pServerInfo = NULL ;
	m_pServerInfo = new ServerInfo;

	ZeroMemory(m_pkChildWnd, sizeof(m_pkChildWnd));
}

CMainFrame::~CMainFrame()
{
	delete m_pServerInfo ;
	m_pServerInfo = NULL ;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME1))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	SetTimer(1000, 20, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기


//void CMainFrame::OnMenuId()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	default:
		{
		}
		break;
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy()
{
	CMDIFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1000);
}

int CMainFrame::GetGameServerID(TCHAR* pszWorldName)
{
	SI32 siGameServerID = 0;

	m_pServerInfo->Wait();

	for(int i=0; i<m_pServerInfo->GetWorldCount(); i++)
	{
		if(_tcscmp(m_pServerInfo->m_gameWorldInfo[i].szWorldName, pszWorldName) == 0)
		{
			siGameServerID = m_pServerInfo->m_gameWorldInfo[i].gameServerInfo.siServerID;
			break;
		}
	}

	m_pServerInfo->Release();

	return siGameServerID;
}
