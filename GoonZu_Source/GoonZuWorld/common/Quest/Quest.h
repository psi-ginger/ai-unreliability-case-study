//---------------------------------
// 2003/11/22 김태곤
//---------------------------------

#ifndef _QUEST_H
#define _QUEST_H

#include <Directives.h>

#include "../../Lib/commonutil.h"
#include "../../common/Item/ItemCommon/ItemUnit.h"
#include "../../NLib/NDataTypes.h"
#include "../../NLib/NArray.h"
#include "../../common/CommonHeader.h"

#include "../../../NLib/NTextManager.h"

class HashStringInt;
class cltQuestManager;
class cltCharCommon;
class cltCharClient;

#define MAX_START_CONVERSATION_NUM	10			// 처음 대화할 수
#define MAX_END_CONVERSATION_NUM	10			// 마지막 대화할 수
#define MAX_TITLE_TEXT_SIZE			256

#define MAX_QUEST_PER_TYPE_NUMBER	32		// 하나의 퀘스트 타입당 최대 퀘스트 수. 

#define MAX_QUEST_REWARD_NUMBER		6

//---------------------------------
// 퀘스트 단계 
//---------------------------------
#define QUEST_STEP_GREETING			10
#define QUEST_STEP_MIDDLE			20
#define QUEST_STEP_GOING			30
#define QUEST_STEP_CLEAR			40
#define QUEST_STEP_NEXTQUEST		50

//---------------------------------
// 퀘스트 타입 
//---------------------------------

// [영훈] 퀘스트타입 enum으로 교체
enum QUEST_TYPE
{
	QUEST_TYPE_NONE					= -1,	//-1 아무 퀘스트 타입도 아니다

	QUEST_TYPE_SPECIAL				= 0,	//00 스페셜 퀘스트. 
	QUEST_TYPE_BATTLE1				= 1,	//01 기초 전투. (내금위종사관)
	QUEST_TYPE_MAKEHEAL1,					//02 기초 조제술
	QUEST_TYPE_MAKESWORD1,					//03 기초 검제조술
	QUEST_TYPE_MAKEBOW1,					//04 기초 활제조술
	QUEST_TYPE_MAKEHELMET1,					//05 기초 투구제조술
	QUEST_TYPE_BEGINNER3,					//06 김정호 퀘스트 
	QUEST_TYPE_MAKESPEAR1,					//07 기초 창제조술
	QUEST_TYPE_MAKEARMOUR1,					//08 갑옷 제조술
	QUEST_TYPE_MAKEAX1,						//09 도끼 제조술

	QUEST_TYPE_MAKEGUN1,					//10 총 제조술
	QUEST_TYPE_MAKESTAFF1,					//11 지팡이 제조술
	QUEST_TYPE_MAKEBELT1,					//12 요대 제조술
	QUEST_TYPE_MAKESHOES1,					//13 신발 제조술
	QUEST_TYPE_BEGINNER2,					//14 내시 퀘스트 
	QUEST_TYPE_BATTLE2,						//15 장길산 퀘스트
	QUEST_TYPE_BASIC1,						//16 기초 상업. (장금이 퀘스트)
	QUEST_TYPE_BEGINNER4,					//17 나죄수퀘스트
	QUEST_TYPE_MAKEBEGINNER1,				//18 초보자를 위한 제조 퀘스트
	QUEST_TYPE_HUNTINGETC01,				//19 야외NPC1

	QUEST_TYPE_NAMING,						//20 네이밍 퀘스트
	QUEST_TYPE_HAPPYBEAN,					//21 KHY - 0822 - 해피빈 퀘스트 추가.
	QUEST_TYPE_HUNTINGGROUND01,				//22 이상한온천 퀘스트
	QUEST_TYPE_HUNTINGGROUND02,				//23 두더지탄광
	QUEST_TYPE_MAKESANTAHAT,				//24 크리스마스 이벤트 퀘스트 1
	QUEST_TYPE_MAKECAKE,					//25 크리스마스 이벤트 퀘스트 2
	QUEST_TYPE_PLAYCHRISTMAS,				//26 크리스마스 이벤트 퀘스트 3
	QUEST_TYPE_HUNTINGGROUND03,				//27 얼음성 퀘스트
	QUEST_TYPE_HUNTINGGROUND04,				//28 마물의 산 퀘스트
	QUEST_TYPE_HUNTINGGROUND05,				//29 벨리타 퀘스트

	QUEST_TYPE_HUNTINGGROUND06,				//30 히니어스 퀘스트
	QUEST_TYPE_HUNTINGGROUND07,				//31 켈란 퀘스트
	QUEST_TYPE_HUNTINGGROUND08,				//32 제프리 퀘스트
	QUEST_TYPE_HUNTINGGROUND09,				//33 몰다 퀘스트
	QUEST_TYPE_HUNTINGGROUND10,				//34 세브리노 퀘스트
	QUEST_TYPE_HUNTINGGROUND11,				//35 일리아나 퀘스트
	QUEST_TYPE_MAKEFOOD1,					//36 프리지아 퀘스트
	QUEST_TYPE_BEGINNER1,					//37 아이린 퀘스트
	QUEST_TYPE_DAILYQUEST2,					//38 데일리 퀘스트2
	QUEST_TYPE_DAILYQUEST3,					//39 데일리 퀘스트3

	QUEST_TYPE_MAKEARTIFACT1,				//40 아티펙트 퀘스트(신디)
	QUEST_TYPE_MISSION1,					//41 미션 퀘스트
	QUEST_TYPE_HUNTINGGROUND12,				//42 프레드릭 퀘스트
	QUEST_TYPE_HUNTINGGROUND13,				//43 로이드 퀘스트

	QUEST_TYPE_FINDTREASURE,				//44 보물찾기 이벤트 _퀘스트 시작
	QUEST_TYPE_HUNTINGGROUND14,				//45 잃어버린 세계 퀘스트

	QUEST_TYPE_AVATAR_DRESS,				//46 아바타 드레스 퀘스트
	QUEST_TYPE_AVATAR_HAT,					//47 아바타 모자 퀘스트
	QUEST_TYPE_AVATAR_MENTAL,				//48 아바타 망토 퀘스트
	QUEST_TYPE_AVATAR_RING,					//49 아바타 반지 퀘스트
	QUEST_TYPE_AVATAR_NECK,					//50 아바타 목걸이 퀘스트
	QUEST_TYPE_PLAYCHRISTMASHIDDEN,			//51 크리스마스 이벤트 퀘스트 4

	QUEST_TYPE_HUNTINGGROUND15,				//52 안개미궁 퀘스트

	//---------------------------
	//	이 위로 추가시켜 주세요
	//---------------------------
	MAX_QUEST_TYPE
};
//KHY - 0822 - 해피빈 퀘스트 추가.
//---------------------------------
// 해피빈 퀘스트 단계 
//---------------------------------
#define QUEST_HAPPYBEAN_STEP_NON			0
#define QUEST_HAPPYBEAN_STEP_START		10
#define QUEST_HAPPYBEAN_STEP_COMPLETE	20

#define MAX_QUEST_ONEDAY_COUNT	2  //하루에 실행 되는 퀘스트 수.

enum MISSIONQUEST_UNIQUE
{
	MISSIONQUEST_UNIQUE_NONE = 0,
	//--------------------------------------
	MISSIONQUEST_UNIQUE_HOLLOWEEN,
	MISSIONQUEST_UNIQUE_REMOVE_UNLUCK,
	MISSIONQUEST_UNIQUE_GOONZUDAY,
};

//---------------------------------
// Person QUest의 타입별 단위 정보. 
//---------------------------------
class cltPersonQuestUnitInfo{
public:
	SI32 siQuestState;			// 퀘스트 클리어 여부. (각 비트단위로 0이면 안클리어 1이면 클리어. ) 
	SI16 siCurQuestUnique;		// 현재 진행중인 각 타입별 퀘스트 Unique. 
	SI16 siCurQuestStep;		// 현재 진행중이 각 타입별 퀘스트의 단계. 
	
