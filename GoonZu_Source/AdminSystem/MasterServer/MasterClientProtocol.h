#ifndef _MASTERCLIENTPROTOCOL_H
#define _MASTERCLIENTPROTOCOL_H

#include "ITZNetworkThread.h"
#include "MasterServer.h"

const SI32 ConstMAXMasterClientCount = 50;

class MasterClientProtocol : public ITZNetworkThread
{
public:
	MasterClientProtocol();
	~MasterClientProtocol();

public:
	void			Init();

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );
	
	ITZSessionFactory< cClient >	m_SF;

	ITZSession*						m_MasterClientSession[ ConstMAXMasterClientCount ];

protected:
	// 게임 서버 데이터 전송 시간
	SI32			m_siRefreshInfoLastSendTick;
	BOOL			m_bRefreshInfoSendState;
};




#endif

