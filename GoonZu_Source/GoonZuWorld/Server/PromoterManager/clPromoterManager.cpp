#include "CommonLogic.h"
#include "server.h"
#include "Char/CharManager/CharManager.h"

#include "../../../DBManager/GameDBManager_World/dbMsg-Person.h"

#include ".\clpromotermanager.h"

#include "Msg/MsgType-npc.h"

#define PROMOTER_LOG

// 이벤트 처리 스레드.
unsigned __stdcall EventThread( LPVOID _pParam )
{
#ifdef USE_PROMOTOR_SERVER
	clThreadInfo*	pclThreadInfo	= (clThreadInfo*)_pParam;
	cltServer*		pclserver		= (cltServer* )pclClient;
	if( NULL == pclThreadInfo || NULL == pclserver ) 
	{
		MessageBox( NULL, "EventThread(): if( NULL == pclThreadInfo )", "Fail", 0 );
	}

	char	svrid[NHNCHINAPROMOTER_SVRID_SIZE]						= "";
	char	promoterUserid[NHNCHINAPROMOTER_PROMOTER_USERID_SIZE]	= "";
	char	promoterCharid[NHNCHINAPROMOTER_PROMOTER_CHARID_SIZE]	= "";
	char	auth[NHNCHINAPROMOTER_AUTH_SIZE]						= "";
	char	msg[NHNCHINAPROMOTER_MSG_SIZE]							= "";
	char 	code[NHNCHINAPROMOTER_CODE_SIZE]						= "";
	char 	url[NHNCHINAPROMOTER_URL_SIZE]							= "";		

	TCHAR	szLogFileName[128] = "";
	StringCchPrintf( szLogFileName, sizeof(szLogFileName), "config\\Promoter Thread[%d].log", pclThreadInfo->GetThreadIndex() );


	NHNChinaPromoter::ReturnCode ret;

	// 쓰레드 동작 실시.
	pclThreadInfo->Run();

	cltCharServer* pclChar = NULL;

	while( pclThreadInfo->GetState() )
	{
		WaitForSingleObject( pclThreadInfo->GetEvent(), INFINITE );

		pclChar = pclserver->pclCM->GetCharServer( pclThreadInfo->GetID() );
		if( NULL == pclChar ) continue;

		switch( pclThreadInfo->GetOperatorType() )
		{
		case clPromoterInfo::GET_PROMOTER_INFO_TOTAL:
			{
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO_TOTAL : Start> AccountID:[%s], CharName:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName()	); 
#endif

				pclChar->bCellularPhoneAuth = false;
				pclChar->bIsPromoter		= false;	

				NHNChinaPromoter::Connection con("purple.promoter/sqlmap");

				//////////////////////////////////////////////////////////////////////////
				// 프로모터 정보					
				//////////////////////////////////////////////////////////////////////////
				ret = NHNChinaPromoter::GetPromoterInfoForNewbie(	con, 
																	"C_GZ", 
																	pclChar->pclCI->clBI.szAccountID, 
																	pclChar->pclCI->clBI.GetName(), 
																	svrid, 
																	promoterUserid, 
																	promoterCharid, 
																	auth, 
																	msg	);

				if( NHNChinaPromoter::SUCC != ret )			break;
				//if( atoi(svrid) != pclserver->siServerID )	break; 
				//if( 0 >= strlen(promoterCharid) )			break;
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO_TOTAL : GetPromoterInfoForNewbie> Account:[%s], CharName:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName(), 
																svrid, 
																promoterUserid, 
																promoterCharid,
																auth				);
#endif

				// 유저의 핸드폰인증 여부.  
				if( 0 == strcmp( "Y", auth ) )
					pclChar->bCellularPhoneAuth = true;
				else
					pclChar->bCellularPhoneAuth = false;

				pclChar->bIsPromoter = true;

				TCHAR	PromoterName[MAX_PLAYER_NAME]  = "";
				WCHAR	wPromoterName[MAX_PLAYER_NAME];
				MultiByteToWideChar( CP_UTF8, 0, promoterCharid,	-1, wPromoterName, sizeof(wPromoterName) );
#ifndef _UNICODE
				WideCharToMultiByte( CP_ACP,  0, wPromoterName,		-1, PromoterName,  sizeof(PromoterName),  NULL, NULL );
#endif

				// 진성 test
				//StringCchPrintf( PromoterName, sizeof(PromoterName), "NDSsadachun"); 

				// 프로모터의 정보가 더 필요해서 DB에 정보를 더 요청하기위해 sDBRequest_PersonSimpleInfo 이걸쓴다.
				sDBRequest_PersonSimpleInfo SendMsg( pclThreadInfo->GetID(), 0, PERSONSIMPLEINFO_REASON_PROMOTER_INFO, promoterCharid ); 
				pclClient->SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&SendMsg );

				//////////////////////////////////////////////////////////////////////////
				// 프로모터 URL					
				//////////////////////////////////////////////////////////////////////////
				ret = NHNChinaPromoter::GetPromoterCode(	con, 
															"C_GZ",
															promoterUserid,
															promoterCharid,
															code, 
															url, 
															msg	);

				if (NHNChinaPromoter::SUCC != ret) break;
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO_TOTAL : GetPromoterCode> PromoterUserID:[%s], PromoterCharID:[%s], URL:[%s]", 
																promoterUserid, 
																PromoterName,
																url				);
