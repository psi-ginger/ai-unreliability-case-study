#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"
#include "char\CharServer\Char-Server.h"
#include "..\Common\Bullet\Bullet.h"
#include "Util\Util.h"

#include "../../../Server/Server.h"

#include "ITZCell.h"
#include "ITZEncrypt.h"

extern cltCommonLogic* pclClient;

extern SI32 AttackAtbInfo[];


// 캐릭터 주변의 이웃 데이터를 정리한다. 
// 서버인 경우에만 수행한다.
// bmode == TRUE :  주변의 이웃을 설정한다. 
// bmode == FALSE : 주변의 이웃을 모두 해제한다.  
BOOL cltCharServer::SetNeighbour()
{
	// 캐릭터가 이동함으로써 위치가 변경되면 이웃 정보를 갱신하게 된다. 
	pclClient->pclMapManager->m_world.GetMap( GetMapIndex() )->MoveChar( GetX(), GetY(), this , false);
	return TRUE;
}

// 이웃에게 메시지를 보낸다. 
BOOL cltCharServer::SendNeighbourMsg(cltMsg* pclmsg, bool bselfswitch)
{
	ITZCell* pCell	= GetCell();
	if(pCell == NULL)return FALSE;

	cltServer* pclserver = (cltServer*)pclClient;
	EnterCriticalSection( &pclserver->m_cs_SendNetMsg );
	{
		UI32 uiCompSize = 0;
		BOOL bCompress = FALSE;
		BYTE* pbufComp = pclserver->m_bufComp;

		if( GetSession() &&
			pclserver->m_uiCompressMinSize > 0 && pclmsg->clHead.usSize >= pclserver->m_uiCompressMinSize )		// 지정된 바이트이상만 압축한다.
		{
			BYTE* pMsg = (BYTE*)pclmsg;
			memcpy(pbufComp, pclmsg, sizeof(sPacketHeader));
			uiCompSize = GetSession()->Compress( &pMsg[sizeof(sPacketHeader)],		pclmsg->clHead.usSize - sizeof(sPacketHeader),		// SOURCE
												 &pbufComp[sizeof(sPacketHeader)],	sizeof(pclserver->m_bufComp) - sizeof(sPacketHeader) );			// COMPRESS BUFFER
			if( uiCompSize > 0 )	
			{
				sPacketHeader* pCompMsg = (sPacketHeader*)pbufComp;
				pCompMsg->usSize = sizeof(sPacketHeader) + uiCompSize;

				// 패킷 명령의 최상위 비트를 세트하면, 패킷이 압축된 것이다.
				pCompMsg->usCmd |= 0x8000;
				bCompress = TRUE;
			}
		}

		if( bCompress == FALSE )
		{
			memcpy(pbufComp, pclmsg, pclmsg->clHead.usSize);
		}

		if(bselfswitch)
		{
			//pCell->SendPacketNearAll((sPacketHeader*)pclmsg);
			pCell->SendPacketNearAll( pbufComp );
		}
		else
		{
			//pCell->SendPacketExceptOne(this, (sPacketHeader*)pclmsg);
			pCell->SendPacketExceptOne( this, pbufComp );	// 암호화 된 데이터를 보낸다.
		}
	}
	LeaveCriticalSection( &pclserver->m_cs_SendNetMsg );

	return TRUE;
}

BOOL cltCharServer::SendNeighbourExceptMeMsg(cltMsg* pclmsg)
{
	ITZCell* pCell	= GetCell();
	if(pCell == NULL)return FALSE;

	cltServer* pclserver = (cltServer*)pclClient;
	EnterCriticalSection( &pclserver->m_cs_SendNetMsg );
	{
		UI32 uiCompSize = 0;
		BOOL bCompress = FALSE;
		BYTE* pbufComp = pclserver->m_bufComp;

		if( pclserver->m_uiCompressMinSize > 0 && pclmsg->clHead.usSize >= pclserver->m_uiCompressMinSize )		// 지정된 바이트이상만 압축한다.
		{
			BYTE* pMsg = (BYTE*)pclmsg;
			memcpy(pbufComp, pclmsg, sizeof(sPacketHeader));
			uiCompSize = GetSession()->Compress( &pMsg[sizeof(sPacketHeader)],		pclmsg->clHead.usSize - sizeof(sPacketHeader),		// SOURCE
												 &pbufComp[sizeof(sPacketHeader)],	sizeof(pclserver->m_bufComp) - sizeof(sPacketHeader) );			// COMPRESS BUFFER
			if( uiCompSize > 0 )	
			{
				sPacketHeader* pCompMsg = (sPacketHeader*)pbufComp;
				pCompMsg->usSize = sizeof(sPacketHeader) + uiCompSize;

				// 패킷 명령의 최상위 비트를 세트하면, 패킷이 압축된 것이다.
				pCompMsg->usCmd |= 0x8000;
				bCompress = TRUE;
			}
		}

		if( bCompress == FALSE )
		{
			memcpy(pbufComp, pclmsg, pclmsg->clHead.usSize);
		}

		pCell->SendPacketNearAllExceptOne( this, pbufComp );
	}
	LeaveCriticalSection( &pclserver->m_cs_SendNetMsg );

	return TRUE;
}