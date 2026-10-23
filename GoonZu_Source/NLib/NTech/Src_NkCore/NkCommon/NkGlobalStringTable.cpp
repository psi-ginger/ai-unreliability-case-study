//-------------------------------------------------------------------------------------------------
// NkGlobalStringTable.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "NkGlobalStringTable.h"

NkGlobalStringTable* NkGlobalStringTable::ms_pkGlobalStringTable = 0;

NkGlobalStringTable::NkGlobalStringTable()
{
	InitializeCriticalSection(&m_csA);
	InitializeCriticalSection(&m_csW);

	m_pkStringMapA = NkNew NkStringHashMapA< StringTag >( 513 );
	m_pkStringMapW = NkNew NkStringHashMapW< StringTag >( 513 );

	NULL_STRINGA = AddString("");
	NULL_STRINGW = AddString(L"");
}

NkGlobalStringTable::~NkGlobalStringTable()
{
	NkDelete m_pkStringMapA;
	NkDelete m_pkStringMapW;

	DeleteCriticalSection(&m_csA);
	DeleteCriticalSection(&m_csW);
}

NkGlobalStringTable::StringHandleA
NkGlobalStringTable::AddString(const char* pcStr)
{
	size_t stLength;
	StringCchLengthA(pcStr, 65536, &stLength);

	NkHashMap_Iterator<char*,StringTag> kIter;
	StringHandleA kHandle;

	EnterCriticalSection(&m_csA);

	if( m_pkStringMapA->Find((char*)pcStr, kIter) )
	{
		kHandle = (StringHandleA)kIter.GetNodePtr();

		IncRefCount(kHandle);

		LeaveCriticalSection(&m_csA);

		return kHandle;
	}

	StringTag kStringTag;
	kStringTag.m_uiLength = (unsigned int)stLength;
	kStringTag.m_uiRefCount = 1;

	m_pkStringMapA->Insert(kIter, (char*)pcStr, kStringTag);

	kHandle = (StringHandleA)kIter.GetNodePtr();

	LeaveCriticalSection(&m_csA);

	return kHandle;
}

NkGlobalStringTable::StringHandleW
NkGlobalStringTable::AddString(const wchar_t* pcStr)
{
	size_t stLength;
	StringCchLengthW(pcStr, 65536, &stLength);

	NkHashMap_Iterator<wchar_t*,StringTag> kIter;
	StringHandleW kHandle;

	EnterCriticalSection(&m_csW);

	if( m_pkStringMapW->Find((wchar_t*)pcStr, kIter) )
	{
		kHandle = (StringHandleW)kIter.GetNodePtr();

		IncRefCount(kHandle);

		LeaveCriticalSection(&m_csW);

		return kHandle;
	}
	
	StringTag kStringTag;
	kStringTag.m_uiLength = (unsigned int)stLength;
	kStringTag.m_uiRefCount = 1;

	m_pkStringMapW->Insert(kIter, (wchar_t*)pcStr, kStringTag);

	kHandle = (StringHandleW)kIter.GetNodePtr();

	LeaveCriticalSection(&m_csW);
	
	return kHandle;
}

NkGlobalStringTable::StringHandleA
NkGlobalStringTable::FindString(const char* pcStr)
{
	NkHashMap_Iterator<char*,StringTag> kIter;
	StringHandleA kHandle;

	EnterCriticalSection(&m_csA);

	if( m_pkStringMapA->Find((char*)pcStr, kIter) )
	{
		kHandle = (StringHandleA)kIter.GetNodePtr();

		LeaveCriticalSection(&m_csA);

		return kHandle;
	}

	LeaveCriticalSection(&m_csA);

	return 0;
}

NkGlobalStringTable::StringHandleW
NkGlobalStringTable::FindString(const wchar_t* pcStr)
{
	NkHashMap_Iterator<wchar_t*,StringTag> kIter;
	StringHandleW kHandle;

	EnterCriticalSection(&m_csW);

	if( m_pkStringMapW->Find((wchar_t*)pcStr, kIter) )
	{
		kHandle = (StringHandleW)kIter.GetNodePtr();
		
		LeaveCriticalSection(&m_csW);

		return kHandle;
	}

	LeaveCriticalSection(&m_csW);

	return 0;
}


void NkGlobalStringTable::Startup()
{
	if( !ms_pkGlobalStringTable )
		ms_pkGlobalStringTable = NkNew NkGlobalStringTable;
}

void NkGlobalStringTable::Shutdown()
{
	if( ms_pkGlobalStringTable )
	{
		NkDelete ms_pkGlobalStringTable;
		ms_pkGlobalStringTable = 0;
	}
}