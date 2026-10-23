//-------------------------------------------------------------------------------------------------
// NkString.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define STRSAFE_LIB
#else
#define STRSAFE_INLINE
#endif

#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CB_FUNCTIONS

#include <tchar.h>
#include <strsafe.h>

#include "NkRTLib.h"
#include "NkDebug.h"
#include "NkMemoryManager.h"

//-------------------------------------------------------------------------------------------------
// NkStringA
//-------------------------------------------------------------------------------------------------

class NkStringA : public NkMemory
{
public:
	static const size_t INVALID_INDEX = (size_t)-1;

	NkStringA();
	NkStringA(const NkStringA& kStr);
	NkStringA(const char* pcStr);
	NkStringA(const char ch);
	~NkStringA();

	inline bool IsEmpty() const;
	inline void Empty();

	inline size_t GetLength() const;
	inline const char* GetString() const;

	inline char GetAt(size_t stPos) const;
	inline void SetAt(size_t stPos, char ch);

	inline int Compare(const NkStringA& kStr) const;
	inline int Compare(const char* pcStr) const;

	inline int CompareNoCase(const NkStringA& kStr) const;
	inline int CompareNoCase(const char* pcStr) const;

	inline bool Equals(const NkStringA& kStr) const;
	inline bool Equals(const char* pcStr) const;

	inline bool EqualsNoCase(const NkStringA& kStr) const;
	inline bool EqualsNoCase(const char* pcStr) const;

	void Append(const NkStringA& kStr);
	void Append(const char* pcStr);
	void Append(const char ch);

	void Insert(const char* pcStr, size_t stBegin);
	
	size_t Replace(const char* pcOrignalStr, const char* pcNewStr);
	
	size_t RemoveRange(size_t stBegin, size_t stCount);

	size_t FindChar(const char ch, size_t stStart) const;
	size_t FindStr(const char* pcStr, size_t stStart) const;
	size_t FindOneOf(const char* pcCharSet, size_t stStart) const;

	size_t FindReverseChar(const char ch, size_t start) const;
	size_t FindReverseStr(const char* pcStr, size_t start) const;
	size_t FindReverseOneOf(const char* pcCharSet, size_t stStart) const;

	void GetSubString(size_t stBegin, size_t stCount, NkStringA& kStr);
	
	NkStringA Mid(size_t stBegin, size_t stCount) const;
	NkStringA Left(size_t stCount) const;
	NkStringA Right(size_t stCount) const;
	
	void TrimLeft(char ch = ' ');
	void TrimRight(char ch = ' ');

	void Reverse();
	void ToUpper();
	void ToLower();

	operator const char*() const;

	inline NkStringA& operator=(const NkStringA& kStr);
	inline NkStringA& operator=(const char* pcStr);
	inline NkStringA& operator=(char ch);
	inline NkStringA& operator+=(const NkStringA& kStr);
	inline NkStringA& operator+=(const char* pcStr);
	inline NkStringA& operator+=(char ch);

	void Allocate(size_t stSize);

private:
	void _Allocate(const NkStringA& kStr);
	void _Allocate(const char* pcStr);
	void _Allocate(const char ch);
	void _Deallocate();

	char* m_pcString;
	size_t m_cchLength;
};

inline bool operator==(const NkStringA& kStr1, const NkStringA& kStr2);
inline bool operator==(const NkStringA& kStr1, const char* pcStr2);
inline bool operator==(const char* pcStr1, const NkStringA& kStr2);

inline bool operator!=(const NkStringA& kStr1, const NkStringA& kStr2);
inline bool operator!=(const NkStringA& kStr1, const char* pcStr2);
inline bool operator!=(const char* pcStr1, const NkStringA& kStr2);

inline bool operator<(const NkStringA& kStr1, const NkStringA& kStr2);
inline bool operator<(const NkStringA& kStr1, const char* pcStr2);
inline bool operator<(const char* pcStr1, const NkStringA& kStr2);

inline bool operator>(const NkStringA& kStr1, const NkStringA& kStr2);
inline bool operator>(const NkStringA& kStr1, const char* pcStr2);
inline bool operator>(const char* pcStr1, const NkStringA& kStr2);

inline bool operator<=(const NkStringA& kStr1, const NkStringA& kStr2);
inline bool operator<=(const NkStringA& kStr1, const char* pcStr2);
inline bool operator<=(const char* pcStr1, const NkStringA& kStr2);

inline bool operator>=(const NkStringA& kStr1, const NkStringA& kStr2);
inline bool operator>=(const NkStringA& kStr1, const char* pcStr2);
inline bool operator>=(const char* pcStr1, const NkStringA& kStr2);


