#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-Guild.h"

class CNGuildDungeonDlg : public CDialogController
{
public:

	CNGuildDungeonDlg();
	~CNGuildDungeonDlg();

	void	Init();
	void	Destroy();

	void	Create();
	void	Action();

	static void CALLBACK StaticGuildDungeonDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildDungeonDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	InterfaceFactory m_InterfaceFactory;

	void	ShowDungeonList();
	void	ShowItemList();
	void	SetList( cltGameMsgResponse_GuildDungeon_Status* pclinfo );

	SI32	m_siItemUnique;
};

