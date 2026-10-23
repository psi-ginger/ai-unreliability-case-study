#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"

#include "..\..\..\DBManager\GameDBManager\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager\DBMsg-Item.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"
#include "../Common/ShipListMgr/ShipListObj.h"
#include "../Common/ShipListMgr/ShipListMgr.h"
#include "../CommonLogic/Msg/MsgType-Ship.h"

#include "../Common/Order/order.h"

#include "../CharServer/Char-Server.h"

#include "../Server/Server.h"

#include "../CommonLogic/MsgRval-Define.h"

extern cltCommonLogic* pclClient;

void cltCharCommon::ShipPassengerDataInit()
{	
	siRequestEmbarkShipID = 0;
	clInnerStatus.siRideShipID = 0;	
	clInnerStatus.siSelfShipID = 0;
	clInnerStatus.siShipCharUnique = 0;
	clInnerStatus.siShipOwnerCharUnique = 0;
}

// 배를 탄 상태로 포구에 들어 갔을때 처리 ( 클라이언트용 )
void cltCharCommon::ShipEnterToPortUnitForPassenger()
{	
	siRequestEmbarkShipID = 0;

	clInnerStatus.siShipCharUnique = 0;		// 배가 없어졌으니까, 배의 CharID 가 초기화 되어야 겠지..
}

bool cltCharCommon::GetShipOwnerInfo( SI32 *pShipOwnerCharUnique )
{
	if ( pShipOwnerCharUnique == NULL )
	{
		return false;
	}

	if ( clInnerStatus.siShipOwnerCharUnique < 1 )
	{
		return false;
	}

	*pShipOwnerCharUnique = clInnerStatus.siShipOwnerCharUnique;
	
	return true;
}

void cltCharCommon::SetShipOwnerInfo( SI32 ShipOwnerCharUnique )
{
	clInnerStatus.siShipOwnerCharUnique = ShipOwnerCharUnique;

	return;
}

void cltCharCommon::SendMsgToAllPassenger( char *pMsg )
{
	if ( clInnerStatus.siRideShipID  < 1 )
	{
		return;
	}



	return;
}

void cltCharCommon::SendMsgToShipOwner( char *pMsg )
{
	if ( clInnerStatus.siRideShipID  < 1 )
	{
		return;
	}

	return;
}

// 승선 하기를 요청
// VillageUnique == 0 인 경우는 야외지역에서 승선
// VillageUnique > 0 인 경우에는 포구에서 승선
void cltCharCommon::ReqEmbarkShip( SI32 ShipID, SI32 ShipOwnerPersonID )
{
	// 배에 타고 있는 상태에서는 안되지..
	if ( clInnerStatus.siRideShipID > 0 || clInnerStatus.siShipOwnerCharUnique > 0 || clInnerStatus.siShipCharUnique > 0 )
	{
		return;
	}	

	if ( ShipOwnerPersonID < 1 )
	{
		return;
	}

	if ( ShipID < 1 )
	{
		return;
	}

	// 선주가 현재 접속 중인지 확인
	SI32 ShipOwnerCharID = pclClient->pclCM->GetIDFromPersonID( ShipOwnerPersonID );

	if ( ShipOwnerCharID < 1 || pclClient->pclCM->IsValidID( ShipOwnerCharID ) == FALSE )
	{
		return;
	}	

	CShipListObj *pShipListObj = pclClient->pclShipListMgr->GetShipListObj( ShipID );

	if ( pShipListObj == NULL )
	{
		// 선주 등록
		pShipListObj = pclClient->pclShipListMgr->CreateShipListObj( ShipID, &pclClient->pclCM->CR[ShipOwnerCharID]->pclCI->clBI.clPerson, ShipOwnerCharID );
		if ( pShipListObj == NULL )
		{
			return;
		}
	}

	SI32 MyID = GetID();

	// 승선하는 사람이 선주인 경우에는 그냥 승선 시킴
	if ( MyID == ShipOwnerCharID )
	{		
		cltShipPassenger clTempShipPassenger;
		clTempShipPassenger.Init();
		clTempShipPassenger.Set( &pclCI->clBI.clPerson, MyID, SHIP_JOB_OWNER );

		// 배를 탔음을 캐릭터에 설정
		EmbarkShip( MyID, pclCI->GetPersonID(), ShipID );

		return;
	}

	/*
	// 선주가 지우기 하면 siRequestEmbarkShipID 를 0 으로 만들어야 하는데.. 그게 더 부하가 클듯
	// 기존에 배에 탈려고 요청한 배에 또 신청한 경우.
	if ( siRequestEmbarkShipID == ShipID )
	{
	// 신청되어 있다고 알려줘야지...

	// 배에 신청이 꽉차면... 뭐 못 타는 거지... 이 경우에는 선주한테 알려줘야지.. 거시기 누가 신청했는데, 신청이 초과해서 못 태운다고.. 리스트 초기화좀 하라고..

	return;
	}
	*/

	siRequestEmbarkShipID = ShipID;

	// 배에 탑승 요청을 성공적으로 했음.
	cltGameMsgResponse_ShipPersonEmbark clinfo( true, ShipID );
	cltMsg clMsg( GAMEMSG_RESPONSE_SHIPPERSONEMBARK,  sizeof( clinfo ), (char*)&clinfo );
	SendNetMsg((char*)&clMsg);

	cltShipPassenger clShipPassenger;
	clShipPassenger.Init();
	clShipPassenger.Set( &pclCI->clBI.clPerson, MyID, SHIP_JOB_PASSENGER );

	// 선주에게 누가 배에 승선하기를 요청했음을 알림
	cltGameMsgResponse_ShipPersonEmbarkNotify clinfo1( ShipID, &clShipPassenger.clSimplePerson, pclCI->clIP.GetLevel() );
	cltMsg clMsg1( GAMEMSG_RESPONSE_SHIPPERSONEMBARKNOTIFY,  sizeof( clinfo1 ), (char*)&clinfo1 );
	pclClient->pclCM->CR[ShipOwnerCharID]->SendNetMsg((char*)&clMsg1);
}

