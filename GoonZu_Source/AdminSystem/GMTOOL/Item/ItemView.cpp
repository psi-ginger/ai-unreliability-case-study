// Item\ItemView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "ItemView.h"
#include ".\itemview.h"


// CItemView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemView, CDialog)
CItemView::CItemView(CWnd* pParent /*=NULL*/)
	: CDialog(CItemView::IDD, pParent)
{
}

CItemView::~CItemView()
{
}

void CItemView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CItemView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CItemView 메시지 처리기입니다.

void CItemView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect	rc;
	GetClientRect(rc);

}

BOOL CItemView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CListCtrl* pkListCtrl_List_ItemResult = (CListCtrl*)GetDlgItem(IDC_LIST_ITEMRESULT);
	if(pkListCtrl_List_ItemResult != NULL)
	{
		pkListCtrl_List_ItemResult->InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
		pkListCtrl_List_ItemResult->InsertColumn(1, "DATE",			LVCFMT_LEFT, 140);
		pkListCtrl_List_ItemResult->InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);
		pkListCtrl_List_ItemResult->InsertColumn(3, "COMMAND",		LVCFMT_LEFT, 140);
		pkListCtrl_List_ItemResult->InsertColumn(4, "USER",			LVCFMT_LEFT, 120);
		pkListCtrl_List_ItemResult->InsertColumn(5, "TARGET USER",	LVCFMT_LEFT, 120);
		pkListCtrl_List_ItemResult->InsertColumn(6, "ITEM UNIQUE",	LVCFMT_LEFT, 100);
		pkListCtrl_List_ItemResult->InsertColumn(7, "LOGTEXT",		LVCFMT_LEFT, 220);

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_List_ItemResult->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_List_ItemResult->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_List_ItemResult->m_hWnd, GWL_STYLE, dwStyle);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CItemView::PreTranslateMessage(MSG* pMsg)
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
