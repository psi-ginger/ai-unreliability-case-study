#ifndef _MAGICBASEINFO_H_
#define _MAGICBASEINFO_H_

#include "../../Common/SMLib/BaseInclude/BaseInclude.h"
//#include <Graphic.h>

#include "../../Client/GlobalImgFile/GlobalImgFile.h"
#include "..\..\Common\CommonHeader.h"

class cltCharCommon;

//----------------------------------------------------------
// 마법의 종류 인덱스로 구별하기 위해서
enum	MAGIC_TYPE
{	
	MAGIC_TYPE_START = 0, 
	MAGIC_TYPE_SWORD = MAGIC_TYPE_START, 
	MAGIC_TYPE_SPEAR,	
	MAGIC_TYPE_AXE,	
	MAGIC_TYPE_STAFF, 
	MAGIC_TYPE_BOW,		
	MAGIC_TYPE_GUN,
	MAGIC_TYPE_MON,
	MAGIC_TYPE_SOULGUARD,
	MAGIC_TYPE_SKILL_BOOK,
	MAGIC_TYPE_LAST	 = MAGIC_TYPE_SKILL_BOOK,
};

enum	MAGIC_GROUP
{	
	MAGIC_GROUP_START		= 1, 
	// [1]
	MAGIC_GROUP_FIREBALL	= MAGIC_GROUP_START, 
	MAGIC_GROUP_BIGWIND,	
	MAGIC_GROUP_THUNDER,
	MAGIC_GROUP_THUNDER1,
	MAGIC_GROUP_WHIRLPOOL,	
	MAGIC_GROUP_FLAME,		
	MAGIC_GROUP_ROCKSLIDE,   
	MAGIC_GROUP_HEAL,		
	MAGIC_GROUP_POISON,
	MAGIC_GROUP_ICE,
	// [11]
	MAGIC_GROUP_DRAIN,
	MAGIC_GROUP_LIGHTINGFIST,
	MAGIC_GROUP_POISONFLAME,
	MAGIC_GROUP_METEOR,	
	MAGIC_GROUP_DROPMONEY,
	MAGIC_GROUP_BLACKINK,
	MAGIC_GROUP_DROPSNOWMAN,
	MAGIC_GROUP_DROPFORK,
	MAGIC_GROUP_CURSEATTACK,	 
	MAGIC_GROUP_CURSECRITICAL,	
	// [21]
	MAGIC_GROUP_CURSEDEFENSE,	
	MAGIC_GROUP_CURSEMOVESPEED,
	MAGIC_GROUP_THROWKNIFE,
	MAGIC_GROUP_LASERBEAM,
	MAGIC_GROUP_TRIMETEOR,
	MAGIC_GROUP_FIREGOUND,
	MAGIC_GROUP_SWORDBOMB,
	MAGIC_GROUP_FREEZEEXPLOSION,
	MAGIC_GROUP_UPPERDRGON,
	MAGIC_GROUP_PALMWIND,
	// [31]
	MAGIC_GROUP_AURABLADE,
	MAGIC_GROUP_TRIPLEDEATH,
	MAGIC_GROUP_BLESSDAMAGE,
	MAGIC_GROUP_BLESSCIRICAL,
	MAGIC_GROUP_BLESSHITRATE,
	MAGIC_GROUP_BLESSRANGE,
	MAGIC_GROUP_BLESSATTACKINTERVAL,
	MAGIC_GROUP_BLESSMPRECOVERYTIME,
	MAGIC_GROUP_BLESSHP,
	MAGIC_GROUP_SUMMONDRAGON,
	// [41]
	MAGIC_GROUP_FIREBURN,
	MAGIC_GROUP_SKELLGHOST,
	MAGIC_GROUP_SWORDMETEOR,
	MAGIC_GROUP_FROZEN,
	MAGIC_GROUP_TWINSPEAR,
	MAGIC_GROUP_FIREWHIRLWIND,
	MAGIC_GROUP_NORTHWIND,
	MAGIC_GROUP_FIRESTORM,
	MAGIC_GROUP_EARTHQUAKE,
	MAGIC_GROUP_SPEAR_DRAGON,
	// [51]
	MAGIC_GROUP_GARGOYLE,
	MAGIC_GROUP_CERBERUS,
	MAGIC_GROUP_FROZENSORCERER,
	MAGIC_GROUP_MARIONETTE,
	MAGIC_GROUP_FROKEN,
	MAGIC_GROUP_ICEHOUND,
	MAGIC_GROUP_FROZENGOLEM,
	MAGIC_GROUP_FROZENKNIGHT,
	MAGIC_GROUP_FROZENQUEEN,
	MAGIC_GROUP_GOTPA,
	// [61]
	MAGIC_GROUP_NINJAPANDA,
	MAGIC_GROUP_RACCOONDOG,
	MAGIC_GROUP_BOSSMONKEY,
	MAGIC_GROUP_TOADPOISON,
	MAGIC_GROUP_BLOODBAT,
	MAGIC_GROUP_WEREWOLF,
	MAGIC_GROUP_PRIEST,
	MAGIC_GROUP_DEVILOUS,
	MAGIC_GROUP_GANGSI,
	MAGIC_GROUP_DRACULA,
	// [71]
	MAGIC_GROUP_HARPY,
	MAGIC_GROUP_MANDRAGORA,
	MAGIC_GROUP_GIRAFFE,
	MAGIC_GROUP_PAPAGOBLIN,
	MAGIC_GROUP_PIRATESOLDIER,
	MAGIC_GROUP_PIRATEARCHER,
	MAGIC_GROUP_BLACKBOSS,
	MAGIC_GROUP_EMPERORCAVALRY,
	MAGIC_GROUP_EMPERORMASTER,
	MAGIC_GROUP_GREATWALLSOLDIER,
	// [81]
	MAGIC_GROUP_GREATWALLMAGICIAN,
	MAGIC_GROUP_EMPERORWARRIOR,
	MAGIC_GROUP_GREATWALLSPIDER,
	MAGIC_GROUP_GREATWALLTWINHEAD,
	MAGIC_GROUP_BEARTANG,
	MAGIC_GROUP_SUPERBEARTANG,
	MAGIC_GROUP_SFROZENGOLEM,
	MAGIC_GROUP_SFROZENQUEEN,
	MAGIC_GROUP_CATPET,
	MAGIC_GROUP_COSMOSRABBIT,
	// [91]
	MAGIC_GROUP_RUDOLPHPET,
	MAGIC_GROUP_DROPGREATWALLASURA,
	MAGIC_GROUP_AMBERSAURUS,
	MAGIC_GROUP_ROLLINGSTEGO,
	MAGIC_GROUP_PUMPKIN,					// 소울 가드 펌킨이 사용 하는 마법
	MAGIC_GROUP_SUMMONDRAGON_STUN,			//[진성] 드래곤.
	MAGIC_GROUP_DRAGONFIRE,					//[진성] 미니 드래곤.

