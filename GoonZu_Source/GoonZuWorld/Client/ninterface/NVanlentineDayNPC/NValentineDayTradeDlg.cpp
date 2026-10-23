#include "NValentineDayTradeDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-NPC.h"

NValentineDayTradeDlg::NValentineDayTradeDlg()
{
	
}

NValentineDayTradeDlg::~NValentineDayTradeDlg()
{
	
}

void NValentineDayTradeDlg::Destroy()
{ 


}

void NValentineDayTradeDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NValentineDay/NValentineDayTrade.ddf");
	file.CreatePopupDialog( this, VALENTINEDAY_TRADE_DLG, "dialog_valentinedaytrade" , StaticNValentineDayTradeDlgProc );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_TRADE_STATIC_PORTRAIT_IMAGE,	this),  VALENTINEDAY_TRADE_STATIC_PORTRAIT_IMAGE,	TEXT("statictext_portrait")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_TRADE_STATIC_NONAME1,			this),  VALENTINEDAY_TRADE_STATIC_NONAME1,			TEXT("statictext_noname1")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_TRADE_STATIC_NONAME2,			this),  VALENTINEDAY_TRADE_STATIC_NONAME2,			TEXT("statictext_noname2")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_TRADE_STATIC_NONAME3,			this),  VALENTINEDAY_TRADE_STATIC_NONAME3,			TEXT("statictext_noname3")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_TRADE_EDIT_EXPLAIN,			this),  VALENTINEDAY_TRADE_EDIT_EXPLAIN,			TEXT("statictext_explain")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,  		VALENTINEDAY_TRADE_EDIT_ITEM_NUM,			this),  VALENTINEDAY_TRADE_EDIT_ITEM_NUM,			TEXT("editbox_itemnum")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_TRADE_BUTTON_BUY_ITEM,			this),  VALENTINEDAY_TRADE_BUTTON_BUY_ITEM,			TEXT("button_buyitem")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW,  	VALENTINEDAY_TRADE_LISTVIEW_MY_INVEN,		this),  VALENTINEDAY_TRADE_LISTVIEW_MY_INVEN,		TEXT("listview_myinven")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW,  	VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN,		this),  VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN,		TEXT("listview_npcinven")	);

	TCHAR	szPortrait[256];
	SI32	siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_GIFTRACCOON") ), szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_InterfaceFactory.GetStatic( VALENTINEDAY_TRADE_STATIC_PORTRAIT_IMAGE )->SetFileName(szPortrait);
	m_InterfaceFactory.GetStatic( VALENTINEDAY_TRADE_STATIC_PORTRAIT_IMAGE )->SetFontIndex( siPortraitFont );

	m_InterfaceFactory.GetStatic(VALENTINEDAY_TRADE_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(10464) ); 

	m_InterfaceFactory.GetListView(VALENTINEDAY_TRADE_LISTVIEW_MY_INVEN)->SetBorder( true );
	m_InterfaceFactory.GetListView(VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN)->SetBorder( true );

	m_InterfaceFactory.GetEdit( VALENTINEDAY_TRADE_EDIT_ITEM_NUM )->SetNumberMode( true ); 
	m_InterfaceFactory.GetEdit( VALENTINEDAY_TRADE_EDIT_ITEM_NUM )->Refresh();  
	
	SetX( 450 ); 
	SetY( 300 );

	SetMyInven();
	SetNPCInven();
}

void NValentineDayTradeDlg::Action()
{

}

void NValentineDayTradeDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	TSpr* pspr = pclClient->GetGlobalSpr( GIMG_VALENTINEDAY_HEART );
	if( pspr ) 
	{
		GP.PutSprScreen( pspr, GetX(),		 GetY(),	   max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  50, GetY() -  20, max(0, TABS(pclClient->CurrentClock / 120))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 120, GetY() +  50, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 160, GetY() +  10, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  50, GetY() +  70, max(0, TABS(pclClient->CurrentClock / 120))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 120, GetY() + 100, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 160, GetY() + 160, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
	}	
}

void NValentineDayTradeDlg::SetMyInven( void )
{
	CListView* pListView = m_InterfaceFactory.GetListView( VALENTINEDAY_TRADE_LISTVIEW_MY_INVEN );
	pListView->DeleteAllItems();

	SI32 				GlobalImageIndex	= -1;
	SI32 				GlobalImageFont		= -1;
	SI32 				StartPos			= 15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 				id					=  1;
	stListViewItemData  ListViewItemData;
	TCHAR*				ItemName			= NULL;
	pListView->SetStartItemPos( StartPos );

	for ( SI32 i = 0; i < MAX_INVITEM_PER_PERSON+15; ++i )
	{
		cltItem* pclItem = &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ];
		if ( pclItem->siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclItem->siUnique );
				if ( ItemName )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( pclItem, ToolTipMsg );

					if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON )
					{
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, pclItem->siItemNum, ItemName, ToolTipMsg );
					}
				}
			}

			if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON )
				pListView->InsertItem( &ListViewItemData );

		}
	}
}

