#include "GachaManager.h"

extern cltCommonLogic* pclClient;

CGachaManager::CGachaManager()
{
	Init();
}

CGachaManager::~CGachaManager()
{

}

void CGachaManager::Init()
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		LoadGachaProductDataFromTxt();

		if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
		{
			LoadCarryOverGachaProductDataFromTxt();
		}
		if ( pclClient->IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) )
		{
			LoadGachaTimePeriodFromTxt();
		}
	}

	LoadGachaRareListDataFromTxt();
}


bool CGachaManager::LoadGachaProductDataFromTxt()
{
	if( pclClient->GameMode == GAMEMODE_CLIENT ) return false;

#ifdef _SAFE_MEMORY
	m_GachaProduct.ZeroMem();
#else
	memset ( m_GachaProduct, 0 , sizeof(stGachaProduct) * MAX_GACHA_PRODUCT * GACHA_MAX_KEYTYPE );
#endif

	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("ServerData/GachaProductK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductG.txt") );
		break;

	}

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datfilename);							
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadGachaProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	SI16 Counter = 0;

	SI32 ItemUniqueGold = 0;
	SI32 ItemNumGold = 0;
	SI32 ItemUsedateGold = 0;
	SI32 GachaRateGold = 0;
	
	SI32 ItemUniqueSilver = 0;
	SI32 ItemNumSilver = 0;
	SI32 ItemUsedateSilver = 0;
	SI32 GachaRateSilver = 0;

	SI32 ItemUniqueBronze = 0;
	SI32 ItemNumBronze = 0;
	SI32 ItemUsedateBronze = 0;
	SI32 GachaRateBronze = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&ItemUniqueGold,4,		
			NDATA_INT32,	&ItemNumGold,4,		
			NDATA_INT32,	&ItemUsedateGold,4,		
			NDATA_INT32,	&GachaRateGold,4,
			NDATA_INT32,	&ItemUniqueSilver,4,		
			NDATA_INT32,	&ItemNumSilver,4,		
			NDATA_INT32,	&ItemUsedateSilver,4,		
			NDATA_INT32,	&GachaRateSilver,4,
			NDATA_INT32,	&ItemUniqueBronze,4,		
			NDATA_INT32,	&ItemNumBronze,4,		
			NDATA_INT32,	&ItemUsedateBronze,4,		
			NDATA_INT32,	&GachaRateBronze,4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 GoldGachaRateSum  = 0;
	SI32 SilverGachaRateSum  = 0;
	SI32 BronzeGachaRateSum  = 0;

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData())
		{
			if ( ItemUniqueGold > 0 && ItemUniqueSilver > 0 && ItemUniqueBronze > 0 )
			{
				m_GachaProduct[0][Counter].ItemUnique =  ItemUniqueGold;
				m_GachaProduct[0][Counter].ItemNum =	  ItemNumGold;
				m_GachaProduct[0][Counter].ItemUseDate =  ItemUsedateGold;
				m_GachaProduct[0][Counter].GachaRate =  GachaRateGold;

				m_GachaProduct[1][Counter].ItemUnique =  ItemUniqueSilver;
				m_GachaProduct[1][Counter].ItemNum =	  ItemNumSilver;
				m_GachaProduct[1][Counter].ItemUseDate =  ItemUsedateSilver;
				m_GachaProduct[1][Counter].GachaRate =  GachaRateSilver;

				m_GachaProduct[2][Counter].ItemUnique =  ItemUniqueBronze;
				m_GachaProduct[2][Counter].ItemNum =	  ItemNumBronze;
				m_GachaProduct[2][Counter].ItemUseDate =  ItemUsedateBronze;
				m_GachaProduct[2][Counter].GachaRate =  GachaRateBronze;

				GoldGachaRateSum += GachaRateGold;
				SilverGachaRateSum += GachaRateSilver;
				BronzeGachaRateSum += GachaRateBronze;
				if ( ++Counter >= MAX_GACHA_PRODUCT )
				{
					break;
				}				
			}
			
		}
	}

	// 가차 아이템의 나올 확률을 전부 더했을때 1000 이 되어야 한다.
	if ( GoldGachaRateSum != 1000 || SilverGachaRateSum != 1000 || BronzeGachaRateSum != 1000 || Counter < MAX_GACHA_PRODUCT )
	{
		MsgBox(TEXT("LoadGachaProductDataFromTxt()"), TEXT("LoadError:[%s], GachaData ReadError"), datfilename);
		return false;
	}

	return true;
}

