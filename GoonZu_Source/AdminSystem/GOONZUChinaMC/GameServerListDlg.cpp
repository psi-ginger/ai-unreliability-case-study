// GameServerListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "GoonZuMCDlg.h"
#include "GameServerListDlg.h"
#include "../MasterServer/ServerInfo.h"
#include "MasterProcess.h"
#include ".\gameserverlistdlg.h"
#include "../MasterServer/MasterMsg.h"
#include "ITZSendBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameServerListDlg dialog


CGameServerListDlg::CGameServerListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameServerListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameServerListDlg)
		// NOTE: the ClassWizard will add member initialization here

		
	//}}AFX_DATA_INIT

	m_bInitView = FALSE;
	m_ServerViewIndex = 0;

	m_bMacroOn = FALSE;
	m_bOldFrame = 0;
}


void CGameServerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameServerListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameServerListDlg, CDialog)
	//{{AFX_MSG_MAP(CGameServerListDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ALARMON, OnButtonAlarmon)
	ON_BN_CLICKED(IDC_BUTTON_ALARMOFF, OnButtonAlarmoff)
	ON_BN_CLICKED(IDC_BUTTON_MACROON, OnButtonMacroon)
	ON_BN_CLICKED(IDC_BUTTON_MACROOFF, OnButtonMacrooff)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CMD, OnButtonCmd)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_ON, OnBnClickedButtonServerOn)
	ON_BN_CLICKED(IDC_BUTTON_GAMESERVER_OFF, OnBnClickedButtonGameserverOff)
	ON_BN_CLICKED(IDC_BUTTON_DBMANAGER_ON, OnBnClickedButtonDbmanagerOn)
	ON_BN_CLICKED(IDC_BUTTON_DBMANAGER_OFF, OnBnClickedButtonDbmanagerOff)
	ON_BN_CLICKED(IDC_BUTTON_ALL_SERVER_ON, OnBnClickedButtonAllServerOn)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DBMANAGER_OFF, OnBnClickedButtonAllDbmanagerOff)
	ON_BN_CLICKED(IDC_BUTTON_ALL_SERVER_OFF, OnBnClickedButtonAllServerOff)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DBMANAGER_ON, OnBnClickedButtonAllDbmanagerOn)
	ON_BN_CLICKED(IDC_SEND_DATA, OnBnClickedSendData)
	ON_BN_CLICKED(IDC_ALL_SEND_DATA, OnBnClickedAllSendData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameServerListDlg message handlers

BOOL CGameServerListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 리스트 컨트롤 초기화 
	SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT );

	int i;

	LVCOLUMN lvcol;
