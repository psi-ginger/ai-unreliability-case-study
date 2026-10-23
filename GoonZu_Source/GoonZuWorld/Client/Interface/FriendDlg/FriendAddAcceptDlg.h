#ifndef _FRIENDADDACCEPTDLG_H_
#define _FRIENDADDACCEPTDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

#include "../../Client/client.h"

class CLibListMgr;

struct stAcceptObj
{
	SI32 siPersonID;
	SI32 siCharID;
	char strCharName[MAX_PLAYER_NAME];
	
	stAcceptObj()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( stAcceptObj ) );
	}

	void Set( SI32 PersonID, SI32 CharID, char *CharName )
	{
		siPersonID = PersonID;
		siCharID = CharID;

		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
	}
};

class CFriendAddAcceptDlg
{
public:
	HWND m_hDlg;

	CFriendAddAcceptDlg();
	~CFriendAddAcceptDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticFriendAddAcceptDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK FriendAddAcceptDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();


	void PushAccpetList( SI32 PersonID, SI32 CharID, char *CharName );
	void PopAcceptList();

private:	
	bool m_bShow;
	CLibListMgr *m_pAcceptListMgr;

	SI32 m_siPersonID;
	SI32 m_siCharID;

	HBRUSH	m_hEditBrush;
	
	char m_strCharName[ MAX_PLAYER_NAME ];
	
};

#endif