	MAGIC_GROUP_SKILL_BOOK_FLAMEBUSTER,		//[진성] 스킬북
	MAGIC_GROUP_SKILL_BOOK_FLAMEWHIRL,	
	MAGIC_GROUP_SKILL_BOOK_ICERAIN,		
	MAGIC_GROUP_SKILL_BOOK_WETFOG,		
	MAGIC_GROUP_SKILL_BOOK_CRASH,			
	MAGIC_GROUP_SKILL_BOOK_SLING,			
	MAGIC_GROUP_SKILL_BOOK_TORNADO,		
	MAGIC_GROUP_SKILL_BOOK_WINDCUTER,		

	MAGIC_GROUP_SKILL_BOOK_SUB,		
	
	MAGIC_GROUP_CLOWNBALL,
	MAGIC_GROUP_BOUQUET,

	MAGIC_GROUP_LAST = MAGIC_GROUP_BOUQUET,
};

enum	//MAGIC_TYPE_SWORD // 1 ~
{ 
	MAGIC_INDEX_START		= 1,
	MAGIC_SWORD_START		= MAGIC_INDEX_START,
	MAGIC_SWORD_STRIKING	= MAGIC_SWORD_START,
	MAGIC_SWORD_AURABLADE,
	MAGIC_SWORD_POISON,			
	MAGIC_SWORD_SWORDBOMB,
	MAGIC_SWORD_LAST		= MAGIC_SWORD_SWORDBOMB,

};
enum	//MAGIC_TYPE_SPEAR  
{ 
	MAGIC_SPEAR_START		= MAGIC_SWORD_LAST + 1,
	MAGIC_SPEAR_DEATHBLOW	= MAGIC_SPEAR_START,			
	MAGIC_SPEAR_POISON,			
	MAGIC_SPEAR_CURSECRITICAL,
	MAGIC_SPEAR_UPPERDRAGON,
	MAGIC_SPEAR_LAST		= MAGIC_SPEAR_UPPERDRAGON, 
};
enum	//MAGIC_TYPE_AXE 
{ 
	MAGIC_AXE_START			= MAGIC_SPEAR_LAST + 1,
	MAGIC_AXE_HITPOINT		= MAGIC_AXE_START,				
	MAGIC_AXE_POISON,
	MAGIC_AXE_DRAIN,
	MAGIC_AXE_METEOR,
	MAGIC_AXE_LAST			= MAGIC_AXE_METEOR, 
};
//-----------------------------------------------------------------
// 시즌2에 캐릭터 능력치 변경되면서 스태프 마법 순서 변경
//-----------------------------------------------------------------
// !!!!!!!!! 절대 이런짓 하지 마시오!!!!!!!!!!!!!!!!!!!!!!!!!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// !! 아래 두개의 enum .. 개수가 틀리면 안됨
enum MAGIC_TYPE_STAFF_ORG	//MAGIC_TYPE_STAFF 
{ 
	ORG_MAGIC_STAFF_START	 = MAGIC_AXE_LAST + 1,		
	ORG_MAGIC_STAFF_FIREBALL = ORG_MAGIC_STAFF_START,
	ORG_MAGIC_STAFF_BIGWIND,		
	ORG_MAGIC_STAFF_THUNDER, 	
	ORG_MAGIC_STAFF_WHIRLPOOL,		
	ORG_MAGIC_STAFF_FLAME,			
	ORG_MAGIC_STAFF_POISON,
	ORG_MAGIC_STAFF_ICE,
	ORG_MAGIC_STAFF_QUICKMANA,
	ORG_MAGIC_STAFF_GAIN,
	ORG_MAGIC_STAFF_LAST	= ORG_MAGIC_STAFF_GAIN,
};
enum MAGIC_TYPE_STAFF_NEW	//MAGIC_TYPE_STAFF 
{ 
	NEW_MAGIC_STAFF_START	 = MAGIC_AXE_LAST + 1,		
	NEW_MAGIC_STAFF_FIREBALL = NEW_MAGIC_STAFF_START,
	NEW_MAGIC_STAFF_BIGWIND,		
	NEW_MAGIC_STAFF_THUNDER,
	NEW_MAGIC_STAFF_POISON,
	NEW_MAGIC_STAFF_ICE,
	NEW_MAGIC_STAFF_WHIRLPOOL,		
	NEW_MAGIC_STAFF_FLAME,
	NEW_MAGIC_STAFF_QUICKMANA,
	NEW_MAGIC_STAFF_GAIN,
	NEW_MAGIC_STAFF_LAST	= NEW_MAGIC_STAFF_GAIN,
};
//-----------------------------------------------------------------
// !! 위에 두개의 enum .. 개수가 틀리면 안됨
//-----------------------------------------------------------------
// !!!!!!!!! 절대 이런짓 하지 마시오!!!!!!!!!!!!!!!!!!!!!!!!!
//-----------------------------------------------------------------
enum	//MAGIC_TYPE_BOW 
{ 
	MAGIC_BOW_START			= ORG_MAGIC_STAFF_LAST + 1,
	MAGIC_BOW_HITPOINT		= MAGIC_BOW_START,
	MAGIC_BOW_TRIPLEDEATH,
	MAGIC_BOW_ICE,
	MAGIC_BOW_TRIMETEOR,
	MAGIC_BOW_LAST			= MAGIC_BOW_TRIMETEOR,
};

