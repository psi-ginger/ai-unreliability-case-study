#ifndef _KOINOBORY_H
#define _KOINOBORY_H

#include <Directives.h>
#include "..\..\Common\CommonHeader.h"


class cltKoinoboryManager
{
public:

	bool bStartEvent;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltCharPos, 6>		clPos;
#else
	cltCharPos clPos[6];
#endif

	cltKoinoboryManager();

	void Action();

	void StartEvent();
	void EndEvent();

	SI32 GetKoinoboryKind();
};


#endif