//---------------------------------
// 2003/6/9 김태곤
//---------------------------------

#include "cltItem.h"
#include "..\..\Skill\Skill.h"
#include "../../Char/KindInfo/kindinfo.h"

#include "../../../CommonLogic/CommonLogic.h"
#include "../../../CommonLogic/TextMgr/TextMgr.h"

#include "../../Server/Server.h"
#include "../../Server/GachaManager/GachaManager.h"
#include "../../Server/Gacha2Manager/Gacha2Manager.h"

#include "../../../common/Event/event.h"
//#include "../../../common/item/ItemCommon/ItemUnit.h"

//extern cltTextMgr g_pTextMgr;
//extern cltTextMgr g_pHelpMgr;
//extern cltTextMgr g_pStructureMgr;

//class cltItemAvatarNeck;

extern cltCommonLogic*	pclClient;

extern	SI32 skilldata[] =
{
	SKILL_MAKEAXE1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPAXE, 0, 
		SKILL_MAKESPEAR1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSPEAR, 0, 
		SKILL_MAKESWORD1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSWORD, 0, 
		SKILL_MAKESTAFF1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSTAFF,  0, 
		SKILL_MAKEBOW1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBOW,  0, 
		SKILL_MAKEGUN1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPGUN,  0, 
		SKILL_MAKEARMOUR1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPARMOUR,  0, 
		SKILL_MAKEHELMET1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHELMET,  0, 
		SKILL_MAKEDRESS1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPDRESS,  0, 
		SKILL_MAKEHAT1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHAT,  0, 
		SKILL_MAKESHOES1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSHOES,  0, 
		SKILL_MAKEBELT1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBELT,  0,
		//SKILL_MAKENECK1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPNECK,  0, 
		//cyj 반지제작술 삭제 SKILL_MAKERING1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING, 
		//SKILL_USESWORD1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD,  0, 
		//SKILL_USESPEAR1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR, 0, 
		//SKILL_USEAXE1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE, 0, 
		//SKILL_USEBOW1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW, 0, 
		//SKILL_USEGUN1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN,  0, 
		//SKILL_USESTAFF1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF,  0, 
		SKILL_USESWORD1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD,
		SKILL_USESPEAR1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR,
		SKILL_USEAXE1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE,
		SKILL_USEBOW1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW,
		SKILL_USEGUN1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN,
		SKILL_USESTAFF1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF,
		SKILL_ARTIFACT,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPATIFACT,  0, //아티팩트 

		0,				0
};

//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
extern	SI32 Crystalskilldata[] =
{
		SKILL_SUMMON,				CRYSTALABILITY_SKILL_SUMMON,			0,
		SKILL_FARM,					CRYSTALABILITY_SKILL_FARM,				0,
		SKILL_MINE, 				CRYSTALABILITY_SKILL_MINE,				0,
		SKILL_FISH,					CRYSTALABILITY_SKILL_FISH,				0,
		SKILL_MAKEFOOD,				CRYSTALABILITY_SKILL_MAKEFOOD,			0,
		SKILL_CAPTURE,				CRYSTALABILITY_SKILL_CAPTURE,			0,
		SKILL_HORSETRAINING,		CRYSTALABILITY_SKILL_HORSETRAINING,		0,

		SKILL_MAKEIRON1,			CRYSTALABILITY_SKILL_MAKEIRON1,			0,
		SKILL_MAKETOOL1,			CRYSTALABILITY_SKILL_MAKETOOL1,			0,
		SKILL_MAKEMED,				CRYSTALABILITY_SKILL_MAKEMED,			0,

		SKILL_MAKESWORD1,			CRYSTALABILITY_SKILL_MAKESWORD1, 		0, 
		SKILL_MAKESPEAR1,			CRYSTALABILITY_SKILL_MAKESPEAR1, 		0, 
		SKILL_MAKEAXE1,				CRYSTALABILITY_SKILL_MAKEAXE1,			0, 
		SKILL_MAKESTAFF1,			CRYSTALABILITY_SKILL_MAKESTAFF1,		0, 
		SKILL_MAKEBOW1,				CRYSTALABILITY_SKILL_MAKEBOW1,			0, 
		SKILL_MAKEGUN1,				CRYSTALABILITY_SKILL_MAKEGUN1,			0, 
		SKILL_MAKEARMOUR1,			CRYSTALABILITY_SKILL_MAKEARMOUR1,		0, 
		SKILL_MAKEHELMET1,			CRYSTALABILITY_SKILL_MAKEHELMET1,		0, 		
		SKILL_MAKESHOES1,			CRYSTALABILITY_SKILL_MAKESHOES1,		0, 
		SKILL_MAKEBELT1,			CRYSTALABILITY_SKILL_MAKEBELT1,			0, 

		SKILL_USESWORD1,			CRYSTALABILITY_SKILL_USESWORD1,			0,
		SKILL_USESPEAR1,			CRYSTALABILITY_SKILL_USESPEAR1,			0,
		SKILL_USEAXE1,				CRYSTALABILITY_SKILL_USEAXE1,			0,
		SKILL_USESTAFF1,			CRYSTALABILITY_SKILL_USESTAFF1,			0,
		SKILL_USEBOW1,				CRYSTALABILITY_SKILL_USEBOW1,			0,
		SKILL_USEGUN1,				CRYSTALABILITY_SKILL_USEGUN1,			0,
		SKILL_ARTIFACT,				CRYSTALABILITY_SKILL_ARTIFACT,			0,

		ALL_MAKESKILL_UNIQUE,		CRYSTALABILITY_SKILL_ALLMAKESKILL,		0,
		ALL_WEAPONSKILL_UNIQUE,		CRYSTALABILITY_SKILL_ALLWEAPONSKILL,	0,
		ALL_PRODUCTSKILL_UNIQUE,	CRYSTALABILITY_SKILL_ALLPRODUCTSKILL,	0,
		ALL_ALLSKILL_UNIQUE,		CRYSTALABILITY_SKILL_ALLSKILL,			0,
		ALL_ALLAVATARSKILL_UNIQUE,		CRYSTALABILITY_SKILL_ALLAVATARSKILL,			0,
		
		0, 0, 0
		//SKILL_MAKEDRESS1,			SCROLL_UNIQUE_FUNCTION_MAKESKILLUPDRESS,	0, 
		//SKILL_MAKEHAT1,			SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHAT,		0, 				
		//SKILL_MAKENECK1,			SCROLL_UNIQUE_FUNCTION_MAKESKILLUPNECK,		0, 
		//cyj 반지제작술 삭제 SKILL_MAKERING1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING, 
		//SKILL_USESWORD1,			SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD,	0, 
		//SKILL_USESPEAR1,			SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR,	0, 
		//SKILL_USEAXE1,			SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE,		0, 
		//SKILL_USEBOW1,			SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW,		0, 
		//SKILL_USEGUN1,			SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN,		0, 
		//SKILL_USESTAFF1,			SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF,	0, 
};


// 아이템 옵션 텍스트를 구한다. 
bool cltItemOptionInfo::GetText(TCHAR* ptext, SI16 txtSize)
{
	TCHAR temp[256];
	StringCchCopy(ptext, txtSize, TEXT(""));

	for(SI32 i = 0;i < 2;i++)
	{
		UI32 type;
		UI32 para;

		if(i == 0)
		{
			type	= uiType1;
			para	= uiPara1;

		}
		else
		{
			type	= uiType2;
			para	= uiPara2;
		}

		if(type == 0)continue;

		UI32 rate; 
		switch(type)
		{
		case  SCROLL_UNIQUE_FUNCTION_UPHITRATE:
			{
				TCHAR* pText = GetTxtFromMgr(687);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate, '%');
				StringCchCat(ptext, txtSize, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_UPCRITICAL:	
			{
				TCHAR* pText = GetTxtFromMgr(688);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate, '%');
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_POISON:
			{
				TCHAR* pText = GetTxtFromMgr(689);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate, '%');
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_ICE:
			{
				TCHAR* pText = GetTxtFromMgr(690);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate, '%');
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_REDUCEDAMAGE:
			{
				TCHAR* pText = GetTxtFromMgr(691);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate, '%');
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_DRAIN:
			{
				TCHAR* pText = GetTxtFromMgr(692);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate, '%');
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPAXE:
			{
				TCHAR* pText = GetTxtFromMgr(693);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSPEAR:
			{
				TCHAR* pText = GetTxtFromMgr(694);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSWORD:
			{
				TCHAR* pText = GetTxtFromMgr(695);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);

			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSTAFF:
			{
				TCHAR* pText = GetTxtFromMgr(696);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBOW:
			{
				TCHAR* pText = GetTxtFromMgr(697);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPGUN:
			{
				TCHAR* pText = GetTxtFromMgr(698);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPARMOUR:
			{
				TCHAR* pText = GetTxtFromMgr(699);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHELMET:
			{
				TCHAR* pText = GetTxtFromMgr(700);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPDRESS:
			{
				TCHAR* pText = GetTxtFromMgr(701);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHAT:
			{
				TCHAR* pText = GetTxtFromMgr(702);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSHOES:
			{
				TCHAR* pText = GetTxtFromMgr(703);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBELT:
			{
				TCHAR* pText = GetTxtFromMgr(704);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPNECK:
			{
				TCHAR* pText = GetTxtFromMgr(705);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		/*cyj 반지제작술 삭제
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING:
			{
				TCHAR* pText = GetTxtFromMgr(706);
				rate = para / 10;
				sprintf(temp, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		*/
		case SCROLL_UNIQUE_FUNCTION_UPMAXDURABILITY:
			{
				TCHAR* pText = GetTxtFromMgr(707);
//				rate = (float)(para);
				rate = para;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);

			}
			break;				

		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD:
			{
				TCHAR* pText = GetTxtFromMgr(708);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR:
			{
				TCHAR* pText = GetTxtFromMgr(709);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE:
			{
				TCHAR* pText = GetTxtFromMgr(710);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW:
			{
				TCHAR* pText = GetTxtFromMgr(711);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN:
			{
				TCHAR* pText = GetTxtFromMgr(712);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF:
			{
				TCHAR* pText = GetTxtFromMgr(713);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON:
			{
				TCHAR* pText = GetTxtFromMgr(5679);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPATIFACT://아티팩트.
			{
				TCHAR* pText = GetTxtFromMgr(711);
				rate = para / 10;
				StringCchPrintf(temp, 256, pText, rate);
				StringCchCat(ptext, txtSize,  temp);
			}
			break;
		default:
			MsgBox(TEXT("vdidjf"), TEXT("f38fdsjf:%d"), type);
		}
	}

	if(_tcscmp(ptext, TEXT("")) == 0)
	{
		return false;
	}
	return true;
}


//---------------------------------------
// cltItem
//---------------------------------------

cltItem::cltItem(SI32 siunique)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	uiDateUseDay = siUseDate;

	
}


//cyj  기간제 생성자
cltItem::cltItem(SI32 siunique, SI32 siUseDate,stElement* element, cltItemWeapon* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemWeapon.Set(pcldata);
	
	uiDateUseDay = siUseDate;

	memcpy(&Element, element, sizeof(stElement));
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate,stElement* element, cltItemArmour* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemArmour.Set(pcldata);

	memcpy(&Element, element, sizeof(stElement));
	uiDateUseDay = siUseDate;
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate,  cltItemNeck* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemNeck.Set(pcldata);

	uiDateUseDay = siUseDate;
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element , cltItemShoes* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemShoes.Set(pcldata);

	memcpy(&Element, element, sizeof(stElement));

	uiDateUseDay = siUseDate;
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element, cltItemRing* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemRing.Set(pcldata);

	memcpy(&Element, element, sizeof(stElement));

	uiDateUseDay = siUseDate;
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element, cltItemStatusArmour* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemStatusArmour.Set(pcldata);

	memcpy(&Element, element, sizeof(stElement));

	uiDateUseDay = siUseDate;
}


cltItem::cltItem(SI32 siunique, SI32 siUseDate, cltItemTool* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemTool.Set(pcldata);

	uiDateUseDay = siUseDate;
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate, cltItemBag* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemBag.Set(pcldata);

	uiDateUseDay = siUseDate;
}


//cyj  내구 생성자
/*
cltItem::cltItem(SI32 siunique, cltItemWeapon* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemWeapon.Set(pcldata);
}

cltItem::cltItem(SI32 siunique, cltItemArmour* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemArmour.Set(pcldata);
}


cltItem::cltItem(SI32 siunique, cltItemRing* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemRing.Set(pcldata);
}

cltItem::cltItem(SI32 siunique, cltItemNeck* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemNeck.Set(pcldata);
}

cltItem::cltItem(SI32 siunique, cltItemShoes* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemShoes.Set(pcldata);
}
//cyj  내구 생성자 여기까지

cltItem::cltItem(SI32 siunique, cltItemTool* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemTool.Set(pcldata);
}
*/

cltItem::cltItem(SI32 siunique, cltItemBag* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemBag.Set(pcldata);
}

cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement * element , cltItemMantle* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;


	memcpy(&Element, element, sizeof(stElement));

	clItemMantle.Set(pcldata);

	uiDateUseDay = siUseDate;
}

// 아바타 망토
cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element, cltItemAvatarMantle* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;


	memcpy(&Element, element, sizeof(stElement));

	clItemAvatarMantle.Set(pcldata);

	uiDateUseDay = siUseDate;
}

// 아바타 반지
cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element, cltItemAvatarRing* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;


	memcpy(&Element, element, sizeof(stElement));

	clItemAvatarRing.Set(pcldata);

	uiDateUseDay = siUseDate;
}

// 아바타 목걸이
cltItem::cltItem(SI32 siunique, SI32 siUseDate, cltItemAvatarNeck* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemAvatarNeck.Set(pcldata);

	uiDateUseDay = siUseDate;
}

// 아바타 의복
cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element, cltItemAvatarDress* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemAvatarDress.Set(pcldata);

	memcpy(&Element, element, sizeof(stElement));

	uiDateUseDay = siUseDate;
}

// 아바타 모자
cltItem::cltItem(SI32 siunique, SI32 siUseDate, stElement* element, cltItemAvatarHat* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemAvatarHat.Set(pcldata);

	memcpy(&Element, element, sizeof(stElement));

	uiDateUseDay = siUseDate;
}

// 아이템의 타입을 찾는다. 
SI32 cltItem::GetType(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		SI32 type = pclItemManager->pclItemInfo[ref]->siType;
		return type;
	}

	return 0;
}


//아바타 아이템의 타입을 찾는다. 
SI32 cltItem::GetAavatarType(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		SI32 type = pclItemManager->pclItemInfo[ref]->siType;

		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				return type;
				break;
			default : 
				return 0;
				break;
		}				
	}

	return 0;
}

// 아이템의 공격타입을 찾는다. 
SI32 cltItem::GetAttackType(cltItemManagerCommon* pclItemManager)const
{
	if(pclItemManager == NULL)return 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetAttackType();
	}

	return 0;
}

// 아이템의 공격주기를  찾는다. 
SI32 cltItem::GetAttackInterval(cltItemManagerCommon* pclItemManager)const
{
	if(pclItemManager == NULL)return 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetAttackInterval();
	}

	return 0;
}


// 아이템의 공격사거리를 찾는다. 
SI32 cltItem::GetAttackRange(cltItemManagerCommon* pclItemManager)const
{
	if(pclItemManager == NULL)return 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetAttackRange();
	}

	return 0;
}

// 허기를 채워주는 정도를 찾는다.
SI32 cltItem::GetHungry(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetHungry();
	}

	return 0;
}


// 치료해 줄 수 있는 질병의 유니크를 찾는다. 
SI32 cltItem::GetDisease(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetDisease();
	}

	return 0;
}

// 체력을 채워주는 정도를 찾는다.
SI32 cltItem::GetLife(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetLife();
	}

	return 0;
}

// 마법력을 채워주는 정도를 찾는다.
SI32 cltItem::GetMana(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetMana();
	}

	return 0;
}

// 도력을 구한다. 
SI32 cltItem::GetEquipMag(cltItemManagerCommon* pclItemManager,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------
	switch(type)
	{
		case ITEMTYPE_STAFF:
			temNum = clItemWeapon.uiMag;
			break;
		case ITEMTYPE_RING:
			temNum = clItemRing.uiMag;
			break;
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiMag;
			break;
		case ITEMTYPE_DRESS:
		case ITEMTYPE_HAT:
			temNum = clItemStatusArmour.uiMag;
			break;
		case ITEMTYPE_CRYSTAL:
			{	
				//KHY - 20090526 - 아바타 결정체.
				if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_MAG) ||
					(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_MAG))
				{
					temNum =  clItemCrystal.uiCrystalAbilityNum;
				}
			}
			break;
	}

	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_MAG);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_MAG);
				break;
		}				
	}

	return temNum;
}

