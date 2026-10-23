#pragma once


// CChar_Status 대화 상자입니다.

class CChar_Status : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CChar_Status)

public:
	CChar_Status(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChar_Status();
// 재정의
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHAR_STAUS, IDH = IDR_HTML_CHAR_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
