//=============================================================================
//
//	CryptToolDlg.cpp
//
//	Copyright 2005 HQGame
//
//=============================================================================

#include "stdafx.h"
#include "CryptTool.h"
#include "CryptToolDlg.h"
#include ".\crypttooldlg.h"

#include "FolderDlg.h"
#include "ViewerDialog.h"

#include "NUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//=============================================================================
//	CLASS   CAboutDlg
//! @brief  응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
//=============================================================================
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//=============================================================================
	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };
	//=============================================================================

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

protected:
	DECLARE_MESSAGE_MAP()
};

//=============================================================================
//  @Name   CAboutDlg()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  생성자
//=============================================================================
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

//=============================================================================
//  @Name   DoDataExchange()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//=============================================================================
// CAboutDlg - Message Map
//=============================================================================
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



//=============================================================================
// CCryptToolDlg 대화 상자
//=============================================================================

//=============================================================================
//  @Name   CCryptToolDlg()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  생성자
//=============================================================================
CCryptToolDlg::CCryptToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCryptToolDlg::IDD, pParent)
	, m_wKeyR(_T("0x0816"))
	, m_wKeyC1(_T("0x6081"))
	, m_wKeyC2(_T("0x1608"))
	, m_bDelText(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//=============================================================================
//  @Name   DoDataExchange()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CCryptToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTRESULT, m_lstResult);
	DDX_Text(pDX, IDC_EDTKEY_R, m_wKeyR);
	DDX_Text(pDX, IDC_EDTKEY_C1, m_wKeyC1);
	DDX_Text(pDX, IDC_EDTKEY_C2, m_wKeyC2);
	DDX_Control(pDX, IDC_LIST1, m_lstFolders);
	DDX_Check(pDX, IDC_CHKDELTEXT, m_bDelText);
}

//=============================================================================
// CCryptToolDlg - Message Map
//=============================================================================
BEGIN_MESSAGE_MAP(CCryptToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNPROCESS, OnBnClickedBtnprocess)
	ON_BN_CLICKED(IDC_BTNVIEW, OnBnClickedBtnview)
	ON_BN_CLICKED(IDC_BTNADD, OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNDEL, OnBnClickedBtndel)
	ON_BN_CLICKED(IDC_BTNDECODE, OnBnClickedBtndecode)
	ON_BN_CLICKED(IDC_BTNDECODEPROCESS, OnBnClickedBtndecodeprocess)
END_MESSAGE_MAP()


//=============================================================================
//  @Name   OnInitDialog()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  초기화 작업
//=============================================================================
BOOL CCryptToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 리스트 컨트롤 초기화 작업
	CRect	rc;
	m_lstResult.GetClientRect(rc);
	m_lstResult.ModifyStyle(LVS_ICON, LVS_REPORT, 0);
	m_lstResult.InsertColumn(0, "결과", LVCFMT_LEFT, rc.Width() - 5, 0);

	TCHAR	szPath[MAX_PATH*2];
	TCHAR	szValue[1024];
	CString	kConfig;

	// 현재 실행 디렉토리 생성
	GetModuleFileName(NULL, szPath, MAX_PATH*2);
	TCHAR	*pSep = HQStrrChr(szPath, '\\');
	szPath[pSep - szPath+1] = '\0';

	// 'config.ini'파일의 위치 생성
	kConfig = szPath;
	kConfig += "config.ini";

//	m_wKeyR.Format("0x%04X", HQCryptTextFile::HQ_CRYPTTEXTFILE_CRYPTOGRAPH_KEY_R);
//	m_wKeyC1.Format("0x%04X", HQCryptTextFile::HQ_CRYPTTEXTFILE_CRYPTOGRAPH_KEY_C1);
//	m_wKeyC2.Format("0x%04X", HQCryptTextFile::HQ_CRYPTTEXTFILE_CRYPTOGRAPH_KEY_C2);

	// 리스트박스에 PATH정보들을 추가 (구분자 ',')
	GetPrivateProfileString("PATH", "PATHES", "", szValue, 1024, kConfig);
	TCHAR *pToken = HQStrTok(szValue, ",");
	while (pToken != NULL)
	{
		m_lstFolders.AddString(pToken);
		pToken = HQStrTok(NULL, ",");
	}

	// 리스트 컨트롤에 이미지리스트 할당
	m_imgIcons.Create(IDB_ICONS, 20, 5, RGB(255, 255, 255));
	m_lstResult.SetImageList(&m_imgIcons, LVSIL_SMALL);

	UpdateData(FALSE);
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

