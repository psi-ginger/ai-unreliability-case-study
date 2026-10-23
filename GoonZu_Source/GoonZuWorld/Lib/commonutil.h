#ifndef _COMMONUTIL_H
#define _COMMONUTIL_H
	
#include <windows.h>

void MStrCpy( TCHAR *pDest, const TCHAR *pSrc, int maxbuf );
int Nsprintf( TCHAR *buffer, TCHAR *format, ... );

#endif