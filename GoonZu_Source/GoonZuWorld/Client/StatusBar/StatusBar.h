//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include <windows.h>
#include <directives.h>

#include <Commctrl.h>		// Common Control 사용법	- Commctrl.h를 include 한다. 

#define STATUS_BAR_DIVIDE_NUMBER	5

class cltStatusBar{
public:

	HWND hParent;
	HWND hStatusBar;

	RECT Rect;

	cltStatusBar(HWND hparent);
	~cltStatusBar();

	SI32 GetYsize();
	void SetText(char* str, SI32 part);
	// 영역을 재설정한다. 
	void ReSize(WPARAM wParam, LPARAM lParam);


};

#endif