enum	//MAGIC_TYPE_GUN
{ 
	MAGIC_GUN_START			= MAGIC_BOW_LAST + 1,
	MAGIC_GUN_WIDEVISION	= MAGIC_GUN_START,
	MAGIC_GUN_CANNON,
	MAGIC_GUN_ICE,
	MAGIC_GUN_PALMWIND,
	MAGIC_GUN_LAST			= MAGIC_GUN_PALMWIND,
};

enum	//MAGIC_TYPE_MON
{ 
	MAGIC_MON_START		= MAGIC_GUN_LAST + 1,
	MAGIC_MON_FIREBALL  = MAGIC_MON_START,		
	MAGIC_MON_BIGWIND,
	MAGIC_MON_LIGHTINGFIST,
	MAGIC_MON_THUNDER,
	MAGIC_MON_THUNDER1,
	MAGIC_MON_WHIRLPOOL,
	MAGIC_MON_FLAME,
	MAGIC_MON_POISONFLAME,
	MAGIC_MON_ROCKSLIDE,
	MAGIC_MON_HEAL,
	MAGIC_MON_POISON,
	MAGIC_MON_ICE,
	MAGIC_MON_METEOR,
	MAGIC_MON_DRAIN,
	MAGIC_MON_DROPMONEY,
	MAGIC_MON_BLACKINK,
	MAGIC_MON_DROPSNOWMAN,
	MAGIC_MON_DROPFORK,
	MAGIC_MON_CURSEATTACK,
	MAGIC_MON_CURSECRITICAL,

