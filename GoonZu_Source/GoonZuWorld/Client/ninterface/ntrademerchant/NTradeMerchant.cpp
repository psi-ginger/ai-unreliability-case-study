#include "../Client/NInterface/NTradeMerchant/NTradeMerchant.h"

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

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNTradeMerchantDlg::CNTradeMerchantDlg() : CDialogController()
{
	m_siImageType = -1;	// 아이템 이미지 종류
	m_siImageIndex = -1;	// 아이템  이미지 인덱스

	m_siNPCKind = 0;
	m_siSelectedIndex = -1;

	m_pclItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	m_pTradeMerchantNPC = new CTradeMerchantNPC();


	m_pStatic[0] = NULL;	
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;

	m_pEdit = NULL;	

	m_pListView = NULL;

	m_pButtonSellItem = NULL;
	m_pButtonClose = NULL;
}

CNTradeMerchantDlg::~CNTradeMerchantDlg()
{
	SetTimerOnOff(false);

	SAFE_DELETE_A(m_pclItem);
	SAFE_DELETE(m_pTradeMerchantNPC);
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pListView);
	SAFE_DELETE(m_pButtonSellItem);
	SAFE_DELETE(m_pButtonClose);
}

void CNTradeMerchantDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTradeMerchantDlg/DLG_TradeMerchant.ddf"));
		file.CreatePopupDialog( this, NTRADEMERCHANT_DLG, TEXT("dialog_TradeMerchant"), StaticCallBackDialogNTradeMerchantDlg );

		m_pStatic[0]		  = new CStatic( this );
		m_pStatic[1]		  = new CStatic( this );
		m_pStatic[2]		  = new CStatic( this );
		m_pEdit				  = new CEdit( this );
		m_pListView			  = new CListView( this );
		m_pButtonSellItem	  = new CButton( this );
		m_pButtonClose		  = new CButton( this );

		file.CreateControl( m_pStatic[0], NTRADEMERCHANT_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		
		file.CreateControl( m_pStatic[2], NTRADEMERCHANT_DIALOG_STATIC3, TEXT("statictext_NONAME3") );

		m_pStatic[2]->SetBKColorUse( true );
		m_pStatic[2]->SetBorder( true );
		m_pStatic[2]->Refresh();

		file.CreateControl( m_pStatic[1], NTRADEMERCHANT_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		

		file.CreateControl( m_pEdit, NTRADEMERCHANT_DIALOG_EDIT, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pListView, NTRADEMERCHANT_DIALOG_LISTVIEW, TEXT("listview_NONAME1") );

		m_pListView->SetBKColorUse( true );
		m_pListView->SetBorder( true );
	
		file.CreateControl( m_pButtonSellItem, NTRADEMERCHANT_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButtonClose, NTRADEMERCHANT_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNTradeMerchantDlg::StaticCallBackDialogNTradeMerchantDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNTradeMerchantDlg *pThis = (CNTradeMerchantDlg *)pControllerMgr;
	pThis->CallBackDialogNTradeMerchantDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNTradeMerchantDlg::CallBackDialogNTradeMerchantDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NTRADEMERCHANT_DIALOG_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					m_siSelectedIndex = m_pListView->GetSelectedItemIndex();
				}
				break;
			}
			break;
		}

	case NTRADEMERCHANT_DIALOG_BUTTON1:	// 팔기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siSelectedIndex > 0 && m_pTradeMerchantNPC->m_siTradeMerchantNPCKind > 0 && m_pTradeMerchantNPC->m_siBuyItemUnique > 0 )
					{
						cltGameMsgRequest_TradeMerchantBuyItemFromPerson clGameMsgRequest_TradeMerchantBuyItemFromPerson( m_pTradeMerchantNPC->m_siTradeMerchantNPCKind, m_siSelectedIndex, &m_pclItem[ m_siSelectedIndex ] );
						cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON, sizeof(clGameMsgRequest_TradeMerchantBuyItemFromPerson), (BYTE*)&clGameMsgRequest_TradeMerchantBuyItemFromPerson );
						pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
					}
				}
				break;
			}
		}
		break;

	case NTRADEMERCHANT_DIALOG_BUTTON2:	// 닫기
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

void CNTradeMerchantDlg::Action()
{
	if ( IsInventoryChanged() )
	{
		LoadInventoryData();
	}

}
void CNTradeMerchantDlg::SetTradeMerChant(SI32 id)
{
	SI32 siKind = pclClient->pclCM->CR[id]->GetKind();
	if ( siKind == pclClient->GetUniqueFromHash( TEXT("KIND_CHINATRATER")) )
		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash( TEXT("KIND_CHINATRATER")) - 1);
	else if ( siKind == pclClient->GetUniqueFromHash( TEXT("KIND_RUSSIATRATER")) )
		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash( TEXT("KIND_RUSSIATRATER")) - 1);
	else if ( siKind == pclClient->GetUniqueFromHash( TEXT("KIND_JAPANTRATER")) )
		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash( TEXT("KIND_JAPANTRATER")) - 1);
	else
		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN") ) - 1);
		
	// 현재 무역 상인이 구입 중인 정보를 요청
	cltGameMsgRequest_TradeMerchantBuyInfo clGameMsgRequest_TradeMerchantBuyInfo( pclClient->pclCM->CR[id]->GetCharUnique(), pclClient->pclCM->CR[id]->GetKind() );
	cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYINFO, sizeof(clGameMsgRequest_TradeMerchantBuyInfo), (BYTE*)&clGameMsgRequest_TradeMerchantBuyInfo );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg );

	TCHAR* pText = GetTxtFromMgr(2002);
	m_pEdit->SetText(pText);

	SetTimerOnOff(true);
}

