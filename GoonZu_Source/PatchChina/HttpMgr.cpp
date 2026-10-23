#include "StdAfx.h"
#include "HttpMgr.h"
#include "HttpObj.h"
#include "TextMgr.h"
#include "../GoonzuWorld/common/SMLib/LibList/LibList.h"


unsigned __stdcall CHttpMgr::FileRecvThread(void* lpArg)
{
	CHttpMgr *pHttpMgr = (CHttpMgr *) lpArg;

	pHttpMgr->FileRecv();

	return 0;
}

CHttpMgr::CHttpMgr( UI32 uiCount ) : CLibListMgr()
{
	m_dwFileTotalSize = 0;
	m_dwFileRecvTotalSize = 0;

	m_strBaseURL = NULL;
	m_strBaseLocalPath = NULL;

	m_hFileRecvThreadHandle = NULL;

	m_ucDownLoadStatus = STOP_STATUS;

	CreateEmptyHttpObj( uiCount );

	m_hInternet = NULL;

	Initialize();
}

CHttpMgr::~CHttpMgr()
{
	Destroy();

	DisConnect();
}

void CHttpMgr::Initialize()
{	
	return;
}

void CHttpMgr::Destroy()
{
	if ( m_strBaseURL )
	{
		delete [] m_strBaseURL;
		m_strBaseURL = NULL;
	}

	if ( m_strBaseLocalPath )
	{
		delete [] m_strBaseLocalPath;
		m_strBaseLocalPath = NULL;
	}


	return;
}

void CHttpMgr::CreateEmptyHttpObj( UI32 uiMaxObjNum )
{
	for ( UI32 i = 0; i < uiMaxObjNum; ++i )
	{
		CHttpObj *pObj = new CHttpObj();
		PushBackToEmptyList( pObj );
	}

	return;
}

void CHttpMgr::SetBasePath( char *strBaseLocalPath, char *strBaseURL )
{
	SI32 siLocalPathStrLen = strlen ( strBaseLocalPath );	
	m_strBaseLocalPath = new char[ siLocalPathStrLen + 1 ];
	memset( m_strBaseLocalPath, 0, siLocalPathStrLen + 1 );
	strcpy( m_strBaseLocalPath, strBaseLocalPath );

	SI32 siURLStrLen = strlen( strBaseURL );	
	m_strBaseURL = new char[ siURLStrLen + 10 ];
	memset( m_strBaseURL, 0, siURLStrLen + 10 );	
	strcpy( m_strBaseURL, "http://" );
	strcat( m_strBaseURL, strBaseURL );

	siURLStrLen = strlen( m_strBaseURL );	

	if ( m_strBaseURL[ siURLStrLen -1 ] != '/' )
	{
		strcat( m_strBaseURL, "/" );
	}
	
	return;
}

// HTTP 연결 처리
bool CHttpMgr::Connect()
{	
	m_hInternet = ::InternetOpen( "HTTPDOWNLOAD", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	char *pText = GetTxtFromMgr(15);

	if ( m_hInternet == NULL )
	{
		AfxMessageBox(pText);
		return false;
	}	

	return true;
}

// HTTP 종료 처리
void CHttpMgr::DisConnect()
{
	if ( m_hInternet != NULL )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}

	return;
}

void CHttpMgr::Push( UI32 uiVersion, char *strVersion )
{
	CHttpObj *pObj = (CHttpObj *)PopFrontToEmptyListAndPushBackToExistList();

	char strHttpURL[ 1024 ] = "";
	strcpy( strHttpURL, m_strBaseURL );
	strcat( strHttpURL, strVersion );

	char strLocalPath[ 1024 ] = "";
	strcpy( strLocalPath, m_strBaseLocalPath );
	strcat( strLocalPath, strVersion );

	pObj->Set( uiVersion, strHttpURL, strLocalPath );

	return;
}

// 받을 파일의 사이즈를 얻어옮
DWORD CHttpMgr::GetFileTotalSize()
{
	m_dwFileTotalSize = 0;

	DWORD dwFileSize = 0;

	CHttpObj *pObj = ( CHttpObj*)GetExistList()->GetFirst();

	while ( pObj )
	{
		dwFileSize = pObj->HttpFileConnect( m_hInternet );

		if ( dwFileSize < 1 )
		{
			return 0;
		}

		m_dwFileTotalSize += dwFileSize;

		pObj->DisConnect();

		pObj = (CHttpObj *)pObj->GetNext();
	}

	return m_dwFileTotalSize;
}

// 받은 파일의 사이즈를 얻어옮
DWORD CHttpMgr::GetFileRecvTotalSize()
{
	return m_dwFileRecvTotalSize;
}

// 현재 다운로드 상태를 알아옮
UC08 CHttpMgr::GetDownLoadStatus()
{
	return m_ucDownLoadStatus;
}

void CHttpMgr::DownLoadStart()
{
	m_ucDownLoadStatus = DOWNLOAD_STATUS;
	UI32 uiFileRecvThreadID = 0;
	m_bThreadRun = true;
	
	m_hFileRecvThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &FileRecvThread, this, 0, &uiFileRecvThreadID );

	return;
}

// 몇 퍼센트 받았는지 알아내기 위해서...
DWORD CHttpMgr::GetFileRecvPercent()
{
	DWORD dwDownLoadPercentage = (DWORD)(( GetFileRecvTotalSize() * 100 ) / GetFileTotalSize());

	if ( dwDownLoadPercentage > 100 )
	{
		return 100;
	}

	return dwDownLoadPercentage;
}

void CHttpMgr::FileRecv()
{
	m_dwFileRecvTotalSize = 0;
	
	CHttpObj *pObj = ( CHttpObj*)GetExistList()->GetFirst();
	
	while( m_bThreadRun && pObj )
	{
		switch ( pObj->GetStatus() )
		{
		case STOP_STATUS:
			{
				pObj->HttpFileConnect( m_hInternet );
				pObj->FileCreate();
			}
			break;
		case DOWNLOAD_STATUS:
			{
				m_dwFileRecvTotalSize += pObj->FileRecv();
			}
			break;
		case DOWNLOAD_COMPLETE_STATUS:
			{
				pObj->DisConnect();
				pObj = (CHttpObj *)pObj->GetNext();
			}
			break;
		}

		Sleep(1);
	}

	if ( pObj == NULL )			// 다운로드 완전히 끝난 상태
	{
		m_ucDownLoadStatus = DOWNLOAD_COMPLETE_STATUS;
	}
	else
	{
		m_ucDownLoadStatus = DOWNLOAD_STOP;
	}

	pObj = ( CHttpObj*)GetExistList()->GetFirst();
	
	while( pObj )
	{
		pObj->DisConnect();
		pObj = (CHttpObj *)pObj->GetNext();
	}
	
	m_bThreadRun = false;
	
	_endthreadex( 0 );

	return;
}

void CHttpMgr::FileRecvStop()
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

CC08 *CHttpMgr::GetLocalFileNamePath( SI32 siIndex )
{
	CHttpObj *pObj = ( CHttpObj*)GetExistList()->GetFirst();

	for ( int i = 0; i < siIndex; ++i )
	{
		pObj = (CHttpObj *)pObj->GetNext();
	}

	return pObj->GetLocalFileNamePath();
}

UI32 CHttpMgr::GetLastVersion()
{
	CHttpObj *pObj = ( CHttpObj*)GetExistList()->GetLast();

	if ( pObj == NULL )
	{
		return 0;
	}

	return pObj->GetVersion();
}