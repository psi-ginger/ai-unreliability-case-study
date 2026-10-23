//---------------------------------
// 2003/7/4 김태곤
//---------------------------------
#ifndef _CHARCOMMON_H
#define _CHARCOMMON_H

#include "../../Char\KindInfo\KindInfo.h"

#include <windows.h>
#include <windowsx.h>
#include <Dplay8.h>
#include <directives.h>
#include "../../Char\CharAni\CharAni.h"
#include "../../Char\parametaBox\ParametaBox.h"
#include "../../Item\ItemCommon\cltItem.h"
#include "../../../CommonLogic\Bank\Bank.h"
#include "../../Char\CharClient\CharCut\CharCut.h"
#include "../../Horse\Horse.h"
#include "../../../Client\Effect\LevelUpEffect\LevelUpEffect.h"
#include "../../../CLient/Effect/HeartEffect/HeartEffect.h"
#include "../../../CLient/Effect/FoueLeafEffect/FourLeafEffect.h"
#include "../../../Client/Effect/Cushion/CushionEffect.h"
#include "../../../Client/Effect/CushionCat/CushionCatEffect.h"



#include "../../../Client\Effect\MVPEffect\MVPEffect.h"
#include "../../../Client\Effect\EnchantEffect\EnchantEffect.h"
#include "../../../Client\Effect\QuestEffect\QuestEffect.h"
#include "../../../Client\Effect\Regeneration\Regeneration.h"
#include "../../../Client\Effect\BossEffect\BossEffect.h"
#include "../../../Client\Effect\SkillLevelUpEffect\SkillLevelUpEffect.h"
#include "../../../Client/Effect/Emoticon/EmoticonMgr.h"

#include "../../Char\IDManager\IDManager.h"
#include "../../Char\CharCommon\Char-EconomyInfo.h"
#include "../../Char\CharManager\CharManager.h"


// 네트워크용 헤더. 
#include "..\..\NetworkLib\ITZNetLib\ITZNetworkThread.h"
#include "..\..\NetworkLib\ITZNetLib\ITZPerson.h"
#include "..\..\NetworkLib\ITZNetLib\ITZListen.h"

// 조합형 캐릭터용 헤더. 
#include "../../Char\CCD\BaseDefinition.h"
#include "../../Char\CCD\XSPRMgr.h"
#include "../../Char\CCD\CharacterDrawing.h"

#include "../../PersonInfo.h"
#include "../../../CommonLogic/MagicMgr/MagicBaseInfo.h"

#include "../../../common/Family/Marriage.h"
#include "../../../common/Family/Family.h"	//가족정보

#include "../../../common/SoulGuard/SoulGuard.h"

//시간비교를위해서 추가. 삭제예정을 위해 추가
#include "../NLib/NUtil.h"
#include "../../../../NLib/NTech/sdk/nkcore/Include/NkList.h"

//----------------------------
// 캐릭터 이동 리턴값. 
//----------------------------
#define MOVERTNVAL_MOVING    0    // Moving
#define MOVERTNVAL_MOVEDONE  1    // Move Done
#define MOVERTNVAL_HALFDONE  2    // arrive Middle Point  

//----------------------------
// 아이템을 사용자에게 주는 모드. 
//----------------------------
#define GIVEITEM_MODE_PERSON		1	// 특정 사용자 1인에게만 준다. 
#define GIVEITEM_MODE_ADULT			2	// 레벨 FATHER_LEVEL이상 사용자에게만 준다.
#define GIVEITEM_MODE_ALL			3	// 모든 사용자에게 준다. 
#define GIVEITEM_MODE_PCROOM		4	// pc방에서 접속한 사용자에게 아이템을 지급한다.
#define GIVEITEM_MODE_MINLEVEL		5	// 해당 레벨 이상의 유저에게만 아이템 지급.
#define GIVEITEM_MODE_VILLAGER		6	// 마을 주민에게 지급
#define GIVEITEM_MODE_SEQGMCOMMAND	7	// [춘기] /seq 명령어를 사용해서 지급

//----------------------------
// 아이템을 사용자에게 줄때 결과 리턴
//----------------------------
#define GIVEITEM_RESULT_SUCCESS	1
#define GIVEITEM_RESULT_NOINVEN	2

#define MAX_FLY_VIBRATE     8

#ifdef _CHINA
	#define CANFARM_LEVEL			30	// 농경 가능 레벨
	#define CANMINE_LEVEL			30	// 채광 가능 레벨
#else
	#define CANFARM_LEVEL			20	// 농경 가능 레벨
	#define CANMINE_LEVEL			20	// 채광 가능 레벨
#endif


//--------------------------------------
// 농사 실패 사유 
//--------------------------------------
#define CANFARM_ERROR_NOSKILL		1		// 농경술 부족. 
#define CANFARM_ERROR_NOTOOL		2		// 농기구 없음. 
#define CANFARM_ERORR_NORENT		3		// 임차한 농경지 업음. 
#define CANFARM_ERORR_HAVEDISEASE	4		// 질병에 걸림
#define CANFARM_ERORR_CANNOTLEVEL	5		// 레벨이 안됨
#define CANFARM_ERORR_RENTING		6		// 이미 다른곳에서 임대중이라서 임대료 다시 낼 필요없음.

#define CANMINE_ERROR_NOSKILL		1		// 채굴술 부족. 
#define CANMINE_ERROR_NOTOOL		2		// 채굴 기구 없음. 
#define CANMINE_ERORR_NORENT		3		// 임차한 채굴지 업음. 
#define CANMINE_ERORR_HAVEDISEASE	4		// 질병에 걸림
#define CANMINE_ERORR_CANNOTLEVEL	5		// 레벨이 안됨
#define	CANMINE_ERORR_RENTING		6		// 이미 다른곳에서 임대중이라서 임대료 다시 낼 필요없음.


#define MAX_CANADDINV_CHECK_NUM		10		// 여러 종류 아이템이 한번에 인벤에 들어갈수 있을지 체크할때 최대가짓수

#define SUMMON_TAKEOUT_DELAY		20		// 소환수 재소환까지 대기 시간(단위: 분)

#define INCREASEMOVESPEED_DELAY		600000
#define INCREASEMOVESPEED_POWER		4
//-----------------------------------------
// 레벨업 퀘스트를 줄 레벨 
//-----------------------------------------
#define  BASIC_QUEST_LEVEL_OVER100	20	// 테스트를 위해 20으로 한다.
class cltCharManager;
class cltIDManager;
class cltMapCommon;
class cltMsg;
class CDrawChat;
class cltLetterUnit;
class CExpCounter;
class CHealCounter;
class CCharCriticalEffect;
class cltGameMsgRequest_MakeItemInv;
class cltStatisticsManager;
class cltPersonInfo;
class cltQuestClearConditionInfo;
class cltMapManager;


#define MAX_UPDATE_SWITCH			18
#define  UPDATE_BASICINFO			0
#define  UPDATE_IP					1
#define  UPDATE_SKILL				2
#define  UPDATE_INVENTORY			3
#define  UPDATE_BANK				4
#define  UPDATE_STOCK				5
#define  UPDATE_STOCKORDER			6
#define  UPDATE_MARKET				7
#define  UPDATE_RANK				8
#define  UPDATE_REALESTATE			9
#define  UPDATE_HEALTH				10
#define  UPDATE_QUEST				11
#define  UPDATE_HORSE				12
#define  UPDATE_SUMMON				13
#define  UPDATE_GUILTY				14
//_LEON_GUILD_STOCK
#define  UPDATE_GUILDSTOCK			15
#define  UPDATE_GUILDSTOCKORDER		16
#define	 UPDATE_WORLDMONEY			17



//--------------------------------------------
// 캐릭터의 행위 단계 
//--------------------------------------------
#define CHARWORKSTEP_DECIDELENGTH		 	1	
#define CHARWORKSTEP_ACTION        			2
#define CHARWORKSTEP_MOVE	    			3
#define CHARWORKSTEP_RETREAT       			4


//-------------------------------------------
// 캐릭터의 아이템 사용시 오류 메시지. 
//-------------------------------------------
enum CANUSEITEM_ERROR
{
	CANUSEITEM_ERROR_INVALID			= 0,			// 원인 불명.
	CANUSEITEM_ERROR_DEADCHAR,							//01 아이템 사용자가 죽었다. 
	CANUSEITEM_ERROR_NOSKILLPOINT,						//02 기술 점수가 전혀 없어 사용불가 
	CANUSEITEM_ERROR_NOTFITCOND,						//03 사용조건을 충족시키지 못함. 
	CANUSEITEM_ERROR_ALREADYMAXSKILL,					//04 이미 최고 수준으로 기술을 향상 시켰다. 
	CANUSEITEM_ERROR_NEEDPARENTSKILL,					//05 부모기술 필요. 
	CANUSEITEM_ERROR_LIMITSKILLLEVEL,					//06 기술 상승 제한. 
	CANUSEITEM_ERROR_NOUSEABLE,							//07 사용속성이 아니다. 
	CANUSEITEM_ERROR_NOHORSE,							//08 말이 아니라 사용할 수 없다. 
	CANUSEITEM_ERROR_NEEDLEVEL,							//09 사용레벨에 도달하지 못하여사용할수없다.
	CANUSEITEM_ERROR_NOMORE,							//10 이미 갖고 있어 사용을 할 수 없다.
	CANUSEITEM_ERROR_SUMMON_NOMORE,						//11 소환수 최대 갯수 도달 
	CANUSEITEM_ERROR_NOPROPERFISHAREA,					//12 주위에 물이 없어서 낚시를 할수 없다.
	CANUSEITEM_ERROR_NOCOUNTRY,							//13 같은 국가가 아니다.
	CANUSEITEM_ERROR_NOMOREUSE,							//14 더이상은 못 사용한다.
	CANUSEITEM_ERROR_NOMORE_DIGEST,						//15 배가 부른 상태가 아니라 더이상은 설사제를 사용 못한다.
	CANUSEITEM_ERROR_NOMORE_DESEASE,					//16 만능약은 질병상태 때만 사용 가능하다.
	CANUSEITEM_ERROR_NOUSE_THISPLACE,					//17 해당장소에서는 사용 불가
	CANUSEITEM_ERROR_OVERMAXHORSE,						//18 최대값이상의 말을 사용할수는 없다.
	CANUSEITEM_ERROR_RIDEHORSEUSEITEM,					//19 말에 올라탄 상태에서 아이템을 사용했다.
	CANUSEITEM_ERROR_CANTOVERSLOT,						//20 더이상 슬롯이 없어서 소환할 수 없다.
	CANUSEITEM_ERROR_TICKETOVERDATE,					//21 사용권의 기간이 다 되었다.
	CANUSEITEM_ERROR_BEFOREUSE_SAMEITEM,				//22 프리미엄 파츠중 이미 같은 파츠를 사용중이다.
	CANUSEITEM_ERROR_CANNOTUSETYPEGORSE_SAMEITEM,		//23 프리미엄 파츠중 이미 같은 파츠를 사용중이다.
	CANUSEITEM_ERROR_CANNOTUSEPREMIUMITEM,				//24 프리미엄 파츠나 부스터필을 사용 하면 한종류는 사용이 불가능 하다.
	CANUSEITEM_ERROR_NOTMARRIED,						//25 결혼하지 않았다
	CANUSEITEM_ERROR_CANNOT_SAMEMAP,					//26 같은 맵상에 있지 않다
	CANUSEITEM_ERROR_ALREADY_MAX_LOVELEVEL,				//27 이미 최고 애정도이다
	CANUSEITEM_ERROR_CAN_NOT_USETIME,					//28 아직 사용할수있는 시간이 아니다
	CANUSEITEM_ERROR_CAN_NOT_FAMILY,					//29 가족관계가 없다.
	CANUSEITEM_ERROR_NOT_ENOUGH_GET_TREASUR,			//30 보물찾기 이벤트를 받을 조건이 안된다.-메리엘 퀘스트를 끝내야 보물찾기를 할수 있다.-
	CANUSEITEM_ERROR_SOULGUARD_NOMORE,					//31 더이상 수호정령을 갖을 수 없음
	CANUSEITEM_ERROR_CANNOT_USE_COOKIE_NO_OVER_TIME,	//32 아직 쿠키를 쓸 타이밍이 안됐다
	CANUSEITEM_ERROR_SKILL_BOOK,						//33 스킬북.
	CANUSEITEM_ERROR_SKILL_BOOK_SUB_MAGIC_FULL,			//34 스킬북 보조 마법 3개 이상 습득이 불가.
	CANUSEITEM_ERROR_BATTLEROYALMAP							    //35 배틀로얄 맵 지역
};



