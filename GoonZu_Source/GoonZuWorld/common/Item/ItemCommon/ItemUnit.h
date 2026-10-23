//---------------------------------
// 2003/5/20 김태곤
//---------------------------------

#ifndef _ITEMUNIT_H
#define _ITEMUNIT_H

#include "../../../Lib/Directives.h"
#include "../../../../NLib/NArray.h"
#include "../../GoonZuWorld/common/Skill/Skill.h"

#include "../../../GMSystem/GMSectionData.h"

#define  MAX_DURABILITY_PERCENT_IN_CHINA	100

//-------------------------------
// 아이템 타입 유니크 .
//-------------------------------
#define ITEMTYPE_SWORD			1
#define ITEMTYPE_SPEAR			2
#define ITEMTYPE_AXE			3
#define ITEMTYPE_BOW			4
#define ITEMTYPE_GUN			5

#define ITEMTYPE_STAFF			6
#define ITEMTYPE_HELMET			7
#define ITEMTYPE_ARMOUR			8
#define ITEMTYPE_BELT			9
#define ITEMTYPE_SHOES			10

#define ITEMTYPE_NECK			11
#define ITEMTYPE_RING			12
#define ITEMTYPE_BAG			13
#define ITEMTYPE_HAT			14
#define ITEMTYPE_DRESS			15

#define ITEMTYPE_MANTLE			16
#define ITEMTYPE_FOOD			17
#define ITEMTYPE_HEAL			18
#define ITEMTYPE_BOOK			19
#define ITEMTYPE_BOX			20	// 아이템타입 추가 박스아이템

#define ITEMTYPE_MINERAL		21	
#define ITEMTYPE_TREE			22

#define ITEMTYPE_CLOTH			23
#define ITEMTYPE_SEAFOOD		24
#define ITEMTYPE_CRYSTAL		25	// 아이템타입 추가 아티팩트 결정체.

#define ITEMTYPE_COMPOSITIONMATERIAL		26	// 아이템타입 추가 아티팩트 합성제료
#define ITEMTYPE_PAPER			27
#define ITEMTYPE_IRON			28
#define ITEMTYPE_DISH			29
#define ITEMTYPE_FISHINGROD		30

#define ITEMTYPE_DOLL			31
#define ITEMTYPE_ANIMAL			32
#define ITEMTYPE_TICKET			33
#define ITEMTYPE_PREMIUM		34	// 아이템타입 추가 프리미엄아이템.
#define ITEMTYPE_EVENTITEM		35	// 아이템타입 추가 이벤트 아이템

#define ITEMTYPE_ETC			36
#define ITEMTYPE_SCROLL			37
#define ITEMTYPE_QUEST			38
#define ITEMTYPE_CANNON			39	// 대포이젠 사용 안함
#define ITEMTYPE_TORPEDO		40	// 어뢰 이젠 사용 안함

#define ITEMTYPE_AVATAR_MANTLE	41	// [지연] 아바타 망토
#define ITEMTYPE_AVATAR_DRESS	42	// [지연] 아바타 의복
#define ITEMTYPE_AVATAR_HAT		43	// [지연] 아바타 모자
#define ITEMTYPE_AVATAR_NECK	44	// [지연] 아바타 목걸이
#define ITEMTYPE_AVATAR_RING	45	// [지연] 아바타 반지

//기존 아이템타입
//#define ITEMTYPE_SWORD			1
//#define ITEMTYPE_SPEAR			2
//#define ITEMTYPE_STAFF			3
//#define ITEMTYPE_AXE			4
//#define ITEMTYPE_BOW			5
//#define ITEMTYPE_GUN			6
//#define ITEMTYPE_CANNON			7
//#define ITEMTYPE_ARMOUR			8
//#define ITEMTYPE_DRESS			9
//#define ITEMTYPE_NECK			10
//#define ITEMTYPE_RING			11
//#define ITEMTYPE_BELT			12
//#define ITEMTYPE_SHOES			13
//#define ITEMTYPE_HELMET			14
//#define ITEMTYPE_HAT			15
//#define ITEMTYPE_FOOD			16
//#define ITEMTYPE_SEAFOOD		17
//#define ITEMTYPE_HEAL			18
//#define ITEMTYPE_BOOK			19
//#define ITEMTYPE_MINERAL		20
//#define ITEMTYPE_ANIMAL			21
//#define ITEMTYPE_TREE			22
//#define ITEMTYPE_PAPER			23
//#define ITEMTYPE_CLOTH			24
//#define ITEMTYPE_SCROLL			25
//#define ITEMTYPE_ETC			26
//#define ITEMTYPE_IRON			27
//#define ITEMTYPE_DISH			28
//#define ITEMTYPE_FISHINGROD		29
//#define ITEMTYPE_DOLL			30
//#define ITEMTYPE_TICKET			31
//#define ITEMTYPE_BAG			32
//#define ITEMTYPE_MANTLE			33
//#define ITEMTYPE_TORPEDO		34
//#define ITEMTYPE_QUEST			35
//#define ITEMTYPE_CRYSTAL		36	// 아이템타입 추가 아티팩트 결정체.
//#define ITEMTYPE_COMPOSITIONMATERIAL		37	// 아이템타입 추가 아티팩트 합성제료
//#define ITEMTYPE_EVENTITEM		38	// 아이템타입 추가 이벤트 아이템
//#define ITEMTYPE_BOX		39	// 아이템타입 추가 박스아이템
//#define ITEMTYPE_PREMIUM		40	// 아이템타입 추가 프리미엄아이템.
//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
#define MAX_CRYSTAL_NUM		5	// 아이템당 최대 슬롯수.
#define MAX_CAN_CRYSTALABILITY_NUM		6	// 한 아이템당 가질수 있는 능력의 종류수.

#define CRYSTAL_DECREASE_DURABILITY_PRO		15	// 내구도가 깍일 확률. 
#define CRYSTAL_DECREASE_DURABILITY_ACNUM		1	// 합번에 깍이는 내구도 양 - 전투.
#define CRYSTAL_DECREASE_DURABILITY_MKNUM		10	// 합번에 깍이는 내구도 양 - 제조.


//합성재료
#define	MAX_COMPOSISTINMANTERIAL_LEVEL	6	//최대 합성제료 등급

//[추가 : 황진성 2008. 3. 3 => 아이템이 환원되는 아이템의 최대 수량.]
#define MAX_RESTORE_ITEM_NUM		5	

#define CRYSTALTYPE_RUBY				1	//루비
#define CRYSTALTYPE_TOPAZ				2	//토파즈 
#define CRYSTALTYPE_VIOLETQUARTZ		3	//자수정
#define CRYSTALTYPE_DIAMOND			4	//다이아몬드
#define CRYSTALTYPE_PEARL				5	//진주
#define CRYSTALTYPE_SAPPHIRE			6	//사파이어
//-----------------------------------------------
#define MAX_CRYSTALABILITY_NUM							68	// 20080228 - 56가지 능력 사용중.

#define CRYSTALABILITY_STATE_STR						1	//근력.
#define CRYSTALABILITY_STATE_DEX						2	//순발력.
#define CRYSTALABILITY_STATE_MAG						3	//지력
#define CRYSTALABILITY_STATE_VIT						4	//생명력.
#define CRYSTALABILITY_STATE_LUK						5	//행운.
#define CRYSTALABILITY_STATE_HND						6	//손재주.

//#define CRYSTALABILITY_EFFECT_DMG						7	// 공격력  - 최대, 최소 데미지.
//#define CRYSTALABILITY_EFFECT_AC						8	// 방어력  
//#define CRYSTALABILITY_EFFECT_AC						9	// 체력  
//#define CRYSTALABILITY_EFFECT_AC						10  // 마법력  
//#define CRYSTALABILITY_EFFECT_DR						11  // 회피율  
//#define CRYSTALABILITY_EFFECT_AR						12  // 명룽률  

#define CRYSTALABILITY_SKILL_SUMMON						13  // 소환술.
#define CRYSTALABILITY_SKILL_FARM						14  // 농경술.
#define CRYSTALABILITY_SKILL_MINE						15  // 채광술
#define CRYSTALABILITY_SKILL_FISH						16  // 어획술.
#define CRYSTALABILITY_SKILL_MAKEFOOD					17  // 요리술.
#define CRYSTALABILITY_SKILL_CAPTURE					18  // 포획술.
#define CRYSTALABILITY_SKILL_HORSETRAINING				19  // 탈것 조련술.

#define CRYSTALABILITY_SKILL_MAKEIRON1					20  // 철물제작술
#define CRYSTALABILITY_SKILL_MAKETOOL1					21  // 생산도구제작술
#define CRYSTALABILITY_SKILL_MAKEMED					22  // 약제조술
#define CRYSTALABILITY_SKILL_MAKESWORD1					23  // 검제조술
#define CRYSTALABILITY_SKILL_MAKESPEAR1					24  // 창제조술
#define CRYSTALABILITY_SKILL_MAKEAXE1					25  // 도끼제조술
#define CRYSTALABILITY_SKILL_MAKESTAFF1					26  // 지팡이제조술
#define CRYSTALABILITY_SKILL_MAKEBOW1					27  // 활제조술
#define CRYSTALABILITY_SKILL_MAKEGUN1					28  // 총제조술
#define CRYSTALABILITY_SKILL_MAKEARMOUR1				29  // 갑옷제조술
#define CRYSTALABILITY_SKILL_MAKEHELMET1				30  // 투구제조술
#define CRYSTALABILITY_SKILL_MAKESHOES1					31  // 신발제조술
#define CRYSTALABILITY_SKILL_MAKEBELT1					32  // 허리띠제조술

#define CRYSTALABILITY_SKILL_USESWORD1					33  // 검술
#define CRYSTALABILITY_SKILL_USESPEAR1					34  // 창술
#define CRYSTALABILITY_SKILL_USEAXE1					35  // 도끼술
#define CRYSTALABILITY_SKILL_USESTAFF1					36  // 지팡이술
#define CRYSTALABILITY_SKILL_USEBOW1					37  // 궁술
#define CRYSTALABILITY_SKILL_USEGUN1					38  // 총술

// 광열 추가
#define CRYSTALABILITY_STATE_WIS						39  // 지혜

#define CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK			40  // 물리 공격력
#define CRYSTALABILITY_EFFECT_MAGICAL_ATTACK			41  // 마법 공격력
#define CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE			42  // 물리 명중률
#define CRYSTALABILITY_EFFECT_MAGICAL_HITRATE			43  // 마법 명중률
#define CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE		44  // 물리 회피율
#define CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE			45  // 마법 회피율
#define CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE	46  // 물리 방어율
#define CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE		47  // 마법 방어율
#define CRYSTALABILITY_EFFECT_HITPOINT					48  // 체력
#define CRYSTALABILITY_EFFECT_MANAPOINT					49  // 마법력
#define CRYSTALABILITY_EFFECT_WEIGHT					50  // 무게량

