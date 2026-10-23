#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CNSelectNamingDlg : public CDialogController
{
public:
	CNSelectNamingDlg();
	~CNSelectNamingDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticSelectNamingDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSelectNamingDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void SelectNaming( SI32 Index, SI32 Level );


};