//---------------------------------
// 2003/5/20 김태곤
//---------------------------------

#ifndef _ITEMUNIT_H
#define _ITEMUNIT_H

#include <Windows.h>
#include <Directives.h>

#include "CommonHeader.h"

//-------------------------------
// 아이템 타입 유니크 .
//-------------------------------
#define ITEMTYPE_SWORD			1
#define ITEMTYPE_SPEAR			2
#define ITEMTYPE_STAFF			3
#define ITEMTYPE_AXE			4
#define ITEMTYPE_BOW			5
#define ITEMTYPE_GUN			6
#define ITEMTYPE_CANNON			7
#define ITEMTYPE_ARMOUR			8
#define ITEMTYPE_DRESS			9
#define ITEMTYPE_NECK			10
#define ITEMTYPE_RING			11
#define ITEMTYPE_BELT			12
#define ITEMTYPE_SHOES			13
#define ITEMTYPE_HELMET			14
#define ITEMTYPE_HAT			15
#define ITEMTYPE_FOOD			16
#define ITEMTYPE_SEAFOOD		17
#define ITEMTYPE_HEAL			18
#define ITEMTYPE_BOOK			19
#define ITEMTYPE_MINERAL		20
#define ITEMTYPE_ANIMAL			21
#define ITEMTYPE_TREE			22
#define ITEMTYPE_PAPER			23
#define ITEMTYPE_CLOTH			24
#define ITEMTYPE_SCROLL			25
#define ITEMTYPE_ETC			26
#define ITEMTYPE_IRON			27
#define ITEMTYPE_DISH			28
#define ITEMTYPE_FISHINGROD		29
#define ITEMTYPE_DOLL			30
#define ITEMTYPE_TICKET			31
#define ITEMTYPE_BAG			32
#define ITEMTYPE_MANTLE			33
#define ITEMTYPE_TORPEDO		34


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




#define MAX_ITEM_PER_PERSON		105
#define MAX_SHAPE_ITEM			15		// 장비가능한 물품의 수. 	
#define MAX_INVITEM_PER_PERSON	60		// 사람과 말의 모든 인벤 합계. 
#define MAX_ITEM_PER_INVENTORY	20
#define MAX_ITEM_PILE_NUMBER	10000				// 한개의 아이템안에 중첩될 수 있는 아이템의 개수. 

#define MAX_DURABILITY			1000	// 아이템 내구도의 최대값. 
#define FOREVER_DURABILITY		1001	// 내구도가 소모되지 않는 상태. 	


#define EQUIPSET_PERSON				1
#define EQUIPSET_SUMMON1			2
#define EQUIPSET_SUMMON2			3

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
#define PERSONITEM_RESERVED0	13
#define PERSONITEM_RESERVED1	14


#define PERSONITEM_INV0			15
#define PERSONITEM_INV1			16
#define PERSONITEM_INV2			17
#define PERSONITEM_INV3			18
#define PERSONITEM_INV4			19
#define PERSONITEM_INV5			20

#define PERSONITEM_HORSE0		35

#define PERSONITEM_BAG0			55

#define PERSONITEM_SUMMON1EQUIP0	75	

#define PERSONITEM_SUMMON2EQUIP0	90



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
#define SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING		20		// 반지제조술 향상 
#define SCROLL_UNIQUE_FUNCTION_UPMAXDURABILITY		21		// 아이템의 최대 내구도 향상 

#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD		22		// 무기사용술 향상 - 칼 
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR		23		// 무기사용술 향상 - 창 
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE		24		// 무기사용술 향상 - 도끼 
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW		25		// 무기사용술 향상 - 활
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN		26		// 무기사용술 향상 - 총  
#define SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF		27		// 무기사용술 향상 - 지팡이

//-------------------------------
// 인벤모드 
//-------------------------------
#define INVMODE_PERSON					1		// 사람 인벤 
#define INVMODE_HORSE					2		// 말	인벤 
#define INVMODE_BAG						3		// 가방 인벤 
#define INVMODE_ALL						4		// 말과 사람 인벤을 모두 검색. 


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

class cltItemManagerCommon;
class cltItem;
class cltSummonScroll;
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
			char			cData8[MAX_ITEM_UNION_BYTE_OLD];
			SI32			siData32[MAX_ITEM_UNION_BYTE_OLD/4];

			cltItemWeaponOld	clItemWeapon;
			cltItemArmourOld	clItemArmour;
			cltItemNeckOld		clItemNeck;
			cltItemShoesOld	clItemShoes;
			cltItemRingOld		clItemRing;
		};

};
//---------------------------------------------------------------------------

