//-------------------------------------------------------------------------------------------------
// NkStringA.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "NkString.h"

//-------------------------------------------------------------------------------------------------
// Constructors
//-------------------------------------------------------------------------------------------------

NkStringA::NkStringA() 
: m_pcString(0), m_cchLength(0)
{
}

NkStringA::NkStringA(const NkStringA& kStr)
: m_pcString(0), m_cchLength(0)
{
	_Allocate(kStr);
}

NkStringA::NkStringA(const char* pcStr)
: m_pcString(0), m_cchLength(0)
{
	_Allocate(pcStr);
}

NkStringA::NkStringA(const char ch)
: m_pcString(0), m_cchLength(0)
{
	_Allocate(ch);
}

NkStringA::~NkStringA()
{
	_Deallocate();
}

//-------------------------------------------------------------------------------------------------
// Allocate Functions
//-------------------------------------------------------------------------------------------------

void NkStringA::Allocate(size_t stSize)
{
	if( m_pcString )
		NkFree(m_pcString);

	if( stSize == 0 )
	{
		m_pcString = 0;
		m_cchLength = 0;
		return;
	}

	m_pcString = NkAlloc(char, stSize);
	
	m_cchLength = stSize - 1;
	m_pcString[m_cchLength] = '\0';
}

void NkStringA::_Allocate(const NkStringA& kStr)
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

	m_pcString = NkAlloc(char, stLength + 1);

	memcpy(m_pcString, kStr.m_pcString, stLength + 1);

	m_cchLength = stLength;
}


void NkStringA::_Allocate(const char* pcStr)
{
	size_t stLength;
	StringCchLengthA(pcStr, 64 * 1024, &stLength);

	if( m_pcString )
		NkFree(m_pcString);

	m_pcString = NkAlloc(char, stLength + 1);

	memcpy(m_pcString, pcStr, stLength + 1);

	m_cchLength = stLength;
}

void NkStringA::_Allocate(const char ch)
{
	if( m_pcString )
		NkFree(m_pcString);

	m_pcString = NkAlloc(char, 2);

	m_pcString[0] = ch;
	m_pcString[1] = '\0';

	m_cchLength = 1;
}

void NkStringA::_Deallocate()
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

void NkStringA::Append(const NkStringA& kStr)
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

	char* pcBuffer = NkAlloc(char, m_cchLength + stLength + 1);

	memcpy(pcBuffer, m_pcString, m_cchLength);

	memcpy(pcBuffer + m_cchLength, kStr.m_pcString, stLength + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength += stLength;
}