	SI16 siQuestCmd;			// 수행해야 할 임무의 타입. 
	SI16 siQuestPara1;
	SI16 siQuestPara2;

	cltPersonQuestUnitInfo()
	{
		Init();
	}

	void Init()
	{
		siQuestState		= 0;
		siCurQuestUnique	= 0;
		siCurQuestStep		= 0;

		siQuestCmd			= 0;
		siQuestPara1		= 0;
		siQuestPara2		= 0;
	}

	void Set(cltPersonQuestUnitInfo* pclinfo)
	{
		siQuestState		= pclinfo->siQuestState;
		siCurQuestUnique	= pclinfo->siCurQuestUnique;
		siCurQuestStep		= pclinfo->siCurQuestStep;
		siQuestCmd			= pclinfo->siQuestCmd;
		siQuestPara1		= pclinfo->siQuestPara1;
		siQuestPara2		= pclinfo->siQuestPara2;
	}

	BOOL IsSame( cltPersonQuestUnitInfo* pclinfo )
	{
		if(siQuestState	!= pclinfo->siQuestState)
			return FALSE;
		if(siCurQuestUnique	!= pclinfo->siCurQuestUnique)
			return FALSE;
		if(siCurQuestStep	!= pclinfo->siCurQuestStep)
			return FALSE;
		if(siQuestCmd	!= pclinfo->siQuestCmd)
			return FALSE;
		if(siQuestPara1	!= pclinfo->siQuestPara1)
			return FALSE;
		if(siQuestPara2	!= pclinfo->siQuestPara2)
			return FALSE;
		return TRUE;
	}


	// 특정 비트를 설정한다. 
	void SetBit(SI32 bit, SI32 data)
	{
		if(data)
		{
			siQuestState |= (1 << bit);
		}
		else
		{
			if(siQuestState  & (1 << bit))
			{
				siQuestState -= (1 << bit);
			}
		}
	}

	// 특정 비트의 정보를 얻어온다. 
	SI32 GetBit(SI32 bit)
	{
		return (siQuestState & (1 << bit));
	}

	
};


//----------------------------------
// 퀘스트 중간 정보. 
//----------------------------------
class cltQuestMiddleInfo{
public:
	GMONEY siGiveMoney;		// 사용자에게 줄 돈. 	
	cltItem clItem;			// 사용자에게 줄 아이템. 

	cltQuestMiddleInfo()
	{
		Init();
	}
	void Init()
	{
		siGiveMoney = 0;
		clItem.Init();
	}
	void Set(cltQuestMiddleInfo* pclinfo)
	{
		siGiveMoney = pclinfo->siGiveMoney;
		clItem.Set(&pclinfo->clItem);
	}
};

//--------------------------------------------
// 퀘스트 txt 정보
//------------------------------------------
class cltQuestTxtInfo{
public:
	// 퀘스트 대화말
	TCHAR szStartQuestConversation[MAX_START_CONVERSATION_NUM][1024];
	// 퀘스트 대화말
	TCHAR szEndQuestConversation[MAX_END_CONVERSATION_NUM][1024];
	// 퀘스트 시작 txt
	TCHAR szQuestGreeting[1024];
	// 퀘스트 진행 txt
	TCHAR szQuestMiddle[1024];
	// 퀘스트 종료 txt
	TCHAR szQuestClear[1024];

#ifdef _SAFE_MEMORY
	// 퀘스트 NPC 얼굴 표정 인덱스
	NSafeTArray<SI32, MAX_START_CONVERSATION_NUM>		siStartSprIndex;
	// 신이 얼굴 표정 인덱스
	NSafeTArray<SI32, MAX_END_CONVERSATION_NUM>			siEndSprIndex;
#else
	// 퀘스트 NPC 얼굴 표정 인덱스
	SI32 siStartSprIndex[MAX_START_CONVERSATION_NUM];
	// 신이 얼굴 표정 인덱스
	SI32 siEndSprIndex[MAX_END_CONVERSATION_NUM];
#endif
	SI32 siStartConversationNum;
	SI32 siEndConversationNum;
	SI32 siGreetingSpr;
	SI32 siMiddleSpr;
	SI32 siClearSpr;

	cltQuestTxtInfo()
	{
		Init();
	}

	void Init()
	{
		for(SI32 i=0; i<MAX_START_CONVERSATION_NUM; ++i)
		{
			MStrCpy(szStartQuestConversation[i], TEXT(""), 1024);
			siStartSprIndex[i] = 0;
		}
		for(SI32 i=0; i<MAX_END_CONVERSATION_NUM; ++i)
		{
			MStrCpy(szEndQuestConversation[i], TEXT(""), 1024);
			siEndSprIndex[i] = 0;
		}
		
		MStrCpy(szQuestGreeting,TEXT(""),1024);
		MStrCpy(szQuestMiddle,TEXT(""),1024);
		MStrCpy(szQuestClear,TEXT(""),1024);

		siStartConversationNum = 0;
		siEndConversationNum = 0;
		siGreetingSpr = 0 ;
		siMiddleSpr = 0 ;
		siClearSpr = 0 ;
	}

	void Set(cltQuestTxtInfo* pclinfo)
	{
		for(SI32 i=0; pclinfo->siStartConversationNum; ++i)
		{
			MStrCpy(szStartQuestConversation[i], pclinfo->szStartQuestConversation[i], 1024);
			siStartSprIndex[i] = pclinfo->siStartSprIndex[i];
		}
		for(SI32 i=0; pclinfo->siEndConversationNum; ++i)
		{
			MStrCpy(szEndQuestConversation[i], pclinfo->szEndQuestConversation[i], 1024);
			siEndSprIndex[i] = pclinfo->siEndSprIndex[i];
		}

		MStrCpy(szQuestGreeting,pclinfo->szQuestGreeting,1024);
		MStrCpy(szQuestMiddle,pclinfo->szQuestMiddle,1024);
		MStrCpy(szQuestClear,pclinfo->szQuestClear,1024);

		siStartConversationNum = pclinfo->siStartConversationNum;
		siEndConversationNum = pclinfo->siEndConversationNum;
		siGreetingSpr = pclinfo->siGreetingSpr ;
		siMiddleSpr = pclinfo->siMiddleSpr ;
		siClearSpr = pclinfo->siClearSpr ;
	}
};

//---------------------------------
// 퀘스트 완료 조건. 
//---------------------------------

// [영훈] 퀘스트 클리어 컨디션 enum으로 변경
enum QUEST_CLEAR_COND
{
	QUEST_CLEAR_COND_NOTMEAN				= 0	//000 퀘스트 클리어 컨디션(아무것도 아님)

	,QUEST_CLEAR_COND_BONUSZERO				= 1	//001 스탯 보너스 수치가  유효하다.
	,QUEST_CLEAR_COND_ITEM						//002 아이템을 갖고 있어야 한다. 
	,QUEST_CLEAR_COND_KILLMON					//003 몬스터 일정수 처치. 
	,QUEST_CLEAR_COND_MAKEITEM					//004 물품 생산해야 한다.  
	,QUEST_CLEAR_COND_OPENCHARSTATUS			//005 캐릭터 정보창을 열어야 한다. 
	,QUEST_CLEAR_COND_LEVEL						//006 캐릭터 레벨이 얼마 이상이어야 한다. 
	,QUEST_CLEAR_COND_FULLLIFE					//007 최대 체력 채우기. 
	,QUEST_CLEAR_COND_OPENSKILL					//008 캐릭터 기술 정보창을 열어야 한다. 
	,QUEST_CLEAR_COND_SKILL						//009 특정 기술의 레벨이 얼마 이상이어야 한다. 

