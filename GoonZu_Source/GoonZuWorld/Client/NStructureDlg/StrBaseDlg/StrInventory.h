#ifndef _STRINVENTORY_H
#define _STRINVENTORY_H

#include "StrBaseDlg.h"
#include "item/itemcommon/itemunit.h"

class CListView;

class NStrInventory
{
public:
	enum
	{
		SHOW_ALL = 0,
		SHOW_MARKET,
		SHOW_NEWMARKET,
		SHOW_HUNTCHIEF,
	};


	NStrInventory();
	~NStrInventory();

	void			CreateInventory( int MaxItemNum, CListView *pListView, cltItem *pclOrgItem = NULL, SI08 Type = SELECT_ITEM );
	
	BOOL			IsMyInventoryChanged();
	void			LoadMyInventory( bool bFeast = false );

	BOOL			IsInventoryChanged( cltItem *pOrgItem = NULL );
	void			LoadInventory( cltItem *pOrgItem = NULL,  BOOL bPrice = FALSE );
	void			LoadInventory2( cltNewMarketAccount *pNewMarketAccount = NULL,  BOOL bPrice = FALSE );
	void			LoadMyInventory_InternationalTrade( );

	BOOL			CheckCurrentSelect( bool numcomp = true );

	int				FindIndexFromUnique( int itemunique );

	void			SelectAction();
	
	SI32			GetSelectOrderIndex()
	{
		return m_siSelectOrderIndex;
	}

public:
	enum
	{
		SELECT_ITEM = 0,
		SELECT_NEWMARKETACCOUNT
	};

	CListView*		m_pListView;

	cltItem*		m_pclOrgItem;
	cltItem*		m_pclItem;

	cltNewMarketAccount*	m_pclNewMarketAccount;

	SI08			m_siType;
	SI32			m_siMaxItemNum;
	SI32			m_siSelectedIndex;
	
	cltItem			m_clSelectedItem;
	SI32			m_siSelectOrderIndex;
};

#endif