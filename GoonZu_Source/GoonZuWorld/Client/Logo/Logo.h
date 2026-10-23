//---------------------------------
// 2003/7/25 ±èÅÂ°ï
//---------------------------------

#ifndef _LOGO_H
#define _LOGO_H
#include <TSpr.h>

#include <Directives.h>
#include <ddraw.h>

class cltLogo{
public:
	TSpr Spr;

	cltLogo();
	~cltLogo();

	void Draw(SI32 screenxsize, SI32 screenysize, SI32 rx, SI32 ry );
};

#endif