#define BASIC_PLAYERMASTER_LEVEL			90	// 플레이어 운영자 기본 레벨
#define BASIC_GAMEMASTER_LEVEL				100	// 운영자 기본 레벨

#define CHAR_ACTION_INTERVAL_FRAME			6	// 캐릭터 행위가 이루어지는 프레임 단위.

//-------------------------------------------
// 캐릭터의 마법 사용시 오류 메시지. 
//-------------------------------------------
#define	CANUSEMAGIC_ERROR_INVALID			0		// 
#define	CANUSEMAGIC_ERROR_NEEDMANA			1		// 마법 부족 
#define	CANUSEMAGIC_ERROR_FARDISTANCE		2		// MAX Distance를 넘어섰다.
#define	CANUSEMAGIC_ERROR_NEEDTIMEDELAY		3		// 마법 Delay 시간이 필요하다.
#define CANUSEMAGIC_ERROR_NOTENOUGHSKILL	4		// 기술레벨이 부족하여 사용 불가. 

enum {	CAPTIONKIND_NEWSCHAT, 
		CAPTIONKIND_STOCK,
		CAPTIONKIND_NEWS, 
		CAPTIONKIND_MARKET,
		CAPTIONKIND_PERSONALSHOP, 
		CAPTIONKIND_GUILD, 
		CAPTIONKIND_COLORNEWSCHAT,
		CAPTIONKIND_END				};


#define MAX_SHOP_ITEM_NUMBER				5
#define MAX_SHOP_MAKEITEM_NUMBER			1		// MAX_SHOP_ITEM_NUMBER 5 보다 작거나 같아야한다.
#define MAX_SHOP_TITLE_LENGTH				21		// 상점 타이틀의 최대 글자 크기

//---------------------------------
// 개인상점 
//---------------------------------
#define PSHOP_MODE_NONE						0		// 상점 모드가 없다.						
#define PSHOP_MODE_BUY						1		// 상점 모드 사기 
#define PSHOP_MODE_SELL						2		// 상점 모드 팔기 
#define PSHOP_MODE_MAKEITEM					3		// 상점 모드 아이템 제조의뢰	-	제조를 의뢰 하기를 원하는 모드 
#define PSHOP_MODE_MAKEITEM_WANT			4		// 상점 모드 아이템 제조의뢰	-	제조를 하기를 원하는 모드 

#define	SUMMON_DECREASESTAMINA_AMT			10		// 소환수의 기본 스태미너 소모량( 1분당)
#define	MAX_SUMMON_TRANSFORM_TIME			600		// [진성] 소환수 변신 스테미너 최대 600 (60분) 
#define	SUMMON_POSSIBLE_TRANSFORM_TIME		10		// [진성] 소환수 변신 스테미너 최대  10 ( 1분) 



//--------------------------------
// 제조동작
//--------------------------------

#define MAKE_ANI_ARMOR	1
#define MAKE_ANI_HEAL	2
#define MAKE_ANI_FOOD	3

#define ENCHANT_ANI_WEPONE 101
#define ENCHANT_ANI_ARMOR 102

//KHY - 1014 - 방석 추가.
#define MAX_CUSHION_NUM 5

//[진성] 하루동안 질문할 수 있는 횟수.
#define MAX_DAILY_QUESTION_NUM 5

#define MAX_EXPLAIN_LENTH	1024


//---------------------------------
// 개인상점 
//---------------------------------
class  cltShopTradeData
{
public:
	SI08		    m_siShopMode;			// PSHOP_MODE_BUY, PSHOP_MODE_SELL
	SI32			m_siPersonID;
	TCHAR			m_szAccountID[MAX_PLAYER_NAME];
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem,	MAX_SHOP_ITEM_NUMBER>		m_clItem;
	NSafeTArray<SI16,		MAX_SHOP_ITEM_NUMBER>		m_siSelectedInventoryPos;
	NSafeTArray<GMONEY,		MAX_SHOP_ITEM_NUMBER>		m_siPrice;
	NSafeTArray<GMONEY,		MAX_SHOP_ITEM_NUMBER>		m_siCurrentPrice;
#else
	cltItem			m_clItem[ MAX_SHOP_ITEM_NUMBER ];
	SI16			m_siSelectedInventoryPos[ MAX_SHOP_ITEM_NUMBER ];
	GMONEY			m_siPrice[ MAX_SHOP_ITEM_NUMBER ];
	GMONEY			m_siCurrentPrice[ MAX_SHOP_ITEM_NUMBER ];
#endif
	SI16			m_siHandPoint;	// 제조 인터페이스 개선 이후에는 행운을 사용
	SI16			m_siSkillLevel;
	TCHAR			m_szShopTitle[MAX_SHOP_TITLE_LENGTH];
	bool			m_bPremium;
	
	cltItem			m_clSpacialItem;									//[진성] 밀봉 프리미엄 상점 이용권 아이템이 등록된다.
	SI16			m_siSelectedSpacialInventoryPos;					//[진성] 위치도 등록된다.

	cltShopTradeData();

	void Init();
	void InitData();

	void Set(cltShopTradeData *pclData);
	void SetCurrentPrice(cltItemManagerCommon *pclitemmanager);
};

//---------------------------------
// 개인상점 배너 
//---------------------------------
class  cltShopBannerData
{
public:
	SI08		    m_siShopMode;			// PSHOP_MODE_BUY, PSHOP_MODE_SELL

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16,	MAX_SHOP_ITEM_NUMBER>		m_siUnique;
	NSafeTArray<GMONEY,	MAX_SHOP_ITEM_NUMBER>		m_siPrice;
#else
	SI16			m_siUnique[ MAX_SHOP_ITEM_NUMBER ];
	GMONEY			m_siPrice[ MAX_SHOP_ITEM_NUMBER ];
#endif
	SI16			m_siHandPoint;
	SI16			m_siSkillLevel;
	TCHAR			m_szShpoTitle[MAX_SHOP_TITLE_LENGTH];
	bool			m_bPremium;

	cltShopBannerData();
	void Init();
	void Set(cltShopBannerData *pclBanner);
	void SetData(cltShopTradeData *pclData);
	void MakeBannerText(cltItemManagerCommon* pclItemManager, TCHAR *ptext,TCHAR *pShopTitle = NULL);
};

//---------------------------------
// 수리의뢰
//---------------------------------
class  cltShopRepairData
{
public:
	SI32			m_siPersonID;
	TCHAR			m_szAccountID[MAX_PLAYER_NAME];

	cltItem			m_clRepairItem;
	SI16			m_siRepairItemPos;
	GMONEY			m_siPrice;

    RepairItemExtraMaterial	m_kMaterial;

	cltShopRepairData();

	void Init();
	void InitData();

	void Set( cltShopRepairData *pclData );
	bool ExistItem( SI16 siPos, cltItem* pclitem );
};

//---------------------------------
// 개인상점 검색 정보 
//---------------------------------
#define MAX_SEARCHSHOP_ITEM_NUMBER			50

class cltSearchShopData	
{
public:
	SI32	m_siShopMode;
	SI32	m_siItemUniqueType;
	GMONEY	m_siItemSearchPrice;
	
	TCHAR	m_szName[MAX_SEARCHSHOP_ITEM_NUMBER][MAX_PLAYER_NAME];

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32,	MAX_SEARCHSHOP_ITEM_NUMBER>		m_siLevel;

	NSafeTArray<SI16,	MAX_SEARCHSHOP_ITEM_NUMBER>		m_siMapIndex;
	NSafeTArray<SI32,	MAX_SEARCHSHOP_ITEM_NUMBER>		m_siMapX;
	NSafeTArray<SI32,	MAX_SEARCHSHOP_ITEM_NUMBER>		m_siMapY;

	NSafeTArray<cltItem, MAX_SEARCHSHOP_ITEM_NUMBER>	m_clItem;
	NSafeTArray<GMONEY,  MAX_SEARCHSHOP_ITEM_NUMBER>	m_siPrice;

	NSafeTArray<SI16,	MAX_SEARCHSHOP_ITEM_NUMBER>		m_siHandPoint;
	NSafeTArray<SI16,	MAX_SEARCHSHOP_ITEM_NUMBER>		m_siSkillLevel;
#else
	SI32	m_siLevel[MAX_SEARCHSHOP_ITEM_NUMBER];

	SI16	m_siMapIndex[MAX_SEARCHSHOP_ITEM_NUMBER];
	SI32	m_siMapX[MAX_SEARCHSHOP_ITEM_NUMBER];
	SI32	m_siMapY[MAX_SEARCHSHOP_ITEM_NUMBER];

	cltItem m_clItem[MAX_SEARCHSHOP_ITEM_NUMBER];
	GMONEY  m_siPrice[MAX_SEARCHSHOP_ITEM_NUMBER];
	
	SI16	m_siHandPoint[MAX_SEARCHSHOP_ITEM_NUMBER];
	SI16	m_siSkillLevel[MAX_SEARCHSHOP_ITEM_NUMBER];
#endif
	
	cltSearchShopData();
	void Init();
	
