//---------------------------------
// 2003/5/17 ���°�
//---------------------------------

#ifndef _CLTITEM_H
#define _CLTITEM_H

#include "ItemCommon.h"
#include "ItemUnit.h"

#include "../../NLib/NDataLoader.h"
#include "../../NLib/NArray.h"
#include "../../../common/Char/PersonNameManager/PersonNameManager.h"
#include "../../../common/Item/ItemCommon/cltSetItem.h"
//-------------------------------------------
// TradeServer
//-------------------------------------------
#define MAX_TRADESERVER_OBJECT_RANK		500 // ���� ���� �ִ� ��� ��

//---------------------------------
// 2007/5/16 ����
//---------------------------------
#define MAKE_BY_MUNIN 1  // 0 = out, 1 = in
#define MAX_ITEMGRADE 10
#define ENCHANT_WRITE_TIME 300000 // 5min
//---------------------------------

#define MAX_ITEM_ITEMMALLIMAGE_NUM		250		// ������ ���� ����� �����Ͽ� �ִ� ���� �̹��� ����

//---------------------------------
// ��ȯ�� �����۰���
//---------------------------------
#define SUMMON_EQUIP_LIMITMINLEVEL 90	// ��ȯ���� �������� �����ϱ� ���� �ʿ��� �ּҷ���


// ������ ������ ����� ��
const SI32 siTradeServerObejectRank[MAX_TRADESERVER_OBJECT_RANK] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
190, 191, 192, 193, 194, 195, 196, 197, 198, 199,

200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
230, 231, 232, 233, 234, 235, 236, 237, 238, 239,	  
240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
250, 251, 252, 253, 254, 255, 256, 257, 258, 259,
260, 261, 262, 263, 264, 265, 266, 267, 268, 269,
270, 271, 272, 273, 274, 275, 276, 277, 278, 279,
280, 281, 282, 283, 284, 285, 286, 287, 288, 289,
290, 291, 292, 293, 294, 295, 296, 297, 298, 299,

300, 301, 302, 303, 304, 305, 306, 307, 308, 309,
310, 311, 312, 313, 314, 315, 316, 317, 318, 319,
320, 321, 322, 323, 324, 225, 326, 327, 328, 329,
330, 331, 332, 333, 334, 335, 336, 337, 338, 339,	  
340, 341, 342, 343, 344, 345, 346, 347, 348, 349,
350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
360, 361, 362, 363, 364, 365, 366, 367, 368, 369,
370, 371, 372, 373, 374, 375, 376, 377, 378, 379,
380, 381, 382, 383, 384, 385, 386, 387, 388, 389,
390, 391, 392, 393, 394, 395, 396, 397, 398, 399,

400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
410, 411, 412, 413, 414, 415, 416, 417, 418, 419,
420, 421, 422, 423, 424, 425, 426, 427, 428, 429,
430, 431, 432, 433, 434, 435, 436, 437, 438, 439,	  
440, 441, 442, 443, 444, 445, 446, 447, 448, 449,
450, 451, 452, 453, 454, 455, 456, 457, 458, 459,
460, 461, 462, 463, 464, 465, 466, 467, 468, 469,
470, 471, 472, 473, 474, 475, 476, 477, 478, 479,
480, 481, 482, 483, 484, 485, 486, 487, 488, 489,
490, 491, 492, 493, 494, 495, 496, 497, 498, 499,

500, 501, 502, 503, 504, 505, 506, 507, 508, 509,
510, 511, 512, 513, 514, 515, 516, 517, 518, 519,
520, 521, 522, 523, 524, 525, 526, 527, 528, 529,
530, 531, 532, 533, 534, 535, 536, 537, 538, 539,	  
540, 541, 542, 543, 544, 545, 546, 547, 548, 549,
550, 551, 552, 553, 554, 555, 556, 557, 558, 549,
560, 561, 562, 563, 564, 565, 566, 567, 568, 569,
570, 571, 572, 573, 574, 575, 576, 577, 578, 579,
580, 581, 582, 583, 584, 585, 586, 587, 588, 589,
590, 591, 592, 593, 594, 595, 596, 597, 598, 599,
};

#define MAX_ITEMTYPE_NUMBER		46	//��Ƽ��Ʈ �߰��ϸ鼭 ������ Ÿ���� �ϳ� �÷���36->37. ������ ī�װ��� �����ϸ鼭 42�� �ø� 37->42, �ƹ�Ÿ����Ÿ�� �߰��ϸ鼭 46�� �ø�. 42->46.
#define MAX_ITEM_NAME_LENGTH	60
#define MAX_TOOL_NUMBER			5

#define DRESS_ADD_DPOWER_RATE	1	// ������ �ִ� ���¿����� �Ǻ��� ������ 1/1��ŭ�� ����ȴ�.  ( �Ϻ��ǿ��� ���õǰ� �ִٴ¼��̴�)

#define MAX_ITEM_UNIQUE			30000

//---------------------------------
// ȿ������ ���� ������ �з� 
//---------------------------------
#define ITEMSOUND_BOOK					1
#define ITEMSOUND_BOW					2
#define ITEMSOUND_CLOTH					3
#define ITEMSOUND_FOOD					4
#define ITEMSOUND_HEAL					5
#define ITEMSOUND_HELMET				6
#define ITEMSOUND_SWORD					7
#define ITEMSOUND_MEAT					8
#define ITEMSOUND_NOODLE				9
#define ITEMSOUND_SOUP					10
#define ITEMSOUND_VEGETABLE				11
#define ITEMSOUND_AXE					12
#define ITEMSOUND_BELT					13
#define ITEMSOUND_ETC					14
#define ITEMSOUND_GUN					15
#define ITEMSOUND_NECK					16
#define ITEMSOUND_SHOES					17
#define ITEMSOUND_STAFF					18
#define ITEMSOUND_TICKET				19
#define ITEMSOUND_CERAMIC				20
#define ITEMSOUND_SMELLHEAL				21 // ������
#define ITEMSOUND_UNFREEZEHEAL			22 // ������

#define CANMAKE_ERROR_NOMATERIAL		1
#define CANMAKE_ERROR_NOTOOL			2
#define CANMAKE_ERROR_USE_PERSONSHOP	3	// ���λ���(�Ǹ�)�� �������� �ö󰡼� ������ �� �� ����
//KHY - 1211 - ���ݻ�ǰ - ���� ����
#define CANMAKE_USE_ALLPURPOSETOOL		4    //���� �������� ��� �ߴ�.



#define HEALEFT_NOFIRSTATTACK			1
#define HEALEFT_UNFREEZE				2
#define HEALEFT_PREMIUM					3	// ����ġ �����̾�
#define HEALEFT_COMBAT					4	// ���� �����̾�
#define HEALEFT_DIGESTIVE				5   // ��θ� ����
#define HEALEFT_ALLDESEASE				6   // ���� ����
#define HEALEFT_FAME					7	// ���� ����
#define HEALEFT_BULLET					8	// źȯ �����̾�
#define HEALEFT_HEROSTAMINA				9	// ���ΰ� ���¹̳� ����
#define HEALEFT_SUMMONSTAMINA			10	// ��ȯ�� ���¹̳� �����̾�
#define HEALEFT_NOFIRSTATTACK_PREMIUM	11	// �����̾� ���� ������
#define HEALEFT_RESURRECTION			12	// ��Ȱ ��
#define HEALEFT_SUMMONATACUP			13	// ��ȯ�� ���ݷ�,���� ���� 20% ��
#define HEALEFT_HEROATTACKUP			14	// ���ΰ� ���ݷ� ���� 20% ��
#define HEALEFT_HERODEFENSEUP			15	// ���ΰ� ���� ���� 20% ��
#define HEALEFT_LOVEPOWER				16	// ���ΰ� ����,���� ����ġ 50% ����, ��ȯ�� ���׹̳��� ȿ��, ��ȯ�� ���ݷ� 20% ����
#define HEALEFT_FRIENDSHIP				17	// ���ΰ� ����,���� ����ġ 20% ����, ���ݷ�, ���� 20% ����
#define HEALEFT_HORSEWINGS				18	// ���ɷ� ��� �̿��
#define HEALEFT_SUCCESS_MANUFACTURE		19	// ���� ���� ����
#define HEALEFT_MOFUMOFU_PERFUME		20	// ���ĸ��� ���
#define HEALEFT_FOURLEAF				21	// ���� Ŭ�ι�
#define HEALEFT_PREMIUMEXPUP			22	// Ư�� �ĳ��Ǽ�����
#define HEALEFT_ADDEXP					23	// ���
#define HEALEFT_MARKETPREMIUM			24	// ���� �����̾�
#define HEALEFT_HAPPYBEAN				25	// ���Ǻ� �̺�Ʈ ����Ÿ�� [2007.08.22 �ռ���].
#define HEALEFT_HEROATTACKUP30			26	// ������ ������ 30�ϰ� [2007.08 30 �ռ���]
#define HEALEFT_HERODEFENSEUP30			27	// ���� ������ 30�ϰ�[2007.08.30 �ռ���]
#define HEALEFT_SUPEREXPUP				28	// ���� EXP ĸ�� 7�� 30�� [2007. 09.18 �ռ���]
#define HEALEFT_BLACKCACAORANDOM		29	// KHY - 0115 �߷�Ÿ�ε��� �̺�Ʈ. - ����īī��.
#define HEALEFT_CANDYRANDOM				30	// KHY - 0218 ȭ��Ʈ���� �̺�Ʈ. - ȭ��Ʈ ĵ��.
#define HEALEFT_REWARDBONUS				31	// ���� - �̰� ���� ������ - ����, ���� ����ġ 200%, ������ ȹ�� 2��
#define HEALEFT_PREMIUMMAKEEXPUP		32	// ��� - ���� �����̾� ������ - 30�ϰ� ���� ����ġ 200% ���
#define HEALEFT_SPECIALAMPUL			33	// ���� - ����� ���� - 48�а� 300% ���� ����ġ ȹ��.
#define HEALEFT_QUEST_ANTIPOTION		34	// ���� - ����Ʈ ����� �Ƽ��󿡰� �޴� �������� 30%�A�´�
#define HEALEFT_ANTIDOTES				35	// ���� - �ص��� - ��� �ߵ� ���¿��� �ص��Ѵ�.
#define HEALEFT_TRANSFORM_SYNCRO_UP		36	// [����] ��ũ�� ������.
#define HEALEFT_QUEST_ATTACK			37  // [���] ����Ʈ ����� ��Ƽ��뿡�� 30%�� ���ݷ� ������ ���Ѵ�.
#define HEALEFT_SUMMONNOFIRSTATTACK		38	// [����] ��ȯ�� ���� ����
#define HEALEFT_RESIDENTS_EXPUP_10		39	// [����] ���� EXP ĸ�� - ���� ����ڰ� �Ҽӵ� ������ �ֹε鿡�� ����ġ 10% ���� ȿ��.
#define HEALEFT_RESIDENTS_STRONG		40  // [����] ���� ��ȭ�� - ���� ����ڰ� �Ҽӵ� ������ �ֹε鿡�� ����/���� ���ݷ�, ���� +10% ���� ȿ��.

#define MAX_SCROLL_EFFECT				500	// ������ �ִ� 50%������ ���� ����.

// ���� ���� ���� ����. 
#define ENCHANT_FAIL_REASON_NOMORE		1		// ������ ���̻� ���� �� ����. 
#define ENCHANT_FAIL_REASON_NOTATBMATCH	2		// Attribute�� �������� �ʴ� ���� 
#define ENCHANT_FAIL_REASON_NOMOREKIND  3		// ������ 2�����̻� ���ϼ� ����.

#define DUR_FOR_DISMANTLE	400		// ��ü�� �ʿ��� �ִ� ������(�� ��ġ ���Ͽ��� ��ü�� �� �� �ִ�. )  


//----------------------------------------
// ������ Ȥ�θ������� ����Ͽ� ������ �� �ִ� �ִ� ����. (�⺻ ������ �� ��� ��ŭ�� ����) 
//----------------------------------------
#define SCROLL_TUNE_MAX_RATE			2

//----------------------------------------
// ������ Ȥ�θ� ������ ����� ���� ���� ���� .
//----------------------------------------
#define SCROLLTUNE_ERR_MINDAMAGE		1
#define SCROLLTUNE_ERR_MAXDAMAGE		2
#define SCROLLTUNE_ERR_AC				3

//----------------------------------------
// �Ӽ� ��þƮ ���� ���� ���.
//----------------------------------------

const SI32 ENCHANT_SUCCESS		= 1;
const SI32 ENCHANT_FAIL			= 0;
const SI32 ENCHANT_DOWNGRADE	= -1;
const SI32 ENCHANT_BROKEN		= -2;

const SI32 ENCHANT_USEDAYPLUS	= 100;
const SI32 ENCHANT_USEDAYPLUS_ITEMUNIQUE		= ITEMUNIQUE(5400);
//----------------------------------------
// [����] ��þƮ �Ӽ� ���� ������
//----------------------------------------
const SI32 ENCHANT_CHANGE		= 110;
const SI32 ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE	= ITEMUNIQUE(5413);

//----------------------------------------
// [����] ��Ƽ��Ʈ ������ ���
//----------------------------------------
enum CRYSTAL_GRADE
{
	CRYSTAL_GRADE_NONE	= 0	//����� ����(ũ����Ż �������� �ƴ�)

	,CRYSTAL_GRADE_F
	,CRYSTAL_GRADE_E
	,CRYSTAL_GRADE_D
	,CRYSTAL_GRADE_C
	,CRYSTAL_GRADE_B
	,CRYSTAL_GRADE_A
	,CRYSTAL_GRADE_S1
	,CRYSTAL_GRADE_S2

	//---------------------------------------
	// ���ο� ����� ���� �߰��� �ּ���
	//---------------------------------------
	,CRYSTAL_GRADE_MAX
};


