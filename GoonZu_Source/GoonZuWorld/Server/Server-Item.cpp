//---------------------------------
// 2003/6/15 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

//-----------------------------------------
// Server
//-----------------------------------------
#include "../Server/Statistics/Statistics.h"

#include "../Server/GachaManager/GachaManager.h"
#include "../Server/ArtifactRestoreManager/ArtifactRestoreManager.h"
#include "../Server/ServerBoxItem/ServerBoxItem.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
//데일리 퀘스트 3
#include "../../../DBManager/GameDBManager_World/DBMsg-DailyQuest3.h"

#include "..\CommonLogic\MsgType-Item.h"
#include "../CommonLogic/Msg/MsgType-DailyQuest3.h"

#include "Util/Util.h"
#include "MsgRval-Define.h"
#include "../Client/minigame/ZooKeeperLogic.h"

#include "../CommonLogic/MagicMgr/MagicBaseInfo.h"
#include "../CommonLogic/MagicMgr/MagicMgr.h"
#include "../CommonLogic/BoxItemMgr/BoxItemMgr.h"

#include "..\CommonLogic\MsgType-Person.h"
#include "../../common/Char/CharServer/Char-Item.h"

#include "../common/Event/event.h"

#include "../../Common/Util/Util.h"

#include "NDate.h"

#include "../../Common/GameMaster/GameMaster.h"

#include "../../common/Item/ItemCommon/cltItem.h"// [성웅] 레벨 보상 상자에서 아이템을 준다 

// NPC수리 요청. 
void cltServer::DoMsg_GAMEMSG_REQUEST_NPCREPAIR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NPCRepair* pclinfo = (cltGameMsgRequest_NPCRepair*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)return ;

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	SI32 itempos = pclinfo->siPos;
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;

	//---------------------------------------------
	// 해당 아이템이 실제로 존재하는지 확인한다. 
	//---------------------------------------------
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;
	cltItem* pclitem = &pclinfo->clItem;
	cltItem* pclorgitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[itempos];
	// 주어진 아이템과 같은 아이템인지 확인한다. 
	if(pclitem->IsSame(pclorgitem, false) == false)
	{
		return ;
	}
	if(pclitem->siUnique <= 0)return ;
	// 수리 불가능한 아이템이어도 실패.
	if(pclitem->CanRepair(pclItemManager) == false)return ;

	//--------------------------------------------
	// 수리 정도를 확인한다. 
	//--------------------------------------------
	SI32 decdur = MIN_DUR_FOR_REPAIR;

	// 기술 수준에 따라 소모되는 내구도를 구한다. 
	decdur = GetWasteDurability(30);

	//--------------------------------------------
	// 수리비를 구한다. 세트아이템은 2만냥에 무수리 시세
	//--------------------------------------------
	SI32 ref = pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
	if(ref <= 0)
		return ;
	GMONEY repairmoney = 0 ;
	if ( pclItemManager->pclItemInfo[ref]->siSetItemIndex > 0 )
	{
		repairmoney = 20000 * pclItemManager->clItemStatistic.siNPCPriceRate / 100 ;
	}
	else
	{
		repairmoney = pclItemManager->GetNPCRepairPrice(pclitem);
	}

	if(repairmoney <= 0)return ;

	cltItem clitem;
	clitem.Set(pclitem);

	if(clitem.Repair(pclItemManager, decdur) == true)
	{
		//--------------------------------------------
		// 수리 하도록 DB매니저에게 요청한다. 
		//--------------------------------------------
		SI32 personid		= pclCM->CR[id]->pclCI->GetPersonID();
		if( pclCM->IsValidID(pclinfo->siCharUnique))
		{
			SI32 VillageUnique = pclCM->CR[ pclinfo->siCharUnique ]->GetCurrentVillageUnique();

			sDBRequest_InvRepair clMsg(id, personid, itempos, &clitem, 0, repairmoney, VillageUnique);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		}

		// 통화량 조절 
		cltMoney clmoney;
		clmoney.Set(repairmoney);
		clCurrencyInfo.clOutputMoney_Repair.Increase(&clmoney);
	}
}

// NPC수리 가격 요청. 
void cltServer::DoMsg_GAMEMSG_REQUEST_NPCREPAIRPRICE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NPCRepairPrice* pclinfo = (cltGameMsgRequest_NPCRepairPrice*)pclMsg->cData;

	if(pclinfo->clItem.siUnique <= 0)return ;

	GMONEY repairprice = pclItemManager->GetNPCRepairPrice(&pclinfo->clItem);

	if(repairprice)
	{
		cltGameMsgResponse_NPCRepairPrice clinfo(pclinfo->clItem.siUnique, repairprice);
		cltMsg clMsg(GAMEMSG_RESPONSE_NPCREPAIRPRICE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}

// WEAPON 1,2 Activate변경 
void cltServer::DoMsg_GAMEMSG_REQUEST_EXCHANGEWEAPON(cltMsg* pclMsg, SI32 id)
{    
	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if ( NULL == pclchar )
	{
		return;
	}

	//[추가 : 황진성 2008. 1. 21 => 변신시 무기 교체 못하게 서버에서 막음.]
	if ( true == pclchar->clTransFormInfo.bTransFormMode )
	{
		return;	
	}

	// 생산 활동 중에는 무기 변경 불가
	switch( pclchar->GetCurrentOrder() )
	{
		case ORDER_FISH:
		case ORDER_FARM:
		case ORDER_MINE:
			return;
	}

	// [진성] 변경전에 차고 있던 무기를 기억한다.
	SI32 siPrevActiveWeapon = pclchar->pclCI->clCharItem.clItem[pclchar->ActiveWeapon].siUnique;

	// Active 무기를 Toggle한다.
	if ( pclchar->ActiveWeapon == PERSONITEM_WEAPON1 )
	{
		pclchar->ActiveWeapon = PERSONITEM_WEAPON2;
	}
	else
	{
		pclchar->ActiveWeapon = PERSONITEM_WEAPON1;
	}

	// 마법을 기억해 두고 기억해둔 마법을 꺼내온다.
	SI32					siTempMagic			= pclchar->GetCurrentMagic();
	stMagicAttributeData*	pMagicAttributeData	= pclMagicManager->GetMagicAttribute( pclchar->siOldWeaponMagic );

	pclchar->SetCurrentMagic( pclchar->siOldWeaponMagic, pMagicAttributeData->si08MagicCastType );
	pclchar->siOldWeaponMagic = siTempMagic;

	// 무기 타입이 달라지면 기존에 있던 버프 마법 효과는 없어진다
	SI32 siWeaponType1 = pclchar->pclCI->clCharItem.clItem[ PERSONITEM_WEAPON1 ].GetType( pclItemManager );
	SI32 siWeaponType2 = pclchar->pclCI->clCharItem.clItem[ PERSONITEM_WEAPON2 ].GetType( pclItemManager );
	if ( siWeaponType1 != siWeaponType2 )
	{
		pclchar->m_clMagicBufMgr.Init();
	}

	// 파라미터 조정을 강제로 설정한다.
	pclchar->ParametaBoxAction();

	// 당사자에게는 ActiveWeapon이 바뀌었다는것을 알려준다.
	cltGameMsgResponse_ExchageWeapon clinfo(pclchar->ActiveWeapon);
	cltMsg clMsg(GAMEMSG_RESPONSE_EXCHANGEWEAPON, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);


	//----------------------------------------------
	// 이웃들에게도 알린다. 
	//----------------------------------------------
	pclchar->SendNeighbourMyShapeStatus();
	pclchar->CheckCurrentMagic( siPrevActiveWeapon );
}


// clothes 1,2 Activate변경 
void cltServer::DoMsg_GAMEMSG_REQUEST_EXCHANGECLOTHES(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->CR[id]->ActiveClothes == PERSONITEM_ARMOUR)
	{
		pclCM->CR[id]->ActiveClothes = PERSONITEM_DRESS;
		pclCM->CR[id]->ActiveHat     = PERSONITEM_HAT;

	}
	else
	{
		pclCM->CR[id]->ActiveClothes = PERSONITEM_ARMOUR;
		pclCM->CR[id]->ActiveHat     = PERSONITEM_HELMET;

	}



	// 당사자에게는 ActiveClothes이 바뀌었다는것을 알려준다.
	cltGameMsgResponse_ExchageClothes clinfo(pclCM->CR[id]->ActiveClothes,pclCM->CR[id]->ActiveHat);
	cltMsg clMsg(GAMEMSG_RESPONSE_EXCHANGECLOTHES, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


	//----------------------------------------------
	// 이웃들에게도 알린다. 
	//----------------------------------------------
	((cltCharServer*)pclCM->CR[id])->SendNeighbourMyShapeStatus();

}

// 부적 부착 요청. 
void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Enchant* pclinfo = (cltGameMsgRequest_Enchant*)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	// 내구도 정보 저장.  - 내구도를 사용하지 않는다.
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->SendDBWasteDurability();
	pclchar->SendDBSummonWasteDurability();

	//----------------------
	// fromitem 확인. 
	//----------------------
	SI32 frompos		= pclinfo->siFromPos;
	SI32 fromitemunique	=  pclinfo->siFromItemUnique; 
	if(frompos < 0 || frompos >= MAX_ITEM_PER_PERSON)return ;
	// 실제로 해당 아이템이 있는지 확인. 
	cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[frompos];
	if(pclitem->siUnique != fromitemunique)return ;


	//----------------------
	// toitem 확인. 
	//----------------------
	SI32 topos	= pclinfo->siToPos;
	if(topos < 0 || topos >= MAX_ITEM_PER_PERSON)return ;
	pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[topos];

	if(pclitem->IsSame(&pclinfo->clToItem, false) == FALSE)
	{
		return ;
	}

	/*	if ( pclitem->siUnique == 9400 || pclitem->siUnique == 9410 || pclitem->siUnique == 13330 ||
	pclitem->siUnique == 13340 || pclitem->siUnique == 13350 || pclitem->siUnique == 13360 )
	{
	if ( pclitem->clItemArmour.uiSkillUp == 1 )
	pclitem->clItemArmour.uiSkillUp = 2 ;
	}*/

	//---------------------------------
	// From아이템이 부적인지 확인한다. 
	//---------------------------------
	GMONEY atb = pclItemManager->GetItemAtb(fromitemunique);
	if((atb & ITEMATB_SCROLL) ==0) return ;


	//-------------------------------
	// ToItem 이 Obj인지 확인한다. 
	//-------------------------------
	GMONEY toitematb = pclItemManager->GetItemAtb(pclinfo->clToItem.siUnique);
	if((toitematb & ITEMATB_OBJECT) ==0) return ;


	//-------------------------------
	// 해당 부적이 ToItem에 붙을수 있는지 검사한다.
	//-------------------------------
	if(pclItemManager->CanItemAttach(fromitemunique, pclinfo->clToItem.siUnique) == FALSE)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(0, SRVAL_ENCHANTFAILNOTATBMATCH, 0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}


	//------------------------------------------
	// 부적을 사용하여 장비의 성능을 향상시킨다.
	//------------------------------------------
	cltItem clitem;
	clitem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[topos]);

	SI32 enchantrrtn	= 0;
	SI32 para1			= 0;
	SI32 para2			= 0;
	if(pclItemManager->Enchant(&clitem, fromitemunique, &enchantrrtn, &para1 , &para2) == false)
	{
		// 더이상 부착 불가. 
		if(enchantrrtn == ENCHANT_FAIL_REASON_NOMORE)
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(0, SRVAL_ENCHANTFAILTOOMUCH,  para1/10, para2, pclCM->CR[id]->GetCharUnique());
		}
		else if(enchantrrtn == ENCHANT_FAIL_REASON_NOTATBMATCH) 
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(0, SRVAL_ENCHANTFAILNOTATBMATCH,  para1/10, para2, pclCM->CR[id]->GetCharUnique());
		}
		else if(enchantrrtn == ENCHANT_FAIL_REASON_NOMOREKIND) 
		{	
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(0, SRVAL_ENCHANTFAILTOOMUCHKIND,  para1/10, para2, pclCM->CR[id]->GetCharUnique());
		}

		return ;
	}

	//--------------------------------------------------------------
	// 사용한 부적 정보와 향상된 성능의 장비를 DB로 보내어 검증한다. 
	//--------------------------------------------------------------
	sDBRequest_Enchant clMsg(id, personid, frompos, fromitemunique, topos, &clitem);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	// 아이템 사용 통계를 업데이트 한다. 
	pclItemManager->AddUseNum(fromitemunique, 1);

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", fromitemunique, "UseNum", 1);

}

// 인벤에서 물품을 만들도록 하는 함수. 
void cltServer::MakeItemProcess(SI32 id, cltGameMsgRequest_MakeItemInv* pclinfo)
{
	if(pclCM->IsValidID(id) == false)			return;
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}
	// 제조 의뢰자가 설정되어 있는데 유효하지 않다면 실패. 
	if(pclinfo->siMaterialCharUnique)
	{
		if(pclCM->IsAlive(pclinfo->siMaterialCharUnique) == false)
		{
			pclCM->CR[id]->siPersonalMakeItemType = 0;
			// true, false로 돌려준다.
			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
		
		
			return ;
		}
	}

	SI32 returnval		= 0;
	SI32 preturnpara1	= 0;
	if(((cltCharServer *)pclCM->CR[id])->CanMakeItem(pclinfo, &returnval, &preturnpara1) == false)
	{
		if(returnval)
		{
			pclCM->CR[id]->siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();

			if(pclinfo->siMaterialCharUnique > 0  )	// 제조 의뢰자가 있으면 제조 의뢰자 에게 알려준다 .
			{

				if(pclCM->IsAlive(pclinfo->siMaterialCharUnique) == TRUE)
				{
					if ( pclinfo->siMaterialCharUnique != id ) 
					{
						cltCharServer* pclcharserver = pclCM->GetCharServer(id)	;
						if(pclcharserver	!=NULL )
						{
							SendServerResponseMsg(0, returnval, preturnpara1, 0, pclinfo->siMaterialCharUnique);								
							SendServerResponseMsg(0, returnval, preturnpara1, 0, id);				
						}					
					}
					else
					{
						SendServerResponseMsg(0, returnval, preturnpara1, 0, id);
					}
				}
				else
				{
					SendServerResponseMsg(0, returnval, preturnpara1, 0, id);
				}
			}
			else
			{
				SendServerResponseMsg(0, returnval, preturnpara1, 0, id);
			}

		}
		return;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->siItemUnique);
	if(ref <= 0)
	{

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();

		return ;
	}

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 만들지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();		
	
		return ;
	}

	if(pclinfo->siItemUnique <= 0)	
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0; 

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();		

		return ;
	}
	if(pclinfo->siItemNum <= 0)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();		

		return ;
	}

	cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclItemManager, pclinfo->siItemUnique);
	if(pclprice == NULL)	
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();		

		return ;
	}


	// 시간을 계산한다.
	SI32 productclock = 0;

	// NPC제조라면 무조건 60초
	if( pclinfo->bNPCMake )
	{
		productclock = 60000;
	}
	else
	{
		SI32 skillunique	= pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
		SI32 siGetType		= pclItemManager->pclItemInfo[ref]->siType;

		if ( pclClient->IsCountrySwitch(Switch_MakeFoodHealEXP) && ((ITEMTYPE_FOOD == siGetType) || ((ITEMTYPE_HEAL == siGetType))) )
		{
			productclock = CalcTime_MakeItem_FoodHeal( pclchar, skillunique, pclinfo->siItemUnique, pclinfo->siItemNum );
		}
		else if ( pclClient->IsCountrySwitch(Switch_MakeItemEXP) )
		{
			productclock = CalcTime_MakeItem_New( pclchar, skillunique, pclinfo->siItemUnique, pclinfo->siItemNum );
		}
		else
		{
			productclock = CalcTime_MakeItem( pclchar, skillunique, pclinfo->siItemUnique, pclinfo->siItemNum, pclprice->siPrice );
		}
	}

	//-----------------------------------------
	// 수호정령 특수능력에 의한 제조시간 단축
	//-----------------------------------------
	if ( (false == pclinfo->bNPCMake) && (IsCountrySwitch(Switch_SoulGuard)) )
	{
		SI32 siMoreThanValue	= max( 0, ((productclock/1000)-1) );
		SI32 siSetClock			= pclchar->SoulGuardAbilityActivity( SOULGUARD_ABILITY_MAKETIME_SHORT, 0, siMoreThanValue );
		if ( 0 < siSetClock )
		{
			// 1초 기준으로 값이 나오므로 1000을 곱해준다
			productclock = (siSetClock * 1000);
		}
	}


	// 운영자 명령 추가 - 제조 시간 단축
	if( pclchar->bMakeTimeReduce )
	{
		productclock = 1000;	// 무조건 1초
	}


/*	else if(pclItemManager->GetItemAtb(pclinfo->siItemUnique) & ITEMATB_OBJECT)
	{
		//  이전공식
		//	productclock = min(60000, max(10000,  pclprice->siPrice / 10) );


		SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

		//아이템 제작 필요 레벨에 따른 등급을 구한다.
		SI32 skillRankLimit = ( skillLevelLimit/6 ) + 1;
		if( skillRankLimit > 16 )	skillRankLimit = 15;

		//아이템 필요레벨 등급에 따른 제작시간을 구한다.
		if( skillRankLimit <= 1 )		productclock = 10000;
		else if (skillRankLimit == 2)	productclock = 15000;
		else if (skillRankLimit == 3)	productclock = 20000;
		else if (skillRankLimit == 4)	productclock = 30000;
		else if (skillRankLimit == 5)	productclock = 45000;
		else if (skillRankLimit == 6)	productclock = 55000;
		else if (skillRankLimit > 6)	productclock = 60000;

		SI32 skillunique = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

		// 해당 기술의 현재 수준을 구한다. 
		SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclchar->pclCI->clCharItem.clItem);
		// 스킬 등급을 구한다
		SI32 skillRank = ( skilllevel/6 ) + 1;
		skillRank = max( 15, skillRank );

		// 등급에 따른 제작시간 단축
		if ( skillRank  <= 2 )	{}
		else productclock -= ( skillRank - 2) * 1000;

		productclock = max( 10000, productclock );
	}
	else
	{
		GMONEY data = max(100,  pclprice->siPrice) * pclinfo->siItemNum * 5 / 10;

		productclock = (SI32)(max(10000, min(300000, data) ));
	}*/

	//cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id] ;

	//------------------------------------------
	// 생산에 필요한 도구를 정리한다. 
	//------------------------------------------

	// NPC제작의 경우 도구 필요없음!

	if ( pclinfo->bNPCMake == false )
	{
		DBToolInfo clToolInfo[MAX_TOOL_NUMBER] ;

		SI32 toolindex = 0;

		for(SI32 i = 0;i < MAX_TOOL_NUMBER;i++)
		{
			clToolInfo[i].Init() ;

			SI32 toolunique = pclItemManager->pclItemInfo[ref]->clProductionInfo.clTool[i].siUnique;
			if(toolunique <= 0)continue;

			bool alreadydealswitch = false;
			bool bSealingupFail = false;

			//KHY - 1211 - 과금상품 - 만능 도구
			if(returnval == SRVAL_RETURN_USE_ALLPURPOSETOOL)
				alreadydealswitch = true;

			// 인벤에서 해당 재료를 찾는다. 
			for(SI32 j = 0;j < MAX_INVITEM_PER_PERSON;j++)
			{
				// 이미 필요한 도구를 찾았다면 , 
				if(alreadydealswitch)break;

				if(pclchar->pclCI->clCharItem.clItem[PERSONITEM_INV0 + j].siUnique == toolunique)
				{
					// 밀봉상태면 못 쓴다.
					if ( pclchar->pclCI->clCharItem.clItem[PERSONITEM_INV0 + j].IsSealingUp(pclItemManager) == true )
					{
						bSealingupFail = true;
						continue;					
					}

					bSealingupFail = false;

					clToolInfo[i].siPos	= PERSONITEM_INV0 + j;
					clToolInfo[i].clItem.Set(&pclchar->pclCI->clCharItem.clItem[clToolInfo[i].siPos]);

					/* 기간제로 변경
					// 내구도를 소모시킨다.(필요한 내구도는 생산되는 물건의 시세에 따라 달라진다.) 
					SI32 requiretooldur = min(100, max(10, (pclprice->siPrice * pclinfo->siItemNum) / 100000));

					if(clToolInfo[i].clItem.GetDurability() > requiretooldur)
					{
					clToolInfo[i].clItem.DecreaseDurability(pclItemManager, requiretooldur);

					// 내구도가 모두 떨어진 경우라도 유니크는 살려야 한다. 그래야 DB에서 
					clToolInfo[i].clItem.siUnique	= pclchar->pclCI->clCharItem.clItem[clToolInfo[i].siPos].siUnique;
					}
					else
					{
					SendServerResponseMsg(0, SRVAL_NOTENOUGHTOOLDURABILITY, toolunique, requiretooldur, id);
					return ;
					}
					*/

					toolindex++;

					alreadydealswitch = true;
				}
			}
			if(bSealingupFail)
			{
				SendServerResponseMsg(0, SRVAL_NOTENOUGHTOOLDURABILITY, toolunique, 0, id);

				// true, false로 돌려준다.

				pclCM->CR[id]->siPersonalMakeItemType = 0;

				cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
				cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
				((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				//----------------------------------------------
				// 이웃들에게도 알린다. 
				//----------------------------------------------
				((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();		


				return ;
			}
		}

		//내구도  - 관련 삭제.
		/*for ( i = 0 ; i < MAX_TOOL_NUMBER ; i ++ )
		{
			if ( clToolInfo[i].clItem.siUnique )
			{
				if( clToolInfo[i].siPos >= 0 && clToolInfo[i].siPos < MAX_ITEM_PER_PERSON )
				{
					pclchar->pclCI->clCharItem.clItem[clToolInfo[i].siPos].Set(&clToolInfo[i].clItem) ;
					pclchar->SetWasteDurUpdateSwitch(clToolInfo[i].siPos, true);

					cltGameMsgResponse_PersonItemInfo clinfo(clToolInfo[i].siPos, PERSONITEMINFO_OPTION_TOOLWASTE, 0, 0, &clToolInfo[i].clItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
		*/
	}

	pclchar->SendDBWasteDurability() ;
	pclchar->SendDBSummonWasteDurability();

	// 캐릭터에게 생산 메시지와 시간을 저장한다.
	pclchar->SetReserverMakeItem((DWORD)productclock, pclinfo);

	//-------------------------------------------
	// 클라이언트로 지연 시간을 통보한다. 
	//-------------------------------------------
	cltGameMsgResponse_MakeItemDelay clinfo((DWORD)productclock);
	cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMDELAY, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	// 미니게임을 할 것인지 말 것인지 검사한다.
	if(pclItemManager->pclItemInfo[ref]->IsSkill_Maked() == true && pclinfo->bNPCMake == false )
	{
		cltGameMsgResponse_IsSkillItem clinfo(1);
		cltMsg clMsg(GAMEMSG_RESPONSE_ISSKILLITEM, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);

		pclchar->bMakeItemNoMinigame = false ;
		pclchar->bMakeItemMinigameSwitch = false ;
	}
	else
	{
		pclchar->bMakeItemNoMinigame = true ;
		pclchar->bMakeItemMinigameSwitch = false ;
	}
	cltServer* pclserver			= (cltServer*)pclClient;
	
	pclchar->siMonsterCorp = 0 ;
	pclchar->dwStartMiniGameTime = CurrentClock;

	pclchar->bMakeItemSwitch = false ;
}

SI32 cltServer::CalcExp_MakeItem( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum, SI64 siPrice )
{
	SI32 exp = 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);

	// 해당 기술의 현재 수준을 구한다. 
	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

	// 제조 레벨에 해당하는 등급을 구한다.
	SI32 skillrank = (skilllevel/6) + 1;
	if( skillrank > 16 )	skillrank = 15;

	SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

	// 제조 하려는 아이템의 등급을 구한다.
	SI32 skillRankLimit = ( skillLevelLimit/6 ) + 1;
	if( skillRankLimit > 16 )	skillRankLimit = 15;

	// 아이템과 유저 등급차에 의한 패널티 계산 ( 전투, 제조 경험치 적용 )
	SI32 siskillgap = skillrank - skillRankLimit;
	SI32 siPenalty = 120;
	if ( siskillgap == 1)		siPenalty = 100;
	else if (siskillgap == 2 )	siPenalty = 80;
	else if (siskillgap == 3 )	siPenalty = 20;
	else if (siskillgap == 4 )	siPenalty = 10;
	else if (siskillgap == 5 )	siPenalty = 5;
	else if (siskillgap >= 5 )	siPenalty = 2;

	// 해당 기술의 책으로 인한 스킬레벨
	SI32 skilllevlbybook = pclchar->pclCI->clCharSkill.GetSkillLevel(skillunique);
	// 책으로 인한 기술습득에 의한 패널티
	SI32 siPenalty2 = (100 - ( 30 - skilllevlbybook )*3 );

	//--------------------------------------
	// 경험치를 높여준다. 
	//--------------------------------------
	if ( (pclItemManager->GetItemAtb(siItemUnique) & ITEMATB_OBJECT) )
	{
		// 기본 경험치를 구한다 - 제조 등급에 따라 틀려진다.
		switch( skillrank )
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:			exp = 450;	break;
		case 6:			exp = 550;	break;
		case 7:			exp = 600;	break;
		case 8:			exp = 600;	break;
		case 9:			exp = 600;	break;
		case 10:		exp = 600;	break;
		case 11:		exp = 600;	break;
		case 12:		exp = 600;	break;
		case 13:		exp = 650;	break;
		case 14:		exp = 700;	break;
		case 15:		exp = 700;	break;
		default:		exp = 0;
		}

		// 패널티 계산
		exp = ( exp * siPenalty * siPenalty2 ) / 10000;

	}
	else
	{
		// 기존 경험치 공식 적용 (오브젝트가 아닐경우)
		exp	= pclItemManager->GetMakeExpFromItemPrice(siPrice * siNum, pclchar->pclCI->clIP.GetLevel()); 
	}

	return exp;
}

SI32 cltServer::CalcSkillExp_MakeItem( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum, SI64 siPrice )
{
	SI32 skillexp = 0;

	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);

	// 제조 레벨에 해당하는 등급을 구한다.
	SI32 skillrank = (skilllevel/6) + 1;
	if( skillrank > 16 )	skillrank = 15;

	SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

	// 제조 하려는 아이템의 등급을 구한다.
	SI32 skillRankLimit = ( skillLevelLimit/6 ) + 1;
	if( skillRankLimit > 16 )	skillRankLimit = 15;

	// 아이템과 유저 등급차에 의한 패널티 계산 ( 전투, 제조 경험치 적용 )
	SI32 siskillgap = skillrank - skillRankLimit;
	SI32 siPenalty = 120;
	if ( siskillgap == 1)		siPenalty = 100;
	else if (siskillgap == 2 )	siPenalty = 80;
	else if (siskillgap == 3 )	siPenalty = 20;
	else if (siskillgap == 4 )	siPenalty = 10;
	else if (siskillgap == 5 )	siPenalty = 5;
	else if (siskillgap >= 5 )	siPenalty = 2;

	// 해당 기술의 책으로 인한 스킬레벨
	SI32 skilllevlbybook = pclchar->pclCI->clCharSkill.GetSkillLevel(skillunique);
	// 책으로 인한 기술습득에 의한 패널티
	SI32 siPenalty2 = (100 - ( 30 - skilllevlbybook )*3 );

	// 등급에 따른 기본 경험치
	if ((pclItemManager->GetItemAtb(siItemUnique) & ITEMATB_OBJECT) )
	{
		skillexp = 230;
		if ( skillrank > 5 )
		{
			skillexp = 40;
			for ( SI16 i = 1 ; i < skillrank ; i++)
				skillexp += ( i + 2 )* 10;
		}

		// 패널티 계산
		skillexp = ( skillexp * siPenalty * siPenalty2 ) / 10000;
	}
	else
	{
		// 기존 기술 경험치 공식, 오브젝트가 아닐경우, 일본판일경우
		skillexp = pclItemManager->GetMakeSkillExpFromItemPrice(siPrice * siNum, skilllevel); 
	}

	return skillexp;
}

SI32 cltServer::CalcExp_MakeItem_New( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum )
{
	static REAL32 fStaticBaseExp = 35.0f;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);

	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

	SI32 exp = 0;
	SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;
	REAL32 fPenalty = 0.0f;

	if ( (pclItemManager->GetItemAtb(siItemUnique) & ITEMATB_OBJECT) )
	{
		exp = fStaticBaseExp * (2.0f + ((skillLevelLimit*skillLevelLimit) * 5.0f / 1000.0f));

		fPenalty = 100.0f - (((skilllevel*10.0f - skillLevelLimit*10.0f)*(skilllevel*10.0f - skillLevelLimit*10.0f)) / 800.0f) ;
		if(fPenalty <= 5.0f)		fPenalty = 5.0f;

		exp = (FLOAT)exp * (fPenalty / 100.0f);
	}
	else
	{
		exp = (fStaticBaseExp * (2.0f + ((skillLevelLimit*skillLevelLimit) * 5.0f / 1000.0f))) * siNum;
		exp = (FLOAT)exp / 500.0f;
	}

	return exp;
}

