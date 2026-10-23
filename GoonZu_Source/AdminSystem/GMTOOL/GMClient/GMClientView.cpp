// Id\IdView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "../MainFrm.h"

#include "GMClientView.h"
#include ".\gmclientview.h"

// CGMClientView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGMClientView, CDialog)
CGMClientView::CGMClientView(CWnd* pParent /*=NULL*/)
	: CDialog(CGMClientView::IDD, pParent)
{
	m_clGMProcess = new GMProcess(this);
	m_clGMProcess->Init();

	m_strGMCmd = "";
	
	//m_clGMProcess = new GMProcess(this);
}

CGMClientView::~CGMClientView()
{
	delete m_clGMProcess;
}

void CGMClientView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITID, m_LoginID);
	DDX_Control(pDX, IDC_EDITPW, m_LoginPW);
	DDX_Control(pDX, IDC_LOGINBUTTON, m_LoginButton);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_GMCommand);
	DDX_Control(pDX, IDC_SEARCHBUTTON, m_SearchButton);
	DDX_Control(pDX, IDC_INFOBOX, m_InfoBox);
	DDX_Control(pDX, IDC_LOGINSECTION, m_LoginSection);
}

BEGIN_MESSAGE_MAP(CGMClientView, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_LOGINBUTTON, OnBnClickedLoginbutton)
	ON_BN_CLICKED(IDC_SEARCHBUTTON, OnBnClickedSearchbutton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
END_MESSAGE_MAP()


// CGMClientView 메시지 처리기입니다.

void CGMClientView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CGMClientView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetTimer(100, 200, NULL);
	SetTimer(200, 200, NULL);
    	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 로그인 버튼 클릭시 
void CGMClientView::OnBnClickedLoginbutton()
{
	// 해당 ID와 PW를 읽어옴
	m_LoginID.GetWindowText(m_strID);
	m_LoginPW.GetWindowText(m_strPW);

	// 서버 접속
	m_clGMProcess->Connect("192.168.100.89", 11400);
}


// Excute 버튼 클릭시 
void CGMClientView::OnBnClickedSearchbutton()
{
	m_GMCommand.GetWindowText(m_strGMCmd);

	if( m_clGMProcess->IsConnected() && m_strGMCmd.GetLength() > 0 );
	{
		// 커맨드 보내기
		sGMRequest_GMCommand GMCmd(m_strGMCmd);
		m_clGMProcess->SendPacket((sPacketHeader*)&GMCmd);

		m_strGMCmd = "";

		//sDBRequest_BasicInfoByName 
	}
}


void CGMClientView::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch( nIDEvent )
	{
	case 100:
		{
			BOOL IsBeforeLogin = m_clGMProcess->IsLoggedIn();

			m_clGMProcess->Run();

			if( m_clGMProcess->IsConnected() &&
				m_strID.GetLength() > 0 && m_strPW.GetLength() > 0 )
			{
				// 로그인 시도
				sGMRequest_Login Login(m_strID, m_strPW);
				m_clGMProcess->SendPacket((sPacketHeader*)&Login);
				
				m_strID = "";
				m_strPW = "";
			}

			// 로그인 성공 시점
			if( IsBeforeLogin == FALSE && m_clGMProcess->IsLoggedIn() )
			{
				// 로그인 창 비활성
				GetDlgItem(IDC_EDITID)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDITPW)->EnableWindow(FALSE);
				GetDlgItem(IDC_LOGINBUTTON)->EnableWindow(FALSE);
				GetDlgItem(IDC_LOGINSECTION)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATICID)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATICPW)->EnableWindow(FALSE);

				AfxMessageBox("로그인 성공");
			}
			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// 화면에 줄단위로 출력하기
void CGMClientView::AddInfoFromServer(const char* pszLine)
{
	CString tmp;
	m_InfoBox.GetWindowText(tmp);
	if( tmp.GetLength() > 0 )	{		tmp += "\r\n";		}
	tmp += pszLine;
	m_InfoBox.SetWindowText(tmp);
}
void CGMClientView::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