#define CRYSTALABILITY_ATTRIBUTE_FIREATTACK				51  // 불 속성 공격력
#define CRYSTALABILITY_ATTRIBUTE_WATERATTACK			52  // 물 속성 공격력
#define CRYSTALABILITY_ATTRIBUTE_WINDATTACK				53  // 바람 속성 공격력
#define CRYSTALABILITY_ATTRIBUTE_EARTHATTACK			54  // 땅 속성 공격력
#define CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE			55  // 불 속성 방어력
#define CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE			56  // 물 속성 방어력
#define CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE			57  // 바람 속성 방어력
#define CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE			58  // 땅 속성 방어력

//<---마법 추가 필요함 -->


#define CRYSTALABILITY_SKILL_ALLMAKESKILL					59	//모든 재조 스킬.
#define CRYSTALABILITY_SKILL_ALLWEAPONSKILL					60	//모든 무기 스킬.
#define CRYSTALABILITY_SKILL_ALLPRODUCTSKILL				61	//모든 생산 스킬.
#define CRYSTALABILITY_SKILL_ALLSKILL						62	//모든 스킬.

#define CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK			63  // 최소 물리 공격력
#define CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK			64  // 최대 물리 공격력
#define CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK			65  // 최소 마법 공격력
#define CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK			66  // 최대 마법 공격력

#define CRYSTALABILITY_SKILL_ARTIFACT						67  // 아트펙트 제조술.
#define CRYSTALABILITY_SKILL_ALLAVATARSKILL				68	//모든 아바타 스킬.

//------이하 미구현-----------------------------------------
#define CRYSTALABILITY_SPECIAL_FISHING						70 // 낚시		
#define CRYSTALABILITY_SPECIAL_MINING						71 // 채광		
#define CRYSTALABILITY_SPECIAL_FARMING						72 // 농경		
#define CRYSTALABILITY_SPECIAL_FREEZE						74 // 냉동		
#define CRYSTALABILITY_SPECIAL_HITRATE						75 // 명중		
#define CRYSTALABILITY_SPECIAL_VAMPIRE						76 // 흡혈		
#define CRYSTALABILITY_SPECIAL_IRONARMOR					77 // 철갑		
#define CRYSTALABILITY_SPECIAL_DEATHBLOW					78 // 필살	 - 구현	
#define CRYSTALABILITY_SPECIAL_POISONING					79 // 중독	
//-----------------------------------------------

// [진성] 아바타 결정체. - 번호의 순서를 바꿔서는 안된다. - 혹시나 다음 아바타 타임을 추가 하게 된다면, GetCountEquipPersonitemAbility 를 살필것.
#define CRYSTALABILITY_AVATAR_STATE_STR						80	// [진성] 아바타 결정체. 근력.
#define CRYSTALABILITY_AVATAR_STATE_DEX						81	// [진성] 아바타 결정체. 순발력.
#define CRYSTALABILITY_AVATAR_STATE_MAG						82	// [진성] 아바타 결정체. 지력
#define CRYSTALABILITY_AVATAR_STATE_VIT						83	// [진성] 아바타 결정체. 생명력.
#define CRYSTALABILITY_AVATAR_STATE_LUK						84	// [진성] 아바타 결정체. 행운.
#define CRYSTALABILITY_AVATAR_STATE_HND						85	// [진성] 아바타 결정체. 손재주.
#define CRYSTALABILITY_AVATAR_STATE_WIS						86	// [진성] 아바타 결정체. 지혜.
#define CRYSTALABILITY_AVATAR_ALLMAKESKILL					87	// [진성] 아바타 결정체. 제조술.
#define CRYSTALABILITY_AVATAR_ALLWEAPONSKILL				88	// [진성] 아바타 결정체. 무기술.
#define CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL				89	// [진성] 아바타 결정체. 생산술.
#define CRYSTALABILITY_AVATAR_SKILL_ALLAVATARSKILL					90	//모든 아바타 스킬.

//-----------------------------------------------

//---------------------------------
//아이템 타입의 속성.
// bit 단위로 증가. 
//---------------------------------
#define ITEMATB_OBJECT			0x1	// 아이템 객체가 별도의 정보를 저장하는 형태다.
#define ITEMATB_NOTRADE			0x2	// 거래 불가 품종. 
#define ITEMATB_FIRSTACTION		0x4	// 선조치 아이템. 
#define ITEMATB_FIRSTWARITEM	0x8	// 전리품으로 선조치되는 아이템 

#define ITEMATB_WEAPON			0x10	// 무기류의 속성을 갖는다.
#define ITEMATB_ARMOUR			0x20
#define ITEMATB_UPARMOUR		0x40
#define ITEMATB_NEARWEAPON		0x80
#define ITEMATB_FARWEAPON		0x100
#define ITEMATB_HELMET			0x200
#define ITEMATB_BELT			0x400
#define ITEMATB_RING			0x800
#define ITEMATB_NECK			0x1000
#define ITEMATB_SHOES			0x2000
#define ITEMATB_HAT				0x4000

#define ITEMATB_BOW				0x8000
#define ITEMATB_SWORD			0x10000
#define ITEMATB_GUN				0x20000
#define ITEMATB_SPEAR			0x40000
#define ITEMATB_STAFF			0x80000
#define ITEMATB_AXE				0x100000

#define ITEMATB_HEAL			0x400000
#define ITEMATB_BOOK			0x800000
#define ITEMATB_MINERAL			0x1000000
#define ITEMATB_ANIMAL			0x2000000
#define ITEMATB_CLOTH			0x4000000
#define ITEMATB_SCROLL			0x8000000

#define ITEMATB_ETC				0x10000000
#define ITEMATB_REPAIR			0x20000000
#define ITEMATB_DOLL			0x40000000
#define ITEMATB_TICKET			0x80000000

#define ITEMATB_TRADESERVER		0x100000000
#define ITEMATB_HIDECOMBOLIST	0x200000000

//아이탭 탭 추가.
#define ITEMATB_CRYSTAL				0x10000000000
#define ITEMATB_COMPOSITIONMATERIAL	0x20000000000
#define ITEMATB_EVENTITEM			0x40000000000
#define ITEMATB_BOX					0x80000000000
#define ITEMATB_PREMIUM				0x100000000000

//---------------------------------
// 이 아이템에 부착되는 Scroll의 속성을 정의 ( 0 ~31)
//---------------------------------
#define ITEMATTACH_NULL			(0)			// 
#define ITEMATTACH_DURABLE		(1<<0)		// 
#define ITEMATTACH_WEAPON		(1<<1)		// 
#define ITEMATTACH_ARMOUR		(1<<2)		// 
#define ITEMATTACH_NECK			(1<<3)		// 
#define ITEMATTACH_RING			(1<<4)		// 
#define ITEMATTACH_BELT			(1<<5)		// 
#define ITEMATTACH_SHOES		(1<<6)		// 
#define ITEMATTACH_HELMET		(1<<7)		// 
#define ITEMATTACH_HAT			(1<<8)		// 
#define ITEMATTACH_BAG	 		(1<<9)		// 
#define ITEMATTACH_MANTLE 		(1<<10)		// 
#define ITEMATTACH_DRESS		(1<<11)		// 
#define ITEMATTACH_NEARWEAPON	(1<<12)		// 
#define ITEMATTACH_FARWEAPON	(1<<13)		// 	
#define ITEMATTACH_UPARMOUR		(1<<14)		// 
#define ITEMATTACH_SKILLUPDRESS	(1<<15)		// 제조스킬 향상 의복에 적용 
#define ITEMATTACH_USEWEAPONUPDRESS	(1<<16)	// 기사용술 향상 의복에 적용 
//#define ITEMATTACH_ATIFACT	(1<<17)	// 아티팩트 



#define MAX_PARTYHALL_APPLICATION_LIST	20		//[진성] 파티홀. 입장 신청자 최대수.
#define MAX_PARTYHALL_DECORATION_ITEM	20		//[진성] 파티홀. 장식용 아이템의 최대수.
#define MAX_ITEM_PER_PERSON				80		// by LEEKH - 캐릭터의 아이템 개수 축소 : 소환수X2 장착 공간 삭제
#define MAX_SHAPE_ITEM					15		// 소환수가 장비가능한 물품의 수. 	
#define MAX_INVITEM_PER_PERSON			64		// 사람과 말의 모든 인벤 합계. 
#define MAX_ITEM_PER_INVENTORY			20
#define MAX_ITEM_PER_BAG				12		// 가방
#define MAX_ITEM_PER_WENNYOLD			20
#define MAX_ITEM_SPACE_BOX				32		//[추가 : 황진성 2008. 1. 29 => 공간 상자	    최대 아이템 수.]
#define MAX_ITEM_TREASURE_BOX			32		//[추가 : 황진성 2008. 1. 29 => 공간 상자(보물) 최대 아이템 수.]
#define MAX_ITEM_ARTIFACT				5		//[추가 : 황진성 2008. 2. 28 => 아티펙트 환원 된 아이템의 최대 수.]
#define MAX_ITEM_ARTIFACT_MIXTURE		5		//[추가 : 황진성 2008. 2. 28 => 아티펙트 조합되는 아이템의 최대 수.]

#define WENNYMSG_SMALL			1		// 30~49%
#define WENNYMSG_MEDIUM			2		// 50~69%
#define WENNYMSG_LARGE			3		// 70~89%
#define WENNYMSG_DOUBLE			4		// 200%
#define	WENNYMSG_BOX			5		// 이호식 상자
#define WENNYMSG_NECKLACE		6		// 목걸이
#define WENNYMSG_NONE			7		// 아무것도 나오지 않음
#define WENNYMSG_FAILED			8		// 다른 이유에 의해 실패
#define WENNYMSG_WENNYMATERIAL	9		// 이호식 전용 아이템이 나옴
#define WENNYMSG_GACHARARE		10		// 가챠 아이템을 분해했을때만 나옴
#define WENNYMSG_VAlENTINEITEM	11		// 발렌타인 아이템.
#define WENNYMSG_WHITEITEM		12		// 화이트데이 아이템.
#define WENNYMSG_DAILYQUEST3	13		// 발렌타인 아이템.
#define WENNYMSG_REPAIR_DUR		14		// 내구도 수리석 나옴
#define WENNYMSG_FORTUNE_COOKIE	15		// 포춘쿠키 조각 나옴.

//cyj  중첩되는 아이템 갯수 1000으로 변경
//#define MAX_ITEM_PILE_NUMBER	10000				// 한개의 아이템안에 중첩될 수 있는 아이템의 개수. 
#define MAX_ITEM_PILE_NUMBER	10000				// 한개의 아이템안에 중첩될 수 있는 아이템의 개수. 
#define DEFAULT_EQUIP_BULLET_NUMBER	500				// 화살 및 탄환의 장착 가능가능 갯수
#define UPGRADE_EQUIP_BULLET_NUMBER 1000			// 탄환서비스 사용시 장착 가능 갯수

