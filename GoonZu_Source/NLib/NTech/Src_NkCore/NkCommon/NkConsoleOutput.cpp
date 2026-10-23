#include "NkConsoleOutput.h"

NkConsoleOutput::NkConsoleOutput()
{
	m_bAllocConsole = FALSE;

	m_hOldScreenBuffer = INVALID_HANDLE_VALUE;
	m_hNewScreenBuffer = INVALID_HANDLE_VALUE;
	m_hInput = INVALID_HANDLE_VALUE;
}

NkConsoleOutput::~NkConsoleOutput()
{
	CloseConsole();
}

bool NkConsoleOutput::CreateConsole(const char* pcTitle, short usBufX, short usBufY)
{

	m_bAllocConsole = AllocConsole();

	m_hOldScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE); 

	m_hNewScreenBuffer = CreateConsoleScreenBuffer( 
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CONSOLE_TEXTMODE_BUFFER, 
		NULL );

	if( m_hOldScreenBuffer == INVALID_HANDLE_VALUE ||
		m_hNewScreenBuffer == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	SetConsoleActiveScreenBuffer(m_hNewScreenBuffer);

	SetConsoleTitle(pcTitle);

	COORD coord;
	coord.X = usBufX;
	coord.Y = usBufY;
	SetConsoleScreenBufferSize(m_hNewScreenBuffer, coord);

	return true;
}


void NkConsoleOutput::CloseConsole()
{
	if( m_hOldScreenBuffer != INVALID_HANDLE_VALUE &&
		m_hNewScreenBuffer != INVALID_HANDLE_VALUE )
	{
		SetConsoleActiveScreenBuffer( m_hOldScreenBuffer );

		CloseHandle( m_hNewScreenBuffer );
		m_hNewScreenBuffer = INVALID_HANDLE_VALUE;
	}

	if( m_bAllocConsole )
	{
		FreeConsole();
		m_bAllocConsole = FALSE;
	}
}

void NkConsoleOutput::WaitInput()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	if( hInput == INVALID_HANDLE_VALUE ) return;

	SetConsoleMode( hInput, ENABLE_MOUSE_INPUT );

	INPUT_RECORD rec;
	DWORD dwRead;

	bool bInfinity = true;
	while(bInfinity)
	{
		ReadConsoleInput( hInput, &rec, 1, &dwRead );

		//if( rec.EventType == KEY_EVENT )
		if( rec.EventType == MOUSE_EVENT && (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) )
		{
			break;
		}
	}
	
}

void NkConsoleOutput::SetCursorPosition(SHORT x, SHORT y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition( m_hNewScreenBuffer, coord );
}

void NkConsoleOutput::SetTextColor(WORD bg, WORD fg)
{
	WORD wAttb = (( bg << 4 ) | fg) & 0x00FF;
	
	SetConsoleTextAttribute( m_hNewScreenBuffer, wAttb );
}

void NkConsoleOutput::Printf(const char* format, ...)
{
	char buf[ 1024 ];

	va_list arg;
	va_start( arg, format );

	_vsnprintf( buf, 1024 - 1, format, arg );
	buf[ 1024 - 1 ] = '\0';

	DWORD dwWritten;
	WriteConsole( m_hNewScreenBuffer, buf, (DWORD)strlen(buf), &dwWritten, 0 );	

	va_end( arg );
}

void NkConsoleOutput::Printf(WORD bg, WORD fg, const char* format, ...)
{
	CONSOLE_SCREEN_BUFFER_INFO kInfo;
	GetConsoleScreenBufferInfo(m_hNewScreenBuffer, &kInfo);

	WORD wOldAttb = kInfo.wAttributes;

	WORD wAttb = (( bg << 4 ) | fg) & 0x00FF;
	SetConsoleTextAttribute(m_hNewScreenBuffer, wAttb);

	char buf[ 1024 ];

	va_list arg;
	va_start( arg, format );

	_vsnprintf( buf, 1024 - 1, format, arg );
	buf[ 1024 - 1 ] = '\0';

	DWORD dwWritten;
	WriteConsole( m_hNewScreenBuffer, buf, (DWORD)strlen(buf), &dwWritten, 0 );	

	va_end( arg );

	SetConsoleTextAttribute(m_hNewScreenBuffer, wOldAttb);
}

void NkConsoleOutput::PutStr(const char* pcStr)
{
	DWORD dwWritten;
	WriteConsole( m_hNewScreenBuffer, pcStr, (DWORD)strlen(pcStr), &dwWritten, 0 );
}

void NkConsoleOutput::PutStr(WORD bg, WORD fg, const char* pcStr)
{
	CONSOLE_SCREEN_BUFFER_INFO kInfo;
	GetConsoleScreenBufferInfo(m_hNewScreenBuffer, &kInfo);

	WORD wOldAttb = kInfo.wAttributes;

	WORD wAttb = (( bg << 4 ) | fg) & 0x00FF;
	SetConsoleTextAttribute(m_hNewScreenBuffer, wAttb);

	DWORD dwWritten;
	WriteConsole( m_hNewScreenBuffer, pcStr, (DWORD)strlen(pcStr), &dwWritten, 0 );	

	SetConsoleTextAttribute(m_hNewScreenBuffer, wOldAttb);
}