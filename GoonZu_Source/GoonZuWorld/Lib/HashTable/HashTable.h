// ============================================================================
// Data Structures For Game Programmers
// Ron Penton
// HashTable.h
// This file holds the Linekd Hash Table implementation.
// ============================================================================

 
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "Array.h"
#include "DLinkedList.h"
#include "MLinkedList.h"


// -------------------------------------------------------
// Name:        HashEntry
// Description: Hash Table Entry Class
//              Key와 Data를 쌍으로 저장한다.
// -------------------------------------------------------
template< class KeyType, class DataType>
class HashEntry
{
public:
    KeyType		m_Key;
    DataType	m_Data;
};



// -------------------------------------------------------
// Name:        HashTable
// Description: Hashtable Class.
// -------------------------------------------------------
template< class KeyType, class DataType>
class HashTable
{

	// typedef the entry class 
    typedef HashEntry<KeyType, DataType> Entry;

public:    
// ----------------------------------------------------------------
//  Name:           m_ArraySize
//  Description:    table의 크기 
// ----------------------------------------------------------------
    int m_siArraySize;

// ----------------------------------------------------------------
//  Name:           m_EntrySize
//  Description:    Entry Node의 크기 
// ----------------------------------------------------------------
    int m_siEntrySize;

// ----------------------------------------------------------------
//  Name:           m_TableEntryCount
//  Description:    Table에 속해있는 Entry의 Count
// ----------------------------------------------------------------
    int m_siTableEntryCount;

// ----------------------------------------------------------------
//  Name:           m_ArrayTable
//  Description:    Entry의 Linked List를 갖고있는 ArrayTable
// ----------------------------------------------------------------
    Array< DLinkedList< Entry > >		m_ArrayTable;

// ----------------------------------------------------------------
//  Name:           m_EntryList
//  Description:    실제 Entry Node의 메모리를 관리하는 Table
// ----------------------------------------------------------------
	MLinkedList< Entry >				m_EntryList;

// ----------------------------------------------------------------
//  Name:           m_HashFunc
//  Description:    Hash function Pointer
// ----------------------------------------------------------------
    unsigned long int (*m_HashFunc)(KeyType);


public:

// ----------------------------------------------------------------
//  Name:           HashTable
//  Description:    HashTable을 구성하기위한 초기화를 수행한다.
//  Arguments:      ArraySize - Table Array의 크기
//                  EntrySize - Node의 크기
//                  HashFunc  - Hashing Function.
//  Return Value:   None
// ----------------------------------------------------------------
    HashTable( int ArraySize, int EntrySize, unsigned long int (*HashFunc)(KeyType) )
        : m_ArrayTable( ArraySize ) , m_EntryList(EntrySize)
    {
        // set the size, hash function, and count.
        m_siArraySize		= ArraySize;
        m_HashFunc			= HashFunc;
		m_siEntrySize		= EntrySize;
        m_siTableEntryCount	= 0;

    }

// ----------------------------------------------------------------
//  Name:           HashTableIndex
//  Description:    Key값으로 ArrayTable index를 얻어온다. 
//  Arguments:      Key  - key
//  Return Value:   Table Index
// ----------------------------------------------------------------
	inline int HashTableIndex(KeyType Key)
	{
		return m_HashFunc( Key ) % m_siArraySize;
	}
	

// ----------------------------------------------------------------
//  Name:           Insert
//  Description:    Table에 Key와 Data를 쌍으로 추가한다.
//  Arguments:      Key  - the key
//                  Data - Key에 부과되는 Data
//  Return Value:   None
// ----------------------------------------------------------------
    bool Insert( KeyType Key, DataType Data)
    {
        // Entry 를 생성하고 값을 설정한다.
        Entry entry;
        entry.m_Key		= Key;
        entry.m_Data	= Data;

		// Key부터 Hash Value를 얻어와 이를 Table의 크기로 
		// 나머지를 계산하여 Table index를 구한다.
		int index = HashTableIndex(Key);		

		// 해당 Index에 Entry를 설정한다.
        bool bsuccess = m_ArrayTable[index].Append( entry , m_EntryList.Pop());
		if(bsuccess)	m_siTableEntryCount++;

		return bsuccess;
    }

// ----------------------------------------------------------------
//  Name:           Find
//  Description:    Table에서 Key값으로 Entry를 검색한다.
//  Arguments:      Key - 검색에 사용될 Key
//  Return Value:   Entry Pointer를 돌려준다.
// ----------------------------------------------------------------
    Entry* Find( KeyType Key )
    {
		// Key를 사용하여 Table index를 얻어온다.
        int index = HashTableIndex(Key);		

        // Iterator를 사용하여 List의 Head를 얻어온다.
        DListIterator<Entry> itr = m_ArrayTable[index].GetIterator();

        // 각 Item이 Valid한지 검사하여 원하는 Key를 찾도록 검색한다.
        while( itr.Valid() )
        {
			// 원하는 key를 찾았으면 해당 Entry의 Pointer를 돌려준다.
            if( itr.Item().m_Key == Key )	return &(itr.Item());
            itr.Forth();
        }

		// 찾지못하면 NULL을 돌려준다.
        return 0;
    }

 
// ----------------------------------------------------------------
//  Name:           Remove
//  Description:    Key 를 사용하여 해당 Entry를 제거한다.
//  Arguments:      Key - key
//  Return Value:   제거에 성공하면 true
// ----------------------------------------------------------------
    bool Remove( KeyType Key )
    {
		// Key로 해당 Index를 계산한다.
        int index = HashTableIndex(Key);	

		// Iterator를 사용하여 List의 Head를 얻어온다.
        DListIterator<Entry> itr = m_ArrayTable[index].GetIterator();

        // 각 Item이 Valid한지 검사하여 원하는 Key를 찾도록 검색한다.
        while( itr.Valid() )
        {
            // 원하는 key를 찾았으면 해당 Entry를 제거하고 메모리를 돌려준다.
            if( itr.Item().m_Key == Key )
            {
				m_EntryList.Push(m_ArrayTable[index].Remove(itr));
                m_siTableEntryCount--;
                return true;
            }
            itr.Forth();
        }

        // 찾지못하면 NULL을 돌려준다.
        return false;
    }


// ----------------------------------------------------------------
//  Name:           Count
//  Description:    Table에 할당되어져 사용되고 있는 Entry갯수를 얻어온다.
//  Arguments:      None
//  Return Value:   Table에 할당되어있는 Entry갯수
// ----------------------------------------------------------------
    int Count()
    {
        return m_siTableEntryCount;
    }

};


#endif

