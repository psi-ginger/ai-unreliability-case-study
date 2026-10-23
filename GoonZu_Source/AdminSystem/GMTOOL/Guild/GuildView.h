#pragma once


// CGuildView 대화 상자입니다.

class CGuildView : public CDialog
{
	DECLARE_DYNAMIC(CGuildView)

public:
	CGuildView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGuildView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GUILDVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
