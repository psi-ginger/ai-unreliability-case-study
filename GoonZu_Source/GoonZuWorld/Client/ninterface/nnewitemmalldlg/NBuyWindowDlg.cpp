#include "../Client/NInterface/NNewItemMalldlg/NBuyWindowDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "Char\CharManager\CharManager.h"
#include "../client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNBuyWindowDlg::CNBuyWindowDlg() : CDialogController()
{
	m_siMyNcoin					= 0;
	m_siMyEventCoin				= 0;
	m_siSelectProductKey		= 0;
	m_siSelectItemPrice			= 0;
	m_siSelectItemEventPrice	= 0;
	m_siBuyItemNum = 1;

	strcpy(m_szSelectItemName, "");

	SI32 i;
	for( i=0; i < 3; ++i)
	{
		// 아이템 초기화
		m_clItemData[i].Init();
		m_pRadioButton[i] = NULL;
	}

	for( i=0; i < 5; ++i)
		m_pButton[i] = NULL;

	for( i=0; i < 21; ++i)
		m_pStatic[i] = NULL;

	for( i=0; i < 9; ++i)
		m_pEventStatic[i] = NULL;

	m_pEventStaticBack = NULL;
	

	m_pEdit = NULL;
	m_pEditExplain = NULL;
}

CNBuyWindowDlg::~CNBuyWindowDlg()
{
	SI32 i;
	for( i=0; i < 3; ++i)
	{
		SAFE_DELETE( m_pRadioButton[i] );
	}

	for( i=0; i < 5; ++i)
		SAFE_DELETE( m_pButton[i] );

	for( i=0; i < 21; ++i)
		SAFE_DELETE( m_pStatic[i] );

	for( i=0; i < 9; ++i)
		SAFE_DELETE( m_pEventStatic[i] );

	SAFE_DELETE(m_pEventStaticBack);

	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pEditExplain);
}

void CNBuyWindowDlg::InitItemData()
{
	SI32 i;
	for( i=0; i < 3; ++i)
	{
		// 아이템 초기화
		m_clItemData[i].Init();
	}
}

void CNBuyWindowDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		cltClient *pclclient = (cltClient *)pclClient;

		if(pclclient->m_pDialog[ NPRESENTWINDOW_DLG ])
			pclclient->m_pDialog[ NPRESENTWINDOW_DLG ]->DeleteDialog();

		if(pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ])
			pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ]->DeleteDialog();

		if(pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ])
			pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ]->DeleteDialog();

		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NNewItemMallDlg/DLG_BuyWindow.ddf");
		file.CreatePopupDialog( this, NBUYWINDOW_DLG, "dialog_buywindow", StaticCallBackDialogNBuyWindowDlg );

		MovePos( pclclient->GetScreenXSize()/2 - 150 + 45, pclclient->GetScreenYSize()/2 - 370 + 100);

		SI32 i;
		for( i=0; i < 3; ++i)
		{
			//NEWCBUTTON(m_pButton[i]);
			NEWCRADIOBUTTON( m_pRadioButton[i] );
		}

		for( i=0; i < 5; ++i)
			NEWCBUTTON(m_pButton[i]);
		for( i=0; i < 21; ++i)
			NEWCSTATIC(m_pStatic[i]);
		for( i=0; i < 9; ++i)
			NEWCSTATIC(m_pEventStatic[i]);

		NEWCSTATIC(m_pEventStaticBack);

		NEWCEDIT(m_pEdit);
		NEWCEDIT(m_pEditExplain);

		// 엔코인 충전
		file.CreateControl( m_pButton[0], NBUYWINDOW_DIALOG_BUTTON_NCOIN, "button_NONAME1"); 
		// 물건 사기
		file.CreateControl( m_pButton[1], NBUYWINDOW_DIALOG_BUTTON_BUY, "button_NONAME2"); 
		// 물건 사기 취소
		file.CreateControl( m_pButton[2], NBUYWINDOW_DIALOG_BUTTON_CANCEL, "button_NONAME3"); 
		// 살 물건 증가
		file.CreateControl( m_pButton[3], NBUYWINDOW_DIALOG_BUTTON_PLUS, "button_NONAME4"); 
		// 살 물건 감소
		file.CreateControl( m_pButton[4], NBUYWINDOW_DIALOG_BUTTON_MINUS, "button_NONAME5"); 

		// USA 는 다중구매를 할 수 없다.
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA))
		{
			m_pButton[3]->Show(FALSE);
			m_pButton[4]->Show(FALSE);
		}

		file.CreateControl( m_pRadioButton[0], NBUYWINDOW_DIALOG_RADIOBUTTON1, "radiobutton_NONAME1"); 
		file.CreateControl( m_pRadioButton[1], NBUYWINDOW_DIALOG_RADIOBUTTON2, "radiobutton_NONAME2"); 
		file.CreateControl( m_pRadioButton[2], NBUYWINDOW_DIALOG_RADIOBUTTON3, "radiobutton_NONAME3"); 

		char tempBuffer[100] = {'\0'};

		for( i=0; i < 17; ++i)
		{
			sprintf(tempBuffer, "statictext_NONAME%d", i + 1);
			file.CreateControl( m_pStatic[i], NBUYWINDOW_DIALOG_STATIC1 + i, tempBuffer);
		}

		for( i=0; i < 9; ++i)
		{
			sprintf(tempBuffer, "statictext_NONAME%d", i + 23);
			file.CreateControl( m_pEventStatic[i], NBUYWINDOW_DIALOG_STATIC_EVENT0 + i, tempBuffer);
		}
		file.CreateControl( m_pEventStaticBack, NBUYWINDOW_DIALOG_STATIC_EVENT_BACK, "statictext_NONAME22" );
				

		file.CreateControl( m_pStatic[19], NBUYWINDOW_DIALOG_STATIC20, "statictext_NONAME20");
		file.CreateControl( m_pStatic[20], NBUYWINDOW_DIALOG_STATIC21, "statictext_NONAME21");


		file.CreateControl( m_pEdit, NBUYWINDOW_DIALOG_EDIT, "editbox_NONAME1"); 
		file.CreateControl( m_pEditExplain, NBUYWINDOW_DIALOG_EDITEXPLAIN, "editbox_NONAME2"); 

		// 배경색으로 채워진다
		file.CreateControl( m_pStatic[17], NBUYWINDOW_DIALOG_STATIC18, "statictext_NONAME18");
		file.CreateControl( m_pStatic[18], NBUYWINDOW_DIALOG_STATIC19, "statictext_NONAME19");

  		m_pEdit->SetTextPrintOption( DT_CENTER);

 		m_pStatic[0]->SetTextPrintOption( DT_CENTER );

		m_pStatic[7]->SetTextPrintOption( DT_LEFT );
		m_pStatic[8]->SetTextPrintOption( DT_LEFT );
		m_pStatic[9]->SetTextPrintOption( DT_LEFT );
		m_pStatic[10]->SetTextPrintOption( DT_RIGHT );
		m_pStatic[11]->SetTextPrintOption( DT_RIGHT );
		m_pStatic[12]->SetTextPrintOption( DT_RIGHT );

		m_pEventStatic[6]->SetTextPrintOption( DT_RIGHT );
		m_pEventStatic[7]->SetTextPrintOption( DT_RIGHT );
		m_pEventStatic[8]->SetTextPrintOption( DT_RIGHT ); 
		
		m_pEdit->SetText("1");
		
		m_pEditExplain->SetChangeTextDrawMode(true);

		SetTopMostWindow( true );

		//[진성] 중국 아이템몰 관련 초기화.
		NHNChinaInit();
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNBuyWindowDlg::StaticCallBackDialogNBuyWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNBuyWindowDlg *pThis = (CNBuyWindowDlg *)pControllerMgr;
	pThis->CallBackDialogNBuyWindowDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNBuyWindowDlg::CallBackDialogNBuyWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBUYWINDOW_DIALOG_BUTTON_NCOIN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->siServiceArea == ConstServiceArea_Japan)
					{					
						cltGameMsgRequest_BillPage_URL clinfo( pclclient->szID,pclclient->szPassword);
						cltMsg clMsg(GAMEMSG_REQUEST_BILLPAGE_URL,sizeof(clinfo),(BYTE*)&clinfo);
						pclclient->SendMsgToServer( (sPacketHeader*)&clMsg);

					}
					else if( pclClient->siServiceArea == ConstServiceArea_Korea )
					{
						ShellExecute( NULL, NULL, TEXT("http://cash.ndoors.com/"), NULL, NULL, SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_China) )
					{
						ShellExecute( NULL,NULL,TEXT("http://www.youxi.net.cn/AccountManage/AddCostIdx.aspx"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_USA) )
					{
						ShellExecute( NULL,NULL,TEXT("https://billing.ijji.com/payment/PaymentProcess.nhn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if ( pclClient->IsWhereServiceArea( ConstServiceArea_EUROPE ) )
					{
						ShellExecute( NULL,NULL,TEXT("http://billing.aeriagames.com/index.asp"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if ( pclClient->IsWhereServiceArea( ConstServiceArea_NHNChina ) )	//중국
					{
						ShellExecute( NULL,NULL,TEXT("http://www.newhjz.cn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_English ) )
					{
						// PCK : 글로벌의 경우 NHN회원과 NDI회원에 따라서 보여지는 링크페이지가 다르다. (09.03.30)			
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nh") >= 0 )		// NHN 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("http://luminary.ijji.com/center/billing/transfer_gcoin_01.asp"), NULL, NULL, SW_SHOWNORMAL );
						}
						else							// NDI 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("https://billing.ndoorsgames.com/fillup/payment_option.asp?site_code=luminary"), NULL, NULL, SW_SHOWNORMAL );
						}
					}

				}
				break;
			}
		}
		break;

	case NBUYWINDOW_DIALOG_BUTTON_BUY:
		{
 			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
 					if ( strlen( m_pEdit->GetText() ) < 1 )
					{
						TCHAR* pTitle = GetTxtFromMgr(5945);
						TCHAR* pText  = GetTxtFromMgr(5947);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

						return;
					}

					if(m_siSelectProductKey < 1)
					{
						TCHAR* pTitle = GetTxtFromMgr(5945);
						TCHAR* pText  = GetTxtFromMgr(5948);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

						return;
					}

					if (pclClient->siServiceArea != ConstServiceArea_NHNChina)
					{
						if(m_siSelectItemPrice < 1)
						{
							TCHAR* pTitle = GetTxtFromMgr(5945);
							TCHAR* pText  = GetTxtFromMgr(5949);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

							return;
						}
					}					

					if(m_siBuyItemNum <= 0)
					{
						TCHAR* pTitle = GetTxtFromMgr(5945);
						TCHAR* pText  = GetTxtFromMgr(5947);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

						return;
					}

					if(m_siBuyItemNum > 10)
					{
						TCHAR* pTitle = GetTxtFromMgr(5945);
						TCHAR* pText  = GetTxtFromMgr(5946);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

						return;
					}
	
					if(pclclient->IsWhereServiceArea(ConstServiceArea_Korea))
					{
						if((m_siRealCoin + m_siMyEventCoin) - m_siBuyItemNum*m_siSelectItemPrice < 0)
						{
							TCHAR* pTitle = GetTxtFromMgr(5945);
							TCHAR* pText  = GetTxtFromMgr(5950);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

							return;	
						}
					}
					else 
					{
						if(m_siRealCoin - m_siBuyItemNum*m_siSelectItemPrice < 0)
						{
							TCHAR* pTitle = GetTxtFromMgr(5945);
							TCHAR* pText  = GetTxtFromMgr(5950);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

							return;
						}
					}

					cltClient *pclclient = (cltClient*)pclClient;
					
					TCHAR*	pTitle = GetTxtFromMgr(5951);
					TCHAR*	pBody = GetTxtFromMgr(5952);
					TCHAR buffer[128] = {'\0'};
					StringCchPrintf( buffer, 128, pBody, m_szSelectItemName );
				

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;

	case NBUYWINDOW_DIALOG_BUTTON_CANCEL:
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

	case NBUYWINDOW_DIALOG_BUTTON_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					++m_siBuyItemNum;
					if(pclclient->IsWhereServiceArea(ConstServiceArea_Korea) == true)
					{
						//[진성] 구입할수있을때 까지만 증가시키자.
						if(0 > (m_siRealCoin + m_siMyEventCoin)  - m_siSelectItemPrice  * m_siBuyItemNum ) 
						{ 
							--m_siBuyItemNum; 
							return;
						}
					}
					else
					{
						//[진성] 구입할수있을때 까지만 증가시키자.
						if(		0 > m_siRealCoin    - m_siSelectItemPrice      * m_siBuyItemNum 
							||	0 > m_siMyEventCoin - m_siSelectItemEventPrice * m_siBuyItemNum	) 
						{ 
							--m_siBuyItemNum; 
							return;
						}
					}

					// 10개 이상 못 사게 막는다
					if( m_siBuyItemNum > 10 )
					{
						m_siBuyItemNum = 10;
						
					//	TCHAR* pTitle = GetTxtFromMgr( 5945 );
					//	TCHAR* pBody  = GetTxtFromMgr( 5946 );

					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
					}

					TCHAR tempBuffer[50] = { '\0' };
					StringCchPrintf( tempBuffer,50, TEXT("%d"), m_siBuyItemNum );
					m_pEdit->SetText( tempBuffer );
					
					g_JYLibFunction.SetNumUnit( m_siSelectItemPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
					m_pStatic[10]->SetText( tempBuffer );

					if(pclclient->IsWhereServiceArea(ConstServiceArea_Korea) == true)
					{
						g_JYLibFunction.SetNumUnit( (m_siRealCoin + m_siMyEventCoin) - m_siSelectItemPrice*m_siBuyItemNum, tempBuffer, 50, NULL );
						m_pStatic[12]->SetText( tempBuffer );
					}
					else
					{
						g_JYLibFunction.SetNumUnit( m_siRealCoin - m_siSelectItemPrice*m_siBuyItemNum, tempBuffer, 50, NULL );
						m_pStatic[12]->SetText( tempBuffer );
					}


					//[진성] 중국 아이템몰
					g_JYLibFunction.SetNumUnit( m_siSelectItemEventPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
					m_pEventStatic[6]->SetText( tempBuffer );
					g_JYLibFunction.SetNumUnit( m_siMyEventCoin - m_siSelectItemEventPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
					m_pEventStatic[8]->SetText( tempBuffer );
				}
				break;
			}
		}
		break;

	case NBUYWINDOW_DIALOG_BUTTON_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					--m_siBuyItemNum;

					// 10개 이상 못 사게 막는다
					if( m_siBuyItemNum <= 0 )
					{
						m_siBuyItemNum = 1;

						//TCHAR* pTitle = GetTxtFromMgr(5945);
						//TCHAR* pText  = "최소한 1개이상 구매 가능합니다.";

						//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
					}

					TCHAR tempBuffer[50] = { '\0' };
					StringCchPrintf( tempBuffer, 50 ,TEXT("%d"), m_siBuyItemNum );
					m_pEdit->SetText( tempBuffer );
					
					g_JYLibFunction.SetNumUnit(m_siSelectItemPrice*m_siBuyItemNum, tempBuffer,50, NULL );
					m_pStatic[10]->SetText(tempBuffer);


					if(pclclient->IsWhereServiceArea(ConstServiceArea_Korea) == true)
					{
						g_JYLibFunction.SetNumUnit((m_siRealCoin + m_siMyEventCoin) - m_siSelectItemPrice*m_siBuyItemNum, tempBuffer, 50 , NULL );
						m_pStatic[12]->SetText(tempBuffer);
					}
					else
					{
						g_JYLibFunction.SetNumUnit(m_siRealCoin - m_siSelectItemPrice*m_siBuyItemNum, tempBuffer, 50 , NULL );
						m_pStatic[12]->SetText(tempBuffer);
					}


					//[진성] 현재 이벤트 머니로 구입 금액
					g_JYLibFunction.SetNumUnit( m_siSelectItemEventPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
					m_pEventStatic[6]->SetText(tempBuffer);
					g_JYLibFunction.SetNumUnit( m_siMyEventCoin, tempBuffer, 50,  NULL );
					m_pEventStatic[7]->SetText(tempBuffer);
					g_JYLibFunction.SetNumUnit( m_siMyEventCoin - m_siSelectItemEventPrice * m_siBuyItemNum, tempBuffer, 50,  NULL );
					m_pEventStatic[8]->SetText(tempBuffer);
				}
				break;
			}
		}
		break;

	case NBUYWINDOW_DIALOG_RADIOBUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					SelectRadioButton(0);
				}
				break;
			}
		}
		break;

	case NBUYWINDOW_DIALOG_RADIOBUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					SelectRadioButton(1);
				}
				break;
			}
		}
		break;

	case NBUYWINDOW_DIALOG_RADIOBUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					SelectRadioButton(2);
				}
				break;
			}
		}
		break;
	}
}

