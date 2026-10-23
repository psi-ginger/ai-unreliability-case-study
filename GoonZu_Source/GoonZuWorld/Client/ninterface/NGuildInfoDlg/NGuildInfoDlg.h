
#pragma once

#include "../../InterfaceMgr/DialogController.h"

#include "Guild/Guild.h"

class cltGameMsgResponse_Guild_GetGuildList;
class cltGameMsgReponse_Guild_VillageList;

class CNGuildInfoDlg : public CDialogController
{
public:

	CNGuildInfoDlg();
	~CNGuildInfoDlg();

	void	Initialize();
	void	Destroy();

	void	Create();

	static void CALLBACK StaticMyGuildInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

	void	SetComboBoxItem( cltGameMsgReponse_Guild_VillageList* pclinfo );
	void	SetList(cltGameMsgResponse_Guild_GetGuildList* pclinfo);

private:

	
    

};

