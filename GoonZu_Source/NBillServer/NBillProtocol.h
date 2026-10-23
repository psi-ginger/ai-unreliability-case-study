#ifndef _NBILLPROTOCOL_H
#define _NBILLPROTOCOL_H

#include "ITZNetworkThread.h"

#include "NBillServer.h"
#include "gbillweb.h"

#define MAX_GAMESERVER_CONNECTION		30

class NBillProtocol : public ITZNetworkThread
{
public:
	NBillProtocol();
	~NBillProtocol();

public:
	void			Init();

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );


public:
	ITZSessionFactory< cGame >	m_SF;

	HINSTANCE	m_hModule;
	pDllFunc	m_pMyFunc;

};












#endif