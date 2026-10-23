//---------------------------------
// 2003/12/2 김태곤
//---------------------------------

#include "cltItem.h"
#include "../../char/CharCommon/Char-Common.h"
#include "../../char/KindInfo/kindinfo.h"
#include "../../../common/PrivateTradeOrderMgr/PrivateTradeOrderBaseInfo.h"


//---------------------------------------
// 캐릭터의 아이템 보관 정보. 
//---------------------------------------
void cltPItemInfo::Set(cltPItemInfo* pclitemmanager)
{
	memcpy(this, pclitemmanager, sizeof(cltPItemInfo));	
}

// 전체의 무게를 얻어온다. 
SI32 cltPItemInfo::GetTotalWeight(cltItemManagerCommon* pclItemManager)
{
	SI32 i;
	SI32 sum = 0;

	for(i = 0;i < MAX_ITEM_PER_PERSON;i++)
	{
		if(clItem[i].siUnique)
		{
			sum += clItem[i].GetWeight(pclItemManager, TRUE);
		}
	}
	return sum;
}


// 무장의 타입을 구한다. 
SI32 cltPItemInfo::GetWeaponType(cltItemManagerCommon* pclItemManager, SI16 activeweapon)
{
	return clItem[activeweapon].GetType(pclItemManager);
}

/*
// 무장의 도력을 구한다. 
SI32 cltPItemInfo::GetEquipMag(cltItemManagerCommon* pclItemManager, SI16 activeweapon)
{
	//return clItem[activeweapon].GetEquipMag(pclItemManager) 
	return clItem[PERSONITEM_RING1].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_RING2].GetEquipMag(pclItemManager)
		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		+ clItem[PERSONITEM_HELMET].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_ARMOUR].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_WEAPON1].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_NECK].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_BELT].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_SHOES].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_MANTLE].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_HAT].GetEquipMag(pclItemManager)
		+ clItem[PERSONITEM_DRESS].GetEquipMag(pclItemManager);
}

// 무장의 근력을 구한다. 
SI32 cltPItemInfo::GetEquipStr(cltItemManagerCommon* pclItemManager)
{
	return clItem[PERSONITEM_RING1].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_RING2].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_MANTLE].GetEquipStr(pclItemManager)
		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		+ clItem[PERSONITEM_HELMET].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_ARMOUR].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_WEAPON1].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_NECK].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_BELT].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_SHOES].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_HAT].GetEquipStr(pclItemManager)
		+ clItem[PERSONITEM_DRESS].GetEquipStr(pclItemManager);	
}

// 무장의 순발력을 구한다. 
SI32 cltPItemInfo::GetEquipDex(cltItemManagerCommon* pclItemManager)
{
	return clItem[PERSONITEM_RING1].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_RING2].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_MANTLE].GetEquipDex(pclItemManager)
		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		+ clItem[PERSONITEM_HELMET].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_ARMOUR].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_WEAPON1].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_NECK].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_BELT].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_SHOES].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_HAT].GetEquipDex(pclItemManager)
		+ clItem[PERSONITEM_DRESS].GetEquipDex(pclItemManager);		
}

// 무장의 생명력을 구한다. 
SI32 cltPItemInfo::GetEquipVit(cltItemManagerCommon* pclItemManager)
{
	return clItem[PERSONITEM_RING1].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_RING2].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_MANTLE].GetEquipVit(pclItemManager)
		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		+ clItem[PERSONITEM_HELMET].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_ARMOUR].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_WEAPON1].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_NECK].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_BELT].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_SHOES].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_HAT].GetEquipVit(pclItemManager)
		+ clItem[PERSONITEM_DRESS].GetEquipVit(pclItemManager);	
}
*/