//***************************************************************************************************
// Function Name	: SetBuyItemInfo
//
// Last Modified 	: 2006 / 01 / 02
// Created			: 김광명
//
// Function			: 일반 아이템 정보 셋팅
//
// Inputs			: pitemmalldata - 일반 아이템 정보
// Outputs			: 
// Returns			: 
//***************************************************************************************************
void CNBuyWindowDlg::SetBuyItemInfo(cltItemMallData& pitemmalldata)
{
	// 일반 아이템이라 기간 가격을 안 보이게 한다
	m_pRadioButton[0]->Show(false);
	m_pRadioButton[1]->Show(false);
	m_pRadioButton[2]->Show(false);

	m_pStatic[4]->Show(false);
	m_pStatic[5]->Show(false);
	m_pStatic[6]->Show(false);

	if (pclClient->IsCountrySwitch( Switch_NoPayBack )) 
	{
		// 아이템 설명
		// 청약철회 불가 문구 추가
		TCHAR buffer[512];
		TCHAR* pText = GetTxtFromMgr(8429);


		if(pclClient->IsCountrySwitch(Switch_ItemExplain2))										// [종호_szExplain2] 새로 추가되는 아이템 설명이 여기 들어간다면 적용
			StringCchPrintf(buffer, 512,TEXT("%s\r\n%s") , pText, pitemmalldata.szExplain2 );		
		else
            StringCchPrintf(buffer, 512,TEXT("%s\r\n%s") , pText, pitemmalldata.szExplain );
		
		m_pEditExplain->SetText( buffer /*pitemmalldata.szExplain*/);
	}
	else
	{
		if(pclClient->IsCountrySwitch(Switch_ItemExplain2))
			m_pEditExplain->SetText( pitemmalldata.szExplain2);									// [종호_szExplain2] 새로 추가되는 아이템 설명이 여기 들어간다면 적용
		else
			m_pEditExplain->SetText( pitemmalldata.szExplain);
		
	}

	// 아이템 이름
	m_pStatic[0]->SetFontKind("돋움");	
	m_pStatic[0]->SetFontSize(17);
	m_pStatic[0]->SetFontWeight(FW_BOLD);
	m_pStatic[0]->SetText(pitemmalldata.szName);

	MStrCpy(m_szSelectItemName, pitemmalldata.szName, 50);

	// 해당 아이템 그림
	// 아이템 이미지가 넘어 가서 이제 읽어오는걸 바꿨다.
	// 받은 아이템 폰트 인덱스에 따라서 읽어 오는 파일을 바꾼다.
	// 그리고 바뀐 파일일 경우에 새롭게 폰트 인덱스를 넣어주는 작업이 필요함.
	TCHAR szImageFilename[256];
	SI32 siImageFontindex;
	pclClient->pclItemManager->GetItemImageItemmallFileName( pitemmalldata.siImageFont, szImageFilename,sizeof(szImageFilename), &siImageFontindex);
	m_pStatic[3]->SetFileName(szImageFilename);
	m_pStatic[3]->SetFontIndex(siImageFontindex);

	TCHAR tempBuffer[50] = { '\0' };

	// 현재 구입 금액
	g_JYLibFunction.SetNumUnit(pitemmalldata.siPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
	m_pStatic[10]->SetText(tempBuffer);

	g_JYLibFunction.SetNumUnit(pitemmalldata.siPrice, tempBuffer, 50,  NULL );
	m_pStatic[20]->SetText(tempBuffer);

	//[진성] 현재 이벤트 머니로 구입 금액
	g_JYLibFunction.SetNumUnit( pitemmalldata.siEventPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
	m_pEventStatic[6]->SetText(tempBuffer);
	g_JYLibFunction.SetNumUnit( m_siMyEventCoin, tempBuffer, 50,  NULL );
	m_pEventStatic[7]->SetText(tempBuffer);
	g_JYLibFunction.SetNumUnit( m_siMyEventCoin - pitemmalldata.siEventPrice, tempBuffer, 50,  NULL );
	m_pEventStatic[8]->SetText(tempBuffer);

	m_siSelectProductKey	 = pitemmalldata.siProductKey;
	m_siSelectItemPrice		 = pitemmalldata.siPrice;
	m_siSelectItemEventPrice = pitemmalldata.siEventPrice;
	m_bAgeLimit				 = pitemmalldata.bAgeLimit;
}

//***************************************************************************************************
// Function Name	: SetBuyFirstItemInfo
//
// Last Modified 	: 2006 / 01 / 02
// Created			: 김광명
//
// Function			: 기간제 아이템 정보 첫번째 불러오기
//
// Inputs			: explain - 아이템 설명, itemname - 아이템 이름, itemfontindex - 아이템의 폰트 인덱스
// Outputs			: 
// Returns			: 
//***************************************************************************************************
void CNBuyWindowDlg::SetBuyFirstItemInfo(TCHAR* explain, TCHAR* explain2, TCHAR* itemname, SI32 itemfontindex)
{
	// 아이템 설명

	if(pclClient->IsCountrySwitch(Switch_ItemExplain2))
		m_pEditExplain->SetText( explain2 );	
	else
		m_pEditExplain->SetText( explain );



    // 일반 가격을 안보이게 한다
//	m_pStatic[13]->Show(false);	// 엔코인 글자
//	m_pStatic[19]->Show(false); // 엔코인 그림
//	m_pStatic[20]->Show(false);	// 가격

	// 아이템 이름
	m_pStatic[0]->SetFontKind("돋움");	
	m_pStatic[0]->SetFontSize(17);
	m_pStatic[0]->SetFontWeight(FW_BOLD);
	m_pStatic[0]->SetText(itemname);

	// 해당 아이템 그림
	m_pStatic[3]->SetFontIndex(itemfontindex);
}

//***************************************************************************************************
// Function Name	: SetBuyDateItemInfo
//
// Last Modified 	: 2006 / 01 / 02
// Created			: 김광명
//
// Function			: 기간제 아이템 정보 셋팅
//
// Inputs			: pitemmalldata - 입력될 기간제 아이템 정보
// Outputs			: 
// Returns			: 
//***************************************************************************************************
void CNBuyWindowDlg::SetBuyDateItemInfo(cltItemMallData* pitemmalldata)
{
	SI32 i;

	for(i = 0; i < 3; ++i)
	{
		// 1초과한것이 기간제다
		if(pitemmalldata[i].siProductKey > 1)
		{
			m_clItemData[i].Set(&pitemmalldata[i]);
		}
	}

	// 첫번째 선택을 한다
	m_pRadioButton[0]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON);
	m_siSelectProductKey		= m_clItemData[0].siProductKey;
	m_siSelectItemPrice			= m_clItemData[0].siPrice;
	m_siSelectItemEventPrice	= m_clItemData[0].siEventPrice;

	TCHAR tempBuffer[50] = { '\0' };

	// 현재 구입 금액
	g_JYLibFunction.SetNumUnit(m_siSelectItemPrice*m_siBuyItemNum, tempBuffer, 50, NULL);
	m_pStatic[10]->SetText(tempBuffer);

	g_JYLibFunction.SetNumUnit(m_clItemData[0].siPrice, tempBuffer,50, NULL);
	m_pStatic[4]->SetText(tempBuffer);

	g_JYLibFunction.SetNumUnit(m_clItemData[1].siPrice, tempBuffer, 50, NULL);
	m_pStatic[5]->SetText(tempBuffer);

	g_JYLibFunction.SetNumUnit(m_clItemData[2].siPrice, tempBuffer, 50, NULL);
	m_pStatic[6]->SetText(tempBuffer);

	//[진성] 중국 아이템몰
	g_JYLibFunction.SetNumUnit( m_siSelectItemEventPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
	m_pEventStatic[6]->SetText(tempBuffer);
}

//***************************************************************************************************
// Function Name	: SetCashInfo
//
// Last Modified 	: 2006 / 01 / 02
// Created			: 김광명
//
// Function			: 구입창에서 현재 나의 캐쉬 정보 표시
//
// Inputs			: mycash - 내 엔코인, myeventcash - 내 이벤트 머니
// Outputs			: 
// Returns			: 
//***************************************************************************************************
void CNBuyWindowDlg::SetCashInfo(SI32 mycash, SI32 myeventcash, bool showsilvercoinmark )
{
	TCHAR tempBuffer[50] = { '\0' };
	
	m_siMyNcoin		= mycash;
	m_siMyEventCoin = myeventcash;

	ShowSiverCoinMark( showsilvercoinmark );

	if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea) == true)
	{
		// 한국은 구매창에 금액 보여줄떄 이벤트머니와 리얼머니를 합산해 보여준다.

		// 내 현재 금액
		g_JYLibFunction.SetNumUnit( m_siRealCoin + m_siMyEventCoin, tempBuffer, 50, NULL );
		m_pStatic[11]->SetText(tempBuffer);									

		// 구입후 내 금액
		g_JYLibFunction.SetNumUnit( m_siRealCoin + m_siMyEventCoin - m_siSelectItemPrice, tempBuffer, 50, NULL );
		m_pStatic[12]->SetText(tempBuffer);
	}
	else
	{
		g_JYLibFunction.SetNumUnit( m_siRealCoin, tempBuffer, 50, NULL );
		m_pStatic[11]->SetText(tempBuffer);									
		g_JYLibFunction.SetNumUnit( m_siRealCoin - m_siSelectItemPrice, tempBuffer, 50, NULL );
		m_pStatic[12]->SetText(tempBuffer);

		g_JYLibFunction.SetNumUnit( m_siMyEventCoin, tempBuffer, 50, NULL );
		m_pEventStatic[7]->SetText(tempBuffer);
		g_JYLibFunction.SetNumUnit( m_siMyEventCoin - m_siSelectItemEventPrice, tempBuffer, 50, NULL );
		m_pEventStatic[8]->SetText(tempBuffer);
	}
}