class cltKindItemProbabilitySet;
class cltPSkillInfo;
class cltItemPrice;
class cltNPCItemInfo;
class cltSkillManager;
class cltDiseaseManager;
class cltItemPriceUnit;
class cltBasicAbility;
class cltCharCommon;
class cltSummon;
class DBMaterialInfo;

class cltSetItemAbility;			//[����] ��Ʈ ������ �ɷ� ����.
class cltSetItemAbilityManager;		//[����] ��Ʈ ������ �ɷ� ���� �޴���.
class CSetItemManager;
class cltSkillBookManager;
// [����] ���� ���� ���ڿ��� �������� �ش� 
class cltRewardItemForLimitLevel;	// �߱����� ���� ������ 

//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ����ü Ÿ��.
class cltCrystalItemData{
public:
	SI32 siCrystalType;
	SI16 siMinValue;
	SI16 siMaxValue;
	SI32 siTypeRate;	

	void Set(cltCrystalItemData* pclcon)
	{
		if(pclcon == NULL)				return;

		siCrystalType		= pclcon->siCrystalType;
		siMinValue			= pclcon->siMinValue;
		siMaxValue			= pclcon->siMaxValue;
		siTypeRate			= pclcon->siTypeRate;
	}
};
////�ռ���� ������ ������
//class cltCompositionManterialItemData{
//public:
//	SI32 siCompositionManterialType;
//	SI32 siPara;
//	SI32 siTypeRate;	
//
//	void Set(cltCompositionManterialItemData* pclcon)
//	{
//		siCompositionManterialType			= pclcon->siCompositionManterialType;
//		siPara								= pclcon->siPara;
//		siTypeRate							= pclcon->siTypeRate;
//	}
//};

//--------------------------------------
// ������ ���� ����. // 2003.10.23
//--------------------------------------
class cltItemConition{
public:
	SI32 siLevel;
	SI32 siStr;
	SI32 siDex;
	SI32 siMag;
	SI32 siVit;
	SI32 siHnd;
	
	SI32 siSkillKind;			// ��� ��ų�� ����
	SI32 siSkillPoint;			// ��� ��ų�� ����Ʈ 
	
	bool bForMan;				// ������ ����. 
	bool bForWoman;				// ������ ����.
	SI16 siGrownUp;				// ���� ���� ������ - ���� ����ȿ��� ���
	
	SI16 siFameLevel;			// ������� ����. 

	SI16 siBaseSkillType;		// [����] �ƹ�Ÿ ����� : ���̽� ��ų Ÿ��


	cltItemConition()
	{
		siLevel = 0;
		siStr	= 0;
		siDex	= 0;
		siMag	= 0;
		siVit	= 0;
		siHnd	= 0;

		siSkillKind = 0;
		siSkillPoint = 0;

		bForMan		= true;
		bForWoman	= true;
		siGrownUp	= 0;

		siFameLevel	= 0;

		siBaseSkillType = 0;
	}

	cltItemConition(SI32 level, SI32 str, SI32 dex, SI32 mag, SI32 Vit, SI32 Hnd, SI32 SkillKind, SI32 SkillPoint, bool bforman, bool bforwoman, SI16 grownup ,SI16 famelevel, SI32 baseSkillType = 0 )
	{
		siLevel			= level;
		siStr			= str;
		siDex			= dex;
		siMag			= mag;
		siVit			= Vit;
		siHnd			= Hnd;
		siSkillKind		= SkillKind;
		siSkillPoint	= SkillPoint;

		bForMan			= bforman;
		bForWoman		= bforwoman;
		siGrownUp		= grownup;

		siFameLevel		= famelevel;

		siBaseSkillType = baseSkillType;		// [����] �ƹ�Ÿ ������� ���̽� ��ų Ÿ�� ����

	}

	void Set(cltItemConition* pclcon)
	{
		siLevel			= pclcon->siLevel;
		siStr			= pclcon->siStr;
		siDex			= pclcon->siDex;
		siMag			= pclcon->siMag;
		siVit			= pclcon->siVit;
		siHnd			= pclcon->siHnd;
		siSkillKind		= pclcon->siSkillKind;
		siSkillPoint	= pclcon->siSkillPoint;

		bForMan			= pclcon->bForMan;
		bForWoman		= pclcon->bForWoman;
		siGrownUp		= pclcon->siGrownUp;
		
		siFameLevel		= pclcon->siFameLevel;

		siBaseSkillType = pclcon->siBaseSkillType;

	}

	// ���ǿ� �����ϴ��� Ȯ���Ѵ�. 
	bool Check(SI32 itemtype, SI32 kind, SI32 level, 
		SI32 str, SI32 dex, SI32 mag, SI32 vit, SI32 hnd, SI32 skillpoint, SI32 famelevel, 
		SI32* returnval);
	
	// ������ ���ڿ��� ���´�. 
	bool GetText( TCHAR* text, SI16 txtSize, TCHAR *SkillName, SI32 SkillKind );
	
	// ���ǿ� �����ϴ��� Ȯ�ε� ���ڿ��� ���´�. ( ���� ��ũ��Ʈ ����( ex - <C05>  </C>) �� ���Եȴ� )
	bool GetCheckedText(  TCHAR* text, SI16 txtSize, TCHAR *SkillName, SI32 SkillKind, cltCharCommon * pclchar  );

};

//--------------------------------------
// ������ ���꿡 �ʿ��� ��ų ����. 
//--------------------------------------
class cltItemSkillInfo{
public:
	SI32 siSkill;			// �ʿ��� ���. 
	SI32 siSkillLevel;		// �ʿ��� ����� ���� ���� 
	SI32 siMakePbt;			// �⺻ ���� ���ɼ�(�۹�) 
	SI32 siIncPbt;			// ���� 1�� ���� ���� ���ɼ� (�۹�) 
	
	SI32 siHnd;				// ������ ���꿡 �ʿ��� ������ 
	SI32 siMag;				// ������ ���꿡 �ʿ��� ����. 
	

	cltItemSkillInfo()
	{
		siSkill			= 0;
		siSkillLevel	= 0;
		siMakePbt		= 0;
		siIncPbt		= 0;

		siHnd			= 0;
		siMag			= 0;
	}
	cltItemSkillInfo(SI32 skill, SI32 level, SI32 makepbt, SI32 incpbt, SI32 hnd, SI32 mag)
	{
		siSkill			= skill;
		siSkillLevel	= level;
		siMakePbt		= makepbt;
		siIncPbt		= incpbt;

		siHnd			= hnd;
		siMag			= mag;
	}
	void Set(cltItemSkillInfo* pclinfo)
	{
		siSkill			= pclinfo->siSkill;
		siSkillLevel	= pclinfo->siSkillLevel;
		siMakePbt		= pclinfo->siMakePbt;
		siIncPbt		= pclinfo->siIncPbt;
		siHnd			= pclinfo->siHnd;
		siMag			= pclinfo->siMag;
	}
};


//---------------------------------------
class cltSplitItemInfo{
public:
	UI16 uiFrom;		
	UI16 uiTo;
	SI32 siCount;		// ������ ����..

	cltSplitItemInfo()
	{
		ZeroMemory(this, sizeof(cltSplitItemInfo));
	}
	cltSplitItemInfo(UI16 uifrom, UI16 uito, SI32 sicount)
	{
		uiFrom		= uifrom;
		uiTo		= uito;
		siCount		= sicount;
	}
};

//---------------------------------------
// ������ �̵� ����. 
//---------------------------------------
class cltMoveItemInfo{
public:
	//From
	UI16 uiFrom;					// ��𿡼� �ű�°�.

	//To
	UI16 uiTo;						// ���� �ű�°� ?

	cltMoveItemInfo()
	{
		ZeroMemory(this, sizeof(cltMoveItemInfo));
	}

	cltMoveItemInfo(UI16 uifrom, UI16 uito)
	{
		uiFrom		= uifrom;
		uiTo		= uito;
	}

};


//-------------------------------------
// �������� ��� ���� 
//-------------------------------------
class cltItemUseInfo{
public:
	// ����� �������� ��ġ. 
	UI16 uiFrom;					// ��𿡼� �ű�°�.

	SI32 siItemUnique;				// ����� �������� ����ũ. 

	cltItemUseInfo()
	{
		ZeroMemory(this, sizeof(cltItemUseInfo));
	}

	cltItemUseInfo(UI16 uifrom, SI32 iteunique)
	{
		uiFrom			= uifrom;
		siItemUnique	= iteunique;
	}
};

//---------------------------------------
// ������ Ÿ�� ����. 
//---------------------------------------	
#define MAX_ITEMTYPE_NAME_LENGTH 128
class cltItemTypeInfo{
public:
	SI32 siUnique;
	SI64 siAtb;
	SI64 siAttach;
	TCHAR szName[MAX_ITEMTYPE_NAME_LENGTH];
	TCHAR szTypeCode[MAX_ITEMTYPE_NAME_LENGTH];

	cltItemTypeInfo(SI32 siunique, SI64 siatb, SI64 siattach, const TCHAR* typecode, const TCHAR* pname);
	~cltItemTypeInfo();

	BOOL IsAtb(SI64 atb)const;
	BOOL IsAttach(SI64 atc)const;

	const TCHAR* GetName()const{return szName;}

};


//---------------------------------------
// ������ ���� ����. 
//---------------------------------------
class cltItemproductionInfo{
public:
	SI32			siProductUnit;							// ������ ���� ����. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltMaterialInfo, MAX_MATERIAL_NUMBER>	clMaterial;
	NSafeTArray<cltToolInfo, MAX_TOOL_NUMBER>			clTool;
#else
	cltMaterialInfo clMaterial[MAX_MATERIAL_NUMBER];		//�������� ���. 
	cltToolInfo		clTool[MAX_TOOL_NUMBER];
#endif

	cltItemproductionInfo(){}

	bool AddMaterial(cltMaterialInfo* pclmaterial)
	{
		SI32 i;
		for(i = 0;i < MAX_MATERIAL_NUMBER;i++)
		{
			if(clMaterial[i].siUnique == 0)
			{
				clMaterial[i].Set(pclmaterial);
				return true;
			}
		}

		return false;
	}

	bool AddTool(cltToolInfo* pcltool)
	{
		SI32 i;

		for(i = 0;i < MAX_TOOL_NUMBER;i++)
		{
			if(clTool[i].siUnique == 0)
			{
				clTool[i].Set(pcltool);
				return true;
			}
		}

		return false;
	}

	SI32 GetToolCount()
	{
		SI32 siRet = 0;
		for(SI32 i = 0;i < MAX_TOOL_NUMBER;i++)
		{
			if(clTool[i].siUnique > 0)
			{
				siRet++;
			}
		}
		return siRet;
	}
};

//---------------------------------------
// ������ Unique�� ����. 
//---------------------------------------
#define ADAPT_MAN		1	// ������� ����Ǵ� ������. 
#define ADAPT_SUMMON	2	// ��ȯ������ ����Ǵ� ������. 


//--------------------------------------
// ItemInfoAtb
//--------------------------------------
#define ITEMINFOATB_FARMTOOL			 			0x1			// ���� ����. 
#define ITEMINFOATB_MINETOOL			 			0x2			// ������ ����. 
#define ITEMINFOATB_CANUSEITEM			 			0x4			// ��밡���� ��ǰ�ΰ�. 
#define ITEMINFOATB_SHOWLIST			 			0x8
#define ITEMINFOATB_TRADELIST			 			0x10		// ���� �ҽ����� �� ��
#define ITEMINFOATB_ITEMMALL			 			0x20		// �����۸����� �Ǹ��ϴ� ��ǰ�̴�. 
#define ITEMINFOATB_NOTDAILYQUEST		 			0x40		// ����(�𺴰�) ����Ʈ�� �ȳ����� ��ǰ
#define ITEMINFOATB_NOTWENNYOLDMAN		 			0x80		// 2ȣ��(Ȥ�θ�) �� �ȳ����� ��ǰ
#define ITEMINFOATB_NOTTRADE			 			0x100		// �ŷ��ȵǴ� ��ǰ( ������ ���� �� ���� )
#define CANUSEITEM_TYPE_EAST			 			0x200		// ����ĳ���� ����
#define CANUSEITEM_TYPE_WEST			 			0x400		// ����ĳ���� ����
#define ITEMINFOATB_TYPE_QUESTITEM		 			0x800		// ����Ʈ ������ 
#define ITEMINFOATB_FIRSTACTION			 			0x1000		// ������ ����ġ ������
#define ITEMINFOATB_SPECIAL_JAPAN		 			0x2000		// �Ϻ� Ư��ǰ
#define ITEMINFOATB_SPECIAL_GLOBAL		 			0x4000		// �۷ι� Ư��ǰ
#define ITEMINFOATB_SPECIAL_KOREA		 			0x8000		// �ѱ� Ư��ǰ
#define ITEMINFOATB_WENNYOLDMAN_MATERIAL 			0x10000		// 2ȣ��(��ȣ��, Ȥ�θ�) ���ؽ� ���� ��Ḧ ��ȯ���ִ� ������
#define ITEMINFOATB_NOTSELL				 			0x20000		// NPC���� �� �� ����
#define ITEMINFOATB_PARTYHALL_SYMBOL	 			0x40000		//[����] ��ƼȦ. ���ǰ ������. => 2008. 3. 12
#define ITEMINFOATB_PARTYHALL_PRESENT	 			0x80000		//[����] ��ƼȦ. ����.			=> 2008. 3. 12
#define ITEMINFOATB_CRYSTAL_REMOVE		 			0x100000	//[����] ��Ƽ��Ʈ ����. �и��� ����ü ITEMINFOATB_ => 2008-4-2  
#define ITEMINFOATB_CRYSTAL_SLOT_CREATE	 			0x200000	//[����] ��Ƽ��Ʈ ����. ������ ����ü ITEMINFOATB_ => 2008-4-2  
#define ITEMINFOATB_RARE_SUMMONDOLL		 			0x400000	//[����] ���� ��ȯ���� ����(���: 1.������ ��� ����.) => 2008-7-15
#define ITEMINFOATB_NOTSHOW_MARKETCONFITIONS		0x800000	// �ü�Ȯ�ο�  - ������ �������� �ü�Ȯ�ο��� ���ܵȴ�.
#define ITEMINFOATB_CRYSTAL_AVATAR					0x1000000	//[��] �ƹ�Ÿ ����ü //KHY - 20090526 - �ƹ�Ÿ ����ü.
#define ITEMINFOATB_SKILLBOOK						0x2000000	//[����] ��ų �ɷ��� �ο��� ������.

