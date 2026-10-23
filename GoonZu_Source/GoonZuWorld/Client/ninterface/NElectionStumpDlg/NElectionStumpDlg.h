// [영훈] 선거시스템 개편

#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CEdit;
class CButton;
class CStatic;
class COutLine;


class CNElectionStumpDlg : public CDialogController
{
	enum NELECTIONSTUMP_BUTTON
	{
		NELECTIONSTUMP_BUTTON_STUMP_START,	// 유세시작
		NELECTIONSTUMP_BUTTON_STUMP_END,	// 유세종료
		NELECTIONSTUMP_BUTTON_MINIMUM,		// 창 최소화
		NELECTIONSTUMP_BUTTON_MAXIMUM,		// 창 최대화

		//-------------------------------
		//	이 위로 추가해 주세요
		//-------------------------------
		NELECTIONSTUMP_BUTTON_MAX
	};

	enum NELECTIONSTUMP_EDIT
	{
		NELECTIONSTUMP_EDIT_CANDIDATE,		// 후보자명
		NELECTIONSTUMP_EDIT_MAINTENANCE,	// 지지사유

		//-------------------------------
		//	이 위로 추가해 주세요
		//-------------------------------
		NELECTIONSTUMP_EDIT_MAX
	};

	enum NELECTIONSTUMP_STATIC
	{
		NELECTIONSTUMP_STATIC_CANDIDATE,	// 후보자명
		NELECTIONSTUMP_STATIC_MAINTENANCE,	// 지지사유

		//-------------------------------
		//	이 위로 추가해 주세요
		//-------------------------------
		NELECTIONSTUMP_STATIC_MAX
	};

	enum NELECTIONSTUMP_OUTLINE
	{
		NELECTIONSTUMP_OUTLINE_CANDIDATE,	// 후보자명
		NELECTIONSTUMP_OUTLINE_MAINTENANCE,	// 지지사유

		//-------------------------------
		//	이 위로 추가해 주세요
		//-------------------------------
		NELECTIONSTUMP_OUTLINE_MAX
	};

	enum NELECTIONSTUMP_VARIABLE
	{
		NELECTIONSTUMP_DIALOG_MODE_MINIMUM		= 0		// 다이얼로그 모드 최소화
		,NELECTIONSTUMP_DIALOG_MODE_MAXIMUM		= 1		// 다이얼로그 모드 최대화

		,NELECTIONSTUMP_DIALOG_MININUM_WIDTH	= 120	// 최소화 되었때 최대화 버튼 위치 X
		,NELECTIONSTUMP_DIALOG_MININUM_HEIGHT	= 19	// 최소화 되었때 최대화 버튼 위치 X
	};


public:

	CNElectionStumpDlg();
	~CNElectionStumpDlg();

	void Init();
	void Destroy();

	void Create();
	void Action();

	static void CALLBACK StaticElectionStumpDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NElectionStumpDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

private:
	void StartElectionStump();
	void EndElectionStump();
	void PreLoadInformation();
	void SetDialogMode( bool bDialogMode );

private:
	CEdit*		m_pEdit[NELECTIONSTUMP_EDIT_MAX];
	CButton*	m_pButton[NELECTIONSTUMP_BUTTON_MAX];
	CStatic*	m_pStatic[NELECTIONSTUMP_STATIC_MAX];
	COutLine*	m_pOutLine[NELECTIONSTUMP_OUTLINE_MAX];

	SI16		m_siDialogWidth;
	SI16		m_siDialogHeight;

	bool		m_bDialogMode;		// 현재 윈도우 모드

};
