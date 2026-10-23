#include "../Client/NInterface/NGongzoDlg/NGongzoDlg.h"

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

#include "../Server/Minister/Minister-Gongzo.h"

extern cltCommonLogic* pclClient;

CNGongzoDlg::CNGongzoDlg() : CDialogController()
{
	StringCchCopy(m_szGongzoName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szGongzoProfile, 256, TEXT(""));
	StringCchCopy(m_szGongzoID, MAX_PLAYER_NAME, TEXT(""));

	m_siPriceTime = 0 ;
	m_siLeftOverValue = 0 ;
	m_siLeftOverValue_Armor = 0;
	m_siSetSkill = 0 ;
	m_siIDCEditNum = 0;

	SI32 i;
	for ( i = 0 ; i < GONGZO_SKILL_TOTAL ; i ++ )
	{
		m_stGongzoMember.siPriceValue[i] = 0 ;
		m_stGongzoMember.siSkillValue[i] = 0 ;
	}
	m_stGongzoMember.siCraftFund = 0;

	m_bSet = FALSE ;
	m_bSetSpin = FALSE ;
	m_bMonthOver = TRUE ;
	m_bGongzoCheck = FALSE;
	m_bSoundCheck = FALSE;
	m_bGongzoReturnValue = FALSE;

	for(i=0;i<33;++i)
		m_pStatic[i] = NULL;
	for(i=0;i<19;++i)
		m_pButton[i] = NULL;
	for(i=0;i<8;++i)
		m_pEdit[i] = NULL;

	m_pOutLine[0] = NULL;
	m_pOutLine[1] = NULL;
	m_pOutLine[2] = NULL;
	m_pOutLine[3] = NULL;

	m_siSetMode = GOONGZO_SETMODE_NONE;
}

CNGongzoDlg::~CNGongzoDlg()
{
	SetTimerOnOff(false)	;

	SI32 i;
	for(i=0;i<33;++i)
		SAFE_DELETE(m_pStatic[i]);
	for(i=0;i<19;++i)
		SAFE_DELETE(m_pButton[i]);
	for(i=0;i<8;++i)
		SAFE_DELETE(m_pEdit[i]);
	
	SAFE_DELETE(m_pOutLine[0]);
	SAFE_DELETE(m_pOutLine[1]);
	SAFE_DELETE(m_pOutLine[2]);
	SAFE_DELETE(m_pOutLine[3]);

	SAFE_DELETE(m_pButton_Weapon);
	SAFE_DELETE(m_pButton_Armor);
}

void CNGongzoDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NGongzoDlg/DLG_Gongzo.ddf"));
		file.CreatePopupDialog( this, NGONGZO_DLG, TEXT("dialog_Gongzo"), StaticCallBackDialogNGongzoDlg );
		
		

		SI32 i;
		for(i=0;i<33;++i)
			NEWCSTATIC(m_pStatic[i]);
		for(i=0;i<19;++i)
			NEWCBUTTON(m_pButton[i]);
		for(i=0;i<8;++i)
			NEWCEDIT(m_pEdit[i]);

 		TCHAR buffer[20] = TEXT("");
		for(i=0;i<33;++i)
		{
			StringCchPrintf(buffer, 20, TEXT("statictext_NONAME%d"), i+1);
			file.CreateControl( m_pStatic[i], NGONGZO_DIALOG_STATIC1 + i, buffer );
		}

		if ( pclClient->siServiceArea == ConstServiceArea_English
			|| pclClient->siServiceArea == ConstServiceArea_NHNChina
			|| pclClient->siServiceArea == ConstServiceArea_EUROPE
		)
			m_pStatic[6]->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );



		for(i=0;i<19;++i)
		{
			StringCchPrintf(buffer, 20, TEXT("button_NONAME%d"), i+1);
			file.CreateControl( m_pButton[i], NGONGZO_DIALOG_BUTTON1 + i, buffer );
		}

		NEWCBUTTON(m_pButton_Weapon);
		NEWCBUTTON(m_pButton_Armor);
		
		if(pclClient->IsCountrySwitch(Switch_GongZoArmorPercent))
		{
			file.CreateControl( m_pButton_Weapon, NGONGZO_DIALOG_BUTTON_WEAPON, TEXT("button_makeweapon") );
			file.CreateControl( m_pButton_Armor, NGONGZO_DIALOG_BUTTON_ARMOR,	TEXT("button_makearmor") );
		}

		for(i=0;i<8;++i)
		{
			StringCchPrintf(buffer, 20, TEXT("editbox_NONAME%d"), i+1);
			file.CreateControl( m_pEdit[i], NGONGZO_DIALOG_EDIT1 + i, buffer );
		}

		//_LEON_ENGLISH
		m_pStatic[1]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_WORDBREAK);

		cltClient *pclclient = (cltClient*)pclClient;

		
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER6 )
		{
			m_bGongzoCheck = TRUE;
			
			m_pEdit[0]->SetReadOnly(FALSE);
			EnableButton(m_pButton[0], FALSE);
			EnableButton(m_pButton[1], FALSE);

			for(i=3;i<18;++i)
				EnableButton(m_pButton[i], TRUE);

            SetTimerOnOff( true );
			SetActionTimer( 500 );
		}
		else
		{
			m_bGongzoCheck = FALSE;

			for(i=3;i<18;++i)
				EnableButton(m_pButton[i], FALSE);
			
			//SetTimerOnOff( true );
			//SetActionTimer( 60 );
		}

 		m_pOutLine[0] = new COutLine(this);
		file.CreateControl(m_pOutLine[0],NGONGZO_DIALOG_OUTLINE, TEXT("outline_NONAME1"));
		
		m_pOutLine[1] = new COutLine(this);
		file.CreateControl(m_pOutLine[1],NGONGZO_DIALOG_OUTLINE, TEXT("outline_NONAME2"));

		m_pOutLine[2] = new COutLine(this);
		file.CreateControl(m_pOutLine[2],NGONGZO_DIALOG_OUTLINE, TEXT("outline_NONAME3"));

		m_pOutLine[3] = new COutLine(this);
		file.CreateControl(m_pOutLine[3],NGONGZO_DIALOG_OUTLINE, TEXT("outline_NONAME4"));
	

		cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENGONGZODLG);

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pButton[0]->Show(false);
		}
		m_pEdit[7]->SetReadOnly( true );
		
		// 기본 모드
		m_siSetMode = GOONGZO_SETMODE_WEAPON;
	}
	else
	{
		DeleteDialog();
	}
}

void CNGongzoDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if(m_bGongzoCheck)
	{
		TCHAR buffer[256] = TEXT("") ;
	//	GetDlgItemText(hDlg,IDC_EDIT_GONGZO,buffer,256);
		MStrCpy(buffer, m_pEdit[0]->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2049);
		StringCchPrintf(buffer, 256, pText,m_szGongzoName,limit);
	//	SetWindowText(m_hGroup,buffer);
		m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER);

	//	if ( SendGongzoMessage() )
	//	{
	//		cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
	//		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	//	}
	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

