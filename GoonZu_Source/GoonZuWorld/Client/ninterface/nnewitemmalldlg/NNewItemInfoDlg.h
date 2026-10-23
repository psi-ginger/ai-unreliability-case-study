#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../DBManager/GameDBManager_world/DBMsg-ItemMall.h"
//#include "../common/ItemMallManager/ItemMallManager.h"
#include "../common/item/ItemCommon/ItemMallData.h"

class CButton;
class CStatic;

// 아이템 구매시 확인 정보창
class CNNewItemMallBuyInfoDlg : public CDialogController
{
public:

	CStatic			*m_pStatic[11];
	CButton			*m_pBtn[2];

	CNNewItemMallBuyInfoDlg();
	~CNNewItemMallBuyInfoDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNNewItemMallBuyInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewItemMallBuyInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetBuyItemInfo(cltItemMallData& itemdada, SI32 itemnumm, SI32 realcash, SI32 eventcash);
	void SetCashInfo(SI32 realcash, SI32 eventcash);
};