// 장비의 중독률 향상을 구한다.  (근거리 무기만)
SI32 cltPItemInfo::GetEquipPoisonRate(cltItemManagerCommon* pclItemManager, SI16 activeweapon)
{
	// 소환수 부착 부적에 의한 영향력 추가 
	SI32 siTotalRate = 0;

	cltItem* pclitem = (cltItem*)&clItem[activeweapon];
	if(pclitem->siUnique == 0)			return siTotalRate;

	SI64 atb = pclItemManager->GetItemAtb(pclitem->siUnique);
	if((atb & ITEMATB_NEARWEAPON) == 0)	return siTotalRate;

	if( pclitem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_POISON)
	{
		siTotalRate +=pclitem->clItemCommon.clOptionInfo.uiPara1;
	}
	if( pclitem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_POISON)
	{
		siTotalRate +=pclitem->clItemCommon.clOptionInfo.uiPara2;
	}

	return siTotalRate;
}

// 장비의 동빙한설확률 향상을 구한다.  ( 원거리 무기만 )
SI32 cltPItemInfo::GetEquipIceRate(cltItemManagerCommon* pclItemManager, SI16 activeweapon)
{
	// 소환수 부착 부적에 의한 영향력 추가 
	SI32 siTotalRate = 0;

	cltItem* pclitem = (cltItem*)&clItem[activeweapon];
	if(pclitem->siUnique == 0)			return siTotalRate;

	SI64 atb = pclItemManager->GetItemAtb(pclitem->siUnique);
	if((atb & ITEMATB_FARWEAPON) == 0)	return siTotalRate;

	if( pclitem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_ICE)
	{
		siTotalRate +=pclitem->clItemCommon.clOptionInfo.uiPara1;
	}
	if( pclitem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_ICE)
	{
		siTotalRate +=pclitem->clItemCommon.clOptionInfo.uiPara2;
	}
	
	return siTotalRate;
}


// 장비의 동빙한설확률 향상을 구한다. (신말에만)
SI32 cltPItemInfo::GetEquipDrainRate(cltItemManagerCommon* pclItemManager,SI16 active)
{

	// 소환수 부착 부적에 의한 영향력 추가 
	SI32 siTotalRate = 0;

	cltItem* pclitem = (cltItem*)&clItem[active];
	if(pclitem->siUnique == 0)		return siTotalRate;

	SI64 atb = pclItemManager->GetItemAtb(pclitem->siUnique);
	if((atb & ITEMATB_SHOES) == 0)	return siTotalRate;

	if( pclitem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_DRAIN)
	{
		siTotalRate +=pclitem->clItemCommon.clOptionInfo.uiPara1;
	}
	if( pclitem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_DRAIN)
	{
		siTotalRate +=pclitem->clItemCommon.clOptionInfo.uiPara2;
	}
	
	return siTotalRate;
}



bool cltPItemInfo::IsBlankInv(cltItemManagerCommon*pclItemManager,SI32 invnum,SI32 invmode)
{
	SI32 index = 0 ;
	SI32 start = 0, end = 0;
	if(invmode == INVMODE_PERSON)
	{
		start	= PERSONITEM_INV0;
		end		= start + MAX_ITEM_PER_INVENTORY;
	}
	else if(invmode == INVMODE_HORSE)
	{
		start	= PERSONITEM_HORSE0;
		end		= start + MAX_ITEM_PER_INVENTORY;
	}
	else if(invmode == INVMODE_BAG)
	{
		start	= PERSONITEM_BAG0;
		end		= start + MAX_ITEM_PER_BAG;
	}
	else if(invmode == INVMODE_BAG_SUB)
	{
		start	= PERSONITEM_BAG1;
		end		= start + MAX_ITEM_PER_BAG;
	}
	else if(invmode == INVMODE_ALL)
	{
		start	= PERSONITEM_INV0;
		end		= start + MAX_INVITEM_PER_PERSON;
	}
	else
	{
		return false;
	}

	for (SI32 i = start ; i < end ; i ++ )
	{
		if ( clItem[i].siUnique == 0 && clItem[i].siItemNum == 0 )
		{
			index ++ ;
		}
	}

	if ( index >= invnum )
		return true ;

	return false ;
}

