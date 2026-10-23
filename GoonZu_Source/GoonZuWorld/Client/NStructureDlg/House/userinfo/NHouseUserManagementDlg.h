#pragma once 

#include "../Client/InterfaceMgr/DialogController.h"
#include "../CommonLogic/House/House.h"


class CEdit;
class CStatic;
class CButton;

class CHouseUnitBuffer;

class CNHouseUserManagementDlg : public CDialogController
{
public:

	CNHouseUserManagementDlg();
	~CNHouseUserManagementDlg();

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	void Action();
	
	static void CALLBACK StaticCallBackDialogNHouseUserManagementDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHouseUserManagementDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer);
	void SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex);

public:

	CHouseUnitBuffer			*m_pHouseUnitBuffer;
	SI32			m_siCurrentVillageIndex;
	SI32			m_siCurrentHouseIndex;

	CStatic*	m_pStatic[2];
	CEdit*		m_pEdit[2];
	CButton*	m_pButton;

};