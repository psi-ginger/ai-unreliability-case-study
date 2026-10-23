//-------------------------------------------------------------------------------------------------
// NkStringRef.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkString.h"

class NkStringRef
{
public:
	inline NkStringRef() : m_pcStr(0) {};
	inline NkStringRef(const wchar_t* pcStr) { m_pcStr = (wchar_t*)pcStr; }
	inline NkStringRef(const NkStringW& kStr) { m_pcStr = (wchar_t*)kStr.GetString(); }

	inline const wchar_t* operator=(const wchar_t* pcStr) { m_pcStr = (wchar_t*)pcStr; return m_pcStr; }
	inline const wchar_t* operator=(const NkStringW& kStr) { m_pcStr = (wchar_t*)kStr.GetString(); return m_pcStr; }

	inline operator const wchar_t*() const
	{
		return m_pcStr;
	}

	inline const wchar_t* GetString() { return m_pcStr; }

protected:
	wchar_t*	m_pcStr;
};
