#include "SoulGuardMgr.h"
#include "../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

cltSoulGuardAbilityList::cltSoulGuardAbilityList()
{
	ZeroMemory( this, sizeof(cltSoulGuardAbilityList) );
}

cltSoulGuardAbilityList::~cltSoulGuardAbilityList()
{
	for (SI32 siIndex=0; siIndex<MAX_SOULGUARD_HAVE_ABILITY_NUM; siIndex++)
	{
		if ( m_pclAbilityUnit[siIndex] )
		{
			delete m_pclAbilityUnit[siIndex];
			m_pclAbilityUnit[siIndex] = NULL;
		}
	}
}

SI32 cltSoulGuardAbilityList::AddUnit( cltSoulGuardAbilityUnit* pclUnit )
{
	if ( NULL == pclUnit )
	{
		return SOULGUARD_ABILITY_UNIT_ADD_INVALID;
	}

	if ( (0 > m_siHaveCount) || (MAX_SOULGUARD_HAVE_ABILITY_NUM <= m_siHaveCount) )
	{
		return SOULGUARD_ABILITY_UNIT_ADD_FAIL_COUNT;
	}

	// 리스트가 존재할 경우 해당 능력치가 혹시 존재 하는지 검사한다
	if ( 0 < m_siHaveCount )
	{
		for ( SI32 siIndex=0; siIndex<m_siHaveCount; siIndex++  )
		{
			if ( NULL == m_pclAbilityUnit[siIndex] )
			{
				continue;
			}

			if ( m_pclAbilityUnit[siIndex]->GetAbilityType() == pclUnit->GetAbilityType() )
			{
				return SOULGUARD_ABILITY_UNIT_ADD_FAIL_SAME;
			}
		}
	}

	// 이미 해당 리스트에 정보가 존재한다면
	if ( m_pclAbilityUnit[m_siHaveCount] )
	{
		return SOULGUARD_ABILITY_UNIT_ADD_FAIL_ALREADY;
	}

	m_pclAbilityUnit[m_siHaveCount] = new cltSoulGuardAbilityUnit;
	// 생성이 안된다면
	if ( NULL == m_pclAbilityUnit[m_siHaveCount] )
	{
		return SOULGUARD_ABILITY_UNIT_ADD_INVALID;
	}

	// 실제 데이터를 넣고
	m_pclAbilityUnit[m_siHaveCount]->Set( pclUnit );

	// 카운트 증가
	m_siHaveCount++;

	return SOULGUARD_ABILITY_UNIT_ADD_SUCCESS;

}

cltSoulGuardAbilityUnit* cltSoulGuardAbilityList::GetUnit( SI32 siAbilityType )
{
	if ( 0 >= m_siHaveCount )
	{
		return NULL;
	}

	SI32 siRealCount = (MAX_SOULGUARD_HAVE_ABILITY_NUM<m_siHaveCount?MAX_SOULGUARD_HAVE_ABILITY_NUM:m_siHaveCount);
	for ( SI32 siIndex=0; siIndex<siRealCount; siIndex++ )
	{
		// 순차적으로 넣기때문에 NULL이 나오면 바로 끝내도 됨
		if ( NULL == m_pclAbilityUnit[siIndex] )
		{
			break;
		}

		if ( m_pclAbilityUnit[siIndex]->GetAbilityType() == siAbilityType )
		{
			return m_pclAbilityUnit[siIndex];
		}
	}

	return  NULL;
}

SI32 cltSoulGuardAbilityList::GetHaveAbility()
{
	if ( 0 >= m_siHaveCount )
	{
		return 0;
	}

	SI32 siHaveAbility = 0;

	SI32 siRealCount = (MAX_SOULGUARD_HAVE_ABILITY_NUM<m_siHaveCount?MAX_SOULGUARD_HAVE_ABILITY_NUM:m_siHaveCount);
	for ( SI32 siIndex=0; siIndex<siRealCount; siIndex++ )
	{
		// 순차적으로 넣기때문에 NULL이 나오면 바로 끝내도 됨
		if ( NULL == m_pclAbilityUnit[siIndex] )
		{
			break;
		}

		// 갖고 있는 모든 어빌리티를 가져온다
		siHaveAbility |= m_pclAbilityUnit[siIndex]->GetAbilityType();
	}

	return siHaveAbility;
}

