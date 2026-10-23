//-----------------------------------------------------------------------------
// 2005/09/20 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#ifndef _NMSGSHAREBUFFER_H
#define _NMSGSHAREBUFFER_H

#include "NDataTypes.h"

#define NMSGSHAREBUFFER_NAME	TEXT("NMSGSHAREBUFFER")

struct NMsgShareBufferInfo
{
	TCHAR		name[ 16 ];
	int			size;
	int			beginpos;
	int			endpos;
	int			count;
};

struct NMsg
{
	int			msg;
	int			param1;
	int			param2;
	int			time;
};

class NMsgShareBuffer
{
public:
	NMsgShareBuffer();
	~NMsgShareBuffer();

	void					Destroy();

	void					CreateMsgShareBuffer( int maxmsgcount, TCHAR *szName );
	void					WriteMsg( NMsg *pMsg );
	void					WriteMsg( int msg, int param1, int param2, int time );


	void					Wait() { WaitForSingleObject( m_hMutex, INFINITE ); }
	void					Release() { ReleaseMutex( m_hMutex ); }

	HANDLE					m_hMapFile;
	HANDLE					m_hMutex;

	NMsgShareBufferInfo*	m_pInfo;
	NMsg*					m_pMsg;

	BYTE*					m_pMapPtr;

	int						m_mapsize;
};

#endif