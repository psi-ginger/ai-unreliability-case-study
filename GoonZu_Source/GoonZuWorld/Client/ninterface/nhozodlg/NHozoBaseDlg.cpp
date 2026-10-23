#include "../Client/NInterface/NHozoDlg/NHozoBaseDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoResidentDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoPrizeDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoChangedNameDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoFixazionDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoNewCharRateDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoAngelApplyerDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoAngelDlg.h"
#include "../Client/NInterface/NHozoDlg/NHozoAppointmentDlg.h"

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

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

//-----------------------------
// Common
//-----------------------------
#include "../common/PersonInfo.h"
#include "../common/HintDlg/HintDlg.h"
#include "../common/Char/CharManager/CharManager.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/Msg/MsgType-Web.h"
#include "../common/Char/CharCommon/Char-Common.h"


#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNHozoBaseDlg::CNHozoBaseDlg() : CDialogController()
{
	m_bHozoCheck = FALSE; // FALSE 일반인

	StringCchCopy(m_szHozoName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szHozoProfile, 256, TEXT(""));
	StringCchCopy(m_szHozoID, MAX_PLAYER_NAME, TEXT(""));

	m_siPrizeMoney = 0 ;
	m_siPrizeSetTime = 0 ;
	m_siFixationMoney = 0 ;

	m_bSet = FALSE;
	m_siListPage = 0;

	m_pStatic = NULL;
	m_pEdit = NULL;
	for(SI32 i=0;i<12;++i)
		m_pButton[i] = NULL;

	m_pNHozoResidentDlg = NULL;
	m_pNHozoPrizeDlg	= NULL;
//	m_pNHozoChangedNameDlg = NULL;
	m_pNHozoFixationDlg	= NULL;
	m_pNHozoNewCharRateDlg = NULL;

	m_pNHozoAppointmentDlg	= NULL;
	m_pNHozoAngelDlg = NULL;
	m_pNHozoAngelApplyerDlg = NULL;
	m_pNHozoBankruptcyDlg = NULL;
}

CNHozoBaseDlg::~CNHozoBaseDlg()
{
	SetTimerOnOff(false);

	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pEdit);
	for(SI32 i=0;i<12;++i)
		SAFE_DELETE(m_pButton[i]);

	SAFE_DELETE(m_pNHozoBankruptcyDlg);
	SAFE_DELETE(m_pNHozoResidentDlg);
	SAFE_DELETE(m_pNHozoPrizeDlg);
//	SAFE_DELETE(m_pNHozoChangedNameDlg);
	SAFE_DELETE(m_pNHozoFixationDlg);
	SAFE_DELETE(m_pNHozoNewCharRateDlg);

	SAFE_DELETE(m_pNHozoAppointmentDlg);
	SAFE_DELETE(m_pNHozoAngelDlg);
	SAFE_DELETE(m_pNHozoAngelApplyerDlg);
}

void CNHozoBaseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_Base.ddf"));
		file.CreatePopupDialog( this, NHOZOBASE_DLG, TEXT("dialog_Hozo_Base"), StaticCallBackDialogNHozoBaseDlg );

		m_pStatic = new CStatic( this );
		m_pEdit = new CEdit( this );

		for(SI32 i=0;i<12;++i)
			m_pButton[i] = new CButton( this );


		file.CreateControl( m_pStatic, NHOZOBASE_DIALOG_STATIC, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pEdit, NHOZOBASE_DIALOG_EDIT, TEXT("editbox_NONAME1") );

		file.CreateControl( m_pButton[0], NHOZOBASE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NHOZOBASE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NHOZOBASE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NHOZOBASE_DIALOG_BUTTON4, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NHOZOBASE_DIALOG_BUTTON5, TEXT("button_NONAME5") );
		file.CreateControl( m_pButton[5], NHOZOBASE_DIALOG_BUTTON6, TEXT("button_NONAME6") );
		file.CreateControl( m_pButton[6], NHOZOBASE_DIALOG_BUTTON7, TEXT("button_NONAME7") );
		file.CreateControl( m_pButton[7], NHOZOBASE_DIALOG_BUTTON8, TEXT("button_NONAME8") );
		file.CreateControl( m_pButton[8], NHOZOBASE_DIALOG_BUTTON9, TEXT("button_NONAME9") );
		file.CreateControl( m_pButton[9], NHOZOBASE_DIALOG_BUTTON10, TEXT("button_NONAME10") );
		file.CreateControl( m_pButton[10], NHOZOBASE_DIALOG_BUTTON11, TEXT("button_NONAME11") );
		file.CreateControl( m_pButton[11], NHOZOBASE_DIALOG_BUTTON12, TEXT("button_NONAME12") );


		if( pclClient->siServiceArea == ConstServiceArea_Korea )
		{
			m_pButton[2]->Show(false);	// 공작(호조) 이란 버튼 한국은 막음
			m_pButton[11]->Show(false);
		}

		m_pButton[5]->Show(false);  //이름 변경 현황.

		if(pclClient->IsCountrySwitch(Switch_Button6HozoDlg)) //KHY -1001-  버튼 제어 국가제한.
		{		
			m_pButton[5]->Show(TRUE);  //이름 변경 현황.
		}
		// 버튼 색상주기
		SetButtonTextColor(3);
//////////////////////////////////////////////////////////////////////////
//	CHILD DLG 
		m_pNHozoResidentDlg = new CNHozoResidentDlg();
		m_pNHozoResidentDlg->Create( this );
		m_pNHozoResidentDlg->Show();

		m_pNHozoPrizeDlg = new CNHozoPrizeDlg();
		m_pNHozoPrizeDlg->Create( this );

//		m_pNHozoChangedNameDlg = new CNHozoChangedNameDlg();
//		m_pNHozoChangedNameDlg->Create( this );
		
		m_pNHozoFixationDlg = new CNHozoFixazionDlg();
		m_pNHozoFixationDlg->Create( this );

		m_pNHozoNewCharRateDlg = new CNHozoNewCharRateDlg();
		m_pNHozoNewCharRateDlg->Create( this );

		m_pNHozoAngelDlg = new CNHozoAngelDlg();
		m_pNHozoAngelDlg->Create(this);

		m_pNHozoAngelApplyerDlg = new CNHozoAngelApplyerDlg();
		m_pNHozoAngelApplyerDlg->Create( this );

		m_pNHozoAppointmentDlg = new CNHozoAppointmentDlg();
		m_pNHozoAppointmentDlg->Create();

		m_pNHozoBankruptcyDlg = new CNHozoBankruptcyDlg;
		m_pNHozoBankruptcyDlg->Create(this);
		
//////////////////////////////////////////////////////////////////////////

		cltClient *pclclient = (cltClient *)pclClient;

		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER2 )
		{	
			m_bHozoCheck = TRUE;
			
			m_pEdit->SetReadOnly(false);

			m_pNHozoResidentDlg->m_pEdit[0]->SetReadOnly(FALSE);
			m_pNHozoResidentDlg->m_pEdit[1]->SetReadOnly(FALSE);

			m_pNHozoPrizeDlg->m_pEdit->SetReadOnly(FALSE);

			m_pNHozoFixationDlg->m_pEdit->SetReadOnly(FALSE);
			if(pclClient->IsCountrySwitch(Switch_TutorialVillage))
			{
				m_pNHozoFixationDlg->m_pEdit_ankarandividend->SetReadOnly(FALSE);
			}

			m_pNHozoResidentDlg->m_pEdit[0]->SetTextPrintOption(DT_LEFT | DT_VCENTER);
			m_pNHozoResidentDlg->m_pEdit[1]->SetTextPrintOption(DT_LEFT | DT_VCENTER);
			m_pNHozoPrizeDlg->m_pEdit->SetTextPrintOption( DT_LEFT | DT_VCENTER);
			m_pNHozoFixationDlg->m_pEdit->SetTextPrintOption( DT_LEFT | DT_VCENTER);

			EnableButton(m_pButton[0], FALSE);	// 꼼지 가기
			EnableButton(m_pButton[1], FALSE);	// 귓말 하기

			SetTimerOnOff( true );
			SetActionTimer( 500 );
		}
		else
		{
			m_bHozoCheck = FALSE;

			EnableButton(m_pButton[10], false);	

			m_pNHozoResidentDlg->m_pEdit[0]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
			m_pNHozoResidentDlg->m_pEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
			m_pNHozoPrizeDlg->m_pEdit->SetTextPrintOption( DT_RIGHT | DT_VCENTER);
			m_pNHozoFixationDlg->m_pEdit->SetTextPrintOption( DT_RIGHT | DT_VCENTER);

			//SetTimerOnOff( true );
			//SetActionTimer( 60 );
		}


		cltMsg clMsg(GAMEMSG_REQUEST_GETHOZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		
		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENHOZODLG);

		Show( true );

		//KHY - 0220 -  푸치코미 옵션 삭제.
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

void CNHozoBaseDlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(TRUE == m_bHozoCheck)
	{
		TCHAR buffer[256] = TEXT("") ;
		MStrCpy(buffer, m_pEdit->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2074);
		StringCchPrintf(buffer, 256, pText,m_szHozoName,limit);
		m_pStatic->SetText(buffer, DT_LEFT);	// 호조 내용 얼마나 적었나 표시
	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETHOZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

void CALLBACK CNHozoBaseDlg::StaticCallBackDialogNHozoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoBaseDlg *pThis = (CNHozoBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoBaseDlg::CallBackDialogNHozoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON1:	// 꼼지 가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szHozoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					/*
					cltGameMsgRequest_Userid clUserid( m_szHozoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON2:	// 귓말 하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen(m_szHozoName) > 0 && m_szHozoName != NULL )
						pclclient->RequestSecretChatByCharName(m_szHozoName);
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON3:	// 호조판서란?
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					
//					if ( g_SoundOnOffDlg.m_bFullScreenMode )
//						{
					pclclient->ShowHelpWindow(TEXT("hozo"));
							//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
							//{
							//	pclclient->CreateInterface( NHINT_DLG );
							//}
							//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("hozo"));

//						}
//					else
//						pclclient->m_pHintDlg->Show(TEXT("hozo"));
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON4:	// 마을 관련 설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(3);

					m_pNHozoResidentDlg->Show();
					m_pNHozoPrizeDlg->Hide();
					m_pNHozoBankruptcyDlg->Hide();
					m_pNHozoFixationDlg->Hide();
					m_pNHozoNewCharRateDlg->Hide();

					m_pNHozoAngelDlg->Hide();
					m_pNHozoAngelApplyerDlg->Hide();
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON5:	// 추천인 관련 설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(4);

					m_pNHozoResidentDlg->Hide();
					m_pNHozoPrizeDlg->Show();
					m_pNHozoBankruptcyDlg->Hide();
					m_pNHozoFixationDlg->Hide();
					m_pNHozoNewCharRateDlg->Hide();

					m_pNHozoAngelDlg->Hide();
					m_pNHozoAngelApplyerDlg->Hide();
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON6:	// 이름 변경 현황
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(5);

					m_pNHozoResidentDlg->Hide();
					m_pNHozoPrizeDlg->Hide();
					m_pNHozoBankruptcyDlg->Show();
					m_pNHozoFixationDlg->Hide();
					m_pNHozoNewCharRateDlg->Hide();


					m_pNHozoAngelDlg->Hide();
					m_pNHozoAngelApplyerDlg->Hide();

					// 리스트를 불러온다.
				//	cltGameMsgRequest_GetChangedName clinfo(1);
				//	cltMsg clMsg( GAMEMSG_REQUEST_GETCHANGEDNAME, sizeof( clinfo ), (BYTE*)&clinfo );
				//	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON7:	// 신규 유저 관련 설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(6);

					m_pNHozoResidentDlg->Hide();
					m_pNHozoPrizeDlg->Hide();
					m_pNHozoBankruptcyDlg->Hide();
					m_pNHozoFixationDlg->Show();
					m_pNHozoNewCharRateDlg->Hide();

					m_pNHozoAngelDlg->Hide();
					m_pNHozoAngelApplyerDlg->Hide();

				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON8:	// 신규 유저 유입비율
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(7);

					m_pNHozoResidentDlg->Hide();
					m_pNHozoPrizeDlg->Hide();
					m_pNHozoBankruptcyDlg->Hide();
					m_pNHozoFixationDlg->Hide();
					m_pNHozoNewCharRateDlg->Show();

					m_pNHozoAngelDlg->Hide();
					m_pNHozoAngelApplyerDlg->Hide();
				}
				break;
			}
		}
		break;
	case NHOZOBASE_DIALOG_BUTTON9:	// 수호천사 선정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(8);

					m_pNHozoResidentDlg->Hide();
					m_pNHozoPrizeDlg->Hide();
					m_pNHozoBankruptcyDlg->Hide();
					m_pNHozoFixationDlg->Hide();
					m_pNHozoNewCharRateDlg->Hide();

					m_pNHozoAngelDlg->Show();
					m_pNHozoAngelApplyerDlg->Hide();
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON10:	// 수호천사 신청
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetButtonTextColor(9);

					m_pNHozoResidentDlg->Hide();
					m_pNHozoPrizeDlg->Hide();
					m_pNHozoBankruptcyDlg->Hide();
					m_pNHozoFixationDlg->Hide();
					m_pNHozoNewCharRateDlg->Hide();

					m_pNHozoAngelDlg->Hide();
					m_pNHozoAngelApplyerDlg->Show();
				}
				break;
			}
		}
		break;

	case NHOZOBASE_DIALOG_BUTTON11:	// 설정하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[8] = TEXT("") ;

				//	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_RESIDENT );
				//	GetWindowText( hWnd, buffer, 8 );
					MStrCpy(buffer, m_pNHozoResidentDlg->m_pEdit[0]->GetText(),8);
					SI32 residentnum = _tstoi(buffer);
					if ( residentnum < 100 || residentnum > 150 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2019);
						TCHAR* pText = GetTxtFromMgr(2075);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					MStrCpy(buffer, m_pNHozoResidentDlg->m_pEdit[1]->GetText(),8);
					SI32 maxshop = _tstoi(buffer);
					if ( maxshop < 0 || maxshop > 40 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2019);
						TCHAR* pText = GetTxtFromMgr(2075);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					MStrCpy(buffer, m_pNHozoPrizeDlg->m_pEdit->GetText(), 8);
				
					SI32 prizemoney = _tstoi(buffer) / UNIT_HOZO_SETMONEY ;
					if ( TABS( m_siPrizeMoney - prizemoney ) > 1 )
					{
						TCHAR moneybuf[256] = TEXT("") ;
						TCHAR errorbuf[256] = TEXT("") ;

						TCHAR* pText = GetTxtFromMgr(611);
						g_JYLibFunction.SetNumUnit(UNIT_HOZO_SETMONEY,moneybuf, 256, pText);
						pText = GetTxtFromMgr(2077);
						StringCchPrintf(errorbuf, 256, pText,moneybuf);
						pText = GetTxtFromMgr(2078);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
						break;
					}
					if ( prizemoney < MIN_HOZO_PRIZEMONEY )
					{
						TCHAR moneybuf1[256] = TEXT("") ;
						TCHAR moneybuf2[256] = TEXT("") ;
						TCHAR errorbuf[256] = TEXT("") ;

						TCHAR* pText = GetTxtFromMgr(611);
						g_JYLibFunction.SetNumUnit(MIN_HOZO_PRIZEMONEY * UNIT_HOZO_SETMONEY,moneybuf1, 256, pText);
						g_JYLibFunction.SetNumUnit(MAX_HOZO_PRIZEMONEY * UNIT_HOZO_SETMONEY,moneybuf2, 256, pText);
						pText = GetTxtFromMgr(2079);
						StringCchPrintf(errorbuf, 256, pText,moneybuf1,moneybuf2);
						pText = GetTxtFromMgr(2019);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
						break;
					}

					MStrCpy(buffer, m_pNHozoFixationDlg->m_pEdit->GetText(), 8);
					SI32 fixationmoney = _tstoi(buffer) / UNIT_HOZO_SETMONEY ;
					if ( TABS( m_siFixationMoney - fixationmoney ) > 1 )
					{
						TCHAR moneybuf[256] = TEXT("") ;
						TCHAR errorbuf[256] = TEXT("") ;

						TCHAR* pText = GetTxtFromMgr(611);
						g_JYLibFunction.SetNumUnit(UNIT_HOZO_SETMONEY,moneybuf, 256, pText);
						pText = GetTxtFromMgr(2080);
						StringCchPrintf(errorbuf, 256, pText,moneybuf);
						pText = GetTxtFromMgr(2078);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
						break;
					}
					if ( fixationmoney < MIN_HOZO_FIXATIONMONEY )
					{
						TCHAR moneybuf1[256] = TEXT("") ;
						TCHAR moneybuf2[256] = TEXT("") ;
						TCHAR errorbuf[256] = TEXT("") ;

						TCHAR* pText = GetTxtFromMgr(611);
						g_JYLibFunction.SetNumUnit(MIN_HOZO_FIXATIONMONEY * UNIT_HOZO_SETMONEY,moneybuf1, 256, pText);
						g_JYLibFunction.SetNumUnit(MAX_HOZO_FIXATIONMONEY * UNIT_HOZO_SETMONEY,moneybuf2, 256, pText);
						pText = GetTxtFromMgr(2081);
						StringCchPrintf(errorbuf, 256, pText,moneybuf1,moneybuf2);
						pText = GetTxtFromMgr(2063);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
						break;
					}

					SI32 ankarandividendmoney = 0;
					if(pclClient->IsCountrySwitch(Switch_TutorialVillage))
					{
						MStrCpy(buffer, m_pNHozoFixationDlg->m_pEdit_ankarandividend->GetText(), 8);
						ankarandividendmoney = _tstoi(buffer) / UNIT_HOZO_SETMONEY ;
						if ( TABS( m_siAnkaranDividendMoney - ankarandividendmoney ) > 1 )
						{
							TCHAR moneybuf[256] = TEXT("") ;
							TCHAR errorbuf[256] = TEXT("") ;

							TCHAR* pText = GetTxtFromMgr(611);
							g_JYLibFunction.SetNumUnit(UNIT_HOZO_SETMONEY,moneybuf, 256, pText);
							pText = GetTxtFromMgr(2080);
							StringCchPrintf(errorbuf, 256, pText,moneybuf);
							pText = GetTxtFromMgr(2078);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
							break;
						}
						if ( ankarandividendmoney < MIN_HOZO_FIXATIONMONEY )
						{
							TCHAR moneybuf1[256] = TEXT("") ;
							TCHAR moneybuf2[256] = TEXT("") ;
							TCHAR errorbuf[256] = TEXT("") ;

							TCHAR* pText = GetTxtFromMgr(611);
							g_JYLibFunction.SetNumUnit(MIN_HOZO_FIXATIONMONEY * UNIT_HOZO_SETMONEY,moneybuf1, 256, pText);
							g_JYLibFunction.SetNumUnit(MAX_HOZO_FIXATIONMONEY * UNIT_HOZO_SETMONEY,moneybuf2, 256, pText);
							pText = GetTxtFromMgr(2081);
							StringCchPrintf(errorbuf, 256, pText,moneybuf1,moneybuf2);
							pText = GetTxtFromMgr(2063);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
							break;
						}
					}

					cltClient *pclclient = (cltClient*)pclClient;
					if ( pclclient->clClientDate.GetDateVary() <= m_siPrizeSetTime + 30 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2063);
						TCHAR* pText = GetTxtFromMgr(2062);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

				//	GetDlgItemText(m_hDlg,IDC_EDIT_HOZO,m_szHozoProfile,256);
					MStrCpy(m_szHozoProfile, m_pEdit->GetText(), 256);
					SI32 limit = _tcslen(m_szHozoProfile);
					if ( limit > 0 && limit < 256 )
					{
						cltGameMsgRequest_SetHozoInfo clinfo( m_szHozoProfile, residentnum,maxshop,
							prizemoney, fixationmoney, ankarandividendmoney);
						cltMsg clMsg(GAMEMSG_REQUEST_SETHOZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
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

	case NHOZOBASE_DIALOG_BUTTON12:	// 닫기
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
	}
}

