#ifndef _FOREIGNMERCHANTOBJ_H_
#define _FOREIGNMERCHANTOBJ_H_

#include "ForeignMerchantBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"

class CForeignMerchantObj : public CObj
{
public:
	CForeignMerchantObj();
	~CForeignMerchantObj();

	void Initialize();
	void Destroy();

private:


};

#endif