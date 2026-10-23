// WennyOldMan.h: interface for the CWennyOldMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WENNYOLDMAN_H__51287E12_AB9F_41F3_93CD_2ED1ABF21EB0__INCLUDED_)
#define AFX_WENNYOLDMAN_H__51287E12_AB9F_41F3_93CD_2ED1ABF21EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../../Resource.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"

class CItemListView;

class CWennyOldMan  
{
public:
	CWennyOldMan();
	virtual ~CWennyOldMan();

	static BOOL CALLBACK	StaticWennyOldManProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			WennyOldManProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StaticWennyOldManUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			WennyOldManUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	
	static BOOL CALLBACK	StaticWennyOldManPerformanceUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			WennyOldManPerformanceUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();

	void Hide();
	void Show();
	void LoadUpgradeItemData();
	void LoadPerformanceUpgradeItemData();

	BOOL IsChangedInvItem();

	void UpgradeItemSuccess( bool Success );
	void PerformanceUpgradeItemSuccess();

private:
	HWND m_hDlg;
	HWND m_hUpgradeItemDlg;
	HWND m_hPerformanceUpgradeItemDlg;

	HWND m_hCurrentDlg;

	HWND m_hEdit;

	HWND m_hTabCtrl;

	HBITMAP			m_hBitmap;
	HBITMAP			m_hHelpBtnBitmap;

	BOOL m_bRectSwitch;
	BOOL m_bShow;

	RECT m_rtDlg;

	cltItem			*m_pInvItem;

	SI32			m_siSelectIndex;

	SI32			m_siVillage;

};

#endif // !defined(AFX_WENNYOLDMAN_H__51287E12_AB9F_41F3_93CD_2ED1ABF21EB0__INCLUDED_)
