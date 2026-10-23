#ifndef __Thread_Old__
#define __Thread_Old__

#include "XSystemBase.h"
#include <Windows.h>

class XSYSTEM_API XThread
{
public:
    XThread(void);
    virtual ~XThread(void);

    void Create(bool bCreateSuspended = true);
    void Destroy(void);

    bool Suspend(void);
    bool Resume(void);
    void Terminate(void);
    unsigned int WaitFor(DWORD dwTimeout = INFINITE, bool bAlertable = false);

    void SetFreeOnTerminate(bool bFreeOnTerminate);
    bool GetFreeOnTerminate(void) const;
    void SetFinished(bool bFinished);
    bool GetFinished(void) const;
    bool SetPriority(int nPriority);
    int GetPriority(void);
    void SetSuspended(bool bSuspended);
    bool GetSuspended(void) const;
    bool Join();
    HANDLE GetHandle(void) const;
    operator HANDLE();

    unsigned int GetThreadID(void) const;

protected:
    bool Terminated(void) const;
    void SetThreadName(const char *szThreadName);
    void SetReturnValue(int nReturnValue);
    int GetReturnValue(void) const;

    virtual void Execute(void) = 0;

private:
    static unsigned int __stdcall InternalThreadProc(XThread *pThread);

private:
	struct CONTEXT;
	CONTEXT* m_pContext;
};

XSYSTEM_API BOOL __stdcall XSetThreadNameA(DWORD dwThreadID, const char *szThreadName);
XSYSTEM_API BOOL __stdcall XSetThreadNameW(DWORD dwThreadID, const wchar_t *szThreadName);
#if !defined(UNICODE) && !defined(_UNICODE)
    #define XSetThreadName XSetThreadNameA
#else
    #define XSetThreadName XSetThreadNameW
#endif

#endif