// 아주아주 특수한 경우(유니크가 같지만, 능력이 있고없고)에만 사용된다. - 반드시 추가되는 아이템 타입에 따라 추가가 되어야 한다.
bool cltPItemInfo::CanToitemPos(cltItemManagerCommon* pclItemManager, SI32 toItempos )
{
	// 결정체에 대해서만 검사한다.
	if(clItem[toItempos].GetType(pclItemManager) == ITEMTYPE_CRYSTAL)
	{
		if( clItem[toItempos].clItemCrystal.uiCrystalAbility != 0 || clItem[toItempos].clItemCrystal.uiCrystalAbilityNum != 0 )
				return false;
	}

	return true;
}



// 인벤토리에 아이템을 추가할 수 있는지 확인한다. 
// onlypileswitch중첩만 허용된다. 새로운 위치에 놓이는것은 금지된다. 
bool cltPItemInfo::CanAddInv(cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve, SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue, SI16* pCheckPos )
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

	SI32 start = 0, end = 0, searchrange = 0;
	if(invmode == INVMODE_PERSON)
	{
		start	= PERSONITEM_INV0;
		end		= start + MAX_ITEM_PER_INVENTORY;
		searchrange = MAX_ITEM_PER_INVENTORY;
	}
	else if(invmode == INVMODE_HORSE)
	{
		start	= PERSONITEM_HORSE0;
		end		= start + MAX_ITEM_PER_INVENTORY;
		searchrange = MAX_ITEM_PER_INVENTORY;
	}
	else if(invmode == INVMODE_BAG)
	{
		start	= PERSONITEM_BAG0;
		end		= start + MAX_ITEM_PER_BAG;
		searchrange = MAX_ITEM_PER_BAG;
	}
	else if(invmode == INVMODE_BAG_SUB)
	{
		start	= PERSONITEM_BAG1;
		end		= start + MAX_ITEM_PER_BAG;
		searchrange = MAX_ITEM_PER_BAG;
	}

	else if(invmode == INVMODE_ALL)
	{
		start	= PERSONITEM_INV0;
		end		= start + MAX_INVITEM_PER_PERSON;
		searchrange = MAX_ITEM_PER_INVENTORY;
	}
	else
	{
		MsgBox(TEXT("vdfd82"), TEXT("fdvkd:%d"), invmode);
		return false;
	}

	// 이미 같은 아이템이 존재하는지 확인한다. 
	// 같은 아이템이 존재하고 중첩된 개수가 최대 범위 안에 있으면 중첩개수를 늘인다. 
	if(pileswitch == TRUE)
	{
		for(i = start;i < end; i++)
		{
			if((clItem[i].siUnique == pclitem->siUnique)&&
				(clItem[i].CanPile(pclItemManager) == true))
			{
				if(clItem[i].GetItemNum() + pclitem->GetItemNum() <= MAX_ITEM_PILE_NUMBER)
				{
					// 마지막으로 결정된 장소의 현재아이템이 겹칠수 있는 아이템인지 검사한다.
					//if(CanToitemPos(pclItemManager, i) == true)
					{
						*pitempos = i;
						pcltoitem->Set(&clItem[i]);
						return true;
					}
				}
			}
		}
	}

	// 새롭게 추가한다. 
	for(i = start;i < start + searchrange;i++)
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

	// 중첩만 허용되는 경우라면, 신규위치는 검색하지 않는다. 
	if(invmode == INVMODE_ALL)
	{
		*prtnvalue = CANADDINV_FAIL_FULL;
		return false;
	}

	// 새롭게 추가한다. 
	for(i = (start + searchrange);i < end;i++)
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

