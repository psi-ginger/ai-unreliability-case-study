#include "commonlogic.h"
#include "..\..\Common\Char\KindInfo\KindInfo.h"

#include "HiredNPCMgr.h"


extern cltCommonLogic* pclClient;


cltHiredNPCPayInfo::cltHiredNPCPayInfo()
{
	Init();
}

cltHiredNPCPayInfo::~cltHiredNPCPayInfo()
{

}

void cltHiredNPCPayInfo::Init()
{
	siNPCKind = 0;
	siTotalHiredNum = 0;
	siPayment = 0;
}

cltHiredNPCPayManager::cltHiredNPCPayManager()
{
	Init();
}

cltHiredNPCPayManager::~cltHiredNPCPayManager()
{

}

void cltHiredNPCPayManager::Init()
{
	siTotalNPCNum = 0;
	siNPCKindNum = 0;

	for( int i = 0; i < MAX_HIREDNPC_KIND_NUM; ++i )
	{
		clHiredNPCPayInfo[ i ].siNPCKind = 0;
	}
}

void cltHiredNPCPayManager::AddNPC( int npckind )
{
	int i;

	// 등록된 NPC Kind가 있는지 검사 
	for( i = 0; i < MAX_HIREDNPC_KIND_NUM; ++i )
	{
		if( clHiredNPCPayInfo[ i ].siNPCKind == npckind ) {
			// 등록 되어 있으면 

			++clHiredNPCPayInfo[ i ].siTotalHiredNum;
			++siTotalNPCNum;

			CalcPayment( i );
			return;
		}
	}

	for( i = 0; i < MAX_HIREDNPC_KIND_NUM; ++i ) 
	{
		if( clHiredNPCPayInfo[ i ].siNPCKind == 0 ) {

			clHiredNPCPayInfo[ i ].siNPCKind = npckind;
			clHiredNPCPayInfo[ i ].siTotalHiredNum = 1;
			++siTotalNPCNum;

			CalcPayment( i );
			return;
		}
	}
}

void cltHiredNPCPayManager::DelNPC( int npckind )
{
	int i;
	for( i = 0; i < MAX_HIREDNPC_KIND_NUM; ++i )
	{
		if( clHiredNPCPayInfo[ i ].siNPCKind == npckind ) {
			// 등록 되어 있으면 숫자를 1뺀다 

			--clHiredNPCPayInfo[ i ].siTotalHiredNum;
			--siTotalNPCNum;

			if( clHiredNPCPayInfo[ i ].siTotalHiredNum == 0 ) {
				// 숫자가 0이면 Kind를 없앤다

				clHiredNPCPayInfo[ i ].siNPCKind = 0;
			}

			CalcPayment( i );
		}
	}
}

void cltHiredNPCPayManager::CalcPayment( int i )
{


	GMONEY pay =  (GMONEY)clHiredNPCPayInfo[ i ].siTotalHiredNum - 2  * 12000 + 20000;

	if( pay < 20000 ) pay = 20000;
	if( pay > 200000 ) pay = 200000;
	
	pay += pclClient->pclKindInfoSet->GetHireBasicMoney( clHiredNPCPayInfo[ i ].siNPCKind );

	clHiredNPCPayInfo[ i ].siPayment = pay;
	
/*
	SI32 pay = pclClient->pclKindInfoSet->GetHireBasicMoney( clHiredNPCPayInfo[ i ].siNPCKind )
		* ( pclClient->pclItemManager->clItemStatistic.siNPCPriceRate / 100. );
*/
}

GMONEY cltHiredNPCPayManager::GetPayment( int npckind )
{
	int i;
	for( i = 0; i < MAX_HIREDNPC_KIND_NUM; ++i )
	{
		if( clHiredNPCPayInfo[ i ].siNPCKind == npckind ) {
			return clHiredNPCPayInfo[ i ].siPayment;
		}
	}
	return 0;
}

