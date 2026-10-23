#pragma once
#include "afxwin.h"

#include "../../NLib/NLua.h"

// CExeScript 대화 상자입니다.

class CExeScript : public CDialog
{
	DECLARE_DYNAMIC(CExeScript)

public:
	CExeScript(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExeScript();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCRIPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_Input;
	CEdit m_edit_Output;
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	NLua	m_clLua;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonExecute();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
