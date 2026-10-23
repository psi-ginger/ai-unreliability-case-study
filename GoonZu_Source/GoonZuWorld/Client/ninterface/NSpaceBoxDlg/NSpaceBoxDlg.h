//*************************************************************************
//
//	황진성 2008. 1. 23 => 흑의 군단 개편 후 진행 상황 정보창 변경.
//
//*************************************************************************

#ifndef _NSPACE_BOX_DLG_H_
#define _NSPACE_BOX_DLG_H_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../../InterfaceMgr/Interface/ListView.h"

class CNSpaceBoxDlg : public CDialogController
{
public:
	enum{ SPACE_BOX, TREASURE_BOX };
	enum{	
			MSG_EXPLAIN,					// 설명.
			MSG_TIME_OVER_BOX,				// 시간 초가로 사용 불가.			
			MSG_FULL_BOX,					// 상자가 가득참.
			MSG_NO_MOVE_ITEM_SELECT,		// 이동할 아이템이 선택 안됨.
			MSG_OVER_COUNT,					// 이동하는 아이템의 개수가 가진 개수 초과.
			MSG_GACHA_ITEM_DIFFICULT_BOX,	// 가챠 아이템을 넣는 박스가 아님.
			MSG_NOMAL_ITEM_DIFFICULT_BOX,	// 일반 아이템을 넣는 박스가 아님.
			MSG_ITEM_MOVE_COUNT_RANGE,		// 아이템 이동 개수가 범위를 벗어나면.
		};

	CNSpaceBoxDlg();
	~CNSpaceBoxDlg();

	void Create();
	void Destroy();
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	static void CALLBACK StaticSpaceBoxDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK SpaceBoxDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
  

    void SetMyListView(void);
	void SetSpaceBoxListView(void);
	void SetTreasureBoxListView(void);
	void SetRemainTime(void);

	void TextMsg(SI32 _MsgType);

private:
	InterfaceFactory	m_InterfaceFactory;

	cltItem				m_clUserItem[ MAX_ITEM_PER_PERSON ];
	
	SI32				m_siKeepPos;
	SI32				m_siRecoveryPos;
	bool				m_BoxViewType;

	TSpr				m_Spr;

	CButton*			m_pKeepButton;
};

#endif