	,QUEST_CLEAR_COND_EQUIP						//010 특정 물품 장착. 
	,QUEST_CLEAR_COND_SELLNPC					//011 특정 NPC에게 물건을 판다. 
	,QUEST_CLEAR_COND_MONEY						//012 돈을 얼마 모아온다. 
	,QUEST_CLEAR_COND_BANK						//013 전장에 돈을 얼마 입금한다. 
	,QUEST_CLEAR_COND_HUNGRY					//014 배고픔을 없앤다. 
	,QUEST_CLEAR_COND_REPAIRWEAPON				//015 무기를 수리한다.  
	,QUEST_CLEAR_COND_ITEMINFO					//016 아이템 정보를 확인한다.
	,QUEST_CLEAR_COND_NEWS						//017 군주 미디어센터로 이동해야 한다. 
	,QUEST_CLEAR_COND_SKILLBONUSZERO			//018 기술 점수를 0으로 만들어야 한다.
	,QUEST_CLEAR_COND_GOMAP						//019 특정 지도에 갔다와야 한다.. 

	,QUEST_CLEAR_COND_GETQUEST					//020 특정 퀘스트를 부여받아야 한다.
	,QUEST_CLEAR_COND_SELLMARKET				//021 특정 물품을 객주에서 팔아야 한다.  
	,QUEST_CLEAR_COND_BASICWEAPONSKILL			//022 기본 무기 사용술 습득(검, 활, 지팡이)
	,QUEST_CLEAR_COND_STRUCTUREOPEN				//023 특정 건물을 열어본다. 
	,QUEST_CLEAR_COND_ITEMINFODLG				//024 물품 상세 정보창 열기. 
	,QUEST_CLEAR_COND_SELLSWORDNPC				//025 검상인에게 특정 검을 판매한다. 
	,QUEST_CLEAR_COND_OTHERMINIHOME				//026 다른 사람 꼼지 가기. 
	,QUEST_CLEAR_COND_OPENHEADCHAT				//027 광고창 띄우기 
	,QUEST_CLEAR_COND_BUYFROMPBSTORE			//028 개인상점에서 구매하기. 
	,QUEST_CLEAR_COND_OPENPBSTORE				//029 개인상점열기 

	,QUEST_CLEAR_COND_OPENSTOCKMARKET			//030 여각 열기 	
	,QUEST_CLEAR_COND_STOCK						//031 주식 가지기. 
	,QUEST_CLEAR_COND_OPENCITYHALL				//032 상단 열기 	
	,QUEST_CLEAR_COND_APPLYVILLAGE				//033 마을 주민 신청하기. 
	,QUEST_CLEAR_COND_USEFAN					//034 부채도사 이용하여 임의 마을 가기 
	,QUEST_CLEAR_COND_USEWENNYOLDMAN			//035 혹부리영감 이용하여 호품 만들기 
	,QUEST_CLEAR_COND_HORSE						//036 말 가지기 
	,QUEST_CLEAR_COND_OPENHUNT					//037 수렵장 열기 	
	,QUEST_CLEAR_COND_USEWARPTICKET1			//038 순간이동이용권 이용하기 (마을가기) 
	,QUEST_CLEAR_COND_USEWARPTICKET2			//038 순간이동이용권 이용하기 (사냥터가기) 

	,QUEST_CLEAR_COND_SUMMON					//040 소환수 데려오기 
	,QUEST_CLEAR_COND_BROADCAST					//041 군주 방송 듣기 
	,QUEST_CLEAR_COND_SKILLMASTER				//042 말 조련 장인정보 조회하기. 
	,QUEST_CLEAR_COND_MAKEFRIEND				//043 친구등록하기. 
	,QUEST_CLEAR_COND_LOOKPANSEO				//044 형조 판서책상 가보기 
	,QUEST_CLEAR_COND_OPENVILLAGEBOARD			//045 상단 게시판 열어보기. 
	,QUEST_CLEAR_COND_FAMELEVEL					//046 명성 레벨 올리기(1만들기)  
	,QUEST_CLEAR_COND_USEITEM					//047 특정 물품을 특정 개수만큼 사용하기 
	,QUEST_CLEAR_COND_SENDITEMBYPOST			//048 역참을 통해 다른 사람에게 물건을 보내보기.(물건은 아무거나 무방하다.)  
	,QUEST_CLEAR_COND_VILLAGEDETAILINFO			//049 마을 상세 정보보기

	,QUEST_CLEAR_COND_USEWENNYOLDMAN2			//050 혹부리영감 이용하여 개조하기. 
	,QUEST_CLEAR_COND_SUMMON2					//051 소환수 2마리 키우기. 
	,QUEST_CLEAR_COND_ITEMMALL					//052 아이템몰에서 아이템 사보기. 
	,QUEST_CLEAR_COND_GGOMGICHANGE				//053 (X)꼼지 프로필 사진과 인사말 수정하기. 
	,QUEST_CLEAR_COND_BAG						//054 가방에 물건 넣어 보기. 
	,QUEST_CLEAR_COND_EQUIPPOS					//055 특정 아이템 장착. (반지, 목걸이, 요대, 신발, 의복, 모자, 갑옷, 투구 등..) 
	,QUEST_CLEAR_COND_FISH						//056 사용자가 낚시하기 
	,QUEST_CLEAR_COND_SUMMONBONUS				//057 모든 소환수의 보너스를 0으로 만들기. 
	,QUEST_CLEAR_COND_BERESIDENT				//058 (X)마을 주민 되기. 
	,QUEST_CLEAR_COND_VILLAGEHUNT				//059 (X)불가사리 사냥 성공하기 . 

	,QUEST_CLEAR_COND_GOGU						//060 (X)고구려유적보호이벤트 성공하기.
	,QUEST_CLEAR_COND_SCROLL					//061 아무부적이나 장비에 부착해보기. 
	,QUEST_CLEAR_COND_MAXUPSCROLL				//062 (X)최대내구상승부적 아무 장비에 부착해보기. 
	,QUEST_CLEAR_COND_SELLTRADER				//063 무역상인에게 물건 팔아보기. (명 무역상 ) 
	,QUEST_CLEAR_COND_FEEDHORSE1				//064 말이 원하는 먹이 먹이기. 
	,QUEST_CLEAR_COND_FEEDHORSE2				//065 말사료 먹이기. 
	,QUEST_CLEAR_COND_REVIVESUMMON				//066 소환수부활이용권 사용하여 소환수 살려보기.
	,QUEST_CLEAR_COND_DAILYQUEST				//067 모병관 퀘스트 깨기. 
	,QUEST_CLEAR_COND_SPECIALQUEST				//068 흥부박퀘스트 깨기 
	,QUEST_CLEAR_COND_SKILLLEVEL				//069 (X)특정 기술의 레벨을 특정 수준으로 올리기.(경험과 책을 읽어서만..) 

	,QUEST_CLEAR_COND_SUMMONEVOLUTION			//070 소환수를 진화시키기(소환수를 진화시키면 된다.)
	,QUEST_CLEAR_COND_CLEARQUEST				//071 특정 퀘스트를 깨야 한다. 
	,QUEST_CLEAR_COND_HORSESPEED				//072 말속도를 10이상 만들기. 
	,QUEST_CLEAR_COND_SUMMONSCROLL				//073 부적을 사용하여 소환수 강화하기. 
	,QUEST_CLEAR_COND_SUMMONBATTLE				//074 소환수가 몬스터(para1)를 para2만큼 사냥해보기. 
	,QUEST_CLEAR_COND_USEMAGIC					//075 아무 마법이나 특정 회수 사용해 보기. 
	,QUEST_CLEAR_COND_FARM						//076 특정 작물(para1)을 특정개수(para2)만큼 수확하라. 
	,QUEST_CLEAR_COND_STOCKPROMPT				//077 (X)주식 정보를 자막정보에 보이게 하기. 
	,QUEST_CLEAR_COND_MINE						//078 특정 광물(para1)을 특정개수(para2)만큼 채광하라.
	,QUEST_CLEAR_COND_WEBFAME					//079 (X)꼼지 방문자수를 올려서 명성훈장 받기(레벨 120이상).

