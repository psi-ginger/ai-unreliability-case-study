// Block\BlockView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "BlockView.h"
#include ".\blockview.h"


// CBlockView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBlockView, CDialog)
CBlockView::CBlockView(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockView::IDD, pParent)
{
}

CBlockView::~CBlockView()
{
}

void CBlockView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBlockView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CBlockView 메시지 처리기입니다.

void CBlockView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CListCtrl* pkListCtrl_Block_Left = (CListCtrl*)GetDlgItem(IDC_BLOCK_LEFT);
	if(pkListCtrl_Block_Left == NULL)			return;

	CListCtrl* pkListCtrl_Block_Right= (CListCtrl*)GetDlgItem(IDC_BLOCK_RIGHT);
	if(pkListCtrl_Block_Right == NULL)			return;

	CRect	rc;
	GetClientRect(rc);
	CRect	listrc;
	pkListCtrl_Block_Left->GetWindowRect(listrc);
	//pkListCtrl_Block_Left->MoveWindow(listrc.left, listrc.top, rc.right - 15, rc.bottom - listrc.top - 5 - 45);

	pkListCtrl_Block_Right->GetWindowRect(listrc);
	


}

BOOL CBlockView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다
	
	CListCtrl* pkListCtrl_Block_Left = (CListCtrl*)GetDlgItem(IDC_BLOCK_LEFT);
	if(pkListCtrl_Block_Left != NULL)
	{
		pkListCtrl_Block_Left->InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
		pkListCtrl_Block_Left->InsertColumn(1, "DATE",			LVCFMT_LEFT, 140);
		pkListCtrl_Block_Left->InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);
		pkListCtrl_Block_Left->InsertColumn(3, "COMMAND",		LVCFMT_LEFT, 140);
		pkListCtrl_Block_Left->InsertColumn(4, "USER",			LVCFMT_LEFT, 120);
		pkListCtrl_Block_Left->InsertColumn(5, "TARGET USER",	LVCFMT_LEFT, 120);
		pkListCtrl_Block_Left->InsertColumn(6, "ITEM UNIQUE",	LVCFMT_LEFT, 100);
		pkListCtrl_Block_Left->InsertColumn(7, "LOGTEXT",		LVCFMT_LEFT, 220);

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_Block_Left->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_Block_Left->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_Block_Left->m_hWnd, GWL_STYLE, dwStyle);
	}


	CListCtrl* pkListCtrl_Block_Right = (CListCtrl*)GetDlgItem(IDC_BLOCK_RIGHT);
	if(pkListCtrl_Block_Right != NULL)
	{
		pkListCtrl_Block_Right->InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
		pkListCtrl_Block_Right->InsertColumn(1, "DATE",			LVCFMT_LEFT, 140);
		pkListCtrl_Block_Right->InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);
		pkListCtrl_Block_Right->InsertColumn(3, "COMMAND",		LVCFMT_LEFT, 140);
		pkListCtrl_Block_Right->InsertColumn(4, "USER",			LVCFMT_LEFT, 120);
		pkListCtrl_Block_Right->InsertColumn(5, "TARGET USER",	LVCFMT_LEFT, 120);
		pkListCtrl_Block_Right->InsertColumn(6, "ITEM UNIQUE",	LVCFMT_LEFT, 100);
		pkListCtrl_Block_Right->InsertColumn(7, "LOGTEXT",		LVCFMT_LEFT, 220);

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_Block_Right->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_Block_Right->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_Block_Right->m_hWnd, GWL_STYLE, dwStyle);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CBlockView::PreTranslateMessage(MSG* pMsg)
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