//--------------------------------------------------------------------------------------------------
// 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선
//--------------------------------------------------------------------------------------------------
cltSoulGuardInfoUnit::cltSoulGuardInfoUnit()
{
	Init();
}

cltSoulGuardInfoUnit::~cltSoulGuardInfoUnit()
{

}

void cltSoulGuardInfoUnit::Init()
{
	ZeroMemory( this, sizeof(cltSoulGuardInfoUnit) );
}

void cltSoulGuardInfoUnit::SetUnit( cltSoulGuardAbilityUnit* pclUnit )
{
	if ( NULL == pclUnit )
	{
		return;
	}

	SI32 siReturnValue = m_clAbilityList.AddUnit( pclUnit );
	switch ( siReturnValue )
	{
	case SOULGUARD_ABILITY_UNIT_ADD_INVALID:
		{
			MsgBox( TEXT("수호정령 어빌리티 오류"), TEXT("오류가 났습니다 확인해 주세요") );
		}
		break;

	case SOULGUARD_ABILITY_UNIT_ADD_FAIL_COUNT:		// 넣을 수 있는 갯수를 초과
		{
			MsgBox( TEXT("수호정령 어빌리티 오류"), TEXT("넣을 수 있는 최대 갯수를 초과 했습니다") );
		}
		break;

	case SOULGUARD_ABILITY_UNIT_ADD_FAIL_ALREADY:	// 이미 넣으려는 곳에 데이터가 있다
		{
			MsgBox( TEXT("수호정령 어빌리티 오류"), TEXT("이미 이곳에는 데이터가 있습니다") );
		}
		break;

	case SOULGUARD_ABILITY_UNIT_ADD_FAIL_SAME:		// 같은 데이터가 있다
		{
			MsgBox( TEXT("수호정령 어빌리티 오류"), TEXT("해당 카인드에 동일 어빌리티 정보가 있습니다") );
		}
		break;
	}

}

cltSoulGuardAbilityUnit* cltSoulGuardInfoUnit::GetUnitFromAbilityType( SI32 siAbilityType )
{
	return m_clAbilityList.GetUnit( siAbilityType );
}

//--------------------------------------------------------------------------------------------------
// 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선
//--------------------------------------------------------------------------------------------------

cltSoulGuardInfoList::cltSoulGuardInfoList()
{
	Init();
}

cltSoulGuardInfoList::~cltSoulGuardInfoList()
{
}

void cltSoulGuardInfoList::Init()
{
#ifdef _SAFE_MEMORY
	m_clInfoList.ZeroMem();
#else
	ZeroMemory( m_clInfoList, (sizeof(cltSoulGuardInfoUnit) * MAX_SOULGUARDINFO_NUM) );
#endif

	m_siListCount = 0;
}

