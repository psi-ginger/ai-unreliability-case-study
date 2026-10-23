// XSyncObject.h
/*!
\file		XSyncObject.h
*/

#ifndef __XSyncObject_Old__
#define __XSyncObject_Old__

#include "XSystemBase.h"
#include <Windows.h>
#include <vector>

/********************************************************************************
	class XSyncObject
********************************************************************************/
/*!
\brief					synchronization objects의 공통 기능을 정의한 pure virtual class
\remarks				XCriticalSection, XMutex, XSemaphore, XEvent는 XSyncObject를 상속받아 구현되어 있다.

						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XCriticalSection, XMutex, XSemaphore, XEvent
*/
class XSYSTEM_API XSyncObject
{
public:
	/*! \brief 호출한 thread가 XSyncObject의 ownership을 얻기위해 대기한다. */
	virtual BOOL Lock(DWORD dwMilliseconds = INFINITE) = 0;

	/*! \brief 호출한 thread가 소유한 XSyncObject의 ownership을 releases 한다. */
	virtual BOOL Unlock() = 0;
};

/********************************************************************************
	class XCriticalSection
********************************************************************************/
/*!
\brief					CRITICAL_SECTION object에 대한 wrapper class

\remarks
						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XSyncObject, XMutex, XSemaphore, XEvent
*/
class XSYSTEM_API XCriticalSection : public XSyncObject
{
public:
	/*!
	\brief					defualt constructor
	\param dwSpinCount		[in] spin count
	*/
	explicit XCriticalSection(DWORD dwSpinCount = 4000);
	/*! \brief distructor */
	virtual ~XCriticalSection();

	/*! \brief 호출한 thread가 critical section object의 ownership을 얻기위해 대기한다. */
	void Enter();

	/*!
	\brief					호출한 thread가 critical section object의 ownership을 얻기위해 시도한다.
	
	\return					
							- TRUE: 호출한 thread가 critical section object의 ownership을 획득함.
									Leave 또는 Unlock를 호출하여 획득한 ownership을 releases해 주어야 한다.
							- FALSE: 호출한 thread가 critical section object의 ownership을 획득하지 못 함
	*/
	BOOL TryEnter();

	/*! \brief 호출한 thread가 소유한 critical section object의 ownership을 releases 한다. */
	void Leave();

	/*! \brief 내부 CRITICAL_SECTION 객체의 pointer를 반환한다. */
	operator CRITICAL_SECTION*();

	/*! \brief 내부 CRITICAL_SECTION 객체를 인자로 지정한 CRITICAL_SECTION object로 attach 한다. */
	void Attach(CRITICAL_SECTION* p);
	CRITICAL_SECTION Detach();

	/*! \brief Enter()를 호출한다. */
	virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);
	
	/*! \brief Leave()를 호출한다. */
	virtual BOOL Unlock();

protected:
	struct Context;
	Context* m_pContext;
};

/********************************************************************************
	class XWaitableObject
********************************************************************************/
/*!
\brief					SYNCHRONIZE 억세스 권한을 가지는 handle에 대한 공통 기능을 정의한 class

\remarks
						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XMutex, XSemaphore, XEvent, XWaitableCollection
*/
class XSYSTEM_API XWaitableObject
{
public:
	/*! \brief defualt constructor */
	XWaitableObject();

	/*! \brief distructor */
	virtual ~XWaitableObject();

	/*! \brief 내부 handle이 valid한지 검사한다. */
	BOOL IsOpen() const;

	/*! \brief 내부 handle을 반환한다. */
	HANDLE GetHandle() const;

	/*! \brief 내부 handle을 반환한다. */
	operator HANDLE() const;

	/*! \brief 내부 handle을 close 한다. */
	virtual void Close();

	/*!
	\brief					내부 handle이 signaled state가 될때까지 기다린다.
	\param dwMilliseconds	[in] Time-out interval, in milliseconds.
	\return					Win32 API의 WaitForSingleObject와 동일
	*/
	DWORD Wait(DWORD dwMilliseconds = INFINITE);