void CALLBACK CNGongzoDlg::StaticCallBackDialogNGongzoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNGongzoDlg *pThis = (CNGongzoDlg *)pControllerMgr;
	pThis->CallBackDialogNGongzoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNGongzoDlg::CallBackDialogNGongzoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NGONGZO_DIALOG_BUTTON1:	// 꼼지가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szGongzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					/*
					cltGameMsgRequest_Userid clUserid( m_szGongzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON2:	// 귓말하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen(m_szGongzoName) > 0 && m_szGongzoName != NULL )
						pclclient->RequestSecretChatByCharName(m_szGongzoName);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON3:	// 나가기
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

	case NGONGZO_DIALOG_BUTTON_WEAPON:
		{
			m_siSetMode = GOONGZO_SETMODE_WEAPON;
			// 컨트롤 제어
			SetSpinButton();			
		}
		break;

	case NGONGZO_DIALOG_BUTTON_ARMOR:
		{
			m_siSetMode = GOONGZO_SETMODE_ARMOR;
			SetSpinButton();
		}
		break;

	case NGONGZO_DIALOG_BUTTON4:	// 검올리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					UpWeaponExp(1);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON5:	// 검내리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DownWeaponExp(1);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON6:	// 창올리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	
				{
					UpWeaponExp(2);
				}
				break;
			}
		}
		break;


	case NGONGZO_DIALOG_BUTTON7:	// 창내리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DownWeaponExp(2);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON8:	// 지팡이 올리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					UpWeaponExp(3);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON9:	// 지팡이 내리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DownWeaponExp(3);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON10:	// 도끼 올리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					UpWeaponExp(4);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON11:	// 도끼 내리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DownWeaponExp(4);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON12:	// 활 올리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					UpWeaponExp(5);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON13:	// 활 내리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DownWeaponExp(5);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON14:	// 총 올리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					UpWeaponExp(6);
				}
				break;
			}
		}
		break;

	case NGONGZO_DIALOG_BUTTON15:	// 총 내리기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DownWeaponExp(6);
				}
				break;
			}
		}
		break;
	case NGONGZO_DIALOG_BUTTON16:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetCraftFund( 5 );
				}
				break;
			}
		}
		break;
	case NGONGZO_DIALOG_BUTTON17:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetCraftFund( -5 );
				}
				break;
			}
		}
		break;
	case NGONGZO_DIALOG_BUTTON18:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendGongzoMessage();
				}
				break;
			}
		}
		break;
	case NGONGZO_DIALOG_BUTTON19:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->ShowHelpWindow(TEXT("NULL"));
			}
		}
		break;
	}
}

void CNGongzoDlg::SetGongzo(CMinisterBase *pMini,SI32 pricetime,SI16 leftovervalue,GongzoMember *gongzomember,SI16 leftovervalue_armor)
{
	cltClient *pclclient = (cltClient*)pclClient;

	m_bSetSpin = FALSE ;
	m_bMonthOver = TRUE ;

	m_siPriceTime = pricetime ;
	m_siLeftOverValue = leftovervalue ;
	m_siLeftOverValue_Armor = leftovervalue_armor;

	TCHAR buffer[256]=TEXT("");
	// 날짜를 구한다.
	cltDate cldate;
	cldate.MakeFullDateFromDateVary(pricetime);

#if 1 //KHY - 0607 시간 삭제.
	if(cldate.GetRealDateText(buffer) == false)
#else
	if(cldate.GetDateText(buffer) == false)
#endif
	{
		return ;
	}

	m_pStatic[3]->SetText(buffer, DT_LEFT | DT_VCENTER);

	if ( m_bSet )
	{
		m_bSet = FALSE ;

		TCHAR* pTitle = GetTxtFromMgr(2045);
		TCHAR* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return ;
	}

	if( m_bSoundCheck == FALSE)
	{
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GONGZO")),0,0);
		m_bSoundCheck = TRUE;
	}

	StringCchCopy(m_szGongzoID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szGongzoName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szGongzoProfile, 256, pMini->m_szGreeting);

	TCHAR* pText = GetTxtFromMgr(2050);
	StringCchPrintf(buffer, 256, pText,m_szGongzoName);
	SetTitle(buffer);
	m_pEdit[0]->SetText(m_szGongzoProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
	m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER);

	for ( int i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
	{
		m_stGongzoMember.siPriceValue[i] = gongzomember->siPriceValue[i] ;
		m_stGongzoMember.siSkillValue[i] = gongzomember->siSkillValue[i] ;
	}

	m_stGongzoMember.siCraftFund = gongzomember->siCraftFund;

	if ( pclclient->clClientDate.GetDateVary() < m_siPriceTime + 30 ||
		pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER6 )
		m_bMonthOver = FALSE ;

	SetSpinDis(m_bMonthOver);

	SetSpinButton();
}

void CNGongzoDlg::SendGongzoMessage()
{

	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR buffer[16] = TEXT("") ;

	SetSpinDis(m_bMonthOver);

	if ( m_siLeftOverValue < 0 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2053);
		TCHAR* pText = GetTxtFromMgr(2054);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	if ( m_siLeftOverValue_Armor < 0 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2053);
		TCHAR* pText = GetTxtFromMgr(2054);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	TCHAR* pText = GetTxtFromMgr(2052);
	switch(m_siSetMode)
	{
	case GOONGZO_SETMODE_WEAPON:	StringCchPrintf(buffer, 256, pText,m_siLeftOverValue);			break;
	case GOONGZO_SETMODE_ARMOR:		StringCchPrintf(buffer, 256, pText,m_siLeftOverValue_Armor);	break;
	}
	m_pStatic[5]->SetText(buffer, DT_CENTER | DT_VCENTER);

	MStrCpy(m_szGongzoProfile, m_pEdit[0]->GetText(), 256);
	SI32 limit = _tcslen(m_szGongzoProfile);
	if ( limit <= 0 || limit > 255 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2024);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	TCHAR* pTitle = GetTxtFromMgr(2055);
	pText = GetTxtFromMgr(2056);

	stMsgBox MsgBox;
	MsgBox.Set( this, pText, pTitle, MSGBOX_TYPE_YESNO, 0 );
	BYTE TempBuffer[ 512 ];
	SI16 Size = sizeof( m_szGongzoProfile ) + sizeof(m_siLeftOverValue) + sizeof(m_stGongzoMember.siPriceValue);
	memcpy( TempBuffer, &m_szGongzoProfile, sizeof( m_szGongzoProfile ) );
	memcpy( &TempBuffer[ sizeof( m_szGongzoProfile ) ] , &m_siLeftOverValue, sizeof(m_siLeftOverValue) );
	memcpy( &TempBuffer[ sizeof( m_szGongzoProfile ) + sizeof(m_siLeftOverValue) ] , &m_stGongzoMember.siPriceValue, sizeof(m_stGongzoMember.siPriceValue) );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );
}

