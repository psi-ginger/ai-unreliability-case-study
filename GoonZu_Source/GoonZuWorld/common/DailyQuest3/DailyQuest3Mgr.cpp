#include "DailyQuest3Mgr.h"
#include "DailyQuest3Obj.h"

#include "../Item/ItemCommon/cltItem.h"

#include "..\SMLib\LibList\LibList.h"

#include "..\CommonLogic\CommonLogic.h"
extern cltCommonLogic *pclClient;

#define MAX_DAILYQUEST3_MONSTER_NUM			150			// DailyQuest 몬스터 최대수
// 참여 가능한 퀘스트 개수는 MAX_AVAILABLE_DAILYQUEST2_NUM=20개를 유지한다.
// 나머지는 유저들이 기존에 퀘스트 참여한 개수임( 1명당 퀘스트 1개씩 진행중이라고 보면 됨 )
#define MAX_DAILYQUEST3_OBJ_NUM			MAX_PERSON_NUMBER

CDailyQuest3Mgr::CDailyQuest3Mgr() : CLibListMgr()
{
	m_pDailyQuest3ObjDataList = new CDailyQuest3ObjData[ MAX_AVAILABLE_DAILYQUEST3_NUM ];

	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST3_NUM; ++i )
	{
		m_pDailyQuest3ObjDataList[ i ].m_siIndex = i + 1;
	}
}

CDailyQuest3Mgr::~CDailyQuest3Mgr()
{
	if ( m_pDailyQuest3ObjDataList )
	{
		delete [] m_pDailyQuest3ObjDataList;
		m_pDailyQuest3ObjDataList = NULL;
	}
}

void CDailyQuest3Mgr::Action( clock_t CurrentClock )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST3_NUM; ++i )
	{
		if ( m_pDailyQuest3ObjDataList[ i ].m_siItemUnique == 0 )
		{
			SetDailyQuest3List( i, 0);
			m_PreClock[ i ] = CurrentClock;
		}
		else if ( TABS( CurrentClock - m_PreClock[ i ] ) >120000)	// 10 분당 1번씩 바꿔줌.// 20분에 한번으로 변경
		{
			m_pDailyQuest3ObjDataList[ i ].Init();
			m_PreClock[ i ] = CurrentClock;
		}
	}
	
	return;
}

CDailyQuest3ObjData* CDailyQuest3Mgr::GetDailyQuest3ListObj( SI32 Index, SI32 Count, SI32 *pListIndex )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST3_NUM; ++i )
	{
		if ( m_pDailyQuest3ObjDataList[ i ].m_siIndex == Index && m_pDailyQuest3ObjDataList[ i ].m_siCount == Count )
		{
			*pListIndex = i;
			return &m_pDailyQuest3ObjDataList[ i ];
		}
	}

	return NULL;
}

