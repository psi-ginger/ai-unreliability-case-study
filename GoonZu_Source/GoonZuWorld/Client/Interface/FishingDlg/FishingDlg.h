/*
#ifndef _FISHINGDLG_H_
#define _FISHINGDLG_H_

#include <Directives.h>
#include <windows.h>

class DBToolInfo;

class CFishingDlg
{
public:
	HWND m_hDlg;
	
	CFishingDlg();
	~CFishingDlg();

	void Init();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticFishingtDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK FishingtDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

	void Set( SI32 FishingBaitItemUnique, DBToolInfo *pclToolInfo );

	void SetFishingExplain();
	void SetFishingPlaceInfoExplain();

	void SetAutoFishing( bool AutoFishing );

private:	
	bool m_bShow;
	
	bool m_bAutoFishing;

	SI32 m_siFishingBaitItemUnique;
	DBToolInfo *m_pToolInfo;
	
	HBRUSH m_hEditBrush;
};


#endif
*/