#include "Char/CharManager/CharManager.h"

#include "../../Client/Client.h"
#include "../../Client/NStructureDlg/Guild/MyGuild/MyGuildBase.h"
#include "../../Client/NStructureDlg/Guild/MyGuild/NBeGuildMemberDlg.h"

#include "../../Client/NStructureDlg/Guild/GuildDlg.h"
#include "../../Client/ninterface/NGuildInfoDlg/NGuildInfoDlg.h"
#include "../../Client/ninterface/NPlayMatchDlg/NGuildDungeonDlg.h"
#include "../../Client/NinterFace/NPlayMatchDlg/NGuildDungeonApplyDlg.h"
#include "../../Client/ninterface/NGuildInfoDlg/NGuildMemberDlg.h"
#include "../../Client/ninterface/NGuildWarApplyDlg/NGuildWarApply.h"
#include "../../Client/NStructureDlg/Guild/GuildWarRecommand/GuildWarRecommand.h"

#include "Guild/Guild.h"

#include "MsgType-Guild.h"
#include "../../Client/ninterface/NPlayMatchDlg_New/NGuildDungeonDlg_New.h"
#include "../../Client/ninterface/NPlayMatchDlg_New/NRequestHuntingMapWarDlg.h"
#include "../CommonLogic/Guild/GuildHuntMapWarStatMgr.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CREATEGUILD( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_CreateGuild * pclinfo = (cltGameMsgResponse_Guild_CreateGuild*)pclMsg->cData;

	if( pclinfo->siResult == 1)
	{
		pclCM->CR[1]->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_MASTER ;
		pclCM->CR[1]->pclCI->clBI.clGuildRank.siVillageUnique = pclinfo->clInfo.siVillageUnique ;
		pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex = pclinfo->clInfo.siUnique ;
		pclCM->CR[1]->pclCI->clBI.clGuildRank.Set(GUILDRANKTYPE_MASTER,pclinfo->clInfo.siVillageUnique,
			pclinfo->clInfo.siUnique,pclinfo->clInfo.szName,pclinfo->clInfo.uiConfirm,pclinfo->clInfo.clGPoint.siMarkIndex);

		TCHAR* pTitle = GetTxtFromMgr(6527);
		TCHAR* pText = GetTxtFromMgr(6544);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);

		cltGameMsgRequest_Guild_GetGuildList clinfo(pclinfo->clInfo.siVillageUnique);
		cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDLIST,sizeof(clinfo),(BYTE*)&clinfo);
		pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else if ( pclinfo->siResult == -2 ) // 동일 길드명으로 생성 실패
	{
		TCHAR* pTitle = GetTxtFromMgr(6605);
		TCHAR* pText = GetTxtFromMgr(6604);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_DESTROYGUILD( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_DestroyGuild * pclinfo = (cltGameMsgResponse_Guild_DestroyGuild*)pclMsg->cData;
	if ( pclinfo->siResult == 1) {
		switch( pclinfo->siReason ) {
			case 4 :
				{
					if(pclClient->IsCountrySwitch(Switch_Guild_DestroyOverMOnth)){
						// 휴면길드 삭제 메세지
						TCHAR* pTitle = GetTxtFromMgr(6621);
						TCHAR* pText = GetTxtFromMgr(6627);

						m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
					}
				}
				break;
			default:
				{
					TCHAR* pTitle = GetTxtFromMgr(6621);
					TCHAR* pText = GetTxtFromMgr(6627);
					//pclCM->CR[1]->pclCI->clBI.clGuildRank.Init() ;

					m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
				}
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_JOINGUILD( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_JoinGuild * pclinfo = (cltGameMsgResponse_Guild_JoinGuild*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clBI.clGuildRank.Set(GUILDRANKTYPE_STANDING,pclinfo->clInfo.siVillageUnique,
		pclinfo->clInfo.siUnique,pclinfo->clInfo.szName,pclinfo->clInfo.uiConfirm,pclinfo->clInfo.clGPoint.siMarkIndex);

	TCHAR* pTitle = GetTxtFromMgr(6524);
	TCHAR* pText = GetTxtFromMgr(6568);

	m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CONFIRMJOINGUILD( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_ConfirmJoinGuild * pclinfo = (cltGameMsgResponse_Guild_ConfirmJoinGuild*)pclMsg->cData;

	pclGuildManager->clCommonUnit.Set(&pclinfo->clInfo);

	if ( m_pDialog[NBEGUILDMEMBER_DLG] != NULL)
		((NBeGuildMemberDlg*)m_pDialog[NBEGUILDMEMBER_DLG])->RefreshList();
	if ( m_pDialog[NMYGUILDBASE_DLG] != NULL)
		((CNMyGuildBase*)m_pDialog[NMYGUILDBASE_DLG])->RefreshMyGuildMemberList(pclinfo->clInfo);

	if ( m_pDialog[NGUILDSTR_DLG] != NULL )
	{
		((NGuildDlg*)m_pDialog[NGUILDSTR_DLG])->RefreshAllList();
	}

	if ( pclinfo->bConfirm )
	{
	}
	else
	{
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_LEAVEGUILD( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_LeaveGuild* pclinfo = (cltGameMsgResponse_Guild_LeaveGuild*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clBI.clGuildRank.Init();
	// 길드탈퇴시 길드 스태이터스 초기화
	pclCM->CR[1]->clPB.clGuildBA.Init();

	TCHAR* pTitle = GetTxtFromMgr(6525);
	TCHAR* pText = GetTxtFromMgr(6569);

	m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGEVILLAGE( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_ChangeVillage* pclinfo = (cltGameMsgResponse_Guild_ChangeVillage*)pclMsg->cData;

	TCHAR* pTitle = GetTxtFromMgr(6619);
	TCHAR* pText = GetTxtFromMgr(6628);

	m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);

	cltGameMsgRequest_Guild_GetGuildList clinfo(pclinfo->clInfo.siMoveVillageUnique);
	cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDLIST,sizeof(clinfo),(BYTE*)&clinfo);
	pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_GETGUILDLIST( cltMsg*pclMsg,SI32 id )
{
	cltGameMsgResponse_Guild_GetGuildList* pclinfo = (cltGameMsgResponse_Guild_GetGuildList*)pclMsg->cData;

	
	if(pclinfo->siInterfaceFlag == NGUILDINFO_DLG )
	{
		if ( m_pDialog[NGUILDINFO_DLG] != NULL )
			((CNGuildInfoDlg*)m_pDialog[NGUILDINFO_DLG])->SetList( pclinfo );
	}
	else
	{
		if ( m_pDialog[NGUILDSTR_DLG] != NULL )
		{
			for ( SI32 i = 0 ; i < MAX_GUILD_PER_VILLAGE ; i ++ )
			{
				pclGuildManager->clUnit[i].Init();
				pclGuildManager->clUnit[i].siUnique = pclinfo->siUnique[i];
				pclGuildManager->clUnit[i].clGPoint.siLevel = pclinfo->siLevel[i];
				MStrCpy(pclGuildManager->clUnit[i].szName,pclinfo->szName[i],MAX_GUILD_NAME);
				MStrCpy(pclGuildManager->clUnit[i].clMember[0].szCharName,pclinfo->szMasterName[i],MAX_PLAYER_NAME);
				pclGuildManager->clUnit[i].uiConfirm = pclinfo->uiConfirm[i];
				pclGuildManager->clUnit[i].siVillageUnique = pclinfo->siVillageUnique[i];
				pclGuildManager->clUnit[i].siMoveVillageUnique = pclinfo->siMoveVillageUnique[i];

				((NGuildListDlg*)((NGuildDlg*)m_pDialog[NGUILDSTR_DLG])->m_pUserChildDlg[0])->siMemberNum[i] = pclinfo->siMemberNum[i] ;
			}

			((NGuildDlg*)m_pDialog[NGUILDSTR_DLG])->RefreshAllList();
		}
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_GETGUILDINFO(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_GetGuildInfo* pclinfo = (cltGameMsgResponse_Guild_GetGuildInfo*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		pclGuildManager->clCommonUnit.Set(&pclinfo->clInfo);

		if ( pclGuildManager->clCommonUnit.uiConfirm == GUILDSTATUS_STANDING )
		{
			TCHAR * pTitle = GetTxtFromMgr(6523);
			TCHAR * pText = GetTxtFromMgr(6533);
			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}

		CreateInterface(NMYGUILDBASE_DLG);

		if ( m_pDialog[NMYGUILDBASE_DLG] != NULL )
		{
			((CNMyGuildBase*)m_pDialog[NMYGUILDBASE_DLG])->RefreshMyGuildMemberList(pclinfo->clInfo);
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_ConfirmCreateGuild* pclinfo = (cltGameMsgResponse_Guild_ConfirmCreateGuild*)pclMsg->cData;

	if ( pclinfo->siResult )
	{
		if ( pclinfo->bConfirm )
		{
			TCHAR* pTitle = GetTxtFromMgr(6536);
			TCHAR* pText = GetTxtFromMgr(6570);

			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
		}
		else
		{
			TCHAR* pTitle = GetTxtFromMgr(6537);
			TCHAR* pText = GetTxtFromMgr(6571);

			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
		}

		cltGameMsgRequest_Guild_GetGuildList clinfo(pclinfo->clInfo.siVillageUnique);
		cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDLIST,sizeof(clinfo),(BYTE*)&clinfo);
		pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_SETSECONDMASTER(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_SetSecondMaster* pclinfo = (cltGameMsgResponse_Guild_SetSecondMaster*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		pclGuildManager->clCommonUnit.Set(&pclinfo->clInfo);

		if ( m_pDialog[NMYGUILDBASE_DLG] != NULL )
		{
			((CNMyGuildBase*)m_pDialog[NMYGUILDBASE_DLG])->RefreshMyGuildMemberList(pclinfo->clInfo);
		}

		if ( pclinfo->bSet )
		{
			TCHAR* pTitle = GetTxtFromMgr(6547);
			TCHAR* pText = GetTxtFromMgr(6545);

			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
		}
		else
		{
			TCHAR* pTitle = GetTxtFromMgr(6547);
			TCHAR* pText = GetTxtFromMgr(6546);

			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_GIVEGUILDMONEY(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_GiveGuildMoney* pclinfo = (cltGameMsgResponse_Guild_GiveGuildMoney*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		pclCM->CR[1]->pclCI->clIP.SetMoney(pclinfo->siLeftMoney);

		pclGuildManager->clCommonUnit.Set(&pclinfo->clInfo);

		if ( m_pDialog[NMYGUILDBASE_DLG] != NULL )
		{
			((CNMyGuildBase*)m_pDialog[NMYGUILDBASE_DLG])->RefreshMyGuildMemberList(pclinfo->clInfo);
		}

		TCHAR* pTitle = GetTxtFromMgr(6501);
		TCHAR* pText = GetTxtFromMgr(6543);
		
		TCHAR szBuffer[256] = {'\0',};

		StringCchPrintf(szBuffer,256,pText,pclinfo->siMoney);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,szBuffer);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_SETSTRFEERATE(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_SetStrFeeRate* pclinfo = (cltGameMsgResponse_Guild_SetStrFeeRate*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		TCHAR* pTitle = GetTxtFromMgr(6575);
		TCHAR* pText = GetTxtFromMgr(6576);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGESTATUS(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_ChangeStatus* pclinfo = (cltGameMsgResponse_Guild_ChangeStatus*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		cltGameMsgRequest_Guild_GetGuildInfo clInfo(pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex);
		cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDINFO, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

		TCHAR* pTitle = GetTxtFromMgr(6582);
		TCHAR* pText = GetTxtFromMgr(6583);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_SetGuildStatus* pclinfo = (cltGameMsgResponse_Guild_SetGuildStatus*)pclMsg->cData;

	SI32 varyid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(varyid <= 0)return ;

	if(pclCM->IsValidID(varyid))
	{
		if( pclinfo->clGuild.IsGuildMember() )
		{
			pclCM->CR[varyid]->clPB.clGuildBA.SetStr(pclinfo->clinfo.siStr);
			pclCM->CR[varyid]->clPB.clGuildBA.SetDex(pclinfo->clinfo.siDex);
			pclCM->CR[varyid]->clPB.clGuildBA.SetMag(pclinfo->clinfo.siMag);
			pclCM->CR[varyid]->clPB.clGuildBA.SetVit(pclinfo->clinfo.siVit);
			pclCM->CR[varyid]->clPB.clGuildBA.SetHnd(pclinfo->clinfo.siHnd);
			pclCM->CR[varyid]->clPB.clGuildBA.SetLuk(pclinfo->clinfo.siLuk);
			pclCM->CR[varyid]->clPB.clGuildBA.SetWis(pclinfo->clinfo.siWis);

			pclCM->CR[varyid]->pclCI->clBI.clGuildRank.Set(&pclinfo->clGuild);
		}
		else
		{
			// 길드 가입안되면 길드 정보나 길드 스테이터스 다 초기화 해버려!
			pclCM->CR[varyid]->pclCI->clBI.clGuildRank.Init();
			pclCM->CR[varyid]->clPB.clGuildBA.Init();
		}
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_SETMARK(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_SetMark* pclinfo = (cltGameMsgResponse_Guild_SetMark*)pclMsg->cData ;

	if ( pclinfo->siResult == 1 )
	{
		TCHAR buffer[128] = "" ;
		TCHAR* pTitle = GetTxtFromMgr(6578);
		TCHAR* pText = GetTxtFromMgr(6579);
		sprintf(buffer,pText,pclinfo->siMarkIndex);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,buffer);

		{
			cltGameMsgRequest_Guild_GetGuildInfo clInfo(pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex);
			cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGENAME(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_ChangeName* pclinfo = (cltGameMsgResponse_Guild_ChangeName*)pclMsg->cData ;

	if ( pclinfo->siResult == 1 )
	{
		TCHAR buffer[128] = "" ;
		TCHAR* pTitle = GetTxtFromMgr(6580);
		TCHAR* pText = GetTxtFromMgr(6581);
		sprintf(buffer,pText,pclinfo->szName);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,buffer);

		{
			cltGameMsgRequest_Guild_GetGuildInfo clInfo(pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex);
			cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		{
			cltGameMsgRequest_Guild_GetGuildList clinfo(pclCM->CR[1]->pclCI->clBI.clGuildRank.siVillageUnique);
			cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDLIST,sizeof(clinfo),(BYTE*)&clinfo);
			pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	else if ( pclinfo->siResult == -2 )
	{
		TCHAR* pTitle = GetTxtFromMgr(6606);
		TCHAR* pText = GetTxtFromMgr(6607);

		m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGEMASTER(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgResponse_Guild_SetSecondMaster* pclinfo = (cltGameMsgResponse_Guild_SetSecondMaster*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		pclGuildManager->clCommonUnit.Set(&pclinfo->clInfo);

		if ( m_pDialog[NMYGUILDBASE_DLG] != NULL )
		{
			((CNMyGuildBase*)m_pDialog[NMYGUILDBASE_DLG])->RefreshMyGuildMemberList(pclinfo->clInfo);
		}

		if ( pclinfo->bSet )
		{
			//KHY - 0708 - 길드장 위임 과련 메시지 수정.
			TCHAR* pTitle = GetTxtFromMgr(6648);
			TCHAR* pText = GetTxtFromMgr(7545);

//			TCHAR* pTitle = GetTxtFromMgr(6547);
//			TCHAR* pText = GetTxtFromMgr(6545);

			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
		}
		else
		{
			TCHAR* pTitle = GetTxtFromMgr(6547);
			TCHAR* pText = GetTxtFromMgr(8813);

			m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
		}
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_RESERVELIST( cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GuildWar_ReserveList* pclinfo = (cltGameMsgResponse_GuildWar_ReserveList*)pclMsg->cData;

	if ( m_pDialog[NGUILDSTR_DLG] == NULL )
		return;

	NGuildWarListDlg* pDialog = ((NGuildWarListDlg*)((NGuildDlg*)m_pDialog[NGUILDSTR_DLG])->m_pUserChildDlg[3]);

	for( SI32 i=0; i<GUILDWAR_ROOMNUM; i++)
		pDialog->m_clUnit[i].Set( &pclinfo->clGuildWarList[i] );

	pDialog->RefreshList();
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_APPLYRESERVE( cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GuildWar_ApplyReserve* pclinfo = (cltGameMsgResponse_GuildWar_ApplyReserve*)pclMsg->cData;

	TCHAR* pText = NULL;
	switch( pclinfo->siResult )
	{
	case GUILDWAR_APPLY_SUCCESS:
		pText = GetTxtFromMgr(6687);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0);
		break;
	case GUILDWAR_APPLY_FAILED_ALREADY_HOME:
		pText = GetTxtFromMgr(6688);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
		break;
	case GUILDWAR_APPLY_FAILED_ALREADY_AWAY:
		pText = GetTxtFromMgr(6689);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
		break;
	case GUILDWAR_APPLY_FAILED_FAULTNAME:
		pText = GetTxtFromMgr(6690);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
		break;
	case GUILDWAR_APPLY_FAILED_MYGUILD:
		pText = GetTxtFromMgr(6691);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
		break;
	case GUILDWAR_APPLY_FAILED_LOWLEVEL:
		pText = GetTxtFromMgr(6692);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
		break;
	case GUILDWAR_APPLY_FAILED_UNKNOWN:
		pText = GetTxtFromMgr(6690);
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
		break;
	}

	TCHAR* pTitle = GetTxtFromMgr(6694);

	((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

	cltGameMsgRequest_GuildWar_ReserveList clinfo(pclinfo->siFieldIndex);
	cltMsg clMsg(GAMEMSG_REQUEST_GUILDWAR_RESERVELIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_APPLYTOENEMY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GuildWar_ApplyToEnemy* pclinfo = (cltGameMsgResponse_GuildWar_ApplyToEnemy*)pclMsg->cData;

	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->CreateInterface( NGUILDWARAPPLY_DLG );

	CNGuildWarApplyDlg* pDialog = (CNGuildWarApplyDlg*)pclclient->m_pDialog[NGUILDWARAPPLY_DLG];
	if ( pDialog )
	{
        pDialog->SetExplain( pclinfo );
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_START( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildWar_Start* pclinfo = (cltGameMsgResponse_GuildWar_Start*)pclMsg->cData;

	pclGuildWarClient->StartGuildWar( pclinfo->szHomeGuildName, pclinfo->szAwayGuildName,pclinfo->siHomeGuildMarkIndex, pclinfo->siAwayGuildMarkIndex );
	if ( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar ) )
	{
		if ( pclClient->pclCM->IsValidID(id)) // hp를 채운다.
		{
			cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[ id ];
			pclChar->ParametaBoxAction()	;
			SI32 siLife		=	pclChar->clPB.GetMaxLife()	;
			pclChar->IncreaseLife( siLife )		;
		}
	}
	
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_START( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuilHuntMapdWar_Start* pclinfo = (cltGameMsgResponse_GuilHuntMapdWar_Start*)pclMsg->cData;

	if ( pclClient->pclCM->IsValidID(id)) // hp를 채운다.
	{
		cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[ id ];
		pclChar->ParametaBoxAction()	;
		SI32 siLife		=	pclChar->clPB.GetMaxLife()	;
		pclChar->IncreaseLife( siLife )		;
	}
	pclClient->m_pclGuildHuntMapWarStatMgr->StartGuildWar(pclinfo->szHomeGuildName, pclinfo->szAwayGuildName,pclinfo->siHomeGuildMarkIndex, pclinfo->siAwayGuildMarkIndex);
	//pclGuildWarClient->StartGuildWar( pclinfo->szHomeGuildName, pclinfo->szAwayGuildName,pclinfo->siHomeGuildMarkIndex, pclinfo->siAwayGuildMarkIndex );
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_ADDKILLPOINT( cltMsg* pclMsg, SI32 id )// 킬포인트 추가
{
	cltGameMsgResponse_GuilHuntMapdWar_ADDKillPoint* pclinfo = (cltGameMsgResponse_GuilHuntMapdWar_ADDKillPoint*)pclMsg->cData;

	//	pclinfo->siHomeAway가 죽인팀의 인덱스가 넘어오는것이 아니라 죽은팀의 인덱스가 넘어오는것이므로 siHomeAway값 swiching

	SI32 siHomeAway = 0;

	if(pclinfo->siHomeAway == 0)
		siHomeAway = 1;
	else if(pclinfo->siHomeAway == 1)
		siHomeAway = 0;
	else
		_ASSERT(0);

	m_pclGuildHuntMapWarStatMgr->SetPersonKillInfo(pclinfo->szKillerName,pclinfo->szKilledName, siHomeAway);
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_MAPUSERLIST( cltMsg* pclMsg, SI32 id )// 맵유저 리스트 추가
{
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_SENDMATCHINGRESULT( cltMsg* pclMsg, SI32 id )// 매칭결과를 각 길드원들에겍 알려준다.
{
	cltGameMsgResponse_GuilHuntMapdWar_SendMatchingResult* pclinfo = (cltGameMsgResponse_GuilHuntMapdWar_SendMatchingResult*)pclMsg->cData;

	TCHAR* pTitle = TEXT("");
	NTCHARString256	strText( GetTxtFromMgr(10330) );

	strText.Replace( TEXT("#enemyguild#"),		pclinfo->szEnemyName );
	((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, strText);
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_END( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildWar_End* pclinfo = (cltGameMsgResponse_GuildWar_End*)pclMsg->cData;

	TCHAR buffer[256];
	TCHAR* pText = NULL;
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
	{
		if ( pclinfo->siRecieveGP > 0 )	// 승리메세지
		{
			pText = GetTxtFromMgr( 9963 );
			StringCchPrintf( buffer, 256, pText, pclinfo->siRecieveGP);
		}
		else if ( pclinfo->siRecieveGP < 0) // 패배 메세지
		{
			pText = GetTxtFromMgr( 9964 );
			StringCchPrintf( buffer, 256, pText, -pclinfo->siRecieveGP);
		}
		else		// 비긴 메세지
		{
			pText = GetTxtFromMgr( 9965 );
			StringCchPrintf( buffer, 256, pText, pclinfo->siRecieveGP);
		}
	}
	else
	{
		if ( pclinfo->siRecieveGP > 0 )	// 승리메세지
		{
			pText = GetTxtFromMgr( 6702 );
			StringCchPrintf( buffer, 256, pText, pclinfo->siRecieveGP);
		}
		else if ( pclinfo->siRecieveGP < 0) // 패배 메세지
		{
			pText = GetTxtFromMgr( 6703 );
			StringCchPrintf( buffer, 256, pText, -pclinfo->siRecieveGP);
		}
		else		// 비긴 메세지
		{
			pText = GetTxtFromMgr( 6701 );
			StringCchPrintf( buffer, 256, pText, pclinfo->siRecieveGP);
		}

	}

	TCHAR* pTitle = GetTxtFromMgr( 6704 );
	((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);

	pclGuildWarClient->EndGuildWar(pclinfo->siRecieveGP);
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_END( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuilHuntMapdWar_End	* pclinfo = (cltGameMsgResponse_GuilHuntMapdWar_End	*)pclMsg->cData;

	for(int i = 0; i < MAX_GUILD_MEMBER_NUM; i++)
	{
		pclClient->m_pclGuildHuntMapWarStatMgr->EndGuildWar(pclinfo->siResult, 
															0, 
															pclinfo->pEndData_Home[i].GetPlayer(), 
															pclinfo->pEndData_Home[i].GetKillPoint(), 
															pclinfo->pEndData_Home[i].GetiDieNum());

		pclClient->m_pclGuildHuntMapWarStatMgr->EndGuildWar(pclinfo->siResult, 
															1, 
															pclinfo->pEndData_Away[i].GetPlayer(), 
															pclinfo->pEndData_Away[i].GetKillPoint(), 
															pclinfo->pEndData_Away[i].GetiDieNum());
	}
	
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_KILLPOINT( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildWar_KillPoint* pclinfo = (cltGameMsgResponse_GuildWar_KillPoint*)pclMsg->cData;

	pclGuildWarClient->m_siHomeGuildPoint = pclinfo->siHomeGuildKill;
	pclGuildWarClient->m_siAwayGuildPoint = pclinfo->siAwayGuildKill;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDDUNGEON_APPLYLIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildDungeon_ApplyList* pclinfo = (cltGameMsgResponse_GuildDungeon_ApplyList*)pclMsg->cData;

	cltClient* pclclient = (cltClient*)pclClient;

	CNGuildDungeonApplyDlg* pDialog = ((CNGuildDungeonApplyDlg*)pclclient->m_pDialog[NGUILDDUNGEONAPPLY_DLG]);

	if ( pDialog )
	{
		pDialog->SetApplyList(pclinfo);
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDDUNGEON_APPLY(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildDungeon_Apply* pclinfo = (cltGameMsgResponse_GuildDungeon_Apply*)pclMsg->cData;
	cltClient* pclclient = (cltClient*)pclClient;

	if ( pclinfo->bSuccess )
	{
		CNGuildDungeonApplyDlg* pDialog = ((CNGuildDungeonApplyDlg*)pclclient->m_pDialog[NGUILDDUNGEONAPPLY_DLG]);

		if ( pDialog )
		{
			pDialog->RefreshApplyList();
		}

		TCHAR *pTitle = GetTxtFromMgr(6739);
		TCHAR *pText = GetTxtFromMgr(6740);

		TCHAR buffer[256];

		StringCchPrintf( buffer, 256, pText, pclclient->pclMapManager->GetMapName( pclinfo->siMapIndex ) );

		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0);
	}
	else
	{
		TCHAR *pTitle = GetTxtFromMgr(6739);
		TCHAR *pText = GetTxtFromMgr(6748);
		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pText );
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILED")), 0, 0);
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDDUNGEON_STATUS(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildDungeon_Status* pclinfo = (cltGameMsgResponse_GuildDungeon_Status*)pclMsg->cData;

	cltClient* pclclient = (cltClient*)pclClient;
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )// 예전꺼면 
	{
		CNGuildDungeonDlg* pDialog = (CNGuildDungeonDlg*)pclclient->m_pDialog[NGUILDDUNGEON_NEW_DLG];

		if ( pDialog )
		{
			pDialog->SetList(pclinfo);
		}		
	}
	else
	{
		CNGuildDungeonDlg* pDialog = (CNGuildDungeonDlg*)pclclient->m_pDialog[NGUILDDUNGEON_DLG];

		if ( pDialog )
		{
			pDialog->SetList(pclinfo);
		}
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_MEMBERLIST(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Guild_MemeberList* pclinfo = (cltGameMsgResponse_Guild_MemeberList*)pclMsg->cData;

	cltClient* pclclient = (cltClient*)pclClient;

	CNGuildMemberDlg* pDialog = ((CNGuildMemberDlg*)pclclient->m_pDialog[ NGUILDMEMBER_DLG ]);

	if ( pDialog )
	{
		pDialog->SetMemberList( pclinfo );
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILD_GETVILLAGELIST(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgReponse_Guild_VillageList* pclinfo = (cltGameMsgReponse_Guild_VillageList*)pclMsg->cData;

	cltClient* pclclient = (cltClient*)pclClient;

	CNGuildInfoDlg* pDialog = ((CNGuildInfoDlg*)pclclient->m_pDialog[ NGUILDINFO_DLG ] );

	if ( pDialog )
	{
		pDialog->SetComboBoxItem(pclinfo);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDWAR_RECOMMANDLIST(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildWar_RecommandList* pclinfo = (cltGameMsgResponse_GuildWar_RecommandList*)pclMsg->cData;

	if ( m_pDialog[NGUILDWARRECOMMAND_DLG] )
	{
		((CNGuildWarRecommand*)m_pDialog[NGUILDWARRECOMMAND_DLG])->SetGuildList(pclinfo);
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_TIEM(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildHuntMapWarWar_Time* pclinfo = (cltGameMsgResponse_GuildHuntMapWarWar_Time*)pclMsg->cData;

	if ( m_pDialog[NREQUESTHUNTMAPWARREQUEST] )
	{
		((CNRequestHuntingMapWarDlgDlg*)m_pDialog[NREQUESTHUNTMAPWARREQUEST])->SetTime( pclinfo->stServerTime);
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_HaveMap(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuildHuntMapWarWar_HaveMapIndex* pclinfo = (cltGameMsgResponse_GuildHuntMapWarWar_HaveMapIndex*)pclMsg->cData;

	if ( m_pDialog[NREQUESTHUNTMAPWARREQUEST] )
	{
		((CNRequestHuntingMapWarDlgDlg*)m_pDialog[NREQUESTHUNTMAPWARREQUEST])->SetButton( pclinfo->siMApIndex );
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_Top10(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_GuilHuntMapdWar_GetTopTen* pclinfo = (cltGameMsgResponse_GuilHuntMapdWar_GetTopTen*)pclMsg->cData;

	if ( m_pDialog[NREQUESTHUNTMAPWARREQUEST] )
	{
		((CNRequestHuntingMapWarDlgDlg*)m_pDialog[NREQUESTHUNTMAPWARREQUEST])->SetRankList(pclinfo);
	}
}