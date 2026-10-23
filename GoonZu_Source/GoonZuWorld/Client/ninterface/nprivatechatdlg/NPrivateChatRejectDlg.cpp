#include "NPrivateChatRejectDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "NPrivateChatDlgMgr.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CNPrivateChatRejectDlg::CNPrivateChatRejectDlg()
{
	m_pNameStatic = NULL;
	m_pRejectCancelButton = NULL;
	m_pList = NULL;
}

CNPrivateChatRejectDlg::~CNPrivateChatRejectDlg()
{
	SAFE_DELETE( m_pNameStatic );
	SAFE_DELETE( m_pRejectCancelButton );
	SAFE_DELETE( m_pList );
}

void CNPrivateChatRejectDlg::Create()
{
	if( IsCreate() )
	{
		DeleteDialog();
	}
	else
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NPrivateChatDlg/DLG_NPrivateChatReject.ddf"));
		file.CreatePopupDialog( this, NPRIVATECHATREJECT_DLG, TEXT("dialog_PrivateChatReject"), StaticCallBackDialogNPrivateChatReject );

		m_pNameStatic = new CStatic( this );
		m_pRejectCancelButton = new CButton( this );
		m_pList = new CList( this );

		file.CreateControl( m_pNameStatic,			NPRIVATECHATREJECT_DIALOG_STATIC_NAME,		TEXT("statictext_Name") );
		file.CreateControl( m_pRejectCancelButton,	NPRIVATECHATREJECT_DIALOG_BUTTON_CANCEL,	TEXT("button_Cancel") );
		file.CreateControl( m_pList,				NPRIVATECHATREJECT_DIALOG_BUTTON_LIST,		TEXT("listbox_PrivateChatRejectList") );
		
		TCHAR* pText = GetTxtFromMgr(3413);
		m_pList->SetColumn( 0, 185, pText );
		m_pList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		m_pList->SetDrawLine( false );
		m_pList->SetBorder( true );
		m_pList->SetBKColorUse( true );

		LoadRejectList();
	}

	return;
}

void CALLBACK CNPrivateChatRejectDlg::StaticCallBackDialogNPrivateChatReject( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPrivateChatRejectDlg *pThis = (CNPrivateChatRejectDlg*)pControllerMgr;
	pThis->CallBackDialogNPrivateChatReject( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNPrivateChatRejectDlg::CallBackDialogNPrivateChatReject( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:		DeleteDialog();		break;
	case NPRIVATECHATREJECT_DIALOG_BUTTON_CANCEL:
		{
			SI16 SelectedIndex = m_pList->GetSelectedIndex();
			if( SelectedIndex > -1 )
			{
				cltClient *pclclient = (cltClient *)pclClient;
				pclclient->m_pNPrivateChatDlgMgr->DelRejectUser( m_pNameStatic->GetText() );
				LoadRejectList();
			}
		}
		break;
	case NPRIVATECHATREJECT_DIALOG_BUTTON_LIST:
		{
			SI16 SelectedIndex = m_pList->GetSelectedIndex();
			if( SelectedIndex > -1 )
			{
				stListBoxItemData *pListBoxItemData = m_pList->GetItemData( SelectedIndex );
				if ( pListBoxItemData && pListBoxItemData->strText[ 0 ] )
				{
					m_pNameStatic->SetText( pListBoxItemData->strText[ 0 ], DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				}
			}
		}
		break;
	}
	return;
}

void CNPrivateChatRejectDlg::LoadRejectList()
{
	cltClient *pclclient = (cltClient *)pclClient;

	m_pNameStatic->SetText( TEXT(""), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	
	m_pList->Clear();

	stListBoxItemData	ListBoxItemData;
	
	for ( SI16 i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		if ( _tcslen( pclclient->m_pNPrivateChatDlgMgr->m_strRejectUserList[i] ) > 0 )
		{
			ListBoxItemData.Set( 0, pclclient->m_pNPrivateChatDlgMgr->m_strRejectUserList[i] );
			m_pList->AddItem( &ListBoxItemData );
		}
	}

	m_pList->Refresh();
}