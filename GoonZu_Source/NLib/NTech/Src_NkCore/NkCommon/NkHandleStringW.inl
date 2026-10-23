inline bool NkHandleStringW::Equals(const NkHandleStringW& kStr) const
{
	return m_handle == kStr.m_handle;
}

inline bool NkHandleStringW::Equals(const wchar_t* pcStr) const
{
	wchar_t* pcStr1 = (wchar_t*)NkGlobalStringTable::GetString(m_handle);

	if( pcStr1 == pcStr )
		return true;

	if( pcStr == 0 )
		return false;

	return wcscmp(pcStr, pcStr1) == 0;
}

bool operator==(const NkHandleStringW& kStr1, const NkHandleStringW& kStr2)
{
	return kStr1.Equals(kStr2);
}

bool operator==(const NkHandleStringW& kStr1, const wchar_t* pcStr2)
{
	return kStr1.Equals(pcStr2);
}

bool operator==(const wchar_t* pcStr1, const NkHandleStringW& kStr2)
{
	return kStr2.Equals(pcStr1);
}

bool operator!=(const NkHandleStringW& kStr1, const NkHandleStringW& kStr2)
{
	return !kStr1.Equals(kStr2);
}

bool operator!=(const NkHandleStringW& kStr1, const wchar_t* pcStr2)
{
	return !kStr1.Equals(pcStr2);
}

bool operator!=(const wchar_t* pcStr1, const NkHandleStringW& kStr2)
{
	return !kStr2.Equals(pcStr1);
}
