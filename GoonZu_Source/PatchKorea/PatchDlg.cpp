// PatchDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Patch.h"
#include "PatchDlg.h"
#include "DownLoadObj.h"
#include "RegistryMgr.h"
#include "FrontProcess.h"
#include "ITZEncrypt.h"
#include "ITZSendBuffer.h"
#include "TextMgr.h"

#include "for_main_function.h"
#include "HttpMgr.h"

#include "FrontProcess.h"
#include ".\patchdlg.h"
#include "WinSock.h"
#include "WinSock2.h"

#define SA_KOR			100
#define SA_NETMARBLE	200
#define SA_ENG			300
#define SA_CHA			400
#define SA_TESTG		500
#define SA_JPN			600
#define SA_TAI			700
#define SA_USA			800
#define SA_NHNCHA		900
#define SA_EUROPE		1000
#define SA_KOREA_NEW	1100

#define PATCH_FOLDER	"Download\\"

#define	DONOT_WRITEFILELOG

enum eGameMode{
	GAMEMODE_SERVER	= 1	,
	GAMEMODE_CLIENT		,
	GAMEMODE_MAPEDIT
};

enum eRunMode {
	RUNMODE_DEBUG = 1,
	RUNMODE_TEST,
	RUNMODE_BETA,
	RUNMODE_REAL	// 바꾸지 않고 뒤에 추가한다.
};

void WriteFileLog(TCHAR* pszFileName, TCHAR* pszSource, int nLine, TCHAR* pszLogFormat, ...)
{

#ifndef DONOT_WRITEFILELOG

	if(pszFileName == NULL)		return;
	if(pszSource == NULL)		return;
	if(pszLogFormat == NULL)	return;

	TCHAR	szLogMsg[1024 * 5];
	szLogMsg[0] = '\0';

	TCHAR	szLine[128];
	sprintf(szLine, "%d", nLine);

	TCHAR	szTime[128];
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d",	systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

	va_list args;
	int len;
	char * buffer;

	va_start( args, pszLogFormat );
	_vsnprintf( szLogMsg, sizeof(szLogMsg) - 10, pszLogFormat, args );
	va_end( args );

	FILE* fp;
	fp = fopen(pszFileName, "a");
	if(fp != NULL)
	{
		fprintf(fp, "Time=");	fprintf(fp, szTime);	fprintf(fp, "|");
		fprintf(fp, "Source=");	fprintf(fp, pszSource);	fprintf(fp, "|");
		fprintf(fp, "Line=");	fprintf(fp, szLine);	fprintf(fp, "|");
		fprintf(fp, "LogMsg=");	fprintf(fp, szLogMsg);	fprintf(fp, "|\n");		

		fclose(fp);
	}

#endif // DONOT_WRITEFILELOG
}

int GetArgument(char* pszString, char* pszOut, SI32 siOutLen)
{
	bool	bQuotation = false;
	SI32	siPtr = 0;
	SI32	siOut = 0;

	if(pszString[siPtr] == '\"' || pszString[siPtr] == '\'')
	{
		bQuotation = true;
		pszOut[siOut++] = pszString[siPtr];
		siPtr++;
	}

	while(siOut < siOutLen && pszString[siPtr] != '\0')
	{
		if(pszString[siPtr] == ' ')
		{
			if(bQuotation == false)		break;
			pszOut[siOut++] = pszString[siPtr];
		}
		else if(pszString[siPtr] == '\'' || pszString[siPtr] == '\"')
		{
			pszOut[siOut++] = pszString[siPtr];
			break;
		}
		else
		{
			pszOut[siOut++] = pszString[siPtr];
		}
		siPtr++;
	}

	pszOut[siOut] = '\0';

	//if(bQuotation == true)		siOut += 2;

	return siOut;
}

// 파일에서 GameMode를 얻어온다. 
bool GetGameModeInFile( SI32* pgamemode, SI32* pres, BOOL* bFullScreen, SI32* siContryCode, SI32* siRunMode )
{
	FILE* fp = NULL;
	TCHAR buffer[1024];

	SI32 gamemode = 0;
	SI32 resolution = 0;
	SI32 bfull = 0;
	SI32 contry = 0;
	SI32 runmode = 0;

	SI16 siSuccess = 0;
	gamemode = GAMEMODE_CLIENT;	// Default GameMode!!
	resolution = 1;        
	bfull = 0;
	contry = 0;
	runmode = RUNMODE_REAL;

	*pgamemode		= gamemode;
	*pres			= resolution;
	*bFullScreen	= bfull;
	*siContryCode	= contry;
	*siRunMode		= runmode;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchDlg dialog

void WinSockInit()
{
	WORD	wVersionRequested;
	WSADATA	wsaData;

	int err;

	// 버젼 2.2
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );

	if( err != 0 ) {
		return;
	}

	// 버젼 확인
	if( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {

			
		WSACleanup();
		return;
	}
}

CPatchDlg::CPatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	WinSockInit();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pFront = NULL;
	m_pHttpMgr = NULL;
	m_pRegistryMgr = NULL;
	m_puiVersionList = NULL;

	m_pFront = new FrontProcess;
	

	m_ucCurrentStatus = DLG_INITIAL_STATUS;	

	StringCchCopy( m_StatusTextBuffer, 256, "");
}

void CPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchDlg)
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_PROGRESS_DOWN, m_DownProgress);
	DDX_Control(pDX, IDC_PROGRESS_DOWN2, m_DownLoadProgress2);
	//DDX_Control(pDX, IDC_STATIC_STATUS, m_StaticStatus);
	DDX_Control(pDX, IDC_BUTTON_RUN, m_ButtonStart);
	//DDX_Control(pDX, IDC_STATIC_VERSION, m_StaticVersion);
	//DDX_Control(pDX, IDC_STATIC_IMAGE, m_StaticImage);
}
BEGIN_MESSAGE_MAP(CPatchDlg, CDialog)
	//{{AFX_MSG_MAP(CPatchDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()	
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	//ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RUN, OnBnClickedButtonRun)
	//ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchDlg message handlers



