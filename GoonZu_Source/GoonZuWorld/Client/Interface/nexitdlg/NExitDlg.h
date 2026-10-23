#ifndef _NEXIT_H_
#define _NEXIT_H_

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CEdit;
class CStatic;

class CNExitDlg : public CDialogController
{
public:

	CNExitDlg();
	~CNExitDlg();

	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogNExitDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNExitDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

private:

	SI32			m_siFontIndex;
	CImageStatic*		m_pStatic;	// 이벤트 그림
	CButton*		m_pBtn[6];
	CButton*		m_pBtnBanner;

};

class CNExitConfirmDlg : public CDialogController
{
public:
	CButton			*m_pBtn[2];
	CStatic			*m_pStatic;

	CNExitConfirmDlg();
	~CNExitConfirmDlg();

	void Create();
	
	static void CALLBACK StaticCallBackDialogNExitConfirmDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNExitConfirmDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};

#endif