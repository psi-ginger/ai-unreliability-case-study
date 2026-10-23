#include "../../ninterface/NGetPresentDlg/NThanksPresentDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client.h"

extern cltCommonLogic* pclClient;

CNThanksPresentDlg::CNThanksPresentDlg()
{

	m_siServerUnique = 0;
	m_szPlayerName[0] = '\0';
	m_szPlayerAccount[0] = '\0';
	
	m_bIsForMsgSend = TRUE;

	m_siCurLetter = 0;
	m_siLetterNum = 0;

}

CNThanksPresentDlg::~CNThanksPresentDlg()
{
}

void CNThanksPresentDlg::Create()
{
	if ( !IsCreate() )
	{
 		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NGetPresentDlg/DLG_NThanksPresent.ddf"));
		file.CreatePopupDialog( this, NTHANKSPRESENT_DLG, TEXT("dialog_NThanksPresent"), StatiCNThanksPresentDlg);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,NTHANKSPRESENT_EDIT_MSG,this), 	NTHANKSPRESENT_EDIT_MSG, "editbox_NONAME1" );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NTHANKSPRESENT_BUTTON_SEND, this ), NTHANKSPRESENT_BUTTON_SEND,   "button_Send"); 	
		
 		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NTHANKSPRESENT_IMAGESTATIC1, this ), NTHANKSPRESENT_IMAGESTATIC1,   "imagestatic_NONAME1"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NTHANKSPRESENT_PAGE, this ), NTHANKSPRESENT_PAGE,   "statictext_page"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NTHANKSPRESENT_BUTTON_PRE, this ), NTHANKSPRESENT_BUTTON_PRE,   "button_pre"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NTHANKSPRESENT_BUTTON_NEXT, this ), NTHANKSPRESENT_BUTTON_NEXT,   "button_next"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NTHANKSPRESENT_BYTE, this ), NTHANKSPRESENT_BYTE,   "statictext_byte"); 
		
	//	m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->SetText(TEXT("TEST"));
		
		m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->SetAutoScroll(true);
				
		m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->SetMaxEditTextLength(200);

		//SetActionTimer(100);
		SetTimerOnOff(true);
	}
}


void CALLBACK CNThanksPresentDlg::StatiCNThanksPresentDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNThanksPresentDlg *pThis = (CNThanksPresentDlg*) pControllerMgr;
	pThis->NThanksPresentDlgdProc( nEvent, nControlID, pControl );
}



void CALLBACK CNThanksPresentDlg::NThanksPresentDlgdProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID)
	{

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NTHANKSPRESENT_BUTTON_PRE:
		{
           PreButtonClicked();
		}
		break;
	case NTHANKSPRESENT_BUTTON_NEXT:
		{
			NextButtonClicked();
		}
		break;
	case NTHANKSPRESENT_BUTTON_SEND:
		{				
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					// 감사 메시지 전송 혹은 감사 메시지 확인 용 버튼
					
					// 현재 감사 메시지 전송용으로 쓰이는 중이라면
					if(m_bIsForMsgSend)
					{
						if( m_siServerUnique <= 0 
							|| m_siServerUnique > MAX_SERVER_NUMBER
							|| m_szPlayerAccount[0] == '\0'
							|| m_szPlayerName[0] == '\0'
							)
						{
							break;
						}
				
						// 메시지를 전송해서 서버에서 어카운트 레터로 만들어 준다.
						
						cltGameMsgRequest_Present_Asnwer clPresentAnswer(m_siServerUnique,m_szPlayerName,m_szPlayerAccount
										, m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->GetText() );
						cltMsg clMsg(GAMEMSG_REQUEST_PRESENT_ANSWER,sizeof(cltGameMsgRequest_Present_Asnwer),(BYTE*)&clPresentAnswer);
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);                        						
					}
					else
					{
						// 그냥 닫는다.

					}                              
					
					DeleteDialog();

				}
				break;
			}
		}
		break;

	}
	return ;
}


void CNThanksPresentDlg::SetRecvLetterData( SI32 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment )
{

	m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->SetReadOnly(true);

	m_InterfaceFactory.GetButton(NTHANKSPRESENT_BUTTON_PRE)->Show(true);
	m_InterfaceFactory.GetButton(NTHANKSPRESENT_BUTTON_NEXT)->Show(true);
	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_PAGE)->Show(true);
	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_BYTE)->Show(false);


	stAnswerData clAnswerData(FromServerUnique,FromPlayerName,Comment);

	m_AnswerList.push_front( clAnswerData );
	
	m_siCurLetter = 1;

	SetCurAnswerData();  


	
}
void CNThanksPresentDlg::SetSendLetterData( SI32 ToServerUnique, TCHAR * ToPlayerName, TCHAR * ToPlayerAccount)
{
// 서버유니크와 어카운트 는 어카운트레터 전송을 위해 변수에 기억해 둔다.
	

	m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->SetReadOnly(false);
	m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->Clear();
    m_InterfaceFactory.GetButton(NTHANKSPRESENT_BUTTON_PRE)->Show(false);
	m_InterfaceFactory.GetButton(NTHANKSPRESENT_BUTTON_NEXT)->Show(false);
 	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_PAGE)->Show(false);
	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_BYTE)->Show(true);

	m_siServerUnique = ToServerUnique;
	StringCchCopy(m_szPlayerAccount,MAX_PLAYER_NAME,ToPlayerAccount);

