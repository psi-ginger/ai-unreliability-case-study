//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "StatusBar.h"



cltStatusBar::cltStatusBar(HWND hparent)
{
	/*
	hParent = hparent;

	hStatusBar = CreateStatusWindow(WS_CHILD|WS_VISIBLE, "Status Line", hParent, 0);

	SendMessage(hStatusBar, SB_GETRECT, 0, (LPARAM)&Rect); 
	*/

}


cltStatusBar::~cltStatusBar()
{
}


SI32 cltStatusBar::GetYsize()
{
	//return Rect.bottom;
	return 0;
}


void cltStatusBar::SetText(char* str, SI32 part)
{
	//SendMessage(hStatusBar, SB_SETTEXT, part, (LPARAM)str);
}


// 영역을 재설정한다. 
void cltStatusBar::ReSize(WPARAM wParam, LPARAM lParam)
{
	/*
	SendMessage(hStatusBar, WM_SIZE, wParam, lParam);
	
	int Part[STATUS_BAR_DIVIDE_NUMBER];
	for(int i = 0;i < STATUS_BAR_DIVIDE_NUMBER;i++)
	{
		Part[i] = LOWORD(lParam) / STATUS_BAR_DIVIDE_NUMBER * (i+1);
	}
	SendMessage(hStatusBar, SB_SETPARTS, STATUS_BAR_DIVIDE_NUMBER, (LPARAM)Part);
	*/

}