	MAGIC_MON_CURSEDEFENSE,
	MAGIC_MON_CURSEMOVESPEED,
	MAGIC_MON_THROWKNIFE,
	MAGIC_MON_LASERBEAM,
	MAGIC_MON_TRIMETEOR,
	MAGIC_MON_FIREGROUND,
	MAGIC_MON_SWORDBOMB,
	MAGIC_MON_FREEZEEXPLOSION,

	MAGIC_MON_UPPERDRAGON,
	MAGIC_MON_PALMWIND,
	MAGIC_MON_SUMMONDRAGON,

	MAGIC_MON_FIREBURN,

	MAGIC_MON_SKELLGHOST,
	MAGIC_MON_SWORDMETEOR,

	MAGIC_MON_FROZEN,

	MAGIC_MON_TWINSPEAR,
	MAGIC_MON_FIREWHIRLWIND,

	MAGIC_MON_NORTHWIND,
	MAGIC_MON_FIRESTORM,
	MAGIC_MON_EARTHQUAKE,
	MAGIC_MON_SPEAR_DRAGON,

	MAGIC_MON_GARGOYLE,
	MAGIC_MON_CERBERUS,

	MAGIC_MON_FROZENSORCERER,
	MAGIC_MON_MARIONETTE,
	MAGIC_MON_FROKEN,
	MAGIC_MON_ICEHOUND,
	MAGIC_MON_FROZENGOLEM,
	MAGIC_MON_FROZENKNIGHT,
	MAGIC_MON_FROZENQUEEN,
	MAGIC_MON_GOTPA,
	MAGIC_MON_NINJAPANDA,
	MAGIC_MON_RACCOONDOG,
	MAGIC_MON_BOSSMONKEY,
	MAGIC_MON_TOADPOISON,

	MAGIC_MON_BLOODBAT,
	MAGIC_MON_WEREWOLF,
	MAGIC_MON_PRIEST,
	MAGIC_MON_DEVILOUS,
	MAGIC_MON_GANGSI,
	MAGIC_MON_DRACULA,
	
	MAGIC_MON_HARPY,
	MAGIC_MON_MANDRAGORA,
	MAGIC_MON_GIRAFFE,
	MAGIC_MON_PAPAGOBLIN,
	MAGIC_MON_PIRATESOLDIER,
	MAGIC_MON_PIRATEARCHER,
	MAGIC_MON_BLACKBOSS,
	MAGIC_MON_EMPERORCAVALRY,
	MAGIC_MON_EMPERORMASTER,
	MAGIC_MON_GREATWALLSOLDIER,
	MAGIC_MON_GREATWALLMAGICIAN,
	MAGIC_MON_EMPERORWARRIOR,
	MAGIC_MON_GREATWALLSPIDER,
	MAGIC_MON_GREATWALLTWINHEAD,
	MAGIC_MON_BEARTANG,
	MAGIC_MON_SUPERBEARTANG,
	MAGIC_MON_SFROZENGOLEM,
	MAGIC_MON_SFROZENQUEEN,
	MAGIC_MON_CATPET,
	MAGIC_MON_COSMOSRABBIT,
	MAGIC_MON_RUDOLPHPET,
	MAGIC_MON_DROPGREATWALLASURA,
	MAGIC_MON_AMBERSAURUS,
	MAGIC_MON_ROLLINGSTEGO,
	MAGIC_MON_SUMMONDRAGON_STUN,
	MAGIC_MON_DRAGONFIRE,
	MAGIC_MON_CLOWNBALL,
	MAGIC_MON_BOUQUET,


	MAGIC_MON_LAST		= MAGIC_MON_BOUQUET,
//	MAGIC_INDEX_LAST	= MAGIC_MON_LAST,
//	MAGIC_TOTAL	
};

