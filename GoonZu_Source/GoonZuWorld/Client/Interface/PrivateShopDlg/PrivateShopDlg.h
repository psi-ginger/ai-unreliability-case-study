#ifndef _PRIVATESHOPDLG_H_
#define _PRIVATESHOPDLG_H_

#include "PrivateShopBaseInfo.h"

class CPrivateShopDlg
{
public:
	CPrivateShopDlg();
	~CPrivateShopDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticPrivateShopDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateShopDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

private:

	HWND m_hDlg;
	bool m_bShow;


};

#endif