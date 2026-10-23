#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CEdit;
class CStatic;
class CList;
class COutLine;

struct HyungzoPoliceList;
class CNHyungzoCriminalListDlg;

class CNHyungzoPoliceDlg : public CDialogController
{
public:

	CNHyungzoPoliceDlg();
	~CNHyungzoPoliceDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHyungzoPoliceDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHyungzoPoliceDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetEnableButton(BOOL flag);
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화

	void ShowListDlg(HyungzoPoliceList* stHyungzoPoliceList);

public:

	CEdit*		m_pEdit[3];		// 포교로 등록할 사람 이름쓰기
	CList*		m_pList;		// 포교 리스트
	CButton*	m_pButton[4];   // 0: 포교 설정 1: 삭제 2: 현행범 목록 보기 3: 신고하기
	CStatic*	m_pStatic[5];
	COutLine*   m_pOutLine;

	SI32		m_siSelectList;
};