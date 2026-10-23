// ============================================================================
// Data Structures For Game Programmers
// Jaycee
// SLinkedList.h
// This is the Single-Linked List class
// ============================================================================
#ifndef _MLINKEDLIST_H_
#define _MLINKEDLIST_H_

#include "HashTable.h"

// -------------------------------------------------------
// Name:        UseFreeList
// Description: This is the Single-linked list node class.
// -------------------------------------------------------
template<class Datatype>
class MLinkedList
{
private:
// ----------------------------------------------------------------
//  Name:           m_Free
//  Description:    현재 사용할수있는 Free nodes
// ----------------------------------------------------------------
    DLinkedList<Datatype>	m_Free;

// ----------------------------------------------------------------
//  Name:           m_pData
//  Description:    미리할당한 모든 Node의 배열 Pointer
// ----------------------------------------------------------------

	DListNode<Datatype>*	m_pData;

// ----------------------------------------------------------------
//  Name:           m_siCount
//  Description:    메모리로 할당한 Node의 Count 
// ----------------------------------------------------------------
    int m_siCount;

// ----------------------------------------------------------------
//  Name:           m_siFreeCount
//  Description:    현재 새로 할당할수 있는 남아있는 Node Count
// ----------------------------------------------------------------
	int m_siFreeCount;

// ----------------------------------------------------------------
//  Name:           m_siUsedCount
//  Description:    이미 할당하여 사용하고 있는 Node Count
// ----------------------------------------------------------------
	int m_siUsedCount;

public:


// ----------------------------------------------------------------
//  Name:           MLinkedList
//  Description:    Node를 할당하고 변수들을 초기화한다.
//  Arguments:      count - 새로 생성할 Node의 갯수
//  Return Value:   None.
// ----------------------------------------------------------------
    MLinkedList(int size)
    {
		m_siCount	  = 0;
		m_siFreeCount = 0;
		m_siUsedCount = 0;

		m_pData = new DListNode<Datatype>[size];
		
		if(m_pData)
		{
			m_siCount	  = size;
			m_siFreeCount = size;
			m_siUsedCount = 0;
		}


		Datatype tempdata;
		for(int i= 0; i < m_siCount; ++i)
		{
			m_Free.Append(tempdata, &m_pData[i]);
		}
    }

    
// ----------------------------------------------------------------
//  Name:           ~MLinkedList
//  Description:    메로리를 할당한 Node를 모두 해제한다.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    ~MLinkedList()
    {
		if(m_pData)	delete [] m_pData;
		m_pData		  = 0;

		m_siCount	  = 0;
		m_siFreeCount = 0;
		m_siUsedCount = 0;
    }

// ----------------------------------------------------------------
//  Name:           Pop
//  Description:    남아있는 Node중 하나를 돌려준다.
//  Arguments:      None.
//  Return Value:   free Node.
// ----------------------------------------------------------------
    DListNode<Datatype> *Pop()
    {
		DListNode<Datatype> *freenode = NULL;

		freenode = m_Free.RemoveTail();

		if(freenode)
		{
			m_siFreeCount--;
			m_siUsedCount++;
		}
		return freenode;

	}

// ----------------------------------------------------------------
//  Name:           Push
//  Description:    사용하고 돌려준 Node를 축적한다.
//  Arguments:      freenode .
//  Return Value:   None.
// ----------------------------------------------------------------

	void Push(DListNode<Datatype> *freenode)
	{
		if(!freenode) return;
		Datatype tempdata;
		m_Free.Append(tempdata, freenode);

		m_siFreeCount++;
		m_siUsedCount--;
	}

// ----------------------------------------------------------------
//  Name:           CheckMemoryLeak
//  Description:    사용되어진 Node와 남아있는 노드의 갯수와 총 갯수를 비교한다.
//  Arguments:      freenode .
//  Return Value:   compare count.
// ----------------------------------------------------------------
	bool CheckMemoryLeak()
	{
		if(m_siCount == (m_siFreeCount+m_siUsedCount)) return true;
		return false;
	}


// ----------------------------------------------------------------
//  Name:           Size
//  Description:    총 메모리로 할당된 Node갯수를 돌려준다.
//  Arguments:      None.
//  Return Value:   size of the list.
// ----------------------------------------------------------------
    int Size()
    {
        return m_siCount;
    }


// ----------------------------------------------------------------
//  Name:           FreeSize
//  Description:    새로 할당할수있는 Node의 갯수 
//  Arguments:      None.
//  Return Value:   free size of the list.
// ----------------------------------------------------------------
    int FreeSize()
    {
        return m_siFreeCount;
    }


// ----------------------------------------------------------------
//  Name:           UsedSize
//  Description:    이미 할당되어진 Node의 갯수
//  Arguments:      None.
//  Return Value:   used size of the list.
// ----------------------------------------------------------------
    int UsedSize()
    {
        return m_siUsedCount;
    }
};

#endif

