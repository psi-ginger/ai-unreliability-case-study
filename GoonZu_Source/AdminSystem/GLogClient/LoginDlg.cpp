// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GLogClient.h"
#include "LoginDlg.h"


// CLoginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)
CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_strLoginID(_T(""))
	, m_strLoginPW(_T(""))
{
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOGINID, m_strLoginID);
	DDX_Text(pDX, IDC_EDIT_LOGINPW, m_strLoginPW);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기입니다.
