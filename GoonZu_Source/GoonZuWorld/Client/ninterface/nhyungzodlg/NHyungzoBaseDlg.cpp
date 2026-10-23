#include "../Client/NInterface/NHyungzoDlg/NHyungzoBaseDlg.h"
#include "../Client/NInterface/NHyungzoDlg/NHyungzoAnnouncementDlg.h"
#include "../Client/NInterface/NHyungzoDlg/NHyungzoMainDlg.h"
#include "../Client/NInterface/NHyungzoDlg/NHyungzoUserDlg.h"
#include "../Client/NInterface/NHyungzoDlg/NHyungzoPoliceDlg.h"
#include "../Client/NInterface/NHyungzoDlg/NHyungzoCriminalList.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../common/PersonInfo.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../Client/Music/Music.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/Msg/MsgType-Web.h"
#include "../common/Char/CharCommon/Char-Common.h"

extern cltCommonLogic* pclClient;

CNHyungzoBaseDlg::CNHyungzoBaseDlg() : CDialogController()
{
	m_bSet = FALSE ;
	m_bSoundCheck = FALSE;

	StringCchCopy(m_szHyungzoName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szHyungzoProfile, 256, TEXT(""));
	StringCchCopy(m_szHyungzoID, MAX_PLAYER_NAME, TEXT(""));


	m_pStatic = NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;
	m_pButton[4] = NULL;
	m_pButton[5] = NULL;
	m_pButton[6] = NULL;
	m_pButton[7] = NULL;
	m_pEdit = NULL;

	m_pNHyungzoMainDlg = NULL;
	m_pNHyungzoUserDlg = NULL;
	m_pNHyungzoAnnouncement = NULL;
	m_pNHyungzoPoliceDlg = NULL;
	m_pNHyungzoCriminalListDlg = NULL;

	m_siActionType = 0;
}

CNHyungzoBaseDlg::~CNHyungzoBaseDlg()
{
	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[4]);
	SAFE_DELETE(m_pButton[5]);
	SAFE_DELETE(m_pButton[6]);
	SAFE_DELETE(m_pButton[7]);
	SAFE_DELETE(m_pButton[8]);

	SAFE_DELETE(m_pEdit);

	SAFE_DELETE(m_pNHyungzoMainDlg);
	SAFE_DELETE(m_pNHyungzoUserDlg);
	SAFE_DELETE(m_pNHyungzoAnnouncement);
	SAFE_DELETE(m_pNHyungzoPoliceDlg);
	SAFE_DELETE(m_pNHyungzoCriminalListDlg);
}

void CNHyungzoBaseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NHyungzoDlg/DLG_Hyungzo_Base.ddf"));
		file.CreatePopupDialog( this, NHYUNGZOBASE_DLG, TEXT("dialog_Hyungzo_Base"), StaticCallBackDialogNHyungzoBaseDlg );

		NEWCSTATIC(m_pStatic);
		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		NEWCBUTTON(m_pButton[4]);
		NEWCBUTTON(m_pButton[5]);
		NEWCBUTTON(m_pButton[6]);
		NEWCBUTTON(m_pButton[7]);
		NEWCBUTTON(m_pButton[8]);
		
		NEWCEDIT(m_pEdit);

		file.CreateControl( m_pStatic, NHYUNGZOBASE_DIALOG_STATIC, TEXT("statictext_NONAME1") );

		file.CreateControl( m_pButton[0], NHYUNGZOBASE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NHYUNGZOBASE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NHYUNGZOBASE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NHYUNGZOBASE_DIALOG_BUTTON4, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NHYUNGZOBASE_DIALOG_BUTTON5, TEXT("button_NONAME5") );
		file.CreateControl( m_pButton[5], NHYUNGZOBASE_DIALOG_BUTTON6, TEXT("button_NONAME6") );
		file.CreateControl( m_pButton[6], NHYUNGZOBASE_DIALOG_BUTTON7, TEXT("button_NONAME7") );
		file.CreateControl( m_pButton[7], NHYUNGZOBASE_DIALOG_BUTTON8, TEXT("button_NONAME8") );
		file.CreateControl( m_pButton[8], NHYUNGZOBASE_DIALOG_BUTTON9, TEXT("button_NONAME19") );
		
		file.CreateControl( m_pEdit, NHYUNGZOBASE_DIALOG_EDIT, TEXT("editbox_NONAME1") );

		SetButtonTextColor(4);

		m_pNHyungzoMainDlg = new CNHyungzoMainDlg();
		m_pNHyungzoMainDlg->Create( this );
		m_pNHyungzoMainDlg->Show();

		m_pNHyungzoUserDlg = new CNHyungzoUserDlg();
		m_pNHyungzoUserDlg->Create( this );

		m_pNHyungzoAnnouncement = new CNHyungzoAnnouncementDlg();
		m_pNHyungzoAnnouncement->Create( this );

		m_pNHyungzoPoliceDlg = new CNHyungzoPoliceDlg();
		m_pNHyungzoPoliceDlg->Create( this );

		m_pNHyungzoCriminalListDlg = new CNHyungzoCriminalListDlg();
		m_pNHyungzoCriminalListDlg->Create();

		cltClient *pclclient = (cltClient*)pclClient;

		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
		{
			EnableButton(m_pButton[0], FALSE);
			EnableButton(m_pButton[1], FALSE);

			m_pNHyungzoMainDlg->SetEnableButton(TRUE);
			m_pNHyungzoUserDlg->SetEnableButton(TRUE);

			SetTimerOnOff( true );
			SetActionTimer( 500 );
		}
		else
		{
			m_pEdit->SetReadOnly(false);

			EnableButton(m_pButton[2], FALSE);
			
			m_pNHyungzoMainDlg->SetEnableButton(FALSE);
			m_pNHyungzoUserDlg->SetEnableButton(FALSE);

			//SetTimerOnOff( true );
			//SetActionTimer( 60 );
		}

		// 형조판서 책상 열기 퀘스트중인가 ?
		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_LOOKPANSEO);

		cltMsg clMsg(GAMEMSG_REQUEST_GETHYUNGZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pButton[0]->Show(false);
		}

		Show( true );
	}
	else
	{
		DeleteDialog();
	}
}

void CNHyungzoBaseDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if( m_siActionType == 3 )
		return;

	if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
	{

		TCHAR buffer[256] = TEXT("") ;
	//	GetDlgItemText(m_hDlg,IDC_EDIT_HYUNGZO,buffer,256);
		MStrCpy(buffer, m_pEdit->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2018);
		StringCchPrintf(buffer, 256, pText,m_szHyungzoName,limit);
	//	SetWindowText(m_hGroup,buffer);
		m_pStatic->SetText(buffer, DT_LEFT | DT_VCENTER);
	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETHYUNGZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

void CALLBACK CNHyungzoBaseDlg::StaticCallBackDialogNHyungzoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHyungzoBaseDlg *pThis = (CNHyungzoBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNHyungzoBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHyungzoBaseDlg::CallBackDialogNHyungzoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON1:	// 꼼지
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szHyungzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					
					/*
					cltGameMsgRequest_Userid clUserid( m_szHyungzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
					*/
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON2:	// 귓말
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{	
					if ( _tcslen(m_szHyungzoName) > 0 && m_szHyungzoName != NULL )
						pclclient->RequestSecretChatByCharName(m_szHyungzoName);
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON3:	// 설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(m_pNHyungzoMainDlg->HyungzoMainCheck() == FALSE)
						return;

				//	GetDlgItemText(m_hDlg,IDC_EDIT_HYUNGZO,m_szHyungzoProfile,256);
					MStrCpy(m_szHyungzoProfile, m_pEdit->GetText(), 256);
					SI32 limit = _tcslen(m_szHyungzoProfile);
					if ( limit > 0 && limit < 256 )
					{
						cltGameMsgRequest_SetHyungzoInfo clinfo(m_szHyungzoProfile, m_pNHyungzoMainDlg->GetStealNum(),
							m_pNHyungzoMainDlg->GetSuspectTime(), m_pNHyungzoMainDlg->GetPrisonTime(), m_pNHyungzoMainDlg->GetPrisonerPenalty());
						cltMsg clMsg(GAMEMSG_REQUEST_SETHYUNGZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						m_bSet = TRUE ;
					}
					else
					{
						TCHAR* pTitle = GetTxtFromMgr(2019);
						TCHAR* pText = GetTxtFromMgr(2024);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON4:	// 닫기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siActionType= 1;

					SetButtonTextColor(4);
				
					m_pNHyungzoMainDlg->Show();
					m_pNHyungzoUserDlg->Hide();
					m_pNHyungzoAnnouncement->Hide();
					m_pNHyungzoPoliceDlg->Hide();

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
					{
						EnableButton(m_pButton[2], TRUE);
					}
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON6:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siActionType = 2;

					SetButtonTextColor(5);

					m_pNHyungzoMainDlg->Hide();
					m_pNHyungzoUserDlg->Show();
					m_pNHyungzoAnnouncement->Hide();
					m_pNHyungzoPoliceDlg->Hide();

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
					{
						EnableButton(m_pButton[2], FALSE);
					}
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON7:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siActionType = 3;

					SetButtonTextColor(6);

					m_pNHyungzoMainDlg->Hide();
					m_pNHyungzoUserDlg->Hide();
					m_pNHyungzoAnnouncement->Show();
					m_pNHyungzoPoliceDlg->Hide();

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
					{
						EnableButton(m_pButton[2], FALSE);
					}
				}
				break;
			}	
		}
		break;

	case NHYUNGZOBASE_DIALOG_BUTTON8:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_POLICESETCLICK );

					m_siActionType =4;

					SetButtonTextColor(7);

					m_pNHyungzoMainDlg->Hide();
					m_pNHyungzoUserDlg->Hide();
					m_pNHyungzoAnnouncement->Hide();
					m_pNHyungzoPoliceDlg->Show();

					cltMsg clMsg(GAMEMSG_REQUEST_GETPOLICELIST, 0, NULL);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
					{
						EnableButton(m_pButton[2], FALSE);
					}

				}
				break;
			}	
		}
		break;
	case NHYUNGZOBASE_DIALOG_BUTTON9:
		{
			if( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->ShowHelpWindow(TEXT("NULL"));
			}
		}
		break;
	}
}