void CNGongzoDlg::SetSpinButton()
{
	TCHAR buffer[16] = TEXT("") ;
	TCHAR* pText = GetTxtFromMgr(2052);
	NTCHARString64	kText;

	switch(m_siSetMode)
	{
	case GOONGZO_SETMODE_WEAPON:
		{
			SetSpinData(GONGZO_SKILL_SWORD, 1, 8);
			SetSpinData(GONGZO_SKILL_SPEAR, 2, 10);
			SetSpinData(GONGZO_SKILL_STAFF, 3, 12);
			SetSpinData(GONGZO_SKILL_AXE,	4, 14);
			SetSpinData(GONGZO_SKILL_BOW,	5, 16);
			SetSpinData(GONGZO_SKILL_GUN,	6, 18);

			pText = GetTxtFromMgr(2052);
			StringCchPrintf(buffer, 16, pText,m_siLeftOverValue);
			m_pStatic[5]->SetText(buffer, DT_CENTER | DT_VCENTER);

			kText = GetTxtFromMgr(4115);		m_pStatic[19]->SetText(kText);
			kText = GetTxtFromMgr(4117);		m_pStatic[23]->SetText(kText);
			kText = GetTxtFromMgr(4119);		m_pStatic[27]->SetText(kText);
			kText = GetTxtFromMgr(4116);		m_pStatic[21]->SetText(kText);
			kText = GetTxtFromMgr(4118);		m_pStatic[25]->SetText(kText);
			kText = GetTxtFromMgr(4120);		m_pStatic[29]->SetText(kText);

			m_pButton[9]->Show(true);
			m_pButton[10]->Show(true);
			m_pButton[13]->Show(true);
			m_pButton[14]->Show(true);

			m_pEdit[4]->Show(true);
			m_pEdit[6]->Show(true);

			m_pStatic[26]->Show(true);
			m_pStatic[30]->Show(true);
		}
		break;
	case GOONGZO_SETMODE_ARMOR:
		{
			SetSpinData(GONGZO_SKILL_HELMET, 1, 8);
			SetSpinData(GONGZO_SKILL_ARMOR, 3, 10);
			SetSpinData(GONGZO_SKILL_BELT, 5, 12);
			SetSpinData(GONGZO_SKILL_SHOES,	2, 14);
			SetSpinData(GONGZO_SKILL_RESERVED1,	4, 14);
			SetSpinData(GONGZO_SKILL_RESERVED2,	6, 14);

			pText = GetTxtFromMgr(2052);
			StringCchPrintf(buffer, 16, pText,m_siLeftOverValue_Armor);
			m_pStatic[5]->SetText(buffer, DT_CENTER | DT_VCENTER);

			kText = GetTxtFromMgr(8246);		m_pStatic[19]->SetText(kText);
			kText = GetTxtFromMgr(8248);		m_pStatic[23]->SetText(kText);
			kText = GetTxtFromMgr(8247);		m_pStatic[27]->SetText(kText);
			kText = GetTxtFromMgr(8249);		m_pStatic[21]->SetText(kText);
			kText = TEXT("");					m_pStatic[25]->SetText(kText);
			kText = TEXT("");					m_pStatic[29]->SetText(kText);

			m_pButton[9]->Show(false);
			m_pButton[10]->Show(false);
			m_pButton[13]->Show(false);
			m_pButton[14]->Show(false);

			m_pEdit[4]->Show(false);
			m_pEdit[6]->Show(false);

			m_pStatic[26]->Show(false);
			m_pStatic[30]->Show(false);
		}
		break;
	}

	SI64 craftfund = m_stGongzoMember.siCraftFund;
	StringCchPrintf(buffer, 16, TEXT("%I64d"), craftfund );

	m_pEdit[7]->SetText( buffer );
}