//***************************************************************************************************
// Function Name	: SelectRadioButton
//
// Last Modified 	: 2006 / 01 / 02
// Created			: 김광명
//
// Function			: 선택된 라디오 버튼의 가격으로 아이템 값을 셋팅한다
//
// Inputs			: 선택된 라디오 버튼의 인덱스(0~2까지)
// Outputs			: 
// Returns			: 
//***************************************************************************************************
void CNBuyWindowDlg::SelectRadioButton( SI16 index )
{
	bool Check = m_pRadioButton[index]->IsCheck();
	if(Check)
	{
		m_siSelectProductKey		= m_clItemData[index].siProductKey;
		m_siSelectItemPrice			= m_clItemData[index].siPrice;
		m_siSelectItemEventPrice	= m_clItemData[index].siEventPrice;

		MStrCpy(m_szSelectItemName, m_clItemData[index].szName, 50);

		// 아이템 이름
		m_pStatic[0]->SetFontKind("돋움");	
		m_pStatic[0]->SetFontSize(17);
		m_pStatic[0]->SetText(m_szSelectItemName);

		TCHAR tempBuffer[50] = { '\0' };
	
		g_JYLibFunction.SetNumUnit(m_siSelectItemPrice*m_siBuyItemNum, tempBuffer, 50, NULL);
		m_pStatic[10]->SetText(tempBuffer);

		if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea) == true)
		{
			g_JYLibFunction.SetNumUnit((m_siRealCoin + m_siMyEventCoin) - m_siSelectItemPrice * m_siBuyItemNum, tempBuffer, 50, NULL);
			m_pStatic[12]->SetText(tempBuffer);
		}
		else
		{
			g_JYLibFunction.SetNumUnit(m_siRealCoin - m_siSelectItemPrice * m_siBuyItemNum, tempBuffer, 50, NULL);
			m_pStatic[12]->SetText(tempBuffer);
		}

		//[진성] 현재 이벤트 머니로 구입 금액
		g_JYLibFunction.SetNumUnit( m_siSelectItemEventPrice * m_siBuyItemNum, tempBuffer, 50, NULL );
		m_pEventStatic[6]->SetText( tempBuffer );
		g_JYLibFunction.SetNumUnit( m_siMyEventCoin, tempBuffer, 50,  NULL );
		m_pEventStatic[7]->SetText(tempBuffer);
		g_JYLibFunction.SetNumUnit( m_siMyEventCoin - m_siSelectItemEventPrice, tempBuffer, 50,  NULL );
		m_pEventStatic[8]->SetText( tempBuffer );
	}
}

void CNBuyWindowDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
                // 중국은 2차 비번체크 한다 - kkm
				if( ConstServiceArea_China == pclClient->siServiceArea )
				{
					pclclient->CreateInterface( NBUYSECONDWINDOW_DLG );
					((CNBuySecondWindowDlg*)pclclient->m_pDialog[ NBUYSECONDWINDOW_DLG ])->SetBuyInfo( m_siSelectProductKey, m_siBuyItemNum, m_siSelectItemPrice, m_siSelectItemEventPrice, m_szSelectItemName );
				}
				else
				{
					cltClient *pclclient = (cltClient *)pclClient;
					cltGameMsgRequest_ItemMall_Buyitem pclinfo(	m_siSelectProductKey, 
																m_siBuyItemNum, 
																m_siSelectItemPrice, 
																m_siSelectItemEventPrice,
																m_szSelectItemName, 
																m_bAgeLimit);
					cltMsg clMsg( GAMEMSG_REQUEST_ITEMMALL_BUYITEM, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
				}
			}
			else
				return;
		}
		break;

	}
}

void CNBuyWindowDlg::ShowSiverCoinMark( bool Flag )
{
	if( true == Flag )
	{
		m_siRealCoin = m_siMyEventCoin;
		// 은전 표시가 그림에서 21번째
		m_pStatic[14]->SetFontIndex( 21 );
		m_pStatic[15]->SetFontIndex( 21 );
		m_pStatic[16]->SetFontIndex( 21 );
		m_pStatic[19]->SetFontIndex( 21 );

		TCHAR* text = GetTxtFromMgr(10148);
		m_pStatic[8]->SetText(text);
		text = GetTxtFromMgr(10149);
		m_pStatic[9]->SetText(text);
	}
	else
	{
		m_siRealCoin = m_siMyNcoin;
		// 금전 표시가 그림에서 0번째
		m_pStatic[14]->SetFontIndex( 0 );
		m_pStatic[15]->SetFontIndex( 0 );
		m_pStatic[16]->SetFontIndex( 0 );
		m_pStatic[19]->SetFontIndex( 0 );

		TCHAR* text = GetTxtFromMgr(5964);
		m_pStatic[8]->SetText(text);
		text = GetTxtFromMgr(5965);
		m_pStatic[9]->SetText(text);
	}
	
}

void CNBuyWindowDlg::NHNChinaInit( void )
{
	bool bShow = false;
	if( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )	bShow = true;

	for( SI32 i = 0; i < 9; ++i )
	{
		m_pEventStatic[i]->Show( bShow );
	}
	
	m_pEventStaticBack->Show( bShow );
}

void CNBuyWindowDlg::NHNChinaItemSet( void )
{

}