// 근력을 구한다. 
SI32 cltItem::GetEquipStr(cltItemManagerCommon* pclItemManager ,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------
	switch(type)
	{
		case ITEMTYPE_RING:
			temNum = clItemRing.uiStr;
			break;
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiStr;
			break;
		case ITEMTYPE_MANTLE:
			temNum =  clItemMantle.uiStr;
			break;
		//case ITEMTYPE_AVATAR_MANTLE:
		//	temNum = clItemAvatarMantle.uiStr;
		//	break;
		case ITEMTYPE_DRESS:
		case ITEMTYPE_HAT:
			temNum =  clItemStatusArmour.uiStr;
			break;
		case ITEMTYPE_CRYSTAL:
			{	
				//KHY - 20090526 - 아바타 결정체.
				if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_STR) ||
					(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_STR))
				{
					temNum =  clItemCrystal.uiCrystalAbilityNum;
				}
			}
			break;
	}
	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_STR);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_STR);
				break;
		}		
		
	}
	
	return temNum;
}

// 순발력을 구한다. 
SI32 cltItem::GetEquipDex(cltItemManagerCommon* pclItemManager ,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;
	
	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------
	switch(type)
	{
		case ITEMTYPE_RING:
			temNum =  clItemRing.uiDex;
			break;
		case ITEMTYPE_MANTLE:
			temNum =  clItemMantle.uiDex;
			break;
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiDex;
			break;
		//case ITEMTYPE_AVATAR_MANTLE:
		//	temNum = clItemAvatarMantle.uiDex;
		//	break;
		case ITEMTYPE_DRESS:
		case ITEMTYPE_HAT:
			temNum =  clItemStatusArmour.uiDex;
			break;
		case ITEMTYPE_CRYSTAL:
				{	
					//KHY - 20090526 - 아바타 결정체.
					if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_DEX) ||
						(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_DEX))
					{
						temNum =  clItemCrystal.uiCrystalAbilityNum;
					}
				}
				break;
	}
	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_DEX);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_DEX);
				break;
		}		
		
	}

	return temNum;
}

// 생명력을 구한다. 
SI32 cltItem::GetEquipVit(cltItemManagerCommon* pclItemManager ,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------
	switch(type)
	{
		case ITEMTYPE_RING:
			temNum = clItemRing.uiVit;
			break;
		case ITEMTYPE_MANTLE:
			temNum = clItemMantle.uiVit;
			break;
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiVit;
			break;
		//case ITEMTYPE_AVATAR_MANTLE:
		//	temNum = clItemAvatarMantle.uiVit;
		//	break;
		case ITEMTYPE_DRESS:
		case ITEMTYPE_HAT:
			temNum = clItemStatusArmour.uiVit;
			break;
		case ITEMTYPE_CRYSTAL:
				{	
					//KHY - 20090526 - 아바타 결정체.
					if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_VIT) ||
						(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_VIT))
					{
						temNum =  clItemCrystal.uiCrystalAbilityNum;
					}
				}
				break;
	}
	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_VIT);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_VIT);
				break;
		}		
		
	}

	return temNum;
}

// 운을 구한다. 
SI32 cltItem::GetEquipLuk(cltItemManagerCommon* pclItemManager ,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------

	switch(type)
	{
		case ITEMTYPE_RING:
			temNum = clItemRing.uiLuk;
			break;
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiLuk;
			break;
		case ITEMTYPE_HAT:
		case ITEMTYPE_DRESS:
			temNum = clItemStatusArmour.uiLuk;
			break;
		case ITEMTYPE_CRYSTAL:
				{	
					//KHY - 20090526 - 아바타 결정체.
					if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_LUK) ||
						(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_LUK))
					{
						temNum =  clItemCrystal.uiCrystalAbilityNum;
					}
				}
				break;
	}
	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_LUK);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_LUK);
				break;
		}		
		
	}

	return temNum;
}

// 손재주을 구한다. 
SI32 cltItem::GetEquipHnd(cltItemManagerCommon* pclItemManager,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------
	switch(type)
	{
		case ITEMTYPE_RING:
			temNum =  clItemRing.uiHnd;
			break;
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiHnd;
			break;
		case ITEMTYPE_HAT:
			
		case ITEMTYPE_DRESS:
			temNum =  clItemStatusArmour.uiHnd;
			break;
		case ITEMTYPE_CRYSTAL:
				{	
					//KHY - 20090526 - 아바타 결정체.
					if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_HND) ||
						(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_HND))
					{
						temNum =  clItemCrystal.uiCrystalAbilityNum;
					}
				}
				break;
	}

	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_HND);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_HND);
				break;
		}		
		
	}

	return temNum;
}

// 지혜 구한다. 
SI32 cltItem::GetEquipWis(cltItemManagerCommon* pclItemManager,SI32* CrystalPer )
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 각 아이템 고유의 능력 검사.
	//---------------------------------------------------------------------------------
	switch(type)
	{
		case ITEMTYPE_AVATAR_RING:
			temNum = clItemAvatarRing.uiWis;
			break;

		case ITEMTYPE_CRYSTAL:
				{	
					//KHY - 20090526 - 아바타 결정체.
					if((clItemCrystal.uiCrystalAbility == CRYSTALABILITY_STATE_WIS) ||
						(clItemCrystal.uiCrystalAbility == CRYSTALABILITY_AVATAR_STATE_WIS))
					{
						temNum =  clItemCrystal.uiCrystalAbilityNum;
					}
				}
				break;
	}

	//---------------------------------------------------------------------------------
	// 해당아이템이 가지고 있는, 결정체의 능력 추가.
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//KHY - 20090526 - 아바타 결정체.
		switch(type)
		{
			case ITEMTYPE_AVATAR_MANTLE:
			case ITEMTYPE_AVATAR_DRESS:
			case ITEMTYPE_AVATAR_HAT:
			case ITEMTYPE_AVATAR_NECK:
			case ITEMTYPE_AVATAR_RING:
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_AVATAR_STATE_WIS);
				break;
			default : 
				* CrystalPer = GetAllCrystalAbilityNum(CRYSTALABILITY_STATE_WIS);
				break;
		}		
		
	}
	
	return temNum;
}


// 최소 데미지를 구한다. 
SI32 cltItem::GetMinDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer )
{
	if(siUnique == 0)				return 0;
	
	SI32 type = GetType(pclItemManager);


	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
		temNum += clItemWeapon.uiMinDamage;
		break;
	default:
		break;
	}

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK); //물리 최소 공격력.
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK); //물리 공격력.
	}

	return temNum;
}


// 최소 데미지를 구한다. 
SI32 cltItem::GetEnchantDamage(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;
	if( Element.siType % 2 ==0) return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{

	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_TORPEDO:
	case ITEMTYPE_RING:
		return Element.siPower;
		break;
	default:
		return 0;
	}
}


// 최소 인챈트 타입을 구한다. 
SI32 cltItem::GetEnchantType(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;
//	if(Element.siType % 2 ==0) return 0;

	SI32 type = GetType(pclItemManager);



	switch(type)
	{

	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_TORPEDO:
	//case ITEMTYPE_RING:
	case ITEMTYPE_HELMET:
	//case ITEMTYPE_HAT:		
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	//case ITEMTYPE_DRESS:
	case ITEMTYPE_SHOES:
	
		return Element.siType;
		break;
	default:
		return 0;
	}
}


// 최대 데미지를 구한다. 
SI32 cltItem::GetMaxDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
		temNum += clItemWeapon.uiMaxDamage;
		break;
	default:
		break;
	}

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK); //물리 최대 공격력.
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK); //물리 공격력.
	}

	return temNum;
}

SI32 cltItem::GetAddMagicalDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

// 기본적으로 마법 뎀쥐를 올려주는 마법 무구는 없다.
/*
	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
	case ITEMTYPE_RING:
		temNum += clItemWeapon.uiMaxDamage;
		break;
	default:
		break;
	}
*/
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_ATTACK); //마법 공격력
	}
	
	return temNum;
}

SI32 cltItem::GetAddMagicalMinDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 기본적으로 마법 뎀쥐를 올려주는 마법 무구는 없다.

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK); // 마법 최소 공격력.
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_ATTACK); //마법 공격력
	}
	
	return temNum;
}

SI32 cltItem::GetAddMagicalMaxDamage(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 기본적으로 마법 뎀쥐를 올려주는 마법 무구는 없다.

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK);// 마법  최대 공격력
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_ATTACK); //마법 공격력
	}
	
	return temNum;
}

