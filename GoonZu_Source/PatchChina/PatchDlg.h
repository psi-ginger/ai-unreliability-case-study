// PatchDlg.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES

#if !defined(AFX_PATCHDLG_H__8DEDA850_CC30_4EC3_85A5_DCB17CED762F__INCLUDED_)
#define AFX_PATCHDLG_H__8DEDA850_CC30_4EC3_85A5_DCB17CED762F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPatchDlg dialog
#include <process.h>

class FrontProcess;

#include "../Server/FrontServer/FrontMsg.h"


#pragma comment(lib, "zlibstat.lib")

#include "TypeDefinition.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ProgressST.h"
#include "xSkinButton.h"

#define MAX_DOWNLOAD_NUM				1					// 한 꺼번에 다운로드 받을 수 있는 최대 파일 수

#define	MAX_DOWNLOAD_PATCH_FILE_NUM		50					// 패치가 가능한 파일의 개수( 최대 30개로 둔다... 그렇다고 패치가 되지 않는 것은 아니다... 30개를 넘게 받아야 하는 상황이 발생하면 웹에서 전체 설치 파일을 다운 받으라고 권고를 하기 때문이다. 권고용 )
#define	DOWNLOAD_PATCH_EXCESS			100					// 패치가 100 개 이상이 필요하다면, 패치가 안됨. 이 경우 완전 재설치를 해야 함.

#define WM_DRAWSTATUS					WM_USER+2

#define PROGRESS_WIDTH					259
#define PROGRESS_HEIGHT					18

#define PROGRESS_START_XPOS				10
#define	PROGRESS_START_YPOS				383

const int HOMEPAGE_BUTTON_X		 = 272;
const int HOMEPAGE_BUTTON_Y		 = 32;
const int HOMEPAGE_BUTTON_WIDTH	 = 64;
const int HOMEPAGE_BUTTON_HEIGHT = 19;

const int ACCOUNT_BUTTON_X		= 337;
const int ACCOUNT_BUTTON_Y		= 32;
const int ACCOUNT_BUTTON_WIDTH	= 64;
const int ACCOUNT_BUTTON_HEIGHT = 19;

const int CARD_BUTTON_X		= 402;
const int CARD_BUTTON_Y		= 32;
const int CARD_BUTTON_WIDTH	= 64;
const int CARD_BUTTON_HEIGHT = 19;

const int DOWN_BUTTON_X		= 467;
const int DOWN_BUTTON_Y		= 32;
const int DOWN_BUTTON_WIDTH	= 64;
const int DOWN_BUTTON_HEIGHT = 19;

const int NEXT_BUTTON_X		= 8;
const int NEXT_BUTTON_Y		= 417;
const int NEXT_BUTTON_WIDTH	= 264;
const int NEXT_BUTTON_HEIGHT = 55;

const int EXIT_BUTTON_X		= 594;
const int EXIT_BUTTON_Y		= 4;
const int EXIT_BUTTON_WIDTH	= 64;
const int EXIT_BUTTON_HEIGHT = 19;

const int CNCIDC_BUTTON_X		= 13;
const int CNCIDC_BUTTON_Y		= 44;
const int CNCIDC_BUTTON_WIDTH	= 110;
const int CNCIDC_BUTTON_HEIGHT = 28;

const int CTCIDC_BUTTON_X		= 126;
const int CTCIDC_BUTTON_Y		= 44;
const int CTCIDC_BUTTON_WIDTH	= 110;
const int CTCIDC_BUTTON_HEIGHT = 28;

const int PINGTEST_BUTTON_X		= 151;
const int PINGTEST_BUTTON_Y		= 438;
const int PINGTEST_BUTTON_WIDTH	= 175;
const int PINGTEST_BUTTON_HEIGHT = 36;

const int START_BUTTON_X		= 328;
const int START_BUTTON_Y		= 438;
const int START_BUTTON_WIDTH	= 175;
const int START_BUTTON_HEIGHT = 36;

