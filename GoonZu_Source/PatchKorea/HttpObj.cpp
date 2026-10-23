#include "StdAfx.h"
#include "HttpObj.h"
#include "TextMgr.h"


CHttpObj::CHttpObj() : CObj()
{
	m_hURL = NULL;
	m_hFile = NULL;

	Initialize();
}

CHttpObj::~CHttpObj()
{
	Destroy();

	DisConnect();
}

void CHttpObj::Initialize()
{
	if ( m_hURL )
	{
		::InternetCloseHandle( m_hURL );
		m_hURL = NULL;
	}

	m_dwFileSize = 0;
	m_dwFileRecvSize = 0;
	ZeroMemory( m_strLocalFileNamePath, 1536 );
	ZeroMemory( m_strHttpURL, 1024 );

	m_ucDownLoadStatus = STOP_STATUS;

	return;
}

void CHttpObj::Destroy()
{
	if ( m_hFile )
	{
		CloseHandle( m_hFile );
		m_hFile = NULL;
	}

	::DeleteFile( m_strLocalFileNamePath );

	return;
}

void CHttpObj::DisConnect()
{
	if ( m_hURL )
	{
		::InternetCloseHandle( m_hURL );
		m_hURL = NULL;
	}

	if ( m_hFile )
	{
		CloseHandle( m_hFile );
		m_hFile = NULL;
	}
}

void CHttpObj::Set( UI32 uiVersion, char *strHttpURL, char *strLocalFilePath )
{
	m_uiVersion = uiVersion;
	strcpy( m_strLocalFileNamePath, strLocalFilePath );
	strcpy( m_strHttpURL, strHttpURL );

	return;
}
	
DWORD CHttpObj::GetFileSize()
{
	return m_dwFileSize;
}

DWORD CHttpObj::GetFileRecvSize()
{
	return m_dwFileRecvSize;
}


DWORD CHttpObj::HttpFileConnect( HINTERNET hInternet )
{
	m_hURL = ::InternetOpenUrl( hInternet, m_strHttpURL, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE |  INTERNET_FLAG_KEEP_CONNECTION, 0 );

	char* pText = TEXT("");
	if ( m_hURL == NULL )
	{
		AfxMessageBox(pText);
		return 0;
	}

	char bufQuery[32] ;
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	
	if ( ::HttpQueryInfo( m_hURL, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL ) == FALSE )
	{
		pText = TEXT("3 After a while, Try to Reconnect, Please.\r");
		AfxMessageBox(pText);
		return 0;
	}

	m_dwFileSize = (DWORD)atoi(bufQuery) ;
	m_dwFileRecvSize = 0;
	
	return m_dwFileSize;
}

void CHttpObj::FileCreate()
{
	m_hFile = ::CreateFile( m_strLocalFileNamePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( m_hFile == INVALID_HANDLE_VALUE  )
	{
		m_ucDownLoadStatus = LOCALFILE_MAKE_ERROR;
		return;
	}

	m_ucDownLoadStatus = DOWNLOAD_STATUS;

	return;
}

BOOL CHttpObj::FileExist()
{
	HANDLE hFile = ::CreateFile( m_strLocalFileNamePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( hFile == INVALID_HANDLE_VALUE  )
	{
		return FALSE;
	}

	DWORD dwLocalFileSize = ::GetFileSize(hFile, NULL);
	if(GetFileSize() != dwLocalFileSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	return TRUE;
}

DWORD CHttpObj::FileRecv()
{
	DWORD dwSize = 0;
	DWORD dwRead = 0;
	DWORD dwWritten = 0;

	BOOL Result;
	CC08 Buf[ URL_BUFFER_SIZE ];

	::InternetQueryDataAvailable( m_hURL, &dwSize, 0, 0 );

	if ( dwSize > URL_BUFFER_SIZE )
	{
		dwSize = URL_BUFFER_SIZE;			// 한번에 500 KB 이상 받지 못함
	}

	Result = ::InternetReadFile( m_hURL, Buf, dwSize, &dwRead );

	if ( dwRead == 0 && Result == TRUE )
	{
		::CloseHandle( m_hFile );
		m_hFile = NULL;
		::InternetCloseHandle( m_hURL );
		m_hURL = NULL;

		m_ucDownLoadStatus = DOWNLOAD_COMPLETE_STATUS;
		return 0;
	}

	::WriteFile( m_hFile, Buf, dwRead, &dwWritten, NULL );

	m_dwFileRecvSize += dwWritten;

	return dwWritten;
}

CC08 *CHttpObj::GetLocalFileNamePath()
{
	return m_strLocalFileNamePath;
}

UI32 CHttpObj::GetVersion()
{
	return m_uiVersion;
}