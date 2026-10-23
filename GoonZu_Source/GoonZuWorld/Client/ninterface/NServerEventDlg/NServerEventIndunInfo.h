#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"


class CNServerEventIndunInfoDlg : public CDialogController
{
public:
	CNServerEventIndunInfoDlg();
	~CNServerEventIndunInfoDlg();

	void Create();

	void Show();
	void Hide();

	void Action();


	static void CALLBACK StaticServerEventIndunInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NServerEventIndunInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

public:
	void SetServerEventIndunInfo( SI16 siServerUnique, SI32 siClearStep, DWORD dwRemainTime, SI32 siRemainMonster, SI64 siScore, SI32 siDungeonScore );


protected:
	InterfaceFactory	m_InterfaceFactory;

	DWORD				m_dwRemainTime;

};
