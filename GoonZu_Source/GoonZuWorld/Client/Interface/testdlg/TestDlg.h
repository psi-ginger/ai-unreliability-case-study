#ifndef _TESTDLG_H_
#define _TESTDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CEdit;
class CListView;
class CButton;
class CList;
class CCheckBox;
class CRadioButton;

class CTestDlg1;
class CTestDlg2;
//class CTestDlg3;

class CTestDlg : public CDialogController
{
public:
	CTestDlg();
	~CTestDlg();
	
	void Create();

	static void CALLBACK StaticCallBackDialogTest( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogTest( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show();
	
private:
	CButton	*m_pButton1;
	CButton	*m_pButton2;
	CButton	*m_pButton3;

	CCheckBox		*m_pCheckBox;
	CRadioButton	*m_pRadioButton1;
	CRadioButton	*m_pRadioButton2;

	
	CTestDlg1	*m_pDlg1;
	CTestDlg2	*m_pDlg2;
	//CTestDlg3	*m_pDlg3;

	/*
	CEdit	*m_pEdit;

	CListView *m_pListView;

	CButton *m_pInsertButton;
	CButton *m_pDeleteButton;

	SI16 m_siSelectedIndex;
	
	CList	*m_pList;
	*/
};

#endif