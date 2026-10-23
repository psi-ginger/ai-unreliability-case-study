#include "..\..\Common\CommonHeader.h"
#include "..\Server.h"

#include "MinisterBase.h"
#include "MinisterMgr.h"

#include "Minister-Byungzo.h"
#include "Minister-Yezo.h"
#include "Minister-Hozo.h"
#include "Minister-Goonzu.h"
#include "Minister-Izo.h"
#include "Minister-Gongzo.h"
#include "Minister-Hyungzo.h"

extern cltCommonLogic* pclClient;

CMinisterMgr::CMinisterMgr()
{
#ifdef _SAFE_MEMORY
	m_MinisterBase.ZeroMem();
#else
	memset(m_MinisterBase, 0, sizeof(m_MinisterBase));
#endif

	Initialize();
}

CMinisterMgr::~CMinisterMgr()
{

	Destroy();

}

void CMinisterMgr::Initialize()
{

	m_MinisterBase[MINISTER_BYUNGZO]	= new CMinisterByungzo();
	m_MinisterBase[MINISTER_YEZO]		= new CMinisterYezo();
	m_MinisterBase[MINISTER_HOZO]		= new CMinisterHozo();
	m_MinisterBase[MINISTER_GOONZU]		= new CMinisterGoonzu();
	m_MinisterBase[MINISTER_IZO]		= new CMinisterIzo();
	m_MinisterBase[MINISTER_GONGZO]		= new CMinisterGongzo();
	m_MinisterBase[MINISTER_HYUNGZO]	= new CMinisterHyungzo();
}

void CMinisterMgr::Destroy()
{
	for(SI32  i= 0; i < MINISTER_TOTAL; ++i)
	{
		if(m_MinisterBase[i])
		{
			delete m_MinisterBase[i];
			m_MinisterBase[i] = NULL;
		}
	}
}

void CMinisterMgr::InitializeDB()
{

	for(SI32  i= 0; i < MINISTER_TOTAL; ++i)
	{
		if(m_MinisterBase[i])
		{
			m_MinisterBase[i]->InitializeDB();
		}
	}
}


CMinisterBase *CMinisterMgr::GetMinister(SI32 siKind)
{
//	if(siKind <MINISTER_TYPE_START || MINISTER_TYPE_START>MINISTER_LAST) return NULL;

	if(siKind <MINISTER_TYPE_START ) return NULL;

	if(m_MinisterBase[siKind])	return m_MinisterBase[siKind];

	return NULL;
}



void	CMinisterMgr::DoMsg_DBMsgResponse_GetMinisterInfo(SI32 siKind, sPacketHeader* pPacket)
{
	CMinisterBase *pMinister = GetMinister(siKind);

	if(pMinister)
	{
		pMinister->DoMsg_DBMsgResponse_GetMinisterInfo(pPacket);
	}
}

void	CMinisterMgr::DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID, SI32 siKind)
{
	CMinisterBase *pMinister = GetMinister(siKind);

	if(pMinister)
	{
		pMinister->DoMsg_GameMsgRequest_GetMinisterInfo(siID);
	}
}

void	CMinisterMgr::DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID, SI32 siKind)
{
	CMinisterBase *pMinister = GetMinister(siKind);
	if(pMinister)
	{
		pMinister->DoMsg_GameMsgRequest_SetMinisterInfo(pclMsg, siID);
	}

}

void    CMinisterMgr::DoMsg_DBMsgResponse_SetMinisterInfo(SI32 siKind, sPacketHeader* pPacket)
{
	CMinisterBase *pMinister = GetMinister(siKind);
	if(pMinister)
	{
		pMinister->DoMsg_DBMsgResponse_SetMinisterInfo(pPacket);
	}

}