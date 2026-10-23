#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNNewNotifyDlg : public CDialogController
{
public:
	CNNewNotifyDlg();
	~CNNewNotifyDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticNewNotifyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NNewNotifyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	DWORD	dwUpdateTime;

	void Action();
	void SetText(TCHAR *Title, TCHAR *strPrintData, ... );
	void SetImage(SI32 siNPCKind);
};
