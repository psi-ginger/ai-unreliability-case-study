//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

// 서버와 클라이언트 등에서 공토응로 사용하는 Define

#ifndef _COMMONHEADER_H
#define _COMMONHEADER_H


#include "commonutil.h"

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>
#include <assert.h>
#include <io.h>
#include <wingdi.h>
#include <math.h>
#include <commctrl.h>

#include <directives.h>

#define NAME  "군주"
#define TITLE "군주"


//-------------------------------------------------------------
// 서비스 지역 
//-------------------------------------------------------------
const SI32 ConstServiceArea_Korea	= 	1;
const SI32 ConstServiceArea_China	=	2;
const SI32 ConstServiceArea_Taiwan	=	3;
const SI32 ConstServiceArea_Japan	=	4;
const SI32 ConstServiceArea_English	=	5;


#define NOOBJECT								0
#define OBJECT1									1


#define NEW_SELECTED_DELAY						6

#define MAX_WAIT_STEP_NUMBER					16	// 캐릭터가 가질 수 있는 최대 정지 모션 프레임의 수 
#define MAX_WAIT_TYPE							2   // 대기시 취하는 애니메이션의 최대 수 .

#define MAX_MOVE_STEP_NUMBER					10	// 캐릭터가 가질 수 있는 최대 이동 모션 프레임의 수 
#define MAX_DYING_STEP_NUMBER					16	// 캐릭터가 가질 수 있는 최대 사망 모션 프레임의 수 
#define  MAX_ATTACK_STEP_NUMBER					16	// 캐릭터가 가질 수 있는 최대 공격 모션 프레임의 수 

#define GAMECOLOR_RED						69
#define GAMECOLOR_YELLOW					70
#define GAMECOLOR_GREEN						68

//============================================================
// DIALOG BOX COLOR 정의 
//============================================================
#define COLOR_DIALOG_YELLOW					RGB(248, 255, 199)
#define COLOR_DIALOG_GREEN					RGB(136, 251, 148)
#define COLOR_DIALOG_RED					RGB(226, 114, 150)
#define COLOR_DIALOG_LIGHTRED				RGB(255, 167, 179)
#define COLOR_DIALOG_BLUE					RGB(15,  167, 240)
#define COLOR_DIALOG_BLACK					RGB( 0, 0, 0 )
#define COLOR_DIALOG_WHITE					RGB(255, 255, 255)
#define COLOR_DIALOG_LIGHTBLUE				RGB(143, 202, 233)
#define COLOR_DIALOG_WHITEBLUE				RGB(238, 238, 255)
#define COLOR_DIALOG_GRAY					RGB(224,213,213)
#define COLOR_DIALOG_DLG_BLUE				RGB(205, 205, 205)
#define COLOR_DIALOG_STATIC_BLUE			RGB(226, 222, 207)
//============================================================

#define DOUBLE_CLICK_DELAY						300

// 방향 정의 						
#define SOUTH              						1
#define SW                 						2
#define WEST               						4
#define NW										8
#define NORTH               					16
#define NE                  					32
#define EAST                					64
#define SE                  					128 
						
#define SSW										1000						
#define WWS 									1001						
#define WWN 									1002						
#define NNW 									1003						
#define NNE 									1004						
#define EEN 									1005						
#define EES 									1006
#define SSE 									1007


//----------------------------------------------------
//    캐릭터  속성 
//----------------------------------------------------
#define ATB_MOVE							0x00000001			// 움직이는 캐릭터 속성 
#define ATB_HERO							0x00000002			// 조합형캐릭터 속성.  
#define ATB_ATTACK							0x00000004			// 공격 속성
#define ATB_SHIP							0x00000008  		// 선박
#define ATB_WEAKENEMYNOATTACK				0x00000010  		// 약한 적은 공격하지 않는 속성. 
#define ATB_FISH							0x00000020  		// 낚시 가능. 
#define ATB_NOJJIM							0x00000040		// 찜되지 않는 캐릭터. 
#define ATB_MINRANGE						0x00000080		// 접근전 불가 
#define ATB_HORSE							0x00000100		// 말류. 
#define ATB_SYSTEMNPC						0x00000200		// SystemNPC
#define ATB_SELFBOMB						0x00000400		// 자폭기능
#define ATB_WAITSELFBOMB					0x00000800		// 정지시 자폭기능
#define ATB_NONPCLIST						0x00001000		// NPC리스트에 포함되지 않는다. 
#define ATB_COWORK							0x00002000		// 주변 동료들과 협동공격한다. 
#define ATB_MONSTER							0x00004000		// 몬스터
#define ATB_GOD								0x00008000		// 성황신이 될수있는 캐릭터 
#define ATB_DEFAULTGOD						0x00010000		// 성황신중 가장 기본이 되는 캐릭터 
#define ATB_DEFAULTCASTLE					0x00020000		// 성문중 가장 기본이 되는 캐릭터 
#define ATB_DAILYQUEST						0x00040000		// DailyQuest로 가능한 캐릭터
#define ATB_WARMONSTER						0x00080000		// 전쟁중에만 나오는 몬스터 - 몬스터 표시창에 표시 불가
#define ATB_EVENTXBOX						0x00100000		// EVENT 속성을 갖고 있다.
#define ATB_CLONE							0x00200000		// 분신술을 쓰도록 한다.
#define ATB_DEFAULTSUMMON					0x00400000		// 소환수가 될수 있는지 설정한다..
#define ATB_HIREABLE						0x00800000		// 마을에서 고용가능하다. 
#define ATB_BOSS							0x01000000		// 보스급 몬스터이다.
#define ATB_FARM							0x02000000		// 농사짓기가 가능하다. 
#define ATB_DAMAGELIMIT						0x04000000		// 피격시 데미지에 한계가 있다. 
#define ATB_MINE							0x08000000		// 채굴이 가능하다.



//------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------
#define TRANSPARENT_MODE_NORMAL					1
#define TRANSPARENT_MODE_OLDTRANS				2
#define TRANSPARENT_MODE_HALFTRANS				3
#define TRANSPARENT_MODE_SCREEN					4
#define TRANSPARENT_MODE_MULTIPLY				5
#define TRANSPARENT_MODE_ADD					6
#define	TRANSPARENT_MODE_ALPHA					7

//------------------------------------------------------------------------------------------------
// 신분 속성 
//------------------------------------------------------------------------------------------------
#define IDENTITY_CASTLE							1			// 신분이 성벽   인지
#define IDENTITY_VILLAGEGOD						2			// 신분이 성황신 인지
#define IDENTITY_VILLAGEGOD_SUMMON				3			// 신분이 마을의 성황신이 소환한 소환수 인지
#define IDENTITY_PERSON_SUMMON					4			// 신분이 개인이 소환한 소환수 인지 
#define IDENTITY_VILLAGEHUNT					5			// 신분이 마들단위 사냥용 몬스터 

//------------------------------------------------------------------------------------------------
// 방어 속성		
//------------------------------------------------------------------------------------------------
#define DEFENCEATB_HUMAN    					1			// 일반 병사 
#define DEFENCEATB_SWORD    					2			// 일반 보병 
#define DEFENCEATB_ARMOUR   					3			// 갑옷 
#define DEFENCEATB_HORSE    					4			// 기병 
#define DEFENCEATB_STONE    					6			// 건물 



//----------------------------------------
// 캐릭터 명령 
//----------------------------------------
#define MAX_ORDER_NUMBER				20

#define ORDER_WAIT    					1
#define ORDER_STOP    					2 
#define ORDER_FORCESTOP    				3					// 클라이언트에 전달되어져야할 강제 STOP	 
#define ORDER_MOVE    					4					// 단순이동 
#define ORDER_ATTACK  					5					// 단순 공격 
#define ORDER_ATTACK_SUMMON				6					// 소환수의 공격.  
#define ORDER_DIE     					7
#define ORDER_DEAD    					8
#define ORDER_MAGIC						9
#define ORDER_REVIVE					10
#define ORDER_STRAIGHTMOVE				11					// 직선이동. 
#define ORDER_FISH						12					// 낚시 하기. 
#define ORDER_FARM						13					// 농사짓기. 
#define ORDER_MAGIC_SUMMON				14					// 소환수의 마법 공격
#define ORDER_MINE						15					// 채굴



#define MAX_HORSE_NAME			12	// 말 이름의 길이. 
#define MAX_SUMMON_NAME			12	// 소환수 이름의 길이. 

#define MAX_HORSE_TYPE			8	// 말 품종 최대수

#define MAX_RENT_FEE			1000000
#define MAX_RENT_PERIOD			10

// 파일 이름 길이 
#define FILE_NAME_SIZE							128

#define NORMAL_TEXT_COLOR	RGB(250, 250, 250)
#define BLANK_TEXT_COLOR	RGB(10, 10, 10)


#define MAX_ORDER_QUE							50

#define MAX_CHAR_NUMBER							18500			// 게임에 등장할 수 있는 최대의 캐릭터 갯수 
#define MAX_PERSON_NUMBER						3000
#define MAX_SHIP_NUMBER							500
#define MAX_NPC_NUMBER							9500	// 건물과 캐릭터 포함함.
#define MAX_INSTANCE_NPC_NUMBER					5500		// 인스턴스 던전 NPC

#define MAX_KIND_NUMBER							220		// 총 캐릭터의 종류 갯수 
#define MAX_KIND_PER_PLAYER						10		// 주인공 캐릭터의 종류 갯수(에서 +1임! 10부터 NPC)

#define MAX_BULLET_NUMBER						1000
#define MAX_SMOKE_NUMBER						1500

#define MAX_PLAYER_NAME							20	// 사용자가 가질 수 있는 최대 이름 크기.

#define MAX_SHIP_NAME							30	// 사용자가 가질 수 있는 최대 이름 크기.

#define MAX_CHAR_PER_USER						3	// 한 유저당 가질 수 있는 캐릭터의 수. 

#define MAX_VILLAGE_NUMBER						100	// 게임에 마을이 몇개나 들어가는가 ?
#define VILLAGE_NONE							0	// 0 번마을( 마을 없음을 의미 ) 	

#define MAX_RESIDENTS_PER_VILLAGE				300 // 한 마을의 최대 주민수

#define MAX_INVEST_NUMBER						15	// 기억해야 하는 마을의 투자자 베스트 10 	

#define	MAX_CONSTRUCTION_NUM					9	// 한 마을에 지을 수 있는 건물의 최대수. 

#define MAX_HEAD_CHAT_TEXT_SIZE					128

#define MAX_CHILDINFO_NUMBER					10	// 제자 정보를 한번에 얻어올 수 있는 최대 수. 

#define WAR_REWARD_VILLAGE_NUMBER				20	// 왜구 전쟁시 20위 마을까지 소팅해서 보상한다. 

#define	MAX_CHAT_DATA_LENGTH					100	// 한글 50 자

#define MAX_PERSONAL_SHOP_TEXT_SIZE				100	// 한글 30자 개인상점 banner 

#define MAX_BLOCKREASON_SIZE					200 // 사용자 블럭시 그 사유의 텍스트 사이즈. 

#define MAX_SCROLL_SUMMON_PER_KIND				2			// 한종류의 소환수에 바를 수있는 최대 부적수

#define MAX_HORSE_LIFE							3000



//-----------------------------------
// 배 정보
//-----------------------------------
#define MAX_SHIP_PER_PERSON				3				// 한 사람이 가질수 있는 배의 최대수
#define MAX_SHIP_SUMMON_NUMBER			5				// 배에 들어갈 소환수의 최대수
// 배를 사용할 수 있는 최하 레벨
#define MIN_USING_SHIP_LEVEL			50

enum { ShipPos_Start = -2, ShipPos_HouseUnit, ShipPos_Field, ShipPos_Port };
enum { SHIP_JOB_START, SHIP_JOB_PASSENGER, SHIP_JOB_GUNNER, SHIP_JOB_OWNER, SHIP_JOB_END };
enum { SHIP_SUMMON_JOB_START, SHIP_SUMMON_JOB_BOATMAN, SHIP_SUMMON_JOB_GUNNER, SHIP_SUMMON_JOB_CANNON, SHIP_SUMMON_JOB_END };


//-----------------------------------
// 감옥위치 정보 
//-----------------------------------
#define PRISON_MAPINDEX	0
#define PRISON_X		379
#define PRISON_Y		503


// PC인가 ?
#define IsPC(x)			(x > 0 && x < MAX_PERSON_NUMBER)
// Ship인가 ?
#define IsShip(x)		(x >= MAX_PERSON_NUMBER && x < (MAX_PERSON_NUMBER + MAX_SHIP_NUMBER))
// InstanceNPC인가?
#define IsInstanceNPC(x)	(x >= (MAX_PERSON_NUMBER + MAX_SHIP_NUMBER) && x < (MAX_PERSON_NUMBER + MAX_SHIP_NUMBER+MAX_INSTANCE_NPC_NUMBER))
// NPC인가 ?
#define IsNPC(x)		(x >= (MAX_PERSON_NUMBER + MAX_SHIP_NUMBER+MAX_INSTANCE_NPC_NUMBER) && x < MAX_CHAR_NUMBER )

#define NETWORK_SERVER		1
#define NETWORK_CLIENT		2

enum eGameMode{
	GAMEMODE_SERVER	= 1	,
	GAMEMODE_CLIENT		,
	GAMEMODE_MAPEDIT
};

#define BY_PLAYER     0		
#define BY_COMPUTER   1

#define MIN_ITEM_PRICE	20			// 아이템 가격의 최소값(이 이하로는 떨어지지 않는다.) 

#define MAX_MATERIAL_NUMBER	10		// 아이템 재료 종류 최대값

#define FATHER_LEVEL	20
#define INIT_LEVEL		10			// 초기화 되는 레벨 

//--------------------------
// 신체 기관 
//--------------------------
#define ORG_1			1			// 소화계 
#define ORG_2			2			// 순환계 
#define ORG_3			3			// 호흡계 
#define ORG_4			4			// 신경계 


#define MAX_LEVEL_FOR_BEGINNERMAP	10		// 초보자 사냥터 출입을 위한 최대 레벨 

//----------------------------------------
// 직급의 타입 
//----------------------------------------
#define MAX_RANKTYPE_NUMBER	20

