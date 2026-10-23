#include "ForeignMerchantNPC.h"

#include "..\..\Char\kindinfo\kindinfo.h"

#include "..\..\..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-ForeignMerchantNPC.h"

#include "..\..\..\CommonLogic\CommonLogic.h"
extern cltCommonLogic *pclClient;

CForeignMerchantNPC::CForeignMerchantNPC()
{	
	Initialize();
}

CForeignMerchantNPC::~CForeignMerchantNPC()
{
	Destroy();
}

void CForeignMerchantNPC::Initialize()
{
	m_bAvailable = false;

	m_siForeignMerchantKind = -1;

	m_siStatus = FOREIGNMERCHANT_STOP_STAUS;

	m_siBuyItemTotalNum = 0;

	m_siSaveLastBuyRestNum = 0;

	memset( &sForeignMerchantNPCInfo, 0, sizeof( stForeignMerchantNPCInfo ) );

	m_siBuyItemType = -1;

	return;
}

void CForeignMerchantNPC::Destroy()
{
	/*
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if ( m_siStatus == FOREIGNMERCHANT_BUY_STATUS )
		{
			cltServer *pclserver = (cltServer *)pclClient;

			// DB 로 교역 NPC 셑팅을 보낸다.
			sDBRequest_SetForeignMerchantNPCInfo clMsg(m_siForeignMerchantKind, sForeignMerchantNPCInfo.siBuyItemUnique, sForeignMerchantNPCInfo.siBuyItemAveragePrice, sForeignMerchantNPCInfo.siBuyItemRestNum, &sForeignMerchantNPCInfo.clDate );
			
			pclClient->SendDBMsg((sPacketHeader *)&clMsg);	
		}
	}
	*/
}

SI16 CForeignMerchantNPC::GetStatus()
{
	return m_siStatus;
}

SI16 CForeignMerchantNPC::GetForeignMerchantKind()
{
	return m_siForeignMerchantKind;
}


void CForeignMerchantNPC::SetKind( SI16 siForeignMerchantKind, SI16 siBuyItemType )
{
	m_siForeignMerchantKind = siForeignMerchantKind;
	m_siBuyItemType = siBuyItemType;
}

void CForeignMerchantNPC::Set( SI32 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount )
{
	for ( SI32 i = 0; i < siListCount; ++ i )
	{
		sForeignMerchantNPCInfo.siBuyItemUniqueList[ i ] = psiItemUniqueList[ i ];
		sForeignMerchantNPCInfo.siBuyItemPriceList[ i ] = psiItemPriceList[ i ];
	}

	sForeignMerchantNPCInfo.siForeignMerchantKind = m_siForeignMerchantKind;
	sForeignMerchantNPCInfo.siBuyItemType = m_siBuyItemType;
	sForeignMerchantNPCInfo.siBuyItemListCount = siListCount;
	m_siStatus = FOREIGNMERCHANT_BUY_STATUS;

	m_bAvailable = true;		// 교역 NPC 가 구매할 준비가 완료 되었다.

	return;
}

void CForeignMerchantNPC::Set( SI16 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount )
{
	for ( SI32 i = 0; i < siListCount; ++ i )
	{
		sForeignMerchantNPCInfo.siBuyItemUniqueList[ i ] = psiItemUniqueList[ i ];
		sForeignMerchantNPCInfo.siBuyItemPriceList[ i ] = psiItemPriceList[ i ];
	}

	sForeignMerchantNPCInfo.siForeignMerchantKind = m_siForeignMerchantKind;
	sForeignMerchantNPCInfo.siBuyItemType = m_siBuyItemType;
	sForeignMerchantNPCInfo.siBuyItemListCount = siListCount;
	m_siStatus = FOREIGNMERCHANT_BUY_STATUS;

	m_bAvailable = true;		// 교역 NPC 가 구매할 준비가 완료 되었다.

	return;
}

/*
// 유니크 값에 따른 시세를 DB 로 부터 받아와 입력함
// 날짜는 파라미터 값이??
// 주석 풀때 가격 SI64로 변경해주세요!
void CForeignMerchantNPC::Set( SI16 siItemUnique, SI32 siItemAveragePrice, SI32 siBuyItemRestNum, cltDate clDate )
{
	if ( siItemUnique < 0 || siItemAveragePrice < 1 )
	{
		return;
	}

	m_siStatus = FOREIGNMERCHANT_BUY_STATUS;

	sForeignMerchantNPCInfo.siForeignMerchantKind = m_siForeignMerchantKind;

	sForeignMerchantNPCInfo.siBuyItemUnique = siItemUnique;
	sForeignMerchantNPCInfo.siBuyItemAveragePrice = siItemAveragePrice;	
	
	sForeignMerchantNPCInfo.siBuyItemRestNum = siBuyItemRestNum;



	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		m_siBuyItemTotalNum = MAX_FOREIGNMERCHANTNPC_BUY_ITEM_TOTAL_PRICE / siItemAveragePrice;

		cltServer *pclserver = (cltServer *)pclClient;	

		cltDate cldate;
		cldate.Set(&pclserver->pclTime->clDate);
		cldate.AddMonth(1);								// 구입 최대 시간은 1달임
		
		sForeignMerchantNPCInfo.clDate.Set( &cldate );


		// DB 로 교역 NPC 셑팅을 보낸다.
		sDBRequest_SetForeignMerchantNPCInfo clMsg(m_siForeignMerchantKind, siItemUnique, siItemAveragePrice, siBuyItemRestNum, &sForeignMerchantNPCInfo.clDate );
		
		pclClient->SendDBMsg((sPacketHeader *)&clMsg);	
	}
	else
	{
		sForeignMerchantNPCInfo.clDate.Set( &clDate );
	}



	m_bAvailable = true;		// 교역 NPC 가 구매할 준비가 완료 되었다.

	return;
}
*/

// 현재 정보를 얻어옮
stForeignMerchantNPCInfo* CForeignMerchantNPC::GetForeignMerchantNPCInfo()
{
	return &sForeignMerchantNPCInfo;
}

// 현재 정보를 얻어와서 복사시켜줌.
void CForeignMerchantNPC::GetForeignMerchantNPCInfo( stForeignMerchantNPCInfo * pstForeignMerchantNPCInfo )
{
	memcpy( pstForeignMerchantNPCInfo, &sForeignMerchantNPCInfo, sizeof( stForeignMerchantNPCInfo ) );
}

/*
// return false 가 가면 새로 아이템을 넣어야 됨.
bool CForeignMerchantNPC::Buy( UI16 uiItemNum )
{
	if ( m_siStatus == FOREIGNMERCHANT_STOP_STAUS && uiItemNum < 1 )
	{
		return false;
	}

	sForeignMerchantNPCInfo.siBuyItemRestNum -= uiItemNum;

	if ( sForeignMerchantNPCInfo.siBuyItemRestNum < 1 )
	{
		m_siStatus = FOREIGNMERCHANT_STOP_STAUS;

		m_siBuyItemTotalNum = 0;

		sForeignMerchantNPCInfo.siBuyItemUnique = -1;
		sForeignMerchantNPCInfo.siBuyItemAveragePrice = -1;
		sForeignMerchantNPCInfo.siBuyItemRestNum = 0;

		return false;
	}

	return true;
}
*/

void CForeignMerchantNPC::GetBuyInfoText( TCHAR *pBuffer )
{
	if ( pBuffer == NULL )
	{
		return;
	}
}

bool CForeignMerchantNPC::IsAvailable()
{
	return m_bAvailable;
}

void CForeignMerchantNPC::RefreshPrice()
{
	if ( m_siForeignMerchantKind < 1 )
	{
		return;
	}

	SI32 siUniqueList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];
	GMONEY siPriceList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];
	SI32 siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( m_siBuyItemType, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( siUniqueList, siPriceList, siListCount );
	}

	return;
}

CForeignMerchantNPCMgr::CForeignMerchantNPCMgr()
{
	Initialize();
}

CForeignMerchantNPCMgr::~CForeignMerchantNPCMgr()
{	
	Destroy();
}

void CForeignMerchantNPCMgr::Initialize()
{
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		m_ForeignMerchant[ i ].Initialize();
	}
}

void CForeignMerchantNPCMgr::Destroy()
{
}

void CForeignMerchantNPCMgr::Add( SI16 siKind, SI16 siBuyItemType )
{
	// 기존에 NPC 가 존재한다면, 추가할 필요는 없으므로...
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			return;
		}
	}

	for ( i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == -1 )
		{
			m_ForeignMerchant[ i ].SetKind( siKind, siBuyItemType );
			return;
		}
	}
}


void CForeignMerchantNPCMgr::Set( SI16 siKind, SI32 siBuyItemType, SI32 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount )
{
	// 기존에 NPC 가 존재한다면, 추가할 필요는 없으므로...
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			m_ForeignMerchant[ i ].Set( psiItemUniqueList, psiItemPriceList, siListCount );			
			return;
		}
	}
	
	// 기존에 NPC가 존재하지 않는다면, 추가하고 세팅 해줘야 함으로...
	for ( i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == -1 )
		{	
			m_ForeignMerchant[ i ].SetKind( siKind, siBuyItemType );
			m_ForeignMerchant[ i ].Set( psiItemUniqueList, psiItemPriceList, siListCount );
			return;
		}
	}

