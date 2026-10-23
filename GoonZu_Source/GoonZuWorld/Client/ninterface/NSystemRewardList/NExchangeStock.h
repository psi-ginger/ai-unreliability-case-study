#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNExchangStockDlg : public CDialogController
{
public:
	CNExchangStockDlg();
	~CNExchangStockDlg();

	void Create();

	static void CALLBACK StaticExchangStockDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSystemExchangStockDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void SetStockAmount( SI32 LeftAmount );

	InterfaceFactory	m_InterfaceFactory;

	SI32 m_siVillageUnique;
};