#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "RealEstate.h"
#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/msg/MsgType-RealEstateMarket.h"
#include "../../CommonLogic/House/House.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-RealEstateMarket.h"
#include "..\..\Client\Music\Music.h"


extern cltCommonLogic* pclClient;


//----------------------------------------------
// cltRealEstateMarket
//----------------------------------------------
cltRealEstateMarket::cltRealEstateMarket(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{

}


cltRealEstateMarket::~cltRealEstateMarket()
{

}




cltRealEstateMarketManager::cltRealEstateMarketManager(cltCharManager* pclcm, SI32 gamemode, SI32 portraitid, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, portraitid, strFileName )
{

	siAddStartIndex = 0;
	siTotalOrderNumber = 0;
	siTotalResponseOrderNumber = 0;

	if(gamemode == GAMEMODE_SERVER)
	{
		pclRealEstateOrder = new cltRealEstateOrder[ MAX_REALESTATEMARKET_ORDER_NUMBER ];

		ZeroMemory( pclRealEstateOrder, sizeof( cltRealEstateOrder ) * MAX_REALESTATEMARKET_ORDER_NUMBER );
	}
	else
	{
		pclRealEstateOrder = NULL;
	}

	siSelectedIndex = -1;

}


cltRealEstateMarketManager::~cltRealEstateMarketManager()
{

	if( pclRealEstateOrder ) delete [] pclRealEstateOrder;
}


// 최초 환영 인사말표시..
void cltRealEstateMarketManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1779);
	SetMainExplain(hDlg, pText);
}

void cltRealEstateMarketManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}


// 거간을 신설한다.
BOOL cltRealEstateMarketManager::CreateRealEstateMarket(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltRealEstateMarket(siunique);
		return TRUE;
	}

	return FALSE;
}

void cltRealEstateMarketManager::AddOrder( cltRealEstateOrder *pclorder )
{
	
	for( int i = siAddStartIndex; i < MAX_REALESTATEMARKET_ORDER_NUMBER; ++i ) {

		if( pclRealEstateOrder[ i ].siPersonID == 0 ) {

			pclRealEstateOrder[ i ].Set( pclorder );

			siAddStartIndex = i + 1;
			
			++siTotalOrderNumber;
			
			break;
		}
	}
}

void cltRealEstateMarketManager::DelOrder( SI32 orderIndex )
{
	
	int count = 0;

	for( int i = 0; i < MAX_REALESTATEMARKET_ORDER_NUMBER; ++i ) {

		if( pclRealEstateOrder[ i ].siPersonID != 0 ) {
			++count;

			if( pclRealEstateOrder[ i ].siIndex == orderIndex ) {
				
				pclRealEstateOrder[ i ].Init();

				if( i < siAddStartIndex ) siAddStartIndex = i;
				--siTotalOrderNumber;

				break;
			}

		}

		if( count >= siTotalOrderNumber ) break;
	}
}

bool cltRealEstateMarketManager::IsExistOrder( SI32 personid, SI32 villageunique, SI32 houseindex)
{
	for( int i = 0; i < MAX_REALESTATEMARKET_ORDER_NUMBER; ++i ) 
	{
		if( pclRealEstateOrder[ i ].siPersonID != 0 ) 
		{

			if( pclRealEstateOrder[ i ].siPersonID == personid
				&& pclRealEstateOrder[ i ].siVillageUnique == villageunique
				&& pclRealEstateOrder[ i ].siHouseIndex == houseindex ) 
			{
				return true;
			}

		}
	}

	return false;
}

void cltRealEstateMarketManager::UpdatePrice( int index, cltRealEstateOrder *pclOrder )
{
	
	if( pclRealEstateOrder[ index ].siIndex != pclOrder->siIndex ) return;

	if( pclRealEstateOrder[ index ].siHouseIndex != pclOrder->siHouseIndex ) return;

	pclRealEstateOrder[ index ].siPrice = pclOrder->siPrice;

	pclRealEstateOrder[ index ].clDate.Set( &pclOrder->clDate );
	
}


void cltRealEstateMarketManager::Action( bool bdaychangedswitch )
{

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;

	//static SI32 marketitemindex	= 0;
	//---------------------------------------------------
	// 매일 주문 정보를 정리한다. (가격을 10%씩 낮춘다 ) 
	//---------------------------------------------------
	if(bdaychangedswitch)
	{
		// 전체 팔자 주문을 검색하여 날짜가 일정 시간 이상 경과한 주문은 가격을 10%씩 낮춘다. 
		// 가격을 더이상 낮출 수 없는 경우에는 주문 자체를 삭제한다. (물품도.)
		cltServer* pclserver = (cltServer* )pclClient;
		if(pclserver->pclTime != NULL)
		{
			cltRealEstateOrder clorder;
			SI32 curdate = pclserver->pclTime->clDate.GetDateVary();

			int count = 0;
			for( int i = 0; i < MAX_REALESTATEMARKET_ORDER_NUMBER; ++i ) {

				clorder.Set( &pclRealEstateOrder[ i ] );
				
				if( clorder.siPersonID ) {
					++count;

					// 주문이 유효 하지 않을 경우 삭제한다 
					cltHouse *pclHouse = (cltHouse *)pclserver->pclHouseManager->pclStruct[ clorder.siVillageUnique ];
					
					if( pclHouse->clUnit[ clorder.siHouseIndex ].clPerson.siPersonID != clorder.siPersonID ) {
						sDBRequest_RealEstateMarketDelOrder clMsg( clorder.siIndex, clorder.siPersonID );
						pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
						continue;
					}
						

					// 30일이 지났을 경우 가격을 낮춘다
					if( curdate - clorder.clDate.GetDateVary() >= 30 ) {
												
						clorder.clDate.Set( &pclserver->pclTime->clDate );
						clorder.siPrice = clorder.siPrice * 9 / 10;

						if( clorder.siPrice < 3000 ) {
							clorder.siPrice = 0;
						}
						
						sDBRequest_ReduceRealEstateOrderPrice clMsg( i, &clorder);
						pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}

				if( count >= siTotalOrderNumber ) break;
			}

		}
	}

}

void cltRealEstateMarketManager::SetMyRealEstateList()
{
	SI32 realestatetype;

#ifdef _SAFE_MEMORY
	clMyRealEstate.ZeroMem();
#else
	ZeroMemory( clMyRealEstate, sizeof( cltRealEstateUnit ) * MAX_REAL_ESTATE_NUMBER_PER_PERSON );
#endif

	int n = 0;

	for( int i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; i++ )
	{
		realestatetype	= pclClient->pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siType;

		if( realestatetype == REALESTATE_TYPE_HOUSE )
		{
			clMyRealEstate[ n ].Set( &pclClient->pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i] );
			n++;
		}
	}
}

void cltRealEstateMarketManager::AddMyOrder( cltRealEstateOrder *pclOrder )
{
	for( int i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; i++ )
	{
		if( clMyOrder[ i ].siPersonID == 0 ) {

			clMyOrder[ i ].Set( pclOrder );
			break;
		}
	}
}

void cltRealEstateMarketManager::DelMyOrder( SI32 orderIndex )
{
	for( int i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; i++ )
	{
		if( clMyOrder[ i ].siIndex == orderIndex ) {

			clMyOrder[ i ].Init();
			break;
		}
	}
}
