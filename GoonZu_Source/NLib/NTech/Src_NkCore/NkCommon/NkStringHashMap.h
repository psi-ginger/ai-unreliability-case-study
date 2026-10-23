//-------------------------------------------------------------------------------------------------
// NkStringHashMap.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkAlgorithm.h"
#include "NkHashMap.h"

//=================================================================================================
// NkStringHashMapA
//=================================================================================================

//-----------------------------------------------------------------------------
// Hash Functor
//-----------------------------------------------------------------------------
class NkStringHashMapHashA
{
public:

	inline static unsigned int KeyToHashIndex(char* pcKey, unsigned int uiHashSize)
	{
		unsigned int uiHash = 0;

		while( *pcKey )
		{
			uiHash = (uiHash << 5) + uiHash + *pcKey++;
		}
		return uiHash % uiHashSize;
	}
};

//-----------------------------------------------------------------------------
// Equals Functor
//-----------------------------------------------------------------------------
class NkStringHashMapEqualsA
{
public:

	inline static bool Equals(char* a, char* b)
	{
		return strcmp(a, b) == 0;
	}
};

//-----------------------------------------------------------------------------
// Data Setter
//-----------------------------------------------------------------------------
template <class T>
class NkStringHashMapA_DataSetter
{
public:
	static inline void SetData(char*& destKey, T& destData, char* srcKey, const T& srcData) 
	{
		size_t size = strlen(srcKey) + 1;
		destKey = NkAlloc(char, size);
		memcpy(destKey, srcKey, size);

		destData = srcData;
	}

	static inline void DeleteData(char*& key, T& /*data*/)
	{
		NkFree(key);
	}
};


template <class TData, 
		  class THash = NkStringHashMapHashA, 
		  class TEquals = NkStringHashMapEqualsA,
		  class TDataSetter = NkStringHashMapA_DataSetter<TData> >
class NkStringHashMapA : public NkHashMap<char*, TData, THash, TEquals, TDataSetter>
{
public:
	NkStringHashMapA(unsigned int uiHashSize = 37) 
	{
		m_uiHashSize = uiHashSize;
		m_uiCount = 0;

		m_pkHashTable = NkNew NkVector<NodePtr>(uiHashSize);
	}
};


//=================================================================================================
// NkStringHashMapNoCaseA
//=================================================================================================

//-----------------------------------------------------------------------------
// Hash Functor
//-----------------------------------------------------------------------------
class NkStringHashMapNoCaseHashA
{
public:

	inline static unsigned int KeyToHashIndex(char* pcKey, unsigned int uiHashSize)
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
};

//-----------------------------------------------------------------------------
// Equals Functor
//-----------------------------------------------------------------------------
class NkStringHashMapNoCaseEqualsA
{
public:

	inline static bool Equals(char* a, char* b)
	{
		return _stricmp(a, b) == 0;
	}
};


template <class TData, 
		  class THash = NkStringHashMapNoCaseHashA, 
		  class TEquals = NkStringHashMapNoCaseEqualsA,
		  class TDataSetter = NkStringHashMapA_DataSetter<TData> >
class NkStringHashMapNoCaseA : public NkHashMap<TData, THash, TEquals, TDataSetter>
{
public:
	NkStringHashMapNoCaseA(unsigned int uiHashSize = 37) 
	{
		m_uiHashSize = uiHashSize;
		m_uiCount = 0;

		m_pkHashTable = NkNew NkVector<NodePtr>(uiHashSize);
	}
};

//=================================================================================================
// NkStringHashMapW
//=================================================================================================

//-----------------------------------------------------------------------------
// Hash Functor
//-----------------------------------------------------------------------------
class NkStringHashMapHashW
{
public:

	inline static unsigned int KeyToHashIndex(wchar_t* pcKey, unsigned int uiHashSize)
	{
		unsigned int uiHash = 0;

		while( *pcKey )
		{
			uiHash = (uiHash << 5) + uiHash + *pcKey++;
		}
		return uiHash % uiHashSize;
	}
};

//-----------------------------------------------------------------------------
// Equals Functor
//-----------------------------------------------------------------------------
class NkStringHashMapEqualsW
{
public:

	inline static bool Equals(wchar_t* a, wchar_t* b)
	{
		return wcscmp(a, b) == 0;
	}
};

//-----------------------------------------------------------------------------
// Data Setter
//-----------------------------------------------------------------------------
template <class T>
class NkStringHashMapW_DataSetter
{
public:
	static inline void SetData(wchar_t*& destKey, T& destData, wchar_t* srcKey, const T& srcData) 
	{
		size_t size = wcslen(srcKey) + 1;
		destKey = NkAlloc(wchar_t, size);
		wmemcpy(destKey, srcKey, size);

		destData = srcData;
	}

	static inline void DeleteData(wchar_t*& key, T& /*data*/)
	{
		NkFree(key);
	}
};



template <class TData, 
		  class THash = NkStringHashMapHashW, 
		  class TEquals = NkStringHashMapEqualsW,
		  class TDataSetter = NkStringHashMapW_DataSetter<TData> >
class NkStringHashMapW : public NkHashMap<wchar_t*, TData, THash, TEquals, TDataSetter>
{
public:
	NkStringHashMapW(unsigned int uiHashSize = 37) 
	{
		m_uiHashSize = uiHashSize;
		m_uiCount = 0;

		m_pkHashTable = NkNew NkVector<NodePtr>(uiHashSize);
	}
};



//=================================================================================================
// NkStringHashMapNoCaseW
//=================================================================================================

//-----------------------------------------------------------------------------
// Hash Functor
//-----------------------------------------------------------------------------
class NkStringHashMapNoCaseHashW
{
public:

	inline static unsigned int KeyToHashIndex(wchar_t* pcKey, unsigned int uiHashSize)
	{
		unsigned int uiHash = 0;

		wchar_t ch = *pcKey;
		while( ch )
		{
			if( ch >= L'A' && ch <= L'Z' ) ch += (L'a' - L'A');

			uiHash = (uiHash << 5) + uiHash + ch;

			ch = *(++pcKey);
		}
		return uiHash % uiHashSize;
	}
};

//-----------------------------------------------------------------------------
// Equals Functor
//-----------------------------------------------------------------------------
class NkStringHashMapNoCaseEqualsW
{
public:
	inline static bool Equals(wchar_t* a, wchar_t* b)
	{
		return _wcsicmp(a, b) == 0;
	}
};


template <class TData, 
		  class THash = NkStringHashMapNoCaseHashW, 
		  class TEquals = NkStringHashMapNoCaseEqualsW,
		  class TDataSetter = NkStringHashMapW_DataSetter<TData> >
class NkStringHashMapNoCaseW : public NkHashMap<TData, THash, TEquals, TDataSetter>
{
public:
	NkStringHashMapNoCaseW(unsigned int uiHashSize = 37) 
	{
		m_uiHashSize = uiHashSize;
		m_uiCount = 0;

		m_pkHashTable = NkNew NkVector<NodePtr>(uiHashSize);
	}
};
