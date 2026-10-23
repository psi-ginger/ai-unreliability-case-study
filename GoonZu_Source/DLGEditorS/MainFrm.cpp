// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "DLGEditorS.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include "./MyDefine/MyDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_StartPath;	// 시작 path
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_pSplitterWnd = NULL;
	m_pSplitterWnd2 = NULL;
}

CMainFrame::~CMainFrame()
{
	if(m_pSplitterWnd != NULL) delete m_pSplitterWnd;
	if(m_pSplitterWnd2 != NULL) delete m_pSplitterWnd2;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndStatusBar.SetPaneInfo( 0, ID_SEPARATOR, SBPS_NORMAL, 20);
	m_wndStatusBar.SetPaneInfo( 1, ID_SEPARATOR, SBPS_NORMAL, 300);
	m_wndStatusBar.SetPaneInfo( 2, ID_SEPARATOR, SBPS_NORMAL, 300);

	//if (!m_wndDialogBar.Create(this, IDD_DLGPROPERTYDLG/*IDD_DLGBAR_BASE*/, CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC, 
	//	IDD_DLGPROPERTYDLG/*IDD_DLGBAR_BASE*/))
	//{
	//	TRACE0("다이얼로그바를 만들지 못했습니다.\n");
	//	return -1;
	//}

	char curPath[512];
	char fullPath[1024];
	memset(curPath, 0, 512);
	memset(fullPath, 0, 512);
	DWORD length = GetCurrentDirectory(512, curPath);
	// patchinfo
	_snprintf(fullPath, 1024, "%s\\DLGSetting.txt", curPath);

	g_StartPath = fullPath;

	//m_wndDialogBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndDialogBar);
	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.
	cs.x = WIN_STARTX;
	cs.y = WIN_STARTY;
	cs.cx = WIN_WIDTH;
	cs.cy = WIN_HEIGHT;

	SetTitle(WIN_TITLE);

	return true;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	// //SplitterWnd 쓸려면..
	//// Base SplitterWnd 생성!!
	//m_pSplitterWnd = new CSplitterWnd_Base();

	//m_pSplitterWnd->Create( this, RUNTIME_CLASS(CSplitterWndViewToolBar), RUNTIME_CLASS(CSplitterWndView), pContext, true );
	//
	//m_nViewNo[0] = m_pSplitterWnd->AddView(RIGHT_SIDE, RUNTIME_CLASS(CSplitterWndView), pContext);
	////m_nViewNo[1] = m_pSplitterWnd->AddView(RIGHT_SIDE, RUNTIME_CLASS(CSplitterWndView), pContext);

	//// empty function
	//m_pSplitterWnd->SetInitialStatus();
	//
	//return true;
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);

	if ( g_bMagnetMode == FALSE ) return;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strDialogName[] = {"버튼 컨트롤 속성"
		, "다이얼로그 속성창"
		, "콤보박스 컨트롤 속성"
		, "에디트박스 컨트롤 속성"
		, "고정 텍스트 속성"
		, "리스트박스 컨트롤 속성"
		, "고정 이미지 컨트롤 속성"
		, "리스트뷰 컨트롤 속성"
		, "체크박스 컨트롤 속성"
		, "라디오버튼 컨트롤 속성"
		, "외곽선 그리기 속성"
		, "채팅 에디트 속성"
	};


	CWnd* pwProperty = NULL;

	

	for (int i=0; i<12; i++)
	{
		pwProperty = FindWindow( NULL, strDialogName[i] );
		
		if ( pwProperty )
		{
			if ( !pwProperty->IsWindowVisible() ) continue;

			CRect rectParent, rectChile;
		
			this->GetWindowRect( rectParent );
			pwProperty->GetWindowRect( rectChile );

			rectChile.MoveToXY( (rectParent.right), rectParent.top );
			pwProperty->MoveWindow( rectChile );
			
			break;
		}
		pwProperty = NULL;
	}

}