enum MAGIC_TYPE_SOULGUARD
{
	MAGIC_SOULGUARD_START = MAGIC_MON_LAST + 1,
	MAGIC_SOULGUARD_FIRE  = MAGIC_SOULGUARD_START,
	MAGIC_SOULGUARD_PUMKIN,							// 소울 가드 펌킨이 사용하는 마법

	MAGIC_SOULGUARD_LAST  = MAGIC_SOULGUARD_PUMKIN,

	//MAGIC_INDEX_LAST	  = MAGIC_SOULGUARD_LAST,
	//MAGIC_TOTAL	
};

enum MAGIC_TYPE_SKILL_BOOK
{
	MAGIC_SKILL_BOOK_START			= MAGIC_SOULGUARD_LAST + 1,
	
	MAGIC_SKILL_BOOK_FLAMEBUSTER	= MAGIC_SKILL_BOOK_START,
	MAGIC_SKILL_BOOK_FLAMEWHIRL,									// 소울 가드 펌킨이 사용하는 마법
	MAGIC_SKILL_BOOK_ICERAIN,
	MAGIC_SKILL_BOOK_WETFOG,
	MAGIC_SKILL_BOOK_CRASH,
	MAGIC_SKILL_BOOK_SLING,
	MAGIC_SKILL_BOOK_TORNADO,
	MAGIC_SKILL_BOOK_WINDCUTER,

	MAGIC_SKILL_BOOK_BUFTIMEINCREASE1,		
	MAGIC_SKILL_BOOK_BUFTIMEINCREASE2,		
	MAGIC_SKILL_BOOK_HITCOUNTINCREASE,		
	MAGIC_SKILL_BOOK_FREEZINGTIMEINCREASE,	
	MAGIC_SKILL_BOOK_ADDMAGIC,				
	MAGIC_SKILL_BOOK_ATTINCREASE,			
	MAGIC_SKILL_BOOK_ATTINCREASE_CANE,		
	MAGIC_SKILL_BOOK_RANGEINCREASE,			
	MAGIC_SKILL_BOOK_COOLTIMEDECREASE,		
	MAGIC_SKILL_BOOK_MPDECREASE,			
	MAGIC_SKILL_BOOK_ORGDECREASE,			


	MAGIC_SKILL_BOOK_LAST			= MAGIC_SKILL_BOOK_ORGDECREASE,
	
	MAGIC_INDEX_LAST				= MAGIC_SKILL_BOOK_LAST,

	MAGIC_TOTAL	
};

enum	{ MAGIC_REST_STATUS, MAGIC_PLAY_STATUS, MAGIC_STOP_STATUS, MAGIC_COMPLETE_STATUS };		// 현재 마법이 진행된 상태

//-----------마법 종류의 개수-------------------------------
#define	MAX_MAGIC_KIND_NUM					MAGIC_TOTAL				// 마법의 종류 최대 개수
//----------------------------------------------------------

//---------- 각 마법들 마다 생성할 최대 개수----------------
//==============================================================
// GAMEMODE_CLIENT, GAMEMODE_MAPEDIT 
//==============================================================
#define MAX_MAGIC_CLIENT_NUM			100						// 클라이언트에서 각 마법을 쓸 수 있는 총 개수

//---------- 마법 MSG 관련 ---------------------------------
#define	MAX_MAGIC_MSG_NUM				500						// 메시지 최대수
//----------------------------------------------------------

#define MAX_MAGIC_MSG_LENGTH			256						// 메시지는 최대 256 byte

//==============================================================
// GAMEMODE_SERVER
//==============================================================
#define MAX_MAGIC_SERVER_NUM			1500					// 서버에서 각 마법을 쓸 수 있는 총 개수

//---------- 마법 MSG 관련 ---------------------------------
#define	MAX_MAGIC_MSG_SERVER_NUM		3000					// 메시지 최대수
//==============================================================




#define MAX_MAGIC_NAME_LENGTH				64

//==============================================================
// 마법 시전 타입
//==============================================================
enum MAGIC_CASTTYPE
{
	MAGIC_CASTTYPE_PASSIVE = 0,	// 패시브형
	MAGIC_CASTTYPE_ACTIVE,		// 엑티브형
	MAGIC_CASTTYPE_BUF,			// 버프형
};

// 마법 속성.
enum MAGIC_ATTRIBUTE
{
	MAGIC_ATTRIBUTE_NONE,	// 없음.
	MAGIC_ATTRIBUTE_FIRE,	// 불.
	MAGIC_ATTRIBUTE_WATER,	// 물.
	MAGIC_ATTRIBUTE_EARTH,	// 땅.
	MAGIC_ATTRIBUTE_WIND,	// 바람.

