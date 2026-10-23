// GMTOOL.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "GMTOOL.h"
#include "MainFrm.h"

#include "Log/LogFrame.h"
#include "NewLog/NewLogFrame.h"
#include "Id/IdFrame.h"
#include "Guild/GuildFrame.h"
#include "Char/CharFrame.h"
#include "Item/ItemFrame.h"
#include "Block/BlockFrame.h"
#include "GMClient/GMClientFrame.h"
//#include "StaticCenterFrame.h"
//#include "InformationChangeFrame.h"
//#include "CharactorRecoveryFrame.h"
#include "Notice/NoticeFrame.h"
#include "TradeLog/TradeLogFrame.h"
#include "SelledLog/SelledLogFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGMTOOLApp

BEGIN_MESSAGE_MAP(CGMTOOLApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MENU_LOG, OnMenuLog)
	ON_COMMAND(ID_MENU_GUILD, OnMenuGuild)
	ON_COMMAND(ID_MENU_ID, OnMenuId)
	ON_COMMAND(ID_MENU_CHAR, OnMenuChar)
	//ON_COMMAND(ID_MENU_ITEM, OnMenuItem)
	//ON_COMMAND(ID_MENU_BLOCK, OnMenuBlock)
	//ON_COMMAND(ID_MENU_STATICS, OnMenuStatics)
	//ON_COMMAND(ID_MENU_INFOR, OnMenuInfor)
	//ON_COMMAND(ID_MENU_RECOVER, OnMenuRecover)
	ON_COMMAND(ID_MENU_NOTICE, OnMenuNotice)
	ON_COMMAND(ID_MENU_BLOCKCHAR, OnMenuBlockchar)
	//ON_COMMAND(ID_MENU_ITEM, OnMenuItem)
	ON_COMMAND(ID_MENU_ITEM, OnMenuItem)
	ON_COMMAND(32806, OnMenuNewLog)
	ON_COMMAND(ID_MENU_GMCLIENT, OnMenuGmclient)
	ON_COMMAND(ID_MENU_TRADELOG, OnMenuTradeLog)
	ON_COMMAND(ID_MENU_SELLEDLOG, OnMenuSelledLog)
END_MESSAGE_MAP()


// CGMTOOLApp 생성

CGMTOOLApp::CGMTOOLApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CGMTOOLApp 개체입니다.

CGMTOOLApp theApp;

// CGMTOOLApp 초기화

