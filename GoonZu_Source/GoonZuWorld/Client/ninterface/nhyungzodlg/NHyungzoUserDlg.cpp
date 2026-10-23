#include "../Client/NInterface/NHyungzoDlg/NHyungzoUserDlg.h"

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


extern cltCommonLogic* pclClient;

CNHyungzoUserDlg::CNHyungzoUserDlg() : CDialogController()
{
	SI32 i;
	for(i=0;i<9;++i)
        m_pStatic[i] = NULL;
	for(i=0;i<4;++i)
		m_pEdit[i] = NULL;
	for(i=0;i<3;++i)
		m_pButton[i] = NULL;

	m_pOutLine = NULL;

}

CNHyungzoUserDlg::~CNHyungzoUserDlg()
{
	SI32 i;
	for(i=0;i<9;++i)
		SAFE_DELETE(m_pStatic[i]);
	for(i=0;i<4;++i)
		SAFE_DELETE(m_pEdit[i]);
	for(i=0;i<3;++i)
		SAFE_DELETE(m_pButton[i]);

	SAFE_DELETE(m_pOutLine);
}

void CNHyungzoUserDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHyungzoDlg/DLG_Hyungzo_User.ddf"));
	file.CreateChildDialog( this, NHYUNGZOUSER_DLG, TEXT("dialog_Hyungzo_User"), StaticCallBackDialogNHyungzoUserDlg, pParent );
	
	NEWCOUTLINE(m_pOutLine);

	SI32 i;
	for(i=0;i<9;++i)
		NEWCSTATIC(m_pStatic[i]);
	for(i=0;i<4;++i)
		NEWCEDIT(m_pEdit[i]);
	for(i=0;i<3;++i)
		NEWCBUTTON(m_pButton[i]);

	TCHAR buffer[20] = TEXT("");
	for(i=0;i<9;++i)
	{
		StringCchPrintf(buffer, 20, TEXT("statictext_NONAME%d"), i+1);
		file.CreateControl( m_pStatic[i], NHYUNGZOUSER_DIALOG_STATIC, buffer );
	}

	for(i=0;i<4;++i)
	{
		StringCchPrintf(buffer, 20, TEXT("editbox_NONAME%d"), i+1);
		file.CreateControl( m_pEdit[i], NHYUNGZOUSER_DIALOG_EDIT1 + i, buffer );
	}

	for(i=0;i<3;++i)
	{
		StringCchPrintf(buffer, 20, TEXT("button_NONAME%d"), i+1);
		file.CreateControl( m_pButton[i], NHYUNGZOUSER_DIALOG_BUTTON1 + i, buffer );
	}
	
	file.CreateControl(m_pOutLine,NHYUNGZOUSER_DIALOG_OUTLINE, "outline_NONAME1");

	// kkm- 중국에서는 일단 매크로 신고 막기
	if( ConstServiceArea_China == pclClient->siServiceArea )
		m_pButton[1]->Enable(false);
	// -kkm

	if ( ConstServiceArea_Korea == pclClient->siServiceArea )
	{
		// 한국은 '사용자신고란?'버튼 안보이기
		m_pButton[0]->Show( false );
	}
	Hide();
}

void CNHyungzoUserDlg::Action()
{

}

void CNHyungzoUserDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHyungzoUserDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHyungzoUserDlg::StaticCallBackDialogNHyungzoUserDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHyungzoUserDlg *pThis = (CNHyungzoUserDlg *)pControllerMgr;
	pThis->CallBackDialogNHyungzoUserDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHyungzoUserDlg::CallBackDialogNHyungzoUserDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NHYUNGZOUSER_DIALOG_BUTTON1:	// 사용자 신고란?
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{					
					//if ( g_SoundOnOffDlg.m_bFullScreenMode )
					//{
					pclclient->ShowHelpWindow(TEXT("accusation"));
						//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
						//{
						//	pclclient->CreateInterface( NHINT_DLG );
						//}
						//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("accusation"));

					//}
					//else
					//	pclclient->m_pHintDlg->Show(TEXT("accusation"));
				}
				break;
			}
		}
		break;

	case NHYUNGZOUSER_DIALOG_BUTTON2:	// 의뢰하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() < REPORT_MINLEVEL )
					{
						TCHAR* pTitle = GetTxtFromMgr(2037);
						TCHAR* pText = GetTxtFromMgr(2038);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					TCHAR name[MAX_PLAYER_NAME] = TEXT("") ;
					TCHAR buffer[256] = TEXT("") ;
					TCHAR buffer2[256] = TEXT("") ;

				//	GetWindowText(GetDlgItem(hDlg,IDC_EDIT_SUSPECT),name,MAX_PLAYER_NAME);
					MStrCpy(name, m_pEdit[0]->GetText(), MAX_PLAYER_NAME);
					if ( _tcslen(name) <= 0 || !_tcscmp(name,TEXT("")) )
					{
						TCHAR* pTitle = GetTxtFromMgr(567);
						TCHAR* pText = GetTxtFromMgr(2039);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					TCHAR* pText = GetTxtFromMgr(2040);
					StringCchPrintf(buffer, 256, pText, name);
					pText = GetTxtFromMgr(2041);
					TCHAR min[128]= TEXT(" ") ;
					StringCchPrintf(min, 128, TEXT("%d"),NOSUSPICTION_PRISON_TIME);
					Nsprintf(buffer2,pText,TEXT("user"),  name  ,TEXT("min"), min   ,NULL);
					//sprintf(buffer2, pText,
					//	name,NOSUSPICTION_PRISON_TIME);

					stMsgBox MsgBox;
					// 현재 dialog가 탭 창 같은 Child Dialog일때
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, buffer, buffer2, MSGBOX_TYPE_YESNO, 0 );
					BYTE TempBuffer[ 128 ];
					MStrCpy((TCHAR*)TempBuffer, name, MAX_PLAYER_NAME);
					pclclient->SetMsgBox( &MsgBox, TempBuffer, MAX_PLAYER_NAME );

				//	if (  MessageBox(NULL, buffer2,
				//	buffer, MB_ICONQUESTION | MB_YESNO) == IDNO )
				//		break;

				//	cltGameMsgRequest_AutoHunting clinfo(name);
				//	cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING, sizeof(clinfo), (BYTE*)&clinfo);
				//	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				//	m_pEdit[0]->SetText(TEXT(""));
				}
				break;
			}
		}
		break;

	case NHYUNGZOUSER_DIALOG_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256] = TEXT("") ;
					HyungzoMember hyungzomember ;
					hyungzomember.siDate = 0 ;

				//	HWND hwnd = GetDlgItem(hDlg,IDC_EDIT_PRISONER);
				//	GetWindowText(hwnd,hyungzomember.szCharName,MAX_PLAYER_NAME);
					MStrCpy(hyungzomember.szCharName, m_pEdit[1]->GetText(), MAX_PLAYER_NAME);
					if ( _tcslen(hyungzomember.szCharName) <= 0 || !_tcscmp(hyungzomember.szCharName,TEXT("")) )
					{
						TCHAR* pTitle = GetTxtFromMgr(2029);
						TCHAR* pText = GetTxtFromMgr(2030);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

				//	hwnd = GetDlgItem(hDlg,IDC_EDIT_REASON);
				//	GetWindowText(hwnd,hyungzomember.szPrisonReason,MAX_PRISON_REASON_SIZE);
					MStrCpy(hyungzomember.szPrisonReason, m_pEdit[2]->GetText(), MAX_PRISON_REASON_SIZE);
					if ( _tcslen(hyungzomember.szPrisonReason) <= 0 || !_tcscmp(hyungzomember.szPrisonReason,TEXT("")) )
					{
						TCHAR* pTitle = GetTxtFromMgr(2031);
						TCHAR* pText = GetTxtFromMgr(2032);
						StringCchPrintf(buffer, 256, pText,MAX_PRISON_REASON_SIZE/2);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
						break;
					}

					TCHAR timelimit[16] = TEXT("") ;
				//	hwnd = GetDlgItem(hDlg,IDC_EDIT_LIMIT_PRISON);
				//	GetWindowText(hwnd,timelimit,16);
					MStrCpy(timelimit, m_pEdit[3]->GetText(), 16);
					hyungzomember.siPrisonSecond = _tstoi(timelimit);
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
				}
				break;
			}
		}
		break;
	}
}

void CNHyungzoUserDlg::SetEnableButton(BOOL flag)
{
	if(flag)
	{
		m_pEdit[0]->SetReadOnly(FALSE);
		m_pEdit[1]->SetReadOnly(FALSE);
		m_pEdit[2]->SetReadOnly(FALSE);
		m_pEdit[3]->SetReadOnly(FALSE);
	}
	else
	{
		EnableButton(m_pButton[2], FALSE);
	}
}

void CNHyungzoUserDlg::EnableButton(CButton* pButton, BOOL Flag)
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

