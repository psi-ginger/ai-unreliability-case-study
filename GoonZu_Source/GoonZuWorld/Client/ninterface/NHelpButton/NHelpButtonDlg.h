#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNHelpButtonDlg : public CDialogController
{
public:
	CNHelpButtonDlg();
	~CNHelpButtonDlg();

	void Init();
	void Destroy();

	void Create();

	void Action();

	static void CALLBACK StaticHelpButtonDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NHelpButtonDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	SI32 m_siMinimizedSlot;

	SI32 GetMinimizedSlot() { return m_siMinimizedSlot; }
};