void CDailyQuest3Mgr::GetDailyQuest3List( CDailyQuest3ObjData* pDailyQuest3ObjDataList )
{
	memcpy( pDailyQuest3ObjDataList, m_pDailyQuest3ObjDataList, sizeof( CDailyQuest3ObjData ) * MAX_AVAILABLE_DAILYQUEST3_NUM );

	return;
}
//---------------------------------------------------------------------------------------------------------------------------------
// 죠엘 퀘스트 ( DailyQuest3 ) 설정.
bool CDailyQuest3Mgr::SetDailyQuest3List( SI32 ListIndex, SI16 Itemunique)
{
	// Index 제외하고 모두 초기화 ( Count 는 증가됨. )
	m_pDailyQuest3ObjDataList[ ListIndex ].Init();
	//---------------------------------------------------------------------------------------------------------------------------------
	// 필요 변수 선연
	//---------------------------------------------------------------------------------------------------------------------------------
	bool bHiddenQuest		=	false ;		//	히든 퀘스트 여부 (기본 false)
	SI16 siItemUnique		=	Itemunique ;//	만들 아이템 유니크
	SI16 siMakeItemNum		=	0 ;			//	만들어야하는 아이템 숫자
	SI16 siMakedItemNum		=	0 ;			//	만든 아이템 숫자
	SI16 siRewardItemUnique	=	0 ;			//	보상 아이템 유니크
	SI16 siRewardItemNum	=	0 ;			//	보상 아이템 갯수
	SI32 siRewardExp		=	0 ;			//	보상 경험치
	SI32 SkillLevelLimit	=	0 ;			//	해당 아이템의 제작기술 레벨
	SI32 siLeftTime			=	0 ;			//	퀘스트 아이템 제작 시간 제한
	
	SI16 SpecialMat			=	0 ;			//	대박 제료 확률.
	GMONEY siFee			=	0 ;			//	퀘스트 비용이지만 지금은 아이템을 사용하는 방식. 현제는 쓰지 않는다.
	//---------------------------------------------------------------------------------------------------------------------------------
	// 만들 아이템을 설정
	//---------------------------------------------------------------------------------------------------------------------------------
	if ( ! pclClient->pclItemManager->GetDailyQuest3KindInfo( &siItemUnique, &siMakeItemNum ) )
	{
		return false;
	}
    // 제작 아이템의 Ref 와 기술 레벨을 구한다.
	SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
	if (Ref <= 0) return false ;
	SkillLevelLimit = pclClient->pclItemManager->pclItemInfo[Ref]->clSkillInfo.siSkillLevel; // 제작기술Lev
	if (SkillLevelLimit <= 0) return false;
	// 만들 시간 설정.
	siLeftTime = SkillLevelLimit * siMakeItemNum *30 ;
	siLeftTime = siLeftTime - ( siLeftTime % 60 ) + 60;	// 초를 60단위 (분 단위)로 맞추기 위해서...
	siLeftTime = siLeftTime - ( siLeftTime % 300 );		// 분을 5분 단위로 맞추기 위해서..
	
	//---------------------------------------------------------------------------------------------------------------------------------
	// 보상 아이템 설정
	//---------------------------------------------------------------------------------------------------------------------------------
	if ( ! pclClient->pclItemManager->GetDailyQuest3RewardInfo( &siRewardExp, siMakeItemNum, &siRewardItemUnique, &siRewardItemNum, SkillLevelLimit ) )
	{
		return false;
	}
	//---------------------------------------------------------------------------------------------------------------------------------
	// 일정 확률로 레어 제료를 준다.
	//---------------------------------------------------------------------------------------------------------------------------------
	// 5프로의 확률로 레어 재료를 준다.
	//---------------------------------------------------------------------------------------------------------------------------------
	SpecialMat = rand() % 100;
	if( SpecialMat == 45 ||SpecialMat == 13 || SpecialMat == 89 || SpecialMat == 65 ||	SpecialMat == 49 ){
		SI16 specialitemnum = rand()%11+5;
        switch(rand()%3) {			 
			case 0:
				siRewardItemUnique = ITEMUNIQUE(5380);
				siRewardItemNum = specialitemnum;
				if (siRewardItemNum > 10) {
					siRewardItemNum = 10;
				}
				break;
			case 1:
				siRewardItemUnique = ITEMUNIQUE(7175);
				siRewardItemNum = specialitemnum;
				if (siRewardItemNum > 10) {
					siRewardItemNum = 10;
				}
				break;
			case 2:
				siRewardItemUnique = ITEMUNIQUE(6140);
				siRewardItemNum = specialitemnum;
				if (siRewardItemNum > 10) {
					siRewardItemNum = 10;
				}
				break;
			}
	}
	// 레어 재료들 당분간 나오지 않도록 합니다.
	////---------------------------------------------------------------------------------------------------------------------------------
	//// 3 프로 확률로 천사의 노끈
	////---------------------------------------------------------------------------------------------------------------------------------
	//if ( SpecialMat == 1 || SpecialMat == 19 || SpecialMat == 72 ) {
	//	siRewardItemUnique = ITEMUNIQUE(8064);
	//	siRewardItemNum = rand()%4+2;
	//	if (siRewardItemNum > 5) {
	//		siRewardItemNum = 5;
	//	}
	//}
	////---------------------------------------------------------------------------------------------------------------------------------
	//// 1프로 확률로 악마의 노끈
	////---------------------------------------------------------------------------------------------------------------------------------
	//if ( SpecialMat == 73) { 
	//	siRewardItemUnique = ITEMUNIQUE(8066);
	//	siRewardItemNum = rand()%4+2;
	//	if (siRewardItemNum > 3) {
	//		siRewardItemNum = 3;
	//	}
	//}

	//---------------------------------------------------------------------------------------------------------------------------------
	// m_pDailyQuest3ObjDataList 셋팅
	//---------------------------------------------------------------------------------------------------------------------------------
	m_pDailyQuest3ObjDataList[ ListIndex ].Set(siItemUnique, siMakeItemNum, siRewardItemUnique,siRewardItemNum, siRewardExp,
		 siFee, siLeftTime, siMakedItemNum, bHiddenQuest);
	
	return true;
}

void CDailyQuest3Mgr::InitQuest3ListData( SI32 ListIndex, SI16 ItemUnique, SI16 siItemNum, SI16 RewardItemUnique, 
										 SI16 RewardItemNum, GMONEY Fee, SI32 LeftTime,SI32 RewardExp, bool bHiddenQuest){

	SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( ItemUnique );

	if (pclClient->pclItemManager->pclItemInfo[Ref] == NULL) return;
	
	if ( m_pDailyQuest3ObjDataList[ ListIndex ].m_siItemUnique  == ItemUnique &&
		m_pDailyQuest3ObjDataList[ ListIndex ].m_siItemNum  == siItemNum &&
		m_pDailyQuest3ObjDataList[ ListIndex ].m_siRewardExp  == RewardExp &&
		m_pDailyQuest3ObjDataList[ ListIndex ].m_siRewardItemNum== RewardItemNum &&
		m_pDailyQuest3ObjDataList[ ListIndex ].m_siRewardItemUnique  == RewardItemUnique)
	{
		m_pDailyQuest3ObjDataList[ ListIndex ].Init();
	}
	
	while ( ! SetDailyQuest3List( ListIndex, ItemUnique));
	m_PreClock[ ListIndex ] = pclClient->CurrentClock;
	
	return;
}
