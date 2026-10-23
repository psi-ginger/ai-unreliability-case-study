//-------------------------------------------------------------------------------------------------
// NkGlobalStringTable.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkMemory.h"
#include "NkStringHashMap.h"

class NkGlobalStringTable : public NkMemory
{
public:
	class StringTag
	{
	public:
		unsigned int m_uiRefCount;
		unsigned int m_uiLength;
	};

	typedef NkHashMapNode<char*,StringTag>* StringHandleA;
	typedef NkHashMapNode<wchar_t*,StringTag>* StringHandleW;

	inline static const StringHandleA GetNullStringA();
	inline static const StringHandleW GetNullStringW();

	inline static NkGlobalStringTable& Get();

	inline static const char* GetString(const StringHandleA& kHandle);
	inline static const wchar_t* GetString(const StringHandleW& kHandle);

	inline void IncRefCount(StringHandleA& kHandle);
	inline void IncRefCount(StringHandleW& kHandle);

	inline void DecRefCount(StringHandleA& kHandle);	
	inline void DecRefCount(StringHandleW& kHandle);

	StringHandleA AddString(const char* pcString);
	StringHandleW AddString(const wchar_t* pcString);

	StringHandleA FindString(const char* pcString);
	StringHandleW FindString(const wchar_t* pcString);

	static void Startup();
	static void Shutdown();

protected:
	NkGlobalStringTable(const NkGlobalStringTable&) {}
	NkGlobalStringTable();
	~NkGlobalStringTable();

	static NkGlobalStringTable* ms_pkGlobalStringTable;

	StringHandleA NULL_STRINGA;
	StringHandleW NULL_STRINGW;

	CRITICAL_SECTION m_csA;
	CRITICAL_SECTION m_csW;

	NkStringHashMapA< StringTag >* m_pkStringMapA;
	NkStringHashMapW< StringTag >* m_pkStringMapW;
};

#include "NkGlobalStringTable.inl"
