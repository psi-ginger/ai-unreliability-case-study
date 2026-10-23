//---------------------------------
// 2003/6/26 김태곤
//---------------------------------

#include <Stdio.h>

#include "House.h"
#include "..\..\Common\Item\ItemCommon\cltItem.h"
#include "..\CityHall\TaxPaymentListDlg.h"

//--------------------------------------------------------
// cltHouseUnitStorageInfo
//--------------------------------------------------------
// 창고에  아이템을 추가할 수 있는지 확인한다. 
bool cltHouseUnitStorageInfo::CanAddStg(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos)
{
	SI32 i;
	BOOL pileswitch = FALSE;			// 아이템의 중첩 허용 여부. 
	if(pclitem == NULL)return FALSE;

	// 중첩이 안되는 아이템인지 확인한다. 
	if(pclitem->CanPile(pclItemManager ) == true)
	{
		pileswitch = TRUE;
	}
	else
	{
		pileswitch = FALSE;
	}

	// 이미 같은 아이템이 존재하는지 확인한다. 
	// 같은 아이템이 존재하고 중첩된 개수가 최대 범위 안에 있으면 중첩개수를 늘인다. 
	SI32 siItemCount = 0;
	if(pileswitch == TRUE)
	{
		for(i = 0;i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER;i++)
		{
			if( clItem[i].siItemNum == MAX_ITEM_PILE_NUMBER )
				continue;

			siItemCount = clItem[i].siItemNum + pclitem->siItemNum;
			if (clItem[i].siUnique == pclitem->siUnique && siItemCount > MAX_ITEM_PILE_NUMBER)
			{
				pclitem->siItemNum = pclitem->siItemNum - (siItemCount - MAX_ITEM_PILE_NUMBER);
			}

			if(clItem[i].siUnique == pclitem->siUnique && (clItem[i].siItemNum + pclitem->siItemNum <= MAX_ITEM_PILE_NUMBER))
			{
				//[기형] 2010.1.26 창고안에 능력 크리스탈과 미 능력 크리스탈이 존재시 인벤의 미능력 크리스탈을 창고 안에 넣으려고 할 경우
				// 기존의 미능력크리스탈과 중첩되지 않는 부분을 수정합니다.
				if(ITEMTYPE_CRYSTAL == pclitem->GetType(pclItemManager))
				{
					if( true == clItem[i].CanPile(pclItemManager) && true == pclitem->CanPile(pclItemManager))
					{
						*pitempos = i;
						return true;
					}
				}
				else
				{
					*pitempos = i;
					return true;
				}
			}
		}
	}

	// 새롭게 추가한다. 
	for(i = 0;i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER;i++)
	{
		if(clItem[i].siUnique == 0)
		{
			*pitempos = i;
			return true;
		}
	}

	return false;
}

// 창고에 있는 아이템의 위치를 구한다.
SI16 cltHouseUnitStorageInfo::GetStgExistItemPos( SI16 ItemUnique, SI16 ItemNum )
{
	if ( ItemUnique < 1 )
	{
		return -1;
	}

	for( SI16 i = 0;i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
	{
		if( clItem[i].siUnique > 0 &&
			clItem[i].siUnique == ItemUnique &&
			clItem[i].siItemNum >= ItemNum )
		{
			return i;
		}
	}
	
	return -1;
}

// 창고에  말을 추가할 수 있는지 확인한다. 
bool cltHouseUnitStorageInfo::CanAddHorse( SI16 *pHorsePos )
{
	for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
	{
		// 마구간이 있어야 하고, 마구간 안에 말은 없어야 함으로...
		if ( bCreatedHouseUnitStable[ i ] == true && clHorse[ i ].siHorseUnique == 0 )
		{
			*pHorsePos = i;

			return true;
		}
	}	
	
	return false;
}

bool cltHouseUnitStorageInfo::IsHorseExist( SI16 HorsePos )
{
	if ( bCreatedHouseUnitStable[ HorsePos ] == true && clHorse[ HorsePos ].siHorseUnique > 0 )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------
// cltHouse
//--------------------------------------------------------
cltHouse::cltHouse(SI32 sivillageunique): cltStructureBasic(sivillageunique)
{
	
}

cltHouse::~cltHouse() 
{
}

// 같은 계약조건인지 확인한다. 
BOOL cltHouse::IsSameContract(const cltRentContract* pclcontract)
{
	if(clStrInfo.clHouseStrInfo.clRentContract.IsSame(pclcontract) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

// 임차 후보자로 신청한다. 
BOOL cltHouse::AddApplyer(const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		if(clApply[i].clPerson.GetPersonID() == 0)
		{
			clApply[i].Set(pclapplyer, pclcontract);
			return TRUE;
		}
	}

	return FALSE;
}

// 임차 신청자를 삭제한다. 
BOOL cltHouse::DeleteApplyer(SI32 personid)
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		if(clApply[i].clPerson.GetPersonID() == personid)
		{
			clApply[i].Init();
			return TRUE;
		}
	}

	return FALSE;
}

// 빈집인지 여부를 확인한다. 
BOOL cltHouse::IsEmpty(SI32 slot)
{
	if(clUnit[slot].clPerson.GetPersonID() ) return FALSE;

	return TRUE;
}

//PersonID를 가지고 희망자의 정보를 구한다. 
cltRentApplyer* cltHouse::GetHouseApplyerFromPersonID(SI32 personid)
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		if(clApply[i].clPerson.GetPersonID() == personid)
		{
			
			return &clApply[i];
		}
	}
	return NULL;
}

// 임차인을 시전에 등록한다.
BOOL cltHouse::SetRent(SI32 slot, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract)
{
	clUnit[slot].clPerson.Set(pclapplyer);
	clUnit[slot].clContract.Set(pclcontract);

	return TRUE;
}

// 특정 Person이 단위 시전의 주인인지 확인한다. 
bool cltHouse::IsHouseHost(SI32 personid, SI32 houseunitslot)
{
	if(personid == 0)return false;
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return false;

	if(clUnit[houseunitslot].clPerson.GetPersonID() == personid)return true;

	return false;
}

// 창고에  아이템을 추가할 수 있는지 확인한다. 
bool cltHouse::CanAddStg(SI32 houseunitslot, cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos)
{
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return false;

	return clUnit[houseunitslot].clStorageInfo.CanAddStg(pclItemManager, pclitem, pitempos);
}

// 아이템을 창고에 넣는다.
void cltHouse::SetItemStg(SI32 houseunitslot, SI32 stgpos, cltItem* pclitem)
{
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return ;

	clUnit[houseunitslot].clStorageInfo.SetItemStg(stgpos, pclitem);
}


// 창고 아이템의 포인터를 구한다. 
cltItem* cltHouse::GetStgItemPointer(SI32 houseunitslot, SI32 stgpos)
{
	if(houseunitslot < 0 || houseunitslot >= MAX_HOUSEUNIT_NUMBER)return NULL;

	return clUnit[houseunitslot].clStorageInfo.GetStgItemPointer(stgpos);

}

// 각 시전의 임대료를 징수한다.
void cltHouse::GetRentFee()
{
	SI32 i;

	// 단위 시전을 검색하여 임대료를 징수한다. 
	for(i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		// 임대가 되어 있는 물건만 검색한다.
		SI32 personid = clUnit[i].clPerson.GetPersonID();
		if( personid <= 0)continue;

		// 월세를 확인한다. 
		GMONEY fee = clUnit[i].clContract.siRentFee;



	}
}