// 인벤토리에 nIndexNum 개수 만큼의 아이템을 추가할 수 있는지 확인한다. 
// onlypileswitch중첩만 허용된다. 새로운 위치에 놓이는것은 금지된다. 
bool cltPItemInfo::CanAddInvManyItems(cltItemManagerCommon* pclItemManager, SI32 invmode, cltItem* pclitem, DWORD* pdwreserve,  SI32* prtnvalue)
{
	if(pclitem == NULL)return FALSE;

	SI32 i = 0, j = 0 , k = 0;
	BOOL pileswitch = FALSE;			// 아이템의 중첩 허용 여부. 
	BOOL bCanAdd = FALSE;				// 넣을수 있는지 여부.
	BOOL bAlreadyAdd = FALSE;			// 이미 넣을수 있는 자리로 체크되어 있는지 여부.
	SI32 itemPos[MAX_CANADDINV_CHECK_NUM]; // 넣을수 있다고 체크된 자리 기억배열. 

	SI32 start = 0, end = 0, searchrange = 0;
	if(invmode == INVMODE_PERSON)
	{
		start	= PERSONITEM_INV0;
		end		= start + MAX_ITEM_PER_INVENTORY;
		searchrange = MAX_ITEM_PER_INVENTORY;
	}
	else if(invmode == INVMODE_HORSE)
	{
		start	= PERSONITEM_HORSE0;
		end		= start + MAX_ITEM_PER_INVENTORY;
		searchrange = MAX_ITEM_PER_INVENTORY;
	}
	else if(invmode == INVMODE_BAG)
	{
		start	= PERSONITEM_BAG0;
		end		= start + MAX_ITEM_PER_BAG;
		searchrange = MAX_ITEM_PER_BAG;
	}
	else if( invmode == INVMODE_BAG_SUB )
	{
		start	= PERSONITEM_BAG1;
		end		= start + MAX_ITEM_PER_BAG;
		searchrange = MAX_ITEM_PER_BAG;
	}
	else if(invmode == INVMODE_ALL)
	{
		start	= PERSONITEM_INV0;
		end		= start + MAX_INVITEM_PER_PERSON;
		searchrange = MAX_ITEM_PER_INVENTORY;
	}
	else
	{
		MsgBox(TEXT("vdfd82"), TEXT("fdvkd:%d"), invmode);
		return false;
	}

	// nIndexNum 개수 만큼 반복하며 아이템을 넣을수 있는지 체크
	for(  j = 0; j < MAX_CANADDINV_CHECK_NUM ; j++ )
	{
		if( pclitem[j].siUnique <= 0 ) continue;

		bCanAdd = FALSE;

		// 중첩이 안되는 아이템인지 확인한다. 
		if( pclitem[j].CanPile(pclItemManager) == true)
		{
			pileswitch = TRUE;
		}
		else
		{
			pileswitch = FALSE;
		}

		// 이미 같은 아이템이 존재하는지 확인한다. 
		// 같은 아이템이 존재하고 중첩된 개수가 최대 범위 안에 있으면 중첩개수를 늘인다. 
		if(pileswitch == TRUE)
		{
			for( i = start;i < end; i++)
			{
				if((clItem[i].siUnique ==pclitem[j].siUnique)&&
					(clItem[i].CanPile(pclItemManager) == true))
				{
					//이미 들어갈 곳으로 판정된 곳이라면 다음으로 그냥 넘어간다. 
					bAlreadyAdd = FALSE;
					for( k = 0; k < MAX_CANADDINV_CHECK_NUM; k++ )
					{			
						if( itemPos[k] == i )
						{
							bAlreadyAdd = TRUE;
							break;
						}
					}
					if( bAlreadyAdd == TRUE)
						continue;

					if(clItem[i].GetItemNum() + pclitem[j].GetItemNum() <= MAX_ITEM_PILE_NUMBER)
					{
						// 마지막으로 결정된 장소의 현재아이템이 겹칠수 있는 아이템인지 검사한다.
						//if(CanToitemPos(pclItemManager, i) == true)
						{
							itemPos[j] = i;
							bCanAdd = TRUE;
							break;
						}
					}
				}
			}			

			if( bCanAdd == TRUE)
				continue;
		}
		
		// 새롭게 추가한다. - by LEEKH 2007.08.21 가방의 크기는 MAX_ITEM_PER_INVENTORY가 아니다.
		// 잘 동작하는 CanAddInv 함수의 기능을 가져옴
		//for(i = start;i < start + MAX_ITEM_PER_INVENTORY;i++)
		for(i = start;i < start + searchrange;i++)
		{
			if(clItem[i].siUnique)continue;

			//이미 들어갈 곳으로 판정된 곳이라면 다음으로 그냥 넘어간다. 
			bAlreadyAdd = FALSE;
			for( k = 0; k < MAX_CANADDINV_CHECK_NUM; k++ )
			{			
				if( itemPos[k] == i )
				{
					bAlreadyAdd = TRUE;
					break;
				}
			}
			if( bAlreadyAdd == TRUE)
				continue;

			// 예약되어 있는 자리라면, 
			if(pdwreserve && pdwreserve[i])continue;

			itemPos[j] = i;
			bCanAdd = TRUE;
			break;
		}
		if( bCanAdd == TRUE)
			continue;


		// 중첩만 허용되는 경우라면, 신규위치는 검색하지 않는다. 
		if(invmode == INVMODE_ALL)
		{
			*prtnvalue = CANADDINV_FAIL_FULL;
			return false;
		}

		// 새롭게 추가한다. - by LEEKH 2007.08.21 - 잘 동작하는 CanAddInv 함수의 기능을 가져옴
		//for(i = (start + MAX_ITEM_PER_INVENTORY);i < end;i++)
		for(i = (start + searchrange);i < end;i++)
		{
			if(clItem[i].siUnique)continue;

			//이미 들어갈 곳으로 판정된 곳이라면 다음으로 그냥 넘어간다. 
			bAlreadyAdd = FALSE;
			for( k = 0; k < MAX_CANADDINV_CHECK_NUM; k++ )
			{			
				if( itemPos[k] == i )
				{
					bAlreadyAdd = TRUE;
					break;
				}
			}
			if( bAlreadyAdd == TRUE)
				continue;

			// 예약되어 있는 자리라면, 
			if(pdwreserve && pdwreserve[i])continue;

			itemPos[j] = i;
			bCanAdd = TRUE;
			break;
		}
		if( bCanAdd == TRUE)
			continue;

		// 여기까지 그냥 빠져 나왔다면 들어갈곳이 없다는것이다.
		*prtnvalue = CANADDINV_FAIL_FULL;
		return false;
	}
	return true;

}

