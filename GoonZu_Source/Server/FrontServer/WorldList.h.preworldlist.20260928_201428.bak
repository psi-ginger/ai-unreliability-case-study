#ifndef _WORLDLIST_H
#define _WORLDLIST_H

#include "FrontMsg.h"

class cltWorldList
{
public:
	cltWorldList();
	~cltWorldList();

	void			Clear();

	int				GetCount() { return m_count; };								// 월드 개수 반환

	
	BOOL			GetWorldInfo( int worldId, WORLDINFO *pWorldInfo );
	BOOL			GetWorldInfo( TCHAR *szIP, WORLDINFO *pWorldInfo );
	BOOL			GetWorldInfoByServerID( SI32 siServerID, WORLDINFO *pWorldInfo );


	BOOL			GetWorldRefreshInfo( int worldId, GAMESERVER_REFRESHINFO *pRefreshInfo );
	BOOL			GetWorldRefreshInfo( TCHAR *szIP, GAMESERVER_REFRESHINFO *pRefreshInfo );
	BOOL			GetWorldRefreshInfoByServerID( SI32 siServerID, GAMESERVER_REFRESHINFO *pRefreshInfo );

	void			SetWorldInfo( int worldId, WORLDINFO *pWorldInfo );
	void			SetWorldRefreshInfo( int worldId, GAMESERVER_REFRESHINFO *pRefreshInfo );

	void			SetWorldList( sResponse_WorldList *packet );
	void			SetWorldRefreshInfo( sNotify_WorldRefreshAll *packet );

	void			MakeWorldListPacket( sResponse_WorldList *packet );
	void			MakeWorldRefreshAllPacket( sNotify_WorldRefreshAll *packet );

public:
	WORLDINFO		m_worldlist[ 100 ];
	int				m_count;


	CRITICAL_SECTION	m_cs;

};

#endif

