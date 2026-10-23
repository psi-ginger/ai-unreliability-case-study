#ifndef _RIGHEDITCHATMGRDLG_H_
#define _RIGHEDITCHATMGRDLG_H_

#include "RichEditChatMgrDlgBaseInfo.h"

#include <RichEdit.h>

#define RICHEDIT_FONT_HEIGHT		180

class CRichEditChatDataMgr;
class CChatStatusBarBuffer;

class CRichEditChatMgrDlg
{
public:
	HWND m_hTab;
	HWND m_hDlg;
	HWND m_hRichEditDlg;

	CRichEditChatMgrDlg();
	~CRichEditChatMgrDlg();

	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	LRESULT CALLBACK StaticEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticRichEditChatMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK RichEditChatMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	BOOL CALLBACK StaticRichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK RichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

	void CharacterLogin();
	void SetFont( COLORREF Color, DWORD dwMask = CFM_COLOR|CFM_SIZE|CFM_BOLD|CFM_ITALIC|CFM_FACE, DWORD dwEffects=~(CFE_AUTOCOLOR), char *FontKind="±¼¸²", SI16 YHeight=RICHEDIT_FONT_HEIGHT );

	bool CheckData( char *PlayerName, char *ChatText );

	void ChatStore( char *PlayerName, char *ChatText, SI16 kind = 0 );
	void ChatRecv( char *PlayerName, char *ChatText, SI16 kind = 0 );
	SI32 CompareKey(char *data, SI32 type);
	void ResidentsChatRecv( char *PlayerName, char *ChatText );
	void GuildChatRecv( char *PlayerName, char *ChatText );
	void PartyChatRecv( char *PlayerName, char *ChatText );
    void MasterNotifyChatRecv( char *PlayerName, char *ChatText );
	void VillageNotifyChatRecv( char *PlayerName, char *ChatText );
	void ResidentsNotifyChatRecv( char *PlayerName, char *ChatText );
	void KingNotifyChatRecv( char *PlayerName, char *ChatText );
	void GoodsProductionNotifyChatRecv( char *PlayerName, char *ChatText );

	void AddChat( char *PlayerName, char *ChatText, COLORREF NameColor, COLORREF TextColor );
	void AddName( char *PlayerName, COLORREF Color );
	void AddText( char *ChatText, COLORREF Color );
	
	void LoadChat();	
	void LoadResidentsChat();
	void LoadGuildChat();
	void LoadPartyChat();
	
	void Push();
	void Push( SI16 SelectedIndex, char *ChatData );
	bool Pop( stRichEditChatDataSend *pSendData );

	CRichEditChatDataMgr *GetRichEditChatDataRecvMgrPtr();
	SI16 m_siCurrentTabMode;


public:
	CHARFORMAT cf;
	HMODULE	m_hRichEditModuleLoad;

	HINSTANCE	m_hInst;

	bool m_bShow;
	HWND m_hRichEdit;

	HWND m_hTabCtrl;
	HWND m_hCurrentTabDlg;

	char *m_strViewData;

	

	char *m_strChatData;
	
	WNDPROC m_OldEditProc;
	HWND m_hChatDataInputEdit;

	CRichEditChatDataMgr *m_pRichEditChatDataRecvMgr;
	CRichEditChatDataMgr *m_pRichEditChatDataSendMgr;
	CChatStatusBarBuffer	*m_pChatStatusBarBuffer;
};

#endif