#define RANKTYPE_CITYHALL				1		// 대행수
#define RANKTYPE_BANK					2		// 전장행수 
#define RANKTYPE_STOCK					3		// 여각행수 
#define RANKTYPE_MARKET					4		// 객주행수
#define RANKTYPE_HOUSE					5		// 시전행수
#define RANKTYPE_HUNT					6		// 수렵장행수 
#define RANKTYPE_POSTOFFICE				7		// 우체국행수
#define RANKTYPE_PORT					8		// 포구행수
#define RANKTYPE_HORSEMARKET			9		// 사복시행수
#define RANKTYPE_REALESTATEMARKET		10		// 거간행수
#define RANKTYPE_SUMMONMARKET			11		// 장예원행수
#define RANKTYPE_SHIPMARKET				12		// 전함사행수 
#define RANKTYPE_LAND					13		// 농장행수  
#define RANKTYPE_FEAST					14		// 내자시행수
#define RANKTYPE_SUMMONHEROMARKET		15		// 비변사행수
#define RANKTYPE_MINE					16		// 광산행수
#define RANKTYPE_GUILD					17		// 상단행수 	

// 지도의 최대 개수. 
#define MAX_MAP_NUMBER					400

#define MAX_DUR_FOR_TUNE	200		// 개조에 필요한 최대 내구도(이 수치 이하이어야 한다.) 

//------------------------------------
// 마을 수익 관련 정의 
//------------------------------------
#define VILLAGE_PROFIT_RATE_FOR_MERCHANT	5	// 상인의 거래 결과중 몇 %를 마을 수익으로 집계하는가. 
#define VILLAGE_PROFIT_RATE_FOR_DAILYQUEST	5	// 모병관 퀘스트  결과중 몇 %를 마을 수익으로 집계하는가. 
#define VILLAGE_PROFIT_RATE_FOR_NPCREPAIR	5	// 장영실 수리 결과중 몇 %를 마을 수익으로 집계하는가. 
#define VILLAGE_PROFIT_FOR_WENNYOLDMAN		5000
#define VILLAGE_PROFIT_FOR_FANPRIEST		2000
#define VILLAGE_PROFIT_FOR_PERSONALSHOP     5000
#define VILLAGE_PROFIT_FOR_INVESTMENT		5000 // 투자상담가(봉이 김선달)

#define MAX_EVENT_NUMBER					6		// 이벤트의 개수. 
#define MIN_LEVEL_FOR_HORSE					20		// 말을 소유하기 위한 최소 레벨 

//--------------------------------
// 지도의 계절 
//--------------------------------
#define SEASON_SPRING		0
#define SEASON_AUTUMN		1
#define SEASON_WINTER		2


//------------------------------
// 마을의 타입 
//------------------------------
#define VILLAGETYPE_SPRING_NORMAL		1
#define VILLAGETYPE_SPRING_PORT			2
#define VILLAGETYPE_AUTUMN_NORMAL		3
#define VILLAGETYPE_AUTUMN_PORT			4
#define VILLAGETYPE_WINTER_NORMAL		5
#define VILLAGETYPE_WINTER_PORT			6

//-----------------------------------------------------------------------------
// Data x가 값이 들어있는지 없는지 검사 있으면 true(TRUE) , 없으면 false(FALSE)
//-----------------------------------------------------------------------------
#define DATA_TO_BOOL(x)				(x!=0)?(x>0):false?true:false
#define DATA_TO_TYPE_BOOL(x)		(x!=0)?(x>0):FALSE?TRUE:FALSE

//-----------------------------------------------------------------------------
// 공조가 사용할 정보들

enum
{
	GONGZO_SKILL_START = 1 ,
	GONGZO_SKILL_SWORD = GONGZO_SKILL_START ,
	GONGZO_SKILL_SPEAR ,
	GONGZO_SKILL_STAFF ,
	GONGZO_SKILL_AXE ,
	GONGZO_SKILL_BOW ,
	GONGZO_SKILL_GUN ,
	GONGZO_SKILL_LAST = GONGZO_SKILL_GUN ,
	GONGZO_SKILL_TOTAL,
};

struct GongzoMember
{
	SI16	siPriceValue[GONGZO_SKILL_TOTAL];
	SI16	siSkillValue[GONGZO_SKILL_TOTAL];
};

#define MAX_INVESTMENT_ITEM_NUMBER	100 // 투자상담가가 보여줄 투자 종목 총 갯수

struct InvestmentLines
{
	SI32	siInvestmentItemUnique; // 아이템의 유니크
	SI32	siInvestmentItemPrice; // 아이템의 가격

	SI32	siMarginalProfit; // 차익

	SI32	siMaterialItemPrice[MAX_MATERIAL_NUMBER];
};

//-------------------------------------------
// 돈 
//-------------------------------------------
#define MAX_MONEY	99999999	// 돈의 최대 단위. 
class cltMoney{
public:
	SI64 itMoney;

	void Init()
	{
		itMoney = 0;
	}

	void Set(cltMoney* pclmoney)
	{
		if(pclmoney == NULL)return;

		Set(pclmoney->itMoney);

	}

	void Set(SI64 money)
	{
		itMoney = money;
	}
	
	cltMoney Minus(cltMoney* pclmoney)
	{
		cltMoney clmoney;
		clmoney.Set(this);
		clmoney.Decrease(pclmoney);

		return clmoney;
	}

	cltMoney Plus(cltMoney* pclmoney)
	{
		cltMoney clmoney;
		clmoney.Set(this);
		clmoney.Increase(pclmoney);

		return clmoney;
	}

	void Increase(cltMoney* pclmoney)
	{
		if(pclmoney == NULL)return ;

		itMoney += pclmoney->itMoney;
		if(itMoney > 2000000000)itMoney = 2000000000;
	}

	void Decrease(cltMoney* pclmoney)
	{
		if(pclmoney == NULL)return ;

		if(itMoney >=  pclmoney->itMoney)
		{
			itMoney -= pclmoney->itMoney;
		}
		else
		{
			itMoney = 0;
		}
	}

	// itMoney에서 pclmoney가 차지하는 비율. 
	SI32 GetPercent(cltMoney* pclmoney)
	{
		if(itMoney)
		{
			SI64 percent = (pclmoney->itMoney * 100) / itMoney;
			return (SI32)percent;
		}

		return 0;
	}

	void Clear()
	{
		itMoney = 0;
	}

	bool Empty()
	{
		if(itMoney == 0)return true;

		return false;
	}

	bool Large(cltMoney* pclmoney)
	{
		if(itMoney > pclmoney->itMoney)return true;
		return false;
	}

	bool Small(cltMoney* pclmoney)
	{
		if(itMoney < pclmoney->itMoney)return true;
		return false;
	}

};


//-------------------------------------------
// 위치 정보 
//-------------------------------------------
class cltPos{
public:
	SI32 siX;
	SI32 siY;

	cltPos()
	{
		siX = 0; siY = 0;
	}
	cltPos(SI32 x, SI32 y)
	{
		siX = x; siY = y;
	}

	void Init()
	{
		siX = 0;
		siY = 0;
	}

	void Set(const cltPos* pclpos)
	{
		siX = pclpos->siX;
		siY	= pclpos->siY;
	}
	void Set(SI32 six, SI32 siy)
	{
		siX	= six;
		siY	= siy;
	}
};

// 워프 정보. 
class cltWarpInfo{
public:
	UI32 uiWarpSwitch:		1;
	UI32 uiMapIndex:		9;
	UI32 uiForceSwitch:		1;		// ㅈ주변자리 검색 없이 바로 그자리로 워프 
	UI32 uiTicketUseSwitch:	1;		// 유료 아이템을 사용한 워프. (특수 효과를 보여줘야 한다) 
	UI32 uiX:				10;
	UI32 uiY:				10; 

	SI16	siGateX, siGateY;	// 게이트 위치. (게이트가 따로 없을때는 -1) 

public:

	cltWarpInfo()
	{
		Init();
	}

	cltWarpInfo(UI32 warpswitch, UI32 mapindex, UI32 forceswitch, UI32 ticketuse, UI32 x, UI32 y, SI16 gatex, SI16 gatey)
	{
		uiWarpSwitch		= warpswitch;
		uiMapIndex			= mapindex;
		uiForceSwitch		= forceswitch;
		uiTicketUseSwitch	=ticketuse;
		uiX					= x;
		uiY					= y;
		siGateX				= gatex;
		siGateY				= gatey;
	}

	void Set(const cltWarpInfo* pclinfo)
	{
		uiWarpSwitch		= pclinfo->uiWarpSwitch;
		uiMapIndex			= pclinfo->uiMapIndex;
		uiForceSwitch		= pclinfo->uiForceSwitch;
		uiTicketUseSwitch	= pclinfo->uiTicketUseSwitch;
		uiX					= pclinfo->uiX;
		uiY					= pclinfo->uiY;
		siGateX				= pclinfo->siGateX;
		siGateY				= pclinfo->siGateY;
	}

	void Init()
	{
		uiWarpSwitch		= 0;
		uiMapIndex			= 0;
		uiX					= 0;
		uiY					= 0;
		uiForceSwitch		= 0;
		uiTicketUseSwitch	= 0;
		siGateX				= -1;
		siGateY				= -1;
	}

	UI32 GetWarpSwitch()const{return uiWarpSwitch;}
	void SetWarpSwitch(UI32 mode){uiWarpSwitch = mode;}

	UI32 GetMapIndex()const{return uiMapIndex;}
	UI32 GetX()const{return uiX;}
	UI32 GetY()const{return uiY;}

	SI16 GetGateX()const{return siGateX;}
	SI16 GetGateY()const{return siGateY;}
};


//---------------------------------
// 간단한 Person정보 
//---------------------------------
class cltSimplePerson{
public:
	SI32 siPersonID;
	char szName[MAX_PLAYER_NAME];


	cltSimplePerson()
	{
		Init();
	}

	cltSimplePerson(SI32 personid, const char* pname)
	{
		Set(personid, pname);
	}

	void Set(const cltSimplePerson* pinfo)
	{
		siPersonID	= pinfo->GetPersonID();
		MStrCpy(szName, pinfo->GetName(), MAX_PLAYER_NAME );
	}

	void Set(SI32 personid, const char* pname)
	{
		siPersonID = personid;
		MStrCpy(szName, pname, MAX_PLAYER_NAME );
	}

	void Init()
	{
		siPersonID = 0;
		MStrCpy(szName, "NOPERSONNAME", MAX_PLAYER_NAME );
	}

	SI32 GetPersonID()const{return siPersonID;}
	void SetPersonID(SI32 id){ siPersonID = id;}

	const char* GetName()const{return szName;}
	void SetName(const char* pname)
	{
		MStrCpy(szName, pname, MAX_PLAYER_NAME );
	}

};



//-------------------------------
// 캐릭터의 위치 정보 
//-------------------------------
class cltCharPos{
public:
	SI16 siMapIndex;	// 캐릭터의 위치.(지도 인덱스)  
	SI16 siX;			// 캐릭터의 위치 (지도 좌표 ) 
	SI16 siY;			// 캐릭터의 위치 (지도 좌표 ) 
	SI16 siReserved;

public:
	cltCharPos()
	{
		Init();
	}

	cltCharPos(SI32 mapindex, SI32 x, SI32 y)
	{
		siMapIndex	= mapindex;
		siX			= x;
		siY			= y;
	}

	void Set(const cltCharPos* pclpos)
	{
		memcpy(this, pclpos, sizeof(cltCharPos));
	}

	void Init()
	{
		siMapIndex	= 0;
		siX			= 0;
		siY			= 0;
	}

	void SetMapIndex(SI32 index){siMapIndex = index;}
	SI32 GetMapIndex()const{return siMapIndex;}

	void SetX(SI32 x){siX = x;}
	SI32 GetX()const{return siX;}

	void SetY(SI32 y){siY = y;}
	SI32 GetY()const{return siY;}
};


//----------------------------------
// 가장 간단한 형태의 신분 정보 
//----------------------------------
class cltSimpleRank{
public:
	SI08 siType;			// 직급의 타입. 
	SI08 siVillageUnique;	// 마을과 관련된 직급일 경우 해당 마을. 

public:

	cltSimpleRank()
	{
		Init();
	}
	
	cltSimpleRank(SI32 ranktype, SI32 villageunique)
	{
		siType			= ranktype;
		siVillageUnique = villageunique;
	}
	cltSimpleRank(cltSimpleRank* pclrank)
	{
		memcpy(this, pclrank, sizeof(cltSimpleRank));
	}


	void Init()
	{
		siType			= 0;
		siVillageUnique = 0;
	}

	void Set(SI08 ranktype, SI08 villageunique)
	{
		siType			= ranktype;
		siVillageUnique = villageunique;
	}

	void Set(const cltSimpleRank* pinfo)
	{
		memcpy(this, pinfo, sizeof(cltSimpleRank));
	}

	void SetRankType(SI08 type){siType = type;}
	SI32 GetRankType()const{return siType;}

	void SetVillageUnique(SI08 villageunique){siVillageUnique = villageunique;}
	SI32 GetVillageUnique()const{return siVillageUnique;}

	BOOL IsSame(cltSimpleRank* pclrank)const
	{
		if(pclrank == NULL)return FALSE;

		if(siType == pclrank->GetRankType() && siVillageUnique == pclrank->GetVillageUnique())
			return TRUE;

		return FALSE;
	}
};

//----------------------------------
// 가장 간단한 형태의 길드 신분 정보 
//----------------------------------
class cltSimpleGuildRank{
public:
	SI08 siType;			// 직급의 타입. 
	SI08 siVillageUnique;	// 길드 소속 마을
	SI08 siGuildUnitIndex;	// 길드 인덱스 

public:

	cltSimpleGuildRank()
	{
		Init();
	}
	
	cltSimpleGuildRank( SI32 ranktype, SI32 villageunique, SI32 guildunitindex )
	{
		siType			 = ranktype;
		siVillageUnique  = villageunique;
		siGuildUnitIndex = guildunitindex;
	}

