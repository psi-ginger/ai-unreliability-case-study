#include "../Client.h"
#include "../Music/Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-Summon.h"

#include "../Common/Map/MapManager/MapManager.h"
#include "../Common/Map/MapManager/MapManager.h"
#include "Char/CharManager/CharManager.h"

#include "../Interface/DrawChat/DrawChat.h"
#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Interface/MuDang/MuDangDlg.h"

#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NCharStatus/NCharStatus.h"
#include "../NInterface/NCharStatus/NCharSummon.h"
#include "../NInterface/NSummonUnitBuyNpcDlg/NSummonUnitBuyNpcDlg.h"
#include "../NInterface/NMySummonDlg/NMySummonDlg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Summon.h"

// 소환수를 풀어준다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONFREE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonFree* pclinfo = (cltGameMsgResponse_SummonFree*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	
	cltSummonInfo *pclSummonInfo = &pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
	cltKindInfo*	pclKI			=	NULL	;
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		//pclChar->pclCI->clSummonInfo.SummonInit(pclinfo->siSummonIndex);
		cltSummon* pclSummon = pclChar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
		
		if(pclSummon != NULL)	{
			// 소환수를 풀어 줐으니 Base쪽도 삭제 해준다.
			for ( SI16 siSummonIndex = 0 ; siSummonIndex < MAX_HAVE_SUMMON; siSummonIndex++){
				cltSimpleSummon *pclSummonBase = pclSummonInfo->GetSummonBase(siSummonIndex);
				if(pclSummonBase)
				{
					SI32 siKind	= pclSummonBase->siKind;
					// 해당 소환수의 카인드 값이 정상 적인지 체크.
					if( pclClient->pclKindInfoSet->IsValidKind( siKind ) == true )
					{
						if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
						{	
							pclKI = pclClient->pclKindInfoSet->pclSummonKI[ siKind ];	
						}
						else
						{
							pclKI = pclClient->pclKindInfoSet->pclKI[ siKind ];
						}
						if( pclKI == NULL )	continue;
						if( pclSummonBase->siSummonID	== pclinfo->siSummonID )
						{
							pclSummonBase->Init();
						}

					}
				}
			}
			pclSummon->Init();
			if ( ((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ])) != NULL ) 
			{
				((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyOutSummon();
				((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyhaveSummonList();
			}
		}
		SI32 siSummonIndex = pclChar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

		// 밖으로 나온 소환수정보가 지워진다면 갖고 있는 정보를 지운다.
		if(pclChar->siSummonIndex == siSummonIndex)
		{
			pclChar->clIdentity.Init();

			// 캐릭터가 제거되었으므로 한번 Clear 시킨다.
			pclChar->siChildCharUnique	= 0;
			pclChar->siSummonIndex		= 0;
		}

		if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
		{
			((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
		}

		// 움직이는 캐릭터를 사용자로 맞춘다.
		pclChar->siSelectedCharIndex = -1 ;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONNAMECHANGE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonNameChange* pclinfo = (cltGameMsgResponse_SummonNameChange*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];

	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		cltSummon *pclSummon = pclChar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
		if(pclSummon == NULL) return;
	
		MStrCpy(pclSummon->szName, pclinfo->strSummonName, MAX_SUMMON_NAME);
		
		SI32 siSummonID = pclChar->GetSummonID();
		if(pclCM->IsValidID(siSummonID))
		{
			((cltCharClient*)pclCM->CR[siSummonID])->SetName(pclSummon->szName);
			((cltCharClient*)pclCM->CR[siSummonID])->MakeCharName( NULL );
		}
		
		if ( pclinfo->siCharUnique == pclCM->CR[1]->GetCharUnique() )
		{
			TCHAR* pTitle = GetTxtFromMgr(1397);
			TCHAR* pText = GetTxtFromMgr(1398);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclinfo->strSummonName );

			if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
			{
				((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
			}
		}
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Summon* pclinfo = (cltGameMsgResponse_Summon*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		cltSummon *pclSummon = pclChar->pclCI->clSummonInfo.GetSummon(pclinfo->siSummonIndex);
		if(pclSummon)	pclSummon->Set(&pclinfo->clSummon);
		
		if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
		{
			((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
		}
		
		//==========================================
		// 지금 소환수가 밖에 나와 있다면 그 소환수 정보를 강제로 Update 시킨다.
		//==========================================
		SI32 siSummonID = pclChar->GetSummonID();
		if(pclCM->IsValidID(siSummonID))
		{
			cltCharClient* pclSummonChar = (cltCharClient*)pclCM->CR[siSummonID];
			if( NULL != pclSummonChar)
			{
				if(pclSummonChar->clIdentity.clSummonInfo.siMySummonIndex == pclinfo->siSummonIndex)
				{
					pclSummon->Set(&pclinfo->clSummon);
					// 박기형 수정 : 2009.06.19  캐릭터매니져가 관리하는 목록에서 소환수에 대한 스테이터스를 변경 한했던 부분을 수정
					// 스테이터스외에 더 추가 해야 되는 것이 있는지 확인해 보야한다.
					pclSummonChar->pclCI->clIP.SetDex(pclSummon->clIP.GetDex());
					pclSummonChar->pclCI->clIP.SetStr(pclSummon->clIP.GetStr());
					pclSummonChar->pclCI->clIP.SetVit(pclSummon->clIP.GetVit());
					pclSummonChar->pclCI->clIP.SetHnd(pclSummon->clIP.GetHnd());
					pclSummonChar->pclCI->clIP.SetWis(pclSummon->clIP.GetWis());
					pclSummonChar->pclCI->clIP.SetLuk(pclSummon->clIP.GetLuk());
					pclSummonChar->pclCI->clIP.SetMag(pclSummon->clIP.GetMag());
				}

				if ( pclChar->siSelectedCharIndex != -1 && pclSummonChar->GetSummonIndex() != pclChar->siSelectedCharIndex )
				{
					pclChar->siSelectedCharIndex = -1 ;
				}
			}
		}
		else
		{
			pclChar->siSelectedCharIndex = -1 ;
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BASESUMMONINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BaseSummon* pclinfo = (cltGameMsgResponse_BaseSummon*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	
	if(id == 1)
	{
		cltCharClient* pclchar = pclCM->GetCharClient(id);
		if(pclchar == NULL)			return;
		if(pclinfo->siSummonIndex < 0 || pclinfo->siSummonIndex >= MAX_HAVE_SUMMON)		return;
		pclchar->pclCI->clSummonInfo.clSummonBase[pclinfo->siSummonIndex].Set(&pclinfo->clSummonBase);
		if ( ((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))) 
		{
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyOutSummon();
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyhaveSummonList();
		}		
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONSLOTIN(cltMsg* pclMsg, SI32 id){
	cltGameMsgResponse_SlotIn* pclinfo = (cltGameMsgResponse_SlotIn*)pclMsg->cData;
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclchar = pclCM->GetCharClient(id);
		if(pclchar == NULL)			return;
		if(pclinfo->siSLotIndex < 0 || pclinfo->siSLotIndex >= MAX_SUMMON_OWN)		return;

		pclchar->pclCI->clSummonInfo.clSummon[pclinfo->siSLotIndex].Set(&pclinfo->clSummon);
		pclchar->pclCI->clSummonInfo.clItemInfo[pclinfo->siSLotIndex].Set(&pclinfo->clSummonItem);
		if ( ((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))) 
		{
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyOutSummon();
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyhaveSummonList();
		}
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONSLOTOUT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SlotOut* pclinfo = (cltGameMsgResponse_SlotOut*)pclMsg->cData;
	if ( pclCM->IsValidID(id))
	{
		cltCharClient* pclchar = pclCM->GetCharClient(id);
		if(pclchar == NULL)			return;
		// 슬롯이 정당한가 확인
		UI08 siSummonIndex = pclinfo->siSummonIndex;
		if( siSummonIndex < 0 || siSummonIndex >= MAX_SUMMON_OWN)		return;
		// 해당 슬롯을 초기화 시킨다.
		pclchar->pclCI->clSummonInfo.clSummon[ siSummonIndex ].Init();
		pclchar->pclCI->clSummonInfo.clItemInfo[ siSummonIndex ].Init();
		if ( ((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))) 
		{
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyOutSummon();
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyhaveSummonList();
		}
	}

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONREVIVE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonRevive* pclinfo = (cltGameMsgResponse_SummonRevive*)pclMsg->cData;
	
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		if(pclinfo->siResult > 0)
		{
		
		}
		else
		{
		
			PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			switch(pclinfo->siResult)
			{
			case -1:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1400);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			default:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1401);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			}
		}
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONATTACKCHANGE(cltMsg* pclMsg, SI32 id)
{
	
	cltGameMsgResponse_SummonAttackChange* pclinfo = (cltGameMsgResponse_SummonAttackChange*)pclMsg->cData;
	
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		if(pclinfo->siResult > 0)
		{
			PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0);
			cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
			//==========================================
			// 주인이 갖고 있는 소환수의 정보를 설정한다.
			//==========================================
			cltSummon *pclSummon = pclChar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
			if(pclSummon)	pclSummon->siAttackType = pclinfo->siAttackType;
		}
		else
		{
			PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			switch(pclinfo->siResult)
			{
			default:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1402);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			}
		}
	}

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONEVOLVE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonEvolve* pclinfo = (cltGameMsgResponse_SummonEvolve*)pclMsg->cData;
	
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		if(pclinfo->siResult > 0)
		{
		}
		else
		{
			PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			switch(pclinfo->siResult)
			{
			case -1:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1404);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			default:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1405);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			}
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONSCROLL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonScroll* pclinfo = (cltGameMsgResponse_SummonScroll*)pclMsg->cData;
	
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		if(pclinfo->siResult > 0)
		{	
		}
		else
		{	
			PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			switch(pclinfo->siResult)
			{
			case -1:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1407);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			default:
				{
					TCHAR* pTitle = GetTxtFromMgr(449);
					TCHAR* pText = GetTxtFromMgr(1408);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}break;
			}
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMON_SETAUTOHEAL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Summon_SetAutoHeal* pclinfo = (cltGameMsgResponse_Summon_SetAutoHeal*)pclMsg->cData;

	if ( pclinfo->siResult != 1 ) return ;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		pclCM->CR[id]->pclCI->clSummonInfo.bAutoHeal = pclinfo->bAutoHeal;
		pclCM->CR[id]->pclCI->clSummonInfo.siSelectAutoHeal = pclinfo->siSelectAutoHeal;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYSUMMONINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuySummonInfo *pclInfo = (cltGameMsgResponse_TradeMerchantBuySummonInfo*)pclMsg->cData;

	if(m_pDialog[ NSUMMONUNITBUYNPC_DLG ])
	{
		((CNSummonUnitBuyNpcDlg*) m_pDialog[ NSUMMONUNITBUYNPC_DLG ])->SetSummonPrice(pclInfo->siSummonID, pclInfo->siSummonPrice) ;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONTRADE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuySummon *pclInfo = (cltGameMsgResponse_TradeMerchantBuySummon*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclInfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
		//==========================================
		// 주인이 갖고 있는 소환수의 정보를 설정한다.
		//==========================================
		SI32 siSummonIndex = pclChar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclInfo->siSummonID);
		pclChar->pclCI->clSummonInfo.SummonInit(siSummonIndex);

		// 밖으로 나온 소환수정보가 지워진다면 갖고 있는 정보를 지운다.
		if(pclChar->siSummonIndex == siSummonIndex)
		{
			pclChar->clIdentity.Init();

			// 캐릭터가 제거되었으므로 한번 Clear 시킨다.
			pclChar->siChildCharUnique	= 0;
			pclChar->siSummonIndex		= 0;
		}

		pclChar->pclCI->clIP.SetMoney(pclInfo->siLeftMoney) ;

		if(m_pDialog[ NSUMMONUNITBUYNPC_DLG ])
		{
			((CNSummonUnitBuyNpcDlg*) m_pDialog[ NSUMMONUNITBUYNPC_DLG ])->SellToNPCSuccess();
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SELECT_SUMMONCHAR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Select_SummonChar* pclinfo = (cltGameMsgResponse_Select_SummonChar*)pclMsg->cData;

	pclCM->CR[1]->siSelectedCharIndex = pclinfo->siSelectIndex ;
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_SELECT_SUMMONUSESTON(cltMsg* pclMsg, SI32 id)
{

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMON_STPCHARGE( cltMsg* pclMsg, SI32 id )
{

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMON_TRANSFORM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Summon_Transform* pclinfo = (cltGameMsgResponse_Summon_Transform*)pclMsg->cData;

}