void NkStringA::Append(const char* pcStr)
{
	if( pcStr == NULL || pcStr[0] == '\0' )
		return;

	if( m_cchLength == 0 )
	{
		_Allocate(pcStr);
		return;
	}

	size_t stLength;
	StringCchLengthA(pcStr, 64 * 1024, &stLength);

	char* pcBuffer;

	pcBuffer = NkAlloc(char, m_cchLength + stLength + 1);

	memcpy(pcBuffer, m_pcString, m_cchLength);

	memcpy(pcBuffer + m_cchLength, pcStr, stLength + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength += stLength;
}

void NkStringA::Append(const char ch)
{
	if( m_cchLength == 0 )
	{
		_Allocate(ch);
		return;
	}

	char* pcBuffer = NkAlloc(char, m_cchLength + 2);

	memcpy(pcBuffer, m_pcString, m_cchLength);

	pcBuffer[ m_cchLength ] = ch;
	pcBuffer[ m_cchLength + 1 ] = '\0';

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	++m_cchLength;
}

//-------------------------------------------------------------------------------------------------
// Insert, Replace, RemoveRange
//-------------------------------------------------------------------------------------------------

void NkStringA::Insert(const char* pcStr, size_t stBegin)
{
	if( pcStr == NULL || pcStr[0] == NULL )
		return;

	if( stBegin >= m_cchLength )
	{
		Append(pcStr);
		return;
	}

	size_t stInsertLength;
	StringCchLengthA(pcStr, 64 * 1024, &stInsertLength);

	size_t stNewLength = m_cchLength + stInsertLength;

	char* pcBuffer = NkAlloc(char, stNewLength + 1);
	char* pcCurPos = pcBuffer;

	if( stBegin != 0 )
	{
		memcpy(pcCurPos, m_pcString, stBegin);
		pcCurPos += stBegin;
	}

	memcpy(pcCurPos, pcStr, stInsertLength);
	pcCurPos += stInsertLength;

	memcpy(pcCurPos, m_pcString + stBegin, m_cchLength - stBegin + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength = stNewLength;
}

size_t NkStringA::Replace(const char* pcOriginalStr, const char* pcReplaceStr)
{
	if( pcOriginalStr == NULL || pcReplaceStr == NULL || 
		pcOriginalStr[0] == '\0' || pcReplaceStr[0] == '\0' )
		return 0;

	size_t stOriginalLength;
	size_t stReplaceLength;

	StringCchLengthA(pcOriginalStr, 1024, &stOriginalLength);
	StringCchLengthA(pcReplaceStr, 1024, &stReplaceLength);

	size_t astOccurancePos[1024];
	size_t stOccuranceCount = 0;

	char* pcSubStr;

	size_t stPos = 0;
	while( stPos < m_cchLength )
	{
		pcSubStr = strstr(m_pcString + stPos, pcOriginalStr);

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

	char* pcBuffer = NkAlloc(char, stNewLength + 1);
	char* pcNewCurPos = pcBuffer;
	char* pcOldCurPos = m_pcString;

	size_t stCopySize;

	for( size_t st = 0; st < stOccuranceCount; ++st )
	{
		if( st < 1024 )
		{
			stCopySize = m_pcString + astOccurancePos[ st ] - pcOldCurPos;
		}
		else
		{
			pcSubStr = strstr(pcOldCurPos, pcOriginalStr);

			if( pcSubStr == NULL ) 
				NkAssert(pcSubStr != NULL);

			stCopySize = pcSubStr - pcOldCurPos;
		}

		if( stCopySize > 0 )
		{
			memcpy(pcNewCurPos, pcOldCurPos, stCopySize);
			pcNewCurPos += stCopySize;
			pcOldCurPos += (stCopySize + stOriginalLength);
		}
		else
		{
			pcOldCurPos += stOriginalLength;
		}

		memcpy(pcNewCurPos, pcReplaceStr, stReplaceLength);
		pcNewCurPos += stReplaceLength;
	}

	stCopySize = m_cchLength - (pcOldCurPos - m_pcString) + 1;

	if( stCopySize <= 0 )
		NkAssert(stCopySize > 0);

	memcpy(pcNewCurPos, pcOldCurPos, stCopySize);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength = stNewLength;

	return stOccuranceCount;
}

size_t NkStringA::RemoveRange(size_t stBegin, size_t stCount)
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

	char* pcBuffer = NkAlloc(char, stNewLength + 1);

	if( stBegin > 0 )
	{
		memcpy(pcBuffer, m_pcString, stBegin);
	}

	memcpy(pcBuffer + stBegin, m_pcString + stEnd, m_cchLength - stEnd + 1);

	NkFree(m_pcString);

	m_pcString = pcBuffer;
	m_cchLength = stNewLength;

	return stCount;
}

//-------------------------------------------------------------------------------------------------
// Find Functions
//-------------------------------------------------------------------------------------------------

size_t NkStringA::FindChar(const char ch, size_t stStart) const
{
	if( stStart >= m_cchLength || ch == '\0' )
		return INVALID_INDEX;

	char* pcSubStr = strchr(m_pcString + stStart, ch);

	if( pcSubStr == NULL )
		return INVALID_INDEX;

	return pcSubStr - m_pcString;
}

size_t NkStringA::FindStr(const char* pcStr, size_t stStart) const
{
	if( stStart >= m_cchLength || pcStr == NULL || pcStr[0] == '\0' )
		return INVALID_INDEX;

	char* pcSubStr = strstr(m_pcString + stStart, pcStr);

	if( pcSubStr == NULL )
		return INVALID_INDEX;

	return pcSubStr - m_pcString;	
}

size_t NkStringA::FindOneOf(const char* pcCharSet, size_t stStart) const
{
	if( stStart >= m_cchLength || pcCharSet == NULL || pcCharSet[0] == '\0' )
		return INVALID_INDEX;

	char* pcSubStr = strpbrk(m_pcString + stStart, pcCharSet);

	if( pcSubStr == NULL )
		return INVALID_INDEX;

	return 0;
}

//-------------------------------------------------------------------------------------------------
// Find Reverse Functions
//-------------------------------------------------------------------------------------------------

size_t NkStringA::FindReverseChar(const char ch, size_t stStart) const
{
	if( m_cchLength == 0 || ch == '\0' )
		return INVALID_INDEX;

	if( stStart >= m_cchLength )
		stStart = m_cchLength - 1;

	char* pcString = m_pcString;
	
	while( stStart >= 0 )
	{
		if( pcString[ stStart ] == ch )
			return stStart;

		--stStart;
	}

	return INVALID_INDEX;
}

size_t NkStringA::FindReverseStr(const char* pcStr, size_t stStart) const
{
	if( m_cchLength == 0 || pcStr == NULL || pcStr[0] == '\0' )
		return INVALID_INDEX;

	if( stStart >= m_cchLength )
		stStart = m_cchLength - 1;

	size_t stLength;
	StringCchLengthA(pcStr, 1024, &stLength);

	if( stLength > stStart + 1 )
	{
		return INVALID_INDEX;
	}

	char* pcString = m_pcString;

	stStart = stStart - stLength + 1;

	while( stStart >= 0 )
	{
		if( strncmp(pcString + stStart, pcStr, stLength) == 0 )
			return stStart;

		--stStart;
	}

	return INVALID_INDEX;
}

size_t NkStringA::FindReverseOneOf(const char* pcCharSet, size_t stStart) const
{
	if( m_cchLength == 0 || pcCharSet == NULL || pcCharSet[0] == '\0' )
		return INVALID_INDEX;

	if( stStart >= m_cchLength )
		stStart = m_cchLength - 1;

	char* pcString = m_pcString;

	while( stStart >= 0 )
	{
		if( strchr(pcCharSet, pcString[ stStart ]) != NULL )
			return stStart;

		--stStart;
	}

	return INVALID_INDEX;
}

//-------------------------------------------------------------------------------------------------
// Trim Functions
//-------------------------------------------------------------------------------------------------

void NkStringA::TrimLeft(char ch)
{
	if( ch == '\0' )
		return;

	char* pcString = m_pcString;

	size_t stCount = 0;
	while( stCount < m_cchLength && pcString[ stCount ] == ch )
		++stCount;

	if( stCount > 0 )
	{
		size_t stNewLength = m_cchLength - stCount;

		char* pcBuffer = NkAlloc(char, stNewLength + 1);

		memcpy(pcBuffer, m_pcString + stCount, stNewLength + 1);

		NkFree(m_pcString);

		m_pcString = pcBuffer;
		m_cchLength -= stCount;
	}
}

void NkStringA::TrimRight(char ch)
{
	if( ch == '\0' )
		return;

	char* pcString = m_pcString;

	size_t stNewLength = m_cchLength;
	while( stNewLength > 0 && pcString[ stNewLength - 1 ] == ch )
		--stNewLength;

	if( stNewLength < m_cchLength )
	{
		char* pcBuffer = NkAlloc(char, stNewLength + 1);

		memcpy(pcBuffer, m_pcString, stNewLength);
		pcBuffer[ stNewLength ] = '\0';

		NkFree(m_pcString);

		m_pcString = pcBuffer;
		m_cchLength = stNewLength;
	}
}

//-------------------------------------------------------------------------------------------------
// Reverse, ToUpper, ToLower
//-------------------------------------------------------------------------------------------------

void NkStringA::Reverse()
{
	if( m_cchLength < 2 )
		return;

	size_t stEnd = m_cchLength - 1;

	char* pcString = m_pcString;

	char ch;
	for( size_t stBegin = 0; stBegin < stEnd; ++stBegin, --stEnd )
	{
		ch = pcString[ stBegin ];
		pcString[ stBegin ] = pcString[ stEnd ];
		pcString[ stEnd ] = ch;
	}
}


void NkStringA::ToUpper()
{
	char ch;
	for( size_t st = 0; st < m_cchLength; ++st )
	{
		ch = m_pcString[st];
		if( ('a' <= ch) && (ch <= 'z') )
			m_pcString[st] -= ('a' - 'A');
	}
}

void NkStringA::ToLower()
{
	char ch;
	for( size_t st = 0; st < m_cchLength; ++st )
	{
		ch = m_pcString[st];
		if( ('A' <= ch) && (ch <= 'Z') )
			m_pcString[st] += ('a' - 'A');
	}
}


NkStringA NkStringA::Mid(size_t stBegin, size_t stCount) const
{
	NkStringA kString;

	size_t stEnd = stBegin + stCount;

	if( stEnd > m_cchLength )
	{
		stEnd = m_cchLength;
	}

	if( stBegin < stEnd )
	{
		size_t stLength = stEnd - stBegin;

		kString.m_pcString = NkAlloc( char, stLength + 1 );
		kString.m_cchLength = stLength;

		memcpy( kString.m_pcString, m_pcString + stBegin, stLength );
		kString.m_pcString[ stLength ] = '\0';
	}

	return kString;
}

