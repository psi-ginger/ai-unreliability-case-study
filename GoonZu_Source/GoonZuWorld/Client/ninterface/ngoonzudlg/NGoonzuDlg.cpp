#include "../Client/NInterface/NGoonzuDlg/NGoonzuDlg.h"
#include "../Client/NInterface/NGoonzuDlg/NGoonzuWorkDlg.h"
#include "../Client/NInterface/NGoonzuDlg/NGoonzuSiege.h"
#include "../Client/NInterface/NGoonzuDlg/NGoonzuRaccoonEventDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"

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

CNGoonzuDlg::CNGoonzuDlg() : CDialogController()
{
	m_bSoundCheck = FALSE;

	m_siResidentNum = 0 ;
	m_bSet = FALSE ;

	m_siLiquidationVillageNumber = 0;

	StringCchCopy(m_szGoonzuName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szGoonzuProfile, 256, TEXT(""));
	StringCchCopy(m_szGoonzuID, MAX_PLAYER_NAME, TEXT(""));

	SI32 i = 0 ;

	for ( i = 0 ; i < 4 ; i ++ )
	{
		m_pStatic[i] = NULL ;
	}

	for ( i = 0 ; i < 6 ; i ++ )
	{
		m_pButton[i] = NULL ;
	}


	for ( i = 0 ; i < 2 ; i ++ )
		m_pEdit[i] = NULL ;

	for ( i = 0 ; i < 6 ; i ++ )
	{
		m_pCheck[i] = NULL ;
	}

	m_pMinister = NULL ;
	m_pSetMinister = NULL ;
	m_pLeaveMinister = NULL ;

	m_siSelectMinister = 0 ;
}

CNGoonzuDlg::~CNGoonzuDlg()
{
	SetTimerOnOff(FALSE);

	SI32 i = 0 ;

	for ( i = 0 ; i < 4 ; i ++ )
	{
		SAFE_DELETE(m_pStatic[i]);
	}

	for ( i = 0 ; i < 6 ; i ++ )
	{
		SAFE_DELETE(m_pButton[i]);
	}


	for ( i = 0 ; i < 2 ; i ++ )
	{
		SAFE_DELETE(m_pEdit[i]);
	}

	for ( i = 0 ; i < 6 ; i ++ )
	{
		SAFE_DELETE(m_pCheck[i]);
	}

	SAFE_DELETE(m_pMinister);
	SAFE_DELETE(m_pSetMinister);
	SAFE_DELETE(m_pLeaveMinister);
}

