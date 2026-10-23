
#pragma once

#include "../../../InterfaceMgr/DialogController.h"

class CNMyGuild : public CDialogController
{
public:
	enum {
		LIST_GUILDMEMBER,
	};

	CNMyGuild();
	~CNMyGuild ();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticMyGuildDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	void	RefreshList(cltGuildInfo guildinfo);

	InterfaceFactory m_InterfaceFactory;

};

