#include "Item-GodHand.h"
#include "NDataLoader.h"
#include "NFile.h"
#include "NUtil.h"

#include "../../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

CItemGodHandManager::CItemGodHandManager()
{
	// 미리 20개 정도만 만들어준다.
	m_vGodHandManager.reserve( 20 );
}

CItemGodHandManager::~CItemGodHandManager()
{
	for( vIterGodHandDataInfo i = m_vGodHandManager.begin(); i != m_vGodHandManager.end(); ++i )
	{
		delete *i;
	}
}

void CItemGodHandManager::LoadFile()
{
	NDataLoader	dataloader;
	bool bLoadFile=false;
	TCHAR fileName[128] = {'\0'};

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(fileName, 128, TEXT("ServerData/GodHandItemListK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(fileName,  128, TEXT("ServerData/GodHandItemListG.txt") );
		break;
	}

	bLoadFile = dataloader.LoadDataFromFile( fileName) ;

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadGodHandItemList()"), TEXT("LoadError:[%s]"), fileName);
		return;
	}

	SI32 itemUnique = 0;
	SI32 itemNumber = 0;
	SI32 itemRate = 0;
	SI32 showAllUser = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&itemUnique,  4,		
		NDATA_INT32,	&itemNumber,  4,		
		NDATA_INT32,	&itemRate,    4,		
		NDATA_INT32,	&showAllUser, 4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 GoldGachaRateSum  = 0;
	SI32 SilverGachaRateSum  = 0;
	SI32 BronzeGachaRateSum  = 0;

	SI32 plusItemRate = 0;
	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			bool flag = showAllUser > 0 ? true : false;
			plusItemRate += itemRate;
			m_vGodHandManager.push_back( new CGodHandData( itemUnique, itemNumber, plusItemRate, flag ) );
		}
	}
}

CGodHandData* CItemGodHandManager::FindItem( SI32 RateValue )
{
	vIterGodHandDataInfo i = m_vGodHandManager.begin();
	for( i ; i != m_vGodHandManager.end(); ++i )
	{
		if( (*i)->GetRate() > RateValue )
			return (*i);
	}
	return NULL;
}
