#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "Char\CharManager\CharManager.h"

#include "../Server/GZFtpMgr/GZFtpMgr.h"

void cltServer::DoMsg_GAMEMSG_GZFTP_PROTOCOL( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgGZFTPProtocol	*pclinfo	=	( cltGameMsgGZFTPProtocol* )pclMsg->cData;

	SI32	__id;

	if( pclinfo->personid_to == 0) 
	{
		// 서버에게 보내는 메시지이다.
		// 서버에서 직접 처리한다.

		if( pclCM->IsValidID( id ) == FALSE )						return;

		m_pclGZFtpMgr->DisaptchGZFtpMsg( ( cltCharServer*)pclCM->CR[ id ], pclinfo );
	}
	else
	{
		// 서버는 브로드캐스팅만 해준다.
		if( ( __id = pclCM->GetIDFromPersonID( pclinfo->personid_to ) ) == 0 )
		{

			return;
		}


		if( pclCM->IsValidID( __id ) == FALSE )
		{
			return;
		}

		pclCM->CR[ __id ]->SendNetMsg( ( sPacketHeader* )pclMsg );
	}
};