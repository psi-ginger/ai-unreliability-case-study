#pragma once 

#include "../Client/InterfaceMgr/DialogController.h"
#include "../CommonLogic/House/House.h"


class CEdit;
class CStatic;
class CButton;

class CHouseUnitBuffer;

class CNHouseUserNewManagementDlg : public CDialogController
{
public:

	CNHouseUserNewManagementDlg();
	~CNHouseUserNewManagementDlg();

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	void Action();
	
	static void CALLBACK StaticCallBackDialogNHouseUserNewManagementDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHouseUserNewManagementDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer);
	void SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex);
	void LoadMyHouseList();
public:

	CHouseUnitBuffer			*m_pHouseUnitBuffer;
	SI32			m_siCurrentVillageIndex;
	SI32			m_siCurrentHouseIndex;

	CStatic*	m_pInfoTextStatic;
	CEdit*		m_pInfoEdit;

	CList*		m_pList;
	CButton*	m_pExtensionButton;
	CButton*	m_pChangeNameButton;

};