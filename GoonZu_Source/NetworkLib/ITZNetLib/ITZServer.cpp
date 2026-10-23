/*
#include "ITZServer.h"

unsigned __stdcall server_worker( LPVOID param )
{
	
	ITZServer	*pClass = (ITZServer *)param;

	while( 1 ) {

		Sleep( 10 );

		ITZSesson *pSession;

		for( pSession = pClass->m_pSM->GetFirstSession(); pSession != NULL; pSession = pClass->m_pSM->NexeSession( pSession ) ) {

			Proc( pSession );

		}
	}

}

ITZServer::ITZServer()
{

}

ITZServer::~ITZServer()
{

}


ITZServer::Proc( ITZSession *pSession )
{
	
}
*/