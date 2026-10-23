//---------------------------------
// 2003/8/7 김태곤
//---------------------------------
#include "cltItem.h"
#include "CommonLogic.h"

extern cltCommonLogic* pclClient;

// 특정 아이템이 개조가능한 것인지 여부. 
bool cltItemManagerCommon::CanTune(cltItem* pclitem)
{
	if(pclitem == NULL)return false;
	if(pclitem->siUnique <= 0)return false;

	// 중첩이 안되는 아이템인지 확인한다. 
	SI32 type = pclitem->GetType(this);
	if(type <= 0)return false;

	SI64 atb = pclClient->pclItemManager->pclItemTypeInfo[type]->siAtb;

	// object 타입이어야 한다. 
	if((atb & ITEMATB_OBJECT) == 0)return false;

	// 최대 내구도가 MAX_DUR_FOR_TUNE이하여야 한다. 
	if(pclitem->clItemCommon.clCommonInfo.uiMaxDurability > MAX_DUR_FOR_TUNE)return false;

	// 밀봉중이 아니어야 한다.
	if ( pclitem->IsSealingUp(this) == true )
		return false ;

	// 평범품이면 가능. 
	if(pclitem->clItemCommon.clCommonInfo.uiRareInfo != 0 && pclitem->clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_1)return false;

	SI32 ref = FindItemRefFromUnique(pclitem->siUnique) ;

	if ( ref )
	{
		if ( pclItemInfo[ref]->siSetItemIndex > 0 )
			return false ;
	}
	else return false ;

	return true;
}

// 개조후의 아이템으로 변경한다. 
bool cltItemManagerCommon::GetTuneItem(cltItem* pclitem,bool bSuccess,SI32 mode)
{
	if(CanTune(pclitem) == false)return false;

	SI32 ref = FindItemRefFromUnique(pclitem->siUnique);
	if(ref <= 0)return false;
	


	// 평범품이면 가능. 
	if(pclitem->clItemCommon.clCommonInfo.uiRareInfo == 0)
	{
		if( (rand() % 7) && !bSuccess )return false;

		// 내구도를 회복시켜준다. 
		pclitem->clItemCommon.clCommonInfo.uiMaxDurability  = pclItemInfo[ref]->clItem.clItemCommon.clCommonInfo.uiMaxDurability;
		pclitem->clItemCommon.clCommonInfo.uiDurability		= pclitem->clItemCommon.clCommonInfo.uiMaxDurability;

		pclitem->clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_1;	// 호품으로 만들어준다.  
	}
	// 호품이면 가능. 
	else if(pclitem->clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1)
	{
		if( (rand() % 10) && !bSuccess )return false;

		// 내구도를 회복시켜준다. 
		pclitem->clItemCommon.clCommonInfo.uiMaxDurability  = pclItemInfo[ref]->clItem.clItemCommon.clCommonInfo.uiMaxDurability;
		pclitem->clItemCommon.clCommonInfo.uiDurability		= pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
	
		// 명품으로 만들어준다. 
		MakeRareItem(pclitem,mode);


		pclitem->clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_2;	// 명품으로 만들어준다.  

	}

	return true;
}


