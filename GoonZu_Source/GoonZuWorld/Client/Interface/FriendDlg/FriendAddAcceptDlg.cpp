#include "FriendAddAcceptDlg.h"

#include "../../Client/client.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../common/SMLib/LibList/LibListMgr.h"
#include "../../common/SMLib/LibList/LibList.h"
#include "../../common/SMLib/LibList/Obj.h"

extern cltCommonLogic* pclClient;

#define MAX_ACCEPTLISTOBJ_BUFFER_SIZE		128

#define FRIENDACCEPTDLG_TIMER_VALUE			10000

CFriendAddAcceptDlg::CFriendAddAcceptDlg()
{
	m_hDlg = NULL;
	m_bShow = false;
	m_hEditBrush = NULL;

	m_pAcceptListMgr = new CLibListMgr();
	m_pAcceptListMgr->CreateMaxEmptyObj( MAX_FRIEND_NUMBER, MAX_ACCEPTLISTOBJ_BUFFER_SIZE );
}

CFriendAddAcceptDlg::~CFriendAddAcceptDlg()
{
	if ( m_pAcceptListMgr )
	{
		delete m_pAcceptListMgr;
		m_pAcceptListMgr = NULL;
	}
	
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}

}

void CFriendAddAcceptDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_FRIENDADDACCEPT ), hWnd,  StaticFriendAddAcceptDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CFriendAddAcceptDlg::StaticFriendAddAcceptDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	pclclient->m_pFriendAddAcceptDlg->FriendAddAcceptDlgProc( hDlg, iMsg, wParam, lParam );
	return FALSE;
}

BOOL CALLBACK CFriendAddAcceptDlg::FriendAddAcceptDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			m_hDlg = hDlg;
			
			m_siPersonID = 0;
			m_siCharID = 0;
			
			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);

			SetTimer( hDlg, 0, FRIENDACCEPTDLG_TIMER_VALUE, NULL );
		}
		return TRUE;
	
	case WM_TIMER:
		{
			// 보이지 않는 경우라면, 버퍼에서 꺼내온다.
			if ( ! IsShow() )
			{				
				PopAcceptList();
			}
		}
		break;

	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_FRIENDADDEXPLAIN ) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
				Hide();
				return TRUE;
				
			case IDCANCEL:					
				Hide();
				return TRUE;

			case IDC_BUTTON_FRIENDADDACCEPT:
				{
					if ( m_siPersonID < 1 || m_strCharName == NULL || strlen( m_strCharName ) < 1 )
					{
				
					}
					else
					{
						cltGameMsgRequest_FriendAddAccept clFriendAddAccept( m_siPersonID, m_siCharID, m_strCharName );
						cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADDACCEPT, sizeof( clFriendAddAccept ), (char*)&clFriendAddAccept );
						pclClient->SendMsgToServer((char*)&clMsg);
					}

					Hide();
				}
				break;
			case IDC_BUTTON_FRIENDADDREFUSE:
				{
					Hide();
				}
				break;
			}
		}
		break;
	

	case WM_CLOSE:
		Hide();
		break;

	case WM_DESTROY:
		{
			KillTimer( hDlg, 0 );

			if(m_hEditBrush)
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
};

bool CFriendAddAcceptDlg::IsShow()
{
	return m_bShow;
}

void CFriendAddAcceptDlg::Show()
{
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
	return;
}

void CFriendAddAcceptDlg::Hide()
{
	m_siPersonID = 0;
	m_siCharID = 0;
	memset( m_strCharName, 0, MAX_PLAYER_NAME );
	
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );
	
	return;
}

void CFriendAddAcceptDlg::PushAccpetList( SI32 PersonID, SI32 CharID, char *CharName )
{	
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}
		
	if ( CharName == NULL || strlen( CharName ) < 1 || PersonID < 1 || CharID < 1 )
	{
		return;
	}

	if ( m_siPersonID == PersonID )
	{
		return;
	}

	CObj *pObj = m_pAcceptListMgr->GetExistList()->GetFirst();

	// 리스트에 있는 지 확인하기
	while ( pObj != NULL )
	{
		stAcceptObj *pAcceptObj = (stAcceptObj *)pObj->GetDataPtr();
		if ( pAcceptObj != NULL )
		{
			if ( pAcceptObj->siPersonID == PersonID )
			{
				return;
			}
		}

		pObj = pObj->GetNext();
	}


	stAcceptObj sAcceptObj;
	
	sAcceptObj.Set( PersonID, CharID, CharName );


	pObj = m_pAcceptListMgr->PopFrontToEmptyListAndPushBackToExistList();
	
	if ( pObj == NULL )
	{
		return;
	}

	pObj->SetData( (char*)&sAcceptObj, sizeof( stAcceptObj ) );
	
	return;
}

void CFriendAddAcceptDlg::PopAcceptList()
{
	if ( IsShow() )
	{
		return;
	}

	CObj *pObj = m_pAcceptListMgr->GetExistList()->GetFirst();

	if ( pObj == NULL )
	{
		return;
	}

	stAcceptObj *pAcceptObj = (stAcceptObj *)pObj->GetDataPtr();

	if ( pAcceptObj == NULL )
	{
		return;
	}

	char Buffer[256 ];

	char* pText = GetTxtFromMgr(3917);
	sprintf( Buffer, pText, pAcceptObj->strCharName );

	m_siPersonID = pAcceptObj->siPersonID;
	m_siCharID = pAcceptObj->siCharID;
	strcpy( m_strCharName, pAcceptObj->strCharName );

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FRIENDADDEXPLAIN ), Buffer );
	
	Show();

	m_pAcceptListMgr->GetExistList()->Pop( pObj );
	pObj->Initialize();
	m_pAcceptListMgr->PushBackToEmptyList( pObj );

	return;
}