#ifndef _FISHKIND_H_
#define _FISHKIND_H_

#include "FishingBaseInfo.h"

struct stFishInfo
{
	SI32 siNeedFishingSkillLevel;									// 이 물고기를 낚기 위한 최소 기술 레벨

	SI32 siIncFishingSkillExp;										// 기술 경험치 획득

	SI32 siCatchPercent;											// 이 물고기를 낚을 확률
	SI32 siIncCatchPercentFromSkillLevel;							// 스킬 레벨에 따른 성공률 향상

	SI32 siFishingRodAtb;											// 사용되는 낚시도구 종류
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_FISHING_TOOL_NUMBER>		siFishingToolItemUniqueList;
	NSafeTArray<SI32, MAX_BAIT_ITEM_NUMBER>			siBaitItemUniqueList;
	NSafeTArray<SI32, FISHING_PLACE_END>			siFishCreatePlaceList;
	NSafeTArray<RECT, MAX_CREATE_FISH_PLACE_LIST>	FishCreatePlaceRectList;
#else
	SI32 siFishingToolItemUniqueList[ MAX_FISHING_TOOL_NUMBER ];	// 낚시도구 리스트
	SI32 siBaitItemUniqueList[ MAX_BAIT_ITEM_NUMBER ];				// 이 물고기를 낚이 위해서 필요한 미끼 리스트.
	SI32 siFishCreatePlaceList[ FISHING_PLACE_END ];				// 이 물고기가 포획 가능한곳( 강, 연안, 근해, 원해 )
	RECT FishCreatePlaceRectList[MAX_CREATE_FISH_PLACE_LIST];		// 이 물고기가 나타나는 곳 ( 타일 좌표 )
#endif
	
	stFishInfo()
	{
		Init();
	}

	void Init()
	{
		memset ( this, 0, sizeof( stFishInfo ) );
	}

	void Set( stFishInfo *pFishInfo )
	{
		memcpy( this, pFishInfo, sizeof( stFishInfo ) );
	}
};

// 해산물 각각에 대한 정보
// 어디서 ( 강, 해안가, 근해, 원해 )
// 스킬 몇 부터 낚을 수 있는지?
// 몇 %의 확률로 낚이는지... 뭐 그런 것이지..
class CFishKind
{
public:
	CFishKind();
	~CFishKind();

	void Init();

	void SetFishKindInfo
		(
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

	void AnalyzeFishingToolAtb( TCHAR *FishingToolAtb );
	void AnalyzeFishingToolList( TCHAR *FishingToolList );
	void AnalyzeBaitItemUniqueList( TCHAR *BaitItemUniqueList );
	void AnalyzeFishCreatePlaceList( TCHAR *FishCreatePlaceList );
	void AnalyzeFishCreatePlaceRectList( TCHAR *FishCreatePlaceRectList );


	SI32 GetFishItemUnique();
	bool IsSameFishItemUnique( SI32 FishItemUnique );

	stFishInfo *GetFishInfoPtr();
	bool GetFishInfo( stFishInfo *pFishInfo );

	// 어떤 X, Y 좌표에서 어떤 미끼로 이 물고기를 획득할수 있는지?
	bool IsAvailableGainFish( SI32 FishingBaitItemUnique, SI32 PosX, SI32 PosY );
	bool IsAvailableGainFish( SI32 PosX, SI32 PosY );
	bool IsAvailableBait( SI32 BaitItemUnique );

	SI16 GetCatchFishPercent( SI16 FishSkillLevel, SI16 FishingBaitItemUnique );
	SI32 CatchFish( SI32 FishSkillLevel, SI32 CatchPercent, SI16 SpecialIncreaseSkillLevel );

	SI32 *GetFishingToolItemUniqueListPtr();


	stFishInfo	m_stFishInfo;

private:

	SI32		m_siFishItemUnique;
	TCHAR		m_strFishName[ MAX_PLAYER_NAME ];	
};


#endif