#pragma once
#include "afxwin.h"


// CItemExplain 대화 상자입니다.

class CItemExplain : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CItemExplain)

public:
	CItemExplain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemExplain();
// 재정의
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG2, IDH = IDR_HTML_ITEMEXPLAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	CEdit m_ExplainEdit;
};
