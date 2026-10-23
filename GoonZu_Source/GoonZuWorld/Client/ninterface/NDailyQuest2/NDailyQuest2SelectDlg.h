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

class CDailyQuest2ObjData;

class	CImageStatic	;
class	CListView		;
class	CStatic	;
class	CButton	;
class	CEdit	;
class	CList	;
class	COutLine	;

class CDailyQuest2SelectDlg : public CDialogController
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


	CDailyQuest2SelectDlg();
	~CDailyQuest2SelectDlg();
	
	void Initialize();

	static	void CALLBACK DailyQuest2SelectStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK DailyQuest2SelectProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();
	void SetNInventoryInListView(CListView* in_pListView, cltItem* in_pCltItem );
	void NListSort( CList* in_pList, SI16 in_SortColumn = 0);

	bool m_bShow;

	void LoadDailyQuest2List();
	
	void Refresh();

	void SetList( CDailyQuest2ObjData *pDailyQuest2ObjDataList );
	
	void LoadCurrentDailyQuest2SituationText();
	
	void LoadDailyQuest2ListInfo( TCHAR *MonsterName, SI16 MonsterNum, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp, SI32 LeftTime, GMONEY Fee );
	void DailyQuest2Complete( bool bComplete );

	void DrawDailQuest2Info(GMONEY Fee , TCHAR *MonsterName, SI16 MonsterNum, SI32 LeftTime, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp);

	void SelectRewardMoney( BOOL MoneySelected );

	bool FindItemFromInventory( SI32 ItemUnique);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void SetMonsterPicture(TCHAR *MonsterName);
	

private:
	
	CDailyQuest2ObjData *m_pDailyQuest2ObjDataList;

	SI16 m_siSelectedListIndex;
	

	bool m_bInitStatus;

	bool m_bDailyQuestSuccessEnd;
	SI16 m_siDailyQuestSuccessViewCounter;

	BOOL m_bMoneySelected;

	CImageStatic	*m_pImageStatic_portrait;
	CEdit			*m_pEdit_questexplain;
    
	CStatic			*m_pStatic_NONAME1;
	CListView		*m_pListView_Reward_Item;
    
	CStatic			*m_pStatic_NONAME3;
	CStatic			*m_pStatic_NONAME11;
	CStatic			*m_pStatic_NONAME2;
	CStatic			*m_pStatic_NONAME12;

	CEdit			*m_pEdit_Reward_Item;
    CEdit			*m_pEdit_Reward_EXP;
	CEdit			*m_pEdit_Reward_Amount;
	CEdit			*m_pEdit_Reward_Guild_Point;
    
	COutLine		*m_pOutLine_NONAME1;

	CStatic			*m_pStatic_NONAME10;
	CListView		*m_pListView_Monster_Picture;

	CStatic			*m_pStatic_NONAME5;
	CStatic			*m_pStatic_NONAME6;
	/*CStatic			*m_pStatic_NONAME7;*/
	CStatic			*m_pStatic_NONAME8;
	CStatic			*m_pStatic_NONAME9;

	CEdit			*m_pEdit_Quest_Time;
	CEdit			*m_pEdit_Moster_Name;
	/*CEdit			*m_pEdit_Quest_Fee;*/
	CEdit			*m_pEdit_Moster_Num;
	CEdit			*m_pEdit_Limit_Time;

	COutLine		*m_pOutLine_NONAME4;

	CList			*m_pList_quest_select;
	CButton			*m_pButton_questrefresh;
	CButton			*m_pButton_queststart;
	CButton			*m_pButton_queststop;

	SI16			ListIndex;
};

