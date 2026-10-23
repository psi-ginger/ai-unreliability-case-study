#ifndef __XSystemBase_Old__
#define __XSystemBase_Old__

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

#endif
