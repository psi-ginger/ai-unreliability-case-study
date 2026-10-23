//---------------------------------
// 2003/8/8 김태곤
//---------------------------------

#include "../Common/CommonHeader.h"
#include "../Server/Server.h"

#include "../../Common/Map/Map.h"
#include "Char/CharManager/CharManager.h"

#include "../../../Server/AuthServer/AuthMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Village.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Guild.h"
//데일리 퀘스트 3
#include "../../../DBManager/GameDBManager_World/DBMsg-DailyQuest3.h"
#include "Skill/Skill-Manager.h"

#include "../../Common/PartyMgr/PartyObj.h"
#include "../../Common/PartyMgr/PartyMgr.h"

#include "../../common/Item/ItemCommon/cltSetItem.h"

#include "Guild/Guild.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "MsgType-Item.h"
#include "Msg/MsgType-Personalshop.h"
#include "Msg/MsgType-Quest.h"
#include "Msg/MsgType-Party.h"
//데일리 퀘스트 3
#include "Msg/MsgType-DailyQuest3.h"

#include "MsgRval-Define.h"

//------------------------------------
// Server
//------------------------------------
#include "Statistics/Statistics.h"
#include "../Minister/MinisterBase.h"
#include "../Minister/Minister-Gongzo.h"
#include "../Minister/MinisterMgr.h"

// 아이템 설정. (다목적) 
void cltServer::DoMsg_DBMSG_RESPONSE_SETITEM(sPacketHeader* pPacket)
{
	sDBResponse_SetItem* pclMsg = (sDBResponse_SetItem*)pPacket;

	if(pclMsg->siResult == 0)return ;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siPos].Set(&pclMsg->clItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// 부적 부착. . 
void cltServer::DoMsg_DBMSG_RESPONSE_ENCHANT(sPacketHeader* pPacket)
{
	sDBResponse_Enchant* pclMsg = (sDBResponse_Enchant*)pPacket;

	if(pclMsg->siResult == 0)return ;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		//--------------------------------
		// FromItem 변경 
		//--------------------------------
		if( pclMsg->siFromPos >= 0 && pclMsg->siFromPos < MAX_ITEM_PER_PERSON )
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siFromPos].Set(&pclMsg->clFromItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siFromPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clFromItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		//---------------------------------
		// ToItem 변경. 
		//---------------------------------
		if( pclMsg->siToPos >= 0 && pclMsg->siToPos < MAX_ITEM_PER_PERSON )
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToPos].Set(&pclMsg->clToItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siToPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clToItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}


		//-----------------------------------------------------------
		// 아무부적이나 장비에 발라보는 퀘스트라면,
		//-----------------------------------------------------------
		if(pclMsg->siResult == 1)
		{
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SCROLL);

		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			TCHAR str[50] = {'\0', };
			StringCchPrintf(str,  sizeof(str), "FromUniqe = %d, FromPos = %d, FromNum = %d", 
												pclMsg->clFromItem.siUnique,
												pclMsg->siFromPos,
												pclMsg->clFromItem.siItemNum);
			

			// [로그추가 : 황진성 2007. 10. 11] // 부적을 무기에 적용시킨 내역.(부적 -> 강화아이템의 위치)
			// param1	 == 아이템 유니크, 
			// param2	 == 아이템 위치, 
			// param3	 == 아이템 개수. 
			// pszParam1 == 이전 아이템 정보.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_ENCHANT_ITEM, 
								 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								 pclMsg->clToItem.siUnique, pclMsg->siToPos, pclMsg->clToItem.siItemNum, 0, 0, 
								 str, NULL);
		}
	}
}

