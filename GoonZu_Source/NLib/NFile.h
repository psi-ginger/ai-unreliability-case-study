//-----------------------------------------------------------------------------
// 2005/11/19 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------


#ifndef _NFILE_H
#define _NFILE_H

#include <windows.h>

class NFile
{
public:
	NFile();
	~NFile();

	bool		LoadFile( TCHAR *filename );
	BYTE*		GetBuffer() { return m_pBuffer; };

	BYTE*		m_pBuffer;

	DWORD		m_dwBufferSize;
	DWORD		m_dwFileSize;
};

#endif

