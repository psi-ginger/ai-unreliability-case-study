#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;
class COutLine;

class CNYezoAngelDlg : public CDialogController
{
public:

	CNYezoAngelDlg();
	~CNYezoAngelDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNYezoAngelDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNYezoAngelDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

private:
	
	CStatic*	m_pStatic;
	CButton*	m_pButton[2];
	COutLine*	m_pOutLine;

};