#endif

				TCHAR szURL[NHNCHINAPROMOTER_URL_SIZE];
#ifndef _UNICODE
				StringCchCopy( szURL, sizeof(szURL), url );
#else
				MultiByteToWideChar( CP_ACP, MB_COMPOSITE, url, -1, szURL, sizeof(szURL) );
#endif
				cltGameResponse_Promoter_URL_Get clinfo( szURL );
				cltMsg clMsg(GAMEMSG_RESPONSE_PROMOTER_URL_GET, sizeof(clinfo), (BYTE*)&clinfo);
				//pclChar->SendNetMsg((sPacketHeader*)&clMsg);
				pclserver->SendNetMsg(pclThreadInfo->GetID(), (sPacketHeader*)&clMsg);

				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO_TOTAL : End>" ); 
			}
			break;
		case clPromoterInfo::GET_PROMOTER_INFO:
			{
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO : Start> AccountID:[%s], CharName:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName()	); 
#endif

				pclChar->bCellularPhoneAuth = false;
				pclChar->bIsPromoter		= false;	

				NHNChinaPromoter::Connection con("purple.promoter/sqlmap");

				//////////////////////////////////////////////////////////////////////////
				// 프로모터 정보					
				//////////////////////////////////////////////////////////////////////////
				ret = NHNChinaPromoter::GetPromoterInfoForNewbie(	con, 
																	"C_GZ", 
																	pclChar->pclCI->clBI.szAccountID, 
																	pclChar->pclCI->clBI.GetName(), 
																	svrid, 
																	promoterUserid, 
																	promoterCharid, 
																	auth, 
																	msg	);

				if( NHNChinaPromoter::SUCC != ret )			break;
				//if( atoi(svrid) != pclserver->siServerID )	break;
				//if( 0 >= strlen(promoterCharid) )			break;
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO : GetPromoterInfoForNewbie> Account:[%s], CharName:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName(), 
																svrid, 
																promoterUserid, 
																promoterCharid,
																auth				);
#endif


				// 유저의 핸드폰인증 여부.  
				if( 0 == strcmp( "Y", auth ) )
					pclChar->bCellularPhoneAuth = true;
				else
					pclChar->bCellularPhoneAuth = false;

				pclChar->bIsPromoter = true;

				//StringCchPrintf( promoterCharid, sizeof(promoterCharid), "NDSsadachun");

				TCHAR	PromoterName[MAX_PLAYER_NAME]  = "";
				WCHAR	wPromoterName[MAX_PLAYER_NAME];
				MultiByteToWideChar( CP_UTF8, 0, promoterCharid,	-1, wPromoterName, sizeof(wPromoterName) );
#ifndef _UNICODE
				WideCharToMultiByte( CP_ACP,  0, wPromoterName,		-1, PromoterName,  sizeof(PromoterName),  NULL, NULL );
