#include "commonutil.h"
#include "Directives.h"
//#include <strsafe.h>
//#include <windows.h>

void MStrCpy( TCHAR *pDest, const TCHAR *pSrc, int maxbuf )
{
	if( pDest == NULL ) return;

	if( pSrc == NULL ) {
		pDest[ 0 ] = NULL;
	
	} else {
		StringCchCopy( pDest, maxbuf, pSrc );
	}
}

//-----------------------------------------------------------------------
// 2005/06/03 이제완
//
// Nsprintf( buffer, TEXT("Hello! My name is #name#! Nice to meet you. #age#"),
//			 TEXT("name"), TEXT("Jewan"),
//			 TEXT("age"), TEXT("26"),
//			 NULL );
//
//
// buffer 출력결과
//
// Hello! My name is Jewan! Nice to meet you.
//-----------------------------------------------------------------------
int Nsprintf( TCHAR *buffer, TCHAR *format, ... )
{
	TCHAR	param[ 32 ][ 64 ];
	TCHAR*	ptr[ 32 ];

	TCHAR*	temp;

	int		count = 0;

	int		dest_endpos = 0;

	int		src_startpos = 0;
	int		src_curpos = 0;

	int		len;

	BOOL	bControl = FALSE;
	TCHAR	szControl[ 64 ];

	int		i;

	va_list ap;
	va_start( ap, format );

	bool loop = true;
	while( loop ) 
	{
		temp = va_arg( ap, TCHAR* );
		if( temp == NULL ) break;
		
		StringCchCopy( param[ count ], 64, temp );

		ptr[ count ] = va_arg( ap, TCHAR* );

		count++;
	}

	va_end( ap );

	while( loop ) 
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
						if( _tcscmp( szControl, param[ i ] ) == 0 ) {

							len = _tcslen( ptr[ i ] );
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

