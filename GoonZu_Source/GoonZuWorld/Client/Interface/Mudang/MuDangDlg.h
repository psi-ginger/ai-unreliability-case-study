#ifndef _MUDANGDLG_H_
#define _MUDANGDLG_H_

#define MUDANGDLG_SETTIMER_TIME 300
#define MAX_MUDANG_PAGE_NUMBER	3
class CMuDangDlg
{
public:
	CMuDangDlg();
	~CMuDangDlg();

	void Create( HINSTANCE hInst, HWND hWnd );
	void Set();

	BOOL CreateTabDlg(HWND hDlg, SI32 page, SI32 dlg, DLGPROC proc);
	
	static	BOOL CALLBACK StaticMuDangDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	static	BOOL CALLBACK MuDangReviveStaticProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	static	BOOL CALLBACK MuDangScrollStaticProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	static	BOOL CALLBACK MuDangEvolveStaticProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	

	BOOL CALLBACK MuDangDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK MuDangDlgReviveProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK MuDangDlgScrollProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK MuDangDlgEvolveProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	BOOL GetComboSummonNameIndex(HWND hDlg, char *CurrSummonName, SI32 *siSummonIndex);
	
	void UpdateCurrentSummonInfo(HWND hDlg, SI32 Item, BOOL bALive);

	// Evolve, Devolution, Transform
	void UpdateEvolveSummonInfo(HWND hDlg, SI32 Item);
	void UpdateDevolutionSummonInfo(HWND hDlg, SI32 Item);
	void UpdateTransformSummonInfo(HWND hDlg, SI32 Item);

	void UpdateScrollSummonInfo(HWND hDlg);

	BOOL SendSummonReviveMessageToServer(HWND hDlg, SI32 id);
	BOOL SendSummonScrollMessageToServer(HWND hDlg, SI32 id);
	// Evolve, Devolution, Transform
	BOOL SendSummonEvolveMessageToServer(HWND hDlg, SI32 id);
	BOOL SendSummonDevolutionMessageToServer(HWND hDlg, SI32 id);
	BOOL SendSummonTransformMessageToServer(HWND hDlg, SI32 id);
	
	BOOL DisplayMessage(BOOL bSuccess, char *pStr);

	void UpdateReviveMent(HWND hDlg);
	void UpdateScrollMent(HWND hDlg);
	void UpdateEvolveMent(HWND hDlg);

public:
	HINSTANCE		m_hInst;
	HWND			m_hWnd;

	HWND			m_hDlg;
	HBRUSH			m_hEditBrush;

	RECT			m_DlgRect;
	bool			m_bRect;

	HBITMAP			m_hPortraitBitmap;
	
	BOOL			m_bChangedScrollPercent;
	SI32			m_siActiveScrollIndex;
	clock_t			m_LastSendSummonTime;

	SI32			m_CurrentStatusPage;
	HWND			m_TabDlg[MAX_MUDANG_PAGE_NUMBER];
	
	// Evolve, Devolution, Transform
	SI32			m_siSelectEvolveDlg;
};


#endif