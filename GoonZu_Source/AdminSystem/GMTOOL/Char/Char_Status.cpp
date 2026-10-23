// Char\Char_Status.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "CChar_Status.h"


// CChar_Status 대화 상자입니다.

IMPLEMENT_DYNCREATE(CChar_Status, CDHtmlDialog)

CChar_Status::CChar_Status(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CChar_Status::IDD, CChar_Status::IDH, pParent)
{
}

CChar_Status::~CChar_Status()
{
}

void CChar_Status::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CChar_Status::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(CChar_Status, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CChar_Status)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CChar_Status 메시지 처리기입니다.

HRESULT CChar_Status::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

HRESULT CChar_Status::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
