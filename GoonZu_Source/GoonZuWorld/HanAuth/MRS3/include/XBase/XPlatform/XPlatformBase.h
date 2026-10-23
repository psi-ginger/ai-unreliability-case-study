//
// XPlatform
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XPlatformBase__
#define __XPlatformBase__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef XPLATFORM_EXPORTS
        #define XPLATFORM_API __declspec(dllexport)
    #else
        #define XPLATFORM_API __declspec(dllimport)

        #ifndef _DEBUG
            #pragma comment(lib, "XPlatform.lib")
        #else
            #pragma comment(lib, "XPlatformD.lib")
        #endif
    #endif
#else
    #define XPLATFORM_API
#endif

#endif
