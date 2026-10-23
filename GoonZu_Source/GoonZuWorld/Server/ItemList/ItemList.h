#ifndef _ITEMLIST_H
#define _ITEMLIST_H

#include "ITZObject.h"
#include "ITZList.h"

#include "../../common/Item/ItemCommon/ItemUnit.h"

class cItemNode : public ITZObject
{
public:
	cItemNode() {};
	~cItemNode() {};

public:
	UI16		mapIndex;
	UI16		x;
	UI16		y;

	cltItem		item;

	SI32		siPersonID;
};


class ItemList
{
public:
	ItemList();
	~ItemList();


public:
	void				CreateItemList( UI16 maxSize );

	UI16				PushItem( UI16 mapIndex, UI16 x, UI16 y, cltItem *pItem, SI32 personid );
	
	cItemNode*			GetItem( UI16 itemIndex );

	BOOL				RemoveItem( UI16 itemIndex );


private:

	ITZList< cItemNode >	m_itemList;

	UI16					m_usMaxSize;

};


#endif