SI32 cltServer::CalcSkillExp_MakeItem_New( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum )
{
	SI32 skillexp = 0;

	static REAL32 fStaticBaseExp = 35.0f;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);

	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

	SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;
	REAL32 fPenalty = 0.0f;

	SI32 siItemType = pclItemManager->GetItemType(siItemUnique);
	// 결정체는 OBJECT와 같은 공식을 따른다.
	if( siItemType == ITEMTYPE_CRYSTAL )
	{
		skillexp = fStaticBaseExp * (1.0f + ((skillLevelLimit*skillLevelLimit) * 5.0f / 1000.0f));

		fPenalty = 100.0f - (((skilllevel*10.0f - skillLevelLimit*10.0f)*(skilllevel*10.0f - skillLevelLimit*10.0f)) / 800.0f) ;
		if(fPenalty <= 5.0f)		fPenalty = 5.0f;

		skillexp = skillexp * (fPenalty / 100.0f);

		skillexp = skillexp * siNum;
		skillexp = skillexp * 0.8;		// 보정치를 계산한다.
	}
	else
	{
		// 결정체가 아닌 아이템은 OBJECT의 여부에 따라 경험치를 계산한다.
		if ( (pclItemManager->GetItemAtb(siItemUnique) & ITEMATB_OBJECT) )
		{
			skillexp = fStaticBaseExp * (1.0f + ((skillLevelLimit*skillLevelLimit) * 5.0f / 1000.0f));

			fPenalty = 100.0f - (((skilllevel*10.0f - skillLevelLimit*10.0f)*(skilllevel*10.0f - skillLevelLimit*10.0f)) / 800.0f) ;
			if(fPenalty <= 5.0f)		fPenalty = 5.0f;

			skillexp = skillexp * (fPenalty / 100.0f);
		}
		else
		{
			skillexp = (fStaticBaseExp * (1.0f + ((skillLevelLimit*skillLevelLimit) * 5.0f / 1000.0f))) * siNum;
			skillexp = (FLOAT)skillexp / 500.0f;
		}
	}

	return skillexp;
}

SI32 cltServer::CalcExp_MakeItem_FoodHeal( cltCharServer* pclchar, SI32 siSkillUnique, SI32 siItemUnique, SI32 siNum )
{
	const static REAL32 fBaseExp = 20.0f;

	SI32	siReturnSkillExp	= 0;

	SI32	ref					= pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= ref )
	{
		return 0;
	}

	SI32	siCharLevel			= pclchar->pclCI->clIP.GetLevel();
	SI32	siSkillExpLevel		= pclchar->pclCI->clCharSkill.GetSkillExpLevel( siSkillUnique );
	SI32	siSkillBookLevel	= pclchar->pclCI->clCharSkill.GetSkillLevel( siSkillUnique );
	SI32	siSkillLevelLimit	= pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

	SI32	siPenalty			= (SI32)(((REAL32)siSkillLevelLimit / (REAL32)max(1, (siSkillExpLevel+siSkillBookLevel))) * 100.0f);


	siReturnSkillExp	= (SI32)(fBaseExp * (2.0f + ((siSkillLevelLimit*siSkillLevelLimit) * 5.0f / 1000.0f))) * siNum;
	siReturnSkillExp	= (SI32)(((REAL32)siReturnSkillExp * (REAL32)siPenalty) / 100.0f);

	siReturnSkillExp	= min( (siCharLevel*20), siReturnSkillExp );


	return siReturnSkillExp;


}

SI32 cltServer::CalcSkillExp_MakeItem_FoodHeal( cltCharServer* pclchar, SI32 siSkillUnique, SI32 siItemUnique, SI32 siNum )
{
	const static REAL32 fBaseExp = 20.0f;

	SI32	siReturnSkillExp	= 0;

	SI32	ref					= pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= ref )
	{
		return 0;
	}

	SI32	siCharLevel			= pclchar->pclCI->clIP.GetLevel();
	SI32	siSkillExpLevel		= pclchar->pclCI->clCharSkill.GetSkillExpLevel( siSkillUnique );
	SI32	siSkillBookLevel	= pclchar->pclCI->clCharSkill.GetSkillLevel( siSkillUnique );
	SI32	siSkillLevelLimit	= pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;
	
	SI32	siPenalty			= (SI32)(((REAL32)siSkillLevelLimit / (REAL32)max(1, (siSkillExpLevel+siSkillBookLevel))) * 100.0f);
	

	siReturnSkillExp	= (SI32)(fBaseExp * (1.0f + ((siSkillLevelLimit*siSkillLevelLimit) * 5.0f / 1000.0f))) * siNum;
	siReturnSkillExp	= (SI32)(((REAL32)siReturnSkillExp * (REAL32)siPenalty) / 100.0f);

	siReturnSkillExp	= min( (siCharLevel*20), siReturnSkillExp );


	return siReturnSkillExp;

}

SI32 cltServer::CalcTime_MakeItem( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum, SI64 siPrice )
{
	SI32 productclock = 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);

	if(pclItemManager->GetItemAtb(siItemUnique) & ITEMATB_OBJECT)
	{
		SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

		//아이템 제작 필요 레벨에 따른 등급을 구한다.
		SI32 skillRankLimit = ( skillLevelLimit/6 ) + 1;
		if( skillRankLimit > 16 )	skillRankLimit = 15;

		//아이템 필요레벨 등급에 따른 제작시간을 구한다.
		if( skillRankLimit <= 1 )		productclock = 10000;
		else if (skillRankLimit == 2)	productclock = 15000;
		else if (skillRankLimit == 3)	productclock = 20000;
		else if (skillRankLimit == 4)	productclock = 30000;
		else if (skillRankLimit == 5)	productclock = 45000;
		else if (skillRankLimit == 6)	productclock = 55000;
		else if (skillRankLimit > 6)	productclock = 60000;

		SI32 skillunique = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;

		// 해당 기술의 현재 수준을 구한다. 
		SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);
		// 스킬 등급을 구한다
		SI32 skillRank = ( skilllevel/6 ) + 1;
		skillRank = max( 15, skillRank );

		// 등급에 따른 제작시간 단축
		if ( skillRank  <= 2 )	{}
		else productclock -= ( skillRank - 2) * 1000;

		productclock = max( 10000, productclock );
	}
	else
	{
		GMONEY data = max(100,  siPrice) * siNum * 5 / 10;

		productclock = (SI32)(max(10000, min(300000, data) ));
	}

	return productclock;
}

SI32 cltServer::CalcTime_MakeItem_New( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum )
{
	SI32 productclock = 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);
	SI32 skillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;
	// 해당 기술의 현재 수준을 구한다. 
	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

	if(pclItemManager->GetItemAtb(siItemUnique) & ITEMATB_OBJECT)
	{
		productclock = (skillLevelLimit * 5.0f) - (skilllevel * 1.0f);
		productclock = max( 10, productclock );
	}
	else
	{
		productclock = ((skillLevelLimit * 100.0f) * ( siNum / 50.0f)) / 1000.0f;

		productclock = max( 10, productclock );
	}

	return productclock * 1000;
}

// [영훈][스위치] 음식,약 전용 제조시간 계산 함수
SI32 cltServer::CalcTime_MakeItem_FoodHeal( cltCharServer* pclchar, SI32 siSkillUnique, SI32 siItemUnique, SI32 siNum )
{
	SI32 siProductClock = 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= ref )
	{
		return 0;
	}

	SI32 siSkillLevelLimit = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel;

	siProductClock = ((siSkillLevelLimit * 100.0f) * ( siNum / 50.0f)) / 1000.0f;
	siProductClock = max( 10, siProductClock );
	siProductClock = min( 300, siProductClock );

	return (siProductClock * 1000); // millisecond로 변환
}

// 인벤에서 물품을 만들도록 하는 함수. 
void cltServer::EnchantItemProcess(SI32 id, cltGameMsgRequest_MakeItemInv* pclinfo)
{
	if(bEnchantSwitch == false) return;
	// 제조 의뢰자가 설정되어 있는데 유효하지 않다면 실패. 
	if(pclinfo->siMaterialCharUnique)
	{
		if(pclCM->IsAlive(pclinfo->siMaterialCharUnique) == false)
		{
			// true, false로 돌려준다.

			pclCM->CR[id]->siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();

			return ;
		}
	}

	SI32 returnval		= 0;
	SI32 preturnpara1	= 0;
	if(((cltCharServer *)pclCM->CR[id])->CanEnchantItem(pclinfo, &returnval, &preturnpara1) == false)
	{
		if(returnval)
		{
			// true, false로 돌려준다.

			pclCM->CR[id]->siPersonalMakeItemType = 0;

			cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
			((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			//----------------------------------------------
			// 이웃들에게도 알린다. 
			//----------------------------------------------
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();

			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(0, returnval, preturnpara1, 0, id);
		}
		return;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->siItemUnique);
	if(ref <= 0)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
		return ;
	}

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 만들지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
		return ;
	}

	if(pclinfo->siItemUnique <= 0)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
		return ;
	}

	if(pclinfo->siItemNum <= 0)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
		return ;
	}

	cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclItemManager, pclinfo->siItemUnique);
	if(pclprice == NULL)
	{
		// true, false로 돌려준다.

		pclCM->CR[id]->siPersonalMakeItemType = 0;

		cltGameMsgResponse_MakeItemSwitch clinfo( 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
		((cltServer *)pclClient)->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//----------------------------------------------
		// 이웃들에게도 알린다. 
		//----------------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
		return ;
	}


	// 시간을 계산한다.
	DWORD productclock;

	if(pclItemManager->GetItemAtb(pclinfo->siItemUnique) & ITEMATB_OBJECT)
	{
		//productclock = min(60000, max(10000,  pclprice->siPrice / 10) );
		productclock =10000;
	}
	else
	{
		GMONEY data = max(100,  pclprice->siPrice) * pclinfo->siItemNum * 5 / 10;

		//productclock = max(10000, min(300000, data) );
		productclock =10000;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id] ;

	//------------------------------------------
	// 생산에 필요한 도구를 정리한다. 
	//------------------------------------------

	/*

	DBToolInfo clToolInfo[MAX_TOOL_NUMBER] ;

	SI32 toolindex = 0;

	for(SI32 i = 0;i < MAX_TOOL_NUMBER;i++)
	{
	clToolInfo[i].Init() ;

	SI32 toolunique = pclItemManager->pclItemInfo[ref]->clProductionInfo.clTool[i].siUnique;
	if(toolunique <= 0)continue;

	bool alreadydealswitch = false;


	//여기여기
	// 인벤에서 해당 재료를 찾는다. 
	for(SI32 j = 0;j < MAX_INVITEM_PER_PERSON;j++)
	{
	// 이미 필요한 도구를 찾았다면 , 
	if(alreadydealswitch)continue;

	if(pclchar->pclCI->clCharItem.clItem[PERSONITEM_INV0 + j].siUnique == toolunique)
	{
	// 밀봉상태면 못 쓴다.
	if ( pclchar->pclCI->clCharItem.clItem[PERSONITEM_INV0 + j].IsSealingUp(pclItemManager) == true )
	{
	SendServerResponseMsg(0, SRVAL_NOTENOUGHTOOLDURABILITY, toolunique, 0, id);
	return ;
	}
	// 고물품이면 못 쓴다.
	if ( pclItemManager->CheckNotGoodItem(&pclchar->pclCI->clCharItem.clItem[PERSONITEM_INV0 + j]) == false )
	{
	SendServerResponseMsg(0, SRVAL_NOTGOODTOOL, toolunique, 0, id);
	return ;
	}

	clToolInfo[i].siPos	= PERSONITEM_INV0 + j;
	clToolInfo[i].clItem.Set(&pclchar->pclCI->clCharItem.clItem[clToolInfo[i].siPos]);


	toolindex++;

	alreadydealswitch = true;
	}
	}
	}

	for ( i = 0 ; i < MAX_TOOL_NUMBER ; i ++ )
	{
	if ( clToolInfo[i].clItem.siUnique )
	{
	pclchar->pclCI->clCharItem.clItem[clToolInfo[i].siPos].Set(&clToolInfo[i].clItem) ;
	pclchar->SetWasteDurUpdateSwitch(clToolInfo[i].siPos, true);

	cltGameMsgResponse_PersonItemInfo clinfo(clToolInfo[i].siPos, PERSONITEMINFO_OPTION_TOOLWASTE, 0, 0, &clToolInfo[i].clItem, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (char*)&clinfo);
	pclchar->SendNetMsg((char*)&clMsg);
	}
	}

	*/
	pclchar->SendDBWasteDurability();
	pclchar->SendDBSummonWasteDurability();


	// 캐릭터에게 생산 메시지와 시간을 저장한다.
	pclchar->SetReserverMakeItem(productclock, pclinfo);

	//-------------------------------------------
	// 클라이언트로 지연 시간을 통보한다. 
	//-------------------------------------------
	cltGameMsgResponse_MakeItemDelay clinfo(productclock);
	cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMDELAY, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);


	pclchar->bEnchantItemSwitch = true ;  //인챈트는 미니게임을 하지 않는다 .


	pclchar->siMonsterCorp = 0 ;
	pclchar->dwStartMiniGameTime = 0;
	pclchar->bFinishEnchantItemSwitch = false ;
}

// 인벤에서 아이템을 만들 것을 요청. 
void cltServer::DoMsg_GAMEMSG_REQUEST_MAKEITEMINV(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->IsValidID(id) == false)
		return;

	cltGameMsgRequest_MakeItemInv* pclinfo = (cltGameMsgRequest_MakeItemInv*)pclMsg->cData;

	if( false == pclEventTimeManager->InEventTime(TEXT("Chuseok_Monster_Appear"), &sTime) )
	{
		// [진성] 추석 이벤트. 우주토끼 모자=> 2008-8-12
		if(	pclinfo->siItemUnique == ITEMUNIQUE(23496) ||
			pclinfo->siItemUnique == ITEMUNIQUE(13047) || 
			pclinfo->siItemUnique == ITEMUNIQUE(13048) ||
			pclinfo->siItemUnique == ITEMUNIQUE(23114)	)	
		{
			SendServerResponseMsg( 0, SRVAL_EVENTDAY_OVER_NOT_MAKE_ITEM, ITEMUNIQUE(pclinfo->siItemUnique), 0, pclCM->CR[id]->GetID() );
			return;
		}
	}

	if( false == bEventToggleSwitch[EVENT_TOGGLE_SWITCH_FISHINGEVENT] )
	{
		// [진성] 낚시 이벤트 기간 지나면 화금낚시대를 만들지 못하게 => 2008-9-5
		if(pclinfo->siItemUnique == ITEMUNIQUE(17160))	
		{
			SendServerResponseMsg( 0, SRVAL_EVENTDAY_OVER_NOT_MAKE_ITEM, ITEMUNIQUE(17160), 0, pclCM->CR[id]->GetID() );
			return;
		}
	}

	pclCM->CR[id]->siPersonalMakeItemType = pclinfo->siType;

	cltGameMsgResponse_MakeItemSwitch clinfo( pclinfo->siType );
	cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//----------------------------------------------
	// 이웃들에게도 알린다. 
	//----------------------------------------------
	((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();

	MakeItemProcess(id, pclinfo);
}
void cltServer::DoMsg_GAMEMSG_REQUEST_ITEMCHANGE(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->CR[id]->siPersonalMakeItemType > 0) return; //제조시 아이템 이동 안된다.
	if( pclCM->CR[id]->bPersonalEnchantItemSwitch == true) return; 
	if(pclCM->CR[id]->pclCI->GetPersonID() <= 0 ) return; // 어찌되었든 잘못된 거다.

	// 아이템 이동 정보를 확보한다. 
	cltMoveItemInfo* pclmoveiteminfo = (cltMoveItemInfo*)pclMsg->cData;

	cltItem* pclFrom = NULL;
	cltItem  clTo;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();


	SI32 slot = -1;

	// 부적절한 데이터를 확인한다. 
	if(pclmoveiteminfo->uiTo < 0	|| pclmoveiteminfo->uiTo >= MAX_ITEM_PER_PERSON)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMCHANGE - PersonID = %d, IP =  %s , uiTo = %d"),	
																	pclCM->CR[id]->pclCI->GetPersonID(),
																	pclCM->CR[id]->GetSession()->GetIP(),
																	pclmoveiteminfo->uiTo
																	);
		return ;
	}	
	if(pclmoveiteminfo->uiFrom < 0	|| pclmoveiteminfo->uiFrom >= MAX_ITEM_PER_PERSON)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMCHANGE - PersonID = %d, IP =  %s , uiFrom = %d"),	
																	pclCM->CR[id]->pclCI->GetPersonID(),
																	pclCM->CR[id]->GetSession()->GetIP(),
																	pclmoveiteminfo->uiFrom
																	);
		return ;
	}	

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 이동 안된다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclmoveiteminfo->uiTo	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclmoveiteminfo->uiFrom	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			// 귀속전용 개인상점
			if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclmoveiteminfo->uiFrom	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			// 귀속전용 개인상점
			if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclmoveiteminfo->uiTo	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}
	}

	// 내구도 정보 저장.  - 내구도 관련 삭제.
	if(pclmoveiteminfo->uiTo < PERSONITEM_INV0 || pclmoveiteminfo->uiFrom < PERSONITEM_INV0)
	{
		pclchar->SendDBWasteDurability();
		pclchar->SendDBSummonWasteDurability();
	}


	//------------------------------------------------
	// 말을 갖고 있지 않으면 말 인벤에 넣을 수는 없다.
	//------------------------------------------------
	if(pclCM->CR[id]->HaveHorse() == false)
	{
		if(pclmoveiteminfo->uiTo >= PERSONITEM_HORSE0 && pclmoveiteminfo->uiTo < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY)return ;
	}

	//--------------------------------------------
	//From의 아이템을 구한다.
	//--------------------------------------------
	pclFrom		= &pclCM->CR[id]->pclCI->clCharItem.clItem[pclmoveiteminfo->uiFrom];			

	// 아이템이 무효한 것이라면..
	if(pclFrom == NULL || pclFrom->siUnique <=0 ||pclFrom->siItemNum <= 0)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		if(pclFrom == NULL )
		{
			pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMCHANGE - PersonID = %d, IP =  %s , pclFrom = NULL"),	
																		pclCM->CR[id]->pclCI->GetPersonID(),
																		pclCM->CR[id]->GetSession()->GetIP()
																		);
		}
		else
		{
			pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMCHANGE - PersonID = %d, IP =  %s , siUnique = %d,  siItemNum = %d"),	
																		pclCM->CR[id]->pclCI->GetPersonID(),
																		pclCM->CR[id]->GetSession()->GetIP(),
																		pclFrom->siUnique ,
																		pclFrom->siItemNum											
																		);
		}
		return ;
	}

	//------------------------------------------------
	// 변신 중에는 아이템을 장착할 수 없다.
	//------------------------------------------------
	if( pclCM->CR[id]->clTransFormInfo.bTransFormMode &&
		pclmoveiteminfo->uiTo >= PERSONITEM_HELMET && pclmoveiteminfo->uiTo < PERSONITEM_HELMET + MAX_SHAPE_ITEM )
	{
		//KHY - 0129 - 변신중이라도, 화살과 총알은 장착및 해제가능
		switch(pclFrom->siUnique)
		{
			case ITEMUNIQUE(7850) : // 화살
			case ITEMUNIQUE(7855) : // 총알
			case ITEMUNIQUE(13600) : // 무한 화살.
			case ITEMUNIQUE(13601) : // 무한 총알.
				break;
			default:
				// 변신중에는 아이템을 장착할 수 없습니다.
				return;
		}
	}

	//------------------------------------------------
	// 변신 중에는 아이템을 장착을 해제 할 수 없다.
	//------------------------------------------------
	if( pclCM->CR[id]->clTransFormInfo.bTransFormMode &&
		pclmoveiteminfo->uiFrom >= PERSONITEM_HELMET && pclmoveiteminfo->uiFrom < PERSONITEM_HELMET + MAX_SHAPE_ITEM )
	{
		//KHY - 0129 - 변신중이라도, 화살과 총알은 장착및 해제가능
		switch(pclFrom->siUnique)
		{
			case ITEMUNIQUE(7850) : // 화살
			case ITEMUNIQUE(7855) : // 총알
			case ITEMUNIQUE(13600) : // 무한 화살.
			case ITEMUNIQUE(13601) : // 무한 총알.
				break;
			default:
				// 변신중에는 아이템을 장착을 해제할 수 없습니다.
				return;
		}
	}

	// 트레이드중인 것은 안된다!
	if ( ((cltCharServer*)pclCM->CR[id])->GetNoMoveItemSwitch(pclmoveiteminfo->uiFrom) != 0 )
	{
		return ;
	}

	//----------------------------------------------------
	// 가방을 갖고 있지 않으면 가방 인벤에 넣을 수는 없다
	//----------------------------------------------------
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique == 0)
	{
		if(pclmoveiteminfo->uiTo >= PERSONITEM_BAG0 && pclmoveiteminfo->uiTo < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG)return ;
	}
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique == 0)
	{
		if(pclmoveiteminfo->uiTo >= PERSONITEM_BAG1 && pclmoveiteminfo->uiTo < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG)return ;
	}

	//----------------------------------------------------
	// 장착된 가방을 가방 인벤내로 옮길 수는 없다.
	//----------------------------------------------------
	if ( pclmoveiteminfo->uiFrom == PERSONITEM_BAG )
	{
		if( pclmoveiteminfo->uiTo >= PERSONITEM_BAG0 && pclmoveiteminfo->uiTo < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG )
		{
			return;
		}

		//----------------------------------------------------
		// 가방에 물건을 둔채로 장착을 해제 시킬수는 없다.
		//----------------------------------------------------
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG; ++t )
		{
			if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				return;
			}
		}
	}

	//----------------------------------------------------
	// 장착된 가방2을 가방2 인벤내로 옮길 수는 없다. 
	//----------------------------------------------------
	if ( pclmoveiteminfo->uiFrom == PERSONITEM_BAG_SUB )
	{
		if( pclmoveiteminfo->uiTo >= PERSONITEM_BAG1 && pclmoveiteminfo->uiTo < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG )
		{
			return;
		}

		//----------------------------------------------------
		// 가방2에 물건을 둔채로 장착을 해제 시킬수는 없다.
		//----------------------------------------------------
		for ( SI16 t = PERSONITEM_BAG1; t < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG; ++t )
		{
			if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				return;
			}
		}
	}

	// 적절하지 않은 곳으로는 이동시킬 수 없다. 
	if(pclItemManager->CheckProperItemPos(pclFrom, pclmoveiteminfo->uiTo, pclchar->GetKind()) == FALSE)return ;

	//------------------------------------------
	// 밀봉상태 아이템은 장착불가 
	// 기간이 지난 아이템은 장착불가 - by LEEKH
	//------------------------------------------
	if((pclmoveiteminfo->uiTo >= 0 && pclmoveiteminfo->uiTo<= 14) || pclmoveiteminfo->uiTo == 82 || pclmoveiteminfo->uiTo== 97)	
	{
		if(pclFrom->IsSealingUp(pclItemManager)) return;

		SI32 useableDay;
		BOOL rVal = GetItemUseAbleDay(pclFrom, &useableDay);
		if(rVal == TRUE && useableDay < 0)		return;
	}

	//------------------------------------------
	// 장착 조건을 충족시키는지 확인한다. 
	//------------------------------------------
	SI32 returnval = 0;
	if(pclCM->CR[id]->CheckEquipCondition(pclItemManager, pclmoveiteminfo->uiTo, pclFrom, &returnval) == false)return ;


	// 결정체 크리스탈 내구도 정보 저장.  - 
	if( pclmoveiteminfo->uiFrom < PERSONITEM_INV0)
	{
		pclchar->SendDBCrystalDurability();
		pclchar->SendClientCrystalDurability();
	}

	//-----------------------------------------------
	//DB메니저로 아이템을 이동하라는 요청을 보낸다. 
	//-----------------------------------------------
	cltItem clFrom;
	clFrom.Set(pclFrom);

	//-----------------------------------------------
	// To 에 넣는다. 
	//-----------------------------------------------
	clTo.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclmoveiteminfo->uiTo]);

	//cyj 옮기는 아이템의 총합이 MAX_ITEM_PILE_NUMBER를 넘으면 남는 아이템을 다른 인벤토리에 저장하도록
	if (DistributePileMoveItem(id, clFrom, pclmoveiteminfo->uiFrom, clTo, pclmoveiteminfo->uiTo) == FALSE)
	{
		bool Canpile = false;
		if(( pclFrom->CanPile(pclItemManager))&&( clTo.CanPile(pclItemManager)))
			Canpile = true;
		
		sDBRequest_MoveItem clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclmoveiteminfo->uiFrom, &clFrom, Canpile,
									pclmoveiteminfo->uiTo, &clTo);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANTITEMINV(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->IsValidID(id) == false)
		return;

	cltGameMsgRequest_MakeItemInv* pclinfo = (cltGameMsgRequest_MakeItemInv*)pclMsg->cData;

	pclCM->CR[id]->siPersonalMakeItemType = pclinfo->siType;

	cltGameMsgResponse_MakeItemSwitch clinfo( pclinfo->siType );
	cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//----------------------------------------------
	// 이웃들에게도 알린다. 
	//----------------------------------------------
	((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();


	EnchantItemProcess(id, pclinfo);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANT_USEDAYPLUS(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->CR[id]->siPersonalMakeItemType > 0) return; //제조시 아이템 기간연장 안된다.
	if( pclCM->CR[id]->bPersonalEnchantItemSwitch == true) return; 

	cltGameMsgRequest_UseDayPlus * pclinfo = (cltGameMsgRequest_UseDayPlus*)pclMsg->cData;
	
	// 각종 유효성 체크
	if( pclinfo->siItemUnique <= 0 ) return;
	if( pclinfo->siOrgInx < 0  || pclinfo->siOrgInx >= MAX_ITEM_PER_PERSON) return;
	if( pclinfo->siEleInx < 0  || pclinfo->siEleInx >= MAX_ITEM_PER_PERSON) return;

	if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgInx].siUnique != pclinfo->siItemUnique ) return;
	if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siEleInx].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE ) return;

	// 기간 연장을 할 수 있는 아이템인지 체크한다.
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgInx].IsPlusUseDate(pclItemManager) == false)
		return;

	// 고급품이며 기한이 있는 아이템인지 체크
	//if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgInx].clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5   || pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgInx].uiDateUseDay <= 0 )

	//KHY - 0313 -  영혼의 돌 (기간 연장의 돌) 사용처 늘어남. 
/*	SI32 itemType  = pclItemManager->GetItemType(pclinfo->siItemUnique);

	if((pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgInx].uiDateUseDay <= 0 )
		||(itemType != ITEMTYPE_HELMET)
		||(itemType != ITEMTYPE_ARMOUR)
		||(itemType != ITEMTYPE_SHOES)
		||(itemType != ITEMTYPE_BELT))
	{		
		return;
	}*/

	// 기간 연장 아이템이 있는지 체크
	if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siEleInx].siItemNum <= 0 ) return;


	cltItem clOutput;
	clOutput.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgInx] );
	

	cltItem clMaterial;
	clMaterial.siUnique 	= ENCHANT_USEDAYPLUS_ITEMUNIQUE;
	clMaterial.siItemNum	= 1;
	
	SI32 useableDay = 0;
	BOOL rVal = GetItemUseAbleDay(&clOutput,&useableDay);
	if(rVal == FALSE) return;

	// 기간이 지난 아이템이라면 오늘 새로 생성된 것처럼 데이터를 넣어주고 사용기간을 10일로 넣어준다.
	if( useableDay < 0 )
	{
		clOutput.uiDateYear = sTime.wYear-2000;
		clOutput.uiDateMonth = sTime.wMonth;
		clOutput.uiDateDay = sTime.wDay;

		clOutput.uiDateUseDay = 10;
	}
	else	// 기간이 안지난 아이템이라면 걍 + 10 해준다
	{
		clOutput.uiDateUseDay += 10;
	}	

	((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclinfo->siOrgInx , pclClient->CurrentClock );
	((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclinfo->siEleInx , pclClient->CurrentClock );

	//KHY - 0723 - 기간아이템 파괴 방지.
	if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
	{
		pclClient->sUpViewEffect = 0;
	}

	sDBRequest_EnchantUseDayPlus clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->siOrgInx , &clOutput ,pclinfo->siEleInx , &clMaterial );	
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);


}

