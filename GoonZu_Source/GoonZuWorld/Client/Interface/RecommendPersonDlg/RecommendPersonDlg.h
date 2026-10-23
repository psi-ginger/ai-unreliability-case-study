#ifndef _RECOMMENDPERSONDLG_H_
#define _RECOMMENDPERSONDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

class CRecommendPersonDlg
{
public:
	HWND m_hDlg;

	CRecommendPersonDlg();
	~CRecommendPersonDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticRecommendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK RecommendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticOKButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OKButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK StaticCancelButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CancelButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool IsShow();
	void Show();
	void Hide();

private:
	bool m_bShow;

	HBITMAP	m_hBankGroundBitmap;
	HBITMAP m_hButtonBitmap;

	HWND		m_hOKButton;
	HWND		m_hCancelButton;

	WNDPROC		m_OldOKButtonProc;
	WNDPROC		m_OldCancelButtonProc;

};

#endif