#pragma once

#include "../common/CommonHeader.h"
#include "ValentineStore.h"
#include "ValentineBuffEventManager.h"
#include "../common/DailyEventTimer/DailyEventTimer.h"

class NUpdateTimer;
class CValentineStore;
class CValentineBuffEventManager;
class CDailyEventTimer;

class CValentineEventProcess
{
public:
	CValentineEventProcess();
	~CValentineEventProcess();

	void Create(CValentineBuffEventManager &pclValentineBuffEventManager, NkVector<CValentineStore> &pclValentineStoreList);
	void Action(bool &bHolding, bool &bNPCBuyPresentItem);

	void EnforseOperation();
protected:
	static const SI32 EVENTTIME = 1000 * 60 * 60;

	CValentineBuffEventManager *m_pclValentineBuffEventManager;
	NkVector<CValentineStore> *m_pclValentineStoreList;

	CDailyEventTimer*	m_pclEventStart;
	NUpdateTimer		m_clTimerEnd;

	bool				m_bEnforseOperation;

	void HoldEvent();
	void CloseEvent();

	void SendMsgHoldEvent();
	void SendMsgCloseEvent();

	void OpenStores();
	void CloseStores();
private:
	
};