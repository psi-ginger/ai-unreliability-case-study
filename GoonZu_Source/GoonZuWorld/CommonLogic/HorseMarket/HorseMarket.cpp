#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "HorseMarket.h"
#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/msg/MsgType-HorseMarket.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-HorseMarket.h"
#include "..\..\Client\Music\Music.h"

#include "Char\CharManager\CharManager.h"


//-------------------------------------
// 이조
//-------------------------------------
#include "..\Server\Minister\MinisterBase.h"
#include "..\Server\Minister\Minister-Izo.h"
#include "..\Server\Minister\MinisterMgr.h"

extern cltCommonLogic* pclClient;


//----------------------------------------------
// cltHorseMarket
//----------------------------------------------
cltHorseMarket::cltHorseMarket(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{

}


cltHorseMarket::~cltHorseMarket()
{

}




cltHorseMarketManager::cltHorseMarketManager(cltCharManager* pclcm, SI32 gamemode, SI32 portraitid, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, portraitid, strFileName )
{

	siAddStartIndex = 0;
	siTotalOrderNumber = 0;
	siTotalResponseOrderNumber = 0;

	if(gamemode == GAMEMODE_SERVER)
	{
		pclHorseOrder = new cltHorseOrder[ MAX_HORSEMARKET_ORDER_NUMBER ];
	}
	else
	{
		pclHorseOrder = NULL;
	}



	siSelectedIndex = -1;


}


cltHorseMarketManager::~cltHorseMarketManager()
{


	if( pclHorseOrder ) delete [] pclHorseOrder;
}


// 최초 환영 인사말표시..
void cltHorseMarketManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1766);
	SetMainExplain(hDlg, pText);
}

void cltHorseMarketManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}


// 사복시를 신설한다.
BOOL cltHorseMarketManager::CreateHorseMarket(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltHorseMarket(siunique);
		return TRUE;
	}

	return FALSE;
}

void cltHorseMarketManager::AddOrder( cltHorseOrder *pclOrder )
{
	for( int i = siAddStartIndex; i < MAX_HORSEMARKET_ORDER_NUMBER; ++i ) {

		if( pclHorseOrder[ i ].siPersonID == 0 ) {

			pclHorseOrder[ i ].Set( pclOrder );
			
			siAddStartIndex = i + 1;
			
			++siTotalOrderNumber;
			
			break;
		}
	}
}

void cltHorseMarketManager::DelOrder( SI32 orderIndex )
{
	int count = 0;

	for( int i = 0; i < MAX_HORSEMARKET_ORDER_NUMBER; ++i ) {

		if( pclHorseOrder[ i ].siPersonID != 0 ) {
			++count;

			if( pclHorseOrder[ i ].siIndex == orderIndex ) {
				
				pclHorseOrder[ i ].Init();

				if( i < siAddStartIndex ) siAddStartIndex = i;
				--siTotalOrderNumber;

				break;
			}

		}

		if( count >= siTotalOrderNumber ) break;
	}

}

void cltHorseMarketManager::UpdatePrice( int index, cltHorseOrder *pclOrder )
{
	if( pclHorseOrder[ index ].siIndex != pclOrder->siIndex ) return;

	if( pclHorseOrder[ index ].clHorse.IsSame( &pclOrder->clHorse ) == FALSE ) return;

	pclHorseOrder[ index ].siPrice = pclOrder->siPrice;

	pclHorseOrder[ index ].clDate.Set( &pclOrder->clDate );

}


void cltHorseMarketManager::Action( bool bdaychangedswitch )
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
			cltHorseOrder clorder;
			SI32 curdate = pclserver->pclTime->clDate.GetDateVary();

			int count = 0;
			for( int i = 0; i < MAX_HORSEMARKET_ORDER_NUMBER; ++i ) {

				clorder.Set( &pclHorseOrder[ i ] );
				
				if( clorder.siPersonID != 0 ) {
					++count;

					if( curdate - clorder.clDate.GetDateVary() >= 30 ) {
						
						clorder.clDate.Set( &pclserver->pclTime->clDate );
						//clorder.siPrice = clorder.siPrice * 9 / 10;	
						// 이조판서(주작관) 의 설정에 영향을 받도록 한다.


						CMinisterIzo *pclminister = (CMinisterIzo *)((cltServer*)pclClient)->pclMinisterMgr->GetMinister(MINISTER_IZO);
						SI32 downrate = pclminister->GetMarketDownRate();

						GMONEY cutprice = clorder.siPrice * downrate / 100;

						clorder.siPrice -= max( 10 , cutprice );


						if( clorder.siPrice < 5000 ) {
							clorder.siPrice = 0;
						}
						
						sDBRequest_ReduceHorseOrderPrice clMsg( i, &clorder);
						pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}

				if( count >= siTotalOrderNumber ) break;
			}
		}
	}

	// 말 자동 구매 루틴 - 임시로 막음 // 2006/5/22

	//if( pclClient->GetFrame() % pclClient->siHorseMarketAutoBuyDelayFrame == 0 ) {

	//	if ( ((cltServer*)pclClient)->siHorseCurrentPrice < 1 )
	//	{
	//		return;
	//	}

	//	SI32 HorsePrice = 0;

	//	int price = 0;
	//	int orderindex = -1;

	//	int count = 0;
	//	for( int i = 0; i < MAX_HORSEMARKET_ORDER_NUMBER; ++i ) {

	//		if( pclHorseOrder[ i ].siPersonID != 0 ) {
	//			++count;


	//			switch ( pclHorseOrder[ i ].clHorse.siHorseUnique )
	//			{
	//			case 1:
	//				HorsePrice = ((cltServer*)pclClient)->siHorseCurrentPrice;
	//				break;
	//			case 2:
	//				HorsePrice = ((cltServer*)pclClient)->siHorseCurrentPrice * 1.3;
	//				break;
	//			case 3:
	//				HorsePrice = ((cltServer*)pclClient)->siHorseCurrentPrice * 1.6;
	//				break;
	//			case 4:
	//			case 5:
	//				HorsePrice = ((cltServer*)pclClient)->siHorseCurrentPrice * 1.9;
	//				break;
	//			}

	//			HorsePrice = HorsePrice * 0.8;

	//			if( HorsePrice == 0 ) return;


	//			if( ( price == 0 || pclHorseOrder[ i ].siPrice < price ) &&
	//				( pclHorseOrder[ i ].siPrice < HorsePrice ) &&
	//				( pclHorseOrder[ i ].clHorse.GetAge( ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() ) ) &&
	//				( pclHorseOrder[ i ].clHorse.siMoveSpeed >= 1500 ) ) {

	//				price = pclHorseOrder[ i ].siPrice;
	//				orderindex = pclHorseOrder[ i ].siIndex;
	//			}
	//		}

	//		if( count >= siTotalOrderNumber ) break;
	//	}

	//	if( orderindex != -1 ) {
	//		sDBRequest_HorseMarketBuyHorse sendMsg( 0, 0, 0,siVillageUnique, orderindex, price );
	//		((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
	//	}
	//}
}




