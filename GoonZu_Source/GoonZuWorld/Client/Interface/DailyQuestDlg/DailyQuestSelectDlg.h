#ifndef _DAILYQUESTSELECTDLG_H_
#define _DAILYQUESTSELECTDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

#include "../../Client/client.h"

class CDailyQuestObjData;

class CDailyQuestSelectDlg
{
public:
	HWND m_hDlg;

	CDailyQuestSelectDlg();
	~CDailyQuestSelectDlg();
	
	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticDailyQuestSelectDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK DailyQuestSelectDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

	void LoadDailyQuestList();
	
	void Refresh();

	void SetList( CDailyQuestObjData *pDailyQuestObjDataList );
	
	void LoadCurrentDailyQuestSituationText();
	
	void LoadDailyQuestListInfo( char *MonsterName, SI16 MonsterNum, SI16 ItemUnique, char *ItemName, SI16 ItemNum, SI32 RewardExp, SI32 LeftTime, SI32 Fee );
	void DailyQuestComplete( bool bComplete );

	void DrawDailQuestInfo(SI32 Fee , char *MonsterName, SI16 MonsterNum, SI32 LeftTime, SI16 ItemUnique, char *ItemName, SI16 ItemNum, SI32 RewardExp);
	

private:
	bool m_bShow;
	
	HWND m_hDailyQuestSelectListCtrl;

	CDailyQuestObjData *m_pDailyQuestObjDataList;

	SI16 m_siSelectedListIndex;
	

	bool m_bInitStatus;
	
	HBITMAP m_hPortraitBitmap;
	HBRUSH	m_hEditBrush;
	
	bool m_bDailyQuestSuccessEnd;
	SI16 m_siDailyQuestSuccessViewCounter;
};

#endif