	cltSimpleGuildRank( cltSimpleGuildRank* pclrank )
	{
		siType		  	 = pclrank->siType;
		siVillageUnique  = pclrank->siVillageUnique;
		siGuildUnitIndex = pclrank->siGuildUnitIndex;
	}

	void Init()
	{
		siType			= 0;
		siVillageUnique = 0;
		siGuildUnitIndex = 0;
	}

	void Set( SI08 ranktype, SI08 villageunique, SI08 guildunitindex )
	{
		siType			 = ranktype;
		siVillageUnique  = villageunique;
		siGuildUnitIndex = guildunitindex; 
	}

	void Set(const cltSimpleGuildRank* pclrank)
	{
		siType		  	 = pclrank->siType;
		siVillageUnique  = pclrank->siVillageUnique;
		siGuildUnitIndex = pclrank->siGuildUnitIndex;
	}

	void SetRankType(SI08 type){siType = type;}
	SI32 GetRankType()const{return siType;}

	void SetVillageUnique(SI08 villageunique){siVillageUnique = villageunique;}
	SI32 GetVillageUnique()const{return siVillageUnique;}

	void SetGuildUnitIndex(SI08 guildunitindex){siGuildUnitIndex = guildunitindex;}
	SI32 GetGuildUnitIndex()const{return siGuildUnitIndex;}

	BOOL IsSame(cltSimpleGuildRank* pclrank)const
	{
		if(pclrank == NULL)return FALSE;

		if(siType == pclrank->siType && 
		   siVillageUnique == pclrank->siVillageUnique &&
		   siGuildUnitIndex == pclrank->siGuildUnitIndex )
			return TRUE;

		return FALSE;
	}
};


//----------------------------------
// 계좌번호 클래스 
//---------------------------------
class cltAccountNum{
public:
	SI32 siVillageUnique;	// 통장이 있는 마을의 유니크. 
	SI32 siAccountNum;		// 계좌번호. 


	cltAccountNum()
	{
		Init();
	}

	cltAccountNum(SI32 villageunique, SI32 accountnum)
	{
		siVillageUnique = villageunique;
		siAccountNum	= accountnum;

	}


	void Init()
	{
		siVillageUnique		= 0;
		siAccountNum		= 0;	
	}

	void Set(const cltAccountNum* pclaccountnum)
	{
		memcpy(this, pclaccountnum, sizeof(cltAccountNum));
	}

	// 이 통장 계좌가 유효한가?
	BOOL IsValid()
	{
		if(siAccountNum)return TRUE;
		return FALSE;
	}

	bool IsSame(cltAccountNum* pclnum)
	{
		if(siVillageUnique != pclnum->siVillageUnique)return false;

		if(siAccountNum != pclnum->siAccountNum)return false;

		return true;
	}

	void SetVillageUnique(SI32 villageunique){siVillageUnique = villageunique; }
	SI32 GetVillageUnique()const{return siVillageUnique;}

	void SetAccountNum(SI32 num){siAccountNum = num;}
	SI32 GetAccountNum()const{return siAccountNum;}

};

//-------------------------------------
// 신분을 가진 사람의 정보. 
// (대행수, 전장행수, 여각행수 등등...
//-------------------------------------
class cltRankPerson{
public:
	SI32 siVillageUnique;
	cltSimplePerson	clPerson;

	cltRankPerson()
	{
		siVillageUnique = 0;
	};

	cltRankPerson(SI32 sivillageunique, const cltSimplePerson* pinfo)
	{
		siVillageUnique = sivillageunique;
		clPerson.Set(pinfo);
	}

	void Set(const cltRankPerson* pclinfo)
	{
		memcpy(this, pclinfo, sizeof(cltRankPerson));
	}
	void Set(SI32 sivillageunique, const cltSimplePerson* pinfo)
	{
		siVillageUnique = sivillageunique;
		clPerson.Set(pinfo);
	}


	void Init(SI32 villageunique)
	{
		siVillageUnique = villageunique;
		clPerson.Init();

	}

	SI32 GetPersonID()const{return clPerson.GetPersonID();}
	SI32 GetVillageUnique()const{return siVillageUnique;}
};


//------------------------------------------
// 서버가 클라이언트로 보내는 리턴값 클래스 
//------------------------------------------
class cltServerResponseValue{
public:
	SI16 siReceiver;
	SI16 siReturnValue;
	SI16 siPara1;
	SI16 siPara2;

	cltServerResponseValue()
	{
		siReceiver		= 0;
		siReturnValue	= 0;
		siPara1			= 0;
		siPara2			= 0;
	}

	cltServerResponseValue(SI16 receiver, SI16 value, SI16 sipara1, SI16 sipara2)
	{
		siReceiver		= receiver;
		siReturnValue	= value;
		siPara1			= sipara1;
		siPara2			= sipara2;
	}
};

#define MIN_FEE	100
#define MAX_FEE	100000
#define MAX_FEE_RATE	100		// 10%

//-----------------------------------------
// 수수료 관리 
//-----------------------------------------
class cltFee{
private:
	SI32 siFeeRate;		// 타행 입출금 수수료 (퍼밀) 
	SI32 siMinFee;		// 최저 수수료. 
	SI32 siMaxFee;		// 최대 수수료. 	
public:

	void Set(cltFee* pclfee)
	{
		memcpy(this, pclfee, sizeof(cltFee));

		if( siFeeRate < 10 ) siFeeRate = 10;
	}

	// 거래 금액을 근거로 수수료를 계산한다. 
	SI32 CalcFee(SI32 money)
	{
		//return max(siMinFee, min(siMaxFee, money * siFeeRate / 1000)) ;

		if( siFeeRate < 10 ) siFeeRate = 10;

		SI32 ret = (int)( money * ( siFeeRate / 1000. ) );
		
		if( ret < 0 ) return 0;
		return ret;
	}

	BOOL SetFeeRate(SI32 rate)
	{
		if( rate < 10 ) { siFeeRate = 10; return FALSE; }
		if( rate > 200 ) { siFeeRate = 200; return FALSE; }
		siFeeRate = rate;
		return TRUE;
	}
	SI32 GetFeeRate()const{return max(10, siFeeRate);}

	BOOL SetMinFee(SI32 fee)
	{
		if(fee < 0 || fee > MAX_FEE)return FALSE;

		siMinFee = fee;
		return TRUE;
	}
	SI32 GetMinFee()const{return siMinFee;}


	BOOL SetMaxFee(SI32 fee)
	{
		if(fee < 0 || fee > MAX_FEE || fee < siMinFee)return FALSE;

		siMaxFee = fee;
		return TRUE;
	}
	SI32 GetMaxFee()const{return siMaxFee;}


	// 현재 수수료가 적절한지 확인한다. 
	BOOL IsValid()
	{
		if(siFeeRate < 10 || siFeeRate > MAX_FEE_RATE)return FALSE;
		return TRUE;
	}


};

//-------------------------------------
// 로그인요청 정보 
//-------------------------------------
class cltLogInInfo{
public:
	SI32	siAuthKey;
	char	szID[MAX_PLAYER_NAME];			// 아이디의 포인터. 
	char	szPassword[MAX_PLAYER_NAME];	// 암호의 포인터. 
	UI08	uiGameMode;						// 현재 클라이언트 게임 모드 : 0 => 윈도우 창 + 윈도우 인터페이스모드, 
											// 1 => 윈도우 + 새인터페이스 모드, 2 => 전체화면 + 새인터페이스 모드

	cltLogInInfo(SI32	authkey, char* id, char* password, UI08 gamemode)
	{
		siAuthKey	= authkey;
		MStrCpy(szID,	id,	MAX_PLAYER_NAME );
		MStrCpy(szPassword, password,	MAX_PLAYER_NAME );
		uiGameMode = gamemode;
	}
};


//-------------------------------
// 캐릭터에게 주어지는 명령 
//-------------------------------
class cltOrderDataInfo{
public:
	SI08 siOrder;             // 수행해야 할 명령 코드 
	SI08 siHostMode;          // 명령을 내린 주체 (플레이어 또는 컴퓨터 )

	SI32 siPara1;					// 명령에 딸려 가는 파라메타 
	SI16 siPara2;				// 명령에 딸려 가는 파라메타 
	SI16 siPara3;				// 명령에 딸려 가는 파라메타 

	cltOrderDataInfo()
	{
		Init();
	}

	cltOrderDataInfo(SI08 order, SI08 hostmode, SI32 para1, SI16 para2, SI16 para3)
	{
		Make(order, hostmode, para1, para2, para3);
	}

	cltOrderDataInfo(cltOrderDataInfo* pclinfo)
	{
		Set(pclinfo);
	}

	void Init()
	{
		siOrder		= ORDER_WAIT;
		siHostMode	= 0;
		siPara1		= 0;
		siPara2		= 0;
		siPara3		= 0;
	}

	void Make(SI08 order, SI08 hostmode, SI32 para1, SI16 para2, SI16 para3)
	{
		siOrder		= order;
		siHostMode	= hostmode;

		siPara1		= para1;
		siPara2		= para2;
		siPara3		= para3;

	}

	void Set(cltOrderDataInfo* pclorder)
	{
		siOrder		=	pclorder->siOrder;
		siHostMode	=	pclorder->siHostMode;
		siPara1		=	pclorder->siPara1;
		siPara2		=	pclorder->siPara2;
		siPara3		=	pclorder->siPara3;
	}

};

// 명령의 실체 
// 컴퓨터가 실제로 전송하는 명령의 데이터 
class cltOrderData {
public:
    cltOrderDataInfo ODI;

	SI32 CharUnique;					 // 명령을 수행해야 할 캐릭터의 CharUnique

	cltOrderData(){ZeroMemory(this, sizeof(cltOrderData));};

	void Make(SI32 order, SI32 charunique, SI32 para1, SI16 para2, SI16 para3, char hostmode)
	{
		ODI.Make(order, hostmode, para1, para2, para3);

		CharUnique		 = charunique;
	}
	void Make(SI32 charunique, cltOrderDataInfo* pclinfo)
	{
		CharUnique = charunique;
		ODI.Set(pclinfo);
	}
	
};

class cltShapeEquipInfoUnit{
public:
	UI32	uiUnique:	16;
	UI32	uiRare:		3;
	UI32	uiReserved:	13;

	cltShapeEquipInfoUnit()
	{
		Init();
	}
	cltShapeEquipInfoUnit(UI32 unique, UI32 rare)
	{
		uiUnique	= unique;
		uiRare		= rare;
		uiReserved	= 0;

	}
	void Init()
	{
		uiUnique	= 0;
		uiRare		= 0;
		uiReserved	= 0;
	}
	void Set(cltShapeEquipInfoUnit* pclinfo)
	{
		uiUnique	= pclinfo->uiUnique;
		uiRare		= pclinfo->uiRare;
		uiReserved	= pclinfo->uiReserved;
	}

};
// 외부 장비품 정보.
class cltShapeEquipInfo{
public:
	cltShapeEquipInfoUnit clArmour;
	cltShapeEquipInfoUnit clHelmet;
	cltShapeEquipInfoUnit clMantle;
	cltShapeEquipInfoUnit clWeapon;

	UI32	uiSwordScroll:	1;
	UI32	uiSpearScroll:	1;
	UI32	uiAxeScroll:	1;
	UI32	uiGunScroll:	1;
	UI32	uiBowScroll:	1;
	UI32	uiStaffScroll:	1;
	UI32	uiCannonScroll:	1;

	SI08	siSwordScrollNum;
	SI08	siSpearScrollNum;
	SI08	siAxeScrollNum;
	SI08	siGunScrollNum;
	SI08	siBowScrollNum;
	SI08	siStaffScrollNum;
	
	cltShapeEquipInfo()
	{
		Init();
	}

	cltShapeEquipInfo(cltShapeEquipInfoUnit* pclArmour, cltShapeEquipInfoUnit* pclHelmet, cltShapeEquipInfoUnit* pclMantle, cltShapeEquipInfoUnit* pclWeapon,
		bool bsword, bool bspear, bool baxe, bool bgun, bool bbow, bool bstaff, bool bcannon,
		SI08 SwordScrollNum, SI08 SpearScrollNum, SI08 AxeScrollNum,
		SI08 GunScrollNum, SI08 BowScrollNum, SI08 StaffScrollNum)
	{
		clArmour.Set(pclArmour);
		clHelmet.Set(pclHelmet);
		clMantle.Set(pclMantle);
		clWeapon.Set(pclWeapon);

		uiSwordScroll	= bsword;
		uiSpearScroll	= bspear;
		uiAxeScroll		= baxe;	
		uiGunScroll		= bgun;		
		uiBowScroll		= bbow;		
		uiStaffScroll	= bstaff;
		uiCannonScroll	= bcannon;

		siSwordScrollNum = SwordScrollNum;
		siSpearScrollNum = SpearScrollNum;
		siAxeScrollNum = AxeScrollNum;
		siGunScrollNum = GunScrollNum;
		siBowScrollNum = BowScrollNum;
		siStaffScrollNum = StaffScrollNum;
	}

	void Init()
	{
		clArmour.Init();
		clHelmet.Init();
		clMantle.Init();
		clWeapon.Init();

		uiSwordScroll	= 0;
		uiSpearScroll	= 0;
		uiAxeScroll		= 0;	
		uiGunScroll		= 0;		
		uiBowScroll		= 0;		
		uiStaffScroll	= 0;
		uiCannonScroll	= 0;

		siSwordScrollNum = 0;
		siSpearScrollNum = 0;
		siAxeScrollNum = 0;
		siGunScrollNum = 0;
		siBowScrollNum = 0;
		siStaffScrollNum = 0;
	}

	void Set(cltShapeEquipInfo* pclinfo)
	{
		clArmour.Set(&pclinfo->clArmour);
		clHelmet.Set(&pclinfo->clHelmet);
		clMantle.Set(&pclinfo->clMantle);
		clWeapon.Set(&pclinfo->clWeapon);

		uiSwordScroll	= pclinfo->uiSwordScroll;
		uiSpearScroll	= pclinfo->uiSpearScroll;
		uiAxeScroll		= pclinfo->uiAxeScroll;	
		uiGunScroll		= pclinfo->uiGunScroll;		
		uiBowScroll		= pclinfo->uiBowScroll;		
		uiStaffScroll	= pclinfo->uiStaffScroll;
		uiCannonScroll	= pclinfo->uiCannonScroll;

		siSwordScrollNum  = pclinfo->siSwordScrollNum;
		siSpearScrollNum  = pclinfo->siSpearScrollNum;
		siAxeScrollNum	  = pclinfo->siAxeScrollNum;
		siGunScrollNum    = pclinfo->siGunScrollNum;
		siBowScrollNum    = pclinfo->siBowScrollNum;
		siStaffScrollNum  = pclinfo->siStaffScrollNum;
	}

};

