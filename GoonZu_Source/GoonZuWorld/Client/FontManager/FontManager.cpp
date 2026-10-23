#include "FontManager.h"

#include "../Client.h"
extern cltCommonLogic* pclClient;

BOOL			CFontManager::m_bInstallFont[MAX_INSTALL_FONT];
NTCHARString64	CFontManager::m_kFontFileName[MAX_INSTALL_FONT];
NTCHARString64	CFontManager::m_kValueName[MAX_INSTALL_FONT];
NTCHARString64	CFontManager::m_kFontName[MAX_INSTALL_FONT];

CFontManager	g_FontManager;

CFontManager::CFontManager()
{
	Initialize();
}

CFontManager::~CFontManager()
{	
	Destroy();
}

void CFontManager::Initialize()
{
	m_si32FontNum = 0;

	for ( int i = 0; i < MAX_FONT_NUM; ++i )
	{
		m_hFont[ i ] = NULL;
		m_si32FontHeight[ i ] = 0;
	}

	memset( m_strFontKind, 0, MAX_FONT_NUM * 32 );
	ZeroMemory(m_bInstallFont, sizeof(m_bInstallFont));


	m_kFontFileName[0]	= TEXT("TimeNTales.ttf");
	m_kValueName[0]		= TEXT("TimeNTales");
	m_kFontName[0]		= TEXT("TimeNTales");

	//===================================================================
	// CUSTOM FONT INSTALL
	//===================================================================
//	InstallCustomFont(pclClient->GetHwnd());

	return;
}

void CFontManager::Destroy()
{
	for ( int i = 0; i < m_si32FontNum; ++i )
	{
		if ( m_hFont[ i ] )
		{
			DeleteObject( m_hFont[ i ] );
			m_hFont[ i ] = NULL;
		}
	}

	return;
}

bool CFontManager::IsExistFont( SI32 si32Height, TCHAR *strFontKind, int &si32ExistIndex )
{
	for ( int i = 0; i < m_si32FontNum; ++i )
	{
		if ( m_strFontKind[ i ] == NULL )
		{
			continue;
		}

		if ( _tcscmp( strFontKind, m_strFontKind[ i ] ) == 0 && m_si32FontHeight[ i ] == si32Height ) 
		{
			si32ExistIndex = i;
			return true;
		}
	}
	
	si32ExistIndex = -1;

	return false;
}

void CFontManager::SetFont( HWND hDlg, SI32 si32ResourceID, SI32 si32Height, TCHAR *strFontKind )
{
	if ( m_si32FontNum > MAX_FONT_NUM )
	{
		return;
	}

	HWND	hResource = GetDlgItem( hDlg, si32ResourceID );

	if ( hResource == NULL )
	{
		return;
	}

	int si32ExistIndex = 0;
	if ( IsExistFont( si32Height, strFontKind, si32ExistIndex ) )
	{
		SendMessage( hResource, WM_SETFONT, (WPARAM)m_hFont[ si32ExistIndex ], TRUE );		
	}
	else
	{
		cltClient *pclclient = (cltClient *)pclClient;
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			m_hFont[ m_si32FontNum ] = CreateFont( si32Height, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, strFontKind );
		}
		else {
			m_hFont[ m_si32FontNum ] = CreateFont( si32Height, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, strFontKind );
		}
		m_si32FontHeight[ m_si32FontNum ] = si32Height;
		StringCchCopy( m_strFontKind[ m_si32FontNum ], 32, strFontKind );

		SendMessage( hResource, WM_SETFONT, (WPARAM)m_hFont[ m_si32FontNum ], TRUE );
		++m_si32FontNum;
	}
	
	return;
}

HFONT CFontManager::SetFont( SI32 si32Height, TCHAR *strFontKind )
{
	if ( m_si32FontNum > MAX_FONT_NUM )
	{
		return NULL;
	}
	
	int si32ExistIndex = 0;

	if ( IsExistFont( si32Height, strFontKind, si32ExistIndex ) )
	{
		return m_hFont[ si32ExistIndex ];
	}
	else
	{
		cltClient *pclclient = (cltClient *)pclClient;
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			m_hFont[ m_si32FontNum ] = CreateFont( si32Height, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, strFontKind );
		}
		else {
			m_hFont[ m_si32FontNum ] = CreateFont( si32Height, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, strFontKind );
		}	
		m_si32FontHeight[ m_si32FontNum ] = si32Height;
		StringCchCopy( m_strFontKind[ m_si32FontNum ], 32, strFontKind );

		return m_hFont[ m_si32FontNum++ ];
	}	
}

