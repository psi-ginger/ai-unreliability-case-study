#include "stdafx.h"
#include "commonutil.h"

#include <windows.h>

void MStrCpy( char *pDest, const char *pSrc, int maxbuf )
{
	if( pSrc == NULL ) {
		pDest[ 0 ] = NULL;
	
	} else {
		memcpy( pDest, pSrc, maxbuf );
		pDest[ maxbuf - 1 ] = NULL;
	}
}

//-----------------------------------------------------------------------
// 2005/06/03 이제완
//
// Nsprintf( buffer, "Hello! My name is #name#! Nice to meet you. #age#",
//			 "name", "Jewan",
//			 "age", "26",
//			 NULL );
//
//
// buffer 출력결과
//
// Hello! My name is Jewan! Nice to meet you.
//-----------------------------------------------------------------------
int Nsprintf( char *buffer, char *format, ... )
{
	char	param[ 32 ][ 64 ];
	char*	ptr[ 32 ];

	char*	temp;

	int		count = 0;

	int		dest_endpos = 0;

	int		src_startpos = 0;
	int		src_curpos = 0;

	int		len;

	BOOL	bControl = FALSE;
	char	szControl[ 64 ];

	int		i;

	va_list ap;
	va_start( ap, format );

	while( 1 ) 
	{
		temp = va_arg( ap, char* );
		if( temp == NULL ) break;
		
		strcpy( param[ count ], temp );

		ptr[ count ] = va_arg( ap, char* );

		count++;
	}

	va_end( ap );

	while( 1 ) 
	{
		if( format[ src_curpos ] == NULL ) {
			len = src_curpos - src_startpos;
			memcpy( &buffer[ dest_endpos ], &format[ src_startpos ], len );
			dest_endpos += len;
			break;
		}

		if( format[ src_curpos ] == '#' ) {

			if( bControl ) {

				len = src_curpos - src_startpos;
				
				if( len > 0 ) {

					memcpy( szControl, &format[ src_startpos ], len );
					szControl[ len ] = NULL;

					for( i = 0; i < count; ++i ) 
					{
						if( strcmp( szControl, param[ i ] ) == 0 ) {

							len = strlen( ptr[ i ] );
							memcpy( &buffer[ dest_endpos ], ptr[ i ], len );

							dest_endpos += len;
							break;
						}
					}
				}

				src_startpos = src_curpos + 1;
				bControl = FALSE;
			
			} else {

				len = src_curpos - src_startpos;

				if( len > 0 ) {

					memcpy( &buffer[ dest_endpos ], 
					  	    &format[ src_startpos ], 
						    len );

					dest_endpos += len;
				}

				src_startpos = src_curpos + 1;
				bControl = TRUE;
			}
		}

		src_curpos++;
	}

	buffer[ dest_endpos ] = NULL;

	return 0;
}