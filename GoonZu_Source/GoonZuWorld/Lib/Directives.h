#ifndef _DIRECTIVES_H
#define _DIRECTIVES_H

typedef signed int			SI32;
typedef signed short		SI16;
typedef signed char			SI08;

typedef unsigned int		UI32;
typedef unsigned short		UI16;
typedef unsigned char		UI08;

typedef __int64				SI64;
typedef unsigned __int64	UI64;

typedef double				REAL64;
typedef float				REAL32;


#define	TABS(x)				(int)abs( (int)(x) )
#define TSQRT(x)			sqrt( (double)(x) )

#include <windows.h>
#include <tchar.h>

#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CB_FUNCTIONS
#include <strsafe.h>

#ifdef _UNICODE

#define TMemCpy( dest, src, count )		wmemcpy( dest, src, count )
#define TMemSet( dest, c, count )		wmemset( dest, c, count )
#define tcsupr( dest )					_wcsupr( dest )

#else

#define TMemCpy( dest, src, count )		memcpy( dest, src, count )
#define TMemSet( dest, c, count )		memset( dest, c, count )
#define tcsupr( dest )					_strupr( dest )

#endif

#endif