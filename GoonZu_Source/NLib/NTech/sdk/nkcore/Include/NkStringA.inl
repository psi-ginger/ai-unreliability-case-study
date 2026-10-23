//-------------------------------------------------------------------------------------------------
// NkStringA.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

inline bool NkStringA::IsEmpty() const
{
	return m_cchLength == 0;
}

inline void NkStringA::Empty()
{
	_Deallocate();
}

inline size_t NkStringA::GetLength() const
{
	return m_cchLength;
}

inline const char* NkStringA::GetString() const
{
	if( !m_pcString )
		return "";

	return m_pcString;
}

inline char NkStringA::GetAt(size_t stPos) const
{
	NkAssert(stPos <= m_cchLength);

	return m_pcString[stPos];
}

inline void NkStringA::SetAt(size_t stPos, char ch)
{
	NkAssert(stPos < m_cchLength);

	m_pcString[stPos] = ch;
}
inline int NkStringA::Compare(const NkStringA& kStr) const
{
	return strcmp(m_pcString, kStr.m_pcString);
}

inline int NkStringA::Compare(const char* pcStr) const
{
	return strcmp(m_pcString, pcStr);
}

inline int NkStringA::CompareNoCase(const NkStringA& kStr) const
{
	return _stricmp(m_pcString, kStr.m_pcString);
}

inline int NkStringA::CompareNoCase(const char* pcStr) const
{
	return _stricmp(m_pcString, pcStr);
}

inline bool NkStringA::Equals(const NkStringA& kStr) const
{
	return strcmp(m_pcString, kStr.m_pcString) == 0;
}

inline bool NkStringA::Equals(const char* pcStr) const
{
	return strcmp(m_pcString, pcStr) == 0;
}

inline bool NkStringA::EqualsNoCase(const NkStringA& kStr) const
{
	return _stricmp(m_pcString, kStr.m_pcString) == 0;
}

inline bool NkStringA::EqualsNoCase(const char* pcStr) const
{
	return _stricmp(m_pcString, pcStr) == 0;
}

//-------------------------------------------------------------------------------------------------
// NkStringA operators
//-------------------------------------------------------------------------------------------------
inline NkStringA::operator const char*() const
{
	return m_pcString;
}

//-------------------------------------------------------------------------------------------------
inline NkStringA& NkStringA::operator=(const NkStringA& kStr)
{
	_Allocate(kStr);
	return *this;
}

inline NkStringA& NkStringA::operator=(const char* pcStr)
{
	_Allocate(pcStr);
	return *this;
}

inline NkStringA& NkStringA::operator=(char ch)
{
	m_pcString[0] = ch;
	m_pcString[1] = '\0';
	m_cchLength = 1;
	return *this;
}

//-------------------------------------------------------------------------------------------------
inline NkStringA& NkStringA::operator+=(const NkStringA& kStr)
{
	Append(kStr);
	return *this;
}

inline NkStringA& NkStringA::operator+=(const char* pcStr)
{
	Append(pcStr);
	return *this;
}

inline NkStringA& NkStringA::operator+=(char ch)
{
	Append(ch);
	return *this;
}

//-------------------------------------------------------------------------------------------------
inline NkStringA operator+(const NkStringA& kStr1, const NkStringA& kStr2)
{
	NkStringA kResult(kStr1);
	kResult.Append(kStr2);
	return kResult;
}

inline NkStringA operator+(const NkStringA& kStr1, const char* pcStr2)
{
	NkStringA kResult(kStr1);
	kResult.Append(pcStr2);
	return kResult;
}

inline NkStringA operator+(const char* pcStr1, const NkStringA& kStr2)
{
	NkStringA kResult(pcStr1);
	kResult.Append(kStr2);
	return kResult;
}

inline NkStringA operator+(const NkStringA& kStr1, char ch2)
{
	NkStringA kResult(kStr1);
	kResult.Append(ch2);
	return kResult;
}

inline NkStringA operator+(char ch1, const NkStringA& kStr2)
{ 
	NkStringA kResult(ch1);
	kResult.Append(kStr2);
	return kResult;
}

//-------------------------------------------------------------------------------------------------
inline bool operator==(const NkStringA& kStr1, const NkStringA& kStr2)
{
	return kStr1.Equals(kStr2);
}

inline bool operator==(const NkStringA& kStr1, const char* pcStr2)
{
	return kStr1.Equals(pcStr2);
}

inline bool operator==(const char* pcStr1, const NkStringA& kStr2)
{
	return kStr2.Equals(pcStr1);
}

//-------------------------------------------------------------------------------------------------
inline bool operator!=(const NkStringA& kStr1, const NkStringA& kStr2)
{
	return !kStr1.Equals(kStr2);
}

inline bool operator!=(const NkStringA& kStr1, const char* pcStr2)
{
	return !kStr1.Equals(pcStr2);
}

inline bool operator!=(const char* pcStr1, const NkStringA& kStr2)
{
	return kStr2.Equals(pcStr1);
}

//-------------------------------------------------------------------------------------------------
inline bool operator<(const NkStringA& kStr1, const NkStringA& kStr2)
{
	return kStr1.Compare(kStr2) < 0;
}

inline bool operator<(const NkStringA& kStr1, const char* pcStr2)
{
	return kStr1.Compare(pcStr2) < 0;
}

inline bool operator<(const char* pcStr1, const NkStringA& kStr2)
{
	return kStr2.Compare(pcStr1) > 0;
}

//-------------------------------------------------------------------------------------------------
inline bool operator>(const NkStringA& kStr1, const NkStringA& kStr2)
{
	return kStr1.Compare(kStr2) > 0;
}

inline bool operator>(const NkStringA& kStr1, const char* pcStr2)
{
	return kStr1.Compare(pcStr2) > 0;
}

inline bool operator>(const char* pcStr1, const NkStringA& kStr2)
{
	return kStr2.Compare(pcStr1) < 0;
}

//-------------------------------------------------------------------------------------------------
inline bool operator<=(const NkStringA& kStr1, const NkStringA& kStr2)
{
	return kStr1.Compare(kStr2) <= 0;
}

inline bool operator<=(const NkStringA& kStr1, const char* pcStr2)
{
	return kStr1.Compare(pcStr2) <= 0;
}

inline bool operator<=(const char* pcStr1, const NkStringA& kStr2)
{
	return kStr2.Compare(pcStr1) >= 0;
}

//-------------------------------------------------------------------------------------------------
inline bool operator>=(const NkStringA& kStr1, const NkStringA& kStr2)
{
	return kStr1.Compare(kStr2) >= 0;
}

inline bool operator>=(const NkStringA& kStr1, const char* pcStr2)
{
	return kStr1.Compare(pcStr2) >= 0;
}

inline bool operator>=(const char* pcStr1, const NkStringA& kStr2)
{
	return kStr2.Compare(pcStr1) <= 0;
}

