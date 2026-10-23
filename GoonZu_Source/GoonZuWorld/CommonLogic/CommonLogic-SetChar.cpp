#include "..\Client\Client.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Map\Map.h"
#include "..\Common\Order\Order.h"
#include "Char\CharManager\CharManager.h"
#include "..\Server\TraceBoot.h"

#include "MsgType-System.h"
#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-Vote.h"
#include "Feast/Feast.h"
#include "MsgType-Person.h"
#include "MsgType-Item.h"
#include "..\Common\Char\KindInfo\KindInfo.h"

// ���� �⺻�� �Ǵ� ĳ���͸� �����Ѵ�. 
bool cltCommonLogic::MakeSimpleChar(SI32 id, SI32 charunique)
{
	SI32 childcharunique	= 0;
	SI32 summonindex		= 0;
	SI32 life				= 1;
	cltIdentity				clIdentity;

	cltBufInfo clBufInfo;
	clBufInfo.Init();
	cltMakeCharInfo clinfo(KIND_SYSTEM, charunique, 1, life, 0, 0, 0, childcharunique, summonindex, 
							false, false, false, false,false,false,false ,false , 
							0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL,NULL,0, NULL, 
							NULL, NULL, false, false, 0, false, 0 ,NULL, NULL, 
							false, 0, false, false, false, false, 0, 0,NULL, false,NULL
							,0,0,0,0,0,0,0,0,0,0,false, &clBufInfo
							);
	SetChar(id, &clinfo);

	return true;
}

