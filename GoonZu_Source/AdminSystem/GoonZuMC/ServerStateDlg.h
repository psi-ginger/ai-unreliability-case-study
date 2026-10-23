#pragma once


// CServerStateDlg 대화 상자입니다.

class CServerStateDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerStateDlg)

public:
	CServerStateDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CServerStateDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERVERSTATEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
