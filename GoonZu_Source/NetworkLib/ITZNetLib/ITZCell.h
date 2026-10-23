#ifndef _ITZCELL_H
#define _ITZCELL_H

#include "ITZObject.h"
#include "ITZList.h"
#include "ITZWorld.h"

class ITZPerson;
class ITZMap;

class ITZCell
{
public:
	ITZCell();
	~ITZCell();

	void				CreateCell( ITZWorld< ITZMap > *pWorld, ITZMap *pMap );
	void				SetPerson( ITZPerson *pPerson );
	void				DelPerson( ITZPerson *pPerson );

	void				SendPacket( sPacketHeader *pPacket );
	void				SendPacketExceptOne( ITZPerson *pExceptedPerson, sPacketHeader *pPacket );

	void				SendPacket( BYTE *pPacket );
	void				SendPacketExceptOne( ITZPerson *pExceptedPerson, BYTE *pPacket );
	
	void				SendPacketNearAll( sPacketHeader *pPacket );
	void				SendPacketNearAllExceptOne( ITZPerson *pExceptedPerson, sPacketHeader *pPacket );

	void				SendPacketNearAll( BYTE *pPacket );
	void				SendPacketNearAllExceptOne( ITZPerson *pExceptedPerson, BYTE *pPacket );

	// 현재 셀에 있는 플레이어숫자를 반환한다
	UI32				GetPlayerCount() { return m_uiPlayerCount; }

	// 현재 셀 주변 셀들중에 플레이어가 한명이라도 있으면 TRUE를 반환한다.
	BOOL				IsTherePlayerNearCells();


public:
	ITZList< ITZObjectPtr >		m_personPtrList;

	ITZWorld< ITZMap >*			m_pWorld;
	ITZMap*						m_pMap;

	ITZCell*					m_pNearCell[9];
	UI32						m_uiNearCellCount;

	sItem**						m_ppItem;
	UI32						m_uiItemSlotCount;

	UI32						m_uiPlayerCount;


};



#endif