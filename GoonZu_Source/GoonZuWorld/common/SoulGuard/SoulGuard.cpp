
#include "SoulGuard.h"

cltSoulGuardHouseUnit::cltSoulGuardHouseUnit( void )	
{	
	Init();										
}
cltSoulGuardHouseUnit::~cltSoulGuardHouseUnit( void )	
{
}

void cltSoulGuardHouseUnit::Init( void )				
{	
	ZeroMemory( this, sizeof(cltSoulGuardHouseUnit) );	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cltSoulGuardHouse::cltSoulGuardHouse()
{
	Init();
}

cltSoulGuardHouse::~cltSoulGuardHouse()
{
}

void cltSoulGuardHouse::Init()
{
#ifdef _SAFE_MEMORY
	m_clSoulGuardHouse.ZeroMem();
#else
	ZeroMemory( m_clSoulGuardHouse, sizeof(cltSoulGuardHouseUnit) * MAX_SOULGUARD_HOUSE_NUM );
#endif

	m_siMaxHaveHouseNum = 0;
}

void cltSoulGuardHouse::Set( SI16 siMaxHaveHouseNum, SI32* psiSoulGuardID, SI16* psiSoulGuardKind )
{
	if ( (0 > siMaxHaveHouseNum) || (MAX_SOULGUARD_HOUSE_NUM < siMaxHaveHouseNum) )
	{
		return;
	}

	if ( (NULL == psiSoulGuardID) || (NULL == psiSoulGuardKind) )
	{
		return;
	}

	m_siMaxHaveHouseNum = siMaxHaveHouseNum;

	for ( SI32 siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		m_clSoulGuardHouse[siIndex].Set( psiSoulGuardID[siIndex], psiSoulGuardKind[siIndex] );
	}
}

void cltSoulGuardHouse::SetHouseUnit( SI32 siIndex, SI32 siSoulGuardID, SI16 siKindUnique )
{
	if ( (0 > siIndex) || (MAX_SOULGUARD_HOUSE_NUM <= siIndex) )
	{
		return;
	}

	m_clSoulGuardHouse[siIndex].Set( siSoulGuardID, siKindUnique );
}

SI32 cltSoulGuardHouse::GetSoulGuardIDFromIndex( SI32 siIndex )
{
	if ( (0 > siIndex) || (MAX_SOULGUARD_HOUSE_NUM <= siIndex) )
	{
		return 0;
	}

	return m_clSoulGuardHouse[siIndex].GetSoulGuardID();
}

SI16 cltSoulGuardHouse::GetKindUniqueFromIndex( SI32 siIndex )
{
	if ( (0 > siIndex) || (MAX_SOULGUARD_HOUSE_NUM <= siIndex) )
	{
		return 0;
	}

	return m_clSoulGuardHouse[siIndex].GetKindUnique();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cltSoulGuard::cltSoulGuard()
{
	Init();
}

cltSoulGuard::~cltSoulGuard()
{
}

void cltSoulGuard::Init( void )
{
	m_siSoulGuadrID		= 0;
	ZeroMemory( m_szName, sizeof(m_szName) );

	m_siStatus			= 0;
	m_siKind			= 0;

	m_siLimitDateVary	= 0;

	m_siFellowShip		= 0;

	m_siHaveAbility		= 0;

	m_clHouse.Init();
}

void cltSoulGuard::InitExceptHouse( void )
{
	m_siSoulGuadrID		= 0;
	ZeroMemory( m_szName, sizeof(m_szName) );

	m_siStatus			= 0;
	m_siKind			= 0;

	m_siLimitDateVary	= 0;

	m_siFellowShip		= 0;

	m_siHaveAbility		= 0;
}

void cltSoulGuard::SetSoulGuard( SI32 siSoulGuardID, TCHAR* pszName, SI16 siStatus, SI16 siKind, SI32 siLimitDateVary, SI16 siFellowShip )
{
	m_siSoulGuadrID		= siSoulGuardID;
	if ( pszName )
	{
		StringCchCopy( m_szName, sizeof(m_szName), pszName );
	}

	m_siStatus			= siStatus;
	m_siKind			= siKind;

	m_siLimitDateVary	= siLimitDateVary;

	m_siFellowShip		= siFellowShip;
}