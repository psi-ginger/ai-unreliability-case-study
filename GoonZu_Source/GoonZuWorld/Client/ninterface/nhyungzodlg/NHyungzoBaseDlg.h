#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Hyungzo.h"
#include "../Lib/Directives.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CMinisterBase;
class CNHyungzoMainDlg;
class CNHyungzoUserDlg;
class CNHyungzoAnnouncementDlg;
class CNHyungzoPoliceDlg;
class CNHyungzoCriminalListDlg;

class CNHyungzoBaseDlg : public CDialogController
{
public:

	CNHyungzoBaseDlg();
	~CNHyungzoBaseDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNHyungzoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHyungzoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetHyungzo(CMinisterBase *pMini,SI16 stealnum,SI16 suspecttime,SI16 prisontime,
				    SI16 prisonerpenalty, HyungzoMember *pHyungzo);

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(SI32 Index);
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void ShowListDlg(HyungzoPoliceList stHyungzoPoliceList[]);
//	void ShowListDlg2();
	void ShowListDlg(HyungzoCriminalList stHyungzoCriminalList[]);

private:

	TCHAR m_szHyungzoName[MAX_PLAYER_NAME];
	TCHAR m_szHyungzoProfile[256];
	TCHAR m_szHyungzoID[MAX_PLAYER_NAME];

	BOOL m_bSet;
	BOOL m_bSoundCheck;

	CStatic*	m_pStatic;
	CButton*	m_pButton[9];
	CEdit*		m_pEdit;

	CNHyungzoMainDlg*			m_pNHyungzoMainDlg;
	CNHyungzoUserDlg*			m_pNHyungzoUserDlg;
	CNHyungzoAnnouncementDlg*	m_pNHyungzoAnnouncement;
	CNHyungzoPoliceDlg*			m_pNHyungzoPoliceDlg;
	CNHyungzoCriminalListDlg*	m_pNHyungzoCriminalListDlg;

	SI32	m_siActionType;
};