	,QUEST_CLEAR_COND_HAVEHOUSE					//080 (X)임의의 시전을 가져보기. (레벨 100이상) 
	,QUEST_CLEAR_COND_MAKEREQUEST				//081 (X)다른 사람에게 제조의뢰 해보기, 아무거나 제조의뢰해서 제조되면 클리어(중렙) 
	,QUEST_CLEAR_COND_GONPC						//082 특정 NPC에게 가서 메뉴 끝까지 보기
	,QUEST_CLEAR_COND_SETFATHER					//083 스승 설정하기
	,QUEST_CLEAR_COND_CLICKITEMINFO				//084 특정 아이템의 설명 문구 클릭해 보기
	,QUEST_CLEAR_COND_OPENMAKEITEM				//085 제조창 열기
	,QUEST_CLEAR_COND_CLICKMAKEBUTTON			//086 버튼 누르기
	,QUEST_CLEAR_COND_TUTORIALMAKEITEM			//087 특정 아이템 제조 퀘스트(조건(레벨,손재주,스킬)을 따지지 않고 가능케 해준다. 실제 재료 이외의 다른 아이템을 소모시키키도 함)
	,QUEST_CLEAR_COND_ITEMCHECK					//088 특정 아이템을 가지고 있어야 한다. 사라지지 않고 체크만 한다.
	,QUEST_CLEAR_COND_UPITEMTOMARKET			//089 특정 아이템을 마켓에 올려야 한다.

	,QUEST_CLEAR_COND_OPENENCHANTITEM			//090 인첸트 창을 열어야 한다.
	,QUEST_CLEAR_COND_SETENCHANTITEM			//091 특정 아이템을 인첸트 창에 올려야 한다.
	,QUEST_CLEAR_COND_SETENCHANTELEMENT			//092 아무 속성이나 인첸트 속성을 설정해야 한다.
	,QUEST_CLEAR_COND_ENCHANTITEM				//093 특정 아이템을 인첸트 해야 한다.
	,QUEST_CLEAR_COND_WARPTOPALACE				//094 경복궁으로 워프하기
	,QUEST_CLEAR_COND_PRIVATETRADE				//095 개인거래 해보기
	,QUEST_CLEAR_COND_SELLFROMPBSTORE			//096 개인상점에서 아이템 팔아보기
	,QUEST_CLEAR_COND_MAKEBOOK					//097 책 만들어보기
	,QUEST_CLEAR_COND_HUNTMAPINFODLG			//098 사냥터 정보창을 열어본다
	,QUEST_CLEAR_COND_SEARCHMONSTERINFO			//099 특정 몬스터의 정보를 검색해 본다 (para1) 에 몬스터 KIND_UNIQUE

	,QUEST_CLEAR_COND_PARTYHUNTING				//100 파티를 맺고 사냥을 한다.
	,QUEST_CLEAR_COND_MAGICINFODLG				//101 마법 정보창을 열어본다.
	,QUEST_CLEAR_COND_OPENHOUSE					//102 임대창고 열어본다
	,QUEST_CLEAR_COND_OPENREALESTATEMARKET		//103 부동산 열어본다
	,QUEST_CLEAR_COND_BLACKARMY					//104 흑의 군단 참여해 본다
	,QUEST_CLEAR_COND_OPENHORSEMARKET			//105 목장 건물 열어보기
	,QUEST_CLEAR_COND_SUMMONEVOLUTIONRANK		//106 소환수를 특정 등급까지 진화시키기 (para1 == 특정 등급)
	,QUEST_CLEAR_COND_REVIVESUMMONBYDOLL		//107 소환수를 인형을 사용하여 부활시킨다
	,QUEST_CLEAR_COND_OPENHOZODLG			 	//108 호조판서(백호관)의 정보창을 열어본다
	,QUEST_CLEAR_COND_OPENBYUNGZODLG		 	//109 병조판서(청룡관)의 정보창을 열어본다

	,QUEST_CLEAR_COND_OPENGONGZODLG			 	//110 공조판서(현무관)의 정보창을 열어본다
	,QUEST_CLEAR_COND_OPENIZODLG			 	//111 이조판서(주작관)의 정보창을 열어본다
	,QUEST_CLEAR_COND_OPENGOONZUDLG			 	//112 군주의 정보창을 열어본다
	,QUEST_CLEAR_COND_VOTEKING					//113 군주선거에 참여해 본다
	,QUEST_CLEAR_COND_USEWARPTICKET3			//114 순간이동이용권 이용하기 (친구에게가기)
	,QUEST_CLEAR_COND_BUYTOPBSTORE				//115 개인상점을 통해 구매하기
	,QUEST_CLEAR_COND_NONE						//116 그냥 클리어 되는 퀘스트 이다
	,QUEST_CLEAR_COND_ENCHANTANYITEM			//117 아무 아이템이나 인첸트 해본다
	,QUEST_CLEAR_COND_HUNT_ITEM					//118 사냥으로 아이템 획득.
	,QUEST_CLEAR_COND_GOHUNTMAP					//119 수렵장 가기

	,QUEST_CLEAR_COND_MAGICACTIVE				//120 마법 등록하기 (단축키 )
	,QUEST_CLEAR_COND_OPENFRIEND				//121 친구 목록창을 연다.
	,QUEST_CLEAR_COND_OPENMASTER				//122 장인 정보창을 연다.
	,QUEST_CLEAR_COND_OPENMAKEREQUEST			//123 제조 의뢰창을 연다.
	,QUEST_CLEAR_COND_MAKEREQUESTSTART			//124 제조 의뢰 시작 ( 제조의뢰 한다는 말풍선 띄우기).
	,QUEST_CLEAR_COND_DISASSEMBLEITEM			//125 분해한다.(유니크 상관없음)
	,QUEST_CLEAR_COND_OPENNAMINGQUEST			//126 명칭 퀘스트 창을 연다.
	,QUEST_CLEAR_COND_SUMMONACTIVE				//127 소환수 활성화
	,QUEST_CLEAR_COND_SUMMONDIE					//128 소환수 사망.
	,QUEST_CLEAR_COND_SUMMONCHANGESTATUS		//129 소환수 상태 창 열기

	,QUEST_CLEAR_COND_PARTYKILLMON				//130 파티로 사냥하기.
	,QUEST_CLEAR_COND_SUMMONATTACK				//131 소환수로 강제 공격해보기
	,QUEST_CLEAR_COND_SUMMONLEVELUP				//132 소환수 레벨업
	,QUEST_CLEAR_COND_MAKEINTBOOK				//133 중급책 만들기
	,QUEST_CLEAR_COND_SETGREETING				//134 인사말 설정
	,QUEST_CLEAR_COND_OPENGUILD					//135 길드창 열기
	,QUEST_CLEAR_COND_APPLYGUILD				//136 길드 가입, 개설( 대기중도 통과)
	,QUEST_CLEAR_COND_LOOKPRISON				//137 수감자 현황 보기
	,QUEST_CLEAR_COND_OPENGOONZUDLG_NOTICE		//138 군주 정책 보기
	,QUEST_CLEAR_COND_MAKEPARTY					//139 파티 맺기

	,QUEST_CLEAR_COND_SORTRESIDENT				//140 마을 정보 보기 - 마을 주민수 정렬
	,QUEST_CLEAR_COND_CLICKITEMPROPERTY			//141 아이템 속성정보 클릭
	,QUEST_CLEAR_COND_OPENSKILLETC				//142 기타 스킬 버튼 누르기
	,QUEST_CLEAR_COND_MINENOSKILL				//143 채광하기( 스킬 올리지 않아도 가능 )
	,QUEST_CLEAR_COND_SITDOWN					//144 앉기(일어서기)
	,QUEST_CLEAR_COND_NPCINFO					//145 NPC정보를 확인한다.
	,QUEST_CLEAR_COND_HITMON					//146 몬스터 일정수 때리기.
	,QUEST_CLEAR_COND_OPENGUILDUSER				//147 길드창에서 개설신청 열기
	,QUEST_CLEAR_COND_SEARCHSTOCKINFO			//148 마을정보 창에서 주식거래소 검색하기
	,QUEST_CLEAR_COND_CLICKSTOCKOFFER		 	//149 공모청약 버튼 클릭하기

