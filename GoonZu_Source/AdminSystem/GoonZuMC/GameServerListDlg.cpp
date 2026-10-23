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

#include "StaticBuffer.h"
#include "GMOrderLogManager.h"

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
	ON_BN_CLICKED(IDC_BUTTON_MACROON, OnButtonMacroon)
	ON_BN_CLICKED(IDC_BUTTON_MACROOFF, OnButtonMacrooff)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CMD, OnButtonCmd)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON, OnBnClickedButton)
	ON_BN_CLICKED(IDC_BTNALARMON, OnBnClickedBtnalarmon)
	ON_BN_CLICKED(IDC_BTNALARMOFF, OnBnClickedBtnalarmoff)
	ON_BN_CLICKED(IDC_BTNSELECTALL, OnBnClickedBtnselectall)
	ON_BN_CLICKED(IDC_BTNNOTSELECTALL, OnBnClickedBtnnotselectall)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameServerListDlg message handlers

#if defined(_JAPAN)
	int g_nAlarmSubItem = 14;
#elif defined(_KOREA)
	int g_nAlarmSubItem = 10;
#elif defined(_USA)
	int g_nAlarmSubItem = 10;
#elif defined(_ENGLISH)
	int g_nAlarmSubItem = 12;
#elif defined(_TAIWAN)
	int g_nAlarmSubItem = 10;
#elif defined(_NHNChina)
	int g_nAlarmSubItem = 10;
#elif defined(_EUROPE)
	int g_nAlarmSubItem = 10;
#endif


BOOL CGameServerListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	////Gameserver DataList
	//	리스트뷰 생성
	m_ListViewGameList.Create(0,::GetDlgItem(m_hWnd,IDC_LIST_GAMESERVER),NULL,0,0,200,200,TRUE);
	//	리스트 뷰 칼럼 만들기