// 인벤토리에 아이템을 추가한다. 
bool cltPItemInfo::AddInv(cltItemManagerCommon* pclItemManager, cltItem* pclitem)
{
	SI16 itempos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(CanAddInv(pclItemManager, INVMODE_ALL, pclitem, NULL, &itempos, &cltoitem, &rtnvalue) == true)
	{
		clItem[itempos].Add(pclItemManager, pclitem);
		return true;
	}

	return false;

}


// 인벤토리 아이템중에서 하나를 랜덤하게 얻어온다. 
bool cltPItemInfo::RobInv(SI32* pitempos, cltItem* pclitem)
{
	SI32 i;

	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_ITEM_PER_INVENTORY;i++)
	{
		if(clItem[i].siUnique)
		{
			*pitempos = i;
			pclitem->Set(&clItem[i]);
			//KHY - 0706 - 기간제 아이템의 경우 기간을 넣어준다.
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
			pclitem->uiDateUseDay = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			
			return true;
		}
	}

	return false;
}


bool cltPItemInfo::IsSame(cltPItemInfo* pcliteminfo, bool numcomp )
{
	SI32 i;

	if(pcliteminfo ==  NULL)return FALSE;

	for(i = 0;i < MAX_ITEM_PER_PERSON;i++)
	{
		if(clItem[i].IsSame(&pcliteminfo->clItem[i], numcomp) == FALSE)return false;
	}

	return true;
}


// 주어진 아이템과 정확하게 일치하는 아이템을 찾아서 그 포인터를 리턴한다. 
SI32 cltPItemInfo::FindItemInv(cltItem* pclitem, bool numcomp)
{
	SI32 i;

	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(clItem[i].siUnique)
		{
			if(clItem[i].IsSame(pclitem, numcomp) == TRUE)
			{
				return i;
			}
		}
	}

	return -1;
}

// 주어진 아이템과 정확하게 일치하는 아이템을 찾아서 그 위치를 리턴한다. 
SI32 cltPItemInfo::FindItemInv(SI32 itemunique)
{
	SI32 i;

	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(clItem[i].siUnique)
		{
			if(clItem[i].siUnique == itemunique)
			{
				return i;
			}
		}
	}

	return -1;
}


