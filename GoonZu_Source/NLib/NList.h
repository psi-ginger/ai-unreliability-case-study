//-----------------------------------------------------------------------------
// 2005/09/13 ������
//-----------------------------------------------------------------------------

#ifndef _NLIST_H
#define _NLIST_H

#include "NArray.h"

template< class Type >
class NList;

template< class Type >
class NListPool;

//-----------------------------------------------------------------------------
//
// POOL����� ����Ʈ�̴� 
//
// �⺻������ ������ NListPool�� ���ؼ� �ȴ�.
// 
//  NListPool :     A  <->  B  <->  C  <->  D 
//                  0       1       2       3 (�迭�ε���)
//
// 
//
//
//
// �� ���� �⺻���� ����Ʈ ���ν� ��,�� ����� ������ �Ӹ��ƴ϶� �迭������ ���� �ε����� ������. 
// �̴� ��带 ������ 
//-----------------------------------------------------------------------------
template< class Type >
class NListNode
{
public:
	friend class NList< Type >;
	friend class NListPool< Type >;

	NListNode() : m_pListPool(0), m_index(0), m_pPrev(0), m_pNext(0), m_pList(0) {}
	~NListNode() {}

	inline NListPool< Type >*	GetListPool() { return m_pListPool; }
	inline int					GetIndex() { return m_index; }

	inline NListNode< Type >*	GetPrev() { return m_pPrev; }
	inline NListNode< Type >*	GetNext() { return m_pNext; }

	inline NList< Type >*		GetList() { return m_pList; }

	inline Type&				GetData() { return m_data; }
	inline Type*				GetDataPtr() { return &m_data; }

	inline void					Remove() { m_pList->Remove( this ); }

private:
	NListPool< Type >*			m_pListPool;		// �ڽ��� ������ ����Ʈ Ǯ
	int							m_index;			// ����Ʈ Ǯ���� ���� ���� ������ �ڽ��� �ε���

	NListNode< Type >*			m_pPrev;			// ���� ��� ������
	NListNode< Type >*			m_pNext;			// ���� ��� ������

	NList< Type >*				m_pList;			// ���� �ڽ��� �����ϰ� �ִ� ����Ʈ

	Type						m_data;
};


//-----------------------------------------------------------------------------
// NListPool
//-----------------------------------------------------------------------------
template< class Type >
class NListPool
{
public:
	friend class NList< Type >;

	NListPool( int maxSize ) { CreateListPool( maxSize ); }
	
	NListPool() {}
	~NListPool() {}

	void CreateListPool( int maxSize )
	{
		m_array.CreateArray( maxSize );				// ��� �迭 ����

		NListNode< Type > *pNode;

		for( int i = 0; i < maxSize; ++i )
		{
			pNode = m_array.GetElementPtr( i );		// �迭���� ��� ������
			
			pNode->m_pListPool = this;				// �ڽ��� ������ ����Ʈ Ǯ ���� (this)
			pNode->m_index = i;						// ����Ʈ Ǯ���� �迭 �ε��� ���� 

			m_list.PushBack( pNode );				// ����Ʈ�� �߰�
		}
	}

	inline int GetMaxSize()							// �� ��� ���� ������
	{ 
		return m_array.GetMaxSize(); 
	}

	inline NListNode< Type >& GetNode( int i )		// Ư�� �ε����� ��� ������ (���۷���)
	{
		return m_array.GetElement( i );
	}

	inline NListNode< Type >* GetNodePtr( int i )	// Ư�� �ε����� ��� ������ (������)
	{ 
		return m_array.GetElementPtr( i ); 
	}

	inline Type& GetData( int i )					// Ư�� �ε����� ��忡 �ִ� ������ ������ (���۷���)
	{ 
		return m_array.GetElement( i ).GetData(); 
	}

	inline Type* GetDataPtr( int i )				// Ư�� �ε����� ��忡 �ִ� ������ ������ (������)
	{ 
		return m_array.GetElement( i ).GetDataPtr(); 
	}

	inline bool IsValidIndex( int i )				// �ε��� ���� ��ȿ���� ���θ� ��ȯ
	{
		return ( i >= 0 && i < m_array.GetMaxSize() );
	}

	inline int	GetListNodeNumber( )
	{
		return m_list.m_siCount;
	}
	
private:
	NArray< NListNode< Type > >		m_array;
	NList< Type >					m_list;
	
};

//-----------------------------------------------------------------------------
// NList
//-----------------------------------------------------------------------------
template< class Type >
class NList
{
public:
	friend class NListPool< Type >;

	NList();
	NList( NListPool< Type > *pListPool );
	NList( int listPoolSize );

	~NList();

	void		CreateList( NListPool< Type > *pListPool );
	void		CreateList( int listPoolSize );						// self ListPool

	void		Destroy();

	inline NListNode< Type >*	GetFirst() { return m_pFirst; }
	inline NListNode< Type >*	GetLast() { return m_pLast; }

	inline int					GetCount() { return m_siCount; }

