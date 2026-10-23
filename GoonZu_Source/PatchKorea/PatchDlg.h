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

// - NHN ACM -
#ifdef USE_HANAUTH
	#include "../HanAuth/HanAuthForClient.h"
#endif

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	#include "../Porte/PorteAuthForClient.h"
	using namespace Porte::PorteAuthSystem;
#endif // USE_PORTEAUTH

#ifdef USE_HANREPORT
	#include "../HanAuth/HanReportForClient.h"

	#ifdef _DEBUG
		#pragma comment(lib, "HanReportForClientD.lib") 
	#else
		#pragma comment(lib, "HanReportForClient.lib") 
	#endif // _DEBUG
#endif

#ifdef USE_CNREPORT
	#include "../HanAuth/HanReportForClient.h"

	#ifdef _DEBUG
		#pragma comment(lib, "HanReportForClientD.lib") 
	#else
		#pragma comment(lib, "HanReportForClient.lib") 
	#endif // _DEBUG
#endif

#define HANREPORT_PATCH_STARTPROGRAM			"250"
#define HANREPORT_PATCH_ENDPROGRAM				"251"

#define HANREPORT_PATCH_INITPROGRAM				"253"

#define HANREPORT_PATCH_GAMESTRING_FAILED		"255"

#define HANREPORT_PATCH_FRONTSERVER_CONNECTED	"260"
#define HANREPORT_PATCH_FRONTSERVER_RECVINFO	"261"

#define HANREPORT_PATCH_FRONTSERVER_ADDRFAILED	"265"

#define HANREPORT_PATCH_DOWNLOAD_START			"270"
#define HANREPORT_PATCH_DOWNLOAD_END			"271"
#define HANREPORT_PATCH_UNZIP_START				"275"
#define HANREPORT_PATCH_UNZIP_END				"276"

#define HANREPORT_PATCH_GAMESTART				"280"

//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 
#define		PURPLE_OUTBOUND_INFORM					9
#define		PURPLE_OUTBOUND_DOWNLOAD_PROGRESS		18
#define		PURPLE_OUTBOUND_DOWNLOAD_SPEED			27
#define		PURPLE_OUTBOUND_DOWNLOAD_RESTTIME		31
#define		PURPLE_OUTBOUND_FILENAME				36
#define		PURPLE_OUTBOUND_PATCH_PROGRESS			45
#define		PURPLE_OUTBOUND_PATCH_FILE_NUMBER		54
#define		PURPLE_OUTBOUND_ENDING					63


	typedef struct PurpleCDS
	{
		TCHAR szString[160];
		DWORD dwIndex;
	} PurpleCDS;

	/**
	 * 1. 외부모듈 기동할 때
	 *		퍼플런쳐는 다음 내용을 시작인자로 전달한다.
	 *		(1) HWND : SendMessage를 받을 윈도우 핸들. 10진수로 표시된다.
	 *		(2) argument : 기타 외부모듈이 기동할 때 필요한 내용을 전달한다.
	 *
	 * 2. 퍼플런쳐/외부모듈 간의 연동
	 *		(1) 퍼플런쳐 => 외부모듈로 통신하는 경우는 없으며 (최초에 인자를 전달하기만 한다), 
	 *			이후에는 외부모듈 => 퍼플런쳐의 통신만을 한다. 
	 *		(2) 통신방식을 WM_COPYDATA를 사용하며 메시지를 전달할 때에는 다음의 형태를 취한다. 
	 *			::SendMessage(HWND, WM_COPYDATA, HWND, PCOPYDATASTRUCT);
	 *				a. HWND : 퍼플런쳐 윈도우 핸들이다. 이 값은 외부모듈이 최초 기동할 때 인자로 전달된다.
	 *				b. WM_COPYDATA : 윈도우 메시지
	 *				c. HWND : 메시지를 리턴받을 윈도우 핸들을 기입한다. (이 값은 미래를 위해서 세팅한다.)
	 *				d. PCOPYDATASTRUCT : WM_COPYDATA를 쓸 때 전송하는 구조체이다. 사용법은 MSDN 참고.
	 *		
	 * 						typedef struct tagCOPYDATASTRUCT {
	 * 							ULONG_PTR dwData;
	 *							DWORD cbData;
	 *							PVOID lpData;
	 *						} COPYDATASTRUCT, *PCOPYDATASTRUCT; 
	 *
	 * 3. 인덱스(dwData)에 따른 의미
	 *		(1) PURPLE_OUTBOUND_INFORM : Inform 스태틱에 스트링을 표시한다.
	 *			- szString : 스태틱에 표시할 스트링
	 *			- dwIndex : N/A
	 *		
	 *		(2) PURPLE_OUTBOUND_DOWNLOAD_PROGRESS : 다운로드 프로그래스를 조정한다.
	 *			- szString : N/A
	 *			- dwIndex : 프로그래스 (0부터 100까지)
	 *
	 *		(3) PURPLE_OUTBOUND_DOWNLOAD_SPEED : 다운로드 속도를 표시한다.
	 *			- szString : N/A
	 *			- dwIndex : 다운로드 스피드 (byte / millisecond)
	 *
	 *		(3.1) PURPLE_OUTBOUND_DOWNLOAD_RESTTIME : 남은시간을 표시한다. 
	 *			- szString : N/A
	 *			- dwIndex : 남은시간 (millisecond)
	 *
	 *		(4) PURPLE_OUTBOUND_FILENAME : 현재 처리하는 파일의 이름을 표시한다. 
	 *			- szString : 처리중인 파일명칭
	 *			- dwIndex : N/A
	 *
	 *		(5) PURPLE_OUTBOUND_PATCH_PROGRESS : 패치 프로그래스를 조정한다. 패치 프로그래스는 전체설치공정을 표시한다.
	 *			- szString : N/A
	 *			- dwIndex : 프로그래스 (0부터 100까지)
	 *
	 *		(6) PURPLE_OUTBOUND_PATCH_FILE_NUMBER : 패치할 전체 파일 개수 및 현재까지 처리한 파일의 개수를 표시한다.
	 *			- szString : N/A
	 *			- dwIndex : HIWORD => 전체파일개수, LOWORD => 현재까지 처리한 파일 개수
	 *
	 *		(7) PURPLE_OUTBOUND_ENDING : 외부모듈이 종료됨을 전달한다. 
	 *			- szString : 오류스트링
	 *			- dwIndex : 오류코드
	 *
	 *			a. 오류코드 0 : 패치완료 후 정상종료됨. 퍼플런쳐는 이 메시지를 받으면 게임시작 stand by 상태가 된다.
	 *			b. 오류코드 1 : 인스톨쉴드를 다시 받아야 함을 뜻한다. szString에 다운받아야 하는 파일 URL을 기록한다.
	 *			c. 기타 오류코드 : 하나씩 지정한다. (예를 들면, dll 로드에 실패하였다. 패치파일 다운로드에 실패하였다등..)
	**/
