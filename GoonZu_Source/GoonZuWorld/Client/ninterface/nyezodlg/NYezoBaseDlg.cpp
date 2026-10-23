#include "../Client/NInterface/NYezoDlg/NYezoBaseDlg.h"
#include "../Client/NInterface/NYezoDlg/NYezoAngelDlg.h"
#include "../Client/NInterface/NYezoDlg/NYezoAngelApplyerDlg.h"
#include "../Client/NInterface/NYezoDlg/NYezoAppointmentDlg.h"
#include "../Client/NInterface/NYezoDlg/NYezoJapanWarDlg.h"

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

CNYezoBaseDlg::CNYezoBaseDlg() : CDialogController()
{
	StringCchCopy(m_szYezoName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szYezoProfile, 256, TEXT(""));
	StringCchCopy(m_szYezoID, MAX_PLAYER_NAME, TEXT(""));

	m_bSet = FALSE;
	m_bSoundCheck= FALSE;

	m_pStatic = NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;
	m_pButton[4] = NULL;
	m_pButton[5] = NULL;
	m_pButton[6] = NULL;
	m_pEdit = NULL;

	m_pNYezoJapanWarDlg = NULL;
	m_pNYezoAngelDlg = NULL;
	m_pNYezoAngelApplyerDlg = NULL;
	m_pNYezoAppointmentDlg = NULL;
}

CNYezoBaseDlg::~CNYezoBaseDlg()
{
	SetTimerOnOff(FALSE);

	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[4]);
	SAFE_DELETE(m_pButton[5]);
	SAFE_DELETE(m_pButton[6]);
	SAFE_DELETE(m_pEdit);

	SAFE_DELETE(m_pNYezoJapanWarDlg);
	SAFE_DELETE(m_pNYezoAngelDlg);
	SAFE_DELETE(m_pNYezoAngelApplyerDlg);
	SAFE_DELETE(m_pNYezoAppointmentDlg);
}

void CNYezoBaseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NYezoDlg/DLG_Yezo_Base.ddf"));
		file.CreatePopupDialog( this, NYEZOBASE_DLG, TEXT("dialog_Yezo_Base"), StaticCallBackDialogNYezoBaseDlg );

		NEWCSTATIC(m_pStatic);
		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		NEWCBUTTON(m_pButton[4]);
		NEWCBUTTON(m_pButton[5]);
		NEWCBUTTON(m_pButton[6]);
		NEWCEDIT(m_pEdit);

		file.CreateControl( m_pStatic, NYEZOBASE_DIALOG_STATIC, TEXT("statictext_NONAME1") );

		file.CreateControl( m_pButton[0], NYEZOBASE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NYEZOBASE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NYEZOBASE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NYEZOBASE_DIALOG_BUTTON4, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NYEZOBASE_DIALOG_BUTTON5, TEXT("button_NONAME5") );
		file.CreateControl( m_pButton[5], NYEZOBASE_DIALOG_BUTTON6, TEXT("button_NONAME6") );
		file.CreateControl( m_pButton[6], NYEZOBASE_DIALOG_BUTTON7, TEXT("button_NONAME7") );

		file.CreateControl( m_pEdit, NYEZOBASE_DIALOG_EDIT, TEXT("editbox_NONAME1") );

		SetButtonTextColor(4);

		m_pNYezoJapanWarDlg = new CNYezoJapanWarDlg();
		m_pNYezoJapanWarDlg->Create( this );
		m_pNYezoJapanWarDlg->Show();

		m_pNYezoAngelDlg = new CNYezoAngelDlg();
		m_pNYezoAngelDlg->Create( this );

		m_pNYezoAngelApplyerDlg = new CNYezoAngelApplyerDlg();
		m_pNYezoAngelApplyerDlg->Create( this );

		m_pNYezoAppointmentDlg = new CNYezoAppointmentDlg();
		m_pNYezoAppointmentDlg->Create();

		cltClient *pclclient = (cltClient*)pclClient;

		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER3 )
		{
			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);

			EnableButton(m_pButton[2], FALSE);
			m_pNYezoJapanWarDlg->SetPrintEditBox(FALSE);

		//	hButton = GetDlgItem(m_hTabDlg[0],IDC_COMBO_WARVILLAGE);
		//	EnableWindow(hButton,FALSE);

			//SetTimerOnOff( true );
			//SetActionTimer( 60 );
		}
		else
		{
			m_pEdit->SetReadOnly(false);

			EnableButton(m_pButton[0], FALSE);
			EnableButton(m_pButton[1], FALSE);

			m_pNYezoJapanWarDlg->EnableEditBox();
			m_pNYezoJapanWarDlg->SetPrintEditBox(TRUE);

			SetTimerOnOff( true );
			SetActionTimer( 500 );
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETYEZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pButton[0]->Show(false);
		}
	}
	else
	{
		DeleteDialog();
	}
}

void CNYezoBaseDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
	{
		TCHAR buffer[256] = TEXT("") ;
		m_pNYezoJapanWarDlg->GetJapanWar();

//		GetDlgItemText(m_hDlg,IDC_EDIT_YEZO,buffer,256);
		MStrCpy(buffer, m_pEdit->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2092);
		StringCchPrintf(buffer, 256, pText,m_szYezoName,limit);
//		SetWindowText(m_hGroup,buffer);
		m_pStatic->SetText(buffer, DT_LEFT | DT_VCENTER);
	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETYEZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

void CALLBACK CNYezoBaseDlg::StaticCallBackDialogNYezoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNYezoBaseDlg *pThis = (CNYezoBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNYezoBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNYezoBaseDlg::CallBackDialogNYezoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NYEZOBASE_DIALOG_BUTTON1:		// 꼼지가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szYezoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					
					/*
					cltGameMsgRequest_Userid clUserid( m_szYezoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/
				}
				break;
			}
		}
		break;

	case NYEZOBASE_DIALOG_BUTTON2:		// 귓말하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen(m_szYezoName) > 0 && m_szYezoName != NULL )
						pclclient->RequestSecretChatByCharName(m_szYezoName);
				}
				break;
			}
		}
		break;

	case NYEZOBASE_DIALOG_BUTTON3:		// 설정하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pNYezoJapanWarDlg->JapanWarCheck();
										
				//	GetDlgItemText(m_hDlg,IDC_EDIT_YEZO,m_szYezoProfile,256);
					MStrCpy(m_szYezoProfile, m_pEdit->GetText(), 256);
					SI32 limit = _tcslen(m_szYezoProfile);
					if ( limit > 0 && limit < 256 )
					{
					//	TCHAR* pTitle = GetTxtFromMgr(2098);
					//	TCHAR* pText = GetTxtFromMgr(2099);

					//	if ( m_pNYezoJapanWarDlg->GetVillageUniqe() == 0 &&
					//		MessageBox(NULL,pTitle,
					//		pText, MB_ICONQUESTION | MB_YESNO) == IDNO )
					//		break;

						cltGameMsgRequest_SetYezoInfo clinfo(m_szYezoProfile,m_pNYezoJapanWarDlg->GetSword(),m_pNYezoJapanWarDlg->GetGun(),m_pNYezoJapanWarDlg->GetCommand(),m_pNYezoJapanWarDlg->GetPorter(),m_pNYezoJapanWarDlg->GetVillageUniqe());
						cltMsg clMsg(GAMEMSG_REQUEST_SETYEZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
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

	case NYEZOBASE_DIALOG_BUTTON4:		// 나가기
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

	case NYEZOBASE_DIALOG_BUTTON5:		// 왜구 전쟁
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(4);

					m_pNYezoJapanWarDlg->Show();
					m_pNYezoAngelDlg->Hide();
					m_pNYezoAngelApplyerDlg->Hide();
				}
				break;
			}
		}
		break;

	case NYEZOBASE_DIALOG_BUTTON6:		// 수호천사 임명
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(5);

					m_pNYezoJapanWarDlg->Hide();
					m_pNYezoAngelDlg->Show();
					m_pNYezoAngelApplyerDlg->Hide();
				}
				break;
			}
		}
		break;

	case NYEZOBASE_DIALOG_BUTTON7:		// 수호천사 신청
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(6);

					m_pNYezoJapanWarDlg->Hide();
					m_pNYezoAngelDlg->Hide();
					m_pNYezoAngelApplyerDlg->Show();
				}
				break;
			}
		}
		break;
	}	
}

void CNYezoBaseDlg::SetYezoBase(CMinisterBase *pMini,SI32 sword,SI32 gun,SI32 command,SI32 porter,SI32 villageunique)
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_bSet )
	{
		TCHAR* pTitle = GetTxtFromMgr(2045);
		TCHAR* pText = GetTxtFromMgr(2046);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		m_bSet = FALSE ;
		return ;
	}

	if(m_bSoundCheck == FALSE)
	{
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_YEZO")),0,0);
		m_bSoundCheck = TRUE;
	}

	StringCchCopy(m_szYezoID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szYezoName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szYezoProfile, 256, pMini->m_szGreeting);

	TCHAR buffer[256] = TEXT("");

	TCHAR* pText = GetTxtFromMgr(2106);
	StringCchPrintf(buffer, 256, pText,m_szYezoName);
//	SetWindowText( m_hDlg ,buffer);
	SetTitle(buffer);
//	SetDlgItemText( m_hDlg,IDC_EDIT_YEZO, m_szYezoProfile );
	m_pEdit->SetText(m_szYezoProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
//	SetWindowText(m_hGroup,buffer);
	m_pStatic->SetText(buffer, DT_LEFT |DT_VCENTER| DT_SINGLELINE);

	m_pNYezoJapanWarDlg->SetJapanWar(sword, gun, command, porter, villageunique);
}


BOOL CNYezoBaseDlg::SetListDlg(SI32 index)
{
	BOOL check;
	check = m_pNYezoAppointmentDlg->SetListDlg(index);

	return check;
}

void CNYezoBaseDlg::ShowListDlg(YezoAngelList yezoangellist [])
{
	m_pNYezoAppointmentDlg->ShowListDlg(yezoangellist);
}

void CNYezoBaseDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNYezoBaseDlg::SetButtonTextColor(SI32 Index)
{
	for( SI16 i = 4; i < 7; ++i )
	{
		if ( i == Index )
		{
			m_pButton[ i ]->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR );
			m_pButton[ i ]->Refresh();
		}
		else
		{
			m_pButton[ i ]->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
			m_pButton[ i ]->Refresh();
		}
	}
}
