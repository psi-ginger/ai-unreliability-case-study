#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "SummonMarket.h"
#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/msg/MsgType-SummonMarket.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-SummonMarket.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;


//----------------------------------------------
// cltSummonMarket
//----------------------------------------------
cltSummonMarket::cltSummonMarket(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{
	
}


cltSummonMarket::~cltSummonMarket()
{

}




cltSummonMarketManager::cltSummonMarketManager(cltCharManager* pclcm, SI32 gamemode, SI32 portraitid, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, portraitid, strFileName )
{

	m_siSelectList = 0;

	siAddStartIndex = 0;
	siTotalOrderNumber = 0;
	siTotalResponseOrderNumber = 0;

	m_hSummonPortraitBitmap = NULL;
	m_siPortraitSummonKind = 0;

	if(gamemode == GAMEMODE_SERVER)
	{
		pclSummonOrder = new cltSummonOrder[ MAX_SUMMONMARKET_ORDER_NUMBER ];
	}
	else
	{
		pclSummonOrder = NULL;
	}


	siSelectedIndex = -1;
}


cltSummonMarketManager::~cltSummonMarketManager()
{
	if( m_hSummonPortraitBitmap ) {
		DeleteObject(m_hSummonPortraitBitmap);	
		m_hSummonPortraitBitmap = NULL;	
	}
	


	if( pclSummonOrder ) delete [] pclSummonOrder;
}


// 최초 환영 인사말표시..
void cltSummonMarketManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1801);
	SetMainExplain(hDlg, pText);
}

void cltSummonMarketManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}


// 소환수거래소를 신설한다.
BOOL cltSummonMarketManager::CreateSummonMarket(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltSummonMarket(siunique);
		return TRUE;
	}

	return FALSE;
}

void cltSummonMarketManager::AddOrder( cltSummonOrder *pclOrder )
{
	for( int i = siAddStartIndex; i < MAX_SUMMONMARKET_ORDER_NUMBER; ++i ) {

		if( pclSummonOrder[ i ].siPersonID == 0 ) {

			pclSummonOrder[ i ].Set( pclOrder );
			
			siAddStartIndex = i + 1;
			
			++siTotalOrderNumber;
			
			break;
		}
	}
}

void cltSummonMarketManager::DelOrder( SI32 orderIndex )
{
	int count = 0;

	for( int i = 0; i < MAX_SUMMONMARKET_ORDER_NUMBER; ++i ) {

		if( pclSummonOrder[ i ].siPersonID != 0 ) {
			++count;

			if( pclSummonOrder[ i ].siIndex == orderIndex ) {
				
				pclSummonOrder[ i ].Init();

				if( i < siAddStartIndex ) siAddStartIndex = i;
				--siTotalOrderNumber;

				break;
			}

		}

		if( count >= siTotalOrderNumber ) break;
	}

}

void cltSummonMarketManager::UpdatePrice( int index, cltSummonOrder *pclOrder )
{
	
	if( pclSummonOrder[ index ].siIndex != pclOrder->siIndex ) return;

	if( pclSummonOrder[ index ].clSummon.siSummonID != pclOrder->clSummon.siSummonID ) return;

	pclSummonOrder[ index ].siPrice = pclOrder->siPrice;

	pclSummonOrder[ index ].clDate.Set( &pclOrder->clDate );

}


void cltSummonMarketManager::Action( bool bdaychangedswitch )
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
		// 가격을 더이상 낮출 수 없는 경우에는 주문 자체를 삭제한다.(물품도)
		cltServer* pclserver = (cltServer* )pclClient;
		if(pclserver->pclTime != NULL)
		{
			cltSummonOrder clorder;
			SI32 curdate = pclserver->pclTime->clDate.GetDateVary();

			int count = 0;
			for( int i = 0; i < MAX_SUMMONMARKET_ORDER_NUMBER; ++i ) {

				clorder.Set( &pclSummonOrder[ i ] );
				
				if( clorder.siPersonID != 0 ) {
					++count;

					if( curdate - clorder.clDate.GetDateVary() >= 30 ) {
						
						clorder.clDate.Set( &pclserver->pclTime->clDate );
						clorder.siPrice = clorder.siPrice * 9 / 10;

						if( clorder.siPrice < 3000 ) {
							clorder.siPrice = 0;
						}
						
						sDBRequest_ReduceSummonOrderPrice clMsg( i, &clorder);
						pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}

				if( count >= siTotalOrderNumber ) break;
			}
		}
	}

}


void cltSummonMarketManager::AddMyOrder( cltSummonOrder *pclOrder )
{
	for( int i = 0; i < MAX_SUMMONMARKET_ORDER_PER_PERSON; i++ )
	{
		if( clMyOrder[ i ].siPersonID == 0 ) {

			clMyOrder[ i ].Set( pclOrder );
			break;
		}
	}
}

void cltSummonMarketManager::DelMyOrder( SI32 orderIndex )
{
	for( int i = 0; i < MAX_SUMMONMARKET_ORDER_PER_PERSON; i++ )
	{
		if( clMyOrder[ i ].siIndex == orderIndex ) {

			clMyOrder[ i ].Init();
			break;
		}
	}
}