bool CGachaManager::LoadCarryOverGachaProductDataFromTxt()
{
	if ( GAMEMODE_CLIENT == pclClient->GameMode ) 
	{
		return false;
	}

#ifdef _SAFE_MEMORY
	m_CarryOverProduct.ZeroMem();
#else
	ZeroMemory(m_CarryOverProduct, sizeof(m_CarryOverProduct));
#endif

	// 파일 읽어오기
	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("ServerData/CarryOverGachaProductK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/CarryOverGachaProductG.txt") );
		break;

	}

	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datfilename );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadCarryOverGachaProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	// 파일 읽어오기 여기까지

	SI32 siIndex			= 0;

	SI32 siGoldUnique		= 0;
	SI32 siGoldNum			= 0;
	SI32 siGoldUsedate		= 0;
	SI32 siGoldRate			= 0;

	SI32 siSilverUnique		= 0;
	SI32 siSilverNum		= 0;
	SI32 siSilverUsedate	= 0;
	SI32 siSilverRate		= 0;

	SI32 siBronzeUnique		= 0;
	SI32 siBronzeNum		= 0;
	SI32 siBronzeUsedate	= 0;
	SI32 siBronzeRate		= 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,			4,

		NDATA_INT32,	&siGoldUnique,		4,
		NDATA_INT32,	&siGoldNum,			4,
		NDATA_INT32,	&siGoldUsedate,		4,
		NDATA_INT32,	&siGoldRate,		4,

		NDATA_INT32,	&siSilverUnique,	4,	
		NDATA_INT32,	&siSilverNum,		4,		
		NDATA_INT32,	&siSilverUsedate,	4,	
		NDATA_INT32,	&siSilverRate,		4,

		NDATA_INT32,	&siBronzeUnique,	4,	
		NDATA_INT32,	&siBronzeNum,		4,
		NDATA_INT32,	&siBronzeUsedate,	4,	
		NDATA_INT32,	&siBronzeRate,		4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 siCounter	= 0;
	SI32 siOldIndex	= -1;

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if ( (0 > siIndex) || (MAX_CARRYOVER_PRODUCT <= siIndex) )
			{
				continue;
			}

			if ( siOldIndex != siIndex )
			{
				siCounter	= 0;
				siOldIndex	= siIndex;
			}

			if ( MAX_GACHA_PRODUCT <= siCounter )
			{
			#ifdef _DEBUG
				MsgBox(TEXT("이월가챠 상품 오류"), TEXT("이월 가챠 상품중 한 상품의 갯수가 9개를 초과 했습니다") );
			#endif
			}

			m_CarryOverProduct[siIndex].SetGoldKeyItemInfo(		siCounter, siGoldUnique,	siGoldNum,		siGoldUsedate,		siGoldRate );
			m_CarryOverProduct[siIndex].SetSilverKeyItemInfo(	siCounter, siSilverUnique,	siSilverNum,	siSilverUsedate,	siSilverRate );
			m_CarryOverProduct[siIndex].SetBronzeKeyItemInfo(	siCounter, siBronzeUnique,	siBronzeNum,	siBronzeUsedate,	siBronzeRate );

			siCounter++;
			if ( MAX_GACHA_PRODUCT == siCounter )
			{
				if ( false == m_CarryOverProduct[siIndex].IsCorrectRateSum() )
				{
					MsgBox(TEXT("이월가챠 상품 오류"), TEXT("[%d]번째 상품의 가챠 비율의 합이 1000이 아닙니다 "), siIndex );
					return false;
				}
			}
		}
	}


	return true;
}

bool CGachaManager::LoadGachaTimePeriodFromTxt()
{
	if ( GAMEMODE_CLIENT == pclClient->GameMode ) 
	{
		return false;
	}

#ifdef _SAFE_MEMORY
	m_GacharareTimePeriod.ZeroMem();
#else
	ZeroMemory(m_GacharareTimePeriod, sizeof(m_GacharareTimePeriod));
#endif

	// 파일 읽어오기
	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("ServerData/GachaTimePeriodK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaTimePeriodG.txt") );
		break;

	}

	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datfilename );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadGachaTimePeriodTxt()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	// 파일 읽어오기 여기까지

	SI32 siIndex	= 0;

	SI32 siUnique	= 0;
	
	SI32 sisYear	= 0;
	SI32 sisMonth	= 0;
	SI32 sisDay		= 0;
	SI32 sisHour	= 0;
	SI32 sisMin		= 0;

	SI32 sieYear	= 0;
	SI32 sieMonth	= 0;
	SI32 sieDay		= 0;
	SI32 sieHour	= 0;
	SI32 sieMin		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,		4,

			NDATA_INT32,	&siUnique,	4,

			NDATA_INT32,	&sisYear,	4,
			NDATA_INT32,	&sisMonth,	4,
			NDATA_INT32,	&sisDay,	4,
			NDATA_INT32,	&sisHour,	4,
			NDATA_INT32,	&sisMin,	4,

			NDATA_INT32,	&sieYear,	4,
			NDATA_INT32,	&sieMonth,	4,
			NDATA_INT32,	&sieDay,	4,
			NDATA_INT32,	&sieHour,	4,
			NDATA_INT32,	&sieMin,	4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 siCounter	= 0;
	SI32 siOldIndex	= -1;

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if ( (0 > siIndex) || (MAX_CHANGERARE_PRODUCT <= siIndex) )
			{
				continue;
			}

			if ( siOldIndex != siIndex )
			{
				siCounter	= 0;
				siOldIndex	= siIndex;
			}

			if ( MAX_CHANGERARE_PRODUCT <= siCounter )
			{
#ifdef _DEBUG
				MsgBox(TEXT("가차레어아이템데이터변경오류"), TEXT("변경아이템이 12개를 초과함") );
#endif
				break	;
			}

			m_GacharareTimePeriod[siIndex].SetRaredata(	siCounter, siUnique, sisYear, sisMonth, sisDay, sisHour, sisMin, sieYear, sieMonth, sieDay, sieHour, sieMin );
			siCounter++;
		}
	}
	return true;
}

