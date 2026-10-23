#include "NValentineDayConfessionQuestionDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-NPC.h"

NValentineDayConfessionQuestionDlg::NValentineDayConfessionQuestionDlg()
{
    	
}

NValentineDayConfessionQuestionDlg::~NValentineDayConfessionQuestionDlg()
{
	
}

void NValentineDayConfessionQuestionDlg::Destroy()
{ 


}

void NValentineDayConfessionQuestionDlg::Create()
{
	if( IsCreate() ) return;

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NValentineDay/NValentineDayConfession_question.ddf");
	file.CreatePopupDialog( this, VALENTINEDAY_CONFESSION_QUESTION_DLG, "dialog_valentinedayconfession" , StaticNValentineDayConfessionQuestionDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,  		VALENTINEDAY_CONFESSION_QUESTION_STATIC_EXPLAIN,		this),  VALENTINEDAY_CONFESSION_QUESTION_STATIC_EXPLAIN,		TEXT("editbox_explain")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_CONFESSION_QUESTION_STATIC_NONAME1,		this),  VALENTINEDAY_CONFESSION_QUESTION_STATIC_NONAME1,		TEXT("statictext_NONAME1")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_CONFESSION_QUESTION_STATIC_NONAME2,		this),  VALENTINEDAY_CONFESSION_QUESTION_STATIC_NONAME2,		TEXT("statictext_NONAME2")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		VALENTINEDAY_CONFESSION_QUESTION_EDIT_NAME,				this),  VALENTINEDAY_CONFESSION_QUESTION_EDIT_NAME,				TEXT("editbox_name")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		VALENTINEDAY_CONFESSION_QUESTION_EDIT_CONFESSION_MSG,	this),  VALENTINEDAY_CONFESSION_QUESTION_EDIT_CONFESSION_MSG,	TEXT("editbox_confession_msg")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_CONFESSION_QUESTION_BUTTON_SEND,			this),  VALENTINEDAY_CONFESSION_QUESTION_BUTTON_SEND,			TEXT("button_send")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_CONFESSION_QUESTION_BUTTON_CANCEL,			this),  VALENTINEDAY_CONFESSION_QUESTION_BUTTON_CANCEL,			TEXT("button_cancel")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,  	VALENTINEDAY_CONFESSION_QUESTION_OUTLINE1,				this),  VALENTINEDAY_CONFESSION_QUESTION_OUTLINE1,				TEXT("outline_NONAME1")			);
	
	
	m_InterfaceFactory.GetEdit(VALENTINEDAY_CONFESSION_QUESTION_STATIC_EXPLAIN)->SetText( GetTxtFromMgr(10452) );
	m_InterfaceFactory.GetEdit(VALENTINEDAY_CONFESSION_QUESTION_EDIT_NAME)->SetMaxEditTextLength(MAX_PLAYER_NAME);
	m_InterfaceFactory.GetEdit(VALENTINEDAY_CONFESSION_QUESTION_EDIT_CONFESSION_MSG)->SetMaxEditTextLength(100);

	SetX( 450 ); 
	SetY( 300 );
}

void NValentineDayConfessionQuestionDlg::Action()
{

}

void NValentineDayConfessionQuestionDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	TSpr* pspr = pclClient->GetGlobalSpr( GIMG_VALENTINEDAY_HEART );
	if( pspr ) 
	{
		GP.PutSprScreen( pspr, GetX(),		 GetY(),	   max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  50, GetY() -  20, max(0, TABS(pclClient->CurrentClock / 120))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 100, GetY() +  80, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 160, GetY() + 180, max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum() );  
		GP.PutSprScreen( pspr, GetX() + 130, GetY() +  80, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 230, GetY() + 180, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() );  
	}	
}

void CALLBACK NValentineDayConfessionQuestionDlg::StaticNValentineDayConfessionQuestionDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NValentineDayConfessionQuestionDlg *pThis = (NValentineDayConfessionQuestionDlg *)pControllerMgr;
	pThis->NValentineDayConfessionQuestionDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NValentineDayConfessionQuestionDlg::NValentineDayConfessionQuestionDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_CONFESSION_QUESTION_BUTTON_SEND:
			{
				TCHAR* pszName = m_InterfaceFactory.GetEdit(VALENTINEDAY_CONFESSION_QUESTION_EDIT_NAME)->GetText();
				TCHAR* pszMsg  = m_InterfaceFactory.GetEdit(VALENTINEDAY_CONFESSION_QUESTION_EDIT_CONFESSION_MSG)->GetText();

				if( 0 >= strlen(pszName) || 0 >= strlen(pszMsg) ) return;

				pclclient->m_pCAbuseFilter->CheckAbuse(pszMsg);

				cltGameRequest_ValentineDay_Confession_Question clinfo( pszName, pszMsg );
				cltMsg clMsg( GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_QUESTION, PACKET(clinfo) );
				((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);

				DeleteDialog();
			}
			break;
		case VALENTINEDAY_CONFESSION_QUESTION_BUTTON_CANCEL:
			{
				DeleteDialog();
			}
			break;
			
		default:
			{

			}
			break;
	}
}

