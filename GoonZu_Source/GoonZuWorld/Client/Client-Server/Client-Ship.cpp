#include "..\Client.h"
#include "..\Music\Music.h"
#include "../CommonLogic/MsgType.h"

#include "../CommonLogic/Msg/MsgType-Ship.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Interface/ShipDlg/ShipDlg.h"
#include "../NInterface/NShipDlg/NShipDlg.h"
#include "../NInterface/NShipDlg/NShipOwnerPassengerChiefDlg.h"
#include "../Interface/ShipDlg/ShipOwnerPassengerChiefDlg.h"

#include "../Common/ShipListMgr/ShipListMgr.h"

#include "../CommonLogic/Port/Port.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NCharStatus/NCharStatus.h"
#include "../NInterface/NCharStatus/NCharSummon.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic *pclClient;

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPINFOGETANDEMBARKTOSHIP(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipInfoGetAndEmbarkToShip *pclShipInfoGetAndEmbarkToShip = (cltGameMsgResponse_ShipInfoGetAndEmbarkToShip*) pclMsg->cData;

	pclShipListMgr->AddShipListObj( &pclShipInfoGetAndEmbarkToShip->cShipListObj );

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipInfoGetAndEmbarkToShip->siShipID );
	if ( pShipListObj == NULL )
	{
		return;
	}

	SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

	if ( ShipCharID < 1 || pclCM->IsValidID( ShipCharID ) == FALSE )
	{
		return;
	}

	
	SI16 i = 0;
	for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER; ++i )
	{
		pclCM->CR[ShipCharID]->clShipArmInfo.CannonItemList[ i ].siUnique = pclShipInfoGetAndEmbarkToShip->siShipCannonArmInfoList[ i ];
	}

	for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER; ++i )
	{
		pclCM->CR[ShipCharID]->clShipArmInfo.GunItemList[ i ].siUnique = pclShipInfoGetAndEmbarkToShip->siShipGunArmInfoList[ i ];
	}

	
	
	return;
}

// 배의 상세 정보를 받아온다.( CShipListObj )
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPINFOGET(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipInfoGet *pclShipInfoGet = (cltGameMsgResponse_ShipInfoGet*) pclMsg->cData;

	pclShipListMgr->AddShipListObj( &pclShipInfoGet->cShipListObj );

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipInfoGet->siShipID );
	if ( pShipListObj == NULL )
	{
		return;
	}

	SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

	if ( ShipCharID < 1 || pclCM->IsValidID( ShipCharID ) == FALSE )
	{
		return;
	}

	SI16 i = 0;
	for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER; ++i )
	{
		pclCM->CR[ShipCharID]->clShipArmInfo.CannonItemList[ i ].siUnique = pclShipInfoGet->siShipCannonArmInfoList[ i ];
	}

	for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER; ++i )
	{
		pclCM->CR[ShipCharID]->clShipArmInfo.GunItemList[ i ].siUnique = pclShipInfoGet->siShipGunArmInfoList[ i ];
	}	


	return;
}


