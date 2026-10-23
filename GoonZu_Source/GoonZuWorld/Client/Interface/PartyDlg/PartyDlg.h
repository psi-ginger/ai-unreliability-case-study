#ifndef _PARTYDLG_H_
#define _PARTYDLG_H_

#include "PartyDlgBaseInfo.h"
#include "..\..\..\Common\PartyMgr\PartyObj.h"
#include "..\..\..\Common\CommonHeader.h"
#include "../../interface/RichEditChatMgrDlg/RichEditChatMgrDlgBaseInfo.h"

#include <RichEdit.h>

#define RICHEDIT_FONT_HEIGHT		180

class CRichEditChatDataMgr;
class CLibListMgr;

struct stPartyChatSend
{	
	char strChatData[ MAX_CHAT_DATA_LENGTH ];

	stPartyChatSend()
	{
		memset( this, 0, sizeof( stPartyChatSend ) );
	}

	stPartyChatSend( char *ChatData )
	{
		MStrCpy( strChatData, ChatData, MAX_CHAT_DATA_LENGTH );
	}
};

struct stClientPartyUser
{
	bool bPartyStatus;						// 파티에 참여한 유저냐, 아니면 빈 리스트의 유저이냐를 구분짓기 위해서...
	bool bIsPartyLeader;					// 파티장이냐, 파티원이냐	
	SI32 siCharUnique;						// 캐릭터 unique 값
	char strCharName[ MAX_PLAYER_NAME ];	// 캐릭터 명
	SI32 siExp;								// 획득 경험치
	SI32 siLevel;							// 레벨

	void Init()
	{
		bPartyStatus = false;
		bIsPartyLeader = false;
		siCharUnique = 0;		
		memset( strCharName, 0, MAX_PLAYER_NAME );
		siExp = 0;
		siLevel = 0;
	}

	void Set( stClientPartyUser *pClientPartyUser )
	{
		memcpy ( this, pClientPartyUser, sizeof( stClientPartyUser ) );
	}
};

class CPartyDlg
{
public:
	CPartyDlg();
	~CPartyDlg();

	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticPartyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PartyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	BOOL CALLBACK StaticRichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK RichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool IsShow();
	void Show();
	void Hide();

	bool IsPartyStatus();
	bool IsPartyLeader();
	bool IsPartyUser( SI32 CharUnique );

	SI32 GetPartyIndex();

	//void PartyCreate( SI32 PartyIndex );
	void PartyCreateJoin( SI32 PartyIndex, SI32 LeaderPersonID, SI32 LeaderCharUnique, char *strLeaderChatName, SI32 LeaderLevel, SI32 MemberPersonID, SI32 MemberCharUnique, char *strMemberCharName, SI32 MemberLevel );	
	void PartyJoin( SI32 PartyIndex, SI16 UserListIndex );
	void PartyDelete( SI32 PartyIndex );
	void PartyAddUser( bool PartyLeader, SI32 CharUnique, char *CharName, SI16 UserListIndex, SI32 Level );
	void PartyDelUser( SI32 CharUnique, SI16 UserListIndex );
	void PartyForceDelUser( SI32 CharUnique, SI16 UserListIndex );
	void PartyChangeLeader( SI32 CharUnique, SI16 UserListIndex );
	void PartyNotifyExp( SI32 CharUnique, SI16 UserListIndex, SI32 Exp );

	void AddChat( char *strUserID, char *strChatData, COLORREF NameColor, COLORREF TextColor );
	void AddChat( char *strChatData, COLORREF NameColor, COLORREF TextColor );
	void AddName( char *PlayerName, COLORREF Color );
	void AddText( char *ChatText, COLORREF Color );
	void SetFont( COLORREF Color, DWORD dwMask = CFM_COLOR|CFM_SIZE|CFM_BOLD|CFM_ITALIC|CFM_FACE, DWORD dwEffects=~(CFE_AUTOCOLOR), char *FontKind="굴림", SI16 YHeight=RICHEDIT_FONT_HEIGHT );
	void SendPartyChat();

	SI32 FindUserSelectedIndex( char *CharName );

	void PrintUserNum();
	
	void ReloadPartyUserList();

	bool PopPartyChatSendData( stPartyChatSend *pPartyChatSend );

private:
	HWND m_hDlg;
	bool m_bShow;

	bool m_bPartyStatus;			// 파티중이냐, 파티중이 아니냐
	bool m_bIsPartyLeader;			// 파티장이냐, 파티원이냐

	SI32 m_siPartyIndex;	

	stClientPartyUser m_stClientPartyUser[ MAX_PARTY_USER_NUM ];
	SI16 m_siPartyUserNum;

	SI32 m_siSelectedUserListIndex;

	HMODULE	m_hRichEditModuleLoad;
	HWND m_hRichEdit;
	HWND m_hRichEditDlg;
	HWND m_hChatDataInputEdit;
	WNDPROC m_OldEditProc;
	HINSTANCE	m_hInst;

	char *m_strViewData;
	char *m_strChatData;
	
	CLibListMgr *m_pChatDataList;
};

#endif