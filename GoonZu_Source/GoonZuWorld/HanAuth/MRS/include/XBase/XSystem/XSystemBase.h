//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XSystemBase__
#define __XSystemBase__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef XSYSTEM_EXPORTS
        #define XSYSTEM_API __declspec(dllexport)
    #else
        #define XSYSTEM_API __declspec(dllimport)

        #ifndef _DEBUG
            #pragma comment(lib, "XSystem.lib")
        #else
            #pragma comment(lib, "XSystemD.lib")
        #endif
    #endif
#else
    #define XSYSTEM_API
#endif

#endif