BOOL CFontManager::InstallFont(TCHAR* pszFileName, TCHAR* pszValueName)
{
	if(pszFileName == NULL)			return FALSE;
	if(pszValueName == NULL)		return FALSE;

	//===================================================================
	// 위치 정보 생성
	//===================================================================
	TCHAR szFilePath[MAX_PATH] = TEXT("");
	::GetCurrentDirectory(MAX_PATH, szFilePath);
	_tcscat(szFilePath, TEXT("\\"));
	_tcscat(szFilePath, pszFileName);

	TCHAR szFontPath[MAX_PATH] = TEXT("");
	::GetWindowsDirectory(szFontPath, MAX_PATH);
	_tcscat(szFontPath, TEXT("\\Fonts\\"));
	_tcscat(szFontPath, pszFileName);

	//===================================================================
	// 파일을 복사한다. 
	//===================================================================
	if(_taccess(szFontPath, 00) != 0)		// Existence Only Check
	{
		if(::CopyFile(szFilePath, szFontPath, FALSE) == FALSE)
		{
			return FALSE;
		}
	}

	//===================================================================
	//폰트를 등록
	//===================================================================
	if(::AddFontResource(szFontPath) == FALSE)		return FALSE;
	else											SendMessage(pclClient->GetHwnd(), WM_FONTCHANGE, 0, 0 );

	DWORD dwError = GetLastError();

	//===================================================================
	// 레지스트리 위치값 구하기
	//===================================================================
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	TCHAR szKey[MAX_PATH];
	if(GetVersionEx(&vi) == FALSE)					return FALSE;

	if( vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		_tcscpy(szKey, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"));
	}
	else
	{
		_tcscpy(szKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}

	//===================================================================
	// 레지스트리에 추가
	//===================================================================
	HKEY	hKey;
	DWORD	dwDisposition;
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
		return FALSE;
	if(RegSetValueEx(HKEY_LOCAL_MACHINE, pszValueName, 0, REG_SZ, (LPBYTE)pszFileName, _tcslen(pszValueName) + 1) != ERROR_SUCCESS)
		return FALSE;

	RegCloseKey(hKey);

	return TRUE;
}

BOOL CFontManager::InstallCustomFont(HWND hWnd)
{
	SI32 i = 0;

	for(i = 0; i < MAX_INSTALL_FONT; ++i)	m_bInstallFont[i] = FALSE;

	//===================================================================
	// 폰트 목록 구하기
	//===================================================================
	HDC hdc;
	hdc = GetDC(hWnd);
	EnumFontFamilies(hdc, NULL, (FONTENUMPROC)EnumFamCallBack, (LPARAM)NULL);
	ReleaseDC(hWnd, hdc);

	//===================================================================
	// 폰트 인스톨
	//===================================================================
	for(i = 0; i < MAX_INSTALL_FONT; ++i)
	{
		if(m_bInstallFont[i] == FALSE)
		{
			if(m_kFontFileName[i].IsEmpty() == true)	continue;
			if(m_kFontName[i].IsEmpty() == true)		continue;
			if(m_kValueName[i].IsEmpty() == true)		continue;

			InstallFont(m_kFontFileName[i], m_kValueName[i]);
		}
	}

	return TRUE;
}

BOOL CFontManager::UnInstallFont(TCHAR* pszFileName, TCHAR* pszValueName)
{
	if(pszFileName == NULL)			return FALSE;
	if(pszValueName == NULL)		return FALSE;

	//===================================================================
	// 위치 정보 생성
	//===================================================================
	TCHAR szFilePath[MAX_PATH] = TEXT("");
	::GetCurrentDirectory(MAX_PATH, szFilePath);
	_tcscat(szFilePath, TEXT("\\"));
	_tcscat(szFilePath, pszFileName);

	TCHAR szFontPath[MAX_PATH] = TEXT("");
	::GetWindowsDirectory(szFontPath, MAX_PATH);
	_tcscat(szFontPath, TEXT("\\Fonts\\"));
	_tcscat(szFontPath, pszFileName);

	//===================================================================
	//폰트 제거
	//===================================================================
	if(::RemoveFontResource(szFontPath) == FALSE)		return FALSE;
	else												SendMessage(pclClient->GetHwnd(), WM_FONTCHANGE, 0, 0 );

	//===================================================================
	// 레지스트리 위치값 구하기
	//===================================================================
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	TCHAR szKey[MAX_PATH];
	if(GetVersionEx(&vi) == FALSE)					return FALSE;

	if( vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		_tcscpy(szKey, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"));
	}
	else
	{
		_tcscpy(szKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}

	//===================================================================
	// 레지스트리에서 제거
	//===================================================================
	HKEY	hKey;
	DWORD	dwDisposition;

	//if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, REG_OPTION_NON_VOLATILE, &hKey) != ERROR_SUCCESS)
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
		return FALSE;

	if(RegDeleteValue(hKey, pszValueName) != ERROR_SUCCESS)
		return FALSE;

	RegCloseKey(hKey);

	//===================================================================
	// 파일을 제거한다. 
	//===================================================================
	if(_taccess(szFontPath, 02) == 0)		// Existence Only Check
	{
		if(::DeleteFile(szFontPath) == FALSE)
		{
			return FALSE;
		}
	}
	DWORD dwError = GetLastError();

	return TRUE;
}

BOOL CFontManager::UnInstallCustomFont(HWND hWnd)
{
	SI32 i = 0;

	//===================================================================
	// 폰트 언인스톨
	//===================================================================
	for(i = 0; i < MAX_INSTALL_FONT; ++i)
	{
		if(m_kFontFileName[i].IsEmpty() == true)	continue;
		if(m_kFontName[i].IsEmpty() == true)		continue;
		if(m_kValueName[i].IsEmpty() == true)		continue;

		UnInstallFont(m_kFontFileName[i], m_kValueName[i]);
	}

	return TRUE;
}

SI32 CALLBACK CFontManager::EnumFamCallBack(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, SI32 siFontType, LPARAM lParam)
{
	//===================================================================
	// 폰트가 설치 되어있는지 검사.
	//===================================================================
	for(SI32 i = 0; i < MAX_INSTALL_FONT; ++i)
	{
		if(m_kFontFileName[i].IsEmpty() == true)	continue;
		if(m_kFontName[i].IsEmpty() == true)		continue;
		if(m_kValueName[i].IsEmpty() == true)		continue;

		if(_tcsicmp(lpelf->elfLogFont.lfFaceName, m_kFontName[i]) == 0)
		{
			m_bInstallFont[i]	= TRUE;
		}
	}

	return TRUE;
}