	,QUEST_CLEAR_COND_CLICKCITYHALLFUND		 	//150 관청->관청자금 버튼 클릭
	,QUEST_CLEAR_COND_CLICKGENERALMETTING	 	//151 관청->주주총회 버튼 클릭
	,QUEST_CLEAR_COND_CLICKPERSONALSHOP		 	//152 커뮤니티->개인상점 클릭
	,QUEST_CLEAR_COND_OPENITEMMALL			 	//153 아이템 몰 창 열기
	,QUEST_CLEAR_COND_ANGELLISTCLICK		 	//154 공작 -> 수호천사 선정 -> 수호천사 목록 클릭
	,QUEST_CLEAR_COND_POLICESETCLICK		 	//155 남작 -> 보안관 설정 클릭
	,QUEST_CLEAR_COND_SEARCHFARM			 	//156 게임정보 -> 마을 -> 농장 선택
	,QUEST_CLEAR_COND_FARMNOSKILL			 	//157 농장에서 아이템 획득(스킬필요없음)
	,QUEST_CLEAR_COND_LEASECLICK			 	//158 창고 -> 임차신청 클릭
	,QUEST_CLEAR_COND_GETMAGICLAMPQUEST		 	//159 요술램프 퀘스트 받기

	,QUEST_CLEAR_COND_OPENQUESTDIALOG		 	//160 퀘스트 창 열기
	,QUEST_CLEAR_COND_FISHNOSKILL			 	//161 낚시해서 아이템 획득 해보기(스킬필요없음)
	,QUEST_CLEAR_COND_ISLEARNED_MANUFACTURE	 	//162 제조스킬중에 하나라도 배운것이 있으면 퀘스트 클리어 
	,QUEST_CLEAR_COND_ISGET_MANUFACTUREQUEST 	//163 엔펠리스에 있는 제조 퀘스트 중 어떤것이라도 1단계 이상 클리어
	,QUEST_CLEAR_COND_REQUESTMENTORSUCCESS	 	//164 스승창 -> 스승님신청 클릭해서 리스트에 등록하기(기존에 있어도 됨)
	,QUEST_CLEAR_COND_TAKEPUPILS				//165 누군가를 제자로 받아야 한다
	,QUEST_CLEAR_COND_CHATTOBEGGINER			//166 초보자 리스트의 누군가에게 귓말 요청하기
	,QUEST_CLEAR_COND_CHECKSTATUS				//167 (Para1)[Quest-LoadTxt.cpp:515참조]의 스텟이 (Para2) 이상이어야 한다
	,QUEST_CLEAR_COND_ALLWEAPONSKILL			//168 무기스킬 중에 레벨이(para1) 이상이 있는 지 확인
	,QUEST_CLEAR_COND_MEMOCLICK					//169 친구목록-> 메모버튼 클릭

	,QUEST_CLEAR_COND_MOVETOGROUPCLICK			//170 친구목록-> 그룹이동 버튼 클릭
	,QUEST_CLEAR_COND_EQUIPBAG					//171 가방칸 2칸 중에 한개라도 가방이 장창되어 있는가?
	,QUEST_CLEAR_COND_KILLDARKSOLDIER			//172 흑의 군단맴버 아무나 (para2)만큼 죽여야 한다.
	,QUEST_CLEAR_COND_MAKE_RAREITEM				//173 신기,명품,호품 아이템을 만들어야한다(para1이 0일때는 아무거나, 그외에는 unique@ para2는 갯수)
	,QUEST_CLEAR_COND_FULL_ORGAN				//174 내장 기관장 어느 한곳이라도 Max이면 클리어
	,QUEST_CLEAR_COND_TWOWAYKILLMON				//175 혼자 혹은 파티 맺고 사냥해도 몬스터 킬수가 같이 올라감
	,QUEST_CLEAR_COND_BUYMARKET					//176 시장에서 물건 사기
	,QUEST_CLEAR_COND_OPENFATHERDIALOG			//177 스승창 열어보기
	,QUEST_CLEAR_COND_ORDERTOMARKET				//178 시장에 물건 올리기
	,QUEST_CLEAR_COND_OPENPOSTOFFICEDIALOG		//179 우체국창 열어보기

	,QUEST_CLEAR_COND_MAKELEVELITEM				//180 Para1에 해당하는 레벨의 아이템을 Para2만큼 만들기
	,QUEST_CLEAR_COND_TAKEEQUIPLEVELWEAPON		//181 Para1에 해당하는 레벨의 무기를 Para2만큼 가져오기
	,QUEST_CLEAR_COND_GETELEMENT_RESTORE		//182 환원을 통해 Para2만큼 원소 아이템 얻기
	,QUEST_CLEAR_COND_GETPOWDER_RESTORE			//183 환원을 통해 Para2만큼 속성가루 아이템 얻기
	,QUEST_CLEAR_COND_ENCHANTEQUIPLEVELWEAPON	//184 Para1에 해당하는 레벨의 무기를 인첸트 해보기
	,QUEST_CLEAR_COND_TAKEITEM_MATERIAL			//185 Para1에 해당하는 아이템의 Para2갯수를 만들수 있는 재료들을 갖고오기
	,QUEST_CLEAR_COND_TAKEWEAPONHAVESLOT		//186 슬롯이 있는 무기를 Para2만큼 가져와라
	,QUEST_CLEAR_COND_ARTIFACTMIXWEAPON			//187 Para2의 갯수만큼 무기 아이템에 결정체 조합해보기
	,QUEST_CLEAR_COND_MAKECRYSTAL				//188 Para1의 등급의 결정체를 Para2갯수만큼 만들어보자
	,QUEST_CLEAR_COND_TEACHERSET				//189 [진성] 스승님 설정하기 버튼 클릭. => 2008-10-7

	,QUEST_CLEAR_COND_SUMMONBONUSZERO			//190 소환된 소환수의 보너스 수치를 0으로 만들기
	,QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM		//191 Para1에 해당하는 레벨의 아이템을 Para2만큼 만들기 - 스킬 레벨 체크 없음
	,QUEST_CLEAR_COND_SWORDSKILLCLICK			//192 알렉스(전투향상술) 창에서 검술버튼 클릭해보기
	,QUEST_CLEAR_COND_LEARNWEAPONSKILL			//193 Para1단계의 스킬을 Para2이상 배우기
	,QUEST_CLEAR_COND_PVPWIN					//194 PVP승리를 para2이상 하기
	,QUEST_CLEAR_COND_PVPLEAGUERESERVE			//195 PVP리그 예약해보기
	,QUEST_CLEAR_COND_PVPLEAGUEWIN				//196 PVP리스 승리를 para2이상 하기
	,QUEST_CLEAR_COND_TWOWAYKILLMONBONUS		//197 혼자 혹은 파티 맺고 사냥해도 몬스터 킬수가 같이 올라감(같은 클리어 컨디션의 파티 인원이 2명 이상일 경우 몬스터 잡을 때 보너스 20% 추가)
	,QUEST_CLEAR_COND_ARTIFACTMIX				//198 Para2만큼 아티팩트 조합해 보기
	,QUEST_CLEAR_COND_SETMYKEY					//199 나만의 메뉴 설정해 보기

	,QUEST_CLEAR_COND_SUMMONHOUSE_INPUT			//200 소환수 보관소에 소환수 넣기
	,QUEST_CLEAR_COND_SUMMONHOUSE_OUTPUT		//201 소환수 보관소에서 소환수 빼기
	,QUEST_CLEAR_COND_MARKETCONDITION			//202 클라라 시세 확인해 보기 (Para1의 아이템 타입을 일반검색 해보기)
	,QUEST_CLEAR_COND_FIND_TREASUREBOX			//203 보물찾기 이벤트	// 일단 클리어 컨디션만 만들어 뒀음.
	,QUEST_CLEAR_COND_ENCHANTEQUIPPOS			//204 일정레벨이상 인챈트된 장비 착용해 보기
	,QUEST_CLEAR_COND_BUFFCLICK					//205 인벤토리 창옆 기간제버튼 - 버프 버튼 클릭해 보기
	,QUEST_CLEAR_COND_GONPC_INFORMATION			//206 특정 엔피시로 가서 메뉴를 클릭하게 되면 특정한 정보가 나옴.
	,QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS	//207 잃어버린 세계 - 몬스터로 변신후에 지정된 위치 주변을 정찰해서 몬스터 정보 파악하기.
	,QUEST_CLEAR_COND_MAPKILLMON				//208 para1 맵에 있는 몬스터 para2죽이기
	,QUEST_CLEAR_COND_MAKESKILLEXPUP			//209 [지연] 제조 경험치가 1 이상 올라가야 한다.

