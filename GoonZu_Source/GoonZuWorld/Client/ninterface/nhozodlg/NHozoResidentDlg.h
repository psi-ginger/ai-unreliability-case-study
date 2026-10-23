#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CEdit;
class CStatic;
class COutLine;

class CNHozoResidentDlg : public CDialogController
{
public:

	CNHozoResidentDlg();
	~CNHozoResidentDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoResidentDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoResidentDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

public:

	COutLine*	m_pOutLine;
	CStatic*	m_pStatic[10];
	CEdit*		m_pEdit[2];
};