// ItemExplain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GoonzuUserlog.h"
#include "ItemExplain.h"


// CItemExplain 대화 상자입니다.

IMPLEMENT_DYNCREATE(CItemExplain, CDHtmlDialog)

CItemExplain::CItemExplain(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CItemExplain::IDD, CItemExplain::IDH, pParent)
{
}

CItemExplain::~CItemExplain()
{
}

void CItemExplain::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ExplainEdit);
}

BOOL CItemExplain::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(CItemExplain, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CItemExplain)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CItemExplain 메시지 처리기입니다.

HRESULT CItemExplain::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

HRESULT CItemExplain::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
