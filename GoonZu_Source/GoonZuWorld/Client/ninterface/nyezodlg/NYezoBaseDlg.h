#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Yezo.h"
#include "../Lib/Directives.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

//class CMinisterBase;
class CNYezoJapanWarDlg;
class CNYezoAngelDlg;
class CNYezoAngelApplyerDlg;
class CNYezoAppointmentDlg;

class CNYezoBaseDlg : public CDialogController
{
public:

	CNYezoBaseDlg();
	~CNYezoBaseDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNYezoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNYezoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetYezoBase(CMinisterBase *pMini,SI32 sword,SI32 gun,SI32 command,SI32 porter,SI32 villageunique);
	BOOL SetListDlg(SI32 index);
	void ShowListDlg(YezoAngelList yezoangellist []);
	
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(SI32 Index);

private:
	
	TCHAR m_szYezoName[MAX_PLAYER_NAME];
	TCHAR m_szYezoProfile[256];
	TCHAR m_szYezoID[MAX_PLAYER_NAME];

	BOOL m_bSet;
	BOOL m_bSoundCheck;

	CStatic*	m_pStatic;
	CButton*	m_pButton[7];
	CEdit*		m_pEdit;

	CNYezoJapanWarDlg*		m_pNYezoJapanWarDlg;
	CNYezoAngelDlg*			m_pNYezoAngelDlg;
	CNYezoAngelApplyerDlg*	m_pNYezoAngelApplyerDlg;
	CNYezoAppointmentDlg*	m_pNYezoAppointmentDlg;
};