#endif
				// 프로모터의 정보가 더 필요해서 DB에 정보를 더 요청하기위해 sDBRequest_PersonSimpleInfo 이걸쓴다.
				sDBRequest_PersonSimpleInfo SendMsg( pclThreadInfo->GetID(), 0, PERSONSIMPLEINFO_REASON_PROMOTER_INFO, promoterCharid );
				pclClient->SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&SendMsg );

#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_INFO : End>" ); 
#endif
			}
			break;
		case clPromoterInfo::GET_PROMOTER_URL:
			{
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_URL : Start> AccountID:[%s], CharName:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName()	); 
#endif

				NHNChinaPromoter::Connection con("purple.promoter/sqlmap");

				ret = NHNChinaPromoter::GetPromoterInfoForNewbie(	con, 
																	"C_GZ", 
																	pclChar->pclCI->clBI.szAccountID, 
																	pclChar->pclCI->clBI.GetName(), 
																	svrid, 
																	promoterUserid, 
																	promoterCharid, 
																	auth, 
																	msg	);

				if( NHNChinaPromoter::SUCC != ret )			break;
				//if( atoi(svrid) != pclserver->siServerID )	break;
				//if( 0 >= strlen(promoterCharid) )			break;
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_URL : GetPromoterInfoForNewbie> Account:[%s], CharName:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName(), 
																svrid, 
																promoterUserid, 
																promoterCharid,
																auth				);
#endif


				ret = NHNChinaPromoter::GetPromoterCode(	con, 
															"C_GZ",
															promoterUserid,
															promoterCharid, 
															code, 
															url, 
															msg	);

				if (NHNChinaPromoter::SUCC != ret) break;
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<GET_PROMOTER_URL: GetPromoterCode> PromoterUserID:[%s], PromoterCharID:[%s], URL:[%s]", 
					promoterUserid, 
					promoterCharid,
					url				);
#endif

				TCHAR szURL[NHNCHINAPROMOTER_URL_SIZE];
#ifndef _UNICODE
				StringCchCopy( szURL, sizeof(szURL), url );
#else
				MultiByteToWideChar( CP_ACP, MB_COMPOSITE, url, -1, szURL, sizeof(szURL) );
#endif
				cltGameResponse_Promoter_URL_Get clinfo( szURL ); 
				cltMsg clMsg(GAMEMSG_RESPONSE_PROMOTER_URL_GET, sizeof(clinfo), (BYTE*)&clinfo);
				//pclChar->SendNetMsg((sPacketHeader*)&clMsg);
				pclserver->SendNetMsg(pclThreadInfo->GetID(), (sPacketHeader*)&clMsg);
			}
			break;
		case clPromoterInfo::SET_PROMOTER_CHAR:
			{
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<SET_PROMOTER_CHAR : Start> AccountID:[%s]", pclThreadInfo->GetAccountID() ); 
#endif

				//pclChar->bCellularPhoneAuth = false;
				//pclChar->bIsPromoter		= false;	

				NHNChinaPromoter::Connection con("purple.promoter/sqlmap");

				// 계정으로 프로모터가 있는지 정보를 얻어온다.
				ret = NHNChinaPromoter::GetPromoterInfoForNewbie(	con, 
																	"C_GZ", 
																	pclThreadInfo->GetAccountID(), 
																	"null",
																	svrid, 
																	promoterUserid, 
																	promoterCharid, 
																	auth, 
																	msg	);

				// 프로모터와 연결된 캐릭이 없을 때 캐릭터 이름을 "null"로 넘겨주면 SUCC를 리턴받을수있다.
				if( NHNChinaPromoter::SUCC != ret )	break;

				TCHAR	PromoterName[MAX_PLAYER_NAME]  = "";
				WCHAR	wPromoterName[MAX_PLAYER_NAME];
				MultiByteToWideChar( CP_UTF8, 0, promoterCharid,	-1, wPromoterName, sizeof(wPromoterName) );
#ifndef _UNICODE
				WideCharToMultiByte( CP_ACP,  0, wPromoterName,		-1, PromoterName,  sizeof(PromoterName),  NULL, NULL );
#endif
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<SET_PROMOTER_CHAR : GetPromoterInfoForNewbie> Account:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
																pclThreadInfo->GetAccountID(), 
																svrid, 
																promoterUserid, 
																PromoterName,
																auth				);

				pclClient->pclLog->FilePrint( szLogFileName, "<SET_PROMOTER_CHAR : ServerID> Game Server:[%d], ServerID:[%s]", pclserver->siServerID, svrid );