// 인벤에서 아이템을 생산하라는 요청에 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MAKEITEMINV(sPacketHeader* pPacket)
{
	sDBResponse_MakeItemInv* pclMsg = (sDBResponse_MakeItemInv*)pPacket;

	//----------------------------
	// 아이디의 적절성을 확인한다. 
	//----------------------------
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)return ;
	if(pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)return ;

	// 주문자의 아이디를 확보한다. 
	SI32 materialcharid;
	if(pclMsg->siPersonID == pclMsg->siMaterialPersonID)
	{
		materialcharid = id;
	}
	else
	{
		materialcharid = pclCM->GetIDFromPersonID(pclMsg->siMaterialPersonID);
	}

	if( pclCM->IsValidID(materialcharid) == false)		return;
	// 아이템이 추가되는 자리에 되어 있던 예약을 해지한다. 
	cltCharServer* pclMaterialChar = (cltCharServer*)pclCM->CR[materialcharid];
	if(pclMaterialChar == NULL)return ;

	pclMaterialChar->SetInputItemReserve(pclMsg->siOutputPos, 0);


	bool bupdateinvswitch = false;
	bool bcontinueswitch = false;
	switch(pclMsg->siResult)
	{
	case 0:	return;		break;
	case 1:	
		{
			// 정상적인 성공. 
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{				cltCharServer* pclMakerChar = pclCM->GetCharServer(id);
			if(NULL == pclMakerChar)
				return;

			// [로그추가 : 황진성 2007. 10. 11] // 아이템 제작으로 생산.
			// param1 == 아이템위치.			
			// param2 == MaterialPersonIDMoney,	 
			// param3 == 주문자가 만들기를 희망한 개수, 
			// param4 == 거의 실패한 제품인가. 
			// param5 == 행운에 의한 퀄리티 향상.
			if( pclMaterialChar->bPersonalShopSwitch == true &&
				pclMaterialChar->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM )
			{
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_COMMISSION , LOGCOMMAND_INDEX_COMMISSION_CLIENT , 
					0, pclMaterialChar, NULL, pclMsg->siPersonID, pclMsg->siFee, pclMsg->siPersonIDMoney, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
					pclMsg->siOutputPos, pclMsg->siMaterialPersonIDMoney, pclMsg->siOrderNumber, pclMsg->bAlmostFailSwitch, pclMsg->siLuckQuality,
					NULL, NULL);
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_COMMISSION , LOGCOMMAND_INDEX_COMMISSION_END , 
					0, pclMaterialChar, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
					0, 0, 0, 0, 0,
					NULL, NULL);
			}
			else if( pclMakerChar->bPersonalShopSwitch == true &&
				pclMakerChar->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
			{
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_COMMISSION , LOGCOMMAND_INDEX_COMMISSION_MAKER , 
					0, pclMakerChar, NULL, pclMsg->siMaterialPersonID, pclMsg->siFee, pclMsg->siPersonIDMoney, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
					pclMsg->siOutputPos, pclMsg->siMaterialPersonIDMoney, pclMsg->siOrderNumber, pclMsg->bAlmostFailSwitch, pclMsg->siLuckQuality,
					NULL, NULL);
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_COMMISSION , LOGCOMMAND_INDEX_COMMISSION_END , 
					0, pclMaterialChar, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
					0, 0, 0, 0, 0,
					NULL, NULL);
			}
			else
			{
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_MAKE_ITEM, 
					0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siMaterialPersonID, pclMsg->siFee, pclMsg->siPersonIDMoney, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
					pclMsg->siOutputPos, pclMsg->siMaterialPersonIDMoney, pclMsg->siOrderNumber, pclMsg->bAlmostFailSwitch, pclMsg->siLuckQuality,
					NULL, NULL);
			}
			}
		}
		break;							

	case 2:								// 완성품 놓을 자리에 이미 다른 물품이 존재. 
		{
			if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
			{
				pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);

				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siOutputPos], 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
			}
			return;	
		}
		break;
	case 3:								// 수수료가 부족하여 물품 제작 실패. 
		break;
	default:
		return ;
	}

	
	
	//--------------------------
	// 인벤을 업데이트 한다. 
	//--------------------------
	if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);

	SI32 skillunique	= 0;
	SI32 skillexp		= 0;

	if(pclMsg->siAddedNum)
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

		SI32 exp = 0;

		SI32 ref = pclItemManager->FindItemRefFromUnique(pclMsg->clItem.siUnique);
		if(ref > 0)
		{
			if(pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill && pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel)
			{
				cltItemPriceUnit* pclprice	= pclItemManager->GetCurrentPrice(pclMsg->clItem.siUnique);
				if(pclprice)
				{
					// 제조 통계를 낸다. 
					pclStatisticsManager->clDailyStatistics.siMakeMoney+= pclprice->siPrice;

					skillunique = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
					if(skillunique >= 0)
					{
						SI32 siGetType = pclItemManager->pclItemInfo[ref]->siType;

						if ( pclClient->IsCountrySwitch(Switch_MakeFoodHealEXP) && ((ITEMTYPE_FOOD == siGetType) || ((ITEMTYPE_HEAL == siGetType))) )
						{
							exp = CalcExp_MakeItem_FoodHeal( pclchar, skillunique, pclMsg->clItem.siUnique, pclMsg->siAddedNum );
						}
						else if ( pclClient->IsCountrySwitch(Switch_MakeItemEXP) )
						{
							exp = CalcExp_MakeItem_New( pclchar, skillunique, pclMsg->clItem.siUnique, pclMsg->siAddedNum );
						}
						else
						{
							exp = CalcExp_MakeItem( pclchar, skillunique, pclMsg->clItem.siUnique, pclMsg->siAddedNum, pclprice->siPrice );
						}


						// 해당 기술의 현재 수준을 구한다. 
						/*SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclchar->pclCI->clCharItem.clItem);

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


						if ( (pclItemManager->GetItemAtb(pclMsg->clItem.siUnique) & ITEMATB_OBJECT) )
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
							exp	= pclItemManager->GetMakeExpFromItemPrice(pclprice->siPrice * pclMsg->siAddedNum, pclchar->pclCI->clIP.GetLevel()); 
						}*/

						// 피로방지 시스템 적용
						exp = pclchar->CalcTiredPoint( exp );

						if(pclMsg->bNPCMake)
						{
							exp = 0;
						}
						if(exp > 0)
						{
							pclchar->IncreaseExp(exp, GETEXP_REASON_MAKEITEM);
						}


						//------------------------------------
						// 기술 경험치를 높여준다. 
						//------------------------------------

						if ( pclClient->IsCountrySwitch(Switch_MakeFoodHealEXP) && ((ITEMTYPE_FOOD == siGetType) || ((ITEMTYPE_HEAL == siGetType))) )
						{
							skillexp = CalcSkillExp_MakeItem_FoodHeal( pclchar, skillunique, pclMsg->clItem.siUnique, pclMsg->siAddedNum );
						}
						else if ( pclClient->IsCountrySwitch(Switch_MakeItemEXP) )
						{
							skillexp = CalcSkillExp_MakeItem_New( pclchar, skillunique, pclMsg->clItem.siUnique, pclMsg->siAddedNum );
						}
						else
						{
							skillexp = CalcSkillExp_MakeItem( pclchar, skillunique, pclMsg->clItem.siUnique, pclMsg->siAddedNum, pclprice->siPrice );
						}


						// 등급에 따른 기본 경험치
/*						if ((pclItemManager->GetItemAtb(pclMsg->clItem.siUnique) & ITEMATB_OBJECT) )
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
							skillexp = pclItemManager->GetMakeSkillExpFromItemPrice(pclprice->siPrice * pclMsg->siAddedNum, skilllevel); 
						}*/


						////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//	[종호]			CODE BARRICADE
						//	Description :	NHNCHINA 와 이외의 국가는 제조경험치 합산 방식이 다릅니다.
						////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						if(pclClient->siServiceArea == ConstServiceArea_NHNChina) // [종호] NHNCHINA인 경우의 제조경험치 합산
						{
							SI32 siTimeBonusPercent = 0;
							SI32 siBuffBonusPercent = 0;
							SI32 siPCRoomBonusPercent = 0;

							cltBufInfo * pclMyBuf = &pclchar->pclCI->clBI.clBufInfo;

							// 제조 보너스 타임이면, 
							if(pclClient->bBonus2TimeSwitch)
							{
								if( bIsGMBonusTime2 && siGMBonusTime2Percent > 0  ) 
									siTimeBonusPercent = siGMBonusTime2Percent ;
								else
									siTimeBonusPercent = 30;
							}
							else if(pclchar->pclCI->clBI.bMakeItemService)
								siTimeBonusPercent = 20;

							else if( pclMyBuf->IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) )
								siTimeBonusPercent = max( siTimeBonusPercent , pclMyBuf->GetBufPercent(BUF_NOTBONUSTIME_MAKEEXPUP) );

							if(pclEventTimeManager->InEventTime("GoonzuDay_MakeExpBonus150_6", &sTime))
							{
								cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_6");
								if(pclEventTime)	{		siTimeBonusPercent = pclEventTime->m_siBonusAmount;		}
							}
							else if(pclEventTimeManager->InEventTime("GoonzuDay_MakeExpBonus150_12", &sTime))
							{
								cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_12");
								if(pclEventTime)	{		siTimeBonusPercent = pclEventTime->m_siBonusAmount;		}
							}
							else if(pclEventTimeManager->InEventTime("GoonzuDay_MakeExpBonus200_21", &sTime))
							{
								cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus200_21");
								if(pclEventTime)	{		siTimeBonusPercent = pclEventTime->m_siBonusAmount;		}
							}

							//if( bonusPercent > 0)
							//{
							//	skillexp += (SI32)( skillexp * bonusPercent / 100 );
							//	bonusPercent = 0;
							//}

							if( pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP) )
							{
								// 보너스가  중첩되지 않으며 더 좋은 효과만 나타난다.
								siBuffBonusPercent = max( siBuffBonusPercent , pclMyBuf->GetBufPercent(BUF_MAKEEXPUP) );
							}

							//사랑 버프 중이라면 경험치를 100% 증가 
							if( pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) 
								&&  pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) == false )
							{
								// 보너스가 줄어들진 않는다.
								siBuffBonusPercent = max( 100 , siBuffBonusPercent) ;
							}
							//내가 PC이고 우정 버프 중이라면 경험치를 50% 증가
							if( pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) 
								&&  pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) == false )
							{
								// 보너스가 줄어들진 않는다.
								siBuffBonusPercent = max( 50 , siBuffBonusPercent) ;
							}	
							//내가 PC이고 사랑과 우정 버프 중이라면 경험치를 중복해서 증가
							if( pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) 
								&&  pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) )
							{
								// 보너스가 중첩되며 줄어들지는 않는다.
								siBuffBonusPercent = max( 150 , siBuffBonusPercent) ;
							}

							if( pclMyBuf->IsHaveBuf(BUF_HINAMOCHI))
							{
								siBuffBonusPercent += 100 ;
							}

							if( pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
							{
								siBuffBonusPercent += 100 ;
							}
							// PCK : 제조 프리미엄 아이템 : 경험치를 50% 증가시킴.
							if ( pclMyBuf->IsHaveBuf(BUF_PREMIUMMAKEEXPUP))
							{
								siBuffBonusPercent += 50;
							}

							// [진성] 자이언트 라쿤 부활 이벤트에 구조물을 최초 완성을 한 마을이면. 경험치를 50% 증가 
							if( pclMyBuf->IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE) )
							{
								siBuffBonusPercent += 50;
							}
							
							if ( pclMyBuf->IsHaveBuf(BUF_VALENTINEDAY))
							{
								siBuffBonusPercent += 20;
							}

							if(pclMyBuf->IsHaveBuf( BUF_PREMIUMMAKEEXPUP)
								&& pclMyBuf->IsHaveBuf( BUF_MAKEEXPUP))
								if(IsCountrySwitch(Switch_Server_Event))
								{
									//서버 이벤트 진행 기간이면...
									if( pclEventTimeManager->InEventTime("GoonzuDay_Week", &sTime) )
									{
										//[진성] 서버 이벤트 하루 1등 서버이면 버프가 적용된다. => 2008-6-23
										// 버프 타입확인.
										if( pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_150 || 
											pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_200		)
										{
											// 중복불가.
											if( false == pclMyBuf->IsHaveBuf(BUF_HINAMOCHI)  &&
												false == pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) &&
												false == pclMyBuf->IsHaveBuf(BUF_LOVEPOWER)  &&
												false == pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP)  &&
												false == pclMyBuf->IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) &&
												false == pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
											{
												switch(pclClient->pclServerEventManager->GetBonusBufType())
												{
												case CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_150:
													{
														siBuffBonusPercent += 50 ;
													}
													break;
												case CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_200:
													{
														siBuffBonusPercent += 100 ;
													}
													break;
												}
											}
										}
									}
									else
									{
										//서버 이벤트 보상 기간이면...
										if( pclEventTimeManager->InEventTime("GoonzuDay_Week_Reward", &sTime) )
										{
											// 전체 맵에서 올려준다.
											//if(pclClient->pclServerEventManager->IsEventMap(pclCM->CR[id]->GetMapIndex()))
											{
												// 중복불가.
												if( false == pclMyBuf->IsHaveBuf(BUF_HINAMOCHI)  &&
													false == pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) &&
													false == pclMyBuf->IsHaveBuf(BUF_LOVEPOWER)  &&
													false == pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP)  &&
													false == pclMyBuf->IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) &&
													false == pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
												{
													// 최종 1위 서버에게는 경험치를 주는데 
													if(pclClient->pclServerEventManager->IsMyServerRankBest())
													{
														//siBuffBonusPercent += 100;
														siBuffBonusPercent += 50;
													}
													/*else if(pclClient->pclServerEventManager->IsMyServerRankBest2())
													{
														siBuffBonusPercent += 50;
													}*/
												}
											}
										}
									}
								}
								if ( IsCountrySwitch(Switch_We_Are_Family) )		//가족효과로 제조 경험치 증가
								{
									if( pclchar->pclFamily != NULL )
									{
										if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_PARANTS)	// 부모일떄.
										{
											if ( pclchar->pclFamily->GetOnChildrenNum() > 0 )	// 자식케릭터가 1명 이상 접속 해야 한다.
											{
												SI32 siAddValue = 0		;//	 증가할 퍼센트 
												SYSTEMTIME stFamilyEffectTime_Now	;
												// 가족 이용권 기간이 남은 자식 숫자를 찾는다.
												for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++)
												{
													if( pclchar->pclFamily->GetChildPersonID(siSlot) > 0 )
													{
														ZeroMemory( &stFamilyEffectTime_Now ,sizeof( SYSTEMTIME ))	;
														stFamilyEffectTime_Now	= pclchar->pclFamily->GetChild_Add_Date( siSlot ) ;

														if ( stFamilyEffectTime_Now.wMonth > 0 && stFamilyEffectTime_Now.wMonth <= MAX_MONTH_FAMILY	&& 
															stFamilyEffectTime_Now.wDay > 0 && stFamilyEffectTime_Now.wDay <= MAX_DAY_FOR_FAMILY)
														{
															if ( sTime.wMonth > 0 && sTime.wMonth <= MAX_MONTH_FAMILY && sTime.wDay > 0 && sTime.wDay <= MAX_DAY_FOR_FAMILY  )
															{
																SI64 servertime = GetTimeValue(&sTime )	;
																SI64 limittime = GetTimeValue(&stFamilyEffectTime_Now )	;

																if ( servertime < limittime )
																{
																	siAddValue++;
																}

															}
														}
													}
												}
												if ( 10 <= siAddValue )
												{
													siAddValue = MAX_FAMILY_ADD_EXP	;	// 최대는 10%
												}
												siBuffBonusPercent += siAddValue;	// 제조 경험치 증가 persent
												siAddValue	=	0	;	// 적용하고는 초기화 
											}
										}
									}
								}

								//if( siBuffBonusPercent > 0)
								//{
								//	skillexp += (SI32)( skillexp * siBuffBonusPercent / 100 );
								//	siBuffBonusPercent = 0;
								//}

								// 버프에 의해 증가되는 가중치
								//skillexp = pclchar->pclCI->clBI.clBufInfo.CalcTotalBufValue( BUF_MAKEEXPUP, skillexp );

								// 공조판서에 의한 가중치를 계산한다. 
								SI32 para	= 0;

								switch(skillunique)
								{
								case SKILL_MAKESWORD1:	para	= GONGZO_SKILL_SWORD;	break;
								case SKILL_MAKESPEAR1:	para	= GONGZO_SKILL_SPEAR;	break;
								case SKILL_MAKESTAFF1:	para	= GONGZO_SKILL_STAFF;	break;
								case SKILL_MAKEAXE1:	para	= GONGZO_SKILL_AXE;		break;
								case SKILL_MAKEBOW1:	para	= GONGZO_SKILL_BOW;		break;
								case SKILL_MAKEGUN1:	para	= GONGZO_SKILL_GUN;		break;

								case SKILL_MAKEHELMET1:	para	= GONGZO_SKILL_HELMET;	break;
								case SKILL_MAKESHOES1:	para	= GONGZO_SKILL_SHOES;	break;
								case SKILL_MAKEBELT1:	para	= GONGZO_SKILL_BELT;	break;
								case SKILL_MAKEARMOUR1:	para	= GONGZO_SKILL_ARMOR;	break;
								}
								if(para)
								{
									CMinisterGongzo* pclminister = (CMinisterGongzo*)pclMinisterMgr->GetMinister(MINISTER_GONGZO);
									SI32 value = pclminister->GetGongzoSkillValue(para);

									// 기존 공식 최소값 1. 기존공식으로 돌릴시에 1로 바꾸어야 함
									skillexp = max(0, skillexp + (skillexp * value / 100)) ;
								}

								// [영훈] PC방 프리미엄 강화 (제조 경험치 50%추가) - 2008.03.02
								if ( pclchar->pclCI->clBI.bPCRoom == true )
									siPCRoomBonusPercent = 50;


								// 제조 경험치 총합산
								//skillexp += (SI32)( skillexp * ((siTimeBonusPercent + siBuffBonusPercent + siPCRoomBonusPercent) / 100));
								skillexp += (SI32)(( skillexp * (siTimeBonusPercent + siBuffBonusPercent + siPCRoomBonusPercent)) / 100);

								if( pclMsg->bNPCMake )
								{
									skillexp = 0;
								}

								// 피로방지 시스템 적용
								skillexp = pclchar->CalcTiredPoint( skillexp );

								if(skillexp)
								{
									pclchar->SetSkillUpdate(skillunique, skillexp);

									// 가입된 마을이 있다면 DB로 정보를 보낸다
									if( pclchar->pclCI->clBI.siHomeVillage > 0)
									{
										sDBRequest_VillageJoinMakeItem clMsg2( pclchar->pclCI->GetPersonID(), skillexp / 100, pclchar->pclCI->clBI.siHomeVillage);
										SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);

										//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
										//{
										//	// [로그추가 : 황진성 2007. 10. 16] // 마을이 있는경우 아이템 만든 스킬 정보.
										//	// param1 == 스킬 경험치.
										//	cltServer * pclServer = (cltServer*)pclClient;
										//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_MAKE_ITEM_SKILL_EXP, 
										//									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										//									skillexp / 100, 0, 0, 0, 0, NULL, NULL);
										//}
									}

									//KHY - 0115 발렌타인데이 이벤트.
									NDate Today;
									Today.SetDate( sTime.wYear, sTime.wMonth, sTime.wDay );

									if ( Today.GetDateVary() >= clValentineEventStart.GetDateVary() &&
										Today.GetDateVary() <= clValentineEventEnd.GetDateVary() )
									{	
										// 최대 siValentineItem% 최소 1% 의 확률로 이벤트 아이템을 준다
										SI32 itemGetPercent = min( siValentineItem , max( 1 , skillexp / 10 ) )  ;								
										SI32 randValue = rand()%100;

										if( itemGetPercent > randValue )
										{
											SI32 itemType = pclItemManager->pclItemInfo[ref]->clItem.GetType(pclItemManager);

											SI32 eventItemUnique = 0;

											//KHY - 1223  일부 수정.
											if( itemType == ITEMTYPE_ARMOUR || itemType == ITEMTYPE_AXE || itemType == ITEMTYPE_BELT 
												|| itemType == ITEMTYPE_BOW || itemType == ITEMTYPE_GUN || itemType == ITEMTYPE_HELMET 
												|| itemType == ITEMTYPE_SHOES || itemType == ITEMTYPE_SPEAR|| itemType == ITEMTYPE_STAFF
												|| itemType == ITEMTYPE_SWORD 
												|| itemType == ITEMTYPE_BOOK || itemType == ITEMTYPE_DISH || itemType == ITEMTYPE_FISHINGROD
												|| itemType == ITEMTYPE_FOOD || itemType == ITEMTYPE_HEAL || itemType == ITEMTYPE_IRON )
											{
												SI16 randval = rand()%100;
												if(randval > 50)
													eventItemUnique = ITEMUNIQUE(13007); //A카드.
												else
													eventItemUnique = ITEMUNIQUE(13012); //I 카드.
											}

											pclchar->GiveItem( eventItemUnique ,  1 , INPUTITEMPERSON_OPTION_WAR ,0);

										}

									}

									//KHY - 0218 화이트데이 이벤트.
									if ( Today.GetDateVary() >= clWhiteEventStart.GetDateVary() &&
										Today.GetDateVary() <= clWhiteEventEnd.GetDateVary() )
									{	
										// 최대 siValentineItem% 최소 1% 의 확률로 이벤트 아이템을 준다
										SI32 itemGetPercent = min( siWhiteItem , max( 1 , skillexp / 10 ) )  ;								
										SI32 randValue = rand()%100;

										if( itemGetPercent > randValue )
										{
											SI32 itemType = pclItemManager->pclItemInfo[ref]->clItem.GetType(pclItemManager);

											SI32 eventItemUnique = 0;

											if( itemType == ITEMTYPE_ARMOUR || itemType == ITEMTYPE_AXE || itemType == ITEMTYPE_BELT 
												|| itemType == ITEMTYPE_BOW || itemType == ITEMTYPE_GUN || itemType == ITEMTYPE_HELMET 
												|| itemType == ITEMTYPE_SHOES || itemType == ITEMTYPE_SPEAR|| itemType == ITEMTYPE_STAFF
												|| itemType == ITEMTYPE_SWORD )
											{
												eventItemUnique = ITEMUNIQUE(13017); //H 카드.
											}
											else if( itemType == ITEMTYPE_BOOK || itemType == ITEMTYPE_DISH || itemType == ITEMTYPE_FISHINGROD 
												|| itemType == ITEMTYPE_FOOD || itemType == ITEMTYPE_HEAL || itemType == ITEMTYPE_IRON )
											{
												eventItemUnique = ITEMUNIQUE(13017); //H 카드.
											}

											pclchar->GiveItem( eventItemUnique ,  1 , INPUTITEMPERSON_OPTION_WAR ,0);

										}

									}							
									////////////////////////////////////////////////////////////////////////////////////////////////////

									// 기술 경험치가 큰 경우 기록에 남긴다. 
									if(skillexp >= 3000)
									{
										pclLog->FilePrint(TEXT("Config\\SkillExp.log"), TEXT("Name:%s Exp:%d Item:%d Num:%d Item2:%d Result:%d"), 
											pclchar->GetName(), skillexp, pclMsg->clItem.siUnique, pclMsg->siAddedNum,
											pclMsg->siItemUnique, pclMsg->siResult);

									}
								}
						}
						else		// [종호] NHNCHINA가 아닌경우의 제조경험치 합산
						{
							SI32 bonusPercent = 0;
							cltBufInfo * pclMyBuf = &pclchar->pclCI->clBI.clBufInfo;

							// 제조 보너스 타임이면, 
							if(pclClient->bBonus2TimeSwitch)
							{
								if( bIsGMBonusTime2 && siGMBonusTime2Percent > 0  ) 
								{
									bonusPercent = siGMBonusTime2Percent ;
								}
								else
								{
									if ( pclClient->siServiceArea == ConstServiceArea_English )
									{
										bonusPercent = 30;
									}
									else if ( pclClient->siServiceArea == ConstServiceArea_Japan )
									{
										bonusPercent = 20;
									}
									else if ( pclClient->siServiceArea == ConstServiceArea_NHNChina )
									{
										bonusPercent = 30;
									}
									//LEEKH - 0810 - 군주S 제조경험치 수정(+50%)
									else if ( pclClient->siServiceArea == ConstServiceArea_Korea)
									{
										bonusPercent = 50;
									}
									else if ( pclClient->siServiceArea == ConstServiceArea_USA)
									{
										// 미국 임시 추가 - 군주 보너스 타임은 130%
										if(bBonusTimeGoonzu)
										{
											bonusPercent = 30;

											if(true == bAlldayBonus)
												bonusPercent = 100;
										}
										else
										{
											bonusPercent = 100;
										}
									}
									else if ( pclClient->siServiceArea == ConstServiceArea_EUROPE )
									{
										bonusPercent = 30;
									}
									else
									{
										bonusPercent = 100;
									}

								}

							}
							else if(pclchar->pclCI->clBI.bMakeItemService)
							{
								bonusPercent = 20;
							}
							else if( pclMyBuf->IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) )
							{
								bonusPercent = max( bonusPercent , pclMyBuf->GetBufPercent(BUF_NOTBONUSTIME_MAKEEXPUP) );
							}

							if(pclEventTimeManager->InEventTime("GoonzuDay_MakeExpBonus150_6", &sTime))
							{
								cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_6");
								if(pclEventTime)	{		bonusPercent = pclEventTime->m_siBonusAmount;		}
							}
							else if(pclEventTimeManager->InEventTime("GoonzuDay_MakeExpBonus150_12", &sTime))
							{
								cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_12");
								if(pclEventTime)	{		bonusPercent = pclEventTime->m_siBonusAmount;		}
							}
							else if(pclEventTimeManager->InEventTime("GoonzuDay_MakeExpBonus200_21", &sTime))
							{
								cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus200_21");
								if(pclEventTime)	{		bonusPercent = pclEventTime->m_siBonusAmount;		}
							}

							if( bonusPercent > 0)
							{
								skillexp += (SI32)( skillexp * bonusPercent / 100 );
								bonusPercent = 0;
							}

							if( pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP) )
							{
								// 보너스가  중첩되지 않으며 더 좋은 효과만 나타난다.
								bonusPercent = max( bonusPercent , pclMyBuf->GetBufPercent(BUF_MAKEEXPUP) );
							}

							//사랑 버프 중이라면 경험치를 100% 증가 
							if( pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) 
								&&  pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) == false )
							{
								// 보너스가 줄어들진 않는다.
								bonusPercent = max( 100 , bonusPercent) ;
							}
							//내가 PC이고 우정 버프 중이라면 경험치를 50% 증가
							if( pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) 
								&&  pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) == false )
							{
								// 보너스가 줄어들진 않는다.
								bonusPercent = max( 50 , bonusPercent) ;
							}	
							//내가 PC이고 사랑과 우정 버프 중이라면 경험치를 중복해서 증가
							if( pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) 
								&&  pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) )
							{
								// 보너스가 중첩되며 줄어들지는 않는다.
								bonusPercent = max( 150 , bonusPercent) ;
							}

							if( pclMyBuf->IsHaveBuf(BUF_HINAMOCHI))
							{
								bonusPercent += 100 ;
							}

							if( pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
							{
								bonusPercent += 100 ;
							}
							// PCK : 제조 프리미엄 아이템 : 경험치를 50% 증가시킴.
							if ( pclMyBuf->IsHaveBuf(BUF_PREMIUMMAKEEXPUP))
							{
								bonusPercent += 50;
							}

							// [진성] 자이언트 라쿤 부활 이벤트에 구조물을 최초 완성을 한 마을이면. 경험치를 50% 증가 
							if( pclMyBuf->IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE) )
							{
								bonusPercent += 50;
							}

							if ( pclMyBuf->IsHaveBuf(BUF_VALENTINEDAY))
							{
								bonusPercent += 20;
							}

							// [춘기] 크리스마스 이벤트 - 엔펠리스 앞에 크리스마스 트리가 세워지면 1단계 : 5%, 2단계 : 10%의 경험치 증가 효과를 받는다.
							if((pclClient->pclEventTimeManager->InEventTime( TEXT("ChristmasEvent"), &sTime) == true))
							{
								cltServer* pclserver = (cltServer*)pclClient;

								SI32	siWorldChristmasTree01Kind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01"));
								SI32	siWorldChristmasTree02Kind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02"));

								SI32 siWorldChristmasTree01ID = pclserver->siWorldTreeID[0];
								SI32 siWorldChristmasTree02ID = pclserver->siWorldTreeID[1];

								cltCharServer*	pclWorldChristmasTreeChar01 = (cltCharServer*)pclClient->pclCM->CR[siWorldChristmasTree01ID];
								cltCharServer*	pclWorldChristmasTreeChar02 = (cltCharServer*)pclClient->pclCM->CR[siWorldChristmasTree02ID];

								if(pclWorldChristmasTreeChar01)
								{
									if((pclWorldChristmasTreeChar01->GetMapIndex() == MAPINDEX_MAINFIELD) && (pclWorldChristmasTreeChar01->GetKind() == siWorldChristmasTree01Kind))
									{
										bonusPercent += 5;
									}
								}

								if(pclWorldChristmasTreeChar02)
								{
									if((pclWorldChristmasTreeChar02->GetMapIndex() == MAPINDEX_MAINFIELD) && (pclWorldChristmasTreeChar02->GetKind() == siWorldChristmasTree02Kind))
									{
										bonusPercent += 10;
									}
								}
							}

							if(pclMyBuf->IsHaveBuf( BUF_PREMIUMMAKEEXPUP) && pclMyBuf->IsHaveBuf( BUF_MAKEEXPUP))
								if(IsCountrySwitch(Switch_Server_Event))
								{
									//서버 이벤트 진행 기간이면...
									if( pclEventTimeManager->InEventTime("GoonzuDay_Week", &sTime) )
									{
										//[진성] 서버 이벤트 하루 1등 서버이면 버프가 적용된다. => 2008-6-23
										// 버프 타입확인.
										if( pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_150 || 
											pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_200		)
										{
											// 중복불가.
											if( false == pclMyBuf->IsHaveBuf(BUF_HINAMOCHI)  &&
												false == pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) &&
												false == pclMyBuf->IsHaveBuf(BUF_LOVEPOWER)  &&
												false == pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP)  &&
												false == pclMyBuf->IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) &&
												false == pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
											{
												switch(pclClient->pclServerEventManager->GetBonusBufType())
												{
												case CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_150:
													{
														bonusPercent += 50 ;
													}
													break;
												case CNServerEvent_Manager::SERVER_EVENT_BUF_MAKE_EXP_200:
													{
														bonusPercent += 100 ;
													}
													break;
												}
											}
										}
									}
									else
									{
										//서버 이벤트 보상 기간이면...
										if( pclEventTimeManager->InEventTime("GoonzuDay_Week_Reward", &sTime) )
										{
											// 전체 맵에서 올려준다.
											//if(pclClient->pclServerEventManager->IsEventMap(pclCM->CR[id]->GetMapIndex()))
											{
												// 중복불가.
												if( false == pclMyBuf->IsHaveBuf(BUF_HINAMOCHI)  &&
													false == pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) &&
													false == pclMyBuf->IsHaveBuf(BUF_LOVEPOWER)  &&
													false == pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP)  &&
													false == pclMyBuf->IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) &&
													false == pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
												{
													// 최종 1위 서버에게는 경험치를 주는데 
													if(pclClient->pclServerEventManager->IsMyServerRankBest())
													{
														//bonusPercent += 100;
														bonusPercent += 50;
													}
													/*else if(pclClient->pclServerEventManager->IsMyServerRankBest2())
													{
														bonusPercent += 50;
													}*/
												}
											}
										}
									}
								}
								if ( IsCountrySwitch(Switch_We_Are_Family) )		//가족효과로 제조 경험치 증가
								{
									if( pclchar->pclFamily != NULL )
									{
										if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_PARANTS)	// 부모일떄.
										{
											if ( pclchar->pclFamily->GetOnChildrenNum() > 0 )	// 자식케릭터가 1명 이상 접속 해야 한다.
											{
												SI32 siAddValue = 0		;//	 증가할 퍼센트 
												SYSTEMTIME stFamilyEffectTime_Now	;
												// 가족 이용권 기간이 남은 자식 숫자를 찾는다.
												for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++)
												{
													if( pclchar->pclFamily->GetChildPersonID(siSlot) > 0 )
													{
														ZeroMemory( &stFamilyEffectTime_Now ,sizeof( SYSTEMTIME ))	;
														stFamilyEffectTime_Now	= pclchar->pclFamily->GetChild_Add_Date( siSlot ) ;

														if ( stFamilyEffectTime_Now.wMonth > 0 && stFamilyEffectTime_Now.wMonth <= MAX_MONTH_FAMILY	&& 
															stFamilyEffectTime_Now.wDay > 0 && stFamilyEffectTime_Now.wDay <= MAX_DAY_FOR_FAMILY)
														{
															if ( sTime.wMonth > 0 && sTime.wMonth <= MAX_MONTH_FAMILY && sTime.wDay > 0 && sTime.wDay <= MAX_DAY_FOR_FAMILY  )
															{
																SI64 servertime = GetTimeValue(&sTime )	;
																SI64 limittime = GetTimeValue(&stFamilyEffectTime_Now )	;

																if ( servertime < limittime )
																{
																	siAddValue++;
																}

															}
														}
													}
												}
												if ( 10 <= siAddValue )
												{
													siAddValue = MAX_FAMILY_ADD_EXP	;	// 최대는 10%
												}
												bonusPercent += siAddValue;	// 제조 경험치 증가 persent
												siAddValue	=	0	;	// 적용하고는 초기화 
											}
										}
									}
								}

								if( bonusPercent > 0)
								{
									skillexp += (SI32)( skillexp * bonusPercent / 100 );
									bonusPercent = 0;
								}

								// 버프에 의해 증가되는 가중치
								//skillexp = pclchar->pclCI->clBI.clBufInfo.CalcTotalBufValue( BUF_MAKEEXPUP, skillexp );

								// 공조판서에 의한 가중치를 계산한다. 
								SI32 para	= 0;

								switch(skillunique)
								{
								case SKILL_MAKESWORD1:	para	= GONGZO_SKILL_SWORD;	break;
								case SKILL_MAKESPEAR1:	para	= GONGZO_SKILL_SPEAR;	break;
								case SKILL_MAKESTAFF1:	para	= GONGZO_SKILL_STAFF;	break;
								case SKILL_MAKEAXE1:	para	= GONGZO_SKILL_AXE;		break;
								case SKILL_MAKEBOW1:	para	= GONGZO_SKILL_BOW;		break;
								case SKILL_MAKEGUN1:	para	= GONGZO_SKILL_GUN;		break;

								case SKILL_MAKEHELMET1:	para	= GONGZO_SKILL_HELMET;	break;
								case SKILL_MAKESHOES1:	para	= GONGZO_SKILL_SHOES;	break;
								case SKILL_MAKEBELT1:	para	= GONGZO_SKILL_BELT;	break;
								case SKILL_MAKEARMOUR1:	para	= GONGZO_SKILL_ARMOR;	break;
								}
								if(para)
								{
									CMinisterGongzo* pclminister = (CMinisterGongzo*)pclMinisterMgr->GetMinister(MINISTER_GONGZO);
									SI32 value = pclminister->GetGongzoSkillValue(para);

									// 기존 공식 최소값 1. 기존공식으로 돌릴시에 1로 바꾸어야 함
									skillexp = max(0, skillexp + (skillexp * value / 100)) ;
								}

								// [영훈] PC방 프리미엄 강화 (제조 경험치 50%추가) - 2008.03.02
								if ( pclchar->pclCI->clBI.bPCRoom == true )
								{
									SI32 siPCRoomBonusPercent = 50;
									skillexp += (SI32)( skillexp * siPCRoomBonusPercent / 100 );
								}

								if( pclMsg->bNPCMake )
								{
									skillexp = 0;
								}

								// 피로방지 시스템 적용
								skillexp = pclchar->CalcTiredPoint( skillexp );

								if(skillexp)
								{
									pclchar->SetSkillUpdate(skillunique, skillexp);

									// 가입된 마을이 있다면 DB로 정보를 보낸다
									if( pclchar->pclCI->clBI.siHomeVillage > 0)
									{
										sDBRequest_VillageJoinMakeItem clMsg2( pclchar->pclCI->GetPersonID(), skillexp / 100, pclchar->pclCI->clBI.siHomeVillage);
										SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);

										//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
										//{
										//	// [로그추가 : 황진성 2007. 10. 16] // 마을이 있는경우 아이템 만든 스킬 정보.
										//	// param1 == 스킬 경험치.
										//	cltServer * pclServer = (cltServer*)pclClient;
										//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_MAKE_ITEM_SKILL_EXP, 
										//									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										//									skillexp / 100, 0, 0, 0, 0, NULL, NULL);
										//}
									}

									//KHY - 0115 발렌타인데이 이벤트.
									NDate Today;
									Today.SetDate( sTime.wYear, sTime.wMonth, sTime.wDay );

									if ( Today.GetDateVary() >= clValentineEventStart.GetDateVary() &&
										Today.GetDateVary() <= clValentineEventEnd.GetDateVary() )
									{	
										// 최대 siValentineItem% 최소 1% 의 확률로 이벤트 아이템을 준다
										SI32 itemGetPercent = min( siValentineItem , max( 1 , skillexp / 10 ) )  ;								
										SI32 randValue = rand()%100;

										if( itemGetPercent > randValue )
										{
											SI32 itemType = pclItemManager->pclItemInfo[ref]->clItem.GetType(pclItemManager);

											SI32 eventItemUnique = 0;

											//KHY - 1223  일부 수정.
											if( itemType == ITEMTYPE_ARMOUR || itemType == ITEMTYPE_AXE || itemType == ITEMTYPE_BELT 
												|| itemType == ITEMTYPE_BOW || itemType == ITEMTYPE_GUN || itemType == ITEMTYPE_HELMET 
												|| itemType == ITEMTYPE_SHOES || itemType == ITEMTYPE_SPEAR|| itemType == ITEMTYPE_STAFF
												|| itemType == ITEMTYPE_SWORD 
												|| itemType == ITEMTYPE_BOOK || itemType == ITEMTYPE_DISH || itemType == ITEMTYPE_FISHINGROD
												|| itemType == ITEMTYPE_FOOD || itemType == ITEMTYPE_HEAL || itemType == ITEMTYPE_IRON 
												|| itemType == ITEMTYPE_AVATAR_MANTLE || itemType == ITEMTYPE_AVATAR_DRESS || itemType == ITEMTYPE_AVATAR_HAT
												|| itemType == ITEMTYPE_AVATAR_NECK || itemType == ITEMTYPE_AVATAR_RING || itemType == ITEMTYPE_CRYSTAL
												)
											{
												SI16 randval = rand()%100;
												if(randval > 50)
													eventItemUnique = ITEMUNIQUE(13007); //A카드.
												else
													eventItemUnique = ITEMUNIQUE(13012); //I 카드.
											}

											pclchar->GiveItem( eventItemUnique ,  1 , INPUTITEMPERSON_OPTION_WAR ,0);

										}

									}

									//KHY - 0218 화이트데이 이벤트.
									if ( Today.GetDateVary() >= clWhiteEventStart.GetDateVary() &&
										Today.GetDateVary() <= clWhiteEventEnd.GetDateVary() )
									{	
										// 최대 siValentineItem% 최소 1% 의 확률로 이벤트 아이템을 준다
										SI32 itemGetPercent = min( siWhiteItem , max( 1 , skillexp / 10 ) )  ;								
										SI32 randValue = rand()%100;

										if( itemGetPercent > randValue )
										{
											SI32 itemType = pclItemManager->pclItemInfo[ref]->clItem.GetType(pclItemManager);

											SI32 eventItemUnique = 0;

											if( itemType == ITEMTYPE_ARMOUR || itemType == ITEMTYPE_AXE || itemType == ITEMTYPE_BELT 
												|| itemType == ITEMTYPE_BOW || itemType == ITEMTYPE_GUN || itemType == ITEMTYPE_HELMET 
												|| itemType == ITEMTYPE_SHOES || itemType == ITEMTYPE_SPEAR|| itemType == ITEMTYPE_STAFF
												|| itemType == ITEMTYPE_SWORD )
											{
												eventItemUnique = ITEMUNIQUE(13017); //H 카드.
											}
											else if( itemType == ITEMTYPE_BOOK || itemType == ITEMTYPE_DISH || itemType == ITEMTYPE_FISHINGROD 
												|| itemType == ITEMTYPE_FOOD || itemType == ITEMTYPE_HEAL || itemType == ITEMTYPE_IRON )
											{
												eventItemUnique = ITEMUNIQUE(13017); //H 카드.
											}

											pclchar->GiveItem( eventItemUnique ,  1 , INPUTITEMPERSON_OPTION_WAR ,0);

										}

									}							
									////////////////////////////////////////////////////////////////////////////////////////////////////

									// 기술 경험치가 큰 경우 기록에 남긴다. 
									if(skillexp >= 3000)
									{
										pclLog->FilePrint(TEXT("Config\\SkillExp.log"), TEXT("Name:%s Exp:%d Item:%d Num:%d Item2:%d Result:%d"), 
											pclchar->GetName(), skillexp, pclMsg->clItem.siUnique, pclMsg->siAddedNum,
											pclMsg->siItemUnique, pclMsg->siResult);

									}
								}
						}
					}
				}
			}
		}

		{
			// 물품 제조 결과를 통보한다. 
			cltItem clmadeitem;
			clmadeitem.Set(&pclMsg->clItem);
			clmadeitem.SetItemNum(pclMsg->siAddedNum);

			cltGameMsgResponse_MakeItemDlg clinfo(&clmadeitem, exp, skillunique, skillexp,pclchar->siMonsterCorp);
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMDLG, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			pclchar->siMonsterCorp = 0 ;
			pclchar->dwStartMiniGameTime = 0;
		}



		//---------------------------------------
		// 인벤의 정보를 클라이언트에게 보내준다. 
		//----------------------------------------
		if(pclMsg->bAlmostFailSwitch == true)
		{
			if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
			{
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_MAKEALMOSTFAIL, pclMsg->siAddedNum, exp, &pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos], 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		else
		{
			if(pclMsg->siLuckQuality)
			{
				if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
				{
					cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_MAKELUCK, pclMsg->siAddedNum, exp, &pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos], 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
			else
			{
				if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
				{
					cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_MAKE, pclMsg->siAddedNum, exp, &pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos], 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}

		// 제작자와 주문자의 소지금을 업데이트 한다. 
		if(pclMsg->siFee)
		{
			{
				pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siPersonIDMoney);

				// 클라이언트로 통보한다. 
				cltGameMsgResponse_ChangeMoney clinfo(pclCM->CR[id]->pclCI->clIP.GetMoney(),0, CHANGE_MONEY_REASON_MAKE);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}

			{
				pclMaterialChar->pclCI->clIP.SetMoney(pclMsg->siMaterialPersonIDMoney);

				// 클라이언트로 통보한다. 
				cltGameMsgResponse_ChangeMoney clinfo(pclMaterialChar->pclCI->clIP.GetMoney(),0, CHANGE_MONEY_REASON_MAKE);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
				pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
			}

		}
		//---------------------------------------
		// 물품 만들기 퀘스트 진행중이었는지 확인하여 조치한다. 
		//---------------------------------------

		// 물품을 만드는  퀘스트가 진행중이었나 ?
		// 만든 물품 수를 증가시킨다. 
		((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_MAKEITEM, pclMsg->clItem.siUnique, pclMsg->siAddedNum);

		((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_TUTORIALMAKEITEM, pclMsg->clItem.siUnique, pclMsg->siAddedNum);

		// Para1 레벨의 아이템을 Para2만큼 만드는 퀘스트라면
		SI32 siConditionLevel = pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
		((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_MAKELEVELITEM, siConditionLevel, pclMsg->siAddedNum);
		((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM, siConditionLevel, pclMsg->siAddedNum);		

		SI32 itemType = pclItemManager->pclItemInfo[ref]->clItem.GetType(pclItemManager);

		if( itemType == ITEMTYPE_BOOK )
		{
			((cltCharServer*)(pclCM->CR[id]))->DoQuestCheckAndAction(QUEST_CLEAR_COND_MAKEBOOK);
			if ( pclItemManager->pclItemInfo[ref]->GetSkillLevelLimit() == 20 )
			{
				((cltCharServer*)(pclCM->CR[id]))->DoQuestCheckAndAction(QUEST_CLEAR_COND_MAKEINTBOOK);
			}
		}

		CNServerEvent_Quest*	pclSE_Quest		= NULL;
		SI32					siMakeAmount	= max( pclMsg->siAddedNum, ((cltCharServer*)(pclCM->CR[id]))->siQuestCount );
		// [영훈] 서버 이벤트 퀘스트 : 레벨 범위 내의 아이템 만들기
		if ( IsCountrySwitch(Switch_Server_Event) )
		{
			pclSE_Quest = (CNServerEvent_Quest*)pclServerEventManager->GetServerEvent_Quest();

			if ( pclSE_Quest )
			{
				pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_MAKELEVELITEM, siConditionLevel, siMakeAmount );
			}
		}

		SI32 siCrystalGrade = pclMsg->clItem.GetCrystalGrade();
		if ( siCrystalGrade )
		{
			((cltCharServer*)(pclCM->CR[id]))->IncQuestPara( QUEST_CLEAR_COND_MAKECRYSTAL, siCrystalGrade, siMakeAmount );
		}

		cltServer* pclserver			= (cltServer*)pclClient;
		//물건을 만들때 데일리 퀘스트3 진행인지 유니크로 확인 한다.
		if(pclchar->pclCI->clDailyQuest3Info.siItemUnique == pclMsg->clItem.siUnique )
		{
			//	히든 퀘스트 일때를 생각해서 레어 타입을 받아올 변수 설정.
			UI32 raretype;
			// 해당 퀘스트 아이템의 갯수를 적용 한다.			
			//// 신기 일때.
			//if ( RARELEVEL_3 ==pclchar->pclCI->clDailyQuest3Info.siMakedeItemNum + pclMsg->clItem.GetRareType() ){
			//}
			//	현제 이퀘스트가 히든 데일리 퀘스트 3인지 구분 한다.
			if ( pclchar->pclCI->clDailyQuest3Info.bHiddenQuest == true )	//	히든 퀘스트
			{
				raretype = pclMsg->clItem.GetRareType();
				if (raretype == RARELEVEL_5) { // 신기일때.
					pclchar->pclCI->clDailyQuest3Info.siMadeItemNum = pclchar->pclCI->clDailyQuest3Info.siMadeItemNum + pclMsg->clItem.siItemNum;
					if (pclchar->pclCI->clDailyQuest3Info.siMadeItemNum >= pclchar->pclCI->clDailyQuest3Info.siItemNum)
					{
						SI32 value = 100;
						cltItem clItem;
						clItem.siUnique = pclchar->pclCI->clDailyQuest3Info.siRewardItemUnique;
						clItem.siItemNum = ( pclchar->pclCI->clDailyQuest3Info.siRewardItemNum * value ) / 100;
						cltItem clToItem;
						clToItem.Init();

						SI16 InvPos = -1;
						SI32 rtnvalue;
						GMONEY money = 0 ;
						//보상으 줄때 이벤토리가 꽉차있는지 확인 해서 보상을 준다
						if ( pclchar->CanAddInv(INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
						{
							sDBRequest_DailyQuest3Del clMsg( pclchar->pclCI->GetPersonID(), id, true, InvPos, 
								pclchar->pclCI->clDailyQuest3Info.siItemUnique,&clItem, 1, money, pclchar->pclCI->clDailyQuest3Info.bHiddenQuest );
							pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}
						else
						{
							// 인벤 꽉 차서 안되므로...
							pclserver->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  1, 0, pclchar->GetCharUnique());

							// 현재 상태를 클라이언트에 전송
							cltGameMsgResponse_DailyQuest3Change clinfo( pclchar->pclCI->clDailyQuest3Info.siLeftTime, pclchar->pclCI->clDailyQuest3Info.siMadeItemNum, pclchar->pclCI->clDailyQuest3Info.bHiddenQuest );
							cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
							pclchar->SendNetMsg((sPacketHeader*)&clMsg);	
						}
					}
				}
			}
			else	//	히든 퀘스트가 아닌 일반 데일리 퀘스트3
			{
				pclchar->pclCI->clDailyQuest3Info.siMadeItemNum = pclchar->pclCI->clDailyQuest3Info.siMadeItemNum + pclMsg->clItem.siItemNum;
				if (pclchar->pclCI->clDailyQuest3Info.siMadeItemNum >= pclchar->pclCI->clDailyQuest3Info.siItemNum) 
				{
					SI32 value = 100;

					cltItem clItem;
					clItem.siUnique = pclchar->pclCI->clDailyQuest3Info.siRewardItemUnique;
					clItem.siItemNum = ( pclchar->pclCI->clDailyQuest3Info.siRewardItemNum * value ) / 100;
					cltItem clToItem;
					clToItem.Init();

					SI16 InvPos = -1;
					SI32 rtnvalue;
					GMONEY money = 0 ;
					// 보상을 준다.
					if ( pclchar->CanAddInv(INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
					{
						sDBRequest_DailyQuest3Del clMsg( pclchar->pclCI->GetPersonID(), id, true, InvPos, 
							pclchar->pclCI->clDailyQuest3Info.siItemUnique,&clItem, 1, money, pclchar->pclCI->clDailyQuest3Info.bHiddenQuest );
						pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
					else
					{
						// 인벤 꽉 차서 안되므로...
						pclserver->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  1, 0, pclchar->GetCharUnique());

						// 현재 상태를 클라이언트에 전송
						cltGameMsgResponse_DailyQuest3Change clinfo( pclchar->pclCI->clDailyQuest3Info.siLeftTime, pclchar->pclCI->clDailyQuest3Info.siMadeItemNum, pclchar->pclCI->clDailyQuest3Info.bHiddenQuest );
						cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
						pclchar->SendNetMsg((sPacketHeader*)&clMsg);	
					}
				}
				else	// 퀘스트가 완료되지 않았으므로 
				{		// DB 로 업데이트 하지는 않고, 유저에게 바로 알려줌.
					cltGameMsgResponse_DailyQuest3Change clinfo( pclchar->pclCI->clDailyQuest3Info.siLeftTime, pclchar->pclCI->clDailyQuest3Info.siMadeItemNum, pclchar->pclCI->clDailyQuest3Info.bHiddenQuest );
					cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);	
				}
			}
		}


		// [영훈] 레어아이템 만들기 퀘스트 클리어 컨디션 추가 : 2008.02.14
		if ( (pclMsg->clItem.GetRareType() == RARELEVEL_1)		// 명품
			|| (pclMsg->clItem.GetRareType() == RARELEVEL_2)	// 호품
			|| (pclMsg->clItem.GetRareType() == RARELEVEL_5)	// 신기
		)
		{
			((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_MAKE_RAREITEM, pclMsg->clItem.siUnique, pclMsg->siAddedNum);

			// [서버] 이벤트 퀘스트 : 신기이상 물품 만들기
			if ( IsCountrySwitch(Switch_Server_Event) )
			{
				if ( pclSE_Quest )
				{
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_MAKERAREITEM, 0, siMakeAmount );
				}
			}

		}

		// 파티퀘스트
		if(IsCountrySwitch(Switch_PartyQuest_PartyMatching))
		{
			// 파티에 속한 플레이어만 파티퀘스트 정보를 갱신 합니다.
			if(pclchar->siPartyIndex > 0)
			{
				pclserver->m_pPartyQuestMgr->UpdatePartyQuest(pclchar->GetID() ,pclchar->siPartyIndex, PARTYQUEST_TYPE_MAKEITEM, pclMsg->clItem.siUnique, pclMsg->siAddedNum);
			}
		}
			

		// 아이템 사용 통계를 업데이트 한다. 
		pclItemManager->AddMakeNum(pclMsg->clItem.siUnique , pclMsg->siAddedNum);

		// 새로운 통계 - 아이템 사용
		NStatistics_Add("Item", pclMsg->clItem.siUnique,"MakeNum", pclMsg->siAddedNum);
		NStatistics_Set("Item", pclMsg->clItem.siUnique, "Name", pclItemManager->GetName(pclMsg->clItem.siUnique));

		// 새로운 통계 - 슬롯 구명 카운트
		if( pclMsg->clItem.siUseCrystalNum > 0 )
		{
			NTCHARString32	kSlotNum;
			kSlotNum = "Slot";		kSlotNum += SI08ToString(pclMsg->clItem.siUseCrystalNum);
			NStatistics_Add("Item", pclMsg->clItem.siUnique, kSlotNum, 1);
		}
	}

	// 이제형바보
	//---------------------------------------------------------------
	// 의뢰자와 제작자가 다른 경우, 의뢰자의 개인상점 정보를 갱신한다. 
	//---------------------------------------------------------------
	if(pclMsg->m_bRequest_Want_Make_Item == true )	// 제작의뢰일 경우
	{
		if(pclMsg->siPersonID != pclMsg->siMaterialPersonID)
		{
			// 이제형바보
			//---------------------------------------------------------------
			// 의뢰자와 제작자가 다른 경우, 의뢰자의 개인상점 정보를 갱신한다. 
			//---------------------------------------------------------------
			// 개인 상점을 아직 열고 있는지 확인한다.
			if(pclCM->IsValidID(id))
			{
				cltCharServer* pclMaker	= (cltCharServer* )pclCM->CR[ id ]	;
				// 개인 상점을 아직 열고 있는지 확인한다. => 상점을 열고 있는건 이때 제작자 
				// 개인 상점중 제작의뢰중인지 확인한다. 
				// 제작의뢰품목이 동일한지 확인한다. 
				if( pclMaker->bPersonalShopSwitch == true &&
					pclMaker->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT &&
					pclMaker->clShopData.m_clItem[0].siUnique == pclMsg->siItemUnique)
				{
					// 같은 물품을 의뢰하고 있다고 판별 

					// 제조 의뢰 개인상점 정보를 업데이트 한다. 
					// 클라이언트에 통보한다. 
					pclMaker->SetUpdateSwitch(UPDATE_IP, true, 0);

					// 실패제 복구한다.
					if(pclMsg->siAddedNum <= 0)
					{
						pclMaker->clShopData.m_clItem[0].siItemNum += pclMsg->siOrderNumber;
					}
					else	// Client에게 성공정보를 보내준다.			
					{

						cltGameMsgResponse_MakePersonalShop clInfo(id, MAKEPERSONALSHOP_SUCCESS_MAKE, pclMsg->siItemUnique,  pclMsg->siAddedNum );
						cltMsg clMsg(GAMEMSG_RESPONSE_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					// 물건이 몽딴 팔리면 Stop시키도록 한다. 상점주인인 제작 의뢰자를 Update한다.
					if(pclMaker->clShopData.m_clItem[0].siItemNum <= 0)
					{
						// 캐릭터의 개인상점을 설정한다. 
						pclMaker->SetPersonalShopBanner(NULL);
						pclMaker->SetPersonalShopData(false, NULL);

						// 이웃들에게 종료가 되었음을 알려준다.
						cltGameMsgResponse_StopPersonalShop clinfo(pclMaker->GetCharUnique(), pclMaker->pclCI->GetPersonID(), 2);
						cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
						pclMaker->SendNeighbourMsg(&clMsg, true);
					}
					else	
					{
						cltGameMsgResponse_UpdatePersonalShopData clinfo(id, &pclMaker->clShopData);
						cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
						pclMaker->SendNetMsg((sPacketHeader*)&clMsg);
					}

					// 개인상점 갱신정보를 보낸다. 제작자 한테 
					{
						cltGameMsgResponse_UpdatePersonalShopData clinfo(id, &pclMaker->clShopData);
						cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->CR[materialcharid]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					//---------------------------------------------------------------
				}
				else // 상점을 닫도록 한다.
				{
					if(pclMaker->bPersonalShopSwitch == false)	
						pclMaker->clShopData.m_siShopMode = PSHOP_MODE_NONE;

					// 개인상점 갱신정보를 보낸다.
					cltGameMsgResponse_UpdatePersonalShopData clinfo(id, &pclMaker->clShopData);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[materialcharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}
                
			}
		}

	}
	else // 기존의 제조의뢰 
	{
		if(pclMsg->siPersonID != pclMsg->siMaterialPersonID)
		{
			// 이제형바보
			//---------------------------------------------------------------
			// 의뢰자와 제작자가 다른 경우, 의뢰자의 개인상점 정보를 갱신한다. 
			//---------------------------------------------------------------
			// 개인 상점을 아직 열고 있는지 확인한다. 
			if(pclCM->IsValidID(materialcharid))
			{
				cltCharServer* pclcharshopper	= (cltCharServer* )pclCM->CR[ materialcharid ];
				// 개인 상점을 아직 열고 있는지 확인한다. 
				// 개인 상점중 제조의뢰중인지 확인한다. 
				// 제조의뢰품목이 동일한지 확인한다. 
				if( pclcharshopper->bPersonalShopSwitch == true &&
					pclcharshopper->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM &&
					pclcharshopper->clShopData.m_clItem[0].siUnique == pclMsg->siItemUnique)
				{
					// 같은 물품을 의뢰하고 있다고 판별 

					// 제조 의뢰 개인상점 정보를 업데이트 한다. 
					// 클라이언트에 통보한다. 
					pclcharshopper->SetUpdateSwitch(UPDATE_IP, true, 0);

					// 실패제 복구한다.
					if(pclMsg->siAddedNum <= 0)
					{
						pclcharshopper->clShopData.m_clItem[0].siItemNum += pclMsg->siOrderNumber;
					}
					else	// Client에게 성공정보를 보내준다.			
					{

						cltGameMsgResponse_MakePersonalShop clInfo(id, MAKEPERSONALSHOP_SUCCESS_MAKE, pclMsg->siItemUnique,  pclMsg->siAddedNum );
						cltMsg clMsg(GAMEMSG_RESPONSE_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					// 물건이 몽딴 팔리면 Stop시키도록 한다. 상점주인인 제작 의뢰자를 Update한다.
					if(pclcharshopper->clShopData.m_clItem[0].siItemNum <= 0)
					{
						// 캐릭터의 개인상점을 설정한다. 
						pclcharshopper->SetPersonalShopBanner(NULL);
						pclcharshopper->SetPersonalShopData(false, NULL);

						// 이웃들에게 종료가 되었음을 알려준다.
						cltGameMsgResponse_StopPersonalShop clinfo(pclcharshopper->GetCharUnique(), pclcharshopper->pclCI->GetPersonID(), 2);
						cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
						pclcharshopper->SendNeighbourMsg(&clMsg, true);

						//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
						//{
						//	// [로그추가 : 황진성 2007. 10. 9] // 개인상점 종료 쓰기.
						//	// param1 == 결과.
						//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_END, 
						//						0, (cltCharServer*)pclcharshopper, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
						//						2, 0, 0, 0, 0, NULL, NULL);
						//}

					}
					else	
					{
						cltGameMsgResponse_UpdatePersonalShopData clinfo(materialcharid, &pclcharshopper->clShopData);
						cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
						pclcharshopper->SendNetMsg((sPacketHeader*)&clMsg);
					}

					// 개인상점 갱신정보를 보낸다. 제작자 한테 
					{
						cltGameMsgResponse_UpdatePersonalShopData clinfo(materialcharid, &pclcharshopper->clShopData);
						cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					//---------------------------------------------------------------
				}
				else // 상점을 닫도록 한다.
				{
					if(pclcharshopper->bPersonalShopSwitch == false)	
						pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;

					// 개인상점 갱신정보를 보낸다.
					cltGameMsgResponse_UpdatePersonalShopData clinfo(materialcharid, &pclcharshopper->clShopData);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_ENCHANTITEMINV(sPacketHeader* pPacket)
{
	sDBResponse_EnchantItemInv* pclMsg = (sDBResponse_EnchantItemInv*)pPacket;

	//----------------------------
	// 아이디의 적절성을 확인한다. 
	//----------------------------
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)return ;
	if(pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)return ;

	// 주문자의 아이디를 확보한다. 
	SI32 materialcharid;
	if(pclMsg->siPersonID == pclMsg->siMaterialPersonID)
	{
		materialcharid = id;
	}
	else
	{
		materialcharid = pclCM->GetIDFromPersonID(pclMsg->siMaterialPersonID);
	}

	if( pclCM->IsValidID(materialcharid) == false )		return;
	// 아이템이 추가되는 자리에 되어 있던 예약을 해지한다. 
	cltCharServer* pclMaterialChar = (cltCharServer*)pclCM->CR[materialcharid];
	if(pclMaterialChar == NULL)return ;

	pclMaterialChar->SetInputItemReserve(pclMsg->siOutputPos, 0);


	bool bupdateinvswitch = false;
	bool bcontinueswitch = false;
	cltServer * pclserver = (cltServer*)pclClient;
	switch(pclMsg->siResult)
	{

	case 1:		// 정상적인 성공. 
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 11] // 인벤토리내에서 아이템 만들기.
					// param1 == 바뀐 물건의 위치. 
					// param2 == 인챈트 성공 여부
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_ENCHANT_ITEM_INV, 
										0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siMaterialPersonID, 0, 0, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
										pclMsg->siOutputPos, pclMsg->siSuccess, 0, 0, 0, 
										NULL, NULL);
				}

		break;							

	case 2:								// 완성품 놓을 자리에 이미 다른 물품이 존재. 
		{
			if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
			{
				pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);

				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_MAKEITEMFAIL_POS, 0, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siOutputPos], 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
			}
			return;	
		}
		break;
	case 0:
	case 3:								// 수수료가 부족하여 물품 제작 실패. 
	default:
		return ;
	}

	if ( pclMsg->siSuccess > 0 )
	{
		// 길드가 있다면 GP를 업데이트한다.
		if ( pclCM->CR[id]->pclCI->clBI.clGuildRank.IsGuildMember() == true )
		{
			cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCM->CR[id]->pclCI->clBI.clGuildRank.GetGuildUnitIndex());
			if ( pGuild )
			{
				if ( pclMsg->clItem.Element.siPower == 5 )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCM->CR[id]->pclCI->GetPersonID(),20,TYPE_GUILDPOINT_ADD_ENCHANT_5);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
				else if ( pclMsg->clItem.Element.siPower == 6 )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCM->CR[id]->pclCI->GetPersonID(),50,TYPE_GUILDPOINT_ADD_ENCHANT_6);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
				else if ( pclMsg->clItem.Element.siPower == 7 )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCM->CR[id]->pclCI->GetPersonID(),100,TYPE_GUILDPOINT_ADD_ENCHANT_7);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
				else if ( pclMsg->clItem.Element.siPower == 8 )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCM->CR[id]->pclCI->GetPersonID(),300,TYPE_GUILDPOINT_ADD_ENCHANT_8);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
				else if ( pclMsg->clItem.Element.siPower == 9 )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCM->CR[id]->pclCI->GetPersonID(),700,TYPE_GUILDPOINT_ADD_ENCHANT_9);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
				else if ( pclMsg->clItem.Element.siPower == 10 )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCM->CR[id]->pclCI->GetPersonID(),1500,TYPE_GUILDPOINT_ADD_ENCHANT_10);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}
		}
	}

	//--------------------------
	// 인벤을 업데이트 한다. 
	//--------------------------
	if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);


	//================================================ 
	// Client의 정보를 인벤토리 Update한다. by tae-yang
	//================================================ 
	if(pclMaterialChar->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		{
			pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	//-------------------------------------------------
	// 인챈트 창을 업데이트 한다.
	//-------------------------------------------------
	if(pclMaterialChar->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		{
			pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONENCHANTITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}




	//================================================ 
	// Client의 정보를 인벤토리 Update한다. by tae-yang
	//================================================ 
	if(pclMaterialChar->pclCI->GetPersonID() != pclMsg->siPersonID && pclMsg->siMaterialPersonID !=0)
	{
		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		{
			pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[materialcharid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	//-------------------------------------------------
	// 인챈트 창을 업데이트 한다.
	//-------------------------------------------------
	if(pclMaterialChar->pclCI->GetPersonID() != pclMsg->siPersonID && pclMsg->siMaterialPersonID !=0)
	{
		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		{
			pclMaterialChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONENCHANTITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[materialcharid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}


	SI32 skillunique	= 0;
	SI32 skillexp		= 0;

	if(pclMsg->siResult)
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];


		// 물품 제조 결과를 통보한다. 
		cltItem clmadeitem;
		clmadeitem.Set(&pclMsg->clItem);

		cltGameMsgResponse_EnchantItemDlg clinfo(&clmadeitem, pclMsg->siSuccess );
		cltMsg clMsg(GAMEMSG_RESPONSE_ENCHANTITEMDLG, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if( materialcharid != id )
		{
			pclCM->CR[ materialcharid ]->SendNetMsg((sPacketHeader*)&clMsg);
		}

	}




	// 제작자와 주문자의 소지금을 업데이트 한다. 
	if(pclMsg->siFee)
	{
		{
			pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siPersonIDMoney);

			// 클라이언트로 통보한다. 
			cltGameMsgResponse_ChangeMoney clinfo(pclCM->CR[id]->pclCI->clIP.GetMoney(),0, CHANGE_MONEY_REASON_MAKE);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		{
			pclMaterialChar->pclCI->clIP.SetMoney(pclMsg->siMaterialPersonIDMoney);

			// 클라이언트로 통보한다. 
			cltGameMsgResponse_ChangeMoney clinfo(pclMaterialChar->pclCI->clIP.GetMoney(),0, CHANGE_MONEY_REASON_MAKE);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
			pclMaterialChar->SendNetMsg((sPacketHeader*)&clMsg);
		}

	}


	// 아이템 사용 통계를 업데이트 한다. 
	pclItemManager->AddMakeNum(pclMsg->clItem.siUnique ,1);

	////////////////////////////////////////////
	// 인첸트 퀘스트 중이었다면 퀘스트 정보 업데이트
	////////////////////////////////////////////
	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( NULL == pclChar )
	{
		return;
	}

	pclChar->IncQuestPara(QUEST_CLEAR_COND_ENCHANTITEM, pclMsg->clItem.siUnique, 1);
	
	pclChar->DoQuestCheckAndAction(QUEST_CLEAR_COND_ENCHANTANYITEM);

	if ( pclMsg->clItem.IsWeapon(pclItemManager) )
	{
		SI32 siLevel = pclMsg->clItem.GetEquipLevel( pclItemManager );

		pclChar->IncQuestPara(QUEST_CLEAR_COND_ENCHANTEQUIPLEVELWEAPON, siLevel, 1);
	}
	
	// 아이템 인첸트 성공 공지
	if ( (1 == pclMsg->siResult) && (ENCHANT_SUCCESS == pclMsg->siSuccess) )
	{
		SI32 siNoticeGrade = pclClient->GetGlobalValue( TEXT("GV_Enchant_Success_NoticeLevel") );
		
		if ( (0 < siNoticeGrade) && (siNoticeGrade <= pclMsg->clItem.Element.siPower) )
		{
			cltGameMsgResponse_RareItemMade	clinfo((TCHAR*)pclMaterialChar->GetName(), pclMaterialChar->pclCI->GetPersonID(), 
				&pclMsg->clItem,
				RAREITEM_NOTICE_SWITCH_ENCHANTSUCCESS,
				0 );

			cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMMADE, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
			
		}
	}

	
}

void cltServer::DoMsg_DBMSG_RESPONSE_ENCHANT_USEDAYPLUS(sPacketHeader * pPacket)
{
	sDBResponse_EnchantUseDayPlus * pclMsg = (sDBResponse_EnchantUseDayPlus*)pPacket;

	//----------------------------
	// 아이디의 적절성을 확인한다. 
	//----------------------------
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)return ;
	if(pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)return ;


	// 아이템이 추가되는 자리에 되어 있던 예약을 해지한다. 
	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if(pclChar == NULL)return ;


	pclChar->SetInputItemReserve(pclMsg->siOutputPos, 0);
	pclChar->SetInputItemReserve(pclMsg->siOutputMaterialPos, 0);

	if( pclMsg->siResult == 1)
	{
		//--------------------------
		// 인벤을 업데이트 한다. 
		//--------------------------
		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON &&
			pclMsg->siOutputMaterialPos >= 0 && pclMsg->siOutputMaterialPos < MAX_ITEM_PER_PERSON )
		{
			pclChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);
			pclChar->pclCI->clCharItem.clItem[pclMsg->siOutputMaterialPos].Set(&pclMsg->clMaterialItem);
		}

		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		{
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);	
		}

		if( pclMsg->siOutputMaterialPos >= 0 && pclMsg->siOutputMaterialPos < MAX_ITEM_PER_PERSON )
		{
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputMaterialPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clMaterialItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);	
		}

		cltGameMsgResponse_EnchantItemDlg clinfo(&pclMsg->clItem, ENCHANT_USEDAYPLUS );
		cltMsg clMsg(GAMEMSG_RESPONSE_ENCHANTITEMDLG, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	if(IsWhereServiceArea(ConstSwitch_NewLog))
	{
		cltItem clMaterialItem;
		clMaterialItem.Set(&pclMsg->clMaterialItem);
		clMaterialItem.SetItemNum(1);

		// 기간 연장 아이템 사용
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
			USEITEM_REASON_USEDAYPLUS, pclChar, 
			NULL, 0, 0, 0, &clMaterialItem, pclMsg->clMaterialItem.GetItemNum(), 0, 0, 0, 0, 0, 
			pclMsg->siOutputMaterialPos, 0, 0, 0, 0, 
			NULL, NULL);
		
		cltItem& pclItemPlused = pclMsg->clItem;

		TCHAR szUseStartdate[128] = TEXT("") ;
		TCHAR* pText = GetTxtFromMgr(5600) ;
		StringCchPrintf(szUseStartdate, 128, pText, pclItemPlused.uiDateYear+2000,pclItemPlused.uiDateMonth,pclItemPlused.uiDateDay) ;

		NDate ndate;
		ndate.SetDate(pclItemPlused.uiDateYear+2000,pclItemPlused.uiDateMonth,pclItemPlused.uiDateDay);
		ndate.AddDays(pclItemPlused.uiDateUseDay);

		TCHAR szUseEnddate[128] = TEXT("") ;
		pText = GetTxtFromMgr(5601) ;
		StringCchPrintf(szUseEnddate, 128, pText,ndate.m_uiYear,ndate.m_uiMonth,ndate.m_uiDay);

		// 기간 연장 아이템 사용
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_EXTENDTHETERM, 
			0, pclChar , 
			NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
			pclMsg->siOutputPos, 0, 0, 0, 0, 
			szUseStartdate, szUseEnddate);
	}

}

// [영훈] 아이템 인첸트 속성 변경에 대한 응답 - 2008.02.20
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGEENCHANT(sPacketHeader* pPacket)
{
	sDBResponse_ChangeEnchant* pclMsg = (sDBResponse_ChangeEnchant*)pPacket;

	//----------------------------
	// 아이디의 적절성을 확인한다. 
	//----------------------------
	SI32 siID = pclMsg->usCharID;

	if (pclCM->IsValidID(siID) == false)								return ;
	if (pclCM->CR[siID]->pclCI->GetPersonID() != pclMsg->siPersonID)	return ;

	// 아이템이 추가되는 자리에 되어 있던 예약을 해지한다. 
	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[siID];
	if (pclChar == NULL)												return ;

	pclChar->SetInputItemReserve(pclMsg->siOutputPos, 0);
	pclChar->SetInputItemReserve(pclMsg->siOutputMaterialPos, 0);

	if( pclMsg->siResult == 1)
	{
		//--------------------------
		// 인벤을 업데이트 한다. 
		//--------------------------
		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON &&
			pclMsg->siOutputMaterialPos >= 0 && pclMsg->siOutputMaterialPos < MAX_ITEM_PER_PERSON )
		{
			pclChar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);
			pclChar->pclCI->clCharItem.clItem[pclMsg->siOutputMaterialPos].Set(&pclMsg->clMaterialItem);
		}

		if( pclMsg->siOutputPos >= 0 && pclMsg->siOutputPos < MAX_ITEM_PER_PERSON )
		{
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);	
		}

		if( pclMsg->siOutputMaterialPos >= 0 && pclMsg->siOutputMaterialPos < MAX_ITEM_PER_PERSON )
		{
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputMaterialPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clMaterialItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);	
		}

		cltGameMsgResponse_EnchantItemDlg clinfo(&pclMsg->clItem, ENCHANT_CHANGE );
		cltMsg clMsg(GAMEMSG_RESPONSE_ENCHANTITEMDLG, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}

// 아이템을 Person의 ItemPos로 넣으라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_INPUTITEMTOPERSON(sPacketHeader* pPacket)
{
	sDBResponse_InputItemToPerson* pclMsg = (sDBResponse_InputItemToPerson*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return;
	}

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if(pclCM->IsValidID(id) == FALSE)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )		return;
 
	// 메시지를 보낸 놈이 대상자와 같다면, 
	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 

		SI32 personitemoption = PERSONITEMINFO_OPTION_NONE;

		switch ( pclMsg->siInputItemPersonOption )
		{
			case INPUTITEMPERSON_OPTION_WAR:
				{
					personitemoption = PERSONITEMINFO_OPTION_WARITEM;	
				} 
				break;

			case INPUTITEMPERSON_OPTION_SPEECIALQUEST:
				{
					personitemoption = PERSONITEMINFO_OPTION_SPECIALQUEST;
				}
				break;

			case INPUTITEMPERSON_OPTION_MASTER:
				{
					personitemoption = PERSONITEMINFO_OPTION_MASTER;
				}
				break;

			case INPUTITEMPERSON_OPTION_EVENT:
				{
					personitemoption = PERSONITEMINFO_OPTION_EVENT;
				}
				break;

			case INPUTITEMPERSON_OPTION_QUEST:
				{
					personitemoption = PERSONITEMINFO_OPTION_QUEST;
				}
				break;

			case INPUTITEMPERSON_OPTION_FAMELEVEL:
				{
					personitemoption = PERSONITEMINFO_OPTION_FAMELEVEL;
				}
				break;

			case INPUTITEMPERSON_OPTION_ROULETTEGAME:
				{
					personitemoption = PERSONITEMINFO_OPTION_ROULETTEGAME;
				}
				break;

			case INPUTITEMPERSON_OPTION_BEGINNERWEAPON:
				{
					personitemoption = PERSONITEMINFO_OPTION_SHOW;
					pclchar->pclCI->clBI.bReceiveBeginnerWeapon = true;
				}
				break;

			case INPUTITEMPERSON_OPTION_MOFUMOFUEVENT:
				{
					personitemoption = PERSONITEMINFO_OPTION_MOFUMOFUEVENT;
				}
				break;

			case INPUTITEMPERSON_OPTION_SERVER_EVENT:
				{
					personitemoption = PERSONITEMINFO_OPTION_SERVEREVENTQUEST;
				}
				break;
			case INPUTITEMPERSON_OPTION_CHAR_ROULETTE:
				{
					personitemoption = PERSONITEMINFO_OPTION_HALLOWEENEVENT;
				}
				break;
			case INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE_SPECIAL:
				{
					personitemoption = PERSONITEMINFO_OPTION_PVP_LEAGUE;

				}break;
			case INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE:
				{
					personitemoption = PERSONITEMINFO_OPTION_PVP_LEAGUE;

				}break;
			case INPUTITEMPERSON_OPTION_NEWYEAREVENT_ROULETTE:
				{
					personitemoption = PERSONITEMINFO_OPTION_NEWYEAREVENT;
				}break;
			case INPUTITEMPERSON_OPTION_DORMANCYACCOUNT:
				{
					personitemoption = PERSONITEMINFO_OPTION_DORMANCY;	
				} 
				break;
			case INPUTITEMPERSON_OPTION_SEQGMCOMMAND:
				{
					personitemoption = PERSONITEMINFO_OPTION_SEQGMCOMMAND;
				}break;
			case INPUTITEMPERSON_OPTION_BATTLEROYAL_KILLINGUSER:
			case INPUTITEMPERSON_OPTION_BATTLEROYAL_WINNER:
				{
					personitemoption = PERSONITEMINFO_OPTION_BATTLEROYAL;
				}break;
			case INPUTITEMPERSON_OPTION_ADDITIONALITEM_GET:
				{
					personitemoption = PERSONITEMINFO_OPTION_ADDITIONALITEM_GET;
				}break;
			// [성웅] 레벨 보상 상자에서 아이템을 준다 
			case INPUTITEMPERSON_OPTION_LIMITLEVELBOX:
				{
					personitemoption = PERSONITEMINFO_OPTION_ADDITIONALITEM_GET;
				}break;
		}
				
		// 아이템의 시세를 구한다. 
		cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclItemManager, pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].siUnique);
		GMONEY price	= 0;
		if(pclprice)
		{
			price = pclprice->siPrice;
		}

		// 아이템이 추가되는 자리에 되어 있던 예약을 해지한다. 
		pclchar->SetInputItemReserve(pclMsg->siItemPos, 0);


		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, personitemoption, pclMsg->siChangedNum, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos], price);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if( pclMsg->siInputItemPersonOption == INPUTITEMPERSON_OPTION_WAR)
		{
			cltCharServer* pclQuestChar = (cltCharServer*)pclCM->CR[id];

			if ( NULL == pclQuestChar )
			{
				return;
			}

			// [영훈] 퀘스트 카운트 적용
			SI32 siQuestItemNum = pclMsg->siChangedNum;

			if ( 0 < pclQuestChar->siQuestCount )
			{
				siQuestItemNum = pclQuestChar->siQuestCount;
			}

			// 퀘스트 업데이트
			pclQuestChar->IncQuestPara(QUEST_CLEAR_COND_HUNT_ITEM, pclMsg->clItem.siUnique, siQuestItemNum );
			

			if ( pclCM->CR[id]->siPartyIndex > 0 ) 
			{
				CPartyObj* pParty = pclPartyMgr->GetPartyObj(pclCM->CR[id]->siPartyIndex);

				if(pParty)
				{
					cltItem clItem;
					clItem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos]);
					clItem.siItemNum = pclMsg->siChangedNum;
					pParty->NotifyGetIemToPartyMember(pclCM->CR[id]->GetCharUnique(), &clItem );
				}
			}
		}

		if(IsWhereServiceArea(ConstSwitch_NewLog))
		{
			if( pclMsg->siInputItemPersonOption == INPUTITEMPERSON_OPTION_MAKEITEM_MATERIAL )
			{
				// 제조 재료 사용 로그 기록 - DB에서 위와 같은 옵션으로 전달해 준다.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
									USEITEM_REASON_MAKE_MATERIAL, (cltCharServer*)pclCM->CR[id], 
									NULL, 0, 0, 0, &pclMsg->clInputedItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
									pclMsg->siItemPos, 0, 0, 0, 0, 
									NULL, NULL);
			}
			else if( pclMsg->siInputItemPersonOption == INPUTITEMPERSON_OPTION_ENCHANT_MATERIAL )
			{
				// 제조 재료 사용 로그 기록 - DB에서 위와 같은 옵션으로 전달해 준다.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
					USEITEM_REASON_ENCHANT_MATERIAL, (cltCharServer*)pclCM->CR[id], 
					NULL, 0, 0, 0, &pclMsg->clInputedItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
					pclMsg->siItemPos, 0, 0, 0, 0, 
					NULL, NULL);
			}
			else
			{
				// [로그추가 : 황진성 2007. 10. 15] // 아이템 획득 방식에 따른 로그 추가.
				// param1: 인벤토리 위치
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_GET_ITEM, 
									pclMsg->siInputItemPersonOption, (cltCharServer*)pclCM->CR[id], 
									NULL, 0, 0, 0, &pclMsg->clInputedItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
									pclMsg->siItemPos, 0, 0, 0, 0, 
									NULL, NULL);
			}
		}
	}
}

