#include "resource.h"
#include "TestServer.h"
#include "ITZListen.h"

TestServer::TestServer()
{
}

TestServer::~TestServer()
{
}

void TestServer::OnCreate()
{
	m_gameProtocol.CreateThread();
}

void TestServer::OnCommand( int wmId, int wmEvent )
{
	
	switch( wmId )
	{
	case IDM_SERVER_START:

		m_gameProtocol.GetListen()->Start();

		break;

	case IDM_SERVER_STOP:

		m_gameProtocol.GetListen()->Stop();

		break;

	case IDM_EXIT:

		PostQuitMessage(0);

		break;
	};


}




