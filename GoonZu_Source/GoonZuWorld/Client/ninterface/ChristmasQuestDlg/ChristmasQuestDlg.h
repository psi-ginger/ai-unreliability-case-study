#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
/*--------------------------------------------------------------------------------------
[추가 : 황진성 2007. 11. 20 크리스마스 퀘스트 클레스 추가.]
 

--------------------------------------------------------------------------------------*/
class CNChristmasQuestDlg : public CDialogController
{ 
public:
	CNChristmasQuestDlg();
	~CNChristmasQuestDlg();

	void Init();
	void Destroy();

	void Create();

	static	void CALLBACK StaticChristmasQuestListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
			void CALLBACK NChristmasQuestListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	//void Action();

	InterfaceFactory	m_InterfaceFactory;

	SI32 m_siSelectedIndex;
	
	void StartChristmasQuest();
};