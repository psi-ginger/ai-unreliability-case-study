#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNMakeItemDemoDlg : public CDialogController
{
public:
	CNMakeItemDemoDlg();
	~CNMakeItemDemoDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticMakeItemDemoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMakeItemDemoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	void Action();

	void ShowMakeItemDemoDlg(bool bShow);

protected:
	InterfaceFactory	m_InterfaceFactory;

	BOOL				m_bExecute;

	SI32				m_siItemUnique;
	SI32				m_siTotal;
	SI32				m_siRare1;
	SI32				m_siRare2;
	SI32				m_siRare3;
};