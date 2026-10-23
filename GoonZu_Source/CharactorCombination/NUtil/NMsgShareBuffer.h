#ifndef _NMSGSHAREBUFFER_H
#define _NMSGSHAREBUFFER_H

#include <windows.h>
#include <stdio.h>

#define NMSGSHAREBUFFER_NAME	"NMSGSHAREBUFFER"

struct NMsgShareBufferInfo
{
	char		name[ 16 ];
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

	void					CreateMsgShareBuffer( int maxmsgcount, char *szName );
	void					WriteMsg( NMsg *pMsg );
	void					WriteMsg( int msg, int param1, int param2, int time );


	void					Wait() { WaitForSingleObject( m_hMutex, INFINITE ); }
	void					Release() { ReleaseMutex( m_hMutex ); }

	HANDLE					m_hMapFile;
	HANDLE					m_hMutex;

	NMsgShareBufferInfo*	m_pInfo;
	NMsg*					m_pMsg;

	char*					m_pMapPtr;

	int						m_mapsize;
};

#endif