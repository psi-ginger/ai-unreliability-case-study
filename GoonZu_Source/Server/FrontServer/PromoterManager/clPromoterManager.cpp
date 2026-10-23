#include "../Promoter/NHNChinaPromoter.h"
#include ".\clpromotermanager.h"
#include "../FrontMsg.h"
#include "../Log.h"


#ifdef USE_PROMOTOR_FRONT_SERVER

// 이벤트 처리 스레드.
unsigned __stdcall EventThread( LPVOID _pParam )
{

	clThreadInfo*	pclThreadInfo	= (clThreadInfo*)_pParam;
	if( NULL == pclThreadInfo ) 
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
	StringCchPrintf( szLogFileName, sizeof(szLogFileName), "Promoter Thread[%d].log", pclThreadInfo->GetThreadIndex() );

	NHNChinaPromoter::ReturnCode ret;

	// 쓰레드 동작 실시.
	pclThreadInfo->Run();

	while( pclThreadInfo->GetState() )
	{
		WaitForSingleObject( pclThreadInfo->GetEvent(), INFINITE );

		switch( pclThreadInfo->GetOperatorType() )
		{
		case clPromoterInfo::GET_PROMOTER_INFO:
			{
				g_CLogPrint.FilePrint( szLogFileName, "Promoter", "<GET_PROMOTER_INFO : Start> AccountID:[%s]", pclThreadInfo->GetName() ); 

				NHNChinaPromoter::Connection con("purple.promoter/sqlmap");

				//////////////////////////////////////////////////////////////////////////
				// 프로모터 정보					
				//////////////////////////////////////////////////////////////////////////
				ret = NHNChinaPromoter::GetPromoterInfoForNewbie(	con, 
																	"C_GZ", 
																	pclThreadInfo->GetName(), 
																	"null", 
																	svrid, 
																	promoterUserid, 
																	promoterCharid, 
																	auth, 
																	msg	);

				if( NHNChinaPromoter::FAIL == ret )			break;

				g_CLogPrint.FilePrint( szLogFileName, "<GET_PROMOTER_INFO_TOTAL : GetPromoterInfoForNewbie> Account:[%s], CharName:[%s], ServerID:[%s], PromoterUserID:[%s], PromoterCharID:[%s], Auth:[%s]", 
														pclThreadInfo->GetName(), 
														"NULL", 
														svrid, 
														promoterUserid, 
														promoterCharid,
														auth				);


				TCHAR	PromoterName[20]  = "";
				WCHAR	wPromoterName[20];
				MultiByteToWideChar( CP_UTF8, 0, promoterCharid,	-1, wPromoterName, sizeof(wPromoterName) );
#ifndef _UNICODE
				WideCharToMultiByte( CP_ACP,  0, wPromoterName,		-1, PromoterName,  sizeof(PromoterName),  NULL, NULL );
#endif

				if( pclThreadInfo->GetSession()->GetState() == SESSION_STATE_ESTABLISHED ) 
				{
					sResponse_Promoter_Info SendMsg;
					SendMsg.packetHeader.usSize = sizeof( sResponse_Promoter_Info );
					SendMsg.packetHeader.usCmd  = FRONTMSG_RESPONSE_PROMOTER_INFO;
					SendMsg.m_ret = ret;

					SendMsg.m_siServerID = atoi( svrid );
					StringCchCopy( SendMsg.m_szPromoterName, sizeof(SendMsg.m_szPromoterName), promoterCharid );
					
					

					pclThreadInfo->GetSession()->GetSendBuffer()->Write( (BYTE *)&SendMsg );	
				}

				g_CLogPrint.FilePrint( szLogFileName, "<GET_PROMOTER_INFO_TOTAL : End>" ); 
			}
			break;
		}

		// 이벤트를 nonsignal로 변경.
		ResetEvent( pclThreadInfo->GetEvent() );

		// 쓰레드 활동이 끝났음을 세팅.
		pclThreadInfo->SetActive( false );	
	} 

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
		// 쓰레드 종료를 위해 이벤트를 시그널 상태로 변경하여 쉬고있는 쓰레드 하나를 사용한다.
		SetEvent( m_clThreadInfo[siCount].GetEvent() );	

		// 쓰레드 종료.
		m_clThreadInfo[siCount].Exit();

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
	m_clThreadInfo[_siThreadIndex].SetSession( PromoterInfo.GetSession() );

	SetEvent( m_clThreadInfo[_siThreadIndex].GetEvent() );	// 이벤트를 시그널 상태로 변경하여 쉬고있는 쓰레드 하나를 사용한다.
}


#endif	