// XObjectPool.h
/*!
\file		XObjectPool.h
*/

#ifndef __XObjectPool_Old__
#define __XObjectPool_Old__

#include "XSyncObject.h"
#include <vector>
#include <cassert>

#pragma warning(disable:4345)


/********************************************************************************
	class XObjectPool<T>
********************************************************************************/
/*!
	\brief				빈번히 발생하는 특정 object의 생성, 소멸 작업의 효율성을 위한 pooling 기능을 제공하는 class

	- Required header: <XBase/XSystem/XObjectPool.h>
*/
template <typename T>
class XObjectPool
{
public:
	/*! \brief defualt constructor */
	XObjectPool();

	/*! \brief distructor */
	virtual ~XObjectPool();


	/*!
	\brief					XObjectPool 객체에 object가 사용할 리소스의 최소 갯수를 reserve 한다.

	\param dwCount			[in] pool에 미리 생성할 object의 리소스 최소 갯수
							- Default: 0(zero)

	\param dwMaximumFreeObj	[in] 반납된 object의 메모리를 해제하지 않고 pool에서 유지할 최대 갯수 
							- Default: 최대값에 대해 제한하지 않는다.

	\return					BOOL
							- TRUE: 생성 성공
							- FALSE: 생성 실패
	*/
	BOOL Reserve(DWORD dwCount);

	/*!
	\brief					XObjectPool 객체의 리소스를 해제한다.

	\param					This function has no parameters.

	\return					This function does not return a value.

	\remarks				Clear 함수는 XObjectPool 객체로 반납된 object에 대한 리소스만을 대상으로 한다.
							즉, XObjectPool 객체로부터 Alloc 함수로 할당한 후 Free 함수에 의해 반납되지 않은 
							object의 리소스에 대해서는 해제하지 않는다.
	*/
	void Clear();


	/*!
	\brief					pool로부터 object를 할당한다.

	\param					This function has no parameters.

	\return					할당된 object의 pointer

	\remarks				Alloc 함수에 의한 object 할당시 object의 defualt constructor가 호출된다.
	*/
	inline T* Alloc();

	/*!
	\brief					할당한 object를 pool에 반납한다.

	\param					반납할 object의 pointer

	\return					This function does not return a value.

	\remarks				Free 함수에 의한 object 반납시 object의 distructor가 호출된다.
	*/
	inline void Free(T* pObject);

protected:
	DWORD m_dwMaximumFreeObj;
	std::vector<void*> m_pool;
	XCriticalSection m_cs;
};


/********************************************************************************
	class XObjectPool<T> Impl
********************************************************************************/
template <typename T>
XObjectPool<T>::XObjectPool()
	: m_dwMaximumFreeObj((DWORD)-1)
{
}

template <typename T>
XObjectPool<T>::~XObjectPool()
{
	Clear();
}

template <typename T>
BOOL XObjectPool<T>::Reserve(DWORD dwCount)
{
	XLock guard(m_cs);
	void* p = NULL;

	Clear();

	m_pool.reserve(dwCount);
	while (m_pool.size() < dwCount)
	{
		p = ::malloc(sizeof(T));
		if (NULL == p)
		{
			Clear();
			return FALSE;
		}
		m_pool.push_back(p);
	}

	return TRUE;
}

template <typename T>
void XObjectPool<T>::Clear()
{
	XLock guard(m_cs);
	void* p = NULL;

	while (!m_pool.empty())
	{
		p = m_pool.back();
		m_pool.pop_back();
		if (NULL != p)
			free(p);
	}
}

template <typename T>
T* XObjectPool<T>::Alloc()
{
	XLock guard(m_cs);
	T* p = NULL;

	if (m_pool.empty())
	{
		p = (T*)malloc(sizeof(T));
	}
	else
	{
		p = (T*)m_pool.back();
		m_pool.pop_back();
	}

	if (NULL != p)
		new (p) T();

	return p;
}

template <typename T>
void XObjectPool<T>::Free(T* p)
{
	if (NULL == p)
		return;

	XLock guard(m_cs);

	p->~T();
	if (((DWORD)-1 == m_dwMaximumFreeObj) || (m_pool.size() < m_dwMaximumFreeObj))
	{
#ifdef _DEBUG
		memset(p, 0xCC, sizeof(T));
#endif
		m_pool.push_back(p);
	}
	else
		free(p);
}


#pragma warning(default:4345)

#endif //__XObjectPool__
