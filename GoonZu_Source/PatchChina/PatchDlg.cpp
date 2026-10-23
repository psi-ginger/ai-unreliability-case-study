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

#include "../NLib/NDataLoader.h"
#include "../NLib/NFile.h"
#include "../NLib/NUtil.h"

#define SA_KOR			100
#define SA_NETMARBLE	200
#define SA_ENG			300
#define SA_CHA			400
#define SA_TESTG		500
#define SA_JPN			600

#define PATCH_FOLDER	"Download\\"

const RECT CPatchDlg::CustomControlRect[RECT_END] =
{	
	{ HOMEPAGE_BUTTON_X, HOMEPAGE_BUTTON_Y, HOMEPAGE_BUTTON_X + HOMEPAGE_BUTTON_WIDTH, HOMEPAGE_BUTTON_Y +HOMEPAGE_BUTTON_HEIGHT },
	{ ACCOUNT_BUTTON_X, ACCOUNT_BUTTON_Y, ACCOUNT_BUTTON_X + ACCOUNT_BUTTON_WIDTH, ACCOUNT_BUTTON_Y + ACCOUNT_BUTTON_HEIGHT },
	{ CARD_BUTTON_X, CARD_BUTTON_Y, CARD_BUTTON_X + CARD_BUTTON_WIDTH, CARD_BUTTON_Y + CARD_BUTTON_HEIGHT },
	{ DOWN_BUTTON_X, DOWN_BUTTON_Y, DOWN_BUTTON_X + DOWN_BUTTON_WIDTH, DOWN_BUTTON_Y + DOWN_BUTTON_HEIGHT },
	{ NEXT_BUTTON_X, NEXT_BUTTON_Y, NEXT_BUTTON_X + NEXT_BUTTON_WIDTH, NEXT_BUTTON_Y + NEXT_BUTTON_HEIGHT },
	{ CHECK_BUTTON_X, CHECK_BUTTON_Y, CHECK_BUTTON_X + CHECK_BUTTON_WIDTH, CHECK_BUTTON_Y + CHECK_BUTTON_HEIGHT },
	{ EXIT_BUTTON_X, EXIT_BUTTON_Y, EXIT_BUTTON_X + EXIT_BUTTON_WIDTH, EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT },

	{ CNCIDC_BUTTON_X, CNCIDC_BUTTON_Y, CNCIDC_BUTTON_X + CNCIDC_BUTTON_WIDTH, CNCIDC_BUTTON_Y + CNCIDC_BUTTON_HEIGHT },
	{ CTCIDC_BUTTON_X, CTCIDC_BUTTON_Y, CTCIDC_BUTTON_X + CTCIDC_BUTTON_WIDTH, CTCIDC_BUTTON_Y + CTCIDC_BUTTON_HEIGHT },
	{ PINGTEST_BUTTON_X, PINGTEST_BUTTON_Y, PINGTEST_BUTTON_X + PINGTEST_BUTTON_WIDTH, PINGTEST_BUTTON_Y + PINGTEST_BUTTON_HEIGHT },
	{ START_BUTTON_X, START_BUTTON_Y, START_BUTTON_X + START_BUTTON_WIDTH, START_BUTTON_Y + START_BUTTON_HEIGHT },
	{ EXIT2_BUTTON_X, EXIT2_BUTTON_Y, EXIT2_BUTTON_X + EXIT2_BUTTON_WIDTH, EXIT2_BUTTON_Y + EXIT2_BUTTON_HEIGHT },
};

char webURI[RECT_BUTTON_DOWNLOAD + 1][255] = { '\0' };

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

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );

	if( err != 0 ) {
		return;
	}

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

	srand((unsigned)time( NULL ));

	WinSockInit();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pFront = NULL;

	m_pCNCFront = NULL;
	m_pCTCFront = NULL;

	m_pHttpMgr = NULL;
	m_pRegistryMgr = NULL;
	m_puiVersionList = NULL;

	m_pFront = new FrontProcess;

	m_ucCurrentStatus = DLG_INITIAL_STATUS;	

	StringCchCopy( m_StatusTextBuffer, 256, "");

	m_ProgressBitmap.LoadBitmap( IDB_BITMAPBAR );

	m_szPatchVersionInfo[0] = '\0';
	m_szState[0] = '\0';
	ZeroMemory( m_szPingNumber, sizeof(m_szPingNumber) );
	m_szLastServerName[0] = '\0';
	m_siPage = FIRST_PAGE;

	ZeroMemory( m_szIDC_IP, sizeof(m_szIDC_IP) );
	ZeroMemory( m_szIDC_Name, sizeof(m_szIDC_Name) );

	m_siMaxCNCIDC_Number = 0;
	m_siMaxCTCIDC_Number = 0;

	m_IDCBitmapClick[CNCIDC] = NULL;
	m_IDCBitmapClick[CTCIDC] = NULL;

	m_IDCBitmapDisable[CNCIDC] = NULL;
	m_IDCBitmapDisable[CTCIDC] = NULL;

	int i;
	for( i = 0; i < MAX_FRONTSERVER_NUMBER; ++i )
	{
		int x = 14 + ( ( i % 3 ) * 8 ) + ( ( i % 3 ) * 206 );
		int y = 79 + ( ( i / 3 ) * 4 ) + ( ( i / 3 ) * 54 );
		int width = x + 206;
		int height = y + 54;
		AreaControlRect[i].left = x;
		AreaControlRect[i].top = y;
		AreaControlRect[i].right = width;
		AreaControlRect[i].bottom = height;

		m_bAreaClick[i] = false;
		m_bAreaHover[i] = false;
	}

	m_siSelectIDC = 0;
	m_siSelectFrontServerIndex = 0;

	m_siCDNSelect = 0;
	m_siPrevAreaButtonIdex = 0;

	m_bFirstConnect = false;
	m_bFailPatchInfo = false;
	m_siWhereConnect = CTCIDC;
	m_siCNCConnectIndex = 0;
	m_siCTCConnectIndex = 0;

	FILE* fp = _tfopen( "PatchInfo/Option.txt", "r" );

	if( fp == NULL )
	{
		m_bFirstConnect = true;
		return;
	}

	_ftscanf( fp, "%d", &m_siCDNSelect );
	_ftscanf( fp, "%d", &m_siPrevAreaButtonIdex );

	if( 0 > m_siCDNSelect )
		m_siCDNSelect= 0;

	if( 0 > m_siPrevAreaButtonIdex )
		m_siPrevAreaButtonIdex= 0;

	fclose( fp );
}

void CPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchDlg)
	DDX_Control(pDX, IDC_EXPLORER_PATCH_CONTENT_VIEWER, m_WebBrowser);
	//}}AFX_DATA_MAP
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
	ON_WM_LBUTTONUP()
	//	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	//	ON_BN_CLICKED(IDC_BUTTON_RUN, OnBnClickedButtonRun)
	//	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
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

	char *argv, *temp;
	temp = argv = GetCommandLine(); 

	int c = 0;
	while( c < 2 && *argv ) {
		if( *argv == '\"' ) ++c;
		++argv;
	}

	if( c < 2 ) argv = temp;

	if( argv ) {
		if( *argv == ' ' ) ++argv;
		if( argv ) strcpy( m_strParameter, argv );
		else m_strParameter[ 0 ] = NULL;
	} else m_strParameter[ 0 ] = NULL;

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

	::SetCurrentDirectory( PathBuffer );	

	FILE * fp = NULL ;
	char szFileName[ MAX_PATH ] = "" ;
	char Buffer[ MAX_PATH ] = {'\0'};
	char str[ MAX_LENGTH ] = {'\0'};


	strcpy( szFileName,"ServiceArea.txt" );

	fp = fopen(szFileName,"r");
	if ( fp == NULL )
		return TRUE;

	fgets( str, MAX_LENGTH, fp );
	sscanf( str, "%s", str );
	fclose( fp );

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
	else if ( strcmp( str, "SERVICEAREA_TESTGOONZU" ) == 0 )
	{
		m_siServiceArea = SA_TESTG;
	}
	else if ( strcmp( str, "SERVICEAREA_JAPAN" ) == 0 )
	{
		m_siServiceArea = SA_JPN;
	}

	sprintf( Buffer, "PatchInfo/PatchBaseInfo%s" ,str );

	fp = fopen(Buffer, "rt" );

	if ( fp != NULL )
	{
		char PatchTextURL[ 128 ] = "";
		fscanf( fp, "%s", PatchTextURL );
		fclose( fp );

		COleVariant* pURL = NULL;
		// LEEKH DUMP CODE - 수정필요 -> 모든 버젼이 데이터를 읽는 것으로 수정필요
		if(m_siServiceArea == SA_JPN)
		{
			pURL = new COleVariant( "http://www.kunshu.jp/update/patchwindow.html" ); 
		}
		else
		{
			pURL = new COleVariant( "http://pub.hljz.net/index.html" );
		}
		COleVariant* pEmpty = new COleVariant;

		if(pURL != NULL && pEmpty != NULL)
		{
			if( FIRST_PAGE == m_siPage )
				m_WebBrowser.Navigate2( pURL, pEmpty, pEmpty, pEmpty, pEmpty );
		}

		delete pURL;
		delete pEmpty;
	}

	HBITMAP hBMP;
	sprintf( Buffer, "PatchInfo/PatchTitle1.bmp" ,str );
	hBMP =  (HBITMAP)LoadImage(NULL, Buffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE );

	if ( hBMP ) 
	{	
		m_BackGroundBitmap[0].Attach( hBMP );

		m_BackGroundBrush[0].CreatePatternBrush( &m_BackGroundBitmap[0] );

		// 타이틀바등의 크기를 고려해서 윈도우 크기 조절
		RECT rtWindow, rtClient;
		GetWindowRect( &rtWindow);
		GetClientRect( &rtClient);
		int nWidth = ( rtWindow.right - rtWindow.left ) -  rtClient.right + 662;
		int nHeight = ( rtWindow.bottom - rtWindow.top ) - rtClient.bottom + 481;
		SetWindowPos( NULL, 199, 199, nWidth, nHeight, SWP_SHOWWINDOW
 );
	}

	sprintf( Buffer, "PatchInfo/PatchTitle2.bmp" ,str );
	hBMP =  (HBITMAP)LoadImage(NULL, Buffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE );

	if ( hBMP ) 
	{	
		m_BackGroundBitmap[1].Attach( hBMP );

		m_BackGroundBrush[1].CreatePatternBrush( &m_BackGroundBitmap[1] );
	}

	AddButtonImage( "PatchInfo/HomePage_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_HOMEPAGE] );
	AddButtonImage( "PatchInfo/Account_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_ACCOUNTREGISTER] );
	AddButtonImage( "PatchInfo/Card_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_CARD] );
	AddButtonImage( "PatchInfo/Down_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_DOWNLOAD] );
	AddButtonImage( "PatchInfo/Next_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_NEXT] );
	AddButtonImage( "PatchInfo/Check_C.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_CHECK] );
	AddButtonImage( "PatchInfo/Exit_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_EXIT] );

	AddButtonImage( "PatchInfo/CncIDC_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_CNCIDC] );
	AddButtonImage( "PatchInfo/CtcIDC_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_CTCIDC] );
	AddButtonImage( "PatchInfo/Ping_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_PINGTEST] );
	AddButtonImage( "PatchInfo/Start_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_START] );
	AddButtonImage( "PatchInfo/Exit_D.bmp", &m_CustomButtonBitmapClick[RECT_BUTTON_EXIT2] );

	

	AddButtonImage( "PatchInfo/HomePage_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_HOMEPAGE] );
	AddButtonImage( "PatchInfo/Account_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_ACCOUNTREGISTER] );
	AddButtonImage( "PatchInfo/Card_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_CARD] );
	AddButtonImage( "PatchInfo/Down_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_DOWNLOAD] );
	AddButtonImage( "PatchInfo/Next_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_NEXT] );
	AddButtonImage( "PatchInfo/Check_D.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_CHECK] );
	AddButtonImage( "PatchInfo/Exit_D.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_EXIT] );

	AddButtonImage( "PatchInfo/CncIDC_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_CNCIDC] );
	AddButtonImage( "PatchInfo/CtcIDC_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_CTCIDC] );
	AddButtonImage( "PatchInfo/Ping_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_PINGTEST] );
	AddButtonImage( "PatchInfo/Start_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_START] );
	AddButtonImage( "PatchInfo/Exit_C.bmp", &m_CustomButtonBitmapDisable[RECT_BUTTON_EXIT2] );

	g_pTextMgr.Create( str );

	TCHAR abuffer[1024];
	NFile file; 
	if(file.LoadFile( TEXT("ServerIP.dat") ) == false)
	{
		return FALSE;
	}

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{

		NLoadCryptFile( TEXT("ServerIP.dat"),  (BYTE*)abuffer );
		abuffer[ pHeader->dwDataSize ] = NULL;
	} 

	char buf[ 256 ] = {'\0'};
	_stscanf( abuffer, "%s", buf );

	char serverip1[ 256 ] = {'\0'};
	char serverip2[ 256 ] = {'\0'};

	strcpy( serverip1, buf );
	strcpy( serverip2, buf );

	fp = _tfopen( "PatchInfo/ServerIP_CNC.txt", "rb" );

	if( fp == NULL )
		return FALSE;

	char buffer[ 256 ] = {'\0'};
	char tempIP[ 64 ] = {'\0'};
	char tempName[ 32 ] = {'\0'};

	BOOL loop = TRUE;
	SI32 count = 0;
	while( loop )
	{
		_fgetts( buffer, 256, fp );
		_stscanf( buffer, "%s %s", tempIP, tempName );

		if( _tcscmp( "<end>", tempIP ) == 0 )
		{
			loop = FALSE;
			break;
		}

		_tcsncpy( m_szIDC_IP[CNCIDC][ count ], tempIP, 64 );
		_tcsncpy( m_szIDC_Name[ CNCIDC ][ count ], tempName, 32 );

		count++;
	}

	fclose( fp );

	m_siMaxCNCIDC_Number = count;
	m_pCNCFront = new FrontProcess[m_siMaxCNCIDC_Number];
	m_IDCBitmapClick[CNCIDC] = new CBitmap[m_siMaxCNCIDC_Number];
	m_IDCBitmapDisable[CNCIDC] = new CBitmap[m_siMaxCNCIDC_Number];

	count = 0;
	fp = _tfopen( "PatchInfo/ServerIP_CTC.txt", "rb" );

	if( fp == NULL )
		return FALSE;

	buffer[ 0 ] = '\0';
	tempIP[ 0 ] = '\0';
	tempName[ 0 ] = '\0';

	loop = TRUE;
	while( loop )
	{
		_fgetts( buffer, 256, fp );
		_stscanf( buffer, "%s %s", tempIP, tempName );

		if( _tcscmp( "<end>", tempIP ) == 0 )
		{
			loop = FALSE;
			break;
		}

		_tcsncpy( m_szIDC_IP[CTCIDC][ count ], tempIP, 64 );
		_tcsncpy( m_szIDC_Name[ CTCIDC ][ count ], tempName, 32 );
		count++;
	}

	fclose( fp );

	m_siMaxCTCIDC_Number = count;

	int i;
	int j;
	bool find = false;
	for( i = 0; i < MAX_IDC; ++i )
	{		
		for( j = 0; j < MAX_FRONTSERVER_NUMBER; ++j )
		{
			if( strcmp( serverip1, m_szIDC_IP[i][j] ) == 0 )
			{
				find = true;
				break;
			}
		}

		if( true == find )
			break;
	}

	m_siSelectIDC = i;
    m_siSelectFrontServerIndex = j;

	for( i = 0; i < m_siMaxCNCIDC_Number; ++ i )
	{
	/*	if( CNCIDC == m_siSelectIDC && m_siSelectFrontServerIndex == i )
		{
		}
		else*/
		{
			m_pCNCFront[i].Init( m_szIDC_IP[CNCIDC][i], m_szIDC_IP[CNCIDC][i], 5700 , this, TRUE, CNCIDC, i );
		}

		AddButtonImage( "PatchInfo/Area1_C.bmp", &m_IDCBitmapClick[CNCIDC][i] );
		AddButtonImage( "PatchInfo/Area1_D.bmp", &m_IDCBitmapDisable[CNCIDC][i] );
	}

	m_pCTCFront = new FrontProcess[m_siMaxCTCIDC_Number];
	m_IDCBitmapClick[CTCIDC] = new CBitmap[m_siMaxCTCIDC_Number];
	m_IDCBitmapDisable[CTCIDC] = new CBitmap[m_siMaxCTCIDC_Number];

	for( i = 0; i < m_siMaxCTCIDC_Number; ++ i )
	{
	/*	if( CTCIDC == m_siSelectIDC && m_siSelectFrontServerIndex == i )
		{
		}
		else*/
		{
			m_pCTCFront[i].Init( m_szIDC_IP[CTCIDC][i], m_szIDC_IP[CTCIDC][i], 5700 , this, TRUE, CTCIDC, i );
		}
		AddButtonImage( "PatchInfo/Area1_C.bmp", &m_IDCBitmapClick[CTCIDC][i] );
		AddButtonImage( "PatchInfo/Area1_D.bmp", &m_IDCBitmapDisable[CTCIDC][i] );
	}

	if( true == m_bFirstConnect )
		m_pFront->Init( m_szIDC_IP[CTCIDC][0], m_szIDC_IP[CTCIDC][0], 5700 , this, false, i, j );
	else
		m_pFront->Init( serverip1, serverip2, 5700 , this, false, i, j );

	// 처음에 패치정보를 받아올려고 하는 곳이 어디인지 판단
	if( strcmp( m_szIDC_IP[CTCIDC][0], m_pFront->GetFrontSession()->GetIP() ) == 0 )
	{
		m_siWhereConnect = CTCIDC;
//		m_siCTCConnectIndex++;
	}
	else
	{
		m_siWhereConnect = CNCIDC;
//		m_siCNCConnectIndex++;
	}

	m_dwTotalFileSize = 0;
	m_usCount = 0;

	m_uiCurrentVersion = 0;

	ZeroMemory( m_strGameFolderPath, 1024 );		// 게임 폴더 경로
	ZeroMemory( m_strGameExeFileName, 128 );		// 게임 실행 파일명
	ZeroMemory( m_strGameFullPath, 1536 );			// 게임 전체 경로	

	/*	pTitle = GetTxtFromMgr(2);
	HWND hWnd = ::FindWindow( pTitle , NULL );

	if ( hWnd != NULL )
	{
	pText = GetTxtFromMgr(3);
	pTitle = GetTxtFromMgr(4);

	MessageBox(pText,pTitle, MB_ICONQUESTION | MB_OK );		
	EndProcess();		
	return TRUE;
	}
	*/
	pText = GetTxtFromMgr(1);
	HWND hWnd = ::FindWindow( NULL, pText );

	if ( hWnd != NULL )
	{
		pText = GetTxtFromMgr(5);
		pTitle = GetTxtFromMgr(1);

		MessageBox(pText,pTitle, MB_ICONQUESTION | MB_OK );		
		EndProcess();		
		return TRUE;
	}

	pText = GetTxtFromMgr(1);
	SetWindowText(pText);	
	/*
	if ( CheckDupGame( pText, pText ) )
	{
		pText =GetTxtFromMgr(5);
		pTitle =GetTxtFromMgr(1);
		MessageBox(pText,pTitle, MB_ICONQUESTION | MB_OK );
		EndProcess();
		return TRUE;
	}
	*/

		if ( ! GetGameInfo() )
		{
			pText =GetTxtFromMgr(1);
			pTitle =GetTxtFromMgr(6);
			MessageBox(pTitle,pText, MB_ICONQUESTION | MB_OK );

			EndProcess();

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );

			CreateProcess( m_strGameFullPath, m_strGameFullPath, NULL, NULL, FALSE, 0, NULL, m_strGameFolderPath, &si, &pi );

		/*	switch( m_siServiceArea )
			{
			case SA_KOR:
				::ShellExecute( NULL, NULL, "http://www.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
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
			}
		*/	
			return TRUE;
		}

		// 패치파일을 받는 폴더 생성
		CreateDirectory( m_strPatchFolderPath, NULL);

		if(m_uiCurrentVersion == 0)
		{
			return TRUE;
		}
		else
		{
			m_hDownLoadMgrThreadHandle = NULL;
			m_bDownLoadMgrThreadRun = false;	

			m_ucCurrentStatus = DLG_SOCKET_STATUS;

			for(int i = 0 ; i < RECT_END ; ++i)
				m_bClick[i] = false;

			FILE* fp = _tfopen( TEXT("fullscreen.txt"), TEXT("rt") );

			if(fp != NULL)
			{
				int useFullScreen = 0;

				_ftscanf( fp, "%d", &useFullScreen );

				if( 0 == useFullScreen )
				{
					m_bClick[RECT_BUTTON_CHECK] = false;
				}
				else
				{
					m_bClick[RECT_BUTTON_CHECK] = true;
				}
			}

			fclose(fp);

			for(int i = 0 ; i < RECT_END ; ++i)
				m_bHover[i] = false;

			if( FIRST_PAGE == m_siPage )
			{
				m_WebBrowser.SetWindowPos( &wndTopMost  , 275, 53, 385, 355,  SWP_SHOWWINDOW );
			}

			m_StartClock = clock();
			SetTimer( 0, 10, NULL );
		}

		return TRUE;  // return TRUE  unless you set the focus to a control
}

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

	if( m_IDCBitmapClick[CNCIDC] )
	{
		delete[] m_IDCBitmapClick[CNCIDC];
		m_IDCBitmapClick[CNCIDC] = NULL;
	}

	if( m_IDCBitmapClick[CTCIDC] )
	{
		delete[] m_IDCBitmapClick[CTCIDC];
		m_IDCBitmapClick[CTCIDC] = NULL;
	}

	if( m_IDCBitmapDisable[CNCIDC] )
	{
		delete[] m_IDCBitmapDisable[CNCIDC];
		m_IDCBitmapDisable[CNCIDC] = NULL;
	}

	if( m_IDCBitmapDisable[CTCIDC] )
	{
		delete[] m_IDCBitmapDisable[CTCIDC];
		m_IDCBitmapDisable[CTCIDC] = NULL;
	}

	if ( m_pCNCFront )
	{
		for( int i = 0; i < m_siMaxCNCIDC_Number; ++i )
		{
			if( m_pCNCFront[i].m_pFrontSession )
				m_pCNCFront[i].m_pFrontSession->CloseSocket();
		}

		delete[] m_pCNCFront;
		m_pCNCFront = NULL;
	}

	if ( m_pCTCFront )
	{
		for( int i = 0; i < m_siMaxCTCIDC_Number; ++i )
		{
			if( m_pCTCFront[i].m_pFrontSession )
				m_pCTCFront[i].m_pFrontSession->CloseSocket();
		}

		delete[] m_pCTCFront;
		m_pCTCFront = NULL;
	}

	if ( m_pFront )
	{
		delete m_pFront;
		m_pFront = NULL;
	}
}

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
		CPaintDC dc(this);
		Drawing(&dc);

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

	char *pText= GetTxtFromMgr(7);
	char *pTitle= GetTxtFromMgr(1);
	if ( MessageBox(pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES )
	{
		KillTimer( 0 );	
		KillTimer( 1 );	

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
		FILE* fp = _tfopen( "PatchInfo/Option.txt", "w" );

		if( fp == NULL )
			return;

		_ftprintf( fp, "%d\n", m_siCDNSelect );
		_ftprintf( fp, "%d\n", m_siPrevAreaButtonIdex );

		fclose( fp );

		char buffer[128] = {'\0'};
		sprintf( buffer, "%s", m_szIDC_IP[m_siCDNSelect][m_siPrevAreaButtonIdex] );

		int len = strlen( buffer );

		if( len > 0 ) 
			NWriteCryptFile( TEXT("ServerIP.dat"), (BYTE*)buffer, len );

		KillTimer( 0 );	
		KillTimer( 1 );	

		STARTUPINFO si={0,};
		PROCESS_INFORMATION pi;

		si.cb = sizeof( STARTUPINFO );
		si.dwFlags = 0;

		if ( m_siServiceArea == SA_NETMARBLE )
		{
			::CreateProcess( m_strGameFullPath, m_strParameter , NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
		}
		else
		{
			::CreateProcess( m_strGameFullPath, m_strParameter, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
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

	HGDIOBJ hObject = m_BackGroundBitmap[0].Detach();
	if ( hObject ) 
	{
		DeleteObject( hObject );
	}

	hObject = m_BackGroundBitmap[1].Detach();
	if ( hObject ) 
	{
		DeleteObject( hObject );
	}

	int i;
	for( i =  0 ; i < RECT_END ; ++i )
	{
		hObject = m_CustomButtonBitmapClick[i].Detach();

		if ( hObject )
		{
			DeleteObject( hObject );
		}

		hObject = m_CustomButtonBitmapDisable[i].Detach();

		if ( hObject )
		{
			DeleteObject( hObject );
		}
	}

	for( i = 0; i < m_siMaxCNCIDC_Number; ++i )
	{
		hObject = m_IDCBitmapClick[CNCIDC][i].Detach();

		if ( hObject )
		{
			DeleteObject( hObject );
		}

		hObject = m_IDCBitmapDisable[CNCIDC][i].Detach();

		if ( hObject )
		{
			DeleteObject( hObject );
		}
	}

	for( i = 0; i < m_siMaxCTCIDC_Number; ++i )
	{
		hObject = m_IDCBitmapClick[CTCIDC][i].Detach();

		if ( hObject )
		{
			DeleteObject( hObject );
		}

		hObject = m_IDCBitmapDisable[CTCIDC][i].Detach();

		if ( hObject )
		{
			DeleteObject( hObject );
		}
	}

/*	if ( m_pCNCFront )
	{
		for( int i = 0; i < m_siMaxCNCIDC_Number; ++i )
		{
			if( m_pCNCFront[i].m_pFrontSession )
				m_pCNCFront[i].m_pFrontSession->CloseSocket();
		}

		delete[] m_pCNCFront;
		m_pCNCFront = NULL;
	}

	if ( m_pCTCFront )
	{
		for( int i = 0; i < m_siMaxCTCIDC_Number; ++i )
		{
			if( m_pCTCFront[i].m_pFrontSession )
				m_pCTCFront[i].m_pFrontSession->CloseSocket();
		}

		delete[] m_pCTCFront;
		m_pCTCFront = NULL;
	}*/

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

	switch ( m_siServiceArea )
	{
	case SA_KOR:
		if ( ! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, "Software\\Intizen\\GoonZu", KEY_ALL_ACCESS ) )
		{
			return false;
		}
		break;
	case SA_ENG:
		if ( ! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, "Software\\NDOORS\\GoonZuEng", KEY_ALL_ACCESS ) )
		{
			return false;
		}
		break;
	case SA_CHA:
		if(! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, "Software\\NDOORS\\GOONZUCHINA", KEY_ALL_ACCESS ) )
		{
			return false;
		}
		break;
	case SA_NETMARBLE:
		if(! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, "Software\\NDOORS\\GOONZUNETMARBLE", KEY_ALL_ACCESS ) )
		{
			return false;
		}
		break;
	case SA_TESTG:
		if(! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, "Software\\NDOORS\\TESTGOONZU", KEY_ALL_ACCESS ) )
		{
			return false;
		}
		break;

	case SA_JPN:
		if(! m_pRegistryMgr->RegistryCreateKey( HKEY_CURRENT_USER, "Software\\GamePot\\Kunshu Online", KEY_ALL_ACCESS ) )
		{
			return false;
		}
		break;
	}

	bool	exist_registry = true;

	if ( ! m_pRegistryMgr->GetRegReadInt( "VERSION", &m_uiCurrentVersion ) )
	{
		m_uiCurrentVersion	= 0;
		exist_registry = false;
		//return false;
	}

	if ( !exist_registry || !m_pRegistryMgr->GetRegReadString( "PATH", m_strGameFolderPath, 1024 ) )
	{
		m_strGameFolderPath[0] = '\0';
		exist_registry = false;
		//return false;
	}

	if ( !exist_registry || !m_pRegistryMgr->GetRegReadString( "FILENAME", m_strGameExeFileName, 128 ) )
	{
		m_strGameExeFileName[0] = '\0';
		exist_registry = false;
		//return false;
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
		default:
			return false;
		}
	}

	// ::SetCurrentDirectory( m_strGameFolderPath );

	strcpy( m_strGameFullPath, m_strGameFolderPath );
	strcat( m_strGameFullPath, m_strGameExeFileName );

	strcpy( m_strPatchFolderPath, m_strGameFolderPath );
	strcat( m_strPatchFolderPath, PATCH_FOLDER);

	// 패치파일을 받는 폴더 생성
	CreateDirectory( m_strPatchFolderPath, NULL);

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CPatchDlg::SetNewDownLoadPathCount( US16 Count )
//	Desc : Count 갯수만큼 Ftp 객체 생성하고, Object 생성한다.
//	Date : 2003. 06. 10
//	Update : 2003. 06. 18 - 주석 수정, Ftp 기본 경로(m_strFtpBasePath) 넣어 줌.
//--------------------------------------------------------------------------------------------
bool CPatchDlg::SetNewDownLoadPathCount(  sResponse_PatchInfo * pRecvMsg  )
{
	char *pText= GetTxtFromMgr(8);
	char *pTitle= GetTxtFromMgr(1);
	if ( pRecvMsg->uiCount > MAX_DOWNLOAD_PATCH_FILE_NUM )				// 이 경우 웹에서 전체 설치 받아서 다운 받기를 권고 함.
	{
		if ( MessageBox( pText, pTitle, MB_ICONQUESTION | MB_YESNO ) == IDYES )
		{
			switch( m_siServiceArea )
			{
			case SA_KOR:
				::ShellExecute( NULL, NULL, "http://www.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_ENG:
				::ShellExecute( NULL, NULL, "http://global.goonzu.com", NULL, NULL, SW_SHOWNORMAL );
				break;
			case SA_CHA:
				::ShellExecute( NULL, NULL, "http://www.hljz.net/Default_Z1.aspx", NULL, NULL, SW_SHOWNORMAL );
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
			}
			return false;
		}
	}

	char strBaseURL[ _MAX_PATH ];
	SI32 siBaseURLLen = 0;

	if (IsCorrectDNS(pRecvMsg->szAddr))
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
		strcpy ( strBaseURL, pRecvMsg->szAddr_2 );
		siBaseURLLen = strlen ( strBaseURL );

		if ( strBaseURL[ siBaseURLLen - 1 ] != '/' )
		{
			strcat( strBaseURL, "/" );
		}

		strcat( strBaseURL, pRecvMsg->szPath_2 );
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

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::DrawDownLoadStatus()
//	Desc : 현재 다운로드 상태를 화면에 보여주기 위해서...
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::DrawDownLoadStatus(CDC * Destination)
{
	if ( m_dwTotalFileSize < 1 )
	{
		return;
	}

	RECT Rect = { 94, 7, 200, 30 };
	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 58, 56, 125 ) );

	Destination->DrawText( m_szPatchVersionInfo, &Rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	RECT Rect3 = { 96, 7, 202, 30 };
	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 176, 211, 255 ) );

	Destination->DrawText( m_szPatchVersionInfo, &Rect3, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	DWORD dwRecvFileSize = m_pHttpMgr->GetFileRecvTotalSize();

	float percent = (float) ( ( dwRecvFileSize * 1.0 ) / m_dwTotalFileSize * 100 );

	if ( percent > 100.0f )
	{
		percent = 100.0f;
	}

	if ( percent < 0.0f )
	{
		percent = 0.0f;
	}


	CString strPercent = "";
	strPercent.Format("%.2f%%", percent );

	CDC mdc1;

	mdc1.CreateCompatibleDC( Destination );		

	CBitmap *pOldBitmap1 = mdc1.SelectObject( &m_ProgressBitmap );	

	for( int i = 0; i < (int)(PROGRESS_WIDTH * percent / 100.0f); ++i )
		Destination->BitBlt( PROGRESS_START_XPOS + i, PROGRESS_START_YPOS, 1, PROGRESS_HEIGHT, &mdc1, 0, 0, SRCCOPY );

	mdc1.SelectObject( pOldBitmap1 );

	ReleaseDC( &mdc1 );

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


	// "Download" 폴더를 삭제한다. - 남아있는 패치파일을 모두 삭제한다.
	::RemoveDirectory( m_strPatchFolderPath );

	SetTimer( 1, 10, NULL );
	GameReady();
	
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
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::GameReady()
//	Desc : 패치 할 필요가 없거나, 패치가 끝 났을 시에 이 함수를 호출하여 게임을 실행함.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::GameReady()
{	
	m_ucCurrentStatus = DLG_GAMESTART_STATUS;

	char * pText = GetTxtFromMgr(24);

	char buffer[128] = {'\0'};
	strncpy( m_szState, pText, 128 );

	RECT RefreshRegion = { 0, 0, 270, PROGRESS_START_YPOS + 18 };
	InvalidateRect(&RefreshRegion);
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CPatchDlg::OnTimer(UINT nIDEvent) 
//	Desc : 네트워크를 담당하는 run 함수를 호출
//	Date : 2003. 08. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CPatchDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static clock_t ClockTemp = clock();

	if( 0 == nIDEvent )
	{
		if ( m_ucCurrentStatus == DLG_SOCKET_STATUS )
		{
			double duration = (double)( clock() - m_StartClock ) / CLOCKS_PER_SEC;

			// 계속 접속하던 유저
			if( false == m_bFirstConnect )
			{
				if ( duration < 3 )
				{
					m_pFront->Run();
				}
				else
				{
					if( FALSE == m_pFront->m_bVersion )
					{
						if( TRUE == m_bFailPatchInfo )
						{
							KillTimer( nIDEvent );
							MessageBox( "Failed RequestPatchInfo", "Error" );
						}

						if( FALSE == m_bFailPatchInfo )
						{
							if( m_siCNCConnectIndex == m_siMaxCNCIDC_Number &&
								m_siCTCConnectIndex == m_siMaxCTCIDC_Number )
							{
								m_bFailPatchInfo = TRUE;
							}
							else
							{
								m_pFront->m_pFrontSession->CloseSocket();
								delete m_pFront;
								m_pFront = NULL;

								if( CNCIDC == m_siWhereConnect )
								{
									if( m_siCTCConnectIndex <= m_siMaxCTCIDC_Number )
									{
										m_pFront = new FrontProcess;

										m_siWhereConnect = CTCIDC;
										m_pFront->Init( m_szIDC_IP[CTCIDC][m_siCTCConnectIndex], m_szIDC_IP[CNCIDC][m_siCTCConnectIndex], 5700, this, false, 0, 0 );

										m_siCTCConnectIndex++;
									}
								}
								else if( CTCIDC == m_siWhereConnect )
								{
									if( m_siCNCConnectIndex <= m_siMaxCNCIDC_Number )
									{
										m_pFront = new FrontProcess;

										m_siWhereConnect = CNCIDC;
										m_pFront->Init( m_szIDC_IP[CNCIDC][m_siCNCConnectIndex], m_szIDC_IP[CNCIDC][m_siCNCConnectIndex], 5700, this, false, 0, 0 );

										m_siCNCConnectIndex++;
									}
								}

								m_StartClock = clock();
							}
						}
					}
					else
					{
						KillTimer( nIDEvent );
					}
				}
			}
			// 처음 군주에 접속하는 유저
			else
			{
				if ( duration < 3 )
				{
					m_pFront->Run();
				}
				else
				{
					if( FALSE == m_pFront->m_bVersion )
					{
						if( TRUE == m_bFailPatchInfo )
						{
							KillTimer( nIDEvent );
							MessageBox( "Failed RequestPatchInfo", "Error" );
						}

						if( FALSE == m_bFailPatchInfo )
						{
							if( m_siCNCConnectIndex == m_siMaxCNCIDC_Number &&
								m_siCTCConnectIndex == m_siMaxCTCIDC_Number )
							{
								m_bFailPatchInfo = TRUE;
							}
							else
							{
								m_pFront->m_pFrontSession->CloseSocket();
								delete m_pFront;
								m_pFront = NULL;

								if( CNCIDC == m_siWhereConnect )
								{
									if( m_siCTCConnectIndex <= m_siMaxCTCIDC_Number )
									{
										m_pFront = new FrontProcess;

										m_siWhereConnect = CTCIDC;
										m_pFront->Init( m_szIDC_IP[CTCIDC][m_siCTCConnectIndex], m_szIDC_IP[CNCIDC][m_siCTCConnectIndex], 5700, this, false, 0, 0 );

										m_siCTCConnectIndex++;
									}
								}
								else if( CTCIDC == m_siWhereConnect )
								{
									if( m_siCNCConnectIndex <= m_siMaxCNCIDC_Number )
									{
										m_pFront = new FrontProcess;

										m_siWhereConnect = CNCIDC;
										m_pFront->Init( m_szIDC_IP[CNCIDC][m_siCNCConnectIndex], m_szIDC_IP[CNCIDC][m_siCNCConnectIndex], 5700, this, false, 0, 0 );

										m_siCNCConnectIndex++;
									}
								}

								m_StartClock = clock();
							}
						}
					}
					else
					{
						KillTimer( nIDEvent );
					}
				}
			}
		}
		else 
		{
			if ( m_pHttpMgr && m_pHttpMgr->GetDownLoadStatus() == DOWNLOAD_COMPLETE_STATUS )
			{
				if( DLG_DECOMPRESS_STATUS == m_ucCurrentStatus )
				{
					BeginWaitCursor();
					Decompress();
					EndWaitCursor();

				}
				else if(  DLG_DOWNLOAD_STATUS == m_ucCurrentStatus )
				{
					char* pText = GetTxtFromMgr(23);

					strncpy( m_szState, pText, 128 );
					m_ucCurrentStatus = DLG_DECOMPRESS_STATUS;
					Invalidate(FALSE);
				}
			}
			else
			{
				Invalidate(FALSE);
			}
		}
	}
	else if( 1 == nIDEvent )
	{
	//	if ( DLG_GAMESTART_STATUS == m_ucCurrentStatus )
		{
			int i;
			for( i = 0; i < m_siMaxCNCIDC_Number; ++i )
			{
				m_pCNCFront[i].Run();
			}

			for( i = 0; i < m_siMaxCTCIDC_Number; ++i )
			{
				m_pCTCFront[i].Run();
			}
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
			sResponse_SecretKey *pRecvMsg = (sResponse_SecretKey *)pPacket;

			FrontProcess *pFP = m_pFront;
			ITZSession *pSession = pFP->m_pFrontSession;

			pSession->GetEncrypt()->SetKey( pRecvMsg->ucKey1, pRecvMsg->ucKey2 );

			InterlockedExchange( (LPLONG)&pFP->m_bSecretKey, TRUE );

		}
		break;

	case FRONTMSG_RESPONSE_PATCHINFO:
		{
			sResponse_PatchInfo *pRecvMsg = (sResponse_PatchInfo *)pPacket;

			SI32 tempversion = m_uiCurrentVersion + pRecvMsg->uiCount;
			sprintf( m_szPatchVersionInfo, "v%d.%d", tempversion / 100, tempversion % 100 );

			bool bConnectDns = false;

			if (pRecvMsg->szAddr != NULL)
			{
				bConnectDns = IsCorrectDNS(pRecvMsg->szAddr);
			}

			// 버전 정보를 받았음을 설정
			m_pFront->m_bVersion = TRUE;

			if ( pRecvMsg->uiCount == 0 )
			{
				// 프론트 서버와의 접속을 끊어준다
				KillTimer( 0 );				
				m_pFront->m_pFrontSession->CloseSocket();

				SetTimer( 1, 10, NULL );
				GameReady();
				break;
			}

			if( FIRST_PAGE == m_siPage )
			{
				m_WebBrowser.SetWindowPos( &wndTopMost  , 275, 53, 385, 355,  SWP_SHOWWINDOW );
			}

			FILE * fp = NULL ;
			char szFileName[ MAX_PATH ] = "" ;
			char str[ MAX_LENGTH ] = "" ;	

			strcpy( szFileName,"ServiceArea.txt" );

			fp = fopen(szFileName,"r");
			if ( fp == NULL )
				return;

			fgets( str, MAX_LENGTH, fp );
			fclose( fp );
			fp = NULL;

			char Buffer[ MAX_PATH ] = "";

			sprintf( Buffer, "PatchInfo/PatchBaseInfo%s" ,str );

			fp = fopen(Buffer, "rt" );

			m_pHttpMgr = new CHttpMgr( pRecvMsg->uiCount );
			m_pHttpMgr->Connect();


			m_puiVersionList = new UI32[ pRecvMsg->uiCount ];

			memset( m_puiVersionList, 0, sizeof( UI32 ) * pRecvMsg->uiCount );
			memcpy( m_puiVersionList, pRecvMsg->uiVersionHistory, sizeof( UI32 ) * pRecvMsg->uiCount );

			if (! SetNewDownLoadPathCount( pRecvMsg ) )
			{
				// 프론트 서버와의 접속을 끊어준다
				KillTimer( 0 );
				m_pFront->m_pFrontSession->CloseSocket();
				EndProcess();

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
		
			char *pText = GetTxtFromMgr(26);
			strncpy( m_szState, pText, 128 );
			m_pHttpMgr->DownLoadStart();

			m_ucCurrentStatus = DLG_DOWNLOAD_STATUS;
			SetTimer( 0, 100, NULL );

			m_pFront->m_pFrontSession->CloseSocket();
		}
		break;

	case FRONTMSG_RESPONSE_PINGTEST:
		{
			sResponse_PingTest *pRecvMsg = (sResponse_PingTest *)pPacket;

			UI08 idcType = pRecvMsg->m_uiIDCType;
			SI32 frontServerIndex = pRecvMsg->m_siFrontServerIndex;
			g_dwDleayClock[idcType][frontServerIndex] = ( GetTickCount() - g_dwSendFrontServerClock[idcType][frontServerIndex] );

			if( CNCIDC == idcType )
			{
				m_pCNCFront[frontServerIndex].m_bRetryConnect = TRUE;
				m_pCNCFront[frontServerIndex].m_pFrontSession->CloseSocket();
		    }
			else if( CTCIDC == idcType )
			{
				m_pCTCFront[frontServerIndex].m_bRetryConnect = TRUE;
				m_pCTCFront[frontServerIndex].m_pFrontSession->CloseSocket();
			}
		
			char ping[20] = {'\0'};
			sprintf( ping, "%dms", g_dwDleayClock[idcType][frontServerIndex] );
			strncpy( m_szPingNumber[idcType][frontServerIndex], ping, 20 );
		}
		break;
	}
}

HBRUSH CPatchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here
	if( pWnd->m_hWnd == m_hWnd )
	{
		if( FIRST_PAGE == m_siPage )
			return (HBRUSH)m_BackGroundBrush[0];
		else if( NEXT_PAGE == m_siPage)
		{
			return (HBRUSH)m_BackGroundBrush[1];
		}
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPatchDlg::OnClose() 
{
	if ( m_ucCurrentStatus == DLG_GAMESTART_STATUS )
	{
		KillTimer( 0 );	
		KillTimer( 1 );	

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
	for(int i = 0 ; i < RECT_END ; ++i)
	{
		if( RECT_BUTTON_CNCIDC == i ||
			RECT_BUTTON_CTCIDC == i )
			continue;

		bool Temp = m_bHover[i];

		m_bHover[i] = false;

		if ( PtInRect(&CustomControlRect[i], point) )
		{
			m_bHover[i] = true;
		}

		if(Temp != m_bHover[i])
		{
			InvalidateRect(&CustomControlRect[i]);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CPatchDlg::DrawInstallStatus( CDC * Destination )
{
	CDC mdc;
	mdc.CreateCompatibleDC( Destination );

	CBitmap *pOldBitmap = mdc.SelectObject( &m_ProgressBitmap );

	for( int i = 0; i < PROGRESS_WIDTH; ++i )
		Destination->BitBlt( PROGRESS_START_XPOS + i, PROGRESS_START_YPOS, 1, PROGRESS_HEIGHT, &mdc, 0, 0, SRCCOPY );

	RECT Rect = { 94, 7, 200, 30 };
	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 58, 56, 125 ) );
	
	Destination->DrawText( m_szPatchVersionInfo, &Rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	RECT Rect3 = { 96, 7, 202, 30 };
	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 176, 211, 255 ) );

	Destination->DrawText( m_szPatchVersionInfo, &Rect3, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	RECT Rect2 = { 20, 361, 320, 380 };
	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 255, 255, 255 ) );

	Destination->DrawText( m_szState, &Rect2, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	mdc.SelectObject( pOldBitmap );

	ReleaseDC( &mdc );

	return;
}

UINT CPatchDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint TempPoint(point);

	ScreenToClient( &TempPoint );

	for(int i = 0 ; i < RECT_END ; ++i)
	{
		if ( PtInRect(&CustomControlRect[i], TempPoint) )
		{
			return CDialog::OnNcHitTest(point);
		}
	}

	if( m_siCDNSelect == CNCIDC )
	{
		for(int j = 0 ; j < m_siMaxCNCIDC_Number ; ++j )
		{
			if ( PtInRect(&AreaControlRect[j], TempPoint) )
			{
				return CDialog::OnNcHitTest(point);
			}
		}
	}
	else if( m_siCDNSelect == CTCIDC )
	{
		for(int j = 0 ; j < m_siMaxCTCIDC_Number ; ++j )
		{
			if ( PtInRect(&AreaControlRect[j], TempPoint) )
			{
				return CDialog::OnNcHitTest(point);
			}
		}
	}

	UINT hit = CDialog::OnNcHitTest( point );

	// 마우스가 눌렸을때만 제목표시줄인척 해줍시다. 안그럼 롤오버가 제대로 안돌아가요. - KHRIX

	if ( hit == HTCLIENT && GetAsyncKeyState(VK_LBUTTON))
	{
		return HTCAPTION;
	}
	else
	{
		return hit;
	}
}

/*
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
*/

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

void CPatchDlg::Drawing(CDC * Destination)
{
	if( FIRST_PAGE == m_siPage )
	{
		if ( m_ucCurrentStatus == DLG_DOWNLOAD_STATUS )
		{
			DrawDownLoadStatus(Destination);
		}
		else if ( DLG_DECOMPRESS_STATUS == m_ucCurrentStatus || 
			DLG_GAMESTART_STATUS == m_ucCurrentStatus	)
		{
			DrawInstallStatus(Destination);
		}

		for(int i = RECT_BUTTON_HOMEPAGE ; i <= RECT_BUTTON_EXIT ; ++i)
		{
			if( RECT_BUTTON_CHECK != i )
			{
				if( true == m_bHover[i] || 
					true == m_bClick[i] )
				{
					DrawCustomButton( Destination, i, 1 );
					break;
				}
			}
			else
			{
				if( true == m_bClick[i] )
				{
					DrawCustomButton( Destination, i, 1 );
					break;
				}
			}
		}
	}
	else
	{
		int i;
		int loopCount = 0;
		if( CNCIDC == m_siCDNSelect )
		{
			loopCount = m_siMaxCNCIDC_Number;
			for( i = 0 ; i < loopCount ; ++i )
			{
				DrawAreaButton( Destination, i, 1 );
			}
		}
		else if( CTCIDC == m_siCDNSelect )
		{
			loopCount = m_siMaxCTCIDC_Number;
			for( i = 0 ; i < loopCount ; ++i )
			{
				DrawAreaButton( Destination, i, 1 );
			}
		}

		CDC mdc;
		mdc.CreateCompatibleDC( Destination );

		CBitmap* pOldBitmap;
		if( true == m_bClick[RECT_BUTTON_CNCIDC] )
			pOldBitmap = mdc.SelectObject( &m_CustomButtonBitmapClick[RECT_BUTTON_CNCIDC] );
		else
			pOldBitmap = mdc.SelectObject( &m_CustomButtonBitmapDisable[RECT_BUTTON_CNCIDC] );

		int X = CustomControlRect[RECT_BUTTON_CNCIDC].left;
		int Y = CustomControlRect[RECT_BUTTON_CNCIDC].top;
		int Width = CustomControlRect[RECT_BUTTON_CNCIDC].right - X;
		int Height = CustomControlRect[RECT_BUTTON_CNCIDC].bottom - Y;

		Destination->BitBlt( X, Y, Width, Height, &mdc, 0, 0, SRCCOPY );

		mdc.SelectObject( pOldBitmap );

		CBitmap* pOldBitmap2;
		if( true == m_bClick[RECT_BUTTON_CTCIDC] )
			pOldBitmap2 = mdc.SelectObject( &m_CustomButtonBitmapClick[RECT_BUTTON_CTCIDC] );
		else
			pOldBitmap2 = mdc.SelectObject( &m_CustomButtonBitmapDisable[RECT_BUTTON_CTCIDC] );

		X = CustomControlRect[RECT_BUTTON_CTCIDC].left;
		Y = CustomControlRect[RECT_BUTTON_CTCIDC].top;
		Width = CustomControlRect[RECT_BUTTON_CTCIDC].right - X;
		Height = CustomControlRect[RECT_BUTTON_CTCIDC].bottom - Y;

		Destination->BitBlt( X, Y, Width, Height, &mdc, 0, 0, SRCCOPY );

		RECT Rect = { 500, 42, 590, 70 };
		Destination->SetBkMode( TRANSPARENT );
		Destination->SetTextColor( RGB( 0, 0, 0 ) );

		Destination->DrawText( m_szLastServerName, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	/*	for( i = 0; i < loopCount; ++i )
		{
			Rect.left   = AreaControlRect[i].left + 2; 
			Rect.top    = AreaControlRect[i].top + 8;
			Rect.right  = AreaControlRect[i].left + 206;
			Rect.bottom = AreaControlRect[i].top + 25;

			Destination->SetBkMode( TRANSPARENT );
			Destination->SetTextColor( RGB( 0, 0, 0 ) );

			Destination->DrawText( m_szIDC_Name[m_siCDNSelect][i], &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE );


			Rect.left   = AreaControlRect[i].left + 2; 
			Rect.top    = AreaControlRect[i].top + 60;
			Rect.right  = AreaControlRect[i].left + 206;
			Rect.bottom = AreaControlRect[i].top + 20;

			Destination->SetBkMode( TRANSPARENT );
			Destination->SetTextColor( RGB( 0, 0, 0 ) );

			Destination->DrawText( m_szPingNumber[m_siCDNSelect][i], &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		}*/

		mdc.SelectObject( pOldBitmap2 );

		ReleaseDC( &mdc );

		for( i = RECT_BUTTON_PINGTEST ; i < RECT_END ; ++i)
		{
			if(m_bHover[i] || m_bClick[i])
			{
				DrawCustomButton( Destination, i, 1 );
				break;
			}
		}
	}
}

void CPatchDlg::DrawCustomButton( CDC * Destination, int ButtonIndex, int ButtonMode )
{	
	CDC mdc;
	mdc.CreateCompatibleDC( Destination );

	CBitmap *pOldBitmap = mdc.SelectObject( &m_CustomButtonBitmapClick[ButtonIndex] );

	int X = CustomControlRect[ButtonIndex].left;
	int Y = CustomControlRect[ButtonIndex].top;
	int Width = CustomControlRect[ButtonIndex].right - X;
	int Height = CustomControlRect[ButtonIndex].bottom - Y;

	Destination->BitBlt( X, Y, Width, Height, &mdc, 0, 0, SRCCOPY );

	mdc.SelectObject( pOldBitmap );

	ReleaseDC( &mdc );

}

void CPatchDlg::DrawAreaButton( CDC * Destination, int ButtonIndex, int ButtonMode )
{	
	CDC mdc;
	mdc.CreateCompatibleDC( Destination );

	CBitmap *pOldBitmap;

	if( true == m_bAreaClick[ButtonIndex] )
		pOldBitmap = mdc.SelectObject( &m_IDCBitmapClick[m_siCDNSelect][ButtonIndex] );
	else
		pOldBitmap = mdc.SelectObject( &m_IDCBitmapDisable[m_siCDNSelect][ButtonIndex] );

	int X = AreaControlRect[ButtonIndex].left;
	int Y = AreaControlRect[ButtonIndex].top;
	int Width = AreaControlRect[ButtonIndex].right - X;
	int Height = AreaControlRect[ButtonIndex].bottom - Y;

	Destination->BitBlt( X, Y, Width, Height, &mdc, 0, 0, SRCCOPY );

	RECT Rect;
	Rect.left   = AreaControlRect[ButtonIndex].left + 2; 
	Rect.top    = AreaControlRect[ButtonIndex].top + 8;
	Rect.right  = AreaControlRect[ButtonIndex].left + 206;
	Rect.bottom = AreaControlRect[ButtonIndex].top + 25;

	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 0, 0, 0 ) );

	Destination->DrawText( m_szIDC_Name[m_siCDNSelect][ButtonIndex], &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE );


	Rect.left   = AreaControlRect[ButtonIndex].left + 2; 
	Rect.top    = AreaControlRect[ButtonIndex].top + 60;
	Rect.right  = AreaControlRect[ButtonIndex].left + 206;
	Rect.bottom = AreaControlRect[ButtonIndex].top + 20;

	Destination->SetBkMode( TRANSPARENT );
	Destination->SetTextColor( RGB( 0, 0, 0 ) );

	Destination->DrawText( m_szPingNumber[m_siCDNSelect][ButtonIndex], &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	mdc.SelectObject( pOldBitmap );

	ReleaseDC( &mdc );

}

void CPatchDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	int i = 0;
	int j = 0;

	ReleaseCapture();

	if( FIRST_PAGE == m_siPage )
	{
		for(i = RECT_BUTTON_HOMEPAGE ; i <= RECT_BUTTON_EXIT ; ++i)
		{
			if ( m_bClick[i] == true)
			{
				InvalidateRect(&CustomControlRect[i]);
			}

			if( RECT_BUTTON_CHECK == i )
			{
				InvalidateRect(&CustomControlRect[i]);
			}

			if( RECT_BUTTON_CHECK != i )
				m_bClick[i] = false;
		}

		for(i = RECT_BUTTON_HOMEPAGE ; i <= RECT_BUTTON_EXIT ; ++i)
		{
			if ( PtInRect(&CustomControlRect[i], point) )
			{
				break;
			}
		}
	}
	else if( NEXT_PAGE == m_siPage )
	{
		for(i = RECT_BUTTON_CNCIDC ; i < RECT_END ; ++i)
		{
			if ( m_bClick[i] == true)
			{
				InvalidateRect(&CustomControlRect[i]);
			}

			if( RECT_BUTTON_CNCIDC == i ||
				RECT_BUTTON_CTCIDC == i )
			{
				InvalidateRect(&CustomControlRect[i]);
			}

			if( i > RECT_BUTTON_CTCIDC)
				m_bClick[i] = false;
		}

		for(i = RECT_BUTTON_CNCIDC ; i < RECT_END ; ++i)
		{
			if ( PtInRect(&CustomControlRect[i], point) )
			{
				break;
			}
		}

		if( m_siCDNSelect == CNCIDC )
		{
			for(j = 0 ; j < m_siMaxCNCIDC_Number ; ++j)
			{
				if ( m_bAreaClick[j] == true)
				{
					InvalidateRect(&AreaControlRect[j]);
					break;
				}
			}

			for(j = 0 ; j < m_siMaxCNCIDC_Number ; ++j)
			{
				if ( PtInRect(&AreaControlRect[j], point) )
				{
					break;
				}
			}
		}
		else if( m_siCDNSelect == CTCIDC )
		{
			for(j = 0 ; j < m_siMaxCTCIDC_Number ; ++j)
			{
				if ( m_bAreaClick[j] == true)
				{
					InvalidateRect(&AreaControlRect[j]);
					break;
				}
			}

			for(j = 0 ; j < m_siMaxCTCIDC_Number ; ++j)
			{
				if ( PtInRect(&AreaControlRect[j], point) )
				{
					break;
				}
			}
		}
	}

	switch(i)
	{
	case RECT_BUTTON_NEXT:
		{
			switch ( m_ucCurrentStatus )
			{
			case DLG_GAMESTART_STATUS:
				{
					m_WebBrowser.SetWindowPos( &wndTopMost  , 0, 0, 0, 0,  SWP_SHOWWINDOW );
					
					m_siPage = NEXT_PAGE;
					if( CNCIDC == m_siCDNSelect )
					{
						m_bClick[RECT_BUTTON_CNCIDC] = true;
						strncpy( m_szState, m_szIDC_Name[CNCIDC][m_siPrevAreaButtonIdex], 32 );
						strncpy( m_szLastServerName, m_szIDC_Name[CNCIDC][m_siPrevAreaButtonIdex], 32 );
					}
					else if( CTCIDC == m_siCDNSelect )
					{
						m_bClick[RECT_BUTTON_CTCIDC] = true;
						strncpy( m_szState, m_szIDC_Name[CTCIDC][m_siPrevAreaButtonIdex], 32 );
						strncpy( m_szLastServerName, m_szIDC_Name[CTCIDC][m_siPrevAreaButtonIdex], 32 );
					}

					m_bAreaClick[m_siPrevAreaButtonIdex] = true;

				/*	char ping[20] = {'\0'};
					sprintf( ping, "%dms", g_dwDleayClock[m_siCDNSelect][m_siPrevAreaButtonIdex] );
					strncpy( m_szPingNumber[m_siCDNSelect][m_siPrevAreaButtonIdex], ping, 20 );
				*/
					InvalidateRect( FALSE );
				}
				break;
			}
		}
		break;
	case RECT_BUTTON_HOMEPAGE:
		{
			::ShellExecute(NULL, NULL, "http://www.hljz.net/Default_Z1.aspx", NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	case RECT_BUTTON_ACCOUNTREGISTER:
		{
			::ShellExecute(NULL, NULL, "http://www.youxi.net.cn/UserManage/RegStep.aspx", NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	case RECT_BUTTON_CARD:
		{
			//	::ShellExecute(NULL, NULL, webURI[i], NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	case RECT_BUTTON_DOWNLOAD:
		{
			::ShellExecute(NULL, NULL, "http://www.hljz.net/download/DownloadClient.aspx", NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	case RECT_BUTTON_CHECK:
		{
			FILE* fp = _tfopen( TEXT("fullscreen.txt"), TEXT("w") );

			if(fp == NULL)
			{
				MsgBox(TEXT("fullscreen.txt"), TEXT("Error"));
				break;
			}

			if( true == m_bClick[RECT_BUTTON_CHECK] )
			{
				_ftprintf( fp, "1\n" );
			}
			else
			{
				_ftprintf( fp, "0\n" );
			}

			fclose(fp);
		}
		break;
	case RECT_BUTTON_EXIT:
		{
			OnCancel();
		}
		break;

	case RECT_BUTTON_EXIT2:
		{
			OnCancel();
		}
		break;
	case RECT_BUTTON_START:
		{
			switch ( m_ucCurrentStatus )
			{
			case DLG_GAMESTART_STATUS:
				{
					OnOK();
				}
				break;
			}
		}
		break;

	case RECT_BUTTON_CNCIDC:
		{
			if( m_siPage == NEXT_PAGE )
			{
				m_siCDNSelect = CNCIDC;
				InvalidateRect( FALSE );
				m_bAreaClick[0] = true;
				m_siPrevAreaButtonIdex = 0;
				InvalidateRect( &AreaControlRect[0] );
				strncpy( m_szState, m_szIDC_Name[CNCIDC][0], 32 );
			}
			//char ping[128] = {'\0'};
			//	sprintf( ping, "%dms", g_dwMyDleayClock );
			//	strncpy( m_szPingNumber, ping, 128 );
		}
		break;

	case RECT_BUTTON_CTCIDC:
		{
			if( m_siPage == NEXT_PAGE )
			{
				m_siCDNSelect = CTCIDC;
				InvalidateRect( FALSE );
				m_bAreaClick[0] = true;
				m_siPrevAreaButtonIdex = 0;
				InvalidateRect( &AreaControlRect[0] );
				strncpy( m_szState, m_szIDC_Name[CTCIDC][0], 32 );
			}
			//char ping[128] = {'\0'};
			//	sprintf( ping, "%dms", g_dwMyDleayClock );
			//	strncpy( m_szPingNumber, ping, 128 );
		}
		break;

	case RECT_BUTTON_PINGTEST:
		{
			if( m_siPage == NEXT_PAGE )
			{
				if( m_siCDNSelect == CNCIDC )
				{
					for( int i = 0; i < m_siMaxCNCIDC_Number; ++i )
					{
						SI32 tempValue = g_dwDleayClock[CNCIDC][i];
						if( rand() % 2 == 0 )
							tempValue = tempValue + rand() % 3;
						else if( rand() % 2 == 1 )
							tempValue = tempValue - rand() % 3;
						
						if( 0 > tempValue )
							tempValue = 0;

						char ping[20] = {'\0'};
						sprintf( ping, "%dms", tempValue );
						strncpy( m_szPingNumber[CNCIDC][i], ping, 20 );
					}

					InvalidateRect(FALSE);

					OnPaint();
				}
				else if( m_siCDNSelect == CTCIDC )
				{
					for( int i = 0; i < m_siMaxCTCIDC_Number; ++i )
					{
						SI32 tempValue = g_dwDleayClock[CTCIDC][i];
						if( rand() % 2 == 0 )
							tempValue = tempValue + rand() % 3;
						else if( rand() % 2 == 1 )
							tempValue = tempValue - rand() % 3;

						if( 0 > tempValue )
							tempValue = 0;

						char ping[20] = {'\0'};
						sprintf( ping, "%dms", tempValue );
						strncpy( m_szPingNumber[CTCIDC][i], ping, 20 );
					}

					InvalidateRect(FALSE);

					OnPaint();
				}
			}
		}
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CPatchDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if( FIRST_PAGE ==  m_siPage )
	{
		for(int i = RECT_BUTTON_HOMEPAGE ; i <= RECT_BUTTON_EXIT ; ++i)
		{
			if ( PtInRect(&CustomControlRect[i], point) )
			{
				SetCapture();
				if( RECT_BUTTON_CHECK != i)
				{
					m_bClick[i] = true;
				}
				else
				{
					if( true == m_bClick[i] )
						m_bClick[i] = false;
					else
						m_bClick[i] = true;
				}
				break;
			}
		}
	}
	else if( NEXT_PAGE == m_siPage )
	{
		if( m_siCDNSelect == CNCIDC )
		{
			for(int j = 0 ; j < m_siMaxCNCIDC_Number ; ++j)
			{
				if ( PtInRect(&AreaControlRect[j], point) )
				{
					if( true == m_bAreaClick[j] )
						break;

					SetCapture();
					m_bAreaClick[j] = true;

					if( 1 != m_siMaxCNCIDC_Number )
					{
						m_bAreaClick[m_siPrevAreaButtonIdex] = false;
						InvalidateRect(&AreaControlRect[m_siPrevAreaButtonIdex]);
						m_siPrevAreaButtonIdex = j;
					}

					strncpy( m_szState, m_szIDC_Name[m_siCDNSelect][j], 32 );
					char ping[20] = {'\0'};
					sprintf( ping, "%dms", g_dwDleayClock[m_siCDNSelect][j] );
					strncpy( m_szPingNumber[m_siCDNSelect][j], ping, 20 );
				}
				else
				{
					if ( false == PtInRect(&CustomControlRect[RECT_BUTTON_PINGTEST], point) )
						m_bAreaClick[j] = false;
				}
			}
		}
		else if( m_siCDNSelect == CTCIDC)
		{
			for(int j = 0 ; j < m_siMaxCTCIDC_Number ; ++j)
			{
				if ( PtInRect(&AreaControlRect[j], point) )
				{
					if( true == m_bAreaClick[j] )
						break;

					SetCapture();
					m_bAreaClick[j] = true;

					if( 1 != m_siMaxCTCIDC_Number )
					{
						m_bAreaClick[m_siPrevAreaButtonIdex] = false;
						InvalidateRect(&AreaControlRect[m_siPrevAreaButtonIdex]);
						m_siPrevAreaButtonIdex = j;
					}

					strncpy( m_szState, m_szIDC_Name[m_siCDNSelect][j], 32 );
					char ping[20] = {'\0'};
					sprintf( ping, "%dms", g_dwDleayClock[m_siCDNSelect][j] );
					strncpy( m_szPingNumber[m_siCDNSelect][j], ping, 20 );
				}
				else
				{
					if ( false == PtInRect(&CustomControlRect[RECT_BUTTON_PINGTEST], point) )
						m_bAreaClick[j] = false;
				}
			}
		}

		for(int i = RECT_BUTTON_CNCIDC ; i < RECT_END ; ++i)
		{
			if ( PtInRect(&CustomControlRect[i], point) )
			{
				SetCapture();
				m_bClick[i] = true;
				if( RECT_BUTTON_CNCIDC == i )
					m_bClick[RECT_BUTTON_CTCIDC] = false;
				else if( RECT_BUTTON_CTCIDC == i )
					m_bClick[RECT_BUTTON_CNCIDC] = false;
				break;
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CPatchDlg::InitFrontServer( FrontProcess* pFrontFrocess, char* pFrontIP )
{
	/*	char serverip1[ 256 ] = {'\0'};
	char serverip2[ 256 ] = {'\0'};

	strcpy( serverip1, pFrontIP );
	strcpy( serverip2, pFrontIP );

	m_pFront->Init( serverip1, serverip2, 5700 , this );*/
}

void CPatchDlg::AddButtonImage( char* pFileName, CBitmap* pBitmap )
{
	HBITMAP hBmp = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if( hBmp)
		pBitmap->Attach( hBmp );
	else
		MessageBox( pFileName, "Load ButtonImage Error" );
}

SI32 CPatchDlg::GetGameVersionInFile()
{
	FILE* fp = NULL;
	TCHAR buffer[1024];

	SI32 gameversion = 0;
	SI16 siSuccess = 0;

	NFile file; 

	if(file.LoadFile( TEXT("Version.dat") ) == false)
	{
		return gameversion;
	}

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{

		NLoadCryptFile( TEXT("Version.dat"),  (BYTE*)buffer );
		buffer[ pHeader->dwDataSize ] = NULL;


	} else {

		fp = _tfopen( TEXT("Version.dat"), TEXT("rt") );
		if(fp == NULL)
		{
			MsgBox(TEXT(" No <Version.dat> file"), TEXT("Error"));
			return false;
		}

		_fgetts(buffer, 1024, fp);
	}

	siSuccess = _stscanf(buffer, TEXT("%d"), &gameversion );
	if( fp )fclose(fp);

	if(siSuccess == 1)
	{

	}
	else
	{
		gameversion = 0;
	}

	return gameversion;
}

bool CPatchDlg::SetGameVersionInFile(SI32 siVersion)
{
	if(siVersion <= 0)
		return false;

	char buffer[ 4096 ];
	buffer[ 0 ] = NULL;

	sprintf( buffer, TEXT("%d"), siVersion );
	int len = strlen( buffer );

	if( len > 0 ) NWriteCryptFile( TEXT("Version.dat"), (BYTE*)buffer, len );

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Kang Hyoungm Lee(riverbos@ndoors.net) - bool CPatchDlg::GetGameInfo()
//	Desc : 버젼정보를 얻어온다.
//	Date : 2007. 03. 21
//	Update :
//--------------------------------------------------------------------------------------------
bool CPatchDlg::GetGameInfo()
{
	m_uiCurrentVersion = GetGameVersionInFile();
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
	default:
		return false;
	}

	strcpy( m_strGameFullPath, m_strGameFolderPath );
	strcat( m_strGameFullPath, m_strGameExeFileName );

	strcpy( m_strPatchFolderPath, m_strGameFolderPath );
	strcat( m_strPatchFolderPath, PATCH_FOLDER);

	return m_uiCurrentVersion > 0 ? true : false;
}