BOOL cltSoulGuardInfoList::LoadSoulGuardInfo()
{
	TCHAR		szFileName[MAX_PATH]	= { "Data\\SoulGuardInfo.txt" };

	NDataLoader	dataloader;
	BOOL		bLoadFile				= FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("cltSoulGuardInfoList::LoadSoulGuardInfo()"), TEXT("LoadError:[%s]"), szFileName );
		return FALSE;
	}

	TCHAR	szKindType[128]		= TEXT("");
	SI32	siLife				= 0;
	SI32	siFellowShip		= 0;
	SI32	siItemUnique		= 0;

	TCHAR	szAbilityType[128]	= TEXT("");
	SI32	siValue				= 0;
	SI32	siProbably			= 0;
	SI32	siDecFellowShip		= 0;
	SI32	siCoolTimeSec		= 0;
	
	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	&szKindType,		128,
		NDATA_INT32,	&siLife,			4,
		NDATA_INT32,	&siFellowShip,		4,
		NDATA_INT32,	&siItemUnique,		4,

		NDATA_MBSTRING,	&szAbilityType,		128,
		NDATA_INT32,	&siValue,			4,
		NDATA_INT32,	&siProbably,		4,
		NDATA_INT32,	&siDecFellowShip,	4,
		NDATA_INT32,	&siCoolTimeSec,		4,
		0,0,0
	};

	SI32	siKIndType		= 0;
	SI32	siAbilityType	= 0;
	bool	bFind			= false;
	
	cltSoulGuardAbilityUnit clUnit;

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			if ( MAX_SOULGUARDINFO_NUM <= m_siListCount )
			{
				MsgBox( TEXT("Load Error"), TEXT("SoulGuardInfo.txt 대이터 내용 갯수 초과") );		
				break;
			}

			if ( 0 >= siDecFellowShip )
			{
				MsgBox( TEXT("Load Error"), TEXT("SoulGuardInfo.txt DecFellowShip의 값은 0 보다 커야 합니다") );		
			}

			bFind = pclClient->pclHashTableString->Find( szKindType, siKIndType );
			if ( false == bFind )
			{
				MsgBox( TEXT("SoulGuardInfo.txt"), TEXT("[%s]카인드를 찾을수없습니다"), szKindType );		
				continue;
			}
			
			// 문자열로된 능력 타입을 숫자로 변경
			siAbilityType = ConvertAbilityTypeFromText( szAbilityType );

			// 합쳐서 보내준다~
			clUnit.Set( siAbilityType, siValue, siProbably, siDecFellowShip, siCoolTimeSec );

			AddSoulGuardInfoUnit( siKIndType, siLife, siFellowShip, siItemUnique, &clUnit );

			m_siListCount++;
		}
	}

	return TRUE;
}

void cltSoulGuardInfoList::AddSoulGuardInfoUnit( SI16 siKindUnique, SI16 siLife, SI16 siFellowShip, SI32 siItemUnique, cltSoulGuardAbilityUnit* pclUnit )
{
	if ( (0 > m_siListCount) || (MAX_SOULGUARDINFO_NUM <= m_siListCount) )
	{
		return;
	}

	if ( NULL == pclUnit )
	{
		return;
	}

	BOOL	bFind		= FALSE; 
	SI32	siFindIndex	= 0;

	// 해당 카인드의 기본 정보가 이미 입력되있다면 어빌리티 정보만 입력해 준다
	for ( SI32 siIndex=0; siIndex<m_siListCount; siIndex++ )
	{
		if ( m_clInfoList[siIndex].GetKindUnique() == siKindUnique )
		{
			siFindIndex	= siIndex;
			bFind		= TRUE;
			break;
		}
	}

	if ( bFind )
	{
		m_clInfoList[siFindIndex].SetUnit( pclUnit );
	}
	else
	{
		m_clInfoList[m_siListCount].SetInfo( siKindUnique, siLife, siFellowShip, siItemUnique );
		m_clInfoList[m_siListCount].SetUnit( pclUnit );

		// 같고 있는 리스트 갯수를 늘려준다
		m_siListCount++;
	}

}

SI32 cltSoulGuardInfoList::ConvertAbilityTypeFromText( TCHAR* pszAbilityType )
{
	if ( NULL == pszAbilityType )
	{
		MsgBox( TEXT("SoulGuard Ability Type Text Error"), TEXT("Incorrect Type Text[NULL]") );
		return 0;
	}

	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_NONE );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_DROPITEM );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_RECOVER_HP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_RECOVER_MP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_DAMAGE_DOWN );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_DAMAGE_UP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_ELEMENTDAMAGE_UP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_HUNGRY_UP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_EXP_UP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_SUMMONRECOVERY_HP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_SOMMONRECOVERY_MP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_MAKEABILITY_UP );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_MAKETIME_SHORT );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_NOICE );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_DEFENSE_MAGIC );

	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_ATTACK_MAGIC );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_ATTACK_STUN );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_MANA_ZERO );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_DEFENSE_SUMMON );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_MAGIC_DEFENSE );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_ATTACK_SOULICE );
	COMPARE_TEXT_WITH_VALUE( pszAbilityType, SOULGUARD_ABILITY_ATTACK_DEFENSE );
	

	MsgBox( TEXT("SoulGuard Ability Type Text Error"), TEXT("Incorrect Type Text[%s]"), pszAbilityType );

	return 0;
}