void cltCharCommon::ReqEmbarkCancel( SI32 ShipID )
{
	// 배를 타고 있으면 끝난거지...
	if ( ShipID > 0 )
	{
		return;
	}

	// 내가 배를 타고 있지 않는 경우에만 해당하겠지...
	if ( clInnerStatus.siRideShipID < 1 )
	{
		return;
	}

	// 기존에 요청한 적이 없는 경우.
	if ( siRequestEmbarkShipID < 1 )
	{
		return;
	}

	siRequestEmbarkShipID = 0;

	cltGameMsgResponse_ShipPersonEmbarkCancel clinfo( true );
	cltMsg clMsg( GAMEMSG_RESPONSE_SHIPPERSONEMBARKCANCEL,  sizeof( clinfo ), (char*)&clinfo );
	SendNetMsg((char*)&clMsg);

	return;
}

// 승선 시키기 - 선주만 호출 가능
void cltCharCommon::EmbarkShip( SI32 ShipPassengerCharID, SI32 ShipPassengerPersonID, SI32 ShipID )
{	
	cltServer *pclServer = (cltServer *)pclClient;

	if ( ShipID < 1 )
	{
		return;
	}

	if ( ShipPassengerCharID < 1 || ShipPassengerPersonID < 1 ||
		pclClient->pclCM->IsValidID( ShipPassengerCharID ) == FALSE ||
		pclClient->pclCM->CR[ShipPassengerCharID]->pclCI->GetPersonID() != ShipPassengerPersonID )
	{
		return;
	}

	SI32 MyID = GetID();

	if ( MyID != ShipPassengerCharID && pclClient->pclCM->CR[ShipPassengerCharID]->siRequestEmbarkShipID != ShipID )
	{
		return;
	}

	// 선주 인지 확인
	if ( ! pclClient->pclCM->CR[MyID]->pclCI->clShipInfo.IsShipExist( ShipID ) )
	{
		return;
	}
	
	// 이미 다른 배를 타고 있다면, 승선 시킬 수 없는 것이지..
	if ( pclClient->pclCM->CR[ShipPassengerCharID]->clInnerStatus.siRideShipID > 0 &&
		pclClient->pclCM->CR[ShipPassengerCharID]->clInnerStatus.siRideShipID != ShipID
		)
	{
		return;
	}

	CShipListObj *pShipListObj = pclClient->pclShipListMgr->GetShipListObj( ShipID );

	if ( pShipListObj == NULL )
	{
		// 선주 등록
		pShipListObj = pclClient->pclShipListMgr->CreateShipListObj( ShipID, &pclCI->clBI.clPerson, MyID );
		if ( pShipListObj == NULL )
		{
			return;
		}
	}

	SI16 AddedIndex = -1;

	// 내배에 나를 승선시키는 경우( 선주가 배를 탄다는 것이지 )
	if ( MyID == ShipPassengerCharID )
	{
		AddedIndex = pShipListObj->Add( &pclCI->clBI.clPerson, MyID, SHIP_JOB_OWNER );
		if ( AddedIndex < 0 )
		{
			pclServer->SendServerResponseMsg(0, SRVAL_SHIP_PASSENGERNUM_EXCEED, 0, 0, GetCharUnique());
			return;
		}

		cltShipPassenger clTempShipPassenger;
		clTempShipPassenger.Init();
		clTempShipPassenger.Set( &pclCI->clBI.clPerson, ShipPassengerCharID, SHIP_JOB_OWNER );

		// 선주에게 승선하게 했음을 알림
		cltGameMsgResponse_ShipPersonEmbarkAllow clinfo( ShipID, AddedIndex, &clTempShipPassenger );
		cltMsg clMsg( GAMEMSG_RESPONSE_SHIPPERSONEMBARKALLOW,  sizeof( clinfo ), (char*)&clinfo );
		SendNetMsg((char*)&clMsg);
		
		// 승객(=선주)에게 승선되었음을 알림
		cltGameMsgResponse_ShipPersonEmbarkAllowNotify clinfo1( ShipID, pShipListObj->GetShipCharUnique(), GetCharUnique(), AddedIndex );
		cltMsg clMsg1( GAMEMSG_RESPONSE_SHIPPERSONEMBARKALLOWNOTIFY,  sizeof( clinfo1 ), (char*)&clinfo1 );
		pclClient->pclCM->CR[ShipPassengerCharID]->SendNetMsg((char*)&clMsg1);
	}
	else
	{
		AddedIndex = pShipListObj->Add( &pclClient->pclCM->CR[ShipPassengerCharID]->pclCI->clBI.clPerson, ShipPassengerCharID, SHIP_JOB_PASSENGER );
		if ( AddedIndex < 0 )
		{			
			pclServer->SendServerResponseMsg(0, SRVAL_SHIP_PASSENGERNUM_EXCEED, 0, 0, GetCharUnique());
			return;
		}

		cltShipPassenger clTempShipPassenger;
		clTempShipPassenger.Init();
		clTempShipPassenger.Set( &pclClient->pclCM->CR[ShipPassengerCharID]->pclCI->clBI.clPerson, ShipPassengerCharID, SHIP_JOB_PASSENGER );

		// 선주에게 승선하게 했음을 알림
		cltGameMsgResponse_ShipPersonEmbarkAllow clinfo( ShipID, AddedIndex, &clTempShipPassenger );
		cltMsg clMsg( GAMEMSG_RESPONSE_SHIPPERSONEMBARKALLOW,  sizeof( clinfo ), (char*)&clinfo );
		SendNetMsg((char*)&clMsg);
		
		// 승객에게 승선되었음을 알림
		cltGameMsgResponse_ShipPersonEmbarkAllowNotify clinfo1( ShipID, pShipListObj->GetShipCharUnique(), pShipListObj->m_siShipOwnerCharUnique, AddedIndex );
		cltMsg clMsg1( GAMEMSG_RESPONSE_SHIPPERSONEMBARKALLOWNOTIFY,  sizeof( clinfo1 ), (char*)&clinfo1 );
		pclClient->pclCM->CR[ShipPassengerCharID]->SendNetMsg((char*)&clMsg1);
	}
		
	pclClient->pclCM->CR[ShipPassengerCharID]->siRequestEmbarkShipID = 0;
	pclClient->pclCM->CR[ShipPassengerCharID]->clInnerStatus.siRideShipID = ShipID;	

	pShipListObj->PersonWarpIntoShip(pclClient->pclCM->CR[ShipPassengerCharID]->pclCI->GetPersonID() );

	cltCharServer *pclPassengerChar = (cltCharServer *)pclClient->pclCM->CR[ShipPassengerCharID];
	pclPassengerChar->SendInnerStatusToNeighbour();

	return;
}

