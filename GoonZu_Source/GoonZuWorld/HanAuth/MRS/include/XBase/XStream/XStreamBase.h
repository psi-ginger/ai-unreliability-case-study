//
// XStream
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XStreamBase__
#define __XStreamBase__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef XSTREAM_EXPORTS
        #define XSTREAM_API __declspec(dllexport)
    #else
        #define XSTREAM_API __declspec(dllimport)

        #ifndef _DEBUG
            #pragma comment(lib, "XStream.lib")
        #else
            #pragma comment(lib, "XStreamD.lib")
        #endif
    #endif
#else
    #define XSTREAM_API
#endif

#endif