// 플레이어 이름을 타이틀에 넣는다.
	StringCchCopy(m_szPlayerName,MAX_PLAYER_NAME, ToPlayerName);

	TCHAR szBuffer[256];
	TCHAR * pText = GetTxtFromMgr(6121);
    
	StringCchPrintf(szBuffer,256,pText,m_szPlayerName);
	SetTitle(szBuffer);    

	
	pText = GetTxtFromMgr(6103);


	m_InterfaceFactory.GetButton(NTHANKSPRESENT_BUTTON_SEND)->SetText(pText);

	m_bIsForMsgSend = TRUE;
}


void CNThanksPresentDlg::SetCurAnswerData()
{
	cltClient * pclclient = (cltClient*) pclClient;

	list<stAnswerData>::size_type	AnswerDataNum;
	AnswerDataNum = m_AnswerList.size();

	if( AnswerDataNum <= 0 ) return;

	if( m_siCurLetter <= 0 || m_siCurLetter > (SI32)AnswerDataNum  )
	{
		m_siCurLetter = 1;
	}

	m_AnswerListItr = m_AnswerList.begin();

	for( SI32 i = 0; i < m_siCurLetter-1; i++ )
	{
		if( m_AnswerListItr == m_AnswerList.end() )
		{
			break;
		}	
		m_AnswerListItr++;        
	}

	stAnswerData * pAnswerData;

	pAnswerData = &(*m_AnswerListItr);	// 값을 가져온다.

	TCHAR szBuffer[256] = {'\0',};



	StringCchPrintf( szBuffer,256,TEXT("%d/%d"), m_siCurLetter, AnswerDataNum );

	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_PAGE)->SetText(szBuffer,DT_CENTER);

	m_siServerUnique = pAnswerData->siFromServerUnique;

	StringCchCopy(m_szPlayerName,MAX_PLAYER_NAME,pAnswerData->szFromPlayerName);

	m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->SetText(pAnswerData->szComment);

	TCHAR * pText = GetTxtFromMgr(6122);

	StringCchPrintf(szBuffer, 256,pText,m_szPlayerName,pclclient->GetServerNameFromUnique(m_siServerUnique));
	SetTitle(szBuffer);

	pText = GetTxtFromMgr(3889);

	m_InterfaceFactory.GetButton(NTHANKSPRESENT_BUTTON_SEND)->SetText(pText);

	m_bIsForMsgSend = FALSE;
	

}
void CNThanksPresentDlg::PreButtonClicked()
{
	if( m_siCurLetter == 1 ) return;

	m_siCurLetter--;

	if(m_siCurLetter <= 0)
		m_siCurLetter = 1;

	SetCurAnswerData();
}
void CNThanksPresentDlg::NextButtonClicked()
{
	list<stAnswerData>::size_type	AnswerDataNum;
	AnswerDataNum = m_AnswerList.size();

	if( (SI32)AnswerDataNum == m_siCurLetter) return;

	m_siCurLetter++;

	if(m_siCurLetter > (SI32)AnswerDataNum)
		m_siCurLetter = AnswerDataNum;

	SetCurAnswerData();
}


void CNThanksPresentDlg::Action()
{

	TCHAR buffer[128];
	TCHAR msg[ 256 ];
	SI32  maxSize = 200;

	MStrCpy( msg, m_InterfaceFactory.GetEdit(NTHANKSPRESENT_EDIT_MSG)->GetText(), 256 );

	TCHAR* pText = GetTxtFromMgr(6123);
	int len = _tcslen( msg );

	StringCchPrintf( buffer, 128, pText, len , maxSize );

	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_BYTE)->SetText(buffer);

	if(len > maxSize)
	{
		m_InterfaceFactory.GetStatic(NTHANKSPRESENT_BYTE)->SetFontColor(RGB(225,50,50));
	}
	else
	{
		m_InterfaceFactory.GetStatic(NTHANKSPRESENT_BYTE)->SetFontColor(RGB(0,0,0) );
	}

	m_InterfaceFactory.GetStatic(NTHANKSPRESENT_BYTE)->Refresh();

}

	