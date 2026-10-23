#include "ServerMap.h"

#include "ITZCell.h"
#include "ITZPerson.h"
#include "..\Server.h"
#include "MsgType.h"
#include "MsgType-Item.h"

#include "../../common/Char/CharServer/Char-Server.h"
#include "../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

cltServerMap::cltServerMap()
{
}

cltServerMap::~cltServerMap()
{
}


// 새로 추가된 셀 영역에 있는 유저들에게 pPerson의 등장을 알린다
void cltServerMap::SendPersonEnterMsgToOthers( ITZPerson *pPerson, bool bticketusewarpswitch )
{

	cltCharServer *pChar = (cltCharServer *)pPerson;
	
	// 캐릭터의 정보를 얻어온다. 
	//---------------------------------
	// 새로 들어온 놈은 그 전에 무엇을 했던지 간에 정지를 취해야 한다.
	//---------------------------------
	if(pChar->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_BOMBDRAGON") ) ) 
	{
		SI32 a = 0;
	}
	cltMakeCharInfo	clMakeCharInfo;
	if(pChar->GetMakeCharInfo(&clMakeCharInfo, true, bticketusewarpswitch) == false)return;

	cltMsg clMsg(GAMEMSG_RESPONSE_LOGIN, sizeof(clMakeCharInfo), (BYTE*)&clMakeCharInfo);

	for( UI32 i = 0; i < m_uiNewCellCount; ++i ) {
	
		m_pNewCell[ i ]->SendPacket( (sPacketHeader *)&clMsg );			// 해당 셀에 있는 유저들에게 패킷 전송
	
	}
	
}

// 삭제된 셀 영역에 있는 유저들에게 pPerson의 삭제를 알린다
void cltServerMap::SendPersonLeaveMsgToOthers( ITZPerson *pPerson )
{

	cltCharServer *pChar = (cltCharServer *)pPerson;

	cltMsg clMsg(GAMEMSG_RESPONSE_LOGOUT, pChar->GetCharUnique());
	
	for( UI32 i = 0; i < m_uiRemoveCellCount; ++i ) {
		
		m_pRemoveCell[ i ]->SendPacket( (sPacketHeader *)&clMsg );		// 해당 셀에 있는 유저들에게 패킷 전송
	}

}

// pPerson의 이동 정보를 가시영역에 있는 유저들에게 알린다 
void cltServerMap::SendPersonMoveMsgToOthers( ITZPerson *pPerson )
{
	//pPerson->GetCell()->SendPacketNearAll( );		// pPerson의 가시영역에 있는 유저들에게 이동 패킷 전송 
}


// 새로 추가된 셀 영역에 있는 캐릭터들의 정보를 pPerson에게 알린다 
void cltServerMap::SendNewOthersInfoToPerson( ITZPerson *pPerson )
{

	ITZObjectPtr *pItor;
	ITZPerson *pOtherPerson;
	cltCharServer *pChar;

	cltMsg clMsg(GAMEMSG_RESPONSE_LOGINMANY, 0, NULL);
	SI32 index = 0;

	if(pPerson->GetSession() == NULL)return ;

	for( UI32 i = 0; i < m_uiNewCellCount; ++i ) 
	{  

		LIST_LOOP( &m_pNewCell[ i ]->m_personPtrList, pItor )
		{
			pOtherPerson = (ITZPerson *)pItor->GetObject();

			pChar = (cltCharServer *)pOtherPerson;
			if(pChar == NULL)
			{
				pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("SendNewOthersInfoToPerson()"));
				continue;
			}

			//-------------------------------
			// 개별 캐릭터의 메시지를 만든다. 
			//--------------------------------
			cltMakeCharInfo	clMakeCharInfo;
			if(pChar->GetMakeCharInfo(&clMakeCharInfo, true, false) == false)continue;

			//-------------------------------
			// 개별 캐릭터의 메시지를 축적한다. 
			//--------------------------------
			if(clMakeCharInfo.uiType == MAKECHARTYPE_PC)
			{
				memcpy(&clMsg.cData[index], &clMakeCharInfo, sizeof(clMakeCharInfo));
				index += sizeof(clMakeCharInfo);
			}
			else 
			{
				memcpy(&clMsg.cData[index], &clMakeCharInfo, sizeof(cltMakeCharInfoCommon));
				index += sizeof(cltMakeCharInfoCommon);
			}

			//----------------------------------------
			// 최대 사이즈에 도달하면 메시지를 보낸다.
			//-----------------------------------------
			if(index + sizeof(clMakeCharInfo) >= MAX_CLTMSG_DATA_SIZE)
			{
				clMsg.clHead.usSize = sizeof(sPacketHeader) + index;

				cltCharServer* pclchar = pclClient->pclCM->GetCharServer( pPerson->GetCharID() );
				if( pclchar )
				{
					pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
				}
				index = 0;	// 다시 처음부터 복사한다. 
			}
		}

	}

	//-------------------------------------------
	// 보내지지 않은 메시지가 있다면 보낸다.
	//-------------------------------------------
	if(index)
	{
		clMsg.clHead.usSize = sizeof(sPacketHeader) + index;

		cltCharServer* pclchar = pclClient->pclCM->GetCharServer( pPerson->GetCharID() );
		if( pclchar )
		{
			pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		index = 0;	// 다시 처음부터 복사한다. 
	}

	


	for( i = 0; i < m_uiRemoveCellCount; ++i ) {

		LIST_LOOP( &m_pRemoveCell[ i ]->m_personPtrList, pItor )
		{
			pOtherPerson = (ITZPerson *)pItor->GetObject();

			pChar = (cltCharServer *)pOtherPerson;

			if( pChar == NULL ) continue;

			cltMsg clMsg(GAMEMSG_RESPONSE_LOGOUT, pChar->GetCharUnique());
			//pPerson->GetSendBuffer()->Write( (BYTE *)&clMsg );
			cltCharServer* pclchar = pclClient->pclCM->GetCharServer( pPerson->GetCharID() );
			if( pclchar )
			{
				pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
			}
		}
	}
}

// 새로 추가된 셀 영역에 있는 아이템들의 정보를 pPerson에게 알린다 
void cltServerMap::SendNewItemsInfoToPerson( ITZPerson *pPerson )
{
	UI32 i, j;
	sItem *pItem;

	cltServer* pclServer = (cltServer*)pclClient;

	if(pPerson->GetSession() == NULL)return ;

	// 새로 추가된 셀 개수 만큼 반복 
	for( i = 0; i < m_uiNewCellCount; ++i ) {  

		// 셀에 있는 아이템 슬롯 개수 만큼 반복 
		for( j = 0; j < m_pNewCell[ i ]->m_uiItemSlotCount; ++j ) {
			
			pItem = m_pNewCell[ i ]->m_ppItem[ j ];

			// 아이템 슬롯에 아이템이 있으면 그 정보를 pPerson에게 전송 
			if( pItem->itemIndex != 0 ) {

				cItemNode* pnode = pclServer->pclItemList->GetItem(pItem->itemIndex);
				if(pnode)
				{
					SI32 itemunique	= pnode->item.siUnique;
					SI32 x			= pnode->x;
					SI32 y			= pnode->y;
					SI32 mapindex	= pnode->mapIndex;

					cltCharPos clPos(mapindex, x, y);
					cltGameMsgResponse_ItemDrop clInfo(&clPos, itemunique, pItem->itemIndex);
					cltMsg clMsg(GAMEMSG_RESPONSE_ITEMDROP, sizeof(clInfo), (BYTE*)&clInfo);

					//pPerson->GetSendBuffer()->Write((BYTE*)&clMsg);
					cltCharServer* pclchar = pclClient->pclCM->GetCharServer( pPerson->GetCharID() );
					if( pclchar )
					{
						pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
					}
				}
			}
		}
	}

	// 삭제된 셀 개수 만큼 반복
	for( i = 0; i < m_uiRemoveCellCount; ++i ) {  

		// 셀에 있는 아이템 슬롯 개수 만큼 반복 
		for( j = 0; j < m_pRemoveCell[ i ]->m_uiItemSlotCount; ++j ) {
			
			pItem = m_pRemoveCell[ i ]->m_ppItem[ j ];

			// 아이템 슬롯에 아이템이 있으면 그 정보를 pPerson에게 전송 
			if( pItem->itemIndex != 0 ) {

				cItemNode* pnode = pclServer->pclItemList->GetItem(pItem->itemIndex);
				if(pnode)
				{
					SI32 itemunique	= 0;			// 아이템 삭제 목적. 
					SI32 x			= pnode->x;
					SI32 y			= pnode->y;
					SI32 mapindex	= pnode->mapIndex;

					cltCharPos clPos(mapindex, x, y);
					cltGameMsgResponse_ItemDrop clInfo(&clPos, itemunique, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_ITEMDROP, sizeof(clInfo), (BYTE*)&clInfo);

					//pPerson->GetSendBuffer()->Write((BYTE*)&clMsg);
					cltCharServer* pclchar = pclClient->pclCM->GetCharServer( pPerson->GetCharID() );
					if( pclchar )
					{
						pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
					}
				}
			}
		}
	}
}


void cltServerMap::SendAddItemMsgToOthers( sItem *pItem )
{
	ITZCell *pCell = GetCell( pItem->x, pItem->y );

	if( pItem->itemIndex == 0 )return ;

	if( pCell == NULL)return ;

	cltServer* pclServer = (cltServer*)pclClient;

	cItemNode* pnode = pclServer->pclItemList->GetItem(pItem->itemIndex);
	if(pnode)
	{
		SI32 itemunique	= pnode->item.siUnique;			
		SI32 x			= pnode->x;
		SI32 y			= pnode->y;
		SI32 mapindex	= pnode->mapIndex;
		
		cltCharPos clPos(mapindex, x, y);
		cltGameMsgResponse_ItemDrop clInfo(&clPos, itemunique, pItem->itemIndex);
		cltMsg clMsg(GAMEMSG_RESPONSE_ITEMDROP, sizeof(clInfo), (BYTE*)&clInfo);
	
		pCell->SendPacketNearAll( (sPacketHeader*)&clMsg);
		
	}
}


void cltServerMap::SendDelItemMsgToOthers( sItem *pItem )
{
	ITZCell *pCell = GetCell( pItem->x, pItem->y );

	if( pItem->itemIndex == 0 )return ;

	if( pCell == NULL)return ;

	cltServer* pclServer = (cltServer*)pclClient;

	cItemNode* pnode = pclServer->pclItemList->GetItem(pItem->itemIndex);
	if(pnode)
	{
		SI32 itemunique	= 0;			// 아이템 삭제 목적. 
		SI32 x			= pnode->x;
		SI32 y			= pnode->y;
		SI32 mapindex	= pnode->mapIndex;
		
		cltCharPos clPos(mapindex, x, y);
		cltGameMsgResponse_ItemDrop clInfo(&clPos, itemunique, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_ITEMDROP, sizeof(clInfo), (BYTE*)&clInfo);
	
		pCell->SendPacketNearAll( (sPacketHeader*)&clMsg);
	}
}