//	LVITEM lvi;

	int width[] = { 80, 60, 60, 70, 40, 40, 70, 40, 60, 50, 50, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Gamania", "NetmarbleJP", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	for( i = 0; i < 13; ++i ) {
		lvcol.cx = width[ i ];
		lvcol.pszText = text[ i ];
		lvcol.iSubItem = i;
		
		SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
	}

	SetTimer( 1, 1000, NULL );
	SetTimer( 10, 10000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameServerListDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default


	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	// TotalUser 
	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());


	ServerInfo *pServerInfo = pApp->m_pServerInfo;

	if( nIDEvent == 1 ) {


		SI32 i;
		char buffer[ 64 ];

		LVITEM lvi;
		lvi.mask = LVIF_TEXT;


		pServerInfo->Wait();

		if( m_bInitView == FALSE && pApp->m_pMasterProcess->m_bInitOK == TRUE ) {

			m_bInitView = TRUE;

			for( i = 0; i < pServerInfo->GetWorldCount(); ++i )
			{
				lvi.iItem = i;

				// 서버명
				lvi.iSubItem = 0;
				lvi.pszText = (char *)pServerInfo->m_gameWorldInfo[ i ].szWorldName;

				SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_INSERTITEM, 0, (LPARAM)&lvi );

				// 알람 

				lvi.iSubItem = 12;
				lvi.pszText = "ON";

				SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );
			}
		}

		//	SendDlgItemMessage( hDlg, IDC_LIST_RESIDENT, LVM_DELETEALLITEMS, 0, 0 );

		// Game Server List Update

		BOOL bServerError = FALSE;

		int totalUser = 0;
		int totalNewChar = 0;
		int totalDelChar = 0;
		//int totalPCRoomChar = 0;

		sprintf(pDlg->m_szErrorServer, "");

		for( i = 0; i < pServerInfo->GetWorldCount(); ++i )
		{

			lvi.iItem = i;

			// GameServer상태 
			pServerInfo->GetServerStatusString( pServerInfo->m_gameRefresh[ i ].usStatus, buffer );

			lvi.iSubItem = 1;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );


			// GameDBManager상태 
			pServerInfo->GetServerStatusString( pServerInfo->m_gameDBManagerRefresh[ i ].usStatus, buffer );

			lvi.iSubItem = 2;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );


			// 동접수
		//	if( pApp->m_bMaster ) {			
	
				sprintf( buffer, "%d/%d", 
					pServerInfo->m_gameRefresh[ i ].usCurrentUserNum,
					pServerInfo->m_gameRefresh[ i ].usMaxUserNum 
					);
		//	} else {
		//		strcpy( buffer, "****" );
		//	}

			lvi.iSubItem = 3;
			lvi.pszText = (char *)buffer;

			totalUser += pServerInfo->m_gameRefresh[ i ].usCurrentUserNum;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			// 감마니아
		//	if( pApp->m_bMaster ) {			

				sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentGamania	);
		//	} else {
		//		strcpy( buffer, "****" );
		//	}


			lvi.iSubItem = 4;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			// 넷마블
		//	if( pApp->m_bMaster ) {			

				sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentNetMarbleJP );
		//	} else {
		//		strcpy( buffer, "****" );
		//	}

			lvi.iSubItem = 5;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			// 프레임 
			itoa( pServerInfo->m_gameRefresh[ i ].uiFrame, buffer, 10 );

			lvi.iSubItem = 6;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			// FPS
			itoa( pServerInfo->m_gameRefresh[ i ].usFPS, buffer, 10 );

			lvi.iSubItem = 7;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			// DB Latency
			itoa( pServerInfo->m_gameRefresh[ i ].usDBLatency, buffer, 10 );

			lvi.iSubItem = 8;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			// 캐릭터 생성 수
		//	if( pApp->m_bMaster ) {			
				
				itoa( pServerInfo->m_gameRefresh[ i ].uiNewCharNum, buffer, 10 );
		//	} else {
		//		strcpy( buffer, "****" );
		//	}
			lvi.iSubItem = 9;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			totalNewChar += pServerInfo->m_gameRefresh[ i ].uiNewCharNum;
			
			// 캐릭터 삭제 수  
		//	if( pApp->m_bMaster ) {			

				itoa( pServerInfo->m_gameRefresh[ i ].uiDelCharNum, buffer, 10 );

		//	} else {
		//		strcpy( buffer, "****" );				
		//	}
			lvi.iSubItem = 10;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			totalDelChar += pServerInfo->m_gameRefresh[ i ].uiDelCharNum;

			// TradeServerSession
		//	if( pApp->m_bMaster ) {
				if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_ESTABLISHED )
					strcpy( buffer, "OK" );
				else if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_CONNECTED )
					strcpy( buffer, "Connecting" );
				else if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_DISCONNECTED )
					strcpy( buffer, "DisConnected" );
				else if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_NONE )
					strcpy( buffer, "None" );
		//	} else {
		//		strcpy( buffer, "****" );
		//	}

			lvi.iSubItem = 11;
			lvi.pszText = (char *)buffer;

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

