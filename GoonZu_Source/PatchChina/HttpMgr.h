#ifndef _HTTPMGR_H_
#define _HTTPMGR_H_


#include "HttpBaseDefinition.h"

#include "../GoonzuWorld/common/SMLib/LibList/LibListMgr.h"

class CHttpMgr : public CLibListMgr
{
public:

	inline static unsigned __stdcall FileRecvThread(void* lpArg);			// FileRecvThread

	CHttpMgr( UI32 uiCount );
	~CHttpMgr();

	void Initialize();
	void Destroy();

	void CreateEmptyHttpObj( UI32 uiMaxObjNum );
	void SetBasePath( char *strBaseLocalPath, char *strBaseURL );

	bool Connect();						// HTTP 연결 처리
	void DisConnect();					// HTTP 종료 처리	

	DWORD GetFileTotalSize();			// 받을 파일 모두의 사이즈를 얻어옮
	DWORD GetFileRecvTotalSize();		// 받은 파일 모두의 사이즈를 얻어옮

	
	UC08 GetDownLoadStatus();			// 현재 다운로드 상태를 알아옮
	void DownLoadStart();

	DWORD GetFileRecvPercent();			// 몇 퍼센트 받았는지 알아내기 위해서...
	

	void Push( UI32 uiVersion, char *strVersion );

	void FileRecv();
	void FileRecvStop();

	CC08 *GetLocalFileNamePath( SI32 siIndex );
	UI32 GetLastVersion();

private:
	HINTERNET		m_hInternet;
	HANDLE			m_hFileRecvThreadHandle;

	bool			m_bThreadRun;											// 파일 받다가 중간에 멈추는 경우가 발생할 수 있으므로 이때 flag 를 둬서 멈추게 하기 위해서..
	UC08			m_ucDownLoadStatus;										// 현재 상태를 가지고 있음. ( 아무 것도 하지 않고 있음, 파일을 다운 받는 중, 파일을 모두 받고 다시 아무것도 하지 않고 있는 상태. )

	DWORD			m_dwFileTotalSize;
	DWORD			m_dwFileRecvTotalSize;

	char			*m_strBaseURL;
	char			*m_strBaseLocalPath;

};

#endif