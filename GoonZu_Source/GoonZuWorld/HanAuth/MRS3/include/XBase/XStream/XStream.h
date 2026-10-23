//
// XStream
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XStream__
#define __XStream__

#include "XStreamBase.h"
#include <XBase/XSystem.h>
#include <XBase/XPlatform/XPlatformConfig.h>

namespace XStream
{
    typedef struct _THandle
    {
    } *Handle, *THandle;

    XSTREAM_API Handle __stdcall CreateHandle(void);
    XSTREAM_API void __stdcall DestroyHandle(Handle stream);

    XSTREAM_API int __stdcall Read(Handle stream, void *p, int count);
    XSTREAM_API int __stdcall Write(Handle stream, const void *p, int count);
    XSTREAM_API int __stdcall Peek(Handle stream, void *p, int count);
    XSTREAM_API int __stdcall RemoveLeft(Handle stream, int count);
    XSTREAM_API int __stdcall RemoveRight(Handle stream, int count);
    XSTREAM_API bool __stdcall Clear(Handle stream);
    XSTREAM_API int __stdcall GetLength(Handle stream);

    XSTREAM_API bool __stdcall Attach(Handle stream, XSystem::MemoryPool::XMemory ptr);
    XSTREAM_API XSystem::MemoryPool::XMemory __stdcall Detach(Handle stream);
    XSTREAM_API XSystem::MemoryPool::XMemory __stdcall GetXMemory(Handle stream);

    XSTREAM_API Handle __stdcall Clone(Handle stream);
}

#endif
