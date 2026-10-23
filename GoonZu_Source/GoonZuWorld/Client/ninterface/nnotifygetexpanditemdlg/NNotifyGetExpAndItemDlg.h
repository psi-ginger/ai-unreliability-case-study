/* ==========================================================================
	클래스 :		CNNotifyGetExpAndItemDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#pragma once

#include <Directives.h>
#include "../../InterfaceMgr/DialogController.h"

class cltItem;
class CListView;
class CEdit;

class CNNotifyGetExpAndItemDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 80,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 80,
	};
	
	CNNotifyGetExpAndItemDlg();
	~CNNotifyGetExpAndItemDlg();
	
	static	void CALLBACK NNNotifyGetExpAndItemStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NNotifyGetExpAndItemProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();

	bool m_bShow;

	void Show( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money );
	void SetNInventoryInListView(CListView* in_pListView, cltItem* in_pCltItem );
	void Show2( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money );
	
	//[진성] 마을 이벤트 - 몬스터 습격 => 2008-4-2
	void MonsterAttackReward( SI32 ItemUnique, SI32 ItemNum, SI32 AddVillageScore );
	// 데일리 퀘스트 2 완료 메세지
	void ShowDailyQuest2( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money );
	// 데일리 퀘스트 3 완료 메세지 
	void ShowDailyQuest3( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money );
	
private:
	CListView*	m_pListView_item_image;
	CEdit*		m_pEdit_explain;	
	
	SI16 m_siCount;
	
};

