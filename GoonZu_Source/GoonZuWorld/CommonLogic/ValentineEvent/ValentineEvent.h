#pragma once

#include "../common/CommonHeader.h"
#include "ValentineStore.h"
#include "ValentineNPC.h"
#include "ValentineBuffEventManager.h"
#include "ValentineEventProcess.h"

enum
{
	EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE = 1,
	EVALENTINEEVENT_GMCOMMAND_RETURN_HOLDON,
	EVALENTINEEVENT_GMCOMMAND_RETURN_CLOSE,
	EVALENTINEEVENT_GMCOMMAND_RETURN_BUF_STATIC,
	EVALENTINEEVENT_GMCOMMAND_RETURN_BUF_RAND
};

class CValentineNPC;
class CValentineBuffEventManager;
class CValentineEventProcess;
class CValentineStore;
class cltServer;
struct sValentineBuf;

class CValentineEventMgr
{
public:
	CValentineEventMgr();
	virtual ~CValentineEventMgr();

	static const UI32 INIT_NPCCOUNT = 10;

	virtual void Create();
	virtual void Destroy();

	bool	IsBufTime();
	bool	IsHolding();
	bool	IsEventPeriod();
	bool	IsValentineNPC(SI32 siCharKind);
	void	GetValentineBuf(cltCharCommon* pclChar, cltBasicAbility &clValentineAbility);

	CValentineNPC	*GetValentineNPC(SI32 siCharKind);
protected:
	static const UI32 INIT_STORECOUNT = 10;

	NkVector<CValentineNPC>		m_ValentineNPCList;

	SI32					m_siUniqueOfBestVillage;
	SI32					m_siAllBufIndex;
	SI32					m_siVillageBufIndex;
	bool					m_bBufTime;
	bool					m_bEventPeriod;
	bool					m_bHolding;
	cltCommonLogic*			m_pclCommon;

	void CreateValentineNPC();
	void GetBufAbility(SI32 siBufIndex, cltBasicAbility &clValentineAbility);
private:

};

class CValentineEventServerMgr : public CValentineEventMgr
{
public:
	CValentineEventServerMgr();
	virtual ~CValentineEventServerMgr();
	virtual void Create();
	virtual void Destroy();
	
	void Action();

	bool BuyPresentToStore(SI32 siCharUnique , SI32 siItemUnique, SI32 siItemCount = 1);
	CValentineStore* FindStoreByCharUnique(SI32 siCharUnique);
	void GivePresentItem(cltCharServer* pclChar,SI32 siNPCKind, cltItem &clItem, SI32 siInvenIndexOfPresentItem);
	void SendBufInfo(cltCharServer* pclChar);
	void SendNewBufInfo(cltCharServer* pclChar);
	BOOL OperationGMCommandEventState();
	BOOL OperationGMCommandBufState();
	bool OperationGMCommandBuyPresent(SI32 siNPCCharUnique,SI32 siItemUnique,SI32 siItemCount);
	SI32 GetRemainedItemCount(SI32 siNPCCharUnique,SI32 siItemUnique);
protected:
	NkVector<CValentineStore>	m_ValentineStoreList;
	SI32						m_siValentineStoreNum;
	CValentineBuffEventManager *m_pclValentineBuffEventManager;
	CValentineEventProcess *m_pclValentineEventProcess;
	
	bool		m_bCreatedStore;
	cltServer*	m_pclServer;
	bool		m_bNPCBuyPresentItem;

	void BuyPresentItemByNPC();
	void CreateValentineStore(CValentineStore &rclValentineStore, cltPos& rclPosition, SI32 siStoreNameTextUnqiue);
	void DestroyValentineStore(CValentineStore &rclValentineStore);
	void CheckEventPeriod();
	void GetBufInfo();
private:

};

class CValentineEventClientMgr : public CValentineEventMgr
{
public:
	CValentineEventClientMgr();
	virtual ~CValentineEventClientMgr();
	virtual void Create();
	virtual void Destroy();

	void ReceiveBuf(bool bBufTime,SI32 siAllBufIndex,SI32 siVillageBufIndex, SI32 siUniqueOfBestVillage);
	void ShowEmoticonWhenNpcReceivedItem(cltCharClient* pclNPCChar);
	void ShowPresentThatStoreSell(SI32 siStoreNameTextUnique);
	void ShowMsgStoreIsReadyForOpening(SI32 siStoreNameTextUnique);
	void ShowGivePresentMenu(cltCharClient *pclTargetNPC);
	void ClickNPC(SI32 siCharUnique);
	void HoldEvent();
	void CloseEvent(SI32 siNPCKind, SI32 siVillageUnique);
	void SetSeletedNPC(SI32 siCharKind);
	CValentineNPC *GetSeletedNPC();
	void GivePresentItem(cltItem &clItem, SI32 siInvenIndexOfPresentItem);
	void ChangedBufInfo(sValentineBuf *sValentineBufList);
protected:
	
	SI32		m_siSelectedCharKind;
	cltClient*	m_pclClient;
	
private:

};