#ifndef _MAINFUNCTION_H
#define _MAINFUNCTION_H

#include <Directives.h>

//--------------------------------------------------------------------------
// Name: WindowProc()
// Desc: 윈도우 Procedure
//-------------------------------------------------------------------------- 
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------
// Name: GetVersionInfo()
// Desc: 버전 정보를 얻어온다.
//--------------------------------------------------------------------------
TCHAR* GetVersionInfo();

//--------------------------------------------------------------------------
// Name: Restore()
// Desc: 미니마이즈된 상태에서 다시 복구할때 사용되는 함수 
//--------------------------------------------------------------------------
BOOL Restore();

//KHY - 0701 - 원키 이벤트 (F1~F12)는 0.3초에 한번씩만 작동하도록 한다.
bool UserdoubleKeyEvent(SI32 KeyEvent);

// 개발용 메시지를 추가한다.
void AddDevMsg(TCHAR* pmsg);

//한게임 투표 모듈
#if defined (USE_HANPOLL)
	// 한게임 설문 조사 모듈
	#include "../HanPollForClient/HanPollForClient.h"
#endif

#endif
