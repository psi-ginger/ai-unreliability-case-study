//-------------------------------------------------------------------------------------------------
// NkHandleString.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkGlobalStringTable.h"

//-------------------------------------------------------------------------------------------------
// NkHandleStringA
//-------------------------------------------------------------------------------------------------
class NkHandleStringA
{
public:
	NkHandleStringA();
	NkHandleStringA(const NkHandleStringA& kStr);
	NkHandleStringA(const char* pcStr);	
	~NkHandleStringA();

    operator const char*() const;

	inline bool Equals(const NkHandleStringA& kStr) const;
	inline bool Equals(const char* pcStr) const;

	NkHandleStringA& operator=(const NkHandleStringA& kStr);
	NkHandleStringA& operator=(const char* pcStr);

protected:
	NkGlobalStringTable::StringHandleA m_handle;
};

inline bool operator==(const NkHandleStringA& kStr1, const NkHandleStringA& kStr2);
inline bool operator==(const NkHandleStringA& kStr1, const char* pcStr2);
inline bool operator==(const char* pcStr1, const NkHandleStringA& kStr2);

inline bool operator!=(const NkHandleStringA& kStr1, const NkHandleStringA& kStr2);
inline bool operator!=(const NkHandleStringA& kStr1, const char* pcStr2);
inline bool operator!=(const char* pcStr1, const NkHandleStringA& kStr2);

//-------------------------------------------------------------------------------------------------
// NkHandleStringW
//-------------------------------------------------------------------------------------------------
class NkHandleStringW
{
public:
	NkHandleStringW();
	NkHandleStringW(const NkHandleStringW& kStr);
	NkHandleStringW(const wchar_t* pcStr);	
	~NkHandleStringW();

	operator const wchar_t*() const;

	inline bool Equals(const NkHandleStringW& kStr) const;
	inline bool Equals(const wchar_t* pcStr) const;

	NkHandleStringW& operator=(const NkHandleStringW& kStr);
	NkHandleStringW& operator=(const wchar_t* pcStr);

protected:
	NkGlobalStringTable::StringHandleW m_handle;
};

inline bool operator==(const NkHandleStringW& kStr1, const NkHandleStringW& kStr2);
inline bool operator==(const NkHandleStringW& kStr1, const wchar_t* pcStr2);
inline bool operator==(const wchar_t* pcStr1, const NkHandleStringW& kStr2);

inline bool operator!=(const NkHandleStringW& kStr1, const NkHandleStringW& kStr2);
inline bool operator!=(const NkHandleStringW& kStr1, const wchar_t* pcStr2);
inline bool operator!=(const wchar_t* pcStr1, const NkHandleStringW& kStr2);


#include "NkHandleStringA.inl"
#include "NkHandleStringW.inl"