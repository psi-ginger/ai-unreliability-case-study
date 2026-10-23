
#include "VersionInfo.h"


#define SECTION_PATCH		1

VersionInfo::VersionInfo()
{
	m_uiVersionCount = 0;
}

VersionInfo::~VersionInfo()
{
	
}

void VersionInfo::ReadFromFile()
{

	DWORD dwBytesRead;

	HANDLE hFile = CreateFile( "versioninfo.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0 );

	if( hFile == INVALID_HANDLE_VALUE ) return;


	BOOL b;

	char	ch;
	char	buf[ 3 ][ 128 ];
		
	int		bufindex = 0;
	int		section = 0;
	
	int		len		= 0;
	BOOL	bStr	= FALSE;

	BOOL	bRun	= TRUE;
	BOOL	bDQ		= FALSE;
	BOOL	bProcess = FALSE;

	while( bRun ) {

		b = ReadFile( hFile, &ch, 1, &dwBytesRead, NULL );

		if( dwBytesRead == 0 ) { ch = 13; bRun = FALSE; };

		if( bStr ) {

			if( bDQ == TRUE && ch == '\"' ) { bDQ = FALSE; ch = ' '; };

			if( bDQ == FALSE && ( ch == ' ' || ch == '=' || ch == 9 ) ) {

				buf[ bufindex ][ len ] = NULL;
				if( bufindex < 2 ) bufindex++;

				len = 0;
				bStr = FALSE;

			} else if( ch == 10 || ch == 13 ) {

				buf[ bufindex ][ len ] = NULL;
				if( bufindex < 2 ) bufindex++;

				bStr = FALSE;

			} else {

				buf[ bufindex ][ len ] = ch;
				len++;
			}

		} else {

			if( ch == '\"' ) { 

				bStr = TRUE;
				bDQ = TRUE;

			} else if( !(ch == ' ' || ch == '=' || ch == 9 || ch == 10 || ch == 13) ) {

				buf[ bufindex ][ len ] = ch;
				len++;

				bStr = TRUE;
				bProcess = FALSE;
			}

		}

		if( bProcess == FALSE && ( ch == 10 || ch == 13 ) ) {

			if( bufindex == 1 ) {

				if( buf[ 0 ][ 0 ] == '/' && buf[ 0 ][ 1 ] == '/' ) {

				} else if( _stricmp( buf[ 0 ], "[patch]" ) == 0 ) {
					section = SECTION_PATCH;
				}

			} else if( bufindex > 1 ) {

				switch( section ) {
				case SECTION_PATCH:

					_setPatch( buf[ 0 ], buf[ 1 ] );
					break;
				};
				
			}

			bufindex = 0;
			len = 0;
			bDQ = FALSE;
			bProcess = TRUE;
		}

	}

	CloseHandle( hFile );
}

void VersionInfo::_setPatch( char *buf1, char *buf2 )
{

	if( _stricmp( buf1, "version" ) == 0 ) {

		m_uiVersionList[ m_uiVersionCount++ ] = atoi( buf2 );
	}
}


void VersionInfo::printInfo()
{

	printf( "Version Count : %d\n", m_uiVersionCount );

	for( UI32 i = 0; i < m_uiVersionCount; ++i ) {

		printf( "Version = %d\n", m_uiVersionList[ i ] );
	}
	

}