void CNGongzoDlg::SetSpinDis(BOOL bView)
{
	SI32 i;
	for(i=3;i<17;++i)
		EnableButton(m_pButton[i], bView);

	if ( m_bSetSpin ) m_bSetSpin = FALSE ;
}

void CNGongzoDlg::SetSpinData(SI32 skillunique, SI32 editunique,SI32 staticunique)
{
	TCHAR buffer[16] = TEXT("") ;

	if(skillunique < 7)
	{
		StringCchPrintf(buffer, 16, TEXT("%d"), m_stGongzoMember.siSkillValue[skillunique] );
		StringCchCat(buffer, 16, TEXT("%"));
		m_pStatic[staticunique]->SetText(buffer, DT_CENTER | DT_VCENTER);
	}

	StringCchPrintf(buffer, 16, TEXT("%d"), m_stGongzoMember.siPriceValue[skillunique] );
	m_pEdit[editunique]->SetText(buffer);
}


void CNGongzoDlg::SetSpinValue(SI32 skillunique,SI32 editunique)
{
	m_bSetSpin = TRUE ;
	m_siSetSkill = skillunique;
	m_siIDCEditNum = editunique;

	//for( SI16 i=3; i<15 ; ++i )
	//	EnableButton( m_pButton[i], FALSE );
}

