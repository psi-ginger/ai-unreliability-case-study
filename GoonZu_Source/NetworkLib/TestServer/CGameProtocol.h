#ifndef _CGAMEPROTOCOL_H
#define _CGAMEPROTOCOL_H

#include "ITZNetworkThread.h"
#include "cChar.h"

class CGameProtocol : public ITZNetworkThread
{
public:
	CGameProtocol();
	~CGameProtocol();

	virtual void	SessionProc( ITZSession *pSession );
	virtual void	LogOut( ITZSession *pSession );

private:
	ITZSessionFactory< cChar >	m_SF;
};


#endif