//#define MAX_DURABILITY			1000	// 아이템 내구도의 최대값. 
//#define FOREVER_DURABILITY		1001	// 내구도가 소모되지 않는 상태. 	
//#define FOREVER_DURABILITY2		1002	// 내구도가 소모되지 않는 상태(기간제 아이템)

#define SELECTED_HUMAN			-1	// 아이템 장착창에 사람이 선택되었다

#define EQUIPSET_PERSON				1
#define EQUIPSET_SUMMON1			2
#define EQUIPSET_SUMMON2			3
#define EQUIPSET_SUMMON3			4

//-----------------------------------
// Person내 아이템 위치. 
//-----------------------------------
#define PERSONITEM_HELMET		0
#define PERSONITEM_ARMOUR		1
#define PERSONITEM_WEAPON1		2
#define PERSONITEM_NECK			3	
#define PERSONITEM_RING1		4
#define PERSONITEM_RING2		5
#define PERSONITEM_BELT			6
#define PERSONITEM_SHOES		7
#define PERSONITEM_WEAPON2		8		
#define PERSONITEM_MANTLE		9
#define PERSONITEM_BAG			10
#define PERSONITEM_HAT			11
#define PERSONITEM_DRESS		12
#define PERSONITEM_BAG_SUB		13
#define PERSONITEM_RESERVED1	14


#define PERSONITEM_INV0			15
#define PERSONITEM_INV1			16
#define PERSONITEM_INV2			17
#define PERSONITEM_INV3			18
#define PERSONITEM_INV4			19
#define PERSONITEM_INV5			20

#define PERSONITEM_NONE         25

#define PERSONITEM_HORSE0		35

#define PERSONITEM_BAG0			55
#define PERSONITEM_BAG1			67

//#define PERSONITEM_SUMMON1EQUIP0	80

//#define PERSONITEM_SUMMON2EQUIP0	95

//#define PERSONITEM_SUMMON3EQUIP0	110



//---------------------------------
// 효과음 연주 
//---------------------------------
#define ITEMSOUND_SELECT		1		// 선택시 효과음. 
#define ITEMSOUND_USE			2		// 사용시 효과음. 
#define ITEMSOUND_REPAIR		3		// 수리시 효과음. 
#define ITEMSOUND_MAKE			4		// 제작시 효과음. 


//--------------------------------
// 부적의 기능. (순서가 바뀌면 안된다.)
//--------------------------------
#define SCROLL_UNIQUE_FUNCTION_UPHITRATE			1		// 무기의 명중률 향상. 
#define SCROLL_UNIQUE_FUNCTION_REDUCEDAMAGE			2		// 적 데미지 감소. 
#define SCROLL_UNIQUE_FUNCTION_UPCRITICAL			3		// 무기의 필살률 향상 
#define SCROLL_UNIQUE_FUNCTION_POISON				4		// 독 
#define SCROLL_UNIQUE_FUNCTION_ICE					5		// 얼음
#define SCROLL_UNIQUE_FUNCTION_DRAIN				6		// DRAIN
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPAXE		7		// 도끼제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSPEAR		8		// 창제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSTAFF		9		// 지팡이제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSWORD		10		// 검제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBOW		11		// 활제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPGUN		12		// 총제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPARMOUR	13		// 갑옷제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHELMET	14		// 투구제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPDRESS		15		// 의복제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHAT		16		// 모자제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSHOES		17		// 신발제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBELT		18		// 요대제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPNECK		19		// 목걸이제조술 향상 
//cyj 반지제작술 삭제 #define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING		20		// 반지제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_UPMAXDURABILITY		21		// 아이템의 최대 내구도 향상 

#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD		22		// 무기사용술 향상 - 칼 
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR		23		// 무기사용술 향상 - 창 
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE		24		// 무기사용술 향상 - 도끼 
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW		25		// 무기사용술 향상 - 활
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN		26		// 무기사용술 향상 - 총  
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF		27		// 무기사용술 향상 - 지팡이
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON	28		// 무기사용술 향상 - 모든 무기
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPATIFACT	29		// 아티팩트결정제조술 향상 

//-------------------------------
// 인벤모드 
//-------------------------------
#define INVMODE_PERSON					1		// 사람 인벤 
#define INVMODE_HORSE					2		// 말	인벤 
#define INVMODE_BAG						3		// 가방 인벤 
#define INVMODE_BAG_SUB					4		// 가방2
#define INVMODE_ALL						5		// 말과 사람 인벤을 모두 검색. 


#define CANADDINV_FAIL_FULL				1		// 인벤에 빈자리가 없어서 추가할 수  없다. 


//------------------------------
// 장비창에 장비 불가 사유. 
//------------------------------
#define ITEMEQUIP_FAIL_OUTOFSKILL		1
#define ITEMEQUIP_FAIL_OUTOFLEVEL		2
#define ITEMEQUIP_FAIL_OUTOFSTR			3	
#define ITEMEQUIP_FAIL_OUTOFDEX			4
#define ITEMEQUIP_FAIL_OUTOFMAG			5
#define ITEMEQUIP_FAIL_OUTOFVIT			6
#define ITEMEQUIP_FAIL_OUTOFHND			7	
#define ITEMEQUIP_FAIL_FORWOMAN			8
#define ITEMEQUIP_FAIL_FORMAN			9
#define ITEMEQUIP_FAIL_OUTOFFAME		10
#define ITEMEQUIP_FAIL_NOSUMMONHERO		11
#define ITEMEQUIP_FAIL_OUTOFSUMMONLEVEL	12
#define ITEMEQUIP_FAIL_NOEQUIPFORHERO	13	// 소환영웅이 사용하는 무기종류가 아니다. 
#define ITEMEQUIP_FAIL_NOTTAKEOUTSTATUS	14	// 소환된 상태가 아니다
#define ITEMEQUIP_FAIL_NOTENOUGHSTONE	15	// 소환수 장비 장착석이 모자르다
#define ITEMEQUIP_FAIL_WRONGEQUIPTYPE	16	// 소환수 장비 타입이 타르다


#define MAX_ENCHANT_NUMBER	5		// 인챈트 아이템 재료 종류 최대값
// [인첸트 UI 개선 - 2007.12.24 김영훈 추가]
#define NOW_ENCHANT_NUMBER	4		// 현재 인첸트 아이템 재료 사용가능한 값 (MAX_ENCHANT_NUMBER보다 크면안됨)
#define MAX_ENCHANT_GRADE	10		// 인챈트 최대 등급

#define AMBER_BOX_LIST_NUM				48 // 호박상자. 산자에서 나오는  아이템의 최대 개수.
#define AMBER_BOX_SELECT_ITEM_LIST_NUM	 3 // 호박상자. 선택되는 아이템의 개수.

#define MAX_NEWSELECTBOX_SELECTINDEX	12 // [영훈] 새로운 선택상자에서 선택 가능한 갯수
#define MAX_NEWSELECTBOX_ITEM_LIST		12 // [영훈] 새로운 선택상자에서 하나의 선택에서 나오는 아이템 갯수

class cltItemManagerCommon;
class cltItem;
class cltSummonScroll;

struct stElement
{
	UI08 siType;	//속성석의 종류
	SI16 siPower;	//속성석의 힘 

	stElement()
	{
		siType = 0;
		siPower = 0;
	}
};

class stEnchantNeed
{
public:
	SI32 siItemGrade;
	REAL32 EnchantRate;	// 성공확률
	REAL32 EnchantDownRate; // 다운그레이드확률
	REAL32 EnchantFailRate; // 실패해서 사라질 확률
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_ENCHANT_NUMBER>		siElementUnique;
	NSafeTArray<SI32, MAX_ENCHANT_NUMBER>		siElementNum;
#else
	SI32 siElementUnique[MAX_ENCHANT_NUMBER];
	SI32 siElementNum[MAX_ENCHANT_NUMBER];
#endif
	SI32 siNeedSkill;

	stEnchantNeed()
	{
		ZeroMemory(this, sizeof(stEnchantNeed));
	}

	~stEnchantNeed()
	{
	}
};


//--------------------------------------------
// OldItem
//--------------------------------------------
class cltItemWeaponOld{
public:
	UI32 uiDurability:		10;
	UI32 uiMaxDurability:	10;
	UI32 uiMinDamage:		11;
	UI32 uiMaxDamage:		11;
};

class cltItemArmourOld{
public:
	UI16 uiDurability;
	UI16 uiMaxDurability;

	UI16 uiAC;
};

class cltItemRingOld
{
public:
	UI16 uiDurability;
	UI16 uiMaxDurability;
	UI16 uiAC;
	UI08 uiStr;
	UI08 uiDex;
	UI08 uiVit;
	UI08 uiMag;
};
class cltItemNeckOld{
public:
	UI16 uiDurability;
	UI16 uiMaxDurability;
	
	UI08 uiSkillPointUp;
};
class cltItemShoesOld{
public:
	UI16 uiDurability;
	UI16 uiMaxDurability;
	
	UI16 uiAC;

	UI08 uiMoveSpeed;
};



// 아이템의 유니온 크기. 
#define MAX_ITEM_UNION_BYTE_OLD 16

class cltItemOld{
	public:
		SI16 siUnique;
		SI16 siItemNum;		// 아이템이 몇개나 있는가 ?

	public:
		union
		{
			BYTE			cData8[MAX_ITEM_UNION_BYTE_OLD];
			SI32			siData32[MAX_ITEM_UNION_BYTE_OLD/4];

			cltItemWeaponOld	clItemWeapon;
			cltItemArmourOld	clItemArmour;
			cltItemNeckOld		clItemNeck;
			cltItemShoesOld		clItemShoes;
			cltItemRingOld		clItemRing;
			
		};

};
//---------------------------------------------------------------------------

// Obj아이템의 기본 정보. (4바이트 ) 
class cltItemCommonInfo
{
public:
	enum
	{
		REASON_NO,				  
		REASON_GIVE_PARTYHALL_ITEM,
	};

	UI32 uiDurability:		11;			// 현재 내구도. (0 ~ 2047)
	UI32 uiMaxDurability:	11;			// 최대 내구도. (0 ~ 2047)
	UI32 uiRareInfo:		3;			// 명품인지 여부. 
	UI32 uiGetItemReason:	7;			// 아이템을 받게 된  특별한 이유.
};

// Obj아이템 옵션 정보. (4바이트) 
class cltItemOptionInfo{
public:
	UI32 uiType1:			6;		// 옵션의 종류.(0~63)
	UI32 uiPara1:			10;		// 옵션에 따른 파라메타. 
	UI32 uiType2:			6;		// 옵션의 종류.(0~63)
	UI32 uiPara2:			10;		// 옵션에 따른 파라메타. 
	
	// 아이템 옵션 텍스트를 구한다. 
	bool GetText(TCHAR* ptext, SI16 txtSize);
	
};


//---------------------------------------
// 개개 아이템의 정보. 
//---------------------------------------
class cltItemCommon{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;
};

