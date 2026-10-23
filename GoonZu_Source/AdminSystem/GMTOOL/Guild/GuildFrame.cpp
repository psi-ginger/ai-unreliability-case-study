// Guild\GuildFrame.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "GuildFrame.h"
#include ".\guildframe.h"

// CGuildFrame

IMPLEMENT_DYNCREATE(CGuildFrame, CMDIChildWnd)

CGuildFrame::CGuildFrame()
{
}

CGuildFrame::~CGuildFrame()
{
}


BEGIN_MESSAGE_MAP(CGuildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CGuildFrame 메시지 처리기입니다.

int CGuildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_wndView.Create(IDD_CHARVIEW, this))	
	{
		return -1;
	}

	m_wndView.SetParent(this);

	RECT	rc;
	GetClientRect(&rc);
	m_wndView.MoveWindow(&rc, TRUE);


	return 0;
}

void CGuildFrame::OnDestroy()
{
	CMDIChildWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CGuildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_wndView.m_hWnd != NULL)
	{
		CRect	rc;
		GetClientRect(rc);

		m_wndView.MoveWindow(rc);
	}



}

void CGuildFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndView.SetFocus();
}

BOOL CGuildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style =	WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX ;//| WS_MAXIMIZE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;

	return CMDIChildWnd::PreCreateWindow(cs);
}
