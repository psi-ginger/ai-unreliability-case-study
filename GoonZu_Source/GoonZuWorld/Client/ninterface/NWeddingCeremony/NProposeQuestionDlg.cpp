
#include "NProposeQuestionDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../common/Family/Marriage.h"

#include "../../CommonLogic/MsgType-Person.h"

extern cltCommonLogic* pclClient;

CNProposeQuestionDlg::CNProposeQuestionDlg( void )
{
	Init();
}

CNProposeQuestionDlg::~CNProposeQuestionDlg( void )
{

}

void CNProposeQuestionDlg::Init( )
{
}

void CNProposeQuestionDlg::Create()
{
	//-- �̹� �����Ǿ��ٸ� ����
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NWeddingCeremony/DLG_NPropose_Question.ddf" ) );
	file.CreatePopupDialog( this, NPROPOSE_QUESTION_DLG, TEXT( "dialog_propose_question" ), StaticCallBackDialogNProposeQuestionDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PROPOSE_QUESTION_DLG_STATICTEXT_NAME,		this), PROPOSE_QUESTION_DLG_STATICTEXT_NAME,	TEXT("statictext_name") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PROPOSE_QUESTION_DLG_STATICTEXT_MESSAGE,	this), PROPOSE_QUESTION_DLG_STATICTEXT_MESSAGE,	TEXT("statictext_message") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	PROPOSE_QUESTION_DLG_BUTTON_PROPOSE,		this), PROPOSE_QUESTION_DLG_BUTTON_PROPOSE,		TEXT("button_propose") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	PROPOSE_QUESTION_DLG_EDITBOX_NAME,			this), PROPOSE_QUESTION_DLG_EDITBOX_NAME,		TEXT("editbox_name") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	PROPOSE_QUESTION_DLG_EDITBOX_MESSAGE,		this), PROPOSE_QUESTION_DLG_EDITBOX_MESSAGE,	TEXT("editbox_message") );

	// ���� �� ����
	CEdit* pclName = m_InterfaceFactory.GetEdit( PROPOSE_QUESTION_DLG_EDITBOX_NAME );
	if ( pclName )
	{
		pclName->SetMaxEditTextLength( MAX_PLAYER_NAME-1 );
	}

	CEdit* pclMessage = m_InterfaceFactory.GetEdit( PROPOSE_QUESTION_DLG_EDITBOX_MESSAGE );
	if ( pclMessage )
	{
		pclMessage->SetMaxEditTextLength( Const_Max_Propose_Message-1 );
	}

	// ����Ǳ��� �⺻������ �־���� ���ǵ��� �˻��Ѵ�
	if ( FALSE == CheckCondition() )
	{
		DeleteDialog();
	}

}

void CALLBACK CNProposeQuestionDlg::StaticCallBackDialogNProposeQuestionDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNProposeQuestionDlg* pThis = (CNProposeQuestionDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNProposeQuestionDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNProposeQuestionDlg::CallBackDialogNProposeQuestionDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID	)
	{
	case PROPOSE_QUESTION_DLG_BUTTON_PROPOSE:
		{
			OnButtonClickPropose();
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}

void CNProposeQuestionDlg::OnButtonClickPropose( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// �⺻ ���� �˻�
	if ( FALSE == CheckCondition() )
	{
		return;
	}

	CEdit* pclCharName = m_InterfaceFactory.GetEdit( PROPOSE_QUESTION_DLG_EDITBOX_NAME );
	if ( NULL == pclCharName )
	{
		return;
	}

	NTCHARString32 strCharName = pclCharName->GetText();
	
	// ĳ���� �̸��� ���� �ʾҴٸ�
	if ( 0 >= strCharName.Length() )
	{
		NTCHARString64	strTitle( GetTxtFromMgr(422) );
		NTCHARString256 strText( GetTxtFromMgr(9988) );

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return;
	}

	CEdit* pclMessage = m_InterfaceFactory.GetEdit( PROPOSE_QUESTION_DLG_EDITBOX_MESSAGE );
	if ( NULL == pclMessage )
	{
		return;
	}

	NTCHARString256 strMessage = pclMessage->GetText();
	
	// ���� �޽����� ���ٸ�
	if ( 0 >= strMessage.Length() )
	{
		NTCHARString64	strTitle( GetTxtFromMgr(422) );
		NTCHARString256 strText( GetTxtFromMgr(9984) );

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return;
	}

	// ûȥ ����
	cltGameRequest_Propose_Question clInfo( strCharName, strMessage );
	cltMsg clMsg( GAMEMSG_REQUEST_PROPOSE_QUESTION, sizeof(clInfo), (BYTE*)&clInfo );
	pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

}

BOOL CNProposeQuestionDlg::CheckCondition( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return FALSE;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return FALSE;
	}

	// ��ȥ ���� ������ �ƴ϶��
	if ( Const_Marriage_Min_Level > pclchar->pclCI->clIP.GetLevel() )
	{
		NTCHARString64	strTitle( GetTxtFromMgr(422) );
		NTCHARString256 strText( GetTxtFromMgr(9983) );

		strText.Replace( TEXT("#level#"), SI32ToString(Const_Marriage_Min_Level) );

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return FALSE;
	}

	// ���� �̹� ��ȥ�� �ߴٸ�
	if ( TRUE == pclchar->IsMarried() )
	{
		NTCHARString64	strTitle( GetTxtFromMgr(422) );
		NTCHARString256 strText( GetTxtFromMgr(9985) );

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return FALSE;
	}

	// ���� �̿���� ����
	if ( 0 >= pclchar->pclCI->clCharItem.GetItemNum(Const_Family_Item_Unique) )
	{
		TCHAR* pszItemName = (TCHAR*)pclclient->pclItemManager->GetName( Const_Family_Item_Unique );
		if ( pszItemName )
		{
			NTCHARString64	strTitle( GetTxtFromMgr(422) );
			NTCHARString256 strText( GetTxtFromMgr(9986) );

			strText.Replace( TEXT("#itemname#"), pszItemName );

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		return FALSE;
	}

	return TRUE;
}