class cltItemWeapon{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	UI32	uiMinDamage:	11;		// 최소 공격력. 
	UI32	uiMaxDamage:	11;		// 최대 공격력.
	UI32	uiMag:			10;		// 지팡이이의 마법력 향상 효과. 

	void Init(UI16 dur, UI16 mindmg, UI16 maxdmg, UI16 mag)
	{
		ZeroMemory(this, sizeof(cltItemWeapon));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiMinDamage		= mindmg;
		uiMaxDamage		= maxdmg;
		uiMag			= mag;
	}
	void Set(cltItemWeapon* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemWeapon));
	}
};


class cltItemArmour{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	UI32				uiAC:		12;	// 방어력. 
	UI32				uiLuck:		12;	// 운 
	UI32				uiSkillUp:	6;	// 기술 향상. 


	void Init(UI16 dur, UI16 uiac, UI16 luck, UI16 skillup)
	{
		ZeroMemory(this, sizeof(cltItemArmour));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiAC			= uiac;
		uiLuck			= luck;
		uiSkillUp		= skillup;
	}
	void Set(cltItemArmour* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemArmour));
	}
};


// 가방 
class cltItemBag{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	UI32				uiAC:		10;	// 방어력. 



	void Init(UI16 dur, UI16 uiac)
	{
		ZeroMemory(this, sizeof(cltItemBag));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiAC			= uiac;
	}
	void Set(cltItemBag* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemBag));
	}
};

// 망토. 
class cltItemMantle{
public:
	cltItemCommonInfo	clCommonInfo;	
	cltItemOptionInfo	clOptionInfo;

	UI32				uiAC:		10;	// 방어력. 
	UI32				uiDex:		6;	// 순발력
	UI32				uiVit:		6;	// 생명력 
	UI32				uiStr:		6;	// 근력

	void Init(UI16 dur, UI16 uiac, UI16 str, UI16 dex, UI16 vit)
	{
		ZeroMemory(this, sizeof(cltItemMantle));

		clCommonInfo.uiDurability		= dur;	
		clCommonInfo.uiMaxDurability	= dur;

		uiAC			= uiac;
		uiDex			= dex;
		uiVit			= vit;
		uiStr			= str;
	}
	void Set(cltItemMantle* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemMantle));
	}
};

// 아바타 망토
class cltItemAvatarMantle
{
public:
	cltItemCommonInfo clCommonInfo;
	cltItemOptionInfo clOptionInfo;

	// 아이템 데이터 크기는 64bit 까지 확장 가능.
	// 방어력 최대 증가 수치 120. (레어템일경우 3배까지 증가될 수 있음) 120*3 = 360 이 넘지 않게 설정.
	UI32	uiAC:		10;		// 방어력. 
	
	// 현재 망토엔 AC(방어력)만 붙으나 추후에 확장 가능성이 있어 일단 주석처리.
	UI32	uiStr:		6;		// 근력
	UI32	uiDex:		6;		// 순발력 
	UI32	uiVit:		6;		// 생명력 
	void init(UI16 dur, UI16 uiac, UI16 str, UI16 dex, UI16 vit)
	{
		ZeroMemory(this, sizeof(cltItemAvatarMantle));

		clCommonInfo.uiDurability		=	dur;	// 현재 내구도.(0~2047)
		clCommonInfo.uiMaxDurability	=	dur;	// 최대 내구도. (0 ~ 2047)

		uiAC		=	uiac;

		//아래도 써도 상관없다.
		uiStr		=	str;		
		uiDex	=	dex;
		uiVit		=	vit;
	}

	void Set(cltItemAvatarMantle* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemAvatarMantle));
	}
};

// 아바타 의복
class cltItemAvatarDress
{
public:
	cltItemCommonInfo clCommonInfo;
	cltItemOptionInfo clOptionInfo;

	//아이템 데이터 크기는 64bit 까지 확장 가능.
	UI32				uiSkillUp:	6;	// 기술 향상. 

	void init(UI16 dur , SI32 skillup)
	{
		ZeroMemory(this, sizeof(cltItemAvatarDress));
		
		clCommonInfo.uiDurability		=	dur;	// 현재내구도
		clCommonInfo.uiMaxDurability	=	dur;	// 최대내구도

		uiSkillUp = skillup;
	}
	
	void Set(cltItemAvatarDress* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemAvatarDress));
	}
};

// 아바타 모자
class cltItemAvatarHat
{
public:
	cltItemCommonInfo clCommonInfo;
	cltItemOptionInfo clOptionInfo;

	// 기술 향상. 아이템 데이터 크기는 64bit 까지 확장 가능.
	UI32 uiSkillUp:		6;		

	void init(UI16 dur ,UI16 siSkillUp)
	{
		ZeroMemory(this, sizeof(cltItemAvatarHat));

		clCommonInfo.uiDurability		=	dur;	// 현재내구도
		clCommonInfo.uiMaxDurability	=	dur;	// 최대내구도
		
		uiSkillUp = siSkillUp;
	}


	void Set(cltItemAvatarHat* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemAvatarHat));
	}
};

// 아바타 목걸이
class cltItemAvatarNeck
{
public:
	cltItemCommonInfo clCommonInfo;
	cltItemOptionInfo clOptionInfo;

	// 아이템 데이터 크기는 64bit 까지 확장 가능.
	//UI32 uiBaseSkillType:		4;	// 기본 대분류 스킬 타입

	UI32 uiUpgradeSkillUnique:	8;	// 최종 선택된 스킬 타입	
	UI32 uiSkillValue:		    8;	// 해당 스킬에 대한 가중치 (실제 증가 수치)

	UI32 uiMaxUsedate:		    8;  // 최대 usedate 
	UI32 uiMaxSkillPoint:		8;	// 최대 기술상승 점수


	void init(UI16 dur, UI16 siSkillValue, UI08 MaxSkillPoint,UI08 MaxUsedate )
	{
		ZeroMemory(this, sizeof(cltItemAvatarNeck));

		clCommonInfo.uiDurability		=	dur;	// 현재 내구도.(0~2047)
		clCommonInfo.uiMaxDurability	=	dur;	// 최대 내구도. (0 ~ 2047)

		uiSkillValue	=	siSkillValue;

		uiMaxUsedate = MaxUsedate;
		uiMaxSkillPoint = MaxSkillPoint;
	}

	void Set(cltItemAvatarNeck* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemAvatarNeck));
	}
};

// 아바타 반지
class cltItemAvatarRing
{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	// 아이템 데이터 크기는 64bit 까지 확장 가능.
	UI32 uiAC:		6;		// 방어력
	UI32 uiStr:		5;		// 힘
	UI32 uiDex:		5;		// 순발력
	UI32 uiVit:		5;		// 생명력

	UI32 uiMag:		5;		// 지력
	UI32 uiWis:		5;		// 지혜
	UI32 uiLuk:		5;		// 행운
	UI32 uiHnd:		5;		// 손재주

	void Init( UI16 dur, UI08 ac, UI08 str, UI08 dex, UI08 vit, UI08 mag, UI08 wis, UI08 luk, UI08 hnd )
	{
		ZeroMemory(this, sizeof(cltItemAvatarRing));

		clCommonInfo.uiDurability		= dur;		// 현재 내구도
		clCommonInfo.uiMaxDurability	= dur;		// 최대 내구도

		uiAC			= ac;
		uiStr			= str;
		uiDex			= dex;
		uiVit			= vit;
		uiMag			= mag;
		uiWis			= wis;
		uiLuk			= luk;
		uiHnd			= hnd;
	}
	void Set(cltItemAvatarRing* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemAvatarRing));
	}

	void SetQualityRate(SI32 siQualityRate);	// 지연 : 레어 수치에 따라 스탯을 변경시켜준다. (레어 물품 나오면 스탯이나 방어력 등이 증가하도록 해주는 함수)
};

class cltItemRing
{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	UI32 uiStr: 4;
	UI32 uiDex: 4;
	UI32 uiVit: 4;
	UI32 uiMag: 4;
	UI32 uiLuk: 4;
	UI32 uiHnd: 4;

	void Init( UI16 dur, UI08 ac, UI08 str, UI08 dex, UI08 vit, UI08 mag, UI08 luk, UI08 hnd )
	{
		ZeroMemory(this, sizeof(cltItemRing));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;
		uiStr			= str;
		uiDex			= dex;
		uiVit			= vit;
		uiMag			= mag;
		uiLuk			= luk;
		uiHnd			= hnd;
	}
	void Set(cltItemRing* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemRing));
	}
};


class cltItemNeck{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;
	UI08 uiSkillPointUp;				// 기술 상승 점수 . (어떤 기술인지는 cltItemInfo에 담겨 있다.) 
	UI08 uiMaxUsedate;				    // 최대 usedate 
	UI08 uiMaxSkillPoint;				// 최대 기술상승 점수

	void Init(UI16 dur, UI08 SkillPointUp, UI08 MaxUsedate, UI08 MaxSkillPoint)
	{
		ZeroMemory(this, sizeof(cltItemNeck));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiSkillPointUp = SkillPointUp;
		uiMaxUsedate = MaxUsedate;
		uiMaxSkillPoint = MaxSkillPoint;

	}

	void Set(cltItemNeck* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemNeck));
	}
};

class cltItemShoes{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;
	UI32				uiAC:			10;		// 방어력. 
	UI32				uiMoveSpeed:	6;		// 이동속도. 	

	void Init(UI16 dur, UI16 ac, UI08 movespeed)
	{
		ZeroMemory(this, sizeof(cltItemShoes));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiAC			= ac;
		uiMoveSpeed		= movespeed;
	}

	void Set(cltItemShoes* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemShoes));
	}
};

// 도구 
class cltItemTool{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	void Init(UI16 dur)
	{
		ZeroMemory(this, sizeof(cltItemTool));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;
	}
	
	void Set(cltItemTool* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemTool));
	}
};

class cltItemStatusArmour{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	UI32				uiSkillUp:	6;	// 기술 향상. 
	UI32				uiReserve:	2;	// 예약

	UI32 uiStr: 8;
	UI32 uiDex: 8;
	UI32 uiVit: 8;
	UI32 uiMag: 8;
	UI32 uiHnd: 8;
	UI32 uiLuk: 8;
	UI32 uiWis: 8;

	void Init(UI16 dur, UI16 skillup)
	{
		ZeroMemory(this, sizeof(cltItemStatusArmour));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiSkillUp		= skillup;
	}

	void Set(cltItemStatusArmour* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltItemStatusArmour));
	}

	void SetStatus(UI08 Str, UI08 Dex, UI08 Mag, UI08 Vit, UI08 Hnd, UI08 Luk)
	{
		uiStr = Str;
		uiDex = Dex;
		uiMag = Mag;
		uiVit = Vit;
		uiHnd = Hnd;
		uiLuk = Luk;
	}
};


//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
class cltItemCrystal{
	public:
		cltItemCommonInfo	clCommonInfo;
		cltItemOptionInfo	clOptionInfo;
	
