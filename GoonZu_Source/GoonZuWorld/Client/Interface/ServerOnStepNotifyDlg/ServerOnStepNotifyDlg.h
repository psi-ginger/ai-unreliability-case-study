#ifndef _SERVERONSTEPNOTIFYDLG_H_
#define _SERVERONSTEPNOTIFYDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>
#include "TSpr.h"

class CServerOnStepNotifyDlg
{
public:
	CServerOnStepNotifyDlg();
	~CServerOnStepNotifyDlg();

	void Create( HINSTANCE hInst, HWND hWnd, SI16 GameMode, SI16 GameScreenMode, SI16 siServiceArea );
	
	static	BOOL CALLBACK StaticServerOnStepNotifyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK ServerOnStepNotifyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	
	void	LoadSpr(SI32 siServiceArea);

	bool IsShow();
	void Show();
	void Hide();

	void Set( SI32 Percent );

	void Draw();

private:
	HWND		m_hDlg;
	HWND		m_hParentWnd;
	bool		m_bShow;
	
	HBITMAP		m_hLoadingImage;
	HINSTANCE	m_hInst;

	HWND		m_hProgress;

	SI16		m_siGameMode;
	
	HBRUSH		m_hEditBrush;

	SI32		m_siPercent;
	SI16        m_siGameScreenMode;

	TSpr		m_BKSpr;
	TSpr		m_ProgressSpr;
		
	SI32 BackGoundXPos;
	SI32 BackGoundYPos;
	SI32 ProgressXPos;
	SI32 ProgressyPos;


};

extern	CServerOnStepNotifyDlg g_ServerOnStepNotifyDlg;

#endif