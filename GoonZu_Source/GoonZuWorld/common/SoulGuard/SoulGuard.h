#pragma once

#include "../common/CommonHeader.h"

const SI32 Const_Max_SoulGuard_ActiveProbably	= 1000;		// 수호정령 능력 발동시 확률 최대 %
const SI32 Const_Max_SoulGuard_FellowShip		= 100;		// 수호정령이 갖을 수 있는 최대 친화력

const SI32 Const_Unlimit_SoulGuard_Life			= 30000;

const SI32 Const_SoulGuardHouse_ItemUnique		= 24123;	// 수호정령 보관소 아이템 유니크
const SI32 Const_SoulGuardHouse_ItemNum			= 1;		// 수호정령 보관소 아이템 사용시 필요 아이템 갯수


class cltSoulGuardHouseUnit
{
private:
	SI32	m_siSoulGuardID;
	SI16	m_siKindUnique;

public:
	cltSoulGuardHouseUnit( void );
	~cltSoulGuardHouseUnit( void );

	void Init( void );

	void Set( SI32 siSoulGuardID, SI16 siKindUnique )
	{
		m_siSoulGuardID	= siSoulGuardID;
		m_siKindUnique	= siKindUnique;
	}

	void Set( cltSoulGuardHouseUnit* pclUnit )
	{
		if ( pclUnit )
		{
			memcpy( this, pclUnit, sizeof(cltSoulGuardHouseUnit) );
		}
	}

public:
	SI32 GetSoulGuardID( void )					{	return m_siSoulGuardID;				}
	void SetSoulGuardID( SI32 siSoulGuardID )	{	m_siSoulGuardID = siSoulGuardID;	}

	SI16 GetKindUnique( void )					{	return m_siKindUnique;				}
	void SetKindUnique( SI16 siKindUnique )		{	m_siKindUnique = siKindUnique;		}

};

typedef NSafeTArray<cltSoulGuardHouseUnit, MAX_SOULGUARD_HOUSE_NUM>	NSafe_SoulGuardHouseUnit;


class cltSoulGuardHouse
{
private:
#ifdef _SAFE_MEMORY
	NSafe_SoulGuardHouseUnit	m_clSoulGuardHouse;
#else
	cltSoulGuardHouseUnit		m_clSoulGuardHouse[MAX_SOULGUARD_HOUSE_NUM];
#endif

	SI16						m_siMaxHaveHouseNum;	// 현재 뚫린 수호정령 보관소 갯수(MAX_SOULGUARD_HOUSE_NUM을 넘을 수 없다)

public:
	cltSoulGuardHouse();
	~cltSoulGuardHouse();

	void Init( void );

public:
	void Set( SI16 siMaxHaveHouseNum, SI32* psiSoulGuardID, SI16* psiSoulGuardKind );
	void SetHouseUnit( SI32 siIndex, SI32 siSoulGuardID, SI16 siKindUnique );

	SI32 GetSoulGuardIDFromIndex( SI32 siIndex );
	SI16 GetKindUniqueFromIndex( SI32 siIndex );

public:
	SI16 GetMaxHaveHouseNum( void )						{	return m_siMaxHaveHouseNum;					}
	void SetMaxHaveHouseNum( SI16 siMaxHaveHouseNum )	{	m_siMaxHaveHouseNum = siMaxHaveHouseNum;	}

};


enum SOULGUARD_STATUS
{
	SOULGUARD_STATUS_IN = 0,	// 안에 있다(회수됨)
	SOULGUARD_STATUS_OUT,		// 밖에 나와있다(소환됨)
	SOULGUARD_STATUS_DEAD,		// 수호정령 죽음
};

class cltSoulGuard
{
private:
	SI32	m_siSoulGuadrID;				// 고유한 ID(생성될때 발급됨)
	TCHAR	m_szName[MAX_SOULGUARD_NAME];	// 이름

	SI16	m_siStatus;						// 상태(In,Out)
	SI16	m_siKind;						// Kind Unique

	SI32	m_siLimitDateVary;				// 이때까지 유효하다

	SI16	m_siFellowShip;					// 친화력

	// 아래정보는 DB에는 저장되지 않음
	SI32	m_siHaveAbility;				// 갖고 있는 능력 BIT마스크연산으로 처리(나중에 일일히 능력이 있는지 없는지를 리소스에서 체크하기 보다는 미리 설정해 놨다가 이용할수있도록 한다)

public:
	cltSoulGuard( void );
	~cltSoulGuard( void );

	void	Init( void );				// cltSoulGuard 클레스 전체를 초기화 시킨다
	void	InitExceptHouse( void );	// 맴버 변수인 보관소는 제외하고 초기화 시킨다

	void	SetSoulGuard( SI32 siSoulGuardID, TCHAR* pszName, SI16 siStatus, SI16 siKind, SI32 siLimitDateVary, SI16 siFellowShip );

public:
	SI32	GetSoulGuardID( void )						{	return m_siSoulGuadrID;					}
	void	SetSoulGuardID( SI32 siSoulGuardID )		{	m_siSoulGuadrID = siSoulGuardID;		}

	TCHAR*	GetName( void )								{	return m_szName;						}
	void	SetName( TCHAR* pszName )					{	if ( pszName ) StringCchCopy( m_szName, sizeof(m_szName), pszName );	}

	SI16	GetStatus( void )							{	return m_siStatus;						}
	void	SetStatus( SI16 siStatus )					{	m_siStatus = siStatus;					}

	SI16	GetKind( void )								{	return m_siKind;						}
	void	SetKind( SI16 siKind )						{	m_siKind = siKind;						}

	SI32	GetLimitDateVary( void )					{	return m_siLimitDateVary;				}
	void	SetLimitDateVary( SI32 siLimitDateVary )	{	m_siLimitDateVary = siLimitDateVary;	}

	SI16	GetFellowShip( void )						{	return m_siFellowShip;					}
	void	SetFellowShip( SI16 siFellowShip )			{	m_siFellowShip = siFellowShip;			}

	void	SetHaveAbility( SI32 siAbility )			{	m_siHaveAbility = siAbility;			}
	void	AddHaveAbility( SI32 siAbility )			{	m_siHaveAbility |= siAbility;			}
	BOOL	IsHaveAbility( SI32 siAbility )				{	return (m_siHaveAbility & siAbility);	}
	void	ClearHaveAbility( void )					{	m_siHaveAbility = 0;					}

public:
	BOOL	IsEmpty( void )
	{
		if ( 0 < m_siSoulGuadrID )		return FALSE;
		if ( 0 < m_siKind )				return FALSE;
		if ( 0 < m_siLimitDateVary )	return FALSE;
		
		return TRUE;
	}

public:
	cltSoulGuardHouse	m_clHouse;	// 수호정령 보관소
};