class cltItemInfo{
	public:
		SI32	siType;
		SI32	siImageIndex;				// ���â�� ǥ�õǴ� ū �̹����� �ε���.
		SI32	siSmallImageIndex;			// ������ ǥ�õǴ� ���� �̹����� �ε���. 
		SI32	siWeight;					// �������� ����. ���� ����ũ�� �������� ���԰� ��� ����. 
		SI32	siSoundType;				// ȿ���� ���ָ� ���� ������ ����. 
		
		SI32	siItemGrade;				

		SI32	siItemInfoAtb;	
		cltItemSkillInfo	clSkillInfo;	// ���꿡 �ʿ��� ��� ����. 
		cltItemConition		clCondition;	// ������ �ʿ��� ����. 

		SI32	siSetItemIndex;

		TCHAR szArmCode[24];				// ���� ������ �ڵ� ���ڿ� 

	private:
		TCHAR	szName[MAX_ITEM_NAME_LENGTH];
		BOOL	m_bDormancy;				// �޸���� ��������� ����
	public:

		SI32 siNeedRank;		// Ư�� �ź� �䱸 ����( ���� ��..) 

		SI32 siDecDur;			// ������ ���� ����. 

		SI32 siUpgradeSkillUnique;		// �� �������� ��� �����ִ� SKill �� unique ��
		SI32 siUpgradeSkillType;		// �� �������� ��� �����ִ� ����� Ÿ��. 

		SI32 siExplainUnique;			// ���� �ؽ�Ʈ ����ũ. 
		
		SI32 siAdapt;					// �������� ���� ���. 
		
		bool bCanEquipSwitch;			// ���� ������ ��ǰ�ΰ�. 	
		
		// ������� ��� 
		SI32 siAttackRange;
		SI32 siDefaultHitRate;			// �⺻ ���߷�(��ꤷ ���� ���߷��� �����ش�. 
		SI32 siAttackType;
		DWORD dwAttackInterval;			// ���ݰ� ���� ������ �ð�(ms)

		// ���ķ��� ��� 
		SI32 siHungry;

		SI32 siHorseFoodSwitch;		// �� ����

		bool bFishingBaitSwitch;	// ���� �̳�
		SI32 siOrg1Penalty;			// Org1(��ȭ���) ���Ƽ. 
		SI32 siOrg2Penalty;			// Org2(��ȯ���) ���Ƽ. 
		SI32 siOrg3Penalty;			// Org3(ȣ�����) ���Ƽ. 
		SI32 siOrg4Penalty;			// Org4(�Ű���) ���Ƽ. 

		// ��ǰ���� ���, 
		SI32 siLife;
		SI32 siMana;
		SI32 siDiseaseUnique;		// ���� �ڵ�. 
		TCHAR szDiseaseName[128];	// ġ���� �� �ִ� ������ �̸�. 
		TCHAR szDiseaseType[128];	// ġ���� �� �ִ� ���� Ÿ���� �̸�. 
		SI32 siHealEft;				// ���� ȿ��. 

		// å���� ���, 
		SI32 siSkillUnique;
		SI32 siSkillLevelLimit;

		//---------------------
		// ������ ��� 
		//---------------------
		SI32 siScrollAdaptAttach;	// � �Ӽ��� ��ǰ�� ����Ǵ°�?
		SI32 siScrollFunction;		// ������ ���. 
		SI32 siScrollAbility;		// �ֹ��� �������. 
		SI32 siScrollDecDuration;	// �Ҹ� ������. 
		SI32 siScrollMax;			// �ɷ��� �ִ� ����. 	
		
		//--------------------
		// ��ȯ ������ ���, 
		//--------------------
		SI32 siMakeKind;			// �� ��ȯ�� ������ ����ϸ� � ���� ��ȯ�Ǵ°�. 

		//-------------------
		// ������ ��� 
		//-------------------
		SI32 BagUseItemType;		// ���濡 ���� �������� ����. 
		

		//-------------------
		// ��� ���, 
		//-------------------
		SI32 siFishClassification;

		//-------------------------------------------------------------------
		// ���� ������ �ö󰡳� �� �ö󰡳�,�ö󰣴ٸ� �ּ� �������� ����
		//-------------------------------------------------------------------
		SI32 siTradeServerMinNum; // 0�̸� �� �ö󰡴� �Ŵ�

		//-------------------------------------------------------------------
		// ������ ��볯¥�� �����ϰ� ���� �ʴ°��� true
		//-------------------------------------------------------------------
		bool bImmutableUseDate;

		//-------------------------------------------------------------------
		//[����] ���ո��Ͽ� �ö󰡳�.
		//-------------------------------------------------------------------
		bool m_bNewMarket; // 0�̸� �� �ö󰡴� �Ŵ�

		//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. 
#ifdef _SAFE_MEMORY
		NSafeTArray<cltCrystalItemData, MAX_CAN_CRYSTALABILITY_NUM>		CrystalData;
#else
		cltCrystalItemData CrystalData[MAX_CAN_CRYSTALABILITY_NUM];
#endif

	public:
		TCHAR*			pszMaterialText; 
		TCHAR*			pszToolText;

		cltItemproductionInfo	clProductionInfo;	// ������ ���� ����. 

		cltItem clItem;

		cltItemInfo(SI32 imageindex, SI32 smallimageindex, TCHAR* name, SI32 sitype, SI32 weight,
					cltItemSkillInfo* pclskillinfo,cltItemConition* pclcondition, TCHAR* armcode, TCHAR* material,
					TCHAR* tool, SI32 productunit, SI32 soundtype, cltItem* pclItem ,SI32 Grade, SI32 TradeServerMinNum, cltCrystalItemData* CrystalItemData, bool bNewMarket, bool ImmutableUseDate = false);


		~cltItemInfo();

		const TCHAR* GetName()const{return szName;}
		void SetName(const TCHAR* pname)
		{
			MStrCpy(szName, pname, MAX_ITEM_NAME_LENGTH );
		}

		TCHAR* GetArmCode();

		// ��Ḧ �м��Ѵ�. 
		bool AnalyzeMaterial(cltItemManagerCommon* pclItemManager);
			//������ �м��Ѵ�. 
		bool AnalyzeTool(cltItemManagerCommon* pclItemManager);

		// ����� �ٰŷ� ��ǰ�� ����� �ִ°� ?
		bool CanMake_Skill(cltPSkillInfo* pclskill, cltPItemInfo* pclcharitem, cltBasicAbility* pclBA);
		bool IsSkill_Maked();

		// �־���  ��Ḧ �ٰŷ� ��ǰ�� ������ �� �ִ°�?
		bool CanMake(SI32 materialnum,  cltItem* pclmaterialitem, cltItem* pclitem, SI32* producnum, SI32* preturnval, SI32* preturnpara1, bool bonlymaterialcheckswitch, BOOL bTutorialMake = FALSE,BOOL bUseQuestItem = FALSE ,cltMaterialInfo* clTutorialMaterial = NULL);

		// �־���  ��Ḧ �ٰŷ� ��ǰ�� ������ �� �ִ°�? ��ġ�� Ȯ���Ѵ�.
		bool CanMakeForServer(SI32 materialnum,  cltItem* pclmaterialitem, cltItem* pclitem, SI32* producnum, SI32* preturnval, SI32* preturnpara1, bool bonlymaterialcheckswitch, SI32* materialindex  , cltItem* pPersonShopItem, bool Check, BOOL bTutorialMake = FALSE,BOOL bUseQuestItem = FALSE ,cltMaterialInfo* clTutorialMaterial = NULL);


		const TCHAR* GetArmCode()const{return szArmCode;}

		// ���� ���� Ȯ���� ���Ѵ�. 
		float GetTotalMakePbt(SI32 skilllevel);

		// Ư�� �Ӽ��� ���������� �Ӽ��ΰ�. 
		bool IsItemInfoAtb(SI32 atb)
		{
			if(siItemInfoAtb & atb)return true;
			return false;
		}
		void DelItemInfoAtb(SI32 atb)
		{
			if(siItemInfoAtb & atb)
				siItemInfoAtb -= atb;
		}
		void AddItemInfoAtb(SI32 atb)
		{
			siItemInfoAtb |= atb;
		}
		
		bool IsThereMaterialAndTool(SI32 siUnique, SI32 *siCount)
		{
			
			for(SI32 i = 0; i < MAX_MATERIAL_NUMBER;i++)
			{
				SI32 unique = clProductionInfo.clMaterial[i].siUnique;
				SI32 num	= clProductionInfo.clMaterial[i].siNeedNum;
				
				if(unique <= 0)continue;
				if(num    <= 0)continue;

				if(siUnique == unique)
				{
					*siCount = num;
					return true;
				}
			}

			for(SI32 i = 0; i < MAX_TOOL_NUMBER;i++)
			{
				SI32 unique = clProductionInfo.clTool[i].siUnique;
			
				if(unique <= 0)continue;
				if(siUnique == unique)
				{
					*siCount = 0;
					return true;
				}
			}

			return false;
		}

		void SetDormancy(BOOL bDormancy)
		{
			m_bDormancy = bDormancy;
		}
		BOOL IsDormancy()
		{
			return m_bDormancy;
		}


		//------------------------------------------
		// ������� ���, 
		//------------------------------------------
		void SetAttackRange(SI32 range){siAttackRange = range;}
		SI32 GetAttackRange()const{return siAttackRange;}

		void SetAttackInterval(DWORD interval){dwAttackInterval = interval;}
		DWORD GetAttackInterval(){return dwAttackInterval;}

		void SetAttackType(SI32 type){siAttackType = type;}
		SI32 GetAttackType()const{return siAttackType;}

		//------------------------------------------
		// ���ķ��� ���, 
		//------------------------------------------
		void SetHungry(SI32 amount){siHungry = amount;}
		SI32 GetHungry()const{return siHungry;}

		//-----------------------------------------
		// ��ǰ���� ���, 
		//-----------------------------------------
		void SetLife(SI32 life){siLife = life;}
		SI32 GetLife()const{return siLife;}
		void SetDiseaseName(TCHAR* pname){StringCchCopy(szDiseaseName, 128, pname);}
		void SetDiseaseType(TCHAR* pDiseaseType){StringCchCopy(szDiseaseType, 128, pDiseaseType);}
		SI32 GetDisease()const{ return siDiseaseUnique;}

		void SetMana(SI32 mana){siMana = mana;}
		SI32 GetMana()const{return siMana;}

		//--------------------------------------
		// å���� ���, 
		//--------------------------------------
		void SetSkillUnique(SI32 unique){siSkillUnique = unique;}
		SI32 GetSkillUnique()const{return siSkillUnique;}
		void SetSkillLevelLimit(SI32 limit){siSkillLevelLimit = limit;}
		SI32 GetSkillLevelLimit()const{return siSkillLevelLimit ;}

		//--------------------------------------
		// ������ ���, 
		//--------------------------------------
		void SetScrollAdaptAttach(SI32 atc){siScrollAdaptAttach = atc;}
		void SetScrollFunction(SI32 function){siScrollFunction = function;}
		void SetScrollAbility(SI32 ability){siScrollAbility = ability;}
		void SetScrollDecDuration(SI32 dur){siScrollDecDuration = dur;}

		//--------------------------------------
		// ��Ʈ ������ ó��
		//--------------------------------------
		void SetSetItemIndex( SI32 SetItemIndex ) {	siSetItemIndex = SetItemIndex; }
		SI16 GetSetItemIndex() { return siSetItemIndex; }

		bool IsImmutableUseDate()
		{
			return bImmutableUseDate;
		}


};

// ��ǰ ��� Ŭ����. 
class cltItemStatistic{
public:

	GMONEY			siItemPriceIndex;		// ��������. 
	SI32			siNPCPriceRate;			// NPC�� �ŷ� �� ����. 
	bool 		bLastNPCPriceRateDaySwitch;

#ifdef _SAFE_MEMORY
	// ������ �ŷ��� ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemTradeStatistic;
	// ������ ��뷮 ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemUseStatistic;
	// ������ ���� ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemMakeStatistic;

	// ������ ����(��ȣ��) ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemTuneStatistic;
	// ������ ����(��ȣ��)���� ���� ������ ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemFromTuneStatistic;

	// ������ ����ǰ ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemMonStatistic;
	// ������ ��� ���. 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemEquipStatistic;
	// ����Ʈ ���� ��ǰ ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemQuestStatistic;
	// �����۸� ���� ��ǰ ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemMallStatistic;
	// ������� ���� ������ ��ǰ ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemProductionStatistic;
	// �������� ������ ��ǰ ���� ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemWorldTradeIncomeStatistic;
	// �������� ������ ��ǰ ���� ��� 
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siItemWorldTradeExportStatistic;
	// ������ ���ο� ���� �ŷ�
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siForeignMerchantStatistics;
	// å���ڿ��� ���� ������ ���
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siBookBoxStatistics;

	//[����] ������ ���
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siElixirStatistics;
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siMagicalNostrumStatistics;

