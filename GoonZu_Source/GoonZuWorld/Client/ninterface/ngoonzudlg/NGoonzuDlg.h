#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Lib/Directives.h"


class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class CCheckBox;

class CNGoonzuDlg : public CDialogController
{
public:

	CNGoonzuDlg();
	~CNGoonzuDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNGoonzuDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNGoonzuDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(CButton* pButton, BOOL Flag);

	void SetGoonzu(CMinisterBase *pMini,SI64 finances, SI16 LiquidationVillageNumber);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SelectMinister( SI32 selectedMinister );

private:

	BOOL	m_bSoundCheck;
	TCHAR m_szGoonzuName[MAX_PLAYER_NAME];
	TCHAR m_szGoonzuProfile[256];
	TCHAR m_szGoonzuID[MAX_PLAYER_NAME];

	SI32 m_siResidentNum;
	BOOL m_bSet;
	INT64	m_iTotalFinances;
	SI16 m_siLiquidationVillageNumber;

	CStatic*	m_pStatic[4];
	CEdit*		m_pEdit[2];
	CButton*	m_pButton[6];

	CCheckBox* m_pCheck[6];
	CEdit* m_pMinister;
	CButton* m_pSetMinister;
	CButton* m_pLeaveMinister;

	SI32 m_siSelectMinister;
};