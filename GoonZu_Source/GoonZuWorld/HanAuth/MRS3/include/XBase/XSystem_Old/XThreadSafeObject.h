//
// XThreadSafeObject
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XThreadSafeObject_Old__
#define __XThreadSafeObject_Old__

#include "XSyncObject.h"

#if (_MSC_VER >= 1301)
namespace Private
{
    template<typename T>
    struct TAddPointer
    {
        typedef T* TPointerType;
        typedef T TPointeeType;
    };
    template<typename T>
    struct TAddPointer<T *>
    {
        typedef T* TPointerType;
        typedef T TPointeeType;
    };
}

template<typename T>
class XThreadSafeObject
{
private:
    typename Private::TAddPointer<T>::TPointeeType m_Obj;
    XCriticalSection m_cs;

private:
    XThreadSafeObject(const XThreadSafeObject &rhs);
    XThreadSafeObject &operator=(const XThreadSafeObject &rhs);

public:
    XThreadSafeObject(void)
    {
    }
    virtual ~XThreadSafeObject(void)
    {
        try
        {
            Lock();
            Unlock();
        }
        catch (...)
        {
        }
    }

    typename Private::TAddPointer<T>::TPointerType Lock(void)
    {
        m_cs.Enter();

        return &m_Obj;
    }
    void Unlock(void)
    {
        m_cs.Leave();
    }
};

template<typename T>
class XRWThreadSafeObject
{
private:
    typename Private::TAddPointer<T>::TPointeeType m_Obj;
    XRWCriticalSection m_cs;

private:
    XRWThreadSafeObject(const XRWThreadSafeObject &rhs);
    XRWThreadSafeObject &operator=(const XRWThreadSafeObject &rhs);

public:
    XRWThreadSafeObject(void)
    {
        if (m_cs.Create() == FALSE)
        {
            throw std::runtime_error("[XRWThreadSafeObject::XRWThreadSafeObject()] XRWCriticalSection::Create() failed.");
        }
    }
    virtual ~XRWThreadSafeObject(void)
    {
        try
        {
            LockWriter();
            UnlockWriter();
        }
        catch (...)
        {
        }
    }

    typename Private::TAddPointer<T>::TPointerType LockReader(void)
    {
        m_cs.LockReader();

        return &m_Obj;
    }
    void UnlockReader(void)
    {
        m_cs.UnlockReader();
    }
    typename Private::TAddPointer<T>::TPointerType LockWriter(void)
    {
        m_cs.LockWriter();

        return &m_Obj;
    }
    void UnlockWriter(void)
    {
        m_cs.UnlockWriter();
    }
};

#endif

#endif