//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XThread__
#define __XThread__

#include "XSystemBase.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include <pthread.h>
#endif
#include <XBase/XPlatform/XPlatformConfig.h>

namespace XSystem
{
    namespace Threading
    {
        class XSYSTEM_API Thread
        {
        private:
            struct Impl;
            Impl *impl_;

        public:
            Thread(void);
            virtual ~Thread(void);

            bool Start(void);
            void Stop(void);

#if defined(_WIN32) || defined(_WIN64)
            bool Suspend(void);
            bool Resume(void);
#endif
            void Terminate(void);
#if defined(_WIN32) || defined(_WIN64)
            bool WaitFor(unsigned int timeout = INFINITE);
#else
            bool WaitFor(void);
#endif

            void SetFreeOnTerminate(bool value);
            bool GetFreeOnTerminate(void) const;
            void SetFinished(bool value);
            bool GetFinished(void) const;
#if defined(_WIN32) || defined(_WIN64)
            bool SetPriority(int value);
            int GetPriority(void);
            void SetSuspended(bool value);
            bool GetSuspended(void) const;
#endif

#if defined(_WIN32) || defined(_WIN64)
            HANDLE GetHandle(void) const;
            unsigned int GetThreadID(void) const;
#else
            pthread_t GetHandle(void) const;
#endif

        protected:
            bool Terminated(void) const;
#if defined(_WIN32) || defined(_WIN64)
            void SetThreadName(const char *name);
#endif
            void SetReturnValue(int value);
            int GetReturnValue(void) const;

            virtual void Execute(void) = 0;

        private:
            static unsigned int __stdcall InternalThreadProc(Thread *thread);
        };

#if defined(_WIN32) || defined(_WIN64)
        XSYSTEM_API bool __stdcall SetThreadName(unsigned int threadID, const char *name);
#endif
    }
}

#endif
