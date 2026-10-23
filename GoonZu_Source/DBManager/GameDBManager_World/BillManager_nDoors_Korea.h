#ifndef	__BILLMANAGER_FOR_KOREA_H__
#define __BILLMANAGER_FOR_KOREA_H__

#include "DBGameProtocol.h"

class CBillManager_nDoors_Korea
{
public:
	CBillManager_nDoors_Korea();
	~CBillManager_nDoors_Korea();

public:
	static SI32 GetUserCash(char*accountid,SI32*real,SI32*bonus,SI32*etc);
	static SI32 Get_Approve(char* accountid,SI32 *price,char*code, bool bGift);
	static SI32 ConfirmTX(char* accountid,SI32 price,char*code, bool bGift);
	static SI32 GetUsePkgCash(char*accountid,SI32*eventmoney);

};

#endif	//	__BILLMANAGER_FOR_KOREA_H__