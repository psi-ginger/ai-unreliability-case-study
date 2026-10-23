#include "../Client/NInterface/NIzoDlg/NIzoPriceValueDlg.h"
#include "../Client/NInterface/NIzoDlg/NIzoWorldTradeTaxDlg.h"
#include "../Client/ninterface/nizodlg/NIzoMarketPriceDlg.h"
#include "../Client/NInterface/NIzoDlg/NIzoBaseDlg.h"

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


extern cltCommonLogic* pclClient;

CNIzoBaseDlg::CNIzoBaseDlg() : CDialogController()
{
	m_bSet = FALSE ;
	m_bIzoCheck = FALSE;
	m_bSound = FALSE;

	StringCchCopy(m_szIzoName, MAX_PLAYER_NAME, TEXT(""));
	StringCchCopy(m_szIzoProfile, 256, TEXT(""));
	StringCchCopy(m_szIzoID, MAX_PLAYER_NAME, TEXT(""));

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;
	m_pButton[4] = NULL;

	m_pEdit = NULL;
	m_pOutLine = NULL;
	m_pNIzoPriceValueDlg = NULL;
	m_pNIzoWorldTradeTaxDlg = NULL;
	m_pNIzoMarketPriceDlg = NULL;

	m_pButtonTab[0] = NULL;
	m_pButtonTab[1] = NULL;
	m_pButtonTab[2] = NULL;
}

CNIzoBaseDlg::~CNIzoBaseDlg()
{
	SetTimerOnOff( false );

	SAFE_DELETE(m_pOutLine);
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[4]);

	SAFE_DELETE(m_pEdit);

	SAFE_DELETE(m_pNIzoPriceValueDlg);
	SAFE_DELETE(m_pNIzoWorldTradeTaxDlg);
	SAFE_DELETE(m_pNIzoMarketPriceDlg);

	SAFE_DELETE(m_pButtonTab[0]);
	SAFE_DELETE(m_pButtonTab[1]);
	SAFE_DELETE(m_pButtonTab[2]);
}

void CNIzoBaseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NIzoDlg/DLG_Izo_Base.ddf"));
		file.CreatePopupDialog( this, NIZOBASE_DLG, TEXT("dialog_Izo_Base"), StaticCallBackDialogNIzoBaseDlg );

		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);
		NEWCSTATIC(m_pStatic[2]);

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		NEWCBUTTON(m_pButton[4]);

		NEWCEDIT(m_pEdit);

		NEWCBUTTON(m_pButtonTab[0]);
		NEWCBUTTON(m_pButtonTab[1]);
		NEWCBUTTON(m_pButtonTab[2]);

		m_pOutLine = new COutLine(this);

		file.CreateControl(m_pOutLine,NIZOBASE_DIALOG_OUTLINE, TEXT("outline_NONAME1"));
		file.CreateControl( m_pStatic[0], NIZOBASE_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pStatic[1], NIZOBASE_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pStatic[2], NIZOBASE_DIALOG_STATIC3, TEXT("statictext_NONAME3") );

		file.CreateControl( m_pButton[0], NIZOBASE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NIZOBASE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NIZOBASE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NIZOBASE_DIALOG_BUTTON4, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NIZOBASE_DIALOG_BUTTON5, TEXT("button_NONAME20") );

		file.CreateControl( m_pEdit, NIZOBASE_DIALOG_EDIT, TEXT("editbox_NONAME1") );

		file.CreateControl( m_pButtonTab[0], NIZOBASE_DIALOG_BUTTONTAB1, TEXT("button_tab1"));
		file.CreateControl( m_pButtonTab[1], NIZOBASE_DIALOG_BUTTONTAB2, TEXT("button_tab2"));
		file.CreateControl( m_pButtonTab[2], NIZOBASE_DIALOG_BUTTONTAB3, TEXT("button_tab3"));

		SetButtonTextColor(0);

		m_pNIzoPriceValueDlg = new CNIzoPriceValueDlg();
		m_pNIzoPriceValueDlg->Create( this );
		m_pNIzoPriceValueDlg->Show();

		m_pNIzoWorldTradeTaxDlg = new CNIzoWorldTradeTaxDlg();
		m_pNIzoWorldTradeTaxDlg->Create( this );
		m_pNIzoWorldTradeTaxDlg->Hide();

		m_pNIzoMarketPriceDlg = new CNIzoMarketPriceDlg();
		m_pNIzoMarketPriceDlg->Create( this );
		m_pNIzoMarketPriceDlg->Hide();

		cltClient *pclclient = (cltClient*)pclClient;

		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER1 )
		{
			m_bIzoCheck = FALSE;

			EnableButton(m_pButton[2], FALSE);	// 설정버튼 비활성화

			m_pNIzoPriceValueDlg->m_pButton[0]->Enable(FALSE);
			m_pNIzoPriceValueDlg->m_pButton[1]->Enable(FALSE);



			//SetTimerOnOff( true );
			//SetActionTimer( 60 );
		}
		else
		{
			m_bIzoCheck = TRUE;

			m_pEdit->SetReadOnly(FALSE);

			EnableButton(m_pButton[0], FALSE);
			EnableButton(m_pButton[1], FALSE);


			SetTimerOnOff( true );
			SetActionTimer( 500 );
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETIZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENIZODLG);
	
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

void CNIzoBaseDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if(m_bIzoCheck)
	{
		TCHAR buffer[256] = TEXT("") ;
		MStrCpy(buffer, m_pEdit->GetText(), 256);
		SI32 limit = _tcslen(buffer);

		TCHAR* pText = GetTxtFromMgr(2059);
		StringCchPrintf(buffer, 256, pText,m_szIzoName,limit);
		m_pStatic[0]->SetText(buffer, DT_LEFT);	// 호조 내용 얼마나 적었나 표시

	}
	else
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GETIZOINFO,0,NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

	}
}

void CALLBACK CNIzoBaseDlg::StaticCallBackDialogNIzoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNIzoBaseDlg *pThis = (CNIzoBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNIzoBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNIzoBaseDlg::CallBackDialogNIzoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NIZOBASE_DIALOG_BUTTON1:	// 꼼지
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_szIzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					/*
					cltGameMsgRequest_Userid clUserid( m_szIzoName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);*/
				}
				break;
			}
		}
		break;

	case NIZOBASE_DIALOG_BUTTON2:	// 귓말
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen(m_szIzoName) > 0 && m_szIzoName != NULL )
						pclclient->RequestSecretChatByCharName(m_szIzoName);
				}
				break;
			}
		}
		break;

	case NIZOBASE_DIALOG_BUTTON3:	// 설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[1024] = TEXT("") ;

					MStrCpy(buffer, m_pNIzoPriceValueDlg->m_pEdit[1]->GetText(), 8);
					SI32 price = _tstoi(buffer);
					if ( price < 50 || price > 75 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2019);
						TCHAR* pText = GetTxtFromMgr(2060);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					if ( TABS(m_pNIzoPriceValueDlg->GetPriceValue() - price) < 0 || TABS(m_pNIzoPriceValueDlg->GetPriceValue() - price) > 1 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2019);
						TCHAR* pText = GetTxtFromMgr(2061);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}
					m_pNIzoPriceValueDlg->SetPriceValue(price);
			
					if(pclClient->IsCountrySwitch(Switch_Izo_TinTinPercent))
					{
						MStrCpy(buffer, m_pNIzoPriceValueDlg->m_pEdit_TinTin->GetText(), 8);
						SI32 tintin = _tstoi(buffer);
						if ( tintin < 1 || tintin > 10 )
						{
							TCHAR* pTitle = GetTxtFromMgr(2019);
							TCHAR* pText = GetTxtFromMgr(8664);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							break;
						}

						if ( TABS(m_pNIzoPriceValueDlg->GetTinTin() - tintin < 0 || TABS(m_pNIzoPriceValueDlg->GetTinTin() - tintin) > 1) )
						{
							TCHAR* pTitle = GetTxtFromMgr(2019);
							TCHAR* pText = GetTxtFromMgr(8665);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							break;
						}
						m_pNIzoPriceValueDlg->SetTinTin(tintin);
					}

					if ( pclclient->clClientDate.GetDateVary() <= m_pNIzoPriceValueDlg->GetPriceTime() + 30 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2063);
						TCHAR* pText = GetTxtFromMgr(2062);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					//	GetDlgItemText(m_hDlg,IDC_EDIT_IZO,m_szIzoProfile,256);
					MStrCpy(m_szIzoProfile, m_pEdit->GetText(), 256);
					SI32 limit = _tcslen(m_szIzoProfile);
					if ( limit > 0 && limit < 256 )
					{	
						cltGameMsgRequest_SetIzoInfo clinfo(m_szIzoProfile,m_pNIzoPriceValueDlg->GetPriceValue(),
							m_pNIzoWorldTradeTaxDlg->GetbWorldTradeTax(),
							m_pNIzoWorldTradeTaxDlg->GetSpecialTaxRange(),
							m_pNIzoWorldTradeTaxDlg->GetSpecialTaxRate(),
							m_pNIzoMarketPriceDlg->GetMarketDownRate(),
							m_pNIzoPriceValueDlg->GetTinTin());
						cltMsg clMsg(GAMEMSG_REQUEST_SETIZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						m_bSet = TRUE ;
					}
					else
					{
						TCHAR* pTitle = GetTxtFromMgr(2019);
						TCHAR* pText = GetTxtFromMgr(2024);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
					}
				}
				break;
			}
		}
		break;

	case NIZOBASE_DIALOG_BUTTON4:	// 나가기
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
	case NIZOBASE_DIALOG_BUTTON5:		// 도움말
		{

			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->ShowHelpWindow(TEXT("NULL"));
			}
		}
		break;
	case NIZOBASE_DIALOG_BUTTONTAB1:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			SetButtonTextColor(0);
			m_pNIzoPriceValueDlg->Show();
			m_pNIzoWorldTradeTaxDlg->Hide();
			m_pNIzoMarketPriceDlg->Hide();
		}
		break;
	case NIZOBASE_DIALOG_BUTTONTAB2:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			SetButtonTextColor(1);
			m_pNIzoPriceValueDlg->Hide();
			m_pNIzoWorldTradeTaxDlg->Show();
			m_pNIzoMarketPriceDlg->Hide();
		}
		break;
	case NIZOBASE_DIALOG_BUTTONTAB3:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			SetButtonTextColor(2);
			m_pNIzoPriceValueDlg->Hide();
			m_pNIzoWorldTradeTaxDlg->Hide();
			m_pNIzoMarketPriceDlg->Show();
		}
		break;
	}
}