#endif // NHNPATCH_HIDEDLG 


#define MAX_DOWNLOAD_NUM				1						// 한 꺼번에 다운로드 받을 수 있는 최대 파일 수

#ifdef _USA
	#define	MAX_DOWNLOAD_PATCH_FILE_NUM		500					// USA 패치가 가능한 파일의 개수( 최대 500개로 둔다... 그렇다고 패치가 되지 않는 것은 아니다... 500개를 넘게 받아야 하는 상황이 발생하면 웹에서 전체 설치 파일을 다운 받으라고 권고를 하기 때문이다. 권고용 )
#else	
	#define	MAX_DOWNLOAD_PATCH_FILE_NUM		50					// 패치가 가능한 파일의 개수( 최대 30개로 둔다... 그렇다고 패치가 되지 않는 것은 아니다... 30개를 넘게 받아야 하는 상황이 발생하면 웹에서 전체 설치 파일을 다운 받으라고 권고를 하기 때문이다. 권고용 )
#endif


#define	DOWNLOAD_PATCH_EXCESS			100					// 패치가 100 개 이상이 필요하다면, 패치가 안됨. 이 경우 완전 재설치를 해야 함.

#define WM_DRAWSTATUS					WM_USER+2

#define PROGRESS_WIDTH					318
#define PROGRESS_HEIGHT					18

#define PROGRESS_START_XPOS				17
#define	PROGRESS_START_YPOS				272

#define DLG_SOCKET_STATUS_LIFETIME		30					//	[종호] 현 패치상태가 소켓상태 제한 시간

class CRegistryMgr;
class CHttpMgr;

enum{ DLG_INITIAL_STATUS, DLG_DOWNLOAD_STATUS, DLG_GAMESTART_STATUS, DLG_SOCKET_STATUS };