void CNGoonzuDlg::Create()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		if(pclclient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu ))	//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
		{
			file.LoadFile(TEXT("NInterface/Data/NGoonzuDlg/DLG_Goonzu_New.ddf"));
		}
		else
		{
			file.LoadFile(TEXT("NInterface/Data/NGoonzuDlg/DLG_Goonzu.ddf"));
		}
		file.CreatePopupDialog( this, NGOONZU_DLG, TEXT("dialog_Goonzu"), StaticCallBackDialogNGoonzuDlg );

		SI32 i = 0 ;

		for ( i = 0 ; i < 4 ; i ++ )
		{
			NEWCSTATIC(m_pStatic[i]);
		}

		for ( i = 0 ; i < 6 ; i ++ )
		{
			NEWCBUTTON(m_pButton[i]);
		}

		for ( i = 0 ; i < 2 ; i ++ )
		{
			NEWCEDIT(m_pEdit[i]);
		}
		for ( i = 0 ; i < 6 ; i ++ )
		{
			NEWCCHECKBOX(m_pCheck[i]);
		}
		NEWCEDIT(m_pMinister);
		NEWCBUTTON(m_pSetMinister);
		NEWCBUTTON(m_pLeaveMinister);

		file.CreateControl( m_pStatic[0], NGOONZU_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pStatic[1], NGOONZU_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pStatic[2], NGOONZU_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
		if ( pclClient->siServiceArea == ConstServiceArea_English
			|| pclClient->siServiceArea == ConstServiceArea_NHNChina
			|| pclClient->siServiceArea == ConstServiceArea_EUROPE
		)
		{
			m_pStatic[2]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
			m_pStatic[2]->Refresh();
		}
		file.CreateControl( m_pStatic[3], NGOONZU_DIALOG_STATIC4, TEXT("statictext_NONAME4") );

		file.CreateControl( m_pEdit[0], NGOONZU_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pEdit[1], NGOONZU_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

		if(pclclient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu ))	//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
		{
			file.CreateControl( m_pButton[0], NGOONZU_DIALOG_BUTTON1, TEXT("button_RacoonEvent") );
		}
		else
		{
			file.CreateControl( m_pButton[0], NGOONZU_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		}
		file.CreateControl( m_pButton[1], NGOONZU_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NGOONZU_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NGOONZU_DIALOG_BUTTON4, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NGOONZU_DIALOG_BUTTON5, TEXT("button_NONAME5") );
		file.CreateControl( m_pButton[5], NGOONZU_DIALOG_BUTTON6, TEXT("button_NONAME6") );

		file.CreateControl( m_pCheck[0], NGOONZU_DIALOG_CHECK_IZO, TEXT("checkbox_IZO") );
		file.CreateControl( m_pCheck[1], NGOONZU_DIALOG_CHECK_BYUNGZO, TEXT("checkbox_BYUNGZO") );
		file.CreateControl( m_pCheck[2], NGOONZU_DIALOG_CHECK_HOZO, TEXT("checkbox_HOZO") );
		file.CreateControl( m_pCheck[3], NGOONZU_DIALOG_CHECK_HYUNGZO, TEXT("checkbox_HYUNGZO") );
		file.CreateControl( m_pCheck[4], NGOONZU_DIALOG_CHECK_YEZO, TEXT("checkbox_YEZO") );
		file.CreateControl( m_pCheck[5], NGOONZU_DIALOG_CHECK_GONGZO, TEXT("checkbox_GONGZO") );

		file.CreateControl( m_pMinister, NGOONZU_DIALOG_EDIT_SETMINISTER, TEXT("editbox_SETMINISTER") );
		file.CreateControl( m_pSetMinister, NGOONZU_DIALOG_BUTTON_SETMINISTER, TEXT("button_SETMINISTER") );
		file.CreateControl( m_pLeaveMinister, NGOONZU_DIALOG_BUTTON_LEAVEMINISTER, TEXT("button_LEAVEMINISTER") );

		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
		{
			m_pEdit[0]->SetReadOnly(false);
			if(pclclient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu ))	//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
			{
			}
			else
			{
				EnableButton(m_pButton[0], FALSE);
			}
			EnableButton(m_pButton[1], FALSE);

			m_pMinister->Enable(true);
			m_pMinister->SetReadOnly(false);

			SetTimerOnOff( true );
			SetActionTimer( 500 );
		}
		else
		{
			EnableButton(m_pButton[2], FALSE); // 설정버튼 비활성화

			for ( SI32 i = 0 ; i < 6 ; i ++ )
			{
				m_pCheck[i]->Enable(false);
			}

			EnableButton(m_pSetMinister,FALSE);
			EnableButton(m_pLeaveMinister,FALSE);

			//SetTimerOnOff( true );
			//SetActionTimer( 60 );
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETGOONZUINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);


		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENGOONZUDLG);

		m_siSelectMinister = 0 ;

		// 일본버전 예조판서 없음
		m_pCheck[4]->Show( false );
		

		if(pclclient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu ))	//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
		{
		}
		else
		{
			//KHY - 0220 -  푸치코미 옵션 삭제.
			if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
			{
				m_pButton[0]->Show(false);
			}
		}
	}
	else
	{
		DeleteDialog();
	}
}

void CNGoonzuDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
	{
		TCHAR buffer[256] = TEXT("") ;
		//	GetDlgItemText(m_hDlg,IDC_EDIT_GOONZU,buffer,256);
		MStrCpy(buffer, m_pEdit[0]->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2111);
		StringCchPrintf(buffer, 256, pText,m_szGoonzuName,limit);
		//	SetWindowText(m_hGroup,buffer);
		m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER);
	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETGOONZUINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

