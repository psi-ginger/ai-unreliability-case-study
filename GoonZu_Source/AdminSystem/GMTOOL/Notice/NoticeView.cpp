// Notice\NoticeView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "NoticeView.h"
#include ".\noticeview.h"


// CNoticeView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNoticeView, CDialog)
CNoticeView::CNoticeView(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeView::IDD, pParent)
{
}

CNoticeView::~CNoticeView()
{
}

void CNoticeView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoticeView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CNoticeView 메시지 처리기입니다.

BOOL CNoticeView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CListCtrl* pkListCtrl_Notice_Listt = (CListCtrl*)GetDlgItem(IDC_NOTICE_LIST);
	if(pkListCtrl_Notice_Listt != NULL)
	{
		pkListCtrl_Notice_Listt->InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
		pkListCtrl_Notice_Listt->InsertColumn(1, "DATE",			LVCFMT_LEFT, 140);
		pkListCtrl_Notice_Listt->InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);
		pkListCtrl_Notice_Listt->InsertColumn(3, "COMMAND",		LVCFMT_LEFT, 140);
		pkListCtrl_Notice_Listt->InsertColumn(4, "USER",			LVCFMT_LEFT, 120);
		pkListCtrl_Notice_Listt->InsertColumn(5, "TARGET USER",	LVCFMT_LEFT, 120);
		pkListCtrl_Notice_Listt->InsertColumn(6, "ITEM UNIQUE",	LVCFMT_LEFT, 100);
		pkListCtrl_Notice_Listt->InsertColumn(7, "LOGTEXT",		LVCFMT_LEFT, 220);

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_Notice_Listt->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_Notice_Listt->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_Notice_Listt->m_hWnd, GWL_STYLE, dwStyle);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNoticeView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CNoticeView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
