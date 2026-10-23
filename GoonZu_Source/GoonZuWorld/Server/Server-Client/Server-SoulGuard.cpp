#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../CommonLogic/SoulGuardMgr/SoulGuardMgr.h"

#include "../../common/Order/order.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-SoulGuard.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-SoulGuard.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SoulGuardStatusInOut* pclinfo = (cltGameMsgRequest_SoulGuardStatusInOut*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	//---------------------------------------------
	// [기형] 배틀로얄 경기장이라면 
	//---------------------------------------------
	if(NULL != m_pclBattleRoyalMgr)
	{
		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclChar);
		if( NULL != pclBattleRoyal)
		{
			if( TRUE == pclBattleRoyal->IsExistStadium(pclChar) )
				return;
		}
	}

	// 죽었으면 튕긴다
	if ( FALSE == pclCM->IsAlive(id) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	if ( TRUE == pclChar->pclSoulGuard->IsEmpty() )
	{
		return;
	}

	if ( pclChar->pclSoulGuard->GetSoulGuardID() != pclinfo->m_siSoulGuardID )
	{
		return;
	}

	if ( pclChar->pclSoulGuard->GetStatus() == pclinfo->m_siStatus )
	{
		return;
	}

	// 죽은 상태라면 안됨
	if ( pclChar->pclSoulGuard->GetStatus() == SOULGUARD_STATUS_DEAD )
	{
		return;
	}

	switch ( pclinfo->m_siStatus )
	{
	// 수호정령 넣기
	case SOULGUARD_STATUS_IN:
		{
			pclChar->DeleteSoulGuard();
		}
		break;

	// 수호정령 꺼내기
	case SOULGUARD_STATUS_OUT:
		{
			SI32 siSoulGuardCharID = pclChar->CreateSoulGuard( pclChar->pclSoulGuard );
			if ( false == pclCM->IsValidID(siSoulGuardCharID) )
			{
				return;
			}

			pclChar->siMySoulGuardUnique = siSoulGuardCharID;
		}
		break;
	}

	// 수호 정령을 꺼냈으니 능력치를 다시 계산 하도록 한다
	pclChar->SetParametaBoxAction( true );

	// 클라이언트로 다시 보내준다
	cltGameMsgResponse_SoulGuardStatusInOut clInfo( pclChar->pclSoulGuard->GetSoulGuardID(), pclChar->pclSoulGuard->GetStatus(), pclChar->siMySoulGuardUnique );
	cltMsg clMsg( GAMEMSG_RESPONSE_SOULGUARD_STATUSINOUT, sizeof(clInfo), (BYTE*)&clInfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SOULGUARD_CHANGENAME(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SoulGuardChangeName* pclinfo = (cltGameMsgRequest_SoulGuardChangeName*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 죽었으면 튕긴다
	if ( FALSE == pclCM->IsAlive(id) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	if ( TRUE == pclChar->pclSoulGuard->IsEmpty() )
	{
		return;
	}

	if ( pclChar->pclSoulGuard->GetSoulGuardID() != pclinfo->m_siSoulGuardID )
	{
		return;
	}

	// 죽은 상태라면 안됨
	if ( pclChar->pclSoulGuard->GetStatus() == SOULGUARD_STATUS_DEAD )
	{
		return;
	}

	// 이름을 가져온다
	TCHAR	szOriginalName[MAX_SOULGUARD_NAME]	= TEXT("");
	TCHAR	szChangeName[MAX_SOULGUARD_NAME]	= TEXT("");

	StringCchCopy( szOriginalName,	MAX_SOULGUARD_NAME,	pclChar->pclSoulGuard->GetName() );
	StringCchCopy( szChangeName,	MAX_SOULGUARD_NAME,	pclinfo->m_szChangeName );

	// 글자 길이 가져오기
	size_t sizeLength = 0;
	StringCchLength( szChangeName, MAX_SOULGUARD_NAME, &sizeLength );

	// 문자열의 길이가 0일때
	if ( 0 == sizeLength )
	{
		return;
	}

	// 기존 이름과 동일할때
	if ( 0 == _tcscmp(szOriginalName, szChangeName) )
	{
		return;
	}

	// 욕설필터링 적용
	m_pCAbuseFilter->CheckAbuse( szChangeName );

	// 유효한 이름인지 검사한다
	if ( FALSE == IsCorrectID(szChangeName) )
	{
		return;
	}

	// DB로 요청한다
	sDBRequest_SoulGuard_Change_Name clMsg( pclChar->pclCI->GetPersonID(), pclChar->pclSoulGuard->GetSoulGuardID(), szChangeName );
	SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SOULGUARD_DEAD(cltMsg* pclMsg, SI32 id )
{
	// 이곳은 수호정령 풀어주기 이다

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 죽었으면 튕긴다
	if ( FALSE == pclCM->IsAlive(id) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	if ( TRUE == pclChar->pclSoulGuard->IsEmpty() )
	{
		return;
	}

	// 죽은 상태라면 안됨
	if ( pclChar->pclSoulGuard->GetStatus() == SOULGUARD_STATUS_DEAD )
	{
		return;
	}

	// DB로 요청한다
	sDBRequest_SoulGuard_Dead clMsg( pclChar->pclCI->GetPersonID(), pclChar->pclSoulGuard->GetSoulGuardID(), SOULGUARD_DEAD_MODE_FREE );
	SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SOULGUARD_HOUSE_CHANGE(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SoulGuardHouse_Change* pclinfo = (cltGameMsgRequest_SoulGuardHouse_Change*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	SI32 siGetelectIndex = pclinfo->m_siSelectIndex;
	if ( (0 > siGetelectIndex) || (MAX_SOULGUARD_HOUSE_NUM <= siGetelectIndex) )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 죽었으면 튕긴다
	if ( FALSE == pclCM->IsAlive(id) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	SI16 siGetKindUnique = pclChar->pclSoulGuard->m_clHouse.GetKindUniqueFromIndex( siGetelectIndex );
	if ( false == pclKindInfoSet->IsValidKind(siGetKindUnique) )
	{
		return;
	}

	SI32 siGetSoulGuardID = pclChar->pclSoulGuard->m_clHouse.GetSoulGuardIDFromIndex( siGetelectIndex );
	if ( 0 >= siGetSoulGuardID )
	{
		return;
	}

	SI32 siCharSoulGuardID = pclChar->pclSoulGuard->GetSoulGuardID();
	
	sDBRequest_SoulGuardHouse_Change clMsg( pclChar->pclCI->GetPersonID(), siCharSoulGuardID, siGetSoulGuardID );
	SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
	
}
void cltServer::DoMsg_GAMEMSG_REQUEST_SOULGUARD_HOUSE_CREATE(cltMsg* pclMsg, SI32 id )
{
	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 죽었으면 튕긴다
	if ( FALSE == pclCM->IsAlive(id) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 이미 최대 갯수일때
	SI32 siMaxHouseNum = pclChar->pclSoulGuard->m_clHouse.GetMaxHaveHouseNum();
	if ( MAX_SOULGUARD_HOUSE_NUM <= siMaxHouseNum )
	{
		return;
	}

	// 보관이용권 아이템 위치 찾기(함께 갯수체크도 한다)
	SI16 siItemPos = -1;
	for ( SI16 siPosition=PERSONITEM_INV0; siPosition<(PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON); siPosition++ )
	{
		if ( Const_SoulGuardHouse_ItemUnique == pclChar->pclCI->clCharItem.clItem[siPosition].siUnique )
		{
			if ( Const_SoulGuardHouse_ItemNum <= pclChar->pclCI->clCharItem.clItem[siPosition].siItemNum )
			{
				siItemPos = siPosition;
				break;
			}
		}
	}

	// 위치를 못찾은 것은 아이템이 없다고 봐야한다
	if ( PERSONITEM_INV0 > siItemPos )
	{
		return;
	}

	// 소모시킬 아이템 셋팅
	cltItem clTicketItem;

	clTicketItem.Init();
	clTicketItem.siUnique	= Const_SoulGuardHouse_ItemUnique;
	clTicketItem.siItemNum	= Const_SoulGuardHouse_ItemNum;

	// 갯수는 +1해준다
	siMaxHouseNum = siMaxHouseNum + 1;

	sDBRequest_SoulGuardHouse_Create clMsg( pclChar->pclCI->GetPersonID(), siMaxHouseNum, siItemPos, &clTicketItem );
	SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
		
}
