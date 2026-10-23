#include "../../Common/CommonHeader.h"
#include "../Client.h"

#include "MsgType-Structure.h"

#include "./NGeneralMeetingLetterDlg.h"

#include "../InterfaceMgr/InterfaceFile.h"
#include "../InterfaceMgr/InterfaceMgr.h"
#include "../InterfaceMgr/Interface/Static.h"
#include "../InterfaceMgr/Interface/Button.h"

#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include <tchar.h>

extern cltCommonLogic* pclClient;

NGeneralMeetingLetterDlg::NGeneralMeetingLetterDlg()
{
	m_pStatic = NULL;
	m_pStatic1 = NULL;//마을명에 의해 글의 뒷부분이 짤리기에 스태틱 분리. [2007.07.05 손성웅]

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;

}

NGeneralMeetingLetterDlg::~NGeneralMeetingLetterDlg()
{
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);

	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pStatic1);//마을명에 의해 글의 뒷부분이 짤리기에 스태틱 분리. [2007.07.05 손성웅]
}

void NGeneralMeetingLetterDlg::InitInfo()
{

	for( int i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
		m_siVillageUnique[ i ] = 0;

	m_curIndex = 0;
}

void NGeneralMeetingLetterDlg::AddInfo( int villageunique )
{
	m_siVillageUnique[ m_curIndex++ ] = villageunique;
}

void NGeneralMeetingLetterDlg::SetText()
{

	int villageunique = m_siVillageUnique[ m_curIndex ];
	if( villageunique )
	{

		char buf[ 1024 ];
		char *pText = GetTxtFromMgr(5839);
		
		StringCchPrintf( buf, 256, pText, pclClient->pclVillageManager->GetName( villageunique ) );
		m_pStatic->SetText( buf );
	}
}

void NGeneralMeetingLetterDlg::Create()
{
	if(!IsCreate())
	{
		cltClient* pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile( TEXT("NInterface/Data/DLG_GeneralMeetingLetter.ddf"));

		file.CreatePopupDialog( this, NGENERALMEETINGLETTER_DLG,  TEXT("dialog_generalmeetingletter"), StaticCallBackDialogNGeneralMeetingLetterDlg);

		NEWCSTATIC(m_pStatic);
		NEWCSTATIC(m_pStatic1);//마을명에 의해 글의 뒷부분이 짤리기에 스태틱 분리. [2007.07.05 손성웅]
		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);

		file.CreateControl( m_pStatic, NGENERALMEETINGLETTER_DIALOG_STATIC1, TEXT("statictext_NONAME1"));
		file.CreateControl( m_pStatic1, NGENERALMEETINGLETTER_DIALOG_STATIC1, TEXT("statictext_NONAME2")); 

		file.CreateControl( m_pButton[0], NGENERALMEETINGLETTER_DIALOG_BUTTON1, TEXT("button_ok")); 
		file.CreateControl( m_pButton[1], NGENERALMEETINGLETTER_DIALOG_BUTTON2, TEXT("button_cancel")); 

		MovePos( 400, 100 );

	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK NGeneralMeetingLetterDlg::StaticCallBackDialogNGeneralMeetingLetterDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NGeneralMeetingLetterDlg *pThis = (NGeneralMeetingLetterDlg*)pControllerMgr;
	pThis->CallBackDialogNGeneralMeetingLetterDlg( nEvent, nControlID, pControl );

}

void CALLBACK NGeneralMeetingLetterDlg::CallBackDialogNGeneralMeetingLetterDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NGENERALMEETINGLETTER_DIALOG_BUTTON1:
		{
			switch( nEvent ) {
			case EVENT_BUTTON_CLICKED:					
				{
					cltGameMsgRequest_GeneralMeetingParticipate  clinfo( m_siVillageUnique[ m_curIndex ], 1 );
					cltMsg clMsg(GAMEMSG_REQUEST_GENERALMEETING_PARTICIPATE,sizeof(clinfo),(BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
				}
				break;
			}
			
		}
		break;

	case NGENERALMEETINGLETTER_DIALOG_BUTTON2:
		{
			switch( nEvent ) {
			case EVENT_BUTTON_CLICKED:					
				{
					cltGameMsgRequest_GeneralMeetingParticipate  clinfo( m_siVillageUnique[ m_curIndex ], 2 );
					cltMsg clMsg(GAMEMSG_REQUEST_GENERALMEETING_PARTICIPATE,sizeof(clinfo),(BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
				}
				break;
			}
		}
		break;
	}
}
/*
void NGeneralMeetingLetterDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
}

void NGeneralMeetingLetterDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
}

bool NGeneralMeetingLetterDlg::IsShow()
{
	return m_bShow;
}

void CNErrorMsgPrint::SetEditText( TCHAR* in_pTmp )
{
	if(m_pEdit_error_print == NULL ) return;
	m_pEdit_error_print->SetText( in_pTmp );
}

void CNErrorMsgPrint::SetTitleCaption( TCHAR* in_pTmp )
{
	if( _tcslen(in_pTmp) >= 260 ) return;
	this->SetTitle( in_pTmp );
}
*/