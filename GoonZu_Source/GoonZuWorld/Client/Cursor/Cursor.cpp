#include "Cursor.h"
#include "..\..\Commonlogic\Commonlogic.h"
#include "..\..\resource.h"

extern cltCommonLogic *pclClient;




cltCursor::cltCursor()
{
		
	m_siCurrentCursor = 0;

	for( int i = 0; i < 32; ++i ) {

		m_hCursor[ i ] = NULL;
	}

	
	LoadCursor( GAME_CURSOR_NORMAL, TEXT("Cursor/cursor_normal.ani") );
	LoadCursor( GAME_CURSOR_CLICK, TEXT("Cursor/cursor_click.ani") );	
	LoadCursor( GAME_CURSOR_ATTACK, TEXT("Cursor/cursorani_attack.ani") );
	LoadCursor( GAME_CURSOR_GRAB, TEXT("Cursor/cursorani_grab.ani") );
	LoadCursor( GAME_CURSOR_GATE, TEXT("Cursor/cursorani_gate.ani") );
	LoadCursor( GAME_CURSOR_NPC, TEXT("Cursor/cursorani_npc.ani") );
	LoadCursor( GAME_CURSOR_HAMMER, TEXT("Cursor/cursorani_hammer.ani") );
	LoadCursor( GAME_CURSOR_FARM, TEXT("Cursor/cursorani_farm.ani") );
	LoadCursor( GAME_CURSOR_MINE, TEXT("Cursor/cursorani_mine.ani") );
	LoadCursor( GAME_CURSOR_SUMMON_ATTACK, TEXT("Cursor/cursorani_smatk.ani") );
	LoadCursor( GAME_CURSOR_LINK, TEXT("Cursor/cursorani_html.ani") );

}

cltCursor::~cltCursor()
{
	for( int i = 0; i < 32; ++i ) {

		if( m_hCursor[ i ] ) {
			DestroyCursor( m_hCursor[ i ] );
		}
		
		m_hCursor[ i ] = NULL;
	}
}

void cltCursor::LoadCursor( int index, int resourceInt )
{
	m_hCursor[ index ] = ::LoadCursor( pclClient->GetInstance(), MAKEINTRESOURCE( resourceInt ) );
}

void cltCursor::LoadCursor( int index, TCHAR *szFilename )
{
	m_hCursor[ index ] = ::LoadCursorFromFile( szFilename );
}


void cltCursor::SetCursor( int index )
{
	if( index == m_siCurrentCursor ) return;
	
	SetCursorState( index );
	SetCursor();
}

void cltCursor::SetCursor() 
{
	if( m_hCursor[ m_siCurrentCursor ] == NULL ) return;

	::SetCursor( m_hCursor[ m_siCurrentCursor ] );
}	

void cltCursor::SetCursorState( int index )
{
	m_siCurrentCursor = index;
}



