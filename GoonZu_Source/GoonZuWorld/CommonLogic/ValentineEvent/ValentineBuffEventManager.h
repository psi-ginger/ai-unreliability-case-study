#pragma once

#include "../common/CommonHeader.h"
#include "ValentineNPC.h"

class CValentineNPC;
class cltServer;

struct sValentineBuf
{
	SI32 m_siBufType;
	SI32 m_siBuyValue;

	void init()
	{
		m_siBufType = 0;
		m_siBuyValue = 0;
	}
};

class CValentineBuffEventManager
{
public:
	CValentineBuffEventManager();
	~CValentineBuffEventManager();

	void Action();

	void Create(NkVector<CValentineNPC> &rclValentineNPCList);

	void Init();

	void RewardBuff();
	void SetPresentItemByVillageUser(SI32 siVillageUnique, SI32 siPresentItemCount);
	SI32 GetAllBufIndex(){return m_siAllBufIndex;}
	SI32 GetVillageBufIndex(){return m_siVillageBufIndex;}
	SI32 GetUniqueOfBestVillage(){return m_siUniqueOfBestVillage;}
	SI32 GetBestNPCKind(){return m_siNPCKindHaveMaxReceivedItem;}
	bool IsBufTime(){return m_bBufTime;}
	void SendEndBuf();
	void SendStartBuf();
	void SendNewBufInfoToAllPerson();
	void SendNewBufInfoToPerson(cltCharServer *pclChar);
	bool ChangeBufStateToStaticBufORRandBuf();
protected:
	static const SI32 MINPRESENTCOUNT_CANGIVEBUFF = 20;
	static const SI32 MAX_BUFSIZE = 10;
	cltServer					*m_pclServer;

	NkVector<CValentineNPC>		*m_pclValentineNPCList;
	NkVector<sValentineBuf>		m_clValentineBufList;
	sValentineBuf				m_sValentineBufListForSendingOnNet[MAX_BUFSIZE];
	SI32						m_siNPCKindHaveMaxReceivedItem;
	SI32						m_siTmpMaxReceivedItemCount;
	SI32						m_siAllBufIndex;
	SI32						m_siVillageBufIndex;
	bool						m_bRandBuf;
	bool						m_bGMCommand;
	SI32						m_siUniqueOfBestVillage;
	bool						m_bBufTime;
	SI32						m_siBufOverTime;
	_SYSTEMTIME					m_LastTimeBufChanged;				

	void CreateValentineBufList();
	SI32 GetBestNPCIndex();
	void ChangeNPCBuf();
	SI32 DeterminUniqueOfBestVillage();
	void RewardAllUser(SI32 siNPCIndex);
	void RewardUserOfBestVillage();
private:

	void GetOffRandBuf(sValentineBuf &sValentineBufGetOff,NkVector<sValentineBuf>	 &clTmpValentineBufList);

};