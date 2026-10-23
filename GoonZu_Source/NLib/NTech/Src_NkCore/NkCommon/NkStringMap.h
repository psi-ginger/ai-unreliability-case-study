//-----------------------------------------------------------------------------
// 2008/08/20 
// 엔도어즈 기술개발팀 
// 이제완
//-----------------------------------------------------------------------------

#ifndef _NKSTRINGMAP_H
#define _NKSTRINGMAP_H

#include "NkHashMap.h"

#define _CRT_SECURE_NO_WARNINGS

#if defined(DEBUG) || defined(_DEBUG)
#define STRSAFE_LIB
#else
#define STRSAFE_INLINE
#endif

#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CB_FUNCTIONS

#include <tchar.h>
#include <strsafe.h>

//-----------------------------------------------------------------------------
// NkStringMapA
//-----------------------------------------------------------------------------
class NkStringMapHashFunctorA
{
public:
	static unsigned int KeyToHashIndex( const char* pcKey, unsigned int uiHashSize );
};

class NkStringMapEqualsFunctorA
{
public:
	static bool IsKeysEqual( const char* pcKey1, const char* pcKey2 );
};


inline unsigned int NkStringMapHashFunctorA::KeyToHashIndex( const char* pcKey, unsigned int uiHashSize )
{
	unsigned int uiHash = 0;

	while( *pcKey )
	{
		uiHash = (uiHash << 5) + uiHash + *pcKey++;
	}
	return uiHash % uiHashSize;
}

inline bool NkStringMapEqualsFunctorA::IsKeysEqual( const char* pcKey1, const char* pcKey2 )
{
	return strcmp( pcKey1, pcKey2 ) == 0;
}


template <class TData, class THash = NkStringMapHashFunctorA, class TEquals = NkStringMapEqualsFunctorA>
class NkStringMapA : public NkHashMap<const char*, TData, THash, TEquals>
{
public:
	NkStringMapA( unsigned int uiHashSize = 37, bool bCopy = true );
	virtual ~NkStringMapA();

protected:
	virtual void SetData( NkHashMapNode<const char*,TData>* pkNode, const char* pcKey, TData& data );
	virtual void ClearData( NkHashMapNode<const char*,TData>* pkNode );

	bool m_bCopy;
};


// 생성자
template <class TData, class THash, class TEquals> inline
NkStringMapA<TData,THash,TEquals>::NkStringMapA( unsigned int uiHashSize, bool bCopy ) : NkHashMap( uiHashSize )
{
	m_bCopy = bCopy;
};

// 소멸자
template <class TData, class THash, class TEquals> inline
NkStringMapA<TData,THash,TEquals>::~NkStringMapA()
{
	if( m_bCopy )
	{
		NkHashMapNode<const char*,TData>* pkNode;

		for( unsigned int i = 0; i < m_uiHashSize; ++i )
		{
			pkNode = m_ppkHashTable[ i ];

			while( pkNode )
			{
				m_ppkHashTable[ i ] = pkNode->m_pkNext;
				free((char*)pkNode->m_key);

				pkNode = m_ppkHashTable[ i ];
			}
		}
	}
};

// SetData
template <class TData, class THash, class TEquals> inline
void NkStringMapA<TData,THash,TEquals>::SetData( NkHashMapNode<const char*,TData>* pkNode, const char* pcKey, TData& data)
{
	if( m_bCopy )
	{
		size_t size;
		StringCchLengthA( pcKey, 4096, &size );
		++size;
		pkNode->m_key = (char*)malloc( size );
		StringCchCopyA( (char*)pkNode->m_key, size, pcKey );
	}
	else
	{
		pkNode->m_key = pcKey;
	}
	pkNode->m_data = data;
}


// ClearData
template <class TData, class THash, class TEquals> inline
void NkStringMapA<TData,THash,TEquals>::ClearData( NkHashMapNode<const char*,TData>* pkNode )
{
	if( m_bCopy )
	{
		free( (char*)pkNode->m_key );
	}
}


//-----------------------------------------------------------------------------
// NkStringMapNoCaseA
//-----------------------------------------------------------------------------

class NkStringMapNoCaseHashFunctorA
{
public:
	static unsigned int KeyToHashIndex( const char* pcKey, unsigned int uiHashSize );
};

class NkStringMapNoCaseEqualsFunctorA
{
public:
	static bool IsKeysEqual( const char* pcKey1, const char* pcKey2 );
};

inline unsigned int NkStringMapNoCaseHashFunctorA::KeyToHashIndex( const char* pcKey, unsigned int uiHashSize )
{
	unsigned int uiHash = 0;
	char ch = *pcKey;

	while( ch )
	{
		if( ch >= 'A' && ch <= 'Z' ) ch += ('a' - 'A');

		uiHash = (uiHash << 5) + uiHash + ch;

		ch = *(++pcKey);
	}
	return uiHash % uiHashSize;
}

inline bool NkStringMapNoCaseEqualsFunctorA::IsKeysEqual( const char* pcKey1, const char* pcKey2 )
{
	return _stricmp( pcKey1, pcKey2 ) == 0;
}


template <class TData, class THash = NkStringMapNoCaseHashFunctorA, class TEquals = NkStringMapNoCaseEqualsFunctorA>
class NkStringMapNoCaseA : public NkStringMapA<TData, THash, TEquals>
{
public:
	NkStringMapNoCaseA( unsigned int uiHashSize = 37, bool bCopy = true ) : NkStringMapA( uiHashSize, bCopy ) {}
};

#endif