void cltServer::DoMsg_GAMEMSG_REQUEST_CHANGEENCHANT(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == false )					return;	// ID가 유효한지 검사
	if ( pclCM->CR[id]->siPersonalMakeItemType > 0)			return; // 제조중이라면 안된다
	if ( pclCM->CR[id]->bPersonalEnchantItemSwitch == true)	return; // 인첸트중이라면 안된다

	cltGameMsgRequest_ChangeEnchant *pclinfo = (cltGameMsgRequest_ChangeEnchant*)pclMsg->cData;
	if ( pclinfo == NULL )									return;

	cltPersonInfo* pclPerson = (cltPersonInfo*)pclCM->CR[id]->pclCI;
	if ( pclPerson == NULL )								return;

	// 유효성 검사
	if ( pclinfo->m_siOrgUnique <= 0 )						return;
	if ( pclinfo->m_siOrgIndex < 0  || pclinfo->m_siOrgIndex >= MAX_ITEM_PER_PERSON)	return;
	if ( pclinfo->m_siEleIndex < 0  || pclinfo->m_siEleIndex >= MAX_ITEM_PER_PERSON)	return;

	if ( pclPerson->clCharItem.clItem[ pclinfo->m_siOrgIndex ].siUnique != pclinfo->m_siOrgUnique )	return;
	if ( pclPerson->clCharItem.clItem[ pclinfo->m_siEleIndex ].siUnique != ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE )	return;

	// 속성변경 아이템의 갯수가 없다면
	if ( pclPerson->clCharItem.clItem[ pclinfo->m_siEleIndex ].siItemNum <= 0 )			return;

	// 인첸트 타입이 없거나(인첸트가 되지 않음) 변경하고자 하는 인첸트 타입과 같다면
	SI32 siEnchantType = pclPerson->clCharItem.clItem[ pclinfo->m_siOrgIndex ].GetEnchantType( pclClient->pclItemManager );
	if ( (siEnchantType <= 0) || (siEnchantType == pclinfo->m_siChangeType) )			return;

	// 변환아이템과 속성변경 아이템 설정을 한다.
	cltItem clChangeItem;
	clChangeItem.Set( &pclPerson->clCharItem.clItem[ pclinfo->m_siOrgIndex ] );
	clChangeItem.Element.siType = pclinfo->m_siChangeType;

	cltItem	clElementItem;
	clElementItem.siUnique 	= ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE;
	clElementItem.siItemNum	= 1;

	// 아이템 사용을 예약한다
	((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclinfo->m_siOrgIndex , pclClient->CurrentClock );
	((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclinfo->m_siEleIndex , pclClient->CurrentClock );


	// 아이템 속성 변경 요청을 DB로 보냄
	sDBRequest_ChangeEnchant clMsg(id, 
		pclPerson->GetPersonID(), 
		pclinfo->m_siOrgIndex, 
		&clChangeItem,
		pclinfo->m_siEleIndex, 
		&clElementItem
	);	

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_ITEMMOVE(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->CR[id]->siPersonalMakeItemType > 0) return; //제조시 아이템 이동 안된다.
	if( pclCM->CR[id]->bPersonalEnchantItemSwitch == true) return; 
	if(pclCM->CR[id]->pclCI->GetPersonID() <= 0 ) return; // 어찌되었든 잘못된 거다.


	// 아이템 이동 정보를 확보한다. 
	cltMoveItemInfo* pclmoveiteminfo = (cltMoveItemInfo*)pclMsg->cData;

	cltItem* pclFrom = NULL;
	cltItem  clTo;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	SI32 slot = -1;

	// 부적절한 데이터를 확인한다. 
	if(pclmoveiteminfo->uiTo < 0	|| pclmoveiteminfo->uiTo >= MAX_ITEM_PER_PERSON)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMMOVE - PersonID = %d, IP =  %s , uiTo = %d"),	
																	pclCM->CR[id]->pclCI->GetPersonID(),
																	pclCM->CR[id]->GetSession()->GetIP(),
																	pclmoveiteminfo->uiTo
																	);
		return ;
	}		
	if(pclmoveiteminfo->uiFrom < 0	|| pclmoveiteminfo->uiFrom >= MAX_ITEM_PER_PERSON)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMMOVE - PersonID = %d, IP =  %s , uiFrom = %d"),	
																	pclCM->CR[id]->pclCI->GetPersonID(),
																	pclCM->CR[id]->GetSession()->GetIP(),
																	pclmoveiteminfo->uiFrom
																	);
		return ;
	}	

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 이동 안된다.
	if( pclchar->bPersonalShopSwitch == true  && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclmoveiteminfo->uiTo	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
                if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclmoveiteminfo->uiFrom	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			// 귀속전용 개인상점
			if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclmoveiteminfo->uiTo	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			if ( (0 < pclchar->clShopData.m_clSpacialItem.siUnique) && (pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclmoveiteminfo->uiFrom) )
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}
	}


	// 내구도 정보 저장.  - 내구도 관련 삭제.
	if(pclmoveiteminfo->uiTo < PERSONITEM_INV0 || pclmoveiteminfo->uiFrom < PERSONITEM_INV0)
	{
		pclchar->SendDBWasteDurability();
		pclchar->SendDBSummonWasteDurability();
	}

	//------------------------------------------------
	// 말을 갖고 있지 않으면 말 인벤에 넣을 수는 없다.
	//------------------------------------------------
	if(pclCM->CR[id]->HaveHorse() == false)
	{
		if(pclmoveiteminfo->uiTo >= PERSONITEM_HORSE0 && pclmoveiteminfo->uiTo < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY)return ;
	}

	//--------------------------------------------
	//From의 아이템을 구한다.
	//--------------------------------------------
	pclFrom		= &pclCM->CR[id]->pclCI->clCharItem.clItem[pclmoveiteminfo->uiFrom];			

	// 아이템이 무효한 것이라면..
	if(pclFrom == NULL || pclFrom->siUnique <=0 ||pclFrom->siItemNum <= 0)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		if(pclFrom == NULL )
		{
			pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMMOVE - PersonID = %d, IP =  %s , pclFrom = NULL"),	
																		pclCM->CR[id]->pclCI->GetPersonID(),
																		pclCM->CR[id]->GetSession()->GetIP()
																		);
		}
		else
		{
			pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMMOVE - PersonID = %d, IP =  %s , siUnique = %d,  siItemNum = %d"),	
																		pclCM->CR[id]->pclCI->GetPersonID(),
																		pclCM->CR[id]->GetSession()->GetIP(),
																		pclFrom->siUnique ,
																		pclFrom->siItemNum											
																		);
		}
		return ;
	}
	
	//------------------------------------------------
	// 변신 중에는 아이템을 장착할 수 없다.
	//------------------------------------------------
	if( pclCM->CR[id]->clTransFormInfo.bTransFormMode &&
		pclmoveiteminfo->uiTo >= PERSONITEM_HELMET && pclmoveiteminfo->uiTo < PERSONITEM_HELMET + MAX_SHAPE_ITEM )
	{
		//KHY - 0129 - 변신중이라도, 화살과 총알은 장착및 해제가능
		switch(pclFrom->siUnique)
		{
			case ITEMUNIQUE(7850) : // 화살
			case ITEMUNIQUE(7855) : // 총알
			case ITEMUNIQUE(13600) : // 무한 화살.
			case ITEMUNIQUE(13601) : // 무한 총알.
				break;
			default:
				// 변신중에는 아이템을 장착할 수 없습니다.
				return;
		}
	}

	//------------------------------------------------
	// 변신 중에는 아이템을 장착을 해제 할 수 없다.
	//------------------------------------------------
	if( pclCM->CR[id]->clTransFormInfo.bTransFormMode &&
		pclmoveiteminfo->uiFrom >= PERSONITEM_HELMET && pclmoveiteminfo->uiFrom < PERSONITEM_HELMET + MAX_SHAPE_ITEM )
	{
		//KHY - 0129 - 변신중이라도, 화살과 총알은 장착및 해제가능
		switch(pclFrom->siUnique)
		{
			case ITEMUNIQUE(7850) : // 화살
			case ITEMUNIQUE(7855) : // 총알
			case ITEMUNIQUE(13600) : // 무한 화살.
			case ITEMUNIQUE(13601) : // 무한 총알.
				break;
			default:
				// 변신중에는 아이템을 장착을 해제할 수 없습니다.
				return;
		}
	}

	// 트레이드중인 것은 안된다!
	if ( ((cltCharServer*)pclCM->CR[id])->GetNoMoveItemSwitch(pclmoveiteminfo->uiFrom) != 0 )
	{
		return ;
	}

	//----------------------------------------------------
	// 가방을 갖고 있지 않으면 가방 인벤에 넣을 수는 없다
	//----------------------------------------------------
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique == 0)
	{
		if(pclmoveiteminfo->uiTo >= PERSONITEM_BAG0 && pclmoveiteminfo->uiTo < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG)return ;
	}

	//----------------------------------------------------
	// 가방2을 갖고 있지 않으면 가방 인벤에 넣을 수는 없다
	//----------------------------------------------------
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique == 0)
	{
		if(pclmoveiteminfo->uiTo >= PERSONITEM_BAG1 && pclmoveiteminfo->uiTo < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG)return ;
	}

	//----------------------------------------------------
	// 장착된 가방을 가방 인벤내로 옮길 수는 없다.
	//----------------------------------------------------
	if ( pclmoveiteminfo->uiFrom == PERSONITEM_BAG )
	{
		if( pclmoveiteminfo->uiTo >= PERSONITEM_BAG0 && pclmoveiteminfo->uiTo < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG )
		{
			return;
		}

		//----------------------------------------------------
		// 가방에 물건을 둔채로 장착을 해제 시킬수는 없다.
		//----------------------------------------------------
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG; ++t )
		{
			if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				return;
			}
		}
	}


	//----------------------------------------------------
	// 장착된 가방을 가방 인벤내로 옮길 수는 없다. (2)
	//----------------------------------------------------
	if ( pclmoveiteminfo->uiFrom == PERSONITEM_BAG_SUB )
	{
		if( pclmoveiteminfo->uiTo >= PERSONITEM_BAG1 && pclmoveiteminfo->uiTo < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG )
		{
			return;
		}

		//----------------------------------------------------
		// 가방에 물건을 둔채로 장착을 해제 시킬수는 없다.
		//----------------------------------------------------
		for ( SI16 t = PERSONITEM_BAG1; t < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG; ++t )
		{
			if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				return;
			}
		}
	}


	//-----------------------------------------------
	// To 에 넣는다. 
	//-----------------------------------------------
	clTo.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclmoveiteminfo->uiTo]);

	// 이미 기존에 물품이 있는 곳인지 확인한다. 
	if(clTo.siUnique)
	{
		// 다른 아이템이면 실패. 
		if(clTo.siUnique != pclFrom->siUnique)return ;

		// 누적되지 않는 아이템은 실패. 
		if(clTo.CanPile(pclItemManager) == false)return ;
	}
	// 옮기는 목표쪽 체크 한다.-> 유니크는 있는데 갯수가 없으면 잘 못된 아이템 
	if(clTo.siUnique > 0 && clTo.siItemNum <= 0)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMMOVE_clTo - PersonID = %d, IP =  %s , siUnique = %d,  siItemNum = %d"),	
			pclCM->CR[id]->pclCI->GetPersonID(),
			pclCM->CR[id]->GetSession()->GetIP(),
			clTo.siUnique ,
			clTo.siItemNum											
			);
		//return ;
	}

	// 적절하지 않은 곳으로는 이동시킬 수 없다. 
	if(pclItemManager->CheckProperItemPos(pclFrom, pclmoveiteminfo->uiTo, pclchar->GetKind()) == FALSE)return ;

	//------------------------------------------
	// 장착 조건을 충족시키는지 확인한다. 
	//------------------------------------------
	SI32 returnval = 0;
	if(pclCM->CR[id]->CheckEquipCondition(pclItemManager, pclmoveiteminfo->uiTo, pclFrom, &returnval) == false)return ;

	//------------------------------------------
	// 밀봉상태 아이템은 장착불가 
	// 기간이 지난 아이템은 장착불가 - by LEEKH
	//------------------------------------------
	if((pclmoveiteminfo->uiTo >= 0 && pclmoveiteminfo->uiTo<= 14) || pclmoveiteminfo->uiTo == 82 || pclmoveiteminfo->uiTo== 97)
	{
		if(pclFrom->IsSealingUp(pclItemManager)) return;

		SI32 useableDay;
		BOOL rVal = GetItemUseAbleDay(pclFrom, &useableDay);
		if(rVal == TRUE && useableDay < 0)		return;
	}

	//---------------------------------------------------------------------------
	// 여기까지 통과해야 아이템 이동이 성공한다.
	
	// 결정체 크리스탈 내구도 정보 저장.  - 
	if( pclmoveiteminfo->uiFrom < PERSONITEM_INV0)
	{
		pclchar->SendDBCrystalDurability();
		pclchar->SendClientCrystalDurability();
	}

	//-----------------------------------------------
	//DB메니저로 아이템을 이동하라는 요청을 보낸다. 
	//-----------------------------------------------
	cltItem clitem;
	clitem.Set(pclFrom);

	//cyj 옮기는 아이템의 총합이 MAX_ITEM_PILE_NUMBER를 넘으면 남는 아이템을 다른 인벤토리에 저장하도록
	if (DistributePileMoveItem(id, clitem, pclmoveiteminfo->uiFrom, clTo, pclmoveiteminfo->uiTo) == FALSE)
	{
		bool Canpile = false;
		if(( pclFrom->CanPile(pclItemManager))&&( clTo.CanPile(pclItemManager)))
			Canpile = true;
		
		sDBRequest_MoveItem clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclmoveiteminfo->uiFrom, &clitem, Canpile,
									pclmoveiteminfo->uiTo, &clTo);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);


	}
}

void cltServer::UseItemAction(SI32 id, cltItemUseInfo* pcliteuseinfo)
{
	cltItem* pclFrom = NULL;
	cltItem* pclTo	 = NULL;

	SI32 slot = -1;

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;

	//	--------------------------------------------


	
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// 감옥에서는 아이템 사용 불가 불가. 
	if( pclchar->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
	{			
		SendServerResponseMsg(0, SRVAL_CAN_NOT_USE_ITEM_IN_PRISON, 0, 0, id);
		return;
	}
	

	//--------------------------------------------
	//From의 아이템을 구한다.
	//--------------------------------------------
	if(pcliteuseinfo->uiFrom < 0 || pcliteuseinfo->uiFrom >= MAX_ITEM_PER_PERSON)	return;
	pclFrom = &pclCM->CR[id]->pclCI->clCharItem.clItem[pcliteuseinfo->uiFrom];

	// 아이템이 무효한 것이라면..
	if(pclFrom == NULL || pclFrom->siUnique ==0)
	{
		return ;
	}

	// 클라이언트에서 보내온 아이템의 유니크와 서버의 아이템 유니크가 다르면 리턴, 
	if(pclFrom->siUnique != pcliteuseinfo->siItemUnique)
	{
		return ;
	}


	// 아이템을 사용할 수 있는 것이라면 
	SI32 returnval = 0;
	bool temp = pclCM->CR[id]->CanUseItem(*pclFrom, &returnval,pcliteuseinfo->uiFrom);
	if( temp == true
		&& pclFrom->IsBulletItem() == false )
	{
		if( true == pclchar->bPersonalShopSwitch ||
			true == pclchar->bPremiumPersonalShop )
		{
			for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
			{
				if( pclchar->clShopData.m_clItem[i].siUnique == pclFrom->siUnique  )
				{
					if( pclchar->clShopData.m_clItem[i].siItemNum >= pclFrom->siItemNum )
					{
						SendServerResponseMsg(0, SRVAL_FAIL_PERSONALSHOP_USEITEM, 0, 0, id);
						return;
					}
				}
			}
		}

		cltItem clItem;
		clItem.Set(pclFrom);
		clItem.SetItemNum(1);	// 한개만 사용하게 한다. !!!!!

		//------------------------------------------------
		// 선조치 속성의  물품이라면 먼저 조치한다. 
		// 이호식 상자일 경우에만 예외 처리..!
		//-------------------------------------------------
		SI64 atb = pclItemManager->GetItemAtb(clItem.siUnique);
		if(atb & ITEMATB_FIRSTACTION || pclItemManager->IsItemInfoAtb(clItem.siUnique, ITEMINFOATB_FIRSTACTION) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			if ( pclchar->UseItem(clItem,pcliteuseinfo->uiFrom) == false ) return ;
			while(pclchar->clItemUseReserveInfo.Push(clItem.siUnique, -clItem.siItemNum, pcliteuseinfo->uiFrom) == false)
			{
				pclchar->ItemUseReserveAction();
			}

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			if(pclFrom->siItemNum == 1)
			{
				pclchar->ItemUseReserveAction();
			}
			// pclchar->ItemUseReserveAction(); 밑에 있어야 한다. 
			// 그렇지 않으면 인벤에서 물품의 위치를 찾을 수 없다.!
			pclFrom->Decrease(1);

			// 클라이언트로 사용후 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pcliteuseinfo->uiFrom, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		// 일반 물품은 DB로 아이템을 소모할 것을 요청한다. 
		else
		{
			sDBRequest_UseItem clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pcliteuseinfo->uiFrom, &clItem,USEITEM_REASON_NONE);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
	}

}
void cltServer::DoMsg_GAMEMSG_REQUEST_USEITEM(cltMsg* pclMsg, SI32 id)
{
	// 아이템 사용 정보를 얻는다. 
	cltItemUseInfo* pcliteuseinfo = (cltItemUseInfo*)pclMsg->cData;

	UseItemAction(id, pcliteuseinfo);

	return ;

}

// 아이템 줍기에 대한 요청 처리. 
void cltServer::DoMsg_GAMEMSG_REQUEST_INVPICKUP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_InvPickUp* pclinfo = (cltGameMsgRequest_InvPickUp*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	//-------------------------------------------------
	// 아이템이 유효한지 확인한다. 
	//-------------------------------------------------
	if(pclinfo->siItemUnique <=0 || pclinfo->siItemUnique >= MAX_ITEM_UNIQUE)return ;
	if(pclinfo->siItemIndex < 0)return ;

	//-------------------------------------------------
	// 주어진 좌표에 실제로 해당 아이템이 떨어져 있는지 확인한다. 
	//-------------------------------------------------
	cItemNode* pitemnode = pclItemList->GetItem(pclinfo->siItemIndex);
	if(pitemnode == NULL)return ;

	if(pitemnode->item.siUnique != pclinfo->siItemUnique)return ;
	if(pitemnode->item.siItemNum	<= 0)return ;
	if(pitemnode->mapIndex		!= pclinfo->siMapIndex)return;
	if(pitemnode->x				!= pclinfo->siX)return;
	if(pitemnode->y				!= pclinfo->siY)return;

	//-------------------------------------------------
	// 인벤에 아이템을 넣을 수 있는지 확인한다. 
	//-------------------------------------------------
	// 해당 인벤에 빈자리가 있는지 확인한다. 
	SI16 sitopos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &pitemnode->item, &sitopos,&cltoitem, &rtnvalue) ==  false)
	{
		return ;
	}	

	//-------------------------------------------------
	// 게임 서버에서 아이템을 삭제하고 인벤에 아이템을 넣도록 DB에 요청한다. 
	//--------------------------------------------------
	if(pclMapManager->m_world.GetMap(pclinfo->siMapIndex)->DelItem(pitemnode->x, pitemnode->y, pclinfo->siItemIndex) == TRUE)
	{
		if(pclItemList->RemoveItem(pclinfo->siItemIndex) == TRUE)
		{
			// 아이템이 추가되는 자리를 예약한다. 
			pclchar->SetInputItemReserve(sitopos, pclClient->CurrentClock);

			// 사용자에게 아이템을 넣는 메시지를 사용한다. 
			sDBRequest_InputItemToPerson clMsg(id, personid, sitopos, INPUTITEMPERSON_OPTION_PICKUP, pitemnode->item.CanPile(pclItemManager), &pitemnode->item, pitemnode->siPersonID, pitemnode->item.IsCreateSID(pclItemManager));
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			// [로그추가 : 황진성 2007. 10. 5] // 사용자에게 아이템을 넣자.
			//// param1 == 아이템 위치, param2 == 이유, param3 == 중첩 허용 여부. param4 == 아이템을 준 사용자	
			//cltServer * pclserver = (cltServer*)pclClient;
			//pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_GET_ITEM, 
			//								0, (cltCharServer*)pclClient->pclCM->CR[id], NULL, personid, 0, 0, &pitemnode->item, 0, 0, 0, 0, 0, 0, 
			//								sitopos, INPUTITEMPERSON_OPTION_PICKUP, pitemnode->item.CanPile(pclItemManager), pitemnode->siPersonID, 0, NULL, NULL);
			//}
		}
	}


}

void cltServer::DoMsg_GAMEMSG_REQUEST_MAKEITEMSWITCH(cltMsg* pclMsg, SI32 id)
{	
	cltGameMsgRequest_MakeItemSwitch* pclinfo = (cltGameMsgRequest_MakeItemSwitch*)pclMsg->cData;

	pclCM->CR[id]->siPersonalMakeItemType = pclinfo->siType;

	// 
	cltGameMsgResponse_MakeItemSwitch clinfo( pclinfo->siType );
	cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMSWITCH, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//----------------------------------------------
	// 이웃들에게도 알린다. 
	//----------------------------------------------
	((cltCharServer*)pclCM->CR[id])->SendNeighbourMyMakeItemStatus();
}


// 아이템 드립에 대한 요청 처리. 
void cltServer::DoMsg_GAMEMSG_REQUEST_INVDROP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_InvDrop* pclinfo = (cltGameMsgRequest_InvDrop*)pclMsg->cData;

	if( pclCM->CR[id]->siPersonalMakeItemType > 0 ) return; //제조시 아이템 이동 안된다.
	if( pclCM->CR[id]->bPersonalEnchantItemSwitch == true) return; 

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;




	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	//---------------------------------------------
	// 해당 아이템이 실제로 존재하는지 확인한다. 
	//---------------------------------------------
	SI32 itempos = pclinfo->siItemPos;

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;

	// 실제로 해당 아이템이 있는지 확인. 
	cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[itempos];

	if(pclitem->IsSame(&pclinfo->clItem, false) == FALSE)
	{
		// 실제로 그 위치에 있는 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
		cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		return ;
	}


	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 이동 안된다.

	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == itempos )
			{
				if( pclchar->clShopData.m_clItem[i].siUnique > 0  )
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
			}
		}
		
		// 귀속전용 개인상점
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == itempos	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}


	// 버리는 아이템이 실제 보유하고 있는 아이템보다 많을 수는 없다. 
	if(pclitem->GetItemNum() < pclinfo->clItem.GetItemNum())return ;

	// 트레이드중인 것은 안된다!
	if ( ((cltCharServer*)pclCM->CR[id])->GetNoMoveItemSwitch(itempos) != 0 )
	{
		SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
		return ;
	}

	// SJY DEBUG
	if ( pclitem->siUnique == 8150 ||
		pclitem->siUnique == 14300 ||
		pclitem->siUnique == 14400 ||
		pclitem->siUnique == 14500 ) // 반격의 깃발 등
	{
		SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
		return;
	}

	//kkm 거래불가아이템은 못버리도록 수정
	if ( pclItemManager->IsItemInfoAtb( pclitem->siUnique, ITEMINFOATB_NOTTRADE) )
	{
		SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
		return;
	}

	//cyj 귀속아이템은 못버리도록 수정
	if ( pclitem->IsBelonging(pclItemManager))
	{
		SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
		return;
	}

	// lkh - 파티홀 아이템은 버릴 수 없다.
	if( pclitem->clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
	{
		SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
		return;
	}

	//------------------------------------------
	// 주변의 빈 자리를 찾는다. 
	//------------------------------------------
	SI32 mapindex	= pclCM->CR[id]->GetMapIndex();
	SI32 x			= pclCM->CR[id]->GetX();
	SI32 y			= pclCM->CR[id]->GetY();

	SI32 targetx, targety;
	if(pclMapManager->FindProperAreaForDrop(mapindex, x, y, &targetx, &targety) == true)
	{
		//------------------------------------------
		//아이템을 버리도록 DB매니저에게 요청한다. 
		//------------------------------------------
		sDBRequest_DropItem clMsg(id, personid, pclinfo->siItemPos, &pclinfo->clItem, mapindex, targetx, targety);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

}

// 귀속아이템 파괴 요청 처리
void cltServer::DoMsg_GAMEMSG_REQUEST_BELONGITEM_DESTROY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_InvDrop* pclinfo = (cltGameMsgRequest_InvDrop*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	//---------------------------------------------
	// 해당 아이템이 실제로 존재하는지 확인한다. 
	//---------------------------------------------
	SI32 itempos = pclinfo->siItemPos;

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;

	// 실제로 해당 아이템이 있는지 확인. 
	cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[itempos];

	if(pclitem->IsSame(&pclinfo->clItem, false) == FALSE)
	{
		// 실제로 그 위치에 있는 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
		cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		return ;
	}
	// 버리는 아이템이 실제 보유하고 있는 아이템보다 많을 수는 없다. 
	if(pclitem->GetItemNum() < pclinfo->clItem.GetItemNum())return ;

	/*	SI32 UseDate;
	if ( pclItemManager->GetLimitDateItemValue( pclitem->siUnique, &UseDate ) ||*/

	// SJY DEBUG
	if ( pclitem->siUnique == 8150 ||
		pclitem->siUnique == 14300 ||
		pclitem->siUnique == 14400 ||
		pclitem->siUnique == 14500 ) // 반격의 깃발 등
	{
		SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
		return;
	}

	if ( IsCountrySwitch(Switch_DestroyDropItem) ){
	}
	else{
		//kkm 거래불가아이템은 못버리도록 수정
		if ( pclItemManager->IsItemInfoAtb( pclitem->siUnique, ITEMINFOATB_NOTTRADE) )
		{
			SendServerResponseMsg(0, SRVAL_CANNOTDROPITEM, 0, 0, id );
			return;
		}
	}
	sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),itempos ,&pclinfo->clItem,USEITEM_REASON_BELONGITEM);
	pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);


}

