#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;
class COutLine;

class CNHozoAngelDlg : public CDialogController
{
public:

	CNHozoAngelDlg();
	~CNHozoAngelDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoAngelDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoAngelDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

private:
	
	CStatic*	m_pStatic;
	CButton*	m_pButton[2];
	COutLine*	m_pOutLine;

};