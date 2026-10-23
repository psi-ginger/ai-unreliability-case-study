#ifndef _FOREIGHMERCHANTNPCBUYCASHITEM_H_
#define _FOREIGHMERCHANTNPCBUYCASHITEM_H_

#include "ForeignMerchantNPC.h"

#include "../../../../NLib/NArray.h"

// 한번에 6만원 까지만 거래 가능
#define MAX_AVAILABLE_CASHMONEY			60000

#define MAX_AVAILABLE_CASHITEMNUM		256

class CForeignMerchantNPCBuyCashItem
{
public:
	CForeignMerchantNPCBuyCashItem();
	~CForeignMerchantNPCBuyCashItem();

	bool LoadBuyCashItemInfo();

	void GetItemTypeList( SI16 *ItemTypeList );
	SI16 GetItemUniqueAndPriceListFromItemType( SI16 ItemType, SI16 *ItemUniqueList, UI16 *ItemPriceList );
	bool GetItemTypeCodeList( TCHAR *pItemTypeList );

	bool GetItemPrice( SI16 ItemUnique, UI16 *ItemPrice );

	bool IsSameItemTypeExist( TCHAR *ItemType );

private:
	TCHAR m_strItemTypeList[ MAX_ITEMTYPE_NUMBER ][ 64 ];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_AVAILABLE_CASHITEMNUM>	m_siItemUniqueList;
	NSafeTArray<UI16, MAX_AVAILABLE_CASHITEMNUM>	m_uiItemPriceList;
#else
	SI16 m_siItemUniqueList[ MAX_AVAILABLE_CASHITEMNUM ];
	UI16 m_uiItemPriceList[ MAX_AVAILABLE_CASHITEMNUM ];
#endif
	
};

#endif