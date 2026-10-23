//-------------------------------------------------------------------------------------------------
// NkHandleStringW.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "NkHandleString.h"


NkHandleStringW::NkHandleStringW()
{
	m_handle = NkGlobalStringTable::GetNullStringW();
}

NkHandleStringW::NkHandleStringW(const NkHandleStringW& kStr)
{
	NkGlobalStringTable::StringHandleW handle = kStr.m_handle;
	NkGlobalStringTable::Get().IncRefCount(handle);
	m_handle = handle;
}

NkHandleStringW::NkHandleStringW(const wchar_t* pcStr)
{
	if( pcStr )
		m_handle = NkGlobalStringTable::Get().AddString(pcStr);
	else
		m_handle = NkGlobalStringTable::GetNullStringW();
}

NkHandleStringW::~NkHandleStringW()
{
	NkGlobalStringTable::Get().DecRefCount(m_handle);
}

NkHandleStringW::operator const wchar_t*() const
{
	return NkGlobalStringTable::GetString(m_handle);
}

NkHandleStringW& NkHandleStringW::operator=(const NkHandleStringW& kStr)
{
	if( m_handle != kStr.m_handle )
	{
		NkGlobalStringTable::StringHandleW handle = kStr.m_handle;
		NkGlobalStringTable::Get().IncRefCount(handle);
		NkGlobalStringTable::Get().DecRefCount(m_handle);
		m_handle = handle;
	}
	return *this;
}

NkHandleStringW& NkHandleStringW::operator=(const wchar_t* pcStr)
{
	if( NkGlobalStringTable::GetString(m_handle) != pcStr )
	{
		NkGlobalStringTable::StringHandleW handle = m_handle;
		m_handle = NkGlobalStringTable::Get().AddString(pcStr);
		NkGlobalStringTable::Get().DecRefCount(handle);
	}
	return *this;
}
