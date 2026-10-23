
#pragma once

#include "../../../InterfaceMgr/DialogController.h"


class CNMyGuildMaster : public CDialogController
{
public:

	CNMyGuildMaster();
	~CNMyGuildMaster ();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticMyGuildMasterDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildMasterDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

};

