//-------------------------------------------------------------------------------------------------
// NkAlgorithm.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

template <class _IterType, class _FuncType> inline
_FuncType NkForEach(_IterType _Begin, _IterType _End, _FuncType _Func)
{
	for( _IterType::_Iter _Iter = _Begin; _Iter != _End; ++_Iter)
		_Func(*_Iter);

	return (_Func);
}

template <class _IterType, class _FuncType> inline
_FuncType NkForEachMap(_IterType _Begin, _IterType _End, _FuncType _Func)
{
	for( _IterType::_Iter _Iter = _Begin; _Iter != _End; ++_Iter)
		_Func(_Iter->GetKey(), _Iter->GetData());

	return (_Func);
}


//-------------------------------------------------------------------------------------------------
// Functors 
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Compare Functor 
//-------------------------------------------------------------------------------------------------
template <class T>
class NkCompareFunctor
{
public:
	inline static bool Greater(const T& a, const T& b)
	{
		return a > b;
	}

	inline static bool Equals(const T& a, const T& b)
	{
		return a == b;
	}
};

//-------------------------------------------------------------------------------------------------
// Equals Functor
//-------------------------------------------------------------------------------------------------
template <class T>
class NkEqualsFunctor
{
public:
	inline static bool Equals(const T& a, const T& b)
	{
		return a == b;
	}
};

//-------------------------------------------------------------------------------------------------
// Hash Functor
//-------------------------------------------------------------------------------------------------
template <class T>
class NkHashFunctor
{
public:
	inline static unsigned int KeyToHashIndex(const T& key, unsigned int uiHashSize)
	{
		return key % uiHashSize;
	}
};

//-------------------------------------------------------------------------------------------------
// NkPairDataSetter
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData>
class NkPairDataSetter
{
public:
	static inline void SetData(TKey& destKey, TData& destData, const TKey& srcKey, const TData& srcData) 
	{
		destKey = srcKey;
		destData = srcData;
	}
	static inline void DeleteData(TKey& /*key*/, TData& /*data*/) {}
};

//-------------------------------------------------------------------------------------------------
// NkPairDataSetter
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData>
class NkPairPtrDataSetter
{
public:
	static inline void SetData(TKey& destKey, TData& destData, const TKey& srcKey, const TData& srcData) 
	{
		destKey = srcKey;
		destData = srcData;
	}
	static inline void DeleteData(TKey& /*key*/, TData& data)
	{
		NkDelete( data );
		data = 0;
	}
};



//-------------------------------------------------------------------------------------------------
// Sort
//-------------------------------------------------------------------------------------------------
template <class T>
class NkSort
{
public:
	static void QuickSort(T* pDatas, unsigned int count, int (*pCompareFunc)(T&, T&));

private:
	static void _QuickSortRecursive(T* p, T* r, int (*pCompareFunc)(T&, T&));

};

template <class T>
void NkSort<T>::QuickSort(T* pDatas, unsigned int count, int (*pCompareFunc)(T&, T&))
{
	_QuickSortRecursive(pDatas, &pDatas[count - 1], pCompareFunc);
}

template <class T>
void NkSort<T>::_QuickSortRecursive(T* p, T* r, int (*pCompareFunc)(T&, T&))
{
	while( p < r )
	{
		T* q = p - 1;
		{
			T* x = r;
			T temp;

			for( T* j = p; j < r; ++j )
			{
				if( pCompareFunc(*x, *j) > 0 )
				{
					++q;
					temp = *q; *q = *j; *j = temp;
				}
			}
			++q;
			temp = *q; *q = *r; *r = temp;
		}

		_QuickSortRecursive(p, q - 1, pCompareFunc);
		p = q + 1;
	}
}
