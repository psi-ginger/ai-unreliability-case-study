#ifndef _FOREIGNMERCHANTMGR_H_
#define _FOREIGNMERCHANTMGR_H_

#include "ForeignMerchantBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"

class CForeignMerchantMgr : public CLibListMgr
{
public:
	CForeignMerchantMgr();
	~CForeignMerchantMgr();

	void Initialize();
	void Destroy();

private:

};

#endif