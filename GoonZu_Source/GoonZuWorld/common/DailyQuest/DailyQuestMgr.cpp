#include "DailyQuestMgr.h"
#include "DailyQuestObj.h"

#include "../Char/KindInfo/kindinfo.h"
#include "DailyQuestBaseInfo.h"

#include "../Item/ItemCommon/cltItem.h"

#include "..\SMLib\LibList\LibList.h"

#include "..\CommonLogic\CommonLogic.h"
extern cltCommonLogic *pclClient;

// 참여 가능한 퀘스트 개수는 MAX_AVAILABLE_DAILYQUEST_NUM=20개를 유지한다.

// 나머지는 유저들이 기존에 퀘스트 참여한 개수임( 1명당 퀘스트 1개씩 진행중이라고 보면 됨 )
#define MAX_DAILYQUEST_OBJ_NUM			MAX_PERSON_NUMBER

CDailyQuestMgr::CDailyQuestMgr() : CLibListMgr()
{
	m_pDailyQuestObjDataList = new CDailyQuestObjData[ MAX_AVAILABLE_DAILYQUEST_NUM ];

	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
	{
		m_pDailyQuestObjDataList[ i ].m_siIndex = i + 1;
		//SetDailyQuestList( i );
	}
}

CDailyQuestMgr::~CDailyQuestMgr()
{
	if ( m_pDailyQuestObjDataList )
	{
		delete [] m_pDailyQuestObjDataList;
		m_pDailyQuestObjDataList = NULL;
	}
}

void CDailyQuestMgr::Action( clock_t CurrentClock )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
	{
		if ( m_pDailyQuestObjDataList[ i ].m_siMonsterKind == 0 )
		{
			//int pcroom = 0;
//			for( int j = 0; j < MAX_AVAILABLE_DAILYQUEST_NUM; ++j ) {
//				if( m_pDailyQuestObjDataList[ j ].m_bPCRoomQuest ) pcroom++;
//			}

//			if( pcroom < 5 ) {
//				SetDailyQuestList( i, 0, TRUE );
//			} else {
				SetDailyQuestList( i, 0);//, FALSE );
//			}
			m_PreClock[ i ] = CurrentClock;
		}
		else if ( TABS( CurrentClock - m_PreClock[ i ] ) > 600000 )			// 10 분당 1번씩 바꿔줌.
		{
			m_pDailyQuestObjDataList[ i ].Init();
			m_PreClock[ i ] = CurrentClock;
		}
	}
	
	return;
}

CDailyQuestObjData*	CDailyQuestMgr::GetDailyQuestListObj( SI16 Index, SI16 Count, SI16 *pListIndex )
{
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
	{
		if ( m_pDailyQuestObjDataList[ i ].m_siIndex == Index && m_pDailyQuestObjDataList[ i ].m_siCount == Count )
		{
			*pListIndex = i;
			return &m_pDailyQuestObjDataList[ i ];
		}
	}

	return NULL;
}

void CDailyQuestMgr::GetDailyQuestList( CDailyQuestObjData* pDailyQuestObjDataList )
{
	memcpy( pDailyQuestObjDataList, m_pDailyQuestObjDataList, sizeof( CDailyQuestObjData ) * MAX_AVAILABLE_DAILYQUEST_NUM );

	return;
}

bool CDailyQuestMgr::SetDailyQuestList( SI16 ListIndex, SI16 MonsterKind)//, BOOL bPCRoomQuest )
{
	// Index 제외하고 모두 초기화 ( Count 는 증가됨. )
	m_pDailyQuestObjDataList[ ListIndex ].Init();

	//m_pDailyQuestObjDataList[ ListIndex ].m_siMonsterKind

	SI32 siMonsterKind = MonsterKind;
	SI32 siAttackPower = 0 ;
	SI32 siExp = 0;				// 몬스터 1마리당 얻는 경험치

	if ( ! pclClient->pclKindInfoSet->GetDailyQuestKindInfo( &siMonsterKind, &siAttackPower, &siExp ) )
	{
		return false;
	}

	// 최소 10 마리
	SI16 siMonsterNum = ( rand() % MAX_DAILYQUEST_MONSTER_NUM ) + 20;

	siMonsterNum = siMonsterNum - ( siMonsterNum % 10 );				// 마리 수를 10단위로 맞추기 위해서..
	
	SI32 siRewardItemUnique = 0;
	SI32 siRewardItemNum = 0;
	SI32 siRewardExp = siExp * siMonsterNum * 0.7;						// 주는 경험치는 일단 기본 몬스터 경험치만 넣어 준다.
																		// 경험치는 차후 퀘스트를 받을때 변한다.
	GMONEY siRewardMoney = 0;											// 주는 돈

	siRewardExp = siRewardExp - ( siRewardExp % 100 ) + 100;
	if ( siRewardExp < 100 )
	{
		siRewardExp = 100;
	}

	if ( ! pclClient->pclItemManager->GetDailyQuestRewardInfo( siRewardExp * 10, &siRewardItemUnique, &siRewardItemNum, &siRewardMoney ) )
	{
		return false;
	}

	//siRewardItemNum = siRewardItemNum - ( siRewardItemNum % 10 ) + 10;		// 개수를 10 단위로 맞추기 위해서...

	GMONEY siFee = siRewardExp * 6 / 10;

	siFee = siFee - ( siFee % 100 ) + 100;

	if ( siFee < 100 )
	{
		siFee = 100;
	}

	//SI32 siLeftTime = ( siExp / 3 + 10 ) * siMonsterNum;
	SI32 siLeftTime = ( siExp * 1 / 2 ) * siMonsterNum + 60;		// 60 은 그 위치로 가는데 걸리는 시간
	siLeftTime = siLeftTime - ( siLeftTime % 60 ) + 60;	// 초를 60단위 (분 단위)로 맞추기 위해서...
	
	siLeftTime = siLeftTime - ( siLeftTime % 300 );		// 분을 5분 단위로 맞추기 위해서.. (나머지 시간은 날림, 시간이 좀 넉넉한거 같아서... )

	if ( siLeftTime < 300 )
	{
		siLeftTime = 300;
	}

//	if( bPCRoomQuest ) siRewardItemNum = siRewardItemNum * 3;
	
	m_pDailyQuestObjDataList[ ListIndex ].Set( siMonsterKind, siMonsterNum, siRewardItemUnique, siRewardItemNum, siRewardMoney, siRewardExp, (GMONEY)siFee, siLeftTime, 0);//, bPCRoomQuest );
	
	return true;
}

void CDailyQuestMgr::InitQuestListData( SI16 ListIndex, SI16 MonsterKind, SI16 MonsterNum, 
									   SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, 
									   SI32 RewardExp, GMONEY Fee, SI32 LeftTime)//, BOOL bPCRoomQuest )
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
	

	if ( m_pDailyQuestObjDataList[ ListIndex ].m_siMonsterKind == MonsterKind &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siMonsterNum == MonsterNum &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siRewardItemUnique == RewardItemUnique &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siRewardItemNum == RewardItemNum &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siRewardExp == RewardExp &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siRewardMoney == RewardMoney &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siFee == Fee &&
		m_pDailyQuestObjDataList[ ListIndex ].m_siLeftTime == LeftTime )
//		m_pDailyQuestObjDataList[ ListIndex ].m_bPCRoomQuest == bPCRoomQuest )
	{
		m_pDailyQuestObjDataList[ ListIndex ].Init();
	}
	
	while ( ! SetDailyQuestList( ListIndex, MonsterKind));//, bPCRoomQuest ) );
	m_PreClock[ ListIndex ] = pclClient->CurrentClock;
	
	return;
}