// 아이템을 Person의 ItemPos에서 빼라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_OUTPUTITEMFROMPERSON(sPacketHeader* pPacket)
{
	sDBResponse_OutputItemFromPerson* pclMsg = (sDBResponse_OutputItemFromPerson*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return;
	}

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )		return;

	// 메시지를 보낸 놈이 대상자와 같다면, 
	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos], 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}



// 아이템을 Person의 한 위치에서 다른 위치로 이동하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MOVEITEM(sPacketHeader* pPacket)
{
	sDBResponse_MoveItem* pclMsg = (sDBResponse_MoveItem*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return;
	}

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if( pclMsg->siFromItemPos < 0	|| pclMsg->siFromItemPos >= MAX_ITEM_PER_PERSON )		return;
	if( pclMsg->siToItemPos < 0		|| pclMsg->siToItemPos >= MAX_ITEM_PER_PERSON )		return;

	// 메시지를 보낸 놈이 대상자와 같다면, 
	if(pclchar->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		//---------------------------------------------
		// 서버를 업데이트 한다. 
		//---------------------------------------------

		// [진성] 변경전에 차고 있던 무기를 기억한다.
		SI32 siPrevActiveWeapon = pclchar->pclCI->clCharItem.clItem[pclchar->ActiveWeapon].siUnique;

		pclchar->pclCI->clCharItem.clItem[pclMsg->siFromItemPos].Set(&pclMsg->clFromItem);
		pclchar->pclCI->clCharItem.clItem[pclMsg->siToItemPos].Set(&pclMsg->clToItem);

		pclchar->SetParametaBoxAction(true);	// ParametaBoxAction을 수행한다. 

		//----------------------------------------------
		// 클라이언트로 업데이트 된 정보를 보낸다. 
		//----------------------------------------------


		cltGameMsgResponse_PersonItemInfo clinfo1(pclMsg->siFromItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clFromItem, 0);
		cltMsg clMsg1(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo1), (BYTE*)&clinfo1);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg1);

		cltGameMsgResponse_PersonItemInfo clinfo2(pclMsg->siToItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clToItem, 0);
		cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo2), (BYTE*)&clinfo2);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg2);

		// 인벤토리내에서 아이템 옮길경우 세트아이템 확인을 해주자
		if( pclMsg->siFromItemPos < PERSONITEM_INV0 ||
			pclMsg->siToItemPos < PERSONITEM_INV0 )
		{
			cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclClient->pclItemManager;
			
			SI16 setItemType  = 0;
			SI16 setItemStep  = 0;
			SI16 setItemType2 = 0;
			SI16 setItemStep2 = 0;
			SI16 setItemType3 = 0;
			SI16 setItemStep3 = 0;
			SI16 setItemType4 = 0;
			SI16 setItemStep4 = 0;
			SI16 setItemType5 = 0;
			SI16 setItemStep5 = 0;

			if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{
				setItemType = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep, USE_SETITEM_1 );
				pclchar->m_siSetItemType = setItemType;

				setItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep2, USE_SETITEM_2 );
				pclchar->m_siSetItemType2 = setItemType2;

				setItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep3, USE_SETITEM_3 );
				pclchar->m_siSetItemType3 = setItemType3;

				setItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep4, USE_SETITEM_4 );
				pclchar->m_siSetItemType4 = setItemType4;

				setItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep5, USE_SETITEM_5 );
			}
			else
			{
				setItemType = pclitemmanager->pclSetItemManager->FindSetItemType( &pclchar->pclCI->clCharItem.clItem[0] );
			}
			pclchar->m_siSetItemType = setItemType;
			pclchar->m_siSetItemStep = setItemStep;

			pclchar->m_siSetItemType2 = setItemType2;
			pclchar->m_siSetItemStep2 = setItemStep2;

			pclchar->m_siSetItemType3 = setItemType3;
			pclchar->m_siSetItemStep3 = setItemStep3;

			pclchar->m_siSetItemType3 = setItemType4;
			pclchar->m_siSetItemStep3 = setItemStep4;

			pclchar->m_siSetItemType3 = setItemType5;
			pclchar->m_siSetItemStep3 = setItemStep5;

			cltGameMsgResponse_SetItemTypeInfo clinfo3( setItemType, setItemStep, setItemType2, setItemStep2, setItemType3, setItemStep3, setItemType4, setItemStep4, setItemType5, setItemStep5 );
			cltMsg clMsg3(GAMEMSG_RESPONSE_SETITEMTYPEINFO, sizeof(clinfo3), (BYTE*)&clinfo3);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg3);
		}

		//----------------------------------------------
		// 장착, 탈착된 장비가 EquipShape라면 이웃들에게도 알린다. 
		//----------------------------------------------
		if(    pclMsg->siFromItemPos	== PERSONITEM_WEAPON1
			|| pclMsg->siFromItemPos	== PERSONITEM_WEAPON2
			|| pclMsg->siFromItemPos	== PERSONITEM_HELMET
			|| pclMsg->siFromItemPos	== PERSONITEM_HAT
			|| pclMsg->siFromItemPos	== PERSONITEM_ARMOUR
			|| pclMsg->siFromItemPos	== PERSONITEM_DRESS
			|| pclMsg->siFromItemPos	== PERSONITEM_MANTLE
			|| pclMsg->siFromItemPos	== PERSONITEM_NECK
			|| pclMsg->siFromItemPos	== PERSONITEM_RING1
			|| pclMsg->siFromItemPos	== PERSONITEM_RING2
			|| pclMsg->siFromItemPos	== PERSONITEM_SHOES
			|| pclMsg->siFromItemPos	== PERSONITEM_BELT
			|| pclMsg->siToItemPos		== PERSONITEM_WEAPON1
			|| pclMsg->siToItemPos		== PERSONITEM_WEAPON2
			|| pclMsg->siToItemPos		== PERSONITEM_HELMET
			|| pclMsg->siToItemPos		== PERSONITEM_HAT
			|| pclMsg->siToItemPos		== PERSONITEM_ARMOUR
			|| pclMsg->siToItemPos		== PERSONITEM_DRESS
			|| pclMsg->siToItemPos		== PERSONITEM_MANTLE
			|| pclMsg->siToItemPos		== PERSONITEM_NECK
			|| pclMsg->siToItemPos		== PERSONITEM_RING1
			|| pclMsg->siToItemPos		== PERSONITEM_RING2
			|| pclMsg->siToItemPos		== PERSONITEM_SHOES
			|| pclMsg->siToItemPos		== PERSONITEM_BELT)
		{

			// 무기 사용 부적을 사용한 것이 있는가 ?
			// skill : SKILL_USESWORD1 ...
			pclchar->CheckWeaponSkillScroll(pclItemManager);

			pclchar->SendNeighbourMyShapeStatus();
			pclchar->CheckCurrentMagic( siPrevActiveWeapon );
		}

		// [로그추가 : 황진성 2007. 10. 18] // person이 아이템 창간에 아이템을 이동시킨다. (인벤에서 갑옷창이나 그 반대와 같은 경우. ) 
		// param1 == 이전아이템 위치, 
		// param2 == 옮긴후의 아이템 위치, 
		// param3 == 이전 아이템의 남은개수
		// charparam1 = "ToItemPos = %d"
		// charparam2 = "ToItemUnique = %d, ToItemNum = %d"

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_MOVE_ITEM, 
							0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, 
							&pclMsg->clMoveItem, pclMsg->clToItem.siItemNum, 0, 0, 0, 0, 0, 
							pclMsg->siFromItemPos, pclMsg->siToItemPos, pclMsg->clFromItem.siItemNum, 0, 0, 
							NULL, NULL);
	}
}