// 필살  - 크리티컬율 향상.
SI32 cltItem::GetCriticalHitRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 기본적으로 마법 뎀쥐를 올려주는 마법 무구는 없다.

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_SPECIAL_DEATHBLOW);// 크리티컬 확률 향상.
	}
	
	return temNum;
}

SI32 cltItem::GetPhysicalHitRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 기본적으로 마법 뎀쥐를 올려주는 마법 무구는 없다.

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE);// 물리 명중률.
	}
	
	return temNum;
}

SI32 cltItem::GetMagicalHitRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_HITRATE);// 마법 명중률.
	}
	
	return temNum;
}

// 방어 능력을 구한다. 
SI32 cltItem::GetPhysicalAC(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	switch(type)
	{
		case ITEMTYPE_HELMET:
		case ITEMTYPE_BELT:
		case ITEMTYPE_ARMOUR:
			temNum += clItemArmour.uiAC;
			break;

		case ITEMTYPE_BAG:	
			temNum += clItemBag.uiAC;
			break;

		case ITEMTYPE_MANTLE:	
			temNum += clItemMantle.uiAC;
			break;

		case ITEMTYPE_AVATAR_MANTLE:
			temNum += clItemAvatarMantle.uiAC; 
			break;

		case ITEMTYPE_AVATAR_RING:
			temNum += clItemAvatarRing.uiAC; 
			break;

		case ITEMTYPE_SHOES:
			temNum += clItemShoes.uiAC;
			break;
		default:
			break;
	}
	
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE); //물리 방어력.
	}

	return temNum;	
}

// 방어 능력을 구한다. 
SI32 cltItem::GetMagicalAC(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;
// 마법 방어력을 가지고 있는 방어구는 없다.
/*
	switch(type)
	{
		case ITEMTYPE_HELMET:
		case ITEMTYPE_BELT:
		case ITEMTYPE_ARMOUR:
			temNum += clItemArmour.uiAC;
			break;

		case ITEMTYPE_BAG:	
			temNum += clItemBag.uiAC;
			break;

		case ITEMTYPE_MANTLE:	
			temNum += clItemMantle.uiAC;
			break;

		case ITEMTYPE_SHOES:
			temNum += clItemShoes.uiAC;
			break;
		default:
			break;
	}
*/
	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);
		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE); //마법 방어력.
	}

	return temNum;	
}

SI32 cltItem::GetPhysicalDodgeRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;


	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE);// 물리 회피율
	}
	
	return temNum;
}

SI32 cltItem::GetMagicalDodgeRate(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//SI32 type = GetType(pclItemManager);

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;


	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE);// 마법회피율
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalHitpoint(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_HITPOINT);// 체력.
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalManapoint(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_MANAPOINT);// 마나량..
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalWeigt(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_EFFECT_WEIGHT);// 무게량.
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalFireAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_FIREATTACK);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalWaterAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_WATERATTACK);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalWindAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_WINDATTACK);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalEarthAttack(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_EARTHATTACK);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalFireDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		*CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalWaterDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalWindDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE);
	}
	
	return temNum;
}

SI32 cltItem::GetCrystalEarthDeffence(cltItemManagerCommon* pclItemManager , SI32* CrystalPer)
{
	if(siUnique == 0)return 0;

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	if(siUseCrystalNum>0)
	{
		//UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		* CrystalPer += GetAllCrystalAbilityNum(CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE);
	}
	
	return temNum;
}

// 최소 데미지를 설정한다. 
SI32 cltItem::SetMinDamage(cltItemManagerCommon* pclItemManager, SI32 damage)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
	case ITEMTYPE_RING:

		clItemWeapon.uiMinDamage = damage;
		return clItemWeapon.uiMinDamage;
		break;
	default:
		MsgBox(TEXT("SetMinDamage()"), TEXT("unique:%d type:%d"), siUnique, type); 
		{
			SI32 a = 0;
		}
		return 0;
	}
}

// 최대 데미지를 설정한다. 
SI32 cltItem::SetMaxDamage(cltItemManagerCommon* pclItemManager, SI32 damage)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
	case ITEMTYPE_RING:
		clItemWeapon.uiMaxDamage = damage;
		return clItemWeapon.uiMaxDamage;
		break;

	default:
		MsgBox(TEXT("SetMaxDamage()"), TEXT("unique:%d type:%d"), siUnique, type);
		{
			SI32 a = 0;
		}

		return 0;
	}
}


// 방어 능력을 구한다. 
SI32 cltItem::GetAC(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_HELMET:
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
		return clItemArmour.uiAC;
		break;

	case ITEMTYPE_HAT:		
	case ITEMTYPE_DRESS:
		return clItemArmour.uiAC;
		break;

	case ITEMTYPE_BAG:	
		return clItemBag.uiAC;
		break;

	case ITEMTYPE_MANTLE:	
		return clItemMantle.uiAC;
		break;

	case ITEMTYPE_AVATAR_MANTLE:
		return clItemAvatarMantle.uiAC;
		break;

	case ITEMTYPE_AVATAR_RING:
		return clItemAvatarRing.uiAC;
		break;

	case ITEMTYPE_SHOES:
		return clItemShoes.uiAC;
		break;


	default:
		MsgBox(TEXT("GetAC()"), TEXT("unique:%d type:%d"), siUnique, type);
		{
			SI32 a = 0;
		}

		return 0;
	}
}


// 방어 능력을 구한다. 
SI32 cltItem::GetEnchantAC(cltItemManagerCommon* pclItemManager,bool bClient)const
{
	if(siUnique == 0)return 0;
	if(Element.siType % 2 ==1 ) return 0;

	SI32 type = GetType(pclItemManager);


	switch(type)
	{
	case ITEMTYPE_DRESS:
	case ITEMTYPE_HAT:		
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_MANTLE:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_RING:

		{
			return Element.siPower ;

		}
		break;	
	default:
		{
			if ( bClient )
				MsgBox("GetEnchantAC()", "unique:%d type:%d", siUnique, type);

			return 0;
		}
		break;
	}
}

// 인챈트관련 방어능력을 구한다. 
SI32 cltItem::GetEnchantACFrom(cltItemManagerCommon* pclItemManager,bool bClient, SI32 attacktype)const
{
	if(siUnique == 0)return 0;
	if(attacktype ==0)return 0;
	if(attacktype % 2 ==1) return 0; //홀수는 공격능력이다.
	if(attacktype != Element.siType) return 0;

	SI32 type = GetType(pclItemManager);
	SI32 siEnchantDef =0;	

	switch(Element.siPower)
	{
	case 0:
		siEnchantDef = 0;
		break;
	case 1:
		siEnchantDef = 5;
		break;
	case 2:
		siEnchantDef = 6;
		break;
	case 3:
		siEnchantDef = 8;
		break;
	case 4:
		siEnchantDef = 11;
		break;
	case 5:
		siEnchantDef = 15;
		break;
	case 6:
		siEnchantDef = 20;
		break;
	case 7:
		siEnchantDef = 26;
		break; 
	case 8:
		siEnchantDef = 33;
		break; 
	case 9:
		siEnchantDef = 41;
		break; 
	case 10:
		siEnchantDef = 50;
		break; 
	default:
		siEnchantDef = 0;
		break;		
	}



	switch(type)
	{
	case ITEMTYPE_DRESS:
	case ITEMTYPE_HAT:		
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_MANTLE:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_RING:
	case ITEMTYPE_AVATAR_MANTLE:	// 아바타 망토
	case ITEMTYPE_AVATAR_DRESS:		// 아바타 의복
	case ITEMTYPE_AVATAR_HAT:		// 아바타 모자
	case ITEMTYPE_AVATAR_RING:		// 아바타 반지
	case ITEMTYPE_AVATAR_NECK:		// 아바타 목걸이
		
		{
			return siEnchantDef ;			
		}
		break;	
	default:
		{
			if ( bClient )
				MsgBox("GetEnchantAC()", "unique:%d type:%d", siUnique, type);

			return 0;
		}
		break;
	}
}

// 인챈트관련 공격능력을 구한다. 
SI32 cltItem::GetEnchantAttFrom(cltItemManagerCommon* pclItemManager,bool bClient, SI32 attacktype)const
{
	if(siUnique == 0)return 0;
	if(attacktype ==0)return 0;
	if(attacktype % 2 ==0) return 0; // 짝수는 방어능력이다.
	if(attacktype != Element.siType) return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD	:
	case ITEMTYPE_SPEAR	:
	case ITEMTYPE_STAFF	:
	case ITEMTYPE_AXE	:
	case ITEMTYPE_BOW	:
	case ITEMTYPE_GUN	:
	case ITEMTYPE_CANNON	:
	case ITEMTYPE_RING:
		{
			return Element.siPower ;

		}
		break;	
	default:
		{
			if ( bClient )
				MsgBox("GetEnchantAC()", "unique:%d type:%d", siUnique, type);

			return 0;
		}
		break;
	}
}


// 방어 능력을 설정한다. 
SI32 cltItem::SetAC(cltItemManagerCommon* pclItemManager, SI32 ac)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_HELMET:
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
		clItemArmour.uiAC = ac;
		return clItemArmour.uiAC;
		break;

	case ITEMTYPE_HAT:		
	case ITEMTYPE_DRESS:
	//	clItemStatusArmour.uiAC = ac;
	//	return clItemStatusArmour.uiAC;
		return 0;
		break;

	case ITEMTYPE_BAG:	
		clItemBag.uiAC = ac;
		return clItemBag.uiAC;
		break;

	case ITEMTYPE_MANTLE:	
		clItemMantle.uiAC	= ac;
		return clItemMantle.uiAC;
		break;

	case ITEMTYPE_AVATAR_MANTLE:
		clItemAvatarMantle.uiAC	= ac;
		return clItemAvatarMantle.uiAC;
		break;

	case ITEMTYPE_SHOES:
		clItemShoes.uiAC	= ac;
		return clItemShoes.uiAC;
		break;

	default:
		MsgBox(TEXT("SetAC()"), TEXT("unique:%d type:%d"), siUnique, type);
		{
			SI32 a = 0;
		}

		return 0;
	}
}


// 특정 기술의 부적에 의한 상승 효과를 구한다. 
SI32 cltItem::GetSkillScrollUp(cltItemManagerCommon* pclItemManager, SI32 skillunique)
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)return 0;

	SI32 index = 0;

	while( skilldata[index*3] )
	{
		if(skillunique == skilldata[index*3])
		{
			UI32 para = 0 ;
			if( clItemCommon.clOptionInfo.uiType1 == (UI32)skilldata[index*3 + 1] ||
				clItemCommon.clOptionInfo.uiType1 == (UI32)skilldata[index*3 + 2])
			{
				//return clItemCommon.clOptionInfo.uiPara1 / 10;
				para += clItemCommon.clOptionInfo.uiPara1 / 10 ;
			}

			if(clItemCommon.clOptionInfo.uiType2 == (UI32)skilldata[index*3 + 1] ||
				clItemCommon.clOptionInfo.uiType2 == (UI32)skilldata[index*3 + 2])
			{
				//return clItemCommon.clOptionInfo.uiPara2 / 10;
				para += clItemCommon.clOptionInfo.uiPara2 / 10 ;
			}

			if ( para > 0 )
			{
				return para ;
			}
		}

		index++;
	}

	return 0;

}


