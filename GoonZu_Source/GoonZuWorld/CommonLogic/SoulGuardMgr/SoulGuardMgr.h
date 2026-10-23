#pragma once 

#include <Directives.h>
#include "../../Common/CommonHeader.h"

// 수호정령 소환관련 리소스 정보의 갯수 
#define MAX_SOULGUARDINFO_NUM					40

// 수호정령 능력
#define SOULGUARD_ABILITY_NONE					0x00000000	//아무것도 아님

#define SOULGUARD_ABILITY_DROPITEM				0x00000001	// 사냥시 일정확률로 드랍 아이템 갯수 늘어남
#define SOULGUARD_ABILITY_RECOVER_HP			0x00000002	// 사냥시 일정확률로 캐릭터 HP 회복
#define SOULGUARD_ABILITY_RECOVER_MP			0x00000004	// 사냥시 일정확률로 캐릭터 MP 회복
#define SOULGUARD_ABILITY_DAMAGE_DOWN			0x00000008	// 사냥시 일정확률로 몬스터로부터 얻는 대미지가 감소한다
#define SOULGUARD_ABILITY_DAMAGE_UP				0x00000010	// 사냥시 일정확률로 대미지가 증가한다
#define SOULGUARD_ABILITY_ELEMENTDAMAGE_UP		0x00000020	// 사냥시 일정확률로 캐릭터의 모든 속성 공격을 증가 시켜준다
#define SOULGUARD_ABILITY_HUNGRY_UP				0x00000040	// 사냥시 일정확률로 캐릭터의 포만감을 올려준다
#define SOULGUARD_ABILITY_EXP_UP				0x00000080	// 사냥시 일정확률로 획득되는 경험치가 증가된다
#define SOULGUARD_ABILITY_SUMMONRECOVERY_HP		0x00000100	// 사냥시 일정확률로 소환수의 HP 회복
#define SOULGUARD_ABILITY_SOMMONRECOVERY_MP		0x00000200	// 사냥시 일정확률로 소환수의 MP 회복
#define SOULGUARD_ABILITY_MAKEABILITY_UP		0x00000400	// 제조시 일정확률로 아이템 능력이 향상된다
#define SOULGUARD_ABILITY_MAKETIME_SHORT		0x00000800	// 제조시 일정확률로 제조 시간이 설정초로 된다
#define SOULGUARD_ABILITY_NOICE					0x00001000	// 사냥시 일정확률로 동빙에 걸렸을 경우 동빙을 풀어준다.
#define SOULGUARD_ABILITY_DEFENSE_MAGIC			0x00002000	// 사냥시 일정확률로 몬스터에게 맞을 때 마법을 발사한다.
#define SOULGUARD_ABILITY_ATTACK_MAGIC			0x00004000	// 사냥시 일정확률로 몬스터에게 공격 마법을 사용한다
#define SOULGUARD_ABILITY_ATTACK_STUN			0x00008000	// 사냥시 일정확률로 몬스터를 못움직이게 한다
#define SOULGUARD_ABILITY_MANA_ZERO				0x00010000	// 마법 사용시 일정 확률로 마나 사용을 0으로 한다
#define SOULGUARD_ABILITY_DEFENSE_SUMMON		0x00020000	// 사냥시 일정 확률로 20초동안 소환수를 소환한다
#define SOULGUARD_ABILITY_MAGIC_DEFENSE			0x00040000	// 사냥시 일정확률로 마법 공격 데미지를 줄여준다
#define SOULGUARD_ABILITY_ATTACK_SOULICE		0x00080000	// 사냥시 일정확률로 마법 공격 데미지를 줄여준다
#define SOULGUARD_ABILITY_ATTACK_DEFENSE		0x00100000	// 사냥시 일정확률로 몬스터에 맞을때 방어율을 올려준다


// 한 수호정령이 갖을수 있는 능력치의 갯수이다 꼭 위의 갯수와 맞을 필요는 없다
// 단 수호정령의 능력치가 최대 위의 갯수만큼 붙을수있으므로 조정이 필요할수도 있음
#define MAX_SOULGUARD_HAVE_ABILITY_NUM			8			// 수호정령이 갖을 수 있는 최대 능력치 갯수 

