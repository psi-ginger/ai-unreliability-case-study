#include "StrInventory.h"

#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../Client.h"

#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

NStrInventory::NStrInventory()
{
	m_pListView = NULL;

	m_pclOrgItem = NULL;
	m_pclItem = NULL;
	m_pclNewMarketAccount = NULL;
	m_siMaxItemNum = 0;
	m_siSelectedIndex = -1;
}

NStrInventory::~NStrInventory()
{
	NDelete( m_pclNewMarketAccount );
	NDelete_Array( m_pclItem );
}

void NStrInventory::CreateInventory( int MaxItemNum, CListView *pListView, cltItem* pclOrgItem, SI08 Type )
{
	NDelete_Array( m_pclItem );

	m_pclItem = new cltItem[ MaxItemNum ];

	if( SELECT_NEWMARKETACCOUNT == Type )
		m_pclNewMarketAccount = new cltNewMarketAccount;

	m_pListView = pListView;

	m_siMaxItemNum = MaxItemNum;

	m_pclOrgItem = pclOrgItem;

	m_siType = Type;

}

BOOL NStrInventory::IsMyInventoryChanged()
{
	int i, j;
	BYTE* src;
	BYTE* dst;

	for ( j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		src = (BYTE*)&m_pclItem[ j ];
		dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

		for( i = 0; i < size; i++ )
		{
			if( src[i] != dst[i] )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void NStrInventory::LoadMyInventory( bool bFeast )
{
	m_siSelectedIndex = -1;

	SI16 Counter = 0;

	TCHAR buffer[ 256 ] = TEXT("");
	TCHAR ItemName[ 128 ] = TEXT("");

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=15;				// 장착하고 있는 아이템 제외 0~14
	SI32 id = 1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );

	for ( SI32 i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_pclItem[ i ].Set( &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ] );

		//cyj 내자시 일때는 인벤에 오브젝트 물품은 안보이도록 수정.. (예외는 싫은데.. ㅡ.ㅡ;;)
		if (bFeast == true)
		{
			if ( m_pclItem[i].CanPile(pclClient->pclItemManager) == false )
				continue;

			// tys - 거래불가 아이템 안보이도록 수정.
			if ( pclClient->pclItemManager->IsItemInfoAtb( m_pclItem[i].siUnique, ITEMINFOATB_NOTTRADE ))
				continue;
		}

		if ( m_pclItem[ i ].siUnique > 0 && m_pclItem[ i ].siItemNum > 0 )
		{

			if( m_pclItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
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
				}
			}
		}
	}
}


