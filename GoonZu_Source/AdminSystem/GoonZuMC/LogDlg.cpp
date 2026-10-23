// LogDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "LogDlg.h"


// CLogDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogDlg, CDialog)
CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
}

CLogDlg::~CLogDlg()
{
}

void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
END_MESSAGE_MAP()


// CLogDlg 메시지 처리기입니다.
