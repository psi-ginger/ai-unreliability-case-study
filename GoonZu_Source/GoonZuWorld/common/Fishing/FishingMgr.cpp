#include "FishingMgr.h"

#include "FishKind.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../Char/CharCommon/Char-Common.h"

#include "../Order/order.h"

#include "../../CommonLogic/Msg/MsgType-Fishing.h"
#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "../../CommonLogic/MsgRval-Define.h"
#include "../../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Fishing.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "CommonLogic.h"
extern cltCommonLogic* pclClient;

CFishingMgr::CFishingMgr()
{
	m_PreClock = 0;

	for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
	{
		m_pFishKindList[ i ]  = NULL;
	}

	if( pclClient->siServiceArea == ConstServiceArea_Korea )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindK.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_English )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindE.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_China )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindC.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindJ.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindT.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_USA  )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindU.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindC.txt") );
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
		LoadFishKindInfo(  TEXT("FishKind\\FishKindG.txt") );

}

CFishingMgr::~CFishingMgr()
{
	for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
	{
		if ( m_pFishKindList[ i ]  != NULL )
		{
			delete m_pFishKindList[ i ];
			m_pFishKindList[ i ] = NULL;
		}
	}
}

void CFishingMgr::Init()
{
	return;
}

void CFishingMgr::LoadFishKindInfo( TCHAR *FileName )
{
	// 후에는 파일을 읽어서 처리하는 부분이 들어가야지...

	FILE *fp = _tfopen( FileName, TEXT("rt") );

	if ( fp == NULL )
	{
		return;
	}

	TCHAR buffer[ 2048 ] = TEXT("");

	for ( SI16 k = 0; k < 6; ++k )
	{
		_fgetts(buffer, 2048, fp);
	}

	SI32 FishItemUnique;	
	TCHAR szFishName[ MAX_PLAYER_NAME ];

	SI32 NeedFishingSkillLevel;
	SI32 IncFishingSkillExp;

	SI32 CatchPercent;
	SI32 IncCatchPercentFromSkillLevel;

	TCHAR szFishingToolAtb[ 128 ];
	TCHAR szFishingToolList[ 1024 ];
	SI32 BaitItemUnique;
	TCHAR szFishCreatePlaceList[ 128 ];
	TCHAR szFishCreatePlaceRectList[ 1024 ];

	SI16 Count = 0;

	while( _fgetts(buffer, 2048, fp) != NULL)
	{
		if ( pclClient->IsWhereServiceArea( ConstServiceArea_Japan 
											| ConstServiceArea_China
											| ConstServiceArea_English
											| ConstServiceArea_NHNChina
											| ConstServiceArea_Taiwan
											| ConstServiceArea_USA
											| ConstServiceArea_EUROPE )
		)
		{
			ParseText( buffer );

			FishItemUnique = _tstoi( tokens[0] );
			NeedFishingSkillLevel = _tstoi( tokens[1] );
			IncFishingSkillExp = _tstoi( tokens[2] );
			CatchPercent = _tstoi( tokens[3] );
			IncCatchPercentFromSkillLevel = _tstoi( tokens[4] );

			MStrCpy( szFishName, tokens[5], MAX_PLAYER_NAME);
			MStrCpy( szFishingToolAtb, tokens[6], 128);
			MStrCpy( szFishingToolList, tokens[7], 256);
			BaitItemUnique = _tstoi( tokens[8] );
			MStrCpy( szFishCreatePlaceList, tokens[9], 128);
			MStrCpy( szFishCreatePlaceRectList, tokens[10], 256);
		}
		else
		{
			_stscanf(buffer, TEXT("%d %d %d %d %d %s %s %s %d %s %s"),
				&FishItemUnique,
				&NeedFishingSkillLevel,
				&IncFishingSkillExp,
				&CatchPercent,
				&IncCatchPercentFromSkillLevel,
				szFishName,
				szFishingToolAtb,
				szFishingToolList,
				&BaitItemUnique,
				szFishCreatePlaceList,
				szFishCreatePlaceRectList
				);
		}

	/*	_stscanf(buffer, TEXT("%d %d %d %d %d %s %s %s %s %s %s"),
			&FishItemUnique,
			&NeedFishingSkillLevel,
			&IncFishingSkillExp,
			&CatchPercent,
			&IncCatchPercentFromSkillLevel,
			szFishName,
			szFishingToolAtb,
			szFishingToolList,
			szBaitItemUniqueList,
			szFishCreatePlaceList,
			szFishCreatePlaceRectList
			);*/
		Add(
			Count++,
			FishItemUnique,
			NeedFishingSkillLevel,
			IncFishingSkillExp,
			CatchPercent, 
			IncCatchPercentFromSkillLevel,
			szFishName,
			szFishingToolAtb,
			szFishingToolList,
			BaitItemUnique,
			szFishCreatePlaceList,
			szFishCreatePlaceRectList
			);
	}

	fclose( fp );

	return;
}