// �Ϲ� ĳ���͸� �����Ѵ�. 
SI32 cltCommonLogic::SetChar(SI32 id, cltMakeCharInfo* pclinfo, cltGMCommandKindInfo* pclGMCommandKindInfo)
{
	if(pclinfo == NULL)return 0;
	if(id == 0)	return 0;
	if(pclinfo->uiMakeCharKind < 0 || pclinfo->uiMakeCharKind >= MAX_KIND_NUMBER)
	{
		TraceBoot("SETCHAR REJECT id=%d kind=%d : kind out of range (MAX=%d)", id, pclinfo->uiMakeCharKind, MAX_KIND_NUMBER);
		return 0;
	}
	if( pclKindInfoSet->pclKI[ pclinfo->uiMakeCharKind ] == NULL	)
	{
		TraceBoot("SETCHAR REJECT id=%d kind=%d : pclKI[%d] is NULL", id, pclinfo->uiMakeCharKind, pclinfo->uiMakeCharKind);
		return 0;
	}

	TraceBoot("SETCHAR proceed id=%d kind=%d", id, pclinfo->uiMakeCharKind);

	cltPersonInfo			charinfo;
	cltInnerStatusInfo		clInnerStatus;

	charinfo.clBI.SetKind(pclinfo->uiMakeCharKind);
	charinfo.clBI.siHomeVillage	= pclinfo->siHomeVillageUnique;
	charinfo.clBI.SetX(pclinfo->uiX);
	charinfo.clBI.SetY(pclinfo->uiY);
	charinfo.clBI.SetMapIndex(pclinfo->uiMapIndex);

	charinfo.clBI.uiGMMode	= pclinfo->uiGMMode;
	charinfo.clBI.bPCRoom	= pclinfo->bPCRoom;

	// CMT�� ������ ĳ���ʹ� pclGMCommandKindInfo�� �ٰŷ� �����͸� ���´�.
	if(pclGMCommandKindInfo)
	{
		charinfo.clIP.SetBasicAbility(&pclGMCommandKindInfo->clBA);
	}
	else
	{
		// BasicAbility�ɷ�ġ�� cltKindInfo�� �ٰŷ� ���´�. 
		charinfo.clIP.SetBasicAbility(&pclKindInfoSet->pclKI[ pclinfo->uiMakeCharKind ]->clBA);
	}

	// �δ� ���� �ɷ�ġ �ϵ��ڵ�
	if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_RACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(5, 40));			}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_KINGRACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(150, 290));			}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_RACCOONLORD_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(100, 145));			}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_ZORRORACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(60, 120));			}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_SANTARACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(300, 431));		}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_MAGICALRACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(30, 100));		}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_RACCOONTOWER_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(350, 450));		}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_MAMARACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(1150, 2000));		}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_BIGRACCOON_AGIT") )
	{		charinfo.clIP.SetVit(RandomInt(5000, 7000));		}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_KEY_RACCOON") )
	{		charinfo.clIP.SetVit(RandomInt(250, 350));			}
	else if(pclinfo->uiMakeCharKind == (UI32)pclClient->GetUniqueFromHash("KIND_GMONBONUS") )
	{		charinfo.clIP.SetVit(RandomInt(625, 750));			}

	charinfo.clIP.SetLevel(pclinfo->uiLevel);

	//-----------------------------------
	// ������ ���� �������ͽ� ����ġ�� �ݿ��Ѵ�.  - ����2 ����.
	//-----------------------------------
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange) == false)
	{
		charinfo.clIP.SetStr(charinfo.clIP.GetStr() + (pclinfo->uiLevel - 1)*2);
		charinfo.clIP.SetDex(charinfo.clIP.GetDex() + (pclinfo->uiLevel - 1));

		if(charinfo.clIP.GetMag())
		{
			charinfo.clIP.SetMag(charinfo.clIP.GetMag() + (pclinfo->uiLevel - 1));
		}

		charinfo.clIP.SetVit(charinfo.clIP.GetVit() + (pclinfo->uiLevel - 1));
	}

	// InitCharClient() �Լ����� ������ ����ϱ� ���ؼ� ���� �����Ѵ�.
	pclCM->CR[id]->clTransFormInfo.uiNowTransFormKind	= pclinfo->uiNowTransFormKind;
	pclCM->CR[id]->clTransFormInfo.bTransFormMode		= pclinfo->bTransFormMode;

	pclCM->CR[id]->clTransFormInfo.bTransForm100Effect		= pclinfo->bTransForm100Effect;

	// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
	pclCM->CR[id]->uiBestGuild = pclinfo->uiBestGuild;

	id = SetChar1(id, pclinfo->uiMakeCharUnique, pclinfo->uiChildCharUnique, pclinfo->uiSummonIndex, &pclinfo->clIdentity, &charinfo, &pclinfo->clInnerStatus, pclinfo->siSoulGuardUnique);
	if(id)
	{
		// �־��� ü���� �ִٸ� ü���� �����ϰ�, 
		if(pclinfo->uiLife)
		{
			pclCM->CR[id]->SetLife( pclinfo->uiLife);
		}
		// �־��� ü���� ���ٸ� �ִ밪���� �����Ѵ�. 
		else
		{
			pclCM->CR[id]->SetLife( pclCM->CR[id]->clPB.GetMaxLife());
		}

		// �������� ������ �ʴ� ���̹Ƿ� �ִ밪���� �����Ѵ�. 
		pclCM->CR[id]->SetMana(pclCM->CR[id]->clPB.GetMaxMana());

		pclCM->CR[id]->siPersonalMakeItemType = pclinfo->siMakeAniType;
		pclCM->CR[id]->SetEnchantEffect(pclinfo->siMakeAniType);
	}
	// PC�̸� ��� ������ ����ϰ� �ִ´�. 
	// Ŭ���̾�Ʈ ���ΰ����Դ� ����ڵ尡 ����� �Ѵ�. 

	// Ŭ���̾�Ʈ�� ���ΰ��� ��쿡�� �ڽ��� ������ ���� �������� �ٰŷ� ���������� ���� �ϱ� ������ 
	// �̷� ���� ����ũ�� ����ؼ��� �ȵȴ�. 
	if(GameMode == GAMEMODE_CLIENT)
	{
		// �ּ��� ������ ������Ʈ �Ѵ�. 
		pclCM->CR[id]->pclCI->clBI.siHomeVillage = pclinfo->siHomeVillageUnique;
		// �б� ������ ������Ʈ �Ѵ�. 
		pclCM->CR[id]->pclCI->clBI.siSchoolUnique = pclinfo->uiSchoolUnique;
		// ������ �Է��Ѵ�. 
		pclCM->CR[id]->pclCI->clBI.uiSex = pclinfo->uiSex;


		// �������� ������Ʈ �Ѵ�. 
		pclCM->CR[id]->clJim.Init();
		pclCM->CR[id]->clJim.SetBeJimed(pclinfo->siJimCharUnique, 1);

		// �� ������ ȭ�鿡 ǥ���Ѵ�. 
		pclCM->CR[id]->SetJimShow();

		// ���ΰ��� �ƴϸ�, 
		// ������ ���� ���� ������ �ٰŷ� ĳ���͸� �����Ѵ�. 
		// ���ΰ��� ���� ���� ������ �ٰŷ� ĳ���͸� �����ؾ� �Ѵ�. 
		if(id != 1)
		{
			if(IsPC(pclinfo->uiMakeCharUnique))
			{
				pclCM->CR[id]->clPB.clShapeEquip.Set(&pclinfo->clShapeEquip);

				pclCM->CR[id]->clPB.GivenMoveSpeed	= pclinfo->uiTotalMoveSpeed;

				// ���� �����Ѵ�. 
				cltHorse* pclhorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
				if(pclhorse)	pclhorse->siHorseUnique	= pclinfo->uiHorseUnique1;
			}

		}

		// �Ӹ����� ������ ������ �Ӹ����� ��û�Ѵ�. 
		if(pclinfo->uiHeadChatSwitch)
		{
			cltGameMsgRequest_HeadChat clinfo(pclCM->CR[id]->GetCharUnique());
			cltMsg clMsg(GAMEMSG_REQUEST_HEADCHAT, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);

		}
		// ���λ����� ������ ���� ������ ��û�Ѵ�.
		if(pclinfo->uiPersonalShopSwitch)
		{
			cltGameMsgRequest_PersonalShopBanner clinfo(pclCM->CR[id]->GetCharUnique());
			cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPBANNER, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);
		}

		// [����] ���� �ý��� : ������ ������ �ٽ� ����
		if ( pclinfo->uiElectionStump )
		{
			cltGameMsgRequest_ElectionStump clinfo( pclCM->CR[id]->GetCharUnique() );
			cltMsg clMsg(GAMEMSG_REQUEST_ELECTIONSTUMP, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);
		}

		// �����Ƿڰ� ������ ������ ��û�Ѵ�.
		if( pclinfo->uiRepairShopSwitch )
		{
			cltGameMsgRequest_NewItemRepairRequestInfo clinfo(pclCM->CR[id]->GetCharUnique(), NEWITEMREPAIR_INFO_BANNER);
			cltMsg clMsg(GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);
		}

		
		//[����] ĳ���� ������ ������ ��ü ������ ���� 
		pclCM->CR[id]->pclCI->clBI.clBufInfo.SetBuf( &pclinfo->m_clBufInfo );

		// ���� ��� ��Ȳ�� ������Ʈ 
		pclCM->CR[id]->bNoFirstAttackSwitch 				= pclinfo->uiNoFirstAttackSwitch;
		// ICE ��� ��Ȳ�� Update
		pclCM->CR[id]->bNoIceAttackSwitch					= pclinfo->uiNoIceAttackSwitch;
		// ��Ʈ ����Ʈ ������Ʈ
		pclCM->CR[id]->bHeartEffectSwitch					= pclinfo->uiHeartEffectSwitch;
		// ���� Ŭ�ι� ����Ʈ ������Ʈ
		pclCM->CR[id]->bFourLeafSwitch						= pclinfo->uiFourLeafSwitch;
		((cltCharClient*)pclCM->CR[id])->siCushionUnique	= pclinfo->uiCusionSwitch;

		pclCM->CR[id]->siBufTransformKind					= pclinfo->siBufTransformKind;
		pclCM->CR[id]->siEnemUnique							= pclinfo->siEnemyUnique;

		pclCM->CR[id]->bElixirSwitch						= pclinfo->uiElixir;

		// �߰� ������ �ʿ�� �ϸ�, 
		if(pclinfo->uiRequestAddinfo)
		{
			// ������ �߰� ������ ��û�Ѵ�. 
			cltMsg clMsg(GAMEMSG_REQUEST_ADDINFO, pclCM->CR[id]->GetCharUnique());
			SendMsgToServer((sPacketHeader*)&clMsg);
		}	

		pclCM->CR[id]->m_siSetItemType = pclinfo->clShapeEquip.siCompletedSetItemIndex;
		pclCM->CR[id]->m_siSetItemStep = pclinfo->clShapeEquip.siCompletedSetItemIndexStep;

		pclCM->CR[id]->m_siSetItemType2 = pclinfo->clShapeEquip.siCompletedSetItemIndex2;
		pclCM->CR[id]->m_siSetItemStep2 = pclinfo->clShapeEquip.siCompletedSetItemIndexStep2;

		// ��� ����
		pclCM->CR[id]->pclCI->clBI.clGuildRank.Set(&pclinfo->clGuildRank);
		pclCM->CR[id]->pclCI->clPersonNaming.Set(&pclinfo->clPersonNaming);	
		// �߷�Ÿ�ε���
		if( pclCM->CR[id]->pclValentineWar )	pclCM->CR[id]->pclValentineWar->Set(&pclinfo->clValentineWarInfo);

		pclCM->CR[id]->m_uiWeaponSkillMasterEmblem = pclinfo->uiWeaponSkillMasterEmblem;
		
		if ( IsPC(pclinfo->uiMakeCharUnique) && (pclCM->CR[id]->pclMarriage) )
		{
			pclCM->CR[id]->pclMarriage->SetLoveLevel( pclinfo->siDisplayLoveLevel );
		}
		
	}	
	SI32 unique;

	unique = pclinfo->clShapeEquip.clArmour.uiUnique;

	if(unique)
	{
		pclCM->CR[id]->ChangeArmCode(/*PERSONITEM_ARMOUR*/ pclCM->CR[id]->ActiveClothes, unique);
	}
	unique = pclinfo->clShapeEquip.clHelmet.uiUnique;

	if(unique)
	{
		pclCM->CR[id]->ChangeArmCode(pclCM->CR[id]->ActiveHat, unique);
	}


	unique = pclinfo->clShapeEquip.clMantle.uiUnique;
	if(unique)
	{
		pclCM->CR[id]->ChangeArmCode(PERSONITEM_MANTLE, unique);
	}

	unique = pclinfo->clShapeEquip.clWeapon.uiUnique;
	if(unique)
	{
		pclCM->CR[id]->ChangeArmCode( pclCM->CR[id]->ActiveWeapon, unique );
	}

	//KHY - 1001 - ��µ��� 5���� ���� ����.
	if(pclinfo->siCurrentHorseRIDEHORSE == TRUE)
	{
		cltDate cldate;
		cldate.MakeFullDateFromDateVary(pclinfo->siAddStatusDate);
		SI32 ticketnum = pclinfo->siStatusTicketNum ;

		// PCK : �������� �ػ����� ������ Ż���� �����ϰ� �ϳ��� �̹����� �ٲ��. (09.08.11)
		if(pclCM->CR[id]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM_HORSE))
		{
			SI32 siTransFormHorseUnique = pclCM->CR[id]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_TRANSFORM_HORSE);
			pclCM->CR[id]->ChangeHorseCode(siTransFormHorseUnique, &cldate, ticketnum);
		}
		else
		{
			pclCM->CR[id]->ChangeHorseCode( pclinfo->uiHorseUnique1,&cldate,ticketnum);
		}
	}
	//������ ����
	if(pclinfo->siPremiumPartsNum > 0)
	{
		cltHorse* pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse()	;
		if(pclHorse != NULL)
		{
			cltDate cldate_Slot1;
			cldate_Slot1.MakeFullDateFromDateVary(pclinfo->siPremiumPartsDate_Slot1);
			pclHorse->clPremiumPartsDate_Slot1.Set(&cldate_Slot1)	;
			cltDate cldate_Slot2;
			cldate_Slot2.MakeFullDateFromDateVary(pclinfo->siPremiumPartsDate_Slot2);
			pclHorse->clPremiumPartsDate_Slot2.Set(&cldate_Slot2)	;
			cltDate cldate_Slot3;
			cldate_Slot1.MakeFullDateFromDateVary(pclinfo->siPremiumPartsDate_Slot3);
			pclHorse->clPremiumPartsDate_Slot3.Set(&cldate_Slot3)	;

			pclHorse->SetPremiumPartsUnique_Slot1(pclinfo->siPremiumPartsUnique_Slot1)	;
			pclHorse->SetPremiumPartsUnique_Slot2(pclinfo->siPremiumPartsUnique_Slot2)	;
			pclHorse->SetPremiumPartsUnique_Slot3(pclinfo->siPremiumPartsUnique_Slot3)	;
			pclinfo->siPremiumPartsNum		=	pclinfo->siPremiumPartsNum	;
		}
	}
	// ĳ���Ϳ���  �ൿ�� �����Ѵ�. 
	cltOrderDataInfo* pclorder = &pclinfo->clOrder;

	switch(pclorder->siOrder)
	{
	case ORDER_STOP:
	case ORDER_FORCESTOP:
	case ORDER_WAIT:
		OrderStop(id, pclorder->siHostMode);
		break;

	case ORDER_MOVE:
		{
			OrderMove(id, LOWORD(pclorder->siPara1), HIWORD(pclorder->siPara1),pclorder->siHostMode);
		}
		break;
	case ORDER_STRAIGHTMOVE:
		{
			OrderStraightMove(id, LOWORD(pclorder->siPara1), HIWORD(pclorder->siPara1),pclorder->siHostMode);
		}
		break;


	case ORDER_ATTACK:
		{
			SI32 attackid = cltCharCommon::CharUniqueInfo[pclorder->siPara2];
			if(pclCM->IsAlive(attackid))
			{
				OrderAttack(id, attackid, pclorder->siHostMode);
			}
		}
		break;

	case ORDER_FISH:
		{
			OrderFish(id, pclorder->siPara1, pclorder->siHostMode);
		}
		break;
	case ORDER_FARM:
		{
			OrderFarm(id, LOWORD(pclorder->siPara1), HIWORD(pclorder->siPara1),pclorder->siHostMode);
		}
		break;
	case ORDER_MINE:
		{
			OrderMine(id, LOWORD(pclorder->siPara1), HIWORD(pclorder->siPara1),pclorder->siHostMode);
		}
		break;

	case ORDER_SITDOWN:
		{
			OrderSitDown( id, pclorder->siPara1, pclorder->siHostMode );
		}
		break;

	case ORDER_DIE :
		{
			OrderDie(id,  pclorder->siHostMode);
		}
		break;

	case ORDER_ATTACK_CLIENT:
		{
			SI32 attackid = cltCharCommon::CharUniqueInfo[pclorder->siPara2];
			if(pclCM->IsAlive(attackid))
			{
				OrderAttackClient(id, attackid, pclorder->siHostMode);
			}
		}
		break;

	case ORDER_SPECIAL_ACTION:
		{
			OrderSpecialAction(id, pclorder->siPara1, pclorder->siHostMode);
		}
		break;

	default:
		MsgBox(TEXT("CommonLogic-SetChar"), TEXT("order:%d"), pclorder->siOrder);
		break;

	}
	return id;
}
// �Ϲ� ĳ���͸� �����Ѵ�. 
SI32 cltCommonLogic::SetChar1(SI32 id, SI32 charunique, SI32 childcharunique, SI32 summonindex, cltIdentity* pclidentityinfo, cltPersonInfo* pclUserCharInfo, cltInnerStatusInfo* pclInnerStatus, SI16 mySoulGuardUnique)
{

	if(id <= 0)return 0;

	cltMapCommon* pclmap = pclMapManager->GetMapPointer(pclUserCharInfo->clBI.GetMapIndex());
	if(pclmap == NULL)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("SetChar() id:%d charunique:%d mapindex:%d"),
				id, charunique, pclUserCharInfo->clBI.GetMapIndex()	);

		return 0;
	}
    
	pclCM->CR[id]->CharInit(id, charunique, childcharunique, summonindex, SOUTH, pclmap, pclidentityinfo, pclUserCharInfo, pclInnerStatus, mySoulGuardUnique);
	
	// Ŭ���̾�Ʈ�κ��� ���������� �޽����� ���� �ð��� ������Ʈ �Ѵ�. 
	pclCM->CR[id]->SetLastReceveFromClientClock(CurrentClock);

    return id;
}