// 아이템을 Person에서 다른 Person으로 옮기라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MOVEITEMPERSON(sPacketHeader* pPacket)
{
	sDBResponse_MoveItemPerson* pclMsg = (sDBResponse_MoveItemPerson*)pPacket;

	if( 1 != pclMsg->siResult )
	{
		return;
	}

	// 통계
	NStatistics_Add("Item", pclMsg->siDestroyItemUnique, "PKDestroy", 1);
	NStatistics_Set("Item", pclMsg->siDestroyItemUnique, "Name", pclItemManager->GetName(pclMsg->siDestroyItemUnique));

	SI32 winID = pclCM->GetIDFromPersonID( pclMsg->siToPersonID );
	SI32 loseID = pclCM->GetIDFromPersonID( pclMsg->siFromPersonID );

	if(winID && loseID)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // Person간에 아이템을 이동. pk시...
			// param1 = 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_MOVE_ITEM_TO_PERSON, 
								 pclMsg->siReason, (cltCharServer*)pclCM->CR[loseID], NULL, pclMsg->siToPersonID, 0, 0, &pclMsg->clMoveItem, pclMsg->clToItem.siItemNum, 0, 0, 0, 0, 0, 
								 pclMsg->siFromItemPos, pclMsg->siToItemPos, pclMsg->bDestroy, 0, 0, 
								 NULL, NULL);
		}	
	}

	// 진사람
	if( pclCM->IsValidID( loseID ) )
	{
		pclCM->CR[loseID]->pclCI->clCharItem.clItem[pclMsg->siFromItemPos].Set(&pclMsg->clFromItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo( pclMsg->siFromItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clFromItem, 0 );
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[loseID]->SendNetMsg((sPacketHeader*)&clMsg);

		if( MOVEITEMPERSON_PVP == pclMsg->siReason )
		{
		//	if( false == pclMsg->bDestroy )
			{
				// 마을 사람들에게 알려주기
				SI32 homeVillageUnique = pclCM->CR[loseID]->pclCI->clBI.siHomeVillage;
				if( homeVillageUnique > 0 )
				{
					SI32 id;

					SI32 index = 0;
					while( id = pclCM->GetCharOrder(index) )
					{
						index++;
						if( id == loseID )
							continue;

						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
						if( pclchar->GetSession() )
						{
							if( pclchar->pclCI->clBI.siHomeVillage == homeVillageUnique )
							{
								cltLetterMsg_PVPResult clletter( false, cltLetterMsg_PVPResult::GUILD, pclMsg->szToUserName, (TCHAR*)pclCM->CR[loseID]->GetName(), pclMsg->bDestroy, pclMsg->siDestroyItemUnique );
								SendLetterMsg( pclchar->pclCI->GetPersonID(), (cltLetterHeader*)&clletter );
							}

						}
					}
				}

				// 길드원에게 내가 죽었다고 알려주기
				if ( pclCM->CR[loseID]->pclCI->clBI.clGuildRank.IsGuildMember() == true )
				{
					cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo( pclCM->CR[loseID]->pclCI->clBI.clGuildRank.GetGuildUnitIndex() );
					if ( pGuild )
					{
						for( SI32 i = 0; i < MAX_GUILD_MEMBER_NUM; ++i )
						{
							if( pGuild->clMember[i].siPersonID > 0 )
							{
								cltLetterMsg_PVPResult clletter( false, cltLetterMsg_PVPResult::GUILD, pclMsg->szToUserName, (TCHAR*)pclCM->CR[loseID]->GetName(), pclMsg->bDestroy, pclMsg->siDestroyItemUnique );
								SendLetterMsg( pGuild->clMember[i].siPersonID, (cltLetterHeader*)&clletter );
							}
						}
					}
				}
				if( true != pclMsg->bDestroy )
				{
					cltLetterMsg_PVPResult clletter( false, cltLetterMsg_PVPResult::LOSE, pclMsg->szToUserName, (TCHAR*)pclCM->CR[loseID]->GetName(), pclMsg->bDestroy, pclMsg->siDestroyItemUnique );
					SendLetterMsg( pclCM->CR[loseID]->pclCI->GetPersonID(), (cltLetterHeader*)&clletter );
				}
			}

			// 무기를 빼았겨서 기존 엠블렘이 바뀌는지 검사
			cltCharServer* pclLoseChar = pclCM->GetCharServer( loseID );
			if ( (pclLoseChar) && (pclMsg->siFromItemPos == pclLoseChar->ActiveWeapon) )
			{
				SI32 siItemType		= pclMsg->clFromItem.GetType( pclItemManager );
				SI32 siSkillUnique	= pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );

				if ( (0 < pclLoseChar->m_uiWeaponSkillMasterEmblem) && (0 >= siSkillUnique) )
				{
					pclLoseChar->m_uiWeaponSkillMasterEmblem = 0;
					
					cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_EMBLEM, false, 0 );
					cltGameMsgResponse_StatusInfo	clInfo( pclLoseChar->GetCharUnique(), &clChangeInfo );

					cltMsg clEmblenMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clInfo), (BYTE*)&clInfo);
					pclLoseChar->SendNeighbourMsg( &clEmblenMsg, true );
				}
			}
		}	// 종료	if( MOVEITEMPERSON_PVP == pclMsg->siReason )
	}

	// 이긴사람
	if( pclCM->IsValidID( winID ) )
	{
		if( MOVEITEMPERSON_PVP == pclMsg->siReason )
		{
			if( true == pclMsg->bDestroy )
			{
				cltLetterMsg_PVPResult clletter( true, cltLetterMsg_PVPResult::WIN, (TCHAR*)pclCM->CR[winID]->GetName(), pclMsg->szFromUserName, pclMsg->bDestroy, pclMsg->siDestroyItemUnique );
				SendLetterMsg( pclMsg->siToPersonID, (cltLetterHeader*)&clletter );

				cltLetterMsg_PVPResult clletter2( false, cltLetterMsg_PVPResult::LOSE, (TCHAR*)pclCM->CR[winID]->GetName(), pclMsg->szFromUserName, pclMsg->bDestroy, pclMsg->siDestroyItemUnique );
				SendLetterMsg( pclMsg->siFromPersonID, (cltLetterHeader*)&clletter2 );
			}
			else
			{
				cltLetterMsg_PVPResult clletter( true, cltLetterMsg_PVPResult::WIN, (TCHAR*)pclCM->CR[winID]->GetName(), pclMsg->szFromUserName, pclMsg->bDestroy, pclMsg->siDestroyItemUnique );
				SendLetterMsg( pclMsg->siToPersonID, (cltLetterHeader*)&clletter );
			}
		}

		pclCM->CR[winID]->pclCI->clCharItem.clItem[pclMsg->siToItemPos].Set(&pclMsg->clToItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siToItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clToItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[winID]->SendNetMsg((sPacketHeader*)&clMsg);
	}

/*	if(pclCM->IsValidID(id) )
	{
		// 메시지를 보낸 놈이 대상자와 같다면, 
		if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siFromPersonID)
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siFromItemPos].Set(&pclMsg->clFromItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siFromItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clFromItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}

		if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siToPersonID)
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToItemPos].Set(&pclMsg->clToItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siToItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clToItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		}
	}*/
}

