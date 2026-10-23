#ifndef __XTimer_Old__
#define __XTimer_Old__

#include "XSystemBase.h"
#include <Windows.h>

typedef void (__stdcall *TWAITORTIMERCALLBACK)(void *, BOOLEAN);

XSYSTEM_API HANDLE __stdcall XCreateTimerQueueTimer(TWAITORTIMERCALLBACK pfnCallback, PVOID pParameter, DWORD dwInterval, BOOL bRepeat = TRUE, ULONG flags = 0);
XSYSTEM_API BOOL __stdcall XDestroyTimerQueueTimer(HANDLE hTimer, HANDLE hCompletionEvent = INVALID_HANDLE_VALUE);
XSYSTEM_API BOOL __stdcall XChangeTimerQueueTimer(HANDLE hTimer, ULONG dueTime, ULONG period);

class IXTimer;
typedef void (*TTimerEvent)(IXTimer *, void *);

class IXTimer
{
protected:
    virtual ~IXTimer(void) = 0;

public:
    virtual BOOL Start(void) = 0;
    virtual BOOL Stop(void) = 0;

    virtual void SetInterval(int nInterval) = 0;
    virtual int GetInterval(void) const = 0;

    virtual void SetRepeat(BOOL bRepeat) = 0;
    virtual BOOL GetRepeat(void) const = 0;

    virtual void SetOnTimer(const TTimerEvent &OnTimer, void *pData) = 0;
};

XSYSTEM_API IXTimer * __stdcall XCreateTimer(void);
XSYSTEM_API void __stdcall XDestroyTimer(IXTimer *pTimer);

#endif //__XTimer__