//
#define CHANGETYPE_POISON			1
#define CHANGETYPE_ICE				2
#define CHANGETYPE_DRAIN			3
#define CHANGETYPE_HEAL				4
#define CHANGETYPE_NOFIRSTATTACK	5	// 선공 방지 효과. 
#define CHANGETYPE_NOICEATTACK		6	// 선공 방지 효과. 
#define CHANGETYPE_CURSEATTACK		7	// 
#define CHANGETYPE_CURSECRITICAL	8
#define CHANGETYPE_CURSEDEFENSE		9	// 
#define CHANGETYPE_CURSEMOVESPEED	10
#define CHANGETYPE_ANGEL			11	// 수호천사 여부 변경. 
#define CHANGETYPE_AUTOFOOD			12	// 자동 말먹이 설정 변경 여부. 
#define CHANGETYPE_TOPSKILLUNIQUE	13	// 최고장인의 기술 
#define CHANGETYPE_MAGICFIRE		14	// 불해치의 마법 공격의 불
#define CHANGETYPE_FROZEN			15	// 얼음성 보스의 얼림 공격

//----------------------------------------
// 캐릭터들의 변경 정보들을 위한 정보
//-----------------------------------------
class cltCharChangeStatusInfo{
public:
	SI32	siChangeType;
	bool	bChangeSwitch;
	SI32	siChageAmount1;
	SI32	siChageAmount2;
	SI32	siChageAmount3;

	cltCharChangeStatusInfo ()
	{
		siChangeType	= 0;
		bChangeSwitch	= false;
		siChageAmount1	= 0;
		siChageAmount2	= 0;
		siChageAmount3	= 0;
	};

	cltCharChangeStatusInfo (SI32 changetype, bool bswitch, SI32 amount1 = 0, SI32 amount2 =0, SI32 amount3 = 0 )
	{
		siChangeType	= changetype;
		bChangeSwitch	= bswitch;
		siChageAmount1	= amount1;
		siChageAmount2  = amount2;
		siChageAmount3  = amount3;
	};
	void Set(cltCharChangeStatusInfo *pclInfo)
	{
		siChangeType	= pclInfo->siChangeType;
		bChangeSwitch	= pclInfo->bChangeSwitch;
		siChageAmount1	= pclInfo->siChageAmount1;
		siChageAmount2  = pclInfo->siChageAmount2;
		siChageAmount3  = pclInfo->siChageAmount3;

	};
};

//----------------------------------------
// 캐릭터들의 부가정보들을 위한 정보
//-----------------------------------------
class cltCharAddStatusInfo{
public:
	SI16			siReserved[2];

/*	SI08			siIdentity;
	SI16			siHome;							// 성황신일 경우 자신이 관할하는 마을의 Unique 값
	SI16			siCapa;							// 성황신의 성능지수 

	SI32			siChildCharUnique;				// 소환수를 갖고 있다면 소환수의 Unique
	SI32			siParentCharUnique;				// 자신이 소환수이라면 부모의 Unique
	SI32			siParentPersonID;				// 자신이 소환수이라면 부모의 PersonID : 확실하게 비교하기 위해서리
	SI16			siSummonIndex;					// 자신이 소환수라면 소환수 Index	
*/

	cltCharAddStatusInfo ()
	{
/*		siIdentity			= 0;
		siHome				= 0;
		siCapa				= 0;
		siChildCharUnique	= 0;
		siParentCharUnique	= 0;
		siParentPersonID	= 0;
		siSummonIndex		= 0;
		*/
	};

/*	cltCharAddStatusInfo (SI08 identity, SI16 home, SI16 capa, SI32 childunique, SI32 parentunique, SI32 parentperson, SI16 summonindex)
	{
		siIdentity			= identity;
		siHome				= home;
		siCapa				= capa;
		siChildCharUnique	= childunique;
		siParentCharUnique	= parentunique;
		siParentPersonID	= parentperson;
		siSummonIndex		= summonindex;
	};
*/
	void Init()
	{
/*		siIdentity			= 0;
		siHome				= 0;
		siCapa				= 0;
		siChildCharUnique	= 0;
		siParentCharUnique	= 0;
		siParentPersonID	= 0;
		siSummonIndex		= 0;
		*/
	};

	void Set(cltCharAddStatusInfo *pclInfo)
	{
/*		siIdentity			= pclInfo->siIdentity;
		siHome				= pclInfo->siHome;
		siCapa				= pclInfo->siCapa;
		siChildCharUnique	= pclInfo->siChildCharUnique;	
		siParentCharUnique	= pclInfo->siParentCharUnique;
		siParentPersonID	= pclInfo->siParentPersonID;	
		siSummonIndex		= pclInfo->siSummonIndex;
		*/
	}
};

//------------------------------------
// Event 캐릭터 여부 정보. 
//------------------------------------
class cltEventCharInfo{
public:
	UI32 bEvent1Switch:		1;
	UI32 bEvent2Switch:		1;
	UI32 bEvent3Switch:		1;
	UI32 bEvent4Switch:		1;
	UI32 bEvent5Switch:		1;

	UI32 bOXQuizSwitch:		1;		// OX퀴즈 참여 여부. 

	cltEventCharInfo()
	{
		Init();
	}

	void Init()
	{
		bEvent1Switch	= false;
		bEvent2Switch	= false;
		bEvent3Switch	= false;
		bEvent4Switch	= false;
		bEvent5Switch	= false;

		bOXQuizSwitch	= false;
	}

	void Set(cltEventCharInfo* pclinfo)
	{
		bEvent1Switch	= pclinfo->bEvent1Switch;
		bEvent2Switch	= pclinfo->bEvent2Switch;
		bEvent3Switch	= pclinfo->bEvent3Switch;
		bEvent4Switch	= pclinfo->bEvent4Switch;
		bEvent5Switch	= pclinfo->bEvent5Switch;

		bOXQuizSwitch	= pclinfo->bOXQuizSwitch;
	}
};
//------------------------------------
// Battlezone 캐릭터 정보 
//------------------------------------
#define BATTLEZONE_READY_NONE		0
#define BATTLEZONE_READY_A			1
#define BATTLEZONE_READY_B			2
#define BATTLEZONE_READY_ALL		3
#define BATTLEZONE_READY_GAMESTART	4

#define BATTLEZONE_SIDE_NONE		0
#define BATTLEZONE_SIDE_A			1
#define BATTLEZONE_SIDE_B			2
#define BATTLEZONE_SIDE_A_OBSERVER	3
#define BATTLEZONE_SIDE_B_OBSERVER	4
#define BATTLEZONE_SIDE_A_APPLIER	5
#define BATTLEZONE_SIDE_B_APPLIER	6

class cltBattleZoneCharInfo
{
public: 
	UI32 siReady	:	3;
	UI32 bLeader	:	1;
	UI32 uiSide		:	3;
	UI32 uiMapIndex :  10;	
	UI32 uiSession  :   7;	

	cltBattleZoneCharInfo()
	{
		Init();
	}
	void Init()
	{
		siReady			= BATTLEZONE_READY_NONE;
		bLeader			= false;
		uiSide			= 0;
		uiMapIndex		= 0;
		uiSession		= 0;
	}
	void Set(cltBattleZoneCharInfo* pclinfo)
	{
		siReady			= pclinfo->siReady;
		bLeader			= pclinfo->bLeader;
		uiSide			= pclinfo->uiSide;
		uiMapIndex		= pclinfo->uiMapIndex;
		uiSession		= pclinfo->uiSession;
	}	
};

//----------------------------------------
// 캐릭터의 내부 Status 정보 
//-----------------------------------------
#define ENEMYTYPE_ALLPC							1	// 모든 사용자와 적대적. 
#define ENEMYTYPE_ALLNPC						2	// 모든 NPC와 적대적 
#define ENEMYTYPE_VILLAGE						4	// 공성관계 마을과 적대적. 
#define ENEMYTYPE_ONEVILLAGE					8	// 특정 한 마을과만 적대적 (para1에 특정 마을의 유니크 정보) 
#define ENEMYTYPE_BATTLEZONE					16	// 배틀존 적대 관계 (para1에 배틀존에서의 편 정보 ) 

class cltEnemyTypeInfo{
public:
	SI16	siEnemyType;
	SI16	siPara1;

	cltEnemyTypeInfo()
	{
		Init();
	}
	void Init()
	{
		siEnemyType = 0;
		siPara1		= 0;
	}
	void Set(cltEnemyTypeInfo* pclinfo)
	{
		siEnemyType = pclinfo->siEnemyType;
		siPara1		= pclinfo->siPara1;
	}
	void Set(SI16 type, SI16 para1)
	{
		siEnemyType = type;
		siPara1		= para1;
	}
	
	bool IsType(SI32 type)
	{
		if(siEnemyType & type)return true;

		return false;
	}


};

#define INNERSTATUS_POISON						1
#define INNERSTATUS_ICE							2
#define INNERSTATUS_CURSEATTACK					4
#define INNERSTATUS_CURSECRITICAL				8
#define INNERSTATUS_CLONE						16
#define INNERSTATUS_PRISON						32
#define INNERSTATUS_CURSEDEFENSE				64
#define INNERSTATUS_CURSEMOVESPEED				128
#define INNERSTATUS_MAGICFIRE					256 // 불해치 마법에 당해서 불 붙음
#define INNERSTATUS_FROZEN						512 // 얼음성 보스 마법에 맞아서 얼어버림

class cltInnerStatusInfo{	
public:
	SI32	siStatus;
	cltEnemyTypeInfo		clEnemyType;		// 적대 관계 정보. 
	cltEventCharInfo		clEventInfo;		// 캐릭터의 이벤트 참여 정보. 
	cltBattleZoneCharInfo	clBZInfo;

	SI32	siRideShipID;						// 사람 캐릭만 가지는 자기가 승선한 배의 ShipID
	SI32	siSelfShipID;						// 배 캐릭만 가지는 자기 자신의 ShipID

	SI16	siShipCharUnique;					// 배의 CharUnique
	SI16	siShipOwnerCharUnique;				// 배 소유주의 CharUnique
	
	SI16	siTopSkillUnique;					// 최고 장인인 기술의 유니크 

	cltInnerStatusInfo()
	{
		ZeroMemory(this, sizeof(cltInnerStatusInfo));
	};

	cltInnerStatusInfo(SI32 status,	cltEnemyTypeInfo* pclenemytype, 
		cltEventCharInfo* pclevent, cltBattleZoneCharInfo* pclbz, 
		SI32 RideShipID, SI32 SelfShipID, SI16 ShipCharUnique, SI16 ShipOwnerCharUnique, SI16 topskillunique )
	{
		siStatus		= status;
		clEnemyType.Set(pclenemytype);
		clEventInfo.Set(pclevent);
		clBZInfo.Set(pclbz);		

		siRideShipID	= RideShipID;
		siSelfShipID	= SelfShipID;
		siShipCharUnique= ShipCharUnique;
		siShipOwnerCharUnique = ShipOwnerCharUnique;
		siTopSkillUnique	= topskillunique;
	}
	
	void Set(cltInnerStatusInfo* pclinfo)
	{
		siStatus		= pclinfo->siStatus;
		clEnemyType.Set(&pclinfo->clEnemyType);
		clEventInfo.Set(&pclinfo->clEventInfo);
		clBZInfo.Set(&pclinfo->clBZInfo);		
		siRideShipID			= pclinfo->siRideShipID;
		siSelfShipID			= pclinfo->siSelfShipID;
		siShipCharUnique		= pclinfo->siShipCharUnique;
		siShipOwnerCharUnique	= pclinfo->siShipOwnerCharUnique;
		siTopSkillUnique		= pclinfo->siTopSkillUnique;
	}

	void Init()
	{
		siStatus				= 0;
		clEnemyType.Init();
		clEventInfo.Init();
		clBZInfo.Init();		
		siRideShipID			= 0;
		siSelfShipID			= 0;
		siShipCharUnique		= 0;
		siShipOwnerCharUnique	= 0;
		siTopSkillUnique		= 0;
	}
	
	bool IsStatus(SI32 status)
	{
		if(siStatus & status)return true;

		return false;
	}

	void SetStatus(SI32 status)
	{
		siStatus |= status;
	}

	void DelStatus(SI32 status)
	{
		if(siStatus & status)
		{
			siStatus -= status;
		}
	}
};

//--------------------------------
// 캐릭터 아이덴티 정보 
//--------------------------------
class cltIdentityCommonInfo{
public:
	SI16	siPara1;
	SI16	siPara2;
	SI32	siPara3;
};

class cltIdentitySummonInfo{
public:
	SI16	siMySummonIndex;
	SI16	siParentCharUnique;
	SI32	siParentPersonID;
};

class cltIdentityVillageGodInfo{
public:
	SI16	siCapa;
};

class cltIdentityVillageHuntInfo{
public:
	SI16	siHome;
	SI16	siPara2;
};



class cltIdentity{
public:
	SI08	siIdentity;

	union
	{
		cltIdentityCommonInfo		clCommonInfo;
		cltIdentitySummonInfo		clSummonInfo;
		cltIdentityVillageGodInfo	clGodInfo;
		cltIdentityVillageHuntInfo	clHuntInfo;
	};

	cltIdentity()
	{
		Init();
	}
	cltIdentity(SI08 identity, SI16 para1, SI16 para2, SI32 para3)
	{
		siIdentity	= identity;
		clCommonInfo.siPara1		= para1;
		clCommonInfo.siPara2		= para2;
		clCommonInfo.siPara3		= para3;
	}
	void Init()
	{
		siIdentity	= 0;
		clCommonInfo.siPara1		= 0;
		clCommonInfo.siPara2		= 0;
		clCommonInfo.siPara3		= 0;
	}

