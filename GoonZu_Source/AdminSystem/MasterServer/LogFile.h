// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__B40F0C60_2692_49EE_98C8_1092AB399924__INCLUDED_)
#define AFX_LOGFILE_H__B40F0C60_2692_49EE_98C8_1092AB399924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <time.h>
#include <stdio.h>

class CLogFile
{
public:
	CLogFile();
	virtual ~CLogFile();

	void Init(); // 시작하기
	void Run(); // 실행하기
	void Clear(); // 데이타 초기화
	void ReadLogFile(); // 로그 파일 읽기 - 마스터 서버 켤 때
	BOOL ReadLogFileEX(int year,int month,int day); // 로그 파일 읽기 - 요청 패킷이 왔을 때
	void WriteLogFile(); // 로그 파일 쓰기 - 매 5분마다
	BOOL ParallelFile(); // 로그 파일 열었을 때 마지막 기록이 있나 없나와 현재 시간이랑 비교하기
	inline void CriTicalWait(){EnterCriticalSection(&m_cs);};
	inline void CriTicalRelease(){LeaveCriticalSection(&m_cs);};

	_SYSTEMTIME			m_sTime; // 시스템 시간
	CRITICAL_SECTION	m_cs; // 크리티컬 섹션

	int					m_iServerTotalUseNum[288]; // 전 서버 동시 접속자 시간대별 합친 수
	int					m_iServer[100][288]; // 서버별 동시 접속자
	int					m_iLastNum; // 파일에서 마지막기록의 순서번호
	int					m_iOldMin ; // 마지막으로 기록한 시간의 분(Minute)

	char				m_chDate[288][24]; // 몇년몇월몇일몇시몇분이 기록될 문자열

	BOOL				m_bWrite; // 파일에 써도 되는가 안되는가
};

extern CLogFile*	logFile;

#endif // !defined(AFX_LOGFILE_H__B40F0C60_2692_49EE_98C8_1092AB399924__INCLUDED_)
