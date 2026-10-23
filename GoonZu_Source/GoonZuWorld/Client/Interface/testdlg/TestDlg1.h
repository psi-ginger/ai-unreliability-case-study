#ifndef _TESTDLG1_H_
#define _TESTDLG1_H_

#include "../../InterfaceMgr/DialogController.h"

class CControllerMgr;
class CTestDlg;
class CEdit;
class CComboBox;

class CTestDlg1 : public CDialogController
{
public:
	CTestDlg1();
	~CTestDlg1();

	void Create( CControllerMgr *pParent );

	static void CALLBACK StaticCallBackDialogTest1( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogTest1( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show();

private:
	CComboBox	*m_pComboBox1;
	CComboBox	*m_pComboBox2;
	CComboBox	*m_pComboBox3;
	CEdit		*m_pEdit;
};

#endif