// unique의 아이템을 몇개나 갖고 있는가 ?
SI32  cltPItemInfo::GetItemNum(SI32 itemunique)
{
	SI32 i;
	SI32 haveitemnum = 0;
	
	if(itemunique <= 0)return 0;

	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(clItem[i].siUnique == itemunique)
		{
			haveitemnum += clItem[i].siItemNum;
		}
	}

	return haveitemnum;
}

// 장비창안에 unique의 아이템을 몇개나 갖고 있는가?
SI32 cltPItemInfo::GetItemNumInEquip(SI32 itemunique)
{
	SI32 i;
	SI32 haveitemnum = 0;

	if(itemunique <= 0)return 0;

	for(i = 0;i < PERSONITEM_INV0; i++)
	{
		if(clItem[i].siUnique == itemunique)
		{
			haveitemnum += clItem[i].siItemNum;
		}
	}

	return haveitemnum;
}

// unique의 아이템이 인벤에 존재하는가 ?
SI32  cltPItemInfo::GetItemPos(SI32 itemunique)
{
	SI32 i;
	
	if(itemunique <= 0)return 0;

	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
	{
		if(clItem[i].siUnique == itemunique)
		{
			return i;
		}
	}

	return -1;
}

// unique의 아이템이 인벤에 존재하는가 ?
SI32  cltPItemInfo::GetItemTotalPos(SI32 itemunique)
{
	SI32 i;

	if(itemunique <= 0)return 0;

	for( i = 0;i < MAX_ITEM_PER_PERSON ; i++ )
	{
		if(clItem[i].siUnique == itemunique)
		{
			return i;
		}
	}

	return -1;
}

// 장비창안에 unique의 아이템의 위치 리턴
SI32  cltPItemInfo::GetItemPosInEquip(SI32 itemunique)
{
	SI32 i;

	if(itemunique <= 0)return 0;

	for( i = 0;i < PERSONITEM_INV0 ; i++ )
	{
		if(clItem[i].siUnique == itemunique)
		{
			return i;
		}
	}

	return -1;
}


// 특정 아이템들이 이 인벤에 들어갈 수 있는가. 
// 들어갈 수 있다면 어느 위치에 들어갈 수 있는가를 구해낸다. 
bool cltPItemInfo::CanInputItemList(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32 itemlistnum, SI16* pclpos)
{

	SI32 i;
	cltItem clitem;
	SI32 rtnvalue;

	SI16 checkPos[MAX_PRIVATETRADEORDER_ITEM_NUM] = {0, 0, 0, 0, 0};

	for(i = 0; i < itemlistnum;i++)
	{
		if(pclitem[i].siUnique<= 0)continue;

		if(CanAddInv(pclItemManager, INVMODE_ALL, &pclitem[i], NULL, &pclpos[i], &clitem, &rtnvalue, checkPos ) == false)
		{
			return false;
		}
		checkPos[i] = *(&pclpos[i]);
	}

	return true;

}

// 특정 아이템이 장비품에 장비되어 있는가 ?
bool cltPItemInfo::HaveInEquip(SI32 itemunique)
{
	SI32 i;

	for(i = 0;i < PERSONITEM_INV0;i++)
	{
		if(clItem[i].siUnique && clItem[i].siUnique == itemunique)
		{
			return true;
		}
	}

	return false;
}

// 모든 장비품이 정상적으로 장비되어 있는가 ?
bool cltPItemInfo::IsAllProperEquip(cltItemManagerCommon* pclItemManager, SI16* punique, SI16* ppos, SI32 siKind)
{

	SI32 i;

	for(i = 0;i < PERSONITEM_INV0;i++)
	{
		if(clItem[i].siUnique <= 0)continue;

		if(pclItemManager->CheckProperItemPos(&clItem[i], i, siKind) == FALSE)
		{
			*punique	= clItem[i].siUnique;
			*ppos		= i;
			return false;
		}
	}

	return true;
}

