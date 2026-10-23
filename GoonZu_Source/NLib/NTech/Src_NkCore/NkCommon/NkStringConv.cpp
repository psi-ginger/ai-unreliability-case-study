//-------------------------------------------------------------------------------------------------
// NkStringConv.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------


#include "NkStringConv.h"

int NkStringConv::AnsiToWide(const char* pcSrcStr, wchar_t* pcDestStr, int iDestBufSize)
{
	int iLength = lstrlenA(pcSrcStr) + 1;

	if( iLength > iDestBufSize ) return 0;

	return ::MultiByteToWideChar(CP_ACP, 0, pcSrcStr, iLength, pcDestStr, iDestBufSize);
}

int NkStringConv::AnsiToWide(const char* pcSrcStr, NkStringW& kDestStr)
{
	int iLength = lstrlenA(pcSrcStr) + 1;

	kDestStr.Allocate(iLength);

	int iRet = ::MultiByteToWideChar(CP_ACP, 0, pcSrcStr, iLength, (LPWSTR)(const wchar_t*)kDestStr, iLength);

	if( iRet == 0 )
		kDestStr.Empty();

	return iRet;
}

int NkStringConv::AnsiToWide(const NkStringA& kSrcStr, NkStringW& kDestStr)
{
	int iLength = (int)kSrcStr.GetLength() + 1;

	kDestStr.Allocate(iLength);

	int iRet = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(const char*)kSrcStr, iLength, (LPWSTR)(const wchar_t*)kDestStr, iLength);

	if( iRet == 0 )
		kDestStr.Empty();

	return iRet;
}


int NkStringConv::WideToAnsi(const wchar_t* pcSrcStr, char* pcDestStr, int iDestBufSize)
{
	int iLength = lstrlenW(pcSrcStr) + 1;

	if( iLength > iDestBufSize ) return 0;

	return ::WideCharToMultiByte(CP_ACP, 0, pcSrcStr, iLength, pcDestStr, iDestBufSize, 0, 0);
}

int NkStringConv::WideToAnsi(const wchar_t* pcSrcStr, NkStringA& kDestStr)
{
	int iLength = lstrlenW(pcSrcStr) + 1;

	kDestStr.Allocate(iLength);

	int iRet = ::WideCharToMultiByte(CP_ACP, 0, pcSrcStr, iLength, (LPSTR)(const char*)kDestStr, iLength, 0, 0);

	if( iRet == 0 )
		kDestStr.Empty();

	return iRet;
}

int NkStringConv::WideToAnsi(const NkStringW& kSrcStr, NkStringA& kDestStr)
{
	int iLength = (int)kSrcStr.GetLength() + 1;

	kDestStr.Allocate(iLength);

	int iRet = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)(const wchar_t*)kSrcStr, iLength, (LPSTR)(const char*)kDestStr, iLength, 0, 0);

	if( iRet == 0 )
		kDestStr.Empty();

	return iRet;
}