void NValentineDayTradeDlg::SetNPCInven( void )
{
	cltClient *pclclient = (cltClient *)pclClient;

	const SI32 siNPCKind = pclClient->GetUniqueFromHash( TEXT("KIND_GIFTRACCOON") );
	m_pNPCMerchantInfo = NULL;
	for( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		if( siNPCKind == pclclient->pclNPCItemInfo->clNPCMerchantInfo[k].siNPCKind )
		{
			m_pNPCMerchantInfo = &pclclient->pclNPCItemInfo->clNPCMerchantInfo[ k ];
			break;
		}
	}
	if( NULL == m_pNPCMerchantInfo )	return;

	CListView* pListView = m_InterfaceFactory.GetListView( VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN );
	pListView->DeleteAllItems();

	SI32 				GlobalImageIndex	= -1;
	SI32 				GlobalImageFont		= -1;
	SI32 				StartPos			= 15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 				id					=  1;
	stListViewItemData  ListViewItemData;
	TCHAR*				ItemName			= NULL;
	for ( SI32 i = 0; i < MAX_NPC_ITEM_NUMBER; ++i )
	{		
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pNPCMerchantInfo->clItemInfo[i].clItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			cltItem clItem;
			clItem.siUnique = ITEMUNIQUE(13661);

			ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pNPCMerchantInfo->clItemInfo[i].clItem.siUnique );
			if ( ItemName )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
				pclClient->pclItemManager->GetExplain( &clItem, ToolTipMsg );
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, 0, ItemName, ToolTipMsg );
				pListView->InsertItem( &ListViewItemData );
				pListView->SetItemNumShow( false );
			}
		}
	}

	// 개수.
	m_InterfaceFactory.GetEdit(VALENTINEDAY_TRADE_EDIT_ITEM_NUM)->SetText( "1" );
}



void CALLBACK NValentineDayTradeDlg::StaticNValentineDayTradeDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NValentineDayTradeDlg *pThis = (NValentineDayTradeDlg *)pControllerMgr;
	pThis->NValentineDayTradeDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NValentineDayTradeDlg::NValentineDayTradeDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN:
			{					
				switch( nEvent )
				{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						m_siNPCInvSelectedIndex = m_InterfaceFactory.GetListView( VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN )->GetSelectedItemIndex();
						if( 0 > m_siNPCInvSelectedIndex ) return;

						TCHAR szMoney[128] = "";
						TCHAR* pText = GetTxtFromMgr(611);
						g_JYLibFunction.SetNumUnit( m_pNPCMerchantInfo->clItemInfo[m_siNPCInvSelectedIndex].siPrice, szMoney, sizeof(szMoney), pText );	

						TCHAR szString[128] = "";
						StringCchPrintf( szString, sizeof(szString), GetTxtFromMgr(1170), szMoney );
						m_InterfaceFactory.GetStatic(VALENTINEDAY_TRADE_EDIT_EXPLAIN)->SetText( szString ); 
					}
					break;
				}
			}
			break;

		case VALENTINEDAY_TRADE_BUTTON_BUY_ITEM:
			{
				CListView* pListView = m_InterfaceFactory.GetListView( VALENTINEDAY_TRADE_LISTVIEW_NPC_INVEN );
				stListViewItemData* pstListViewItemData = pListView->GetListViewItemData( m_siNPCInvSelectedIndex );
				if( NULL == pstListViewItemData ) 
				{
					m_InterfaceFactory.GetStatic(VALENTINEDAY_TRADE_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(2239) ); 
					return;
				}

				SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pstListViewItemData->ItemName );
				if( 0 >= siItemUnique ) 
				{
					m_InterfaceFactory.GetStatic(VALENTINEDAY_TRADE_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(2239) ); 
					return;
				}

				SI32 siItemNum = _ttoi( m_InterfaceFactory.GetEdit(VALENTINEDAY_TRADE_EDIT_ITEM_NUM)->GetText() );
				if( 0 >= siItemNum || MAX_ITEM_PILE_NUMBER < siItemNum ) 
				{
					m_InterfaceFactory.GetStatic(VALENTINEDAY_TRADE_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(2237) ); 
					return;
				}
	
				cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
				if( NULL == pclChar ) return;

				if( m_pNPCMerchantInfo->clItemInfo[m_siNPCInvSelectedIndex].siPrice * siItemNum > pclChar->pclCI->clIP.GetMoney() ) 
				{
					m_InterfaceFactory.GetStatic(VALENTINEDAY_TRADE_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(5870) ); 
					return;
				}
				
				cltItem clBuyItem;
				clBuyItem.siUnique  = siItemUnique;
				clBuyItem.siItemNum = siItemNum;
				cltGameRequest_ValentineDay_Buy_Item clinfo( &clBuyItem, m_pNPCMerchantInfo->clItemInfo[m_siNPCInvSelectedIndex].siPrice * siItemNum );
				cltMsg clMsg( GAMEMSG_REQUEST_VALENTINEDAY_BUY_ITEM, PACKET(clinfo) );
				((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);

				m_siNPCInvSelectedIndex = pListView->GetSelectedItemIndex();
			}
			break;
		case VALENTINEDAY_TRADE_EDIT_ITEM_NUM:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_CHANGE:	
					{
						SI32 siItemNum = _ttoi(m_InterfaceFactory.GetEdit(VALENTINEDAY_TRADE_EDIT_ITEM_NUM)->GetText() );
						if( 0 > siItemNum || siItemNum > MAX_ITEM_PILE_NUMBER ) 
						{
							m_InterfaceFactory.GetEdit(VALENTINEDAY_TRADE_EDIT_ITEM_NUM)->SetText( SI32ToString(MAX_ITEM_PILE_NUMBER) );
						}
					} 
					break; 
				}

			}
			break;
			
		default:
			{

			}
			break;
	}
}