// 아이템을 사용하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_USEITEM(sPacketHeader* pPacket)
{
	sDBResponse_UseItem* pclMsg = (sDBResponse_UseItem*)pPacket;

	if(pclMsg->siResult != 1)
	{
		return;
	}
	// 통계
	switch(pclMsg->siUseReason)
	{
	case USEITEM_REASON_DATEOVER:		// 기간제 아이템 기간 끝
		{	NStatistics_Add("Item", pclMsg->clUsedItem.siUnique, "DateOver", pclMsg->clUsedItem.siItemNum);
			NStatistics_Set("Item", pclMsg->clUsedItem.siUnique, "Name", pclItemManager->GetName(pclMsg->clUsedItem.siUnique));
		}	break;
	}

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if (pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )		return;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [로그추가 : 황진성 2007. 10. 10] // 아이템 사용.
		// param1 == 사용된 아이템의 위치. 
		// param2 == 사용되고 남은 개수,
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
							 pclMsg->siUseReason, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clUsedItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
							 pclMsg->siItemPos, 0, 0, 0, 0, NULL, NULL);
	}

	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);


		if( pclClient->IsCountrySwitch(Switch_TreasureEvent) )
		{
			if ( pclMsg->clUsedItem.siUnique == ITEMUNIQUE(13058))
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION__TREASUREEVENTITEMUSE, 0, 0, &pclMsg->clItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			else
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		else
		{
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		//--------------------------------------------
		// 선조치 아이템은 이미 조치를 취열했다. 
		//--------------------------------------------
		SI64 atb = pclItemManager->GetItemAtb(pclMsg->clUsedItem.siUnique);
		if(atb & ITEMATB_FIRSTACTION || pclItemManager->IsItemInfoAtb(pclMsg->clUsedItem.siUnique, ITEMINFOATB_FIRSTACTION) )
		{
		}
		else
		{
			switch(pclMsg->siUseReason)
			{
			case USEITEM_REASON_DATEOVER:		// 기간제 아이템 기간 끝
			case USEITEM_REASON_ROULLET:		// 박대박으로 사용
			case USEITEM_REASON_BELONGITEM:		// 귀속아이템 파괴
			case USEITEM_REASON_PARTYHALLGIFT:		// 귀속아이템 파괴
				{
					// 아무처리를 하지 않는다.
				}
				break;
			default:
				{
					if(((cltCharServer*)pclCM->CR[id])->UseItem(pclMsg->clUsedItem) == true)
					{

					}
				}
				break;
			}
			//if( pclMsg->siUseReason != USEITEM_REASON_ROULLET )	// 박대박 게임중이면 안한다
			//{
			//}
		}

		// 이호식 분해하였을 경우 국가(마을) 자본금 +5000
		if ( pclMsg->siUseReason == USEITEM_REASON_DISASSEMBLE )
		{
			SI32 currentVillage = pclCM->CR[id]->GetCurrentVillageUnique();
			if( currentVillage>0 && currentVillage < MAX_VILLAGE_NUMBER)
			{
				GMONEY siGetVillageMoney = 5000; // 마을 이익금 증가 SI32 -> GMONEY
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, currentVillage, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}

		// 아이템 사용 통계를 업데이트 한다. 
		pclItemManager->AddUseNum(pclMsg->clUsedItem.siUnique, pclMsg->clUsedItem.siItemNum);

		// 새로운 통계 - 아이템 사용
		//NStatistics_Add("Item", pclMsg->clUsedItem.siUnique, "UseNum", pclMsg->clUsedItem.siItemNum);

	}
}

// 아이템 내구도 소모에 대한 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_WASTEDURABILITY(sPacketHeader* pPacket)
{
	sDBResponse_WasteDurability* pclMsg = (sDBResponse_WasteDurability*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return;
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 11] // 내구도 변경 기록.
		// param1 == 위치.
		SI32 siCharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
		cltCharServer* pclchar = (cltCharServer*)pclCM->GetChar(siCharID);
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_WASTE_DUR_ITEM, 
							 0, pclchar, NULL, 0, 0, 0, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
							 pclMsg->siFromPos, 0, 0, 0, 0, NULL, NULL);
	}
}

// 아이템을 버린다. 
void cltServer::DoMsg_DBMSG_RESPONSE_DROPITEM(sPacketHeader* pPacket)
{
	sDBResponse_DropItem* pclMsg = (sDBResponse_DropItem*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return;
	}

	if( pclMsg->siFromPos < 0 && pclMsg->siFromPos >= MAX_ITEM_PER_PERSON )		return;

	// 떨어뜨린 아이템이 실제로 있어야 한다. 
	if(pclMsg->clChangedItem.siUnique && pclMsg->clChangedItem.siItemNum)
	{
		DropItem(pclMsg->siMapIndex, pclMsg->siX, pclMsg->siY, &pclMsg->clChangedItem, pclMsg->siPersonID);
	}

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 11] // 아이템 버려.
		// param1 == 아이템 위치. 
		// param2 == 지도 번호, 
		// param3 == 지도위치 : x 
		// param4 == 지도위치 : y 

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_DROP_ITEM, 
							 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clChangedItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
							 pclMsg->siFromPos, pclMsg->siMapIndex, pclMsg->siX, pclMsg->siY, 0,
							 NULL, NULL);
	}

	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siFromPos].Set(&pclMsg->clItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siFromPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}

