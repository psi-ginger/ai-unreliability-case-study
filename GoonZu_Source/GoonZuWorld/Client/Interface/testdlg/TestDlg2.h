#ifndef _TESTDLG2_H_
#define _TESTDLG2_H_

#include "../../InterfaceMgr/DialogController.h"

class CControllerMgr;
class CTestDlg;
class CEdit;
class CChatEdit;

class CTestDlg2 : public CDialogController
{
public:
	CTestDlg2();
	~CTestDlg2();

	void Create( CControllerMgr *pParent );

	static void CALLBACK StaticCallBackDialogTest2( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogTest2( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show();

private:
	CChatEdit	*m_pChatEdit;
	CButton		*m_pButton;
};

#endif