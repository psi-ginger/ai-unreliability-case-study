#pragma once

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"


class CEachPersonTradeOrderInfo;
class CNPrivateTradeDlg;
class CLibListMgr;
class CList;
class CButton;
class CStatic;


class CNTradeChild4 : public CDialogController
{
	
public:
	CNTradeChild4();
	~CNTradeChild4();


	
	static void CALLBACK StaticNTradeChild4Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NTradeChild4Proc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Create(CControllerMgr *pParent);
	
	void LoadHouseUnitList();	
	bool IsHouseUnitListChanged();
	void Show();
	void Hide();

    CNPrivateTradeDlg*  m_pParent;
	CList               *m_pNPTrade4List;
	//SI32				m_siHouseVillageUniqueList[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];	// 시전 소속 마을
	//SI32				m_siHouseUnitSlotList[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];			// 시전 번호

private:	
	
    CStatic             *m_pNPTrade4Static1;
	CStatic             *m_pNPTrade4Static2;
	
	CButton             *m_pNPTrade4Button;

	
};