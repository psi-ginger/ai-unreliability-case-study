//-------------------------------------------------------------------------------------------------
// NkList.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkAlgorithm.h"

//-------------------------------------------------------------------------------------------------
// 리스트 순환 예제1 - Iterator의 사용
//-------------------------------------------------------------------------------------------------
/*
	NkList< int > list;
	NkList< int >::Iterator iter;

    ...

	// 리스트의 모든 데이터 출력
	for( iter = list.Begin(); iter != list.End(); ++iter )
	{
		printf( "%d", iter->GetData() );
	}
*/

//-------------------------------------------------------------------------------------------------
// 리스트 순환 예제2 - NkForEach 사용
//-------------------------------------------------------------------------------------------------
/*
	NkList< int > klist;

	...

	struct MyAction
	{
		void operator()( int& data )
		{
			printf( "%d ", data );
		}
	} kMyAction;

	// 리스트의 모든 데이터 출력
	NkForEach( list.Begin(), list.End(), kMyAction );

*/

//-------------------------------------------------------------------------------------------------
// Remove_If, Find_If 사용 예제
//-------------------------------------------------------------------------------------------------
/*
	NkList< int > list;
	NkList< int >::Iterator iter;

	...

	// 사용자 정의 함수자(삭제)
	struct MyRemoveCond
	{
		bool operator()( int& data )
		{
			return ( data > 10 );
		}
	} kRemoveCond;

	// 10보다 큰 데이터를 모두 삭제
	list.Remove_If( kRemoveCond );


	// 사용자 정의 함수자(검색)
	struct MyFindCond
	{
		bool operator()( int& data )
		{
			return ( data % 2 == 0 );
		}
	} kFindCond;

	// 짝수 데이터 모두 출력	
	iter = list.Find_If( kFindCond );

	while( iter != 0 )
	{		
		printf( "%d ", iter->GetData() );
		iter = list.Find_If( kFindCond, ++iter );
	}

*/


//-------------------------------------------------------------------------------------------------
// NkListPos
//-------------------------------------------------------------------------------------------------
typedef void* NkListPos;

//-------------------------------------------------------------------------------------------------
// NkListNode
//-------------------------------------------------------------------------------------------------
template <class T>
class NkListNode : public NkMemory
{
public:
	T& GetData() { return m_data; }

	NkListNode<T>* m_pkPrev;
	NkListNode<T>* m_pkNext;	
	T m_data;
};

//-------------------------------------------------------------------------------------------------
// NkListIterator
//-------------------------------------------------------------------------------------------------
template <class T>
class NkList_Iterator : public NkMemory
{
public: 
	typedef NkListNode<T> _Node;
	typedef NkList_Iterator<T> _Iter;

	NkList_Iterator() : m_pkNode(0) {}
	NkList_Iterator(_Node* pkNode) : m_pkNode(pkNode) {}

	T& operator*() const { return m_pkNode->m_data; }
	_Node* operator->() const { return m_pkNode; }

	_Iter& operator++() { m_pkNode = m_pkNode->m_pkNext; return (*this); }
	_Iter operator++(int) { _Node* pkTemp = m_pkNode; m_pkNode = m_pkNode->m_pkNext; return (_Iter)pkTemp; }

	_Iter& operator--() { m_pkNode = m_pkNode->m_pkPrev; return (*this); }
	_Iter operator--(int) { _Node* pkTemp = m_pkNode; m_pkNode = m_pkNode->m_pkPrev; return (_Iter)pkTemp; }

	bool operator==(const _Iter& kRight) const { return m_pkNode == kRight.m_pkNode; }
	bool operator!=(const _Iter& kRight) const { return m_pkNode != kRight.m_pkNode; }

	_Node* m_pkNode;
};

//-------------------------------------------------------------------------------------------------
// NkList
//-------------------------------------------------------------------------------------------------
template <class T>
class NkList : public NkMemory
{
public:
	typedef NkListNode<T>* NodePtr;
	typedef NkList_Iterator<T> Iterator;

	NkList();
	~NkList();

	// 크기 관련 함수
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedByteSize() const;

	Iterator Begin() const;
	Iterator End() const;

	// data access
	inline T& GetData(NkListPos kPos) const;
	inline T* GetDataPtr(NkListPos kPos) const;

	// pos
	inline NkListPos GetFrontPos() const;
	inline NkListPos GetBackPos() const;

	inline NkListPos GetNextPos(NkListPos kPos) const;
	inline NkListPos GetPrevPos(NkListPos kPos) const;

	inline void MoveFront(NkListPos& kPos) const;
	inline void MoveBack(NkListPos& kPos) const;
	inline void MoveNext(NkListPos& kPos) const;
	inline void MovePrev(NkListPos& kPos) const;

	// node
	NodePtr GetFrontNode() const { return m_pkFront; }
	NodePtr GetBackNode() const { return m_pkBack; }

	// 삽입 함수
	T* NewFront();
	T* NewBack();

	void PushFront(const T& data);
	void PushBack(const T& data);

	void PushNodeFront(NodePtr pkNode);
	void PushNodeBack(NodePtr pkNode);

	T* NewAfter(Iterator kIter);
	T* NewBefore(Iterator kIter);

	Iterator InsertAfter(Iterator kIter, const T& data);
	Iterator InsertBefore(Iterator kIter, const T& data);

	Iterator InsertNodeAfter(Iterator kIter, NodePtr pkNode);
	Iterator InsertNodeBefore(Iterator kIter, NodePtr pkNode);

	// 삭제 함수
	void PopFront();
	void PopBack();

	void RemoveAll();
	Iterator Remove(Iterator kIter);

	template<class _FuncType>
	void Remove_If(_FuncType _Cond)
	{
		Iterator _Last = End();
		for( Iterator _First = Begin(); _First != _Last; )
			if( _Cond(*_First) )
				_First = Remove(_First);
			else
				++_First;
	}

	// detach
	NodePtr Detach(Iterator kIter);

	// 검색 함수
	Iterator Find(const T& data);

	template<class _FuncType>
	Iterator Find_If(_FuncType _Cond)
	{
		Iterator _Last = End();
		for( Iterator _First = Begin(); _First != _Last; ++_First )
			if( _Cond(*_First) ) return _First;

		return 0;
	}

	template<class _FuncType>
	Iterator Find_If(_FuncType _Cond, Iterator _Start)
	{
		Iterator _Last = End();
		for( ; _Start != _Last; ++_Start )
			if( _Cond(*_Start) ) return _Start;

		return 0;
	}


protected:
	NodePtr _NewNode();
	void _DeleteNode(NodePtr pkNode);

	NodePtr m_pkFront;
	NodePtr m_pkBack;

	unsigned int m_uiCount;
};

#include "NkList.inl"