// 아이템 수리에 대한 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_INVREPAIR(sPacketHeader* pPacket)
{
	sDBResponse_InvRepair* pclMsg = (sDBResponse_InvRepair*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return;
	}

	if( pclMsg->siFromPos < 0 && pclMsg->siFromPos >= MAX_ITEM_PER_PERSON )		return;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [로그추가 : 황진성 2007. 10. 11] // 아이템 수리.
		// param1 == 위치. 
		// param2 == 수리비. 
		// param3 == 수리자 보유금, 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_REPAIR_ITEM, 
							 0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siRepairPersonID, 0, pclMsg->siPersonIDMoney, &pclMsg->clItem, 0, 0, 0, pclMsg->siNPCVillageUnique, 0, 0, 
							 pclMsg->siFromPos, pclMsg->siRepairMoney, pclMsg->siRepairerMoney, 0, 0, 
							 NULL, NULL);
	}


	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siFromPos].Set(&pclMsg->clItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siFromPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// 자기자신이 수리하면 수리비용이 0 이므로 다음의 두경우를 모두 SKIP한다.
		// 사용된 수리비용 
		if(pclMsg->siRepairMoney)
		{
			// 의뢰자 자금을 정산한다. 
			pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siPersonIDMoney);

			// 클라이언트로 통보한다. 
			cltGameMsgResponse_ChangeMoney clinfo(pclMsg->siPersonIDMoney, -pclMsg->siRepairMoney, CHANGE_MONEY_REASON_NPCREPAIRPAY);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			// NPC가 수리를 한경우 NPC가 속한 마을에 돈을 적립한다.
			// 10%만 수리비로 
			GMONEY incmoney = pclMsg->siRepairMoney * VILLAGE_PROFIT_RATE_FOR_NPCREPAIR / 100;
			if(incmoney)
			{
				// villageunique로 자금 전달
				SI32 villageunique = pclMsg->siNPCVillageUnique;

				if(villageunique > 0)
				{
					//sDBRequest_ChangeStrMoney sendMsg(CHANGESTRMONEY_REPAIRNPC, 0, villageunique, RANKTYPE_CITYHALL, incmoney);
					sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, villageunique, incmoney );

					pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
				}
			}

		}

		// 수리를 대행해준 수리자의 현금을 설정한다.
		if(pclMsg->siRepairerMoney)
		{
			// 수리자 자금을 정산한다. 
			SI32 repairid = pclCM->GetIDFromPersonID(pclMsg->siRepairPersonID);
			if(pclCM->IsValidID(repairid))
			{
				pclCM->CR[repairid]->pclCI->clIP.SetMoney(pclMsg->siRepairerMoney);

				// 클라이언트로 통보한다. 
				cltGameMsgResponse_ChangeMoney clinfo(pclMsg->siRepairerMoney, pclMsg->siRepairMoney, CHANGE_MONEY_REASON_REPAIRGET);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[repairid]->SendNetMsg((sPacketHeader*)&clMsg);

			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_UPGRADEITEM(sPacketHeader* pPacket)
{
	sDBResponse_UpgradeItem * pclMsg = (sDBResponse_UpgradeItem *)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	// 상단 자금 업데이트
	if ( pclMsg->siVillageUnique > 0 && pclMsg->siVillageUnique <= CUR_VILLAGE_LIMIT &&
		pclMsg->clMoney.itMoney > 0 )
	{
		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
		if( pStrInfo ) 
		{
			pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clMoney );
		}
	}

	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		if ( pclMsg->siFromPos >= 0 && pclMsg->siFromPos < MAX_ITEM_PER_PERSON &&
			 pclMsg->siToPos >= 0	&& pclMsg->siToPos < MAX_ITEM_PER_PERSON )
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siFromPos ].Set( &pclMsg->clFromItem );
			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siToPos ].Set( &pclMsg->clToItem );

			cltGameMsgResponse_UpgradeItem clinfo( pclMsg->bSuccess, pclMsg->siFromPos, &pclMsg->clFromItem, pclMsg->siToPos, &pclMsg->clToItem );
			cltMsg clMsg(GAMEMSG_RESPONSE_UPGRADEITEM, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 15] // 무기 강화.
			// param1 == 성공여부.			 
			// param2 == 이전 아이템 유니크, 
			// param3 == 이전 아이템 위치, 
			// param4 == 현재 아이템 유니크. 
			// param5 == 현재 아이템 위치.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_UPGRADE_ITEM, 
			 					 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								 pclMsg->bSuccess, 
								 pclMsg->clFromItem.siUnique, 
								 pclMsg->siFromPos,
								 pclMsg->clToItem.siUnique,	 
								 pclMsg->siToPos,
								 NULL, NULL);
		}
	}

	if( pclCM->IsValidID(id) == false )
		return;

	// 개조에 성공하면, 
	if( pclMsg->bSuccess )
	{
		// 호품은 공지하지 않는다. 
		if(pclMsg->clToItem.clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1)
		{
			// 혹부리영감을 이용하여 호품 만드는 퀘스트중이라면, 
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_USEWENNYOLDMAN);

			pclCM->CR[id]->pclCI->clBI.bWennyOldManUpgrade = true ;
		}
		else
		{
			// 공지로 띄운다. 
			SendRareItemMade(id, &pclMsg->clToItem, RAREITEM_NOTICE_SWITCH_WENNYOLDMAN);
		}
	}

	return;
}

// 아이템 성능 향상 ( 아이템의 성능을 향상 시킨다. 내구는 깎이면서, 제조 부적을 사용하면서.. )
void cltServer::DoMsg_DBMSG_RESPONSE_PERFORMANCEUPGRADEITEM(sPacketHeader* pPacket)
{
	sDBResponse_PerformanceUpgradeItem * pclMsg = (sDBResponse_PerformanceUpgradeItem *)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		pclLog->FilePrint( TEXT("Config\\UpgradeItem.log"), TEXT("DoMsg_DBMSG_RESPONSE_PERFORMANCEUPGRADEITEM pclMsg->siResult = %d"), pclMsg->siResult );
		return;
	}

	SI32 id = pclMsg->siCharID;

	if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON &&
			pclMsg->siLeftScrollItemPos >= 0 && pclMsg->siLeftScrollItemPos < MAX_ITEM_PER_PERSON )

		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clItem );
			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siLeftScrollItemPos ].Set( &pclMsg->clLeftScrollItem );

			cltGameMsgResponse_PerformanceUpgradeItem clinfo( pclMsg->siItemPos, &pclMsg->clItem, pclMsg->siLeftScrollItemPos, &pclMsg->clLeftScrollItem );
			cltMsg clMsg(GAMEMSG_RESPONSE_PERFORMANCEUPGRADEITEM, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}


		if ( pclMsg->siResult == 1 )
		{
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_USEWENNYOLDMAN2);
		}


		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 15] // 아이템 성능 향상
			// pszParam1 == 성능이 변경될 아이템 정보, 
			// pszParam2 == 사용하고 남은 부적 아이템 정보.
			TCHAR pszCharParam[2][50] = {'\0', };
			StringCchPrintf(pszCharParam[0],  sizeof(pszCharParam[0]), "Item=> Uniqe: %d, Pos: %d, Num: %d", 
																		pclMsg->clItem.siUnique,
																		pclMsg->siItemPos,
																		pclMsg->clItem.siItemNum);
				
			StringCchPrintf(pszCharParam[1],  sizeof(pszCharParam[1]), "ScrollItem=> Uniqe: %d, Pos: %d, Num: %d",
																		pclMsg->clLeftScrollItem.siUnique,
																		pclMsg->siLeftScrollItemPos,
																		pclMsg->clLeftScrollItem.siItemNum);
				
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PERSONITEM_PERFORMANCE_UPGRADE, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											0, 0, 0, 0, 0, pszCharParam[0], pszCharParam[1]);
		}


	}




	return;
}

/*void cltServer::DoMsg_DBMSG_RESPONSE_DELETE_LIMITDATEITEM(sPacketHeader* pPacket)
{
sDBResponse_Delete_LimitDateItem * pclMsg = (sDBResponse_Delete_LimitDateItem*)pPacket;

if ( pclMsg->siResult == 1 )
{
SI32 id = pclMsg->siCharID ;

if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
{
pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
pclCM->CR[id]->pclCI->clLimitDateItem.Delete(pclMsg->siItemUnique);

cltGameMsgResponse_LimitDateItem_Info clinfo(pclCM->CR[id]->pclCI->clLimitDateItem.siItemUnique,pclCM->CR[id]->pclCI->clLimitDateItem.siItemLimitDate,
pclMsg->siItemPos,&pclMsg->clItem);
cltMsg clMsg(GAMEMSG_RESPONSE_LIMITDATEITEM_INFO, sizeof(clinfo), (BYTE*)&clinfo);
pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}
}
else if ( pclMsg->siResult == -1 )
{
SI32 id = pclMsg->siCharID ;

pclCM->CR[id]->pclCI->clLimitDateItem.Delete(pclMsg->siItemUnique);

cltGameMsgResponse_LimitDateItem_Info clinfo(pclCM->CR[id]->pclCI->clLimitDateItem.siItemUnique,pclCM->CR[id]->pclCI->clLimitDateItem.siItemLimitDate,
pclMsg->siItemPos,&pclMsg->clItem);
cltMsg clMsg(GAMEMSG_RESPONSE_LIMITDATEITEM_INFO, sizeof(clinfo), (BYTE*)&clinfo);
pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}
}*/

/*void cltServer::DoMsg_DBMSG_RESPONSE_GETLIMITUSEITEM(sPacketHeader* pPacket)
{
sDBResponse_GetLimitUseItem* pclMsg = (sDBResponse_GetLimitUseItem*)pPacket ;

if ( pclMsg->siResult == 1 )
{
SI32 id = pclMsg->siCharID ;

if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
{
pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
//			pclCM->CR[id]->pclCI->clLimitDateItem.Add(pclMsg->clItem.siUnique,pclMsg->siUseDate) ;

{
cltGameMsgResponse_LimitDateItem_Info clinfo(pclMsg->siItemPos,&pclMsg->clItem);
cltMsg clMsg(GAMEMSG_RESPONSE_LIMITDATEITEM_INFO, sizeof(clinfo), (BYTE*)&clinfo);
pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

// 아이템 넣은 것을 기록한다.
pclLog->FilePrint(TEXT("Config\\InstanceItem.txt"), TEXT("User : [%s] Item : [%s] "),
pclCM->CR[id]->GetName(),pclItemManager->GetName(pclMsg->clItem.siUnique));

{
cltGameMsgResponse_BigWarItem clinfo(pclCM->CR[id]->GetName(), pclCM->CR[id]->pclCI->GetPersonID(), 
&pclMsg->clItem, 0, INPUTITEMPERSON_OPTION_INSTANCEMAP, 0);

cltMsg clMsg(GAMEMSG_RESPONSE_BIGWARITEM, sizeof(clinfo), (BYTE*)&clinfo);
pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}
}
}
else if ( pclMsg->siResult == -1 ) // 아이템이 꽉 차서 못 얻음
{
SI32 id = pclMsg->siCharID ;

if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
{
SendServerResponseMsg(0, SRVAL_ADDINV_FAIL_FULL, pclMsg->clItem.siUnique, 0, pclCM->CR[id]->GetCharUnique());
}
}
}*/

void cltServer::DoMsg_DBMSG_RESPONSE_SETPREMIUM(sPacketHeader* pPacket)
{
	sDBResponse_SetPremium* pclMsg = (sDBResponse_SetPremium*)pPacket ;

	SI32 id = pclMsg->siCharID ;

	if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
		if ( pclchar == NULL ) return ;

		if ( pclMsg->siType == SETPREMIUM_EXP )
			pclchar->bIsSetPremium = false ;
		else if ( pclMsg->siType == SETPREMIUM_COMBAT )
			pclchar->bIsSetCombat = false ;
		else if ( pclMsg->siType == SETPREMIUM_MAKEITEM )
			pclchar->bIsSetMakeItem = false ;
		else if ( pclMsg->siType == SETPREMIUM_BULLET )
			pclchar->bIsBulletService = false ;
		else if ( pclMsg->siType == SETPREMIUM_SUMMONSTIMINA )
			pclchar->bIsSummonStaminaService = false ;


		if ( pclMsg->siResult == 1 )
		{
			if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
				pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

			if ( pclMsg->siType == SETPREMIUM_EXP )
			{
				pclchar->pclCI->clBI.bPremiumService = pclMsg->bPremium ;
				pclchar->pclCI->clBI.siPremiumServiceDate = pclMsg->siPremiumUseDate ;
			}
			else if ( pclMsg->siType == SETPREMIUM_COMBAT )
			{
				pclchar->pclCI->clBI.bCombatService = pclMsg->bPremium ;
				pclchar->pclCI->clBI.siCombatServiceDate = pclMsg->siPremiumUseDate ;
			}
			else if ( pclMsg->siType == SETPREMIUM_MAKEITEM )
			{
				pclchar->pclCI->clBI.bMakeItemService = pclMsg->bPremium ;
				pclchar->pclCI->clBI.siMakeItemServiceDate = pclMsg->siMakeItemDate ;
			}
			else if ( pclMsg->siType == SETPREMIUM_BULLET)
			{
				pclchar->pclCI->clBI.bBulletService = pclMsg->bPremium ;
				pclchar->pclCI->clBI.siBulletServiceDate = pclMsg->siBulletDate;
			}
			else if ( pclMsg->siType == SETPREMIUM_SUMMONSTIMINA)
			{
				pclchar->pclCI->clBI.bSummonStaminaService = pclMsg->bPremium ;
				pclchar->pclCI->clBI.siSummonStaminaServiceDate = pclMsg->siSummonStaminaDate;
			}
			else return ;

			cltGameMsgResponse_Premium_Info clinfo(pclMsg->siItemPos,&pclMsg->clItem,pclMsg->bPremium,
				pclMsg->siPremiumUseDate,pclMsg->siCombatUseDate,pclMsg->siMakeItemDate, pclMsg->siBulletDate, pclMsg->siSummonStaminaDate, pclMsg->siType);
			cltMsg clMsg(GAMEMSG_RESPONSE_PREMIUM_INFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			{				
				// [로그추가 : 황진성 2007. 10. 11] // 프리미엄 효과.(수정필요)
				// param1 == 프리미엄 여부, 
				// param2 == 아이템 위치, 
				// param3 == 타입, 
				// pszcharParam1 == date 정보.
				// pszcharParam2 == date 정보.
				TCHAR pszCharParam1[50] = {'\0', };
				TCHAR pszCharParam2[50] = {'\0', };
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("Date=> PremiumUse: %d, CombatUse: %d, MakeItem: %d"), 																		
																			pclMsg->siPremiumUseDate,
																			pclMsg->siCombatUseDate,
																			pclMsg->siMakeItemDate);


				StringCchPrintf(pszCharParam2, sizeof(pszCharParam2), TEXT("Date=> Bullet: %d, SummonStamina: %d"),	
																			pclMsg->siBulletDate,
																			pclMsg->siSummonStaminaDate);


				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PREMIUM_SET, 
									 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
									 pclMsg->bPremium, pclMsg->siItemPos, pclMsg->siType, 0, 0, 
									 pszCharParam1, pszCharParam2);
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_EXCHANGEITEM(sPacketHeader* pPacket)
{

	sDBResponse_ExchangeItem* pclMsg = (sDBResponse_ExchangeItem*)pPacket;

	if(pclMsg->siResult != 1)
	{
		return;
	}

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if( pclCM->IsValidID(id) == false )
		return;

	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )		return;

	pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clChangedItem);

	// 클라이언트로 업데이트 된 정보를 보낸다. 
	cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clChangedItem, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
    
	// 아이템 사용 통계를 업데이트 한다. 
	pclItemManager->AddUseNum(pclMsg->clItem.siUnique, pclMsg->clItem.siItemNum);

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", pclMsg->clItem.siUnique, "UseNum", pclMsg->clItem.siItemNum);

	/*if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 아이템 교체.
		// param1 == 교체할 아이템 위치,  
		// param2 == 교체할 아이템 유니크.
		// param3 == 교체할 아이템 개수
		// param4 == 교체한 아이템 유니크.
		// param5 == 교체한 아이템 개수.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_EXCHANGE_ITEM_FROM, 
							 pclMsg->siReason, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
							 pclMsg->siItemPos, 
							 pclMsg->clItem.siUnique,		
							 pclMsg->clItem.siItemNum, 
						 	 pclMsg->clChangedItem.siUnique, 
							 pclMsg->clChangedItem.siItemNum, 
							 NULL, NULL);
	}*/
}


void cltServer::DoMsg_DBMSG_RESPONSE_OPENTREASUREBOX(sPacketHeader* pPacket)
{

	sDBResponse_OpenTreasureBox* pclMsg = (sDBResponse_OpenTreasureBox*)pPacket;

	if(pclMsg->siResult != 1)
	{
		return;
	}

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if( pclCM->IsValidID(id) == false )
		return;

	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )		return;
	if( pclMsg->siToItemPos1 < 0 || pclMsg->siToItemPos1 >= MAX_ITEM_PER_PERSON )		return;
	//if( pclMsg->siToItemPos2 < 0 || pclMsg->siToItemPos2 >= MAX_ITEM_PER_PERSON )		return;

	pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

	if( pclMsg->siKeyItemPos >= PERSONITEM_INV0 && pclMsg->siKeyItemPos < MAX_ITEM_PER_PERSON )
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siKeyItemPos].Set(&pclMsg->clKeyItem);
		((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclMsg->siKeyItemPos , 0 );
	}

	pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToItemPos1].Set(&pclMsg->clToItem1);
	((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclMsg->siToItemPos1 , 0 );

	if( pclMsg->siToItemPos2 >= PERSONITEM_INV0 && pclMsg->siToItemPos2 < MAX_ITEM_PER_PERSON && pclMsg->clToItem2.siUnique > 0)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToItemPos2].Set(&pclMsg->clToItem2);
		((cltCharServer*)pclCM->CR[id])->SetInputItemReserve( pclMsg->siToItemPos2 , 0 );
	}

	// 클라이언트로 업데이트 된 정보를 보낸다. 
	cltGameMsgResponse_OpenTreasureBox clinfo( pclMsg->siItemPos, &pclMsg->clItem, pclMsg->siKeyItemPos, &pclMsg->clKeyItem, pclMsg->siToItemPos1, &(pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToItemPos1]), pclMsg->siGetItemNumber1 , pclMsg->siToItemPos2, &(pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToItemPos2]), pclMsg->siGetItemNumber2 );
	cltMsg clMsg(GAMEMSG_RESPONSE_OPENTREASUREBOX, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	// [메시지 구조를 바꾸기 전까지 하드코딩 ㅠ_ㅠ]
	// 대박 아이템 알림창
	if ( (true == pclMsg->bCanNotice1) || (true == pclMsg->bCanNotice2) )
	{
		NTCHARString32 ntstrName( pclCM->CR[id]->GetName() );

		if ( true == pclMsg->bCanNotice1 )
		{
			cltGameMsgResponse_RareItemFromTreasureBox clInfo( ntstrName, &pclMsg->clItem, &pclMsg->clToItem1 );
			cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
		}
		else if ( true == pclMsg->bCanNotice2 )
		{
			cltGameMsgResponse_RareItemFromTreasureBox clInfo( ntstrName, &pclMsg->clItem, &pclMsg->clToItem2 );
			cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
		}
	}
	else
	{
		bool bSendRareItemDialog = false;

		switch ( pclMsg->clItem.siUnique ) 
		{
			// [지연] : 2010신년이벤트 - 포춘쿠키를 썼을떄
		case ITEMUNIQUE( 13063 ):
			{
				if ( ITEMUNIQUE( 13658 ) == pclMsg->clToItem1.siUnique )	// 팅키인형[이벤트]
				{
					bSendRareItemDialog = true;
				}
			}
			break;
			// 신년 복주머니 (대만전용)
		case ITEMUNIQUE(13324):
			{
				if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(24102) )	// 순간이동이용권(영구)
				{
					bSendRareItemDialog = true;
				}
			}
			break;
			// 초콜릿 상자
		case ITEMUNIQUE(8126):
			{
				if ( pclClient->IsWhereServiceArea( ConstServiceArea_Japan ) == true )
				{
					if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(2756)	)	// 네잎 클로버2
					{
						bSendRareItemDialog = true;
					}
				}
				else //if ( pclClient->IsWhereServiceArea( ConstServiceArea_English ) == true )
				{
					if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(5370)		// 축복의 돌
						|| pclMsg->clToItem1.siUnique == ITEMUNIQUE(2756)	// 네잎 클로버2
						)
					{
						bSendRareItemDialog = true;
					}
				}
			}
			break;
			// 랜덤 캔디 박스
		case ITEMUNIQUE(8128):
			{
				if ( pclClient->IsWhereServiceArea( ConstServiceArea_Japan ) == true )
				{
					if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(2756) )	// 행운의 네잎클로버
					{
						bSendRareItemDialog = true;
					}
				}
				else if ( pclClient->IsWhereServiceArea( ConstServiceArea_Taiwan ) == true )
				{
					if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(5370)		// 축복의 돌
						|| pclMsg->clToItem1.siUnique == ITEMUNIQUE(2756)	// 네잎 클로버2
						)
					{
						bSendRareItemDialog = true;
					}
				}
				else //if ( pclClient->IsWhereServiceArea( ConstServiceArea_English | ConstServiceArea_USA ) == true )
				{
					if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(5370)		// 축복의 돌
						|| pclMsg->clToItem1.siUnique == ITEMUNIQUE(2756)	// 네잎 클로버2
						)
					{
						bSendRareItemDialog = true;
					}
				}
			}
			break;

		case ITEMUNIQUE(13325):		// 대박 상자
			{
				if( pclMsg->clToItem1.siUnique == ITEMUNIQUE(15900) || 
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(24098) || 
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(2760) || 
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(5710) || 
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(5715) || 
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(1803) || 
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(5412) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(1801)
					)
				{
					bSendRareItemDialog = true;
				}
			}
			break;

		case ITEMUNIQUE(13331):
			{
				if (pclMsg->clToItem2.siUnique == ITEMUNIQUE(12506))
					bSendRareItemDialog = true;
			}
			break;
			// 낡은상자에서 잠자는 냐옹이 상자 나오면 공지 (KOREA)
		case ITEMUNIQUE(13326):
			{
				if(pclClient->IsWhereServiceArea( ConstServiceArea_Korea))
				{
					if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(13350)) // 잠자는 냐옹이 상자
						bSendRareItemDialog = true;
				}
			}
			break;
		case ITEMUNIQUE(13333): // 라쿤 가죽 지갑.
			{
				if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(12531) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(5748) )
					bSendRareItemDialog = true;
			}
			break;
		case ITEMUNIQUE(13335):
			{
				if ( pclMsg->clToItem1.siUnique == ITEMUNIQUE(5695)  ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(13305) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(13306) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(5690)  ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(13317) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(5685)  ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(13320) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(13330) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(15916) ||
					pclMsg->clToItem1.siUnique == ITEMUNIQUE(12506)	 )
					bSendRareItemDialog = true;
			}
			break;
		case ITEMUNIQUE(13359):	// 3차 복주머니
			{
				if ( pclMsg->clToItem2.siUnique == ITEMUNIQUE(15918) )	// 라쿤가방
					bSendRareItemDialog = true;
			}
			break;
		}

		if ( bSendRareItemDialog == true )
		{

			NTCHARString32 ntstrName( pclCM->CR[id]->GetName() );
			if ( pclMsg->clToItem2.siUnique == ITEMUNIQUE(12506)  || pclMsg->clToItem2.siUnique == ITEMUNIQUE(15918))
			{
				cltGameMsgResponse_RareItemFromTreasureBox clInfo( ntstrName, &pclMsg->clItem, &pclMsg->clToItem2 );
				cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX, sizeof(clInfo), (BYTE*)&clInfo);
				pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
			}
			else
			{
				cltGameMsgResponse_RareItemFromTreasureBox clInfo( ntstrName, &pclMsg->clItem, &pclMsg->clToItem1 );
				cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX, sizeof(clInfo), (BYTE*)&clInfo);
				pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
			}

		}
	}

	// 아이템 사용 통계를 업데이트 한다. 
	pclItemManager->AddUseNum(pclMsg->clItem.siUnique, 1);

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", pclMsg->clItem.siUnique, "UseNum", 1);


