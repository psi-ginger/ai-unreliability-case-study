#pragma once


// CItemView 대화 상자입니다.

class CItemView : public CDialog
{
	DECLARE_DYNAMIC(CItemView)

public:
	CItemView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