// 하선 - 내 스스로 배에서 내리는 경우임
void cltCharCommon::DisEmbarkShip( SI32 ShipID, bool Force )
{
	if ( ShipID < 1 )
	{
		return;
	}

	// 배에 타고 있지 않은 상태 또는 다른 배에서 하선하라는 요상한 경우.
	if ( clInnerStatus.siRideShipID < 1 || clInnerStatus.siRideShipID != ShipID )
	{
		return;
	}

	CShipListObj *pShipListObj = pclClient->pclShipListMgr->GetShipListObj( ShipID );

	// 배를 타고는 있는데, 배가 없는 아주 이상한 경우.
	if ( pShipListObj == NULL )
	{
		char* pText = GetTxtFromMgr(3810);
		pclClient->pclLog->FilePrint( "Config/Ship.txt", pText, ShipID );
		return;
	}
	
	SI32 DisEmbarkX, DisEmbarkY;
	if ( pclMap->FindEmptyAreaForCharLocate( GetKind(), GetX(), GetY(), 10, &DisEmbarkX, &DisEmbarkY, true ) )
	{
		OrderStop(GetID(), BY_COMPUTER);
		((cltCharServer*)this)->SetInitProtectSwitch();

		cltWarpInfo clWarpInfo(1, GetMapIndex(), true, false, DisEmbarkX, DisEmbarkY, -1, -1);
		SetWarpInfo(&clWarpInfo, 3463);
		//Warp();
	}
	else
	{
		// 근처에 하선 할 곳이 없는 상태에서 강제 하선 이라면... 경복궁으로 보내 버림
		if ( Force )
		{
			OrderStop(GetID(), BY_COMPUTER);
			((cltCharServer*)this)->SetInitProtectSwitch();
			
			if ( pclClient->siServiceArea == ConstServiceArea_China )
			{
				cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, true, false, 27, 87, -1, -1);
				SetWarpInfo( &clWarpInfo, 2465 );
			}
			else
			{
				if ( pclKI->siDefaultCountry == ConstServiceArea_China )
				{
					cltWarpInfo clWarpInfo(1, MAPINDEX_CHINABORDER, true, false, 17, 19, -1, -1);
					SetWarpInfo( &clWarpInfo, 2465 );
				}
				else
				{
					cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, true, false, 27, 87, -1, -1);
					SetWarpInfo( &clWarpInfo, 2465 );
				}
			}
			//Warp();
		}
		else
		{	
			// 현재 하선할 수 없는 위치임을 알려줘야지..
			return;
		}
	}
		
	pShipListObj->Del( pclCI->GetPersonID() );

	/*
	cltGameMsgResponse_ShipPersonDisEmbark clinfo( true, ShipID );
	cltMsg clMsg( GAMEMSG_RESPONSE_SHIPPERSONDISEMBARK,  sizeof( clinfo ), (char*)&clinfo );
	SendNetMsg( (char*)&clMsg );
	*/

	ShipPassengerDataInit();

	cltCharServer *pclChar = (cltCharServer *)this;
	pclChar->SendInnerStatusToNeighbour();	

	return;
}