void WriteFileLog(TCHAR* pszFileName, TCHAR* pszSource, int nLine, TCHAR* pszLogFormat, ...);

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
	bool GetGameInfo();										// 게임 버젼정보와 실행경로 얻어온다.
	

	bool SetNewDownLoadPathCount( sResponse_PatchInfo * pRecvMsg, bool bUseSecondAddr );				// Count 갯수만큼 m_stDownLoadPath 를 생성한다. new stDownLoadPath[Count]
	void ParsingVersionData( char * );						// 패치 버전 서버로 부터 온 VersionData 들을 Parsing 하는 것.
	
	void DownLoadThreadStart();								// 다운로드 시작.
	void DownLoadThreadStop();								// 다운로드 종료.
	void DownLoadMgrThreadRun();							// 실제 다운로드 관리를 하는 부분.
	

	void DrawDownLoadStatus();								// 현재 다운로드 상태를 화면에 보여주기 위해서..
	void DrawInstallStatus( char *pString );

	void Decompress();										// 압축 해제.

	int ZipFileDecompress(char* zipfilename);				// 실제 압축을 해제하는 곳
	int GetZipFileLise(char* zipfilename);					// zip 파일내에 있는 파일들 리스트를 얻어온다.

	void GameStart();										// 패치 할 필요가 없거나, 패치가 끝 났을 시에 이 함수를 호출하여 게임을 실행함.

	void	CreateGoonZuShourCut( void );
	HRESULT CreateShortCut( char* szSrcFile, char* szLinkFile, char* szArgument, char* szDesc, char* szWorkingDirectory );
	void	GetSpecialFolderPath( OUT char* szGetFolderPath, size_t nSize, int nFolder );

	void FrontMsgProcess( sPacketHeader *pPacket );

	void Drawing();

	bool IsCorrectDNS(char* hostname);

	void HanAuth_Lib_HanReportSendPosLog_Client(TCHAR* pszID, TCHAR* pszValue);
	void HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(TCHAR* pszUserID, TCHAR* pszValue);	// 패치런처떄는 서버 아이디가 없다.

	UI32			m_uiCurrentVersion;	
//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 	
	void SendMessageToPurple(int nType, LPCTSTR lpszMsg, DWORD dwValue);
#endif // NHNPATCH_HIDEDLG 

// Dialog Data
	//{{AFX_DATA(CPatchDlg)
	enum { IDD = IDD_PATCH_DIALOG };
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
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//afx_msg void OnInitialPatch( WPARAM wParam, LPARAM lParam );
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
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

	HBITMAP			m_hTitleBmp;
	
	char			m_strParameter[ 4096 ];

	int				m_siServiceArea;
	int				m_siRunMode;
	//CProgressCtrl m_DownProgress;
#ifdef _USA_NEW_AUTH_
	bool									m_bUSAAuth;	
	TCHAR									m_szUSAAuth_String[4096];
#endif
#ifdef USE_NDOORS_KOREA
	bool									m_bUSAAuth;	
	TCHAR									m_szUSAAuth_String[4096];
#endif

#ifdef USE_HANAUTH
	// -HanAuth
	TCHAR									m_szHanAuth_GameString[SIZE_GAMESTRING];
	TCHAR									m_szHanAuth_AuthString[SIZE_AUTHSTRING];
	TCHAR									m_szHanAuth_UserID[20];
	int										m_siHanAuth_ServiceTypeCode;

	// -HanAuth
#endif // USE_HANAUTH

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	TCHAR									m_szPorteAuth_GameInfoString[SIZE_GAMEINFOSTRING];
	TCHAR									m_szServerIP[20]; // AG로 들어오는 인증서버 접속용 IP
#endif // USE_PORTEAUTH
	//int										m_siPorteAuth_ServiceTypeCode;
	TCHAR									m_szPorteAuth_UserID[13]; // User ID size is 12byte
//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 
	HWND 									m_hTarget;  // AG로 들어오는 exLauncher의 핸들러.
	TCHAR									m_szServerIP[20]; // AG로 들어오는 인증서버 접속용 IP
	TCHAR									m_szPortNum[10]; // AG로 들어오는 인증서버 접속용 포트 번호.
#endif // NHNPATCH_HIDEDLG 


public:
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	CProgressST m_DownLoadProgress2;
	//CStatic m_StaticStatus;
	// PCK : 버전표시 컨트롤 추가
	/*CStatic	m_StaticVersion;*/

	CxSkinButton m_ButtonStart;

	char		m_StatusTextBuffer[256];
	//CStatic m_StaticImage;

	clock_t			m_StartClock;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHDLG_H__8DEDA850_CC30_4EC3_85A5_DCB17CED762F__INCLUDED_)
