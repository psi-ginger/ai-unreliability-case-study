#ifndef _CURSOR_H
#define _CURSOR_H

#include <Directives.h>

enum {
	GAME_CURSOR_NORMAL = 0,
	GAME_CURSOR_CLICK,
	GAME_CURSOR_ATTACK,
	GAME_CURSOR_GRAB,
	GAME_CURSOR_GATE,
	GAME_CURSOR_NPC,
	GAME_CURSOR_HAMMER,
	GAME_CURSOR_FARM,
	GAME_CURSOR_MINE,
	GAME_CURSOR_SUMMON_ATTACK,
	GAME_CURSOR_LINK,
};


class cltCursor
{
public:
	cltCursor();
	~cltCursor();

	void		LoadCursor( int index, int resourceInt );
	void		LoadCursor( int index, TCHAR *szFilename );
	
	void		SetCursor();
	void		SetCursor( int index );
	void		SetCursorState( int index );

	SI32		GetCursorState() { return m_siCurrentCursor; };

	HCURSOR		m_hCursor[ 32 ];

	int			m_siCurrentCursor;
};

#endif