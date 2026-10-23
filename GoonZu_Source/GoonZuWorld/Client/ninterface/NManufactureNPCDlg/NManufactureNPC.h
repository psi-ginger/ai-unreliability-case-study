#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNManufactureNPCDlg : public CDialogController
{
public:
	CNManufactureNPCDlg();
	~CNManufactureNPCDlg();

    void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticManufactureNPCDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NManufactureNPCDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

    InterfaceFactory	m_InterfaceFactory;


	void SetMakeableItemList();

	void RegistItem();
	void CancelRegistItem();

	void RequestStart();
	void RequestStop();

	void SetItemImg( SI32 unique );
	void RequestSelectedItemPrice( SI32 unique );
	void SetMenuFactureFee( GMONEY price );
	void SetExplain( TCHAR* text );

	SI32 m_siManufactureItemUnique;
};