	,QUEST_CLEAR_COND_LAMPGOMAP					//210 [지연] 요술램프 마을 순회 퀘스트의 조건 (기존 GOMAP조건을 같이 사용하는것이 문제가 생겨 분리함)

	,QUEST_CLEAR_COND_FORTUNE_NPCCOMPLETE		//211 [지연] 2010신년이벤트 - npc

	, QUEST_CLEAR_COND_SEARCH_TOTALMAP			//212 안개미궁 - 전체지도에서 특정 위치 검색 para1 : CheckIndex, para2 : SelectIndex
	, QUEST_CLEAR_COND_USE_SKILLBOOK			//213 안개미궁 - 매직스킬북 사용하기         para1 : 1         , para2 : amount

	//-----------------------------------
	// 이 위로 추가해 주세요
	//-----------------------------------
	,MAX_QUEST_CLEAR_COND_NUMBER
};


class cltQuestClearConditionInfo{
public:
	SI16	siFlag;			// 어떤 조건이 유효한지 파악. 

	SI32	siPara1;
	SI32	siPara2;

	cltQuestClearConditionInfo()
	{
		Init();
	}
	void Init()
	{
		siFlag	= 0;
		siPara1	= 0;
		siPara2 = 0;
	}

	void Set(cltQuestClearConditionInfo* pclinfo)
	{
		siFlag	= pclinfo->siFlag;
		siPara1	= pclinfo->siPara1;
		siPara2 = pclinfo->siPara2;
	}

};

// 특정 좌표에서 아이템사용하기 퀘스트
struct stCoordinate_UseItem
{
	SI32 m_siQuestType;
	SI32 m_siQuestUnique;
	SI32 m_siMapIndex;
	SI32 m_siX;
	SI32 m_siY;
};

//---------------------------------
// 퀘스트 보상 
//---------------------------------
class cltQuestRewardInfo{
public:
	
	GMONEY siMoney;			// 돈을 보상으로 준다. 
	
	SI32 siExp;				// 경험치 보상. 
	
	SI16 siFameExp;			// 명성 경험치 보상 
	
	SI32 siNCoin;			// NCoin 지급. 
	
	BOOL bMoveBuf;			// 이동속도 증가 버프

	bool bSelectItem;		// 아이템 선택

	// 네이밍 보상
	SI32 siNamingIndex;		// 어떤 종류의 네이밍인지
	SI32 siNamingLevel;		// 몇단계 네이밍인지.

	//KHY - 0219 - 스킬 경험치 보상.
	SI32 siSkillType;		// 어떤 스킬 타입인지
	SI32 siSkillExp;		// 스킬 경험치

	bool bIsGetMaterial;	// 보상아템을 재료로 받을 것인가?

	bool bApplyNpcRate;		// 돈 보상에 NPCRate를 적용할지 여부

	//[지연] : 2010신년이벤트 - 디버프 보상 (특정 버프를 하나 지워준다. 일단은 무조건 '흉'버프만 지움)
	bool bDeBuff;

	// 아이템 보상.
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_QUEST_REWARD_NUMBER>		siItemUnique;
	NSafeTArray<SI16, MAX_QUEST_REWARD_NUMBER>		siItemNum;
	NSafeTArray<SI32, MAX_QUEST_REWARD_NUMBER>		siItemUseDate;
#else
	SI16 siItemUnique[MAX_QUEST_REWARD_NUMBER];
	SI16 siItemNum[MAX_QUEST_REWARD_NUMBER];
	SI32 siItemUseDate[MAX_QUEST_REWARD_NUMBER];
#endif
	

	cltQuestRewardInfo()
	{
		Init();
	}

	void Init()
	{
		memset(this,0,sizeof(cltQuestRewardInfo));		

	}

	void Set(cltQuestRewardInfo* pclinfo)
	{
		siMoney			= pclinfo->siMoney;
		bApplyNpcRate	= pclinfo->bApplyNpcRate;

		siExp			= pclinfo->siExp;
				
		siFameExp		= pclinfo->siFameExp;
		siNCoin			= pclinfo->siNCoin;	
		bMoveBuf		= pclinfo->bMoveBuf;

		siNamingIndex	= pclinfo->siNamingIndex;
		siNamingLevel	= pclinfo->siNamingLevel;

		siSkillType		= pclinfo->siSkillType;
		siSkillExp		= pclinfo->siSkillExp;

		bIsGetMaterial	= pclinfo->bIsGetMaterial;

		bSelectItem		= pclinfo->bSelectItem;

		for( SI32 i = 0; i < MAX_QUEST_REWARD_NUMBER ; i++ )
		{
			siItemUnique[i]		= pclinfo->siItemUnique[i];
			siItemNum[i]		= pclinfo->siItemNum[i];
			siItemUseDate[i]	= pclinfo->siItemUseDate[i];
		}

		bDeBuff			= pclinfo->bDeBuff;
	}

	// 보상 내역을 텍스트에 담아낸다.
	bool GetExplainText(TCHAR* ptext, SI16 txtSize);
};

//--------------------------------
// 퀘스트 부여 조건. 
//--------------------------------
class cltQuestInitConditionInfo{
public:
	SI32 siLevel;
	SI32 siSkillUnique;
	SI32 siSkillLevel;
	SI32 siMustClearQuestUnique;

	cltQuestInitConditionInfo()
	{
		Init();
	}
	void Init()
	{
		siLevel			=	0;
		siSkillUnique	=	0;
		siSkillLevel	= 	0;
		siMustClearQuestUnique = 0;
	}
	
	cltQuestInitConditionInfo(SI32 level, SI32 skillunique, SI32 skilllevel, SI32 mustclearquestunique )
	{
		siLevel			=	level;
		siSkillUnique	=	skillunique;
		siSkillLevel	= 	skilllevel;
		siMustClearQuestUnique = mustclearquestunique;
	}

	void Set(cltQuestInitConditionInfo* pclinfo)
	{
		siLevel			=	pclinfo->siLevel;
		siSkillUnique	=	pclinfo->siSkillUnique;
		siSkillLevel	= 	pclinfo->siSkillLevel;
		siMustClearQuestUnique = pclinfo->siMustClearQuestUnique;
	}
};

//---------------------------------
// 한개 퀘스트의 정보. 
//---------------------------------
class cltQuestInfoCommon{
public:
	SI32 siUnique;		// 퀘스트 고유 유니크. 

	//--------------------
	// 완료조건. 
	//--------------------
	cltQuestClearConditionInfo clQuestClearConditionInfo;

	//-------------------
	// 보상 
	//--------------------
	cltQuestRewardInfo clQuestRewardInfo;

	cltQuestInfoCommon()
	{
		siUnique	= 0;
	}

	void Init()
	{
		siUnique	= 0;

		clQuestClearConditionInfo.Init();
		clQuestRewardInfo.Init();
	}

	void Set(cltQuestInfoCommon* pclinfo)
	{
		siUnique	= pclinfo->siUnique;
		clQuestClearConditionInfo.Set(&pclinfo->clQuestClearConditionInfo);
		clQuestRewardInfo.Set(&pclinfo->clQuestRewardInfo);

	}
};

class cltQuestInfo : public cltQuestInfoCommon{
public:
	SI32 siType;		// 퀘스트의 타입. 

	TCHAR szTitleText[MAX_TITLE_TEXT_SIZE];

	//--------------------
	// 초기초건 
	//--------------------
	cltQuestInitConditionInfo clInitConditionInfo;

	//--------------------
	// 중간 액션 
	//--------------------
	cltQuestMiddleInfo clMiddleInfo;


	TCHAR*	pImageFileName;