	/*!
	\brief					내부 handle이 signaled state가 될때까지 기다린다.
	\param bAlertable		[in] If this parameter is TRUE, the function returns when the system queues 
								an I/O completion routine or APC, and the thread runs the routine or function.
								If this parameter is FALSE, the function does not return, 
								and the completion routine or APC function is not executed.
	\param dwMilliseconds	[in] Time-out interval, in milliseconds.
	\return					Win32 API의 WaitForSingleObject와 동일
	*/
	DWORD WaitEx(BOOL bAlertable, DWORD dwMilliseconds = INFINITE);

	/*! \brief 내부 handle을 hObject로 attach 한다. */
	virtual void Attach(HANDLE hObject);

	/*! \brief 내부 handle을 detach 한다. */
	virtual HANDLE Detach();

protected:
	struct Context;
	Context* m_pContext;
};

/********************************************************************************
	class XMutex
********************************************************************************/
/*!
\brief					mutex object에 대한 wrapper class

\remarks
						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XSyncObject, XWaitableObject, XCriticalSection, XSemaphore, XEvent, XWaitableCollection
*/
class XSYSTEM_API XMutexA : public XWaitableObject, public XSyncObject
{
public:
	/*! \brief defualt constructor */
	XMutexA() {}

	/*! \brief distructor */
	virtual ~XMutexA() {}

	/*! \brief event object 생성 */
	BOOL Create(BOOL bInitialOwner = FALSE, const char *pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);

	/*! \brief 이미 생성된 event object를 open 한다. */
	BOOL Open(const char *pszName = NULL, DWORD dwDesiredAccess = MUTEX_ALL_ACCESS, BOOL bInheritHandle = FALSE);

	/*! \brief event object를 Releases 한다. */
	BOOL Release();

	/*! \brief Wait를 호출한다. */
	virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);

	/*! \brief Release를 호출한다. */
	virtual BOOL Unlock();
};

class XSYSTEM_API XMutexW : public XWaitableObject, public XSyncObject
{
public:
    /*! \brief defualt constructor */
    XMutexW() {}

    /*! \brief distructor */
    virtual ~XMutexW() {}

    /*! \brief event object 생성 */
    BOOL Create(BOOL bInitialOwner = FALSE, const wchar_t *pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);

    /*! \brief 이미 생성된 event object를 open 한다. */
    BOOL Open(const wchar_t *pszName = NULL, DWORD dwDesiredAccess = MUTEX_ALL_ACCESS, BOOL bInheritHandle = FALSE);

    /*! \brief event object를 Releases 한다. */
    BOOL Release();

    /*! \brief Wait를 호출한다. */
    virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);

    /*! \brief Release를 호출한다. */
    virtual BOOL Unlock();
};

#if !defined(UNICODE) && !defined(_UNICODE)
    typedef XMutexA XMutex;
#else
    typedef XMutexW XMutex;
#endif

/********************************************************************************
	class XSemaphore
********************************************************************************/
/*!
\brief					semaphore object에 대한 wrapper class

\remarks
						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XSyncObject, XWaitableObject, XCriticalSection, XMutex, XEvent, XWaitableCollection
*/
class XSYSTEM_API XSemaphoreA : public XWaitableObject, public XSyncObject
{
public:
	/*! \brief defualt constructor */
	XSemaphoreA() {}

	/*! \brief distructor */
	virtual ~XSemaphoreA() {}

	/*! \brief semaphore object 생성 */
	BOOL Create(int nInitialCount, int nMaximumCount, const char *pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);

	/*! \brief 이미 생성된 semaphore object를 open 한다. */
	BOOL Open(const char *pszName = NULL, DWORD dwDesiredAccess = SEMAPHORE_ALL_ACCESS, BOOL bInheritHandle = FALSE);

	/*! \brief semaphore object를 Releases 한다. */
	BOOL Release(LONG nReleaseCount, LONG* pnPreviousCount = NULL);

	/*! \brief Wait를 호출한다. */
	virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);

	/*! \brief Release(1)를 호출한다. */
	virtual BOOL Unlock();
};

class XSYSTEM_API XSemaphoreW : public XWaitableObject, public XSyncObject
{
public:
    /*! \brief defualt constructor */
    XSemaphoreW() {}

