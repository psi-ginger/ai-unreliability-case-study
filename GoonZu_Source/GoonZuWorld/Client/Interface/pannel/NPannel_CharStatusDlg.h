// [영훈] 내부 UI : Pannel (케릭터 기본정보)
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../NLib/NUpdateTimer.h"


class CNPannel_CharStatusDlg : public CDialogController
{
enum CHARSTATUS_VARIABLE
{
	CHARSTATUS_HP_NUM		= 0
	,CHARSTATUS_MP_NUM		= 4
	,CHARSTATUS_EXP_NUM		= 8

	// 게이지의 세로 길이
	,CHARSTATUS_BAR_WIDTH		= 144
	// HP 게이지 시작위치
	,CHARSTATUS_HP_START_X		= 96
	,CHARSTATUS_HP_START_Y		= 40
	// MP 게이지 시작위치
	,CHARSTATUS_MP_START_X		= 96
	,CHARSTATUS_MP_START_Y		= 52
	// EXP 게이지 시작위치
	,CHARSTATUS_EXP_START_X		= 96
	,CHARSTATUS_EXP_START_Y		= 64
	// 배고픔 시작위치
	,CHARSTATUS_HUNGRY_START_X	= 117
	,CHARSTATUS_HUNGRY_START_Y	= 7
	// 현재 배고픔 알리기 크기
	,CHARSTATUS_HUNGRY_WIDTH	= 8

	// 안에서 쓰일 텍스트 버퍼 사이즈
	,CHARSTATUS_BUF_SIZE		= 128

};

public:
	CNPannel_CharStatusDlg();
	~CNPannel_CharStatusDlg();

	void Create();

	static void CALLBACK StaticPannel_CharStatusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPannel_CharStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );

	void Show();
	void Hide();


private:
	void DrawCharStatus( SI32 ScreenRX, SI32 ScreenRY );// 캐릭터 상태 그리기 처리 함수
	void DrawHungry( SI32 ScreenRX, SI32 ScreenRY );	// 배고픔 그리기 처리 함수
	void WriteCharStatus( void );						// 케릭터 상태 텍스트 관련 처리 함수
	void WriteToolTip( void );							// 말풍선 처리 함수

	bool GetToolTipText( TCHAR* pszToolTip );			// 말풍선 텍스트를 갖고 온다
	SI32 GetHealthTextNum( void );						// 건강관련 정보 텍스트메니져 번호를 갖고 온다

private:
	InterfaceFactory	m_InterfaceFactory;

	TSpr				m_sprGage;
	TSpr				m_sprHungry;

	SI32				m_siHungryStatus;

	NUpdateTimer		m_kUpdateTimer_DrawToolTip;

};