void CNGongzoDlg::UpWeaponExp(SI32 Index)
{
	SetSpinValue(Index, Index);

	if ( m_bSetSpin && m_bMonthOver )
	{
		TCHAR buffer[10] = TEXT("");
		SI32 TempValue;


		TempValue = _tstoi(m_pEdit[Index]->GetText());

		TempValue += 5;
		if( TempValue > 50 )
		{
			m_bMonthOver = TRUE;
			return;
		}
		_itot(TempValue, buffer, 10);
		m_pEdit[Index]->SetText(buffer);
		
		// 값 저장
		SI32 siSkill = Index;
		switch(m_siSetMode)
		{
		case GOONGZO_SETMODE_WEAPON:									break;
		case GOONGZO_SETMODE_ARMOR:		siSkill += GONGZO_SKILL_GUN;	break;
		}
	
		m_stGongzoMember.siPriceValue[siSkill] = TempValue;

		switch(m_siSetMode)
		{
		case GOONGZO_SETMODE_WEAPON:	m_siLeftOverValue -= 5;			break;
		case GOONGZO_SETMODE_ARMOR:		m_siLeftOverValue_Armor -= 5;	break;
		}

		TCHAR szBuffer[256];
		szBuffer[0] = 0;

		switch(m_siSetMode)
		{
		case GOONGZO_SETMODE_WEAPON:	StringCchPrintf( szBuffer , 256 , TEXT("%d") , m_siLeftOverValue );			break;
		case GOONGZO_SETMODE_ARMOR:		StringCchPrintf( szBuffer , 256 , TEXT("%d") , m_siLeftOverValue_Armor );	break;
		}

		m_pStatic[5]->SetText( szBuffer );
		
	}
}

void CNGongzoDlg::DownWeaponExp(SI32 Index)
{
	SetSpinValue(Index, Index);

	if ( m_bSetSpin && m_bMonthOver )
	{
		TCHAR buffer[10] = TEXT("");
		SI32 TempValue;

//		m_bMonthOver = FALSE ;

		TempValue = _tstoi(m_pEdit[Index]->GetText());

		TempValue -= 5;
		if( TempValue < 0 )
		{
			m_bMonthOver = TRUE;
			return;
		}
		_itot(TempValue, buffer, 10);
		m_pEdit[Index]->SetText(buffer);

		// 값 저장
		SI32 siSkill = Index;
		switch(m_siSetMode)
		{
		case GOONGZO_SETMODE_WEAPON:									break;
		case GOONGZO_SETMODE_ARMOR:		siSkill += GONGZO_SKILL_GUN;	break;
		}

		m_stGongzoMember.siPriceValue[siSkill] = TempValue;

		switch(m_siSetMode)
		{
		case GOONGZO_SETMODE_WEAPON:	m_siLeftOverValue += 5;				break;
		case GOONGZO_SETMODE_ARMOR:		m_siLeftOverValue_Armor += 5;		break;
		}
		
		TCHAR szBuffer[256];
		szBuffer[0] = 0;


		switch(m_siSetMode)
		{
		case GOONGZO_SETMODE_WEAPON:	StringCchPrintf( szBuffer , 256 , TEXT("%d") , m_siLeftOverValue );			break;
		case GOONGZO_SETMODE_ARMOR:		StringCchPrintf( szBuffer , 256 , TEXT("%d") , m_siLeftOverValue_Armor );	break;
		}

		m_pStatic[5]->SetText( szBuffer );

	}
}

void CNGongzoDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNGongzoDlg::SetButtonTextColor(CButton* pButton, BOOL Flag)
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

void CNGongzoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				cltGameMsgRequest_SetGongzoInfo clinfo(m_szGongzoProfile, m_siLeftOverValue, &m_stGongzoMember.siPriceValue[0], m_stGongzoMember.siCraftFund, m_siLeftOverValue_Armor );
				cltMsg clMsg(GAMEMSG_REQUEST_SETGONGZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				m_bSet = TRUE ;
				m_bGongzoReturnValue = TRUE;
			}
			else
			{
				m_bGongzoReturnValue = FALSE;
				if ( !m_bGongzoReturnValue )
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}

		}
		break;
	}
}
void CNGongzoDlg::SetCraftFund(SI32 inc)
{
	cltClient* pclclient = (cltClient*)pclClient;

	m_stGongzoMember.siCraftFund += inc;

	// 0~100 사이
	m_stGongzoMember.siCraftFund = max( ConstCraftFund_Min, min( ConstCraftFund_Max, m_stGongzoMember.siCraftFund ) );
	TCHAR buf[16] = {0,};
	StringCchPrintf( buf, 16, TEXT("%I64d"), m_stGongzoMember.siCraftFund );
	m_pEdit[7]->SetText( buf );
}
