#pragma once


// CIdView 대화 상자입니다.

class CIdView : public CDialog
{
	DECLARE_DYNAMIC(CIdView)

public:
	CIdView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIdView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IDVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo5();
};
