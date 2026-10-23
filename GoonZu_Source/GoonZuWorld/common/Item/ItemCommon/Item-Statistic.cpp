//---------------------------------
// 2004/5/19 김태곤
//---------------------------------
#include "cltItem.h"
#include "CommonLogic.h"

void cltItemManagerCommon::WriteStatisticAction(_SYSTEMTIME sTime)
{
	TCHAR fileNameBuffer[256];
	fileNameBuffer[0] = 0;

	// 서버가 켜진후 기존 백업된 데이터를 읽어 온다

	static bool backUpDataLoaded = false;
	static SI32 lastWrittenDay = 0;

	if( backUpDataLoaded == false )
	{
		ReadStatisticBackUp( TEXT("Config\\Item_backup.bak") , sTime);

		backUpDataLoaded = true;
		lastWrittenDay	= sTime.wDay; // 초기화된 날짜를 마지막 업데이트 
	}

	// 날짜가 바뀌면 일단위 통계 데이터를 초기화 해준다.
	if( lastWrittenDay != sTime.wDay )
	{
		clItemStatistic.InitStatistic();
		lastWrittenDay = sTime.wDay;
	}
	
	// 통계 데이터를 쓴다
	if ( ConstServiceArea_China == pclClient->siServiceArea )
	{
		StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\Item\\Item_%04d%02d%02d.log") , sTime.wYear , sTime.wMonth , sTime.wDay );
	}
	else
	{
		StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\Item_%04d%02d%02d.log") , sTime.wYear , sTime.wMonth , sTime.wDay );
	}

	WriteStatistic( fileNameBuffer );
    
	// 백업 통계 데이터를 쓴다
	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\Item_backup.bak") , sTime.wYear , sTime.wMonth , sTime.wDay );
	WriteStatisticBackUp( fileNameBuffer , sTime  );

}

void cltItemManagerCommon::WriteStatistic(TCHAR * filename)
{
	SI32 i;

	FILE *fp = _tfopen( filename, TEXT("wt") );

	if ( fp == NULL )
	{
		return;
	}

	// 맨 첫줄. 
	//cyj 생산술에 의한 통계 컬럼 추가
	_ftprintf( fp, TEXT("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n"), 
					TEXT("Unique"), 
					TEXT("Name"), 
					TEXT("TradeNum"), 
					TEXT("UseNum"), 
					TEXT("MakeNum"), 
					TEXT("TuneNum"), 
					TEXT("FromTuneNum"), 
					TEXT("MonNum"), 
					TEXT("EquipNum"), 
					TEXT("Quest"), 
					TEXT("ItemMall"), 
					TEXT("ProductionNum"), 
					TEXT("Price"), 
					TEXT("Trade Income"), 
					TEXT("Trade Export"), 
					TEXT("ForeignMerchant"), 
					TEXT("BookBox"), 
					TEXT("ELIXIREquip"), 
					TEXT("MagicalNostrumNUM") );

	for (i = 0; i < MAX_ITEMINFO_NUMBER; ++i )
	{
		if(pclItemInfo[i] == NULL)continue;

		GMONEY siPrice = 0;
		cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, pclItemInfo[i]->clItem.siUnique);
		if (pclprice)
			siPrice = pclprice->siPrice;

		_ftprintf( fp, TEXT("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%I64d\t%d\t%d\t%d\t%d\t%d\t%d\n"), 
			pclItemInfo[i]->clItem.siUnique,
			pclItemInfo[i]->GetName(), 
			clItemStatistic.siItemTradeStatistic[i], 
			clItemStatistic.siItemUseStatistic[i], 
			clItemStatistic.siItemMakeStatistic[i], 
			clItemStatistic.siItemTuneStatistic[i],
			clItemStatistic.siItemFromTuneStatistic[i],
			clItemStatistic.siItemMonStatistic[i],
			clItemStatistic.siItemEquipStatistic[i],
			clItemStatistic.siItemQuestStatistic[i],
			clItemStatistic.siItemMallStatistic[i],
			clItemStatistic.siItemProductionStatistic[i],
			siPrice,
			clItemStatistic.siItemWorldTradeIncomeStatistic[i],
			clItemStatistic.siItemWorldTradeExportStatistic[i],
			clItemStatistic.siForeignMerchantStatistics[i],
			clItemStatistic.siBookBoxStatistics[i],
			clItemStatistic.siElixirStatistics[i],
			clItemStatistic.siMagicalNostrumStatistics[i]
			);
	}
	
	fclose( fp );

	return;
}

// 통계 데이터를 파일로 백업시킨다 ( 서버를 다시 켜도 유지되게끔 )
void cltItemManagerCommon::WriteStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime )
{
	FILE *fp = _tfopen( filename , TEXT("wb") );

	if ( fp == NULL )
	{
		return;
	}

	stServerBackUpFileHeader clBackUpFileHeader;
	StringCchPrintf(clBackUpFileHeader.szDesc, 32 , TEXT("Statistics%04d%02d%02d"), sTime.wYear , sTime.wMonth , sTime.wDay );
	clBackUpFileHeader.siVersion = SERVER_BACKUPFILE_HEADER_VERSION;

	fwrite(&clBackUpFileHeader , sizeof(stServerBackUpFileHeader) , 1 , fp);
	fwrite(&clItemStatistic , sizeof(cltItemStatistic) , 1 , fp);

	fclose( fp );
	
}