BOOL CGMTOOLApp::InitInstance()//초기화 함수부분.
{
//	_CrtSetBreakAlloc(406);

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// 소켓 라이브러리 초기화
	AfxSocketInit();

	
	char	szPath[MAX_PATH];
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';
	strcpy(szPath, szExecuteFile);

	//strcpy(szIniPath, szPath);
	TCHAR	szIniPath[MAX_PATH] = { '\0', };
	strcpy(szIniPath, "LogData\\Setting.ini");

	DWORD	dwLen;
	TCHAR	szRegistryKey[MAX_PATH] = { '\0', };
	TCHAR	szServiceArea[64] = { '\0', };
	TCHAR	szLanguage[32] = { '\0', };

	if (SERVICEAREA == ConstServiceArea_Korea)
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\NDOORS\\GoonZuS"));
		StringCchCopy(szLanguage, 32 , TEXT("KOREAN"));
	}
	else if (SERVICEAREA == ConstServiceArea_Taiwan)
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\NDOORS\\GoonZuTaiwan"));
		StringCchCopy(szLanguage, 32 , TEXT("TAIWAN"));
	}
	else if (SERVICEAREA == ConstServiceArea_Japan)
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\GamePot\\Kunshu Online"));
		StringCchCopy(szLanguage, 32 , TEXT("JAPAN"));
	}
	else if (SERVICEAREA == ConstServiceArea_English)
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\NDOORS\\GoonZuEng"));
		StringCchCopy(szLanguage, 32 , TEXT("ENGLISH"));
	}
	else if (SERVICEAREA == ConstServiceArea_EUROPE)
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\NDOORS\\GoonZuEurope"));
		StringCchCopy(szLanguage, 32 , TEXT("EUROPE"));
	}
	else if (SERVICEAREA == ConstServiceArea_NHNChina)
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\NDOORS\\GoonZuC"));
		StringCchCopy(szLanguage, 32 , TEXT("CHINA"));
	}
	else
	{
		StringCchCopy(szRegistryKey, MAX_PATH , TEXT("Software\\NDOORS\\GoonZuS"));
		StringCchCopy(szLanguage, 32 , TEXT("KOREAN"));
	}

	HKEY	hKey;
	TCHAR	szGamePath[MAX_PATH] = { '\0', };

	//KHY - 0112 - 중국은 레지스트리를 쓰지 않고, 직접복사한다.
	if(SERVICEAREA == ConstServiceArea_NHNChina)
	{
		strcpy(szGamePath, szRegistryKey);
	}
	else
	{
		RegCreateKey(HKEY_CURRENT_USER, szRegistryKey, &hKey);
		if(hKey != NULL)
		{
			DWORD ldwSize = MAX_PATH;
			RegQueryValueEx( hKey, "PATH", 0, NULL, (LPBYTE)szGamePath, &ldwSize );
			RegCloseKey( hKey );
			hKey = NULL;
		}
	}

	m_kDataManager.m_siServiceArea = ConstServiceArea_USA;
	m_kDataManager.Init( szLanguage);

	TCHAR	szCurrentDir[MAX_PATH];

	if( szGamePath != NULL && _tcslen(szGamePath) > 0 )
	{
		GetCurrentDirectory(MAX_PATH, szCurrentDir);
		SetCurrentDirectory(szGamePath);
	}

	m_kDataManager.InitGameData( SERVICEAREA);

	m_clItemManagerClient.InIt(m_kDataManager);
	m_clItemManagerClient.SetItemData(SERVICEAREA);

	if( szGamePath != NULL && _tcslen(szGamePath) > 0 )
	{
		SetCurrentDirectory(szCurrentDir);
	}

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 주 MDI 프레임 창을 만듭니다.
	if (!pFrame->LoadFrame(IDR_MAINFRAME1))
		return FALSE;
	// 공유 MDI 메뉴 및 액셀러레이터 키 테이블의 로드를 시도합니다.
	//TODO: 추가 멤버 변수를 추가하고 응용 프로그램에 필요한 추가 메뉴 형식에 대한
	//	 호출을 로드합니다.
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_GMTOOLTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_GMTOOLTYPE));
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();

	//OnMenuLog();

	return TRUE;
}


// CGMTOOLApp 메시지 처리기

int CGMTOOLApp::ExitInstance() 
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void CGMTOOLApp::OnMenuLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// 새 MDI 자식 창을 만듭니다.
	pFrame->CreateNewChild(RUNTIME_CLASS(CLogFrame), IDR_GMTOOL_LOG, m_hMDIMenu, m_hMDIAccel);
}


void CGMTOOLApp::OnMenuId()
{
	return;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame,m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CIdFrame), IDR_GMTOOL_ID, m_hMDIMenu, m_hMDIAccel);

}

void CGMTOOLApp::OnMenuGuild()
{
	return;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CGuildFrame),IDR_GMTOOL_GUILD, m_hMDIMenu, m_hMDIAccel );
}




// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

	// 구현
protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnMenuGuild();
	//	afx_msg void OnMenuItem();
	afx_msg void OnMenuChar();
	//	afx_msg void OnMenuInfor();
	afx_msg void OnMenuNotice();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CGMTOOLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CGMTOOLApp 메시지 처리기






