#ifndef _CLTTIME_H
#define _CLTTIME_H

#include <Directives.h>

#include "../../common/CommonHeader.h"

class cltTime{
protected:
	bool bDayChangedSwitch;			// 날짜가 변경되었는지 여부. 

public:

	cltDate clDate;

	cltTime();
	BOOL Action(bool bforcetimeswitch);
	
	// 날짜가 변경되었는가 ?
	bool IsDayChanged(){return bDayChangedSwitch;}

};

#endif
