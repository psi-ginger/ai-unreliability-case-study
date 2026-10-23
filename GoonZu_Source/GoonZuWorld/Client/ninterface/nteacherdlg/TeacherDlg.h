#ifndef _NTEACHER_H_
#define _NTEACHER_H_

#include "../../InterfaceMgr/DialogController.h"
//#include "../../common/Father/Father.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNFatherDlg;
class CNChildDlg;

class CNTeacherDlg : public CDialogController
{
public:

	CNTeacherDlg();
	~CNTeacherDlg();

	void Create(bool bTeacherMode);

	static void CALLBACK StaticCallBackDialogNTeacherDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNTeacherDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

//	void ChildSort(SI32 sisortdata[]);

//	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

//	static BOOL CALLBACK StaticDlg2Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	BOOL CALLBACK Dlg2Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
/*
	// 스승님을 보여준다. 
	void ShowFatherName(HWND hDlg);
	// 스승님 등록자 명단을 보여준다. 
	void ShowFatherList();

	// 스승님 등록 신청자를 추가한다. 
	bool AddApplyer(cltFatherApplyerInfo* pclinfo, SI32* preturnval);
	// 스승님 등록 신청자를 삭제 한다. 
	bool DeleteApplyer(SI32 personid);
	// 특정 사용자가 리스트에 존재하는가. 
	bool IsExist(SI32 personid);


	// 피후견자 리스트를 구한다 
	bool AddChildList(cltChildInfo* pclinfo);

	// 피후견자 리스트를 모두 삭제한다. 
	void DelChildList();

	// 피후견자 이름을 구한다. 
	TCHAR* GetChildName(SI32 personid);

	void Action(cltCharManager* pclCM);
*/
public:

	bool			m_bTeacherMode;

	CNFatherDlg*	m_pNFatherDlg;
	CNChildDlg*		m_pNChildDlg;
/*
	bool m_bDlgRectInitSwitch;

	bool m_bUpdateFatherSwitch;
	bool m_bUpdateFatherApplyerListSwitch;
	bool m_bUpdateChildListSwitch;

	// 스승님 등록 신청자 명단. 
	cltFatherApplyerInfo clApplyerList[MAX_FATHER_APPLYER_LIST_NUMBER];

	// 제자님 리스트. 
	cltChildInfo clChildList[MAX_CHILD_LIST_IN_CLIENT_NUMBER];

	SI32 m_siSelectedIndex ;								// 선택한 리스트 데이타
	SI32 m_siFatherApplyInx[MAX_FATHER_APPLYER_LIST_NUMBER]; // 스승 신청자 리스트 순서에 따른 인덱스
	SI32 m_siChildListInx[MAX_CHILD_LIST_IN_CLIENT_NUMBER];	// 제자 리스트 소팅후 들어갈 리스트 데이타(위치)
*/

};

#endif