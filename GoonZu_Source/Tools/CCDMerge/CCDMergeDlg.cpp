// CCDMergeDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "CCDMerge.h"
#include "CCDMergeDlg.h"
#include ".\ccdmergedlg.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCCDMergeDlg 대화 상자



CCCDMergeDlg::CCCDMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCCDMergeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCCDMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCCDMergeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FINDPATH, OnBnClickedButtonFindpath)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, OnBnClickedButtonExecute)
END_MESSAGE_MAP()


// CCCDMergeDlg 메시지 처리기

BOOL CCCDMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CCCDMergeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CCCDMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCCDMergeDlg::OnBnClickedButtonFindpath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CCCDMergeDlg::OnBnClickedButtonExecute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString		strPath;
	GetDlgItemText(IDC_EDIT_PATH, strPath);
	if( strPath[strPath.GetLength() - 1] == '\\' )
	{
		strPath.Delete( strPath.GetLength() - 1, 1 );
	}

	strPath += "\\CCD\\ImageData";

	CString		strFindfile;
	strFindfile = strPath;
	strFindfile += "\\";
	strFindfile += "*.spr";

	CFileFind cf;
	BOOL bRet = cf.FindFile( strFindfile );
	while ( bRet )
	{
		bRet = cf.FindNextFile();

		CString	dest;
		CString folder;
		CString filename;
		filename = cf.GetFileName();
		folder = cf.GetFileName().Mid(0, 5);
		folder.MakeUpper();
		if( strncmp( (LPCSTR)folder, "KOR", 3 ) == 0 )			continue;

		dest.Format("%s\\%s", (LPCSTR)strPath, (LPCSTR)folder );
		CreateDirectory( dest, NULL );
		dest.Format("%s\\%s\\%s", (LPCSTR)strPath, (LPCSTR)folder, (LPCSTR)cf.GetFileName() );
		MoveFile( cf.GetFilePath(), dest );

		_AFX_THREAD_STATE* pState = AfxGetThreadState();
		if( pState )
		{
			if( PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE) )
			{
				if( GetMessage(&(pState->m_msgCur), NULL, NULL, NULL) )
				{
					TranslateMessage(&(pState->m_msgCur));
					DispatchMessage(&(pState->m_msgCur));
				}
			}
		}
	}

	strFindfile = strPath;
	strFindfile += "\\";
	strFindfile += "*.*";
	bRet = cf.FindFile( strFindfile );

	while ( bRet )
	{
		bRet = cf.FindNextFile();

		if( cf.IsDirectory() == FALSE )		continue;
		if( cf.IsDots() == TRUE )			continue;

		CString	src;
		CString folder;
		CString dest;
		CString filename;
		filename = cf.GetFileName();
		folder = cf.GetFileName().Mid(0, 5);
		folder.MakeUpper();

		if( strncmp( (LPCSTR)folder, ".SVN", 4 ) == 0 )			continue;
		if( strncmp( (LPCSTR)folder, "_SVN", 4 ) == 0 )			continue;

		src.Format("\"%s\\%s\\*\"", (LPCSTR)strPath, (LPCSTR)folder );
		dest.Format("\"%s\\%s.sz\"", (LPCSTR)strPath, (LPCSTR)folder );

		HANDLE hProcess = (HANDLE)spawnl( P_NOWAIT, "C:\\Program Files\\7-zip\\7z.exe", 
											"\"C:\\Program Files\\7-zip\\7z.exe\"",
											"a",
											"-tzip",
											(LPCSTR)dest,
											(LPCSTR)src,
											NULL);


		int nExitCode = STILL_ACTIVE;
		if( hProcess != INVALID_HANDLE_VALUE )
		{
			while(nExitCode == STILL_ACTIVE)
			{
				GetExitCodeProcess( hProcess, (LPDWORD)&nExitCode );

				_AFX_THREAD_STATE* pState = AfxGetThreadState();
				if( pState )
				{
					if( PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE) )
					{
						if( GetMessage(&(pState->m_msgCur), NULL, NULL, NULL) )
						{
							TranslateMessage(&(pState->m_msgCur));
							DispatchMessage(&(pState->m_msgCur));
						}
					}
				}
			}
			if( nExitCode == 0 )
			{
				CListBox* pkListBox_Output = (CListBox*)GetDlgItem(IDC_LIST_OUTPUT);
				INT nItem = pkListBox_Output->GetCount();
				CString tmp;
				tmp.Format("[%s] Create Success...", dest);
				pkListBox_Output->AddString(tmp);
			}
			else
			{
				CListBox* pkListBox_Output = (CListBox*)GetDlgItem(IDC_LIST_OUTPUT);
				INT nItem = pkListBox_Output->GetCount();
				CString tmp;
				tmp.Format("[%s] Create Failed!!!!", dest);
				pkListBox_Output->AddString(tmp);
			}
		}
	}
}