	void Set(cltIdentity* pclinfo)
	{
		siIdentity	= pclinfo->siIdentity;
		clCommonInfo.siPara1		= pclinfo->clCommonInfo.siPara1;
		clCommonInfo.siPara2		= pclinfo->clCommonInfo.siPara2;
		clCommonInfo.siPara3		= pclinfo->clCommonInfo.siPara3;
	}

	// 소환수를 설정한다. 
	void MakeSummon(SI32 iddentity, SI32 index, SI32 parentcharid, SI32 parentpersonid)
	{
		siIdentity			= iddentity;

		clSummonInfo.siMySummonIndex		= index;
		clSummonInfo.siParentCharUnique	= parentcharid;
		clSummonInfo.siParentPersonID	= parentpersonid;
	}

	// 성벽, 성황신, 성환신소환수를 설정한다.
	void MakeGod(SI32 iddentity, SI32 capa)
	{
		siIdentity			= iddentity;
		clGodInfo.siCapa	= capa;
	}

	// 불가사리를 설정한다.
	void MakeHunt(SI32 iddentity, SI32 villageunique)
	{
		siIdentity			= iddentity;
		clHuntInfo.siHome	= villageunique;
	}

	
};

#define MAKECHARTYPE_NPC		0
#define MAKECHARTYPE_PC			1


//----------------------------------------
// 캐릭터를 만들때 사용하는 정보. 
//-----------------------------------------
class cltMakeCharInfoCommon{
public:
	UI32	uiType:					1;		// NPC용인가 사용자 용인가. 
	UI32    uiMakeCharKind:			8;		// 만들어 져야할 캐릭터 종류. 
	UI32    uiMakeCharUnique:		16;		// 이 메시지의 결과로 만들어져야 할 캐릭터의 CharUnique
	UI32	uiLevel:				8;		// (0~255)
	UI32	uiLife:					20;		// 현재 이 캐릭터의 체력. 
	UI32	uiX:					10;
	UI32	uiY:					10;
	UI32	uiMapIndex:				10;		// 지도 인덱스 (0~2^10 = 1024)
	UI32	uiTotalMoveSpeed:		6;	
	SI08	siHomeVillageUnique;		// 주소지 마을의 유니크. 
	SI16	siJimCharUnique;			// 이 캐릭터를 찜한 캐릭터의 유니크. 
	cltOrderDataInfo	clOrder;		// 캐릭터가 만들어진 후 행해야 할 임무. 		
	cltIdentity			clIdentity;		// 아이덴티 정보. 
	cltInnerStatusInfo	clInnerStatus;	// 캐릭터의 추가정보 (독, 얼음)

	cltMakeCharInfoCommon()
	{
		Init();
	}

	cltMakeCharInfoCommon(UI32 makekind, UI32 makecharunique, SI32 home, UI32 level, 
		UI32 life, UI32 x, UI32 y, UI32 mapindex, UI32 speed, SI16 jimcharunique, 
		cltOrderDataInfo* pclorder,
		cltIdentity* pclidentity, 
		cltInnerStatusInfo* pclinnerstatus)
	{
		if(IsPC(makecharunique))
		{
			uiType				= MAKECHARTYPE_PC;
		}
		else
		{
			uiType				= MAKECHARTYPE_NPC;
		}
		
		uiMakeCharKind		= makekind;
		uiMakeCharUnique	= makecharunique;
		siHomeVillageUnique	= home;
		uiLevel				= level;
		uiLife				= life;
		uiX					= x;
		uiY					= y;
		uiMapIndex			= mapindex;
		uiTotalMoveSpeed	= speed;
		siJimCharUnique		= jimcharunique;
		if(pclorder)
		{
			clOrder.Set(pclorder);
		}
		
		if(pclidentity)
		{
			clIdentity.Set(pclidentity);
		}
		
		if(pclinnerstatus)
		{
			clInnerStatus.Set(pclinnerstatus);
		}
	}

	void Init()
	{
		uiType				= 0;
		uiMakeCharKind		= 0;
		uiMakeCharUnique	= 0;
		siHomeVillageUnique	= 0;
		uiLevel				= 0;
		uiLife				= 0;
		uiX					= 0;
		uiY					= 0;
		uiMapIndex			= 0;
		uiTotalMoveSpeed	= 0;
		siJimCharUnique		= 0;

		clOrder.Init();
		
		clIdentity.Init();
		
		clInnerStatus.Init();
	}

	void Set(cltMakeCharInfoCommon* pclinfo)
	{
		uiType				= pclinfo->uiType;
		uiMakeCharKind		= pclinfo->uiMakeCharKind;
		uiMakeCharUnique	= pclinfo->uiMakeCharUnique;
		siHomeVillageUnique	= pclinfo->siHomeVillageUnique;
		uiLevel				= pclinfo->uiLevel;
		uiLife				= pclinfo->uiLife;
		uiX					= pclinfo->uiX;
		uiY					= pclinfo->uiY;
		uiMapIndex			= pclinfo->uiMapIndex;
		uiTotalMoveSpeed	= pclinfo->uiTotalMoveSpeed;
		siJimCharUnique		= pclinfo->siJimCharUnique;
		clOrder.Set(&pclinfo->clOrder);
		clIdentity.Set(&pclinfo->clIdentity);
		clInnerStatus.Set(&pclinfo->clInnerStatus);
	}
};

class cltMakeCharInfoUser{
public:

};


class cltMakeCharInfo : public cltMakeCharInfoCommon{
public:
	//--------------------------------
	// 사용자 전용 정보 
	//--------------------------------
	UI32	uiChildCharUnique:		16;			// 보유한 소환수의 유니크. 
	UI32	uiSummonIndex:			3;			// 현재 소환수의 인덱스. 
	UI32	uiHeadChatSwitch:		1;			// 머릿말이 있는가 ?
	UI32	uiPersonalShopSwitch:	1;			// 개인상점이 있는가?
	UI32	uiNoFirstAttackSwitch:	1;			// 선공 방어가 설정되어 있는가?
	UI32	uiNoIceAttackSwitch:	1;			// ICE 방어가 설정되어 있는가?
	UI32	uiSex:					1;			// 성별. 

	
	UI32	uiHorseUnique1:			8;		// 말의 정보.
	UI32	uiSchoolUnique:			10;		// 학교의 유니크. 
	UI32	uiRequestAddinfo:		1;		// 신분의 설정 추가정보 요청 여부
	UI32	uiTicketWarpSwitch:		1;		// 유료이용권을 사용한 공간이동인가. 

	SI08	siHorseIndex;
	SI08	siGameMasterLevel;			// 운영자 권한 단계. 
	SI16	siMapUnique;				// 맵유니크. 

	cltShapeEquipInfo clShapeEquip;
	
	cltSimpleRank		clRank;

	SI32				siAddStatusDate;
	SI32				siStatusTicketNum;

	bool				bPCRoom;		// pc방 유저인가

	cltMakeCharInfo() : cltMakeCharInfoCommon()
	{
		Init();
	};


	cltMakeCharInfo(UI32 uikind, UI32 makecharunique, UI32 level, UI32 life, UI32 x, UI32 y, UI32 mapindex, 
		UI32 childcharunique, 
		UI32 summonindex, 
		UI32 bheadchatswitch, 
		UI32 bpersonalshopswitch, 
		UI32 bnofirstattackswitch,
		UI32 bnoiceattackswitch,
		UI32 bsex,
		cltShapeEquipInfo* pclequipinfo, 
		SI08 horseindex, UI32 totalmovespeed, 
		SI16 jimcharunique, 
		SI08 gamemasterlevel, 
		SI16 mapunique, 
		UI32 horseunique1, 
		UI32 homevillageunique, 
		UI32 schoolunique,
		cltSimpleRank* pclrank,SI32 addstatusdate,SI32 ticketnum, cltOrderDataInfo* pclorder, 
		cltIdentity* pclidentity,
		cltInnerStatusInfo *pclinnerstatus, UI32 requestaddinfo,
		UI32 ticketwarp,
		bool bpcroom ) 
		: cltMakeCharInfoCommon (uikind, makecharunique, homevillageunique, level, 
		life, x, y, mapindex, totalmovespeed, jimcharunique, 
		pclorder,
		pclidentity, 
		pclinnerstatus )
	{
		
		uiChildCharUnique		= childcharunique;
		uiSummonIndex			= summonindex;

		uiHeadChatSwitch		= bheadchatswitch;
		uiPersonalShopSwitch	= bpersonalshopswitch;
		
		uiNoFirstAttackSwitch	= bnofirstattackswitch;
		uiNoIceAttackSwitch		= bnoiceattackswitch;
		uiSex					= bsex;

		uiHorseUnique1		= horseunique1;
		uiSchoolUnique		= schoolunique;

		siHorseIndex		= horseindex;
		uiRequestAddinfo    = requestaddinfo;
		uiTicketWarpSwitch	= ticketwarp;


		siGameMasterLevel	= gamemasterlevel;
		siMapUnique			= mapunique;

		if(pclequipinfo)
		{
			clShapeEquip.Set(pclequipinfo);
		}

		if(pclrank)
		{
			clRank.Set(pclrank);
		}

		siAddStatusDate = addstatusdate ;
		siStatusTicketNum = ticketnum;

		bPCRoom = bpcroom;
	}

	void Init()
	{
		cltMakeCharInfoCommon::Init();
		
		uiChildCharUnique	= 0;
		uiSummonIndex	= 0;
		uiHeadChatSwitch	= 0;
		uiPersonalShopSwitch	= 0;
		uiNoFirstAttackSwitch	= 0;
		uiNoIceAttackSwitch	= 0;
		uiSex	= 0;
		
		
		uiHorseUnique1	= 0;
		uiSchoolUnique	= 0;
		uiRequestAddinfo	= 0;
		uiTicketWarpSwitch	= 0;
						
		siHorseIndex	= 0;
		siGameMasterLevel	= 0;
		siMapUnique		= 0;
		
		clShapeEquip.Init();
		
		clRank.Init();

		siAddStatusDate = 0 ;
		siStatusTicketNum = 0 ;
		
		bPCRoom = false;
	}


	void Set(cltMakeCharInfoCommon* pclinfo)
	{
		Init();
		cltMakeCharInfoCommon::Set(pclinfo);
	}
	
	void Set(cltMakeCharInfo* pclinfo)
	{
		cltMakeCharInfoCommon::Set(pclinfo);

		uiChildCharUnique	= pclinfo->uiChildCharUnique;
		uiSummonIndex		= pclinfo->uiSummonIndex;

		uiHeadChatSwitch	= pclinfo->uiHeadChatSwitch;
		uiPersonalShopSwitch= pclinfo->uiPersonalShopSwitch;

		uiNoFirstAttackSwitch	= pclinfo->uiNoFirstAttackSwitch;
		uiNoIceAttackSwitch		= pclinfo->uiNoIceAttackSwitch;
		uiSex					= pclinfo->uiSex;	

		uiHorseUnique1	 = pclinfo->uiHorseUnique1;
		uiSchoolUnique	= pclinfo->uiSchoolUnique;

		siHorseIndex		= pclinfo->siHorseIndex;
		uiRequestAddinfo	= pclinfo->uiRequestAddinfo;
		uiTicketWarpSwitch	= pclinfo->uiTicketWarpSwitch;

		siGameMasterLevel	= pclinfo->siGameMasterLevel;
		siMapUnique			= pclinfo->siMapUnique;

		clShapeEquip.Set(&pclinfo->clShapeEquip);
		
		clRank.Set(&pclinfo->clRank);

		siAddStatusDate = pclinfo->siAddStatusDate ;
		siStatusTicketNum = pclinfo->siStatusTicketNum ;

		bPCRoom = pclinfo->bPCRoom;
	}
};


class cltPFatherInfo{
public:
	cltSimplePerson	clFather;			// 스승 정보. 
	char			szAccountID[MAX_PLAYER_NAME];
	SI08			siMyLevelForFather;	// 스승이 선정된 당시의 내 레벨 
	SI08			siFatherChangeNumber;	// 스승을 변경한 횟수. 

	cltPFatherInfo()
	{
		Init();
	}

	void Init()
	{
		clFather.Init();
		szAccountID[ 0 ] = NULL;
		siMyLevelForFather		= 0;
		siFatherChangeNumber	= 0;
	}

	void Set(cltPFatherInfo* pclinfo)
	{
		clFather.Set(&pclinfo->clFather);
		MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );
		siMyLevelForFather		= pclinfo->siMyLevelForFather;
		siFatherChangeNumber	= pclinfo->siFatherChangeNumber;
	}
};

//------------------------------------
// 신분. 
//------------------------------------
#define GRANK_KING			1		// 왕 
#define GRANK_MINISTER1		2		// 이조판서 
#define GRANK_MINISTER2		3		// 호조 
#define GRANK_MINISTER3		4		// 예조
#define GRANK_MINISTER4		5		// 병조 
#define GRANK_MINISTER5		6		// 형조 
#define GRANK_MINISTER6		7		// 공조 

#define GRANK_MC_WOMAN		100		// 여자 MC
#define GRANK_MC_MAN		101		// 남자 MC

//-----------------------------------
// 가장 기본이  되는 캐릭터 정보. 
//-----------------------------------
class cltPBasicInfo{
public:
	SI32		siIDNum;						// 이 Person이 소속된 IDNum
	char		szAccountID[MAX_PLAYER_NAME];	// 이 Person이 소속된 계정의 아이디. 	
	UI08		uiIndex;		// 캐릭터 슬롯중 몇 번째 슬롯에 위치하는가 ?
	UI08		uiSex;			// 성별, (남자 1, 여자 0)
	UI08		uiKind;			// 캐릭터의 종류. 
	SI08		siHomeVillage;	// 주소지. 
	UI08		uiGRank;		// 신분. 
	UI08		uiAngelSwitch;	// 수호천사인지 여부. 
	cltCharPos	clPos;	// 캐릭터의 위치 
	SI32		siPlaySecond;	// 플레이한 총 누적 시간(초) 
	cltPos		clLastMainMapPos;	// 야외지도의 최종 위치. 
	SI16		siSchoolUnique;	// 등록 학교의 유니크. 
	