	void CopyGeneralData(cltSearchShopData *pclData);
	void CopyData(SI32 index1, SI32 index2, cltSearchShopData *pclData);
	void ClearData(SI32 index1);
	void MoveData(SI32 index1, SI32 index2, cltSearchShopData *pclData);
	void Set(cltSearchShopData *pclData);
};

//---------------------------------
// 피격 정보. 
//---------------------------------
class cltBeHitInfo{
public:
	bool 			bBeAttackedSwitch;
	SI16			siAttackerID;

	cltBeHitInfo();
	void Init();
	void Set(bool bhitswitch, SI16 attackerid);
};


class cltJim{
public:
	SI32	siJimHostCharUnique;		// 찜한 주체 캐릭터. 
	SI32	siJimSlaveCharUnique;		// 찜당한 캐릭터. 
	DWORD	dwJimFrame;					// 찜당한 프레임. 
	DWORD	dwNewJimFrame;				// 신규로 찜당한 프레임. 

	cltJim();
	void Init();

	bool SetJim(SI32 slavecharunique);	// 찜하다. 
	bool SetBeJimed(SI32 hostcharunique, DWORD frame);		// 찜 되다. 
};


// 사용자의 로그인시 서버가 클라이언트로 보내는 캐릭터의 정보를 담을 클래스
// 원래 아래에 있는 cltUserCharInfo 를 사용하면 가장 좋겠지만 cltPersonInfo 의 용량이 
// 너무 크고 캐릭터 선택 화면에서 굳이 필요없는 정보까지도 포함하고 있으므로 별도의 클래스를 
// 만들었다. 
class cltCharListInfo{
public:
	cltPBasicInfo			clBI;			// 캐릭터의 가장 기본이 되는 정보. 
	cltPIPInfo				clIP;			// 핵심적인 정보. 	
	/*
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_SHAPE_ITEM>		clCharItem;
#else
	cltItem					clCharItem[MAX_SHAPE_ITEM];		// 아이템 보유 등과 관련된 정보.
#endif
	*/
	SYSTEMTIME				DelCharTime;	// 케릭터삭제유예기간을 위해 추가한 시스템시간
	
public:	

	cltCharListInfo();
	void Init();
	void Set(cltCharListInfo* pclinfo);
};


//-----------------------------------
// 소환수의 상태 
//-----------------------------------
#define SUMMON_STATUS_TAKEOUT		1
#define SUMMON_STATUS_TAKEIN		2
#define SUMMON_STATUS_DEAD			3

#define SUMMON_ATTACKTYPE_ACTIVE	1
#define SUMMON_ATTACKTYPE_AVOID		2

#define SUMMON_IDENTITY_NORMAL		0
#define SUMMON_IDENTITY_ITEMMALL	1


#define LIMIT_LEVEL_HATCH_DOLL		20		// Level이 20이 되어야 소환수 사용할수 있다.	
#define HATCH_SUMMON_PERCENT		60		// 10% 확률 
#define BASICTICKET_SUMMON_RANK_EVOLVE	7	// 6 단계 까지는 기본 티켓으로 진화가 된다.
#define LIMIT_SUMMON_RANK_EVOLVE		1000	// 소환수 진화 제한 레벨 (제한이 없다고 봐야지 )
#define LIMIT_SUMMON_ADD_SCROLL_PERCENT	300	// 소환수 스크롤 강화 PERCENT (1000 분율)

inline SI32 GetSummonTicketNum(SI32 SummonRank)
{

	if( SummonRank < BASICTICKET_SUMMON_RANK_EVOLVE )	// 6등급 미만일때는 진화이용권 1장으로 충분하다.
	{
		return 10 ;
	}

	SI32 siTicketNum = 1;

	siTicketNum = ( SummonRank/8 + 1 ) * 10 ;

	if ( SummonRank == 48 )
	{
		return 200 ;
	}
	else if ( SummonRank == 46 || SummonRank == 47)
	{
		return 100 ;
	}
	else if ( SummonRank >= 39 )
		return min(50, siTicketNum);	// 축돌 갯수 공식	

	return min(40, siTicketNum);		// 축돌 갯수 공식	
}

// 현재 6단계 미만인 소환수 일경우 티켓으로 진화 시킨다.
inline SI32 GetSummonTicketUnique(SI32 SummonRank)
{
	if( SummonRank < BASICTICKET_SUMMON_RANK_EVOLVE )	// 6등급 미만일때는 진화이용권 1장으로 충분하다.
	{
		return ITEMUNIQUE(24015);
	}

	return ITEMUNIQUE(5370);		// 축돌 갯수 공식	
}

//inline SI32 GetSummonTicketNum(SI32 SummonRank)
//{
//	SI32 siTicketNum = 1;
//	siTicketNum = SummonRank/8 + 1;
//
//	if ( SummonRank >= 39 )
//		return min(5, siTicketNum);					// 진화이용권 갯수 공식	
//
//	return min(4, siTicketNum);					// 진화이용권 갯수 공식	
//}




//-------------------------------------------
// 캐릭터가 이동하게 되는 경로.
//-------------------------------------------
#define MAX_CHAR_MOVE_POINT 10
class cltMovePointList{
public:
	SI32	siPointNum;					// 경로의 개수. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltPos, MAX_CHAR_MOVE_POINT>	clPos;
#else
	cltPos	clPos[MAX_CHAR_MOVE_POINT];	// 실제 이동 경로. 
#endif

	cltMovePointList();
	void Init();
	bool AddPos(SI32 six, SI32 siy);
	bool IsArrive(SI32 six, SI32 siy);		// 첫번째 지점에 도착했는가?

	// 첫번째 경유지 도착처리.
	void Arrive();
};

//------------------------------------------
// 캐릭터 이동 관련 정보. 
//------------------------------------------
class cltCharMoveInfo{
public:
	SI32			siDX, siDY;			// 목적지 
	SI32			siMovePercent;		// 이동 %
	SI32			siSparePercent;		// 자투리 %
	SI32 			siNX, siNY;			// 캐릭터의 다음 위치 
	SI32			siStepWidth;        // 이동 단위

	DWORD			dwLastMoveClock;	// 마지막으로 이동조치한 시간. 	


	cltCharMoveInfo();
	void Init();
	
	SI32 IncMovePercent(SI32 movespeed);		// 이동 %를 증가시킨다. 
};



class cltCharCommonSimpleVary{
public:
	bool	bForceparametaBoxActionSwitch;					// ParametaBoxAction을 강제로 수행하게 하는 변수. 
	DWORD	dwLastCheckParametaBoxActionClock;				// ParametaBoxAction을 마지막으로 수행한 시간. 

	cltCharCommonSimpleVary();
};

class cltFairyStoreInfo
{
public:
	SI08	siIndex;
	
	UI08 	uiDateYear;
	UI08 	uiDateMonth;
	UI08 	uiDateDay;
	UI16 	uiDateUseDay;

	cltFairyStoreInfo()
	{
		Init();
	}

	void Init()
	{
		siIndex			= 0;
		uiDateYear		= 0;
		uiDateMonth		= 0;
		uiDateDay		= 0;
		uiDateUseDay	= 0;
	}

	void Set(cltFairyStoreInfo* pclFairyStoreInfo)
	{
		if(pclFairyStoreInfo == NULL)		return;

		siIndex			= pclFairyStoreInfo->siIndex;
		uiDateYear		= pclFairyStoreInfo->uiDateYear;
		uiDateMonth		= pclFairyStoreInfo->uiDateMonth;
		uiDateDay		= pclFairyStoreInfo->uiDateDay;
		uiDateUseDay	= pclFairyStoreInfo->uiDateUseDay;
	}
};

class cltFairyInfo
{
public:
	// 말 저장소 3개
	cltFairyStoreInfo	clFairyHorseStore[3];

	// 아이템 저장소 3개
	cltFairyStoreInfo	clFairyItemStore[3];

	// DB에서 받아서 저장하고 있는 현재 저장소
	SI16			siCurFairyHorseStore;
	SI16			siCurFairyItemStore;

	// 말 5개
	cltHorse		clFairyHorse[5];

	// 아이템 30개
	cltItem			clFairyItems[30];
};

//[진성] 스킬북으로 획득한 마법
class cltSkillBookMagic
{
public:
	enum 
	{
		LIMIT_DAY = 12,	// 마법 사용 가는 시간.(일)
	};


	cltSkillBookMagic( void )	
	{
		Clear();
	}
	void Clear( void )
	{
		m_siSkillUnique = 0;
		m_siMagicKind	= 0;
	}

	cltSkillBookMagic( SI32 _siSkillUnique, SI32 _siMagicKind, SYSTEMTIME* _pstStartTime = NULL )	
	{
		m_siSkillUnique = _siSkillUnique;
		m_siMagicKind	= _siMagicKind;

		if( _pstStartTime )
			memcpy( &m_stTime, _pstStartTime, sizeof(m_stTime) );
		else
			ZeroMemory( &m_stTime, sizeof(m_stTime) );	
	}
	cltSkillBookMagic( cltSkillBookMagic* _pclSkillBookMagic )	
	{
		memcpy( this, _pclSkillBookMagic, sizeof(*this) );
	}
	~cltSkillBookMagic(){}

	void		Set( cltSkillBookMagic* _pclSkillBookMagic )		{	memcpy( this, _pclSkillBookMagic, sizeof(*this) );	}

	void		SetSkillUnique( SI32 _siSkillUnique )				{	m_siSkillUnique = _siSkillUnique;					}
	SI32		GetSkillUnique( void )								{	return m_siSkillUnique;								}

	void		SetMagicKind( SI32 _siMagicKind )					{	m_siMagicKind = _siMagicKind;						}
	SI32		GetMagicKind( void )								{	return m_siMagicKind;								}

	void		SetStartTime( SYSTEMTIME* _pstStartTime )			{	memcpy( &m_stTime, _pstStartTime, sizeof(m_stTime) );	}
	SYSTEMTIME*	GetStartTime( void )								{	return &m_stTime;									}

	bool		IsTimeOver( SYSTEMTIME* _stCurTime )				
	{
		if( 0 >= m_siMagicKind || 0 >= m_siSkillUnique )	return false;
		if( 0 == m_stTime.wYear )							return false;
		 		
		if( LIMIT_DAY <= DiffDate( _stCurTime, &m_stTime ) )
		{
			m_stTime.wYear = 0;
			return true;
		}

		return false;
	}

	SI32		m_siSkillUnique;	// 마법과 연결되있는 장비(검, 도끼, 창등...)
	SI32		m_siMagicKind;		// 마법의 종류
	SYSTEMTIME	m_stTime;			// 마법 획득 시각 (현실시간)
};

//[진성] 스킬북으로 보조 마법
class cltSkillBookSubMagic
{
public:
	enum 
	{
		LIMIT_DAY = 30,	// 마법 사용 가는 시간.(일)
	};

