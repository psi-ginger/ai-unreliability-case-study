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
	
	// TODO: Add extra initialization here
	
	// 리스트 컨트롤 초기화 
	SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	int i;

	LVCOLUMN lvcol;
	LVITEM lvi;

	int width[] = { 140, 120 };
	char *text[] = { "", "" };

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	for( i = 0; i < 2; ++i ) {
		lvcol.cx = width[ i ];
		lvcol.pszText = text[ i ];
		lvcol.iSubItem = i;
		
		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
	}

	char *text2[] = { "Server State", "AuthServer Connect", "GameServer Connect", "Client Connect", "ProcessID", "Game Version" };

	lvi.mask = LVIF_TEXT;

	for( i = 0; i < 6; ++i ) {

		lvi.iItem = i;

		lvi.iSubItem = 0;
		lvi.pszText = text2[ i ];

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_INSERTITEM, 0, (LPARAM)&lvi );
	}

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

		LVITEM lvi;		
		lvi.mask = LVIF_TEXT;	

		lvi.iSubItem = 1;

		BOOL bServerError = FALSE;

		// 서버 상태
		if( pServerInfo->m_frontRefresh.usStatus == 0 ) {
			strcpy( buf, "OK" );
		} else {
			strcpy( buf, "No Heartbeat" );
			bServerError = TRUE;
		}

		
		lvi.iItem = 0;
		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

		// AuthServer연결
		itoa( pServerInfo->m_frontRefresh.siAuthConnect, buf, 10 );
		
		lvi.iItem = 1;
		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

		if( pServerInfo->m_frontRefresh.siAuthConnect == 0 ) bServerError = TRUE;


		// GameServer연결

		itoa( pServerInfo->m_frontRefresh.siGameServerConnect, buf, 10 );

		lvi.iItem = 2;
		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

		if( pServerInfo->m_frontRefresh.siGameServerConnect == 0 ) bServerError = TRUE;

		// Client연결 
		itoa( pServerInfo->m_frontRefresh.siUserNum, buf, 10 );

		lvi.iItem = 3;
		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

		//if( pServerInfo->m_frontRefresh.siUserNum == 0 ) bServerError = TRUE;

		// ProcessID
		itoa( pServerInfo->m_frontRefresh.dwProcessId, buf, 10 );

		lvi.iItem = 4;
		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

		// GameVersion
		itoa( pServerInfo->m_frontRefresh.siGameVersion, buf, 10 );

		lvi.iItem = 5;
		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_FRONTSERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

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