	cltQuestInfo() : cltQuestInfoCommon()
	{
		szTitleText[ 0 ] = NULL;
		siType		= 0;

		pImageFileName		= NULL;
	}
	
	~cltQuestInfo()
	{
		if(pImageFileName)
		{
			delete[] pImageFileName;
			pImageFileName = NULL;
		}
	}

	void Init()
	{
		cltQuestInfoCommon::Init();

		siType		= 0;
	
		szTitleText[ 0 ] = NULL;
		clInitConditionInfo.Init();
		clMiddleInfo.Init();
	}

	void Set(cltQuestInfo* pclinfo)
	{
		cltQuestInfoCommon::Set( (cltQuestInfoCommon*)pclinfo);

		siType = pclinfo->siType;

		MStrCpy( szTitleText, pclinfo->szTitleText, MAX_TITLE_TEXT_SIZE );
		clInitConditionInfo.Set(&pclinfo->clInitConditionInfo);
		clMiddleInfo.Set(&pclinfo->clMiddleInfo);
	}

	void Set(SI32 type, SI32 unique, cltQuestInitConditionInfo* pclinitcond, cltQuestClearConditionInfo* pclclearcond, cltQuestRewardInfo* pclreward, 
		TCHAR* titletext, TCHAR* pimagefilename);
};

class cltMissionQuestInfo : public cltQuestInfoCommon
{
private:
	SI32	m_siLeftTimeSec;

public:
	cltMissionQuestInfo() : cltQuestInfoCommon()
	{
		Init();
	}

	~cltMissionQuestInfo()
	{		
	}

	void Init()
	{
		cltQuestInfoCommon::Init();
		m_siLeftTimeSec		= 0;
	}

	void Set(cltMissionQuestInfo* pclinfo)
	{
		cltQuestInfoCommon::Set((cltQuestInfoCommon*)pclinfo);

		m_siLeftTimeSec	= pclinfo->m_siLeftTimeSec;
	}

	bool IsTimeLeft()
	{
		return (bool)(m_siLeftTimeSec > 0);
	}

	bool TimeReduce(SI32 siTimeSec)
	{
		// 2흉괘 제거를 위한 미션일경우엔 시간이 줄어들지 않는다. (시간제한이 없다)
		if ( MISSIONQUEST_UNIQUE_REMOVE_UNLUCK == siUnique )
		{
			return IsTimeLeft();
		}
		else
		{
			m_siLeftTimeSec -= siTimeSec;
			if(m_siLeftTimeSec < 0)		m_siLeftTimeSec = 0;

			return IsTimeLeft();
		}
	}

	inline SI32 GetLeftTime()						{		return m_siLeftTimeSec;				}
	inline void SetLeftTime(SI32 siLeftTimeSec)		{		m_siLeftTimeSec = siLeftTimeSec;	}	
};

//----------------------------------
// Person의 퀘스트 정보. 
//----------------------------------
#define MAX_WORD_ASSEMBLY_CHAR_NUMBER	10
class cltPQuestInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_WORD_ASSEMBLY_CHAR_NUMBER>	bWordAssemblySwitch;
#else
	bool bWordAssemblySwitch[MAX_WORD_ASSEMBLY_CHAR_NUMBER];
#endif

	SI32 siClearWordSpecialQuest;
	SI32 siEventClearWordSpecialQuest;		// 이벤트 기간중 단어조합 퀘스트 클리어 횟 수 저장을 위한 변수.
	SI32 siClearCount;

	// 퀘스트 정보.  
	cltQuestInfoCommon	clSpecialQuestInfo;
	cltQuestInfoCommon	clNamingQuestInfo;
	cltQuestInfoCommon	clHappyBeanQuestInfo;		//KHY - 0822 - 해피빈 퀘스트 추가.	

	// 미션 퀘스트 정보
	cltMissionQuestInfo	clMissionQuestInfo;			// 미션 퀘스트 - by LEEKH 2008-09-09

	// 신년이벤트 퀘스트 (흉점괘시)
	cltQuestInfoCommon	clRemoveUnLuckQuestInfo;	// [지연-2009.10.14]
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltPersonQuestUnitInfo, MAX_QUEST_TYPE>		clUnit;
#else
	cltPersonQuestUnitInfo clUnit[MAX_QUEST_TYPE];
#endif

	cltPQuestInfo()
	{
		SI32 i;

		for(i = 0;i < MAX_WORD_ASSEMBLY_CHAR_NUMBER;i++)
		{
			bWordAssemblySwitch[i] = false;
		}

		siClearWordSpecialQuest = 0 ;
		siEventClearWordSpecialQuest = 0;
		siClearCount = 0;

		clSpecialQuestInfo.Init();
		clNamingQuestInfo.Init();		
		clHappyBeanQuestInfo.Init();		//KHY - 0822 - 해피빈 퀘스트 추가. 강형. 일단 검색을위해.
		clMissionQuestInfo.Init();
		for(i = 0;i < MAX_QUEST_TYPE;i++)				{		clUnit[i].Init();					}
	}

	void Set(cltPQuestInfo* pclinfo)
	{
		SI32 i;

		for(i = 0;i < MAX_WORD_ASSEMBLY_CHAR_NUMBER;i++)
		{
			bWordAssemblySwitch[i] = pclinfo->bWordAssemblySwitch[i];
		}

		siClearWordSpecialQuest = pclinfo->siClearWordSpecialQuest ;
		siEventClearWordSpecialQuest = pclinfo->siEventClearWordSpecialQuest;

		clSpecialQuestInfo.Set(&pclinfo->clSpecialQuestInfo);
		clNamingQuestInfo.Set(&pclinfo->clNamingQuestInfo);		
		clHappyBeanQuestInfo.Set(&pclinfo->clHappyBeanQuestInfo);	//KHY - 0822 - 해피빈 퀘스트 추가. 강형. 일단 검색을위해.

		clMissionQuestInfo.Set(&pclinfo->clMissionQuestInfo);
		for(i = 0;i < MAX_QUEST_TYPE;i++)			{	clUnit[i].Set(&pclinfo->clUnit[i]);								}
	}

	// 특정 퀘스트가 진행중인지 확인한다 
	bool IsQuestGoing(SI32 clearcond, SI16* ptype, SI16* punique, SI16* prara1, SI16* ppara2, SI32* pstartindex = NULL);
	bool IsQuestGoing( SI32 siClearCondition );

	// 특정 컨디션, Para1/2값이 동일한 퀘스트가 있는가
	bool IsSameParaQuestGoing( SI32 siClearCond, SI16 siPara1, SI16 siPara2=0 );

	// 파라메타를 증가시킨다. 
	bool IncPara(SI32 questtype, SI32 para1, SI32 incnum);

	// 만든 물품의 수를 증가시킨다. 
	SI32 IncMakeItem(SI32 itemunique, SI32 incnum);

	// 사용한 물품의 수를 증가시킨다. 
	SI32 IncUseItem(SI32 itemunique, SI32 incnum);

	// 특정 타입의 퀘스트가 진행중이거나 수행된 것이 있는지 확인한다. 
	bool IsQuestClearOrGoing(SI32 questtype);

	// 진행중인 퀘스트를 DB에 저장한다. 
	bool SaveGoingQuestToDB(cltCharCommon* pclchar);

	// 특정 퀘스트가 끝났는가. 
	bool IsQuestEnd(SI32 questunique);

	// 특정 유니크의 퀘스트를 종료했는가. 
	bool IsClear(cltQuestManager* pclquestmanager, SI32 questunique);
	
	// 특정 타입의 퀘스트가 모두 완료되었는지 확인한다. 
	bool IsAllClear(cltQuestManager* pclquestmanager, SI32 questtype);

	// 완료된 퀘스트의 수를 구한다. 
	SI32 GetClearQuestNum(cltQuestManager* pclquestmanager, SI32 questtype);

	//-----------------------------------------
	// 단어 조합 관련 
	//-----------------------------------------
	// 흥부박 퀘스트의 단어 맞추기가  완료되었다면, 
	bool IsWordAssembleCompleted(cltQuestManager* pclQuestManager);

	// 단어 조합 정보를 초기화한다. 
	void InitWordAssemble();

	// 흥부박 퀘스트 얻은 단어 얻어오기
	bool GetWordAssembleText( cltQuestManager* pclQuestManager, TCHAR* pszBuffer, SI32 siBufferSize );
};

