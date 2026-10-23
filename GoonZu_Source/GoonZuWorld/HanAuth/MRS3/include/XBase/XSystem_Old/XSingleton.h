// XSingleton.h

#ifndef __XSingleton_Old__
#define __XSingleton_Old__

#include "XSyncObject.h"

/********************************************************************************
	class template XSingleton
********************************************************************************/
/*!
	\brief				Ensure a class has only one instance and provide a global point of access to it.
						.
							- Creation: new, delete
							- Lifetime: Last In First Out
							- Threading Model: class-level locking
						.

	- Required header: <XBase/XSystem/XSmartPtr.h>
*/
template<typename T>
class XSingleton
{
public:
	inline static T& Instance()
	{
		if (NULL == m_pInstance)
			MakeInstance();

		return (T&)*m_pInstance;
	}

private:
	XSingleton() {}
	static void MakeInstance()
	{
		XLock guard(m_cs);

		if (NULL == m_pInstance)
		{
			if (m_bDestroyed)
			{
				OnDeadReference();
				m_bDestroyed = FALSE;
			}

			m_pInstance = new T;
			if (NULL == m_pInstance)
				throw std::bad_alloc();

			atexit(DestroySingleton);
		}
	}
	static void DestroySingleton()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
		m_bDestroyed = FALSE;
	}
	static void OnDeadReference()
	{
		throw std::logic_error("Dead Reference Detected");
	}

private:
	static volatile T* m_pInstance;
	static XCriticalSection m_cs;
	static BOOL m_bDestroyed;
};
template<typename T> volatile T* XSingleton<T>::m_pInstance = NULL;
template<typename T> XCriticalSection XSingleton<T>::m_cs;
template<typename T> BOOL XSingleton<T>::m_bDestroyed = FALSE;


/********************************************************************************
	class template XSingletonStatic
********************************************************************************/
template <typename T>
struct XSingletonStatic
{
public:
	inline static T& Instance()
	{
		static T obj;
		m_ObjectCreator.DoNothing();
		return obj;
	}

private:
	XSingletonStatic() {}

	class InternalObjectCreator
	{
	public:
		InternalObjectCreator()
		{
			XSingletonStatic<T>::Instance();
		}
		inline void DoNothing() const {}
	};
	static InternalObjectCreator m_ObjectCreator;
};
template <typename T>
typename XSingletonStatic<T>::InternalObjectCreator XSingletonStatic<T>::m_ObjectCreator;


#endif //__XSingleton__