void cltItemManagerCommon::ReadStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime )
{

	TCHAR buffer[256];
	buffer[0] = 0;

	FILE *fp = _tfopen( filename , TEXT("rb") );

	if ( fp == NULL )
	{
		return;
	}

	// 물가지수와, NPC 물가는 계속 유지 시킨다.
	SI32 npcPriceRate =  clItemStatistic.siNPCPriceRate;
	GMONEY itemPriceIndex = clItemStatistic.siItemPriceIndex;

	clItemStatistic.InitStatistic();
    
	stServerBackUpFileHeader clBackUpFileHeader;
	memset( &clBackUpFileHeader , 0 , sizeof(stServerBackUpFileHeader) );

	fread( &clBackUpFileHeader , sizeof(stServerBackUpFileHeader) , 1 , fp );
	clBackUpFileHeader.szDesc[31] = 0; // Insert NULL

	// 오늘 날짜를 문자열로 만들어 헤더 정보와 비교

	StringCchPrintf( buffer, 256 , TEXT("Statistics%04d%02d%02d"), sTime.wYear , sTime.wMonth , sTime.wDay );


	// 파일의 헤더가 적절하지 않다면 읽지 않는다.
	if( _tcscmp( clBackUpFileHeader.szDesc , buffer ) != 0 ) return;


	// 파일 버젼이 다르면 읽지 않는다
	if( clBackUpFileHeader.siVersion != SERVER_BACKUPFILE_HEADER_VERSION ) return;

	fread( &clItemStatistic , sizeof( cltItemStatistic ) , 1 , fp );
	
	clItemStatistic.siNPCPriceRate = npcPriceRate;
	clItemStatistic.siItemPriceIndex = itemPriceIndex;

	fclose( fp );

}


// 아이템 거래 통계를 낸다. 
void cltItemManagerCommon::AddTradeNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemTradeStatistic[ref] +=	itemnum;
}

// 아이템 사용 통계를 낸다. 
void cltItemManagerCommon::AddUseNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemUseStatistic[ref] +=	itemnum;

}

// 아이템 제작 통계를 낸다. 
void cltItemManagerCommon::AddMakeNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemMakeStatistic[ref] +=	itemnum;
}

// 분해 통계를 낸다. 
void cltItemManagerCommon::AddTuneNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemTuneStatistic[ref] +=	itemnum;

}

// 분해로 나온 아이템 통계를 낸다. 
void cltItemManagerCommon::AddFromTuneNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemFromTuneStatistic[ref] +=	itemnum;

}


// 전리품 아이템 통계를 낸다. 
void cltItemManagerCommon::AddWarItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemMonStatistic[ref] +=	itemnum;
}

// 장비품의 통계를 낸다. 
void cltItemManagerCommon::AddEquipItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemEquipStatistic[ref] +=	itemnum;
}

// 퀘스트 공급 물품의 통계를 낸다. 
void cltItemManagerCommon::AddQuestItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemQuestStatistic[ref] +=	itemnum;
}


// 아이템몰에서 공급 물품의 통계를 낸다. 
void cltItemManagerCommon::AddMallItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemMallStatistic[ref] +=	itemnum;
}

// 생산술에 의한 공급 물품의 통계를 낸다. 
void cltItemManagerCommon::AddProductionItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemProductionStatistic[ref] +=	itemnum;
}


// 서버무역에 의한 수입 물품의 통계를 낸다. 
void cltItemManagerCommon::AddWorldTradeIncomeItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemWorldTradeIncomeStatistic[ref] +=	itemnum;
}

// 서버무역에 의한 수출 물품의 통계를 낸다. 
void cltItemManagerCommon::AddWorldTradeExportItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0)return ;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siItemWorldTradeExportStatistic[ref] +=	itemnum;
}

void cltItemManagerCommon::AddForeignMerchantItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0 ) return;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	clItemStatistic.siForeignMerchantStatistics[ref] +=	itemnum; 
}

void cltItemManagerCommon::AddBookBoxItemNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0 ) return;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if( ref <= 0 ) return;

	clItemStatistic.siBookBoxStatistics[ref] += itemnum;
}

void cltItemManagerCommon::AddElixirNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0 ) return;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if( ref <= 0 ) return;

	clItemStatistic.siElixirStatistics[ref] += itemnum;
}

void cltItemManagerCommon::AddMagicalNostrumNum(SI32 itemunique, SI32 itemnum)
{
	if(itemunique <= 0 ) return;

	SI32 ref = FindItemRefFromUnique(itemunique);
	if( ref <= 0 ) return;

	clItemStatistic.siMagicalNostrumStatistics[ref] += itemnum;
}