// StatisticsTool.h : StatisticsTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호


// CStatisticsToolApp:
// 이 클래스의 구현에 대해서는 StatisticsTool.cpp을 참조하십시오.
//

class CStatisticsToolApp : public CWinApp
{
public:
	CStatisticsToolApp();


// 재정의
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CStatisticsToolApp theApp;