// Obj아이템의 기본 정보. (4바이트 ) 
class cltItemCommonInfo{
public:
	UI32 uiDurability:		11;			// 현재 내구도. (0 ~ 2047)
	UI32 uiMaxDurability:	11;			// 최대 내구도. (0 ~ 2047)
	UI32 uiRareInfo:		3;			// 명품인지 여부. 
	UI32 uiBrand:			7;			// 브랜드 식별자
};

// Obj아이템 옵션 정보. (4바이트) 
class cltItemOptionInfo{
public:
	UI32 uiType1:			6;		// 옵션의 종류.(0~63)
	UI32 uiPara1:			10;		// 옵션에 따른 파라메타. 
	UI32 uiType2:			6;		// 옵션의 종류.(0~63)
	UI32 uiPara2:			10;		// 옵션에 따른 파라메타. 
	
	// 아이템 옵션 텍스트를 구한다. 
	bool GetText(char* ptext);
	
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

class cltItemRing
{
public:
	cltItemCommonInfo	clCommonInfo;
	cltItemOptionInfo	clOptionInfo;

	UI08 uiStr;
	UI08 uiDex;
	UI08 uiVit;
	UI08 uiMag;


	void Init( UI16 dur, UI16 uiac, UI08 str, UI08 dex, UI08 vit, UI08 mag )
	{
		ZeroMemory(this, sizeof(cltItemRing));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;
		uiStr			= str;
		uiDex			= dex;
		uiVit			= vit;
		uiMag			= mag;
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

	void Init(UI16 dur, UI08 SkillPointUp)
	{
		ZeroMemory(this, sizeof(cltItemNeck));

		clCommonInfo.uiDurability		= dur;
		clCommonInfo.uiMaxDurability	= dur;

		uiSkillPointUp = SkillPointUp;
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

// 아이템의 유니온 크기. 
#define MAX_ITEM_UNION_BYTE 12

//-------------------------------------------
// 아이템의 실질 데이터와 같은 구조여야 한다. 
//-------------------------------------------
class cltItemData{
public:
	SI16 siUnique;
	SI16 siItemNum;		// 아이템이 몇개나 있는가 ?
	
public:
	union
	{
		char			cData8[MAX_ITEM_UNION_BYTE];
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
	};
	
	void Init()
	{
		ZeroMemory(this, sizeof(cltItemData));
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

	public:
		union
		{
			char			cData8[MAX_ITEM_UNION_BYTE];
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
		};

	cltItem()
	{
		ZeroMemory(this, sizeof(cltItem));
		
		siItemNum = 0;		

	};
	cltItem(SI32 siUnique);
	cltItem(SI32 siUnique, cltItemWeapon* pcldata);
	cltItem(SI32 siUnique, cltItemArmour* pcldata);
	cltItem(SI32 siUnique, cltItemNeck* pcldata);
	cltItem(SI32 siUnique, cltItemShoes* pcldata);
	cltItem(SI32 siUnique, cltItemRing* pcldata);
	cltItem(SI32 siUnique, cltItemTool* pcldata);
	cltItem(SI32 siUnique, cltItemBag* pcldata);
	cltItem(SI32 siUnique, cltItemMantle* pcldata);

	void Init()
	{
		ZeroMemory(this, sizeof(cltItem));
	}
	void Set(const cltItem* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItem));
	}
	void Set(const char* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItem));
	}
	void Set(const cltItemData* pclitem)
	{
		memcpy(this, pclitem, sizeof(cltItem));
	}


	SI32 GetImage(cltItemManagerCommon* pclItemManager);
	const char* GetShortName(cltItemManagerCommon* pclItemManager);
	bool GetFullName(cltItemManagerCommon* pclItemManager, char* pname);
	// 아이템의 이름과 개수를 얻어온다. 
	bool GetFullNameNNum(cltItemManagerCommon* pclItemManager, char* pname);
	char* GetArmCode(cltItemManagerCommon* pclItemManager);

	
	// 아이템이 제 정보를 제대로 받았는지 확인한다.
	BOOL CheckType(cltItemManagerCommon* pclItemManager, SI32 orgtype);

