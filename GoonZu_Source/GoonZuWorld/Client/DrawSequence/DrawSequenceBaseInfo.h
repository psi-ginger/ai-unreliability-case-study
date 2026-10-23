#ifndef _DRAWSEQUENCEBASEINFO_H_
#define _DRAWSEQUENCEBASEINFO_H_

#include <Directives.h>
#include "..\..\Common\Map\Map.h"

#define MAX_DRAW_DATA_SIZE									30
#define	MAX_DRAW_SEQUENCE_DATA_BUFFER_NUM					500		// 최대 500개를 버퍼에 담아 둘 수 있음.

enum { BEFORE_DRAW_SEQUENCE, LATER_DRAW_SEQUENCE, NOT_RELATIVE_SEQUENCE };

#endif