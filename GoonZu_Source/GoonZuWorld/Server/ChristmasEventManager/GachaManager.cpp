//#include "GachaManager.h"
//
//extern cltCommonLogic* pclClient;
//
//CGachaManager::CGachaManager()
//{
//	Init();
//	
//}
//CGachaManager::~CGachaManager()
//{
//
//}
//
//void CGachaManager::Init()
//{
//	if(pclClient->GameMode == GAMEMODE_SERVER)
//	{
//		LoadGachaProductDataFromTxt();		
//	}
//
//	LoadGachaRareListDataFromTxt();
//}
//
//
//bool CGachaManager::LoadGachaProductDataFromTxt()
//{
//	if( pclClient->GameMode == GAMEMODE_CLIENT ) return false;
//
//	memset ( m_GachaProduct, 0 , sizeof(stGachaProduct) * MAX_GACHA_PRODUCT * GACHA_MAX_KEYTYPE );
//
//	NDataLoader	dataloader;
//	BOOL bLoadFile=false;
//	TCHAR datfilename[128] = TEXT("");
//
//	switch (pclClient->siServiceArea)
//	{
//	case ConstServiceArea_Korea :
//		StringCchPrintf(datfilename, 128, TEXT("ServerData/GachaProductK.txt") );
//		break;
//	case ConstServiceArea_China :
//		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductC.txt") );
//		break;
//	case ConstServiceArea_Japan :
//		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductJ.txt") );
//		break;
//	case ConstServiceArea_English :
//		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductE.txt") );
//		break;
//	case ConstServiceArea_Taiwan :
//		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductT.txt") );
//		break;
//	case ConstServiceArea_USA  :
//		StringCchPrintf(datfilename,  128, TEXT("ServerData/GachaProductU.txt") );
//		break;
//
//	}
//
//	bLoadFile = dataloader.LoadDataFromFile(datfilename);
//
//	if (!bLoadFile)
//	{
//		MsgBox(TEXT("LoadGachaProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
//		return false;
//	}
//
//	SI16 Counter = 0;
//
//	SI32 ItemUniqueGold = 0;
//	SI32 ItemNumGold = 0;
//	SI32 ItemUsedateGold = 0;
//	SI32 GachaRateGold = 0;
//	
//	SI32 ItemUniqueSilver = 0;
//	SI32 ItemNumSilver = 0;
//	SI32 ItemUsedateSilver = 0;
//	SI32 GachaRateSilver = 0;
//
//	SI32 ItemUniqueBronze = 0;
//	SI32 ItemNumBronze = 0;
//	SI32 ItemUsedateBronze = 0;
//	SI32 GachaRateBronze = 0;
//
//	NDataTypeInfo pDataTypeInfo[] =
//	{
//		NDATA_INT32,	&ItemUniqueGold,4,		
//			NDATA_INT32,	&ItemNumGold,4,		
//			NDATA_INT32,	&ItemUsedateGold,4,		
//			NDATA_INT32,	&GachaRateGold,4,
//			NDATA_INT32,	&ItemUniqueSilver,4,		
//			NDATA_INT32,	&ItemNumSilver,4,		
//			NDATA_INT32,	&ItemUsedateSilver,4,		
//			NDATA_INT32,	&GachaRateSilver,4,
//			NDATA_INT32,	&ItemUniqueBronze,4,		
//			NDATA_INT32,	&ItemNumBronze,4,		
//			NDATA_INT32,	&ItemUsedateBronze,4,		
//			NDATA_INT32,	&GachaRateBronze,4,
//			0,0,0
//	};
//
//	dataloader.RegDataType( pDataTypeInfo );
//
//	SI32 GoldGachaRateSum  = 0;
//	SI32 SilverGachaRateSum  = 0;
//	SI32 BronzeGachaRateSum  = 0;
//
//	while( !dataloader.IsEndOfData() ) {
//
//		if( dataloader.ReadData())
//		{
//			if ( ItemUniqueGold > 0 && ItemUniqueSilver > 0 && ItemUniqueBronze > 0 )
//			{
//				m_GachaProduct[0][Counter].ItemUnique =  ItemUniqueGold;
//				m_GachaProduct[0][Counter].ItemNum =	  ItemNumGold;
//				m_GachaProduct[0][Counter].ItemUseDate =  ItemUsedateGold;
//				m_GachaProduct[0][Counter].GachaRate =  GachaRateGold;
//
//				m_GachaProduct[1][Counter].ItemUnique =  ItemUniqueSilver;
//				m_GachaProduct[1][Counter].ItemNum =	  ItemNumSilver;
//				m_GachaProduct[1][Counter].ItemUseDate =  ItemUsedateSilver;
//				m_GachaProduct[1][Counter].GachaRate =  GachaRateSilver;
//
//				m_GachaProduct[2][Counter].ItemUnique =  ItemUniqueBronze;
//				m_GachaProduct[2][Counter].ItemNum =	  ItemNumBronze;
//				m_GachaProduct[2][Counter].ItemUseDate =  ItemUsedateBronze;
//				m_GachaProduct[2][Counter].GachaRate =  GachaRateBronze;
//
//				GoldGachaRateSum += GachaRateGold;
//				SilverGachaRateSum += GachaRateSilver;
//				BronzeGachaRateSum += GachaRateBronze;
//				if ( ++Counter >= MAX_GACHA_PRODUCT )
//				{
//					break;
//				}				
//			}
//			
//		}
//	}
//
//	// 가차 아이템의 나올 확률을 전부 더했을때 1000 이 되어야 한다.
//	if ( GoldGachaRateSum != 1000 || SilverGachaRateSum != 1000 || BronzeGachaRateSum != 1000 || Counter < MAX_GACHA_PRODUCT )
//	{
//		MsgBox(TEXT("LoadGachaProductDataFromTxt()"), TEXT("LoadError:[%s], GachaData ReadError"), datfilename);
//		return false;
//	}
//
//	return true;
//}
//
//bool CGachaManager::LoadGachaRareListDataFromTxt()
//{
//	memset(m_GachaRareList , 0 , sizeof(SI32) * GACHA_MAX_RARELIST );
//
//
//	NDataLoader	dataloader;
//	BOOL bLoadFile=false;
//	TCHAR datfilename[128] = TEXT("");
//
//	switch (pclClient->siServiceArea)
//	{
//	case ConstServiceArea_Korea :
//		StringCchPrintf(datfilename, 128, TEXT("Item/Data/GachaRareItemListK.txt") );
//		break;
//	case ConstServiceArea_China :
//		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListC.txt") );
//		break;
//	case ConstServiceArea_Japan :
//		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListJ.txt") );
//		break;
//	case ConstServiceArea_English :
//		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListE.txt") );
//		break;
//	case ConstServiceArea_Taiwan :
//		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListT.txt") );
//		break;
//	case ConstServiceArea_USA  :
//		StringCchPrintf(datfilename,  128, TEXT("Item/Data/GachaRareItemListU.txt") );
//		break;
//
//	}
//
//	//KHY - 0809 - 텍스트 리소스 암호화.
//	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
//	{
//		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
//	}
//	else
//	{
//		bLoadFile = dataloader.LoadDataFromFile(datfilename);
//	}
//
//	if (!bLoadFile)
//	{
//		MsgBox(TEXT("LoadGachaProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
//		return false;
//	}
//
//	SI16 Counter = 0;
//
//	SI32 ItemUniqueGold = 0;
//	SI32 ItemNumGold = 0;
//	
//
//	NDataTypeInfo pDataTypeInfo[] =
//	{
//		NDATA_INT32,	&ItemUniqueGold,4,		
//		NDATA_INT32,	&ItemNumGold,4,					
//		0,0,0
//	};
//
//	dataloader.RegDataType( pDataTypeInfo );
//
//	while( !dataloader.IsEndOfData() ) {
//
//		if( dataloader.ReadData())
//		{
//			if ( ItemUniqueGold > 0 )
//			{
//				m_GachaRareList[Counter] = ItemUniqueGold;
//
//				++Counter;				
//			}
//
//		}
//	}
//}
//
//bool CGachaManager::IsGachaRareItem( SI32 ItemUnique )
//{
//	for( SI16 i = 0 ; i < GACHA_MAX_RARELIST ; i++ )
//	{
//		if(m_GachaRareList[i] <= 0)
//			return false;
//
//		if(m_GachaRareList[i] == ItemUnique)
//			return true;		
//	}
//	
//	return false;
//}
