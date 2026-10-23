#include "NAutoAnswerDlg.h"
#include "NPrivateChatDlgMgr.h"

#include "../../CommonLogic/CommonLogic.h"
#include "Client.h"
#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

CNAutoAnswerDlg::CNAutoAnswerDlg()
{
	Init();
}

CNAutoAnswerDlg::~CNAutoAnswerDlg()
{
	Destroy();
}

void CNAutoAnswerDlg::Init()
{
}

void CNAutoAnswerDlg::Destroy()
{
}

void CNAutoAnswerDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NPrivateChatDlg/DLG_NAutoAnswer.ddf"));
	file.CreatePopupDialog( this, NAUTOANSWER_DLG, TEXT("dialog_autoanswer"),StaticAutoAnswerDlgProc );

	// 컨트롤 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NAUTOANSWER_STATIC_NONAME, this) ,NAUTOANSWER_STATIC_NONAME, TEXT("statictext_NONAME1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NAUTOANSWER_EDIT_INPUT, this) ,NAUTOANSWER_EDIT_INPUT, TEXT("editbox_input"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX ,NAUTOANSWER_CHECK_USE, this) ,NAUTOANSWER_CHECK_USE, TEXT("checkbox_use"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NAUTOANSWER_BUTTON_SUBMIT, this) ,NAUTOANSWER_BUTTON_SUBMIT, TEXT("button_submit"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NAUTOANSWER_BUTTON_CANCEL, this) ,NAUTOANSWER_BUTTON_CANCEL, TEXT("button_cancel"));

	if( pclclient->m_pNPrivateChatDlgMgr )
	{
		CEdit* pEdit = m_InterfaceFactory.GetEdit( NAUTOANSWER_EDIT_INPUT );
		if ( pEdit )
		{
			pEdit->SetText( pclclient->m_pNPrivateChatDlgMgr->GetAutoAnswerMsg() );
		}

		CCheckBox* pCheck = m_InterfaceFactory.GetCheckBox( NAUTOANSWER_CHECK_USE );
		if ( pCheck )
		{
			pCheck->SetCheck( pclclient->m_pNPrivateChatDlgMgr->GetAutoAnswerSwitch() );
		}
	}
}

void CALLBACK CNAutoAnswerDlg::StaticAutoAnswerDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAutoAnswerDlg *pThis = (CNAutoAnswerDlg*) pControllerMgr;
	pThis->NAutoAnswerDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNAutoAnswerDlg::NAutoAnswerDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NAUTOANSWER_BUTTON_SUBMIT:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				Submit();
			}
		}
		break;
	case NAUTOANSWER_BUTTON_CANCEL:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				DeleteDialog();
			}
		}
		break;
	default:
		break;
	}
}

void CNAutoAnswerDlg::Submit()
{
	cltClient* pclclient = (cltClient*)pclClient;

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NAUTOANSWER_EDIT_INPUT );
	CCheckBox* pCheck = m_InterfaceFactory.GetCheckBox( NAUTOANSWER_CHECK_USE );

	if ( pclclient->m_pNPrivateChatDlgMgr )
	{
		pclclient->m_pNPrivateChatDlgMgr->SetAutoAnswerMsg( pEdit->GetText() );
		pclclient->m_pNPrivateChatDlgMgr->SetAutoAnswerSwitch( pCheck->IsCheck() );
	}

	DeleteDialog();
}