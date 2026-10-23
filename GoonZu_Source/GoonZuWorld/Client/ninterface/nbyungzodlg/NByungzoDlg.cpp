#include "../Client/NInterface/NByungzoDlg/NByungzoDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

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

CNByungzoDlg::CNByungzoDlg() : CDialogController()
{
	m_siVillageWarTime = 0;
	m_bSet = FALSE ;
	m_bByungzoCheck = FALSE;
	m_bSound = FALSE;
	m_bWarTimeSet	 = TRUE;

	StringCchCopy(m_szByungzoName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szByungzoProfile, 256, TEXT(""));
	StringCchCopy(m_szByungzoID, MAX_PLAYER_NAME, TEXT(""));

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pEdit = NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;
	m_pButton[4] = NULL;
	m_pButton[5] = NULL;
	m_pRadio[0] = NULL;
	m_pRadio[1] = NULL;
	m_pRadio[2] = NULL;

	m_siSetTime = 0;				// 전쟁설정시간
	m_siWarStartTime = 0;			// 전쟁시작시간
	m_siSetWarCount = 0;			// 전쟁설정 횟수
	m_siBlackArmyNum = 0;			// 전쟁 몬스터 설정 수

	SI32 i;
	for(i = 0; i < 12; ++i)
		m_pWarStatic[i] = NULL;
	for(i = 0; i < 2; ++i)
		m_pWarEdit[i] = NULL;
	for(i = 0; i < 4; ++i)
		m_pWarCombo[i] = NULL;

	m_pWarOutLine = NULL;
}

CNByungzoDlg::~CNByungzoDlg()
{
	SetTimerOnOff( false );

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);

	SAFE_DELETE(m_pEdit);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[4]);
	SAFE_DELETE(m_pButton[5]);

	SAFE_DELETE(m_pRadio[0]);
	SAFE_DELETE(m_pRadio[1]);
	SAFE_DELETE(m_pRadio[2]);

	SAFE_DELETE(m_pOutLine);

	SI32 i;
	for(i = 0;i < 12; ++i)
		SAFE_DELETE(m_pWarStatic[i]);
	for(i = 0;i < 2; ++i)
		SAFE_DELETE(m_pWarEdit[i]); 

	for(i = 0;i < 4; ++i)
		SAFE_DELETE(m_pWarCombo[i]);

	SAFE_DELETE(m_pWarOutLine);

}

void CNByungzoDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NByungzoDlg/DLG_Byungzo.ddf"));
		file.CreatePopupDialog( this, NBYUNGZO_DLG, TEXT("dialog_Byungzo"), StaticCallBackDialogNByungzoDlg );
		
		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);
		NEWCSTATIC(m_pStatic[2]);

		NEWCEDIT(m_pEdit);

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		NEWCBUTTON(m_pButton[4]);
		NEWCBUTTON(m_pButton[5]);
		NEWCOUTLINE(m_pOutLine);

		SI32 i;
		for(i = 0;i < 12; ++i)
			NEWCSTATIC(m_pWarStatic[i]);
		for(i = 0;i < 2; ++i)
			NEWCEDIT(m_pWarEdit[i]); 
		for(i = 0;i < 4; ++i)
			NEWCCOMBOBOX(m_pWarCombo[i]);

		NEWCOUTLINE(m_pWarOutLine);

	//	SAFE_DELETE(m_pRadio[0]);
	//	SAFE_DELETE(m_pRadio[1]);
	//	SAFE_DELETE(m_pRadio[2]);

		m_pRadio[0] = new CRadioButton( this );
		m_pRadio[1] = new CRadioButton( this );
		m_pRadio[2] = new CRadioButton( this );

		file.CreateControl( m_pStatic[0], NBYUNGZO_DIALOG_STATIC1, TEXT("statictext_NONAME1")) ;	// 공작 설명
		file.CreateControl( m_pStatic[1], NBYUNGZO_DIALOG_STATIC2, TEXT("statictext_NONAME2")) ;	// 공성전
		file.CreateControl( m_pStatic[2], NBYUNGZO_DIALOG_STATIC3, TEXT("statictext_NONAME3")) ;	// 공성전 지속시간

		file.CreateControl( m_pEdit, NBYUNGZO_DIALOG_EDIT, TEXT("editbox_NONAME1")) ;				// 인사말 설정 EDIT

		file.CreateControl( m_pButton[0], NBYUNGZO_DIALOG_BUTTON1, TEXT("button_NONAME1") );	// 꼼지가기
		file.CreateControl( m_pButton[1], NBYUNGZO_DIALOG_BUTTON2, TEXT("button_NONAME2") );	// 귓말하기
		file.CreateControl( m_pButton[2], NBYUNGZO_DIALOG_BUTTON3, TEXT("button_NONAME3") );	// 설정하기(전체)
		file.CreateControl( m_pButton[3], NBYUNGZO_DIALOG_BUTTON4, TEXT("button_NONAME4") );	// 나가기
		file.CreateControl( m_pButton[4], NBYUNGZO_DIALOG_BUTTON5, TEXT("button_NONAME5") );	// 설정하기(공성)

		file.CreateControl( m_pRadio[0], NBYUNGZO_DIALOG_RADIO1, TEXT("radiobutton_NONAME1") );	// 공성시간 40분
		file.CreateControl( m_pRadio[1], NBYUNGZO_DIALOG_RADIO2, TEXT("radiobutton_NONAME2") );	// 공성시간 50분
		file.CreateControl( m_pRadio[2], NBYUNGZO_DIALOG_RADIO3, TEXT("radiobutton_NONAME3") );	// 공성시간 60분

		file.CreateControl( m_pOutLine, NBYUNGZO_DIALOG_OUTLINE, TEXT("outline_NONAME1") );		// 외각선

		file.CreateControl( m_pWarStatic[0], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME4"));	// 흑의 군단
		file.CreateControl( m_pWarStatic[1], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME5"));	// 개최가능 횟수
		file.CreateControl( m_pWarStatic[2], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME6"));	// 개최일시
		file.CreateControl( m_pWarStatic[3], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME7"));	// 병력설정
		file.CreateControl( m_pWarStatic[4], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME8"));	// 흑의 군단(100~600명)
		file.CreateControl( m_pWarStatic[5], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME9"));	// 년
		file.CreateControl( m_pWarStatic[6], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME10"));	// 월
		file.CreateControl( m_pWarStatic[7], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME11"));	// 회
		file.CreateControl( m_pWarStatic[8], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME12"));	// 명
		file.CreateControl( m_pWarStatic[9], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME13"));	// 일
		file.CreateControl( m_pWarStatic[10], NBYUNGZOWAR_DIALOG_STATIC, TEXT("statictext_NONAME14"));	// 시

		file.CreateControl( m_pWarEdit[0], NBYUNGZOWAR_DIALOG_EDIT1, TEXT("editbox_NONAME2"));			// 흑군 가능 횟수
		file.CreateControl( m_pWarEdit[1], NBYUNGZOWAR_DIALOG_EDIT2, TEXT("editbox_NONAME3"));			// 흑군 몬스터 수

		file.CreateControl( m_pWarCombo[0], NBYUNGZOWAR_DIALOG_COMBOBOX_YEAR, TEXT("combobox_NONAME1"));	// 년 설정
		file.CreateControl( m_pWarCombo[1], NBYUNGZOWAR_DIALOG_COMBOBOX_MONTH, TEXT("combobox_NONAME2"));	// 월 설정
		file.CreateControl( m_pWarCombo[2], NBYUNGZOWAR_DIALOG_COMBOBOX_DAY, TEXT("combobox_NONAME3"));		// 일 설정
		file.CreateControl( m_pWarCombo[3], NBYUNGZOWAR_DIALOG_COMBOBOX_HOUR, TEXT("combobox_NONAME4"));	// 시 설정

		file.CreateControl( m_pWarOutLine, NBYUNGZOWAR_DIALOG_OUTLINE, TEXT("outline_WAR"));	// 외각선

		cltClient* pclclient = (cltClient*)pclClient;
		SI32 Year = pclclient->sTime.wYear;

		TCHAR buf[32];

		SYSTEMTIME	stCurTime;
		memcpy(&stCurTime, &pclclient->sTime, sizeof(stCurTime));

		// 전쟁 개시 설정은 현재시각으로 부터 12시간 이후 부터 10일 전까지이다.
		TimeAddHour(&stCurTime, 13, &m_stStartTime);
		TimeAddDate(&stCurTime, 10, &m_stEndTime);

		stComboBoxItemData tmpComboBoxItemData;

		m_pWarCombo[0]->Clear();
		for (i = m_stStartTime.wYear; i <= m_stEndTime.wYear; i++ )
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[0]->AddItem( &tmpComboBoxItemData );
			
		}
		m_pWarCombo[0]->SetCursel(0, false);

		m_pWarCombo[1]->Clear();
		for (i = 1; i <= 12; i++)
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[1]->AddItem( &tmpComboBoxItemData );
		}
		m_pWarCombo[1]->SetCursel(0, false);

		m_pWarCombo[2]->Clear();
		for (i = 1; i <= 31; i++)
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[2]->AddItem( &tmpComboBoxItemData );
		}
		m_pWarCombo[2]->SetCursel(0, false);
	
		m_pWarCombo[3]->Clear();
		for (i = 0; i <= 24; i++)
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[3]->AddItem( &tmpComboBoxItemData );
		}
		m_pWarCombo[3]->SetCursel(0, false);

		// 캐릭터에 따라 컨트롤 활성화 설정
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER4 )
		{
			m_bByungzoCheck = TRUE;

			EnableButton(m_pButton[0], FALSE);
			EnableButton(m_pButton[1], FALSE);

			m_pRadio[0]->Enable(TRUE);
			m_pRadio[1]->Enable(TRUE);
			m_pRadio[2]->Enable(TRUE);

			m_pWarCombo[0]->Enable(FALSE);
			m_pWarCombo[1]->Enable(FALSE);
			m_pWarCombo[2]->Enable(FALSE);
			m_pWarCombo[3]->Enable(FALSE);

			m_pEdit->SetReadOnly(FALSE);

			EnableEditBox();
			SetPrintEditBox(TRUE);

			SetTimerOnOff( true );
			SetActionTimer( 500 );
		}
		else
		{
			EnableButton(m_pButton[2], FALSE);
			EnableButton(m_pButton[4], FALSE);

			m_pRadio[0]->Enable(FALSE);
			m_pRadio[1]->Enable(FALSE);
			m_pRadio[2]->Enable(FALSE);

			m_pWarCombo[0]->Enable(FALSE);
			m_pWarCombo[1]->Enable(FALSE);
			m_pWarCombo[2]->Enable(FALSE);
			m_pWarCombo[3]->Enable(FALSE);

			SetPrintEditBox(FALSE);
		}

		// 서버로 정보요청
		cltMsg clMsg(GAMEMSG_REQUEST_GETBYUNGZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		// 퀘스트 통과
		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENBYUNGZODLG);

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

void CNByungzoDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if(m_bByungzoCheck)
	{
		TCHAR buffer[256] = TEXT("") ;
		MStrCpy(buffer, m_pEdit->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2108);
		StringCchPrintf(buffer, 256, pText,m_szByungzoName,limit);
		m_pStatic[0]->SetText(buffer, DT_LEFT);	// 호조 내용 얼마나 적었나 표시

		GetWar();
	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETBYUNGZOINFO, 0, NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

void CALLBACK CNByungzoDlg::StaticCallBackDialogNByungzoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNByungzoDlg *pThis = (CNByungzoDlg *)pControllerMgr;
	pThis->CallBackDialogNByungzoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNByungzoDlg::CallBackDialogNByungzoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBYUNGZO_DIALOG_BUTTON1:	// 꼼지
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szByungzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					/*
					cltGameMsgRequest_Userid clUserid( m_szByungzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/
				}
				break;
			}
		}
		break;

	case NBYUNGZO_DIALOG_BUTTON2:	// 귓말
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen(m_szByungzoName) > 0 && m_szByungzoName != NULL )
						pclclient->RequestSecretChatByCharName(m_szByungzoName);
				}
				break;
			}
		}
		break;

	case NBYUNGZO_DIALOG_BUTTON3:	// 설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if (WarCheck()) 
					{
						MStrCpy(m_szByungzoProfile, m_pEdit->GetText(), 256);
						SI32 limit = _tcslen(m_szByungzoProfile);
						if ( limit > 0 && limit < 256 )
						{
							cltGameMsgRequest_SetByungzoInfo clinfo(m_szByungzoProfile,m_siVillageWarTime, 
								m_siSetTime, m_siWarStartTime, m_siSetWarCount, m_siBlackArmyNum );
							cltMsg clMsg(GAMEMSG_REQUEST_SETBYUNGZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
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
				}
				break;
			}
		}
		break;
 
	case NBYUNGZO_DIALOG_BUTTON4:
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


	case NBYUNGZO_DIALOG_BUTTON5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->bVillageWarSwitch)	// 공성전 중일경우 설정 불가
					{
						TCHAR * pTitle = GetTxtFromMgr(2019);
						TCHAR * pText = GetTxtFromMgr(6493);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
						break;
					}
					
					MStrCpy(m_szByungzoProfile, m_pEdit->GetText(), 256);
					SI32 limit = _tcslen(m_szByungzoProfile);
					if ( limit > 0 && limit < 256 )
					{
						cltGameMsgRequest_SetByungzoInfoVillageWarTime clinfo( m_szByungzoProfile ,m_siVillageWarTime);
						cltMsg clMsg(GAMEMSG_REQUEST_SETBYUNGZOINFO_VILLAGEWARINFO,sizeof(clinfo),(BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						m_bWarTimeSet = TRUE;
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

	case NBYUNGZO_DIALOG_RADIO1:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[0]->IsCheck();
					if(Check)
					{
						m_siVillageWarTime = 40 ;
						m_bWarTimeSet = FALSE;
					}
				}
				break;
			}
		}
		break;

	case NBYUNGZO_DIALOG_RADIO2:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[1]->IsCheck();
					if(Check)
					{
						m_siVillageWarTime = 50 ;
						m_bWarTimeSet = FALSE;
					}
				}
				break;
			}
		}
		break;

	case NBYUNGZO_DIALOG_RADIO3:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[2]->IsCheck();
					if(Check)
					{
						m_siVillageWarTime = 60 ;
						m_bWarTimeSet = FALSE;
					}
				}
				break;
			}
		}
		break;
	case NBYUNGZOWAR_DIALOG_COMBOBOX_YEAR:
		{
			_SYSTEMTIME startTime;
			TCHAR buffer[256] = TEXT("") ;
			if( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				if( m_pWarCombo[0]->GetSelectedIndex() >= 0 )
				{
					MStrCpy(buffer, m_pWarCombo[0]->GetText(m_pWarCombo[0]->GetSelectedIndex()), 8);
					startTime.wYear = _tstoi(buffer);
					SetWarTimeComboBox(startTime.wYear, 0, 0);
				}
			}
		}
		break;
	case NBYUNGZOWAR_DIALOG_COMBOBOX_MONTH:
		{
			_SYSTEMTIME startTime;
			TCHAR buffer[256] = TEXT("") ;
			if( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				if( m_pWarCombo[0]->GetSelectedIndex() >= 0 && 
					m_pWarCombo[1]->GetSelectedIndex() >= 0 )
				{
					MStrCpy(buffer, m_pWarCombo[0]->GetText(m_pWarCombo[0]->GetSelectedIndex()), 8);
					startTime.wYear = _tstoi(buffer);
					MStrCpy(buffer, m_pWarCombo[1]->GetText(m_pWarCombo[1]->GetSelectedIndex()), 8);
					startTime.wMonth = _tstoi(buffer);
					SetWarTimeComboBox(startTime.wYear, startTime.wMonth, 0);
				}
			}
		}
		break;
	case NBYUNGZOWAR_DIALOG_COMBOBOX_DAY:
		{
			_SYSTEMTIME startTime;
			TCHAR buffer[256] = TEXT("") ;
			if( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				if( m_pWarCombo[0]->GetSelectedIndex() >= 0 && 
					m_pWarCombo[1]->GetSelectedIndex() >= 0 &&
					m_pWarCombo[2]->GetSelectedIndex() >= 0)
				{
					MStrCpy(buffer, m_pWarCombo[0]->GetText(m_pWarCombo[0]->GetSelectedIndex()), 8);
					startTime.wYear = _tstoi(buffer);
					MStrCpy(buffer, m_pWarCombo[1]->GetText(m_pWarCombo[1]->GetSelectedIndex()), 8);
					startTime.wMonth = _tstoi(buffer);
					MStrCpy(buffer, m_pWarCombo[2]->GetText(m_pWarCombo[2]->GetSelectedIndex()), 8);
					startTime.wDay = _tstoi(buffer);
					SetWarTimeComboBox(startTime.wYear, startTime.wMonth, startTime.wDay);
				}
			}
		}
		break;
	case NBYUNGZOWAR_DIALOG_COMBOBOX_HOUR:
		{
			if( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
			}
		}
		break;
	}
}

void CNByungzoDlg::SetByungzo(CMinisterBase *pMini,SI32 villagewartime, SI32 settime, SI32 warstarttime, 
							  SI16 setwarcount, SI32 blackarmynum)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_bSet )
	{	
		TCHAR* pTitle = GetTxtFromMgr(2045);
		TCHAR* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		m_bSet = FALSE ;	
		SetWar(settime, warstarttime, setwarcount, blackarmynum);
		return;
	}

	if( m_bSound == FALSE )
	{
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BYUNGZO")),0,0);
		m_bSound = TRUE;
	}

	StringCchCopy(m_szByungzoID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szByungzoName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szByungzoProfile, 256, pMini->m_szGreeting);

	switch(villagewartime)
	{
	case 40 : m_pRadio[0]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON); break;
	case 50 : m_pRadio[1]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON); break;
	case 60 : m_pRadio[2]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON); break;
	default:  m_pRadio[0]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON); break;
	}

	m_siVillageWarTime = villagewartime ;

	TCHAR buffer[256] = TEXT("") ;
	TCHAR* pText = GetTxtFromMgr(2109);
	StringCchPrintf(buffer, 256, pText,m_szByungzoName);
	SetTitle(buffer);

	m_pEdit->SetText(m_szByungzoProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
	m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER);

	SetWar(settime, warstarttime, setwarcount, blackarmynum);
}

void CNByungzoDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNByungzoDlg::SetButtonTextColor(CButton* pButton, BOOL Flag)
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

void CNByungzoDlg::EnableEditBox()
{
	m_pWarEdit[1]->SetReadOnly(FALSE);
}

void CNByungzoDlg::SetPrintEditBox(BOOL flag)
{
	if(flag)
	{
		m_pWarEdit[0]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
		m_pWarEdit[1]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
	}
	else
	{
		m_pWarEdit[0]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pWarEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pWarCombo[0]->Enable(FALSE);
		m_pWarCombo[1]->Enable(FALSE);
		m_pWarCombo[2]->Enable(FALSE);
		m_pWarCombo[3]->Enable(FALSE);

	}
}

bool CNByungzoDlg::WarCheck()
{
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR buffer[8] = TEXT("") ;

	_SYSTEMTIME startTime;
	ZeroMemory(&startTime, sizeof(_SYSTEMTIME));

	UI32 nowtime = pclClient->GetMinuteFromsTime(pclclient->sTime);
	UI32 warstarttime = (UI32)m_siWarStartTime;

	MStrCpy(buffer, m_pWarEdit[1]->GetText(), 8);
	if (m_siBlackArmyNum != _tstoi(buffer))
	{
		// 병력설정은 전쟁시작시간 12시간 이후부터 가능
		if (nowtime - warstarttime > 0 && nowtime - warstarttime <= 12 * 60 )
		{
			TCHAR* pTitle = GetTxtFromMgr(2019);
			TCHAR* pText = GetTxtFromMgr(6373);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return false;
		}
	}

	// 이벤트 가능 횟수 초과
	if (m_siSetWarCount >= MAX_SETWARCOUNT)
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(6374);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return false;
	}

	m_siBlackArmyNum = _tstoi(buffer);

	if ( m_siBlackArmyNum < 100 || m_siBlackArmyNum > 600 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(6368);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return false;
	}

	MStrCpy(buffer, m_pWarCombo[0]->GetText(m_pWarCombo[0]->GetSelectedIndex()), 8);
	startTime.wYear = _tstoi(buffer);
	MStrCpy(buffer, m_pWarCombo[1]->GetText(m_pWarCombo[1]->GetSelectedIndex()), 8);
	startTime.wMonth = _tstoi(buffer);
	MStrCpy(buffer, m_pWarCombo[2]->GetText(m_pWarCombo[2]->GetSelectedIndex()), 8);
	startTime.wDay = _tstoi(buffer);
	MStrCpy(buffer, m_pWarCombo[3]->GetText(m_pWarCombo[3]->GetSelectedIndex()), 8);
	startTime.wHour = _tstoi(buffer);

	m_siWarStartTime = pclClient->GetMinuteFromsTime(startTime);

	warstarttime = (UI32)m_siWarStartTime;

	if ( (SI32)nowtime > m_siWarStartTime 
		 || m_siWarStartTime - nowtime < 12 * 60		// 전쟁시작시간이 12시간 이후가 아닐때
		 || m_siWarStartTime - nowtime > 10 * 24 * 60   // 전쟁시작시간이 10일 이후일때
		)
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(6369);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return false;
	}

	return true;
}

void CNByungzoDlg::GetWar()
{
	TCHAR buffer[256] = TEXT("") ;

	MStrCpy(buffer, m_pWarEdit[1]->GetText(), 8);
	m_siBlackArmyNum = _tstoi(buffer);

	_SYSTEMTIME startTime;

	MStrCpy(buffer, m_pWarCombo[0]->GetText(m_pWarCombo[0]->GetSelectedIndex()), 8);
	startTime.wYear = _tstoi(buffer);
	MStrCpy(buffer, m_pWarCombo[1]->GetText(m_pWarCombo[1]->GetSelectedIndex()), 8);
	startTime.wMonth = _tstoi(buffer);
	MStrCpy(buffer, m_pWarCombo[2]->GetText(m_pWarCombo[2]->GetSelectedIndex()), 8);
	startTime.wDay = _tstoi(buffer);
	MStrCpy(buffer, m_pWarCombo[3]->GetText(m_pWarCombo[3]->GetSelectedIndex()), 8);
	startTime.wHour = _tstoi(buffer);

	m_siWarStartTime = pclClient->GetMinuteFromsTime(startTime);


}

void CNByungzoDlg::SetWar(SI32 settime, SI32 warstarttime, SI16 setwarcount, SI32 blackarmynum)
{
	m_siSetTime = settime;
	m_siWarStartTime = warstarttime;
	m_siSetWarCount = setwarcount;
	m_siBlackArmyNum = blackarmynum;

	_SYSTEMTIME time = pclClient->GetTimeFromsMinute(m_siWarStartTime);

	TCHAR buf[32];

	if (m_siWarStartTime > 0)
	{
		StringCchPrintf(buf, 32, "%d", time.wYear );
		m_pWarCombo[0]->SetCursel(m_pWarCombo[0]->FindIndexFromText(buf), false);

		StringCchPrintf(buf, 32, "%d", time.wMonth);
		m_pWarCombo[1]->SetCursel(m_pWarCombo[1]->FindIndexFromText(buf), false);

		StringCchPrintf(buf, 32, "%d", time.wDay);
		m_pWarCombo[2]->SetCursel(m_pWarCombo[2]->FindIndexFromText(buf), false);

		StringCchPrintf(buf, 32, "%d", time.wHour);
		m_pWarCombo[3]->SetCursel(m_pWarCombo[3]->FindIndexFromText(buf), false);

		m_pWarCombo[0]->Enable(FALSE);
		m_pWarCombo[1]->Enable(FALSE);
		m_pWarCombo[2]->Enable(FALSE);
		m_pWarCombo[3]->Enable(FALSE);

	}
	else
	{

		cltClient *pclclient = (cltClient*)pclClient;

		SYSTEMTIME stCurTime;
		memcpy(&stCurTime, &pclclient->sTime, sizeof(stCurTime));

		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER4 )
		{
			m_pWarCombo[0]->Enable(TRUE);
			m_pWarCombo[1]->Enable(TRUE);
			m_pWarCombo[2]->Enable(TRUE);
			m_pWarCombo[3]->Enable(TRUE);

			TimeAddHour(&stCurTime, 10, &stCurTime);
			SetWarTimeComboBox( stCurTime.wYear, 0, 0 );
			SetWarTimeComboBox( stCurTime.wYear, stCurTime.wMonth, 0 );
			SetWarTimeComboBox( stCurTime.wYear, stCurTime.wMonth, stCurTime.wDay );
		}
		else
		{
            // 전쟁시간이 설정되지 않았으므로 콤보박스를 초기화한다.
			m_pWarCombo[0]->Clear();
			m_pWarCombo[1]->Clear();
			m_pWarCombo[2]->Clear();
			m_pWarCombo[3]->Clear();

			m_pWarCombo[0]->Enable(FALSE);
			m_pWarCombo[1]->Enable(FALSE);
			m_pWarCombo[2]->Enable(FALSE);
			m_pWarCombo[3]->Enable(FALSE);
		}
	}

	StringCchPrintf(buf, 32, "%d", MAX_SETWARCOUNT - setwarcount);
	m_pWarEdit[0]->SetText(buf);

	StringCchPrintf(buf, 32, "%d", m_siBlackArmyNum);
	m_pWarEdit[1]->SetText(buf);

}

void CNByungzoDlg::SetWarTimeComboBox(SI32 siSelYear, SI32 siSelMonth, SI32 siSelDay)
{
	if( siSelYear <= 0 && siSelMonth <= 0 && siSelDay <= 0 )			return;

	cltClient* pclclient = (cltClient*)pclClient;

	SI32	siMinYear = 3000, siMaxYear = 0;
	SI32	siMinMonth = 13, siMaxMonth = 0;
	SI32	siMinDay = 32, siMaxDay = 0;
	SI32	siMinHour = 25, siMaxHour = 0;

	SYSTEMTIME	stTime;
	TimeAddMinute(&m_stStartTime, 1, &stTime);

	bool bUnlimited = true;
	bool bInValid = false;
	while( bUnlimited )
	{
		if( TimeInBounds(&stTime, &m_stStartTime, &m_stEndTime) == false )		break;

		bInValid = false;
		// 선택된 날짜에서 벗어나면, 체크하지 않는다.
		if( siSelYear > 0 )		{	if( stTime.wYear  != siSelYear )	bInValid = true;	}
		if( siSelMonth > 0 )	{	if( stTime.wMonth != siSelMonth )	bInValid = true;	}
		if( siSelDay > 0 )		{	if( stTime.wDay   != siSelDay )		bInValid = true;	}

		if( bInValid )			{	TimeAddHour(&stTime, 1, &stTime);	continue;			}

		if( stTime.wYear <= siMinYear )		siMinYear = stTime.wYear;
		if( stTime.wYear >= siMaxYear )		siMaxYear = stTime.wYear;

		if( stTime.wMonth <= siMinMonth )	siMinMonth = stTime.wMonth;
		if( stTime.wMonth >= siMaxMonth )	siMaxMonth = stTime.wMonth;

		if( stTime.wDay <= siMinDay )		siMinDay = stTime.wDay;
		if( stTime.wDay >= siMaxDay )		siMaxDay = stTime.wDay;

		if( stTime.wHour <= siMinHour )		siMinHour = stTime.wHour;
		if( stTime.wHour >= siMaxHour )		siMaxHour = stTime.wHour;

		TimeAddHour(&stTime, 1, &stTime);
	}

	if( siMinYear >= 3000 || siMinMonth >= 13 || siMinDay >= 32)			return;

	TCHAR buf[32] = { '\0' };
	SI32  i=0;
	stComboBoxItemData tmpComboBoxItemData;

	if( siSelYear <= 0 )
	{
		m_pWarCombo[0]->Clear();
		for (i = siMinYear; i <= siMaxYear; i++ )
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[0]->AddItem( &tmpComboBoxItemData );
		}
		m_pWarCombo[0]->SetCursel(0, false);
	}

	if( siSelMonth <= 0 )
	{
		m_pWarCombo[1]->Clear();
		for (i = siMinMonth; i <= siMaxMonth; i++)
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[1]->AddItem( &tmpComboBoxItemData );
		}
		m_pWarCombo[1]->SetCursel(0, false);
	}

	if( siSelDay <= 0 )
	{
		m_pWarCombo[2]->Clear();
		for (i = siMinDay; i <= siMaxDay; i++)
		{
			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			m_pWarCombo[2]->AddItem( &tmpComboBoxItemData );
		}
		m_pWarCombo[2]->SetCursel(0, false);
	}

	
	m_pWarCombo[3]->Clear();
	for (i = siMinHour; i <= siMaxHour; i++)
	{
		StringCchPrintf(buf, 32, "%d", i);

		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( 0, buf );
		m_pWarCombo[3]->AddItem( &tmpComboBoxItemData );
	}
	m_pWarCombo[3]->SetCursel(0, false);
}