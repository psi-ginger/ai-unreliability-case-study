//*************************************************************************
//
//	황진성 2008. 1. 23 => 흑의 군단 개편 후 진행 상황 정보창 변경.
//
//*************************************************************************

#ifndef _NBLACK_WAR_REFORM_RUN_DLG_H_
#define _NBLACK_WAR_REFORM_RUN_DLG_H_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../common/Map/MapManager/MapManager.h"

class CNBlackWarReformRun : public CDialogController
{
	enum{ BIG_SIZE, SMALL_SIZE };
public:
	CNBlackWarReformRun();
	~CNBlackWarReformRun();

	void Create();
	void Destroy();
	
	void SetRunStatus(TCHAR* pWarMsg, TCHAR** WarPlayersMsg, TCHAR** WarVillageMsg);
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	
	void WindowBigSize(void);
	void WindowSmallSize(void);
	
    void Action(void);

	static void CALLBACK StaticBlackWarReformRunDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK BlackWarReformRunDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
private:
	InterfaceFactory	m_InterfaceFactory;

	TSpr				m_Spr;

	bool				m_WindowKind;
	bool				m_bDelay;
};


#endif