//			totalPCRoomChar += pServerInfo->m_gameRefresh[ i ].uiPCRoomCharNum;

			if( CheckServer( i ) == FALSE ) {

				CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());

				LVITEM lvi;

				lvi.mask = LVIF_TEXT;
				lvi.iItem = i;
				lvi.iSubItem = 12;
				lvi.pszText = buffer;
				lvi.cchTextMax = 32;

				SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_GETITEM, 0, (LPARAM)&lvi );

				char buf[32];
				sprintf( buf, "[Game Server %d]", pServerInfo->m_gameWorldInfo[ i ].usWorldID);

				strcat( pDlg->m_szErrorServer, buf);
				strcat( pDlg->m_szErrorServer, " ");

				if( strcmp( buffer, "ON" ) == 0 ) bServerError = TRUE; 
			}
		}

		// GameServer Disp
		GameServerDisp();

		// DBManager Disp
		DBManagerDisp();
	
		if( pDlg->m_maxUser < totalUser ) pDlg->m_maxUser = totalUser;

	//	if( pApp->m_bMaster ) {
			sprintf( buffer, "%d/%d", totalUser, pDlg->m_maxUser );
	//	} else {
	//		strcpy( buffer, "*****" );
	//	}
		pDlg->SetDlgItemText( IDC_EDIT_TOTALUSER, buffer );


		// Total New/Del Char

	//	if( pApp->m_bMaster ) {
			sprintf( buffer, "%d/%d", totalNewChar, totalDelChar );
	//	} else {
	//		strcpy( buffer, "*****" );
	//	}
		pDlg->SetDlgItemText( IDC_EDIT_NEWCHAR, buffer );

		// Total PCRoom User 
		/*
		if( pApp->m_bMaster ) {
			if( totalUser == 0 ) {
				strcpy( buffer, "0" );
			} else {
				sprintf( buffer, "%d (%3.1f%%)", 
					totalPCRoomChar,
					(float)totalPCRoomChar / (float)totalUser * (float)100
					);
			}
		} else {
			strcpy( buffer, "*****" );
		}
		pDlg->SetDlgItemText( IDC_EDIT_PCUSER, buffer );
		*/


		pServerInfo->Release();

		if( bServerError ) {
			pDlg->m_bError = TRUE;
			pDlg->m_errorCode = pDlg->m_errorCode | 2;
		}
		else {
			if( !(pDlg->m_errorCode & (~2) ) ) {
				pDlg->m_bError = FALSE;
			}

			pDlg->m_errorCode = pDlg->m_errorCode & (~2);			
		}
	}

	//cyj 메크로 사용시
	//if (m_bMacroOn)
	//{
	//	if (pServerInfo)
	//	{
	//		int index = 0;

	//		for (int i = 0; i < 100; i++)
	//		{
	//			if (pServerInfo->m_gameRefresh[ i ].uiFrame > 0)
	//			{
	//				index = i;
	//				break;
	//			}
	//		}

	//		if (abs(pServerInfo->m_gameRefresh[ index ].uiFrame - m_bOldFrame) > 72000)
	//		{
	//			m_bOldFrame = pServerInfo->m_gameRefresh[ index ].uiFrame;

	//			char buffer[256] = "";
	//			sprintf(buffer, "%s", "/skoinobory");

	//			MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	//			sRequest_GMC_GMOrder	sendMsg( buffer );
	//			pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );
	//		}

	//	}
	//}


	if( nIDEvent == 10 ) {

	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CGameServerListDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	LPNMHDR hdr;
	LPNMLISTVIEW nlv;
//	LPNMITEMACTIVATE nia;
	hdr = (LPNMHDR)lParam;
	nlv = (LPNMLISTVIEW)lParam;

	HWND hwnd = ::GetDlgItem( m_hWnd, IDC_LIST_GAMESERVER );

	if( hdr->hwndFrom == hwnd ) {

		if( hdr->code == LVN_ITEMCHANGED ) {

			if( nlv->uChanged == LVIF_STATE && (nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED )) ) {

				m_ServerViewIndex = nlv->iItem;

				if( m_ServerViewIndex < 0 ) m_ServerViewIndex = 0;
/*
				char buf[ 32 ];

				LVITEM lvi;

				lvi.mask = LVIF_TEXT;
				lvi.iItem = nlv->iItem;
				lvi.iSubItem = 0;
				lvi.pszText = buf;
				lvi.cchTextMax = 32;

				SendDlgItemMessage( hDlg, IDC_LIST_GAMESERVER, LVM_GETITEM, 0, (LPARAM)&lvi );

				hwnd = GetDlgItem( hDlg, IDC_EDIT_NAME );
				SetWindowText( hwnd, buf );
*/
			}

		}

	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CGameServerListDlg::GameServerDisp()
{

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	ServerInfo *pServerInfo = pApp->m_pServerInfo;
	
	char szDisp[ 1024 ] = {'\0'};
	char buf[ 128 ] = {'\0'};

	// GameServer상태 
	//pServerInfo->GetServerStatusString( pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usStatus, buffer );

	sprintf( buf, "GameServer - [ %s ]", pServerInfo->m_gameWorldInfo[ m_ServerViewIndex ].szWorldName );
	SetDlgItemText( IDC_EDIT_GAMESERVERTITLE, buf );

	pServerInfo->GetServerStatusString( pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usStatus, buf );
	sprintf( szDisp, "State: [%s]\r\n", buf );

	if( pApp->m_bMaster ) {
		sprintf( buf, "User Count: [%d/%d %d %d]\r\n", 
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentUserNum,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usMaxUserNum ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentNetMarbleJP ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentGamania 
			);
		strcat( szDisp, buf );

		sprintf( buf, "NPC Count: [%d]\r\n", pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentNPCNum );
		strcat( szDisp, buf );
	}

	sprintf( buf, "Frame: [%d]\r\n", pServerInfo->m_gameRefresh[ m_ServerViewIndex ].uiFrame );
	strcat( szDisp, buf );

	sprintf( buf, "FPS: [%d]\r\n", pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usFPS );
	strcat( szDisp, buf );

	if( pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usFrontServerConnection == 1 )
		strcpy( buf, "FrontServer: [Established]\r\n" );
	else 
		strcpy( buf, "FrontServer: [Not Connect]\r\n" );

	strcat( szDisp, buf );

	if( pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usAuthServerConnection == 1 )
		strcpy( buf, "AuthServer: [Established]\r\n" );
	else 
		strcpy( buf, "AuthServer: [Not Connect]\r\n" );

	strcat( szDisp, buf );

	if( pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usDBManagerConnection == 1 )
		strcpy( buf, "DBManager: [Established]\r\n" );
	else 
		strcpy( buf, "DBManager: [Not Connect]\r\n" );

	strcat( szDisp, buf );

	SetDlgItemText( IDC_EDIT_GAMESERVERINFO, szDisp );


}

void CGameServerListDlg::DBManagerDisp()
{

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	ServerInfo *pServerInfo = pApp->m_pServerInfo;

	char szDisp[ 1024 ];
	char buf[ 128 ];

	szDisp[ 0 ] = NULL;

	sprintf( buf, "DBManager - [ %s ]", pServerInfo->m_gameWorldInfo[ m_ServerViewIndex ].szWorldName );
	SetDlgItemText( IDC_EDIT_DBMANAGERTITLE, buf );

}

BOOL CGameServerListDlg::CheckServer( int index )
{
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	ServerInfo *pServerInfo = pApp->m_pServerInfo;

	// 게임서버 상태 
	if( pServerInfo->m_gameRefresh[ index ].usStatus != 0 ) return FALSE;


	// DBManager상태 
	if( pServerInfo->m_gameDBManagerRefresh[ index ].usStatus != 0 ) return FALSE;

	// FPS
	if( pServerInfo->m_gameRefresh[ index ].usFPS < 35 ) {
		if( check[ index ].last_tick_fps == 0 ) 
		{
			check[ index ].last_tick_fps = GetTickCount();

		} else if( GetTickCount() - check[ index ].last_tick_fps > 10000 ) {
			return FALSE;
		}
	} else {
		check[ index ].last_tick_fps = 0;
	}

	// DBLatency
	if( pServerInfo->m_gameRefresh[ index ].usDBLatency > 10 ) return FALSE;

	return TRUE;

}

void CGameServerListDlg::OnButtonAlarmon() 
{
	// TODO: Add your control notification handler code here

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	HWND hwnd = ::GetDlgItem( m_hWnd, IDC_LIST_GAMESERVER );

	int index = ListView_GetNextItem( hwnd, -1, LVNI_ALL | LVNI_SELECTED );

	if( index != -1 ) {

		do {
				lvi.iItem = index;
				lvi.iSubItem = 12;
				lvi.pszText = "ON";

				SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

				index = ListView_GetNextItem( hwnd, index, LVNI_ALL | LVNI_SELECTED );

		} while ( index != -1 );

	}
	
}

void CGameServerListDlg::OnButtonAlarmoff() 
{
	// TODO: Add your control notification handler code here

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	HWND hwnd = ::GetDlgItem( m_hWnd, IDC_LIST_GAMESERVER );

	int index = ListView_GetNextItem( hwnd, -1, LVNI_ALL | LVNI_SELECTED );

	if( index != -1 ) {

		do {
				lvi.iItem = index;
				lvi.iSubItem = 12;
				lvi.pszText = "off";

				SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

				index = ListView_GetNextItem( hwnd, index, LVNI_ALL | LVNI_SELECTED );

		} while ( index != -1 );

	}	

}

void CGameServerListDlg::OnButtonMacroon()
{
	//cyj 메크로 사용시
	//m_bMacroOn = TRUE;

	//char buffer[256] = "";
	//sprintf(buffer, "%s", "/skoinobory");

	//CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	//MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	//sRequest_GMC_GMOrder	sendMsg( buffer );
	//pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

}

void CGameServerListDlg::OnButtonMacrooff() 
{
	//cyj 메크로 사용시
	//m_bMacroOn = FALSE;
}

void CGameServerListDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer( 1 );
	KillTimer( 10 );
	
}

void CGameServerListDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());

	CDialog::OnLButtonDown(nFlags, point);
}

void CGameServerListDlg::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGameServerListDlg::OnButtonCmd() 
{
	// TODO: Add your control notification handler code here
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	char buffer[ 1024 ];

	GetDlgItemText( IDC_EDIT_CMD, buffer, 256 );

	// 모니터링 요원은 블럭, 언블럭만 할 수 있다
	if( strcmp( buffer, "/unblock") == 0 || strcmp( buffer, "/block") == 0 )
	{  
	}
	else
	{
		if( TRUE == pApp->m_bMoniterMan )
			return;
	}

	sRequest_GMC_GMOrder	sendMsg( buffer );
	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "Send AllServer Order [%s]\n", buffer );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );

	SetDlgItemText( IDC_EDIT_CMD, "" );


}

BOOL CGameServerListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {


		CWnd *pWnd = GetDlgItem( IDC_EDIT_CMD );

		if( pMsg->hwnd == pWnd->m_hWnd ) 
		{
			OnButtonCmd();
		}
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGameServerListDlg::FindFileRecursive( TCHAR* pPath, HANDLE sendFileHandle, HANDLE sendDirectoryHandle  )
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL result = TRUE;
	TCHAR findName[ MAX_PATH ] = {'\0'};
	TCHAR newDirectory[ MAX_PATH ] = {'\0'};
	TCHAR drive[ MAX_PATH ] = {'\0'};
	TCHAR dir[ MAX_PATH ] = {'\0'};
	TCHAR newPath[ MAX_PATH ] = {'\0'};

	hSrch = FindFirstFile( pPath, &wfd );

	DWORD write = 0;
	while( result )
	{
		_splitpath( pPath, drive, dir, NULL, NULL );

		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( wfd.cFileName[0] != '.' )
			{
				wsprintf( newPath, "%s%s%s\\*.*", drive, dir, wfd.cFileName );
				wsprintf( newDirectory, "%s%s%s\r\n", drive, dir, wfd.cFileName );
				WriteFile( sendDirectoryHandle, newDirectory, strlen( newDirectory ), &write, NULL );
				FindFileRecursive( newPath, sendFileHandle, sendDirectoryHandle );
			}
		}
		else
		{
			wsprintf( findName, "%s%s%s\r\n", drive, dir, wfd.cFileName );
			printf( "%s", findName );
			WriteFile( sendFileHandle, findName, strlen( findName ), &write, NULL );
		}

		result = FindNextFile( hSrch, &wfd );
	}

	FindClose( hSrch );
}

