#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CEdit;
class CStatic;
class COutLine;

class CNHozoPrizeDlg : public CDialogController
{
public:

	CNHozoPrizeDlg();
	~CNHozoPrizeDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoPrizeDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoPrizeDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	
	COutLine*	m_pOutLine;
	CStatic*	m_pStatic[4];
	CEdit*		m_pEdit;

};