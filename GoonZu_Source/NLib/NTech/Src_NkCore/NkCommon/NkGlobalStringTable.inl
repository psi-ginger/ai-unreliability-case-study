//-------------------------------------------------------------------------------------------------
// NkGlobalStringTable.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

inline NkGlobalStringTable& NkGlobalStringTable::Get()
{
	return *ms_pkGlobalStringTable;
}

inline const NkGlobalStringTable::StringHandleA 
NkGlobalStringTable::GetNullStringA()
{
	return Get().NULL_STRINGA;
}

inline const NkGlobalStringTable::StringHandleW 
NkGlobalStringTable::GetNullStringW()
{
	return Get().NULL_STRINGW;
}

inline const char* NkGlobalStringTable::GetString(const StringHandleA& kHandle)
{
	return ((NkHashMapNode<char*,StringTag>*)kHandle)->m_key;
}

inline const wchar_t* NkGlobalStringTable::GetString(const StringHandleW& kHandle)
{
	return ((NkHashMapNode<wchar_t*,StringTag>*)kHandle)->m_key;
}

inline void NkGlobalStringTable::IncRefCount(StringHandleA& kHandle)
{
	NkHashMapNode<char*,StringTag>* pkNode = (NkHashMapNode<char*,StringTag>*)kHandle;

	InterlockedIncrement((LONG*)&pkNode->m_data.m_uiRefCount);
}

inline void NkGlobalStringTable::IncRefCount(StringHandleW& kHandle)
{
	NkHashMapNode<wchar_t*,StringTag>* pkNode = (NkHashMapNode<wchar_t*,StringTag>*)kHandle;

	InterlockedIncrement((LONG*)&pkNode->m_data.m_uiRefCount);
}

inline void NkGlobalStringTable::DecRefCount(StringHandleA& kHandle)
{
	NkHashMapNode<char*,StringTag>* pkNode = (NkHashMapNode<char*,StringTag>*)kHandle;

	if( InterlockedDecrement((LONG*)&pkNode->m_data.m_uiRefCount) == 0 )
	{
		EnterCriticalSection(&m_csA);

		if( pkNode->m_data.m_uiRefCount == 0 )
			m_pkStringMapA->Remove(pkNode->m_key);

		LeaveCriticalSection(&m_csA);
	}
}

inline void NkGlobalStringTable::DecRefCount(StringHandleW& kHandle)
{
	NkHashMapNode<wchar_t*,StringTag>* pkNode = (NkHashMapNode<wchar_t*,StringTag>*)kHandle;

	if( InterlockedDecrement((LONG*)&pkNode->m_data.m_uiRefCount) == 0 )
	{
		EnterCriticalSection(&m_csW);

		if( pkNode->m_data.m_uiRefCount == 0 )
			m_pkStringMapW->Remove(pkNode->m_key);

		LeaveCriticalSection(&m_csW);
	}
}
