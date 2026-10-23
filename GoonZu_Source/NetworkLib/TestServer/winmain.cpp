#include "TestServer.h"
#include "ITZWorld.h"

#include "resource.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{



	TestServer app;

	app.Init( hInstance, "testserver", "testserver", (LPCSTR)IDR_MENU1, 0, 0 );

	app.ProcessMessageLoop();


	return 0;
}

