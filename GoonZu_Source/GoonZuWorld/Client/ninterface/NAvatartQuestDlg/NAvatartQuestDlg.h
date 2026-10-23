#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


class CNAvatarQuestDlg : public CDialogController
{ 
public:
	CNAvatarQuestDlg();
	~CNAvatarQuestDlg();

	void Init();
	void Destroy();

	void Create();

	static	void CALLBACK StaticNAvatarQuestDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
			void CALLBACK NAvatarQuestDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	//void Action();

	InterfaceFactory	m_InterfaceFactory;

	SI32 m_siSelectedIndex;
	
	void StartAvatarQuest();
};