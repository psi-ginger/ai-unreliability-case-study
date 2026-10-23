inline bool NkHandleStringA::Equals(const NkHandleStringA& kStr) const
{
	return m_handle == kStr.m_handle;
}

inline bool NkHandleStringA::Equals(const char* pcStr) const
{
	char* pcStr1 = (char*)NkGlobalStringTable::GetString(m_handle);

	if( pcStr1 == pcStr )
		return true;

	if( pcStr == 0 )
		return false;

	return strcmp(pcStr, pcStr1) == 0;
}

bool operator==(const NkHandleStringA& kStr1, const NkHandleStringA& kStr2)
{
	return kStr1.Equals(kStr2);
}

bool operator==(const NkHandleStringA& kStr1, const char* pcStr2)
{
	return kStr1.Equals(pcStr2);
}

bool operator==(const char* pcStr1, const NkHandleStringA& kStr2)
{
	return kStr2.Equals(pcStr1);
}

bool operator!=(const NkHandleStringA& kStr1, const NkHandleStringA& kStr2)
{
	return !kStr1.Equals(kStr2);
}

bool operator!=(const NkHandleStringA& kStr1, const char* pcStr2)
{
	return !kStr1.Equals(pcStr2);
}

bool operator!=(const char* pcStr1, const NkHandleStringA& kStr2)
{
	return !kStr2.Equals(pcStr1);
}
