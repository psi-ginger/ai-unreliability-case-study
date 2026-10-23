//---------------------------------
// 2003/8/7 김태곤
//---------------------------------

#include "Item-ManagerServer.h"
#include "..\Server\Server.h"
#include "../../Server/Minister/MinisterBase.h"
#include "../../Server/Minister/MinisterMgr.h"
#include "../../Server/Minister/Minister-Izo.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharServer\Char-Server.h"

#include "item/ItemCommon/cltSetItem.h"

extern cltCommonLogic* pclClient;

cltItemManagerServer::cltItemManagerServer(SI32 gamemode) : cltItemManagerCommon()
{
	//pclSetItemManager = new CSetItemManager;
	//pclSetItemManager->LoadSetItemInfo();
}

cltItemManagerServer::~cltItemManagerServer()
{
	if( pclSetItemManager)
	{
		delete pclSetItemManager;
		pclSetItemManager = NULL;
	}
}

// KindInfo의 아이템 정보를 근거로 랜덤한 아이템을 하나 만든다. 
bool cltItemManagerServer::MakeRandItem(SI32 level, SI32 addrate, cltKindItemProbabilitySet* pclikinditem, cltItem* pclitem)
{
	cltItem clItem;

	// KindInfo에 있는 아이템의 종류를 얻어온다. 
	SI32 siitemunique, itemnum;
	if(pclikinditem->GetRandDropItem(level, addrate, &siitemunique, &itemnum) == true)
	{
		if(siitemunique == 0)return false;
		
		// 아이템의 Unique정보를 토대로 랜덤하게 아이템을 하나 만든다.
		SI16 brareswitch = 0;
		if(MakeRandItemUnique(siitemunique, pclitem, 0, 0, &brareswitch) == true)
		{
			pclitem->siItemNum	= itemnum;
			return true;
		}
	}


	return false;
}

// 해체시의 원료 리스트를 얻어온다. (리스트는 깨끗하게 지워진 상태에서 전달되어야 한다.) 
bool cltItemManagerServer::GetDismantleMaterial(cltCharServer* pclchar, cltItem clitem, cltItem* pclitemlist, SI16 *pMakePercent )
{
	if ( pMakePercent == NULL )
	{
		return false;
	}

	// 해당 아이템의 타입을 구한다.
	SI32 itemtype = clitem.GetType(this);
	if(itemtype == 0)return false;

	if(pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) == false)return false;

	// 내구도가 DUR_FOR_DISMANTLE 이하여야 한다. 
	if ( pclchar != NULL && pclchar->GetGameMasterLevel() > BASIC_GAMEMASTER_LEVEL )
	{
	}
	else if(clitem.GetDurability() > DUR_FOR_DISMANTLE)
		return false;

	SI32 ref = FindItemRefFromUnique(clitem.siUnique);
	if(ref <= 0)return false;

	// 제조 불가 물품. 
	if(pclItemInfo[ref]->clProductionInfo.clMaterial[0].siUnique == 0)return false;

	// 고물처리술 레벨을 얻어온다. 
	//SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ALCHEMY, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);
	SI32 skilllevel = 0;
	if(skilllevel <= 0)return false;

	/*
	SI16 itemgetrate = 40;

	if(skilllevel > 30)
	{
	itemgetrate += (skilllevel - 30) / 2;
	skilllevel = 30;
	}

	itemgetrate += skilllevel/2;
	*/

	// 원료 획득률을 구한다. 
	SI16 itemgetrate = 40 + skilllevel * 2 / 3 + ( skilllevel - 30 ) /2;

	cltServer* pclserver = (cltServer*)pclClient;
	CMinisterIzo* pclminister = (CMinisterIzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_IZO);
	SI32 rate = min(75, max(50, pclminister->m_siPriceValue));
	itemgetrate += ( 75 - rate ) * (-0.5 );

	if ( itemgetrate < 40 )
	{
		itemgetrate = 40;
	}

	if( itemgetrate > 80 )
	{
		itemgetrate = 80;
	}

	*pMakePercent = itemgetrate;

	SI32 index = 0;
	for(SI32 i = 0;i < MAX_MATERIAL_NUMBER;i++)
	{
		// 원료 아이템이 무효해서는 안된다. 
		SI32 itemunique = pclItemInfo[ref]->clProductionInfo.clMaterial[i].siUnique;
		if(itemunique == 0)continue;

		SI32 tempref = FindItemRefFromUnique(itemunique);
		if(ref <= 0)continue;

		// 원료가 Object여서는 안된다. 
		SI32 tempitemtype = pclItemInfo[tempref]->siType;
		if(pclItemTypeInfo[tempitemtype]->IsAtb(ITEMATB_OBJECT) == TRUE)continue;

		// 개수를 설정한다. 
		SI32 itemnum = pclItemInfo[ref]->clProductionInfo.clMaterial[i].siNeedNum * itemgetrate / 100;
		if(itemnum <= 0)continue;


		pclitemlist[index].Set( &pclItemInfo[tempref]->clItem);
		pclitemlist[index].SetItemNum(itemnum);
		index++;

	}

	if(index)
	{
		return true;
	}

	return false;


}

/*
bool cltItemManagerServer::GetEnchantDamageFromGrade(SI32 siGrade)
{



}*/