void CGameServerListDlg::OnBnClickedButtonServerOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	if( 0 > m_ServerViewIndex )
		return;

	sRequest_GMCServerOn sendMsg( m_ServerViewIndex );

	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "[%d]Server On\n", m_ServerViewIndex );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedButtonGameserverOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	if( 0 > m_ServerViewIndex )
		return;

	sRequest_GMCServerOff sendMsg( m_ServerViewIndex );

	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "[%d]Server Off\n", m_ServerViewIndex );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedButtonDbmanagerOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	if( 0 > m_ServerViewIndex )
		return;

	sRequest_GMCGameDBManagerOn sendMsg( m_ServerViewIndex );

	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "[%d]DBManager On\n", m_ServerViewIndex );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedButtonDbmanagerOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	if( 0 > m_ServerViewIndex )
		return;

	sRequest_GMCGameDBManagerOff sendMsg( m_ServerViewIndex );

	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "[%d]DBManager Off\n", m_ServerViewIndex );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedButtonAllServerOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	sRequest_GMC_AllServerOn sendMsg;
	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "All Server On\n" );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedButtonAllServerOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	sRequest_GMC_AllServerOff sendMsg;
	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "All Server Off\n" );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedButtonAllDbmanagerOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	sRequest_GMC_AllGameDBManagerOn sendMsg;
	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "All GameDBManager On\n" );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}


