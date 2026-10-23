#include "../Client/NInterface/NHyungzoDlg/NHyungzoCriminalList.h"

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

CNHyungzoCriminalListDlg::CNHyungzoCriminalListDlg() : CDialogController()
{
	m_pList = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;

	m_siSelectList = -1;
}

CNHyungzoCriminalListDlg::~CNHyungzoCriminalListDlg()
{
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);

	SAFE_DELETE(m_pList);
}

void CNHyungzoCriminalListDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHyungzoDlg/DLG_Hyungzo_CriminalList.ddf"));
	file.CreatePopupDialog( this, NHYUNGZOCRIMINAL_DLG, TEXT("dialog_CriminalList"), StaticCallBackDialogNHyungzoCriminalListDlg );

	NEWCLIST(m_pList);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	
	file.CreateControl( m_pList, NHYUNGZOCRIMINAL_DIALOG_LIST, TEXT("listbox_NONAME1") );

	file.CreateControl( m_pButton[0], NHYUNGZOCRIMINAL_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHYUNGZOCRIMINAL_DIALOG_BUTTON2,TEXT("button_NONAME2") );
	
	if( ConstServiceArea_China == pclClient->siServiceArea )
	{
		TCHAR* pText = GetTxtFromMgr(1556);
		m_pList->SetColumn(0,160, pText);
		pText = GetTxtFromMgr(5547);
		m_pList->SetColumn(1,250,pText);
		m_pList->SetBKColorUse( true );
		m_pList->SetBorder( true );
		m_pList->Refresh();
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1556);
		m_pList->SetColumn(0,100, pText);
		pText = GetTxtFromMgr(5547);
		m_pList->SetColumn(1,100,pText);
		m_pList->SetBKColorUse( true );
		m_pList->SetBorder( true );
		m_pList->Refresh();
	}

	Hide();
}

void CNHyungzoCriminalListDlg::Action()
{

}

void CNHyungzoCriminalListDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHyungzoCriminalListDlg::Show()
{
	CControllerMgr::Show( true );

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_LOOKPRISON);
}

void CALLBACK CNHyungzoCriminalListDlg::StaticCallBackDialogNHyungzoCriminalListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHyungzoCriminalListDlg *pThis = (CNHyungzoCriminalListDlg *)pControllerMgr;
	pThis->CallBackDialogNHyungzoCriminalListDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHyungzoCriminalListDlg::CallBackDialogNHyungzoCriminalListDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;

	case NHYUNGZOCRIMINAL_DIALOG_LIST:
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

	case NHYUNGZOCRIMINAL_DIALOG_BUTTON1:	// 감옥 수감한다
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siSelectList < 0)
						return;

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
						return;

					TCHAR buffer[256] = TEXT("") ;
					HyungzoMember hyungzomember ; 
					hyungzomember.siDate = 0 ;

					//	HWND hwnd = GetDlgItem(hDlg,IDC_EDIT_PRISONER);
					//	GetWindowText(hwnd,hyungzomember.szCharName,MAX_PLAYER_NAME);
					MStrCpy(hyungzomember.szCharName, m_pList->GetText(m_siSelectList, 0), MAX_PLAYER_NAME);
					if ( _tcslen(hyungzomember.szCharName) <= 0 || !_tcscmp(hyungzomember.szCharName,TEXT("")) )
					{
						TCHAR* pTitle = GetTxtFromMgr(2029);
						TCHAR* pText = GetTxtFromMgr(2030);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					//	hwnd = GetDlgItem(hDlg,IDC_EDIT_REASON);
					//	GetWindowText(hwnd,hyungzomember.szPrisonReason,MAX_PRISON_REASON_SIZE);
					MStrCpy(hyungzomember.szPrisonReason, m_pList->GetText(m_siSelectList, 1), MAX_PRISON_REASON_SIZE);
					if ( _tcslen(hyungzomember.szPrisonReason) <= 0 || !_tcscmp(hyungzomember.szPrisonReason,TEXT("")) )
					{
						TCHAR* pTitle = GetTxtFromMgr(2031);
						TCHAR* pText = GetTxtFromMgr(2032);
						StringCchPrintf(buffer, 256, pText,MAX_PRISON_REASON_SIZE/2);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
						break;
					}

					//TCHAR timelimit[16] = TEXT("") ;
					//	hwnd = GetDlgItem(hDlg,IDC_EDIT_LIMIT_PRISON);
					//	GetWindowText(hwnd,timelimit,16);
					//MStrCpy(timelimit, m_pEdit[3]->GetText(), 16);
					hyungzomember.siPrisonSecond = 30;
					if ( hyungzomember.siPrisonSecond <= 0 || hyungzomember.siPrisonSecond > MAX_PRISON_SECOND )
					{
						TCHAR* pTitle = GetTxtFromMgr(2033);
						TCHAR* pText = GetTxtFromMgr(2034);
						StringCchPrintf(buffer, 256, pText,MAX_PRISON_SECOND);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
						break;
					}

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 || 
						pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != 23510 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2035);
						TCHAR* pText = GetTxtFromMgr(2036);
						StringCchPrintf(buffer, 256, pText);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
						break;
					}

					cltGameMsgRequest_SetHyungzoPrison clinfo(&hyungzomember);
					cltMsg clMsg(GAMEMSG_REQUEST_SETHYUNGZOPRISON, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					cltGameMsgRequest_SetCriminal clinfo2(m_pList->GetText(m_siSelectList, 0), m_pList->GetText(m_siSelectList, 1), false);
					cltMsg clMsg2( GAMEMSG_REQUEST_SETCRIMINAL, sizeof( clinfo2 ), (BYTE*)&clinfo2 );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg2);
				}
				break;
			}
		}
		break;

	case NHYUNGZOCRIMINAL_DIALOG_BUTTON2:	// 현행범 풀어주기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siSelectList < 0 )
						return;

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
						return;

					cltGameMsgRequest_SetCriminal clinfo(m_pList->GetText(m_siSelectList, 0), m_pList->GetText(m_siSelectList, 1), false);
					cltMsg clMsg( GAMEMSG_REQUEST_SETCRIMINAL, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	}
}

void CNHyungzoCriminalListDlg::ShowListDlg(HyungzoCriminalList* stHyungzoCriminalList)
{
	Show();

	m_pList->Clear();

	for ( int i = 0 ; i < MAX_HYUNGZO_CRIMINALLIST ; i ++ )
	{
		if ( stHyungzoCriminalList[i].siPersonID <= 0 ) continue ;
		if ( !_tcscmp(stHyungzoCriminalList[i].szAccountID,TEXT("")) ) continue ;
		if ( !_tcscmp(stHyungzoCriminalList[i].szName,TEXT("")) ) continue ;
		if ( !_tcscmp(stHyungzoCriminalList[i].szReason,TEXT("")) ) continue ;

		TCHAR buffer[256] = TEXT("") ;

		stListBoxItemData Data;
		Data.Set(0, stHyungzoCriminalList[i].szName);
		Data.Set(1, stHyungzoCriminalList[i].szReason);

		m_pList->AddItem(&Data);
	}
	m_pList->Refresh();
}

void CNHyungzoCriminalListDlg::SetEnableButton(BOOL flag)
{

}

void CNHyungzoCriminalListDlg::EnableButton(CButton* pButton, BOOL Flag)
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