// 기술력 향상을 구한다. 
SI32 cltItem::GetUpgradeSkillLevel(cltItemManagerCommon* pclItemManager, SI32 skillunique  , bool countCrystalAbilityNum)
{
	if(siUnique == 0)return 0;
	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	SI32 temNum = 0;

	//---------------------------------------------------------
	// 부적 부착에 의한 효과를 구한다. - 부적에 관한 효과 삭제. 
	//---------------------------------------------------------
	temNum += GetSkillScrollUp(pclItemManager, skillunique);
	//if(up)return up;

	//--------------------------------------------
	// 고유의 성능향상을 구한다. 
	//--------------------------------------------
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)return 0;

	// 슬롯을 검사하여 결정체의 능력을 더해준다.
	// 슬롯이 있고, 슬롯에 관한 계산을 해야 할 경우만. - 전체 스킬을 계산 할때는 결정체는 따로 따로 계산 해야만 한다.
	if((siUseCrystalNum>0) && (countCrystalAbilityNum == true))
	{
		UI08 uiSkillunique = GetCrystalAbilityFromSkillUnique(skillunique);

		temNum += GetAllCrystalAbilityNum(uiSkillunique);
	}


	SI32 type = GetType(pclItemManager);
	// 아바타 아이템인지 검사한다.
	SI32 siAvatarItem = GetAavatarType(pclItemManager);


	// 아바타 아이템인지를 먼저 찾는다.
	if(siAvatarItem >0) 
	{
		switch(type)
		{
		case ITEMTYPE_AVATAR_NECK:
			{
				if(skillunique == (SI32)clItemAvatarNeck.uiUpgradeSkillUnique)
				{
					temNum += clItemAvatarNeck.uiSkillValue;
				}
				else if(ALL_MAKESKILL_UNIQUE == clItemAvatarNeck.uiUpgradeSkillUnique)
				{
					if( ( skillunique >= SKILL_MAKEARMOUR1 && skillunique <= SKILL_MAKEDRESS1 ) ||skillunique == SKILL_MAKEHAT1 )
						temNum += clItemAvatarNeck.uiSkillValue;			
				}
				else if(ALL_WEAPONSKILL_UNIQUE == clItemAvatarNeck.uiUpgradeSkillUnique)
				{
					if ( skillunique >= SKILL_USESWORD1 && skillunique <= SKILL_USESTAFF1 )
						temNum += clItemAvatarNeck.uiSkillValue;			
				}
				else if(ALL_PRODUCTSKILL_UNIQUE == clItemAvatarNeck.uiUpgradeSkillUnique)
				{
					if( skillunique == SKILL_HORSETRAINING || skillunique == SKILL_FARM ||
						skillunique == SKILL_MINE		 || skillunique == SKILL_FISH ||
						skillunique == SKILL_MAKEIRON1	 || skillunique == SKILL_MAKETOOL1 ||
						skillunique == SKILL_MAKEFOOD || skillunique == SKILL_MAKEMED ||
						skillunique == SKILL_ARTIFACT)
						temNum += clItemAvatarNeck.uiSkillValue;			
				}
				else if(ALL_ALLSKILL_UNIQUE == clItemAvatarNeck.uiUpgradeSkillUnique)
				{
					if( (skillunique >= SKILL_MAKEMED && skillunique < SKILL_SWORD_INCDR)|| 
					      skillunique == SKILL_MAKETOOL1 ||skillunique == SKILL_ARTIFACT  ||
					      (skillunique >= SKILL_MAKEAVATAR_DRESS && skillunique <= SKILL_MAKEAVATAR_NECK))
						temNum += clItemAvatarNeck.uiSkillValue;			
				}
				else if(ALL_ALLAVATARSKILL_UNIQUE == clItemAvatarNeck.uiUpgradeSkillUnique)
				{
					if( skillunique >= SKILL_MAKEAVATAR_DRESS && skillunique <= SKILL_MAKEAVATAR_NECK)
						temNum += clItemAvatarNeck.uiSkillValue;			
				}
			}
			break;
		// 모자하고, 의복 추가 될때는 함수로 만들자.  - 지금은 졸리다.
		/*
		case ITEMTYPE_AVATAR_HAT:		
			{
				if(skillunique == pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique)
					temNum += clItemAvatarHat.uiSkillUp;
			}
		case ITEMTYPE_AVATAR_DRESS:
			{
				if(skillunique == pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique)
					temNum += clItemAvatarDress.uiSkillUp;
			}
			break;
		*/
		}

		return temNum;		
	}
	else if((skillunique == pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique) && (siAvatarItem <= 0))
	{
		switch(type)
		{
		case ITEMTYPE_HELMET:
		case ITEMTYPE_BELT:
		case ITEMTYPE_ARMOUR:
			temNum += clItemArmour.uiSkillUp;
			break;
		case ITEMTYPE_HAT:		
		case ITEMTYPE_DRESS:
			temNum += clItemStatusArmour.uiSkillUp;
			break;
		case ITEMTYPE_NECK:
			temNum += clItemNeck.uiSkillPointUp;
			break;		
		}

		return temNum;
	}
	// 제조부적 전부 바른 것과 같은 효과
	else if ((pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique == ALL_MAKESKILL_UNIQUE) && (siAvatarItem <= 0) )
	{
		if( ( skillunique >= SKILL_MAKEARMOUR1 && skillunique <= SKILL_MAKEDRESS1 ) ||
			/*skillunique == SKILL_MAKENECK1 ||*/ skillunique == SKILL_MAKEHAT1 )
		{
			switch(type)
			{
			case ITEMTYPE_HAT:
			case ITEMTYPE_DRESS:
				{
					temNum +=  clItemStatusArmour.uiSkillUp;
				}
				break;
			case ITEMTYPE_NECK:
				temNum +=  clItemNeck.uiSkillPointUp;
				break;
			}

			return temNum;			
		}
	}
	// 무기사용술부적 전부 바른 것과 같은 효과
	else if ((pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique == ALL_WEAPONSKILL_UNIQUE) && (siAvatarItem <= 0) )
	{
		if ( skillunique >= SKILL_USESWORD1 && skillunique <= SKILL_USESTAFF1 )
		{
			switch(type)
			{	
			case ITEMTYPE_HAT:
			case ITEMTYPE_DRESS:
				{
					temNum += clItemStatusArmour.uiSkillUp;
				}
				break;
			case ITEMTYPE_NECK:
				temNum += clItemNeck.uiSkillPointUp;
				break;
			}
		}

		return temNum;
	}
	// 생산기술부적 전부 바른 것과 같은 효과
	else if ( (pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique == ALL_PRODUCTSKILL_UNIQUE) && (siAvatarItem <= 0) )
	{
		if( skillunique == SKILL_HORSETRAINING || skillunique == SKILL_FARM ||
			skillunique == SKILL_MINE		 || skillunique == SKILL_FISH ||
			skillunique == SKILL_MAKEIRON1	 || skillunique == SKILL_MAKETOOL1 ||
			skillunique == SKILL_MAKEFOOD || skillunique == SKILL_MAKEMED ||
			skillunique == SKILL_ARTIFACT)
		{
			switch(type)
			{
			case ITEMTYPE_HAT:
			case ITEMTYPE_DRESS:
				{
					temNum += clItemStatusArmour.uiSkillUp;
				}
				break;
			case ITEMTYPE_NECK:
				temNum += clItemNeck.uiSkillPointUp;
				break;
			}
		}

		return temNum;

	}
	// 모든스킬
	else if ((pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique == ALL_ALLSKILL_UNIQUE) && (siAvatarItem <= 0) )
	{
		// 제조술부터 무기향상술 전까지의 모든 스킬
		if( (skillunique >= SKILL_MAKEMED && skillunique < SKILL_SWORD_INCDR)|| 
		      skillunique == SKILL_MAKETOOL1 ||skillunique == SKILL_ARTIFACT ||
		      (skillunique >= SKILL_MAKEAVATAR_DRESS && skillunique <= SKILL_MAKEAVATAR_NECK))
		{
			switch(type)
			{
			case ITEMTYPE_HAT:
			case ITEMTYPE_DRESS:
				{
					temNum += clItemStatusArmour.uiSkillUp;
				}
				break;
			case ITEMTYPE_NECK:
				temNum += clItemNeck.uiSkillPointUp;
				break;
			}
		}

		return temNum;

	}

	if(temNum >0 )
		return temNum;
	else
		return 0;
}

// 운을 구한다. 
SI32 cltItem::GetLuck(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_HELMET:
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	//case ITEMTYPE_HAT:
	//case ITEMTYPE_DRESS:
		return clItemArmour.uiLuck;
		break;
	default:
		return 0;
	}

//	return 0;
}


//----------------------------------------
// 이동속도 관련 
//----------------------------------------
SI32 cltItem::GetMoveSpeed(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SHOES:
		return clItemShoes.uiMoveSpeed;
		break;
	default:
		MsgBox(TEXT("fdsmv8jfd"), TEXT("fmcvijsd:%d"), type);
		break;
	}

	return 0;
}

//----------------------------------------
// 내구성 관련. 
//----------------------------------------
//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 내구도 관련.

// 내구성을 줄인다. 
bool cltItem::DecreaseCrystalDurability(cltItemManagerCommon* pclItemManager, SI16 durvary)
{
	// 내구도가 감소되는 아이템인지 확인한다. 
	bool reVal = false;

	// 일단 슬롯이 있어야 한다.
	if(GetUseCrystalNum() >0)
	{
		// 해당 아이템의 모든 결정체 내구도를 감소시킨다.
		for(int i = 0; i <MAX_CRYSTAL_NUM ; i++ )
		{		
			if(cltItemCrytal[i].GetCrystalType() != 0)
			{
				// 최대 내구도가 0이면 - 해당 결정체 데이터 삭제. - 파괴. 
				if(cltItemCrytal[i].GetDurability() <= 0)
				{
					cltItemCrytal[i].Init();
					reVal = true;
				}
				else if(cltItemCrytal[i].GetDurability() > 0)
				{
					SI16 randval = rand()%100;

					if(randval < CRYSTAL_DECREASE_DURABILITY_PRO) //  확률로 감소.
					{
						cltItemCrytal[i].DecDurability(durvary);				
					}
					reVal = true;
				}
			}
		}
	}
	return reVal;
}

// 크리스탈의 내구도가 1%단위로 줄어, Client로 보내야 하는지 여부를 결정한다. - 계산 안한다.
bool cltItem::GetDecreaseCrystalDurabilityPer(cltItemManagerCommon* pclItemManager)
{
	// 내구도가 감소되는 아이템인지 확인한다. 
	bool reVal = false;

	// 일단 슬롯이 있어야 한다.
	if(GetUseCrystalNum() >0)
	{
		return true; // 걍 다 보낸다.
	
		/*
		// 해당 아이템의 모든 결정체 내구도를 감소시킨다.
		for(int i = 0; i <MAX_CRYSTAL_NUM ; i++ )
		{		
			if(cltItemCrytal[i].GetCrystalType() != 0)
			{
				SI16 canDurPer = cltItemCrytal[i].GetDurability()%300; // 300 = 1% 이다.
				SI16 nowDurPer = (cltItemCrytal[i].GetDurability()*100)/30000; // 현재 내구도 %.
								
				if(nowDurPer == 100)
				{
					return false; // 100% 일때는 보내지 않는다.
				}
				else if((cltItemCrytal[i].GetDurability() > 29989) && (cltItemCrytal[i].GetDurability() < 30000))
				{
					return true; // 99% 로 변하는 것만 예외로 처리한다. - 1번만 보내게 하려구.(어치피 5분에 한번꼴로 들어온다)				
				}
				else if(canDurPer == 0) // 1% 단위로 바뀌었다면.				
				{
					return true; // 하나라도 변했다면, 어차피 해당 아이템의 전체 정보를 보내야한다.
				}
				
			}
		}
		*/
	}
	return reVal;
}
//------------------------------------------------------------------------------


REAL32 cltItem::GetDurabilityPenlty( cltItemManagerCommon* pclItemManager )
{
//================================================================
// DBMANAGER 인경우는 사용하지 않는다.
//================================================================
#ifdef _DBMANAGER
	return 0.0f;;
#else
	SI32 siForeverDurability	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
	SI32 siForeverDurability2	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

	SI32 siGetMaxDurability		= GetMaxDurability();
	if( (siGetMaxDurability == siForeverDurability) || (siGetMaxDurability == siForeverDurability2) )
	{
		return 1.0f;
	}

	if ( NULL == pclItemManager )
	{
		return 0.0f;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if ( 0 >= ref )
	{
		return 0.0f;
	}

	if ( 0 < siGetMaxDurability )
	{
		REAL32 fResourceDurability	= (REAL32)pclItemManager->pclItemInfo[ref]->clItem.GetMaxDurability( pclItemManager );
		REAL32 fMaxDurability		= (REAL32)GetMaxDurability();

		return min( 1.0f, (fMaxDurability/fResourceDurability) );
	}

	return 0.0f;
#endif
}

// 내구도를 설정한다. 
SI32 cltItem::SetDurability(cltItemManagerCommon* pclItemManager, UI32 dur)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:

		clItemWeapon.clCommonInfo.uiDurability = dur;
		return clItemWeapon.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_BELT:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_ARMOUR:
		clItemArmour.clCommonInfo.uiDurability		= dur;
		return clItemArmour.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_HAT:
	case ITEMTYPE_DRESS:
		clItemStatusArmour.clCommonInfo.uiDurability		= dur;
		return clItemStatusArmour.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_BAG:
		clItemBag.clCommonInfo.uiDurability		= dur;
		return clItemBag.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_MANTLE:
		clItemMantle.clCommonInfo.uiDurability		= dur;
		return clItemMantle.clCommonInfo.uiDurability;
		break;

	// 내구도를 설정한다
	case ITEMTYPE_AVATAR_MANTLE:
		clItemAvatarMantle.clCommonInfo.uiDurability	= dur;
		return clItemMantle.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_AVATAR_NECK:
		clItemAvatarNeck.clCommonInfo.uiDurability	= dur;
		return clItemAvatarNeck.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_AVATAR_DRESS:
		clItemAvatarDress.clCommonInfo.uiDurability	= dur;
		return clItemAvatarDress.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_AVATAR_HAT:
		clItemAvatarHat.clCommonInfo.uiDurability	= dur;
		return clItemAvatarHat.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_AVATAR_RING:
		clItemAvatarRing.clCommonInfo.uiDurability	= dur;
		return clItemAvatarRing.clCommonInfo.uiDurability;
		break; 



	case ITEMTYPE_SHOES:
		clItemShoes.clCommonInfo.uiDurability		= dur;
		return clItemShoes.clCommonInfo.uiDurability;
		break;
	case ITEMTYPE_RING:
		clItemRing.clCommonInfo.uiDurability		= dur;
		return clItemRing.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_NECK:
		clItemNeck.clCommonInfo.uiDurability		= dur;
		return clItemNeck.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_IRON:
	case ITEMTYPE_DISH:
	case ITEMTYPE_PAPER:
	case ITEMTYPE_FISHINGROD:
		clItemTool.clCommonInfo.uiDurability		= dur;
		return clItemTool.clCommonInfo.uiDurability;
		break;
	}

	clItemCommon.clCommonInfo.uiMaxDurability = dur;
	return clItemCommon.clCommonInfo.uiMaxDurability;

}


