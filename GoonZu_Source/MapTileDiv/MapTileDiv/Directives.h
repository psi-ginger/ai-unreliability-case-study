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


#endif