#ifndef _ONGAMENETENVIROMENTDLG_H_
#define _ONGAMENETENVIROMENTDLG_H_



class COnGameNetEnviromentDlg
{
public:
	enum {
		MAX_ONGAMENETENVIROMENT_PAGE_NUM = 2,
	};

	COnGameNetEnviromentDlg();
	~COnGameNetEnviromentDlg();

	void Create( HINSTANCE hInst, HWND hParent );
	
	static BOOL CALLBACK StaticOnGameNetEnviromentDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK OnGameNetEnviromentDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//_LEON_TUGOFWAR----------------------------------------------------------------------------------------
	static BOOL CALLBACK StaticOnGameNetMainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK OnGameNetMainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK StaticOnGameNetTugOfWarDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK OnGameNetTugOfWarDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CreateTabDlg(HWND hDlg, SI32 page, SI32 dlg, DLGPROC proc);
	
	HWND TabDlg[MAX_ONGAMENETENVIROMENT_PAGE_NUM];
	SI32 m_CurPage;
	SI16 m_CurTugMode;
	//------------------------------------------------------------------------------------------------------

	bool IsShow();

	void Show();
	void Hide();

	HWND m_hDlg;
	bool m_bShow;

	void PushData( SI16 Kind, char *Text );

	SI32 m_arrayMinute[60];
	SI32 m_arrayLevel[100];
	SI32 m_arrayNum[41];

	SI32 m_siSelectIndex ;
	SI32 m_siSelectVillageIndex;
	SI32 m_siSelectMinuteIndex;
	SI32 m_siSelectLevelIndex;
	SI32 m_siSelectNumIndex;

private:
	
};

#endif