//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - unsigned __stdcall CPatchDlg::DownLoadMgrThread(void* lpArg)
//	Desc : DownLoad 관리 쓰레드
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
unsigned __stdcall CPatchDlg::DownLoadMgrThread(void* lpArg)
{
	CPatchDlg *pPatchDlg = (CPatchDlg *) lpArg;

	pPatchDlg->DownLoadMgrThreadRun();

	_endthreadex( 0 );
	return 0;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - BOOL CPatchDlg::OnInitDialog()
//	Desc : 기본적인 초기화를 담당
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
BOOL CPatchDlg::OnInitDialog()
{
	m_strParameter[ 0 ] = NULL;

	ZeroMemory(m_szUSAAuth_String, sizeof(m_szUSAAuth_String));

	m_bUSAAuth = false;

	char *pargv;
	SI32 siPtr = 0;
	char szName[MAX_PATH];
	SI32 siLen = 0;
	pargv = GetCommandLine();

	// 실행파일명
	siLen = GetArgument(&pargv[siPtr], szName, MAX_PATH);
	siPtr += siLen;
	siPtr++;

	// 첫번째 Argument
	siLen = GetArgument(&pargv[siPtr], m_szUSAAuth_String, sizeof(m_szUSAAuth_String));

	// 글로벌은 두번째 인자 없다. 그러니까 두번째 인자값이 있으면 글로벌화 된 USA의 처리를 한다. 
	if( 3 < siLen ) 
		m_bUSAAuth = true;

	CDialog::OnInitDialog();

	char *pText="";
	char *pTitle="";

	StringCchCopy( m_StatusTextBuffer, 256, "");

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//------------------------------------------------------------------
	// 실행파일명 추출
	char FilePathBuffer[ MAX_PATH ] = "";
	::GetModuleFileName( NULL, FilePathBuffer, sizeof( FilePathBuffer ) );
	
	char Drive[_MAX_DRIVE];
	char Path[_MAX_PATH];
	char Filename[_MAX_FNAME];
	char Ext[_MAX_EXT];
	
	::_splitpath( FilePathBuffer, Drive, Path, Filename, Ext );

	char PathBuffer[ MAX_PATH ] = "";
	strcpy( PathBuffer, Drive );
	strcat( PathBuffer, Path );
	//------------------------------------------------------------------

	// 현재 디렉토리 설정
	::SetCurrentDirectory( PathBuffer );	

	m_DownLoadProgress2.SetRange32( 0, 100 );

	//------------------------------------------------------------------
	
	// 서비스 지역 설정
	FILE * fp = NULL ;
	char szFileName[ MAX_PATH ] = "" ;
	char str[ MAX_LENGTH ] = "" ;	

	strcpy(str,"SERVICEAREA_NDOORSKOREA");	//서비스 지역 설정 - 설정내용

	/*
	if ( strcmp( str, "SERVICEAREA_KOREA" ) == 0 )
	{
		m_siServiceArea = SA_KOR;
	}
	else if ( strcmp( str, "SERVICEAREA_NETMARBLE" ) == 0 )
	{
		m_siServiceArea = SA_NETMARBLE;
	}
	else if ( strcmp( str, "SERVICEAREA_ENGLISH" ) == 0 )
	{
		m_siServiceArea = SA_ENG;
	}
	else if ( strcmp( str, "SERVICEAREA_CHINA" ) == 0 )
	{
		m_siServiceArea = SA_CHA;
	}
	else if ( strcmp( str, "SERVICEAREA_NHNCHINA" ) == 0 )
	{
		m_siServiceArea = SA_NHNCHA;
	}
	else if ( strcmp( str, "SERVICEAREA_TESTGOONZU" ) == 0 )
	{
		m_siServiceArea = SA_TESTG;
	}
	else if ( strcmp( str, "SERVICEAREA_JAPAN" ) == 0 )
	{
		m_siServiceArea = SA_JPN;
	}
	else if ( strcmp( str, "SERVICEAREA_TAIWAN" ) == 0 )
	{
		m_siServiceArea = SA_TAI;
	}
	else if ( strcmp( str, "SERVICEAREA_USA" ) == 0 )
	{
		m_siServiceArea = SA_USA;
	}
	else if ( strcmp( str, "SERVICEAREA_EUROPE" ) == 0 )
	{
		m_siServiceArea = SA_EUROPE;
	}
	else if ( strcmp( str, "SERVICEAREA_NDOORSKOREA" ) == 0 )
	{
		m_siServiceArea = SA_KOREA_NEW;
	}
	//------------------------------------------------------------------

	//------------------------------------------------------------------
	// GAMEMODE
	//------------------------------------------------------------------
	/*
	SI32 siGameMode;
	SI32 siRes, siFull, siContryCode, siRunMode;

	GetGameModeInFile(&siGameMode, &siRes, &siFull, &siContryCode, &siRunMode);

	m_siRunMode = siRunMode;

	// -중국임시
	WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "m_siServiceArea = %d , siGameMode=%d ",m_siServiceArea, siGameMode  );
	*/

	m_siServiceArea = SA_KOREA_NEW;
	m_siRunMode		= 4;
	//------------------------------------------------------------------
		
	//------------------------------------------------------------------
	// 버튼 및 컨트롤 위치 결정
	// PCK - 버튼및 컨트롤 위치와 크기 변경됨 (08.10.20)
	//m_ButtonStart.MoveWindow(220,50, 100,20);

//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 
#else
	m_ButtonStart.RedrawWindow();
	g_pTextMgr.Create( str );
#endif
	//------------------------------------------------------------------
	// FRONT SERVER IP 
	char serverip1[ 256 ] ="";
	char serverip2[ 256 ] ="aaa.bbb.com";

	char buf[ 1024 ];
	ZeroMemory(buf, 1024);

#ifdef USE_HANAUTH	// 한게임은 주소를 두개 사용함. "리얼	알파"
#elif USE_PORTEAUTH
//KHY - 0316 - NHNChina Outbound
#elif NHNPATCH_HIDEDLG 
#elseif USE_NDOORS_KOREA // 직접 아이피를 박아서 사용
#else		// 기타 국가는 기존 방식 그대로 사용
	char	masterip1[256], masterip2[256];

	strcpy(masterip1,"gzsauth.ndoors.com" );
	strcpy(masterip2, "gzsauth.ndoors.com" );

	strcpy( serverip1, masterip1 );
	strcpy( serverip2, masterip2 );

#endif
	
WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Page-2-1");
	
#ifdef USE_HANAUTH	// 한게임 인증의 경우 11000 으로 연결
	WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "ip1=%s;ip2=%s;port=%d", serverip1, serverip2, 11000);

	m_pFront->Init( serverip1, serverip2, 11000, this );

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#elif USE_PORTEAUTH
		WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "ip1=%s;ip2=%s;port=%d", serverip1, 11000);

		m_pFront->Init( serverip1, serverip2, 11000, this );

//KHY - 0316 - NHNChina Outbound
#elif NHNPATCH_HIDEDLG 
		UI16 portNum = (UI16)(atoi(m_szPortNum));

		WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "NHNPATCH_HIDEDLG -2-2 ip1=%s;port=%d", serverip1, portNum);

		m_pFront->Init( serverip1, serverip2, portNum, this );
#else
	WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "NHNPATCH_HIDEDLG -2-2 ip1=%s;port=%d", serverip1, 5700);
	m_pFront->Init( serverip1, serverip2, 5700 , this );
#endif	
	//------------------------------------------------------------------

	m_dwTotalFileSize = 0;
	m_usCount = 0;

	m_uiCurrentVersion = 0;
		
	ZeroMemory( m_strGameFolderPath, 1024 );		// 게임 폴더 경로
	ZeroMemory( m_strGameExeFileName, 128 );		// 게임 실행 파일명
	ZeroMemory( m_strGameFullPath, 1536 );			// 게임 전체 경로	

#ifndef SA_KOREA	// 한국 HANGAME은 멀티 클라이언트가 가능하다.
#ifndef SA_NHNChina  // NHNChina는 멀티 클라이언트가 가능하다. 

	pTitle = GetTxtFromMgr(2);
	HWND hWnd = ::FindWindow( pTitle , NULL );

	if ( hWnd != NULL )
	{
		pText = GetTxtFromMgr(3);
		pTitle = GetTxtFromMgr(4);

		MessageBox(pText,pTitle, MB_ICONQUESTION | MB_OK );		
		EndProcess();		
		return TRUE;
	}

	pText = GetTxtFromMgr(1);
	hWnd = ::FindWindow( NULL, pText );

	if ( hWnd != NULL )
	{
		pText = GetTxtFromMgr(5);
		pTitle = GetTxtFromMgr(1);

		MessageBox(pText,pTitle, MB_ICONQUESTION | MB_OK );		
		EndProcess();		
		return TRUE;
	}
#endif
#endif

	pText = TEXT("Patch");
	SetWindowText(pText);

