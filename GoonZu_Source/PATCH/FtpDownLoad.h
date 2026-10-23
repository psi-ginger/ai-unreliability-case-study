#ifndef _FTPDOWNLOAD_H_
#define _FTPDOWNLOAD_H_

#include <wininet.h>
#include <process.h>
#include "TypeDefinition.h"

enum { STOP_STATUS, DOWNLOAD_STATUS, DOWNLOAD_COMPLETE_STATUS, DOWNLOAD_SPEED_ERROR, DOWNLOAD_STOP, LOCALFILE_MAKE_ERROR };

class CFtpDownLoad  
{
public:
	CFtpDownLoad();															// 생 성 자
	~CFtpDownLoad();														// 소 멸 자

	
	inline static unsigned __stdcall FileRecvThread(void* lpArg);			// FileRecvThread

	void Initialize();														// 초 기 화
	void Destroy();															// 종료 처리

	bool Connect( char *strServerIP, char *strFolderPath, char *strID, char *strPassWord, US16 usFtpPort );					// FTP 연결 처리
	void DisConnect();														// HTTP 종료 처리

	bool FtpFileConnect( char* strURL, char* strLocalFileName );			// HTTP 파일 URL로 연결 처리, 하드에 저장할 파일명도 설정

	void FtpDownLoadThreadStart();											// HTTP 파일을 받기 위한 쓰레드를 구동
	void FtpDownLoadThreadEnd();											// HTTP 파일을 받기 위한 쓰레드를 중지

	void FileRecv();														// 쓰레드에서 파일 받기 위해서 이 함수를 호출함.
	void FileRecvStop();													// 파일 받기 중단.

	
	__int64 GetFileSize();													// 받을 파일의 사이즈를 얻어옮
	__int64	GetFileRecvSize();												// 받은 파일의 사이즈를 얻어옮

	DWORD GetFileRecvPercent();												// 몇 퍼센트 받았는지 알아내기 위해서...
	UC08 GetDownLoadStatus();												// 현재 다운로드 상태를 알아옮

private:
	HINTERNET		m_hInternet;	
	HINTERNET		m_hFtp;
	HINTERNET		m_FindFile;
	HINTERNET		m_hFtpFile;
	
	
	HINTERNET		m_hReq;

	bool			m_bThreadRun;											// 파일 받다가 중간에 멈추는 경우가 발생할 수 있으므로 이때 flag 를 둬서 멈추게 하기 위해서..
	HANDLE			m_hFileRecvThreadHandle;								// 쓰레드 핸들
	UC08			m_ucDownLoadStatus;										// 현재 상태를 가지고 있음. ( 아무 것도 하지 않고 있음, 파일을 다운 받는 중, 파일을 모두 받고 다시 아무것도 하지 않고 있는 상태. )

	CC08			m_strLocalFileNamePath[512];							// 다운로드시에 하드에 넣을 파일 경로

	__int64			m_dwFileSize;											// 다운로드할 파일의 크기
	__int64			m_dwFileRecvSize;										// 얼마만큼의 데이터를 다운 받았는지 알수 있게 하기 위해서...

	CC08			m_strServerIP[128];
};

#endif