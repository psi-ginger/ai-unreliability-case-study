#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../CommonLogic/SoulGuardMgr/SoulGuardMgr.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-SoulGuard.h"


#include "Msg/MsgType-SoulGuard.h"


void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_CREATE(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuardCreate* pclMsg = (sDBResponse_SoulGuardCreate*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	// 1=사람, 2=보관소
	if ( (1 != pclMsg->m_siPosType) && (2 != pclMsg->m_siPosType) )
	{
		return;
	}

	// 보관소에 생성될 경우 보관소 목록을 다시 받는다
	if ( 2 == pclMsg->m_siPosType )
	{
		sDBRequest_SoulGuardHouse_Get clHouseGet( pclMsg->m_siPersonID );
		SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clHouseGet );
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 기본적으로 소환이 안된 상태로 갖고있는다
	SI16 siSoulGuardStatus = pclMsg->m_siStatus;
	if ( SOULGUARD_STATUS_IN != siSoulGuardStatus )
	{
		siSoulGuardStatus = SOULGUARD_STATUS_IN;
	}

	// 서버 캐릭터를 설정하고
	pclChar->pclSoulGuard->SetSoulGuard( pclMsg->m_siSoulGuardID, pclMsg->m_szName, siSoulGuardStatus, pclMsg->m_siKind, pclMsg->m_siLimitDateVary, pclMsg->m_siFellowShip );

	// 일단 정보를 클라로 보내준다
	cltGameMsgResponse_SoulGuardGet clGetInfo(  pclMsg->m_siSoulGuardID, pclMsg->m_szName, siSoulGuardStatus, pclMsg->m_siKind, pclMsg->m_siLimitDateVary, pclMsg->m_siFellowShip  );
	cltMsg clGetMsg( GAMEMSG_RESPONSE_SOULGUARD_GET, sizeof(clGetInfo), (BYTE*)&clGetInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clGetMsg );

	// 수호정령을 처음 얻으면 바로 소환된 상태로 된다
	SI32 siSoulGuardCharID = pclChar->CreateSoulGuard( pclChar->pclSoulGuard );
	if ( pclCM->IsValidID(siSoulGuardCharID) )
	{
		pclChar->siMySoulGuardUnique = siSoulGuardCharID;
	}

	// 소환된 정보를 보내준다 클라이언트로 보내준다
	cltGameMsgResponse_SoulGuardStatusInOut clStatusInfo( pclChar->pclSoulGuard->GetSoulGuardID(), pclChar->pclSoulGuard->GetStatus(), pclChar->siMySoulGuardUnique );
	cltMsg clStatusMsg( GAMEMSG_RESPONSE_SOULGUARD_STATUSINOUT, sizeof(clStatusInfo), (BYTE*)&clStatusInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clStatusMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_GET(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuardGet* pclMsg = (sDBResponse_SoulGuardGet*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	
	// 혹시 오류가 났다면
	if ( 1 != pclMsg->m_siResult )
	{
		//return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 기본적으로 소환이 안된 상태로 갖고있는다
	SI16 siSoulGuardStatus = pclMsg->m_siStatus;
	if ( SOULGUARD_STATUS_IN != siSoulGuardStatus )
	{
		siSoulGuardStatus = SOULGUARD_STATUS_IN;
	}

	// 서버 캐릭터 설정해 주고
	pclChar->pclSoulGuard->SetSoulGuard( pclMsg->m_siSoulGuardID, pclMsg->m_szName, siSoulGuardStatus, pclMsg->m_siKind, pclMsg->m_siLimitDateVary, pclMsg->m_siFellowShip );

	// 클라이언트로 보내준다
	cltGameMsgResponse_SoulGuardGet clGetInfo( pclMsg->m_siSoulGuardID, pclMsg->m_szName, siSoulGuardStatus, pclMsg->m_siKind, pclMsg->m_siLimitDateVary, pclMsg->m_siFellowShip );
	cltMsg clGetMsg( GAMEMSG_RESPONSE_SOULGUARD_GET, sizeof(clGetInfo), (BYTE*)&clGetInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clGetMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_FELLOWSHIP_SET(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuard_FellowShipSet* pclMsg = (sDBResponse_SoulGuard_FellowShipSet*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	if ( pclChar->pclSoulGuard->GetSoulGuardID() != pclMsg->m_siSoulGuardID )
	{
		return;
	}

	// 서버 캐릭에 설정해 주고
	pclChar->pclSoulGuard->SetFellowShip( pclMsg->m_siFellowShip );

	// 친화력이 변경되었으니 그에 따른 능력치가 변경된다 그래서 파라메타 박스를 강제로 돌려준다
	pclChar->SetParametaBoxAction( true );

	// 클라이언트로 보내준다
	cltGameMsgResponse_SoulGuardFellowShipSet clInfo( pclMsg->m_siSoulGuardID, pclMsg->m_siFellowShip );
	cltMsg clMsg( GAMEMSG_RESPONSE_SOULGUARD_FELLOWSHIPSET, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_CHANGE_NAME(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuard_Change_Name* pclMsg = (sDBResponse_SoulGuard_Change_Name*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	if ( pclChar->pclSoulGuard->GetSoulGuardID() != pclMsg->m_siSoulGuardID )
	{
		return;
	}

	// 서버 캐릭터에 설정해 주고
	pclChar->pclSoulGuard->SetName( pclMsg->m_szChangeName );

	// 클라이언트로 보내준다
	cltGameMsgResponse_SoulGuardChangeName clInfo( pclMsg->m_siSoulGuardID, pclMsg->m_szChangeName );
	cltMsg clMsg( GAMEMSG_RESPONSE_SOULGUARD_CHANGENAME, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_DEAD(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuard_Dead* pclMsg = (sDBResponse_SoulGuard_Dead*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 풀어주는 모드일 경우는 소환된 캐릭터를 강제로 지운다
	if ( SOULGUARD_DEAD_MODE_FREE == pclMsg->m_siDeadMode )
	{
		// 소환되어있을경우 그 케릭터는 없애준다
		pclChar->DeleteSoulGuard();
	}
	else
	{
		pclChar->siMySoulGuardUnique = 0;
	}
	
	// 서버 캐릭터에 설정해 주고
	pclChar->pclSoulGuard->InitExceptHouse();

	// 클라이언트에도 보내준다
	cltGameMsgResponse_SoulGuardDead clInfo( pclMsg->m_siDeadMode );
	cltMsg clMsg( GAMEMSG_RESPONSE_SOULGUARD_DEAD, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_HOUSE_GET(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuardHouse_Get* pclMsg = (sDBResponse_SoulGuardHouse_Get*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 갯수가 0개 이하라면 2개 넣어준다
	if ( 0 >= pclMsg->m_siHouseNum )
	{
		cltItem clTempItem;
		clTempItem.Init();

		sDBRequest_SoulGuardHouse_Create clHouseCreate( pclMsg->m_siPersonID, 2, 0, &clTempItem );
		SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clHouseCreate );
	}

	// 서버쪽 캐릭터 설정해 준 후
	pclChar->pclSoulGuard->m_clHouse.Set( pclMsg->m_siHouseNum, pclMsg->m_siSoulGuardID, pclMsg->m_sisoulGuardKind );

	// 클라이언트도 보내준다
	cltGameMsgResponse_SoulGuardHouse_Get clInfo( pclMsg->m_siHouseNum, pclMsg->m_siSoulGuardID, pclMsg->m_sisoulGuardKind );
	cltMsg clMsg( GAMEMSG_RESPONSE_SOULGUARD_HOUSE_GET, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_HOUSE_CHANGE(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuardHouse_Change* pclMsg = (sDBResponse_SoulGuardHouse_Change*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 수호 정령 목록도 변경 되었으므로 다 다시가져온다
	sDBRequest_SoulGuardHouse_Get clHouseGet( pclMsg->m_siPersonID );
	SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clHouseGet );
	

    // 혹시 이전 수호정령이 소환되어져 있을수 있으므로 지운다
	pclChar->DeleteSoulGuard();
	
	// 기본적으로 소환이 안된 상태로 갖고있는다
	SI16 siSoulGuardStatus = pclMsg->m_siStatus;
	if ( SOULGUARD_STATUS_IN != siSoulGuardStatus )
	{
		siSoulGuardStatus = SOULGUARD_STATUS_IN;
	}

	// 서버쪽 셋팅해주고
	pclChar->pclSoulGuard->SetSoulGuard( pclMsg->m_siSoulGuardID, pclMsg->m_szName, siSoulGuardStatus, pclMsg->m_siKind, pclMsg->m_siLimitDateVary, pclMsg->m_siFellowShip );
	
	// 일단 정보를 클라로 보내준다
	cltGameMsgResponse_SoulGuardGet clGetInfo(  pclMsg->m_siSoulGuardID, pclMsg->m_szName, siSoulGuardStatus, pclMsg->m_siKind, pclMsg->m_siLimitDateVary, pclMsg->m_siFellowShip  );
	cltMsg clGetMsg( GAMEMSG_RESPONSE_SOULGUARD_GET, sizeof(clGetInfo), (BYTE*)&clGetInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clGetMsg );

	// 수호정령을 처음 얻으면 바로 소환된 상태로 된다
	SI32 siSoulGuardCharID = pclChar->CreateSoulGuard( pclChar->pclSoulGuard );
	if ( pclCM->IsValidID(siSoulGuardCharID) )
	{
		pclChar->siMySoulGuardUnique = siSoulGuardCharID;
	}

	// 소환된 정보를 보내준다 클라이언트로 보내준다
	cltGameMsgResponse_SoulGuardStatusInOut clStatusInfo( pclChar->pclSoulGuard->GetSoulGuardID(), pclChar->pclSoulGuard->GetStatus(), pclChar->siMySoulGuardUnique );
	cltMsg clStatusMsg( GAMEMSG_RESPONSE_SOULGUARD_STATUSINOUT, sizeof(clStatusInfo), (BYTE*)&clStatusInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clStatusMsg );
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_SOULGUARD_HOUSE_CREATE(sPacketHeader* pPacket)
{
	sDBResponse_SoulGuardHouse_Create* pclMsg = (sDBResponse_SoulGuardHouse_Create*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 서버쪽 캐릭터 설정해 준 후
	if ( (0 <= pclMsg->m_siHouseNum) && (MAX_SOULGUARD_HOUSE_NUM >= pclMsg->m_siHouseNum) )
	{
		pclChar->pclSoulGuard->m_clHouse.SetMaxHaveHouseNum( pclMsg->m_siHouseNum );
	}

	// 사용된 아이템 설정
	if ( (PERSONITEM_INV0 <= pclMsg->m_siLeftItemPos ) && ((PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON) > pclMsg->m_siLeftItemPos) )
	{
		pclChar->pclCI->clCharItem.clItem[pclMsg->m_siLeftItemPos].Set( &pclMsg->m_clLeftItem );
	}

	// 클라이언트도 보내준다
	cltGameMsgResponse_SoulGuardHouse_Create clInfo( pclMsg->m_siHouseNum, pclMsg->m_siLeftItemPos, &pclMsg->m_clLeftItem );
	cltMsg clMsg( GAMEMSG_RESPONSE_SOULGUARD_HOUSE_CREATE, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
}