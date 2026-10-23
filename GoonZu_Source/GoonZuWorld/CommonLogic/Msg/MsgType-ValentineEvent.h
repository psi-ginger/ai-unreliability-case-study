#pragma once

#include "../CommonLogic/ValentineEvent/ValentineEvent.h"

class cltGameMsgResponse_ValentineEventChangeBuf
{
public:
	sValentineBuf m_sBuf[CValentineEventMgr::INIT_NPCCOUNT];

	cltGameMsgResponse_ValentineEventChangeBuf(sValentineBuf *sBuf)
	{
		memcpy(m_sBuf, sBuf, sizeof(sValentineBuf) * CValentineEventMgr::INIT_NPCCOUNT );
	}
}; //GAMEMSG_RESPONSE_VALENTINEEVENT_CHANGEBUF


class cltGameMsgResponse_ValentineEventBuf
{
public:
	bool m_bBufTime;
	SI32 m_siAllBufIndex;
	SI32 m_siVillageBufIndex;
	SI32 m_siUniqueOfBestVillage;

	cltGameMsgResponse_ValentineEventBuf(bool bBufTime, SI32 siAllBufIndex, SI32 siVillageBufIndex, SI32 siUniqueOfBestVillage)
	{
		m_bBufTime = bBufTime;
		m_siAllBufIndex = siAllBufIndex;
		m_siVillageBufIndex = siVillageBufIndex;
		m_siUniqueOfBestVillage = siUniqueOfBestVillage;
	}
}; //GAMEMSG_RESPONSE_VALENTINEEVENT_BUF

class cltGameMsgRequest_ValentineEventIsHolding
{
public:
	SI32 m_siCharUnique;

	cltGameMsgRequest_ValentineEventIsHolding(SI32 siCharUnique)
	{
		m_siCharUnique = siCharUnique;
	}
};	// GAMEMSG_REQUEST_VALENTINEEVENT_ISHOLDING


class cltGameMsgResponse_ValentineEventIsHolding
{
public:
	SI32 m_siCharUnique;
	bool m_bHolding;
	SI32 m_siStoreNameTextUnique;
	
	cltGameMsgResponse_ValentineEventIsHolding(SI32 siCharUnique,bool bHolding,SI32 siStoreNameTextUnique)
	{
		m_siCharUnique = siCharUnique;
		m_bHolding = bHolding;
		m_siStoreNameTextUnique = siStoreNameTextUnique;
	}
};	// GAMEMSG_RESPONSE_VALENTINEEVENT_ISHOLDING


class cltGameMsgRequest_ValentineEventGivePresentToNPC
{
public:
	SI32 m_siNPCCharKind;
	cltItem m_clPresentItem;
	SI32 m_siInvenIndexOfPresentItem;

	cltGameMsgRequest_ValentineEventGivePresentToNPC(SI32 siNPCCharKind, cltItem *clPresentItem,SI32 siInvenIndexOfPresentItem)
	{
		m_siNPCCharKind = siNPCCharKind;
		m_clPresentItem.Set(clPresentItem);
		m_siInvenIndexOfPresentItem = siInvenIndexOfPresentItem;
	}
};	// GAMEMSG_REQUEST_VALENTINEEVENT_GIVE_PRESENT_TONPC

class cltGameMsgResponse_ValentineEventGivePresentToNPC
{
public:
	SI32 m_siNPCCharKind;

	cltGameMsgResponse_ValentineEventGivePresentToNPC(SI32 siNPCCharKind)
	{
		m_siNPCCharKind = siNPCCharKind;
	}
};	// GAMEMSG_RESPONSE_VALENTINEEVENT_GIVE_PRESENT_TONPC

class cltGameMsgResponse_ValentineEventEnd
{
public:
	SI32 m_siVillageUnique;
	SI32 m_siPopularNPCCharKind;

	cltGameMsgResponse_ValentineEventEnd(SI32 siVillageUnique, SI32 siPopularNPCCharKind)
	{
		m_siVillageUnique = siVillageUnique;
		m_siPopularNPCCharKind = siPopularNPCCharKind;
	}
};	// GAMEMSG_RESPONSE_VALENTINEEVENT_END
