#ifndef _NFather_H_
#define _NFather_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
//#include "../common/Father/Father.h"

#define ADD_FATHERSUPPLYER_ALREADY	1				// 이미 등록되어 있다. 
#define ADD_FATHERSUPPLYER_FULL		2				// 등록자가 꽉찼다. 
#define MAX_CHILD_LIST_IN_CLIENT_NUMBER		50		// 피후견자 최대 수

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNFatherDlg : public CDialogController
{
public:

	CNFatherDlg();
	~CNFatherDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNFatherDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNFatherDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

//	void Show();
//	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	// 피후견자 이름을 구한다. 
	TCHAR* GetChildName(SI32 personid);

	// 피후견자 리스트를 모두 삭제한다. 
	void DelChildList();
	void ChildSort(SI32 sisortdata[]);
	bool AddChildList(cltChildInfo* pclinfo);

	void EnableButton(CButton* pButton, TCHAR* pText, BOOL Flag = false);	// 버튼 활성화 비활성화

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
public:

	SI32				m_siSelectedIndex;
	SI32				m_siSelectedChildID;
	bool				m_bUpdateChildListSwitch;

	cltChildInfo		clChildList[MAX_CHILD_LIST_IN_CLIENT_NUMBER];
	SI32				m_siChildListInx[MAX_CHILD_LIST_IN_CLIENT_NUMBER];	// 제자 리스트 소팅후 들어갈 리스트 데이타(위치)

	CButton*			m_pButton[9];
	CStatic*			m_pStatic[3];
	CList*				m_pList;	

	CButton*		m_pResidentInviteButton;

};

#endif