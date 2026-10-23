#ifndef _PERSONALSHOPSEARCHDLG_H_
#define _PERSONALSHOPSEARCHDLG_H_

#define PERSONALSHOPSEARCHDLG_SETTIMER_TIME 300

class CPersonalShopSearchDlg
{
public:
	CPersonalShopSearchDlg();
	~CPersonalShopSearchDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticPersonalShopSearchDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PersonalShopSearchDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	
	bool IsShow();
	void Show();
	void Hide();

	bool IsShopShowCaseChanged();
	void SortSearchedData();
	void LoadShopShwocase();
	void DisplaySowCaseItemInfo(SI32 index);
	void SearchShopItem();
	
public:
	bool			m_bShow;
	bool			m_bTimer;


	HWND			m_hDlg;
	HWND			m_hUserShopShowcase;
	HBRUSH			m_hEditBrush;
	HBITMAP			m_hWarpBitmap;
	
	BOOL			m_bShopDataChanged;

	cltSearchShopData	m_clSearhShopData;

	char			m_szAccountID[MAX_PLAYER_NAME];
	SI32			m_siShopShowCaseSelectedIndex;

	clock_t			m_LastSearchTime; 
};

extern CPersonalShopSearchDlg g_PersonalShopSearchDlg;
#endif