#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNAutoAnswerDlg : public CDialogController
{
public:
	CNAutoAnswerDlg();
	~CNAutoAnswerDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticAutoAnswerDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAutoAnswerDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void Submit();
};