//=============================================================================
//  @Name   OnSysCommand()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CCryptToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//=============================================================================
//  @Name   OnPaint()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.
void CCryptToolDlg::OnPaint() 
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

//=============================================================================
//  @Name   OnQueryDragIcon()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CCryptToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//=============================================================================
//  @Name   OnBnClickedBtnprocess()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CCryptToolDlg::OnBnClickedBtnprocess()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileFind			fileFinder;
	CString			kTxtPathName, stTxtName;
	CString	kFolder;
	TCHAR				szCryptTxtName[MAX_PATH * 2], szCryptTxtPathName[MAX_PATH*2];
	BOOL				bSuccess;
	CString				strResultString;

	// 다이얼로그의 데이터 할당
	UpdateData();

	// 리스트컨트롤의 모든 아이템 삭제
	m_lstResult.DeleteAllItems();
	// 폴더값이 저장된 리스트박스의 아이템 개수만큼 루프
	for (int i=0; i<m_lstFolders.GetCount(); i++)
	{
		// 폴더값 얻음
		CString	strText;
		m_lstFolders.GetText(i, strText);
		kFolder = strText;
		
		// 작업 시작
		strResultString.Format(_T("========= Process %s ========"), (LPCTSTR)kFolder);
		m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 2);

		// Text파일만 찾음
		kFolder += "\\*.txt";
		// 파일 찾기 시작
		if (!fileFinder.FindFile((LPCTSTR)kFolder))	return;

		// 모든 Text파일에 대해 작업
		while(TRUE)
		{
			// 다음 파일 찾음 - bSuccess 가 FALSE이면 다음 파일이 없다는 것으로
			// 마지막으로 작업하고 끝
			bSuccess = fileFinder.FindNextFile();
			
			stTxtName = fileFinder.GetFileName();		// 파일명
			NLibStrCpy(szCryptTxtName, stTxtName);			// 새로운 파일명에 복사
			kTxtPathName = fileFinder.GetFilePath();	// 폴더+파일명
			NLibStrCpy(szCryptTxtPathName, kTxtPathName);	// 새로운 폴더+파일명에 복사

			// 새로운 파일명의 확장자를 '.txe'로 변경
			char	*pSep;
			pSep = NLibStrrChr(szCryptTxtName, '.');
			szCryptTxtName[pSep - szCryptTxtName] = '\0';
			NLibStrCat(szCryptTxtName, ".txe");

			pSep = NLibStrrChr(szCryptTxtPathName, '.');
			szCryptTxtPathName[pSep - szCryptTxtPathName] = '\0';
			NLibStrCat(szCryptTxtPathName, ".txe");

			// HQCryptTextFile클래스의 static 함수를 사용하여 암호화 된 파일 생성
			switch (NWriteEnCryptFile(kTxtPathName, szCryptTxtPathName))
			{
				case 1 :
				{				
					// 작업 상황 출력
					strResultString.Format(_T("%s -> %s : Success"), (LPCTSTR)stTxtName, szCryptTxtName);
					m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 0);
					// 원본파일을 지우는 옵션이 활성화 된 경우 원본파일을 삭제
					//if (m_bDelText)			DeleteFile(kTxtPathName);
					DeleteFile(kTxtPathName);

					CopyFile(szCryptTxtPathName,kTxtPathName,false);

					DeleteFile(szCryptTxtPathName);
					break;
				}
				case 2 :
				{
					// 작업 상황 출력
					strResultString.Format(_T("%s -> %s : Already EnCrypted!!"), (LPCTSTR)stTxtName, szCryptTxtName);
					m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 1);
					break;
				}
				case 0 :
				{
					// 작업 상황 출력
					strResultString.Format(_T("%s -> %s : Fail"), (LPCTSTR)stTxtName, szCryptTxtName);
					m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 1);
					break;
				}

			}
			// 다음 파일이 없으면 루프 종료
			if (!bSuccess)		break;
		}
	}
}

