
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../../CommonLogic\MsgType-System.h"
#include "../../../CommonLogic\Msg\MsgType-Farming.h"
#include <Directives.h>

#define MAX_FARM_NUMBER	20
#define MAX_FARM_NAME_LENGTH	60

class CImageStatic;
class CStatic;
class CComboBox;
class CButton;
class CEdit;
class TSpr;
class NUpdateTimer;

class CNFarmingDlg : public CDialogController
{
public:
	bool	m_bStopFarming;
	SI32	m_siFarmingItemUnique;
	UI32	m_uiLeaseLeftTime;
	SI32	m_siSuccessPercent;

	CNFarmingDlg();
	~CNFarmingDlg();

	void Create();

	static	void CALLBACK StatiCNFarmingDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NFarmingDlgProc(  UINT nEvent, int nControlID, CControllerObj* pControl );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void   Init();
    void   Action();
	void   Show();
	void   Hide();
	void   LentLand(cltMsg* pclMsg, TCHAR* VillageName, GMONEY siRentPrice, SI32 siItemUnique);
	
	void SetFarmingItem( SI16 ItemUnique, SI32 DelayTime , SI32 SuccessPercent);
	void SetFarmingExplain();
	BOOL ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex, bool btradeinterface);
	bool IsFarmingStatus();	
	void Reset();					// 초기화 시킨다.

	void SetFarmingInfo(TCHAR* strFarmName, SI16 nFarmNum);

	BOOL ReturnAutoFarming();

	void DrawFarmingDelay(SI32 rx, SI32 ry);
	
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );

	DWORD	m_dwFarmingStartClock;
	DWORD	m_dwFarmingEndClock;

private:
	bool m_bShow;
	bool m_bAutoFarming;
	SI32 m_siDelayTime;

	SI16 m_siSelectedIndex;
	SI32 m_siFarmingItemNum;
	SI32 *m_pNFarmingItemUniqueListPtr;
	SI16 m_siBitmapAnimationIndex;
	
	TCHAR	m_strFarmName[MAX_FARM_NUMBER][MAX_FARM_NAME_LENGTH];
	SI16	m_siFarmNum[MAX_FARM_NUMBER];

	CImageStatic *m_pNFarmingImageStatic;
	CComboBox *m_pNFarmingCombo;
	CButton *m_pNFarmingButton;
	CEdit *m_pNFarmingEdit;
	CStatic *m_pNFarmingStatic1;
	CStatic *m_pNFarmingStatic2;
	CStatic *m_pNFarmingStatic3;
	CEdit *m_pNFarmInfoEdit;
	COutLine *m_pOutline;

	TSpr	m_SprFarmGage;

	NUpdateTimer	kUpdateTimer_DrawImage;

	SI08	m_siNowFontNum;
};