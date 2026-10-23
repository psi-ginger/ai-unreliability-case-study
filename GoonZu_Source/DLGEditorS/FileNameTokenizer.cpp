#include "stdafx.h"
#include "FileNameTokenizer.h"

CFileNameTok::CFileNameTok()
{
	m_szPath[ 0 ] = NULL;
	m_szFileName[ 0 ] = NULL;
	m_szExt[ 0 ] = NULL;
}

CFileNameTok::~CFileNameTok()
{

}

void CFileNameTok::Initialize()
{
	m_szPath[ 0 ] = NULL;
	m_szFileName[ 0 ] = NULL;
	m_szExt[ 0 ] = NULL;
}

void CFileNameTok::InitFullPath( char *szFullPath )
{
	
	int len = strlen( szFullPath );

	BOOL bGetExt = FALSE;
	int posDot = 0;

	for( int i = len; i >= 0; --i ) {

		if( bGetExt == FALSE ) {

			if( szFullPath[ i ] == '.' ) {

				strcpy( m_szExt, &szFullPath[ i + 1 ] );
				
				bGetExt = TRUE;
				posDot = i;
			}

			continue;
		}

		if( szFullPath[ i ] == '\\' ||
			szFullPath[ i ] == '/' ) {

			int len = posDot - i - 1;
			
			memcpy( m_szFileName, &szFullPath[ i + 1 ], len );
			m_szFileName[ len ] = NULL;

			memcpy( m_szPath, szFullPath, i );
			m_szPath[ i ] = NULL;

			break;
		}

	}
}
