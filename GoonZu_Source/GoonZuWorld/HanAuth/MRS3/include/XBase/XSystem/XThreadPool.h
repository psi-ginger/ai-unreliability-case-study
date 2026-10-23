//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XThreadPool__
#define __XThreadPool__

#include "XSystemBase.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include <XBase/XPlatform/XPlatformConfig.h>
#endif
#include <cstddef>

namespace XSystem
{
    namespace ThreadPool
    {
        typedef struct _THandle
        {
        } *Handle, *THandle;

        typedef unsigned int (__stdcall *ThreadEvent)(void *context);
        typedef void (__stdcall *OverlappedCompletionEvent)(unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);

        XSYSTEM_API Handle __stdcall CreateHandle(size_t size);
        XSYSTEM_API void __stdcall DestroyHandle(Handle threadPool);

        XSYSTEM_API size_t __stdcall GetPoolSize(Handle threadPool);

#if defined(_WIN32) || defined(_WIN64)
        XSYSTEM_API bool __stdcall BindCompletionCallback(Handle threadPool, HANDLE handle, OverlappedCompletionEvent completionCallback);
#endif

        XSYSTEM_API bool __stdcall QueueWorkItem(Handle threadPool, ThreadEvent callback, void *context);
        XSYSTEM_API bool __stdcall QueueWorkItem(Handle threadPool, OverlappedCompletionEvent callback, unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);
    }
}

#endif
