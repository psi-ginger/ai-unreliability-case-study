#pragma once


// CCharView 대화 상자입니다.

class CCharView : public CDialog
{
	DECLARE_DYNAMIC(CCharView)

public:
	CCharView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCharView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHARVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkListname(NMHDR *pNMHDR, LRESULT *pResult);
};
