//----------------------------------------------------------------------------
//
//	아이템을 땅에 떨어뜨렸을때 이름을 받아서 삭제 메세지를 날리는 다이얼로그
//
//	2008.05.08 손성웅
//
//----------------------------------------------------------------------------
#ifndef _NDESTROYITEMDLG_H
#define _NDESTROYITEMDLG_H

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


class NDestroyItemDlg : public CDialogController
{
public:
	NDestroyItemDlg();
	~NDestroyItemDlg();

	void Create( void );

	static void CALLBACK StaticNDestroyItemDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NNDestroyItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	void Action(void);
	void SetDropItem( SI32 ItemIndex , cltItem dropitem );
private:
	InterfaceFactory	m_InterfaceFactory;
	cltItem				*Targetitem;
	TCHAR*				pItemName; // 받아올 아이템 이름
	cltItem				dropitemtoDestroy; // 파괴할 아이템 설정
	TCHAR*				ItemNameToDestroy;
	SI32				DropItemIndex;
};

#endif