	// 재산 관련 정보. 
	SI32		siWealthRank;		// 재산순위. 
	cltMoney	clTotalWealthMoney;	// 총 재산액. 
	SI32		siWealthCalcDay;	//	재산액을 계산한 날. 

	cltSimplePerson	clPerson;	// 자신의 정보. 

	// 스승 관련 정보.
	cltPFatherInfo clFatherInfo;

	// 추천인 관련 정보
	cltSimplePerson clRecommenderInfo;	// 추천인 정보
	
	SI16	siVoteKingSelectionIndex;	// 투표한 후보의 번호	
	
	// 출석 정보 
	bool	bTodayFirstAttendanceSwitch;	// 오늘 첫번째 출석인가.	
	SI32	siAttendanceDays;				// 연속 출석일수. 

	bool	bWennyOldManUpgrade;			// 호품 한 적이 있느냐?

	bool	bNewAccount; // 특정일 이후에 가입한 계정인가?
	bool	bGameEvent; // 현재 진행되고 있는 이벤트를 수행했는가?
	bool	bWinPrizeEvent; // 현재 진행되고 있는 이벤트에 당첨되었는가?

	bool	bUseNoLimitTicketWarp ; // 무한워프 하는가?
	SI32	siNoLimitTicketWarpDateVary ; // 무한워프 기간

	bool	bPCRoom;	// PC방 유저인가?
	bool	bQuestEvolve; // 퀘스트 진화를 한적이 있느냐?

public:

	cltPBasicInfo()
	{		
		siIDNum				= 0;
		uiIndex				= 0;
		uiSex				= 0;
		uiKind				= 0;
		siHomeVillage		= 0;
		uiGRank				= 0;
		uiAngelSwitch		= 0;
		siPlaySecond		= 0;
		clLastMainMapPos.Init();
		siSchoolUnique		= 0;
		clPerson.Init();
		clFatherInfo.Init();
		clRecommenderInfo.Init();
		siVoteKingSelectionIndex = 0;

		siWealthRank		= 0;
		clTotalWealthMoney.Clear();
		siWealthCalcDay		= 0;

		szAccountID[ 0 ] = NULL;

		bTodayFirstAttendanceSwitch	= false;
		siAttendanceDays			= 0;

		bWennyOldManUpgrade = false ;

		bNewAccount = false ;
		bGameEvent = false ;
		bWinPrizeEvent = false ;

		bUseNoLimitTicketWarp = false ;
		siNoLimitTicketWarpDateVary = 0 ;

		bPCRoom = false;
		bQuestEvolve = false ;
	}

	cltPBasicInfo(char* paccountid, SI32 idnum, UI08 index, UI08 sex, cltSimplePerson* pclperson, 
		UI08 kind, SI08 homevillage, cltCharPos* pclpos, SI32 playsecond, SI16 schoolunique, 
		cltPos* pclmainmappos, cltPFatherInfo* pclfather, cltSimplePerson* pclRecommenderInfo, 
		SI16 VoteKingSelectionIndex, SI32 wealthrank, cltMoney totalmoney, SI32 wealthday,
		bool attendanceswitch, SI32 attendanceday,bool wennyoldmanupgrade,bool newaccount,
		bool gameevent , bool winprizeevent,bool nolimitticketwarp , SI32 warpticketdatevary,
		bool bpcroom ,bool bquestevolve)
	{
		MStrCpy(szAccountID, paccountid, MAX_PLAYER_NAME );

		siIDNum			= idnum;
		uiIndex			= index;
		uiSex			= sex;

		uiKind			= kind;
		siHomeVillage	= homevillage;
		uiGRank			= 0;
		uiAngelSwitch	= 0;
		clPos.Set(pclpos);
		siPlaySecond	= playsecond;
		siSchoolUnique	= schoolunique;
		clLastMainMapPos.Set(pclmainmappos);

		clPerson.Set(pclperson);

		clFatherInfo.Set(pclfather);
		clRecommenderInfo.Set(pclRecommenderInfo);
		siVoteKingSelectionIndex = VoteKingSelectionIndex;

		siWealthRank		= wealthrank; 
		clTotalWealthMoney.Set(&totalmoney);
		siWealthCalcDay		= wealthday;

		bTodayFirstAttendanceSwitch	= attendanceswitch;
		siAttendanceDays			= attendanceday;

		bWennyOldManUpgrade = wennyoldmanupgrade ;

		bNewAccount = newaccount ;
		bGameEvent = gameevent ;
		bWinPrizeEvent = winprizeevent ;

		bUseNoLimitTicketWarp = nolimitticketwarp ;
		siNoLimitTicketWarpDateVary = warpticketdatevary ;

		bPCRoom = bpcroom;
		bQuestEvolve = bquestevolve ;
	}
	void Set(cltPBasicInfo* pclinfo)
	{
		MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );

		siIDNum			= pclinfo->siIDNum;
		uiIndex			= pclinfo->GetIndex();
		uiSex			= pclinfo->uiSex;
		uiKind			= pclinfo->GetKind();
		siHomeVillage	= pclinfo->siHomeVillage;
		uiGRank			= pclinfo->uiGRank;
		uiAngelSwitch	= pclinfo->uiAngelSwitch;
		clPos.Set(pclinfo->GetPos());
		siPlaySecond	= pclinfo->siPlaySecond;
		siSchoolUnique	= pclinfo->siSchoolUnique;
		clLastMainMapPos.Set(&pclinfo->clLastMainMapPos);

		clPerson.Set(&pclinfo->clPerson);
		clFatherInfo.Set(&pclinfo->clFatherInfo);
		clRecommenderInfo.Set(&pclinfo->clRecommenderInfo);
		siVoteKingSelectionIndex = pclinfo->siVoteKingSelectionIndex;

		siWealthRank		= pclinfo->siWealthRank; 
		clTotalWealthMoney.Set(&pclinfo->clTotalWealthMoney);
		siWealthCalcDay		= pclinfo->siWealthCalcDay;

		bTodayFirstAttendanceSwitch	= pclinfo->bTodayFirstAttendanceSwitch;
		siAttendanceDays			= pclinfo->siAttendanceDays;

		bWennyOldManUpgrade			= pclinfo->bWennyOldManUpgrade ;

		bNewAccount					= pclinfo->bNewAccount ;
		bGameEvent					= pclinfo->bGameEvent ;
		bWinPrizeEvent				= pclinfo->bWinPrizeEvent ;

		bUseNoLimitTicketWarp = pclinfo->bUseNoLimitTicketWarp ;
		siNoLimitTicketWarpDateVary = pclinfo->siNoLimitTicketWarpDateVary ;

		bPCRoom						= pclinfo->bPCRoom;
		bQuestEvolve				= pclinfo->bQuestEvolve ;
	}


	~cltPBasicInfo(){};

	void Init()
	{
		szAccountID[ 0 ] = NULL;
		siIDNum		= 0;
		uiIndex		= 0;
		uiSex		= 0;
		uiKind		= 0;
		siHomeVillage	= 0;
		siPlaySecond	= 0;
		siSchoolUnique	= 0;
		uiGRank			= 0;
		uiAngelSwitch	= 0;

		clPos.Init();
		clLastMainMapPos.Init();

		clPerson.Init();
		clFatherInfo.Init();
		clRecommenderInfo.Init();
		siVoteKingSelectionIndex = 0;
		
		siWealthRank		= 0;
		clTotalWealthMoney.Clear();
		siWealthCalcDay		= 0;

		bTodayFirstAttendanceSwitch	= false;
		siAttendanceDays			= 0;

		bWennyOldManUpgrade = false ;

		bNewAccount = false ;
		bGameEvent = false ;
		bWinPrizeEvent = false ;

		bUseNoLimitTicketWarp = false ;
		siNoLimitTicketWarpDateVary = 0 ;

		bPCRoom = false;
		bQuestEvolve = false ;
	}

	void SetIndex(SI32 index){uiIndex = index;}
	SI32 GetIndex()const{return uiIndex;}

	void SetKind(SI32 kind){uiKind = kind;}
	SI32 GetKind()const{return uiKind;}

	const cltCharPos* GetPos()const{return &clPos;}

	void SetMapIndex(SI32 index){clPos.SetMapIndex(index);}
	SI32 GetMapIndex()const{return clPos.GetMapIndex();}

	void SetX(SI32 x){clPos.SetX(x);}
	SI32 GetX()const{return clPos.GetX();}

	void SetY(SI32 y){clPos.SetY(y);}
	SI32 GetY()const{return clPos.GetY();}

	void SetName(const char* pname)
	{
		MStrCpy( clPerson.szName, pname, MAX_PLAYER_NAME );
	}
	const char*GetName()const{return clPerson.szName;}

	bool IsPCRoom() { return bPCRoom; }

	bool IsQuestEvolve() { return bQuestEvolve ; }
	void SetQuestEvolve(bool questevolve)
	{
		bQuestEvolve = questevolve ;
	}

};

//-------------------------------
// 기본이 되는 능력치. 
//-------------------------------
class cltBasicAbility{
public:
	// 기본 능력치.(사용자가 보너스를 가지고 올리는 능력치.) 
	SI32	siStr;				// 근력	
	SI32	siDex;				// 순발력
	SI32	siVit;				// 생명력 
	SI32	siMag;				// 도력 
	SI32	siHnd;				// 손재주. 
	
	cltBasicAbility()
	{
		Init();		
	}

	~cltBasicAbility(){};

	void Init()
	{
		ZeroMemory(this, sizeof(cltBasicAbility));
	}

	void Set(SI32 str, SI32 dex, SI32 vit, SI32 mag, SI32 hnd)
	{
		siStr	= str;
		siDex	= dex;
		siVit	= vit;
		siMag	= mag;
		siHnd	= hnd;
	}
	void Set(cltBasicAbility* pclinfo)
	{
		Set(pclinfo->siStr, pclinfo->siDex, pclinfo->siVit, pclinfo->siMag, pclinfo->siHnd);
	}

	SI32 GetStr()const{return siStr;}
	void SetStr(SI32 dat){	siStr = dat;}
	SI32 IncreaseStr(SI32 amount){ siStr += amount; return siStr;}
	SI32 DecreaseStr(SI32 amount){ siStr -= amount; return siStr;}
	SI32 GetStrDamage(){return siStr / 5;}

	SI32 GetDex()const{	return siDex;}
	void SetDex(SI32 dat){	siDex = dat;}
	SI32 IncreaseDex(SI32 amount){ siDex += amount; return siDex;}
	SI32 DecreaseDex(SI32 amount){ siDex -= amount; return siDex;}

	SI32 GetVit()const{return siVit;	}
	void SetVit(SI32 dat){siVit = dat;}
	SI32 IncreaseVit(SI32 amount){ siVit += amount; return siVit;}
	SI32 DecreaseVit(SI32 amount){ siVit -= amount; return siVit;}
	
	SI32 GetMag()const{return siMag;	}
	void SetMag(SI32 dat){siMag = dat;}
	SI32 IncreaseMag(SI32 amount){ siMag += amount; return siMag;}
	SI32 DecreaseMag(SI32 amount){ siMag -= amount; return siMag;}

	SI32 GetHnd()const{return siHnd;	}
	void SetHnd(SI32 dat){siHnd = dat;}
	SI32 IncreaseHnd(SI32 amount){ siHnd += amount; return siHnd;}
	SI32 DecreaseHnd(SI32 amount){ siHnd -= amount; return siHnd;}

};

//-----------------------------------------
// 마을 투자자 정보
//-----------------------------------------
class cltInvestInfo{
public:
	cltSimplePerson	clPerson;
	SI32			siStockNum;		// 보유 주식수. 

public:
	cltInvestInfo()
	{
		Init();
	}

	void Set(cltInvestInfo* pinfo)
	{
		clPerson.Set(&pinfo->clPerson);
		siStockNum	= pinfo->siStockNum;
	}

	void Init()
	{
		clPerson.Init();
		siStockNum	= 0;
	}

	SI32 GetStockNum()const{return siStockNum;}
	void SetStockNum(SI32 num){siStockNum = num;}
};

class cltVillageInvestInfo{
public:
	SI32 siVillageUnique;
	cltInvestInfo clInvestInfo[MAX_INVEST_NUMBER];

	cltVillageInvestInfo()
	{
		siVillageUnique = 0;
	}

	void Set(cltVillageInvestInfo* pclMayorInfo)
	{
		memcpy(this, pclMayorInfo, sizeof(cltVillageInvestInfo));
	}
};

//--------------------------------------
// 건강 상태
//--------------------------------------
#define HEALTH_ORGAN_TYPE_ORG1			1		// 소화기관
#define HEALTH_ORGAN_TYPE_ORG2			2		// 순환기관
#define HEALTH_ORGAN_TYPE_ORG3			3		// 호흡기관
#define HEALTH_ORGAN_TYPE_ORG4			4		// 신경기관

//--------------------------------
// 건강 클래스. (핵심) 
//--------------------------------
class cltHealthInfo{
public:
	//------------------------
	// 기초 상태 
	//------------------------
	SI16 siHungry		;			// 배고픔.		(0~ 256)	기본은 128	

	//-------------------------
	// 기관 상태 
	//-------------------------
	SI08 siInnerOrg1	;		// 소화계 (-100 ~ 100)
	SI08 siInnerOrg2	;		// 순환계 
	SI08 siInnerOrg3	;		// 호흡계 
	SI08 siInnerOrg4	;		// 신경계 

	//------------------------
	// 질병 상태 
	//------------------------
	UI08 uiOrg1Disease	;		// 소화계 질병.			
	UI08 uiOrg2Disease	;		// 순환계 질병. 
	UI08 uiOrg3Disease	;		// 호흡계 질병. 
	UI08 uiOrg4Disease	;		// 신경계 질병. 

	//-----------------------
	// 종합 건강 지수.
	//-----------------------
	UI08 uiHealthPoint;		// 0 ~ 100	:100이면 능력치를 모두 발휘할 수 있따. 

	cltHealthInfo()
	{
		Init();
	}

