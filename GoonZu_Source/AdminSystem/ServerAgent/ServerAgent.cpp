#include "ServerAgent.h"
#include "SessionList.h"


void cMaster::Init()
{

	m_dwLastRecvTick = GetTickCount();
	m_dwLastSendTick = GetTickCount() - 5000;

	g_sessionList.Wait();

	m_pServer = NULL;

	m_siServerID = 0;
	m_szFilePath[ 0 ] = NULL;

	g_sessionList.Release();

	printf( "- Master Session 연결\n" );
}


void cMaster::Close()
{
	g_sessionList.Wait();

	m_pServer = NULL;

	m_siServerID = 0;
	m_szFilePath[ 0 ] = NULL;

	g_sessionList.DelMasterSession( this );

	g_sessionList.Release();

	printf( "- Master Session 연결 종료\n" );
}


void cServer::Init()
{
	g_sessionList.Wait();

	m_pMaster = NULL;
	
	m_siServerID = 0;
	m_szFilePath[ 0 ] = NULL;

	g_sessionList.Release();

	printf( "- Server Session 연결\n" );

}

void cServer::Close()
{
	g_sessionList.Wait();

	m_pMaster = NULL;

	m_siServerID = 0;
	m_szFilePath[ 0 ] = NULL;

	g_sessionList.DelServerSession( this );

	g_sessionList.Release();

	printf( "- Server Session 연결 종료\n" );
}