enum SOULGUARD_ABILITY_UNIT_ADD
{
	SOULGUARD_ABILITY_UNIT_ADD_INVALID = 0,		// 알수없는 오류

	SOULGUARD_ABILITY_UNIT_ADD_FAIL_COUNT,		// 넣을 수 있는 갯수를 초과
	SOULGUARD_ABILITY_UNIT_ADD_FAIL_ALREADY,	// 이미 넣으려는 곳에 데이터가 있다
	SOULGUARD_ABILITY_UNIT_ADD_FAIL_SAME,		// 같은 데이터가 있다

	SOULGUARD_ABILITY_UNIT_ADD_SUCCESS			// 성공
};


class cltSoulGuardAbilityUnit
{
private:
	SI32	m_siAbilityType;	// 발동 능력
	SI16	m_siValue;			// 발동시 적용될 능력치

	SI16	m_siProbably;		// 발동 확률

	SI16	m_siDecFellowShip;	// 소모되는 친화력
	SI16	m_siCoolTimeSec;	// 능력발휘시 쿨타임(초단위임)

public:
	cltSoulGuardAbilityUnit()
	{
		Init();
	}

	~cltSoulGuardAbilityUnit()
	{
		
	}

	void Init( void )
	{
		ZeroMemory( this, sizeof(cltSoulGuardAbilityUnit) );
	}

	void Set( cltSoulGuardAbilityUnit* pclUnit )
	{
		if ( pclUnit )
		{
			memcpy( this, pclUnit, sizeof(cltSoulGuardAbilityUnit) );
		}
	}

	void Set( SI32 siAbilityType, SI16 siValue, SI16 siProbably, SI16 siDecFellowShip, SI16 siCoolTimeSec )
	{
		m_siAbilityType		= siAbilityType;
		m_siValue			= siValue;

		m_siProbably		= siProbably;

		m_siDecFellowShip	= siDecFellowShip;
		m_siCoolTimeSec		= siCoolTimeSec;
	}

public:
	SI32 GetAbilityType( void )						{	return m_siAbilityType;					}
	void SetAbilityType( SI32 siAbilityType )		{	m_siAbilityType = siAbilityType;		}

	SI16 GetValue( void )							{	return m_siValue;						}
	void SetValue( SI16 siValue )					{	m_siValue = siValue;					}

	SI16 GetProbably( void )						{	return m_siProbably;					}
	void SetProbably( SI16 siProbably )				{	m_siProbably = siProbably;				}

	SI16 GetDecFellowShip( void )					{	return m_siDecFellowShip;				}
	void SetDecFellowShip( SI16 siDecFellowShip )	{	m_siDecFellowShip = siDecFellowShip;	}

	SI16 GetCoolTimeSec( void )						{	return m_siCoolTimeSec;					}
	void SetCoolTimeSec( SI16 siCoolTimeSec )		{	m_siCoolTimeSec = siCoolTimeSec;		}
};

class cltSoulGuardAbilityList
{
private:
	SI16						m_siHaveCount;	// 내가 갖고 있는 유닛의 갯수, 당연한 얘기이지만 MAX_SOULGUARD_HAVE_ABILITY_NUM를 넘을수는 없다
	cltSoulGuardAbilityUnit*	m_pclAbilityUnit[MAX_SOULGUARD_HAVE_ABILITY_NUM];

public:
	cltSoulGuardAbilityList();
	~cltSoulGuardAbilityList();
	
public:
	SI32						AddUnit( cltSoulGuardAbilityUnit* pclUnit );
	cltSoulGuardAbilityUnit*	GetUnit( SI32 siAbilityType );

	SI32						GetHaveAbility( void );

};

class cltSoulGuardInfoUnit
{
private:
	SI16						m_siKindUnique;		// 수호정령 카인드
	SI16						m_siLife;			// 수호정령 생명