	MAGIC_ATTRIBUTE_NUM
};

// 추가 시연 마법. 
enum MAGIC_ADDMAGIC
{
	ADDMAGIC_NONE,				// 없음.
	ADDMAGIC_CURSE_POISON,		// 독.
	ADDMAGIC_CURSE_ATTACK,		
	ADDMAGIC_CURSE_DEFENSE,	
	ADDMAGIC_CURSE_DODGE,		
	ADDMAGIC_CURSE_HIT,	
	ADDMAGIC_CURSE_ICE,

	ADDMAGIC_NUM
};

//---------- 마법의 기본 속성들 ----------------------------
struct	stMagicAttributeData
{
	TCHAR	strMagicKindName[ MAX_MAGIC_NAME_LENGTH ];
	TCHAR	strMagicName[ MAX_MAGIC_NAME_LENGTH ];	
	SI16	si16MagicUserUsing;							// 유저도 사용 가능한 마법인지 구별함
	SI08	si08MagicCastType;							// 마법 시전 타입(0:Passive, 1:Active, 2:Buf)
	SI32	si32MagicDelay;								// 마법이 실행되고 다음 마법이 실행되기 까지의 간격( 태곤이형 쪽에서 처리하고 있음 )
	SI32	si32MagicSpendMana;
	SI32	si32MagicYIndent;
	SI32	si32MagicMaxDrawFrame;
	SI32	si32MagicDirection;
	SI32	si32MagicMinDamage;
	SI32	si32MagicMaxDamage;
	SI32	si32MagicRadius;
	SI32	si32MagicDistance;
	SI32	si32MagicHitIDExist;
	SI32	si32MagicMaxActionFrame;					// 마법이 얼마의 시간동안 지속되어야 하는게 있을 수 있으므로...
	SI32	si32MagicNextFrameDelay;					// 한 동작 후에 다음 동작이 있기 위한 delay time
	SI16	si16MagicSkillSpendMana;					// 스킬 레벨에 따른 마나 소모량(%)
	SI16	si16MagicSkillIncreaseDamage;				// 스킬 레벨에 따른 타격 증가량(%)
	SI16	si16MagicSkillLevel;						// si32MagicSkillUnique 스킬 몇 부터 적용가능한지...
	TCHAR	strMagicOrganName[MAX_MAGIC_NAME_LENGTH];	// 마법에 따른 내상의 종류 문자열 
	SI16	si16MagicOrganKind;							// 마법에 따른 내상의 종류
	SI16	si16MagicOrganDamage;						// 내상의 정도 
	SI32	si32MagicExplainNum;						// 마법 설명 텍스트 메니져 번호
	SI08	siAttribute;								// 마법 속성.
	SI16	siAddMagic;									// 추가 시연 마법. 
	DWORD	dwAddMagicDuration;							// 추가 시연 마법 정보. 
	SI16	siAddMagicValue;							// 추가 시연 마법 지속 시간. 

};
//----------------------------------------------------------


//---------- Push 및 Pop 관련 메시지 정의 및 구조체 --------
#define	MAGIC_INCREASE_LIFE					35000					// 에너지 채움 ( 힐링 마법 )
#define	MAGIC_DAMAGE						35001					// 에너지 감소 ( 타격을 받았을 시에 )
#define MAGIC_CIRCLE_DAMAGE					35002					// 에너지 감소 ( 타격을 받았을 시에 반경을 중심으로... ) - 반경이 계속 틀림 ( 중심 좌표 일정 )
#define MAGIC_WHIRLPOOL_DAMAGE				35003					// 에너지 감소 ( 타격을 받았을 시에 반경을 중심으로... ) - 반경이 일정 ( 중심 좌표 이동 )

struct	stMagicMsgHeader
{
	SI16				si16MsgSize;								// 메시지 크기
	SI32				si32MagicKind;								// 마법의 종류
	SI32				si32MsgKind;								// 메시지 정의
};

struct	stMagicMsgDamage
{
	stMagicMsgHeader	sMagicMsgHeader;
	
	
	SI32				si32FromID;
	SI32				si32ToID;

	SI32				si32Damage;									// 타격치

};