	cltSkillBookSubMagic( void )	
	{
		ZeroMemory(this, sizeof(*this));	
	}
	cltSkillBookSubMagic( SI32	_siSkillUnique, SI32 _siParentMagicKind, SI32 _siMagicKind, SYSTEMTIME* _pstStartTime = NULL)	
	{
		m_siSkillUnique		= _siSkillUnique;
		m_siParentMagicKind = _siParentMagicKind;
		m_siMagicKind		= _siMagicKind;

		if( _pstStartTime )
			memcpy( &m_stTime, _pstStartTime, sizeof(m_stTime) );
		else
			ZeroMemory( &m_stTime, sizeof(m_stTime) );	
	}
	cltSkillBookSubMagic( cltSkillBookSubMagic* _pclSkillBookSubMagic )	
	{
		memcpy( this, _pclSkillBookSubMagic, sizeof(*this ) );
	}
	~cltSkillBookSubMagic()	{}

	void Clear( void )
	{
		m_siParentMagicKind = 0;
		m_siMagicKind		= 0;
	}

	void		Set( cltSkillBookSubMagic* _pclSkillBookSubMagic )		{	memcpy( this, _pclSkillBookSubMagic, sizeof(*this) );	}

	void		SetSkillUnique( SI32 _siSkillUnique )					{	m_siSkillUnique = _siSkillUnique;						}
	SI32		GetSkillUnique( void )									{	return m_siSkillUnique;									}

	void		SetParentMagicKind( SI32 _siParentMagicKind )			{	m_siParentMagicKind = _siParentMagicKind;				}
	SI32		GetParentMagicKind( void )	const						{	return m_siParentMagicKind;								}

	void		SetMagicKind( SI32 _siMagicKind )						{	m_siMagicKind = _siMagicKind;							}
	SI32		GetMagicKind( void )		const						{	return m_siMagicKind;									}

	void		SetStartTime( SYSTEMTIME* _pstStartTime )				{	memcpy( &m_stTime, _pstStartTime, sizeof(m_stTime) );	}
	SYSTEMTIME*	GetStartTime( void )									{	return &m_stTime;										}

	bool		IsTimeOver( SYSTEMTIME* _stCurTime )				
	{
		if( 0 >= m_siMagicKind || 0 >= m_siParentMagicKind )	return false;
		if( 0 == m_stTime.wYear )								return false;

		if( LIMIT_DAY <= DiffDate( _stCurTime, &m_stTime ) )
		{
			m_stTime.wYear = 0;
			return true;
		}

		return false;
	}

	friend bool operator==( const cltSkillBookSubMagic& _LValue, cltSkillBookSubMagic& _RValue );

	SI32		m_siSkillUnique;		// 마법과 연결되있는 장비(검, 도끼, 창등...)
	SI32		m_siParentMagicKind;	// 보조마법과 연결되있는 마법
	SI32		m_siMagicKind;			// 보조마법의 종류
	SYSTEMTIME	m_stTime;				// 마법 획득 시각 (현실시간)
};

//[진성] 스킬북관련 캐릭터 정보
#define MAX_SKILLBOOK_SUBMAGIC_NUM 30 // 보조마법의 개수는 최대 30개이다.
#define SKILLBOOK_MAGIC_NUM			6
class cltCharSkillBookInfo : public NkMemory 
{
public:
	cltCharSkillBookInfo( void )	
	{
		Clear();
	}
	~cltCharSkillBookInfo( void )	
	{
		m_clSkillBookSubMagicList.RemoveAll();
	}
	void							Clear( void );
	bool 							IsSkillBookMagic( void );
	void							SetSkillBookMagic( IN SI32 _siIndex, cltSkillBookMagic* _pclAddSkillBookMagic );
	void 							AddSkillBookMagic( IN SI32 _siAttackType, IN cltSkillBookMagic* _pclAddSkillBookMagic );
	cltSkillBookMagic*				GetSkillBookMagicIndex( IN SI32 _siIndex );
	void 							DelSkillBookMagic( IN SI32 _siAttackType );
	void 							AddSkillBookSubMagic( IN cltSkillBookSubMagic _clAddSkillBookSubMagic );
	cltSkillBookSubMagic*			FindSkillBookSubMagicFromParentMagic( IN SI32 _siParentMagic );
	stMagicAttributeData*			FindMagicAttributeDataFromParentMagic( IN SI32 _siParentMagic, OUT SI32& _siSubMagicKind );
	void							DelSkillBookSubMagic( IN const cltSkillBookSubMagic _clAddSkillBookSubMagic );
	SI32							GetIndexFromAttackType( IN SI32 _siAttackType );
	cltSkillBookMagic*				GetSkillBookMagic( IN SI32 _siAttackType );			
	NkList<cltSkillBookSubMagic>*	GetSkillBookSubMagicList( void );
	bool							IsTimeOver( IN cltCharServer* _pclChar );
	bool							IsFullSubMagic( IN SI32 _siSkillunique );
	
		
private:
	NSafeTArray<cltSkillBookMagic, SKILLBOOK_MAGIC_NUM>		m_clSkillBookMagic;
	NkList<cltSkillBookSubMagic>							m_clSkillBookSubMagicList;
};

// 캐릭터에게 피해를 주는 것.
class cltCharCurseEffect
{
public:
	enum CURSE_EFFECT
	{
		CURSE_NONE		= 0x00000000,
		CURSE_ATTACK	= 0x00000001,
		CURSE_DEFENSE	= 0x00000002,
		CURSE_DODGE		= 0x00000004,
		CURSE_HIT		= 0x00000008,
	};

	cltCharCurseEffect()	{ m_dwCurseType = CURSE_NONE;	}
	~cltCharCurseEffect()	{								}

	void SendMsgCurseInfo( SI32 _siTargetID );
	
	void Clear( void )
	{
		ZeroMemory( this, sizeof(*this) );
		
		m_dwCurseType = CURSE_NONE; // 하나만 적용
	}

	void SetCurse( cltCharCurseEffect* _pclCharCurseEffect, SI32 _siTargetID )
	{
		m_dwCurseType   = _pclCharCurseEffect->m_dwCurseType;   
		m_siPara		= _pclCharCurseEffect->m_siPara;		
		m_dwStartClock  = _pclCharCurseEffect->m_dwStartClock ;			
		m_dwClock		= _pclCharCurseEffect->m_dwClock;		

		SendMsgCurseInfo( _siTargetID );
	}

	void SetCurse( DWORD _dwCurse, DWORD _dwClock, SI32	_siPara, SI32 _siTargetID )
	{ 
		m_dwCurseType	= CURSE_NONE; // 하나만 적용

		m_dwCurseType   = _dwCurse; 
		m_siPara		= _siPara;
		m_dwStartClock  = pclClient->CurrentClock;			
		m_dwClock		= _dwClock;

		SendMsgCurseInfo( _siTargetID );
	}

	bool IsCurse( DWORD _dwCurse, SI32 _siTargetID )
	{
		switch( m_dwCurseType )
		{
		case CURSE_ATTACK:
		case CURSE_DEFENSE:
		case CURSE_DODGE:
		case CURSE_HIT: 
			{
				if(	   pclClient->GameMode == GAMEMODE_SERVER
					&& (DWORD)TABS(pclClient->CurrentClock - m_dwStartClock) > m_dwClock )	
				{
					m_dwCurseType = CURSE_NONE; 
					SendMsgCurseInfo( _siTargetID );
				}
			}
			break;
		}
		return m_dwCurseType & _dwCurse ? true : false;
	}

	DWORD GetCurse( SI32 _siTargetID ) 
	{ 
		switch( m_dwCurseType )
		{
		case CURSE_ATTACK:
		case CURSE_DEFENSE:
		case CURSE_DODGE:
		case CURSE_HIT: 
			{
				if(    pclClient->GameMode == GAMEMODE_SERVER
					&& (DWORD)TABS(pclClient->CurrentClock - m_dwStartClock) > m_dwClock )	
				{
					m_dwCurseType = CURSE_NONE; 
					SendMsgCurseInfo( _siTargetID );
				}
			}
			break;
		}

		return m_dwCurseType;
	}

	bool IsTimeOver( SI32 _siTargetID )
	{
		if( m_dwCurseType == CURSE_NONE ) return false; 

		if(    pclClient->GameMode == GAMEMODE_SERVER
			&& (DWORD)TABS(pclClient->CurrentClock - m_dwStartClock) > m_dwClock )	
		{
			m_dwCurseType = CURSE_NONE; 
			SendMsgCurseInfo( _siTargetID );
			return true;
		}
		return false;
	}

	SI32 GetPara( void ) 
	{ 
		return m_siPara;
	}

	SI32 Dead( SI32 _siTargetID ) 
	{ 
		m_dwCurseType = CURSE_NONE; 
		SendMsgCurseInfo( _siTargetID );
		return true;
	}

private:
	DWORD	m_dwStartClock;	// 시작 시간.
	DWORD	m_dwClock;		// 지속 시간.
	DWORD	m_dwCurseType;
	SI32	m_siPara;
};





//-------------------------------------------
// cltCharCommon(클라이언트 서버에서 각각 사용되는 캐릭터 클래스의 Parent
//-------------------------------------------
class cltCharCommon : public ITZPerson{
protected:
	cltCharManager* pclCM;
	SI32 			siCenterX, siCenterY;           // 캐릭터의 중심 좌표(절대좌표) 

	//------------------------------
	// 명령 
	//------------------------------
	cltOrderDataInfo	clCurOrder;					// 현재 수행하는 명령. 
	cltOrderDataInfo	ReserveOrder;				// 다음 수행할 명령. 

	bool				bBusySwitch;   				// 수행중인 일이 있는가 ? (true이면 false가 될때 까지 다른 일이 할당되지 않는다.) 
	SI16				siSubOrder;					// 수행중인 작업의 서브 단계. 
	
	//----------------------------
	// 이동 
	//----------------------------
	cltMovePointList	clMovePointList;				// 캐릭터의 이동경로. 
	SI16				siMoveMapIndex;					// 이동해 가야할 곳의 맵 인덱스. 

	//-----------------------------
	// 장인 정보 
	//-----------------------------
	SI16				siReservedTopSkillUnique;		// 설정 예약되어 있는 탑 기술 
public:

	SI16				siAccountStatus; 
	SI32				siIDNUM;
	

	//---------------------------
	// 지도 
	//---------------------------
	cltMapCommon*		pclMap;							// 캐릭터가 활동할 지도 

 
	//-----------------------------
	// 신분
	//-----------------------------
    SI16				siID;
	SI16				CharUnique;
	cltIdentity			clIdentity;
	SI16				siChildCharUnique;
	SI16				siSummonIndex;		// 주인 입장에서 현재 나와있는 소환수의 유니크. 

	//-----------------------------
	// [영훈] 수호정령_090525
	//-----------------------------
	SI16				siMySoulGuardUnique;	// 내가 불러낸 수호정령 유니크
	DWORD				clockSoulGuardCoolTime;	// 특수능력 쿨타임(여기 설정된 시간 이상이 되어야 능력을 씀)
	cltSoulGuard*		pclSoulGuard;			// 수호정령