	//[����] ������ ���, ������ �ŷ�, ������ �ü�.
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siNewMarketItemTradeStatistic;
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siNewMarketItemOrderStatistic;
	NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>		siNewMarketItemRateStatistic;
#else
	// ������ �ŷ��� ��� 
	SI32			siItemTradeStatistic[MAX_ITEMINFO_NUMBER];
	// ������ ��뷮 ��� 
	SI32			siItemUseStatistic[MAX_ITEMINFO_NUMBER];
	// ������ ���� ��� 
	SI32			siItemMakeStatistic[MAX_ITEMINFO_NUMBER];

	// ������ ����(��ȣ��) ��� 
	SI32			siItemTuneStatistic[MAX_ITEMINFO_NUMBER];
	// ������ ����(��ȣ��)���� ���� ������ ��� 
	SI32			siItemFromTuneStatistic[MAX_ITEMINFO_NUMBER];

	// ������ ����ǰ ��� 
	SI32			siItemMonStatistic[MAX_ITEMINFO_NUMBER];
	// ������ ��� ���. 
	SI32			siItemEquipStatistic[MAX_ITEMINFO_NUMBER];
	// ����Ʈ ���� ��ǰ ��� 
	SI32			siItemQuestStatistic[MAX_ITEMINFO_NUMBER];
	// �����۸� ���� ��ǰ ��� 
	SI32			siItemMallStatistic[MAX_ITEMINFO_NUMBER];
	// ������� ���� ������ ��ǰ ��� 
	SI32			siItemProductionStatistic[MAX_ITEMINFO_NUMBER];
	// �������� ������ ��ǰ ���� ��� 
	SI32			siItemWorldTradeIncomeStatistic[MAX_ITEMINFO_NUMBER];
	// �������� ������ ��ǰ ���� ��� 
	SI32			siItemWorldTradeExportStatistic[MAX_ITEMINFO_NUMBER];
	// ������ ���ο� ���� �ŷ�
	SI32			siForeignMerchantStatistics[MAX_ITEMINFO_NUMBER];
	// å���ڿ��� ���� ������ ���
	SI32			siBookBoxStatistics[MAX_ITEMINFO_NUMBER];

	// ������
	SI32			siElixirStatistics[MAX_ITEMINFO_NUMBER];
	SI32			siMagicalNostrumStatistics[MAX_ITEMINFO_NUMBER];

	//[����] ������ ���, ������ �ŷ�, ������ �ü�.
	SI32			siNewMarketItemTradeStatistic[MAX_ITEMINFO_NUMBER];
	SI32			siNewMarketItemOrderStatistic[MAX_ITEMINFO_NUMBER];
	SI32			siNewMarketItemRateStatistic[MAX_ITEMINFO_NUMBER];

#endif
	cltItemStatistic()
	{
		siItemPriceIndex	= 0;
		siNPCPriceRate		= 0;

		bLastNPCPriceRateDaySwitch = false;
			
		InitStatistic();
	}

	~cltItemStatistic()
	{
	}
	
    void InitStatistic() // ���� �����͸� �ʱ�ȭ �Ѵ�.
	{
		// ������ ��� 
		for(SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{
			siItemTradeStatistic[i] = 0;
			siItemUseStatistic[i]	= 0;
			siItemMakeStatistic[i]	= 0;
			siItemTuneStatistic[i]	= 0;
			siItemFromTuneStatistic[i]	= 0;
			siItemMonStatistic[i]	= 0;
			siItemEquipStatistic[i]	= 0;
			siItemQuestStatistic[i]	= 0;
			siItemMallStatistic[i]	= 0;
			siItemProductionStatistic[i] = 0;
			siItemWorldTradeIncomeStatistic[i] = 0;
			siItemWorldTradeExportStatistic[i] = 0;
			siForeignMerchantStatistics[i] = 0;
			siBookBoxStatistics[i] = 0;

		}
	}

};
class clMarketConditionsBasic{
public:
	SI16 		siItemUnique;					// �ƿ��� ����ũ.
	GMONEY		siItemMarketConditions;		// �ü�.

	clMarketConditionsBasic()
	{
		ZeroMemory(this, sizeof(clMarketConditionsBasic));
	}

	~clMarketConditionsBasic()
	{
	}	
};

class clItemMarketConditions{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<clMarketConditionsBasic * , MAX_ITEM_UNIQUE> pclMarketConditionsB;
#else
	clMarketConditionsBasic * pclMarketConditionsB[MAX_ITEM_UNIQUE];
#endif

	_SYSTEMTIME		sTimeMarketConditions;

	clItemMarketConditions()
	{
		ZeroMemory(this, sizeof(clItemMarketConditions));
	}

	~clItemMarketConditions()
	{
	}
	

};




// ��æƮ ��� Ŭ����. 
//MAX_ITEMGRADE
#ifdef MAKE_BY_MUNIN //2007-05-16, 06-05
class cltItemEnchantStatistic{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siGradeSucess;
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siGradeFail;
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siGradeDownGrade;
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siGradeBroken;

	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siEnchantMaterial_EM_F; //Fire Stone
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siEnchantMaterial_EM_WT; //Water Stone
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siEnchantMaterial_EM_WD; //Wind Stone
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siEnchantMaterial_EM_L;//Land Stone
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, MAX_ITEMGRADE>		siEnchantMaterial_BS; //Blessing Stone
#else
	SI32			siGradeSucess[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER];
	SI32			siGradeFail[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER];
	SI32			siGradeDownGrade[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER];
	SI32			siGradeBroken[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER];
	
	SI32			siEnchantMaterial_EM_F[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER]; //Fire Stone
	SI32			siEnchantMaterial_EM_WT[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER]; //Water Stone
	SI32			siEnchantMaterial_EM_WD[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER]; //Wind Stone
	SI32			siEnchantMaterial_EM_L[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER];//Land Stone
	SI32			siEnchantMaterial_BS[MAX_ITEMGRADE][MAX_ITEMINFO_NUMBER]; //Blessing Stone
#endif		
	

	cltItemEnchantStatistic()
	{
		InitEnchantStatistic();
	}

	void InitEnchantStatistic()
	{
		for(SI32 j = 0 ; j < MAX_ITEMGRADE; j++ )
		{
			// ��æƮ ������ ��� 
			for(SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++)
			{
				siGradeSucess[j][i] = 0;
				siGradeFail[j][i] = 0;	
				siGradeDownGrade[j][i] = 0;
				siGradeBroken[j][i] = 0;

				siEnchantMaterial_EM_F[j][i] = 0;
				siEnchantMaterial_EM_WT[j][i] = 0;
				siEnchantMaterial_EM_WD[j][i] = 0;
				siEnchantMaterial_EM_L[j][i] = 0;
				siEnchantMaterial_BS[j][i] = 0;				
			}
		}
	}

	~cltItemEnchantStatistic()
	{
	}
};
#else

class cltItemEnchantStatistic{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, 7>		siGradeSucess;
	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, 7>		siGradeFail;

	NSafeTArray< NSafeTArray<SI32, MAX_ITEMINFO_NUMBER>, 5>		siEnchantMaterial;
#else
	SI32			siGradeSucess[7][MAX_ITEMINFO_NUMBER];
	SI32			siGradeFail[7][MAX_ITEMINFO_NUMBER];

    SI32			siEnchantMaterial[5][MAX_ITEMINFO_NUMBER];
#endif

	cltItemEnchantStatistic()
	{
		for(SI32 j = 0 ; j < 7; j++ )
		{
			// ��æƮ ������ ��� 
			for(SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++)
			{
				siGradeSucess[j][i] = 0;
				siGradeFail[j][i] = 0;	
				
				if( j < 5 )
					siEnchantMaterial[j][i] = 0;

			}
		}
	}

	~cltItemEnchantStatistic()
	{
	}
};
#endif


//-------------------------------------
// ������ ������. 
//-------------------------------------
#define RARE1_ITEM_RATE	200		// ȣǰ
#define RARE2_ITEM_RATE	260		// ��ǰ
#define RARE3_ITEM_RATE	150		// ����ǰ

class cltItemManagerCommon
{
public:
	// ���� Ŭ���̾�Ʈ ���� �κ� 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItemTypeInfo*, MAX_ITEMTYPE_NUMBER>	pclItemTypeInfo;
	NSafeTArray<cltItemInfo*, MAX_ITEMINFO_NUMBER>		pclItemInfo;
#else
	cltItemTypeInfo* pclItemTypeInfo[MAX_ITEMTYPE_NUMBER];
	cltItemInfo* pclItemInfo[MAX_ITEMINFO_NUMBER];
#endif
	cltItemPrice* pclItemPrice;
	cltItemPrice* pclItemNewPrice;
	
	//[����] ��Ʈ �������� �ɷ�ġ ����.
	cltSetItemAbilityManager*	pcltSetItemAbilityManager;
	CSetItemManager*			pclSetItemManager;

	cltSkillBookManager*		pclSkillBookManager;
	// [����] ���� ���� ���ڿ��� �������� �ش� 
	cltRewardItemForLimitLevel*	pclRewardItemLimitLevelForChina	;	// �߱����� ���� ������

	// ��ǰ ��� ����. 
	cltItemStatistic clItemStatistic;
	bool m_bcheckcnppricerateswitch;	// ������ �ü��� �ѹ��� �����ȴ�. 

	cltItemEnchantStatistic clItemEnchantStatistic;

	// [��] �ü�Ȯ�� 2008-07-18 - 8��ġ�� �����Ѵ�.
#ifdef _SAFE_MEMORY
	NSafeTArray<clItemMarketConditions  *, 9> clMarketConditions;
#else
	clItemMarketConditions * clMarketConditions[9];
#endif

	NDataAtbParser  ItemInfoAtbParser;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_ITEM_UNIQUE>		ItemRef;
#else
	// ItemUnique�� �ٰŷ� ����� Ref�� ã�� ���� ����. 
	SI16	ItemRef[MAX_ITEM_UNIQUE];
#endif
	
	// �������� ����. (�� ������ �������� ���ӿ� �ִ°� ?) 
	SI32	siItemNum;		
	
	bool bCanMakeRareItemSwitch;	// ��ǰ�� ���� �� �ִ� ����ΰ�. 
	DWORD dwLastMakeRareItemClock;	// ���������� ��ǰ�� ���� �ð�. 

	//---------------------------------
	// ���ڻ�㰡�� ����
	//---------------------------------
#ifdef _SAFE_MEMORY
	NSafeTArray<InvestmentLines, MAX_INVESTMENT_ITEM_NUMBER>	stInvestmentLines;
#else
	InvestmentLines stInvestmentLines[MAX_INVESTMENT_ITEM_NUMBER];
#endif

	//-----------------------------
	// TradeServer
	//-------------------------------
//	SI32		siTradeServerObejectRank[MAX_TRADESERVER_OBJECT_RANK]; // ������ ������ ����� ��


	//KHY - NPCRATE Ȯ�� - �ӽ� �ڵ�
	SI32 ReadLastNPCRATE();
	void WriteLastNPCRATE(SI32 nowLastNpcrate);
public:

	virtual BOOL IsDialogMessage(MSG* pmsg){return FALSE;}
	virtual bool MakeRandItem(SI32 level, SI32 addrate, cltKindItemProbabilitySet* pclikinditem, cltItem* pclitem){return false;}
	virtual SI32 GetBitmapIndexFromBitmapName(TCHAR* bitmapname){return 0;}
	virtual BOOL GetExplain(cltItem* pclitem, TCHAR* ptext, bool numswitch = true, bool basicswitch = false, bool bcolorswitch = false, SI32 siKind = 0, bool bweightswitch = true ){return FALSE;}
	virtual BOOL GetProductionExplain(SI32 siunique, cltPSkillInfo* pclskillinfo, cltPItemInfo* pclcharitem, TCHAR* ptext){return FALSE;}
	virtual void SetItemImageBigList(){};
	virtual void SetBitmapImage(){};
	virtual TCHAR* GetBitMapImageFileNamePointer(SI32 index){return NULL;}
	virtual HBITMAP MakeDDBFromDIB(HDC hdc, SI32 imageindex){return NULL;}
	virtual HBITMAP MakeDDBFromDIB(HDC hdc, TCHAR* path){return NULL;}
	virtual SI32 GetBitmapIndexFromFileName(const TCHAR* filename){return -1;}
	virtual void PushEffect(SI32 effect){}
	virtual void PlaySound(SI32 itemunique, SI32 type){};
    
	cltItemManagerCommon();
	virtual ~cltItemManagerCommon();

	void Create();

	BOOL SetItemType(SI32 siunique, SI64 siatb, SI64 siattach, const TCHAR* typecode, const TCHAR* pname);

	//-----------------------------------------------
	// cltItem-SetItemData.cpp
	//------------------------------------------------
	void SetItemData();
	// �������� �����ϱ� ���� ���� �������� �����Ѵ�. 
	void SetItemMaterial();

	void SetItemType();

	// ���� �������� �߰��Ѵ�. 
	void AddFoodItem(TCHAR* datfilename);
	
	// ���깰�������� �߰��Ѵ�. 
	void AddSeaFoodItem(TCHAR* datfilename);


