#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../Client/client.h"


enum NATTENDCHECKDLG_MODE
{
	NATTENDCHECKDLG_MODE_MINIMUM = 0,
	NATTENDCHECKDLG_MODE_MAXIMUM,
};

class CNAttendCheckDlg : public CDialogController
{

public:
	CNAttendCheckDlg();
	~CNAttendCheckDlg();
 
	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticAttendCheckDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAttendCheckDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void SendCheckAttend();
	void SendAttendRewardItem();

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	
	void SetAttendDay( SI32 siAttendDay );
	void SetDialogMode( bool bDialogMode );
	void SaveNowDialogPos( void );

	void DrawRaccoon( SI32 ScreenRX, SI32 ScreenRY, SI32 siDirection );

	virtual void Action();

private:
	bool		m_bDialogMode;
	
	SI32		m_siAttendDay;
	SI32		m_siOldAttendDay;
	POINT		m_pointPosition[4];
	POINT		m_pointAddNum[4];
	TSpr		m_sprRaccoon;

	SI16		m_siPosition_X;
	SI16		m_siPosition_Y;
	SI16		m_siPosition_Width;
	SI16		m_siPosition_Height;

	cltAniInfo	m_clAniInfo;
	SI32		m_siAnimation;
	SI32		m_siDirection;
	SI32		m_siFont;

	NUpdateTimer	m_kUpdateTimerForAinmation;
};