void CGameServerListDlg::OnBnClickedButtonAllDbmanagerOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	sRequest_GMC_AllGameDBManagerOff sendMsg;
	pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());
	TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
	StringCchPrintf( tempBuffer, MAX_PATH, "All GameDBManager Off\n" );
	pDlg->m_richMsg.AddText( tempBuffer, RGB( 0, 0, 0 ) );
}

void CGameServerListDlg::OnBnClickedSendData()
{
	return;
}

void CGameServerListDlg::OnBnClickedAllSendData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	if( !pApp->m_bMaster ) 
		return;

	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	HANDLE hFile = CreateFile( PATCH_INFOFILE, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( INVALID_HANDLE_VALUE ==  hFile )
		return;

	HANDLE hFile2 = CreateFile( PATCH_INFODIR, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( INVALID_HANDLE_VALUE ==  hFile2 )
		return;

	FindFileRecursive( PATCH_ROOTDIRECTORY, hFile, hFile2 );

	DWORD write = 0;
	WriteFile( hFile, "<end>", strlen( "<end>" ), &write, NULL );
	WriteFile( hFile2, "<end>", strlen( "<end>" ), &write, NULL );

	CloseHandle( hFile );
	CloseHandle( hFile2 );

	FILE* fp2 = _tfopen( PATCH_INFODIR, "rb" );

	if( fp2 == NULL )
		return;

	TCHAR sendDirectoryName[100][ MAX_PATH ] = {'\0'};
	TCHAR buffer[ MAX_PATH ];
	TCHAR changeBuffer[ MAX_PATH ];

	BOOL loop = TRUE;
	SI32 count = 0;
	while( loop )
	{
		_fgetts( buffer, MAX_PATH, fp2 );

		if( _tcscmp( "<end>", buffer ) == 0 )
		{
			loop = FALSE;
			break;
		}

		_stscanf( buffer, "%s", changeBuffer );

		_tcsncpy( sendDirectoryName[ count ], changeBuffer, MAX_PATH );

		sRequest_GMC_MakeDirectory clMakeDirectory( sendDirectoryName[count] );
		pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&clMakeDirectory );
		count++;
	}

	fclose( fp2 );

	FILE* fp = _tfopen( PATCH_INFOFILE, "rb" );

	if( fp == NULL )
		return;

	TCHAR sendFilename[100][ MAX_PATH ] = {'\0'};

	loop = TRUE;
	count = 0;
	while( loop )
	{
		_fgetts( buffer, MAX_PATH, fp );

		if( _tcscmp( "<end>", buffer ) == 0 )
		{
			loop = FALSE;
			break;
		}

		_stscanf( buffer, "%s", changeBuffer );

		_tcsncpy( sendFilename[ count++ ], changeBuffer, MAX_PATH );
	}

	fclose( fp );

	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());

	static DWORD sendClock = 0;
	for( SI32 i = 0; i < 100; ++i )
	{
		if( _tcscmp( sendFilename[i], "\0" ) != 0 )
		{
			HANDLE hFile = CreateFile( sendFilename[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

			if( INVALID_HANDLE_VALUE ==  hFile )
				return;

			sRequest_GMC_MakeFile clMakeFile( sendFilename[i] );
			pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&clMakeFile );

			TCHAR tempBuffer[ MAX_PATH ] = {"\0"};
			_tcsncpy( tempBuffer, sendFilename[i], MAX_PATH );
			_tcscat( tempBuffer, " Sending...\n" );
			pDlg->m_richMsg.AddText( tempBuffer, RGB( 255, 0, 0 ) );

			BOOL loop = TRUE;
			BOOL result = FALSE;
			DWORD read = 0;
			SI32 count = 0;
			while( loop )
			{
				if( GetTickCount() - sendClock > 100 )
				{
					TCHAR buf[MAX_SEND_BYPE] = {'\0'};
					result = ReadFile( hFile, buf, MAX_SEND_BYPE, &read, NULL );
					if( ( result == TRUE ) && ( 0 == read ) )
					{
						tempBuffer[0] = '\0';
						_tcsncpy( tempBuffer, sendFilename[i], MAX_PATH );
						_tcscat( tempBuffer, "Send End\n" );
						pDlg->m_richMsg.AddText( tempBuffer, RGB( 255, 0, 0 ) );
						loop = false;
					}
					else
					{
						sRequest_GMC_SendFileData clSendFileData( count++, read, sendFilename[i], buf );
						pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&clSendFileData );
						sendClock = GetTickCount();
					}
				}
			}

			CloseHandle( hFile );
		}
	}
}
