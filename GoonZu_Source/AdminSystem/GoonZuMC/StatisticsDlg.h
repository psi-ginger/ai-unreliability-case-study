#if !defined(AFX_STATISTICSDLG_H__4E975FDB_CB18_4D12_9A97_697B09E033D7__INCLUDED_)
#define AFX_STATISTICSDLG_H__4E975FDB_CB18_4D12_9A97_697B09E033D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog

class CStatisticsGraph;

class CStatisticsDlg : public CDialog
{
// Construction
public:
	CStatisticsDlg(CWnd* pParent = NULL);   // standard constructor

	CStatisticsGraph*		m_pStatisticsGraph;

	CRITICAL_SECTION		m_CS;

	bool					IsCheckSaveFile();
	void					Clear( int layer );
	void					Request( int year, int month, int day, int layer );
	
	void					DrawGraph();

	void					ShowUserNum( int layer );

	int						m_graphX;
	int						m_graphY;

	BOOL					m_bResponseData;
	int						m_siResponseLayer;
// Dialog Data
	//{{AFX_DATA(CStatisticsDlg)
	enum { IDD = IDD_DIALOG_STATISTICS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatisticsDlg)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButtonClear1();
	afx_msg void OnButtonClear2();
	afx_msg void OnButtonClear3();
	afx_msg void OnButtonClear4();
	afx_msg void OnButtonRequest1();
	afx_msg void OnButtonRequest2();
	afx_msg void OnButtonRequest3();
	afx_msg void OnButtonRequest4();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSDLG_H__4E975FDB_CB18_4D12_9A97_697B09E033D7__INCLUDED_)