    /*! \brief distructor */
    virtual ~XSemaphoreW() {}

    /*! \brief semaphore object 생성 */
    BOOL Create(int nInitialCount, int nMaximumCount, const wchar_t *pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);

    /*! \brief 이미 생성된 semaphore object를 open 한다. */
    BOOL Open(const wchar_t *pszName = NULL, DWORD dwDesiredAccess = SEMAPHORE_ALL_ACCESS, BOOL bInheritHandle = FALSE);

    /*! \brief semaphore object를 Releases 한다. */
    BOOL Release(LONG nReleaseCount, LONG* pnPreviousCount = NULL);

    /*! \brief Wait를 호출한다. */
    virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);

    /*! \brief Release(1)를 호출한다. */
    virtual BOOL Unlock();
};

#if !defined(UNICODE) && !defined(_UNICODE)
    typedef XSemaphoreA XSemaphore;
#else
    typedef XSemaphoreW XSemaphore;
#endif

/********************************************************************************
	class XEvent
********************************************************************************/
/*!
\brief					event object에 대한 wrapper class

\remarks
						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XSyncObject, XWaitableObject, XCriticalSection, XMutex, XSemaphore, XWaitableCollection
*/
class XSYSTEM_API XEventA : public XWaitableObject, public XSyncObject
{
public:
	/*! \brief defualt constructor */
	XEventA() {}

	/*! \brief distructor */
	virtual ~XEventA() {}

	/*! \brief event object 생성 */
	BOOL Create(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, const char *pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);

	/*! \brief 이미 생성된 event object를 open 한다. */
	BOOL Open(const char *pszName, DWORD dwDesiredAccess = EVENT_ALL_ACCESS, BOOL bInheritHandle = FALSE);

	/*! \brief event object를 signaled state로 만든다. */
	BOOL Set();

	/*! \brief event object를 nonsignaled state로 만든다. */
	BOOL Reset();

	/*! \brief event object를 signaled state로 만들어 waiting threads를 releases하고 다시 event object를 nonsignaled state로 만든다. */
	BOOL Pulse();

	/*! \brief Wait를 호출한다. */
	virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);

	virtual BOOL Unlock();
};

class XSYSTEM_API XEventW : public XWaitableObject, public XSyncObject
{
public:
    /*! \brief defualt constructor */
    XEventW() {}

    /*! \brief distructor */
    virtual ~XEventW() {}

    /*! \brief event object 생성 */
    BOOL Create(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, const wchar_t *pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);

    /*! \brief 이미 생성된 event object를 open 한다. */
    BOOL Open(const wchar_t *pszName, DWORD dwDesiredAccess = EVENT_ALL_ACCESS, BOOL bInheritHandle = FALSE);

    /*! \brief event object를 signaled state로 만든다. */
    BOOL Set();

    /*! \brief event object를 nonsignaled state로 만든다. */
    BOOL Reset();

    /*! \brief event object를 signaled state로 만들어 waiting threads를 releases하고 다시 event object를 nonsignaled state로 만든다. */
    BOOL Pulse();

    /*! \brief Wait를 호출한다. */
    virtual BOOL Lock(DWORD dwMilliseconds = INFINITE);

    virtual BOOL Unlock();
};

#if !defined(UNICODE) && !defined(_UNICODE)
    typedef XEventA XEvent;
#else
    typedef XEventW XEvent;
#endif

/********************************************************************************
    class XRWCriticalSection
********************************************************************************/
class XSYSTEM_API XRWCriticalSection
{
public:
    XRWCriticalSection();
    virtual ~XRWCriticalSection();

    BOOL Create();
    BOOL Destroy();

    BOOL LockReader(IN DWORD dwMilliseconds = INFINITE);
    BOOL UnlockReader();
    BOOL LockWriter(IN DWORD dwMilliseconds = INFINITE);
    BOOL UnlockWriter();

protected:
    struct Context;
    Context *m_pContext;
};

