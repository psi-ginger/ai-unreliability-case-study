
#include "NProposeAnswerDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../common/Family/Marriage.h"

#include "../../CommonLogic/MsgType-Person.h"

extern cltCommonLogic* pclClient;

CNProposeAnswerDlg::CNProposeAnswerDlg( void )
{

}

CNProposeAnswerDlg::~CNProposeAnswerDlg( void )
{

}

void CNProposeAnswerDlg::Init( )
{
		
}

void CNProposeAnswerDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NWeddingCeremony/DLG_NPropose_Answer.ddf" ) );
	file.CreatePopupDialog( this, NPROPOSE_ANSWER_DLG, TEXT( "dialog_propose_answer" ), StaticCallBackDialogNProposeAnswerDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PROPOSE_ANSWER_DLG_STATICTEXT_NONAME1,	this),	PROPOSE_ANSWER_DLG_STATICTEXT_NONAME1,	TEXT("statictext_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	PROPOSE_ANSWER_DLG_BUTTON_YES,			this),	PROPOSE_ANSWER_DLG_BUTTON_YES,			TEXT("button_yes") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	PROPOSE_ANSWER_DLG_BUTTON_NO,			this),	PROPOSE_ANSWER_DLG_BUTTON_NO,			TEXT("button_no") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	PROPOSE_ANSWER_DLG_EDITBOX_NAME,		this),	PROPOSE_ANSWER_DLG_EDITBOX_NAME,		TEXT("editbox_name") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	PROPOSE_ANSWER_DLG_EDITBOX_MESSAGE,		this),	PROPOSE_ANSWER_DLG_EDITBOX_MESSAGE,		TEXT("editbox_message") );

}

void CALLBACK CNProposeAnswerDlg::StaticCallBackDialogNProposeAnswerDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNProposeAnswerDlg* pThis = (CNProposeAnswerDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNProposeAnswerDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNProposeAnswerDlg::CallBackDialogNProposeAnswerDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case PROPOSE_ANSWER_DLG_BUTTON_YES:
		{
			OnButtonClickYes();
		}
		break;

	case PROPOSE_ANSWER_DLG_BUTTON_NO:
		{
			OnButtonClickNo();
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			// 그냥 닫을 시에는 거절이라고 봐도 무방하다
			AnswerThePropose( false );

			DeleteDialog();
		}
	}
}

void CNProposeAnswerDlg::OnButtonClickYes( void )
{
	AnswerThePropose( true );
}

void CNProposeAnswerDlg::OnButtonClickNo( void )
{
	AnswerThePropose( false );
}

void CNProposeAnswerDlg::AnswerThePropose( bool bAccept )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclclient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 청혼 승낙시에는 각종 검사를 한다
	if ( true == bAccept )
	{
		// 이미 결혼을 했는가
		if ( TRUE == pclchar->IsMarried() )
		{
			return;
		}

		// 결혼 가능 레벨인가
		if ( Const_Marriage_Min_Level > pclchar->pclCI->clIP.GetLevel() )
		{
			return;
		}

		// 가족 이용권 아이템이 없다면
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
			return;
		}
	}

	cltGameRequest_Propose_Answer clInfo( bAccept );
	cltMsg clMsg( GAMEMSG_REQUEST_PROPOSE_ANSWER, sizeof(clInfo), (BYTE*)&clInfo );
	pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

	// 패킷을 보냈으면 창을 닫아 버린다
	DeleteDialog();

}

void CNProposeAnswerDlg::SetPropose( TCHAR* pszCharName, TCHAR* pszMessage )
{
	if ( (NULL == pszCharName) || (NULL == pszMessage) )
	{
		DeleteDialog();
		return;
	}

	CEdit* pclCharName = m_InterfaceFactory.GetEdit( PROPOSE_ANSWER_DLG_EDITBOX_NAME );
	if ( NULL == pclCharName )
	{
		DeleteDialog();
		return;
	}

	CEdit* pclMessage = m_InterfaceFactory.GetEdit( PROPOSE_ANSWER_DLG_EDITBOX_MESSAGE );
	if ( NULL == pclMessage )
	{
		DeleteDialog();
		return;
	}

	// 청혼한 캐릭터 이름
	NTCHARString128 strCharName = GetTxtFromMgr( 9977 );

	strCharName.Replace( TEXT("#name#"), pszCharName );

	pclCharName->SetText( strCharName );

	// 청혼 메시지
	pclMessage->SetText( pszMessage );

}
