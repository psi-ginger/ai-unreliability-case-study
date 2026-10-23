#include "ForeignMerchantNPCBuyCashItem.h"

#include "..\..\..\CommonLogic\CommonLogic.h"
#include "NDataLoader.h"

extern cltCommonLogic *pclClient;

CForeignMerchantNPCBuyCashItem::CForeignMerchantNPCBuyCashItem()
{
	LoadBuyCashItemInfo();
}


CForeignMerchantNPCBuyCashItem::~CForeignMerchantNPCBuyCashItem()
{
}

bool CForeignMerchantNPCBuyCashItem::LoadBuyCashItemInfo()
{
#ifdef _SAFE_MEMORY
	m_siItemUniqueList.ZeroMem();
	m_uiItemPriceList.ZeroMem();
#else
	memset( m_siItemUniqueList, 0, sizeof( SI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_uiItemPriceList,  0, sizeof( UI16 ) * MAX_AVAILABLE_CASHITEMNUM );
#endif
	memset( m_strItemTypeList,  0, sizeof(TCHAR)  * MAX_ITEMTYPE_NUMBER * 64  );

	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("Item/Data/CashItemBuyInfoK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/CashItemBuyInfoG.txt") );
		break;

	}

	//KHY - 0809 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( datfilename);
		}
	}


	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadBuyCashItemInfo()"), TEXT("LoadError:[%s]"), datfilename);
		return FALSE;
	}

	SI16 Counter = 0;
	SI16 ItemTypeCounter = 0;

	SI32 ItemUnique = 0;
	TCHAR ItemName[ 64 ] = TEXT("");
	TCHAR ItemType[ 64 ] = TEXT("");
	SI32 ItemPrice = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&ItemUnique,4,		
			NDATA_MBSTRING,	ItemName, 64,
			NDATA_MBSTRING,	ItemType, 64,
			NDATA_INT32,	&ItemPrice, 4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData())
		{
			if ( ItemName[ 0 ] != NULL)
			{
				m_siItemUniqueList[ Counter ] = ItemUnique;
				m_uiItemPriceList[ Counter ] = ItemPrice;

				if ( ! IsSameItemTypeExist( ItemType ) )
				{
					StringCchCopy ( m_strItemTypeList[ ItemTypeCounter++ ], 64, ItemType );
				}

				if ( ++Counter >= MAX_AVAILABLE_CASHITEMNUM )
				{
					break;
				}

			}
		}
	}

	return true;
}

void CForeignMerchantNPCBuyCashItem::GetItemTypeList( SI16 *ItemTypeList )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_CASHITEMNUM; ++i )
	{
		if ( m_siItemUniqueList[ i ] > 0 )
		{
			ItemTypeList[ i ] = pclClient->pclItemManager->GetItemType( m_siItemUniqueList[ i ] );
		}
	}

	return;
}

SI16 CForeignMerchantNPCBuyCashItem::GetItemUniqueAndPriceListFromItemType( SI16 ItemType, SI16 *ItemUniqueList, UI16 *ItemPriceList )
{
	if ( ItemType < 1 || ItemUniqueList == NULL )
	{
		return 0;
	}

	memset( ItemUniqueList, 0, sizeof( SI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( ItemPriceList, 0, sizeof( UI16 ) * MAX_AVAILABLE_CASHITEMNUM );

	SI16 Counter = 0;

	for ( SI16 i = 0; i < MAX_AVAILABLE_CASHITEMNUM; ++i )
	{
		if ( m_siItemUniqueList[ i ] > 0 )
		{
			if ( pclClient->pclItemManager->GetItemType( m_siItemUniqueList[ i ] ) == ItemType )
			{
				ItemUniqueList[ Counter ] = m_siItemUniqueList[ i ];
				ItemPriceList[ Counter ] = m_uiItemPriceList[ i ];
				++Counter;
			}
		}
	}

	return Counter;
}

bool CForeignMerchantNPCBuyCashItem::GetItemTypeCodeList( TCHAR *pItemTypeCodeList )
{
	memcpy ( pItemTypeCodeList, m_strItemTypeList, MAX_ITEMTYPE_NUMBER * 64 );

	return true;
}

bool CForeignMerchantNPCBuyCashItem::GetItemPrice( SI16 ItemUnique, UI16 *ItemPrice )
{
	for( SI16 i = 0; i < MAX_AVAILABLE_CASHITEMNUM; ++i )
	{
		if ( m_siItemUniqueList[ i ] > 0 && m_siItemUniqueList[ i ] == ItemUnique )
		{
			*ItemPrice = m_uiItemPriceList[ i ];
			return true;
		}
	}
	return false;
}

bool CForeignMerchantNPCBuyCashItem::IsSameItemTypeExist( TCHAR *ItemType )
{
	for ( SI16 i = 0; i < MAX_ITEMTYPE_NUMBER; ++i )
	{
		if ( _tcslen( m_strItemTypeList[ i ] ) > 0 )
		{
			if ( _tcscmp( m_strItemTypeList[ i ], ItemType ) == 0  || _tcscmp( m_strItemTypeList[ i ], TEXT("생생탕") ) == 0)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}

	return false;
}