void cltServer::DoMsg_GAMEMSG_REQUEST_DISASSEMBLEITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_ItemDisAssemble* pclinfo = (cltGameMsgRequest_ItemDisAssemble*)pclMsg->cData;

	if (pclClient->pclCM->IsValidID(id) == false)
		return;

	cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;

	// 개인상점을 열고 있으면 아이템 선택을 못하도록 했다
	if ( (true == pclchar->bPersonalShopSwitch) || 
		 (true == pclchar->bPremiumPersonalShop) )
	{
		return;
	}
		
	// 추가 : 황진성 2008. 2. 28 => 틴틴분해이면 틴틴저장 메시지를 DB로 보낸다.]
	if( ItemDisAssemble_NPC_KIND::NPC_TINTIN == pclinfo->m_siNPCKind )		
	{
		if( pclinfo->siItemPos < PERSONITEM_INV0 || pclinfo->siItemPos >= PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON)
			return;

		if ( pclinfo->clItem.siUnique != pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siUnique )
			return;

		cltItem clItem;
		clItem.Set( &pclchar->pclCI->clCharItem.clItem[ pclinfo->siItemPos ] );

		if (clItem.siUnique <= 0)
			return;

		if( clItem.IsDisassemble(pclClient->pclItemManager) == false)
		{
			SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_CANNOTITEM, 0, 0, id );
			return;
		}

	/*	SI64 itematb = pclItemManager->GetItemAtb( clItem.siUnique );

		SI32 itemtype = pclItemManager->GetItemType( clItem.siUnique );


		if((itematb & ITEMATB_OBJECT) == 0 && pclGachaManager->IsGachaRareItem( clItem.siUnique ) == false )
			return;

		if ( pclItemManager->IsItemInfoAtb( clItem.siUnique, ITEMINFOATB_ITEMMALL) == false )
		{
			if ( pclItemManager->IsItemInfoAtb( clItem.siUnique, ITEMINFOATB_TRADELIST) == false )
			{
				SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_CANNOTITEM, 0, 0, id );
				return;
			}

			
			if( itemtype == ITEMTYPE_NECK || itemtype == ITEMTYPE_MANTLE || itemtype == ITEMTYPE_RING )
				return;
		}

		// tys[06/04/09] - 20 레벨 이상 물품만 분해 가능,
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( clItem.siUnique );
		SI32 equipLevel = pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel;

		if ( equipLevel <= 20 && pclClient->IsWhereServiceArea(ConstSwitch_WENNYDISASSEMBLELEVEL) )
		{
			return;
		}*/

		//---------------------------------------------
		// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclchar->ItemUseReserveAction();

		//---------------------------------------------
		// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
		//---------------------------------------------
		pclchar->BulletUseReserveAction();

		SI32 wennypercent = GetAverageWithWeenyOldMan();
		//SI16 wennypercentMsg = 0;

		//// 보여줄 메세지 설정
		//if( wennypercent <= 80 )		wennypercentMsg = WENNY_PERCENT_SMALL;
		//else if ( wennypercent <= 120 )	wennypercentMsg = WENNY_PERCENT_MEDIUM;
		//else wennypercentMsg = WENNY_PERCENT_LARGE;

		cltItem DisAssembleItem[MAX_ITEM_PER_WENNYOLD];
		for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
		{
			DisAssembleItem[i].Init();
		}

		SI16 siWennyResult = 0;
		SI16 siResolutionRate = 0;
		
		
	/*
		//if ( 과금아이템? )
		if ( pclItemManager->IsItemInfoAtb( pclinfo->clItem.siUnique,ITEMINFOATB_ITEMMALL) == true )
		{
			//	아이템 사용기간의 남은 일수를 구한다.
			//  switch( 과금 아이템 환원 결정 루틴 )
			//		case 1 -> 목걸이나옴 ( 10%부터 남은일수 +1%, 최대 30%)
			//		case 2 -> 이호식상자 ( 나머지 확률 )
			//		case 3 -> 실패 ( 2% 고정 )
			siWennyResult = pclItemManager->GetWennyOldItem( &pclinfo->clItem, &siResolutionRate , true );
		}
		//else( 일반 아이템이면 )
		else
		{
			//	일반 아이템 물가 시세를 계산한다.
			//  switch( 일반 아이템 환원 결정 루틴 )
			//		case 1 -> 이호식 상자
			//		case 2 -> 분해아이템의 재료 환원
			//					아이템 재료 분해율 구함
			//					분해율 많큼 아이템 재료 환원
			//		case 3 -> 재료 200% 환원
			siWennyResult = pclItemManager->GetWennyOldItem( &pclinfo->clItem, &siResolutionRate , false );
		}
	*/
		
		siWennyResult = pclItemManager->GetWennyOldItem( &clItem, &siResolutionRate );

		switch( siWennyResult )
		{
		case WENNYMSG_SMALL:
		case WENNYMSG_MEDIUM:
		case WENNYMSG_LARGE:
		case WENNYMSG_DOUBLE:
			{		
				pclClient->pclItemManager->GetWeenyOldManMaterialInfo(siResolutionRate, clItem.siUnique, DisAssembleItem);
			}
			break;
		case WENNYMSG_NECKLACE:
			{
				cltItem clToItem;
				SI32 bigitemunique = pclClient->pclItemManager->GetRandBigWennyItem();
				SI16 brareswitch = 0;
				pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);
				DisAssembleItem[0].Set(&clToItem);
			}
			break;
		case WENNYMSG_BOX:
			{		
				cltItem clToItem;
				clToItem.Init();
				SI16 brareswitch = 0;
				pclClient->pclItemManager->MakeRandItemUnique( ITEMUNIQUE(13301), &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);
				clToItem.siItemNum = 1;
				DisAssembleItem[0].Set( &clToItem );
			}
			break;
		case WENNYMSG_NONE:
			{
				// 빵(3750)을 준다
				cltItem clToItem;
				clToItem.Init();
				SI16 brareswitch = 0;
				pclClient->pclItemManager->MakeRandItemUnique( ITEMUNIQUE(3750), &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);
				clToItem.siItemNum = 100;
				DisAssembleItem[0].Set( &clToItem );
			}
			break;
		case WENNYMSG_WENNYMATERIAL:
			{
				cltItem clToItem;
				clToItem.Init();
				SI16 brareswitch = 0;

				SI32 siItemTable[] = {
					ITEMUNIQUE(6140),	// 마호가니
					ITEMUNIQUE(5380),	// 티타늄
					ITEMUNIQUE(7175)	// 아라미드
				};

				SI32 siRand = rand() % ( sizeof(siItemTable) / sizeof(SI32) );

				pclClient->pclItemManager->MakeRandItemUnique( siItemTable[siRand] , &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);

				// 1~100 개 까지의 아이템을 준다.
				clToItem.siItemNum  = rand() % 100 + 1;
				
				DisAssembleItem[0].Set( &clToItem );
			}
			break;
		//case WENNYMSG_FORTUNE_COOKIE:
		//	{
		//		cltItem clToItem;
		//		clToItem.Init();
		//		SI16 brareswitch = 0;

		//		SI32 siItemTable[] = {
		//			ITEMUNIQUE(13062);	// 포춘쿠키 조각.	
		//		};

		//		SI32 siItemNum = 1;
		//		
		//		pclClient->pclItemManager->MakeRandItemUnique( siItemTable[siItemNum], &clToItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, 0);
		//	}
		//	break;
		case WENNYMSG_GACHARARE:
			{
				cltItem clToItem;
				clToItem.Init();
				SI16 brareswitch = 0;
				SI32 MakeItemUnique = 0;
				SI32 MakeItemNum = 1;
				SI32 rndVal = rand()%100;

				if( rndVal < 5 )
				{
					MakeItemUnique = ITEMUNIQUE(13604);
					MakeItemNum	= 2;
				}
				else if( rndVal < 20 )
				{
					MakeItemUnique = ITEMUNIQUE(13604);
					MakeItemNum	= 1;
				}
				else if( rndVal < 30 )
				{
					MakeItemUnique = ITEMUNIQUE(13603);
					MakeItemNum	= 2;
				}
				else if( rndVal < 50 )
				{
					MakeItemUnique = ITEMUNIQUE(13603);
					MakeItemNum	= 1;
				}
				else if( rndVal < 70 )
				{
					MakeItemUnique = ITEMUNIQUE(13602);
					MakeItemNum	= 2;
				}
				else 
				{
					MakeItemUnique = ITEMUNIQUE(13602);
					MakeItemNum	= 1;
				}

				pclClient->pclItemManager->MakeRandItemUnique( MakeItemUnique , &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);
				clToItem.SetItemNum(MakeItemNum);

				DisAssembleItem[0].Set( &clToItem );		

			}
			break;
		//KHY - 0115 발렌타인데이 이벤트.	
		case WENNYMSG_VAlENTINEITEM :
			{
				cltItem clToItem;
				clToItem.Init();
				SI16 brareswitch = 0;
				SI32 MakeItemUnique = 0;
				SI32 MakeItemNum = 1;
				SI32 rndVal = rand()%100;

				if( rndVal < 45 )
				{
					MakeItemUnique = ITEMUNIQUE(3916);
					MakeItemNum	= 1;
				}
				else if( rndVal < 50 )
				{
					MakeItemUnique = ITEMUNIQUE(13040);
					MakeItemNum	= 1;
				}
				else if( rndVal < 70 )
				{
					MakeItemUnique = ITEMUNIQUE(2010);
					MakeItemNum	= (rand()%5) +1;
				}
				else if( rndVal < 90 )
				{
					MakeItemUnique = ITEMUNIQUE(2016);
					MakeItemNum	= (rand()%5) +1;
				}
				else if( rndVal < 100 )
				{
					MakeItemUnique = ITEMUNIQUE(3875);
					MakeItemNum	= 1;
				}


				pclClient->pclItemManager->MakeRandItemUnique( MakeItemUnique , &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);
				clToItem.SetItemNum(MakeItemNum);

				DisAssembleItem[0].Set( &clToItem );		
			}
			break;
		//KHY - 0218 화이트데이 이벤트.
		case WENNYMSG_WHITEITEM :
			{
				cltItem clToItem;
				clToItem.Init();
				SI16 brareswitch = 0;
				SI32 MakeItemUnique = 0;
				SI32 MakeItemNum = 1;
				SI32 rndVal = rand()%100;

				if( rndVal < 55 )
				{
					MakeItemUnique = ITEMUNIQUE(3917); //캔디가루.
					MakeItemNum	= 1;
				}
				else if( rndVal < 65 )
				{
					MakeItemUnique = ITEMUNIQUE(13041); //캔디 응축기.
					MakeItemNum	= 1;
				}
				else if( rndVal < 80 )
				{
					MakeItemUnique = ITEMUNIQUE(5010); //철광석.
					MakeItemNum	= (rand()%5) +1;
				}
				else if( rndVal < 95 )
				{
					MakeItemUnique = ITEMUNIQUE(5075); //석회석.
					MakeItemNum	= (rand()%3) +1;
				}
				else if( rndVal < 100 )
				{
					MakeItemUnique = ITEMUNIQUE(3875); //초코파이
					MakeItemNum	= 1;
				}


				pclClient->pclItemManager->MakeRandItemUnique( MakeItemUnique , &clToItem, 0, 0, &brareswitch,0,0,0,0,0,0);
				clToItem.SetItemNum(MakeItemNum);

				DisAssembleItem[0].Set( &clToItem );		
			}
			break;		
		// 수리석
		case  WENNYMSG_REPAIR_DUR:
			{
				cltItem clToItem;
				clToItem.Init();

				SI16 siRareSwitch			= 0;
				SI32 siRepareStoneUnique	= ITEMUNIQUE(8123);

				// 공식에 기본이 되는 값은 은광석
				SI32 siSilverMineralUnique	= ITEMUNIQUE(5050);

				pclClient->pclItemManager->MakeRandItemUnique( siRepareStoneUnique, &clToItem, 0, 0, &siRareSwitch, 0,0,0,0,0,0 );

				cltItemPriceUnit*	pclSilverPriceUnit	= pclItemManager->GetCurrentPrice( siSilverMineralUnique );
				cltItemPriceUnit*	pclGetItemPriceUnit	= pclItemManager->GetCurrentPrice( clItem.siUnique );
				GMONEY				gmTempValue			= 0;
				SI16				siCalcItemNum		= 0;

				if ( (NULL != pclSilverPriceUnit) && (NULL != pclGetItemPriceUnit) )
				{
					gmTempValue = pclSilverPriceUnit->siPrice * 40;
					if ( 0 < gmTempValue )
					{
						gmTempValue		= max( 1, min( 1000, (pclGetItemPriceUnit->siPrice / gmTempValue) ) );

						siCalcItemNum	= (SI16)gmTempValue;
					}
					else
					{
						siCalcItemNum = 1;
					}
				}
				else
				{
					siCalcItemNum = 1;
				}

				clToItem.siItemNum = siCalcItemNum;

				DisAssembleItem[0].Set( &clToItem );			}
			break;
		case WENNYMSG_FAILED:
			SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_UNLUCK, 0, 0, id);
			return;
		}

		bool bSuccess = false;

		// 내구도에 의한 지급갯수 조정
		bool	bApplyDurability	= false; // 신내구도 스위치와 아래 스위치 문의 분해 종류일때 내구도에 의한 패널티를 적용한다
		REAL32	fDurabilityPenalty	= 0;
		SI16	siPenaltyPercent	= 0;

		if ( IsCountrySwitch(Switch_NewDurability) )
		{
			switch ( siWennyResult )
			{
			// 재료 
			case WENNYMSG_SMALL:
			case WENNYMSG_MEDIUM:
			case WENNYMSG_LARGE:
			case WENNYMSG_DOUBLE:
			// 빵
			case WENNYMSG_NONE:
			// 레어재료
			case WENNYMSG_WENNYMATERIAL:
			// 내구도 수리석
			case WENNYMSG_REPAIR_DUR:
				{
					bApplyDurability	= true;
					fDurabilityPenalty	= clItem.GetDurabilityPenlty( pclItemManager );
					siPenaltyPercent	= (SI16)(100.0f * fDurabilityPenalty);
				}
				break;
			}
		}
		
		for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
		{
			if (DisAssembleItem[i].siUnique > 0)
			{
				bSuccess = true;
			
				// 내구도에 의한 지급갯수 패널티를 적용하자
				if ( bApplyDurability )
				{
					DisAssembleItem[i].siItemNum = (SI16)((REAL32)DisAssembleItem[i].siItemNum * fDurabilityPenalty);
				}
				
				if (DisAssembleItem[i].siItemNum <= 0)
				{
					DisAssembleItem[i].siItemNum = 1;
				}	

				if (DisAssembleItem[i].siItemNum > MAX_ITEM_PILE_NUMBER)
				{
					DisAssembleItem[i].siItemNum = MAX_ITEM_PILE_NUMBER;
				}

				// 혹부리(2호식) 해제로 나온 통계를 낸다.
				pclItemManager->AddFromTuneNum(DisAssembleItem[i].siUnique, DisAssembleItem[i].siItemNum);

				// 새로운 통계 - 혹부리(2호식) 해제로 나온 통계를 낸다.
				NStatistics_Add("Item", DisAssembleItem[i].siUnique, "FromTuneNum", DisAssembleItem[i].siItemNum);
				NStatistics_Set("Item", DisAssembleItem[i].siUnique, "Name", pclItemManager->GetName(DisAssembleItem[i].siUnique));

			}	// if (DisAssembleItem[i].siUnique > 0)
		}	// for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++) 종료


		// 혹부리(2호식) 해제 통계를 낸다.
		pclItemManager->AddTuneNum(pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siUnique, pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siItemNum);

		// 새로운 통계 - 혹부리(2호식) 해제 통계를 낸다.
		NStatistics_Add("Item", pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siUnique, "TuneNum", pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siItemNum);
		NStatistics_Set("Item", pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siUnique, "Name", pclItemManager->GetName(pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos].siUnique));

		if (bSuccess)
		{
			cltItem clUseItem;
			clUseItem.Set( &pclchar->pclCI->clCharItem.clItem[pclinfo->siItemPos] );
			clUseItem.siItemNum = 1;

			bool	bCreateSID[MAX_ITEM_PER_WENNYOLD];
			memset(bCreateSID, 0, sizeof(bCreateSID));

			// 물품들의 SID 생성 여부를 저장한다.
			for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
			{
				if(DisAssembleItem[i].siUnique > 0)
					bCreateSID[i] = DisAssembleItem[i].IsCreateSID(pclItemManager);
			}
			
			if( pclClient->IsCountrySwitch( Switch_InstanceDungeonTicket ) )
			{
				//[추가 : 황진성 2008. 1. 11 인던 출입 티켓 주기]
				if((rand()%100) < 10 + pclClient->pclItemManager->FindItemInfo(clUseItem.siUnique)->clCondition.siLevel)
				{
					for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
					{
						if(DisAssembleItem[i].siUnique <= 0)
						{
							DisAssembleItem[i].siUnique  = ITEMUNIQUE(24107);

							DisAssembleItem[i].siItemNum = 1;
							bCreateSID[i]				 =  DisAssembleItem[i].IsCreateSID(pclItemManager);
							break;
						}
					}
				}
			}
			if( pclClient->IsCountrySwitch( Switch_NewYearEvent_2010) ) // 2010신년이벤트 (이호식)
			{
				cltServer* pclserver = (cltServer*)pclClient;

				// 신년이벤트 스위치가 켜져있고, 포춘 이벤트 타임인 동안에만 포춘쿠키 조각 지급
				if( true == pclserver->pclEventTimeManager->InEventTime("Fortune_Event", &pclserver->sTime) )
				{
					SI16 sirandvalue = rand() % 10;

					if( sirandvalue == 0)	// 분해시 10% 확률로 조각 지급. [2009.12.24]
					{
						// [지연 - 2009.10.20 - 아이템 분해시 포춘쿠키조각 1개 지급]
						for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
						{
							if ( 0 >= DisAssembleItem[i].siUnique )
							{
								DisAssembleItem[i].siUnique  = ITEMUNIQUE(13062);
								DisAssembleItem[i].siItemNum = 1;
								bCreateSID[i]				 =  DisAssembleItem[i].IsCreateSID(pclItemManager);
								break;
							}
						}

					}
				}
			}
			if (pclClient->IsCountrySwitch( Switch_BlackArmyTicket )) 
			{
				bool bResult = true;
				SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( clItem.siUnique );
				SI32 DailyQuest3Itemrate = pclClient->pclItemManager->pclItemInfo[Ref]->clSkillInfo.siSkillLevel - 10;
				if ( DailyQuest3Itemrate < 1 )						bResult = false;
				if (siResolutionRate > 0 ) {
					if (DailyQuest3Itemrate / siResolutionRate < 1) 
						bResult = false;
				}
				// 나오는 확률
				if ( bResult == true) {
					float siRate = siResolutionRate/100;
					if ( max(1,DailyQuest3Itemrate / siRate ) > 1)
					{
						for (SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
						{
							if(DisAssembleItem[i].siUnique <= 0)
							{
								DisAssembleItem[i].siUnique  = ITEMUNIQUE(24084);
								DisAssembleItem[i].siItemNum = 1;
								bCreateSID[i]				 =  DisAssembleItem[i].IsCreateSID(pclItemManager);
								break;
							}
						}
					}
				}
			}
			
			sDBRequest_Wennyoldman_SaveInventory clMsg(id, personid, pclinfo->siItemPos, siWennyResult, siResolutionRate, &clUseItem , DisAssembleItem, bCreateSID, siPenaltyPercent );
			// 아이템 정보 저장
			clMsg.siUsedItemUnique = clMsg.clUseItem.siUnique;
			clMsg.siUsedItemNum = clMsg.clUseItem.siItemNum;
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_UNLUCK, 0, 0, id);
		}
	}
	// 추가 : 황진성 2008. 2. 28 => 아티펙트 환원이면 아티펙트 저장 메시지를 DB로 보낸다.]
	else if( ItemDisAssemble_NPC_KIND::NPC_ATIFACT == pclinfo->m_siNPCKind )			
	{
		bool	bCreateSID[MAX_ITEM_PER_WENNYOLD];
		memset(bCreateSID, 0, sizeof(bCreateSID));

		// 환원하는 아이템을 담을 구조체.
		cltItem RestoreItem[ITEM_BY_RESTORE_MAX_NUM];
		sizeof( RestoreItem, sizeof( RestoreItem ) );

		if( false == pclClient->pclItemManager->MakeItembyRestore(&pclinfo->clItem, RestoreItem, pclchar) )
			return;
				
		sDBRequest_Artifact_SaveInventory clMsg(id, personid, pclinfo->siItemPos, 0, 0, &pclinfo->clItem , RestoreItem, bCreateSID );
		
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_ItemDisAssemble_List* pclinfo = (cltGameMsgRequest_ItemDisAssemble_List*)pclMsg->cData;

	if (pclClient->pclCM->IsValidID(id) == false)
		return;

	cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;

	if( ItemDisAssemble_NPC_KIND::NPC_TINTIN == pclinfo->m_siNPCKind )
	{
		sDBRequest_Wennyoldman_InventoryList clMsg(id, personid);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else if( ItemDisAssemble_NPC_KIND::NPC_ATIFACT == pclinfo->m_siNPCKind )
	{
		sDBRequest_Artifact_InventoryList clMsg(id, personid);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_ItemDisAssemble_Recieve* pclinfo = (cltGameMsgRequest_ItemDisAssemble_Recieve*)pclMsg->cData;

	if (pclClient->pclCM->IsValidID(id) == false)
		return;

	if (pclinfo->clItem.siUnique <= 0)
		return;

	if (pclinfo->siStgPos < 0 || pclinfo->siStgPos >= MAX_ITEM_PER_WENNYOLD)
		return;

	cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
	SI32 personid = pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	cltItem clItem;
	clItem.Set(&pclinfo->clItem);

	SI16 invpos;
	cltItem cltoitem;
	SI32 rtnvalue;
	bool canpile = pclClient->pclItemManager->CanPile(clItem.siUnique);

	if(pclchar->CanAddInv(INVMODE_ALL, &pclinfo->clItem, &invpos, &cltoitem, &rtnvalue) == false)
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FULLINVEN, 0, 0, id);
		return ;
	}
	
	pclchar->SetInputItemReserve( invpos, CurrentClock );

	// 추가 : 황진성 2008. 2. 28 => 틴틴분해면 틴틴 이동 메시지를 DB로 보낸다.]
	if( ItemDisAssemble_NPC_KIND::NPC_TINTIN == pclinfo->m_siNPCKind )
	{
		sDBRequest_Wennyoldman_MoveItemStgToInv clMsg(id, pclinfo->siStgPos, personid, invpos, &clItem, canpile);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	// 추가 : 황진성 2008. 2. 28 => 아티펙트이면 아티펙트 이동 메시지를 DB로 보낸다.]
	else if( ItemDisAssemble_NPC_KIND::NPC_ATIFACT == pclinfo->m_siNPCKind )
	{
		sDBRequest_Artifact_MoveItemStgToInv clMsg(id, pclinfo->siStgPos, personid, invpos, &clItem, canpile);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
}

//--------------------------------------------
// 수리 함수. 
//--------------------------------------------
bool cltServer::Repair(SI32 mastercharunique, SI32 clientcharunique, SI32 clientitempos, cltItem* pclitem, GMONEY repairmoney, SI32* preturnval)
{
	//---------------------------------------------
	// 수리자가 유효한지 검사한다. 
	//---------------------------------------------
	if(pclCM->IsValidID(mastercharunique) == false)return false;

	//---------------------------------------------
	// 의뢰자가 유효한지 검사한다.
	//---------------------------------------------
	if(pclCM->IsValidID(clientcharunique) == false)return false;

	//---------------------------------------------
	// 해당 아이템이 실제로 존재하는지 확인한다. 
	//---------------------------------------------
	if(clientitempos < 0 || clientitempos >= MAX_ITEM_PER_PERSON)return false;
	cltItem* pclorgitem = &pclCM->CR[clientcharunique]->pclCI->clCharItem.clItem[clientitempos];
	// 주어진 아이템과 같은 아이템인지 확인한다. 
	if(pclitem->IsSame(pclorgitem, false) == false)
	{
		*preturnval = REPAIR_ERR_NOTSAMEITEM;
		return false;
	}
	if(pclitem->siUnique <= 0)return false;
	// 수리 불가능한 아이템이어도 실패.
	if(pclitem->CanRepair(pclItemManager) == false)return false;

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
	if ( ref <= 0 ) return false ;
	if ( pclItemManager->pclItemInfo[ref]->siSetItemIndex > 0 )
	{
		*preturnval = REPAIR_ERR_NOTSAMEITEM ;
		return false ;
	}

	//--------------------------------------------
	// 수리 정도를 확인한다. 
	//--------------------------------------------
	SI32 decdur = MIN_DUR_FOR_REPAIR;

	// 아이템 생산에 필요한 스킬을 확보한다. 
	SI32 skillunique = pclitem->GetMakeSkill(pclItemManager);
	if(skillunique)
	{
#ifdef _SAFE_MEMORY
		SI32 skilllevel		= pclCM->CR[mastercharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclCM->CR[mastercharunique]->pclCI->clCharItem.clItem[0]);
#else
		SI32 skilllevel		= pclCM->CR[mastercharunique]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclCM->CR[mastercharunique]->pclCI->clCharItem.clItem);
#endif
		// 기술 수준에 따라 소모되는 내구도를 구한다. 
		decdur = GetWasteDurability(skilllevel);
	}


	cltItem clitem;
	clitem.Set(pclitem);

	if(clitem.Repair(pclItemManager, decdur) == true)
	{
		//--------------------------------------------
		// 수리 하도록 DB매니저에게 요청한다. 
		//--------------------------------------------
		SI32 personid		= pclCM->CR[clientcharunique]->pclCI->GetPersonID();
		SI32 repairpersonid = pclCM->CR[mastercharunique]->pclCI->GetPersonID();

		sDBRequest_InvRepair clMsg(clientcharunique, personid, clientitempos, &clitem, repairpersonid, repairmoney, 0);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	}



	return true;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_INVREPAIR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_InvRepair* pclinfo = (cltGameMsgRequest_InvRepair*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)return ;

	// 내구도 정보 저장. 
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->SendDBWasteDurability();
	pclchar->SendDBSummonWasteDurability();

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	SI32 itempos = pclinfo->siItemPos;
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;

	cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ itempos ];
	SI32 rtnval = 0;
	if(Repair(id, id, itempos, pclitem, 0, &rtnval) == false)
	{
		// 수리 의뢰된 아이템과 실제 아이템이 다르다면, 
		if(rtnval == REPAIR_ERR_NOTSAMEITEM)
		{
			// 실제로 그 위치에 있는 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_UPGRADEITEM(cltMsg* pclMsg, SI32 id)
{
	return;
}


void cltServer::DoMsg_GAMEMSG_REQUEST_MAKEITEMINVINDEX(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_MakeItem_SelectedIndex* pclinfo = (cltGameMsgRequest_MakeItem_SelectedIndex*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;
	if( pclinfo->siMoveInvIndex < 0 || pclinfo->siMoveInvIndex >= 29 )	return;

	pclCM->CR[id]->siInvMakeItemIndex[pclinfo->siMoveInvIndex] = pclinfo->siInvSelectedValue ;

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_PERFORMANCEUPGRADEITEM(cltMsg* pclMsg, SI32 id)
{
	//if ( pclCM->IsValidID(id) == false )
	//{
	//	return;
	//}
	//
	//cltGameMsgRequest_PerformanceUpgradeItem *pclinfo = (cltGameMsgRequest_PerformanceUpgradeItem*)pclMsg->cData;

	//SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	//if ( FromPersonID < 1 )
	//{
	//	return;
	//}

	//// 업그레이드할 아이템 위치 확인
	//if ( pclinfo->siItemPos < PERSONITEM_INV0 || pclinfo->siItemPos >= MAX_ITEM_PER_PERSON )
	//{
	//	return;
	//}

	///*
	//if( ( pclinfo->siVillageUnique < 1 || pclinfo->siVillageUnique > CUR_VILLAGE_LIMIT ) &&
	//	pclinfo->siVillageUnique != MAPINDEX_PALACE )
	//	return;
	//*/

	//// 아이템이 존재하는지 확인
	//if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique < 1 )
	//{
	//	return;
	//}

	//// 아이템 일치 확인
	//if ( ! pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].IsSame( &pclinfo->clItem, true ) )
	//{
	//	return;
	//}

	//// 중첩 아이템 안됨
	//if ( pclinfo->clItem.CanPile( pclItemManager ) )
	//{
	//	return;
	//}

	//cltItem clDestItem;
	//clDestItem.Init();

	//SI32 ScrollUnique, ScrollNum;
	//SI32 returnval = 0;
	//if( ! pclItemManager->GetScrollForTune2( pclinfo->clItem, &ScrollUnique, &ScrollNum, &clDestItem, &returnval ) )
	//{
	//	return;
	//}

	//if ( ScrollUnique < 1 || ScrollNum < 1 )
	//{
	//	return;
	//}

	////---------------------------------------------
	//// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	////---------------------------------------------
	//cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	//pclChar->ItemUseReserveAction();

	////---------------------------------------------
	//// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	////---------------------------------------------
	//pclChar->BulletUseReserveAction();

	//// 부적이 있는지 확인
	//SI32 ScrollPos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( ScrollUnique );
	//if ( ScrollPos < 0 )
	//{
	//	return;
	//}

	//if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ ScrollPos ].GetItemNum() < ScrollNum )
	//{
	//	return;
	//}

	//// 혹부리 영감 이용권 확인
	//SI16 HokburiItemPos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( ITEMUNIQUE(24005) );
	//if ( HokburiItemPos < 0 )
	//{
	//	return;
	//}

	//if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ HokburiItemPos ].GetItemNum() < 1 )
	//{
	//	return;
	//}

	//cltItem clScrollItem;
	//clScrollItem.Init();
	//clScrollItem.siUnique = ScrollUnique;
	//clScrollItem.siItemNum = ScrollNum;

	//sDBRequest_PerformanceUpgradeItem clMsg( id, FromPersonID, pclinfo->siItemPos, &pclinfo->clItem, &clDestItem, ScrollPos, &clScrollItem );
	//pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	//// 물품 사용 통계를 낸다.(부적)
	//pclItemManager->AddUseNum( ScrollUnique, ScrollNum );   
	//// 혹부리 영감 이용권 사용
	//cltItem clHokburiItem;
	//clHokburiItem.Init();

	//clHokburiItem.siUnique = ITEMUNIQUE(24005);
	//clHokburiItem.siItemNum = 1;

	//sDBRequest_UseItem clMsg1( id, FromPersonID, HokburiItemPos, &clHokburiItem ,USEITEM_REASON_TICKET);
	//pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg1);

	//// 물품 사용 통계를 낸다.(혹부리영감이용권)
	//pclItemManager->AddUseNum( ITEMUNIQUE(24005), 1 );

	//// 부적 이용 개조 건수 통계. 
	//pclStatisticsManager->clDailyStatistics.siScrollTuneNumber+= ScrollNum;

	return;
}

// 아이템을 바닥에 떨어뜨린다. 
void cltServer::DropItem(SI32 mapindex, SI32 x, SI32 y, cltItem* pclitem, SI32 droppersonid)
{
	//------------------------------------------
	// 아이템을 아이템리스트에 추가한다. 
	//------------------------------------------
	UI16 index = pclItemList->PushItem(mapindex, x, y, pclitem, droppersonid );
	if(index)
	{
		pclMapManager->m_world.GetMap(mapindex)->AddItem(x, y, index);
	}

	if( mapindex < 0 || mapindex >= MAX_MAP_NUMBER )		return;
	//------------------------------------------
	// 지도에 아이템을 떨군다. 
	//------------------------------------------
	pclMapManager->pclMapArray[mapindex]->SetItemMap(x, y, pclitem->siUnique, index);
}
/*
void cltServer::DoMsg_GAMEMSG_REQUEST_MONSTERCORP_START(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if (pclchar == NULL ) 
		return ;

	pclchar->dwStartMiniGame = CurrentClock;
}*/

void cltServer::DoMsg_GAMEMSG_REQUEST_MONSTERCORP_SCORE(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false )
	{
		return;
	}

	cltGameMsgRequest_MonsterCorp_Score* pclinfo = (cltGameMsgRequest_MonsterCorp_Score*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if (pclchar == NULL ) 
		return ;

	if ( pclchar->bMakeItemNoMinigame || pclchar->bMakeItemSwitch )
	{
		pclchar->siMonsterCorp = 0 ;
		pclchar->dwStartMiniGameTime = 0;
		return ;
	}
    
	if( pclchar->siMonsterCorp == pclinfo->siScore )
	{
		SI32 score = min(10000,max(-10000,pclinfo->siScore));

		// 중국은 미니게임 점수 로그를 남긴다.
		if( ConstServiceArea_China == siServiceArea )
		{
			if( 1000 <= score )
			{
				DWORD totalTime = ( CurrentClock - pclchar->dwStartMiniGameTime ) / 1000;
				pclLog->FileConfigPrint( "MiniGameLog", "Name : [%s], Score : [%d] Time : [%d]", (TCHAR*)pclchar->GetName(), pclinfo->siScore, totalTime );
			}
		}

		pclchar->bMakeItemMinigameSwitch = true ;
		pclchar->siMonsterCorp = score ;
		pclchar->dwStartMiniGameTime = 0;
	}
	else
	{
		pclchar->bMakeItemMinigameSwitch = true ;
		pclchar->siMonsterCorp = 0 ;
		pclchar->dwStartMiniGameTime = 0;
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MONSTERCORP_COMBOINFO(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false )
	{
		return;
	}

	cltGameMsgRequest_MonsterCorp_ComboInfo* pclinfo = (cltGameMsgRequest_MonsterCorp_ComboInfo*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if (pclchar == NULL ) 
		return ;

	if( pclinfo->GetScore() == SCORE ||
		pclinfo->GetScore() == PENALTYSCORE )
	{
		pclchar->siMonsterCorp += pclinfo->GetScore() * pclinfo->GetComboInfo();
	}
}

// 박대박 게임
void cltServer::DoMsg_GAMEMSG_REQUEST_ROULETTEGAME_INFO(cltMsg* pclMsg,SI32 id)
{
	// 중지중이면 리턴
	if( bRouletteGame )
		return;

	if ( pclCM->IsValidID(id) == false )
	{
		return;
	}

	cltGameMsgRequest_RouletteGame_Info* pclinfo = (cltGameMsgRequest_RouletteGame_Info*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if (pclchar == NULL ) return ;

	for(SI32 y = 0;y<5;++y)
	{
		for (SI32 x = 0;x<5;++x) 
		{
			if( pclinfo->m_stRoulette[y][x].m_bCpuSelect == true || pclinfo->m_stRoulette[y][x].m_bResult == true)
				return;

			pclchar->m_stRouletteTable[y][x].Set(pclinfo->m_stRoulette[y][x].m_bUserSelect,
				pclinfo->m_stRoulette[y][x].m_bCpuSelect,
				pclinfo->m_stRoulette[y][x].m_bResult,
				pclinfo->m_stRoulette[y][x].m_siX,
				pclinfo->m_stRoulette[y][x].m_siY,
				pclinfo->m_stRoulette[y][x].m_siValue);	

			if( pclchar->m_stRouletteTable[y][x].m_bUserSelect == true )
				pclchar->m_siBettingCount++;
		}
	}

	pclchar->m_siRandomValue[0] = rand()%25 + 1;

	do
	{
		pclchar->m_siRandomValue[1] = rand()%25 + 1;
	}while( (pclchar->m_siRandomValue[0] != pclchar->m_siRandomValue[1]) == false);

	do
	{
		pclchar->m_siRandomValue[2] = rand()%25 + 1;
	}
	while( (pclchar->m_siRandomValue[0] != pclchar->m_siRandomValue[2] && pclchar->m_siRandomValue[1] != pclchar->m_siRandomValue[2]) == false);

	do
	{
		pclchar->m_siRandomValue[3] = rand()%25 + 1;
	}
	while( (pclchar->m_siRandomValue[0] != pclchar->m_siRandomValue[3] && pclchar->m_siRandomValue[1] != pclchar->m_siRandomValue[3] && pclchar->m_siRandomValue[2] != pclchar->m_siRandomValue[3]) == false);

	do
	{
		pclchar->m_siRandomValue[4] = rand()%25 + 1;
	}
	while( (pclchar->m_siRandomValue[0] != pclchar->m_siRandomValue[4] && pclchar->m_siRandomValue[1] != pclchar->m_siRandomValue[4] && pclchar->m_siRandomValue[2] != pclchar->m_siRandomValue[4] && pclchar->m_siRandomValue[3] != pclchar->m_siRandomValue[4]) == false);

	for(SI32 y = 0;y<5;++y)
	{
		for (SI32 x = 0;x<5;++x) 
		{
			if( pclchar->m_stRouletteTable[y][x].m_siValue  == pclchar->m_siRandomValue[0] )
				pclchar->m_stRouletteTable[y][x].m_bCpuSelect = true;
			else if( pclchar->m_stRouletteTable[y][x].m_siValue  == pclchar->m_siRandomValue[1] )
				pclchar->m_stRouletteTable[y][x].m_bCpuSelect = true;
			else if( pclchar->m_stRouletteTable[y][x].m_siValue  == pclchar->m_siRandomValue[2] )
				pclchar->m_stRouletteTable[y][x].m_bCpuSelect = true;
			else if( pclchar->m_stRouletteTable[y][x].m_siValue  == pclchar->m_siRandomValue[3] )
				pclchar->m_stRouletteTable[y][x].m_bCpuSelect = true;
			else if( pclchar->m_stRouletteTable[y][x].m_siValue  == pclchar->m_siRandomValue[4] )
				pclchar->m_stRouletteTable[y][x].m_bCpuSelect = true;
		}
	}

	pclchar->SetRoulette(true);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_ROULETTEGAME_STOP(cltMsg* pclMsg,SI32 id)
{
	// 중지중이면 리턴
	if( bRouletteGame )
		return;

	if ( pclCM->IsValidID(id) == false )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if (pclchar == NULL ) return ;

	pclchar->SetRoulette(false);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_INV_CANCELLATIONSEALINGUP(cltMsg* pclMsg, SI32 id)
{
 	cltGameMsgRequest_Inv_CancellationSealingUp* pclinfo = (cltGameMsgRequest_Inv_CancellationSealingUp*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;
	pclchar->SendDBWasteDurability();
	pclchar->SendDBSummonWasteDurability();

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	SI32 itempos = pclinfo->siItemPos;
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 밀봉해제가 안된다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == itempos	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTSEALINGUP_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}

		// 귀속전용 개인상점
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == itempos	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}


	cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ itempos ];

	if( NULL == pclitem )
		return;

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
	if( ref <= 0)
		return;

	SI32 rtnval = 0;
	if(pclitem->IsSealingUp(pclItemManager) )
	{
		pclitem->uiDateYear = sTime.wYear - 2000 ;
		pclitem->uiDateMonth = sTime.wMonth ;
		pclitem->uiDateDay = sTime.wDay ;

		//cyj 목걸이는 밀봉 해제할때 성능이 정해짐..
		SI32 type = pclItemManager->GetItemType(pclitem->siUnique);
		if (type == ITEMTYPE_NECK)
		{
			if ( pclitem->siUnique == ITEMUNIQUE(1200))		// 추천인 목걸이
			{
				SI32 skillup = (rand() % (TABS(pclitem->clItemNeck.uiMaxSkillPoint - pclitem->clItemNeck.uiSkillPointUp) + 1)) + pclitem->clItemNeck.uiSkillPointUp;
				pclitem->clItemNeck.uiSkillPointUp	= max(pclitem->clItemNeck.uiSkillPointUp, skillup);

				SI32 usedate = (rand() % (TABS(pclitem->clItemNeck.uiMaxUsedate - pclitem->uiDateUseDay) + 1)) + pclitem->uiDateUseDay;

				pclitem->uiDateUseDay = max(pclitem->uiDateUseDay, usedate);

			}
			else
			{
				pclitem->clItemNeck.uiMaxSkillPoint = 5;

				SI32 siRand = rand() % 100 + 1;
				SI32 skillup = 0;
				SI32 useDate = 0;
				if ( siRand <= 20 )	
				{
					skillup = 1;
					useDate = 14;
				}
				else if ( siRand <= 60 ) 
				{
					skillup = 2;
					useDate = 12;
				}
				else if ( siRand <= 85 ) 
				{
					skillup = 3;
					useDate = 10;
				}
				else if ( siRand <= 95 ) 
				{
					skillup = 4;
					useDate = 8;
				}
				else	
				{
					skillup = 5;
					useDate = 7;
				}

				pclitem->clItemNeck.uiSkillPointUp	= skillup;
				if( true == pclItemManager->pclItemInfo[ref]->IsImmutableUseDate() )
					pclitem->uiDateUseDay = pclitem->uiDateUseDay;
				else
					if( ITEMUNIQUE(1190) == pclitem->siUnique ) // 뱀파이어 목걸이만 날짜는 리소스의 날짜로 한다.
					{

					}
					else
					{
						pclitem->uiDateUseDay = useDate + ( rand() % 8 );
					}
				if( skillup >= 4 )
					pclitem->clItemNeck.clCommonInfo.uiRareInfo = RARE2_ITEM_RATE;
			}
		}

		{
			// DB에 저장한다. 일단 내구도 메시지로 수정한다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

		}

		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

// 연금술사로부터 이이템의 능력을 변경한다.
void cltServer:: DoMsg_GAMEMSG_REQUEST_INV_ChangItemFromNPC(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_Inv_ChangItemFormNpc* pclinfo = (cltGameMsgRequest_Inv_ChangItemFormNpc*)pclMsg->cData;

	if( pclCM->IsValidID(id) == false )return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;
	
	// 무효한 범위의 아이템 위치가 오면 리턴. 
	SI32 itempos = pclinfo->siItemPos;
	if(itempos < 0 || itempos >= MAX_ITEM_PER_PERSON)return ;

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 스탯을 변경할수 없다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == itempos	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTSEALINGUP_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}
		// 귀속전용 개인상점
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == itempos	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}
	cltItem* pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ itempos ];

	if( NULL == pclitem )
		return;
	// 손재주가 아닌 스탯을 변경할시에는 안된다.
	if (pclitem->clItemStatusArmour.uiStr > 0 ||
	pclitem->clItemStatusArmour.uiDex > 0 ||
	pclitem->clItemStatusArmour.uiLuk > 0 ||
	pclitem->clItemStatusArmour.uiMag > 0 ||
	pclitem->clItemStatusArmour.uiVit > 0 
	)
	{
		return;
	}
	// 가차아이템이 아니면 막는다.
	if (pclClient->pclGachaManager->IsGachaRareItem(pclitem->siUnique ) == false) {
		return ;
	}
	if( pclitem->GetType(pclItemManager) != ITEMTYPE_DRESS && pclitem->GetType(pclItemManager) !=ITEMTYPE_HAT ) return;

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
	if( ref <= 0)
		return;
	if (pclinfo->uiStatusLater == ChangeStrStatus) {// 스텟을 힘으로 변경할경우
		pclitem->clItemStatusArmour.uiStr = pclinfo->uiStatusNow;
		pclitem->clItemStatusArmour.uiDex = 0;
		pclitem->clItemStatusArmour.uiHnd = 0;
		pclitem->clItemStatusArmour.uiLuk = 0;
		pclitem->clItemStatusArmour.uiMag = 0;
		pclitem->clItemStatusArmour.uiVit = 0;
		{
			// 디비로 해당 아이템정보를 날린다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	else if (pclinfo->uiStatusLater == ChangeDexStatus) {// 스텟을 순발으로 변경할경우
		pclitem->clItemStatusArmour.uiDex = pclinfo->uiStatusNow;
		pclitem->clItemStatusArmour.uiHnd = 0;
		pclitem->clItemStatusArmour.uiLuk = 0;
		pclitem->clItemStatusArmour.uiMag = 0;
		pclitem->clItemStatusArmour.uiStr = 0;
		pclitem->clItemStatusArmour.uiVit = 0;
		{
			// 디비로 해당 아이템정보를 날린다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	else if (pclinfo->uiStatusLater == ChangeVitStatus) {// 스텟을 생명력으로 변경할경우
		pclitem->clItemStatusArmour.uiVit = pclinfo->uiStatusNow;
		pclitem->clItemStatusArmour.uiDex = 0;
		pclitem->clItemStatusArmour.uiHnd = 0;
		pclitem->clItemStatusArmour.uiLuk = 0;
		pclitem->clItemStatusArmour.uiMag = 0;
		pclitem->clItemStatusArmour.uiStr = 0;
		{
			// 디비로 해당 아이템정보를 날린다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	else if (pclinfo->uiStatusLater == ChangeHndStatus) {// 스텟을 손재주으로 변경할경우
		pclitem->clItemStatusArmour.uiHnd = pclinfo->uiStatusNow;
		pclitem->clItemStatusArmour.uiDex = 0;
		pclitem->clItemStatusArmour.uiLuk = 0;
		pclitem->clItemStatusArmour.uiMag = 0;
		pclitem->clItemStatusArmour.uiStr = 0;
		pclitem->clItemStatusArmour.uiVit = 0;
		{
			// 디비로 해당 아이템정보를 날린다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	else if (pclinfo->uiStatusLater == ChangeLukStatus) {// 스텟을 행운으로 변경할경우
		pclitem->clItemStatusArmour.uiLuk = pclinfo->uiStatusNow;
		pclitem->clItemStatusArmour.uiDex = 0;
		pclitem->clItemStatusArmour.uiHnd = 0;
		pclitem->clItemStatusArmour.uiMag = 0;
		pclitem->clItemStatusArmour.uiStr = 0;
		pclitem->clItemStatusArmour.uiVit = 0;
		{
			// 디비로 해당 아이템정보를 날린다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	else if (pclinfo->uiStatusLater == ChangeMagStatus) {// 스텟을 마법력으로 변경할경우
		pclitem->clItemStatusArmour.uiMag = pclinfo->uiStatusNow;
		pclitem->clItemStatusArmour.uiDex = 0;
		pclitem->clItemStatusArmour.uiHnd = 0;
		pclitem->clItemStatusArmour.uiLuk = 0;
		pclitem->clItemStatusArmour.uiStr = 0;
		pclitem->clItemStatusArmour.uiVit = 0;
		{
			// 디비로 해당 아이템정보를 날린다.
			sDBRequest_WasteDurability clMsg(id, pclchar->pclCI->GetPersonID(), itempos, pclitem);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		{
			// 아이템의 정보를 보내서 클라이언트를 업데이트 시킨다. 
			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	// 연금술사 아이템 변경 성공 메세지를 클라이언트에 보낸다.
	SendServerResponseMsg( 0, SRVAL_CHANGE_ITEM_FROM_NPC, 0, 0, id ); 

}

BOOL cltServer::GetItemUseAbleDay(cltItem* clItem, SI32* outUseAbleDay) //[영진] 아이템 남은 사용기간은 outUseAbleDay 변수에 담아 리턴해준다
{
	*outUseAbleDay = 0; // 0값으로 초기화

	if(clItem == NULL) return FALSE; // 아이템 포인터가 잘못되었다면 에러
	SI32 itemunique = clItem->siUnique ;
	if(itemunique <= 0)return FALSE; // 아이템 유니크가 잘못되었다면 에러


	if(clItem->IsSealingUp(pclItemManager) == TRUE)
	{
		*outUseAbleDay = clItem->uiDateUseDay;	// 밀봉된 아이템이라면 사용기간 반환
		return TRUE;
	}


	if(clItem->IsHaveUseDay() == TRUE)  // 밀봉 풀린 기간제 아이템이라면 사용기간 계산해서 반환
	{

		NDate ndate;
		ndate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
		SI32 todaydatevary = ndate.GetDateVary() ;

		ndate.SetDate(clItem->uiDateYear+2000,clItem->uiDateMonth,clItem->uiDateDay);
		SI32 datevary = ndate.GetDateVary() + clItem->uiDateUseDay ;

		// 아이템 사용 가능한 기한 - 오늘. 
		// 양수면 사용 가능일,  0 이면 마지막 사용일, 음수면 사라져야 할 아이템
		*outUseAbleDay =  datevary - todaydatevary;

		//KHY - 0723 - 기간아이템 파괴 방지. 혹시 몰라서.
		if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
		{
			if(*outUseAbleDay < 0)
			{
				pclClient->sUpViewEffect = 1;				
			}

			if(*outUseAbleDay < (-10000))
				*outUseAbleDay = -10000;
		}

		return TRUE;
	}
	else if(clItem->uiDateUseDay == 0 ) // 날짜가 없는 일반 아이템이다.
		return TRUE;		

	return FALSE ; //  Error 루틴. 

}

BOOL cltServer::DistributePileMoveItem(SI32 charid, cltItem clFrom, UI16 uiFrom, cltItem clTo, UI16 uiTo)
{
	SI32 max_pile_number = MAX_ITEM_PILE_NUMBER;
	SI32 total_plie_number;
	BOOL bBulletMove = FALSE;

	// 장비창으로 화살이나 탄환 넣을때는 중첩갯수를 DEFAULT_EQUIP_BULLET_NUMBER로
	if ((uiTo == PERSONITEM_WEAPON1 ||  uiTo == PERSONITEM_WEAPON2) && 
		clFrom.IsBulletItem())
	{
		if (pclCM->IsValidID(charid) && pclCM->CR[charid]->pclCI->clBI.bBulletService)
		{
			max_pile_number = UPGRADE_EQUIP_BULLET_NUMBER;
		}
		else
		{
			max_pile_number = DEFAULT_EQUIP_BULLET_NUMBER;
		}

		bBulletMove = TRUE;

		/*
		// 화살이나 탄환아이템 장착할때 인벤토리가 빈칸이면 아이템 분배를 위해 siUnique를 같게 해줌
		if (bBulletMove)
		{
		if (clTo.siUnique == 0)
		{
		clTo.siUnique = clFrom.siUnique;
		clTo.siItemNum = 0;
		}
		}
		*/
	}

	// 양쪽다 가능하거나 총알 아이템일 경우
	if ( (clFrom.CanPile(pclClient->pclItemManager) == true) && 
		((bBulletMove == TRUE) || (clTo.CanPile(pclClient->pclItemManager) == true))
	)
		
	{
		if (clFrom.siUnique == clTo.siUnique)
		{
			total_plie_number = clFrom.siItemNum + clTo.siItemNum;
		}
		else
			total_plie_number = clFrom.siItemNum;

		//동일한 물품끼리 옮길때 나눠줌
		if ((uiFrom != uiTo && (clFrom.siUnique == clTo.siUnique || clTo.siUnique == 0 || bBulletMove) && total_plie_number > max_pile_number))
		{
			cltItem tmpFromItem, tmpToItem;
			//SI16 sitopos;
			//SI32 rtnvalue;

			tmpFromItem.Set(&clFrom);
			tmpFromItem.siItemNum = max_pile_number - clTo.siItemNum;

			if ( pclCM->IsValidID(charid) )
			{
				sDBRequest_MoveItem clMsg(charid, pclCM->CR[charid]->pclCI->GetPersonID(), uiFrom, &tmpFromItem, tmpFromItem.CanPile(pclItemManager),
					uiTo, &clTo);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}

			return TRUE;
		}
	}

	return FALSE;
}



void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANTITEMINVINDEX(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_EnchantItem_SelectedIndex* pclinfo = (cltGameMsgRequest_EnchantItem_SelectedIndex*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	pclCM->CR[id]->siOrgInvEnchantInx = pclinfo->siOrgInvEnchantInx;
	pclCM->CR[id]->siMatInvEnchantInx = pclinfo->siMatInvEnchantInx;	

	for ( SI16 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++ )
	{
		pclCM->CR[id]->siElementEnchantInx[siIndex] = pclinfo->siElementEnchantInx[siIndex];
	}
	pclCM->CR[id]->siElementType = pclinfo->siElementType;


	return;

}

void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANTITEMSWITCH(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Enchant_Switch* pclinfo = (cltGameMsgRequest_Enchant_Switch*)pclMsg->cData;

	if(pclCM->IsValidID(id) == false)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;


	pclCM->CR[id]->bPersonalEnchantItemSwitch = pclinfo->bEnchantSwitch;

	return;

}


void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANTITEMSTARTOREND(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Enchant_StartOrEnd clinfo(bEnchantSwitch);
	cltMsg clMsg(GAMEMSG_RESPONSE_ENCHANTITEMSTARTOREND, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}


void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANTOTHERPRICE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_EnchantPrice* pclinfo = (cltGameMsgResponse_EnchantPrice*)pclMsg->cData;


	if(pclinfo->siUnique <=0 ) return;
	cltItemPriceUnit *CurrentPrice = pclItemManager->GetCurrentPrice(pclinfo->siUnique);
	SI64 sifee = CurrentPrice->siPrice;


	cltGameMsgRequest_EnchantPrice clinfo(sifee);
	cltMsg clMsg(GAMEMSG_RESPONSE_ENCHANTOTHERPRICE, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);   

}
void cltServer::DoMsg_GAMEMSG_REQUEST_ENCHANTOTHERSHOW(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_EnchantOther* pclinfo = (cltGameMsgRequest_EnchantOther*)pclMsg->cData;

	SI32 otherid = pclinfo->m_siOtherCharUnique;
	if(pclCM->IsValidID(otherid) == false)		return;

	cltItem clOrgItem;
	cltItem clMatItem;
	cltItem clEleItem[NOW_ENCHANT_NUMBER];

	pclCM->CR[otherid]->siOrgInvEnchantInx  = pclinfo->siOrgIndex;
	pclCM->CR[otherid]->siMatInvEnchantInx = pclinfo->siMatIndex;


	
	for (SI16 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
	{
		pclCM->CR[otherid]->siElementEnchantInx[siIndex]= pclinfo->siEleIndex[siIndex];
		clEleItem[siIndex].Init();

		if (pclinfo->siEleIndex[siIndex] >= 0 && pclinfo->siEleIndex[siIndex] < MAX_ITEM_PER_PERSON)
			clEleItem[siIndex].Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siEleIndex[siIndex]]);
	}


	if(pclinfo->siOrgIndex >= 0 && pclinfo->siOrgIndex < MAX_ITEM_PER_PERSON)
		clOrgItem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siOrgIndex]);

	if(pclinfo->siMatIndex >= 0 && pclinfo->siMatIndex < MAX_ITEM_PER_PERSON)
		clMatItem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siMatIndex]);


	//업데이트와 동시에 물건 이동이 안된다.
	pclCM->CR[id]->bPersonalEnchantItemSwitch = false;

#ifdef _SAFE_MEMORY
	cltGameMsgResponse_EnchantOther clinfo(&clOrgItem, &clMatItem, clEleItem, pclinfo->siOrgIndex,pclinfo->siMatIndex , &pclinfo->siEleIndex[0], true, false);
#else
	cltGameMsgResponse_EnchantOther clinfo(&clOrgItem, &clMatItem, clEleItem, pclinfo->siOrgIndex,pclinfo->siMatIndex , pclinfo->siEleIndex, true, false);
#endif
	cltMsg clMsg(GAMEMSG_RESPONSE_ENCHANTOTHERSHOW, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[otherid]->SendNetMsg((sPacketHeader*)&clMsg);


#ifdef _SAFE_MEMORY
	cltGameMsgResponse_EnchantOther clinfo1(&clOrgItem, &clMatItem, clEleItem, pclinfo->siOrgIndex,pclinfo->siMatIndex , &pclinfo->siEleIndex[0], false, false);
#else
	cltGameMsgResponse_EnchantOther clinfo1(&clOrgItem, &clMatItem, clEleItem, pclinfo->siOrgIndex,pclinfo->siMatIndex , pclinfo->siEleIndex, false, false);
#endif
	cltMsg clMsg1(GAMEMSG_RESPONSE_ENCHANTOTHERSHOW, sizeof(clinfo1), (BYTE*)&clinfo1);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);


	return;
}
void cltServer::DoMsg_GAMEMSG_REQUEST_ITEMSPLIT(cltMsg* pclMsg, SI32 id)
{
	// 스플릿!!
	if( pclCM->CR[id]->siPersonalMakeItemType > 0) return; //제조시 아이템 이동 안된다.
	if( pclCM->CR[id]->bPersonalEnchantItemSwitch == true) return; 
	if(pclCM->CR[id]->pclCI->GetPersonID() <= 0 ) return; // 어찌되었든 잘못된 거다.

	cltSplitItemInfo* pclinfo = (cltSplitItemInfo*)pclMsg->cData;


	cltItem* pclFrom = NULL;
	cltItem  clTo;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();


	SI32 slot = -1;

	// 부적절한 데이터를 확인한다. 
	if(pclinfo->uiTo < 0	|| pclinfo->uiTo >= MAX_ITEM_PER_PERSON)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMSPLIT - PersonID = %d, IP =  %s , uiTo = %d"),	
																	pclCM->CR[id]->pclCI->GetPersonID(),
																	pclCM->CR[id]->GetSession()->GetIP(),
																	pclinfo->uiTo
																	);
		return ;
	}	
	if(pclinfo->uiFrom < 0	|| pclinfo->uiFrom >= MAX_ITEM_PER_PERSON)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMSPLIT - PersonID = %d, IP =  %s , uiFrom = %d"),	
																	pclCM->CR[id]->pclCI->GetPersonID(),
																	pclCM->CR[id]->GetSession()->GetIP(),
																	pclinfo->uiFrom
																	);
		return ;
	}	

	if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->uiTo].siUnique > 0 )
	{
		// 보내려는 위치에 아이템이 존재한다면 혹시 몰라서..리턴
		return;
	}
	if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->uiFrom].siItemNum < pclinfo->siCount )
	{
		// 나누려는 아이템 수량이 실제 수량보다 많으면..(그럴리는 없겠지만..)
		return;
	}

	//--------------------------------------------
	//From의 아이템을 구한다.
	//--------------------------------------------
	pclFrom		= &pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->uiFrom];			

	// 아이템이 무효한 것이라면..
	if(pclFrom == NULL || pclFrom->siUnique <=0 ||pclFrom->siItemNum <= 0)
	{
		// 악의적 클라이언트 일 수 있으니, 로그를 찍어 두자.		
		if(pclFrom == NULL )
		{
			pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMSPLIT - PersonID = %d, IP =  %s , pclFrom = NULL"),	
																		pclCM->CR[id]->pclCI->GetPersonID(),
																		pclCM->CR[id]->GetSession()->GetIP()
																		);
		}
		else
		{
			pclLog->FilePrint(TEXT("Config\\Server-ItemMoveErr.log"), TEXT("DoMsg_GAMEMSG_REQUEST_ITEMSPLIT - PersonID = %d, IP =  %s , siUnique = %d,  siItemNum = %d"),	
																		pclCM->CR[id]->pclCI->GetPersonID(),
																		pclCM->CR[id]->GetSession()->GetIP(),
																		pclFrom->siUnique ,
																		pclFrom->siItemNum											
																		);
		}
		return ;
	}

	// 트레이드중인 것은 안된다!
	if ( ((cltCharServer*)pclCM->CR[id])->GetNoMoveItemSwitch(pclinfo->uiFrom) != 0 )
	{
		return ;
	}

	cltItem clFrom;
	clFrom.Set(pclFrom);
	// 정해진 갯수만큼만!!
	clFrom.siItemNum = pclinfo->siCount;

	//-----------------------------------------------
	// To 에 넣는다. 
	//-----------------------------------------------
	clTo.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->uiTo]);

	if (DistributePileMoveItem(id, clFrom, pclinfo->uiFrom, clTo, pclinfo->uiTo) == FALSE)		
	{
		bool Canpile = false;
		if(( pclFrom->CanPile(pclItemManager))&&( clTo.CanPile(pclItemManager)))
			Canpile = true;
		
		sDBRequest_MoveItem clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->uiFrom, &clFrom,Canpile,
			pclinfo->uiTo, &clTo);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
	}
}


