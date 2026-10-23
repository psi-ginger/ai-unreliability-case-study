#ifndef _NRECOMMENDPERSON_H_
#define _NRECOMMENDPERSON_H_

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNRecommendPersonDlg : public CDialogController
{
public:

	CNRecommendPersonDlg();
	~CNRecommendPersonDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNRecommendPersonDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNRecommendPersonDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

//	void Action();

//	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	
private:
	
	CEdit*		m_pEdit;
	CButton*	m_pButton[3]; // 0 : 확인 , 1 : 취소, 2 : 찾기
	CStatic*	m_pStatic[3];
};

#endif