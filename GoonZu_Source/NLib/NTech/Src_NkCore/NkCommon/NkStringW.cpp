//-------------------------------------------------------------------------------------------------
// NkStringW.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "NkString.h"

//-------------------------------------------------------------------------------------------------
// Constructors
//-------------------------------------------------------------------------------------------------

NkStringW::NkStringW() 
: m_pcString(0), m_cchLength(0)
{
}

NkStringW::NkStringW(const NkStringW& kStr)
: m_pcString(0), m_cchLength(0)
{
	_Allocate(kStr);
}

NkStringW::NkStringW(const wchar_t* pcStr)
: m_pcString(0), m_cchLength(0)
{
	_Allocate(pcStr);
}

NkStringW::NkStringW(const wchar_t ch)
: m_pcString(0), m_cchLength(0)
{
	_Allocate(ch);
}

NkStringW::~NkStringW()
{
	_Deallocate();
}

//-------------------------------------------------------------------------------------------------
// Allocate Functions
//-------------------------------------------------------------------------------------------------

void NkStringW::Allocate(size_t stSize)
{
	if( m_pcString )
		NkFree(m_pcString);

	if( stSize == 0 )
	{
		m_pcString = 0;
		m_cchLength = 0;
		return;
	}

	m_pcString = NkAlloc(wchar_t, stSize);

	m_cchLength = stSize - 1;
	m_pcString[m_cchLength] = '\0';
}

void NkStringW::_Allocate(const NkStringW& kStr)
{
	if( m_pcString )
		NkFree(m_pcString);

	size_t stLength = kStr.m_cchLength;

	if( stLength == 0 )
	{
		m_pcString = 0;
		m_cchLength = 0;
		return;
	}

	m_pcString = NkAlloc(wchar_t, stLength + 1);

	wmemcpy(m_pcString, kStr.m_pcString, stLength + 1);

	m_cchLength = stLength;
}


void NkStringW::_Allocate(const wchar_t* pcStr)
{
	size_t stLength;
	StringCchLengthW(pcStr, 64 * 1024, &stLength);

	if( m_pcString )
		NkFree(m_pcString);

	m_pcString = NkAlloc(wchar_t, stLength + 1);

	wmemcpy(m_pcString, pcStr, stLength + 1);

	m_cchLength = stLength;
}

void NkStringW::_Allocate(const wchar_t ch)
{
	if( m_pcString )
		NkFree(m_pcString);

	m_pcString = NkAlloc(wchar_t, 2);

	m_pcString[0] = ch;
	m_pcString[1] = L'\0';

	m_cchLength = 1;
}

