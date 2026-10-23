#pragma once

#include "Client.h"
#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-System.h"

#define MAX_POWER	300		// 성능부분에 입력되는 최대값

class CNEquipmentCreateDlg : public CDialogController
{
public:
	CNEquipmentCreateDlg();
	~CNEquipmentCreateDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticCallBackDialogCNCNEquipmentCreateProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK CallBackDialogCNEquipmentCreateDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
	void GetItemInfo(CComboBox* pComboBox, SI32 siTypeindex);		// 아이템 생성을 위한 정보 가져오기

	void AddComboItem();
	void AddComboProPerty();
	void AddComboEnchant();
	void AddComboSlot();

	void SetCheckBox (SI32 siControllID);

	void SetItemInfo();					// 아이템을 생성

private:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_siSelectedIndex;
	SI32				m_siWeaponUnique;
	SI32				m_siHelmetUnique;
	SI32				m_siArmorUnique;
	SI32				m_siBeltUnique;
	SI32				m_siShoeUnique;
	SI32				m_siNeckUnique;
	SI32				m_siRingUnique;
	SI32				m_siMantleUnique;

	SI32				m_siWeaponProPerty;
	SI32				m_siHelmetProPerty;
	SI32				m_siArmorProPerty;
	SI32				m_siBeltProPerty;
	SI32				m_siShoeProPerty;

	SI32				m_siWeaponEnchant;
	SI32				m_siHelmetEnchant;
	SI32				m_siArmorEnchant;
	SI32				m_siBeltEnchant;
	SI32				m_siShoeEnchant;

	SI32				m_siWeaponSlot;
	SI32				m_siHelmetSlot;
	SI32				m_siArmorSlot;
	SI32				m_siBeltSlot;
	SI32				m_siShoeSlot;

	bool				m_bCheckWeapon;
	bool				m_bCheckHelmet;
	bool				m_bCheckArmor;
	bool				m_bCheckBelt;
	bool				m_bCheckShoe;
	bool				m_bCheckNeck;
	bool				m_bCheckRing;
	bool				m_bCheckMantle;

	SI32				m_siaArrItemTypeIndex[MAX_ITEMTYPE_NUMBER];
};