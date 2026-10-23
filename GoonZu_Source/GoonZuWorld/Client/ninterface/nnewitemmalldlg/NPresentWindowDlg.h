#pragma once

#include "../../InterfaceMgr/DialogController.h"
//#include "../common/ItemMallManager/ItemMallManager.h"
#include "../common/item/ItemCommon/ItemMallData.h"

class CButton;
class CStatic;
class CEdit;
class CRadioButton;
class CComboBox; 

class CNPresentWindowDlg : public CDialogController
{
public:

	SI32			m_siMyNcoin;
	SI32			m_siMyEventNcoin;
	SI32			m_siSelectProductKey;
	SI32			m_siSelectItemPrice;
	SI32			m_siSelectItemEventPrice;
	SI32			m_siSelectFontIndex;
	SI32			m_siBuyItemNum;
	bool			m_bIdCheck;

	TCHAR			m_szSelectItemName[50];
	TCHAR			m_szAccountID[20];
	SI32			m_siServerUnique;
	TCHAR			m_szCharName[20];

	cltItemMallData	m_clItemData[3];

	CButton*		m_pButton[6];		// 0 : 아이디 체크, 1 : 엔코인 충전, 2 : 선물 3 : 취소
	CRadioButton*	m_pRadioButton[3];
	CStatic*		m_pStatic[29];	
	CEdit*			m_pEdit[4];
	CComboBox*		m_pComboBox;


	//[진성] 중국 아이템몰 관련 인터페이스.
	CStatic*		m_pEventStaticBack;
	CStatic*		m_pEventStatic[9];

	CNPresentWindowDlg();
	~CNPresentWindowDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNPresentWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPresentWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void InitItemData();

	void SetPresentItemInfo(cltItemMallData& pitemmalldata, SI32 realcash, SI32 eventcash);

	// 기간제 아이템은 2단계로 나누어서 체크한다
	void SetPresentFirstItemInfo(TCHAR * itemname, TCHAR * explain , TCHAR * explain2, SI32 itemfontindex, SI32 realcash, SI32 eventcash);
	void SetPresentDateItemInfo(cltItemMallData* pitemmalldata);

	// ID 체크 결과값
	void AccountIdCheckResult(TCHAR* accountId, TCHAR* name, SI16 ServerUnique ,TCHAR* CharName );

	void SelectRadioButton(SI32 index);
	
	void SetServerUniqueAndName();
	BOOL   CheckSameServer(SI16 ServerUnique);
	TCHAR* GetSeverNameFromUnique(SI16 SeverUnique);
	SI16   GetSeverUniqueFromName(TCHAR* SeverName);

	void Action();

	void NHNChinaInit( void );
	void NHNChinaItemSet( void );
};