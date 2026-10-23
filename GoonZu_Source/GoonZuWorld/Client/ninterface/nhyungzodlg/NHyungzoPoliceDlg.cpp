#include "../Client/NInterface/NHyungzoDlg/NHyungzoPoliceDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../common/HintDlg/HintDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../common/PersonInfo.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../Client/Music/Music.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../common/Char/CharCommon/Char-Common.h"
//#include "../common/AutoHunting/AutoHunting.h"
#include "NInterface/NAutoHunting/NAutoHunting.h"

#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../Server/Minister/Minister-Hyungzo.h"


extern cltCommonLogic* pclClient;

CNHyungzoPoliceDlg::CNHyungzoPoliceDlg() : CDialogController()
{
	m_pEdit[0] = NULL;
	m_pEdit[1] = NULL;
	m_pEdit[2] = NULL;

	m_pList = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[3] = NULL;
	m_pButton[4] = NULL;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pStatic[3] = NULL;
	m_pStatic[4] = NULL;

	m_pOutLine = NULL;

	m_siSelectList = -1;
}

CNHyungzoPoliceDlg::~CNHyungzoPoliceDlg()
{
	SAFE_DELETE(m_pOutLine);

	SAFE_DELETE(m_pStatic[4]);
	SAFE_DELETE(m_pStatic[3]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[0]);

	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);

	SAFE_DELETE(m_pList);

	SAFE_DELETE(m_pEdit[2]);
	SAFE_DELETE(m_pEdit[1]);
	SAFE_DELETE(m_pEdit[0]);

}

void CNHyungzoPoliceDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHyungzoDlg/DLG_Hyungzo_Police.ddf"));
	file.CreateChildDialog( this, NHYUNGZOPOLICE_DLG, TEXT("dialog_Hyungzo_Police"), StaticCallBackDialogNHyungzoPoliceDlg, pParent );

	NEWCEDIT(m_pEdit[0]);
	NEWCEDIT(m_pEdit[1]);
	NEWCEDIT(m_pEdit[2]);

	NEWCLIST(m_pList);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	NEWCBUTTON(m_pButton[2]);
	NEWCBUTTON(m_pButton[3]);

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);
	NEWCSTATIC(m_pStatic[3]);
	NEWCSTATIC(m_pStatic[4]);

	NEWCOUTLINE(m_pOutLine);

	file.CreateControl( m_pEdit[0], NHYUNGZOPOLICE_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pEdit[1], NHYUNGZOPOLICE_DIALOG_EDIT2, TEXT("editbox_NONAME2") );
	file.CreateControl( m_pEdit[2], NHYUNGZOPOLICE_DIALOG_EDIT3, TEXT("editbox_NONAME3") );

	file.CreateControl( m_pList, NHYUNGZOPOLICE_DIALOG_LIST, TEXT("listbox_NONAME1") );

	file.CreateControl( m_pButton[0], NHYUNGZOPOLICE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHYUNGZOPOLICE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	file.CreateControl( m_pButton[2], NHYUNGZOPOLICE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
	file.CreateControl( m_pButton[3], NHYUNGZOPOLICE_DIALOG_BUTTON4, TEXT("button_NONAME4") );

	file.CreateControl( m_pStatic[0], NHYUNGZOPOLICE_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHYUNGZOPOLICE_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHYUNGZOPOLICE_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NHYUNGZOPOLICE_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
	file.CreateControl( m_pStatic[4], NHYUNGZOPOLICE_DIALOG_STATIC5, TEXT("statictext_NONAME5") );

	file.CreateControl( m_pOutLine, NHYUNGZOPOLICE_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	TCHAR* pText = GetTxtFromMgr(1556);
	m_pList->SetColumn(0,100, pText);
	pText = GetTxtFromMgr(1506);
	m_pList->SetColumn(1,80,pText);
	m_pList->SetBKColorUse( true );
	m_pList->SetBorder( true );
	m_pList->Refresh();

//	if(pclClient->pclCM->CR[1]


	Hide();
}

void CNHyungzoPoliceDlg::Action()
{

}

void CNHyungzoPoliceDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHyungzoPoliceDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHyungzoPoliceDlg::StaticCallBackDialogNHyungzoPoliceDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHyungzoPoliceDlg *pThis = (CNHyungzoPoliceDlg *)pControllerMgr;
	pThis->CallBackDialogNHyungzoPoliceDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHyungzoPoliceDlg::CallBackDialogNHyungzoPoliceDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NHYUNGZOPOLICE_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 tempSelect = m_pList->GetSelectedIndex();

					if( tempSelect < 0 )
						return;

					m_siSelectList = tempSelect;
				}
				break;
			}
		}
		break;

	case NHYUNGZOPOLICE_DIALOG_BUTTON1:	// 포교 설정한다
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 NameLength = _tcslen( m_pEdit[0]->GetText() );
					if( NameLength <= 0)
						return;

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
						return;
					
					cltGameMsgRequest_SetPolice clinfo(m_pEdit[0]->GetText(),true);
					cltMsg clMsg( GAMEMSG_REQUEST_SETPOLICE, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHYUNGZOPOLICE_DIALOG_BUTTON2:	// 포교 삭제한다
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siSelectList < 0 )
						return;

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
						return;

					cltGameMsgRequest_SetPolice clinfo(m_pList->GetText(m_siSelectList, 0), false);
					cltMsg clMsg( GAMEMSG_REQUEST_SETPOLICE, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHYUNGZOPOLICE_DIALOG_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETCRIMINALLIST,0,NULL);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHYUNGZOPOLICE_DIALOG_BUTTON4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 NameLength = _tcslen( m_pEdit[1]->GetText() );
					if( NameLength <= 0)
						return;

					SI32 ReasonLength = _tcslen( m_pEdit[2]->GetText() );
					if( ReasonLength <= 0)
						return;

					cltGameMsgRequest_SetCriminal clinfo(m_pEdit[1]->GetText(), m_pEdit[2]->GetText(), true);
					cltMsg clMsg( GAMEMSG_REQUEST_SETCRIMINAL, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					
				}
				break;
			}
		}
		break;
	}
}

void CNHyungzoPoliceDlg::SetEnableButton(BOOL flag)
{
}

void CNHyungzoPoliceDlg::EnableButton(CButton* pButton, BOOL Flag)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
		pButton->Enable(false);
	}
}

void CNHyungzoPoliceDlg::ShowListDlg(HyungzoPoliceList* stHyungzoPoliceList)
{
	m_pList->Clear();

	for ( int i = 0 ; i < MAX_HYUNGZO_POLICELIST ; i ++ )
	{
		if ( stHyungzoPoliceList[i].siLevel <= 0 ) continue ;
		if ( stHyungzoPoliceList[i].siPersonID <= 0 ) continue ;
		if ( !_tcscmp(stHyungzoPoliceList[i].szAccountID,TEXT("")) ) continue ;
		if ( !_tcscmp(stHyungzoPoliceList[i].szName,TEXT("")) ) continue ;

		TCHAR buffer[256] = TEXT("") ;

		stListBoxItemData Data;
		Data.Set(0, stHyungzoPoliceList[i].szName);
		StringCchPrintf(buffer, 256, TEXT("%d"),stHyungzoPoliceList[i].siLevel ) ;
		Data.Set(1, buffer);
		
		m_pList->AddItem(&Data);
	}
	m_pList->Refresh();
}
