#ifndef _PROMPT_H_
#define _PROMPT_H_

#include "PromptBaseDefinition.h"

class CPrompt
{
public:
	CPrompt();
	~CPrompt();

	void	Initialize();
	void	Destroy();

	CPrompt	*GetNext();
	void	SetNext( CPrompt *pNext );

	CPrompt	*GetPre();
	void	SetPre( CPrompt	*pPre );

	void	SetData( TCHAR *strPrompt, TCHAR* strURL, SI32	si32RepetitionNum, COLORREF TextColor );

	TCHAR	*GetPrompt();
	SI32	GetPromptLength();	

	TCHAR*	GetURL();

	bool	IsSameRepetitionCount();

	CPrompt	*ShiftLeft();

	SI32	GetCurrentDrawPos();

	bool	GetMoveStatus();
	void	SetMoveStatus( bool bStatus );

	bool	IsSame( TCHAR *strData );

	COLORREF m_TextColor;

private:
	
	TCHAR	m_strPrompt[ MAX_PROMT_STRING_LENGTH ];
	TCHAR	m_strURL[ MAX_PATH ];
	SI32	m_si32RepetitionNum;
	SI32	m_si32CurrentRepetitionCount;

	SI32	m_siCurrentDrawPos;

	bool	m_bMoveStatus;

	CPrompt	*m_pPre;
	CPrompt	*m_pNext;

};


#endif