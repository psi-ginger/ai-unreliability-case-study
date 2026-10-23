#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;
class COutLine;

class CNYezoAngelApplyerDlg : public CDialogController
{
public:

	CNYezoAngelApplyerDlg();
	~CNYezoAngelApplyerDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNYezoAngelApplyerDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNYezoAngelApplyerDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

private:

	CStatic*	m_pStatic;
	CButton*	m_pButton[2];
	COutLine*	m_pOutLine;

};