void CGMTOOLApp::OnMenuChar()
{
	return;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CMDIChildWnd* pkMDIChild = pFrame->GetChild(CHILD_CHAR);
	if(pkMDIChild == NULL || pkMDIChild->m_hWnd == NULL)
	{
		CMDIChildWnd* pkNewChild = pFrame->CreateNewChild(RUNTIME_CLASS(CCharFrame),IDR_GMTOOL_CHAR, m_hMDIMenu, m_hMDIAccel );
		pFrame->SetChild(CHILD_CHAR, pkNewChild);
	}
	else
	{
		CMDIChildWnd* pkMDIChild = pFrame->GetChild(CHILD_CHAR);
		pFrame->MDIActivate(pkMDIChild);
	}
}


//void CGMTOOLApp::OnMenuItem()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
//
//	pFrame->CreateNewChild(RUNTIME_CLASS(CItemFrame),IDR_GMTOOL_ITEM, m_hMDIMenu, m_hMDIAccel );
//}
//


//void CGMTOOLApp::OnMenuStatics()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
//
//	pFrame->CreateNewChild(RUNTIME_CLASS(CStaticCenterFrame),IDR_GMTOOL_STATICS, m_hMDIMenu, m_hMDIAccel );
//
//}


//void CGMTOOLApp::OnMenuInfor()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
//
//	pFrame->CreateNewChild(RUNTIME_CLASS(CInformationChangeFrame),IDR_GMTOOL_CHANGEINFOR, m_hMDIMenu, m_hMDIAccel );
//
//}



//void CGMTOOLApp::OnMenuRecover()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//
//	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
//
//	pFrame->CreateNewChild(RUNTIME_CLASS(CCharactorRecoveryFrame),IDR_GMTOOL_RECOVER, m_hMDIMenu, m_hMDIAccel );
//}


void CGMTOOLApp::OnMenuNotice()
{
	return;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CNoticeFrame),IDR_GMTOOL_NOTICE, m_hMDIMenu, m_hMDIAccel );


}

void CGMTOOLApp::OnMenuBlockchar()
{
	return;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CBlockFrame),IDR_GMTOOL_BLOCKCHAR, m_hMDIMenu, m_hMDIAccel );
}

void CGMTOOLApp::OnMenuTradeLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CTradeLogFrame),IDR_GMTOOL_TRADELOG, m_hMDIMenu, m_hMDIAccel );
}

void CGMTOOLApp::OnMenuSelledLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CSelledLogFrame),IDR_GMTOOL_SELLEDLOG, m_hMDIMenu, m_hMDIAccel );
}

//void CGMTOOLApp::OnMenuItem()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
//	pFrame->CreateNewChild(RUNTIME_CLASS(CItemFrame),IDR_GMTOOL_ITEM, m_hMDIMenu, m_hMDIAccel );
//}

void CGMTOOLApp::OnMenuItem()
{
	return;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	pFrame->CreateNewChild(RUNTIME_CLASS(CItemFrame),IDR_GMTOOL_ITEM, m_hMDIMenu, m_hMDIAccel );
}

CNewLogFrame* CGMTOOLApp::CreatNewLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// 새 MDI 자식 창을 만듭니다.
	CNewLogFrame* pNewLogFrame = (CNewLogFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CNewLogFrame), IDR_GMTOOL_NEWLOG, m_hMDIMenu, m_hMDIAccel);
	pNewLogFrame->m_pclItemManagerClient = &m_clItemManagerClient;

	pNewLogFrame->ActivateFrame( SW_NORMAL);

	return pNewLogFrame;
}

void CGMTOOLApp::OnMenuNewLog()
{
	CreatNewLog();
}

void CGMTOOLApp::OnMenuGmclient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// 새 MDI 자식 창을 만듭니다.
	pFrame->CreateNewChild(RUNTIME_CLASS(CGMClientFrame), IDR_GMTOOL_GMCLIENT, m_hMDIMenu, m_hMDIAccel);
}
