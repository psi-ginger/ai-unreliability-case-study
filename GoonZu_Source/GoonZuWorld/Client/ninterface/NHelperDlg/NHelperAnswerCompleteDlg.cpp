#include "NHelperAnswerCompleteDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-System.h"

NHelperAnswerCompleteDlg::NHelperAnswerCompleteDlg()
{

}


NHelperAnswerCompleteDlg::~NHelperAnswerCompleteDlg()
{
	Destroy();
}


void NHelperAnswerCompleteDlg::Init()
{

}


void NHelperAnswerCompleteDlg::Destroy()
{


}


void NHelperAnswerCompleteDlg::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NHelperDlg/NHelper_Answer_Complete.ddf");

	file.CreatePopupDialog( this, NHELPER_ANSWER_COMPLETE_DLG, "dialog_answer_complete" , StaticNHelperAnswerCompleteDlgProc );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NHELPER_ANSWER_COMPLETE_DLG_STATIC_TITLE,			this), NHELPER_ANSWER_COMPLETE_DLG_STATIC_TITLE,			"statictext_title"		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NHELPER_ANSWER_COMPLETE_DLG_STATIC_CONTENTS,		this), NHELPER_ANSWER_COMPLETE_DLG_STATIC_CONTENTS,			"statictext_contents"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NHELPER_ANSWER_COMPLETE_DLG_EDIT_TITLE,				this), NHELPER_ANSWER_COMPLETE_DLG_EDIT_TITLE,				"editbox_title"			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NHELPER_ANSWER_COMPLETE_DLG_EDIT_CONTENTS,			this), NHELPER_ANSWER_COMPLETE_DLG_EDIT_CONTENTS,			"editbox_contents"		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NHELPER_ANSWER_COMPLETE_DLG_BUTTON_SATISFACTION,	this), NHELPER_ANSWER_COMPLETE_DLG_BUTTON_SATISFACTION,		"button_satisfaction"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NHELPER_ANSWER_COMPLETE_DLG_BUTTON_DISSATISFACTION,	this), NHELPER_ANSWER_COMPLETE_DLG_BUTTON_DISSATISFACTION,	"button_dissatisfaction");
	

	m_InterfaceFactory.GetEdit( NHELPER_ANSWER_COMPLETE_DLG_EDIT_TITLE )->SetMaxEditTextLength( TITLE_LETTER_NUM );
	m_InterfaceFactory.GetEdit( NHELPER_ANSWER_COMPLETE_DLG_EDIT_CONTENTS )->SetMaxEditTextLength( CONTENTS_LETTER_NUM );
} 

void NHelperAnswerCompleteDlg::Action()
{
	
}

void CALLBACK NHelperAnswerCompleteDlg::StaticNHelperAnswerCompleteDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NHelperAnswerCompleteDlg *pThis = (NHelperAnswerCompleteDlg *)pControllerMgr;
	pThis->NHelperAnswerCompleteDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NHelperAnswerCompleteDlg::NHelperAnswerCompleteDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;
	
	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				//DeleteDialog();
			}
			break;

		case NHELPER_ANSWER_COMPLETE_DLG_BUTTON_SATISFACTION:
		case NHELPER_ANSWER_COMPLETE_DLG_BUTTON_DISSATISFACTION:
			{
				bool Result = NHELPER_ANSWER_COMPLETE_DLG_BUTTON_SATISFACTION == nControlID ? true : false;
				
				cltGameMsgRequest_Add_Answer_Complete_Result clInfo( Result, m_siAnswerCharPersonID );
				cltMsg clMsg( GAMEMSG_REQUEST_ADD_ANSWER_COMPLETE_RESULT, sizeof(clInfo), (BYTE*)&clInfo );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				DeleteDialog();
			}
			break;
	}
}

void NHelperAnswerCompleteDlg::SetAnswer( SI32 _siAnswerPersonID, TCHAR* _pAnswerCharName, TCHAR* _pTitle, TCHAR* _pContents )
{
	m_siAnswerCharPersonID = _siAnswerPersonID;
	
	NTCHARString512 kbuffer(GetTxtFromMgr(9624));
	kbuffer.Replace( "#name#",	_pAnswerCharName );
	
	SetTitle( kbuffer );
	
	m_InterfaceFactory.GetEdit(NHELPER_ANSWER_COMPLETE_DLG_EDIT_TITLE)->SetText( _pTitle );
	m_InterfaceFactory.GetEdit(NHELPER_ANSWER_COMPLETE_DLG_EDIT_CONTENTS)->SetText( _pContents );
}