//[추가 : 황진성 2007. 12. 11 => 신년 이벤튼 버튼를 클라이언트에서 선택 했음. C->S]
void cltServer::DoMsg_GAMEMSG_REQUEST_NEWYEAR_CLICK(cltMsg* pclMsg, SI32 id )
{
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	cltItem clitem;
	ZeroMemory(&clitem, sizeof(clitem));
	clitem.siUnique  = 13316;
	clitem.siItemNum =	  1;
	
	if(siServiceArea == ConstServiceArea_Taiwan)
	{
		clitem.siItemNum = 3;
	}

	InputItem(pclchar, &clitem, INPUTITEMPERSON_OPTION_EVENT);
}

//[추가 : 황진성 2008. 1. 29 =>  공간 상자 리스트 얻기 메시지 처리 함수. C->S]
void  cltServer::DoMsg_GAMEMSG_REQUEST_SPACE_BOX_LIST(cltMsg* pclMsg,SI32 id)
{
	cltMsg clMsg(GAMEMSG_REQUEST_SPACE_BOX_LIST, 0);
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

//[추가 : 황진성 2008. 1. 29 =>  공간 상자(보물) 메시지 처리 함수. 사용 안함.삭제하자.]
void  cltServer::DoMsg_GAMEMSG_REQUEST_TRESURE_BOX_LIST(cltMsg* pclMsg,SI32 id)
{
	cltMsg clMsg(GAMEMSG_REQUEST_TRESURE_BOX_LIST, 0);
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

}

//[추가 : 황진성 2008. 1. 30 => 공간 상자 저장. C->S]
void cltServer::DoMsg_GAMEMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Keep_Box_Move_Item* pclinfo = (cltGameMsgRequest_Keep_Box_Move_Item*)pclMsg->cData;
	
	// 티켓이 사용되지 않으면 또는 시간이 없다면.
	if( ( false == pclCM->CR[id]->pclCI->clBI.bSpaceBoxPremiumService || 
		  0     == pclCM->CR[id]->pclCI->clBI.siSpaceBoxPremiumServiceDate ) &&		
		  false == pclinfo->m_bGacha)
	{
		SendServerResponseMsg(0, SRVAL_SPACE_BOX_TIME_OVER,  0, 0, pclCM->CR[id]->GetCharUnique()); 
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if( NULL == pclchar )			return;
	if( pclchar->GetLife() <= 0)	return;
 
	if ( pclchar->bPremiumPersonalShop == true ) 
	{
		return	;
	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. (처리하지 않으면 사용한 아이템이 다시 복구될 수 있다.) 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	// 상점 아이템이 실제로 존재하는지 체크 
	if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->m_siPos )
	{
		if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
		{
			SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
			return ;
		}
	}

	//----------------------------------------------
	// 인벤에 실제로 해당 물품이 있는지 확인한다. 
	//----------------------------------------------
	SI32	 frompos	  = pclinfo->m_siPos;
	SI32	 MaxItemCount = 0;
	cltItem* pclitem	  = NULL;

	if( frompos < 0 || frompos >= MAX_ITEM_PER_PERSON )
		return;

	pclitem	= &pclCM->CR[id]->pclCI->clCharItem.clItem[frompos];

	// 파티홀 아이템은 저장할 수 없다.
	if( pclitem->clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
		return;

	// 같은 아이템인지 비교한다. 임의 개수를 지정하여 유니크로만 비교한다.
	cltItem clitem1;
	clitem1.Set(&pclinfo->m_KeepItem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(pclitem);
	clitem2.SetItemNum(1);

	if(clitem1.IsSame(&clitem2, false) == FALSE)
		return ;

	// 개수가 적절한지 확인한다. 
	if(pclinfo->m_KeepItem.GetItemNum() > pclitem->GetItemNum())
		return ;
	
	//기간제 아이템의 경우 남은 사용기간이 있는지 확인한다.
	SI32 itemunique = pclinfo->m_KeepItem.siUnique ;
	if ( itemunique > 0 )
	{
		if ( pclinfo->m_KeepItem.IsHaveUseDay() )  //[영진] 남은 사용기간 없음 알림
		{
			SI32 useableDay = 0;
			BOOL rVal = GetItemUseAbleDay(&pclinfo->m_KeepItem,&useableDay);
			if(rVal == FALSE) return;

			if( useableDay < 0) //사용기간이 지나서 지워준다 그리고 클라이언트에 알림.
			{
				//KHY - 0723 - 기간아이템 파괴 방지.
				if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
				{
					return;	
				}
				else
				{
					// 기간이 지난 고급품은 창고에 넣을 수 없다. - by LEEKH
					if( pclinfo->m_KeepItem.clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
					{
						// 메시지 출력
						return;
					}
					else
					{
						// 나머지 아이템은 지워준다.
						sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),frompos,&pclinfo->m_KeepItem,USEITEM_REASON_DATEOVER);
						pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);


						cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclinfo->m_KeepItem.siUnique, useableDay);
						cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );				
					}
					return;
				}
			}
		}
	}

	SI16		siBoxItemPos;
	cltItem     BoxItem;
	SI32		rtnvalue;

	cltItem*     pBoxitem = NULL;
	// 공간 상자인지 보물 상자인지 확인하여 최대 아이템을 담을 있는 수와 해당 포인터를 얻음.
	if( pclinfo->m_bGacha )
	{
		if(pclCM->CR[id]->pclTreasureBoxItem)	pBoxitem = &pclCM->CR[id]->pclTreasureBoxItem->clItem[0];
	}
	else
	{
		if(pclCM->CR[id]->pclSpaceBoxItem)		pBoxitem = &pclCM->CR[id]->pclSpaceBoxItem->clItem[0];
	}

	if(pBoxitem == NULL)		return;

	if(CanAddInv(pclinfo->m_bGacha, pBoxitem, pclClient->pclItemManager, INVMODE_ALL, &pclinfo->m_KeepItem, NULL, &siBoxItemPos, &BoxItem, &rtnvalue ) == false)
	{
		SendServerResponseMsg(0, SRVAL_SPACE_BOX_INVEN_FULL,  0, 0, pclCM->CR[id]->GetCharUnique());
		
		return;
	}  


	sDBRequest_Move_Item clMsg( DBMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM, 
								id, 
								pclCM->CR[id]->pclCI->GetPersonID(), 
								pclinfo->m_siPos, 
								&pclinfo->m_KeepItem, 
								siBoxItemPos, 
								&BoxItem, 
								pclinfo->m_bGacha, 
								pclinfo->m_KeepItem.CanPile( pclItemManager ) );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