	// 아이템의 타입을 찾는다. 
	SI32 GetType(cltItemManagerCommon* pclItemManager)const;
	
	// 아이템의 공격사거리를 찾는다. 
	SI32 GetAttackRange(cltItemManagerCommon* pclItemManager)const;

	// 아이템의 공격타입을 구한다. 
	SI32 GetAttackType(cltItemManagerCommon* pclItemManager)const;

	// 아이템의 공격주기를  찾는다. 
	SI32 GetAttackInterval(cltItemManagerCommon* pclItemManager)const;


	// 최소 데미지를 구한다. 
	SI32 GetMinDamage(cltItemManagerCommon* pclItemManager)const;
	// 최대 데미지를 구한다. 
	SI32 GetMaxDamage(cltItemManagerCommon* pclItemManager)const;
	
	// 최소 데미지를 설정한다. 
	SI32 SetMinDamage(cltItemManagerCommon* pclItemManager, SI32 damage);
	// 최대 데미지를 설정한다. 
	SI32 SetMaxDamage(cltItemManagerCommon* pclItemManager, SI32 damage);


	// 근력을 구한다. 
	SI32 GetEquipStr(cltItemManagerCommon* pclItemManager)const;
	// 순발력을 구한다. 
	SI32 GetEquipDex(cltItemManagerCommon* pclItemManager)const;
	// 생명력을 구한다. 
	SI32 GetEquipVit(cltItemManagerCommon* pclItemManager)const;
	// 도력을 구한다. 
	SI32 GetEquipMag(cltItemManagerCommon* pclItemManager)const;

	// 방어능력을 구한다 
	SI32 GetAC(cltItemManagerCommon* pclItemManager)const;
	
	// 방어 능력을 설정한다. 
	SI32 SetAC(cltItemManagerCommon* pclItemManager, SI32 ac);

	
	// 운을 구한다 
	SI32 GetLuck(cltItemManagerCommon* pclItemManager)const;

	// 기술력 향상을 구한다. 
	SI32 GetUpgradeSkillLevel(cltItemManagerCommon* pclItemManager, SI32 skillunique);

	// 특정 기술의 부적에 의한 상승 효과를 구한다. 
	SI32 GetSkillScrollUp(cltItemManagerCommon* pclItemManager, SI32 skillunique);




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
	BOOL IsSame(const cltItem* pclitem, bool numcomp);

	
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
	

};


//---------------------------------------
// 캐릭터의 아이템 보관 정보. 
//---------------------------------------
class cltPItemInfo{
public:
	cltItem		clItem[MAX_ITEM_PER_PERSON];

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
	
	// 인벤토리에 아이템을 추가할 수 있는지 확인한다. 
	bool CanAddInv(cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve,
		SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue);
	
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
	SI32  GetItemNum(SI32 itemunique);
	
	// unique의 아이템이 인벤에 존재하는가 ?
	SI32  GetItemPos(SI32 itemunique);
	SI32  GetItemTotalPos(SI32 itemunique);

	// 특정 아이템들이 이 인벤에 들어갈 수 있는가. 
	// 들어갈 수 있다면 어느 위치에 들어갈 수 있는가를 구해낸다. 
	bool CanInputItemList(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32 itemlistnum, SI16* pclpos);
	
	// 특정 아이템이 장비품에 장비되어 있는가 ?
	bool HaveInEquip(SI32 itemunique);
	
	// 모든 장비품이 정상적으로 장비되어 있는가 ?
	bool IsAllProperEquip(cltItemManagerCommon* pclItemManager, SI16* punique, SI16* ppos);
	
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
		case EQUIPSET_SUMMON1:	return &clItem[PERSONITEM_SUMMON1EQUIP0];
		case EQUIPSET_SUMMON2:	return &clItem[PERSONITEM_SUMMON2EQUIP0];
		}

