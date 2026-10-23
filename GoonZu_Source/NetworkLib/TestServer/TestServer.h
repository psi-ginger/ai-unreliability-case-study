#include "ITZWinApp.h"

#include "CGameProtocol.h"


class TestServer : public ITZWinApp
{
public:
	TestServer();
	~TestServer();

	// winapp virtual func
	virtual void	OnCreate();
	virtual void	OnCommand( int wmId, int wmEvent );


public:
	CGameProtocol	m_gameProtocol;

};

