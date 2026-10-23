//#ifndef _SEARCHDETAILDLG_H_
//#define _SEARCHDETAILDLG_H_
//
//#define BULLETINBOARD_KNOWLEDGE			1		// 지식마당 
//#define BULLETINBOARD_SARANGBANG		2		// 사랑방 
//#define BULLETINBOARD_HYUNGZO			3		// 포도청
//#define BULLETINBOARD_VILLAGERESIDENT	4		// 마을 게시판 
//#define BULLETINBOARD_BROADCAST			5		// 방송게시판
//#define BULLETINBOARD_MARKET			6		// 객주게시판
//
//class cltToolBarInfo;
//class CSearchDetailDlg  
//{
//public:
//	CSearchDetailDlg();
//	virtual ~CSearchDetailDlg();
//
//	static LRESULT CALLBACK	StaticSearchDetailDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	static LRESULT CALLBACK StaticBrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	LRESULT CALLBACK			BrowserWindowProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	LRESULT CALLBACK			SearchDetailDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//
//
//	void SetToolBar(SI32 index, SI32 style, SI32 bitmap, SI32 idcommand, TCHAR *Tooltip);
//	void Create( HINSTANCE hInst, HWND hParent );
//	void Show(SI32 siBulletinBoardType, TCHAR *pKeyWord,SI32 siVillage = 0);
//	void GoURL(SI32 siBoardType, TCHAR *pKeyword,SI32 siVillage = 0);
//	
//
//private:
//	HINSTANCE m_hInst;
//	HWND m_hWnd;
//	
//	HWND m_hDlg;
//	HWND m_hDlgChild;
//	TCHAR m_Keyword[MAX_PATH];
//
//	HWND m_hToolBar;				// 툴바의 핸들 
//	BOOL m_bShow;
//	cltToolBarInfo* m_pToolBarGameInfo;
//	SI32			m_siToolBarNumber;
//
//	SI32 m_siBulletinBoardType;
//};
//
//#endif 
