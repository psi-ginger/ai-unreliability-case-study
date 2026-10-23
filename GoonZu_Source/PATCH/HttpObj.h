#ifndef _HTTPOBJ_H_
#define _HTTPOBJ_H_

#include "HttpBaseDefinition.h"

#include "../GoonzuWorld/common/SMLib/LibList/Obj.h"

class CHttpObj : public CObj
{
public:
	CHttpObj();
	~CHttpObj();

	void Initialize();
	void Destroy();
	
	void DisConnect();

	void Set( UI32 uiVersion, char *strHttpURL, char *strLocalFilePath );

	UC08 GetStatus()
	{
		return m_ucDownLoadStatus;
	}

	DWORD GetFileSize();													// 받을 파일의 사이즈를 얻어옮
	DWORD GetFileRecvSize();												// 받은 파일의 사이즈를 얻어옮

	DWORD HttpFileConnect( HINTERNET hInternet );			// HTTP 파일 URL로 연결 처리, 하드에 저장할 파일명도 설정

	void FileCreate();
	DWORD FileRecv();

	BOOL FileExist();										// 하드에 받을 파일과 같은 파일이 있는지 체크

	CC08 *GetLocalFileNamePath();
	UI32 GetVersion();

	void SetDownLoadComplete()	{	m_ucDownLoadStatus = DOWNLOAD_COMPLETE_STATUS;	}	// 다운로드 완료로 설정

private:
	HINTERNET		m_hURL;													// 한번에 여러개의 파일에 접속해서 받아 오려면 m_hURL 도 여러개야 되지...

	DWORD			m_dwFileSize;											// 다운로드할 파일의 크기
	DWORD			m_dwFileRecvSize;										// 얼마만큼의 데이터를 다운 받았는지 알수 있게 하기 위해서...

	CC08			m_strLocalFileNamePath[1536];							// 다운로드시에 하드에 넣을 파일 경로
	CC08			m_strHttpURL[ 1024 ];

	UC08			m_ucDownLoadStatus;										// 현재 상태를 가지고 있음. ( 아무 것도 하지 않고 있음, 파일을 다운 받는 중, 파일을 모두 받고 다시 아무것도 하지 않고 있는 상태. )
	
	UI32			m_uiVersion;

	HANDLE			m_hFile;
};

#endif