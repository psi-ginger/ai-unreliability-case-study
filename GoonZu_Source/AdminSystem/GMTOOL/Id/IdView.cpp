// Id\IdView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "IdView.h"
#include ".\idview.h"

#include "../MainFrm.h"
#include "../NGMClientProcess.h"
#include "../../MasterServer/ServerInfo.h"
#include "../../MasterServer/MasterMsg.h"


// CIdView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CIdView, CDialog)
CIdView::CIdView(CWnd* pParent /*=NULL*/)
	: CDialog(CIdView::IDD, pParent)
{
}

CIdView::~CIdView()
{
}

void CIdView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIdView, CDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO5, OnCbnSelchangeCombo5)
END_MESSAGE_MAP()


// CIdView 메시지 처리기입니다.

void CIdView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CIdView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//DB REAL & LIKE
	CComboBox* pkComboBox_IdRealLike = (CComboBox*)GetDlgItem(IDC_ID_REALLIKE);
	if(pkComboBox_IdRealLike)
	{
		pkComboBox_IdRealLike->AddString("REAL");
		pkComboBox_IdRealLike->AddString("LIKE");

		pkComboBox_IdRealLike->SetCurSel(1);
	}

	CComboBox* pkComboBox_IdrName = (CComboBox*)GetDlgItem(IDC_ID_IDNAME);
	if(pkComboBox_IdrName)
	{
		pkComboBox_IdrName->AddString("ID");
		pkComboBox_IdrName->AddString("Server");

		pkComboBox_IdrName->SetCurSel(0);
	}
	
	//칼럼추가
	CListCtrl* pkListCtrl_ListID = (CListCtrl*)GetDlgItem(IDC_LISTID);
	if(pkListCtrl_ListID != NULL)
	{
		pkListCtrl_ListID->InsertColumn(0, "ID",			LVCFMT_LEFT,  100);
		pkListCtrl_ListID->InsertColumn(1, "SERVER",			LVCFMT_LEFT, 170);
		
		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_ListID->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_ListID->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_ListID->m_hWnd, GWL_STYLE, dwStyle);
	}
    	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CIdView::OnCbnSelchangeCombo5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