/********************************************************************************
	class XLock
********************************************************************************/
/*!
\brief					생성자와 소멸자에서 내부 XSyncObject 객체의 Lock과 Unlock 함수를 각각 호출한다.

\remarks
						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XSyncObject, XCriticalSection, XMutex, XSemaphore, XEvent
*/
class XSYSTEM_API XLock
{
public:
	/*! \brief constructor */
	explicit XLock(XSyncObject& r, DWORD dwMilliseconds = INFINITE);

	/*! \brief constructor */
	explicit XLock(XSyncObject* p, DWORD dwMilliseconds = INFINITE);

	/*! \brief distructor */
	virtual ~XLock();

	/*! \brief 내부 XSyncObject를 Lock 한다. */
	BOOL Lock(DWORD dwMilliseconds = INFINITE);

	/*! \brief 내부 XSyncObject를 Unlock 한다. */
	BOOL Unlock();

	/*! \brief 내부 XSyncObject 객체가 Lock 상태인지 검사한다. */
	BOOL IsLocked();

	/*! \brief 내부 XSyncObject 객체를 pObj로 attach 한다. */
	void Attach(XSyncObject* pObj);

	/*! \brief 내부 XSyncObject 객체를 detach 한다. */
	XSyncObject* Detach();

protected:
	struct Context;
	enum { ContextSize = 12 };
	BYTE m_context[ContextSize];
};

/********************************************************************************
    class XRWLock
********************************************************************************/
class XSYSTEM_API XRWLock
{
public:
    XRWLock(IN XRWCriticalSection* p, IN BOOL bWriter, IN DWORD dwMilliseconds = INFINITE);
    XRWLock(IN XRWCriticalSection& r, IN BOOL bWriter, IN DWORD dwMilliseconds = INFINITE);
    virtual ~XRWLock();

    BOOL Lock(IN DWORD dwMilliseconds = INFINITE);
    BOOL Unlock();
    BOOL IsLocked();

    void Attach(IN XRWCriticalSection* p, IN BOOL bWriter);
    XRWCriticalSection* Detach();

protected:
    struct Context;
    enum { ContextSize = 12 };
    BYTE m_context[ContextSize];
};

/********************************************************************************
	class XWaitableCollection
********************************************************************************/
/*!
\brief					WaitForMultipleObjectsEx 사용을 위한 wrapper class

\remarks				SYNCHRONIZE 억세스 권한을 가지는 하나 이상의 handle이 signaled state가 될때까지 기다린다.

						- Required header: <XBase/XSystem/XSyncObject.h>

\sa						XMutex, XSemaphore, XEvent
*/
class XSYSTEM_API XWaitableCollection
{
public:
	/*! \brief default constructor */
	XWaitableCollection();

	/*! \brief distructor */
	virtual ~XWaitableCollection();

	/*! \brief XWaitableCollection 객체에 Add된 waiting할 handle 갯수 */
	int GetCount() const;

	/*! \brief waiting할 handle을 추가한다. */
	BOOL Add(HANDLE hObject);

	/*! \brief waiting할 handle을 제거한다. */
	void Remove(HANDLE hObject);


	/*! \brief Add된 hande의 array로 Win32 API의 WaitForMultipleObjects를 호출한다. */
	DWORD Wait(BOOL bWaitAll = FALSE, DWORD dwMilliseconds = INFINITE);

	/*! \brief Add된 hande의 array로 Win32 API의 WaitForMultipleObjectsEx를 호출한다. */
	DWORD WaitEx(BOOL bAlertable = TRUE, BOOL bWaitAll = FALSE, DWORD dwMilliseconds = INFINITE);

	/*! \brief Add된 hande의 array로 Win32 API의 MsgWaitForMultipleObjects를 호출한다. */
	DWORD MsgWait(DWORD dwWakeMask = QS_ALLINPUT, BOOL bWaitAll = FALSE, DWORD dwMilliseconds = INFINITE);

	/*! \brief Add된 hande의 array로 Win32 API의 MsgWaitForMultipleObjectsEx를 호출한다. */
	DWORD MsgWaitEx(DWORD dwFlags = 0, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwMilliseconds = INFINITE);

private:
	struct Context;
	Context* m_pContext;
};

#endif //__XSyncObject__
