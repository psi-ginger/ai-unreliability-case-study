// FieldObjectMergeDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "FieldObjectMerge.h"
#include "FieldObjectMergeDlg.h"
#include ".\fieldobjectmergedlg.h"
#include <process.h>

#include "../../NLib/NDataLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFieldObjectMergeDlg 대화 상자



CFieldObjectMergeDlg::CFieldObjectMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldObjectMergeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFieldObjectMergeDlg::~CFieldObjectMergeDlg()
{
	for( int i=0; i<m_arrFieldObject.GetSize(); i++ )
	{
		CFieldObject*	pkFieldObject = (CFieldObject*)m_arrFieldObject.GetAt( i );
		delete pkFieldObject;
		m_arrFieldObject.RemoveAt( i );
	}
}

void CFieldObjectMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFieldObjectMergeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, OnBnClickedButtonExecute)
END_MESSAGE_MAP()


// CFieldObjectMergeDlg 메시지 처리기

BOOL CFieldObjectMergeDlg::OnInitDialog()
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

void CFieldObjectMergeDlg::OnPaint() 
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
HCURSOR CFieldObjectMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFieldObjectMergeDlg::OnBnClickedButtonExecute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString		strPath;
	GetDlgItemText(IDC_EDIT_PATH, strPath);
	if( strPath[strPath.GetLength() - 1] == '\\' )
	{
		strPath.Delete( strPath.GetLength() - 1, 1 );
	}

	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	SI32 unique = 0 ;
	SI32 imagenumber = 0 ;
	TCHAR type[128] = TEXT("") ;
	TCHAR name[128] = TEXT("") ;
	SI32 transmode = 0 ;
	SI32 alphavalue = 0 ;
	SI32 shadow = 0 ; // 나무 그림자들
	TCHAR zipname[128] = TEXT("") ;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,		4,
		NDATA_MBSTRING,	type,			128,
		NDATA_INT32,	&imagenumber,	4,
		NDATA_MBSTRING,	name,			128,
		NDATA_INT32,	&transmode,		4,
		NDATA_INT32,	&alphavalue, 	4,
		NDATA_INT32,	&shadow,		4,
		NDATA_MBSTRING,	zipname,		128,
		0,0,0
	};

	CString	strFile;
	strFile = strPath;
	strFile += "\\Data\\FieldObjectSetInfo.txt";

	dataloader.RegDataType( pDataTypeInfo );
	bLoadFile = dataloader.LoadDataFromFile( (TCHAR*)(LPCTSTR)strFile );

	SI32 index = 0;
	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())
		{
			CFieldObject*	pnewfieldobject = new CFieldObject();
			pnewfieldobject->m_strName = name;
			pnewfieldobject->m_ZipName = zipname;

			m_arrFieldObject.Add( pnewfieldobject );
		}
	}

	int i;
    CString src, dest, path;

	for( i=0; i<m_arrFieldObject.GetSize(); i++ )
	{
		CFieldObject*	pkFieldObject = (CFieldObject*)m_arrFieldObject.GetAt( i );

		path.Format( "%s\\FieldObject\\%s.pak",  (LPCSTR)strPath, (LPCSTR)pkFieldObject->m_ZipName );
		CreateDirectory( path, NULL);

		path.Format( "%s\\FieldObject\\%s.pak\\%s",  (LPCSTR)strPath, (LPCSTR)pkFieldObject->m_ZipName, (LPCSTR)pkFieldObject->m_strName);
		CreateDirectory( path, NULL );

		src.Format("%s\\FieldObject\\%s\\%s1.mpr", (LPCSTR)strPath, (LPCSTR)pkFieldObject->m_strName, (LPCSTR)pkFieldObject->m_strName );
		dest.Format("%s\\FieldObject\\%s.pak\\%s\\%s1.mpr", (LPCSTR)strPath, (LPCSTR)pkFieldObject->m_ZipName, (LPCSTR)pkFieldObject->m_strName, (LPCSTR)pkFieldObject->m_strName );
		MoveFile( src, dest );

		src.Format("%s\\FieldObject\\%s\\%s.txt", (LPCSTR)strPath, (LPCSTR)pkFieldObject->m_strName, (LPCSTR)pkFieldObject->m_strName );
		dest.Format("%s\\FieldObject\\%s.pak\\%s\\%s.txt", (LPCSTR)strPath, (LPCSTR)pkFieldObject->m_ZipName, (LPCSTR)pkFieldObject->m_strName, (LPCSTR)pkFieldObject->m_strName );
		MoveFile( src, dest );
	}

	for( i=0; i<m_arrFieldObject.GetSize(); i++ )
	{
		CFieldObject*	pkFieldObject = (CFieldObject*)m_arrFieldObject.GetAt( i );
		delete pkFieldObject;
		m_arrFieldObject.RemoveAt( i );
	}

	CFileFind cf;
	CString		strFindfile;

	strFindfile = strPath;
	strFindfile += "\\FieldObject\\";
	strFindfile += "*.*";
	BOOL bRet = cf.FindFile( strFindfile );

	while ( bRet )
	{
		bRet = cf.FindNextFile();

		if( cf.IsDirectory() == FALSE )		continue;
		if( cf.IsDots() == TRUE )			continue;

		CString	src;
		CString dest;
		CString filename;
		CString workingdir;
		filename = cf.GetFileName();
		filename.MakeUpper();

		if( strncmp( (LPCSTR)filename, ".SVN", 4 ) == 0 )			continue;
		if( strncmp( (LPCSTR)filename, "_SVN", 4 ) == 0 )			continue;

		int nFind = filename.Find(".PAK");
		if( nFind < 0 )												continue;

		src.Format("\"%s\\FieldObject\\%s\\*\"", (LPCSTR)strPath, (LPCSTR)cf.GetFileName() );
		dest.Format("\"%s\\FieldObject\\%s\"", (LPCSTR)strPath, (LPCSTR)cf.GetFileName() );
		dest.Replace(".pak", ".mz");
		workingdir.Format("\"%s\\FieldObject\"", (LPCSTR)strPath);

		SetCurrentDirectory( workingdir );

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
