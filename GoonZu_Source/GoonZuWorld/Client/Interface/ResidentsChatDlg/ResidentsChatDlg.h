#ifndef _RESIDENTSCHATDLG_H_
#define _RESIDENTSCHATDLG_H_

#include "ResidentsChatDlgBaseInfo.h"

class	CResidentsChatDataMgr;

class	CResidentsChatDlg
{
public:
	CResidentsChatDlg();
	~CResidentsChatDlg();

	void	Create( HINSTANCE hInst, HWND hWnd );

	void	Initialize();
	void	Destroy();

	static	LRESULT CALLBACK StaticEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticResidentsChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL	CALLBACK ResidentsChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void	LoadData();

	void	Show();											// 다이얼로그 보여 주기	
	void	Hide();											// 다이얼로그 숨기기
	bool	IsShow();

	void	Push();
	void	Push( char *strUserID, char *strChatRecvData );
	bool	Pop( stChatStatusBarData * pChatStatusBarData );

private:

	HINSTANCE					m_hInst;
	HWND						m_hDlg;

	HWND						m_hChatDataViewEdit;
	HWND						m_hChatDataInputEdit;

	bool						m_bShowFlag;

	char						m_strChatData[ MAX_CHAT_DATA_LENGTH ];

	CResidentsChatDataMgr		*m_pChatDataViewMgr;
	CResidentsChatDataMgr		*m_pChatDataSendMgr;

	char						*m_strChatViewData;

	WNDPROC						m_OldEditProc;

	HBRUSH						m_hChatViewBrush;
	HBRUSH						m_hChatInputBrush;

};

#endif