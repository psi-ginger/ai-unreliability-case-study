#include "FishKind.h"

#include "CommonLogic.h"
extern cltCommonLogic* pclClient;

#define FISHING_RARE_ITEM_RATE_FOR_SCROLL	1	// 최대 내구 상승 부적을 줄 확률 1/100000 = 0.00001 = 0.001%
#define FISHING_RARE_ITEM_RATE_FOR_GOLD		100	// 금광석을 줄 확률 100/10000 = 1%

CFishKind::CFishKind()
{
	Init();
}

CFishKind::~CFishKind()
{
}

void CFishKind::Init()
{
	memset( this, 0, sizeof( CFishKind ) );

	return;
}


void CFishKind::SetFishKindInfo(
								SI32 FishItemUnique, 
								SI32 NeedFishingSkillLevel,
								SI32 IncFishingSkillExp,
								SI32 CatchPercent,
								SI32 IncCatchPercentFromSkillLevel,
								TCHAR *FishName, 
								TCHAR *FishingToolAtb,
								TCHAR *FishingToolList, 
								SI32 BaitItemUnique,
								TCHAR *FishCreatePlaceList,
								TCHAR *FishCreatePlaceRectList
								)
{
	m_siFishItemUnique = FishItemUnique;
	
	StringCchCopy( m_strFishName, MAX_PLAYER_NAME, FishName );

	m_stFishInfo.siIncCatchPercentFromSkillLevel = IncCatchPercentFromSkillLevel;

	m_stFishInfo.siIncFishingSkillExp = IncFishingSkillExp;

	m_stFishInfo.siNeedFishingSkillLevel = NeedFishingSkillLevel;
	m_stFishInfo.siCatchPercent = CatchPercent;

	m_stFishInfo.siBaitItemUniqueList[ 0 ] = BaitItemUnique;

	AnalyzeFishingToolAtb( FishingToolAtb );
	AnalyzeFishingToolList( FishingToolList );
	AnalyzeFishCreatePlaceList( FishCreatePlaceList );
	AnalyzeFishCreatePlaceRectList( FishCreatePlaceRectList );
}

// 낚시대+찌+ 이런 방식의 파싱
void CFishKind::AnalyzeFishingToolAtb( TCHAR *FishingToolAtb )
{
	SI32 CurrentIndex = 0;

	TCHAR FishingToolName[ 20 ] = TEXT("");
	
	SI32 PreIndex = 0;	

	SI16 FishingToolListIndex = 0;

	SI32 FishingToolItemUnique = 0;

	TCHAR* pText[2];
	pText[0] = GetTxtFromMgr(5080);
	pText[1] = GetTxtFromMgr(5081);
	bool loop = true;
	while( loop )
	{
		if( FishingToolAtb[CurrentIndex] == TEXT('\0') )
		{
			TMemCpy( FishingToolName, &FishingToolAtb[ PreIndex ], CurrentIndex - PreIndex );

			FishingToolName[ CurrentIndex - PreIndex ] = NULL;
			/*
			if ( _tcscmp( FishingToolName, pText[0] ) == 0 )
			{
				m_stFishInfo.siFishingRodAtb = m_stFishInfo.siFishingRodAtb|FISHINGROD_ATB_HOOK;
			}
			else if ( _tcscmp( FishingToolName, pText[1] ) == 0 )
			{
				m_stFishInfo.siFishingRodAtb = m_stFishInfo.siFishingRodAtb|FISHINGROD_ATB_STAND;
			}
			*/

			m_stFishInfo.siFishingRodAtb = m_stFishInfo.siFishingRodAtb|FISHINGROD_ATB_STAND;

			PreIndex = CurrentIndex + 1;

			break;
		}

		if ( FishingToolAtb[CurrentIndex] == TEXT('+') )
		{
			memcpy( FishingToolName, &FishingToolAtb[ PreIndex ], CurrentIndex - PreIndex );

			FishingToolName[ CurrentIndex - PreIndex ] = NULL;

			if ( _tcscmp( FishingToolName, pText[0] ) == 0 )
			{
				m_stFishInfo.siFishingRodAtb = m_stFishInfo.siFishingRodAtb|FISHINGROD_ATB_HOOK;
			}
			else if ( _tcscmp( FishingToolName, pText[1] ) == 0 )
			{
				m_stFishInfo.siFishingRodAtb = m_stFishInfo.siFishingRodAtb|FISHINGROD_ATB_STAND;
			}

			PreIndex = CurrentIndex + 1;
		}

		++CurrentIndex;
	}

	return;
}


