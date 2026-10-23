#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CNNamingQuestListDlg : public CDialogController
{
public:
	CNNamingQuestListDlg();
	~CNNamingQuestListDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticNamingQuestListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NNamingQuestListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	//void Action();

	InterfaceFactory	m_InterfaceFactory;


	SI32 m_siSelectedIndex;
	SI32 m_siSelectedLevel;

	void ShowDetailInfo( SI32 Index );
	void StartNamingQuest();

	void SetExplain(SI32 _TextNum);
};