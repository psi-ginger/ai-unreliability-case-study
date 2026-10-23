//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XTimer__
#define __XTimer__

#include "XSystemBase.h"
#include "XThreadPool.h"

namespace XSystem
{
    namespace Timers
    {
        namespace TimerQueue
        {
            typedef struct _THandle
            {
            } *Handle, *THandle;

            XSYSTEM_API Handle __stdcall CreateHandle(XSystem::ThreadPool::Handle threadPool);
            XSYSTEM_API void __stdcall DestroyHandle(Handle timerQueue);
        }

        namespace Timer
        {
            typedef void (__stdcall *Event)(void *context);

            typedef struct _Handle
            {
            } *Handle, *THandle;

            XSYSTEM_API Handle __stdcall CreateHandle(XSystem::Timers::TimerQueue::Handle timerQueue, unsigned int interval, Event callback, void *context);
            XSYSTEM_API void __stdcall DestroyHandle(Handle timer);
        }
    }
}

#endif
