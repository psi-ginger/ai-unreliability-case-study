#include "Prompt.h"
#include "../lib/commonutil.h"

CPrompt::CPrompt()
{	
	Initialize();
}

CPrompt::~CPrompt()
{
	Destroy();
}

void CPrompt::Initialize()
{
	m_bMoveStatus = false;

	m_pPre = NULL;
	m_pNext = NULL;

	m_si32RepetitionNum = 0;
	m_si32CurrentRepetitionCount = 0;

	m_siCurrentDrawPos = PROMPT_DRAW_WIDTH;

	memset( m_strPrompt, 0, MAX_PROMT_STRING_LENGTH );
	memset( m_strURL, 0, MAX_PATH );

	return;
}

void CPrompt::Destroy()
{
	m_pPre = NULL;
	m_pNext = NULL;

	m_si32RepetitionNum = 0;
	m_si32CurrentRepetitionCount = 0;

	return;
}

CPrompt	*CPrompt::GetNext()
{
	return m_pNext;
}

void CPrompt::SetNext( CPrompt *pNext )
{
	m_pNext = pNext;
	return;
}

CPrompt	*CPrompt::GetPre()
{
	return	m_pPre;
}

void CPrompt::SetPre( CPrompt *pPre )
{
	m_pPre = pPre;
	return;
}

void CPrompt::SetData( TCHAR *strPrompt, TCHAR* strURL, SI32	si32RepetitionNum, COLORREF TextColor )
{
	if ( _tcslen( strPrompt ) >= MAX_PROMT_STRING_LENGTH - 1 )		// -1 은 스트링 마지막에 있는 NULL 땜시...
	{
		_tcsncpy( m_strPrompt, strPrompt, MAX_PROMT_STRING_LENGTH - 1 );
		m_strPrompt[ MAX_PROMT_STRING_LENGTH - 1 ] = NULL;
	}
	else
	{
		StringCchCopy( m_strPrompt, MAX_PROMT_STRING_LENGTH, strPrompt );
	}

	if ( _tcslen( strURL ) >= MAX_PATH - 1 )		// -1 은 스트링 마지막에 있는 NULL 땜시...
	{
		_tcsncpy( m_strURL, strURL, MAX_PATH - 1 );
		m_strURL[ MAX_PATH - 1 ] = NULL;
	}
	else
	{
		MStrCpy(m_strURL ,strURL, MAX_PATH);
	}
	
	m_si32RepetitionNum = si32RepetitionNum;

	m_TextColor = TextColor;

	return;
}

TCHAR *CPrompt::GetPrompt()
{
	return m_strPrompt;
}
TCHAR* CPrompt::GetURL()
{
	return m_strURL;
}

SI32 CPrompt::GetPromptLength()
{
	return _tcslen( m_strPrompt );
}

CPrompt* CPrompt::ShiftLeft()
{
	if ( ! m_bMoveStatus )
	{
		return NULL;
	}

	m_siCurrentDrawPos -= 2;

	if ( m_siCurrentDrawPos < -PROMPT_DRAW_WIDTH )
	{
		++m_si32CurrentRepetitionCount;
		m_siCurrentDrawPos = PROMPT_DRAW_WIDTH;
		m_bMoveStatus = false;
		return this;
	}

	return NULL;
}

bool CPrompt::IsSameRepetitionCount()
{
	if ( m_si32CurrentRepetitionCount == m_si32RepetitionNum )
	{
		return true;
	}
	return false;
}

SI32 CPrompt::GetCurrentDrawPos()
{
	return m_siCurrentDrawPos;
}


bool CPrompt::GetMoveStatus()
{
	return m_bMoveStatus;
}

void CPrompt::SetMoveStatus( bool bStatus )
{
	m_bMoveStatus = bStatus;
	return;
}

bool CPrompt::IsSame( TCHAR *strData )
{
	if ( m_strPrompt == NULL || strData == NULL )
	{
		return false;
	}

	if ( _tcscmp( m_strPrompt, strData ) == 0 )
	{
		return true;
	}

	return false;
}
