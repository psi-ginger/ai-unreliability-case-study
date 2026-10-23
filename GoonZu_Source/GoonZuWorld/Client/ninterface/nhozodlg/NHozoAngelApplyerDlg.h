#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;
class COutLine;

class CNHozoAngelApplyerDlg : public CDialogController
{
public:
	
	CNHozoAngelApplyerDlg();
	~CNHozoAngelApplyerDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoAngelApplyerDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoAngelApplyerDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

private:

	CStatic*	m_pStatic;
	CButton*	m_pButton[2];
	COutLine*	m_pOutLine;

};