// 낚시대+찌+ 이런 방식의 파싱
void CFishKind::AnalyzeFishingToolList( TCHAR *FishingToolList )
{
	SI32 CurrentIndex = 0;

	TCHAR FishingToolName[ 20 ] = TEXT("");
	
	SI32 PreIndex = 0;	

	SI16 FishingToolListIndex = 0;

	SI32 FishingToolItemUnique = 0;

	bool loop = true;
	while( loop )
	{
		if( FishingToolList[CurrentIndex] == TEXT('\0') )
		{
			TMemCpy( FishingToolName, &FishingToolList[ PreIndex ], CurrentIndex - PreIndex );

			FishingToolName[ CurrentIndex - PreIndex ] = NULL;

			
			SI32 Unique = _tstoi( FishingToolName );
			
			if(Unique)
			{
				m_stFishInfo.siFishingToolItemUniqueList[ FishingToolListIndex++ ] = Unique;
			}

			PreIndex = CurrentIndex + 1;

			break;
		}

		if ( FishingToolList[CurrentIndex] == TEXT('.') || FishingToolList[CurrentIndex] == TEXT('+') )
		{
			TMemCpy( FishingToolName, &FishingToolList[ PreIndex ], CurrentIndex - PreIndex );

			FishingToolName[ CurrentIndex - PreIndex ] = NULL;

			SI32 Unique = _tstoi( FishingToolName );
			
			if(Unique)
			{
				m_stFishInfo.siFishingToolItemUniqueList[ FishingToolListIndex++ ] = Unique;
			}

			PreIndex = CurrentIndex + 1;
		}

		++CurrentIndex;
	}

	return;
}

// 미꾸라지+소라+ 이런 방식의 파싱
void CFishKind::AnalyzeBaitItemUniqueList( TCHAR *BaitItemUniqueList )
{
	SI32 CurrentIndex = 0;

	TCHAR BaitItemName[ MAX_PLAYER_NAME ] = TEXT("");
	
	SI32 PreIndex = 0;	

	SI16 BaitItemUniqueListIndex = 0;

	SI32 FishingToolItemUnique = 0;

	bool roop = true;

	while( roop )
	{
		if( BaitItemUniqueList[CurrentIndex] == TEXT('\0') )
		{
			TMemCpy( BaitItemName, &BaitItemUniqueList[ PreIndex ], CurrentIndex - PreIndex );

			BaitItemName[ CurrentIndex - PreIndex ] = NULL;

			
			SI32 Unique = pclClient->pclItemManager->FindUniqueFromName( BaitItemName );
			
			if(Unique)
			{
				m_stFishInfo.siBaitItemUniqueList[ BaitItemUniqueListIndex++ ] = Unique;
			}

			PreIndex = CurrentIndex + 1;

			break;
		}

		if ( BaitItemUniqueList[CurrentIndex] == TEXT('+') )
		{
			TMemCpy( BaitItemName, &BaitItemUniqueList[ PreIndex ], CurrentIndex - PreIndex );

			BaitItemName[ CurrentIndex - PreIndex ] = NULL;

			
			SI32 Unique = pclClient->pclItemManager->FindUniqueFromName( BaitItemName );
			
			if(Unique)
			{
				m_stFishInfo.siBaitItemUniqueList[ BaitItemUniqueListIndex++ ] = Unique;
			}

			PreIndex = CurrentIndex + 1;
		}

		++CurrentIndex;
	}

	return;
}