	//-----------------------------
	// []
	//-----------------------------
	cltCharSkillBookInfo*	pclCharSkillBookInfo;
	cltCharCurseEffect		m_clCharCurseEffect;

	//--------------------------------
	// 제조를 위한 공간 저장
	//--------------------------------
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, 29>		siInvMakeItemIndex;
#else
    SI32			siInvMakeItemIndex[29];
#endif

	//--------------------------------
	// 인챈트를 위한 공간 저장
	//--------------------------------
	SI16			siOrgInvEnchantInx;  
	SI16			siMatInvEnchantInx;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, NOW_ENCHANT_NUMBER>	siElementEnchantInx;
#else
	SI16			siElementEnchantInx[NOW_ENCHANT_NUMBER];
#endif
	SI16			siElementType;
	
	//윈도우 모드를 위한 포인트 아이템 저장
	cltItem			clMouseInvItem;
	SI16			siMouseInvItem;

public:
#ifdef _SAFE_MEMORY
	static NSafeTArray<SI32, MAX_CHAR_NUMBER>		CharUniqueInfo;
#else
	static	SI32	CharUniqueInfo[MAX_CHAR_NUMBER];	
#endif

	bool			bGodMode;
	bool			bOneKill;			// 원킬모드

	cltCharAddStatusInfo clAddInfo;					// 특수 캐릭터에게만 존재하는 정보들 

	//----------------------------
	// 공격 대상 
	//----------------------------
	cltAttackTarget	clAttackTarget;
	SI16 siCurrentAttackType;					// 현재 공격동작 타입 

protected:

	//-----------------------------
	// 속성들 
	//----------------------------
	SI32			Atb;
	SI32			DefenceAtb;


	BOOL			bActiveNPC;



public:
	cltPersonInfo*			pclCI;					// DB에 저장되어야 캐릭터의 중요정보를 모은 공간. 
	cltParametaBox			clPB;					// 캐릭터의 행동을 제어하는 파라메타들의 클래스. 

	cltKindInfo*			pclKI;
	cltGMCommandKindInfo*	pclGMCommandKindInfo;

	//--------------------------
	// 친구 정보.
	// cltPersonInfo에서 분리
	// 메모리 단축 - 포인터로 변경
	//--------------------------
	//cltFriendInfo	clFriendInfo;
	cltFriendInfo* pclFriendInfo;

	//_LEON_GUILD
	//------------------------------------
	// 개인별 길드경험치 누적 관련 임시 변수
	//-----------------------------------
    SI32					siExpForMyGuild;		// 길드 탈퇴시 초기화 필요
	//------------------------------------
	// 개인별 길드이익금 누적 관련 임시 변수
	//-----------------------------------
    cltMoney				clProfitForMyGuild;		// 길드 탈퇴시 초기화 필요


	//------------------------------------
	// pclCM
	//-----------------------------------
	void SetpclCM(cltCharManager* pclcm);

	//-------------------------------------------------------------
	// 충돌 관련 변수. 
	//------------------------------------------------------------
	SI32 			SelfBlock;                   				// 자체 충돌 속성 
	DWORD 			Block;                         				// 충돌하는 속성 

	BOOL 			SetIDSwitch;                            // 유닛을 지도상에 설정했는지 여부를 기억한다.

	//----------------------------------
	// 출력과 관계된 변수들 
	//----------------------------------
	SI16 			Animation;					// 어떤 종류의 애니메이션을 수행하는가?
	SI16			HeroAnimation;
	bool 			bUpdateAnimationSwitch;		// 애니메이션을 갱신할지여부를 알려준다. 
	SI16 			Frame;                		// 현재 어느 단계를 수행하고 있는가?

	// 정지 관련 
	SI16 			MaxWaitType;   // 이 유닛이 정지 애니메이션으로 가질 수 있는 애니의 갯수 
	SI16 			WaitType;       // 현재 이 유닛이 취하고 있는 정지 애니 

	//----------------------------
	// 방향 
	//----------------------------
	SI16			siDirection;                     				// 방향 
	SI16			TempDirection;                				// 16방위로 존재하는가?
	SI16			TargetDirection;               				// 목표의 방위 

	//------------------------------
	// 공격관련 
	//------------------------------
	DWORD			dwAttackGiveUpDelay;		// 공격 추적 시간.( 이 시간을 벗어날 때까지 실제로 공격하지 못하면 공격 포기) 
	DWORD			dwLastAttackClock;			// 마지막 공격 성공 시간. 
	DWORD 			dwLastAttackIntervalClock;
	DWORD			dwLastMagicClock;			// 마지막으로 공격중 마법을 구현하려고 했던 시간. 

	DWORD			dwLastTorpedoClock;			// 마지막으로 공격중 어뢰를 쏠려고 했던 시간

	SI16			ActiveWeapon;				// 현재 활성화된 Weapon 1 or 2
	SI16			ActiveClothes;				// 현재 보여지는 옷 
	SI16            ActiveHat;

	//---------------------------
	// 이동
	//---------------------------
	cltCharMoveInfo clMoveInfo;

	//---------------------------
	// 농사 
	//---------------------------
	SI16 siFarmX, siFarmY;


	//---------------------------
	// 채굴
	//---------------------------
	SI16 siMineX, siMineY;


	// -----------
	// 애니메니션 
	//------------
	DWORD		dwAniStartClock;
	cltAniInfo	clAniInfo;
	CCharacterDrawing* pclCharDraw;						// 조합형 캐릭터 출력용 클래스. 
	bool		bHitDoneSwitch;							// 타격이 이미 가해졌음. 

	//------------------------------------
	// 현재 선택된 마법 
	//------------------------------------
	SI16	siCurrentMagic;
	SI08	siCurrentMagicCastType;

	//------------------------------------
	// 내부 변수들. 
	//------------------------------------
	cltInnerStatusInfo		clInnerStatus;

	//------------
	// 중독 관련 
	//------------
	SI16	siPoisonCharUnique;			// 나를 중독시킨 캐릭터 유니크. 
	DWORD	dwPoisonLastClock;			// 중독의 지연시간. 
	DWORD	dwPoisonStartClock;			// 중독 시작시간. 
	SI32	siPoisonPower;				// 중독의 강도. 
	DWORD	dwPoisonCheckClock;			// 일정시간간격마다 검사해서 Life를 줄여주기위한 임시 시간 변수 
	

	//-------------
	// 얼음 관련 
	//-------------
	DWORD	dwIceLastClock;
	DWORD	dwIceStartClock;

	//----------------------
	// 얼음성 보스 얼음
	//-----------------------
	DWORD	dwFrozenLastClock;
	DWORD	dwFrozenStartClock;


	//----------------------
	// 스턴
	//-----------------------
	DWORD	dwStunLastClock;
	DWORD	dwStunStartClock;


	//-------------
	// 공격력 저하 저주 관련 
	//-------------
	DWORD	dwCurseAttackStartClock;
	DWORD	dwCurseAttackLastClock;
	SI32	siCurseAttackPower;			// 강도. 

	//-------------
	// CRITICAL 저하 저주 관련 
	//-------------
	DWORD	dwCurseCriticalStartClock;
	DWORD	dwCurseCriticalLastClock;
	SI32	siCurseCriticalPower;		// 강도. 
		
	//-------------
	// DEFENSE 저하 저주 관련 
	//-------------
	DWORD	dwCurseDefenseStartClock;
	DWORD	dwCurseDefenseLastClock;
	SI32	siCurseDefensePower;		// 강도. 

	//-------------
	// MOVESPEED 저하 저주 관련 
	//-------------
	DWORD	dwCurseMovespeedStartClock;
	DWORD	dwCurseMovespeedLastClock;
	SI32	siCurseMovespeedPower;		// 강도. 

	//-------------
	// MOVESPEED 증가 버프 관련 
	//-------------
	DWORD	dwIncreaseMovespeedStartClock;
	DWORD	dwIncreaseMovespeedLastClock;
	SI32	siIncreaseMovespeedPower;		// 강도. 

	//------------
	// 마법의 불 관련 
	//------------
	SI16	siMagicFireCharUnique;			// 나를 불붙게한 캐릭터 유니크
	DWORD	dwMagicFireLastClock;			// 불의 지연시간. 
	DWORD	dwMagicFireStartClock;			// 불 시작시간. 
	SI32	siMagicFirePower;				// 불의 강도. 
	DWORD	dwMagicFireCheckClock;			// 일정시간간격마다 검사해서 Life를 줄여주기위한 임시 시간 변수 

	//-------------
	// DRAIN 관련 
	//-------------
	bool	bDrainSwitch;
	SI16	siDrainFrame;

	UI32	uiStaminaFrame;
	
	//--------------
	// 피격
	//--------------
	bool bHitSwitch;		// 피격 중인지 여부. 

	//-------------------
	// 전쟁 
	//-------------------
	SI32	siWarEnemyKillNumber;	// 전쟁에서 처치한 적의수. 
	SI32	siWarKilledNumber;		// 전쟁중 사망한 횟수. 
	SI16	siBlackArmyWarMapIndex; // 흑의군단 전쟁맵 인덱스

	//-------------------
	// 흑의군단 전쟁 
	//-------------------
	SI32	siBlackWarEnemyKillNumber;	// 전쟁에서 처치한 적의수. 
	SI32	siBlackWarKilledNumber;		// 전쟁중 사망한 횟수. 
	SI32	siBlackWarEnemyKillScore;			// [추가 : 황진성 2008. 1. 21 => 전쟁에서 처치한 적의 점수.]
	SI32	siBlackWarEnemyKillVillageScore;	// [추가 : 황진성 2008. 1. 23 => 전쟁에서 처치한 적에 해당하는 내 마을 점수.]

	
	//------------------------------------
	// 모후모후 이벤트
	//------------------------------------
//	SI08	siMofuMofuEventField;		// 모후모후 이벤트 진행 여부 - 모후병사를 통해 들어간 필드번호

	//------------------------------------
	// 찜
	//------------------------------------
	cltJim	clJim;

	//------------------------------------
	// 파티 인덱스.
	//------------------------------------
	SI16 siPartyIndex;
	
	//------------------------------------
	// 만들어진 프레임
	//------------------------------------
	DWORD dwMakeFrame;

	//-------------------------------------
	// 선공 방어 여부. 
	//-------------------------------------
	bool bNoFirstAttackSwitch;
	
	//-------------------------------------
	// ICE 방어 여부. 
	//-------------------------------------
	bool bNoIceAttackSwitch;

	//-------------------------------------
	// 하트 이펙트 여부.  ( 클라이언트에서만 유효하게 쓰인다. 서버에선 BUF_LOVEPOWER, BUF_FRIENDSHIP 두개가 중복된 경우이다 )
	//-------------------------------------
	bool bHeartEffectSwitch;

