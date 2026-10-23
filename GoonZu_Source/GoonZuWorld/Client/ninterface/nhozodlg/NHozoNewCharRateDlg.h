#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class COutLine;
class CStatic;

class CNHozoNewCharRateDlg : public CDialogController
{
public:

	CNHozoNewCharRateDlg();
	~CNHozoNewCharRateDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoNewCharRateDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoNewCharRateDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

public:

	COutLine*	m_pOutLine;
	CStatic*	m_pStatic[6];
};