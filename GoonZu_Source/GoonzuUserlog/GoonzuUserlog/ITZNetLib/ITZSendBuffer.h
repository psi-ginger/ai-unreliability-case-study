#ifndef _ITZSENDBUFFER_H
#define _ITZSENDBUFFER_H

#include "itzbase.h"

class ITZSendBuffer 
{
public:
	ITZSendBuffer();
	~ITZSendBuffer();

public:
	void			CreateSendBuffer( SI32 size, SI32 headerType = 0 );
	void			ClearBuffer();

	BOOL			IsReadyToSend() { if( m_bComplete && m_siDataLen > 0 ) return TRUE; else return FALSE; };

	BOOL			GetSendParam( char **pSendPtr, SI32 *psiDataLen );

	void			Completion( SI32 siSentBytes );

	BOOL			Write( char *pPacket );

private:	
	char			*m_pSendBuf;
	
	SI32			m_siBufSize;
	SI32			m_siBeginPos;
	SI32			m_siEndPos;
	SI32			m_siDataLen;
	
	SI32			m_siNumPacket;

	BOOL			m_bComplete;

private:
	// 동기화 관련
	CRITICAL_SECTION	m_cs;

private:
	// 멤버 함수에서 사용할 로컬 변수들
	SI32			local_siLenA;
	SI32			local_siLenB;
	SI32			local_siLenC;

	SI32			m_headerType;

};

#endif