// 하선 - 내 스스로 배에서 내리는 경우임
void cltCharCommon::QuitGameDisEmbarkShip( SI32 ShipID, bool Force )
{
	if ( ShipID < 1 )
	{
		return;
	}

	// 배에 타고 있지 않은 상태 또는 다른 배에서 하선하라는 요상한 경우.
	if ( clInnerStatus.siRideShipID < 1 || clInnerStatus.siRideShipID != ShipID )
	{
		return;
	}

	CShipListObj *pShipListObj = pclClient->pclShipListMgr->GetShipListObj( ShipID );

	// 배를 타고는 있는데, 배가 없는 아주 이상한 경우.
	if ( pShipListObj == NULL )
	{
		char* pText = GetTxtFromMgr(3811);
		pclClient->pclLog->FilePrint( "Config/Ship.txt", pText, ShipID );
		return;
	}
	
	SI32 villageunique = 0;
	if(villageunique<=0)
	{
		// 현재 위치한 지도의 부모 지도를 찾는다. 
		SI32 parentmap = pclClient->pclMapManager->GetParentMap( GetMapIndex() );
		if(parentmap == 0)parentmap = GetMapIndex();


		// 부모 마을의 야외지역 좌표를 찾는다. 
		SI32 x, y;
		if( parentmap != 0 &&
			pclClient->pclMapManager->pclGateManager->FindMainFieldPos(parentmap, &x, &y) == true )
		{
		}
		else
		{
			x	= pclCI->clBI.clLastMainMapPos.siX;
			y	= pclCI->clBI.clLastMainMapPos.siY;

		}

		// 가까운 마을을 찾는다.
		villageunique = pclClient->pclVillageManager->FindNearVillage(x, y);
	}
	
	if(villageunique >0)
	{
		
		SI32 mapindex	= pclClient->pclVillageManager->pclVillageInfo[villageunique]->siVillageUnique;
		
		SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[ mapindex ]->siMapUnique;
		SI32 centerx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
		SI32 centery = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

		OrderStop(siID, BY_COMPUTER);
		((cltCharServer*)this)->SetInitProtectSwitch();
		
		cltWarpInfo clWarpInfo(1, mapindex, true, false, centerx, centery, -1, -1);
		SetWarpInfo(&clWarpInfo, 3464);
		Warp();
	}
	else
	{
		if ( Force )
		{
			OrderStop(siID, BY_COMPUTER);
			((cltCharServer*)this)->SetInitProtectSwitch();

			if ( pclClient->siServiceArea == ConstServiceArea_China )
			{
				cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, true, false, 27, 87, -1, -1);
				SetWarpInfo( &clWarpInfo, 2465 );
				Warp();
			}
			else
			{
				if ( pclKI->siDefaultCountry == ConstServiceArea_China )
				{
					cltWarpInfo clWarpInfo(1, MAPINDEX_CHINABORDER, true, false, 17, 19, -1, -1);
					SetWarpInfo( &clWarpInfo, 2465 );
					Warp();
				}
				else
				{
					cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, true, false, 27, 87, -1, -1);
					SetWarpInfo( &clWarpInfo, 2465 );
					Warp();
				}
			}
		}
		else
		{	
			// 현재 하선할 수 없는 위치임을 알려줘야지..
			return;
		}
	}

	pShipListObj->Del( pclCI->GetPersonID() );	

	/*
	cltGameMsgResponse_ShipPersonDisEmbark clinfo( true, ShipID );
	cltMsg clMsg( GAMEMSG_RESPONSE_SHIPPERSONDISEMBARK,  sizeof( clinfo ), (char*)&clinfo );
	SendNetMsg( (char*)&clMsg );
	*/

	ShipPassengerDataInit();

	cltCharServer *pclChar = (cltCharServer *)this;
	pclChar->SendInnerStatusToNeighbour();	

	return;
}