void NkStringW::_Deallocate()
{
	if( m_pcString )
	{
		NkFree(m_pcString);
		m_pcString = 0;
		m_cchLength = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// Append Functions
//-------------------------------------------------------------------------------------------------

void NkStringW::Append(const NkStringW& kStr)
{
	size_t stLength;
	stLength = kStr.m_cchLength;

	if( stLength == 0 )
		return;

	if( m_cchLength == 0 )
	{
		_Allocate(kStr);
		return;
	}

	wchar_t* pcBuffer = NkAlloc(wchar_t, m_cchLength + stLength + 1);

	wmemcpy(pcBuffer, m_pcString, m_cchLength);

	wmemcpy(pcBuffer + m_cchLength, kStr.m_pcString, stLength + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength += stLength;
}

void NkStringW::Append(const wchar_t* pcStr)
{
	if( pcStr == NULL || pcStr[0] == L'\0' )
		return;

	if( m_cchLength == 0 )
	{
		_Allocate(pcStr);
		return;
	}

	size_t stLength;
	StringCchLengthW(pcStr, 64 * 1024, &stLength);

	wchar_t* pcBuffer;

	pcBuffer = NkAlloc(wchar_t, m_cchLength + stLength + 1);

	wmemcpy(pcBuffer, m_pcString, m_cchLength);

	wmemcpy(pcBuffer + m_cchLength, pcStr, stLength + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength += stLength;
}

void NkStringW::Append(const wchar_t ch)
{
	if( m_cchLength == 0 )
	{
		_Allocate(ch);
		return;
	}

	wchar_t* pcBuffer = NkAlloc(wchar_t, m_cchLength + 2);

	wmemcpy(pcBuffer, m_pcString, m_cchLength);

	pcBuffer[ m_cchLength ] = ch;
	pcBuffer[ m_cchLength + 1 ] = L'\0';

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	++m_cchLength;
}

//-------------------------------------------------------------------------------------------------
// Insert, Replace, RemoveRange
//-------------------------------------------------------------------------------------------------

void NkStringW::Insert(const wchar_t* pcStr, size_t stBegin)
{
	if( pcStr == NULL || pcStr[0] == NULL )
		return;

	if( stBegin >= m_cchLength )
	{
		Append(pcStr);
		return;
	}

	size_t stInsertLength;
	StringCchLengthW(pcStr, 64 * 1024, &stInsertLength);

	size_t stNewLength = m_cchLength + stInsertLength;

	wchar_t* pcBuffer = NkAlloc(wchar_t, stNewLength + 1);
	wchar_t* pcCurPos = pcBuffer;

	if( stBegin != 0 )
	{
		wmemcpy(pcCurPos, m_pcString, stBegin);
		pcCurPos += stBegin;
	}

	wmemcpy(pcCurPos, pcStr, stInsertLength);
	pcCurPos += stInsertLength;

	wmemcpy(pcCurPos, m_pcString + stBegin, m_cchLength - stBegin + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength = stNewLength;
}

size_t NkStringW::Replace(const wchar_t* pcOriginalStr, const wchar_t* pcReplaceStr)
{
	if( pcOriginalStr == NULL || pcReplaceStr == NULL || 
		pcOriginalStr[0] == L'\0' || pcReplaceStr[0] == L'\0' )
		return 0;

	size_t stOriginalLength;
	size_t stReplaceLength;

	StringCchLengthW(pcOriginalStr, 1024, &stOriginalLength);
	StringCchLengthW(pcReplaceStr, 1024, &stReplaceLength);

	size_t astOccurancePos[1024];
	size_t stOccuranceCount = 0;

	wchar_t* pcSubStr;

	size_t stPos = 0;
	while( stPos < m_cchLength )
	{
		pcSubStr = wcsstr(m_pcString + stPos, pcOriginalStr);

		if( pcSubStr == NULL )
			break;

		stPos = pcSubStr - m_pcString;

		if( stOccuranceCount < 1024 )
			astOccurancePos[ stOccuranceCount++ ] = stPos;

		stPos += stReplaceLength;
	}

	if( stOccuranceCount == 0 )
		return 0;

	size_t stTotalCharsOriginal = stOccuranceCount * stOriginalLength;
	size_t stTotalCharsReplace = stOccuranceCount * stReplaceLength;
	size_t stNewLength = m_cchLength - stTotalCharsOriginal + stTotalCharsReplace;

	wchar_t* pcBuffer = NkAlloc(wchar_t, stNewLength + 1);
	wchar_t* pcNewCurPos = pcBuffer;
	wchar_t* pcOldCurPos = m_pcString;

	size_t stCopySize;

	for( size_t st = 0; st < stOccuranceCount; ++st )
	{
		if( st < 1024 )
		{
			stCopySize = m_pcString + astOccurancePos[ st ] - pcOldCurPos;
		}
		else
		{
			pcSubStr = wcsstr(pcOldCurPos, pcOriginalStr);

			if( pcSubStr == NULL ) 
				NkAssert(pcSubStr != NULL);

			stCopySize = pcSubStr - pcOldCurPos;
		}

		if( stCopySize > 0 )
		{
			wmemcpy(pcNewCurPos, pcOldCurPos, stCopySize);
			pcNewCurPos += stCopySize;
			pcOldCurPos += (stCopySize + stOriginalLength);
		}
		else
		{
			pcOldCurPos += stOriginalLength;
		}

		wmemcpy(pcNewCurPos, pcReplaceStr, stReplaceLength);
		pcNewCurPos += stReplaceLength;
	}

	stCopySize = m_cchLength - (pcOldCurPos - m_pcString) + 1;

	if( stCopySize <= 0 )
		NkAssert(stCopySize > 0);

	wmemcpy(pcNewCurPos, pcOldCurPos, stCopySize);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength = stNewLength;

	return stOccuranceCount;
}

size_t NkStringW::RemoveRange(size_t stBegin, size_t stCount)
{
	if( stBegin >= m_cchLength )
		return 0;

	size_t stEnd = stBegin + stCount;

	if( stEnd > m_cchLength )
	{
		stEnd = m_cchLength;
		stCount = stEnd - stBegin;
	}

	size_t stNewLength = m_cchLength - stCount;

	wchar_t* pcBuffer = NkAlloc(wchar_t, stNewLength + 1);

	if( stBegin > 0 )
	{
		wmemcpy(pcBuffer, m_pcString, stBegin);
	}

	wmemcpy(pcBuffer + stBegin, m_pcString + stEnd, m_cchLength - stEnd + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength = stNewLength;

	return stCount;
}

//-------------------------------------------------------------------------------------------------
// Find Functions
//-------------------------------------------------------------------------------------------------

size_t NkStringW::FindChar(const wchar_t ch, size_t stStart) const
{
	if( stStart >= m_cchLength || ch == L'\0' )
		return INVALID_INDEX;

	wchar_t* pcSubStr = wcschr(m_pcString + stStart, ch);

	if( pcSubStr == NULL )
		return INVALID_INDEX;

	return pcSubStr - m_pcString;
}

size_t NkStringW::FindStr(const wchar_t* pcStr, size_t stStart) const
{
	if( stStart >= m_cchLength || pcStr == NULL || pcStr[0] == L'\0' )
		return INVALID_INDEX;

	wchar_t* pcSubStr = wcsstr(m_pcString + stStart, pcStr);

	if( pcSubStr == NULL )
		return INVALID_INDEX;

	return pcSubStr - m_pcString;	
}

size_t NkStringW::FindOneOf(const wchar_t* pcCharSet, size_t stStart) const
{
	if( stStart >= m_cchLength || pcCharSet == NULL || pcCharSet[0] == L'\0' )
		return INVALID_INDEX;

	wchar_t* pcSubStr = wcspbrk(m_pcString + stStart, pcCharSet);

	if( pcSubStr == NULL )
		return INVALID_INDEX;

	return 0;
}

//-------------------------------------------------------------------------------------------------
// Find Reverse Functions
//-------------------------------------------------------------------------------------------------

size_t NkStringW::FindReverseChar(const wchar_t ch, size_t stStart) const
{
	if( m_cchLength == 0 || ch == L'\0' )
		return INVALID_INDEX;

	if( stStart >= m_cchLength )
		stStart = m_cchLength - 1;

	wchar_t* pcString = m_pcString;

	while( stStart >= 0 )
	{
		if( pcString[ stStart ] == ch )
			return stStart;

		--stStart;
	}

	return INVALID_INDEX;
}

size_t NkStringW::FindReverseStr(const wchar_t* pcStr, size_t stStart) const
{
	if( m_cchLength == 0 || pcStr == NULL || pcStr[0] == L'\0' )
		return INVALID_INDEX;

	if( stStart >= m_cchLength )
		stStart = m_cchLength - 1;

	size_t stLength;
	StringCchLengthW(pcStr, 1024, &stLength);

	if( stLength > stStart + 1 )
	{
		return INVALID_INDEX;
	}

	wchar_t* pcString = m_pcString;

	stStart = stStart - stLength + 1;

	while( stStart >= 0 )
	{
		if( wcsncmp(pcString + stStart, pcStr, stLength) == 0 )
			return stStart;

		--stStart;
	}

	return INVALID_INDEX;
}

size_t NkStringW::FindReverseOneOf(const wchar_t* pcCharSet, size_t stStart) const
{
	if( m_cchLength == 0 || pcCharSet == NULL || pcCharSet[0] == L'\0' )
		return INVALID_INDEX;

	if( stStart >= m_cchLength )
		stStart = m_cchLength - 1;

	wchar_t* pcString = m_pcString;

	while( stStart >= 0 )
	{
		if( wcschr(pcCharSet, pcString[ stStart ]) != NULL )
			return stStart;

		--stStart;
	}

	return INVALID_INDEX;
}

//-------------------------------------------------------------------------------------------------
// Trim Functions
//-------------------------------------------------------------------------------------------------

void NkStringW::TrimLeft(wchar_t ch)
{
	if( ch == L'\0' )
		return;

	wchar_t* pcString = m_pcString;

	size_t stCount = 0;
	while( stCount < m_cchLength && pcString[ stCount ] == ch )
		++stCount;

	if( stCount > 0 )
	{
		size_t stNewLength = m_cchLength - stCount;

		wchar_t* pcBuffer = NkAlloc(wchar_t, stNewLength + 1);

		wmemcpy(pcBuffer, m_pcString + stCount, stNewLength + 1);

		NkFree(m_pcString);

		m_pcString = pcBuffer;
		m_cchLength -= stCount;
	}
}

void NkStringW::TrimRight(wchar_t ch)
{
	if( ch == L'\0' )
		return;

	wchar_t* pcString = m_pcString;

	size_t stNewLength = m_cchLength;
	while( stNewLength > 0 && pcString[ stNewLength - 1 ] == ch )
		--stNewLength;

	if( stNewLength < m_cchLength )
	{
		wchar_t* pcBuffer = NkAlloc(wchar_t, stNewLength + 1);

		wmemcpy(pcBuffer, m_pcString, stNewLength);
		pcBuffer[ stNewLength ] = L'\0';

		NkFree(m_pcString);

		m_pcString = pcBuffer;
		m_cchLength = stNewLength;
	}
}

//-------------------------------------------------------------------------------------------------
// Reverse, ToUpper, ToLower
//-------------------------------------------------------------------------------------------------

void NkStringW::Reverse()
{
	if( m_cchLength < 2 )
		return;

	size_t stEnd = m_cchLength - 1;

	wchar_t* pcString = m_pcString;

	wchar_t ch;
	for( size_t stBegin = 0; stBegin < stEnd; ++stBegin, --stEnd )
	{
		ch = pcString[ stBegin ];
		pcString[ stBegin ] = pcString[ stEnd ];
		pcString[ stEnd ] = ch;
	}
}


void NkStringW::ToUpper()
{
	wchar_t ch;
	for( size_t st = 0; st < m_cchLength; ++st )
	{
		ch = m_pcString[st];
		if( (L'a' <= ch) && (ch <= L'z') )
			m_pcString[st] -= (L'a' - L'A');
	}
}

void NkStringW::ToLower()
{
	wchar_t ch;
	for( size_t st = 0; st < m_cchLength; ++st )
	{
		ch = m_pcString[st];
		if( (L'A' <= ch) && (ch <= L'Z') )
			m_pcString[st] += (L'a' - L'A');
	}
}


NkStringW NkStringW::Mid(size_t stBegin, size_t stCount) const
{
	NkStringW kString;

	size_t stEnd = stBegin + stCount;

	if( stEnd > m_cchLength )
	{
		stEnd = m_cchLength;
	}

	if( stBegin < stEnd )
	{
		size_t stLength = stEnd - stBegin;

		kString.m_pcString = NkAlloc( wchar_t, stLength + 1 );
		kString.m_cchLength = stLength;

		wmemcpy( kString.m_pcString, m_pcString + stBegin, stLength );
		kString.m_pcString[ stLength ] = L'\0';
	}

	return kString;
}