void CNHyungzoBaseDlg::SetHyungzo(CMinisterBase *pMini,SI16 stealnum,SI16 suspecttime,SI16 prisontime,
					   SI16 prisonerpenalty, HyungzoMember *pHyungzo)
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( m_bSet )
	{
		m_bSet = FALSE ;

		TCHAR* pTitle = GetTxtFromMgr(2045);
		TCHAR* pText = GetTxtFromMgr(2046);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return ;
	}

	if(m_bSoundCheck == FALSE)
	{
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_HYUNGZO")),0,0);
		m_bSoundCheck = TRUE;
	}

	StringCchCopy(m_szHyungzoID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szHyungzoName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szHyungzoProfile, 256, pMini->m_szGreeting);

	TCHAR buffer[256] = TEXT("") ;

	TCHAR* pText = GetTxtFromMgr(2047);
	StringCchPrintf(buffer, 256, pText,m_szHyungzoName);
//	SetWindowText( m_hDlg ,buffer);
	SetTitle(buffer);
//	SetDlgItemText( m_hDlg,IDC_EDIT_HYUNGZO, m_szHyungzoProfile );
	m_pEdit->SetText(m_szHyungzoProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
//	SetWindowText(m_hGroup,buffer);
	m_pStatic->SetText(buffer, DT_LEFT | DT_VCENTER);

	m_pNHyungzoMainDlg->SetHyungzoMain(stealnum, suspecttime, prisontime, prisonerpenalty);
	m_pNHyungzoAnnouncement->SetHyungzoAnnouncement(pHyungzo);
}

void CNHyungzoBaseDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNHyungzoBaseDlg::SetButtonTextColor(SI32 Index)
{
	for( SI16 i = 4; i < 8; ++i )
	{
		if ( i == Index )
		{
			m_pButton[ i ]->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
			m_pButton[ i ]->Refresh();
		}
		else
		{
			m_pButton[ i ]->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
			m_pButton[ i ]->Refresh();
		}
	}
}

void CNHyungzoBaseDlg::ShowListDlg(HyungzoPoliceList stHyungzoPoliceList[])
{
	m_pNHyungzoPoliceDlg->ShowListDlg(stHyungzoPoliceList);
}
/*
void CNHyungzoBaseDlg::ShowListDlg2()
{
	m_pNHyungzoCriminalListDlg->Show();
}*/

void CNHyungzoBaseDlg::ShowListDlg(HyungzoCriminalList stHyungzoCriminalList[])
{
	m_pNHyungzoCriminalListDlg->ShowListDlg(stHyungzoCriminalList);
}

void CNHyungzoBaseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				cltGameMsgRequest_AutoHunting clinfo((TCHAR*)pData);
				cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				m_pNHyungzoUserDlg->m_pEdit[0]->SetText(TEXT(""));
			}
			else
				return;
		}
		break;
	}
}

