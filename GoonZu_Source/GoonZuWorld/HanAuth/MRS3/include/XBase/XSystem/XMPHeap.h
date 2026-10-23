//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XMPHeap__
#define __XMPHeap__

#include "XSystemBase.h"
#include <XBase/XPlatform/XPlatformConfig.h>
#include <cstddef>

namespace XSystem
{
    namespace MPHeap
    {
        typedef struct _THandle
        {
        } *Handle, *THandle;

        XSYSTEM_API Handle __stdcall CreateHandle(size_t numberOfConcurrentThreads);
        XSYSTEM_API void __stdcall DestroyHandle(Handle heap);

        XSYSTEM_API void * __stdcall Alloc(Handle heap, size_t size);
        XSYSTEM_API void __stdcall Free(Handle heap, void *ptr);
        XSYSTEM_API void * __stdcall Realloc(Handle heap, void *ptr, size_t newSize);
        XSYSTEM_API size_t __stdcall GetSize(Handle heap, const void *ptr);
    }
}

#endif
