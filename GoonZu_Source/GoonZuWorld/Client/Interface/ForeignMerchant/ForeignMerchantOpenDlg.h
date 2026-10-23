#ifndef _FOREIGNMERCHANTOEPNDLG_H_
#define _FOREIGNMERCHANTOEPNDLG_H_

#include <Directives.h>
#include <windows.h>

class CForeignMerchantOpenDlg
{
public:
	HWND m_hDlg;
	
	CForeignMerchantOpenDlg();
	~CForeignMerchantOpenDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticForeignMerchantOpenDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK ForeignMerchantOpenDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show( SI32 npccharunique );
	void Hide();

private:
	bool m_bShow;

	SI32 m_siNPCCharUnique;

	HBITMAP m_hPortraitBitmap;

};


#endif