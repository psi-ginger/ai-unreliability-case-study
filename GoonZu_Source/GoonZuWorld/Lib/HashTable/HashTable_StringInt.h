// ============================================================================
// Data Structures For Game Programmers
// Ron Penton
// HashTable.h
// This file holds the Linekd Hash Table implementation.
// ============================================================================

 
#ifndef _HASHTABLE_STRINGINT_H_
#define _HASHTABLE_STRINGINT_H_

#include "HashTable.h"

// ============================================================================
//  Classes
// ============================================================================
class HashString
{
public:
    TCHAR m_String[64];

    // empty constructor
    HashString() 
    {
        
		_tcscpy( m_String, TEXT("") );    
    }

    // copy-string constructor
    HashString( TCHAR* pString )
    {
/*		if(pString == NULL)
		{
			SI32 b = 0;
		}
		if(lstrlen(pString) >= 64)
		{
			SI32 a = 0;
		}
		*/
        _tcscpy( m_String, pString );
    }

    // == operator
    bool operator== ( HashString& Right )
    {
        return !_tcscmp( m_String, Right.m_String);
    }
};

// ============================================================================
//  Classes
// ============================================================================

class HashData
{
public:
	int m_Data1;
	int m_Data2;
	
public:
	// constructor
	HashData()
	{
        m_Data1 = 0;
		m_Data2 = 0;
	}
	// constructor
    HashData( int Data1, int Data2)
    {
        m_Data1 = Data1;
		m_Data2 = Data2;
    }

	void GetHashDat( int &Data1, int &Data2)
	{
        Data1 = m_Data1;
		Data2 = m_Data2;
	}

    // = operator
    void operator= ( HashData& Right )
    {
		m_Data1 = Right.m_Data1;
		m_Data2 = Right.m_Data2;
    }

    // == operator
    bool operator== ( HashData& Right )
    {
		if(m_Data1 != Right.m_Data1 || m_Data2 != Right.m_Data2 ) return false;
		return true;
    }
};

// ============================================================================
//  Classes
// ============================================================================

class HashStringInt
{

public:
// ----------------------------------------------------------------
//  Name:           m_Table
//  Description:    Hash Table
// ----------------------------------------------------------------
	HashTable< HashString, HashData > *m_Table;
	
	
public:
// ----------------------------------------------------------------
//  Name:           HashStringInt
//  Description:    String을 Key로 하고 Data를 설정할수 있는 HashTable을 생성한다.
//  Arguments:      siArraySize -  The size of the table
//                  siEntrySize -  실제로 저장할 최대 Entry의 갯수를 저장한다.
//  Return Value:   None
// ----------------------------------------------------------------
	HashStringInt(int siArraySize, int siEntrySize)
	{
		m_Table = new HashTable< HashString, HashData >(siArraySize, siEntrySize, StringHashKeyFunc);
	}
// ----------------------------------------------------------------
//  Name:           ~HashStringInt
//  Description:    생성된 HashTable을 제거한다.
//  Arguments:      None
//  Return Value:   None
// ----------------------------------------------------------------
	~HashStringInt()
	{
		if(m_Table)		delete m_Table;
	}

// ----------------------------------------------------------------
//  Name:           String Hasher
//  Description:    주어진 String으로부터 Hash 정수를 계산한다.
//  Arguments:      None
//  Return Value:   None
// ----------------------------------------------------------------
	static unsigned long int StringHashKeyFunc( HashString String )
	{
		unsigned long int hash = 0;
		int i;
		int length = _tcslen( String.m_String );
		for( i = 0; i < length; i++ )
		{
			hash += ( (i + 1) * (int)String.m_String[i] );
		}
		return hash;
	}
// ----------------------------------------------------------------
//  Name:           Insert
//  Description:    String을 Key값으로 int Data1, Data2를 설정한다.
//  Arguments:      pString - Key로 사용할 문자열 
//					siData1 - 첫번째 Data
//					siData2 - 두번째 Data
//  Return Value:   HashTable에 추가 성공여부 
//               	(기존에 같은 값이 존재하거나 추가실패시 false)
// ----------------------------------------------------------------

	bool Insert(TCHAR *pString, int siData1, int siData2)
	{
		if(Find(pString, siData1, siData2, 10) == true)	{	Remove(pString);	}
		
		HashData Data(siData1, siData2);
		return m_Table->Insert(pString, Data);
	}
	
// ----------------------------------------------------------------
//  Name:           Remove
//  Description:    해당 Key로 설정된 Entry의 제거
//  Arguments:      pString - Key로 사용할 문자열 
//  Return Value:   HashTable에서의 제거 성공여부
//               	(기존에 같은 값이 존재하지 않으면 false)
// ----------------------------------------------------------------
	bool Remove(TCHAR *pString)
	{
		return m_Table->Remove(pString);
	}
	
// ----------------------------------------------------------------
//  Name:           Find
//  Description:    해당 Key로 설정된 Entry의 제거
//  Arguments:      pString - Key로 사용할 문자열 
//					siData1 - 첫번째 Data
//					siData2 - 두번째 Data
//  Return Value:   HashTable에서의 검색 성공여부
//               	(기존에 같은 값이 존재하지 않으면 false)
// ----------------------------------------------------------------
	bool Find(TCHAR *pString, int& Data1, int& Data2, SI32 unique)
	{
		if(pString == (TCHAR*)0xcdcdcdcd)
		{
			SI32 a = 0;
		}

		// String 을 생성한다.
		HashString str( pString );
		
		// hash entry 를 생성한다.
		HashEntry< HashString, HashData >* entry;
		
		// 해당 Key값으로 Data를 검색한다.
		entry = m_Table->Find( str );
		
		if( entry != 0 )
		{
			HashData Data(entry->m_Data);

			// 돌려 보낼 Data를 설정한다.
			Data.GetHashDat(Data1, Data2);
			return true;
		}

		return false;
	}

	

};

#endif