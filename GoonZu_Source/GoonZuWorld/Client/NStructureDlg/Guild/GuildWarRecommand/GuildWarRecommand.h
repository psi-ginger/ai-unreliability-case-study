#pragma once

#include "../../../InterfaceMgr/DialogController.h"

class cltGameMsgResponse_GuildWar_RecommandList;

class CNGuildWarRecommand : public CDialogController
{
public:

	CNGuildWarRecommand();
	~CNGuildWarRecommand ();

	void	Initialize();
	void	Destroy();

	void	Create();

	static void CALLBACK StaticGuildWarRecommandDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildWarRecommandDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory m_InterfaceFactory;

	void	SetGuildList( cltGameMsgResponse_GuildWar_RecommandList* pclinfo );
	void	InputGuildName();

};

