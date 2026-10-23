#ifndef _NChild_H_
#define _NChild_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

#define ADD_FATHERSUPPLYER_ALREADY	1				// 이미 등록되어 있다. 
#define ADD_FATHERSUPPLYER_FULL		2				// 등록자가 꽉찼다. 
#define MAX_CHILD_LIST_IN_CLIENT_NUMBER		50		// 피후견자 최대 수

class CNChildDlg : public CDialogController
{
public:

	CNChildDlg();
	~CNChildDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNChildDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNChildDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

//	void Show();
//	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	// 스승님을 보여준다. 
	void ShowFatherName();
	// 스승님 등록자 명단을 보여준다. 
	void ShowFatherList();

	// 스승님 등록 신청자를 추가한다. 
	bool AddApplyer(cltFatherApplyerInfo* pclinfo, SI32* preturnval);
	// 스승님 등록 신청자를 삭제 한다. 
	bool DeleteApplyer(SI32 personid);
	// 특정 사용자가 리스트에 존재하는가. 
	bool IsExist(SI32 personid);

	void EnableButton(CButton* pButton, BOOL Flag);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:

	SI32			m_siSelectedIndex;	// 스승목록에서 선택된 인덱스
	cltFatherApplyerInfo clApplyerList[MAX_FATHER_APPLYER_LIST_NUMBER];
	SI32 m_siFatherApplyInx[MAX_FATHER_APPLYER_LIST_NUMBER]; // 스승 신청자 리스트 순서에 따른 인덱스

	bool			m_bUpdateFatherSwitch;
	bool			m_bUpdateFatherApplyerListSwitch;

	CButton*		m_pButton[6];
	CStatic*		m_pStatic[4];
	CList*			m_pList;	
	CEdit*			m_pEdit;

};

#endif