	// �� �������� �߰��Ѵ�. 
	void AddHealItem(TCHAR* datfilename);
	// Į �������� �߰��Ѵ�. 
	void AddSwordItem(TCHAR* datfilename);
	// â �������� �߰��Ѵ�. 
	void AddSpearItem(TCHAR* datfilename);
	// ������ �������� �߰��Ѵ�. 
	void AddStaffItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddAxeItem(TCHAR* datfilename);
	// Ȱ �������� �߰��Ѵ�. 
	void AddBowItem(TCHAR* datfilename);
	// chd �������� �߰��Ѵ�. 
	void AddGunItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddArmourItem(TCHAR* datfilename);
	// �Ǻ� �������� �߰��Ѵ�. 
	void AddDressItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddHelmetItem(TCHAR* datfilename);
	// ��� �������� �߰��Ѵ�. 
	void AddBeltItem(TCHAR* datfilename);
	// ����� �������� �߰��Ѵ�. 
	void AddNeckItem(TCHAR* datfilename);
	// �Ź� �������� �߰��Ѵ�. 
	void AddShoesItem(TCHAR* datfilename);
	// å �������� �߰��Ѵ�. 
	void AddBookItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddAnimalItem(TCHAR* datfilename);
	// ��Ÿ �������� �߰��Ѵ�. 
	void AddEtcItem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	// �Ϲ� �������� �߰��Ѵ�. 
	void AddItem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	// ���� �������� �߰��Ѵ�. 
	void AddRingItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddScrollItem(TCHAR* datfilename);
	// ö��  �������� �߰��Ѵ�. 
	void AddIronItem(TCHAR* datfilename);
	// ����  �������� �߰��Ѵ�. 
	void AddDishItem(TCHAR* datfilename);
	// �  �������� �߰��Ѵ�. 
	void AddFishingRodItem(TCHAR* datfilename);
	// ��ȯ���� �������� �߰��Ѵ�. 
	void AddDollItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddHatItem(TCHAR* datfilename);
	// ǥ�������� �߰��Ѵ�. 
	void AddTicketItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�.
	void AddBagItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�.
	void AddMantleItem(TCHAR* datfilename);

	// [����] �ƹ�Ÿ ���� �������� �߰��Ѵ�.
	void AddAvatarMantleItem(TCHAR* datfilename);
	// [����] �ƹ�Ÿ �Ǻ� �������� �߰��Ѵ�.
	void AddAvatarDressItem(TCHAR* datfilename);
	// [����] �ƹ�Ÿ ���� �������� �߰��Ѵ�.
	void AddAvatarHatItem(TCHAR* datfilename);
	// [����] �ƹ�Ÿ ����� �������� �߰��Ѵ�.
	void AddAvatarNeckItem(TCHAR* datfilename);
	// [����] �ƹ�Ÿ ���� �������� �߰��Ѵ�.
	void AddAvatarRingItem(TCHAR* datfilename);

	// ���� �������� �߰��Ѵ�. 
	void AddPaperItem(TCHAR* datfilename);
	// ���� �������� �߰��Ѵ�. 
	void AddCannonItem(TCHAR* datfilename);
	// ��� �������� �߰��Ѵ�. 
	void AddTorpedoItem(TCHAR* datfilename);
	// ����ü �������� �߰��Ѵ�. 
	void AddCrystalItem(TCHAR* datfilename);
	//�ռ� ��� �������� �߰��Ѵ�
	void AddCompositionManterial(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//�̺�Ʈ �������� �߰��Ѵ�
	void AddEventitem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//���� �������� �߰��Ѵ�
	void AddBox(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//�����̾� �������� �߰��Ѵ�
	void AddPremium(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//���� �������� �߰��Ѵ�
	void AddMineralItem(TCHAR* datfilename);
    	
	void AddQuestItem(TCHAR* datfilename);
	//----------------------------------------
	// TradeServer
	//---------------------------------------
	void AddTradeServerRank(TCHAR *filename);
	SI16 GetTradeServerObjectRank(cltItem*pclitem); // �� ��ǰ�� ����� ���Ѵ�.
	BOOL MakeTradeServerObjectForRank(cltItem*pclitem,SI16 unique,SI16 num ,SI16 rank,SI08 rare,SI16 type,SI16 power); // ������ ���� �������� �����.
	
	// ������(��)�� ġ���� �� �ִ� ������ �����Ѵ�. 
	void ConnectDisease( cltDiseaseManager *pcldiseasemanager);

	// ������(��)�� ġ���� �� �ִ� ����Ÿ���� �����Ѵ�. 
	void ConnectDiseaseType( cltDiseaseManager *pcldiseasemanager);

	SI32 FindItemRefFromUnique(SI32 unique)
	{
		if(unique < 0)return 0;
		if(unique >= MAX_ITEM_UNIQUE)return 0;
		
		return ItemRef[unique];
	}

//	bool GetLimitDateItemValue(SI32 unique , SI32* usedate) ;

	// �������� ����ũ�� �ٰŷ� �������� ���� �����͸� ���Ѵ�.
	cltItemInfo* FindItemInfo(SI32 unique)
	{
		if(unique < 0)return 0;
		if(unique >= MAX_ITEM_UNIQUE)return 0;

		SI32 ref = FindItemRefFromUnique(unique);
		if(ref <= 0)return NULL;

		return pclItemInfo[ref];

	}

	// �������� �̸����� �������� ����ũ ���� ���Ѵ�. 
	SI32 FindUniqueFromName(const TCHAR* pname);

	SI08 FindMakeAniTypeFromUnique(SI32 siUnique);
	SI08 FindEnchantAniTypeFromUnique(SI32 siUnique);

	SI32 FindRefFromStaticIndex(SI32*  pindex)
	{
		if(*pindex < 0)
		{
			*pindex	= 0;
			return 0;
		}
		if(*pindex >= MAX_ITEMINFO_NUMBER)
		{
			*pindex	= 0;
			return 0;
		}

		for(SI32 i = *pindex; i < MAX_ITEMINFO_NUMBER;i++)
		{
			if(pclItemInfo[i])
			{
				*pindex	= (i+1);
				return i;
			}
		}

		*pindex = 0;
		return 0;
	}

	// �������� Ÿ���̸��� �ٰŷ� ������Ÿ���� ����ũ���� ���Ѵ�.
	SI32 FindItemTypeUniqueFromTypeName(const TCHAR* itemtypename);
	// �������� Ÿ���ڵ带  �ٰŷ� ������Ÿ���� ����ũ���� ���Ѵ�.
	SI32 FindItemTypeUniqueFromTypeCode(const TCHAR* itemtypeCode);
	bool IsCanDropItem( SI32 siItemUniuqe);


	SI32 AddItem(SI32 imageindex, SI32 smallimageindex, TCHAR* name, SI32 sitype, SI32 weight,
				 cltItemSkillInfo* pclskillinfo, cltItemConition* pclcondition,
				 TCHAR* armcode, TCHAR* material, TCHAR* tool, SI32 productunit,
				 SI32 soundtype, cltItem* pclitem , SI32 Grade, SI32 MinNum, cltCrystalItemData *CrystalItemData, bool bSuperMaket, bool ImmutableUseDate = false);

	// �������� ������ ��ġ���� Ȯ���Ѵ�. 
	bool CheckProperItemPos(cltItem* pclItem, SI32 siDragFrom, SI32 siKind = 0);

	// siUnique�������� ���� ��Ÿ��� ���Ѵ�. 
	SI32 GetAttackRange(SI32 siUnique);

	// �������� �ü��� �����Ѵ�. 
	void SetCurrentPrice(SI32 itemunique, GMONEY price, SI32 itemnum, bool bforceswitch = false);
	void SetCurrentNewPrice( SI32 itemunique, SI64 price, SI32 itemnum, bool bforceswitch = false );
	
	// �������� �ü��� ���´�.
	cltItemPriceUnit* GetCurrentPrice(SI32 itemnunique);
	cltItemPriceUnit* GetCurrentNewPrice(SI32 itemnunique);
	
	SI32 GetItemGrade(SI32 ItemUnique);

	// Unique�� �ٰŷ� Atb�� ���Ѵ�. 
	SI64 GetItemAtb(SI32 unique);
	
	//����ũ�� �ٰŷ� Ÿ���� ���Ѵ�. 
	SI32 GetItemType(SI32 unique);

	// Unique�� �ٰŷ� Attach�� ���Ѵ�. 
	SI64 GetItemAttach(SI32 unique);
	
	// �ش� Scroll �� �����ۿ� ���ϼ� �ִ��� Ȯ���Ѵ�.
	BOOL CanItemAttach(SI32 scrollunique, SI32 tounique);

	// unique�� �ٰŷ� �̸��� ���Ѵ�. 
	const TCHAR* GetName(SI32 unique);
	
	// �̹����� �ε����� �����´�. 
	SI32 GetImage(SI32 siunique);

	// ������ ���� ������ ������Ű���� Ȯ���Ѵ�. (����ڿ�) 
	bool CheckCondition(SI32 siitemunique, cltCharCommon* pclchar, SI32* returnval);
	// ������ ���� ������ ������Ű���� Ȯ���Ѵ�. (��ȯ����) 
	//bool CheckCondition(SI32 siitemunique, cltSummon* pclsummon, SI32* returnval,SI32 addstr , SI32 adddex,SI32 addmag);
	// ��ȯ�� ������ ���� ������ ������Ű���� Ȯ���Ѵ�
	bool CheckConditionForSummon( IN cltCharCommon* pclChar, IN SI32 siItemUnique, IN cltSummon *pclsummon, IN cltItem* pclItemList, OUT SI32 *psiReturnValue );
	// ��ȯ���� ���������� �����ġ����(��ȯ���� ����� �޸� �ƿ� �����Ҽ����� ��� ��ġ�� �ִ�)
	bool IsCanEquipItemPosForSummon( SI32 siEquipPosition, SI32 siItemUnique );


	// iconcode�� �ٰŷ� 
	SI32 FindBitmapIndex(TCHAR* typecode, TCHAR* iconcode);

	// ������ �����Ѵ�. 
	bool Enchant(cltItem* pclitem, SI32 scrollunique, SI32* prtnval, SI32* ppara1, SI32* ppara2);

	// Ư�� ��ǰ�� Ư�� ĳ���Ͱ� ���鶧 ǰ�� ������ ���Ѵ�. 
	SI32 GetQualityRate(SI32 itemunique, SI32 id, SI32* addlevel);
	
	// ���� ���ο� ������ ������ ��´�. 
	bool GetForeignNPCItemInfo(SI32 itemtype, SI32* punique, GMONEY* pprice, SI32* pitemnum);

	// ��ǰ �ü��� �����Ѵ�. 
	void CalcItemPrice( _SYSTEMTIME sTime);
	
	// �������� ������ ���Ѵ�. 
	GMONEY GetNPCRepairPrice(cltItem* pclitem);

	// ĳ���� kind�� �ٰŷ� ��ȯ ������ ����ũ�� ���Ѵ�. 
	SI32 GetItemDollUnique(SI32 kind);

	// ���� ����Ʈ�� �������� ������ ��ǰ�� ������ ���� Ȯ��. 
	bool GetDailyQuestRewardInfo(GMONEY money, SI32* pitemunique, SI32* pitemnum, GMONEY* prewardmoney );

	// ���� ����Ʈ2�� �������� ������ ��ǰ�� ������ ���� Ȯ��. 
	//bool GetDailyQuest2RewardInfo( SI32* pitemunique, SI32* pitemnum, GMONEY* prewardmoney );
	bool GetDailyQuest2RewardInfo(GMONEY money, SI32* pitemunique, SI32* pitemnum, GMONEY* prewardmoney );

	//---------------------------------------------------------------------------------------------------
	// ���ϸ� ����Ʈ3 ����Ʈ ������ ����
	bool GetDailyQuest3KindInfo(SI16* pItemUnique,SI16* pMakeNum);
	// ���� ����Ʈ3�� �������� ������ ��ǰ�� ������ ���� Ȯ��.
	bool GetDailyQuest3RewardInfo( SI32* siRewardExp, SI16 siMakeItemNum, SI16* pitemunique, SI16* pitemnum, SI32 SkillLevelLimit );
	//----------------------------------------------------------------------------------------------------
	// Ȥ�θ�(��ȣ��)�� ������ ���ط� ������ ��ǰ�� ������ ���� Ȯ��. 
	bool GetWeenyOldManMaterialInfo(SI16 resolutionRate, SI32 disassembleItem, cltItem* pDisAssemitem);

	bool GetRareItemWithWeenyOld(cltItem *pclitem);

	SI16 GetWennyOldItem( cltItem* pclItem, SI16 *resolutionRate );

	// �ǹ� ������ �������� ���Ѵ�.
	bool GetItemForStrRepair(GMONEY strprice, SI32* pitemunique, SI32* pitemnum);
	
	//-------------------------------------
	// ������ ��� ���� 
	//-------------------------------------
	//���� �ֱ⸶�� ������Ʈ �Ǹ� ��� ��Ͽ� ���� �۾��� �Ѵ�. 
	void WriteStatisticAction(_SYSTEMTIME sTime);
	//��踦 ����Ѵ�. 
	void WriteStatistic(TCHAR * filename );

	void WriteStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime );
	void ReadStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime );
    
	// ������ �ŷ� ��踦 ����. 
	void AddTradeNum(SI32 itemunique, SI32 itemnum);
	// ������ ��� ��踦 ����. 
	void AddUseNum(SI32 itemunique, SI32 itemnum);
	// ������ ���� ��踦 ����. 
	void AddMakeNum(SI32 itemunique, SI32 itemnum);
	// ������ ����(��ȣ��) ��踦 ����. 
	void AddTuneNum(SI32 itemunique, SI32 itemnum);
	// ������ ����(��ȣ��)���� ���� ������ ��踦 ����. 
	void AddFromTuneNum(SI32 itemunique, SI32 itemnum);
	// ����ǰ ������ ��踦 ����. 
	void AddWarItemNum(SI32 itemunique, SI32 itemnum);
	// ���ǰ�� ��踦 ����. 
	void AddEquipItemNum(SI32 itemunique, SI32 itemnum);
	// ����Ʈ ���� ��ǰ�� ��踦 ����. 
	void AddQuestItemNum(SI32 itemunique, SI32 itemnum);
	// �����۸����� ���� ��ǰ�� ��踦 ����. 
	void AddMallItemNum(SI32 itemunique, SI32 itemnum);
	// ������� ���� ���� ��ǰ�� ��踦 ����.
	void AddProductionItemNum(SI32 itemunique, SI32 itemnum);
	// ���������� ���� ���Թ�ǰ�� ��踦 ����.
	void AddWorldTradeIncomeItemNum(SI32 itemunique, SI32 itemnum);
	// ���������� ���� ���⹰ǰ�� ��踦 ����.
	void AddWorldTradeExportItemNum(SI32 itemunique, SI32 itemnum);
	// ������ ���ο� ���� ��鿩�� ��ǰ�� ��踦 ����.
	void AddForeignMerchantItemNum(SI32 itemunique, SI32 itemnum);
	// å���ڿ� ���� ���� ��ǰ�� ��踦 ����.
	void AddBookBoxItemNum(SI32 itemunique, SI32 itemnum);

	// ������
	void AddElixirNum(SI32 itemunique, SI32 itemnum);
	void AddMagicalNostrumNum(SI32 itemunique, SI32 itemnum);


	//----------------------------------------------
	// ��æƮ ��� ����
	//----------------------------------------------

#if  MAKE_BY_MUNIN //2007-05-21
	void WriteEnchantStatisticAction(bool backUpEnchantDataLoaded  , _SYSTEMTIME sTime);
	void WriteEnchantStatistic( _SYSTEMTIME sTime );
	void AddEnchantNum(SI32 itemunique, SI32 ItemGrade, DBMaterialInfo* pclinfo,  SI32 sucess);
	void WriteEnchantStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime );
	void ReadEnchantStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime );
#else
	void WriteEnchantStatistic();
	void AddEnchantNum(SI32 itemunique, SI32 ItemGrade, DBMaterialInfo* pclinfo,  bool sucess);
#endif