bool CGachaManager::LoadGachaRareListDataFromTxt()
{
#ifdef _SAFE_MEMORY
	m_GachaRareList.ZeroMem();
#else
	memset(m_GachaRareList , 0 , sizeof(SI32) * GACHA_MAX_RARELIST );
#endif


	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("Item/Data/GachaRareItemListK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListG.txt") );
		break;

	}

	//KHY - 0809 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datfilename);
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadGachaProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	SI16 Counter = 0;

	SI32 ItemUniqueGold = 0;
	SI32 ItemNumGold = 0;
	

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&ItemUniqueGold,4,		
		NDATA_INT32,	&ItemNumGold,4,					
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData())
		{
			if ( ItemUniqueGold > 0 )
			{
				m_GachaRareList[Counter] = ItemUniqueGold;

				++Counter;				
			}

		}
	}

	return true;
}

bool CGachaManager::IsGachaRareItem( SI32 ItemUnique )
{
	for( SI16 i = 0 ; i < GACHA_MAX_RARELIST ; i++ )
	{
		if(m_GachaRareList[i] <= 0)
			return false;

		if(m_GachaRareList[i] == ItemUnique)
			return true;		
	}
	
	return false;
}

SI32 CGachaManager::GetCarryOverGachaItemUnique( const SI32 siIndex )
{
	if ( (0 > siIndex) || (MAX_CARRYOVER_PRODUCT <= siIndex) )
	{
		return 0;
	}

	return m_CarryOverProduct[siIndex].m_stGachaCarryOver[0][0].ItemUnique;
}

bool CGachaManager::IsPare( const SI32 siIndex, const SI32 siItemUnique )
{
	if ( (0 > siIndex) || (TOTAL_GACHA_PRODUCT <= siIndex) )
	{
		return false;
	}

	if ( 0 == siIndex )
	{
		if ( siItemUnique == m_GachaProduct[0][0].ItemUnique )
		{
			return true;
		}
	}
	else
	{
		SI32 siRealIndex = siIndex - 1;

		if ( siItemUnique == m_CarryOverProduct[siRealIndex].m_stGachaCarryOver[0][0].ItemUnique )
		{
			return true;
		}
	}

	return false;
}

void CGachaManager::GetGachaProductFromIndex( IN const SI32 siIndex, IN const SI32 siKeyKind, OUT SI32* psiProductList )
{
	if ( (0 > siIndex) || (TOTAL_GACHA_PRODUCT <= siIndex) )
	{
		return;
	}

	if ( NULL == psiProductList )
	{
		return;
	}

	SI32 siKeyIndex = -1;

	switch ( siKeyKind )
	{
	case GACHA_USE_GOLDKEY:
	case CARRYOVER_USE_GOLDKEY:
		{
			siKeyIndex = 0;
		}
		break;

	case GACHA_USE_SILVERKEY:
	case CARRYOVER_USE_SILVERKEY:
		{
			siKeyIndex = 1;
		}
		break;

	case GACHA_USE_BRONZEKEY:
	case CARRYOVER_USE_BRONZEKEY:
		{
			siKeyIndex = 2;
		}
		break;

	default:
		{
			return;
		}
		break;
	}

	if ( 0 == siIndex )
	{

		for ( SI32 siCount=0; siCount<MAX_GACHA_PRODUCT; siCount++ )
		{
			psiProductList[siCount] = m_GachaProduct[siKeyIndex][siCount].ItemUnique;
		}
	}
	else
	{
		SI32 siRealIndex = siIndex - 1;

		for ( SI32 siCount=0; siCount<MAX_GACHA_PRODUCT; siCount++ )
		{
			psiProductList[siCount] = m_CarryOverProduct[siRealIndex].m_stGachaCarryOver[siKeyIndex][siCount].ItemUnique;
		}
	}
}