//[추가 : 황진성 2008. 1. 30 => 공간 상자 회수. C->S]
void cltServer::DoMsg_GAMEMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_Recovery_Box_Move_Item* pclinfo = (cltGameMsgRequest_Recovery_Box_Move_Item*)pclMsg->cData;
	
	//----------------------------------------------
	// 인벤에 실제로 해당 물품이 있는지 확인한다. 
	//----------------------------------------------
	SI32	 frompos	  = pclinfo->m_siPos;
	SI32	 MaxItemCount = 0;
	cltItem* pclitem	  = NULL;

	if( frompos < 0 || frompos >= MAX_ITEM_PER_PERSON )		return;

	
	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if( NULL == pclchar )			return;
	if( pclchar->GetLife() <= 0)	return;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. (처리하지 않으면 사용한 아이템이 다시 복구될 수 있다.) 
	//---------------------------------------------
	pclchar->ItemUseReserveAction(); 

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	// 공간 상자인지 보물 상자인지 확인하여 최대 아이템을 담을 있는 수와 해당 포인터를 얻음.
	if( pclinfo->m_bGacha )
	{
		MaxItemCount = MAX_ITEM_TREASURE_BOX;

		if(pclCM->CR[id]->pclTreasureBoxItem)	pclitem	= &pclCM->CR[id]->pclTreasureBoxItem->clItem[frompos];
	}
	else
	{
		MaxItemCount = MAX_ITEM_SPACE_BOX;

		if(pclCM->CR[id]->pclSpaceBoxItem)		pclitem	= &pclCM->CR[id]->pclSpaceBoxItem->clItem[frompos];
	}
	if(pclitem == NULL)							return;	

	if(frompos < 0 || frompos >= MaxItemCount)	return;


	// 같은 아이템인지 비교한다. 임의 개수를 지정하여 유니크로만 비교한다.
	cltItem clitem1;
	clitem1.Set(&pclinfo->m_RecoveryItem);
	clitem1.SetItemNum(1);

	cltItem clitem2;
	clitem2.Set(pclitem);
	clitem2.SetItemNum(1);

	if(clitem1.IsSame(&clitem2, false) == FALSE)return ;

	// 개수가 적절한지 확인한다. 
	if(pclinfo->m_RecoveryItem.GetItemNum() > pclitem->GetItemNum())return ;

	//기간제 아이템의 경우 남은 사용기간이 있는지 확인한다.
	SI32 itemunique = pclinfo->m_RecoveryItem.siUnique ;
	if ( itemunique > 0 )
	{
		if ( pclinfo->m_RecoveryItem.IsHaveUseDay() )  //[영진] 남은 사용기간 없음 알림
		{
			SI32 useableDay = 0;
			BOOL rVal = GetItemUseAbleDay(&pclinfo->m_RecoveryItem,&useableDay);
			if(rVal == FALSE) return;

			if( useableDay < 0) //사용기간이 지나서 지워준다 그리고 클라이언트에 알림.
			{
				//KHY - 0723 - 기간아이템 파괴 방지.
				if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
				{
					return;	
				}
				else
				{
					// 기간이 지난 고급품은 공간상자에서 뺄수 있게한다.
					if( pclinfo->m_RecoveryItem.clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
					{
						//
						//return;
					}
					else
					{
						// 나머지 아이템은 지워준다.
						/*sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),frompos,&pclinfo->m_RecoveryItem,USEITEM_REASON_DATEOVER);
						pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);*/

						sDBRequest_Use_SpaceBox_Item useitem(id,pclCM->CR[id]->pclCI->GetPersonID(), frompos, &pclinfo->m_RecoveryItem, USEITEM_REASON_DATEOVER);
						pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);


						cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclinfo->m_RecoveryItem.siUnique, useableDay);
						cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );				
						return;
					}
				}
			}
		}
	}

	SI16		InvenItemPos;
	cltItem		InvenItem;
	SI32		rtnvalue;
	
	//cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
	if(false == pclchar->CanAddInv(INVMODE_ALL, &pclinfo->m_RecoveryItem, &InvenItemPos, &InvenItem, &rtnvalue))
	{
		SendServerResponseMsg(0, SRVAL_SPACE_BOX_INVEN_FULL,  0, 0, pclCM->CR[id]->GetCharUnique());

		return; 
	}

	sDBRequest_Move_Item clMsg( DBMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM, 
								id, 
								pclCM->CR[id]->pclCI->GetPersonID(), 
								InvenItemPos, 
								&InvenItem, 
								pclinfo->m_siPos, 
								&pclinfo->m_RecoveryItem, 
								pclinfo->m_bGacha, 
								pclinfo->m_RecoveryItem.CanPile( pclItemManager ) );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

// [추가 : 황진성 2008. 2. 28 => 아티펙트 조합 메시지. C->S ]
void cltServer::DoMsg_GAMEMSG_REQUEST_ARTIFACT_MIXTURE(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_Artifact_Mixture* pclinfo = (cltGameMsgRequest_Artifact_Mixture*)pclMsg->cData;

	// 결합이 가능한 정보들인지 검사.
	BOOL bResult= pclClient->pclItemManager->CanCrystalMixtureData( &pclinfo->m_clEquipItem, pclinfo->m_clCrystalItem ,pclinfo->m_siEquipItemPos);
	switch( bResult )
	{
		case cltItemManagerCommon::NO_ITEM_UNIQUE:	
		case cltItemManagerCommon::NO_SLOT_ITEM:
		case cltItemManagerCommon::NO_USED_SLOT:
		case cltItemManagerCommon::NO_CRYSTAL:		
		case cltItemManagerCommon::ALREADY_USE_SLOT:	
		case cltItemManagerCommon::SAME_CRYSTAL:
		case cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY:		return; 
	}

	// 성공이 아니면 무조건 꺼져~
	if(cltItemManagerCommon::POSSIBLE != bResult)
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_ARTIFACT, 0, 0, id ); 
		return; 
	}

	cltCrystalItem* pEquipCristalInfo	= NULL;
	cltItem*		pCristalItemInfo	= NULL;			

	for(SI32 Count = 0; Count < pclinfo->m_clEquipItem.siUseCrystalNum; ++Count )
	{
		//KHY - 20090526 - 아바타 결정체. - 능력을 중첩시켜 주어야 한다.
		if(( pclinfo->m_clCrystalItem[Count].siUnique > 0 ) 
			&&( pclClient->pclItemManager->IsItemInfoAtb(pclinfo->m_clCrystalItem[Count].siUnique,ITEMINFOATB_CRYSTAL_AVATAR) == true))
		{
			pEquipCristalInfo	= &pclinfo->m_clEquipItem.cltItemCrytal[Count];	
			pCristalItemInfo	= &pclinfo->m_clCrystalItem[Count];

			// 아바타 결정체는  - 능력치가 같아야 하며, 내구도는 상관없고, 능력치는 중첩되어야 한다.
			if((pEquipCristalInfo->siCrystalType == 0 )
			 ||(pEquipCristalInfo->siCrystalType == pCristalItemInfo->siUnique))
			{
				pEquipCristalInfo->siCrystalType	   = pCristalItemInfo->siUnique;				 // Crystal Unique

				// 내구도는 상관없지만, 새로 넣을때는 있어야 함으로.
				pEquipCristalInfo->siDurability		   = pCristalItemInfo->GetCrystalMaxDurability(); 
				pEquipCristalInfo->siMaxDurability     = pCristalItemInfo->GetCrystalMaxDurability();

				// 아이템의 타입과 결정체의 능력에 따라 최대값이 다르다.
				SI32 maxAvatarCrystalAbility = pclClient->pclItemManager->GetMaxAbatarCrystalAbilityNum(pclinfo->m_clEquipItem.GetType(pclItemManager) , pCristalItemInfo->GetCrystalAbility());

				// 0 이라면 잘못된 결정체다.
				if(maxAvatarCrystalAbility > 0)
				{
					pEquipCristalInfo->uiCrystalAbility    = pCristalItemInfo->GetCrystalAbility();
					pEquipCristalInfo->uiCrystalAbilityNum += pCristalItemInfo->GetCrystalAbilityNum(); //누적.

					// 클라이언트에서 검사해서 보내겠지만.
					if(pEquipCristalInfo->uiCrystalAbilityNum > maxAvatarCrystalAbility)
					{
						pEquipCristalInfo->uiCrystalAbilityNum = maxAvatarCrystalAbility;
						return;
					}					
				}
				else
					return;
				
			}
		}		
		//KHY - 0805 - 아트펙스 수정. - 결정체가 있고, 그 능력을 아직 가지지 못한 경우에만 만들어준다.
		else if(( pclinfo->m_clCrystalItem[Count].siUnique > 0 ) &&( pclinfo->m_clCrystalItem[Count].clItemCrystal.GetCrystalAblilty() == 0 ))
		{
			pclClient->pclItemManager->MakeCrystal(&pclinfo->m_clCrystalItem[Count],0);

			pEquipCristalInfo	= &pclinfo->m_clEquipItem.cltItemCrytal[Count];	
			pCristalItemInfo	= &pclinfo->m_clCrystalItem[Count];
			
			pEquipCristalInfo->siCrystalType	   = pCristalItemInfo->siUnique;				 // Crystal Unique
			pEquipCristalInfo->siDurability		   = pCristalItemInfo->GetCrystalMaxDurability(); 
			pEquipCristalInfo->siMaxDurability     = pCristalItemInfo->GetCrystalMaxDurability();
			pEquipCristalInfo->uiCrystalAbility    = pCristalItemInfo->GetCrystalAbility();
			pEquipCristalInfo->uiCrystalAbilityNum = pCristalItemInfo->GetCrystalAbilityNum();
		}
		else if( pclinfo->m_clCrystalItem[Count].siUnique > 0)
		{
			pEquipCristalInfo	= &pclinfo->m_clEquipItem.cltItemCrytal[Count];	
			pCristalItemInfo	= &pclinfo->m_clCrystalItem[Count];
			
			pEquipCristalInfo->siCrystalType	   = pCristalItemInfo->siUnique;				 // Crystal Unique
		
			// 능력치가 붙어있는 결정체 일경우 인벤토리 슬롯 번호로 유효성을 체크한다
			if ( (PERSONITEM_INV0 <= pclinfo->m_siCrystalItemPos[Count]) && (MAX_ITEM_PER_PERSON > pclinfo->m_siCrystalItemPos[Count]) )
			{
				pEquipCristalInfo->siDurability		= pCristalItemInfo->GetCrystalMaxDurability();
			}

			pEquipCristalInfo->siMaxDurability		= pCristalItemInfo->GetCrystalMaxDurability();
			pEquipCristalInfo->uiCrystalAbility		= pCristalItemInfo->GetCrystalAbility();
			pEquipCristalInfo->uiCrystalAbilityNum	= pCristalItemInfo->GetCrystalAbilityNum();

			// 아이템 위치가 0 인 경우( 이미 조합된 결정체)의 경우는 초기화 시켜준다.
			if(pclinfo->m_siCrystalItemPos[Count] == 0)
				pclinfo->m_clCrystalItem[Count].Init();
		}
		else
		{
			pclinfo->m_clCrystalItem[Count].Init();
		}
	}
	
	sDBRequest_Artifact_Mixture clMsg( id, pclCM->CR[id]->pclCI->GetPersonID(),  pclinfo->m_siEquipItemPos,		&pclinfo->m_clEquipItem,
																			     pclinfo->m_siCrystalItemPos,    pclinfo->m_clCrystalItem );

	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

// [추가 : 황진성 2008. 2. 28 => 아티펙트 제거 메시지. C->S ]
void cltServer::DoMsg_GAMEMSG_REQUEST_ARTIFACT_REMOVE(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_Artifact_Remove* pclinfo = (cltGameMsgRequest_Artifact_Remove*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	
	// 슬롯이 있는 아이템인가.
	BOOL bResult = pclClient->pclItemManager->CanCrystalRemoveData( &pclinfo->m_clEquipItem, pclinfo->m_bRemoveSlotIndexList, pclinfo->m_clCrystalItemList );
	switch( bResult )	
	{
		case cltItemManagerCommon::NO_ITEM_UNIQUE:
		case cltItemManagerCommon::NO_SLOT_ITEM:
		case cltItemManagerCommon::NO_CRYSTAL_SLOT:
		case cltItemManagerCommon::NO_REMOVE_CRYSTAL:	return;
	}

	// 성공이 아니면 무조건 꺼져~
	if(cltItemManagerCommon::POSSIBLE != bResult)
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_ARTIFACT, 0, 0, id );
		return; 
	}

	SI16	clCreateCrystalPos[ MAX_CRYSTAL_NUM ];
	ZeroMemory( clCreateCrystalPos, sizeof( clCreateCrystalPos ) );

	cltItem clCreateCrystal[ MAX_CRYSTAL_NUM ];
	ZeroMemory( clCreateCrystal,	   sizeof( clCreateCrystal ) );

	SI32 DeleteItemPos[MAX_CRYSTAL_NUM];
	ZeroMemory(DeleteItemPos, sizeof(DeleteItemPos));

	SI32 ItemUnique		 = 0; 
	SI32 SaveItemSlotNum = 0;
	SI16 brareswitch	 = 0;
	SI16 NewDurability	 = 0;
	
	// 내구도 최대치는 무조건 30000 이다.
	const SI16 ConstCrystalDurability = 30000;
	SI32 CreateSlotCount = 0;
	
	for( SI32 Count = 0; Count < MAX_CRYSTAL_NUM; ++Count )
	{
		//KHY - 0805 - 아트펙스 수정.
		if(pclinfo->m_clCrystalItemList[Count].siUnique == ITEMUNIQUE(1803)) // 리무브 결정체 라면.
		{
			if( true == pclinfo->m_bRemoveSlotIndexList[Count] )
			{
				ItemUnique = pclinfo->m_clEquipItem.cltItemCrytal[Count].GetCrystalType();
				if( 0 < ItemUnique )
				{
					if( false == pclItemManager->MakeRandItemUnique( ItemUnique, &clCreateCrystal[ SaveItemSlotNum ], 0, 0, &brareswitch ) ) 
					{
						continue;		
					}
								
					// 새로운 내구도 설정. 이전 내구도에서 최대 내구도의 10%를 빼서 아이템이 가지는 최대 내구도를 설정한다.
					NewDurability = pclinfo->m_clEquipItem.cltItemCrytal[Count].GetMaxDurability() - ( ConstCrystalDurability / 10 );
					if( 3000 > NewDurability	)
					{
						// 3000이 결정체 제거시 생성되는 결정의 최하 내구도.
						NewDurability = 3000;
					}
					clCreateCrystal[ SaveItemSlotNum ].clItemCrystal.SetMaxDurability(NewDurability);
					clCreateCrystal[ SaveItemSlotNum ].clItemCrystal.uiCrystalAbility    = pclinfo->m_clEquipItem.cltItemCrytal[Count].GetCrystalAblilty();
					clCreateCrystal[ SaveItemSlotNum ].clItemCrystal.uiCrystalAbilityNum = pclinfo->m_clEquipItem.cltItemCrytal[Count].GetCrystalAbilityNum();
					
					//  리무브 결정체를 사용하면,결정체를 생성한다. - 인벤 위치 선정.
					SI16 invpos;
					cltItem cltoitem;
					SI32 rtnvalue;
					
					if(pclchar->CanAddInv(INVMODE_ALL, &clCreateCrystal[ SaveItemSlotNum ], &invpos, &cltoitem, &rtnvalue) == false)
					{
						((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_CANNOTINPUTTOINV, 0, 0, id);
							return ;
					}
					else				
					{
						// 하나씩만 만들어 질 수 있다.  - 위치값을 잡기가 애매해서.	
						clCreateCrystalPos[CreateSlotCount] = invpos;
						++CreateSlotCount;							
						/*
						// 이미 선택된 인벤포즈 값이 있는지 검사한다.
						bool check = true;
						for(SI32 x = 0; x < MAX_CRYSTAL_NUM ;x++)
						{
							if(clCreateCrystalPos[x] == invpos)
								check = false;
						}

						if(check == true)
						{
							clCreateCrystalPos[CreateSlotCount] = invpos;
							++CreateSlotCount;							
						}
						*/
					}

					++SaveItemSlotNum;	// 다음 슬롯에 저장하게...
				}
				
				ZeroMemory(&pclinfo->m_clEquipItem.cltItemCrytal[Count], sizeof(pclinfo->m_clEquipItem.cltItemCrytal[Count]));
			}
		}
		else if(pclinfo->m_clCrystalItemList[Count].siUnique == ITEMUNIQUE(1804)) // 리차지 결정체 라면.
		{
			if( true == pclinfo->m_bRemoveSlotIndexList[Count] )
			{
				ItemUnique = pclinfo->m_clEquipItem.cltItemCrytal[Count].GetCrystalType();
				if( 0 < ItemUnique )
				{
					if(pclinfo->m_clEquipItem.cltItemCrytal[Count].GetMaxDurability() <= 1500) // 내구도 5 이하라면, 
					{
						SendServerResponseMsg( 0, SRVAL_FAIL_ARTIFACT, 0, 0, id );
						return; 
					}
						
					// 새로운 내구도 설정. 이전 내구도에서 최대 내구도의 5%를 빼서 아이템이 가지는 최대 내구도를 설정한다.
					NewDurability = pclinfo->m_clEquipItem.cltItemCrytal[Count].GetMaxDurability() - ( ConstCrystalDurability / 20 );

					pclinfo->m_clEquipItem.cltItemCrytal[Count].SetMaxDurability(NewDurability);
					pclinfo->m_clEquipItem.cltItemCrytal[Count].SetDurability(NewDurability);					
				}
			}
		}
	}


	sDBRequest_Artifact_Remove clMsg(  id,  
									   pclCM->CR[id]->pclCI->GetPersonID(), 
									   pclinfo->m_siEquipItemPos, 
									  &pclinfo->m_clEquipItem,
									   pclinfo->m_siCrystalItemPosList,
									   pclinfo->m_clCrystalItemList,
									   clCreateCrystalPos, 
									   clCreateCrystal							);	
	
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );   
}

