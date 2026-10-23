//-------------------------------------------------------------------------------------------------
// NkStringW.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

inline bool NkStringW::IsEmpty() const
{
	return m_cchLength == 0;
}

inline void NkStringW::Empty()
{
	_Deallocate();
}

inline size_t NkStringW::GetLength() const
{
	return m_cchLength;
}

inline const wchar_t* NkStringW::GetString() const
{
	if( !m_pcString )
		return L"";

	return m_pcString;
}

inline wchar_t NkStringW::GetAt(size_t stPos) const
{
	NkAssert(stPos <= m_cchLength);

	return m_pcString[stPos];
}

inline void NkStringW::SetAt(size_t stPos, wchar_t ch)
{
	NkAssert(stPos < m_cchLength);

	m_pcString[stPos] = ch;
}

inline int NkStringW::Compare(const NkStringW& kStr) const
{
	return wcscmp(m_pcString, kStr.m_pcString);
}

inline int NkStringW::Compare(const wchar_t* pcStr) const
{
	return wcscmp(m_pcString, pcStr);
}

inline int NkStringW::CompareNoCase(const NkStringW& kStr) const
{
	return _wcsicmp(m_pcString, kStr.m_pcString);
}

inline int NkStringW::CompareNoCase(const wchar_t* pcStr) const
{
	return _wcsicmp(m_pcString, pcStr);
}

inline bool NkStringW::Equals(const NkStringW& kStr) const
{
	return wcscmp(m_pcString, kStr.m_pcString) == 0;
}

inline bool NkStringW::Equals(const wchar_t* pcStr) const
{
	return wcscmp(m_pcString, pcStr) == 0;
}

inline bool NkStringW::EqualsNoCase(const NkStringW& kStr) const
{
	return _wcsicmp(m_pcString, kStr.m_pcString) == 0;
}

inline bool NkStringW::EqualsNoCase(const wchar_t* pcStr) const
{
	return _wcsicmp(m_pcString, pcStr) == 0;
}

//-------------------------------------------------------------------------------------------------
// NkStringW operators
//-------------------------------------------------------------------------------------------------
inline NkStringW::operator const wchar_t*() const
{
	return m_pcString;
}

//-------------------------------------------------------------------------------------------------
inline NkStringW& NkStringW::operator=(const NkStringW& kStr)
{
	_Allocate(kStr);
	return *this;
}

inline NkStringW& NkStringW::operator=(const wchar_t* pcStr)
{
	_Allocate(pcStr);
	return *this;
}

inline NkStringW& NkStringW::operator=(wchar_t ch)
{
	m_pcString[0] = ch;
	m_pcString[1] = L'\0';
	m_cchLength = 1;
	return *this;
}

//-------------------------------------------------------------------------------------------------
inline NkStringW& NkStringW::operator+=(const NkStringW& kStr)
{
	Append(kStr);
	return *this;
}

inline NkStringW& NkStringW::operator+=(const wchar_t* pcStr)
{
	Append(pcStr);
	return *this;
}

inline NkStringW& NkStringW::operator+=(wchar_t ch)
{
	Append(ch);
	return *this;
}

//-------------------------------------------------------------------------------------------------
inline NkStringW operator+(const NkStringW& kStr1, const NkStringW& kStr2)
{
	NkStringW kResult(kStr1);
	kResult.Append(kStr2);
	return kResult;
}

inline NkStringW operator+(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	NkStringW kResult(kStr1);
	kResult.Append(pcStr2);
	return kResult;
}

inline NkStringW operator+(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	NkStringW kResult(pcStr1);
	kResult.Append(kStr2);
	return kResult;
}

inline NkStringW operator+(const NkStringW& kStr1, wchar_t ch2)
{
	NkStringW kResult(kStr1);
	kResult.Append(ch2);
	return kResult;
}

inline NkStringW operator+(wchar_t ch1, const NkStringW& kStr2)
{ 
	NkStringW kResult(ch1);
	kResult.Append(kStr2);
	return kResult;
}

//-------------------------------------------------------------------------------------------------
inline bool operator==(const NkStringW& kStr1, const NkStringW& kStr2)
{
	return kStr1.Equals(kStr2);
}

inline bool operator==(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	return kStr1.Equals(pcStr2);
}

inline bool operator==(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	return kStr2.Equals(pcStr1);
}

//-------------------------------------------------------------------------------------------------
inline bool operator!=(const NkStringW& kStr1, const NkStringW& kStr2)
{
	return !kStr1.Equals(kStr2);
}

inline bool operator!=(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	return !kStr1.Equals(pcStr2);
}

inline bool operator!=(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	return !kStr2.Equals(pcStr1);
}

//-------------------------------------------------------------------------------------------------
inline bool operator<(const NkStringW& kStr1, const NkStringW& kStr2)
{
	return kStr1.Compare(kStr2) < 0;
}

inline bool operator<(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	return kStr1.Compare(pcStr2) < 0;
}

inline bool operator<(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	return kStr2.Compare(pcStr1) > 0;
}

//-------------------------------------------------------------------------------------------------
inline bool operator>(const NkStringW& kStr1, const NkStringW& kStr2)
{
	return kStr1.Compare(kStr2) > 0;
}

inline bool operator>(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	return kStr1.Compare(pcStr2) > 0;
}

inline bool operator>(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	return kStr2.Compare(pcStr1) < 0;
}

//-------------------------------------------------------------------------------------------------
inline bool operator<=(const NkStringW& kStr1, const NkStringW& kStr2)
{
	return kStr1.Compare(kStr2) <= 0;
}

inline bool operator<=(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	return kStr1.Compare(pcStr2) <= 0;
}

inline bool operator<=(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	return kStr2.Compare(pcStr1) >= 0;
}

//-------------------------------------------------------------------------------------------------
inline bool operator>=(const NkStringW& kStr1, const NkStringW& kStr2)
{
	return kStr1.Compare(kStr2) >= 0;
}

inline bool operator>=(const NkStringW& kStr1, const wchar_t* pcStr2)
{
	return kStr1.Compare(pcStr2) >= 0;
}

inline bool operator>=(const wchar_t* pcStr1, const NkStringW& kStr2)
{
	return kStr2.Compare(pcStr1) <= 0;
}

