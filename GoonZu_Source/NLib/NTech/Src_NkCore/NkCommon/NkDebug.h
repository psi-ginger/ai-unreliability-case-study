#ifndef _NKDEBUG_H
#define _NKDEBUG_H

#include <cassert>

inline bool NkDebugBreak() { __debugbreak(); return false; }

#define NkAssert(boolean_expresion)		assert(boolean_expresion)

#endif
