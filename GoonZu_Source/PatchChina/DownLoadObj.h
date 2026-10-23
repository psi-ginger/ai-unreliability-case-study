#ifndef _DOWNLOADOBJ_H_
#define _DOWNLOADOBJ_H_

#include "TypeDefinition.h"

//class CHttp;
class CFtpDownLoad;

class CDownLoadObj  
{
public:
	CDownLoadObj();													// 생 성 자
	virtual ~CDownLoadObj();										// 소 멸 자

	void Initialize();												// 초 기 화
	void Destroy();													// 종료처리

	void SetBaseInfo( char *strUrl, char *strFolderPath, char *strUserID, char *strPassWord, US16 ftpPort );

	UI32 GetDownLoadVersion();										// DownLoad Version 값 얻어옮
	void SetDownLoadVersion();										// DownLoad Version 값 설정.

	CC08 *GetstrDownLoadVersion();									// DownLoad Version 스트링 포인터 얻어옮.
	void SetstrDownLoadVersion( CC08 * strDownLoadVersion, SI32 siAddPathLength );		// DownLoad Version 스트링 값 설정

	//CC08 *GetstrDownLoadPath();										// DownLoad 경로 얻어옮.
	void SetstrDownLoadPath( CC08 * strDownLoadPath );				// DownLoad 경로 설정. ( http://www.xxx.com/xxx.gif )
	
	CC08 *GetstrLocalFilePath();									// 로컬 파일의 경로를 얻어옮.
	void SetstrLocalFilePath( CC08 * strLocalFilePath );			// 로컬 파일의 경로를 설정함. ( c:\\Program Files\XXXCompany\GameName\\1.zip )
	void AddstrLocalPath();											// 기본 경로("c:\program files\xxx\game\") 에 더해서 추가해야 하는 것. (xxx.exe)

	void SetFolderPath( char *strFolderPath );						// FTP 폴더 경로

	void DownLoadStart();											// 이 객체가 담당한 패치 파일을 다운로드 시작
	
	bool CreateFtpObject();											// Ftp 객체 생성 및 http connect
	//CHttp *GetHttp();												// DownLoad를 담당하는 객체의 포인터를 얻어옮
	CFtpDownLoad *GetFtp();													// DownLoad를 담당하는 객체의 포인터를 얻어옮

private:
	UI32 m_uiDownLoadVersion;										// 다운로드 할려면 파일의 버전
	CC08 m_strDownLoadVersion[32];									// 다운로드 할려는 파일의 버전(스트링)
	CC08 m_strDownLoadPath[128];									// 다운로드 전체 경로
	CC08 m_strLocalFilePath[512];									// 다운로드한 파일을 저장할 로컬 파일의 전체 경로
	
	CC08 m_strFolderPath[128];										// FTP 폴더 경로 땜시
	

	CC08 m_strID[ 32 ];
	CC08 m_strPassWord[ 32 ];

	US16 m_usFtpPort;
	
	//CHttp *m_pHttp;												// 다운로드를 실제로 실행할 CHttp 객체
	CFtpDownLoad	*m_pFtp;										// 다운로드를 실제로 실행할 CFtp 객체
};

#endif