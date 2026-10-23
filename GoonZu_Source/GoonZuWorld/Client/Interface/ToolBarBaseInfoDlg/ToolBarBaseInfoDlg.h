#ifndef _TOOLBARBASEINFODLG_H_
#define _TOOLBARBASEINFODLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>

class CToolBarBaseInfoDlg
{
public:
	CToolBarBaseInfoDlg();
	~CToolBarBaseInfoDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticToolBaseInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK ToolBaseInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void	SetDate( TCHAR *strDate );

private:
	HWND m_hDlg;
};


extern CToolBarBaseInfoDlg g_ToolBarBaseInfoDlg;

#endif