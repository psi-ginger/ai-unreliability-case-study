#pragma once 

#include "../Client/InterfaceMgr/DialogController.h"
#include "../Client/Interface/HouseUnit/HouseUnitBaseInfo.h"
#include "../CommonLogic/House/House.h"


class CEdit;
class CStatic;
class CButton;
class CComboBox;


class CHouseUnitBuffer;

class CNHouseUserShipDlg : public CDialogController
{
public:

	CNHouseUserShipDlg();
	~CNHouseUserShipDlg();

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen );

	static void CALLBACK StaticCallBackDialogNHouseUserShipDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHouseUserShipDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	//	void SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer);
	void SetHouseUserItem(cltHouseUnit *pclHouseUnit);
	void SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex);

	bool IsShipDataChanged();
	void LoadShipData();

public:

	CHouseUnitBuffer*	m_pHouseUnitBuffer;
	cltHouseUnit*		m_pclHouseUnit;
	SI32				m_siCurrentVillageIndex;
	SI32				m_siCurrentHouseIndex;

	cltShip						*m_pclHouseUnitShip;
	bool						m_bShipBuildingCreated;
	SI16						m_siShipBuildingLevel;
	SI32						m_siCurrentShipKindMake;
	SI32						m_siCurrentShipStepMake;
	SI32						m_siCurrentShipStepMakeDateVary;

	CStatic*	m_pStatic[3];	// 2: 조선소 이미지
	CComboBox*	m_pCombo;	// 배종류
	CEdit*		m_pEdit;
	CButton*	m_pButton[4]; // 0 : 조선소 짓기 1: 배 건조하기 2: 배 진수하기 3: 배 건조취소

};