// 배의 창고 정보 요청
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPGETSTGINFO(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipGetStgInfo *pclShipGetStgInfo = (cltGameMsgResponse_ShipGetStgInfo*) pclMsg->cData;

	SI16 Ref = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclShipGetStgInfo->siShipID );

	if ( Ref < 0 )
	{
		return;
	}
	
	pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].Set( &pclShipGetStgInfo->clShipStgItemInfo );
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPINFOCHANGE(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipInfoChange *pclShipInfoChange = (cltGameMsgResponse_ShipInfoChange*) pclMsg->cData;

	pclCM->CR[1]->pclCI->clShipInfo.SetShipDataChange( &pclShipInfoChange->clShip );

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPEXPLODE(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipExplode *pclShipExplode= (cltGameMsgResponse_ShipExplode*) pclMsg->cData;

	SI16 ShipRef = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclShipExplode->siShipID );
	if ( ShipRef < 0 )
	{
		return;
	}

	// 배 주인인 경우 배를 삭제 시킨다.
	if ( pclCM->CR[1]->pclCI->clShipInfo.IsShipExist( pclShipExplode->siShipID ) )
	{
		pclCM->CR[1]->pclCI->clShipInfo.ShipExplode( pclShipExplode->siShipID );
		pclCM->CR[1]->pclCI->clShipItemInfo.Init( ShipRef );
		pclCM->CR[1]->pclCI->clShipSummonInfo.Init( ShipRef );
		
		char* pTitle = GetTxtFromMgr(1385);
		char* pText  = GetTxtFromMgr(1386);
		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	}
	
	pclCM->CR[1]->ShipPassengerDataInit();

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPENTERTOPORTUNITFORPASSENGER(cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipEnterToPortUnitForPassenger *pclShipExplode= (cltGameMsgResponse_ShipEnterToPortUnitForPassenger*) pclMsg->cData;

	if ( pclCM->CR[1]->clInnerStatus.siRideShipID > 0 && pclCM->CR[1]->clInnerStatus.siRideShipID == pclShipExplode->siShipID )
	{
		pclCM->CR[1]->ShipEnterToPortUnitForPassenger();
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPARMINFO(cltMsg*  pclMsg )
{
	cltGameMsgResponse_ShipArmInfo *pclShipArmInfo = ( cltGameMsgResponse_ShipArmInfo *) pclMsg->cData;

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipArmInfo->siShipID );

	if ( pShipListObj == NULL )
	{
		return;
	}

	if ( pShipListObj->GetShipCharUnique() < 1 )
	{
		return;
	}

	SI32 ShipID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

	if ( ShipID < 1 || pclCM->IsValidID( ShipID ) == FALSE )
	{
		return;
	}

	SI16 i = 0;

	for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER; ++i )
	{
		pclCM->CR[ShipID]->clShipArmInfo.CannonItemList[ i ].siUnique = pclShipArmInfo->siShipCannonArmInfoList[ i ];
	}

	for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER; ++i )
	{
		pclCM->CR[ShipID]->clShipArmInfo.GunItemList[ i ].siUnique = pclShipArmInfo->siShipGunArmInfoList[ i ];
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPSTGITEMIN(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipStgItemIn *pclShipStgItemIn = (cltGameMsgResponse_ShipStgItemIn*) pclMsg->cData;

	SI16 Ref = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclShipStgItemIn->siShipID );

	if ( Ref < 0 )
	{
		return;
	}

	pclCM->CR[1]->pclCI->clCharItem.clItem[ pclShipStgItemIn->siPersonInvItemPos ].Set( &pclShipStgItemIn->clPersonInvItem );

	pclCM->CR[1]->pclCI->clShipItemInfo.SetItem( Ref, pclShipStgItemIn->siStgKind, pclShipStgItemIn->siShipStgItemPos, &pclShipStgItemIn->clShipStgItem );
	
	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipStgItemIn->siShipID );
	if ( pShipListObj == NULL )
	{
		return;
	}

	SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

	if ( ShipCharID < 1 || pclCM->IsValidID( ShipCharID ) == FALSE )
	{
		return;
	}
	
	SI16 l = 0;
	for ( l = 0; l < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER; ++l )
	{
		if ( pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeCannonItem[ l ].siUnique > 0 )
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.CannonItemList[ l ].siUnique = pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeCannonItem[ l ].siUnique;
		}
		else
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.CannonItemList[ l ].siUnique = 0;
		}		
	}

	for ( l = 0; l < MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER; ++l )
	{
		if ( pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeGunItem[ l ].siUnique > 0 )
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.GunItemList[ l ].siUnique = pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeGunItem[ l ].siUnique;
		}
		else
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.GunItemList[ l ].siUnique = 0;
		}
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPSTGITEMOUT(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipStgItemOut *pclShipStgItemOut = (cltGameMsgResponse_ShipStgItemOut*) pclMsg->cData;

	SI16 Ref = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclShipStgItemOut->siShipID );

	if ( Ref < 0 )
	{
		return;
	}

	pclCM->CR[1]->pclCI->clCharItem.clItem[ pclShipStgItemOut->siPersonInvItemPos ].Set( &pclShipStgItemOut->clPersonInvItem );

	pclCM->CR[1]->pclCI->clShipItemInfo.SetItem( Ref, pclShipStgItemOut->siStgKind, pclShipStgItemOut->siShipStgItemPos, &pclShipStgItemOut->clShipStgItem );

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipStgItemOut->siShipID );
	if ( pShipListObj == NULL )
	{
		return;
	}

	SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

	if ( ShipCharID < 1 || pclCM->IsValidID( ShipCharID ) == FALSE )
	{
		return;
	}
	
	SI16 l = 0;
	for ( l = 0; l < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER; ++l )
	{
		if ( pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeCannonItem[ l ].siUnique > 0 )
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.CannonItemList[ l ].siUnique = pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeCannonItem[ l ].siUnique;
		}
		else
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.CannonItemList[ l ].siUnique = 0;
		}		
	}

	for ( l = 0; l < MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER; ++l )
	{
		if ( pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeGunItem[ l ].siUnique > 0 )
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.GunItemList[ l ].siUnique = pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clWeaponStgTypeGunItem[ l ].siUnique;
		}
		else
		{
			pclCM->CR[ ShipCharID ]->clShipArmInfo.GunItemList[ l ].siUnique = 0;
		}
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPTRANSPORTSTGITEMCONSUME(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipTransportStgItemConsume *pclShipTransportStgItemConsume = (cltGameMsgResponse_ShipTransportStgItemConsume*) pclMsg->cData;

	SI16 Ref = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclShipTransportStgItemConsume->siShipID );

	if ( Ref < 0 )
	{
		return;
	}

	pclCM->CR[1]->pclCI->clShipItemInfo.TransportStgItemConsume( Ref, pclShipTransportStgItemConsume->siTransportStgItemPosList, pclShipTransportStgItemConsume->clTransportStgItemList );

	return;
}

