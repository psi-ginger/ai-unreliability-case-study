//-----------------------------------------------------------------------------
// 2007/12/12 이강형
//-----------------------------------------------------------------------------

#ifndef _NTCHARSTRING_H
#define _NTCHARSTRING_H

#include "NDataTypes.h"

template<int T_MAXSIZE>
class NTCHARString
{
private:
	TCHAR	m_szData[T_MAXSIZE];

public:
	NTCHARString()	{	memset(m_szData, 0, sizeof(m_szData));	}
	~NTCHARString()	{}

	NTCHARString(NTCHARString& kVal)	{	*this = kVal;		}
	NTCHARString(const TCHAR* pszVal)	{	*this = pszVal;		}
	NTCHARString(TCHAR* pszVal)			{	*this = pszVal;		}

	inline operator const TCHAR* ()	{	return (const TCHAR*)m_szData;	}
	inline operator TCHAR* ()		{	return m_szData;				}

	inline int MaxLength()			{	return T_MAXSIZE - 1;			}
	inline int Length()
	{
		int ret = (int)_tcslen(m_szData);
		if( ret >= T_MAXSIZE - 1 )		ret = T_MAXSIZE - 1;
		return ret;
	}

	inline NTCHARString<T_MAXSIZE>& operator = (const TCHAR* pszVal)
	{
		if(pszVal == NULL)		return *this;

		_tcsncpy(m_szData, pszVal, T_MAXSIZE - 1);
		m_szData[T_MAXSIZE-1] = '\0';

		return *this;
	}

	inline bool IsEmpty()
	{
		if(_tcslen(m_szData) == 0)
			return true;

		return false;
	}

	inline bool operator == (const TCHAR* pszVal)
	{
		if(pszVal == NULL)				return false;

		return _tcscmp(*this, pszVal) == 0;
	}

	inline int Compare(const TCHAR* pszVal)
	{
		if(pszVal == NULL)				return 1;
		return _tcscmp(m_szData, pszVal);
	}

	inline NTCHARString<T_MAXSIZE>& operator += (const TCHAR* pszVal)
	{
		if(pszVal == NULL)		return *this;

		if(T_MAXSIZE - Length() - 1 > 0)
			_tcsncat(m_szData, pszVal, T_MAXSIZE - Length() - 1);
		m_szData[T_MAXSIZE-1] = '\0';

		return *this;
	}

	inline NTCHARString<T_MAXSIZE>& operator += (const TCHAR cVal)
	{
		if(T_MAXSIZE - Length() - 1 > 0)
			m_szData[Length()] = cVal;
		m_szData[T_MAXSIZE-1] = '\0';

		return *this;
	}

	int Find(const TCHAR* pszVal, int Start = 0)
	{
		if(pszVal == NULL)			return -1;
		if(Start < 0)				return -1;
		if(Start > T_MAXSIZE - 1)	return -1;

		TCHAR* pFind = _tcsstr(&m_szData[Start], pszVal);
		if( pFind == NULL )			return -1;

		return (SI32)(pFind - m_szData);
	}

	int RFind(const TCHAR cVal)
	{
		TCHAR* pFind = _tcsrchr(m_szData, cVal);
		if( pFind == NULL )			return -1;

		return (SI32)(pFind - m_szData);
	}

	NTCHARString<T_MAXSIZE> SubStr(int Start, int Len = T_MAXSIZE)
	{
		NTCHARString<T_MAXSIZE>		Ret;

		if(Len == T_MAXSIZE)	{	Len = Length();		}

		if(Start >= T_MAXSIZE - 1)	return Ret;
		if(Start < 0)				return Ret;
		if(Len < 0)					return Ret;

		if( Len + Start >= T_MAXSIZE )		Len = T_MAXSIZE - Start - 1;
		if(Len > 0)
			_tcsncpy(Ret.m_szData, &m_szData[Start], Len);
		Ret.m_szData[ Len ] = '\0';

		return Ret;
	}

	void Replace(const TCHAR* pszSrc, const TCHAR* pszDest)
	{
		if(pszSrc == NULL)			return;
		if(pszDest == NULL)			return;

		NTCHARString<T_MAXSIZE>		tmp;

		int nFind = Find(pszSrc);
		if(nFind >= 0)
		{
			tmp = SubStr(0, nFind);
			tmp += pszDest;
			tmp += SubStr(nFind + (int)_tcslen(pszSrc));
		}
		else
		{
			tmp = *this;
		}

		*this = tmp;
	}