void CALLBACK CNGoonzuDlg::StaticCallBackDialogNGoonzuDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNGoonzuDlg *pThis = (CNGoonzuDlg *)pControllerMgr;
	pThis->CallBackDialogNGoonzuDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNGoonzuDlg::CallBackDialogNGoonzuDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NGOONZU_DIALOG_CHECK_IZO:
		{
			if( nEvent == EVENT_CHECKBOX_CHANGED  )
			{
				if( m_pCheck[0]->IsCheck() )
					SelectMinister( 1 );
			}
		}
		break;
	case NGOONZU_DIALOG_CHECK_BYUNGZO:
		{
			if( nEvent == EVENT_CHECKBOX_CHANGED  )
			{
				if( m_pCheck[1]->IsCheck() )
					SelectMinister( 2 );
			}
		}
		break;
	case NGOONZU_DIALOG_CHECK_HOZO:
		{
			if( nEvent == EVENT_CHECKBOX_CHANGED  )
			{
				if( m_pCheck[2]->IsCheck() )
					SelectMinister( 3 );
			}
		}
		break;
	case NGOONZU_DIALOG_CHECK_HYUNGZO:
		{
			if( nEvent == EVENT_CHECKBOX_CHANGED  )
			{
				if( m_pCheck[3]->IsCheck() )
					SelectMinister( 4 );
			}
		}
		break;
	case NGOONZU_DIALOG_CHECK_YEZO:
		{
			if( nEvent == EVENT_CHECKBOX_CHANGED  )
			{
				if( m_pCheck[4]->IsCheck() )
					SelectMinister( 5 );
			}
		}
		break;
	case NGOONZU_DIALOG_CHECK_GONGZO:
		{
			if( nEvent == EVENT_CHECKBOX_CHANGED  )
			{
				if( m_pCheck[5]->IsCheck() )
					SelectMinister( 6 );
			}
		}
		break;
	case NGOONZU_DIALOG_BUTTON_SETMINISTER:
		{
			TCHAR charname[MAX_PLAYER_NAME] = TEXT("") ;
			MStrCpy(charname, m_pMinister->GetText(), MAX_PLAYER_NAME);

			if ( m_siSelectMinister < 1 || m_siSelectMinister > 6 )
				break ;

			if ( _tcslen(charname) <= 0 || charname[0] == NULL )
				break ;

			TCHAR *pText = GetTxtFromMgr(5517);
			TCHAR *pTitle = GetTxtFromMgr(5516);

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( charname );
			memcpy( TempBuffer, charname, Size );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
		break;
	case NGOONZU_DIALOG_BUTTON_LEAVEMINISTER:
		{
			TCHAR charname[MAX_PLAYER_NAME] = TEXT("") ;
			MStrCpy(charname, m_pMinister->GetText(), MAX_PLAYER_NAME);

			if ( m_siSelectMinister < 1 || m_siSelectMinister > 6 )
				break ;

			if ( _tcslen(charname) <= 0 || charname[0] == NULL )
				break ;

			//KHY  - 0629 - 문구수정.
			TCHAR *pText = GetTxtFromMgr(5515);
			TCHAR *pTitle = GetTxtFromMgr(5514);

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( charname );
			memcpy( TempBuffer, charname, Size );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NGOONZU_DIALOG_BUTTON1:	
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclclient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu ))	//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
					{
						pclclient->CreateInterface( NRACCOONEVENT_CHANGE_GOONZUEVENT_DLG );
					}
					else
					{
						// 꼼지가기
						cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szGoonzuName );
						cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

						/*
						cltGameMsgRequest_Userid clUserid( m_szGoonzuName );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						*/
					}
				}
				break;
			}
		}
		break;

	case NGOONZU_DIALOG_BUTTON2:	// 귓말하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen(m_szGoonzuName) > 0 && m_szGoonzuName != NULL )
						pclclient->RequestSecretChatByCharName(m_szGoonzuName);
				}
				break;
			}
		}
		break;

	case NGOONZU_DIALOG_BUTTON3:	// 설정하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//	GetDlgItemText(m_hDlg,IDC_EDIT_GOONZU,m_szGoonzuProfile,256);
					MStrCpy(m_szGoonzuProfile, m_pEdit[0]->GetText(), 256);
					SI32 limit = _tcslen(m_szGoonzuProfile);
					if ( limit > 0 && limit < 256 )
					{
						cltGameMsgRequest_SetGoonzuInfo clinfo(m_szGoonzuProfile);
						cltMsg clMsg(GAMEMSG_REQUEST_SETGOONZUINFO, sizeof(clinfo), (BYTE*)&clinfo);
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

	case NGOONZU_DIALOG_BUTTON4:	// 나가기
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

	case NGOONZU_DIALOG_BUTTON5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclclient->m_pDialog[ NGOONZUPOLICY_DLG ] == NULL )
					{
						pclclient->CreateInterface( NGOONZUPOLICY_DLG );
						((CNGoonZuPolicyDlg*)pclclient->m_pDialog[ NGOONZUPOLICY_DLG ])->SetLiquidationVillageNumber( m_siLiquidationVillageNumber );
					}
					else
						pclclient->CreateInterface( NGOONZUPOLICY_DLG );
				}
				break;
			}
		}
		break;

	case NGOONZU_DIALOG_BUTTON6:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclclient->m_pDialog[ NGOONZUSIEGE_DLG ] == NULL )
					{
						pclclient->CreateInterface( NGOONZUSIEGE_DLG );
					}
				}
				break;
			}
		}
		break;

	}
}

