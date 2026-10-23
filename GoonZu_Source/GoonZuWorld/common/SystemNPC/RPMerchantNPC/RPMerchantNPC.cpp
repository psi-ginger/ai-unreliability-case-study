#include "RPMerchantNPC.h"

extern cltCommonLogic* pclClient;

CRPMerchantNPCMgr::CRPMerchantNPCMgr()
{
	LoadRecommandProductInfo();
}
CRPMerchantNPCMgr::~CRPMerchantNPCMgr()
{
	
}

void CRPMerchantNPCMgr::Init()
{
	

}


bool CRPMerchantNPCMgr::LoadRecommandProductInfo()
{
#ifdef _SAFE_MEMORY
	m_RecommandProduct.ZeroMem();
#else
	memset ( m_RecommandProduct, 0 , sizeof(stRecommandProduct) * MAX_RECOMMAND_PRODUCT);
#endif

	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("Data/RecommandProductK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductT.txt") );
		break;
	case ConstServiceArea_USA :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("Data/RecommandProductG.txt") );
		break;
	}

	//KHY - 0810 - 텍스트 리소스 암호화.
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
		MsgBox(TEXT("LoadRecommandProductInfo()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	SI16 Counter		= 0;

	SI32 ItemUnique		= 0;
	SI32 ItemNum		= 0;
	SI32 ItemUsedate	= 0;
	SI32 NeedPoint		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&ItemUnique,	4,		
		NDATA_INT32,	&ItemNum,		4,		
		NDATA_INT32,	&ItemUsedate,	4,		
		NDATA_INT32,	&NeedPoint,		4,		
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() )
		{
			if ( ItemUnique > 0 )
			{
				m_RecommandProduct[Counter].ItemUnique =  ItemUnique;
				m_RecommandProduct[Counter].ItemNum =	  ItemNum;
				m_RecommandProduct[Counter].ItemUseDate =  ItemUsedate;
				m_RecommandProduct[Counter].NeedPoint =  NeedPoint;

				if ( ++Counter >= MAX_RECOMMAND_PRODUCT )
				{
					break;
				}
			}
		}
	}

	return true;
}