	SI16						m_siFellowShip;		// 수호정령 친화도

	SI32						m_siItemUnique;		// 이 아이템을 사용하면 위에 카인드의 수호정령이 나온다

private:
	cltSoulGuardAbilityList		m_clAbilityList;	// 이 수호정령이 갖고 있는 능력 목록
	

public:
	cltSoulGuardInfoUnit();
	~cltSoulGuardInfoUnit();

	void Init( void );

public:
	void SetInfo( SI16 siKindUnique, SI16 siLife, SI16 siFellowShip, SI32 siItemUnique )
	{
		m_siKindUnique	= siKindUnique;
		m_siLife		= siLife;		
	
		m_siFellowShip	= siFellowShip;

		m_siItemUnique	= siItemUnique;
	}

public:
	SI16 GetKindUnique( void )					{	return m_siKindUnique;			}
	void SetKindUnique( SI16 siKindUnique )		{	m_siKindUnique = siKindUnique;	}

	SI16 GetLife( void )						{	return m_siLife;				}
	void SetLife( SI16 siLife )					{	m_siLife = siLife;				}

	SI16 GetFellowShip( void )					{	return m_siFellowShip;			}
	void SetFellowShip( SI16 siFellowShip )		{	m_siFellowShip = siFellowShip;	}

	SI32 GetItemUnique( void )					{	return m_siItemUnique;			}
	void SetItemUnique( SI32 siItemUnique )		{	m_siItemUnique = siItemUnique;	}

public:
	void						SetUnit( cltSoulGuardAbilityUnit* pclUnit );
	cltSoulGuardAbilityUnit*	GetUnitFromAbilityType( SI32 siAbilityType );

	SI32 GetHaveAbility( void )
	{
		return m_clAbilityList.GetHaveAbility();
	}


public:
	
};

typedef NSafeTArray<cltSoulGuardInfoUnit, MAX_SOULGUARDINFO_NUM> NSafe_SoulGuardInfoUnit;

class cltSoulGuardInfoList
{
private:
#ifdef _SAFE_MEMORY
	NSafe_SoulGuardInfoUnit	m_clInfoList;
#else
	cltSoulGuardInfoUnit	m_clInfoList[MAX_SOULGUARDINFO_NUM];
#endif

	SI32					m_siListCount;

public:
	cltSoulGuardInfoList();
	~cltSoulGuardInfoList();

	void						Init( );
public:
	BOOL						LoadSoulGuardInfo( void );

private:
	void						AddSoulGuardInfoUnit( SI16 siKindUnique, SI16 siLife, SI16 siFellowShip, SI32 siItemUnique, cltSoulGuardAbilityUnit* pclUnit );
	SI32						ConvertAbilityTypeFromText( TCHAR* pszAbilityType );

public:
	cltSoulGuardInfoUnit*		GetUnitFromItemUnique( SI32 siItemUnique );
	cltSoulGuardInfoUnit*		GetUnitFromKindUnique( SI32 siKindUnique );

	SI32						GetHaveAbilityFromKindUnique( SI16 siKindUnique );

	cltSoulGuardAbilityUnit*	GetUnitFromKindUniqueAndAbilityType( SI16 siKindUnique, SI32 siAbilityType );
};

class cltSoulGuardMgr
{
private:
	cltSoulGuardInfoList	m_clSoulGuardInfoList;

public:
	cltSoulGuardMgr();
	~cltSoulGuardMgr();

	void Init();

public:
	cltSoulGuardInfoUnit*		GetSoulGuardUnitFromItemUnique( SI32 siItemUnique );
	cltSoulGuardInfoUnit*		GetSoulGuardUnitFromKindUnique( SI32 siKindUnique );

	cltSoulGuardAbilityUnit*	GetAbilityUnit( SI16 siKindUnique, SI32 siAbility );
	SI32						GetHaveAbility( SI16 siKindUnique );

public:
	SI32						GetSoulGuardSpeechTextNum( SI32 siAbility );
	SI32						GetSoulGuardExplainTextNum( SI32 siAbility );
};