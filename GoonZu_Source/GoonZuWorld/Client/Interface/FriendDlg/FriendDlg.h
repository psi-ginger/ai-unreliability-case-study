#ifndef _FRIENDDLG_H_
#define _FRIENDDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>
#include <commctrl.h>

#include "../../Client/client.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"

class cltFriendInfo;


// 리스트 컨트롤을 사용해야 할듯
// 앞에 접속 여부를 체크박스 넣으면 편리하고..
// 세로 줄로 나눠져 있어서 보기도 좋을듯.
class CFriendDlg
{
public:
	HWND m_hDlg;

	CFriendDlg();
	~CFriendDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticFriendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK FriendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

	bool IsChanged();
	void LoadFriendList();

	void FriendAddList( SI16 ListIndex, SI32 PersonID, SI32 CharId, char *FriendName ,char *AccountID);
	void FriendDelList( SI16 ListIndex, SI32 PersonID );
	
	void FriendLogin( SI32 PersonID, SI32 CharID ,char * AccountID);
	void FriendLogOut( SI32 PersonID );

private:

	bool m_bShow;
	HWND m_hFriendListCtrl;

	HWND m_hNameInputEdit;

	HIMAGELIST	m_hLogInOutImg;
	
	HBITMAP m_hFriendWarpBtnBitmap;

	cltFriendInfo *m_pFriendInfo;
	
	char m_szAccountID[MAX_FRIEND_NUMBER][MAX_PLAYER_NAME];
};

#endif