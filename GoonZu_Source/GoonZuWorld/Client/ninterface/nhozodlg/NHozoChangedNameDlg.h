#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNHozoChangedNameDlg : public CDialogController
{
public:

	CNHozoChangedNameDlg();
	~CNHozoChangedNameDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoChangedNameDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoChangedNameDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	inline void SetPage(SI32 Index)		{ m_siListPage = Index; }

public:

	SI32		m_siListPage;

	CStatic*	m_pStatic;
	CButton*	m_pButton[2];
	CEdit*		m_pEdit;
	CList*		m_pList;
};