	Type*						PushFrontGetData();
	Type*						PushBackGetData();

	Type*						RemoveFrontGetData();
	Type*						RemoveBackGetData();

	NListNode< Type >*			PushFront();
	NListNode< Type >*			PushBack();

	NListNode< Type >*			RemoveFront();
	NListNode< Type >*			RemoveBack();

	NListNode< Type >*			Remove( NListNode< Type > *pNode );

	NListNode< Type >*			Find( Type data );

private:
	NListNode< Type >*			PushFront( NListNode< Type > *pNode );
	NListNode< Type >*			PushBack( NListNode< Type > *pNode );

	NListNode< Type >*			PopFront();
	NListNode< Type >*			PopBack();


private:
	NListPool< Type >*			m_pListPool;

	NListNode< Type >*			m_pFirst;
	NListNode< Type >*			m_pLast;

	int							m_siCount;

	bool						m_bSelfListPool;
};


//-----------------------------------------------------------------------------
// NList
//-----------------------------------------------------------------------------
template< class Type >
NList< Type >::NList()
{
	m_pListPool = NULL;
	
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;

	m_bSelfListPool = FALSE;
}

template< class Type >
NList< Type >::NList( NListPool< Type > *pListPool )
{
	m_pListPool = pListPool;
	
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;	

	m_bSelfListPool = FALSE;
}

template< class Type >
NList< Type >::NList( int listPoolSize )
{
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;	

	m_pListPool = new NListPool< Type >
	m_pListPool->CreateListPool( listPoolSize );

	m_bSelfListPool = TRUE;
}

template< class Type >
NList< Type >::~NList()
{
	Destroy();
}

template< class Type >
void NList< Type >::Destroy()
{
	NListNode< Type > *pNode, *pTemp;

	pNode = m_pFirst;

	while( pNode ) 
	{
		pTemp = pNode;
		pNode = pNode->GetNext();

		Remove( pTemp );
	}

	if( m_bSelfListPool && m_pListPool )
	{
		delete m_pListPool;
	}
	
	m_pListPool = NULL;
	m_bSelfListPool = FALSE;

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;
}

template< class Type >
void NList< Type >::CreateList( NListPool< Type > *pListPool )
{
	Destroy();

	m_pListPool = pListPool;
}

template< class Type >
void NList< Type >::CreateList( int listPoolSize )
{
	Destroy();

	m_pListPool = new NListPool< Type >;
	m_pListPool->CreateListPool( listPoolSize );

	m_bSelfListPool = TRUE;
}


//-----------------------------------------------------------------------------
// ����Ʈ Ǯ���� ��带 �����ͼ� ����Ʈ �տ� �����ϰ�,
// ����� ������ �����͸� ��ȯ�Ѵ�
// ����Ʈ Ǯ�� ������ �� �ִ� ��尡 ���ٸ� NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
inline Type* NList< Type >::PushFrontGetData()
{
	NListNode< Type > *pNode;	
	
	pNode = PushFront( m_pListPool->m_list.PopFront() );

	if( pNode ) return &pNode->m_data;

	return NULL;
}

//-----------------------------------------------------------------------------
// ����Ʈ Ǯ���� ��带 �����ͼ� ����Ʈ �ڿ� �����ϰ�,
// ����� ������ �����͸� ��ȯ�Ѵ�
// ����Ʈ Ǯ�� ������ �� �ִ� ��尡 ���ٸ� NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
inline Type* NList< Type >::PushBackGetData()
{
	NListNode< Type > *pNode;	
	
	pNode = PushBack( m_pListPool->m_list.PopFront() );

	if( pNode ) return &pNode->m_data;

	return NULL;
}

//-----------------------------------------------------------------------------
// ����Ʈ Ǯ���� ��带 �����ͼ� ����Ʈ �ڿ� �����ϰ�,
// ����� ������ �����͸� ��ȯ�Ѵ�
// ����Ʈ Ǯ�� ������ �� �ִ� ��尡 ���ٸ� NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
inline Type* NList< Type >::RemoveFrontGetData()
{
	NListNode< Type > *pNode;

	pNode = PopFront();

	if( pNode == NULL ) return NULL;

	pNode = pNode->m_pListPool->m_list.PushBack( pNode );

	if( pNode == NULL ) return NULL;
		
	return &pNode->m_data;
}

template< class Type >
inline Type* NList< Type >::RemoveBackGetData()
{
	NListNode< Type > *pNode;

	pNode = PopBack();

	if( pNode == NULL ) return NULL;

	pNode = m_pListPool->m_list.PushBack( pNode );

	if( pNode == NULL ) return NULL;
		
	return &pNode->m_data;
}

//-----------------------------------------------------------------------------
// ����Ʈ Ǯ���� ��带 �����ͼ� ����Ʈ �ڿ� �����ϰ�, ��� �����͸� ��ȯ�Ѵ�
// ����Ʈ Ǯ���� ������ �� �ִ� ��尡 ���ٸ� NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
inline NListNode< Type >* NList< Type >::PushFront()
{
	return PushFront( m_pListPool->m_list.PopFront() );
}

