#include "../Client/NInterface/NAqTradeWindowDlg/NAqTradeWindowDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/OutLine.h"
#include "../../InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"

#include "Msg/MsgType-WorldMoney.h"

#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNAqTradeWindowDlg::CNAqTradeWindowDlg() : CDialogController()
{
	SI32 i;
	for(i = 0; i < 5; ++i)
		m_pButton[i] = NULL;

	for(i = 0; i < 13; ++i)
		m_pStatic[i];	// 2: 서버환율, 5: 보유, 총금액:10, 12: 팔기예약

	m_pList = NULL;
	m_pMyWorldMoneyList = NULL;

	for(i = 0; i < 3; ++i)
		m_pOutLine[i];
	
	m_pEditExplain = NULL;	// 아큐 윈도우 설명
	m_pEditSellAq = NULL;	// 팔 아큐량
	m_pEditAqPrice = NULL;	// 1아큐당 금약
	m_pEditBuyAq = NULL;	// 살려는 아큐량

	m_siSellWorldMoneyNum = 0;
	m_siSellWorldMoneyPrice = 0;

	siSelectedIndex = -1 ;

	m_siAverageWorldMoneyPrice = 0 ;

}

CNAqTradeWindowDlg::~CNAqTradeWindowDlg()
{
	SI32 i;
	for(i = 0; i < 5; ++i)
		SAFE_DELETE(m_pButton[i]);

	for(i = 0; i < 13; ++i)
		SAFE_DELETE(m_pStatic[i]);	// 2: 서버환율, 5: 보유, 총금액:10, 12: 팔기예약

	SAFE_DELETE(m_pList);
	SAFE_DELETE(m_pMyWorldMoneyList);

	for(i = 0; i < 3; ++i)
		SAFE_DELETE(m_pOutLine[i]);

	SAFE_DELETE(m_pEditExplain);	// 아큐 윈도우 설명
	SAFE_DELETE(m_pEditSellAq );	// 팔 아큐량
	SAFE_DELETE(m_pEditAqPrice);	// 1아큐당 금약
	SAFE_DELETE(m_pEditBuyAq);	// 살려는 아큐량
}

void CNAqTradeWindowDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		char buffer[256] = "" ;
		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NAqTradeWindowDlg/DLG_NAqTradeWindow.ddf") );
		file.CreatePopupDialog( this, NAQTRADEWINDOW_DLG, TEXT("dialog_aqtrade"), StaticCallBackDialogNAqTradeWindowDlg );

		SI32 i;
		for(i = 0; i < 5; ++i)
			NEWCBUTTON(m_pButton[i]);

		for(i = 0; i < 13; ++i)
			NEWCSTATIC(m_pStatic[i]);	// 2: 서버환율, 5: 보유, 총금액:10, 12: 팔기예약

		NEWCLIST(m_pList);
		NEWCLIST(m_pMyWorldMoneyList);

		for(i = 0; i < 3; ++i)
			NEWCOUTLINE(m_pOutLine[i]);

		NEWCEDIT(m_pEditExplain);	// 아큐 윈도우 설명
		NEWCEDIT(m_pEditSellAq );	// 팔 아큐량
		NEWCEDIT(m_pEditAqPrice);	// 1아큐당 금약
		NEWCEDIT(m_pEditBuyAq);	// 살려는 아큐량

		char tempBuffer[100] = { '\0' };

		for( i=0; i < 13; ++i)
		{
			sprintf(tempBuffer, "statictext_NONAME%d", i + 1);
			file.CreateControl( m_pStatic[i], NAQTRADEWINDOW_DIALOG_STATIC1 + i, tempBuffer);
		}

		file.CreateControl( m_pButton[0], NAQTRADEWINDOW_DIALOG_BUTTONRESET, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NAQTRADEWINDOW_DIALOG_BUTTONSELL, TEXT("button_NONAME2"));
		file.CreateControl( m_pButton[2], NAQTRADEWINDOW_DIALOG_BUTTONBUY, TEXT("button_NONAME3"));
		file.CreateControl( m_pButton[3], NAQTRADEWINDOW_DIALOG_BUTTONCLOSE, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NAQTRADEWINDOW_DIALOG_BUTTONREFRESH, TEXT("button_NONAME5") );

		file.CreateControl( m_pEditExplain, NAQTRADEWINDOW_DIALOG_EDIT1, TEXT("editbox_NONAME1"));
		file.CreateControl( m_pEditSellAq,  NAQTRADEWINDOW_DIALOG_EDIT2, TEXT("editbox_NONAME2"));
		file.CreateControl( m_pEditAqPrice, NAQTRADEWINDOW_DIALOG_EDIT3, TEXT("editbox_NONAME3"));
		file.CreateControl( m_pEditBuyAq,   NAQTRADEWINDOW_DIALOG_EDIT4, TEXT("editbox_NONAME4"));

		file.CreateControl( m_pList,   NAQTRADEWINDOW_DIALOG_LIST, TEXT("listbox_NONAME1"));
		file.CreateControl( m_pMyWorldMoneyList,   NAQTRADEWINDOW_DIALOG_MYWORLDMONEYLIST, TEXT("listbox_NONAME2"));

		TCHAR* pText = GetTxtFromMgr(6146);
		m_pEditExplain->SetText( pText );

		pText = GetTxtFromMgr(6147);
		m_pList->SetColumn(0, 132, pText );

		pText = GetTxtFromMgr(6148);
		m_pList->SetColumn(1, 132, pText );

		m_pList->SetColumn(2, 16, TEXT("") );
		m_pList->SetBorder(TRUE);
		m_pList->SetBKColorUse(TRUE);
		m_pList->Refresh();

        pText = GetTxtFromMgr(6149);
		m_pMyWorldMoneyList->SetColumn(0, 140, pText );

		pText = GetTxtFromMgr(6148);
		m_pMyWorldMoneyList->SetColumn(1, 140, pText );
		m_pMyWorldMoneyList->SetBorder(TRUE);
		m_pMyWorldMoneyList->SetBKColorUse(TRUE);
		m_pMyWorldMoneyList->Refresh();

		file.CreateControl( m_pOutLine[0], NAQTRADEWINDOW_DIALOG_OUTLINE1, "outline_NONAME1");
		file.CreateControl( m_pOutLine[1], NAQTRADEWINDOW_DIALOG_OUTLINE2, "outline_NONAME2");
		file.CreateControl( m_pOutLine[2], NAQTRADEWINDOW_DIALOG_OUTLINE3, "outline_NONAME3");

		sprintf(buffer,"%I64d",pclClient->pclCM->CR[1]->pclCI->clWorldMoney.GetWorldMoney());
		m_pStatic[5]->SetText(buffer);

		stListBoxItemData Data;
		sprintf(buffer, "%I64d",pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siTradeWorldMoney);
		Data.Set( 0, buffer );

		sprintf(buffer, "%I64d",pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siTradeWorldMoneyPrice);
		Data.Set( 1, buffer );

		m_pMyWorldMoneyList->Clear();
		m_pMyWorldMoneyList->AddItem( &Data );   // 타입값에 넣는다
		m_pMyWorldMoneyList->SetTextPrintOption( DT_LEFT | DT_SINGLELINE );
		m_pMyWorldMoneyList->Refresh();

		cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_TRADELIST, 0, NULL );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNAqTradeWindowDlg::StaticCallBackDialogNAqTradeWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNAqTradeWindowDlg *pThis = (CNAqTradeWindowDlg *)pControllerMgr;
	pThis->CallBackDialogNAqTradeWindowDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNAqTradeWindowDlg::CallBackDialogNAqTradeWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case NAQTRADEWINDOW_DIALOG_BUTTONCLOSE:
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
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NAQTRADEWINDOW_DIALOG_BUTTONRESET:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siTradeWorldMoney > 0 &&
						 pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siTradeWorldMoneyPrice > 0 )
					{
						cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_CANCELORDER, 0, NULL );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			}
		}
		break;
	case NAQTRADEWINDOW_DIALOG_BUTTONSELL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI64 sellworldmoney = _tstoi64(m_pEditSellAq->GetText()) ;
					SI64 sellprice = _tstoi64( m_pEditAqPrice->GetText() );

					if ( sellworldmoney > pclClient->pclCM->CR[1]->pclCI->clWorldMoney.GetWorldMoney() )
						break;

					TCHAR *pTitle = GetTxtFromMgr(6198);
					TCHAR *pText = GetTxtFromMgr(6199);

					SI64 fee = sellworldmoney * sellprice / 100 ;
					if ( pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < fee )
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						break;
					}

					//------------------------------
					//	전장에 계좌가 있는지 확인한다. 
					//------------------------------
					if(pclclient->pclCM->CR[1]->pclCI->clBank.clAccountNum.GetAccountNum() == 0)
					{
						TCHAR* pText = GetTxtFromMgr(6357);
						TCHAR* pTitle = GetTxtFromMgr(6175);
						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pText );
						break;
					}

					// 화폐단위 콤마표기 추가
					TCHAR szFee[128] = { '\0', };
					g_JYLibFunction.SetNumUnit( fee , szFee, 128 );

					NTCHARString512 strText( GetTxtFromMgr(6197) );
					strText.Replace( TEXT("#fee#"), szFee );

					pTitle = GetTxtFromMgr(6196);

					stMsgBox MsgBox;
					MsgBox.Set( pclclient->m_pDialog[NAQTRADEWINDOW_DLG], pTitle, strText , MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;
	case NAQTRADEWINDOW_DIALOG_BUTTONBUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI64 buyworldmoney = _tstoi(m_pEditBuyAq->GetText()) ;

					if ( buyworldmoney <= 0 )
						break; 

					if ( siSelectedIndex < 0)
						break;

					cltGameMsgRequest_WorldMoney_BuyOrder clinfo(siSelectedIndex,buyworldmoney);
					cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_BUYORDER, sizeof(clinfo), (BYTE *)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					siSelectedIndex = -1 ;
				}
				break;
			}
		}
		break;
	case NAQTRADEWINDOW_DIALOG_EDIT2:	// 팔려면 아큐 수량 적기
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					m_siSellWorldMoneyNum = _tstoi( m_pEditSellAq->GetText() );

					if( m_siSellWorldMoneyNum < 1)
						return;

					SI32 tempLen = strlen( m_pEditSellAq->GetText() );

					if(tempLen < 1)
						return;

					// 총금액
					char tempBuffer[100] = { '\0' };
					g_JYLibFunction.SetNumUnit(m_siSellWorldMoneyNum*m_siSellWorldMoneyPrice, tempBuffer, 100);
					m_pStatic[10]->SetText(tempBuffer);
				}
				break;
			}

		}
		break;

	case NAQTRADEWINDOW_DIALOG_EDIT3:	// 팔려면 아큐 가격 적기
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					m_siSellWorldMoneyPrice = _tstoi( m_pEditAqPrice->GetText() );

					if( m_siSellWorldMoneyPrice < 1)
						return;

					SI32 tempLen = strlen( m_pEditAqPrice->GetText() );

					if(tempLen < 1)
						return;


					// 총금액
					char tempBuffer[100] = { '\0' };
					g_JYLibFunction.SetNumUnit(m_siSellWorldMoneyNum*m_siSellWorldMoneyPrice, tempBuffer, 100);
					m_pStatic[10]->SetText(tempBuffer);
				}
				break;
			}

		}
		break;

	case NAQTRADEWINDOW_DIALOG_BUTTONREFRESH:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_TRADELIST, 0, NULL );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case NAQTRADEWINDOW_DIALOG_LIST:
		{

			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					siSelectedIndex = siListIndex[m_pList->GetSelectedIndex()];
				}
				break;
			}

		}
		break;
	}
}