		MessageBox(NULL, "vefs2", "vefsw", MB_OK);
		return &clItem[0];
	}
	
};

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

};

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
		memcpy ( clItem, pclitemmallitem, sizeof( cltItem ) * MAX_ITEMNUM_PER_ITEMMALL );
		memcpy ( siItemID, itemid, sizeof( SI32 ) * MAX_ITEMNUM_PER_ITEMMALL );
		
		siTotalItemNum = total;
		siListPage = page;
		
		bItemExistence = existence ;
	}
	
	void SetItem(cltItem * pclitemmallitem)
	{
		memcpy ( clItem, pclitemmallitem, sizeof( cltItem ) * MAX_ITEMNUM_PER_ITEMMALL );
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
		memcpy ( siItemID, itemid, sizeof( SI32 ) * MAX_ITEMNUM_PER_ITEMMALL );
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
	cltItem		clItem[MAX_ITEMNUM_PER_ITEMMALL];
	SI32		siItemID[MAX_ITEMNUM_PER_ITEMMALL];
	
	SI32		siTotalItemNum; // 총 아이템 갯수
	SI32		siListPage; // 현재 선택된 리스트의 페이지 수 1~
	
	bool		bItemExistence; // 산 아이템이 있는지 없는지
	
};

//-------------------------------------------------
// 배 아이템 목록
//-------------------------------------------------
#define MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER			5		// 대포 또는 어뢰 장착 최대수
#define MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER			5		// 총 또는 활의 장착 최대수
#define MAX_SHIP_TRANSPORT_STG_ITEM_NUMBER					10		// 배 짐 창고마다 실을 수 있는 아이템 최대 개수

#define MAX_SHIP_TRANSPORT_STG_ITEM_CONSUME					10		// 짐칸에 아이템을 소모하는 최대 개수.

enum { SHIP_STG_KIND_WEAPON_TYPE_CANNON, SHIP_STG_KIND_WEAPON_TYPE_GUN, SHIP_STG_KIND_TRANSPORT };	// 무기고냐 짐칸이냐 구분

class cltPShipStgItemInfo
{
public:
	cltItem clWeaponStgTypeCannonItem[ MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER ];
	cltItem clWeaponStgTypeGunItem[ MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER ];
	cltItem clTransportStgItem[ MAX_SHIP_TRANSPORT_STG_ITEM_NUMBER ];
	
	cltPShipStgItemInfo()
	{
		Init();
	}
	
	void Init()
	{
		memset( this, 0, sizeof( cltPShipStgItemInfo ) );
	}
	
	bool IsHaveItem()
	{
		SI16 i = 0;

		for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER; ++i )
		{
			if ( clWeaponStgTypeCannonItem[ i ].siUnique > 0 )
			{
				return true;
			}
		}

		for ( i = 0; i < MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER; ++i )
		{
			if ( clWeaponStgTypeGunItem[ i ].siUnique > 0 )
			{
				return true;
			}
		}

		for ( i = 0; i < MAX_SHIP_TRANSPORT_STG_ITEM_NUMBER; ++i )
		{
			if ( clTransportStgItem[ i ].siUnique > 0 )
			{
				return true;
			}
		}

		return false;
	}
	
	void Set( cltPShipStgItemInfo *pShipStgItemInfo )
	{
		memcpy( this, pShipStgItemInfo, sizeof( cltPShipStgItemInfo ) );
	}
	
	void Set( cltItem *pclWeaponStgTypeCannonItemList, cltItem *pclWeaponStgTypeGunItemList, cltItem *pclTransportStgItemList )
	{		
		memcpy ( clWeaponStgTypeCannonItem, pclWeaponStgTypeCannonItemList, sizeof( cltItem ) * MAX_SHIP_WEAPON_STG_TYPE_CANNON_ITEM_NUMBER );
		memcpy ( clWeaponStgTypeGunItem, pclWeaponStgTypeGunItemList, sizeof( cltItem ) * MAX_SHIP_WEAPON_STG_TYPE_GUN_ITEM_NUMBER );
		memcpy ( clTransportStgItem, pclTransportStgItemList, sizeof( cltItem ) * MAX_SHIP_TRANSPORT_STG_ITEM_NUMBER );
	}
	
	SI16 GetAddItemPos( SI08 StgKind, bool PileSwitch, SI16 *pItemPos, cltItem *pclItem );
	bool CanMove( SI08 StgKind, bool PileSwitch, SI16 ItemPos, cltItem *pclItem );
	SI16 GetShipEmptyPos( SI08 StgKind );	
	SI16 GetShipItemPos( SI08 StgKind, SI32 ItemUnique );	
	bool SetItem( SI08 StgKind, SI16 ItemPos, cltItem *pclItem );	
	void TransportStgItemConsume( SI16 *ItemPosList, cltItem *pclItemList );
		
	SI32 GetStgTotalWeight( cltItemManagerCommon* pclItemManager, SI08 StgKind );	
	SI32 GetStgWeaponWeight( cltItemManagerCommon* pclItemManager );
};

