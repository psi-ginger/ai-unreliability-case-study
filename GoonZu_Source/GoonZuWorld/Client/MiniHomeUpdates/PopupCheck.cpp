#include "PopupCheck.h"
#include "Directives.h"
#include "../NLib/NDataTypes.h"

cltPopupCheck::cltPopupCheck()
{
	m_httpconnect.Init();

	m_bRequest		= FALSE;
	m_bPCBang		= FALSE;
	m_bPopup		= FALSE;

	m_FullURL[0]	= '\0';
}

cltPopupCheck::~cltPopupCheck()
{
}

void cltPopupCheck::Request_PopupCheck( TCHAR *pszURL )
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


BOOL cltPopupCheck::CheckUpdate_PopupCheck()
{
	if( m_bRequest == FALSE )					return FALSE;
	if(m_httpconnect.IsComplete() == FALSE )	return FALSE;

	SI16   DestBufSize = strlen(m_httpconnect.m_pRecvUnitBuffer) + 1;
	TCHAR* DestBuffer  = new TCHAR[DestBufSize];
	if(DestBuffer == NULL)
		return FALSE;

	ZeroMemory(DestBuffer, DestBufSize);

	#ifndef _UNICODE
		StringCchCopy(DestBuffer, DestBufSize, m_httpconnect.m_pRecvUnitBuffer);
	#else
		MultiByteToWideChar( CP_ACP, MB_COMPOSITE, m_httpconnect.m_pRecvUnitBuffer, -1, DestBuffer, DestBufSize );
	#endif

	m_bRequest = FALSE;
	m_bPopup   = FALSE;

	TCHAR* pResult = NULL;
	TCHAR* pNextURL = NULL;

	pResult = _tcstok(DestBuffer, "|");
	if(pResult)
	{
		if(_tcscmp(pResult, "1") == 0)	// 팝업 있음
		{
			m_bPopup = TRUE;

			pNextURL = _tcstok(NULL, "|");

			TCHAR* pURL = _tcsstr(pNextURL, "=");
			if(pURL)
			{
				pURL++;
				StringCchCopy(m_FullURL, sizeof(m_FullURL), pURL);
				m_FullURL[sizeof(m_FullURL)-1] = '\0';
			}
		}
	}

//=============================================================================
// DEBUG 테스트시에는 무조건 NAVER.COM 페이지 출력
//#ifdef _DEBUG
//	m_bPopup = TRUE;
//
//	StringCchCopy(m_FullURL, sizeof(m_FullURL), "http://www.naver.com");
//	
//	// 알파테스트 코드. 
//	StringCchPrintf( m_FullURL, sizeof(m_FullURL),
//		"http://alpha-services.ijji.com/service/api/popupCheck?id=searer1&gameid=U_GOONZU&type=0");
//
//	m_FullURL[sizeof(m_FullURL)-1] = '\0';
//#endif // _DEBUG
//=============================================================================

	NDelete(DestBuffer);

	return m_bPopup;
}