void CNAqTradeWindowDlg::SetTradeList()
{
	char averageprice[128] = "" ;

	TCHAR* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit(m_siAverageWorldMoneyPrice,averageprice, 128,pText);
	m_pStatic[2]->SetText(averageprice);
	
	SI32 count = 0;
	SI32 i = 0 ;	
	cltWorldMoneyTradeList temp[500];
	
	do{
		temp[i].Set(0,0,0,0);
		if(pclClient->pclDistributionWorldMoney->clTradeList[i].siPrice > 0  && 
			pclClient->pclDistributionWorldMoney->clTradeList[i].siPersonID >0 &&
			pclClient->pclDistributionWorldMoney->clTradeList[i].siWorldMoney >0 )
		{			
			siListIndex[count] = i;
			memcpy( &temp[count] , &pclClient->pclDistributionWorldMoney->clTradeList[i], sizeof(cltWorldMoneyTradeList) );
			count++;
		}

		i++;
	}while(i < 500);
	
	GMONEY comp1 = 0;
	GMONEY comp2 = 0;
	SI32 tempindex=0;
	cltWorldMoneyTradeList moneytemp;

	for(SI32 i =0 ; i < count - 1; i++)
	{
		for(SI32 j = i+1;  j < count ; j++)
		{
			 comp1 = temp[i].siPrice;
			 comp2 = temp[j].siPrice;
			if( comp1 > comp2)
			{
				tempindex = siListIndex[i];
				siListIndex[i] =siListIndex[j];
				siListIndex[j] = tempindex;

				memcpy(&moneytemp , &temp[i] , sizeof(cltWorldMoneyTradeList));
				memcpy(&temp[i] , &temp[j] ,sizeof(cltWorldMoneyTradeList)) ;
				memcpy(&temp[j] ,  &moneytemp, sizeof(cltWorldMoneyTradeList));
			}

		}
	}
	

	//cltWorldMoneyTradeList cldata[250];

	m_pList->Clear();

	SI32 sortCount = 250;

	if(sortCount > count)
		sortCount = count;

	stListBoxItemData WorldMoneyListBoxData;

	for(SI32 i = 0; i<sortCount ; i++)
	{
		TCHAR buf[64]=TEXT("");		
		WorldMoneyListBoxData.Init();
		StringCchPrintf( buf, 64, TEXT("%I64d"), temp[i].siWorldMoney);
		WorldMoneyListBoxData.Set(0,buf) ;
		StringCchPrintf( buf, 64, TEXT("%I64d") ,temp[i].siPrice);
		WorldMoneyListBoxData.Set(1,buf);
		m_pList->AddItem( &WorldMoneyListBoxData );	
        
	}
	m_pList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );	
	m_pList->Refresh();

	siSelectedIndex = -1 ;

	TCHAR	buffer[256] = TEXT("") ;
	StringCchPrintf(buffer, 256, TEXT("%I64d"),pclClient->pclCM->CR[1]->pclCI->clWorldMoney.GetWorldMoney());
	m_pStatic[5]->SetText(buffer);

	stListBoxItemData Data;
	StringCchPrintf(buffer, 256, TEXT("%I64d"),pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siTradeWorldMoney);
	Data.Set( 0, buffer );

	StringCchPrintf(buffer, 256, TEXT("%I64d"),pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siTradeWorldMoneyPrice);
	Data.Set( 1, buffer );

	m_pMyWorldMoneyList->Clear();
	m_pMyWorldMoneyList->AddItem( &Data );   // 타입값에 넣는다
	m_pMyWorldMoneyList->SetTextPrintOption( DT_LEFT | DT_SINGLELINE );
	m_pMyWorldMoneyList->Refresh();
}

void CNAqTradeWindowDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				cltGameMsgRequest_WorldMoney_SellOrder clinfo(m_siSellWorldMoneyNum,m_siSellWorldMoneyPrice);
				cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_SELLORDER, sizeof(clinfo), (BYTE *)&clinfo );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
				return;
		}
		break;

	}
}