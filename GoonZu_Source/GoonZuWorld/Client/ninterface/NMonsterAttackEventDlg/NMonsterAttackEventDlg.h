//*************************************************************************
//
//	황진성 2008. 1. 23 => 흑의 군단 개편 후 진행 상황 정보창 변경.
//
//*************************************************************************

#ifndef _NMOSTERATTACK_H_
#define _NMOSTERATTACK_H_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../../InterfaceMgr/Interface/ListView.h"


class NMonsterAttackEventDlg : public CDialogController
{
public:
	enum
	{	
		IMAGE_BACK,
		IMAGE_MONSTER_FACE,
		IMAGE_WANTED_TEXT,
		IMAGE_NUM,
	};


public:
	
	NMonsterAttackEventDlg();
	~NMonsterAttackEventDlg();

	void Create();
	void Action();
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	void MonsterDie(SI32 _Slot);
	void SetRemainSecondTime(DWORD _dwRemainSecondTime);

	static void CALLBACK StaticNMonsterAttackEventDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NMonsterAttackEventDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
  
  

private:
	InterfaceFactory	m_InterfaceFactory;

	TSpr				m_Spr;
	bool				m_bMonsterDie[CMonsterAttackEvent::MOSTER_NUM];
	
	DWORD				m_dwStartTickCount;
	DWORD				m_dwRemainSecondTime;
};

#endif