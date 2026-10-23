#include "./NotifyGetExpAndItemDlg.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "..\..\..\CommonLogic\ImageList\ImageList.h"
#include "../../../Common/Util/Control/Control.h"
#include "../../Client.h"

#include <CommonLogic.h>
extern cltCommonLogic* pclClient;


CNotifyGetExpAndItemDlg::CNotifyGetExpAndItemDlg()
{
	m_siCount = 0;
	m_bShow = false;
}

CNotifyGetExpAndItemDlg::~CNotifyGetExpAndItemDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CNotifyGetExpAndItemDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_NOTIFYGETEXPANDITEM ), hWnd,  StaticNotifyGetExpAndItem );
	Hide();

	return;
}

BOOL CALLBACK CNotifyGetExpAndItemDlg::StaticNotifyGetExpAndItem( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->m_pNotifyGetExpAndItemDlg->NotifyGetExpAndItem( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CNotifyGetExpAndItemDlg::NotifyGetExpAndItem( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			m_hImageWnd = GetDlgItem( hDlg, IDC_LIST_ITEM_IMAGE );

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( m_hImageWnd, LVSIL_NORMAL );
		}

		return TRUE;

	/*
	case WM_TIMER:
		{
			if ( m_siCount++ >= 1 )
			{
				Hide();
			}		
		}
		break;
	*/
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				return FALSE;
			}
		}
		break;

	case WM_CLOSE:
		Hide();
		break;

	case WM_DESTROY:
		{
			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

void CNotifyGetExpAndItemDlg::Show( cltItem *pclItem, SI32 Exp )
{
	if ( pclItem == NULL )
	{
		return;
	}	
	
	//KillTimer( m_hDlg, 0 );

	char itemname[128] = "";
	if(pclItem->GetFullName(pclClient->pclItemManager , itemname) == true)
	{
		m_siCount = 0;
		char szString[ 512 ] = "";


		SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_ITEM_IMAGE, NULL, pclItem, 5, 5 );

	
		char num[128]= " " ;
		char exp[128]= " " ;

		sprintf(num,"%d",pclItem->GetItemNum());
		sprintf(exp,"%d",Exp);

		char* pText = GetTxtFromMgr(1890);
		Nsprintf(szString,pText,"item",  itemname  ,"num",  num  ,"exp",  exp  ,NULL);
		//sprintf( szString, pText, itemname, pclItem->GetItemNum(), Exp );
		
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), szString );

		//SetTimer( m_hDlg, 0, 5000, NULL );

		m_bShow = true;

		ShowWindow( m_hDlg, SW_SHOWNOACTIVATE );
	}

	return;
}

void CNotifyGetExpAndItemDlg::Hide()
{
	m_siCount = 0;
	m_bShow = false;

	//KillTimer( m_hDlg, 0 );
	
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}