struct	stMagicMsgCircleDamage
{
	stMagicMsgHeader	sMagicMsgHeader;
	SI32				si32FromID;
	SI32				si32ToID;
	SI32				si32Damage;									// 타격치
	SI32				si32CurrentRadius;							// 현재 반경( 현재 반경내에만 영향을 주기 위해서... )
	SI32				si32Radius;									// 반경
};

struct	stMagicMsgWhirlPoolDamage
{
	stMagicMsgHeader	sMagicMsgHeader;
	SI32				si32FromID;
	SI32				si32ToID;
	SI32				si32Damage;									// 타격치	
	SI32				si32Radius;									// 반경
	SI32				si32XCenterPos;
	SI32				si32YCenterPos;
};


//=========================================================

struct stMagicHeader
{
	SI32				si32MagicKind;
	SI32				si32Size;
	SI32				si32FromID;
	SI32				si32ToID;
	SI32				si32Mana;
};

// 
struct stMagicInfo
{
	stMagicHeader sHeader;

	stMagicInfo()
	{
		sHeader.si32MagicKind	= 0;
		sHeader.si32Size		= sizeof(stMagicInfo);
		sHeader.si32FromID		= 0;
		sHeader.si32ToID		= 0;
		sHeader.si32Mana		= 0;
	}
	stMagicInfo(SI32 kind, SI32 fromid, SI32 toid, SI32 mana)
	{
		sHeader.si32MagicKind	= kind;
		sHeader.si32Size		= sizeof(stMagicInfo);
		sHeader.si32FromID		= fromid;
		sHeader.si32ToID		= toid;
		sHeader.si32Mana		= mana;
	}
};

//--------------------------------------------------------------------------------
// 버프 마법 관련 정보
//--------------------------------------------------------------------------------
#define MAX_MAGICBUF_NUM	2	// 한번에 효과가 적용되는 버프 마법 갯수(이 갯수만 늘리면 다른 작업 필요없이 증가된다)

//--------------------------------------------------------------------------------
// 버프 마법 정보 저장
//--------------------------------------------------------------------------------
class CNMagicBuf
{
private:
	SI32	m_siMagicKind;		// 마법 종류(버프 마법만 들어온다)
	SI32	m_siActionFrame;	// 버프 효과가 사용되는 프레임수(20을 1초로 계산한다)
	SI32	m_siDelayFrame;		// 다음 마법을 사용하기까지의 대기 시간(20을 1초로 계산한다)

	SI32	m_siStartTime;		// 클라이언트는 프레임이 아니라 시작 시간으로 시간을 정한다(밀리세컨드)
	SI32	m_siActionTime;		// 해당 마법의 총 유지시간(밀리세컨드)

	SI16	m_siDrawFrame;		// 현재까지 마법 이펙트 그린 프레임
	SI16	m_siMaxFrame;		// 이 마법이 그려야할 최대 프레임

public:
	CNMagicBuf()	{	Init();	};
	~CNMagicBuf()	{			};

	void	Init( void )
	{
		ZeroMemory( this, sizeof(CNMagicBuf) );
	}

	void	Set( CNMagicBuf* pclMagicBuf )
	{
		if ( pclMagicBuf )
		{	
			memcpy( this, pclMagicBuf, sizeof(CNMagicBuf) );
		}
	}

	void	Set( SI32 siMagicKind, SI32 siLemainFrame, SI32 siDelayFrame, SI32 siStartTime, SI32 siActionTime, SI16 siDrawFrame=0, SI16 siMaxFrame=0 )
	{
		m_siMagicKind	= siMagicKind;

		m_siActionFrame	= siLemainFrame;
		m_siDelayFrame	= siDelayFrame;

		m_siStartTime	= siStartTime;
		m_siActionTime	= siActionTime;

		m_siDrawFrame	= siDrawFrame;
		m_siMaxFrame	= siMaxFrame;
	}

	SI32	GetMagicKind( void )					{	return m_siMagicKind;					}
	void	SetMagicKind( SI32 siMagicKind )		{	m_siMagicKind = siMagicKind;			}

	SI32	GetActionFrame( void )					{	return m_siActionFrame;					}
	void	SetActionFrame( SI32 siActionFrame )	{	m_siActionFrame = siActionFrame;		}
	SI32	AddActionFrame( SI32 siAddFrame )		{	return (m_siActionFrame += siAddFrame);	}
	SI32	SubActionFrame( SI32 siSubFrame )		{	return (m_siActionFrame -= siSubFrame);	}