// 최대 내구도를 구한다. 
SI32 cltItem::GetMaxDurability(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
		return clItemWeapon.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_BELT:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_ARMOUR:
		return clItemArmour.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_HAT:
	case ITEMTYPE_DRESS:
		return clItemStatusArmour.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_BAG:
		return clItemBag.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_MANTLE:
		return clItemMantle.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_MANTLE:
		return clItemAvatarMantle.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_NECK:
		return clItemAvatarNeck.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_DRESS:
		return clItemAvatarDress.clCommonInfo.uiMaxDurability;
		break;


	case ITEMTYPE_AVATAR_HAT:
		return clItemAvatarHat.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_RING:
		return clItemAvatarRing.clCommonInfo.uiMaxDurability;
		break;
		

	case ITEMTYPE_SHOES:
		return clItemShoes.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_RING:
		return clItemRing.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_NECK:
		return clItemNeck.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_IRON:
	case ITEMTYPE_DISH:
	case ITEMTYPE_PAPER:
	case ITEMTYPE_FISHINGROD:
		return clItemTool.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_CRYSTAL:
		return clItemCrystal.clCommonInfo.uiMaxDurability;
		break;
	}

	return clItemCommon.clCommonInfo.uiMaxDurability;
}

// 최대내구도를 설정한다. 
SI32 cltItem::SetMaxDurability(cltItemManagerCommon* pclItemManager, UI32 dur)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
		clItemWeapon.clCommonInfo.uiMaxDurability = dur;
		return clItemWeapon.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_BELT:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_ARMOUR:
		clItemArmour.clCommonInfo.uiMaxDurability		= dur;
		return clItemArmour.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_HAT:
	case ITEMTYPE_DRESS:
		clItemStatusArmour.clCommonInfo.uiMaxDurability		= dur;
		return clItemStatusArmour.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_BAG:
		clItemBag.clCommonInfo.uiMaxDurability		= dur;
		return clItemBag.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_MANTLE:
		clItemMantle.clCommonInfo.uiMaxDurability		= dur;
		return clItemMantle.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_MANTLE:
		clItemAvatarMantle.clCommonInfo.uiMaxDurability	= dur;
		return clItemAvatarMantle.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_NECK:
		clItemAvatarNeck.clCommonInfo.uiMaxDurability	= dur;
		return clItemAvatarNeck.clCommonInfo.uiMaxDurability;
		break;


	case ITEMTYPE_AVATAR_DRESS:
		clItemAvatarDress.clCommonInfo.uiMaxDurability	= dur;
		return clItemAvatarDress.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_HAT:
		clItemAvatarHat.clCommonInfo.uiMaxDurability	= dur;
		return clItemAvatarHat.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_AVATAR_RING:
		clItemAvatarRing.clCommonInfo.uiMaxDurability	= dur;
		return clItemAvatarRing.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_SHOES:
		clItemShoes.clCommonInfo.uiMaxDurability		= dur;
		return clItemShoes.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_RING:
		clItemRing.clCommonInfo.uiMaxDurability		= dur;
		return clItemRing.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_NECK:
		clItemNeck.clCommonInfo.uiMaxDurability		= dur;
		return clItemNeck.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_IRON:
	case ITEMTYPE_DISH:
	case ITEMTYPE_PAPER:
	case ITEMTYPE_FISHINGROD:
		clItemTool.clCommonInfo.uiMaxDurability		= dur;
		return clItemTool.clCommonInfo.uiMaxDurability;
		break;
	}

	clItemCommon.clCommonInfo.uiMaxDurability = dur;
	return clItemCommon.clCommonInfo.uiMaxDurability;

}

// 내구성을 줄인다. 
bool cltItem::DecreaseDurability(cltItemManagerCommon* pclItemManager, SI32 durvary)
{
#ifndef _DBMANAGER

	SI32 type = GetType(pclItemManager);
	if( type <= 0 ) 														return false;

	SI64 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;
	if( (atb & ITEMATB_OBJECT) == 0)										return false;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)															return false;

	SI32 siForeverDurability  = (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
	SI32 siForeverDurability2 = (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

	SI32 maxdur = GetMaxDurability();
	if( maxdur == siForeverDurability || maxdur == siForeverDurability2 )	return false;

#ifdef _DEBUG
	SI32 siBefore = GetDurability();
#endif //_DEBUG

	SI32 dur = GetDurability();
	dur -= durvary;
	if( dur < 0 )	dur = 0;

	SetDurability(pclItemManager, dur);

#ifdef _DEBUG
	SI32 siLater = GetDurability();
#endif //_DEBUG

#ifdef _DEBUG
	NTCHARString64 kMsg;
	kMsg += "DecreaseDurability ";
	kMsg += "Unique[";
	kMsg += SI16ToString( siUnique );
	kMsg += "] Before [";
	kMsg += SI32ToString( siBefore );
	kMsg += "] Later [";
	kMsg += SI32ToString( siLater );
	kMsg += "]\n";
	OutputDebugString( kMsg );
#endif //_DEBUG

#endif //_DBMANAGER
	return true;
}




// 아이템이 제 정보를 제대로 받았는지 확인한다.
BOOL cltItem::CheckType(cltItemManagerCommon* pclItemManager, SI32 orgtype)
{
	SI32 type = GetType(pclItemManager);

	if(type != orgtype)
	{
		MsgBox(TEXT("cltItem0"), TEXT("unique:%d type:%d"), siUnique, type);
		return FALSE;
	}

	return TRUE;
}

SI32 cltItem::GetImage(cltItemManagerCommon* pclItemManager)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->siImageIndex;
	}

	return 0;
}

const TCHAR* cltItem::GetShortName(cltItemManagerCommon* pclItemManager)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetName();
	}

	return NULL;
}

bool cltItem::GetFullName(cltItemManagerCommon* pclItemManager, TCHAR* pname, SI16 txtSize)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		StringCchCopy(pname, txtSize, pclItemManager->pclItemInfo[ref]->GetName());

		if(clItemCommon.clCommonInfo.uiRareInfo)
		{
			switch(clItemCommon.clCommonInfo.uiRareInfo)
			{
			case  RARELEVEL_1:		// 명품
				{
					TCHAR* pText = GetTxtFromMgr(714);
					StringCchCat(pname, txtSize, pText);
				}break;
			case  RARELEVEL_2:		// 호품
				{
					TCHAR* pText = GetTxtFromMgr(715);
					StringCchCat(pname, txtSize, pText);
				}break;
			case  RARELEVEL_5:		// 고급품 - by LEEKH 2008-02-04
				{
					TCHAR* pText = GetTxtFromMgr(716);
					StringCchCat(pname, txtSize, pText);
				}break;
			case  RARELEVEL_6:		// 고물품
				{
					TCHAR* pText = GetTxtFromMgr(10200);
					StringCchCat(pname, txtSize, pText);
				}break;	
			default:
				{
					// 기본 처리 없음. - by LEEKH 2008-02-04
					//TCHAR* pText = GetTxtFromMgr(716);
					//StringCchCat(pname, txtSize, pText);	
				}break;
			}
		}
		return true;

	}

	return false;
}

// 아이템의 이름과 개수를 얻어온다. 
bool cltItem::GetFullNameNNum(cltItemManagerCommon* pclItemManager, TCHAR* pname, SI16 txtSize )
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		StringCchCopy(pname, txtSize, pclItemManager->pclItemInfo[ref]->GetName());

		if(clItemCommon.clCommonInfo.uiRareInfo)
		{
			switch(clItemCommon.clCommonInfo.uiRareInfo)
			{
			case  RARELEVEL_1:
				{
					TCHAR* pText = GetTxtFromMgr(714);
					StringCchCat(pname, txtSize, pText);
				}break;
			case  RARELEVEL_2:
				{
					TCHAR* pText = GetTxtFromMgr(715);
					StringCchCat(pname, txtSize, pText);
				}break;
			case  RARELEVEL_6:
				{
					TCHAR* pText = GetTxtFromMgr(10200);
					StringCchCat(pname, txtSize, pText);
				}break;
			default:
				{
					TCHAR* pText = GetTxtFromMgr(716);
					StringCchCat(pname, txtSize, pText);	
				}break;
			}
		}

		TCHAR temp[256];
		TCHAR* pText = GetTxtFromMgr(717);
		StringCchPrintf(temp, 256, pText, siItemNum);
		StringCchCat(pname, txtSize, temp);
		return true;

	}

	return false;
}



TCHAR* cltItem::GetArmCode(cltItemManagerCommon* pclItemManager)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		if(_tcscmp(pclItemManager->pclItemInfo[ref]->szArmCode, TEXT("")) == 0)
		{
			return NULL;
		}
		else
		{
			return pclItemManager->pclItemInfo[ref]->szArmCode;
		}
	}
	return NULL;
}

// 아이템에다가 아이템을 더한다. 
bool cltItem::Add(cltItemManagerCommon* pclItemManager, cltItem* pclitem)
{
	if(siUnique == 0)
	{
		Set(pclitem);
		return true;
	}
	else 
	{
		if(siUnique != pclitem->siUnique)return false;
		if(siItemNum >= MAX_ITEM_PILE_NUMBER)return false;
		if(CanPile(pclItemManager) == false)return false;

		Increase(pclitem->siItemNum);
		return true;
	}

//	return false;
}

// 아이템이 같은 것인지 비교하는 함수. 
// numcomp : 개수까지 같아야 하는지 여부. 
BOOL cltItem::IsSame(cltItem* pclitem, bool numcomp)
{
	if( pclitem == NULL )		return FALSE;

	SI32 i = 0;

	if(pclitem->siUnique != siUnique)				return FALSE;
	if(numcomp)
	{
		if(pclitem->siItemNum != siItemNum)			return FALSE;
	}

	for(i = 0;i < MAX_ITEM_UNION_BYTE/4;i++)
	{
		if(pclitem->siData32[i] != siData32[i])		return FALSE;
	}

	//cyj 아이템 비교시 기간도 체크하도록 추가
	// 기간도 체크한다.
	if ( uiDateYear != pclitem->uiDateYear )		return FALSE ;
	if ( uiDateMonth != pclitem->uiDateMonth )		return FALSE ;
	if ( uiDateDay != pclitem->uiDateDay )			return FALSE ;
	if ( uiDateUseDay != pclitem->uiDateUseDay )	return FALSE ;

	//[영진] 속성도 체크한다
	if ( Element.siType != pclitem->Element.siType )	return FALSE;
	if ( Element.siPower != pclitem->Element.siPower )  return FALSE;

	//[강형] SID 비교
	if( siSID != pclitem->siSID )						return FALSE;

	//[강형] 크리스탈 비교
	if( siUseCrystalNum != pclitem->siUseCrystalNum )	return FALSE;
	for( i=0; i<MAX_CRYSTAL_NUM; i++ )
	{
		if( cltItemCrytal[i].IsSame( &pclitem->cltItemCrytal[i] ) == FALSE )		return FALSE;
	}

	return TRUE;
}

