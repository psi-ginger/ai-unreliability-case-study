//-------------------------------------------------------------------------------------------------
// NkStringConv.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"
#include "NkString.h"

class NkStringConv
{
public:
	static int AnsiToWide(const char* pcSrcStr, wchar_t* pcDestStr, int iDestBufSize);
	static int AnsiToWide(const char* pcSrcStr, NkStringW& kDestStr);
	static int AnsiToWide(const NkStringA& kSrcStr, NkStringW& kDestStr);
	
	static int WideToAnsi(const wchar_t* pcSrcStr, char* pcDestStr, int iDestBufSize);
	static int WideToAnsi(const wchar_t* pcSrcStr, NkStringA& kDestStr);
	static int WideToAnsi(const NkStringW& kSrcStr, NkStringA& kDestStr);
};

template <size_t size>
class NkAnsiToWide
{
public:
	inline NkAnsiToWide(const char* pcSrcStr)
	{
		int iLength = lstrlenA(pcSrcStr) + 1;

		NkAssert( iLength <= size );
			
		::MultiByteToWideChar(CP_ACP, 0, pcSrcStr, iLength, m_str, size);
	}

	inline operator wchar_t*() const
	{
		return m_str;
	}

protected:
	wchar_t m_str[size];
};


template <size_t size>
class NkWideToAnsi
{
public:
	inline NkWideToAnsi(const wchar_t* pcSrcStr)
	{
		int iLength = lstrlenW(pcSrcStr) + 1;

		NkAssert( iLength <= size );

		::WideCharToMultiByte(CP_ACP, 0, pcSrcStr, iLength, m_str, size, 0, 0);
	}

	inline operator char*() const
	{
		return m_str;
	}

protected:
	char m_str[size];
};