// 선박의 위치가 변경되었다. 
void cltCharCommon::ShipMoveAction()
{
	// 서버여야 한다. 
	if(pclClient->GameMode != GAMEMODE_SERVER)return ;
	
	// 배가 아니면 리턴한다. 
	if(IsShip(GetCharUnique()) == false)return ;	
	

	if ( clInnerStatus.siSelfShipID > 0 )
	{	
		CShipListObj *pShipLibObj = pclClient->pclShipListMgr->GetShipListObj( clInnerStatus.siSelfShipID );
		
		if ( pShipLibObj == NULL )
		{
			pclClient->pclLog->FilePrint("Config/Ship.txt", "ShipMoveAction pShipListObj == NULL, clInnerStatus.siSelfShipID = %d", clInnerStatus.siSelfShipID );
			return;
		}

		pShipLibObj->ShipAndPersonMove( GetX(), GetY() );
	}
}

// 내 기술과 손재주로 배를 만든다.
bool cltCharCommon::MakeShip( cltShip *pclShip )
{
	if ( pclShip->siKind < 1 )
	{
		return false;
	}

	// 기술은 적용하지 않았음.
	// 행운도 적용되지 않았음.


	
	SI32 MakeShipHnd = pclCI->clIP.GetHnd();

	// 손재주에 따른 배 건조 제한
	SI16 Ref = pclClient->pclShipManager->GetRefFromShipKind( pclShip->siKind );
	if ( Ref < 0 )
	{
		return false;
	}

	// 손재주 부족으로 배 제조 불가능
	if ( pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedHnd > MakeShipHnd )
	{
		return false;
	}

	// 최대 손재주는 500 이고 500일때 25 % 의 성능 샹상이 가능하다.
	float MaxIncresePercent = 0.25f;
	float MaxHnd = 500.0f;


	//float IncreasePercent = MaxIncresePercent * MakeShipHnd / MaxHnd;

	// 우선은 손재주에 따른 성능 향상 막아 놨음. ( 윗줄 풀면 바로 됨 )
	float IncreasePercent = 0.f;

	pclShip->siAP += (SI32)(pclShip->siAP * IncreasePercent);
	pclShip->siAC += (SI32)(pclShip->siAC * IncreasePercent);
	pclShip->siMoveSpeed += (SI32)(pclShip->siMoveSpeed * IncreasePercent);
	//pclShip->siMaxDurability += (SI32)( ( pclShip->siMaxDurability * 4 ) * IncreasePercent);
	pclShip->siDecreaseDurability = 0;
	pclShip->siDurability = pclShip->siDurability;

	// 속도는 최대 20 이다.
	if ( pclShip->siMoveSpeed > 20 )
	{
		pclShip->siMoveSpeed = 20;
	}

	// 내구도은 최대 10000 이다.
	if ( pclShip->siDurability > 30000 )
	{
		pclShip->siDurability = 30000;
	}

	return true;
}

// 내 기술과 손재주로 어떤 배가 만들어 질지에 대한 설명을 받아옮( 클라이언트용임. )
bool cltCharCommon::GetShipPerformance( cltShip *pShip, char *ShipText )
{
	if ( pShip == NULL || pShip->siShipID < 1 || ShipText == NULL )
	{
		return false;
	}

	return false;
}

