#include "ValentineEventProcess.h"
#include "../msg/msgtype-valentineevent.h"
#include "../MsgType.h"
#include "../common/Char/CharManager/CharManager.h"

CValentineEventProcess::CValentineEventProcess()
{
	m_pclValentineBuffEventManager = NULL;
	m_pclValentineStoreList = NULL;
	m_pclEventStart = NULL;
	m_bEnforseOperation = false;
}
CValentineEventProcess::~CValentineEventProcess()
{
	if (NULL != m_pclValentineBuffEventManager)
	{
		m_pclValentineBuffEventManager = NULL;
	}
	if (NULL != m_pclValentineStoreList)
	{
		m_pclValentineStoreList = NULL;
	}
	if (NULL != m_pclEventStart)
	{
		delete m_pclEventStart;
		m_pclEventStart = NULL;
	}
}

void CValentineEventProcess::Create(CValentineBuffEventManager &pclValentineBuffEventManager, NkVector<CValentineStore> &pclValentineStoreList)
{
	m_pclValentineBuffEventManager = &pclValentineBuffEventManager;
	m_pclValentineStoreList = &pclValentineStoreList;

	const SI32 START_MINITUE = 0;

	m_pclEventStart = new CDailyEventTimer();
	if( m_pclEventStart == NULL)
		return;

	m_pclEventStart->AddTime(EVENT_4_OCLOCK | EVENT_8_OCLOCK | EVENT_12_OCLOCK | EVENT_16_OCLOCK |
		EVENT_20_OCLOCK | EVENT_0_OCLOCK);
	m_pclEventStart->Execute( START_MINITUE,  NULL);
}

void CValentineEventProcess::EnforseOperation()
{
	m_bEnforseOperation = true;
}

void CValentineEventProcess::Action(bool &bHolding, bool &bNPCBuyPresentItem)
{
	if( NULL == m_pclValentineBuffEventManager)
		return;
	if( NULL == m_pclValentineStoreList)
		return;
	if( NULL == m_pclEventStart)
		return;

	if( (TRUE == m_pclEventStart->IsTimed() && bHolding == false) || ( bHolding == false && m_bEnforseOperation == true))
	{
		bHolding = true;
		HoldEvent();
	
		m_clTimerEnd.Init(EVENTTIME);
		m_clTimerEnd.SetActive(true, GetTickCount());
		m_bEnforseOperation = false;
	}

	if( (TRUE == m_clTimerEnd.IsTimed(GetTickCount(),false) && bHolding == true) || ( bHolding == true && m_bEnforseOperation == true))
	{
		bHolding = false;
		CloseEvent();

		m_clTimerEnd.Init(0);
		m_bEnforseOperation = false;

		bNPCBuyPresentItem = true;
	}
}

void CValentineEventProcess::HoldEvent()
{
	if( NULL == m_pclValentineBuffEventManager)
		return;

	OpenStores();
	m_pclValentineBuffEventManager->Init();


	SendMsgHoldEvent();
}
void CValentineEventProcess::CloseEvent()
{
	if( NULL == m_pclValentineBuffEventManager)
		return;

	CloseStores();
	m_pclValentineBuffEventManager->RewardBuff();
	if(m_pclValentineBuffEventManager->IsBufTime())
		m_pclValentineBuffEventManager->SendStartBuf();

	SendMsgCloseEvent();
}

void CValentineEventProcess::OpenStores()
{
	if( NULL == m_pclValentineStoreList)
		return;

	SI32 siMaxStoreCount = m_pclValentineStoreList->GetCount();
	for(SI32 siStoreListIndex = 0; siStoreListIndex < siMaxStoreCount; siStoreListIndex++)
	{
		CValentineStore& rclValentineStore = m_pclValentineStoreList->GetData(siStoreListIndex);

		if( !rclValentineStore.IsCreate())
			break;
		rclValentineStore.Open();
	}
}
void CValentineEventProcess::CloseStores()
{
	if(NULL == m_pclValentineStoreList)
		return;

	SI32 siMaxStoreCount = m_pclValentineStoreList->GetCount();
	for(SI32 siStoreListIndex = 0; siStoreListIndex < siMaxStoreCount; siStoreListIndex++)
	{
		CValentineStore& rclValentineStore = m_pclValentineStoreList->GetData(siStoreListIndex);

		if( !rclValentineStore.IsCreate())
			break;
		rclValentineStore.Close();
	}
}

void CValentineEventProcess::SendMsgHoldEvent()
{
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_START, 0);
	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}
void CValentineEventProcess::SendMsgCloseEvent()
{
	if( m_pclValentineBuffEventManager == NULL)
		return;

	cltGameMsgResponse_ValentineEventEnd clSendMsg(m_pclValentineBuffEventManager->GetUniqueOfBestVillage(),m_pclValentineBuffEventManager->GetBestNPCKind());
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_END, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}