	SI32	GetDelayFrame( void )					{	return m_siDelayFrame;					}
	void	SetDelayFrame( SI32 siDelayFrame )		{	m_siDelayFrame = siDelayFrame;			}
	SI32	AddDelayFrame( SI32 siAddFrame )		{	return (m_siDelayFrame += siAddFrame);	}
	SI32	SubDelayFrame( SI32	siSubFrame )		{	return (m_siDelayFrame -= siSubFrame);	}

	SI32	GetStartTime( void )					{	return m_siStartTime;					}
	void	SetStartTime( SI32 siStartTime )		{	m_siStartTime = siStartTime;			}

	SI32	GetActionTime( void )					{	return m_siActionTime;					}
	void	SetActionTime( SI32 siActionTime )		{	m_siActionTime = siActionTime;			}

	SI16	GetDrawFrame( void )					{	return m_siDrawFrame;					}
	void	SetDrawFrame( SI16 siDrawFrame )		{	m_siDrawFrame = siDrawFrame;			}
	SI16	AddDrawFrame( SI16 siAddFrame )			{	return (m_siDrawFrame += siAddFrame);	}
	SI16	SubDrawFrame( SI16 siSubFrame )			{	return (m_siDrawFrame -= siSubFrame);	}

	SI16	GetMaxFrame( void )						{	return m_siMaxFrame;					}
	void	SetMaxFrame( SI16 siMaxFrame )			{	m_siMaxFrame = siMaxFrame;				}

	bool	IsEmpty( void )							{	return (m_siMagicKind<=0?true:false);	}

};

//--------------------------------------------------------------------------------
// 버프 마법 정보 메니저
//--------------------------------------------------------------------------------
class CNMagicBufMgr
{
public:
	enum MBM_VARIABLE
	{
		MBM_VARIABLE_RET_DONOTHAVE = -1,
	};
private:
	CNMagicBuf	m_clMagicBuf[MAX_MAGICBUF_NUM];

public:
	CNMagicBufMgr()
	{
		Init();
	}
	~CNMagicBufMgr()
	{
	}

	void	Init( void )
	{
		ZeroMemory( this, sizeof(CNMagicBufMgr) );
	}

	void	Set( CNMagicBufMgr* pclMagicBufMgr );

	void	PushMagicBuf( SI32 siMagicKind, SI32 siDecMagicDelayRate, SI32 siID );


	// 해당 마법을 쓸 수 있는 딜레이 타임을 검사한다
	bool	CheckDelay( SI32 siMagicKind );
	// 마법을 쓸 수 있는 슬롯이 있는지를 검사
	bool	CheckMagicBufSlot( SI32 siMagicKind );
	// 쿨타임/유효시간 프레임을 감소한다(siDecFram은 양수이다)
	void	DecFrame( SI32 siDecFrame );

	void	Draw( SI32 si32CenterXPos, SI32	si32CenterYPos );
	void	MagicBufDraw( SI32 siMagicBufSlotNum, SI32 si32CenterXPos, SI32	si32CenterYPos );
	SI32	GetGIMGIndex( SI32 siMagicGroup );


	SI32	GetIncDamage( cltCharCommon* pclCharCommon );			// 버프로 증가되는 공격력
	SI32	GetIncCriticalRate( cltCharCommon* pclCharCommon );		// 버프로 증가되는 필살률
	SI32	GetIncHitRate( cltCharCommon* pclCharCommon );			// 버프로 증가되는 명중률
	SI32	GetIncRange( cltCharCommon* pclCharCommon );			// 버프로 증가되는 사정거리
	SI32	GetIncAttackInterval( cltCharCommon* pclCharCommon );	// 버프로 증가되는 공격속도
	SI32	GetIncMPRecoveryTime( cltCharCommon* pclCharCommon );	// 버프로 증가되는 MP 회복시간
	SI32	GetIncHP( cltCharCommon* pclCharCommon );				// 버프로 증가되는 HP

	void	MoveToFront( void );						// 모든 버프를 0번슬롯쪽으로 이동시킨다

	CNMagicBuf*	GetMagicBuf( SI32 siIndex );
private:
	// 내가 갖고 있는 버프중에 siMagicGroup을 갖고 있는가?
	SI32	IsHaveMagicGroup( SI32 siMagicGroup );
	SI32	GetTotalSkillLevel( cltCharCommon* pclCharCommon, SI32 siMagicKind );
	void	PlayTheEffect( SI32 siMagicGroup );

};

#endif