	//------------------------------------
	// ���� ����.
	//------------------------------------
	// Ư�� �������� ���������� ������ ����. 
	bool CanTune(cltItem* pclitem);

	// �������� ���������� �����Ѵ�. 
	bool GetTuneItem(cltItem* pclitem,bool bSuccess = false,SI32 mode = 0 );

	// ���ǰ ���� ����� ���� ���� �䱸 ������ ���Ѵ�. 
	bool GetScrollForTune2(cltItem clitem, SI32* pscrollunique, SI32* pscrollnum, cltItem* pcldestitem, SI32* preturnval);

	//---------------------------------------------
	// ��ǰ ���� 
	//---------------------------------------------

	//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ����ü Ÿ��.
	void MakeCrystal(cltItem* pclitem, SI32 qualityrate);

	// ���ݷ��� �ο��Ѵ�. 
	void MakeApower(cltItem* pclitem, SI32 qualityrate);
	// ������ �ο��Ѵ�. 
	void MakeDpower(cltItem* pclitem, SI32 qualityrate);
	
	// ��ǰ�� �������� �����. 
	void MakeDur(SI16* pbrareswitch, SI32 orgqualityrate, SI32 addlevel, cltItem* pclitem, bool bforceswitch,SI32 durrate = 0,SI32 monstercorp = 0 );
	// ��ǰ�� �Ⱓ�� �����.
	void MakeDate(SI16* pbrareswitch, SI32 orgqualityrate, SI32 addlevel, cltItem* pclitem, bool bforceswitch,SI32 durrate = 0,SI32 monstercorp = 0 );

	// �ƹ�Ÿ ��ǰ�� �Ⱓ�� �����.
	void MakeAvatarDate(SI16* pbrareswitch, SI32 orgqualityrate, SI32 addlevel, cltItem* pclitem, bool bforceswitch,SI32 durrate = 0,SI32 monstercorp = 0 );

	// ����ǰ�� �Ⱓ�� ���´�. (����)
	SI32 GetHighItemDate();

	// ��ǰ�� �Ⱓ�� Ȯ���Ѵ�.
	bool IsExpiredDate(cltItem* pclitem);


	// [�߰� : Ȳ���� 2008. 3. 3 => �������� ȯ��.]
	bool MakeItembyRestore( cltItem* pclEquipItem, cltItem* pclRestoreItem, cltCharCommon* pclchar );

	// �������� Unique������ ���� �����ϰ� �������� �ϳ� �����.
	bool MakeRandItemUnique(SI32 siunique, cltItem* pclitem,
		SI32 qualityrate, SI32 addlevel, SI16* pbrareswitch,
		SI32 attackrate = 0 ,SI32 magrate = 0 ,SI32 skillrate = 0, SI32 durrate = 0,
		SI32 monstercorp = 0 ,SI32 usedate = 0, SI32 enchanttype = 0, SI32 enchantlevel = 0, SI32 slotnum = 0, bool bSEQCreate = false);

	// �� �������� �����Ѵ�.
	bool MakeDefaultItem(SI32 siunique, SI32 itemnum, cltItem* pclitem);
	
	// �־��� ��Ḧ �ٰŷ� ���� ������ �������� ����Ʈ�� �ۼ��ؼ� �����Ѵ�. 
	// ���� ����Ʈ�� ����� ������ �־�� �Ѵ�. 
	bool FindMakeableItem(cltPSkillInfo* pclskill, cltPItemInfo* pclcharitem, cltBasicAbility* pclBA, SI32 materialnum, cltItem pclmaterialitem[], SI32* pmakeableitemnum, SI32** ppitemlist, bool bonlymaterialcheckswitch);

	// ��ǰ�� ��ǰ���� ������ش�. 
	bool MakeRareItem(cltItem* pclitem,SI32 mode = 0);
	
	// ����� ��ǰ�� ������ �ٰŷ� ����ġ�� ���Ѵ�. 
	SI32 GetMakeExpFromItemPrice(GMONEY totalprice, SI32 level);

	// ����� ��ǰ�� ������ �ٰŷ� ����ġ�� ���Ѵ�. 
	SI32 GetMakeSkillExpFromItemPrice(GMONEY totalprice, SI32 skilllevel);

	// Ư�� �������� ���� �����Ѱ� ?
	bool CanPile(SI32 itemunique);
	
	// ���� �� �ִ� ��ǰ�ΰ� ?(���ᰡ �����ϴ� ��ǰ�ΰ�. ) 
	bool CanMakableItem(SI32 itemunique);

	// ���ڻ�㰡�� ������ ����Ѵ�.
	void CalcInvestment(bool bStart);

	// ��� ����ǰ�� �����ϰ� ���´�. 
	SI32 GetRandBigWarItem();
	// ��� ����ǰ�� �����ϰ� ���´�. - ��ȣ��
	SI32 GetRandBigWennyItem();
	// ��� ����ǰ�� �����ϰ� ���´�. - ũ��������
	SI32 GetRandBigChristmasItem();

	// Ư�� ��ǰ�� ��� ��ǰ�ΰ�. 
	bool IsBigWarItem(SI32 itemunique);

	// �����ϰ� �������� �̸��� ���Ѵ�. 
	const TCHAR* GetRandomItemName();
	//------------------------------------------------

	// �ɸ��Ϳ� �´� ���� ���� �������� �˾ƿ´�.

	SI32 GetCushionUniqueFromCharKind(SI32 charkind);


	//------------------------------------------------
	//            ��æƮ  by tae-yang
	//------------------------------------------------
//	stEnchantNeed GetEnchantNeedItem(cltItem cltOrgItem, SI32 siSuccessUpItemNum = 0, BOOL bUseBlessingStone = FALSE, BOOL bUseSealingUpIgnore = FALSE );
	void GetEnchantNeedItem(cltItem cltOrgItem, stEnchantNeed* pclEnchantNeed, SI32 siSuccessUpItemNum = 0, BOOL bUseBlessingStone = FALSE, BOOL bUseSealingUpIgnore = FALSE );
	//SI32 GetGradeItem(cltItem cltOrgItem);
	//SI32 GetEnchantGrade(SI32 siEnchantDamage);
	//TCHAR* GetEnchantTypeName(TCHAR* szEnchantName, SI32 siEnchantType);
	SI32 GetEnchantDamageFromGrade(SI32 siGrade);


	// ��� ������ ǰ���� ����Ʈ�� ��´�. (plist�� �Ҵ��ϰ� ������ �߰����Ŀ� �����Ѵ�.) 
	bool GetFarmItemList(SI32* pitemnum, SI32** plist);
	
	// ���������� ����� ǰ���� ȹ���Ѵ�. 
	bool GetTraderItem(SI16* pitemunique, SI16* pitemnum, GMONEY* pitemprice);

	// Ư�� �Ӽ��� ���������� �Ӽ��ΰ�. 
	bool IsItemInfoAtb(SI16 itemunique, SI32 atb);
	
	// ���ո��Ͽ��� �ŷ��� �����Ѱ�?
	bool IsNewMarketTrade(SI16 itemunique);

	// ����� ���ؼ� ���� �� �ִ� ��� ����ġ.  
	SI32 GetFarmSkillExp(SI32 itemunique);

	// ä���� ���ؼ� ���� �� �ִ� ��� ����ġ.  
	SI32 GetMineSkillExp(SI32 itemunique);

