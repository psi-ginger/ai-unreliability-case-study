#ifndef _CNLowLevelDlg_H_
#define _CNLowLevelDlg_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"


#define MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER		20		// 피후견자 최대 수

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNLowLevelDlg : public CDialogController
{
public:

	CNLowLevelDlg();
	~CNLowLevelDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNLowLevelDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNLowLevelDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	//	void Show();
	//	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	// 이름을 구한다. 
	char* GetLowLevelName(SI32 personid);

	//리스트를 모두 삭제한다. 
	void DelLowLevelList();
	void LowLevelSort(SI32 sisortdata[]);

	void EnableButton(CButton* pButton, char* pText, BOOL Flag = false);	// 버튼 활성화 비활성화

public:
	DWORD				m_dwStratClock;
	DWORD				m_dwLastClock;
	SI32				m_siSelectedIndex;
	SI16				m_siCurrentPage;
	SI16				m_siMaxPage;
	bool				m_bUpdateLowLevelListSwitch;
	cltChildInfo		clLowLevelList[MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER];
	SI32				m_siLowLevelListInx[MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER];	// 리스트 소팅후 들어갈 리스트 데이타(위치)

	CButton*			m_pButton[3];
	CList*				m_pList;		

};

#endif