void CNIzoBaseDlg::SetIzoBase(CMinisterBase *pMini,SI32 pricetime,SI32 priceinx,SI32 pricevalue, 
							  GMONEY money,SI32 calctime, GMONEY tradetax,GMONEY tradelimitmoney, 
							  bool worldtradetax, SI32 specialtaxrange, SI32 specialtaxrate, 
							  SI32 marketdownrate, SI16 tintin )
{
	cltClient *pclclient = (cltClient *)pclClient;

	cltDate cldate;
	TCHAR buffer[256] = TEXT("") ;
	TCHAR numbuf[128] = TEXT("") ;

	m_pNIzoPriceValueDlg->SetPriceValue(pricevalue);
	m_pNIzoPriceValueDlg->SetPriceTime(pricetime);
	m_pNIzoPriceValueDlg->SetTotalMoney(money);
	m_pNIzoPriceValueDlg->SetCalcTime(calctime);

	m_pNIzoWorldTradeTaxDlg->SetbWorldTradeTax(worldtradetax);
	m_pNIzoWorldTradeTaxDlg->SetSpecialTaxRange(specialtaxrange);
	m_pNIzoWorldTradeTaxDlg->SetSpecialTaxRate(specialtaxrate);

	m_pNIzoMarketPriceDlg->SetMarketDownRate(marketdownrate);

	/*TCHAR buffer[256]=TEXT("");
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
	}*/


	// 날짜를 구한다.
	cldate.MakeFullDateFromDateVary(m_pNIzoPriceValueDlg->GetPriceTime());// pricetime);
#if 1 //KHY - 0607 시간 삭제.
	if(cldate.GetRealDateText(buffer) ==false)
#else
	if(cldate.GetDateText(buffer) == false)
#endif
	{
		return ;
	}
	//	SetDlgItemText(m_hDlg,IDC_STATIC_IZODAY,buffer);
	if ( pclClient->siServiceArea == ConstServiceArea_English 
		|| pclClient->siServiceArea == ConstServiceArea_NHNChina
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		m_pStatic[2]->SetText(buffer, DT_LEFT | DT_VCENTER); // 마지막 변경일
	}
	else
	{
		m_pStatic[2]->SetText(buffer, DT_CENTER | DT_VCENTER); // 마지막 변경일
	}

	if ( m_bSet )
	{
		TCHAR* pTitle = GetTxtFromMgr(2045);
		TCHAR* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		m_bSet = FALSE ;
		return ;
	}

	if(m_bSound == FALSE)
	{
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_IZO")),0,0);
		m_bSound = TRUE;
	}

	StringCchCopy(m_szIzoID, MAX_PLAYER_NAME, pMini->m_szAccountID);
	StringCchCopy(m_szIzoName, MAX_PLAYER_NAME, pMini->m_szPersonName);
	StringCchCopy(m_szIzoProfile, 256, pMini->m_szGreeting);
	// 통화량 업데이트 시간. 
	cldate.MakeFullDateFromDateVary(m_pNIzoPriceValueDlg->GetCalcTime());// m_siCalcTime);
	if(cldate.GetDateText(numbuf) == TRUE)
	{
		TCHAR* pText = GetTxtFromMgr(612);
		StringCchPrintf(buffer, 256, pText, numbuf);
		//		SetDlgItemText(m_hTabDlg[0], IDC_STATIC_TOTALMONEY_DATE, buffer);
		m_pNIzoPriceValueDlg->m_pStatic[4]->SetText(buffer, DT_CENTER | DT_VCENTER);
	}

	TCHAR* pText = GetTxtFromMgr(2067);
	StringCchPrintf(buffer, 256, pText,m_szIzoName);
	SetTitle(buffer);

	//	MStrCpy(m_szIzoProfile, m_pEdit->GetText(), 256);
	m_pEdit->SetText(m_szIzoProfile);
	pText = GetTxtFromMgr(2048);
	StringCchCat(buffer, 256, pText);
	m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER);

	//	StringCchPrintf(buffer, 256, TEXT("%d"),priceinx);
	g_JYLibFunction.SetNumUnit( priceinx , buffer, 256 );
	//	SetDlgItemText( m_hTabDlg[0],IDC_EDIT_PRICEINX, buffer );
	m_pNIzoPriceValueDlg->m_pEdit[0]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
	m_pNIzoPriceValueDlg->m_pEdit[0]->SetText(buffer);

	StringCchPrintf(buffer, 256, TEXT("%d"), m_pNIzoPriceValueDlg->GetPriceValue() );
	//	SetDlgItemText(m_hTabDlg[0], IDC_EDIT_PRICEVALUE, buffer);
	m_pNIzoPriceValueDlg->m_pEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
	m_pNIzoPriceValueDlg->m_pEdit[1]->SetText(buffer);

	pText = GetTxtFromMgr(611);
	//	g_JYLibFunction.SetNumUnit(m_siTotalMoney,numbuf, pText);
	g_JYLibFunction.SetNumUnit(m_pNIzoPriceValueDlg->GetTotalMoney(),numbuf, 128, pText);
	m_pNIzoPriceValueDlg->m_pStatic[3]->SetText(numbuf, DT_LEFT | DT_VCENTER);

	SI32 alchemyrate = ( 75 - m_pNIzoPriceValueDlg->GetPriceValue() ) * (-0.5 ) + 13 ;
	StringCchPrintf(buffer, 256, TEXT("%d %%"), alchemyrate );
	//	SetDlgItemText(m_hTabDlg[0], IDC_STATIC_ALCHEMY_RATE, buffer);

	if(pclClient->IsCountrySwitch(Switch_Izo_TinTinPercent))
	{
		m_pNIzoPriceValueDlg->SetTinTin(tintin);

		StringCchPrintf(buffer, 256, TEXT("%d"), m_pNIzoPriceValueDlg->GetTinTin() );
		m_pNIzoPriceValueDlg->m_pEdit_TinTin->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
		m_pNIzoPriceValueDlg->m_pEdit_TinTin->SetText(buffer);
	}
	else
	{
		m_pNIzoPriceValueDlg->m_pStatic[10]->SetText(buffer, DT_CENTER | DT_VCENTER);
	}
	
	m_pNIzoWorldTradeTaxDlg->LoadWorldTradeTax();
	m_pNIzoMarketPriceDlg->LoadMarketDownRate();

	if ( pclclient->clClientDate.GetDateVary() <= m_pNIzoPriceValueDlg->GetPriceTime() + 30 )
	{

		m_pNIzoPriceValueDlg->m_pButton[0]->Enable(false);
		m_pNIzoPriceValueDlg->m_pButton[1]->Enable(false);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRangeUp->Enable(false);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRangeDown->Enable(false);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRateUp->Enable(false);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRateDown->Enable(false);
		m_pNIzoMarketPriceDlg->m_pButtonRateUp->Enable(false);
		m_pNIzoMarketPriceDlg->m_pButtonRateDown->Enable(false);
	}
	else
	{
		m_pNIzoPriceValueDlg->m_pButton[0]->Enable(true);
		m_pNIzoPriceValueDlg->m_pButton[1]->Enable(true);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRangeUp->Enable(true);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRangeDown->Enable(true);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRateUp->Enable(true);
		m_pNIzoWorldTradeTaxDlg->m_pButtonTaxRateDown->Enable(true);
		m_pNIzoMarketPriceDlg->m_pButtonRateUp->Enable(true);
		m_pNIzoMarketPriceDlg->m_pButtonRateDown->Enable(true);
	}
}

void CNIzoBaseDlg::EnableButton(CButton* pButton, BOOL Flag)
{
	if(Flag)
	{
		pButton->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR );
		pButton->Enable(false);
	}
}

void CNIzoBaseDlg::SetButtonTextColor(SI32 Index)
{
	for( SI16 i = 0; i < 3; ++i )
	{
		if ( i == Index )
		{
			m_pButtonTab[ i ]->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
			m_pButtonTab[ i ]->Refresh();
		}
		else
		{
			m_pButtonTab[ i ]->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
			m_pButtonTab[ i ]->Refresh();
		}
	}
}