void CFishingMgr::Add(
					  SI16 Index,
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
	if ( m_pFishKindList[ Index ] != NULL )
	{
		return;
	}

	m_pFishKindList[ Index ] = new CFishKind();
	m_pFishKindList[ Index ]->SetFishKindInfo
		(
		FishItemUnique,
		NeedFishingSkillLevel,
		IncFishingSkillExp,
		CatchPercent,
		IncCatchPercentFromSkillLevel,
		FishName,
		FishingToolAtb,
		FishingToolList,
		BaitItemUnique,
		FishCreatePlaceList,
		FishCreatePlaceRectList
		);
}

SI16 CFishingMgr::GetRefFromFishItemUnique( SI32 FishItemUnique )
{
	for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
	{
		if ( m_pFishKindList[ i ] == NULL )
		{
			continue;
		}

		if ( m_pFishKindList[ i ]->GetFishItemUnique() == FishItemUnique )
		{
			return i;
		}
	}
	
	return -1;
}

CFishKind *CFishingMgr::GetFishKindFromRef( SI16 Ref )
{
	if ( m_pFishKindList[ Ref ] == NULL )
	{
		return NULL;
	}

	return m_pFishKindList[ Ref ];
}

bool CFishingMgr::IsAvailableFishing( SI32 id, SI32 FishKindListRef )
{
	if ( id < 1 || pclClient->pclCM->IsValidID( id ) == FALSE )
	{
		return false;
	}

	SI32 *FishingTollItemUniqueListPtr = m_pFishKindList[ FishKindListRef ]->GetFishingToolItemUniqueListPtr();

	SI16 ItemPos = -1;
	SI32 Durability = 0;

	SI32 FishClassification = 0;

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( FishingTollItemUniqueListPtr[ i ] > 0 )
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(FishingTollItemUniqueListPtr[ i ]);
			if( ref <= 0 )
				return false;
			
			if ( FishClassification & pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification )
			{
				// 이 종류의 아이템은 추가했다.
				continue;
			}

			FishClassification = FishClassification | pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification;

			ItemPos = -1;
			Durability = 0;

			//ItemPos = pclClient->pclCM->CR[id]->pclCI->clCharItem.GetItemPos( FishingTollItemUniqueListPtr[ i ] );
			if ( pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[id]->ActiveWeapon ].siUnique == FishingTollItemUniqueListPtr[ i ] )
			{
				ItemPos = pclClient->pclCM->CR[id]->ActiveWeapon;
			}
			else
			{
				ItemPos = -1;
			}

			if ( ItemPos < 0 )
			{
				// 도구가 없네.
				((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTEXIST, FishingTollItemUniqueListPtr[ i ], 0, pclClient->pclCM->CR[id]->GetCharUnique());
				return false;
			}

			Durability = pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ].GetDurability(  );

	/*		if ( FISHING_TOOL_DURABILITY_DECREASE > Durability )
			{
				// 어떤 어구의 내구도가 낮아서 낚시를 할 수 없습니다.
				((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTENOUGHDURABILITY, FishingTollItemUniqueListPtr[ i ], 0, pclClient->pclCM->CR[id]->GetCharUnique());
				return false;
			}*/
		}
	}

	return true;
}

//((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_NOPROPERFISHAREA, 0, 0, GetCharUnique());
bool CFishingMgr::IsAvailableFishingAndGetFishingToolList( SI32 id, SI32 FishBaitItemUnique, DBToolInfo *pFishingToolList )
{
	if ( id < 1 || pclClient->pclCM->IsValidID( id ) == FALSE )
	{
		return false;
	}

	for ( SI16 j = 0; j < MAX_FISHKIND_NUMBER; ++j )
	{
		if ( m_pFishKindList[ j ] != NULL )
		{			
			if ( m_pFishKindList[ j ]->IsAvailableBait( FishBaitItemUnique ) )
			{
				break;
			}
		}
	}

	if ( j == MAX_FISHKIND_NUMBER )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHNOTBAITITEM, 0, 0, pclClient->pclCM->CR[id]->GetCharUnique());
		return false;
	}

	SI32 FishKindListRef = j;

	if ( ! IsAvailableFishing( id, FishKindListRef ) )
	{
		return false;
	}
	
	SI32 *FishingTollItemUniqueListPtr = m_pFishKindList[ FishKindListRef ]->GetFishingToolItemUniqueListPtr();

	SI16 ItemPos = -1;
	SI32 Durability = 0;

	SI16 FishingToolCounter = 0;

	cltItem clTempItem;	
	SI32 FishClassification = 0;
	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( FishingTollItemUniqueListPtr[ i ] > 0 )
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(FishingTollItemUniqueListPtr[ i ]);
			if( ref <= 0 )
				return false;
			
			if ( FishClassification & pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification )
			{
				// 이 종류의 아이템은 추가했다.
				continue;
			}

			FishClassification = FishClassification | pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification;

			ItemPos = -1;
			Durability = 0;

			//ItemPos = pclClient->pclCM->CR[id]->pclCI->clCharItem.GetItemPos( FishingTollItemUniqueListPtr[ i ] );
			if ( pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[id]->ActiveWeapon ].siUnique == FishingTollItemUniqueListPtr[ i ] )
			{
				ItemPos = pclClient->pclCM->CR[id]->ActiveWeapon;
			}
			else
			{
				ItemPos = -1;
			}
			
			if ( ItemPos < 0 )
			{
				return false;
			}

			clTempItem.Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ] );
			
		/*	if ( pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ].GetDurability(  ) <= FISHING_TOOL_DURABILITY_DECREASE )
			{
				((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTENOUGHDURABILITY, clTempItem.siUnique, 0, pclClient->pclCM->CR[id]->GetCharUnique());
				return false;
			}

			clTempItem.DecreaseDurability( pclClient->pclItemManager, FISHING_TOOL_DURABILITY_DECREASE );
		*/
			if ( clTempItem.siItemNum == 0 )
			{
				clTempItem.siUnique = pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ ItemPos ].siUnique;
			}

			pFishingToolList[ FishingToolCounter ++ ].Set( ItemPos, &clTempItem );
		}
	}

	return true;
}


