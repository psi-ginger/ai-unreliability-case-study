//#ifndef _CHATDLG_H_
//#define _CHATDLG_H_
//
//#include "ChatDlgBaseInfo.h"
//
//class	CChatDataMgr;
//
//class	CChatDlg
//{
//public:
//	CChatDlg();
//	~CChatDlg();
//
//	void	Create( HINSTANCE hInst, HWND hWnd );
//
//	void	Initialize();
//	void	Destroy();
//
//	static	LRESULT CALLBACK StaticEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	LRESULT CALLBACK EditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//
//	static	BOOL CALLBACK StaticChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	BOOL	CALLBACK ChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//
//	void	LoadData();
//
//	void	Show();											// 다이얼로그 보여 주기	
//	void	Hide();											// 다이얼로그 숨기기
//	bool	IsShow();
//
//	void	Push();
//	void	Push( TCHAR *strUserID, TCHAR *strChatRecvData );
//	bool	Pop( stChatStatusBarData * pChatStatusBarData );
//
//private:
//
//	HINSTANCE					m_hInst;
//	HWND						m_hDlg;
//
//	HWND						m_hChatDataViewEdit;
//	HWND						m_hChatDataInputEdit;
//
//	bool						m_bShowFlag;
//
//	TCHAR						m_strChatData[ MAX_CHAT_DATA_LENGTH ];
//
//	CChatDataMgr				*m_pChatDataViewMgr;
//	CChatDataMgr				*m_pChatDataSendMgr;
//
//	TCHAR						*m_strChatViewData;
//
//	WNDPROC						m_OldEditProc;
//
//	HBRUSH						m_hChatViewBrush;
//	HBRUSH						m_hChatInputBrush;
//
//};
//
//#endif