//=============================================================================
//  @Name   OnBnClickedBtndecode()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  *.txe 파일을 선택하여 *.txt파일로 복호화
//=============================================================================
void CCryptToolDlg::OnBnClickedBtndecode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog	filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"CryptText Files (*.txt) |*.txt||", NULL);

	CString	kCryptTxtPathName;
	TCHAR				szTxtPathName[MAX_PATH*2];

	// 파일 선택창 실행
	if (filedlg.DoModal() == IDOK)
	{
		kCryptTxtPathName = filedlg.GetPathName();
		NLibStrCpy(szTxtPathName, kCryptTxtPathName);

		// 새로운 파일명의 확장자를 '.txt'로 변경
		char	*pSep;
		pSep = NLibStrrChr(szTxtPathName, '.');
		szTxtPathName[pSep - szTxtPathName] = '\0';
		NLibStrCat(szTxtPathName, ".txe");

		NWriteDeCryptFile(kCryptTxtPathName, szTxtPathName);

		DeleteFile((LPCTSTR)kCryptTxtPathName);

		CopyFile(szTxtPathName,kCryptTxtPathName,false);

		DeleteFile(szTxtPathName);
		
	}
}

//=============================================================================
//  @Name   OnBnClickedBtnview()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  암호화 된 텍스트의 내용을 보는 창 실행
//=============================================================================
void CCryptToolDlg::OnBnClickedBtnview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CViewerDialog	viewerdlg;

	// 텍스트 내용을 보는 창을 실행.
	viewerdlg.DoModal();
}

//=============================================================================
//  @Name   OnBnClickedBtnadd()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  리스트박스 컨트롤에 작업할 디렉토리를 추가하는 기능
//=============================================================================
void CCryptToolDlg::OnBnClickedBtnadd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog	folderdlg(_T("Text 파일의 위치를 선택하세요."), _T("C:\\GoonzuWorld"), this);

	// 폴더 선택창을 실행한다.
	if (folderdlg.DoModal() == IDOK)
	{
		m_lstFolders.AddString(folderdlg.GetFolderPath());
	}

	// 리스트박스 컨트롤에 있는 폴더들을 ','를 토큰으로 하여 문자열로 생성한다.
	CString	 kWrite;
	for (int i=0; i<m_lstFolders.GetCount(); i++)
	{
		// 리스트박스 컨트롤의 아이템 문자열 얻기
		CString strText;
		m_lstFolders.GetText(i, strText);

		// ','를 추가하여 문자열 생성
//		if (kWrite.Length() > 0)			
		kWrite += ",";

		kWrite += strText;
	}

	TCHAR		szPath[MAX_PATH*2];
	CString		kConfig;

	// 현재 실행 디렉토리 생성
	GetModuleFileName(NULL, szPath, MAX_PATH*2);
	TCHAR	*pSep = HQStrrChr(szPath, '\\');
	szPath[pSep - szPath+1] = '\0';

	// 'config.ini' 위치 생성
	kConfig = szPath;
	kConfig += "config.ini";

	// 'config.ini'에 생성한 문자열을 저장
	WritePrivateProfileString("PATH", "PATHES", (LPCTSTR)kWrite, (LPCTSTR)kConfig);
}


//=============================================================================
//  @Name   OnBnClickedBtndel()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  리스트박스 컨트롤에서 선택된 아이템 삭제
//=============================================================================
void CCryptToolDlg::OnBnClickedBtndel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 현재 선택된 아이템이 있다면 그 아이템 삭제
	if (m_lstFolders.GetCurSel() >= 0)
		m_lstFolders.DeleteString(m_lstFolders.GetCurSel());
	else
	// 선택된 것이 없으면 종료
		return;

	// 리스트 박스의 모든 아이템들을 ','를 기준으로 문자열로 생성
	CString kWrite;
	for (int i=0; i<m_lstFolders.GetCount(); i++)
	{
		// 아이템 문자열 얻음
		CString strText;
		m_lstFolders.GetText(i, strText);

		// 아이템과 아이템 사이에 ',' 추가
		//if (kWrite.Length() > 0)			
		kWrite += ",";
		kWrite += strText;
	}

	TCHAR		szPath[MAX_PATH*2];
	CString	kConfig;

	// 현재 실행 디렉토리 생성
	GetModuleFileName(NULL, szPath, MAX_PATH*2);
	TCHAR	*pSep = HQStrrChr(szPath, '\\');
	szPath[pSep - szPath+1] = '\0';

	// 'config.ini' 위치 생성
	kConfig = szPath;
	kConfig += "config.ini";

	// 'config.ini'에 생성한 문자열을 저장
	WritePrivateProfileString("PATH", "PATHES", (LPCTSTR)kWrite, (LPCTSTR)kConfig);
}

