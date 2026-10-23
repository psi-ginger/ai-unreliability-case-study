#ifndef _GOMZIDLG_H_
#define _GOMZIDLG_H_

#include <windows.h>

class CGomZiDlg
{
public:
	CGomZiDlg();
	~CGomZiDlg();

	void Init();

	void Create( HINSTANCE hInst, HWND hParent );

	static LRESULT CALLBACK StaticBrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK BrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK StaticGomZiDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK GomZiDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool IsShow();

	void Show();
	void Hide();
	
	void Set( char *URL );

	HWND m_hDlg;
	HWND m_hDlgChild;

private:
	bool m_bShow;


};

#endif