	//-------------------------------------
	// 네잎 클로버 여부.  ( 클라이언트에서만 유효하게 쓰인다. 서버에선 BUF_FOURLEAF )
	//-------------------------------------
	bool bFourLeafSwitch;

	//-------------------------------------
	// 사망시 사용되는 변수. 
	//-------------------------------------
	bool	bDyingSwitch;

	//----------------------------
	// 개인상점 데이터 
	//----------------------------
	bool bPersonalShopSwitch;	// 개인상점이 열려 있는가?
	bool bPremiumPersonalShop;	// 프리미엄 개인 상점인가?

	//----------------------------
	// 수리의뢰 데이터
	//----------------------------
	bool bRepairShopSwitch;		// 수리의뢰가 설정되어 있는가?

	//----------------------------
	// 현재 제조중인가 
	//----------------------------
	SI08 siPersonalMakeItemType;


	//bool bPersonalMakeItemSwitch;

	bool bPersonalEnchantItemSwitch;

	//해피빈 이벤트 중인가
	bool bHappyBean;

	//-------------------------------------
	// 네잎 클로버 여부.  ( 클라이언트에서만 유효하게 쓰인다. 서버에선 BUF_FOURLEAF )
	//-------------------------------------
	bool bfarmminingSwitch;
	
	// 현재 레벨업을 해서 레벨업 동작인가
	bool bLevelUpDrawSwitch;

	// [진성] 버프 룰렛 작동중인가? => 2008-6-10
	bool bBuffRouletteSwitch;

	//-------------------------------------	
	// [진성] 엘릭서.
	//-------------------------------------
	bool bElixirSwitch;

	// [성웅] PVP시작 직전 머리위에 카운트 이미지를 그린다.
	bool bPvpCountDown;

	// [성웅] 라쿤이벤트 시작 전에 머리위에 카운트 이미지를 그린다
	bool bRacoonDodgeEventCountDown;

	// [지연] 복불복 이벤트 : 그래픽 리소스 출력 : 5초 카운트
	bool	bBBBEvent_5SecCountDownFlag;
	DWORD	siBBBEvent_5SecCountDown;

	bool	bBBBEvent_StageFailFlag;
	DWORD	siBBBEvent_StageFailCountDown;

	bool	bBBBEvent_StageClearFlag;
	DWORD	siBBBEvent_StageClearCountDown;

	bool	bBBBEvent_TotalStageClearFlag;
	DWORD	siBBBEvent_TotalStageClearCountDown;
	SI16	siBBBEvent_TotalStageClearResultImageIndex;

	//-------------------------------------
	// 변신버프 캐릭터 KIND.  ( 클라이언트에서만 유효하게 쓰인다. 서버에선 BUF_TRANSFORM )
	//-------------------------------------
	SI16 siBufTransformKind;

	//--------------------------
	// 배틀로얄 파워업 모드 상태 인지
	//--------------------------
	bool	bBattleRoyalPowerupMode;

	//-------------------------------------------------
	// 소환수 및 사용자간 컨트롤 // Server에선 선택 캐릭터Unique , Client에선 1일 때는 캐릭터 , 아니면 소환수 Unique
	//-----------------------------------------------
	SI16 siSelectedCharIndex;

	//[진성] 프로모터.
	bool bCellularPhoneAuth;	//[진성] 핸드폰 인증 여부.
	bool bIsPromoter;			//[진성] 프로모터가 있는가?
	
	//KHY - 1220 - 변신시스템.
	//-------------------------------------
	// 캐릭터 변신시 필요한 정보.
	//-------------------------------------
	cltTransFormInfo clTransFormInfo;
	//-------------------------------------
	// 케릭터 옵션
	//-------------------------------------
	cltPCharOptionInfo	clCharOption;
	// [추가 : 황진성 2008. 1. 14 => 자신의 길드가 최강인가.]
	UI32 uiBestGuild;
	//-------------------------------------------------------
	// pvp를 할 상대방 Unique (DB에 저장하지 않는다.)
	SI16 siEnemUnique	;
	// pvp를 기다릴 시간 (DB에 저장하지 않는다.)
	DWORD siPVPWaitTime	;
	// pvp를 기다릴 동안 상대방의 ID를 저장해둔다. (DB에 저장하지 않는다.)
	SI16 siPVPWaitRequestId	;
	SI16 siPVPWaitAcceptId	;
	// PVP 결과
	bool siPVPWin			;	// 승리
	bool siPVPLose			;	// 패배
	bool siPVPDraw			;	// 비김
	//-------------------------------------	
	// [성웅] 현제 PVP 싸우는 시간
	//-------------------------------------
	DWORD	siPVPTime	;
	// pvp끝난시간 시간 (DB에 저장하지 않는다.)
	DWORD siPVPEndTime	;
	// [성웅] PVP리그라서 머리위에 그린다.
	bool bPvpLeague;
	cltPVP_LeagueInfo*	pclPVP_LeagueInfo;
	// 발렌타인데이 이벤트를 위해 자신이 솔로인지 커플인지 대답한 시간.
	DWORD dwValentineAnswerTime	;
	//-------------------------------------------------------

	CMarriage*			pclMarriage;	// 결혼
	CFamily*			pclFamily;		// 가족정보


	//  [8/11/2009 종호_GAWIBAWIBO] 가위바위보 연승 누적 카운트
	SI08				m_siGawiBawiBoCount;		

	cltValentineWarInfo*	pclValentineWar	;

	// 라쿤이벤트 시작 전에 기다리는 5초 (DB에 저장하지 않는다.)
	DWORD siRacoonDodgeWaitTime	;//Switch_10MonthEvent_RacoonDodge

	//------------------------
	// 쿠션 출력 관련
	//------------------------
	SI32 bHaveCushion;

	//-------------------------------------
	// Protected Member Methods
	//-------------------------------------
//protected:
	//------------------------------------
	// 운영자 권한 등급. 
	//------------------------------------
	SI16 siGameMasterLevel;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, 4>	siRareItemRate;
#else
	SI16 siRareItemRate[4];
#endif
	bool bWennyOldMan ;
	SI16 siWennyOldMan ;

	//-----------------------------
	// 피격관련 
	//-----------------------------
	cltBeHitInfo	clBeHitInfo;


	cltCharCommonSimpleVary clCommonSimpleVary;

	SI16	m_siSetItemType;
	SI16	m_siSetItemStep;	//[진성] 셋트 아이템의 단계. 2008-11-12

	SI16	m_siSetItemType2;
	SI16	m_siSetItemStep2;	//[진성] 2번째 셋트 아이템의 단계. 2008-11-12

	SI16	m_siSetItemType3;
	SI16	m_siSetItemStep3;	

	SI16	m_siSetItemType4;
	SI16	m_siSetItemStep4;	

	SI16	m_siSetItemType5;
	SI16	m_siSetItemStep5;	

	//-----------------------------
	// 선거시스템
	//-----------------------------
	bool	bIsElectionStump;						// [영훈] 선거시스템 : 선거유세 진행중인가?
	TCHAR	szCandidate[ Const_Max_Candidate ];		// 지지 후보자 이름
	TCHAR	szMaintenance[ Const_Max_Maintenance ];	// 지지이유

	// KHY - 0404 - 장비 장착, 탈착 여부 확인값. - 아직은 생명력과 마나만 쓰자.
	//SI16 ChangeEquipAbility ;  // 장비 장착, 탈착 여부 확인  - 값은 장착탈착된 장비의 위치다.	

	// cltPersonInfo에서 옮김. - by LEEKH 2008-09-03
	cltSpaceBox*			pclSpaceBoxItem;		// 아이템 보유 등과 관련된 정보.
	cltTreasureBox*			pclTreasureBoxItem;		// 아이템 보유 등과 관련된 정보.

	cltElixir*				pcltElixir;				//[진성] 엘릭서.

	//-----------------------------
	// 버프마법관련
	//-----------------------------
	CNMagicBufMgr			m_clMagicBufMgr;

	//[진성] 도우미. 하루 질문한 횟수를 저장.
	UI08					m_uiDailyQuestionCount;

	//-----------------------------
	// 채팅블럭 관련
	//-----------------------------
	SYSTEMTIME				m_stMuteTime;			// 채팅 블럭 시간

	//-----------------------------
	// 무기술의 달인 - 엠블렘
	//-----------------------------
	UI08					m_uiWeaponSkillMasterEmblem;

	// 피로방지 상태
	enum { TIRED_STATUS_NORMAL = 0, TIRED_STATUS_LITTLE_TIRED, TIRED_STATUS_MUCH_TIRED };

	// 피로방지 시간 상태 경고 값.
	enum { TIRED_NOTICE_1_HOUR = 1, TIRED_NOTICE_2_HOUR, TIRED_NOTICE_2_5_HOUR,TIRED_NOTICE_3_HOUR,
		TIRED_NOTICE_3_5_HOUR,TIRED_NOTICE_4_HOUR ,TIRED_NOTICE_4_5_HOUR,TIRED_NOTICE_5_HOUR,TIRED_NOTICE_5_OVER_HOUR};

public:
	//------------------------------------
	// Virtual Function
	//------------------------------------
	virtual void CalculateRealXY(){};
	virtual BOOL SetNeighbour(){return FALSE;}
	//virtual void SetNewSelectedSwitch(bool mode){}; //[영진] 논리적으로 커먼에는 필요가 없다는 판단하에 과감히 없앰
	virtual void InitWarpInfo(){}
	virtual void SetWarpInfo(const cltWarpInfo* pclCharWarpInfo, SI32 warpunique){}
	virtual void SetWarpToVillage(cltMapManager* pclmapmanager, SI16 villageunique, SI32 warpunique){}