// [추가 : 황진성 2008. 2. 28 => 아티펙트 슬롯생성 메시지. C->S ]
void cltServer::DoMsg_GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_Artifact_Slot_Create* pclinfo = (cltGameMsgRequest_Artifact_Slot_Create*)pclMsg->cData;

	// 아이템 위치 범위 조사.
	if( pclinfo->m_siCrystalItemPos < 0 || pclinfo->m_siCrystalItemPos >= MAX_ITEM_PER_PERSON || 
	 	pclinfo->m_siEquipItemPos   < 0 || pclinfo->m_siEquipItemPos   >= MAX_ITEM_PER_PERSON	) 
	{
		return ;
	}

	BOOL bResult = pclClient->pclItemManager->CanCrystalSlotCreateData( &pclinfo->m_clEquipItem, &pclinfo->m_clCrystalItem );
	switch( bResult )	
	{
		case cltItemManagerCommon::NO_ITEM_UNIQUE:
		case cltItemManagerCommon::NO_SLOT_ITEM:
		case cltItemManagerCommon::USE_CRYSTAL_SLOT:
		case cltItemManagerCommon::NO_CRYSTAL:			return; 
	}

	// 성공이 아니면 무조건 꺼져~
	if(cltItemManagerCommon::POSSIBLE != bResult)
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_ARTIFACT, 0, 0, id );
		return; 
	}

	SI32	ref			  = pclItemManager->FindItemRefFromUnique(pclinfo->m_clEquipItem.siUnique);	
	if(0 >= ref)
		return;

	SI32 siLevel		= pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
	SI32 oldCrystalNum	= pclinfo->m_clEquipItem.GetUseCrystalNum(); // 본래 가지고 있던, 슬롯수.
	SI32 temCrystalNum	= 0;
	cltItem clOldItem;	clOldItem.Set(&pclinfo->m_clEquipItem);

	if(pclinfo->m_clCrystalItem.siUnique == ITEMUNIQUE(1801)) //슬롯 결정체
	{	
		SI32 temnum = 0;
		if((siLevel >= 1)&&(siLevel <= 29))
		{
			temCrystalNum = 1;
			
			// 누적시킨다.
			temnum = pclinfo->m_clEquipItem.siUseCrystalNum + temCrystalNum;
			temCrystalNum = min(2, temnum);
		}
		else if((siLevel >= 30)&&(siLevel <= 59))
		{
			temCrystalNum = 1 + rand()%2;

			// 누적시킨다.
			temnum = pclinfo->m_clEquipItem.siUseCrystalNum + temCrystalNum;
			temCrystalNum = min(3, temnum);
		}
		else if((siLevel >= 60)&&(siLevel <= 89))
		{
			temCrystalNum = 1 + rand()%3;

			// 누적시킨다.
			temnum = pclinfo->m_clEquipItem.siUseCrystalNum + temCrystalNum;
			temCrystalNum = min(4, temnum);
		}
		else if(siLevel >= 90)
		{
			temCrystalNum = 1 + rand()%4;

			// 누적시킨다.
			temnum = pclinfo->m_clEquipItem.siUseCrystalNum + temCrystalNum;
			temCrystalNum = min(5, temnum);
		}

		pclinfo->m_clEquipItem.siUseCrystalNum = temCrystalNum;
	}
	else if(pclinfo->m_clCrystalItem.siUnique == ITEMUNIQUE(1802)) // 슬롯 결정체[S]
	{
		if((siLevel >= 1)&&(siLevel <= 29))
		{
			temCrystalNum =  oldCrystalNum + rand()%2;
			temCrystalNum = min(2, temCrystalNum);
		}
		else if((siLevel >= 30)&&(siLevel <= 59))
		{
			temCrystalNum = oldCrystalNum + rand()%3;
			temCrystalNum = min(3, temCrystalNum);
		}
		else if((siLevel >= 60)&&(siLevel <= 89))
		{
			temCrystalNum = oldCrystalNum + rand()%4;
			temCrystalNum = min(4, temCrystalNum);
		}
		else if(siLevel >= 90)
		{
			temCrystalNum = oldCrystalNum + rand()%5;
			temCrystalNum = min(5, temCrystalNum);
		}

		// 누적시키지 않는다.
		pclinfo->m_clEquipItem.siUseCrystalNum =  temCrystalNum;
	}
	
	// 슬롯에 있던 모든 결정체를 읍쎄버린다.
	ZeroMemory(&pclinfo->m_clEquipItem.cltItemCrytal, sizeof(pclinfo->m_clEquipItem.cltItemCrytal));
			
	sDBRequest_Artifact_Slot_Create clMsg(	id, 
											pclCM->CR[id]->pclCI->GetPersonID(),
											pclinfo->m_siEquipItemPos,
										   &pclinfo->m_clEquipItem,
											pclinfo->m_siCrystalItemPos,
										   &pclinfo->m_clCrystalItem,
										   &clOldItem		);

	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_OPENBIGITEMBOX(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_OpenBigItemBox* pclinfo = (cltGameMsgRequest_OpenBigItemBox*)pclMsg->cData;

	const SI32 siMax_Item_Count = 100;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	cltItem clOutPutItem;
	clOutPutItem.Init();
	SI16 brareswitch = 0;

	cltUniqueRange* pGrade = NULL;
	SI32 siGradeCount = 0;

	// 랜덤 아이템 결정
	if ( pclinfo->m_clKeyItem.siUnique == ITEMUNIQUE(8113) )		
	{
		pGrade = (cltUniqueRange*)ItemUnique_8113_Grade;		siGradeCount = sizeof(ItemUnique_8113_Grade) / sizeof(cltUniqueRange);
	}
	else if	( pclinfo->m_clKeyItem.siUnique == ITEMUNIQUE(13621) )
	{
		pGrade = (cltUniqueRange*)ItemUnique_13621_Grade;	siGradeCount = sizeof(ItemUnique_13621_Grade) / sizeof(cltUniqueRange);	
	}

	if( pGrade == NULL || siGradeCount <= 0)			
		return;

	SI32 siGrade = GetUniqueWithRange( pGrade, siGradeCount, 100);

	//----------------------------------------------------------------------------------------------------
	// 1 등급 아이템 설정
	//----------------------------------------------------------------------------------------------------
	cltValuesRange BigItemBox_1_Grade[siMax_Item_Count] = 
	{
		// 퍼센트,		유니크,			개수,			기간.
		{   5,		5350,				1,				0 }	
		,{   5,		5355,				1,				0 }	
		,{   5,		5360,				1,				0 }	
		,{   5,		5365,				1,				0 }	
		,{   5,		5370,				1,				0 }
		,{   5,		5410,				1,				0 }	
		,{   5,		2757,				1,				0 }
		,{   5,		5400,				1,				0 }
		,{   4,		3890,				1,				0 }	
		,{   4,		3899,				1,				0 }	
		,{   3,		5600,				1,				0 }	
		,{  10,		24111,				1,				0 }	
		,{   3,		2740,				1,				0 }	
		,{   5,		24040,				1,				0 }	
		,{   5,		24094,				1,				0 }	
		,{   1,		15900,				1,			   30 }	
		,{   1,		13621,				1,				0 }	
		,{   1,		24098,				1,				0 }	
		,{   1,		2760,				1,				0 }	
		,{   1,		5710,				1,				0 }	
		,{   1,		5715,				1,				0 }	
		,{   1,		5412,				1,				0 }	
		,{   0,		   0,				0,				0 }	
	};
	SI32 BigItemBox_1_Grade_Count = 0;
	for(SI32 i=0; i<siMax_Item_Count; i++)
	{
		if( BigItemBox_1_Grade[i].siPercent <= 0 )			break;
		BigItemBox_1_Grade_Count++;
	}

	if( IsCountrySwitch( Switch_Artifact ) )
	{
		//,{   9,		2756,				1,				0 }	
		//,{   8,		2710,				1,				0 }	
		//,{   1,		1803,				1,				0 }	
		//,{   1,		1801,				1,				0 }	

		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set(9, 2756, 1, 0);		BigItemBox_1_Grade_Count++;
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set(8, 2710, 1, 0);		BigItemBox_1_Grade_Count++;
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set(1,	1803, 1, 0);		BigItemBox_1_Grade_Count++;
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set(1,	1801, 1, 0);		BigItemBox_1_Grade_Count++;
		// 마지막 체크 설정
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set(0,	   0, 0, 0);		BigItemBox_1_Grade_Count++;
	}
	else
	{
		//,{  10,		2756,				1,				0 }	
		//,{   9,		2710,				1,				0 }	
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set(10, 2756, 1, 0);		BigItemBox_1_Grade_Count++;
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set( 9, 2710, 1, 0);		BigItemBox_1_Grade_Count++;
		// 마지막 체크 설정
		BigItemBox_1_Grade[BigItemBox_1_Grade_Count].Set( 0,	0, 0, 0);		BigItemBox_1_Grade_Count++;
	}
	//----------------------------------------------------------------------------------------------------
	// 1 등급 아이템 설정 여기까지
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// 2 등급 아이템 설정
	//----------------------------------------------------------------------------------------------------
	// "대박상자"에서 나오는 아이템 유니크 
	cltValuesRange  BigItemBox_2_Grade[siMax_Item_Count] = 
	{
		// 퍼센트,		유니크,			개수,			기간.
		{   20,		3895,				1,				0 }	
		,{   19,		3901,				1,				0 }	
		,{	  1,		13605,				1,				0 } // 가챠키조각
		,{   10,		15910,				1,				7 }	
		,{   10,		13621,				1,				0 }	
		,{   50,		2715,				1,				0 }
		,{   50,		2720,				1,				0 }	
		,{   40,		3880,				1,				0 }
		,{  200,		24082,				1,				0 }
		,{   20,		5380,				1,				0 }	
		,{   20,		7175,				1,				0 }	
		,{   20,		6140,				1,				0 }	
		,{   70,		2672,				1,				0 }	
		,{   70,		2705,				1,				0 }	
		,{   50,		3855,				1,				0 }	
		,{  100,		24075,				1,				0 }	
		,{   50,		2781,				1,				0 }	
		,{   50,		2782,				1,				0 }	
		,{   50,		2635,				1,				0 }	
		,{   50,		24020,				1,				0 }	
		,{   50,		2730,				1,				0 }	
	};

	SI32 BigItemBox_2_Grade_Count = 0;
	for ( SI32 siIndex=0; siIndex<siMax_Item_Count; siIndex++ )
	{
		if( BigItemBox_2_Grade[siIndex].siPercent <= 0 )
		{
			break;
		}
		BigItemBox_2_Grade_Count++;
	}

	// 산타상자 판매시 산타상자조각 나오도록 함
	if ( pclEventTimeManager->InEventTime(TEXT("SantaBoxPiece"), &sTime) )
	{
		BigItemBox_2_Grade[BigItemBox_2_Grade_Count++].Set(1, 8124, 1, 0);	// 산타상자 조각
	}

	// 랜덤 상자 조각 유니크
	if ( pclEventTimeManager->InEventTime(TEXT("RandomBoxPiece"), &sTime) )
	{
		SI32 siRandomBoxPieceUnique = (SI32)GetGlobalValue( TEXT("GV_RandomBoxPieceUnique") );
		if ( 0 < siRandomBoxPieceUnique )
		{
			BigItemBox_2_Grade[BigItemBox_2_Grade_Count++].Set(1, siRandomBoxPieceUnique, 1, 0);	// 산타상자 조각
		}
	}

	// ### 끝에는 0,0,0,0 의 형식으로 한다 새로운것을 중간에 추가하고 싶다면 위에다 한다 ###
	BigItemBox_2_Grade[BigItemBox_2_Grade_Count].Set( 0,0,0,0 );
	//----------------------------------------------------------------------------------------------------
	// 2 등급 아이템 설정 여기까지
	//----------------------------------------------------------------------------------------------------

	cltValuesRange* pValues = NULL;
	switch(siGrade)
	{
	case 1:		pValues = GetValues(BigItemBox_1_Grade);																	break;
	case 2:		pValues = GetValues(BigItemBox_2_Grade);																	break;
	case 3:		pValues = GetValuesWithRange(BigItemBox_3_Grade, sizeof(BigItemBox_3_Grade)/sizeof(cltValuesRange), 100);	break;
	}
	if( pValues == NULL )
		return;

	SI32 siItemUnique	= pValues->siPara1;
	SI32 siItemNum		= pValues->siPara2;
	SI32 usedate		= 0;

	SI32 ref = pclItemManager->FindItemRefFromUnique(siItemUnique);
	if( ref < 0 )				return;

	if( pValues->siPara3 > 0 )
		usedate = pValues->siPara3;
	else
		usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

	if( pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clOutPutItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate ) == false )
		return;

	//---------------------------------------------------------
	// 대박상자에서 나오는 물품은 다시 밀봉시킨다.
	clOutPutItem.uiDateYear	 = 0;
	clOutPutItem.uiDateMonth = 0;
	clOutPutItem.uiDateDay	 = 0;
	//---------------------------------------------------------

	clOutPutItem.SetItemNum(siItemNum);

	// 인벤토리 빈 공간 체크
	if( pclchar->IsBlankInv(1,INVMODE_ALL) == false )
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 1, 0, pclchar->GetID());
		return;
	}

	cltItem cltoitem;
	SI16	invpos;

	// 아이템 지급
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &clOutPutItem, &invpos, &cltoitem, &rtnvalue) == false)
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, pclchar->GetID());
		return;
	}
	else
	{
		if(invpos >= PERSONITEM_INV0 && invpos < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON)
		{
			pclchar->SetInputItemReserve( invpos , pclClient->CurrentClock );
		}
		else
		{
			return;
		}
	}

	// 현재는 1등급만 로그를 찍는다.
	if(siGrade == 1)
		pclClient->pclLog->FilePrint(TEXT("Config\\BigItemBoxGet.Txt"), 
					TEXT("RareItem(Grade:%d) Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d] / Use Key Unique : \t[%d]"), 
					siGrade, pclCM->CR[id]->GetName(), clOutPutItem.siUnique, clOutPutItem.siItemNum, pclinfo->m_clKeyItem.siUnique );

	sDBRequest_OpenTreasureBox clMsg( pclchar->pclCI->GetPersonID(), pclinfo->m_siBoxPos, &pclinfo->m_clBoxItem, pclinfo->m_siKeyPos, &pclinfo->m_clKeyItem, invpos, &clOutPutItem, pclItemManager->CanPile(clOutPutItem.siUnique), clOutPutItem.IsCreateSID(pclItemManager), false );
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONPUSHITEM(cltMsg* pclMsg, SI32 id)
{
	//---------------------------------------------
	// 기본적인 오류검사
	//---------------------------------------------
	// 제조시에는 이동 불가
	if ( 0 < pclCM->CR[id]->siPersonalMakeItemType)
	{
		return;
	}

	// 인첸트시에는 이동 불가
	if ( true == pclCM->CR[id]->bPersonalEnchantItemSwitch )
	{
		return; 
	}

	cltGameMsgRequest_SummonPushItem* pclSummonMoveItemInfo = (cltGameMsgRequest_SummonPushItem*)pclMsg->cData;
	if ( NULL == pclSummonMoveItemInfo )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( NULL == pclchar )
	{
		return;
	}

	//---------------------------------------------
	// 자주쓰일 변수 선언
	//---------------------------------------------
	SI32		siPersonID		= pclchar->pclCI->GetPersonID();
	SI32		siSummonIndex	= pclSummonMoveItemInfo->m_siSummonIndex;
	UI16		uiFrom			= pclSummonMoveItemInfo->m_uiFrom;
	UI16		uiTo			= pclSummonMoveItemInfo->m_uiTo;

	cltItem*	pclFrom			= NULL;
	cltItem		clTo;

	// 잘못된 소환수 번호가 들어왔을때
	if ( (0 >siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	cltSummon* pclsummon = (cltSummon*)pclchar->pclCI->clSummonInfo.GetSummon( siSummonIndex );
	if ( NULL == pclsummon )
	{
		return;
	}
	
	// 소환된 상태가 아니라면 
	if ( pclsummon->siStatus != SUMMON_STATUS_TAKEOUT )
	{
		return;
	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();


	//---------------------------------------------
	// 아이템 관련 검사
	//---------------------------------------------

	// 소환수 인벤 검사
	if ( (0 > uiTo)					|| (MAX_SHAPE_ITEM <= uiTo)			)	return;
	// 사람 인벤 검사( 사람에서 오는것이기 때문에 장비 칸은 포함되지 않는다 )
	if ( (MAX_SHAPE_ITEM > uiFrom)	|| (MAX_ITEM_PER_PERSON <= uiFrom)	)	return;
	

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 이동 안된다.
	if( pclchar->bPersonalShopSwitch == true  && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == uiTo	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == uiFrom	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}

			// 귀속전용 개인상점
			if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == uiTo	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}
	}	// if( pclchar->bPersonalShopSwitch == true  && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL) 종료

	//--------------------------------------------
	//트레이드중인 것은 안된다!
	//--------------------------------------------
	if ( 0 != pclchar->GetNoMoveItemSwitch( uiFrom ) )
	{
		return;
	}

	//--------------------------------------------
	//From의 아이템을 구한다.
	//--------------------------------------------
	pclFrom = &pclchar->pclCI->clCharItem.clItem[uiFrom];
	
	// 아이템이 무효한 것이라면..
	if ( (NULL == pclFrom) || (0 >= pclFrom->siUnique) )
	{
		return;
	}

	//-----------------------------------------------
	// To 에 넣는다. 
	//-----------------------------------------------
	if ( NULL == pclchar->pclCI->clSummonInfo.GetSummonEquipItem( siSummonIndex, uiTo ) )
	{
		return;
	}
	clTo.Set( pclchar->pclCI->clSummonInfo.GetSummonEquipItem(siSummonIndex, uiTo) );


	//------------------------------------------
	// 장착 조건을 충족시키는지 확인한다. 
	//------------------------------------------
	// 착용 가능한 장비위치인지 검사
	if ( false == pclItemManager->IsCanEquipItemPosForSummon( uiTo, pclFrom->siUnique ) )
	{
		return;
	}

	// 적절하지 않은 곳으로는 이동시킬 수 없다. 
	if ( false == pclItemManager->CheckProperItemPos(pclFrom, uiTo, pclsummon->siKind) )
	{
		return ;
	}

	SI32 siSummonID = pclchar->GetSummonID();
	if ( false == pclCM->IsValidID(siSummonID) )
	{
		return;
	}

	cltCharCommon* pclSummonChar = pclCM->CR[ siSummonID ];
	if ( NULL == pclSummonChar )
	{
		return;
	}

	// 해당 아이템을 착용할 수 있는 능력이 있는지 검사	
	SI32		siDummy		= 0;
	cltItem*	pclItemList = pclchar->pclCI->clSummonInfo.GetSummonEquipItemList( siSummonIndex );
	if ( NULL == pclItemList )
	{
		return;
	}

	if ( false == pclItemManager->CheckConditionForSummon( pclCM->CR[id], pclFrom->siUnique, pclsummon, pclItemList, &siDummy) )
	{
		return;
	}


	//------------------------------------------
	// 밀봉상태 아이템은 장착불가 
	//------------------------------------------
	if ( true == pclFrom->IsSealingUp(pclItemManager) )
	{
		return;
	}
	// 기간이 지난 아이템은 장착불가 - by LEEKH	
	SI32 siUseableDay;
	BOOL rVal			= GetItemUseAbleDay( pclFrom, &siUseableDay );
	
	if ( (TRUE == rVal) && (0 > siUseableDay) )
	{
		return;
	}

	cltItem clitem;
	clitem.Set(pclFrom);


	//-----------------------------------------------
	// 발사체 아이템일때 갯수 제한
	//-----------------------------------------------
	if ( (TRUE == clitem.IsBulletItem()) && ( (clTo.siUnique == clitem.siUnique) || (DEFAULT_EQUIP_BULLET_NUMBER < clitem.siItemNum) ) )
	{
		if ( DEFAULT_EQUIP_BULLET_NUMBER < (clTo.siItemNum + clitem.siItemNum) )
		{
			clitem.siItemNum = (DEFAULT_EQUIP_BULLET_NUMBER - clTo.siItemNum);
		}
	}

	if ( 0 >= clitem.siItemNum )
	{
		return;
	}

	sDBRequest_SummonPushItem clMsg( id, siPersonID, pclsummon->siSummonID, uiFrom, &clitem, pclFrom->CanPile(pclItemManager), uiTo, &clTo );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONPOPITEM(cltMsg* pclMsg, SI32 id)
{
	// 사람만 해당된다
	if ( false == IsPC(id) )
	{
		return;
	}

	// 제조시에는 이동 불가
	if ( 0 < pclCM->CR[id]->siPersonalMakeItemType)
	{
		return;
	}

	// 인첸트시에는 이동 불가
	if ( true == pclCM->CR[id]->bPersonalEnchantItemSwitch )
	{
		return; 
	}

	cltGameMsgRequest_SummonPopItem* pclSummonPopItemInfo = (cltGameMsgRequest_SummonPopItem*)pclMsg->cData;
	if ( NULL == pclSummonPopItemInfo )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( NULL == pclchar )
	{
		return;
	}

	SI32		siPersonID		= pclchar->pclCI->GetPersonID();
	SI32		siSummonIndex	= pclSummonPopItemInfo->m_siSummonIndex;
	UI16		uiFrom			= pclSummonPopItemInfo->m_uiFrom;
	UI16		uiTo			= pclSummonPopItemInfo->m_uiTo;

	cltItem*	pclFrom			= NULL;
	cltItem		clTo;

	// 잘못된 소환수 번호가 들어왔을때
	if ( (0 >siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	cltSummon*	pclsummon = pclchar->pclCI->clSummonInfo.GetSummon( siSummonIndex );
	if(pclsummon == NULL)			return;

	if ( pclsummon->siStatus != SUMMON_STATUS_TAKEOUT )
	{
		return;
	}
	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();


	// 소환수에서 사람에게로 이동할 경우
	if ( (0 > uiTo)		|| (MAX_ITEM_PER_PERSON <= uiTo)	)	return;
	if ( (0 > uiFrom)	|| (MAX_SHAPE_ITEM <= uiFrom)		)	return;

	//------------------------------------------------
	// 말을 갖고 있지 않으면 말 인벤에 넣을 수는 없다.
	//------------------------------------------------
	if ( pclchar->HaveHorse() == false )
	{
		if ( (uiTo >= PERSONITEM_HORSE0) && (uiTo < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY) )	return;
	}

	//----------------------------------------------------
	// 가방을 갖고 있지 않으면 가방 인벤에 넣을 수는 없다
	//----------------------------------------------------
	if ( 0 >= pclchar->pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique )
	{
		if ( (uiTo >= PERSONITEM_BAG0) && (uiTo < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG) )	return;
	}

	//----------------------------------------------------
	// 가방2을 갖고 있지 않으면 가방 인벤에 넣을 수는 없다
	//----------------------------------------------------
	if ( 0 >= pclchar->pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique )
	{
		if ( (uiTo >= PERSONITEM_BAG1) && (uiTo < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG) )	return;
	}

	//--------------------------------------------
	//From의 아이템을 구한다.
	//--------------------------------------------
	pclFrom	= &pclchar->pclCI->clSummonInfo.clItemInfo[ siSummonIndex ].clEquipItem[ uiFrom ];

	// 아이템이 무효한 것이라면..
	if ( (NULL == pclFrom) || (0 >= pclFrom->siUnique) )
	{
		return;
	}

	//-----------------------------------------------
	// To 에 넣는다. 
	//-----------------------------------------------
	clTo.Set( &pclchar->pclCI->clCharItem.clItem[uiTo] );
	if ( 0 < clTo.siUnique )
	{
		return;
	}


	//-----------------------------------------------
	//DB메니저로 아이템을 이동하라는 요청을 보낸다. 
	//-----------------------------------------------
	cltItem clitem;
	clitem.Set(pclFrom);

	sDBRequest_SummonPopItem clMsg( id, siPersonID, pclsummon->siSummonID, uiFrom, &clitem, pclFrom->CanPile(pclItemManager), uiTo, &clTo );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_ADD(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Amber_Box_Select_Item_Add* pclInfo = (cltGameMsgRequest_Amber_Box_Select_Item_Add*)pclMsg->cData;
	if( NULL == pclInfo )	return;
	
	cltCharServer* pclchar = pclClient->pclCM->GetCharServer(id);
	if ( NULL == pclchar )	return;

	if( 0 > pclInfo->m_siSlotIndex || AMBER_BOX_SELECT_ITEM_LIST_NUM <= pclInfo->m_siSlotIndex ) return;

	// 호박상자류가 아니면 리턴
	if ( (NULL == m_pclBoxItemMgr) || (FALSE == m_pclBoxItemMgr->IsAmberBoxItem(pclInfo->m_clUseItemUnique)) )
	{
		return;
	}

	// 랜덤하게 아이템을 가져온다
	CBoxItemUnit* pclGetBoxItemUnit = m_pclBoxItemMgr->GetRandItemFromUnique( pclInfo->m_clUseItemUnique );
	if ( NULL == pclGetBoxItemUnit )
	{
		return;
	}
	
	AmberboxSelectItemInfo	clAmberboxItemList;
	clAmberboxItemList.m_siItemUnique = pclGetBoxItemUnit->GetItemUnique();
	clAmberboxItemList.m_siItemNum    =	pclGetBoxItemUnit->GetItemNum();
	clAmberboxItemList.m_siSlot		  = pclInfo->m_siSlotIndex;
	
	sDBRequest_Amber_Box_Select_Item_Add clMsg(id, pclchar->pclCI->GetPersonID(), &clAmberboxItemList, pclInfo->m_clUseItemUnique);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}


void cltServer::DoMsg_GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_GET(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Amber_Box_Select_Item_Get* pclInfo = (cltGameMsgRequest_Amber_Box_Select_Item_Get*)pclMsg->cData;
	if( NULL == pclInfo )	return;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if ( NULL == pclchar )	return;

	if( 0 > pclInfo->m_clAmberboxItem.m_siSlot || AMBER_BOX_SELECT_ITEM_LIST_NUM <= pclInfo->m_clAmberboxItem.m_siSlot ) return;

	// 사용되는 아이템을 등록. 그 외에는 다 리턴.
	if ( (NULL == m_pclBoxItemMgr) || (FALSE == m_pclBoxItemMgr->IsAmberBoxItem(pclInfo->m_clUseItemUnique)) )
	{
		return;
	}

	SI16 siUseItemPos = pclchar->pclCI->clCharItem.FindItemInv( pclInfo->m_clUseItemUnique );
	if( 0 >= siUseItemPos)	return;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	cltItem clUseItem;
	clUseItem.Set(&pclchar->pclCI->clCharItem.clItem[siUseItemPos]);
	clUseItem.SetItemNum(1);

	// 얻는 아이템 정보.
	SI16 brareswitch = 0;
	SI32 ItemUnique  = pclInfo->m_clAmberboxItem.m_siItemUnique;
	if( 0 >= ItemUnique )	return;

	SI32 ref = pclItemManager->FindItemRefFromUnique( ItemUnique );
	if( 0 >= ref )			return;

	SI32 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

	cltItem clGetItem;	// 얻을 아이템
	clGetItem.Init(); 

	if( false == pclItemManager->MakeRandItemUnique( ItemUnique, &clGetItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) )
		return ;

	// 재 밀봉한다.
	clGetItem.uiDateYear = 0;
	clGetItem.uiDateMonth = 0;
	clGetItem.uiDateDay = 0;

	if( 0 >= pclInfo->m_clAmberboxItem.m_siItemNum || 10000 < pclInfo->m_clAmberboxItem.m_siItemNum) return;

	clGetItem.SetItemNum( pclInfo->m_clAmberboxItem.m_siItemNum );

	// 아이템을 얻고 세팅될 아이템 정보.
	SI16 siGetItemPos = 0;
	SI32 siRtnvalue   = 0;
	cltItem clToItem;	
	clToItem.Init();

	if( false == pclchar->CanAddInv(INVMODE_ALL, &clGetItem, &siGetItemPos, &clToItem, &siRtnvalue) )
	{ 
		SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 100, 0, id); 
		return;	
	}

	bool bCanPile = true;
	// 중첩이 불가능한 아이템은 요렇게 주자.
	if(false == clGetItem.CanPile(pclClient->pclItemManager))
	{
		bCanPile = false; 
		//cltItem	clItem;
		//clItem.Set(&clGetItem);
		//clItem.SetItemNum(1);

		//for(SI32 Count = 0; Count < clGetItem.siItemNum; ++Count)
		//	//pclchar->GiveItem( ItemUnique, 1, INPUTITEMPERSON_OPTION_AMBERBOX, usedate );
		//	InputItem( pclchar, &clItem, INPUTITEMPERSON_OPTION_AMBERBOX );
	}


	sDBRequest_Amber_Box_Select_Item_Get clMsg( id, pclchar->pclCI->GetPersonID(), pclInfo->m_clAmberboxItem.m_siSlot, 
												siUseItemPos,					&clUseItem,
												bCanPile ? siGetItemPos : -1,   &clGetItem,	bCanPile );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWSELECTBOX_SELECT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NewSelectBox_Select* pclInfo = (cltGameMsgRequest_NewSelectBox_Select*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	SI16 siSelectIndex = pclInfo->m_siSelectIndex;
	if ( (0 >= siSelectIndex) || (MAX_NEWSELECTBOX_SELECTINDEX < siSelectIndex) )
	{
		return;
	}

	if ( NULL == pclNewSelectBoxMgr )
	{
		return;
	}

	SI16 siBoxItemUnique = pclInfo->m_siBoxItemUnique;
	switch ( siBoxItemUnique )
	{
		case ITEMUNIQUE(13340):
			{
				// 원래는 선택 인덱스 별로 아이템을 받아와야 하지만 다시한번 랜덤하게 선택하게 한다 (사양변경 그냥 유저가 선택한 인덱스로)
				// siSelectIndex = (rand() % MAX_NEWSELECTBOX_SELECTINDEX) + 1; 
			}
			break;
		default:	{	return;	}
	}

	// 이곳에 선택 인덱스에 따른 받을 아이템 목록을 받아온다
	//-------------------------------------------------------------------------------------
	// 실제 넘오는 값이나 서버에 저장되는 값은 시작 인덱스가 1이다
	// 하지만 아이템정보는 0부터 저장되어 있기때문에 -1을 해주는 것이다
	// 주의하도록해주세요
	//-------------------------------------------------------------------------------------
	SI16 siRealSelectIndex	= (siSelectIndex-1); 
	SI16 siItemList[MAX_NEWSELECTBOX_ITEM_LIST] = { 0, };
	if ( false == pclNewSelectBoxMgr->GetItemListFromIndex(siRealSelectIndex, siItemList) )
	{
		return;
	}
	
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	// 인벤에 아이템이 없는 상태로 버튼을 누를수도 있기 때문에 일단 검사
	SI32 siBoxItemPos = pclchar->pclCI->clCharItem.FindItemInv( siBoxItemUnique );
	if ( PERSONITEM_INV0 > siBoxItemPos )
	{
		return;
	}

	// 아이템 넣을 인벤이 없다면 상자 자체가 열리지가 않는다
	if ( false == pclchar->IsBlankInv(1, INVMODE_ALL) )
	{
		return;
	}

	// 클라이언트 조작을 방지하고자 선택한 값을 서버에 저장
	pclchar->m_siNewSelectBoxValue = siSelectIndex;

	cltGameMsgResponse_NewSelectBox_Select clInfo( siBoxItemUnique, siSelectIndex, siItemList );
	cltMsg clMsg( GAMEMSG_RESPONSE_NEWSELECTBOX_SELECT, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWSELECTBOX_GETITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NewSelectBox_GetItem* pclInfo = (cltGameMsgRequest_NewSelectBox_GetItem*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( NULL == pclNewSelectBoxMgr )
	{
		return;
	}

	SI16 siSelectIndex = pclInfo->m_siSelectIndex;
	if ( (0 >= siSelectIndex) || (MAX_NEWSELECTBOX_SELECTINDEX < siSelectIndex) )
	{
		return;
	}

	// 처음에 선택했던 값과 다르다면
	if ( pclchar->m_siNewSelectBoxValue != siSelectIndex )
	{
		return;
	}
	
	SI16 siBoxItemUnique = pclInfo->m_siBoxItemUnique;
	switch ( siBoxItemUnique )
	{
		case ITEMUNIQUE(13340):
			{
			}
			break;
		default:	{	return;	}
	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	//-----------------------------------------------------------------------
	// 이곳에 siSelectIndex값에 따른 랜덤 아이템을 갖고 오는 소스를 만든다
	//-----------------------------------------------------------------------
	SI16 siRealSelectIndex	= (siSelectIndex-1); 
	cltBoxItem clGetBoxItem;
	if ( false == pclNewSelectBoxMgr->GetRandItemFromIndex(siRealSelectIndex, &clGetBoxItem) )
	{
		return;
	}

	SI16 siGetItemUnique	= clGetBoxItem.GetItemUnique();
	SI16 siGetItemNum		= clGetBoxItem.GetItemNum();
	SI32 siGetItemUseDate	= clGetBoxItem.GetUseDate();
	SI16 siGetItemNotice	= clGetBoxItem.GetCanNotice();

	//-----------------------------------------------------------------------
	// 상자 아이템 정보 얻어오기
	//-----------------------------------------------------------------------
	SI16 siBoxItemPos = pclchar->pclCI->clCharItem.FindItemInv( siBoxItemUnique );
	if ( PERSONITEM_INV0 > siBoxItemPos )
	{
		return;
	}
	
	cltItem clBoxItem;
	clBoxItem.Set( &pclchar->pclCI->clCharItem.clItem[siBoxItemPos] );
	clBoxItem.SetItemNum( 1 );	// 1개만 소모된다

	//-----------------------------------------------------------------------
	// 받는 아이템 만들기
	//-----------------------------------------------------------------------
	SI32 siRef = pclItemManager->FindItemRefFromUnique( siGetItemUnique );
	if ( 0 >= siRef )
	{
		return;
	}
	SI32 siUseDate = pclItemManager->pclItemInfo[siRef]->clItem.uiDateUseDay;
	if ( 0 < siGetItemUseDate )
	{
		siUseDate = siGetItemUseDate;
	}
	
	cltItem clGetItem;
	clGetItem.Init();

	SI16 siRareSwitch = 0;
	if ( false == pclItemManager->MakeRandItemUnique(siGetItemUnique, &clGetItem, 0, 0, &siRareSwitch, 0, 0, 0, 0, 0, siUseDate) )
	{
		return;
	}

	clGetItem.SetItemNum( siGetItemNum );

	// 다시 밀봉 해준다
	clGetItem.uiDateYear	= 0;
	clGetItem.uiDateMonth	= 0;
	clGetItem.uiDateDay		= 0;

	//-----------------------------------------------------------------------
	// 아이템을 실제 받을 수 있는지 검사
	//-----------------------------------------------------------------------
	SI16	siGetItemPos	= 0;
	SI32	siReturn		= 0;
	cltItem	clToItem;
	clToItem.Init();

	if( false == pclchar->CanAddInv(INVMODE_ALL, &clGetItem, &siGetItemPos, &clToItem, &siReturn) )
	{ 
		// 아이템을 넣을 수 없다는 메시지를 여기에 넣는다
		return;
	}
	
	bool bGetItemCanPile = clGetItem.CanPile( pclItemManager );
	
	//-----------------------------------------------------------------------
	// DB로 보낸다
	//-----------------------------------------------------------------------
	sDBRequest_NewSelectBox_Item_Get clMsg( pclchar->pclCI->GetPersonID(), id, siSelectIndex,
										&clBoxItem, siBoxItemPos,
										&clGetItem,	siGetItemPos, bGetItemCanPile, siGetItemNotice );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWNPCREPAIR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NewNPCRepair* pclInfo = (cltGameMsgRequest_NewNPCRepair*)pclMsg->cData;
	if ( NULL == pclInfo )											return;

	if( pclCM->IsValidID(id) == false )								return;
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if( pclchar == NULL )											return;

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	SI32 itempos = pclInfo->m_siPos;
	if( itempos < 0 || itempos >= MAX_ITEM_PER_PERSON )				return;

	cltItem beforeitem;
	cltItem* pclitem = &pclInfo->m_clItem;
	cltItem* pclorgitem = &pclchar->pclCI->clCharItem.clItem[itempos];
	if( pclitem->siUnique <= 0 || pclitem->siItemNum <= 0 )			return;
	if( pclorgitem->siUnique <= 0 || pclorgitem->siItemNum <= 0 )	return;

	beforeitem.Set( pclitem );
	// 주어진 아이템과 같은 아이템인지 확인한다. 
	if( pclitem->IsSame(pclorgitem, false) == false )				return;

	// 수리 불가능한 아이템이어도 실패.
	if( pclItemManager->CanRepairItem(pclitem) == false )			return;

	GMONEY price = pclItemManager->RepairPrice( pclitem );
	if( price != pclInfo->m_money )
	{
		// 수리비가 올바르지 않습니다.
		return;
	}

	RepairItemExtraMaterial		kMaterial;
	// 수리석 체크
	SI32 stonepos = pclInfo->m_Material[0].siStgPos;
	if( stonepos < 0 || stonepos >= MAX_ITEM_PER_PERSON )			return;
	cltItem* pclstoneitem = &pclchar->pclCI->clCharItem.clItem[stonepos];
	if( pclstoneitem->siUnique <= 0 || pclstoneitem->siItemNum <= 0 )	return;
	if( pclItemManager->IsRepairStone(pclstoneitem) == false )		return;
	if( pclstoneitem->siItemNum < pclItemManager->NeedRepairStoneNum(pclitem) )	
	{
		// 수리석 개수 부족
		return;
	}
	kMaterial[0].Set( &pclInfo->m_Material[0] );
	kMaterial[0].siUseItemNum = pclItemManager->NeedRepairStoneNum(pclitem);

	SI32 maxdur = pclitem->GetMaxDurability( pclItemManager );
	SI32 curdur = pclitem->GetDurability( );
	SI32 decmaxdur = GetGlobalValue( "GV_Decrease_MaxDurability" );

	// 축복의 돌 체크
	bool bHasBlessingStone = false;
	for( SI32 i=1; i<pclInfo->m_Material.GetSize(); i++ )
	{
		SI32 siBlessingStoneNum = pclItemManager->NeedBlessingStoneNum( pclitem, ITEMUNIQUE(pclInfo->m_Material[i].siItemUnique) );
		if( siBlessingStoneNum > 0 && siBlessingStoneNum <= pclInfo->m_Material[i].siUseItemNum )
		{
			kMaterial[i].Set( &pclInfo->m_Material[i] );
			kMaterial[i].siUseItemNum = siBlessingStoneNum;

			bHasBlessingStone = true;
		}
	}

	// 감소하는 최대내구도가 현재 최대 내구도보다 크거나 같을경우는 무조건 축복의 돌이 있어야 한다
	if ( (decmaxdur >= maxdur) && (false == bHasBlessingStone) )
	{
		return;
	}

	// 축복의 돌이 있기 때문에 최대 내구도가 차감되지 않는다.
	if( bHasBlessingStone )			decmaxdur = 0;

	// 새로운 최대 내구도 값 설정
	SI32 newmaxdur = maxdur - decmaxdur;
	if( newmaxdur <= 0 )
	{
		newmaxdur = 0;
	}

	pclitem->SetMaxDurability( pclItemManager, newmaxdur );
	pclitem->SetDurability( pclItemManager, pclitem->GetMaxDurability( pclItemManager ) );

	sDBRequest_RepairItem clRepairItem( id, pclchar->pclCI->GetPersonID(), itempos, &beforeitem, pclitem, kMaterial, pclInfo->m_money, 0, ITEMREPAIR_MODE_NPC );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clRepairItem);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWITEMREPAIR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NewItemRepair* pclInfo = (cltGameMsgRequest_NewItemRepair*)pclMsg->cData;
	if ( NULL == pclInfo )											return;

	if( pclCM->IsValidID(id) == false )								return;
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if( pclchar == NULL )											return;

	SI32 siItemCharID = pclCM->GetIDFromPersonID( pclInfo->m_siItemPersonID );
	cltCharServer* pclitemchar = pclCM->GetCharServer( siItemCharID );
	if( pclitemchar == NULL )										return;

	pclchar->ItemUseReserveAction();
	pclchar->BulletUseReserveAction();

	pclitemchar->ItemUseReserveAction();
	pclitemchar->BulletUseReserveAction();

	// 수리요청 취소는 먼저 처리하고 끝내자.
	switch( pclInfo->m_siType )
	{
	case ITEMREPAIR_MODE_CANCEL:		// 수리요청 취소
		{
			pclchar->SetRepairShopData( false, NULL );

			cltGameMsgResponse_NewItemRepair clResult( 1, pclitemchar->GetCharUnique(), &pclInfo->m_clItem, pclInfo->m_siType );
			cltMsg clNetMsg1( GAMEMSG_RESPONSE_NEWITEMREPAIR, sizeof(clResult), (BYTE*)&clResult );
			pclchar->SendNeighbourMsg( &clNetMsg1, true );
			return;
		}
		break;
	}		

	// 무효한 범위의 아이템 위치가 오면 리턴. 
	SI32 itempos = pclInfo->m_siPos;
	if( itempos < 0 || itempos >= MAX_ITEM_PER_PERSON )				return;

	cltItem beforeitem;
	cltItem* pclitem = &pclInfo->m_clItem;
	cltItem* pclorgitem = &pclitemchar->pclCI->clCharItem.clItem[itempos];
	if( pclitem->siUnique <= 0 || pclitem->siItemNum <= 0 )			return;
	if( pclorgitem->siUnique <= 0 || pclorgitem->siItemNum <= 0 )	return;

	beforeitem.Set( pclitem );
	// 주어진 아이템과 같은 아이템인지 확인한다. 
	if( pclitem->IsSame(pclorgitem, false) == false )				return;

	// 수리석 체크
	SI32 stonepos = pclInfo->m_Material[0].siStgPos;
	if( stonepos < 0 || stonepos >= MAX_ITEM_PER_PERSON )			return;
	cltItem* pclstoneitem = &pclitemchar->pclCI->clCharItem.clItem[stonepos];
	if( pclstoneitem->siUnique <= 0 || pclstoneitem->siItemNum <= 0 )	return;

	// 최대 금액이 GV_Repair_MaxRequestMoney를 넘을 수는 없다
	GMONEY gLimitePrice = pclClient->GetGlobalValue( TEXT("GV_Repair_MaxRequestMoney") );
	if ( gLimitePrice < pclInfo->m_money )
	{
		return;
	}

	RepairItemExtraMaterial		kMaterial;
	kMaterial[0].Set( &pclInfo->m_Material[0] );

	SI32 maxdur = pclitem->GetMaxDurability( pclItemManager );
	SI32 curdur = pclitem->GetDurability( );
	SI32 decmaxdur = GetGlobalValue( "GV_Decrease_MaxDurability" );

	// 최대 내구도 회복일 경우
	if ( ITEMREPAIR_MODE_GROWUP_MAX == pclInfo->m_siType )
	{
		if ( false == pclItemManager->CanMaxRepairItem(pclitem) )						return;
		if ( false == pclItemManager->IsMaxRepairStone(pclstoneitem) )					return;
		if ( 0 >= pclstoneitem->siItemNum )												return;

		kMaterial[0].siUseItemNum = 1;
	}
	else
	{
		// 수리 불가능한 아이템이어도 실패.
		if( pclItemManager->CanRepairItem(pclitem) == false )							return;
		// 수리할 수 있는가? - 직접 수리하는 경우에만 체크한다.
		if( pclInfo->m_siType == 1 && pclitemchar->CanRepairItem( pclitem ) == false )	return;
		if( pclItemManager->IsRepairStone(pclstoneitem) == false )						return;
		if( pclstoneitem->siItemNum < pclItemManager->NeedRepairStoneNum(pclitem) )		return;

		kMaterial[0].siUseItemNum = pclItemManager->NeedRepairStoneNum(pclitem);

		

		// 축복의 돌 체크
		bool bHasBlessingStone = false;
		for( SI32 i=1; i<pclInfo->m_Material.GetSize(); i++ )
		{
			SI32 siBlessingStoneNum = pclItemManager->NeedBlessingStoneNum( pclitem, ITEMUNIQUE(pclInfo->m_Material[i].siItemUnique) );
			if( siBlessingStoneNum > 0 && siBlessingStoneNum <= pclInfo->m_Material[i].siUseItemNum )
			{
				kMaterial[i].Set( &pclInfo->m_Material[i] );
				kMaterial[i].siUseItemNum = siBlessingStoneNum;

				bHasBlessingStone = true;
			}
		}

		// 내구도 수리시 최대 내구도가 GV_Decrease_MaxDurability보다 적다면 축복의 돌 없이는 수리 안됨
		switch ( pclInfo->m_siType )
		{
		case ITEMREPAIR_MODE_SELF:
		case ITEMREPAIR_MODE_REQUEST_PRECESS:
			{
				if ( (decmaxdur >= maxdur) && (false == bHasBlessingStone) )
				{
					return;
				}
			}
			break;
		}

		// 축복의 돌이 있기 때문에 최대 내구도가 차감되지 않는다.
		if( bHasBlessingStone )			decmaxdur = 0;
	}

	switch( pclInfo->m_siType )
	{
	case ITEMREPAIR_MODE_SELF:		// 직접 수리
		{
			// 내구도 회복일경우
			SI32 newmaxdur = maxdur - decmaxdur;
			if( newmaxdur <= 0 )
			{
				newmaxdur = 0;
			}
			pclitem->SetMaxDurability( pclItemManager, newmaxdur );
			pclitem->SetDurability( pclItemManager, pclitem->GetMaxDurability( pclItemManager ) );

			sDBRequest_RepairItem clRepairItem( id, pclitemchar->pclCI->GetPersonID(), itempos, &beforeitem, pclitem, kMaterial, pclInfo->m_money, 0, pclInfo->m_siType );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clRepairItem);
		}
		break;

	case ITEMREPAIR_MODE_REQUEST:		// 수리 요청
		{
			// 이용권 티켓
			SI32 siRepairRequestTicketUnique	= Const_RepairRequestTicket_ItemUnique;
			SI16 siRepairRequestTicketNum		= 1;

			SI32 siRepairRequestTicketPos		= pclchar->pclCI->clCharItem.GetItemPos( siRepairRequestTicketUnique );
			if ( (PERSONITEM_INV0 > siRepairRequestTicketPos) || ((PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON) <= siRepairRequestTicketPos) )
			{
				SendServerResponseMsg( 0, SRVAL_REPAIR_REQUEST_FAIL, 0, 0, id );
				return;
			}
			
			cltItem clRepairRequestTicket;
			
			clRepairRequestTicket.Set( &pclchar->pclCI->clCharItem.clItem[siRepairRequestTicketPos] );
			clRepairRequestTicket.SetItemNum( 1 );
			
			// 티켓 아이템 사용 되도록 한다
			sDBRequest_UseItem stPacket_UseItem( id, pclchar->pclCI->GetPersonID(), siRepairRequestTicketPos, &clRepairRequestTicket, USEITEM_REASON_NONE );
			pclGameDBProcess->SendMsg(DBSELECT_BASE, (sPacketHeader *)&stPacket_UseItem);
			
			cltShopRepairData clShopRepairData;

			clShopRepairData.m_siPersonID = pclchar->pclCI->GetPersonID();			
			StringCchCopy(clShopRepairData.m_szAccountID, sizeof(clShopRepairData.m_szAccountID), pclchar->pclCI->clBI.szAccountID );

			clShopRepairData.m_siRepairItemPos	= itempos;
			clShopRepairData.m_clRepairItem.Set( pclitem );

			clShopRepairData.m_siPrice			= pclInfo->m_money;
			for( SI32 i=0; i<pclInfo->m_Material.GetSize(); i++ )
			{
				clShopRepairData.m_kMaterial[i].Set( &pclInfo->m_Material[i] );
			}
			pclchar->SetRepairShopData( true, &clShopRepairData );

			cltGameMsgResponse_NewItemRepair clResult( 1, pclchar->GetCharUnique(), &pclInfo->m_clItem, pclInfo->m_siType );
			cltMsg clNetMsg1( GAMEMSG_RESPONSE_NEWITEMREPAIR, sizeof(clResult), (BYTE*)&clResult );
			pclchar->SendNetMsg( (sPacketHeader*)&clNetMsg1 );

			cltGameMsgResponse_NewItemRepairRequestInfo clMsgBanner(pclchar->pclCI->GetPersonID(), pclchar->GetCharUnique(), 
				pclchar->m_pclShopRepairData->m_siRepairItemPos,
				&pclchar->m_pclShopRepairData->m_clRepairItem,
				pclchar->m_pclShopRepairData->m_siPrice,
				pclchar->m_pclShopRepairData->m_kMaterial,
				NEWITEMREPAIR_INFO_BANNER );		// 배너정보를 주변 캐릭터에게 보낸다

			cltMsg clNetMsg2( GAMEMSG_RESPONSE_NEWITEMREPAIRREQUESTINFO, sizeof(clMsgBanner), (BYTE*)&clMsgBanner );
			pclchar->SendNeighbourMsg( &clNetMsg2, true );

			cltGameMsgResponse_NewItemRepairRequestInfo clMsgInfo(	pclchar->pclCI->GetPersonID(), pclchar->GetCharUnique(), 
				pclchar->m_pclShopRepairData->m_siRepairItemPos,
				&pclchar->m_pclShopRepairData->m_clRepairItem,
				pclchar->m_pclShopRepairData->m_siPrice,
				pclchar->m_pclShopRepairData->m_kMaterial,
				NEWITEMREPAIR_INFO_ITEM );		// 수리아이템 정보를 보낸다.
			cltMsg clNetMsg3( GAMEMSG_RESPONSE_NEWITEMREPAIRREQUESTINFO, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
			pclchar->SendNetMsg( (sPacketHeader*)&clNetMsg3 );
		}
		break;
	case ITEMREPAIR_MODE_REQUEST_PRECESS:		// 의뢰수리
		{
			// 새로운 최대 내구도 값 설정
			SI32 newmaxdur = maxdur - decmaxdur;
			if( newmaxdur <= 0 )
			{
				newmaxdur = 0;
			}
			pclitem->SetMaxDurability( pclItemManager, newmaxdur );
			pclitem->SetDurability( pclItemManager, pclitem->GetMaxDurability( pclItemManager ) );

			sDBRequest_RepairItem clRepairItem( id, pclitemchar->pclCI->GetPersonID(), itempos, &beforeitem, pclitem, kMaterial, pclInfo->m_money, pclchar->pclCI->GetPersonID(), pclInfo->m_siType );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clRepairItem);
		}
		break;

	case ITEMREPAIR_MODE_CANCEL:		// 수리 요청 취소는 위에서 먼저 처리했다.
		{
		}
		break;

	case ITEMREPAIR_MODE_GROWUP_MAX:// 최대 내구도 회복일 경우
		{
			SI32 siGV_Repair_MaxDurability	= (SI32)GetGlobalValue( TEXT("GV_Repair_MaxDurability") );
			SI32 siGV_MaxDurability			= (SI32)GetGlobalValue( TEXT("GV_MaxDurability") );
			SI32 siNewMaxDur				= pclitem->GetMaxDurability( pclItemManager ) + siGV_Repair_MaxDurability; 

			if ( siGV_MaxDurability < siNewMaxDur )
			{
				siNewMaxDur = siGV_MaxDurability;
			}

			pclitem->SetMaxDurability( pclItemManager, siNewMaxDur );

			sDBRequest_RepairItem clRepairItem( id, pclitemchar->pclCI->GetPersonID(), itempos, &beforeitem, pclitem, kMaterial, pclInfo->m_money, 0, pclInfo->m_siType );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clRepairItem);

		}
		break;

	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_NewItemRepairRequestInfo* pclInfo = (cltGameMsgRequest_NewItemRepairRequestInfo*)pclMsg->cData;
	if ( NULL == pclInfo )											return;

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if( pclchar == NULL )											return;

	SI32 siCharID_Request = pclCM->GetIDFromCharUnique( pclInfo->m_siCharUnique );
	cltCharServer* pclchar_request = pclCM->GetCharServer( siCharID_Request );
	if( pclchar_request == NULL )									return;

	if( pclchar_request->m_pclShopRepairData == NULL )				return;
	if( pclchar_request->bRepairShopSwitch == false )				return;

	cltGameMsgResponse_NewItemRepairRequestInfo clMsg( pclchar_request->pclCI->GetPersonID(), pclchar_request->GetCharUnique(), 
		                                               pclchar_request->m_pclShopRepairData->m_siRepairItemPos,
													   &pclchar_request->m_pclShopRepairData->m_clRepairItem,
													   pclchar_request->m_pclShopRepairData->m_siPrice,
													   pclchar_request->m_pclShopRepairData->m_kMaterial,
													   pclInfo->m_siType );
	cltMsg clNetMsg( GAMEMSG_RESPONSE_NEWITEMREPAIRREQUESTINFO, sizeof(clMsg), (BYTE*)&clMsg );
	pclchar->SendNetMsg( (sPacketHeader*)&clNetMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_EQUIPMENTCREATE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_EquipmentCreate* pclinfo = (cltGameMsgRequest_EquipmentCreate*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if(pclchar == NULL)	return;

	SI32 GMref = pclGMManager->FindUniqueFromOrder(TEXT("seq"));
	SI32 gamemasterlevel = pclchar->GetGameMasterLevel();

	if(GMref < 1 || GMref >= MAX_GMINFO_NUMBER)	return;

	// 운영자 권한이 없으면 실행 못한다.
	if(gamemasterlevel < pclGMManager->pclGMInfo[GMref]->siNeedGMLevel)	return;

	SI32 ref = 0;
	UI16 usedate = 0;

	// 무기류
	if(pclinfo->m_siWeaponUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siWeaponPower1, pclinfo->m_siWeaponPower2, pclinfo->m_siWeaponPower3, pclinfo->m_siWeaponPower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siWeaponUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siWeaponUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID(), 0, 0, 
			            pclinfo->m_siWeaponProPerty, pclinfo->m_siWeaponEnchant, pclinfo->m_siWeaponSlot );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], ItemProPerty : [%d], ItemEnchant: [%d], ItemSlot : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siWeaponUnique, pclinfo->m_siWeaponProPerty, pclinfo->m_siWeaponEnchant, pclinfo->m_siWeaponSlot, pclinfo->m_siWeaponPower1, pclinfo->m_siWeaponPower2, pclinfo->m_siWeaponPower3, pclinfo->m_siWeaponPower4);
	}

	// 투구
	if(pclinfo->m_siHelmetUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siHelmetPower1, pclinfo->m_siHelmetPower2, pclinfo->m_siHelmetPower3, pclinfo->m_siHelmetPower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siHelmetUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siHelmetUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID(), 0, 0, 
			            pclinfo->m_siHelmetProPerty, pclinfo->m_siHelmetEhchant, pclinfo->m_siHelmetSlot );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], ItemProPerty : [%d], ItemEnchant: [%d], ItemSlot : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siHelmetUnique, pclinfo->m_siHelmetProPerty, pclinfo->m_siHelmetEhchant, pclinfo->m_siHelmetSlot, pclinfo->m_siHelmetPower1, pclinfo->m_siHelmetPower2, pclinfo->m_siHelmetPower3, pclinfo->m_siHelmetPower4);
	}

	// 갑옷
	if(pclinfo->m_siArmorUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siArmorPower1, pclinfo->m_siArmorPower2, pclinfo->m_siArmorPower3, pclinfo->m_siArmorPower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siArmorUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siArmorUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID(), 0, 0, 
			            pclinfo->m_siArmorProPerty, pclinfo->m_siArmorEnchant, pclinfo->m_siArmorSlot );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], ItemProPerty : [%d], ItemEnchant: [%d], ItemSlot : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siArmorUnique, pclinfo->m_siArmorProPerty, pclinfo->m_siArmorEnchant, pclinfo->m_siArmorSlot, pclinfo->m_siArmorPower1, pclinfo->m_siArmorPower2, pclinfo->m_siArmorPower3, pclinfo->m_siArmorPower4);
	}

	// 허리띠
	if(pclinfo->m_siBeltUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siBeltPower1, pclinfo->m_siBeltPower2, pclinfo->m_siBeltPower3, pclinfo->m_siBeltPower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siBeltUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siBeltUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID(), 0, 0, 
			            pclinfo->m_siBeltProPerty, pclinfo->m_siBeltEnchat, pclinfo->m_siBeltSlot );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], ItemProPerty : [%d], ItemEnchant: [%d], ItemSlot : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siBeltUnique, pclinfo->m_siBeltProPerty, pclinfo->m_siBeltEnchat, pclinfo->m_siBeltSlot, pclinfo->m_siBeltPower1, pclinfo->m_siBeltPower2, pclinfo->m_siBeltPower3, pclinfo->m_siBeltPower4);
	}

	// 신발
	if(pclinfo->m_siShoeUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siShoePower1, pclinfo->m_siShoePower2, pclinfo->m_siShoePower3, pclinfo->m_siShoePower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siShoeUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siShoeUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID(), 0, 0, 
			            pclinfo->m_siShoeProPerty, pclinfo->m_siShoeEnchant, pclinfo->m_siShoeSlot );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], ItemProPerty : [%d], ItemEnchant: [%d], ItemSlot : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siShoeUnique, pclinfo->m_siShoeProPerty, pclinfo->m_siShoeEnchant, pclinfo->m_siShoeSlot, pclinfo->m_siShoePower1, pclinfo->m_siShoePower2, pclinfo->m_siShoePower3, pclinfo->m_siShoePower4);
	}

	// 망토
	if(pclinfo->m_siMantleUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siMantlePower1, pclinfo->m_siMantlePower2, pclinfo->m_siMantlePower3, pclinfo->m_siMantlePower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siMantleUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siMantleUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID() );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siMantleUnique, pclinfo->m_siMantlePower1, pclinfo->m_siMantlePower2, pclinfo->m_siMantlePower3, pclinfo->m_siMantlePower4);
	}

	// 목걸이
	if(pclinfo->m_siNeckUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siNeckLacePower1, pclinfo->m_siNeckLacePower2, pclinfo->m_siNeckLacePower3, pclinfo->m_siNeckLacePower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siNeckUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siNeckUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID() );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siNeckUnique, pclinfo->m_siNeckLacePower1, pclinfo->m_siNeckLacePower2, pclinfo->m_siNeckLacePower3, pclinfo->m_siNeckLacePower4);
	}

	// 반지
	if(pclinfo->m_siRingUnique > 0)
	{
		pclchar->SetRareItemRate(0, 0, 0, 0);
		pclchar->SetRareItemRate(pclinfo->m_siRingPower1, pclinfo->m_siRingPower2, pclinfo->m_siRingPower3, pclinfo->m_siRingPower4);
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->m_siRingUnique);
		if (ref <= 0)	return;
		usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		pclCM->GiveItem(id, pclinfo->m_siRingUnique, 1, GIVEITEM_MODE_SEQGMCOMMAND, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, pclchar->GetID() );

		// 운영자명령어 로그에 남긴다.
		pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] Use seq : ItemUnique : [%d], RareItemRate : [%d] [%d] [%d] [%d]"), 
			pclCM->CR[id]->GetName(), pclinfo->m_siRingUnique, pclinfo->m_siRingPower1, pclinfo->m_siRingPower2, pclinfo->m_siRingPower3, pclinfo->m_siRingPower4);
	}	
}
// [성웅] 레벨 보상 상자에서 아이템을 준다 
void cltServer::DoMsg_GAMEMSG_REQUEST_OPEN_LEVEL_LIMIT_BOX(cltMsg* pclMsg, SI32 id)
{
	if ( pclClient->IsCountrySwitch(Switch_LevelLimitBox) == false)		return	;

	cltGameMsgRequest_UseLimitLevleBox* pclInfo = (cltGameMsgRequest_UseLimitLevleBox*)pclMsg->cData;
	if ( NULL == pclInfo )											return;

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if( pclchar == NULL )											return;

	SI32 siUse_Box_Item_Unique	=	0	;
	SI32 siUse_Box_OpenLevel	=	0	;
	SI32 siUse_Box_Total_itemnum	=	0	;	// 주는 돈을 빼고 나머지 주는 아이템의 총 갯수


	siUse_Box_Item_Unique = pclInfo->m_siOpenItemBox	;	// 사용한 아이템 상자 
	if ( siUse_Box_Item_Unique <= 0 )	return	;
	if(pclClient->pclItemManager->pclRewardItemLimitLevelForChina == NULL )	return	;
	if( pclItemManager->pclRewardItemLimitLevelForChina->IsRewardBox_Unique(siUse_Box_Item_Unique )	== false )		return	;
	siUse_Box_OpenLevel = pclItemManager->pclRewardItemLimitLevelForChina->GetOpenLevel( siUse_Box_Item_Unique )	;
	if ( siUse_Box_OpenLevel < 0 )	return	;
	if(pclchar->pclCI->clIP.siLevel	<	siUse_Box_OpenLevel)	// 오픈 레벨 이하면 리턴 한다 
	{
		SendServerResponseMsg(0, SRVAL_FAIL_OPEN_LEVELLIMTBOX_NOT_OVER_LEVEL, siUse_Box_OpenLevel, 0, id); 
		// 열수 있는레벨이 안됐다
		return	;
	}
	siUse_Box_Total_itemnum	=	pclItemManager->pclRewardItemLimitLevelForChina->GetTotalItemNum(siUse_Box_Item_Unique)	;
	if ( siUse_Box_Total_itemnum < 0 )	return	;
	if( false == pclchar->IsBlankInv(siUse_Box_Total_itemnum,INVMODE_ALL )	)		
	{
		SendServerResponseMsg(0, SRVAL_FAIL_OPEN_LEVELLIMTBOX_NOT_NOT_ENOUGH_INV, siUse_Box_Total_itemnum, 0, id); 
		// 인벤토리에 빈공간이 부족 하다 .
		return	;
	}
	// 해당 아이을 가지고 있나 검색 한다 
	SI32 siitempos = pclchar->pclCI->clCharItem.GetItemPos( siUse_Box_Item_Unique )	;
	if ( siitempos <= 0 )	return	;

	BoxData RewardIteminfo	;
	ZeroMemory(&RewardIteminfo,sizeof(BoxData))	;
	
	for ( SI32 siindex = 0 ; siindex < MAX_REWARDITEMBOX_NUM ; siindex++ ) 
	{
		if( pclItemManager->pclRewardItemLimitLevelForChina->GetBoxUnique(siindex) ==  siUse_Box_Item_Unique )
		{
			memcpy(&RewardIteminfo,&pclItemManager->pclRewardItemLimitLevelForChina->m_RewardITemBoxLinmitLevel_ForNHNChina[siindex],sizeof(BoxData));
        }
	}
	if ( RewardIteminfo.siBoxitemunique <= 0 ) return	;
	//상자 아이템 사용한다. 
	cltItem clitem;
	SI32 pos	=	0	;	
	SI32 Boxref = pclClient->pclItemManager->FindItemRefFromUnique(siUse_Box_Item_Unique);
	if(Boxref > 0)	// 박스를 사용 한다 
	{
		clitem.Set(&pclClient->pclItemManager->pclItemInfo[Boxref]->clItem);
		clitem.siItemNum = 1;
		pos = pclchar->pclCI->clCharItem.FindItemInv(&clitem, false);
		if(pos >= 0)
		{
			sDBRequest_UseItem clMsg(id, pclchar->pclCI->GetPersonID(), pos, &clitem,USEITEM_REASON_NONE);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
	}
	// 돈을 준다
	sDBRequest_ChangeMoney clMsg(id, pclchar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_LEVELLIMITBOX_REWARD, RewardIteminfo.gRewardMoney);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	// 빈공간도 충분 하고 각 아이템을 준다
	if(RewardIteminfo.siRewardItemUnique_1	> 0 && RewardIteminfo.siRewardItemNum_1 > 0 )
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(RewardIteminfo.siRewardItemUnique_1);
		if (ref > 0)
		{
			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			// 아이템을 준다
			pclCM->GiveItem(id, RewardIteminfo.siRewardItemUnique_1, RewardIteminfo.siRewardItemNum_1, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_LIMITLEVELBOX,usedate, 0 );
		}		

	}
	if(RewardIteminfo.siRewardItemUnique_2	> 0 && RewardIteminfo.siRewardItemNum_2 > 0 )
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(RewardIteminfo.siRewardItemUnique_2);
		if (ref > 0)
		{
			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			// 아이템을 준다
			pclCM->GiveItem(id, RewardIteminfo.siRewardItemUnique_2, RewardIteminfo.siRewardItemNum_2, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_LIMITLEVELBOX,usedate, 0 );
		}
	}
	if(RewardIteminfo.siRewardItemUnique_3	> 0 && RewardIteminfo.siRewardItemNum_3 > 0 )
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(RewardIteminfo.siRewardItemUnique_3);
		if (ref > 0)
		{
			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			// 아이템을 준다
			pclCM->GiveItem(id, RewardIteminfo.siRewardItemUnique_3, RewardIteminfo.siRewardItemNum_3, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_LIMITLEVELBOX,usedate, 0 );
		}		
	}
	if(RewardIteminfo.siRewardItemUnique_4	> 0 && RewardIteminfo.siRewardItemNum_4 > 0)
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(RewardIteminfo.siRewardItemUnique_4);
		if (ref > 0)
		{
			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			// 아이템을 준다
			pclCM->GiveItem(id, RewardIteminfo.siRewardItemUnique_4, RewardIteminfo.siRewardItemNum_4, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_LIMITLEVELBOX,usedate, 0 );
		}
	}
	if(RewardIteminfo.siRewardItemUnique_5	> 0 && RewardIteminfo.siRewardItemNum_5 > 0 )
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(RewardIteminfo.siRewardItemUnique_5);
		if (ref > 0)
		{
			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			// 아이템을 준다
			pclCM->GiveItem(id, RewardIteminfo.siRewardItemUnique_5, RewardIteminfo.siRewardItemNum_5, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_LIMITLEVELBOX,usedate, 0 );
		}
	}
}
//[성웅] // 죽었던 말던 어떤상황이던 아이템을 줄인다
void cltServer::DoMsg_GAMEMSG_REQUEST_JustItemUse(cltMsg* pclMsg, SI32 id)
{

	//**앤간하면 쓰지 마세요죽었을때 축복받은 부활약 사용을 위해 만든 것입니다 **//
	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;

	//	--------------------------------------------

	cltGameMsgRequest_UseJustUseItem* pclInfo = (cltGameMsgRequest_UseJustUseItem*)pclMsg->cData;
	if ( NULL == pclInfo )											return;

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if( pclchar == NULL )											return;
	SI32 itemunique = pclInfo->m_siitemunique;								if( itemunique <= 0 )	return;	// 정상 적인 아이템 유니크 인지 확인
	SI32 ref		= pclItemManager->FindItemRefFromUnique(itemunique);	if(ref <= 0)			return	;
	
	SI32 itemtype	= pclItemManager->pclItemInfo[ref]->siType;
	SI64 itematb	= pclItemManager->pclItemTypeInfo[itemtype]->siAtb;		if ( itematb == ITEMATB_OBJECT )	return	;// 오브젝트는 사용할수없다 
	SI32 pos = pclInfo->m_siitempos	;

	SI32 sihaveItempos = pclchar->pclCI->clCharItem.GetItemPos( itemunique )	;
	// 해당 위치에 아이템이 있는가 ?
	if(pos	!= sihaveItempos)	return	;

	cltItem clItem;
	clItem.Set( &pclchar->pclCI->clCharItem.clItem[ pos ] );
	clItem.SetItemNum(pclInfo->m_siitemnum);

	sDBRequest_UseItem clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pos, &clItem,USEITEM_REASON_NONE);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}