//-----------------------------------------------------------------------------
// 2009/03/06
// 엔도어즈 기술개발팀 
// 이제완
//-----------------------------------------------------------------------------

#ifndef _NKPOOL_H
#define _NKPOOL_H

#include "NkMemory.h"

template <class T>
class NkPoolContainer
{
	NkPoolContainer(unsigned int uiSize);
	~NkPoolContainer();

	T* GetObject(unsigned int uiIndex);
	
	void SetNext( );

protected:
	T* m_pkObjects;
	unsigned int m_uiSize;

	NkPoolContainer
};


template <class T>
class NkPool : public NkMemory
{
public:
	NkPool(unsigned int uiInitSize, unsigned int uiMaxSize );
	~NkPool();

	T* GetFreeObject();
	void ReleaseObject(T* pObject);

	void PurgeAllObjects();

	NkQueue< NkPoolContainer<T> >* m_kContainers;
	NkQueue<T*>* m_pkFreeObjects;

	unsigned int m_uiLimitSize;
	unsigned int m_uiGrowhSize;
};

template <class T>
NkPool<T>::NkPool()
{

}

template <class T>
NkPool<T>::~NkPool()
{

}

template <class T>
void NkPool<T>::PurgeAllObjects( T* data )
{

}

template <class T>
T* NkPool<T>::GetFreeObject()
{

}

template <class T>
void NkPool<T>::ReleaseObject( T* data )
{

}