//---------------------------------
// 퀘스트 관리자. 
//---------------------------------
class cltQuestManager{
public:
	HWND m_hDlg;

	BOOL DlgRectInitSwitch;
	RECT DlgRect;
	HBITMAP hPortraitBitmap;
	HBITMAP hItemImage;
	SI32 siKind;				// NPC Kind
	SI32 siID;					// 대상이 되는 캐릭터. 
	SI32 siQuestType;			// 어떤 타입의 퀘스트인가 ?
	SI32 siCurrentQuestUnique;	// 현재 이 대화상자가 진행하는 퀘스트
	SI32 siCurrentQuestStep;	// 현재 진행중인 퀘스트의 단계. 
	SI32 siStartConversationNum;		// 퀘스트 받을때
	SI32 siStartConversationCounter;	// 대화 진행수
	SI32 siEndConversationNum;			// 퀘스트 클리어후
	SI32 siEndConversationCounter;		// 대화 진행수
	SI32 siViewState;			// 0: 대화진행 1: 임무관련
	BOOL bNewQuseOpened;		// 광명(임시로) 새 퀘스트창이 열리는가?
	BOOL bNewQuestDoing;		// 역관 새 퀘스트를 하느냐?

	SI32 siWordAssemblyNumber;
	TCHAR szWordAssemblyString[MAX_WORD_ASSEMBLY_CHAR_NUMBER][10];

#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<SI32, MAX_QUEST_PER_TYPE_NUMBER>, MAX_QUEST_TYPE>	siQuestBitInfo;
#else
	// 퀘스트와 비트의 연결. 
	SI32 siQuestBitInfo[MAX_QUEST_TYPE][MAX_QUEST_PER_TYPE_NUMBER];
#endif

	// 퀘스트 타입 이름. 
	TCHAR szQuestTypeName[MAX_QUEST_TYPE][128];

#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<cltQuestInfo, MAX_QUEST_PER_TYPE_NUMBER>, MAX_QUEST_TYPE>	clQuestInfo;
#else
	//  퀘스트. 
	cltQuestInfo clQuestInfo[MAX_QUEST_TYPE][MAX_QUEST_PER_TYPE_NUMBER];
#endif

	// 퀘스트 정보를 서버로 부터 받았다.
	bool bQuestInfoChangedSwitch;

	HashStringInt*		pclHashTable;

	NTextManager*		m_pQuestText;

	cltQuestManager();
	~cltQuestManager();

	// 퀘스트를 추가한다. 
	void Add(SI32 type, SI32 unique, cltQuestInitConditionInfo* pclinitcond, cltQuestClearConditionInfo* pclclearcond, cltQuestRewardInfo* pclreward, 
		TCHAR* titletext, TCHAR* pimagefilename);

	// 퀘스트의 비트를 가지고 일치하는 유니크를 찾는다.
	SI32 GetQuestUniqueFromBit(SI32 questtype, SI32 minbit);

	// 텍스트를 화면에 보여준다. 
	void ShowText(HWND hDlg, bool forceswitch);

	// 퀘스트의 유니크와 스텝에 따라 적절한 대사를 갖고 있는 파일이름을 얻어온다.
	bool  GetText(SI32 questunique, SI32 queststep, TCHAR *filename);

	// questunique를 가지고 적절한 퀘스트 정보를 찾는다. 
	cltQuestInfo* GetQuestInfo(SI32 questunique);

	// 진행중인 퀘스트를 확보한다. 
	void GetCurrentQuestInfo();

	// 퀘스트의 유니크를 근거로 해당 비트를 찾는다.
	SI32 GetBitFromUnique(SI32 questtype, SI32 questunique);
	
	// 비트를 근거로 유니크를 찾는다. 
	SI32 GetUniqueFromBit(SI32 questtype, SI32 bit);

	
	BOOL CALLBACK QuestDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	
	// 퀘스트의 유니크와 비트를 연결한다.
	bool ConnectBitWithUnique();

	// 퀘스트 텍스트를 불러온다. 
	bool LoadQuestText();

	// 퀘스트 텍스트를 불러온다. 
	bool LoadQuestText(SI32 questtype, SI32 questunique);

	// 퀘스트 정보를 보고 텍스트를 구성한다.
	bool GetCurStatusText(cltCharClient* pclchar,  SI16 questtype, cltQuestInfoCommon* pclquestinfo, TCHAR* ptext, SI16 txtSize);

	// 현재 상황에 대한 정보 텍스트를 얻어온다.
	bool GetCurStatusText(SI32 id, SI32 questtype, SI32 questunique, TCHAR* ptext, SI16 txtSize);
/*
	// 현재 진행중인 퀘스트 정보를 화면에 보여준다. 
	void ShowQuestClearInfo(SI32 id, HWND hDlg, SI32 dlgitem, SI32 sumdlgitem);

	// 현재 진행중인 퀘스트 정보를 화면에 보여준다. 
	void ShowCurrentQuestInfo(SI32 id, HWND hDlg, SI32 dlgitem, SI32 dlgitem2);
*/
	// NPCKind가 부여하는 퀘스트의 타입을 얻어온다. 
	SI32 GetQuestTypeFromNPCKind(SI32 kind);

	// 퀘스트 타입을 근거로 이 퀘스트를 부여하는 NPC의  kind를 찾는다. 
	SI32 GetNPCKindFromQuestType(SI32 questtype);
	
	// NPCKind의 퀘스트에 대한 간단한 설명.
	bool GetQuestExplain(SI32 kind, TCHAR* ptext, SI16 txtSize);
	
	// 퀘스트 타입 이름을 구한다. 
	TCHAR* GetQuestTypeName(SI32 questtype);
	
	// 퀘스트의 진행 상황을 텍스트로 얻어온다. 
	bool GetQuestProgressText(SI16 questtype, cltPQuestInfo* pclquestinfo, TCHAR* ptext, SI16 txtSize);


	// 특정 타입 퀘스트의 개수를 구한다. 
	SI32 GetQuestNum(SI32 type);

	// 흥부박 퀘스트의 글자 맞추기 단어중에서 랜덤하게 한 글자를 찾는다. 
	SI32 GetRandWordAssembly();

	// NInterface때문에 추가되는 함수들
	void InitDialog(SI32 kind);

	void ShowText(bool forceswitch);
	TCHAR* GetText(SI32 questunique, SI32 queststep);
	bool GetSprPath(SI32 questunique,SI32 queststep,SI32 * font , TCHAR * fullpathname);

	void Action();

#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<cltQuestTxtInfo, MAX_QUEST_PER_TYPE_NUMBER>, MAX_QUEST_TYPE>		clQuestTxtInfo;
#else
	cltQuestTxtInfo clQuestTxtInfo[MAX_QUEST_TYPE][MAX_QUEST_PER_TYPE_NUMBER];
#endif

	// 헬프 마우스모양
	void DrawClickMouse(LPDIRECTDRAWSURFACE7 lpSurface, SI32 unique);
	// 방향 도우미
	void DrawDirectionArrow( SI32 drawX, SI32 drawY );
	void DrawDirectionArrow( SI32 drawX, SI32 drawY, SI32 direction );

	bool GetTutorialMakeQuestItem(SI16 QuestUnique, SI32 ItemUnique, OUT cltMaterialInfo * clMaterialItem);
	void ShowSelectItemDlg( cltQuestRewardInfo* pclinfo, SI16 questtype, SI16 questunique);

	// 
	SI16 ConvertQuestTypeTextToNum( TCHAR* pszQuestType );

	// [지연] 시간제한 퀘스트의 시간 갱신 표시 : 남은시간 갱신 함수
	void UpdateRemainTime( SI32 siTimeReduce = 0 );	
};




#endif
