//-------------------------------------------------------------------------------------------------
// NkCharsA.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <size_t size>
NkCharsA<size>::NkCharsA()
{
	m_str[0] = '\0';
}

template <size_t size>
NkCharsA<size>::NkCharsA(const char* pcStr)
{
	NkAssert(size > 1);

	StringCchCopyA(m_str, size, pcStr);
}

template <size_t size>
NkCharsA<size>::NkCharsA(char ch)
{
	NkAssert(size > 1);

	m_str[0] = ch;
	m_str[1] = '\0';
}

template <size_t size> inline 
bool NkCharsA<size>::IsEmpty() const
{
	return m_str[0] != 0;
}

template <size_t size> inline 
void NkCharsA<size>::Empty()
{
	m_str[0] = '\0';
}

template <size_t size> inline
void NkCharsA<size>::ZeroBuffer()
{
	memset(m_str, 0, size);
}

template <size_t size> inline
size_t NkCharsA<size>::GetMaxCchSize() const
{
	return size;
}

template <size_t size> inline
size_t NkCharsA<size>::GetMaxByteSize() const
{
	return sizeof(m_str);
}

template <size_t size> inline
size_t NkCharsA<size>::GetLength() const
{
	size_t stLength;
	StringCchLengthA(m_str, size, &stLength);
	return stLength;
}

template <size_t size> inline
const char* NkCharsA<size>::GetString() const
{
	return m_str;
}

template <size_t size> inline
char NkCharsA<size>::GetAt(size_t stPos) const
{
	if( stPos >= size )
		return '\0';

	return m_str[stPos];
}

template <size_t size> inline
void NkCharsA<size>::SetAt(size_t stPos, char ch)
{
	if( stPos >= size || m_str[stPos] == '\0' )
		return;

	m_str[stPos] = ch;
}

template <size_t size> inline
int NkCharsA<size>::Compare(const char* pcStr) const
{
	return strcmp(m_str, pcStr);
}

template <size_t size> inline
int NkCharsA<size>::CompareNoCase(const char* pcStr) const
{
	return _stricmp(m_str, pcStr);
}

template <size_t size> inline
bool NkCharsA<size>::Equals(const char* pcStr) const
{
	return strcmp(m_str, pcStr) == 0;
}

template <size_t size> inline
bool NkCharsA<size>::EqualsNoCase(const char* pcStr) const
{
	return _stricmp(m_str, pcStr) == 0;
}

template <size_t size>
void NkCharsA<size>::Append(const char* pcStr)
{
	m_str[size - 1] = '\0';
	size_t stLength = strlen(m_str);

	StringCchCopyA( m_str + stLength, size - stLength, pcStr );
}

template <size_t size>
void NkCharsA<size>::Append(char ch)
{
	m_str[size - 1] = '\0';
	size_t stLength = strlen(m_str);

	if( stLength < size - 1 )
	{
		m_str[stLength] = ch;
		m_str[stLength + 1] = '\0';
	}
}

template <size_t size>
void NkCharsA<size>::TrimLeft(char ch)
{
	if( ch == '\0' )
		return;

	char* pcCurPos = m_str;
	char* pcEndPos = pcCurPos + size - 1;

	*pcEndPos = '\0';

	while( pcCurPos < pcEndPos && *pcCurPos == ch )
		++pcCurPos;

	if( pcCurPos > m_str )
		memmove(m_str, pcCurPos, pcEndPos - pcCurPos + 1);
}

template <size_t size>
void NkCharsA<size>::TrimRight(char ch)
{
	if( ch == '\0' )
		return;

	m_str[size - 1] = '\0';
	size_t stLength = strlen(m_str);

	char* pcBeginPos = m_str;
	char* pcCurPos = pcBeginPos + stLength - 1;

	while( pcCurPos >= pcBeginPos && *pcCurPos == ch )
		--pcCurPos;

	if( pcCurPos < pcBeginPos + stLength - 1)
		*(pcCurPos + 1) = '\0';
}

template <size_t size>
void NkCharsA<size>::Format(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);

	_vsnprintf(m_str, size - 1, format, arg);
	m_str[size - 1] = '\0';

	va_end( arg );
}

template <size_t size>
void NkCharsA<size>::AppendFormat(const char* format, ...)
{
	va_list arg;
	va_start( arg, format );

	m_str[size - 1] = '\0';
	int length = (int)strlen( m_str );

	_vsnprintf( m_str + length, size - length - 1, format, arg );
	m_str[size - 1] = '\0';

	va_end( arg );
}

template <size_t size>
void NkCharsA<size>::Reverse()
{
	m_str[ size - 1 ] = '\0';
	size_t stLength = strlen(m_str);

	if( stLength < 2 )
		return;

	size_t stEnd = stLength - 1;

	char* pcString = m_str;

	char ch;
	for( size_t stBegin = 0; stBegin < stEnd; ++stBegin, --stEnd )
	{
		ch = pcString[stBegin];
		pcString[stBegin] = pcString[stEnd];
		pcString[stEnd] = ch;
	}
}

template <size_t size>
void NkCharsA<size>::ToUpper()
{
	m_str[size - 1] = '\0';

	char* pcCurPos = m_str;

	while( *pcCurPos != '\0' )
	{
		if( ('a' <= *pcCurPos) && (*pcCurPos <= 'z') )
			*pcCurPos -= ('a' - 'A');

		++pcCurPos;
	}
}

template <size_t size>
void NkCharsA<size>::ToLower()
{
	m_str[ size - 1 ] = '\0';

	char* pcCurPos = m_str;

	while( *pcCurPos != '\0' )
	{
		if( ('A' <= *pcCurPos) && (*pcCurPos <= 'Z') )
			*pcCurPos += ('a' - 'A');

		++pcCurPos;
	}
}

//-------------------------------------------------------------------------------------------------
// NkCharsA operators
//-------------------------------------------------------------------------------------------------
template <size_t size> inline
NkCharsA<size>::operator const char*() const
{
	return m_str;
}

template <size_t size> inline
NkCharsA<size>::operator char*()
{
	return m_str;
}

template <size_t size> inline
char& NkCharsA<size>::operator[] (size_t st) const
{
	return m_str[st];
}

template <size_t size> inline
const char* NkCharsA<size>::operator=(const char* pcStr)
{
	StringCchCopyA(m_str, size, pcStr);
	return m_str;
}

template <size_t size> inline
const char* NkCharsA<size>::operator=(char ch)
{
	m_str[0] = ch;
	m_str[1] = '\0';
	return m_str;
}

template <size_t size> inline
const char* NkCharsA<size>::operator+=(const char* pcStr)
{
	Append(pcStr);
	return m_str;
}

template <size_t size> inline
const char* NkCharsA<size>::operator+=(char ch)
{
	Append(ch);
	return m_str;
}
