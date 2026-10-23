/*
#ifndef _LETTERDLG_H_
#define _LETTERDLG_H_

#include "LetterDlgBaseInfo.h"

class CLetterMgr;
class CObj;

class CLetterDlg
{
public:
	CLetterDlg();
	~CLetterDlg();

	void	Initialize();
	void	Destroy();

	void	Create( HWND hWnd, HINSTANCE hInst );
	
	static	BOOL CALLBACK StaticLetterDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK LetterDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	//void	DeleteLetter();

	void	Show();
	void	Hide();

	bool	IsShow();

	bool	Push( TCHAR *strLetterMsg );	

private:
	CLetterMgr	*m_pLetterMgr;
	HWND		m_hDlg;

	bool		m_bShow;
	CObj		*m_pCurrentLetter;
	HBRUSH		m_hAnswerEditBrush;
};

#endif
*/