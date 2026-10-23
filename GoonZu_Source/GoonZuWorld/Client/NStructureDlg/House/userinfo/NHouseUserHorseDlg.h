#pragma once 

#include "../Client/InterfaceMgr/DialogController.h"
#include "../CommonLogic/House/House.h"


class CEdit;
class CStatic;
class CButton;
class CList;
class COutLine;

class CHouseUnitBuffer;

class CNHouseUserHorseDlg : public CDialogController
{
public:

	CNHouseUserHorseDlg();
	~CNHouseUserHorseDlg();

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	void Action();
	
	static void CALLBACK StaticCallBackDialogNHouseUserHorseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHouseUserHorseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
//	void SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer);
	void SetHouseUserItem(cltHouseUnit *pclHouseUnit);
	void SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex);

	bool IsHorseListChanged();
	void LoadHorseList();


public:

	CHouseUnitBuffer*	m_pHouseUnitBuffer;
	cltHouseUnit*		m_pclHouseUnit;
	SI32				m_siCurrentVillageIndex;
	SI32				m_siCurrentHouseIndex;
	SI32				m_siSelectHorseIndex;

	cltHorse*			m_pclHouseUnitHorseList;
	bool				m_bStableCreated[ MAX_HOUSEUNIT_HORSENUMBER ];

	COutLine*   m_pOutLine;
	CStatic*	m_pStatic[3];
	CList*		m_pList;
	CEdit*		m_pEdit;
	CButton*	m_pButton[3]; // 0 : 마굿간 짓기 1: 말 넣기 2: 말 꺼내기

};
