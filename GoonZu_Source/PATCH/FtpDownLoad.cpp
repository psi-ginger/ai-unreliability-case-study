#include "stdafx.h"
#include "patch.h"
#include "FtpDownLoad.h"
#include "TextMgr.h"

unsigned __stdcall CFtpDownLoad::FileRecvThread(void* lpArg)
{
	CFtpDownLoad *pFtp = ( CFtpDownLoad *) lpArg;

	pFtp->FileRecv();

	_endthreadex( 0 );

	return 0;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CFtpDownLoad::CFtpDownLoad()
//	Desc : 생 성 자
//	Date : 2003. 08. 24
//	Update : 
//--------------------------------------------------------------------------------------------
CFtpDownLoad::CFtpDownLoad()
{
	m_hInternet = NULL;
	m_hFtp = NULL;
	m_hFtpFile = NULL;
	m_FindFile = NULL;
	m_hFileRecvThreadHandle  = NULL;

	m_dwFileSize = 0;
	m_dwFileRecvSize = 0;

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CFtpDownLoad::~CFtpDownLoad()
//	Desc : 소 멸 자
//	Date : 2003. 08. 24
//	Update : 
//--------------------------------------------------------------------------------------------
CFtpDownLoad::~CFtpDownLoad()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::Initialize()
//	Desc : Ftp 관련 초기화
//	Date : 2003. 08. 24
//	Update : 
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::Initialize()
{	
	if ( m_hFtpFile )
	{
		::InternetCloseHandle( m_hFtpFile );
		m_hFtpFile = NULL;
	}
	
	if ( m_FindFile )
	{
		::InternetCloseHandle( m_FindFile );
		m_FindFile = NULL;
	}

	if ( m_hFtp )
	{
		::InternetCloseHandle( m_hFtp );
		m_hFtp = NULL;
	}

	if ( m_hInternet )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}

	m_bThreadRun = false;

	m_dwFileSize = 0;
	ZeroMemory( m_strLocalFileNamePath, 512 );

	m_ucDownLoadStatus = STOP_STATUS;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::Destroy()
//	Desc : Ftp 관련 종료 처리
//	Date : 2003. 08. 24
//	Update : 
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::Destroy()
{
	FtpDownLoadThreadEnd();

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::DisConnect()
//	Desc : 접속 끊기
//	Date : 2003. 08. 24
//	Update : 
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::DisConnect()
{
	if ( m_hFtpFile )
	{
		::InternetCloseHandle( m_hFtpFile );
		m_hFtpFile = NULL;
	}
	
	if ( m_FindFile )
	{
		::InternetCloseHandle( m_FindFile );
		m_FindFile = NULL;
	}

	if ( m_hFtp )
	{
		::InternetCloseHandle( m_hFtp );
		m_hFtp = NULL;
	}

	if ( m_hInternet )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CFtpDownLoad::Connect( char *ServerIP, char *strFolderPath, char *strID, char *strPassWord, US16 usFtpPort )
//	Desc : Ftp 초 기 화
//	Date : 2003. 08. 24
//	Update : Connect 파라미터들 Front Server 에서 받아오게 변경 - 2003. 10. 15
//--------------------------------------------------------------------------------------------
bool CFtpDownLoad::Connect( char *ServerIP, char *strFolderPath, char *strID, char *strPassWord, US16 usFtpPort )
{
	memset( m_strServerIP, 0, 128 );
	strcpy( m_strServerIP, ServerIP );
	char * pText = GetTxtFromMgr(12);

	m_hInternet = ::InternetOpen( "GoonZuFtp"
		, INTERNET_OPEN_TYPE_PRECONFIG
		, NULL, NULL, 0);

	if ( m_hInternet == NULL )
	{
		AfxMessageBox(pText);
		return false;
	}


	if ( strPassWord != NULL )
	{
		m_hFtp = ::InternetConnect
			( m_hInternet, m_strServerIP, usFtpPort
			, strID, strPassWord
			, INTERNET_SERVICE_FTP
			, INTERNET_FLAG_PASSIVE
			, 0);
	}
	else
	{
		m_hFtp = ::InternetConnect
			( m_hInternet, m_strServerIP, usFtpPort
			, NULL, NULL
			, INTERNET_SERVICE_FTP
			, INTERNET_FLAG_PASSIVE
			, 0);
	}

	if ( m_hFtp == NULL )
	{
		char *pText = GetTxtFromMgr(16);
		AfxMessageBox(pText);
		return false;
	}

	if ( strFolderPath != NULL )
	{
		::FtpSetCurrentDirectory( m_hFtp, strFolderPath );
	}
	

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CFtpDownLoad::FtpFileConnect( char* strFileName, char* strLocalFileName )
//	Desc : Ftp File 받기 위한 Ftp 접속 ( FTP 파일 연결 처리 )
//		   이 함수는 외부에서 이 클래스를 객체로 생성한 곳에서 호출하도록 함.
//		   하드에 저장할 파일명도 설정, Ftp로 받아올 파일의 정보도 얻어오게 함.
//	Date : 2003. 08. 24
//	Update :
//--------------------------------------------------------------------------------------------
bool CFtpDownLoad::FtpFileConnect( char* strFileName, char* strLocalFileName )
{
	WIN32_FIND_DATA wfd;

	m_FindFile = ::FtpFindFirstFile( m_hFtp, strFileName, &wfd, 0 ,0);
	char*pText = GetTxtFromMgr(17);

	if( m_FindFile == NULL )
	{	
		AfxMessageBox(pText);
		return false;
	}
	
	m_hFtpFile = ::FtpOpenFile( m_hFtp, strFileName, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 0 );

	if( m_hFtpFile == NULL )
	{	
		char*pText = GetTxtFromMgr(18);
		AfxMessageBox(pText);
		return false;
	}

	m_dwFileSize = wfd.nFileSizeLow;

	strcpy( m_strLocalFileNamePath, strLocalFileName );
	
	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::FtpDownLoadThreadStart()
//	Desc : FTP 파일을 받기 위한 쓰레드를 구동
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::FtpDownLoadThreadStart()
{
	m_ucDownLoadStatus = DOWNLOAD_STATUS;
	UI32 uiFileRecvThreadID = 0;
	m_bThreadRun = true;
	
	m_hFileRecvThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &FileRecvThread, this, 0, &uiFileRecvThreadID );

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::FtpDownLoadThreadEnd()
//	Desc : FTP 파일을 다 받았으면, 쓰레드를 안전하게 종료 시키기 위해서 호출 하는 것임. - 일반적인 쓰레드 종료 방법
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::FtpDownLoadThreadEnd()
{
	if ( m_bThreadRun )
	{
		m_bThreadRun = false;
		::WaitForSingleObject( m_hFileRecvThreadHandle, INFINITE );
	}

	if ( m_hFileRecvThreadHandle )
	{
		::CloseHandle( m_hFileRecvThreadHandle );
		m_hFileRecvThreadHandle = NULL;
	}	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::FileRecvStop()
//	Desc : 파일 받는 중일 때, 강제 종료 시킴. - 강제적인 쓰레드 종료
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::FileRecvStop()
{
	m_ucDownLoadStatus = DOWNLOAD_STOP;

	if ( m_bThreadRun )
	{
		m_bThreadRun = false;

		if ( m_hFileRecvThreadHandle )
		{
			::WaitForSingleObject( m_hFileRecvThreadHandle, INFINITE );
			::CloseHandle( m_hFileRecvThreadHandle );
			m_hFileRecvThreadHandle = NULL;
		}
	}

	if ( m_hFileRecvThreadHandle )
	{
		::CloseHandle( m_hFileRecvThreadHandle );
		m_hFileRecvThreadHandle = NULL;
	}	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - UC08 CFtpDownLoad::GetDownLoadStatus()
//	Desc : 현재 다운로드 상태를 알아오기 위해서...
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
UC08 CFtpDownLoad::GetDownLoadStatus()
{
	return m_ucDownLoadStatus;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CFtpDownLoad::FileRecv()
//	Desc : 쓰레드에서 이 함수를 호출함. 파일을 받는 부분은 여기서 구현됨.
//	Date : 2003. 06. 10
//	Update : 2003. 06. 13 - 실제 파일 받는 부분 코딩.
//--------------------------------------------------------------------------------------------
void CFtpDownLoad::FileRecv()
{
	HANDLE hFile;
	DWORD dwRead = 0;
	DWORD dwWritten = 0;
	m_dwFileRecvSize = 0;

	CC08 Buf[ 1000 ];				// 한번에 최대 1KB 만큼씩 받음.

	BOOL Result;

	hFile = ::CreateFile( m_strLocalFileNamePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( hFile == INVALID_HANDLE_VALUE  )
	{
		m_ucDownLoadStatus = LOCALFILE_MAKE_ERROR;
		return;
	}

	while( m_bThreadRun )						// m_bThreadRun 은 FileRecv 를 호출하는 쓰레드 공통으로 쓰이는 변수 이므로, 이 변수 값이 도중에 false로 변하면 모든 쓰레드가 중지됨.
	{
		//::InternetQueryDataAvailable( m_hURL, &dwSize, 0, 0 );
		Result = ::InternetReadFile( m_hFtpFile, Buf, 1000, &dwRead );
		
		if ( dwRead == 0 && Result == TRUE )
		{
			::CloseHandle( hFile );
			hFile = NULL;
			::InternetCloseHandle( m_hFtpFile );
			m_hFtpFile = NULL;

			m_ucDownLoadStatus = DOWNLOAD_COMPLETE_STATUS;
			m_bThreadRun = false;

			break;
		}

		::WriteFile( hFile, Buf, dwRead, &dwWritten, NULL );

		m_dwFileRecvSize += dwWritten;

		Sleep(1);
	}
	
	if ( hFile )			//이 경우는 DownLoad 를 강제 중단 시킨 경우이므로, 받고 있던 파일을 닫은 후에 지워줌.
	{
		::CloseHandle( hFile );
		::DeleteFile( m_strLocalFileNamePath );
	}

	if ( m_hFtpFile )
	{
		::InternetCloseHandle( m_hFtpFile );
		m_hFtpFile = NULL;
	}
	
	if ( m_FindFile )
	{
		::InternetCloseHandle( m_FindFile );
		m_FindFile = NULL;
	}

	if ( m_hFtp )
	{
		::InternetCloseHandle( m_hFtp );
		m_hFtp = NULL;
	}	

	if ( m_hInternet )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}

}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - __int64 CFtpDownLoad::GetFileSize()
//	Desc : 받을 파일의 사이즈를 얻어옮
//	Date : 2003. 08. 22
//	Update : 
//--------------------------------------------------------------------------------------------
__int64 CFtpDownLoad::GetFileSize()
{
	return m_dwFileSize;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - __int64	CFtpDownLoad::GetFileRecvSize()
//	Desc : 받은 파일의 사이즈를 얻어옮
//	Date : 2003. 08. 22
//	Update : 
//--------------------------------------------------------------------------------------------
__int64	CFtpDownLoad::GetFileRecvSize()
{
	return m_dwFileRecvSize;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - DWORD CFtpDownLoad::GetFileRecvPercent()
//	Desc : 몇 퍼센트 받았는지 알아내기 위해서...
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
DWORD CFtpDownLoad::GetFileRecvPercent()
{
	DWORD dwDownLoadPercentage = (DWORD)(( m_dwFileRecvSize * 100 ) / m_dwFileSize);

	if ( dwDownLoadPercentage > 100 )
	{
		return 100;
	}

	return dwDownLoadPercentage;
}