#endif

				if( atoi(svrid) != pclserver->siServerID )	break;

				// 계정으로 얻은 프로모터를 해당 캐릭터에 연결시킨다.
				ret = NHNChinaPromoter::SetFirstCharidInfoForNewbie(	con, 
																		"C_GZ", 
																		svrid,
																		pclThreadInfo->GetAccountID(),
																		pclThreadInfo->GetName(),
																		promoterUserid,
																		promoterCharid,
																		msg);

				if( NHNChinaPromoter::SUCC != ret )	break;

#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<SET_PROMOTER_CHAR : SetFirstCharidInfoForNewbie> Account:[%s], CharName:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
																pclThreadInfo->GetAccountID(), 
																pclThreadInfo->GetName(), 
																svrid, 
																promoterUserid, 
																promoterCharid,
																auth				);
#endif

				// 피추천인에게 추천인이 있음을 등록 시킴
				cltGameResponse_Promoter_Set clinfo( PromoterName );
				cltMsg clMsg(GAMEMSG_RESPONSE_PROMOTER_SET, sizeof(clinfo), (BYTE*)&clinfo);
				//pclChar->SendNetMsg((sPacketHeader*)&clMsg);
				pclserver->SendNetMsg(pclThreadInfo->GetID(), (sPacketHeader*)&clMsg);

				pclClient->pclLog->FilePrint( szLogFileName, "<SET_PROMOTER_CHAR : End>" ); 
			}
			break;
		case clPromoterInfo::PROMOTER_INTRODUCE:
			{
#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<PROMOTER_INTRODUCE : Start> AccountID:[%s], CharName:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName()	); 
#endif

				NHNChinaPromoter::Connection con("purple.promoter/sqlmap");

				ret = NHNChinaPromoter::GetPromoterInfoForNewbie(	con, 
																	"C_GZ", 
																	pclChar->pclCI->clBI.szAccountID, 
																	pclChar->pclCI->clBI.GetName(), 
																	svrid,
																	promoterUserid, 
																	promoterCharid, 
																	auth, 
																	msg	);

				if( NHNChinaPromoter::SUCC != ret )			break;

#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<PROMOTER_INTRODUCE : GetPromoterInfoForNewbie> Account:[%s], CharName:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
																pclChar->pclCI->clBI.szAccountID, 
																pclChar->pclCI->clBI.GetName(), 
																svrid, 
																promoterUserid, 
																promoterCharid,
																auth				);
#endif

				TCHAR	PromoterName[MAX_PLAYER_NAME]  = "";
				WCHAR	wPromoterName[MAX_PLAYER_NAME];
				MultiByteToWideChar( CP_UTF8, 0, promoterCharid,	-1, wPromoterName, sizeof(wPromoterName) );
#ifndef _UNICODE
				WideCharToMultiByte( CP_ACP,  0, wPromoterName,		-1, PromoterName,  sizeof(PromoterName),  NULL, NULL );
#endif

				//StringCchPrintf( PromoterName, sizeof(PromoterName), "NDSsadachun");

				// 추천인이 피추천인에게 인사말. (추천인의 정보를 요청)
				sDBRequest_PersonSimpleInfo SendMsg1( pclThreadInfo->GetID(), 0, PERSONSIMPLEINFO_REASON_RECOMMEND_INTRODUCE, pclChar->GetName(), promoterCharid);
				pclClient->SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&SendMsg1 );

				// 피추천인이 추천인에게 인사말.( 피추천인의 정보를 요청)
				sDBRequest_PersonSimpleInfo SendMsg2( pclThreadInfo->GetID(), 0, PERSONSIMPLEINFO_REASON_RECOMMENDEE_INTRODUCE, promoterCharid, pclChar->GetName() );
				pclClient->SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&SendMsg2 );

#ifdef PROMOTER_LOG
				pclClient->pclLog->FilePrint( szLogFileName, "<PROMOTER_INTRODUCE : End>" ); 
