//-------------------------------------------------------------------------------------------------
// NkConsoleOutput.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

class NKCORE_ENTRY NkConsoleOutput
{
public:
	NkConsoleOutput();
	~NkConsoleOutput();

	bool CreateConsole(const char* pcTitle, short usBufX = 128, short usBufY = 1024);
	void CloseConsole();
	void WaitInput();

	HANDLE GetHandle() { return m_hNewScreenBuffer; }

	void SetCursorPosition(SHORT x, SHORT y);
	void SetTextColor(WORD bg, WORD fg);
	
	void Printf(const char* format, ...);
	void Printf(WORD bg, WORD fg, const char* format, ...);
	
	void PutStr(const char* pcStr);
	void PutStr(WORD bg, WORD fg, const char* pcStr);

protected:
	HANDLE m_hOldScreenBuffer;
	HANDLE m_hNewScreenBuffer;
	HANDLE m_hInput;

	BOOL m_bAllocConsole;
};
