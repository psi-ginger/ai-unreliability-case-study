//-----------------------------------------------------------------------------
// 2005/11/19 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------


#ifndef _NBUFFER_H
#define _NBUFFER_H

#include "NDataTypes.h"

class NBuffer
{
public:
	NBuffer();
	~NBuffer();

	void				CreateBuffer( int size );
	void				ClearBuffer();

	BYTE*				GetBuffer() { return m_pBuffer; }
	DWORD				GetBufferSize() { return m_dwBufferSize; }

	BYTE*				m_pBuffer;
	DWORD				m_dwBufferSize;
};


#endif