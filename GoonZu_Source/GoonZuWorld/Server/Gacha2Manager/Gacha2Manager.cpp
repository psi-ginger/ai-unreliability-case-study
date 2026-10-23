#include "Gacha2Manager.h"

extern cltCommonLogic* pclClient;

CGacha2Manager::CGacha2Manager()
{
	Init();
	
}
CGacha2Manager::~CGacha2Manager()
{

}

void CGacha2Manager::Init()
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		LoadGacha2ProductDataFromTxt();		
	}

	//LoadGacha2RareListDataFromTxt();
}


bool CGacha2Manager::LoadGacha2ProductDataFromTxt()
{
	if( pclClient->GameMode == GAMEMODE_CLIENT ) return false;

#ifdef _SAFE_MEMORY
	m_GachaProduct.ZeroMem();
#else
	memset ( m_GachaProduct, 0 , sizeof(stGacha2Product) * MAX_GACHA2_PRODUCT * GACHA2_MAX_KEYTYPE );
#endif

	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("ServerData/Gacha2ProductK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/Gacha2ProductG.txt") );
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
		MsgBox(TEXT("LoadGacha2ProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
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
				if ( ++Counter >= MAX_GACHA2_PRODUCT )
				{
					break;
				}				
			}
			
		}
	}

	// 가차 아이템의 나올 확률을 전부 더했을때 1000 이 되어야 한다.
	if ( GoldGachaRateSum != 1000 || SilverGachaRateSum != 1000 || BronzeGachaRateSum != 1000 || Counter < MAX_GACHA2_PRODUCT )
	{
		MsgBox(TEXT("LoadGacha2ProductDataFromTxt()"), TEXT("LoadError:[%s], GachaData ReadError"), datfilename);
		return false;
	}

	return true;
}
/*
bool CGacha2Manager::LoadGacha2RareListDataFromTxt()
{
#ifdef _SAFE_MEMORY
	m_GachaRareList.ZeroMem();
#else
	memset(m_GachaRareList , 0 , sizeof(SI32) * GACHA2_MAX_RARELIST );
#endif


	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("Item/Data/Gacha2RareItemListK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/Gacha2RareItemListC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/Gacha2RareItemListJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/Gacha2RareItemListE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/Gacha2RareItemListT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/Gacha2RareItemListU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("Item/Data/Gacha2RareItemListC.txt") );
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
		MsgBox(TEXT("LoadGacha2ProductDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
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

// CGachaManager 의 기존 목록을 사용하고, 실제로 사용하지는 않는다.
bool CGacha2Manager::IsGachaRareItem( SI32 ItemUnique )
{
	for( SI16 i = 0 ; i < GACHA2_MAX_RARELIST ; i++ )
	{
		if(m_GachaRareList[i] <= 0)
			return false;

		if(m_GachaRareList[i] == ItemUnique)
			return true;		
	}
	
	return false;
}
*/