// ============================================================================
// Data Structures For Game Programmers
// Ron Penton
// HashTable.h
// This file holds the Linekd Hash Table implementation.
// ============================================================================

 
#ifndef _HASHTABLE_INTINT_H_
#define _HASHTABLE_INTINT_H_

#include "HashTable.h"

// ============================================================================
//  Classes
// ============================================================================

class HashInt
{
public:
	int m_Data1;

public:
	// constructor
	HashInt()
	{
        m_Data1 = 0;
	}
	// constructor
    HashInt( int Data1)
    {
        m_Data1 = Data1;
    }

	void GetHashDat( int &Data1)
	{
        Data1 = m_Data1;
	}

    // = operator
    void operator= ( HashInt& Right )
    {
		m_Data1 = Right.m_Data1;
    }

    // == operator
    bool operator== ( HashInt& Right )
    {
		if(m_Data1 != Right.m_Data1) return false;
		return true;
    }
};

// ============================================================================
//  Classes
// ============================================================================

class HashIntInt
{

public:
// ----------------------------------------------------------------
//  Name:           m_Table
//  Description:    Hash Table
// ----------------------------------------------------------------
	HashTable< HashInt, HashInt > *m_Table;
	
	
public:
// ----------------------------------------------------------------
//  Name:           HashIntInt
//  Description:    Int를 Key(Unique하다)로 하고 Data를 설정할수 있는 HashTable을 생성한다.
//  Arguments:      siArraySize -  The size of the table
//                  siEntrySize -  실제로 저장할 최대 Entry의 갯수를 저장한다.
//  Return Value:   None
// ----------------------------------------------------------------
	HashIntInt(int siArraySize, int siEntrySize)
	{
		m_Table = new HashTable< HashInt, HashInt >(siArraySize, siEntrySize, IntHashKeyFunc);
	}
// ----------------------------------------------------------------
//  Name:           ~HashIntInt
//  Description:    생성된 HashTable을 제거한다.
//  Arguments:      None
//  Return Value:   None
// ----------------------------------------------------------------
	~HashIntInt()
	{
		if(m_Table)		delete m_Table;
	}

// ----------------------------------------------------------------
//  Name:           String Hasher
//  Description:    주어진 Int를 그냥 Hash Key로 사용한다.
//  Arguments:      None
//  Return Value:   None
// ----------------------------------------------------------------
	static unsigned long int IntHashKeyFunc( HashInt IntKey )
	{
		return IntKey.m_Data1;
	}
// ----------------------------------------------------------------
//  Name:           Insert
//  Description:    String을 Key값으로 int Data1, Data2를 설정한다.
//  Arguments:      Key     - Key로 사용할 Int
//					siData1 - 첫번째 Data
//  Return Value:   HashTable에 추가 성공여부 
//               	(기존에 같은 값이 존재하거나 추가실패시 false)
// ----------------------------------------------------------------

	bool Insert(int Key, int siData1)
	{
		if(Find(Key, siData1) == true) 	{	Remove(Key);	} 
		
		HashInt Data(siData1);
		return m_Table->Insert(Key, Data);
	}
	
// ----------------------------------------------------------------
//  Name:           Remove
//  Description:    해당 Key로 설정된 Entry의 제거
//  Arguments:      Key     - Key로 사용할 Int
//  Return Value:   HashTable에서의 제거 성공여부
//               	(기존에 같은 값이 존재하지 않으면 false)
// ----------------------------------------------------------------
	bool Remove(int Key)
	{
		return m_Table->Remove(Key);
	}
	
// ----------------------------------------------------------------
//  Name:           Find
//  Description:    해당 Key로 설정된 Entry의 제거
//  Arguments:      Key		- Key로 사용할 문자열 
//					siData1 - 첫번째 Data
//  Return Value:   HashTable에서의 검색 성공여부
//               	(기존에 같은 값이 존재하지 않으면 false)
// ----------------------------------------------------------------
	bool Find(int Key, int& Data1)
	{
		HashInt HashKey(Key);
		
		// hash entry 를 생성한다.
		HashEntry< HashInt, HashInt >* entry;
		
		// 해당 Key값으로 Data를 검색한다.
		entry = m_Table->Find( HashKey );
		
		if( entry != 0 )
		{
			HashInt Data(entry->m_Data);

			// 돌려 보낼 Data를 설정한다.
			Data.GetHashDat(Data1);
			return true;
		}

		return false;
	}

	

};

#endif