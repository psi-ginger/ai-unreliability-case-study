// FrontServerListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "GoonZuMCDlg.h"
#include "FrontServerListDlg.h"
#include "../MasterServer/ServerInfo.h"
#include "../MasterServer/MasterMsg.h"
#include "MasterProcess.h"
#include "ITZSendBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrontServerListDlg dialog


CFrontServerListDlg::CFrontServerListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrontServerListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrontServerListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFrontServerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrontServerListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrontServerListDlg, CDialog)
	//{{AFX_MSG_MAP(CFrontServerListDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FRONTON, OnButtonFronton)
	ON_BN_CLICKED(IDC_BUTTON_FRONTOFF, OnButtonFrontoff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrontServerListDlg message handlers

BOOL CFrontServerListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//// Frontserver DataList
	//	리스트뷰 생성
	m_FrontServerList.Create(0,::GetDlgItem(m_hWnd,IDC_LIST_FRONTSERVER));
	//	리스트 뷰 칼럼 만들기
	m_FrontServerList.Set_Col(TEXT("Type"),140);
	m_FrontServerList.Set_Col(TEXT("Value"),120);
	const SI32	ConstFrontServerDataTypeCount = 6;
	// 서버 상태, 작동 시작 시각, 클라이언트 접속 수, 마지막 패치 시간, 게임 버전, 메시지 송수신량
	/*TCHAR *pszFrontserverData[] = { TEXT("Server State"),
					   TEXT("Power-On Time"), 
					   TEXT("Client Connect"), 
					   TEXT("Last Patch Time"), 
					   TEXT("Game Version"), 
					   TEXT("Message Traffic")};*/
	TCHAR *pszFrontserverData[] = { TEXT("Server State"),
						TEXT("AuthServer Connect"), 
						TEXT("GameServer Connect"), 
						TEXT("Client Connect"), 
						TEXT("ProcessID"), 
						TEXT("Game Version")};

	for(SI32 i = 0; i < ConstFrontServerDataTypeCount; ++i ) {

		m_FrontServerList.Set_Data(i,0,pszFrontserverData[i]);
	}

	//// Authserver DataList
	//	리스트뷰 생성
	m_AuthServerList.Create(0,::GetDlgItem(m_hWnd,IDC_LIST_AUTHSERVER));
	//	리스트 뷰 칼럼 만들기
	m_AuthServerList.Set_Col(TEXT("Type"),140);
	m_AuthServerList.Set_Col(TEXT("Value"),120);

	const SI32  ConstAuthServerDataType = 3;
	// 서버 상태, 작동 시작 시각, 메시지 송수신량
	TCHAR *pszAuthserverData[] = { TEXT("Server State"), TEXT("Power-On Time"), TEXT("Message Traffic")};

	for(SI32 i = 0; i < ConstAuthServerDataType; ++i ) {

		m_AuthServerList.Set_Data(i,0,pszAuthserverData[i]);
	}

	// 서버에서 첫 메시지가 올때 까지 대기
	Sleep(4000);

	SetTimer( 100, 1000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFrontServerListDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( nIDEvent == 100 ) {

		char buf[ 64 ];

		CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

		ServerInfo *pServerInfo = pApp->m_pServerInfo;

		pServerInfo->Wait();

		BOOL bServerError = FALSE;

		// 서버 상태
		if( pServerInfo->m_frontRefresh.usStatus == 0 ) {
			strcpy( buf, "OK" );
		} else {
			strcpy( buf, "No Heartbeat" );
			bServerError = TRUE;
		}

		m_FrontServerList.Set_Data(0,1,buf);

		//AuthServer 연결
		m_FrontServerList.Set_Data(1,1,pServerInfo->m_frontRefresh.siAuthConnect);

		if( pServerInfo->m_frontRefresh.siAuthConnect == 0 ) bServerError = TRUE;
		//GameServer 연결
		m_FrontServerList.Set_Data(2,1,pServerInfo->m_frontRefresh.siGameServerConnect);

		if( pServerInfo->m_frontRefresh.siGameServerConnect == 0 ) bServerError = TRUE;
		//Client 연결
		m_FrontServerList.Set_Data(3,1,pServerInfo->m_frontRefresh.siUserNum);

		//Process ID
		m_FrontServerList.Set_Data(4,1,pServerInfo->m_frontRefresh.dwProcessId);

		//GameVersion
		m_FrontServerList.Set_Data(5,1,pServerInfo->m_frontRefresh.siGameVersion);

		//// 작동 시작 시각
		////m_FrontServerList.Set_Data(1,1,pServerInfo->m_frontRefresh.siAuthConnect);

		//if( pServerInfo->m_frontRefresh.siAuthConnect == 0 ) bServerError = TRUE;

		//// 클라이언트 접속 수
		//m_FrontServerList.Set_Data(2,1,pServerInfo->m_frontRefresh.siUserNum);

		//if( pServerInfo->m_frontRefresh.siGameServerConnect == 0 ) bServerError = TRUE;

		////  마지막 패치 시간
		////m_FrontServerList.Set_Data(3,1,pServerInfo->m_frontRefresh.siUserNum);

		////if( pServerInfo->m_frontRefresh.siUserNum == 0 ) bServerError = TRUE;

		//// 게임 버전
		//m_FrontServerList.Set_Data(4,1,pServerInfo->m_frontRefresh.siGameVersion);
		//// 메시지 송수신량
		////m_FrontServerList.Set_Data(5,1,pServerInfo->m_frontRefresh.siGameVersion);

		pServerInfo->Release();
		
		CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());

		if( bServerError ) {
		
			pDlg->m_bError = TRUE;
			pDlg->m_errorCode = pDlg->m_errorCode | 1;

		} else {
			if( !(pDlg->m_errorCode & (~1) ) ) {
				pDlg->m_bError = FALSE;
			}

			pDlg->m_errorCode = pDlg->m_errorCode & (~1);			
			
		}

	}


	CDialog::OnTimer(nIDEvent);
}


void CFrontServerListDlg::OnButtonFronton() 
{
	// TODO: Add your control notification handler code here

	sRequest_GMCFrontServerOn sendMsg;

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	pApp->m_pMasterProcess->m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

}


void CFrontServerListDlg::OnButtonFrontoff() 
{
	// TODO: Add your control notification handler code here
	
	sRequest_GMCFrontServerOff sendMsg;

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	pApp->m_pMasterProcess->m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

}



BOOL CFrontServerListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