// 배의 짐칸 아이템 정보를 업데이트 한다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPTRANSPORTSTGITEMUPDATE(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipTransportStgItemUpdate *pclShipTransportStgItemUpdate = (cltGameMsgResponse_ShipTransportStgItemUpdate*) pclMsg->cData;

	SI16 Ref = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclShipTransportStgItemUpdate->siShipID );

	if ( Ref < 0 )
	{
		return;
	}

	pclCM->CR[1]->pclCI->clShipItemInfo.clShipStgItemInfo[ Ref ].clTransportStgItem[ pclShipTransportStgItemUpdate->siTransportStgItemPos ].Set( &pclShipTransportStgItemUpdate->clTransportStgItemList );

	switch ( pclShipTransportStgItemUpdate->siReason )
	{
	case SHIPSTGITEMUPATE_REASON_TORPEDOSHOOT:
		{
			/*
			어뢰 버튼 disable 
			시간 시작 ( 클라이언트 타이머 셑팅 )
			*/
		}
		break;
	default:
		return;
	}

	return;
}


// 배에 승선하기를 요청한 것에 대한 응답 - 일반 유저 ( 성공적으로 신청했다 또는 실패했다 겠지.. )
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONEMBARK(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipPersonEmbark *pclShipPersonEmbark = (cltGameMsgResponse_ShipPersonEmbark*) pclMsg->cData;
	
	char* pTitle = GetTxtFromMgr(1387);
	char* pText  = GetTxtFromMgr(1388);

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

	return;
}

