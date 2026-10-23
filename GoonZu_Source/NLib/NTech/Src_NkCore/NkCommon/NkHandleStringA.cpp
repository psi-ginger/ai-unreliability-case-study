//-------------------------------------------------------------------------------------------------
// NkHandleStringA.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "NkHandleString.h"


NkHandleStringA::NkHandleStringA()
{
	m_handle = NkGlobalStringTable::GetNullStringA();
}

NkHandleStringA::NkHandleStringA(const NkHandleStringA& kStr)
{
	NkGlobalStringTable::StringHandleA handle = kStr.m_handle;
	NkGlobalStringTable::Get().IncRefCount(handle);
	m_handle = handle;
}

NkHandleStringA::NkHandleStringA(const char* pcStr)
{
	if( pcStr )
		m_handle = NkGlobalStringTable::Get().AddString(pcStr);
	else
		m_handle = NkGlobalStringTable::GetNullStringA();
}

NkHandleStringA::~NkHandleStringA()
{
	NkGlobalStringTable::Get().DecRefCount(m_handle);
}

NkHandleStringA::operator const char*() const
{
	return NkGlobalStringTable::GetString(m_handle);
}

NkHandleStringA& NkHandleStringA::operator=(const NkHandleStringA& kStr)
{
	if( m_handle != kStr.m_handle )
	{
		NkGlobalStringTable::StringHandleA handle = kStr.m_handle;
		NkGlobalStringTable::Get().IncRefCount(handle);
		NkGlobalStringTable::Get().DecRefCount(m_handle);
		m_handle = handle;
	}
	return *this;
}

NkHandleStringA& NkHandleStringA::operator=(const char* pcStr)
{
	if( NkGlobalStringTable::GetString(m_handle) != pcStr )
	{
		NkGlobalStringTable::StringHandleA handle = m_handle;
		m_handle = NkGlobalStringTable::Get().AddString(pcStr);
		NkGlobalStringTable::Get().DecRefCount(handle);
	}
	return *this;
}