//=============================================================================
//  @Name   OnBnClickedBtndecodeprocess()
//! @author LeeKH						
//! @date   2006-1-23
//! @param  X   X
//! @return X
//! @brief  등록된 디렉토리내의 모든 암호화 파일을 txt파일로 복호화
//=============================================================================
void CCryptToolDlg::OnBnClickedBtndecodeprocess()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileFind			fileFinder;
	CString	kCryptTxtName, kCryptTxtPathName;
	CString	kFolder;
	TCHAR				szTxtName[MAX_PATH * 2], szTxtPathName[MAX_PATH*2];
	BOOL				bSuccess;
	CString				strResultString;

	// 다이얼로그의 데이터 할당
	UpdateData();

	// 리스트컨트롤의 모든 아이템 삭제
	m_lstResult.DeleteAllItems();
	// 폴더값이 저장된 리스트박스의 아이템 개수만큼 루프
	for (int i=0; i<m_lstFolders.GetCount(); i++)
	{
		// 폴더값 얻음
		CString	strText;
		m_lstFolders.GetText(i, strText);
		kFolder = strText;

		// 작업 시작
		strResultString.Format(_T("========= Process %s ========"), (LPCTSTR)kFolder);
		m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 2);

		// Txe파일만 찾음
		kFolder += "\\*.txt";
		// 파일 찾기 시작
		if (!fileFinder.FindFile((LPCTSTR)kFolder))	return;

		// 모든 Text파일에 대해 작업
		while(TRUE)
		{
			// 다음 파일 찾음 - bSuccess 가 FALSE이면 다음 파일이 없다는 것으로
			// 마지막으로 작업하고 끝
			bSuccess = fileFinder.FindNextFile();

			kCryptTxtName = fileFinder.GetFileName();		// 파일명
			NLibStrCpy(szTxtName, kCryptTxtName);			// 새로운 파일명에 복사
			kCryptTxtPathName = fileFinder.GetFilePath();	// 폴더+파일명
			NLibStrCpy(szTxtPathName, kCryptTxtPathName);	// 새로운 폴더+파일명에 복사

			// 새로운 파일명의 확장자를 '.txt'로 변경
			char	*pSep;
			pSep = NLibStrrChr(szTxtName, '.');
			szTxtName[pSep - szTxtName] = '\0';
			NLibStrCat(szTxtName, ".txe");

			pSep = NLibStrrChr(szTxtPathName, '.');
			szTxtPathName[pSep - szTxtPathName] = '\0';
			NLibStrCat(szTxtPathName, ".txe");

			// HQCryptTextFile클래스의 static 함수를 사용하여 암호화 된 파일 생성
			if (NWriteDeCryptFile(kCryptTxtPathName, szTxtPathName))
			{
				// 작업 상황 출력
				strResultString.Format(_T("%s -> %s : Success"), (LPCTSTR)kCryptTxtName, szTxtName);
				m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 0);
				// 원본파일을 지우는 옵션이 활성화 된 경우 원본파일을 삭제
				//if (m_bDelText)			DeleteFile((LPCTSTR)kCryptTxtPathName);
				DeleteFile((LPCTSTR)kCryptTxtPathName);

				CopyFile(szTxtPathName,kCryptTxtPathName,false);

				DeleteFile(szTxtPathName);
			}
			else
			{
				// 작업 상황 출력
				strResultString.Format(_T("%s -> %s : Fail"),  (LPCTSTR)kCryptTxtName, szTxtName);
				m_lstResult.InsertItem(m_lstResult.GetItemCount(), strResultString, 1);
			}

			// 다음 파일이 없으면 루프 종료
			if (!bSuccess)		break;
		}
	}
}
