#include "stdafx.h"
#include "Patch.h"
#include "DownLoadObj.h"
#include "FtpDownLoad.h"


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CDownLoadObj::CDownLoadObj()
//	Desc : FTP 파일을 받기 위한 쓰레드를 구동
//	Date : 2003. 06. 13
//	Update : 
//--------------------------------------------------------------------------------------------
CDownLoadObj::CDownLoadObj()
{
	m_pFtp = NULL;
	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CDownLoadObj::~CDownLoadObj()
//	Desc : HTTP 파일을 받기 위한 쓰레드를 구동
//	Date : 2003. 06. 13
//	Update : 
//--------------------------------------------------------------------------------------------
CDownLoadObj::~CDownLoadObj()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::Initialize()
//	Desc : 초 기 화
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::Initialize()
{
	m_uiDownLoadVersion = 0;
	ZeroMemory( m_strDownLoadPath, 128 );
	ZeroMemory( m_strDownLoadVersion, 32 );
	ZeroMemory( m_strFolderPath, 128 );
	ZeroMemory( m_strID, 32 );
	ZeroMemory( m_strPassWord, 32 );

	if ( m_pFtp )
	{
		delete m_pFtp;
		m_pFtp = NULL;
	}
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::Destroy()
//	Desc : 종료처리
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::Destroy()
{
	m_uiDownLoadVersion = 0;
	ZeroMemory( m_strDownLoadPath, 128 );
	ZeroMemory( m_strDownLoadVersion, 32 );
	ZeroMemory( m_strFolderPath, 128 );

	if ( m_pFtp )
	{
		delete m_pFtp;
		m_pFtp = NULL;
	}

	return;
}

void CDownLoadObj::SetBaseInfo( char *strUrl, char *strFolderPath, char *strUserID, char *strPassWord, US16 ftpPort )
{
	strcpy( m_strDownLoadPath, strUrl );

	if ( strFolderPath == NULL )
	{
		m_strFolderPath[0] = NULL;
	}
	else
	{
		strcpy( m_strFolderPath, strFolderPath );
	}

	strcpy( m_strID, strUserID );

	if ( strPassWord == NULL )
	{
		m_strPassWord[0] = NULL;
	}
	else
	{
		strcpy( m_strPassWord, strPassWord );
	}
	
	m_usFtpPort = ftpPort;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - UI32 CDownLoadObj::GetDownLoadVersion()
//	Desc : DownLoad Version 값 얻어옮
//	Date : 2003. 06. 13
//	Update : 
//--------------------------------------------------------------------------------------------
UI32 CDownLoadObj::GetDownLoadVersion()
{
	return m_uiDownLoadVersion;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::SetDownLoadVersion()
//	Desc : DownLoad Version 값 설정.
//	Date : 2003. 06. 13
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::SetDownLoadVersion()
{
	m_uiDownLoadVersion = atoi( m_strDownLoadVersion );

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::SetDownLoadVersion()
//	Desc : DownLoad Version 스트링 포인터 얻어옮.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
CC08 *CDownLoadObj::GetstrDownLoadVersion()
{
	return m_strDownLoadVersion;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::SetstrDownLoadVersion( CC08 * strDownLoadVersion, SI32 siAddPathLength )
//	Desc : DownLoad Version 스트링 값 설정
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::SetstrDownLoadVersion( CC08 * strDownLoadVersion, SI32 siAddPathLength )
{
	strncpy( m_strDownLoadVersion, strDownLoadVersion, siAddPathLength );
	strcat( m_strDownLoadVersion, "\0" );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CC08 *CDownLoadObj::GetstrDownLoadPath()
//	Desc : DownLoad 경로 얻어옮.
//	Date : 2003. 06. 13
//	Update : 
//--------------------------------------------------------------------------------------------
/*
CC08 *CDownLoadObj::GetstrDownLoadPath()
{
	return m_strDownLoadPath;
}
*/

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::SetstrDownLoadPath(  CC08 * strDownLoadPath )
//	Desc : DownLoad 경로 설정. ( http://www.xxx.com/xxx.gif )
//		   실제로 패치 프로그램에서 이 함수를 호출시에는 http://www.xxx.com/ 까지만 해 줌.
//		   뒤에 AddDownLoadPath 함수를 호출해서 완전한 DownLoad 경로를 설정함.
//	Date : 2003. 06. 13
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::SetstrDownLoadPath(  CC08 * strDownLoadPath )
{
	strcpy( m_strDownLoadPath, strDownLoadPath );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CC08 *CDownLoadObj::GetstrLocalFilePath()
//	Desc : 로컬 파일의 경로를 얻어옮.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
CC08 *CDownLoadObj::GetstrLocalFilePath()
{
	return m_strLocalFilePath;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::SetstrDownLoadPath(  CC08 * strDownLoadPath )
//	Desc : 로컬 파일의 경로를 설정함. ( c:\Program Files\XXXCompany\GameName\1.zip )
//		   실제로 패치 프로그램에서 이 함수를 호출시에는 c:\Program Files\XXXCompany\GameName\ 까지만 해 줌.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::SetstrLocalFilePath( CC08 * strLocalFilePath )
{
	strcpy( m_strLocalFilePath, strLocalFilePath );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::AddstrLocalPath()
//	Desc : 기본 경로("c:\program files\xxx\game\") 에 더해서 추가해야 하는 것. (xxx.exe)
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::AddstrLocalPath()
{
	strcat( m_strLocalFilePath, m_strDownLoadVersion );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::SetFolderPath( char *strFolderPath )
//	Desc : FTP 경로 설정
//	Date : 2003. 09. 05
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::SetFolderPath( char *strFolderPath )
{
	memset( m_strFolderPath, 0, 128 );
	strcpy( m_strFolderPath, strFolderPath );
	
	return;
}



//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CDownLoadObj::CreateFtpObject()
//	Desc : Ftp 객체 생성 및 Ftp connect
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
bool CDownLoadObj::CreateFtpObject()
{
	m_pFtp = new CFtpDownLoad();

	if ( ! m_pFtp->Connect( m_strDownLoadPath, m_strFolderPath, m_strID, m_strPassWord, m_usFtpPort ) )
	{
		return false;
	}

	if ( ! m_pFtp->FtpFileConnect( m_strDownLoadVersion, m_strLocalFilePath ) )
	{
		return false;
	}
	

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CDownLoadObj::DownLoadStart()
//	Desc : 이 객체가 담당한 패치 파일을 다운로드 시작.
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CDownLoadObj::DownLoadStart()
{
	m_pFtp->FtpDownLoadThreadStart();

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CFtpDownLaod *CDownLoadObj::GetFtp()
//	Desc : DownLoad를 담당하는 객체의 포인터를 얻어옮
//	Date : 2003. 06. 23
//	Update : 
//--------------------------------------------------------------------------------------------
CFtpDownLoad *CDownLoadObj::GetFtp()
{
	return m_pFtp;
}