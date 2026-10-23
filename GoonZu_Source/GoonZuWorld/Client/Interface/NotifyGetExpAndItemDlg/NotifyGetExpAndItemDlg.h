#ifndef _NOTIFYGETEXPANDITEMDLG_H_
#define _NOTIFYGETEXPANDITEMDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

class cltItem;

class CNotifyGetExpAndItemDlg
{
public:
	HWND m_hDlg;
	
	CNotifyGetExpAndItemDlg();
	~CNotifyGetExpAndItemDlg();
	
	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticNotifyGetExpAndItem( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK NotifyGetExpAndItem( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void Show( cltItem *pclItem, SI32 Exp );
	void Hide();
	
private:
	bool m_bShow;

	HWND m_hImageWnd;
	
	SI16 m_siCount;
	
};

#endif
