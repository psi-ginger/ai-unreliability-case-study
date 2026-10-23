#include "MiniHomeUpdates.h"
#include "Directives.h"
#include "../NLib/NDataTypes.h"

cltMiniHomeUpdates::cltMiniHomeUpdates()
{
	m_siCount = 0;
	m_httpconnect.Init();

	m_bRequest = FALSE;
}

cltMiniHomeUpdates::~cltMiniHomeUpdates()
{
}

void cltMiniHomeUpdates::Request( TCHAR *szAccountID )
{
	WCHAR szID[ 64 ];
	WCHAR szURL[ 256 ];

#ifndef _UNICODE
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szAccountID, -1, szID, 63 );
#else
	StringCchCopy(szID, 64, szAccountID);
#endif
		// goonzu 
		swprintf( szURL, L"http://ccomzi.ndoors.com/mh/history_view.asp?user_id=%s", szID );

	m_httpconnect.HttpOpen();
	m_httpconnect.SendRequest( szURL );

	m_bRequest = TRUE;

}

void cltMiniHomeUpdates::CheckUpdate()
{
	if( m_bRequest == FALSE ) return;
	
	SI16 DestBufSize = strlen(m_httpconnect.m_pRecvUnitBuffer) + 1;

	TCHAR* DestBuffer = new TCHAR[DestBufSize];

#ifndef _UNICODE
	StringCchCopy(DestBuffer, DestBufSize, m_httpconnect.m_pRecvUnitBuffer);
#else
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, m_httpconnect.m_pRecvUnitBuffer, -1, 
		DestBuffer, DestBufSize );
#endif

	int i, len, col, bp;

	if( m_httpconnect.IsComplete() ) {

		m_bRequest = FALSE;

		ZeroMemory( m_message, sizeof( m_message ) );
		
		m_siCount = 0;

		len = strlen( m_httpconnect.m_pResourceBuffer );
		col = 0;
		bp = 0;

		for( i = 0; i < len; ++i ) {

			// 다음 행
			if( DestBuffer[ i ] == '|' ) {

				if( i == 0 ) {
					m_siCount = 0;
				} else {

					setText( m_siCount, col, DestBuffer, bp, i );

					m_siCount++;
				}

				col = 0;
				bp = i + 1;
				continue;
			}

			// 다음 열
			if( DestBuffer[ i ] == '$' ) {
				setText( m_siCount, col, DestBuffer, bp, i );				
				
				col++;
				bp = i + 1;
				continue;
			}
		}
	}

	NDelete(DestBuffer);
}

void cltMiniHomeUpdates::setText( SI32 index, SI32 col, TCHAR *pSrcMsg, SI32 bp, SI32 ep )
{
	if( index >= MAX_MH_MESSAGE_NUMBER ) return;

	TCHAR *pDest;
	int cpylen, maxlen;

	switch( col )
	{
	case 0:
		pDest = m_message[ index ].szKind;
		maxlen = MAX_MH_KIND;
		break;
	case 1:
		pDest = m_message[ index ].szWriter;
		maxlen = MAX_MH_WRITER;
		break;
	case 2:
		pDest = m_message[ index ].szDate;
		maxlen = MAX_MH_DATE;
		break;
	case 3:
		pDest = m_message[ index ].szText;
		maxlen = MAX_MH_TEXT;
		break;
	default:
		return;
	}

	cpylen = ep - bp;
	if( cpylen <= 0 ) {
		pDest[ 0 ] = NULL;
		return;
	}

	if( cpylen > maxlen - 1 ) cpylen = maxlen - 1;

	memcpy( pDest, &pSrcMsg[ bp ], cpylen );
	pDest[ cpylen ] = NULL;
}