// 장비품 성능 향상을 위한 부적 요구 정보를 구한다. 
bool cltItemManagerCommon::GetScrollForTune2(cltItem clitem, SI32* pscrollunique, SI32* pscrollnum, cltItem* pcldestitem, SI32* preturnval)
{
	SI32 ref = FindItemRefFromUnique(clitem.siUnique);
	if(ref <= 0)return false;

	if ( pclItemInfo[ref]->siSetItemIndex > 0 )
		return false ;

	// 오브젝트 물건이어야 한다. 
	SI32 itemtype = pclItemInfo[ref]->siType;
	if(pclItemTypeInfo[itemtype]->siAtb & ITEMATB_OBJECT)
	{
	}
	else
	{
		*preturnval	= 0;
		return false;
	}

	pcldestitem->Set(&clitem);
	pcldestitem->DecreaseDurability(this, 40);
	if(pcldestitem->GetDurability() == 0)
	{
		*preturnval	= 0;
		return false;
	}

	// 부적의 요구 장수는 10장. 
	*pscrollnum		= 10;


	bool bweaponswitch = false;
	bool barmourswitch = false;
	switch(pclItemInfo[ref]->siType)
	{
	case ITEMTYPE_SWORD:	
		*pscrollunique	= ITEMUNIQUE(8845); // 검제작부적. 
		bweaponswitch = true;
		break;
	case ITEMTYPE_SPEAR:
		*pscrollunique	= ITEMUNIQUE(8835); // 창제작부적. 
		bweaponswitch = true;
		break;
	case ITEMTYPE_AXE:
		*pscrollunique	= ITEMUNIQUE(8830); // 도끼제작부적. 
		bweaponswitch = true;
		break;
	case ITEMTYPE_STAFF:
		*pscrollunique	= ITEMUNIQUE(8850); // 지팡이제작부적. 
		bweaponswitch = true;
		break;

	case ITEMTYPE_BOW:
		*pscrollunique	= ITEMUNIQUE(8840); // 활제작부적. 
		bweaponswitch = true;
		break;

	case ITEMTYPE_GUN:
		*pscrollunique	= ITEMUNIQUE(8855); // 총제작부적. 
		bweaponswitch = true;
		break;

	case ITEMTYPE_BELT:
		*pscrollunique	= ITEMUNIQUE(8875); // 요대제작부적. 
		barmourswitch = true;
		break;

	case ITEMTYPE_HELMET:
		*pscrollunique	= ITEMUNIQUE(8590); // 투구제작부적. 
		barmourswitch = true;
		break;

	case ITEMTYPE_HAT:
		*pscrollunique	= ITEMUNIQUE(8610); // 모자제작부적. 
		barmourswitch = true;
		break;

	case ITEMTYPE_ARMOUR:
		*pscrollunique	= ITEMUNIQUE(8580); // 갑옷제작부적. 
		barmourswitch = true;
		break;

	case ITEMTYPE_DRESS:
		*pscrollunique	= ITEMUNIQUE(8600); // 의복제작부적. 
		barmourswitch = true;
		break;

	case ITEMTYPE_SHOES:
		*pscrollunique	= ITEMUNIQUE(8620); // 신발제작부적. 
		barmourswitch = true;
		break;

/*	case ITEMTYPE_NECK:
		*pscrollunique	= ITEMUNIQUE(8640); //  목걸이제작부적. 
		barmourswitch = true;
		break;

	case ITEMTYPE_RING:
		*pscrollunique	= ITEMUNIQUE(8650); // 반지제작부적. 
		barmourswitch = true;
		break;
*/

	}

	if(bweaponswitch)
	{
		// 오리지널 제품의 성능을 구한다. 
// 부적 시스템은 현재 사용하지 않는다.
/*		SI32 orgmindamage = pclItemInfo[ref]->clItem.GetMinDamage(this);
		SI32 orgmaxdamage = pclItemInfo[ref]->clItem.GetMaxDamage(this);

		// 최소 데미지가 오리지널 제품보다 SCROLL_TUNE_MAX_RATE배이상이다.
		if(pcldestitem->GetMinDamage(this) > (orgmindamage*SCROLL_TUNE_MAX_RATE) )
		{
			*preturnval	= SCROLLTUNE_ERR_MINDAMAGE;
			return false;
		}
		// 최대 데미지가 오리지널 제품보다 SCROLL_TUNE_MAX_RATE배이상이다.
		if(pcldestitem->GetMaxDamage(this) > (orgmaxdamage*SCROLL_TUNE_MAX_RATE) )
		{
			*preturnval	= SCROLLTUNE_ERR_MAXDAMAGE;
			return false;
		}


		pcldestitem->SetMinDamage(this, pcldestitem->GetMinDamage(this) + 2);
		pcldestitem->SetMaxDamage(this, pcldestitem->GetMaxDamage(this) + 2);
		return true;
*/
	}

	if(barmourswitch)
	{
		SI32 orgac = pclItemInfo[ref]->clItem.GetAC(this);

		// AC가 오리지널 제품보다 SCROLL_TUNE_MAX_RATE배이상이다.
		if(pcldestitem->GetAC(this) > (orgac*SCROLL_TUNE_MAX_RATE) )
		{
			*preturnval	= SCROLLTUNE_ERR_AC;
			return false;
		}

		pcldestitem->SetAC(this, pcldestitem->GetAC(this) + 4);
		return true;
	}

	return false;
}