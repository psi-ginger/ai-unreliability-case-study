//-----------------------------------------------------------------------------
// 2005/09/13 ������
//-----------------------------------------------------------------------------

#ifndef _NARRAY_H
#define _NARRAY_H

#include "NDataTypes.h"
#include <assert.h>
#include <exception>

//-----------------------------------------------------------------------------
// NArray
//-----------------------------------------------------------------------------
template< class Type >
class NArray
{
public:
	NArray() : m_pArray(0), m_iMaxSize(0) {}

	NArray( int maxSize ) 
	{
		m_pArray = new Type[ maxSize ];
		m_iMaxSize = maxSize;
	}
	
	~NArray() 
	{
		if( m_pArray ) {
			delete [] m_pArray;
			m_pArray = NULL;
		}

		m_iMaxSize = 0;
	}

	void CreateArray( int maxSize )
	{
		if( m_pArray ) delete [] m_pArray;

		m_pArray = new Type[ maxSize ];
		m_iMaxSize = maxSize;
	}

	inline int GetMaxSize() 
	{
		return m_iMaxSize;
	}

	inline Type& GetElement( int i ) 
	{ 
		return m_pArray[ i ]; 
	}

	inline Type* GetElementPtr( int i )
	{
		return &m_pArray[ i ];
	}

	inline bool IsValidIndex( int i )
	{
		return (i >= 0 && i < m_iMaxSize );
	}

	inline Type& operator [] (int i)
	{
		return GetElement( i );
	}

protected:
	Type*		m_pArray;
	int			m_iMaxSize;
};


//-----------------------------------------------------------------------------
// NStringArray
//-----------------------------------------------------------------------------
class NStringArray : public NArray< TCHAR* >
{
public:
	NStringArray() : m_pBuffer(0), m_iMaxStrSize(0) {}
	~NStringArray() 
	{
		if( m_pBuffer ) 
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}

		m_iMaxStrSize = 0;
	}

	void CreateStringArray( int maxArraySize, int maxStrSize )
	{ 
		CreateArray( maxArraySize );

		if( m_pBuffer ) delete [] m_pBuffer;

		m_pBuffer = new TCHAR[ maxArraySize * maxStrSize ];

		TMemSet( m_pBuffer, 0, maxArraySize * maxStrSize );

		TCHAR *pTemp = m_pBuffer;

		for( int i = 0; i < maxArraySize; ++i ) 
		{
			m_pArray[ i ] = pTemp;
			pTemp += maxStrSize;
		}

		m_iMaxStrSize = maxStrSize;
	}
	
	inline int GetMaxStrSize() { return m_iMaxStrSize; }

protected:
	TCHAR*		m_pBuffer;
	int			m_iMaxStrSize;

};

template<class T, int T_MAXSIZE>
class NSafeTArray
{
private:
	// ���ܽ� �߻��ÿ� ��ȯ�� �޸� ������ �ϳ� ��´�.
	T	m_Array[T_MAXSIZE];

public:
	NSafeTArray()
	{
#ifdef _DEBUG
		assert(T_MAXSIZE > 0);
#endif
		ZeroMem();
	}
	~NSafeTArray()	{}

	void ZeroMem()
	{
		ZeroMemory(m_Array, sizeof(m_Array));
	}

	T& operator [] (int nIndex)
	{
#ifdef _DEBUG
		assert( nIndex >= 0 && nIndex < T_MAXSIZE );
		if( nIndex < 0 || nIndex >= T_MAXSIZE )
		{
			int a = 0;
		}

#else
		if( nIndex < 0 || nIndex >= T_MAXSIZE )
		{
			// �ε��� �ʰ��� ��ȯ�� �ӽ� ����
			static T Dump;
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, 0);

			ZeroMemory(&Dump, sizeof(Dump));
			return Dump;
		}
#endif
		return m_Array[nIndex];
	}

	void MemCpy(T* pSrc)
	{
		if(pSrc != NULL)	memcpy(m_Array, pSrc, sizeof(T) * T_MAXSIZE);
	}

	void MemCpy(NSafeTArray<T, T_MAXSIZE>& tData)
	{
		memcpy(m_Array, tData, sizeof(T) * T_MAXSIZE);
	}

	inline int GetSize()		{		return T_MAXSIZE;		}
};

template<class T>
class NSafeTMemArray
{
private:
	T*		m_pArray;
	int		m_iMaxSize;

public:
	NSafeTMemArray()
	{
		m_pArray = NULL;
		m_iMaxSize = 0;
	}

	~NSafeTMemArray()
	{
		if( m_pArray )	delete[] m_pArray;
	}

	void NewMem(int nSize)
	{
		if( m_pArray != NULL )	return;
		if( nSize <= 0 )		return;

		m_pArray = new T[nSize];
		m_iMaxSize = nSize;
	}

	// Grow the allocation when more room is required. Used by loaders that can
	// size themselves from the data they are about to read; without this a
	// fixed-size buffer raises EXCEPTION_ARRAY_BOUNDS_EXCEEDED on every
	// out-of-range write (see operator[] below).
	void ReNewMem(int nSize)
	{
		if( nSize <= 0 )			return;
		if( nSize <= m_iMaxSize )	return;

		if( m_pArray )	delete[] m_pArray;

		m_pArray = new T[ nSize ];
		m_iMaxSize = nSize;
	}

	T& operator [] (int nIndex)
	{
#ifdef _DEBUG
		assert( m_pArray != NULL);
		assert( nIndex >= 0 && nIndex < m_iMaxSize );
#else
		if( m_pArray == NULL || nIndex < 0 || nIndex >= m_iMaxSize )
		{
			// �ε��� �ʰ��� ��ȯ�� �ӽ� ����
			static T Dump;
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, 0);

			ZeroMemory(&Dump, sizeof(Dump));
			return Dump;
		}
#endif
		return m_pArray[nIndex];
	}

	inline int GetSize()		{		return m_iMaxSize;		}
};

#endif