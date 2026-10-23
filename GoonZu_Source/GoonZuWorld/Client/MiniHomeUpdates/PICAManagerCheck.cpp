#include "PICAManagerCheck.h"
#include "Directives.h"
#include "../NLib/NDataTypes.h"

cltPICAManagerCheck::cltPICAManagerCheck()
{
	m_httpconnect.Init();

	m_bRequest		= FALSE;
	m_bPCBang		= FALSE;
}

cltPICAManagerCheck::~cltPICAManagerCheck()
{
}

void cltPICAManagerCheck::Request_PICAManager( TCHAR *pszURL )
{
	if(pszURL == NULL)						return;

	WCHAR szURL[ 256 ];
	WCHAR szData[ 128 ];
	
	ZeroMemory(szURL, sizeof(szURL));
	ZeroMemory(szData, sizeof(szData));

#ifndef _UNICODE
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pszURL, -1, szData, 127 );
#else
	StringCchCopy(szData, 128, szAccountID);
#endif

	swprintf( szURL, L"%s", szData );

	m_httpconnect.HttpOpen();
	m_httpconnect.SendRequest( szURL );

	m_bRequest = TRUE;
}


BOOL cltPICAManagerCheck::CheckUpdate_PICAManager()
{
	if( m_bRequest == FALSE )					return FALSE;
	if(m_httpconnect.IsComplete() == FALSE )	return FALSE;

	SI16 DestBufSize = strlen(m_httpconnect.m_pRecvUnitBuffer) + 1;

	TCHAR* DestBuffer = new TCHAR[DestBufSize];
	if(DestBuffer == NULL)						return FALSE;

	ZeroMemory(DestBuffer, DestBufSize);

#ifndef _UNICODE
	StringCchCopy(DestBuffer, DestBufSize, m_httpconnect.m_pRecvUnitBuffer);
#else
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, m_httpconnect.m_pRecvUnitBuffer, -1, DestBuffer, DestBufSize );
#endif

	m_bRequest = FALSE;

	if(_tcsncmp(DestBuffer, "Y", 1) == 0)
	{
		m_bPCBang = TRUE;
	}
	else
	{
		m_bPCBang = FALSE;
	}
	NDelete(DestBuffer);

	return TRUE;
}