		SI16 siMaxDurability;			// 최대 내구도. (0 ~ 65535)

		// MAX값은 16이다.
		UI08 uiCrystalAbility; // 결정체가 가지는 능력 타입.
		UI08 uiCrystalAbilityNum;  // 결정체가 가지는 능력 수치.	
		
		void Init(UI16 dur, SI16 MaxDurability, UI08 CrystalAbility, UI08 CrystalAbilityNum)
		{
			ZeroMemory(this, sizeof(cltItemCrystal));

			clCommonInfo.uiDurability		= dur;
			clCommonInfo.uiMaxDurability	= dur;

			siMaxDurability = MaxDurability;
			
			uiCrystalAbility = CrystalAbility;
			uiCrystalAbilityNum = CrystalAbilityNum;

		}

		void Set(cltItemCrystal* pcldata)
		{
			memcpy(this, pcldata, sizeof(cltItemCrystal));
		}

		void SetMaxDurability(SI16 _siMaxDurability){ siMaxDurability = _siMaxDurability; }

		SI16 GetMaxDurability(){ return siMaxDurability; }
		SI16 GetCrystalAblilty(){ return (SI16)uiCrystalAbility; }
		SI16 GetCrystalAbilityNum(){ return (SI16)uiCrystalAbilityNum; }
};


//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
class cltCrystalItem{
	public:	
		SI16 siCrystalType; // 결정체 종류.(유니크 )
		SI16 siDurability;			// 현재 내구도. (0 ~ 65535)
		SI16 siMaxDurability;			// 최대 내구도. (0 ~ 65535)

		// MAX값은 16이다.
		UI08 uiCrystalAbility; // 결정체가 가지는 능력 타입.
		UI08 uiCrystalAbilityNum;  // 결정체가 가지는 능력 수치.	
	
		cltCrystalItem()
		{
			ZeroMemory(this, sizeof(cltCrystalItem));
		}

		BOOL IsSame( cltCrystalItem* pclCrystalItem )
		{
			if(pclCrystalItem == NULL )										return FALSE;

			if(siCrystalType != pclCrystalItem->siCrystalType)				return FALSE;
			if(siDurability != pclCrystalItem->siDurability)				return FALSE;
			if(siMaxDurability != pclCrystalItem->siMaxDurability)			return FALSE;

			if(uiCrystalAbility != pclCrystalItem->uiCrystalAbility)		return FALSE;
			if(uiCrystalAbilityNum != pclCrystalItem->uiCrystalAbilityNum)	return FALSE;

			return TRUE;
		}

		void Init()
		{
			ZeroMemory(this, sizeof(cltCrystalItem));
		}

		void Set(cltCrystalItem* pcldata)
		{
			memcpy(this, pcldata, sizeof(cltCrystalItem));
		}

		bool SetMaxDurability(SI16 maxDurability) // 내구도의 최대치를 설정한다.
		{
			if(maxDurability <= 30000) //최대 내구도는 30000 이다. - 타입상.
			{
				siMaxDurability = 	maxDurability;
				return true;
			}

			return false;
		}

		void SetDurability(SI16 Durability){ siDurability = Durability; }
		
		void DecDurability(SI16 decDurability) // 현재 내구도를 감소 시킨다.
		{
			siDurability  -= decDurability;

			if(siDurability < 0)
				siDurability = 0;		
		}
		SI16 GetCrystalType(){ return siCrystalType; }
		SI16 GetDurability(){ return siDurability; }
		SI16 GetMaxDurability(){ return siMaxDurability; }
		SI16 GetCrystalAblilty(){ return (SI16)uiCrystalAbility; }
		SI16 GetCrystalAbilityNum(){ return (SI16)uiCrystalAbilityNum; }

};


// 아이템의 유니온 크기. 
#define MAX_ITEM_UNION_BYTE 16

//-------------------------------------------
// 아이템의 실질 데이터와 같은 구조여야 한다. 
//-------------------------------------------
class cltItemData{
public:
	SI16 siUnique;
	SI16 siItemNum;		// 아이템이 몇개나 있는가 ?

	UI08 uiDateYear;
	UI08 uiDateMonth;
	UI08 uiDateDay;
	UI16 uiDateUseDay;

	SI32 siMakerPersonID;

	stElement Element;
public:
	union
	{
		BYTE			cData8[MAX_ITEM_UNION_BYTE];
		SI32			siData32[MAX_ITEM_UNION_BYTE/4];

		cltItemCommon	clItemCommon;
		cltItemWeapon	clItemWeapon;
		cltItemArmour	clItemArmour;
		cltItemNeck		clItemNeck;
		cltItemShoes	clItemShoes;
		cltItemRing		clItemRing;
		cltItemTool		clItemTool;
		cltItemBag		clItemBag;
		cltItemMantle	clItemMantle;
		cltItemStatusArmour	clItemStatusArmour;
		cltItemCrystal			clItemCrystal;
		cltItemAvatarRing	clItemAvatarRing;
		cltItemAvatarMantle	clItemAvatarMantle;
		cltItemAvatarNeck	clItemAvatarNeck;
		cltItemAvatarDress	clItemAvatarDress;
		cltItemAvatarHat clItemAvatarHat;
	};

	// by LEEKH 2007.08.21 - 아이템 시리얼 ID 추가
	SI32 siSID;		// 아이템의 SERIAL ID

	SI16 siPalette;	// 팔레트 유니크

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI08 siUseCrystalNum;  // 사용가능한 결정체 슬롯수.
#ifdef _SAFE_MEMORY
	NSafeTArray<cltCrystalItem, MAX_CRYSTAL_NUM>		cltItemCrytal;
#else
	cltCrystalItem cltItemCrytal[MAX_CRYSTAL_NUM];
#endif

	void Init()
	{
		ZeroMemory(this, sizeof(cltItemData));

		siUnique = 0 ;
		siItemNum = 0 ;

		uiDateYear = 0 ;
		uiDateMonth = 0 ;
		uiDateDay = 0 ;
		uiDateUseDay = 0 ;

		Element.siType = 0;
		Element.siPower = 0;

		siSID = 0;

		siUseCrystalNum = 0;
		for ( SI16 i=0 ; i < MAX_CRYSTAL_NUM ; i++ )
		{
			cltItemCrytal[i].Init();
		}
	}
	void Set(const cltItem* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItemData));
	}
	void Set(const cltItemData* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItemData));
	}
	

};

//---------------------------------------
// 아이템 실질 구조. 
//---------------------------------------
class cltItem{
	public:	
		SI16 siUnique;
		SI16 siItemNum;		// 아이템이 몇개나 있는가 ?

		UI08 uiDateYear;
		UI08 uiDateMonth;
		UI08 uiDateDay;
		UI16 uiDateUseDay;

		SI32 siMakerPersonID;

		stElement Element;

public:
	union
	{
		BYTE			cData8[MAX_ITEM_UNION_BYTE];
		SI32			siData32[MAX_ITEM_UNION_BYTE/4];

		cltItemCommon	clItemCommon;
		cltItemWeapon	clItemWeapon;
		cltItemArmour	clItemArmour;
		cltItemNeck		clItemNeck;
		cltItemShoes	clItemShoes;
		cltItemRing		clItemRing;
		cltItemTool		clItemTool;
		cltItemBag		clItemBag;
		cltItemMantle	clItemMantle;
		cltItemStatusArmour	clItemStatusArmour;
		cltItemCrystal			clItemCrystal;
		cltItemAvatarMantle	clItemAvatarMantle;
		cltItemAvatarNeck	clItemAvatarNeck;
		cltItemAvatarDress	clItemAvatarDress;
		cltItemAvatarHat clItemAvatarHat;
		cltItemAvatarRing	clItemAvatarRing;

	};

	// by LEEKH 2007.08.21 - 아이템 시리얼 ID 추가
	SI32 siSID;		// 아이템의 SERIAL ID

	SI16 siPalette;	// 팔레트 유니크

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI08 siUseCrystalNum;  // 사용가능한 결정체 슬롯수.
#ifdef _SAFE_MEMORY
	NSafeTArray<cltCrystalItem, MAX_CRYSTAL_NUM>		cltItemCrytal;
#else
	cltCrystalItem cltItemCrytal[MAX_CRYSTAL_NUM];
#endif


	cltItem()
	{
		ZeroMemory(this, sizeof(cltItem));
	};

