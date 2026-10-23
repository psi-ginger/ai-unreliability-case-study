#include "ForleafManager.h"

extern cltCommonLogic* pclClient;

CFourLeafManager::CFourLeafManager()
{
	Init();
	
}
CFourLeafManager::~CFourLeafManager()
{

}

void CFourLeafManager::Init()
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		LoadFourleafProductDataFromTxt();		
	}
}


bool CFourLeafManager::LoadFourleafProductDataFromTxt()
{
	if( pclClient->GameMode == GAMEMODE_CLIENT ) return false;

#ifdef _SAFE_MEMORY
	m_FourleafProduct.ZeroMem();
#else
	memset ( m_FourleafProduct, 0 , sizeof(stFourleafProduct));
#endif

	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datafilename[128] = TEXT("");
	
	SI16 Counter = 0;
	SI32 ItemUnique  = 0;
	SI32 ItemNum = 0;

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datafilename, 128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datafilename,  128, TEXT("ServerData/fourleafmanager.txt") );
		break;
	}

	bLoadFile = dataloader.LoadDataFromFile(datafilename);

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadFourleafProductDataFromTxt()"), TEXT("LoadError:[%s]"), datafilename);
		return false;
	}
#ifdef _SAFE_MEMORY
	m_100healmanager.ZeroMem();
#else
	memset(m_100healmanager,0,sizeof(SI32) * MAX_UNIQUENUM);
#endif
	//KHY - 0809 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datafilename);							
	}
    
	NDataTypeInfo pDataTypeInfo[] = 
	{
		NDATA_INT32,	&ItemUnique,4,
		NDATA_INT32,	&ItemNum,4,
			0,0,0
	};
	
	dataloader.RegDataType(pDataTypeInfo);

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData())
		{
			if ( ItemUnique > 0 )
			{
				m_100healmanager[Counter] = ItemUnique;
				++Counter;				
			}

		}
	}

	return true;
}

bool CFourLeafManager::IsFourleafRareItem( SI32 ItemUnique )
{
	for( SI16 i = 0 ; i < Fourleaf_MAX_LIST ; i++ )
	{
		if(m_100healmanager[i] <= 0)	return false;
		if(m_100healmanager[i] == ItemUnique)	return true;		
	}

	return false;
}

