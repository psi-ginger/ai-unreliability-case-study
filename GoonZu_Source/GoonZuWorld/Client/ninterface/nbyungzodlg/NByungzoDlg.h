#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Server/Minister/Minister-Byungzo.h"
#include "../Lib/Directives.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class CRadioButton;
class COutLine;

class CMinisterBase;

class CNByungzoDlg : public CDialogController
{
public:

	CNByungzoDlg();
	~CNByungzoDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNByungzoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNByungzoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(CButton* pButton, BOOL Flag);				

	void Set();
	void SetByungzo(CMinisterBase *pMini,SI32 villagewartime, SI32 settime, SI32 warstarttime, SI16 setwarcount, SI32 blackarmynum);
	
	void SetPrintEditBox(BOOL flag);
	void EnableEditBox();
	bool WarCheck();

	void SetWar(SI32 settime, SI32 warstarttime, SI16 setwarcount, SI32 blackarmynum);
	void GetWar();


private:

	SI32 m_siVillageWarTime;
	BOOL m_bWarTimeSet;	// 클라이언트의 정보가 서버로 세팅이 되었는지 체크하는 플레그

	TCHAR m_szByungzoName[MAX_PLAYER_NAME];
	TCHAR m_szByungzoProfile[256];
	TCHAR m_szByungzoID[MAX_PLAYER_NAME];

	BOOL m_bSet;
	BOOL m_bByungzoCheck;
	BOOL m_bSound;

	SI32				m_siSetTime;				// 전쟁설정시간
	SI32				m_siWarStartTime;			// 전쟁시작시간
	SI16				m_siSetWarCount;			// 전쟁설정 횟수
	SI32				m_siBlackArmyNum;			// 전쟁 몬스터 설정 수
	
	CStatic*	m_pStatic[3];
	CEdit*		m_pEdit;
	CButton*	m_pButton[6];
	CRadioButton* m_pRadio[3];
	COutLine*   m_pOutLine;

	CStatic*	m_pWarStatic[12];
	CEdit*		m_pWarEdit[2];
	CComboBox*	m_pWarCombo[4];
	COutLine*   m_pWarOutLine;

	SYSTEMTIME	m_stStartTime, m_stEndTime;
    void SetWarTimeComboBox(SI32 siSelYear, SI32 siSelMonth, SI32 siSelDay);
};