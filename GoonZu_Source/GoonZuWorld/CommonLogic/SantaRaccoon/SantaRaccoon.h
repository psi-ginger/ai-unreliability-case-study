#pragma once 

#include <Directives.h>
#include "..\..\Common\CommonHeader.h"


#define MAX_SANTA_RACCOON_MAKE_POS	10

class cltSantaRaccoonManager
{
public:

	bool bStartEvent;

	SI16 m_siLastSantaCreatedTime;
	SI16 m_siLastSantaDeletedTime;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltCharPos, MAX_SANTA_RACCOON_MAKE_POS>		clPos;
#else
	cltCharPos clPos[MAX_SANTA_RACCOON_MAKE_POS];
#endif

	cltSantaRaccoonManager();

	void Action();

	void StartEvent();
	void EndEvent();

	SI32 GetSantaRaccoonKind();
};

