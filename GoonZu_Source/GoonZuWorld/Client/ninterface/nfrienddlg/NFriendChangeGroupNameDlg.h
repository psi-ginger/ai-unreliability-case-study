#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNFriendChangeGroupNameDlg : public CDialogController
{
public:
	CNFriendChangeGroupNameDlg();
	~CNFriendChangeGroupNameDlg();

	void Initialize();
	void Destroy();

	void Create();

	static void CALLBACK StaticFriendChangeGroupNameDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NFriendChangeGroupNameDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void SetGroupName();
};