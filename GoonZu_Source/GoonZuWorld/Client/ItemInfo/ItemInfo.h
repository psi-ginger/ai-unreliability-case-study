//---------------------------------
// 2003/10/1 ±Ë≈¬∞Ô
//---------------------------------
#ifndef _ITEMINFO_H
#define _ITEMINFO_H

#include <WIndows.h>
#include <Directives.h>
#include <CommCtrl.h>

class cltItemDetailInfo{
public:
	HWND m_hDlg;					// æ∆¿Ã≈€ ¡§∫∏. 

	RECT DlgRect;
	BOOL DlgRectInitSwitch;

	HBITMAP hItemImage;
	HBITMAP m_hSearchDetailBitmap;
	
	HBRUSH	m_hEditBrush;
	HWND	m_hComboListCtrl;
	HWND	m_hMonsterListCtrl;
	HWND	m_hMakeItemListCtrl;
	HIMAGELIST	m_hImgSM;

	bool	m_bShow;

	
	cltItemDetailInfo();
	~cltItemDetailInfo();
	
	BOOL Create(HINSTANCE hinst, HWND hwnd);

	static BOOL CALLBACK StaticItemInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK ItemInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	void ShowItemInfo(char * pStrname);
	void SetItemInfo(HWND hDlg, char* szSelText);

	void Hide();
	void Show();

	BOOL IsShow()	{ return m_bShow; }
	
};


#endif
