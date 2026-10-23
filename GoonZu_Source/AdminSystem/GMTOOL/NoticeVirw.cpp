// NoticeVirw.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTOOL.h"
#include "NoticeVirw.h"


// CNoticeVirw 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNoticeVirw, CDialog)
CNoticeVirw::CNoticeVirw(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeVirw::IDD, pParent)
{
}

CNoticeVirw::~CNoticeVirw()
{
}

void CNoticeVirw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoticeVirw, CDialog)
END_MESSAGE_MAP()


// CNoticeVirw 메시지 처리기입니다.
