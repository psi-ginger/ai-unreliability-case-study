// Guild\GuildView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "GuildView.h"
#include ".\guildview.h"


// CGuildView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGuildView, CDialog)
CGuildView::CGuildView(CWnd* pParent /*=NULL*/)
	: CDialog(CGuildView::IDD, pParent)
{
}

CGuildView::~CGuildView()
{
}

void CGuildView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGuildView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGuildView 메시지 처리기입니다.

BOOL CGuildView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CListCtrl* pkListCtrl_GuildL = (CListCtrl*)GetDlgItem(IDC_LIST_GUILDL);
	if(pkListCtrl_GuildL != NULL)
	{
		pkListCtrl_GuildL->InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
		pkListCtrl_GuildL->InsertColumn(1, "DATE",			LVCFMT_LEFT, 140);
		pkListCtrl_GuildL->InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_GuildL->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_GuildL->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_GuildL->m_hWnd, GWL_STYLE, dwStyle);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGuildView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect	rc;
	GetClientRect(rc);
}



BOOL CGuildView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			{
				return TRUE;
			}
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
