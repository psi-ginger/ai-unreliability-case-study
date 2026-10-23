#ifndef _NDATATYPES_H
#define _NDATATYPES_H

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

extern const SI32	MIN_SI32;
extern const SI32	MAX_SI32;
extern const SI16	MIN_SI16;
extern const SI16	MAX_SI16;
extern const SI08	MIN_SI08;
extern const SI08	MAX_SI08;
extern const UI32	MIN_UI32;
extern const UI32	MAX_UI32;
extern const UI16	MIN_UI16;
extern const UI16	MAX_UI16;
extern const UI08	MIN_UI08;
extern const UI08	MAX_UI08;

extern const float	MIN_REAL32;
extern const float	MAX_REAL32;
extern const float	TINY_REAL32;

#endif