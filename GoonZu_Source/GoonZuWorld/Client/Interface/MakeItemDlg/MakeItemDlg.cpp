// MakeItemDlg.cpp: implementation of the CMakeItemDlg class.
//
//////////////////////////////////////////////////////////////////////
/*
#include "MakeItemDlg.h"

#include "..\..\..\Common\Skill\Skill-Manager.h"

#include "../../CommonLogic/MsgType-System.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBaseInfo.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeItemDlg::CMakeItemDlg()
{
	m_hImage = NULL ;
	m_hDlg = NULL ;
	m_hEdit = NULL ;
	m_hEditBrush = NULL ;
	m_hDlgBrush = NULL ;	
}

CMakeItemDlg::~CMakeItemDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow(m_hDlg);
		m_hDlg = NULL ;
	}
}

void CMakeItemDlg::Create( HWND hParent , HINSTANCE hInst )
{
	m_hWnd = hParent ;
	m_hInst = hInst ;
}

void CMakeItemDlg::CreateDlg()
{
	if ( !m_hDlg )
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_MAKEITEM ), m_hWnd, StaticMakeItemDlgMsgDlgProc );

	Hide();
}

BOOL CALLBACK CMakeItemDlg::StaticMakeItemDlgMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pMakeItemDlg->MakeItemDlgMsgDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CMakeItemDlg::MakeItemDlgMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEdit = GetDlgItem(hDlg,IDC_EDIT_MAKEITEM);
			m_hImage = GetDlgItem( hDlg, IDC_LIST_MAKEITME_IMAGE );

			m_hEditBrush = CreateSolidBrush(COLOR_DIALOG_YELLOW);
			m_hDlgBrush = CreateSolidBrush(COLOR_DIALOG_LIGHTBLUE);
			
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( m_hImage, LVSIL_NORMAL );
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_MAKEITEM ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );

				return (BOOL)m_hEditBrush;
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK );
			SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );

			return (BOOL)m_hDlgBrush;
		}
		break;
	case WM_CLOSE:
		{
			Hide();
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}

			m_hImage = NULL ;
			m_hEdit = NULL ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CMakeItemDlg::Set( cltItem *pclItem, SI32 baseexp, SI32 skillunique,SI32 skillexp,SI32 monstercorp)
{
	char buffer[1024] = "" ;
	char itemname[128] = "";
	char skillname[128] = "";

	float fMonsterCorp = max(-1000.0f,min(1000.0f,(float)monstercorp / 1000.0f)) ;

	if ( pclItem->GetFullName(pclClient->pclItemManager,itemname) == true)
	{
		char* pText = GetTxtFromMgr(1948);
		sprintf(buffer, pText,itemname,pclItem->GetItemNum(),baseexp,fMonsterCorp);
	}
	else return ;

	if(pclClient->pclSkillManager->GetSkillName(skillunique,skillname) == true )
	{
		char temp[256];
		char* pText = GetTxtFromMgr(1949);
		sprintf(temp, pText,skillname,skillexp);
		strcat(buffer, temp);
	}


	SetWindowText( m_hEdit, buffer );

	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_MAKEITME_IMAGE, NULL, pclItem, 5, 5 );

	Show();
}

void CMakeItemDlg::Show()
{
	ShowWindow(m_hDlg,SW_SHOW);
}

void CMakeItemDlg::Hide()
{
	ShowWindow(m_hDlg,SW_HIDE);
}

*/