// 민물+해안+근해+원해 이런 방식의 파싱
void CFishKind::AnalyzeFishCreatePlaceList( TCHAR *FishCreatePlaceList )
{
	SI32 CurrentIndex = 0;

	TCHAR FishCreatePlaceName[ MAX_PLAYER_NAME ] = TEXT("");
	
	SI32 PreIndex = 0;	

	SI16 FishCreatePlaceListIndex = 0;

	SI32 FishingToolItemUnique = 0;
	TCHAR* pText[4];
	pText[0] = GetTxtFromMgr(5082);
	pText[1] = GetTxtFromMgr(5083);
	pText[2] = GetTxtFromMgr(5084);
	pText[3] = GetTxtFromMgr(5085);

	bool loop = true;
	while( loop )
	{
		if( FishCreatePlaceList[CurrentIndex] == TEXT('\0') )
		{
			TMemCpy( FishCreatePlaceName, &FishCreatePlaceList[ PreIndex ], CurrentIndex - PreIndex );

			FishCreatePlaceName[ CurrentIndex - PreIndex ] = NULL;

			if ( _tcscmp( FishCreatePlaceName,pText[0] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_NEARSEA;
			}
			else if ( _tcscmp( FishCreatePlaceName, pText[1] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_SEASIDE;
			}
			else if ( _tcscmp( FishCreatePlaceName, pText[2] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_FARSEA;
			}
			else if ( _tcscmp( FishCreatePlaceName, pText[3] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_RIVER;
			}

			PreIndex = CurrentIndex + 1;

			break;
		}

		if ( FishCreatePlaceList[CurrentIndex] == TEXT('+') )
		{
			TMemCpy( FishCreatePlaceName, &FishCreatePlaceList[ PreIndex ], CurrentIndex - PreIndex );

			FishCreatePlaceName[ CurrentIndex - PreIndex ] = NULL;

			if ( _tcscmp( FishCreatePlaceName, pText[0] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_NEARSEA;
			}
			else if ( _tcscmp( FishCreatePlaceName, pText[1] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_SEASIDE;
			}
			else if ( _tcscmp( FishCreatePlaceName, pText[2] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_FARSEA;
			}
			else if ( _tcscmp( FishCreatePlaceName, pText[3] ) == 0 )
			{
				m_stFishInfo.siFishCreatePlaceList[ FishCreatePlaceListIndex++ ] = FISHING_PLACE_RIVER;
			}

			PreIndex = CurrentIndex + 1;
		}

		++CurrentIndex;
	}

	return;
}

//(1,1,12,2)(3,4,5,6) 이런방식 구성의 파싱임
void CFishKind::AnalyzeFishCreatePlaceRectList( TCHAR *FishCreatePlaceRectList )
{
	SI32 CurrentIndex = 0;

	SI32 PreIndex = 0;

	TCHAR szPos[ 20 ];

	SI16 FishCreatePlaceRectListIndex = 0;

	RECT rect;
	SI16 RectCount = 0;

	bool loop = true;
	while( loop )
	{
		if( FishCreatePlaceRectList[CurrentIndex] == TEXT('\0') )
			break;

		if ( FishCreatePlaceRectList[CurrentIndex] == TEXT('.') )
		{
			TMemCpy ( szPos, &FishCreatePlaceRectList[ PreIndex ], CurrentIndex - PreIndex );
			szPos[ CurrentIndex - PreIndex ] = NULL;

			SI32 siPos = _tstoi( szPos );

			switch ( RectCount++ )
			{
			case 0:
				rect.left = siPos;				
				break;
			case 1:
				rect.top = siPos;
				break;
			case 2:
				rect.right = siPos;
				break;
			case 3:
				rect.bottom = siPos;
				break;				
			}

			PreIndex = CurrentIndex + 1;
		}
		else if( FishCreatePlaceRectList[CurrentIndex] == TEXT('(') )
		{
			PreIndex = CurrentIndex + 1;
		}		
		else if( FishCreatePlaceRectList[CurrentIndex] == TEXT(')') )
		{
			TMemCpy ( szPos, &FishCreatePlaceRectList[ PreIndex ], CurrentIndex - PreIndex );
			szPos[ CurrentIndex - PreIndex ] = NULL;

			SI32 siPos = _tstoi( szPos );
			rect.bottom = siPos;
			PreIndex = CurrentIndex + 1;

			memcpy( &m_stFishInfo.FishCreatePlaceRectList[ FishCreatePlaceRectListIndex++ ], &rect, sizeof( RECT ) );

			RectCount = 0;
		}

		++CurrentIndex;
	}

	return;
}

SI32 CFishKind::GetFishItemUnique()
{
	if ( m_siFishItemUnique < 1 )
	{
		return -1;
	}

	return m_siFishItemUnique;
}

bool CFishKind::IsSameFishItemUnique( SI32 FishItemUnique )
{
	if ( m_siFishItemUnique < 1 || FishItemUnique < 1 )
	{
		return false;
	}
	
	if ( m_siFishItemUnique == FishItemUnique )
	{
		return true;
	}

	return false;
}

stFishInfo *CFishKind::GetFishInfoPtr()
{
	if ( m_siFishItemUnique < 1 )
	{
		return NULL;
	}

	return &m_stFishInfo;
}

bool CFishKind::GetFishInfo( stFishInfo *pFishInfo )
{
	if ( pFishInfo == NULL || m_siFishItemUnique < 1 )
	{
		return false;
	}

	memcpy ( pFishInfo, &m_stFishInfo, sizeof( stFishInfo ) );

	return true;
}

bool CFishKind::IsAvailableGainFish( SI32 FishingBaitItemUnique, SI32 PosX, SI32 PosY )
{
	if ( FishingBaitItemUnique < 1 )
	{
		return false;
	}

	SI16 i = 0;

	for ( i = 0; i < MAX_BAIT_ITEM_NUMBER; ++i )
	{
		if ( m_stFishInfo.siBaitItemUniqueList[ i ] == FishingBaitItemUnique )
		{
			break;
		}
	}

	if ( i == MAX_BAIT_ITEM_NUMBER )
	{
		return false;
	}

	for ( i = 0; i < MAX_CREATE_FISH_PLACE_LIST; ++i )
	{
		if ( m_stFishInfo.FishCreatePlaceRectList[ i ].left		<= PosX && 
			 m_stFishInfo.FishCreatePlaceRectList[ i ].right	>= PosX &&
			 m_stFishInfo.FishCreatePlaceRectList[ i ].top		<= PosY &&
			 m_stFishInfo.FishCreatePlaceRectList[ i ].bottom	>= PosY )
		{			
			return true;
		}
	}

	return false;
}

bool CFishKind::IsAvailableGainFish( SI32 PosX, SI32 PosY )
{
	for ( SI16 i = 0; i < MAX_CREATE_FISH_PLACE_LIST; ++i )
	{
		if ( m_stFishInfo.FishCreatePlaceRectList[ i ].left <= PosX && 
			m_stFishInfo.FishCreatePlaceRectList[ i ].right >= PosX &&
			m_stFishInfo.FishCreatePlaceRectList[ i ].top <= PosY &&
			m_stFishInfo.FishCreatePlaceRectList[ i ].bottom >= PosY )
		{			
			return true;
		}
	}

	return false;
}

bool CFishKind::IsAvailableBait( SI32 BaitItemUnique )
{
	if ( BaitItemUnique < 1 )
	{
		return false;
	}

	for ( SI16 i = 0; i < MAX_BAIT_ITEM_NUMBER; ++i )
	{
		if (m_stFishInfo.siBaitItemUniqueList[ i ] == BaitItemUnique )
		{
			return true;
		}
	}

	return false;
}

SI16 CFishKind::GetCatchFishPercent( SI16 FishSkillLevel, SI16 FishingBaitItemUnique )
{
	if ( FishingBaitItemUnique < 1 )
	{
		return 0;
	}

	SI16 i = 0;

	for ( i = 0; i < MAX_BAIT_ITEM_NUMBER; ++i )
	{
		if ( m_stFishInfo.siBaitItemUniqueList[ i ] == FishingBaitItemUnique )
		{
			break;
		}
	}

	if ( i == MAX_BAIT_ITEM_NUMBER )
	{
		return 0;
	}

	SI32 LeftSkillLevel = FishSkillLevel - m_stFishInfo.siNeedFishingSkillLevel;
	
	if ( LeftSkillLevel < 0 )
	{
		return 0;
	}

	return m_stFishInfo.siCatchPercent + LeftSkillLevel * m_stFishInfo.siIncCatchPercentFromSkillLevel;
}

SI32 CFishKind::CatchFish( SI32 FishSkillLevel, SI32 CatchPercent, SI16 SpecialIncreaseSkillLevel )
{
	// 잡을 수 없는 물고기의 확률은 0 이다.
	if ( m_stFishInfo.siCatchPercent == 0 )
	{
		// 확률이 부족해서이지..
		return FISHING_FAIL;
	}

	if ( CatchPercent >= 100000 )
	{
		CatchPercent = 100000;
	}

	// 최대 내구 상승부적 줄 확률 ( 0.0001 % )
	if ( CatchPercent <= FISHING_RARE_ITEM_RATE_FOR_SCROLL )
	{
		//return FISHING_GET_SCROLL;	//[영진] 대박 전리품 폐지
		return FISHING_SUCCESS;

	}

	SI32 LeftSkillLevel = FishSkillLevel - m_stFishInfo.siNeedFishingSkillLevel;

	if ( LeftSkillLevel < 0 )
	{
		return FISHING_SKILL_LEVEL_LOW;
	}

	REAL64 CalcCatchPercent = ( m_stFishInfo.siCatchPercent + LeftSkillLevel * m_stFishInfo.siIncCatchPercentFromSkillLevel + SpecialIncreaseSkillLevel * 0.2 ) * 1000;

	if ( CatchPercent <= CalcCatchPercent )
	{
		if ( CalcCatchPercent >= 400000 )
		{
			return rand()%4 + 1;
		}
		else if ( CalcCatchPercent >= 300000 )
		{
			return rand()%3 + 1;
		}
		else if ( CalcCatchPercent >= 200000 )
		{
			return rand()%2 + 1;
		}
		else
		{
			return FISHING_SUCCESS;			// 1 마리라는 뜻임 ( 물론, 낚시 성공이라는 의미도 내포함 )
		}
	}

	return FISHING_FAIL;
}

SI32 *CFishKind::GetFishingToolItemUniqueListPtr()
{
	return &m_stFishInfo.siFishingToolItemUniqueList[0];
}
