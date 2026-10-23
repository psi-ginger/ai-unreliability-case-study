#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CEdit;
class CStatic;
class CList;
class COutLine;

struct HyungzoCriminalList;

class CNHyungzoCriminalListDlg : public CDialogController
{
public:

	CNHyungzoCriminalListDlg();
	~CNHyungzoCriminalListDlg();

	void Create();
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHyungzoCriminalListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHyungzoCriminalListDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetEnableButton(BOOL flag);
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화

	void ShowListDlg(HyungzoCriminalList* stHyungzoCriminalList);

public:

	CList*		m_pList;		// 현행범 리스트
	CButton*	m_pButton[2];   // 0: 감옥 수감 1: 풀어주기

	SI32		m_siSelectList;	// 현행범 고른 인덱스
};