/*	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 11] // 보물상자 열기.(수정필요)
		// param1		== 아이템1번 위치, 
		// param1		== 아이템1번 유니크, 
		// param3		== 아이템1번 개수, 
		// pszParam1	== 아이템2번 위치, 유니크, 개수. => <뭔지 모르겠다!>
		TCHAR str[50];
		StringCchPrintf(str, sizeof(str), "pos: %d Unique: %d Num: %d",
										   pclMsg->siToItemPos2,
										   pclMsg->clToItem2.siUnique, 
										   pclMsg->siGetItemNumber2);
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_TREASUREBOX_OPEN_USED, 
							 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
							 pclMsg->siToItemPos1, pclMsg->clToItem1.siUnique, pclMsg->siGetItemNumber1, 0, 0, 
							 str, NULL);
	}*/

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// 상자 사용로그
		// param1 = 인벤토리 위치
		// param2 = 키 유니크
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_TREASUREBOX_OPEN_USED, 
							0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clUse_Item, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
							pclMsg->siItemPos, pclMsg->clUse_KeyItem.siUnique, 0, 0, 0, 
							NULL, NULL);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		if( pclMsg->clUse_KeyItem.siUnique > 0 )
		{
			// 키 사용로그
			// param1 = 인벤토리 위치
			// praam2 = 상자 유니크
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
								USEITEM_REASON_OPENBOX_KEY, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clUse_KeyItem, pclMsg->clKeyItem.siItemNum, 0, 0, 0, 0, 0, 
								pclMsg->siKeyItemPos, pclMsg->clUse_Item.siUnique, pclMsg->clAdd_ToItem1.siUnique, pclMsg->clAdd_ToItem2.siUnique, 0, 
								NULL, NULL);
		}
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		if( pclMsg->clAdd_ToItem1.siUnique > 0 )
		{
			// 받은 아이템 로그 - 첫번째
			// param1 = 인벤토리 위치
			// param2 = 박스 유니크
			// param3 = 키 유니크
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_GET_ITEM, 
								INPUTITEMPERSON_OPTION_OPENBOX, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clAdd_ToItem1, pclMsg->clToItem1.siItemNum, 0, 0, 0, 0, 0, 
								pclMsg->siToItemPos1, pclMsg->clUse_Item.siUnique, pclMsg->clUse_KeyItem.siUnique, 0, 0, 
								NULL, NULL);
		}
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		if( pclMsg->clAdd_ToItem2.siUnique > 0 )
		{
			// 받은 아이템 로그 - 두번째
			// param1 = 인벤토리 위치
			// param2 = 박스 유니크
			// param3 = 키 유니크
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_GET_ITEM, 
								INPUTITEMPERSON_OPTION_OPENBOX, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clAdd_ToItem2, pclMsg->clToItem2.siItemNum, 0, 0, 0, 0, 0, 
								pclMsg->siToItemPos2, pclMsg->clUse_Item.siUnique, pclMsg->clUse_KeyItem.siUnique, 0, 0, 
								NULL, NULL);
		}
	}
}

//[추가 : 황진성 2008. 1. 30 => 공간 상자(보물)에 저장. DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM(sPacketHeader* pPacket)
{
	sDBResponse_Move_Item* pclMsg = (sDBResponse_Move_Item*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if( false == pclCM->IsValidID( id ) )
		return;

	if( pclMsg->siInvenItemPos < 0 || pclMsg->siInvenItemPos >= MAX_ITEM_PER_PERSON )	return;
	if( pclMsg->siBoxItemPos < 0 || pclMsg->siBoxItemPos >= MAX_ITEM_TREASURE_BOX )		return;

	cltPersonInfo* pPersonInfo = pclCM->CR[id]->pclCI;

	cltItem* InvenItem = &pPersonInfo->clCharItem.clItem[pclMsg->siInvenItemPos];	

	cltItem* BoxItem = NULL;
	if(pclMsg->type)
	{
		if(pclCM->CR[id]->pclTreasureBoxItem)	BoxItem = &pclCM->CR[id]->pclTreasureBoxItem->clItem[pclMsg->siBoxItemPos];
	}
	else
	{
		if(pclCM->CR[id]->pclSpaceBoxItem)		BoxItem = &pclCM->CR[id]->pclSpaceBoxItem->clItem[pclMsg->siBoxItemPos];
	}
	if(BoxItem == NULL)			return;
	
	InvenItem->Init();
	InvenItem->Set(&pclMsg->clInvenItem);

	BoxItem->Init();
	BoxItem->Set(&pclMsg->clBoxItem);

	// 공간 상자로 회수
	if( TYPE_SPACE_BOX == pclMsg->type ) 
	{
		cltGameMsgResponse_Keep_Box_Move_Item clInfo( TYPE_SPACE_BOX, pclMsg->siInvenItemPos, &pclMsg->clInvenItem, pclMsg->siBoxItemPos, &pclMsg->clBoxItem);
		cltMsg clMsg(GAMEMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);	
	}
	// 보물 상자로 회수
	else if( TYPE_TREASURE_BOX == pclMsg->type )
	{
		cltGameMsgResponse_Keep_Box_Move_Item clInfo( TYPE_TREASURE_BOX, pclMsg->siInvenItemPos, &pclMsg->clInvenItem, pclMsg->siBoxItemPos, &pclMsg->clBoxItem);
		cltMsg clMsg(GAMEMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2008-4-18 ] // 공간 상자에 넣는 아이템 로그.
		// param1 == 인벤토리의 아이템 위치.
		// param2 == 공간상자의 아이템 위치.
		// param3 == 공간상자에 남은 개수.
		// param4 == 공간상자 타입
		cltServer* pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SPACEBOX, LOGCOMMAND_INDEX_TO_SPACEBOX, 
			pclMsg->type, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clMoveItem, pclMsg->clInvenItem.siItemNum, 0, 0, 0, 0, 0, 
			pclMsg->siInvenItemPos, pclMsg->siBoxItemPos, pclMsg->clBoxItem.siItemNum, pclMsg->type, 0, NULL, NULL);
	}
}

//[추가 : 황진성 2008. 1. 30 => 공간 상자(보물)에 회수.DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM(sPacketHeader* pPacket)
{
	sDBResponse_Move_Item* pclMsg = (sDBResponse_Move_Item*)pPacket;
	
	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if( false == pclCM->IsValidID(id) )
	{
		return;
	}
		
	if( pclMsg->siInvenItemPos < 0 || pclMsg->siInvenItemPos >= MAX_ITEM_PER_PERSON )	return;
	if( pclMsg->siBoxItemPos < 0 || pclMsg->siBoxItemPos >= MAX_ITEM_TREASURE_BOX )		return;
	
	cltPersonInfo* pPersonInfo = pclCM->CR[id]->pclCI;

	cltItem* InvenItem = &pPersonInfo->clCharItem.clItem[pclMsg->siInvenItemPos];	

	cltItem* BoxItem = NULL;
	if(pclMsg->type)
	{
		if(pclCM->CR[id]->pclTreasureBoxItem)	BoxItem = &pclCM->CR[id]->pclTreasureBoxItem->clItem[pclMsg->siBoxItemPos];
	}
	else
	{
		if(pclCM->CR[id]->pclSpaceBoxItem)		BoxItem = &pclCM->CR[id]->pclSpaceBoxItem->clItem[pclMsg->siBoxItemPos];
	}
	if(BoxItem == NULL)				return;

	BoxItem->Init();
	BoxItem->Set(&pclMsg->clBoxItem);

	InvenItem->Init();
	InvenItem->Set(&pclMsg->clInvenItem);

	// 공간 상자로 회수
	if( TYPE_SPACE_BOX == pclMsg->type ) 
	{
		cltGameMsgResponse_Recovery_Box_Move_Item clInfo( TYPE_SPACE_BOX, pclMsg->siInvenItemPos, &pclMsg->clInvenItem, pclMsg->siBoxItemPos, &pclMsg->clBoxItem);
		cltMsg clMsg(GAMEMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// [기형] 휴면 계정 시스템 - 공간상자에서 아이템을 찾을때 휴면 장비 아이템인지 검사 하고 맞다면 업그래이드 해준다.
		if( NULL != pclClient->pclDormancySystem)
		{
			CDormancyEquipItemRewardServer *pclDormancyServer = (CDormancyEquipItemRewardServer *)pclClient->pclDormancySystem->GetDormancyEquipItemReward();
			if( NULL != pclDormancyServer)
			{
				if( TRUE == pclDormancyServer->IsDormancyEquipItem(InvenItem->siUnique))
				{
					pclDormancyServer->UpgradeItem(pclCM->CR[id], InvenItem->siUnique, pclMsg->siInvenItemPos, InvenItem->GetType(pclClient->pclItemManager));
				
				}
			}
		}

	}
	// 보물 상자로 회수
	else if( TYPE_TREASURE_BOX == pclMsg->type )
	{
		cltGameMsgResponse_Recovery_Box_Move_Item clInfo( TYPE_TREASURE_BOX, pclMsg->siInvenItemPos, &pclMsg->clInvenItem, pclMsg->siBoxItemPos, &pclMsg->clBoxItem);
		cltMsg clMsg(GAMEMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	if(pclMsg->siResult == 1)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2008-4-18 ] // 보물 상자에 있는 아이템을 인벤토리로.
			// param1 == 인벤의 위치.
			// param2 == 공간상자의 위치.
			// param3 == 공간상자에 남은 개수.
			// param4 == 공간상자 타입
			cltServer* pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SPACEBOX, LOGCOMMAND_INDEX_SPACEBOX_TO_INVENTORY, 
				pclMsg->type, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clMoveItem, pclMsg->clInvenItem.siItemNum, 0, 0, 0, 0, 0, 
				pclMsg->siInvenItemPos, pclMsg->siBoxItemPos, pclMsg->clBoxItem.siItemNum, pclMsg->type, 0, NULL, NULL);
		}
	}
}

//[추가 : 황진성 2008. 1. 30 => 공간 상자에 아이템 리스트. DB->S]
void  cltServer::DoMsg_DBMSG_RESPONSE_SPACE_BOX_ITEM_LIST(sPacketHeader* pPacket)
{
	sDBResponse_Space_Box_List* pclMsg = (sDBResponse_Space_Box_List*)pPacket;

	SI32 id = pclMsg->usCharID; 

	if( false == pclCM->IsValidID(id) )
	{
		return;
	}

	if(pclCM->CR[id]->pclSpaceBoxItem == NULL)			return;
	if(pclCM->CR[id]->pclTreasureBoxItem == NULL)		return;

	pclCM->CR[id]->pclSpaceBoxItem->Set(&pclMsg->clSpaceBoxItem);
	pclCM->CR[id]->pclTreasureBoxItem->Set(&pclMsg->clTreasureBoxItem);

	// 상자 안의 아이템 정보가 변경되면 다시 리스트를 요청하기 위해 사용.
	bool RecallMsg = false;
	
	//[진성] 공간상자에 기간이 지난 아이템 삭제하기.	
	cltSpaceBox*	pclSpaceBoxitem = pclCM->CR[id]->pclSpaceBoxItem;
	for (SI32 i = 0 ; i < MAX_ITEM_SPACE_BOX ; i ++ )
	{
		SI32 itemunique = pclSpaceBoxitem->clItem[i].siUnique ;
		if ( itemunique > 0 )
		{
			if ( pclSpaceBoxitem->clItem[i].IsHaveUseDay() )  //[영진] 남은 사용기간 3일미만 알림
			{
				SI32 useableDay = 0;
				BOOL rVal = GetItemUseAbleDay(&pclSpaceBoxitem->clItem[i],&useableDay);

				if(pclSpaceBoxitem->clItem[i].IsSealingUp(pclItemManager) == true)
					continue;

				if(rVal == FALSE) continue;

				if(useableDay >= 3) continue; // 사용가능기간이 3일 이상 넉넉하면 패스 
				else if(useableDay >= 0)		 //  3일 미만이지만 사용가능 기간이 남아있다면 클라이언트로 경고
				{
					cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclSpaceBoxitem->clItem[i].siUnique, useableDay);
					cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
				}
				else if( useableDay < 0) //사용기간이 지나서 지워준다 그리고 클라이언트에 알림.
				{
					//KHY - 0723 - 기간아이템 파괴 방지.
					if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
					{

					}
					else
					{
						// 고급품은 지워지지 않는다. - by LEEKH
						if( pclSpaceBoxitem->clItem[i].clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
						{
							sDBRequest_Use_SpaceBox_Item useitem(id,pclCM->CR[id]->pclCI->GetPersonID(), i, &pclSpaceBoxitem->clItem[i], USEITEM_REASON_DATEOVER);
							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

							cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclSpaceBoxitem->clItem[i].siUnique, useableDay);
							cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
							pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );			

							RecallMsg = true;
						}
					}
				}
			}
		}
	}

	//[진성] 보물상자에 기간이 지난 아이템 삭제하기.	
	cltTreasureBox* pclTreasureBox  = pclCM->CR[id]->pclTreasureBoxItem;
	for (SI32 i = 0 ; i < MAX_ITEM_TREASURE_BOX ; i ++ )
	{
		SI32 itemunique = pclTreasureBox->clItem[i].siUnique ;
		if ( itemunique > 0 )
		{
			if ( pclTreasureBox->clItem[i].IsHaveUseDay() )  //[영진] 남은 사용기간 3일미만 알림
			{
				SI32 useableDay = 0;
				BOOL rVal = GetItemUseAbleDay(&pclTreasureBox->clItem[i],&useableDay);

				if(pclSpaceBoxitem->clItem[i].IsSealingUp(pclItemManager) == true)
					continue;

				if(rVal == FALSE) continue;

				if(useableDay >= 3) continue; // 사용가능기간이 3일 이상 넉넉하면 패스 
				else if(useableDay >= 0)		 //  3일 미만이지만 사용가능 기간이 남아있다면 클라이언트로 경고
				{
					cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclTreasureBox->clItem[i].siUnique, useableDay);
					cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
				}
				else if( useableDay < 0) //사용기간이 지나서 지워준다 그리고 클라이언트에 알림.
				{
					//KHY - 0723 - 기간아이템 파괴 방지.
					if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
					{

					}
					else
					{
						// 고급품은 지워지지 않는다. - by LEEKH
						if( pclTreasureBox->clItem[i].clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
						{
							sDBRequest_Use_Treasure_Item useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), i, &pclTreasureBox->clItem[i], USEITEM_REASON_DATEOVER);
							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

							cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclTreasureBox->clItem[i].siUnique, useableDay);
							cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
							pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );				

							RecallMsg = true;
						}
					}
				}
			}
		}
	}

	SI32 InvenCount		= 0;
	UI32 GetItemReason	= 0;

	// 공간상자에 들어있는 파티홀 아이템도 제거	
	pclSpaceBoxitem = pclCM->CR[id]->pclSpaceBoxItem;
	if(pclSpaceBoxitem)
	{
		for(InvenCount = 0; InvenCount < MAX_ITEM_SPACE_BOX; ++InvenCount)	
		{
			GetItemReason = pclSpaceBoxitem->clItem[InvenCount].clItemCommon.clCommonInfo.uiGetItemReason;
			if( cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM == GetItemReason )
			{
				// 아이템 제거.  
				sDBRequest_Use_SpaceBox_Item useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), InvenCount, &pclSpaceBoxitem->clItem[InvenCount], USEITEM_REASON_DATEOVER);
				pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

				RecallMsg = true;
			}
		}
	}

	pclTreasureBox  = pclCM->CR[id]->pclTreasureBoxItem;
	if(pclTreasureBox)
	{
		for(InvenCount = 0; InvenCount < MAX_ITEM_TREASURE_BOX; ++InvenCount)	
		{
			GetItemReason = pclTreasureBox->clItem[InvenCount].clItemCommon.clCommonInfo.uiGetItemReason;
			if( cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM == GetItemReason )
			{
				// 아이템 제거.  
				sDBRequest_Use_Treasure_Item useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), InvenCount, &pclTreasureBox->clItem[InvenCount], USEITEM_REASON_DATEOVER);
				pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

				RecallMsg = true;
			}
		}
	}

	// 공간상자에 들어있는 사용되지않는 네잎클로버 아이템도 제거	
	pclSpaceBoxitem = pclCM->CR[id]->pclSpaceBoxItem;
	if(pclSpaceBoxitem)
	{
		for(InvenCount = 0; InvenCount < MAX_ITEM_SPACE_BOX; ++InvenCount)	
		{
			if( ITEMUNIQUE(2755) == pclSpaceBoxitem->clItem[InvenCount].siUnique || 
				ITEMUNIQUE(2754) == pclSpaceBoxitem->clItem[InvenCount].siUnique )	
			{
				if( pclSpaceBoxitem->clItem[InvenCount].siUnique != siFourLeafUnique)
				{
					// 아이템 제거.  
					sDBRequest_Use_SpaceBox_Item useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), InvenCount, &pclSpaceBoxitem->clItem[InvenCount], USEITEM_REASON_DATEOVER);
					pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

					RecallMsg = true;
				}
			}
		}
	}

	pclTreasureBox  = pclCM->CR[id]->pclTreasureBoxItem;
	if(pclTreasureBox)
	{
		for(InvenCount = 0; InvenCount < MAX_ITEM_TREASURE_BOX; ++InvenCount)	
		{
			if( ITEMUNIQUE(2755) == pclTreasureBox->clItem[InvenCount].siUnique || 
				ITEMUNIQUE(2754) == pclTreasureBox->clItem[InvenCount].siUnique	)	
			{
				if( pclTreasureBox->clItem[InvenCount].siUnique != siFourLeafUnique)
				{
					// 아이템 제거.  
					sDBRequest_Use_Treasure_Item useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), InvenCount, &pclTreasureBox->clItem[InvenCount], USEITEM_REASON_DATEOVER);
					pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

					RecallMsg = true;
				}
			}
		}
	}

	if(false == RecallMsg)
	{
		cltGameMsgResponse_Space_Box_List clInfo( pclCM->CR[id]->pclSpaceBoxItem, pclCM->CR[id]->pclTreasureBoxItem );
		cltMsg clMsg(GAMEMSG_RESPONSE_SPACE_BOX_LIST, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);		
	}
	else
	{
		sDBRequest_Space_Box_List clSpace_Box_List( id, pclCM->CR[id]->pclCI->GetPersonID() );
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clSpace_Box_List);
	}
	
}


// 아이템을 사용하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_USE_SPACEBOX_ITEM(sPacketHeader* pPacket)
{
	sDBResponse_Use_SpaceBox_Item* pclMsg = (sDBResponse_Use_SpaceBox_Item*)pPacket;

	if(0 > pclMsg->siResult)
		return;
	
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)
		return;

	if (pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_SPACE_BOX)		
		return;

	if(pclCM->CR[id]->pclSpaceBoxItem == NULL)			return;

	//변경된 공간상자의 아이템을 세팅한다.
	pclCM->CR[id]->pclSpaceBoxItem->clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

	//변경된 공간상자의 아이템 리스트를 클라이언트에게 보내준다.
	cltGameMsgResponse_Use_SpaceBox_Item clInfo( pclMsg->siItemPos, &pclMsg->clItem );
	cltMsg clMsg(GAMEMSG_RESPONSE_USE_SPACEBOX_ITEM, sizeof(clInfo), (BYTE*)&clInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);		
}

