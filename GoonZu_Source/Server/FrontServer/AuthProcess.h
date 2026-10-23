#ifndef _AUTHPROCESS_H
#define _AUTHPROCESS_H

#include "FrontServer.h"
#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"


class ITZSession;

class AuthProcess 
{
public:
	AuthProcess();
	~AuthProcess();

	void				Init( char *szIP, UI16 usPort );
	void				Run();



public:
	ITZSessionFactory< cAuth >	m_sf;
	ITZSessionManager			m_sm;
	ITZNetIOCP					m_iocp;
	
	ITZSession*					m_pAuthSession;

	char						m_szIP[ 16 ];
	UI16						m_usPort;

};



#endif