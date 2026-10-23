#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class COutLine;

class CNHyungzoUserDlg : public CDialogController
{
public:

	CNHyungzoUserDlg();
	~CNHyungzoUserDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHyungzoUserDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHyungzoUserDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void SetEnableButton(BOOL flag);
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화

public:

	CStatic*	m_pStatic[9];
	CEdit*		m_pEdit[4];
	CButton*	m_pButton[3];
	COutLine*	m_pOutLine;
};