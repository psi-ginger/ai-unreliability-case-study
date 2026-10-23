//---------------------------------
// 2003/8/7 김태곤
//---------------------------------

#ifndef _ITEMMANAGERSERVER_H
#define _ITEMMANAGERSERVER_H

#include "..\ItemCommon\cltItem.h"

class cltCharServer;
class CSetItemManager;

class cltItemManagerServer : public cltItemManagerCommon{
public:

	//CSetItemManager* pclSetItemManager;	

	cltItemManagerServer(SI32 gamemode);
	~cltItemManagerServer();

	// KindInfo의 아이템 정보를 근거로 랜덤한 아이템을 하나 만든다. 
	bool MakeRandItem(SI32 level, SI32 addrate, cltKindItemProbabilitySet* pclikinditem, cltItem* pclitem );

	// 해체시의 원료 리스트를 얻어온다. 
	bool GetDismantleMaterial( cltCharServer* pclchar, cltItem clitem, cltItem* pclitemlist, SI16 *pMakePercent );

//	SI32 GetEnchantDamageFromGrade(SI32 siGrade);
};


#endif