void CGachaManager::GetGachaItem( IN const SI32 siIndex, IN const SI32 siKeyKind, IN const SI32 siProductIndex, OUT cltItem* pclGetItem )
{
	if ( (0 > siIndex) || (TOTAL_GACHA_PRODUCT <= siIndex) )
	{
		return;
	}

	if ( (0 > siProductIndex) || (MAX_GACHA_PRODUCT <= siProductIndex) )
	{
		return;
	}

	if ( NULL == pclGetItem )
	{
		return;
	}

	SI32 siKeyIndex = -1;

	switch ( siKeyKind )
	{
	case GACHA_USE_GOLDKEY:
	case CARRYOVER_USE_GOLDKEY:
		{
			siKeyIndex = 0;
		}
		break;

	case GACHA_USE_SILVERKEY:
	case CARRYOVER_USE_SILVERKEY:
		{
			siKeyIndex = 1;
		}
		break;

	case GACHA_USE_BRONZEKEY:
	case CARRYOVER_USE_BRONZEKEY:
		{
			siKeyIndex = 2;
		}
		break;

	default:
		{
			return;
		}
		break;
	}

	if ( 0 == siIndex )
	{
		pclGetItem->siUnique		= m_GachaProduct[siKeyIndex][siProductIndex].ItemUnique;
		pclGetItem->siItemNum		= m_GachaProduct[siKeyIndex][siProductIndex].ItemNum;
		pclGetItem->uiDateUseDay	= m_GachaProduct[siKeyIndex][siProductIndex].ItemUseDate;
	}
	else
	{
		SI32 siRealIndex = siIndex - 1;

		pclGetItem->siUnique		= m_CarryOverProduct[siRealIndex].m_stGachaCarryOver[siKeyIndex][siProductIndex].ItemUnique;
		pclGetItem->siItemNum		= m_CarryOverProduct[siRealIndex].m_stGachaCarryOver[siKeyIndex][siProductIndex].ItemNum;
		pclGetItem->uiDateUseDay	= m_CarryOverProduct[siRealIndex].m_stGachaCarryOver[siKeyIndex][siProductIndex].ItemUseDate;
	}

}

SI32 CGachaManager::GetGachaRate( const SI32 siIndex, const SI32 siKeyKind, const SI32 siProductIndex )
{
	if ( (0 > siIndex) || (TOTAL_GACHA_PRODUCT <= siIndex) )
	{
		return 0;
	}

	if ( (0 > siProductIndex) || (MAX_GACHA_PRODUCT <= siProductIndex) )
	{
		return 0;
	}

	SI32 siKeyIndex = -1;

	switch ( siKeyKind )
	{
	case GACHA_USE_GOLDKEY:
	case CARRYOVER_USE_GOLDKEY:
		{
			siKeyIndex = 0;
		}
		break;

	case GACHA_USE_SILVERKEY:
	case CARRYOVER_USE_SILVERKEY:
		{
			siKeyIndex = 1;
		}
		break;

	case GACHA_USE_BRONZEKEY:
	case CARRYOVER_USE_BRONZEKEY:
		{
			siKeyIndex = 2;
		}
		break;

	default:
		{
			return 0;
		}
		break;
	}

	SI32 siReturnRate = 0;

	if ( 0 == siIndex )
	{
		siReturnRate = m_GachaProduct[siKeyIndex][siProductIndex].GachaRate;
	}
	else
	{
		SI32 siRealIndex = siIndex - 1;

		siReturnRate = m_CarryOverProduct[siRealIndex].m_stGachaCarryOver[siKeyIndex][siProductIndex].GachaRate;
	}

	return siReturnRate;
}

UI08 CGachaManager::GetCachaCntAddNum( const SI32 siKeyKind )
{
	UI08 uiReturnAddNum = 0;

	switch ( siKeyKind )
	{
	case GACHA_USE_GOLDKEY:
	case CARRYOVER_USE_GOLDKEY:
		{
			uiReturnAddNum = (UI08)pclClient->GetGlobalValue( TEXT("GV_GachaGoldKeyBonus") );
		}
		break;

	case GACHA_USE_SILVERKEY:
	case CARRYOVER_USE_SILVERKEY:
		{
			uiReturnAddNum = (UI08)pclClient->GetGlobalValue( TEXT("GV_GachaSilverKeyBonus") );
		}
		break;

	case GACHA_USE_BRONZEKEY:
	case CARRYOVER_USE_BRONZEKEY:
		{
			uiReturnAddNum = (UI08)pclClient->GetGlobalValue( TEXT("GV_GachaBronzeKeyBonus") );
		}
		break;
	}

	return uiReturnAddNum;
}