// GMTOOL.h : GMTOOL 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호

#include "../../NGameDataManager/NGameDataManager.h"
#include "./ItemManagerClient.h"
#include "./NewLog/NewLogFrame.h"

// CGMTOOLApp:
// 이 클래스의 구현에 대해서는 GMTOOL.cpp을 참조하십시오.
//

#if defined(KOREA)
	#define SERVICEAREA ConstServiceArea_Korea
#elif defined(ENGLISH)
	#define SERVICEAREA ConstServiceArea_English
#elif defined(JAPAN)
	#define SERVICEAREA ConstServiceArea_Japan
#elif defined(EUROPE)
	#define SERVICEAREA ConstServiceArea_EUROPE
#elif defined(NHNCHINA)
	#define SERVICEAREA ConstServiceArea_NHNChina
#elif defined(TAIWAN)
	#define SERVICEAREA ConstServiceArea_Taiwan
#else
	#define SERVICEAREA ConstServiceArea_Korea
#endif


class CGMTOOLApp : public CWinApp
{
public:
	CGMTOOLApp();


// 재정의
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	NGameDataManager	m_kDataManager;
	cltItemManagerClient m_clItemManagerClient;

	CNewLogFrame* CreatNewLog();
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMenuLog();
	afx_msg void OnMenuGuild();
	afx_msg void OnMenuId();

	afx_msg void OnMenuChar();
//	afx_msg void OnMenuItem();
	
//	afx_msg void OnMenuStatics();
//	afx_msg void OnMenuInfor();
//	afx_msg void OnMenuRecover();
	afx_msg void OnMenuNotice();
	afx_msg void OnMenuBlockchar();
//	afx_msg void OnMenuItem();
	afx_msg void OnMenuItem();
	afx_msg void OnMenuNewLog();
	afx_msg void OnMenuGmclient();
	afx_msg void OnMenuTradeLog();
	afx_msg void OnMenuSelledLog();
};

extern CGMTOOLApp theApp;
