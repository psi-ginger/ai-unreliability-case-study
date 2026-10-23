//---------------------------------
// 2003/6/9 김태곤
//---------------------------------
#include "stdafx.h"
#include "cltItem.h"
#include "TextMgr.h"

extern	SI32 skilldata[] =
{
	SKILL_MAKEAXE1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPAXE,
		SKILL_MAKESPEAR1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSPEAR,
		SKILL_MAKESWORD1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSWORD,
		SKILL_MAKESTAFF1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSTAFF, 
		SKILL_MAKEBOW1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBOW, 
		SKILL_MAKEGUN1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPGUN, 
		SKILL_MAKEARMOUR1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPARMOUR, 
		SKILL_MAKEHELMET1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHELMET, 
		SKILL_MAKEDRESS1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPDRESS, 
		SKILL_MAKEHAT1,		SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHAT, 
		SKILL_MAKESHOES1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSHOES, 
		SKILL_MAKEBELT1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBELT, 
		SKILL_MAKENECK1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPNECK, 
		SKILL_MAKERING1,	SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING, 
		SKILL_USESWORD1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD, 
		SKILL_USESPEAR1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR,
		SKILL_USEAXE1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE,
		SKILL_USEBOW1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW,
		SKILL_USEGUN1,		SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN, 
		SKILL_USESTAFF1,	SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF, 
		0,				0
};



