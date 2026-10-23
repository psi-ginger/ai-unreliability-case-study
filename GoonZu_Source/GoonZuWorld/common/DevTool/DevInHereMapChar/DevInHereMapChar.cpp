#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "Char/CharClient/Char-Client.h"
#include "../common/Char/CharManager/CharManager.h"
#include "..\resource.h"

#include "DevInHereMapChar.h"

extern cltCommonLogic* pclClient;

cltDevInHereMapChar::cltDevInHereMapChar(cltCharManager* pclcm) : cltDevTool()
{
	m_pclCM	= pclcm;
}

cltDevInHereMapChar::~cltDevInHereMapChar()
{

}


BOOL cltDevInHereMapChar::OnInitDialog(HWND hDlg)
{
	m_hDlg					= hDlg;
	m_hInHereMapListCtrl	= GetDlgItem( hDlg, IDC_LIST_INHEREMAPCHAR );

	LVCOLUMN Col;			

	Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	Col.fmt = LVCFMT_RIGHT;

	Col.cx = 150;	Col.pszText = "Name";	Col.iSubItem = 0;
	SendMessage( m_hInHereMapListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

	Col.cx = 50;	Col.pszText = "X";				Col.iSubItem = 1;
	SendMessage( m_hInHereMapListCtrl, LVM_INSERTCOLUMN, 1, (LPARAM)&Col );

	Col.cx = 50;	Col.pszText = "Y";				Col.iSubItem = 2;
	SendMessage( m_hInHereMapListCtrl, LVM_INSERTCOLUMN, 2, (LPARAM)&Col );

	SetTimer(hDlg, 1, 3000, NULL);

	return TRUE;

}

BOOL cltDevInHereMapChar::OnButtonRefresh(HWND hDlg)
{
	ListView_DeleteAllItems( m_hInHereMapListCtrl );
	TCHAR szBuffer0[128];
	TCHAR szBuffer1[128];
	TCHAR szBuffer2[128];

	LVITEM Li;
	Li.mask			= LVIF_TEXT;
	Li.state		= 0;
	Li.stateMask	= 0;
	Li.iImage		= 0;
	Li.iSubItem 	= 0;

	SI32 siCount	= 0;
	SI32 siIndex	= 0;
	SI32 siID		= 0;
	SI32 siMyID		= 1;
	SI32 siMapIndex = 0;

	if ( pclClient->pclCM->IsValidID( siMyID ) == false )
	{
		return FALSE;
	}

	siMapIndex = pclClient->pclCM->CR[siMyID]->GetMapIndex();
	if ( (0 >= siMapIndex) || (MAX_MAP_NUMBER <= siMapIndex) )
	{
		return FALSE;
	}

	while(siID = pclClient->pclCM->GetCharOrder(siIndex))
	{
		siIndex++;

		// 유효한 아이디만 뽑아온다
		if ( pclClient->pclCM->IsValidID( siID ) == false )
		{
			continue;
		}

		cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[siID];
		if ( NULL == pclchar )
		{
			continue;
		}

		if ( pclchar->GetMapIndex() != siMapIndex )
		{
			continue;
		}

		StringCchPrintf( szBuffer0, sizeof(szBuffer0), "%s", pclchar->GetName() ) ;
		StringCchPrintf( szBuffer1, sizeof(szBuffer1), "%d", pclchar->GetX() ) ;
		StringCchPrintf( szBuffer2, sizeof(szBuffer2), "%d", pclchar->GetY() ) ;
		Li.iSubItem = 0;	Li.pszText = szBuffer0;	SendMessage( m_hInHereMapListCtrl, LVM_INSERTITEM, 0, (LPARAM)&Li );
		Li.iSubItem = 1;	Li.pszText = szBuffer1;	SendMessage( m_hInHereMapListCtrl, LVM_SETITEM,	 0, (LPARAM)&Li );
		Li.iSubItem = 2;	Li.pszText = szBuffer2;	SendMessage( m_hInHereMapListCtrl, LVM_SETITEM,	 0, (LPARAM)&Li );
		Li.iItem = siCount++;	
	}

	return TRUE;
}

BOOL CALLBACK cltDevInHereMapChar::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			OnInitDialog( hDlg );
		}
		return TRUE;

	case WM_CHAR:
		return TRUE;

	case WM_TIMER:
		{
			OnButtonRefresh(m_hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		{
			// 윈도우의 위치를 기억한다. 
			GetWindowRect(m_hDlg, &DlgRect);

			DlgRectInitSwitch = TRUE;

			m_hDlg = NULL;

		}
		return TRUE;

	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_BUTTON_CHAR_REFRESH:
			{
				OnButtonRefresh(m_hDlg);
			}
			return TRUE;

		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;

		}
		return TRUE;
	}
	return FALSE;
}

void cltDevInHereMapChar::ShowInfo(HWND hDlg)
{
	SetWindowText( hDlg, "TEST" );
}