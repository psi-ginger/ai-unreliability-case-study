//---------------------------------
// 2004/8/17 이제형
//---------------------------------

#include "Char\CharManager\CharManager.h"


#include "..\Server\Server.h"

#include "..\Common\Map\Map.h"
#include "..\Common\Father\Father.h"
#include "..\Common\VoteMgr\VoteMgr.h"
#include "..\Common\Skill\Skill-Manager.h"

#include "..\Common\Order\Order.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "MsgType-Horse.h"
#include "Msg/MsgType-Vote.h"
#include "Msg/MsgType-Friend.h"
#include "Msg/MsgType-Summon.h"

#include "../../Server/AuthServer/AuthMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Summon.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "..\Notice\Notice.h"
#include "HashTable/HashTable_StringInt.h"
#include "HashTable/HashTable_IntInt.h"



void cltServer::DoMsg_DBMSG_RESPONSE_SETSUMMON(sPacketHeader* pPacket)
{
	sDBResponse_SetSummon* pclMsg = (sDBResponse_SetSummon *)pPacket;

	SI32 id	= pclMsg->usCharID;
	// 캐릭터 로그아웃을 할때도 SetSummon을 호출하기 때문에 ValidID를 먼저 체크한다.
	if( pclCM->IsValidID(id) != TRUE )	return; 

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if( pclchar == NULL )										return;
	if( pclchar->pclCI->GetPersonID() != pclMsg->siPersonID)	return;

	if( pclMsg->siResult == 1)
	{
		SI16 siSummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->clSummon.siSummonID);

		cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummon(siSummonIndex);

		cltSummon* pclLogSummon = &pclMsg->clSummon;

		if( pclLogSummon == NULL)
			return;

		SI32 siOriWeaponStoneNum = 0;
		SI32 siOriDefenseStoneNum  = 0;
		if( NULL != pclSummon)
		{
			siOriWeaponStoneNum = pclSummon->siAttackWearStoneNum;
			siOriDefenseStoneNum = pclSummon->siDefenseWearStoneNum;
		}
		switch(pclMsg->siReason)
		{
		case SETSUMMON_REASON_MAKE:
			{
				// 소환수를 추가하는 DB통신이 끝난다.
				pclchar->bIsNowAddSummonThroughDB = false;

				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				// 처음 소환수를 생성하는 것이므로, 서버에서 DB로 전송한 SummonIndex를 사용한다.
				cltSimpleSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonBase(pclMsg->siSummonIndex);

				cltSimpleSummon clSimpleSummon;
				clSimpleSummon.siKind = pclMsg->clSummon.siKind;
				clSimpleSummon.siLevel = pclMsg->clSummon.clIP.siLevel;
				clSimpleSummon.siSummonID = pclMsg->clSummon.siSummonID;
				MStrCpy( clSimpleSummon.szName, pclMsg->clSummon.szName, MAX_SUMMON_NAME );

				if(clSimpleSummon.siSummonID > 0 )
				{
					pclSummon->Set(&clSimpleSummon);

					cltGameMsgResponse_BaseSummon clInfo(pclCM->CR[id]->GetCharUnique(), pclMsg->siSummonIndex, &clSimpleSummon);
					cltMsg clMsg(GAMEMSG_RESPONSE_BASESUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				if(pclClient->IsCountrySwitch(Switch_SummonHouse))
				{
					SI32 siSummonIndex;
					pclchar->pclCI->clSummonInfo.GetEmptySummonIndex(&siSummonIndex);

					if(siSummonIndex >= 0)
					{
						sDBRequest_SummonSlotIn clMsg(id, pclchar->pclCI->GetPersonID(), siSummonIndex, pclMsg->clSummon.siSummonID );
						pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
				else
				{
					SI32 siSummonIndex;
					pclchar->pclCI->clSummonInfo.GetEmptySummonIndex(&siSummonIndex);

					if(siSummonIndex >= 0)
					{
						sDBRequest_SummonSlotIn clMsg(id, pclchar->pclCI->GetPersonID(), siSummonIndex, pclMsg->clSummon.siSummonID );
						pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
			}
			break;

		case SETSUMMON_REASON_UPDATE:
			{
				// 소환수 이름이 변경되면 호출된다. 관련 로그를 남기는 것은 없당!
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 9. 7] // 소환수 진화 로그쓰기. 
					// SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_UPDATE, 
					//						0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->clSummon.siUnique, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					//						0, 0, 0, 0, 0, NULL, NULL);
				}
			}
			break;
		case SETSUMMON_REASON_FREE:
		case SETSUMMON_REASON_DIE:
			{
				if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
				{
					// 소환수가 변신이 되있으면 변신을 해제한다.
					if( pclchar->pclCI->clSummonInfo.IsSummonTranform( siSummonIndex ) )
					{
						pclchar->TransformSummon( pclCM, GetSummonTransformManager() );
					}
				}
				//---------------------------------------------
				// 소환수가 생성되어있어야한다.
				//---------------------------------------------
				SI32 siSummonID = pclCM->CR[id]->GetSummonID();
				if(pclCM->IsValidID(siSummonID)  == TRUE )
				{
					if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex)
					{
						//---------------------------------------------
						// 게임에서 소환수를 지운다. 
						//---------------------------------------------
						pclCM->CR[siSummonID]->DeleteInCell();
						pclCM->DeleteChar(siSummonID);
						//==========================================
						// 주인이 갖고 있는 소환수의 정보를 설정한다.
						//==========================================
						pclCM->CR[id]->siChildCharUnique  = 0;
						pclCM->CR[id]->siSummonIndex	  = 0;

						// 주인한테도 정보를 갱신시킨다.
						cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}

				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				cltSummon*			pclSummon		= pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clFreeSummon.siSummonID);
				cltSimpleSummon*	pclSimpleSummon = pclchar->pclCI->clSummonInfo.GetSimpleSummonFromSummonID(pclMsg->clFreeSummon.siSummonID);

				if( pclMsg->siReason == SETSUMMON_REASON_FREE ) 
				{
					if( pclSummon )					pclSummon->Init();
					if( pclSimpleSummon ) 			pclSimpleSummon->Init();
					pclLogSummon = &pclMsg->clFreeSummon;
				}

				// 클라이언트로 알린다. (소유주에게) 
				cltGameMsgResponse_SummonFree clinfo(pclCM->CR[id]->GetCharUnique(), pclMsg->clSummon.siSummonID);
				cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONFREE, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				// 소환수가 죽었을때만 클라이언트에 메시지로 알려준다.
				if(pclMsg->siReason == SETSUMMON_REASON_DIE)
					SendServerResponseMsg(0, SRVAL_SUMMONDIE,  0, 0, pclCM->CR[id]->GetCharUnique());

				// 움직이는 캐릭터를 사용자로 맞춘다.
				pclCM->CR[id]->siSelectedCharIndex = -1 ;
			}
			break;

		case SETSUMMON_REASON_TAKEIN:
			{
				//---------------------------------------------
				// 소환수가 생성되어있어야한다.
				//---------------------------------------------
				SI32 siSummonID = pclCM->CR[id]->GetSummonID();
				if(pclCM->IsValidID(siSummonID)  == TRUE)
				{
					if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex)
					{
						if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
						{
							// 소환수가 변신이 되있으면 변신을 해제한다.
							if( pclchar->pclCI->clSummonInfo.IsSummonTranform( siSummonIndex ) )
							{
								pclchar->TransformSummon( pclCM, GetSummonTransformManager() );
								siSummonID = pclchar->GetSummonID();
							}
						}

						//---------------------------------------------
						// 게임에서 소환수를 지운다. 
						//---------------------------------------------
						if ( pclCM->IsValidID(siSummonID) )
						{
							pclCM->CR[siSummonID]->DeleteInCell();
							pclCM->DeleteChar(siSummonID);
						}
						//==========================================
						// 주인이 갖고 있는 소환수의 정보를 설정한다.
						//==========================================
						pclCM->CR[id]->siChildCharUnique	= 0;
						pclCM->CR[id]->siSummonIndex		= 0;

						// 주인한테도 정보를 갱신시킨다.
						cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);

						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}

				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
				if(pclSummon)
				{
					pclSummon->Set(&pclMsg->clSummon);
					cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
					cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				// 움직이는 캐릭터를 사용자로 맞춘다.
				pclCM->CR[id]->siSelectedCharIndex = -1 ;
			}
			break;
		case SETSUMMON_REASON_TAKEOUT:
			{
				//---------------------------------------------
				// 소환수가 이미 있으면 집어 넣는다.
				//---------------------------------------------
				SI32 siSummonID = pclCM->CR[id]->GetSummonID();
				cltSummon *pclSummon = NULL ;
				if(pclCM->IsValidID(siSummonID)  == TRUE )
				{
					if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
					{
						// 소환수가 변신이 되있으면 변신을 해제한다.
						if( pclchar->pclCI->clSummonInfo.IsSummonTranform( pclCM->CR[id]->siSummonIndex ) )
						{
							pclchar->TransformSummon( pclCM, GetSummonTransformManager() );
							siSummonID = pclchar->GetSummonID();
						}
					}

					SI32 oldindex = pclCM->CR[id]->siSummonIndex ;
					//--------------------------------------------
					//DB로 업데이트시켜준다.
					//--------------------------------------------
					((cltCharServer*)pclCM->CR[id])->SendDBSummon(SETSUMMON_REASON_UPDATE, oldindex);

					//---------------------------------------------
					// 게임에서 소환수를 지운다. 
					//---------------------------------------------
					pclCM->CR[siSummonID]->DeleteInCell();
					pclCM->DeleteChar(siSummonID);
					//==========================================
					// 주인이 갖고 있는 소환수의 정보를 설정한다.
					//==========================================
					pclCM->CR[id]->siChildCharUnique	= 0;
					pclCM->CR[id]->siSummonIndex		= 0;

					// 주인한테도 정보를 갱신시킨다.
					cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

					pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummon(oldindex);
					if(pclSummon)
					{
						pclSummon->siStatus = SUMMON_STATUS_TAKEIN ;
						pclSummon->Set(pclSummon);
						cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
						cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}

				//---------------------------------------------
				// 소환수를 생성하고 정하고 정보를 갱신한다.
				//---------------------------------------------
				siSummonID = ((cltCharServer*)(pclCM->CR[id]))->CreateSummon(&pclMsg->clSummon);
				if(pclCM->IsValidID(siSummonID)  == TRUE)
				{
					//==========================================
					// 주인이 갖고 있는 소환수의 정보를 설정한다.
					//==========================================
					pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
					if(pclSummon)
					{
						pclSummon->Set(&pclMsg->clSummon);
						cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
						cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					pclCM->CR[id]->siChildCharUnique	= siSummonID;
					pclCM->CR[id]->siSummonIndex		= siSummonIndex;

					cltCharServer *pclSummon = (cltCharServer*)pclCM->GetCharServer(siSummonID);
					if( NULL != pclSummon)
					{
						pclSummon->ParametaBoxAction();
					}

					{
						// 주인한테도 정보를 갱신시킨다.
						cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}	

				// 움직이는 캐릭터를 사용자로 맞춘다.
				pclCM->CR[id]->siSelectedCharIndex = -1 ;
			}
			break;
		case SETSUMMON_REASON_SUMMONSTON:// 소환수 무기장착돌을 사용했을때 
			{
				cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
				// 디비로 부터 받은 정보를 셋팅 한다.
	
				pclSummon->siAttackWearStoneNum = pclMsg->clSummon.siAttackWearStoneNum;
				pclSummon->siDefenseWearStoneNum = pclMsg->clSummon.siDefenseWearStoneNum;
				pclLogSummon = pclSummon;
				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				if(&pclMsg->clSummon)
				{					
					pclSummon->Set(pclSummon);
					cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
					cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				//---------------------------------------------
				// 소환수가 생성되어 있지 않으면 생성하고 설정한다.
				//---------------------------------------------
				SI32 siSummonID = pclCM->CR[id]->GetSummonID();
				if(pclCM->IsValidID(siSummonID)  == FALSE)
				{
					//---------------------------------------------
					// 소환수를 생성하고 정하고 정보를 갱신한다.
					//---------------------------------------------
					siSummonID = ((cltCharServer*)(pclCM->CR[id]))->CreateSummon(pclSummon);
					if(pclCM->IsValidID(siSummonID)  == TRUE)
					{
						pclCM->CR[id]->siChildCharUnique = siSummonID;
						pclCM->CR[id]->siSummonIndex     = siSummonIndex;

						pclCM->CR[siSummonID]->ParametaBoxAction();

						{
							// 주인한테도 정보를 갱신시킨다.
							cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
							cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
							// 성공 메세지를 보내준다
							pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}

				// 성공 메세지를 보내준다
				SendServerResponseMsg(0, SRVAL_SUMMONSUCCESS_USESTON, 0, 0, pclCM->CR[id]->GetCharUnique() );
			}
			break;
		case SETSUMMON_REASON_CHARGESTAMINA:	// PCK - 소환수 스태미너 충전 (08.07.23)
			{
				if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
				{
					// 소환수가 변신이 되있으면 스테미너 충전은 없다.
					if( pclchar->pclCI->clSummonInfo.IsSummonTranform( siSummonIndex ) )
					{
						break;
					}
				}

				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);

				// 스테미너 정보를 업데이트 한다.
				if(pclSummon)
				{
					pclSummon->clIP.SetLuk( pclMsg->clSummon.clIP.GetLuk() );

					cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
					cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

					// 성공 메세지를 보내준다
					((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMONSUCCESS_CHARGESTAMINA,  0, 0, id);
				}				
			}
			break;
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog) && pclLogSummon != NULL) 
		{
			// PCK - 소환수의 상태를 로그로 남긴다. 
			// PCK : 추가 - 소환수의 레벨, 스탯 추가 (09.07.30)
			TCHAR szBA[50];
			ZeroMemory(szBA, sizeof(szBA));
			StringCchPrintf(szBA, sizeof(szBA), TEXT("Level:%d, Str:%d, Dex:%d, Vit:%d"),	
				pclLogSummon->clIP.GetLevel(), pclLogSummon->clIP.GetStr(), pclLogSummon->clIP.GetDex(), 
				pclLogSummon->clIP.GetVit());
			TCHAR szBA2[50];
			ZeroMemory(szBA2, sizeof(szBA2));
			StringCchPrintf(szBA2, sizeof(szBA2), TEXT("Mag:%d, Hnd:%d, Wis:%d"),	
				pclLogSummon->clIP.GetMag(), pclLogSummon->clIP.GetHnd(),pclLogSummon->clIP.GetWis() );

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_SET, 
				pclMsg->siReason, (cltCharServer*)pclCM->CR[id], pclLogSummon, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				siOriWeaponStoneNum, siOriDefenseStoneNum, pclLogSummon->siAttackWearStoneNum, pclLogSummon->siDefenseWearStoneNum, 0, szBA, szBA2);
		}
	}
	else
	{
		switch(pclMsg->siReason)
		{
		case SETSUMMON_REASON_MAKE:
			{
				// 소환수를 추가하는 DB통신이 끝난다.
				pclchar->bIsNowAddSummonThroughDB = false;
			}
			break;
		case SETSUMMON_REASON_UPDATE:
		case SETSUMMON_REASON_FREE:
		case SETSUMMON_REASON_DIE:
		case SETSUMMON_REASON_TAKEIN:
		case SETSUMMON_REASON_TAKEOUT:
			break;
		case SETSUMMON_REASON_SUMMONSTON:// 소환수 돌을 사용했을때 
			break;
		case SETSUMMON_REASON_CHARGESTAMINA:	// PCK : 소환수 스태미너 충전 (08.07.23)
			break;
		}
		return;
	}

	return;
}


void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONREVIVE(sPacketHeader* pPacket)
{
	sDBResponse_SummonRevive* pclMsg = (sDBResponse_SummonRevive *)pPacket;

	BOOL bSuccess = FALSE;

	if( pclMsg->siResult >= 1)
	{
		SI32 id	= pclMsg->siCharID;	
		if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return; 

		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
		//================================================ 
		// Client의 정보를 인벤토리 Update한다.
		//================================================ 
		if(pclchar->pclCI->GetPersonID() == pclMsg->siPersonID)
		{
			if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
			{
				pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		//================================================ 
		// 마을에서 부활시키면 마을에 소환인형 갯수 * 10 냥을 추가한다.
		//================================================ 
		if(pclMsg->siVillageUnique>0 && pclMsg->siVillageUnique < MAX_VILLAGE_NUMBER)
		{
			GMONEY siGetVillageMoney = pclMsg->siItemNum * 10;
			if(pclMsg->siItemNum>= 0)
			{
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, pclMsg->siVillageUnique, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}

		//--------------------------------
		// 부활시킬 소환수의 Kind를 점검한다.
		//--------------------------------
		SI32 kind = pclMsg->clSummon.siKind;
		if(kind > 0 && kind < MAX_KIND_NUMBER)	
		{
			//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
			cltKindInfo* pclki = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
			}
			else
			{
				pclki = pclClient->pclKindInfoSet->pclKI[kind];
			}

			if(pclki == NULL)							return;
			if(pclki->siSummonRank <= 0)				return;
		}

		//---------------------------------------------
		// 소환수를 부활시킨다..
		//---------------------------------------------
		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
		SI32 siSummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->clSummon.siSummonID);
		if(pclSummon)
		{
			pclSummon->Set(&pclMsg->clSummon);
			cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		//---------------------------------------------
		// 소환수가 생성이 되어 있으면 생성하지 않는다.
		//---------------------------------------------
		SI32 siSummonID = pclCM->CR[id]->GetSummonID();
		if(pclCM->IsValidID(siSummonID)  == FALSE)
		{
			//---------------------------------------------
			// 소환수를 생성하고 정하고 정보를 갱신한다.
			//---------------------------------------------
			siSummonID = ((cltCharServer*)(pclCM->CR[id]))->CreateSummon(&pclMsg->clSummon);

			if(pclCM->IsValidID(siSummonID)  == TRUE)
			{
				pclCM->CR[id]->siChildCharUnique	= siSummonID;
				pclCM->CR[id]->siSummonIndex		= siSummonIndex;

				pclCM->CR[siSummonID]->ParametaBoxAction();

				{
					// 주인한테도 정보를 갱신시킨다.
					cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);

					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
		
		{
			//================================================ 
			// 결과를 전송한다.
			//================================================ 
			cltGameMsgResponse_SummonRevive clinfo(pclCM->CR[id]->GetCharUnique(), 1, pclMsg->clSummon.siSummonID);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONREVIVE , sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				cltServer * pclServer = (cltServer*)pclClient;
				// PCK - 아이템 사용 로그를 남긴다.
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
												USEITEM_REASON_SUMMONREVIVE, pclchar, NULL, 0, 0, 0, &pclMsg->clItem_Log, 0, 0, 0, 0, 0, 
												pclMsg->siItemPos, 0, 0, 0, 0, 0, NULL, NULL);

				// [로그추가 : 황진성 2007. 10. 8] // 소환수 부활.
				// param1 == 아이템 유니크
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_REVIVE, 
												0, (cltCharServer*)pclCM->CR[id], &pclMsg->clSummon, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
												pclMsg->clItem_Log.siUnique, 0, 0, 0, 0, NULL, NULL);
			}
		}
		
		bSuccess = TRUE;
		return;
	}
	
	if(bSuccess == FALSE)
	{
		SI32 id	= pclMsg->siCharID;	
		if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return; 
		
		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
		//================================================ 
		// 결과를 전송한다.
		//================================================ 
		cltGameMsgResponse_SummonRevive clinfo(pclchar->GetCharUnique(), 0, pclMsg->clSummon.siSummonID);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONREVIVE , sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

	

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONEVOLVE(sPacketHeader* pPacket)
{
	sDBResponse_SummonEvolve*pclMsg = (sDBResponse_SummonEvolve *)pPacket;

	BOOL bSuccess = FALSE;


	if( pclMsg->siResult >= 1)
	{
		SI32 id	= pclMsg->siCharID;	
		if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return; 
	
		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];

		//===================================
		// 진화처리가 완료되었음을 알린다.
		//===================================
		pclchar->bIsEvolving = false;

		//================================================ 
		// Client의 정보를 인벤토리 Update한다.
		//================================================ 
		if(pclchar->pclCI->GetPersonID() == pclMsg->siPersonID && pclMsg->bItemUse )
		{
			if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
			{
				pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		
		//================================================ 
		// 마을에서 진화시키면 마을에 이익금을 준다.
		//================================================ 
		if(pclMsg->siVillageUnique>0 && pclMsg->siVillageUnique < MAX_VILLAGE_NUMBER)
		{
			GMONEY siGetVillageMoney = 0; // 마을 이익금 증가액 SI32 -> GMONEY

			// 국가 스위치 적용 - by LEEKH
			if( pclClient->IsCountrySwitch( Switch_SummonEnolveVillageMoney ) )
			{
				// 진화시에 마을 이익금 주는 부분 변경 -KKM-
				// 이용권으로 진화
				if( ITEMUNIQUE(24015) == pclMsg->siUsedItemUnique )
				{
					SI64 itemPrice = pclNPCItemInfo->GetRealPrice( GetUniqueFromHash( TEXT("KIND_INVESTMENT") ), pclMsg->siUsedItemUnique );
					siGetVillageMoney = pclMsg->siItemNum * itemPrice / 2;
				}
				// 축돌로 진화
				else
				{
					cltItemPriceUnit* pPrice = pclItemManager->pclItemPrice->GetPrice( pclItemManager, pclMsg->siUsedItemUnique );
					if( NULL != pPrice )
					{
						siGetVillageMoney = pclMsg->siItemNum * pPrice->siPrice / 2;
					}
				}
			}
			else
			{
				siGetVillageMoney = pclMsg->siItemNum * 100000;
			}

			if(pclMsg->siItemNum>= 0)
			{
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, pclMsg->siVillageUnique, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}

		//--------------------------------
		// 진화시킬 소환수의 Kind를 점검한다.
		//--------------------------------
		SI32 kind = pclMsg->clSummon.siKind;
		if(kind > 0 && kind < MAX_KIND_NUMBER)	
		{
			//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
			cltKindInfo* pclki = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
			}
			else
			{
				pclki = pclClient->pclKindInfoSet->pclKI[kind];
			}

			if(pclki == NULL)							return;
			if(pclki->siSummonRank <= 0)				return;
		}

		SI32 siSummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->clSummon.siSummonID);

		//---------------------------------------------
		// 소환수가 나와있지 않으면 진화시킬수 없다.
		//---------------------------------------------
		SI32 siSummonID = pclCM->CR[id]->GetSummonID();
		if(pclCM->IsValidID(siSummonID)  == TRUE)
		{
			SI32 siSummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->clSummon.siSummonID);
			if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex)
			{
				//---------------------------------------------
				// 게임에서 소환수를 지운다. 
				//---------------------------------------------
				pclCM->CR[siSummonID]->DeleteInCell();
				pclCM->DeleteChar(siSummonID);
				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				pclCM->CR[id]->siChildCharUnique  = 0;
				pclCM->CR[id]->siSummonIndex	  = 0;
				
				// 주인한테도 정보를 갱신시킨다.
				cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);

				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				
			}
		}
			
		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
		if(pclSummon)
		{
			pclSummon->Set(&pclMsg->clSummon);
			cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		//==========================================
		// 보관소 안의 소환수의 정보를 설정한다.
		//==========================================
		cltSimpleSummon *pclSimpleSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSimpleSummonFromSummonID(pclMsg->clSummon.siSummonID);
		SI32 siSlotIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonBaseIndexFromSummonID(pclMsg->clSummon.siSummonID);
		if(pclSimpleSummon)
		{
			pclSimpleSummon->Set(&pclMsg->clSummon);
			cltGameMsgResponse_BaseSummon clInfo(pclCM->CR[id]->GetCharUnique(), siSlotIndex, pclSimpleSummon);
			cltMsg clMsg(GAMEMSG_RESPONSE_BASESUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		//================================================ 
		// 결과를 전송한다.
		//================================================ 
		{
			cltGameMsgResponse_SummonEvolve clinfo(pclCM->CR[id]->GetCharUnique(), 1, pclMsg->clSummon.siSummonID, pclMsg->clSummon.siKind,pclMsg->siMode);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONEVOLVE, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				cltServer * pclServer = (cltServer*)pclClient;
				// PCK - 아이템 사용 로그를 남긴다.
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
									USEITEM_REASON_SUMMONEVOLVE, pclchar, NULL, 0, 0, 0, &pclMsg->clItem_Log, 0, 0, 0, 0, 0, 
									pclMsg->siItemPos, 0, 0, 0, 0, 0, NULL, NULL);

				// [로그추가 : 황진성 2007. 10. 9] // 소환수 진화.
				// param1 == 모드, param2 == 아이템 유니크, param3 == 진화되기전의 소환수 카인드
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_EVOLVE, 
												0, (cltCharServer*)pclCM->CR[id], &pclMsg->clSummon, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
												pclMsg->siMode, pclMsg->siUsedItemUnique, pclMsg->siSummonKind, 0, 0, NULL, NULL);
			}
		}

/*

#ifdef EVENT_SUMMON_EVENT
		//================================================ 
		// 소환수 Rank ? 이상은 전체공지를 한다.
		//================================================ 
		if(kind > 0 && kind < MAX_KIND_NUMBER)	
		{
			cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];

			if ( pclMsg->siMode == SUMMONEVOLVE_EVOLVE )
			{
				if(pclki && pclki->siSummonRank >= 8)
				{
					SI32 itemTable[] = 
					{
						ITEMUNIQUE(5350),
						ITEMUNIQUE(5355),
						ITEMUNIQUE(5360),
						ITEMUNIQUE(5365)
					};

					SI16 rndIndex = rand() % 4;
                    
					SI32 itemUnique = itemTable[ rndIndex ];

					SI32 itemNum = 1;

					if( pclki->siSummonRank >= 25 )
					{
						itemNum = 6;
					}
					else if( pclki->siSummonRank >= 17 )
					{
						itemNum = 4;
					}
					else if( pclki->siSummonRank >= 8 )
					{
						itemNum = 2;
					}
					
					pclchar->GiveItem( itemUnique , itemNum , INPUTITEMPERSON_OPTION_EVENT,0 );
					
				}
			}
		}

#endif			
		*/

		//================================================ 
		// 소환수 Rank ? 이상은 전체공지를 한다.
		//================================================ 
		#define LIMIT_RANK_SUMMON_REVIVE_BROADCAST	30

		if(kind > 0 && kind < MAX_KIND_NUMBER)	
		{
			//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
			cltKindInfo* pclki = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
			}
			else
			{
				pclki = pclClient->pclKindInfoSet->pclKI[kind];
			}

			bool bGameMaster = false ;
			if(pclCM->CR[id]->GetGameMasterLevel()< BASIC_GAMEMASTER_LEVEL + 1 )
			{
				bGameMaster = true ;
			}

#ifdef _DEBUG
			// 파티획득막음 //bGameMaster = true ;
#endif

			if ( bGameMaster && pclMsg->siMode == SUMMONEVOLVE_EVOLVE )
			{
				if(pclki && pclki->siSummonRank >= LIMIT_RANK_SUMMON_REVIVE_BROADCAST)
				{
					cltGameMsgResponse_HighSummon clinfo((TCHAR*)pclCM->CR[id]->GetName(), pclCM->CR[id]->pclCI->GetPersonID(), kind, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_HIGHSUMMON, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
				}
			}
		}

		bSuccess = TRUE;

		if ( pclMsg->siMode == SUMMONEVOLVE_EVOLVE )
		{
			// 소환수 진화 퀘스트 중이라면, 
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SUMMONEVOLUTION);
			if ( pclMsg->bQuestEvolve )
			{
				pclCM->CR[id]->pclCI->clBI.bQuestEvolve = true ;
			}
		}

		return;
	}
	
	if(bSuccess == FALSE)
	{
		SI32 id	= pclMsg->siCharID;	
		if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return; 
		
		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];

		//===================================
		// 진화처리가 완료되었음을 알린다.
		//===================================
		pclchar->bIsEvolving = false;

		//================================================ 
		// 결과를 전송한다.
		//================================================ 
		cltGameMsgResponse_SummonEvolve clinfo(pclchar->GetCharUnique(), 0, pclMsg->clSummon.siSummonID, pclMsg->clSummon.siKind,pclMsg->siMode);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONEVOLVE , sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONSCROLL(sPacketHeader* pPacket)
{
	sDBResponse_SummonScroll*pclMsg = (sDBResponse_SummonScroll *)pPacket;

	BOOL bSuccess = FALSE;

	if( pclMsg->siResult >= 1)
	{
		SI32 id	= pclMsg->siCharID;	
		if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return; 
	
		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
		//================================================ 
		// Client의 정보를 인벤토리 Update한다.
		//================================================ 
		if(pclchar->pclCI->GetPersonID() == pclMsg->siPersonID)
		{
			if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
			{
				pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		//================================================ 
		// 마을에서 진화시키면 마을에 부적 갯수 * 20 냥을 추가한다.
		//================================================ 
		if(pclMsg->siVillageUnique>0 && pclMsg->siVillageUnique < MAX_VILLAGE_NUMBER)
		{
			GMONEY siGetVillageMoney = pclMsg->siItemNum * 20;
			if(pclMsg->siItemNum>= 0)
			{
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, pclMsg->siVillageUnique, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}
		

		//--------------------------------
		// 부활시킬 소환수의 Kind를 점검한다.
		//--------------------------------
		SI32 kind = pclMsg->clSummon.siKind;
		if(kind > 0 && kind < MAX_KIND_NUMBER)	
		{
			//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
			cltKindInfo* pclki = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
			}
			else
			{
				pclki = pclClient->pclKindInfoSet->pclKI[kind];
			}

			if(pclki == NULL)							return;
			if(pclki->siSummonRank <= 0)				return;
		}
		
		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
		SI32 siSummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->clSummon.siSummonID);
		if(pclSummon)
		{
			pclSummon->Set(&pclMsg->clSummon);
			cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		{
			//================================================ 
			// 결과를 전송한다.
			//================================================ 
			cltGameMsgResponse_SummonScroll clinfo(pclCM->CR[id]->GetCharUnique(), 1, pclMsg->clSummon.siSummonID);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONSCROLL, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		
		// 소환수 강화 퀘스트 중이라면, 
		pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SUMMONSCROLL);

		
		bSuccess = TRUE;
		return;

	}
	
	if(bSuccess == FALSE)
	{
		SI32 id	= pclMsg->siCharID;	
		if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return; 
		
		cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
		//================================================ 
		// 결과를 전송한다.
		//================================================ 
		cltGameMsgResponse_SummonScroll clinfo(pclchar->GetCharUnique(), 0, pclMsg->clSummon.siSummonID);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONSCROLL , sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMON_STATUSINIT(sPacketHeader* pPacket)
{
	sDBResponse_Summon_StatusInit *pclMsg = (sDBResponse_Summon_StatusInit *)pPacket;

	if ( pclMsg->siResult != 1 ) return ;

	SI32 id	= pclMsg->siCharID;	
	if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return;

	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
	//================================================ 
	// Client의 정보를 인벤토리 Update한다.
	//================================================ 
	if(pclchar->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	//--------------------------------
	// 소환수의 Kind를 점검한다.
	//--------------------------------
	SI32 kind = pclMsg->clSummon.siKind;
	if(kind > 0 && kind < MAX_KIND_NUMBER)	
	{
		//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
		cltKindInfo* pclki = NULL;
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
		}
		else
		{
			pclki = pclClient->pclKindInfoSet->pclKI[kind];
		}

		if(pclki == NULL)							return;
		if(pclki->siSummonRank <= 0)				return;
	}

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclMsg->clSummon.siSummonID);

	SI32 siSummonID = pclCM->CR[id]->GetSummonID();
	if(pclCM->IsValidID(siSummonID)  == TRUE)
	{
		if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex)
		{
			//---------------------------------------------
			// 게임에서 소환수를 지운다. 
			//---------------------------------------------
			pclCM->CR[siSummonID]->DeleteInCell();
			pclCM->DeleteChar(siSummonID);
			//==========================================
			// 주인이 갖고 있는 소환수의 정보를 설정한다.
			//==========================================
			pclCM->CR[id]->siChildCharUnique  = 0;
			pclCM->CR[id]->siSummonIndex	  = 0;

			// 주인한테도 정보를 갱신시킨다.
			cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);

			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	//==========================================
	// 주인이 갖고 있는 소환수의 정보를 설정한다.
	//==========================================
	cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonFromSummonID(pclMsg->clSummon.siSummonID);
	if(pclSummon)
	{
		pclSummon->Set(&pclMsg->clSummon);
		cltGameMsgResponse_Summon clInfo(pclCM->CR[id]->GetCharUnique(), siSummonIndex, pclSummon);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			cltServer * pclServer = (cltServer*)pclClient;
			// PCK - 아이템 사용 로그를 남긴다.
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
				USEITEM_REASON_SUMMONSTATUSINIT, pclchar, NULL, 0, 0, 0, &pclMsg->clItem_Log, 0, 0, 0, 0, 0, 
				pclMsg->siItemPos, 0, 0, 0, 0, 0, NULL, NULL);

			// PCK - 소환수 스탯 초기화
			// param1 == 아이템 유니크.
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_STATUS_INIT, 
				0, (cltCharServer*)pclCM->CR[id], &pclMsg->clSummon, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				pclMsg->clItem_Log.siUnique, 0, 0, 0, 0, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMON_SETAUTOHEAL(sPacketHeader* pPacket)
{
	sDBResponse_Summon_SetAutoHeal* pclMsg = (sDBResponse_Summon_SetAutoHeal*)pPacket ;

	if ( pclMsg->siResult != 1 ) return ;

	SI32 id	= pclMsg->siCharID;	
	if(pclCM->IsValidID(id) != TRUE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)	return;

	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	pclchar->pclCI->clSummonInfo.bAutoHeal = pclMsg->bAutoHeal ;
	pclchar->pclCI->clSummonInfo.siSelectAutoHeal = pclMsg->siSelectAutoHeal ;

	cltGameMsgResponse_Summon_SetAutoHeal clinfo(pclCM->CR[id]->GetCharUnique(), pclMsg->siResult,
												 pclMsg->siSelectAutoHeal,pclMsg->bAutoHeal);
	cltMsg clMsg(GAMEMSG_RESPONSE_SUMMON_SETAUTOHEAL, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 8] // 소환수 자동 회복.
	//	// param1 == 결과, param2 == 30%냐 50%냐 70%냐. param3 == 쓸 것인가 말 것인가.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_SET_AUTO_HEAL, 
	//									0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
	//									pclMsg->siResult, pclMsg->siSelectAutoHeal, pclMsg->bAutoHeal, 0, 0, NULL, NULL);
	//}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONTRADE( sPacketHeader* pPacket )
{
	sDBResponse_SummonTrade* pclMsg = (sDBResponse_SummonTrade* )pPacket;

	if ( pclMsg->siResult != 1 )
		return;

	SI32 id = pclMsg->usCharID;

	/*
	//---------------------------------------
	// 통계에 추가
	//---------------------------------------
	++pclStatisticsManager->clDailyStatistics.siSystemBuyHorseNum; 
	pclStatisticsManager->clDailyStatistics.siSystemBuyHorseMoney += pclMsg->siIncreaseMoney;
	//---------------------------------------
	*/
	if ( id > 0 && pclCM->IsValidID( id ) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		//---------------------------------------------
		// 소환수가 생성되어있어야한다.
		//---------------------------------------------
		SI32 siSummonID = pclCM->CR[id]->GetSummonID();
		if(pclCM->IsValidID(siSummonID)  == TRUE )
		{
			if(pclCM->CR[siSummonID]->GetSummonIndex() == pclMsg->clSummon.siSummonID)
			{
				cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
				pclchar->bIsTradeing = false;
				// 기록한다.
				pclLog->FilePrint(TEXT("Config\\BuySummon.log"), TEXT("User : [%s] / SummonKind : [%s] / SummonPrice : [%d]"),
								  pclCM->CR[id]->GetName() , pclCM->CR[siSummonID]->pclKI->szCode,pclMsg->siMoney);

				//---------------------------------------------
				// 게임에서 소환수를 지운다. 
				//---------------------------------------------
				pclCM->CR[siSummonID]->DeleteInCell();
				pclCM->DeleteChar(siSummonID);
				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				pclCM->CR[id]->siChildCharUnique  = 0;
				pclCM->CR[id]->siSummonIndex	  = 0;

				{
					// 주인한테도 정보를 갱신시킨다.
					cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				pclCM->CR[id]->pclCI->clSummonInfo.SummonInit(pclMsg->clSummon.siSummonID);

				pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );

				{
					cltGameMsgResponse_TradeMerchantBuySummon clTradeMerchantBuySummon(id, pclMsg->siResult, pclMsg->clSummon.siSummonID, pclMsg->siMoney, pclMsg->siLeftMoney );
					cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONTRADE, sizeof(clTradeMerchantBuySummon), (BYTE*)&clTradeMerchantBuySummon);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 황진성 2007. 10. 8] // 소환수 판매.
						// param1 == 결과. param2 == 유니크.
						cltServer * pclServer = (cltServer*)pclClient;
						pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_TRADE, 
														0, (cltCharServer*)pclCM->CR[id], &pclMsg->clSummon, 0, pclMsg->siMoney, pclMsg->siLeftMoney, NULL, 0, 0, 0, 0, 0, 0, 
														pclMsg->siResult, pclMsg->clSummon.siSummonID, 0, 0, 0, NULL, NULL);
					}
				}

				// 소환수 정보를 업데이트 시킨다. 
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_SUMMON, true, 0);
			}
		}
	}
}

// [성웅] 서버 이벤트 관련 개인 점수 요청 결과
void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONUSESTON(sPacketHeader *pPacket)
{
}

// Base에 있는 해당 소환수를 SLot에 넣는다.
void cltServer::DoMsg_DBMSG_RESPONSE_SUMMON_SLOTIN(sPacketHeader* pPacket)
{
	sDBResponse_SummonSlotIn* pclMsg = (sDBResponse_SummonSlotIn*)pPacket;
	if ( NULL == pclMsg )																return	;
	SI08 siSlotIndex =	pclMsg->siSummonIndex	;			// 해당 소환수를 넣을 슬롯
	SI32 id = pclMsg->siCharID					;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if( pclchar == NULL )																return ;
	
	if (pclMsg->siResult == 1)
	{
		// 성공 했을때만 받은 cltSummon 과 cliItem 정보를 셋팅 한다.
		//	먼저 해당 슬롯이 정상적인 슬롯인지 확인 한다.
		if( siSlotIndex < 0 || siSlotIndex >= MAX_SUMMON_OWN )							return	;
		// 실제로 해당 슬롯이 비어 있는지 확인 한다.
		if( pclchar->pclCI->clSummonInfo.clSummon[ siSlotIndex ].siSummonID > 0 )		return	;

		// 슬롯이 비었으면 해당 소환수를 셋팅
		pclchar->pclCI->clSummonInfo.SetSummon( siSlotIndex, &pclMsg->clSummon)	;
		// 해당 슬롯의 소환수의 아이템을 셋팅한다.
		pclchar->pclCI->clSummonInfo.clItemInfo[ siSlotIndex ].Set(&pclMsg->clSummonItemInfo)	;

		// 클라이언트도 갱신 한다.
		cltGameMsgResponse_SlotIn clInfo( pclchar->GetCharUnique(), siSlotIndex, &pclMsg->clSummon ,&pclMsg->clSummonItemInfo );
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONSLOTIN, sizeof(clInfo), (BYTE*)&clInfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		pclchar->SetUpdateSwitch(UPDATE_SUMMON, true, 0);

		// 퀘스트 관련 소환수보관소에서 빼기
		((cltCharServer*)pclCM->CR[id])->IncQuestPara( QUEST_CLEAR_COND_SUMMONHOUSE_OUTPUT, 0, 1 );

		if(pclClient->IsCountrySwitch(Switch_SummonHouse))
		{
		}
		else
		{
			if(pclMsg->clSummon.siStatus == SUMMON_STATUS_TAKEOUT)
			{
				//---------------------------------------------
				// 소환수가 생성되어 있지 않으면 생성하고 설정한다.
				//---------------------------------------------
				SI32 siSummonID = pclchar->GetSummonID();
				if(pclCM->IsValidID(siSummonID)  == FALSE)
				{
					//---------------------------------------------
					// 소환수를 생성하고 정하고 정보를 갱신한다.
					//---------------------------------------------
					siSummonID = pclchar->CreateSummon(&pclMsg->clSummon);
					if(pclCM->IsValidID(siSummonID)  == TRUE)
					{
						pclchar->siChildCharUnique = siSummonID;
						pclchar->siSummonIndex     = siSlotIndex;

						pclCM->CR[siSummonID]->ParametaBoxAction();

						// 주인한테도 정보를 갱신시킨다.
						cltGameMsgResponse_ChangeSummonStatus clInfo(pclchar->GetCharUnique(), pclchar->siChildCharUnique, pclchar->siSummonIndex);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
						
						pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}
	}
}

// 해당 소환수를 SLot에서 빼낸다.
void cltServer::DoMsg_DBMSG_RESPONSE_SUMMON_SLOTOUT(sPacketHeader* pPacket)
{
	sDBResponse_SummonSlotOut* pclMsg = (sDBResponse_SummonSlotOut*)pPacket;
	if ( NULL == pclMsg )																return	;
	SI08 siSummonIndex =	pclMsg->siSummonIndex	;			// 해당 소환수 인덱스
	SI32 id = pclMsg->siCharID					;
	if ( id < 0 )																		return	;
	if (pclMsg->siResult == 1) {
		// 성공 했을때만 해당 슬롯의 소환수를 제거 한다.
		//	먼저 해당 슬롯이 정상적인 슬롯인지 확인 한다.
		if( siSummonIndex > MAX_SUMMON_OWN )												return	;
		// 실제로 해당 슬롯의 정보를 비운다
		pclCM->CR[id]->pclCI->clSummonInfo.clSummon[ siSummonIndex ].Init()	;
		// 해당 슬롯의 소환수의 아이템을 초기화한다
		pclCM->CR[id]->pclCI->clSummonInfo.clItemInfo[ siSummonIndex ].Init();
		// 클라이언트도 갱신 한다.
		cltGameMsgResponse_SlotOut clInfo( id, siSummonIndex );
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONSLOTOUT, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_SUMMON, true, 0);

		// 퀘스트 관련 소환수보관소에 넣어보기
		((cltCharServer*)pclCM->CR[id])->IncQuestPara( QUEST_CLEAR_COND_SUMMONHOUSE_INPUT, 0, 1 );

		//---------------------------------------------
		// 소환수가 생성되어있어야한다.
		//---------------------------------------------
		SI32 siSummonID = pclCM->CR[id]->GetSummonID();
		if(pclCM->IsValidID(siSummonID)  == TRUE )
		{
			if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex)
			{
				//---------------------------------------------
				// 게임에서 소환수를 지운다. 
				//---------------------------------------------
				pclCM->CR[siSummonID]->DeleteInCell();
				pclCM->DeleteChar(siSummonID);
				//==========================================
				// 주인이 갖고 있는 소환수의 정보를 설정한다.
				//==========================================
				pclCM->CR[id]->siChildCharUnique  = 0;
				pclCM->CR[id]->siSummonIndex	  = 0;

				// 주인한테도 정보를 갱신시킨다.
				cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
}