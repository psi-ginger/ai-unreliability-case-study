
#pragma once

#include "../../../InterfaceMgr/DialogController.h"

class CNMyGuildMasterManage : public CDialogController
{
public:

	CNMyGuildMasterManage();
	~CNMyGuildMasterManage ();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticMyGuildMasterManageDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildMasterManageDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

	void RefreshManager();

	SI32 m_siSelectListIndex;
	SI32 m_siSelectCharPersonID;
	SI32 m_siSelectMemberIndex;

};

