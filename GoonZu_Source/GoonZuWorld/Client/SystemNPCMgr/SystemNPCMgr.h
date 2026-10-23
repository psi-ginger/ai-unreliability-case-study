#ifndef _SYSTEMNPCMGR_H_
#define _SYSTEMNPCMGR_H_

#include "SystemNPCBaseInfo.h"

class CSystemNPCMgr : public CLibListMgr
{
public:
	CSystemNPCMgr();
	~CSystemNPCMgr();

	void	Initialize();
	void	Destroy();

private:

};

#endif