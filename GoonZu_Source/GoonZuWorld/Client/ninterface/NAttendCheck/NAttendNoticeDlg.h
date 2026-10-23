// [영훈] 출석체크

#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CEdit;
class CButton;


class CNAttendNoticeDlg : public CDialogController
{

	enum ATTENDNOTICE_EDIT
	{
		ATTENDNOTICE_EDIT_NOTICE		= 0		// 현재 창 설명
		,ATTENDNOTICE_EDIT_INFORMATION			// 도장 받은 갯수
		
		// 이 위로 추가해 주세요
		,ATTENDNOTICE_EDIT_MAX
	};

	enum ATTENDNOTICE_BUTTON
	{
		ATTENDNOTICE_BUTTON_STAMP		= 0		// 출석 도장 찍기
		,ATTENDNOTICE_BUTTON_CANCEL				// 출석하지 않기

		// 이 위로 추가해 주세요
		,ATTENDNOTICE_BUTTON_MAX
	};

public:
	CNAttendNoticeDlg();
	~CNAttendNoticeDlg();

	void Init();
	void Destroy();

	void Create();
	void Action();

	void Show( bool bShow );

	static void CALLBACK StaticAttendNoticeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAttendNoticeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

public:
	void SetInfo( SI16 siAttendenceDay, SI16 siNextRewardDay );
	

private:
	CEdit*		m_pEdit[ATTENDNOTICE_EDIT_MAX];
	CButton*	m_pButton[ATTENDNOTICE_BUTTON_MAX];

	SI16		m_siAttendenceDays;
	SI16		m_siNextRewardDays;
};

