#include "MasterProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../MasterServer/MasterMsg.h"

#include "GoonZuMC.h"
#include "GoonZuMCDlg.h"
#include "StatisticsDlg.h"
#include "StatisticsData.h"

#include "StaticBuffer.h"
#include "GMOrderLogManager.h"

#include "../../NLib/NMap.h"

const SI32 ConstTmpBufferSize = 256;

MasterProcess::MasterProcess()
{

	m_pMasterSession = NULL;
	m_szIP[ 0 ] = NULL;

	m_szID[ 0 ] = NULL;
	m_szPW[ 0 ] = NULL;

	m_bLoginOK = FALSE;
	m_bInitOK = FALSE;

	InitializeCriticalSection( &m_cs );
}

MasterProcess::~MasterProcess()
{
	DeleteCriticalSection( &m_cs );
}

void MasterProcess::Init( char *szIP, UI16 usPort )
{

	strcpy( m_szIP, szIP );
	m_usPort = usPort;
		
	m_sf.CreateFactory( &m_sm, 1, 200000, 200000 );
		
	ITZSession *pSession = m_sm.GetSession( 0 );
		
	InterlockedExchange( (LPLONG)&m_pMasterSession, (LONG)pSession );
		
	m_overlapped.CreateOverlapped( &m_sm );
}

void MasterProcess::SetIP( char *szIP )
{
	lock();

	strcpy( m_szIP, szIP );

	unlock();
}

void MasterProcess::SetIDPW( char *szID, char *szPW )
{
	lock();

	strcpy( m_szID, szID );
	strcpy( m_szPW, szPW );
	
	unlock();
}

void MasterProcess::SetConnect( BOOL b )
{		
	lock();

	m_bConnecting = b;

	unlock();
}