	void Init()
	{
		//------------------------
		// 기초 상태 
		//------------------------
		siHungry		= 0;			// 배고픔.		
		
		//-------------------------
		// 기관 상태 
		//-------------------------
		siInnerOrg1		= 0;		// 소화계 
		siInnerOrg2		= 0;		// 순환계 
		siInnerOrg3		= 0;		// 호흡계 
		siInnerOrg4		= 0;		// 신경계 
		
		//------------------------
		// 질병 상태 
		//------------------------
		uiOrg1Disease	= 0;		// 소화계 질병.			
		uiOrg2Disease	= 0;		// 순환계 질병. 
		uiOrg3Disease	= 0;		// 호흡계 질병. 
		uiOrg4Disease	= 0;		// 신경계 질병. 
		
		//-----------------------
		// 종합 건강 지수.
		//-----------------------
		uiHealthPoint	= 100;
	}

	void Set(cltHealthInfo* pclinfo)
	{
		memcpy(this, pclinfo, sizeof(cltHealthInfo));
	}
	
	void IncreaseInnerOrg1(SI32 amount)
	{
		SI32 sum = siInnerOrg1 + amount;

		siInnerOrg1 = max(-100, min(100, sum));
	}
	void IncreaseInnerOrg2(SI32 amount)
	{
		SI32 sum = siInnerOrg2 + amount;

		siInnerOrg2 = max(-100, min(100, sum));
	}
	void IncreaseInnerOrg3(SI32 amount)
	{
		SI32 sum = siInnerOrg3 + amount;

		siInnerOrg3 = max(-100, min(100, sum));
	}
	void IncreaseInnerOrg4(SI32 amount)
	{
		SI32 sum = siInnerOrg4 + amount;

		siInnerOrg4 = max(-100, min(100, sum));
	}

	// 내장 기관을 손상시킨다. 
	bool DecreaseOrg(SI32 type, SI32 amount, SI32 level);

	// 내장 기관의 상태를 얻어온다. 
	SI32 GetOrg(SI32 type)
	{
		switch(type)
		{
		case HEALTH_ORGAN_TYPE_ORG1: return siInnerOrg1;
		case HEALTH_ORGAN_TYPE_ORG2: return siInnerOrg2;
		case HEALTH_ORGAN_TYPE_ORG3: return siInnerOrg3;
		case HEALTH_ORGAN_TYPE_ORG4: return siInnerOrg4;
		}

		return 0;
	}
	
};

//-------------------------
// 시간정보를 갖는 클래스 
//-------------------------
class cltDate{
public:
	UI32 uiYear:	8;	// 0 ~255
	UI32 uiMonth:	4;	// 0 ~15
	UI32 uiDay:		6;	// 0 ~63
	UI32 uiHour:	5;	// 0 ~31
	UI32 uiMinute:	6;	// 0 ~63
	UI32 uiReserved:3;	

public:

	void Set(SI32 year, SI32 month, SI32 day, SI32 hour, SI32 minute)
	{
		uiYear		= year;
		uiMonth		= month;
		uiDay		= day;
		uiHour		= hour;
		uiMinute	= minute;
		uiReserved	= 0;
	}

	void Set(const cltDate* pdate)
	{
		memcpy(this, pdate, sizeof(cltDate));
	}

	BOOL IncreaseTime(bool* pbdaychangedswitch, bool bforcetimeswitch);

	// 시간을 문자열로 만든다. 
	BOOL GetDateText(char* buffer);

	// 달을 더한다. 
	void AddMonth(SI32 month);
	
	// 날짜 변수를 구한다. (SI32 변수 하나로 날짜를 모두 뭉뚱그린다) 
	SI32 GetDateVary()
	{
		SI32 vary ;
		vary = (uiDay-1) + (uiMonth-1)*30 + (uiYear*12)*30;

		return vary;
	}
	// 시간 변수를 구한다. 
	SI32 GetHourVary()
	{
		return  GetDateVary() * 24 + uiHour;
	}

	// DateVary를 가지고 완전 날짜 정보를 조합한다. 
	void MakeFullDateFromDateVary(SI32 datevary)
	{
		uiYear  = datevary / (12* 30);
		datevary %= (12* 30);

		uiMonth = datevary / 30 + 1;
		datevary %= 30;

		uiDay		= datevary + 1;

		uiHour		= 0;
		uiMinute	= 0;
	}
	
	BOOL IsLaterDayThan( cltDate* pclDate );
	BOOL IsLaterMonthThan( cltDate* pclDate );

	void DiffDate( cltDate *pCurrentDate, cltDate *pOldDate );
	
	SI32 GetDay()
	{
		return min(30, max(1, uiDay));
	}
};

//----------------------------------------
// 임대계약 정보 
//----------------------------------------
class cltRentContract{
public:
	SI32 siRentFee;		// 월 임대료. 
	SI32 siRentPeriod;	// 임대기간. 
	cltDate	clDate;		// 계약일시. 

	void Set(SI32 fee, SI32 period)
	{
		siRentFee		= fee;
		siRentPeriod	= period;		
	}
	
	void Set( SI32 Fee, SI32 Period, cltDate *pclDate )
	{
		siRentFee = Fee;
		siRentPeriod = Period;
		clDate.Set( pclDate );
	}

	void Set(const cltRentContract* pclcontract)
	{
		siRentFee		= pclcontract->siRentFee;
		siRentPeriod	= pclcontract->siRentPeriod;
		clDate.Set(&pclcontract->clDate);
	}

	BOOL IsValid()
	{
		if(siRentFee < 0		|| siRentFee > MAX_RENT_FEE)return FALSE;
		if(siRentPeriod <= 0	|| siRentPeriod > MAX_RENT_PERIOD)return FALSE;

		return TRUE;
	}

	BOOL IsSame(const cltRentContract* pclcontract)
	{
		if(siRentFee != pclcontract->siRentFee)return FALSE;
		if(siRentPeriod != pclcontract->siRentPeriod)return FALSE;

		return TRUE;
	}
	// 계약이 종료되었는가?
	bool IsEnd(cltDate* pcldate)
	{
		if(clDate.uiYear + siRentPeriod < pcldate->uiYear)return true;
		else if(clDate.uiYear + siRentPeriod == pcldate->uiYear)
		{
			if(clDate.uiMonth	< pcldate->uiMonth)return true;
			else if(clDate.uiMonth	== pcldate->uiMonth)
			{
				if( clDate.uiDay <= pcldate->uiDay)return true;
			}
		}

		return false;
	}

	void Init()
	{
		siRentFee		= 0;		// 월 임대료. 
		siRentPeriod	= 0;		// 임대기간.
		clDate.Set(0, 0, 0, 0, 0);
	}
};


//-----------------------------------------------
// 말의 정보 
//-----------------------------------------------
#define HORSE_NEED_FEED			1		// 먹이를 줄 것을 요구. 

#define HORSE_REWARDTYPE_SPEED	1
#define HORSE_REWARDTYPE_STR	2
#define HORSE_REWARDTYPE_DEX	3
#define HORSE_REWARDTYPE_MAG	4

class cltHorse{
public:

	SI16 siHorseUnique;
	char	szName[MAX_HORSE_NAME];		// 이름 
	cltDate clBirthDate;				// 생일 

	SI08	siSex;						// 성별 
	SI08	siPregnentPeriod;			// 임신기간
	SI16	siLife;						// 수명 

	SI16	siStr;
	SI16	siDex;
	SI16	siMag;
	SI16	siMoveSpeed;

	SI32	siStatusBonus;				// 말 보너스

	cltDate clAddStatusDate;			// 말능력향상이용권 바른 일자
	SI32	siStatusTicketNum;			// 말능력향상이용권 몇장 발랐냐

	//------------------------------
	// 마지막으로 업데이트 한 시간. 
	//------------------------------
	SI32	siLastUpdateSecond;
	
	//---------------------------
	// 말의 요구 조건 관련 
	//---------------------------
	SI32	siLastNeedTime;				// 마지막으로 요구했던 시간. 
	SI32	siDueTime;					// 요구조건 충족의 한계 시간. ( 이 시간까지 요구조건을 충족시키지 못하면 실패) 

	SI16	siNeed;						// 말의 요구조건. 
	SI16	siPara1;		
	SI16	siPara2;
	SI16	siSupply;					// 충족 상황. 
	SI16	siRate;						// 말의 요구를 이해할 확률. 

	//--------------------------
	// 보상 관련 
	//--------------------------
	SI16	siRewardType;
	SI16	siRewardAmount;

public:

	cltHorse()
	{
		Init();
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltHorse));
	}

	cltHorse(SI16 unique, char* pname, cltDate* pcldate, SI08 sex, SI16 life, SI08 siPregnentPeriod,
			 SI16 str, SI16 dex, SI16 mag, SI16 movepseed, SI32 curhourvary,SI32 bonus,
			 cltDate* pclstatusdate , SI32 ticketnum )
	{
		siHorseUnique	= unique;
		MStrCpy( szName, pname, MAX_HORSE_NAME );
		clBirthDate.Set(pcldate);

		siSex				= sex;
		siPregnentPeriod	= siPregnentPeriod;
		siLife				= life;

		siStr				= str;
		siDex				= dex;
		siMag				= mag;
		siMoveSpeed			= movepseed;

		siStatusBonus		= bonus ;

		siLastUpdateSecond	= 0;
		siLastNeedTime		= curhourvary;	
		siDueTime			= 0;
		siNeed				= 0;
		siPara1				= 0;
		siPara2				= 0;
		siSupply			= 0;
		siRate				= 0;

		siRewardType		= 0;
		siRewardAmount		= 0;

		clAddStatusDate.Set(pclstatusdate);
		siStatusTicketNum	= ticketnum;
	}

	void Set(cltHorse* pclhorse)
	{
		memcpy(this, pclhorse, sizeof(cltHorse));
	}

	// 게임에 적용될 실제 이동 속도를 구한다.
	SI16 GetRealMoveSpeed()
	{
		return siMoveSpeed/100;
	}

	// 나이를 구한다. 
	SI32 GetAge(SI32 curday)
	{
		SI32 birthday	= clBirthDate.GetDateVary();

		if(curday >= birthday)
		{
			SI32 age = (curday - birthday) / 360 + 1;
			return age;
		}
		return 0;
	}

	// 같은 말인지 여부를 판단한다. 
	bool IsSame(cltHorse* pclhorse)
	{
		if(pclhorse == NULL)return false;
		if(siHorseUnique	!= pclhorse->siHorseUnique)return false;
		if(siSex			!= pclhorse->siSex)return false;
		if(siLife			!= pclhorse->siLife)return false;
		if(siStr			!= pclhorse->siStr)return false;
		if(siDex			!= pclhorse->siDex)return false;
		if(siMag			!= pclhorse->siMag)return false;
		if(siMoveSpeed		!= pclhorse->siMoveSpeed)return false;
		if(siStatusBonus	!= pclhorse->siStatusBonus)return false;
		if(strcmp(szName, pclhorse->szName) != 0)return false;

		return true;
	}
};


//-------------------------------------------------
// cltAttackTarget
//-------------------------------------------------
class cltAttackTarget{
public:
	SI32 			siAttackID;                    // 공격할 대상 아이디 
	cltCharPos		clPos;
	
	SI32			siHitRate;						// 공격 대상에 대한 명중률. 

	cltAttackTarget()
	{
		siAttackID	= 0;

		siHitRate	= 0;
	}

	cltAttackTarget(SI32 attackid, const cltCharPos* pclpos, SI32 hitrate)
	{
		siAttackID	= attackid;
		clPos.Set(pclpos);

		siHitRate	= hitrate;
	}

	void Set(const cltAttackTarget* pcltarget)
	{
		siAttackID	= pcltarget->siAttackID;
		clPos.Set(&pcltarget->clPos);

		siHitRate	= pcltarget->siHitRate;
	}

	void Init()
	{
		siAttackID	= 0;
		siHitRate	= 0;
	}

	void SetAttackID(SI32 id){siAttackID	= id;}
	SI32 GetAttackID()const{return siAttackID;}

};

//---------------------------------
// 전과 기록. 
//---------------------------------
#define MAX_PRISON_REASON_SIZE	50
#define MAX_PRISON_SECOND		30
#define MAX_PRISON_LIST_NUM		50 // 형조판서 창에서 보여줄 리스트 총 갯수

struct HyungzoMember
{
	SI32	siDate;										// 수감설정일
	SI32	siPrisonSecond;								// 수감시간
	char	szCharName[MAX_PLAYER_NAME];				// 수감자
	char	szPrisonReason[MAX_PRISON_REASON_SIZE];		// 수감이유
};

class cltGuiltyInfo{
public:
	SI32	siTotalPrisonSecond;			// 총 수감시간. 
	SI32	siLeftPrisonSecond;			// 남은 수감시간. 
	char	szPrisonReason[MAX_PRISON_REASON_SIZE];

	cltGuiltyInfo()
	{
		Init();
	}
	void Init()
	{
		siTotalPrisonSecond = 0;
		siLeftPrisonSecond	= 0;
		szPrisonReason[ 0 ] = NULL;
	}
	void Set(cltGuiltyInfo* pclinfo)
	{
		siTotalPrisonSecond = pclinfo->siTotalPrisonSecond;
		siLeftPrisonSecond	= pclinfo->siLeftPrisonSecond;

		MStrCpy( szPrisonReason, pclinfo->szPrisonReason, MAX_PRISON_REASON_SIZE );
	}
	void SetTotalPrisonSecond(SI32 second)
	{
		siTotalPrisonSecond = second ;
	}
	void SetLeftPrisonSecond(SI32 second)
	{
		siLeftPrisonSecond = second ;
	}
	void SetPrisonReason(char * reason)
	{
		MStrCpy(szPrisonReason, reason, MAX_PRISON_REASON_SIZE );
	}
};

//----------------------------------------------
// 수렵장 저장 정보 
//----------------------------------------------
class cltHuntStg{
public:
	SI16 siItemUnique;
	SI16 siItemNum;

	void Set(cltHuntStg* pclstg)
	{
		siItemUnique	= pclstg->siItemUnique;
		siItemNum		= pclstg->siItemNum;
	}

};

