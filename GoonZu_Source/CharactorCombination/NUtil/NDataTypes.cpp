#include "ndatatypes.h"

const SI32  MIN_SI32 = (SI32)0xffffffff;	// (-2147483646);
const SI32  MAX_SI32 = (SI32)0x7fffffff;	// ( 2147483647);
const SI16  MIN_SI16 = (SI16)0xffff;		// (-32768);
const SI16  MAX_SI16 = (SI16)0x7fff;		// ( 32767);
const SI08  MIN_SI08 = (SI08)0xff;			// (-128);
const SI08  MAX_SI08 = (SI08)0x7f;			// ( 127);
const UI32	MIN_UI32 = (UI32)0;				// ( 0);
const UI32  MAX_UI32 = (UI32)0xffffffff;	// ( 65535);
const UI16  MIN_UI16 = (UI16)0;				// ( 0);
const UI16  MAX_UI16 = (UI16)0xffff;		// ( 4294967295);
const UI08  MIN_UI08 = (UI08)0;				// ( 0);
const UI08  MAX_UI08 = (UI08)0xff;			// ( 255);

const float	MAX_REAL32 = static_cast<float>(3.4E+38);
const float	MIN_REAL32 = static_cast<float>(-3.4E+38);
const float	TINY_REAL32 = static_cast<float>(3.4E-38);