	void ReplaceAll(const TCHAR* pszSrc, const TCHAR* pszDest)
	{
		if(pszSrc == NULL)			return;
		if(pszDest == NULL)			return;

		NTCHARString<T_MAXSIZE>		tmp;

		int nStart = 0;
		int nFind = Find(pszSrc, nStart);
		while( nFind >= 0 )
		{
			tmp += SubStr(nStart, nFind - nStart);
			tmp += pszDest;

			nStart = nFind + (int)_tcslen(pszSrc);
			nFind = Find(pszSrc, nStart);
		}
		tmp += SubStr(nStart);

		*this = tmp;
	}

	// 양쪽에 있는 큰따옴표를 제거한다.
	void RemoveDoubleQuotation()
	{
		NTCHARString<T_MAXSIZE>		kTmp;		kTmp = *this;

		SI32 siStartPos = 0;
		SI32 siLen = kTmp.Length();

		if( kTmp[ 0 ] == '\"' )						{	siStartPos++;	siLen--;	}
		if( kTmp[ kTmp.Length() - 1 ] == '\"' )		{	siLen--;					}
        
		*this = kTmp.SubStr( siStartPos, siLen );
	}

	void GetModuleFileName(HMODULE hModule)
	{
		TCHAR szRet[T_MAXSIZE];
		::GetModuleFileName(hModule, szRet, T_MAXSIZE);
		
		*this = szRet;
	}

	// 이전 버젼 호환용 함수
	void FormatString(const TCHAR* pszFormat, ...)
	{
		va_list args;
		
		va_start( args, pszFormat );
		StringCchVPrintf(m_szData, T_MAXSIZE, pszFormat, args);

	}

	// 이전 버젼 호환용 함수
	char operator [] (int index) const
	{
		if( index < 0 && index >= T_MAXSIZE )		return 0;
		return m_szData[index];
	}
};

class NTCHARStringBuilder
{
private:
	NTCHARString<2048>		m_kStr;
	SI32					m_siCurParam;
public:
	NTCHARStringBuilder()	{}
	~NTCHARStringBuilder()	{}

	NTCHARStringBuilder& SetFormat(const TCHAR* pszFormat);
	NTCHARStringBuilder& operator<< (const TCHAR* pszParam);
	NTCHARStringBuilder& operator<< (TCHAR* pszParam);
	NTCHARStringBuilder& operator<< (SI32 siVal);
	NTCHARStringBuilder& operator<< (UI32 uiVal);
	NTCHARStringBuilder& operator<< (SI16 siVal);
	NTCHARStringBuilder& operator<< (UI16 uiVal);
	NTCHARStringBuilder& operator<< (SI08 siVal);
	NTCHARStringBuilder& operator<< (UI08 uiVal);
	NTCHARStringBuilder& operator<< (SI64 siVal);
	NTCHARStringBuilder& operator<< (UI64 uiVal);
	NTCHARStringBuilder& operator<< (REAL32 fVal);
	NTCHARStringBuilder& operator<< (REAL64 fVal);

	operator const TCHAR* ()	{		return m_kStr;		}
};

typedef NTCHARString<1024>			NTCHARString1024;
typedef NTCHARString<512>			NTCHARString512;
typedef NTCHARString<256>			NTCHARString256;
typedef NTCHARString<128>			NTCHARString128;
typedef NTCHARString<64>			NTCHARString64;
typedef NTCHARString<32>			NTCHARString32;
typedef NTCHARString<16>			NTCHARString16;

typedef NTCHARString<20>			NTCHARStringName;
typedef NTCHARString<MAX_PATH>		NTCHARStringPath;

NTCHARString64 SI64ToString( SI64 siVal, SI32 siRadix = 10 );
NTCHARString64 UI64ToString( UI64 uiVal, SI32 siRadix = 10 );
NTCHARString64 SI32ToString( SI32 siVal, SI32 siRadix = 10 );
NTCHARString64 UI32ToString( UI32 uiVal, SI32 siRadix = 10 );
NTCHARString64 SI16ToString( SI16 siVal, SI32 siRadix = 10 );
NTCHARString64 UI16ToString( UI16 uiVal, SI32 siRadix = 10 );
NTCHARString64 SI08ToString( SI08 siVal, SI32 siRadix = 10 );
NTCHARString64 UI08ToString( UI08 uiVal, SI32 siRadix = 10 );
NTCHARString64 REAL32ToString( REAL32 fVal, SI32 siNumber = -1, SI32 siDecimal = 2 );
NTCHARString64 REAL64ToString( REAL64 fVal, SI32 siNumber = -1, SI32 siDecimal = 2 );

#endif
