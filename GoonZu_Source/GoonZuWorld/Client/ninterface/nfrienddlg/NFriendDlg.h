/* ==========================================================================
클래스 :		CNFriendDlg

작성일 :		05/03/05
작성자 :		정용래

목  적 :		CNFriendDlg 클래스는  전체 화면 모드 실행시 NInterface에
따라 게임 화면 안에서 친구 목록 및 관리 다이얼로그를 생성한다.

용  법 :		

변경사항 :		없음

차후작업 :		cpp 참고

========================================================================*/
//#if (defined(__LEON_START_ALL) || defined(__LEON_NFRIENDDLG__)) && defined(_DEBUG)

#pragma once

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <commctrl.h>

#include "../../Client/client.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Item/ItemCommon/ItemUnit.h"


class cltFriendInfo;

class CList;
class CStatic;
class CEdit;
class CButton;
class CImageStatic;

// 리스트 컨트롤을 사용해야 할듯
// 앞에 접속 여부를 체크박스 넣으면 편리하고..
// 세로 줄로 나눠져 있어서 보기도 좋을듯.
class CNFriendDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 80,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 80,
	};

	CNFriendDlg();
	~CNFriendDlg();

	void Initialize();
	void Destroy();

	void Create( );

	static	void CALLBACK StaticNFriendDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NFriendDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	bool IsShow();
	void Show();
	void Hide();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	bool IsChanged();
	void LoadFriendList();

	void FriendAddList( SI16 ListIndex, SI32 PersonID, SI32 CharId, TCHAR *FriendName ,TCHAR *AccountID);
	void FriendDelList( SI16 ListIndex, SI32 PersonID );

	void FriendLogin( SI32 PersonID, SI32 CharID ,TCHAR * AccountID);
	void FriendLogOut( SI32 PersonID );

	void ShowFriendMemo();

	void Action();

	CList			*m_pFriendList;
	CStatic			*m_pFriendNameStatic;
	CEdit			*m_pFriendNameEdit;
	CButton			*m_pFriendDelButton;
	CButton			*m_pFriendWhisperChatButton;
	CButton			*m_pFriendAddButton;
	CButton			*m_pFriendMinhomeButton;
	CButton			*m_pFriendWarpButton;
	CImageStatic	*m_pFriendProfileImage;
	CStatic			*m_pFriendIntroStatic;
	CButton			*m_pFriendMemoButton;

	cltFriendInfo *m_pFriendInfo;

	TCHAR m_szAccountID[MAX_FRIEND_NUMBER][MAX_PLAYER_NAME];

private:

	bool m_bShow;


};

//#endif
