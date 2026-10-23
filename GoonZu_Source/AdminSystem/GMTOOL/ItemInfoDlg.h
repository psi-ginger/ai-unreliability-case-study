#pragma once


// CItemInfoDlg 대화 상자입니다.

class CItemInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemInfoDlg)

public:
	CItemInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMINFO };
	
	void SetInfoText(TCHAR *pszInfoText);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
