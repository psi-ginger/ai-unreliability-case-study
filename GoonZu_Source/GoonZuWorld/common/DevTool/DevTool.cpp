//---------------------------------
// 2003/11/25 김태곤
//---------------------------------

#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "..\resource.h"

#include "DevTool.h"


cltDevTool::cltDevTool()
{
	m_hDlg	= NULL;	

	DlgRectInitSwitch	= false;

}

cltDevTool::~cltDevTool()
{
}

// 대화상자가 없으면 만들고 있으면 없앤다. 
bool cltDevTool::Create(HINSTANCE hinst, HWND hwnd, SI32 dlgitem, DLGPROC dlgproc, bool bShow /* = true */ )
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(dlgitem), hwnd, dlgproc);

		if(DlgRectInitSwitch == true)
		{
			MoveWindow(m_hDlg, DlgRect.left, DlgRect.top, DlgRect.right - DlgRect.left, DlgRect.bottom - DlgRect.top, FALSE);
		}

		if( bShow )
			ShowWindow(m_hDlg, SW_SHOW);
		else
			ShowWindow(m_hDlg, SW_HIDE);

		return true;
	}
	else
	{
		DestroyWindow(m_hDlg);
	}

	return false;
	
}

// WM_DESTROY에서의 조치.
void cltDevTool::ACTION_WM_DESTROY()
{
	// 윈도우의 위치를 기억한다. 
	GetWindowRect(m_hDlg, &DlgRect);
	
	DlgRectInitSwitch = true;

	m_hDlg = NULL;
}