const int EXIT2_BUTTON_X		= 597;
const int EXIT2_BUTTON_Y		= 2;
const int EXIT2_BUTTON_WIDTH	= 64;
const int EXIT2_BUTTON_HEIGHT = 19;

const int CHECK_BUTTON_X		= 189;
const int CHECK_BUTTON_Y		= 338;
const int CHECK_BUTTON_WIDTH	= 13;
const int CHECK_BUTTON_HEIGHT	= 13;

class CRegistryMgr;
class CHttpMgr;

enum{ DLG_INITIAL_STATUS, DLG_DOWNLOAD_STATUS, DLG_DECOMPRESS_STATUS, DLG_GAMESTART_STATUS, DLG_SOCKET_STATUS };

enum
{ 
	FIRST_PAGE = 0,
	NEXT_PAGE
};

enum
{
	RECT_BUTTON_HOMEPAGE = 0,
	RECT_BUTTON_ACCOUNTREGISTER,
	RECT_BUTTON_CARD,		
	RECT_BUTTON_DOWNLOAD,	
	RECT_BUTTON_NEXT,	
	RECT_BUTTON_CHECK,	// 풀스크린 체크
	RECT_BUTTON_EXIT,

	RECT_BUTTON_CNCIDC,
	RECT_BUTTON_CTCIDC,
	RECT_BUTTON_PINGTEST,
	RECT_BUTTON_START,
	RECT_BUTTON_EXIT2,

	RECT_END
};

class CPatchDlg : public CDialog
{
	// Construction
public:
	CPatchDlg(CWnd* pParent = NULL);	// standard constructor

	inline static unsigned __stdcall DownLoadMgrThread(void* lpArg); // DownLoad 관리 쓰레드

	bool CheckDupGame(char *name, char* title);

	void EndProcess();

	//bool GetBaseInfo();										// 기본 Ftp 경로를 파일에서 얻어 온다.
	bool GetRegistryInfo();									// 레지스트리에서 정보를 얻어온다.


	bool SetNewDownLoadPathCount( sResponse_PatchInfo * pRecvMsg );				// Count 갯수만큼 m_stDownLoadPath 를 생성한다. new stDownLoadPath[Count]
	void ParsingVersionData( char * );						// 패치 버전 서버로 부터 온 VersionData 들을 Parsing 하는 것.

	void DownLoadThreadStart();								// 다운로드 시작.
	void DownLoadThreadStop();								// 다운로드 종료.
	void DownLoadMgrThreadRun();							// 실제 다운로드 관리를 하는 부분.

	void Decompress();										// 압축 해제.

	int ZipFileDecompress(char* zipfilename);				// 실제 압축을 해제하는 곳
	int GetZipFileLise(char* zipfilename);					// zip 파일내에 있는 파일들 리스트를 얻어온다.

	void FrontMsgProcess( sPacketHeader *pPacket );

	bool IsCorrectDNS(char* hostname);

	// 현재 다운로드 상태를 화면에 보여주기 위해서..
	void DrawDownLoadStatus(CDC * Destination);								
	void DrawInstallStatus( CDC * Destination );
	void Drawing(CDC * Destination);
	void DrawCustomButton( CDC * Destination, int ButtonIndex, int ButtonMode );
	void DrawAreaButton( CDC * Destination, int ButtonIndex, int ButtonMode );

	// 패치 할 필요가 없거나, 패치가 끝 났을 시에 이 함수를 호출하여 게임을 실행함.
	void GameReady();		

	// 프론트 찾기
	void InitFrontServer( FrontProcess* pFrontFrocess, char* pFrontIP );
	// 버튼 이미지 등록
	void AddButtonImage( char* pFileName, CBitmap* pBitmap );

	int GetGameVersionInFile();
	bool SetGameVersionInFile(int siVersion);
	bool GetGameInfo();										// 게임 버젼정보와 실행경로 얻어온다.

