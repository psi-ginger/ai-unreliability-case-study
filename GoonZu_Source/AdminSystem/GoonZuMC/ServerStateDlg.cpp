// ServerStateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "ServerStateDlg.h"


// CServerStateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CServerStateDlg, CDialog)
CServerStateDlg::CServerStateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerStateDlg::IDD, pParent)
{
}

CServerStateDlg::~CServerStateDlg()
{
}

void CServerStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerStateDlg, CDialog)
END_MESSAGE_MAP()


// CServerStateDlg 메시지 처리기입니다.