// FishSkillLevel이 10000이면, 스킬은 체크하지 않는다. - by LEEKH 2008-09-03
bool CFishingMgr::GetAvailableGainFishKindListRef( SI32 FishingBaitItemUnique, SI32 XPos, SI32 YPos, SI32 FishSkillLevel, SI32 *pAvailableGainFishKindListRef, SI16 *pAvailableGainFishKindListRefCounter )
{
	SI16 AvailableGainFishKindListRefCounter = 0;

	for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
	{
		if ( m_pFishKindList[ i ] != NULL )
		{
			// 어떤 미끼로 그 좌표에서 낚시가 가능한지...
			if ( m_pFishKindList[ i ]->IsAvailableGainFish( FishingBaitItemUnique, XPos, YPos ) )
			{
				// 레벨은 체크 하지 않는다 - by LEEKH 2008-09-03
				if ( FishSkillLevel == 10000 )
				{
					pAvailableGainFishKindListRef[ AvailableGainFishKindListRefCounter++ ] = i;
				}
				else
				{
					if ( FishSkillLevel >= m_pFishKindList[ i ]->m_stFishInfo.siNeedFishingSkillLevel )
					{
						pAvailableGainFishKindListRef[ AvailableGainFishKindListRefCounter++ ] = i;
					}
				}
				if ( AvailableGainFishKindListRefCounter >= 30 )
				{
					break;
				}
			}
		}
	}

	// 그 영역에 물고기 없음.
	if ( AvailableGainFishKindListRefCounter < 1 )
	{
		return false;
	}

	*pAvailableGainFishKindListRefCounter = AvailableGainFishKindListRefCounter;

	return true;
}

SI32 *CFishingMgr::GetFishingToolItemUniqueListPtr( SI16 FishKindListRef )
{
	return m_pFishKindList[ FishKindListRef ]->GetFishingToolItemUniqueListPtr();
}

SI16 CFishingMgr::IsAvailableBait( SI32 FishingBaitItemUnique )
{
	for ( SI16 j = 0; j < MAX_FISHKIND_NUMBER; ++j )
	{
		if ( m_pFishKindList[ j ] != NULL )
		{			
			if ( m_pFishKindList[ j ]->IsAvailableBait( FishingBaitItemUnique ) )
			{
				return j;
			}
		}
	}

	return -1;
}

// 이 지점에서 획득 가능한 물고기와 성공 확률을 얻어옮
//SI16 CFishingMgr::GetFishingKindInPos( SI16 SkillLevel, SI16 FishingBaitItemUnique, SI32 PosX, SI32 PosY, SI16 **FishKindList, SI16 **FishCatchPercentList )
SI16 CFishingMgr::GetFishingKindInPos( SI16 SkillLevel, SI16 FishingBaitItemUnique, SI32 PosX, SI32 PosY, SI16 *FishKindList, SI16 *FishCatchPercentList )
{
//	(*FishKindList) = new SI16[ MAX_FISHKIND_NUMBER ];
//	(*FishCatchPercentList) = new SI16[ MAX_FISHKIND_NUMBER ];

	memset( FishKindList, 0, sizeof( SI16 ) * MAX_FISHKIND_NUMBER );
	memset( FishCatchPercentList, 0, sizeof( SI16 ) * MAX_FISHKIND_NUMBER );

	SI16 Counter = 0;
	for ( SI16 i = 0; i < MAX_FISHKIND_NUMBER; ++i )
	{
		if ( m_pFishKindList[ i ] != NULL )
		{
			if ( m_pFishKindList[ i ]->IsAvailableGainFish( PosX, PosY ) )
			{
				FishKindList[ Counter ] = m_pFishKindList[ i ]->GetFishItemUnique();
				FishCatchPercentList[ Counter ] = m_pFishKindList[ i ]->GetCatchFishPercent( SkillLevel, FishingBaitItemUnique );
				++Counter;
			}
		}
	}

	return Counter;
}
