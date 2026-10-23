#ifndef _STRSUBINVENTORY_H
#define _STRSUBINVENTORY_H

#include "StrSubBaseDlg.h"
#include "item/itemcommon/itemunit.h"

class CListView;

class NStrSubInventory
{
public:
	NStrSubInventory();
	~NStrSubInventory();

	void			CreateInventory( int MaxItemNum, CListView *pListView, cltItem *pclOrgItem = NULL );
	
	BOOL			IsMyInventoryChanged();
	void			LoadMyInventory();

	BOOL			IsInventoryChanged( cltItem *pOrgItem = NULL );
	void			LoadInventory( cltItem *pOrgItem = NULL,  BOOL bPrice = FALSE );

	BOOL			CheckCurrentSelect( bool numcomp = true );

	int				FindIndexFromUnique( int itemunique );

	void			SelectAction();

public:
	CListView*		m_pListView;

	cltItem*		m_pclOrgItem;
	cltItem*		m_pclItem;

	SI32			m_siMaxItemNum;
	SI32			m_siSelectedIndex;
	
	cltItem			m_clSelectedItem;
};

#endif