	cltItem(SI32 siUnique);
	cltItem(SI32 siUnique , SI32 siUsedate);

	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemWeapon* pcldata);
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemArmour* pcldata);	
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemShoes* pcldata);
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemRing* pcldata);	
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemStatusArmour* pcldata);	
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemMantle* pcldata);
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemAvatarMantle* pcldata); // 아바타 망토
	cltItem(SI32 siUnique, SI32 siUseDate, cltItemAvatarNeck* pcldata); // 아바타 목걸이
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemAvatarDress* pcldata); // 아바타 의복
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemAvatarHat* pcldata); // 아바타 모자
	cltItem(SI32 siUnique, SI32 siUseDate, stElement* element, cltItemAvatarRing* pcldata); // 아바타 망토
	cltItem(SI32 siUnique, SI32 siUseDate, cltItemTool* pcldata);
	cltItem(SI32 siUnique, SI32 siUseDate, cltItemNeck* pcldata);

	cltItem(SI32 siUnique, SI32 siUseDate, cltItemBag* pcldata);
	cltItem(SI32 siUnique, cltItemBag* pcldata);
	


	void Init()
	{
		ZeroMemory(this, sizeof(cltItem));
	}
	void Set(const cltItem* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItem));
	}
	void Set(const TCHAR* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItem));
	}
	void Set(const cltItemData* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItem));
	}


	SI32 GetImage(cltItemManagerCommon* pclItemManager);
	const TCHAR* GetShortName(cltItemManagerCommon* pclItemManager);
	bool GetFullName(cltItemManagerCommon* pclItemManager, TCHAR* pname, SI16 txtSize);
	// 아이템의 이름과 개수를 얻어온다. 
	bool GetFullNameNNum(cltItemManagerCommon* pclItemManager, TCHAR* pname, SI16 txtSize);
	TCHAR* GetArmCode(cltItemManagerCommon* pclItemManager);

	
	// 아이템이 제 정보를 제대로 받았는지 확인한다.
	BOOL CheckType(cltItemManagerCommon* pclItemManager, SI32 orgtype);

	// 아이템의 타입을 찾는다. 
	SI32 GetType(cltItemManagerCommon* pclItemManager)const;

	// 아이템의 타입을 찾는다. 
	SI32 GetAavatarType(cltItemManagerCommon* pclItemManager)const;

	// 아이템의 공격사거리를 찾는다. 
	SI32 GetAttackRange(cltItemManagerCommon* pclItemManager)const;

	// 아이템의 공격타입을 구한다. 
	SI32 GetAttackType(cltItemManagerCommon* pclItemManager)const;

	// 아이템의 공격주기를  찾는다. 
	SI32 GetAttackInterval(cltItemManagerCommon* pclItemManager)const;



	//----------------------------------------------------------------------------------------------------
	// 추가 마법 데미지를 구한다. - 크리스탈.
	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.

	// 최소 데미지를 구한다. 
	SI32 GetMinDamage(cltItemManagerCommon* pclItemManager, SI32* CrystalPer);
	// 최대 데미지를 구한다. 
	SI32 GetMaxDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);

	SI32 GetAddMagicalDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetAddMagicalMinDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetAddMagicalMaxDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);

	// 명중률 상승. 
	SI32 GetPhysicalHitRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetMagicalHitRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);

	SI32 GetPhysicalAC(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetMagicalAC(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	
	SI32 GetPhysicalDodgeRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetMagicalDodgeRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);

	SI32 GetCrystalHitpoint(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalManapoint(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalWeigt(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);

	SI32 GetCrystalFireAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalWaterAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalWindAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalEarthAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalFireDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalWaterDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalWindDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	SI32 GetCrystalEarthDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);

	SI32 GetCriticalHitRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	//------------------------------------------------------------------------------------------------------


	// 인챈트 데미지를 구한다.  by tae-yang
	SI32 GetEnchantDamage(cltItemManagerCommon* pclItemManager)const;
	// 인챈트 타입 구한다.  
	SI32 GetEnchantType(cltItemManagerCommon* pclItemManager)const;

	SI32 GetEnchantAC(cltItemManagerCommon* pclItemManager,bool bClient)const;
	SI32 GetEnchantACFrom(cltItemManagerCommon* pclItemManager,bool bClient, SI32 attacktype)const;

	SI32 GetEnchantAttFrom(cltItemManagerCommon* pclItemManager,bool bClient, SI32 attacktype)const;
		
	// 최소 데미지를 설정한다. 
	SI32 SetMinDamage(cltItemManagerCommon* pclItemManager, SI32 damage);
	// 최대 데미지를 설정한다. 
	SI32 SetMaxDamage(cltItemManagerCommon* pclItemManager, SI32 damage);


	// 근력을 구한다. 
	SI32 GetEquipStr(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	// 순발력을 구한다. 
	SI32 GetEquipDex(cltItemManagerCommon* pclItemManager , SI32* CrystalPer);
	// 생명력을 구한다. 
	SI32 GetEquipVit(cltItemManagerCommon* pclItemManager, SI32* CrystalPer);
	// 도력을 구한다. 
	SI32 GetEquipMag(cltItemManagerCommon* pclItemManager, SI32* CrystalPer);
	// 운을 구한다. 
	SI32 GetEquipLuk(cltItemManagerCommon* pclItemManager, SI32* CrystalPer);
	// 손재주을 구한다. 
	SI32 GetEquipHnd(cltItemManagerCommon* pclItemManager, SI32* CrystalPer);
	// 지혜를 구한다. 
	SI32 GetEquipWis(cltItemManagerCommon* pclItemManager, SI32* CrystalPer);


	// 방어능력을 구한다 
	SI32 GetAC(cltItemManagerCommon* pclItemManager)const;
	
	// 방어 능력을 설정한다. 
	SI32 SetAC(cltItemManagerCommon* pclItemManager, SI32 ac);

	
	// 운을 구한다 
	SI32 GetLuck(cltItemManagerCommon* pclItemManager)const;

	// 기술력 향상을 구한다. 
	// 크리스탈에 관한 능력은 따로 계산해 줘야 한다. - 쟏러쟏리ㅓㅑㄷ로ㅑㅈ도리
	SI32 GetUpgradeSkillLevel(cltItemManagerCommon* pclItemManager, SI32 skillunique , bool countCrystalAbilityNum = true);

	// 특정 기술의 부적에 의한 상승 효과를 구한다. 
	SI32 GetSkillScrollUp(cltItemManagerCommon* pclItemManager, SI32 skillunique);

	//----------------------------------------
	// 기간제 관련. 
	//----------------------------------------
	BOOL IsHaveUseDay() // 기간제 아이템인가?
	{
		return (uiDateUseDay > 0); //  아이템이 기간제 아이템인지 체크
	}

	//----------------------------------------
	// 내구성 관련. 
	//----------------------------------------
	// 내구도를 구한다. 
	SI32 GetDurability()
	{
		return clItemCommon.clCommonInfo.uiDurability;
	}
	// 최대내구도를 구한다.
	SI32 GetMaxDurability()
	{
		return clItemCommon.clCommonInfo.uiMaxDurability;
	}

	// 내구도에 의한 패널티를 리턴한다
	REAL32 GetDurabilityPenlty( cltItemManagerCommon* pclItemManager );

	// 명품/호품 여부를 구한다.
	SI32 GetRareType()
	{
		return clItemCommon.clCommonInfo.uiRareInfo;
	}

	// 내구도를 설정한다. 
	SI32 SetDurability(cltItemManagerCommon* pclItemManager, UI32 dur);

	// 최대 내구도를 구한다. 
	SI32 GetMaxDurability(cltItemManagerCommon* pclItemManager)const;
	// 최대 내구도를 설정한다. 
	SI32 SetMaxDurability(cltItemManagerCommon* pclItemManager, UI32 dur);

	// 내구성을 줄인다. 
	bool DecreaseDurability(cltItemManagerCommon* pclitemmanager, SI32 durvary);

	//----------------------------------------
	// 이동속도 관련 
	//----------------------------------------
	SI32 GetMoveSpeed(cltItemManagerCommon* pclItemManager)const;


	// 아이템에다가 아이템을 더한다. 
	bool Add(cltItemManagerCommon* pclItemManager, cltItem* pclitem);

	// 아이템이 같은 것인지 비교하는 함수. 
	// numcomp : 개수까지 같아야 하는지 여부. 
	BOOL IsSame(cltItem* pclitem, bool numcomp);
	
	BOOL IsBulletItem();

	
	void Set(cltItem* pclitem)
	{
		if(pclitem == NULL)return ;

		memcpy(this, pclitem, sizeof(cltItem));
	}

	// 무게를 구한다.개수에 따라 무게도 늘어난다.
	SI32 GetWeight(cltItemManagerCommon* pclItemManager, BOOL mode);

	//----------------------------------------------
	// 아이템 개수 관련. 
	//----------------------------------------------
	SI16 GetItemNum()const{return siItemNum;}
	void SetItemNum(SI16 itemnum)
	{
		if(itemnum > MAX_ITEM_PILE_NUMBER)
		{
			siItemNum	= MAX_ITEM_PILE_NUMBER;
		}
		else if(itemnum <0)
		{
			siItemNum	= 0;
		}
		else
		{
			siItemNum	= itemnum;
		}
	}

	// 아이템의 수를 늘인다. 
	SI32 Increase(SI32 amount);
	// 아이템의 수를 줄인다. 
	SI32 Decrease(SI32 amount);

	// 중첩이 되는 아이템인지 확인한다. 
	bool CanPile(cltItemManagerCommon* pclItemManager);

	// 수리가 가능한 아이템인지 확인한다. 
	bool CanRepair(cltItemManagerCommon* pclItemManager);
	
	// 이 아이템을 만드는데 필요한 스킬이 무엇인지 확인한다. 
	SI32 GetMakeSkill(cltItemManagerCommon* pclItemManager);
	
	// 이 무기의 기본 명중률을 구한다. 
	SI32 GetDefaultHitRate(cltItemManagerCommon* pclItemManager);


	//--------------
	// 식품류. 
	//-------------
	// 허기를 채워주는 정도를 찾는다.
	SI32 GetHungry(cltItemManagerCommon* pclItemManager)const;

	//-------------
	//약류.
	//-------------
	// 체력을 회복시켜주는 정도. 
	SI32 GetLife(cltItemManagerCommon* pclItemManager)const;
	// 마법력을 회복시켜주는 정도. 
	SI32 GetMana(cltItemManagerCommon* pclItemManager)const;
	// 치료해 줄 수 있는 질병의 유니크를 찾는다. 
	SI32 GetDisease(cltItemManagerCommon* pclItemManager)const;


	//---------------
	// 책류
	//---------------
	//연마시키는 스킬의 유니크
	SI32 GetSkillUnique(cltItemManagerCommon* pclItemManager)const;
	//기술의 레벨 제한. 
	SI32 GetSkillLevelLimit(cltItemManagerCommon* pclItemManager)const;
	

	// 아이템을 수리한다. 
	bool Repair(cltItemManagerCommon* pclItemManager, SI32 reducedur);

	// 밀봉상태인가?
	bool IsSealingUp(cltItemManagerCommon* pclItemManager);

	// 귀속상태인가?
	bool IsBelonging(cltItemManagerCommon* pclItemManager);

	// 전용아이템이 맞나?
	bool IsCorrectPrivateEquip(cltItemManagerCommon* pclItemManager, SI32 siKind);

	// 이호식에서 분해 가능한가?
	bool IsDisassemble(cltItemManagerCommon* pclItemManager);

	// 기간을 연장할 수 있는 아이템인가?
	bool IsPlusUseDate(cltItemManagerCommon* pclItemManager);

	// SID를 생성해야 하는가?
	bool IsCreateSID(cltItemManagerCommon* pclItemManager);

	// ----------------------------------------------------------------------
	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 슬롯 셋팅.
	void  DelUseCrystalNum(SI08 indexDelUseCrystalNum)
	{
		if((indexDelUseCrystalNum > 0 )||(indexDelUseCrystalNum <= MAX_CRYSTAL_NUM))
		{
			 cltItemCrytal[indexDelUseCrystalNum].Init(); 
		}
	}

	bool SetUseCrystalNum(SI08 UseCrystalNum)
	{
		if((UseCrystalNum < 0 )||(UseCrystalNum >MAX_CRYSTAL_NUM))
			return false;
	
		if( UseCrystalNum <= MAX_CRYSTAL_NUM)
		{
			siUseCrystalNum = UseCrystalNum;

			for(int i = siUseCrystalNum+1; i <=MAX_CRYSTAL_NUM; i++)
			{	
				DelUseCrystalNum(i);
			}
			return true;
		}

		return false;
	}

	// [추가 : 황진성 2008. 3. 2 => 생성되있는 빈 슬롯이 있는가? 
	bool EmptyCrystalSlot(void)
	{
		for(int Count = 0; Count < siUseCrystalNum; ++Count)
		{
			if( 0 == cltItemCrytal[Count].GetCrystalType() )
			{
				return true;
			}
		}
		return false;
	}

	// [ 추가 : 황진성 2008. 3. 2 => 생성되있는 빈 슬롯의 개수 ] 
	SI32 EmptyCrystalSlotCount(void)
	{
		SI32 EmptySlotCount = 0;
        	for(int Count = 0; Count < siUseCrystalNum; ++Count)
		{
			if( 0 == cltItemCrytal[Count].GetCrystalType() )
			{
				++EmptySlotCount;
			}
		}
		
		return EmptySlotCount;
	}

	SI16 GetUseCrystalNum(){ return (SI16)siUseCrystalNum;}
	SI32 GetAllCrystalAbilityNum(UI08 CrystalAbility);  // 이 아이템에 박혀있는 모든 결정체에서, 원하는 능력의 합을 구하여반환한다.
	SI32 GetCrystalAbilityFromSkillUnique(SI32 skillunique);

	// 해당 아이템의 모든 결정체 정보를 받아 온다. - 내구도 등에 쓸려구
	//bool GetAllCrystalInfo(cltItemManagerCommon* pclItemManager ,cltCrystalItem  * ItemCrytal );

	// 내구성을 줄인다. - 아이템 단위로 '박혀'있는 모든 결정체가 일괄적으로 내구도가 설정된다.
	bool DecreaseCrystalDurability(cltItemManagerCommon* pclitemmanager, SI16 durvary);

	// 크리스탈의 내구도가 1%단위로 줄어, Client로 보내야 하는지 여부를 결정한다.
	bool GetDecreaseCrystalDurabilityPer(cltItemManagerCommon* pclItemManager);

	SI32 GetCrystalMaxDurability()	{ return clItemCrystal.siMaxDurability;		}
	SI32 GetCrystalAbility()		{ return clItemCrystal.uiCrystalAbility;	}
	SI32 GetCrystalAbilityNum()		{ return clItemCrystal.uiCrystalAbilityNum; }

	// ----------------------------------------------------------------------

	SI32 GetCrystalGrade( void );								// [영훈] 크리스탈 아이템의 등급을 돌려준다
	bool IsWeapon( cltItemManagerCommon* pclItemManager );		// [영훈] 무기인가?
	SI16 GetEquipLevel( cltItemManagerCommon* pclItemManager );	// [영훈] 착용레벨을 얻어온다	
	bool IsHaveCrystalSlot( void )								// [영훈] 크리스탈 슬롯을 갖고 있는가
	{
		if ( siUseCrystalNum )
		{
			return true;
		}

		return false;
	}

	void GetGMSectionData(GMSectionData* pkGMSectionData);

	// 밀봉을 해제한다
	bool CancellationSealingUp( SYSTEMTIME sTime );

	SI32 SelectUseBaseSkillTypeToAvatar(UI32 uiBaseSkillType);
};


//---------------------------------------
// 캐릭터의 아이템 보관 정보. 
//---------------------------------------
class cltPItemInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEM_PER_PERSON>	clItem;		// 소환수 장비 포함 - 제거 by LEEKH 2008-08-07
#else
	cltItem		clItem[MAX_ITEM_PER_PERSON];		// 소환수 장비 포함 - 제거 by LEEKH 2008-08-07
#endif

	cltPItemInfo(){};
	~cltPItemInfo(){};

	void Set(cltPItemInfo* pclitemmanager);

	void Init()
	{
		SI32 i;

		for(i = 0; i < MAX_ITEM_PER_PERSON;i++)
		{
			clItem[i].Init();
		}
	}

	// 전체의 무게를 얻어온다. 
	SI32 GetTotalWeight(cltItemManagerCommon* pclItemManager);

	// 무장의 타입을 구한다. 
	SI32 GetWeaponType(cltItemManagerCommon* pclItemManager, SI16 activeweapon);

	// 무장의 사거리를 구한다. 
	SI32 GetWeaponAttackRange(cltItemManagerCommon* pclItemManager, SI16 activeweapon);

	// 무장의 최소 데미지를 구한다. 
	SI32 GetWeaponMinDamage(cltItemManagerCommon* pclItemManager, SI16 activeweapon);
	// 무장의 최대 데미지를 구한다. 
	SI32 GetWeaponMaxDamage(cltItemManagerCommon* pclItemManager, SI16 activeweapon);

	// 무장의 근력을 구한다. 
	SI32 GetEquipStr(cltItemManagerCommon* pclItemManager);
	// 무장의 순발력을 구한다. 
	SI32 GetEquipDex(cltItemManagerCommon* pclItemManager);
	// 무장의 생명력을 구한다. 
	SI32 GetEquipVit(cltItemManagerCommon* pclItemManager);
	// 무장의 도력을 구한다. 
	SI32 GetEquipMag(cltItemManagerCommon* pclItemManager, SI16 activeweapon);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
	//=====================================
	
	
	//=====================================
	
	//=====================================
	// 무기의 명중률 향상을 구한다. 
	SI32 GetEquipHitRate(cltItemManagerCommon* pclItemManager, SI16 activeweapon);
	// 무기의 필살률 향상을 구한다. 
	SI32 GetEquipCriticalHitRate(cltItemManagerCommon* pclItemManager, SI16 activeweapon);
	// 장비의 데미지 감소를 구한다. 
	SI32 GetDamageReduceRate(cltItemManagerCommon* pclItemManager);
	//=====================================
	// 근 무기의 중독률 향상을 구한다. 
	SI32 GetEquipPoisonRate(cltItemManagerCommon* pclItemManager, SI16 activeweapon);
	// 원 무기의 동빙한설확률 향상을 구한다. 
	SI32 GetEquipIceRate(cltItemManagerCommon* pclItemManager, SI16 activeweapon);
	// 신발의 흡성대법확률 향상을 구한다. (신말의 위차가 고정되어 있다)
	SI32 GetEquipDrainRate(cltItemManagerCommon* pclItemManager,SI16 active = PERSONITEM_SHOES );
	//=====================================
	
	// 무장의 AC를 구한다. 
	SI32 GetAC(cltItemManagerCommon* pclItemManager);
	
	// 무장의 운을 구한다. 
	SI32 GetLuck(cltItemManagerCommon* pclItemManager);
	
	// 신발의 이동속도를 구한다. 
	SI32 GetMoveSpeed(cltItemManagerCommon* pclItemManager);
	
	// 장비에의한 기술 향상을 구한다. 
	SI32 GetUpgradeSkillLevel(cltItemManagerCommon* pclItemManager, SI32 skillunique);
	
	// 무장의 공격 타입을 구한다. 
	SI32 GetWeaponAttackType(cltItemManagerCommon* pclItemManager, SI16 activeweapon);
	
	// 무장의 공격 주기를 구한다. 
	SI32 GetWeaponAttackInterval(cltItemManagerCommon* pclItemManager, SI16 activeweapon);

	// 특정위치에, 특정아이템 타입의 아이템을 추가 할 수 있는지 검사한다.
	bool CanToitemPos(cltItemManagerCommon* pclItemManager, SI32 toItempos );
	
	// 인벤토리에 아이템을 추가할 수 있는지 확인한다. 
	bool CanAddInv( cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve,
		SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue, SI16* pCheckPos = NULL );

	bool IsBlankInv(cltItemManagerCommon*pclItemManager,SI32 invnum,SI32 invmode);

	// 인벤토리에 여러 개수(최대 MAX_CANADDINV_CHECK_NUM) 만큼의 아이템을 추가할 수 있는지 확인만 한다.  
	bool CanAddInvManyItems(cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve,
		SI32* prtnvalue);

	// 인벤토리에 아이템을 추가한다. 
	bool AddInv(cltItemManagerCommon* pclItemManager, cltItem* pclitem);
	
	// 인벤토리 아이템중에서 하나를 랜덤하게 얻어온다. 
	bool RobInv(SI32* pitempos, cltItem* pclitem);
	
	bool IsSame(cltPItemInfo* pcliteminfo,  bool numcomp );
	
	// 주어진 아이템과 정확하게 일치하는 아이템을 찾아서 그 위치를 리턴한다. 
	SI32 FindItemInv(cltItem* pclitem, bool numcomp);

	// 주어진 아이템과 정확하게 일치하는 아이템을 찾아서 그 위치를 리턴한다. 
	SI32 FindItemInv(SI32 itemunique);

	
	// unique의 아이템을 몇개나 갖고 있는가 ?
	SI32 GetItemNum(SI32 itemunique);

	// 장비창안에 unique의 아이템을 몇개나 갖고 있는가?
	SI32 GetItemNumInEquip(SI32 itemunique);
	
	// unique의 아이템이 인벤에 존재하는가 ?
	SI32  GetItemPos(SI32 itemunique);
	SI32  GetItemTotalPos(SI32 itemunique);

	// 장비창안에 unique의 아이템의 위치 리턴
	SI32 GetItemPosInEquip(SI32 itemunique);



	// 특정 아이템들이 이 인벤에 들어갈 수 있는가. 
	// 들어갈 수 있다면 어느 위치에 들어갈 수 있는가를 구해낸다. 
	bool CanInputItemList(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32 itemlistnum, SI16* pclpos);
	
	// 특정 아이템이 장비품에 장비되어 있는가 ?
	bool HaveInEquip(SI32 itemunique);
	
	// 모든 장비품이 정상적으로 장비되어 있는가 ?
	bool IsAllProperEquip(cltItemManagerCommon* pclItemManager, SI16* punique, SI16* ppos, SI32 siKind = 0);
	
	// 말이 물품을 가지고 있는가?
	bool IsHorseHaveItem();
	
	// 가방에  물품을 가지고 있는가?
	bool IsBagHaveItem();

	// 무기 사용 부적을 사용한 것이 있는가 ?
	// skill : SKILL_USESWORD1 ...
	bool UseWeasponSkillScroll(cltItemManagerCommon* pclItemManager,SI32 skill);
	UI32 UseWeasponSkillScrollNum(cltItemManagerCommon* pclItemManager,SI32 skill);


	cltItem* GetEquipSet(SI32 equipset)
	{
		switch(equipset)
		{
			case EQUIPSET_PERSON:	return &clItem[0];
		}
		
		MessageBox(NULL, TEXT("vefs2") , TEXT("vefsw") , MB_OK);
		return &clItem[0];
	}

	// siItemUnique를 siAmount만큼 만들 수 있는 재료를 같고 있는가?
	bool IsHaveMaterialItem( cltItemManagerCommon* pclItemManager, SI32 siItemUnique, SI32 siAmount );
	
};

//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
// 클라이언트 전달용 정보 클레스.( 현재는 내구도만 가지고 간다.)
class cltCrystalItemInfo{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_CRYSTAL_NUM>		siDurability;
#else
	SI16 siDurability[MAX_CRYSTAL_NUM];	// 클라이언트로 보내는 각 결정체의 현재 내구도. (0 ~ 65535)
#endif

	cltCrystalItemInfo()
	{
		ZeroMemory(this, sizeof(cltCrystalItemInfo));
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltCrystalItemInfo));
	}

	void Set(cltCrystalItemInfo* pcldata)
	{
		memcpy(this, pcldata, sizeof(cltCrystalItemInfo));
	}

};
/*
//---------------------------------------------------
// 기간제 귀속 아이템 관련
//---------------------------------------------------
#define MAX_ITEMNUM_PER_LIMITDATEITEM	5


class cltPLimitDateItemInfo
{
public:
	SI32		siItemUnique[MAX_ITEMNUM_PER_LIMITDATEITEM];
	SI32		siItemLimitDate[MAX_ITEMNUM_PER_LIMITDATEITEM];

public:
	cltPLimitDateItemInfo(){};
	~cltPLimitDateItemInfo(){};

	void Set(cltPLimitDateItemInfo * pcllimitdateiteminfo)
	{
		memcpy ( this, pcllimitdateiteminfo, sizeof( cltPLimitDateItemInfo ));
	}

	void Set(SI32* itemunique,SI32* limitdate)
	{
		for ( SI32 i = 0 ; i < MAX_ITEMNUM_PER_LIMITDATEITEM ; i ++ )
		{
			siItemUnique[i] = itemunique[i] ;
			siItemLimitDate[i] = limitdate[i] ;
		}
	}

	bool Delete(SI32 itemunique)
	{
		for ( SI32 i = 0 ; i < MAX_ITEMNUM_PER_LIMITDATEITEM ; i ++ )
		{
			if ( siItemUnique[i] == itemunique )
			{
				siItemUnique[i] = 0 ;
				siItemLimitDate[i] = 0 ;
				return true ;
			}
		}

		return false ;
	}

	bool Add(SI32 itemunique,SI32 limitdate)
	{
		for ( SI32 i = 0 ; i < MAX_ITEMNUM_PER_LIMITDATEITEM ; i ++ )
		{
			if ( siItemUnique[i] == 0 )
			{
				siItemUnique[i] = itemunique ;
				siItemLimitDate[i] = limitdate ;
				return true ;
			}
		}

		return false ;
	}

};*/

