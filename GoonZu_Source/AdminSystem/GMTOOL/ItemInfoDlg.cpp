// ItemInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTOOL.h"
#include "ItemInfoDlg.h"


// CItemInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemInfoDlg, CDialog)
CItemInfoDlg::CItemInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemInfoDlg::IDD, pParent)
{
	AfxInitRichEdit2(); 
}

CItemInfoDlg::~CItemInfoDlg()
{
}

void CItemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CItemInfoDlg::SetInfoText(TCHAR *pszInfoText)
{

	CRichEditCtrl* richEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_ITEMINFO);
	richEdit->SetWindowText(pszInfoText);
	//SetDlgItemText(IDC_RICHEDIT_ITEMINFO, strInfoText);
}

BEGIN_MESSAGE_MAP(CItemInfoDlg, CDialog)
END_MESSAGE_MAP()


// CItemInfoDlg 메시지 처리기입니다.