	virtual BOOL IsAbsPtInObj(SHORT x, SHORT y, SHORT& length){return FALSE;};
	virtual void SetUpdateSwitch(SI32 updatetype, bool mode, SI32 para1){}
	virtual void SetNPCGroup(SI32 group){}
	virtual BOOL AddItemToInventory(cltItem* pclitem){return FALSE;}
	virtual bool UseItem(cltItem clitem){return false;}
	virtual void Revive(){};
	virtual BOOL Warp(){return FALSE;}
	virtual	void DrawName(HDC hdc){};
	virtual	void DrawName(){};
	virtual	void DrawChatBalloon(){};
	virtual SI32 GetRX() const{return 0;}
	virtual SI32 GetRY() const{return 0;}
	virtual void CalculateRXY(){};
	virtual void SetMoveStandardPos(SI32 mox, SI32 moy, SI32 mdx, SI32 mdy){}
	virtual void SetHitDrawSwitch(bool mode){};
	virtual void CharBossAction(){};
	virtual void CharRegenerationAction(){};
	virtual void SetCharRegeneration(){};
	virtual void CharCutAction(){}
	virtual void SetCharCut(SI32 mode){}
	virtual	void SetLevelUp(){}
	virtual	void SetMVPEffect(){}
	virtual void SetEnchantEffect(SI08 AniType){}
	virtual void SetQuestEffectReceive(){}
	virtual void SetQuestEffectClear(){}
	virtual void SetSkillLevelUp(){}
	virtual	void SetLastReceveFromClientClock(DWORD clock){}
	virtual void DeleteInCell(){};
	virtual bool Action_Dying();
	virtual void Dead(){}
	virtual void SendLetterMsg(cltLetterUnit* pclletter){};
	virtual void GetBottomPoint(cltPos* pclpos){};
	virtual	void SetMagicDelay(SI32 delay){}
	virtual SI32 GetMagicDelay(){return 10000;}
	virtual void SendDBLevelInfo(SI32 incbonus, SI32 incskillbonus){};
	virtual	bool ChangeArmCode(SI32 itempos, SI32 siunique){return false;}
	virtual void ChangeHorseCode(SI32 code,cltDate* cldate = NULL , SI32 ticket = 0 ){};
	virtual bool GetInitProtectSwitch(){return false;}
	virtual void DeleteOperation(){}
	virtual void DeleteSlave(){};
	virtual bool AfterCharAction() = 0;
	virtual bool IsWarpStatus(){return false;}
	virtual DWORD GetDrawFrame(){return 0;}
	virtual void SetJimShow(){};
	virtual bool MagicInAttackAction(){return false;}
	virtual bool CheckLastMsgClock(DWORD interval){return false;}
	virtual bool MakeItem(cltGameMsgRequest_MakeItemInv* pclinfo){return false;}
	virtual bool SetReserverMakeItem(DWORD productclock, cltGameMsgRequest_MakeItemInv* pclinfo){return false;}
	virtual bool SetMakeDelayDraw(DWORD delayclock){return false;}
	virtual void CellAction(){};
	virtual void DoQuestCheckAndAction(SI32 questclearcond){}
	virtual void FogAction(){}		// 포그관련
	virtual void AddKillMonsterNum(){}	//

	cltCharCommon();
	virtual ~cltCharCommon();

	void CharInit(SI32 id, SI32 charunique, SI32 childcharunique, SI32 summonindex, SI32 dir, cltMapCommon* pclmap, 
		cltIdentity* pclidentityinfo, cltPersonInfo* pclUserCharInfo, cltInnerStatusInfo* pclInnerStatus, SI16 mySoulGuardUnique);

	void SetInfo();		// 캐릭터가 생성되었음을 알린다. 

	SI32 GetCharUnique()const;
	void SetCharUnique(SI32 unique);

	void SetCurrentOrder(SI32 order);
	SI32 GetCurrentOrder()const;

	void SetID(SI32 id);
	SI32 GetID()const;

	SI32 GetX()const;
	SI32 GetY()const;

	void  SetX(SI32 x);
	void  SetY(SI32 y);

	SI32 GetKind()const;		// 종류를 돌려준다.
	void SetKind(SI32 kind);
	cltKindInfo* GetKindInfo(bool bForDraw = false);	// KindInfo 정보를 돌려준다 - 변신 후에는 변신된 캐릭터의 정보가 반환된다. - by LEEKH 2007.12.24
														// 외관변경 버프를 위한 변수 추가 - by LEEKH 2008-09-10

	SI32 GetMapIndex()const;			// 맵 인덱스를 얻어온다. 
	void SetMapIndex(SI32 index);		// 맵 인덱스를 설정한다.  

	SI32 GetCurrentVillageUnique();		// Person이 현재 위치한 마을의 Unique를 구한다. 
	SI32 GetHuntVillageUnique();		// Person이 현재 위치한 수렵장이 있는 마을의 Unique를 구한다. 

	SI32 GetCenterX()const;
	SI32 GetCenterY()const;

	const TCHAR* GetName()const;			// 캐릭터의 이름을 얻어온다. 
	void SetName(const TCHAR* name);

	SI32 GetMana();
	void SetMana(SI32 mana);
	SI32 DecreaseMana(SI32 mana);
	SI32 IncreaseMana(SI32 mana);

	SI32 GetLife();
	void SetLife(SI32 life);

	SI32 IncreaseLife(SI32 life);

	SI32 GetWis()const;

	SI32 GetStr()const;
	
	SI32 GetDex()const;

	SI32 GetVit()const;
	void SetVit(SI32 vit);

	SI32 GetMag()const;
	void SetMag(SI32 mag);

	SI32 GetHnd()const;
	void SetHnd(SI32 hnd);


	SI32 GetSubOrder();			// 수행중인 작업의 서브 단계를 얻어온다.

	SI64 GetMyExp()const;		// 내 자신의 경험치를 구한다.(상대가 나를 처치했을 때 얻을 수 있는 경험치. ) // SI32->SI64 경험치 오버플로우때문에 수정

	SI32 GetAttackRange()const;
	SI32 GetAttackType();

	SI32 GetMaxWeight();		// 들 수 있는 최대 무게를 구한다.
	SI32 GetTotalWeight();		// 현재의 무게 합계를 구한다. 
	
	BOOL IsSameMap(const cltMapCommon* pclmap);
	
	bool IsSBS();				// SBS사용자인가. 

	SI32 GetSummonID();			// 나의 소환수 ID를 얻어온다.
	SI32 GetSummonParentID();	// 소환수의 PARENT ID를 얻어온다.
	SI32 GetSummonIndex();		// 소환수의 Unique Index를 얻어온다.

	// [영훈] 수호정령_090525
	SI32 GetSoulGuardParentCharID( void );	// 수호정령의 부모 CharID를 얻어온다
	SI32 GetSoulGuardCharID( void );

	void	InitGawiBawiBoCount(){m_siGawiBawiBoCount = 0;}  // 가위바위보 연승 정보 초기화
 	SI08	IncreaseGawiBawiBoCount(){return m_siGawiBawiBoCount++;}  // 가위바위보 연승
	SI08	GetGawiBawiBoCount(){return m_siGawiBawiBoCount;}
    
	//-----------------------------------------------------------------------------------
	// KHY - 0404 - 장비 장착, 탈착 여부 확인값. - 아직은 생명력과 마나만 쓰자.
	//-----------------------------------------------------------------------------------
	/*void SetChangeEquipAbility (SI16 equipPos)
	{
		ChangeEquipAbility= equipPos;
	}

	SI16 GetChangeEquipAbility()
	{
		return ChangeEquipAbility;
	}
*/
	//--------------------------------------
	// 속성 
	//--------------------------------------	
	BOOL 			IsAtb(SI32 atb);	// 속성을 구한다. 
	BOOL 			IsAtbAnd(SI32 atb);	// 모든 조건이 충족되어야 
    BOOL 			IsDefenceAtb(SI32 atb);
    
	//---------------------------------------
	// 공격/전투 
	//---------------------------------------
    void GetHitPoint(cltPos* pclpos);
	void DecideAttackPoint(SI32 port, SI32& x, SI32&y );
	BOOL IsInRange(SI32 id, SI32 range);	// id가 range범위안에 있는가?

	BOOL CanAttack( SI32 id, bool pvpflag = false );
	BOOL CanAttack(const cltAttackTarget* pcltarget);
	
	bool CanMagicManaDelay(SI32 magickind, SI32* preturn );							// 마법을 사용하기전에 마나와 시간이 적절한지 검사한다.
	bool CanMagic(SI32 targetid, SI32 magickind,  SI32* preturn, bool PVP = false);	// 마법으로 캐릭터를 공격할 수 있는지 여부를 알려준다. 
	bool CanMagicBuf( SI32 siMagicKind, SI32* psiReturn );							// 버프 마법을 사용할 수 있는지 검사
	
	bool IsEnemyPCvsNOTPC(SI32 myid, SI32 enemyid);		// PC vs !PC 캐릭터를 공격할 수 있는지 여부를 알려준다. 
	bool IsEnemy(SI32 id);					// id가 나의 적인가?
	bool IsEnemyPCvsPC( SI32 enemyid );		// 내가 다른 유저를 공격할 수 있나? 
	
	BOOL IsInMinRange(SHORT id, SHORT minrange);		// id가 나의 최소 범위안에 있는지 확인한다. 
	bool Fire();
	bool FireSub();
	SI32 GetApower();

	BOOL GetBulletUnique_Char(SI16* psiBulletUnique);						// 공격타입과 일치하는 총알(화살)의 유니크를 돌려준다. - 캐릭터
	BOOL GetBulletUnique_Summon(SI32 siSummonIndex, SI16* psiBulletUnique);	// 공격타입과 일치하는 총알(화살)의 유니크를 돌려준다. - 소환수

	//---------------------------------------
	// Action
	//---------------------------------------
	// 리턴값:  FALSE  .  특이 사항 없음 
	//          TRUE   ->  캐릭터를 삭제함 
	BOOL Action();
	bool PreCharAction();

	//---------------------------------------
	// 명령 처리 
	//--------------------------------------
	void InitPreGetOrder();			// 명령을 새로 수여하기 전에 해야할 초기화 
	BOOL GetOrderBuffer();			// 비축된 명령을 꺼내서 수행한다. 
	BOOL PushOrder(cltOrderDataInfo order);		// 명령을 버퍼에 넣는다. 

	//-------------------------------------
	//	캐릭터 지도에 심거나 삭제 
	//-------------------------------------
	BOOL SetID();			// 캐릭터의 흔적을 남긴다 
	BOOL DeleteID();		// 캐릭터의 흔적을 없앤다. 

	//-------------------------------------
    /// 마법 시전. 
	//-------------------------------------
	BOOL Magic();
	void MagicOperation();	
	
	//-------------------------------------
	// 낚시 
	//-------------------------------------
	bool IsFishArea(SI32 x, SI32 y, SI32* pdir);		// 낚시가능 지역인가 ?
	bool FindFishArea(SI32* px, SI32* py);				// 이 캐릭터 주변의 적절한 낚시 위치를 찾는다. 
	
	bool Fish();

	//-------------------------------------
	// 농사 
	//-------------------------------------
	bool CanFarm(SI32 dx, SI32 dy, SI32* ptrnval);		// 농사를 지을 수 있는가 ?

	bool SetFarm(SI32 dx, SI32 dy);
	bool Farm();
	bool FarmAction();

	//-------------------------------------
	// 채굴
	//-------------------------------------
	bool CanMine(SI32 dx, SI32 dy, SI32* ptrnval);

	bool SetMine(SI32 dx, SI32 dy);
	bool Mine();
	bool MineAction();


	//-------------------------------------
	// 앉기
	//-------------------------------------
	bool SitDown(SI32 SitDownAnytype);


	//--------------------------------------------------
	//   이동 관련 함수 
	//--------------------------------------------------
	bool			SetMove(SI32 dx, SI32 dy);		// 캐릭터를 이동하게 만들기 위한 설정을 한다. 

	int 			Move(bool bstraightswitch = false);

	int				GetMovePercent();
	
	BOOL			MoveDone();