void MasterProcess::Run()
{
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	if( pApp->m_bRun == FALSE ) return;


	m_sm.Update();

	cMaster *pMaster = (cMaster *)m_pMasterSession->GetPerson();

	if( m_pMasterSession->GetState() == SESSION_STATE_NONE ) {


		if( m_bConnecting == TRUE ) {
					
			if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pMasterSession ) {

			} else {

			}
		}

		if( m_bLoginOK & m_bInitOK ) {

			if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pMasterSession ) {

			} else {

			}
			
		}

	} else if( m_pMasterSession->GetState() == SESSION_STATE_ESTABLISHED ) {

		if( m_bSendLogin == FALSE ) {
			
			sRequest_GMCLogin	sendMsg;

			strcpy( sendMsg.szID, m_szID );
			strcpy( sendMsg.szPW, m_szPW );

			m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			OutputDebugString("send sRequest_GMCLogin\n");

			m_bSendLogin = TRUE;						// 로그인 요청을 보냈다 

			return;
		}

		if( m_bLoginOK == TRUE && m_bSendReqInit == FALSE && m_bInitOK == FALSE ) {
			// 로그인 된 상태이고, 초기화 정보 요청을 보내지 않았고, 초기화가 안된 상태라면,

			sRequest_ServerInfo1 sendMsg;

			m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			sRequest_ServerInfo2 sendMsg2;

			m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg2 );

			SYSTEMTIME systime;
			GetLocalTime( &systime );

			sRequest_GMCDAYUSERNUM sendMsg3( systime.wYear, systime.wMonth, systime.wDay, 0 );
			
			CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
			pApp->m_pMasterProcess->m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg3 );

			OutputDebugString("send sRequest_GMCDAYUSERNUM\n");
		
			m_bSendReqInit = TRUE;
		}

		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pMasterSession->GetRecvBuffer()->GetFirstPacket();		

		
		if( pPacket != NULL ) {										
			// 패킷이 있으면,
 
			switch( pPacket->usCmd ) {

				case GOONZUMC_RESPONSE_GMORDER:
					{
						CGMOrderLogManager* pGMOrderLogManager = pApp->m_pGMOrderLogManager;
						ServerInfo*	pServerInfo = pApp->m_pServerInfo;
						if(pGMOrderLogManager==NULL)
							break;
						if(pServerInfo == NULL)
							break;
						StaticBuffer*		pBuffer = pGMOrderLogManager->m_pGMOrderLogBuffer;
						if(pBuffer == NULL)
							break;

						sResponse_GMC_GMOrder *pRecvMsg = (sResponse_GMC_GMOrder *)pPacket;
						OutputDebugString("recv sResponse_GMC_GMOrder\n");
						
						TCHAR szWorldName[ConstTmpBufferSize] = {0,};
						sGameWorldInfo* pGameWorldInfo = pServerInfo->GetGameWorldInfo(pRecvMsg->siWorldId);
				
						if( pGameWorldInfo == NULL)
							break;

						if(pGameWorldInfo != NULL)
							StringCchCopy(szWorldName,ConstTmpBufferSize,pGameWorldInfo->szWorldName);

						CGMOrderLog	clGMOrderLog;
						clGMOrderLog.SetOrder(pRecvMsg->order);
						clGMOrderLog.SetOrderPersonID(pRecvMsg->szRequestID);
						clGMOrderLog.SetOrderPersonIP(pRecvMsg->szRequestIP);
						clGMOrderLog.SetOrderTime(pRecvMsg->szRequestTime);
						clGMOrderLog.SetWorldId(pRecvMsg->siWorldId);
						clGMOrderLog.SetGameServerName(szWorldName);
						clGMOrderLog.SetResult(pRecvMsg->siResult);
						pBuffer->In_Data_Buffer((BYTE*)&clGMOrderLog,sizeof(CGMOrderLog));
					}
					break;

			case MASTERMSG_RESPONSE_GMCLOGIN:
				{
					sResponse_GMCLogin	*pRecvMsg = (sResponse_GMCLogin *)pPacket;

					OutputDebugString("recv sResponse_GMCLogin\n");

					if( strcmp( m_szID, "guest" ) == 0 &&
						strcmp( m_szPW, "guest" ) == 0 ) {

						m_bLoginOK = TRUE;
						((CGoonZuMCApp*)AfxGetApp())->m_bMaster = FALSE;
						((CGoonZuMCApp*)AfxGetApp())->m_nMasterLevel = 0;
						break;
					}


					if( pRecvMsg->usResult == 0 ) {

						m_pMasterSession->CloseSocket();

						m_bLoginFailed = TRUE;

						OutputDebugString("pRecvMsg->usResult= 0\n");
						
						return;

					} else {

						OutputDebugString("m_bLoginOK = TRUE\n");

						m_bLoginOK = TRUE;
						((CGoonZuMCApp*)AfxGetApp())->m_bMaster = TRUE;
						((CGoonZuMCApp*)AfxGetApp())->m_nMasterLevel = pRecvMsg->usResult;
					}
				}

				break;

				
			case MASTERMSG_RESPONSE_SERVERINFO1:
				{

					sResponse_ServerInfo1 *pRecvMsg = (sResponse_ServerInfo1 *)pPacket;
					OutputDebugString("recv sResponse_ServerInfo1\n");
					
					ServerInfo *pServerInfo;

					pServerInfo = ((CGoonZuMCApp *)AfxGetApp())->m_pServerInfo;

					pServerInfo->Wait();

					pServerInfo->m_frontServerInfo = pRecvMsg->frontServerInfo;
					pServerInfo->m_authServerInfo = pRecvMsg->authServerInfo;
					pServerInfo->m_accountDBManagerInfo = pRecvMsg->accountDBManagerInfo;

					pServerInfo->Release();

				}
				break;

				
			case MASTERMSG_RESPONSE_SERVERINFO2:
				{
					sResponse_ServerInfo2 *pRecvMsg = (sResponse_ServerInfo2 *)pPacket;
					OutputDebugString("recv sResponse_ServerInfo2\n");

					ServerInfo *pServerInfo;

					pServerInfo = ((CGoonZuMCApp *)AfxGetApp())->m_pServerInfo;
					
					pServerInfo->Wait();

					pServerInfo->m_usWorldCount = pRecvMsg->usCount;

					memcpy( pServerInfo->m_gameWorldInfo, pRecvMsg->gameWorldInfo,
						sizeof( sGameWorldInfo ) * pRecvMsg->usCount );

					pServerInfo->SetServerInfo();

					pServerInfo->Release();

					m_bInitOK = TRUE;

				}
				break;

			case MASTERMSG_REPORT_SERVERREFRESHINFO:
				{

					if( m_bInitOK == FALSE ) break;

					sReport_RefreshServerInfo *pRecvMsg = (sReport_RefreshServerInfo *)pPacket;
					OutputDebugString("recv sReport_RefreshServerInfo\n");

					CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

					if( pApp == NULL ) break;

					

					InterlockedExchange( (LPLONG)&((CGoonZuMCDlg *)pApp->GetMainWnd())->m_maxUser, pRecvMsg->siMaxUserNum );

					ServerInfo *pServerInfo = ((CGoonZuMCApp *)AfxGetApp())->m_pServerInfo;

					pServerInfo->Wait();

					pServerInfo->m_frontRefresh				= pRecvMsg->frontRefresh;
					pServerInfo->m_authRefresh				= pRecvMsg->authRefresh;
					pServerInfo->m_accountDBManagerRefresh	= pRecvMsg->accountDBManagerRefresh;

					for( int i = 0; i < pRecvMsg->usCount; ++i ) {

						pServerInfo->m_gameRefresh[ i ] = pRecvMsg->gameRefresh[ i ];
						pServerInfo->m_gameDBManagerRefresh[ i ] = pRecvMsg->gameDBManagerRefresh[ i ];
						
					}

					pServerInfo->Release();

				}
				break;
			case MASTERMSG_RESPONSE_GMCDAYUSERNUM:
				{

					CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

					if( pApp == NULL ) break;

					sResponse_GMCDAYUSERNUM * pRecvMsg = (sResponse_GMCDAYUSERNUM*)pPacket;
					OutputDebugString("recv sResponse_GMCDAYUSERNUM\n");

					if( pRecvMsg->siResult &&
						pRecvMsg->siLayer >=0 &&
						pRecvMsg->siLayer < MAX_LAYER_NUMBER )
					{

						CStatisticsData *pData = NULL;

						pData = pApp->m_pStatisticsData;

						pData->CriticalEnter();

						for( int i = 0; i < MAX_DATA_NUMBER; ++i )
							pData->m_siUserNum[ pRecvMsg->siLayer ][ i ] = pRecvMsg->siUserNum[ i ];

						pData->m_bVisible[ pRecvMsg->siLayer ]  = TRUE;

						char buf[ 32 ];
						sprintf( buf, "%4d-%02d-%02d", pRecvMsg->siYear, pRecvMsg->siMonth, pRecvMsg->siDay );

						strcpy( pData->m_szDate[ pRecvMsg->siLayer ], buf );

						pData->SetMaxUser( pRecvMsg->siLayer );

						pData->CritcalLeave();

						
						CGoonZuMCDlg *pMainDlg = (CGoonZuMCDlg *)pApp->GetMainWnd();
						CStatisticsDlg *pStatisticsDlg = &pMainDlg->m_StatisticsDlg;

						pStatisticsDlg->m_bResponseData = TRUE;
						pStatisticsDlg->m_siResponseLayer = pRecvMsg->siLayer;

					}
				}
				break;

			case GOONZUMC_RESPONSE_GAMEDBMGR_ERRORQUERY_STRING:
				{
					sResponse_GameDBMgr_ErrorQuery_String* pRecvMsg = (sResponse_GameDBMgr_ErrorQuery_String*)pPacket;

					CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
					if( pApp == NULL ) break;

					CGoonZuMCDlg* pDlg = (CGoonZuMCDlg*)pApp->GetMainWnd();
					if( pDlg == NULL ) break;

					TCHAR szMsg[1024] = { '\0', };

					NMapString<32, 128>		kMap;
					kMap.CreateMap( 20 );
					kMap.Parse( pRecvMsg->m_szMapString );

					SYSTEMTIME systime;			GetLocalTime( &systime );

					// 첫줄만 표시한다.
					int nFind = kMap["ErrorQuery"].Find( "\n" );
					NTCHARString128		kErrorQuery;

					kErrorQuery = kMap["ErrorQuery"].SubStr(0, nFind);

					sprintf( szMsg, "[%02d:%02d:%02d] ErrorQuery :[[%s]%s][%s]\r\n",
							 systime.wHour, systime.wMinute, systime.wSecond,
							 (TCHAR*)kMap["WorldID"], (TCHAR*)kMap["WorldName"], (TCHAR*)kErrorQuery );

					//[진성] 일단 외부에 보여지는 부분이라 개발팀만 볼수있게 수정되기 전까지는 주석처리.
					//pDlg->m_richMsg.AddText( szMsg );
				}
				break;
			}	

			// 처리 후 패킷을 버퍼에서 지운다
			m_pMasterSession->GetRecvBuffer()->RemoveFirstPacket();

		}

	} else {

		 TCHAR	szBuf[1024];
		 if(m_pMasterSession)
		 {
			 StringCchPrintf(szBuf, sizeof(szBuf), "m_pMasterSession->GetState() :%d \n", m_pMasterSession->GetState());
			 OutputDebugString(szBuf);
		 }

	 }
}

