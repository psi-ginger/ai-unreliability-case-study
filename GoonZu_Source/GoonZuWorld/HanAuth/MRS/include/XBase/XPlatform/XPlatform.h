//
// XPlatform
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XPlatform__
#define __XPlatform__

#include "XPlatformBase.h"
#include "XPlatformConfig.h"

namespace XPlatform
{
#if defined(_WIN32) || defined(_WIN64)
    XPLATFORM_API bool __stdcall IsWindows2000(void);
#endif
    XPLATFORM_API unsigned int __stdcall GetProcessorCount(void);

    XPLATFORM_API void __stdcall SetLastError(unsigned int errorCode);
    XPLATFORM_API unsigned int __stdcall GetLastError(void);
}

#endif
