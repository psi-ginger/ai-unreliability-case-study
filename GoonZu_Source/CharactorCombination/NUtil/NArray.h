//-----------------------------------------------------------------------------
// 2005 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#ifndef _NARRAY_H
#define _NARRAY_H

#include <windows.h>

//-----------------------------------------------------------------------------
// NArray
//-----------------------------------------------------------------------------
template< class Type >
class NArray
{
public:
	NArray() : m_pArray(0), m_siMaxSize(0) {}

	NArray( int maxsize ) 
	{
		m_pArray = new Type[ maxsize ];
		m_siMaxSize = maxsize;
	}

	~NArray() 
	{
		if( m_pArray ) {
			delete [] m_pArray;
			m_pArray = NULL;

			m_siMaxSize = 0;
		}
	}

	void CreateArray( int maxsize )
	{
		if( m_pArray ) delete [] m_pArray;

		m_pArray = new Type[ maxsize ];
		m_siMaxSize = maxsize;
	}

	inline int GetMaxSize() 
	{
		return m_siMaxSize;
	}

	inline Type& GetData( int i ) 
	{ 
		return m_pArray[ i ]; 
	}

	inline Type* GetDataPtr( int i )
	{
		return &m_pArray[ i ];
	}

	inline Type* GetSafeDataPtr( int i )
	{
		if( i < 0 || i >= m_siMaxSize ) return &m_pArray[ i ];
		return NULL;
	}

	inline bool IsValidIndex( int i )
	{
		return (i >= 0 && i < m_siMaxSize );
	}

protected:
	Type*		m_pArray;
	int			m_siMaxSize;
};


//-----------------------------------------------------------------------------
// NStringArray
//-----------------------------------------------------------------------------
class NStringArray : public NArray< char* >
{
public:
	NStringArray() : m_pString(0), m_siMaxStrUnitSize(0) {}
	~NStringArray() 
	{
		if( m_pString ) 
		{
			delete [] m_pString;
			m_pString = NULL;

			m_siMaxStrUnitSize = 0;
		}
	}

	void CreateStringArray( int maxsize, int maxstrunitsize )
	{ 
		CreateArray( maxsize );

		if( m_pString ) delete [] m_pString;

		m_pString = new char[ maxsize * maxstrunitsize ];

		char *pTemp = m_pString;

		for( int i = 0; i < maxsize; ++i ) 
		{
			m_pArray[ i ] = pTemp;
			pTemp += maxstrunitsize;
		}

		m_siMaxStrUnitSize = maxstrunitsize;
	}

	inline int GetMaxStrUnitSize() { return m_siMaxStrUnitSize; }

protected:
	char*		m_pString;
	int			m_siMaxStrUnitSize;

};

#endif