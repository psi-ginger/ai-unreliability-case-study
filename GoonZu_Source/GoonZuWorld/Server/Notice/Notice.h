#ifndef _NOTICE_H
#define _NOTICE_H

#define MAX_NOTICE_TEXT_SIZE	1024
#define MAX_NOTICE_NUM			10


#include <Directives.h>
#include "../../../NLib/NArray.h"

//----------------------------
// 하나하나의 공지 사항. 
//----------------------------
class cltNoticeUnit{
public:
	TCHAR szNoticeText[MAX_NOTICE_TEXT_SIZE];
	DWORD dwStartFrame;	// 첫 출력 프레임. 
	DWORD dwInterval;	// 출력 간격. 


	cltNoticeUnit()
	{
		Init();
	}

	void Init()
	{
		StringCchCopy(szNoticeText, MAX_NOTICE_TEXT_SIZE, TEXT(""));
		dwStartFrame	= 0;
		dwInterval		= 0;

	}

	void Set(DWORD startframe, DWORD interval, TCHAR* ptext)
	{
		dwStartFrame	= startframe;
		dwInterval		= interval;
		StringCchCopy(szNoticeText, MAX_NOTICE_TEXT_SIZE, ptext);
	}
};

//------------------------------
// 공지 관리자.
//-----------------------------
class cltNoticeManager{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<cltNoticeUnit, MAX_NOTICE_NUM>		clNotice;
	NSafeTArray<SI32, MAX_NOTICE_NUM>		Counting;	// 현제 공지를 날린 횟수
	NSafeTArray<SI32, MAX_NOTICE_NUM>		Count;		// 목표 공지 
#else
	cltNoticeUnit clNotice[MAX_NOTICE_NUM];
	SI32 Counting[MAX_NOTICE_NUM];
	SI32 Count[MAX_NOTICE_NUM];
#endif

	//로그인 공지 
	bool bLoginNoticeSwitch;
	TCHAR szLogInNotice[2048];

	cltNoticeManager();
	~cltNoticeManager();

	// 공지를 추가한다. 
	void Add(DWORD interval, TCHAR* ptext , SI32 siCount);	// 반복횟수 추가

	// 공지를 보여준다. 
	void Action();

	// 모든 공지를 지운다. 
	void DelAllNotice();

	// 로그인 공지의 on/ff
	void OnOffLogInNotice();

};


#endif