BOOL cltItem::IsBulletItem()//총알아이템장착
//과금 총알 장착 가능하도록 추가 [성웅]
{
	if (siUnique == ITEMUNIQUE(7850) || siUnique == ITEMUNIQUE(7855) 
		|| siUnique == ITEMUNIQUE(13600) || siUnique == ITEMUNIQUE(13601)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

// 아이템의 수를 늘인다. 
SI32 cltItem::Increase(SI32 amount)
{

	if(siItemNum + amount > MAX_ITEM_PILE_NUMBER)
	{
		siItemNum  = MAX_ITEM_PILE_NUMBER;
	}
	else
	{
		siItemNum += amount;
	}

	return siItemNum;
}

// 아이템의 수를 줄인다. 
SI32 cltItem::Decrease(SI32 amount)
{
	if(siItemNum < amount)return siItemNum;

	siItemNum -= amount;

	if(siItemNum == 0)
	{
		Init();
	}
	if (amount == 0)return siItemNum;

	return siItemNum;
}

// 무게를 구한다.개수에 따라 무게도 늘어난다.
SI32 cltItem::GetWeight(cltItemManagerCommon* pclItemManager, BOOL mode)
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		// 개수만큼 무게를 확인한다.
		if(mode == TRUE)
		{
			return (pclItemManager->pclItemInfo[ref]->siWeight * siItemNum);
		}
		// 한개의 무게만 얻어온다.
		else
		{
			return pclItemManager->pclItemInfo[ref]->siWeight;
		}

	}
	else
		return 0;

}


//연마시키는 스킬의 유니크
SI32 cltItem::GetSkillUnique(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetSkillUnique();
	}

	return 0;
}

//기술의 레벨 제한. 
SI32 cltItem::GetSkillLevelLimit(cltItemManagerCommon* pclItemManager)const
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetSkillLevelLimit();
	}

	return 0;
}


// 중첩이 되는 아이템인지 확인한다. 
bool cltItem::CanPile(cltItemManagerCommon* pclItemManager)
{
	if(pclItemManager == NULL)		return false;

	// 기간제 아이템은 겹칠 수 없다	- By LEEKH 2007.06.19
	if(IsHaveUseDay() == TRUE)				return false;

	// 중첩이 안되는 아이템인지 확인한다. 
	SI32 type = GetType(pclItemManager);
	if(type)
	{
		// 아티펙트 개선전의 아이템 - 결정체가 능력을 가지고 있는 아이템
		if( type == ITEMTYPE_CRYSTAL )
		{
			if( clItemCrystal.uiCrystalAbility != 0 || clItemCrystal.uiCrystalAbilityNum != 0 )
				return false;
		}

		SI64 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;

		// ITEMATB_OBJECT이 아니면 중첩을 허용한다. 

		if((atb & ITEMATB_OBJECT) == 0)
		{
			return true;
		}
	}

	return false;
}

// 수리가 가능한 아이템인지 확인한다.
bool cltItem::CanRepair(cltItemManagerCommon* pclItemManager)
{
	// 중첩이 안되는 아이템인지 확인한다. 
	SI32 type = GetType(pclItemManager);
	if(type)
	{
		SI64 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;

		if((atb & ITEMATB_REPAIR))
		{
			return true;
		}
	}

	return false;
}

// 이 아이템을 만드는데 필요한 스킬이 무엇인지 확인한다. 
SI32 cltItem::GetMakeSkill(cltItemManagerCommon* pclItemManager)
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
	}

	return 0;

}

// 이 무기의 기본 명중률을 구한다. 
SI32 cltItem::GetDefaultHitRate(cltItemManagerCommon* pclItemManager)
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->siDefaultHitRate;
	}

	return 0;

}

// 아이템을 수리한다. 
bool cltItem::Repair(cltItemManagerCommon* pclItemManager, SI32 reducedur)
{
	// 수리 불가능한 아이템이어도 실패.
	if(CanRepair(pclItemManager) == false)return false;

	// 내구도가 닳지 않은 상태에서도 수리 불가. 
	SI32 curdur = GetDurability();
	SI32 maxdur	= GetMaxDurability(pclItemManager);

	if(curdur >= maxdur)return false;

	maxdur -= reducedur; // -> 스킬에 따라 내구도 감소가 줄어든다. 
	if(maxdur <= 0)
	{
		// 수리 불가. 
		return false;
	}

	SetMaxDurability(pclItemManager, maxdur);
	SetDurability(pclItemManager, maxdur);

	return true;

}

//=============================================================================
//  @Name   IsSealingUp()
//! @brief  아이템이 봉인되어있는지 체크한다.
//=============================================================================
bool cltItem::IsSealingUp(cltItemManagerCommon* pclItemManager)
{
//================================================================
// DBMANAGER 인경우는 사용하지 않는다.
//================================================================
#ifdef _DBMANAGER
	return false;
#else
//================================================================
	SI32 type = GetType(pclItemManager);
	if(type)
	{
		SI64 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;
		if( (atb & ITEMATB_OBJECT) == 0)return false;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)return false;

	if ( ref )
	{
		SI32 atb = pclItemManager->pclItemInfo[ref]->siItemInfoAtb ;

		if (pclClient->IsCountrySwitch(Switch_NotScailed))
		{	
			//30레벨 이하 아이템은 봉인 안되어서 생성.[2007.07.23]
			SI32 slv = pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
			SI32 eq = pclItemManager->pclItemInfo[ref]->clCondition.siStr;
			if (slv !=NULL && slv < 30 ) {
				return false;
			}
		}

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
		// 결정체는 봉인 안되어서 생성.
		if(type == ITEMTYPE_CRYSTAL)
			return false;
		
		//if( ITEMTYPE_BAG != pclItemManager->pclItemInfo[ref]->siType )
		//{
		//	if( (atb & ITEMINFOATB_ITEMMALL) != 0)
		//		return false;
		//}
	}

	if ( uiDateYear == 0 && uiDateMonth == 0 && uiDateDay == 0 )
	{
		return true ;
	}
	else return false ;

//================================================================
#endif
//================================================================
}

//=============================================================================
//  @Name   IsBelonging()
//! @brief  아이템 귀속 여부를 체크한다.
//=============================================================================
bool cltItem::IsBelonging(cltItemManagerCommon* pclItemManager)
{
//================================================================
// DBMANAGER 인경우는 사용하지 않는다.
//================================================================
#ifdef _DBMANAGER
	return false;
#else
//================================================================

	SI32 siGetItemType = GetType(pclItemManager);
	if ( siGetItemType )
	{
		SI64 siGetATB = pclItemManager->pclItemTypeInfo[siGetItemType]->siAtb;
		if ( (siGetATB & ITEMATB_OBJECT) == 0 )
		{
			return false;
		}
	}

	SI32 siGetRef = pclItemManager->FindItemRefFromUnique(siUnique);
	if ( siGetRef <= 0 )
	{
		return false;
	}

	// 내구도 사용시 최대 내구도값이 무한이 아니면 무조건 귀속이 아님
	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		SI32 siForeverDurability	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
		SI32 siForeverDurability2	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

		SI32 siGetMaxDurability		= GetMaxDurability();
		if( (siGetMaxDurability != siForeverDurability) && (siGetMaxDurability != siForeverDurability2) )
		{
			return false;
		}
	}

/*	if ( ref )
	{
		SI32 atb = pclItemManager->pclItemInfo[ref]->siItemInfoAtb ;
        if( (atb & ITEMINFOATB_ITEMMALL) != 0){
			return false;
		}
	}
*/
	//cyj 날짜가 있어도 밀봉 풀면 귀속~~!
	//if ( uiDateYear > 0 && uiDateMonth > 0 && uiDateDay > 0 && uiDateUseDay == 0 )
	if ( uiDateYear > 0 && uiDateMonth > 0 && uiDateDay > 0 )
	{

		return true ;
	}
	 
	return false ;
//================================================================
#endif
//================================================================
}

//=============================================================================
//  @Name   IsDisassemble()
//! @brief  이호식에서 분해 가능한가?
//=============================================================================
bool cltItem::IsDisassemble(cltItemManagerCommon* pclItemManager)
{
//================================================================
// DBMANAGER 인경우는 사용하지 않는다.
//================================================================
#ifdef _DBMANAGER
	return false;
#else
//================================================================
	
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)																				return false;

	//=============================================================================
	// 분해 안되는 조건
	// tys[06/04/09] - 20 레벨 이상 물품만 분해 가능 - 국가 옵션 사용
	SI32 equipLevel = pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
	if ( equipLevel <= 20 && pclClient->IsCountrySwitch(Switch_WENNYDISASSEMBLELEVEL) )	return false;

	SI32 itemtype = pclItemManager->GetItemType(siUnique);
	if ( pclItemManager->IsItemInfoAtb(siUnique, ITEMINFOATB_ITEMMALL) )
	{
		if ( pclClient->IsCountrySwitch( Switch_ItemmallNotDisassemble) )
		{
			return false;
		}
	}
	else
	{
		if ( itemtype == ITEMTYPE_NECK )							
			return false;
		if ( itemtype == ITEMTYPE_MANTLE )													
			return false;
		if ( itemtype == ITEMTYPE_RING )													
			return false;
		if ( pclItemManager->IsItemInfoAtb(siUnique, ITEMINFOATB_NOTTRADE) )		
			return false;

		if (itemtype == ITEMTYPE_AVATAR_MANTLE)
			return false;
		if (itemtype == ITEMTYPE_AVATAR_DRESS)
			return false;
		if (itemtype == ITEMTYPE_AVATAR_HAT)
			return false;
		if (itemtype == ITEMTYPE_AVATAR_NECK)
			return false;
		if (itemtype == ITEMTYPE_AVATAR_RING)
			return false;
	}

	// 대만에서만 아바타상품이 이호식 분해되지 않도록 함 (PCK - 2007.09.11)
	// [영훈] 가챠 의복은 분해가 됨 (2008.06.20)
	if ( (pclItemManager->IsItemInfoAtb(siUnique, ITEMINFOATB_ITEMMALL) == true) && (false == pclClient->pclGachaManager->IsGachaRareItem( siUnique )) )
	{
		if(pclClient->IsCountrySwitch(Switch_WENNYDISASSEMBLEAvata))
		{
			if (itemtype == ITEMTYPE_DRESS)													return false;
			if (itemtype == ITEMTYPE_HAT)													return false;
			if (itemtype == ITEMTYPE_MANTLE)												return false;
		}
	}

	// 화살통
	if(siUnique == ITEMUNIQUE(13600))														return false;
	// 매거진
	if(siUnique == ITEMUNIQUE(13601))														return false;

	// [진성] 황금 낚시대 => 2008-8-12
	if(siUnique == ITEMUNIQUE(17160))														return false;
	
	cltServer* pclserver = (cltServer*)pclClient;
	if( false == pclClient->pclEventTimeManager->InEventTime(TEXT("Chuseok_Monster_Appear"), &pclserver->sTime) )
	{
		// [진성] 추석 이벤트. 우주토끼 모자=> 2008-8-12
		if(siUnique == ITEMUNIQUE(23496))													return false;

		if(siUnique == ITEMUNIQUE(13047))													return false;
		if(siUnique == ITEMUNIQUE(13048))													return false;
		if(siUnique == ITEMUNIQUE(23114))													return false;
	}

	// 결정체 제외 - 당분간.
	if ( itemtype == ITEMTYPE_CRYSTAL )							
		return false;

	// 파티홀에서 받은 아이템은 분해되지 않는다.
	if ( clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )	return false;
	//=============================================================================

	//=============================================================================
	// 분해 되는 조건

	SI32 type = GetType(pclItemManager);
	if( type )
	{
		SI64 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;
		if( atb & ITEMATB_OBJECT )
		{
			SI32 slv = pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
			if( pclClient->IsCountrySwitch(Switch_NotScailed) && (slv > 0 && slv < 30) )		return true;
		}
	}

	//----------------------------------------------------------------------------
	// 귀속물품은 분해 가능
	if( IsBelonging( pclItemManager )  == true )											return true;
	// 아이템 몰
	if( pclItemManager->IsItemInfoAtb(siUnique,ITEMINFOATB_ITEMMALL)  == true )
	{
		// 겹쳐지는 않는 유료 아이템 분해 가능 - 아이템 몰
		if( CanPile(pclClient->pclItemManager) == false )									return true;
		// 가차 아이템 분해 가능
	}
	//----------------------------------------------------------------------------

	if( pclClient->pclGachaManager->IsGachaRareItem( siUnique ) )						return true;
	//----------------------------------------------------------------------------
	//KHY - 0115 발렌타인데이 이벤트.
	switch(siUnique)
	{
		//KHY - 0220 - 발렌타인 카드 분해목록 삭제.
	case ITEMUNIQUE(13006): // V카드 
	case ITEMUNIQUE(13007): // A카드.
	case ITEMUNIQUE(13008): // L카드.
	case ITEMUNIQUE(13009): // N1카드
	case ITEMUNIQUE(13010): // E1카드
	case ITEMUNIQUE(13011): // T카드
	case ITEMUNIQUE(13012): // I카드.
	case ITEMUNIQUE(13013): // E2카드 
	case ITEMUNIQUE(13014): // N2카드
		//KHY - 0218 화이트데이 이벤트.
	case ITEMUNIQUE(13016): // W카드 
	case ITEMUNIQUE(13017): // H카드 
	case ITEMUNIQUE(13018): // I카드 
	case ITEMUNIQUE(13019): // T카드 
	case ITEMUNIQUE(13020): // E카드 		
		return true;
		break;
	}
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	// 내구도가 존재 하는지 확인 한다 
	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		if( IsSealingUp( pclItemManager )  == false )	// 봉인되어 있지 않고 
		{
			// 내구도도 존재 하면 보여준다 
			SI32 siDurability = GetDurability()	;
			SI32 siMAXDurability = GetMaxDurability()	;

			if(  siDurability > 0 && siMAXDurability > 0 )
			{
				return true	;
			}
		}
	}
	
	//----------------------------------------------------------------------------



	//=============================================================================

	return false;