void CNHozoBaseDlg::SetHozoDlg(CMinisterBase *pMini,SI32 residentnum,SI32 maxshop,SI32 totalcharnum,
							   SI32 calctime, GMONEY prizemoney,SI32 prizesettime,GMONEY fixationmoney,
							   SI32 newcharrate, SI16 BankruptcyVillageNumber, SI32 SetBankruptcyVillageDate,
							   GMONEY TutorialVillageDivideend)
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_bSet )
	{
		TCHAR* pTilte = GetTxtFromMgr(2045);
		TCHAR* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTilte, pText);
		m_bSet = FALSE ;
		return ;
	}

	StringCchCopy(m_szHozoID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szHozoName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szHozoProfile, 256, pMini->m_szGreeting);

	m_siPrizeMoney = prizemoney ;
	m_siPrizeSetTime = prizesettime ;
	m_siFixationMoney = fixationmoney ;
	m_siAnkaranDividendMoney = TutorialVillageDivideend;

	cltDate cldate;
	TCHAR buffer[256] = TEXT("") ;
	TCHAR numbuf[128] = TEXT("") ;

	// 마을 관련
	TCHAR* pText = GetTxtFromMgr(2085);
	g_JYLibFunction.SetNumUnit(totalcharnum,numbuf, 128, pText);
	m_pNHozoResidentDlg->m_pStatic[2]->SetText(numbuf, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	
	cldate.MakeFullDateFromDateVary(calctime);
#if 1 //KHY - 0607 시간 삭제.
	if(cldate.GetRealDateText(numbuf) == TRUE)
#else
	if(cldate.GetDateText(numbuf) == TRUE)
#endif
	{
		pText = GetTxtFromMgr(612);
		StringCchPrintf(buffer, 256, pText, numbuf);
		m_pNHozoResidentDlg->m_pStatic[3]->SetText(buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	cldate.MakeFullDateFromDateVary(m_siPrizeSetTime);
#if 1 //KHY - 0607 시간 삭제.
	if(cldate.GetRealDateText(numbuf) == TRUE)
#else
	if(cldate.GetDateText(numbuf) == TRUE)
#endif
	{
		pText = GetTxtFromMgr(2086);
		StringCchPrintf(buffer, 256, pText, numbuf);
		m_pNHozoResidentDlg->m_pStatic[4]->SetText(buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	StringCchPrintf(buffer, 256, TEXT("%d"),residentnum);
	m_pNHozoResidentDlg->m_pEdit[0]->SetText(buffer);
	
	StringCchPrintf(buffer, 256, TEXT("%d"),maxshop);
	m_pNHozoResidentDlg->m_pEdit[1]->SetText(buffer);
	
	// 추천인 포상
	pText = GetTxtFromMgr(3235);
	m_pNHozoPrizeDlg->m_pStatic[1]->SetText(pText, DT_LEFT | DT_VCENTER);
	StringCchPrintf(buffer, 256, TEXT("%I64d"),m_siPrizeMoney * UNIT_HOZO_SETMONEY );
	m_pNHozoPrizeDlg->m_pEdit->SetText(buffer);
	

	// 신규 유저 정착금
	pText = GetTxtFromMgr(3236);
	m_pNHozoFixationDlg->m_pStatic[1]->SetText(pText, DT_LEFT | DT_VCENTER);
	StringCchPrintf(buffer, 256, TEXT("%I64d"),m_siFixationMoney * UNIT_HOZO_SETMONEY );
	m_pNHozoFixationDlg->m_pEdit->SetText(buffer);
	
	if(pclClient->IsCountrySwitch(Switch_TutorialVillage))
	{
		// 앙카란 주주배당금
		StringCchPrintf(buffer, 256, TEXT("%I64d"),m_siAnkaranDividendMoney * UNIT_HOZO_SETMONEY );
		m_pNHozoFixationDlg->m_pEdit_ankarandividend->SetText(buffer);
	}
	// 신규 윶저 비율
	StringCchPrintf(buffer, 256, TEXT("%d"),newcharrate);
	m_pNHozoNewCharRateDlg->m_pStatic[4]->SetText(buffer, DT_CENTER | DT_VCENTER);

	pText = GetTxtFromMgr(2087);
	StringCchPrintf(buffer, 256, pText,m_szHozoName);
//	SetWindowText( m_hDlg ,buffer);
	SetTitle(buffer);	// 호조 판서 타이플 이름
//	SetDlgItemText( m_hDlg,IDC_EDIT_HOZO, m_szHozoProfile );
	m_pEdit->SetText(m_szHozoProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
//	SetWindowText(m_hGroup,buffer);
	m_pStatic->SetText(buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	// 호조판서 설명위에 이름 표시

	m_pNHozoBankruptcyDlg->SetBankCruptcyVillageInfo( BankruptcyVillageNumber, SetBankruptcyVillageDate );
}

void CNHozoBaseDlg::ShowChangedNameList(SI32 page , ChangedNameChar changednamechar [])
{
/*	if ( page <= 0 ) return ;
	if ( !m_pNHozoChangedNameDlg->IsShow() ) return ;

	m_pNHozoChangedNameDlg->SetPage(page);

	TCHAR buffer[256] = TEXT("") ;
	TCHAR* pText = GetTxtFromMgr(2084);
	StringCchPrintf(buffer, 256, pText,page) ;

	m_pNHozoChangedNameDlg->m_pEdit->SetText(buffer);

	m_pNHozoChangedNameDlg->m_pList->Clear();

	SI32 listindex = 0 ;

	for ( int i = 0 ; i < MAX_HOZO_CHANGEDNAME_LIST ; i ++ )
	{
		if ( changednamechar[i].siDate <= 0 ) continue ;

		cltDate cldate;
		cldate.MakeFullDateFromDateVary(changednamechar[i].siDate);
		if(cldate.GetDateText(buffer) == false) continue;

		stListBoxItemData Data;
		Data.Set(0,  buffer);
		Data.Set(1,  changednamechar[i].szOldName);
		Data.Set(2,  changednamechar[i].szNewName);
	
		m_pNHozoChangedNameDlg->m_pList->AddItem(&Data);
		
		listindex ++ ;
	}
	m_pNHozoChangedNameDlg->m_pList->Refresh();
*/
}

void CNHozoBaseDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNHozoBaseDlg::SetButtonTextColor(SI32 Index)
{
	for( SI16 i = 3; i < 10; ++i )
	{
		if ( i == Index )
		{
			m_pButton[ i ]->SetFontColor(  CONTROLLER_BUTTONTEXT_COLOR  );
			m_pButton[ i ]->Refresh();
		}
		else
		{
			m_pButton[ i ]->SetFontColor(  CONTROLLER_BACK_BUTTONTEXT_COLOR  );
			m_pButton[ i ]->Refresh();
		}
	}
}

BOOL CNHozoBaseDlg::SetListDlg(SI32 index)
{
	BOOL check;
	check = m_pNHozoAppointmentDlg->SetListDlg(index);

	return check;
}

void CNHozoBaseDlg::ShowListDlg(HozoAngelList hozoangellist [])
{
	m_pNHozoAppointmentDlg->ShowListDlg(hozoangellist);
}

void CNHozoBaseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	
		{
			m_pNHozoBankruptcyDlg->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		}
	}
}
