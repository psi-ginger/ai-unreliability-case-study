// [영훈] 내부 UI : Pannel (소환수정보)
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"


CEdit;
CImageStatic;

class CNPannel_SoulGuardDlg : public CDialogController
{

public:
	CNPannel_SoulGuardDlg();
	~CNPannel_SoulGuardDlg();

	void Create();

	static void CALLBACK StaticPannel_SoulGuardDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPannel_SoulGuardDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

	void Show();
	void Hide();

	void DisplaySoulGuardInfo( void );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	bool GetShowDialog() { return m_bShowDialog; }


private:
	TSpr			m_sprGage;

	bool			m_bShowDialog;				// 이 다이얼로그가 보여질지 안보여질지
	SI16			m_siShowFellwShip;

private:
	CEdit*			m_pclEditName;
	CImageStatic*	m_pclImagePortrait;

};