#if defined(_JAPAN)
	int width[] = { 110, 80, 80, 80, 80, 80, 80, 80, 80, 70, 40, 70, 50, 50, 80, 50 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "HanGameJP" , "GamaniaJP", "GamePotJP","NDEJP","NEXONJP", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#elif defined(_KOREA)
	int width[] = { 110, 80, 80, 90, 70, 40, 70, 60, 60, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#elif defined(_USA)
	int width[] = { 110, 80, 80, 90, 70, 40, 70, 60, 60, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#elif defined(_ENGLISH)
	int width[] = { 110, 80, 80, 90, 80, 80, 70, 40, 70, 60, 60, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Global", "NHN", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#elif defined(_TAIWAN)
	int width[] = { 110, 80, 80, 90, 70, 40, 70, 60, 60, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#elif defined(_NHNChina)
	int width[] = { 110, 80, 80, 90, 70, 40, 70, 60, 60, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#elif defined(_EUROPE)
	int width[] = { 110, 80, 80, 90, 70, 40, 70, 60, 60, 80, 60 };
	char *text[] = { "ServerName", "GameServer", "DBManager", "UserCount", "Frame", "FPS", "DBLatency", "Create", "Del", "TradeServer", "Alarm" };
#endif

	int i = 0;
#if defined(_JAPAN)
	for( i = 0; i < 15; ++i ) {
#elif defined(_KOREA)
	for( i = 0; i < 11; ++i ) {
#elif defined(_USA)
	for( i = 0; i < 11; ++i ) {
#elif defined(_ENGLISH)
	for( i = 0; i < 13; ++i ) {
#elif defined(_TAIWAN)
	for( i = 0; i < 11; ++i ) {
#elif defined(_NHNChina)
	for( i = 0; i < 11; ++i ) {
#elif defined(_EUROPE)
	for( i = 0; i < 11; ++i ) {
#endif
		m_ListViewGameList.Set_Col(text[i],width[i]);
	}

	SetTimer( 1, 1000, NULL );
	SetTimer( 10, 10000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameServerListDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	const SI32	ConstTmpBufferSize = 64;

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	// TotalUser 
	CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());

	ServerInfo *pServerInfo = pApp->m_pServerInfo;

	if( nIDEvent == 1 ) {
		SI32 i = 0;
		char buffer[ ConstTmpBufferSize ];

		pServerInfo->Wait();

		if( m_bInitView == FALSE && pApp->m_pMasterProcess->m_bInitOK == TRUE ) {

			m_bInitView = TRUE;

			for( i = 0; i < pServerInfo->GetWorldCount(); ++i )
			{
				//lvi.iItem = i;

				// 서버명
				TCHAR szTmp[ConstTmpBufferSize] = {0,};
				_stprintf(szTmp, TEXT("[%d]%s"), pServerInfo->m_gameWorldInfo[ i ].usWorldID, pServerInfo->m_gameWorldInfo[ i ].szWorldName );
				m_ListViewGameList.Set_Data(i,0,szTmp);
				// 알람 
				m_ListViewGameList.Set_Data(i,g_nAlarmSubItem,TEXT("ON"));
			}
		}

		//	SendDlgItemMessage( hDlg, IDC_LIST_RESIDENT, LVM_DELETEALLITEMS, 0, 0 );

		// Game Server List Update

		BOOL bServerError = FALSE;

		int totalUser = 0;
		int totalNewChar = 0;
		int totalDelChar = 0;
		//int totalPCRoomChar = 0;

		pDlg->m_strErrorServer = "";
		//sprintf(pDlg->m_szErrorServer, "");

		for( i = 0; i < pServerInfo->GetWorldCount(); ++i )
		{
			int		nSubItem = 1;

			// GameServer상태 
			pServerInfo->GetServerStatusString( pServerInfo->m_gameRefresh[ i ].usStatus, buffer );
			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// GameDBManager상태 
			pServerInfo->GetServerStatusString( pServerInfo->m_gameDBManagerRefresh[ i ].usStatus, buffer );
			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// 동접수
			if( pApp->m_bMaster ) {			
	
				sprintf( buffer, "%d/%d", 
					pServerInfo->m_gameRefresh[ i ].usCurrentUserNum,
					pServerInfo->m_gameRefresh[ i ].usMaxUserNum 
					);
			} else {
				strcpy( buffer, "****" );
			}
			totalUser += pServerInfo->m_gameRefresh[ i ].usCurrentUserNum;
			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;
#ifdef _ENGLISH
			if( pApp->m_bMaster ) 
			{
				// 글로벌
				if( pApp->m_bMaster ) 
				{			
					sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentGLOBAL );
				} 
				else 
				{
					strcpy( buffer, "****" );
				}

				m_ListViewGameList.Set_Data(i,nSubItem,buffer);
				nSubItem++;
				// NHN
				if( pApp->m_bMaster ) 
				{			
					sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentNHN);
				} 
				else 
				{
					strcpy( buffer, "****" );
				}

				m_ListViewGameList.Set_Data(i,nSubItem,buffer);
				nSubItem++;
			}
#endif
#if defined(_JAPAN)
			//KHY - 0708 - 일본 모니터링 클라이언트 수정.
			// 한게임
			if( pApp->m_bMaster ) {			

				sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentHanGameJP );
			} else {
				strcpy( buffer, "****" );
			}

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// 감마니아
			if( pApp->m_bMaster ) {			

			sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentGamaniaJP);
			} else {
			strcpy( buffer, "****" );
			}


			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// 게임팟
			if( pApp->m_bMaster ) {			

				sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentGamePotJP);
			} else {
				strcpy( buffer, "****" );
			}

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// NDE
			if( pApp->m_bMaster ) {			

				sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentNDEJP);
			} else {
				strcpy( buffer, "****" );
			}

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// NEXON
			if( pApp->m_bMaster ) {			

				sprintf( buffer, "%d", pServerInfo->m_gameRefresh[ i ].usCurrentNEXONJP);
			} else {
				strcpy( buffer, "****" );
			}

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

#endif

			// 프레임 
			itoa( pServerInfo->m_gameRefresh[ i ].uiFrame, buffer, 10 );

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// FPS
			itoa( pServerInfo->m_gameRefresh[ i ].usFPS, buffer, 10 );

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// DB Latency
			itoa( pServerInfo->m_gameRefresh[ i ].usDBLatency, buffer, 10 );

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			// 캐릭터 생성 수
			if( pApp->m_bMaster ) {			
				
				itoa( pServerInfo->m_gameRefresh[ i ].uiNewCharNum, buffer, 10 );
			} else {
				strcpy( buffer, "****" );
			}
			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			totalNewChar += pServerInfo->m_gameRefresh[ i ].uiNewCharNum;
			
			// 캐릭터 삭제 수  
			if( pApp->m_bMaster ) {			

				itoa( pServerInfo->m_gameRefresh[ i ].uiDelCharNum, buffer, 10 );

			} else {
				strcpy( buffer, "****" );				
			}
			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

			totalDelChar += pServerInfo->m_gameRefresh[ i ].uiDelCharNum;

			// TradeServerSession
			if( pApp->m_bMaster ) {
				if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_ESTABLISHED )
					strcpy( buffer, "OK" );
				else if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_CONNECTED )
					strcpy( buffer, "Connecting" );
				else if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_DISCONNECTED )
					strcpy( buffer, "DisConnected" );
				else if ( pServerInfo->m_gameRefresh[ i ].uiTradeServerSession == SESSION_STATE_NONE )
					strcpy( buffer, "None" );
			} else {
				strcpy( buffer, "****" );
			}

			m_ListViewGameList.Set_Data(i,nSubItem,buffer);
			nSubItem++;

//			totalPCRoomChar += pServerInfo->m_gameRefresh[ i ].uiPCRoomCharNum;

			int nServerErr;
			//if( CheckServer( i ) == FALSE ) {
			if( ( nServerErr = CheckServer( i ) ) != RTN_SERVERERROR_OK ) {

				CGoonZuMCDlg *pDlg = (CGoonZuMCDlg *)(GetParent()->GetParent());

				m_ListViewGameList.Get_Data(buffer,ConstTmpBufferSize,i,nSubItem);
				nSubItem++;

				char buf[ConstTmpBufferSize];
				switch(nServerErr)
				{
				case RTN_SERVERERROR_GAMESERVER:
					{
						sprintf( buf, "[Game Server %d]", pServerInfo->m_gameWorldInfo[ i ].usWorldID);
					}
					break;
				case RTN_SERVERERROR_DBMANAGER:
					{
						sprintf( buf, "[Game DBManager %d]", pServerInfo->m_gameWorldInfo[ i ].usWorldID);
					}
					break;
				case RTN_SERVERERROR_DBLATENCY:
					{
						sprintf( buf, "[Game DBManager Latancy %d]", pServerInfo->m_gameWorldInfo[ i ].usWorldID);
					}
					break;
				case RTN_SERVERERROR_GAMESERVER_FPS:				
					{
						sprintf( buf, "[Game Server FPS %d]", pServerInfo->m_gameWorldInfo[ i ].usWorldID);
					}
					break;
				}
				//sprintf( buf, "[Game Server %d]", pServerInfo->m_gameWorldInfo[ i ].usWorldID);

				pDlg->m_strErrorServer += buf;
				//strcat( pDlg->m_szErrorServer, buf);
				pDlg->m_strErrorServer += " ";
				//strcat( pDlg->m_szErrorServer, " ");

				if( strcmp( buffer, "ON" ) == 0 ) bServerError = TRUE; 
			}
		}

		// GameServer Disp
		GameServerDisp();

		// DBManager Disp
		DBManagerDisp();
	
		// GMorder Disp
		GMOrderDisp();

		if( pDlg->m_maxUser < totalUser ) pDlg->m_maxUser = totalUser;

		if( pApp->m_bMaster ) {
			sprintf( buffer, "%d/%d", totalUser, pDlg->m_maxUser );
		} else {
			strcpy( buffer, "*****" );
		}
		pDlg->SetDlgItemText( IDC_EDIT_TOTALUSER, buffer );


		// Total New/Del Char

		if( pApp->m_bMaster ) {
			sprintf( buffer, "%d/%d", totalNewChar, totalDelChar );
		} else {
			strcpy( buffer, "*****" );
		}
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
		sprintf( buf, "User Count: [%d/%d %d %d %d]\r\n", 
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentUserNum,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usMaxUserNum ,
			//pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentNetMarbleJP ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentHanGameJP ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentGamaniaJP ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentGamePotJP ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentNDEJP ,
			pServerInfo->m_gameRefresh[ m_ServerViewIndex ].usCurrentNEXONJP 
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

int CGameServerListDlg::CheckServer( int index )
{
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	ServerInfo *pServerInfo = pApp->m_pServerInfo;

	// 게임서버 상태 
	//if( pServerInfo->m_gameRefresh[ index ].usStatus != 0 ) return FALSE;
	if( pServerInfo->m_gameRefresh[ index ].usStatus != 0 ) return RTN_SERVERERROR_GAMESERVER;


	// DBManager상태 
	//if( pServerInfo->m_gameDBManagerRefresh[ index ].usStatus != 0 ) return FALSE;
	if( pServerInfo->m_gameDBManagerRefresh[ index ].usStatus != 0 ) return RTN_SERVERERROR_DBMANAGER;

	// FPS
	if( pServerInfo->m_gameRefresh[ index ].usFPS < 35 ) {
		if( check[ index ].last_tick_fps == 0 ) 
		{
			check[ index ].last_tick_fps = GetTickCount();

		} else if( GetTickCount() - check[ index ].last_tick_fps > 10000 ) {
			//return FALSE;
			return RTN_SERVERERROR_GAMESERVER_FPS;
		}
	} else {
		check[ index ].last_tick_fps = 0;
	}

	// DBLatency
	//if( pServerInfo->m_gameRefresh[ index ].usDBLatency > 10 ) return FALSE;
	if( pServerInfo->m_gameRefresh[ index ].usDBLatency > 10 ) return RTN_SERVERERROR_DBLATENCY;

	//return TRUE;
	return RTN_SERVERERROR_OK;

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
	
	

}

BOOL CGameServerListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {

		if( pMsg->hwnd == ::GetDlgItem( m_hWnd, IDC_EDIT_CMD ) ) {

			pMsg->hwnd = m_hWnd;
			pMsg->message = WM_COMMAND;
			pMsg->wParam = MAKEWPARAM( IDC_BUTTON, BN_CLICKED );
			pMsg->lParam = 0;

		} else if( pMsg->hwnd != ::GetDlgItem( m_hWnd, IDC_BUTTON ) ) {
			pMsg->message = 0;
		}

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

void CGameServerListDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGameServerListDlg::OnBnClickedButton()
{
	// TODO: Add your control notification handler code here

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;

	if( !pApp->m_bMaster ) 
		return;

	// 마스터 권한(102)가 없으면 실행 불가
	if( pApp->m_nMasterLevel < 102 ) 
	{
		AfxMessageBox("To execute this command, you need that masterlevel is 102", MB_OK);
		return;
	}

	//#ifdef _DEBUG
	//	HANDLE hFile = CreateFile( "c:\\Patch\\PatchInfo.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	//
	//	if( INVALID_HANDLE_VALUE ==  hFile )
	//		return;
	//
	//	HANDLE hFile2 = CreateFile( "c:\\Patch\\PatchDir.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	//
	//	if( INVALID_HANDLE_VALUE ==  hFile2 )
	//		return;
	//
	//	FindFileRecursive( "c:\\Patch", hFile, hFile2 );
	//
	//	DWORD write = 0;
	//	WriteFile( hFile, "<end>", strlen( "<end>" ), &write, NULL );
	//	WriteFile( hFile2, "<end>", strlen( "<end>" ), &write, NULL );
	//
	//	CloseHandle( hFile );
	//	CloseHandle( hFile2 );
	//
	//	FILE* fp2 = _tfopen( "c:\\Patch\\PatchDir.txt", "rb" );
	//
	//	if( fp2 == NULL )
	//		return;
	//
	//	TCHAR sendDirectoryName[100][ MAX_PATH ] = {'\0'};
	//	TCHAR buffer[ MAX_PATH ];
	//	TCHAR changeBuffer[ MAX_PATH ];
	//
	//	BOOL loop = TRUE;
	//	SI32 count = 0;
	//	while( loop )
	//	{
	//		_fgetts( buffer, MAX_PATH, fp2 );
	//
	//		if( _tcscmp( "<end>", buffer ) == 0 )
	//		{
	//			loop = FALSE;
	//			break;
	//		}
	//
	//		_stscanf( buffer, "%s", changeBuffer );
	//
	//		_tcsncpy( sendDirectoryName[ count ], changeBuffer, MAX_PATH );
	//
	//		sRequest_GMC_MakeDirectory clMakeDirectory( sendDirectoryName[count] );
	//		pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&clMakeDirectory );
	//		count++;
	//	}
	//
	//	fclose( fp2 );
	//
	///*	FILE* fp = _tfopen( "c:\\Patch\\PatchInfo.txt", "rb" );
	//
	//	if( fp == NULL )
	//		return;
	//
	//	TCHAR sendFilename[100][ MAX_PATH ] = {'\0'};
	//
	//	loop = TRUE;
	//	count = 0;
	//	while( loop )
	//	{
	//		_fgetts( buffer, MAX_PATH, fp );
	//
	//		if( _tcscmp( "<end>", buffer ) == 0 )
	//		{
	//			loop = FALSE;
	//			break;
	//		}
	//
	//		_stscanf( buffer, "%s", changeBuffer );
	//
	//		_tcsncpy( sendFilename[ count++ ], changeBuffer, MAX_PATH );
	//	}
	//
	//	fclose( fp );
	//
	//	for( SI32 i = 0; i < 100; ++i )
	//	{
	//		if( _tcscmp( sendFilename[i], "\0" ) != 0 )
	//		{
	//			HANDLE hFile = CreateFile( sendFilename[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	//
	//			if( INVALID_HANDLE_VALUE ==  hFile )
	//				return;
	//
	//			sRequest_GMC_MakeFile clMakeFile( sendFilename[i] );
	//			pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&clMakeFile );
	//
	//			BOOL loop = TRUE;
	//			BOOL result = FALSE;
	//			DWORD read = 0;
	//			SI32 count = 0;
	//			while( loop )
	//			{
	//				TCHAR buf[1000] = {'\0'};
	//				result = ReadFile( hFile, buf, 1000, &read, NULL );
	//				if( ( result == TRUE ) && ( 0 == read ) )
	//				{
	//					loop = false;
	//				}
	//				else
	//				{
	//					sRequest_GMC_SendFileData clSendFileData( count++, read, sendFilename[i], buf );
	//					pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&clSendFileData );
	//					//	first = FALSE;
	//				}
	//			}
	//
	//			CloseHandle( hFile );
	//		}
	//	}*/
	//#else

	//-----------------------------------------------------------------------------------------------
	// 선택한 서버로만 메시지 보내기
	//-----------------------------------------------------------------------------------------------

	const SI32	ConstTmpBufferSize = 1024;

	// 서버 정보
	ServerInfo *pServerInfo = pApp->m_pServerInfo;
	if(pServerInfo == NULL)
		return;

	// 선택 서버 인덱스(배열인덱스)
	if(m_ServerViewIndex < 0)
		return;

	// 전송 커맨드
	TCHAR szBuffer[ ConstTmpBufferSize ];
	GetDlgItemText( IDC_EDIT_CMD, szBuffer, ConstTmpBufferSize );
	SetDlgItemText( IDC_EDIT_CMD, "" );
	// 서버 한개라도 체크되었는지
	BOOL bChecked = FALSE;

	// 전송 실패 텍스트
	TCHAR	szFailText[ConstTmpBufferSize] = {0,};

	// 명령 문자열이 작성되었는지 확인
	if(!_tcscmp(szBuffer,TEXT("")))
	{
		StringCchCopy(szFailText,ConstTmpBufferSize,"You must write command text.");
		::SendMessage(::GetDlgItem(m_hWnd, IDC_LISTGMCMDLOG), LB_INSERTSTRING,0,(LPARAM)szFailText);
		return;
	}

	// 체크박스로 선택된 서버들에게 전송
	for(SI32 i = 0; i < pServerInfo->GetWorldCount(); ++i )
	{
		if(m_ListViewGameList.IsCheckedRecord(i))
		{
			bChecked = TRUE;

			// 게임 서버 정보
			sGameServerInfo* pGameServerInfo = (sGameServerInfo*)&pServerInfo->m_gameWorldInfo[ i ].gameServerInfo;
			if(pGameServerInfo == NULL)
				continue;

			// 접속중인 상태가 아닌 경우
			if(pServerInfo->m_gameRefresh[i].usStatus == SERVERSTATUS_NOHEARTBEAT)
			{
				StringCchPrintf(szFailText,ConstTmpBufferSize,"[%s] is not connected", pServerInfo->m_gameWorldInfo[ i ].szWorldName);

				::SendMessage(::GetDlgItem(m_hWnd, IDC_LISTGMCMDLOG), LB_INSERTSTRING,0,(LPARAM)szFailText);
			}
			else if(pServerInfo->m_gameRefresh[i].usStatus == SERVERSTATUS_OK)
			{
				sRequest_GMC_GMOrder	sendMsg( szBuffer, pGameServerInfo->usWorldID );
				pMasterProcess->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );
			}
		}
	}

	// 한개의 서버도 선택되지 않고 명령어를 실행 했을 경우
	if(bChecked == FALSE)
	{
		StringCchCopy(szFailText,ConstTmpBufferSize,"You must select at least one GameServer.");
		::SendMessage(::GetDlgItem(m_hWnd, IDC_LISTGMCMDLOG), LB_INSERTSTRING,0,(LPARAM)szFailText);
		return;
	}
	//#endif
}
void CGameServerListDlg::GMOrderDisp()
{
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	CGMOrderLogManager* pGMOrderLogManager = pApp->m_pGMOrderLogManager;
	if(pGMOrderLogManager==NULL)
		return;
	StaticBuffer*		pBuffer = pGMOrderLogManager->m_pGMOrderLogBuffer;
	if(pBuffer == NULL)
		return;

	SI32	siSize = sizeof(CGMOrderLog);
	CGMOrderLog	gMOrderLog;
	ZeroMemory(&gMOrderLog, siSize);
	
	const SI32	ConstLogTextSize = 256;
	const SI32	ConstTmpBufSize = 32;

	TCHAR	szIsSuccess[ConstTmpBufSize] = {0,};
	TCHAR	szLog[ConstLogTextSize] = {0,};
	// gm명령어 로그를 하나씩 쓴다.
	while(pBuffer->Out_Data_Buffer((BYTE*)&gMOrderLog,siSize))
	{
		if(gMOrderLog.GetResult() == GMORDER_RESULT_SUCCESS)
		{
			StringCchCopy(szIsSuccess,ConstTmpBufSize,TEXT("SUCCESS"));
		}
		else if(gMOrderLog.GetResult() == GMORDER_RESULT_DENY)
		{
			StringCchCopy(szIsSuccess,ConstTmpBufSize,TEXT("NOTACCESS"));
		}
		else
		{
			StringCchCopy(szIsSuccess,ConstTmpBufSize,TEXT("ERROR"));
		}

		StringCchPrintf(szLog,ConstLogTextSize,TEXT("[%s] [%s] [%s] [%s] [%s] [%s]"),
												szIsSuccess,
												gMOrderLog.GetOrderTime(),
												gMOrderLog.GetGameServerName(),
												gMOrderLog.GetOrderPersonIP(),
												gMOrderLog.GetOrderPersonID(),
												gMOrderLog.GetOrder());
		
		::SendMessage(::GetDlgItem(m_hWnd, IDC_LISTGMCMDLOG), LB_INSERTSTRING,0,(LPARAM)szLog);

		ZeroMemory(&gMOrderLog, siSize);
	}
	return;
}

void CGameServerListDlg::OnBnClickedBtnalarmon()
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	HWND hwnd = ::GetDlgItem( m_hWnd, IDC_LIST_GAMESERVER );

	int index = ListView_GetNextItem( hwnd, -1, LVNI_ALL | LVNI_SELECTED );

	if( index != -1 ) {

		do {
			lvi.iItem = index;
			lvi.iSubItem = g_nAlarmSubItem;
			lvi.pszText = "ON";

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			index = ListView_GetNextItem( hwnd, index, LVNI_ALL | LVNI_SELECTED );

		} while ( index != -1 );

	}
}

void CGameServerListDlg::OnBnClickedBtnalarmoff()
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	HWND hwnd = ::GetDlgItem( m_hWnd, IDC_LIST_GAMESERVER );

	int index = ListView_GetNextItem( hwnd, -1, LVNI_ALL | LVNI_SELECTED );

	if( index != -1 ) {

		do {
			lvi.iItem = index;
			lvi.iSubItem = g_nAlarmSubItem;
			lvi.pszText = "OFF";

			SendDlgItemMessage( IDC_LIST_GAMESERVER, LVM_SETITEM, 0, (LPARAM)&lvi );

			index = ListView_GetNextItem( hwnd, index, LVNI_ALL | LVNI_SELECTED );

		} while ( index != -1 );

	}	
}

void CGameServerListDlg::OnBnClickedBtnselectall()
{
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	if(pApp  == NULL)
		return;
	ServerInfo *pServerInfo = pApp->m_pServerInfo;
	if(pServerInfo == NULL)
		return;

	for(SI32 i = 0; i < pServerInfo->GetWorldCount(); ++i )
	{
		m_ListViewGameList.SetCheckRecord(i);
	}
}

void CGameServerListDlg::OnBnClickedBtnnotselectall()
{
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	if(pApp  == NULL)
		return;
	ServerInfo *pServerInfo = pApp->m_pServerInfo;
	if(pServerInfo == NULL)
		return;

	for(SI32 i = 0; i < pServerInfo->GetWorldCount(); ++i )
	{
		m_ListViewGameList.SetUncheckRecord(i);
	}
}