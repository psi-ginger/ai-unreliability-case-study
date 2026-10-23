#include "../Client/NInterface/NTradeMerchantHighClassDlg/NTradeMerchantHighClassDlg.h"

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

CNTradeMerchantHighClassDlg::CNTradeMerchantHighClassDlg() : CDialogController()
{
	m_siImageType = -1;	// 아이템 이미지 종류
	m_siImageIndex = -1;	// 아이템  이미지 인덱스

	m_siNPCKind = 0;
	m_siSelectedIndex = -1;

	m_pclItem = new cltItem[ MAX_ITEM_PER_PERSON ];

	m_pStatic[0] = NULL;	
	m_pStatic[1] = NULL;	
	m_pStatic[2] = NULL;	

	m_pEdit = NULL;	

	m_pListView = NULL;

	m_pButtonSellItem = NULL;
	m_pButtonClose = NULL;
}

CNTradeMerchantHighClassDlg::~CNTradeMerchantHighClassDlg()
{
	SetTimerOnOff(false);

	SAFE_DELETE_A(m_pclItem);
	
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pListView);
	SAFE_DELETE(m_pButtonSellItem);
	SAFE_DELETE(m_pButtonClose);
}

void CNTradeMerchantHighClassDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTradeMerchantDlg/DLG_TradeMerchant.ddf"));
		file.CreatePopupDialog( this, NTRADEMERCHANTHIGHCLASS_DLG, TEXT("dialog_TradeMerchant"), StaticCallBackDialogNTradeMerchantHighClassDlg );

		TCHAR* pText = GetTxtFromMgr(3402);
		SetTitle(pText);

		m_pStatic[0]		  = new CStatic( this );
		m_pStatic[1]		  = new CStatic( this );
		m_pStatic[2]		  = new CStatic( this );
		m_pEdit				  = new CEdit( this );
		m_pListView			  = new CListView( this );
		m_pButtonSellItem	  = new CButton( this );
		m_pButtonClose		  = new CButton( this );

		file.CreateControl( m_pStatic[0], NTRADEMERCHANTHIGHCLASS_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		m_pStatic[0]->SetFontIndex(pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) - 1);

//		m_pStatic[1]->SetBKColorUse( true );
//		m_pStatic[1]->SetBorder( true );
//		m_pStatic[1]->Refresh();

		file.CreateControl( m_pStatic[2], NTRADEMERCHANTHIGHCLASS_DIALOG_STATIC3, TEXT("statictext_NONAME3") );

		m_pStatic[2]->SetBKColorUse( true );
		m_pStatic[2]->SetBorder( true );
		m_pStatic[2]->Refresh();

		file.CreateControl( m_pStatic[1], NTRADEMERCHANTHIGHCLASS_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		


		file.CreateControl( m_pEdit, NTRADEMERCHANTHIGHCLASS_DIALOG_EDIT, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pListView, NTRADEMERCHANTHIGHCLASS_DIALOG_LISTVIEW, TEXT("listview_NONAME1") );

		m_pListView->SetBKColorUse( true );
		m_pListView->SetBorder( true );
	

		file.CreateControl( m_pButtonSellItem, NTRADEMERCHANTHIGHCLASS_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButtonClose, NTRADEMERCHANTHIGHCLASS_DIALOG_BUTTON2, TEXT("button_NONAME2") );

		SetHighClassMerchant();
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNTradeMerchantHighClassDlg::StaticCallBackDialogNTradeMerchantHighClassDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNTradeMerchantHighClassDlg *pThis = (CNTradeMerchantHighClassDlg *)pControllerMgr;
	pThis->CallBackDialogNTradeMerchantHighClassDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNTradeMerchantHighClassDlg::CallBackDialogNTradeMerchantHighClassDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NTRADEMERCHANTHIGHCLASS_DIALOG_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					m_siSelectedIndex = m_pListView->GetSelectedItemIndex();

					if(m_siSelectedIndex < 0)
						break;

					cltGameMsgRequest_TradeMerchantBuyItemInfo clGameMsgRequest_TradeMerchantBuyItemInfo( pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")), m_pclItem[ m_siSelectedIndex ].siUnique, m_siSelectedIndex );
					cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMINFO, sizeof(clGameMsgRequest_TradeMerchantBuyItemInfo), (BYTE*)&clGameMsgRequest_TradeMerchantBuyItemInfo );
					pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
				}
				break;
			}
			
		}
		break;

	case NTRADEMERCHANTHIGHCLASS_DIALOG_BUTTON1:	// 팔기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siItemPrice < 1 )
					{
						return;
					}

					if ( m_siSelectedIndex > 0 )
					{
						TCHAR buffer[256 ] = TEXT("");
						TCHAR* pText = GetTxtFromMgr(2118);
						StringCchPrintf( buffer, 256, pText, m_siItemPrice );
						pText = GetTxtFromMgr(2119);

						cltClient *pclclient = (cltClient *)pclClient;

						stMsgBox MsgBox;
						MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
					}
				}
				break;
			}
		}
		break;

	case NTRADEMERCHANTHIGHCLASS_DIALOG_BUTTON2:	// 닫기
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

