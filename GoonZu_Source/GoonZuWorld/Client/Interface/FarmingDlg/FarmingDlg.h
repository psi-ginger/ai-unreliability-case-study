/*
#ifndef _FARMINGDLG_H_
#define _FARMINGDLG_H_

#include <Directives.h>
#include <windows.h>

class CFarmingDlg
{
public:
	HWND m_hDlg;

	CFarmingDlg();
	~CFarmingDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticFarmingDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK FarmingDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();
	
	void SetFarmingItem( SI16 ItemUnique, SI32 DelayTime );
	void SetFarmingExplain();
	
	bool IsFarmingStatus();
	
	void Reset();					// 초기화 시킨다.
	
private:
	bool m_bShow;
	
	bool m_bAutoFarming;

	SI32 m_siFarmingItemUnique;
	SI32 m_siDelayTime;
	
	SI32 m_siFarmingItemNum;
	SI32 *m_pFarmingItemUniqueListPtr;
	
	HBRUSH m_hEditBrush;
};

#endif
*/