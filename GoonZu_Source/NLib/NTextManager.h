//-----------------------------------------------------------------------------
// 2005/09/13 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#ifndef _NTEXTMANAGER_H
#define _NTEXTMANAGER_H

#define MAX_TEXT_KEY		64

template< class Type >
class NHashTableString;

class NVirtualBuffer;

#include "NArray.h"

class NTextManager
{
public:
	NTextManager();
	~NTextManager();

	void	CreateTextManager();

	bool	LoadTextFromFile( TCHAR *filename, int setcolumn );
	bool	LoadTextFromCryptFile( TCHAR *filename, int setcolumn );
	
	TCHAR*	GetText( TCHAR *strKey );
	TCHAR*	GetSafeText( TCHAR *strKey );

public:
	//NVirtualBuffer*					m_pVBuffer;
	//TCHAR*		 					m_pBuffer;
	NSafeTMemArray<TCHAR>			m_pBuffer;
	NHashTableString< TCHAR* >*		m_pHashTable;

	int								m_iCurBufPos;
};


#endif