	UI32			m_uiCurrentVersion;	

	// Dialog Data
	//{{AFX_DATA(CPatchDlg)
	enum { IDD = IDD_PATCH_DIALOG };
	CWebBrowser2	m_WebBrowser;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchDlg)
public:
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//HANDLE m_hFileRecvThreadHandle;
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CPatchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//afx_msg void OnInitialPatch( WPARAM wParam, LPARAM lParam );
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:	

	//CRITICAL_SECTION	Crit;
	HANDLE			m_hDownLoadMgrThreadHandle;								// 다운로드 관리하는 쓰레드의 핸들
	bool			m_bDownLoadMgrThreadRun;


	char			m_strGameFolderPath[1024];								// 게임 폴더 경로
	char			m_strPatchFolderPath[1024];								// 패치 파일 저장 경로
	char			m_strGameExeFileName[128];								// 게임 실행 파일명
	char			m_strGameFullPath[1536];								// 게임 전체 경로( GameFolderPath + GameFileName )

	CRegistryMgr	*m_pRegistryMgr;										// 레지스트리 정보 받아 오거나 레지스트리에 정보 쓰거나 할 때 편하게 쓰기 위해서 클래스화 했음.

	US16			m_usCount;												// DownLoad 하는 파일의 갯수
	UI32			*m_puiVersionList;				

	CHttpMgr		*m_pHttpMgr;

	UC08			m_ucCurrentStatus;										// 현재 상태

	bool			m_bButtonClick;

	DWORD			m_dwTotalFileSize;

	FrontProcess	*m_pFront;
	FrontProcess	*m_pCNCFront;
	FrontProcess	*m_pCTCFront;

	HBITMAP			m_hTitleBmp;

	char			m_strParameter[ 2048 ];

	int				m_siServiceArea;

public:
	//	afx_msg void OnBnClickedButtonRun();
	//	afx_msg void OnBnClickedButtonStop();
	afx_msg UINT OnNcHitTest(CPoint point);

	char		m_StatusTextBuffer[256];

	clock_t			m_StartClock;

	char			m_szPatchVersionInfo[16];
	char			m_szState[128];
	char			m_szPingNumber[MAX_IDC][MAX_FRONTSERVER_NUMBER][20];
	char			m_szLastServerName[128];
	CBitmap			m_ProgressBitmap;

	bool			m_bClick[RECT_END];
	bool			m_bHover[RECT_END];

	static	const	RECT CustomControlRect[RECT_END];

	CBitmap			m_CustomButtonBitmapClick[RECT_END];
	CBitmap			m_CustomButtonBitmapDisable[RECT_END];

	CBrush			m_BackGroundBrush[2];
	CBitmap			m_BackGroundBitmap[2];

	char			m_szIDC_IP[MAX_IDC][MAX_FRONTSERVER_NUMBER][64];
	char			m_szIDC_Name[MAX_IDC][MAX_FRONTSERVER_NUMBER][32];

	RECT			AreaControlRect[MAX_FRONTSERVER_NUMBER];
	bool			m_bAreaClick[MAX_FRONTSERVER_NUMBER];
	bool			m_bAreaHover[MAX_FRONTSERVER_NUMBER];

	int				m_siMaxCNCIDC_Number;
	int				m_siMaxCTCIDC_Number;
	int				m_siPrevAreaButtonIdex;
	int				m_siCDNSelect;

	CBitmap*		m_IDCBitmapClick[MAX_IDC];
	CBitmap*		m_IDCBitmapDisable[MAX_IDC];

	int				m_siPage;

	int				m_siSelectIDC;
	int				m_siSelectFrontServerIndex;

	bool			m_bFirstConnect;
	bool			m_bFailPatchInfo;
	int				m_siWhereConnect;
	int				m_siCNCConnectIndex;
	int				m_siCTCConnectIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHDLG_H__8DEDA850_CC30_4EC3_85A5_DCB17CED762F__INCLUDED_)