//================================================================
#endif
//================================================================
}

//=============================================================================
//  @Name   IsPlusUseDate()
//! @brief  기간연장이 가능한가?
//=============================================================================
bool cltItem::IsPlusUseDate(cltItemManagerCommon* pclItemManager)
{
//================================================================
// DBMANAGER 인경우는 사용하지 않는다.
//================================================================
#ifdef _DBMANAGER
	return false;
#else
//================================================================
	
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)																				return false;

	//=============================================================================
	// 기간연장이 가능한 조건을 추가한다.
	//=============================================================================

	if(pclClient->IsCountrySwitch(Switch_UseDayPlusRevitalize))
	{
		SI32 itemType = pclItemManager->GetItemType(siUnique);

		// 장비품이고,
		if( itemType == ITEMTYPE_HELMET || 
			itemType == ITEMTYPE_ARMOUR ||
			itemType == ITEMTYPE_SHOES	||
			itemType == ITEMTYPE_BELT	||
			itemType == ITEMTYPE_SWORD	||
			itemType == ITEMTYPE_SPEAR	||
			itemType == ITEMTYPE_AXE	||
			itemType == ITEMTYPE_BOW	||
			itemType == ITEMTYPE_GUN	||
			itemType == ITEMTYPE_STAFF ||

			itemType == ITEMTYPE_AVATAR_MANTLE ||
			itemType == ITEMTYPE_AVATAR_DRESS ||
			itemType == ITEMTYPE_AVATAR_HAT ||
			itemType == ITEMTYPE_AVATAR_NECK ||
			itemType == ITEMTYPE_AVATAR_RING 

			)
		{
			// [기형] 휴면 계정이라면 기간 연장 할 수 없다.
			if( NULL != pclClient->pclDormancySystem)
			{
				CDormancyEquipItemReward *pclDormancyEquipItem = (CDormancyEquipItemReward *)pclClient->pclDormancySystem->GetDormancyEquipItemReward();

				if( NULL != pclDormancyEquipItem)
				{
					if( TRUE == pclDormancyEquipItem->IsDormancyEquipItem(siUnique))
					{
						return false;
					}
				}
			}

			// 기간이 존재하면, 기간을 연장할 수 있다.
			if(uiDateUseDay > 0)
				return true;
		}
	}
	else
	{
		// 사용기간이 존재하는 고급품은 기간 연장이 가능하다.
		if( clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 && uiDateUseDay > 0 )
		{
			return true;
		}
	}

	//=============================================================================

	return false;

//================================================================
#endif
//================================================================
}

bool cltItem::IsCorrectPrivateEquip(cltItemManagerCommon* pclItemManager, SI32 siKind)
{
	SI32 type = GetType(pclItemManager);

	if (IsBulletItem() == TRUE)
		return TRUE;

	if(type)
	{
		SI64 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;
		if( (atb & ITEMATB_OBJECT) == 0)return false;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if ( ref > 0 )
	{
		SI32 atb = pclItemManager->pclItemInfo[ref]->siItemInfoAtb ;

		if(pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_EAST) == true)
		{
			if (siKind != 0 && siKind != KIND_HERO3 && siKind != KIND_HEROINE3)
				return false;
		}

		if(pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_WEST) == true)
		{
			if (siKind != 0 && siKind != KIND_HERO4 && siKind != KIND_HEROINE4)
				return false;
		}

		return true;

	}
	else
	{
		return false;
	}


}

bool cltItem::IsCreateSID(cltItemManagerCommon* pclItemManager)
{
//================================================================
// DBMANAGER 인경우는 사용하지 않는다.
//================================================================
#ifdef _DBMANAGER
	return false;
#else
//================================================================
	if( siUnique <= 0 )													return false;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if( ref <= 0 )														return false;

	// SID 필요
	if( CanPile(pclItemManager)	== false )								return true;

//================================================================
#endif
//================================================================
	return false;
}


//[추가 : 황진성 2008. 1. 31 => cltTreasureBox, cltSpaceBox 에서 사용할 아이템 넣을곳 찾기.]
bool CanAddInv( bool BoxKind, cltItem* clItem, cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve,
				SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue, SI16* pCheckPos )
{
	SI32 i;
	BOOL pileswitch = FALSE;			// 아이템의 중첩 허용 여부. 
	if(pclitem == NULL)return FALSE;

	if (pclitem->GetItemNum() > MAX_ITEM_PILE_NUMBER)
		return false;

	// 중첩이 안되는 아이템인지 확인한다. 
	if( pclitem->CanPile(pclItemManager ) == true)
	{
		pileswitch = TRUE;
	}
	else
	{
		pileswitch = FALSE;
	}

	//KHY - 0805 - 아트펙스 수정.
	// 결정체의 경우 특별히 취급한다. -> old = 겹치면 안된다. new = 겹쳐야 한다.
		

	SI32 end = 0, searchrange = 0;
	
	if( true == BoxKind)
	{
		end = MAX_ITEM_TREASURE_BOX;
	}
	else
	{
		end = MAX_ITEM_SPACE_BOX;
	}


	// 이미 같은 아이템이 존재하는지 확인한다. 
	// 같은 아이템이 존재하고 중첩된 개수가 최대 범위 안에 있으면 중첩개수를 늘인다. 
	if(pileswitch == TRUE)
	{
		for(i = 0; i < end; i++)
		{
			if((clItem[i].siUnique == pclitem->siUnique) &&
				(clItem[i].CanPile(pclItemManager) == true))
			{
				if(clItem[i].GetItemNum() + pclitem->GetItemNum() <= MAX_ITEM_PILE_NUMBER)
				{
					*pitempos = i;
					pcltoitem->Set(&clItem[i]);
					return true;
				}
			}
		}
	}

	// 새롭게 추가한다. 
	for(i = 0; i < end; i++)
	{
		if(clItem[i].siUnique)continue;

		// 예약되어 있는 자리라면, 
		if(pdwreserve && pdwreserve[i])continue;

		bool loop = false;
		if(pCheckPos)
		{
			for(SI16 j = 0; j < 5; ++j)
			{
				if(pCheckPos[j] == i)
				{
					loop = true;
					break;
				}
			}
		}

		if(true == loop)
			continue;

		*pitempos = i;
		return true;
	}

	return false;
}

SI16 GetBoxItemCount(bool BoxKind, cltItem* pclItem, SI16 siArrayCount)
{
	if(pclItem == NULL)
		return 0;

	SI32 end = 0;
	if( true == BoxKind)
	{
		end = MAX_ITEM_TREASURE_BOX;
	}
	else
	{
		end = MAX_ITEM_SPACE_BOX;
	}

	if(end != siArrayCount)
	{
		return 0;
	}

	SI16 ItemCount = 0;
	
	for(int  i = 0; i < end; i++)
	{
		if( pclItem[i].siUnique )
		{
			++ItemCount;
			continue;
		}
	}

	return ItemCount;
}

// 결정체 능력에 따른 능력 수치를 찾는다. - 가장 큰 값만 적용한다.
SI32 cltItem::GetAllCrystalAbilityNum(UI08 CrystalAbility)
{
	SI32 temAbNum = 0;
	SI32 returnAbNum = 0;

	bool bCount = false;
	
	for(SI16 i = 0; i < MAX_CRYSTAL_NUM; i++)
	{
		// 아바타 아이템의 모든스킬일 경우, 특별히 처리해야 한다. 
		switch(cltItemCrytal[i].uiCrystalAbility)
		{
			case CRYSTALABILITY_SKILL_ALLMAKESKILL :  // 모든 제조술 
			case CRYSTALABILITY_AVATAR_ALLMAKESKILL :
				{
					switch(CrystalAbility)
					{
						case CRYSTALABILITY_SKILL_MAKESWORD1 :
						case CRYSTALABILITY_SKILL_MAKESPEAR1 :
						case CRYSTALABILITY_SKILL_MAKEAXE1 :
						case CRYSTALABILITY_SKILL_MAKESTAFF1 :
						case CRYSTALABILITY_SKILL_MAKEBOW1 :
						case CRYSTALABILITY_SKILL_MAKEGUN1 :
						case CRYSTALABILITY_SKILL_MAKEARMOUR1 :
						case CRYSTALABILITY_SKILL_MAKEHELMET1 :
						case CRYSTALABILITY_SKILL_MAKESHOES1 :
						case CRYSTALABILITY_SKILL_MAKEBELT1 :
							bCount = true;
							break;				
					}
				}
				break;
			case CRYSTALABILITY_SKILL_ALLWEAPONSKILL :  // 모든 무기술 
			case CRYSTALABILITY_AVATAR_ALLWEAPONSKILL :
				{
					switch(CrystalAbility)
					{
						case CRYSTALABILITY_SKILL_USESWORD1 :
						case CRYSTALABILITY_SKILL_USESPEAR1 :
						case CRYSTALABILITY_SKILL_USEAXE1 :
						case CRYSTALABILITY_SKILL_USESTAFF1 :
						case CRYSTALABILITY_SKILL_USEBOW1 :
						case CRYSTALABILITY_SKILL_USEGUN1 :
							bCount = true;
							break;				
					}
				}
				break;			break;
			case CRYSTALABILITY_SKILL_ALLPRODUCTSKILL :  // 모든 생산술 
			case CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL :
				{
					switch(CrystalAbility)
					{
						case CRYSTALABILITY_SKILL_HORSETRAINING :
						case CRYSTALABILITY_SKILL_FARM :
						case CRYSTALABILITY_SKILL_MINE :
						case CRYSTALABILITY_SKILL_FISH :
						case CRYSTALABILITY_SKILL_MAKEIRON1 :
						case CRYSTALABILITY_SKILL_MAKETOOL1 :
						case CRYSTALABILITY_SKILL_MAKEFOOD :
						case CRYSTALABILITY_SKILL_MAKEMED :
						case CRYSTALABILITY_SKILL_ARTIFACT :
							bCount = true;
							break;				
					}
				}
				break;
			case CRYSTALABILITY_SKILL_ALLAVATARSKILL :  // 모든 아바타 스킬 
			case CRYSTALABILITY_AVATAR_SKILL_ALLAVATARSKILL :
				{
					switch(CrystalAbility)
					{
						case SKILL_MAKEAVATAR_DRESS :
						case SKILL_MAKEAVATAR_HAT :
						case SKILL_MAKEAVATAR_MANTLE :
						case SKILL_MAKEAVATAR_RING :
						case SKILL_MAKEAVATAR_NECK :
							bCount = true;
							break;				
					}
				}
				break;
			default:
				{
					if(cltItemCrytal[i].uiCrystalAbility == CrystalAbility)  // 기타 각 스킬 
						bCount = true;

				}
				break;
		}

		if(bCount)
		{
			bCount = false;

			temAbNum = (SI32)cltItemCrytal[i].uiCrystalAbilityNum;

			if(temAbNum > returnAbNum)  // 가장 큰 값만.
				returnAbNum = temAbNum;
		}
	}

	return returnAbNum;
}

