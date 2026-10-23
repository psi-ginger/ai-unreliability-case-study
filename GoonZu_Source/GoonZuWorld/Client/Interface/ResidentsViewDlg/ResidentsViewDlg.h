#ifndef _RESIDENTSVIEWDLG_H_
#define _RESIDENTSVIEWDLG_H_

#include <RichEdit.h>
#include "../RichEditChatMgrDlg/RichEditChatMgrDlg.h"

class cltVillageResidentsList;

class CResidentsViewDlg
{
public:
	CResidentsViewDlg();
	~CResidentsViewDlg();

	void Init();
	void Create( HINSTANCE hInst, HWND hParent );

	static	LRESULT CALLBACK StaticEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	
	static BOOL CALLBACK	StaticResidentsViewDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			ResidentsViewDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool IsShow();

	void Show();
	void Hide();

	void RecvResidentsList( cltVillageResidentsList *pclPersonList, UI32 ListCount );

	void LoadResidentsChat();

	void SetFont( COLORREF Color, DWORD dwMask = CFM_COLOR|CFM_SIZE|CFM_BOLD|CFM_ITALIC|CFM_FACE, DWORD dwEffects=~(CFE_AUTOCOLOR), char *FontKind="±¼¸²", SI16 YHeight=RICHEDIT_FONT_HEIGHT );

	void AddChat( char *PlayerName, char *ChatText, COLORREF NameColor, COLORREF TextColor );
	void AddName( char *PlayerName, COLORREF Color );
	void AddText( char *ChatText, COLORREF Color );


	HWND m_hDlg;
	bool m_bShow;

private:
	cltVillageResidentsList *m_pVillageResidentsList;

	HWND m_hResidentsViewListCtrl;
	
	SI32 m_siSelectedIndex;

	HMODULE	m_hRichEditModuleLoad;
	HWND m_hRichEdit;

	char *m_strChatData;
	char *m_strViewData;
	
	WNDPROC m_OldEditProc;
	HWND m_hChatDataInputEdit;
};


#endif