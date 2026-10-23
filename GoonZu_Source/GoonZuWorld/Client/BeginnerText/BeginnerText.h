#pragma once

#include <Directives.h>

class CBeginnerText
{
private:
	
	SI32		m_siIndex;
	DWORD		m_dwLastShowClock;
	TCHAR**		m_szHelpText;


public:

	CBeginnerText();
	~CBeginnerText();

	void Action();
	void LoadBeginnerText( TCHAR* BeginnerText );
	TCHAR* GetHelpText( SI32 Index );
};