//-------------------------------------------------
// 각각의 배들의 아이템들 묶음 
//-------------------------------------------------
class cltPShipItemInfo{
public:
	cltPShipStgItemInfo clShipStgItemInfo[ MAX_SHIP_PER_PERSON ];
	
	cltPShipItemInfo()
	{
		Init();
	}
	
	void Init()
	{
		memset( this, 0, sizeof( cltPShipItemInfo ) );
	}
	
	void Init( SI16 Ref )
	{
		clShipStgItemInfo[ Ref ].Init();
	}
	
	~cltPShipItemInfo(){};
	
	void Set( cltPShipItemInfo *pclShipItemInfo )
	{
		memcpy( this, pclShipItemInfo, sizeof( cltPShipItemInfo ) );
	}
	
	void Set( SI16 ShipRef, cltItem *pclWeaponStgTypeCannonItemList, cltItem *pclWeaponStgTypeGunItemList, cltItem *pclTransportStgItemList )
	{
		clShipStgItemInfo[ ShipRef ].Set( pclWeaponStgTypeCannonItemList, pclWeaponStgTypeGunItemList, pclTransportStgItemList );
	}
	
	bool SetItem( SI16 ShipRef, SI08 StgKind, SI16 ItemPos, cltItem *pclItem );
	SI16 GetAddItemPos( SI16 ShipRef, SI08 StgKind, bool PileSwitch, SI16 *pItemPos, cltItem *pclItem );	
	bool CanMove( SI16 ShipRef, SI08 StgKind, bool PileSwitch, SI16 ItemPos, cltItem *pclItem );	
	void TransportStgItemConsume( SI16 ShipRef, SI16 *ItemPosList, cltItem *pclItemList );	
	SI16 GetShipItemPos( SI16 ShipRef, SI08 StgKind, SI16 ItemUnique );
};

// cltItemOld정보를 cltItem정보로 바꿔준다.
inline bool ConvertItemToitem2(cltItemOld* pclitem, cltItem* pclitem2)
{

	pclitem2->Init();
	
	SI32 unique = pclitem->siUnique;

	pclitem2->siUnique	= pclitem->siUnique;
	pclitem2->siItemNum	= pclitem->siItemNum;

	// 무기류. 
	if(unique <= 599)
	{
		pclitem2->clItemWeapon.clCommonInfo.uiMaxDurability		= pclitem->clItemWeapon.uiMaxDurability;
		pclitem2->clItemWeapon.clCommonInfo.uiDurability		= pclitem->clItemWeapon.uiDurability;
		pclitem2->clItemWeapon.uiMinDamage						= pclitem->clItemWeapon.uiMinDamage;
		pclitem2->clItemWeapon.uiMaxDamage						= pclitem->clItemWeapon.uiMaxDamage;
	}
	// 방어구류. 
	else if(unique <= 999)
	{
		pclitem2->clItemArmour.clCommonInfo.uiMaxDurability	= pclitem->clItemArmour.uiMaxDurability;
		pclitem2->clItemArmour.clCommonInfo.uiDurability	= pclitem->clItemArmour.uiDurability;
		pclitem2->clItemArmour.uiAC							= pclitem->clItemArmour.uiAC;
	}
	// 목걸이류. 
	else if(unique <= 1499)
	{
		pclitem2->clItemNeck.clCommonInfo.uiMaxDurability	= pclitem->clItemNeck.uiMaxDurability;
		pclitem2->clItemNeck.clCommonInfo.uiDurability	= pclitem->clItemNeck.uiDurability;
		pclitem2->clItemNeck.uiSkillPointUp				= pclitem->clItemNeck.uiSkillPointUp;
	}
	// 신발류. 
	else if(unique <= 1999)
	{
		pclitem2->clItemShoes.clCommonInfo.uiMaxDurability	= pclitem->clItemShoes.uiMaxDurability;
		pclitem2->clItemShoes.clCommonInfo.uiDurability	= pclitem->clItemShoes.uiDurability;
		pclitem2->clItemShoes.uiAC			= pclitem->clItemShoes.uiAC;
		pclitem2->clItemShoes.uiMoveSpeed	= pclitem->clItemShoes.uiMoveSpeed;
	}
	// 반지류 .
	else if(unique >= 7500 && unique < 8000)
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

#endif