BOOL NStrInventory::IsInventoryChanged( cltItem *pOrgItem )
{
	if( pOrgItem == NULL ) pOrgItem = m_pclOrgItem;

	for ( int j = 0; j < m_siMaxItemNum; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_pclItem[ j ];
		BYTE* dst = (BYTE*)&pOrgItem[j];

		for(int i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void NStrInventory::LoadInventory( cltItem *pOrgItem, BOOL bPrice )
{
	if( pOrgItem == NULL ) pOrgItem = m_pclOrgItem;

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

	for ( SI32 i = 0; i < m_siMaxItemNum; ++i )
	{
		m_pclItem[ i ].Set( &pOrgItem[ i ] );

		if ( m_pclItem[ i ].siUnique > 0 && m_pclItem[ i ].siItemNum > 0 )
		{

			if( m_pclItem[ i ].GetFullName( pclClient->pclItemManager, ItemName , 128) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{

				//	TCHAR* pText = GetTxtFromMgr(676);
				//	sprintf(buffer, pText, ItemName, m_pclItem[i].GetItemNum() );
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_pclItem[ i ], ToolTipMsg );

					if( bPrice ) {
						
						cltMarketAccount* pclaccount = &pclClient->pclCM->CR[1]->pclCI->clMarket.clInfo;
						if ( pclaccount->clMoney[i].itMoney > 0 )
						{

							TCHAR* pText = GetTxtFromMgr(611);
							TCHAR moneybuf[256] ;
							
							g_JYLibFunction.SetNumUnit( pclaccount->clMoney[i].itMoney, moneybuf, 256, pText );
							pText = GetTxtFromMgr(280);

							StringCchCat(ToolTipMsg, MAX_TOOLTIP_MSG_SIZE, pText);
							StringCchCat(ToolTipMsg, MAX_TOOLTIP_MSG_SIZE, moneybuf);
						}
						else if (pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siPrice > 0)
						{
							TCHAR moneybuf[256] ;
							TCHAR *pText = GetTxtFromMgr(6147);

							g_JYLibFunction.SetNumUnit( pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siPrice, moneybuf, 256, pText );

							pText = GetTxtFromMgr(6200);

							StringCchCat(ToolTipMsg, MAX_TOOLTIP_MSG_SIZE, pText);
							StringCchCat(ToolTipMsg, MAX_TOOLTIP_MSG_SIZE, moneybuf);

						}
					}

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_pclItem[i].GetItemNum(), ItemName, ToolTipMsg );
					m_pListView->InsertItem( &ListViewItemData );
				}
			}
		}
	}

}

BOOL NStrInventory::CheckCurrentSelect( bool numcomp )
{
	if( m_siSelectedIndex < 0 ) return FALSE;

	if( SELECT_ITEM == m_siType )
		return m_pclItem[ m_siSelectedIndex ].IsSame( &m_clSelectedItem, numcomp );
	else
		return m_pclNewMarketAccount->clItem[ m_siSelectedIndex ].IsSame( &m_clSelectedItem, numcomp );
}

void NStrInventory::SelectAction()
{
	m_siSelectedIndex = m_pListView->GetSelectedItemIndex();

	if( m_siSelectedIndex < 0 ) 
	{
		m_clSelectedItem.Init();
	} 
	else 
	{
		if( SELECT_ITEM == m_siType)
		{
			m_clSelectedItem.Set( &m_pclItem[ m_siSelectedIndex ] );
		}
		else if( SELECT_NEWMARKETACCOUNT == m_siType )
		{
			m_clSelectedItem.Set( &m_pclNewMarketAccount->clItem[ m_siSelectedIndex ] );
			m_siSelectOrderIndex = m_pclNewMarketAccount->siOrderIndex[m_siSelectedIndex];
		}
	}
}

int NStrInventory::FindIndexFromUnique( int itemunique )
{
	for( int i = 0; i < m_siMaxItemNum; ++i )
	{
		if( m_pclItem[ i ].siUnique == itemunique )
		{
			return i;
		}
	}
	return -1;
}

void NStrInventory::LoadInventory2( cltNewMarketAccount *pNewMarketAccount, BOOL bPrice )
{
	if( pNewMarketAccount == NULL ) 
		return;

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	
	m_siSelectedIndex = -1;

	SI16 Counter = 0;

	char buffer[ 256 ] = "";
	char ItemName[ 128 ] = "";

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );

	m_pclNewMarketAccount->Set( pNewMarketAccount );
	for ( SI32 i = 0; i < m_siMaxItemNum; ++i )
	{
		if ( m_pclNewMarketAccount->clItem[ i ].siUnique > 0 && m_pclNewMarketAccount->clItem[ i ].siItemNum > 0 )
		{

			if( m_pclNewMarketAccount->clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, sizeof(ItemName) ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclNewMarketAccount->clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					pclClient->pclItemManager->GetExplain( &m_pclNewMarketAccount->clItem[ i ], ToolTipMsg );

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_pclNewMarketAccount->clItem[i].GetItemNum(), ItemName, ToolTipMsg );
					m_pListView->InsertItem( &ListViewItemData );
				}
			}
		}
	}
}

void NStrInventory::LoadMyInventory_InternationalTrade()
{
	m_siSelectedIndex = -1;

	SI16 Counter = 0;

	TCHAR buffer[ 256 ] = TEXT("");
	TCHAR ItemName[ 128 ] = TEXT("");

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=15;				// 장착하고 있는 아이템 제외 0~14
	SI32 id = 1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );

	for ( SI32 i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
		if( pclItem == NULL )											continue;

		m_pclItem[ i ].Set( pclItem );

		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pclItem->siUnique );	if( ref < 0 )				continue;
		cltItemInfo* pclItemInfo = pclClient->pclItemManager->pclItemInfo[ref];				if( pclItemInfo == NULL )	continue;

		// 무역거래를 할 수 없는 물건은 추가하지 않는다.
		if( pclItemInfo->siTradeServerMinNum <= 0 )						continue;

		if ( m_pclItem[ i ].siUnique > 0 && m_pclItem[ i ].siItemNum > 0 )
		{
			if( m_pclItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
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
