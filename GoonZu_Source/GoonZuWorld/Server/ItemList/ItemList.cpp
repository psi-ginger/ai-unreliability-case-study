#include "ItemList.h"


ItemList::ItemList()
{
}

ItemList::~ItemList()
{
}

void ItemList::CreateItemList( UI16 maxSize )
{
	m_usMaxSize = maxSize;

	m_itemList.CreateList( maxSize, TRUE );
}


UI16 ItemList::PushItem( UI16 mapIndex, UI16 x, UI16 y, cltItem *pItem, SI32 personid )
{
	cItemNode *pItemNode = m_itemList.push();

	if( pItemNode ) {

		pItemNode->mapIndex = mapIndex;
		pItemNode->x = x;
		pItemNode->y = y;
		pItemNode->item.Set( pItem );
		pItemNode->siPersonID = personid;

		return pItemNode->m_usIndex + 1;
	} 

	return 0;
}

cItemNode* ItemList::GetItem( UI16 itemIndex )
{
	cItemNode *pItemNode = m_itemList.get( itemIndex - 1 );

	if( pItemNode == NULL ) return NULL;

	return pItemNode;
}

BOOL ItemList::RemoveItem( UI16 itemIndex )
{
	cItemNode *pItemNode = m_itemList.get( itemIndex - 1 );

	if( pItemNode == NULL ) return FALSE;

	m_itemList.remove( pItemNode );

	return TRUE;
}