//----------------------------------------------------
// 아이템 몰 아이템 관련
//----------------------------------------------------
#define MAX_ITEMNUM_PER_ITEMMALL	8 // 아이템 몰에서의 최대 아이템 갯수

class cltPItemMallItemInfo
{
public:
	cltPItemMallItemInfo(){};
	~cltPItemMallItemInfo(){};
	
	void Set(cltPItemMallItemInfo * pclitemmall)
	{
		memcpy ( this, pclitemmall, sizeof( cltPItemMallItemInfo ));
	}
	
	void Set( cltItem * pclitemmallitem ,SI32 *itemid,SI32 total,SI32 page,bool existence)
	{
#ifdef _SAFE_MEMORY
		SI32 i=0;
		for(i=0; i<MAX_ITEMNUM_PER_ITEMMALL; i++)		clItem[i].Set(&pclitemmallitem[i]);
		for(i=0; i<MAX_ITEMNUM_PER_ITEMMALL; i++)		siItemID[i] = itemid[i];
#else
		memcpy ( clItem, pclitemmallitem, sizeof( cltItem ) * MAX_ITEMNUM_PER_ITEMMALL );
		memcpy ( siItemID, itemid, sizeof( SI32 ) * MAX_ITEMNUM_PER_ITEMMALL );
#endif
		
		siTotalItemNum = total;
		siListPage = page;
		
		bItemExistence = existence ;
	}
	
