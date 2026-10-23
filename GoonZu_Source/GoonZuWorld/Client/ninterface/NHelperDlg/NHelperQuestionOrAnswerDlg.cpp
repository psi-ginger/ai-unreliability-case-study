#include "NHelperQuestionOrAnswerDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-System.h"

NHelperQuestionOrAnswerDlg::NHelperQuestionOrAnswerDlg()
{

}


NHelperQuestionOrAnswerDlg::~NHelperQuestionOrAnswerDlg()
{
	Destroy();
}


void NHelperQuestionOrAnswerDlg::Init()
{

}


void NHelperQuestionOrAnswerDlg::Destroy()
{


}


void NHelperQuestionOrAnswerDlg::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NHelperDlg/NHelper_Question_or_Answer.ddf");

	file.CreatePopupDialog( this, NHELPER_QUESTION_OR_ANSWER_DLG, "dialog_question_or_answer" , StaticNHelperQuestionOrAnswerProc );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NHELPER_QUESTION_OR_ANSWER_DLG_STATIC_TITLE,	this), NHELPER_QUESTION_OR_ANSWER_DLG_STATIC_TITLE,		"statictext_title");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NHELPER_QUESTION_OR_ANSWER_DLG_STATIC_CONTENTS, this), NHELPER_QUESTION_OR_ANSWER_DLG_STATIC_CONTENTS,  "statictext_contents");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE,		this), NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE,		"editbox_title");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS,	this), NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS,	"editbox_contents");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NHELPER_QUESTION_OR_ANSWER_DLG_BUTTON,			this), NHELPER_QUESTION_OR_ANSWER_DLG_BUTTON,			"button_NONAME1");
	

	m_InterfaceFactory.GetEdit( NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE )->SetMaxEditTextLength( TITLE_LETTER_NUM );
	m_InterfaceFactory.GetEdit( NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS )->SetMaxEditTextLength( CONTENTS_LETTER_NUM );

	SetActionTimer( 1000 );	// 
	SetTimerOnOff( true );

	SetX(220);
	SetY(230); 
} 

void NHelperQuestionOrAnswerDlg::Action()
{
	// 답변중이면 10분이내에 작성하도록 한다.
	if( ANSWER_DLG == m_bQuestionDlg )
	{
		if( 600000 < GetTickCount() - m_dwPrevTime )
		{
			// 답변하지 않고 그냥 창을 닫아서 답변자 삭제를 서버에 보낸다.
			cltMsg clMsg( GAMEMSG_REQUEST_DEL_ANSWER, 0, NULL);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

			DeleteDialog();
		}
		else 
		{
			NTCHARString256 kBuffer(GetTxtFromMgr(1378)); 
			kBuffer.Replace("%d", SI32ToString( (600000 - (GetTickCount() - m_dwPrevTime)) / 1000 ));
			SetTitle( kBuffer );   
		}
	}
}

void CALLBACK NHelperQuestionOrAnswerDlg::StaticNHelperQuestionOrAnswerProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NHelperQuestionOrAnswerDlg *pThis = (NHelperQuestionOrAnswerDlg *)pControllerMgr;
	pThis->NHelperQuestionOrAnswerProc( nEvent, nControlID, pControl );
}


void CALLBACK NHelperQuestionOrAnswerDlg::NHelperQuestionOrAnswerProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;
	
	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				if( ANSWER_DLG == m_bQuestionDlg )
				{
					// 답변하지 않고 그냥 창을 닫아서 답변자 삭제를 서버에 보낸다.
					cltMsg clMsg( GAMEMSG_REQUEST_DEL_ANSWER, 0, NULL);
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
				}
				
				DeleteDialog();
			}
			break;

		case NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE:
			{
				switch( nEvent )
				{
					case EVENT_EDITBOX_CHANGE:
						{
							CEdit* pTitle = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE);
					
							memcpy( m_szTitle, pTitle->GetText(), strlen(pTitle->GetText()) );
						}
				}
				break;
			}
			break;
		
		case NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS:
			{
				switch( nEvent )
				{
					case EVENT_EDITBOX_CHANGE:
						{
							CEdit* pContents = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS);

							memcpy( m_szContents, pContents->GetText(), strlen(pContents->GetText()) );
						}
				}
			}
			break;
		case NHELPER_QUESTION_OR_ANSWER_DLG_BUTTON:
			{
				// 질문하기.
				if( QUESTION_DLG == m_bQuestionDlg )
				{
					CEdit* pTitle	 = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE);
					CEdit* pContents = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS);

					if( 4 > _tcslen(pTitle->GetText()) || 4 > _tcslen(pContents->GetText())	)
						return;

					cltGameMsgRequest_Add_Question clInfo( pTitle->GetText(), pContents->GetText() );
					cltMsg clMsg(GAMEMSG_REQUEST_ADD_QUESTION, sizeof(clInfo), (BYTE*)&clInfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				//답변하기.
				else if( ANSWER_DLG == m_bQuestionDlg )
				{
					CEdit* pTitle	 = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE);
					CEdit* pContents = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_CONTENTS);

					if( 4 > _tcslen(pTitle->GetText()) || 4 > _tcslen(pContents->GetText())	)
						return;

					if( 0 < m_siQuestionPersonID )
					{
						cltGameMsgRequest_Add_Answer_Complete clInfo( m_siQuestionPersonID, pTitle->GetText(), pContents->GetText() );
						cltMsg clMsg(GAMEMSG_REQUEST_ADD_ANSWER_COMPLETE, sizeof(clInfo), (BYTE*)&clInfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
			}
			break;
	}
}


void NHelperQuestionOrAnswerDlg::SetQuestionDlg( bool _bQuestionDlg )
{
	m_bQuestionDlg = _bQuestionDlg;	

	// 질문하기.
	if( QUESTION_DLG == m_bQuestionDlg )
	{
		m_InterfaceFactory.GetButton( NHELPER_QUESTION_OR_ANSWER_DLG_BUTTON )->SetText(GetTxtFromMgr(9735));
	}
	//답변하기.
	else if( ANSWER_DLG == m_bQuestionDlg )
	{
		m_dwPrevTime = GetTickCount();
		m_InterfaceFactory.GetButton( NHELPER_QUESTION_OR_ANSWER_DLG_BUTTON )->SetText(GetTxtFromMgr(9736));
	}
}

void NHelperQuestionOrAnswerDlg::SetQuestionCharInfo( const TCHAR* _pszName, TCHAR* _pszQuestionTitle, SI32 _siQuestionPersonID )
{
	memcpy(m_szQuestionCharName, _pszName,	sizeof(m_szQuestionCharName) );
		
	CEdit* pTitle = m_InterfaceFactory.GetEdit(NHELPER_QUESTION_OR_ANSWER_DLG_EDIT_TITLE);
	pTitle->SetReadOnly(true);
	pTitle->SetText(_pszQuestionTitle);

	m_siQuestionPersonID = _siQuestionPersonID;
}

