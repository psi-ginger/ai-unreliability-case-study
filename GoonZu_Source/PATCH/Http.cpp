#include "stdafx.h"
#include "Patch.h"
#include "Http.h"
#include "TextMgr.h"

unsigned __stdcall CHttp::FileRecvThread(void* lpArg)
{
	CHttp *pHttp = (CHttp *) lpArg;

	pHttp->FileRecv();

	_endthreadex( 0 );

	return 0;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - Connect( char * strURL )
//	Desc : 생 성 자
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
CHttp::CHttp()
{
	m_hInternet = NULL;
	m_hURL = NULL;

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - Connect( char * strURL )
//	Desc : 소 멸 자
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
CHttp::~CHttp()
{
	Destroy();	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - Connect( char * strURL )
//	Desc : http 관련 초기화
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CHttp::Initialize()
{	
	if ( m_hURL )
	{
		::InternetCloseHandle( m_hURL );
		m_hURL = NULL;
	}

	if ( m_hInternet )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}

	m_bThreadRun = false;

	m_dwFileSize = 0;
	ZeroMemory( m_strLocalFileNamePath, 1536 );

	m_ucDownLoadStatus = STOP_STATUS;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - Connect( char * strURL )
//	Desc : http 관련 종료 처리
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CHttp::Destroy()
{
	HttpDownLoadThreadEnd();
	DisConnect();

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - DisConnect()
//	Desc : 접속 끊기
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CHttp::DisConnect()
{
	if ( m_hURL )
	{
		::InternetCloseHandle( m_hURL );
		m_hURL = NULL;
	}

	if ( m_hInternet != NULL )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - Connect()
//	Desc : http 초 기 화
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
bool CHttp::Connect()
{
	m_hInternet = ::InternetOpen( "HTTPDOWNLOAD", INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, 0 );
	char * pText = GetTxtFromMgr(13);
	
	//m_hInternet = ::InternetOpen( "HTTPDOWNLOAD", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

	if ( m_hInternet == NULL )
	{
		AfxMessageBox(pText);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - HttpFileConnect( char* strURL, char* strLocalFileName )
//	Desc : http file 받기 위한 http 접속 ( HTTP 파일 URL로 연결 처리 )
//		   이 함수는 외부에서 이 클래스를 객체로 생성한 곳에서 호출하도록 함.
//		   하드에 저장할 파일명도 설정, http로 받아올 파일의 정보도 얻어오게 함.
//	Date : 2003. 06. 10
//	Update : 2003. 06. 13 - 하드에 저장할 파일명 설정, http로 받아올 파일의 정보도 얻어오게 함.
//--------------------------------------------------------------------------------------------
bool CHttp::HttpFileConnect( char* strURL, char* strLocalFileName )
{
	m_hURL = ::InternetOpenUrl( m_hInternet, strURL, NULL, 0, INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE| INTERNET_FLAG_KEEP_CONNECTION, 0 );
	
	char *pText = GetTxtFromMgr(14);
	if ( m_hURL == NULL )
	{
		AfxMessageBox(pText);
		return false;
	}

	char bufQuery[32] ;
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	
	if ( ::HttpQueryInfo( m_hURL, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL ) == FALSE )
	{
		pText = GetTxtFromMgr(19);
		AfxMessageBox(pText);
		return false;
	}

	m_dwFileSize = (DWORD)atoi(bufQuery) ;

	strcpy( m_strLocalFileNamePath, strLocalFileName );
	
	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - HttpDownLoadThreadStart()
//	Desc : HTTP 파일을 받기 위한 쓰레드를 구동
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CHttp::HttpDownLoadThreadStart()
{
	m_ucDownLoadStatus = DOWNLOAD_STATUS;
	UI32 uiFileRecvThreadID = 0;
	m_bThreadRun = true;
	
	m_hFileRecvThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &FileRecvThread, this, 0, &uiFileRecvThreadID );

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CHttp::HttpDownLoadThreadEnd()
//	Desc : HTTP 파일을 다 받았으면, 쓰레드를 안전하게 종료 시키기 위해서 호출 하는 것임. - 일반적인 쓰레드 종료 방법
//	Date : 2003. 06. 10
//	Update : 
//--------------------------------------------------------------------------------------------
void CHttp::HttpDownLoadThreadEnd()
{
	if ( m_bThreadRun )
	{
		m_bThreadRun = false;
		::WaitForSingleObject( m_hFileRecvThreadHandle, INFINITE );
	}

	::CloseHandle( m_hFileRecvThreadHandle );
	m_hFileRecvThreadHandle = NULL;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CHttp::FileRecvStop()
//	Desc : 파일 받는 중일 때, 강제 종료 시킴. - 강제적인 쓰레드 종료
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CHttp::FileRecvStop()
{
	m_ucDownLoadStatus = DOWNLOAD_STOP;

	if ( m_bThreadRun )
	{
		m_bThreadRun = false;
		::WaitForSingleObject( m_hFileRecvThreadHandle, INFINITE );
	}

	::CloseHandle( m_hFileRecvThreadHandle );
	m_hFileRecvThreadHandle = NULL;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - UC08 CHttp::GetDownLoadStatus()
//	Desc : 현재 다운로드 상태를 알아오기 위해서...
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
UC08 CHttp::GetDownLoadStatus()
{
	return m_ucDownLoadStatus;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - FileRecv()
//	Desc : 쓰레드에서 이 함수를 호출함. 파일을 받는 부분은 여기서 구현됨.
//	Date : 2003. 06. 10
//	Update : 2003. 06. 13 - 실제 파일 받는 부분 코딩.
//--------------------------------------------------------------------------------------------
void CHttp::FileRecv()
{
	HANDLE hFile;
	DWORD dwSize = 0;
	DWORD dwRead = 0;
	DWORD dwWritten = 0;
	m_dwFileRecvSize = 0;

	CC08 Buf[1000000];	//설마 MB 단위로 Recv 되지는 않겠지... 인터넷 속도가 그만큼 되면... 쩝... ㅡㅡ

	BOOL Result;

	hFile = ::CreateFile( m_strLocalFileNamePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( hFile == INVALID_HANDLE_VALUE  )
	{
		m_ucDownLoadStatus = LOCALFILE_MAKE_ERROR;
		return;
	}

	while( m_bThreadRun )						// m_bThreadRun 은 FileRecv 를 호출하는 쓰레드 공통으로 쓰이는 변수 이므로, 이 변수 값이 도중에 false로 변하면 모든 쓰레드가 중지됨.
	{
		::InternetQueryDataAvailable( m_hURL, &dwSize, 0, 0 );

		if ( dwSize > 1000000 )
		{
			char *pText = GetTxtFromMgr(20);
			AfxMessageBox(pText);
			
			::CloseHandle( hFile );
			hFile = NULL;

			::DeleteFile( m_strLocalFileNamePath );			//받던 파일을 지워줌.
			
			::InternetCloseHandle( m_hURL );
			m_hURL = NULL;

			m_ucDownLoadStatus = DOWNLOAD_SPEED_ERROR;
			m_bThreadRun = false;
			break;
		}

		Result = ::InternetReadFile( m_hURL, Buf, dwSize, &dwRead );

		if ( dwRead == 0 && Result == TRUE )
		{
			::CloseHandle( hFile );
			hFile = NULL;
			::InternetCloseHandle( m_hURL );
			m_hURL = NULL;

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
	
	if ( m_hURL )
	{
		::InternetCloseHandle( m_hURL );
		m_hURL = NULL;
	}	
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - __int64 CHttp::GetFileSize()
//	Desc : 받을 파일의 사이즈를 얻어옮
//	Date : 2003. 08. 22
//	Update : 
//--------------------------------------------------------------------------------------------
__int64 CHttp::GetFileSize()
{
	return m_dwFileSize;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - __int64	CHttp::GetFileRecvSize()
//	Desc : 받은 파일의 사이즈를 얻어옮
//	Date : 2003. 08. 22
//	Update : 
//--------------------------------------------------------------------------------------------
__int64	CHttp::GetFileRecvSize()
{
	return m_dwFileRecvSize;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - float CHttp::GetFileRecvPercent()
//	Desc : 몇 퍼센트 받았는지 알아내기 위해서...
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
DWORD CHttp::GetFileRecvPercent()
{
	DWORD dwDownLoadPercentage = (DWORD)(( m_dwFileRecvSize * 100 ) / m_dwFileSize);

	if ( dwDownLoadPercentage > 100 )
	{
		return 100;
	}

	return dwDownLoadPercentage;
}