#ifndef __XSignals_Old__
#define __XSignals_Old__

#include "XSystemBase.h"
#include <Windows.h>

XSYSTEM_API HANDLE __stdcall XCreateSignals(void);
XSYSTEM_API BOOL __stdcall XDestroySignals(HANDLE hSignals);

XSYSTEM_API BOOL __stdcall XAddSignal(HANDLE hSignals, DWORD dwSigNo);
XSYSTEM_API BOOL __stdcall XRemoveSignal(HANDLE hSignals, DWORD dwSigNo);

XSYSTEM_API BOOL __stdcall XSetSignal(HANDLE hSignals, DWORD dwSigNo, void *pData);
XSYSTEM_API DWORD __stdcall XWaitSignal(HANDLE hSignals, DWORD dwSigNo, void **ppData, DWORD dwMilliseconds);


#endif