class cltHuntLineUp{
public:
	SI16 siLineUpKind;		// 전선에 배치된 몬스터 종류
	SI16 siLineUpKindNum;	// 전선에 배치된 몬스터 수량
	SI16 siLineUpKindLevel; // 전선에 배치된 몬스터 레벨

	void Set(cltHuntLineUp* pclinfo)
	{
		siLineUpKind		= pclinfo->siLineUpKind;
		siLineUpKindNum		= pclinfo->siLineUpKindNum;
		siLineUpKindLevel	= pclinfo->siLineUpKindLevel;
	}
};

#define MAX_FEAST_PRESENT_NUMBER							10
#define MAX_FEAST_STG_NUMBER								10

// 내자시 창고 정보
class cltFeastStg 
{
public:
	SI16 siItemUnique;
	SI16 siItemNum;

	void Set( cltFeastStg* pclstg )
	{
		siItemUnique	= pclstg->siItemUnique;
		siItemNum		= pclstg->siItemNum;
	}

};


//-----------------------------------------
// 스승 등록 신청자 정보. 
//-----------------------------------------
#define MAX_FATHER_APPLYER_LIST_NUMBER	50

class cltFatherApplyerInfo{
public:
	SI16 siCharUnique;
	SI32 siPersonID;
	SI16 siLevel;
	SI16 siFameLevel;
	bool bAngelSwitch;					// 수호천사인지 여부. 
	char szAccountID[MAX_PLAYER_NAME];
	char szName[MAX_PLAYER_NAME];

	cltFatherApplyerInfo()
	{
		Init();
	}
	
	cltFatherApplyerInfo(SI16 charunique, SI32 personid, char* pname, char* paccountid, SI16 level, SI16 famelevel, bool bangel)
	{
		siCharUnique	= charunique;
		siPersonID		= personid;
		MStrCpy(szName,		pname, MAX_PLAYER_NAME);
		MStrCpy(szAccountID, paccountid, MAX_PLAYER_NAME);

		siLevel			= level;
		siFameLevel		= famelevel;
		bAngelSwitch	= bangel;
	}

	void Init()
	{
		siCharUnique	= 0;
		siPersonID		= 0;
		szName[ 0 ] = NULL;
		szAccountID[ 0 ] = NULL;

		siLevel			= 0;
		siFameLevel		= 0;
		bAngelSwitch	= false;
	}

	void Set(cltFatherApplyerInfo* pclinfo)
	{
		siCharUnique	= pclinfo->siCharUnique;
		siPersonID		= pclinfo->siPersonID;
		MStrCpy(szName, pclinfo->szName, MAX_PLAYER_NAME);

		MStrCpy(szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME);

		siLevel			= pclinfo->siLevel;
		siFameLevel		= pclinfo->siFameLevel;
		bAngelSwitch	= pclinfo->bAngelSwitch;
	}

};

//-------------------------------------
// 제자 1인의 정보. 
//-------------------------------------
class cltChildInfo{
public:
	cltSimplePerson clPerson;
	SI16			siLevel;
	char			szAccountID[MAX_PLAYER_NAME];

	cltChildInfo()
	{
		siLevel = 0;
		szAccountID[ 0 ] = NULL;
	}
	cltChildInfo(cltSimplePerson* pclperson, SI16 level,char * accountID)
	{
		clPerson.Set(pclperson);
		siLevel	= level;
		MStrCpy(szAccountID,accountID,MAX_PLAYER_NAME);
	}
	void Init()
	{
		clPerson.Init();
		siLevel = 0;
		szAccountID[ 0 ] = NULL;
	}
	void Set(cltChildInfo* pclchild)
	{
		clPerson.Set(&pclchild->clPerson);
		siLevel	= pclchild->siLevel;
		MStrCpy(szAccountID,pclchild->szAccountID,MAX_PLAYER_NAME);
	}

};

//------------------------------
// Person친구 정보. 
//------------------------------
#define MAX_FRIEND_NUMBER					20
class cltPFriendInfo{
public:
	cltSimplePerson clFriend[MAX_FRIEND_NUMBER];
	char szAccountID[MAX_FRIEND_NUMBER][MAX_PLAYER_NAME];
	void Set(cltPFriendInfo* pclinfo)
	{
		SI32 i;

		for(i = 0;i < MAX_FRIEND_NUMBER;i++)
		{
			clFriend[i].Set(&pclinfo->clFriend[i]);
			MStrCpy(szAccountID[i],pclinfo->szAccountID[i],MAX_PLAYER_NAME);
		}
	}
};

class cltFriendInfo : public cltPFriendInfo
{
public:
	SI32 siCharID[ MAX_FRIEND_NUMBER ];
	bool bConnectStatus[ MAX_FRIEND_NUMBER ];
//	char szAccountID[ MAX_FRIEND_NUMBER ][MAX_PLAYER_NAME];

	void Init()
	{
		for( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			memset( &clFriend[ i ], 0, sizeof( cltSimplePerson ) );
			siCharID[ i ] = 0;
			bConnectStatus[ i ] = false;
			szAccountID[ i ][ 0 ] = NULL;
		}
	}

	void Init( SI16 Index )
	{
		memset( &clFriend[ Index ], 0, sizeof( cltSimplePerson ) );
		siCharID[ Index ] = 0;
		bConnectStatus[ Index ] = false;
		szAccountID[ Index ][ 0 ] = NULL;
	}

	void Set( SI16 Index, SI32 PersonID, SI32 CharID, char *CharName, bool ConnectStatus ,char *AccountID)
	{
		siCharID[ Index ] = CharID;
		bConnectStatus[ Index ] = ConnectStatus;
		clFriend[ Index ].Set( PersonID, CharName );
		MStrCpy(szAccountID[Index],AccountID,MAX_PLAYER_NAME);
	}

	void Set( SI16 Index, SI32 CharID, bool ConnectStatus ,char * AccountID)
	{
		siCharID[ Index ] = CharID;
		bConnectStatus[ Index ] = ConnectStatus;
		MStrCpy(szAccountID[Index],AccountID, MAX_PLAYER_NAME);
	}
	
	void Set( cltPFriendInfo *pclInfo )
	{
		SI32 i;

		for(i = 0;i < MAX_FRIEND_NUMBER;i++)
		{
			clFriend[i].Set(&pclInfo->clFriend[i]);
		}
	}
	
	void Set( cltFriendInfo *pclInfo )
	{
		memcpy( this, pclInfo, sizeof( cltFriendInfo) );
	}
	
	SI16 GetFriendNum()
	{
		SI16 Counter = 0;
		for( SI32 i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			if ( clFriend[ i ].GetPersonID() > 0 )
			{
				++Counter;
			}
		}

		return Counter;
	}
};

class cltPDailyQuestInfo
{
public:
	SI16 siMonsterKind;
	SI16 siMonsterNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	SI32 siRewardExp;

	SI32 siFee;
	SI32 siLeftTime;

	SI16 siKillMonsterNum;

	cltPDailyQuestInfo()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( cltPDailyQuestInfo ) );
	}

	void Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, SI32 RewardExp, SI32 Fee, SI32 LeftTime, SI16 KillMonsterNum )
	{
		siMonsterKind = MonsterKind;
		siMonsterNum = MonsterNum;

		siRewardItemUnique = RewardItemUnique;
		siRewardItemNum = RewardItemNum;
		siRewardExp = RewardExp;
		
		siFee = Fee;

		siLeftTime = LeftTime;

		siKillMonsterNum = KillMonsterNum;
	}

	void Set( cltPDailyQuestInfo *pclInfo )
	{
		memcpy( this, pclInfo, sizeof( cltPDailyQuestInfo) );
	}
};

//--------------------------------------
// 재료
//--------------------------------------
class cltMaterialInfo {
public:
	SI32	siUnique;		// 재료의 유니크. 
	SI32	siNeedNum;		// 재료의 필요 개수. 

	cltMaterialInfo()
	{
		siUnique	= 0;
		siNeedNum	= 0;
	}

	cltMaterialInfo(SI32 unique, SI32 itemnum)
	{
		siUnique	= unique;
		siNeedNum	= itemnum;
	}

	void Set(cltMaterialInfo* pclmaterial)
	{
		siUnique	= pclmaterial->siUnique;
		siNeedNum	= pclmaterial->siNeedNum;
	}
	void Set(SI32 unique, SI32 itemnum)
	{
		siUnique	= unique;
		siNeedNum	= itemnum;
	}
};

//--------------------------------------
// 도구
//--------------------------------------
class cltToolInfo {
public:
	SI32	siUnique;		// 도구의 유니크. 

	cltToolInfo()
	{
		siUnique	= 0;
	}

	cltToolInfo(SI32 unique)
	{
		siUnique	= unique;
	}

	void Set(cltToolInfo* pcltool)
	{
		siUnique	= pcltool->siUnique;
	}
	void Set(SI32 unique)
	{
		siUnique	= unique;
	}
};


class cltShipCarryWeight
{
public:
	SI32		siShipWeaponWeight;				// 대포 또는 배 무기 무게 총합	
	SI32		siTransportWeight;				// 짐 무게 총합( 운반 되는 것 + 소모되는 것 ) - 소모는 연료 또는 음식이 되겠지...
	SI32		siPassengerWeight;				// 사람 무게 총합

	cltShipCarryWeight()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( cltShipCarryWeight ) );
	}

	void Set( cltShipCarryWeight *pclShipCarryWeight )
	{
		memcpy( this, pclShipCarryWeight, sizeof( cltShipCarryWeight ));
	}

	void Set( SI32 ShipWeaponWeight, SI32 TransportWeight, SI32 PassengerWeight )
	{
		siShipWeaponWeight = ShipWeaponWeight;		
		siTransportWeight = TransportWeight;
		siPassengerWeight = PassengerWeight;
	}
};

//--------------------------------------
// 각각의 배의 기본 정보. ( 공용 정보 ex) 황포돛배 - kind = 1, 배의 기본 능력치, 배 기본 적재 무게.
//--------------------------------------
class cltShipBaseInfo
{
public:
	SI32				siKind;						// 배 종류	

	SI16				siAP;						// 기본 공격력( Attack Power )
	SI16				siAC;						// 기본 방어력( Armor Class )
	SI16				siMoveSpeed;				// 기본 이동속도. 

	SI16				siDurability;				// 현재 내구도
	SI16				siDecreaseDurability;		// 최대 내구도 깎이는 것.
	
	cltShipCarryWeight	clShipCarryWeight;			// 배 기본 적재 무게

	cltShipBaseInfo()
	{
		Init();
	}
	
	cltShipBaseInfo( SI16 kind, SI16 Damage, SI16 ac, SI16 movespeed, SI16 Durability, SI16 DecreaseDurability, cltShipCarryWeight *pclShipCarryWeight );

	void Init()
	{
		memset( this, 0, sizeof( cltShipBaseInfo ) );
	}

	void Set( cltShipBaseInfo *pclShipInfo )
	{
		memcpy( this, pclShipInfo, sizeof( cltShipBaseInfo ) );
	}

	void Set( SI32 Kind, SI32 Damage, SI32 AC, SI32 MoveSpeed, SI32 Durability, SI32 DecreaseDurability, cltShipCarryWeight *pclShipCarryWeight );

	// 배 기본 성능 정보를 얻어옮.
	bool GetShipStatus( cltShipBaseInfo *pclShipBaseInfo );
	
};

class cltShip : public cltShipBaseInfo
{
public:
	SI32				siShipID;						// 배의 고유 키( PersonID 같은 거 )
	
	char				szName[MAX_SHIP_NAME];		// 배의 이름.

	SI16				siShipPos;						// 배의 위치( 시전, 필드, 마을, 던전 )
	SI16				siPortUnitSlot;					// 배가 포구에 있는 경우 포구 번호
	cltCharPos			clPos;							// 배가 위치한곳.( 필드인 경우에만 )
	
	cltShip() : cltShipBaseInfo()
	{
		Init();
	}
	
	void Init()
	{
		ZeroMemory(this, sizeof(cltShip));
		siPortUnitSlot = -1;
		return;
	}
	
	cltShip( SI32 ShipID, SI16 kind, SI16 ShipPos, SI16 PortUnitSlot, cltCharPos *pclpos, SI16 ac, SI16 movespeed, cltShipCarryWeight *pclShipCarryWeight );

	void Set(cltShip* pclship)
	{
		memcpy( this, pclship, sizeof( cltShip ) );
		return;
	}
	
	void Set( cltShipBaseInfo *pclShipBaseInfo )
	{
		cltShipBaseInfo::Set( pclShipBaseInfo );
		return;
	}

	void Set( SI32 Kind, SI32 Damage, SI32 AC, SI32 MoveSpeed, SI32 Durability, SI32 MaxDurability, cltShipCarryWeight *pclShipCarryWeight,
		SI32 ShipID, char *Name, SI16 ShipPos, SI16 PortUnitSlot, cltCharPos *pclPos );

	bool GetShipPos( SI16 *pShipPos, SI16* pPortUnitSlot, cltCharPos *pclPos );
	bool SetShipPos( SI16 ShipPos, SI16 PortUnitSlot, cltCharPos *pclPos );
	
	char* GetName();
	bool SetName( char *ShipName );
};


//------------------------------------
// 사용자가 갖는 배의 총정보. 
//------------------------------------
class cltPShipInfo{
public:	
	SI32					siRideShipID;
	cltShip					clShip[MAX_SHIP_PER_PERSON];
	
	cltPShipInfo()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( cltPShipInfo ) );
	}
		
	void Set(cltPShipInfo* pclinfo )
	{
		memcpy( this, pclinfo, sizeof( cltPShipInfo ) );
	}

	SI32 GetShipNumber();

	bool IsShipExist( SI32 ShipID );

	cltShip *GetShipDataPtr( SI32 ShipID );
	SI16 GetShipDataIndex( SI32 ShipID );
	
	bool GetShipData( SI32 ShipID, cltShip *pclShip );
	bool SetShipDataChange( cltShip *pclShip );
		

	SI32 AddShip( cltShip *pclShip );
	
	// 배가 없어 진다. 내구도가 0 이거나, 포구에 연체를 해서...
	bool ShipExplode( SI32 ShipID );
};

#endif

