#include "DailyQuest2Mgr.h"
#include "DailyQuest2Obj.h"

#include "../Char/KindInfo/kindinfo.h"
#include "DailyQuest2BaseInfo.h"

#include "../Item/ItemCommon/cltItem.h"

#include "..\SMLib\LibList\LibList.h"

#include "..\CommonLogic\CommonLogic.h"
extern cltCommonLogic *pclClient;

// 참여 가능한 퀘스트 개수는 MAX_AVAILABLE_DAILYQUEST2_NUM=20개를 유지한다.

// 나머지는 유저들이 기존에 퀘스트 참여한 개수임( 1명당 퀘스트 1개씩 진행중이라고 보면 됨 )
#define MAX_DAILYQUEST2_OBJ_NUM			MAX_PERSON_NUMBER

CDailyQuest2Mgr::CDailyQuest2Mgr() : CLibListMgr()
{
	m_pDailyQuest2ObjDataList = new CDailyQuest2ObjData[ MAX_AVAILABLE_DAILYQUEST2_NUM ];

	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; ++i )
	{
		m_pDailyQuest2ObjDataList[ i ].m_siIndex = i + 1;
		//SetDailyQuestList( i );
	}
}

CDailyQuest2Mgr::~CDailyQuest2Mgr()
{
	if ( m_pDailyQuest2ObjDataList )
	{
		delete [] m_pDailyQuest2ObjDataList;
		m_pDailyQuest2ObjDataList = NULL;
	}
}

void CDailyQuest2Mgr::Action( clock_t CurrentClock )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; ++i )
	{
		if ( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind == 0 )
		{
		SetDailyQuest2List( i, 0);
		m_PreClock[ i ] = CurrentClock;
		}
		else if ( TABS( CurrentClock - m_PreClock[ i ] ) > 1200000) //600000 )			// 10 분당 1번씩 바꿔줌.// 20분에 한번으로 변경
		{
			m_pDailyQuest2ObjDataList[ i ].Init();
			m_PreClock[ i ] = CurrentClock;
		}
	}
	
	return;
}

CDailyQuest2ObjData* CDailyQuest2Mgr::GetDailyQuest2ListObj( SI16 Index, SI16 Count, SI16 *pListIndex )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; ++i )
	{
		if ( m_pDailyQuest2ObjDataList[ i ].m_siIndex == Index && m_pDailyQuest2ObjDataList[ i ].m_siCount == Count )
		{
			*pListIndex = i;
			return &m_pDailyQuest2ObjDataList[ i ];
		}
	}

	return NULL;
}

void CDailyQuest2Mgr::GetDailyQuest2List( CDailyQuest2ObjData* pDailyQuest2ObjDataList )
{
	memcpy( pDailyQuest2ObjDataList, m_pDailyQuest2ObjDataList, sizeof( CDailyQuest2ObjData ) * MAX_AVAILABLE_DAILYQUEST2_NUM );

	return;
}

//bool CDailyQuest2Mgr::SetDailyQuest2List( SI16 ListIndex, SI16 MonsterKind)
//{
//	// Index 제외하고 모두 초기화 ( Count 는 증가됨. )
//	m_pDailyQuest2ObjDataList[ ListIndex ].Init();
//
//	SI32 siMonsterKind		=	MonsterKind ;	// 몬스터 카인드
//	SI32 siMonsterLevel		=	0 ;
//	SI32 siExp				=	0 ;			// 몬스터 1마리당 얻는 경험치
//	GMONEY siFee			=	0 ;			// 퀘스트 비용 ( 사용 안함 )
//	SI16 siMonsterNum		=	0 ;			// 잡아야할 몬스터 숫자
//	SI32 siLeftTime			=	0 ;			// 퀘스트 수행 시간
//	SI32 siRewardItemUnique	=	0 ;			// 보상 아이템 유니크
//	SI32 siRewardItemNum	=	0 ;			// 보상아이템 갯수
//	SI32 siRewardExp		=	siExp ;		// 보상 경험치 ( 하지만 이곳에서 설정 안한다.여기서의 보상경험치는 한마리 잡을때의 경험치)
//	SI32 siRewardGuildPoint	=	0 ;			// 보상 길드 경험치
//	GMONEY siRewardMoney	=	0 ;			// 보상 금
//	bool bHiddenQuest		=	false ;		// 히든 퀘스트 수행 여부 ( 기본 false )
//	//------------------------------------------------------------------------------------------------------------------
//	// 퀘스트 내용 설정
//	//------------------------------------------------------------------------------------------------------------------
//	if ( ! pclClient->pclKindInfoSet->GetDailyQuest2KindInfo( &siMonsterKind, &siMonsterLevel, &siExp ) )
//	{
//		return false;
//	}
//	// 잡아야할 몬스터 숫자는 80~150 마리
//	siMonsterNum =  ( rand() % MAX_DAILYQUEST2_MONSTER_NUM ) + 80;
//	// 150마리를 넘어서면 150 마리만 잡는다
//	if (siMonsterNum >= MAX_DAILYQUEST2_MONSTER_NUM) siMonsterNum = MAX_DAILYQUEST2_MONSTER_NUM;	
//	// 잡을 몬스터 숫자의 자릿수를 10단위로 만든다
//	siMonsterNum = siMonsterNum - ( siMonsterNum % 10 );
//	// 퀘스트 수행 시간 설정.
//	siLeftTime = ( siExp * 1 / 4 ) * siMonsterNum/2;		// 60 은 그 위치로 가는데 걸리는 시간
//	siLeftTime = siLeftTime - ( siLeftTime % 60 ) + 60;	// 초를 60단위 (분 단위)로 맞추기 위해서...	
//	siLeftTime = siLeftTime - ( siLeftTime % 300 );		// 분을 5분 단위로 맞추기 위해서..
//	//------------------------------------------------------------------------------------------------------------------
//	// 퀘스트 보상 설정
//	//------------------------------------------------------------------------------------------------------------------
//	if ( ! pclClient->pclItemManager->GetDailyQuest2RewardInfo( &siRewardItemUnique, &siRewardItemNum, &siRewardMoney ) )
//	{
//		return false;
//	}
//	//------------------------------------------------------------------------------------------------------------------
//	// 퀘스트 설정
//	//------------------------------------------------------------------------------------------------------------------
//	m_pDailyQuest2ObjDataList[ ListIndex ].Set( siMonsterKind, 
//												siMonsterNum, 
//												siRewardItemUnique, 
//												siRewardItemNum, 
//												siRewardMoney, 
//												siRewardExp, 
//												(GMONEY)siFee, 
//												siLeftTime, 
//												0, 
//												siRewardGuildPoint, 
//												bHiddenQuest);
//	
//	return true;
//}

