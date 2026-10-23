#include "StrSubInventory.h"

#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../Client.h"

#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

NStrSubInventory::NStrSubInventory()
{
	m_pListView = NULL;

	m_pclOrgItem = NULL;
	m_pclItem = NULL;
	m_siMaxItemNum = 0;
	m_siSelectedIndex = -1;
}

NStrSubInventory::~NStrSubInventory()
{
	NDelete_Array( m_pclItem );
}

void NStrSubInventory::CreateInventory( int MaxItemNum, CListView *pListView, cltItem* pclOrgItem )
{
	NDelete_Array( m_pclItem );

	m_pclItem = new cltItem[ MaxItemNum ];

	m_pListView = pListView;

	m_siMaxItemNum = MaxItemNum;

	m_pclOrgItem = pclOrgItem;

}

BOOL NStrSubInventory::IsMyInventoryChanged()
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

void NStrSubInventory::LoadMyInventory()
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

BOOL NStrSubInventory::IsInventoryChanged( cltItem *pOrgItem )
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

void NStrSubInventory::LoadInventory( cltItem *pOrgItem, BOOL bPrice )
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

			if( m_pclItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{

					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_pclItem[ i ], ToolTipMsg );

					if( bPrice ) {
						
						cltMarketAccount* pclaccount = &pclClient->pclCM->CR[1]->pclCI->clMarket.clInfo;
						TCHAR* pText = GetTxtFromMgr(611);
						TCHAR moneybuf[256] ;
						
						g_JYLibFunction.SetNumUnit( pclaccount->clMoney[i].itMoney, moneybuf, 256, pText );
						pText = GetTxtFromMgr(280);

						StringCchCat(ToolTipMsg, MAX_TOOLTIP_MSG_SIZE, pText);
						StringCchCat(ToolTipMsg, MAX_TOOLTIP_MSG_SIZE, moneybuf);
					}

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_pclItem[i].GetItemNum(), ItemName, ToolTipMsg );
					m_pListView->InsertItem( &ListViewItemData );
				}
			}
		}
	}

}

BOOL NStrSubInventory::CheckCurrentSelect( bool numcomp )
{
	if( m_siSelectedIndex < 0 ) return FALSE;

	return m_pclItem[ m_siSelectedIndex ].IsSame( &m_clSelectedItem, numcomp );
}

void NStrSubInventory::SelectAction()
{

	m_siSelectedIndex = m_pListView->GetSelectedItemIndex();

	if( m_siSelectedIndex < 0 ) {

		m_clSelectedItem.Init();

	} else {

		m_clSelectedItem.Set( &m_pclItem[ m_siSelectedIndex ] );
	}

}

int NStrSubInventory::FindIndexFromUnique( int itemunique )
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