void CNGoonzuDlg::SetGoonzu(CMinisterBase *pMini,SI64 finances, SI16 LiquidationVillageNumber)
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
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINISTER_GOONZU")),0,0);
		m_bSoundCheck = TRUE;
	}

	StringCchCopy(m_szGoonzuID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szGoonzuName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szGoonzuProfile, 256, pMini->m_szGreeting);

	m_iTotalFinances = finances ;

	TCHAR buffer[256] = TEXT("");

	TCHAR* pText = GetTxtFromMgr(2112);
	StringCchPrintf(buffer, 256, pText,m_szGoonzuName);
	//	SetWindowText( m_hDlg ,buffer);
	SetTitle(buffer);
	//	SetDlgItemText( m_hDlg,IDC_EDIT_GOONZU, m_szGoonzuProfile );
	m_pEdit[0]->SetText(m_szGoonzuProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
	//	SetWindowText(m_hGroup,buffer);
	m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER);

	g_JYLibFunction.SetNumUnit(m_iTotalFinances,buffer, 256);
	//	SetDlgItemText( m_hDlg,IDC_EDIT_TOTALTAX, buffer );
	m_pEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
	m_pEdit[1]->SetText(buffer);

	m_siLiquidationVillageNumber = LiquidationVillageNumber;
}

void CNGoonzuDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNGoonzuDlg::SetButtonTextColor(CButton* pButton, BOOL Flag)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
	}
}

void CNGoonzuDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
				TCHAR charname[MAX_PLAYER_NAME] = TEXT("") ;
				MStrCpy(charname,(TCHAR*)pData,MAX_PLAYER_NAME);

				cltGameMsgRequest_Goonzu_SetMinister clinfo(m_siSelectMinister,charname,false);
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETMINISTER,sizeof(clinfo),(BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				TCHAR charname[MAX_PLAYER_NAME] = TEXT("") ;
				MStrCpy(charname,(TCHAR*)pData,MAX_PLAYER_NAME);

				cltGameMsgRequest_Goonzu_SetMinister clinfo(m_siSelectMinister,charname,true);
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETMINISTER,sizeof(clinfo),(BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
	return;
}

void CNGoonzuDlg::SelectMinister( SI32 selectedMinister )
{
	for (SI16 i = 0 ; i < 6 ; i ++ )
	{
		if( i == selectedMinister - 1 ) continue;

		m_pCheck[i]->SetCheck(false);
	}

	m_siSelectMinister = selectedMinister ;
}

