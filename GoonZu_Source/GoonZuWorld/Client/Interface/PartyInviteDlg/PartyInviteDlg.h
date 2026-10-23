#ifndef _PARTYINVITEDLG_H_
#define _PARTYINVITEDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

#include "..\..\..\Common\PartyMgr\PartyBaseInfo.h"

class CPartyInviteDlg
{
public:
	CPartyInviteDlg();
	~CPartyInviteDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticPartyInviteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PartyInviteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	
	bool IsShow();
	void Show();
	void Hide();

	void PartyCreateInvite( SI32 LeaderPersonID, char *strLeaderCharName );
	void PartyInvite( char *CharName, SI32 PartyLeaderPersonID, SI32 PartyIndex );

private:
	HWND m_hDlg;
	bool m_bShow;
	SI32 m_siPartyIndex;
	SI32 m_siPartyLeaderPeronID;
};

extern CPartyInviteDlg g_PartyInviteDlg;

#endif