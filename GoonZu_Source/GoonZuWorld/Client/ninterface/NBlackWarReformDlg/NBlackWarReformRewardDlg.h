//*************************************************************************
//
//	황진성 2008. 1. 23 => 흑의 군단 개편 후 보상창 변경.
//
//*************************************************************************

#ifndef _NBLACK_WAR_REFORM_REWARD_DLG_H_
#define _NBLACK_WAR_REFORM_REWARD_DLG_H_

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../common/Map/MapManager/MapManager.h"

#include "../CommonLogic/MsgType-System.h"

class CNBlackWarReformReward : public CDialogController
{
	enum{ WIN, LOSE };

public:
	CNBlackWarReformReward();
	~CNBlackWarReformReward();

	void Create();
	void Destroy();
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	
	void SetMyReward(cltGameMsgResponse_Black_War_Reform_Char_Result* _MyResult);
	void SetVillageReward(cltGameMsgResponse_Black_War_Reform_Village_Result* _VillageResult);
	
    static void CALLBACK StaticBlackWarReformRewardDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK BlackWarReformRewardDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

private:
	InterfaceFactory	m_InterfaceFactory;
	TSpr				m_Spr;

	bool				m_bWinLose;
	bool				m_bMyRewardOK;
};


#endif