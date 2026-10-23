// NdoorsGameStarter.cpp : CNdoorsGameStarter의 구현입니다.

#include "stdafx.h"
#include "NdoorsGameStarter.h"


HRESULT ReadStringRegKey( HKEY hKey , TCHAR* strRegName , TCHAR* strValue , DWORD dwLength , TCHAR* strDefault )
{
	DWORD dwType;

	if ( ERROR_SUCCESS != RegQueryValueEx( hKey , strRegName , 0 , &dwType , ( BYTE* )strValue , &dwLength ) )
	{
		_tcscpy_s( strValue, dwLength, strDefault );
	}

	return S_OK;
}


// CNdoorsGameStarter

STDMETHODIMP CNdoorsGameStarter::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_INdoorsGameStarter
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CNdoorsGameStarter::InitStarter(BSTR GameTitle, BSTR FileName)
{
	// TODO: 여기에 구현 코드를 추가합니다.

	m_runresult = 0;
	m_gameinstalled = 0;

	_tcscpy_s( m_szGameTitle, MAX_PATH, GameTitle );
	_tcscpy_s( m_szFileName, MAX_PATH, FileName );

	TCHAR szRegName[ MAX_PATH ];
	TCHAR szPath[ MAX_PATH ];

	// 레지스트리 이름 만들기
	_tcscpy_s( szRegName, MAX_PATH, TEXT("Software\\NDOORS\\") );
	_tcscat_s( szRegName, MAX_PATH, GameTitle );

	// 레지스트리에서 경로명 읽기

/*
	HKEY hKey = NULL;
	HKEY hKeyCU = NULL;
	
	if( RegOpenCurrentUser( KEY_READ, &hKeyCU ) == ERROR_SUCCESS )
	{
		if( RegOpenKeyEx( hKeyCU, szRegName, 0 , KEY_READ , &hKey ) == ERROR_SUCCESS )
		{

			ReadStringRegKey( hKey, TEXT("PATH"), szPath, MAX_PATH, TEXT("\0") );
			RegCloseKey( hKey );

			MessageBox( 0, szPath, 0, 0 );

			if( szPath[ 0 ] != TEXT('\0') )
			{
				_tcscpy_s( m_szDirectory, MAX_PATH, szPath );

				m_gameinstalled = 1;
			}
		}
		RegCloseKey( hKeyCU );
	}
*/
	
	HKEY hKey = NULL;
	if( RegOpenKeyEx( HKEY_CURRENT_USER , szRegName, 0 , KEY_READ , &hKey ) == ERROR_SUCCESS )
	{
		ReadStringRegKey( hKey, TEXT("PATH"), szPath, MAX_PATH, TEXT("\0") );
		RegCloseKey( hKey );

		if( szPath[ 0 ] == TEXT('\0') )
		{

			HKEY hKeyCU = NULL;
			if( RegOpenCurrentUser( KEY_READ, &hKeyCU ) == ERROR_SUCCESS )
			{
				if( RegOpenKeyEx( hKeyCU, szRegName, 0 , KEY_READ , &hKey ) == ERROR_SUCCESS )
				{
					ReadStringRegKey( hKey, TEXT("PATH"), szPath, MAX_PATH, TEXT("\0") );
					RegCloseKey( hKey );

					if( szPath[ 0 ] != TEXT('\0') )
					{
						_tcscpy_s( m_szDirectory, MAX_PATH, szPath );

						m_gameinstalled = 1;
					}
				}

				RegCloseKey( hKeyCU );
			}

		}
		else
		{
			_tcscpy_s( m_szDirectory, MAX_PATH, szPath );

			m_gameinstalled = 1;
		}
		
	}

	m_init = 1;

	return S_OK;
}

STDMETHODIMP CNdoorsGameStarter::Run(void)
{
	// TODO: 여기에 구현 코드를 추가합니다.

	m_runresult = 0;
	if( m_init == 0 ) return S_OK;

	if( m_gameinstalled == 0 ) 
	{
		m_runresult = -1;			// 게임이 설치되어있지 않음
		return S_OK;
	}

	/*
	SHELLEXECUTEINFO si;
	ZeroMemory(&si, sizeof(SHELLEXECUTEINFO));

	si.cbSize = sizeof(SHELLEXECUTEINFO);
	si.lpVerb = _TEXT("open");             
	si.lpFile = m_szFileName;			       
	si.lpDirectory = m_szDirectory;                 
	si.nShow = SW_SHOW;
	si.fMask = 0;

	if( ShellExecuteEx(&si) )
	{
		m_runresult = 1;
	}
	*/

	TCHAR szPath[ MAX_PATH ];
	_tcscpy_s( szPath, MAX_PATH, m_szDirectory );
	
	int len = _tcslen( szPath );
	if( len > 0 && szPath[ len - 1 ] != TEXT('\\') ) _tcscat_s( szPath, MAX_PATH, TEXT("\\") );

	_tcscat_s( szPath, MAX_PATH, m_szFileName );

	STARTUPINFO si = { sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;

	//if( CreateProcess(  NULL, szPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
	if( CreateProcess( szPath, NULL, NULL, NULL, FALSE, 0, NULL, m_szDirectory, &si, &pi ) )
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );

		m_runresult = 1;
	}
	else
	{	
		m_runresult = -2;
	}


	return S_OK;
}

STDMETHODIMP CNdoorsGameStarter::get_GameInstalled(LONG* pVal)
{
	// TODO: 여기에 구현 코드를 추가합니다.

	*pVal = m_gameinstalled;

	return S_OK;
}

STDMETHODIMP CNdoorsGameStarter::get_RunResult(LONG* pVal)
{
	// TODO: 여기에 구현 코드를 추가합니다.

	*pVal = m_runresult;

	return S_OK;
}
