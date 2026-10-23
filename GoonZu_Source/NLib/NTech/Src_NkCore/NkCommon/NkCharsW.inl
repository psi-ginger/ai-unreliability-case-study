//-------------------------------------------------------------------------------------------------
// NkCharsW.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <size_t size>
NkCharsW<size>::NkCharsW()
{
	m_str[0] = L'\0';
}

template <size_t size>
NkCharsW<size>::NkCharsW(const wchar_t* pcStr)
{
	NkAssert(size > 1);

	StringCchCopyW(m_str, size, pcStr);
}

template <size_t size>
NkCharsW<size>::NkCharsW(wchar_t ch)
{
	NkAssert(size > 1);

	m_str[0] = ch;
	m_str[1] = L'\0';
}

template <size_t size>
inline bool NkCharsW<size>::IsEmpty() const
{
	return m_str[0] != 0;
}

template <size_t size>
inline void NkCharsW<size>::Empty()
{
	m_str[0] = L'\0';
}

template <size_t size>
inline void NkCharsW<size>::ZeroBuffer()
{
	wmemset(m_str, 0, size);
}

template <size_t size>
inline size_t NkCharsW<size>::GetMaxCchSize() const
{
	return size;
}

template <size_t size>
inline size_t NkCharsW<size>::GetMaxByteSize() const
{
	return sizeof(m_str);
}

template <size_t size>
inline size_t NkCharsW<size>::GetLength() const
{
	size_t stLength;
	StringCchLengthW(m_str, size, &stLength);
	return stLength;
}

template <size_t size>
inline const wchar_t* NkCharsW<size>::GetString() const
{
	return m_str;
}

template <size_t size>
inline wchar_t NkCharsW<size>::GetAt(size_t stPos) const
{
	if( stPos >= size )
		return L'\0';

	return m_str[ stPos ];
}

template <size_t size>
inline void NkCharsW<size>::SetAt(size_t stPos, wchar_t ch)
{
	if( stPos >= size || m_str[ stPos ] == L'\0' )
		return;

	m_str[ stPos ] = ch;
}

template <size_t size>
inline int NkCharsW<size>::Compare(const wchar_t* pcStr) const
{
	return wcscmp(m_str, pcStr);
}

template <size_t size>
inline int NkCharsW<size>::CompareNoCase(const wchar_t* pcStr) const
{
	return _wcsicmp(m_str, pcStr);
}

template <size_t size>
inline bool NkCharsW<size>::Equals(const wchar_t* pcStr) const
{
	return wcscmp(m_str, pcStr) == 0;
}

template <size_t size>
inline bool NkCharsW<size>::EqualsNoCase(const wchar_t* pcStr) const
{
	return _wcsicmp(m_str, pcStr) == 0;
}

template <size_t size>
void NkCharsW<size>::Append(const wchar_t* pcStr)
{
	m_str[ size - 1 ] = L'\0';
	size_t stLength = wcslen( m_str );

	StringCchCopyW( m_str + stLength, size - stLength, pcStr );
}

template <size_t size>
void NkCharsW<size>::Append(wchar_t ch)
{
	m_str[ size - 1 ] = L'\0';
	size_t stLength = wcslen( m_str );

	if( stLength < size - 1 )
	{
		m_str[ stLength ] = ch;
		m_str[ stLength + 1 ] = L'\0';
	}
}

template <size_t size>
void NkCharsW<size>::TrimLeft(wchar_t ch)
{
	if( ch == L'\0' )
		return;

	wchar_t* pcCurPos = m_str;
	wchar_t* pcEndPos = pcCurPos + size - 1;

	*pcEndPos = L'\0';

	while( pcCurPos < pcEndPos && *pcCurPos == ch )
		++pcCurPos;

	if( pcCurPos > m_str )
		wmemmove(m_str, pcCurPos, pcEndPos - pcCurPos + 1);
}

template <size_t size>
void NkCharsW<size>::TrimRight(wchar_t ch)
{
	if( ch == L'\0' )
		return;

	m_str[ size - 1 ] = L'\0';
	size_t stLength = wcslen(m_str);

	wchar_t* pcBeginPos = m_str;
	wchar_t* pcCurPos = pcBeginPos + stLength - 1;

	while( pcCurPos >= pcBeginPos && *pcCurPos == ch )
		--pcCurPos;

	if( pcCurPos < pcBeginPos + stLength - 1)
		*(pcCurPos + 1) = L'\0';
}

template <size_t size>
void NkCharsW<size>::Format(const wchar_t* format, ...)
{
	va_list arg;
	va_start(arg, format);

	_vsnwprintf(m_str, size - 1, format, arg);
	m_str[ size - 1 ] = L'\0';

	va_end( arg );
}

template <size_t size>
void NkCharsW<size>::AppendFormat(const wchar_t* format, ...)
{
	va_list arg;
	va_start( arg, format );

	m_str[ size - 1 ] = L'\0';
	int length = (int)wcslen( m_str );

	_vsnwprintf( m_str + length, size - length - 1, format, arg );
	m_str[ size - 1 ] = '\0';

	va_end( arg );
}

template <size_t size>
void NkCharsW<size>::Reverse()
{
	m_str[ size - 1 ] = L'\0';
	size_t stLength = wcslen(m_str);

	if( stLength < 2 )
		return;

	size_t stEnd = stLength - 1;

	wchar_t* pcString = m_str;

	wchar_t ch;
	for( size_t stBegin = 0; stBegin < stEnd; ++stBegin, --stEnd )
	{
		ch = pcString[ stBegin ];
		pcString[ stBegin ] = pcString[ stEnd ];
		pcString[ stEnd ] = ch;
	}
}

template <size_t size>
void NkCharsW<size>::ToUpper()
{
	m_str[ size - 1 ] = L'\0';

	wchar_t* pcCurPos = m_str;

	while( *pcCurPos != L'\0' )
	{
		if( (L'a' <= *pcCurPos) && (*pcCurPos <= L'z') )
			*pcCurPos -= (L'a' - L'A');

		++pcCurPos;
	}
}

template <size_t size>
void NkCharsW<size>::ToLower()
{
	m_str[ size - 1 ] = L'\0';

	wchar_t* pcCurPos = m_str;

	while( *pcCurPos != L'\0' )
	{
		if( (L'A' <= *pcCurPos) && (*pcCurPos <= L'Z') )
			*pcCurPos += (L'a' - L'A');

		++pcCurPos;
	}
}

//-------------------------------------------------------------------------------------------------
// NkCharsW operators
//-------------------------------------------------------------------------------------------------
template <size_t size>
inline NkCharsW<size>::operator const wchar_t*() const
{
	return m_str;
}

template <size_t size>
inline NkCharsW<size>::operator wchar_t*()
{
	return m_str;
}

template <size_t size>
inline wchar_t& NkCharsW<size>::operator[] (size_t st) const
{
	return m_str[ st ];
}

template <size_t size>
inline const wchar_t* NkCharsW<size>::operator=(const wchar_t* pcStr)
{
	StringCchCopyW(m_str, size, pcStr);
	return m_str;
}

template <size_t size>
inline const wchar_t* NkCharsW<size>::operator=(wchar_t ch)
{
	m_str[0] = ch;
	m_str[1] = L'\0';
	return m_str;
}

template <size_t size>
inline const wchar_t* NkCharsW<size>::operator+=(const wchar_t* pcStr)
{
	Append(pcStr);
	return m_str;
}

template <size_t size>
inline const wchar_t* NkCharsW<size>::operator+=(wchar_t ch)
{
	Append(ch);
	return m_str;
}
