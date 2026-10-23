//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XInterlocked__
#define __XInterlocked__

#include "XSystemBase.h"
#include <XBase/XPlatform/XPlatformConfig.h>

namespace XSystem
{
    namespace Threading
    {
        namespace Interlocked
        {
            XSYSTEM_API int __stdcall Increment(volatile int *value);
            XSYSTEM_API unsigned int __stdcall Increment(volatile unsigned int *value);

            XSYSTEM_API int __stdcall Decrement(volatile int *value);
            XSYSTEM_API unsigned int __stdcall Decrement(volatile unsigned *value);

            XSYSTEM_API int __stdcall Exchange(volatile int *dest, int value);
            XSYSTEM_API unsigned int __stdcall Exchange(volatile unsigned int *dest, unsigned int value);

            XSYSTEM_API int __stdcall CompareExchange(volatile int *dest, int value, int comperand);
            XSYSTEM_API unsigned int __stdcall CompareExchange(volatile unsigned int *dest, unsigned int value, unsigned int comperand);
        }
    }
}

#endif
