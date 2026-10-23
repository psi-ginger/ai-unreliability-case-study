#ifndef _PRIVATECHATOBJ_H_
#define _PRIVATECHATOBJ_H_

#include "PrivateChatBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"

class CPrivateChatObj : public CObj
{
public:
	CPrivateChatObj();
	~CPrivateChatObj();

	void Initialize();
	void Destroy();

private:


};

#endif