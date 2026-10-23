//---------------------------------
// 2003/11/25 김태곤
//---------------------------------

#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "..\resource.h"

#include "DevProfileInfo.h"

extern cltCommonLogic* pclClient;

cltDevProfileInfo::cltDevProfileInfo() : cltDevTool()
{
	pkProfileIterator	= NProfileSystem::Get_Iterator();
	m_siMaxCalls		= 0;
	m_bReset			= FALSE;
}

cltDevProfileInfo::~cltDevProfileInfo()
{
	NProfileSystem::Release_Iterator(pkProfileIterator);
}

BOOL CALLBACK cltDevProfileInfo::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256] = TEXT("");

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			OnInitDialog(hDlg);
		}
		return TRUE;

	case WM_TIMER:
		{
			OnTimerDialog(hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(m_hDlg, &DlgRect);

		DlgRectInitSwitch = TRUE;

		m_hDlg = NULL;
		return TRUE;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_RESET:	{	m_bReset = TRUE;		}	return TRUE;
			case IDOK:				{	DestroyWindow(m_hDlg);	}	return TRUE;
			case IDCANCEL:			{	DestroyWindow(m_hDlg);	}	return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

BOOL cltDevProfileInfo::OnInitDialog(HWND hDlg)
{
	m_hDlg = hDlg;
	m_hProfileListCtrl = GetDlgItem( hDlg, IDC_LIST_PROFILE);

	LVCOLUMN Col;			

	Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	Col.fmt = LVCFMT_RIGHT;

	Col.cx = 220;	Col.pszText = "PROFILE(초당 갱신)";	Col.iSubItem = 0;
	SendMessage( m_hProfileListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

	Col.cx = 70;	Col.pszText = "Calls";				Col.iSubItem = 1;
	SendMessage( m_hProfileListCtrl, LVM_INSERTCOLUMN, 1, (LPARAM)&Col );

	Col.cx = 100;	Col.pszText = "Time";				Col.iSubItem = 2;
	SendMessage( m_hProfileListCtrl, LVM_INSERTCOLUMN, 2, (LPARAM)&Col );

	Col.cx = 100;	Col.pszText = "Time/Call";			Col.iSubItem = 3;
	SendMessage( m_hProfileListCtrl, LVM_INSERTCOLUMN, 3, (LPARAM)&Col );

	Col.cx = 100;	Col.pszText = "Time/Frame";			Col.iSubItem = 4;
	SendMessage( m_hProfileListCtrl, LVM_INSERTCOLUMN, 4, (LPARAM)&Col );

	// 타이머를 설정한다. 
	SetTimer(hDlg, 1, 1000, NULL);

	PROFILE_CALC_START();

	return TRUE;
}

BOOL cltDevProfileInfo::OnTimerDialog(HWND hDlg)
{
	ListView_DeleteAllItems( m_hProfileListCtrl );
	TCHAR szBuffer0[128];
	TCHAR szBuffer1[128];
	TCHAR szBuffer2[128];
	TCHAR szBuffer3[128];
	TCHAR szBuffer4[128];

	LVITEM Li;
	Li.mask			= LVIF_TEXT;
	Li.state		= 0;
	Li.stateMask	= 0;
	Li.iImage		= 0;
	Li.iSubItem 	= 0;

	SI32 siCount	= 0;
	SI32 siCalls = 0;
	SI32 siFrame = NProfileSystem::Get_Frame_Count_Since_Reset();

	if(m_bReset  == TRUE || m_siMaxCalls > 200000000)	{ m_bReset  = TRUE;	m_siMaxCalls = 0;	}

	pkProfileIterator->First();
	while(!pkProfileIterator->Is_Done())
	{
		StringCchPrintf(szBuffer0, sizeof(szBuffer0), "%s",			pkProfileIterator->Get_Current_Name()) ;
		StringCchPrintf(szBuffer1, sizeof(szBuffer1), "%d",			pkProfileIterator->Get_Current_Total_Calls()); 
		StringCchPrintf(szBuffer2, sizeof(szBuffer2), "%.5f",			pkProfileIterator->Get_Current_Total_Time()*1000);

		siCalls = pkProfileIterator->Get_Current_Total_Calls();

		if(siCalls!=0)		{	StringCchPrintf(szBuffer3, sizeof(szBuffer3), "%.5f", pkProfileIterator->Get_Current_Total_Time()*1000/pkProfileIterator->Get_Current_Total_Calls());	}
		else				{	StringCchPrintf(szBuffer3, sizeof(szBuffer3), "%.5f", 0);	}

		if(siFrame!=0)		{	StringCchPrintf(szBuffer4, sizeof(szBuffer4), "%.5f", pkProfileIterator->Get_Current_Total_Time()*1000/siFrame);	}
		else				{	StringCchPrintf(szBuffer4, sizeof(szBuffer4), "%.5f", 0);	}


		Li.iSubItem = 0;	Li.pszText = szBuffer0;	SendMessage( m_hProfileListCtrl, LVM_INSERTITEM, 0, (LPARAM)&Li );
		Li.iSubItem = 1;	Li.pszText = szBuffer1;	SendMessage( m_hProfileListCtrl, LVM_SETITEM,	 0, (LPARAM)&Li );
		Li.iSubItem = 2;	Li.pszText = szBuffer2;	SendMessage( m_hProfileListCtrl, LVM_SETITEM,	 0, (LPARAM)&Li );
		Li.iSubItem = 3;	Li.pszText = szBuffer3;	SendMessage( m_hProfileListCtrl, LVM_SETITEM,	 0, (LPARAM)&Li ); 
		Li.iSubItem = 4;	Li.pszText = szBuffer4;	SendMessage( m_hProfileListCtrl, LVM_SETITEM,	 0, (LPARAM)&Li ); 

		Li.iItem = siCount++;	

		m_siMaxCalls = max(m_siMaxCalls, siCalls);
		pkProfileIterator->Next();
	}

	if(m_bReset)			{	PROFILE_RESET();				}
	// Reset이 끝났으면 초기화한다.
	m_bReset = FALSE;

	return TRUE;
}