void CNTradeMerchantHighClassDlg::Action()
{
	if ( IsInventoryChanged() )
	{
		LoadDish();
	}
}

void CNTradeMerchantHighClassDlg::SetHighClassMerchant()
{
	TCHAR* pText = GetTxtFromMgr(2120);
	m_pEdit->SetText( pText );

	LoadDish();

	SetTimerOnOff(true);
}

void CNTradeMerchantHighClassDlg::LoadDish()
{
	m_siSelectedIndex = -1;

	LVITEM Li;
	Li.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	Li.state = 0;
	Li.stateMask = 0;	

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

		if ( m_pclItem[ i ].siUnique > 0 )
		{			
			if ( m_pclItem[ i ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 ||
				m_pclItem[ i ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1
				)
			{
				if( m_pclItem[i].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
				{
					if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
					{
						TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
						pclClient->pclItemManager->GetExplain( &m_pclItem[ i ], ToolTipMsg );

						stListViewItemData ListViewItemData;
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_pclItem[i].GetItemNum(), ItemName, ToolTipMsg );														
						m_pListView->InsertItem( &ListViewItemData );
					}
				}
			}

		}
	}
}

bool CNTradeMerchantHighClassDlg::IsInventoryChanged()
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



void CNTradeMerchantHighClassDlg::SetDishPrice( SI16 DishItemUnique, GMONEY DishPrice )
{
	cltItem clItem;
	clItem.Init();
	clItem.siUnique = DishItemUnique;
	clItem.siItemNum = 1;

	if(clItem.siUnique)
	{
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( clItem.siUnique, &m_siImageType, &m_siImageIndex ) )
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

	TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( DishItemUnique );
	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2121);
	StringCchPrintf( Buffer, 256, pText, ItemName, DishPrice );
	m_pEdit->SetText(Buffer);

	m_siItemPrice = DishPrice;
}

void CNTradeMerchantHighClassDlg::SellDish( SI16 DishItemUnique )
{
	TCHAR buffer[256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2122);
	StringCchPrintf( buffer, 256, pText, pclClient->pclItemManager->GetName( DishItemUnique ) );
	m_pEdit->SetText(buffer);

	m_siItemPrice = 0;
}

void CNTradeMerchantHighClassDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
                cltGameMsgRequest_TradeMerchantBuyItem clGameMsgRequest_TradeMerchantBuyItem( pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")), m_pclItem[ m_siSelectedIndex ].siUnique, m_siSelectedIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYITEM, sizeof(clGameMsgRequest_TradeMerchantBuyItem), (BYTE*)&clGameMsgRequest_TradeMerchantBuyItem );
				pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );

				m_pStatic[1]->SetFileName(TEXT(""));
			}
			else
				return;
		}
		break;
	}
}