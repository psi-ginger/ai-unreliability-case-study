#include "ValentineBuffEventManager.h"
#include "../msg/msgtype-ValentineEvent.h"
#include "../Server/Server.h"

CValentineBuffEventManager::CValentineBuffEventManager()
{
	m_pclServer = (cltServer*)pclClient;

	m_pclValentineNPCList = NULL;
	m_bGMCommand = false;
	m_bBufTime = false;
	ZeroMemory(&m_LastTimeBufChanged, sizeof(_SYSTEMTIME));
	Init();
}

CValentineBuffEventManager::~CValentineBuffEventManager()
{

}

void CValentineBuffEventManager::Create(NkVector<CValentineNPC> &rclValentineNPCList)
{
	m_pclValentineNPCList = &rclValentineNPCList;
	Init();
	SI32 siCount = m_pclValentineNPCList->GetCount();

	m_clValentineBufList.Resize(siCount,siCount);

	CreateValentineBufList();
}


void CValentineBuffEventManager::ChangeNPCBuf()
{
	if( NULL == m_pclValentineNPCList)
		return;

	NkVector<sValentineBuf>		clTmpValentineBufList;

	clTmpValentineBufList.Assign(m_clValentineBufList);

	SI32 siMaxNPCCount = m_pclValentineNPCList->GetCount();

	sValentineBuf sValentineBufGetOff;
	for( SI32 siNPCListIndex = 0 ; siNPCListIndex < siMaxNPCCount; siNPCListIndex++)
	{
		CValentineNPC &rclNPC = m_pclValentineNPCList->GetData(siNPCListIndex);
		sValentineBufGetOff.init();
	
		GetOffRandBuf(sValentineBufGetOff, clTmpValentineBufList);
	
		rclNPC.SetBuffType(sValentineBufGetOff.m_siBufType);
		rclNPC.SetBuffValue(sValentineBufGetOff.m_siBuyValue);

		if( MAX_BUFSIZE > siNPCListIndex)
		{
			m_sValentineBufListForSendingOnNet[siNPCListIndex].m_siBufType = rclNPC.GetBuffType();
			m_sValentineBufListForSendingOnNet[siNPCListIndex].m_siBuyValue = rclNPC.GetBuffValue();
		}
	}	
}

bool CValentineBuffEventManager::ChangeBufStateToStaticBufORRandBuf()
{
	if( m_bRandBuf)
	{	
		m_bRandBuf = false;
	}
	else
		m_bRandBuf = true;

	m_bGMCommand = true;

	RewardUserOfBestVillage();
	SendStartBuf();
	return m_bRandBuf;
}

void CValentineBuffEventManager::GetOffRandBuf(sValentineBuf &sValentineBufGetOff,NkVector<sValentineBuf> &clTmpValentineBufList)
{
	SI32 siMaxTmpBufListSize = clTmpValentineBufList.GetCount();
	
	if( siMaxTmpBufListSize <= 0)
		return;

	if ( true ==  m_bRandBuf)
	{
		SI32 siGetOffIndex  = rand() % siMaxTmpBufListSize;
		
		sValentineBuf &sTmpValentineBuf = clTmpValentineBufList.GetData(siGetOffIndex);

		sValentineBufGetOff.m_siBufType = sTmpValentineBuf.m_siBufType;
		sValentineBufGetOff.m_siBuyValue = sTmpValentineBuf.m_siBuyValue;

		clTmpValentineBufList.SwapData(siGetOffIndex, siMaxTmpBufListSize-1);
		clTmpValentineBufList.PopBack();
	}
	else
	{
		clTmpValentineBufList.PopFront(sValentineBufGetOff);
	}
}

void CValentineBuffEventManager::Init()
{
	if(NULL == m_pclValentineNPCList)
		return;

	SI32 siMaxNPCCount = m_pclValentineNPCList->GetCount();

	for( SI32 siNPCListIndex = 0 ; siNPCListIndex < siMaxNPCCount; siNPCListIndex++)
	{
		CValentineNPC &rclNPC = m_pclValentineNPCList->GetData(siNPCListIndex);
		rclNPC.Init();
	}

	m_siNPCKindHaveMaxReceivedItem = 0;
	m_siTmpMaxReceivedItemCount = 0;
	m_siAllBufIndex = 0;
	m_siVillageBufIndex = 0;
	m_bRandBuf = true;
	m_siUniqueOfBestVillage = 0;
	m_siBufOverTime = 0;
}

void CValentineBuffEventManager::Action()
{	
	if( m_pclServer->pclTime->clDate.GetHourVary() >= m_siBufOverTime && m_bBufTime == true )
	{
		m_bBufTime = false;
		SendEndBuf();
	}

	_SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);

	if( (CurrentTime.wYear != m_LastTimeBufChanged.wYear 
		|| CurrentTime.wMonth != m_LastTimeBufChanged.wMonth
		|| CurrentTime.wDay != m_LastTimeBufChanged.wDay) || true == m_bGMCommand)
	{
		ChangeNPCBuf();
		memcpy(&m_LastTimeBufChanged, &CurrentTime, sizeof(_SYSTEMTIME));
		SendNewBufInfoToAllPerson();

		m_bGMCommand = false;
	}

}

void CValentineBuffEventManager::SendNewBufInfoToAllPerson()
{
	cltGameMsgResponse_ValentineEventChangeBuf clSendMsg((sValentineBuf*)&m_sValentineBufListForSendingOnNet);
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_CHANGEBUF, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	m_pclServer->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}

