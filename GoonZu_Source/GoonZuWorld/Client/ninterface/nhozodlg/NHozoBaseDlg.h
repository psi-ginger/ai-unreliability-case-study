#pragma once 


#include "../common/CommonHeader.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Lib/Directives.h"

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;


class CMinisterBase;
class CNHozoResidentDlg;
class CNHozoPrizeDlg;
class CNHozoChangedNameDlg;
class CNHozoFixazionDlg;
class CNHozoNewCharRateDlg;

class CNHozoAppointmentDlg;
class CNHozoAngelDlg;
class CNHozoAngelApplyerDlg;

class CNHozoBankruptcyDlg : public CDialogController
{
public:

	CNHozoBankruptcyDlg();
	~CNHozoBankruptcyDlg();

	void Create( CControllerMgr *pParent );
	
	void Show();
	void Hide();

	CControllerObj* AddInterface( SI16 Type, SI32 TypeID );

	static void CALLBACK StaticCallBackDialogNHozoBankruptcyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoBankruptcyDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetBankCruptcyVillageInfo( SI16 LiquidationVillageNumber, SI32 Date );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
public:

	InterfaceFactory m_InterfaceFactory_;
	SI32 m_siSelectNumber;
};

class CNHozoBaseDlg : public CDialogController
{
public:

	CNHozoBaseDlg();
	~CNHozoBaseDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNHozoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetHozoDlg( CMinisterBase *pMini,SI32 residentnum,SI32 maxshop,SI32 totalcharnum,SI32 calctime,
		             GMONEY prizemoney,SI32 prizesettime,GMONEY fixationmoney,SI32 newcharrate, 
					 SI16 BankCruptcyVillageNumber, SI32 SetBankCruptcyVillageDate,
					 GMONEY TutorialVillageDivideend );
	void Set();
	void ShowChangedNameList(SI32 page , ChangedNameChar changednamechar []);
	
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(SI32 Index);

	BOOL SetListDlg(SI32 index);
	void ShowListDlg(HozoAngelList hozoangellist []);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:
	
//	BOOL m_bRectSwitch;
	BOOL m_bHozoCheck;
	
	TCHAR m_szHozoName[MAX_PLAYER_NAME];
	TCHAR m_szHozoProfile[256];
	TCHAR m_szHozoID[MAX_PLAYER_NAME];

	GMONEY m_siPrizeMoney;
	SI32 m_siPrizeSetTime;
	GMONEY m_siFixationMoney;
	GMONEY m_siAnkaranDividendMoney;

	SI32 m_siViewDlg ;
	BOOL m_bSet;
	SI32 m_siListPage;

	CStatic*	m_pStatic;
	CEdit*		m_pEdit;
	CButton*	m_pButton[12];

	CNHozoResidentDlg*		m_pNHozoResidentDlg;
	CNHozoPrizeDlg*			m_pNHozoPrizeDlg;
//	CNHozoChangedNameDlg*	m_pNHozoChangedNameDlg;
	CNHozoFixazionDlg*		m_pNHozoFixationDlg;
	CNHozoNewCharRateDlg*	m_pNHozoNewCharRateDlg;

	CNHozoAppointmentDlg*	m_pNHozoAppointmentDlg;
	CNHozoAngelDlg*			m_pNHozoAngelDlg;
	CNHozoAngelApplyerDlg*	m_pNHozoAngelApplyerDlg;
	CNHozoBankruptcyDlg*	m_pNHozoBankruptcyDlg;
};