cltSoulGuardInfoUnit* cltSoulGuardInfoList::GetUnitFromItemUnique( SI32 siItemUnique )
{
	if ( 0 >= siItemUnique )
	{
		return NULL;
	}

	SI32 siRealCount = (MAX_SOULGUARDINFO_NUM<m_siListCount ? MAX_SOULGUARDINFO_NUM : m_siListCount );
	for ( SI32 siIndex=0; siIndex<siRealCount; siIndex++ )
	{
		if ( m_clInfoList[siIndex].GetItemUnique() == siItemUnique )
		{
			return &m_clInfoList[siIndex];
		}
	}

	return NULL;
}

cltSoulGuardInfoUnit* cltSoulGuardInfoList::GetUnitFromKindUnique( SI32 siKindUnique )
{
	if ( 0 >= siKindUnique )
	{
		return NULL;
	}

	SI32 siRealCount = (MAX_SOULGUARDINFO_NUM<m_siListCount ? MAX_SOULGUARDINFO_NUM : m_siListCount );
	for ( SI32 siIndex=0; siIndex<siRealCount; siIndex++ )
	{
		if ( m_clInfoList[siIndex].GetKindUnique() == siKindUnique )
		{
			return &m_clInfoList[siIndex];
		}
	}

	return NULL;
}

SI32 cltSoulGuardInfoList::GetHaveAbilityFromKindUnique( SI16 siKindUnique )
{
	if ( 0 >= siKindUnique )
	{
		return 0;
	}

	SI32 siRealCount = (MAX_SOULGUARDINFO_NUM<m_siListCount ? MAX_SOULGUARDINFO_NUM : m_siListCount );
	for ( SI32 siIndex=0; siIndex<siRealCount; siIndex++ )
	{
		if ( m_clInfoList[siIndex].GetKindUnique() == siKindUnique )
		{
			return m_clInfoList[siIndex].GetHaveAbility();
		}
	}

	return 0;
	
}