//-------------------------------------------------------------------------------------------------
// NkStringW
//-------------------------------------------------------------------------------------------------

class NkStringW : public NkMemory
{
public:
	static const size_t INVALID_INDEX = (size_t)-1;

	NkStringW();
	NkStringW(const NkStringW& kStr);
	NkStringW(const wchar_t* pcStr);
	NkStringW(const wchar_t ch);
	~NkStringW();

	inline bool IsEmpty() const;
	inline void Empty();

	inline size_t GetLength() const;
	inline const wchar_t* GetString() const;

	inline wchar_t GetAt(size_t stPos) const;
	inline void SetAt(size_t stPos, wchar_t ch);

	inline int Compare(const NkStringW& kStr) const;
	inline int Compare(const wchar_t* pcStr) const;

	inline int CompareNoCase(const NkStringW& kStr) const;
	inline int CompareNoCase(const wchar_t* pcStr) const;

	inline bool Equals(const NkStringW& kStr) const;
	inline bool Equals(const wchar_t* pcStr) const;

	inline bool EqualsNoCase(const NkStringW& kStr) const;
	inline bool EqualsNoCase(const wchar_t* pcStr) const;

	void Append(const NkStringW& kStr);
	void Append(const wchar_t* pcStr);
	void Append(const wchar_t ch);

	void Insert(const wchar_t* pcStr, size_t stBegin);

	size_t Replace(const wchar_t* pcOrignalStr, const wchar_t* pcNewStr);

	size_t RemoveRange(size_t stBegin, size_t stCount);

	size_t FindChar(const wchar_t ch, size_t stStart) const;
	size_t FindStr(const wchar_t* pcStr, size_t stStart) const;
	size_t FindOneOf(const wchar_t* pcCharSet, size_t stStart) const;

	size_t FindReverseChar(const wchar_t ch, size_t start) const;
	size_t FindReverseStr(const wchar_t* pcStr, size_t start) const;
	size_t FindReverseOneOf(const wchar_t* pcCharSet, size_t stStart) const;

	void GetSubString(size_t stBegin, size_t stCount, NkStringW& kStr);

	NkStringW Mid(size_t stBegin, size_t stCount) const;
	NkStringW Left(size_t stCount) const;
	NkStringW Right(size_t stCount) const;

	void TrimLeft(wchar_t ch = L' ');
	void TrimRight(wchar_t ch = L' ');

	void Reverse();
	void ToUpper();
	void ToLower();

	operator const wchar_t*() const;

	inline NkStringW& operator=(const NkStringW& kStr);
	inline NkStringW& operator=(const wchar_t* pcStr);
	inline NkStringW& operator=(wchar_t ch);
	inline NkStringW& operator+=(const NkStringW& kStr);
	inline NkStringW& operator+=(const wchar_t* pcStr);
	inline NkStringW& operator+=(wchar_t ch);

	void Allocate(size_t stSize);

private:
	void _Allocate(const NkStringW& kStr);
	void _Allocate(const wchar_t* pcStr);
	void _Allocate(const wchar_t ch);
	void _Deallocate();

	wchar_t* m_pcString;
	size_t m_cchLength;
};

inline bool operator==(const NkStringW& kStr1, const NkStringW& kStr2);
inline bool operator==(const NkStringW& kStr1, const wchar_t* pcStr2);
inline bool operator==(const wchar_t* pcStr1, const NkStringW& kStr2);

inline bool operator!=(const NkStringW& kStr1, const NkStringW& kStr2);
inline bool operator!=(const NkStringW& kStr1, const wchar_t* pcStr2);
inline bool operator!=(const wchar_t* pcStr1, const NkStringW& kStr2);

inline bool operator<(const NkStringW& kStr1, const NkStringW& kStr2);
inline bool operator<(const NkStringW& kStr1, const wchar_t* pcStr2);
inline bool operator<(const wchar_t* pcStr1, const NkStringW& kStr2);

inline bool operator>(const NkStringW& kStr1, const NkStringW& kStr2);
inline bool operator>(const NkStringW& kStr1, const wchar_t* pcStr2);
inline bool operator>(const wchar_t* pcStr1, const NkStringW& kStr2);

inline bool operator<=(const NkStringW& kStr1, const NkStringW& kStr2);
inline bool operator<=(const NkStringW& kStr1, const wchar_t* pcStr2);
inline bool operator<=(const wchar_t* pcStr1, const NkStringW& kStr2);

inline bool operator>=(const NkStringW& kStr1, const NkStringW& kStr2);
inline bool operator>=(const NkStringW& kStr1, const wchar_t* pcStr2);
inline bool operator>=(const wchar_t* pcStr1, const NkStringW& kStr2);


#include "NkStringA.inl"
#include "NkStringW.inl"
