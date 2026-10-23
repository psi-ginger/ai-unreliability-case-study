/* ==========================================================================
	클래스 :		CNDailyQuestSelectDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#pragma once

#include <Directives.h>

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CDailyQuestObjData;

class	CImageStatic	;
class	CListView		;
class	CStatic	;
class	CButton	;
class	CEdit	;
class	CList	;
class	COutLine	;

class CNDailyQuestSelectDlg : public CDialogController
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


	CNDailyQuestSelectDlg();
	~CNDailyQuestSelectDlg();
	
	void Initialize();

	static	void CALLBACK NDailyQuestSelectStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NDailyQuestSelectProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();
	void SetNInventoryInListView(CListView* in_pListView, cltItem* in_pCltItem );
	void NListSort( CList* in_pList, SI16 in_SortColumn = 0);

	bool m_bShow;

	void LoadDailyQuestList();
	
	void Refresh();

	void SetList( CDailyQuestObjData *pDailyQuestObjDataList );
	
	void LoadCurrentDailyQuestSituationText();
	
	void LoadDailyQuestListInfo( TCHAR *MonsterName, SI16 MonsterNum, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp, SI32 LeftTime, GMONEY Fee );
	void DailyQuestComplete( bool bComplete );

	void DrawDailQuestInfo(GMONEY Fee , TCHAR *MonsterName, SI16 MonsterNum, SI32 LeftTime, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp);

	void SelectRewardMoney( BOOL MoneySelected );
	

private:
	
	CDailyQuestObjData *m_pDailyQuestObjDataList;

	SI16 m_siSelectedListIndex;
	

	bool m_bInitStatus;

	bool m_bDailyQuestSuccessEnd;
	SI16 m_siDailyQuestSuccessViewCounter;

	BOOL m_bMoneySelected;

	CImageStatic		*m_pImageStatic_portrait	;
	CListView		*m_pListView_reward_item	;
	CStatic		*m_pStatic_NONAME1	;
	CStatic		*m_pStatic_NONAME2	;
	CStatic		*m_pStatic_NONAME3	;
	CStatic		*m_pStatic_NONAME4	;
	CButton		*m_pButton_questrefresh	;
	CButton		*m_pButton_queststart	;
	CButton		*m_pButton_queststop	;
	CEdit		*m_pEdit_questexplain	;
	CEdit		*m_pEdit_condition_fee	;
	CEdit		*m_pEdit_condition_monster	;
	CEdit		*m_pEdit_monster_num	;
	CEdit		*m_pEdit_condition_time	;
	CEdit		*m_pEdit_reward_item	;
	CEdit		*m_pEdit_reward_num	;
	CEdit		*m_pEdit_reward_exp	;
	CList		*m_pList_quest_select	;
	COutLine		*m_pOutLine_NONAME1	;

};

