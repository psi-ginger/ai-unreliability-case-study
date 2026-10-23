#ifndef _SYSTEMNPCINFODLG_H_
#define _SYSTEMNPCINFODLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>

class CSystemNPCInfoDlg
{
public:
	CSystemNPCInfoDlg();
	~CSystemNPCInfoDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticSystemNPCInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK SystemNPCInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void SetBaseInfo( char *text );

	void SetRegenPos( char *text );
	void SetRegenPos( UI08 mapindex [] , SI16 maxmap );
	void AddRegenPos( char *text );

	bool IsShow();
	void Show();
	void Hide();

private:
	HWND m_hDlg;
	bool m_bShow;

	bool m_bComboBoxInit;

	SI16 m_siSelectedIndex;
	SI32 m_iSelectVillageInx;

	cltTSpr *m_pBackGroundSpr;

	HBITMAP m_hPortraitBitmap;
	HBITMAP m_hSearchDetailBitmap;
	HBRUSH	m_hEditBrush;
	
	HBITMAP m_hWarpBitmap ;

	char m_strBaseInfo[ 512 ];

};

extern CSystemNPCInfoDlg g_SystemNPCInfoDlg;



#endif