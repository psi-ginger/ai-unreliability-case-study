
#pragma once

#include "../../../InterfaceMgr/DialogController.h"

class CNMyGuildMasterHuntMap : public CDialogController
{
public:

	CNMyGuildMasterHuntMap();
	~CNMyGuildMasterHuntMap ();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticMyGuildMasterHuntMapDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildMasterHuntMapDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

private:



};

