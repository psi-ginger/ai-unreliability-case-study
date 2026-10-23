#pragma once

#include "../../InterfaceMgr/DialogController.h"

#define BIGENNERITEM_NUM		6			// 초보자 아이템의 갯수

class CNBeginnerWeaponNPCDlg : public CDialogController
{
public:
	CNBeginnerWeaponNPCDlg();
	~CNBeginnerWeaponNPCDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticBeginnerWeaponNPCDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NBeginnerWeaponNPCDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void	SetBeginnerItemList();
	void	RequestReceiveItem();

	SI32	m_siItemUnique[BIGENNERITEM_NUM];
	SI32	m_siSelectedItemUnique;
};