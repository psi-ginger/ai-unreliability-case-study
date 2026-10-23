#ifndef _ITZRECVBUFFER_H
#define _ITZRECVBUFFER_H

#include "itzbase.h"

class ITZRecvBuffer 
{
public:
	ITZRecvBuffer();
	~ITZRecvBuffer();

public:
	void			CreateRecvBuffer( SI32 size, SI32 headerType = 0 );
	void			ClearBuffer();
	
	void			GetRecvParam( char **pRecvPtr, SI32 *psiFreeSize );

	BOOL			Completion( SI32 siRecvSize );

	char*			GetFirstPacket();
	void			RemoveFirstPacket();

	SI32			GetPacketCount() { return m_siNumPacket; };

	SI32			GetMaxSize() { return m_siMaxSize; };
	SI32			GetCurrentSize() { 
		int t = m_pLastPtr - m_pBeginPtr;
		if( t < 0 ) return m_siMaxSize - t;
		return t;
	};

public:	
	SI32			m_siMaxSize;
	
	char			*m_pRecvBuf;

	char			*m_pBasePtr;
	char			*m_pBeginPtr;
	char			*m_pCompletePtr;
	char			*m_pLastPtr;
	char			*m_pEndPtr;

	SI32			m_siNumPacket;
		
	BOOL			m_bRemovable;			

private:
	CRITICAL_SECTION	m_cs;

	SI32			m_headerType;

};

#endif