// 배에 어떤 사용자가 승선하기를 요청한 것을 선주에게 알림( 선주가 허용해 줘야 탑승 가능함으로.. )
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONEMBARKNOTIFY(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipPersonEmbarkNotify *pclShipPersonEmbarkNotify = (cltGameMsgResponse_ShipPersonEmbarkNotify*) pclMsg->cData;

//#if /*defined(_DEBUG) &&*/ defined(_NSHIPDLG_)

//	if( g_SoundOnOffDlg.m_bFullScreenMode)
//	{
		if(m_pDialog[ NSHIP_DLG ] != NULL )
		{
			((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->m_pNShipPassengerDlg->ReqShipEmbark( pclShipPersonEmbarkNotify->siShipID, &pclShipPersonEmbarkNotify->clShipPassenger, pclShipPersonEmbarkNotify->siLevel );
		}
/*	}
	else
	{
		m_pShipDlg->m_pShipOwnerPassengerChiefDlg->ReqShipEmbark( pclShipPersonEmbarkNotify->siShipID, &pclShipPersonEmbarkNotify->clShipPassenger, pclShipPersonEmbarkNotify->siLevel );
	}
#else
	m_pShipDlg->m_pShipOwnerPassengerChiefDlg->ReqShipEmbark( pclShipPersonEmbarkNotify->siShipID, &pclShipPersonEmbarkNotify->clShipPassenger, pclShipPersonEmbarkNotify->siLevel );
#endif
	*/

	return;
}

// 배에서 하선하기를 요청
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONDISEMBARK(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipPersonDisEmbark *pclInfo = (cltGameMsgResponse_ShipPersonDisEmbark*)pclMsg->cData;
	if ( pclInfo->siShipID == pclCM->CR[1]->clInnerStatus.siRideShipID )
	{
		pclCM->CR[1]->ShipPassengerDataInit();
	}
	
	return;
}

// 선주에게 알려줌 ( 배에서 강제 하선 요청에 대한 응답 )
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONDISEMBARKFORCE(cltMsg* pclMsg)
{
	return;
}

// 배에서 하선한 것을 선주에게 알림
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONDISEMBARKNOTIFY(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipPersonDisEmbarkNotify *pclShipPersonDisEmbarkNotify = (cltGameMsgResponse_ShipPersonDisEmbarkNotify*) pclMsg->cData;

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipPersonDisEmbarkNotify->siShipID );

	if ( pShipListObj == NULL )
	{
		return;
	}
	
	for( SI16 i = 0; i < MAX_SHIP_PER_PERSON; ++i )
	{
		if ( pShipListObj->m_clShipPassenger[ i ].clSimplePerson.GetPersonID() == pclShipPersonDisEmbarkNotify->siPersonID )
		{
			pShipListObj->m_clShipPassenger[ i ].Init();
			break;
		}
	}
	
//#if /*defined(_DEBUG) &&*/ defined(_NSHIPDLG_)

//	if( g_SoundOnOffDlg.m_bFullScreenMode)
//	{
		if(m_pDialog[ NSHIP_DLG ] != NULL )
		{
			((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->m_pNShipPassengerDlg->DeleteDisEmbarkPerson( pclShipPersonDisEmbarkNotify->siPersonID );
		}
/*	}
	else
	{
		m_pShipDlg->m_pShipOwnerPassengerChiefDlg->DeleteDisEmbarkPerson( pclShipPersonDisEmbarkNotify->siPersonID );
	}
#else
	m_pShipDlg->m_pShipOwnerPassengerChiefDlg->DeleteDisEmbarkPerson( pclShipPersonDisEmbarkNotify->siPersonID );
#endif
*/
	return;
}

// 배에 승선 허가 - 선주
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONEMBARKALLOW(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipPersonEmbarkAllow *pclShipPersonEmbarkAllow = (cltGameMsgResponse_ShipPersonEmbarkAllow*) pclMsg->cData;
	
	if ( pclShipPersonEmbarkAllow->siShipID < 1	|| 
		pclShipPersonEmbarkAllow->siAddedIndex < 0 ||
		pclShipPersonEmbarkAllow->siAddedIndex >= MAX_SHIP_PASSENGER_NUMBER
		)
	{
		return;
	}

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipPersonEmbarkAllow->siShipID );

	if ( pShipListObj == NULL )
	{
		return;
	}

	pShipListObj->m_clShipPassenger[ pclShipPersonEmbarkAllow->siAddedIndex ].Set( &pclShipPersonEmbarkAllow->clShipPassenger );

//#if /*defined(_DEBUG) &&*/ defined(_NSHIPDLG_)

//	if( g_SoundOnOffDlg.m_bFullScreenMode)
//	{
		if(m_pDialog[ NSHIP_DLG ] != NULL )
		{
			((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->m_pNShipPassengerDlg->DeleteOneReqShipEmbarkPerson( (char*)pclShipPersonEmbarkAllow->clShipPassenger.clSimplePerson.GetName() );
			((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->m_pNShipPassengerDlg->ViewShipPassengerList();
		}
/*	}
	else
	{
		m_pShipDlg->m_pShipOwnerPassengerChiefDlg->DeleteOneReqShipEmbarkPerson( (char*)pclShipPersonEmbarkAllow->clShipPassenger.clSimplePerson.GetName() );
		m_pShipDlg->m_pShipOwnerPassengerChiefDlg->ViewShipPassengerList();
	}
#else
	m_pShipDlg->m_pShipOwnerPassengerChiefDlg->DeleteOneReqShipEmbarkPerson( (char*)pclShipPersonEmbarkAllow->clShipPassenger.clSimplePerson.GetName() );
	m_pShipDlg->m_pShipOwnerPassengerChiefDlg->ViewShipPassengerList();
#endif
*/
	return;
}

// 승선 허가가 떨어져서 배에 탔음을 알림
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONEMBARKALLOWNOTIFY(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipPersonEmbarkAllowNotify *pclInfo = (cltGameMsgResponse_ShipPersonEmbarkAllowNotify *)pclMsg->cData;

	if ( pclInfo->siShipID > 0 )
	{
		pclCM->CR[1]->clInnerStatus.siRideShipID		= pclInfo->siShipID;
		pclCM->CR[1]->clInnerStatus.siShipCharUnique	= pclInfo->siShipCharUnique;
		pclCM->CR[1]->clInnerStatus.siShipOwnerCharUnique	= pclInfo->siShipOwnerCharUnique;
	}

	
	return;
}

// 배에 탑승한 사람들 리스트 ( 승선 요청이 아니라, 승선되어 있는 사람들 리스트임 )
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONEMBARKLIST(cltMsg* pclMsg)
{
	return;
}

// 탑승한 배의 정보를 얻어옮
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPLISTOBJINFO(cltMsg* pclMsg)
{
	return;
}

// 배에 탑승한 사람의 직업을 변경( 포수로 한다는지, 승객으로 다시 바꾼다던지.. )
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPERSONCHANGEJOB(cltMsg* pclMsg)
{
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPASSENGERLIST( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipPassengerList *pclInfo = (cltGameMsgResponse_ShipPassengerList *)pclMsg->cData;

	if ( pclInfo->siShipID > 0 )
	{
		CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclInfo->siShipID );

		if ( pShipListObj == NULL )
		{
			return;
		}

		memcpy( pShipListObj->m_clShipPassenger, pclInfo->clShipPassengerList, sizeof ( cltShipPassenger ) * MAX_SHIP_PASSENGER_NUMBER );

//#if /*defined(_DEBUG) &&*/ defined(_NSHIPDLG_)

//		if( g_SoundOnOffDlg.m_bFullScreenMode)
//		{
			if(m_pDialog[ NSHIP_DLG ] != NULL )
			{
				((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->m_pNShipPassengerDlg->ViewShipPassengerList();
			}
/*		}
		else
		{
			m_pShipDlg->m_pShipOwnerPassengerChiefDlg->ViewShipPassengerList();
		}
#else
		m_pShipDlg->m_pShipOwnerPassengerChiefDlg->ViewShipPassengerList();
#endif
		*/
	}
	

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPPASSENGERDETAILINFO( cltMsg* pclMsg )
{
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPMAKENOTIFY( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipMakeNotify *pclInfo = (cltGameMsgResponse_ShipMakeNotify*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clShipInfo.AddShip( &pclInfo->clShip );

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPREPAIR( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipRepair *pclInfo = (cltGameMsgResponse_ShipRepair*)pclMsg->cData;

	if ( pclInfo->siVillageUnique > 0 )
	{
		if ( pclPortManager->clClientPortUnit[ pclInfo->siPortUnitSlot ].clShip.siShipID == pclInfo->clShipInfo.siShipID )
		{
			pclPortManager->clClientPortUnit[ pclInfo->siPortUnitSlot ].clShip.Set( &pclInfo->clShipInfo );
		}
	}

	SI16 MyShipRef = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclInfo->clShipInfo.siShipID );

	if ( MyShipRef < 0 )
	{
		return;
	}	

	pclCM->CR[1]->pclCI->clShipInfo.SetShipDataChange( &pclInfo->clShipInfo );

	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclInfo->clShipInfo.siShipID );
	if ( pShipListObj != NULL )
	{
		pShipListObj->m_clClientShip.Set( &pclInfo->clShipInfo );

		SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

		if ( ShipCharID > 0 && pclCM->IsValidID( ShipCharID ) == TRUE )
		{
			pclCM->CR[ShipCharID]->SetLife( pclInfo->clShipInfo.siDurability );
		}
	}


	for ( SI16 i = 0; i < MAX_SHIP_TRANSPORT_STG_ITEM_CONSUME; ++i )
	{
		if ( pclInfo->siTransportStgItemPosList[ i ] >= 0 )
		{
			pclCM->CR[1]->pclCI->clShipItemInfo.SetItem( MyShipRef, SHIP_STG_KIND_TRANSPORT, pclInfo->siTransportStgItemPosList[ i ], &pclInfo->clTransportStgItemList[ i ] );
		}
	}
	
//#if /*defined(_DEBUG) &&*/ defined(_NSHIPDLG_)

//		if( g_SoundOnOffDlg.m_bFullScreenMode)
//		{
			if(m_pDialog[ NSHIP_DLG ] != NULL )
			{
				((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->ReloadShipData( pclInfo->clShipInfo.siShipID );
			}
/*		}
		else
		{
			m_pShipDlg->ReloadShipData( pclInfo->clShipInfo.siShipID );
		}
#else
		m_pShipDlg->ReloadShipData( pclInfo->clShipInfo.siShipID );
#endif
		*/
	
	char* pTitle = GetTxtFromMgr(1389);
	char* pText  = GetTxtFromMgr(1390);

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	
	return;
}

// 배의 무기고 아이템을 수리
void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPWEAPONSTGITEMUPDATE( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipWeaponStgItemUpdate *pclInfo = (cltGameMsgResponse_ShipWeaponStgItemUpdate*)pclMsg->cData;

	SI32 MyShipRef = pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( pclInfo->siShipID );
	
	if ( MyShipRef < 0 )
	{
		return;
	}

	pclCM->CR[1]->pclCI->clShipItemInfo.SetItem( MyShipRef, pclInfo->siStgKind, pclInfo->siItemPos, &pclInfo->clItem );
	
	if ( pclInfo->clItem.siUnique > 0 && pclInfo->clItem.GetDurability(  ) < LOW_DURABILITY_NOTIFY )
	{
		char *ItemName = (char*)pclItemManager->GetName( pclInfo->clItem.siUnique );
		if ( ItemName != NULL && strlen( ItemName ) > 0 )
		{
			char Msg[ 128 ] = "";
			char* pText = GetTxtFromMgr(1391);
			sprintf( Msg, pText, ItemName );
			pclMessage->SetMsg( Msg );
		}
	}

	// 배 캐릭 정보 Update 
	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclInfo->siShipID );
	if ( pShipListObj != NULL )
	{
		SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

		if ( ShipCharID > 0 && pclCM->IsValidID( ShipCharID ) == TRUE )
		{
			pclCM->CR[ShipCharID]->clShipArmInfo.Set( pclInfo->siStgKind, pclInfo->siItemPos, &pclInfo->clItem );
		}
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPCHATSEND( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipChatSend *pclShipChatSend = (cltGameMsgResponse_ShipChatSend*) pclMsg->cData;
	
//#if /*defined(_DEBUG) &&*/ defined(_NSHIPDLG_)

//	if( g_SoundOnOffDlg.m_bFullScreenMode)
//	{
		if(m_pDialog[ NSHIP_DLG ] != NULL )
		{
			((CNShipDlg*)m_pDialog[ NSHIP_DLG ])->ShipChatRecv( pclShipChatSend->siShipID, pclShipChatSend->strCharName, pclShipChatSend->strShipChatData );
		}
/*	}
	else
	{
		m_pShipDlg->ShipChatRecv( pclShipChatSend->siShipID, pclShipChatSend->strCharName, pclShipChatSend->strShipChatData );
	}
#else
	m_pShipDlg->ShipChatRecv( pclShipChatSend->siShipID, pclShipChatSend->strCharName, pclShipChatSend->strShipChatData );
#endif
*/
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPNAMECHANGE( cltMsg* pclMsg )
{
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPINPUTSUMMON( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipInputSummon *pclShipInputSummon = (cltGameMsgResponse_ShipInputSummon*) pclMsg->cData;

	SI32 id = 1;

	// 소환수 업데이트
	pclCM->CR[id]->pclCI->clSummonInfo.SummonInit( pclShipInputSummon->uiFromSummonIndex );
	
	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}

	SI32 MyShipRef = pclCM->CR[id]->pclCI->clShipInfo.GetShipDataIndex( pclShipInputSummon->siShipID );	
	if ( MyShipRef < 0 )
	{
		return;
	}
	
	// 배 소환수 정보 업데이트
	pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[pclShipInputSummon->uiToInputIndex].clSummon.Set( &pclShipInputSummon->clSummon );
	pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[pclShipInputSummon->uiToInputIndex].uiSummonJob = SHIP_SUMMON_JOB_BOATMAN;
	pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[pclShipInputSummon->uiToInputIndex].uiSummonJobPos = 0;

	// 배 캐릭 정보 Update 
	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipInputSummon->siShipID );
	if ( pShipListObj != NULL )
	{
		SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

		if ( ShipCharID > 0 && pclCM->IsValidID( ShipCharID ) == TRUE )
		{
			pclCM->CR[ShipCharID]->clShipTotalSummonInfo.clShipSummon[pclShipInputSummon->uiToInputIndex].Set( &pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[pclShipInputSummon->uiToInputIndex] );
		}
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPOUTPUTSUMMON( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipOutputSummon *pclShipOutputSummon = (cltGameMsgResponse_ShipOutputSummon*) pclMsg->cData;

	SI32 id = 1;

	// 배 소환수 정보 업데이트
	SI32 MyShipRef = pclCM->CR[id]->pclCI->clShipInfo.GetShipDataIndex( pclShipOutputSummon->siShipID );	
	if ( MyShipRef < 0 )
	{
		return;
	}

	pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[ pclShipOutputSummon->uiFromSummonIndex ].Init();


	// 소환수 업데이트
	pclCM->CR[id]->pclCI->clSummonInfo.SetSummon( pclShipOutputSummon->uiToInputIndex, &pclShipOutputSummon->clSummon );
	
	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}

	// 배 캐릭 정보 Update 
	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipOutputSummon->siShipID );
	if ( pShipListObj != NULL )
	{
		SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

		if ( ShipCharID > 0 && pclCM->IsValidID( ShipCharID ) == TRUE )
		{
			pclCM->CR[ShipCharID]->clShipTotalSummonInfo.clShipSummon[pclShipOutputSummon->uiFromSummonIndex].Init();
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPSUMMONSET( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipSummonSet *pclShipSummonSet = (cltGameMsgResponse_ShipSummonSet*) pclMsg->cData;

	SI32 id = 1;

	SI32 MyShipRef = pclCM->CR[id]->pclCI->clShipInfo.GetShipDataIndex( pclShipSummonSet->siShipID );	
	if ( MyShipRef < 0 )
	{
		return;
	}

	for ( SI16 i = 0; i < MAX_SHIP_SUMMON_NUMBER; ++i )
	{
		pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[ i ].uiSummonJob = pclShipSummonSet->uiSummonJob[ i ];
		pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[ i ].uiSummonJobPos = pclShipSummonSet->uiSummonJobPos[ i ];
	}

	// 배 캐릭 정보 Update 
	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipSummonSet->siShipID );
	if ( pShipListObj != NULL )
	{
		SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

		if ( ShipCharID > 0 && pclCM->IsValidID( ShipCharID ) == TRUE )
		{
			for ( SI16 k = 0; k < MAX_SHIP_SUMMON_NUMBER; ++k )
			{
				pclCM->CR[ShipCharID]->clShipTotalSummonInfo.clShipSummon[ k ].uiSummonJob = pclShipSummonSet->uiSummonJob[ k ];
				pclCM->CR[ShipCharID]->clShipTotalSummonInfo.clShipSummon[ k ].uiSummonJobPos = pclShipSummonSet->uiSummonJobPos[ k ];
			}
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPSUMMONUPDATE( cltMsg* pclMsg )
{
	cltGameMsgResponse_ShipSummonUpdate *pclShipSummonUpdate = (cltGameMsgResponse_ShipSummonUpdate*) pclMsg->cData;

	SI32 id = 1;

	SI32 MyShipRef = pclCM->CR[id]->pclCI->clShipInfo.GetShipDataIndex( pclShipSummonUpdate->siShipID );	
	if ( MyShipRef < 0 )
	{
		return;
	}

	pclCM->CR[id]->pclCI->clShipSummonInfo.clShipTotalSummonInfo[ MyShipRef ].clShipSummon[ pclShipSummonUpdate->uiShipSummonPos ].Set( &pclShipSummonUpdate->clShipSummon );

	// 배 캐릭 정보 Update 
	CShipListObj *pShipListObj = pclShipListMgr->GetShipListObj( pclShipSummonUpdate->siShipID );
	if ( pShipListObj != NULL )
	{
		SI32 ShipCharID = cltCharCommon::CharUniqueInfo[ pShipListObj->GetShipCharUnique() ];

		if ( ShipCharID > 0 && pclCM->IsValidID( ShipCharID ) == TRUE )
		{
			
			pclCM->CR[ShipCharID]->clShipTotalSummonInfo.clShipSummon[  pclShipSummonUpdate->uiShipSummonPos ].Set( &pclShipSummonUpdate->clShipSummon );
		}
	}
}

void  cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPCANONSHOOT( cltMsg* pclMsg )
{

	cltGameMsgResponse_ShipCanonShoot *clGameMsgResponse_ShipCanonShoot = (cltGameMsgResponse_ShipCanonShoot*) pclMsg->cData;
	int attackstartx, attackstarty;
	SI32 stgkind	= 0;
	SI32 itempos	= 0;
	SI32 crewapower	= 0;
	SI32 i = 0;
	SI32 ShipID = clGameMsgResponse_ShipCanonShoot->siShipID;
	if(ShipID < 0)
	{
		return;
	}	
	
	SI32 attackid =pclClient->pclCM->CR[ ShipID ]->clAttackTarget.GetAttackID();		

	if( attackid < 1 )
	{
		return;
	}

	cltItem* pclitem = NULL;
	if(i < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER)
	{
		stgkind = SHIP_STG_KIND_WEAPON_TYPE_CANNON;
		itempos	= i;
		pclitem = &pclCM->CR[ ShipID ]->clShipArmInfo.CannonItemList[itempos];
		crewapower = pclCM->CR[ ShipID ]->GetCannonCrewDamage(pclKindInfoSet, itempos);
	}

	pclCM->CR[ ShipID ]->DecideAttackPoint(0, attackstartx, attackstarty);

	cltPos cltilestartpos(pclCM->CR[ ShipID ]->GetX(), pclCM->CR[ ShipID ]->GetY());
	cltPos cldotstartpos(attackstartx, attackstarty);

	cltPos cldotdestpos;
	pclCM->CR[attackid]->GetHitPoint(&cldotdestpos);
	cldotdestpos.siX = cldotdestpos.siX + 20 - (rand()  % 40);
	cldotdestpos.siY = cldotdestpos.siY + 20 - (rand()  % 40);

	
	SI32 bulletunique = ATTACKTYPE_CASTLECANNON-1;
	if(bulletunique <= 0) return;
	//continue;

	SI32 apower = crewapower + pclitem->GetMinDamage(pclItemManager) + pclitem->GetMaxDamage(pclItemManager) / 2;


	
	// 캐릭터와 공격대상 지점이 같은 지도여야 한다. 
	if(pclCM->CR[attackid]->GetMapIndex() == pclCM->CR[attackid]->clAttackTarget.clPos.GetMapIndex()
		&& pclCM->CR[attackid]->pclMap->IsInMapArea(pclCM->CR[attackid]->clAttackTarget.clPos.siX, pclCM->CR[attackid]->clAttackTarget.clPos.siY) == true)
	{
		/*
		id = pclClient->pclBulletManager->SetBullet(pclMap, bulletunique, 0,  
		pclCM->CR[attackid]->siID, apower, 
		pclCM->CR[attackid]->cltilestartpos,
		pclCM->CR[attackid]->cldotstartpos, 
		pclCM->CR[attackid]->clAttackTarget,
		pclCM->CR[attackid]->cldotdestpos);
		*/

	}


	return;



}