// NHNChina 는 파일에서 정보를 읽어온다.
#ifndef SA_NHNChina 
	//------------------------------------------------------------------
	// 레지스트리 정보
	if( !GetRegistryInfo() )
	{
		CHAR szTitle[1024]	=	"\0";
		CHAR szText[1024]	=	"\0";


		/*
		pText = GetTxtFromMgr(1);
		pTitle =GetTxtFromMgr(6);
		MessageBox(pTitle,pText, MB_ICONQUESTION | MB_OK );
		*/
		
		switch( m_siServiceArea )
		{
		case SA_KOR:
			::ShellExecute( NULL, NULL, "http://gzs.hangame.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_ENG:
			::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_CHA:
			::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_NHNCHA: // 임시  - TAI와 같다.
			//::ShellExecute( NULL, NULL, "http://goonzu.omg.com.tw/", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_NETMARBLE:
			::ShellExecute( NULL, NULL, "http://www.netmarble.net", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_TESTG:
			::ShellExecute( NULL, NULL, "http://www.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_JPN:
			::ShellExecute( NULL, NULL, "http://www.kunshu.jp", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_TAI:
			::ShellExecute( NULL, NULL, "http://goonzu.omg.com.tw/", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_USA:
			::ShellExecute( NULL, NULL, "http://luminary.ijji.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_EUROPE:
			::ShellExecute( NULL, NULL, "http://luminary.aeriagames.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_KOREA_NEW:
			if ( (0 < m_uiCurrentVersion) && (465 > m_uiCurrentVersion) )
			{
				StringCchCopy( szTitle,	1024, "군주Special" );
				StringCchCopy( szText,	1024, "현재 버전은 게임을 실행할 수 없습니다\r게임을 다시 설치하여 주십시요." );
			}
			else
			{
				StringCchCopy( szTitle,	1024, "군주Special" );
				StringCchCopy( szText,	1024, "게임이 설치되어있지 않습니다.\r게임을 다시 설치하여 주십시요." );
			}
			
			MessageBox(szText, szTitle, MB_ICONQUESTION | MB_OK );

			::ShellExecute( NULL, NULL, "http://goonzus.ndoors.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		}

		EndProcess();
		return TRUE;
	}
#else
//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 
#else
	if( !GetGameInfo() ) // 파일로부터 게임의 경로를 읽어 온다.
	{
		pText = GetTxtFromMgr(1);
		pTitle =GetTxtFromMgr(6);
		MessageBox(pTitle,pText, MB_ICONQUESTION | MB_OK );
		
		switch( m_siServiceArea )
		{
		case SA_KOR:
			::ShellExecute( NULL, NULL, "http://gzs.hangame.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_ENG:
			::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_CHA:
			::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_NHNCHA: // 임시  - TAI와 같다.
			//::ShellExecute( NULL, NULL, "http://goonzu.omg.com.tw/", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_NETMARBLE:
			::ShellExecute( NULL, NULL, "http://www.netmarble.net", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_TESTG:
			::ShellExecute( NULL, NULL, "http://www.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_JPN:
			::ShellExecute( NULL, NULL, "http://www.kunshu.jp", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_TAI:
			::ShellExecute( NULL, NULL, "http://goonzu.omg.com.tw/", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_USA:
			::ShellExecute( NULL, NULL, "http://luminary.ijji.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_EUROPE:
			::ShellExecute( NULL, NULL, "http://luminary.aeriagames.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		case SA_KOREA_NEW:
			::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
			break;
		}


		EndProcess();
		return TRUE;
	}
#endif // NHNPATCH_HIDEDLG 

#endif
	// 패치파일을 받는 폴더 생성
	CreateDirectory( m_strPatchFolderPath, NULL);
	WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Page-4 -");

	//m_uiCurrentVersion	=	555;
	if(m_uiCurrentVersion == 0)
	{
		// 버전정보가 없는 경우 일단 게임을 실행할 수 있도록 해준다.
		m_DownLoadProgress2.SetBkColor( RGB(0, 0, 0));
		m_DownLoadProgress2.SetBitmap( IDB_BITMAPBAR );

		m_DownLoadProgress2.SetPos( 100 );
		m_DownLoadProgress2.Invalidate();
		m_ButtonStart.EnableWindow( TRUE );
		return TRUE;
	}
	else
	{
		m_hDownLoadMgrThreadHandle = NULL;
		m_bDownLoadMgrThreadRun = false;	

		m_ucCurrentStatus = DLG_SOCKET_STATUS;

		m_bButtonClick = false;

		m_StartClock = clock();
		m_ButtonStart.EnableWindow( FALSE );
		SetTimer( 0, 10, NULL );

		m_DownLoadProgress2.SetBkColor( RGB(0, 0, 0));
		m_DownLoadProgress2.SetBitmap( IDB_BITMAPBAR );
	}

//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW); // 작업표시줄에 표시를 하지 않는다.

	ShowWindow(SW_SHOWMINIMIZED); // 깜박이는 현상을 없앤다.

	PostMessage (WM_SHOWWINDOW,FALSE, SW_OTHERUNZOOM); // 다이얼로그 방식의 창을 안보이게 설정한다.
#endif // NHNPATCH_HIDEDLG

	WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Page-5 -");


	return TRUE;  // return TRUE  unless you set the focus to a control
}

//--------------------------------------------------------------------------------------------
//	Name : CheckDupGame()
//	Desc : 같은 프로그램 확인
//	Date : 2007. 05. 17
//	Update : 
//--------------------------------------------------------------------------------------------
bool CPatchDlg::CheckDupGame(char *name, char* title)
{
	HANDLE FileMapHandle = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, 1024, name);
	
	if(FileMapHandle != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle( FileMapHandle );
		return true;
	}

	CloseHandle( FileMapHandle );
	return false;
}

//--------------------------------------------------------------------------------------------
//	Name : OnDestroy()
//	Desc : 
//	Date : 2007. 05. 17
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	if ( m_pRegistryMgr )
	{
		delete m_pRegistryMgr;
		m_pRegistryMgr = NULL;
	}

	if ( m_puiVersionList ) 
	{
		delete [] m_puiVersionList;
		m_puiVersionList = NULL;
	}
	
	if ( m_pFront )
	{
		delete m_pFront;
		m_pFront = NULL;
	}

	if ( m_hTitleBmp )
	{
		DeleteObject( m_hTitleBmp );
		m_hTitleBmp = NULL;
	}


}

//--------------------------------------------------------------------------------------------
//	Name : OnDestroy()
//	Desc : 
//	Date : 2007. 05. 17
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

//--------------------------------------------------------------------------------------------
//	Name : OnDestroy()
//	Desc : 
//	Date : 2007. 05. 17
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		CWindowDC dc(this); 
		CDC memDC; 
		memDC.CreateCompatibleDC(&dc);
		memDC.DeleteDC();

		m_ButtonStart.Invalidate();
		m_DownLoadProgress2.Invalidate();
	
		CDialog::OnPaint();
	}	
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPatchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::OnCancel() 
//	Desc : 다이얼로그가 종료 될때 처리해 주는 것.
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnCancel() 
{	
	// TODO: Add extra cleanup here	

	char *pText= TEXT("Are you sure you want to stop this installation?");
	char *pTitle= TEXT("Patch");
	if ( MessageBox(pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES )
	{
		KillTimer( 0 );		

		if ( m_pHttpMgr != NULL )
		{
			m_pHttpMgr->FileRecvStop();
		}

		EndProcess();
	}
	else
	{
		return;
	}
		
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::OnOK() 
//	Desc : 다이얼로그에서 엔터키 치면 닫히는 것 방지 하기 위해서...
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnOK() 
{
	// TODO: Add extra validation here

	if ( m_ucCurrentStatus == DLG_GAMESTART_STATUS )
	{
		KillTimer( 0 );		

		STARTUPINFO si={0,};
		PROCESS_INFORMATION pi;

		si.cb = sizeof( STARTUPINFO );
		si.dwFlags = 0;


		{
			FILE * fp = NULL;
			fp = fopen("Text.txt", "a+");

			if(fp)
			{

				fprintf(fp , TEXT("%s"), m_strParameter);
				fclose(fp);
			}

		}
		::CreateProcess( m_strGameFullPath, m_strParameter, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );

		{
			FILE * fp = NULL;
			fp = fopen("Text3.txt", "a+");

			if(fp)
			{

				fprintf(fp , TEXT("%s"), m_strGameFullPath);
				fclose(fp);
			}

		}
		EndProcess();

		return;
	}
	
	return;	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::EndProcess()
//	Desc : 다이얼로그가 종료 될때 처리해 주는 것.
//		   먼저 다운로드 하던 객체들의 쓰레드 종료 시킴
//		   다운로드 하던 객체들 다 삭제 시켜주고, HTTP 관련 객체들 다 삭제 시켜주고...
//		   마지막으로 현재 다운로드를 관리하는 DownLoadMgrThread 를 종료 시킴
//		   다이얼로그 닫는 것...
//	Date : 2003. 06. 10
//	Update : 2003. 09. 06
//--------------------------------------------------------------------------------------------
void CPatchDlg::EndProcess()
{	
	KillTimer( 0 );

	if ( m_pHttpMgr )
	{
		delete m_pHttpMgr;
		m_pHttpMgr = NULL;
	}

	if ( m_pRegistryMgr )
	{
		delete m_pRegistryMgr;
		m_pRegistryMgr = NULL;
	}

	if ( m_hTitleBmp )
	{
		DeleteObject( m_hTitleBmp );
		m_hTitleBmp = NULL;
	}


	if ( m_pFront )
	{
		if ( m_pFront->m_pFrontSession )
		{
			m_pFront->m_pFrontSession->CloseSocket();
		}

		delete m_pFront;
		m_pFront = NULL;
	}

	EndDialog( IDOK );
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::WinHelp(DWORD dwData, UINT nCmd)
//	Desc : F1 Key 눌렀을 때, 해 주어야 하는 것.
//	Date : 2003. 06. 10
//	Update : 2003. 06. 18 - 주석 수정
//--------------------------------------------------------------------------------------------
void CPatchDlg::WinHelp( DWORD dwData, UINT nCmd )
{
	// TODO: Add your specialized code here and/or call the base class
	
	//AfxMessageBox("문의 사항이 있으시면 이상민(duryzip@hotmail.com) 으로 전화 주십시오.\nIf you have a question, E-mail to duryzip@hotmail.com");
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Kang Hyoungm Lee(riverbos@ndoors.net) - bool CPatchDlg::GetGameInfo()
//	Desc : 버젼정보를 얻어온다.
//	Date : 2007. 03. 21
//	Update :
//--------------------------------------------------------------------------------------------
bool CPatchDlg::GetGameInfo()
{
	CFile classFile;
	if ( classFile.Open( "./log.txt", CFile::modeCreate|CFile::modeWrite ) )
	{
		CHAR szLog[] = "게임인포\n";
		classFile.Write( szLog, sizeof(szLog) );
		classFile.Close();
	}

	m_uiCurrentVersion = 555 ; //GetGameVersionInFile();	// 게임클라이언트 버전을 지정 한다 - 설정내용
	if(m_uiCurrentVersion <= 0)
	{
		m_uiCurrentVersion = 0;
	}

	// 런처의 실행폴더를 게임 실행 폴더로 바꾸어준다.
	char FilePathBuffer[ MAX_PATH ] = "";
	::GetModuleFileName( NULL, FilePathBuffer, sizeof( FilePathBuffer ) );

	char Drive[_MAX_DRIVE];
	char Path[_MAX_PATH];
	char Filename[_MAX_FNAME];
	char Ext[_MAX_EXT];

	::_splitpath( FilePathBuffer, Drive, Path, Filename, Ext );

	char PathBuffer[ MAX_PATH ] = "";
	strcpy( PathBuffer, Drive );
	strcat( PathBuffer, Path );

	strcpy( m_strGameFolderPath, PathBuffer );

	// Patch 프로그램의 폴더를 기준으로 만든다.
	// 중국버젼은 임시로 Goonzu.exe 를 사용하였음.
	switch ( m_siServiceArea )
	{
	case SA_JPN:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Kunshu.exe");
		break;
	case SA_ENG:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Goonzu.exe");
		break;
	case SA_CHA:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Goonzu.exe");
		break;
	case SA_KOR:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Goonzu.exe");
		break;
	case SA_TAI:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Goonzu.exe");
		break;
	case SA_NHNCHA:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Goonzu.exe");
		break;
	case SA_USA:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Luminary.exe");
		break;
	case SA_EUROPE:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Luminary.exe");
		break;
	case SA_KOREA_NEW:
		strcpy(m_strGameFolderPath, PathBuffer);
		strcpy(m_strGameExeFileName, "Goonzu.exe");
	default:
		return false;
	}

	strcpy( m_strGameFullPath, m_strGameFolderPath );
	strcat( m_strGameFullPath, m_strGameExeFileName );

	strcpy( m_strPatchFolderPath, m_strGameFolderPath );
	strcat( m_strPatchFolderPath, PATCH_FOLDER);

    return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CPatchDlg::GetRegistryInfo()
//	Desc : 레지스트리 정보를 얻어 온다.
//		   레지스트리 정보를 얻어와서 필요한 데이터들을 저장해 두고, 관련 데이터를 설정해 준다.
//		   폴더 경로도 설정해줌.(SetCurrentDirectory)
//	Date : 2003. 06. 23
//	Update :
//--------------------------------------------------------------------------------------------
bool CPatchDlg::GetRegistryInfo()
{
	m_pRegistryMgr = new CRegistryMgr;

	CString		RegistryPath;

	switch ( m_siServiceArea )
	{
	case SA_KOR:
		RegistryPath = "Software\\NDOORS\\GoonZuS";
		break;
	case SA_ENG:
		RegistryPath = "Software\\NDOORS\\GoonZuEng";
		break;
	case SA_CHA:
		RegistryPath = "Software\\NDOORS\\GOONZUCHINA";
		break;
	case SA_NETMARBLE:
		RegistryPath = "Software\\NDOORS\\GOONZUNETMARBLE";
		break;
	case SA_TESTG:
		RegistryPath = "Software\\NDOORS\\TESTGOONZU";
		break;
	case SA_JPN:
#ifdef	_JAPAN_USER_TEST
		RegistryPath = "Software\\GamePot\\Kunshu User Test";
#else
		RegistryPath = "Software\\GamePot\\Kunshu Online";
#endif
		break;
	case SA_TAI:
		RegistryPath = "Software\\NDOORS\\GoonzuTaiwan";
		break;
	case SA_NHNCHA:
		RegistryPath = "Software\\NDOORS\\GoonzuNHNChina";
		break;
	case SA_USA:
		RegistryPath = "Software\\NDOORS\\Luminary";
		break;
	case SA_EUROPE:
		RegistryPath = "Software\\NDOORS\\GoonZuEurope";
		break;
	case SA_KOREA_NEW:
		RegistryPath = "Software\\NDOORS\\GoonZuS";
		break;
	}

	switch(m_siRunMode)
	{
	case RUNMODE_DEBUG:		RegistryPath += "_debug";			break;
	case RUNMODE_TEST:		RegistryPath += "_test";			break;
	case RUNMODE_BETA:		RegistryPath += "_beta";			break;
	case RUNMODE_REAL:											break;
	}

	if ( ! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, RegistryPath, KEY_ALL_ACCESS ) )
	{
		return false;
	}
	bool	exist_registry = true;

	if ( ! m_pRegistryMgr->GetRegReadInt( "VERSION", &m_uiCurrentVersion ) )
	{
		m_uiCurrentVersion	= 0;
		exist_registry = false;
		return false;
	}

	if ( !exist_registry || !m_pRegistryMgr->GetRegReadString( "PATH", m_strGameFolderPath, 1024 ) )
	{
		m_strGameFolderPath[0] = '\0';
		exist_registry = false;
		return false;
	}

	if ( !exist_registry || !m_pRegistryMgr->GetRegReadString( "FILENAME", m_strGameExeFileName, 128 ) )
	{
		m_strGameExeFileName[0] = '\0';
		exist_registry = false;
		return false;
	}

	
	if ( SA_KOREA_NEW == m_siServiceArea )
	{
		if ( (0 < m_uiCurrentVersion) && (465 > m_uiCurrentVersion) )
		{
			return false;
		}
	}

	// 런처의 실행폴더를 게임 실행 폴더로 바꾸어준다.
	char FilePathBuffer[ MAX_PATH ] = "";
	::GetModuleFileName( NULL, FilePathBuffer, sizeof( FilePathBuffer ) );

	char Drive[_MAX_DRIVE];
	char Path[_MAX_PATH];
	char Filename[_MAX_FNAME];
	char Ext[_MAX_EXT];

	::_splitpath( FilePathBuffer, Drive, Path, Filename, Ext );

	char PathBuffer[ MAX_PATH ] = "";
	strcpy( PathBuffer, Drive );
	strcat( PathBuffer, Path );

	char GameExe[MAX_PATH];
	strcpy(GameExe, PathBuffer);


	// 일본, 글로벌, 중국 버젼은 레지스트리가 없는경우 실행 경로를 강제로 생성한다.
	// 레지스트리에 저장하지는 않는다. (게임프로그램에서 자신의 경로를 한번 저장해 준다.)
	// Patch 프로그램의 폴더를 기준으로 만든다.
	// 중국버젼은 임시로 Goonzu.exe 를 사용하였음.
	/*
	if(!exist_registry)
	{
		switch ( m_siServiceArea )
		{
		case SA_JPN:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Kunshu.exe");
			break;
		case SA_ENG:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Goonzu.exe");
			break;
		case SA_CHA:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Goonzu.exe");
			break;			
		case SA_TAI:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Goonzu.exe");
			break;
		case SA_USA:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Luminary.exe");
			break;
		case SA_EUROPE:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Luminary.exe");
			break;
		case SA_KOREA_NEW:
			strcpy(m_strGameFolderPath, PathBuffer);
			strcpy(m_strGameExeFileName, "Goonzu.exe");
			break;
		default:
			return false;
		}
	}
	*/

	 ::SetCurrentDirectory( m_strGameFolderPath );

	strcpy( m_strGameFullPath, m_strGameFolderPath );
	strcat( m_strGameFullPath, m_strGameExeFileName );

	strcpy( m_strPatchFolderPath, m_strGameFolderPath );
	strcat( m_strPatchFolderPath, PATCH_FOLDER);

	// 패치파일을 받는 폴더 생성
	CreateDirectory( m_strPatchFolderPath, NULL);

	/*
	CFile classFile;
	if ( classFile.Open( "./log.txt", CFile::modeCreate|CFile::modeWrite ) )
	{
		CHAR szLog[1024] = "";

		StringCchCopy( szLog, 1024, "GameFullPath : " );
		classFile.Write( szLog, strlen(szLog) );
		classFile.Write( m_strGameFullPath, strlen(m_strGameFullPath) );

		StringCchCopy( szLog, 1024, "\r\nPathFolder : " );
		classFile.Write( szLog, strlen(szLog) );
		classFile.Write( m_strPatchFolderPath, strlen(m_strGameFullPath) );

		StringCchPrintf(  szLog, 1024, "\r\nGameVersion : %d", m_uiCurrentVersion );
		classFile.Write( szLog, strlen(szLog) );

		classFile.Close();
	}
	*/


	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CPatchDlg::SetNewDownLoadPathCount( US16 Count )
//	Desc : Count 갯수만큼 Ftp 객체 생성하고, Object 생성한다.
//	Date : 2003. 06. 10
//	Update : 2003. 06. 18 - 주석 수정, Ftp 기본 경로(m_strFtpBasePath) 넣어 줌.
//--------------------------------------------------------------------------------------------
bool CPatchDlg::SetNewDownLoadPathCount(  sResponse_PatchInfo * pRecvMsg, bool bUseSecondAddr)
{
	char szText[1024]	= "패치파일이 너무 많습니다..\r군주Speical 홈페이지에서 다운로드 받아 다시 설치하십시요.\r홈페이지로 이동하시겠습니까?";
	char szTitle[1024]	= "군주Special";

	if ( pRecvMsg->uiCount > MAX_DOWNLOAD_PATCH_FILE_NUM )				// 이 경우 웹에서 전체 설치 받아서 다운 받기를 권고 함.
	{
		if ( MessageBox( szText, szTitle, MB_ICONQUESTION | MB_YESNO ) == IDYES )
		{
			switch( m_siServiceArea )
			{
			case SA_KOR:
				::ShellExecute( NULL, NULL, "http://gzs.hangame.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_ENG:
				::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_CHA:
				::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_NETMARBLE:
				::ShellExecute( NULL, NULL, "http://www.netmarble.net", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_TESTG:
				::ShellExecute( NULL, NULL, "http://www.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_JPN:
				::ShellExecute( NULL, NULL, "http://www.kunshu.jp", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_TAI:
				::ShellExecute( NULL, NULL, "http://goonzu.omg.com.tw", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_NHNCHA:  // 임시  - TAI 나중에 
				//::ShellExecute( NULL, NULL, "http://goonzu.omg.com.tw", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_USA:
				::ShellExecute( NULL, NULL, "http://luminary.ijji.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_EUROPE:
				::ShellExecute( NULL, NULL, "http://luminary.aeriagames.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_KOREA_NEW:
				::ShellExecute( NULL, NULL, "http://goonzus.ndoors.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			}
			return false;
		}
	}

    char strBaseURL[ _MAX_PATH ];
	SI32 siBaseURLLen = 0;

	// 기본 주소만 가지고 DNS체크를 한다.
	int nFind = 0;
	char* pFind = strstr(pRecvMsg->szAddr, "/");
	if(pFind != NULL)
	{
		nFind = pFind - pRecvMsg->szAddr;
	}
	
	char szBaseAddr[128];
	strcpy(szBaseAddr, pRecvMsg->szAddr);

	if(nFind > 0)
	{
		memset(szBaseAddr, 0, sizeof(szBaseAddr));
		memcpy(szBaseAddr, pRecvMsg->szAddr, nFind);
	}

	//if (IsCorrectDNS(pRecvMsg->szAddr))
	if (IsCorrectDNS(szBaseAddr))
	{
		strcpy ( strBaseURL, pRecvMsg->szAddr );
		siBaseURLLen = strlen ( strBaseURL );

		if ( strBaseURL[ siBaseURLLen - 1 ] != '/' )
		{
			strcat( strBaseURL, "/" );
		}

		strcat( strBaseURL, pRecvMsg->szPath );
	}
	else
	{
		if(bUseSecondAddr)
		{
			strcpy ( strBaseURL, pRecvMsg->szAddr_2 );
			siBaseURLLen = strlen ( strBaseURL );

			if ( strBaseURL[ siBaseURLLen - 1 ] != '/' )
			{
				strcat( strBaseURL, "/" );
			}

			strcat( strBaseURL, pRecvMsg->szPath_2 );
		}
		else
			return false;
	}

	siBaseURLLen = strlen ( strBaseURL );
	
	if ( strBaseURL[ siBaseURLLen - 1 ] != '/' )
	{
		strcat( strBaseURL, "/" );
	}

	//m_pHttpMgr->SetBasePath( m_strGameFolderPath, strBaseURL );
	m_pHttpMgr->SetBasePath( m_strPatchFolderPath, strBaseURL );

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::DownLoadThreadStart()
//	Desc : 다운로드 시작.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::DownLoadThreadStart()
{
	UI32 uiDownLoadMgrThreadID = 0;
	m_bDownLoadMgrThreadRun = true;
	m_hDownLoadMgrThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &DownLoadMgrThread, this, 0, &uiDownLoadMgrThreadID );
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::DownLoadThreadStop()
//	Desc : 다운로드 종료.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::DownLoadThreadStop()
{
	m_pHttpMgr->FileRecvStop();
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::DownLoadMgrRun()
//	Desc : 실제 다운로드 관리를 하는 부분
//		   다운로드 상태를 모두 확인 후에, 그 상태를 그려주는 함수(DrawDownLoadStatus) 호출함.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::DownLoadMgrThreadRun()
{
	return;
}

void CPatchDlg::Drawing()
{
	if ( m_ucCurrentStatus == DLG_DOWNLOAD_STATUS )
	{
		DrawDownLoadStatus();
	}
	else if ( DLG_GAMESTART_STATUS == m_ucCurrentStatus)
	{
		m_DownLoadProgress2.SetPos( 100 );

		if ( ! m_bButtonClick )
		{
			switch ( m_ucCurrentStatus )
			{
			case DLG_INITIAL_STATUS:
			case DLG_SOCKET_STATUS:
			case DLG_DOWNLOAD_STATUS:
				break;
			
			case DLG_GAMESTART_STATUS:
				break;
			default:
				break;
			}
		}
	}
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::DrawDownLoadStatus()
//	Desc : 현재 다운로드 상태를 화면에 보여주기 위해서...
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::DrawDownLoadStatus()
{
	if ( m_dwTotalFileSize < 1 )
	{
#ifdef _DEBUG
		AfxMessageBox( "m_dwTotalFilesize < 1 이게 나옴." );
#endif
		return;
	}

	DWORD dwRecvFileSize = m_pHttpMgr->GetFileRecvTotalSize();

	int percent = ( ( dwRecvFileSize * 1.0 ) / m_dwTotalFileSize * 100 );

	if ( percent > 100 )
	{
		percent = 100;
	}

	if ( percent < 0 )
	{
		percent = 0;
	}
	
	if ( dwRecvFileSize > 0 )
	{
		DWORD dwOneFileSize = m_dwTotalFileSize/m_usCount;
		m_DownLoadProgress2.SetPos( percent );
	}
	

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::Decompress()
//	Desc : 쓰레드 종료 후 압축 해제 후 게임 실행
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::Decompress()
{
	DownLoadThreadStop();
	char *pText= GetTxtFromMgr(9);
	char *pTitle= GetTxtFromMgr(1);;


	//이 부분에 압축 해제 코드 추가.

	int i = 0;

	
	for ( i = 0; i < m_usCount; ++ i )
	{
		WIN32_FIND_DATA wfd;
		HANDLE hSrch = ::FindFirstFile( m_pHttpMgr->GetLocalFileNamePath( i ), &wfd );

		if ( hSrch == INVALID_HANDLE_VALUE )
		{
			MessageBox(pText,pTitle, MB_ICONQUESTION | MB_OK );
			EndProcess();
			return;
		}
		else
		{
			ZipFileDecompress( m_pHttpMgr->GetLocalFileNamePath( i ) );
			::DeleteFile( m_pHttpMgr->GetLocalFileNamePath( i ) );
			::FindClose( hSrch );
		}
	}
	

	//m_pRegistryMgr->SetRegWriteInt( "VERSION", m_pHttpMgr->GetLastVersion() );

	m_ButtonStart.EnableWindow( TRUE );

	// "Download" 폴더를 삭제한다. - 남아있는 패치파일을 모두 삭제한다.
	::RemoveDirectory( m_strPatchFolderPath );
	
	return;
}


int CPatchDlg::ZipFileDecompress( char* zipfilename )
{	
	const char *filename_to_extract=NULL;
	char *pText=GetTxtFromMgr(10);
	
	int opt_do_extract_withoutpath=NULL;
	int opt_overwrite=1;
	
	unzFile uf = NULL;

	char filename_try[512] = "";
	
	if ( zipfilename != NULL )
	{		
		strcpy( filename_try, zipfilename );	
		uf = unzOpen( zipfilename );		
		if ( uf==NULL )
		{
			uf = unzOpen( filename_try );
			if ( uf == NULL )
			{
				MessageBox( pText, filename_try, MB_OK );
			}
		}
	}
	
	do_extract(uf,opt_do_extract_withoutpath,opt_overwrite);
	unzClose( uf );
	return 0;
}

// zip 파일내에 있는 파일들 리스트를 얻어온다
int CPatchDlg::GetZipFileLise( char* zipfilename )
{
	unzFile uf=NULL;

	char filename_try[512] = "";
	char *pText=  GetTxtFromMgr(11);
	
	if ( zipfilename != NULL )
	{		
		strcpy( filename_try, zipfilename );	
		uf = unzOpen( zipfilename );		
		if ( uf==NULL )
		{
			uf = unzOpen( filename_try );
			if ( uf == NULL )
			{
				MessageBox(pText, filename_try, MB_OK );
			}
		}
	}

	return do_list(uf);
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::GameStart()
//	Desc : 패치 할 필요가 없거나, 패치가 끝 났을 시에 이 함수를 호출하여 게임을 실행함.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::GameStart()
{	
	KillTimer( 0 );

	// 군주 숏컷 만들기 추가
	//if ( m_siServiceArea == SA_KOREA_NEW )
	{
		CoInitialize(0);

		//
		CreateGoonZuShourCut();
		//

		CoUninitialize();
	}
	//

	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;

	si.cb = sizeof( STARTUPINFO );
	si.dwFlags = 0;
	if( m_bUSAAuth )
	{
		TCHAR szParameter[4096];
		ZeroMemory(szParameter, sizeof(szParameter));
		strcat(szParameter, "\"");
		strcat(szParameter, m_strGameFullPath);
		strcat(szParameter, "\" ");
		strcat(szParameter, m_szUSAAuth_String);

		::CreateProcess( m_strGameFullPath, szParameter, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
	}
	else
	{
		::CreateProcess( m_strGameFullPath, m_strParameter, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
	}

	//AfxMessageBox( m_strGameFullPath );
	EndProcess();
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::OnTimer(UINT nIDEvent) 
//	Desc : 네트워크를 담당하는 run 함수를 호출
//	Date : 2003. 08. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnTimer(UINT nIDEvent) 
{
	// 건들면 안되겠다 '';
	// TODO: Add your message handler code here and/or call default
	static clock_t ClockTemp = clock();
	
	if ( m_ucCurrentStatus == DLG_SOCKET_STATUS )
	{
		double duration = (double)( clock() - m_StartClock ) / CLOCKS_PER_SEC;
		if ( duration < DLG_SOCKET_STATUS_LIFETIME )							//	[종호] 현재 상태가 DLG_SOCKET_STATUS로 제한시간을 넘기면 패치를 종료한다.
		{
			m_pFront->Run();
		}
		else
		{
			KillTimer( 0 );

			FILE * fp = NULL ;
			char szFileName[ MAX_PATH ] = "" ;
			char str[ MAX_LENGTH ] = "" ;	

			strcpy( szFileName,"ServiceArea.txt" );

			fp = fopen(szFileName,"r");
			if ( fp == NULL )				return;

			fgets( str, MAX_LENGTH, fp );
			fclose( fp );
			fp = NULL;

			char *pText=  GetTxtFromMgr(31);
			MessageBox("접속에 실패 하였습니다. 다시 실행해 주시기 바랍니다.", "Notify Message", MB_OK );

			EndDialog(IDOK);
		}
	}
	else if (  DLG_GAMESTART_STATUS == m_ucCurrentStatus || DLG_DOWNLOAD_STATUS == m_ucCurrentStatus )
	{
		// 다운로드가 완료되었다.
		if ( m_pHttpMgr->GetDownLoadStatus() == DOWNLOAD_COMPLETE_STATUS )
		{
//KHY - 0316 - NHNChina Outbound - 군주중국은 여기서 끝낸다.
#ifndef NHNPATCH_HIDEDLG 
#ifdef USE_HANREPORT
			HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_PATCH_DOWNLOAD_END);
#endif //USE_HANREPORT
#ifdef USE_CNREPORT
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(m_szPorteAuth_UserID, HANREPORT_PATCH_DOWNLOAD_END);
#endif

			m_DownLoadProgress2.SetPos( 100 );

			char* pText = GetTxtFromMgr(23);
			DrawInstallStatus( pText );

#ifdef USE_HANREPORT
			HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_PATCH_UNZIP_START);
#endif //USE_HANREPORT
#ifdef USE_CNREPORT
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(m_szPorteAuth_UserID, HANREPORT_PATCH_UNZIP_START);
#endif

			Decompress();

#ifdef USE_HANREPORT
			HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_PATCH_UNZIP_END);
#endif //USE_HANREPORT
#ifdef USE_CNREPORT
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(m_szPorteAuth_UserID, HANREPORT_PATCH_UNZIP_END);
#endif

			pText = GetTxtFromMgr(24);
			DrawInstallStatus( pText );

			m_ucCurrentStatus = DLG_GAMESTART_STATUS;
#else
#ifdef USE_CNREPORT
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(m_szPorteAuth_UserID, HANREPORT_PATCH_DOWNLOAD_END);
#endif

			char* pText = GetTxtFromMgr(23);
			// 다운로드가 완료된 이후에 알림창을 바꾼다. 
			SendMessageToPurple(PURPLE_OUTBOUND_INFORM, pText, 0);
			//DrawInstallStatus( pText );
#ifdef USE_CNREPORT
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(m_szPorteAuth_UserID, HANREPORT_PATCH_UNZIP_START);
#endif

			Decompress();

#ifdef USE_CNREPORT
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(m_szPorteAuth_UserID, HANREPORT_PATCH_UNZIP_END);
#endif

			pText = GetTxtFromMgr(24);
			//DrawInstallStatus( pText );
			SendMessageToPurple(PURPLE_OUTBOUND_INFORM, pText, 0);

			// 패치가 다 끝났다는 메시지를 보낸다. 
			SendMessageToPurple(PURPLE_OUTBOUND_ENDING, "", 0);

			Sleep(500);
			//m_ucCurrentStatus = DLG_GAMESTART_STATUS;
			EndProcess();
#endif // NHNPATCH_HIDEDLG 
		}
		else
		{
			Drawing();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::FrontMsgProcess( sPacketHeader *pPacket )
//	Desc : 받은 패킷 처리
//	Date : 2003. 08. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::FrontMsgProcess( sPacketHeader *pPacket )
{
	switch( pPacket->usCmd ) 
	{
	case FRONTMSG_RESPONSE_SECRETKEY:
		{
			WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Recv FRONTMSG_RESPONSE_SECRETKEY");

			sResponse_SecretKey *pRecvMsg = (sResponse_SecretKey *)pPacket;

			FrontProcess *pFP = m_pFront;
			ITZSession *pSession = pFP->m_pFrontSession;
			
			pSession->GetEncrypt()->SetKey( pRecvMsg->ucKey1, pRecvMsg->ucKey2 );

			InterlockedExchange( (LPLONG)&pFP->m_bSecretKey, TRUE );
		}
		break;
		
	case FRONTMSG_RESPONSE_PATCHINFO:
		{
			WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Recv FRONTMSG_RESPONSE_PATCHINFO");

			sResponse_PatchInfo *pRecvMsg = (sResponse_PatchInfo *)pPacket;

			int  packetsize = sizeof(sResponse_PatchInfo);

			bool	bUseIP = true;
			if(pPacket->usSize < sizeof(sResponse_PatchInfo))
				bUseIP = false;
			else
				bUseIP = true;
			
			// 버전 정보를 받았음을 설정
			m_pFront->m_bVersion = TRUE;

			WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "pRecvMsg->uiCount=%d", pRecvMsg->uiCount);

			if ( pRecvMsg->uiCount == 0 )
			{
				// 프론트 서버와의 접속을 끊어준다
				KillTimer( 0 );				
				m_pFront->m_pFrontSession->CloseSocket();

				m_ButtonStart.EnableWindow( TRUE );
				break;
			}
			
			m_pHttpMgr = new CHttpMgr( pRecvMsg->uiCount );
			m_pHttpMgr->Connect();
			
			m_puiVersionList = new UI32[ pRecvMsg->uiCount ];

			memset( m_puiVersionList, 0, sizeof( UI32 ) * pRecvMsg->uiCount );
			memcpy( m_puiVersionList, pRecvMsg->uiVersionHistory, sizeof( UI32 ) * pRecvMsg->uiCount );

			if (! SetNewDownLoadPathCount( pRecvMsg, bUseIP ) )
			{
				WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "! SetNewDownLoadPathCount( pRecvMsg, bUseIP )");

				// 프론트 서버와의 접속을 끊어준다
				KillTimer( 0 );
				m_pFront->m_pFrontSession->CloseSocket();
				EndProcess();

				FILE * fp = NULL ;
				char szFileName[ MAX_PATH ] = "" ;
				char str[ MAX_LENGTH ] = "" ;	

				return;
			}
			
			char temp[ 32 ];
			m_usCount = pRecvMsg->uiCount;
			for ( UI32 i = 0; i < pRecvMsg->uiCount; ++i )
			{
				memset( temp, 0, 32 );
				itoa( m_puiVersionList[ i ], temp, 10 );

				m_pHttpMgr->Push( m_puiVersionList[ i ], temp );
			}

			m_dwTotalFileSize = m_pHttpMgr->GetFileTotalSize();

			if ( m_dwTotalFileSize < 1 )
			{
				KillTimer( 0 );
				m_pFront->m_pFrontSession->CloseSocket();
				EndProcess();

				return;
			}

			KillTimer( 0 );
			SetTimer( 0, 100, NULL );
			
			char *pText = GetTxtFromMgr(26);
			if ( pText )
			{
				DrawInstallStatus( pText );
			}			
			m_pHttpMgr->DownLoadStart();
			WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "m_pHttpMgr->DownLoadStart()");

			m_ucCurrentStatus = DLG_DOWNLOAD_STATUS;

			m_pFront->m_pFrontSession->CloseSocket();
		}
		break;
	}
}

HBRUSH CPatchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here

	CBrush tmpBrush;
	if (nCtlColor == CTLCOLOR_STATIC ) {
		tmpBrush.CreateStockObject(NULL_BRUSH);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH) tmpBrush;
	}

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPatchDlg::OnClose() 
{
	if ( m_ucCurrentStatus == DLG_GAMESTART_STATUS )
	{
		KillTimer( 0 );		

		// TODO: Add your message handler code here and/or call default
		if ( m_bDownLoadMgrThreadRun )
		{
			m_bDownLoadMgrThreadRun = false;
			::WaitForSingleObject( m_hDownLoadMgrThreadHandle, INFINITE );		
		}
		
		if( m_hDownLoadMgrThreadHandle != NULL )
		{
			::CloseHandle( m_hDownLoadMgrThreadHandle );
			m_hDownLoadMgrThreadHandle = NULL;
		}		
		
		EndProcess();
	}
	CDialog::OnClose();
}

void CPatchDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}

void CPatchDlg::DrawInstallStatus( char *pString )
{
	if( pString )
	{
		//m_StaticStatus.SetWindowText( pString );
		StringCchCopy( m_StatusTextBuffer, 256, pString);
		//m_StatusTextBuffer.SetString( pString, strlen(pString) );
		Invalidate( FALSE );
	}

	return;
}

UINT CPatchDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	ScreenToClient( &point );
	if ( point.x > 310 && point.x < 382 && point.y > 310 && point.y < 337 )
	{
		return CDialog::OnNcHitTest(point);
	}
	
	if ( point.x > 289 && point.x < 303 && point.y > 320 && point.y < 334 )
	{	
		return CDialog::OnNcHitTest(point);
	}

	UINT hit = CDialog::OnNcHitTest( point );

	if ( hit == HTCLIENT )
	{
		return HTCAPTION;
	}
	else
	{
		return hit;
	}
}

void CPatchDlg::OnBnClickedButtonRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GameStart();
}

void CPatchDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}


bool CPatchDlg::IsCorrectDNS(char* hostname)
{
	struct hostent *host = NULL;

	host = gethostbyname(hostname);
	if (host == NULL)
		return false;

	bool bAddrFind = false;
	
	for (int i = 0; host->h_addr_list[i]; i++)
	{
		bAddrFind = true;
	}

	return bAddrFind;

}
void CPatchDlg::HanAuth_Lib_HanReportSendPosLog_Client(TCHAR* pszID, TCHAR* pszValue)
{
	if(pszID == NULL)		return;
	if(pszValue == NULL)	return;

	TCHAR	szReportString[1024];
	StringCchPrintf(szReportString, sizeof(szReportString), "id=%s&value=%s", pszID, pszValue);

#ifdef USE_HANREPORT
	HanReportSendPosLog(szReportString);
#endif //USE_HANREPORT
}

//KHY - 0316 - NHNChina Outbound
#ifdef NHNPATCH_HIDEDLG 
void CPatchDlg::SendMessageToPurple(int nType, LPCTSTR lpszMsg, DWORD dwValue)
{
	if(!m_hTarget)
	{
		MessageBox("대상 HWND가 없습니다.");
		return;
	}

	// 퍼플런쳐로 넘길 구조체를 만든다.
	PurpleCDS pds;
	pds.dwIndex = dwValue;
	_tcscpy(pds.szString, lpszMsg);

	// WM_COPYDATA에 넘길 구조체를 구성한다. 
	COPYDATASTRUCT cd;
	cd.dwData = nType;
	cd.cbData = sizeof(pds);
	cd.lpData = &pds;

	LRESULT r = ::SendMessage(m_hTarget, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&cd);

	if( r == 0 )
	{
		MessageBox("퍼플런쳐가 사라졌습니다. 개발사런쳐를 종료합니다.");
		ExitProcess(0);
	}
}
#endif // NHNPATCH_HIDEDLG 

void CPatchDlg::HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(TCHAR* pszUserID, TCHAR* pszValue)//서버아이디/유저 아이디/ 밸류
{
	if(pszUserID == NULL)		return;
	if(pszValue == NULL)		return;
	TCHAR	szReportString[1024];
	{
		StringCchPrintf(szReportString, sizeof(szReportString), "svrno=%0.5d%&id=%s&value=%s", 0 ,pszUserID, pszValue);
	}
#ifdef USE_CNREPORT
	HanReportSendPosLog(szReportString);
#endif // USE_CNREPORT
}

void CPatchDlg::CreateGoonZuShourCut( void )
{
	char szExcuteFile[MAX_PATH]			= {0,};
	char szLinkFile[MAX_PATH]			= {0,};
	char szArgument[MAX_PATH]			= {0,};
	char szDesc[MAX_PATH]				= "군주 Special";
	char szWorkingDirectory[MAX_PATH]	= {0,};

	// 숏컷 만들 폴더를 설정하고
	GetSpecialFolderPath( szLinkFile, sizeof(szLinkFile), CSIDL_DESKTOP );

	size_t sizeLast = strlen(szLinkFile);
	if ( MAX_PATH <= sizeLast )
	{
		return;
	}

	if ( '\\' != szLinkFile[sizeLast-1] )
	{
		StringCchCat( szLinkFile, MAX_PATH, "\\" );
	}

	StringCchCat( szLinkFile, MAX_PATH, "군주 Special.lnk" );

	// 숏컷 내에서 쓰일 시작위치 설정
	if ( 0 == GetCurrentDirectory(MAX_PATH, szWorkingDirectory) )
	{
		return;
	}

	// 실행파일명
	StringCchCopy( szExcuteFile, MAX_PATH, szWorkingDirectory );

	StringCchLength( szExcuteFile, MAX_PATH, &sizeLast );

	if ( '\\' != szExcuteFile[sizeLast-1] )
	{
		StringCchCat( szExcuteFile, MAX_PATH, "\\" );
	}

	StringCchCat( szExcuteFile, MAX_PATH, "GoonZuRun.exe" );

	// 숏켯을 만든다
	CreateShortCut( szExcuteFile, szLinkFile, szArgument, szDesc, szWorkingDirectory );

}

void CPatchDlg::GetSpecialFolderPath( OUT char* szGetFolderPath, size_t nSize, int nFolder )
{
	if ( (NULL == szGetFolderPath) || (MAX_PATH != nSize) )
	{
		return;
	}

	LPITEMIDLIST	lpIIDList;
	LPMALLOC		lpMalloc;

	SHGetSpecialFolderLocation( HWND_DESKTOP, nFolder, &lpIIDList );
	SHGetPathFromIDList( lpIIDList, szGetFolderPath );

	SHGetMalloc( &lpMalloc );
	lpMalloc->Free( lpIIDList );
	lpMalloc->Release();

}

HRESULT CPatchDlg::CreateShortCut( char* szSrcFile, char* szLinkFile, char* szArgument, char* szDesc, char* szWorkingDirectory )
{
	if ( (NULL == szSrcFile) || (NULL == szLinkFile) || (NULL == szArgument) || (NULL == szDesc) || (NULL == szWorkingDirectory) )
	{
		return E_FAIL;
	}

	HRESULT			hrReturn;
	IShellLink*		pISellLink;
	IPersistFile*	pIPersistFile;
	WCHAR			wszLinkFile[MAX_PATH] = {0,};

	hrReturn = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pISellLink );
	if ( FAILED(hrReturn) )
	{
		return E_FAIL;
	}

	pISellLink->SetPath( szSrcFile );
	pISellLink->SetDescription( szDesc );
	pISellLink->SetArguments( szArgument );
	pISellLink->SetWorkingDirectory( szWorkingDirectory );

	hrReturn = pISellLink->QueryInterface( IID_IPersistFile, (void**)&pIPersistFile );
	if ( FAILED(hrReturn) )
	{
		pISellLink->Release();

		return E_FAIL;
	}

	MultiByteToWideChar( CP_ACP, 0, szLinkFile, -1, wszLinkFile, MAX_PATH );

	hrReturn = pIPersistFile->Save( wszLinkFile, TRUE );

	pIPersistFile->Release();
	pISellLink->Release();

	return hrReturn;

}