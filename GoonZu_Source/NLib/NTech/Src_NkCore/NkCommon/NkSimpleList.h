//-----------------------------------------------------------------------------
// 2008/08/20 
// 엔도어즈 기술개발팀 
// 이제완
//-----------------------------------------------------------------------------

#ifndef _NKSIMPLELIST_H
#define _NKSIMPLELIST_H

template <class T>
class NkSimpleListNode
{
public:
	NkSimpleListNode<T>* GetNext() { return m_pkNext; }
	T* GetDataPtr() { return &m_data; }
	T GetData() { return m_data; }

	NkSimpleListNode<T>* m_pkNext;
	T m_data;
};

template <class T>
class NkSimpleList
{
public:
	NkSimpleList();
	~NkSimpleList();

	void RemoveAll();

	NkSimpleListNode<T>* NewNode();
	void DeleteNode( NkSimpleListNode<T>* pkNode );

	void AddNodeFirst( NkSimpleListNode<T>* pkNode );

	T* AddNewFirst();

	void MoveFirst();
	void MoveNext();

	T& GetCurData() { return m_pkCurrent->m_data; }
	T* GetCurDataPtr() { return &m_pkCurrent->m_data; }

	NkSimpleListNode<T>* GetFirstNode() { return m_pkFirst; }
	NkSimpleListNode<T>* GetCurNode() { return m_pkCurrent; }

	int GetCount() { return m_iCount; }

protected:
	NkSimpleListNode<T>* m_pkFirst;
	NkSimpleListNode<T>* m_pkCurrent;

	int m_iCount;
};

template <class T> inline
NkSimpleList<T>::NkSimpleList()
{
	m_iCount = 0;
	m_pkFirst = 0;
	m_pkCurrent = 0;
}

template <class T> inline
NkSimpleList<T>::~NkSimpleList()
{
	RemoveAll();
}

template <class T> inline
void NkSimpleList<T>::RemoveAll()
{
	NkSimpleListNode<T>* pkNode = m_pkFirst;
	NkSimpleListNode<T>* pkDelNode;

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
NkSimpleListNode<T>* NkSimpleList<T>::NewNode()
{
	return new NkSimpleListNode< T >;
}

template <class T> inline
void NkSimpleList<T>::DeleteNode( NkSimpleListNode<T>* pkNode )
{
	delete pkNode;
}

template <class T> inline
void NkSimpleList<T>::AddNodeFirst( NkSimpleListNode<T>* pkNode )
{
	pkNode->m_pkPrev = 0;
	pkNode->m_pkNext = m_pkFirst;

	if( m_pkFirst ) m_pkFirst->m_pkPrev = pkNode;
	else m_pkLast = pkNode;

	m_pkFirst = pkNode;
	m_iCount++;
}

template <class T> inline
T* NkSimpleList<T>::AddNewFirst()
{
	NkSimpleListNode<T>* pkNode;
	pkNode = NewNode();

	AddNodeFirst( pkNode );

	return &pkNode->m_data;
}

template <class T> inline
void NkSimpleList<T>::MoveFirst()
{
	m_pCurrent = m_pkFirst;
}

template <class T> inline
void NkSimpleList<T>::MoveNext()
{
	m_pkCurrent = m_pkCurrent->m_pkNext;
}

#endif