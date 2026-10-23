
//후에 다시 작업 예정.. 우선 보류 .. 050325
#ifdef __SUN_GAMECONTRACT__


//---------------------------------
// 2004/1/28 김태곤
//---------------------------------

#include <Directives.h>

class cltGameContract{
public:
	HWND m_hDlg;					// 아이템 정보. 

	RECT DlgRect;
	bool bDlgRectInitSwitch;


	cltGameContract();
	~cltGameContract();


	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif