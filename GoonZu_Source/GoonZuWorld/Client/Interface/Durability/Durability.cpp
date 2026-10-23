// Durability.cpp: implementation of the CDurability class.
//
//////////////////////////////////////////////////////////////////////

#include "Durability.h"

#include "../../Client/client.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDurability::CDurability(SI32 x , SI32 y)
{
	m_bDraw = FALSE ;

	m_siDelay = 0 ;
	m_siDrawX = x ;
	m_siDrawY = y ;

	m_dwType = 0x00000000 ;

	GP.LoadSprFromFile(TEXT("./Interface/Durability/Char.spr"), &m_DurabilityChar );

	GP.LoadSprFromFile(TEXT("./Interface/Durability/Weapon.spr"), &m_DurabilityWeapon );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Armour.spr"), &m_DurabilityArmour );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Helmet.spr"), &m_DurabilityHelmet );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Belt.spr"), &m_DurabilityBelt );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Shoes.spr"), &m_DurabilityShoes );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Ring.spr"), &m_DurabilityRing );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Neck.spr"), &m_DurabilityNeck );
	GP.LoadSprFromFile(TEXT("./Interface/Durability/Mantle.spr"), &m_DurabilityMantle );
}

CDurability::~CDurability()
{
	if ( m_DurabilityChar.pImage )
	{
		GP.FreeSpr( m_DurabilityChar );
	}

	if ( m_DurabilityWeapon.pImage )
	{
		GP.FreeSpr( m_DurabilityWeapon );
	}
	if ( m_DurabilityArmour.pImage )
	{
		GP.FreeSpr( m_DurabilityArmour );
	}
	if ( m_DurabilityHelmet.pImage )
	{
		GP.FreeSpr( m_DurabilityHelmet );
	}
	if ( m_DurabilityBelt.pImage )
	{
		GP.FreeSpr( m_DurabilityBelt );
	}
	if ( m_DurabilityShoes.pImage )
	{
		GP.FreeSpr( m_DurabilityShoes );
	}
	if ( m_DurabilityRing.pImage )
	{
		GP.FreeSpr( m_DurabilityRing );
	}
	if ( m_DurabilityNeck.pImage )
	{
		GP.FreeSpr( m_DurabilityNeck );
	}
	if ( m_DurabilityMantle.pImage )
	{
		GP.FreeSpr(m_DurabilityMantle);
	}
}

void CDurability::Set(DWORD dwType,BOOL bRepair)
{
	m_dwType = dwType ;

	if ( !bRepair )
	{
		m_siDelay = 0 ;
		m_bDraw = FALSE ;
		return ;
	}
}

void CDurability::Action()
{
	if ( m_dwType == 0 )
	{
		return;
	}

	m_siDelay++ ;

	if ( m_siDelay <= DURABILITY_DELAY )
		m_bDraw = TRUE ;
	else if ( m_siDelay > DURABILITY_DELAY )
		m_bDraw = FALSE ;

	if ( m_siDelay > DURABILITY_DELAY * 2 )
	{
		m_siDelay = 0 ;
		m_bDraw = TRUE ;
	}
}

void CDurability::Draw(LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y )
{

	
	
	
	
	////////////////////////////////////////////////////
	return;			// 내구도 안씀다
	////////////////////////////////////////////////////






	//cltClient *pclclient = (cltClient *)pclClient;

	//SI32 siScreenDrawX = m_siDrawX + x ;
	//SI32 siScreenDrawY = pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet - m_siDrawY - m_DurabilityChar.GetYSize() + y - m_DurabilityChar.GetYSize() ;

	//if ( m_dwType == 0 )
	//{
	//	return;
	//}

	//if( GP.LockSurface(lpsurface) == TRUE )
	//{			
	//	if ( m_bDraw )
	//	{
	//		if ( m_dwType & BAG_DURABILITY )
	//			GP.PutSpr(&m_DurabilityMantle, siScreenDrawX, siScreenDrawY, 0 );
	//	}

	//	GP.PutSpr(&m_DurabilityChar, siScreenDrawX, siScreenDrawY, 0 );

	//	if ( m_bDraw )
	//	{
	//		if ( m_dwType & MANTLE_DURABILITY )
	//			GP.PutSpr(&m_DurabilityMantle, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & WEAPON_DURABILITY )
	//			GP.PutSpr(&m_DurabilityWeapon, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & ARMOUR_DURABILITY )
	//			GP.PutSpr(&m_DurabilityArmour, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & HELMET_DURABILITY )
	//			GP.PutSpr(&m_DurabilityHelmet, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & BELT_DURABILITY )
	//			GP.PutSpr(&m_DurabilityBelt, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & SHOES_DURABILITY )
	//			GP.PutSpr(&m_DurabilityShoes, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & RING_DURABILITY )
	//			GP.PutSpr(&m_DurabilityRing, siScreenDrawX, siScreenDrawY, 0 );
	//		if ( m_dwType & NECK_DURABILITY )
	//			GP.PutSpr(&m_DurabilityNeck, siScreenDrawX, siScreenDrawY, 0 );
	//	}

	//	GP.UnlockSurface(lpsurface);
	//}
}

BOOL CDurability::IsTypeExist(DWORD dwType)
{
	if ( m_dwType == dwType ) return TRUE ;
	else return FALSE ;
}