#endif
			}
			break;
		}


		// 이벤트를 nonsignal로 변경.
		ResetEvent( pclThreadInfo->GetEvent() );

		// 쓰레드 활동이 끝났음을 세팅.
		pclThreadInfo->SetActive( false );	
	} 

#endif	
	return 0;
}


clPromoterManager::clPromoterManager()
{
	InitializeCriticalSection( &m_CS );

	// 각 쓰레드에 사용된 이벤트 생성.
	for( int siCount = 0; siCount < PROMOTER_THREAD_NUM; ++siCount )
	{
		m_clThreadInfo[siCount].SetEvent( CreateEvent( NULL, TRUE, FALSE, NULL ) );

		if( NULL == m_clThreadInfo[siCount].GetEvent() ) 
			MessageBox( NULL, "clPromoterManager(): m_clPromoterEvent[siCount].GetPromoterEvent() Fail", "Fail", 0 );
	}

	// 쓰레드 생성.
	unsigned int uiThreadID;
	for( int siCount = 0; siCount < PROMOTER_THREAD_NUM; ++siCount )
	{
		m_clThreadInfo[siCount].SetThreadIndex( siCount );

		if( 0 == _beginthreadex( NULL, 0, EventThread, (LPVOID)&m_clThreadInfo[siCount], 0, &uiThreadID ) )
			MessageBox( NULL, "clPromoterManager(): _beginthreadex() Fail", "Fail", 0 );
	}
}

clPromoterManager::~clPromoterManager(void)
{
	DeleteCriticalSection( &m_CS );

	for( int siCount = 0; siCount < PROMOTER_THREAD_NUM; ++siCount )
	{
		// 쓰레드 종료.
		m_clThreadInfo[siCount].Exit();

		// 쓰레드 종료를 위해 이벤트를 시그널 상태로 변경하여 쉬고있는 쓰레드 하나를 사용한다.
		SetEvent( m_clThreadInfo[siCount].GetEvent() );	

		// 이벤트 핸들 제거.
		CloseHandle( m_clThreadInfo[siCount].GetEvent() );
	}
}

void clPromoterManager::Action( void )
{
	Pop();
}

void clPromoterManager::Push( clPromoterInfo PromoterInfo )
{
	m_clPromoterInfoQueue.push( PromoterInfo );
}

bool clPromoterManager::Pop( void )
{
	if( m_clPromoterInfoQueue.empty() )	return false;

	for( int siCount = 0; siCount < PROMOTER_THREAD_NUM; ++siCount )
	{
		if( false == m_clThreadInfo[siCount].GetActive() ) 
		{
			ActionThread( siCount );

			return true;
		}
	}

	return false;
}

void clPromoterManager::Lock( void )
{
	EnterCriticalSection( &m_CS );
}

void clPromoterManager::UnLock( void )
{
	LeaveCriticalSection( &m_CS );
}

clThreadInfo* clPromoterManager::GetCreateThreadInfo( void )
{
	static int m_siGetThreadCount = 0;	

	if( PROMOTER_THREAD_NUM <= m_siGetThreadCount )
	{
		MessageBox( NULL, "GetCreateEventThread(): GetCreateEventThread() Fail", "Fail", 0 );
		return NULL;
	}

	return &m_clThreadInfo[m_siGetThreadCount++];
}

void clPromoterManager::ActionThread( int _siThreadIndex )
{
	static clPromoterInfo PromoterInfo;
	PromoterInfo = m_clPromoterInfoQueue.front();
	m_clPromoterInfoQueue.pop();

	m_clThreadInfo[_siThreadIndex].SetActive( true );
	m_clThreadInfo[_siThreadIndex].SetOperatorType( PromoterInfo.GetOperatorType() );
	m_clThreadInfo[_siThreadIndex].SetID( PromoterInfo.GetID() );
	m_clThreadInfo[_siThreadIndex].SetName( PromoterInfo.GetName() );
	m_clThreadInfo[_siThreadIndex].SetAccountID( PromoterInfo.GetAccountID() );

	SetEvent( m_clThreadInfo[_siThreadIndex].GetEvent() );	// 이벤트를 시그널 상태로 변경하여 쉬고있는 쓰레드 하나를 사용한다.
}

