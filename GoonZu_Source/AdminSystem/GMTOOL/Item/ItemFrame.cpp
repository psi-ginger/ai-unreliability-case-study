// Item\ItemFrame.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "ItemFrame.h"
#include ".\itemframe.h"


// CItemFrame

IMPLEMENT_DYNCREATE(CItemFrame, CMDIChildWnd)

CItemFrame::CItemFrame()
{
}

CItemFrame::~CItemFrame()
{
}


BEGIN_MESSAGE_MAP(CItemFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CItemFrame 메시지 처리기입니다.

int CItemFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_wndView.Create(IDD_ITEMVIEW, this))	
	{
		return -1;
	}

	m_wndView.SetParent(this);

	RECT	rc;
	GetClientRect(&rc);
	m_wndView.MoveWindow(&rc, TRUE);


	return 0;
}

void CItemFrame::OnDestroy()
{
	CMDIChildWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CItemFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndView.SetFocus();

}

void CItemFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CItemFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CMDIChildWnd::OnSetCursor(pWnd, nHitTest, message);
}


//BOOL CItemFrame::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	if(pMsg->message == WM_KEYDOWN)
//	{
//		switch(pMsg->wParam)
//		{
//		case VK_ESCAPE:
//		case VK_RETURN:
//			{
//				return TRUE;
//			}
//			break;
//		}
//	}
//
//	return CMDIChildWnd::PreTranslateMessage(pMsg);
//}

BOOL CItemFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style =	WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX ;//| WS_MAXIMIZE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}