template< class Type >
inline NListNode< Type >* NList< Type >::PushBack()
{
	 return PushBack( m_pListPool->m_list.PopFront() );
}

template< class Type >
inline NListNode< Type >* NList< Type >::RemoveFront()
{
	NListNode< Type > *pNode = PopFront();

	if( pNode == NULL ) return NULL;

	return pNode->m_pListPool->m_list.PushBack( pNode );
}

template< class Type >
inline NListNode< Type >* NList< Type >::RemoveBack()
{	
	NListNode< Type > *pNode = PopBack();

	if( pNode == NULL ) return NULL;

	return pNode->m_pListPool->m_list.PushBack( pNode );
}


//-----------------------------------------------------------------------------
// ����Ʈ���� �־��� ��带 ���� �Ѵ�
// ����� �Ҽ� ����Ʈ �����Ͱ� this�� ���� �ʴٸ� NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
NListNode< Type >* NList< Type >::Remove( NListNode<Type> *pNode )
{
	if( pNode->m_pList == &pNode->m_pListPool->m_list ) return NULL;

	if( pNode->m_pList != this ) return NULL;

	if( pNode == m_pFirst ) {

		m_pFirst = pNode->m_pNext;

	} else {

		pNode->m_pPrev->m_pNext = pNode->m_pNext;
	}

	if( pNode == m_pLast ) {

		m_pLast = pNode->m_pPrev;

	} else {

		pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}

	m_siCount--;

	pNode->m_pListPool->m_list.PushBack( pNode );

	return pNode;
}

template< class Type >
inline NListNode< Type >* NList< Type >::Find( Type data )
{

	NListNode< Type > *pNode = m_pFirst;

	while( pNode )
	{
		if( pNode->m_data == data ) return pNode;

		pNode = pNode->m_pNext;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// ����Ʈ ù��° ��� �տ� �־��� ��带 �����Ѵ�
// �μ��� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------
template< class Type >
NListNode< Type >* NList< Type >::PushFront( NListNode< Type > *pNode )
{

	if( pNode == NULL ) return NULL;

	pNode->m_pList = this;

	if( m_siCount == 0 ) {

		pNode->m_pPrev = NULL;
		pNode->m_pNext = NULL;

		m_pFirst = m_pLast = pNode;

	} else {

		pNode->m_pPrev = NULL;
		pNode->m_pNext = m_pFirst;

		m_pFirst->m_pPrev = pNode;
 
		m_pFirst = pNode;

	}

	m_siCount++;

	return pNode;
}

//-----------------------------------------------------------------------------
// ����Ʈ ������ ��� �ڿ� �־��� ��带 �����Ѵ�
// �μ��� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------
template< class Type >
NListNode< Type >* NList< Type >::PushBack( NListNode< Type > *pNode )
{

	if( pNode == NULL ) return NULL;

	pNode->m_pList = this;

	if( m_siCount == 0 ) {

		pNode->m_pPrev = NULL;
		pNode->m_pNext = NULL;

		m_pFirst = m_pLast = pNode;

	} else {

		pNode->m_pNext = NULL;
		pNode->m_pPrev = m_pLast;

		m_pLast->m_pNext = pNode;

		m_pLast = pNode;

	}	

	m_siCount++;

	return pNode;
}


//-----------------------------------------------------------------------------
// ����Ʈ���� ù��° ��带 ������
// ����� �Ҽ� ����Ʈ �����ʹ� NULL�� �ȴ�
// ����Ʈ�� ��尡 �ϳ��� ������ NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
NListNode< Type >* NList< Type >::PopFront()
{
	
	NListNode< Type > *pNode;

	if( m_siCount == 0 ) return NULL;		
	
	m_siCount--;

	pNode = m_pFirst;
	pNode->m_pList = NULL;

	if( m_siCount == 0 ) {
		
		m_pFirst = NULL;
		m_pLast = NULL;

		return pNode;
	}

	m_pFirst = pNode->m_pNext;
	m_pFirst->m_pPrev = NULL;

	

	return pNode;
}

//-----------------------------------------------------------------------------
// ����Ʈ���� ������ ��带 ������
// ����� �Ҽ� ����Ʈ �����ʹ� NULL�� �ȴ�
// ����Ʈ�� ��尡 �ϳ��� ������ NULL�� ��ȯ�Ѵ�
//-----------------------------------------------------------------------------
template< class Type >
NListNode< Type >* NList< Type >::PopBack()
{

	NListNode< Type > *pNode;

	if( m_siCount == 0 ) return NULL;		

	m_siCount--;

	pNode = m_pLast;

	pNode->m_pList = NULL;

	if( m_siCount == 0 ) {		
		
		m_pFirst = NULL;
		m_pLast = NULL;

		return pNode;
	}

	m_pLast = pNode->m_pPrev;
	m_pLast->m_pNext = NULL;
	
	return pNode;
}


#endif