cltSoulGuardAbilityUnit* cltSoulGuardInfoList::GetUnitFromKindUniqueAndAbilityType( SI16 siKindUnique, SI32 siAbilityType )
{
	if ( (0 >= siKindUnique) || (0 >= siAbilityType) )
	{
		return NULL;
	}

	SI32 siRealCount = (MAX_SOULGUARDINFO_NUM<m_siListCount ? MAX_SOULGUARDINFO_NUM : m_siListCount );
	for ( SI32 siIndex=0; siIndex<siRealCount; siIndex++ )
	{
		if ( m_clInfoList[siIndex].GetKindUnique() == siKindUnique )
		{
			return m_clInfoList[siIndex].GetUnitFromAbilityType( siAbilityType );
		}
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------
// 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선
//--------------------------------------------------------------------------------------------------

cltSoulGuardMgr::cltSoulGuardMgr() : m_clSoulGuardInfoList()
{
	m_clSoulGuardInfoList.LoadSoulGuardInfo();
}

cltSoulGuardMgr::~cltSoulGuardMgr()
{

}

void cltSoulGuardMgr::Init()
{
	m_clSoulGuardInfoList.Init();
}

cltSoulGuardInfoUnit* cltSoulGuardMgr::GetSoulGuardUnitFromItemUnique( SI32 siItemUnique )
{
	return m_clSoulGuardInfoList.GetUnitFromItemUnique( siItemUnique );
}

cltSoulGuardInfoUnit* cltSoulGuardMgr::GetSoulGuardUnitFromKindUnique( SI32 siKindUnique )
{
	return m_clSoulGuardInfoList.GetUnitFromKindUnique( siKindUnique );
}

cltSoulGuardAbilityUnit* cltSoulGuardMgr::GetAbilityUnit( SI16 siKindUnique, SI32 siAbilityType )
{
	return m_clSoulGuardInfoList.GetUnitFromKindUniqueAndAbilityType( siKindUnique, siAbilityType );
}

SI32 cltSoulGuardMgr::GetHaveAbility( SI16 siKindUnique )
{
	return m_clSoulGuardInfoList.GetHaveAbilityFromKindUnique( siKindUnique );
}
//능력쓸때 대사
SI32 cltSoulGuardMgr::GetSoulGuardSpeechTextNum( SI32 siAbility )
{
	switch ( siAbility )
	{
	case SOULGUARD_ABILITY_DROPITEM:			return (30106);
	case SOULGUARD_ABILITY_RECOVER_HP:			return (30107);
	case SOULGUARD_ABILITY_RECOVER_MP:			return (30108);
	case SOULGUARD_ABILITY_DAMAGE_DOWN:			return (30109);
	case SOULGUARD_ABILITY_DAMAGE_UP:			return (30110);
	case SOULGUARD_ABILITY_ELEMENTDAMAGE_UP:	return (30111);
	case SOULGUARD_ABILITY_HUNGRY_UP:			return (30112);
	case SOULGUARD_ABILITY_EXP_UP:				return (30113);
	case SOULGUARD_ABILITY_SUMMONRECOVERY_HP:	return (30114);
	case SOULGUARD_ABILITY_SOMMONRECOVERY_MP:	return (30115);
	case SOULGUARD_ABILITY_MAKEABILITY_UP:		return (30116);
	case SOULGUARD_ABILITY_MAKETIME_SHORT:		return (30117);
	case SOULGUARD_ABILITY_NOICE:				return (30243);
	case SOULGUARD_ABILITY_DEFENSE_MAGIC:		return (30244);

	case SOULGUARD_ABILITY_ATTACK_MAGIC:		return (40010);
	case SOULGUARD_ABILITY_ATTACK_STUN:			return (40011);
	case SOULGUARD_ABILITY_MANA_ZERO:			return (40012);
	case SOULGUARD_ABILITY_DEFENSE_SUMMON:		return (40013);
	case SOULGUARD_ABILITY_MAGIC_DEFENSE:		return (40014);
	// 얼음정령 능력쓸때 대사
	case SOULGUARD_ABILITY_ATTACK_SOULICE:		return (40265);
	case SOULGUARD_ABILITY_ATTACK_DEFENSE:		return (40266);
		

	}

	return 0;
}
// 정령 능력 설명
SI32 cltSoulGuardMgr::GetSoulGuardExplainTextNum( SI32 siAbility )
{
	switch ( siAbility )
	{
	case SOULGUARD_ABILITY_DROPITEM:			return (30219);
	case SOULGUARD_ABILITY_RECOVER_HP:			return (30220);
	case SOULGUARD_ABILITY_RECOVER_MP:			return (30221);
	case SOULGUARD_ABILITY_DAMAGE_DOWN:			return (30222);
	case SOULGUARD_ABILITY_DAMAGE_UP:			return (30223);
	case SOULGUARD_ABILITY_ELEMENTDAMAGE_UP:	return (30224);
	case SOULGUARD_ABILITY_HUNGRY_UP:			return (30225);
	case SOULGUARD_ABILITY_EXP_UP:				return (30226);
	case SOULGUARD_ABILITY_SUMMONRECOVERY_HP:	return (30227);
	case SOULGUARD_ABILITY_SOMMONRECOVERY_MP:	return (30228);
	case SOULGUARD_ABILITY_MAKEABILITY_UP:		return (30229);
	case SOULGUARD_ABILITY_MAKETIME_SHORT:		return (30230);
	case SOULGUARD_ABILITY_NOICE:				return (30287);
	case SOULGUARD_ABILITY_DEFENSE_MAGIC:		return (30288);

	case SOULGUARD_ABILITY_ATTACK_MAGIC:		return (40015);
	case SOULGUARD_ABILITY_ATTACK_STUN:			return (40016);
	case SOULGUARD_ABILITY_MANA_ZERO:			return (40017);
	case SOULGUARD_ABILITY_DEFENSE_SUMMON:		return (40018);
	case SOULGUARD_ABILITY_MAGIC_DEFENSE:		return (40019);

	case SOULGUARD_ABILITY_ATTACK_SOULICE:			return (40275);	
	case SOULGUARD_ABILITY_ATTACK_DEFENSE:			return (40276);	
		
	}

	return 0;
}