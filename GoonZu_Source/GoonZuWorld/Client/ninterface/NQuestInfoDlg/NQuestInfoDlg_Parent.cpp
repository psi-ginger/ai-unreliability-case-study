#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"

#include "NQuestInfoDlg_Parent.h"
#include "NQuestInfoDlg_GoingQuest.h"
#include "NQuestInfoDlg_AllQuest.h"


CNQuestInfoDlg_Parent::CNQuestInfoDlg_Parent() : CDialogController()
{
	ZeroMemory( m_pTabDialog, sizeof(m_pTabDialog) );

}

CNQuestInfoDlg_Parent::~CNQuestInfoDlg_Parent()
{
	for ( SI32 siIndex=0; siIndex<QUEST_INFO_TAB_MAX; ++siIndex )
	{
		SAFE_DELETE( m_pTabDialog[siIndex] );
	}

}

void CNQuestInfoDlg_Parent::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NQuestInfoDlg/DLG_NQuestInfo_Parent.ddf" ) );
	file.CreatePopupDialog( this, NQUESTINFODLG_PARENT, TEXT( "dialog_questinfo" ), StaticQuestInfoDlg_ParentProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NQUESTINFO_PARENT_EDITBOX_SUBTITLE,		this ), NQUESTINFO_PARENT_EDITBOX_SUBTITLE,		TEXT( "editbox_subtitle" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NQUESTINFO_PARENT_BUTTON_GOINGQUEST,	this ), NQUESTINFO_PARENT_BUTTON_GOINGQUEST,	TEXT( "button_goingquest" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NQUESTINFO_PARENT_BUTTON_ALLQUEST,		this ), NQUESTINFO_PARENT_BUTTON_ALLQUEST,		TEXT( "button_allquest" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NQUESTINFO_PARENT_BUTTON_OK,			this ), NQUESTINFO_PARENT_BUTTON_OK,			TEXT( "button_ok" ) );

	//-- 컨트롤 설정


	// 진행중인 퀘스트 다이얼로그
	m_pTabDialog[QUEST_INFO_TAB_GOINGQUEST] = new CNQuestInfoDlg_GoingQuest;
	if ( NULL == m_pTabDialog[QUEST_INFO_TAB_GOINGQUEST] )
	{
		DeleteDialog();
	}
	
	((CNQuestInfoDlg_GoingQuest*)m_pTabDialog[QUEST_INFO_TAB_GOINGQUEST])->Create( this );
	((CNQuestInfoDlg_GoingQuest*)m_pTabDialog[QUEST_INFO_TAB_GOINGQUEST])->Hide();


	// 전체 퀘스트 다이얼로그
	m_pTabDialog[QUEST_INFO_TAB_ALLQUEST] = new CNQuestInfoDlg_AllQuest;
	if ( NULL == m_pTabDialog[QUEST_INFO_TAB_ALLQUEST] )
	{
		DeleteDialog();
	}

	((CNQuestInfoDlg_AllQuest*)m_pTabDialog[QUEST_INFO_TAB_ALLQUEST])->Create( this );
	((CNQuestInfoDlg_AllQuest*)m_pTabDialog[QUEST_INFO_TAB_ALLQUEST])->Hide();


	//-- 기본 초기 설정
	SetTimerOnOff( true );
	SetActionTimer( 1000 );	

	SelectTabDialog( QUEST_INFO_TAB_GOINGQUEST );

	//-- 퀘스트 설정
	pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_OPENQUESTDIALOG );
}

void CALLBACK CNQuestInfoDlg_Parent::StaticQuestInfoDlg_ParentProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNQuestInfoDlg_Parent *pThis = (CNQuestInfoDlg_Parent*) pControllerMgr;
	pThis->NQuestInfoDlg_ParentProc( nEvent, nControlID, pControl );
}

void CALLBACK CNQuestInfoDlg_Parent::NQuestInfoDlg_ParentProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch( nControlID )
	{
		case NQUESTINFO_PARENT_BUTTON_GOINGQUEST:
			{
				SelectTabDialog( QUEST_INFO_TAB_GOINGQUEST );
			}
			break;
		case NQUESTINFO_PARENT_BUTTON_ALLQUEST:
			{
				SelectTabDialog( QUEST_INFO_TAB_ALLQUEST );
			}
			break;

		case NQUESTINFO_PARENT_BUTTON_OK:
			{
				DeleteDialog();
			}
			break;

		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;

	}

}

void CNQuestInfoDlg_Parent::Action()
{
	for ( SI32 siIndex=0; siIndex<QUEST_INFO_TAB_MAX; ++siIndex )
	{
		m_pTabDialog[siIndex]->Action();
	}

}

void CNQuestInfoDlg_Parent::Show()
{
	CControllerMgr::Show( true );
}

void CNQuestInfoDlg_Parent::Hide()
{
	CControllerMgr::Show( false );
}

void CNQuestInfoDlg_Parent::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch ( MsgIndex )
	{
		case QUEST_INFO_REGMSG_GOING_QUESTCANCEL:
			{
				((CNQuestInfoDlg_GoingQuest*)m_pTabDialog[QUEST_INFO_TAB_GOINGQUEST])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
			}
			break;
	}
}

void CNQuestInfoDlg_Parent::SelectTabDialog( SI32 siTabIndex )
{
	if ( (QUEST_INFO_TAB_GOINGQUEST > siTabIndex) || (QUEST_INFO_TAB_MAX <= siTabIndex) )
	{
		return;
	}

	// 텝 컨틀롤러가 추가되면 아래 siID배열에 넣으세요
	SI32 siID[]		= { NQUESTINFO_PARENT_BUTTON_GOINGQUEST, NQUESTINFO_PARENT_BUTTON_ALLQUEST };
	SI32 siCount	= sizeof( siID ) / sizeof( siID[0] );


	for ( SI32 siIndex=0; siIndex<siCount; ++siIndex )
	{
		SI32		siNowID		= siID[siIndex];
		CButton*	pclButton	= m_InterfaceFactory.GetButton( siNowID );
		if ( NULL == pclButton )
		{
			continue;
		}

		if ( siTabIndex == siIndex )
		{
			SetTabDialogButton( pclButton, CONTROLLER_BUTTONTEXT_COLOR, FW_BOLD );
			m_pTabDialog[siIndex]->Show( true );


			// 부 제목 변경
			m_InterfaceFactory.GetEdit( NQUESTINFO_PARENT_EDITBOX_SUBTITLE )->SetText( pclButton->GetText() );
		}
		else
		{
			SetTabDialogButton( pclButton, CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL );
			m_pTabDialog[siIndex]->Show( false );
		}
	}
	
}

void CNQuestInfoDlg_Parent::SetTabDialogButton( CButton* pclButton, COLORREF FontColor, SI16 siWeight )
{
	if ( NULL == pclButton )
	{
		return;
	}

	pclButton->SetFontColor( FontColor  );
	pclButton->SetFontWeight( siWeight );
	pclButton->Refresh();

}