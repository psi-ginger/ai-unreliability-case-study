#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-Guild.h"

class CNGuildDungeonApplyDlg : public CDialogController
{
public:

	CNGuildDungeonApplyDlg();
	~CNGuildDungeonApplyDlg();

	void	Init();
	void	Destroy();

	void	Create();
	void	Action();
	void	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );


	static void CALLBACK StaticGuildDungeonApplyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildDungeonApplyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory m_InterfaceFactory;

	void	SetComboBox();
	void	RefreshApplyList();
	void	SetApplyList( cltGameMsgResponse_GuildDungeon_ApplyList* pclInfo );

	void	ApplyGuildDungeon();

private:

	SI32	m_siSelectedMapIndex;
};