// 아이템 옵션 텍스트를 구한다. 
bool cltItemOptionInfo::GetText(char* ptext)
{
	char temp[256];
	strcpy(ptext, "");

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

		float rate; 
		switch(type)
		{
		case  SCROLL_UNIQUE_FUNCTION_UPHITRATE:
			{
				char* pText = GetTxtFromMgr(687);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate, '%');
					strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_UPCRITICAL:	
			{
				char* pText = GetTxtFromMgr(688);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate, '%');
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_POISON:
			{
				char* pText = GetTxtFromMgr(689);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate, '%');
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_ICE:
			{
				char* pText = GetTxtFromMgr(690);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate, '%');
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_REDUCEDAMAGE:
			{
				char* pText = GetTxtFromMgr(691);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate, '%');
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_DRAIN:
			{
				char* pText = GetTxtFromMgr(692);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate, '%');
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPAXE:
			{
				char* pText = GetTxtFromMgr(693);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSPEAR:
			{
				char* pText = GetTxtFromMgr(694);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSWORD:
			{
				char* pText = GetTxtFromMgr(695);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);

			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSTAFF:
			{
				char* pText = GetTxtFromMgr(696);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBOW:
			{
				char* pText = GetTxtFromMgr(697);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPGUN:
			{
				char* pText = GetTxtFromMgr(698);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPARMOUR:
			{
				char* pText = GetTxtFromMgr(699);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHELMET:
			{
				char* pText = GetTxtFromMgr(700);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPDRESS:
			{
				char* pText = GetTxtFromMgr(701);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPHAT:
			{
				char* pText = GetTxtFromMgr(702);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPSHOES:
			{
				char* pText = GetTxtFromMgr(703);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPBELT:
			{
				char* pText = GetTxtFromMgr(704);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPNECK:
			{
				char* pText = GetTxtFromMgr(705);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_MAKESKILLUPRING:
			{
				char* pText = GetTxtFromMgr(706);
				rate = (float)(para / 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_UPMAXDURABILITY:
			{
				char* pText = GetTxtFromMgr(707);
				rate = (float)(para);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);

			}
			break;				

		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_SWORD:
			{
				char* pText = GetTxtFromMgr(708);
				rate = (float)(para/ 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_SPEAR:
			{
				char* pText = GetTxtFromMgr(709);
				rate = (float)(para/ 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_AXE:
			{
				char* pText = GetTxtFromMgr(710);
				rate = (float)(para/ 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_BOW:
			{
				char* pText = GetTxtFromMgr(711);
				rate = (float)(para/ 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_GUN:
			{
				char* pText = GetTxtFromMgr(712);
				rate = (float)(para/ 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;
		case SCROLL_UNIQUE_FUNCTION_USESKILLUP_STAFF:
			{
				char* pText = GetTxtFromMgr(713);
				rate = (float)(para/ 10.0);
				sprintf(temp, pText, rate);
				strcat(ptext, temp);
			}
			break;




		default:
			MsgBox("vdidjf", "f38fdsjf:%d", type);
		}
	}

	if(strcmp(ptext, "") == 0)
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

cltItem::cltItem(SI32 siunique, cltItemTool* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemTool.Set(pcldata);
}

cltItem::cltItem(SI32 siunique, cltItemBag* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemBag.Set(pcldata);
}

cltItem::cltItem(SI32 siunique, cltItemMantle* pcldata)
{
	ZeroMemory(this, sizeof(cltItem));

	siItemNum = 1;		// 최소한 한개는 있다. 

	siUnique = siunique;

	clItemMantle.Set(pcldata);
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
SI32 cltItem::GetEquipMag(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_STAFF:
		return clItemWeapon.uiMag;
		break;
	case ITEMTYPE_RING:
		return clItemRing.uiMag;
		break;
	default:
		return 0;
	}

	return 0;
}

// 근력을 구한다. 
SI32 cltItem::GetEquipStr(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_RING:
		return clItemRing.uiStr;
		break;
	case ITEMTYPE_MANTLE:
		return clItemMantle.uiStr;
		break;

	default:
		return 0;
	}

	return 0;
}

// 순발력을 구한다. 
SI32 cltItem::GetEquipDex(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_RING:
		return clItemRing.uiDex;
		break;
	case ITEMTYPE_MANTLE:
		return clItemMantle.uiDex;
		break;

	default:
		return 0;
	}

	return 0;
}

// 생명력을 구한다. 
SI32 cltItem::GetEquipVit(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_RING:
		return clItemRing.uiVit;
		break;
	case ITEMTYPE_MANTLE:
		return clItemMantle.uiVit;
		break;

	default:
		return 0;
	}

	return 0;
}


// 최소 데미지를 구한다. 
SI32 cltItem::GetMinDamage(cltItemManagerCommon* pclItemManager)const
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
		return clItemWeapon.uiMinDamage;
		break;
	default:
		return 0;
	}

	return 0;

}

// 최대 데미지를 구한다. 
SI32 cltItem::GetMaxDamage(cltItemManagerCommon* pclItemManager)const
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
		return clItemWeapon.uiMaxDamage;
		break;
	default:
		return 0;
	}

	return 0;

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
		clItemWeapon.uiMinDamage = damage;
		return clItemWeapon.uiMinDamage;
		break;
	default:
		MsgBox("SetMinDamage()", "unique:%d type:%d", siUnique, type); 
		{
			SI32 a = 0;
		}
		return 0;
	}

	return 0;

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
		clItemWeapon.uiMaxDamage = damage;
		return clItemWeapon.uiMaxDamage;
		break;

	default:
		MsgBox("SetMaxDamage()", "unique:%d type:%d", siUnique, type);
		{
			SI32 a = 0;
		}

		return 0;
	}

	return 0;

}


// 방어 능력을 구한다. 
SI32 cltItem::GetAC(cltItemManagerCommon* pclItemManager)const
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:		
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
		return clItemArmour.uiAC;
		break;

	case ITEMTYPE_BAG:	
		return clItemBag.uiAC;
		break;

	case ITEMTYPE_MANTLE:	
		return clItemMantle.uiAC;
		break;

	case ITEMTYPE_SHOES:
		return clItemShoes.uiAC;
		break;


	default:
		MsgBox("GetAC()", "unique:%d type:%d", siUnique, type);
		{
			SI32 a = 0;
		}

		return 0;
	}

	return 0;
}

// 방어 능력을 설정한다. 
SI32 cltItem::SetAC(cltItemManagerCommon* pclItemManager, SI32 ac)
{
	if(siUnique == 0)return 0;

	SI32 type = GetType(pclItemManager);

	switch(type)
	{
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:		
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
		clItemArmour.uiAC = ac;
		return clItemArmour.uiAC;
		break;

	case ITEMTYPE_BAG:	
		clItemBag.uiAC = ac;
		return clItemBag.uiAC;
		break;

	case ITEMTYPE_MANTLE:	
		clItemMantle.uiAC	= ac;
		return clItemMantle.uiAC;
		break;

	case ITEMTYPE_SHOES:
		clItemShoes.uiAC	= ac;
		return clItemShoes.uiAC;
		break;

	default:
		MsgBox("SetAC()", "unique:%d type:%d", siUnique, type);
		{
			SI32 a = 0;
		}

		return 0;
	}

	return 0;
}


// 특정 기술의 부적에 의한 상승 효과를 구한다. 
SI32 cltItem::GetSkillScrollUp(cltItemManagerCommon* pclItemManager, SI32 skillunique)
{
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)return 0;

	SI32 index = 0;

	while( skilldata[index*2] )
	{
		if(skillunique == skilldata[index*2])
		{
			if(clItemCommon.clOptionInfo.uiType1 == skilldata[index*2 + 1])
			{
				return clItemCommon.clOptionInfo.uiPara1 / 10;
			}

			if(clItemCommon.clOptionInfo.uiType2 == skilldata[index*2 + 1])
			{
				return clItemCommon.clOptionInfo.uiPara2 / 10;
			}
		}

		index++;
	}

	return 0;

}

// 기술력 향상을 구한다. 
SI32 cltItem::GetUpgradeSkillLevel(cltItemManagerCommon* pclItemManager, SI32 skillunique)
{
	if(siUnique == 0)return 0;

	//---------------------------------------------------------
	// 부적 부착에 의한 효과를 구한다. 
	//---------------------------------------------------------
	SI32 up = GetSkillScrollUp(pclItemManager, skillunique);
	if(up)return up;


	//--------------------------------------------
	// 고유의 성능향상을 구한다. 
	//--------------------------------------------
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)return 0;

	if(skillunique == pclItemManager->pclItemInfo[ref]->siUpgradeSkillUnique)
	{
		SI32 type = GetType(pclItemManager);

		switch(type)
		{
		case ITEMTYPE_HELMET:
		case ITEMTYPE_HAT:		
		case ITEMTYPE_BELT:
		case ITEMTYPE_ARMOUR:
		case ITEMTYPE_DRESS:
			return clItemArmour.uiSkillUp;
			break;
		case ITEMTYPE_NECK:
			return clItemNeck.uiSkillPointUp;
			break;

		default:
			return 0;
		}
	}



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
	case ITEMTYPE_HAT:
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
		return clItemArmour.uiLuck;
		break;
	default:
		return 0;
	}

	return 0;
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
		MsgBox("fdsmv8jfd", "fmcvijsd:%d", type);
		break;
	}

	return 0;
}

//----------------------------------------
// 내구성 관련. 
//----------------------------------------

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
	case ITEMTYPE_HAT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
		clItemArmour.clCommonInfo.uiDurability		= dur;
		return clItemArmour.clCommonInfo.uiDurability;
		break;
	case ITEMTYPE_BAG:
		clItemBag.clCommonInfo.uiDurability		= dur;
		return clItemBag.clCommonInfo.uiDurability;
		break;

	case ITEMTYPE_MANTLE:
		clItemMantle.clCommonInfo.uiDurability		= dur;
		return clItemMantle.clCommonInfo.uiDurability;
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

	default:
		MsgBox("SetDurability()", "fd182:%d", type);
	}

	return 0;

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
	case ITEMTYPE_HAT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
		return clItemArmour.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_BAG:
		return clItemBag.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_MANTLE:
		return clItemMantle.clCommonInfo.uiMaxDurability;
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

	default:
		MsgBox("GetMaxDurability()", "fd182:%d", type);
	}

	return 0;
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
	case ITEMTYPE_HAT:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
		clItemArmour.clCommonInfo.uiMaxDurability		= dur;
		return clItemArmour.clCommonInfo.uiMaxDurability;
		break;
	case ITEMTYPE_BAG:
		clItemBag.clCommonInfo.uiMaxDurability		= dur;
		return clItemBag.clCommonInfo.uiMaxDurability;
		break;

	case ITEMTYPE_MANTLE:
		clItemMantle.clCommonInfo.uiMaxDurability		= dur;
		return clItemMantle.clCommonInfo.uiMaxDurability;
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

	default:
		MsgBox("SetMaxDurability()", "fd182:%d", type);
	}

	return 0;

}

// 내구성을 줄인다. 
bool cltItem::DecreaseDurability(cltItemManagerCommon* pclItemManager, SI32 durvary)
{
	// 내구도가 감소되는 아이템인지 확인한다. 
	SI32 type = GetType(pclItemManager);
	if(type)
	{
		SI32 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;
		if( (atb & ITEMATB_OBJECT) == 0)return false;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref <= 0)return false;


	// 최대 내구도가 0이면,, (이런 경우에는 기본값으로 설정해 준다. ) 
	if(GetMaxDurability(pclItemManager) == 0)
	{
		SetMaxDurability(pclItemManager, pclItemManager->pclItemInfo[ref]->clItem.clItemCommon.clCommonInfo.uiMaxDurability);
		SetDurability(pclItemManager, pclItemManager->pclItemInfo[ref]->clItem.clItemCommon.clCommonInfo.uiMaxDurability);
	}


	// 무기 아이템의 내구도를 구해서 감소시킨다. 
	SI32 dur = GetDurability();
	// 내구도가 영원히 지속되는 물품이면, 
	if(dur == FOREVER_DURABILITY)
	{
		return false;
	}
	if(dur >= durvary)
	{
		// 손상되지 않는 내구를 갖는 물품이라면, 
		if(pclItemManager->pclItemInfo[ref]->clItem.GetDurability() == FOREVER_DURABILITY )
		{
			dur = FOREVER_DURABILITY;
			SetMaxDurability(pclItemManager, dur);
		}
		else
		{
			dur -= durvary;	
		}

	}
	else
	{
		dur	= 0;
	}

	// 내구도가 0이면 아이템을 소멸 시킨다. 
	if(dur <= 0)
	{
		Init();
	}
	// 내구도가 유효하면 아이템은 소멸되지 않는다. 
	else
	{
		SetDurability(pclItemManager, dur);
	}

	return true;
}




// 아이템이 제 정보를 제대로 받았는지 확인한다.
BOOL cltItem::CheckType(cltItemManagerCommon* pclItemManager, SI32 orgtype)
{
	SI32 type = GetType(pclItemManager);

	if(type != orgtype)
	{
		MsgBox("cltItem0", "unique:%d type:%d", siUnique, type);
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

const char* cltItem::GetShortName(cltItemManagerCommon* pclItemManager)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		return pclItemManager->pclItemInfo[ref]->GetName();
	}

	return NULL;
}

bool cltItem::GetFullName(cltItemManagerCommon* pclItemManager, char* pname)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		strcpy(pname, pclItemManager->pclItemInfo[ref]->GetName());

		if(clItemCommon.clCommonInfo.uiRareInfo)
		{
			switch(clItemCommon.clCommonInfo.uiRareInfo)
			{
			case  RARELEVEL_1:
				{
					char* pText = GetTxtFromMgr(714);
					strcat(pname, pText);
				}break;
			case  RARELEVEL_2:
				{
					char* pText = GetTxtFromMgr(715);
					strcat(pname, pText);
				}break;
			default:
				{
					char* pText = GetTxtFromMgr(716);
					strcat(pname, pText);	
				}break;
			}
		}
		return true;

	}

	return false;
}

// 아이템의 이름과 개수를 얻어온다. 
bool cltItem::GetFullNameNNum(cltItemManagerCommon* pclItemManager, char* pname)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		strcpy(pname, pclItemManager->pclItemInfo[ref]->GetName());

		if(clItemCommon.clCommonInfo.uiRareInfo)
		{
			switch(clItemCommon.clCommonInfo.uiRareInfo)
			{
			case  RARELEVEL_1:
				{
					char* pText = GetTxtFromMgr(714);
					strcat(pname, pText);
				}break;
			case  RARELEVEL_2:
				{
					char* pText = GetTxtFromMgr(715);
					strcat(pname, pText);
				}break;
			default:
				{
					char* pText = GetTxtFromMgr(716);
					strcat(pname, pText);	
				}break;
			}
		}

		char temp[256];
		char* pText = GetTxtFromMgr(717);
		sprintf(temp, pText, siItemNum);
		strcat(pname, temp);
		return true;

	}

	return false;
}



