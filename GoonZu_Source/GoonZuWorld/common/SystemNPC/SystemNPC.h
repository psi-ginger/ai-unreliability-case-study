
#ifndef _SYSTEMNPC_H
#define _SYSTEMNPC_H

#include <Directives.h>

#define MAX_SYSTEM_NPC_NUMBER		90

//-------------------------------------
// 시스템 NPC 메시지. 
//-------------------------------------

#define SYSTEMNPC_SUCCESS_BUY								1	// NPC에게 구매 성공. 
#define SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY		2	// 돈 부족으로 구입 실패
#define	SYSTEMNPC_SUCCESS_SELL								3	// NPC에게 판매 성공
#define SYSTEMNPC_FAIL_SELL_BULLETITEM						4	// NPC에게 탄환류 아이템 판매할수 없음
#define SYSTEMNPC_FAIL_SELL_ITEMMALLITEM					5	// NPC에게 아이템몰 아이템 판매할수 없음
#define SYSTEMNPC_FAIL_SELL_QUESTITEM						6	// NPC에게 아이템몰 아이템 판매할수 없음
#define SYSTEMNPC_SUCCESS_BUY_ARROW							7	// NPC에게 화살 구매 성공.
#define SYSTEMNPC_SUCCESS_BUY_BULLET						8	// NPC에게 총알 구매 성공.
#define SYSTEMNPC_SUCCESS_BUY_FOOD							9	// NPC에게 음식 구매 성공.
#define SYSTEMNPC_SUCCESS_BUY_HEAL							10	// NPC에게 약 구매 성공.
#define SYSTEMNPC_FAIL_BUY_SOLDOUT							11	// 입고 물품이 모두 판매되어 실패
/*
class cltSystemNPC{
public:
	SI32 siKind;
	SI32 siQuestType;
	TCHAR szExplain[512];

	cltSystemNPC()
	{
		siKind		= 0;
		siQuestType	= 0;
		szExplain[ 0 ] = NULL;
	}

	void Set(cltSystemNPC* pclinfo)
	{
		Set(pclinfo->siKind, pclinfo->siQuestType, pclinfo->szExplain);
	}

	void Set(SI32 kind, SI32 questtype, TCHAR* explain)
	{
		siKind		= kind;
		siQuestType	= questtype;

		StringCchCopy(szExplain, 512, explain);
	}
};

class cltSystemNPCManager{
public:
	cltSystemNPC clSystemNPC[MAX_SYSTEM_NPC_NUMBER];

	cltSystemNPCManager();
	~cltSystemNPCManager();

	bool Add(SI32 kind, SI32 questtype, TCHAR* explain);
	bool FindSystemNPCInfo(SI32 unique, cltSystemNPC* pclinfo);
	bool FindSystemNPCInfoFromQuestType(SI32 questtype, cltSystemNPC* pclinfo);
};
*/

class cltSystemNPC
{
public:
	SI32 m_siKind;
	SI16 m_siQuestType;
	SI32 m_siExplainNum;		// NPC Info창에 나오는 NPC설명
	SI16 m_siCategory;			// 이 퀘스트의 분류
	SI32 m_siSimpleExplainNum;	// 이 퀘스트의 간략 설명
	SI32 m_siLocationNum;		// NPC위치 Text Number
	SI16 m_siShowNotice;		// 퀘스트 알림창에 표시될지 여부(0=표시안됨 0<검색순서)
	SI32 m_siNoticeExplainNum;	// 퀘스트 알림창 설명
	SI32 m_siNoticeLocationNum;	// 퀘스트 알림창 지역 설명
	SI16 m_siMapIndex;			// NPC가 있는 위치
	SI16 m_siImageDisplayMode;	// 퀘스트 부여 이미지 관련

public:
	cltSystemNPC();
	cltSystemNPC( cltSystemNPC* pclSystemNPC );
	~cltSystemNPC();

	void Init( void );
	void Set( cltSystemNPC* pclSystemNPC );
	void Set( SI32 siKind, SI16 siQuestType, SI32 siExplainNum, SI32 siLocationNum, SI16 siCategory, SI32 siSimpleExplainNum, SI16 siShowNotice, SI32 siNoticeExplainNum, SI32 siNoticeLocationNum, SI16 siMapIndex, SI16 siImageDisplayMode );

	bool GetExplainText( TCHAR* pszBuffer, SI32 siBufferSize );

};

enum QUEST_CATEGORY
{
	CATEGORY_NONE			= 0
	,CATEGORY_BEGINNER
	,CATEGORY_MANUFACTURE
	,CATEGORY_WORLD

	//---------------------------
	// 이 위로 추가해 주세요
	//---------------------------
	,CATEGORY_MAX
};

enum QUESTDISPLAY_MARK
{
	QUESTDISPLAY_MODE_NONE = 0
	,QUESTDISPLAY_MODE_ALL			// 무조건 표시함
	,QUESTDISPLAY_MODE_CONDITION	// 조건이 일치하면 표시함
	,QUESTDISPLAY_MODE_GOING		// 진행중에만 표시함

	,QUESTDISPLAY_MODE_MAX
};

class cltSystemNPCManager
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltSystemNPC, MAX_SYSTEM_NPC_NUMBER>		clSystemNPC;
#else
	cltSystemNPC clSystemNPC[ MAX_SYSTEM_NPC_NUMBER ];
#endif

public:
	cltSystemNPCManager();
	~cltSystemNPCManager();
	
	void Init( void );

	bool LoadSystemNPCInfo( void );
	bool Add( cltSystemNPC* pclSystemNPC );

	SI16 ConvertCategoryTextToNum( TCHAR* pszCategory );

	bool GetSystemNPCListFormCategory( IN SI16 siCategory, OUT cltSystemNPC* pclSystemNPCList, IN SI32 siListCount );
	bool GetSystemNPCListForShowNotice( OUT cltSystemNPC* pclSystemNPCList, IN SI32 siListCount );

	bool FindSystemNPCInfoFromNPCKind( IN SI32 siKind, OUT cltSystemNPC* pclSystemNPC );
	bool FindSystemNPCInfoFromQuestType( IN SI32 siQuestType, OUT cltSystemNPC* pclSystemNPC );
};

#endif
