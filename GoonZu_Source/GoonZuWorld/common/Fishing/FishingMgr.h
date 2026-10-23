#ifndef _FISHINGMGR_H_
#define _FISHINGMGR_H_

#include "FishingBaseInfo.h"

class CFishKind;

class DBToolInfo;

class CFishingMgr
{
public:
	CFishingMgr();
	~CFishingMgr();

	void Init();

	void LoadFishKindInfo( TCHAR *FileName );
	void Add(
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
		);

	SI16 GetRefFromFishItemUnique( SI32 FishItemUnique );
	CFishKind *GetFishKindFromRef( SI16 Ref );

	bool IsAvailableFishing( SI32 id, SI32 FishKindListRef );
	bool IsAvailableFishingAndGetFishingToolList( SI32 id, SI32 FishBaitItemUnique, DBToolInfo *pFishingToolList );
	
	bool GetAvailableGainFishKindListRef( SI32 FishingBaitItemUnique, SI32 XPos, SI32 YPos, SI32 FishSkillLevel, SI32 *pAvailableGainFishKindListRef, SI16 *pAvailableGainFishKindListRefCounter );

	SI32 *GetFishingToolItemUniqueListPtr( SI16 FishKindListRef );
		
	SI16 IsAvailableBait( SI32 FishingBaitItemUnique );
	
	SI16 GetFishingKindInPos( SI16 SkillLevel, SI16 FishingBaitItemUnique, SI32 PosX, SI32 PosY, SI16 *FishKindList, SI16 *FishCatchPercentList );

#ifdef _SAFE_MEMORY
	NSafeTArray<CFishKind*, MAX_FISHKIND_NUMBER>		m_pFishKindList;
#else
	CFishKind	*m_pFishKindList[ MAX_FISHKIND_NUMBER ];
#endif

private:

	clock_t		m_PreClock;

};

#endif