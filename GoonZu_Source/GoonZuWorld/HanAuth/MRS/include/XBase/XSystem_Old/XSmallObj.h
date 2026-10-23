// XSmallObj.h

#ifndef __XSmallObject_Old__
#define __XSmallObject_Old__

#include "XSystemBase.h"
#include <cstddef>

////////////////////////////////////////////////////////////////////////////////
// class XSmallObject
// Base class for polymorphic small objects, offers fast
//     allocations/deallocations
////////////////////////////////////////////////////////////////////////////////
//#if defined(new) && defined(DEBUG_NEW)
//	#define DEFINED_NEW_MACRO
//	#undef new
//#endif

#if (_MSC_VER < 1301)
    #pragma warning(disable:4291)
#endif

class XSYSTEM_API XSmallObject
{
public:
	virtual ~XSmallObject();

	static void *operator new(size_t size);
    static void operator delete(void *p, size_t size);
};

//#ifdef DEFINED_NEW_MACRO
//	#define new DEBUG_NEW
//#endif

#endif // __XSmallObject__