	BOOL			CanMoveTo(SI32 x, SI32 y);
	BOOL			MoveTo(SI32 x, SI32 y);

	bool			IsColl(SHORT x, SHORT y);					// 캐릭터가 x, y위치에 있을 수 있는가 없는가?
	bool			IsColl(SHORT x, SHORT y, SHORT block);		// 캐릭터가 x, y위치에 있을 수 있는가 없는가?

	void			MoveInit();

	bool SetBlock();		// 충돌 영역을 설정한다. 


	//---------------------------------
	//공격 관련 함수 
	//---------------------------------
    SI32 Action_Attack();

	BOOL TryAttack();
	BOOL Escape();

	BOOL ReadyToFire();			// fire직전에 확인 
	BOOL SetAttack(SI32 id, bool PVPHit = false);	////  attack_id 적을 공격하게 만드는 함수 

    BOOL PreAttack();			// 본격적인 전투에 들어가기 전에 처리해야 할 것을 처리한다. 

	//-------------------
	// 애니메이션 관련. 
	//-------------------
	SI32 CalcHeroAnimation();	// Hero Animation 코드를 결정한다. 

	void			SetAnimation(SI32 ani);
	virtual BOOL			DoAnimationDelay(BOOL atb = 0);

	//---------------------------
	// 방향전환 관련 
	//----------------------------
    BOOL			ChangeDirection(SHORT destdir);			// 캐릭터의 방향을 전환한다. 
    BOOL			ChangeDirectionNormal(SHORT destdir);	// 캐릭터의 방향을 전환한다. 
    BOOL			ChangeDirection16(SHORT destdir);		// 캐릭터의 방향을 전환한다. 

	//---------------------------------------------
	// EconomyInfo관련. 
	//---------------------------------------------
	GMONEY GetBankMoney()const;				// 전장 잔고를 리턴한다. 
	SI32 GetStockOrderNumber()const;		// 주식 주문 개수를 확인한다. 
	SI32 GetGuildStockOrderNumber()const;	// 길드 주식 주문 개수를 확인한다. 
	SI32 GetMarketOrderNumber()const;		// 물품 주문 개수를 확인한다. 
	SI32 GetNewMarketOrderNumber() const; //[진성] 통합마켓 물품 주문 개수를 확인한다. 

	//-----------------------------------
	// 메시지 전송함수.
	//-----------------------------------
	void SendNetMsg(sPacketHeader* pmsg);

	bool IsValidConnection();		// 연결이 되어 있는 Person인지 여부. 

	//-----------------------------------
	// 말 관련 
	//-----------------------------------
	virtual bool RideHorse();		// 말을 탄다. 
	virtual bool UnrideHorse();		// 말에서 내린다.

	bool HaveHorse();	// 말을 가지고 있는가?


	//----------------------
	// 피격 
	//---------------------
	virtual void SetHitSwitch(bool mode);
	
	bool BeHitOperation();		// 피격 행동을 취한다. 

	//----------------------------------
	// 장인 관련 
	//----------------------------------
	void ChangeTopSkillUnique(SI32 skillunique);		// 최고 장인 등록 여부를 갱신한다. 

	//---------------------------------
	// 정보 텍스트 
	//---------------------------------
	bool GetCharInfoText(TCHAR* buffer);				// 캐릭터의 정보를 텍스트로 정리해서 받는다.

	//------------------------------------
	// 운영자 권한 등급. 
	//------------------------------------
	SI32 GetGameMasterLevel();
	void SetGameMasterLevel(SI32 level);
	SI32 GetRareItemRate(SI32 where);
	void SetRareItemRate(SI32 attack,SI32 mag,SI32 skill,SI32 dur);
	bool GetWennyOldManSuccess();
	SI32 GetWennyOldManMode();
	void SetWennyOldMan(bool success,SI32 mode);

	//---------------------------------
	// 퀘스트 관련 
	//----------------------------------
	bool IsClearCondition(SI32 questtype, cltQuestClearConditionInfo* pclcond);		// 임무의 완료 조건을 충족시키는가?
	bool DoClearCondition(cltQuestClearConditionInfo* pclcond);		// 임무의 완료 조건에서 아이템이 요구되면 아이템을 캐릭터에게서 삭제한다. 
	bool ClearQuest(SI32 questunique);			// 특정 유니크의 퀘스트를 완료했는지 확인한다. 
	SI32 GetMinBit(SI32 type);					// 아직 완료되지 않은 가장 낮은 퀘스트의비트를 찾는다. 
	
	//----------------------------
	// 아이템 
	//-----------------------------
	bool CanUseSummonDoll(SI32 itemunique, SI32 *summonkind,SI32 *summonindex, SI32 *preturn);	// 소환인형을 이용한 소환수의 생성 
	bool CanUseItem(cltItem clitem, SI32* preturn, SI32 siPos);		// 아이템을 사용할수 있는 것인지 확인한다.
	// 아이템을 사용하여 수호정령을 소환할수있는가?
	bool CanUseSoulGuardItem( SI32 siItemUnique, SI16* psiSoulGuardKind, SI16* psiLife, SI16* psiFellowShip, SI32* pReturn );
	bool CanRepairItem( cltItem* pclitem );

	//---------------------------
	// 장비 
	//---------------------------
	bool CheckEquipCondition(cltItemManagerCommon* pclItemManager, SI32 pos, cltItem* pclitem, SI32* preturnval);

	//------------------------------------
	// 현재 선택된 마법 
	//------------------------------------
	void SetCurrentMagic(SI32 magic, SI08 siCastType);
	SI32 GetCurrentMagic();
	SI08 GetCurrentMagicCastType();
	
	//-------------------------------------
	// 이벤트 관련 함수. 
	//-------------------------------------
	void SetEventChar(SI32 eventindex, bool bmode);		// 이벤트 캐릭터로 설정한다. 
	
	//-----------------------------------------
	// 수호천사 관련 
	//-----------------------------------------
	void SetAngel(bool mode);

	//-----------------------------------------
	// 포교 관련 
	//-----------------------------------------
	void SetPolice(bool mode);
	
	void GetShapeEquipInfo(cltShapeEquipInfo* pclequip);		// 내 외양 장비 상황을 얻어온다. 


	// 쿠션 아이템 관련
	SI32 IsHaveCushionItem();

	//KHY - 1220 - 변신시스템.
	bool IsCanTransFormRecover();
	bool CanTransFormAttackType(SI32 siCharKind);

	SI32 GetItemUniqueFromCharKind(SI32 siCharKind);
	SI32 GetCharKindFromItemUnique(SI32 siItemUnique);

	SI32 GetFarmRate(SI32 farmskilllevel);		// 농경을 통해서 얻을 수 있는 작물의 확률. 
	SI32 GetMineRate(SI32 mineskilllevel);		// 채광을 통해서 얻을 수 있는 광물의 확률과 갯수. 

	void ParametaBoxAction();
	void SetParametaBoxAction(bool mode);

	//---------------------------------------------
	// CharCommon-Summon.cpp
	//---------------------------------------------
	SI16 GetCannonCrewDamage(cltKindInfoSet* pclkindmanager,  SI16 SummonPos );		// 포수의 소환수 공격력을 얻어온다.
	SI16 GetGunCrewDamage( cltKindInfoSet* pclkindmanager,  SI16 SummonPos );		// 사수의 소환수 공격력을 얻어온다.
	//bool IsSummonHaveItem(SI32 summonindex);		// 소환수가 물품을 장비하고 있는가? - 안씀

	//---------------------------------------------
	// 워프 관련 함수
	//---------------------------------------------
	bool CanWarpFrom();
	bool CanWarpTo( SI32 siToMapIndex );
	bool CanFreeWarp( SI32 siDestMapIndex );
	bool CanWarpToChar( SI32 siCharID );		// 서버 전용

	void PartyHallItemRemove(SI32 id);				//[진성] 파티홀. 파티장에서 얻는 아이템을 제거.
	void NotFourLeafItemRemove( SI32 id );			//[진성] 네잎크로버가 현재 사용할수 없는 거면 삭제 => 2008-8-14
	void FestivalFoodItemRemove( SI32 id );			//[진성] 축제 음식.
	
	//---------------------------------------------
	// 워프 관련 함수
	//---------------------------------------------
	bool Action_Special();

	// 변경 애야 하는 에니메이션 타입인지 결정한다.
	bool selectRealAnyType( );

	//---------------------------------------------
	// PVP_League 관련 함수
	//---------------------------------------------
	SI16 GetPVP_LeagueEnemyPersonID( void );
	void SetPVP_LeagueEnemyPersonID( SI16 siEnemyID );

	SI16 GetPVP_League_Win( void );
	void SetPVP_League_Win( SI16 siWin );
	void IncPVP_League_Win( SI16 siWin );

	SI16 GetPVP_League_Draw( void );
	void SetPVP_League_Draw( SI16 siDraw );
	void IncPVP_League_Draw( SI16 siDraw );

	SI16 GetPVP_League_Lose( void );
	void SetPVP_League_Lose( SI16 siLose );
	void IncPVP_League_Lose( SI16 siLose );

	SI16 GetPVP_LeagueStraight_Win( void );
	void SetPVP_LeagueStraight_Win( SI16 siWin );
	void IncPVP_LeagueStraight_Win( SI16 siWin );

	SI16 GetPVP_LeagueStraight_Draw( void );
	void SetPVP_LeagueStraight_Draw( SI16 siDraw );
	void IncPVP_LeagueStraight_Draw( SI16 siDraw );

	SI16 GetPVP_LeagueStraight_Lose( void );
	void SetPVP_LeagueStraight_Lose( SI16 siLose );
	void IncPVP_LeagueStraight_Lose( SI16 siLose );

	SI16 GetPVP_LeagueGrade( void );
	void SetPVP_LeagueGrade( SI16 siGrade );

	bool IsPVP_LeagueLatestPersonID( SI32 siDestPersonID );
	void PushPVP_LeagueLatestPersonID( SI32 siDestPersonID );

	void SetPVP_LeagueStraight_Lose_Grade( SI16 siWin );
	SI16 GetPVP_LeagueStraight_Lose_Grade( void );
	void SetPVP_LeagueStraight_Win_Grade( SI16 siWin );
	SI16 GetPVP_LeagueStraight_Win_Grade( void );

	SYSTEMTIME GetPVP_LeagueEndBAttleTime( void );
	void SetPVP_LeagueEndBAttleTime( SYSTEMTIME stEndTiem );

	// 결혼 관련
	BOOL IsMarried( void );
	void SetProposePersonID( SI32 siProposePersonID );
	SI32 GetProposePersonID( void );
	// 가족관련
	SI32 GetMyFamilyClass( void )	;

	//---------------------------------------------
	// 수호정령
	//---------------------------------------------
	BOOL IsSoulGuardStatusOut( void );	// 수호정령이 현재 소환되어져있는 상태인지 검사
};



#endif
