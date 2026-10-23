#include "NValentineDayConfessionAnswerDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-NPC.h"

NValentineDayConfessionAnswerDlg::NValentineDayConfessionAnswerDlg()
{
	
}

NValentineDayConfessionAnswerDlg::~NValentineDayConfessionAnswerDlg()
{
	
}

void NValentineDayConfessionAnswerDlg::Destroy()
{ 


}

void NValentineDayConfessionAnswerDlg::Create()
{
	if( IsCreate() ) return;

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NValentineDay/NValentineDayConfession_Answer.ddf");
	file.CreatePopupDialog( this, VALENTINEDAY_CONFESSION_ANSWER_DLG, "dialog_valentinedayconfession" , StaticNValentineDayConfessionAnswerDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_CONFESSION_ANSWER_STATIC_NONAME1,		this),  VALENTINEDAY_CONFESSION_ANSWER_STATIC_NONAME1,		TEXT("statictext_NONAME1")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_CONFESSION_ANSWER_STATIC_NONAME2,		this),  VALENTINEDAY_CONFESSION_ANSWER_STATIC_NONAME2,		TEXT("statictext_NONAME2")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		VALENTINEDAY_CONFESSION_ANSWER_EDIT_CONFESSION_MSG,	this),  VALENTINEDAY_CONFESSION_ANSWER_EDIT_CONFESSION_MSG,	TEXT("editbox_confession_msg")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_CONFESSION_ANSWER_BUTTON_SEND,			this),  VALENTINEDAY_CONFESSION_ANSWER_BUTTON_SEND,			TEXT("button_send")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_CONFESSION_ANSWER_BUTTON_CANCEL,		this),  VALENTINEDAY_CONFESSION_ANSWER_BUTTON_CANCEL,		TEXT("button_cancel")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,  	VALENTINEDAY_CONFESSION_ANSWER_OUTLINE1,			this),  VALENTINEDAY_CONFESSION_ANSWER_OUTLINE1,			TEXT("outline_NONAME1")			);
	
	SetActionTimer( 30000, pclClient->CurrentClock );    
	SetTimerOnOff( true );
	
	SetX( 450 ); 
	SetY( 300 );
}

void NValentineDayConfessionAnswerDlg::Action()
{
	cltGameRequest_ValentineDay_Confession_Answer clinfo( m_siPersonID, false );
	cltMsg clMsg( GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_ANSWER, PACKET(clinfo) );
	((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);

	DeleteDialog();
}

void NValentineDayConfessionAnswerDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	TSpr* pspr = pclClient->GetGlobalSpr( GIMG_VALENTINEDAY_HEART );
	if( pspr ) 
	{
		GP.PutSprScreen( pspr, GetX(),		 GetY(),	   max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  50, GetY() -  20, max(0, TABS(pclClient->CurrentClock / 120))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 120, GetY() +  80, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 160, GetY() + 120, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
	}	
}

void NValentineDayConfessionAnswerDlg::SetQuestion( SI32 _siPersonID, TCHAR* _pszName, TCHAR* _pszMsg )
{
	m_siPersonID = _siPersonID;

	NTCHARString128 szExplain = GetTxtFromMgr(10460);
	szExplain.Replace("#sname#", _pszName );
	m_InterfaceFactory.GetStatic(VALENTINEDAY_CONFESSION_ANSWER_STATIC_NONAME1)->SetText( szExplain );

	m_InterfaceFactory.GetEdit(VALENTINEDAY_CONFESSION_ANSWER_EDIT_CONFESSION_MSG)->SetText( _pszMsg );
}

void CALLBACK NValentineDayConfessionAnswerDlg::StaticNValentineDayConfessionAnswerDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NValentineDayConfessionAnswerDlg *pThis = (NValentineDayConfessionAnswerDlg *)pControllerMgr;
	pThis->NValentineDayConfessionAnswerDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NValentineDayConfessionAnswerDlg::NValentineDayConfessionAnswerDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_CONFESSION_ANSWER_BUTTON_SEND:
			{
				cltGameRequest_ValentineDay_Confession_Answer clinfo( m_siPersonID, true );
				cltMsg clMsg( GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_ANSWER, PACKET(clinfo) );
				((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);
				
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_CONFESSION_ANSWER_BUTTON_CANCEL:
			{
				cltGameRequest_ValentineDay_Confession_Answer clinfo( m_siPersonID, false );
				cltMsg clMsg( GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_ANSWER, PACKET(clinfo) );
				((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);

				DeleteDialog();
			}
			break;
			
		default:
			{

			}
			break;
	}
}

