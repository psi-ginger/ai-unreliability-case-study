#ifndef __XStaticObject_Old__
#define __XStaticObject_Old__

#include <Windows.h>

template<typename T>
class XStaticObject
{
private:
    static volatile int m_nInstanceCount;
    static volatile T *m_pInstance;

public:
    XStaticObject(void)
    {
        if (InterlockedIncrement(reinterpret_cast<volatile LONG *>(&m_nInstanceCount)) == 1)
        {
		    m_pInstance = new T;
        }
    }
    ~XStaticObject(void)
    {
        if (InterlockedDecrement(reinterpret_cast<volatile LONG *>(&m_nInstanceCount)) == 0)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    T *operator->()
    {
        return m_pInstance;
    }

    const T *operator->() const
    {
        return m_pInstance;
    }

    static T *GetInstance(void)
    {
        return m_pInstance;
    }
};

template<typename T> volatile int XStaticObject<T>::m_nInstanceCount = 0;
template<typename T> volatile T *XStaticObject<T>::m_pInstance = 0;

#endif