void CValentineBuffEventManager::SendNewBufInfoToPerson(cltCharServer *pclChar)
{
	if( NULL == pclChar)
		return;

	cltGameMsgResponse_ValentineEventChangeBuf clSendMsg((sValentineBuf*)&m_sValentineBufListForSendingOnNet);
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_CHANGEBUF, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void CValentineBuffEventManager::CreateValentineBufList()
{
	SI32 siMaxBufCount = m_clValentineBufList.GetCount();

	for( SI32 siBufListIndex = 0 ; siBufListIndex < siMaxBufCount; siBufListIndex++)
	{
		sValentineBuf &rclBuf = m_clValentineBufList.GetData(siBufListIndex);

		switch(siBufListIndex)
		{
		case 0:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_HAND;
				rclBuf.m_siBuyValue = 5;
				break;
			}
		case 1:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_MAG;
				rclBuf.m_siBuyValue = 10;
				break;
			}
		case 2:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_STR;
				rclBuf.m_siBuyValue = 10;
				break;
			}
		case 3:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_DEX;
				rclBuf.m_siBuyValue = 5;
				break;
			}
		case 4:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_STR;
				rclBuf.m_siBuyValue = 5;
				break;
			}
		case 5:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_MAG;
				rclBuf.m_siBuyValue = 5;
				break;
			}
		case 6:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_HAND;
				rclBuf.m_siBuyValue = 10;
				break;
			}
		case 7:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_VIT;
				rclBuf.m_siBuyValue = 5;
				break;
			}
		case 8:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_VIT;
				rclBuf.m_siBuyValue = 10;
				break;
			}
		case 9:
			{
				rclBuf.m_siBufType = BUF_STATUE_UP_DEX;
				rclBuf.m_siBuyValue = 10;
				break;
			}
		}
	}
}

void CValentineBuffEventManager::SendEndBuf()
{
	cltGameMsgResponse_ValentineEventBuf clSendMsg(false, 0, 0, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_BUF, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	m_pclServer->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}

void CValentineBuffEventManager::SendStartBuf()
{
	cltGameMsgResponse_ValentineEventBuf clSendMsg(true, m_siAllBufIndex, m_siVillageBufIndex, m_siUniqueOfBestVillage);
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_BUF, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	m_pclServer->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}

void CValentineBuffEventManager::RewardBuff()
{
	SI32 siNPCIndex = GetBestNPCIndex();

	if( m_siNPCKindHaveMaxReceivedItem <= 0 || siNPCIndex < 0)
	{
		return;
	}

	RewardAllUser(siNPCIndex);
	RewardUserOfBestVillage();

	DeterminUniqueOfBestVillage();

	m_siBufOverTime = m_pclServer->pclTime->clDate.GetHourVary() + 30;
	m_bBufTime = true;
}

SI32 CValentineBuffEventManager::GetBestNPCIndex()
{
	if(NULL == m_pclValentineNPCList)
		return -1;

	SI32 siNPCIndex = 0;
	m_siNPCKindHaveMaxReceivedItem = 0;
	m_siTmpMaxReceivedItemCount = 0;

	SI32 siMaxNPCCount = m_pclValentineNPCList->GetCount();

	for( SI32 siNPCListIndex = 0 ; siNPCListIndex < siMaxNPCCount; siNPCListIndex++)
	{
		CValentineNPC &rclNPC = m_pclValentineNPCList->GetData(siNPCListIndex);

		if( m_siTmpMaxReceivedItemCount < rclNPC.GetReceivedItemCount())
		{
			m_siTmpMaxReceivedItemCount = rclNPC.GetReceivedItemCount();
			m_siNPCKindHaveMaxReceivedItem = rclNPC.GetCharKind();
			siNPCIndex = siNPCListIndex;
		}
	}
	
	if( MINPRESENTCOUNT_CANGIVEBUFF > m_siTmpMaxReceivedItemCount)
		m_siNPCKindHaveMaxReceivedItem = 0;

	return siNPCIndex;
}

SI32 CValentineBuffEventManager::DeterminUniqueOfBestVillage()
{
	if(NULL == m_pclValentineNPCList)
		return 0;

	SI32 siNPCListCount = m_pclValentineNPCList->GetCount();

	if( m_siAllBufIndex >= siNPCListCount )
		return 0;

	CValentineNPC &rclNPC = m_pclValentineNPCList->GetData(m_siAllBufIndex);

	m_siUniqueOfBestVillage = rclNPC.GetUniqueOfBestVillage();

	return m_siUniqueOfBestVillage;
}
void CValentineBuffEventManager::RewardAllUser(SI32 siNPCIndex)
{
	if(NULL == m_pclValentineNPCList)
		return;

	m_siAllBufIndex = siNPCIndex;
}
void CValentineBuffEventManager::RewardUserOfBestVillage()
{
	if(NULL == m_pclValentineNPCList)
		return;

	SI32 siMaxNPCCount = m_pclValentineNPCList->GetCount();

	if( siMaxNPCCount == 0)
		return;

	if( true == m_bRandBuf)
	{
		
		m_siVillageBufIndex = rand() % siMaxNPCCount;
		if( m_siVillageBufIndex == m_siAllBufIndex)
		{
			m_siVillageBufIndex = m_siVillageBufIndex + 1;
			if( m_siVillageBufIndex == siMaxNPCCount)
				m_siVillageBufIndex = 0;
		}
	}
	else
	{
		m_siVillageBufIndex = m_siAllBufIndex + 1;
		if( m_siVillageBufIndex == siMaxNPCCount)
			m_siVillageBufIndex = 0;
	}
}