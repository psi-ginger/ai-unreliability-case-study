#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;

// Áü²Û ¿Ö±¸ ¾Ë¸²Ã¢
class CNWarJoinDlg : public CDialogController
{
public:
	CButton			*m_pBtn[2];
	CStatic			*m_pStatic;

	CNWarJoinDlg();
	~CNWarJoinDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNWarJoinDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNWarJoinDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};