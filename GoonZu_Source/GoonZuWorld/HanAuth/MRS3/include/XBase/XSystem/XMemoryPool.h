//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XMemoryPool__
#define __XMemoryPool__

#include "XSystemBase.h"
#include <XBase/XPlatform/XPlatformConfig.h>
#include <cstddef>

namespace XSystem
{
    namespace MemoryPool
    {
        typedef struct _XMemory
        {
        } *XMemory;

        XSYSTEM_API XMemory __stdcall Alloc(size_t size);
        XSYSTEM_API void __stdcall Free(XMemory ptr);
        XSYSTEM_API XMemory __stdcall Realloc(XMemory ptr, size_t newSize);
        XSYSTEM_API size_t __stdcall GetSize(const XMemory ptr);
        XSYSTEM_API XMemory __stdcall Copy(const XMemory ptr);
        XSYSTEM_API XMemory __stdcall Clone(const XMemory ptr);

        class XSYSTEM_API XMemoryPtr
        {
        private:
            XMemory ptr_;

        public:
            XMemoryPtr(void);
            XMemoryPtr(size_t size);
            XMemoryPtr(const XMemory ptr);
            XMemoryPtr(const XMemoryPtr &rhs);
            ~XMemoryPtr(void);

            bool Attach(const XMemory ptr);
            XMemory Detach(void);

            void Clear(void);

            size_t GetSize(void) const;

            bool Realloc(size_t newSize);
            XMemoryPtr Clone(void);

            void *GetRawPtr(void) const;
            XMemory GetXMemory(void) const;

            XMemoryPtr &operator=(const XMemoryPtr &rhs);
        };
    }
}

#endif
