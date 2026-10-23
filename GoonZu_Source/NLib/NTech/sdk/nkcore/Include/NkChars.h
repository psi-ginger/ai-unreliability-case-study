//-------------------------------------------------------------------------------------------------
// NkChars.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include <windows.h>

#if defined(DEBUG) || defined(_DEBUG)
#define STRSAFE_LIB
#else
#define STRSAFE_INLINE
#endif

#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CB_FUNCTIONS

#include <tchar.h>
#include <strsafe.h>

#include "NkDebug.h"

//-------------------------------------------------------------------------------------------------
// NkCharsA
//-------------------------------------------------------------------------------------------------
template <size_t size>
class NkCharsA
{
public:
	NkCharsA();
	NkCharsA(const char* pcStr);
	NkCharsA(char ch);

	inline bool IsEmpty() const;
	inline void Empty();

	inline void ZeroBuffer();

	inline size_t GetMaxCchSize() const;
	inline size_t GetMaxByteSize() const;

	inline size_t GetLength() const;
	inline const char* GetString() const;

	inline char GetAt(size_t stPos) const;
	inline void SetAt(size_t stPos, char ch);

	inline int Compare(const char* pcStr) const;
	inline int CompareNoCase(const char* pcStr) const;

	inline bool Equals(const char* pcStr) const;
	inline bool EqualsNoCase(const char* pcStr) const;

	void Append(const char* pcStr);
	void Append(const char ch);

	void Format(const char* format, ...);
	void AppendFormat(const char* format, ...);

	void TrimLeft(char ch = ' ');
	void TrimRight(char ch = ' ');

	void Reverse();
	void ToUpper();
	void ToLower();

	inline operator const char*() const;
	inline operator char*();

	inline char& operator[] (size_t st) const;

	inline const char* operator=(const char* pcStr);
	inline const char* operator=(char ch);
	inline const char* operator+=(const char* pcStr);
	inline const char* operator+=(char ch);

protected:
	char m_str[ size ];
};

//-------------------------------------------------------------------------------------------------
// NkCharsW
//-------------------------------------------------------------------------------------------------
template <size_t size>
class NkCharsW
{
public:
	NkCharsW();
	NkCharsW(const wchar_t* pcStr);
	NkCharsW(wchar_t ch);

	inline bool IsEmpty() const;
	inline void Empty();

	inline void ZeroBuffer();

	inline size_t GetMaxCchSize() const;
	inline size_t GetMaxByteSize() const;

	inline size_t GetLength() const;
	inline const wchar_t* GetString() const;

	inline wchar_t GetAt(size_t stPos) const;
	inline void SetAt(size_t stPos, wchar_t ch);

	inline int Compare(const wchar_t* pcStr) const;
	inline int CompareNoCase(const wchar_t* pcStr) const;

	inline bool Equals(const wchar_t* pcStr) const;
	inline bool EqualsNoCase(const wchar_t* pcStr) const;

	void Append(const wchar_t* pcStr);
	void Append(const wchar_t ch);

	void Format(const wchar_t* format, ...);
	void AppendFormat(const wchar_t* format, ...);

	void TrimLeft(wchar_t ch = L' ');
	void TrimRight(wchar_t ch = L' ');

	void Reverse();
	void ToUpper();
	void ToLower();

	inline operator const wchar_t*() const;
	inline operator wchar_t*();

	inline wchar_t& operator[] (size_t st) const;

	inline const wchar_t* operator=(const wchar_t* pcStr);
	inline const wchar_t* operator=(wchar_t ch);
	inline const wchar_t* operator+=(const wchar_t* pcStr);
	inline const wchar_t* operator+=(wchar_t ch);

protected:
	wchar_t m_str[ size ];
};

#ifndef _UNICODE

#define NkCharsT NkCharsA

/*
template <size_t size> 
class NkCharsT : public NkCharsA<size> 
{
public:
	NkCharsT() {}
	NkCharsT(const char* str) : NkCharsA(str) {}
	NkCharsT(char ch) : NkCharsA(ch) {}
};
*/
#else

template <size_t size>
class NkCharsT : public NkCharsW<size> 
{
	NkCharsT() {}
	NkCharsT(const wchar_t* str) : NkCharsW(str) {}
	NkCharsT(wchar_t ch) : NkCharsW(ch) {}
};

#endif

#include "NkCharsA.inl"
#include "NkCharsW.inl"