bool CDailyQuest2Mgr::SetDailyQuest2List( SI16 ListIndex, SI16 MonsterKind)//, BOOL bPCRoomQuest )
{
	// Index 제외하고 모두 초기화 ( Count 는 증가됨. )
	m_pDailyQuest2ObjDataList[ ListIndex ].Init();

	SI32 siMonsterKind = MonsterKind;
	SI32 siAttackPower = 0 ;
	SI32 siExp = 0;				// 몬스터 1마리당 얻는 경험치// SI32->SI64 경험치 오버플로우때문에 수정

	if ( ! pclClient->pclKindInfoSet->GetDailyQuest2KindInfo( &siMonsterKind, &siAttackPower, &siExp ) )
	{
		return false;
	}

	//몬스터 숫자는 80~150마리
	SI16 siMonsterNum = ( rand() % MAX_DAILYQUEST2_MONSTER_NUM ) + 80;//기본 몬스터 갯수
	if (siMonsterNum >= MAX_DAILYQUEST2_MONSTER_NUM) siMonsterNum = MAX_DAILYQUEST2_MONSTER_NUM;	
	siMonsterNum = siMonsterNum - ( siMonsterNum % 10 );				// 마리 수를 10단위로 맞추기 위해서..

	SI32 siRewardItemUnique = 0;
	SI32 siRewardItemNum = 0;
	SI32 siRewardExp = siExp ;
	SI32 siRewardGuildPoint = 0;// 주는 경험치	
	GMONEY siRewardMoney = 0;											// 주는 돈

	if ( ! pclClient->pclItemManager->GetDailyQuest2RewardInfo( siRewardExp * 10, &siRewardItemUnique, &siRewardItemNum, &siRewardMoney ) )
	{
		return false;
	}

	GMONEY siFee = 0 ;/*siRewardExp * 6 / 10*/;

	SI32 siLeftTime = ( siExp * 1 / 4 ) * siMonsterNum/2;		// 60 은 그 위치로 가는데 걸리는 시간
	siLeftTime = siLeftTime - ( siLeftTime % 60 ) + 60;	// 초를 60단위 (분 단위)로 맞추기 위해서...

	siLeftTime = siLeftTime - ( siLeftTime % 300 );		// 분을 5분 단위로 맞추기 위해서..

	siRewardGuildPoint = 0;

	bool bHiddenQuest = false;

	m_pDailyQuest2ObjDataList[ ListIndex ].Set( siMonsterKind, siMonsterNum, siRewardItemUnique, siRewardItemNum, siRewardMoney, siRewardExp, (GMONEY)siFee, siLeftTime, 0, siRewardGuildPoint, bHiddenQuest);//, bPCRoomQuest );

	return true;
}
void CDailyQuest2Mgr::InitQuest2ListData( SI16 ListIndex, SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, 
										 SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, 
										 GMONEY Fee, SI32 LeftTime, bool bHiddenQuest)
{
	if (pclClient->pclKindInfoSet->pclKI[MonsterKind] != NULL)
	{
		switch (pclClient->siServiceArea)
		{
		case ConstServiceArea_Korea :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_KOREA))
					return;
			}
			break;
		case ConstServiceArea_Japan :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_JAPAN))
					return;

			}
			break;
		case ConstServiceArea_China :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_CHINA))
					return;
			}
			break;
		case ConstServiceArea_English :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_ENGLISH))
					return;
			}
			break;
		case ConstServiceArea_Taiwan :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_TAIWAN))
					return;
			}
			break;
		case ConstServiceArea_USA :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_USA))
					return;
			}
			break;
		case ConstServiceArea_NHNChina :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_NHNCHINA))
					return;
			}
			break;
		case ConstServiceArea_EUROPE :
			{
				if (!(pclClient->pclKindInfoSet->pclKI[MonsterKind]->siServiceAreaAtb & SERVICEAREA_ATB_EUROPE))
					return;
			}
			break;

		}
	}
	

	if ( m_pDailyQuest2ObjDataList[ ListIndex ].m_siMonsterKind == MonsterKind &&
		m_pDailyQuest2ObjDataList[ ListIndex ].m_siMonsterNum == MonsterNum &&
		m_pDailyQuest2ObjDataList[ ListIndex ].m_siRewardItemUnique == RewardItemUnique &&
		m_pDailyQuest2ObjDataList[ ListIndex ].m_siLeftTime == LeftTime)
	{
		m_pDailyQuest2ObjDataList[ ListIndex ].Init();
	}	
	while ( ! SetDailyQuest2List( ListIndex, MonsterKind));
	m_PreClock[ ListIndex ] = pclClient->CurrentClock;
	
	return;
}
