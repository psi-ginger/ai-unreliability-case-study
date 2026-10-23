#ifndef _PRIVATECHATMGR_H_
#define _PRIVATECHATMGR_H_

#include "PrivateChatBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"

class CPrivateChatMgr : public CLibListMgr
{
public:
	CPrivateChatMgr();
	~CPrivateChatMgr();

	void Initialize();
	void Destroy();

private:

};

#endif