#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNGuildSendMsgDlg : public CDialogController
{
public:

	CNGuildSendMsgDlg();
	~CNGuildSendMsgDlg();

	void	Initialize();
	void	Destroy();

	void	Create();

	static void CALLBACK StatiNGuildSendMsgDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildSendMsgDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	void	SetMasterName( TCHAR* masterName );
	void	Send();


	InterfaceFactory m_InterfaceFactory;

	TCHAR	m_szMasterName[MAX_PLAYER_NAME];

};