// 말이 물품을 가지고 있는가?
bool cltPItemInfo::IsHorseHaveItem()
{
	SI32 i;

	for(i = PERSONITEM_HORSE0;i < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY;i++)
	{
		if(clItem[i].siUnique)return true;
	}

	return false;
}

// 가방에  물품을 가지고 있는가?
bool cltPItemInfo::IsBagHaveItem()
{
	SI32 i;

	for(i = PERSONITEM_BAG0;i < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG;i++)
	{
		if(clItem[i].siUnique)return true;
	}

	return false;
}


// 무기 사용 부적을 사용한 것이 있는가 ?
// skill : SKILL_USESWORD1 ...
bool cltPItemInfo::UseWeasponSkillScroll(cltItemManagerCommon* pclItemManager,SI32 skill)
{

	SI32 up = clItem[PERSONITEM_HELMET].GetSkillScrollUp(pclItemManager, skill)
			+ clItem[PERSONITEM_HAT].GetSkillScrollUp(pclItemManager, skill)
			+ clItem[PERSONITEM_DRESS].GetSkillScrollUp(pclItemManager, skill)
			+ clItem[PERSONITEM_ARMOUR].GetSkillScrollUp(pclItemManager, skill);

	if(up)return true;

	return false;
}

UI32 cltPItemInfo::UseWeasponSkillScrollNum(cltItemManagerCommon* pclItemManager,SI32 skill)
{

	UI32 up = clItem[PERSONITEM_HELMET].GetSkillScrollUp(pclItemManager, skill)
		+ clItem[PERSONITEM_HAT].GetSkillScrollUp(pclItemManager, skill)
		+ clItem[PERSONITEM_DRESS].GetSkillScrollUp(pclItemManager, skill)
		+ clItem[PERSONITEM_ARMOUR].GetSkillScrollUp(pclItemManager, skill);

	return up;
}

bool cltPItemInfo::IsHaveMaterialItem( cltItemManagerCommon* pclItemManager, SI32 siItemUnique, SI32 siAmount )
{
	if ( NULL == pclItemManager )
	{
		return false;
	}

	SI32 siReference = pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= siReference )
	{
		return false;
	}

	cltItemInfo* pclMaterialInfo = (cltItemInfo*)pclItemManager->pclItemInfo[ siReference ];
	if ( pclMaterialInfo == NULL )
	{
		return false;
	}

	SI32 siMaterialUnique[MAX_MATERIAL_NUMBER]	= {0, };
	SI32 siMaterialAmount[MAX_MATERIAL_NUMBER]	= {0, };
	SI32 siDoNotHaveCount						= 0;

	// 해당아이템의 필요한 재료아이템을 구해온다
	for ( SI32 siIndex=0; siIndex<MAX_MATERIAL_NUMBER; ++siIndex )
	{
		SI32 siGetUnique	= pclMaterialInfo->clProductionInfo.clMaterial[ siIndex ].siUnique;
		SI32 siGetAmount	= pclMaterialInfo->clProductionInfo.clMaterial[ siIndex ].siNeedNum;
		bool bSearchResult	= false;


		if ( (0 >= siGetUnique) || (0 >= siGetAmount) )
		{
			// 이 갯수가 MAX_MATERIAL_NUMBER 이상일때는 재료아이템이 없는 것이므로 이것또한 false로 간주한다
			++siDoNotHaveCount;
			continue;
		}
		
		if ( 0 < siAmount )
		{
			siGetAmount = siGetAmount * siAmount;
		}

		// 실재 내 인벤토리에 재료 아이템이 있는지 검사
		for ( SI32 siPos=MAX_SHAPE_ITEM; siPos<MAX_ITEM_PER_PERSON; ++siPos )
		{
			if ( (clItem[siPos].siUnique == siGetUnique) && (clItem[siPos].siItemNum >= siGetAmount) )
			{
				bSearchResult = true;
				break;
			}
		}
		
		// 아이템이 없거나 갯수가 충분하지않다면 
		if ( false == bSearchResult )
		{
			return false;
		}
	}

	if ( MAX_MATERIAL_NUMBER <= siDoNotHaveCount )
	{
		return false;
	}

	return true;

}