// 아이템을 사용하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_USE_TREASURE_ITEM(sPacketHeader* pPacket)
{
	sDBResponse_Use_Treasure_Item* pclMsg = (sDBResponse_Use_Treasure_Item*)pPacket;

	if(0 > pclMsg->siResult)
		return;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)
		return;

	if (pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_TREASURE_BOX )		
		return;

	if(pclCM->CR[id]->pclTreasureBoxItem == NULL)				return;

	pclCM->CR[id]->pclTreasureBoxItem->clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

	cltGameMsgResponse_Use_Treasure_Item clInfo( pclMsg->siItemPos, &pclMsg->clItem );
	cltMsg clMsg(GAMEMSG_RESPONSE_USE_TREASURE_ITEM, sizeof(clInfo), (BYTE*)&clInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);		
}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONPUSHITEM(sPacketHeader* pPacket)
{
	sDBResponse_SummonPushItem* pclMsg = (sDBResponse_SummonPushItem*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	
	if ( 0 >= pclMsg->m_siResult )
	{
		return;
	}

	SI32 siID = pclMsg->m_usCharID;
	if ( FALSE == pclCM->IsValidID(siID) )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
	if ( NULL == pclchar )
	{
		return;
	}
	
	// 혹시 이동하려는 사람이 다른경우가 있을 수 있으니 
	if ( pclchar->pclCI->GetPersonID() != pclMsg->m_siPersonID )
	{
		return;
	}

/*	SI16 siSummonIndex = pclMsg->m_siSummonIndex;
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}*/

	//cltSummon* pclsummon = (cltSummon*)pclchar->pclCI->clSummonInfo.GetSummon( siSummonIndex );
	cltSummon* pclsummon = (cltSummon*)pclchar->pclCI->clSummonInfo.GetSummonFromSummonID( pclMsg->m_siSummonID );
	if ( (NULL == pclsummon) || (0 >= pclsummon->siKind) || (MAX_KIND_NUMBER <= pclsummon->siKind) )
	{
		return;
	}

	SI32 siSummonID = pclchar->GetSummonID();
	if ( false == pclCM->IsValidID( siSummonID ) )
	{
		return;
	}

	SI08 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID( pclMsg->m_siSummonID );
	if(siSummonIndex < 0 || siSummonIndex >= MAX_SUMMON_OWN)		return;

	cltCharServer* pclSummonChar = (cltCharServer*)pclCM->CR[siSummonID];
	if ( NULL == pclSummonChar )
	{
		return;
	}

	// 사람의 인벤
	SI16 siFrom = pclMsg->m_siFromItemPos;
	// 소환수 인벤
	SI16 siTo	= pclMsg->m_siToItemPos;

	// 사람인벤은 장비 장착창에서 이동은 안되니까
	if ( (MAX_SHAPE_ITEM >= siFrom) || (MAX_ITEM_PER_PERSON <= siFrom) )
	{
		return;
	}

	// 소환수 장착창
	if ( (0 > siTo) || (MAX_SHAPE_ITEM <= siTo) )
	{
		return;
	}

	//---------------------------------------------
	// 서버를 업데이트 한다. 
	//---------------------------------------------

	// 캐릭터 장비를 업데이트 한다
	pclchar->pclCI->clCharItem.clItem[ siFrom ].Set( &pclMsg->m_clFromItem );
	// 소환수 장비를 업데이트 한다
	pclchar->pclCI->clSummonInfo.SetSummonEquipItem( pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->m_siSummonID),
													 siTo, &pclMsg->m_clToItem );

	//---------------------------------------------
	// 소환수 능력치 계산을 다시 하도록 한다
	//---------------------------------------------
	pclSummonChar->SetParametaBoxAction( true );

	//----------------------------------------------
	// 클라이언트로 업데이트 된 정보를 보낸다. 
	//----------------------------------------------
	cltGameMsgResponse_PersonItemInfo clPersonItemInfo( siFrom, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->m_clFromItem, 0 );
	cltMsg clPersonMsg( GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clPersonItemInfo), (BYTE*)&clPersonItemInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clPersonMsg );

	cltGameMsgResponse_SummonChangeItem clSummonChangItem( siSummonIndex, siTo, &pclMsg->m_clToItem );
	cltMsg clSummonMsg( GAMEMSG_RESPONSE_SUMMONCHANGEITEM, sizeof(clSummonChangItem), (BYTE*)&clSummonChangItem );
	pclchar->SendNetMsg( (sPacketHeader*)&clSummonMsg );

}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONPOPITEM(sPacketHeader* pPacket)
{
	sDBResponse_SummonPopItem* pclMsg = (sDBResponse_SummonPopItem*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 0 >= pclMsg->m_siResult )
	{
		return;
	}

	SI32 siID = pclMsg->m_usCharID;
	if ( FALSE == pclCM->IsValidID(siID) )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
	if ( NULL == pclchar )
	{
		return;
	}

	// 혹시 이동하려는 사람이 다른경우가 있을 수 있으니 
	if ( pclchar->pclCI->GetPersonID() != pclMsg->m_siPersonID )
	{
		return;
	}

	SI16 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->m_siSummonID);//pclMsg->m_siSummonIndex;
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	cltSummon* pclsummon = (cltSummon*)pclchar->pclCI->clSummonInfo.GetSummon( siSummonIndex );
	if ( (NULL == pclsummon) || (0 >= pclsummon->siKind) || (MAX_KIND_NUMBER <= pclsummon->siKind) )
	{
		return;
	}

	SI32 siSummonID = pclchar->GetSummonID();
	if ( false == pclCM->IsValidID( siSummonID ) )
	{
		return;
	}

	cltCharServer* pclSummonChar = (cltCharServer*)pclCM->CR[siSummonID];
	if ( NULL == pclSummonChar )
	{
		return;
	}

	// 소환수 인벤
	SI16 siFrom = pclMsg->m_siFromItemPos;
	// 사람 인벤
	SI16 siTo	= pclMsg->m_siToItemPos;

	// 소환수
	if ( (0 > siFrom) || (MAX_SHAPE_ITEM <= siFrom) )
	{
		return;
	}

	// 사람
	if ( (MAX_SHAPE_ITEM >= siTo) || (MAX_ITEM_PER_PERSON <= siTo) )
	{
		return;
	}

	//---------------------------------------------
	// 서버를 업데이트 한다. 
	//---------------------------------------------
	// 소환수 장비를 업데이트 한다
	pclchar->pclCI->clSummonInfo.SetSummonEquipItem( siSummonIndex, siFrom, &pclMsg->m_clFromItem );
	// 캐릭터 장비를 업데이트 한다
	pclchar->pclCI->clCharItem.clItem[ siTo ].Set( &pclMsg->m_clToItem );
	
	//---------------------------------------------
	// 소환수 능력치 계산을 다시 하도록 한다
	//---------------------------------------------
	pclSummonChar->SetParametaBoxAction( true );

	//----------------------------------------------
	// 클라이언트로 업데이트 된 정보를 보낸다. 
	//----------------------------------------------
	cltGameMsgResponse_SummonChangeItem clSummonChangItem( siSummonIndex, siFrom, &pclMsg->m_clFromItem );
	cltMsg clSummonMsg( GAMEMSG_RESPONSE_SUMMONCHANGEITEM, sizeof(clSummonChangItem), (BYTE*)&clSummonChangItem );
	pclchar->SendNetMsg( (sPacketHeader*)&clSummonMsg );

	cltGameMsgResponse_PersonItemInfo clPersonItemInfo( siTo, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->m_clToItem, 0 );
	cltMsg clPersonMsg( GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clPersonItemInfo), (BYTE*)&clPersonItemInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clPersonMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONITEM_STATUSCHANGE(sPacketHeader* pPacket)
{
	sDBResponse_SummonItem_StatusChange* pclMsg = (sDBResponse_SummonItem_StatusChange*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	//---------------------------------------------------------
	// 기본적인 체크
	//---------------------------------------------------------

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32 siID = pclMsg->m_usCharID;
	if ( FALSE == pclCM->IsValidID(siID) )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
	if ( NULL == pclchar )
	{
		return;
	}

	// 혹시 다른 케릭터일수 잇으므로
	if ( pclchar->pclCI->GetPersonID() != pclMsg->m_siPersonID )
	{
		return;
	}

	SI16 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->m_siSummonID);//pclMsg->m_siSummonIndex;
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	SI32 siSummonID = pclchar->GetSummonID();
	if ( false == pclCM->IsValidID( siSummonID ) )
	{
		return;
	}

	cltCharServer* pclSummonChar = (cltCharServer*)pclCM->CR[siSummonID];
	if ( NULL == pclSummonChar )
	{
		return;
	}
	//---------------------------------------------------------
	// 여기까지
	//---------------------------------------------------------

	// 소환수 인벤
	SI16 siPosition = pclMsg->m_siItemPos;

	// 소환수 인벤 체크
	if ( (0 > siPosition) || (MAX_SHAPE_ITEM <= siPosition) )
	{
		return;
	}

	//---------------------------------------------
	// 서버를 업데이트 한다. 
	//---------------------------------------------
	// 소환수 장비를 업데이트 한다
	pclchar->pclCI->clSummonInfo.SetSummonEquipItem( siSummonIndex, siPosition, &pclMsg->m_clItem );

	//---------------------------------------------
	// 소환수 능력치 계산을 다시 하도록 한다
	//---------------------------------------------
	pclSummonChar->SetParametaBoxAction( true );

	//----------------------------------------------
	// 클라이언트로 업데이트 된 정보를 보낸다. 
	//----------------------------------------------
	cltGameMsgResponse_SummonChangeItem clSummonChangItem( siSummonIndex, siPosition, &pclMsg->m_clItem );
	cltMsg clSummonMsg( GAMEMSG_RESPONSE_SUMMONCHANGEITEM, sizeof(clSummonChangItem), (BYTE*)&clSummonChangItem );
	pclchar->SendNetMsg( (sPacketHeader*)&clSummonMsg );

}
void cltServer::DoMsg_DBMSG_RESPONSE_REPAIRITEM(sPacketHeader* pPacket)
{
	sDBResponse_RepairItem* pclMsg = (sDBResponse_RepairItem*)pPacket;

	SI32 id			= pclMsg->m_usCharID;
	SI32 itempos	= pclMsg->m_siPos;
	if( itempos < 0 || itempos >= MAX_ITEM_PER_PERSON )						return;

	SI32 siItemCharID = pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer* pclitemchar = pclCM->GetCharServer( siItemCharID );

	if( pclMsg->m_siResult == 1 )
	{
		if( pclitemchar )
		{
			pclitemchar->pclCI->clCharItem.clItem[itempos].Set( &pclMsg->m_clItem );

			cltGameMsgResponse_PersonItemInfo clinfo(itempos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclitemchar->pclCI->clCharItem.clItem[itempos], 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclitemchar->SendNetMsg((sPacketHeader*)&clMsg);

			if ( 0 < pclMsg->m_siRepairMoney )
			{
				pclitemchar->pclCI->clIP.SetMoney( pclMsg->m_siRepairerMoney );
				pclitemchar->SetUpdateSwitch(UPDATE_IP, true, 0);
			}

			pclitemchar->SetRepairShopData( false, NULL );

			cltGameMsgResponse_NewItemRepair clResult1( 1, pclitemchar->GetCharUnique(), &pclMsg->m_clItem, pclMsg->m_siType );
			cltMsg clNetMsg1( GAMEMSG_RESPONSE_NEWITEMREPAIR, sizeof(clResult1), (BYTE*)&clResult1 );
			pclitemchar->SendNetMsg( (sPacketHeader*)&clNetMsg1 );

			// 주변 캐릭터에게 수리요청 배너를 지우도록 한다.
			cltGameMsgResponse_NewItemRepair clResult2( 1, pclitemchar->GetCharUnique(), &pclMsg->m_clItem, 4 );
			cltMsg clNetMsg2( GAMEMSG_RESPONSE_NEWITEMREPAIR, sizeof(clResult2), (BYTE*)&clResult2 );
			pclitemchar->SendNeighbourMsg( &clNetMsg2, false );
		}
	}

	if( pclMsg->m_siResult == 1 )
	{
		SI32 siRepairCharID = pclCM->GetIDFromPersonID( pclMsg->m_siRepairPersonID );
		if( pclCM->IsValidID( siRepairCharID ) )
		{
			cltCharServer* pclrepairchar = pclCM->GetCharServer( siRepairCharID );
			if( pclrepairchar )
			{
				if ( 0 < pclMsg->m_siRepairMoney )
				{
					pclrepairchar->pclCI->clIP.SetMoney( pclMsg->m_siRepairPersonMoney );
					pclrepairchar->SetUpdateSwitch(UPDATE_IP, true, 0);
				}

				cltGameMsgResponse_NewItemRepair clResult( pclMsg->m_siResult, pclitemchar->GetCharUnique(), &pclMsg->m_clItem, pclMsg->m_siType );
				cltMsg clMsg(GAMEMSG_RESPONSE_NEWITEMREPAIR, sizeof(clResult), (BYTE*)&clResult);
				pclrepairchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_GIVEVILLAGEITEM(sPacketHeader* pPacket)
{
	// PCK : 로그를 남기기 위해서 작성.
	sDBResponse_GiveVillageItem* pclMsg = (sDBResponse_GiveVillageItem*)pPacket;

	// PCK : PersonID를 근거로 CharID를 찾음.(PersonID넣지말자!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	// param1 == 사용기간
	// charparam1 == 지급할때 남기는 메시지
	cltServer * pclServer = (cltServer*)pclClient;
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_VILLAGEGIVEITEM, 
		0, pclchar, NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		pclMsg->siUseDate, 0, 0, 0, 0, pclMsg->szMsg, NULL);
}

//[진성] 호박상자 => 2008-9-16
void cltServer::DoMsg_DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST(sPacketHeader* pPacket)
{
	sDBResponse_Amber_Box_Select_Item_List* pclMsg = (sDBResponse_Amber_Box_Select_Item_List*)pPacket;

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer(pclMsg->m_siCharID);
	if ( NULL == pclchar )	return;
		
	cltGameMsgResponse_Amber_Box_Select_Item_List clInfo( pclMsg->m_clAmberboxItemList );
	cltMsg clInfoMsg( GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clInfoMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET(sPacketHeader* pPacket)
{
	sDBResponse_Amber_Box_Select_Item_Get* pclMsg = (sDBResponse_Amber_Box_Select_Item_Get*)pPacket;
	
	if( 0 >= pclMsg->m_siResult )	return;

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer(pclMsg->m_siCharID);
	if ( NULL == pclchar )			return;

	if(false == pclMsg->m_clGetItem.CanPile(pclClient->pclItemManager))
	{
		cltItem	clItem;
		clItem.Set(&pclMsg->m_clGetItem);
		clItem.SetItemNum(1);

		for(SI32 Count = 0; Count < pclMsg->m_clGetItem.siItemNum; ++Count)
			InputItem( pclchar, &clItem, INPUTITEMPERSON_OPTION_AMBERBOX );
	}
	else
	{
		if ( 0 < pclMsg->m_siToItemPos && MAX_ITEM_PER_PERSON > pclMsg->m_siToItemPos )
		{
			pclchar->pclCI->clCharItem.clItem[pclMsg->m_siToItemPos].Set( &pclMsg->m_clToItem );
		}
	}

	if( 0 < pclMsg->m_siRemainItemPos && MAX_ITEM_PER_PERSON > pclMsg->m_siRemainItemPos )
	{
		pclchar->pclCI->clCharItem.clItem[pclMsg->m_siRemainItemPos].Set( &pclMsg->m_clRemainItem );
	}

	//명품 공지.
	/*
	if( ITEMUNIQUE(12531) == pclMsg->m_clGetItem.siUnique	|| 
		ITEMUNIQUE(13626) == pclMsg->m_clGetItem.siUnique	|| 
		ITEMUNIQUE(13627) == pclMsg->m_clGetItem.siUnique	||
		ITEMUNIQUE(12532) == pclMsg->m_clGetItem.siUnique	||
		ITEMUNIQUE(5748)  == pclMsg->m_clGetItem.siUnique	||
		ITEMUNIQUE(5745)  == pclMsg->m_clGetItem.siUnique	||
		ITEMUNIQUE(13356)  == pclMsg->m_clGetItem.siUnique	||
		ITEMUNIQUE(13655)  == pclMsg->m_clGetItem.siUnique	||
		ITEMUNIQUE(13659)  == pclMsg->m_clGetItem.siUnique)
	{
	*/
	if ( (m_pclBoxItemMgr) && (TRUE == m_pclBoxItemMgr->IsInBoxItemCanNotice(pclMsg->m_clUseItem.siUnique, pclMsg->m_clGetItem.siUnique)) )
	{
		// pck : GM권한이 없을때만 공지나오도록 한다.
		SI32 gamemasterlevel = pclchar->GetGameMasterLevel();
		if( gamemasterlevel < BASIC_GAMEMASTER_LEVEL )
		{
			NTCHARString32 ntstrName( pclchar->GetName() );
			cltGameMsgResponse_RareItemFromTreasureBox clInfo2( ntstrName, &pclMsg->m_clUseItem, &pclMsg->m_clGetItem );
			cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX, sizeof(clInfo2), (BYTE*)&clInfo2);
		
			// [종호_NHN->NDOORS] 한국인경우 18세 이상 유저들에게만 공지
            if((IsWhereServiceArea(ConstServiceArea_Korea) == true))
				pclCM->SendAllAdultUserMsg((sPacketHeader*)&clMsg);
			else
				pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
	}
	
	cltGameMsgResponse_Amber_Box_Select_Item_Get clInfo( pclMsg->m_siRemainItemPos, &pclMsg->m_clRemainItem,
														 pclMsg->m_siToItemPos,		&pclMsg->m_clToItem	);
	cltMsg clInfoMsg( GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clInfoMsg );

	// [로그추가 : 황진성 2008. 9. 30] // 호박상자에서 얻은 아이템
	// [춘기] : Param1 - 상자 유니크
	SendLog_WriteLogToDB(	LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_AMBER_BOX_GET_ITEM, 
							0, pclchar, NULL, 0, 0, 0, &pclMsg->m_clGetItem, 0, 0, 0, 0, 0, 0, 
							pclMsg->m_clUseItem.siUnique, 0, 0, 0, 0, 
							NULL, NULL);
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWSELECTBOX_ITEM_GET(sPacketHeader* pPacket)
{
	sDBResponse_NewSelectBox_Item_Get* pclMsg = (sDBResponse_NewSelectBox_Item_Get*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32 siGetCharID = pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	if ( siGetCharID != pclMsg->m_uiCharID )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( siGetCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	// 사용하고 남은 상자 아이템 정보를 셋팅한다
	if ( (0 < pclMsg->m_siBoxItemPos) && (MAX_ITEM_PER_PERSON > pclMsg->m_siBoxItemPos) )
	{
		pclchar->pclCI->clCharItem.clItem[ pclMsg->m_siBoxItemPos ].Set( &pclMsg->m_clBoxItem );
	}

	// 받은 아이템 정보를 셋팅한다
	if ( (0 < pclMsg->m_siGetItemPos) && (MAX_ITEM_PER_PERSON > pclMsg->m_siGetItemPos) )
	{
		pclchar->pclCI->clCharItem.clItem[ pclMsg->m_siGetItemPos ].Set( &pclMsg->m_clGetItem );
	}

	if ( 0 < pclMsg->m_siCanNotice )
	{
		NTCHARString32 strName( pclchar->GetName() );
		cltGameMsgResponse_RareItemFromTreasureBox clInfo2( strName, &pclMsg->m_clUseItem, &pclMsg->m_clToItem );
		cltMsg clMsg( GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX, sizeof(clInfo2), (BYTE*)&clInfo2 );
		pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
	}

	cltGameMsgResponse_NewSelectBox_GetItem clInfo( &pclMsg->m_clBoxItem, pclMsg->m_siBoxItemPos, &pclMsg->m_clGetItem, pclMsg->m_siGetItemPos, &pclMsg->m_clToItem );
	cltMsg clMsg( GAMEMSG_RESPONSE_NEWSELECTBOX_GETITEM, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );

	pclLog->FilePrint( TEXT("Config\\NewYear.log"), TEXT("PersonID=%d\tBoxItemUnique=%d\tSelectIndex=%d\tGetItemUnique=%d\tGetItemNum=%d"), 
					pclMsg->m_siPersonID,
					pclMsg->m_clUseItem.siUnique, pclMsg->m_siSelectIndex, 
					pclMsg->m_clToItem.siUnique, pclMsg->m_clToItem.siItemNum);

	// 로그
	// siParam1 = 선택한 인덱스값
	// siParam2 = 사용한 아이템(박스 아이템)
	SendLog_WriteLogToDB( LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_NEWSELECTBOX_GET_ITEM, 
						0, pclchar, NULL, 0, 0, 0, &pclMsg->m_clToItem, 0, 0, 0, 0, 0, 0, 
						pclMsg->m_siSelectIndex, pclMsg->m_clUseItem.siUnique, 0, 0, 0, 
						NULL, NULL);

}

