#ifndef _PRIVATECHATDLG_H_
#define _PRIVATECHATDLG_H_

#include "PrivateChatBaseInfo.h"

class CChatDataMgr;
class CPrivateChatDlgMgr;

class CPrivateChatRejectViewDlg
{
public:
	CPrivateChatRejectViewDlg();
	~CPrivateChatRejectViewDlg();

	void Create( HWND hWnd, HINSTANCE hInst, CPrivateChatDlgMgr *pPrivateChatDlgMgr );
	static BOOL CALLBACK StaticPrivateChatRejectViewDlg(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK PrivateChatRejectViewDlg(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool IsShow();
	
	void Show();
	void Hide();

	void LoadRejectList();

private:
	HWND m_hDlg;
	bool m_bShow;

	HWND m_hRejectUserList;
	HWND m_hUserName;

	CPrivateChatDlgMgr	*m_pPrivateChatDlgMgr;
};

class CPrivateChatDlg
{

public:

	CPrivateChatDlg();
	~CPrivateChatDlg();

	void	Initialize();
	void	Destroy();

	BOOL	SetCtlColorStatic(WPARAM wParam);
	BOOL	SetCtlColorDlg(WPARAM wParam);
	void	Create( HWND hWnd, HINSTANCE hInst, CPrivateChatDlgMgr *pPrivateChatDlgMgr, SI32 siPrivateChatDlgIndex );

	void	Set( SI32 siCharUnique, char *strUserName,SI32 level );	

	bool	IsExist( SI32 siCharUnique, char *strUserName );

	SI32	m_siPrivateChatDlgIndex;

	static	LRESULT CALLBACK StaticEditSubProc0(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc0(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc0( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc0( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc1( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc1( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc2( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc2( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc3(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc3(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc3( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc3( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc4(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc4(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc4( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc4( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc5(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc5(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc5( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc5( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc6(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc6(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc6( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc6( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc7(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc7(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc7( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc7( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc8(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc8(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc8( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc8( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	LRESULT CALLBACK StaticEditSubProc9(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc9(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticPrivateChatDlgProc9( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PrivateChatDlgProc9( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void	Send();
	void	Recv( char *strUserID, char *strChatRecvData );

	bool	IsShow();
	
	void	Show();
	void	Hide();


	bool	Pop ( stWhisperChatDataSend *pWhisperChatDataSend );

	void	LoadData();

	void	Reject();				// 수신 거부

	CChatDataMgr		*m_pChatDataSendMgr;


	char				m_strUserName[ MAX_PLAYER_NAME ];

	HWND				m_hDlg[ MAX_PRIVATE_CHAT_DLG_NUM ];
	bool				m_bActive[ MAX_PRIVATE_CHAT_DLG_NUM ];
	bool				m_bTimerStatus[ MAX_PRIVATE_CHAT_DLG_NUM ];

private:
	
	

	bool				m_bShow;
	char				m_strChatData[ MAX_CHAT_DATA_LENGTH ];
	

	CChatDataMgr		*m_pChatDataViewMgr;
	
	HWND				m_hChatDataViewEdit[ MAX_PRIVATE_CHAT_DLG_NUM ];
	HWND				m_hChatDataInputEdit[ MAX_PRIVATE_CHAT_DLG_NUM ];

	
	SI32				m_siCharUnique;

	CPrivateChatDlgMgr	*m_pPrivateChatDlgMgr;

	char				*m_strChatViewData;

	WNDPROC				m_OldEditProc[ MAX_PRIVATE_CHAT_DLG_NUM ];	 // 원래는 배열형태가 아니라, m_OldEditProc 이렇게 하나만 있었는데... 이상하게 10개의 다이얼로그가 중복해서 사용해서리... 이것을 그냥 열개로 잡았음. 실제로 각 객체당 9개는 메모리만 잡고 있지, 필요없는 놈...

	HBRUSH				m_hAnswerEditBrush;
	HBRUSH				m_hAnswerDlgBrush;

	HBITMAP				m_bBitmap[10][10];
	
	void BitMapLoad(HWND hDlg,SI32 num);

};

#endif