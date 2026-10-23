#pragma once


// CNoticeView 대화 상자입니다.

class CNoticeView : public CDialog
{
	DECLARE_DYNAMIC(CNoticeView)

public:
	CNoticeView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNoticeView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NOTICEVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