//#ifdef _DEBUG
	pclClient->pclLog->FilePrint(TEXT("ForeignMerchantLog.txt"), TEXT("Set( SI16 siKind, SI32 siBuyItemType, SI32 *psiItemUniqueList, SI32 *psiItemPriceList, SI32 siListCount )상인 없는 번호 왔다. 상인번호 = %d"), siKind );		
//#endif
	
	return;
}

void CForeignMerchantNPCMgr::Set( SI16 siKind, SI32 siBuyItemType, SI16 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount )
{
	// 기존에 NPC 가 존재한다면, 추가할 필요는 없으므로...
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			m_ForeignMerchant[ i ].Set( psiItemUniqueList, psiItemPriceList, siListCount );
			return;
		}
	}
	
	// 기존에 NPC가 존재하지 않는다면, 추가하고 세팅 해줘야 함으로...
	for ( i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == -1 )
		{	
			m_ForeignMerchant[ i ].SetKind( siKind, siBuyItemType );
			m_ForeignMerchant[ i ].Set( psiItemUniqueList, psiItemPriceList, siListCount );
			return;
		}
	}

//#ifdef _DEBUG
	if (pclClient->GameMode == GAMEMODE_SERVER)
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Set( SI16 siKind, SI32 siBuyItemType, SI16 *psiItemUniqueList, SI32 *psiItemPriceList, SI32 siListCount )상인 없는 번호 왔다. 상인번호 = %d"), siKind );			
//#endif
	
	return;
}


/*
void CForeignMerchantNPCMgr::Set( SI16 siKind, SI16 siBuyItemUnique, SI32 siBuyItemAveragePrice, SI32 siBuyItemRestNum, cltDate	clDate )
{
	// 기존에 NPC 가 존재한다면, 추가할 필요는 없으므로...
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			m_ForeignMerchant[ i ].Set( siBuyItemUnique, siBuyItemAveragePrice, siBuyItemRestNum, clDate );			
			return;
		}
	}
	
	// 기존에 NPC가 존재하지 않는다면, 추가하고 세팅 해줘야 함으로...
	for ( i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == -1 )
		{	
			m_ForeignMerchant[ i ].SetKind( siKind );
			m_ForeignMerchant[ i ].Set( siBuyItemUnique, siBuyItemAveragePrice, siBuyItemRestNum, clDate );
			return;
		}
	}

#ifdef _DEBUG
	pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("없는 상인 뭘로 찾노?"), TEXT("상인 없는 번호 왔다. 상인번호 = %d"), siKind );
#endif
	
	return;
}
*/

void CForeignMerchantNPCMgr::Action( cltTime *pTime )
{
	if ( pTime == NULL )					// 종료시 pTime == NULL 인 경우가 와서...
	{
		return;
	}

	if(  pTime->clDate.uiYear == 0 && pTime->clDate.uiMonth == 0 && pTime->clDate.uiDay == 0 && pTime->clDate.uiHour == 0 )			// 유요하지 않은 시간
	{
		return;
	}
	
	if(pclClient->GetFrame() % 100 ) return ;

	/*
	// 서버에서 현재 날짜가 유효한 시간(pTime->clDate.uiYear >= 1)인 경우 그리고 DB로 부터 설정도 끝난 경우임

	if ( ++m_siSaveDelayCount > SAVE_TIME_FOREIGNMERCHANTNPC_BUY_INFO_TO_DB )
	{
		for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
		{
			m_ForeignMerchant[ i ].SaveDifferent();
		}

		m_siSaveDelayCount = 0;
	}
	*/
		
	//상인이 없는 상태이거나, 주문이 없는 상태면 주문을 만들고, 주문이 있다면 그냥 리턴
	
	SI32 siUniqueList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];
	GMONEY siPriceList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];
	SI32 siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_HELMET, ITEMTYPE_HELMET );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_HELMET, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_HELMET, ITEMTYPE_HELMET, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_ARMOUR, ITEMTYPE_ARMOUR );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_ARMOUR, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_ARMOUR, ITEMTYPE_ARMOUR, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_AXE, ITEMTYPE_AXE );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_AXE, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_AXE, ITEMTYPE_AXE, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_SWORD, ITEMTYPE_SWORD );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_SWORD, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_SWORD, ITEMTYPE_SWORD, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_STAFF, ITEMTYPE_STAFF );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_STAFF, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_STAFF, ITEMTYPE_STAFF, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_SPEAR, ITEMTYPE_SPEAR );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_SPEAR, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_SPEAR, ITEMTYPE_SPEAR, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_GUN, ITEMTYPE_GUN );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_GUN, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_GUN, ITEMTYPE_GUN, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_BOW, ITEMTYPE_BOW );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_BOW, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_BOW, ITEMTYPE_BOW, siUniqueList, siPriceList, siListCount );
	}
		
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_SHOES, ITEMTYPE_SHOES );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_SHOES, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_SHOES, ITEMTYPE_SHOES, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_BELT, ITEMTYPE_BELT );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_BELT, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_BELT, ITEMTYPE_BELT, siUniqueList, siPriceList, siListCount );
	}
		
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_POTTER, ITEMTYPE_DISH );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_DISH, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_POTTER, ITEMTYPE_DISH, siUniqueList, siPriceList, siListCount );
	}

	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_HARDWARE, ITEMTYPE_IRON );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_IRON, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_HARDWARE, ITEMTYPE_IRON, siUniqueList, siPriceList, siListCount );
	}

	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_RING, ITEMTYPE_RING );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_RING, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_RING, ITEMTYPE_RING, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_CLOTHES, ITEMTYPE_DRESS );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_DRESS, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_CLOTHES, ITEMTYPE_DRESS, siUniqueList, siPriceList, siListCount );
	}

	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_HAT, ITEMTYPE_HAT );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_HAT, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_HAT, ITEMTYPE_HAT, siUniqueList, siPriceList, siListCount );
	}

	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_NECK, ITEMTYPE_NECK );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_NECK, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_NECK, ITEMTYPE_NECK, siUniqueList, siPriceList, siListCount );
	}
	
	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_PAPER, ITEMTYPE_PAPER );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_PAPER, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_PAPER, ITEMTYPE_PAPER, siUniqueList, siPriceList, siListCount );
	}

	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_FISHING, ITEMTYPE_FISHINGROD );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_FISHINGROD, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_FISHING, ITEMTYPE_FISHINGROD, siUniqueList, siPriceList, siListCount );
	}

	siListCount = MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM;
	
	Add ( FOREIGNMERCHANT_CANNON, ITEMTYPE_CANNON );
	
	if ( pclClient->pclItemManager->GetForeignNPCItemInfo( ITEMTYPE_CANNON, siUniqueList, siPriceList, &siListCount ) == true )
	{
		Set( FOREIGNMERCHANT_CANNON, ITEMTYPE_CANNON, siUniqueList, siPriceList, siListCount );
	}

	

	return;
}

SI16 CForeignMerchantNPCMgr::GetStatus( SI16 siKind )
{
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			return m_ForeignMerchant[ i ].GetStatus();
		}
	}

	return FOREIGNMERCHANT_NOT_EXIST;
}

CForeignMerchantNPC *CForeignMerchantNPCMgr::GetForeignMerchantNPC( SI16 siKind )
{
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			return &m_ForeignMerchant[ i ];
		}
	}

//#ifdef _DEBUG
	if (pclClient->GameMode == GAMEMODE_SERVER)
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("GetForeignMerchantNPC상인 없는 번호 왔다. 상인번호 = %d"), siKind );
//#endif
	return NULL;
}

stForeignMerchantNPCInfo* CForeignMerchantNPCMgr::GetForeignMerchantNPCInfo( SI16 siKind )
{
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			return m_ForeignMerchant[ i ].GetForeignMerchantNPCInfo();
		}
	}

//#ifdef _DEBUG
	if (pclClient->GameMode == GAMEMODE_SERVER)
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("GetForeignMerchantNPCInfo상인 없는 번호 왔다. 상인번호 = %d"), siKind );	
//#endif
	return NULL;
}

void CForeignMerchantNPCMgr::GetBuyInfoText( SI16 siKind, TCHAR *pBuffer )
{
	for ( int i = 0; i < MAX_FOREIGNMERCHANTNPC_NUM; ++i )
	{
		if ( m_ForeignMerchant[ i ].GetForeignMerchantKind() == siKind )
		{
			m_ForeignMerchant[ i ].GetBuyInfoText( pBuffer );
			return;
		}
	}

//#ifdef _DEBUG
	if (pclClient->GameMode == GAMEMODE_SERVER)
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("GetBuyInfoText상인 없는 번호 왔다. 상인번호 = %d"), siKind );		
//#endif

	return;
}