bool CNTradeMerchantDlg::IsInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_pclItem[ j ];
		BYTE* dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j];

		for(int i = 0;i < size; i++)
		{
			if(src[i] != dst[i])
			{
				return true;
			}
		}
	}

	return false;
}

void CNTradeMerchantDlg::LoadInventoryData()
{
//	m_pListView->DeleteAllItems();

	m_siSelectedIndex = -1;

	SI16 Counter = 0;

	TCHAR buffer[ 256 ] = TEXT("");
	TCHAR ItemName[ 128 ] = TEXT("");

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );


	for ( SI32 i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_pclItem[ i ] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];

		if ( m_pclItem[ i ].siUnique > 0 && m_pclItem[ i ].siUnique == m_pTradeMerchantNPC->m_siBuyItemUnique )
		{
			if( m_pclItem[i].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
				//	TCHAR* pText = GetTxtFromMgr(676);
				//	sprintf(buffer, pText, ItemName, m_pclItem[i].GetItemNum() );
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_pclItem[ i ], ToolTipMsg );
										
					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_pclItem[i].GetItemNum(), ItemName, ToolTipMsg );														
					m_pListView->InsertItem( &ListViewItemData );

				//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_USERINVENTORY ), LVM_INSERTITEM, 0, (LPARAM)&Li );
				}
			}
		}
	}

	return;
}

void CNTradeMerchantDlg::Set( CTradeMerchantNPC *pTradeMerchantNPC )
{
	m_pTradeMerchantNPC->Set( pTradeMerchantNPC );

	LoadTradeMerchantNPCBuyData();
	LoadInventoryData();

}

void CNTradeMerchantDlg::LoadTradeMerchantNPCBuyData()
{
	if ( m_pTradeMerchantNPC->m_siBuyItemUnique > 0 )
	{
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pTradeMerchantNPC->m_siBuyItemUnique );

		if ( ItemName != NULL && _tcslen ( ItemName ) > 0 )
		{
			if ( m_pTradeMerchantNPC->m_siBuyItemPrice > 0 )
			{
				GMONEY tradetax = m_pTradeMerchantNPC->m_siBuyItemPrice/100*m_pTradeMerchantNPC->m_siTradeTax ;
				TCHAR Buffer[ 256 ] = TEXT("");
				TCHAR* pText = GetTxtFromMgr(2003);
				TCHAR tax[128]= TEXT(" ") ;
				TCHAR taxmoney[128]= TEXT(" ") ;
				TCHAR money[128]= TEXT(" ") ;
				TCHAR num1[128]= TEXT(" ") ;
				TCHAR num2[128]= TEXT(" ") ;
				
				StringCchPrintf(tax, 128, TEXT("%d"),m_pTradeMerchantNPC->m_siTradeTax);
				StringCchPrintf(taxmoney, 128, TEXT("%d"),tradetax);
				StringCchPrintf(money, 128, TEXT("%d"),m_pTradeMerchantNPC->m_siBuyItemPrice-tradetax);
				StringCchPrintf(num1, 128, TEXT("%d"),m_pTradeMerchantNPC->m_siBuyTotalMoney/m_pTradeMerchantNPC->m_siBuyItemPrice);
				StringCchPrintf(num2, 128, TEXT("%d"),MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/m_pTradeMerchantNPC->m_siBuyItemPrice);
				Nsprintf(Buffer,pText,TEXT("item"), ItemName   ,TEXT("tax"),  tax ,TEXT("taxmoney"), taxmoney   ,TEXT("money"),  money  ,TEXT("num1"), num1,TEXT("num2"),num2,NULL);
			//	sprintf( Buffer, pText,
			//		ItemName,m_pTradeMerchantNPC->m_siTradeTax,tradetax,
			//		m_pTradeMerchantNPC->m_siBuyItemPrice-tradetax, 
			//		m_pTradeMerchantNPC->m_siBuyTotalMoney/m_pTradeMerchantNPC->m_siBuyItemPrice,
			//		MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/m_pTradeMerchantNPC->m_siBuyItemPrice);
				m_pEdit->SetText(Buffer);

				cltItem clItem;
				clItem.Init();
				clItem.siUnique = m_pTradeMerchantNPC->m_siBuyItemUnique;
				clItem.siItemNum = 1;

				SI32 itemunique = clItem.siUnique;

				if(itemunique)
				{
					cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

					if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) )
					{}
					else return;
				}
				if( 105 == m_siImageType)
				{
					m_pStatic[1]->SetFileName(TEXT("GImg/ItemImg0.SPR"));
				}
				else if( 106 == m_siImageType)
				{
					m_pStatic[1]->SetFileName(TEXT("GImg/ItemImg1.SPR"));
				}
				else if( 107 == m_siImageType)
				{
					m_pStatic[1]->SetFileName(TEXT("GImg/ItemImg2.SPR"));
				}

				m_pStatic[1]->SetFontIndex(m_siImageIndex);

			//	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE, NULL, &clItem, 5, 5 );
			}
			else
			{
			//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE ) );
				m_pStatic[1]->SetFileName(TEXT(""));

				TCHAR* pText = GetTxtFromMgr(2004);
				m_pEdit->SetText(pText);// SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_GETITEM_EXPLAIN ), pText );
				return;
			}
		}
	}
	else
	{
		m_pStatic[2]->SetFileName(TEXT("")); //ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE ) );
		TCHAR* pText = GetTxtFromMgr(2004);
		m_pEdit->SetText(pText);
	}

}

void CNTradeMerchantDlg::SellToNPCSuccess()
{
	TCHAR* pText = GetTxtFromMgr(2005);
	m_pEdit->SetText( pText );
}