	void SetItem(cltItem * pclitemmallitem)
	{
#ifdef _SAFE_MEMORY
		SI32 i=0;
		for(i=0; i<MAX_ITEMNUM_PER_ITEMMALL; i++)		clItem[i].Set(&pclitemmallitem[i]);
#else
		memcpy ( clItem, pclitemmallitem, sizeof( cltItem ) * MAX_ITEMNUM_PER_ITEMMALL );
#endif
	}
	cltItem GetItem(SI32 pos)
	{
		cltItem pclitem ;
		pclitem.Init();
		if ( pos < 0 || pos >= MAX_ITEMNUM_PER_ITEMMALL ) return pclitem ;
		return clItem[pos];
	}
	
	void SetItemID(SI32 * itemid)
	{
#ifdef _SAFE_MEMORY
		SI32 i=0;
		for(i=0; i<MAX_ITEMNUM_PER_ITEMMALL; i++)		siItemID[i] = itemid[i];
#else
		memcpy ( siItemID, itemid, sizeof( SI32 ) * MAX_ITEMNUM_PER_ITEMMALL );
#endif
	}
	SI32 GetItemID(SI32 pos)
	{
		SI32 itemid = 0 ;
		if ( pos < 0 || pos >= MAX_ITEMNUM_PER_ITEMMALL ) return itemid ;
		return siItemID[pos];
	}
	
	void SetTotalItemNum(SI32 total)
	{
		siTotalItemNum = total;
	}
	SI32 GetTotalItemNum()
	{
		return siTotalItemNum ;
	}
	
	void SetListPage(SI32 page)
	{
		siListPage = page;
	}
	SI32 GetListPage()
	{
		return siListPage ;
	}
	
	void SetExistence(bool existence)
	{
		bItemExistence = existence ;
	}
	bool GetExistence()
	{
		return bItemExistence ;
	}
	
	void Init()
	{
		for(SI32 i = 0; i < MAX_ITEMNUM_PER_ITEMMALL;i++)
		{
			clItem[i].Init();
		}
		
		SetTotalItemNum(0);
		SetListPage(0);
		
		SetExistence(false);
	}
	
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEMNUM_PER_ITEMMALL>		clItem;
	NSafeTArray<SI32, MAX_ITEMNUM_PER_ITEMMALL>			siItemID;
#else
	cltItem		clItem[MAX_ITEMNUM_PER_ITEMMALL];
	SI32		siItemID[MAX_ITEMNUM_PER_ITEMMALL];
#endif
	
	SI32		siTotalItemNum; // 총 아이템 갯수
	SI32		siListPage; // 현재 선택된 리스트의 페이지 수 1~
	
	bool		bItemExistence; // 산 아이템이 있는지 없는지
	
};

// cltItemOld정보를 cltItem정보로 바꿔준다.
/*cyj ConvertItemToitem2 주석처리
inline bool ConvertItemToitem2(cltItemOld* pclitem, cltItem* pclitem2)
{

	pclitem2->Init();
	
	SI32 unique = pclitem->siUnique;

	pclitem2->siUnique	= pclitem->siUnique;
	pclitem2->siItemNum	= pclitem->siItemNum;

	// 무기류. 
	if(unique <= ITEMUNIQUE(599))
	{
		pclitem2->clItemWeapon.clCommonInfo.uiMaxDurability		= pclitem->clItemWeapon.uiMaxDurability;
		pclitem2->clItemWeapon.clCommonInfo.uiDurability		= pclitem->clItemWeapon.uiDurability;
		pclitem2->clItemWeapon.uiMinDamage						= pclitem->clItemWeapon.uiMinDamage;
		pclitem2->clItemWeapon.uiMaxDamage						= pclitem->clItemWeapon.uiMaxDamage;
	}
	// 방어구류. 
	else if(unique <= ITEMUNIQUE(999))
	{
		pclitem2->clItemArmour.clCommonInfo.uiMaxDurability	= pclitem->clItemArmour.uiMaxDurability;
		pclitem2->clItemArmour.clCommonInfo.uiDurability	= pclitem->clItemArmour.uiDurability;
		pclitem2->clItemArmour.uiAC							= pclitem->clItemArmour.uiAC;
	}
	// 목걸이류. 
	else if(unique <= ITEMUNIQUE(1499))
	{
		pclitem2->clItemNeck.clCommonInfo.uiMaxDurability	= pclitem->clItemNeck.uiMaxDurability;
		pclitem2->clItemNeck.clCommonInfo.uiDurability	= pclitem->clItemNeck.uiDurability;
		pclitem2->clItemNeck.uiSkillPointUp				= pclitem->clItemNeck.uiSkillPointUp;
	}
	// 신발류. 
	else if(unique <= ITEMUNIQUE(1999))
	{
		pclitem2->clItemShoes.clCommonInfo.uiMaxDurability	= pclitem->clItemShoes.uiMaxDurability;
		pclitem2->clItemShoes.clCommonInfo.uiDurability	= pclitem->clItemShoes.uiDurability;
		pclitem2->clItemShoes.uiAC			= pclitem->clItemShoes.uiAC;
		pclitem2->clItemShoes.uiMoveSpeed	= pclitem->clItemShoes.uiMoveSpeed;
	}
	// 반지류 .
	else if(unique >= ITEMUNIQUE(7500) && unique < ITEMUNIQUE(8000))
	{
		pclitem2->clItemRing.clCommonInfo.uiMaxDurability	= pclitem->clItemRing.uiMaxDurability;
		pclitem2->clItemRing.clCommonInfo.uiDurability		= pclitem->clItemRing.uiDurability;
		pclitem2->clItemRing.uiStr							= pclitem->clItemRing.uiStr;
		pclitem2->clItemRing.uiDex							= pclitem->clItemRing.uiDex;
		pclitem2->clItemRing.uiVit							= pclitem->clItemRing.uiVit;
		pclitem2->clItemRing.uiMag							= pclitem->clItemRing.uiMag;
	}
	// 기타 모든 아이템. . 
	else 
	{
	}

	return true;
}
*/

//[추가 : 황진성 2008. 1. 31 => cltTreasureBox, cltSpaceBox 에서 사용할 아이템 넣을곳 찾기.]
bool CanAddInv( bool BoxKind, cltItem* clItem, cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve,
			   SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue, SI16* pCheckPos = NULL );

SI16 GetBoxItemCount(bool BoxKind, cltItem* clItem, SI16 siArrayCount);

//[추가 : 황진성 2008. 1. 29 => 공간 상자 아이템 구조체]
class cltSpaceBox
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEM_SPACE_BOX>	clItem;
#else
	cltItem		clItem[MAX_ITEM_SPACE_BOX];
#endif

	cltSpaceBox() { ZeroMemory( this, sizeof( cltSpaceBox ) ); }
	~cltSpaceBox(){}

	void Init()
	{
		ZeroMemory( this, sizeof( cltSpaceBox ) );
	}

	void Set(cltSpaceBox* pclSpaceBox)
	{
		if(pclSpaceBox == NULL)		return;

		for(SI32 i=0; i<MAX_ITEM_SPACE_BOX; i++)
		{
			clItem[i].Set(&pclSpaceBox->clItem[i]);
		}
	}
};

//[추가 : 황진성 2008. 1. 29 => 공간 상자.(보물) 아이템 구조체]
class cltTreasureBox
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEM_TREASURE_BOX>	clItem;
#else
	cltItem		clItem[MAX_ITEM_TREASURE_BOX];
#endif

	cltTreasureBox() { ZeroMemory( this, sizeof( cltTreasureBox ) ); }
	~cltTreasureBox(){}

	void Init()
	{
		ZeroMemory( this, sizeof( cltTreasureBox ) );
	}

	void Set(cltTreasureBox* pclTreasureBox)
	{
		if(pclTreasureBox == NULL)		return;

		for(SI32 i=0; i<MAX_ITEM_TREASURE_BOX; i++)
		{
			clItem[i].Set(&pclTreasureBox->clItem[i]);
		}
	}
};


#endif