char* cltItem::GetArmCode(cltItemManagerCommon* pclItemManager)
{
	// 유니크 값을 근거로 아이템의 타입을 구한다. 
	SI32 ref = pclItemManager->FindItemRefFromUnique(siUnique);
	if(ref > 0)
	{
		if(strcmp(pclItemManager->pclItemInfo[ref]->szArmCode, "") == 0)
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

	return false;
}
// 아이템이 같은 것인지 비교하는 함수. 
// numcomp : 개수까지 같아야 하는지 여부. 
BOOL cltItem::IsSame(const cltItem* pclitem, bool numcomp)
{
	SI32 i;

	if(pclitem == NULL)return FALSE;

	if(pclitem->siUnique != siUnique)return FALSE;
	if(numcomp)
	{
		if(pclitem->siItemNum	!= siItemNum)return FALSE;
	}

	for(i = 0;i < MAX_ITEM_UNION_BYTE/4;i++)
	{
		if(pclitem->siData32[i] != siData32[i])return FALSE;
	}

	return TRUE;
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
	// 중첩이 안되는 아이템인지 확인한다. 
	SI32 type = GetType(pclItemManager);
	if(type)
	{
		SI32 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;

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
		SI32 atb = pclItemManager->pclItemTypeInfo[type]->siAtb;

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