// 해당 스킬 유니크에 따라 결정체 능력으로 변환한다.
SI32 cltItem::GetCrystalAbilityFromSkillUnique(SI32 skillunique)
{
	SI32 index = 0;

	while( Crystalskilldata[index*3] )
	{
		if(skillunique == Crystalskilldata[index*3])
		{		
			if( Crystalskilldata[index*3 + 1] !=0)
				return (SI32)Crystalskilldata[index*3 + 1];
		}

		index++;
	}

	return 0;
}


SI32 cltItem::GetCrystalGrade()
{
	switch ( siUnique )
	{
		case ITEMUNIQUE(1805):	// 루비결정체[F]
		case ITEMUNIQUE(1807):	// 토파즈결정체[F]
		case ITEMUNIQUE(1809):	// 자수정결정체[F]
		case ITEMUNIQUE(1811):	// 다이아결정체[F]
		case ITEMUNIQUE(1813):	// 진주결정체[F]
		case ITEMUNIQUE(1815):	// 사파이어결정체[F]
			{
				return CRYSTAL_GRADE_F;
			}
			break;

		case ITEMUNIQUE(1817):	// 루비결정체[E]
		case ITEMUNIQUE(1819):	// 토파즈결정체[E]
		case ITEMUNIQUE(1821):	// 자수정결정체[E]
		case ITEMUNIQUE(1823):	// 다이아결정체[E]
		case ITEMUNIQUE(1825):	// 진주결정체[E]
		case ITEMUNIQUE(1827):	// 사파이어결정체[E]
			{
				return CRYSTAL_GRADE_E;
			}
			break;

		case ITEMUNIQUE(1829):	// 루비결정체[D]
		case ITEMUNIQUE(1831):	// 토파즈결정체[D]
		case ITEMUNIQUE(1833):	// 자수정결정체[D]
		case ITEMUNIQUE(1835):	// 다이아결정체[D]
		case ITEMUNIQUE(1837):	// 진주결정체[D]
		case ITEMUNIQUE(1839):	// 사파이어결정체[D]
			{
				return CRYSTAL_GRADE_D;
			}
			break;

		case ITEMUNIQUE(1841):	// 루비결정체[C]
		case ITEMUNIQUE(1843):	// 토파즈결정체[C]
		case ITEMUNIQUE(1845):	// 자수정결정체[C]
		case ITEMUNIQUE(1847):	// 다이아결정체[C]
		case ITEMUNIQUE(1849):	// 진주결정체[C]
		case ITEMUNIQUE(1851):	// 사파이어결정체[C]
			{
				return CRYSTAL_GRADE_C;
			}
			break;

		case ITEMUNIQUE(1853):	// 루비결정체[B]
		case ITEMUNIQUE(1855):	// 토파즈결정체[B]
		case ITEMUNIQUE(1857):	// 자수정결정체[B]
		case ITEMUNIQUE(1859):	// 다이아결정체[B]
		case ITEMUNIQUE(1861):	// 진주결정체[B]
		case ITEMUNIQUE(1863):	// 사파이어결정체[B]
			{
				return CRYSTAL_GRADE_B;
			}
			break;

		case ITEMUNIQUE(1865):	// 루비결정체[A]
		case ITEMUNIQUE(1867):	// 토파즈결정체[A]
		case ITEMUNIQUE(1869):	// 자수정결정체[A]
		case ITEMUNIQUE(1871):	// 다이아결정체[A]
		case ITEMUNIQUE(1873):	// 진주결정체[A]
		case ITEMUNIQUE(1875):	// 사파이어결정체[A]
			{
				return CRYSTAL_GRADE_A;
			}
			break;

		case ITEMUNIQUE(1877):	// 루비결정체[S1]
		case ITEMUNIQUE(1879):	// 토파즈결정체[S1]
		case ITEMUNIQUE(1881):	// 자수정결정체[S1]
		case ITEMUNIQUE(1883):	// 다이아결정체[S1]
		case ITEMUNIQUE(1885):	// 진주결정체[S1]
		case ITEMUNIQUE(1887):	// 사파이어결정체[S1]
			{
				return CRYSTAL_GRADE_S1;
			}
			break;

		case ITEMUNIQUE(1889):	// 루비결정체[S2]
		case ITEMUNIQUE(1891):	// 토파즈결정체[S2]
		case ITEMUNIQUE(1893):	// 자수정결정체[S2]
		case ITEMUNIQUE(1895):	// 다이아결정체[S2]
		case ITEMUNIQUE(1897):	// 진주결정체[S2]
		case ITEMUNIQUE(1899):	// 사파이어결정체[S2]
			{
				return CRYSTAL_GRADE_S2;
			}
			break;

	}

	return CRYSTAL_GRADE_NONE;
}

bool cltItem::IsWeapon(cltItemManagerCommon* pclItemManager)
{
	if ( NULL == pclItemManager )
	{
		return false;
	}

	SI32 siType = GetType( pclItemManager );

	switch ( siType )
	{
		case ITEMTYPE_SWORD:
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_AXE:
		case ITEMTYPE_BOW:
		case ITEMTYPE_GUN:
		case ITEMTYPE_STAFF:
			{
				return true;
			}
			break;
	}

	return false;
}

SI16 cltItem::GetEquipLevel( cltItemManagerCommon* pclItemManager )
{
	if ( NULL == pclItemManager )
	{
		return 0;
	}

	SI32 siRef = pclItemManager->FindItemRefFromUnique( siUnique );
	if ( 0 >= siRef )
	{
		return 0 ;
	}

	return pclItemManager->pclItemInfo[siRef]->clCondition.siLevel;

}

void cltItem::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )			return;

}

bool cltItem::CancellationSealingUp( SYSTEMTIME sTime )
{
	// 사용기간이 없는것은 밀봉해제할 필요없음
	if ( 0 >= uiDateUseDay )
	{
		return false;
	}

	// 현재 날짜로 들어오는것이 2000이하이면 실패
	if ( 2000 >= sTime.wYear )
	{
		return false;
	}

	// 현재 날짜로 설정해준다
	uiDateYear	= (UI08)(sTime.wYear - 2000);
	uiDateMonth	= (UI08)sTime.wMonth;
	uiDateDay	= (UI08)sTime.wDay;

	return true;
}

// 무기술, 제조술, 생산술 하위 스킬들중 하나를 선택해준다.
SI32 cltItem::SelectUseBaseSkillTypeToAvatar(UI32 uiBaseSkillType)
{
	//================================================================
	// DBMANAGER 인경우는 사용하지 않는다.
	//================================================================
#ifdef _DBMANAGER
	return false;
#else
	//================================================================

	//	[종호] 설정할수 있는 스킬은 중분류 스킬타입들중 한가지의 스킬을 줍니다.
	//	스킬타입별로 포함되는 스킬 갯수를 스킬매니저에 추가하는 것이 하드코딩을 조금 줄일 것 같습니다.

	UI32 returnDivideSkillType = 0;
	UI32 uiDetailSelectValue = 0;

	switch(uiBaseSkillType)
	{
	case SKILLTYPE_R_USEWEAPON:		
		{
			uiDetailSelectValue = rand() % 6;	//	무기술은 총 6가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_USESWORD1;	break;	//	검술
			case 1:	returnDivideSkillType = SKILL_USESPEAR1;	break;	//	창술
			case 2:	returnDivideSkillType = SKILL_USEAXE1;	break;	//	도끼술
			case 3:	returnDivideSkillType = SKILL_USESTAFF1;	break;	//	지팡이술
			case 4:	returnDivideSkillType = SKILL_USEBOW1;	break;	//	활술
			case 5:	returnDivideSkillType = SKILL_USEGUN1;	break;	//	총술
			default:
				false;
				break;
			}
		}
		break;

	case SKILLTYPE_R_MAKEWEAPON:		
		{
			uiDetailSelectValue = rand() % 6;		//	무기제작술은 총 6가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_MAKESWORD1;	break;	//	검제작술
			case 1:	returnDivideSkillType = SKILL_MAKESPEAR1;	break;	//	창제작술
			case 2:	returnDivideSkillType = SKILL_MAKEAXE1;	break;	//	도끼제작술
			case 3:	returnDivideSkillType = SKILL_MAKESTAFF1;	break;	//	지팡이제작술
			case 4:	returnDivideSkillType = SKILL_MAKEBOW1;	break;	//	활제작술
			case 5:	returnDivideSkillType = SKILL_MAKEGUN1;	break;	//	총제작술
			default:
				false;
				break;
			}
		}
		break;

	case SKILLTYPE_R_ARMOR:		
		{
			uiDetailSelectValue = rand() % 4;		//	방어구제작술은 총 4가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_MAKEARMOUR1;	break;	//	갑옷제작술
			case 1:	returnDivideSkillType = SKILL_MAKEHELMET1;	break;	//	투구제작술
			case 2:	returnDivideSkillType = SKILL_MAKEBELT1;		break;	//	벨트제작술
			case 3:	returnDivideSkillType = SKILL_MAKESHOES1;		break;	//	신발제작술
			default:
				false;
				break;
			}
		}

		break;

	case SKILLTYPE_R_MAKEHARVEST:	
		{
			if (pclClient->IsCountrySwitch(Switch_Artifact))
				uiDetailSelectValue = rand() % 5;		//	결정체 스위치가 켜져 있다면 생산제작술은 총 5가지
			else
				uiDetailSelectValue = rand() % 4;		//	결정체 스위치가 꺼져있다면 4가지

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_MAKEMED;		break;	//	약제조술
			case 1:	returnDivideSkillType = SKILL_MAKEFOOD;		break;	//	요리제조술
			case 2:	returnDivideSkillType = SKILL_MAKEIRON1;		break;	//	철물제작술
			case 3:	returnDivideSkillType = SKILL_MAKETOOL1;		break;	//	생산도구제작술
			case 4:	returnDivideSkillType = SKILL_ARTIFACT;		break;	//	결정체제작술
			default:
				false;
				break;
			}
		}
		break;

	case SKILLTYPE_R_HARVEST:		
		{
			uiDetailSelectValue = rand() % 3;		//	생산기술은 총 3가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_FISH;		break;	//	어획술
			case 1:	returnDivideSkillType = SKILL_FARM;		break;	//	농경술
			case 2:	returnDivideSkillType = SKILL_MINE;		break;	//	채광술	
			default:
				false;
				break;
			}
		}
		break;

	case SKILLTYPE_R_MAKECLOTH:		
		{
			uiDetailSelectValue = rand() % 3;		//	재단술은 총 3가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_MAKEAVATAR_DRESS;		break;	//	의복제작술
			case 1:	returnDivideSkillType = SKILL_MAKEAVATAR_HAT;			break;	//	모자제작술
			case 2:	returnDivideSkillType = SKILL_MAKEAVATAR_MANTLE;		break;	//	망토제작술	 
			default:
				false;
				break;
			}
		}
		break;

	case SKILLTYPE_R_MAKEACCESSARY:	
		{
			uiDetailSelectValue = rand() % 2;		//	액세서리제작술은 총 2가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_MAKEAVATAR_RING;		break;	//	반지제작술
			case 1:	returnDivideSkillType = SKILL_MAKEAVATAR_NECK;		break;	//	목걸이제작술
			default:
				false;
				break;
			}
		}
		break;

	case SKILLTYPE_R_ETC:				
		{
			uiDetailSelectValue = rand() % 3;		//	기타기술은 총 3가지		

			switch(uiDetailSelectValue)
			{
			case 0:	returnDivideSkillType = SKILL_CAPTURE;			break;	//	소환술
			case 1:	returnDivideSkillType = SKILL_SUMMON;				break;	//	포획술
			case 2:	returnDivideSkillType = SKILL_HORSETRAINING;		break;	//	말조련술
			default:
				false;
				break;
			}
		}
		break;
	}	

	return returnDivideSkillType;

#endif
}


void cltItemAvatarRing::SetQualityRate( SI32 siQualityRate )
{
	if(siQualityRate > 0)
	{
		if(uiAC > 0)
			uiAC		= max(1, uiAC * siQualityRate / 100 );

		if(uiStr > 0)
			uiStr		= max(1, uiStr * siQualityRate / 100 );

		if(uiDex > 0)
			uiDex		= max(1, uiDex * siQualityRate / 100 );

		if(uiVit > 0)
			uiVit		= max(1, uiVit * siQualityRate / 100 );

		if(uiMag > 0)
			uiMag		= max(1, uiMag * siQualityRate / 100 );

		if(uiWis > 0)		
			uiWis		= max(1, uiWis * siQualityRate / 100 );

		if(uiLuk > 0)		
			uiLuk		= max(1, uiLuk * siQualityRate / 100 );

		if(uiHnd > 0)		
			uiHnd		= max(1, uiHnd * siQualityRate / 100 );
	}
}
	
