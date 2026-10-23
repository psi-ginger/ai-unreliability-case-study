//-----------------------------------------------------------------------------
// 2008/08/20 
// 엔도어즈 기술개발팀 
// 이제완
//-----------------------------------------------------------------------------

#ifndef _NKPTRLIST_H
#define _NKPTRLIST_H

#include "NkMemory.h"

typedef void* NkPtrListPos;

template <class T>
class NkPtrListNode : public NkMemory
{
public:
	NkPtrListNode<T>* GetNext() { return m_pkNext; }
	NkPtrListNode<T>* GetPrev() { return m_pkPrev; }
	T* GetDataPtr() { return &m_data; }
	T& GetData() { return m_data; }

	NkPtrListNode<T>* m_pkNext;
	NkPtrListNode<T>* m_pkPrev;
	T m_data;
};

template <class T>
class NkPtrList : public NkMemory
{
public:
	NkPtrList();
	~NkPtrList();

	// pos
	void MoveFirst( NkPtrListPos pos );
	void MoveNext( NkPtrListPos pos );
	void MovePrev( NkPtrListPos pos );

	NkPtrListPos GetFirstPos();
	NkPtrListPos GetLastPos();

	// node

	// remove
	void RemoveAll();
	void RemoveFirst();
	void RemoveLast();
	void Remove( NkPtrListNode<T>* pkNode );

	// new
	T* NewFirst();
	T* NewLast();

	// add
	void AddNodeFirst( NkPtrListNode<T>* pkNode );
	void AddNodeLast( NkPtrListNode<T>* pkNode );

	// insert
	void InsertNodeAfter( NkPtrListPos pos, NkPtrListNode<T>* pkNode );
	void InsertNodeBefore( NkPtrListPos pos, NkPtrListNode<T>* pkNode );


	NkPtrListNode<T>* GetFirstNode() { return m_pkFirst; }
	NkPtrListNode<T>* GetLastNode() { return m_pkLast; }

	int GetNodeCount() { return m_iCount; }

protected:
	NkPtrListNode<T>* NewNode();
	void DeleteNode( NkPtrListNode<T>* pkNode );

	NkPtrListNode<T>* m_pkFirst;
	NkPtrListNode<T>* m_pkLast;

	int m_iCount;
};

template <class T> inline
NkPtrList<T>::NkPtrList()
{
	m_iCount = 0;
	m_pkFirst = 0;
	m_pkLast = 0;
}

template <class T> inline
NkPtrList<T>::~NkPtrList()
{
	RemoveAll();
}

//-----------------------------------------------------------------------------
// pos
//-----------------------------------------------------------------------------

template <class T> inline
void NkPtrList<T>::MoveFirst()
{
	m_pkCurrent = m_pkFirst;
}

template <class T> inline
void NkPtrList<T>::MoveNext()
{
	m_pkCurrent = m_pkCurrent->m_pkNext;
}


template <class T> inline
void NkPtrList<T>::RemoveAll()
{
	NkPtrListNode<T>* pkNode = m_pkFirst;
	NkPtrListNode<T>* pkDelNode;

	while( pkNode )
	{
		pkDelNode = pkNode;
		pkNode = pkNode->m_pkNext;
		DeleteNode( pkDelNode );
	}

	m_iCount = 0;
	m_pkFirst = 0;
	m_pkLast = 0;
}

template <class T> inline
void NkPtrList<T>::RemoveFirst()
{
	NkPtrListNode<T>* pkNode = m_pkFirst;

	if( !pkNode ) return;

	m_pkFirst = m_pkFirst->m_pkNext;

	if( m_pkFirst ) m_pkFirst->m_pkPrev = 0;
	else m_pkLast = 0;

	DeleteNode( pkNode );
}

template <class T> inline
void NkPtrList<T>::RemoveLast()
{
	NkPtrListNode<T>* pkNode = m_pkLast;

	if( !pkNode ) return;

	m_pkLast = m_pkLast->m_pkPrev;

	if( m_pkLast ) m_pkLast->m_pkNext = 0;
	else m_pkFirst = 0;

	DeleteNode( pkNode );
}

template <class T> inline
void NkPtrList<T>::Remove( NkPtrListNode<T>* pkNode )
{
	if( pkNode == m_pkFirst )
	{
		RemoveFirst();
	}
	else if( pkNode == m_pkLast )
	{
		RemoveLast();
	}
	else
	{
		NkPtrListNode<T>* pkPrev = pkNode->m_pkPrev;
		NkPtrListNode<T>* pkNext = pkNode->m_pkNext;

		if( pkPrev ) pkPrev->m_pkNext = pkNext;
		if( pkNext ) pkNext->m_pkPrev = pkPrev;

		DeleteNode( pkNode );
	}
}


template <class T> inline
NkPtrListNode<T>* NkPtrList<T>::NewNode()
{
	return NkNew NkPtrListNode< T >;
}

template <class T> inline
void NkPtrList<T>::DeleteNode( NkPtrListNode<T>* pkNode )
{
	NkDelete pkNode;
}

template <class T> inline
void NkPtrList<T>::AddNodeFirst( NkPtrListNode<T>* pkNode )
{
	pkNode->m_pkPrev = 0;
	pkNode->m_pkNext = m_pkFirst;

	if( m_pkFirst ) m_pkFirst->m_pkPrev = pkNode;
	else m_pkLast = pkNode;

	m_pkFirst = pkNode;
	m_iCount++;
}

template <class T> inline
void NkPtrList<T>::AddNodeLast( NkPtrListNode<T>* pkNode )
{
	pkNode->m_pkNext = 0;
	pkNode->m_pkPrev = m_pkLast;

	if( m_pkLast ) m_pkLast->m_pkNext = pkNode;
	else m_pkFirst = pkNode;

	m_pkLast = pkNode;
	m_iCount++;
}

template <class T> inline
void NkPtrList<T>::AddNodeAfter( NkPtrListNode<T>* pkNode )
{
	pkNode->m_pkPrev = 0;
	pkNode->m_pkNext = m_pkFirst;

	if( m_pkFirst ) m_pkFirst->m_pkPrev = pkNode;
	else m_pkLast = pkNode;

	m_pkFirst = pkNode;
	m_iCount++;
}

template <class T> inline
void NkPtrList<T>::AddNodeBefore( NkPtrListNode<T>* pkNode )
{
	pkNode->m_pkNext = 0;
	pkNode->m_pkPrev = m_pkLast;

	if( m_pkLast ) m_pkLast->m_pkNext = pkNode;
	else m_pkFirst = pkNode;

	m_pkLast = pkNode;
	m_iCount++;
}

template <class T> inline
T* NkPtrList<T>::AddNewFirst()
{
	NkPtrListNode<T>* pkNode;
	pkNode = NewNode();

	AddNodeFirst( pkNode );

	return &pkNode->m_data;
}

template <class T> inline
T* NkPtrList<T>::AddNewLast()
{
	NkPtrListNode<T>* pkNode;
	pkNode = NewNode();

	AddNodeLast( pkNode );

	return &pkNode->m_data;
}

#endif