	// ��ȯ�� �������� ���� ���̶� ����ġ���
	bool GetSummonFoodInfo(SI32* pitemunique,SI32* pitemnum,SI32* pexp,SI32* pskillexp);
	// ������ ������ ���Ѵ�. 
	SI32 GetEquipMag(cltItem* pclitemlist, SI16 activeweapon);
	// ������ ������ ���Ѵ�. 
	SI32 GetEquipMag(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar);
	// ������ �ٷ��� ���Ѵ�. 
	SI32 GetEquipStr(cltItem* pclitemlist , SI16 activeweapon);
	// ������ �ٷ��� ���Ѵ�. 
	SI32 GetEquipStr(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	// ������ ���߷��� ���Ѵ�. 
	SI32 GetEquipDex(cltItem* pclitemlist ,SI16 activeweapon);
	// ������ ���߷��� ���Ѵ�. 
	SI32 GetEquipDex(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	// ������ �������� ���Ѵ�. 
	SI32 GetEquipVit(cltItem* pclitemlist ,SI16 activeweapon);
	// ������ �������� ���Ѵ�. 
	SI32 GetEquipVit(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	// ������ ���� ���Ѵ�. 
	SI32 GetEquipLuk(cltItem* pclitemlist ,SI16 activeweapon);
	// ������ ���� ���Ѵ�. 
	SI32 GetEquipLuk(cltItem* pclitemlist, SI16 activeweapon,cltCharCommon* pclchar);
	// ������ �������� ���Ѵ�. 
	SI32 GetEquipHnd(cltItem* pclitemlist ,SI16 activeweapon);
	// ������ �������� ���Ѵ�. 
	SI32 GetEquipHnd(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	// ������ ���Ѵ�.
	SI32 GetEquipWis(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);

	// ������ ����Ÿ���� ���Ѵ�. 
	SI32 GetWeaponAttackType(cltItem* pclitemlist, SI16 activeweapon);
	// ������ ��Ÿ��� ���Ѵ�. 
	SI32 GetWeaponAttackRange(cltItem* pclitemlist, SI16 activeweapon);
	// ������ ���� �ֱ⸦ ���Ѵ�. 
	SI32 GetWeaponAttackInterval(cltItem* pclitemlist, SI16 activeweapon);
	// ����� ����  ���Ѵ�. 
	SI32 GetLuck(cltItem* pclitemlist);
	// ������ AC�� ���Ѵ�. 
	SI32 GetAC(cltItem* pclitemlist);
	// ������ AC�� ���Ѵ�. 
	SI32 GetAC(cltItem* pclitemlist, cltCharCommon* pclchar);

	// ��æƮ�� AC �� ���Ѵ� by tae-yang
	SI32 GetEnchantAC(cltItem* pclitemlist,cltCharCommon* pclchar );
	// ��æƮ�� AC �� ���Ѵ�. server-attack.cpp ���� ����ϱ� ���� 
	SI32 GetEnchantACFrom(cltItem* pclitemlist, SI32 attacktype = ENCHANT_TYPE_DEFAULT);	
	// ��æƮ�� Attack Damage �� ���Ѵ�. server-attack.cpp ���� ����ϱ� ���� 
	SI32 GetEnchantAttFrom(cltItem* pclitemlist, SI32 attacktype = ENCHANT_TYPE_DEFAULT);	

	// ������ �ּ� �������� ���Ѵ�. 
	SI32 GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon);
	SI32 GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar);
	// ��ȯ���� �ּ� �������� ���Ѵ�. 
	SI32 GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltSummon* pclsummon );
	// ������ �ִ� �������� ���Ѵ�. 
	SI32 GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon);
	SI32 GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar);
	//��ȯ���� �ִ� ������� ���Ѵ�.
	SI32 GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltSummon* pclsummon);

	// ������ ��æƮ �������� ���Ѵ�. by tae-yang
	//SI32 GetEnchantWeaponDamage(cltItem* pclitemlist, SI16 activeweapon);
	// ������ ��æƮ Ÿ���� ���Ѵ�. 
	SI32 GetEnchantWeaponType(cltItem* pclitemlist, SI16 activeweapon);

	bool IsCheckSkill(cltItem clOutputItem, SI32 materialcharunique);

	// ������� ��� ����� ���Ѵ�. 
	SI32 GetUpgradeSkillLevel(cltItem* pclitemlist, SI32 skillunique, SI32 id = 0);
	// ����� �ʻ�� ����� ���Ѵ�.   (���⿡��) 
	SI32 GetEquipCriticalHitRate(cltItem* pclitemlist, SI16 activeweapon);
	// ����� ���߷� ����� ���Ѵ�.  (���⿡��)
	SI32 GetEquipHitRate(cltItem* pclitemlist, SI16 activeweapon);
	// ����� ������ ���Ҹ� ���Ѵ�. 
	SI32 GetDamageReduceRate(cltItem* pclitemlist);
	// �Ź��� �̵��ӵ��� ���Ѵ�. 
	SI32 GetMoveSpeed(cltItem* pclitemlist);
	// �Ź��� �̵��ӵ��� ���Ѵ�. 
	SI32 GetMoveSpeed(cltItem* pclitemlist, cltCharCommon* pclchar);

	// ������ �̹��� ������ ���´�.
	bool GetItemGImageFileIndexAndFont( SI32 ItemUnique, SI32 *pGlobalImageIndex, SI32 *pGlobalImageFont );
	
	void GetItemImageItemmallFileName( SI32 siFromDBImgIndex, TCHAR* FileName, SI32 siLen , SI32* ImageIndex);		// [����] �����۸��� �� ���ϳ��Ӱ� �̹��� �ε����� ������ �ɴϴ�.
	void GetItemNameItemmallFileName( SI32 siFromDBImgIndex, TCHAR* FileName, SI32 siLen , SI32* ImageIndex);		// [����] �����۸��� �� ���ϳ��Ӱ� �̹��� �ε����� ������ �ɴϴ�.

	SI32 GetCompletedSetItemIndex( cltItem* pclitemlist );

	bool CheckCanPutonItem( SI32 unique );

	//KHY - 0115 �߷�Ÿ�ε��� �̺�Ʈ. - �̺�Ʈ ���������� ã�´�.
	bool GetValentineEvent(SI32 siUnique);

	bool GetWhiteEvent(SI32 siUnique);

	//////////////////////////////////////////////////////////////////////////
	//KHY - 20090525 - �ƹ�Ÿ ����ü. �ƹ�Ÿ ����ü��  ���밡����  �������ΰ�.
	bool IsCrystalAbatarItem( SI32 siItemType );	
	bool IsAbatarCrystalItem( UI08 uiCrystalAbility );

	// �ƹ�Ÿ�� ������ ����, �ɷ�ġ�� MAX ���� ���Ѵ�.
	SI32 GetMaxAbatarCrystalAbilityNum(SI32 siItemType , UI08 uiCrystalAbility);
	//////////////////////////////////////////////////////////////////////////
	
	enum SLOTITEMTYPE{	SLOTITEMTYPE_NONE, SLOTITEMTYPE_WEAPON, SLOTITEMTYPE_AVATAR, SLOTITEMTYPE_DORMANCY };
	
	// [�߰� : Ȳ���� 2008. 2. 29 => ������ ����ϴ� �������ΰ�.]
	SI32 UsedSlotItem(SI32 siUnique);		
	//////////////////////////////////////////////////////////////////////////
	// [���� 090507] - �ƹ�Ÿ������Ÿ������ �ƴ��� ��.
	//enum ITEMTYPECOMPARE{	ITEMTYPE_AVATAR, ITEMTYPE_ETC 	};

	//-------------------------------------------------------------------------------------------------------------------
	//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ��� ���� ������ ���Ѵ�. - ���⼭ ��� ���� �� ���.
	SI32 GetCountEquipPersonitemAbility(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar  ,UI08 abilityType ,SI32 *ReturnValPer);

	//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ��� ���� ������ ������ ���Ѵ�.	
	// ������ �ּ� �������� ���Ѵ�. 
	SI32 GetWeaponMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar , SI32 * CrystalPer);
	// ������ �ִ� �������� ���Ѵ�. 
	SI32 GetWeaponMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar, SI32 * CrystalPer);

	SI32 GetAddMagicalDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar );
	SI32 GetAddMagicalMinDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar );
	SI32 GetAddMagicalMaxDamage(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar );

	SI32 GetEquipCrystalPhysicalHitRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalMagicalHitRate(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar);

	SI32 GetEquipCrystalPhysicalDefensiveRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar , SI32 * CrystalPer);
	SI32 GetEquipCrystalMagicalDefensiveRate(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar , SI32 * CrystalPer);

	SI32 GetEquipCrystalPhysicalDodgeRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalMagicalDodgeRate(cltItem* pclitemlist, SI16 activeweapon, cltCharCommon* pclchar);

	SI32 GetEquipCrystalHitpoint(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalManapoint(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalWeigt(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);


	SI32 GetEquipCrystalFireAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalWaterAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalWindAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalEarthAttack(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalFireDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalWaterDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalWindDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);
	SI32 GetEquipCrystalEarthDeffence(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar);

	SI32 GetEquipCrystalCriticalHitRate(cltItem* pclitemlist, SI16 activeweapon ,cltCharCommon* pclchar );

	bool CanRepairItem( cltItem* pclitem );
	bool IsRepairStone( cltItem* pclitem );
	bool IsRepairStone( SI32 siItemUnique );
	bool IsRepairExtraMaterial( cltItem* pclitem );
	GMONEY RepairPrice( cltItem* pclitem );

	bool CanMaxRepairItem( cltItem* pclitem );
	bool IsMaxRepairStone( cltItem* pclitem );
	bool IsMaxRepairStone( SI32 siItemUnique );
	
	SI16 NeedRepairStoneNum( cltItem* pclitem );
	SI16 NeedBlessingStoneNum( cltItem* pclitem, SI32 BlessingStoneUnique );
	//-------------------------------------------------------------------------------------------------------------------

	
	//�Ʒ� CanMixtureData()�� ���� ����.
	enum
	{ 
		NO_ITEM_UNIQUE,					//													"��� ����ü�� ����� �� �ٽ� �õ��� �ּ���."
		NO_SLOT_ITEM,					// ������ �ִ� �������̴�.							"�߸��� �������� ��ϵ� �ֽ��ϴ�. �������� Ȯ���� �ּ���."
		NO_USED_SLOT,					// ��� ������ ������ ����.							"����� ����ü ������ ��� ������Դϴ�."					
		NO_CRYSTAL,						// �����ϰ��� �ϴ� �������� ����ü�� �ƴϴ�.		"�߸��� �������� ��ϵ� �ֽ��ϴ�. �������� Ȯ���� �ּ���."
		ALL_EMPTY_CRYSTAL,				// �����ϰ��� ����ü�� ����.						"�߸��� �������� ��ϵ� �ֽ��ϴ�. �������� Ȯ���� �ּ���."
		NO_REMOVE_CRYSTAL,				// ����ü ���� �������� �ƴϴ�.						"�߸��� �������� ��ϵ� �ֽ��ϴ�. �������� Ȯ���� �ּ���."
		NO_CRYSTAL_SLOT,				// ���Կ� ����ü�� ����.							"���� �ϰ��� �ϴ� ���Կ� ����ü�� �������� �ʽ��ϴ�. ��� �ٽ� ����Ͽ� �õ��� �ּ���"
		USE_CRYSTAL_SLOT,				// ������ �� ��� ���̴�.							"��� ����ü ������ ��� ���Դϴ�."
		FULL_CRYSTAL_SLOT,  			// ������ ��� �������ִ�.							"�ִ� �����̻����Ǿ� �ִ� �������Դϴ�."
		ALREADY_USE_SLOT,				// �̹� ������� ����.								"�̹� ������� ���� �Դϴ�."
		ALREADY_USED_INVEN_ITEM,		// �̹� �ڽ��� �������� ������� ������.			"�̹� ����ü ���Կ� �߰��� �������Դϴ�. �ٸ� ����ü�� ����ϼ���."
		SAME_CRYSTAL,					// ���� ����ü�� �̹� ������ �ִ�.					"�̹� ���� ȿ���� ����ü�� �ֽ��ϴ�. �ٸ� ȿ���� ����ü�� ����ϼ���."
		LIMIT_LEVEL,					// ����ü�� ������ ��� ������ �����ϴ�.			"��� ������ �����Ͽ� ����ü�� ���� �� �� �����ϴ�."
		LIMIT_CRYSTAL_ABILITY,			// ����ü�� ��� ������ �Ұ����� �ɷ��� ��������.	"�� ����ü�� �ɷ��� �ش� ��� ������ �Ұ����� �ɷ��Դϴ�."
		NO_RESTORE_ITEM,				// ȯ���� �� ���� ������.							"ȯ���� �� ���� �������Դϴ�."
		NO_RESTORE_ITEM_REGISTER,		// ȯ���� �������� ��� �ȵ�.						"ȯ���� �������� ����� ���� �ٽ� �õ��� �ּ���."
		ALREADY_RESULT_RESTORE_ITEM,	// ȯ���� �������� �־� �����ض�.					"ȯ�� ��� â�� �ִ� �������� ������ �� �ٽ� �õ��� �ּ���."
		DIFFERENT_CRYSTAL,
		POSSIBLE,						// ���� ������.	
		NO_LOW_DURABILITY,			// ������ ����������, �������� ������ų �� ����.(5����)	"����ü�� �ִ� �������� ���� �������� ������ų �� �����ϴ�."
	};
	// [�߰� : Ȳ���� 2008. 2. 29 => ����ü ������ ������ ����Ÿ �ΰ�?.]
	BOOL CanCrystalMixtureData( cltItem* _EquipItem, cltItem* _CrystalItem , SI16 _EquipItemPos);		

	// [�߰� : Ȳ���� 2008. 2. 30 => ����ü ���Ű� ������ ����Ÿ �ΰ�?.]
	BOOL CanCrystalRemoveData( cltItem* _EquipItem, bool* _RemoveSlot, cltItem* _CrystalItem);

	// [�߰� : Ȳ���� 2008. 2. 2 => ����ü�� ���� ������ ������ ����Ÿ �ΰ�?.]
	BOOL CanCrystalSlotCreateData( cltItem* _EquipItem, cltItem* _CrystalItem );

	// [�߰� : Ȳ���� 2008. 2. 29 => ����ü�� ��� ������ ������ �ɷ��ΰ�?]
	bool IsMixtureAbilityType(cltItem* pclEquipItem, SI32 CristalItemAbilytype);
};


class cltItemPriceUnit{
public:
	SI16	siUnique;
	SI16	siItemNum;				// ����� �Ǵ� �������� ����. 
	GMONEY	siPrice;				// �ŷ��� ����. 
	REAL64	fPrice;					// ���а���. 
	GMONEY	siMarketOrderPrice;		// ���� ����. 	
	
	cltItemPriceUnit()
	{
		siUnique			= 0;
		siPrice				= 0;
		fPrice				= 0.0f;
		siItemNum			= 0;
		siMarketOrderPrice	= 0;
	}
};

//------------------------------------------------
// ������ ������ �����ϴ� Ŭ���� 
//------------------------------------------------
class cltItemPrice{
private:

public:
	cltItemManagerCommon*	pclItemManager;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltItemPriceUnit, MAX_ITEMINFO_NUMBER>	clUnit;
#else
	cltItemPriceUnit clUnit[MAX_ITEMINFO_NUMBER];
#endif

	cltItemPrice(cltItemManagerCommon* pclitemmanager);
	~cltItemPrice();


	// �������� ������ �����Ѵ�.
	void SetPrice(cltItemManagerCommon* pclitemmanager, SI32 unique, GMONEY price, SI32 itemnum, bool bforceswitch);
	// �������� ������ ���Ѵ�. 
	cltItemPriceUnit* GetPrice(cltItemManagerCommon* pclitemmanager,SI32 unique);


	// ���� �ֹ������� �����Ѵ�. 
	void SetMarketOrderPrice(cltItemManagerCommon* pclitemmanager, SI32 unique, GMONEY price);

	// ���� �ֹ������� ���´�. 
	GMONEY GetMarketOrderPrice(SI32 unique);

	
	// Ư�� ��ǰ�� �ü��� �˷��ش�.(NPC��)
	GMONEY GetPriceSellToNPC(cltItemManagerCommon* pclitemmanager, SI32 unique);
};

//[����] ȣ�ڻ��ڿ� �����ϰ� ���µ� �ƾ�������.
class AmberboxSelectItemInfo
{
public:
	UI08	m_siSlot;
	SI16	m_siItemNum;
	SI16	m_siItemUnique;

	AmberboxSelectItemInfo()
	{
		ZeroMemory(this, sizeof(AmberboxSelectItemInfo));
	}
};

//	[��ȣ] ��Ʈ������ ����Ʈ �ܰ�
enum SETITEM_EFFECT_STEP
{
	SETITEM_EFFECT_NONE = 0,		//	nothing
	SETITEM_EFFECT_BASE_AURA,		//	�ٴڿ� �򸮴� Ÿ���� ����Ʈ
	SETITEM_EFFECT_SMALL_WIND,		//	���� ȸ����
	SETITEM_EFFECT_LARGE_WIND,		//	ū ȸ����
	SETITEM_EFFECT_END,
};

enum SETITEM_ABILITY_TYPE
{
	SETITEM_ABILITY_TYPE_EFFECT,						// ����Ʈ
	SETITEM_ABILITY_TYPE_ATTACK,						// ���ݷ� ( ������ ).
	SETITEM_ABILITY_TYPE_DEFENSIVE,						// ����.
	SETITEM_ABILITY_TYPE_STR,							// ��
	SETITEM_ABILITY_TYPE_DEX,							// ���߷�.
	SETITEM_ABILITY_TYPE_VIT,							// ������.
	SETITEM_ABILITY_TYPE_MAG,							// ����.
	SETITEM_ABILITY_TYPE_HAND,							// ������.
	SETITEM_ABILITY_TYPE_LUK,							// ���.
	SETITEM_ABILITY_TYPE_WIS,							// ����.
	SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE,		// �� �Ӽ�����..
	SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE,		// �� �Ӽ�����..
	SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE,		// �ٶ� �Ӽ�����..
	SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE,		// �� �Ӽ�����..
	SETITEM_ABILITY_TYPE_DRAIN_SUCCESS_PERCENT,			// �������� ������ �̾Ƴ��� Ȯ��.
	SETITEM_ABILITY_TYPE_DRAIN,							// �������� ������ �̾Ƴ���.
	SETITEM_ABILITY_TYPE_HIT_RATE,						// ���߷�
	SETITEM_ABILITY_TYPE_WEAPON_USE_SKILL,				// ���� ����
	SETITEM_ABILITY_TYPE_HP,							// HP ����
	SETITEM_ABILITY_TYPE_MP,							// MP ����
	SETITEM_ABILITY_TYPE_CRITICAL,						// ũ��Ƽ�� ���� Ȯ��
	SETITEM_ABILITY_TYPE_ICE_MAGIC_TARGET_ATTACK,		// ���ݽ� ��븦 ��������� Ȯ��
	SETITEM_ABILITY_TYPE_POISON_MAGIC_TARGET_ATTACK,	// [����] ���ݽ� ��븦 �ߵ���ų Ȯ��
	SETITEM_ABILITY_TYPE_ICE_MAGIC_GET_ATTACK,			// ���� ���� ������ ���� �� ��� Ȯ��
	SETITEM_ABILITY_TYPE_MAGIC_ATTACK,					// ���� ���ݷ� ( ������ ).
	SETITEM_ABILITY_TYPE_MAGIC_DEFENCE,					// [����] ���� ���� ( ���� ).
	SETITEM_ABILITY_TYPE_NO_FIRST_ATTACK,				// ��������
	SETITEM_ABILITY_TYPE_NATURE,						// ���� �Ӽ� ���
	SETITEM_ABILITY_TYPE_FIRE_ENCHANT_ATTACK,			// ��   �Ӽ� ����..
	SETITEM_ABILITY_TYPE_WATER_ENCHANT_ATTACK,			// ��   �Ӽ� ���ݷ�..
	SETITEM_ABILITY_TYPE_WIND_ENCHANT_ATTACK,			// �ٶ� �Ӽ� ���ݷ�..
	SETITEM_ABILITY_TYPE_LAND_ENCHANT_ATTACK,			// ��   �Ӽ� ���ݷ�..
};

enum USE_SETITEM_NUM	// ��� ������ ��Ʈ ������ ����.
{
	USE_SETITEM_1 = 1,
	USE_SETITEM_2,
	USE_SETITEM_3,
	USE_SETITEM_4,
	USE_SETITEM_5,
};

// [����] ��Ʈ ������ �ɷ�ġ ����.
class cltSetItemAbility
{
public:
	SI32	m_siEffect;					// ����Ʈ
	SI32	m_siAttack;					// ���ݷ� ������.
	SI32	m_siDefensive;				// ����.
	SI32	m_siStr;					// �ٷ�.	
	SI32	m_siDex;					// ���߷�
	SI32	m_siVit;					// ������ 
	SI32	m_siMag;					// ����. 
	SI32	m_siHnd;					// ������. 
	SI32	m_siLuk;					// ���. 
	SI32	m_siWis;					// ����. 
	SI32	m_siFireEnchantDefensive;	// ��   �Ӽ� ����.
	SI32	m_siWaterEnchantDefensive;	// ��   �Ӽ� ����.
	SI32	m_siWindEnchantDefensive;	// �ٶ� �Ӽ� ����.
	SI32	m_siLandEnchantDefensive;	// ��   �Ӽ� ����.
	SI32	m_siDrainSuccessPercent;	// �������� ������ �̾Ƴ��� Ȯ��.
	SI32	m_siDrain;					// �������� ������ �̾Ƴ���.
	SI32	m_siHitRate;				// ���߷�
	SI32	m_siWeaponUseSkill;			// ���� ����
	SI32	m_siHP;						// hp ����
	SI32	m_siMP;						// mp ����
	SI32	m_siCritical;				// ũ��Ƽ��
	SI32	m_siIceMagicTargetAttack;	// ���ݽ� ��븦 ��������� Ȯ��
	SI32	m_siPoisonMagicTargetAttack;// [����] ���ݽ� ��븦 �ߵ� ��ų Ȯ��
	SI32	m_siIceMagicGetAttack;		// ���� ���� ������ ���� �� ��� Ȯ��
	SI32	m_siMagicAttack;			// ���� ������
	SI32	m_siMagicDefence;			// [����] ���� ���� (������ ������ 10% ���� ��, ���� ���� 10% ����)
	SI32	m_siNoFirstAttack;			// ��������
	SI32	m_siNature;					// ���� �Ӽ� ����.
	SI32	m_siFireEnchantAttack;		// ��   �Ӽ� ���ݷ�.
	SI32	m_siWaterEnchantAttack;		// ��   �Ӽ� ���ݷ�.
	SI32	m_siWindEnchantAttack;		// �ٶ� �Ӽ� ���ݷ�.
	SI32	m_siLandEnchantAttack;		// ��   �Ӽ� ���ݷ�.

	

public:
	cltSetItemAbility()		{	ZeroMemory( this, sizeof(*this) );	}
	~cltSetItemAbility()	{	;									}

};

// [����] ��Ʈ ������ �ɷ�ġ ���� �޴���.

enum
{
	VAMPIRE_SETITEM = 1,
	XMAS_SETITEM,
	GIRL_SULBIM_SETITEM,
	BOY_SULBIM_SETITEM,
	HALLOWEEN1_SETITEM,
	HALLOWEEN2_SETITEM,
	HALLOWEEN3_SETITEM,
	HALLOWEEN4_SETITEM,

	FROZEN_SETITEM_STEP,
	FLAME_SETITEM_STEP,
	DEMON_SETITEM_STEP,
	ABEL_SETITEM_STEP,
	BLACKKNIGHT_SETITEM_STEP,
	DEVIL_MAN,
	DEVIL_WOMAN,

	SETITEM_NUM
};

enum
{
	TWO_STEP = 2,
	THREE_STEP,
	FOUR_STEP,
	FIVE_STEP,
	SIX_STEP,
	SEVEN_STEP,

	MAX_STEP_NUM = 6	// �ܰ� ��.
};


class cltSetItemAbilityManager
{
private:
	cltSetItemAbility	m_clSetItemAbility[ SETITEM_NUM ][ MAX_STEP_NUM ];

public:
	cltSetItemAbilityManager();
	~cltSetItemAbilityManager();

	void				Init( void );
	void				FileLoad( void );
	cltSetItemAbility*	GetSetItemAbility( SI16 _SetItemType );
	cltSetItemAbility*  GetSetItemAbilityData( cltCharCommon* _pclchar, SI32 _UseSetItem );
	SI32				GetSetItemAbilityValue( cltCharCommon* _pclchar, SI16 _siAbilityType, SI32 _UseSetItem );

	SI32				GetSetItemE( cltCharCommon* _pclchar, SI32 _UseSetItem );
	SI32				GetSetItemEffet( cltCharCommon* _pclchar, SI32 _UseSetItem );
	SI32				GetSetItemHumanEffect( cltCharCommon* _pclchar, SI32 _UseSetItem );
};

//-----------------------------------------
//  ��ǰ ���꿡 �ʿ��� ��� ����. 
//-----------------------------------------
class DBMaterialInfo{
public:
	SI16 siStgPos;		// â���� ��ġ. 
	SI32 siItemUnique;	// ��� �������� ����ũ 
	SI32 siUseItemNum;	// ���Ǵ� �������� ����. 

	DBMaterialInfo()
	{
		Init();
	}
	void Init()
	{
		siStgPos		= 0;
		siItemUnique	= 0;
		siUseItemNum	= 0;
	}

	void Set(DBMaterialInfo* pclinfo)
	{
		siStgPos		= pclinfo->siStgPos;
		siItemUnique	= pclinfo->siItemUnique;
		siUseItemNum	= pclinfo->siUseItemNum;
	}
};

//-----------------------------------------
//  ��ǰ ���꿡 �ʿ��� ���� ����. 
//-----------------------------------------
class DBToolInfo{
public:
	cltItem clItem;
	SI16	siPos;

	DBToolInfo()
	{
		siPos = 0;
	}
	void Init()
	{
		clItem.Init();
		siPos = 0;
	}
	void Set(DBToolInfo* pclinfo)
	{
		clItem.Set(&pclinfo->clItem);
		siPos	= pclinfo->siPos;
	}

	void Set( SI16 Pos, cltItem *pclItem )
	{
		clItem.Set( pclItem );
		siPos = Pos;
	}
};

#define REPAIRE_ITEM_EXTRAMATERIAL_COUNT 5
typedef NSafeTArray<DBMaterialInfo, REPAIRE_ITEM_EXTRAMATERIAL_COUNT> RepairItemExtraMaterial;





//[����] ��ų��
#define SKILLBOOK_INFO_NUM	100

class cltSkillBookInfo
{
public:
	SI32	m_siItemUnique;				
	SI32	m_siMagicKind;			
	SI32	m_siMagicName;			
	SI32	m_siParentMagicKind;
	SI32	m_siImageNum;
	SI32	m_siTextNum;	
	SI32	m_siAttribute;	
	SI32	m_siBookGrade;
	SI32	m_siBookNum;	
	GMONEY  m_siCost;
};

// [����] ��ų��
class cltSkillBookManager
{
public:
	enum ATTIRIBUTE
	{
		ATTRIBUTE_NONE,
		ATTRIBUTE_FIRE, 
		ATTRIBUTE_WATER, 
		ATTRIBUTE_EARTH, 
		ATTRIBUTE_WIND,

		ATTRIBUTE_NUM,
	};

	enum MAGIC_NAME
	{
		NONE,
		BUFTIMEINCREASE1,		//	�������� �����ð� 10%����
		BUFTIMEINCREASE2,		//	�������� �����ð� 20%����
		HITCOUNTINCREASE,		//	���� ����ȸ�� ����
		FREEZINGTIMEINCREASE,	//	���Ḷ�� ���������ð� ����
		ADDMAGIC,				//	Ư�� �����ο�
		ATTINCREASE,			//	�������ݷ� ����
		ATTINCREASE_CANE,		//	�������ݷ� ����(������)
		RANGEINCREASE,			//	������Ÿ� ����
		COOLTIMEDECREASE,		// ���� ����ð� ����
		MPDECREASE,				// �Ҹ�Ǵ� ���� ����
		ORGDECREASE,			// �Ҹ�Ǵ� �����ġ ����

		MAGIC_NAME_NUM
	};

	enum BOOK_GRADE
	{
		BOOK_GRADE_NONE,
		BOOK_GRADE_BGN,		
		BOOK_GRADE_INT,		
		BOOK_GRADE_ADV,		
		
		BOOK_GRADE_NUM,
	};

public:
	cltSkillBookManager();
	~cltSkillBookManager();

	void		Init( void );
	void		FileLoad( void );

	SI32		GetSkillBookInfoNum( void );
	bool		IsSubMagicFromParentMagicKind( IN SI32 _siParentMagicKind );
	bool		GetSkillBookUniqueFromMagicKind( IN SI32 _siParentMagicKind, IN SI32 _siMagicKind, OUT SI32& _siItemUnique, OUT SI32& _siImageNum, OUT SI32& _siTextNum );
	bool		GetSkillBookUniqueFromParentMagicKind( IN SI32 _siParentMagicKind, OUT SI32& _siItemUnique, OUT SI32& _siImageNum, OUT SI32& _siTextNum );
	bool		GetSkillBookItemUniqueFromAttribute( IN SI32 _siAttribute, IN SI32 _siOutItemNum, OUT SI32* _siItemUnique );
	SI32		GetSkillBookMagicFromItemUnique( IN SI32 _siItemUnique );
	SI32		GetSkillBookParentMagicFromItemUnique( IN SI32 _siItemUnique );
	bool		GetSkillBookInfoFromAttribute( IN SI32 _siAttribute, IN SI32 _siOutNum, OUT cltSkillBookInfo* _pclSkillBookInfo );
	TCHAR*		GetGradeText( SI32 _siGrade );
	SI32		GetSkillBookGradeFromItemUnique( IN SI32 _siItemUnique );
	bool		IsSkillBookItemGrade( IN SI32 _siGrade, IN SI32 _siItemUnique );
	bool		IsSkillBookItem( IN SI32 _siItemUnique );

private:
	cltSkillBookInfo	m_clSkillBookInfo[SKILLBOOK_INFO_NUM];
	SI32				m_siSkillBookInfoNum;
};
// [����] ���� ���� ���ڿ��� �������� �ش� 
#define MAX_REWARDITEMBOX_NUM	10	// ���ҽ��� ������ ������ �ִ� �ڽ� ������ �� ����
#define TOTALITEMNUM_USELEVELLIMITBOX	5	// �Ѱ��� �ڽ����� �ټ� �ִ� �������� �� ����

class BoxData
{
public:
	SI32	siBoxitemunique	;
	SI32	siBoxiOpenLevel	;
	GMONEY	gRewardMoney	;
	SI32	siRewardTotalNum	;
	SI32	siRewardItemUnique_1	;
	SI32	siRewardItemNum_1	;
	SI32	siRewardItemUnique_2	;
	SI32	siRewardItemNum_2	;
	SI32	siRewardItemUnique_3	;
	SI32	siRewardItemNum_3	;
	SI32	siRewardItemUnique_4	;
	SI32	siRewardItemNum_4	;
	SI32	siRewardItemUnique_5	;
	SI32	siRewardItemNum_5	;
};

class cltRewardItemForLimitLevel	// �߱����� ���� ������ 
{
public:
	cltRewardItemForLimitLevel()	;
	~cltRewardItemForLimitLevel()	;

	// �ڽ� ������ ���� ���� 
#ifdef _SAFE_MEMORY
	NSafeTArray<BoxData, MAX_REWARDITEMBOX_NUM>		m_RewardITemBoxLinmitLevel_ForNHNChina;
#else
	BoxData  m_RewardITemBoxLinmitLevel_ForNHNChina[MAX_REWARDITEMBOX_NUM];
#endif
	void init()	;
	void LoadBoxInfo()	;							// ���� ������  �о� ���δ�
	bool IsRewardBox_Unique( SI32 siItemUnique )	;
	SI32 GetOpenLevel( SI32 siItemUnique )	;
	SI32 GetTotalItemNum( SI32 siItemUnique )	;
	SI32 GetBoxUnique( SI32 siArray_Index )	;
};
#endif
